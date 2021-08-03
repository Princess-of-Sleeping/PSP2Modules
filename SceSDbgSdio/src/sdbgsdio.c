/*
 * PS Vita SceSDbgSdio driver
 * Copyright (C) 2021, Princess of Sleeping
 */

#include <psp2kern/kernel/modulemgr.h>
#include <psp2kern/kernel/cpu.h>
#include <psp2kern/kernel/dipsw.h>
#include <psp2kern/kernel/sysmem.h>

int SceKernelSuspendForDriver_CE7A2207(int a1, void *a2, int a3);
int SceKernelSuspendForDriver_105C5752(int a1, void *a2, int a3);
void SceSysrootForKernel_8E4B61F1(int a1);
void SceSysrootForKernel_AE55B7CC(void);
int ksceKernelMaskIntr(int intr_code);
int ksceKernelSetIntrMasked(int intr_code);

typedef struct SceSDbgSdioData {
	volatile void *mem_base;
	volatile int *data_0x04;
	volatile int *data_0x08;
	volatile int *data_0x0C;

	volatile void *data_0x10;
	volatile char *data_0x14;
	volatile void *data_0x18;
	volatile char *data_0x1C;

	volatile char *data_0x20;
	int data_0x24;
	int data_0x28;
	int data_0x2C;

	unsigned int data_0x30;
	volatile char *data_0x34;
	volatile char *data_0x38;
	volatile char *data_0x3C;

	volatile char *data_0x40;
	int data_0x44;
	int data_0x48;
	int data_0x4C;

	int data_0x50;
	int data_0x54;
	char data_0x58[0x11000];

	char   *data_0x11058;
	SceSize data_0x1105C;
} SceSDbgSdioData; // size is 0x11060-bytes

typedef struct SceSDbgSdioContext {
	int data_0x00;
	SceUID memid;
	int data_0x08;
	int data_0x0C;
	SceSDbgSdioData sdio_data[4];
} SceSDbgSdioContext; // size is 0x44190-bytes

SceSDbgSdioContext sdio_ctx;

