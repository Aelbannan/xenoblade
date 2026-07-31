// Decompiled monolib/src/core/code_804EE558
//
// Screen-space "marker" drawing system: a bind/track object
// (CLytBind-like) that derives a world matrix and translation from a
// pane/source according to a type code, plus the GX triangle-fan
// renderer used to draw the marker shape (ring or unit quad fan) with
// screen clamping, gradient texcoord shift and per-vertex colors.

#include <harness_catalog.h>
#include <revolution/GX.h>
#include <revolution/MTX.h>
#include <revolution/gx/GXVert.h>
#include <string.h>

#include "monolib/math/CMat34.hpp"
#include "monolib/math/CVec3.hpp"
#include "monolib/math/MathConstants.hpp"
#include "monolib/device/CDeviceGX.hpp"

extern "C" {

// Shared read-only literals (monolibdata2 .sdata2).
extern const u32 lbl_eu_8066B400;    // 0xFFFFFFFF (GXColor white)
extern const u32 lbl_eu_8066B404;    // 0xFFFFFFFF (GXColor white)
extern const float lbl_eu_8066B408;  // 0.0f
extern const float lbl_eu_8066B40C;  // 1.0f
extern const float lbl_eu_8066B410;  // -1.0f
extern const float lbl_eu_8066B414;  // 0.5f
extern const float lbl_eu_8066B420;  // 255.0f
extern const double lbl_eu_8066B428; // 0x4330000000000000 (u->f magic)
extern const double lbl_eu_8066B430; // 0x4330000080000000 (s->f magic)
extern const float lbl_eu_8066B438;  // 45.0f

// Gradient tables: 360 entries of {base, slope}, one per degree.
struct CGradEntry {
    float base;
    float slope;
};
extern CGradEntry lbl_eu_80660038[360]; // sin table
extern CGradEntry lbl_eu_80660B78[360]; // cos table

// External helpers (other monolib TUs).
extern GXRenderModeObj* getRenderModeObj__9CDeviceVIFv();
extern void* cacheInstance__9CDeviceGX;
extern u32 func_8044BE24__8CGXCacheFv(void* cache);
extern void* func_80496264(void* rsrc, int idx);
extern void func_804D8B28(void* desktop);
extern void func_804D8B30(void* desktop);
extern void func_804D8B38(void* draw);
extern int func_804D8B4C(void* draw, void* desktop, void* material);
extern void func_804D8C18(void* draw);
extern void func_804D8C68(void* draw, int a, int b);
extern void func_804DCA88(void* dst, void* src);
extern void func_804DCD94(void* dst, void* src);
extern void func_804DD440(void* dst, void* src, float f);
extern void func_804DD4F8(void* mtx, float angle);
extern void func_804DF164(void* a, int b, int c, void* d);
extern void func_804F2AA0(void* player, void* anim, u32 id);
extern void func_804F2B20(void* player, void* anim);
extern void func_804F2C04(void* player, void* mtx);
extern void func_804F2DF0(void* player, void* vec);
extern Mtx* func_804F42A0(u32 a, u32 b);
extern void func_804F4628(u32 a, float w, float h);

// Static per-vertex color table, initialized by sinit_804F01C8.
extern float lbl_eu_80661748[30];

} // extern "C"

float lbl_eu_80661748[30];

