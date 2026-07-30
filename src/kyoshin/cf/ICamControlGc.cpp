// ICamControlGc — bitfield property getters
// Each calls a virtual function (vtable+0x8) and extracts one flag bit.

#include "kyoshin/harness_catalog.hpp"

typedef void* (*VF_GetObj)(void*);

int func_80274DD8(void* self) {
    u32* vtable = *(u32**)self;
    VF_GetObj vf = (VF_GetObj)vtable[2];
    void* obj = vf(self);
    return (*(int*)obj >> 19) & 1;
}

int func_80274E0C(void* self) {
    u32* vtable = *(u32**)self;
    VF_GetObj vf = (VF_GetObj)vtable[2];
    void* obj = vf(self);
    return (*(int*)obj >> 20) & 1;
}

int func_80274E40(void* self) {
    u32* vtable = *(u32**)self;
    VF_GetObj vf = (VF_GetObj)vtable[2];
    void* obj = vf(self);
    return (*(int*)obj >> 17) & 1;
}

int func_80274E74(void* self) {
    u32* vtable = *(u32**)self;
    VF_GetObj vf = (VF_GetObj)vtable[2];
    void* obj = vf(self);
    return (*(int*)obj >> 18) & 1;
}