int sdbgsdio_reset_config(void){

	if(*(volatile int *)(sdio_ctx.sdio_data[0].mem_base + 0x40) != 1){
		*(volatile char *)(sdio_ctx.sdio_data[0].mem_base + 0x400) = 0x20;
		*(volatile char *)(sdio_ctx.sdio_data[0].mem_base + 0x401) = 4;
		*(volatile char *)(sdio_ctx.sdio_data[0].mem_base + 0x402) = 0x55;
		*(volatile char *)(sdio_ctx.sdio_data[0].mem_base + 0x403) = 0xFF;
		*(volatile char *)(sdio_ctx.sdio_data[0].mem_base + 0x404) = 0xBB;
		*(volatile char *)(sdio_ctx.sdio_data[0].mem_base + 0x405) = 0xAA;
		*(volatile char *)(sdio_ctx.sdio_data[0].mem_base + 0x406) = 0x22;
		*(volatile char *)(sdio_ctx.sdio_data[0].mem_base + 0x407) = 4;
		*(volatile char *)(sdio_ctx.sdio_data[0].mem_base + 0x408) = 0;
		*(volatile char *)(sdio_ctx.sdio_data[0].mem_base + 0x409) = 0;
		*(volatile char *)(sdio_ctx.sdio_data[0].mem_base + 0x40A) = 8;
		*(volatile char *)(sdio_ctx.sdio_data[0].mem_base + 0x40B) = 0x32;
		*(volatile char *)(sdio_ctx.sdio_data[0].mem_base + 0x40C) = 0xFF;
		*(volatile char *)(sdio_ctx.sdio_data[0].mem_base + 0x40D) = 1;
		*(volatile char *)(sdio_ctx.sdio_data[0].mem_base + 0x480) = 0x20;
		*(volatile char *)(sdio_ctx.sdio_data[0].mem_base + 0x481) = 4;
		*(volatile char *)(sdio_ctx.sdio_data[0].mem_base + 0x482) = 0x55;
		*(volatile char *)(sdio_ctx.sdio_data[0].mem_base + 0x483) = 0xFF;
		*(volatile char *)(sdio_ctx.sdio_data[0].mem_base + 0x484) = 0xBB;
		*(volatile char *)(sdio_ctx.sdio_data[0].mem_base + 0x485) = 0xAA;
		*(volatile char *)(sdio_ctx.sdio_data[0].mem_base + 0x486) = 0x22;
		*(volatile char *)(sdio_ctx.sdio_data[0].mem_base + 0x487) = 0x2A;
		*(volatile char *)(sdio_ctx.sdio_data[0].mem_base + 0x488) = 1;
		*(volatile char *)(sdio_ctx.sdio_data[0].mem_base + 0x489) = 1;
		*(volatile char *)(sdio_ctx.sdio_data[0].mem_base + 0x48A) = 0;
		*(volatile char *)(sdio_ctx.sdio_data[0].mem_base + 0x48B) = 0;
		*(volatile char *)(sdio_ctx.sdio_data[0].mem_base + 0x48C) = 0;
		*(volatile char *)(sdio_ctx.sdio_data[0].mem_base + 0x48D) = 0;
		*(volatile char *)(sdio_ctx.sdio_data[0].mem_base + 0x48E) = 0;
		*(volatile char *)(sdio_ctx.sdio_data[0].mem_base + 0x48F) = 0;
		*(volatile char *)(sdio_ctx.sdio_data[0].mem_base + 0x490) = 0;
		*(volatile char *)(sdio_ctx.sdio_data[0].mem_base + 0x491) = 0;
		*(volatile char *)(sdio_ctx.sdio_data[0].mem_base + 0x492) = 0;
		*(volatile char *)(sdio_ctx.sdio_data[0].mem_base + 0x493) = 0;
		*(volatile char *)(sdio_ctx.sdio_data[0].mem_base + 0x494) = 0;
		*(volatile char *)(sdio_ctx.sdio_data[0].mem_base + 0x495) = 8;
		*(volatile char *)(sdio_ctx.sdio_data[0].mem_base + 0x496) = 0;
		*(volatile char *)(sdio_ctx.sdio_data[0].mem_base + 0x497) = 0;
		*(volatile char *)(sdio_ctx.sdio_data[0].mem_base + 0x498) = ~3;
		*(volatile char *)(sdio_ctx.sdio_data[0].mem_base + 0x499) = 0;
		*(volatile char *)(sdio_ctx.sdio_data[0].mem_base + 0x49A) = 0;
		*(volatile char *)(sdio_ctx.sdio_data[0].mem_base + 0x49B) = 0;
		*(volatile char *)(sdio_ctx.sdio_data[0].mem_base + 0x49C) = 0;
		*(volatile char *)(sdio_ctx.sdio_data[0].mem_base + 0x49D) = 0;
		*(volatile char *)(sdio_ctx.sdio_data[0].mem_base + 0x49E) = 0;
		*(volatile char *)(sdio_ctx.sdio_data[0].mem_base + 0x49F) = 0;
		*(volatile char *)(sdio_ctx.sdio_data[0].mem_base + 0x4A0) = 0;
		*(volatile char *)(sdio_ctx.sdio_data[0].mem_base + 0x4A1) = 0;
		*(volatile char *)(sdio_ctx.sdio_data[0].mem_base + 0x4A2) = 0;
		*(volatile char *)(sdio_ctx.sdio_data[0].mem_base + 0x4A3) = 0;
		*(volatile char *)(sdio_ctx.sdio_data[0].mem_base + 0x4A4) = 0;
		*(volatile char *)(sdio_ctx.sdio_data[0].mem_base + 0x4A5) = 0;
		*(volatile char *)(sdio_ctx.sdio_data[0].mem_base + 0x4A6) = 0;
		*(volatile char *)(sdio_ctx.sdio_data[0].mem_base + 0x4A7) = 0;
		*(volatile char *)(sdio_ctx.sdio_data[0].mem_base + 0x4A8) = 0;
		*(volatile char *)(sdio_ctx.sdio_data[0].mem_base + 0x4A9) = 0;
		*(volatile char *)(sdio_ctx.sdio_data[0].mem_base + 0x4AA) = 0;
		*(volatile char *)(sdio_ctx.sdio_data[0].mem_base + 0x4AB) = 0;
		*(volatile char *)(sdio_ctx.sdio_data[0].mem_base + 0x4AC) = 0;
		*(volatile char *)(sdio_ctx.sdio_data[0].mem_base + 0x4AD) = 0;
		*(volatile char *)(sdio_ctx.sdio_data[0].mem_base + 0x4AE) = 0;
		*(volatile char *)(sdio_ctx.sdio_data[0].mem_base + 0x4AF) = 0;
		*(volatile char *)(sdio_ctx.sdio_data[0].mem_base + 0x4B0) = 0;
		*(volatile char *)(sdio_ctx.sdio_data[0].mem_base + 0x4B1) = 0;
		*(volatile char *)(sdio_ctx.sdio_data[0].mem_base + 0x4B2) = 0x21;
		*(volatile char *)(sdio_ctx.sdio_data[0].mem_base + 0x4B3) = 2;
		*(volatile char *)(sdio_ctx.sdio_data[0].mem_base + 0x4B4) = 0xC;
		*(volatile char *)(sdio_ctx.sdio_data[0].mem_base + 0x4B5) = 0;
		*(volatile char *)(sdio_ctx.sdio_data[0].mem_base + 0x4B6) = 0x80;
		*(volatile char *)(sdio_ctx.sdio_data[0].mem_base + 0x4B7) = 2;
		*(volatile char *)(sdio_ctx.sdio_data[0].mem_base + 0x4B8) = 1;
		*(volatile char *)(sdio_ctx.sdio_data[0].mem_base + 0x4B9) = 1;
		*(volatile char *)(sdio_ctx.sdio_data[0].mem_base + 0x4BA) = 0xFF;
		*(volatile char *)(sdio_ctx.sdio_data[0].mem_base + 0x4BB) = 1;
		*(volatile int *)(sdio_ctx.sdio_data[0].mem_base + 0x40) = 0;
		*(volatile int *)(sdio_ctx.sdio_data[0].mem_base + 0x40) = 1;
	}
	return 0;
}

