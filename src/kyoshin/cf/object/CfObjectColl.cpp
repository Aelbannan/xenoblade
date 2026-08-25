// Auto-scaffolded catalog TU for kyoshin/cf/object/CfObjectColl
// Replace stubs with high-level C/C++ during decomp.

#include "monolib/math/CVec3.hpp"
#include <nw4r/math.h>

#include "kyoshin/cf/object/CfObjectColl.hpp"
#include "kyoshin/cf/CfGameManagerData.hpp"  // H3 label-owner decl (lbl_eu_80663E14; lbl_eu_80663E24)

// Minimal local view: the shared CfGameManager.hpp declares func_80086B48 as
// a void member, but retail returns the frame delta in f1 (call sites add it
// straight into +0x154). The shared header is outside this session's writable
// scope, so re-declare the enclosing class here with the correct signature.
namespace cf {
class CfGameManager {
public:
    ~CfGameManager();
    static float func_80086B48();
};
} // namespace cf

// Retail callee keeps its flat, unmangled name (reloc-name fix, PLAN.md 17.6).
extern "C" u32 func_800AA2BC(u32 a, u32 b);
extern "C" u32 func_800AA714(const char* path);

struct CfObjIf {
    virtual void _v0008();
    virtual void _v000C();
    virtual void _v0010();
    virtual void _v0014();
    virtual void _v0018();
    virtual void _v001C();
    virtual void _v0020();
    virtual void _v0024();
    virtual void _v0028();
    virtual void _v002C();
    virtual void _v0030();
    virtual void _v0034();
    virtual void _v0038();
    virtual void _v003C();
    virtual void _v0040();
    virtual void _v0044();
    virtual void _v0048();
    virtual void _v004C();
    virtual void _v0050();
    virtual void _v0054();
    virtual void _v0058();
    virtual void _v005C();
    virtual void _v0060();
    virtual void _v0064();
    virtual void _v0068();
    virtual void _v006C();
    virtual void _v0070();
    virtual void vf0074();
    virtual void _v0078();
    virtual void _v007C();
    virtual void _v0080();
    virtual void _v0084();
    virtual void _v0088();
    virtual void _v008C();
    virtual void _v0090();
    virtual void _v0094();
    virtual void _v0098();
    virtual void vf009C();
    virtual void _v00A0();
    virtual void _v00A4();
    virtual void _v00A8();
    virtual void _v00AC();
    virtual void _v00B0();
    virtual void _v00B4();
};



// Null-guarded bit-15 flag test on +0x64; returns self when the flag is
// set (retail keeps r3 = self and tests with r0 via bclr 4,2).
extern "C" void* func_800AB3D0(void* self) {
    if (self && (*(u32*)((u8*)self + 0x64) & 0x8000))
        return self;
    return 0;
}

// Reset the +0x154 default value, then notify via virtual slot 0x70.
// Forced-name free function (retail symbol); const self so MWCC hoists the
// sdata2 lfs above the LR-save store (load-hoist family).
int func_800AB3EC__Q22cf12CfObjectCollFv(cf::CfObjectColl* self) {
    self->field_0x154 = lbl_eu_80666910;
    reinterpret_cast<CfObjIf*>(self)->_v0070();
    return true;
}

// Most-derived destructor: releases the resource via virtual slot 0x68, then
// the compiler runs the CfObjectPoint base destructor and the flag-guarded
// operator delete.
cf::CfObjectColl::~CfObjectColl() {
    reinterpret_cast<CfObjIf*>(this)->_v0068();
}

