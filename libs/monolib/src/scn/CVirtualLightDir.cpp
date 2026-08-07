// Decompiled for monolib/src/scn/CVirtualLightDir

#include <types.h>

// Retail base-class ctor (no header declares it in this unit's scope).
extern "C" void __ct__CVirtualLightObj(void* self);

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
class CVirtualLightDir : public CVirtualLightObjBase {
public:
    CVirtualLightDir();
    virtual ~CVirtualLightDir();

    f32 mField34;   // +0x34
    f32 mField38;   // +0x38
    u8 _3C[0x1138]; // padding up to 0x1174
    u8 mUnk1174;    // +0x1174
};

CVirtualLightDir::CVirtualLightDir() {
    mField34 = 1000.0f;
    mField38 = 1000.0f;
    mField2C = 2;
}

// Stores the two f32 fields at 0x34/0x38 (retail symbol is unmangled C).
extern "C" void func_8049474C(CVirtualLightDir* self, float a, float b) {
    self->mField34 = a;
    self->mField38 = b;
}

CVirtualLightDir::~CVirtualLightDir() {}