// Auto-scaffolded catalog TU for kyoshin/action/CActParamAnim
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/harness_catalog.hpp"

#include "kyoshin/action/CActParamAnim.hpp"
CActParamAnim::CActParamAnim() {}

void __dt__8004B070(){}

void CActParamAnim_initSub1() {}

void CActParamAnim_initSub2() {}

CActParamAnim::~CActParamAnim() {}

void CActParamAnim::func_8004B114() {}

void* CActParamAnim_getChild(CActParamAnim* self) { return &reinterpret_cast<CActParamAnimFull*>(self)->mChildData; }

float CActParamAnim_getBlendWeight(CActParamAnim* self) {
    return *(float*)((char*)self + 0x43c);
}

void func_8004B354(){}

int CActParamAnim_checkFlag(CActParamAnim* self, int mask) {
    return (*(int*)((char*)self) & mask) != 0 ? 1 : 0;
}

void CActParamAnim_copyTranslation(void* dst, const void* src) {
    *(int*)((char*)dst + 0) = *(int*)((char*)src + 0);
    *(int*)((char*)dst + 4) = *(int*)((char*)src + 4);
    *(int*)((char*)dst + 8) = *(int*)((char*)src + 8);
}

void func_8004B40C(){}

void func_8004B4A4(){}

void* CActParamAnim_getOwner(CActParamAnim* self) {
    return *(void**)((char*)self + 8);
}

bool CActParamAnim_isEnabled() { return true; }

void func_8004B52C(){}

void CActParamAnim_copyRotation(void* dst, const void* src) {
    *(int*)((char*)dst + 0xC) = *(int*)((char*)src + 0);
    *(int*)((char*)dst + 0x10) = *(int*)((char*)src + 4);
    *(int*)((char*)dst + 0x14) = *(int*)((char*)src + 8);
}

void vec3_set(float *data, float a, float b, float c) { data[0] = a; data[1] = b; data[2] = c; }

float CActParamAnim_getScale(CActParamAnim* self) {
    return *(float*)((char*)self + 0x444);
}

void func_8004B624(){}

void bits_clear(uint32_t* ptr, uint32_t mask) {
    *ptr &= ~mask;
}

void CActParamAnim_updateSubAnim(CActParamAnim* self) {
    extern void func_80055AC4(void*);
    if (*(int*)((char*)self + 0x3A4) != 0) func_80055AC4((char*)self + 16);
}

void func_8004B6BC(){}

void CActParamAnim_setOwner(CActParamAnim* self, int val) { *(int*)((char*)self + 8) = val; }

// STALLED: uses paired-single (PS) SIMD instructions (psq_l/ps_sum1/psq_st)
// that MWCC Wii 1.1 cannot generate from standard C. Requires DECOMP_ASM_INSN
// carve-outs per PLAN.md 17.6, but the entire function body is PS ops, making
// it effectively a whole-function asm body which is not permitted.
void func_8004B738(){}

void func_8004B75C(){}

void vec3_copy(float *dst, const float *src) {
    dst[0] = src[0];
    dst[1] = src[1];
    dst[2] = src[2];
}

float CActParamAnim_getAnimSpeed(const CActParamAnim* self) {
    return *(const float*)((const char*)self + 0x390);
}

void CActParamAnim_copyVec3To3C0(CActParamAnim* self, const float* src) {
    *(int*)((char*)self + 0x3C0) = *(int*)((char*)src + 0);
    *(int*)((char*)self + 0x3C4) = *(int*)((char*)src + 4);
    *(int*)((char*)self + 0x3C8) = *(int*)((char*)src + 8);
}

void func_8004B7DC(){}

void CActParamAnim_setVec3Y(CActParamAnim* self, float val) {
    *(float*)((char*)self + 0x3c4) = val;
}

int CActParamAnim_checkAnimFlag(CActParamAnim* self) {
    return (*(unsigned int *)((char *)self + 12) >> 1) & 1;
}

float func_8004B854__13CActParamAnimFv(CActParamAnim* self) {
    extern float func_80484F18(void*);
    extern float lbl_eu_80665EA0;
    void* obj = *(void**)((char*)self + 0x3A0);
    if (obj) return func_80484F18(obj);
    return lbl_eu_80665EA0;
}

float func_8004B86C__13CActParamAnimFv(CActParamAnim* self) {
    extern float func_804850A4(void*);
    extern float lbl_eu_80665EA0;
    void* obj = *(void**)((char*)self + 0x3A0);
    if (obj != 0) return func_804850A4(obj);
    return lbl_eu_80665EA0;
}

void CActParamAnim_callObjUpdate(CActParamAnim* self) {
    extern void func_80484F80(void*);
    void* obj = *(void**)((char*)self + 0x3A0);
    if (obj != 0) func_80484F80(obj);
}

