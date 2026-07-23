#include <revolution/GX.h>
#include <revolution/OS.h>
#include <revolution/base/PPCArch.h>
#include <string.h>

#define TOPHYSICAL(a) (((u32)(a)) & 0x3FFFFFFF)

#define CP_ENABLE 0x01
#define CP_CLR 0x02
#define CP_STATUS 0x00

#define CP_FIFO_BASEL 0x10
#define CP_FIFO_BASEH 0x11
#define CP_FIFO_TOPL 0x12
#define CP_FIFO_TOPH 0x13
#define CP_FIFO_HICNTL 0x14
#define CP_FIFO_HICNTH 0x15
#define CP_FIFO_LOCNTL 0x16
#define CP_FIFO_LOCNTH 0x17
#define CP_FIFO_COUNTL 0x18
#define CP_FIFO_COUNTH 0x19
#define CP_FIFO_WPTRL 0x1A
#define CP_FIFO_WPTRH 0x1B
#define CP_FIFO_RPTRL 0x1C
#define CP_FIFO_RPTRH 0x1D
#define CP_FIFO_BRKL 0x1E
#define CP_FIFO_BRKH 0x1F

#define PI_REG_CPBAS 0x0C
#define PI_REG_CPTOP 0x10
#define PI_REG_CPWRT 0x14

#define GX_CP_REG_READ_U16(a) \
    (*(volatile u16*)((volatile u16*)__cpReg + (a)))

#define GX_CP_REG_WRITE_U16(a, d) \
    (*(volatile u16*)((volatile u16*)__cpReg + (a)) = (u16)(d))

#define GX_PI_REG_READ_U32(off) \
    (*(volatile u32*)((u8*)__piReg + (off)))

#define GX_PI_REG_WRITE_U32(off, v) \
    (*(volatile u32*)((u8*)__piReg + (off)) = (u32)(v))

#define CP_CTRL_SET_OVFLINT(reg, v) \
    ((reg) = GX_BITSET((reg), 2, 1, (v)))

#define CP_CTRL_SET_UNFLINT(reg, v) \
    ((reg) = GX_BITSET((reg), 3, 1, (v)))

#define CP_CTRL_SET_FIFORD(reg, v) \
    ((reg) = GX_BITSET((reg), 0, 1, (v)))

#define CP_CTRL_SET_WRPTRINC(reg, v) \
    ((reg) = GX_BITSET((reg), 4, 1, (v)))

#define CP_CTRL_SET_FIFOBRK(reg, v) \
    ((reg) = GX_BITSET((reg), 1, 1, (v)))

#define CP_CTRL_SET_FIFOBRKINT(reg, v) \
    ((reg) = GX_BITSET((reg), 5, 1, (v)))

#define CP_CLR_SET_OVFL(reg, v) \
    ((reg) = GX_BITSET((reg), 0, 1, (v)))

#define CP_CLR_SET_UNFL(reg, v) \
    ((reg) = GX_BITSET((reg), 1, 1, (v)))

#define CP_STAT_GET_UNFL(reg) GX_BITGET((reg), 1, 1)
#define CP_STAT_GET_OVFL(reg) GX_BITGET((reg), 0, 1)
#define CP_STAT_GET_FIFOBRK(reg) GX_BITGET((reg), 4, 1)

#define __GXWriteFifoIntEnable(hi, lo) \
    do { \
        CP_CTRL_SET_OVFLINT(gxdt->cpCtrlReg, (hi)); \
        CP_CTRL_SET_UNFLINT(gxdt->cpCtrlReg, (lo)); \
        GX_CP_REG_WRITE_U16(CP_ENABLE, (u16)gxdt->cpCtrlReg); \
    } while (0)

#define __GXWriteFifoIntReset(hi, lo) \
    do { \
        CP_CLR_SET_OVFL(gxdt->cpClrReg, (hi)); \
        CP_CLR_SET_UNFL(gxdt->cpClrReg, (lo)); \
        GX_CP_REG_WRITE_U16(CP_CLR, (u16)gxdt->cpClrReg); \
    } while (0)

#define __GXFifoReadEnable() \
    do { \
        CP_CTRL_SET_FIFORD(gxdt->cpCtrlReg, 1); \
        GX_CP_REG_WRITE_U16(CP_ENABLE, (u16)gxdt->cpCtrlReg); \
    } while (0)

