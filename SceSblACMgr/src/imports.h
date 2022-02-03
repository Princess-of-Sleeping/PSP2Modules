
#ifndef _PSP2_SBL_ACMGR_IMPORTS_H_
#define _PSP2_SBL_ACMGR_IMPORTS_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <psp2kern/types.h>

SceUInt32 ksceKernelSysrootGetThreadAccessLevel(void);

int SceSysrootForDriver_26AA237C(void);
int SceSysrootForDriver_56D85EB0(SceUInt64 authId);
int SceSysrootForDriver_E2515A08(void);

typedef SceBool (* SceKernelSysrootCheckRemapCodeForUserFunc)(SceUID pid);

void SceSysrootForKernel_BE1EF51C(const SceKernelSysrootCheckRemapCodeForUserFunc *ppFunc);

int ksceSysrootGetSelfAuthInfo(SceUID pid, SceSelfAuthInfo *pAuthInfo);

int SceQafMgrForDriver_694D1096(void);
int ksceSblQafMgrIsAllowSystemAppDebug(void);

int look_ctype_table(char ch);

#ifdef __cplusplus
}
#endif

#endif /* _PSP2_SBL_ACMGR_IMPORTS_H_ */
