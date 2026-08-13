// Auto-scaffolded catalog TU for monolib/src/core/code_804F0258
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>
#include <revolution/GX.h>
#include <revolution/MTX.h>
#include "monolib/effect/code_804CC2B8.hpp"
#include "monolib/math/CVec3.hpp"
#include "monolib/device/CDeviceVI.hpp"

// Shared read-only literals (monolibdata2 .sdata2).
extern const f32 lbl_eu_8066B478;  // 0.0f
extern const f32 lbl_eu_8066B488;  // 1.0f
extern const f64 lbl_eu_8066B498;  // 0x4330000000000000 (u->f magic)
extern const f64 lbl_eu_8066B4A0;  // 0x4330000080000000 (s->f magic)
extern const f32 lbl_eu_8066B4A8;  // 0.5f
extern const f32 lbl_eu_8066B4AC;  // 2.0f

// Last emitted line-segment vertex (screen-space bookkeeping).
extern f32 lbl_eu_80665A68;
extern f32 lbl_eu_80665A6C;
extern f32 lbl_eu_80665A70;
extern f32 lbl_eu_80665A74;

// u16 -> float through the shared 0x4330000000000000 magic constant.
inline float u16ToF(u16 v) {
    union {
        double d;
        u32 w[2];
    } c;
    c.w[1] = v;
    c.w[0] = 0x43300000u;
    return (float)(c.d - lbl_eu_8066B498);
}

// s32 -> float through the shared 0x4330000080000000 magic constant.
inline float s32ToF(s32 v) {
    union {
        double d;
        u32 w[2];
    } c;
    c.w[0] = 0x43300000u;
    c.w[1] = (u32)v ^ 0x80000000u;
    return (float)(c.d - lbl_eu_8066B4A0);
}

// Object whose virtual slot 3 (vptr+0xC) returns a per-frame scale factor.
struct CLineObj {
    virtual void v0() = 0;
    virtual void v1() = 0;
    virtual void v2() = 0;
    virtual f32 v3() = 0;
};

void func_804F0258(){}

void func_804F06C4(){}

void func_804F0F2C(){}

void func_804F10A0(){}

// func_804F1B88: draw a screen-space line strip. Each segment advances the
// current point along (dir-pos)/count; the offset vector perturbs the point
// used for the per-vertex unit direction, and the GX vertex stream carries
// position, color and (optionally) two texcoord pairs.
void func_804F1B88(f32 scale, const ml::CVec3* pos, const ml::CVec3* dir,
                   const ml::CVec3* off, s32 count, const GXColor* color,
                   CLineObj* obj, s32 flag, GXColor glow) {
    f32 fbWidth = u16ToF(CDeviceVI::getRenderModeObj()->fbWidth);
    f32 fbHeight = u16ToF(CDeviceVI::getRenderModeObj()->efbHeight);
    f32 fCount = s32ToF(count);

    f32 div = lbl_eu_8066B488 / fCount; // 1.0f / segment count

    ml::CVec3 diff = *dir - *pos;
    ml::CVec3 step = diff * div;
    ml::CVec3 current = *pos;

    for (s16 i = 0; i < count; i++) {
        ml::CVec3 delta = current - *off;

        f32 mag = PSVECMag(delta);
        if (lbl_eu_8066B4A8 < mag) {
            if (delta.magnitude() == lbl_eu_8066B478) {
                delta = ml::CVec3::zero;
            } else {
                PSVECNormalize(delta, delta);
            }
            f32 s = scale * obj->v3();
            delta.x *= s;
            delta.y *= s;
            delta.z *= s;
        } else {
            f32 s = lbl_eu_8066B4AC * scale * obj->v3();
            delta.x *= s;
            delta.y *= s;
            delta.z *= s;
        }

        f32 px = current.x * fbWidth + delta.x;
        f32 py = current.y * fbHeight + delta.y;
        WGPIPE.f = px;
        WGPIPE.f = py;
        WGPIPE.f = lbl_eu_8066B478;
        WGPIPE.uc = color->r;
        WGPIPE.uc = color->g;
        WGPIPE.uc = color->b;
        WGPIPE.uc = color->a;
        WGPIPE.f = current.x;
        WGPIPE.f = current.y;
        if (flag != 0) {
            WGPIPE.f = current.x;
            WGPIPE.f = current.y;
        }
        if (glow.a != 0 && (s16)i == 0) {
            lbl_eu_80665A68 = px;
            lbl_eu_80665A6C = py;
            lbl_eu_80665A70 = current.x;
            lbl_eu_80665A74 = current.y;
        }
        current += step;
    }
}

