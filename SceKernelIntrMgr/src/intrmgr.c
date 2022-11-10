
#include <psp2kern/kernel/modulemgr.h>
#include <psp2kern/kernel/cpu.h>
#include <psp2kern/kernel/sysmem.h>
#include <psp2kern/kernel/excpmgr.h>
#include <psp2/kernel/error.h>
#include "intrmgr_internal.h"


int SceKernelSuspendForDriver_CE7A2207(int a1, void *a2, int a3);
int SceKernelSuspendForDriver_105C5752(int a1, void *a2, int a3);
void *sceKernelSysrootGetVbaseResetVector(void);
void sceKernelSysrootSetModulePrivate(SceUInt32 idx, void *pModulePrivate);
void *SceSysrootForKernel_118657C6(void);
int sceKernelSysrootCorelockLock(SceUInt32 core);
int sceKernelSysrootCorelockUnlock();

int SceSysmemForKernel_08AB3DAE(const char *name, int a2, char **result);
int SceSysmemForKernel_9C7B62AB(char *name, int a2);
SceUID sceKernelAllocPartitionMemBlock(SceUID part, const char *name, SceKernelMemBlockType type, SceSize size, const void *pOpt);
void *sceKernelSysrootGetSharedMemory(void);

char *ksceKernelGetNameForUid2(SceUID guid);


SceKernelIntrEntry entries[0x100];
SceKernelIntrNest nest[4];


void *SceInterruptControllerReg_0x100;
void *SceInterruptControllerReg_0x1000;


ScePVoid DAT_81008200;
ScePVoid DAT_81008240;
ScePVoid DAT_81008244;

ScePVoid DAT_810882c0;
ScePVoid DAT_810882c4;
ScePVoid DAT_810882c8;
ScePVoid DAT_810882cc;

void *DAT_810882d4;
SceUID DAT_810882d8;

ScePVoid DAT_810882e0;
ScePVoid DAT_810882e4;
ScePVoid DAT_810882e8;
ScePVoid DAT_810882ec;

SceUID DAT_810882f0;
ScePVoid DAT_810882f4;


void intr_init_stack_magic(SceUInt32 magic);
int intr_check_stack_magic(void);
int AllocSharedMemory_S(SceUIntPtr address, SceSize length);
int sceKernelIrqHandler(void);
int sceKernelSvcHandler();


#define sceKernelGetMPIDR() ({ \
	SceUIntPtr v; \
	asm volatile ("mrc p15, #0, %0, c0, c0, #5\n": "=r"(v)); \
	v; \
	})

int intr_get_nest_count(void){
/*
	SceUIntPtr v;

	asm volatile (
		"mrc p15, #0, %0, c0, c0, #5\n"
		: "=r"(v)
	);
*/
	return nest[sceKernelGetMPIDR() & 3].nestCount;
}

int sceKernelDisableIntr(SceUInt32 intr_code){

	if((intr_code - 0x10) > 0xEF){
		return SCE_KERNEL_ERROR_ILLEGAL_INTRCODE;
	}

	if(entries[intr_code].handler == NULL){
		return SCE_KERNEL_ERROR_NOTFOUND_HANDLER;
	}

	*(int *)(SceInterruptControllerReg_0x1000 + ((intr_code >> 5) + 0x60) * 4) = 1 << (intr_code & 0x1F);

	return 0;
}

int sceKernelEnableIntr(SceUInt32 intr_code){

	SceKernelIntrEntry *entry;

	if((intr_code - 0x10) > 0xEF){
		ksceDebugPrintf("Illegal interrupt code. %d\n", intr_code);
		return SCE_KERNEL_ERROR_ILLEGAL_INTRCODE;
	}

	entry = &(entries[intr_code]);

	if(entry->handler == NULL){
		ksceDebugPrintf("interrupt handler is not registered. intrcode %d\n", intr_code);
		return SCE_KERNEL_ERROR_NOTFOUND_HANDLER;
	}

	*(int *)(SceInterruptControllerReg_0x1000 + ((intr_code >> 5) + 0x40) * 4) = 1 << (intr_code & 0x1F);

	return 0;
}

int sceKernelDisableSubIntr(SceUInt32 intr_code, SceUInt32 subintr_code){

	int res, prev;
	SceKernelIntrEntry *entry;

	if(intr_code >= 0x100){
		ksceDebugPrintf("Unknown interrupt code. %d\n", intr_code);
		return SCE_KERNEL_ERROR_ILLEGAL_INTRCODE;
	}

	entry = &(entries[intr_code]);

	prev = ksceKernelCpuSuspendIntr(&(entry->mutex));

	if((SceInt32)subintr_code < 0 || subintr_code >= entry->unk_0x3C){
		ksceDebugPrintf("Unknown subinterrupt code. %d-%d\n", intr_code, subintr_code);
		ksceKernelCpuResumeIntr(&(entry->mutex), prev);
		return SCE_KERNEL_ERROR_ILLEGAL_INTRCODE;
	}

	if(entry->handler == NULL || entry->sub_handler == NULL || entry->sub_handler->disable_subintr == NULL){
		ksceKernelCpuResumeIntr(&(entry->mutex), prev);
		return SCE_KERNEL_ERROR_NOTFOUND_HANDLER;
	}

	res = entry->sub_handler->disable_subintr(intr_code, subintr_code);
	ksceKernelCpuResumeIntr(&(entry->mutex), prev);

	return res;
}

int sceKernelEnableSubIntr(SceUInt32 intr_code, SceUInt32 subintr_code){

	int res, prev;
	SceKernelIntrEntry *entry;

	if(intr_code >= 0x100){
		ksceDebugPrintf("Unknown interrupt code. %d\n", intr_code);
		return SCE_KERNEL_ERROR_ILLEGAL_INTRCODE;
	}

	entry = &(entries[intr_code]);

	prev = ksceKernelCpuSuspendIntr(&(entry->mutex));

	if((SceInt32)subintr_code < 0 || subintr_code >= entry->unk_0x3C){
		ksceDebugPrintf("Unknown subinterrupt code. %d-%d\n", intr_code, subintr_code);
		ksceKernelCpuResumeIntr(&(entry->mutex), prev);
		return SCE_KERNEL_ERROR_ILLEGAL_INTRCODE;
	}

	if(entry->handler == NULL || entry->sub_handler == NULL || entry->sub_handler->enable_subintr == NULL){
		ksceKernelCpuResumeIntr(&(entry->mutex), prev);
		return SCE_KERNEL_ERROR_NOTFOUND_HANDLER;
	}

	res = entry->sub_handler->enable_subintr(intr_code, subintr_code);
	ksceKernelCpuResumeIntr(&(entry->mutex), prev);

	return res;
}

