#include <revolution/GX.h>
#include <revolution/OS.h>
#include <revolution/base/PPCArch.h>
#include <revolution/vi/vi.h>
#include <revolution/vi/vitypes.h>

#define GX_TMEM_LO 0x00000
#define GX_TMEM_HI 0x80000
#define GX_32k 0x08000
#define GX_8k 0x02000

#define GX_TEXCACHE_32K 0
#define GX_TLUT_256 16
#define GX_TLUT_1K 64

#define GX_MT_XF_FLUSH 1
#define GX_TO_ZERO 0
#define GX_GM_1_0 0
#define GX_COPY_PROGRESSIVE 0
#define GX_READ_FF 1

#define CP_ENABLE 0x01
#define CP_CLR 0x02
#define CP_MEMPERF_SEL 0x03
#define CP_VAT_B 0x08
#define CP_STAT_SEL 0x02

#define CP_REG_ENABLE(fiford, fifobrk, ovflint, unflint, wrptrinc, fifobrkint) \
    (((fiford) << 0) | ((fifobrk) << 1) | ((ovflint) << 2) | ((unflint) << 3) | \
     ((wrptrinc) << 4) | ((fifobrkint) << 5))

#define CP_REG_CLR(ovflint, unflint, perfcnt) \
    (((ovflint) << 0) | ((unflint) << 1) | ((perfcnt) << 2))

#define CP_REG_MEMPERFSEL(perf) (perf)

#define MEM_PE_REQCOUNTL_IDX 0x28
#define MEM_PE_REQCOUNTH_IDX 0x27

#define GX_CP_REG_WRITE_U16(a, d) \
    (*(volatile u16*)((volatile u16*)__cpReg + (a)) = (u16)(d))

#define GX_MEM_REG_READ_U16(a) (*(volatile u16*)((volatile u16*)__memReg + (a)))

#define GX_WRITE_U32(v) WGPIPE.i = (u32)(v)

#define GX_WRITE_CP_STRM_REG(addr, vtxfmt, data) \
    do { \
        WGPIPE.c = 0x08; \
        WGPIPE.c = (u8)(((vtxfmt)&0xF) | (((addr)&0xF) << 4)); \
        WGPIPE.i = (u32)(data); \
    } while (0)

#define GX_WRITE_XF_REG(addr, data, cnt) GX_XF_LOAD_REG(addr, data)

#define GX_WRITE_RA_REG(reg) GX_BP_LOAD_REG(reg)

#define SC_CP_VAT_REG_A_SET_BYTEDEQUANT(reg, v) \
    GX_CP_SET_VAT_GROUP0_BYTEDEQUANT(reg, v)

#define SC_CP_VAT_REG_B_SET_VCACHE_ENHANCE(reg, v) \
    ((reg) = GX_BITSET((reg), 31, 1, (v)))

#define SC_XF_ERROR_F_SET_CTEX_BUG_ENABLE(reg, v) ((reg) |= (v) << 0)
#define SC_XF_ERROR_F_SET_TFAN4_BUG_ENABLE(reg, v) ((reg) |= (v) << 1)
#define SC_XF_ERROR_F_SET_TFAN16_BUG_ENABLE(reg, v) ((reg) |= (v) << 2)
#define SC_XF_ERROR_F_SET_DUALTRAN_REG_ENABLE(reg, v) ((reg) |= (v) << 3)
#define SC_XF_ERROR_F_SET_BYPASS_BUG_ENABLE(reg, v) ((reg) |= (v) << 4)
#define SC_XF_ERROR_F_SET_FAST_MATRIX_ENABLE(reg, v) ((reg) |= (v) << 5)

#define SC_XF_DUALTEXTRAN_F_SET_DUALTEXTRAN_ENABLE(reg, v) ((reg) |= (v) << 0)

#define SC_PE_CHICKEN_SET_PIWR(reg, v) ((reg) = GX_BITSET(reg, 31, 1, v))
#define SC_PE_CHICKEN_SET_TXCPY_FMT(reg, v) ((reg) = GX_BITSET(reg, 30, 1, v))
#define SC_PE_CHICKEN_SET_TXCPY_CCV(reg, v) ((reg) = GX_BITSET(reg, 29, 1, v))
#define SC_PE_CHICKEN_SET_BLENDOP(reg, v) ((reg) = GX_BITSET(reg, 28, 1, v))
#define SC_PE_CHICKEN_SET_RID(reg, v) ((reg) = GX_BITSET(reg, 0, 8, v))

#define SC_GEN_MODE_SET_RID(reg, v) GX_BP_SET_OPCODE(reg, v)

