// Decompiled for monolib/src/scn/CVirtualLightAmb

#include <types.h>
#include "libs/monolib/src/scn/CVirtualLightAmb.hpp"

extern "C" {
    extern void __dl__FPv(void* ptr);
    extern void __ct__CVirtualLightObj(void* self);
    extern char lbl_eu_8056E878[];  // CVirtualLightAmb vtable
}

extern "C" CVirtualLightAmb* __ct__CVirtualLightAmb(CVirtualLightAmb* self) {
    __ct__CVirtualLightObj(self);  // Initialize base portion
    *(void**)self = (void*)lbl_eu_8056E878;  // Set vtable pointer
    self->mField2C = 1;
    return self;
}

CVirtualLightAmb::~CVirtualLightAmb() {
}
