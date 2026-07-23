#include <revolution/GX.h>
#include <revolution/OS.h>
#include <revolution/base/PPCArch.h>

typedef enum {
    GX_MT_NULL = 0,
    GX_MT_XF_FLUSH = 1,
    GX_MT_DL_SAVE_CONTEXT = 2,
    GX_MT_ABORT_WAIT_COPYOUT = 3
} GXMiscToken;

typedef enum {
    GX_READ_00,
    GX_READ_FF,
    GX_READ_NONE
} GXAlphaReadMode;

/* Initialized in GXInit; retail accesses via SDA, not absolute PE/MI maps. */
extern volatile u16* __peReg;
extern volatile u16* __memReg;

#define MEM_PE_REQCOUNTH_IDX 0x27
#define MEM_PE_REQCOUNTL_IDX 0x28

#define GX_PE_REG_READ_U16(i) (*(volatile u16*)(__peReg + (i)))
#define GX_PE_REG_WRITE_U16(i, v) (*(volatile u16*)(__peReg + (i)) = (u16)(v))
#define GX_MEM_REG_READ_U16(i) (*(volatile u16*)(__memReg + (i)))

extern GXBool __GXIsGPFifoReady(void);
extern void __GXCleanGPFifo(void);
extern void __GXInitRevisionBits(void);

static GXDrawSyncCallback TokenCB;
static GXDrawDoneCallback DrawDoneCB;
static volatile GXBool DrawDone;
static OSThreadQueue FinishQueue;

void GXSetMisc(UNKWORD token, UNKWORD val) {
    switch ((GXMiscToken)token) {
    case GX_MT_NULL:
        break;
    case GX_MT_XF_FLUSH:
        gxdt->SHORT_0x4 = (u16)val;
        gxdt->SHORT_0x0 = (u16)!gxdt->SHORT_0x4;
        gxdt->lastWriteWasXF = TRUE;
        if (gxdt->SHORT_0x4 > 0) {
            gxdt->gxDirtyFlags |= GX_DIRTY_VCD;
        }
        break;
    case GX_MT_DL_SAVE_CONTEXT:
        gxdt->dlistSave = (val != 0) ? TRUE : FALSE;
        break;
    case GX_MT_ABORT_WAIT_COPYOUT:
        gxdt->BYTE_0x5FA = (u8)(val != 0);
        break;
    }
}

void GXFlush(void) {
    if (gxdt->gxDirtyFlags) {
        __GXSetDirtyState();
    }

    WGPIPE.i = 0;
    WGPIPE.i = 0;
    WGPIPE.i = 0;
    WGPIPE.i = 0;
    WGPIPE.i = 0;
    WGPIPE.i = 0;
    WGPIPE.i = 0;
    WGPIPE.i = 0;
    PPCSync();
}

inline void __GXAbortWait(u32 clocks) {
    s64 time0;
    s64 time1;

    time0 = OSGetTime();
    do {
        time1 = OSGetTime();
    } while (time1 - time0 <= (s64)(clocks / 4));
}

/* Petari-style MEM counter read: high-half consistency before combine. */
inline u32 __GXReadMEMCounterU32(u32 regAddrL, u32 regAddrH) {
    u32 ctrH0;
    u32 ctrH1;
    u32 ctrL;

    ctrH0 = GX_MEM_REG_READ_U16(regAddrH);
    do {
        ctrH1 = ctrH0;
        ctrL = GX_MEM_REG_READ_U16(regAddrL);
        ctrH0 = GX_MEM_REG_READ_U16(regAddrH);
    } while (ctrH0 != ctrH1);

    return (ctrH0 << 16) | ctrL;
}

inline void __GXAbortWaitPECopyDone(void) {
    u32 peCnt0;
    u32 peCnt1;

    peCnt0 =
        __GXReadMEMCounterU32(MEM_PE_REQCOUNTL_IDX, MEM_PE_REQCOUNTH_IDX);
    do {
        peCnt1 = peCnt0;
        __GXAbortWait(32);
        peCnt0 =
            __GXReadMEMCounterU32(MEM_PE_REQCOUNTL_IDX, MEM_PE_REQCOUNTH_IDX);
    } while (peCnt0 != peCnt1);
}