int sceSDbgSdioInit(void){

	SceKernelAllocMemBlockKernelOpt opt;
	SceUID memid;
	void *mem_base;

	memset(&sdio_ctx, 0, sizeof(sdio_ctx));
	memset(&opt, 0, sizeof(opt));
	opt.size = 0x58;
	opt.attr = 2;
	opt.paddr = 0xE5800000;

	memid = ksceKernelAllocMemBlock("SceSDbgSdio0", 0x20100806, 0x10000, &opt);
	if(memid < 0)
		return memid;

	ksceKernelGetMemBlockBase(memid, &mem_base);

	sdio_ctx.data_0x00 = 0;
	sdio_ctx.memid     = memid;
	sdio_ctx.data_0x08 = 1;
	sdio_ctx.data_0x0C = 4;

	for(int i=0;i<4;i++){
		sdio_ctx.sdio_data[i].mem_base  = mem_base;
		sdio_ctx.sdio_data[i].data_0x04 = mem_base + 0x108 + 0x10 * i;
		sdio_ctx.sdio_data[i].data_0x08 = mem_base + 0x100 + 0x10 * i;
		sdio_ctx.sdio_data[i].data_0x0C = mem_base + 0x100 + 0x10 * i;

		sdio_ctx.sdio_data[i].data_0x10 = mem_base + 0x108 + 0x10 * i;
		sdio_ctx.sdio_data[i].data_0x14 = mem_base + 0xC00 + 0x100 * i;
		sdio_ctx.sdio_data[i].data_0x18 = mem_base + 0xC10 + 0x100 * i;
		sdio_ctx.sdio_data[i].data_0x1C = mem_base + 0xC20 + 0x100 * i;

		sdio_ctx.sdio_data[i].data_0x20 = mem_base + 0xC00 + 0x100 * i;
		sdio_ctx.sdio_data[i].data_0x24 = 0xE0;
		sdio_ctx.sdio_data[i].data_0x28 = 0;
		sdio_ctx.sdio_data[i].data_0x2C = 0xC00 + 0x100 * i;

		sdio_ctx.sdio_data[i].data_0x30 = 0x100;
		sdio_ctx.sdio_data[i].data_0x34 = mem_base + 0x800 + 0x100 * i;
		sdio_ctx.sdio_data[i].data_0x38 = mem_base + 0x810 + 0x100 * i;
		sdio_ctx.sdio_data[i].data_0x3C = mem_base + 0x820 + 0x100 * i;

		sdio_ctx.sdio_data[i].data_0x40 = mem_base + 0x800 + 0x100 * i;
		sdio_ctx.sdio_data[i].data_0x44 = 0xE0;
		sdio_ctx.sdio_data[i].data_0x48 = 0;
		sdio_ctx.sdio_data[i].data_0x4C = 0x800 + 0x100 * i;

		sdio_ctx.sdio_data[i].data_0x50 = 0x100;
		sdio_ctx.sdio_data[i].data_0x54 = i;

		sdio_ctx.sdio_data[i].data_0x11058 = NULL;
		sdio_ctx.sdio_data[i].data_0x1105C = 0;
	}

	sdbgsdio_reset_config();

	return 0;
}

