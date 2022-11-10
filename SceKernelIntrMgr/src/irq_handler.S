
	.balign 0x10
	.text
	.cpu cortex-a9
	.arch armv7-a
	.syntax unified
	.arm
	.fpu neon

LAB_81000000:
	add        r3, r3, #1
	movw       r2, #:lower16:nest_store_area_end
	movt       r2, #:upper16:nest_store_area_end
	sub        r2, r2, r4, lsl #0x11
	sub        r4, r2, r3, lsl #0xD
	ldrd       r2, r3, [r0, #0x38]
	stmdb      r4!, {r2, r3}
	stmdb      r4!, {sp, lr}
	mov        sp, r4
	stmdb      sp!, {r5, r6, r7, r8, r9, r10, r11, r12}
	vpush      {d16, d17, d18, d19, d20, d21, d22, d23, d24, d25, d26, d27, d28, d29, d30, d31}
	vpush      {d0, d1, d2, d3, d4, d5, d6, d7, d8, d9, d10, d11, d12, d13, d14, d15}
	vmrs       r5, fpexc
	vmrs       r6, fpscr
	stmdb      sp!, {r5, r6}
	ldr        r2, [r0, #0x10]
	b          irq_try_save_cp14

LAB_81000044:
	ldmia      sp!, {r0, r1}
	vmsr       fpexc, r0
	vmsr       fpscr, r1
	vpop       {d0, d1, d2, d3, d4, d5, d6, d7, d8, d9, d10, d11, d12, d13, d14, d15}
	vpop       {d16, d17, d18, d19, d20, d21, d22, d23, d24, d25, d26, d27, d28, d29, d30, d31}

	mrc        p15, #0, r12, c12, c0, #0
	ldr        r12, [r12, #0x24]
	mov        lr, #1
	str        lr, [r12, #0x100]
	ldmia      sp!, {r5, r6, r7, r8, r9, r10, r11, r12}
	ldmia      sp!, {r1}
	ldmia      sp!, {lr}
	mov        sp, r1
	cps        #18
	ldmia      sp, {r0, r1, r2, r3}
	mcr        p15, #0, r1, c3, c0, #0
	mcr        p15, #0, r3, c7, c4, #0
	mov        r1, #0
	mcr        p15, #0, r1, c13, c0, #1
	isb        sy
	mcr        p15, #0, r0, c2, c0, #1
	isb        sy
	mcr        p15, #0, r2, c13, c0, #1
	add        sp, sp, #0x20
	ldmia      sp!, {r0, r1, r2, r3, r4, r12}
	clrex
	rfeia      sp!

ignore_intr_handle:
	pop        {r0, r1, r2, r3, r4, ip}
	add        sp, sp, #8
	movs       pc, lr
	nop

	.globl irqEnterCallbackStart

irqEnterCallbackStart:
	movw       r3, #:lower16:DAT_810882e4
	movt       r3, #:upper16:DAT_810882e4
	ldr        r3, [r3, #0x0]
	cmp        r3, #0x0
	beq        irqEnterResume

	stmdb      sp!, {r0, r1, r4, r12}
	ldrh       r4, [r1, #0xC]
	ldr        r2, [r1, #0x8]
	lsrs       r4, r4, #4
	ldr        r1, [r1, #0x0]
	stmdb      sp!, {r0, r1, r2, r4}
	ldr        r0, [sp, #0x38]
	stmdb      sp!, {r0, r1}
	mov        r0, sp
	blx        r3
	add        sp, sp, #0x18
	ldmia      sp!, {r0, r1, r4, r12}
	b          irqEnterResume

	.globl irqLeaveCallbackStart

irqLeaveCallbackStart:
	movw       r3, #:lower16:DAT_810882c4
	movt       r3, #:upper16:DAT_810882c4
	ldr        r3, [r3,#0x0]
	cmp        r3, #0
	beq        irqLeaveResume

	stmdb      sp!, {r0, r12}
	mov        r0, sp
	blx        r3
	ldmia      sp!, {r0, r12}
	b          irqLeaveResume
	nop
	nop

	.globl sceKernelIrqHandler
	.type  sceKernelIrqHandler, %function

sceKernelIrqHandler:
	.word 0
	.word 0
	sub        lr, lr, #4
	sub        sp, sp, #4
	stmdb      sp!, {r0, r1, r2, r3, r4, r12, lr}

	movw       r0, #:lower16:SceInterruptControllerReg_0x100
	movt       r0, #:upper16:SceInterruptControllerReg_0x100
	ldr        r0, [r0, #0x0]
	ldr        r12, [r0, #0xC]
	ubfx       r0, r12, #0x0, #0xA
	cmp        r0, #0xFF
	bhi        ignore_intr_handle // invalid intr number

	movw       r1, #:lower16:entries
	movt       r1, #:upper16:entries
	ldr        r2, [r1, r0, lsl #0x6]!
	cmp        r2, #0
	beq        ignore_intr_handle // no intr handler

	.globl irqEnterCallbackBranch
irqEnterCallbackBranch:
	nop

irqEnterResume:
	sub        sp, sp, #0x8
	mrc        p15, #0, lr, c13, c0, #4 // TPIDRPRW
	mrc        p15, #0, r0, c9, c12, #1 // PMCNTENSET
	mcr        p15, #0, r0, c9, c12, #2
	stmdb      sp!, {r0}
	mrc        p15, #0, r4, c7, c4, #0  // PAR
	mrc        p15, #0, r3, c13, c0, #1 // CONTEXTIDR
	mrc        p15, #0, r2, c3, c0, #0  // DACR
	mrc        p15, #0, r0, c2, c0, #1  // TTBR1
	stmdb      sp!, {r0, r2, r3, r4, lr}

	mov        r0, #0

	movw       lr, #:lower16:DAT_81008240
	movt       lr, #:upper16:DAT_81008240
	ldr        lr, [lr]
	cmp        lr, #0
	bne        clear_DBGBCR

LAB_810001c0:
	movw       lr, #:lower16:DAT_81008244
	movt       lr, #:upper16:DAT_81008244
	ldr        lr, [lr, #0]
	cmp        lr, #0
	bne        clear_DBGWVR

LAB_810001d4:
	mcr        p15, #0, r0, c13, c0, #4 // TPIDRPRW
	movw       r0, #:lower16:DAT_81008200
	movt       r0, #:upper16:DAT_81008200
	ldr        r0, [r0]
	ldr        r2, [r0]
	ldr        r3, [r0, #0x8]
	bfi        r3, r12, #0x10, #0xA
	mov        r4, #0
	mcr        p15, #0, r4, c13, c0, #1 // CONTEXTIDR
	isb        sy
	mcr        p15, #0, r2, c2, c0, #1 // TTBR1
	isb        sy
	mcr        p15, #0, r3, c13, c0, #1 // CONTEXTIDR
	movw       lr, #:lower16:0x15450000
	movt       lr, #:upper16:0x15450000
	mcr        p15, #0, lr, c3, c0, #0 // DACR
	mrs        lr, spsr
	str        lr, [sp, #0x3C]
	mov        r0, sp

	// Enter System mode
	cps        #0x1F
	mrc        p15, #0, r4, c0, c0, #5 // MPIDR
	and        r4, r4, #0xF
	add        r2, r1, r4, lsl #2
	ldr        r3, [r2, #0x20]
	add        r3, r3, #1
	str        r3, [r2, #0x20]

	/*
	 * core_nest++ and check nestCount
	 */
	movw       r2, #:lower16:nest
	movt       r2, #:upper16:nest
	ldr        r3, [r2, r4, lsl #0x7]! // load core_nest
	add        r3, r3, #1              // core_nest++
	str        r3, [r2]                // save core_nest
	sub        r3, r3, #2
	ldr        r2, [r0, #0x10]
	ands       r2, r2, r3, asr #0x1F
	beq        LAB_81000000    // Happened interrupt of depth > 1

	ldr        r2, [r0, #0x10] // load TPIDRPRW
	cmp        r2, #0          // TPIDRPRW == 0
	beq        LAB_81000000    // Happened interrupt within intr_handler

	ldr        r3, [r2, #0xC]
	str        lr, [r3, #0x3C]
	str        sp, [r3, #0x38]!
	ldr        lr, [r0, #0x34]
	stmdb      r3!, {r5, r6, r7, r8, r9, r10, r11, lr}
	mov        r4, #1
	add        lr, r0, #0x20
	ldmia      lr, {r5, r6, r7, r8, r9}
	stmdb      r3!, {r4, r5, r6, r7, r8, r9}
	add        r3, r3, #0x4C

	ldr        r4, [r2, #0x10]
	cmp        r4, #0
	beq        LAB_810002b4

	vstmia     r4!, {d0, d1, d2, d3, d4, d5, d6, d7, d8, d9, d10, d11, d12, d13, d14, d15}
	vstmia     r4!, {d16, d17, d18, d19, d20, d21, d22, d23, d24, d25, d26, d27, d28, d29, d30, d31}
	vmrs       r5, fpscr
	vmrs       r6, fpexc
	stmia      r4!, {r5, r6}

LAB_810002b4:
	ldr        r4, [r0, #0x3C]
	and        r4, r4, #0x1F
	cmp        r4, #0x1F
	bne        LAB_810002cc
	ldr        r4, [r0, #4]
	str        r4, [r3, #-0x4]

LAB_810002cc:
	/*
	 * Save PMU/some regs
	 */
	mrc        p15, #0, r4, c1, c0, #2 // CPACR
	mrc        p15, #0, r5, c13, c0, #2 // TPIDRURW
	mrc        p15, #0, r6, c13, c0, #3 // TPIDRURO
	ldr        r7, [r0]
	ldr        r8, [r0, #8]
	ldr        r9, [r0, #0xc]
	mrc        p15, #0, r10, c9, c12, #0 // PMCR
	mrc        p15, #0, r11, c9, c12, #3
	stmia      r3!, {r4, r5, r6, r7, r8, r9, r10, r11}

	mrc        p15, #0, r4, c9, c12, #5 // PMSELR
	str        r4, [r3], #0x4
	add        r3, r3, #0x4
	ldr        r4, [r0, #0x14]
	str        r4, [r3, #0x7C]
	mrc        p15, #0, r5, c9, c13, #0 // PMCCNTR
	str        r5, [r3], #0x4

	mov        r10, #0
	mcr        p15, #0, r10, c9, c12, #5 // PMSELR
	mrc        p15, #0, r6, c9, c13, #1  // PMXEVTYPER
	mrc        p15, #0, r7, c9, c13, #2  // PMXEVCNTR
	strd       r6, r7, [r3], #0x8

	mov        r10, #1
	mcr        p15, #0, r10, c9, c12, #5
	mrc        p15, #0, r6, c9, c13, #1
	mrc        p15, #0, r7, c9, c13, #2
	strd       r6, r7, [r3], #0x8

	mov        r10, #2
	mcr        p15, #0, r10, c9, c12, #5
	mrc        p15, #0, r6, c9, c13, #1
	mrc        p15, #0, r7, c9, c13, #2
	strd       r6, r7, [r3], #0x8

	mov        r10, #3
	mcr        p15, #0, r10, c9, c12, #5
	mrc        p15, #0, r6, c9, c13, #1
	mrc        p15, #0, r7, c9, c13, #2
	strd       r6, r7, [r3], #0x8

	mov        r10, #4
	mcr        p15, #0, r10, c9, c12, #5
	mrc        p15, #0, r6, c9, c13, #1
	mrc        p15, #0, r7, c9, c13, #2
	strd       r6, r7, [r3], #0x8

	mov        r10, #5
	mcr        p15, #0, r10, c9, c12, #5
	mrc        p15, #0, r6, c9, c13, #1
	mrc        p15, #0, r7, c9, c13, #2
	strd       r6, r7, [r3], #0x8

	mrc        p15, #0, r4, c5, c0, #0 // DFSR
	mrc        p15, #0, r5, c5, c0, #1 // IFSR
	mrc        p15, #0, r6, c6, c0, #0 // DFAR
	mrc        p15, #0, r7, c6, c0, #2 // IFAR
	mrc        p14, #6, r8, c1, c0, #0 // SCTLR
	movw       lr, #:lower16:SceInterruptControllerReg_0x100
	movt       lr, #:upper16:SceInterruptControllerReg_0x100
	ldr        lr, [lr,#0x0]
	ldr        r9, [lr,#0x4]
	ldrd       r10, r11, [r0, #0x38]
	stmia      r3!, {r4, r5, r6, r7, r8, r9, r10, r11}

	mrc        p15, #0, r5, c0, c0, #5 // MPIDR
	and        r5, r5, #0xF
	movw       sp, #:lower16:(nest_store_area_end - 0x138)
	movt       sp, #:upper16:(nest_store_area_end - 0x138)
	sub        sp, sp, r5, lsl #0x11
	mov        r6, #0xF00000
	mcr        p15, #0, r6, c1, c0, #2 // CPACR
	mov        r6, #0x40000000
	vmsr       fpexc, r6
	mov        r6, #0
	vmsr       fpscr, r6
	mrc        p15, #0, r5, c12, c0, #0 // VBAR
	ldr        r5, [r5, #0x24]
	mov        r6, #0x87
	str        r6, [r5, #0x0]
	ldr        r0, [r2, #0x40]
	cmp        r0, #0x2000
	beq        LAB_810008d8

irq_try_save_cp14:
	/*
	 * Check cp14 save flag
	 */
	stmdb      sp!, {r2, r12}
	mov        r3, #0x3000000
	vmsr       fpscr, r3
	movw       r0, #:lower16:DAT_81008240
	movt       r0, #:upper16:DAT_81008240
	ldr        r0, [r0]
	cmp        r0, #0
	beq        irq_invake_handler

	/*
	 * Save cp14
	 */
	stmdb      sp!, {r1, lr}
	bl         ksceKernelCpuLockStoreLR
	add        r2, r0, #0x18
	ldmia      r2!, {r3, r4}
	mcr        p14, #0, r3, c0, c2, #4 // DBGBVR2
	mcr        p14, #0, r4, c0, c2, #5 // DBGBCR2
	ldmia      r2!, {r3, r4}
	mcr        p14, #0, r3, c0, c3, #4 // DBGBVR3
	mcr        p14, #0, r4, c0, c3, #5 // DBGBCR3
	ldmia      r2!, {r3, r4}
	mcr        p14, #0, r3, c0, c4, #4 // DBGBVR4
	mcr        p14, #0, r4, c0, c4, #5 // DBGBCR4
	isb        sy
	bl         ksceKernelCpuUnlockStoreLR
	ldmia      sp!, {r1, lr}

	/*
	 * Check cp14 save flag (second)
	 */
	movw       r0, #:lower16:DAT_81008244
	movt       r0, #:upper16:DAT_81008244
	ldr        r0, [r0]
	cmp        r0, #0x0
	beq        irq_invake_handler

	/*
	 * Save cp14 (second)
	 */
	stmdb      sp!, {r1, lr}
	bl         ksceKernelCpuLockStoreLR
	add        r2, r0, #0x10
	ldmia      r2!, {r3, r4}
	mcr        p14, #0, r3, c0, c1, #6 // DBGWVR1
	mcr        p14, #0, r4, c0, c1, #7 // DBGWVR1
	ldmia      r2!, {r3, r4}
	mcr        p14, #0,r3, c0, c2, #6 // DBGWVR2
	mcr        p14, #0,r4, c0, c2, #7 // DBGWVR2
	ldmia      r2!, {r3, r4}
	mcr        p14, #0, r3, c0, c3, #6 // DBGWVR3
	mcr        p14, #0, r4, c0, c3, #7 // DBGWVR3
	isb        sy
	bl         ksceKernelCpuUnlockStoreLR
	ldmia      sp!, {r1, lr}

irq_invake_handler:
	mov        r0, #0
	mcr        p15, #0, r0, c13, c0, #3
	cpsie      i
	ldr        r12, [r1]
	ldrh       r0, [r1, #4]
	ldrh       r2, [r1, #0xc]
	ldr        r1, [r1, #8]
	blx        r12
	cpsid      i
	ldmia      sp!, {r4, r12}

	.globl irqLeaveCallbackBranch

irqLeaveCallbackBranch:
	nop

irqLeaveResume:
	cmn        r0, #1
	beq        LAB_8100050c
	bic        r0, r12, #0x1c00
	cmp        r0, #0x1F
	bls        LAB_8100050c
	bic        r1, r0, #0x3e0
	mov        r2, #1
	lsl        r2, r2, r1
	lsr        r1, r0, #5
	lsl        r1, r1, #2
	movw       lr, #:lower16:SceInterruptControllerReg_0x1000
	movt       lr, #:upper16:SceInterruptControllerReg_0x1000
	ldr        lr, [lr]
	add        r3, lr, #0x180
	str        r2, [r3, r1]

LAB_8100050c:
	movw       r11, #:lower16:SceInterruptControllerReg_0x100
	movt       r11, #:upper16:SceInterruptControllerReg_0x100
	ldr        r11, [r11]
	str        r12, [r11, #0x10]

	/*
	 * Check stack overflow
	 */
	mrc        p15, #0, r3, c0, c0, #5 // MPIDR
	and        r3, r3, #0xF
	movw       r2, #:lower16:nest
	movt       r2, #:upper16:nest
	add        r2, r2, r3, lsl #0x7
	ldr        r0, [r2]
	movw       r1, #:lower16:nest_store_area_end
	movt       r1, #:upper16:nest_store_area_end
	sub        r1, r1, r3, lsl #0x11
	sub        r1, r1, r0, lsl #0xD
	ldr        r1, [r1]
	movw       r3, #0xe8df
	movt       r3, #0x1ab4
	cmp        r1, r3
	bne        detect_stack_overflow

	subs       r0, r0, #1
	str        r0, [r2]
	bne        LAB_81000044 // r0 != 0 -> jump
	cmp        r4, #0
	beq        LAB_81000044
	movw       r1, #:lower16:DAT_81088284
	movt       r1, #:upper16:DAT_81088284
	ldr        r0, [r1]
	blx        r0
	cmp        r0, #0
	mcr        p15, #0, r4, c13, c0, #4
	ldr        r0, [r4, #0xC]
	beq        LAB_810005a0

	mov        r1, #1
	movw       r2, #:lower16:DAT_81088288
	movt       r2, #:upper16:DAT_81088288
	ldr        r3, [r2]
	blx        r3

LAB_810005a0:
	add        r1, r0, #0xC0
	mrc        p15, #0, r12, c13, c0, #4
	ldmdb      r1!, {r4, r5, r6, r7, r8, r9}
	str        r9, [r11, #0x4]
	mcr        p14, #6, r8, c1, c0, #0 // TEEHBR
	mcr        p15, #0, r7, c6, c0, #2 // IFAR
	mcr        p15, #0, r6, c6, c0, #0 // DFAR
	mcr        p15, #0, r5, c5, c0, #1 // IFSR
	mcr        p15, #0, r4, c5, c0, #0 // DFSR
	ldr        r11, [r0, #0xf0]
	mcr        p15, #0, r11, c9, c12, #2

	mov        r9, #5
	ldrd       r4, r5, [r1, #-0x8]!
	and        r4, r4, #0xFF
	mcr        p15, #0, r9, c9, c12, #5
	mcr        p15, #0, r4, c9, c13, #1
	mcr        p15, #0, r5, c9, c13, #2

	mov        r9, #4
	ldrd       r4, r5, [r1, #-0x8]!
	and        r4, r4, #0xFF
	mcr        p15, #0, r9, c9, c12, #5
	mcr        p15, #0, r4, c9, c13, #1
	mcr        p15, #0, r5, c9, c13, #2

	mov        r9, #3
	ldrd       r4, r5, [r1, #-0x8]!
	and        r4, r4, #0xFF
	mcr        p15, #0, r9, c9, c12, #5
	mcr        p15, #0, r4, c9, c13, #1
	mcr        p15, #0, r5, c9, c13, #2

	mov        r9, #2
	ldrd       r4, r5, [r1, #-0x8]!
	and        r4, r4, #0xFF
	mcr        p15, #0, r9, c9, c12, #5
	mcr        p15, #0, r4, c9, c13, #1
	mcr        p15, #0, r5, c9, c13, #2

	mov        r9, #1
	ldrd       r4, r5, [r1, #-0x8]!
	and        r4, r4, #0xFF
	mcr        p15, #0, r9, c9, c12, #5
	mcr        p15, #0, r4, c9, c13, #1
	mcr        p15, #0, r5, c9, c13, #2

	mov        r9, #0
	ldrd       r4, r5, [r1, #-0x8]!
	and        r4, r4, #0xFF
	mcr        p15, #0, r9, c9, c12, #5
	mcr        p15, #0, r4, c9, c13, #1
	mcr        p15, #0, r5, c9, c13, #2

	ldr        r8, [r1, #-0x4]!
	mcr        p15, #0, r8, c9, c13, #0
	ldmdb      r1!, {r2, r3, r4, r5, r6, r7, r8, r9, r10}
	mcr        p15, #0, r10, c9, c14, #0
	mcr        p15, #0, r9, c9, c12, #5
	mcr        p15, #0, r8, c9, c12, #3
	mcr        p15, #0, r7, c9, c12, #0
	mcr        p15, #0, r6, c7, c4, #0  // PAR
	mcr        p15, #0, r3, c13, c0, #3 // TPIDRURO
	mcr        p15, #0, r2, c13, c0, #2 // TPIDRURW
	mov        r2, #0
	mcr        p15, #0, r2, c13, c0, #1 // CONTEXTIDR
	isb        sy
	mcr        p15, #0, r4, c2, c0, #1 // TTBR1
	isb        sy
	mcr        p15, #0, r5, c13, c0, #1 // CONTEXTIDR
	ldr        r2, [r1, #-0x4]!
	mcr        p15, #0, r2, c1, c0, #2 // CPACR
	sub        r1, r1, #0x8
	mrc        p15, #0, r4, c13, c0, #4 // TPIDRPRW
	ldr        r5, [r4, #0x10]
	cmp        r5, #0
	beq        LAB_810006ec

	mov        r6, #0x40000000
	vmsr       fpexc, r6
	mov        r6, #0
	vmsr       fpscr, r6
	vmsr       fpexc, r6

	mrc        p15, #0, r6, c12, c0, #0 // VBAR
	ldr        r6, [r6, #0x24]
	ldr        r7, [r4, #0x14]
	str        r7, [r6, #0x0]
	ldr        r6, [r5, #0x104]
	vmsr       fpexc, r6
	ldr        r6, [r5, #0x100]
	vmsr       fpscr, r6

LAB_810006ec:
	ldr        r2, [r0, #0x0]
	cmp        r2, #1
	mov        r10, r2
	beq        LAB_81000758
	cmp        r5, #0
	beq        LAB_81000748

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
	add        r5, r5, #0x40
	vldmia     r5!, {d8, d9, d10, d11, d12, d13, d14, d15}

LAB_81000748:
	ldr        r3, [r1, #-0x4]!
	ldr        r2, [r1, #-0x4]!
	ldr        sp, [r1, #-0x4]!
	b          LAB_81000778

LAB_81000758:
	cmp        r5, #0
	beq        LAB_81000768
	vldmia     r5!, {d0, d1, d2, d3, d4, d5, d6, d7, d8, d9, d10, d11, d12, d13, d14, d15}
	vldmia     r5!, {d16, d17, d18, d19, d20, d21, d22, d23, d24, d25, d26, d27, d28, d29, d30, d31}

LAB_81000768:
	sub        r1, r1, #0x4
	ldr        lr, [r1, #-0x4]!
	ldr        sp, [r1, #-0x4]!
	ldrd       r2, r3, [r0, #0xC0]

LAB_81000778:
	// Return to Irq mode
	cps        #0x12
	and        r5, r3, #0x1F
	cmp        r5, #0x10
	ldr        r5, [r0, #0x48]
	bne        LAB_810007a0
	ldr        r5, [r0, #0x44]
	movw       r7, #0xaa8e
	movt       r7, #0xeaba
	tst        r5, r7
	bne        illegal_user_DACR

LAB_810007a0:
	mcr        p15, #0, r5, c3, c0, #0
	ldr        r5, [r4, #0x18] // r4 value is TPIDRPRW
	ldr        r6, [r5, #0xC]  // r5 value is SceUIDProcessObject pointer
	cmp        r6, #0
	beq        LAB_81000850

	/*
	 * Restore cp14
	 */
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
	beq        LAB_81000850

	/*
	 * Restore cp14 (second)
	 */
	stmdb      sp!, {r0, r1, r2, r3, r12, lr}
	mov        r0, r6
	bl         ksceKernelCpuLockStoreLR
	mov        r0, r6
	add        r6, r6, #0x10
	ldmia      r6!, {r7, r8}
	mcr        p14, #0, r7, c0, c1, #6 // DBGWVR1
	mcr        p14, #0, r8, c0, c1, #7 // DBGWVR1
	ldmia      r6!, {r7, r8}
	mcr        p14, #0, r7, c0, c2, #6
	mcr        p14, #0, r8, c0, c2, #7
	ldmia      r6!, {r7, r8}
	mcr        p14, #0, r7, c0, c3, #6
	mcr        p14, #0, r8, c0, c3, #7
	bl         ksceKernelCpuUnlockStoreLR
	ldmia      sp!, {r0, r1, r2, r3, r12, lr}
	ldr        r6, [r4, #0x1C]
	mcr        p14, #0, r6, c0, c5, #4
	ldr        r6, [r4, #0x20]
	mcr        p14, #0, r6, c0, c5, #5

LAB_81000850:
	add        sp, sp, #0x40
	mcr        p15, #0, r11, c9, c12, #1
	stmdb      sp!, {r2, r3}
	cmp        r10, #1
	beq        LAB_81000888

	sub        r1, r1, #4
	mrc        p15, #0, r4, c12, c0, #0
	ldr        r4, [r4, #0x24]
	mov        r5, #1
	str        r5, [r4, #0x100]
	ldmdb      r1!, {r4, r5, r6, r7, r8, r9, r10, r11}
	mov        r0, #1
	clrex
	rfeia      sp!

LAB_81000888:
	ldmdb      r1!, {r0}
	stmdb      sp!, {r0}
	mrc        p15, #0, r4, c12, c0, #0
	ldr        r4, [r4, #0x24]
	mov        r5, #1
	str        r5, [r4, #0x100]
	mov        r12, r1
	ldmdb      ip!, {r0, r1, r2, r3, r4, r5, r6, r7, r8, sb, sl, fp}
	ldmia      sp!, {r12}
	clrex
	rfeia      sp!

clear_DBGBCR:
	mcr        p14, #0, r0, c0, c2, #5 // DBGBCR2
	mcr        p14, #0, r0, c0, c3, #5 // DBGBCR3
	mcr        p14, #0, r0, c0, c4, #5 // DBGBCR4
	mcr        p14, #0, r0, c0, c5, #5 // DBGBCR5
	b          LAB_810001c0

clear_DBGWVR:
	mcr        p14, #0, r0, c0, c1, #7 // DBGWVR1
	mcr        p14, #0, r0, c0, c2, #7 // DBGWVR2
	mcr        p14, #0, r0, c0, c3, #7 // DBGWVR3
	b          LAB_810001d4

LAB_810008d8:
	stmdb      sp!, {r0, r1, r2, r12}
	mov        r0, r12
	mov        r1, r2
	movw       r2, #:lower16:DAT_81088280
	movt       r2, #:upper16:DAT_81088280
	ldr        r2, [r2]
	blx        r2
	ldmia      sp!, {r0, r1, r2, r12}
	b          irq_try_save_cp14

detect_stack_overflow:
	mov        r1, r12
	blx        detectIntrStackOverflowCfunc

LAB_81000904:
	b          LAB_81000904

illegal_user_DACR:
	mov        r0, r4
	mov        r1, r5
	blx        illegalUserDACRCfunc

	.data

	.bss

	.global nest_store_area
	.align	7
	.type	nest_store_area, %object
	.size	nest_store_area, 0x2000 * 0x10 * 4
nest_store_area:
	.space	0x2000 * 0x10 * 4

	.global nest_store_area_end
nest_store_area_end:
