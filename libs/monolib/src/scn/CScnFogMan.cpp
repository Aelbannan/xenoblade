// Auto-scaffolded catalog TU for monolib/src/scn/CScnFogMan
#include <harness_catalog.h>
#include "revolution/GX.h"
#include "nw4r/db/db_assert.h"  // nw4r::db::Panic
#include "nw4r/g3d/g3d_anmclr.h"
#include "nw4r/g3d/g3d_anmobj.h"
#include "nw4r/g3d/g3d_anmscn.h"
#include "nw4r/g3d/g3d_anmtexpat.h"
#include "nw4r/g3d/g3d_anmtexsrt.h"
#include "nw4r/g3d/g3d_fog.h"
#include "nw4r/g3d/g3d_scnmdl.h"
#include "nw4r/g3d/g3d_scnobj.h"
#include "nw4r/g3d/g3d_scnroot.h"
#include "nw4r/g3d/res/g3d_resfile.h"
#include "nw4r/g3d/res/g3d_resmdl.h"
#include "nw4r/g3d/res/g3d_resanmchr.h"
#include "nw4r/math/math_arithmetic.h"
#include "nw4r/math/math_triangular.h"
#include "revolution/MTX.h"
// CScnItemModelNw4r.hpp declares FogManCopyFogParams with a void*/f32* view of the
// signature; this TU defines it with the typed CScnFogMan*/SWordVec* view.
// Rename the header's declaration out of the way for this inclusion so only
// the definition below is visible (same technique as CScnEnvLgtCtrl.cpp).
#define FogManCopyFogParams FogManCopyFogParams_header_view
#include "libs/monolib/src/scn/CScnItemModelNw4r.hpp"  // getScnRootSlot10
#undef FogManCopyFogParams
#include "libs/monolib/src/scn/CScnFogMan.hpp"

// Retail flash constants (fog default colour/parameters). Referenced directly
// so the emitted lfs relocations hit the exact retail labels.
extern const f32 lbl_eu_8066ABB0; // (f4 in ctor)
extern const f32 lbl_eu_8066ABB4; // (f3 in ctor)
extern const f32 lbl_eu_8066ABB8; // (f2 in ctor)
extern const f32 lbl_eu_8066ABBC; // (f1 in ctor)
extern const f32 lbl_eu_8066ABC0; // (f0 in ctor)
extern const f32 lbl_eu_8066ABC4;
extern const f32 lbl_eu_8066ABC8;
extern const f32 lbl_eu_8066ABCC;
extern const f32 lbl_eu_8066ABD0;
extern const f32 lbl_eu_8066ABD4;
extern const double lbl_eu_8066ABD8;

// NW4R assert strings living in this TU's blob (see data below):
// each Panic site references its own copy of the file/message pair.
extern char lbl_eu_8056EBE0[];
extern char lbl_eu_8056EC00[];
extern char lbl_eu_8056EC10[];
extern char lbl_eu_8056EC30[];
extern char lbl_eu_8056EC40[];
extern char lbl_eu_8056EC60[];

// vtable (blob data defined below).
extern "C" u32 lbl_eu_8056EBD0[4];

// ===== Dissolved monolibdata2 (blob surgery) data owned by this TU =====
extern "C" u32 lbl_eu_80663A68[2];  // RTTI locator (owned by CScnItemCamera.cpp)
extern "C" CScnFogMan* __dt__10CScnFogManFv(CScnFogMan* self, int flag);

// [.rodata] 0x80524218-0x80524228 (0x10 = 16B): RTTI name "CScnFogMan".
extern "C" __declspec(align(8)) const char lbl_eu_80524218[0x10] = {
    0x43,0x53,0x63,0x6E,0x46,0x6F,0x67,0x4D,0x61,0x6E,0x00,0x00,0x00,0x00,0x00,0x00,
};

// [.data] 0x8056EBD0-0x8056EC70 (0xA0 = 160B): vtable + NW4R assert strings.
extern "C" u32 lbl_eu_8056EBD0[4] = {
    (u32)&lbl_eu_80663A68, 0x00000000, (u32)&__dt__10CScnFogManFv, 0x00000000,
};

