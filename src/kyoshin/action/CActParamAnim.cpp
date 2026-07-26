// Auto-scaffolded catalog TU for kyoshin/action/CActParamAnim
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/harness_catalog.hpp"

extern "C" void __ct__13CActParamAnimFv() {}

extern "C" void __dt__8004B070() {}

extern "C" void func_8004B0B0() {}

extern "C" void func_8004B0B4() {}

extern "C" void __dt__13CActParamAnimFv() {}

extern "C" void func_8004B114__13CActParamAnimFv() {}

extern "C" void* func_8004B344(void* self) { return static_cast<char*>(self) + 0x10; }

extern "C" float func_8004B34C(void* self) {
    return *(float*)((char*)self + 0x43c);
}

extern "C" void func_8004B354() {}

extern "C" int func_8004B3D8(void* self, int mask) {
    return (*(int*)((char*)self) & mask) != 0 ? 1 : 0;
}

extern "C" void func_8004B3F0(void* dst, const void* src) {
    *(int*)((char*)dst + 0) = *(int*)((char*)src + 0);
    *(int*)((char*)dst + 4) = *(int*)((char*)src + 4);
    *(int*)((char*)dst + 8) = *(int*)((char*)src + 8);
}

extern "C" void func_8004B40C() {}

extern "C" void func_8004B4A4() {}

extern "C" void* func_8004B51C(void* self) {
    return *(void**)((char*)self + 8);
}

extern "C" bool func_8004B524() { return true; }

extern "C" void func_8004B52C() {}

extern "C" void func_8004B5F0(void* dst, const void* src) {
    *(int*)((char*)dst + 0xC) = *(int*)((char*)src + 0);
    *(int*)((char*)dst + 0x10) = *(int*)((char*)src + 4);
    *(int*)((char*)dst + 0x14) = *(int*)((char*)src + 8);
}

extern "C" void func_8004B60C(float *data, float a, float b, float c) { data[0] = a; data[1] = b; data[2] = c; }

extern "C" float func_8004B61C(void* self) {
    return *(float*)((char*)self + 0x444);
}

extern "C" void func_8004B624() {}

extern "C" void func_8004B694(uint32_t* ptr, uint32_t mask) {
    *ptr &= ~mask;
}

extern "C" void func_8004B6A4(void* self) {
    extern void func_80055AC4(void*);
    if (*(int*)((char*)self + 0x3A4) != 0) func_80055AC4((char*)self + 16);
}

extern "C" void func_8004B6BC() {}

extern "C" void func_8004B730(void* self, int val) { *(int*)((char*)self + 8) = val; }

// STALLED: uses paired-single (PS) SIMD instructions (psq_l/ps_sum1/psq_st)
// that MWCC Wii 1.1 cannot generate from standard C. Requires DECOMP_ASM_INSN
// carve-outs per PLAN.md 17.6, but the entire function body is PS ops, making
// it effectively a whole-function asm body which is not permitted.
extern "C" void func_8004B738() {}

extern "C" void func_8004B75C() {}

extern "C" void func_8004B79C(float *dst, const float *src) {
    dst[0] = src[0];
    dst[1] = src[1];
    dst[2] = src[2];
}

extern "C" float func_8004B7B8(const void* self) {
    return *(const float*)((const char*)self + 0x390);
}

extern "C" void func_8004B7C0(void* self, const void* src) {
    *(int*)((char*)self + 0x3C0) = *(int*)((char*)src + 0);
    *(int*)((char*)self + 0x3C4) = *(int*)((char*)src + 4);
    *(int*)((char*)self + 0x3C8) = *(int*)((char*)src + 8);
}

extern "C" void func_8004B7DC() {}

extern "C" void func_8004B840(void* self, float val) {
    *(float*)((char*)self + 0x3c4) = val;
}

extern "C" int func_8004B848(void *self) {
    return (*(unsigned int *)((char *)self + 12) >> 1) & 1;
}

extern "C" float func_8004B854__13CActParamAnimFv(void* self) {
    extern float func_80484F18(void*);
    extern float lbl_eu_80665EA0;
    void* obj = *(void**)((char*)self + 0x3A0);
    if (obj) return func_80484F18(obj);
    return lbl_eu_80665EA0;
}

