// Auto-scaffolded catalog TU for monolib/src/scn/CScnEnvLgtCtrl
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>

extern "C" void func_804C6B64(void* self, u32 val) { *(u32*)((u8*)self + 0xa8) = val; }










extern "C" void func_804C1500() {}

extern "C" void func_804C1600() {}

extern "C" void func_804C1674() {}

extern "C" void func_804C1720(void) {}

extern "C" void func_804C172C() {}

extern "C" void func_804C190C() {}

extern "C" void func_804C19B8() {}

extern "C" void func_804C1A64() {}

extern "C" void func_804C1AFC() {}

extern "C" void func_804C1BA0() {}

extern "C" void func_804C1D7C() {}

extern "C" void func_804C1F10() {}

extern "C" void func_804C2014() {}

extern "C" void func_804C2094() {}

extern "C" void func_804C2124() {}

extern "C" void func_804C22F0() {}

extern "C" void func_804C2654() {}

extern "C" void func_804C26F0() {}

extern "C" void __ct__CScnEnvLgtCtrl() {}

extern "C" void __dt__14CScnEnvLgtCtrlFv() {}

extern "C" void func_804C30E8() {}

extern "C" void func_804C31C8() {}

extern "C" void func_804C33F0() {}

extern "C" void func_804C3404() {}

extern "C" void func_804C34A0() {}

extern "C" void func_804C3778() {}

extern "C" void func_804C392C() {}

extern "C" void func_804C3AC8() {}

extern "C" void func_804C3C9C() {}

extern "C" void func_804C3F58() {}

extern "C" void func_804C406C() {}

extern "C" void func_804C42A8() {}

extern "C" void func_804C43A4() {}

extern "C" void func_804C4954() {}

extern "C" void func_804C4D28() {}

extern "C" void func_804C4E04() {}

extern "C" void func_804C5198() {}

extern "C" bool func_804C51D4(void* r3, void* r4) {
    if (!(*(unsigned int*)((char*)r3 + 4) & 0x400)) return false;
    unsigned int v7 = *(unsigned int*)((char*)r3 + 0x64);
    unsigned int v6 = *(unsigned int*)((char*)r3 + 0x68);
    unsigned int v5 = *(unsigned int*)((char*)r3 + 0x6c);
    unsigned int v0 = *(unsigned int*)((char*)r3 + 0x70);
    *(unsigned int*)((char*)r4 + 0) = v7;
    *(unsigned int*)((char*)r4 + 4) = v6;
    *(unsigned int*)((char*)r4 + 8) = v5;
    *(unsigned int*)((char*)r4 + 0xc) = v0;
    return true;
}

extern "C" void func_804C5210() {}

extern "C" void func_804C526C() {}

extern "C" void func_804C5380() {}

extern "C" void func_804C54D4() {}

extern "C" void func_804C5628() {}

extern "C" void func_804C58D8() {}

extern "C" void func_804C5990() {}

extern "C" void func_804C5A48() {}

extern "C" void func_804C5B00() {}

extern "C" void func_804C5C08() {}

extern "C" void func_804C5C6C() {}

extern "C" void func_804C5D7C(void* self, unsigned int* out) {
    struct SelfStruct {
        char pad[0x40];
        void* ptr;
    };
    SelfStruct* s = static_cast<SelfStruct*>(self);
    void* subPtr = s->ptr;
    if (subPtr == nullptr) return;
    struct SubStruct {
        unsigned short flags;
        unsigned short value;
    };
    SubStruct* sub = static_cast<SubStruct*>(subPtr);
    if (!(sub->flags & 1)) return;
    *out = sub->value;
}

extern "C" void func_804C5DA0() {}

extern "C" void func_804C5E04() {}

extern "C" void func_804C5E9C() {}

extern "C" void func_804C5F6C() {}

extern "C" void func_804C5FB0() {}

extern "C" void func_804C6010(void* _this, int r4, int r5) {
    void* r3 = *(void**)((char*)_this + 0x24);
    if (r3 == nullptr) {
        return;
    }
    if (r5 > 1) {
        r5 = 0;
    }
    int r0 = -r4;
    r0 |= r4;
    r0 >>= 31;
    r0 &= 2;
    r5 += r0;
    char* ptr = (char*)r3 + r5 * 0x30;
    unsigned short val = *(unsigned short*)(ptr + 0x28);
    val &= 0xFFFB;
    *(unsigned short*)(ptr + 0x28) = val;
}

extern "C" void func_804C6054() {}

extern "C" void func_804C6110() {}

extern "C" void func_804C64A8() {}

extern "C" void func_804C678C() {}

extern "C" void func_804C6A70() {}

extern "C" u32 func_804C6ADC(u32 val) { return (val >> 16) & 0xFF; }

extern "C" int func_804C6AE8(unsigned int* arg0) {
    return (arg0[1] >> 28) & 1;
}

