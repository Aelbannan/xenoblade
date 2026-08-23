#include "kyoshin/cf/CfCam.hpp"
#include <nw4r/math.h>
#include <math.h>
#include <monolib/math.hpp>
#include "kyoshin/cf/CfCam_ps.inl"
// CfGameManager.hpp's extern "C" func_8049603C(CScn*) conflicts with this
// TU's no-arg caller shape - rename it out of the way (CMenuGetItemMulti.cpp
// pattern).
#define func_8049603C cfCamGameManager9603CUnused
// Redundant fwd decl: MWCC 10140 rejects CfGameManager.hpp's qualified
// cf::CfObjectMove uses despite its own bare fwd decl, so pre-declare here.
namespace cf {
class CfObjectMove;
}
#include "kyoshin/cf/CfGameManager.hpp"
#undef func_8049603C
#include "monolib/core/CPadManager.hpp"
#include "monolib/device/CDeviceRemotePad.hpp"
#include <revolution/mtx/quat.h>
#include <revolution/mtx/mtxvec.h>
// Runtime RTTI helper + typeinfo pair for func_80073DDC's dynamic_cast
// (fixed-size 8-byte typeinfo decls keep them sdata-eligible -> li @sda21).
void* __dynamic_cast(void* obj, long offset, const void* src_type,
                     const void* dst_type, void* src2dst);
extern char lbl_eu_80661B28[8];
extern char lbl_eu_80661B30[8];
// C++-linkage global; MWCC mangles it to func_8006EF04__Fi (the retail name).
int func_8006EF04(int gate);
// Cross-TU imports (flat retail names).
extern "C" int func_804BE398(void* vec, u32 a, u32 b, u32 c);
extern "C" int func_8006BA80(void* pos);
// In-TU definitions further down this file.
int func_80071D50(ml::CVec3* out, u32 probeArg, float f);
int func_8006E18C(cf::CfCamFollow* self, float val);
void func_8006BFDC(cf::CfCamFollow* self);
void func_80074090(ml::CVec3* out, cf::CfCamSrcVt* src, ml::CVec3* pos, int sel);
void func_80074010(nw4r::math::VEC3* a, nw4r::math::VEC3* b, nw4r::math::VEC3* c, float f);

// cf::CfCam base ctor, C linkage so the retail symbol name is emitted verbatim.
// noinline keeps the `bl __ct__cf_CfCam` in the follow ctor (body is in-TU).
extern "C" __declspec(noinline) void* __ct__cf_CfCam(void* self, void* arg);
// In-TU CVec3::zero copy; noinline keeps the retail `bl func_8006BEC4` sites.
extern "C" __declspec(noinline) void func_8006BEC4(void* self);
extern "C" {
void func_8004B0B0(void* self);
void* func_8004B60C(void* self, f32 x, f32 y, f32 z);
void* memset(void* dest, int val, u32 count);

char lbl_eu_80527260[];
char lbl_eu_805272E8[];
cf::CfCamFollow* lbl_eu_80663DEC;

extern const f32 lbl_eu_806662DC; // 0.0f
extern const f32 lbl_eu_806662B8; // 8.0f
extern f32 lbl_eu_80661B50;       // 40.0f (.sdata)
extern const f32 lbl_eu_806662A0; // 0.1f
extern const f32 lbl_eu_806662F0; // 0.4f
extern const f32 lbl_eu_806662D0; // 1.0f
}

extern "C" cf::CfCamFollow* __ct__cf_CfCamFollow(cf::CfCamFollow* self, void* arg1,
                                                 void* arg2) {
    __ct__cf_CfCam(self, arg2);
    self->vtable = lbl_eu_80527260;

    func_8004B0B0(self->unk1C);
    func_8004B60C(self->unk1C + 12, lbl_eu_806662DC, lbl_eu_806662DC, lbl_eu_806662DC);
    func_8004B60C(self->unk1C + 24, lbl_eu_806662DC, lbl_eu_806662DC, lbl_eu_806662DC);
    func_8004B0B0(self->unk1C + 36);
    func_8004B0B0(self->unk1C + 48);
    func_8004B0B0(self->unk1C + 60);
    func_8004B60C(self->unk1C + 72, lbl_eu_806662DC, lbl_eu_806662DC, lbl_eu_806662DC);
    func_8006BEF8(self->unk70);
    func_8006BEC0(self->unk130);

    self->unk164 = arg1;

    func_8004B0B0(self->unk168);
    func_8004B0B0(self->unk168 + 12);
    func_8004B60C(self->unk168 + 24, lbl_eu_806662DC, lbl_eu_806662DC, lbl_eu_806662DC);
    func_8004B60C(self->unk168 + 36, lbl_eu_806662DC, lbl_eu_806662DC, lbl_eu_806662DC);
    func_8004B0B0(self->unk168 + 48);
    func_8004B0B0(self->unk168 + 60);
    func_8004B0B0(self->unk168 + 72);
    func_8004B0B0(self->unk168 + 84);
    func_8004B60C(self->unk168 + 96, lbl_eu_806662DC, lbl_eu_806662DC, lbl_eu_806662DC);
    func_8006BEFC(&self->unk1D4);
    func_8006BF08(&self->unk1D8);

    self->unk1DC = lbl_eu_806662DC;
    self->unk1E0 = lbl_eu_80661B50;
    self->unk1E4 = lbl_eu_806662DC;
    self->unk1E8 = lbl_eu_806662DC;
    self->unk1EC = lbl_eu_806662DC;
    self->unk1F0 = lbl_eu_806662DC;
    self->unk1F4 = lbl_eu_806662DC;
    self->unk1F8 = lbl_eu_806662DC;
    self->unk1FC = lbl_eu_806662B8;
    self->unk200 = lbl_eu_806662DC;
    self->unk204 = lbl_eu_806662A0;
    self->unk21C = lbl_eu_806662DC;
    self->unk220 = lbl_eu_806662DC;
    self->unk224 = lbl_eu_806662DC;
    self->unk228 = lbl_eu_806662F0;
    self->unk234 = lbl_eu_806662DC;
    self->unk238 = lbl_eu_806662DC;
    self->unk24C = 0;
    self->unk252 = 0;
    self->unk254 = 0;
    self->unk256 = 0;
    self->unk258 = 0;

    lbl_eu_80663DEC = self;
    self->unk160 = arg2;

    func_8006BEC4(self->unk10);
    func_8006BEC4(self->unk168 + 60);
    func_8006BEC4(self->unk168 + 72);
    func_8006BEC4(self->unk168 + 84);
    func_8006BEE4(self);
    memset(func_8006BF14(self->unk70, 0), 0, 0xC0);

    self->unk250 = 0;
    self->unk22C = lbl_eu_806662D0;
    self->unk230 = lbl_eu_806662D0;

    // Real virtual calls: MWCC loads the vtable into r12 (lwz r12, 0(r29);
    // lwz r12, slot(r12)) right before each bctrl, matching retail's epilogue.
    reinterpret_cast<cf::CfCamCtorVt*>(self)->v14(1);
    reinterpret_cast<cf::CfCamCtorVt*>(self)->v08();

    return self;
}

extern "C" void* cfCam_getUnk10Ptr(cf::CfCamFollow* self) { return self->unk10; }
extern "C" u32 cfCam_getUnk164(const cf::CfCamFollow* self) { return reinterpret_cast<u32>(self->unk164); }
extern "C" void* cfCam_getUnk40Ptr(cf::CfCamFollow* ptr) { return ptr->unk1C + 0x24; }
extern "C" void* cfCam_getUnk1CPtr(cf::CfCamFollow* self) { return self->unk1C; }
extern "C" bool cfCam_getTrue() { return true; }
// func_8006BA80: zero-vector test - true when every component magnitude is
// within the shared 0x8066A208 epsilon. Two-stage boolean structure mirrors
// retail (|x|,|y| checked first, then |z| behind a temp flag).
int func_8006BA80(const ml::CVec3* v) {
    int result = 0;
    int temp = 0;
    if (ml::math::abs(v->x) <= lbl_eu_8066A208 && ml::math::abs(v->y) <= lbl_eu_8066A208) {
        temp = 1;
    }
    if (temp != 0 && ml::math::abs(v->z) <= lbl_eu_8066A208) {
        result = 1;
    }
    return result;
}
// func_8006BAF0: |*self| (single-precision abs of a float). Defined under the
// C++-mangled name so the extern "C" declaration in CfCam.hpp has no in-TU
// body - MWCC then keeps a real `bl func_8006BAF0` at call sites (retail
// keeps the call across every caller).
float func_8006BAF0(u8* self) {
    double x = *(float*)self;
    x = __fabs(x);
    return (float)x;
}
extern "C" float cfCam_vecMag(const float* v) {
    return PSVECMag(reinterpret_cast<const Vec*>(v));
}
extern "C" void cfCam_setOrClearUnk04Bits(cf::CfCamFollow* r3, int r4, int r5) {
    cf::CfCamFollow* self = static_cast<cf::CfCamFollow*>(r3);
    if (r5) {
        self->unk04 |= r4;
    } else {
        self->unk04 &= ~r4;
    }
}
void cf::CfCamFollow::clearUnk04Bits(unsigned int mask) {
    unk04 &= ~mask;
}
// cf::CfCam base ctor: stores the retail vtable manually, zeroes +0x04 and
// saves the caller argument at +0x0C (matches CfCamFollow's flat-layout view).
extern "C" __declspec(noinline) void* __ct__cf_CfCam(void* self, void* arg) {
    cf::CfCam* cam = static_cast<cf::CfCam*>(self);
    cam->vtable = lbl_eu_805272E8;
    cam->field_0x04 = 0;
    cam->field_0x0C = arg;
    return self;
}
// cf::CfCam dtor: empty body - MWCC still emits the null-check + delete-flag
// machinery (cmpwi r3,0 / cmpwi r4,0; ble / bl __dl__FPv).
cf::CfCam::~CfCam() {}
// cf::CfCamEvent dtor: empty body - MWCC emits the null-check, the base
// CfCamFollow dtor call (flag 0) and the delete-flag machinery.
// NOTE: retail __dt__Q22cf10CfCamEventFv cannot be defined here - CfCamEvent.hpp
// (read-only) does not declare ~CfCamEvent().
extern "C" __declspec(noinline) void func_8006BEC0(void* self) {}
// Copy ml::CVec3::zero into a caller-provided CVec3 (retail: lwzu+lwz/stw
// struct-copy triple from the static zero vector). noinline keeps the bl.
extern "C" __declspec(noinline) void func_8006BEC4(void* self) {
    *static_cast<ml::CVec3*>(self) = ml::CVec3::zero;
}
extern "C" __declspec(noinline) void func_8006BEE4(cf::CfCamFollow* p) {
    p->unk04 = 0;
}
extern "C" void cfCam_setUnk08(cf::CfCamFollow* self, int val) { self->unk08 = val; }
extern "C" __declspec(noinline) void func_8006BEF8(void* self) {}
__declspec(noinline) void func_8006BEFC(void* self) { *(u32*)self = 0; }
extern "C" u32 cfCam_getUnk08(cf::CfCamFollow* self) { return self->unk08; }
extern "C" bool cfCam_testUnk04Bits(const cf::CfCamFollow* self, unsigned int mask) { return (self->unk04 & mask) != 0; }
extern "C" int cfCam_getBit1_0x64(void* p) {
    return (*(int*)((char*)p + 0x64) >> 1) & 1;
}
extern "C" void cfCam_setField1E0AndGlobal(cf::CfCamFollow* ptr, float f) {
    lbl_eu_80661B50 = f;
    ptr->unk1E0 = f;
}
extern "C" void cfCam_setOrClearUnk1D4Bits(cf::CfCamFollow* obj, unsigned int bits, int set_flag) {
    cf::CfCamFollow* self = static_cast<cf::CfCamFollow*>(obj);
    if (set_flag != 0)
        self->unk1D4 |= bits;
    else
        self->unk1D4 &= ~bits;
}
extern "C" int cfCam_getUnkC4(void* self) { return *(int*)((char*)self + 0xc4); }
extern "C" int cfCam_getBit0_0x530(void* p) {
    return *(unsigned short*)((unsigned char*)p + 0x530) & 1;
}
extern "C" bool cfCam_testBits2_3_0x530(const unsigned char* this_) { return (*(const unsigned short*)(this_ + 0x530) & 0x000C) != 0; }
extern "C" bool cfCam_testBits4_5_0x530(const u8* self) { return (*(const unsigned short*)((const unsigned char*)self + 0x530) & 0x30) != 0; }
extern "C" bool cfCam_testUnk1D4Bits(cf::CfCamFollow* self, unsigned int mask) { return (self->unk1D4 & mask) != 0; }
// func_8006C6CC: copy the translation column of a 3x4 matrix (row-major) into a vec3.
// Declare x/y/z first (FPR coloring x->f0, y->f1, z->f2), assign in reverse
// (loads descend), store forward (stores ascend) - MWCC_CASES §10158 shape.
extern "C" void func_8006C6CC(float* dst, const float* src) {
    float x, y, z;
    z = src[11];
    y = src[7];
    x = src[3];
    dst[0] = x;
    dst[1] = y;
    dst[2] = z;
}
extern "C" void cfCam_clearUnk1D4Bits(cf::CfCamFollow* _this, unsigned int mask) {
    _this->unk1D4 &= ~mask;
}
extern "C" int cfCam_getBit1_0x4EC(void* _this) {
    return (*(int*)((char*)_this + 0x4EC) >> 1) & 1;
}
// Store the shared follow-distance float into *out, then re-init the global
// camera-relative position vec3.
extern "C" void func_8006CBD8(u8* unused, f32* out) {
    *out = lbl_eu_80663DE4;
    func_8004B3F0(unused, lbl_eu_80570A38);
}
// func_8006CBEC: store the follow distance into both .sbss mirrors and the
// first component of the shared camera-relative position into both .bss vec3s.
extern "C" void func_8006CBEC(const ml::CVec3* v, f32 f) {
    float x = v->x;
    lbl_eu_80663DE8 = f;
    lbl_eu_80663DE4 = f;
    lbl_eu_80570A44[0] = x;
    lbl_eu_80570A38[0] = x;
}
// func_8006CC4C: copy the shared follow-distance float into the .sbss mirror,
// then re-init the first component of the global camera-relative vec3.
extern "C" void func_8006CC4C() {
    lbl_eu_80663DE8 = lbl_eu_80663DE4;
    lbl_eu_80570A44[0] = lbl_eu_80570A38[0];
}
extern "C" void cfCam_zeroUnk1D4(cf::CfCamFollow* arg0) {
    arg0->unk1D4 = 0;
}
extern "C" u32 cfCam_getSignBitUnk04(cf::CfCamFollow* p) {
    return p->unk04 >> 31;
}
extern "C" u32 cfCam_getUnk04(cf::CfCamFollow* self) { return self->unk04; }
extern "C" float cfCam_getField1E0(cf::CfCamFollow* self) { return self->unk1E0; }
extern "C" float cfCam_scaleByConst(float f) {
    extern float lbl_eu_8066A20C;
    return f * lbl_eu_8066A20C;
}
extern "C" void cfCam_setBit9_Unk04(unsigned* p) {
    p[1] |= 0x200;
}
extern "C" void* cfCam_getUnk9CPtr(void* ptr) { return (char*)ptr + 0x9c; }
extern "C" bool cfCam_testUnk04Bit8(void* param_1) {
    return ((bool (*)(void*, int))cfCam_testUnk04Bits)(param_1, 256);
}
unsigned int cf::CfCamFollow::getBit26_0x4EC() {
    return (*(unsigned int*)((u8*)this + 0x4ec) >> 26) & 1;
}
extern "C" int cfCam_getBit24_0x4EC(void* p) {
    return (*(unsigned int*)((char*)p + 0x4EC) >> 24) & 1;
}
extern "C" int cfCam_testUnk4ECBits(void* self, unsigned int mask) { return ((*((unsigned int*)((char*)self + 0x4ec)) & mask) != 0); }
extern "C" uint32_t cfCam_getBit25_0x4EC(void* self) {
    return (*(uint32_t*)((char*)self + 0x4ec) >> 25) & 1;
}
extern "C" int cfCam_getBit23_0x4EC(void* self) {
    return (*(const unsigned int*)((const char*)self + 0x4ec) >> 23) & 1;
}
float cf::CfCam::getUnk4FC() {
    return *(float *)((char *)this + 0x4FC);
}
extern "C" float cfCam_getUnk4F8(void* self) { return *(float*)((char*)self + 0x4f8); }
extern "C" float cfCam_getUnk508(const void* self) {
    return *(const float*)((const char*)self + 0x508);
}
extern "C" int cfCam_getBit13_0x4EC(void* self) { return (*(uint32_t*)((char*)self + 0x4ec) >> 13) & 1; }
extern "C" unsigned int cfCam_getBit2_0x64(void* self) {
    unsigned int val = *(unsigned int*)((unsigned char*)self + 0x64);
    return (val >> 2) & 1U;
}
extern "C" int cfCam_getUnk90E4(u8* self) { return *(int *)((char *)self + 0x90e4); }
extern "C" unsigned int cfCam_getBit27_0x64(u8* self) {
    unsigned int word = *(unsigned int *)((char *)self + 0x64);
    return (word >> 27) & 1U;
}
extern "C" f32 func_8006DFC8(const nw4r::math::VEC3* vector) {
    return nw4r::math::VEC3LenSq(vector);
}
extern int lbl_eu_80663DF0;