extern "C" float func_8004B86C__13CActParamAnimFv(void* self) {
    extern float func_804850A4(void*);
    extern float lbl_eu_80665EA0;
    void* obj = *(void**)((char*)self + 0x3A0);
    if (obj != 0) return func_804850A4(obj);
    return lbl_eu_80665EA0;
}

extern "C" void func_8004B884(void* self) {
    extern void func_80484F80(void*);
    void* obj = *(void**)((char*)self + 0x3A0);
    if (obj != 0) func_80484F80(obj);
}

extern "C" float func_8004B898__13CActParamAnimFv(void* self) {
    extern float func_80484EB0(void*);
    extern float lbl_eu_80665EA0;
    void* obj = *(void**)((char*)self + 0x3A0);
    if (obj != 0) return func_80484EB0(obj);
    return lbl_eu_80665EA0;
}

extern "C" void func_8004B8B0() {}

extern "C" void func_8004B8F8() {}

extern "C" {
extern int func_80485464(void*, void*);
extern int func_80485244(void*);
}
extern "C" int func_8004B990(void* self, void* param) {
    void* val = *(void**)((char*)self + 0x3A0);
    if (!val) return 0;
    if (!param) {
        return func_80485244(val);
    }
    return func_80485464(val, param);
}

extern "C" void* func_8004B9B8(void* self) {
    void* v = *(void**)((char*)self + 0x8);
    if (v) {
        return *(void**)((char*)v + 0x18);
    }
    return *(void**)((char*)self + 0x4b4);
}

extern "C" void func_8004B9D4() {}

extern "C" void func_8004BC28() {}

extern "C" void func_8004BC64(void* self, int param) {
    if (param != 0) {
        int tmp = *(int*)((char*)self + 0xc);
        *(unsigned char*)((char*)self + 0x4da) = 30;
        *(int*)((char*)self + 0xc) = tmp | 0x4000000;
    } else {
        *(int*)((char*)self + 0xc) &= ~0x4000000;
    }
}

extern "C" void func_8004BC94() {}

extern "C" void func_8004BDCC() {}

extern "C" void func_8004C5E8__13CActParamAnimFv() {}

extern "C" void* func_8004C5EC(void* _this) {
    void* result = *(void**)((char*)_this + 0x2fc);
    if (result) return result;
    return *(void**)((char*)_this + 0x27c);
}

extern "C" void func_8004C608() {}

extern "C" void func_8004CB80() {}

extern "C" void func_8004CBC8() {}

extern "C" float Atan2FIdx__Q24nw4r4mathFff(float, float);
extern "C" float func_8004CC40(float y, float x) {
    extern float lbl_eu_80665ED0;
    return lbl_eu_80665ED0 * Atan2FIdx__Q24nw4r4mathFff(y, x);
}

extern "C" void func_8004CC68() {}

extern "C" void func_8004CC74() {}

extern "C" int func_8004CC80(void *self) { return ((*(unsigned int*)((unsigned char*)self + 0x260) >> 5) & 1); }

extern "C" void func_8004CC8C() {}

extern "C" void func_8004CEF8(void* obj, int value) {
    *(int*)((char*)obj + 0x374) = value;
}

extern "C" void func_8004CF00() {}

extern "C" void func_8004D074__13CActParamAnimFv() {}

extern "C" void func_8004D194__13CActParamAnimFv() {}

extern "C" void func_8004D2F8__13CActParamAnimFv() {}

extern "C" void func_8004D4AC__13CActParamAnimFv() {}

extern "C" void func_8004D650__13CActParamAnimFv() {}

extern "C" void func_8004D7EC__13CActParamAnimFv() {}

extern "C" void func_8004D950__13CActParamAnimFv() {}

extern "C" void* func_8004DAC4(void* self) {
    return *(void**)((char*)self + 0x4c0);
}

extern "C" void func_8004DACC(void *self) {
    ((char *)self)[0x4be] = 0;
    ((char *)self)[0x4bf] = 0;
    *(int *)((char *)self + 0x4c0) = 0;
}