namespace {

// The retail SDK takes channel colors by pointer; this repo's header
// declares them by value, so call through the pointer signature.
inline void setChanAmbColor(GXChannelID chan, const GXColor* color) {
    reinterpret_cast<void (*)(GXChannelID, const GXColor*)>(GXSetChanAmbColor)(chan, color);
}
inline void setChanMatColor(GXChannelID chan, const GXColor* color) {
    reinterpret_cast<void (*)(GXChannelID, const GXColor*)>(GXSetChanMatColor)(chan, color);
}
// Retail passes a trailing scale/zero word to these TEV setters.
inline void setTevColorIn6(GXTevStageID stage, u32 a, u32 b, u32 c, u32 d, u32 e) {
    reinterpret_cast<void (*)(GXTevStageID, u32, u32, u32, u32, u32)>(GXSetTevColorIn)(stage, a,
                                                                                       b, c, d, e);
}
inline void setTevAlphaIn6(GXTevStageID stage, u32 a, u32 b, u32 c, u32 d, u32 e) {
    reinterpret_cast<void (*)(GXTevStageID, u32, u32, u32, u32, u32)>(GXSetTevAlphaIn)(stage, a,
                                                                                       b, c, d, e);
}

// u16 -> float through the shared 0x4330000000000000 magic constant.
inline float u16ToF(u16 v) {
    union {
        double d;
        u32 w[2];
    } c;
    c.w[0] = 0x43300000u;
    c.w[1] = v;
    return (float)(c.d - lbl_eu_8066B428);
}

// s32 -> float through the shared 0x4330000080000000 magic constant.
inline float s32ToF(s32 v) {
    union {
        double d;
        u32 w[2];
    } c;
    c.w[0] = 0x43300000u;
    c.w[1] = (u32)v ^ 0x80000000u;
    return (float)(c.d - lbl_eu_8066B430);
}

// Table interpolation over one of the 360-entry gradient tables.
inline float gradLookup(const CGradEntry* table, float deg) {
    int n = (int)deg;
    int idx = n % 360;
    if (idx < 0) {
        idx += 360;
    }
    float frac = deg - s32ToF(n);
    return table[idx].base + frac * table[idx].slope;
}

// Pane the bind object attaches to.
struct CBindAnim {
    virtual void slot0();
    virtual void slot1();
    virtual void slot2();
    virtual void slot3();
    virtual void slot4();
    virtual void slot5();
    virtual void slot6();
    virtual void slot7();
    virtual void slot8();
    virtual void slot9();
    virtual void slot10();
    virtual void slot11();
    virtual void slot12();
    virtual void* getPos();
    virtual void* getMtx();
    virtual void slot15();
    virtual void slot16();
    virtual void slot17();
    virtual void slot18();
    virtual void slot19();
    virtual void* slot20();
};

struct CBindPane {
    u8 mPad00[0x10];
    void* mRsrc;   // 0x10
    CBindAnim* mAnimA;  // 0x14
    CBindAnim* mAnimB;  // 0x18
    u8 mPad1C[0x44];
    ml::CMat34 mWorldMtx; // 0x60
};

// Explicit matrix source (type 6): matrix lives at 0x1DC.
struct CBindSource {
    u8 mPad[0x1DC];
    ml::CMat34 mMtx; // 0x1DC
};

// Animation player reference embedded in the bind object (0x0C, 0xC bytes).
struct CAnimPlayerRef {
    void* mAnim;   // 0x0
    u32 mId;       // 0x4
    void* mPlayer; // 0x8

    void init(void* anim, u32 id) { func_804F2AA0(this, anim, id); }
    void update(void* anim) { func_804F2B20(this, anim); }
    void getMtx(ml::CMat34* mtx) { func_804F2C04(this, mtx); }
    void getPos(ml::CVec3* pos) { func_804F2DF0(this, pos); }
};

// The bind/track object itself (0x48 bytes).
struct CLytBind {
    s32 mType;             // 0x00
    CBindPane* mPane;      // 0x04
    u8 mFlag;              // 0x08
    u8 mPad09[3];
    CAnimPlayerRef mPlayer; // 0x0C; its mPlayer is the bind field at 0x14
    ml::CMat34 mMtx;       // 0x18
};

// Draw context built on the stack by func_804D8B38/func_804D8B4C.
struct CDrawCtx {
    u32 mField00;
    GXTexObj* mTexObj; // 0x04
    u8 mPad08[0x38];
};

// Optional texture-matrix source passed to the marker draw.
struct CMarkerMtxSrc {
    void* mTex;   // 0x00
    s8 mIndex;    // 0x04
    u8 mPad05[3];
    void* mField08; // 0x08
    float mField0C; // 0x0C
    float mField10; // 0x10
    float mField14; // 0x14
    float mScaleX;  // 0x18
    float mScaleY;  // 0x1C
    float mScaleZ;  // 0x20
    ml::CMat34 mTexMtx; // 0x24
};

// Per-vertex color used by the fan renderer (RGB triple).
struct CFanColor {
    float r, g, b;
};

} // namespace

// func_804EE558: set up the bind object for a pane.
extern "C" void func_804EE558(CLytBind* self, CBindPane* pane, s32 type, u32 id, u8 flag) {
    self->mType = type;
    self->mPane = pane;
    self->mFlag = flag;
    self->mMtx = ml::CMat34::identity;
    switch (type) {
    case 0x1A:
        self->mPlayer.init(pane->mAnimA, id);
        break;
    case 0x1B:
        self->mPlayer.init(pane->mAnimB, id);
        break;
    default:
        self->mPlayer.init(NULL, 0);
        break;
    }
}

// func_804EE60C: refresh the animation player binding.
extern "C" void func_804EE60C(CLytBind* self) {
    switch (self->mType) {
    case 0x1A:
        if (self->mPane->mAnimA == NULL) {
            return;
        }
        self->mPlayer.update(self->mPane->mAnimA);
        return;
    case 0x1B:
        if (self->mPane->mAnimB == NULL) {
            return;
        }
        self->mPlayer.update(self->mPane->mAnimB);
        return;
    default:
        return;
    }
}

// func_804EE658: rebuild the world matrix from the bound source,
// preserving the current translation.
extern "C" void func_804EE658(CLytBind* self, CBindSource* src) {
    float tx = self->mMtx.m[0][3];
    float ty = self->mMtx.m[1][3];
    float tz = self->mMtx.m[2][3];

    self->mMtx = ml::CMat34::identity;

    switch (self->mType) {
    case 0:
    case 16:
    case 17:
        self->mMtx = self->mPane->mWorldMtx;
        break;
    case 6:
        if (src != NULL) {
            self->mMtx = src->mMtx;
        }
        break;
    case 11: {
        if (self->mFlag == 0) {
            void* obj = func_80496264(self->mPane->mRsrc, -1);
            func_804DCD94(&self->mMtx, (u8*)obj + 0x118);
        } else {
            void* obj = func_80496264(self->mPane->mRsrc, -1);
            func_804DD440(&self->mMtx, obj, *(float*)((u8*)obj + 0x11C));
        }
        break;
    }
    case 21: {
        CBindAnim* obj = self->mPane->mAnimA;
        if (obj == NULL) {
            break;
        }
        func_804DCA88(&self->mMtx, obj->getMtx());
        break;
    }
    case 22: {
        CBindAnim* obj = self->mPane->mAnimB;
        if (obj == NULL) {
            break;
        }
        func_804DCA88(&self->mMtx, obj->getMtx());
        break;
    }
    case 26:
        if (self->mPane->mAnimA != NULL) {
            self->mPlayer.getMtx(&self->mMtx);
        }
        break;
    case 27:
        if (self->mPane->mAnimB != NULL) {
            self->mPlayer.getMtx(&self->mMtx);
        }
        break;
    default:
        break;
    }

    self->mMtx.m[0][3] = tx;
    self->mMtx.m[1][3] = ty;
    self->mMtx.m[2][3] = tz;
}

// func_804EE8FC: rebuild the translation from the bound source.
extern "C" void func_804EE8FC(CLytBind* self, CBindSource* src) {
    ml::CVec3 pos(self->mMtx.m[0][3], self->mMtx.m[1][3], self->mMtx.m[2][3]);

    switch (self->mType) {
    case 0:
    case 16:
    case 17: {
        const ml::CMat34* m = &self->mPane->mWorldMtx;
        pos.set(m->m[0][3], m->m[1][3], m->m[2][3]);
        break;
    }
    case 6:
        if (src != NULL) {
            const ml::CMat34* m = &src->mMtx;
            pos.set(m->m[0][3], m->m[1][3], m->m[2][3]);
        }
        break;
    case 11: {
        void* obj = func_80496264(self->mPane->mRsrc, -1);
        const float* v = (const float*)((u8*)obj + 0x10C);
        pos.set(v[0], v[1], v[2]);
        break;
    }
    case 21: {
        CBindAnim* obj = self->mPane->mAnimA;
        if (obj == NULL) {
            break;
        }
        const float* v = (const float*)obj->getPos();
        pos.set(v[0], v[1], v[2]);
        break;
    }
    case 22: {
        CBindAnim* obj = self->mPane->mAnimB;
        if (obj == NULL) {
            break;
        }
        const float* v = (const float*)obj->getPos();
        pos.set(v[0], v[1], v[2]);
        break;
    }
    case 26:
        if (self->mPane->mAnimA != NULL) {
            self->mPlayer.getPos(&pos);
        }
        break;
    case 27:
        if (self->mPane->mAnimB != NULL) {
            self->mPlayer.getPos(&pos);
        }
        break;
    default:
        break;
    }

    self->mMtx.m[0][3] = pos.x;
    self->mMtx.m[1][3] = pos.y;
    self->mMtx.m[2][3] = pos.z;
}