#define __GXFifoReadDisable() \
    do { \
        CP_CTRL_SET_FIFORD(gxdt->cpCtrlReg, 0); \
        GX_CP_REG_WRITE_U16(CP_ENABLE, (u16)gxdt->cpCtrlReg); \
    } while (0)

#define __GXFifoLink(en) \
    do { \
        CP_CTRL_SET_WRPTRINC(gxdt->cpCtrlReg, (en) ? 1 : 0); \
        GX_CP_REG_WRITE_U16(CP_ENABLE, (u16)gxdt->cpCtrlReg); \
    } while (0)

extern volatile void* __piReg;
extern volatile void* __cpReg;

static GXFifoObjImpl GPFifo;
static GXFifoObjImpl CPUFifo;
static u8 CPUFifoReady;
static u8 GPFifoReady[3];
static volatile u32 __GXOverflowCount;
volatile void* __GXCurrentBP;
static GXBreakPtCallback BreakPointCB;
static volatile BOOL GXOverflowSuspendInProgress;
static OSThread* __GXCurrentThread;
static u8 CPGPLinked[8];

static const char sCpuFifoReport[0x18] = "CPUFifo: %08X - %08X\n";
static const char sGpFifoReport[0x18] = "GP Fifo: %08X - %08X\n";

void GXCPInterruptHandler(s32 interrupt, OSContext* context) {
    gxdt->cpStatReg = GX_CP_REG_READ_U16(CP_STATUS);

    if (GX_BITGET(gxdt->cpCtrlReg, 3, 1) && CP_STAT_GET_UNFL(gxdt->cpStatReg)) {
        OSResumeThread(__GXCurrentThread);
        GXOverflowSuspendInProgress = FALSE;
        __GXWriteFifoIntReset(GX_TRUE, GX_TRUE);
        __GXWriteFifoIntEnable(GX_TRUE, GX_FALSE);
    }

    if (GX_BITGET(gxdt->cpCtrlReg, 2, 1) && CP_STAT_GET_OVFL(gxdt->cpStatReg)) {
        __GXOverflowCount++;
        __GXWriteFifoIntEnable(GX_FALSE, GX_TRUE);
        __GXWriteFifoIntReset(GX_TRUE, GX_FALSE);
        GXOverflowSuspendInProgress = TRUE;
        OSSuspendThread(__GXCurrentThread);
    }

    if (GX_BITGET(gxdt->cpCtrlReg, 5, 1) && CP_STAT_GET_FIFOBRK(gxdt->cpStatReg)) {
        OSContext exceptionContext;

        CP_CTRL_SET_FIFOBRKINT(gxdt->cpCtrlReg, 0);
        GX_CP_REG_WRITE_U16(CP_ENABLE, (u16)gxdt->cpCtrlReg);

        if (BreakPointCB) {
            OSClearContext(&exceptionContext);
            OSSetCurrentContext(&exceptionContext);
            BreakPointCB();
            OSClearContext(&exceptionContext);
            OSSetCurrentContext(context);
        }
    }
}

void GXInitFifoBase(GXFifoObj* fifo, void* base, u32 size) {
    GXFifoObjImpl* realFifo = (GXFifoObjImpl*)fifo;
    u32 lo;
    BOOL en;

    lo = (size >> 1) & ~1u;

    realFifo->base = base;
    realFifo->end = (void*)((u8*)base + size - 4);
    realFifo->size = size;
    realFifo->count = 0;
    realFifo->hiWatermark = (void*)(size - 0x4000);
    realFifo->loWatermark = (void*)lo;

    en = OSDisableInterrupts();
    realFifo->readPtr = base;
    realFifo->writePtr = base;
    realFifo->count = 0;
    if ((s32)realFifo->count < 0) {
        realFifo->count += realFifo->size;
    }
    OSRestoreInterrupts(en);
}

GXBool CPGPLinkCheck(void) {
    u32 check = 0;
    s32 range1;
    s32 range2;

    if (!CPUFifoReady || !GPFifoReady[0]) {
        return GX_FALSE;
    }

    if (CPUFifo.base == GPFifo.base) {
        check++;
    }

    if (CPUFifo.end == GPFifo.end) {
        check++;
    }

    if (check == 2) {
        return GX_TRUE;
    }

    range1 = (s32)((u8*)CPUFifo.end - (u8*)GPFifo.base);
    range2 = (s32)((u8*)GPFifo.end - (u8*)CPUFifo.base);

    if ((range1 > 0 && range2 > 0) || (range1 < 0 && range2 < 0)) {
        OSReport(sCpuFifoReport, (u32)CPUFifo.base, (u32)CPUFifo.end);
        OSReport(sGpFifoReport, (u32)GPFifo.base, (u32)GPFifo.end);
    }

    return GX_FALSE;
}