int sceKernelIsIntrContext(void){

	SceUIntPtr v;

	asm volatile (
		"mrc p15, #0, %0, c13, c0, #4\n"
		: "=r"(v)
	);

	if(v != 0){
		return 0;
	}

	return intr_get_nest_count();
}

int sceKernelIsSubInterruptOccurred(SceUInt32 intr_code, SceUInt32 subintr_code){

	int res, prev;
	SceKernelIntrEntry *entry;

	if(intr_code >= 0x100){
		ksceDebugPrintf("Unknown interrupt code. %d\n", intr_code);
		return SCE_KERNEL_ERROR_ILLEGAL_INTRCODE;
	}

	entry = &(entries[intr_code]);

	prev = ksceKernelCpuSuspendIntr(&(entry->mutex));

	if((SceInt32)subintr_code < 0 || subintr_code >= entry->unk_0x3C){
		ksceDebugPrintf("Unknown subinterrupt code. %d-%d\n", intr_code, subintr_code);
		ksceKernelCpuResumeIntr(&(entry->mutex), prev);
		return SCE_KERNEL_ERROR_ILLEGAL_INTRCODE;
	}

	if(entry->handler == NULL || entry->sub_handler == NULL || entry->sub_handler->is_subinterrupt_occurred == NULL){
		ksceKernelCpuResumeIntr(&(entry->mutex), prev);
		return SCE_KERNEL_ERROR_NOTFOUND_HANDLER;
	}

	res = entry->sub_handler->is_subinterrupt_occurred(intr_code, subintr_code);
	ksceKernelCpuResumeIntr(&(entry->mutex), prev);

	return res;
}

int sceKernelResumeIntr(SceUInt32 intr_code, SceUInt32 a2){

	if((intr_code - 0x10) > 0xEF){
		ksceDebugPrintf("Illegal interrupt code. %d\n", intr_code);
		return SCE_KERNEL_ERROR_ILLEGAL_INTRCODE;
	}

	if(a2 > 1){
		ksceDebugPrintf("Invalid argument\n");
	    return SCE_KERNEL_ERROR_INVALID_ARGUMENT;
	}

	if(a2 == 1){
		*(int *)(SceInterruptControllerReg_0x1000 + ((intr_code >> 5) + 0x40) * 4) = 1 << (intr_code & 0x1F);
	}else{
		*(int *)(SceInterruptControllerReg_0x1000 + ((intr_code >> 5) + 0x60) * 4) = 1 << (intr_code & 0x1F);
	}

	return 0;
}

int sceKernelSuspendIntr(SceUInt32 intr_code, SceUInt32 *a2){

	int value;

	if((intr_code - 0x10) > 0xEF){        
		ksceDebugPrintf("Illegal interrupt code. %d\n", intr_code);
		return SCE_KERNEL_ERROR_ILLEGAL_INTRCODE;
	}

	if(a2 == NULL){                                     
		ksceDebugPrintf("Invalid argument\n");
		return SCE_KERNEL_ERROR_INVALID_ARGUMENT;
	}

	value = 1 << (intr_code & 0x1F);

	if((*(int *)(SceInterruptControllerReg_0x1000 + ((intr_code >> 5) + 0x40) * 4) & value) != 0){
		*(int *)(SceInterruptControllerReg_0x1000 + ((intr_code >> 5) + 0x40) * 4 + 0x80) = value;
		*a2 = 1;
	}else{
		*a2 = 0;
	}

	return 0;
}

int sceKernelGetIntrPriority(SceUInt32 intr_code, SceUInt32 *priority){

	if(intr_code >= 0x100){        
		ksceDebugPrintf("Unknown interrupt code. %d\n", intr_code);
		return SCE_KERNEL_ERROR_ILLEGAL_INTRCODE;
	}

	if(entries[intr_code].handler == NULL){
		ksceDebugPrintf("interrupt handler is not registered\n");
		return SCE_KERNEL_ERROR_NOTFOUND_HANDLER;
	}

	*priority = entries[intr_code].priority;

	return 0;
}

int sceKernelGetIntrTargetCpu(SceUInt32 intr_code, SceUInt32 *intr_target){

	if(intr_code >= 0x100){        
		ksceDebugPrintf("Unknown interrupt code. %d\n", intr_code);
		return SCE_KERNEL_ERROR_ILLEGAL_INTRCODE;
	}

	if(entries[intr_code].handler == NULL){
		ksceDebugPrintf("interrupt handler is not registered\n");
		return SCE_KERNEL_ERROR_NOTFOUND_HANDLER;
	}

	*intr_target = entries[intr_code].unk_0x06;

	return 0;
}

