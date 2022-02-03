
#ifndef _PSP2_SBL_ACMGR_HAS_FLAG_H_
#define _PSP2_SBL_ACMGR_HAS_FLAG_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <psp2kern/types.h>

SceBool sceSblACMgrHasCapability(SceUID pid, SceUInt32 index);
SceBool sceSblACMgrHasAttributes(SceUID pid, SceUInt32 index);

SceBool sceSblACMgrHasMagicGateFlag(SceUID pid);
SceBool sceSblACMgrHasMarlinFlag(SceUID pid);
SceBool sceSblACMgrHasSystemDataFileFlag(SceUID pid);
SceBool sceSblACMgrHasSystemDataFilePlayReadyFlag(SceUID pid);

// Used by SceDeci4pLoadp/SceDeci4pDbgp
SceBool sceSblACMgrHasUserDebugFlag(SceUID pid);

SceBool sceSblACMgrIsDevelopmentMode(void);

SceBool SceSblACMgrForKernel_30575458(SceUID pid);
SceBool SceSblACMgrForKernel_48F4D5EE(SceUID pid);
SceBool SceSblACMgrForKernel_A50FDA27(SceUID pid);

// Used by SceKernelThreadMgr
SceInt32 SceSblACMgrForKernel_8241AB5C(SceUID pid, SceUInt32 index);

#ifdef __cplusplus
}
#endif

#endif /* _PSP2_SBL_ACMGR_HAS_FLAG_H_ */
