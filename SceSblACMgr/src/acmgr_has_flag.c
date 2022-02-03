
#include <psp2kern/kernel/dipsw.h>
#include "imports.h"
#include "acmgr_has_flag.h"
#include "acmgr_is_program.h"

SceBool sceSblACMgrHasCapability(SceUID pid, SceUInt32 index){

	SceSelfAuthInfo auth_info;

	if(0 != ksceSysrootGetSelfAuthInfo(pid, &auth_info))
		return SCE_FALSE;

	return (auth_info.capability[index >> 3] >> ((~index) & 7)) & 1;
}

SceBool sceSblACMgrHasAttributes(SceUID pid, SceUInt32 index){

	SceSelfAuthInfo auth_info;

	if(0 != ksceSysrootGetSelfAuthInfo(pid, &auth_info))
		return SCE_FALSE;

	return (auth_info.attribute[index >> 3] >> ((~index) & 7)) & 1;
}

SceBool sceSblACMgrIsDevelopmentMode(void){
	return ksceKernelCheckDipsw(0x9F);
}

SceBool SceSblACMgrForKernel_48F4D5EE(SceUID pid){
	return sceSblACMgrHasCapability(pid, 0x1F);
}

SceBool sceSblACMgrHasUserDebugFlag(SceUID pid){
	return sceSblACMgrHasCapability(pid, 0x20);
}

// sceSblACMgrHasMemoryRemapFlag
SceBool SceSblACMgrForKernel_30575458(SceUID pid){

	if(SCE_FALSE != sceSblACMgrHasCapability(pid, 0x22))
		return SCE_TRUE;

	if(SCE_FALSE == sceSblACMgrIsFself(pid))
		return SCE_FALSE;

	if(0 != SceSysrootForDriver_26AA237C())
		return SCE_TRUE;

	return SCE_FALSE;
}

SceBool sceSblACMgrHasMarlinFlag(SceUID pid){
	return sceSblACMgrHasCapability(pid, 0x80);
}

SceBool sceSblACMgrHasMagicGateFlag(SceUID pid){
	return sceSblACMgrHasCapability(pid, 0x81);
}

SceBool SceSblACMgrForKernel_A50FDA27(SceUID pid){
	return sceSblACMgrHasAttributes(pid, 0x81);
}

SceInt32 SceSblACMgrForKernel_8241AB5C(SceUID pid, SceUInt32 index){

	if(index != 1 && index != 0x85)
		return 0x800F0916;

	if(SCE_FALSE == sceSblACMgrHasCapability(pid, index))
		return 0x800F0902;

	return 0;
}

SceBool sceSblACMgrHasSystemDataFilePlayReadyFlag(SceUID pid){

	if(SCE_FALSE != sceSblACMgrHasCapability(pid, 0x86))
		return SCE_TRUE;

	if(SCE_FALSE == sceSblACMgrIsFself(pid))
		return SCE_FALSE;

	if(0 != SceSysrootForDriver_E2515A08())
		return SCE_TRUE;

	return SCE_FALSE;
}

SceBool sceSblACMgrHasSystemDataFileFlag(SceUID pid){
	return sceSblACMgrHasCapability(pid, 0x87);
}
