;******************************************************************************
;* MSP430 C/C++ Codegen                                             PC v3.3.2 *
;* Date/Time created: Tue Oct 04 00:37:10 2011                                *
;******************************************************************************
	.compiler_opts --hll_source=on --mem_model:code=small --mem_model:data=small --object_format=coff --silicon_version=msp --symdebug:dwarf --symdebug:dwarf_version=2 

$C$DW$CU	.dwtag  DW_TAG_compile_unit
	.dwattr $C$DW$CU, DW_AT_name("../../core/net/rime/rimestats.c")
	.dwattr $C$DW$CU, DW_AT_producer("MSP430 C/C++ Codegen PC v3.3.2 Copyright (c) 2003-2010 Texas Instruments Incorporated")
	.dwattr $C$DW$CU, DW_AT_TI_version(0x01)
	.dwattr $C$DW$CU, DW_AT_comp_dir("D:\Documenti\WORK\Sensori\VirtualSens\contiki-2.5\examples\smartRF")
	.global	rimestats
	.bss	rimestats,72,2
$C$DW$1	.dwtag  DW_TAG_variable, DW_AT_name("rimestats")
	.dwattr $C$DW$1, DW_AT_TI_symbol_name("rimestats")
	.dwattr $C$DW$1, DW_AT_location[DW_OP_addr rimestats]
	.dwattr $C$DW$1, DW_AT_type(*$C$DW$T$19)
	.dwattr $C$DW$1, DW_AT_external
;	opt430 C:\\Users\\Lattanzi\\AppData\\Local\\Temp\\053203 C:\\Users\\Lattanzi\\AppData\\Local\\Temp\\053205 

;******************************************************************************
;* TYPE INFORMATION                                                           *
;******************************************************************************
$C$DW$T$4	.dwtag  DW_TAG_base_type
	.dwattr $C$DW$T$4, DW_AT_encoding(DW_ATE_boolean)
	.dwattr $C$DW$T$4, DW_AT_name("bool")
	.dwattr $C$DW$T$4, DW_AT_byte_size(0x01)
$C$DW$T$5	.dwtag  DW_TAG_base_type
	.dwattr $C$DW$T$5, DW_AT_encoding(DW_ATE_signed_char)
	.dwattr $C$DW$T$5, DW_AT_name("signed char")
	.dwattr $C$DW$T$5, DW_AT_byte_size(0x01)
$C$DW$T$6	.dwtag  DW_TAG_base_type
	.dwattr $C$DW$T$6, DW_AT_encoding(DW_ATE_unsigned_char)
	.dwattr $C$DW$T$6, DW_AT_name("unsigned char")
	.dwattr $C$DW$T$6, DW_AT_byte_size(0x01)
$C$DW$T$7	.dwtag  DW_TAG_base_type
	.dwattr $C$DW$T$7, DW_AT_encoding(DW_ATE_signed_char)
	.dwattr $C$DW$T$7, DW_AT_name("wchar_t")
	.dwattr $C$DW$T$7, DW_AT_byte_size(0x02)
$C$DW$T$8	.dwtag  DW_TAG_base_type
	.dwattr $C$DW$T$8, DW_AT_encoding(DW_ATE_signed)
	.dwattr $C$DW$T$8, DW_AT_name("short")
	.dwattr $C$DW$T$8, DW_AT_byte_size(0x02)
$C$DW$T$9	.dwtag  DW_TAG_base_type
	.dwattr $C$DW$T$9, DW_AT_encoding(DW_ATE_unsigned)
	.dwattr $C$DW$T$9, DW_AT_name("unsigned short")
	.dwattr $C$DW$T$9, DW_AT_byte_size(0x02)
$C$DW$T$10	.dwtag  DW_TAG_base_type
	.dwattr $C$DW$T$10, DW_AT_encoding(DW_ATE_signed)
	.dwattr $C$DW$T$10, DW_AT_name("int")
	.dwattr $C$DW$T$10, DW_AT_byte_size(0x02)
$C$DW$T$11	.dwtag  DW_TAG_base_type
	.dwattr $C$DW$T$11, DW_AT_encoding(DW_ATE_unsigned)
	.dwattr $C$DW$T$11, DW_AT_name("unsigned int")
	.dwattr $C$DW$T$11, DW_AT_byte_size(0x02)
$C$DW$T$12	.dwtag  DW_TAG_base_type
	.dwattr $C$DW$T$12, DW_AT_encoding(DW_ATE_signed)
	.dwattr $C$DW$T$12, DW_AT_name("long")
	.dwattr $C$DW$T$12, DW_AT_byte_size(0x04)