struct If158 {
    virtual void _v008(); virtual void _v00C(); virtual void _v010(); virtual void _v014();
    virtual void _v018(); virtual void _v01C(); virtual void _v020(); virtual void _v024();
    virtual void _v028(); virtual void _v02C(); virtual void _v030(); virtual void _v034();
    virtual void _v038(); virtual void _v03C(); virtual void _v040(); virtual void _v044();
    virtual void _v048(); virtual void _v04C(); virtual void _v050(); virtual void _v054();
    virtual void _v058(); virtual void _v05C(); virtual void _v060(); virtual void _v064();
    virtual void _v068(); virtual void _v06C(); virtual void _v070(); virtual void _v074();
    virtual void _v078(); virtual void _v07C(); virtual void _v080(); virtual void _v084();
    virtual void _v088(); virtual void _v08C(); virtual void _v090(); virtual void _v094();
    virtual void _v098(); virtual void _v09C(); virtual void _v0A0(); virtual void _v0A4();
    virtual void _v0A8(); virtual void _v0AC(); virtual void _v0B0(); virtual void _v0B4();
    virtual void _v0B8(); virtual void _v0BC(); virtual void _v0C0(); virtual void _v0C4();
    virtual void _v0C8(); virtual void _v0CC(); virtual void _v0D0(); virtual void _v0D4();
    virtual void _v0D8(); virtual void _v0DC(); virtual void _v0E0(); virtual void _v0E4();
    virtual void _v0E8(); virtual void _v0EC(); virtual void _v0F0(); virtual void _v0F4();
    virtual void _v0F8(); virtual void _v0FC(); virtual void _v100(); virtual void _v104();
    virtual void _v108(); virtual void _v10C(); virtual void _v110(); virtual void _v114();
    virtual void _v118(); virtual void _v11C(); virtual void _v120(); virtual void _v124();
    virtual void _v128(); virtual void _v12C(); virtual void _v130(); virtual void _v134();
    virtual void _v138(); virtual void _v13C(); virtual void _v140(); virtual void _v144();
    virtual void _v148(); virtual void _v14C(); virtual void _v150(); virtual void _v154();
    virtual void vf158(u32 a);
};
extern "C" void func_800AB498__Q22cf12CfObjectCollFv(cf::CfObjectColl* self) {
    reinterpret_cast<If158*>(self)->vf158(1);
}

// Interface view whose slot 0xAC returns a pointer to an embedded vector.
struct IfRetVec {
    virtual void _v008(); virtual void _v00C(); virtual void _v010(); virtual void _v014();
    virtual void _v018(); virtual void _v01C(); virtual void _v020(); virtual void _v024();
    virtual void _v028(); virtual void _v02C(); virtual void _v030(); virtual void _v034();
    virtual void _v038(); virtual void _v03C(); virtual void _v040(); virtual void _v044();
    virtual void _v048(); virtual void _v04C(); virtual void _v050(); virtual void _v054();
    virtual void _v058(); virtual void _v05C(); virtual void _v060(); virtual void _v064();
    virtual void _v068(); virtual void _v06C(); virtual void _v070(); virtual void _v074();
    virtual void _v078(); virtual void _v07C(); virtual void _v080(); virtual void _v084();
    virtual void _v088(); virtual void _v08C(); virtual void _v090(); virtual void _v094();
    virtual void _v098(); virtual void _v09C(); virtual void _v0A0(); virtual void _v0A4();
    virtual void _v0A8();
    virtual ml::CVec3* getVec(); // 0xAC
};

// Lazily bind each coll-impl singleton once, then return the packed resource
// pointer for field_0x98 in 1..5 (defaulting to the second instance slot).
// Retail callee keeps a flat, unmangled name (reloc-name fix, PLAN.md 17.6).
extern "C" void* CfObjectColl_initCollImplInstances(cf::CfObjectColl* self) {
    if (lbl_eu_80663EA0 == 0) {
        lbl_eu_80663EA4 = lbl_eu_80528820;
        lbl_eu_80663EA0 = 1;
    }
    if (lbl_eu_80663EA8 == 0) {
        lbl_eu_80663EAC = lbl_eu_80528800;
        lbl_eu_80663EA8 = 1;
    }
    if (lbl_eu_80663EB0 == 0) {
        lbl_eu_80663EB4 = lbl_eu_805287E0;
        lbl_eu_80663EB0 = 1;
    }
    if (lbl_eu_80663EB8 == 0) {
        lbl_eu_80663EBC = lbl_eu_805287C0;
        lbl_eu_80663EB8 = 1;
    }
    if (lbl_eu_80663EC0 == 0) {
        lbl_eu_80663EC4 = lbl_eu_805287A0;
        lbl_eu_80663EC0 = 1;
    }
    int id = self->field_0x98;
    if (1 <= id) {
        if (id < 6) {
            return lbl_eu_805285E8[id];
        }
    }
    return &lbl_eu_80663EAC;
}
void cf::CfObjectColl::func_800AB57C() {
    func_800C1638();
}

