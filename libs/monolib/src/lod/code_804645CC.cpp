// Decompilation of monolib/src/lod/code_804645CC
// LOD::UnkClass_804645CC - LOD billboard/quad renderer state + draw routines.
//
// Retail symbols use shortened Fv names in symbols.txt (the DOL is stripped),
// while several entry points receive data in r3/r4/f1.  These explicit retail
// name entry points model the extra ABI values as ordinary C++ parameters, as
// documented in docs/MWCC_REFERENCE.md.  Register usage follows the retail
// ABI (r3 is the first explicit value).

#include <harness_catalog.h>
#include <types.h>
#include <revolution/GX.h>
#include <revolution/gx/GXTev.h>
#include <revolution/gx/GXAttr.h>
#include <revolution/gx/GXGeometry.h>
#include <revolution/gx/GXTransform.h>

// ---- shared LOD renderer state (retail .sbss, small-data) ----
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

typedef void (*LodTexCoordSetup)(s32, s32);
extern "C" LodTexCoordSetup lbl_eu_8056D728[];

extern "C" void func_80465314__Q23LOD17UnkClass_804645CCFv(s32 value) {
    if (lbl_eu_806657D0 == value) {
        return;
    }

    u32 offset = ((u32)value << 1) & 0x7C;
    s32 lowBit = value & 1;
    LodTexCoordSetup handler = *(LodTexCoordSetup*)((u8*)lbl_eu_8056D728 + offset);
    s32 highByte = value >> 8;
    lbl_eu_806657D0 = value;
    handler(lowBit + 2, highByte);
}

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

// The retail table keeps these entry points under shortened Fv names while
// passing their real ABI values in the argument registers.  Keep the linker
// names explicit and model those values as ordinary C++ parameters.
extern "C" void func_80465704__Q23LOD17UnkClass_804645CCFv(s32 value) {
    lbl_eu_80665814 = value;
    lbl_eu_806657E8 |= 2;
}

extern "C" void func_80465730__Q23LOD17UnkClass_804645CCFv(f32 scale) {
    s32 value = lbl_eu_80665814;
    lbl_eu_806657E8 |= 2;
    lbl_eu_80665814 = (s32)((f32)value * scale);
}

extern "C" void func_8046577C__Q23LOD17UnkClass_804645CCFv(s32 value) {
    if (value == 0xFF) {
        s32 flags = lbl_eu_806657E8;
        lbl_eu_80665804 = 0;
        lbl_eu_806657E8 = flags & ~4;
        return;
    }

    u32 flags = lbl_eu_806657E8 | 4;
    lbl_eu_806657E8 = flags;
    lbl_eu_80665804 = 6;
    lbl_eu_80665808 = lbl_eu_8066A630 * (f32)value;
}

extern "C" void func_804657E4__Q23LOD17UnkClass_804645CCFv(s16 value) {
    lbl_eu_806657E4 = (f32)value;
}
