#include <revolution/OS.h>
#include <revolution/SI.h>
#include <revolution/VI.h>

typedef enum {
    SI_ERROR_NOREP = (1 << 3),
    SI_ERROR_BUSY = (1 << 7),
    SI_WIRELESS_LITE = (1 << 18),
    SI_WIRELESS_CONT = (1 << 19),
} SIStatus;

typedef void (*SICallbackCtx)(s32 chan, u32 status, OSContext* ctx);

typedef struct SIMain {
    s32 chan;               // at 0x0
    u32 poll;               // at 0x4
    u32 inSize;             // at 0x8
    void* inAddr;           // at 0xC
    SICallbackCtx callback; // at 0x10
    s32 pad;                // at 0x14
} SIMain;

typedef struct SIPacket {
    s32 chan;            // at 0x0
    void* outAddr;       // at 0x4
    u32 outSize;         // at 0x8
    void* inAddr;        // at 0xC
    u32 inSize;          // at 0x10
    SICallback callback; // at 0x14
    s64 fire;            // at 0x18
} SIPacket;

const char* __SIVersion =
    "<< RVL_SDK - SI \trelease build: Feb 27 2009 10:04:44 (0x4302_145) >>";

SIMain Si = {SI_CHAN_NONE};
u32 Type[SI_MAX_CHAN] = {SI_ERROR_NOREP, SI_ERROR_NOREP, SI_ERROR_NOREP,
                         SI_ERROR_NOREP};

SIPacket Packet[SI_MAX_CHAN];
s64 XferTime[SI_MAX_CHAN];
s64 TypeTime[SI_MAX_CHAN];
BOOL InputBufferValid[SI_MAX_CHAN];
u32 InputBuffer[SI_MAX_CHAN][2];
OSInterruptHandler RDSTHandler[SI_MAX_CHAN];
u32 InputBufferVcount[SI_MAX_CHAN];
OSAlarm Alarm[SI_MAX_CHAN];
SICallback TypeCallback[SI_MAX_CHAN][SI_MAX_TYPE];
u32 __PADFixBits;

static void SIClearTCInterrupt(void) {
    u32 csr = SI_HW_REGS[SI_SICOMSCR];

    csr |= SI_SICOMCSR_TCINT;
    csr &= ~SI_SICOMCSR_TSTART;

    SI_HW_REGS[SI_SICOMSCR] = csr;
}

u32 CompleteTransfer(void) {
    u32 i;
    u32 sr;
    u8* dst;
    u32 size;
    u32 imm;

    sr = SI_HW_REGS[SI_SISR];
    SIClearTCInterrupt();

    if (Si.chan != SI_CHAN_NONE) {
        XferTime[Si.chan] = __OSGetSystemTime();

        dst = (u8*)Si.inAddr;

        size = Si.inSize / sizeof(u32);
        for (i = 0; i < size; dst += sizeof(u32), i++) {
            *(u32*)dst = SI_HW_REGS[SI_RAM_BASE + i];
        }

        size = Si.inSize % sizeof(u32);
        if (size > 0) {
            imm = SI_HW_REGS[SI_RAM_BASE + i];

            for (i = 0; i < size; dst++, i++) {
                *dst = imm >> (3 - i) * 8;
            }
        }

        if (SI_HW_REGS[SI_SICOMSCR] & SI_SICOMCSR_COMERR) {
            // Flags relative to channel ID
            sr >>= ((SI_MAX_CHAN - 1) - Si.chan) * 8;
            sr &= (SI_UNRUN | SI_OVRUN | SI_COLL | SI_NOREP);

            if ((sr & SI_NOREP) && (Type[Si.chan] & SI_ERROR_BUSY) == 0) {
                Type[Si.chan] = SI_ERROR_NOREP;
            }

            if (sr == 0) {
                sr = SI_COLL;
            }
        } else {
            TypeTime[Si.chan] = __OSGetSystemTime();
            sr = 0;
        }

        Si.chan = SI_CHAN_NONE;
    }

    return sr;
}

static void GetTypeCallback(s32 chan, u32 status, OSContext* ctx);