extern "C" void func_8004DAE0__13CActParamAnimFv() {}

extern "C" void func_8004DDD0__13CActParamAnimFv() {}

extern "C" void func_8004DF9C__13CActParamAnimFv() {}

extern "C" void func_8004E168__13CActParamAnimFv() {}

extern "C" void func_8004E334__13CActParamAnimFv() {}

extern "C" void func_8004E500__13CActParamAnimFv() {}

extern "C" void func_8004E694__13CActParamAnimFv() {}

extern "C" void func_8004E828__13CActParamAnimFv() {}

extern "C" void func_8004E9EC__13CActParamAnimFv() {}

extern "C" void func_8004EC78() {}

extern "C" void func_8004ECF4__13CActParamAnimFv() {}

extern "C" void func_8004F1E4__13CActParamAnimFv() {}

extern "C" void func_8004F334__13CActParamAnimFv() {}

extern "C" void func_8004F484__13CActParamAnimFv() {}

extern "C" void func_8004F5FC__13CActParamAnimFv() {}

extern "C" void func_8004F884__13CActParamAnimFv() {}

extern "C" void func_8004FAB4__13CActParamAnimFv() {}

extern "C" void func_8004FCE0__13CActParamAnimFv() {}

extern "C" void func_8004FE58__13CActParamAnimFv() {}

extern "C" void func_8004FFBC__13CActParamAnimFv() {}

extern "C" f32 func_800504BC(const f32* a, const f32* b) {
    return a[0] * b[0] + a[1] * b[1] + a[2] * b[2];
}

extern "C" void func_800504DC() {}

extern "C" void func_80050744__13CActParamAnimFv() {}

extern "C" void func_80050890__13CActParamAnimFv() {}

extern "C" void func_80050C50__13CActParamAnimFv() {}

extern "C" void func_80050DB0__13CActParamAnimFv() {}

extern "C" void func_80050F5C__13CActParamAnimFv() {}

extern "C" void func_800510FC__13CActParamAnimFv() {}

extern "C" void func_800512A8__13CActParamAnimFv() {}

extern "C" void func_80051448__13CActParamAnimFv() {}

extern "C" void func_80051584__13CActParamAnimFv() {}

extern "C" void func_800516C0__13CActParamAnimFv() {}

extern "C" void func_800517FC__13CActParamAnimFv() {}

extern "C" void func_8005194C__13CActParamAnimFv() {}

extern float lbl_eu_80665EA0;
extern "C" void func_80051A9C(void* self) { *(float*)((u8*)self + 964) = lbl_eu_80665EA0; }

extern "C" void func_80051AA8(void* self, s32 param) {
    if (param != 0) {
        *(u32*)((u8*)self + 0xC) |= 2;
    } else {
        *(u32*)((u8*)self + 0xC) &= ~2;
    }
}

extern "C" void func_80051AD0() {}

extern "C" void func_80051B38() {}

extern "C" void func_80051B84(void *this_) {
    *(unsigned int *)((char *)this_ + 0x4a8) = 0x44a05;
    *(unsigned int *)((char *)this_ + 0xc) |= 0x800000;
}

extern "C" void func_80051BA0(void* self) {
    extern float lbl_eu_80665F18;
    u32 v = *(u32*)((u8*)self + 12);
    float f = lbl_eu_80665F18;
    *(volatile u32*)((u8*)self + 0x4A8) = 0x44A05;
    *(volatile float*)((u8*)self + 0x484) = f;
    *(u32*)((u8*)self + 12) = v | 0x00800000;
}

extern "C" void func_80051BC4(void* self) {
    extern float lbl_eu_80665F18;
    float tmp = lbl_eu_80665F18;
    *(volatile unsigned int*)((char*)self + 0x4A8) = 0x44A09;
    *(volatile float*)((char*)self + 0x484) = tmp;
}

extern "C" void func_80051BDC(void* self) {
    extern float lbl_eu_80665F18;
    float tmp = lbl_eu_80665F18;
    *(volatile unsigned int*)((char*)self + 0x4A8) = 0x44A11;
    *(volatile float*)((char*)self + 0x484) = tmp;
}

