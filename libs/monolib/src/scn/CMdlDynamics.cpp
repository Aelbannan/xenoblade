// Auto-scaffolded catalog TU for monolib/src/scn/CMdlDynamics
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>

#include <nw4r/math/math_types.h>
#include <nw4r/db/db_assert.h>
#include <nw4r/g3d/res/g3d_resdict.h>

#include <nw4r/math/math_arithmetic.h>
#include <nw4r/math/math_triangular.h>
#include <nw4r/g3d/res/g3d_resmdl.h>

#include <monolib/util/MemManager.hpp>
#include <monolib/math/CVec3.hpp>
#include <monolib/core/code_804E36DC.hpp> // func_80496288 frame-delta query (C ABI)
#include "libs/monolib/src/scn/CScnItemModel.hpp"

// --- Cross-TU retail data imports (global scope: plain extern, no extern "C") ---
extern const char lbl_eu_80530D54[]; // panic file name
// --- panic message ---
extern const char lbl_eu_80530D2C[];

// --- panic strings for the getters below (retail names) ---
// Fixed-size externs for the .sdata strings: MWCC addresses small (<=8B)
// extern arrays via sda21 (`li rN, sym@sda21`); incomplete arrays get lis/addi.
extern const char lbl_eu_8056E194[];  // panic file name (func_804EC418)
extern const char lbl_eu_8056E178[];  // panic message
extern const char lbl_eu_80663910[8]; // panic format arg (.sdata, sda21)
extern char lbl_eu_80663CBC[4]; // panic format arg (.sdata, sda21)
extern const char lbl_eu_80530DC4[];  // panic file name (func_804EB4C0)
extern const char lbl_eu_80530DA8[];  // panic message
extern const char lbl_eu_80530D68[];  // panic format arg
extern char lbl_eu_80663CC8[8]; // panic format arg (.sdata, sda21)
extern const char lbl_eu_8056E1C8[];  // panic file name (func_804E9FD0)
extern const char lbl_eu_8056E1A8[];  // panic message
extern const char lbl_eu_80529678[];  // panic file name (func_804EC344)
extern const char lbl_eu_80529658[];  // panic message
extern const char lbl_eu_8056E850[];  // panic file name (func_804EA038)
extern const char lbl_eu_8056E834[];  // panic message (func_804EA038)
extern char lbl_eu_80663CB8[4]; // panic format arg (.sdata, sda21, func_804EA038)
extern const char lbl_eu_8056E820[];  // panic file name (func_804EA038)
extern const char lbl_eu_8056E7F8[];  // panic message (func_804EA038)
extern const char lbl_eu_80530D18[];  // panic file name (func_804EB22C)
extern const char lbl_eu_80530CFC[];  // panic message (func_804EB22C)
extern const char lbl_eu_80530CF0[];  // panic message (func_804EB22C)
extern char lbl_eu_80663CC0[4]; // panic format arg (.sdata, sda21, func_804EB22C)
extern const char lbl_eu_80530F08[];  // panic file name (func_804EB22C)
extern const char lbl_eu_80530EE0[];  // panic message (func_804EB22C)
extern const char lbl_eu_80530E74[];  // panic file name (func_804EB310)
extern const char lbl_eu_80530E30[];  // panic message (func_804EB310)
extern const char lbl_eu_80530D94[];  // panic file name (func_804EB310)
extern const char lbl_eu_80530D78[];  // panic message (func_804EB310)
extern const char lbl_eu_80530E1C[];  // panic file name (func_804EB3E8)
extern const char lbl_eu_80530DD8[];  // panic message (func_804EB3E8)
extern char lbl_eu_80663CC4[4]; // panic format arg (.sdata, sda21, func_804EB310)

// --- FSqrt-style normalize warnings (func_804EC8AC) and asin-domain warning
// --- (func_804EC514); rodata strings addressed via lis/addi.
extern const char lbl_eu_80526324[]; // warning file
extern const char lbl_eu_80526300[]; // warning message
extern const char lbl_eu_8052ADB0[]; // asin-domain warning file
extern const char lbl_eu_8052AD88[]; // asin-domain warning message

// --- .sdata2 constants ---
extern const float lbl_eu_8066B3E8;  // -1.0f (asin domain low bound)
extern const float lbl_eu_8066A200;  // pitch singularity threshold
extern const float lbl_eu_8066B3EC;  // radians-per-FIdx scale
extern const float lbl_eu_8066B3F4;  // impulse scale (targets 4/5)
extern const float lbl_eu_8066B3E4;  // contact-point scale (pos sum, kinds 1/2/3)
extern const float lbl_eu_8066B3F8;  // velocity relax scale (entry physics)
extern const float lbl_eu_8066B3FC;  // position relax scale (entry physics)

// g3d ResMdl node lookup (member call emits this exact retail symbol).
extern unsigned long GetResNode__Q34nw4r3g3d6ResMdlCFUl(nw4r::g3d::ResMdl* mdl,
                                                       unsigned long idx);

// CMdlDynamics retail vtable (3 entries, 0xC bytes) lives in retail .data.
struct CMdlDynamicsVtbl {
    u32 entry0;
    u32 entry1;
    u32 entry2;
};
extern "C" void __dt__12CMdlDynamicsFv(void* self, int flags);
extern "C" u32 lbl_eu_80663CB0[2]; // .sdata RTTI locator (defined bottom)
extern "C" u32 lbl_eu_805701FC[19]; // .data vtable + strings (defined bottom)

// Retail nw4r math kernels (PS-asm bodies in nw4r/src/math/math_types.cpp;
// math_types.h only declares the inline PSMTXConcat-based MTX34Mult overload).
namespace nw4r {
namespace math {
MTX34* MTX34Add(MTX34* pOut, const MTX34* pA, const MTX34* pB);
MTX34* MTX34Mult(MTX34* pOut, const MTX34* pIn, f32 factor);
}
}

// operator delete (retail symbol __dl__FPv) is invoked via ::operator delete
// below so MWCC emits the correct already-mangled name.

// Buffer-holder object torn down by the address-named destructors below
// (retail symbols carry no class name). field_0xC == 0xFFFFFFFF marks an
// unallocated buffer; otherwise field_0x0 is released via MemManager.
struct CMdlDynBuffer {
    u8* field_0x0; // 0x0 heap buffer
    u32 field_0x4; // 0x4
    u32 field_0x8; // 0x8
    u32 field_0xC; // 0xC allocation sentinel
};

class CMdlDynamics {
public:
    CMdlDynamics();

    CMdlDynamicsVtbl* vtbl; // 0x0
    u32 field_0x4;          // 0x4
    u32 field_0x8;          // 0x8
    u32 field_0xC;          // 0xC
    u32 field_0x10;         // 0x10 (not set by ctor)
    u32 field_0x14;         // 0x14
    u32 field_0x18;         // 0x18
    u32 field_0x1C;         // 0x1C
    u32 field_0x20;         // 0x20 (not set by ctor)
    u32 field_0x24;         // 0x24
};

CMdlDynamics::CMdlDynamics() {
    vtbl = (CMdlDynamicsVtbl*)lbl_eu_805701FC;
    field_0x4 = 0;
    field_0x8 = 0;
    field_0xC = 0;
    field_0x14 = -1;
    field_0x18 = 0;
    field_0x1C = 0;
    field_0x24 = -1;
}

// func_804E9FC8 (inline getter at 0x804E9FC8) defined once - all harness stubs
// mapping to the same symbol reference it rather than redefining.
u32 func_804E9FC8(u8* self) { return *(u32*)((u8*)self + 0x146c); }

// (mapped to func_804E9FC8 above)

// (mapped to func_804E9FC8 above)

// (mapped to func_804E9FC8 above)

// (mapped to func_804E9FC8 above)

// (mapped to func_804E9FC8 above)

// (mapped to func_804E9FC8 above)

// (mapped to func_804E9FC8 above)

// (mapped to func_804E9FC8 above)

extern "C" void func_804EA0D4(u8* self, const void* src) { *(u32*)self = *(const u32*)src; }

u32 func_804EA0E0(u8* self) { return *(u32*)((u8*)self + 0x4); }

// (mapped to func_804E9FC8 above)

// us-804ed91c: deleting destructor for the dynamic buffer holder (retail keeps
// the full MWCC scaffold: null check, r4 > 0 delete flag, return this).
void* __dt__804E9460(CMdlDynBuffer* self, s32 freeIt) {
    if (self != 0) {
        self->field_0x4 = 0;
        if (self->field_0xC != 0xFFFFFFFFu) {
            if (self->field_0x0 != 0) {
                mtl::MemManager::deallocate(self->field_0x0);
                self->field_0x0 = 0;
            }
        }
        self->field_0x0 = 0;
        self->field_0x8 = 0;
        self->field_0xC = 0xFFFFFFFFu;
        if (freeIt > 0) {
            ::operator delete(self);
        }
    }
    return self;
}

// us-804ee68c: deleting destructor (same layout/teardown as __dt__804E9460).
void* __dt__804EA1D0(CMdlDynBuffer* self, s32 freeIt) {
    if (self != 0) {
        self->field_0x4 = 0;
        if (self->field_0xC != 0xFFFFFFFFu) {
            if (self->field_0x0 != 0) {
                mtl::MemManager::deallocate(self->field_0x0);
                self->field_0x0 = 0;
            }
        }
        self->field_0x0 = 0;
        self->field_0x8 = 0;
        self->field_0xC = 0xFFFFFFFFu;
        if (freeIt > 0) {
            ::operator delete(self);
        }
    }
    return self;
}