#define SC_BP_IMASK_SET_RID(reg, v) GX_BP_SET_OPCODE(reg, v)
#define SC_BP_IMASK_SET_IMASK(reg, v) GX_BP_SET_INDIMASK_IMASK(reg, v)

#define SC_SU_LPSIZE_SET_RID(reg, v) GX_BP_SET_OPCODE(reg, v)
#define SC_TEV_COLOR_ENV_SET_RID(reg, v) GX_BP_SET_OPCODE(reg, v)
#define SC_TEV_ALPHA_ENV_SET_RID(reg, v) GX_BP_SET_OPCODE(reg, v)
#define SC_TEV_KSEL_SET_RID(reg, v) GX_BP_SET_OPCODE(reg, v)
#define SC_RAS1_TREF_SET_RID(reg, v) GX_BP_SET_OPCODE(reg, v)
#define SC_RAS1_IREF_SET_RID(reg, v) GX_BP_SET_OPCODE(reg, v)
#define SC_SU_TS0_SET_RID(reg, v) GX_BP_SET_OPCODE(reg, v)
#define SC_SU_TS1_SET_RID(reg, v) GX_BP_SET_OPCODE(reg, v)
#define SC_SU_SCIS0_SET_RID(reg, v) GX_BP_SET_OPCODE(reg, v)
#define SC_SU_SCIS1_SET_RID(reg, v) GX_BP_SET_OPCODE(reg, v)
#define SC_PE_CMODE0_SET_RID(reg, v) GX_BP_SET_OPCODE(reg, v)
#define SC_PE_CMODE1_SET_RID(reg, v) GX_BP_SET_OPCODE(reg, v)
#define SC_PE_ZMODE_SET_RID(reg, v) GX_BP_SET_OPCODE(reg, v)
#define SC_PE_CONTROL_SET_RID(reg, v) GX_BP_SET_OPCODE(reg, v)
#define SC_PE_COPY_CMD_SET_GAMMA(reg, v) ((reg) = GX_BITSET(reg, 16, 2, v))

#define SC_CP_STAT_SEL_REG_SET_STALLPERF_SEL(reg, v) \
    ((reg) = GX_BITSET(reg, 24, 4, v))

#define STALLPERF_ZERO 0
#define MEMPERF_ZERO 0

#define TX_REFRESH(interval, enable, rid)                                       \
    ((u32)((interval) << 16) | ((u32)(enable) << 8) | (u32)(rid))

#define PE_REFRESH(interval, enable, rid)                                       \
    ((u32)((interval) << 16) | ((u32)(enable) << 8) | (u32)(rid))

#define XF_PERF0_F(a, b, c, d) 0
#define SU_PERF(a, b, c, d, e, f, g, h, i, rid) ((u32)((u32)(rid) << 24))
#define RAS_PERF(a, b, c, d, e, rid) ((u32)((u32)(rid) << 24))
#define TX_PERFMODE(a, b, c, d, rid) ((u32)((u32)(rid) << 24))

static GXFifoObj FifoObj;
static GXData gxData;
GXData* const __GXData = &gxData;

volatile void* __piReg;
volatile void* __cpReg;
volatile u16* __peReg;
volatile u16* __memReg;

static inline u32 GX_MEM_COUNTER_READ_U32(u32 name) {
    u32 ctrH0, ctrH1, ctrL;
    (void)name;
    ctrH0 = GX_MEM_REG_READ_U16(MEM_PE_REQCOUNTH_IDX);
    do {
        ctrH1 = ctrH0;
        ctrL = GX_MEM_REG_READ_U16(MEM_PE_REQCOUNTL_IDX);
        ctrH0 = GX_MEM_REG_READ_U16(MEM_PE_REQCOUNTH_IDX);
    } while (ctrH0 != ctrH1);
    return (ctrH0 << 16) | ctrL;
}

static void EnableWriteGatherPipe(void);

static GXTlutRegion* __GXDefaultTlutRegionCallback(u32);
static GXTexRegion* __GXDefaultTexRegionCallback(const GXTexObj*, GXTexMapID);

/* Retail .data pool head: version[0x48] then align-32 DefaultTexData at +0x60.
   __GXInitGX bases r28 here so +0x60/+0x80/+0x150 hit tex/VAT/proj. */
static char s_GXVersionStr[0x48] =
    "<< RVL_SDK - GX \trelease build: Feb 27 2009 10:04:13 (0x4302_145) >>";
const char* __GXVersion = s_GXVersionStr;

static BOOL __GXShutdown(BOOL, u32);

static u16 DefaultTexData[] __attribute__((aligned(32))) = {0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
                                                            0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF};

