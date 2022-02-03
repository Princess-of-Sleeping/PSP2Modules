
#ifndef _PSP2_SBL_ACMGR_PFS_ATTR_H_
#define _PSP2_SBL_ACMGR_PFS_ATTR_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <psp2kern/types.h>

typedef struct SceSblACAttribute {
	SceUInt32 cmd;
	SceUInt8 *data;
	SceUInt32 length;
} SceSblACAttribute;

SceInt32 sceSblACMgrSetFSModeByAttribute(const SceSblACAttribute *param_1, SceUInt32 *result);

SceInt32 SceSblACMgrForKernel_1B160234(SceUID pid, SceSblACAttribute *dst, SceSblACAttribute *src, SceUInt32 a4);

SceInt32 sceSblACMgrSetFSAttributeByMode(SceUID pid, SceSblACAttribute *dst, SceUInt32 mode);

SceInt32 sceSblACMgrGetFSAttributeByThreadAccessLevel(SceUInt32 mode, int threadAccessLevel, SceSblACAttribute *dst);

SceInt32 SceSblACMgrForKernel_75AAF981(SceUID pid, const SceSblACAttribute *src, SceUInt32 a3);

// Used by SceExfatfs
SceInt32 SceSblACMgrForKernel_CCDBB74D(SceUID pid, SceSblACAttribute *dst, const SceSblACAttribute *src);

#ifdef __cplusplus
}
#endif

#endif /* _PSP2_SBL_ACMGR_PFS_ATTR_H_ */
