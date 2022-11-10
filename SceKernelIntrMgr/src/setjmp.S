
	.balign 4
	.text
	.cpu cortex-a9
	.arch armv7-a
	.syntax unified
	.arm
	.fpu neon

	.globl SceIntrmgrForKernel_EE4CE1DB
	.type  SceIntrmgrForKernel_EE4CE1DB, %function

SceIntrmgrForKernel_EE4CE1DB:
	mov        r2, #0
	stmia      r0!, {r2}
	add        r0, r0, #0x10
	stmia      r0!, {r4, r5, r6, r7, r8, r9, r10, r11}
	add        r0, r0, #4
	stmia      r0!, {sp}
	stmia      r0!, {lr}
	mrs        r2, cpsr
	orr        r2, r2, #0x20
	stmia      r0!, {r2}
	add        r0, r0, #4
	mrc        p15, #0, r2, c3, c0, #0
	str        r2, [r0], #4
	cmp        r1, #0
	beq        LAB_81000974

	tst        r1, #1
	bicne      r1, r1, #1
	addeq      r1, r1, #0x40
	vstmia     r1!, {d8, d9, d10, d11, d12, d13, d14, d15}
	addeq      r1, r1, #0x80
	vmrs       r2, fpscr
	vmrs       r3, fpexc
	stmia      r1!, {r2, r3}

