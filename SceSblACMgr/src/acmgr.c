
#include <psp2kern/kernel/modulemgr.h>
#include "imports.h"
#include "acmgr.h"
#include "acmgr_has_flag.h"

SceInt32 sceSblACMgrGetProcessProgramAuthId(SceUID pid, SceUInt64 *pAuthId){

	SceSelfAuthInfo auth_info;

	if(pAuthId == NULL)
		return 0x800F0916;

	if(0 != ksceSysrootGetSelfAuthInfo(pid, &auth_info))
		return 0x800F0916;

	*pAuthId = auth_info.program_authority_id;

	return 0;
}

const SceKernelSysrootCheckRemapCodeForUserFunc checkRemapCodeFunc = SceSblACMgrForKernel_30575458;

void _start() __attribute__ ((weak, alias("sceSblACMgrModuleStart")));
int sceSblACMgrModuleStart(SceSize args, void *argp){

	SceSysrootForKernel_BE1EF51C(&checkRemapCodeFunc);

	return SCE_KERNEL_START_SUCCESS;
}