void GXSetCPUFifo(GXFifoObj* fifo) {
    GXFifoObjImpl* realFifo;
    BOOL en;
    u32 reg;

    en = OSDisableInterrupts();

    if (fifo == NULL) {
        CPUFifoReady = 0;
        CPGPLinked[0] = 0;
        CPUFifo.bind_cpu = GX_FALSE;
        CPUFifo.bind_gp = GX_FALSE;
        OSRestoreInterrupts(en);
        return;
    }

    realFifo = (GXFifoObjImpl*)fifo;
    CPUFifo = *realFifo;
    CPUFifoReady = 1;
    CPUFifo.bind_cpu = GX_TRUE;

    if (CPGPLinkCheck()) {
        CPGPLinked[0] = 1;
        CPUFifo.bind_gp = GX_TRUE;

        reg = 0;

        GX_PI_REG_WRITE_U32(PI_REG_CPBAS, TOPHYSICAL(CPUFifo.base) >> 2);
        GX_PI_REG_WRITE_U32(PI_REG_CPTOP, TOPHYSICAL(CPUFifo.end) >> 2);
        reg = GX_BITSET(reg, 3, 24, TOPHYSICAL(CPUFifo.writePtr) >> 5);
        GX_PI_REG_WRITE_U32(PI_REG_CPWRT, reg);

        __GXWriteFifoIntReset(GX_TRUE, GX_TRUE);
        __GXWriteFifoIntEnable(GX_TRUE, GX_FALSE);
        __GXFifoLink(GX_TRUE);
    } else {
        CPUFifo.bind_gp = GX_FALSE;

        if (CPGPLinked[0]) {
            __GXFifoLink(GX_FALSE);
            CPGPLinked[0] = 0;
        }

        __GXWriteFifoIntEnable(GX_FALSE, GX_FALSE);

        reg = 0;

        GX_PI_REG_WRITE_U32(PI_REG_CPBAS, TOPHYSICAL(CPUFifo.base) >> 2);
        GX_PI_REG_WRITE_U32(PI_REG_CPTOP, TOPHYSICAL(CPUFifo.end) >> 2);
        reg = GX_BITSET(reg, 3, 24, TOPHYSICAL(CPUFifo.writePtr) >> 5);
        GX_PI_REG_WRITE_U32(PI_REG_CPWRT, reg);
    }

    PPCSync();
    OSRestoreInterrupts(en);
}