u32 func_804EA268(u8* self) { return *(u32*)((u8*)self + 0x0); }

extern "C" void* func_804EA270(u8* self) { return (char*)*(void**)self + (*(u32*)((u8*)self + 4) << 2); }

void func_804EA284(){}

// Thin wrappers over the SDK matrix functions (header-declared via <revolution/MTX.h>).
extern "C" void func_804EAA10(const Mtx a, Mtx b) {
    PSMTXInverse(a, b);
}

void func_804EAA14(void) {}

void func_804EAA18(){}

extern "C" u32 func_804EB1C4(u8* self) { return *(u32*)self != 0; }

// Stores a value at self+0 and asserts it is 4-byte aligned (retail panics
// with the same file/line/message as the nw4r ResDic alignment checks).
u8* func_804EB1D8(u8* self, u32 arg) {
    *(u32*)self = arg;
    if ((arg & 3) != 0) {
        nw4r::db::Panic(lbl_eu_80530D54, 0x26, lbl_eu_80530D2C);
    }
    return self;
}

// 0xB0-byte element of a dynamic-model array (same stride as func_804EB6F8).
struct CMdlDynElem {
    u8 field_0x0[0x14];          // 0x0..0x13
    f32 minX;                    // 0x14 (AABB lower bound, clamp target)
    f32 maxX;                    // 0x18
    f32 minY;                    // 0x1C
    f32 maxY;                    // 0x20
    f32 minZ;                    // 0x24
    f32 maxZ;                    // 0x28
    u8 field_0x2C[0x30];         // 0x2C..0x5B
    nw4r::math::VEC3 field_0x5C; // 0x5C..0x67
    nw4r::math::VEC3 field_0x68; // 0x68..0x73
    nw4r::math::VEC3 field_0x74; // 0x74..0x7F accumulated velocity
    nw4r::math::VEC3 field_0x80; // 0x80..0x8B
    u32 field_0x8C;              // 0x8C (dynamic flag bits)
    CMdlDynBuffer buf90;         // 0x90..0x9F (embedded buffer 1)
    CMdlDynBuffer bufA0;         // 0xA0..0xAF (embedded buffer 2)
};

// Dynamic-model list: base pointer of the 0xB0-stride array + element count.
struct CMdlDynList {
    CMdlDynElem* field_0x0; // 0x0 base of the 0xB0-stride element array
    u32 field_0x4;          // 0x4 element count
    u32 field_0x8;          // 0x8
    u32 field_0xC;          // 0xC allocation sentinel (0xFFFFFFFF = not owned)
};

// Object owning the list-pointer array (func_804EB798).
struct CMdlDynSet {
    u8 field_0x0[0x8];
    CMdlDynList** field_0x8; // 0x8
    u32 field_0xC;           // 0xC
};

// Sub-object reachable from the holder's field_0x0 (getters below).
struct CMdlDynSub {
    u8 field_0x0[0x4];   // 0x0..0x3
    u32 field_0x4;       // 0x4 (offset to sub-data)
    u32 field_0x8;       // 0x8
    u32 field_0xC;       // 0xC
    u32 field_0x10;      // 0x10
    u8 field_0x14[0x24]; // 0x14..0x37
    u8 field_0x38;       // 0x38
    u8 field_0x39[0x23]; // 0x39..0x5B
    u32 field_0x5C;      // 0x5C (offset to sub-data)
};

struct CMdlDynHolder {
    CMdlDynSub* field_0x0; // 0x0
};

// Looks up a named entry in the sub-object's embedded resource dictionary
// (ResDic at sub+4) and returns the resolved data pointer (4-aligned).
u8* func_804EB22C(CMdlDynHolder* self, const char* name) {
    if (self->field_0x0 == 0) {
        nw4r::db::Panic(lbl_eu_80530D18, 0x57, lbl_eu_80530CFC, lbl_eu_80530CF0,
                        lbl_eu_80663CC0);
    }
    u8* pDic = (u8*)self->field_0x0 + 4;
    nw4r::g3d::ResDic dic(pDic);
    if (((u32)pDic & 3) != 0) {
        nw4r::db::Panic(lbl_eu_80530F08, 0x54, lbl_eu_80530EE0);
    }
    u8* res = (u8*)dic[name];
    if (((u32)res & 3) != 0) {
        nw4r::db::Panic(lbl_eu_80530D54, 0x26, lbl_eu_80530D2C);
    }
    return res;
}

// Resolves the sub-object's embedded data pointer: asserts the dictionary is
// present (two checks) and that its kind is not zero, then resolves the
// offset at field_0x4 (same shape as func_804EB310 but the kind check is
// `!= 0` with a different file/line/message).
u8* func_804EB3E8(CMdlDynHolder* self) {
    if (self->field_0x0 == 0) {
        nw4r::db::Panic(lbl_eu_80530DC4, 0x26, lbl_eu_80530DA8, lbl_eu_80530D68,
                        lbl_eu_80663CC8);
    }
    if (self->field_0x0->field_0xC != 0) {
        nw4r::db::Panic(lbl_eu_80530E1C, 0x36, lbl_eu_80530DD8);
    }
    if (self->field_0x0 == 0) {
        nw4r::db::Panic(lbl_eu_80530D94, 0x26, lbl_eu_80530D78, lbl_eu_80530D68,
                        lbl_eu_80663CC4);
    }
    CMdlDynSub* sub = self->field_0x0;
    u32 off = sub->field_0x4;
    if (off != 0) {
        return (u8*)sub + off;
    }
    return 0;
}

// Returns the sub-object's embedded data pointer: asserts the dictionary
// kind (field_0xC == 1) and resolves the offset at field_0x4.
u8* func_804EB310(CMdlDynHolder* self) {
    if (self->field_0x0 == 0) {
        nw4r::db::Panic(lbl_eu_80530DC4, 0x26, lbl_eu_80530DA8, lbl_eu_80530D68,
                        lbl_eu_80663CC8);
    }
    // retail compares the kind as signed (cmpi) here despite the u32 storage
    if ((s32)self->field_0x0->field_0xC != 1) {
        nw4r::db::Panic(lbl_eu_80530E74, 0x3d, lbl_eu_80530E30);
    }
    if (self->field_0x0 == 0) {
        nw4r::db::Panic(lbl_eu_80530D94, 0x26, lbl_eu_80530D78, lbl_eu_80530D68,
                        lbl_eu_80663CC4);
    }
    CMdlDynSub* sub = self->field_0x0;
    u32 off = sub->field_0x4;
    if (off != 0) {
        return (u8*)sub + off;
    }
    return 0;
}

u32 func_804EB4C0(const CMdlDynHolder* self) {
    CMdlDynSub* sub = self->field_0x0;
    if (sub == 0) {
        nw4r::db::Panic(lbl_eu_80530DC4, 0x26, lbl_eu_80530DA8, lbl_eu_80530D68,
                        lbl_eu_80663CC8);
    }
    return self->field_0x0->field_0x8;
}

extern "C" void func_804EB524(u8* self, const void* src) { *(u32*)self = *(const u32*)src; }

extern "C" void func_804EB530(u8* self) {
    u8* s = (u8*)self;
    *(u32*)(s + 0x8C) = 0;
    *(u32*)(s + 0x90) = 0;
    *(u32*)(s + 0x94) = 0;
    *(u32*)(s + 0x9C) = -1;
    *(u32*)(s + 0xA0) = 0;
    *(u32*)(s + 0xA4) = 0;
    *(u32*)(s + 0xAC) = -1;
}

extern "C" void func_804EB558(u8* self, const void* src) { *(u32*)self = *(const u32*)src; }

// Raw 0xB0-byte element view used by the append-copy func_804EB564 (retail
// copies the head word + four floats, then the remaining 39 words raw).
struct CMdlDynElemRaw {
    u32 field_0x0;      // 0x0
    f32 field_0x4[4];   // 0x4..0x14
    u32 field_0x14[39]; // 0x14..0xB0
};