static void SIInterruptHandler(s16 intr, OSContext* ctx) {
    u32 comsr;
    u32 csr;
    s32 chan;
    u32 sr;
    SICallbackCtx callback;
    s32 i;
    s32 chan2;
    SIPacket* p;
    s64 now;
    s64 wait;
    u32 line;
    u32 poll;
    u32 v;
    u32 hi;
    BOOL enabled;
    static u32 cmdTypeAndStatus;

    comsr = SI_HW_REGS[SI_SICOMSCR];

    if ((comsr & 0xC0000000) == 0xC0000000) {
        chan = Si.chan;
        sr = CompleteTransfer();
        callback = Si.callback;
        Si.callback = NULL;

        chan2 = chan;
        for (i = 0; i < SI_MAX_CHAN; i++) {
            chan2 = (chan2 + 1) % SI_MAX_CHAN;
            p = &Packet[chan2];
            if (p->chan == SI_CHAN_NONE) {
                continue;
            }
            now = __OSGetSystemTime();
            if (now < p->fire) {
                continue;
            }
            if (__SITransfer(p->chan, p->outAddr, p->outSize, p->inAddr,
                             p->inSize, p->callback)) {
                OSCancelAlarm(&Alarm[chan2]);
                p->chan = SI_CHAN_NONE;
            }
            break;
        }

        if (callback != NULL) {
            callback(chan, sr, ctx);
        }

        csr = SI_HW_REGS[SI_SISR];
        csr &= 0x0F000000 >> (chan * 8);
        SI_HW_REGS[SI_SISR] = csr;

        if (Type[chan] == SI_ERROR_BUSY) {
            BOOL pending = Packet[chan].chan != SI_CHAN_NONE || Si.chan == chan;
            if (!pending) {
                v = *(volatile u32*)0x800000F8;
                hi = __mulhwu(0x431BDE83, v >> 2);
                wait = ((hi >> 15) + (((hi << 23) | (hi >> 9)) & 0x7FFFC0)) >> 3;
                SITransfer(chan, &cmdTypeAndStatus, 1, &Type[chan], 3,
                           (SICallback)GetTypeCallback, wait);
            }
        }
    }

    if ((comsr & 0x18000000) == 0x18000000) {
        line = VIGetCurrentLine() + 1;
        poll = (Si.poll >> 6) & 0x3FF;

        for (i = 0; i < SI_MAX_CHAN; i++) {
            u32 sr2;
            BOOL rdst;

            enabled = OSDisableInterrupts();
            sr2 = SI_HW_REGS[SI_SISR] >> ((SI_MAX_CHAN - 1) - i) * 8;
            if ((sr2 & SI_ERROR_NOREP) && !(Type[i] & SI_ERROR_BUSY)) {
                Type[i] = SI_ERROR_NOREP;
            }
            OSRestoreInterrupts(enabled);
            if (sr2 & SI_ERROR_RDST) {
                rdst = TRUE;
                InputBuffer[i][0] = SI_HW_REGS[3 * i + 1];
                InputBuffer[i][1] = SI_HW_REGS[3 * i + 2];
                InputBufferValid[i] = TRUE;
            } else {
                rdst = FALSE;
            }
            if (rdst) {
                InputBufferVcount[i] = line;
            }
        }

        for (i = 0; i < SI_MAX_CHAN; i++) {
            if (Si.poll & (0x80000000 >> (24 + i))) {
                if (InputBufferVcount[i] != 0) {
                    if ((InputBufferVcount[i] + (poll >> 1)) >= line) {
                        continue;
                    }
                    return;
                }
                return;
            }
        }

        InputBufferVcount[0] = 0;
        InputBufferVcount[1] = 0;
        InputBufferVcount[2] = 0;
        InputBufferVcount[3] = 0;
        for (i = 0; i < SI_MAX_CHAN; i++) {
            if (RDSTHandler[i] != NULL) {
                RDSTHandler[i](intr, ctx);
            }
        }
    }
}

void SIInit(void) {
    static BOOL Initialized = FALSE;

    if (Initialized) {
        return;
    }

    OSRegisterVersion(__SIVersion);

    Packet[SI_CHAN_0].chan = Packet[SI_CHAN_1].chan = Packet[SI_CHAN_2].chan =
        Packet[SI_CHAN_3].chan = SI_CHAN_NONE;

    Si.poll = 0;
    SISetSamplingRate(0);

    while (SI_HW_REGS[SI_SICOMSCR] & SI_SICOMCSR_TSTART) {
        ;
    }

    SI_HW_REGS[SI_SICOMSCR] = SI_SICOMCSR_TCINT;

    __OSSetInterruptHandler(OS_INTR_PI_SI, (OSInterruptHandler)SIInterruptHandler);
    __OSUnmaskInterrupts(OS_INTR_MASK(OS_INTR_PI_SI));

    SIGetType(SI_CHAN_0);
    SIGetType(SI_CHAN_1);
    SIGetType(SI_CHAN_2);
    SIGetType(SI_CHAN_3);

    Initialized = TRUE;
}

