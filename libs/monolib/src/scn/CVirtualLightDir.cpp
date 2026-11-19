// Decompiled for monolib/src/scn/CVirtualLightDir

#include <types.h>
#include "monolib/scn/CVirtualLightDir.hpp"

CVirtualLightDir::CVirtualLightDir() {
    mField34 = 0.0f;
    mField38 = 0.0f;
    mField2C = 2;
}

// Stores the two f32 fields at 0x34/0x38 (retail symbol is unmangled C).
extern "C" void VirtLight_SetPair(CVirtualLightDir* self, float a, float b) {
    self->mField34 = a;
    self->mField38 = b;
}

CVirtualLightDir::~CVirtualLightDir() {}