// func_804EEACC: check whether the bind object can resolve a target.
extern "C" u32 func_804EEACC(CLytBind* self) {
    switch (self->mType) {
    case 0x1A:
        if (self->mPane->mAnimA != NULL && self->mPlayer.mPlayer == NULL) {
            return 0;
        }
        break;
    case 0x1B:
        if (self->mPane->mAnimB != NULL && self->mPlayer.mPlayer == NULL) {
            return 0;
        }
        break;
    default:
        break;
    }
    return self->mPane != NULL ? 1 : 0;
}

// func_804EF830: build the 10-vertex octagon fan (center + ring + close).
extern "C" void func_804EF830(ml::CVec3* verts) {
    for (s16 i = 0; i < 10; i++) {
        if (i == 0) {
            verts[i].set(lbl_eu_8066B408, lbl_eu_8066B408, lbl_eu_8066B408);
        } else if (i < 9) {
            float deg = s32ToF(i - 1) * lbl_eu_8066B438;
            float rad = ml::deg2rad * deg;
            float angle = ml::rad2deg * rad;
            verts[i].x = gradLookup(lbl_eu_80660B78, angle);
            verts[i].y = gradLookup(lbl_eu_80660038, angle);
            verts[i].z = lbl_eu_8066B408;
        } else {
            verts[i] = verts[1];
        }
    }
}

// func_804EF9B8: scale/rotate the fan vertices and offset them to pos.
extern "C" void func_804EF9B8(ml::CVec3* verts, const ml::CVec3* pos, ml::CVec3 size, float angle) {
    Mtx rotMtx;
    if (angle != lbl_eu_8066B408) {
        func_804DD4F8(rotMtx, angle);
    }

    for (s16 i = 0; i < 10; i++) {
        if (i == 0) {
            verts[i] = verts[i] + *pos;
        } else if (i < 9) {
            verts[i].x *= size.x;
            verts[i].y *= size.y;
            verts[i].z *= size.z;
            if (angle != lbl_eu_8066B408) {
                ml::CVec3 tmp;
                PSMTXMultVec(rotMtx, verts[i], tmp);
                verts[i] = tmp;
            }
            verts[i] = verts[i] + *pos;
        } else {
            verts[i] = verts[1];
        }
    }
}

// func_804EFB38: clamp the fan to the screen; returns the scale factor.
extern "C" float func_804EFB38(ml::CVec3* verts) {
    float fbW = u16ToF(getRenderModeObj__9CDeviceVIFv()->fbWidth);
    float fbH = u16ToF(getRenderModeObj__9CDeviceVIFv()->efbHeight);

    float scale = lbl_eu_8066B40C;
    for (u32 i = 1; i <= 8; i++) {
        float t = lbl_eu_8066B40C;
        float dx = ml::math::abs(verts[i].x - verts[0].x);
        if (dx != lbl_eu_8066B408) {
            if (verts[i].x < lbl_eu_8066B408) {
                t = verts[0].x / dx;
            } else if (fbW < verts[i].x) {
                t = (fbW - verts[0].x) / dx;
            }
        }
        float dy = ml::math::abs(verts[i].y - verts[0].y);
        if (dy != lbl_eu_8066B408) {
            if (verts[i].y < lbl_eu_8066B408) {
                t = verts[0].y / dy;
            } else if (fbH < verts[i].y) {
                t = (fbH - verts[0].y) / dy;
            }
        }
        if (t < scale) {
            scale = t;
        }
    }

    if (scale == lbl_eu_8066B40C) {
        return lbl_eu_8066B40C;
    }

    for (s16 i = 1; i < 10; i++) {
        if (i < 9) {
            ml::CVec3 d = verts[i] - verts[0];
            d.x *= scale;
            d.y *= scale;
            d.z *= scale;
            verts[i] = verts[0] + d;
        } else {
            verts[i] = verts[1];
        }
    }
    return scale;
}

