/*
 *	main.c
 *
 *	Copyright (c) 2008 CSIRO, Delft University of Technology.
 *
 *	This file is part of Darjeeling.
 *
 *	Darjeeling is free software: you can redistribute it and/or modify
 *	it under the terms of the GNU General Public License as published by
 *	the Free Software Foundation, either version 3 of the License, or
 *	(at your option) any later version.
 *
 *	Darjeeling is distributed in the hope that it will be useful,
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *	GNU General Public License for more details.
 *
 *	You should have received a copy of the GNU General Public License
 *	along with Darjeeling.  If not, see <http://www.gnu.org/licenses/>.
 */
#include "stdlib.h"
#include "stdio.h"
#include <unistd.h>

#include "common/parse_infusion.h"
#include "common/heap/heap.h"
#include "common/vmthread.h"
#include "common/infusion.h"
#include "common/vm.h"
#include "common/execution/execution.h"
#include "common/debug.h"

// included from build/generated
#include "base_native.h"
#include "base_definitions.h"
#include "darjeeling_native.h"
#include "virtualsense_native.h"

// TODO NB this should not be a global 'extern int' ffs :)
// ref_t is now  only 16-bits wide for everyone. Thus,  we need a base
// address to resolve ref_t references into 32-bits pointers.
// see linux/pointerwidth.h

char * ref_t_base_address;
static unsigned char mem[MEMSIZE];
static int16_t waiting_thread_id;
static int8_t command_id;
static int8_t execution_context_id;


static unsigned char virtual_sense_mem[MAX_DI_SIZE];
static unsigned char darjeeling_mem[MAX_DI_SIZE];
static unsigned char base_un_mem[MAX_DI_SIZE];
static unsigned char app_mem_blink[MAX_DI_SIZE];
static unsigned char app_mem_sense[MAX_DI_SIZE];


// load raw infusion file into memory
dj_di_pointer loadDI(char *fileName, unsigned char *mem)
{

	// TODO: error handling - but this is just for testing anyway

	FILE *diFile;
	long size = 0;
	char *bytes = mem;
	dj_di_pointer di;


	diFile = fopen(fileName,"rb");

	if (!diFile)
	{
		printf("Fatal: file not found: %s\n", fileName);
		exit(-1);
	}

	// get file size
	fseek(diFile,0,SEEK_END);
	size = ftell(diFile);
	fseek(diFile,0,SEEK_SET);

	// load the entire thing as one block
	//bytes = (char*)malloc(size);
	fread(bytes,1,size,diFile);

	// close file
	fclose(diFile);

	// cast the byte pointer to a flex file struct and dump its contents
	di = (dj_di_pointer)bytes;

	return di;

}