extern "C" int cfCam_getGlobal_80663DF0()
{
    return lbl_eu_80663DF0;
}
extern "C" bool cfCam_callCheckFlag(void* self) { return func_800755B0(self, 0); }
extern "C" void cfCam_copyInt(int* dst, int* src) {
    *dst = *src;
}
extern "C" int cfCam_getConst16() { return 0x10; }
extern "C" void cfCam_copyU32(unsigned* dst, const unsigned* src) {
    *dst = *src;
}
extern "C" void cfCam_copyU16(u16* dest, const u16* src) {
    *dest = *src;
}
extern unsigned int lbl_eu_80663E28;

extern "C" int cfCam_getBit24_GlobalE28() {
    return (lbl_eu_80663E28 >> 24) & 1;
}
extern "C" void* cfCam_getUnk3D8Ptr(void* self) { return static_cast<char*>(self) + 0x3d8; }
extern "C" int cfCam_getBit9_Arg1(unsigned int* arg0) {
    return (arg0[1] >> 9) & 1;
}
// func_8006EF1C: whole-block copy of the 48-byte (12-word) camera data block.
void func_8006EF1C(CfCamWordBlock48* dst, const CfCamWordBlock48* src) {
    *dst = *src;
}
// func_8006F9D4: flag-bit probe on lbl_eu_80663E24 with a fixed mask.
// Branchless idiom: r0 = flag; r3 = r0 & mask; r3 = -r3; r0 = r0 | r3; return r0 & 1.
extern "C" bool func_8006F9D4() {
    unsigned int flag = lbl_eu_80663E24;
    unsigned int x = flag & 0xAFA40000u;
    return ((((unsigned int)-(int)x | x) >> 31) & 1) != 0;
}
__declspec(noinline) void func_80071694(void* a, void* b){}
extern "C" void cfCam_copy4Words(void *r3, const void *r4) {
    const unsigned int *src = (const unsigned int *)r4;
    unsigned int *dst = (unsigned int *)r3;
    dst[0] = src[0];
    dst[1] = src[1];
    dst[2] = src[2];
    dst[3] = src[3];
}
extern "C" int cfCam_getBit12_Unk04(void* ptr) {
    return (*(unsigned int*)((char*)ptr + 4) >> 12) & 1;
}
extern "C" void cfCam_clearUnk04Bits_EDFF(cf::CfCamFollow* self) { self->unk04 &= 0xFFFFEDFFu; }

extern "C" bool cfCam_setUnk1E0AndValidate(void* obj, float val) {
    *(float*)((char*)obj + 0x1e0) = val;
    return func_8049EB60(obj);
}
extern "C" void cfCam_stub_80071B74() {}
extern "C" void cfCam_stub_80071B78() {}
extern "C" int cfCam_getGlobal_80665958() {
    extern int lbl_eu_80665958;
    return lbl_eu_80665958;
}
extern "C" void* cfCam_getUnk60Ptr(void* self) { return static_cast<char*>(self) + 0x60; }
extern "C" void* cfCam_getUnk6CPtr(void* ptr) {
    return (char*)ptr + 0x6c;
}
extern "C" int cfCam_getBit19_0x0C(void* _this) {
    return ((*(unsigned*)((char*)_this + 0xc)) >> 19) & 1;
}
// func_8006EF04__Fi: flag-bit probe.  Retail codegen is the branchless idiom
// r0 = flag; r3 = r0 & mask; r3 = -r3; r0 = r0 | r3; return r0 & 1.
extern "C" bool func_8006EF04__Fi(s32 mask) {
    unsigned int flag = lbl_eu_80663E24;
    unsigned int x = flag & (unsigned int)mask;
    return ((((unsigned int)-(int)x | x) >> 31) & 1) != 0;
}

extern "C" int cfCam_getBit6_GlobalE24() {
    return (lbl_eu_80663E24 >> 6) & 1;
}
extern "C" float cfCam_getUnk14Float(const void* arg) { return *(const float*)((const char*)arg + 0x14); }
extern "C" void cfCam_resetUnk10(u8* p, void* src) {
    func_8004B3F0(p + 0x10, src);
}
// func_8007420C: copy the translation column of a 3x4 matrix (row-major) into a vec3.
// Declare x/y/z first (FPR coloring x->f0, y->f1, z->f2), assign in reverse
// (loads descend), store forward (stores ascend) - MWCC_CASES §10158 shape.
extern "C" void func_8007420C(float* dst, const float* src) {
    float x, y, z;
    z = src[11];
    y = src[7];
    x = src[3];
    dst[0] = x;
    dst[1] = y;
    dst[2] = z;
}
extern "C" void cfCam_setMtxTranslation(float* dst, const float* src) {
    dst[3] = src[0];
    dst[7] = src[1];
    dst[11] = src[2];
}
// func_800743C0: in-place inverse of a rigid-transform 3x4 matrix: transpose
// the rotation part and replace the translation with -(R^T * t). The three
// negated translation components are named locals first (retail loads them
// before the nine rotation entries), then the three products are computed in
// _13/_03/_23 order into locals so all translation stores land after the
// transpose stores, like retail.
void func_800743C0(nw4r::math::MTX34* m) {
    f32 n13 = -m->_13;
    f32 n03 = -m->_03;
    f32 n23 = -m->_23;
    f32 r10 = m->_10;
    f32 r01 = m->_01;
    f32 r12 = m->_12;
    f32 r11 = m->_11;
    f32 r21 = m->_21;
    f32 r02 = m->_02;
    f32 r00 = m->_00;
    f32 r20 = m->_20;
    f32 r22 = m->_22;

    f32 t13 = r01 * n03 + r11 * n13 + r21 * n23;
    f32 t03 = r00 * n03 + r10 * n13 + r20 * n23;
    f32 t23 = r02 * n03 + r12 * n13 + r22 * n23;

    m->_01 = r10;
    m->_10 = r01;
    m->_02 = r20;
    m->_20 = r02;
    m->_12 = r21;
    m->_21 = r12;

    m->_03 = t03;
    m->_13 = t13;
    m->_23 = t23;
}
extern "C" bool cfCam_testBits(const unsigned* value, unsigned mask) { return (*value & mask) != 0; }
extern "C" int cfCam_getZero(void* self) { return 0x0; }
extern "C" void* cfCam_getNestedPtr118(cf::CfCamFollow* p) {
    return static_cast<char*>(p->unk0C) + 0x118;
}
extern "C" float cfCam_getFloat1E0(cf::CfCamFollow* self) { return self->unk1E0; }
extern "C" void cfCam_copyVec3U32(u32* self, const u32* src)
{
    self[10] = src[0];
    self[11] = src[1];
    self[12] = src[2];
}
void sinit_80074D7C(){}

struct Class_80296898 {
    static void* getInstance();
};

// func_8006B8E4: refresh the shared camera tables from the Class_80296898
// instance's selection bytes. inst[3] picks the 0x80527208 table entry (copied
// into the .sdata float 0x80661B3C); inst[2] picks the 0x8052721C table entry
// and the +0x40/+0x44/+0x48/+0x74 .sdata floats. The two .sdata2 constants
// stay in FPRs (f1/f0).
void func_8006B8E4() {
    const unsigned char* inst = reinterpret_cast<const unsigned char*>(Class_80296898::getInstance());
    unsigned int idx3 = inst[3];
    lbl_eu_80661B3C = lbl_eu_80527208[idx3];
    unsigned int idx2 = inst[2];
    lbl_eu_80661B74 = lbl_eu_8052721C[idx2];
    lbl_eu_80661B40 = lbl_eu_806662B0;
    lbl_eu_80661B44 = lbl_eu_806662B0;
    lbl_eu_80661B48 = lbl_eu_806662B4;
}