// [.data] string definitions live at end of file (same order); keeping this
// block comment-only preserves downstream line numbers for -ipa file.
// Layout: EBE0[0x20] EC00[0x10] EC10[0x20] EC30[0x10] EC40[0x20] EC60[0x10].
// (msg strings are 0x20 each; file [0x10] sizes include the 6 zero pad
// bytes to the next 8-aligned label; section stays byte-identical 0x90.)
// Retail defines one label per string (msg 0x20, file 0xA); see EOF defs.
// The three message/file pairs below appear in source order at EOF.
// EBE0/EC00 is the first pair, EC10/EC30 the second, EC40/EC60 third.
// Each pair is one assert message plus its g3d_fog.h file string, exactly
// as the retail splitter records them (sizes 0x20/0xA alternating).
// Byte content is unchanged from the previous single-blob initializer.
// (retired DECOMP_FORCEACTIVE keep-alives: assert-blob and RTTI name are
// section-defined globals that survive linking without text refs.)

// Typed primary-vptr slot at +0x0 (retail blob vtable).
struct CScnFogManVptrView {
    u32* vtPrimary;
};

// Constructor is a C-ABI free function named __ct__CScnFogMan (retail has no
// mangled arg suffix), so it must be given extern "C" linkage to keep the name.
extern "C" void __ct__CScnFogMan(CScnFogMan* self, u32 param) {
    reinterpret_cast<CScnFogManVptrView*>(self)->vtPrimary = lbl_eu_8056EBD0;
    self->field_0x04 = param;
    self->value08 = 0;
    self->field_0xC = lbl_eu_8066ABB0;
    self->field_0x10 = lbl_eu_8066ABB0;
    self->field_0x14 = lbl_eu_8066ABB0;
    self->field_0x18 = lbl_eu_8066ABB4;
    self->field_0x1c = lbl_eu_8066ABB8;
    self->field_0x20 = lbl_eu_8066ABBC;
    self->field_0x24 = 0;
    self->field_0x28 = 0;
    self->field_0x2c = 2;
    self->field_0x30 = lbl_eu_8066ABB4;
    self->field_0x34 = lbl_eu_8066ABC0;
    self->field_0x38 = lbl_eu_8066ABB4;
    self->field_0x3c = lbl_eu_8066ABC0;
    self->field_0x40 = lbl_eu_8066ABB4;
    self->field_0x44 = lbl_eu_8066ABB4;
    self->field_0x48 = lbl_eu_8066ABB4;
    self->field_0x4c = lbl_eu_8066ABB4;
    self->field_0x50 = 2;
    self->field_0x54 = lbl_eu_8066ABB4;
    self->field_0x58 = lbl_eu_8066ABC0;
    self->field_0x5c = lbl_eu_8066ABB4;
    self->field_0x60 = lbl_eu_8066ABC0;
    self->field_0x64 = lbl_eu_8066ABB4;
    self->field_0x68 = lbl_eu_8066ABB4;
    self->field_0x6c = lbl_eu_8066ABB4;
    self->field_0x70 = lbl_eu_8066ABB4;
}

extern "C" void FogManSetValue08(u8* self, u32 val) {
    reinterpret_cast<CScnFogMan*>(self)->value08 = val;
}
// Tail-call trampoline over FogManApplyFog (retail is a single `b`).
// Retail symbol is unmangled (C linkage); other TUs' relocs target
// `FogManApplyFog` directly.
// (memberized: FogManApplyFog -> CScnFogMan::ApplyFog; declaration moved into class CScnFogMan)
extern "C" bool FogManIsFogEnabled(CScnFogMan* self) {
    return self->ApplyFog();
}

extern "C" void FogManSetNearFar(u8* self, float a, float b) {
    CScnFogMan* fog = reinterpret_cast<CScnFogMan*>(self);
    fog->field_0x20 = a;
    fog->field_0x1c = b;
}

// Retail dtor (free-function form so MWCC emits no local vtable; the retail
// vtable lives in the blob data above and the ctor stores it explicitly).
extern "C" CScnFogMan* __dt__10CScnFogManFv(CScnFogMan* self, int flag) {
    if (self != nullptr) {
        if (flag > 0) {
            operator delete(self);
        }
    }
    return self;
}

extern "C" void FogManCopyFogWords(u8* self, const void* src) {
    CScnFogMan* fog = reinterpret_cast<CScnFogMan*>(self);
    const u32* words = static_cast<const u32*>(src);
    *(u32*)&fog->field_0xC = words[0];
    *(u32*)&fog->field_0x10 = words[1];
    *(u32*)&fog->field_0x14 = words[2];
    *(u32*)&fog->field_0x18 = words[3];
}