// func_804EFD78: clamp positions to the screen and drag texcoords/colors along.
extern "C" void func_804EFD78(ml::CVec3* verts, ml::CVec3* tex, CFanColor* colors) {
    float fbW = u16ToF(getRenderModeObj__9CDeviceVIFv()->fbWidth);
    float fbH = u16ToF(getRenderModeObj__9CDeviceVIFv()->efbHeight);

    for (s16 i = 1; i < 10; i++) {
        if (i >= 9) {
            verts[i] = verts[1];
            tex[i] = tex[1];
            colors[i] = colors[1];
            continue;
        }

        ml::CVec3 orig = verts[i];
        bool clamped = false;
        if (verts[i].x < lbl_eu_8066B408) {
            verts[i].x = lbl_eu_8066B408;
            clamped = true;
        } else if (fbW < verts[i].x) {
            verts[i].x = fbW;
            clamped = true;
        }
        if (verts[i].y < lbl_eu_8066B408) {
            verts[i].y = lbl_eu_8066B408;
            clamped = true;
        } else if (fbH < verts[i].y) {
            verts[i].y = fbH;
            clamped = true;
        }
        if (!clamped) {
            continue;
        }

        ml::CVec3 d = verts[0] - verts[i];
        float rx = (d.x == lbl_eu_8066B408)
                       ? lbl_eu_8066B40C
                       : ml::math::abs((verts[i].x - verts[0].x) / d.x);
        float ry = (d.y == lbl_eu_8066B408)
                       ? lbl_eu_8066B40C
                       : ml::math::abs((verts[i].y - verts[0].y) / d.y);

        ml::CVec3 tdiff = tex[i] - tex[0];
        tex[i].x = tex[0].x + tdiff.x * rx;
        tex[i].y = tex[0].y + tdiff.y * ry;
        tex[i].z = tex[0].z + tdiff.z * lbl_eu_8066B40C;
        if (tex[i].x < lbl_eu_8066B408) {
            tex[i].x = lbl_eu_8066B408;
        } else if (lbl_eu_8066B40C < tex[i].x) {
            tex[i].x = lbl_eu_8066B40C;
        }
        if (tex[i].y < lbl_eu_8066B408) {
            tex[i].y = lbl_eu_8066B408;
        } else if (lbl_eu_8066B40C < tex[i].y) {
            tex[i].y = lbl_eu_8066B40C;
        }

        ml::CVec3 cdiff((float)colors[i].r, (float)colors[i].g, (float)colors[i].b);
        cdiff.x -= colors[0].r;
        cdiff.y -= colors[0].g;
        cdiff.z -= colors[0].b;
        colors[i].r = colors[0].r + cdiff.x * rx;
        colors[i].g = colors[0].g + cdiff.y * ry;
        colors[i].b = colors[0].b + cdiff.z * lbl_eu_8066B40C;
        if (colors[i].r < lbl_eu_8066B408) {
            colors[i].r = lbl_eu_8066B408;
        } else if (lbl_eu_8066B40C < colors[i].r) {
            colors[i].r = lbl_eu_8066B40C;
        }
        if (colors[i].g < lbl_eu_8066B408) {
            colors[i].g = lbl_eu_8066B408;
        } else if (lbl_eu_8066B40C < colors[i].g) {
            colors[i].g = lbl_eu_8066B40C;
        }
    }
}

