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
extern u32 lbl_eu_806657DC;
extern u32 lbl_eu_806657E0;
extern f32 lbl_eu_806657E4;
extern u32 lbl_eu_806657E8;
extern u32 lbl_eu_806657EC;
extern u32 lbl_eu_806657F0;
extern u32 lbl_eu_806657F4;
extern u32 lbl_eu_806657F8;
extern u32 lbl_eu_806657FC;
extern f32 lbl_eu_80665800;
extern u32 lbl_eu_80665804;
extern f32 lbl_eu_80665808;
extern u32 lbl_eu_8066580C;
extern u32 lbl_eu_80665810;
extern u32 lbl_eu_80665814;
extern u32 lbl_eu_80665818;
extern u32 lbl_eu_8066581C;
extern u32 lbl_eu_80665820;
extern u32 lbl_eu_80665824;
extern const double lbl_eu_8066A628;
extern const float lbl_eu_8066A630;
}

typedef void (*LodTexCoordSetup)(s32, s32);
extern "C" LodTexCoordSetup lbl_eu_8056D728[];