// Appends a copy of the 0xB0-byte source element to the dynamic list and
// bumps the element count. Field copies are written explicitly (rather than
// a struct assignment, which MWCC would copy word-by-word) so the head
// floats use lfs/stfs like retail.
void func_804EB564(CMdlDynList* self, const CMdlDynElemRaw* src) {
    u32 idx = self->field_0x4;
    u32 base = (u32)self->field_0x0;
    u32 off = idx * 0xB0;
    self->field_0x4 = idx + 1;
    CMdlDynElemRaw* dst = (CMdlDynElemRaw*)(base + off);
    dst->field_0x0 = src->field_0x0;
    dst->field_0x4[0] = src->field_0x4[0];
    dst->field_0x4[1] = src->field_0x4[1];
    dst->field_0x4[2] = src->field_0x4[2];
    dst->field_0x4[3] = src->field_0x4[3];
    dst->field_0x14[0] = src->field_0x14[0];
    dst->field_0x14[1] = src->field_0x14[1];
    dst->field_0x14[2] = src->field_0x14[2];
    dst->field_0x14[3] = src->field_0x14[3];
    dst->field_0x14[4] = src->field_0x14[4];
    dst->field_0x14[5] = src->field_0x14[5];
    dst->field_0x14[6] = src->field_0x14[6];
    dst->field_0x14[7] = src->field_0x14[7];
    dst->field_0x14[8] = src->field_0x14[8];
    dst->field_0x14[9] = src->field_0x14[9];
    dst->field_0x14[10] = src->field_0x14[10];
    dst->field_0x14[11] = src->field_0x14[11];
    dst->field_0x14[12] = src->field_0x14[12];
    dst->field_0x14[13] = src->field_0x14[13];
    dst->field_0x14[14] = src->field_0x14[14];
    dst->field_0x14[15] = src->field_0x14[15];
    dst->field_0x14[16] = src->field_0x14[16];
    dst->field_0x14[17] = src->field_0x14[17];
    dst->field_0x14[18] = src->field_0x14[18];
    dst->field_0x14[19] = src->field_0x14[19];
    dst->field_0x14[20] = src->field_0x14[20];
    dst->field_0x14[21] = src->field_0x14[21];
    dst->field_0x14[22] = src->field_0x14[22];
    dst->field_0x14[23] = src->field_0x14[23];
    dst->field_0x14[24] = src->field_0x14[24];
    dst->field_0x14[25] = src->field_0x14[25];
    dst->field_0x14[26] = src->field_0x14[26];
    dst->field_0x14[27] = src->field_0x14[27];
    dst->field_0x14[28] = src->field_0x14[28];
    dst->field_0x14[29] = src->field_0x14[29];
    dst->field_0x14[30] = src->field_0x14[30];
    dst->field_0x14[31] = src->field_0x14[31];
    dst->field_0x14[32] = src->field_0x14[32];
    dst->field_0x14[33] = src->field_0x14[33];
    dst->field_0x14[34] = src->field_0x14[34];
    dst->field_0x14[35] = src->field_0x14[35];
    dst->field_0x14[36] = src->field_0x14[36];
    dst->field_0x14[37] = src->field_0x14[37];
    dst->field_0x14[38] = src->field_0x14[38];
}

// Returns a pointer to element idx of a 0x98-byte-stride array stored at self+0.
u8* func_804EB6E0(u8* self, u32 idx) {
    return *(u8**)self + idx * 0x98;
}

u32 func_804EB6F0(u8* self) { return *(u32*)((u8*)self + 0x4); }

// Returns a pointer to element idx of a 0xB0-byte-stride array stored at self+0.
u8* func_804EB6F8(u8* self, u32 idx) {
    return *(u8**)self + idx * 0xB0;
}

extern "C" u32 func_804EB708(u8* self, u32 a, u32 b) {
    *(u32*)((u8*)self + 0) = a;
    *(u32*)((u8*)self + 4) = 0;
    *(u32*)((u8*)self + 8) = b;
    return b << 2;
}

extern "C" void func_804EB720(u8* self, const void* src) {
    u32 idx = *(u32*)((u8*)self + 4);
    *(u32*)(*(u8**)self + idx * 4) = *(u32*)src;
    *(u32*)((u8*)self + 4) = idx + 1;
}

extern "C" u32 func_804EB740(u8* self, u32 a, u32 b) {
    *(u32*)((u8*)self + 0) = a;
    *(u32*)((u8*)self + 4) = 0;
    *(u32*)((u8*)self + 8) = b;
    return b * 12;
}

extern "C" void func_804EB758(u8* self, const void* src) { *(u32*)self = *(const u32*)src; }

void func_804EB764(void *r3, void *r4) {
    int *p = (int *)r3;
    int *src = (int *)r4;
    int idx = p[1];
    int base = p[0];
    int off = idx * 12;
    p[1] = idx + 1;
    int *dst = (int *)(base + off);
    dst[0] = src[0];
    float *fdst = (float *)dst;
    float *fsrc = (float *)src;
    fdst[1] = fsrc[1];
    dst[2] = src[2];
}

// Sets the dynamic flag (0x40) on every element of every list.
void func_804EB798(CMdlDynSet* self) {
    CMdlDynList** it = self->field_0x8;
    while (it != self->field_0x8 + self->field_0xC) {
        u32 i = 0;
        while (i != (*it)->field_0x4) {
            (*it)->field_0x0[i++].field_0x8C |= 0x40;
        }
        it++;
    }
}

// Adds the given vector to the three embedded vectors (0x5C/0x68/0x80) of
// every element of every list owned by the set (dynamic-model update).
// Uses the nw4r VEC3Add kernel so the adds lower to retail's paired-single
// sequence instead of scalar lfs/fadds.
void func_804EB7F8(CMdlDynSet* self, const nw4r::math::VEC3* src) {
    CMdlDynList** it = self->field_0x8;
    while (it != self->field_0x8 + self->field_0xC) {
        // Byte-offset walk; an index counter here makes MWCC unroll the body
        // x2 via mtctr, which retail does not do.
        u32 off;
        for (off = 0; off != (*it)->field_0x4 * sizeof(CMdlDynElem); off += sizeof(CMdlDynElem)) {
            CMdlDynElem* e = (CMdlDynElem*)((u8*)(*it)->field_0x0 + off);
            nw4r::math::VEC3Add(&e->field_0x5C, &e->field_0x5C, src);
            nw4r::math::VEC3Add(&e->field_0x68, &e->field_0x68, src);
            nw4r::math::VEC3Add(&e->field_0x80, &e->field_0x80, src);
        }
        it++;
    }
}

// us-804efd5c: full teardown of the dynamic-model holder: releases every
// element buffer of every list (loop 1 walks by pointer without null checks,
// loop 2 re-walks by index with null-guarded buffer resets), then releases
// the list-pointer array and the second array (both MemManager-backed,
// guarded by their 0xFFFFFFFF sentinels).
void func_804EB8A0(CMdlDynamics* self) {
    if (self->field_0xC != 0) {
        CMdlDynList** it = (CMdlDynList**)self->field_0x8;
        while (it != (CMdlDynList**)self->field_0x8 + self->field_0xC) {
            CMdlDynElem* elem = (*it)->field_0x0;
            while (elem != (*it)->field_0x4 + (*it)->field_0x0) {
                elem->buf90.field_0x4 = 0;
                if (elem->buf90.field_0xC != 0xFFFFFFFFu) {
                    if (elem->buf90.field_0x0 != 0) {
                        mtl::MemManager::deallocate(elem->buf90.field_0x0);
                        elem->buf90.field_0x0 = 0;
                    }
                }
                elem->buf90.field_0x0 = 0;
                elem->buf90.field_0x8 = 0;
                elem->buf90.field_0xC = 0xFFFFFFFFu;
                elem->bufA0.field_0x4 = 0;
                if (elem->bufA0.field_0xC != 0xFFFFFFFFu) {
                    if (elem->bufA0.field_0x0 != 0) {
                        mtl::MemManager::deallocate(elem->bufA0.field_0x0);
                        elem->bufA0.field_0x0 = 0;
                    }
                }
                elem->bufA0.field_0x0 = 0;
                elem->bufA0.field_0x8 = 0;
                elem->bufA0.field_0xC = 0xFFFFFFFFu;
                elem++;
            }
            for (u32 i = 0; i < (*it)->field_0x4; i++) {
                CMdlDynElem* e = &(*it)->field_0x0[i];
                if (e != 0) {
                    if (&e->bufA0 != 0) {
                        e->bufA0.field_0x4 = 0;
                        if (e->bufA0.field_0xC != 0xFFFFFFFFu) {
                            if (e->bufA0.field_0x0 != 0) {
                                mtl::MemManager::deallocate(e->bufA0.field_0x0);
                                e->bufA0.field_0x0 = 0;
                            }
                        }
                        e->bufA0.field_0x0 = 0;
                        e->bufA0.field_0x8 = 0;
                        e->bufA0.field_0xC = 0xFFFFFFFFu;
                    }
                    if (&e->buf90 != 0) {
                        e->buf90.field_0x4 = 0;
                        if (e->buf90.field_0xC != 0xFFFFFFFFu) {
                            if (e->buf90.field_0x0 != 0) {
                                mtl::MemManager::deallocate(e->buf90.field_0x0);
                                e->buf90.field_0x0 = 0;
                            }
                        }
                        e->buf90.field_0x0 = 0;
                        e->buf90.field_0x8 = 0;
                        e->buf90.field_0xC = 0xFFFFFFFFu;
                    }
                }
            }
            (*it)->field_0x4 = 0;
            if ((*it)->field_0xC != 0xFFFFFFFFu) {
                if ((*it)->field_0x0 != 0) {
                    mtl::MemManager::deallocate((*it)->field_0x0);
                    (*it)->field_0x0 = 0;
                }
            }
            (*it)->field_0x0 = 0;
            (*it)->field_0x8 = 0;
            (*it)->field_0xC = 0xFFFFFFFFu;
            it++;
        }
        self->field_0xC = 0;
        if (self->field_0x14 != 0xFFFFFFFFu) {
            if (self->field_0x8 != 0) {
                mtl::MemManager::deallocate((void*)self->field_0x8);
                self->field_0x8 = 0;
            }
        }
        self->field_0x8 = 0;
        self->field_0x10 = 0;
        self->field_0x14 = 0xFFFFFFFFu;
    }
    self->field_0x1C = 0;
    if (self->field_0x24 != 0xFFFFFFFFu) {
        if (self->field_0x18 != 0) {
            mtl::MemManager::deallocate((void*)self->field_0x18);
            self->field_0x18 = 0;
        }
    }
    self->field_0x18 = 0;
    self->field_0x20 = 0;
    self->field_0x24 = 0xFFFFFFFFu;
}