// func_804EECB0: GX renderer for the marker fan.
extern "C" u32 func_804EECB0(u32 texMapId, CDrawCtx* draw, const ml::CVec3* pos,
                             const float* color, const ml::CVec3* size, void* clampInfo,
                             s16 flag, CMarkerMtxSrc* mtxSrc, float angleDeg, float alpha,
                             float vertRot) {
    float fbW = u16ToF(getRenderModeObj__9CDeviceVIFv()->fbWidth);
    float fbH = u16ToF(getRenderModeObj__9CDeviceVIFv()->efbHeight);

    if (pos->x < lbl_eu_8066B408 || fbW < pos->x) {
        return 0;
    }
    if (pos->y < lbl_eu_8066B408 || fbH < pos->y) {
        return 0;
    }

    ml::CVec3 verts[10];
    if (flag < 0) {
        func_804EF830(verts);
    } else {
        const float z = lbl_eu_8066B408;
        const float p = lbl_eu_8066B40C;
        const float n = lbl_eu_8066B410;
        verts[0].set(z, z, z);
        verts[1].set(p, z, z);
        verts[2].set(p, p, z);
        verts[3].set(z, p, z);
        verts[4].set(n, p, z);
        verts[5].set(n, z, z);
        verts[6].set(n, n, z);
        verts[7].set(z, n, z);
        verts[8].set(p, n, z);
        verts[9].set(p, z, z);
    }

    func_804EF9B8(verts, pos, *size * lbl_eu_8066B414, vertRot);

    if (clampInfo != NULL) {
        alpha *= func_804EFB38(verts);
    }

    ml::CVec3 tex[10];
    fbW = u16ToF(getRenderModeObj__9CDeviceVIFv()->fbWidth);
    fbH = u16ToF(getRenderModeObj__9CDeviceVIFv()->efbHeight);
    for (s16 i = 0; i < 10; i++) {
        if (i < 9) {
            tex[i].x = verts[i].x / fbW;
            tex[i].y = verts[i].y / fbH;
            tex[i].z = lbl_eu_8066B408;
        } else {
            tex[i] = tex[1];
        }
    }

    CFanColor colors[10];
    memcpy(colors, lbl_eu_80661748, 0x78);

    if (clampInfo == NULL) {
        func_804EFD78(verts, tex, colors);
    }

    if (alpha > lbl_eu_8066B408) {
        float gfbW = u16ToF(getRenderModeObj__9CDeviceVIFv()->fbWidth);
        float rad = ml::deg2rad * angleDeg;
        float deg = ml::rad2deg * rad;
        float gfbH = u16ToF(getRenderModeObj__9CDeviceVIFv()->efbHeight);
        float ax = alpha * (size->x / gfbW);
        float ay = alpha * (size->y / gfbH);
        tex[0].x = tex[0].x + size->x * (ax * gradLookup(lbl_eu_80660038, deg)) / gfbW;
        tex[0].y = tex[0].y + size->y * (ay * gradLookup(lbl_eu_80660B78, deg)) / gfbH;
        if (tex[0].x < lbl_eu_8066B408) {
            tex[0].x = lbl_eu_8066B408;
        } else if (lbl_eu_8066B40C < tex[0].x) {
            tex[0].x = lbl_eu_8066B40C;
        }
        if (tex[0].y < lbl_eu_8066B408) {
            tex[0].y = lbl_eu_8066B408;
        } else if (lbl_eu_8066B40C < tex[0].y) {
            tex[0].y = lbl_eu_8066B40C;
        }
    }

    GXColor matColor;
    matColor.r = (u8)(lbl_eu_8066B420 * color[0]);
    matColor.g = (u8)(lbl_eu_8066B420 * color[1]);
    matColor.b = (u8)(lbl_eu_8066B420 * color[2]);
    matColor.a = (u8)(lbl_eu_8066B420 * color[3]);

    float h4628 = u16ToF(getRenderModeObj__9CDeviceVIFv()->efbHeight);
    float w4628 = u16ToF(getRenderModeObj__9CDeviceVIFv()->fbWidth);
    func_804F4628(1, w4628, h4628);

    Mtx ident;
    PSMTXIdentity(ident);
    GXLoadPosMtxImm(ident, GX_PNMTX0);
    GXSetCurrentMtx(GX_PNMTX0);
    GXInitTexObjFilter(draw->mTexObj, GX_LINEAR, GX_LINEAR);
    GXSetZMode(GX_FALSE, GX_ALWAYS, GX_FALSE);
    GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_NOOP);
    GXLoadTexObj(draw->mTexObj, (GXTexMapID)texMapId);

    if (flag < 0) {
        GXSetNumTexGens(1);
        GXSetNumTevStages(1);
        Mtx* texMtx = func_804F42A0(1, 0);
        GXLoadTexMtxImm((const float(*)[4])texMtx, GX_TEXMTX0, GX_MTX_2x4);
        GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_TEXMTX0, GX_FALSE,
                          0x7D);
        GXSetNumChans(1);
        GXSetChanCtrl(GX_COLOR0A0, GX_FALSE, GX_SRC_REG, GX_SRC_REG, GX_LIGHT_NULL, GX_DF_NONE,
                      GX_AF_NONE);
        GXColor amb;
        *(u32*)&amb = lbl_eu_8066B400;
        setChanAmbColor(GX_COLOR0A0, &amb);
        GXColor mat;
        *(u32*)&mat = *(u32*)&matColor;
        setChanMatColor(GX_COLOR0A0, &mat);
        GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, (GXTexMapID)texMapId, GX_COLOR0A0);
        GXSetTevOp(GX_TEVSTAGE0, GX_REPLACE);
        GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
        GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_TEX_S, GX_F32, 0);
        GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
        GXSetVtxDesc(GX_VA_CLR0, GX_NONE);
        GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);
        GXSetVtxDesc(GX_VA_TEX1, GX_NONE);
        GXBegin(GX_TRIANGLEFAN, GX_VTXFMT0, 10);
        for (int i = 0; i < 10; i++) {
            GXPosition3f32(verts[i].x, verts[i].y, verts[i].z);
            GXTexCoord2f32(tex[i].x, tex[i].y);
        }
    } else {
        if (mtxSrc == NULL) {
            GXSetNumTexGens(2);
            GXSetNumTevStages(2);
            Mtx* texMtx = func_804F42A0(1, 0);
            GXLoadTexMtxImm((const float(*)[4])texMtx, GX_TEXMTX0, GX_MTX_2x4);
            GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_TEXMTX0, GX_FALSE,
                              0x7D);
            GXSetTexCoordGen2(GX_TEXCOORD1, GX_TG_MTX2x4, GX_TG_TEX1, GX_IDENTITY, GX_FALSE,
                              0x7D);
        } else {
            GXSetNumTexGens(2);
            GXSetNumTevStages(2);
            Mtx* texMtx = func_804F42A0(1, 0);
            GXLoadTexMtxImm((const float(*)[4])texMtx, GX_TEXMTX0, GX_MTX_2x4);
            GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_TEXMTX0, GX_FALSE,
                              0x7D);
            mtxSrc->mTexMtx.setScale(mtxSrc->mScaleX, mtxSrc->mScaleY, mtxSrc->mScaleZ);
            mtxSrc->mTexMtx.replaceTranslation(
                ml::CVec3(mtxSrc->mField0C + lbl_eu_8066B408, mtxSrc->mField10 + lbl_eu_8066B408,
                          mtxSrc->mField14 + lbl_eu_8066B408));
            GXLoadTexMtxImm(mtxSrc->mTexMtx.mtx, GX_TEXMTX1, GX_MTX_2x4);
            GXSetTexCoordGen2(GX_TEXCOORD1, GX_TG_MTX2x4, GX_TG_TEX1, GX_TEXMTX1, GX_FALSE,
                              0x7D);
        }
        GXSetNumChans(1);
        GXSetChanCtrl(GX_COLOR0A0, GX_FALSE, GX_SRC_REG, GX_SRC_REG, GX_LIGHT_NULL, GX_DF_NONE,
                      GX_AF_NONE);
        GXColor amb;
        *(u32*)&amb = lbl_eu_8066B404;
        setChanAmbColor(GX_COLOR0A0, &amb);
        GXColor mat;
        *(u32*)&mat = *(u32*)&matColor;
        setChanMatColor(GX_COLOR0A0, &mat);
        GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, (GXTexMapID)texMapId, GX_COLOR0A0);
        GXSetTevOp(GX_TEVSTAGE0, GX_REPLACE);
        GXSetTevOrder(GX_TEVSTAGE1, GX_TEXCOORD1, (GXTexMapID)flag, GX_COLOR_NULL);
        setTevColorIn6(GX_TEVSTAGE1, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO, 0);
        GXSetTevColorOp(GX_TEVSTAGE1, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_FALSE, GX_TEVPREV);
        setTevAlphaIn6(GX_TEVSTAGE1, GX_CA_ZERO, GX_CA_APREV, GX_CA_TEXA, GX_CA_ZERO, 0);
        GXSetTevAlphaOp(GX_TEVSTAGE1, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
        GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
        GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_TEX_S, GX_F32, 0);
        GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX1, GX_TEX_S, GX_F32, 0);
        GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
        GXSetVtxDesc(GX_VA_CLR0, GX_NONE);
        GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);
        GXSetVtxDesc(GX_VA_TEX1, GX_DIRECT);
        GXBegin(GX_TRIANGLEFAN, GX_VTXFMT1, 10);
        for (int i = 0; i < 10; i++) {
            GXPosition3f32(verts[i].x, verts[i].y, verts[i].z);
            GXTexCoord2f32(tex[i].x, tex[i].y);
            GXTexCoord2f32(colors[i].r, colors[i].g);
        }
    }

    return 1;
}