$C$DW$T$13	.dwtag  DW_TAG_base_type
	.dwattr $C$DW$T$13, DW_AT_encoding(DW_ATE_unsigned)
	.dwattr $C$DW$T$13, DW_AT_name("unsigned long")
	.dwattr $C$DW$T$13, DW_AT_byte_size(0x04)
$C$DW$T$14	.dwtag  DW_TAG_base_type
	.dwattr $C$DW$T$14, DW_AT_encoding(DW_ATE_signed)
	.dwattr $C$DW$T$14, DW_AT_name("long long")
	.dwattr $C$DW$T$14, DW_AT_byte_size(0x04)
$C$DW$T$15	.dwtag  DW_TAG_base_type
	.dwattr $C$DW$T$15, DW_AT_encoding(DW_ATE_unsigned)
	.dwattr $C$DW$T$15, DW_AT_name("unsigned long long")
	.dwattr $C$DW$T$15, DW_AT_byte_size(0x04)
$C$DW$T$16	.dwtag  DW_TAG_base_type
	.dwattr $C$DW$T$16, DW_AT_encoding(DW_ATE_float)
	.dwattr $C$DW$T$16, DW_AT_name("float")
	.dwattr $C$DW$T$16, DW_AT_byte_size(0x04)
$C$DW$T$17	.dwtag  DW_TAG_base_type
	.dwattr $C$DW$T$17, DW_AT_encoding(DW_ATE_float)
	.dwattr $C$DW$T$17, DW_AT_name("double")
	.dwattr $C$DW$T$17, DW_AT_byte_size(0x04)
$C$DW$T$18	.dwtag  DW_TAG_base_type
	.dwattr $C$DW$T$18, DW_AT_encoding(DW_ATE_float)
	.dwattr $C$DW$T$18, DW_AT_name("long double")
	.dwattr $C$DW$T$18, DW_AT_byte_size(0x04)

$C$DW$T$19	.dwtag  DW_TAG_structure_type
	.dwattr $C$DW$T$19, DW_AT_name("rimestats")
	.dwattr $C$DW$T$19, DW_AT_byte_size(0x48)
$C$DW$2	.dwtag  DW_TAG_member
	.dwattr $C$DW$2, DW_AT_type(*$C$DW$T$13)
	.dwattr $C$DW$2, DW_AT_name("tx")
	.dwattr $C$DW$2, DW_AT_TI_symbol_name("tx")
	.dwattr $C$DW$2, DW_AT_data_member_location[DW_OP_plus_uconst 0x0]
	.dwattr $C$DW$2, DW_AT_accessibility(DW_ACCESS_public)
$C$DW$3	.dwtag  DW_TAG_member
	.dwattr $C$DW$3, DW_AT_type(*$C$DW$T$13)
	.dwattr $C$DW$3, DW_AT_name("rx")
	.dwattr $C$DW$3, DW_AT_TI_symbol_name("rx")
	.dwattr $C$DW$3, DW_AT_data_member_location[DW_OP_plus_uconst 0x4]
	.dwattr $C$DW$3, DW_AT_accessibility(DW_ACCESS_public)
$C$DW$4	.dwtag  DW_TAG_member
	.dwattr $C$DW$4, DW_AT_type(*$C$DW$T$13)
	.dwattr $C$DW$4, DW_AT_name("reliabletx")
	.dwattr $C$DW$4, DW_AT_TI_symbol_name("reliabletx")
	.dwattr $C$DW$4, DW_AT_data_member_location[DW_OP_plus_uconst 0x8]
	.dwattr $C$DW$4, DW_AT_accessibility(DW_ACCESS_public)
$C$DW$5	.dwtag  DW_TAG_member
	.dwattr $C$DW$5, DW_AT_type(*$C$DW$T$13)
	.dwattr $C$DW$5, DW_AT_name("reliablerx")
	.dwattr $C$DW$5, DW_AT_TI_symbol_name("reliablerx")
	.dwattr $C$DW$5, DW_AT_data_member_location[DW_OP_plus_uconst 0xc]
	.dwattr $C$DW$5, DW_AT_accessibility(DW_ACCESS_public)
$C$DW$6	.dwtag  DW_TAG_member
	.dwattr $C$DW$6, DW_AT_type(*$C$DW$T$13)
	.dwattr $C$DW$6, DW_AT_name("rexmit")
	.dwattr $C$DW$6, DW_AT_TI_symbol_name("rexmit")
	.dwattr $C$DW$6, DW_AT_data_member_location[DW_OP_plus_uconst 0x10]
	.dwattr $C$DW$6, DW_AT_accessibility(DW_ACCESS_public)