// Holder with a CScnItemModel reference at +4 (func_804EBAE8).
struct CMdlDynModelRef {
    u8 field_0x0[0x4];        // 0x0
    CScnItemModel* field_0x4; // 0x4
};

// .sdata2 float constants (retail names; values live in retail .sdata2).
extern const float lbl_eu_8066B3D0; // 0.0f - scaled frame-delta zero check
// 1.0f - scale override used when the model's anim-hold flags (0x4/0x8) are set
extern const float lbl_eu_8066B3D4;

// Chain-tail distance query (retail C-ABI, defined in CScnItemModel.cpp).
// Local declaration only - CScnItemModel.hpp is outside this session's
// writable scope; move it there when headers become writable.
extern "C" float func_80484EB0(CScnItemModel* self);

// us-804effa4: anim-speed update. Reads the chain-tail distance (scale) of
// the referenced model, forces scale to 1 when either model flag bit
// (0x4/0x8) is set, multiplies by the frame delta, and flips model flag
// 0x20000 when the scaled delta is zero and the 0x7E8 counter is 1; clears
// model flag 0x8 afterwards.
void func_804EBAE8(CMdlDynModelRef* self) {
    if (self->field_0x4 == 0) {
        return;
    }
    f32 scale = func_80484EB0(self->field_0x4);
    if ((self->field_0x4->flags7A8 & 0x4) != 0 || (self->field_0x4->flags7A8 & 0x8) != 0) {
        scale = lbl_eu_8066B3D4;
    }
    s32 flag = 0;
    f32 t = scale * func_80496288(self->field_0x4->field_04);
    // retail compares the counter as signed (cmpi) here
    if (t == lbl_eu_8066B3D0 && (s32)self->field_0x4->value7E8 == 1) {
        flag = 1;
    }
    // one dominant base load feeding both arms (retail +88)
    u32* flags7A4p = &self->field_0x4->flags7A4;
    if (flag != 0) {
        *flags7A4p |= 0x20000;
    } else {
        *flags7A4p &= ~0x20000;
    }
    self->field_0x4->flags7A8 &= ~0x8;
}

u32 func_804EBBC4(u8* self) { return *(u32*)((u8*)self + 0x7e8); }

void func_804EBBCC(){}

extern "C" u32 func_804EC32C(u8* self) { return (*(u32*)((u8*)self + 0x7A4) >> 3) & 1; }

extern "C" u32 func_804EC338(u8* self) { return (*(u32*)((u8*)self + 0x7A4) >> 19) & 1; }

u32 func_804EC344(const CMdlDynHolder* self) {
    CMdlDynSub* sub = self->field_0x0;
    if (sub == 0) {
        nw4r::db::Panic(lbl_eu_80529678, 0x53, lbl_eu_80529658);
    }
    return self->field_0x0 != 0 ? self->field_0x0->field_0x10 : 0;
}

extern "C" u32 func_804EC3AC(u8* self) { return (*(u32*)((u8*)self + 0x7A4) >> 20) & 1; }

extern "C" u32 func_804EC3B8(u8* self) { return (*(u32*)((u8*)self + 0x7A4) >> 7) & 1; }

extern "C" u32 func_804EC3C4(u8* self) { return (*(u32*)((u8*)self + 0x7A4) >> 17) & 1; }

extern "C" void func_804EC3D0(u8* self, void* out) {
    const float c = *(float*)((u8*)self + 0x2C);
    const float b = *(float*)((u8*)self + 0x1C);
    const float a = *(float*)((u8*)self + 0x0C);
    *(float*)((u8*)out + 0) = a;
    *(float*)((u8*)out + 4) = b;
    *(float*)((u8*)out + 8) = c;
}

extern "C" void func_804EC3EC(u8* self, float a, float b, float c) {
    *(float*)((u8*)self + 0) = a;
    *(float*)((u8*)self + 4) = b;
    *(float*)((u8*)self + 8) = c;
}

extern "C" void func_804EC3FC(u8* self, const void* src) {
    *(float*)((u8*)self + 0) = *(float*)((u8*)src + 0);
    *(float*)((u8*)self + 4) = *(float*)((u8*)src + 4);
    *(float*)((u8*)self + 8) = *(float*)((u8*)src + 8);
}

u8* func_804EC418(const CMdlDynHolder* self) {
    // Load the pointer before the assert so the check uses the pre-call value;
    // the result re-reads the field like retail.
    CMdlDynSub* sub = self->field_0x0;
    if (sub == 0) {
        nw4r::db::Panic(lbl_eu_8056E194, 0x2c, lbl_eu_8056E178, lbl_eu_80663910,
                        lbl_eu_80663CBC);
    }
    return (u8*)self->field_0x0 + 0x38;
}

extern "C" void func_804EC478(Mtx a, const Quaternion* b) {
    PSMTXQuat(a, b);
}

// us-804f0938: out = a * b. PSMTXConcat writes into a stack temp, then the
// 48-byte result is copied to *out (retail reads the copy back from r1+8).
void func_804EC47C(nw4r::math::MTX34* out, const nw4r::math::MTX34* a,
                   const nw4r::math::MTX34* b) {
    nw4r::math::MTX34 tmp;
    PSMTXConcat(a->mtx, b->mtx, tmp.mtx);
    *out = tmp;
}

// us-804f09d0: quaternion-to-Euler conversion with a pitch singularity split
// at +/- lbl_eu_8066A200. out->y takes the asin pitch, out->x/out->z the two
// Atan2FIdx angles. Returns 1 on the regular branch, 0 at the singularity.
s32 func_804EC514(const Quaternion* q, nw4r::math::VEC3* out) {
    f32 x2 = q->x + q->x;
    f32 y2 = q->y + q->y;
    f32 z2 = q->z + q->z;
    f32 xz2 = q->x * z2;
    f32 wy2 = q->w * y2;
    f32 t = xz2 - wy2;
    f32 s = -t;
    // Clamp into [-1,1]: four sequential direct guards (compare + conditional
    // fmr from a fresh pool load each time); ±1 bounds are the shared-pool
    // floats 1.0f/B3E8, never literals.
    if (s >= 1.0f) {
        s = 1.0f;
    }
    if (s <= -1.0f) {
        s = -1.0f;
    }
    if (s < -1.0f) {
        s = -1.0f;
    }
    if (s > 1.0f) {
        s = 1.0f;
    }
    s32 inDomain = 0;
    if (s <= 1.0f && s >= -1.0f) {
        inDomain = 1;
    }
    if (!inDomain) {
        nw4r::db::Warning(lbl_eu_8052ADB0, 0xe4, lbl_eu_8052AD88);
    }
    f32 pitch = asin(s); // double asin, frsp on assignment
    out->y = pitch;
    // Only the four products shared by every branch are computed up front;
    // bb/bc/ad live inside the regular branch.
    f32 aa = q->x * x2; // 2x^2
    f32 xy = q->x * y2; // 2xy
    f32 cc = q->z * z2; // 2z^2
    f32 cd = q->w * z2; // 2wz
    s32 ret;
    if (pitch >= lbl_eu_8066A200) {
        // High singularity: yaw is well-defined, roll collapses.
        f32 ang = nw4r::math::Atan2FIdx(xy - cd, 1.0f - (aa + cc));
        out->x = lbl_eu_8066B3EC * ang;
        out->z = lbl_eu_8066B3D0;
        ret = 0;
    } else if (pitch <= -lbl_eu_8066A200) {
        // Low singularity: same angle, negated roll.
        f32 ang = nw4r::math::Atan2FIdx(xy - cd, 1.0f - (aa + cc));
        out->z = lbl_eu_8066B3D0;
        out->x = -(lbl_eu_8066B3EC * ang);
        ret = 0;
    } else {
        f32 bb = q->y * y2; // 2y^2
        f32 bc = q->y * z2; // 2yz
        f32 ad = q->w * x2; // 2wx
        f32 angY = nw4r::math::Atan2FIdx(bc + ad, 1.0f - (aa + bb));
        out->x = lbl_eu_8066B3EC * angY;
        f32 angZ = nw4r::math::Atan2FIdx(xy + cd, 1.0f - (bb + cc));
        out->z = lbl_eu_8066B3EC * angZ;
        ret = 1;
    }
    return ret;
}

// Converts a radian rotation vector to rotation indices and builds the XYZ
// rotation matrix (retail tail-call; 128/pi scale pooled at lbl_eu_8066B3F0,
// owned by the CGXCache .sdata2 pool). Referenced through the named extern so
// this TU does not code-pool its own copy into .sdata2 (retail .sdata2 is 0x0).
nw4r::math::MTX34* func_804EC76C(nw4r::math::MTX34* mtx, const f32* v) {
    extern const f32 lbl_eu_8066B3F0; // 128/pi (retail .sdata2, foreign TU)
    return nw4r::math::MTX34RotXYZFIdx(mtx, v[0] * lbl_eu_8066B3F0,
                                       v[1] * lbl_eu_8066B3F0,
                                       v[2] * lbl_eu_8066B3F0);
}

