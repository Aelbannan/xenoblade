#include <harness_catalog.h>

extern int MWSFD_CnvFrmInfToSfx(void *ctx, void *ply, void *inf);
extern int SFX_SetBytePerPixelOutBuf(void *ctx, int val);
extern int SFX_Make2PlaneCftDstBuf(void *ctx, void *inf, void *frame, void *dst, int a, int b, int c);
extern int SFX_CnvFrmY84C44ByCbFunc(void *ctx, void *inf, void *dst);

int mwPlyFxCnvFrmY84C44(void *unk, void *ply, void *frame, void *pict) {
    void *sfx_ctx;
    u8 inf_buf[0x68];
    u8 dst_buf[0x68];
    
    sfx_ctx = *(void **)((u8 *)unk + 0xC8);
    MWSFD_CnvFrmInfToSfx(unk, ply, inf_buf);
    SFX_SetBytePerPixelOutBuf(sfx_ctx, 1);
    *(u32 *)&dst_buf[0] = *(u32 *)((u8 *)ply + 0x10);
    SFX_Make2PlaneCftDstBuf(sfx_ctx, inf_buf, frame, dst_buf, 0, 0, *(u32 *)((u8 *)ply + 0x0C));
    return SFX_CnvFrmY84C44ByCbFunc(sfx_ctx, inf_buf, dst_buf);
}
