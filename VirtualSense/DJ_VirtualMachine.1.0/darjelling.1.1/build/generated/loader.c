#include "loader.h"
#include "common/parse_infusion.h"
#include "common/infusion.h"
#include "common/vmthread.h"

// infusion files
#include "base_di.h"
#include "darjeeling_di.h"
#include "sense_di.h"
// native stubs for infusions
#include "base_native.h"
#include "darjeeling_native.h"

void dj_loadEmbeddedInfusions(dj_vm * vm)
{
	dj_infusion * infusion;
	dj_thread * thread;
	dj_global_id entryPoint;
	uint16_t entryPointIndex;

	// base
	infusion = dj_vm_loadSystemInfusion(vm, (dj_di_pointer)base_di);
	infusion->native_handler = base_native_handler;
	dj_vm_runClassInitialisers(vm, infusion);
	// darjeeling
	infusion = dj_vm_loadInfusion(vm, (dj_di_pointer)darjeeling_di);
	infusion->native_handler = darjeeling_native_handler;
	dj_vm_runClassInitialisers(vm, infusion);
	// sense
	infusion = dj_vm_loadInfusion(vm, (dj_di_pointer)sense_di);
	dj_vm_runClassInitialisers(vm, infusion);
	if ((entryPointIndex=dj_di_header_getEntryPoint(infusion->header))!=255)
	{
		entryPoint.infusion = infusion;
		entryPoint.entity_id = entryPointIndex;
		thread = dj_thread_create_and_run(entryPoint);
		dj_vm_addThread(vm, thread);
	}

}