// Null-guarded bit-13 flag test on +0x68: reject when the object is marked
// as coll-disabled, optionally fetch the current vector through slot 0xAC,
// then run the resource load through the coll-impl singleton (slot 0xC) and
// drive the per-state +0x154 timer update.
struct IfLoader {
    virtual void _v0008();
    // slot 0xC also receives the live angle in f1
    virtual int load(cf::CfObjectColl* self, cf::CfObject* obj, ml::CVec3* out,
                     float angle);
};
int func_800AB580(cf::CfObjectColl* self, cf::CfObject* obj, ml::CVec3* out, float f1) {
    if (obj == NULL)
        return 0;
    u32 flags = self->field_0x68;
    // IBM bits 13/15 of the shared flag word.
    if (flags & 0x00040000)
        return 0;
    if (!(flags & 0x00010000) && out != NULL) {
        *out = *reinterpret_cast<IfRetVec*>(obj)->getVec();
        out = NULL; // vector already captured; loader receives a null sink
    }
    int ok = reinterpret_cast<IfLoader*>(CfObjectColl_initCollImplInstances(self))
                 ->load(self, obj, out, f1);
    // Flag bit 0: a successful load while the flag is set fails without
    // clearing; a failed load clears the flag.
    if (self->field_0x158 & 1) {
        if (ok != 0) {
            ok = 0;
        } else {
            self->field_0x158 &= ~1;
        }
    }
    if ((int)self->field_0x94 == 6) {
        if (ok != 0) {
            if (lbl_eu_80663E24 & 0x00200000) {
                self->field_0x154 = lbl_eu_8066694C;
            } else {
                self->field_0x154 += cf::CfGameManager::func_80086B48();
            }
            if (self->field_0x154 < lbl_eu_8066694C) {
                ok = 0;
            } else {
                self->field_0x154 = lbl_eu_8066694C;
            }
        } else {
            self->field_0x154 = lbl_eu_80666910;
        }
    } else if ((int)self->field_0x94 == 4) {
        if (ok != 0) {
            self->field_0x154 += cf::CfGameManager::func_80086B48();
            if (self->field_0x154 < lbl_eu_80666950) {
                ok = 0;
            } else {
                self->field_0x154 = lbl_eu_80666950;
            }
        } else {
            self->field_0x154 = lbl_eu_80666910;
        }
    }
    if (ok != 0 && ((int)self->field_0x94 == 1 || (int)self->field_0x94 == 6)) {
        if ((int)self->field_0x98 == 3) {
            self->field_0x150 = lbl_eu_80666910;
        } else {
            self->field_0x150 = func_800A4B5C(
                *reinterpret_cast<const ml::CVec3*>(&self->field_0xA0),
                *reinterpret_cast<const ml::CVec3*>(&self->field_0xAC),
                *reinterpret_cast<IfRetVec*>(obj)->getVec());
        }
    }
    return ok;
}


// Interface view with a two-argument slot at 0x9C (self + embedded param block).
struct IfArgs {
    virtual void _v008(); virtual void _v00C(); virtual void _v010(); virtual void _v014();
    virtual void _v018(); virtual void _v01C(); virtual void _v020(); virtual void _v024();
    virtual void _v028(); virtual void _v02C(); virtual void _v030(); virtual void _v034();
    virtual void _v038(); virtual void _v03C(); virtual void _v040(); virtual void _v044();
    virtual void _v048(); virtual void _v04C(); virtual void _v050(); virtual void _v054();
    virtual void _v058(); virtual void _v05C(); virtual void _v060(); virtual void _v064();
    virtual void _v068(); virtual void _v06C(); virtual void _v070(); virtual void _v074();
    virtual void _v078(); virtual void _v07C(); virtual void _v080(); virtual void _v084();
    virtual void _v088(); virtual void _v08C(); virtual void _v090(); virtual void _v094();
    virtual void _v098();
    virtual void fn39(void* param); // 0x9C
};

void cf::CfObjectColl::func_800AB7A8() {
    reinterpret_cast<CfObjIf*>(this)->_v00B4();
    reinterpret_cast<IfArgs*>(this)->fn39((u8*)this + 0x3c);
}

// Refresh resource through slot 0x9C, then copy both vector blocks, stamp the
// state words, stash four halfwords and reset +0x154 / raise flag bit 0.
void func_800AB7F8(cf::CfObjectColl* self, const cf::CollVec* a, const cf::CollVec* b,
                   u16 p4, u16 p5, u16 p6, u16 p7, float val) {
    reinterpret_cast<CfObjIf*>(self)->vf009C();
    u32 v0 = a->w0;
    self->field_0x98 = 4;
    self->field_0xA4 = a->w4;
    u16 flg = self->field_0x158;
    self->field_0x94 = 6;
    u32 t0 = b->w0;
    u32 t4 = b->w4;
    u32 v8 = a->w8;
    u32 t8 = b->w8;
    self->field_0xA0 = v0;
    self->field_0xA8 = v8;
    self->field_0xAC = t0;
    self->field_0xB0 = t4;
    self->field_0xB4 = t8;
    self->field_0xB8 = val;
    flg |= 1;
    self->field_0x15A = p5;
    self->field_0x15C = p4;
    self->field_0x15E = p7;
    self->field_0x160 = p6;
    self->field_0x154 = lbl_eu_80666910;
    self->field_0x158 = flg;
}

// Same vector-copy shape as func_800ABDE4, but stamps 0x94=1/0x98=5 and
// raises bit 0 of the flag halfword at +0x158.
void func_800AB8CC(cf::CfObjectColl* self, const cf::CollVec* a, const cf::CollVec* b, float val) {
    reinterpret_cast<CfObjIf*>(self)->vf009C();
    u32 v0 = a->w0;
    self->field_0xA4 = a->w4;
    u32 t0 = b->w0;
    u32 t4 = b->w4;
    u32 v8 = a->w8;
    u32 t8 = b->w8;
    self->field_0xA0 = v0;
    self->field_0xA8 = v8;
    self->field_0xAC = t0;
    self->field_0xB0 = t4;
    self->field_0xB4 = t8;
    self->field_0xB8 = val;
    self->field_0x98 = 5;
    self->field_0x94 = 1;
    self->field_0x158 |= 1;
}

// Copy two 12-byte vector blocks into the object and store the pair of
// scalars, after refreshing the packed resource through virtual slot 0x9C.
void func_800AB978(cf::CfObjectColl* self, cf::CollVec* vecA, cf::CollVec* vecB, float f1, float f2) {
    reinterpret_cast<CfObjIf*>(self)->vf009C();
    u32 t = vecA->w0;
    self->field_0xC0 = vecA->w4;
    u32 bw0 = vecB->w0;
    self->field_0xBC = t;
    u32 bw4 = vecB->w4;
    self->field_0xD4 = f1;
    u32 aw8 = vecA->w8;
    u32 bw8 = vecB->w8;
    self->field_0xC4 = aw8;
    self->field_0xC8 = bw0;
    self->field_0xCC = bw4;
    self->field_0xD0 = bw8;
    self->field_0x148 = f2;
}

// Copy vector A into the 0xA0 block / B into the 0xAC block with default
// scalar, refresh resource via slot 0x9C, negate/expand B into the 0xD8 block,
// build+invert the Y-rotation matrix at +0xF0, raise flag bit 0 and stamp
// 0x98=3 / 0x94=1.
// Copy vector A into both the 0xA0 and 0xAC blocks (retail quirk: the second
// block also reads A), store the default scalar, refresh resource via slot
// 0x9C, negate/expand B into the 0xD8 block, build+invert the Y-rotation
// matrix at +0xF0 using the translation from A, raise flag bit 0 and stamp
// 0x98=3 / 0x94=1. Pointers stay non-const: retail reloads A's words between
// the two block copies, which requires potential aliasing.
void func_800ABA18(cf::CfObjectColl* self, cf::CollVec* a, const ml::CVec3* ext,
                   float angle) {
    u32 v0 = a->w0;
    self->field_0xA4 = a->w4;
    self->field_0xA0 = v0;
    self->field_0xA8 = a->w8;
    u32 t0 = a->w0;
    self->field_0xB0 = a->w4;
    self->field_0xAC = t0;
    self->field_0xB4 = a->w8;
    self->field_0xB8 = lbl_eu_80666910;
    reinterpret_cast<CfObjIf*>(self)->vf009C();
    Mtx& m = self->field_0xF0;
    self->field_0xD8 = -ext->x;
    self->field_0xDC = lbl_eu_80666910;
    self->field_0xE0 = -ext->z;
    self->field_0xE4 = ext->x;
    self->field_0xE8 = ext->y;
    self->field_0xEC = ext->z;
    self->field_0x14C = lbl_eu_80666910;
    f32 s = nw4r::math::SinFIdx(lbl_eu_80666928 * angle);
    f32 c = nw4r::math::CosFIdx(lbl_eu_80666928 * angle);
    m[0][0] = c;
    m[0][1] = lbl_eu_80666910;
    m[0][2] = s;
    m[0][3] = lbl_eu_80666910;
    m[1][0] = lbl_eu_80666910;
    m[1][1] = lbl_eu_80666914;
    m[1][2] = lbl_eu_80666910;
    m[1][3] = lbl_eu_80666910;
    m[2][0] = -s;
    m[2][1] = lbl_eu_80666910;
    m[2][2] = c;
    m[2][3] = lbl_eu_80666910;
    ml::CVec3* pos = reinterpret_cast<ml::CVec3*>(a);
    m[0][3] = pos->x;
    m[1][3] = pos->y;
    m[2][3] = pos->z;
    PSMTXInverse(m, m);
    self->field_0x98 = 3;
    self->field_0x94 = 1;
    self->field_0x158 |= 1;
}