float func_8004B898__13CActParamAnimFv(CActParamAnim* self) {
    extern float func_80484EB0(void*);
    extern float lbl_eu_80665EA0;
    void* obj = *(void**)((char*)self + 0x3A0);
    if (obj != 0) return func_80484EB0(obj);
    return lbl_eu_80665EA0;
}

void func_8004B8B0(){}

void func_8004B8F8(){}

extern int func_80485464(void*, void*);
extern int func_80485244(void*);
int CActParamAnim_callObjFunc(CActParamAnim* self, void* param) {
    void* val = *(void**)((char*)self + 0x3A0);
    if (!val) return 0;
    if (!param) {
        return func_80485244(val);
    }
    return func_80485464(val, param);
}

void* CActParamAnim_getNextChainObj(CActParamAnim* self) {
    void* v = *(void**)((char*)self + 0x8);
    if (v) {
        return *(void**)((char*)v + 0x18);
    }
    return *(void**)((char*)self + 0x4b4);
}

void func_8004B9D4(){}

void func_8004BC28(){}

void CActParamAnim_setBlendFlag(CActParamAnim* self, int param) {
    if (param != 0) {
        int tmp = *(int*)((char*)self + 0xc);
        *(unsigned char*)((char*)self + 0x4da) = 30;
        *(int*)((char*)self + 0xc) = tmp | 0x4000000;
    } else {
        *(int*)((char*)self + 0xc) &= ~0x4000000;
    }
}

void func_8004BC94(){}

void func_8004BDCC(){}

void func_8004C5E8__13CActParamAnimFv() {}

void* CActParamAnim_getModelObj(CActParamAnim* _this) {
    void* result = *(void**)((char*)_this + 0x2fc);
    if (result) return result;
    return *(void**)((char*)_this + 0x27c);
}

void func_8004C608(){}

void func_8004CB80(){}

void func_8004CBC8(){}

float Atan2FIdx__Q24nw4r4mathFff(float, float);
float CActParamAnim_atan2Scaled(float y, float x) {
    extern float lbl_eu_80665ED0;
    return lbl_eu_80665ED0 * Atan2FIdx__Q24nw4r4mathFff(y, x);
}

void func_8004CC68(){}

void func_8004CC74(){}

int CActParamAnim_checkRenderFlag(CActParamAnim* self) { return ((*(unsigned int*)((unsigned char*)self + 0x260) >> 5) & 1); }

void func_8004CC8C(){}

void CActParamAnim_setAnimCounter(CActParamAnim* obj, int value) {
    *(int*)((char*)obj + 0x374) = value;
}

void func_8004CF00(){}

void CActParamAnim::func_8004D074() {}

void CActParamAnim::func_8004D194() {}

void CActParamAnim::func_8004D2F8() {}

void CActParamAnim::func_8004D4AC() {}

void CActParamAnim::func_8004D650() {}

void CActParamAnim::func_8004D7EC() {}

void CActParamAnim::func_8004D950() {}

void* CActParamAnim_getEffObj(CActParamAnim* self) {
    return *(void**)((char*)self + 0x4c0);
}

void CActParamAnim_clearEffObj(CActParamAnim* self) {
    auto* full = reinterpret_cast<CActParamAnimFull*>(self);
    full->mEffByte0 = 0;
    full->mEffByte1 = 0;
    *(int *)((char *)self + 0x4c0) = 0;
}

void CActParamAnim::func_8004DAE0() {}

void CActParamAnim::func_8004DDD0() {}

void CActParamAnim::func_8004DF9C() {}

void CActParamAnim::func_8004E168() {}

void CActParamAnim::func_8004E334() {}

void CActParamAnim::func_8004E500() {}

void CActParamAnim::func_8004E694() {}

void CActParamAnim::func_8004E828() {}

void CActParamAnim::func_8004E9EC() {}

void func_8004EC78(){}

void CActParamAnim::func_8004ECF4() const {}

void CActParamAnim::func_8004F1E4() {}

void CActParamAnim::func_8004F334() {}

void CActParamAnim::func_8004F484() {}

void CActParamAnim::func_8004F5FC() {}

void CActParamAnim::func_8004F884() {}

void CActParamAnim::func_8004FAB4() {}

void CActParamAnim::func_8004FCE0() {}

void CActParamAnim::func_8004FE58() {}

void CActParamAnim::func_8004FFBC() {}

f32 func_800504BC(const f32* a, const f32* b){
    return a[0] * b[0] + a[1] * b[1] + a[2] * b[2];
}

void func_800504DC(){}

void CActParamAnim::func_80050744() {}

void CActParamAnim::func_80050890() {}

void CActParamAnim::func_80050C50() {}