extern "C" void cfCam_updateLbl61BA8() {
    unsigned char idx = reinterpret_cast<unsigned char*>(Class_80296898::getInstance())[4];
    lbl_eu_80661BA8 = lbl_eu_80527230[idx];
}
// func_8006B980: camera-constant init + selection-byte table refresh. Stores
// the initial .sdata camera globals (positions, follow distance, flag word,
// selection byte) from .sdata2 constants, then mirrors the three
// Class_80296898 selection bytes through the 0x80527208 / 0x8052721C /
// 0x80527230 tables (func_8006B8E4 plus the +0x4 entry) and hands both args
// to func_8006B720.
void func_8006B980(int arg1, int arg2) {
    lbl_eu_80661B60 = lbl_eu_806662B8;
    lbl_eu_80661B98 = lbl_eu_806662BC;
    lbl_eu_80661B9C = lbl_eu_806662C0;
    lbl_eu_80661B7C = 4;
    lbl_eu_80661B8C = lbl_eu_806662C4;
    lbl_eu_80661B90 = lbl_eu_806662C8;
    lbl_eu_80663DE1 = 0;
    lbl_eu_80661B38 = lbl_eu_806662CC;
    lbl_eu_80663DD0 = lbl_eu_806662D0 / lbl_eu_806662CC;
    const_cast<float&>(lbl_eu_80663DD8) = lbl_eu_8066A210 * lbl_eu_806662D4;
    const u8* inst = reinterpret_cast<const u8*>(Class_80296898::getInstance());
    lbl_eu_80661B3C = lbl_eu_80527208[inst[3]];
    unsigned int idx2 = inst[2];
    lbl_eu_80661B74 = lbl_eu_8052721C[idx2];
    lbl_eu_80661B40 = lbl_eu_806662B0;
    lbl_eu_80661B44 = lbl_eu_806662B0;
    lbl_eu_80661B48 = lbl_eu_806662B4;
    const u8* inst2 = reinterpret_cast<const u8*>(Class_80296898::getInstance());
    lbl_eu_80661BA8 = lbl_eu_80527230[inst2[4]];
    func_8006B720(arg1, arg2);
}
// func_8006BB20: sine-table blend factor. Offsets the input FIdx angle by
// lbl_eu_806662D8; when positive, a (1.0 - x / lbl_eu_806662E0) factor is
// clamped into [0,1], scaled by pi/2 * lbl_eu_806662E4 and fed through the
// nw4r sine table; otherwise the initial 1.0 falls through unchanged (retail
// returns f31 which was preloaded with 1.0 before the branch). A result above
// 0.1 is wrapped back into [0, 2pi) with fmod (arg selects whether the wrap
// runs at all).
float func_8006BB20(void* arg, float angle) {
    f32 x = angle - lbl_eu_806662D8;
    f32 s = lbl_eu_806662DC; // 0.0f - sin-input accumulator (retail f2)
    f32 v = lbl_eu_806662D0; // 1.0f - return value / clamp cap (retail f31)
    if (x > s) {
        f32 t = v - x / lbl_eu_806662E0;
        if (t >= s) {
            if (t <= v) {
                v = t;
            }
            s = v;
        }
        v = nw4r::math::SinFIdx(s * lbl_eu_8066A200 * lbl_eu_806662E4);
        if (arg != 0 && v > lbl_eu_806662A0) {
            f32 m = (f32)fmod((double)v, lbl_eu_806662E8);
            if (v >= lbl_eu_806662DC) {
                v -= m;
            } else {
                v += m;
            }
        }
    }
    return v;
}
// func_8006BFDC: reset the camera constants block - refreshes the shared
// direction vec3 (0x80570A20 -> 0x80570A2C), stores the tuning constants into
// the .sdata globals, claims the 0x4 / 0x8 / 0x200 flags, reinitializes the
// selection tables (func_8006B980) and resets the object through vtable slot
// 0xC.
void func_8006BFDC(cf::CfCamFollow* self) {
    func_8004B3F0(lbl_eu_80570A2C, lbl_eu_80570A20);
    f32 cF8 = lbl_eu_806662F8;
    f32 c304 = lbl_eu_80666304;
    f32 cBC = lbl_eu_806662BC;
    f32 cC0 = lbl_eu_806662C0;
    f32 cF4 = lbl_eu_806662F4;
    f32 cE0 = lbl_eu_806662E0;
    f32 cB4 = lbl_eu_806662B4;
    f32 cFC = lbl_eu_806662FC;
    f32 c300 = lbl_eu_80666300;
    f32 cDC = lbl_eu_806662DC;
    f32 cD0 = lbl_eu_806662D0;
    f32 cB8 = lbl_eu_806662B8;
    f32 c308 = lbl_eu_80666308;
    f32 c30C = lbl_eu_8066630C;
    f32 cC4 = lbl_eu_806662C4;
    f32 cC8 = lbl_eu_806662C8;
    f32 c310 = lbl_eu_80666310;
    f32 c314 = lbl_eu_80666314;
    lbl_eu_80661B3C = cF4;
    lbl_eu_80661B74 = cF4;
    lbl_eu_80661B40 = cF8;
    lbl_eu_80661B44 = cF8;
    lbl_eu_80661B48 = cB4;
    lbl_eu_80661B4C = cFC;
    lbl_eu_80661B50 = c300;
    lbl_eu_80663DDC = cDC;
    lbl_eu_80661B54 = c304;
    lbl_eu_80661B58 = c304;
    lbl_eu_80661B5C = cD0;
    lbl_eu_80661B60 = cB8;
    lbl_eu_80661B64 = c308;
    lbl_eu_80661B68 = cBC;
    lbl_eu_80661B6C = cBC;
    lbl_eu_80661B70 = cE0;
    lbl_eu_80661B78 = c30C;
    lbl_eu_80661B7C = 4;
    lbl_eu_80661B80 = cF8;
    lbl_eu_80661B84 = cE0;
    lbl_eu_80661B88 = c304;
    lbl_eu_80661B8C = cC4;
    lbl_eu_80661B90 = cC8;
    lbl_eu_80661B94 = cC0;
    lbl_eu_80661B98 = cBC;
    lbl_eu_80661B9C = cC0;
    lbl_eu_80661BA0 = c310;
    lbl_eu_80661BA8 = c314;
    lbl_eu_80661BAC = cC0;
    func_8006BBF4(self, 0x4, 1);
    func_8006BBF4(self, 0x8, 1);
    func_8006B980(lbl_eu_80661BA4, 0);
    func_8006BBF4(self, 0x200, 1);
    reinterpret_cast<cf::CfCamVt01*>(self)->fnAt0C(0);
}
// func_8006C16C: stash the parent handle at +0x164, then - when a handle is
// present and the sibling check func_8006C1B0 accepts it - run the global
// 0x80661BA4 state routine.
void func_8006C16C(cf::CfCamFollow* self, void* arg) {
    self->unk164 = arg;
    if (arg != 0 && func_8006C1B0(arg) != 0) {
        func_8006B720(lbl_eu_80661BA4, 0);
    }
}
void func_8006C1C8(){}
// func_8006CA2C: follow-cam position update. When the follow target handle
// (+0x164) reports 0 from its vtable slot 0x74, re-arm the 0x40 camera flag.
// func_8006C740 derives a source vector, func_80073C7C transforms it, the
// result is fanned out to the +0x28/+0x34/+0x1C vec3 slots (func_8004B3F0
// returns its dst, feeding the next copy), and the +0x20 heading float
// accumulates the +0x1DC follow distance.
void func_8006CA2C(cf::CfCamFollow* self, void* arg2, float f1) {
    if (self->unk164 != 0) {
        if (reinterpret_cast<cf::CfCamPosSource*>(self->unk164)->fn0x74() == 0) {
            func_8006BBF4(self, 0x40, 1);
        }
    }
    ml::CVec3 local2;
    ml::CVec3 local1;
    func_8006C740(&local1, self);
    func_80073C7C(&local2, self, &local1, arg2, f1);
    func_8004B3F0(self->unk1C,
                  func_8004B3F0(self->unk1C + 24, func_8004B3F0(self->unk1C + 12, &local2)));
    *(reinterpret_cast<f32*>(self->unk1C) + 1) += self->unk1DC;
}
// func_8006C740: follow-camera source-vector derivation. When the follow
// target handle at +0x164 is present (and passes the func_800B8920 /
// func_800BBC0C liveness probes) the vtable-slot-0xAC aim vector is copied
// into local_2c (overridden by self+0x1BC when the 0x2000 flag is set and
// func_8006C670 reports a child), then offset by lbl_eu_80570A2C. A probe
// point is built 0x806662F8 ahead and collision-tested (mask 0x4); on a hit
// a height clamp (lbl_eu_8066631C) lifts the camera and the 0x800 flag is
// set, else a second probe at +0x80666320 runs when the child exists. The
// 0x2 flag is set with the hit bit and the result vector is copied out.
void func_8006C740(ml::CVec3* out, cf::CfCamFollow* self) {
    if (self->unk164 == 0) {
        func_8004B79C(out, reinterpret_cast<const ml::CVec3*>(self->unk10));
        return;
    }
    if (func_800B8920(self->unk164) == 0) {
        self->unk164 = 0;
        func_8004B79C(out, reinterpret_cast<const ml::CVec3*>(self->unk10));
        return;
    }
    if (func_800BBC0C(self->unk164) == 0) {
        self->unk164 = 0;
        func_8004B79C(out, reinterpret_cast<const ml::CVec3*>(self->unk10));
        return;
    }
    ml::CVec3 local_2c;
    ml::CVec3 local_20;
    ml::CVec3 local_14;
    ml::CVec3 local_8;
    func_8004B79C(&local_2c,
                  reinterpret_cast<const ml::CVec3*>(reinterpret_cast<cf::CfCamPosSource*>(self->unk164)->fn0xAC()));
    void* f = func_8006C670(self->unk164);
    if (f != 0 && func_8006C6B4(reinterpret_cast<int>(self), 0x2000) != 0) {
        func_8004B3F0(&local_2c, self->unk168 + 0x54);
    }
    func_8004B738(reinterpret_cast<float*>(&local_2c), lbl_eu_80570A2C);
    int flag = 0;
    func_8004B0B0(&local_20);
    func_8004B0B0(&local_14);
    func_8004B79C(&local_8, &local_2c);
    local_8.z += lbl_eu_806662F8;
    if (func_804BE348(reinterpret_cast<cf::CfCamPosSource*>(self->unk164)->fn0xAC(), &local_8, 0, 4,
                      0) != 0) {
        func_804BE4B4(&local_20, 0);
        f32 h = lbl_eu_8066631C + local_20.y;
        if (local_14.y < h) {
            local_14.y = h;
            if (func_8007FE24__Q22cf13CfGameManagerFv(8) != 0) {
                func_800C1DF0(0x140, 0x70, lbl_eu_804FB4F0 + 0x15);
            }
            func_8006C640(self, 0x800, 1);
            flag = 1;
        }
    } else if (f != 0) {
        if (func_8006CA20(f) != 0 || func_8006C6B4(reinterpret_cast<int>(self), 4) != 0) {
            func_8004B3F0(&local_8, reinterpret_cast<cf::CfCamPosSource*>(self->unk164)->fn0xAC());
            local_8.z += lbl_eu_80666320;
            if (func_804BE348(reinterpret_cast<cf::CfCamPosSource*>(self->unk164)->fn0xAC(), &local_8,
                              0, 4, 0) != 0) {
                func_804BE4B4(&local_20, 0);
                f32 h = lbl_eu_8066631C + local_20.y;
                if (local_14.y <= h) {
                    local_14.y = h;
                    if (func_8007FE24__Q22cf13CfGameManagerFv(8) != 0) {
                        func_800C1DF0(0x140, 0x70, lbl_eu_804FB4F0 + 0x21);
                    }
                    flag = 1;
                }
            }
        }
    }
    func_8006C640(self, 2, flag);
    func_8004B79C(out, &local_2c);
}
// func_8006CB0C: follow-cam vector prep. Reads the follow target handle at
// +0x164; when present, its vtable slot 0xCC supplies a float (else 0). arg4
// selects scaleByGlobal(lbl_eu_80661B4C) vs the shared camera-relative vec3's
// x; func_8004B60C packs (s, f, 0) into a temp vec3 passed to func_80073C7C
// along with the first three args.
void func_8006CB0C(void* arg1, cf::CfCamFollow* self, void* arg3, void* arg4) {
    f32 f;
    if (self->unk164 != 0) {
        f = reinterpret_cast<cf::CfCamPosSource*>(self->unk164)->fn0xCC();
    } else {
        f = lbl_eu_806662DC;
    }
    f32 s;
    if (arg4 != 0) {
        s = scaleByGlobal(lbl_eu_80661B4C);
    } else {
        s = lbl_eu_80570A38[0];
    }
    f32 local[3];
    func_8004B60C(local, s, f, lbl_eu_806662DC);
    func_80073C7C(arg1, self, arg3, local, lbl_eu_80661B60);
}
void func_8006CC0C() {
    lbl_eu_80663DE8 = lbl_eu_80661B60;
    lbl_eu_80663DE4 = lbl_eu_80661B60;
    f32 s = scaleByGlobal(lbl_eu_80661B4C);
    lbl_eu_80570A44[0] = s;
    lbl_eu_80570A38[0] = s;
}
void func_8006CC68(){}
void func_8006CE24(void* self, void* arg, int a, int b, int c, int d, int e){}
// func_8006D380: spherical azimuth - atan2(y, sqrt(x^2+z^2)) scaled by the
// same fixed FIdx factor as func_8006D3D0. MWCC computes the fmadds addend
// (z*z) first, so the sum is written x*x + z*z to get the retail load order
// (z at +8 before x at +0).
float func_8006D380(const ml::CVec3* v) {
    return nw4r::math::Atan2FIdx(v->y, (float)sqrt((double)(v->x * v->x + v->z * v->z))) *
           lbl_eu_8066632C;
}
// func_8006D3D0: yaw from the x/z components of a vector, scaled by a fixed
// lookup-index factor (nw4r Atan2FIdx returns FIdx units). z is read first so
// MWCC colors it to f2 and loads it before f1 (retail load order). Defined
// under the C++-mangled name so callers (extern "C" decl in CfCam.hpp, no
// in-TU body) keep a real `bl func_8006D3D0` instead of an inline.
float func_8006D3D0(const ml::CVec3* v) {
    float z = v->z;
    float x = v->x;
    return nw4r::math::Atan2FIdx(x, z) * lbl_eu_8066632C;
}
// func_8006D41C: acos wrapper; retail converts the double result back to f32.
extern "C" float func_8006D41C(float x) {
    return (float)acos(x);
}
// func_8006D450: follow-camera state recompute from a new eye/target pair.
// Builds two direction vectors (target-eye through the normalized mid vector
// and the follow-cam source), stores the yaw/pitch/magnitude fields, then
// derives the angular speed (unk204) and resets the 0x800 flag.
void func_8006D450(cf::CfCamFollow* self, void* argA, void* argB, void* argC,
                   int modeA, unsigned int modeB, float dist) {
    if (func_8006D374(self) == 0) {
        return;
    }
    ml::CVec3 l44;
    ml::CVec3 l38;
    ml::CVec3 l14;
    ml::CVec3 l8;
    ml::CVec3 l2c;
    ml::CVec3 l20;
    func_8004CB80(&l44.x, argA, argB);
    lbl_eu_80570A38[0] = *reinterpret_cast<const f32*>(self->unk168 + 0x18);
    func_8006C740(&l38.x, self);
    const f32* mid;
    if (argC != 0) {
        func_8006CB0C(&l14.x, self, &l38.x, 0);
        mid = &l14.x;
    } else {
        func_800733B8(&l8, self, &l38.x,
                      reinterpret_cast<const f32*>(self->unk168 + 0x18),
                      self->unk244);
        mid = &l8.x;
    }
    func_8004B79C(&l2c, reinterpret_cast<const ml::CVec3*>(mid));
    func_8004CB80(&l20.x, &l2c.x, &l38.x);
    func_8004B3F0(self->unk168 + 0x30, argB);
    if (modeA == 0) {
        func_8004B3F0(&l44, &l20);
        func_8004B3F0(self->unk168 + 0x30, &l38);
    }
    f32 yaw1 = func_8006D380(&l44);
    f32 pitch1 = func_8006D3D0(static_cast<const void*>(&l44));
    f32 yaw2 = func_8006D380(&l20);
    f32 pitch2 = func_8006D3D0(static_cast<const void*>(&l20));
    *reinterpret_cast<f32*>(self->unk168 + 0x00) = yaw1;
    *reinterpret_cast<f32*>(self->unk168 + 0x04) = pitch1;
    *reinterpret_cast<f32*>(self->unk168 + 0x08) = lbl_eu_806662DC;
    *reinterpret_cast<f32*>(self->unk168 + 0x0C) = yaw2;
    *reinterpret_cast<f32*>(self->unk168 + 0x10) = pitch2;
    *reinterpret_cast<f32*>(self->unk168 + 0x14) = lbl_eu_806662DC;
    self->unk208 = func_8006BB00(static_cast<const void*>(&l44));
    self->unk20C = func_8006BB00(static_cast<const void*>(&l20));
    self->unk210 = dist;
    func_800A3C48(&l44);
    func_800A3C48(&l20);
    f32 dot = func_800504BC(&l44.x, &l20.x);
    f32 t = func_8006D41C((lbl_eu_806662D0 + dot) * lbl_eu_80666304);
    t = func_8006D410(t);
    if (t != t) {
        t = lbl_eu_806662DC; // NaN guard
    }
    f32 blend =
        lbl_eu_80661B8C + t * (lbl_eu_80661B90 - lbl_eu_80661B8C) / lbl_eu_80666324;
    if (blend < lbl_eu_806662D0) {
        blend = lbl_eu_80661B8C;
    }
    f32 denom = blend;
    if (modeB != 0) {
        denom = static_cast<float>(modeB); // unsigned int -> float conversion
    }
    self->unk204 = lbl_eu_806662D0 / denom;
    if (modeA == 0) {
        self->unk204 = lbl_eu_806662D0;
    } else if (t < lbl_eu_8066A208) {
        self->unk204 = lbl_eu_80666304;
    }
    self->unk200 = lbl_eu_806662DC;
    self->unk238 = lbl_eu_806662DC;
    func_8006BBF4(self, 0x800, 1);
}
// func_8006D6A8: transform src by mtx into a stack temp, then copy the result
// out component-wise (lfs/stfs triple, f0 reused).
void func_8006D6A8(Vec* dst, const Mtx mtx, const Vec* src) {
    Vec tmp;
    PSMTXMultVec(mtx, src, &tmp);
    dst->x = tmp.x;
    dst->y = tmp.y;
    dst->z = tmp.z;
}
// func_8006D700: remote-pad availability query; forwards the controller id to
// the pad-state check and returns whether it reported "not present" (== 0).
extern "C" __declspec(noinline) bool func_8006D700(int controllerId) {
    return func_8006BFC4(controllerId, 3) == 0;
}
// func_8006D7A8: init the follow-cam defaults (func_8006D734), then fan the
// source vec3 out to the +0x40 / +0x4C / +0x58 slots. func_8006D734 itself is
// declared extern (no in-TU body) so MWCC keeps the bl instead of inlining.
extern "C" void func_8006D7A8(cf::CfCamFollow* self, ml::CVec3* src) {
    func_8006D734(self, src);
    func_8004B3F0(self->unk1C + 0x30, src);
    func_8004B3F0(self->unk1C + 0x24, src);
    func_8004B3F0(self->unk1C + 0x3C, src);
}
// func_8006D734: reset the follow-cam slots - zero the +0x250 halfword, copy
// src into all 16 stride-12 vec3 slots at +0x70 (element base via
// func_8006BF14, which is only declared here so the loop keeps its bl), and
// set the +0x22C/+0x230 distances to 1.0.
void func_8006D734(cf::CfCamFollow* self, u8* src) {
    self->unk250 = 0;
    for (unsigned int i = 0; i < 16; i++) {
        func_8004B3F0(func_8006BF14(self->unk70, i), src);
    }
    self->unk22C = lbl_eu_806662D0;
    self->unk230 = lbl_eu_806662D0;
}
// func_8006D804: follow-cam flag sync from the Class_80296898 selection
// bytes. Bytes 0/1 toggle the 0x8/0x4 flags; byte 6 clears the 0x40000 flag
// and zeroes the +0x21C/+0x220 distances. When the 0x80663DE0 gate is clear,
// refresh the shared tables (func_8006B8E4) and - if selection byte 5 differs
// from the 0x80661BA4 state - run the state routine with flag 1.
void func_8006D804(cf::CfCamFollow* self) {
    const u8* inst = reinterpret_cast<const u8*>(Class_80296898::getInstance());
    func_8006BBF4(self, 8, inst[0] != 0);
    func_8006BBF4(self, 4, inst[1] != 0);
    func_8006BBF4(self, 0x40000, inst[6] == 0);
    if (inst[6] == 0) {
        self->unk21C = lbl_eu_806662DC;
        self->unk220 = lbl_eu_806662DC;
    }
    if (lbl_eu_80663DE0 == 0) {
        func_8006B8E4();
        func_8006B948();
        if ((unsigned int)inst[5] != (unsigned int)lbl_eu_80661BA4) {
            func_8006B720(inst[5], 1);
        }
    }
}
// func_8006D8D0: follow-camera flag sync from the target object at `arg`.
// func_800BBC0C(arg) yields the object; when func_8006C670 reports a child,
// a battery of func_8006DC*/probe results is fanned into the 0x8 / 0x1 / 0x2
// / 0x4 / 0x80 / 0x40 / 0x200 / 0x20 / 0x8000 / 0x10000 flag bits. The
// +0x18C vec3 magnitude below lbl_eu_8066629C decays +0x238 (toward 0) or
// resets it to lbl_eu_80666334; the +0x80000 flag tracks +0x238 > 0. The
// func_8006DC7C/DC84/DC8C probes decide the 0x40000 flag and the
// func_8006DC94 probe feeds the 0x100000 bit.
void func_8006D8D0(cf::CfCamFollow* self, void* arg) {
    // CfGameManager.hpp declares func_80069EA0 as void, but the retail call
    // consumes its f1 return (CfTFile.cpp defines it as `float`) - call it
    // through a float-returning cast at the use site.
    void* obj = func_800BBC0C(arg);
    int flag;
    void* a = func_8016FE34(obj);
    if (a != 0) {
        flag = func_8006DBD4(reinterpret_cast<cf::CfCamFollow*>(a), 4);
    } else {
        flag = 0;
    }
    func_8006C640(self, 0x10, flag);
    if (obj != 0 && func_8006C670(obj) != 0) {
        void* x = func_8006C670(obj);
        if (func_8006DC34(x) != 0) {
            if (func_8006C6B4(reinterpret_cast<int>(self), 1) == 0) {
                self->unk224 = lbl_eu_80666330;
            }
        }
        func_8006C6B4(reinterpret_cast<int>(self), 0x10000);
        func_8006C640(self, 8, func_8006DC40(x));
        func_8006C640(self, 1, func_8006DC34(x));
        func_8006C640(self, 2, func_8006DC4C(x, 0x40000));
        func_8006C640(self, 4, func_8006DC4C(x, 0x20000));
        func_8006C640(self, 0x80, func_8004B848(x));
        func_8006C640(self, 0x40, func_8006CA20(x));
        func_8006C640(self, 0x200, func_8006DC64(x));
        func_8006C640(self, 0x20, func_8006DC70(x));
        func_8006C640(self, 0x8000, func_8006DC4C(x, 0x400000));
        func_8006C640(self, 0x10000, func_8006DC4C(x, 0x40000000));
        int magFlag = func_8006BB00(&self->unk168[0x24]) < lbl_eu_8066629C;
        func_8006C640(self, 0x100, magFlag);
        if (magFlag != 0) {
            self->unk238 -= ((float (*)())func_80069EA0)();
            if (self->unk238 <= lbl_eu_806662DC) {
                self->unk238 = lbl_eu_806662DC;
            }
        } else {
            self->unk238 = lbl_eu_80666334;
        }
        int posFlag = self->unk238 > lbl_eu_806662DC;
        func_8006C640(self, 0x80000, posFlag);
        int f2 = 0;
        f32 c = func_8006DC7C(x);
        if (func_8006BAF0(&c) > lbl_eu_8066A208) {
            f32 b = func_8006DC84(x);
            if (func_8006BAF0(&b) > lbl_eu_8066A208) {
                f2 = 1;
            }
        }
        int f3 = 0;
        if (f2 != 0) {
            f32 d8c = func_8006DC8C(x);
            if (func_8006DC84(x) - d8c >= lbl_eu_80666338) {
                f3 = 1;
            }
        }
        func_8006C640(self, 0x40000, f3);
        func_8006C640(self, 0x100000, func_8006DC94(x));
    }
}
// func_8006DBD4: query the +0x4 sub-object's vtable slot-0x30 state word and
// feed it to the func_80174C98 arts-state gate. noinline keeps the retail
// `bl func_8006DBD4` at call sites.
extern "C" __declspec(noinline) int func_8006DBD4(cf::CfCamFollow* self, int flags) {
    u32 val = reinterpret_cast<cf::CfSubObjView*>(self->unk04)->fnAt30()->field_0;
    return func_80174C98(self, &val, flags);
}
// func_8006DCA0: follow-target arts-state gate. Resolves the actor state via
// func_8016FE34, requires the CfGameManager gate, and returns true only when
// NONE of the 3/4/1 state flags and ALL... i.e. the 8 flag probe reports clear.
int func_8006DCA0(void* obj) {
    cf::CfCamFollow* a = static_cast<cf::CfCamFollow*>(func_8016FE34(obj));
    if (a == 0) {
        return 0;
    }
    if (func_8007F91C__Q22cf13CfGameManagerFv() == 0) {
        return 0;
    }
    if (func_8006DBD4(a, 3) != 0) {
        return 0;
    }
    if (func_8006DBD4(a, 4) != 0) {
        return 0;
    }
    if (func_8006DBD4(a, 1) != 0) {
        return 0;
    }
    return func_8006DBD4(a, 8) == 0;
}
// func_8006DD58: follow-cam per-frame update driver. Resolves the follow
// target handle (+0x164) into its manager block (base = target - 0x3E9C) when
// the gate probes pass, then runs the actor-param heading selection and
// refreshes the +0x1C8 camera vector toward the selected angle. Finally
// applies the 0x40-flag magnitude decay/clear and clears the 0x1000 flag.
void func_8006DD58(cf::CfCamFollow* self) {
    u8* base = 0;
    void* target = self->unk164;
    if (target != 0) {
        if (func_8006C1B0(target) || func_8006DF9C(target)) {
            base = static_cast<u8*>(target) - 0x3E9C;
        }
    }
    func_8006C640(self, 0x2000, 0);
    const u8* inst = reinterpret_cast<const u8*>(Class_80296898::getInstance());
    if (inst[7] != 0) {
        // game-manager probe via vtable slot 0x60; result unused in retail
        reinterpret_cast<cf::CfCamGmView*>(self)->fnAt60();
        if (func_8006DCA0(self) != 0 && base != 0 &&
            testResInfoFlag(0x80000) == 0 &&
            func_8006BFC4(reinterpret_cast<int>(self), 2) == 0 &&
            func_800FE68C() != 0) {
            func_800FE68C();
            int sel = func_800B708C__Fi(func_8006DFA8());
            // zero-constructed temp vec3 (ctor + CVec3::zero copy)
            ml::CVec3 tmp2c;
            func_8004B0B0(&tmp2c);
            func_8006BEC4(&tmp2c);
            void* actor = func_8016FE34(reinterpret_cast<void*>(sel));
            if (actor != 0) {
                int flag = 1;
                f32 ang;
                switch (CActorParam_UnkVirtualFunc22__Q22cf11CActorParamFv(actor)) {
                case 3:
                    ang = lbl_eu_806662E0;
                    flag = 0;
                    break;
                case 4:
                case 5:
                    ang = lbl_eu_806662D8;
                    break;
                case 6:
                    ang = lbl_eu_806662D0;
                    break;
                default:
                    flag = 0;
                    break;
                }
                if (func_8006DFBC(static_cast<u8*>(actor) + 0x3E9C)) {
                    ang += lbl_eu_80666304;
                }
                func_8006C640(self, 0x2000, flag);
            }
            f32 sp8[3];
            f32 sp14[3];
            f32 sp20[3];
            func_8004CB80(sp8, &tmp2c.x,
                          reinterpret_cast<const f32*>(self->unk168 + 0x60));
            func_8004B75C(sp14, sp8, lbl_eu_806662F8);
            func_8006C6E8(reinterpret_cast<nw4r::math::VEC3*>(sp20),
                          reinterpret_cast<const nw4r::math::VEC3*>(
                              self->unk168 + 0x60),
                          reinterpret_cast<const nw4r::math::VEC3*>(sp14));
            func_8004B3F0(self->unk168 + 0x60, sp20);
        }
    }
    if (func_8006BFC4(reinterpret_cast<int>(self), 0x40) == 0) {
        func_8006BB04(reinterpret_cast<nw4r::math::VEC3*>(self->unk168 + 0x60),
                      lbl_eu_80666318);
        f32 lenSq = func_8006DFC8(
            reinterpret_cast<const nw4r::math::VEC3*>(self->unk168 + 0x60));
        if (lenSq < lbl_eu_8066633C) {
            func_8006BEC4(self->unk168 + 0x60);
        }
        func_8006C640(self, 0x1000, 0);
    }
}
// func_8006DFE0: follow-camera heading/distance update. Two direction vectors
// are derived (arg2 - (self+0x28), arg2 - (self+0x10)); out1 gets the
// sin-dist product (func_8004CC40 of 8.0 * SinFIdx(lbl_eu_80663DC8) with the
// distance float); out2 gets pi + yaw(tmp2) - or pi + yaw(tmp2) - yaw(tmp1)
// when flag == 0 - offset by lbl_eu_80663DCC and wrapped by func_8004BC28.
void func_8006DFE0(cf::CfCamFollow* self, const ml::CVec3* arg2, float* out1,
                   float* out2, int flag, float dist) {
    ml::CVec3 tmp1;
    ml::CVec3 tmp2;
    func_8004CB80(&tmp1.x, &arg2->x, reinterpret_cast<const f32*>(&self->unk1C[12]));
    func_8004CB80(&tmp2.x, &arg2->x, reinterpret_cast<const f32*>(self->unk10));
    *out1 = func_8004CC40(lbl_eu_806662B8 * func_8004CC68(lbl_eu_80663DC8), dist);
    if (flag != 0) {
        *out2 = lbl_eu_8066A1F8 + func_8006D3D0(static_cast<const void*>(&tmp2));
    } else {
        f32 yaw1 = func_8006D3D0(static_cast<const void*>(&tmp1));
        *out2 = lbl_eu_8066A1F8 + (func_8006D3D0(static_cast<const void*>(&tmp2)) - yaw1);
    }
    *out2 += lbl_eu_80663DCC;
    *out2 = func_8004BC28(*out2);
}
// func_8006E0C8: follow-camera heading update. Two direction vectors are
// derived (arg2 minus the +0x28 / +0x10 positions); out1 gets the +0x1F8
// distance; out2 gets pi plus the yaw of v1 (or pi + yaw(v1) - yaw(v2) when
// flag == 0), wrapped into [-pi, pi) by func_8004BC28.
void func_8006E0C8(cf::CfCamFollow* self, const ml::CVec3* arg2, float* out1,
                   float* out2, int flag) {
    ml::CVec3 v2;
    ml::CVec3 v1;
    func_8004CB80(&v2.x, &arg2->x, reinterpret_cast<const f32*>(&self->unk1C[12]));
    func_8004CB80(&v1.x, &arg2->x, reinterpret_cast<const f32*>(&self->unk10[0]));
    *out1 = self->unk1F8;
    if (flag != 0) {
        *out2 = lbl_eu_8066A1F8 + func_8006D3D0(static_cast<const void*>(&v1));
    } else {
        *out2 = lbl_eu_8066A1F8 + (func_8006D3D0(static_cast<const void*>(&v1)) -
                                   func_8006D3D0(static_cast<const void*>(&v2)));
    }
    *out2 = func_8004BC28(*out2);
}
// func_8006E18C: follow-flag maintenance driven by the config singleton run
// byte (+7). While running: claims the 0x4000 flag when absent (clearing stale
// 0x800000 / 0x8000 bits and snapshotting +0x180/+0x23C into +0x1F8/+0x1FC on
// first entry), and syncs the 0x400000 bit against the 0x20000000 follow flag,
// storing the float arg into +0x1F0 while engaged. Returns whether the
// manager is running.
int func_8006E18C(cf::CfCamFollow* self, float val) {
    const unsigned char* inst =
        reinterpret_cast<const unsigned char*>(Class_80296898::getInstance());
    if (inst[7] != 0) {
        if (func_8006BFC4(reinterpret_cast<int>(self), 0x4000) == 0) {
            func_8006BBF4(self, 0x4000, 1);
            if (func_8006BFC4(reinterpret_cast<int>(self), 0x800000) != 0) {
                func_8006BC1C(self, 0x800000);
            }
            // result deliberately discarded in retail
            func_8006C6B4(reinterpret_cast<int>(self), 0x200000);
            if (func_8006BFC4(reinterpret_cast<int>(self), 0x8000) != 0) {
                func_8006BC1C(self, 0x8000);
            }
            if (self->unk256 == 0) {
                self->unk1F8 = *reinterpret_cast<const f32*>(
                    reinterpret_cast<const u8*>(self) + 0x180);
                self->unk1FC = self->unk23C;
            }
            self->unk256 = 8;
        }
        if (func_8006BFC4(reinterpret_cast<int>(self), 0x400000) != 0) {
            if (func_8006C6B4(reinterpret_cast<int>(self), 0x20000000) == 0) {
                func_8006BC1C(self, 0x400000);
            }
        } else {
            if (func_8006C6B4(reinterpret_cast<int>(self), 0x20000000) != 0) {
                func_8006BBF4(self, 0x400000, 1);
                self->unk1F0 = val;
                self->unk256 = 8;
            }
        }
    }
    return inst[7] != 0 ? 1 : 0;
}
// func_8006E2FC: copy a saved camera-state block (or id's object) into the
// active camera-state buffer. Gates on the manager and the active object,
// then fans the vec3 slots, the 9-word +0x114 block, the sub-object copies
// and the scalar tail across, finally setting the source's 0x20 flag.
void func_8006E2FC(int id) {
    u8* dst = id != 0 ? reinterpret_cast<u8*>(id) : lbl_eu_80570A8C;
    CfCamEventManager* mgr = func_8006E59C();
    if (mgr == 0) {
        return;
    }
    u8* src = reinterpret_cast<u8*>(func_8006E5A4(mgr));
    if (src == 0) {
        return;
    }
    func_8004B3F0(dst + 0x00, src + 0x10);
    func_8004B3F0(dst + 0x0C, src + 0x1C);
    func_8004B3F0(dst + 0x18, src + 0x28);
    func_8004B3F0(dst + 0x24, src + 0x34);
    func_8004B3F0(dst + 0x30, src + 0x40);
    func_8004B3F0(dst + 0x3C, src + 0x4C);
    func_8004B3F0(dst + 0x48, src + 0x58);
    for (int i = 0; i < func_8006E5B8(src + 0x70); i++) {
        func_8004B3F0(func_8006BF14(dst + 0x54, i), func_8006BF14(src + 0x70, i));
    }
    func_8004B3F0(dst + 0x114, src + 0x168);
    func_8004B3F0(dst + 0x120, src + 0x174);
    func_8004B3F0(dst + 0x12C, src + 0x180);
    func_8004B3F0(dst + 0x138, src + 0x18C);
    func_8004B3F0(dst + 0x144, src + 0x198);
    func_8004B3F0(dst + 0x150, src + 0x1A4);
    func_8004B3F0(dst + 0x15C, src + 0x1B0);
    func_8004B3F0(dst + 0x168, src + 0x1BC);
    func_8004B3F0(dst + 0x174, src + 0x1C8);
    func_8006E5AC(dst + 0x180, src + 0x04);
    func_8006E5C0(dst + 0x184, src + 0x1D4);
    func_8006E5CC(dst + 0x188, src + 0x1D8);
    *reinterpret_cast<f32*>(dst + 0x18C) = *(const f32*)(src + 0x1DC);
    func_8006C640(reinterpret_cast<cf::CfCamFollow*>(src), 0x20, 1);
    *reinterpret_cast<f32*>(dst + 0x190) = *(const f32*)(src + 0x1E0);
    *reinterpret_cast<f32*>(dst + 0x194) = *(const f32*)(src + 0x1E4);
    *reinterpret_cast<f32*>(dst + 0x198) = *(const f32*)(src + 0x1E8);
    *reinterpret_cast<f32*>(dst + 0x19C) = *(const f32*)(src + 0x1EC);
    *reinterpret_cast<f32*>(dst + 0x1A0) = *(const f32*)(src + 0x1F0);
    *reinterpret_cast<f32*>(dst + 0x1A4) = *(const f32*)(src + 0x1F4);
    *reinterpret_cast<f32*>(dst + 0x1A8) = *(const f32*)(src + 0x1F8);
    *reinterpret_cast<f32*>(dst + 0x1AC) = *(const f32*)(src + 0x1FC);
    *reinterpret_cast<f32*>(dst + 0x1B0) = *(const f32*)(src + 0x200);
    *reinterpret_cast<f32*>(dst + 0x1B4) = *(const f32*)(src + 0x204);
    *reinterpret_cast<f32*>(dst + 0x1B8) = *(const f32*)(src + 0x208);
    *reinterpret_cast<f32*>(dst + 0x1BC) = *(const f32*)(src + 0x20C);
    *reinterpret_cast<f32*>(dst + 0x1C0) = *(const f32*)(src + 0x210);
    *reinterpret_cast<u32*>(dst + 0x1C4) = *(const u32*)(src + 0x214);
    *reinterpret_cast<f32*>(dst + 0x1C8) = *(const f32*)(src + 0x218);
    *reinterpret_cast<f32*>(dst + 0x1CC) = *(const f32*)(src + 0x21C);
    *reinterpret_cast<f32*>(dst + 0x1D0) = *(const f32*)(src + 0x220);
    *reinterpret_cast<f32*>(dst + 0x1D4) = *(const f32*)(src + 0x224);
    *reinterpret_cast<f32*>(dst + 0x1D8) = *(const f32*)(src + 0x228);
    *reinterpret_cast<f32*>(dst + 0x1DC) = *(const f32*)(src + 0x22C);
    *reinterpret_cast<f32*>(dst + 0x1E0) = *(const f32*)(src + 0x230);
    *reinterpret_cast<f32*>(dst + 0x1E4) = *(const f32*)(src + 0x234);
    *reinterpret_cast<f32*>(dst + 0x1E8) = *(const f32*)(src + 0x238);
    *reinterpret_cast<f32*>(dst + 0x1EC) = *(const f32*)(src + 0x23C);
    *reinterpret_cast<f32*>(dst + 0x1F0) = *(const f32*)(src + 0x240);
    *reinterpret_cast<f32*>(dst + 0x1F4) = *(const f32*)(src + 0x244);
    *reinterpret_cast<f32*>(dst + 0x1F8) = *(const f32*)(src + 0x248);
    *reinterpret_cast<s16*>(dst + 0x1FC) = *(const s16*)(src + 0x250);
    *reinterpret_cast<u16*>(dst + 0x1FE) = *(const u16*)(src + 0x254);
    *reinterpret_cast<s16*>(dst + 0x200) = *(const s16*)(src + 0x256);
    *reinterpret_cast<s16*>(dst + 0x202) = *(const s16*)(src + 0x258);
}
// func_8006E5D8: copy the follow-camera state from `self` (or the shared
// fallback buffer lbl_eu_80570A8C when self == 0) into the active camera
// object. Gates: the camera manager must exist (func_8006E59C), the active
// object must come back non-null from func_8006E5A4 and carry the 0x20 flag.
// Copies the 16 stride-12 vec3 slots at +0x70, the +0x114..0x178 block, then
// the float/word/halfword tail through func_8006E5AC/C0/CC.
void func_8006E5D8(cf::CfCamFollow* self) {
    u8* src = reinterpret_cast<u8*>(self);
    if (self == 0) {
        src = lbl_eu_80570A8C;
    }
    CfCamEventManager* mgr = func_8006E59C();
    if (mgr == 0) {
        return;
    }
    cf::CfCamFollow* dst = reinterpret_cast<cf::CfCamFollow*>(func_8006E5A4(mgr));
    if (dst == 0) {
        return;
    }
    if (func_8006C6B4(reinterpret_cast<int>(dst), 0x20) == 0) {
        return;
    }
    func_8004B3F0(dst->unk10, src + 0x00);
    func_8004B3F0(dst->unk1C, src + 0x0C);
    func_8004B3F0(dst->unk1C + 12, src + 0x18);
    func_8004B3F0(dst->unk1C + 24, src + 0x24);
    func_8004B3F0(dst->unk1C + 36, src + 0x30);
    func_8004B3F0(dst->unk1C + 48, src + 0x3C);
    func_8004B3F0(dst->unk1C + 60, src + 0x48);
    for (int i = 0; i < func_8006E5B8(src + 0x54); i++) {
        func_8004B3F0(func_8006BF14(dst->unk70, i), func_8006BF14(src + 0x54, i));
    }
    func_8004B3F0(dst->unk168, src + 0x114);
    func_8004B3F0(dst->unk168 + 12, src + 0x120);
    func_8004B3F0(dst->unk168 + 24, src + 0x12C);
    func_8004B3F0(dst->unk168 + 36, src + 0x138);
    func_8004B3F0(dst->unk168 + 48, src + 0x144);
    func_8004B3F0(dst->unk168 + 60, src + 0x150);
    func_8004B3F0(dst->unk168 + 72, src + 0x15C);
    func_8004B3F0(dst->unk168 + 84, src + 0x168);
    func_8004B3F0(dst->unk168 + 96, src + 0x174);
    func_8006E5AC(&dst->unk04, src + 0x180);
    func_8006E5C0(&dst->unk1D4, src + 0x184);
    func_8006E5CC(&dst->unk1D8, src + 0x188);
    dst->unk1DC = *(const f32*)(src + 0x18C);
    dst->unk1E0 = *(const f32*)(src + 0x190);
    dst->unk1E4 = *(const f32*)(src + 0x194);
    dst->unk1E8 = *(const f32*)(src + 0x198);
    dst->unk1EC = *(const f32*)(src + 0x19C);
    dst->unk1F0 = *(const f32*)(src + 0x1A0);
    dst->unk1F4 = *(const f32*)(src + 0x1A4);
    dst->unk1F8 = *(const f32*)(src + 0x1A8);
    dst->unk1FC = *(const f32*)(src + 0x1AC);
    dst->unk200 = *(const f32*)(src + 0x1B0);
    dst->unk204 = *(const f32*)(src + 0x1B4);
    dst->unk208 = *(const f32*)(src + 0x1B8);
    dst->unk20C = *(const f32*)(src + 0x1BC);
    dst->unk210 = *(const f32*)(src + 0x1C0);
    dst->unk214 = *(const u32*)(src + 0x1C4);
    dst->unk218 = *(const f32*)(src + 0x1C8);
    dst->unk21C = *(const f32*)(src + 0x1CC);
    dst->unk220 = *(const f32*)(src + 0x1D0);
    dst->unk224 = *(const f32*)(src + 0x1D4);
    dst->unk228 = *(const f32*)(src + 0x1D8);
    dst->unk22C = *(const f32*)(src + 0x1DC);
    dst->unk230 = *(const f32*)(src + 0x1E0);
    dst->unk234 = *(const f32*)(src + 0x1E4);
    dst->unk238 = *(const f32*)(src + 0x1E8);
    dst->unk23C = *(const f32*)(src + 0x1EC);
    dst->unk240 = *(const f32*)(src + 0x1F0);
    dst->unk244 = *(const f32*)(src + 0x1F4);
    dst->unk248 = *(const f32*)(src + 0x1F8);
    dst->unk250 = *(const s16*)(src + 0x1FC);
    dst->unk254 = *(const u16*)(src + 0x1FE);
    dst->unk256 = *(const s16*)(src + 0x200);
    dst->unk258 = *(const s16*)(src + 0x202);
    func_8006C730();
}
void func_8006E884(){}
void func_8006EFA0(){}
// func_8006F5C8: follow-camera per-frame target/heading selection. Zeroes the
// two output floats, then (unless both the 0x20000 flag probe and the
// func_80075640 gate reject) runs the +0x70 vtable sub-object path: clears
// the 0x10000 flag, reads the remote-pad availability (its result becomes the
// return value), and - when the func_8006D72C gate is clear - picks out1 from
// the 0x1/0x4 flag + camera-register matrix (scaled by |sub+0x68| on a classic
// pad) and out2 from the 0x2/0x8 flag + move-register matrix (scaled by
// |sub+0x6C|). The flag-word OR of CfRes_getE24Bit22/func_8006F9D4/
// func_8006EEE4 drives a delayed reset path (0x254 countdown, func_8008585C /
// func_80070EBC) and finally zeroes both outputs when set.
int func_8006F5C8(cf::CfCamFollow* self, void* arg, float* out1, float* out2) {
    *out1 = lbl_eu_806662DC;
    *out2 = lbl_eu_806662DC;
    if (func_8006BFC4(reinterpret_cast<int>(self), 0x20000) == 0 &&
        func_80075640() == 0) {
        return 0;
    }
    void* sub = reinterpret_cast<cf::CfCamPosSource*>(self)->v26();
    func_8006BC1C(self, 0x10000);
    int result = 0;
    if (*(reinterpret_cast<const u8*>(sub) + 0xED) != 0) {
        result = func_8006D700(reinterpret_cast<int>(self));
        if (result != 0) {
            func_8006BBF4(self, 0x10000, 1);
        } else {
            func_8006BC1C(self, 0x10000);
        }
        if (func_8006D72C(self) == 0) {
            if (func_8006BFC4(reinterpret_cast<int>(self), 1) == 0) {
                if (func_80275378(self) != 0) {
                    *out1 = func_8006BFC4(reinterpret_cast<int>(self), 4) != 0
                                ? lbl_eu_80666358
                                : lbl_eu_806662D0;
                } else if (func_80275338(self) != 0) {
                    *out1 = func_8006BFC4(reinterpret_cast<int>(self), 4) != 0
                                ? lbl_eu_806662D0
                                : lbl_eu_80666358;
                }
                if (func_80074A74() != 0) {
                    *out1 *= func_8006BAF0(reinterpret_cast<u8*>(sub) + 0x68);
                }
            }
            if (func_8006BFC4(reinterpret_cast<int>(self), 2) == 0) {
                if (func_802752B8(self) != 0) {
                    *out2 = func_8006BFC4(reinterpret_cast<int>(self), 8) != 0
                                ? lbl_eu_806662D0
                                : lbl_eu_80666358;
                } else if (func_802752F8(self) != 0) {
                    *out2 = func_8006BFC4(reinterpret_cast<int>(self), 8) != 0
                                ? lbl_eu_80666358
                                : lbl_eu_806662D0;
                }
                if (func_80074A74() != 0) {
                    *out2 *= func_8006BAF0(reinterpret_cast<u8*>(sub) + 0x6C);
                }
            }
        }
    }
    int flags = CfRes_getE24Bit22() | (int)func_8006F9D4() |
                (int)func_8006EEE4();
    int r31 = 1;
    if (func_801B0F8C() == 0 && func_8017FD44() == 0) {
        r31 = 0;
    }
    if (func_8006BFC4(reinterpret_cast<int>(self), 0x400) == 0 && flags == 0 &&
        r31 == 0) {
        if (func_802751F8(self) != 0 && func_8006EEF8(self) == 0 &&
            func_8006BFC4(reinterpret_cast<int>(self), 0x800) == 0) {
            self->unk254 = 15;
            self->unk248 = lbl_eu_806662DC;
            func_8006BBF4(self, 0x8000, 1);
            reinterpret_cast<cf::CfCamPosSource*>(self)->v22();
            int dca0 = func_8006DCA0(arg);
            if (dca0 != 0) {
                if (func_8006BFC4(reinterpret_cast<int>(self), 0x10) == 0) {
                    func_8006BBF4(self, 0x10, 1);
                }
            }
            func_8006CE24(self, arg, 1, 1, 1, 1, 0);
            if (dca0 != 0 &&
                func_8006BFC4(reinterpret_cast<int>(self), 0x10) != 0) {
                func_8006BC1C(self, 0x10);
            }
        }
    }
    int flags2 = CfRes_getE24Bit22() | (int)func_8006EF04__Fi(0xABA40000) |
                 (int)func_8006EEE4();
    if (func_8006BFC4(reinterpret_cast<int>(self), 0xC10) == 0 && r31 == 0) {
        if (self->unk254 != 0) {
            self->unk254 -= 1;
        } else if (flags2 == 0) {
            if (func_8008585C__Q22cf13CfGameManagerFv() == 0) {
                func_80070EBC(self);
            }
        }
    }
    if (flags2 != 0) {
        *out2 = lbl_eu_806662DC;
        *out1 = lbl_eu_806662DC;
    }
    return result;
}
// func_8006F9EC: follow-cam state snapshot/restore driver. With the 0x40
// flag: claim 0x20, clear 0x40, chain-copy the position vec3 slots from
// srcPos through +0x58/+0x64/+0x4C/+0x40 into arg, then feed the source's
// scale and the shared vec3 into the slot-0x18 virtual. The 0x2000 path
// clears the follow state; the 0x3020 path re-runs the pad-action selection.
void func_8006F9EC(cf::CfCamFollow* self, void* arg, void* srcPos) {
    if (func_8006BFC4(reinterpret_cast<int>(self), 0x40) != 0 &&
        self->unk164 != 0) {
        func_8006BBF4(self, 0x20, 1);
        func_8006BC1C(self, 0x40);
        void* p = func_8004B3F0(self->unk1C + 0x3C, srcPos);
        p = func_8004B3F0(self->unk1C + 0x48, p);
        p = func_8004B3F0(self->unk1C + 0x30, p);
        p = func_8004B3F0(self->unk1C + 0x24, p);
        func_8004B3F0(arg, p);
        f32 scale =
            reinterpret_cast<cf::CfCamPosSource*>(self->unk164)->fn0xCC();
        nw4r::math::VEC3 v;
        func_8004B60C(&v, lbl_eu_80570A38[0], scale, lbl_eu_806662DC);
        reinterpret_cast<cf::CfCamVt18*>(self)->fnAt18(&v, lbl_eu_80661B60);
    }
    if (func_8006BFC4(reinterpret_cast<int>(self), 0x2000) != 0) {
        func_8006BBF4(self, 0x20, 1);
        func_8006BC1C(self, 0x2000);
        self->unk252 = 0;
    }
    if (func_8006BFC4(reinterpret_cast<int>(self), 0x20) != 0 &&
        func_8006BFC4(reinterpret_cast<int>(self), 0x4000) == 0 &&
        (s16)self->unk256 > 0) {
        func_8006BC1C(self, 0x20);
    }
    if (func_8006BFC4(reinterpret_cast<int>(self), 0x3020) != 0) {
        int bit1000 = func_8006BFC4(reinterpret_cast<int>(self), 0x1000);
        int bit2000 = func_8006BFC4(reinterpret_cast<int>(self), 0x2000);
        func_8006BC1C(self, 0x3020);
        self->unk248 = lbl_eu_806662DC;
        if (func_8006EEF8(self) == 0 &&
            func_8006BFC4(reinterpret_cast<int>(self), 0x800) == 0) {
            if (bit1000 != 0) {
                func_8006CE24(self, arg, 1, 1, 1, 1, 0);
            } else if (bit2000 != 0) {
                func_8006CE24(self, arg, 1, 0, 0, 0, 0);
            } else {
                func_8006CE24(self, arg, 0, 0, 0, 1, 0);
            }
        }
    }
}
// func_8006FC44: clamp a heading angle toward the follow-cam's current facing.
// When the 0x400 flag is set and the +0x164 source object exists, the source's
// vtable slot 0xCC facing is wrapped into [-pi, pi) (func_8004BC28); the
// +/-span window around it (span = lbl_eu_8066635C * pi/2 / lbl_eu_80666324)
// is snapped by lbl_eu_8066A1FC steps and finally clamped into [lo, hi].
// Constants are inlined (not named locals) so MWCC picks retail's FPR set.
// Returns whether the clamp changed *angle.
int func_8006FC44(cf::CfCamFollow* self, float* angle) {
    int result = 0;
    if (func_8006C6B4((int)self, 0x400) != 0 && self->unk164 != 0) {
        f32 w = func_8004BC28(lbl_eu_8066A1F8 +
                              reinterpret_cast<cf::CfCamPosSource*>(self->unk164)->fn0xCC());
        f32 lo = w - lbl_eu_8066635C * lbl_eu_8066A200 / lbl_eu_80666324;
        f32 hi = w + lbl_eu_8066635C * lbl_eu_8066A200 / lbl_eu_80666324;
        if (*angle < lo - lbl_eu_8066A200) {
            *angle += lbl_eu_8066A1FC;
        } else if (*angle > hi + lbl_eu_8066A200) {
            *angle -= lbl_eu_8066A1FC;
        }
        if (*angle > hi) {
            *angle = hi;
            result = 1;
        } else if (*angle < lo) {
            *angle = lo;
            result = 1;
        }
    }
    return result;
}
// func_8006FD3C: follow-camera heading update from a stick input value.
// Nonzero input ramps unk1E4 by input*scale (clamped to +/-|input| when the
// classic-pad gate is set); zero input decays unk1E4 toward 0. When either
// decay float is still live, rotates the +0x28/+0x30 camera position around
// the target by the blended heading angle.
void func_8006FD3C(cf::CfCamFollow* self, const nw4r::math::VEC3* arg,
                   float input) {
    f32 lim = lbl_eu_806662D0;
    if (input != lbl_eu_806662DC) {
        f32 scale;
        if (func_8006C6B4(reinterpret_cast<int>(self), 0x100) != 0) {
            scale = lbl_eu_80666360 * lbl_eu_80661B40;
        } else {
            scale = lbl_eu_80661B40;
        }
        self->unk1E4 = input * scale + self->unk1E4;
        if (func_80074A74()) {
            lim = func_8006BAF0(&input);
        }
        // clamp unk1E4 into [-lim, lim]
        if (self->unk1E4 < -lim) {
            self->unk1E4 = -lim;
        } else if (self->unk1E4 > lim) {
            self->unk1E4 = lim;
        }
    } else {
        self->unk1E4 *= lbl_eu_80661B48;
        f32 mag = func_8006BAF0(&self->unk1E4);
        if (mag <= lbl_eu_8066A208) {
            self->unk1E4 = lbl_eu_806662DC;
        }
    }
    if (func_8006BAF0(&self->unk1F4) > lbl_eu_8066A208 ||
        func_8006BAF0(&self->unk1E4) > lbl_eu_8066A208) {
        f32 l10[3];
        func_8004CB80(
            l10, reinterpret_cast<const f32*>(self->unk1C + 0x0C),
            reinterpret_cast<const f32*>(arg));
        f32 f30 = self->unk244;
        f32 camY = *reinterpret_cast<const f32*>(self->unk168 + 0x18);
        f32 yaw = func_8006D3D0(l10);
        f32 angle = yaw;
        if (func_8006BAF0(&self->unk1F4) > lbl_eu_8066A208) {
            f32 step = lbl_eu_8066634C * self->unk1F4;
            step = scaleByGlobal(step);
            // clamp step to +/-lbl_eu_80661B78 with sign of unk1F4
            if (self->unk1F4 >= lbl_eu_806662DC) {
                if (step > lbl_eu_80661B78) {
                    step = lbl_eu_80661B78;
                }
            } else {
                if (step < -lbl_eu_80661B78) {
                    step = -lbl_eu_80661B78;
                }
            }
            angle = yaw + step;
            self->unk1F4 -= step;
        } else {
            angle = self->unk1E4 * lbl_eu_80661B3C + yaw;
            func_8006FC44(self, &angle);
        }
        f32 wrappedCamY = func_8004BC28(camY);
        angle = func_8004BC28(angle);
        f32 mag = f30 * func_8004CC74(wrappedCamY);
        f32 sn = func_8004CC68(angle);
        *reinterpret_cast<f32*>(self->unk1C + 0x0C) = mag * sn + arg->x;
        *reinterpret_cast<f32*>(self->unk1C + 0x14) =
            mag * func_8004CC74(angle) + arg->z;
        *reinterpret_cast<f32*>(self->unk168 + 0x1C) = angle;
    }
}
// func_8006FFA8: follow-distance output selector. Defaults two output floats,
// then overrides outA from the 0x1D4 flag word: masks 0x40 / 0x20000 / 0x40000
// all map to scaleByGlobal(-d); 0x400 maps to scaleByGlobal(lbl_eu_806662D4).
// d is a named local so MWCC keeps lbl_eu_80666330 in a saved FPR (f31)
// across the scaleByGlobal calls, like retail.
void func_8006FFA8(cf::CfCamFollow* self, float* outA, float* outB) {
    *outB = lbl_eu_80663DD4;
    *outA = lbl_eu_80663DD8;
    f32 d = lbl_eu_80666330;
    if (func_8006C6B4((int)self, 0x40) != 0) {
        *outA = scaleByGlobal(-d);
    } else if (func_8006C6B4((int)self, 0x20000) != 0) {
        *outA = scaleByGlobal(-d);
    } else if (func_8006C6B4((int)self, 0x40000) != 0) {
        *outA = scaleByGlobal(-d);
    } else if (func_8006C6B4((int)self, 0x400) != 0) {
        *outA = scaleByGlobal(lbl_eu_806662D4);
    }
}
void func_80070088(){}
// func_80070518: follow-distance ramp. flag != 0 ramps unk1DC up by
// lbl_eu_80666368 toward the lbl_eu_806662F8 cap; flag == 0 decays toward 0
// (snap to 0 when |unk1DC| < epsilon, else step lbl_eu_8066636C toward zero,
// clamped at 0 - >= tests so MWCC emits the cror eq,gt,eq + bne shape).
void func_80070518(cf::CfCamFollow* self, int flag) {
    if (flag != 0) {
        f32 cap;
        f32 inc;
        f32 d;
        f32 sum;
        d = self->unk1DC;
        inc = lbl_eu_80666368;
        cap = lbl_eu_806662F8;
        sum = d + inc;
        self->unk1DC = sum;
        if (sum > cap) {
            self->unk1DC = cap;
        }
    } else if (func_8006BAF0(&self->unk1DC) < lbl_eu_8066A208) {
        self->unk1DC = lbl_eu_806662DC;
    } else {
        if (self->unk1DC >= lbl_eu_806662DC) {
            self->unk1DC -= lbl_eu_8066636C;
            if (self->unk1DC < lbl_eu_806662DC) {
                self->unk1DC = lbl_eu_806662DC;
            }
        } else {
            self->unk1DC += lbl_eu_8066636C;
            if (self->unk1DC >= lbl_eu_806662DC) {
                self->unk1DC = lbl_eu_806662DC;
            }
        }
    }
}
void func_800705D8(){}
// func_80070674: distance-gated aim adjustment. Computes arg3 - arg2; when its
// squared length is inside (c70, c74] a collision probe (func_804BE348, mask
// 0x4a03) runs, and on a hit arg3 is replaced: func_80070784 seeds two temp
// vectors, the normalized difference is scaled by 0.1 and accumulated into v1,
// then arg3 is overwritten with v1. Returns whether the probe hit.
int func_80070674(cf::CfCamFollow* arg1, float* arg2, float* arg3) {
    ml::CVec3 tmp;
    int hit;
    func_8004CB80(&tmp.x, arg3, arg2);
    f32 lenSq = func_8006DFC8(reinterpret_cast<nw4r::math::VEC3*>(&tmp));
    if (lenSq > lbl_eu_80666370 || lenSq < lbl_eu_80666374) {
        return 0;
    }
    hit = func_804BE348(arg3, arg2, 0x4a03, 0, 0) != 0;
    if (hit != 0) {
        ml::CVec3 v1;
        ml::CVec3 v2;
        ml::CVec3 v3;
        func_8004B0B0(&v1);
        func_8004B0B0(&v2);
        func_80070784(&v1, &v2);
        func_8004CBC8(&tmp);
        func_8004B75C(&v3.x, &v2.x, lbl_eu_806662A0);
        func_8004B738(&v1.x, &v3.x);
        func_8004B3F0(arg3, &v1);
    }
    return hit;
}
extern "C" void* cfCam_initAndGetSomething(void* a, void* b) {
    func_804BE4B4(a, 0);
    return func_804BE4E0(b, 0);
}
__declspec(noinline) void func_800707C0(void* self, void* a, void* b){}
// func_80070EBC: decay/accumulate the camera's +0x23C heading value. When the
// remote pad is absent (func_8006BFC4 0x800000 == 0): the camera-control
// register (func_80275238) decays +0x23C by lbl_eu_80661BA8 clamped at
// lbl_eu_80666308; the movement register (func_80275278) then derives a
// normalized direction (self+0x34 minus self+0x4C), probes it against
// self+0x34 (func_804BE348, mask 0x4004a03) and - when clear - accumulates
// +0x23C by lbl_eu_80661BA8 clamped at lbl_eu_80666390. The +0x23C value is
// cached in a local so the clamp compares the register value (retail shape).
void func_80070EBC(cf::CfCamFollow* self) {
    if (func_8006BFC4((int)self, 0x800000) != 0) {
        return;
    }
    if (func_80275238(self) != 0) {
        f32 v = self->unk23C;
        f32 r = v - lbl_eu_80661BA8;
        self->unk23C = r;
        if (r < lbl_eu_80666308) {
            self->unk23C = lbl_eu_80666308;
        }
    }
    if (func_80275278(self) != 0) {
        ml::CVec3 tmp1;
        ml::CVec3 tmp3;
        ml::CVec3 tmp2;
        func_8004CB80(&tmp1.x, reinterpret_cast<const f32*>(&self->unk1C[0x18]),
                      reinterpret_cast<const f32*>(&self->unk1C[0x30]));
        func_800A3C48(&tmp1);
        func_8004B75C(&tmp2.x, &tmp1.x, lbl_eu_806662D0);
        func_8006C6E8(reinterpret_cast<nw4r::math::VEC3*>(&tmp3),
                      reinterpret_cast<nw4r::math::VEC3*>(&self->unk1C[0x18]),
                      reinterpret_cast<nw4r::math::VEC3*>(&tmp2));
        int hit = func_804BE348(&tmp3, &self->unk1C[0x18], 0x44a03, 0, 0) != 0;
        if (hit != 0) {
            return;
        }
        f32 v = self->unk23C;
        f32 r = v + lbl_eu_80661BA8;
        self->unk23C = r;
        if (r > lbl_eu_80666390) {
            self->unk23C = lbl_eu_80666390;
        }
    }
}
// func_80070FB8: follow-camera per-frame update (main body). Ramps +0x200 by
// +0x204 toward 1.0 (clearing the 0x800 flag at the cap), slerps between the
// +0x168/+0x174 rotation quats by +0x200, derives the follow-vector set
// (func_8006C740 -> subtract/scale/add), drives the +0x244 heading through
// func_80073E74 / func_8006FFA8 / func_800733B8, fans the +0x90 vector out to
// the +0x4C/+0x40/+0x58/+0x64 slots, and mirrors the +0x23C/+0x180/+0x184
// values into the shared 0x80663DE4 / 0x80570A38 globals.
void func_80070FB8(cf::CfCamFollow* self) {
    self->unk200 += self->unk204;
    if (self->unk200 >= lbl_eu_806662D0) {
        self->unk200 = lbl_eu_806662D0;
        func_8006BC1C(self, 0x800);
    }
    f32 t = self->unk200;

    ml::CQuat stackD8;
    ml::CQuat stackC8;
    ml::CQuat stackB8;
    ml::CQuat stackA8;
    nopFunc(&stackD8);
    func_8004B0B4(&stackC8);
    func_8004B0B4(&stackB8);
    stackC8.setRotZXY(reinterpret_cast<const ml::CVec3*>(self->unk168));
    func_80071364(reinterpret_cast<Quaternion*>(&stackC8));
    stackB8.setRotZXY(reinterpret_cast<const ml::CVec3*>(self->unk168 + 0xC));
    func_80071364(reinterpret_cast<Quaternion*>(&stackB8));
    func_80071398(reinterpret_cast<Quaternion*>(&stackA8),
                  reinterpret_cast<Quaternion*>(&stackC8),
                  reinterpret_cast<Quaternion*>(&stackB8), t);
    func_80071364(reinterpret_cast<Quaternion*>(&stackA8));
    func_80071694(&stackD8, &stackA8);

    ml::CVec3 stack9C;
    ml::CVec3 stack90;
    ml::CVec3 stack84;
    ml::CVec3 stack78;
    ml::CQuat stack68;
    ml::CVec3 stack5C;
    ml::CVec3 stack50;
    ml::CVec3 stack44;
    ml::CVec3 stack38;
    ml::CVec3 stack2C;
    ml::CVec3 stack20;
    ml::CVec3 stack14;
    f32 outA;
    f32 outB;
    f32 stack8;

    f32 f31 = self->unk200 * (self->unk20C - self->unk208) + self->unk208;
    func_8006C740(&stack9C, self);
    func_8004CB80(&stack44.x, &stack9C.x, reinterpret_cast<const f32*>(self->unk168 + 0x30));
    func_8004B75C(&stack50.x, &stack44.x, self->unk200);
    func_8006C6E8(reinterpret_cast<nw4r::math::VEC3*>(&stack90),
                  reinterpret_cast<nw4r::math::VEC3*>(&stack50),
                  reinterpret_cast<nw4r::math::VEC3*>(self->unk168 + 0x30));
    reinterpret_cast<cf::CfCamPosSource*>(self)->v13(
        self->unk200 * (lbl_eu_80666300 - self->unk210) + self->unk210);

    func_8004B0B0(&stack84);
    func_800598A8(&stack38, &stackD8,
                  func_8004B60C(&stack2C, lbl_eu_806662DC, lbl_eu_806662DC, -f31));
    func_8004B3F0(&stack84, &stack38);

    *(f32*)(self->unk1C + 0x0C) = stack90.x - stack84.x;
    *(f32*)(self->unk1C + 0x14) = stack90.z - stack84.z;
    self->unk1F4 = lbl_eu_806662DC;
    *(f32*)(self->unk1C + 0x10) = stack90.y + stack84.y;
    self->unk1DC = lbl_eu_806662DC;
    func_8006BFC4(reinterpret_cast<int>(self), 0x800);

    self->unk244 = self->unk23C + self->unk240;
    stack8 = *(f32*)(self->unk168) - *(f32*)(self->unk168 + 0xC);
    int big = func_8006BAF0(&stack8) > lbl_eu_8066629C;
    func_80073E74(&stack90, reinterpret_cast<const ml::CVec3*>(self->unk1C + 0x0C),
                  reinterpret_cast<f32*>(self->unk168 + 0x18), big, 0);

    func_8004B79C(&stack78, reinterpret_cast<const ml::CVec3*>(self->unk168 + 0x18));
    func_8006FFA8(self, &outA, &outB);
    f32 sum = stack78.x + scaleByGlobal(self->unk21C);
    stack78.x = sum;
    if (sum > lbl_eu_80663DD4) {
        stack78.x = lbl_eu_80663DD4;
    } else if (sum < outA) {
        stack78.x = outA;
    }
    stack78.x = func_8004BC28(stack78.x);

    func_800733B8(&stack20, self, &stack90, reinterpret_cast<const f32*>(&stack78),
                  self->unk244);
    func_8004B3F0(self->unk1C, func_8004B3F0(self->unk1C + 0x0C, &stack20));
    func_8006D734(self, &stack90);
    func_8004B3F0(self->unk1C + 0x30, &stack90);
    func_8004B3F0(self->unk1C + 0x24, &stack90);
    func_8004B3F0(self->unk1C + 0x3C, &stack90);
    func_8004B3F0(self->unk1C + 0x48, &stack90);
    self->unk228 = lbl_eu_806662A0;
    int r31 = 0;
    if (CfRes_getD80Flag() != 0) {
        func_80071730(&stack68, func_8049603C());
        if (stack68.w >= lbl_eu_8066638C) {
            r31 = 1;
        }
    }
    if (r31 == 0) {
        func_8004B79C(&stack5C, reinterpret_cast<const ml::CVec3*>(self->unk1C + 0x0C));
        func_800707C0(self, &stack5C, &stack90);
    } else {
        func_8004B3F0(self->unk1C + 0x18, self->unk1C + 0x0C);
    }
    func_8006C6E8(reinterpret_cast<nw4r::math::VEC3*>(&stack14),
                  reinterpret_cast<nw4r::math::VEC3*>(&stack90),
                  reinterpret_cast<nw4r::math::VEC3*>(self->unk168 + 0x60));
    func_80071AB0(self, self->unk1C + 0x18, &stack14, 0, lbl_eu_80661B50,
                  lbl_eu_806662DC);
    lbl_eu_80663DE4 = self->unk23C;
    lbl_eu_80570A38[0] = *(f32*)(self->unk168 + 0x18);
    lbl_eu_80570A38[1] = *(f32*)(self->unk168 + 0x1C);
}
// func_80071364: normalize a quaternion in place and hand the pointer back.
Quaternion* func_80071364(Quaternion* q) {
    PSQUATNormalize(q, q);
    return q;
}
// func_80071398: quaternion slerp - out = a * sin((1-t)θ)/sin(θ) + q1 *
// sin(tθ)/sin(θ) where θ = acos(clamp(dot(a, b))) and q1 = -b when the dot
// product is negative. θ near zero (|θ| < epsilon) copies q1 straight out.
// The dot is clamped to [-1, 1] with an nw4r Warning on an out-of-range
// input, mirroring the retail assert block.
void func_80071398(Quaternion* out, const Quaternion* a, const Quaternion* b, float t) {
    float dot = PSQUATDotProduct(a, b);
    Quaternion q1;
    if (dot < lbl_eu_806662DC) {
        dot = -dot;
        q1.x = -b->x;
        q1.y = -b->y;
        q1.z = -b->z;
        q1.w = -b->w;
    } else {
        q1 = *b;
    }
    if (dot >= lbl_eu_806662D0) {
        dot = lbl_eu_806662D0;
    }
    if (dot < -lbl_eu_806662D0) {
        dot = -lbl_eu_806662D0;
    } else if (dot > lbl_eu_806662D0) {
        dot = lbl_eu_806662D0;
    }
    int valid = 0;
    if (dot <= lbl_eu_806662D0 && dot >= -lbl_eu_806662D0) {
        valid = 1;
    }
    if (valid == 0) {
        nw4r::db::Warning(lbl_eu_805262F0, 0xef, lbl_eu_805262C8);
    }
    float theta = (float)acos(dot);
    if (fabsf(theta) < lbl_eu_8066A208) {
        *out = q1;
    } else {
        Quaternion b2, b1, s2, s1, sum;
        float sinTheta = nw4r::math::SinFIdx(lbl_eu_806662E4 * theta);
        float invSin = lbl_eu_806662D0 / sinTheta;
        float w1 = invSin * nw4r::math::SinFIdx(lbl_eu_806662E4 * ((lbl_eu_806662D0 - t) * theta));
        float w2 = invSin * nw4r::math::SinFIdx(lbl_eu_806662E4 * (t * theta));
        PSQUATScale(&q1, &s1, w2);
        PSQUATScale(a, &s2, w1);
        b1.x = s1.x;
        b1.y = s1.y;
        b1.z = s1.z;
        b1.w = s1.w;
        b2.x = s2.x;
        b2.y = s2.y;
        b2.z = s2.z;
        b2.w = s2.w;
        PSQUATAdd(&b2, &b1, &sum);
        out->x = sum.x;
        out->y = sum.y;
        out->z = sum.z;
        out->w = sum.w;
    }
}
// func_80071754: follow-camera per-frame update. The func_8006EEF8 gate ramps
// +0x200 by +0x204 and - once it hits 1.0 - clears the 0x8000 pad flag
// (func_8006BC1C). The 0x400 pad flag drives +0x23C toward the
// (1.0 - +0x200) * +0x218 + +0x20C blend, reset to +0x20C at the cap. The
// func_80071A90 gate decays +0x21C (multipliers chosen by the 0x1000 flag
// and the |+0x21C| magnitude) and snaps it to 0 when small. With the gate
// again, the +0x168/+0x174 rotation vectors build two quats (setRotZXY +
// normalize), slerped by +0x200 (func_80071398) and applied to +0x70 via
// func_80071694; the +0x23C + +0x240 offset is then written into +0x28,
// +0x244 and +0x1F4, and the result is pushed through func_80073E74 /
// func_800733B8 before the vtable-slot-0x3C blend update.
void func_80071754(cf::CfCamFollow* self, ml::CVec3* arg2) {
    if (func_8006EEF8(self) != 0) {
        self->unk200 += self->unk204;
        if (self->unk200 >= lbl_eu_806662D0) {
            self->unk200 = lbl_eu_806662D0;
            if (func_8006BFC4(reinterpret_cast<int>(self), 0x8000) != 0) {
                func_8006BC1C(self, 0x8000);
            }
        }
    }
    if (func_8006BFC4(reinterpret_cast<int>(self), 0x400) != 0) {
        self->unk23C = (lbl_eu_806662D0 - self->unk200) * self->unk218 + self->unk20C;
        if (self->unk200 >= lbl_eu_806662D0) {
            func_8006BC1C(self, 0x400);
            self->unk23C = self->unk20C;
        }
    }
    if (func_80071A90(self) != 0) {
        if (func_8006C6B4(reinterpret_cast<int>(self), 0x1000) != 0) {
            self->unk21C *= lbl_eu_80666318;
        } else if (func_8006BAF0(&self->unk21C) > lbl_eu_8066636C) {
            self->unk21C *= lbl_eu_806662D8;
        } else {
            self->unk21C *= lbl_eu_806662F0;
        }
        if (func_8006BAF0(&self->unk21C) < lbl_eu_8066A208) {
            self->unk21C = lbl_eu_806662DC;
            func_8006BC1C(self, 0x1000);
        }
    }
    if (func_8006EEF8(self) != 0) {
        if (self->unk200 >= lbl_eu_806662D0) {
            func_80071A9C(self);
        }
        ml::CQuat stack70;
        ml::CQuat stack60;
        ml::CQuat stack50;
        ml::CQuat stack40;
        ml::CVec3 stack30;
        ml::CVec3 stack24;
        ml::CVec3 stack18;
        ml::CVec3 stackC;
        ml::CVec3 stack8;
        f32 t = self->unk200;
        f32 f30 = self->unk23C + self->unk240;
        nopFunc(&stack70);
        func_8004B0B4(&stack60);
        func_8004B0B4(&stack50);
        stack60.setRotZXY(reinterpret_cast<const ml::CVec3*>(self->unk168));
        func_80071364(reinterpret_cast<Quaternion*>(&stack60));
        stack50.setRotZXY(reinterpret_cast<const ml::CVec3*>(self->unk168 + 0xC));
        func_80071364(reinterpret_cast<Quaternion*>(&stack50));
        func_80071398(reinterpret_cast<Quaternion*>(&stack40),
                      reinterpret_cast<Quaternion*>(&stack60),
                      reinterpret_cast<Quaternion*>(&stack50), t);
        func_80071364(reinterpret_cast<Quaternion*>(&stack40));
        func_80071694(&stack70, &stack40);
        func_8004B0B0(&stack30);
        func_800598A8(&stack24, &stack70,
                      func_8004B60C(&stack18, lbl_eu_806662DC, lbl_eu_806662DC, -f30));
        func_8004B3F0(&stack30, &stack24);
        *(f32*)(self->unk1C + 0x0C) = arg2->x - stack30.x;
        *(f32*)(self->unk1C + 0x14) = arg2->z - stack30.z;
        *(f32*)(self->unk1C + 0x10) = arg2->y + stack30.y;
        self->unk1F4 = lbl_eu_806662DC;
        self->unk244 = f30;
        stack8.x = *(f32*)(self->unk168) - *(f32*)(self->unk168 + 0xC);
        int flag = func_8006BAF0(&stack8) > lbl_eu_8066629C;
        func_80073E74(arg2, reinterpret_cast<const ml::CVec3*>(self->unk1C + 0x0C),
                      reinterpret_cast<f32*>(self->unk168 + 0x18), flag, 0);
        func_800733B8(&stackC, self, arg2, reinterpret_cast<const f32*>(self->unk168 + 0x18),
                      self->unk244);
        func_8004B3F0(self->unk1C + 0x0C, &stackC);
        f32 v = self->unk200 * (lbl_eu_80666300 - self->unk210) + self->unk210;
        reinterpret_cast<cf::CfCamPosSource*>(self)->v13(v);
    }
}
// In-TU definition (bottom of file); forward decl so the call site links.
extern "C" bool func_80071B6C(cf::CfCamFollow* self, float val);
// func_80071AB0: camera-relative position update. Copies src into the +0x1C
// vec3, accumulates the +0x1DC follow distance into its y (offset 0x20),
// mirrors the +0x180/+0x184 floats into the shared lbl_eu_80570A38 vec3, then
// - when the sibling gate func_8006D374 passes - applies f2 through
// func_8049EFF8 on the follow target handle (+0x164) and stores f1 into the
// target's +0x1E0 via func_80071B6C.
void func_80071AB0(void* selfPtr, void* src, void* arg, int c, float f1,
                   float f2) {
    cf::CfCamFollow* self = static_cast<cf::CfCamFollow*>(selfPtr);
    func_8004B3F0(self->unk1C, src);
    f32 h = *(f32*)(self->unk1C + 4);
    h = h + self->unk1DC;
    *(f32*)(self->unk1C + 4) = h;
    lbl_eu_80570A38[0] = self->unk168[0x18];
    lbl_eu_80570A38[1] = self->unk168[0x1C];
    if (func_8006D374(self) != 0) {
        func_8049EFF8(self->unk164, f2, self->unk1C, arg);
        func_80071B6C(static_cast<cf::CfCamFollow*>(self->unk164), f1);
    }
}
// func_80071B7C: camera-position update. Probes arg1 against arg2
// (func_804BE348, mask arg3); on a hit the shared 0x80570A50 / 0x80570A5C
// vec3s are seeded by func_80070784. Then the per-frame object
// (func_80071CDC) is enumerated with func_804B5088(obj, arg1, arg2, 5, 0);
// when it returns non-null, v1/v2 snapshot the object's position/camera
// slots (func_80071CE4 / func_80071CEC) and - if the probe hit and the new
// distance (func_80071CF4 squared length) is closer than the stored one -
// the shared vec3s are updated. Returns whether the probe hit or the
// enumeration produced a result.
int func_80071B7C(ml::CVec3* arg1, ml::CVec3* arg2, int arg3) {
    int ret = func_804BE348(arg1, arg2, arg3, 0, 0);
    int probe = (int)(((unsigned int)(-(int)ret) | (unsigned int)ret) >> 31);
    if (probe != 0) {
        func_80070784(reinterpret_cast<ml::CVec3*>(lbl_eu_80570A50),
                      reinterpret_cast<ml::CVec3*>(lbl_eu_80570A5C));
    }
    cf::CfCamFollow* obj = func_80071CDC();
    int r = func_804B5088(obj, arg1, arg2, 5, 0);
    if (r != 0) {
        ml::CVec3 v1;
        ml::CVec3 v2;
        func_8004B0B0(&v1);
        func_8004B0B0(&v2);
        func_8004B3F0(&v1, func_80071CE4(func_80071CDC()));
        func_8004B3F0(&v2, func_80071CEC(func_80071CDC()));
        if (probe != 0) {
            f32 f = func_80071CF4(reinterpret_cast<const nw4r::math::VEC3*>(arg1),
                                  reinterpret_cast<const nw4r::math::VEC3*>(lbl_eu_80570A50));
            if (func_80071CF4(reinterpret_cast<const nw4r::math::VEC3*>(arg1),
                              reinterpret_cast<const nw4r::math::VEC3*>(&v1)) < f) {
                func_8004B3F0(lbl_eu_80570A50, &v1);
                func_8004B3F0(lbl_eu_80570A5C, &v2);
            }
        } else {
            func_8004B3F0(lbl_eu_80570A50, &v1);
            func_8004B3F0(lbl_eu_80570A5C, &v2);
        }
    }
    int result = 0;
    if (probe != 0 || r != 0) {
        result = 1;
    }
    return result;
}
int func_80071D50(ml::CVec3* out, u32 probeArg, float f) {
    // func_804BE398's retail ABI here passes only four GPR args.
    typedef int (*BE398Fn4)(void*, u32, u32, u32);
    int probe = ((BE398Fn4)func_804BE398)(out, probeArg, 0, 0);
    if (probe != 0) {
        func_80070784(reinterpret_cast<ml::CVec3*>(lbl_eu_80570A50),
                      reinterpret_cast<ml::CVec3*>(lbl_eu_80570A5C));
    }
    ml::CVec3 tmp;
    func_8004B79C(&tmp, out);
    tmp.y += f;
    cf::CfCamFollow* cam = func_80071CDC();
    int hit = func_804B5088(cam, out, &tmp, 5, 0);
    if (hit != 0) {
        ml::CVec3 posA;
        ml::CVec3 posB;
        func_8004B0B0(&posA);
        func_8004B0B0(&posB);
        func_8004B3F0(&posA, func_80071CE4(func_80071CDC()));
        func_8004B3F0(&posB, func_80071CEC(func_80071CDC()));
        if (probe != 0) {
            // keep the shared globals only when the new camera is closer
            f32 best = func_80071CF4(reinterpret_cast<const nw4r::math::VEC3*>(out),
                                     reinterpret_cast<const nw4r::math::VEC3*>(lbl_eu_80570A50));
            f32 d = func_80071CF4(reinterpret_cast<const nw4r::math::VEC3*>(out),
                                  reinterpret_cast<const nw4r::math::VEC3*>(&posA));
            if (d < best) {
                func_8004B3F0(lbl_eu_80570A50, &posA);
                func_8004B3F0(lbl_eu_80570A5C, &posB);
            }
        } else {
            func_8004B3F0(lbl_eu_80570A50, &posA);
            func_8004B3F0(lbl_eu_80570A5C, &posB);
        }
    }
    return probe != 0 || hit != 0 ? 1 : 0;
}
// func_80071ED0: copy the two shared camera-relative position globals
// (lbl_eu_80570A50 / lbl_eu_80570A5C) into the caller's vectors.
void func_80071ED0(ml::CVec3* a, ml::CVec3* b) {
    func_8004B3F0(a, lbl_eu_80570A50);
    func_8004B3F0(b, lbl_eu_80570A5C);
}
// func_80071F14: usable when neither the 0x40000 pad-state bit nor the
// 0x10000000 follow-cam flag bit is set.
extern "C" int func_80071F14(cf::CfCamFollow* self) {
    int result = 0;
    if (func_8006BFC4((int)self, 0x40000) == 0 && func_8006C6B4((int)self, 0x10000000) == 0) {
        result = 1;
    }
    return result;
}
void func_80071F74(){}
void func_80072194(){}
// func_800733B8: follow-camera offset vector builder. With the 0x200 script
// gate clear, out just copies the +0x34 position. Otherwise dir is blended
// toward self+0x28 by a yaw-scaled normalized XZ direction, lifted by the
// pitch sine term clamped by the sign-gated constants.
__declspec(noinline) void func_800733B8(ml::CVec3* out, cf::CfCamFollow* self,
                                        const ml::CVec3* dir, const f32* angles,
                                        float f) {
    if (func_8006EF04(0x200) == 0) {
        func_8004B79C(out, reinterpret_cast<const ml::CVec3*>(self->unk1C + 24));
        return;
    }
    f32 lift = f * func_8004CC68(angles[0]);
    f32 push = f * func_8004CC74(angles[0]);
    if (lbl_eu_80663DE1 != 0) {
        if (angles[0] < lbl_eu_806662DC) {
            lift = lbl_eu_80666318 * func_8004CC68(angles[0]);
        } else {
            lift *= lbl_eu_806662CC;
        }
    } else {
        if (angles[0] < lbl_eu_806662DC) {
            lift *= lbl_eu_80661B38;
        } else {
            lift *= lbl_eu_806662CC;
        }
    }
    ml::CVec3 blend;
    func_8004CB80(&blend.x, reinterpret_cast<const f32*>(&self->unk1C[12]),
                  reinterpret_cast<const f32*>(dir));
    blend.y = lbl_eu_806662DC;
    func_800A3C48(&blend);
    ml::CVec3 res;
    func_8004B0B0(&res);
    res.x = blend.x * push + dir->x;
    res.z = blend.z * push + dir->z;
    res.y = dir->y + lift;
    func_8004B79C(out, &res);
}
// func_80074090: pick a follow aim vector. src == 0 copies pos verbatim.
// Otherwise the source's aim body (slot 0xAC) seeds the working vector and
// sel picks a replacement: >0 uses fnAt12C(sel); <0 indexes the
// lbl_eu_80527244 table into fnAt120; sel == 0 probes pos (func_8006BA80) and
// falls back to fnAt128(). The chosen vector feeds the scaled camera basis
// transform (func_80074010) using the slot-0xCC scale factor.
void func_80074090(ml::CVec3* out, cf::CfCamSrcVt* src, ml::CVec3* pos, int sel) {
    if (src == 0) {
        func_8004B79C(out, pos);
        return;
    }
    nw4r::math::MTX33 pad; // retail reserves this unused stack space
    nopFunc(&pad);
    ml::CVec3 cur;
    func_8004B79C(&cur, src->fn0xAC());
    if (sel > 0) {
        ml::CVec3 v;
        func_8007420C(&v.x, src->fnAt12C(sel));
        func_8004B3F0(&cur, &v);
    } else if (sel < 0) {
        ml::CVec3 v;
        func_8007420C(&v.x, src->fnAt120(lbl_eu_80527244[-sel]));
        func_8004B3F0(&cur, &v);
    } else {
        if (func_8006BA80(pos) != 0) {
            const float* p = src->fnAt128();
            if (p != 0) {
                ml::CVec3 v;
                func_8007420C(&v.x, p);
                func_8004B3F0(&cur, &v);
            }
        }
    }
    func_80074010(reinterpret_cast<nw4r::math::VEC3*>(out),
                  reinterpret_cast<nw4r::math::VEC3*>(&cur),
                  reinterpret_cast<nw4r::math::VEC3*>(pos), src->fn0xCC());
}
// func_8007351C: collision-adjusted camera reset helper. Probes arg3 against
// a copy of itself lowered by lbl_eu_80661B58 (func_804BE348, mask
// 0x40000). On a hit: v1/v2 are seeded by func_80070784, arg4 takes v1 and
// its y bumps by lbl_eu_80661B58, the 0x80570A74 / 0x80570A80 globals are
// refreshed, and when the CfGameManager state gate (8) is set the 0x90/0x120
// message at lbl_eu_804FB4F0+0x92 is emitted. Otherwise arg4 just copies
// arg2. The 0x4000 flag is set on arg1 via func_8006C640 with the hit bit.
int func_8007351C(cf::CfCamFollow* arg1, ml::CVec3* arg2, ml::CVec3* arg3,
                  ml::CVec3* arg4) {
    int result = 0;
    ml::CVec3 tmp;
    func_8004B79C(&tmp, arg2);
    tmp.y = tmp.y - lbl_eu_80661B58;
    if (func_804BE348(arg3, &tmp, 0, 0x40000, 0) != 0) {
        ml::CVec3 v1;
        ml::CVec3 v2;
        func_8004B0B0(&v1);
        func_8004B0B0(&v2);
        func_80070784(&v1, &v2);
        func_8004B3F0(arg4, &v1);
        arg4->y = arg4->y + lbl_eu_80661B58;
        func_8004B3F0(lbl_eu_80570A74, arg4);
        func_8004B3F0(lbl_eu_80570A80, &v2);
        result = 1;
        if (func_8007FE24__Q22cf13CfGameManagerFv(8) != 0) {
            func_800C1DF0(0x90, 0x120, lbl_eu_804FB4F0 + 0x92);
        }
    } else {
        func_8004B3F0(arg4, arg2);
    }
    func_8006C640(arg1, 0x4000, result);
    return result;
}
void func_80073640(){}
// func_80073D8C: compute a follow-parameter vec3 via func_80074090 and
// propagate it through the camera body: the vec3 at +0x28 (position copy)
// gets the fresh value, the vec3 at +0x34 (source pos) mirrors it.
// func_8004B3F0 returns its dst, feeding the second copy's source.
void func_80073D8C(cf::CfCamFollow* self, int cond) {
    if (cond != 0) {
        ml::CVec3 local;
        // full 4-arg retail shape; this caller only needs the out vector
        func_80074090(&local, reinterpret_cast<cf::CfCamSrcVt*>(0), &local, 0);
        func_8004B3F0(self->unk1C + 24, func_8004B3F0(self->unk1C + 12, &local));
    }
}
// func_80073DDC: dynamic_cast the source object to the camera-state type and,
// on success, copy its state block (positions + the 0x1E0 float) into the
// follow camera. The final copy reuses the +0x40 vector as the source for
// self's +0x10 slot.
void func_80073DDC(cf::CfCamFollow* self, cf::CfCamPosSource* src) {
    CfCamStateView* state = reinterpret_cast<CfCamStateView*>(
        __dynamic_cast(src, 0, &lbl_eu_80661B28, &lbl_eu_80661B30, 0));
    if (state != NULL) {
        CfCamStateView* dst = reinterpret_cast<CfCamStateView*>(self);
        func_8004B3F0(&dst->field34, &state->field34);
        func_8004B3F0(&dst->field28, &state->field28);
        func_8004B3F0(&dst->field58, &state->field58);
        func_8004B3F0(&dst->field40, &state->field40);
        dst->field1E0 = state->field1E0;
        func_8004B3F0(&dst->field10, &state->field40);
    }
}
// func_80073C7C: build the follow-camera offset vector from a direction
// vec3 `d` and a distance `f`. The result temp starts zeroed, then:
//   tmp[0] = (f*cos(d[0])) * -sin(d[4]);  tmp[2] = (f*cos(d[0])) * -cos(d[4])
//   tmp[1] = f*sin(d[0]), clamped by sign: >= 0 scaled by 0x806662CC;
//   negative values scale by 0x80661B38, or by 0x80666318*sin(d[0]) when the
//   0x80663DE1 gate is set. `c` + tmp is summed into `a` (func_8006C6E8).
void func_80073C7C(ml::CVec3* a, cf::CfCamFollow* b, ml::CVec3* c, const f32* d, float f) {
    f32 tmp[3];
    func_8004B0B0(tmp);
    f32 s = f * func_8004CC74(d[0]);
    tmp[0] = s * -func_8004CC68(d[1]);
    tmp[2] = s * -func_8004CC74(d[1]);
    f32 t = f * func_8004CC68(d[0]);
    tmp[1] = t;
    if (t >= lbl_eu_806662DC) {
        tmp[1] = t * lbl_eu_806662CC;
    } else if (lbl_eu_80663DE1 != 0) {
        tmp[1] = func_8004CC68(d[0]) * lbl_eu_80666318;
    } else {
        tmp[1] = t * lbl_eu_80661B38;
    }
    func_8006C6E8(reinterpret_cast<nw4r::math::VEC3*>(a),
                  reinterpret_cast<const nw4r::math::VEC3*>(c),
                  reinterpret_cast<const nw4r::math::VEC3*>(tmp));
}
// func_80073E74: direction/angle follow adjustment. tmp = arg2 - arg1 (XZ
// plane work), len = XZ length (func_80073F88), out[1] = yaw(tmp). Then
// func_8004CC40 fills out[0] = sin-scale with the scale/span chosen by the
// 0x80663DE1 gate, flag2 and flag3:
//   gate && flag2: tmp[1] >= 0 -> scale 0x80666318, span len; else scale
//   0x80663DD0, span 1.0
//   !gate && flag2: flag3 == 0 additionally scales tmp[1] by 0x80666318,
//   span is always len
void func_80073E74(const ml::CVec3* arg1, const ml::CVec3* arg2, f32* out, int flag2, int flag3) {
    f32 tmp[3];
    func_8004CB80(tmp, reinterpret_cast<const f32*>(arg2), reinterpret_cast<const f32*>(arg1));
    f32 len = func_80073F88(tmp);
    out[1] = func_8006D3D0(static_cast<const void*>(tmp));
    if (lbl_eu_80663DE1 != 0) {
        if (flag2 != 0) {
            if (tmp[1] >= lbl_eu_806662DC) {
                tmp[1] *= lbl_eu_80666318;
                out[0] = func_8004CC40(tmp[1], len);
            } else {
                tmp[1] *= lbl_eu_80663DD0;
                out[0] = func_8004CC40(tmp[1], lbl_eu_806662D0);
            }
        }
    } else if (flag2 != 0) {
        if (flag3 != 0) {
            out[0] = func_8004CC40(tmp[1], len);
        } else {
            tmp[1] *= lbl_eu_80666318;
            out[0] = func_8004CC40(tmp[1], len);
        }
    }
}
// func_80073F88: XZ-plane length sqrt (len2 = x*x + z*z) with the nw4r FSqrt
// assert path (Warning on negative/NaN squared length) and 0 for non-positive
// len2. Written x*x + z*z so MWCC computes the fmadds addend (z*z) first and
// loads z (+8) before x (+0), matching retail.
float func_80073F88(const ml::CVec3* v) {
    float len2 = v->x * v->x + v->z * v->z;
    if (!(len2 >= lbl_eu_806662DC)) {
        nw4r::db::Warning(lbl_eu_80526324, 0x273, lbl_eu_80526300);
    }
    return len2 <= lbl_eu_806662DC ? lbl_eu_806662DC : len2 * nw4r::math::FrSqrt(len2);
}
// func_80074010: build a follow-parameter vec3 - init a stack temp, scale it
// by the float arg via func_80059610, derive the follow offset with
// func_800598A8, then hand the result to func_8006C6E8 (PS kernel in the .inl).
void func_80074010(nw4r::math::VEC3* a, nw4r::math::VEC3* b, nw4r::math::VEC3* c, float f) {
    nw4r::math::VEC3 tmp2;
    nw4r::math::MTX33 tmp1; // larger stack slot - retail reserves extra frame space here
    nopFunc(&tmp1);
    func_80059610(&tmp1, f);
    func_800598A8(&tmp2, &tmp1, c);
    func_8006C6E8(a, b, &tmp2);
}
// func_80074230: transform two aim vectors by the inverse camera matrix of a
// camera-position source. Builds an MTX34 on the stack, scales it by the
// source's vtable-slot-0xCC factor (func_800742FC), stamps the slot-0xAC aim
// vector into the translation column (func_800743A4), inverts in place
// (func_800743C0), then maps dst1/dst2 through it via func_8006D6A8 + copy
// back. The source pointer is a cf::CfCamPosSource view.
void func_80074230(cf::CfCamPosSource* src, ml::CVec3* dst1, ml::CVec3* dst2) {
    if (src == 0) {
        return;
    }
    nw4r::math::MTX34 mtx;
    func_8006BEC0(&mtx);
    func_800742FC(mtx.a, src->fn0xCC());
    func_800743A4(mtx.a, src->fn0xAC());
    func_800743C0(static_cast<void*>(&mtx));
    ml::CVec3 tmp1;
    func_8006D6A8(&tmp1, mtx.mtx, dst1);
    func_8004B3F0(dst1, &tmp1);
    ml::CVec3 tmp2;
    func_8006D6A8(&tmp2, mtx.mtx, dst2);
    func_8004B3F0(dst2, &tmp2);
}
void func_80074488(){}
void func_80074534(){}
void func_80074900(){}
// func_800749AC: direction vector from `from` to `to` (tmp = to - from); the
// tmp length is returned, and the caller's vec gets a pitch/yaw pair via the
// sin-wrapper func_8004CC40 (out[1] negated).
float func_800749AC(const ml::CVec3* from, const ml::CVec3* to, float* out) {
    ml::CVec3 tmp;
    func_8004CB80(&tmp.x, &to->x, &from->x);
    float dist = func_8006BB00(&tmp);
    float r = func_800A3EF4(tmp.x * tmp.x + tmp.z * tmp.z);
    out[0] = func_8004CC40(tmp.y, r);
    out[1] = -func_8004CC40(tmp.x, -tmp.z);
    return dist;
}
// func_80074A3C: pick the active pad source - the remote GC pad when the
// 0x8000000 availability bit is set, otherwise the CfGameManager pad.
CPad* func_80074A3C(int controllerId) {
    if (func_8006BFC4(controllerId, 0x8000000) != 0) {
        return CDeviceRemotePad::getMainGCPad();
    }
    return cf::CfGameManager::getCurrentPad();
}
// func_80074A74: whether the current pad is a classic controller (type 4).
__declspec(noinline) bool func_80074A74() {
    return cf::CfGameManager::getCurrentPad()->mPadType == PAD_TYPE_CLASSIC;
}
void func_80074AA4(){}
// func_80074CEC: allocate a 0x25c-byte CfCamFollow block on the mtl heap and
// construct it with (self, arg2).
extern "C" void func_80074CEC(void* self, void* arg2) {
    u32 heap = func_80061FE8();
    cf::CfCamFollow* cam = (cf::CfCamFollow*)allocate__Q23mtl10MemManagerFUlUl(0x25c, heap);
    if (cam != 0) {
        __ct__cf_CfCamFollow(cam, self, arg2);
    }
}
extern "C" int CfObject_UnkVirtualFunc48__Q22cf8CfObjectFv() { return 0; }
extern "C" int CfObject_UnkVirtualFunc55__Q22cf8CfObjectFv() { return 0; }
extern "C" int CfObject_UnkVirtualFunc52__Q22cf8CfObjectFv() { return 0; }
// cf::CfObject base virtuals (retail: li r3,0; blr / li r3,1; blr)
extern "C" u32 CfObject_UnkVirtualFunc54__Q22cf8CfObjectFv(void* self) { return 0; }
extern "C" u32 CfObject_UnkVirtualFunc9__Q22cf8CfObjectFv(void* self) { return 1; }
// cf::CActorParam base virtual (retail: lwz r3,0x15e4(r3); blr)
extern "C" u32 CActorParam_UnkVirtualFunc22__Q22cf11CActorParamFv(void* self) { return *(u32*)((char*)self + 0x15e4); }

