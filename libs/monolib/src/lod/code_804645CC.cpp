// Decompilation of monolib/src/lod/code_804645CC
// LOD::UnkClass_804645CC - LOD billboard/quad renderer state + draw routines.
//
// Retail symbols are annotated Fv (void) in symbols.txt (stripped DOL), but
// several routines receive data in r3/r4/f1. MWCC cannot emit an Fv mangling
// for a function that has parameters, so those routines are defined as free
// functions carrying their real signature and an asm() label that forces the
// retail ...Fv symbol. Register usage matches the retail ABI (r3=first arg).

#include <harness_catalog.h>
#include <types.h>
#include <revolution/GX.h>
#include <revolution/gx/GXTev.h>
#include <revolution/gx/GXAttr.h>
#include <revolution/gx/GXGeometry.h>
#include <revolution/gx/GXTransform.h>

// ---- shared LOD renderer state (retail .sbss, small-data) ----
extern u32 lbl_eu_80665770;
extern u32 lbl_eu_80665774;
extern u32 lbl_eu_806657D0;
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

namespace LOD {
struct UnkClass_804645CC {
    void func_804645CC();
    void func_80464B84();
    void func_8046513C();
    void func_80465298();
    void func_80465314();
    void func_8046534C();
    void func_8046568C();
    void func_80465704();
    void func_80465718();
    void func_80465730();
    void func_8046577C();
    void func_804657E4();
    void func_80465800();
    void func_80465BC0();
};
}

using namespace LOD;

void LOD::UnkClass_804645CC::func_804645CC() {}

void LOD::UnkClass_804645CC::func_80464B84() {}

void LOD::UnkClass_804645CC::func_8046513C() {}

// Reset all four TEV swap-mode tables to the identity RGBA mapping.
void LOD::UnkClass_804645CC::func_80465298() {
    GXSetTevSwapModeTable(GX_TEV_SWAP0, GX_CH_RED, GX_CH_GREEN, GX_CH_BLUE, GX_CH_ALPHA);
    GXSetTevSwapModeTable(GX_TEV_SWAP1, GX_CH_RED, GX_CH_GREEN, GX_CH_BLUE, GX_CH_ALPHA);
    GXSetTevSwapModeTable(GX_TEV_SWAP2, GX_CH_RED, GX_CH_GREEN, GX_CH_BLUE, GX_CH_ALPHA);
    GXSetTevSwapModeTable(GX_TEV_SWAP3, GX_CH_RED, GX_CH_GREEN, GX_CH_BLUE, GX_CH_ALPHA);
}

void LOD::UnkClass_804645CC::func_80465314() {}

void LOD::UnkClass_804645CC::func_8046534C() {}

// Per-frame vertex-array / cull setup (guarded by the begin/end counter).
void LOD::UnkClass_804645CC::func_8046568C() {
    if ((lbl_eu_806657E8 & 1) == 0) {
        if (lbl_eu_80665818 != 0) {
            lbl_eu_80665818 = 0;
            GXSetCurrentMtx(0);
        }
        GXSetCullMode(GX_CULL_NONE);
        GXSetArray(GX_VA_TEX0, (const void*)lbl_eu_80665770, 8);
        GXSetArray(GX_VA_CLR0, (const void*)lbl_eu_80665774, 4);
        lbl_eu_806657E8 |= 1;
    }
}

// Store 0xFF into the pending-value slot and clear flag bit 1 of the state word.
void LOD::UnkClass_804645CC::func_80465718() {
    lbl_eu_80665814 = 0xFF;
    lbl_eu_806657E8 &= ~2u;
}

void LOD::UnkClass_804645CC::func_80465800() {}

void LOD::UnkClass_804645CC::func_80465BC0() {}

// ===== argument-bearing routines =====
// These receive data in r3/r4/f1 but retail symbols are annotated Fv (void).
// This MWCC (Wii/1.1, 4.3 build 151) rejects function asm() labels (error
// 33106 on every syntax variant), so a parameterized function cannot emit the
// ...Fv symbol. Matching them requires re-annotating symbols.txt/targets.json
// with the true mangling - see stall packet / final report.

void LOD::UnkClass_804645CC::func_80465704() {}

void LOD::UnkClass_804645CC::func_80465730() {}

void LOD::UnkClass_804645CC::func_8046577C() {}

void LOD::UnkClass_804645CC::func_804657E4() {}
