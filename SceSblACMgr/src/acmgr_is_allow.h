
#ifndef _PSP2_SBL_ACMGR_IS_ALLOW_H_
#define _PSP2_SBL_ACMGR_IS_ALLOW_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <psp2kern/types.h>

SceBool sceSblACMgrIsAllowCache(SceUID pid);
SceBool sceSblACMgrIsAllowUdcd(SceUID pid);
SceBool sceSblACMgrIsAllowUsbSerial(SceUID pid);
SceBool sceSblACMgrIsAllowLoopBackMount(SceUID pid);
SceBool sceSblACMgrIsAllowVirtualMachine(SceUID pid);
SceBool sceSblACMgrIsAllowProcessDebug(SceUID pid);
SceBool sceSblACMgrIsAllowPspMsMount(SceUID pid);
SceBool sceSblACMgrIsAllowPspSaveDataRootMount(SceUID pid);

#ifdef __cplusplus
}
#endif

#endif /* _PSP2_SBL_ACMGR_IS_ALLOW_H_ */