extern "C" bool func_8006B6C0() { return true; }
extern "C" __declspec(noinline) void func_8006BF08(u16* p) { *p = 0; }
// Return a pointer to element `index` of a stride-12 (Vec3) array at `self`.
// noinline keeps the retail `bl func_8006BF14` at call sites.
extern "C" __declspec(noinline) u8* func_8006BF14(u8* self, int index) {
    return self + index * 12;
}
// Store the follow distance into both the shared global and the object field.
extern "C" void func_8006C1BC(cf::CfCamFollow* self, f32 value) {
    lbl_eu_80661B50 = value;
    self->unk1E0 = value;
}
// Bit 1 of the 0x4EC flag word. C++ linkage (mangled) so the extern "C"
// (const void*) declaration in CfCam.hpp stays a distinct signature and call
// sites keep the unmangled `bl func_8006CA20`.
unsigned int func_8006CA20(const cf::CfCamFollow* self) {
    return (self->field_0x4EC >> 1) & 1;
}
// Bit 2 of the 0x64 flag word.
extern "C" unsigned int func_8006DF9C(const cf::CfCamFollow* self) {
    return (self->field_0x64 >> 2) & 1;
}
extern "C" int func_8006E5B8(void* self) { return 0x10; }
extern "C" bool func_80071B6C(cf::CfCamFollow* self, float val) {
    self->unk1E0 = val;
    return func_8049EB60(self);
}
extern "C" u8* func_80071CEC(cf::CfCamFollow* self) { return self->unk1C + 0x50; }