static GXVtxAttrFmtList GXDefaultVATList[] = {
    {GX_VA_POS, GX_POS_XYZ, GX_F32, 0},          {GX_VA_NRM, GX_NRM_XYZ, GX_F32, 0}, {GX_VA_CLR0, GX_CLR_RGBA, GX_RGBA8, 0},
    {GX_VA_CLR1, GX_CLR_RGBA, GX_RGBA8, 0},      {GX_VA_TEX0, GX_TEX_ST, GX_F32, 0}, {GX_VA_TEX1, GX_TEX_ST, GX_F32, 0},
    {GX_VA_TEX2, GX_TEX_ST, GX_F32, 0},          {GX_VA_TEX3, GX_TEX_ST, GX_F32, 0}, {GX_VA_TEX4, GX_TEX_ST, GX_F32, 0},
    {GX_VA_TEX5, GX_TEX_ST, GX_F32, 0},          {GX_VA_TEX6, GX_TEX_ST, GX_F32, 0}, {GX_VA_TEX7, GX_TEX_ST, GX_F32, 0},
    {GX_VA_NULL, (GXCompCnt)0, (GXCompType)0, 0}};

/* Retail object size 0x20 (trailing pad word). */
static f32 GXDefaultProjData[8] = {1.0f, 0.0f, 1.0f, 0.0f, -1.0f, -2.0f, 0.0f, 0.0f};

static u32 GXTexRegionAddrTable[] = {
    GX_TMEM_LO + 0x00000, GX_TMEM_LO + 0x10000, GX_TMEM_LO + 0x20000, GX_TMEM_LO + 0x30000, GX_TMEM_LO + 0x40000, GX_TMEM_LO + 0x50000,
    GX_TMEM_LO + 0x60000, GX_TMEM_LO + 0x70000, GX_TMEM_LO + 0x08000, GX_TMEM_LO + 0x18000, GX_TMEM_LO + 0x28000, GX_TMEM_LO + 0x38000,
    GX_TMEM_LO + 0x48000, GX_TMEM_LO + 0x58000, GX_TMEM_LO + 0x68000, GX_TMEM_LO + 0x78000, GX_TMEM_LO + 0x00000, GX_TMEM_HI + 0x10000,
    GX_TMEM_LO + 0x20000, GX_TMEM_HI + 0x30000, GX_TMEM_LO + 0x40000, GX_TMEM_HI + 0x18000, GX_TMEM_LO + 0x60000, GX_TMEM_HI + 0x38000,
    GX_TMEM_HI + 0x00000, GX_TMEM_LO + 0x10000, GX_TMEM_HI + 0x20000, GX_TMEM_LO + 0x30000, GX_TMEM_HI + 0x08000, GX_TMEM_LO + 0x50000,
    GX_TMEM_HI + 0x28000, GX_TMEM_LO + 0x70000, GX_TMEM_LO + 0x00000, GX_TMEM_HI + 0x10000, GX_TMEM_LO + 0x20000, GX_TMEM_HI + 0x30000,
    GX_TMEM_LO + 0x40000, GX_TMEM_HI + 0x10000, GX_TMEM_LO + 0x60000, GX_TMEM_HI + 0x30000, GX_TMEM_HI + 0x00000, GX_TMEM_LO + 0x10000,
    GX_TMEM_HI + 0x20000, GX_TMEM_LO + 0x30000, GX_TMEM_HI + 0x00000, GX_TMEM_LO + 0x50000, GX_TMEM_HI + 0x20000, GX_TMEM_LO + 0x70000,
};

static OSShutdownFunctionInfo GXShutdownFuncInfo = {__GXShutdown, 127};

static void EnableWriteGatherPipe(void) {
    u32 hid2 = PPCMfhid2();
    PPCMtwpar((u32)OSUncachedToPhysical((void*)0xCC008000));
    hid2 |= 0x40000000;
    PPCMthid2(hid2);
}

static GXTexRegion* __GXDefaultTexRegionCallback(const GXTexObj* t_obj, GXTexMapID id) {
    GXTexFmt fmt;
    GXBool mm;
    fmt = GXGetTexObjFmt(t_obj);
    mm = GXGetTexObjMipMap(t_obj);
    id = (GXTexMapID)(id % 8);

    switch (fmt) {
    case GX_TF_RGBA8:
        if (mm) {
            return &gxdt->TexRegions2[id];
        } else {
            return &gxdt->TexRegions1[id];
        }
        break;
    case GX_TF_C4:
    case GX_TF_C8:
    case GX_TF_C14X2:
        return &gxdt->TexRegions0[id];
        break;
    default:
        if (mm) {
            return &gxdt->TexRegions1[id];
        } else {
            return &gxdt->TexRegions0[id];
        }
        break;
    }
}

