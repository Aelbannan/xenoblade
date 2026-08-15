#include <harness_catalog.h>
#include "libs/CriWare/src/sofdec/sfx/sfx_types.h"

/* SFX conversion state handle: the MWSFD handle keeps the SFX state at 0xC8. */
typedef struct MWSFDHn {
    u8 _00[0xC8];
    void *sfx; /* 0xC8 */
} MWSFDHn;

/* Movie-player handle: the Y84C44 frame size fields read by this converter. */
typedef struct MWPly {
    u8 _00[0x0C];
    u32 width;       /* 0x0C */
    u32 bytesPerLine; /* 0x10 */
} MWPly;

extern int MWSFD_CnvFrmInfToSfx(void *ctx, void *ply, void *inf);
extern void SFX_SetBytePerPixelOutBuf(void *self, u32 val);
extern void SFX_Make2PlaneCftDstBuf(void *self, void *stmInf, void *buf0, void *buf1,
                                    void *dstArray, int xOfs, int yOfs,
                                    u32 width, u32 bytesPerLine);
extern int SFX_CnvFrmY84C44ByCbFunc(void *ctx, void *inf, void *dst);

int mwPlyFxCnvFrmY84C44(MWSFDHn *unk, MWPly *ply, void *frame, void *pict) {
    void *sfx_ctx;
    u8 inf_buf[0x98];
    u8 dst_buf[0x68];

    sfx_ctx = unk->sfx;
    MWSFD_CnvFrmInfToSfx(unk, ply, inf_buf);
    SFX_SetBytePerPixelOutBuf(sfx_ctx, 1);
    SFX_Make2PlaneCftDstBuf(sfx_ctx, inf_buf, frame, pict, dst_buf, 0, 0,
                            ply->width, ply->bytesPerLine);
    return SFX_CnvFrmY84C44ByCbFunc(sfx_ctx, inf_buf, dst_buf);
}