void CActParamAnim::func_80050DB0() {}

void CActParamAnim::func_80050F5C() {}

void CActParamAnim::func_800510FC() {}

void CActParamAnim::func_800512A8() {}

void CActParamAnim::func_80051448() {}

void CActParamAnim::func_80051584() {}

void CActParamAnim::func_800516C0() {}

void CActParamAnim::func_800517FC() {}

void CActParamAnim::func_8005194C() {}

extern float lbl_eu_80665EA0;
void CActParamAnim_resetVec3Y(CActParamAnim* self) { *(float*)((u8*)self + 964) = lbl_eu_80665EA0; }

void CActParamAnim_setActiveFlag(CActParamAnim* self, s32 param) {
    if (param != 0) {
        *(u32*)((u8*)self + 0xC) |= 2;
    } else {
        *(u32*)((u8*)self + 0xC) &= ~2;
    }
}

void func_80051AD0(){}

void func_80051B38(){}

void CActParamAnim_startAnimA(CActParamAnim* this_) {
    *(unsigned int *)((char *)this_ + 0x4a8) = 0x44a05;
    *(unsigned int *)((char *)this_ + 0xc) |= 0x800000;
}

void CActParamAnim_startAnimA2(CActParamAnim* self) {
    extern float lbl_eu_80665F18;
    u32 v = *(u32*)((u8*)self + 12);
    float f = lbl_eu_80665F18;
    *(volatile u32*)((u8*)self + 0x4A8) = 0x44A05;
    *(volatile float*)((u8*)self + 0x484) = f;
    *(u32*)((u8*)self + 12) = v | 0x00800000;
}

void CActParamAnim_startAnimB(CActParamAnim* self) {
    extern float lbl_eu_80665F18;
    float tmp = lbl_eu_80665F18;
    *(volatile unsigned int*)((char*)self + 0x4A8) = 0x44A09;
    *(volatile float*)((char*)self + 0x484) = tmp;
}

void CActParamAnim_startAnimC(CActParamAnim* self) {
    extern float lbl_eu_80665F18;
    float tmp = lbl_eu_80665F18;
    *(volatile unsigned int*)((char*)self + 0x4A8) = 0x44A11;
    *(volatile float*)((char*)self + 0x484) = tmp;
}

void func_80051BF4(){}

void func_80051C40(){}

void CActParamAnim_stopAnim(CActParamAnim* self) {
    u32 val = *(u32*)((u8*)self + 12);
    u32 tmp = val & ~0x10u;
    *(u32*)((u8*)self + 12) = tmp;
    if ((val & 2u) == 0) return;
    tmp &= ~0x80u;
    *(u8*)((u8*)self + 1176) = 0;
    *(u32*)((u8*)self + 12) = tmp;
}

void func_80051CD4(){}

u32 CActParamAnim_checkFlag13(CActParamAnim* self) { return (*(u32*)((u8*)self + 608) >> 13) & 0x1u; }

float CActParamAnim_getParamFloat(CActParamAnim* self) { return *(float*)((char*)self + 0x4A0); }

float CActParamAnim_getField10(CActParamAnim* self) { return *(float *)((char *)self + 0x10); }

u32 CActParamAnim_checkFlag16(CActParamAnim* ptr) {
    return (*(u32*)((u8*)ptr + 0x260) >> 0x10) & 1;
}

u32 CActParamAnim_testAndClearFlag18(CActParamAnim* self) {
    u32 result = (*(u32*)((u8*)self + 0x260) >> 18) & 1u;
    *(volatile u32*)((u8*)self + 0x260) &= ~(1u << 18);
    return result;
}

void func_80052584(){}

void func_800526C0(){}

void func_8005274C(){}

void func_80052780(void* self, void* src){
    *(int*)((char*)self + 168) = *(int*)((char*)src + 0);
    *(int*)((char*)self + 172) = *(int*)((char*)src + 4);
    *(int*)((char*)self + 176) = *(int*)((char*)src + 8);
    *(int*)((char*)self + 180) = *(int*)((char*)src + 12);
    *(int*)((char*)self + 224) |= 4;
}

void func_800527B0(){}

void func_800527E8(){}

void CActParamAnim_mulVec3Y(CActParamAnim* param_1, float param_2) {
    *(float*)((char*)param_1 + 0x3c4) *= param_2;
}

void CActParamAnim::func_80052934() {}

void CActParamAnim::func_80053164() {}

void func_80053198(){}

void func_80053490(){}

extern "C" {
extern float lbl_eu_80665ECC;
}
extern "C" float CActParamAnim_getAnimConstant() { return lbl_eu_80665ECC; }

extern "C" int CActParamAnim_getChainInt(CActParamAnim* self) {
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