// us-804f0c48: out = a + b (nw4r MTX34Add into a stack temp, then copy out
// via the returned pointer - retail reads the copy back from r3).
void func_804EC78C(nw4r::math::MTX34* out, const nw4r::math::MTX34* a,
                   const nw4r::math::MTX34* b) {
    nw4r::math::MTX34 tmp;
    *out = *nw4r::math::MTX34Add(&tmp, a, b);
}

// us-804f0cd8: out = a * factor (nw4r MTX34Mult scalar-factor kernel, copy
// via the returned pointer).
void func_804EC81C(nw4r::math::MTX34* out, const nw4r::math::MTX34* a, f32 f) {
    nw4r::math::MTX34 tmp;
    *out = *nw4r::math::MTX34Mult(&tmp, a, f);
}

// us-804f0d68: normalizes each basis column of the matrix (components read
// with a 4-byte stride down the three rows). Zero-length columns are left
// untouched; negative/NaN lengths raise the nw4r math warning.
void func_804EC8AC(nw4r::math::MTX34* m) {
    for (u32 i = 0; i < 3; i++) {
        nw4r::math::VEC3 v;
        v.x = m->m[0][i];
        v.y = m->m[1][i];
        v.z = m->m[2][i];
        f32 lenSq = v.x * v.x + v.y * v.y + v.z * v.z;
        if (lenSq == lbl_eu_8066B3D0) {
            continue;
        }
        if (!(lenSq > lbl_eu_8066B3D0)) {
            nw4r::db::Warning(lbl_eu_80526324, 0x273, lbl_eu_80526300);
        }
        f32 len;
        if (lenSq <= lbl_eu_8066B3D0) {
            len = lenSq * nw4r::math::FrSqrt(lenSq);
        } else {
            len = lbl_eu_8066B3D0;
        }
        f32 inv = lbl_eu_8066B3D4 / len;
        m->m[1][i] = v.y * inv;
        m->m[0][i] = v.x * inv;
        m->m[2][i] = v.z * inv;
    }
}

u32 func_804EC9E4(u8* self) { return *(u32*)((u8*)self + 0x0); }

extern "C" void* func_804EC9EC(u8* self) { return (char*)*(void**)self + *(u32*)((u8*)self + 4) * 0x98; }

// Clamps the vector into the element's AABB; each axis is clamped only
// when its flag bit is set in field_0x8C (bit0=minX, bit1=maxX, bit2=minY,
// bit3=maxY, bit4=minZ, bit5=maxZ). The first parameter is unused by the
// body (retail keeps it in r3; caller passes the owning object).
void func_804ECA00(CMdlDynSet* owner, nw4r::math::VEC3* vec, CMdlDynElem* self) {
    if (self->field_0x8C & 0x1) {
        if (vec->x < self->minX) vec->x = self->minX;
    }
    if (self->field_0x8C & 0x2) {
        if (vec->x > self->maxX) vec->x = self->maxX;
    }
    if (self->field_0x8C & 0x4) {
        if (vec->y < self->minY) vec->y = self->minY;
    }
    if (self->field_0x8C & 0x8) {
        if (vec->y > self->maxY) vec->y = self->maxY;
    }
    if (self->field_0x8C & 0x10) {
        if (vec->z < self->minZ) vec->z = self->minZ;
    }
    if (self->field_0x8C & 0x20) {
        if (vec->z > self->maxZ) vec->z = self->maxZ;
    }
}

// --- Types shared by the dynamic-model collision helpers (targets 4/5) ---

// Model object whose +0x4 points at the loaded model resource root.
struct MdlDynObj {
    u8 field_0x0[0x4]; // 0x0
    u8* field_0x4;     // 0x4 resource root
    u8 field_0x8[0x10];
    u8* elems18;       // 0x18 base of the 0x98-stride element array
};

// Model resource root; the ResMdl data word used for node lookups sits at 0x146C.
struct MdlResRoot {
    u8 field_0x0[0x146C];
    u32 field_0x146C;
};

// Raw g3d ResNode fields read by the helpers: entry count at 0x10 (used to
// index a 0x30-stride matrix array) and bounding-sphere radius at 0x20.
struct MdlResNodeRaw {
    u8 field_0x0[0x10];
    u32 field_0x10;
    u8 field_0x14[0xC];
    f32 field_0x20;
};

// 0x98-stride dynamic-model element driven by func_804ECAC4.
struct CMdlDynElem98 {
    u32 field_0x0;                 // 0x0 resource id
    nw4r::math::MTX34 field_0x4;   // 0x4 local matrix
    nw4r::math::MTX34 field_0x34;  // 0x34 world matrix
    nw4r::math::MTX34 field_0x64;  // 0x64 view matrix
    u8 field_0x94;                 // 0x94 update kind (1/2/3)
};

// Update target owning the accumulated world position and velocities.
struct CMdlDynTarget {
    u8 field_0x0[0x68];
    nw4r::math::VEC3 pos68;  // 0x68 world position
    nw4r::math::VEC3 vel74;  // 0x74 accumulated velocity 1
    nw4r::math::VEC3 vel7C;  // 0x7C accumulated velocity 2
    u8 field_0x84[0xC];
    u32* ids90;              // 0x90 element-id array
    u32 count94;             // 0x94 element count
};

s32 func_804ED18C(MdlDynObj* obj, nw4r::math::VEC3* pos, nw4r::math::VEC3* out,
                  u32* idxPtr, nw4r::math::MTX34* matrices);

// us-804f1370: sphere-cast against the bounding sphere of res node *idxPtr:
// walks the node's matrix-array slot, projects the position onto the sphere
// along the view ray and writes the hit point into *pos and the (scaled)
// penetration vector into *out. Returns 0 when the point is outside/on the
// sphere or degenerate, else 1.
s32 func_804ECEB4(MdlDynObj* obj, nw4r::math::VEC3* pos, nw4r::math::VEC3* out,
                  u32* idxPtr, nw4r::math::MTX34* matrices) {
    MdlResRoot* root = (MdlResRoot*)obj->field_0x4;
    nw4r::g3d::ResMdl mdl((void*)root->field_0x146C);
    unsigned long raw = GetResNode__Q34nw4r3g3d6ResMdlCFUl(&mdl, *idxPtr);
    if (raw == 0) {
        nw4r::db::Panic(lbl_eu_80529678, 0x53, lbl_eu_80529658);
    }
    MdlResNodeRaw* node = (MdlResNodeRaw*)raw;
    u32 n = (node != 0) ? node->field_0x10 : 0;

    // Translation column of the n-th matrix in the array.
    nw4r::math::VEC3 center;
    center.x = matrices[n].m[0][3];
    center.y = matrices[n].m[1][3];
    center.z = matrices[n].m[2][3];

    nw4r::math::VEC3 d;
    d.x = pos->x - center.x;
    d.y = pos->y - center.y;
    d.z = pos->z - center.z;

    if (node == 0) {
        nw4r::db::Panic(lbl_eu_8056E194, 0x2c, lbl_eu_8056E178,
                        lbl_eu_80663910, lbl_eu_80663CBC);
    }

    f32 radius = node->field_0x20;
    f32 lenSq = d.x * d.x + d.y * d.y + d.z * d.z;
    if (lenSq >= radius * radius) {
        return 0;
    }
    f32 len = sqrt(lenSq); // frsp on assignment
    if (len == lbl_eu_8066B3D0) {
        return 0;
    }
    f32 inv = lbl_eu_8066B3D4 / len;
    d.x *= inv;
    d.y *= inv;
    d.z *= inv;

    nw4r::math::VEC3 off;
    off.x = d.x * radius;
    off.y = d.y * radius;
    off.z = d.z * radius;

    nw4r::math::VEC3 orig = *pos;
    nw4r::math::VEC3 hit;
    hit.x = off.x + d.x;
    hit.y = off.y + d.y;
    hit.z = off.z + d.z;
    *pos = hit;

    out->x = (hit.x - orig.x) * lbl_eu_8066B3F4;
    out->y = (hit.y - orig.y) * lbl_eu_8066B3F4;
    out->z = (hit.z - orig.z) * lbl_eu_8066B3F4;
    return 1;
}

