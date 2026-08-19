// Auto-scaffolded catalog TU for monolib/src/scn/CMdlDynamics
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>

#include <nw4r/math/math_types.h>
#include <nw4r/db/db_assert.h>
#include <nw4r/g3d/res/g3d_resdict.h>

#include <monolib/util/MemManager.hpp>
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
extern char lbl_eu_80663CC8[8]; // panic format arg (.sdata, sda21; retail 4B + 4B tail pad)
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

// CMdlDynamics retail vtable (3 entries, 0xC bytes) lives in retail .data.
extern "C" void __dt__12CMdlDynamicsFv(void* self, int flags);
extern "C" u32 lbl_eu_80663CB0[2]; // .sdata RTTI locator (defined below)
struct CMdlDynamicsVtbl {
    u32 entry0;
    u32 entry1;
    u32 entry2;
};
extern "C" CMdlDynamicsVtbl lbl_eu_805701FC;

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
    vtbl = &lbl_eu_805701FC;
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
    u8 field_0x74[0xC];          // 0x74..0x7F
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
    CMdlDynSub* sub = self->field_0x0;
    if (sub == 0) {
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
    if (self->field_0x0->field_0xC != 1) {
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

u32 func_804EB4C0(CMdlDynHolder* self) {
    if (self->field_0x0 == 0) {
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
void func_804EB7F8(CMdlDynSet* self, const nw4r::math::VEC3* src) {
    CMdlDynList** it = self->field_0x8;
    while (it != self->field_0x8 + self->field_0xC) {
        u32 i = 0;
        while (i != (*it)->field_0x4) {
            CMdlDynElem* e = &(*it)->field_0x0[i];
            e->field_0x5C.x += src->x;
            e->field_0x5C.y += src->y;
            e->field_0x5C.z += src->z;
            e->field_0x68.x += src->x;
            e->field_0x68.y += src->y;
            e->field_0x68.z += src->z;
            e->field_0x80.x += src->x;
            e->field_0x80.y += src->y;
            e->field_0x80.z += src->z;
            i++;
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
    if (t == lbl_eu_8066B3D0 && self->field_0x4->value7E8 == 1) {
        flag = 1;
    }
    if (flag != 0) {
        self->field_0x4->flags7A4 |= 0x20000;
    } else {
        self->field_0x4->flags7A4 &= ~0x20000;
    }
    self->field_0x4->flags7A8 &= ~0x8;
}

u32 func_804EBBC4(u8* self) { return *(u32*)((u8*)self + 0x7e8); }

void func_804EBBCC(){}

extern "C" u32 func_804EC32C(u8* self) { return (*(u32*)((u8*)self + 0x7A4) >> 3) & 1; }

extern "C" u32 func_804EC338(u8* self) { return (*(u32*)((u8*)self + 0x7A4) >> 19) & 1; }

u32 func_804EC344(CMdlDynHolder* self) {
    if (self->field_0x0 == 0) {
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

u8* func_804EC418(CMdlDynHolder* self) {
    if (self->field_0x0 == 0) {
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

void func_804EC514(){}

// Converts a radian rotation vector to rotation indices and builds the XYZ
// rotation matrix (retail tail-call; 128/pi scale pooled at lbl_eu_8066B3F0,
// owned by the CGXCache .sdata2 pool). Referenced via the named extern so this
// TU does not code-pool its own copy into .sdata2 (retail .sdata2 is 0x0).
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

void func_804EC8AC(){}

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

void func_804ECAC4(){}

void func_804ECEB4(){}

void func_804ED18C(){}

void func_804ED67C(){}

extern "C" void func_804E95E0() {}
u32 func_804E9FD0(CMdlDynHolder* self) {
    if (self->field_0x0 == NULL) {
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
// align(4) so the arrays pack at 4-byte boundaries exactly like retail.
extern "C" __declspec(align(4)) CMdlDynamicsVtbl lbl_eu_805701FC = {
    (u32)&lbl_eu_80663CB0, 0x00000000, (u32)&__dt__12CMdlDynamicsFv,
};
// 64B: "NW4R:Pointer must not be NULL (mpData)\0" (39B) + 1B pad +
// "g3d_rescommon_ac.h\0" (19B) + 5B tail pad.
extern "C" __declspec(align(4)) char lbl_eu_80570208[0x40] = {
    0x4E,0x57,0x34,0x52,0x3A,0x50,0x6F,0x69,0x6E,0x74,0x65,0x72,0x20,0x6D,0x75,0x73,
    0x74,0x20,0x62,0x65,0x20,0x4E,0x55,0x4C,0x4C,0x20,0x28,0x6D,0x70,0x44,0x61,0x74,
    0x61,0x29,0x00,0x00,0x67,0x33,0x64,0x5F,0x72,0x65,0x73,0x63,0x6F,0x6D,0x6D,0x6F,
    0x6E,0x5F,0x61,0x63,0x2E,0x68,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
};
DECOMP_FORCEACTIVE(CMdlDynamics_cpp, lbl_eu_805701FC);
DECOMP_FORCEACTIVE(CMdlDynamics_cpp, lbl_eu_80570208);

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