int sceKernelRegisterIntrHandler(SceUInt32 intr_code, const char *name, int interrupt_type, SceKernelIntrHandler handler, void *user_ctx, int priority, int target_cpu, SceKernelSubIntrParam *opt){

	SceUInt8 unk_0x0F, target;
	int prev;
	SceUInt32 v;
	SceKernelIntrEntry *entry, *sub_entry;

	asm volatile (
		"mrc p15, #0, %0, c13, c0, #4\n"
		:"=r"(v)
	);
	if(v == 0 && intr_get_nest_count() != 0){
		return SCE_KERNEL_ERROR_ILLEGAL_CONTEXT;
	}

	if(intr_code >= 0x100){
		return SCE_KERNEL_ERROR_ILLEGAL_INTRCODE;
	}

	if((interrupt_type & 0xFFFFFFEE) != 0){
		return SCE_KERNEL_ERROR_ILLEGAL_INTRTYPE;
	}

	if(handler == NULL){
		return SCE_KERNEL_ERROR_ILLEGAL_HANDLER;
	}

	if((priority & 0xFFFFFF0F) != 0){
		return SCE_KERNEL_ERROR_ILLEGAL_INTRPRIORITY;
	}

	if(target_cpu == 0){
		return SCE_KERNEL_ERROR_ILLEGAL_TARGET_CPU;
	}

	if((target_cpu & 0xF) != 0 && (target_cpu & 0xF0000) != 0){
		return SCE_KERNEL_ERROR_ILLEGAL_TARGET_CPU;
	}

	if((target_cpu & 0xF0000) == 0){
		if((target_cpu & 0xF) != 0){
			if(target_cpu < 0){
				return SCE_KERNEL_ERROR_ILLEGAL_TARGET_CPU;
			}
			target = (SceUInt8)target_cpu;
		}else{
			target = (SceUInt8)((SceUInt32)target_cpu >> 0x10);
		}
	}else{
		target = (SceUInt8)((SceUInt32)target_cpu >> 0x10);
	}

	if(opt == NULL){
		unk_0x0F = 1;
	}else{
		if(opt->size != 0x14){
			return SCE_KERNEL_ERROR_ILLEGAL_INTRPARAM;
		}

		if((opt->unk_C & 1) != 0){
			if((opt->unk_10 & 0xFFFFFFFD) != 1){
				return SCE_KERNEL_ERROR_ILLEGAL_INTRPARAM;
			}
			unk_0x0F = (uint8_t)opt->unk_10;
			int shift = (intr_code & 0xF) << 1;
			int value = *(int *)(SceInterruptControllerReg_0x1000 + ((intr_code >> 4) + 0x300) * 4);
			*(int *)(SceInterruptControllerReg_0x1000 + ((intr_code >> 4) + 0x300) * 4) = ((opt->unk_10 ^ value >> shift) & 3) << shift ^ value;
		}else{
			unk_0x0F = 1;
		}
	}

	entry = &(entries[intr_code]);

	prev = ksceKernelCpuSuspendIntr(&(entry->mutex));

	if(entry->handler != NULL){
		ksceKernelCpuResumeIntr(&(entry->mutex), prev);
		return SCE_KERNEL_ERROR_FOUND_HANDLER;
	}

	if(name == NULL){
		entry->name = NULL;
	}else{
		SceSysmemForKernel_08AB3DAE(name, 0, &(entry->name));
	}

	entry->handler        = handler;
	entry->priority       = (uint16_t)priority;
	entry->intr_code      = (uint16_t)intr_code;
	entry->interrupt_type = (uint8_t)interrupt_type;
	entry->argp = user_ctx;
	memset(&(entry->nest), 0, 0x10);
	entry->hook_store = NULL;

	if(opt == NULL || opt->num == 0){
		entry->sub_intr = NULL;
		entry->sub_handler = NULL;
		entry->unk_0x3C = 0;
	}else{
		sub_entry = ksceKernelAllocHeapMemory(DAT_810882f0, opt->num * sizeof(SceKernelIntrEntry));
		entry->sub_intr = sub_entry;
		if(sub_entry == NULL){
			ksceKernelCpuResumeIntr(&(entry->mutex), prev);
			return SCE_KERNEL_ERROR_NO_MEMORY;
		}

		entry->sub_handler = opt->handler;
		entry->unk_0x3C = opt->num;
		memset(sub_entry, 0, opt->num * sizeof(SceKernelIntrEntry));
	}

	if((SceInt32)intr_code > 0xF){
		*(char *)(SceInterruptControllerReg_0x1000 + intr_code + 0x400) = priority & 0xFF;
		*(char *)(SceInterruptControllerReg_0x1000 + intr_code + 0x800) = target;
	}

	entry->unk_0x0F = unk_0x0F;
	entry->unk_0x06 = (uint16_t)(*(uint8_t *)(SceInterruptControllerReg_0x1000 + intr_code + 0x800));
	ksceKernelCpuResumeIntr(&(entry->mutex), prev);

	if(DAT_810882f4 != NULL){
		int (* f)(SceUInt32 intr_code) = DAT_810882f4;
		f(intr_code);
	}

	// ksceDebugPrintf("Intr registered. [%-31s] %d %d 0x%X\n", name, intr_code, priority, target_cpu);

	return 0;
}

int sceKernelRegisterIntrHookHandler(SceUInt32 intr_code, SceKernelIntrHandler hook_handler, SceKernelIntrHandler *a3){

	int prev;
	SceKernelIntrEntry *entry;
	SceUInt32 v;

	asm volatile (
		"mrc p15, #0, %0, c13, c0, #4\n"
		:"=r"(v)
	);
	if(v == 0 && intr_get_nest_count() != 0){
		return SCE_KERNEL_ERROR_ILLEGAL_CONTEXT;
	}

	if(intr_code >= 0x100){
		ksceDebugPrintf("Unknown interrupt code. %d\n", intr_code);
		return SCE_KERNEL_ERROR_ILLEGAL_INTRCODE;
	}

	if(hook_handler == NULL){
		return SCE_KERNEL_ERROR_ILLEGAL_HANDLER;
	}

	if(a3 == NULL){
		return SCE_KERNEL_ERROR_INVALID_ARGUMENT;
	}

	entry = &(entries[intr_code]);

	prev = ksceKernelCpuSuspendIntr(&(entry->mutex));

	if(entry->handler == NULL){
		ksceKernelCpuResumeIntr(&(entry->mutex), prev);
		return SCE_KERNEL_ERROR_NOTFOUND_HANDLER;
	}

	if(entry->hook_store != NULL){
		ksceKernelCpuResumeIntr(&(entry->mutex), prev);
		return SCE_KERNEL_ERROR_FOUND_HANDLER;
	}

	*a3 = entry->handler;
	entry->hook_store = entry->handler;
	entry->handler = hook_handler;

	ksceKernelCpuResumeIntr(&(entry->mutex), prev);

	return 0;
}

