#pragma once

#include <types.h>

// ---- shared LOD renderer state (retail .sbss, small-data) ----
// C-linkage imports (retail symbol names - keep linkage/signatures verbatim)
extern "C" {
extern u32 lbl_eu_80665770;
extern u32 lbl_eu_80665774;
extern s32 lbl_eu_806657D0;
extern u32 lbl_eu_806657D4;
extern u32 lbl_eu_806657D8;
extern u32 sLastColorChanMode;
extern u32 sLastAlphaChanMode;
extern f32 sAmbientIntensityScale;
extern u32 lbl_eu_806657E8;
extern u32 sLodLightMask0;
extern u32 sLodLightMask1;
extern u32 sLodLightMask2;
extern u32 lbl_eu_806657F8;
extern u32 lbl_eu_806657FC;
extern f32 lbl_eu_80665800;
extern u32 lbl_eu_80665804;
extern f32 sAmbientAlphaScale;
extern u32 lbl_eu_8066580C;
extern u32 lbl_eu_80665810;
extern u32 sAmbientAlpha;
extern u32 sCachedCurrentMtx;
extern u32 lbl_eu_8066581C;
extern u32 lbl_eu_80665820;
extern u32 lbl_eu_80665824;
extern const float lbl_eu_8066A630;
}

typedef void (*LodTexCoordSetup)(s32, s32);
extern "C" LodTexCoordSetup sLodTexCoordSetupTable[];