void func_804F1F18(){}

void func_804F213C(){}

extern "C" void func_804F2A8C(void* self) {
    *(u32*)((u8*)self + 8) = 0;
    *(u32*)((u8*)self + 0) = 0;
    *(u32*)((u8*)self + 4) = 0;
}

// Animation-player handle: the bound anim object, its id, and the player
// (world matrix) that v13() fetches for that id.
struct CAnim;
struct CAnimPlayer;

struct CAnimPlayerRef {
    CAnim* mAnim;         // 0x00
    u32 mId;              // 0x04
    CAnimPlayer* mPlayer; // 0x08
};

// Virtual dispatch target: v_i at vptr offset 8+4*i (MWCC RTTI header, the
// retail vptr points at the symbol start). Only slots 13 (0x3c) and 23
// (0x64) are actually called from this TU.
struct CAnim {
    virtual void v0() = 0;
    virtual void v1() = 0;
    virtual void v2() = 0;
    virtual void v3() = 0;
    virtual void v4() = 0;
    virtual void v5() = 0;
    virtual void v6() = 0;
    virtual void v7() = 0;
    virtual void v8() = 0;
    virtual void v9() = 0;
    virtual void v10() = 0;
    virtual void v11() = 0;
    virtual void v12() = 0;
    virtual CAnimPlayer* v13(u32 id) = 0; // vptr + 0x3c: fetch player for id
    virtual void v14() = 0;
    virtual void v15() = 0;
    virtual void v16() = 0;
    virtual void v17() = 0;
    virtual void v18() = 0;
    virtual void v19() = 0;
    virtual void v20() = 0;
    virtual void v21() = 0;
    virtual void v22() = 0;
    virtual bool v23() = 0; // vptr + 0x64: player availability check
};

void func_804F2AA0(CAnimPlayerRef* self, CAnim* anim, u32 id) {
    self->mAnim = anim;
    self->mId = id;
    self->mPlayer = 0;
    if (anim != 0 && id != 0) {
        if (anim->v23()) {
            self->mPlayer = self->mAnim->v13(self->mId);
        }
    }
}

void func_804F2B20(CAnimPlayerRef* self, CAnim* anim) {
    if (self->mAnim != anim) {
        self->mAnim = anim;
        u32 id = self->mId;
        self->mPlayer = 0;
        if (anim != 0 && id != 0) {
            if (anim->v23()) {
                self->mPlayer = self->mAnim->v13(self->mId);
            }
        }
    }
    if (self->mPlayer == 0 && self->mAnim != 0 && self->mId != 0) {
        if (self->mAnim->v23()) {
            self->mPlayer = self->mAnim->v13(self->mId);
        }
    }
}

void func_804F2C04(){}

// The player is a 4x4 row-major matrix; its translation column
// ([0][3], [1][3], [2][3]) is the position getPos() copies out.
struct CAnimPlayer {
    f32 m[4][4];
};

// Float -> u32 bit copy. mwcc has no fpr->gpr move, so this spills the
// value through a stack slot (lfs/stfs/lwz).
static inline u32 floatBits(f32 f) {
    union {
        f32 f;
        u32 u;
    } c;
    c.f = f;
    return c.u;
}

s32 func_804F2DF0(CAnimPlayerRef* self, ml::CVec3* out) {
    CAnimPlayer* p = self->mPlayer;
    if (p == 0) return 0;
    f32 z = p->m[2][3];
    f32 y = p->m[1][3];
    f32 x = p->m[0][3];
    *(u32*)&out->x = floatBits(x);
    *(u32*)&out->y = floatBits(y);
    *(u32*)&out->z = floatBits(z);
    return 1;
}

void func_804F2E44(){}

void func_804F3258(){}

void func_804F3988(){}

void func_804F3B4C(Mtx mtx, const void* scene) {
    func_80496120(scene, mtx, -1);
}

void func_804F3B60(){}

void func_804F42A0(){}

void func_804F45EC(const void* src) {
    Mtx44 m;
    func_80496120(src, m, -1);
    GXSetProjection(m, GX_PERSPECTIVE);
}

void func_804F4620(Mtx mtx){ GXSetProjection(mtx, GX_PERSPECTIVE); }

void func_804F4628(){}

void func_804F4D74(Mtx mtx){ GXSetProjection(mtx, GX_ORTHOGRAPHIC); }