int sceKernelRegisterSubIntrHandler(SceUInt32 intr_code, SceUInt32 subintr_code, const char *name, void *handler, void *argp){

	int res, prev;
	SceKernelIntrEntry *entry, *sub_entry;
	SceUInt32 v;

	asm volatile (
		"mrc p15, #0, %0, c13, c0, #4\n"
		:"=r"(v)
	);
	if(v == 0 && intr_get_nest_count() != 0){
		return SCE_KERNEL_ERROR_ILLEGAL_CONTEXT;
	}

	if(intr_code >= 0x100){
		ksceDebugPrintf("Unknown interrupt code. %d\n", intr_code);
		return SCE_KERNEL_ERROR_ILLEGAL_INTRCODE;
	}

	if(handler == NULL){
		ksceDebugPrintf("invalid handler address.\n");
		return SCE_KERNEL_ERROR_ILLEGAL_HANDLER;
	}

	entry = &(entries[intr_code]);

	prev = ksceKernelCpuSuspendIntr(&(entry->mutex));

	if(entry->handler == NULL){
		ksceDebugPrintf("not yet registered. %d\n", intr_code);
		ksceKernelCpuResumeIntr(&(entry->mutex), prev);
		return SCE_KERNEL_ERROR_NOTFOUND_HANDLER;
	}

	if(entry->sub_handler == NULL){
		ksceKernelCpuResumeIntr(&(entry->mutex), prev);
		return SCE_KERNEL_ERROR_ERROR;
	}

	if((SceInt32)subintr_code < 0 || entry->unk_0x3C <= subintr_code){
		ksceDebugPrintf("Unknown subinterrupt code. %d-%d\n", intr_code, subintr_code);
		ksceKernelCpuResumeIntr(&(entry->mutex), prev);
		return SCE_KERNEL_ERROR_ILLEGAL_INTRCODE;
	}

	sub_entry = &(entry->sub_intr[subintr_code]);

	if(sub_entry->handler != NULL){
		ksceDebugPrintf("already registered: %d-%d %s %p\n", intr_code, subintr_code, name, sub_entry->handler);
		ksceKernelCpuResumeIntr(&(entry->mutex), prev);
		return SCE_KERNEL_ERROR_FOUND_HANDLER;
	}

	if(entry->sub_handler->register_subintr_start != NULL){
		res = entry->sub_handler->register_subintr_start(intr_code, subintr_code, handler, argp);
		if(res < 0){
			ksceKernelCpuResumeIntr(&(entry->mutex), prev);
			return res;
		}
	}

	if(name == NULL){
		sub_entry->name = NULL;
	}else{
		SceSysmemForKernel_08AB3DAE(name, 0, &(sub_entry->name));
	}

	sub_entry->handler        = handler;
	sub_entry->interrupt_type = entry->interrupt_type;
	sub_entry->priority       = entry->priority;
	sub_entry->unk_0x06       = entry->unk_0x06;
	sub_entry->argp           = argp;
	sub_entry->sub_intr       = NULL;
	sub_entry->sub_handler    = NULL;
	sub_entry->unk_0x3C       = 0;
	memset(sub_entry->nest, 0, 0x10);

	if(entry->sub_handler->register_subintr_end != NULL){
		res = entry->sub_handler->register_subintr_end(intr_code, subintr_code, handler, argp);
		if(res < 0){
			sub_entry->handler = NULL;
			ksceKernelCpuResumeIntr(&(entry->mutex), prev);
			return res;
		}
	}

	// ksceDebugPrintf("subintr [%-31s] %d-%d\n", name, intr_code, subintr_code);

	ksceKernelCpuResumeIntr(&(entry->mutex), prev);

	return 0;
}

int sceKernelReleaseIntrHandler(SceUInt32 intr_code){

	int prev;
	SceKernelIntrEntry *entry;
	SceUInt32 v;

	asm volatile (
		"mrc p15, #0, %0, c13, c0, #4\n"
		:"=r"(v)
	);
	if(v == 0 && intr_get_nest_count() != 0){
		return SCE_KERNEL_ERROR_ILLEGAL_CONTEXT;
	}

	if(intr_code >= 0x100){
		ksceDebugPrintf("Unknown interrupt code. %d\n", intr_code);
		return SCE_KERNEL_ERROR_ILLEGAL_INTRCODE;
	}

	entry = &(entries[intr_code]);

	prev = ksceKernelCpuSuspendIntr(&(entry->mutex));

	if(entry->handler == NULL){
		ksceDebugPrintf("interrupt handler is not registered. %d\n", intr_code);
		ksceKernelCpuResumeIntr(&(entry->mutex), prev);
		return SCE_KERNEL_ERROR_NOTFOUND_HANDLER;
	}

    if ((SceInt32)intr_code > 0xF && entry->priority > 0x3F) {
      *(int *)(SceInterruptControllerReg_0x1000 + ((intr_code >> 5) + 0x60) * 4) = 1 << (intr_code & 0x1F);
    }

	entry->handler        = NULL;
	entry->hook_store     = NULL;
	entry->intr_code      = 0;
	entry->priority       = 0;
	entry->interrupt_type = 0;
	entry->argp           = NULL;
	entry->unk_0x06       = 0;
	entry->unk_0x0F       = 0;

	if(entry->sub_intr != NULL){
		ksceKernelFreeHeapMemory(DAT_810882f0, entry->sub_intr);
		entry->sub_intr = NULL;
		entry->sub_handler = NULL;
		entry->unk_0x3C = 0;
	}

	SceSysmemForKernel_9C7B62AB(entry->name, 0);
	ksceKernelCpuResumeIntr(&(entry->mutex), prev);

	if(DAT_810882ec != NULL){
		int (* f)(SceUInt32 intr_code) = DAT_810882ec;
		f(intr_code);
	}

	return 0;
}

int sceKernelReleaseIntrHookHandler(SceUInt32 intr_code){

	int prev;
	SceKernelIntrEntry *entry;
	SceUInt32 v;

	asm volatile (
		"mrc p15, #0, %0, c13, c0, #4\n"
		:"=r"(v)
	);
	if(v == 0 && intr_get_nest_count() != 0){
		return SCE_KERNEL_ERROR_ILLEGAL_CONTEXT;
	}

	if(intr_code >= 0x100){
		ksceDebugPrintf("Unknown interrupt code. %d\n", intr_code);
		return SCE_KERNEL_ERROR_ILLEGAL_INTRCODE;
	}

	entry = &(entries[intr_code]);

	prev = ksceKernelCpuSuspendIntr(&(entry->mutex));

	if(entry->handler == NULL || entry->hook_store == NULL){
		ksceKernelCpuResumeIntr(&(entry->mutex), prev);
		return SCE_KERNEL_ERROR_NOTFOUND_HANDLER;
	}

	entry->handler = entry->hook_store;
	entry->hook_store = NULL;

	ksceKernelCpuResumeIntr(&(entry->mutex), prev);

	return 0;
}

