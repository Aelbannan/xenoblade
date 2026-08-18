// Decompiled for monolib/src/scn/CScnFilter

#include <types.h>
#include <monolib/scn/CScnFilter.hpp>

extern "C" {
    extern void __dl__FPv(void* ptr);
    extern u32 lbl_eu_8056EB60[];   // CScnFilter vtable
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

// ===== Dissolved monolibdata2 (blob surgery) data owned by this TU =====
// [.data] 0x8056EB60-0x8056EB78 (24B): CScnFilter vtable. The typeinfo pair
// lbl_eu_806639F0 is owned by a foreign TU.
extern "C" u32 lbl_eu_806639F0;
extern "C" void __dt__10CScnFilterFv();
extern "C" u32 lbl_eu_8056EB60[6] = {
    (u32)&lbl_eu_806639F0, 0x00000000,
    (u32)&__dt__10CScnFilterFv,
    (u32)&func_8049C868,
    0x00000000, 0x00000000,
};
DECOMP_FORCEACTIVE(CScnFilter_cpp, lbl_eu_8056EB60);

// [.rodata] 0x805240AC-0x80524138 (140B, align 4): RTTI class-name strings.
// Referenced by foreign typeinfo objects (lbl_eu_806639F0 et al.).
extern "C" __declspec(align(4)) const char lbl_eu_805240AC[0xB] = { 0x43, 0x53, 0x63, 0x6E, 0x46, 0x69, 0x6C, 0x74, 0x65, 0x72, 0x00 };
extern "C" __declspec(align(4)) const char lbl_eu_805240B8[0xA] = { 0x43, 0x53, 0x63, 0x6E, 0x42, 0x6C, 0x6F, 0x6F, 0x6D, 0x00 };
extern "C" __declspec(align(4)) const char lbl_eu_805240C4[0x14] = { 0x43, 0x53, 0x63, 0x6E, 0x42, 0x6C, 0x6F, 0x6F, 0x6D, 0x42, 0x61, 0x73, 0x65, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };  // +6B tail pad (retail gap)
extern "C" __declspec(align(4)) const char lbl_eu_805240D8[0x1E] = { 0x63, 0x61, 0x6D, 0x73, 0x65, 0x74, 0x20, 0x25, 0x64, 0x20, 0x25, 0x76, 0x33, 0x20, 0x25, 0x76, 0x33, 0x20, 0x25, 0x66, 0x20, 0x25, 0x66, 0x20, 0x25, 0x66, 0x20, 0x25, 0x66, 0x00 };
extern "C" __declspec(align(4)) const char lbl_eu_805240F8[0xE] = { 0x43, 0x53, 0x63, 0x6E, 0x43, 0x61, 0x6D, 0x65, 0x72, 0x61, 0x4D, 0x61, 0x6E, 0x00 };
extern "C" __declspec(align(4)) const char lbl_eu_80524108[0x12] = { 0x43, 0x53, 0x63, 0x6E, 0x45, 0x66, 0x66, 0x65, 0x63, 0x74, 0x41, 0x63, 0x74, 0x4E, 0x77, 0x34, 0x72, 0x00 };
extern "C" __declspec(align(4)) const char lbl_eu_8052411C[0xB] = { 0x49, 0x45, 0x66, 0x66, 0x65, 0x63, 0x74, 0x41, 0x63, 0x74, 0x00 };
extern "C" __declspec(align(4)) const char lbl_eu_80524128[0xC] = { 0x43,0x53,0x63,0x6E,0x46,0x61,0x64,0x65,0x4D,0x61,0x6E,0x00 };
DECOMP_FORCEACTIVE(CScnFilter_cpp, lbl_eu_80524128);
// retail tail: 4B zero object at +0x88 (kept PROGBITS via explicit section)
extern "C" __declspec(section ".rodata") const char lbl_eu_80524134[0x4] = { 0x00,0x00,0x00,0x00 };
DECOMP_FORCEACTIVE(CScnFilter_cpp, lbl_eu_80524134);
