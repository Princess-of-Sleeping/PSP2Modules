
#ifndef _PSP2_SBL_ACMGR_H_
#define _PSP2_SBL_ACMGR_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <psp2kern/types.h>

SceInt32 sceSblACMgrGetProcessProgramAuthId(SceUID pid, SceUInt64 *pAuthId);

#ifdef __cplusplus
}
#endif

#endif /* _PSP2_SBL_ACMGR_H_ */
