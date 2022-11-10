
	.balign 4
	.text
	.cpu cortex-a9
	.arch armv7-a
	.syntax unified
	.thumb
	.thumb_func
	.fpu neon
	.arch_extension sec

	.globl SceIntrmgrForKernel_01E5233E
	.type  SceIntrmgrForKernel_01E5233E, %function

SceIntrmgrForKernel_01E5233E:
	push  {r4, r5, r6, r7, r8, r9, r10, r11}
	ldm.w r1!, {r4, r5, r6, r7, r8, r9, r10, r11}
	stm.w r0!, {r4, r5, r6, r7, r8, r9, r10, r11}
	ldm.w r1!, {r4, r5, r6, r7, r8, r9, r10, r11}
	stm.w r0!, {r4, r5, r6, r7, r8, r9, r10, r11}
	ldm.w r1!, {r4, r5, r6, r7, r8, r9, r10, r11}
	stm.w r0!, {r4, r5, r6, r7, r8, r9, r10, r11}
	ldm.w r1!, {r4, r5, r6, r7, r8, r9, r10, r11}
	stm.w r0!, {r4, r5, r6, r7, r8, r9, r10, r11}
	ldm.w r1!, {r4, r5, r6, r7, r8, r9, r10, r11}
	stm.w r0!, {r4, r5, r6, r7, r8, r9, r10, r11}
	ldm.w r1!, {r4, r5, r6, r7, r8, r9, r10, r11}
	stm.w r0!, {r4, r5, r6, r7, r8, r9, r10, r11}
	ldm.w r1!, {r4, r5, r6, r7, r8, r9, r10, r11}
	stm.w r0!, {r4, r5, r6, r7, r8, r9, r10, r11}
	ldm.w r1!, {r4, r5, r6, r7, r8, r9, r10, r11}
	stm.w r0!, {r4, r5, r6, r7, r8, r9, r10, r11}
	ldm.w r1!, {r4, r5, r6, r7, r8, r9, r10, r11}
	stm.w r0!, {r4, r5, r6, r7, r8, r9, r10, r11}
	ldm.w r1!, {r4, r5, r6, r7, r8, r9, r10, r11}
	stm.w r0!, {r4, r5, r6, r7, r8, r9, r10, r11}
	ldm.w r1!, {r4, r5, r6, r7, r8, r9, r10, r11}
	stm.w r0!, {r4, r5, r6, r7, r8, r9, r10, r11}
	ldm.w r1!, {r4, r5, r6, r7, r8, r9, r10, r11}
	stm.w r0!, {r4, r5, r6, r7, r8, r9, r10, r11}
	ldm.w r1!, {r4, r5, r6, r7, r8, r9, r10, r11}
	stm.w r0!, {r4, r5, r6, r7, r8, r9, r10, r11}
	ldm.w r1!, {r4, r5, r6, r7, r8, r9, r10, r11}
	stm.w r0!, {r4, r5, r6, r7, r8, r9, r10, r11}
	ldm.w r1!, {r4, r5, r6, r7, r8, r9, r10, r11}
	stm.w r0!, {r4, r5, r6, r7, r8, r9, r10, r11}
	ldm.w r1!, {r4, r5, r6, r7, r8, r9, r10, r11}
	stm.w r0!, {r4, r5, r6, r7, r8, r9, r10, r11}
	pop.w {r4, r5, r6, r7, r8, r9, r10, r11}
	bx    lr

	.data
