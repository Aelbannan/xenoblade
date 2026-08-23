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
    u32 width;        /* 0x0C */
    u32 bytesPerLine; /* 0x10 */
} MWPly;

/* SFX conversion API implemented in sofdec/sfx/*.c and sofdec/mwply/mwsfdsfx.c. */
extern int MWSFD_CnvFrmInfToSfx(MWSFDHn *ctx, MWPly *ply, SFXStmInf *inf);
extern void SFX_SetBytePerPixelOutBuf(SFXConvertState *self, u32 val);
extern void SFX_Make2PlaneCftDstBuf(SFXConvertState *self, SFXStmInf *stmInf,
                                    u8 *buf0, u8 *buf1,
                                    SFXDstBufInf *dstArray, int xOfs, int yOfs,
                                    u32 width, u32 bytesPerLine);
extern int SFX_CnvFrmY84C44ByCbFunc(SFXConvertState *ctx, SFXStmInf *inf,
                                    SFXDstBufInf *dst);

/*
 * Convert an Y84C44 movie frame to SFX output.
 *
 * Builds the SFX stream info from the movie-player state (MWSFD_CnvFrmInfToSfx),
 * selects a one-byte-per-pixel output format, then describes a two-plane
 * destination buffer covering the full frame at offset (0, 0). The actual
 * pixel conversion runs through the callback-driven converter, which walks
 * the plane descriptors filled in by SFX_Make2PlaneCftDstBuf.
 */
int mwPlyFxCnvFrmY84C44(MWSFDHn *handle, MWPly *ply, u8 *frame, u8 *pict) {
    SFXConvertState *sfx = handle->sfx;
    SFXStmInf inf;
    SFXDstBufInf dst;
    MWSFD_CnvFrmInfToSfx(handle, ply, &inf);
    SFX_SetBytePerPixelOutBuf(sfx, 1);
    /* bytesPerLine feeds the stack-passed 9th argument slot */
    SFX_Make2PlaneCftDstBuf(sfx, &inf, frame, pict,
                            &dst, 0, 0, ply->width,
                            ply->bytesPerLine);
    return SFX_CnvFrmY84C44ByCbFunc(sfx, &inf, &dst);
}
