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
    *(void**)self = (void*)lbl_eu_8056EB60;
    self->mEnabled = 0;
    self->mUnk0C = 0;
    self->mIntensity = lbl_eu_8066ABA0;
    self->mUnk14 = 0;
    self->mFlags = (u32)self->mEnabled | 3;
}

CScnFilter::~CScnFilter() {
}

static int check_over(CScnFilter* self) {
    int over;
    if (self->mUnk0C != 0 && self->mIntensity > (f32)self->mUnk0C) {
        over = 1;
    } else {
        over = 0;
    }
    return over;
}

extern "C" void func_8049C868(CScnFilter* self, void* arg) {
    if (self->mUnk0C == 0) {
        return;
    }
    if (check_over(self)) {
        return;
    }
    void* obj = *(void**)((char*)arg + 0x84);
    self->mIntensity += *(f32*)((char*)obj + 0x0C);
    if (check_over(self)) {
        if (self->mUnk14 != 0) {
            void* cb = (void*)self->mUnk14;
            void (*func)(void*) = (void (*)(void*))(*(void**)((char*)*(void**)cb + 0x0C));
            func(cb);
        }
    }
}