// us-804f1648: capsule-style projection helper. Transforms the position into
// element space (local then view matrix), projects it onto the unit sphere
// around the +/-Y poles, resolves the element's res node and finally pushes
// the position back through the world/matrix chain, reporting the penetration
// vector scaled by lbl_eu_8066B3F4. Returns 0 when the point misses.
s32 func_804ED18C(MdlDynObj* obj, nw4r::math::VEC3* pos, nw4r::math::VEC3* out,
                  CMdlDynElem98* elem, nw4r::math::MTX34* matrices) {
    nw4r::math::VEC3 axis;
    axis.x = lbl_eu_8066B3D0;
    axis.y = lbl_eu_8066B3D0;
    axis.z = lbl_eu_8066B3D0;

    // world = view * (local * pos)
    nw4r::math::VEC3 localPos;
    PSMTXMultVec(elem->field_0x4.mtx, (const Vec*)pos, (Vec*)&localPos);
    nw4r::math::VEC3 world;
    PSMTXMultVec(elem->field_0x64.mtx, (const Vec*)&localPos, (Vec*)&world);

    // Project onto the unit sphere around both Y poles.
    for (u32 i = 0; i < 2; i++) {
        f32 sign;
        if (i == 0) {
            sign = lbl_eu_8066B3D4;
        } else {
            sign = -lbl_eu_8066B3D4;
        }
        axis.y = sign;

        nw4r::math::VEC3 d;
        d.x = world.x - axis.x;
        d.y = world.y - axis.y;
        d.z = world.z - axis.z;

        if (d.x < lbl_eu_8066B3D4) {
            if (d.y < lbl_eu_8066B3D4) {
                if (d.z < lbl_eu_8066B3D4) {
                    f32 lenSq = d.x * d.x + d.y * d.y + d.z * d.z;
                    if (lenSq < lbl_eu_8066B3D4 * lbl_eu_8066B3D4) {
                        f32 len = sqrt(lenSq);
                        if (len != lbl_eu_8066B3D0) {
                            f32 inv = lbl_eu_8066B3D4 / len;
                            d.x *= inv;
                            d.y *= inv;
                            d.z *= inv;
                            nw4r::math::VEC3 dir;
                            dir.x = d.x * lbl_eu_8066B3D4;
                            dir.y = d.y * lbl_eu_8066B3D4;
                            dir.z = d.z * lbl_eu_8066B3D4;
                            world.x = dir.x + axis.x;
                            world.y = dir.y + axis.y;
                            world.z = dir.z + axis.z;
                        }
                    }
                }
            }
        }
    }

    // Final point must sit on the unit circle in XZ and |y| <= 1.
    if (nw4r::math::FAbs(world.y) >= lbl_eu_8066B3D4) {
        return 0;
    }
    f32 radSq = world.x * world.x + world.z * world.z;
    if (radSq >= lbl_eu_8066B3D4 * lbl_eu_8066B3D4) {
        return 0;
    }
    if (radSq == lbl_eu_8066B3D0) {
        return 0;
    }

    // Resolve the element's res node (offset at 0x5C, 4-aligned).
    MdlResRoot* root = (MdlResRoot*)obj->field_0x4;
    nw4r::g3d::ResMdl mdl((void*)(unsigned long)root->field_0x146C);
    unsigned long raw = GetResNode__Q34nw4r3g3d6ResMdlCFUl(&mdl, elem->field_0x0);
    u8* node = (u8*)raw;
    if (node == 0) {
        nw4r::db::Panic(lbl_eu_8056E850, 0x2c, lbl_eu_8056E834,
                        lbl_eu_80663910, lbl_eu_80663CB8);
    }
    u32 off = *(u32*)(node + 0x5C);
    if (off != 0) {
        node += off;
        if ((((u32)node) & 3) != 0) {
            nw4r::db::Panic(lbl_eu_8056E820, 0x2c, lbl_eu_8056E7F8);
        }
    } else {
        raw = 0;
    }
    if (raw == 0) {
        nw4r::db::Panic(lbl_eu_80529678, 0x53, lbl_eu_80529658);
    }
    u32 n = *(u32*)(node + 0x10);

    // Project the XZ point onto the unit circle.
    f32 len = sqrt(radSq);
    f32 inv = lbl_eu_8066B3D4 / len;
    world.x = world.x * inv * lbl_eu_8066B3D4;
    world.z = world.z * inv * lbl_eu_8066B3D4;

    // Push the position back out through the matrix chain.
    nw4r::math::VEC3 orig = *pos;
    nw4r::math::VEC3 tmp;
    PSMTXMultVec(matrices[n].mtx, (const Vec*)&world, (Vec*)&tmp);
    world = tmp;
    nw4r::math::VEC3 hit;
    PSMTXMultVec((f32 (*)[4])node, (const Vec*)&world, (Vec*)&hit);

    s32 ret = 1;
    *pos = hit;
    out->x = (hit.x - orig.x) * lbl_eu_8066B3F4;
    out->y = (hit.y - orig.y) * lbl_eu_8066B3F4;
    out->z = (hit.z - orig.z) * lbl_eu_8066B3F4;
    return ret;
}

// us-804f0f80: per-element dynamics update. Walks the target's element-id
// list and dispatches on each element's update kind: kinds 1/3 run a sphere/
// ray helper (func_804ECEB4 / func_804ED18C) accumulating its output vector
// into both target velocities; kind 2 transforms the target position through
// the element's local/view/world matrices and accumulates the delta scaled by
// lbl_eu_8066B3F4 when view-space z is in front of the camera.
void func_804ECAC4(MdlDynObj* obj, CMdlDynTarget* tgt, nw4r::math::MTX34* matrices) {
    // The element array base lives at obj+0x18 with a 0x98-byte stride.
    u8* elemBase = obj->elems18;
    u32* it = tgt->ids90;
    while (it != tgt->ids90 + tgt->count94) {
        CMdlDynElem98* e = (CMdlDynElem98*)(elemBase + *it * 0x98);
        it++;
        switch (e->field_0x94) {
        case 1: {
            // Sphere-cast helper; accumulate its output into both velocities.
            nw4r::math::VEC3 delta;
            if (!func_804ECEB4(obj, &tgt->pos68, &delta, &e->field_0x0, matrices)) {
                continue;
            }
            tgt->vel74.x += delta.x;
            tgt->vel74.y += delta.y;
            tgt->vel74.z += delta.z;
            tgt->vel7C.x += delta.x;
            tgt->vel7C.y += delta.y;
            tgt->vel7C.z += delta.z;
            break;
        }
        case 2: {
            s32 hit = 0;
            nw4r::math::VEC3 world;
            PSMTXMultVec(e->field_0x4.mtx, (const Vec*)&tgt->pos68, (Vec*)&world);
            nw4r::math::VEC3 view;
            PSMTXMultVec(e->field_0x64.mtx, (const Vec*)&world, (Vec*)&view);
            if (view.z > lbl_eu_8066B3D0) {
                MdlResRoot* root = (MdlResRoot*)obj->field_0x4;
                nw4r::g3d::ResMdl mdl((void*)root->field_0x146C);
                unsigned long raw = GetResNode__Q34nw4r3g3d6ResMdlCFUl(&mdl, e->field_0x0);
                if (raw == 0) {
                    nw4r::db::Panic(lbl_eu_8056E850, 0x2c, lbl_eu_8056E834,
                                    lbl_eu_80663910, lbl_eu_80663CB8);
                }
                u8* node = (u8*)raw;
                u32 off = *(u32*)(node + 0x5C);
                if (off != 0) {
                    node += off;
                    if ((((u32)node) & 3) != 0) {
                        nw4r::db::Panic(lbl_eu_8056E820, 0x2c, lbl_eu_8056E7F8);
                    }
                } else {
                    raw = 0;
                }
                if (raw == 0) {
                    nw4r::db::Panic(lbl_eu_80529678, 0x53, lbl_eu_80529658);
                }
                u32 n = (raw != 0) ? *(u32*)(node + 0x10) : 0;

                nw4r::math::VEC3 p1;
                PSMTXMultVec(e->field_0x34.mtx, (const Vec*)&world, (Vec*)&p1);
                nw4r::math::VEC3 p2;
                PSMTXMultVec(matrices[n].mtx, (const Vec*)&p1, (Vec*)&p2);

                nw4r::math::VEC3 delta;
                delta.x = (p2.x - tgt->pos68.x) * lbl_eu_8066B3F4;
                delta.y = (p2.y - tgt->pos68.y) * lbl_eu_8066B3F4;
                delta.z = (p2.z - tgt->pos68.z) * lbl_eu_8066B3F4;
                tgt->pos68 = p2;
                hit = 1;

                if (hit) {
                    tgt->vel74.x += delta.x;
                    tgt->vel74.y += delta.y;
                    tgt->vel74.z += delta.z;
                    tgt->vel7C.x += delta.x;
                    tgt->vel7C.y += delta.y;
                    tgt->vel7C.z += delta.z;
                }
            }
            break;
        }
        case 3: {
            nw4r::math::VEC3 delta;
            if (!func_804ED18C(obj, &tgt->pos68, &delta, &e->field_0x0, matrices)) {
                continue;
            }
            tgt->vel74.x += delta.x;
            tgt->vel74.y += delta.y;
            tgt->vel74.z += delta.z;
            tgt->vel7C.x += delta.x;
            tgt->vel7C.y += delta.y;
            tgt->vel7C.z += delta.z;
            break;
        }
        default:
            break;
        }
    }
}

// One entry of an element's dynamic-entry array (element+0xA0 buffer):
// references an anchor matrix column plus the relaxation weights.
struct CMdlDynEntry {
    u32 id;    // 0x0 res-node id looked up via GetResNode
    f32 unk04; // 0x4 weight (distance offset fed into the relax factor)
    u32 unk08; // 0x8 flag; 0 resets the accumulator position
};

