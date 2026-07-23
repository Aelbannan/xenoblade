#include <revolution/GX.h>

/* CP MMIO base (initialized in GXInit to uncached 0x0C000000). */
extern volatile void* __cpReg;

#define GX_CP_REG_WRITE_U16(idx, data)                                         \
    (*(volatile u16*)((volatile u16*)__cpReg + (idx)) = (u16)(data))

/* CP stream register address for CP_STAT_SEL (vtxfmt 0). */
#define GX_CP_REG_STAT_SEL 0x20
/* CP MMIO u16 index for memperf select (byte offset 6). */
#define GX_CP_MEMPERF_SEL 0x03
/* CP MMIO u16 index written by GXClearGPMetric (byte offset 4). */
#define GX_CP_CLEAR_METRIC 0x02

/* CP_STAT_SEL_REG stallperf_sel field (PPC bits 24-27 / LSB shift 4). */
#define GX_CP_SET_STAT_SEL_STALLPERF(reg, x)                                   \
    ((reg) = GX_BITSET((reg), 24, 4, (x)))

enum {
    STALLPERF_ZERO = 0,
    ELEMQ_FULL = 2,
    MISSQ_FULL = 3,
    MEMREQ_FULL = 4,
    VC_STATCNT7 = 5,
    VC_MISS_REP_FULL = 6,
    VC_STALL_STMBUFLOW = 7,
    VTX_CNT = 8,
    ALL_STALL = 9
};

enum {
    MEMPERF_ZERO = 0,
    DFIFO_REQ_CNT = 2,
    OBJCALL_REQ_CNT = 3,
    VCMISS_REQ_CNT = 4,
    ALL_MEMREQ_CNT = 5
};