int main(int argc,char* argv[])
{
	uint8_t resume_from_ibernation = 0;
	int entryPointIndex;
	dj_vm *vm;
	dj_di_pointer di;
	dj_infusion *infusion;
	dj_thread *thread;
	dj_global_id entryPoint;
	uint16_t index = 0;

	// initialise memory manager
	//void *mem = malloc(MEMSIZE);

	dj_timer_init();
	// initialize the heap
	dj_mem_init(mem, MEMSIZE);
	ref_t_base_address = (char*)mem - 42;

    //load the VM from the restored heap
	//load the heap content from the ibernation file if found
	resume_from_ibernation = load_machine(mem);

	if(resume_from_ibernation){
		printf("Loading VM from ibernation\n");
		vm = dj_vm_load_from_heap(mem);
		loadDI("build/infusions/base.di", base_un_mem);
		loadDI("build/infusions/virtualsense.di", virtual_sense_mem);
		loadDI("build/infusions/darjeeling.di", darjeeling_mem);

		DEBUG_LOG("VM_POINTER %p\n", vm);
		DEBUG_LOG("heap left pointer %p\n", (dj_mem_get_base_pointer()+dj_mem_get_left_pointer()));
		DEBUG_LOG("heap right pointer %p\n", (dj_mem_get_base_pointer()+dj_mem_get_right_pointer()));
		DEBUG_LOG("current thread pointer %p\n", vm->currentThread);
		if(vm->currentThread != NULL)
			DEBUG_LOG("current thread id %d\n", vm->currentThread->id);

		if(argc>1)
			loadDI(argv[1], app_mem_blink);
		else
			loadDI("build/infusions/blink.di", app_mem_blink);
	}else { // we have to create a new VM
		printf("Creating a new VM\n");
    	vm = dj_vm_create();
	}


   // tell the execution engine to use the newly created VM instance
	dj_exec_setVM(vm);

	// load native infusion
	if(!resume_from_ibernation){
		di = loadDI("build/infusions/base.di", base_un_mem);
		infusion = dj_vm_loadSystemInfusion(vm, di);
		infusion->native_handler = base_native_handler;
		dj_vm_runClassInitialisers(vm, infusion);
		DEBUG_LOG("BASE infusion loaded at pointer %p\n", infusion);

		// load infusion files
		di = loadDI("build/infusions/darjeeling.di", darjeeling_mem);
		infusion = dj_vm_loadInfusion(vm, di);
		infusion->native_handler = darjeeling_native_handler;
		dj_vm_runClassInitialisers(vm, infusion);
		DEBUG_LOG("DARJEELING infusion loaded at pointer %p\n", infusion);

		// load infusion files
		di = loadDI("build/infusions/virtualsense.di", virtual_sense_mem);
		infusion = dj_vm_loadInfusion(vm, di);
		infusion->native_handler = virtualsense_native_handler;
		dj_vm_runClassInitialisers(vm, infusion);
		DEBUG_LOG("VIRTUALSENSE infusion loaded at pointer %p position %d\n", infusion, dj_vm_getInfusionId(vm, infusion) );



/*		if(argc>1)
			di = loadDI(argv[1], app_mem);
		else
			di=loadDI("build/infusions/blink.di", app_mem);

		infusion = dj_vm_loadInfusion(vm, di);
		dj_vm_runClassInitialisers(vm, infusion);
*/
		// pre-allocate an OutOfMemoryError object
		dj_object *obj = dj_vm_createSysLibObject(vm, BASE_CDEF_java_lang_OutOfMemoryError);
		dj_mem_setPanicExceptionObject(obj);

		// find the entry point for the infusion
		if ((entryPointIndex = dj_di_header_getEntryPoint(infusion->header))==255)
		{
			printf("No entry point found\n");
			return 0;
		} else
		{
			DEBUG_LOG("ENtry point index is %d\n", entryPointIndex);
			entryPoint.infusion = infusion;
			entryPoint.entity_id = entryPointIndex;
		}

		// create a new thread and add it to the VM
		thread = dj_thread_create_and_run(entryPoint,0);
		dj_vm_addThread(vm, thread);

	}
	DEBUG_LOG("Starting the main execution loop\n");
    // start the main execution loop


	while (dj_vm_countLiveThreads(vm)>0)
	{
		dj_vm_schedule(vm);
		if (vm->currentThread!=NULL)
			if (vm->currentThread->status==THREADSTATUS_RUNNING)
				dj_exec_run(RUNSIZE);
		usleep(50);
		//printf("------> %d\n", index);
		index++;
		if(index == 100){
			DEBUG_LOG("SEND A LOAD COMMAND \n");
			wake_up_waiting_thread(0,1);
			//index = 0;
		}
		if(index == 1000){
			DEBUG_LOG("SEND A START COMMAND\n");
			wake_up_waiting_thread(1,1);
					//index = 0;
		}
		if(index == 2000){
			DEBUG_LOG("SEND A LOAD COMMAND \n");
			wake_up_waiting_thread(0,2);
			//index = 0;
		}
		if(index == 3000){
			DEBUG_LOG("SEND A START COMMAND\n");
			wake_up_waiting_thread(1,2);
			//index = 0;
		}
		if(index == 10000){
			DEBUG_LOG("SEND A STOP COMMAND\n");
			wake_up_waiting_thread(2,2);
			//index = 0;
		}
	}

	dj_vm_schedule(vm);
	dj_mem_gc();
	printf("VM exit\n");
	return 0;
}

void wake_up_waiting_thread(uint8_t c_id, uint8_t ex_id){
	dj_thread *w_thread;
	w_thread = dj_vm_getThreadById(dj_exec_getVM(), waiting_thread_id);
	// put message on the buffer
	command_id = c_id;
	execution_context_id = ex_id;
	if(w_thread != nullref){
			w_thread->status = THREADSTATUS_RUNNING;
			// we need to ensure that this thread will take the CPU before other running thread
			w_thread->need_resched = 1;
			command_id = c_id;
			execution_context_id = ex_id;

	}
}

void waiting_thread_init(int16_t id){
		// save the waiting  thread id.
		waiting_thread_id = id;
}

dj_infusion *load_external_infusion(int16_t infusionID){
		dj_di_pointer di;
		dj_infusion *infusion;
		// load infusion file
		DEBUG_LOG("Loading infusion id %d\n", infusionID);
		if(infusionID == 1)
			di = loadDI("build/infusions/blink_multi_user.di", app_mem_blink);
		if(infusionID == 2)
			di = loadDI("build/infusions/sense_multi_user.di", app_mem_sense);
		infusion = dj_vm_loadInfusion(dj_exec_getVM(), di);
		//infusion->native_handler = nullref; // we do not allow Task to use native methods !!!!
		return infusion;
}


int8_t get_command_id(){
	return command_id;
}
int8_t get_execution_context_id(){
	return execution_context_id;
}


