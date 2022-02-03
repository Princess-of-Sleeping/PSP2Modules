
#ifndef _PSP2_SBL_ACMGR_MEDIA_H_
#define _PSP2_SBL_ACMGR_MEDIA_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <psp2kern/types.h>

SceBool SceSblACMgrForKernel_1948E9DB(void);
SceBool SceSblACMgrForKernel_FBA1A256(void);

SceBool sceSblACMgrIsVs0UserDriveAccessibleProgram(SceUID pid, const SceChar8 *path);

SceBool sceSblACMgrIsMediaAccessibleProgram(SceUID pid, const SceChar8 *path);
SceBool sceSblACMgrIsMediaAccessibleProgramWithThreadAccessLevel(SceUID pid, const SceChar8 *path);

SceInt32 sceSblACMgrGetMediaType(const SceChar8 *path, SceUInt32 *pMediaType); // Real name
SceInt32 sceSblACMgrGetMediaMode(SceUID pid, const SceChar8 *path, SceUInt32 *mode);

#ifdef __cplusplus
}
#endif

#endif /* _PSP2_SBL_ACMGR_MEDIA_H_ */