// Lerp the stored vector pair A'(0xBC)..B'(0xC8) toward B' by the factor at
// +0x150 and write the result through the out pointer.
void func_800ABB9C(ml::CVec3* out, cf::CfObjectColl* self) {
    ml::CVec3 diff = *reinterpret_cast<ml::CVec3*>(&self->field_0xC8)
                   - *reinterpret_cast<ml::CVec3*>(&self->field_0xBC);
    ml::CVec3 step = diff * self->field_0x150;
    out->set(*reinterpret_cast<ml::CVec3*>(&self->field_0xBC) + step);
}

// State 3 fetches the current vector through slot 0xAC; otherwise the result
// is the scaled sum of the two stored vectors (0xA0 / 0xAC).
void func_800ABC5C(ml::CVec3* out, cf::CfObjectColl* self) {
    if ((int)self->field_0x98 == 3) {
        out->set(*reinterpret_cast<IfRetVec*>(self)->getVec());
    } else {
        ml::CVec3 res = *reinterpret_cast<ml::CVec3*>(&self->field_0xA0)
                      + *reinterpret_cast<ml::CVec3*>(&self->field_0xAC);
        ml::CVec3 scaled = res * lbl_eu_80666930;
        out->set(scaled);
    }
}

// Copy two 12-byte vector blocks into the 0xA0/0xAC region, store the scalar,
// then stamp the state marker words at 0x94/0x98.
void func_800ABD44(cf::CfObjectColl* self, cf::CollVec* a, cf::CollVec* b, float val) {
    reinterpret_cast<CfObjIf*>(self)->vf009C();
    // Retail interleaves each load with the following store instead of
    // batching all six loads up front.
    u32 v0 = a->w0;
    u32 v4 = a->w4;
    self->field_0xA4 = v4;
    u32 t0 = b->w0;
    self->field_0xA0 = v0;
    u32 t4 = b->w4;
    u32 t8 = b->w8;
    u32 v8 = a->w8;
    self->field_0xA8 = v8;
    self->field_0xAC = t0;
    self->field_0xB0 = t4;
    self->field_0xB4 = t8;
    self->field_0xB8 = val;
    self->field_0x98 = 4;
    self->field_0x94 = 0;
}

// Copy two 12-byte vector blocks into the 0xA0/0xAC region, store the scalar,
// then stamp the state marker words at 0x94/0x98.
void func_800ABDE4(cf::CfObjectColl* self, const cf::CollVec* a, const cf::CollVec* b, float val) {
    reinterpret_cast<CfObjIf*>(self)->vf009C();
    u32 v0 = a->w0;
    self->field_0xA4 = a->w4;
    u32 t0 = b->w0;
    u32 t4 = b->w4;
    u32 v8 = a->w8;
    u32 t8 = b->w8;
    self->field_0xA0 = v0;
    self->field_0xA8 = v8;
    self->field_0xAC = t0;
    self->field_0xB0 = t4;
    self->field_0xB4 = t8;
    self->field_0xB8 = val;
    self->field_0x98 = 4;
    self->field_0x94 = 2;
}

void func_800ABE84(cf::CfObjectColl* self, const cf::CollVec* a, const cf::CollVec* b, float val) {
    reinterpret_cast<CfObjIf*>(self)->vf009C();
    u32 v0 = a->w0;
    self->field_0xA4 = a->w4;
    u32 t0 = b->w0;
    u32 t4 = b->w4;
    u32 v8 = a->w8;
    u32 t8 = b->w8;
    self->field_0xA0 = v0;
    self->field_0xA8 = v8;
    self->field_0xAC = t0;
    self->field_0xB0 = t4;
    self->field_0xB4 = t8;
    self->field_0xB8 = val;
    self->field_0x98 = 4;
    self->field_0x94 = 3;
}

void func_800ABF24(cf::CfObjectColl* self, const cf::CollVec* a, const cf::CollVec* b, float val) {
    reinterpret_cast<CfObjIf*>(self)->vf009C();
    u32 v0 = a->w0;
    self->field_0xA4 = a->w4;
    u32 t0 = b->w0;
    u32 t4 = b->w4;
    u32 v8 = a->w8;
    u32 t8 = b->w8;
    self->field_0xA0 = v0;
    self->field_0xA8 = v8;
    self->field_0xAC = t0;
    self->field_0xB0 = t4;
    self->field_0xB4 = t8;
    self->field_0xB8 = val;
    self->field_0x98 = 4;
    self->field_0x94 = 5;
}