void GXSetGPMetric(GXPerf0 perf0, GXPerf1 perf1) {
    u32 reg;

    switch (gxdt->perf0) {
    case GX_PERF0_VERTICES:
    case GX_PERF0_CLIP_VTX:
    case GX_PERF0_CLIP_CLKS:
    case GX_PERF0_XF_WAIT_IN:
    case GX_PERF0_XF_WAIT_OUT:
    case GX_PERF0_XF_XFRM_CLKS:
    case GX_PERF0_XF_LIT_CLKS:
    case GX_PERF0_XF_BOT_CLKS:
    case GX_PERF0_XF_REGLD_CLKS:
    case GX_PERF0_XF_REGRD_CLKS:
    case GX_PERF0_CLOCKS:
    case GX_PERF0_CLIP_RATIO:
        GX_XF_LOAD_REG(0x1006, 0);
        break;
    case GX_PERF0_TRIANGLES:
    case GX_PERF0_TRIANGLES_CULLED:
    case GX_PERF0_TRIANGLES_PASSED:
    case GX_PERF0_TRIANGLES_SCISSORED:
    case GX_PERF0_TRIANGLES_0TEX:
    case GX_PERF0_TRIANGLES_1TEX:
    case GX_PERF0_TRIANGLES_2TEX:
    case GX_PERF0_TRIANGLES_3TEX:
    case GX_PERF0_TRIANGLES_4TEX:
    case GX_PERF0_TRIANGLES_5TEX:
    case GX_PERF0_TRIANGLES_6TEX:
    case GX_PERF0_TRIANGLES_7TEX:
    case GX_PERF0_TRIANGLES_8TEX:
    case GX_PERF0_TRIANGLES_0CLR:
    case GX_PERF0_TRIANGLES_1CLR:
    case GX_PERF0_TRIANGLES_2CLR:
        GX_BP_LOAD_REG(0x23000000);
        break;
    case GX_PERF0_QUAD_0CVG:
    case GX_PERF0_QUAD_NON0CVG:
    case GX_PERF0_QUAD_1CVG:
    case GX_PERF0_QUAD_2CVG:
    case GX_PERF0_QUAD_3CVG:
    case GX_PERF0_QUAD_4CVG:
    case GX_PERF0_AVG_QUAD_CNT:
        GX_BP_LOAD_REG(0x24000000);
        break;
    case GX_PERF0_NONE:
    default:
        break;
    }

    switch (gxdt->perf1) {
    case GX_PERF1_TEXELS:
    case GX_PERF1_TX_IDLE:
    case GX_PERF1_TX_REGS:
    case GX_PERF1_TX_MEMSTALL:
    case GX_PERF1_TC_MISS:
    case GX_PERF1_CLOCKS:
    case GX_PERF1_TC_CHECK1_2:
    case GX_PERF1_TC_CHECK3_4:
    case GX_PERF1_TC_CHECK5_6:
    case GX_PERF1_TC_CHECK7_8:
        GX_BP_LOAD_REG(0x67000000);
        break;
    case GX_PERF1_VC_ELEMQ_FULL:
    case GX_PERF1_VC_MISSQ_FULL:
    case GX_PERF1_VC_MEMREQ_FULL:
    case GX_PERF1_VC_STATUS7:
    case GX_PERF1_VC_MISSREP_FULL:
    case GX_PERF1_VC_STREAMBUF_LOW:
    case GX_PERF1_VC_ALL_STALLS:
    case GX_PERF1_VERTICES:
        GX_CP_SET_STAT_SEL_STALLPERF(gxdt->perfSel, STALLPERF_ZERO);
        GX_CP_LOAD_REG(GX_CP_REG_STAT_SEL, gxdt->perfSel);
        break;
    case GX_PERF1_FIFO_REQ:
    case GX_PERF1_CALL_REQ:
    case GX_PERF1_VC_MISS_REQ:
    case GX_PERF1_CP_ALL_REQ:
        reg = MEMPERF_ZERO;
        GX_CP_REG_WRITE_U16(GX_CP_MEMPERF_SEL, reg);
        break;
    case GX_PERF1_NONE:
    default:
        break;
    }

    gxdt->perf0 = perf0;

    switch (gxdt->perf0) {
    case GX_PERF0_VERTICES:
        GX_XF_LOAD_REG(0x1006, 0x273);
        break;
    case GX_PERF0_CLIP_VTX:
        GX_XF_LOAD_REG(0x1006, 0x14a);
        break;
    case GX_PERF0_CLIP_CLKS:
        GX_XF_LOAD_REG(0x1006, 0x16b);
        break;
    case GX_PERF0_XF_WAIT_IN:
        GX_XF_LOAD_REG(0x1006, 0x84);
        break;
    case GX_PERF0_XF_WAIT_OUT:
        GX_XF_LOAD_REG(0x1006, 0xc6);
        break;
    case GX_PERF0_XF_XFRM_CLKS:
        GX_XF_LOAD_REG(0x1006, 0x210);
        break;
    case GX_PERF0_XF_LIT_CLKS:
        GX_XF_LOAD_REG(0x1006, 0x252);
        break;
    case GX_PERF0_XF_BOT_CLKS:
        GX_XF_LOAD_REG(0x1006, 0x231);
        break;
    case GX_PERF0_XF_REGLD_CLKS:
        GX_XF_LOAD_REG(0x1006, 0x1AD);
        break;
    case GX_PERF0_XF_REGRD_CLKS:
        GX_XF_LOAD_REG(0x1006, 0x1CE);
        break;
    case GX_PERF0_CLOCKS:
        GX_XF_LOAD_REG(0x1006, 0x21);
        break;
    case GX_PERF0_CLIP_RATIO:
        GX_XF_LOAD_REG(0x1006, 0x153);
        break;
    case GX_PERF0_TRIANGLES:
        GX_BP_LOAD_REG(0x2300ae7f);
        break;
    case GX_PERF0_TRIANGLES_CULLED:
        GX_BP_LOAD_REG(0x23008e7f);
        break;
    case GX_PERF0_TRIANGLES_PASSED:
        GX_BP_LOAD_REG(0x23009e7f);
        break;
    case GX_PERF0_TRIANGLES_SCISSORED:
        GX_BP_LOAD_REG(0x23001e7f);
        break;
    case GX_PERF0_TRIANGLES_0TEX:
        GX_BP_LOAD_REG(0x2300ac3f);
        break;
    case GX_PERF0_TRIANGLES_1TEX:
        GX_BP_LOAD_REG(0x2300ac7f);
        break;
    case GX_PERF0_TRIANGLES_2TEX:
        GX_BP_LOAD_REG(0x2300acbf);
        break;
    case GX_PERF0_TRIANGLES_3TEX:
        GX_BP_LOAD_REG(0x2300acff);
        break;
    case GX_PERF0_TRIANGLES_4TEX:
        GX_BP_LOAD_REG(0x2300ad3f);
        break;
    case GX_PERF0_TRIANGLES_5TEX:
        GX_BP_LOAD_REG(0x2300ad7f);
        break;
    case GX_PERF0_TRIANGLES_6TEX:
        GX_BP_LOAD_REG(0x2300adbf);
        break;
    case GX_PERF0_TRIANGLES_7TEX:
        GX_BP_LOAD_REG(0x2300adff);
        break;
    case GX_PERF0_TRIANGLES_8TEX:
        GX_BP_LOAD_REG(0x2300ae3f);
        break;
    case GX_PERF0_TRIANGLES_0CLR:
        GX_BP_LOAD_REG(0x2300a27f);
        break;
    case GX_PERF0_TRIANGLES_1CLR:
        GX_BP_LOAD_REG(0x2300a67f);
        break;
    case GX_PERF0_TRIANGLES_2CLR:
        GX_BP_LOAD_REG(0x2300aa7f);
        break;
    case GX_PERF0_QUAD_0CVG:
        GX_BP_LOAD_REG(0x2402c0c6);
        break;
    case GX_PERF0_QUAD_NON0CVG:
        GX_BP_LOAD_REG(0x2402c16b);
        break;
    case GX_PERF0_QUAD_1CVG:
        GX_BP_LOAD_REG(0x2402c0e7);
        break;
    case GX_PERF0_QUAD_2CVG:
        GX_BP_LOAD_REG(0x2402c108);
        break;
    case GX_PERF0_QUAD_3CVG:
        GX_BP_LOAD_REG(0x2402c129);
        break;
    case GX_PERF0_QUAD_4CVG:
        GX_BP_LOAD_REG(0x2402c14a);
        break;
    case GX_PERF0_AVG_QUAD_CNT:
        GX_BP_LOAD_REG(0x2402c1ad);
        break;
    case GX_PERF0_NONE:
        break;
    default:
        break;
    }

    gxdt->perf1 = perf1;

    switch (gxdt->perf1) {
    case GX_PERF1_TEXELS:
        GX_BP_LOAD_REG(0x67000042);
        break;
    case GX_PERF1_TX_IDLE:
        GX_BP_LOAD_REG(0x67000084);
        break;
    case GX_PERF1_TX_REGS:
        GX_BP_LOAD_REG(0x67000063);
        break;
    case GX_PERF1_TX_MEMSTALL:
        GX_BP_LOAD_REG(0x67000129);
        break;
    case GX_PERF1_TC_MISS:
        GX_BP_LOAD_REG(0x67000252);
        break;
    case GX_PERF1_CLOCKS:
        GX_BP_LOAD_REG(0x67000021);
        break;
    case GX_PERF1_TC_CHECK1_2:
        GX_BP_LOAD_REG(0x6700014b);
        break;
    case GX_PERF1_TC_CHECK3_4:
        GX_BP_LOAD_REG(0x6700018d);
        break;
    case GX_PERF1_TC_CHECK5_6:
        GX_BP_LOAD_REG(0x670001cf);
        break;
    case GX_PERF1_TC_CHECK7_8:
        GX_BP_LOAD_REG(0x67000211);
        break;
    case GX_PERF1_VC_ELEMQ_FULL:
        GX_CP_SET_STAT_SEL_STALLPERF(gxdt->perfSel, ELEMQ_FULL);
        GX_CP_LOAD_REG(GX_CP_REG_STAT_SEL, gxdt->perfSel);
        break;
    case GX_PERF1_VC_MISSQ_FULL:
        GX_CP_SET_STAT_SEL_STALLPERF(gxdt->perfSel, MISSQ_FULL);
        GX_CP_LOAD_REG(GX_CP_REG_STAT_SEL, gxdt->perfSel);
        break;
    case GX_PERF1_VC_MEMREQ_FULL:
        GX_CP_SET_STAT_SEL_STALLPERF(gxdt->perfSel, MEMREQ_FULL);
        GX_CP_LOAD_REG(GX_CP_REG_STAT_SEL, gxdt->perfSel);
        break;
    case GX_PERF1_VC_STATUS7:
        GX_CP_SET_STAT_SEL_STALLPERF(gxdt->perfSel, VC_STATCNT7);
        GX_CP_LOAD_REG(GX_CP_REG_STAT_SEL, gxdt->perfSel);
        break;
    case GX_PERF1_VC_MISSREP_FULL:
        GX_CP_SET_STAT_SEL_STALLPERF(gxdt->perfSel, VC_MISS_REP_FULL);
        GX_CP_LOAD_REG(GX_CP_REG_STAT_SEL, gxdt->perfSel);
        break;
    case GX_PERF1_VC_STREAMBUF_LOW:
        GX_CP_SET_STAT_SEL_STALLPERF(gxdt->perfSel, VC_STALL_STMBUFLOW);
        GX_CP_LOAD_REG(GX_CP_REG_STAT_SEL, gxdt->perfSel);
        break;
    case GX_PERF1_VC_ALL_STALLS:
        GX_CP_SET_STAT_SEL_STALLPERF(gxdt->perfSel, ALL_STALL);
        GX_CP_LOAD_REG(GX_CP_REG_STAT_SEL, gxdt->perfSel);
        break;
    case GX_PERF1_VERTICES:
        GX_CP_SET_STAT_SEL_STALLPERF(gxdt->perfSel, VTX_CNT);
        GX_CP_LOAD_REG(GX_CP_REG_STAT_SEL, gxdt->perfSel);
        break;
    case GX_PERF1_FIFO_REQ:
        reg = DFIFO_REQ_CNT;
        GX_CP_REG_WRITE_U16(GX_CP_MEMPERF_SEL, reg);
        break;
    case GX_PERF1_CALL_REQ:
        reg = OBJCALL_REQ_CNT;
        GX_CP_REG_WRITE_U16(GX_CP_MEMPERF_SEL, reg);
        break;
    case GX_PERF1_VC_MISS_REQ:
        reg = VCMISS_REQ_CNT;
        GX_CP_REG_WRITE_U16(GX_CP_MEMPERF_SEL, reg);
        break;
    case GX_PERF1_CP_ALL_REQ:
        reg = ALL_MEMREQ_CNT;
        GX_CP_REG_WRITE_U16(GX_CP_MEMPERF_SEL, reg);
        break;
    case GX_PERF1_NONE:
        break;
    default:
        break;
    }

    gxdt->lastWriteWasXF = GX_FALSE;
}

void GXClearGPMetric(void) {
    GX_CP_REG_WRITE_U16(GX_CP_CLEAR_METRIC, 4);
}