$C$DW$7	.dwtag  DW_TAG_member
	.dwattr $C$DW$7, DW_AT_type(*$C$DW$T$13)
	.dwattr $C$DW$7, DW_AT_name("acktx")
	.dwattr $C$DW$7, DW_AT_TI_symbol_name("acktx")
	.dwattr $C$DW$7, DW_AT_data_member_location[DW_OP_plus_uconst 0x14]
	.dwattr $C$DW$7, DW_AT_accessibility(DW_ACCESS_public)
$C$DW$8	.dwtag  DW_TAG_member
	.dwattr $C$DW$8, DW_AT_type(*$C$DW$T$13)
	.dwattr $C$DW$8, DW_AT_name("noacktx")
	.dwattr $C$DW$8, DW_AT_TI_symbol_name("noacktx")
	.dwattr $C$DW$8, DW_AT_data_member_location[DW_OP_plus_uconst 0x18]
	.dwattr $C$DW$8, DW_AT_accessibility(DW_ACCESS_public)
$C$DW$9	.dwtag  DW_TAG_member
	.dwattr $C$DW$9, DW_AT_type(*$C$DW$T$13)
	.dwattr $C$DW$9, DW_AT_name("ackrx")
	.dwattr $C$DW$9, DW_AT_TI_symbol_name("ackrx")
	.dwattr $C$DW$9, DW_AT_data_member_location[DW_OP_plus_uconst 0x1c]
	.dwattr $C$DW$9, DW_AT_accessibility(DW_ACCESS_public)
$C$DW$10	.dwtag  DW_TAG_member
	.dwattr $C$DW$10, DW_AT_type(*$C$DW$T$13)
	.dwattr $C$DW$10, DW_AT_name("timedout")
	.dwattr $C$DW$10, DW_AT_TI_symbol_name("timedout")
	.dwattr $C$DW$10, DW_AT_data_member_location[DW_OP_plus_uconst 0x20]
	.dwattr $C$DW$10, DW_AT_accessibility(DW_ACCESS_public)
$C$DW$11	.dwtag  DW_TAG_member
	.dwattr $C$DW$11, DW_AT_type(*$C$DW$T$13)
	.dwattr $C$DW$11, DW_AT_name("badackrx")
	.dwattr $C$DW$11, DW_AT_TI_symbol_name("badackrx")
	.dwattr $C$DW$11, DW_AT_data_member_location[DW_OP_plus_uconst 0x24]
	.dwattr $C$DW$11, DW_AT_accessibility(DW_ACCESS_public)
$C$DW$12	.dwtag  DW_TAG_member
	.dwattr $C$DW$12, DW_AT_type(*$C$DW$T$13)
	.dwattr $C$DW$12, DW_AT_name("toolong")
	.dwattr $C$DW$12, DW_AT_TI_symbol_name("toolong")
	.dwattr $C$DW$12, DW_AT_data_member_location[DW_OP_plus_uconst 0x28]
	.dwattr $C$DW$12, DW_AT_accessibility(DW_ACCESS_public)
$C$DW$13	.dwtag  DW_TAG_member
	.dwattr $C$DW$13, DW_AT_type(*$C$DW$T$13)
	.dwattr $C$DW$13, DW_AT_name("tooshort")
	.dwattr $C$DW$13, DW_AT_TI_symbol_name("tooshort")
	.dwattr $C$DW$13, DW_AT_data_member_location[DW_OP_plus_uconst 0x2c]
	.dwattr $C$DW$13, DW_AT_accessibility(DW_ACCESS_public)
$C$DW$14	.dwtag  DW_TAG_member
	.dwattr $C$DW$14, DW_AT_type(*$C$DW$T$13)
	.dwattr $C$DW$14, DW_AT_name("badsynch")
	.dwattr $C$DW$14, DW_AT_TI_symbol_name("badsynch")
	.dwattr $C$DW$14, DW_AT_data_member_location[DW_OP_plus_uconst 0x30]
	.dwattr $C$DW$14, DW_AT_accessibility(DW_ACCESS_public)
$C$DW$15	.dwtag  DW_TAG_member
	.dwattr $C$DW$15, DW_AT_type(*$C$DW$T$13)
	.dwattr $C$DW$15, DW_AT_name("badcrc")
	.dwattr $C$DW$15, DW_AT_TI_symbol_name("badcrc")
	.dwattr $C$DW$15, DW_AT_data_member_location[DW_OP_plus_uconst 0x34]
	.dwattr $C$DW$15, DW_AT_accessibility(DW_ACCESS_public)
$C$DW$16	.dwtag  DW_TAG_member
	.dwattr $C$DW$16, DW_AT_type(*$C$DW$T$13)
	.dwattr $C$DW$16, DW_AT_name("contentiondrop")
	.dwattr $C$DW$16, DW_AT_TI_symbol_name("contentiondrop")
	.dwattr $C$DW$16, DW_AT_data_member_location[DW_OP_plus_uconst 0x38]
	.dwattr $C$DW$16, DW_AT_accessibility(DW_ACCESS_public)
