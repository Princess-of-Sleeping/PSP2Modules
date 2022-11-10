
	.balign 4
	.text
	.cpu cortex-a9
	.arch armv7-a
	.syntax unified
	.thumb
	.thumb_func
	.fpu neon
	.arch_extension sec

	.globl SceIntrmgrForKernel_A60D79A4
	.type  SceIntrmgrForKernel_A60D79A4, %function

// enable_irq
SceIntrmgrForKernel_A60D79A4:
	cpsie      i
	bx         lr

	.data
