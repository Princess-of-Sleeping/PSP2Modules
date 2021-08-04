/**
 * \usergroup{SceSDbgSdio}
 * \usage{psp2kern/kernel/sdbgsdio.h,SceSDbgSdioForKernel_stub}
 */


#ifndef _PSP2_KERNEL_SDBGSDIO_H_
#define _PSP2_KERNEL_SDBGSDIO_H_

#include <psp2kern/types.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Init sdbgsdio
 *
 * @return zero or error code
 */
int sceSDbgSdioInit(void);

/**
 * Fini sdbgsdio
 *
 * @return always zero
 */
int sceSDbgSdioFini(void);

/**
 * Read sdbgsdio data
 *
 * @param[in]  core - The using sdbgsdio core
 * @param[out] data - The read output buffer pointer
 * @param[in]  size - The read size
 *
 * @return read size or error code
 */
int sceSDbgSdioRead(SceUInt32 core, void *data, SceSize size);


/**
 * Write sdbgsdio data
 *
 * @param[in] core - The using sdbgsdio core
 * @param[in] data - The write output buffer pointer
 * @param[in] size - The write size
 *
 * @return read size or error code
 */
int sceSDbgSdioWrite(SceUInt32 core, const void *data, SceSize size);

#ifdef __cplusplus
}
#endif

#endif /* _PSP2_KERNEL_SDBGSDIO_H_ */