void __GXAbort(void) {
    if (gxdt->BYTE_0x5FA && __GXIsGPFifoReady()) {
        __GXAbortWaitPECopyDone();
    }

    PI_HW_REGS[PI_REG_0x18] = 1;
    __GXAbortWait(200);
    PI_HW_REGS[PI_REG_0x18] = 0;
    __GXAbortWait(20);
}

void GXAbortFrame(void) {
    /* IPA inlines __GXAbort / GXFlush into this context (retail has no bl). */
    __GXAbort();

    if (__GXIsGPFifoReady()) {
        __GXCleanGPFifo();
        __GXInitRevisionBits();
        gxdt->gxDirtyFlags = 0;
        GXFlush();
    }
}

void GXSetDrawSync(u16 token) {
    BOOL en;
    u32 reg;

    en = OSDisableInterrupts();

    /* oris shape: token | (PETOKENINT << 24), then rlwimi RID to PETOKEN */
    reg = (u32)token | (GX_BP_REG_PETOKENINT << 24);
    GX_BP_LOAD_REG(reg);
    reg = GX_BITSET(reg, 16, 16, token);
    GX_BP_SET_OPCODE(reg, GX_BP_REG_PETOKEN);
    GX_BP_LOAD_REG(reg);

    GXFlush();
    OSRestoreInterrupts(en);
    gxdt->lastWriteWasXF = FALSE;
}

u16 GXReadDrawSync(void) {
    return GX_PE_REG_READ_U16(7);
}

void GXSetDrawDone(void) {
    u32 reg;
    BOOL en = OSDisableInterrupts();

    /* lis/addi shape: (DRAWDONE << 24) | finish-dest; avoid SET_OPCODE rlwimi */
    reg = (GX_BP_REG_DRAWDONE << 24) | 2;
    GX_BP_LOAD_REG(reg);

    GXFlush();
    DrawDone = FALSE;
    OSRestoreInterrupts(en);
    gxdt->lastWriteWasXF = FALSE;
}

static inline void GXWaitDrawDone(void) {
    BOOL en = OSDisableInterrupts();
    while (!DrawDone) {
        OSSleepThread(&FinishQueue);
    }
    OSRestoreInterrupts(en);
}

void GXDrawDone(void) {
    /* IPA inlines GXSetDrawDone + GXWaitDrawDone (no WaitDrawDone symbol). */
    GXSetDrawDone();
    GXWaitDrawDone();
}

void GXPixModeSync(void) {
    GX_BP_LOAD_REG(gxdt->zControl);
    gxdt->lastWriteWasXF = FALSE;
}

void GXPokeAlphaMode(GXCompare func, u8 threshold) {
    u32 reg = ((u32)func << 8) | threshold;
    GX_PE_REG_WRITE_U16(3, reg);
}

void GXPokeAlphaRead(GXAlphaReadMode mode) {
    u32 reg = 0;
    reg = GX_BITSET(reg, 30, 2, mode);
    reg = GX_BITSET(reg, 29, 1, 1);
    GX_PE_REG_WRITE_U16(4, reg);
}

void GXPokeAlphaUpdate(GXBool update_en) {
    u32 reg = GX_PE_REG_READ_U16(1);
    GX_BP_SET_BLENDMODE_ALPHA_UPDATE(reg, update_en);
    GX_PE_REG_WRITE_U16(1, reg);
}

void GXPokeBlendMode(GXBlendMode type, GXBlendFactor src, GXBlendFactor dst,
                     GXLogicOp op) {
    u32 reg = GX_PE_REG_READ_U16(1);
    GX_BP_SET_BLENDMODE_BLEND_ENABLE(
        reg, (type == GX_BM_BLEND) || (type == GX_BM_SUBTRACT));
    GX_BP_SET_BLENDMODE_SUBTRACT(reg, type == GX_BM_SUBTRACT);
    GX_BP_SET_BLENDMODE_LOGIC_OP_ENABLE(reg, type == GX_BM_LOGIC);
    GX_BP_SET_BLENDMODE_LOGIC_MODE(reg, op);
    GX_BP_SET_BLENDMODE_SRC_FACTOR(reg, src);
    GX_BP_SET_BLENDMODE_DST_FACTOR(reg, dst);
    GX_BP_SET_OPCODE(reg, GX_BP_REG_BLENDMODE);
    GX_PE_REG_WRITE_U16(1, reg);
}