static GXTlutRegion* __GXDefaultTlutRegionCallback(u32 idx) {
    if (idx >= 20) {
        return 0;
    }

    return &gxdt->TlutRegions[idx];
}

static BOOL __GXShutdown(BOOL final, u32 event) {
    static u32 peCount;
    static s64 time;
    static u32 calledOnce = 0;
    u32 reg, peCountNew;
    s64 timeNew;

    if (!final) {
        if (!calledOnce) {
            peCount = GX_MEM_COUNTER_READ_U32(0);
            time = OSGetTime();
            calledOnce = 1;
            return FALSE;
        }

        timeNew = OSGetTime();
        peCountNew = GX_MEM_COUNTER_READ_U32(0);

        if (timeNew - time < 10) {
            return FALSE;
        }

        if (peCountNew != peCount) {
            peCount = peCountNew;
            time = timeNew;
            return FALSE;
        }
    } else {
        GXSetBreakPtCallback(NULL);
        GXSetDrawSyncCallback(NULL);
        GXSetDrawDoneCallback(NULL);

        GX_WRITE_U32(0);
        GX_WRITE_U32(0);
        GX_WRITE_U32(0);
        GX_WRITE_U32(0);
        GX_WRITE_U32(0);
        GX_WRITE_U32(0);
        GX_WRITE_U32(0);
        GX_WRITE_U32(0);
        PPCSync();

        reg = CP_REG_ENABLE(0, 0, 0, 0, 0, 0);
        GX_CP_REG_WRITE_U16(CP_ENABLE, (u16)reg);
        reg = CP_REG_CLR(1, 1, 0);
        GX_CP_REG_WRITE_U16(CP_CLR, (u16)reg);
        gxdt->BYTE_0x5FA = 1;
        __GXAbort();
    }

    return TRUE;
}

void __GXInitRevisionBits(void) {
    u32 i;

    for (i = 0; i < GX_MAX_VTXFMT; i++) {
        SC_CP_VAT_REG_A_SET_BYTEDEQUANT(gxdt->vatA[i], 1);
        SC_CP_VAT_REG_B_SET_VCACHE_ENHANCE(gxdt->vatB[i], 1);
        GX_WRITE_CP_STRM_REG(CP_VAT_B, (s32)i, gxdt->vatB[i]);
    }

    {
        u32 reg1 = 0;
        u32 reg2 = 0;
        SC_XF_ERROR_F_SET_CTEX_BUG_ENABLE(reg1, 1);
        SC_XF_ERROR_F_SET_TFAN4_BUG_ENABLE(reg1, 1);
        SC_XF_ERROR_F_SET_TFAN16_BUG_ENABLE(reg1, 1);
        SC_XF_ERROR_F_SET_DUALTRAN_REG_ENABLE(reg1, 1);
        SC_XF_ERROR_F_SET_BYPASS_BUG_ENABLE(reg1, 1);
        SC_XF_ERROR_F_SET_FAST_MATRIX_ENABLE(reg1, 1);
        GX_WRITE_XF_REG(0x1000, reg1, 0);
        SC_XF_DUALTEXTRAN_F_SET_DUALTEXTRAN_ENABLE(reg2, 1);
        GX_WRITE_XF_REG(0x1012, reg2, 0);
    }

    {
        u32 reg = 0;
        SC_PE_CHICKEN_SET_PIWR(reg, 1);
        SC_PE_CHICKEN_SET_TXCPY_FMT(reg, 1);
        SC_PE_CHICKEN_SET_TXCPY_CCV(reg, 1);
        SC_PE_CHICKEN_SET_BLENDOP(reg, 1);
        SC_PE_CHICKEN_SET_RID(reg, 0x58);
        GX_WRITE_RA_REG(reg);
    }
}

