/*
 * PS Vita Kernel bootconfig Header
 * Copyright (C) 2019, Princess of Sleeping
 */

#ifndef _KBL_H_
#define _KBL_H_

#include <psp2kern/types.h>

// SceDebug
int ksceKblPutchar(void *args, char c);
int sceKernelPrintf(const char *fmt, ...);
// ksceKblPrintfLevel
int sceKernelGetDebugLevel(void);
void *sceKernelGetDebugPutchar(void);

// SceSysroot
typedef struct SceSysrootHardwareFlags {
	uint32_t data[4];
} __attribute__((packed)) SceSysrootHardwareFlags;

int ksceKblGetHardwareFlags(SceSysrootHardwareFlags *data);
int sceKernelSysrootIofilemgrStart(void);
int sceKernelSysrootProcessmgrStart2(void);
int sceKernelSysrootThreadMgrStartAfterProcess(void);
void sceKernelSysrootCorelockLock(int core);
void sceKernelSysrootCorelockUnlock(void);

// SceKbl
typedef struct SceModuleLoadList {
	const char *filename;
} __attribute__((packed)) SceModuleLoadList;

int ksceKblLoadModule(const SceModuleLoadList *list, SceUID *uid, int count, int some_flag);
int ksceKblStartModule(SceUID *uid_list, int count, SceSize args, void *argp);
int ksceKblFreeFileSystemCtx(void);
int ksceKblSetNonSyncModuleStart(void);
int ksceKblAuthMgrClose(void);
void ksceKblCpuDisableIrqInterrupts(void);

// SceSblAIMgr
int sceSblAimgrIsCEX(void);
int sceSblAimgrIsDEX(void);
int sceSblAimgrIsDiag(void);
int sceSblAimgrIsTool(void);
int sceSblAimgrIsTest(void);

int sceSblAimgrIsVITA(void);
int sceSblAimgrIsDolce(void);
int sceSblAimgrIsGenuineDolce(void);

// SceDipsw
int sceKernelCheckDipsw(int a1);

// SceSblQafMgr
int sceSblQafManagerIsAllowKernelDebug(void);

#endif	/* _KBL_H_ */