void GXPokeColorUpdate(GXBool update_en) {
    u32 reg = GX_PE_REG_READ_U16(1);
    GX_BP_SET_BLENDMODE_COLOR_UPDATE(reg, update_en);
    GX_PE_REG_WRITE_U16(1, reg);
}

void GXPokeDstAlpha(GXBool enable, u8 alpha) {
    u32 reg = 0;
    GX_BP_SET_DSTALPHA_ALPHA(reg, alpha);
    GX_BP_SET_DSTALPHA_ENABLE(reg, enable);
    GX_PE_REG_WRITE_U16(2, reg);
}

void GXPokeDither(GXBool dither) {
    u32 reg = GX_PE_REG_READ_U16(1);
    GX_BP_SET_BLENDMODE_DITHER(reg, dither);
    GX_PE_REG_WRITE_U16(1, reg);
}

void GXPokeZMode(GXBool compare_en, GXCompare func, GXBool update_en) {
    u32 reg = 0;
    GX_BP_SET_ZMODE_TEST_ENABLE(reg, compare_en);
    GX_BP_SET_ZMODE_COMPARE(reg, func);
    GX_BP_SET_ZMODE_UPDATE_ENABLE(reg, update_en);
    GX_PE_REG_WRITE_U16(0, reg);
}

GXDrawSyncCallback GXSetDrawSyncCallback(GXDrawSyncCallback cb) {
    GXDrawSyncCallback oldcb = TokenCB;
    BOOL en = OSDisableInterrupts();
    TokenCB = cb;
    OSRestoreInterrupts(en);
    return oldcb;
}

static void GXTokenInterruptHandler(s32 intr, OSContext* context) {
    u16 token;
    OSContext exceptionContext;
    u32 reg;

    token = GX_PE_REG_READ_U16(7);
    if (TokenCB) {
        OSClearContext(&exceptionContext);
        OSSetCurrentContext(&exceptionContext);
        (*TokenCB)(token);
        OSClearContext(&exceptionContext);
        OSSetCurrentContext(context);
    }

    reg = GX_PE_REG_READ_U16(5);
    reg |= 1 << 2;
    GX_PE_REG_WRITE_U16(5, reg);
}

GXDrawDoneCallback GXSetDrawDoneCallback(GXDrawDoneCallback cb) {
    GXDrawDoneCallback oldcb = DrawDoneCB;
    BOOL en = OSDisableInterrupts();
    DrawDoneCB = cb;
    OSRestoreInterrupts(en);
    return oldcb;
}

static void GXFinishInterruptHandler(s32 intr, OSContext* context) {
    OSContext exceptionContext;
    u32 reg;

    reg = GX_PE_REG_READ_U16(5);
    reg |= 1 << 3;
    GX_PE_REG_WRITE_U16(5, reg);

    DrawDone = TRUE;

    if (DrawDoneCB) {
        OSClearContext(&exceptionContext);
        OSSetCurrentContext(&exceptionContext);
        (*DrawDoneCB)();
        OSClearContext(&exceptionContext);
        OSSetCurrentContext(context);
    }

    OSWakeupThread(&FinishQueue);
}

void __GXPEInit(void) {
    u32 reg;

    __OSSetInterruptHandler(OS_INTR_PI_PE_TOKEN, GXTokenInterruptHandler);
    __OSSetInterruptHandler(OS_INTR_PI_PE_FINISH, GXFinishInterruptHandler);

    OSInitThreadQueue(&FinishQueue);

    __OSUnmaskInterrupts(OS_INTR_MASK(OS_INTR_PI_PE_TOKEN));
    __OSUnmaskInterrupts(OS_INTR_MASK(OS_INTR_PI_PE_FINISH));

    reg = GX_PE_REG_READ_U16(5);
    reg |= (1 << 2) | (1 << 3) | (1 << 0) | (1 << 1);
    GX_PE_REG_WRITE_U16(5, reg);
}