// Refresh resource via slot 0x9C, negate/expand the extent vector into the
// 0xD8 block, build a Y-rotation matrix at +0xF0 (angle scaled to FIdx space),
// invert it in place, then stamp the state words 0x98=3 / 0x94=5.
void func_800ABFC4(cf::CfObjectColl* self, const ml::CVec3* pos, const ml::CVec3* ext, float angle) {
    reinterpret_cast<CfObjIf*>(self)->vf009C();
    ml::CVec3 e = *ext;
    self->field_0xD8 = -e.x;
    self->field_0xDC = lbl_eu_80666910;
    self->field_0xE0 = -e.z;
    self->field_0xE4 = e.x;
    self->field_0xE8 = e.y;
    self->field_0xEC = e.z;
    self->field_0x14C = angle;
    f32 s = nw4r::math::SinFIdx(lbl_eu_80666928 * angle);
    f32 c = nw4r::math::CosFIdx(lbl_eu_80666928 * angle);
    Mtx& m = self->field_0xF0;
    m[0][0] = c;
    m[0][1] = lbl_eu_80666910;
    m[0][2] = s;
    m[0][3] = lbl_eu_80666910;
    m[1][0] = lbl_eu_80666910;
    m[1][1] = lbl_eu_80666914;
    m[1][2] = lbl_eu_80666910;
    m[1][3] = lbl_eu_80666910;
    m[2][0] = -s;
    m[2][1] = lbl_eu_80666910;
    m[2][2] = c;
    m[2][3] = lbl_eu_80666910;
    m[0][3] = pos->x;
    m[1][3] = pos->y;
    m[2][3] = pos->z;
    PSMTXInverse(m, m);
    self->field_0x98 = 3;
    self->field_0x94 = 5;
}

// Reset the +0x154 default, refresh the resource via virtual slot 0x9C, then
// copy two 12-byte vector blocks into the 0xA0/0xAC region with marker 4/4.
void func_800AC110(cf::CfObjectColl* self, const cf::CollVec* a, const cf::CollVec* b, float val) {
    self->field_0x154 = lbl_eu_80666910;
    reinterpret_cast<CfObjIf*>(self)->vf009C();
    u32 v0 = a->w0;
    self->field_0xA4 = a->w4;
    u32 t0 = b->w0;
    u32 t4 = b->w4;
    u32 v8 = a->w8;
    u32 t8 = b->w8;
    self->field_0xA0 = v0;
    self->field_0xA8 = v8;
    self->field_0xAC = t0;
    self->field_0xB0 = t4;
    self->field_0xB4 = t8;
    self->field_0xB8 = val;
    self->field_0x98 = 4;
    self->field_0x94 = 4;
}

// Refresh resource via slot 0x9C, negate/expand the extent vector into the
// 0xD8 block, reset the +0x154 timer, build a Y-rotation matrix at +0xF0
// (angle scaled to FIdx space), invert it in place, then stamp 0x98=3 / 0x94=4.
// pos is intentionally non-const: retail relies on potential aliasing with
// the matrix block, which keeps both translation stores alive.
void func_800AC1BC(cf::CfObjectColl* self, ml::CVec3* pos, const ml::CVec3* ext,
                   float angle) {
    reinterpret_cast<CfObjIf*>(self)->vf009C();
    Mtx& m = self->field_0xF0;
    ml::CVec3 e;
    e.x = ext->x;
    e.y = ext->y;
    e.z = ext->z;
    self->field_0xD8 = -e.x;
    self->field_0xDC = lbl_eu_80666910;
    self->field_0xE0 = -e.z;
    self->field_0xE4 = e.x;
    self->field_0xE8 = e.y;
    self->field_0xEC = e.z;
    self->field_0x14C = angle;
    self->field_0x154 = lbl_eu_80666910;
    f32 s = nw4r::math::SinFIdx(lbl_eu_80666928 * angle);
    f32 c = nw4r::math::CosFIdx(lbl_eu_80666928 * angle);
    m[0][0] = c;
    m[0][1] = lbl_eu_80666910;
    m[0][2] = s;
    m[0][3] = lbl_eu_80666910;
    m[1][0] = lbl_eu_80666910;
    m[1][1] = lbl_eu_80666914;
    m[1][2] = lbl_eu_80666910;
    m[1][3] = lbl_eu_80666910;
    m[2][0] = -s;
    m[2][1] = lbl_eu_80666910;
    m[2][2] = c;
    m[2][3] = lbl_eu_80666910;
    m[0][3] = pos->x;
    m[1][3] = pos->y;
    m[2][3] = pos->z;
    PSMTXInverse(m, m);
    self->field_0x98 = 3;
    self->field_0x94 = 4;
}

