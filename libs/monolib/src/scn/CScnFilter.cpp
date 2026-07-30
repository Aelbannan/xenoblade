// Decompiled for monolib/src/scn/CScnFilter

#include <types.h>
#include <monolib/scn/CScnFilter.hpp>

extern "C" {
    extern void __dl__FPv(void* ptr);
    extern char lbl_eu_8056EB60[];   // CScnFilter vtable
    extern const float lbl_eu_8066ABA0;   // 0.0f
    extern const double lbl_eu_8066ABA8;  // 0x4330000000000000 (int-to-float magic)
}

extern "C" void __ct__CScnFilter(CScnFilter* self) {
    u32 zero = 0;
    f32 fzero = lbl_eu_8066ABA0;
    void* vtable = (void*)lbl_eu_8056EB60;
    u32 flags3 = 3;
    self->mEnabled = 0;
    *(void**)self = vtable;
    self->mUnk0C = zero;
    self->mIntensity = fzero;
    self->mUnk14 = zero;
    self->mFlags = flags3;
}

CScnFilter::~CScnFilter() {
}

extern "C" void func_8049C868(CScnFilter* self, void* arg) {
    s32 threshold = self->mUnk0C;
    if (threshold == 0) {
        return;
    }
    f32 threshold_f = (f32)threshold;
    if (self->mIntensity > threshold_f) {
        return;
    }
    void* obj = *(void**)((char*)arg + 0x84);
    self->mIntensity += *(f32*)((char*)obj + 0x0C);
    threshold = self->mUnk0C;
    threshold_f = (f32)threshold;
    if (self->mIntensity > threshold_f) {
        if (self->mUnk14 != 0) {
            void* cb = (void*)self->mUnk14;
            void* vt = *(void**)cb;
            void (*func)(void*) = (void (*)(void*))(*(void**)((char*)vt + 0x0C));
            func(cb);
        }
    }
}