// Copy a fog parameter block into the near/far fog fields. mValue lands in
// field_0x50, the four floats into 0x54-0x60, and the source word-vector is
// mirrored into both 0x64-0x70 and 0xc-0x18. The word copies are bit-reinterpreted
// integer stores so they compile to lwz/stw (retail has no float conversion).
extern "C" void FogManCopyFogParams(CScnFogMan* self, u32 value, const SWordVec* src,
                              f32 p0, f32 p1, f32 p2, f32 p3) {
    self->field_0x50 = value;
    self->field_0x54 = p0;
    self->field_0x58 = p1;
    self->field_0x5c = p2;
    self->field_0x60 = p3;
    *(u32*)&self->field_0x64 = src->v0;
    *(u32*)&self->field_0x68 = src->v1;
    *(u32*)&self->field_0x6c = src->v2;
    *(u32*)&self->field_0x70 = src->v3;
    *(u32*)&self->field_0xC = src->v0;
    *(u32*)&self->field_0x10 = src->v1;
    *(u32*)&self->field_0x14 = src->v2;
    *(u32*)&self->field_0x18 = src->v3;
    self->field_0x1c = p1;
    self->field_0x20 = p0;
}

// Apply the fog-manager state to the scene root's slot-0 fog according to
// value08, then optionally propagate a grayscaled fog colour to all 32 slots
// (when field_0x28 is set). Fog mode 4 means "fog disabled".
bool CScnFogMan::ApplyFog() {
    if (this->value08 == 4) {
        return false;
    }

    nw4r::g3d::ScnRoot* root =
        (nw4r::g3d::ScnRoot*)getScnRootSlot10(this->rootView);
    nw4r::g3d::Fog fog = root->GetFog(0);

    // Comparison order matches the retail branch chain (1, 2, 0, 3).
    if (this->value08 == 1) {
        GXFogType newType = (GXFogType)this->field_0x2c;
        if (fog.ptr() == NULL) {
            nw4r::db::Panic(lbl_eu_8056EC00, 0x41, lbl_eu_8056EBE0);
        }
        if (fog.ptr() != NULL) {
            fog.ref().type = newType;
        }

        f32 startz1;
        f32 endz1;
        endz1 = this->field_0x34;
        startz1 = this->field_0x30;
        if (fog.ptr() == NULL) {
            nw4r::db::Panic(lbl_eu_8056EC30, 0x4b, lbl_eu_8056EC10);
        }
        if (fog.ptr() != NULL) {
            fog.ref().startz = startz1;
            fog.ref().endz = endz1;
        }

        // Pack the scaled channels through a byte temp so MWCC emits the
        // retail stb/lwz/stw sequence.
        u8 tmp1[4];
        tmp1[0] = (u8)(lbl_eu_8066ABC4 * this->field_0x40);
        tmp1[1] = (u8)(lbl_eu_8066ABC4 * this->field_0x44);
        tmp1[2] = (u8)(lbl_eu_8066ABC4 * this->field_0x48);
        tmp1[3] = (u8)(lbl_eu_8066ABC4 * this->field_0x4c);
        GXColor color1 = *(GXColor*)tmp1;

        if (fog.ptr() == NULL) {
            nw4r::db::Panic(lbl_eu_8056EC60, 0x63, lbl_eu_8056EC40);
        }
        if (fog.ptr() != NULL) {
            fog.ref().color.r = color1.r;
            fog.ref().color.g = color1.g;
            fog.ref().color.b = color1.b;
            fog.ref().color.a = color1.a;
        }
        return true;
    } else if (this->value08 == 2) {
        scnLgtApplyFogByte(this->rootView->field_0x7c);
    } else if (this->value08 == 0) {
        GXFogType newType = (GXFogType)this->field_0x50;
        if (fog.ptr() == NULL) {
            nw4r::db::Panic(lbl_eu_8056EC00, 0x41, lbl_eu_8056EBE0);
        }
        if (fog.ptr() != NULL) {
            fog.ref().type = newType;
        }

        // Pack the scaled channels through a byte temp so MWCC emits the
        // retail stb/lwz/stw sequence.
        u8 tmp0[4];
        tmp0[0] = (u8)(lbl_eu_8066ABC4 * this->field_0xC);
        tmp0[1] = (u8)(lbl_eu_8066ABC4 * this->field_0x10);
        tmp0[2] = (u8)(lbl_eu_8066ABC4 * this->field_0x14);
        tmp0[3] = (u8)(lbl_eu_8066ABC4 * this->field_0x18);
        GXColor color0 = *(GXColor*)tmp0;

        if (fog.ptr() == NULL) {
            nw4r::db::Panic(lbl_eu_8056EC60, 0x63, lbl_eu_8056EC40);
        }
        if (fog.ptr() != NULL) {
            fog.ref().color.r = color0.r;
            fog.ref().color.g = color0.g;
            fog.ref().color.b = color0.b;
            fog.ref().color.a = color0.a;
        }

        f32 startz0;
        f32 endz0;
        endz0 = this->field_0x1c;
        startz0 = this->field_0x20;
        if (fog.ptr() == NULL) {
            nw4r::db::Panic(lbl_eu_8056EC30, 0x4b, lbl_eu_8056EC10);
        }
        if (fog.ptr() != NULL) {
            fog.ref().startz = startz0;
            fog.ref().endz = endz0;
        }
    } else if (this->value08 == 3) {
        if (fog.ptr() == NULL) {
            nw4r::db::Panic(lbl_eu_8056EC00, 0x41, lbl_eu_8056EBE0);
        }
        if (fog.ptr() != NULL) {
            fog.ref().type = GX_FOG_NONE;
        }
    }

    if (this->field_0x28 != 0) {
        for (int i = 0; i < 0x20; i++) {
            nw4r::g3d::Fog fogSlot = root->GetFog(i);
            GXColor color;
            fogSlot.GetFog(NULL, NULL, NULL, NULL, NULL, &color);

            // Luminance-weighted grayscale of the current fog colour,
            // clamped above only (retail has no lower clamp).
            f32 lum = lbl_eu_8066ABD0 * color.g + lbl_eu_8066ABCC * color.r +
                      lbl_eu_8066ABC8 * color.b;
            s32 gray = (s32)lum;
            if (gray > 255) {
                gray = 255;
            }
            color.r = (u8)gray;
            color.g = (u8)gray;
            color.b = (u8)gray;
            GXColor setColor = color;

            if (fogSlot.ptr() == NULL) {
                nw4r::db::Panic(lbl_eu_8056EC60, 0x63, lbl_eu_8056EC40);
            }
            if (fogSlot.ptr() != NULL) {
                nw4r::g3d::FogData* pfd = &fogSlot.ref();
                pfd->color.r = setColor.r;
                pfd->color.g = setColor.g;
                pfd->color.b = setColor.b;
                pfd->color.a = setColor.a;
            }
        }
    }

    return true;
}

