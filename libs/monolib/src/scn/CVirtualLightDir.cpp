// Decompiled for monolib/src/scn/CVirtualLightDir

#include <types.h>
#include "monolib/scn/CVirtualLightDir.hpp"

// C-linkage imports (retail symbol names - keep linkage/signatures verbatim)
// Retail CVirtualLightDir vtable data (monolibdata2.s @0x8056E898) and the
// shared sdata2 {0.0f, 0.0f} constant (monolibdata2.s @0x8066AA80). The
// classes are __declspec(novtable), so ctor/dtor never emit a local vtable
// and assign the retail label explicitly instead of the compiler-generated
// __vt__16CVirtualLightDir / __vt__20CVirtualLightObjBase (which would add
// .data/.rodata/RTTI to a retail-empty TU).
extern "C" u32 lbl_eu_8056E898[];
extern "C" const float lbl_eu_8066AA80; // 0.0f (first float of the 8-byte pair)

// CVirtualLightObj-compatible prefix laid out exactly to 0x34 so that the
// derived CVirtualLightDir extension fields land at 0x34/0x38. Keeping the
// base polymorphic (its vptr occupies +0x00, matching CVirtualLightObj's own
// vtable slot) makes the data members start at +0x04. __declspec(novtable)
// stops MWCC from emitting an extra inline vptr store before the base-ctor
// call - the store inside __ct__CVirtualLightObj is the only base vptr write,
// exactly as retail does.
struct __declspec(novtable) CVirtualLightObjBase {
public:
    CVirtualLightObjBase() { __ct__CVirtualLightObj(this); }
    // A non-dtor virtual makes the base polymorphic so its vptr occupies +0x00
    // (data members start at +0x04) WITHOUT adding a base-dtor that a derived
    // dtor would then have to chain (retail's derived dtor chains none).
    virtual void vfunc();

    f32 mField04;   // +0x04 = 0.0f
    f32 mField08;   // +0x08 = 0.0f
    f32 mField0C;   // +0x0C = 0.0f
    f32 mField10;   // +0x10 = 0.0f
    f32 mField14;   // +0x14 = 0.0f
    u8 mByte18;     // +0x18 = 1
    u8 mByte19;     // +0x19 = 0
    f32 mField1C;   // +0x1C = 0.0f
    f32 mField20;   // +0x20 = 0.0f
    f32 mField24;   // +0x24 = 0.0f
    f32 mField28;   // +0x28 = 0.0f
    s32 mField2C;   // +0x2C
    s32 mField30;   // +0x30
};

// Directional light node: CVirtualLightObj prefix + f32 extension fields.
class __declspec(novtable) CVirtualLightDir : public CVirtualLightObjBase {
public:
    CVirtualLightDir();
    virtual ~CVirtualLightDir();

    f32 mField34;   // +0x34
    f32 mField38;   // +0x38
    u8 _3C[0x1138]; // padding up to 0x1174
    u8 mUnk1174;    // +0x1174
};

CVirtualLightDir::CVirtualLightDir() {
    *(void**)this = (void*)lbl_eu_8056E898;
    mField34 = lbl_eu_8066AA80;
    mField38 = lbl_eu_8066AA80;
    mField2C = 2;
}

// Stores the two f32 fields at 0x34/0x38 (retail symbol is unmangled C).
extern "C" void func_8049474C(CVirtualLightDir* self, float a, float b) {
    self->mField34 = a;
    self->mField38 = b;
}

CVirtualLightDir::~CVirtualLightDir() {}

// ===== Dissolved monolibdata2 (blob surgery) data owned by this TU =====
namespace VLBlob {
extern "C" void __dt__16CVirtualLightDirFv();
extern "C" void func_804947EC();
extern "C" void func_8049488C();
}
extern "C" u32 lbl_eu_806639D8;   // .sdata (foreign)
extern "C" u32 lbl_eu_806639D0;   // .sdata (foreign)

// [.data] 0x8056E898-0x8056E8B8 (32B): CVirtualLightDir vtable
extern "C" u32 lbl_eu_8056E898[8] = {
    (u32)&lbl_eu_806639D8, 0x00000000,
    (u32)&VLBlob::__dt__16CVirtualLightDirFv,
    (u32)&VLBlob::func_804947EC,
    (u32)&VLBlob::func_8049488C,
    (u32)&lbl_eu_806639D0, 0x00000000, 0x00000000,
};

// [.rodata] 0x80524078-0x805240A0 (40B)
extern "C" __declspec(align(4)) const char lbl_eu_80524078[0x28] = {
    0x43,0x56,0x69,0x72,0x74,0x75,0x61,0x6C,0x4C,0x69,0x67,0x68,0x74,0x44,0x69,0x72,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x25,0x73,0x28,0x25,0x73,0x29,0x00,0x43,0x53,0x63,0x6E,0x00,0x00,0x00,0x00,
};