// Resolve the packed resource id from the (a, b) pair via the shared helper,
// then format the display name from the retail format string.
void func_800AC30C(cf::CfObjectColl* self, u32 a, u32 b) {
    self->field_0x9C = func_800AA2BC(a, b);
    self->fieldName120.format(lbl_eu_804FC134, a, b);
}

// Set or clear the indexed FixStr<16> entry in the name array that starts at
// offset 0x120 (each entry is 0x14 bytes: 16-char buffer + length word).
void func_800AC378(cf::CfObjectColl* self, const char* name, int index) {
    // Indexed name-table entry (each slot is 0x14 bytes); the 0x120 field
    // offset is left in the store displacements rather than folded into the
    // entry pointer.
    if (name != NULL) {
        (&self->fieldName120)[index].mLength = strlen(name);
        strcpy((&self->fieldName120)[index].mString, name);
    } else {
        (&self->fieldName120)[index].mString[0] = '\0';
        (&self->fieldName120)[index].mLength = 0;
    }
}

// Resolve the resource id for the given path, then format its display name
// into the embedded FixStr<16> using the suffix of the retail format string.
void func_800AC3F4(cf::CfObjectColl* self, const char* name) {
    self->field_0x9C = func_800AA714(name);
    self->fieldName120.format(lbl_eu_804FC134 + 0xa, name);
}

void func_800AC450(void* self, unsigned long a, unsigned long b) {
    *(unsigned long*)((char*)self + 0x9c) = (a << 16) + b;
}

unsigned long func_800AC460(void* self) {
    extern unsigned long func_8009D018(unsigned long);
    unsigned long v = *(unsigned long*)((char*)self + 0x9c);
    return func_8009D018((v >> 16) + 0x20c8);
}

// Sign test on a call through the u16 index at +0x9C (offset 0x20C8 table).
extern "C" int func_8009CF8C(u32 resourceId);
extern "C" bool func_800AC470(void* self) {
    return func_8009CF8C((*(u32*)((u8*)self + 0x9C) >> 16) + 0x20C8) != 0;
}

extern "C" void func_800AC4A8(void* self, u32 val) { *(u32*)((u8*)self + 0x9C) = val; }

extern const float lbl_eu_80666914;
extern "C" float CfObject_UnkVirtualFunc73__Q22cf8CfObjectFv() { return lbl_eu_80666914; }

extern "C" void CfObject_UnkVirtualFunc72__Q22cf8CfObjectFv() {}

extern const float lbl_eu_80666910;
extern "C" float CfObject_UnkVirtualFunc71__Q22cf8CfObjectFv() { return lbl_eu_80666910; }

extern "C" void CfObject_UnkVirtualFunc70__Q22cf8CfObjectFv() {}

extern "C" int CfObject_UnkVirtualFunc69__Q22cf8CfObjectFv(cf::CfObject* self) { return 0; }

// Retail symbol is Fv even though the body reads r4; defined at global scope
// under the retail-mangled identifier with an explicit (self, flag) pair.
// Set/clear bit 22 of the shared flag word at +0x68.
void CfObject_UnkVirtualFunc67__Q22cf8CfObjectFv(cf::CfObject* self, int flag) {
    if (flag != 0) {
        self->field_0x68 |= 0x00400000;
    } else {
        self->field_0x68 &= ~0x00400000;
    }
}

// Set/clear bit 25 of the shared flag word at +0x68.
void CfObject_UnkVirtualFunc65__Q22cf8CfObjectFv(cf::CfObject* self, int flag) {
    if (flag != 0) {
        self->field_0x68 |= 0x02000000;
    } else {
        self->field_0x68 &= ~0x02000000;
    }
}

extern "C" int CfObject_UnkVirtualFunc62__Q22cf8CfObjectFv(cf::CfObject* self) { return 0; }

extern "C" void CfObject_UnkVirtualFunc61__Q22cf8CfObjectFv() {}

extern const float lbl_eu_80666910;
extern "C" float CfObject_UnkVirtualFunc60__Q22cf8CfObjectFv() { return lbl_eu_80666910; }

extern "C" void CfObject_UnkVirtualFunc59__Q22cf8CfObjectFv() {}