void GXSetGPFifo(GXFifoObj* fifo) {
    GXFifoObjImpl* realFifo;
    BOOL en;
    u32 stbtmp;

    en = OSDisableInterrupts();
    __GXFifoReadDisable();
    __GXWriteFifoIntEnable(GX_FALSE, GX_FALSE);

    if (fifo == NULL) {
        GPFifoReady[0] = 0;
        CPGPLinked[0] = 0;
        __GXFifoLink(GX_FALSE);
        GPFifo.bind_gp = GX_FALSE;
        GPFifo.bind_cpu = GX_FALSE;
        OSRestoreInterrupts(en);
        return;
    }

    realFifo = (GXFifoObjImpl*)fifo;
    GPFifo = *realFifo;
    GPFifoReady[0] = 1;
    GPFifo.bind_gp = GX_TRUE;

    GX_CP_REG_WRITE_U16(CP_FIFO_BASEL, (u16)TOPHYSICAL(GPFifo.base));
    GX_CP_REG_WRITE_U16(CP_FIFO_TOPL, (u16)TOPHYSICAL(GPFifo.end));
    GX_CP_REG_WRITE_U16(CP_FIFO_COUNTL, (u16)GPFifo.count);
    GX_CP_REG_WRITE_U16(CP_FIFO_WPTRL, (u16)TOPHYSICAL(GPFifo.writePtr));
    GX_CP_REG_WRITE_U16(CP_FIFO_RPTRL, (u16)TOPHYSICAL(GPFifo.readPtr));
    GX_CP_REG_WRITE_U16(CP_FIFO_HICNTL, (u16)(u32)GPFifo.hiWatermark);
    GX_CP_REG_WRITE_U16(CP_FIFO_LOCNTL, (u16)(u32)GPFifo.loWatermark);
    GX_CP_REG_WRITE_U16(CP_FIFO_BASEH, (u16)(TOPHYSICAL(GPFifo.base) >> 16));
    GX_CP_REG_WRITE_U16(CP_FIFO_TOPH, (u16)(TOPHYSICAL(GPFifo.end) >> 16));
    GX_CP_REG_WRITE_U16(CP_FIFO_COUNTH, (u16)(GPFifo.count >> 16));
    GX_CP_REG_WRITE_U16(CP_FIFO_WPTRH, (u16)(TOPHYSICAL(GPFifo.writePtr) >> 16));
    GX_CP_REG_WRITE_U16(CP_FIFO_RPTRH, (u16)(TOPHYSICAL(GPFifo.readPtr) >> 16));
    GX_CP_REG_WRITE_U16(CP_FIFO_HICNTH, (u16)((u32)GPFifo.hiWatermark >> 16));
    GX_CP_REG_WRITE_U16(CP_FIFO_LOCNTH, (u16)((u32)GPFifo.loWatermark >> 16));

    PPCSync();

    if (CPGPLinkCheck()) {
        CPGPLinked[0] = 1;
        GPFifo.bind_cpu = GX_TRUE;
        __GXWriteFifoIntEnable(GX_TRUE, GX_FALSE);
        __GXFifoLink(GX_TRUE);
    } else {
        CPGPLinked[0] = 0;
        GPFifo.bind_cpu = GX_FALSE;
        __GXWriteFifoIntEnable(GX_FALSE, GX_FALSE);
        __GXFifoLink(GX_FALSE);
    }

    stbtmp = gxdt->cpCtrlReg;
    CP_CTRL_SET_FIFOBRK(stbtmp, 0);
    CP_CTRL_SET_FIFOBRKINT(stbtmp, 0);
    GX_CP_REG_WRITE_U16(CP_ENABLE, (u16)stbtmp);
    GX_CP_REG_WRITE_U16(CP_ENABLE, (u16)gxdt->cpCtrlReg);
    __GXWriteFifoIntReset(GX_TRUE, GX_TRUE);
    __GXFifoReadEnable();
    OSRestoreInterrupts(en);
}

void __GXSaveFifo(void) {
    u32 reg;
    u32 temp;
    BOOL en = OSDisableInterrupts();

    if (CPUFifoReady) {
        reg = GX_PI_REG_READ_U32(PI_REG_CPWRT);
        CPUFifo.writePtr = OSPhysicalToCached(GX_BITGET(reg, 3, 24) << 5);
        CPUFifo.wrap = (u8)GX_BITGET(reg, 2, 1);
    }

    if (GPFifoReady[0]) {
        temp = (u32)GX_CP_REG_READ_U16(CP_FIFO_RPTRH) << 16;
        temp |= (u32)GX_CP_REG_READ_U16(CP_FIFO_RPTRL);
        GPFifo.readPtr = (void*)OSPhysicalToCached(temp);

        temp = (u32)GX_CP_REG_READ_U16(CP_FIFO_COUNTH) << 16;
        temp |= (u32)GX_CP_REG_READ_U16(CP_FIFO_COUNTL);
        GPFifo.count = temp;
    }

    if (CPGPLinked[0]) {
        CPUFifo.readPtr = GPFifo.readPtr;
        CPUFifo.count = GPFifo.count;
        GPFifo.writePtr = CPUFifo.writePtr;
        GPFifo.wrap = CPUFifo.wrap;
    } else if (CPUFifoReady) {
        CPUFifo.count = (s32)CPUFifo.writePtr - (s32)CPUFifo.readPtr;
        if ((s32)CPUFifo.count < 0) {
            CPUFifo.count += CPUFifo.size;
        }
    }

    OSRestoreInterrupts(en);
}

GXBool __GXIsGPFifoReady(void) {
    return GPFifoReady[0];
}

GXBool GXGetCPUFifo(GXFifoObj* fifo) {
    GXFifoObjImpl* realFifo = (GXFifoObjImpl*)fifo;

    if (!CPUFifoReady) {
        return GX_FALSE;
    }

    __GXSaveFifo();
    *realFifo = CPUFifo;
    return GX_TRUE;
}

void GXGetFifoPtrs(const GXFifoObj* fifo, void** readPtr, void** writePtr) {
    const GXFifoObjImpl* realFifo = (const GXFifoObjImpl*)fifo;

    *readPtr = realFifo->readPtr;
    *writePtr = realFifo->writePtr;
}