extern "C" void func_804C6AF4(void* ptr, int flag) {
    unsigned int* word = (unsigned int*)((char*)ptr + 4);
    if (flag != 0)
        *word |= 0x40000000;
    else
        *word &= ~0x40000000;
}

extern "C" void func_804C6B1C(void* r3, int r4) {
    if (r4 != 0) {
        *(unsigned int*)((char*)r3 + 4) |= 0x80000000u;
    } else {
        *(unsigned int*)((char*)r3 + 4) &= ~0x80000000u;
    }
}

extern "C" void func_804C6B44() {}


extern "C" void func_804C6B6C() {}

extern "C" float func_804C6B88(void* self) { return *(float*)((u8*)self + 0xc8); }

extern "C" void func_804C6B90(void* self, float val) { *(float*)((u8*)self + 0xc8) = val; }

extern "C" float func_804C6B98(void* self) { return *(float*)((u8*)self + 0xcc); }

extern "C" void func_804C6BA0(void* self, float val) { *(float*)((u8*)self + 0xcc) = val; }

extern "C" void func_804C6BA8() {}

extern "C" void func_804C6D64() {}

extern "C" void func_804C6F78() {}

extern "C" void func_804C7190() {}

extern "C" void func_804C7530() {}

extern "C" void func_804C7564() {}

extern "C" void func_804C7578() {}

extern "C" void func_804C758C() {}

extern "C" void func_804C75A0() {}

extern "C" u32 func_804C75B4(u32 unused, void* obj, float val) { *(float*)((char*)obj + 0) = val; return unused; }

extern "C" u32 func_804C75BC(u32 unused, void* obj, float val) { *(float*)((char*)obj + 4) = val; return unused; }

extern "C" u32 func_804C75C4(u32 unused, void* obj, float val) { *(float*)((char*)obj + 8) = val; return unused; }

extern "C" void func_804C75CC() {}

extern "C" void func_804C75E0() {}

extern "C" void func_804C75F4() {}

extern "C" u32 func_804C7608(u32 unused, void* obj, float val) { *(float*)((char*)obj + 40) = val; return unused; }

extern "C" u32 func_804C7610(u32 unused, void* obj, float val) { *(float*)((char*)obj + 44) = val; return unused; }

extern "C" u32 func_804C7618(u32 unused, void* obj, float val) { *(float*)((char*)obj + 48) = val; return unused; }

extern "C" void func_804C7620() {}

extern "C" void func_804C763C(int dummy, float f, void* ptr) {
    int val = (int)f;
    *(int*)((char*)ptr + 0x38) = val;
}

extern "C" void func_804C7658() {}

extern "C" u32 func_804C7674(u32 unused, void* obj, float val) { *(float*)((char*)obj + 184) = val; return unused; }

extern "C" u32 func_804C767C(u32 unused, void* obj, float val) { *(float*)((char*)obj + 188) = val; return unused; }

extern "C" u32 func_804C7684(u32 unused, void* obj, float val) { *(float*)((char*)obj + 192) = val; return unused; }

extern "C" u32 func_804C768C(u32 unused, void* obj, float val) { *(float*)((char*)obj + 196) = val; return unused; }

extern "C" u32 func_804C7694(u32 unused, void* obj, float val) { *(float*)((char*)obj + 200) = val; return unused; }

extern "C" u32 func_804C769C(u32 unused, void* obj, float val) { *(float*)((char*)obj + 204) = val; return unused; }

extern "C" u32 func_804C76A4(u32 unused, void* obj, float val) { *(float*)((char*)obj + 208) = val; return unused; }

extern "C" void func_804C76AC() {}

extern "C" void func_804C76C8() {}

extern "C" void func_804C76E4() {}

extern "C" void func_804C7700() {}

extern "C" void func_804C771C(int, void* ptr, float value) {
    int i = (int)value;
    *(short*)((char*)ptr + 0xe) = (short)i;
}

extern "C" void func_804C7738() {}

extern "C" u32 func_804C7754(u32 unused, void* obj, float val) { *(float*)((char*)obj + 200) = val; return unused; }

extern "C" u32 func_804C775C(u32 unused, void* obj, float val) { *(float*)((char*)obj + 204) = val; return unused; }

extern "C" u32 func_804C7764(u32 unused, void* obj, float val) { *(float*)((char*)obj + 208) = val; return unused; }

extern "C" u32 func_804C776C(u32 unused, void* obj, float val) { *(float*)((char*)obj + 212) = val; return unused; }

extern "C" void func_804C7774() {}

extern "C" void func_804C7790() {}

extern "C" void func_804C7880() {}

extern "C" void func_804C7910() {}

extern "C" void func_804C7B54() {}

extern "C" void func_804C8054() {}

// --- hard-symbol stubs (scaffold_hard_symbols) ---
extern "C" void sinit_804C8174() {}
