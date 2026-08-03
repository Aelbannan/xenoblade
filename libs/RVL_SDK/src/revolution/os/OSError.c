#include <revolution/BASE.h>
#include <revolution/DSP.h>
#include <revolution/OS.h>

#include <stdio.h>

OSErrorHandler __OSErrorTable[OS_ERR_MAX];
u8 oserror_bss_pad[12]; /* retail .bss 0x44 -> 0x50 (align tail); non-static so -ipa file keeps it */
u32 __OSFpscrEnableBits = FPSCR_VE | FPSCR_OE | FPSCR_UE | FPSCR_ZE | FPSCR_XE;

DECL_WEAK void OSReport(const char* msg, ...) {
    va_list list;
    va_start(list, msg);
    vprintf(msg, list);
    va_end(list);
}

DECL_WEAK void OSVReport(const char* msg, va_list arg){
    vprintf(msg,arg);
}

// unused in Xenoblade retail: OSPanic (defined in monolib/src/util/CErrorWii.cpp instead).
// Its 3 message strings are still present in retail .data at 0x0-0x5B; keep them via .init.
void fake_function(...);
__declspec(section ".init") void FORCEACTIVEOSError_keep(void) {
    fake_function(" in \"%s\" on line %d.\n", "\nAddress:      Back Chain    LR Save\n",
                  "0x%08x:   0x%08x    0x%08x\n");
}

OSErrorHandler OSSetErrorHandler(u16 error, OSErrorHandler handler) {
    OSErrorHandler oldHandler;
    BOOL enabled = OSDisableInterrupts();

    oldHandler = __OSErrorTable[error];
    __OSErrorTable[error] = handler;

    if (error == OS_ERR_FP_EXCEPTION) {
        OSThread* thread;
        u32 msr;
        u32 fpscr;

        msr = PPCMfmsr();
        PPCMtmsr(msr | MSR_FP);
        fpscr = PPCMffpscr();

        if (handler != NULL) {
            int i;
            for (thread = OS_THREAD_QUEUE.head; thread != NULL;
                 thread = thread->nextActive) {
                thread->context.srr1 |= 0x900;

                if (!(thread->context.state & 0x1)) {
                    thread->context.state |= 0x1;

                    for (i = 0; i < 32; i++) {
                        *(u64*)&thread->context.fprs[i] = 0xFFFFFFFFFFFFFFFF;
                        *(u64*)&thread->context.psfs[i] = 0xFFFFFFFFFFFFFFFF;
                    }

                    thread->context.fpscr = FPSCR_NI;
                }
                thread->context.fpscr |=
                    __OSFpscrEnableBits &
                    (FPSCR_VE | FPSCR_OE | FPSCR_UE | FPSCR_ZE | FPSCR_XE);
                thread->context.fpscr &=
                    (FPSCR_FEX | FPSCR_VX | FPSCR_FR | FPSCR_FPRF |
                     FPSCR_UNK20 | FPSCR_VE | FPSCR_OE | FPSCR_UE | FPSCR_ZE |
                     FPSCR_XE | FPSCR_NI | FPSCR_RN);
            }

            fpscr |= __OSFpscrEnableBits &
                     (FPSCR_VE | FPSCR_OE | FPSCR_UE | FPSCR_ZE | FPSCR_XE);
            msr |= (MSR_FE0 | MSR_FE1);
        } else {
            for (thread = OS_THREAD_QUEUE.head; thread != NULL;
                 thread = thread->nextActive) {
                thread->context.srr1 &= ~0x900;
                thread->context.fpscr &=
                    ~(FPSCR_VE | FPSCR_OE | FPSCR_UE | FPSCR_ZE | FPSCR_XE);
                thread->context.fpscr &=
                    (FPSCR_FEX | FPSCR_VX | FPSCR_FR | FPSCR_FPRF |
                     FPSCR_UNK20 | FPSCR_VE | FPSCR_OE | FPSCR_UE | FPSCR_ZE |
                     FPSCR_XE | FPSCR_NI | FPSCR_RN);
            }

            fpscr &= ~(FPSCR_VE | FPSCR_OE | FPSCR_UE | FPSCR_ZE | FPSCR_XE);
            msr &= ~(MSR_FE0 | MSR_FE1);
        }

        PPCMtfpscr(fpscr & (FPSCR_FEX | FPSCR_VX | FPSCR_FR | FPSCR_FPRF |
                            FPSCR_UNK20 | FPSCR_VE | FPSCR_OE | FPSCR_UE |
                            FPSCR_ZE | FPSCR_XE | FPSCR_NI | FPSCR_RN));
        PPCMtmsr(msr);
    }

    OSRestoreInterrupts(enabled);
    return oldHandler;
}