GXBreakPtCallback GXSetBreakPtCallback(GXBreakPtCallback cb) {
    GXBreakPtCallback oldcb = BreakPointCB;
    BOOL enabled = OSDisableInterrupts();

    BreakPointCB = cb;
    OSRestoreInterrupts(enabled);
    return oldcb;
}

void GXEnableBreakPt(void* break_pt) {
    BOOL enabled = OSDisableInterrupts();

    __GXFifoReadDisable();
    GX_CP_REG_WRITE_U16(CP_FIFO_BRKL, (u16)TOPHYSICAL(break_pt));
    GX_CP_REG_WRITE_U16(CP_FIFO_BRKH, (u16)(TOPHYSICAL(break_pt) >> 16));
    CP_CTRL_SET_FIFOBRK(gxdt->cpCtrlReg, 0);
    CP_CTRL_SET_FIFOBRKINT(gxdt->cpCtrlReg, 0);
    GX_CP_REG_WRITE_U16(CP_ENABLE, (u16)gxdt->cpCtrlReg);
    CP_CTRL_SET_FIFOBRK(gxdt->cpCtrlReg, 1);
    CP_CTRL_SET_FIFOBRKINT(gxdt->cpCtrlReg, 1);
    GX_CP_REG_WRITE_U16(CP_ENABLE, (u16)gxdt->cpCtrlReg);
    __GXCurrentBP = break_pt;
    __GXFifoReadEnable();
    OSRestoreInterrupts(enabled);
}

void __GXFifoInit(void) {
    __OSSetInterruptHandler(OS_INTR_PI_CP, (OSInterruptHandler)GXCPInterruptHandler);
    __OSUnmaskInterrupts(OS_INTR_MASK(OS_INTR_PI_CP));
    __GXCurrentThread = OSGetCurrentThread();
    GXOverflowSuspendInProgress = FALSE;
    memset(&CPUFifo, 0, sizeof(GXFifoObjImpl));
    memset(&GPFifo, 0, sizeof(GXFifoObjImpl));
    CPUFifoReady = 0;
    GPFifoReady[0] = 0;
}

void __GXCleanGPFifo(void) {
    BOOL enabled;
    u32 reg;

    if (!GPFifoReady[0]) {
        return;
    }

    enabled = OSDisableInterrupts();
    __GXFifoReadDisable();
    __GXWriteFifoIntEnable(GX_FALSE, GX_FALSE);
    GPFifo.readPtr = GPFifo.writePtr;
    GPFifo.count = 0;
    GX_CP_REG_WRITE_U16(CP_FIFO_COUNTL, (u16)GPFifo.count);
    GX_CP_REG_WRITE_U16(CP_FIFO_WPTRL, (u16)TOPHYSICAL(GPFifo.writePtr));
    GX_CP_REG_WRITE_U16(CP_FIFO_RPTRL, (u16)TOPHYSICAL(GPFifo.readPtr));
    GX_CP_REG_WRITE_U16(CP_FIFO_COUNTH, (u16)(GPFifo.count >> 16));
    GX_CP_REG_WRITE_U16(CP_FIFO_WPTRH, (u16)(TOPHYSICAL(GPFifo.writePtr) >> 16));
    GX_CP_REG_WRITE_U16(CP_FIFO_RPTRH, (u16)(TOPHYSICAL(GPFifo.readPtr) >> 16));

    PPCSync();

    if (CPGPLinked[0]) {
        CPUFifo.readPtr = GPFifo.readPtr;
        CPUFifo.writePtr = GPFifo.writePtr;
        CPUFifo.count = GPFifo.count;
        reg = 0;
        reg = GX_BITSET(reg, 3, 24, TOPHYSICAL(CPUFifo.writePtr) >> 5);
        GX_PI_REG_WRITE_U32(PI_REG_CPWRT, reg);
        __GXWriteFifoIntEnable(GX_TRUE, GX_FALSE);
        __GXFifoLink(GX_TRUE);
    }

    CP_CTRL_SET_FIFOBRK(gxdt->cpCtrlReg, 0);
    CP_CTRL_SET_FIFOBRKINT(gxdt->cpCtrlReg, 0);
    GX_CP_REG_WRITE_U16(CP_ENABLE, (u16)gxdt->cpCtrlReg);
    __GXCurrentBP = NULL;
    __GXWriteFifoIntReset(GX_TRUE, GX_TRUE);
    __GXFifoReadEnable();
    OSRestoreInterrupts(enabled);
}