int sceSDbgSdioFini(void){
	ksceKernelFreeMemBlock(sdio_ctx.memid);
	return 0;
}

int sceSDbgSdioRead(SceUInt32 core, void *data, SceSize size){

	SceBool wait_read_comp;
	SceSize size_remain, size_read, recv_size, cached_data_size;
	char *read_data_ptr, *next_read_point;
	int has_read_cache;
	uint32_t send_buffer[4], recv_buffer[4];

	if(core >= 4)
		return 0x80020005;

	SceSDbgSdioData *pSdioData = &sdio_ctx.sdio_data[core];

	cached_data_size = pSdioData->data_0x1105C;
	if(cached_data_size != 0){
		if(size < cached_data_size)
			cached_data_size = size;

		memcpy(data, pSdioData->data_0x11058, cached_data_size);
		pSdioData->data_0x11058 += cached_data_size;
		pSdioData->data_0x1105C -= cached_data_size;
	}

	if(size <= cached_data_size)
		return cached_data_size;

	if(((*pSdioData->data_0x04) & 0xFFFF) == 0)
		return cached_data_size;

	while(((*pSdioData->data_0x04) & 3) == 0);
	*pSdioData->data_0x04 = 3;
	asm volatile("dsb sy\n");

	for(int i=0;i<0x10;i++)
		((char *)&recv_buffer[0])[i] = pSdioData->data_0x34[i];

	size -= cached_data_size;
	next_read_point = data + cached_data_size;

	if((recv_buffer[1] & 2) == 0){
		if(recv_buffer[0] > size){
			has_read_cache = 1;
			recv_size = size;
		}else{
			has_read_cache = 0;
			recv_size = recv_buffer[0];
		}

		send_buffer[0] = pSdioData->data_0x4C;
		send_buffer[1] = recv_buffer[0];
		send_buffer[2] = 0;
		send_buffer[3] = pSdioData->data_0x50;

		for(int i=0;i<0x10;i++)
			pSdioData->data_0x38[i] = ((char *)&send_buffer[0])[i];

		*pSdioData->data_0x0C = 0x20000;

		while((*pSdioData->data_0x04 & 4) == 0);
		*pSdioData->data_0x04 = 4;
		asm volatile("dsb sy\n");

		if(has_read_cache != 0){
			read_data_ptr = pSdioData->data_0x58;
			pSdioData->data_0x11058 = read_data_ptr;
			pSdioData->data_0x1105C = recv_buffer[0];
		}else{
			read_data_ptr = next_read_point;
		}

		wait_read_comp = SCE_FALSE;
		size_remain = recv_buffer[0];
		while(size_remain != 0){
			size_read = size_remain;
			if(send_buffer[3] <= size_remain)
				size_read = send_buffer[3];

			if(wait_read_comp){
				while((*pSdioData->data_0x04 & 4) == 0);
				*pSdioData->data_0x04 = 4;
				asm volatile("dsb sy\n");
			}

			for(int i=0;i<size_read;i++)
				read_data_ptr[i] = pSdioData->data_0x40[i];

			*pSdioData->data_0x0C = 0x40000;

			read_data_ptr += size_read;
			size_remain -= size_read;
			wait_read_comp = SCE_TRUE;
		}

		if(has_read_cache != 0){
			memcpy(next_read_point, pSdioData->data_0x11058, size);
			pSdioData->data_0x11058 += size;
			pSdioData->data_0x1105C -= size;
		}
	}else{
		while((*pSdioData->data_0x04 & 4) == 0);
		*pSdioData->data_0x04 = 4;
		asm volatile("dsb sy\n");

		if(size < recv_buffer[0]){
			for(int i=0;i<size;i++)
				next_read_point[i] = pSdioData->data_0x3C[i];

			pSdioData->data_0x11058 = pSdioData->data_0x58;
			pSdioData->data_0x1105C = recv_buffer[0] - size;

			for(int i=0;i<(recv_buffer[0] - size);i++)
				pSdioData->data_0x58[i] = pSdioData->data_0x3C[size + i];

			recv_size = size;
		}else{
			for(int i=0;i<recv_buffer[0];i++)
				next_read_point[i] = pSdioData->data_0x3C[i];

			recv_size = recv_buffer[0];
		}
		*pSdioData->data_0x0C = 0x40000;
	}

	return recv_size + cached_data_size;
}

