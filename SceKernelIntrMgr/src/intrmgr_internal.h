
#ifndef _PSP2_KERNEL_INTRMGR_INTERNAL_H_
#define _PSP2_KERNEL_INTRMGR_INTERNAL_H_

#ifdef __cplusplus
extern "C" {
#endif


#include <psp2kern/types.h>


typedef int (* SceKernelIntrHandler)(SceUInt16 intr_code, void *userCtx, SceUInt16 priority);


typedef struct SceKernelSubIntrtHandler { // 0x28-bytes
	SceSize size;
	int (* register_subintr_start)(int intr_code, int subintr_code, SceKernelIntrHandler handler, void *register_arg);
	int (* register_subintr_end)(int intr_code, int subintr_code, SceKernelIntrHandler handler, void *register_arg);
	int (* release_subintr_start)(int intr_code, int subintr_code);
	int (* release_subintr_end)(int intr_code, int subintr_code);
	int (* enable_subintr)(int intr_code, int subintr_code);
	int (* disable_subintr)(int intr_code, int subintr_code);
	int (* suspend_subintr)(int intr_code, int subintr_code, void *arg);
	int (* resume_subintr)(SceUInt32 intr_code, SceUInt32 subintr_code, int data);
	int (* is_subinterrupt_occurred)(int intr_code, int subintr_code);
} SceKernelSubIntrtHandler;

typedef struct SceKernelSubIntrParam { // 0x14-bytes
	SceSize size;
	SceUInt32 num;
	const SceKernelSubIntrtHandler *handler;
	SceUInt32 unk_C;
	SceUInt32 unk_10;
} SceKernelSubIntrParam;

typedef struct SceKernelIntrEntry { // size is 0x40-bytes
	SceKernelIntrHandler handler;
	SceUInt16 intr_code;
	SceUInt16 unk_0x06;
	ScePVoid argp;
	SceUInt16 priority;
	SceUInt8 interrupt_type;
	SceUInt8 unk_0x0F;
	char *name;
	SceKernelIntrHandler hook_store;
	SceUInt32 unk_0x18;
	SceUInt32 unk_0x1C;
	SceUInt32 nest[4];
	int mutex;
	struct SceKernelIntrEntry *sub_intr;
	const SceKernelSubIntrtHandler *sub_handler;
	SceUInt32 unk_0x3C;
} SceKernelIntrEntry;

typedef struct SceKernelIntrNest { // size is 0x80-bytes
	SceUInt32 nestCount;
	SceUInt32 unk_0x04[0x1F];
} SceKernelIntrNest;

typedef struct SceKernelIntrStack { // size is 0x2000-bytes
	SceUInt32 stack[(0x2000 - 0x138) / 4];
	SceUInt32 store[0x138 / 4];
} SceKernelIntrStack;


#ifdef __cplusplus
}
#endif

#endif /* _PSP2_KERNEL_INTRMGR_INTERNAL_H_ */