int sceKernelReleaseSubIntrHandler(SceUInt32 intr_code, SceUInt32 subintr_code){

	int res, prev;
	SceKernelIntrEntry *entry, *sub_entry;
	SceKernelIntrHandler handler;
	SceUInt32 v;

	asm volatile (
		"mrc p15, #0, %0, c13, c0, #4\n"
		:"=r"(v)
	);
	if(v == 0 && intr_get_nest_count() != 0){
		return SCE_KERNEL_ERROR_ILLEGAL_CONTEXT;
	}

	if(intr_code >= 0x100){
		ksceDebugPrintf("Unknown interrupt code. %d\n", intr_code);
		return SCE_KERNEL_ERROR_ILLEGAL_INTRCODE;
	}

	entry = &(entries[intr_code]);

    prev = ksceKernelCpuSuspendIntr(&(entry->mutex));

	if(entry->handler == NULL){
		ksceDebugPrintf("interrupt handler is not registered. %d\n", intr_code);
		ksceKernelCpuResumeIntr(&(entry->mutex), prev);
		return SCE_KERNEL_ERROR_NOTFOUND_HANDLER;
	}

	if((SceInt32)subintr_code < 0 || entry->unk_0x3C <= subintr_code){
		ksceDebugPrintf("Unknown subinterrupt code. %d-%d\n", intr_code, subintr_code);
		ksceKernelCpuResumeIntr(&(entry->mutex), prev);
		return SCE_KERNEL_ERROR_ILLEGAL_INTRCODE;
	}

	sub_entry = &(entry->sub_intr[subintr_code]);

	if(sub_entry->handler == NULL){
		ksceDebugPrintf("sub interrupt handler is not registered. %d-%d\n", intr_code, subintr_code);
		ksceKernelCpuResumeIntr(&(entry->mutex), prev);
		return SCE_KERNEL_ERROR_NOTFOUND_HANDLER;
	}

	handler = sub_entry->handler;

	if(entry->sub_handler->release_subintr_start != NULL){
		res = entry->sub_handler->release_subintr_start(intr_code, subintr_code);
		if(res < 0) {
			ksceKernelCpuResumeIntr(&(entry->mutex), prev);
			return res;
		}
	}

	sub_entry->handler = NULL;

	if(entry->sub_handler->release_subintr_end != NULL){
		res = entry->sub_handler->release_subintr_end(intr_code, subintr_code);
		if(res < 0){
			sub_entry->handler = handler;
			ksceKernelCpuResumeIntr(&(entry->mutex), prev);
    		return res;
		}
	}

	SceSysmemForKernel_9C7B62AB(sub_entry->name, 0);
	ksceKernelCpuResumeIntr(&(entry->mutex), prev);

	return 0;
}

int sceKernelSuspendSubIntr(SceUInt32 intr_code, SceUInt32 subintr_code, int *a3){

	int res, prev;
	SceKernelIntrEntry *entry;

	entry = &(entries[intr_code]);

	if(intr_code >= 0x100){
		return SCE_KERNEL_ERROR_ILLEGAL_INTRCODE;
	}

	if(a3 == NULL){
		return SCE_KERNEL_ERROR_ERROR;
	}

	prev = ksceKernelCpuSuspendIntr(&(entry->mutex));

	if((SceInt32)subintr_code < 0 || entry->unk_0x3C <= subintr_code){
		ksceKernelCpuResumeIntr(&(entry->mutex), prev);
		return SCE_KERNEL_ERROR_ILLEGAL_INTRCODE;
	}

	if(entry->handler == NULL || entry->sub_handler == NULL || entry->sub_handler->suspend_subintr == NULL){
		ksceKernelCpuResumeIntr(&(entry->mutex), prev);
		return SCE_KERNEL_ERROR_NOTFOUND_HANDLER;
	}

	res = entry->sub_handler->suspend_subintr(intr_code, subintr_code, a3);
	ksceKernelCpuResumeIntr(&(entry->mutex), prev);
	return res;
}

int sceKernelResumeSubIntr(SceUInt32 intr_code, SceUInt32 subintr_code, int data){

	int res, prev;
	SceKernelIntrEntry *entry;

	if(intr_code >= 0x100){
		ksceDebugPrintf("Unknown interrupt code. %d\n", intr_code);
		return SCE_KERNEL_ERROR_ILLEGAL_INTRCODE;
	}

	entry = &(entries[intr_code]);

	prev = ksceKernelCpuSuspendIntr(&(entry->mutex));

	if((SceInt32)subintr_code < 0 || subintr_code >= entry->unk_0x3C){
		ksceDebugPrintf("Unknown subinterrupt code. %d-%d\n", intr_code, subintr_code);
		ksceKernelCpuResumeIntr(&(entry->mutex), prev);
		return SCE_KERNEL_ERROR_ILLEGAL_INTRCODE;
	}

	if(entry->handler == NULL || entry->sub_handler == NULL || entry->sub_handler->resume_subintr == NULL){
		ksceKernelCpuResumeIntr(&(entry->mutex), prev);
		return SCE_KERNEL_ERROR_NOTFOUND_HANDLER;
	}

	res = entry->sub_handler->resume_subintr(intr_code, subintr_code, data);
	ksceKernelCpuResumeIntr(&(entry->mutex), prev);
	return res;
}

int sceKernelSetIntrPriority(SceUInt32 intr_code, SceUInt32 priority){

	int prev;
	SceKernelIntrEntry *entry;

	entry = &(entries[intr_code]);

	if((intr_code - 0x10) > 0xEF){
		return SCE_KERNEL_ERROR_ILLEGAL_INTRCODE;
	}

	if(entry->handler == NULL){
		return SCE_KERNEL_ERROR_NOTFOUND_HANDLER;
	}

	if((priority & 0xFFFFFF0F) != 0){
		return SCE_KERNEL_ERROR_ILLEGAL_INTRPRIORITY;
	}

	prev = ksceKernelCpuSuspendIntr(&(entry->mutex));
	entry->priority = (uint16_t)priority;
	*(char *)(intr_code + SceInterruptControllerReg_0x1000 + 0x400) = (char)priority;
	ksceKernelCpuResumeIntr(&(entry->mutex), prev);

	return 0;
}

int sceKernelSetIntrTargetCpu(SceUInt32 intr_code, SceUInt32 target){

	int prev;
	SceKernelIntrEntry *entry;

	entry = &(entries[intr_code]);
  
	if(intr_code >= 0x100){
		return SCE_KERNEL_ERROR_ILLEGAL_INTRCODE;
	}

	if(entry->handler == NULL){
		return SCE_KERNEL_ERROR_NOTFOUND_HANDLER;
	}

	if(target == 0){
		return SCE_KERNEL_ERROR_ILLEGAL_TARGET_CPU;
	}

	if((target & 0xF) != 0 && (target & 0xF0000) != 0){
		return SCE_KERNEL_ERROR_ILLEGAL_TARGET_CPU;
	}

	if((target & 0xF0000) == 0){
		if((target & 0xF) != 0){
			if((int)target < 0){
				return SCE_KERNEL_ERROR_ILLEGAL_TARGET_CPU;
			}
		}else{
			target = target >> 0x10;
		}
	}else{
		target = target >> 0x10;
	}

	prev = ksceKernelCpuSuspendIntr(&(entry->mutex));
	entry->unk_0x06 = (uint16_t)target;
	*(char *)(intr_code + SceInterruptControllerReg_0x1000 + 0x800) = (char)target;
	ksceKernelCpuResumeIntr(&(entry->mutex), prev);

	return 0;
}

