
#include "imports.h"
#include "acmgr.h"
#include "acmgr_has_flag.h"
#include "acmgr_is_program.h"

SceBool sceSblACMgrIsAllowUsbSerial(SceUID pid){

	if(SCE_TRUE == sceSblACMgrHasCapability(pid, 1))
		return SCE_TRUE;

	if(SCE_FALSE != sceSblACMgrHasCapability(pid, 0x83))
		return SCE_TRUE;

	if(SCE_FALSE == sceSblACMgrIsFself(pid))
		return SCE_FALSE;

	if(0 == SceSysrootForDriver_26AA237C())
		return SCE_FALSE;

	return SCE_TRUE;
}

SceBool sceSblACMgrIsAllowLoopBackMount(SceUID pid){

	SceUInt64 authId = 0LL;

	if(0 != sceSblACMgrGetProcessProgramAuthId(pid, &authId))
		return SCE_FALSE;

	if(authId == 0x280000000000001E) // SceVideoPlayer
		return SCE_TRUE;

	if(authId == 0x220000101CC73883)
		return SCE_TRUE;

	if(authId == 0x2800000000000010) // SceSettings
		return SCE_TRUE;

	if(authId == 0x2800000000000031) // SceAvMediaService
		return SCE_TRUE;

	if(SCE_TRUE == sceSblACMgrHasCapability(pid, 0x82))
		return SCE_TRUE;

	return SCE_FALSE;
}

SceBool sceSblACMgrIsAllowVirtualMachine(SceUID pid){

	if(SCE_TRUE == sceSblACMgrHasCapability(pid, 0x84))
		return SCE_TRUE;

	if(SCE_FALSE == sceSblACMgrIsFself(pid))
		return SCE_FALSE;

	if(0 == SceSysrootForDriver_26AA237C())
		return SCE_FALSE;

	return SCE_TRUE;
}

SceBool sceSblACMgrIsAllowProcessDebug(SceUID pid){

	SceUInt64 authId = 0LL;

	if(0 != sceSblACMgrGetProcessProgramAuthId(pid, &authId))
		return SCE_FALSE;

	if((authId & 0xFFF0000000000000) == 0x2800000000000000){
		if(0 != ksceSblQafMgrIsAllowSystemAppDebug()) {
			return 1;
		}
	}else if((authId & 0xFFF0000000000000) == 0x2100000000000000 || (authId & 0xFFF0000000000000) == 0x2200000000000000){
		if(0 == SceQafMgrForDriver_694D1096()) {
			if(0 != SceSysrootForDriver_56D85EB0(authId)){
				return 1;
			}
		}else{
			return 1;
		}
	}

	if(SCE_FALSE == sceSblACMgrHasAttributes(pid, 0x10))
		return SCE_TRUE;

	return SCE_FALSE;
}

SceBool sceSblACMgrIsAllowCache(SceUID pid){

	SceUInt64 authId = 0LL;

	if(0 != sceSblACMgrGetProcessProgramAuthId(pid, &authId))
		return SCE_FALSE;

	if(SCE_FALSE != sceSblACMgrHasCapability(pid, 3))
		return SCE_TRUE;

	if(authId == 0x210000101CD20007)
		return SCE_TRUE;

	if(authId == 0x210000101CD20008)
		return SCE_TRUE;

	if(authId == 0x210000101CD20009)
		return SCE_TRUE;

	if(SCE_FALSE != sceSblACMgrIsFself(pid))
		return SCE_TRUE;

	if(0 != SceSysrootForDriver_26AA237C())
		return SCE_TRUE;

	if(authId == 0x2800000000000044) // SceVideoStreaming
		return SCE_TRUE;

	if(authId == 0x280000000000001E) // SceVideoPlayer
		return SCE_TRUE;

	return SCE_FALSE;
}

// Used by sceAppMgrWorkDirMount
SceBool sceSblACMgrIsAllowPspMsMount(SceUID pid){

	SceUInt64 authId = 0LL;

	if(0 != sceSblACMgrGetProcessProgramAuthId(pid, &authId))
		return SCE_FALSE;

	if(authId == 0x2800000000000013) // ScePspemu
		return SCE_TRUE;

	if(authId == 0x2800000000007009)
		return SCE_TRUE;

	if(authId == 0x2800000000000010) // SceSettings
		return SCE_TRUE;

	if(authId == 0x280000000000002D) // SceContentManager
		return SCE_TRUE;

	if(authId == 0x280000000000002E) // fake_package_installer
		return SCE_TRUE;

	if(authId == 0x2800000000000022) // SceGameManual
		return SCE_TRUE;

	if(authId == 0x2800000000000039) // SceDailyCheckerBg
		return SCE_TRUE;

	if(authId == 0x2800000000000001) // SceShell
		return SCE_TRUE;

	return SCE_FALSE;
}

// Used by _sceAppMgrPspSaveDataRootMount
SceBool sceSblACMgrIsAllowPspSaveDataRootMount(SceUID pid){

	SceUInt64 authId = 0LL;

	if(0 != sceSblACMgrGetProcessProgramAuthId(pid, &authId))
		return SCE_FALSE;

	if(SCE_FALSE != sceSblACMgrIsGameProgram(pid))
		return SCE_TRUE;

	if(SCE_FALSE != sceSblACMgrIsAllowPspMsMount(pid))
		return SCE_TRUE;

	return SCE_FALSE;
}