static BOOL __SITransfer(s32 chan, void* outAddr, u32 outSize, void* inAddr,
                         u32 inSize, SICallback callback) {
    BOOL enabled;
    u32 sr;
    u32 alignSize;
    u32 i;

    union {
        struct {
            u32 TCINT : 1;
            u32 TCINTMSK : 1;
            u32 COMERR : 1;
            u32 RDSTINT : 1;
            u32 RDSTINTMSK : 1;
            u32 reserved2 : 4;
            u32 OUTLNGTH : 7;
            u32 reserved1 : 1;
            u32 INLNGTH : 7;
            u32 reserved0 : 5;
            u32 CHANNEL : 2;
            u32 TSTART : 1;
        };

        u32 reg;
    } comscr;

    enabled = OSDisableInterrupts();

    if (Si.chan != SI_CHAN_NONE) {
        OSRestoreInterrupts(enabled);
        return FALSE;
    }

    sr = SI_HW_REGS[SI_SISR];
    sr &= (SI_SISR_UNRUN0 | SI_SISR_OVRUN0 | SI_SISR_COLL0 | SI_SISR_NOREP0) >>
          (chan * 8);
    SI_HW_REGS[SI_SISR] = sr;

    Si.chan = chan;
    Si.callback = (SICallbackCtx)callback;
    Si.inSize = inSize;
    Si.inAddr = inAddr;

    alignSize = (outSize + 3) / 4;
    {
        u32* out = (u32*)outAddr;
        for (i = 0; i < alignSize; i++) {
            SI_HW_REGS[SI_RAM_BASE + i] = out[i];
        }
    }

    comscr.reg = SI_HW_REGS[SI_SICOMSCR];

    comscr.TCINT = TRUE;
    comscr.TCINTMSK = callback != NULL;
    // Minimum transfer is 1 byte. 0x00 will transfer 128 bytes
    comscr.OUTLNGTH = outSize == 128 ? 0 : outSize;
    comscr.INLNGTH = inSize == 128 ? 0 : inSize;
    comscr.CHANNEL = chan;
    comscr.TSTART = TRUE;

    SI_HW_REGS[SI_SICOMSCR] = comscr.reg;

    OSRestoreInterrupts(enabled);
    return TRUE;
}

u32 SISetXY(u32 lines, u32 times) {
    BOOL enabled;
    u32 poll;

    poll = lines << 16;
    poll |= times << 8;

    enabled = OSDisableInterrupts();

    Si.poll &= ~(SI_SIPOLL_X | SI_SIPOLL_Y);
    Si.poll |= poll;

    poll = Si.poll;
    SI_HW_REGS[SI_SIPOLL] = poll;

    OSRestoreInterrupts(enabled);
    return poll;
}

static void AlarmHandler(OSAlarm* alarm, OSContext* ctx) {
    s32 chan;
    SIPacket* packet;

    chan = alarm - Alarm;
    packet = &Packet[chan];

    if (packet->chan != SI_CHAN_NONE) {
        if (__SITransfer(packet->chan, packet->outAddr, packet->outSize,
                         packet->inAddr, packet->inSize, packet->callback)) {
            packet->chan = SI_CHAN_NONE;
        }
    }
}

BOOL SITransfer(s32 chan, void* outAddr, u32 outSize, void* inAddr, u32 inSize,
                SICallback callback, s64 wait) {
    SIPacket* packet;
    BOOL enabled;
    s64 start;
    s64 fire;

    packet = &Packet[chan];
    enabled = OSDisableInterrupts();

    if (packet->chan != SI_CHAN_NONE || Si.chan == chan) {
        OSRestoreInterrupts(enabled);
        return FALSE;
    }

    start = __OSGetSystemTime();
    fire = wait == 0 ? start : wait + XferTime[chan];

    if (start < fire) {
        OSSetAlarm(&Alarm[chan], fire - start, AlarmHandler);
    } else if (__SITransfer(chan, outAddr, outSize, inAddr, inSize, callback)) {
        OSRestoreInterrupts(enabled);
        return TRUE;
    }

    packet->chan = chan;
    packet->outAddr = outAddr;
    packet->outSize = outSize;
    packet->inAddr = inAddr;
    packet->inSize = inSize;
    packet->callback = callback;
    packet->fire = fire;

    OSRestoreInterrupts(enabled);
    return TRUE;
}