GXFifoObj* GXInit(void* base, u32 size) {
    static u32 shutdownFuncRegistered = 0;
    u32 i;

    OSRegisterVersion(__GXVersion);
    gxdt->dlistActive = FALSE;
    gxdt->dlistSave = TRUE;
    gxdt->BYTE_0x5FA = TRUE;
    gxdt->tcsManEnab = 0;
    gxdt->tevTcEnab = 0;
    GXSetMisc(GX_MT_XF_FLUSH, 0);
    __piReg = (void*)OSPhysicalToUncached(0x0c003000);
    __cpReg = (void*)OSPhysicalToUncached(0x0c000000);
    __peReg = (volatile u16*)OSPhysicalToUncached(0x0c001000);
    __memReg = (volatile u16*)OSPhysicalToUncached(0x0c004000);

    __GXFifoInit();
    GXInitFifoBase(&FifoObj, base, size);
    GXSetCPUFifo(&FifoObj);
    GXSetGPFifo(&FifoObj);

    if (!shutdownFuncRegistered) {
        OSRegisterShutdownFunction(&GXShutdownFuncInfo);
        shutdownFuncRegistered = 1;
    }

    __GXPEInit();
    EnableWriteGatherPipe();

    gxdt->genMode = 0;
    SC_GEN_MODE_SET_RID(gxdt->genMode, 0);
    gxdt->ind_imask = 255;
    SC_BP_IMASK_SET_RID(gxdt->ind_imask, 0xF);
    gxdt->linePtWidth = 0;
    SC_SU_LPSIZE_SET_RID(gxdt->linePtWidth, 0x22);

    for (i = 0; i < 16; i++) {
        gxdt->tevc[i] = 0;
        gxdt->teva[i] = 0;
        gxdt->tref[i / 2] = 0;
        gxdt->texmapId[i] = GX_TEXMAP_NULL;
        SC_TEV_COLOR_ENV_SET_RID(gxdt->tevc[i], (0xC0 + i * 2));
        SC_TEV_ALPHA_ENV_SET_RID(gxdt->teva[i], (0xC1 + i * 2));
        SC_TEV_KSEL_SET_RID(gxdt->tevKsel[i / 2], (0xF6 + i / 2));
        SC_RAS1_TREF_SET_RID(gxdt->tref[i / 2], (0x28 + i / 2));
    }

    gxdt->ras1_iref = 0;
    SC_RAS1_IREF_SET_RID(gxdt->ras1_iref, 0x27);

    for (i = 0; i < 8; i++) {
        gxdt->suTs0[i] = 0;
        gxdt->suTs1[i] = 0;
        SC_SU_TS0_SET_RID(gxdt->suTs0[i], 0x30 + i * 2);
        SC_SU_TS1_SET_RID(gxdt->suTs1[i], 0x31 + i * 2);
    }

    SC_SU_SCIS0_SET_RID(gxdt->scissorTL, 0x20);
    SC_SU_SCIS1_SET_RID(gxdt->scissorBR, 0x21);
    SC_PE_CMODE0_SET_RID(gxdt->blendMode, 0x41);
    SC_PE_CMODE1_SET_RID(gxdt->dstAlpha, 0x42);
    SC_PE_ZMODE_SET_RID(gxdt->zMode, 0x40);
    SC_PE_CONTROL_SET_RID(gxdt->zControl, 0x43);
    SC_PE_COPY_CMD_SET_GAMMA(gxdt->cpTex, 0);

    gxdt->scaleZ = (f32)(0xFFFFFF + 1);
    gxdt->offsetZ = 0.0f;
    gxdt->gxDirtyFlags = 0;
    gxdt->vatDirtyFlags = 0;

    {
        u32 reg;
        u32 freqBase;
        freqBase = OS_BUS_CLOCK_SPEED / 500;
        __GXFlushTextureState();
        reg = TX_REFRESH(freqBase / 2048, 1, 0x69);
        GX_WRITE_RA_REG(reg);
        __GXFlushTextureState();

        reg = PE_REFRESH(freqBase / 4224, 1, 0x46);
        GX_WRITE_RA_REG(reg);
    }

    __GXInitRevisionBits();

    for (i = 0; i < 8; i++) {
        GXInitTexCacheRegion(&gxdt->TexRegions0[i], GX_FALSE, GXTexRegionAddrTable[i], GX_TEXCACHE_32K, GXTexRegionAddrTable[i + 8], GX_TEXCACHE_32K);
        GXInitTexCacheRegion(&gxdt->TexRegions1[i], GX_FALSE, GXTexRegionAddrTable[i + 16], GX_TEXCACHE_32K, GXTexRegionAddrTable[i + 24],
                             GX_TEXCACHE_32K);
        GXInitTexCacheRegion(&gxdt->TexRegions2[i], GX_TRUE, GXTexRegionAddrTable[i + 32], GX_TEXCACHE_32K, GXTexRegionAddrTable[i + 40],
                             GX_TEXCACHE_32K);
    }

    for (i = 0; i < 16; i++) {
        GXInitTlutRegion(&gxdt->TlutRegions[i], (GX_TMEM_HI + GX_32k * 8 + GX_8k * i), GX_TLUT_256);
    }

    for (i = 0; i < 4; i++) {
        GXInitTlutRegion(&gxdt->TlutRegions[i + 16], (GX_TMEM_HI + GX_32k * 8 + GX_8k * 16 + GX_32k * i), GX_TLUT_1K);
    }

    {
        u32 reg = CP_REG_MEMPERFSEL(MEMPERF_ZERO);
        GX_CP_REG_WRITE_U16(CP_MEMPERF_SEL, reg);

        SC_CP_STAT_SEL_REG_SET_STALLPERF_SEL(gxdt->perfSel, STALLPERF_ZERO);
        GX_WRITE_CP_STRM_REG(CP_STAT_SEL, 0, gxdt->perfSel);
        reg = XF_PERF0_F(0, 0, 0, 0);
        GX_WRITE_XF_REG(0x1006, reg, 0);
        reg = SU_PERF(0, 0, 0, 0, 0, 0, 0, 0, 0, 0x23);
        GX_WRITE_RA_REG(reg);
        reg = RAS_PERF(0, 0, 0, 0, 0, 0x24);
        GX_WRITE_RA_REG(reg);
        reg = TX_PERFMODE(0, 0, 0, 0, 0x67);
        GX_WRITE_RA_REG(reg);
    }

    __GXSetIndirectMask(0);
    __GXSetTmemConfig(2);
    __GXInitGX();
    return &FifoObj;
}