LAB_81000974:
	mrc        p15, #0, r1, c1, c0, #2  // CPACR
	mrc        p15, #0, r2, c13, c0, #2 // TPIDRURW
	mrc        p15, #0, r3, c13, c0, #3 // TPIDRURO
	mrc        p15, #0, r12, c2, c0, #1 // TTBR1
	stmia      r0!, {r1, r2, r3, r12}
	mrc        p15, #0, r1, c13, c0, #1 // CONTEXTIDR
	mrc        p15, #0, r2, c7, c4, #0  // PAR
	mrc        p15, #0, r3, c9, c12, #0
	mrc        p15, #0, r12, c9, c12, #3
	stmia      r0!, {r1, r2, r3, r12}
	mrc        p15, #0, r1, c9, c12, #5
	str        r1, [r0], #4
	add        r0, r0, #4
	mrc        p15, #0, r1, c9, c12, #1
	mcr        p15, #0, r1, c9, c12, #2
	str        r1, [r0, #0x7c]
	mrc        p15, #0, r2, c9, c13, #0 // PMCCNTR
	str        r2, [r0], #4

	mov        r12, #0
	mcr        p15, #0, r12, c9, c12, #5
	mrc        p15, #0, r3, c9, c13, #1
	str        r3, [r0], #4
	mrc        p15, #0, r3, c9, c13, #2
	str        r3, [r0], #4

	mov        r12, #1
	mcr        p15, #0, r12, c9, c12, #5
	mrc        p15, #0, r3, c9, c13, #1
	str        r3, [r0], #4
	mrc        p15, #0, r3, c9, c13, #2
	str        r3, [r0], #4

	mov        r12, #2
	mcr        p15, #0, r12, c9, c12, #5
	mrc        p15, #0, r3, c9, c13, #1
	str        r3, [r0], #4
	mrc        p15, #0, r3, c9, c13, #2
	str        r3, [r0], #4

	mov        r12, #3
	mcr        p15, #0, r12, c9, c12, #5
	mrc        p15, #0, r3, c9, c13, #1
	str        r3, [r0], #4
	mrc        p15, #0, r3, c9, c13, #2
	str        r3, [r0], #4

	mov        r12, #4
	mcr        p15, #0, r12, c9, c12, #5
	mrc        p15, #0, r3, c9, c13, #1
	str        r3, [r0], #4
	mrc        p15, #0, r3, c9, c13, #2
	str        r3, [r0], #4

	mov        r12, #5
	mcr        p15, #0, r12, c9, c12, #5
	mrc        p15, #0, r3, c9, c13, #1
	str        r3, [r0], #4
	mrc        p15, #0, r3, c9, c13, #2
	str        r3, [r0], #4

	mrc        p15, #0, r1, c5, c0, #0
	mrc        p15, #0, r2, c5, c0, #1
	mrc        p15, #0, r3, c6, c0, #0
	mrc        p15, #0, r12, c6, c0, #2
	stmia      r0!, {r1, r2, r3, r12}
	mrc        p14, #6, r1, c1, c0, #0
	stmia      r0!, {r1}
	movw       r1, #:lower16:SceInterruptControllerReg_0x100
	movt       r1, #:upper16:SceInterruptControllerReg_0x100
	ldr        r1, [r1]
	ldr        r2, [r1, #4]
	stmia      r0!, {r2}
	mov        r0, #0
	bx         lr



	.globl SceIntrmgrForKernel_8ED485C0
	.type  SceIntrmgrForKernel_8ED485C0, %function

SceIntrmgrForKernel_8ED485C0:
	mov        lr, r2
	mov        r12, r0
	ldr        r3, [r0, #0x0]
	add        r0, r0, #0xc0
	ldmdb      r0!, {r2}
	movw       r1, #:lower16:SceInterruptControllerReg_0x100
	movt       r1, #:upper16:SceInterruptControllerReg_0x100
	ldr        r1, [r1, #0x0]
	str        r2, [r1, #0x4]
	ldmdb      r0!, {r4, r5, r6, r7, r8}
	mcr        p14, #6, r8, c1, c0, #0
	mcr        p15, #0, r7, c6, c0, #2
	mcr        p15, #0, r6, c6, c0, #0
	mcr        p15, #0, r5, c5, c0, #1
	mcr        p15, #0, r4, c5, c0, #0
	ldr        r11, [r12, #0xF0]
	mcr        p15, #0, r11, c9, c12, #2

	mov        r9, #5
	ldrd       r4, r5, [r0, #-0x8]!
	and        r4, r4, #0xFF
	mcr        p15, #0, r9, c9, c12, #5
	mcr        p15, #0, r4, c9, c13, #1
	mcr        p15, #0, r5, c9, c13, #2

	mov        r9, #4
	ldrd       r4, r5, [r0, #-0x8]!
	and        r4, r4, #0xFF
	mcr        p15, #0, r9, c9, c12, #5
	mcr        p15, #0, r4, c9, c13, #1
	mcr        p15, #0, r5, c9, c13, #2

	mov        r9, #3
	ldrd       r4, r5, [r0, #-0x8]!
	and        r4, r4, #0xFF
	mcr        p15, #0, r9, c9, c12, #5
	mcr        p15, #0, r4, c9, c13, #1
	mcr        p15, #0, r5, c9, c13, #2

	mov        r9, #2
	ldrd       r4, r5, [r0, #-0x8]!
	and        r4, r4, #0xFF
	mcr        p15, #0, r9, c9, c12, #5
	mcr        p15, #0, r4, c9, c13, #1
	mcr        p15, #0, r5, c9, c13, #2

	mov        r9, #1
	ldrd       r4, r5, [r0, #-0x8]!
	and        r4, r4, #0xFF
	mcr        p15, #0, r9, c9, c12, #5
	mcr        p15, #0, r4, c9, c13, #1
	mcr        p15, #0, r5, c9, c13, #2

	mov        r9, #0
	ldrd       r4, r5, [r0, #-0x8]!
	and        r4, r4, #0xFF
	mcr        p15, #0, r9, c9, c12, #5
	mcr        p15, #0, r4, c9, c13, #1
	mcr        p15, #0, r5, c9, c13, #2

	ldr        r8, [r0, #-0x4]!
	mcr        p15, #0, r8, c9, c13, #0
	ldmdb      r0!, {r4, r5, r6, r7, r8, r9, r10}
	mcr        p15, #0, r10, c9, c14, #0
	mcr        p15, #0, r9, c9, c12, #5
	mcr        p15, #0, r8, c9, c12, #3
	mcr        p15, #0, r7, c9, c12, #0
	mcr        p15, #0, r6, c7, c4, #0
	mov        r6, #0
	mcr        p15, #0, r6, c13, c0, #1
	isb        sy
	mcr        p15, #0, r4, c2, c0, #1
	isb        sy
	mcr        p15, #0, r5, c13, c0, #1
	ldmdb      r0!, {r4, r5, r6}
	mcr        p15, #0, r6, c13, c0, #3
	mcr        p15, #0, r5, c13, c0, #2
	mcr        p15, #0, r4, c1, c0, #2
	sub        r0, r0, #8
	mrc        p15, #0, r4, c13, c0, #4
	cmp        lr, #0
	beq        LAB_81000bd4

	mov        r6, #0x40000000
	vmsr       fpexc, r6
	mov        r6, #0
	vmsr       fpscr, r6
	vmsr       fpexc, r6
	mrc        p15, #0, r6, c12, c0, #0
	ldr        r6, [r6, #0x24]
	ldr        r7, [r4, #0x14]
	str        r7, [r6, #0x0]

LAB_81000bd4:
	cmp        r3, #1
	mov        r10, r3
	beq        LAB_81000c60
	cmp        lr, #0
	beq        LAB_81000c50
	tst        lr, #1
	movne      lr, lr, lsr #0x1
	movne      lr, lr, lsl #0x1
	ldrne      r6, [lr, #0x44]
	ldreq      r6, [lr, #0x104]
	vmsr       fpexc, r6
	ldrne      r6, [lr, #0x40]
	ldreq      r6, [lr, #0x100]
	vmsr       fpscr, r6
	movw       r2, #0xdead
	movt       r2, #0x7f80
	add        r3, r2, #0x780000
	vmov       d0, r2, r3
	vmov       d1, d0
	vmov       q2, q0
	vmov       q3, q0
	vmov       q8, q0
	vmov       q9, q0
	vmov       q10, q0
	vmov       q11, q0
	vmov       q12, q0
	vmov       q13, q0
	vmov       q14, q0
	vmov       q15, q0
	addeq      lr, lr, #0x40
	vldmia     lr!, {d8, d9, d10, d11, d12, d13, d14, d15}

LAB_81000c50:
	ldr        r3, [r0, #-0x4]!
	ldr        r2, [r0, #-0x4]!
	ldr        sp, [r0, #-0x4]!
	b          LAB_81000c90

LAB_81000c60:
	cmp        lr, #0
	beq        LAB_81000c80
	ldr        r6, [lr, #0x104]
	vmsr       fpexc, r6
	ldr        r6, [lr, #0x100]
	vmsr       fpscr, r6

	vldmia     lr!, {d0, d1, d2, d3, d4, d5, d6, d7, d8, d9, d10, d11, d12, d13, d14, d15}
	vldmia     lr!, {d16, d17, d18, d19, d20, d21, d22, d23, d24, d25, d26, d27, d28, d29, d30, d31}

LAB_81000c80:
	sub        r0, r0, #0x4
	ldr        lr, [r0, #-0x4]!
	ldr        sp, [r0, #-0x4]!
	ldrd       r2, r3, [r12, #0xc0]

LAB_81000c90:
	cps        #18
	and        r5, r3, #0x1F
	cmp        r5, #0x10
	ldr        r5, [r12, #0x48]
	bne        LAB_81000cb8
	ldr        r5, [r12, #0x44]
	movw       r7, #0xaa8e
	movt       r7, #0xeaba
	tst        r5, r7
	bne        LAB_81000dcc

LAB_81000cb8:
	mcr        p15, #0, r5, c3, c0, #0
	ldr        r5, [r4, #0x18]
	ldr        r6, [r5, #0xc]
	cmp        r6, #0x0
	beq        LAB_81000d68
	stmdb      sp!, {r0, r1, r2, r3, r12, lr}
	mov        r0, r6
	bl         ksceKernelCpuLockStoreLR
	mov        r0, r6
	add        r6, r6, #0x18
	ldmia      r6!, {r7, r8}
	mcr        p14, #0, r7, c0, c2, #4
	mcr        p14, #0, r8, c0, c2, #5
	ldmia      r6!, {r7, r8}
	mcr        p14, #0, r7, c0, c3, #4
	mcr        p14, #0, r8, c0, c3, #5
	ldmia      r6!, {r7, r8}
	mcr        p14, #0, r7, c0, c4, #4
	mcr        p14, #0, r8, c0, c4, #5
	bl         ksceKernelCpuUnlockStoreLR
	ldmia      sp!, {r0, r1, r2, r3, r12, lr}
	ldr        r6, [r5, #0x10]
	cmp        r6, #0
	beq        LAB_81000d68
	stmdb      sp!, {r0, r1, r2, r3, r12, lr}
	mov        r0, r6
	bl         ksceKernelCpuLockStoreLR
	mov        r0, r6
	add        r6, r6, #0x10
	ldmia      r6!, {r7, r8}
	mcr        p14, #0, r7, c0, c1, #6
	mcr        p14, #0, r8, c0, c1, #7
	ldmia      r6!, {r7, r8}
	mcr        p14, #0, r7, c0, c2, #6
	mcr        p14, #0, r8, c0, c2, #7
	ldmia      r6!, {r7, r8}
	mcr        p14, #0, r7, c0, c3, #6
	mcr        p14, #0, r8, c0, c3, #7
	bl         ksceKernelCpuUnlockStoreLR
	ldmia      sp!, {r0, r1, r2, r3, r12, lr}
	ldr        r6, [r4, #0x1c]
	mcr        p14, #0, r6, c0, c5, #4
	ldr        r6, [r4, #0x20]
	mcr        p14, #0, r6, c0, c5, #5

LAB_81000d68:
	cmp        r10, #1
	mcr        p15, #0, r11, c9, c12, #1
	beq        LAB_81000d9c
	stmdb      sp!, {r2, r3}
	mrc        p15, #0, r4, c12, c0, #0
	ldr        r4, [r4, #0x24]
	mov        r5, #1
	str        r5, [r4, #0x100]
	sub        r0, r0, #0x4
	ldmdb      r0!, {r4, r5, r6, r7, r8, r9, r10, r11}
	mov        r0, #1
	clrex
	rfeia      sp!

LAB_81000d9c:
	stmdb      sp!, {r2, r3}
	ldmdb      r0!, {r1}
	stmdb      sp!, {r1}
	mrc        p15, #0, r4, c12, c0, #0
	ldr        r4, [r4, #0x24]
	mov        r5, #1
	str        r5, [r4, #0x100]
	mov        r12, r0
	ldmdb      r12!, {r0, r1, r2, r3, r4, r5, r6, r7, r8, r9, r10, r11}
	ldmia      sp!, {r12}
	clrex
	rfeia      sp!

LAB_81000dcc:
	mov        r0, r4
	mov        r1, r5
	blx        illegalUserDACRCfunc


	.data