int sceKernelTriggerSGI(SceUInt32 intr_code, SceUInt32 filter, SceUInt32 target_cpu){

	if(intr_code >= 0x10){
		return SCE_KERNEL_ERROR_ILLEGAL_INTRCODE;
	}

	if(filter > 2){
		return SCE_KERNEL_ERROR_ILLEGAL_INTRFILTER;
	}

	if(filter != 0){
		*(SceUInt32 *)(SceInterruptControllerReg_0x1000 + 0xF00) = intr_code | (filter << 0x18);
		return 0;
	}

	if(target_cpu == 0){
		return SCE_KERNEL_ERROR_ILLEGAL_TARGET_CPU;
	}

	if((target_cpu & 0xF) == 0 && (target_cpu & 0xF0000) == 0){
		return SCE_KERNEL_ERROR_ILLEGAL_TARGET_CPU;
	}

	if((target_cpu & 0xF0000) == 0){
		if((target_cpu & 0xF) != 0){
			if((int)target_cpu < 0){
				return SCE_KERNEL_ERROR_ILLEGAL_TARGET_CPU;
			}
		}else{
			target_cpu = target_cpu >> 0x10;
		}
	}else{
		target_cpu = target_cpu >> 0x10;
	}

	*(SceUInt32 *)(SceInterruptControllerReg_0x1000 + 0xF00) = intr_code | (target_cpu << 0x10);

	return 0;
}

int sceKernelTriggerSubIntr(SceUInt32 intr_code, SceUInt32 subintr_code, int a3){

	int res;
	SceKernelIntrEntry *entry, *sub_entry;

	entry = &(entries[intr_code]);

	if(intr_code >= 0x100 || (SceInt32)subintr_code < 0 || entry->unk_0x3C <= subintr_code){
		return SCE_KERNEL_ERROR_ILLEGAL_INTRCODE;
	}

	sub_entry = &(entry->sub_intr[subintr_code]);

	if(sub_entry->handler == NULL){
		return SCE_KERNEL_ERROR_NOTFOUND_HANDLER;
	}

	SceUInt32 v;

	asm volatile (
		"mrc p15, #0, %0, c0, c0, #5\n"
		:"=r"(v)
	);

	sub_entry->nest[v & 3] += 1;

	if(DAT_810882c0 != NULL){
		int (* f)(SceUInt32 args, ...) = DAT_810882c0;
		f(4, intr_code, subintr_code, a3, sub_entry->argp);
	}

	res = sub_entry->handler(a3, sub_entry->argp, entry->priority & 0xFF);

	if(DAT_810882e0 != NULL){
		int (* f)(SceUInt32 args, ...) = DAT_810882e0;
		f(3, intr_code, subintr_code, a3);
	}

	if(intr_check_stack_magic() < 0){
		while(1);
	}

	return res;
}

void SceIntrmgrForKernel_37F4627B(void *a1, void *a2){
	DAT_810882ec = a2;
	DAT_810882f4 = a1;
}

// sceKernelClearIntrPendingStatusForDebugger
int SceIntrmgrForKernel_4DC48A01(SceUInt32 intr_code){

	if(intr_code >= 0x100){
		return SCE_KERNEL_ERROR_ILLEGAL_INTRCODE;
	}

	*(int *)(SceInterruptControllerReg_0x1000 + ((intr_code >> 5) + 0xA0) * 4) = 1 << (intr_code & 0x1F);

	return 0;
}

// sceKernelGetIntrPendingStatusForDebugger
int SceIntrmgrForKernel_A269003D(SceUInt32 intr_code){

	if(intr_code >= 0x100){
		return SCE_KERNEL_ERROR_ILLEGAL_INTRCODE;
	}

	return (*(int *)(SceInterruptControllerReg_0x1000 + ((intr_code >> 5) + 0x80) * 4) >> (intr_code & 0x1F)) & 1;
}

extern int irqEnterCallbackStart;
extern int irqEnterCallbackBranch;
extern int irqLeaveCallbackStart;
extern int irqLeaveCallbackBranch;

// set_irq_debug_callback
void SceIntrmgrForKernel_6FAC5DEF(void *a1, void *a2){

	SceUInt32 opcode;

	if(a1 == NULL){
		opcode = 0xE320F000;
	}else{
		opcode = 0xEA000000 | (((((SceUIntPtr)(&irqEnterCallbackStart) - 8) - (SceUIntPtr)(&irqEnterCallbackBranch)) >> 2) & 0xFFFFFF);
	}

	DAT_810882c4 = a2;
	DAT_810882e4 = a1;

	ksceKernelCpuUnrestrictedMemcpy(&irqEnterCallbackBranch, &opcode, sizeof(opcode));

	if(a2 == NULL){
		opcode = 0xE320F000;
	}else{
		opcode = 0xEA000000 | (((((SceUIntPtr)(&irqLeaveCallbackStart) - 8) - (SceUIntPtr)(&irqLeaveCallbackBranch)) >> 2) & 0xFFFFFF);
	}

	ksceKernelCpuUnrestrictedMemcpy(&irqLeaveCallbackBranch, &opcode, sizeof(opcode));
}

extern int svcEnterCallbackStart;
extern int svcEnterCallbackBranch;
extern int svcLeaveCallbackStart;
extern int svcLeaveCallbackBranch;

// sceKernelSetSyscallEnterLeave
void SceIntrmgrForKernel_CE553E9C(void *a1, void *a2){

	SceUInt32 opcode;

	if(a1 == NULL){
		opcode = 0xBF00;
	}else{
		opcode = 0xE000 | ((((((SceUIntPtr)&svcEnterCallbackStart) - ((SceUIntPtr)&svcEnterCallbackBranch)) >> 1) - 2) & 0x7FF);
	}

	DAT_810882c8 = a1;
	DAT_810882cc = a2;

	ksceKernelCpuUnrestrictedMemcpy(&svcEnterCallbackBranch, &opcode, 2);

	if(a2 == NULL){
		opcode = 0xBF00;
	}else{
		opcode = 0xE000 | ((((((SceUIntPtr)&svcLeaveCallbackStart) - ((SceUIntPtr)&svcLeaveCallbackBranch)) >> 1) - 2) & 0x7FF);
	}

	ksceKernelCpuUnrestrictedMemcpy(&svcLeaveCallbackBranch, &opcode, 2);
}

void SceIntrmgrForKernel_82ADA185(void *a1, void *a2){
	DAT_810882c0 = a1;
	DAT_810882e0 = a2;
}

void SceIntrmgrForKernel_842B45DC(void *a1){
	DAT_810882e8 = a1;
}