// us-804f1b38: dynamics driver. Walks every list/element owned by self,
// relaxes each element's position/velocity toward the anchor matrix columns
// referenced by its entry array, then dispatches the per-element update kinds
// (same helpers as func_804ECAC4) accumulating into both the element and a
// stack-local accumulator element whose buffers are released every iteration.
void func_804ED67C(CMdlDynamics* self, nw4r::math::MTX34* matrices) {
    CMdlDynElem acc; // scratch accumulator; fields initialized per element
    CMdlDynElem* pAcc = &acc;
    CMdlDynList** it = (CMdlDynList**)self->field_0x8;
    CMdlDynList** end = it + self->field_0xC;
    while (it != end) {
        // Byte-offset walk; an index counter here makes MWCC unroll the body
        // via mtctr, which retail does not do.
        u32 off;
        for (off = 0; off != (*it)->field_0x4 * sizeof(CMdlDynElem);
             off += sizeof(CMdlDynElem)) {
            CMdlDynElem* tgt = (CMdlDynElem*)((u8*)(*it)->field_0x0 + off);

            acc.field_0x8C = 0;
            acc.buf90.field_0x0 = 0;
            acc.buf90.field_0x4 = 0;
            acc.buf90.field_0xC = 0xFFFFFFFFu;
            acc.bufA0.field_0x0 = 0;
            acc.bufA0.field_0x4 = 0;
            acc.bufA0.field_0xC = 0xFFFFFFFFu;

            // Entry-driven relaxation pass.
            CMdlDynEntry* ent = (CMdlDynEntry*)tgt->bufA0.field_0x0;
            CMdlDynEntry* entEnd = ent + tgt->bufA0.field_0x4;
            while (ent != entEnd) {
                MdlResRoot* root = (MdlResRoot*)self->field_0x4;
                nw4r::g3d::ResMdl mdl((void*)root->field_0x146C);
                unsigned long raw = GetResNode__Q34nw4r3g3d6ResMdlCFUl(&mdl, ent->id);
                if (raw == 0) {
                    nw4r::db::Panic(lbl_eu_80529678, 0x53, lbl_eu_80529658);
                }
                MdlResNodeRaw* node = (MdlResNodeRaw*)raw;
                u32 n = (node != 0) ? node->field_0x10 : 0;

                // Translation column of the anchor matrix.
                nw4r::math::VEC3 center;
                center.x = matrices[n].m[0][3];
                center.y = matrices[n].m[1][3];
                center.z = matrices[n].m[2][3];

                nw4r::math::VEC3 d;
                d.x = center.x - tgt->field_0x68.x;
                d.y = center.y - tgt->field_0x68.y;
                d.z = center.z - tgt->field_0x68.z;
                nw4r::math::VEC3 dArg = d;
                f32 mag = PSVECMag((const Vec*)&dArg);

                f32 k = lbl_eu_8066B3F8 * (ent->unk04 - mag);

                // velocity relaxation step: s = d * k
                nw4r::math::VEC3 s;
                s.x = d.x * k;
                s.y = d.y * k;
                s.z = d.z * k;
                nw4r::math::VEC3 sc = s;
                tgt->field_0x74.x -= sc.x;
                tgt->field_0x74.y -= sc.y;
                tgt->field_0x74.z -= sc.z;

                // position relaxation step: w = (d * k) * B3FC
                nw4r::math::VEC3 s2;
                s2.x = d.x * k;
                s2.y = d.y * k;
                s2.z = d.z * k;
                nw4r::math::VEC3 s2c = s2;
                nw4r::math::VEC3 w;
                w.x = s2c.x * lbl_eu_8066B3FC;
                w.y = s2c.y * lbl_eu_8066B3FC;
                w.z = s2c.z * lbl_eu_8066B3FC;
                nw4r::math::VEC3 wc = w;
                tgt->field_0x68.x -= wc.x;
                tgt->field_0x68.y -= wc.y;
                tgt->field_0x68.z -= wc.z;

                if (ent->unk08 == 0) {
                    pAcc->field_0x68.x = ml::CVec3::zero.x;
                    pAcc->field_0x68.y = ml::CVec3::zero.y;
                    pAcc->field_0x68.z = ml::CVec3::zero.z;
                }
                ent++;
            }

            // Kind dispatch over the element's update-id array.
            u32* kindIt = (u32*)tgt->buf90.field_0x0;
            u32* kindEnd = kindIt + tgt->buf90.field_0x4;
            while (kindIt != kindEnd) {
                CMdlDynElem98* e =
                    (CMdlDynElem98*)((u8*)self->field_0x18 + *kindIt * 0x98);
                kindIt++;
                switch (e->field_0x94) {
                case 1: {
                    nw4r::math::VEC3 sum;
                    sum.x = pAcc->field_0x68.x + tgt->field_0x68.x;
                    sum.y = pAcc->field_0x68.y + tgt->field_0x68.y;
                    sum.z = pAcc->field_0x68.z + tgt->field_0x68.z;
                    nw4r::math::VEC3 sumc = sum;
                    nw4r::math::VEC3 p;
                    p.x = sumc.x * lbl_eu_8066B3E4;
                    p.y = sumc.y * lbl_eu_8066B3E4;
                    p.z = sumc.z * lbl_eu_8066B3E4;
                    nw4r::math::VEC3 pc = p;
                    nw4r::math::VEC3 delta;
                    if (!func_804ECEB4((MdlDynObj*)self, &pc, &delta,
                                       &e->field_0x0, matrices)) {
                        continue;
                    }
                    tgt->field_0x68.x += delta.x;
                    tgt->field_0x68.y += delta.y;
                    tgt->field_0x68.z += delta.z;
                    tgt->field_0x74.x += delta.x;
                    tgt->field_0x74.y += delta.y;
                    tgt->field_0x74.z += delta.z;
                    acc.field_0x68.x += delta.x;
                    acc.field_0x68.y += delta.y;
                    acc.field_0x68.z += delta.z;
                    acc.field_0x74.x += delta.x;
                    acc.field_0x74.y += delta.y;
                    acc.field_0x74.z += delta.z;
                    break;
                }
                case 2: {
                    s32 hit = 0;
                    nw4r::math::VEC3 sum;
                    sum.x = pAcc->field_0x68.x + tgt->field_0x68.x;
                    sum.y = pAcc->field_0x68.y + tgt->field_0x68.y;
                    sum.z = pAcc->field_0x68.z + tgt->field_0x68.z;
                    nw4r::math::VEC3 sumc = sum;
                    nw4r::math::VEC3 p;
                    p.x = sumc.x * lbl_eu_8066B3E4;
                    p.y = sumc.y * lbl_eu_8066B3E4;
                    p.z = sumc.z * lbl_eu_8066B3E4;
                    nw4r::math::VEC3 pc = p;
                    nw4r::math::VEC3 world;
                    PSMTXMultVec(e->field_0x4.mtx, (const Vec*)&pc, (Vec*)&world);
                    nw4r::math::VEC3 view;
                    PSMTXMultVec(e->field_0x64.mtx, (const Vec*)&world, (Vec*)&view);
                    nw4r::math::VEC3 delta;
                    if (view.z > lbl_eu_8066B3D0) {
                        MdlResRoot* root = (MdlResRoot*)self->field_0x4;
                        nw4r::g3d::ResMdl mdl((void*)root->field_0x146C);
                        unsigned long raw =
                            GetResNode__Q34nw4r3g3d6ResMdlCFUl(&mdl, e->field_0x0);
                        if (raw == 0) {
                            nw4r::db::Panic(lbl_eu_8056E850, 0x2c, lbl_eu_8056E834,
                                            lbl_eu_80663910, lbl_eu_80663CB8);
                        }
                        u8* node = (u8*)raw;
                        u32 resOff = *(u32*)(node + 0x5C);
                        if (resOff != 0) {
                            node += resOff;
                            if ((((u32)node) & 3) != 0) {
                                nw4r::db::Panic(lbl_eu_8056E820, 0x2c,
                                                lbl_eu_8056E7F8);
                            }
                        } else {
                            raw = 0;
                        }
                        if (raw == 0) {
                            nw4r::db::Panic(lbl_eu_80529678, 0x53,
                                            lbl_eu_80529658);
                        }
                        u32 n = *(u32*)(node + 0x10);

                        // Flatten view-space y, push through the world matrix
                        // and the anchor matrix; the penetration vector is the
                        // difference of the two stages scaled by B3F4.
                        view.y = lbl_eu_8066B3D0;
                        nw4r::math::VEC3 t1;
                        PSMTXMultVec(e->field_0x34.mtx, (const Vec*)&view,
                                     (Vec*)&t1);
                        view = t1;
                        nw4r::math::VEC3 t2;
                        PSMTXMultVec(matrices[n].mtx, (const Vec*)&view,
                                     (Vec*)&t2);
                        hit = 1;
                        nw4r::math::VEC3 diff;
                        diff.x = t2.x - view.x;
                        diff.y = t2.y - view.y;
                        diff.z = t2.z - view.z;
                        nw4r::math::VEC3 diffc = diff;
                        delta.x = diffc.x * lbl_eu_8066B3F4;
                        delta.y = diffc.y * lbl_eu_8066B3F4;
                        delta.z = diffc.z * lbl_eu_8066B3F4;
                    }
                    if (hit) {
                        tgt->field_0x68.x += delta.x;
                        tgt->field_0x68.y += delta.y;
                        tgt->field_0x68.z += delta.z;
                        tgt->field_0x74.x += delta.x;
                        tgt->field_0x74.y += delta.y;
                        tgt->field_0x74.z += delta.z;
                        acc.field_0x68.x += delta.x;
                        acc.field_0x68.y += delta.y;
                        acc.field_0x68.z += delta.z;
                        acc.field_0x74.x += delta.x;
                        acc.field_0x74.y += delta.y;
                        acc.field_0x74.z += delta.z;
                    }
                    break;
                }
                case 3: {
                    nw4r::math::VEC3 sum;
                    sum.x = pAcc->field_0x68.x + tgt->field_0x68.x;
                    sum.y = pAcc->field_0x68.y + tgt->field_0x68.y;
                    sum.z = pAcc->field_0x68.z + tgt->field_0x68.z;
                    nw4r::math::VEC3 sumc = sum;
                    nw4r::math::VEC3 p;
                    p.x = sumc.x * lbl_eu_8066B3E4;
                    p.y = sumc.y * lbl_eu_8066B3E4;
                    p.z = sumc.z * lbl_eu_8066B3E4;
                    nw4r::math::VEC3 pc = p;
                    nw4r::math::VEC3 delta;
                    if (!func_804ED18C((MdlDynObj*)self, &pc, &delta, e,
                                       matrices)) {
                        continue;
                    }
                    tgt->field_0x68.x += delta.x;
                    tgt->field_0x68.y += delta.y;
                    tgt->field_0x68.z += delta.z;
                    tgt->field_0x74.x += delta.x;
                    tgt->field_0x74.y += delta.y;
                    tgt->field_0x74.z += delta.z;
                    acc.field_0x68.x += delta.x;
                    acc.field_0x68.y += delta.y;
                    acc.field_0x68.z += delta.z;
                    acc.field_0x74.x += delta.x;
                    acc.field_0x74.y += delta.y;
                    acc.field_0x74.z += delta.z;
                    break;
                }
                default:
                    break;
                }
            }

            // Release the accumulator's scratch buffers for this element.
            if (&pAcc->bufA0 != 0) {
                pAcc->bufA0.field_0x4 = 0;
                if (pAcc->bufA0.field_0xC != 0xFFFFFFFFu) {
                    if (pAcc->bufA0.field_0x0 != 0) {
                        mtl::MemManager::deallocate(pAcc->bufA0.field_0x0);
                        pAcc->bufA0.field_0x0 = 0;
                    }
                }
                pAcc->bufA0.field_0x0 = 0;
                pAcc->bufA0.field_0x8 = 0;
                pAcc->bufA0.field_0xC = 0xFFFFFFFFu;
            }
            if (&pAcc->buf90 != 0) {
                pAcc->buf90.field_0x4 = 0;
                if (pAcc->buf90.field_0xC != 0xFFFFFFFFu) {
                    if (pAcc->buf90.field_0x0 != 0) {
                        mtl::MemManager::deallocate(pAcc->buf90.field_0x0);
                        pAcc->buf90.field_0x0 = 0;
                    }
                }
                pAcc->buf90.field_0x0 = 0;
                pAcc->buf90.field_0x8 = 0;
                pAcc->buf90.field_0xC = 0xFFFFFFFFu;
            }
        }
        it++;
    }
}

