
#ifndef _PSP2_SBL_ACMGR_IS_PROGRAM_H_
#define _PSP2_SBL_ACMGR_IS_PROGRAM_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <psp2kern/types.h>

SceBool sceSblACMgrIsRootProgram(SceUID pid);
SceBool sceSblACMgrIsSystemProgram(SceUID pid);
SceBool sceSblACMgrIsUserProgram(SceUID pid);
SceBool sceSblACMgrIsNonGameProgram(SceUID pid);
SceBool sceSblACMgrIsGameProgram(SceUID pid);
SceBool sceSblACMgrIsPSM(SceUID pid);
SceBool sceSblACMgrIsPSMDevAssistant(SceUID pid);
SceBool sceSblACMgrIsPSMRuntime(SceUID pid);
SceBool sceSblACMgrIsFself(SceUID pid);
SceBool sceSblACMgrIsSceShell(SceUID pid);
SceBool sceSblACMgrIsPspEmu(SceUID pid);
SceBool sceSblACMgrIsWebKitProcess(SceUID pid);
SceBool sceSblACMgrIsUpdaterUISetupperOrPkgInstallerSpawn(SceUID pid);
SceBool sceSblACMgrIsUnityRuntimeForPSM(SceUID pid);
SceBool sceSblACMgrIsPsNowClient(SceUID pid);

// Used by SceCtrl/SceHid. sceSblACMgrHasRemoteCtrlFlag?
SceBool SceSblACMgrForDriver_2E992B02(SceUID pid);

#ifdef __cplusplus
}
#endif

#endif /* _PSP2_SBL_ACMGR_IS_PROGRAM_H_ */