int FUN_810014f8(int a1, int a2, int a3){

	if(DAT_810882e8 != NULL){
		int (* f)(int a1, int a2, int a3) = DAT_810882e8;
	    return f(a1, a2, a3);
	}

	return 0;
}

int SceIntrmgrForDriver_C568F145(int a1){

	int args[3];
	int (* f)(int a1, int a2, void *a3) = DAT_810882e8;

	args[0] = a1;

	return f(0x100000, 4, args);
}

// sceKernelSetIrqDBGBVRSaveContext
void SceIntrmgrForKernel_4F890B0C(void *a1){
	DAT_81008240 = a1;
}

// sceKernelSetIrqDBGWVRSaveContext
void SceIntrmgrForKernel_89E47181(void *a1){
	DAT_81008244 = a1;
}

int sceKernelAllocSystemCallTable(SceUInt32 number, void **base){

	SceUID memid;

	if(number == 0 || DAT_810882d8 != 0 || DAT_810882d4 != NULL){
		return -1;
	}

	memid = sceKernelAllocPartitionMemBlock(0x10009, "SceSyscallTable", 0x1020D006, number << 2, NULL);
	if(memid < 0){
		return memid;
	}

	DAT_810882d8 = memid;
	ksceKernelGetMemBlockBase(memid, base);
	DAT_810882d4 = *base;

	return 0;
}

void *SceIntrmgrForKernel_EF1D3865(void){
	return SceInterruptControllerReg_0x100;
}

typedef struct SceKernelIntrHandlerInfo { // size is 0x70-bytes
	SceSize size;
	void *handler;
	void *argp;
	SceUInt16 intr_code;
	SceUInt16 interrupt_type;

	SceUInt16 subintr_num;
	SceUInt16 priority;
	SceUInt16 target_cpu;
	SceUInt16 some_intr_state;
	int nest[4];

	char unk_0x28[0x10];
	char unk_0x38[0x10];
	char unk_0x48[0x20];
	char *name;
	int interrupt_type2;
} SceKernelIntrHandlerInfo;


int sceKernelQueryIntrHandlerInfo(SceUInt32 intr_code, SceUInt32 subintr_code, SceKernelIntrHandlerInfo *dst){

	int sr_data;
	SceKernelIntrEntry *entry;

	if(intr_code >= 0x100){
		return SCE_KERNEL_ERROR_ILLEGAL_INTRCODE;
	}

	if(dst->size != sizeof(SceKernelIntrHandlerInfo)){
	    return SCE_KERNEL_ERROR_ILLEGAL_INTRPARAM;
	}

	entry = &(entries[intr_code]);

	if(entry->unk_0x3C != 0){
		if((SceInt32)subintr_code < 0){
			if(subintr_code != 0xFFFFFFFF){
				return SCE_KERNEL_ERROR_ILLEGAL_INTRCODE;
			}
		}else{
			if(entry->unk_0x3C <= subintr_code){
				return SCE_KERNEL_ERROR_ILLEGAL_INTRCODE;
			}

			entry = &(entry->sub_intr[subintr_code]);
		}
	}

	dst->handler = (entry->hook_store != NULL) ? entry->hook_store : entry->handler;

	dst->argp            = entry->argp;
	dst->intr_code       = (SceUInt16)intr_code;
	dst->priority        = entry->priority >> 4;
	dst->target_cpu      = entry->unk_0x06;
	dst->interrupt_type  = (SceUInt16)entry->interrupt_type;
	dst->some_intr_state = (*(int *)(SceInterruptControllerReg_0x1000 + ((intr_code >> 5) + 0x60) * 4) >> (intr_code & 0x1F)) & 1;

	if(dst->some_intr_state != 0 && (int)subintr_code >= 0){
		if(entry->handler != NULL){

			if(entry->sub_handler != NULL && entry->sub_handler->suspend_subintr != NULL && entry->sub_handler->resume_subintr != NULL){
				entry->sub_handler->suspend_subintr(intr_code, subintr_code, &sr_data);
				entry->sub_handler->resume_subintr(intr_code, subintr_code, sr_data);

				dst->some_intr_state = sr_data != 0;
			}
		}else{
			dst->some_intr_state = 0;
		}
	}

	dst->subintr_num = (SceUInt16)entry->unk_0x3C;
	dst->nest[0] = entry->nest[0];
	dst->nest[1] = entry->nest[1];
	dst->nest[2] = entry->nest[2];
	dst->nest[3] = entry->nest[3];

	memset(dst->unk_0x28, 0, 0x10);
	memset(dst->unk_0x38, 0, 0x10);
	memset(dst->unk_0x48, 0, 0x20);

	if(dst->name != NULL){
		if(entry->name == NULL){
			dst->name[0] = 0;
		}else{
			memcpy(dst->name, entry->name, 0x20);
		}
	}

	dst->interrupt_type2 = entry->interrupt_type & 0x10;

	return 0;
}

void invalidApiStatCfunc(void *a1, int a2, int a3){
	ksceDebugPrintf("warning: detect invalid api stat(0x%08X), lr = 0x%08X, syscall_entry 0x%08x\n", a3, *(int *)(a1 + 0x58), *(int *)(a1 + 0x38));
}

static const SceKernelDebugMessageContext L925_ctx = {
	.hex_value0_hi = 0xDA5190B1,
	.hex_value0_lo = 0x3BF21BBA,
	.hex_value1    = 0xD0FCCF36,
	.file          = "intrmgr.c",
	.line          = 0x925,
	.func          = "invalidFastMutexOwnerCfunc"
};

typedef struct __SceThreadmgrLinker { // fake struct
	struct __SceThreadmgrLinker *next;
	struct __SceThreadmgrLinker *prev;
	SceUID id;
} __SceThreadmgrLinker;

void invalidFastMutexOwnerCfunc(void){

	SceUID guid;
	SceUIntPtr v;

	asm volatile ("mrc p15, #0, %0, c13, c0, #4\n":"=r"(v));

	ksceDebugPrintf("\n*** FastMutex has not been unlocked after syscall.\n");

	guid = *(SceUID *)(v + 0x38);
	ksceDebugPrintf("** ProcessID: 0x%08X [%s]\n", guid, ksceKernelGetNameForUid2(guid));

	guid = *(SceUID *)(v + 8);
	ksceDebugPrintf("** ProcessID: 0x%08X [%s]\n", guid, ksceKernelGetNameForUid2(guid));

	const __SceThreadmgrLinker *link = *(__SceThreadmgrLinker **)(v + 0x68);

	for(;link != (const __SceThreadmgrLinker *)(v + 0x68);link = link->next){
		ksceDebugPrintf("*  FastMutex: 0x%08X [%s]\n", link->id, ksceKernelGetNameForUid2(link->id));
	}

	ksceDebugPrintKernelPanic(&L925_ctx, __builtin_return_address(0));
}