void __OSUnhandledException(u8 error, OSContext* ctx, u32 dsisr, u32 dar) {
    s64 tb = OSGetTime();

    if (!(ctx->srr1 & 0x2)) {
        OSReport("Non-recoverable Exception %d", error);
    } else {
        if (error == OS_ERR_PROGRAM && ctx->srr1 & 0x100000 &&
            __OSErrorTable[OS_ERR_FP_EXCEPTION] != NULL) {
            u32 msr;
            u32 fpscr;

            error = OS_ERR_FP_EXCEPTION;

            msr = PPCMfmsr();
            PPCMtmsr(msr | MSR_FP);

            if (OS_CURRENT_FPU_CONTEXT != NULL) {
                OSSaveFPUContext(OS_CURRENT_FPU_CONTEXT);
            }

            fpscr = PPCMffpscr();
            PPCMtfpscr(fpscr & (FPSCR_FEX | FPSCR_VX | FPSCR_FR | FPSCR_FPRF |
                                FPSCR_UNK20 | FPSCR_VE | FPSCR_OE | FPSCR_UE |
                                FPSCR_ZE | FPSCR_XE | FPSCR_NI | FPSCR_RN));
            PPCMtmsr(msr);

            if (OS_CURRENT_FPU_CONTEXT == ctx) {
                OSDisableScheduler();
                __OSErrorTable[error](error, ctx, dsisr, dar);
                ctx->srr1 &= ~0x2000;
                OS_CURRENT_FPU_CONTEXT = NULL;
                ctx->fpscr &= (FPSCR_FEX | FPSCR_VX | FPSCR_FR | FPSCR_FPRF |
                               FPSCR_UNK20 | FPSCR_VE | FPSCR_OE | FPSCR_UE |
                               FPSCR_ZE | FPSCR_XE | FPSCR_NI | FPSCR_RN);
                OSEnableScheduler();
                __OSReschedule();
            } else {
                ctx->srr1 &= ~0x2000;
                OS_CURRENT_FPU_CONTEXT = NULL;
            }

            OSLoadContext(ctx);
        }

        if (__OSErrorTable[error] != NULL) {
            OSDisableScheduler();
            __OSErrorTable[error](error, ctx, dsisr, dar);
            OSEnableScheduler();
            __OSReschedule();
            OSLoadContext(ctx);
        }

        if (error == OS_ERR_DECREMENTER) {
            OSLoadContext(ctx);
        }

        OSReport("Unhandled Exception %d", error);
    }

    OSReport("\n"); /* retail pools \"\\n\" as a 4-byte .sdata slot (2 pad) */
    OSDumpContext(ctx);
    OSReport("\nDSISR = 0x%08x                   DAR  = 0x%08x\n", dsisr, dar);
    OSReport("TB = 0x%016llx\n", tb);
    switch (error) {
    case OS_ERR_DSI:
        OSReport("\nInstruction at 0x%x (read from SRR0) attempted to access "
                 "invalid address 0x%x (read from DAR)\n",
                 ctx->srr0, dar);
        break;
    case OS_ERR_ISI:
        OSReport("\nAttempted to fetch instruction from invalid address 0x%x "
                 "(read from SRR0)\n",
                 ctx->srr0);
        break;
    case OS_ERR_ALIGNMENT:
        OSReport("\nInstruction at 0x%x (read from SRR0) attempted to access "
                 "unaligned address 0x%x (read from DAR)\n",
                 ctx->srr0, dar);
        break;
    case OS_ERR_PROGRAM:
        OSReport("\nProgram exception : Possible illegal instruction/operation "
                 "at or around 0x%x (read from SRR0)\n",
                 ctx->srr0, dar);
        break;
    case OS_ERR_PROTECTION:
        OSReport("\n"); /* retail pools \"\\n\" as a 4-byte .sdata slot (2 pad) */
        OSReport("AI DMA Address =   0x%04x%04x\n",
                 DSP_HW_REGS[DSP_AI_DMA_START_H],
                 DSP_HW_REGS[DSP_AI_DMA_START_L]);
        OSReport("ARAM DMA Address = 0x%04x%04x\n",
                 DSP_HW_REGS[DSP_AR_DMA_MMADDR_H],
                 DSP_HW_REGS[DSP_AR_DMA_MMADDR_L]);
        OSReport("DI DMA Address =   0x%08x\n", DI_HW_REGS[DI_DMA_ADDR]);
        break;
    }

    OSReport("\nLast interrupt (%d): SRR0 = 0x%08x  TB = 0x%016llx\n\0\0\0\0\0\0\0",
             __OSLastInterrupt, __OSLastInterruptSrr0, __OSLastInterruptTime);
    PPCHalt();
}