void __GXInitGX() {
    GXRenderModeObj* rmode;
    GXTexObj tex_obj;
    Mtx identity_mtx;
    GXColor clear = {64, 64, 64, 255};
    GXColor black = {0, 0, 0, 0};
    GXColor white = {255, 255, 255, 255};
    u32 i;

    switch (VIGetTvFormat()) {
    case VI_TVFORMAT_NTSC:
        rmode = &GXNtsc480IntDf;
        break;
    case VI_TVFORMAT_PAL:
        rmode = &GXPal528IntDf;
        break;
    case VI_TVFORMAT_EURGB60:
        rmode = &GXEurgb60Hz480IntDf;
        break;
    case VI_TVFORMAT_MPAL:
        rmode = &GXMpal480IntDf;
        break;
    default:
        rmode = &GXNtsc480IntDf;
        break;
    }

    GXSetCopyClear(clear, 0xFFFFFF);
    GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_IDENTITY, 0, 0x7D);
    GXSetTexCoordGen2(GX_TEXCOORD1, GX_TG_MTX2x4, GX_TG_TEX1, GX_IDENTITY, 0, 0x7D);
    GXSetTexCoordGen2(GX_TEXCOORD2, GX_TG_MTX2x4, GX_TG_TEX2, GX_IDENTITY, 0, 0x7D);
    GXSetTexCoordGen2(GX_TEXCOORD3, GX_TG_MTX2x4, GX_TG_TEX3, GX_IDENTITY, 0, 0x7D);
    GXSetTexCoordGen2(GX_TEXCOORD4, GX_TG_MTX2x4, GX_TG_TEX4, GX_IDENTITY, 0, 0x7D);
    GXSetTexCoordGen2(GX_TEXCOORD5, GX_TG_MTX2x4, GX_TG_TEX5, GX_IDENTITY, 0, 0x7D);
    GXSetTexCoordGen2(GX_TEXCOORD6, GX_TG_MTX2x4, GX_TG_TEX6, GX_IDENTITY, 0, 0x7D);
    GXSetTexCoordGen2(GX_TEXCOORD7, GX_TG_MTX2x4, GX_TG_TEX7, GX_IDENTITY, 0, 0x7D);
    GXSetNumTexGens(1);
    GXClearVtxDesc();
    GXInvalidateVtxCache();

    for (i = 9; i <= 24; i++) {
        GXSetArray((GXAttr)i, (void*)gxdt, 0);
    }

    for (i = 0; i < 8; i++) {
        GXSetVtxAttrFmtv((GXVtxFmt)i, GXDefaultVATList);
    }

    GXSetLineWidth(6, GX_TO_ZERO);
    GXSetPointSize(6, GX_TO_ZERO);
    GXEnableTexOffsets(GX_TEXCOORD0, GX_DISABLE, GX_DISABLE);
    GXEnableTexOffsets(GX_TEXCOORD1, GX_DISABLE, GX_DISABLE);
    GXEnableTexOffsets(GX_TEXCOORD2, GX_DISABLE, GX_DISABLE);
    GXEnableTexOffsets(GX_TEXCOORD3, GX_DISABLE, GX_DISABLE);
    GXEnableTexOffsets(GX_TEXCOORD4, GX_DISABLE, GX_DISABLE);
    GXEnableTexOffsets(GX_TEXCOORD5, GX_DISABLE, GX_DISABLE);
    GXEnableTexOffsets(GX_TEXCOORD6, GX_DISABLE, GX_DISABLE);
    GXEnableTexOffsets(GX_TEXCOORD7, GX_DISABLE, GX_DISABLE);

    identity_mtx[0][0] = 1.0f;
    identity_mtx[0][1] = 0.0f;
    identity_mtx[0][2] = 0.0f;
    identity_mtx[0][3] = 0.0f;
    identity_mtx[1][0] = 0.0f;
    identity_mtx[1][1] = 1.0f;
    identity_mtx[1][2] = 0.0f;
    identity_mtx[1][3] = 0.0f;
    identity_mtx[2][0] = 0.0f;
    identity_mtx[2][1] = 0.0f;
    identity_mtx[2][2] = 1.0f;
    identity_mtx[2][3] = 0.0f;

    GXLoadPosMtxImm(identity_mtx, GX_PNMTX0);
    GXLoadNrmMtxImm(identity_mtx, GX_PNMTX0);
    GXSetCurrentMtx(GX_PNMTX0);
    GXLoadTexMtxImm(identity_mtx, GX_IDENTITY, GX_MTX_3x4);
    GXLoadTexMtxImm(identity_mtx, GX_PTIDENTITY, GX_MTX_3x4);
    GXSetViewport(0.0f, 0.0f, (f32)rmode->fbWidth, (f32)rmode->xfbHeight, 0.0f, 1.0f);
    GXSetProjectionv(GXDefaultProjData);

    __GXSetGenMode();
    GXSetCoPlanar(GX_DISABLE);
    GXSetCullMode(GX_CULL_BACK);
    GXSetClipMode(GX_CLIP_ENABLE);
    GXSetScissor(0, 0, rmode->fbWidth, rmode->efbHeight);
    GXSetScissorBoxOffset(0, 0);
    GXSetNumChans(0);
    GXSetChanCtrl(GX_COLOR0A0, GX_DISABLE, GX_SRC_REG, GX_SRC_VTX, GX_LIGHT_NULL, GX_DF_NONE, GX_AF_NONE);
    GXSetChanAmbColor(GX_COLOR0A0, black);
    GXSetChanMatColor(GX_COLOR0A0, white);
    GXSetChanCtrl(GX_COLOR1A1, GX_DISABLE, GX_SRC_REG, GX_SRC_VTX, GX_LIGHT_NULL, GX_DF_NONE, GX_AF_NONE);
    GXSetChanAmbColor(GX_COLOR1A1, black);
    GXSetChanMatColor(GX_COLOR1A1, white);
    GXInvalidateTexAll();

    GXSetTexRegionCallback(__GXDefaultTexRegionCallback);
    GXSetTlutRegionCallback(__GXDefaultTlutRegionCallback);

    GXInitTexObj(&tex_obj, DefaultTexData, 4, 4, GX_TF_IA8, GX_CLAMP, GX_CLAMP, GX_FALSE);
    GXLoadTexObj(&tex_obj, GX_TEXMAP0);
    GXLoadTexObj(&tex_obj, GX_TEXMAP1);
    GXLoadTexObj(&tex_obj, GX_TEXMAP2);
    GXLoadTexObj(&tex_obj, GX_TEXMAP3);
    GXLoadTexObj(&tex_obj, GX_TEXMAP4);
    GXLoadTexObj(&tex_obj, GX_TEXMAP5);
    GXLoadTexObj(&tex_obj, GX_TEXMAP6);
    GXLoadTexObj(&tex_obj, GX_TEXMAP7);

    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR0A0);
    GXSetTevOrder(GX_TEVSTAGE1, GX_TEXCOORD1, GX_TEXMAP1, GX_COLOR0A0);
    GXSetTevOrder(GX_TEVSTAGE2, GX_TEXCOORD2, GX_TEXMAP2, GX_COLOR0A0);
    GXSetTevOrder(GX_TEVSTAGE3, GX_TEXCOORD3, GX_TEXMAP3, GX_COLOR0A0);
    GXSetTevOrder(GX_TEVSTAGE4, GX_TEXCOORD4, GX_TEXMAP4, GX_COLOR0A0);
    GXSetTevOrder(GX_TEVSTAGE5, GX_TEXCOORD5, GX_TEXMAP5, GX_COLOR0A0);
    GXSetTevOrder(GX_TEVSTAGE6, GX_TEXCOORD6, GX_TEXMAP6, GX_COLOR0A0);
    GXSetTevOrder(GX_TEVSTAGE7, GX_TEXCOORD7, GX_TEXMAP7, GX_COLOR0A0);
    GXSetTevOrder(GX_TEVSTAGE8, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR_NULL);
    GXSetTevOrder(GX_TEVSTAGE9, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR_NULL);
    GXSetTevOrder(GX_TEVSTAGE10, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR_NULL);
    GXSetTevOrder(GX_TEVSTAGE11, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR_NULL);
    GXSetTevOrder(GX_TEVSTAGE12, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR_NULL);
    GXSetTevOrder(GX_TEVSTAGE13, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR_NULL);
    GXSetTevOrder(GX_TEVSTAGE14, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR_NULL);
    GXSetTevOrder(GX_TEVSTAGE15, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR_NULL);
    GXSetNumTevStages(1);
    GXSetTevOp(GX_TEVSTAGE0, GX_REPLACE);
    GXSetAlphaCompare(GX_ALWAYS, 0, GX_AOP_AND, GX_ALWAYS, 0);
    GXSetZTexture(GX_ZT_DISABLE, GX_TF_Z8, 0);

    for (i = 0; i < 16; i++) {
        GXSetTevKColorSel((GXTevStageID)i, GX_TEV_KCSEL_1_4);
        GXSetTevKAlphaSel((GXTevStageID)i, GX_TEV_KASEL_1);
        GXSetTevSwapMode((GXTevStageID)i, GX_TEV_SWAP0, GX_TEV_SWAP0);
    }

    GXSetTevSwapModeTable(GX_TEV_SWAP0, GX_CH_RED, GX_CH_GREEN, GX_CH_BLUE, GX_CH_ALPHA);
    GXSetTevSwapModeTable(GX_TEV_SWAP1, GX_CH_RED, GX_CH_RED, GX_CH_RED, GX_CH_ALPHA);
    GXSetTevSwapModeTable(GX_TEV_SWAP2, GX_CH_GREEN, GX_CH_GREEN, GX_CH_GREEN, GX_CH_ALPHA);
    GXSetTevSwapModeTable(GX_TEV_SWAP3, GX_CH_BLUE, GX_CH_BLUE, GX_CH_BLUE, GX_CH_ALPHA);

    for (i = 0; i < 16; i++) {
        GXSetTevDirect((GXTevStageID)i);
    }

    GXSetNumIndStages(0);
    GXSetIndTexCoordScale(GX_INDTEXSTAGE0, GX_ITS_1, GX_ITS_1);
    GXSetIndTexCoordScale(GX_INDTEXSTAGE1, GX_ITS_1, GX_ITS_1);
    GXSetIndTexCoordScale(GX_INDTEXSTAGE2, GX_ITS_1, GX_ITS_1);
    GXSetIndTexCoordScale(GX_INDTEXSTAGE3, GX_ITS_1, GX_ITS_1);

    GXSetFog(GX_FOG_NONE, black, 0.0f, 1.0f, 0.1f, 1.0f);
    GXSetFogRangeAdj(GX_DISABLE, 0, 0);
    GXSetBlendMode(GX_BM_NONE, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_CLEAR);
    GXSetColorUpdate(GX_ENABLE);
    GXSetAlphaUpdate(GX_ENABLE);
    GXSetZMode(GX_TRUE, GX_LEQUAL, GX_TRUE);
    GXSetZCompLoc(GX_TRUE);
    GXSetDither(GX_ENABLE);
    GXSetDstAlpha(GX_DISABLE, 0);
    GXSetPixelFmt(GX_PF_RGB8_Z24, GX_ZC_LINEAR);
    GXSetFieldMask(GX_ENABLE, GX_ENABLE);
    GXSetFieldMode((GXBool)(rmode->field_rendering), (GXBool)(((rmode->viHeight == 2 * rmode->xfbHeight) ? GX_ENABLE : GX_DISABLE)));
    GXSetDispCopySrc(0, 0, rmode->fbWidth, rmode->efbHeight);
    GXSetDispCopyDst(rmode->fbWidth, rmode->efbHeight);
    GXSetDispCopyYScale((f32)(rmode->xfbHeight) / (f32)(rmode->efbHeight));
    GXSetCopyClamp((GXCopyClamp)(GX_CLAMP_TOP | GX_CLAMP_BOTTOM));
    GXSetCopyFilter(rmode->aa, rmode->sample_pattern, GX_TRUE, rmode->vfilter);
    GXSetDispCopyGamma(GX_GM_1_0);
    GXSetDispCopyFrame2Field(GX_COPY_PROGRESSIVE);
    GXClearBoundingBox();

    GXPokeColorUpdate(GX_TRUE);
    GXPokeAlphaUpdate(GX_TRUE);
    GXPokeDither(GX_FALSE);
    GXPokeBlendMode(GX_BM_NONE, GX_BL_ZERO, GX_BL_ONE, GX_LO_SET);
    GXPokeAlphaMode(GX_ALWAYS, 0);
    GXPokeAlphaRead(GX_READ_FF);
    GXPokeDstAlpha(GX_DISABLE, 0);
    GXPokeZMode(GX_TRUE, GX_ALWAYS, GX_TRUE);

    GXSetGPMetric(GX_PERF0_NONE, GX_PERF1_NONE);
    GXClearGPMetric();
}