static void GetTypeCallback(s32 chan, u32 status, OSContext* ctx) {
    u32 type;
    u32 chanBit;
    u32 fix;
    s32 i;
    u32 id;
    u32 cmd;
    static u32 cmdFixDevice[SI_MAX_CHAN];

    Type[chan] &= ~SI_ERROR_BUSY;
    Type[chan] |= status;
    TypeTime[chan] = __OSGetSystemTime();
    type = Type[chan];
    chanBit = 0x80000000 >> chan;
    fix = __PADFixBits & chanBit;
    __PADFixBits &= ~chanBit;

    if ((status & 0xF) || (type & 0x18000000) != 0x08000000 ||
        !(type & 0x80000000) || (type & 0x04000000)) {
        OSSetWirelessID(chan, 0);
        type = Type[chan];
        for (i = 0; i < SI_MAX_TYPE; i++) {
            SICallback cb = TypeCallback[chan][i];
            if (cb != NULL) {
                TypeCallback[chan][i] = NULL;
                cb(chan, type);
            }
        }
        return;
    }

    id = OSGetWirelessID(chan) << 8;

    if (fix && (id & 0x00100000)) {
        u32* fixDev = &cmdFixDevice[chan];
        cmd = (id & 0xCFFF00) | 0x4E100000;
        *fixDev = cmd;
        Type[chan] = SI_ERROR_BUSY;
        SITransfer(chan, fixDev, 3, &Type[chan], 3, (SICallback)GetTypeCallback,
                   0);
        return;
    }

    if (type & 0x00100000) {
        if ((id & 0xCFFF00) != (type & 0xCFFF00)) {
            if (!(id & 0x00100000)) {
                id = (type & 0xCFFF00) | 0x00100000;
                OSSetWirelessID(chan, (u16)(id >> 8));
            }
            cmd = 0x4E000000 | id;
            cmdFixDevice[chan] = cmd;
            Type[chan] = SI_ERROR_BUSY;
            SITransfer(chan, &cmdFixDevice[chan], 3, &Type[chan], 3,
                       (SICallback)GetTypeCallback, 0);
            return;
        }
    } else if (type & 0x40000000) {
        id = (type & 0xCFFF00) | 0x00100000;
        OSSetWirelessID(chan, (u16)(id >> 8));
        cmd = 0x4E000000 | id;
        cmdFixDevice[chan] = cmd;
        Type[chan] = SI_ERROR_BUSY;
        SITransfer(chan, &cmdFixDevice[chan], 3, &Type[chan], 3,
                   (SICallback)GetTypeCallback, 0);
        return;
    } else {
        OSSetWirelessID(chan, 0);
    }

    type = Type[chan];
    for (i = 0; i < SI_MAX_TYPE; i++) {
        SICallback cb = TypeCallback[chan][i];
        if (cb != NULL) {
            TypeCallback[chan][i] = NULL;
            cb(chan, type);
        }
    }
}

u32 SIGetType(s32 chan) {
    static u32 cmdTypeAndStatus;
    BOOL enabled;
    u32 type;
    s64 diff;

    enabled = OSDisableInterrupts();
    type = Type[chan];
    diff = __OSGetSystemTime() - TypeTime[chan];

    if (Si.poll & (0x80 >> chan)) {
        if (type != SI_ERROR_NOREP) {
            TypeTime[chan] = __OSGetSystemTime();
            OSRestoreInterrupts(enabled);
            return type;
        }
        Type[chan] = SI_ERROR_BUSY;
        type = SI_ERROR_BUSY;
    } else if (diff <= OS_MSEC_TO_TICKS(50) && type != SI_ERROR_NOREP) {
        OSRestoreInterrupts(enabled);
        return type;
    } else if (diff <= OS_MSEC_TO_TICKS(75)) {
        Type[chan] = SI_ERROR_BUSY;
    } else {
        type = Type[chan] = SI_ERROR_BUSY;
    }

    TypeTime[chan] = __OSGetSystemTime();
    SITransfer(chan, &cmdTypeAndStatus, 1, &Type[chan], 3,
               (SICallback)GetTypeCallback, OS_USEC_TO_TICKS(65));
    OSRestoreInterrupts(enabled);
    return type;
}

void SISetCommand(s32 chan, u32 command) {
    SI_HW_REGS[chan * 3] = command;
}
void SITransferCommands() {
    *(volatile u32*)0xCD006438 = 0x80000000;
}
