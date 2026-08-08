#include <harness_catalog.h>

extern int MWSFD_CnvFrmInfToSfx(void *ctx, void *ply, void *inf);
extern int SFX_SetBytePerPixelOutBuf(void *ctx, int val);
extern int SFX_Make2PlaneCftDstBuf(void *ctx, void *inf, void *frame, void *pict,
                                   void *dst, int a, int b, int c, int bytesPerLine);
extern int SFX_CnvFrmY84C44ByCbFunc(void *ctx, void *inf, void *dst);

int mwPlyFxCnvFrmY84C44(void *unk, void *ply, void *frame, void *pict) {
    void *sfx_ctx;
    u8 inf_buf[0x98];
    u8 dst_buf[0x68];

    sfx_ctx = *(void **)((u8 *)unk + 0xC8);
    MWSFD_CnvFrmInfToSfx(unk, ply, inf_buf);
    SFX_SetBytePerPixelOutBuf(sfx_ctx, 1);
    SFX_Make2PlaneCftDstBuf(sfx_ctx, inf_buf, frame, pict, dst_buf, 0, 0,
                            *(u32 *)((u8 *)ply + 0x0C), *(u32 *)((u8 *)ply + 0x10));
    return SFX_CnvFrmY84C44ByCbFunc(sfx_ctx, inf_buf, dst_buf);
}