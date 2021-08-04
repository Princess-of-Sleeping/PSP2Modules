
#ifndef _PSP2_KERNEL_SDBGSDIO_INTERNAL_H_
#define _PSP2_KERNEL_SDBGSDIO_INTERNAL_H_

#include <psp2kern/types.h>

#ifdef __cplusplus
extern "C" {
#endif

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

#ifdef __cplusplus
}
#endif

#endif /* _PSP2_KERNEL_SDBGSDIO_INTERNAL_H_ */