$C$DW$17	.dwtag  DW_TAG_member
	.dwattr $C$DW$17, DW_AT_type(*$C$DW$T$13)
	.dwattr $C$DW$17, DW_AT_name("sendingdrop")
	.dwattr $C$DW$17, DW_AT_TI_symbol_name("sendingdrop")
	.dwattr $C$DW$17, DW_AT_data_member_location[DW_OP_plus_uconst 0x3c]
	.dwattr $C$DW$17, DW_AT_accessibility(DW_ACCESS_public)
$C$DW$18	.dwtag  DW_TAG_member
	.dwattr $C$DW$18, DW_AT_type(*$C$DW$T$13)
	.dwattr $C$DW$18, DW_AT_name("lltx")
	.dwattr $C$DW$18, DW_AT_TI_symbol_name("lltx")
	.dwattr $C$DW$18, DW_AT_data_member_location[DW_OP_plus_uconst 0x40]
	.dwattr $C$DW$18, DW_AT_accessibility(DW_ACCESS_public)
$C$DW$19	.dwtag  DW_TAG_member
	.dwattr $C$DW$19, DW_AT_type(*$C$DW$T$13)
	.dwattr $C$DW$19, DW_AT_name("llrx")
	.dwattr $C$DW$19, DW_AT_TI_symbol_name("llrx")
	.dwattr $C$DW$19, DW_AT_data_member_location[DW_OP_plus_uconst 0x44]
	.dwattr $C$DW$19, DW_AT_accessibility(DW_ACCESS_public)
	.dwendtag $C$DW$T$19


;***************************************************************
;* DWARF CIE ENTRIES                                           *
;***************************************************************

$C$DW$CIE	.dwcie 16
	.dwcfi	cfa_register, 1
	.dwcfi	cfa_offset, 0
	.dwcfi	undefined, 0
	.dwcfi	undefined, 1
	.dwcfi	undefined, 2
	.dwcfi	undefined, 3
	.dwcfi	undefined, 4
	.dwcfi	undefined, 5
	.dwcfi	undefined, 6
	.dwcfi	undefined, 7
	.dwcfi	undefined, 8
	.dwcfi	undefined, 9
	.dwcfi	undefined, 10
	.dwcfi	undefined, 11
	.dwcfi	undefined, 12
	.dwcfi	undefined, 13
	.dwcfi	undefined, 14
	.dwcfi	undefined, 15
	.dwcfi	undefined, 16
	.dwendentry

;***************************************************************
;* DWARF REGISTER MAP                                          *
;***************************************************************

$C$DW$20	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("PC")
	.dwattr $C$DW$20, DW_AT_location[DW_OP_reg0]
$C$DW$21	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("SP")
	.dwattr $C$DW$21, DW_AT_location[DW_OP_reg1]
$C$DW$22	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("SR")
	.dwattr $C$DW$22, DW_AT_location[DW_OP_reg2]
$C$DW$23	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("CG")
	.dwattr $C$DW$23, DW_AT_location[DW_OP_reg3]
$C$DW$24	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("r4")
	.dwattr $C$DW$24, DW_AT_location[DW_OP_reg4]
$C$DW$25	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("r5")
	.dwattr $C$DW$25, DW_AT_location[DW_OP_reg5]
$C$DW$26	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("r6")
	.dwattr $C$DW$26, DW_AT_location[DW_OP_reg6]
$C$DW$27	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("r7")
	.dwattr $C$DW$27, DW_AT_location[DW_OP_reg7]
$C$DW$28	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("r8")
	.dwattr $C$DW$28, DW_AT_location[DW_OP_reg8]
$C$DW$29	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("r9")
	.dwattr $C$DW$29, DW_AT_location[DW_OP_reg9]
$C$DW$30	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("r10")
	.dwattr $C$DW$30, DW_AT_location[DW_OP_reg10]
$C$DW$31	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("r11")
	.dwattr $C$DW$31, DW_AT_location[DW_OP_reg11]
$C$DW$32	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("r12")
	.dwattr $C$DW$32, DW_AT_location[DW_OP_reg12]
$C$DW$33	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("r13")
	.dwattr $C$DW$33, DW_AT_location[DW_OP_reg13]
$C$DW$34	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("r14")
	.dwattr $C$DW$34, DW_AT_location[DW_OP_reg14]
$C$DW$35	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("r15")
	.dwattr $C$DW$35, DW_AT_location[DW_OP_reg15]
$C$DW$36	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("CIE_RETA")
	.dwattr $C$DW$36, DW_AT_location[DW_OP_reg16]
	.dwendtag $C$DW$CU

