// Auto-scaffolded catalog TU for monolib/src/scn/CScnFogMan
#include <harness_catalog.h>
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

// vtable (blob data defined below).
extern "C" u32 lbl_eu_8056EBD0[4];

// ===== Dissolved monolibdata2 (blob surgery) data owned by this TU =====
extern "C" u32 lbl_eu_80663A68[2];  // RTTI locator (owned by CScnItemCamera.cpp)
extern "C" void* __dt__10CScnFogManFv(CScnFogMan* self, int flag);
extern "C" void __dl__FPv(void*);

// [.rodata] 0x80524218-0x80524228 (0x10 = 16B): RTTI name "CScnFogMan".
extern "C" __declspec(align(8)) const char lbl_eu_80524218[0x10] = {
    0x43,0x53,0x63,0x6E,0x46,0x6F,0x67,0x4D,0x61,0x6E,0x00,0x00,0x00,0x00,0x00,0x00,
};

// [.data] 0x8056EBD0-0x8056EC70 (0xA0 = 160B): vtable + NW4R assert strings.
extern "C" u32 lbl_eu_8056EBD0[4] = {
    (u32)&lbl_eu_80663A68, 0x00000000, (u32)&__dt__10CScnFogManFv, 0x00000000,
};

extern "C" __declspec(align(8)) char lbl_eu_8056EBE0[0x90] = {
    0x4E, 0x57, 0x34, 0x52, 0x3A, 0x46, 0x61, 0x69, 0x6C, 0x65, 0x64, 0x20, 0x61, 0x73, 0x73, 0x65,
    0x72, 0x74, 0x69, 0x6F, 0x6E, 0x20, 0x49, 0x73, 0x56, 0x61, 0x6C, 0x69, 0x64, 0x28, 0x29, 0x00,
    0x67, 0x33, 0x64, 0x5F, 0x66, 0x6F, 0x67, 0x2E, 0x68, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x4E, 0x57, 0x34, 0x52, 0x3A, 0x46, 0x61, 0x69, 0x6C, 0x65, 0x64, 0x20, 0x61, 0x73, 0x73, 0x65,
    0x72, 0x74, 0x69, 0x6F, 0x6E, 0x20, 0x49, 0x73, 0x56, 0x61, 0x6C, 0x69, 0x64, 0x28, 0x29, 0x00,
    0x67, 0x33, 0x64, 0x5F, 0x66, 0x6F, 0x67, 0x2E, 0x68, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x4E, 0x57, 0x34, 0x52, 0x3A, 0x46, 0x61, 0x69, 0x6C, 0x65, 0x64, 0x20, 0x61, 0x73, 0x73, 0x65,
    0x72, 0x74, 0x69, 0x6F, 0x6E, 0x20, 0x49, 0x73, 0x56, 0x61, 0x6C, 0x69, 0x64, 0x28, 0x29, 0x00,
    0x67, 0x33, 0x64, 0x5F, 0x66, 0x6F, 0x67, 0x2E, 0x68, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};
DECOMP_FORCEACTIVE(CScnFogMan_cpp, lbl_eu_8056EBE0);
DECOMP_FORCEACTIVE(CScnFogMan_cpp, lbl_eu_80524218);

// Constructor is a C-ABI free function named __ct__CScnFogMan (retail has no
// mangled arg suffix), so it must be given extern "C" linkage to keep the name.
extern "C" void __ct__CScnFogMan(CScnFogMan* self, u32 param) {
    *(void**)self = lbl_eu_8056EBD0;
    self->unk04 = param;
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

extern "C" void func_8049DE68(u8* self, u32 val) {
    ((CScnFogMan*)self)->value08 = val;
}
extern "C" void func_8049DEC4();
extern "C" void func_8049DE70(void) { func_8049DEC4(); }
void func_8049E374(u8* self, float a, float b) { *(float*)(self + 0x20) = a; *(float*)(self + 0x1C) = b; }

// Retail dtor (free-function form so MWCC emits no local vtable; the retail
// vtable lives in the blob data above and the ctor stores it explicitly).
extern "C" void* __dt__10CScnFogManFv(CScnFogMan* self, int flag) {
    if (self != nullptr) {
        if (flag > 0) {
            __dl__FPv(self);
        }
    }
    return self;
}

extern "C" void func_8049E350(u8* self, const void* src) {
    *(u32*)((u8*)self + 0xC) = *(u32*)((u8*)src + 0);
    *(u32*)((u8*)self + 0x10) = *(u32*)((u8*)src + 4);
    *(u32*)((u8*)self + 0x14) = *(u32*)((u8*)src + 8);
    *(u32*)((u8*)self + 0x18) = *(u32*)((u8*)src + 0xC);
}

// Copy a fog parameter block into the near/far fog fields. mValue lands in
// field_0x50, the four floats into 0x54-0x60, and the source word-vector is
// mirrored into both 0x64-0x70 and 0xc-0x18. The word copies are bit-reinterpreted
// integer stores so they compile to lwz/stw (retail has no float conversion).
extern "C" void func_8049DE74(CScnFogMan* self, u32 value, const SWordVec* src,
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

#pragma push
#pragma auto_inline off
extern "C" void func_8049DEC4() {}
#pragma pop
