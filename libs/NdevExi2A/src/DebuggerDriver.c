
/*
 * Ndev EXI2 debugger driver (NdevExi2A).
 *
 * Host-PC debugger <-> console link over EXI channel 2. The host polls a
 * mailbox (each mail is a message descriptor) and moves data through the
 * EXI2 RAM commands; the DB* functions are the guest-side API.
 */
#include "NdevExi2A/DebuggerDriver.h"
#include "NdevExi2A/ODEMU.h"
#include "NdevExi2A/exi2.h"

#include <revolution/OS.h>

static u32 __DBRecvDataSize;
static u32 __DBRecvMail;
static u8 __DBEXIInputFlag;
static OSInterruptHandler __DBDbgCallback;
static OSInterruptHandler __DBMtrCallback;

static u8 __DBReadUSB_CSR(void);
static void __DBWaitForSendMail(void);

void __DBMtrHandler(s32 type, OSContext* ctx) {
    __DBEXIInputFlag = TRUE;
    if (__DBMtrCallback != NULL)
        __DBMtrCallback(0, ctx);
}

void __DBIntrHandler(s32 type, OSContext* ctx) {
    // Acknowledge the PI debug interrupt.
    PI_HW_REGS[PI_INTSR] = PI_INTSR_DEBUG;
    if (__DBDbgCallback != NULL)
        __DBDbgCallback(type, ctx);
}

static void __DBCheckMailBox(void) {
    const u8 csr = __DBReadUSB_CSR();
    // CSR bit 3 clear = the receive mailbox holds a new mail.
    if (!(csr & 0x8)) {
        u32 mail;
        __DBReadMailbox(&mail);
        if (ODEMUIsValidMail(mail)) {
            __DBRecvMail = mail;
            __DBRecvDataSize = ODEMUGetSize(mail);
            __DBEXIInputFlag = TRUE;
        }
    }
}

static u8 __DBReadUSB_CSR(void) {
    u8 val;
    // 0x34000000 = EXI2 command word for the debugger CSR register (read).
    __DBEXIReadReg(0x34000000, &val, sizeof(val));
    return val;
}

void DBInitComm(u8** flagOut, OSInterruptHandler handler) {
    const BOOL enabled = OSDisableInterrupts();

    *flagOut = &__DBEXIInputFlag;
    __DBMtrCallback = handler;
    __DBEXIInit();

    OSRestoreInterrupts(enabled);
}

// Retail compiled DBInitInterrupts at -O0. Keep the pragma scoped with
// push/pop: an unguarded "#pragma optimization_level 0" would also compile
// the following functions (DBQueryData/DBRead/DBWrite) unoptimized, breaking
// their byte match and overflowing the 0x334 split budget.
#pragma push
#pragma optimization_level 0

void DBInitInterrupts(void) {
    __OSMaskInterrupts(OS_INTR_MASK(OS_INTR_EXI_2_EXI) |
                       OS_INTR_MASK(OS_INTR_EXI_2_TC));
    __OSMaskInterrupts(OS_INTR_MASK(OS_INTR_PI_DEBUG));
    __DBDbgCallback = __DBMtrHandler;
    __OSSetInterruptHandler(OS_INTR_PI_DEBUG, __DBIntrHandler);
    __OSUnmaskInterrupts(OS_INTR_MASK(OS_INTR_PI_DEBUG));
}

#pragma pop

u32 DBQueryData(void) {
    __DBEXIInputFlag = FALSE;

    if (__DBRecvDataSize == 0) {
        const BOOL enabled = OSDisableInterrupts();
        __DBCheckMailBox();
        OSRestoreInterrupts(enabled);
    }

    return __DBRecvDataSize;
}

BOOL DBRead(void* dst, u32 size) {
    const BOOL enabled = OSDisableInterrupts();

    // Mail payload buffer: base 0x1000 + page offset decoded from the mail.
    __DBRead(ODEMUGetPc2NngcOffset(__DBRecvMail) + 0x1000, dst,
             ROUND_UP(size, 4));
    __DBRecvDataSize = 0;
    __DBEXIInputFlag = FALSE;

    OSRestoreInterrupts(enabled);

    return FALSE; // Retail always returns FALSE here.
}

BOOL DBWrite(const void* src, u32 size) {
    // Alternates the RAM offset between the two 0x800-byte mail pages on each
    // call (odd counter -> page 1); the page is also stamped into the mail.
    static u8 l_byOffsetCounter = 128;
    const BOOL enabled = OSDisableInterrupts();

    u32 ofs, mail;

    __DBWaitForSendMail();

    ofs = 0;
    if (!(++l_byOffsetCounter & 0x1)) {
        ofs = 0;
    } else {
        ofs = 0x800;
    }

    // Busy-retry until the debugger accepts the RAM write.
    while (__DBWrite(ofs, src, ROUND_UP(size, 4)) == 0) {
    }

    __DBWaitForSendMail();

    mail = ODEMUGenMailData(l_byOffsetCounter, size);
    // Busy-retry until the send mailbox accepts the descriptor.
    while (__DBWriteMailbox(mail) == 0) {
    }

    __DBWaitForSendMail();

    OSRestoreInterrupts(enabled);

    return FALSE; // Retail always returns FALSE here.
}

static void __DBWaitForSendMail(void) {
    // CSR bit 2 set = a send is still in flight; wait until it completes.
    while (__DBReadUSB_CSR() & 0x4) {
    }
}

void DBOpen(void) {} // No-op in retail.

void DBClose(void) {} // No-op in retail.