int intr_susppend_handler_0(){
	return 0;
}

void intr_reset_ctrl_reg(void){

	int n, v;

	*(volatile int *)(SceInterruptControllerReg_0x100) = 0;

	if(ksceKernelCpuGetCpuId() == 0){
		*(volatile int *)(SceInterruptControllerReg_0x1000) = 0;
	}

	v = 0;
	n = *(volatile int *)(SceInterruptControllerReg_0x1000 + 4);

	do {
		*(volatile int *)(SceInterruptControllerReg_0x1000 + 0x180 + (v << 2)) = 0xFFFFFFFF;
		v++;
	} while(v != ((n & 0x1F) + 1));

	*(volatile int *)(SceInterruptControllerReg_0x100) = 1;

	if(ksceKernelCpuGetCpuId() == 0){
		*(volatile int *)(SceInterruptControllerReg_0x1000) = 1;
	}
}

int intr_susppend_handler_1(){

	sceKernelSysrootCorelockLock(0);
	intr_reset_ctrl_reg();
	sceKernelSysrootCorelockUnlock();

	sceKernelSysrootCorelockLock(0);

	if(ksceKernelCpuGetCpuId() == 0){
		for(int i=0;i<0x100;i++){
			SceKernelIntrEntry *entry = &(entries[i]);

			*(char *)(SceInterruptControllerReg_0x1000 + i + 0x400) = entry->priority;
			*(char *)(SceInterruptControllerReg_0x1000 + i + 0x800) = entry->unk_0x06;

			if(entry->unk_0x0F == 3){
				int shift = (i & 0xF) << 1;
				int value = *(int *)(SceInterruptControllerReg_0x1000 + ((i >> 4) + 0x300) * 4);

				*(int *)(SceInterruptControllerReg_0x1000 + ((i >> 4) + 0x300) * 4) = ((((value >> shift) ^ 3) & 3) << shift) ^ value;
			}
		}

		SceUIntPtr address;

		ksceKernelVAtoPA(sceKernelSysrootGetSharedMemory(), &address);
		AllocSharedMemory_S(address, 0xA000);
	}

	sceKernelSysrootCorelockUnlock();

	return 0;
}

void detectIntrStackOverflowCfunc(int count, int code){

	ksceDebugPrintf("!!! detect intr stack overflow !!!\n");
	ksceDebugPrintf("     CPU = %d, intr name %s (0x%08x:%d), nest count = %d\n", ksceKernelCpuGetCpuId(), entries[code & 0x3FF].name, code, code & 0x3FF, count);

	do {
	} while(1);
}

static const SceKernelDebugMessageContext L932_ctx = {
	.hex_value0_hi = 0xDA5190B1,
	.hex_value0_lo = 0x9B788DAB,
	.hex_value1    = 0x8552473F,
	.file          = "intrmgr.c",
	.line          = 0x932,
	.func          = "illegalUserDACRCfunc"
};

void illegalUserDACRCfunc(void *a1, SceUInt32 DACR){

	SceUID guid;

	ksceDebugPrintf("\n*** Illegal User DACR = 0x%08X.\n", DACR);

	guid = *(SceUID *)(a1 + 0x38);
	ksceDebugPrintf("** ProcessID: 0x%08X [%s]\n", guid, ksceKernelGetNameForUid2(guid));

	guid = *(SceUID *)(a1 + 8);
	ksceDebugPrintf("** ThreadID:  0x%08X [%s]\n", guid, ksceKernelGetNameForUid2(guid));

	ksceDebugPrintKernelPanic(&L932_ctx, __builtin_return_address(0));
}

int module_bootstart(SceSize args, void *argp){

	ksceKernelCpuUnrestrictedMemcpy(sceKernelSysrootGetVbaseResetVector() + 0x28 + ksceKernelCpuGetCpuId() * 0x40, (void *[1]){&(nest[ksceKernelCpuGetCpuId()])}, 4);

	return SCE_KERNEL_START_SUCCESS;
}

void _start() __attribute__ ((weak, alias ("sceKernelIntrMgrModuleStart")));
int sceKernelIntrMgrModuleStart(SceSize args, void *argp){

	int res;
	void *base;
	SceKernelAllocMemBlockKernelOpt opt;
	SceKernelHeapCreateOpt heapopt;

	do {
		memset(&opt, 0, sizeof(opt));
		opt.size = 0x58;
		opt.attr = 2;
		opt.paddr = 0x1A000000;

		res = ksceKernelAllocMemBlock("SceInterruptControllerReg", 0x20100206, 0x2000, &opt);
		if(res < 0){
			break;
		}

		ksceKernelGetMemBlockBase(res, &base);

		SceInterruptControllerReg_0x100  = base + 0x100;
		SceInterruptControllerReg_0x1000 = base + 0x1000;

		intr_reset_ctrl_reg();

		nest[ksceKernelCpuGetCpuId()].nestCount = 0;

		if(ksceKernelCpuGetCpuId() == 0){
			res = ksceExcpmgrRegisterHandler(6, 0, sceKernelIrqHandler);
			if(res < 0){
				break;
			}

			heapopt.size = sizeof(heapopt);
			heapopt.attr = 1;
			res = ksceKernelCreateHeap("SceIntrMgr", 0x8000, &heapopt);
			if(res < 0){
				break;
			}

			DAT_810882f0 = res;
			ksceExcpmgrRegisterHandler(2, 0, sceKernelSvcHandler);
			SceKernelSuspendForDriver_CE7A2207(0x1D, intr_susppend_handler_0, 0);
			SceKernelSuspendForDriver_105C5752(0x1D, intr_susppend_handler_1, 0);

			DAT_81008200 = SceSysrootForKernel_118657C6();
			DAT_81008240 = 0;
			DAT_81008244 = 0;

			for(int i=0;i<0x100;i++){
				entries[i].handler = NULL;
				entries[i].mutex = 0;
			}
		}

		sceKernelSysrootSetModulePrivate(5, entries);
		DAT_810882c8 = 0;
		DAT_810882cc = 0;
		DAT_810882e4 = 0;
		DAT_810882c4 = 0;
		DAT_810882c0 = 0;
		DAT_810882e0 = 0;
		intr_init_stack_magic(0x1ab4e8df);

		res = 0;
	} while(0);

	return res;
}
