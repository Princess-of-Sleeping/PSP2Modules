
#include "imports.h"
#include "acmgr_is_program.h"
#include "acmgr.h"
#include "acmgr_has_flag.h"

SceBool sceSblACMgrIsRootProgram(SceUID pid){
	return sceSblACMgrHasCapability(pid, 0);
}

SceBool sceSblACMgrIsSystemProgram(SceUID pid){
	return sceSblACMgrHasCapability(pid, 1);
}

SceBool sceSblACMgrIsUserProgram(SceUID pid){

	if(0 != sceSblACMgrHasCapability(pid, 2))
		return SCE_TRUE;

	if(0 != sceSblACMgrHasCapability(pid, 3))
		return SCE_TRUE;

	return SCE_FALSE;
}

SceBool sceSblACMgrIsNonGameProgram(SceUID pid){
	return sceSblACMgrHasCapability(pid, 3);
}

SceBool sceSblACMgrIsGameProgram(SceUID pid){

	SceUInt64 authId = 0LL;

	if(0 != sceSblACMgrGetProcessProgramAuthId(pid, &authId))
		return SCE_FALSE;

	if((authId & 0xFFF0000000000000) == 0x2100000000000000)
		return SCE_TRUE;

	if(authId != 0x2F00000000000001)
		return SCE_FALSE;

	return SCE_TRUE;
}

SceBool sceSblACMgrIsFself(SceUID pid){

	SceUInt64 authId = 0LL;

	if(0 != sceSblACMgrGetProcessProgramAuthId(pid, &authId))
		return SCE_FALSE;

	if((authId & 0xFFF0000000000000) == 0x2F00000000000000)
		return SCE_FALSE;

	return SCE_TRUE;
}

SceBool sceSblACMgrIsPSM(SceUID pid){

	SceUInt64 authId = 0LL;

	if(0 != sceSblACMgrGetProcessProgramAuthId(pid, &authId))
		return SCE_FALSE;

	if(authId == 0x2800C0101CD2000B)
		return SCE_TRUE;

	if(authId == 0x210000101CD20007)
		return SCE_TRUE;

	if(authId == 0x210000101CD20008)
		return SCE_TRUE;

	if(authId == 0x210000101CD20009)
		return SCE_TRUE;

	if(authId == 0x210000101CD2000A)
		return SCE_TRUE;

	if(SCE_FALSE == sceSblACMgrIsFself(pid))
		return SCE_FALSE;

	if(0 == SceSysrootForDriver_26AA237C())
		return SCE_FALSE;

	return SCE_TRUE;
}

SceBool sceSblACMgrIsSceShell(SceUID pid){

	SceUInt64 authId = 0LL;

	if(0 != sceSblACMgrGetProcessProgramAuthId(pid, &authId))
		return SCE_FALSE;

	if(authId == 0x2800000000000001)
		return SCE_TRUE;

	return SCE_FALSE;
}

SceBool sceSblACMgrIsPSMDevAssistant(SceUID pid){

	SceUInt64 authId = 0LL;

	if(0 != sceSblACMgrGetProcessProgramAuthId(pid, &authId))
		return SCE_FALSE;

	if(authId == 0x210000101CD20007)
		return SCE_TRUE;

	if(authId == 0x210000101CD20008)
		return SCE_TRUE;

	if(authId == 0x210000101CD20009)
		return SCE_TRUE;

	return SCE_FALSE;
}

SceBool sceSblACMgrIsPspEmu(SceUID pid){

	SceUInt64 authId = 0LL;

	if(0 != sceSblACMgrGetProcessProgramAuthId(pid, &authId))
		return SCE_FALSE;

	if(authId == 0x2800000000007009)
		return SCE_TRUE;

	if(authId == 0x2800000000000013)
		return SCE_TRUE;

	return SCE_FALSE;
}

SceBool sceSblACMgrIsPSMRuntime(SceUID pid){

	SceUInt64 authId = 0LL;

	if(0 != sceSblACMgrGetProcessProgramAuthId(pid, &authId))
		return SCE_FALSE;

	if(authId == 0x210000101CD2000A)
		return SCE_TRUE;

	if(authId == 0x2800C0101CD2000B)
		return SCE_TRUE;

	return SCE_FALSE;
}

SceBool sceSblACMgrIsUpdaterUISetupperOrPkgInstallerSpawn(SceUID pid){

	SceUInt64 authId = 0LL;

	if(0 != sceSblACMgrGetProcessProgramAuthId(pid, &authId))
		return SCE_FALSE;

	if((authId & 0xFFFFFFFFFFFFFFF0) == 0x2800800000000000)
		return SCE_TRUE;

	return SCE_FALSE;
}

SceBool sceSblACMgrIsAllowUdcd(SceUID pid){

	SceUInt64 authId = 0LL;

	if(0 != sceSblACMgrGetProcessProgramAuthId(pid, &authId))
		return SCE_FALSE;

	if(authId == 0x220000101CC73883)
		return SCE_TRUE;;

	if(authId == 0x2800000000000010)
		return SCE_TRUE;

	if(SCE_FALSE != sceSblACMgrHasCapability(pid, 0x82))
		return SCE_TRUE;

	return SCE_FALSE;
}

SceBool sceSblACMgrIsUnityRuntimeForPSM(SceUID pid){

	SceUInt64 authId = 0LL;

	if(0 != sceSblACMgrGetProcessProgramAuthId(pid, &authId))
		return SCE_FALSE;

	if(authId == 0x210000101CD2000A)
		return SCE_TRUE;

	return SCE_FALSE;
}

SceBool sceSblACMgrIsWebKitProcess(SceUID pid){

	SceUInt64 authId = 0LL;

	if(0 != sceSblACMgrGetProcessProgramAuthId(pid, &authId))
		return SCE_FALSE;

	if(authId == 0x2800000000008005)
		return SCE_TRUE;

	if(authId == 0x2800000000008003)
		return SCE_TRUE;

	return SCE_FALSE;
}

SceBool SceSblACMgrForDriver_2E992B02(SceUID pid){

	SceUInt64 authId = 0LL;

	if(0 != sceSblACMgrGetProcessProgramAuthId(pid, &authId))
		return SCE_FALSE;

	if((authId & 0xFFFF000000000000) == 0x2801000000000000)
		return SCE_TRUE;

	return SCE_FALSE;
}

SceBool sceSblACMgrIsPsNowClient(SceUID pid){

	SceUInt64 authId = 0LL;

	if(0 != sceSblACMgrGetProcessProgramAuthId(pid, &authId))
		return SCE_FALSE;

	if(authId == 0x220000101CD2000C)
		return SCE_TRUE;

	if(authId == 0x220000101CD2000D)
		return SCE_TRUE;

	return SCE_FALSE;
}