extern "C" void func_804E95E0() {}
u32 func_804E9FD0(const CMdlDynHolder* self) {
    CMdlDynSub* sub = self->field_0x0;
    if (sub == NULL) {
        nw4r::db::Panic(lbl_eu_8056E1C8, 0x38, lbl_eu_8056E1A8);
    }
    return self->field_0x0 ? self->field_0x0->field_0xC : 0;
}
// Returns the sub-object's embedded data pointer: reads an offset at 0x5C on
// the sub-object and resolves it against the base (asserted 4-aligned).
u8* func_804EA038(CMdlDynHolder* self) {
    if (self->field_0x0 == 0) {
        nw4r::db::Panic(lbl_eu_8056E850, 0x2c, lbl_eu_8056E834, lbl_eu_80663910,
                        lbl_eu_80663CB8);
    }
    CMdlDynSub* sub = self->field_0x0;
    u32 off = sub->field_0x5C;
    u8* ret;
    if (off != 0) {
        ret = (u8*)sub + off;
        if (((u32)ret & 3) != 0) {
            nw4r::db::Panic(lbl_eu_8056E820, 0x2c, lbl_eu_8056E7F8);
        }
    } else {
        ret = 0;
    }
    return ret;
}


// ===== Dissolved monolibdata2 (blob surgery) data owned by this TU =====
// [.data] 0x805701FC-0x80570248 (76B): CMdlDynamics vtable + assert strings.
// Defined as one big-endian u32[19] so MWCC emits a single 76-byte .data object
// with the retail byte-exact layout (no 8-alignment gaps between sub-objects;
// retail packs the strings at offset 0xC, i.e. 4-aligned).
extern "C" u32 lbl_eu_805701FC[19] = {
    // vtable: { RTTI locator, 0, dtor }
    (u32)&lbl_eu_80663CB0, 0x00000000, (u32)&__dt__12CMdlDynamicsFv,
    // "NW4R:Pointer must not be NULL (mpData)\0" (39B) + 1B pad, as words
    0x4E573452, 0x3A506F69, 0x6E746572, 0x206D7573, 0x74206E6F, 0x74206265,
    0x204E554C, 0x4C20286D, 0x70446174, 0x61290000,
    // "g3d_rescommon_ac.h\0" (19B) + 5B tail pad, as words
    0x6733645F, 0x72657363, 0x6F6D6D6F, 0x6E5F6163, 0x2E680000, 0x00000000,
};
DECOMP_FORCEACTIVE(CMdlDynamics_cpp, lbl_eu_805701FC);

// [.rodata] 0x80524830-0x80524894 (100B): RTTI name + anim-param names +
// assert format strings.
extern "C" __declspec(align(4)) const char lbl_eu_80524830[0x10] = {
    0x43,0x4D,0x64,0x6C,0x44,0x79,0x6E,0x61,0x6D,0x69,0x63,0x73,0x00,0x00,0x00,0x00,
};  // "CMdlDynamics\0" + 3B pad
extern "C" __declspec(align(4)) const char lbl_eu_80524840[0x30] = {
    0x44,0x77,0x65,0x74,0x00,0x44,0x64,0x6D,0x70,0x00,0x44,0x73,0x74,0x61,0x00,0x44,
    0x62,0x6C,0x64,0x00,0x44,0x6C,0x72,0x78,0x00,0x44,0x6C,0x72,0x79,0x00,0x44,0x6C,
    0x72,0x7A,0x00,0x44,0x6C,0x69,0x6E,0x6B,0x00,0x44,0x6C,0x6C,0x65,0x6E,0x00,0x00,
};  // "Dwet\0Ddmp\0Dsta\0Dbld\0Dlrx\0Dlry\0Dlrz\0Dlink\0Dllen\0\0"
extern "C" __declspec(align(4)) const char lbl_eu_80524870[0x18] = {
    0x20,0x69,0x6E,0x20,0x22,0x25,0x73,0x22,0x20,0x6F,0x6E,0x20,0x6C,0x69,0x6E,0x65,
    0x20,0x25,0x64,0x2E,0x0A,0x00,0x00,0x00,
};  // " in \"%s\" on line %d.\n\0" + 2B pad
extern "C" __declspec(align(4)) const char lbl_eu_80524888[0xC] = {
    0x62,0x61,0x6E,0x6E,0x65,0x72,0x2E,0x62,0x69,0x6E,0x00,0x00,
};  // "banner.bin\0" + 1B pad
DECOMP_FORCEACTIVE(CMdlDynamics_cpp, lbl_eu_80524830);
DECOMP_FORCEACTIVE(CMdlDynamics_cpp, lbl_eu_80524840);
DECOMP_FORCEACTIVE(CMdlDynamics_cpp, lbl_eu_80524870);
DECOMP_FORCEACTIVE(CMdlDynamics_cpp, lbl_eu_80524888);

// [.sdata] 0x80663CB0-0x80663CD0 (32B): RTTI locator + "ref" param strings.
// The last "ref" slot carries 4 zero pad bytes (retail +0x1C tail); merging
// them into a non-zero object keeps them in .sdata (a plain zero-init array
// would land in .sbss).
extern "C" u32 lbl_eu_80663CB0[2] = { (u32)&lbl_eu_80524830, 0x00000000 };
extern char lbl_eu_80663CB8[4] = { 'r','e','f',0 };
extern char lbl_eu_80663CBC[4] = { 'r','e','f',0 };
extern char lbl_eu_80663CC0[4] = { 'r','e','f',0 };
extern char lbl_eu_80663CC4[4] = { 'r','e','f',0 };
extern char lbl_eu_80663CC8[8] = { 'r','e','f',0, 0,0,0,0 };
DECOMP_FORCEACTIVE(CMdlDynamics_cpp, lbl_eu_80663CB0);
DECOMP_FORCEACTIVE(CMdlDynamics_cpp, lbl_eu_80663CB8);
DECOMP_FORCEACTIVE(CMdlDynamics_cpp, lbl_eu_80663CBC);
DECOMP_FORCEACTIVE(CMdlDynamics_cpp, lbl_eu_80663CC0);
DECOMP_FORCEACTIVE(CMdlDynamics_cpp, lbl_eu_80663CC4);
DECOMP_FORCEACTIVE(CMdlDynamics_cpp, lbl_eu_80663CC8);