// func_804EEB40: high-level marker draw entry point.
extern "C" void func_804EEB40(void* desktop, const ml::CVec3* pos, const float* color,
                              const ml::CVec3* size, void* clampInfo, void* material,
                              CMarkerMtxSrc* mtxSrc, float angleDeg, float alpha, float vertRot) {
    if (func_8044BE24__8CGXCacheFv(cacheInstance__9CDeviceGX) != 0) {
        return;
    }

    func_804D8B28(desktop);
    CDrawCtx draw;
    func_804D8B38(&draw);
    if (func_804D8B4C(&draw, desktop, material) != 0) {
        if (mtxSrc == NULL || mtxSrc->mTex == NULL || mtxSrc->mIndex < 0) {
            func_804D8C68(&draw, 0, 0);
            func_804EECB0(0, &draw, pos, color, size, clampInfo, -1, NULL, angleDeg, alpha,
                          vertRot);
        } else {
            func_804D8C68(&draw, 0, 0);
            if (mtxSrc->mTex != NULL) {
                func_804DF164(mtxSrc->mTex, mtxSrc->mIndex, 1, mtxSrc->mField08);
            }
            func_804EECB0(0, &draw, pos, color, size, clampInfo, 1, mtxSrc, angleDeg, alpha,
                          vertRot);
        }
        func_804D8C18(&draw);
    }
    func_804D8B30(desktop);
}

// sinit_804F01C8: static initializer for the per-vertex color table.
extern "C" void sinit_804F01C8() {
    const float o = lbl_eu_8066B40C; // 1.0
    const float z = lbl_eu_8066B408; // 0.0
    const float h = lbl_eu_8066B414; // 0.5
    float* t = lbl_eu_80661748;
    t[0] = h;
    t[1] = h;
    t[2] = z;
    t[3] = o;
    t[4] = h;
    t[5] = z;
    t[6] = o;
    t[7] = o;
    t[8] = z;
    t[9] = h;
    t[10] = o;
    t[11] = z;
    t[12] = z;
    t[13] = o;
    t[14] = z;
    t[15] = z;
    t[16] = h;
    t[17] = z;
    t[18] = z;
    t[19] = z;
    t[20] = z;
    t[21] = h;
    t[22] = z;
    t[23] = z;
    t[24] = o;
    t[25] = z;
    t[26] = z;
    t[27] = h;
    t[28] = h;
    t[29] = z;
}