extern "C" void CfObject_UnkVirtualFunc57__Q22cf8CfObjectFv() {}

extern "C" int CfObject_UnkVirtualFunc51__Q22cf8CfObjectFv(cf::CfObject* self) { return -1; }

extern "C" void CfObject_UnkVirtualFunc45__Q22cf8CfObjectFv() {}

extern "C" int CfObject_UnkVirtualFunc42__Q22cf8CfObjectFv(cf::CfObject* self) { return 0; }

extern "C" int CfObject_UnkVirtualFunc43__Q22cf8CfObjectFv(cf::CfObject* self) { return 0; }

extern "C" int CfObject_UnkVirtualFunc44__Q22cf8CfObjectFv(cf::CfObject* self) { return 0; }

extern "C" int CfObject_UnkVirtualFunc39__Q22cf8CfObjectFv(cf::CfObject* self) { return 0; }

extern "C" int CfObject_UnkVirtualFunc40__Q22cf8CfObjectFv(cf::CfObject* self) { return 0; }

extern "C" int CfObject_UnkVirtualFunc41__Q22cf8CfObjectFv(cf::CfObject* self) { return 0; }

extern "C" int CfObject_UnkVirtualFunc38__Q22cf8CfObjectFv(cf::CfObject* self) { return 0; }

extern "C" int CfObject_UnkVirtualFunc37__Q22cf8CfObjectFv(cf::CfObject* self) { return 0; }

extern "C" float CfObject_UnkVirtualFunc36__Q22cf8CfObjectFv(cf::CfObject* self) { return *(float*)((u8*)self + 0x60); }

extern "C" void CfObject_UnkVirtualFunc35__Q22cf8CfObjectFv(cf::CfObject* self, float val) { *(float*)((u8*)self + 0x60) = val; }

extern "C" void* CfObject_UnkVirtualFunc28__Q22cf8CfObjectFv(cf::CfObject* self) { return (void*)((u8*)self + 0x48); }

extern "C" void* CfObject_UnkVirtualFunc24__Q22cf8CfObjectFv(cf::CfObject* self) { return (void*)((u8*)self + 0x3c); }

extern "C" void CfObject_UnkVirtualFunc21__Q22cf8CfObjectFv(cf::CfObject* self, float a, float b, float c) {
    *(float*)((char*)self + 0x3c) = a;
    *(float*)((char*)self + 0x40) = b;
    *(float*)((char*)self + 0x44) = c;
}

extern "C" int CfObject_UnkVirtualFunc18__Q22cf8CfObjectFv(cf::CfObject* self) { return 1; }

extern const float lbl_eu_80666910;
extern "C" float CfObject_UnkVirtualFunc17__Q22cf8CfObjectFv() { return lbl_eu_80666910; }

extern "C" void* CfObject_UnkVirtualFunc16__Q22cf8CfObjectFv(cf::CfObject* self) { return self; }

extern const float lbl_eu_80666910;
extern "C" float CfObject_UnkVirtualFunc15__Q22cf8CfObjectFv() { return lbl_eu_80666910; }

void* cf::CfObject::CfObject_UnkVirtualFunc14(float value) { return this; }

extern "C" int CfObject_UnkVirtualFunc13__Q22cf8CfObjectFv(cf::CfObject* self) { return 1; }

extern "C" void CfObject_UnkVirtualFunc12__Q22cf8CfObjectFv() {}

int cf::CfObject::CfObject_UnkVirtualFunc11() {
    return *(u32*)((u8*)this + 0x6C) & 1;
}

void cf::CfObject::CfObject_UnkVirtualFunc10() {
    void** vtable = *(void***)this;
    void (*func)(void*) = (void (*)(void*))vtable[0x74 / 4];
    func(this);
}

extern "C" void CfObject_UnkVirtualFunc5__Q22cf8CfObjectFv() {}

int cf::CfObjectPoint::func_800AC604() {
    return (field_0x68 >> 20) & 1;
}

extern "C" void* func_800AC610(void* param_1) {
    if (param_1 != NULL && (*(unsigned int*)((char*)param_1 + 0x64) & 0x20)) {
        return param_1;
    }
    return NULL;
}

extern "C" void func_800AB798__Q22cf12CfObjectCollFv(cf::CfObjectColl* self) { reinterpret_cast<CfObjIf*>(self)->vf009C(); }

extern "C" void CfObject_UnkVirtualFunc10__Q22cf8CfObjectFv(cf::CfObject* self) { reinterpret_cast<CfObjIf*>(self)->vf0074(); }