extern "C" void func_80051BF4() {}

extern "C" void func_80051C40() {}

extern "C" void func_80051CAC(void* self) {
    u32 val = *(u32*)((u8*)self + 12);
    u32 tmp = val & ~0x10u;
    *(u32*)((u8*)self + 12) = tmp;
    if ((val & 2u) == 0) return;
    tmp &= ~0x80u;
    *(u8*)((u8*)self + 1176) = 0;
    *(u32*)((u8*)self + 12) = tmp;
}

extern "C" void func_80051CD4() {}

extern "C" u32 func_80052540(void* self) { return (*(u32*)((u8*)self + 608) >> 13) & 0x1u; }

extern "C" float func_8005254C(void* self) { return *(float*)((char*)self + 0x4A0); }

extern "C" float func_80052554(void *self) { return *(float *)((char *)self + 0x10); }

extern "C" u32 func_8005255C(void* ptr) {
    return (*(u32*)((u8*)ptr + 0x260) >> 0x10) & 1;
}

extern "C" u32 func_80052568(void* self) {
    u32 result = (*(u32*)((u8*)self + 0x260) >> 18) & 1u;
    *(volatile u32*)((u8*)self + 0x260) &= ~(1u << 18);
    return result;
}

extern "C" void func_80052584() {}

extern "C" void func_800526C0() {}

extern "C" void func_8005274C() {}

extern "C" void func_80052780(void* self, void* src) {
    *(int*)((char*)self + 168) = *(int*)((char*)src + 0);
    *(int*)((char*)self + 172) = *(int*)((char*)src + 4);
    *(int*)((char*)self + 176) = *(int*)((char*)src + 8);
    *(int*)((char*)self + 180) = *(int*)((char*)src + 12);
    *(int*)((char*)self + 224) |= 4;
}

extern "C" void func_800527B0() {}

extern "C" void func_800527E8() {}

extern "C" void func_80052924(void* param_1, float param_2) {
    *(float*)((char*)param_1 + 0x3c4) *= param_2;
}

extern "C" void func_80052934__13CActParamAnimFv() {}

extern "C" void func_80053164__13CActParamAnimFv() {}

extern "C" void func_80053198() {}

extern "C" void func_80053490() {}

extern "C" {
extern float lbl_eu_80665ECC;
}
extern "C" float func_80053958() { return lbl_eu_80665ECC; }

extern "C" int func_80053960(void* self) {
    int val = *(int*)((char*)self + 1220);
    if (val) return val;
    void* ptr = *(void**)((char*)self + 8);
    if (ptr) return *(int*)((char*)ptr + 24);
    return *(int*)((char*)self + 1204);
}

extern u8 lbl_eu_80663D4C;
extern "C" void func_eu_80053FC8() { lbl_eu_80663D4C = 0; }

extern "C" u8 lbl_eu_80663D4C;
extern "C" u8 func_eu_80053FD4() { return lbl_eu_80663D4C; }

extern u8 lbl_eu_80663D4C;
extern "C" void func_eu_80053FDC() { lbl_eu_80663D4C = 1; }

extern "C" bool func_80053990__13CActParamAnimFv() { return false; }

extern "C" bool func_80053998__13CActParamAnimFv() { return false; }

extern "C" bool func_800539A0__13CActParamAnimFv() { return false; }

extern "C" bool func_800539A8__13CActParamAnimFv() { return false; }

extern "C" bool func_800539B0__13CActParamAnimFv() { return false; }

extern "C" bool func_800539B8__13CActParamAnimFv() { return false; }

extern "C" bool func_800539C0__13CActParamAnimFv() { return false; }

extern "C" bool func_800539C8__13CActParamAnimFv() { return false; }

extern "C" bool func_800539D0__13CActParamAnimFv() { return false; }

extern "C" bool func_800539D8__13CActParamAnimFv() { return false; }

// --- hard-symbol stubs (scaffold_hard_symbols) ---
extern "C" void sinit_800539E0() {
    extern float lbl_eu_8066AF20;
    extern float lbl_eu_80663D48;
    lbl_eu_80663D48 = lbl_eu_8066AF20;
}