int sceSDbgSdioWrite(SceUInt32 core, const void *data, SceSize size){

	SceBool wait_write_comp;
	SceSize size_remain, size_write;
	const char *write_data_ptr;
	int send_buffer[4];

	if(core >= 4)
		return 0x80020005;

	SceSDbgSdioData *pSdioData = &sdio_ctx.sdio_data[core];

	if(size <= pSdioData->data_0x24){
		write_data_ptr = data + size;

		for(int i=0;i<size;i++)
			pSdioData->data_0x1C[i] = *(char *)(data + i);

		send_buffer[0] = pSdioData->data_0x2C + 0x20;
		send_buffer[1] = size;
		send_buffer[2] = 2;
		send_buffer[3] = pSdioData->data_0x30;

		size_remain = 0;
	}else{
		write_data_ptr = data;

		send_buffer[0] = pSdioData->data_0x2C;
		send_buffer[1] = size;
		send_buffer[2] = 0;
		send_buffer[3] = pSdioData->data_0x30;

		size_remain = size;
	}

	for(int i=0;i<0x10;i++)
		pSdioData->data_0x14[i] = ((char *)&send_buffer[0])[i];

	ksceKernelMaskIntr(0xB0 + pSdioData->data_0x54);
	*pSdioData->data_0x08 = 2;

	if(size_remain != 0){
		while((*pSdioData->data_0x04 & 0x20000) == 0);
		*pSdioData->data_0x04 = 0x20000;
		asm volatile("dsb sy\n");
		ksceKernelSetIntrMasked(0xB0 + pSdioData->data_0x54);

		wait_write_comp = SCE_FALSE;
		while( size_remain != 0 ){
			size_write = size_remain;
			if (pSdioData->data_0x30 <= size_remain)
				size_write = pSdioData->data_0x30;

			if(wait_write_comp){
				while((*pSdioData->data_0x04 & 0x40000) == 0);
				*pSdioData->data_0x04 = 0x40000;
				asm volatile("dsb sy\n");
				ksceKernelSetIntrMasked(0xB0 + pSdioData->data_0x54);
			}

			for(int i=0;i<size_write;i++)
				pSdioData->data_0x20[i] = write_data_ptr[i];

			ksceKernelMaskIntr(0xB0 + pSdioData->data_0x54);
			*pSdioData->data_0x08 = 4;

			wait_write_comp = SCE_TRUE;
			size_remain -= size_write;
			write_data_ptr += size_write;
		}
	}

	while((*pSdioData->data_0x04 & 0x40000) == 0);
	*pSdioData->data_0x04 = 0x40000;
	asm volatile("dsb sy\n");
	ksceKernelSetIntrMasked(0xB0 + pSdioData->data_0x54);

	return size;
}

int sdbgsdio_reset_config_flag_handler(void){
	SceSysrootForKernel_8E4B61F1(0);

	if(ksceKernelCpuGetCpuId() == 0)
		*(uint32_t *)(sdio_ctx.sdio_data[0].mem_base + 0x40) = 0;

	SceSysrootForKernel_AE55B7CC();
	return 0;
}

int sdbgsdio_reset_config_handler(void){
	if(ksceKernelCpuGetCpuId() == 0)
		sdbgsdio_reset_config();

	return 0;
}

void _start() __attribute__ ((weak, alias ("module_start")));
int module_start(SceSize args, void *argp){

	if(ksceKernelCheckDipsw(0xC1) == 0)
		return SCE_KERNEL_START_NO_RESIDENT;

	if(sceSDbgSdioInit() < 0)
		return SCE_KERNEL_START_NO_RESIDENT;

	SceKernelSuspendForDriver_CE7A2207(0x1A, sdbgsdio_reset_config_flag_handler, 0);
	SceKernelSuspendForDriver_105C5752(0x1A, sdbgsdio_reset_config_handler, 0);

	return SCE_KERNEL_START_SUCCESS;
}
