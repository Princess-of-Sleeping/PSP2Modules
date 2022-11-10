
	.balign 4
	.text
	.cpu cortex-a9
	.arch armv7-a
	.syntax unified
	.thumb
	.thumb_func
	.fpu neon
	.arch_extension sec

	.globl intr_init_stack_magic
	.type  intr_init_stack_magic, %function

intr_init_stack_magic:
	movw  r1, #:lower16:nest_store_area
	movt  r1, #:upper16:nest_store_area
	mov.w r2, #0x80000
	add.w r3, r1, r2

LAB_81000dec:
	str   r0, [r1]
	add.w r1, r1, #0x2000
	cmp   r3, r1
	bne   LAB_81000dec
	bx    lr

	.globl intr_check_stack_magic
	.type  intr_check_stack_magic, %function

intr_check_stack_magic:
	mrc        p15, #0, r3, c0, c0, #5
	and        r3, r3, #0xF
	mov.w      r2, #0x80
	mul        r1, r2, r3
	movw       r2, #:lower16:nest
	movt       r2, #:upper16:nest
	add        r2, r1
	ldr        r0, [r2, #0x0]
	movw       r1, #:lower16:nest_store_area_end
	movt       r1, #:upper16:nest_store_area_end
	mov.w      r2, #0x20000
	mul        r2, r3, r2
	sub.w      r1, r1, r2
	mov.w      r3, #0x2000
	mul        r2, r3, r0
	sub.w      r1, r1, r2
	ldr        r1, [r1, #0x0]
	movw       r3, #0xe8df
	movt       r3, #0x1ab4
	cmp        r1, r3
	it         ne
	movne.w    r0, #0xFFFFFFFF
	bx         lr


	.globl SceIntrmgrForKernel_F281330D
	.type  SceIntrmgrForKernel_F281330D, %function
// get_nest_cb
SceIntrmgrForKernel_F281330D:
	cmp        r0, #4
	bcs        LAB_81000e7c
	cmp        r1, #1
	bcc        LAB_81000e7c
	cmp        r1, #0x10
	bhi        LAB_81000e7c
	movw       r2, #:lower16:nest_store_area_end
	movt       r2, #:upper16:nest_store_area_end
	mov.w      r3, #0x20000
	mul        r12, r0, r3
	sub.w      r2, r2, r12
	mov.w      r3, #0x2000
	sub.w      r1, r1, #1
	mul        r12, r3, r1
	sub.w      r2, r2, r12
	mov        r0, r2
	bx         lr

LAB_81000e7c:
	mov.w      r0, #0
	bx         lr

	.data