// [.data] 0x8056EBE0-0x8056EC70: NW4R assert message/file string pairs.
// One label per retail string; file [0x10] sizes include the 6 zero pad
// bytes to the next 8-aligned label so the section stays byte-identical.
extern "C" __declspec(align(8)) char lbl_eu_8056EBE0[0x20] = { 0x4E,0x57,0x34,0x52,0x3A,0x46,0x61,0x69,0x6C,0x65,0x64,0x20,0x61,0x73,0x73,0x65, 0x72,0x74,0x69,0x6F,0x6E,0x20,0x49,0x73,0x56,0x61,0x6C,0x69,0x64,0x28,0x29,0x00 };
extern "C" __declspec(align(8)) char lbl_eu_8056EC00[0x10] = { 0x67,0x33,0x64,0x5F,0x66,0x6F,0x67,0x2E,0x68,0x00,0x00,0x00,0x00,0x00,0x00,0x00 };
extern "C" __declspec(align(8)) char lbl_eu_8056EC10[0x20] = { 0x4E,0x57,0x34,0x52,0x3A,0x46,0x61,0x69,0x6C,0x65,0x64,0x20,0x61,0x73,0x73,0x65, 0x72,0x74,0x69,0x6F,0x6E,0x20,0x49,0x73,0x56,0x61,0x6C,0x69,0x64,0x28,0x29,0x00 };
extern "C" __declspec(align(8)) char lbl_eu_8056EC30[0x10] = { 0x67,0x33,0x64,0x5F,0x66,0x6F,0x67,0x2E,0x68,0x00,0x00,0x00,0x00,0x00,0x00,0x00 };
extern "C" __declspec(align(8)) char lbl_eu_8056EC40[0x20] = { 0x4E,0x57,0x34,0x52,0x3A,0x46,0x61,0x69,0x6C,0x65,0x64,0x20,0x61,0x73,0x73,0x65, 0x72,0x74,0x69,0x6F,0x6E,0x20,0x49,0x73,0x56,0x61,0x6C,0x69,0x64,0x28,0x29,0x00 };
extern "C" __declspec(align(8)) char lbl_eu_8056EC60[0x10] = { 0x67,0x33,0x64,0x5F,0x66,0x6F,0x67,0x2E,0x68,0x00,0x00,0x00,0x00,0x00,0x00,0x00 };
