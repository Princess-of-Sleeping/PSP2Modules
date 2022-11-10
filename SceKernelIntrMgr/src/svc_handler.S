
	.balign 4
	.text
	.cpu cortex-a9
	.arch armv7-a
	.syntax unified
	.thumb
	.thumb_func
	.fpu neon
	.syntax unified
	.arch_extension sec

	.global AllocSharedMemory_S
	.type  AllocSharedMemory_S, %function
AllocSharedMemory_S:
	movw       r12, #0x101
	smc        #0x0
	bx         lr
	nop
	nop.w
	nop.w
	nop.w
	nop.w
	nop.w
	nop.w
	nop.w
	nop.w
	nop.w
	nop.w
	nop.w
	nop.w

	.balign 0x20

	.global svcEnterCallbackStart
	.global svcLeaveCallbackBranch

svcEnterCallbackStart:
	movw       r2, #:lower16:DAT_810882c8
	movt       r2, #:upper16:DAT_810882c8
	ldr        r2, [r2]
	cbz        r2, LAB_81000f14
	mov        r0, sp
	blx        r2
	ldr.w      ip, [sp, #0x38]

LAB_81000f14:
	b          svc_invake_handler

	.global svcLeaveCallbackStart
svcLeaveCallbackStart:
	movw       r2, #:lower16:DAT_810882cc
	movt       r2, #:upper16:DAT_810882cc
	ldr        r2, [r2, #0x0]
	cbz        r2, LAB_81000f2c
	add        r0, sp, #0x10
	mov        r1, sp
	blx        r2
	mrc        p15, #0, r3, c13, c0, #4

LAB_81000f2c:
	b          svcLeaveResume

LAB_81000f2e:
	bkpt       #0
	nop.w
	nop.w

	.global sceKernelSvcHandler
	.type  sceKernelSvcHandler, %function

sceKernelSvcHandler:
	.word 0
	.word 0
	stmdb      sp, {r0, r1, r2, r3}
	mrs        r2, spsr
	strd       lr, r2, [sp, #-0x18]
	and        r2, r2, #0x1F
	subs       r2, #0x10
	bne        LAB_81000f2e

	mov        r0, sp
	cps        #0x1F
	mrc        p15, #0, r1, c13, c0, #4 // TPIDRPRW
	mrc        p15, #0, r3, c3, c0, #0  // DACR
	str.w      r3, [r0, #-0x1C]
	movt       r2, #0x1545
	ldr        r3, [r1, #0xC]
	str        r2, [r3, #0x48]
	mcr        p15, #0, r2, c3, c0, #0  // DACR
	movs       r2, #1
	mcr        p15, #0, r2, c13, c0, #3 // TPIDRURO
	ldr        r2, [r1, #0x28]
	str        r2, [r1, #0x2C]
	mov        r2, sp
	ldr.w      sp, [r1, #0x30]
	ldr        r3, [r1, #0xC]
	ldr.w      r3, [r3, #0xFC]
	sub        sp, #0x60
	str        r3, [sp, #0x34]
	str.w      sp, [r1, #0x44]
	ldrd       r1, r3, [r0, #-0x18]
	strd       r1, r3, [sp, #0x58]
	strd       r2, lr, [sp, #0x50]
	movs       r3, #0
	mrc        p15, #0, r2, c13, c0, #2 // TPIDRURW
	strd       r3, r2, [sp, #0x40]
	mov        lr, r0
	ldmdb      lr, {r0, r1, r2, r3}
	stm.w      sp, {r0, r1, r2, r3, r4, r5, r6, r7, r8, sb, sl, fp, ip}

	cmp.w      r12, #0x1000
	bcs.w      svcInvalidNumber // syscall_number >= 0x1000

	movw       r0, #:lower16:DAT_810882d4
	movt       r0, #:upper16:DAT_810882d4
	ldr        r0, [r0, #0x0]
	ldr.w      r12, [r0, r12, lsl #2]
	vmrs       r1, fpscr
	strd       r12, r1, [sp, #0x38]
	ldr.w      r1, [lr, #-0x1C]
	movw       r0, #0xaa8e
	movt       r0, #0xeaba
	tst        r1, r0
	bne.w      svcIllegalUserDACR

	mov.w      r1, #0x3000000
	vmsr       fpscr, r1

	.global svcEnterCallbackBranch

svcEnterCallbackBranch:
	nop

svc_invake_handler:
	ldm.w      sp, {r0, r1, r2, r3}
	cpsie      i
	blx        r12
	cpsid      i
	push       {r0, r1, r2, r3}

	// syscall enter check
	mrc        p15, #0, r2, c13, c0, #3 // TPIDRURO
	cmp        r2, #1
	bne        svcInvalidApiStat_trampoline

LAB_81000ffe:
	// fastmutex unlock check
	mrc        p15, #0, r3, c13, c0, #4 // TPIDRPRW
	add.w      r2, r3, #0x68
	ldr.w      r12, [r2, #0x0]
	cmp        r2, r12
	bne        LAB_810010fc

LAB_8100100e:
	ldrd       r2, r12, [r3, #0x24]
	str.w      r12, [r3, #0x2C]
	ands       r2, r2, #0xF0000000
	bne.w      LAB_81001120

	ldr.w      r12, [r3, #0x70]
	cmp.w      r12, #0
	bne        LAB_81001112

	ldr        r2, [sp, #0x50]
	subs       r2, #1
	bhi        LAB_81001118
	bne        LAB_81001034
	movs       r2, r1
	bpl        svcLeave

LAB_81001034:
	cmp.w      r2, r0, asr #0x1E
	beq        svcCriticalError
	movw       lr, #:lower16:0x8002710C
	movt       lr, #:upper16:0x8002710C
	cmp        r0, lr
	beq        svcNotRegistered

svcLeaveCallbackBranch:
svcLeave:
	nop

svcLeaveResume:
	ldr        r2, [sp, #0x44]
	ldr.w      r12, [r3, #0xC]
	str.w      r2, [r12, #0xFC]
	mov.w      r2, #0
	str        r2, [r3, #0x44]
	ldr        r2, [r3, #0x34]
	mcr        p15, #0, r2, c13, c0, #3

LAB_8100105e:
	ldrex      lr, [r3, #0x24]
	bic        r12, lr, #0xA000
	strex      r12, r12, [r3, #0x24]
	cmp.w      r12, #0
	bne        LAB_8100105e

	mrc        p15, #0, r2, c3, c0, #0
	ldr        r3, [r3, #0xC]
	str        r2, [r3, #0x48]
	ldr        r2, [r3, #0x44]
	movw       r3, #0x5575
	movt       r3, #0x5555
	ands       r2, r3
	mcr        p15, #0, r2, c3, c0, #0 // DACR
	pop        {r0, r1, r2, r3}
	tst        lr, #0x2000
	bne        svcRestoreRegister // != 0

svcCleanRegister:
	ldr.w      r12, [sp, #0x40]
	cmp.w      r12, #4
	bcs        svcExit // >=
	ands       r12, r12, #1
	movw       r3, #0xbeef
	movt       r3, #0xdead
	it         eq
	moveq      r1, r3
	mov        r2, r3

svcExit:
	mrc        p15, #0, r12, c12, c0, #0
	ldr.w      r12, [r12, #0x24]
	cmp.w      r12, #0
	mov.w      lr, #1
	it         ne
	strne.w    lr, [r12, #0x100]
	ldr.w      r12, [sp, #0x44]
	mcr        p15, #0, r12, c13, c0, #2
	ldr.w      r12, [sp, #0x3C]
	vmsr       fpscr, r12
	add.w      r12, sp, #0x58
	ldr.w      lr, [sp, #0x54]
	ldr.w      sp, [sp, #0x50]
	cps        #19
	ldm.w      r12, {r12, lr}
	push       {r12, lr}
	movw       r12, #:lower16:0xdeadbeef
	movt       r12, #:upper16:0xdeadbeef
	clrex
	rfeia      sp!

svcInvalidApiStat_trampoline:
	b          svcInvalidApiStat

LAB_810010fc:
	bl         invalidFastMutexOwnerCfunc
	ldm.w      sp, {r0, r1}
	mrc        p15, #0, r3, c13, c0, #4
	b          LAB_8100100e

svcRestoreRegister:
	add        r4, sp, #0x10
	ldmdb      r4, {r4, r5, r6, r7, r8, r9, r10, r11}
	b          svcCleanRegister

LAB_81001112:
	mov.w      r2, #0x2000000
	b          LAB_81001120

LAB_81001118:
	adds       r2, #1
	bge        svcLeave

svcCriticalError:
	mov.w      r2, #0x1000000

LAB_81001120:
	add        r0, sp, #0x10
	mov        r1, sp
	blx        sceKernelSysrootTrapThreadAfterSyscall
	mrc        p15, #0, r3, c13, c0, #4
	cmp        r0, #0
	bne        svcLeave
	add        sp, #0x10
	ldr.w      r12, [sp, #0x38]
	b          svc_invake_handler

svcInvalidNumber:
	sub        sp, #0x10

svcNotRegistered:
	mov.w      r0, #0x80000
	mov.w      r1, #0x0
	mov.w      r2, #0x0
	bl         FUN_810014f8
	mrc        p15, #0, r3, c13, c0, #4
	ldr        r2, [r3, #0x24]
	ands       r2, r2, #0xF0000000
	bne        LAB_81001120
	b          LAB_81001112

svcInvalidApiStat:
	add        r0, sp, #0x10
	mov        r1, sp
	bl         invalidApiStatCfunc
	ldm.w      sp, {r0, r1}
	b          LAB_81000ffe

svcIllegalUserDACR:
	mrc        p15, #0, r0, c13, c0, #4
	bl         illegalUserDACRCfunc

	.data
