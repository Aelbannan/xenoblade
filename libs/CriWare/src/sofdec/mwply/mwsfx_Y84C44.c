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

extern int MWSFD_CnvFrmInfToSfx(MWSFDHn *ctx, MWPly *ply, SFXStmInf *inf);
extern void SFX_SetBytePerPixelOutBuf(SFXConvertState *self, u32 val);
extern void SFX_Make2PlaneCftDstBuf(SFXConvertState *self, SFXStmInf *stmInf,
                                    void *buf0,
                                    void *buf1, int xOfs, int yOfs,
                                    SFXDstBufInf *dstArray, u32 width,
                                    u32 bytesPerLine);
extern int SFX_CnvFrmY84C44ByCbFunc(SFXConvertState *ctx, SFXStmInf *inf,
                                    SFXDstBufInf *dst);


/*
 * Convert an Y84C44 movie frame to SFX output.
 * Builds the SFX stream info from the movie player state, configures a
 * single-byte-per-pixel output buffer as two planes, then runs the
 * callback-driven conversion over the plane descriptors.
 */
int mwPlyFxCnvFrmY84C44(MWSFDHn *unk, MWPly *ply, void *frame, void *pict) {
    SFXConvertState *sfx = unk->sfx;
    u32 bytesPerLine = ply->bytesPerLine;
    u8 inf_buf[0x98];
    u8 dst_buf[0x68];
    MWSFD_CnvFrmInfToSfx(unk, ply, (SFXStmInf *)inf_buf);
    SFX_SetBytePerPixelOutBuf(sfx, 1);
    u32 width = ply->width;
    SFX_Make2PlaneCftDstBuf(sfx, (SFXStmInf *)inf_buf, frame, pict, 0, 0,
                            (SFXDstBufInf *)dst_buf, width, bytesPerLine);
    return SFX_CnvFrmY84C44ByCbFunc(sfx, (SFXStmInf *)inf_buf,
                                    (SFXDstBufInf *)dst_buf);
}
