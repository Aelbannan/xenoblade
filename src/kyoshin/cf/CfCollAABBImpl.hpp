#pragma once

#include <types.h>
#include "monolib/math/CVec3.hpp"
#include "kyoshin/cf/CfGameManagerData.hpp"  // H3 label-owner decl (lbl_eu_80663E14; lbl_eu_80663E24)

namespace ml {
    struct CCol4;
    struct CMat34;
}

// ---------------------------------------------------------------------------
// C-linkage imports (retail symbol names - keep linkage/signatures verbatim)
// ---------------------------------------------------------------------------

extern "C" void setCol__7CDrawGXFRCQ22ml5CCol4(void* self, const ml::CCol4* col);
extern "C" void setMatrix__7CDrawGXFRCQ22ml6CMat34(void* self, const ml::CMat34* mat);
extern "C" void renderCube__7CDrawGXFRCQ22ml5CVec3RCQ22ml5CVec3(void* self, const ml::CVec3* min, const ml::CVec3* max);
extern "C" void func_8049034C(void* a, void* b, int c);
extern "C" void func_800A5FE8(void* a, void* b, void* c, void* d, void* e);

// Debug color / scale constants from .sdata2. Declared `const` so MWCC treats
// the loads as constants and schedules them at the retail positions.
extern const float lbl_eu_80666910; // 0.0
extern const float lbl_eu_80666914; // 1.0
extern const float lbl_eu_80666918; // 0.3
extern const float lbl_eu_80666928; // sin/cos index scale
extern const float lbl_eu_8066692C; // 0.1
extern const float lbl_eu_80666930; // 0.5
extern const float lbl_eu_80666934; // 0.15

namespace cf {

// Interface view exposing the world-position getter. Declaring every slot
// from +0x08 up to +0xAC makes _v0AC line up with the retail vtable index,
// so MWCC emits the retail r12 virtual-call sequence.
struct CfCollAABBVtIf {
    virtual void _v008();
    virtual void _v00C();
    virtual void _v010();
    virtual void _v014();
    virtual void _v018();
    virtual void _v01C();
    virtual void _v020();
    virtual void _v024();
    virtual void _v028();
    virtual void _v02C();
    virtual void _v030();
    virtual void _v034();
    virtual void _v038();
    virtual void _v03C();
    virtual void _v040();
    virtual void _v044();
    virtual void _v048();
    virtual void _v04C();
    virtual void _v050();
    virtual void _v054();
    virtual void _v058();
    virtual void _v05C();
    virtual void _v060();
    virtual void _v064();
    virtual void _v068();
    virtual void _v06C();
    virtual void _v070();
    virtual void _v074();
    virtual void _v078();
    virtual void _v07C();
    virtual void _v080();
    virtual void _v084();
    virtual void _v088();
    virtual void _v08C();
    virtual void _v090();
    virtual void _v094();
    virtual void _v098();
    virtual void _v09C();
    virtual void _v0A0();
    virtual void _v0A4();
    virtual void _v0A8();
    virtual ml::CVec3* _v0AC(); // slot 0xAC
};

// Collision AABB shape implementation.
// Layout offsets derived from CfCollAABBImpl.s.
struct CfCollAABBImpl {
    void* vtable;           // 0x00
    u8 pad_04[0x94 - 0x04];
    int field_94;           // 0x94: color-mode selection flag (compared signed)
    u8 pad_98[0xD8 - 0x98];
    ml::CVec3 mMin;         // 0xD8: AABB minimum corner
    ml::CVec3 mMax;         // 0xE4: AABB maximum corner
    ml::CVec3 mCenter;      // 0xF0: AABB center
    u8 pad_FC[0x14C - 0xFC]; // padding / other collision fields
    float mAngle;           // 0x14C: rotation angle (used by debug render)

    // Fetch world position through the vtable slot at 0xAC.
    ml::CVec3* GetPos() {
        return reinterpret_cast<CfCollAABBVtIf*>(this)->_v0AC();
    }
};

typedef ml::CVec3* (*CfCollAABBImplGetPosFn)(void*);

} // namespace cf
