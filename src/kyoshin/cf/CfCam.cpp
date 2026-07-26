#include "kyoshin/cf/CfCam.hpp"

extern "C" {
void* __ct__cf_CfCam(void* self, void* arg);
void func_8004B0B0(void* self);
void func_8004B60C(void* self, f32 x, f32 y, f32 z);
void func_8006BEF8(void* self);
void func_8006BEC0(void* self);
void func_8006BEC4(void* self);
void func_8006BEE4(void* self);
void func_8006BEFC(void* self);
void func_8006BF08(void* self);
void* func_8006BF14(void* self, int index);
void* memset(void* dest, int val, u32 count);

char lbl_eu_80527260[];
cf::CfCamFollow* lbl_eu_80663DEC;

extern const f32 lbl_eu_806662DC; // 0.0f
extern const f32 lbl_eu_806662B8; // 8.0f
extern f32 lbl_eu_80661B50;       // 40.0f (.sdata)
extern const f32 lbl_eu_806662A0; // 0.1f
extern const f32 lbl_eu_806662F0; // 0.4f
extern const f32 lbl_eu_806662D0; // 1.0f
}

typedef void (*CfCamVFn_ui)(cf::CfCamFollow* self, u32 arg);
typedef void (*CfCamVFn)(cf::CfCamFollow* self);

extern "C" cf::CfCamFollow* __ct__cf_CfCamFollow(cf::CfCamFollow* self, void* arg1,
                                                 void* arg2) {
    u8* base = reinterpret_cast<u8*>(self);

    __ct__cf_CfCam(self, arg2);
    self->vtable = lbl_eu_80527260;

    func_8004B0B0(base + 0x1C);
    func_8004B60C(base + 0x28, lbl_eu_806662DC, lbl_eu_806662DC, lbl_eu_806662DC);
    func_8004B60C(base + 0x34, lbl_eu_806662DC, lbl_eu_806662DC, lbl_eu_806662DC);
    func_8004B0B0(base + 0x40);
    func_8004B0B0(base + 0x4C);
    func_8004B0B0(base + 0x58);
    func_8004B60C(base + 0x64, lbl_eu_806662DC, lbl_eu_806662DC, lbl_eu_806662DC);
    func_8006BEF8(base + 0x70);
    func_8006BEC0(base + 0x130);

    self->unk164 = arg1;

    func_8004B0B0(base + 0x168);
    func_8004B0B0(base + 0x174);
    func_8004B60C(base + 0x180, lbl_eu_806662DC, lbl_eu_806662DC, lbl_eu_806662DC);
    func_8004B60C(base + 0x18C, lbl_eu_806662DC, lbl_eu_806662DC, lbl_eu_806662DC);
    func_8004B0B0(base + 0x198);
    func_8004B0B0(base + 0x1A4);
    func_8004B0B0(base + 0x1B0);
    func_8004B0B0(base + 0x1BC);
    func_8004B60C(base + 0x1C8, lbl_eu_806662DC, lbl_eu_806662DC, lbl_eu_806662DC);
    func_8006BEFC(base + 0x1D4);
    func_8006BF08(base + 0x1D8);

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

    func_8006BEC4(base + 0x10);
    func_8006BEC4(base + 0x1A4);
    func_8006BEC4(base + 0x1B0);
    func_8006BEC4(base + 0x1BC);
    func_8006BEE4(self);
    memset(func_8006BF14(base + 0x70, 0), 0, 0xC0);

    self->unk250 = 0;
    self->unk22C = lbl_eu_806662D0;
    self->unk230 = lbl_eu_806662D0;

    reinterpret_cast<CfCamVFn_ui>((*reinterpret_cast<void***>(self))[0x40 / 4])(self, 1);
    reinterpret_cast<CfCamVFn>((*reinterpret_cast<void***>(self))[0x28 / 4])(self);

    return self;
}

extern "C" void* func_8006B6A0(void* self) { return (char*)self + 0x10; }
extern "C" u32 func_8006B6A8(const void* self) { return *(const u32*)((const char*)self + 0x164); }
extern "C" void* func_8006B6B0(void* ptr) { return static_cast<char*>(ptr) + 0x40; }
extern "C" void* func_8006B6B8(void* self) { return (char*)self + 0x1c; }
extern "C" bool func_8006B6C0() { return true; }
extern "C" void func_8006BA80() {}
extern "C" float func_8006BAF0(void* self) {
    return __fabs(*(float*)self);
}
extern "C" float PSVECMag(const float* v);
extern "C" float func_8006BB00(const float* v) { return PSVECMag(v); }
extern "C" void func_8006BB04() {}
extern "C" void func_8006BBF4(void* r3, int r4, int r5) {
    if (r5) {
        *(int*)((char*)r3 + 4) |= r4;
    } else {
        *(int*)((char*)r3 + 4) &= ~r4;
    }
}
extern "C" void func_8006BC1C(unsigned int* p, unsigned int mask) {
    p[1] &= ~mask;
}
extern "C" void __ct__Q22cf5CfCamFv() {}
extern "C" void func_8006BEC0(void* self) {}
extern "C" void func_8006BEC4(void* self) {}
extern "C" void func_8006BEE4(void* p) {
    ((u32*)p)[1] = 0;
}
extern "C" void func_8006BEF0(void* self, int val) { *(int*)((char*)self + 0x8) = val; }
extern "C" void func_8006BEF8(void* self) {}
void func_8006BEFC(void* param_1) { *(u32*)param_1 = 0; }
extern "C" void func_8006BF08(void* ptr) {
    *(short*)ptr = 0;
}
void* func_8006BF14(void* r3, int r4) { return (char*)r3 + r4 * 12; }
extern "C" u32 func_8006BFBC(void *self) { return *(u32 *)((char *)self + 8); }
extern "C" bool func_8006BFC4(const void* self, unsigned int mask) { return ((*reinterpret_cast<const unsigned int*>(static_cast<const unsigned char*>(self) + 4)) & mask) != 0; }
extern "C" int func_8006C1B0(void* p) {
    return (*(int*)((char*)p + 0x64) >> 1) & 1;
}
extern "C" void func_8006C1BC(void* ptr, float f) {
    lbl_eu_80661B50 = f;
    *(float*)((u8*)ptr + 0x1e0) = f;
}
extern "C" void func_8006C640(void* obj, unsigned int bits, int set_flag) {
    unsigned int* ptr = (unsigned int*)((char*)obj + 0x1d4);
    if (set_flag != 0)
        *ptr |= bits;
    else
        *ptr &= ~bits;
}
extern "C" int func_8006C670(void* self) { return *(int*)((char*)self + 0xc4); }
extern "C" int func_8006C678(void* p) {
    return *(unsigned short*)((unsigned char*)p + 0x530) & 1;
}
extern "C" bool func_8006C684(const unsigned char* this_) { return (*(const unsigned short*)(this_ + 0x530) & 0x000C) != 0; }
extern "C" bool func_8006C69C(const void* self) { return (*(const unsigned short*)((const unsigned char*)self + 0x530) & 0x30) != 0; }
extern "C" bool func_8006C6B4(void* self, unsigned int mask) { return ((*(unsigned int*)((char*)self + 0x1d4) & mask) != 0); }
extern "C" void func_8006C6CC() {}
extern "C" void func_8006C6E8() {}
extern "C" void func_8006C730(void* _this, unsigned int mask) {
    *(unsigned int*)((char*)_this + 0x1d4) &= ~mask;
}
extern "C" int func_8006CA20(void* _this) {
    return (*(int*)((char*)_this + 0x4EC) >> 1) & 1;
}
extern "C" void func_8006CBD8() {}
extern "C" void func_8006CBEC() {}
extern "C" void func_8006CC4C() {}
extern "C" void func_8006CE18(void* arg0) {
    *(u32*)((char*)arg0 + 0x1d4) = 0;
}
extern "C" u32 func_8006D374(void* p) {
    return *(u32*)((u8*)p + 4) >> 31;
}
extern "C" u32 func_8006D400(void *self) { return *(u32*)((char*)self + 4); }
extern "C" float func_8006D408(void* self) { return *(float*)((char*)self + 0x1e0); }
extern "C" float func_8006D410(float f) {
    extern float lbl_eu_8066A20C;
    return f * lbl_eu_8066A20C;
}
extern "C" void func_8006D440(unsigned* p) {
    p[1] |= 0x200;
}
extern "C" void* func_8006D6F8(void* ptr) { return (char*)ptr + 0x9c; }
extern "C" bool func_8006D72C(void* param_1) {
    return ((bool (*)(void*, int))func_8006BFC4)(param_1, 256);
}
extern "C" unsigned int func_8006DC34(unsigned char* self) {
    return (*(unsigned int*)(self + 0x4ec) >> 26) & 1;
}
extern "C" int func_8006DC40(void* p) {
    return (*(unsigned int*)((char*)p + 0x4EC) >> 24) & 1;
}
extern "C" int func_8006DC4C(void* self, unsigned int mask) { return ((*((unsigned int*)((char*)self + 0x4ec)) & mask) != 0); }
extern "C" uint32_t func_8006DC64(void* self) {
    return (*(uint32_t*)((char*)self + 0x4ec) >> 25) & 1;
}
extern "C" int func_8006DC70(void* self) {
    return (*(const unsigned int*)((const char*)self + 0x4ec) >> 23) & 1;
}
struct CfCam;
extern "C" float func_8006DC7C(CfCam* self) {
    return *(float *)((char *)self + 0x4FC);
}
extern "C" float func_8006DC84(void* self) { return *(float*)((char*)self + 0x4f8); }
extern "C" float func_8006DC8C(const void* self) {
    return *(const float*)((const char*)self + 0x508);
}
extern "C" int func_8006DC94(void* self) { return (*(uint32_t*)((char*)self + 0x4ec) >> 13) & 1; }
extern "C" unsigned int func_8006DF9C(void* self) {
    unsigned int val = *(unsigned int*)((unsigned char*)self + 0x64);
    return (val >> 2) & 1U;
}
extern "C" int func_8006DFA8(void *self) { return *(int *)((char *)self + 0x90e4); }
extern "C" unsigned int func_8006DFBC(void *self) {
    unsigned int word = *(unsigned int *)((char *)self + 0x64);
    return (word >> 27) & 1U;
}
extern "C" void func_8006DFC8() {}
extern int lbl_eu_80663DF0;

extern "C" int func_8006E59C()
{
    return lbl_eu_80663DF0;
}
extern "C" bool func_800755B0(void*, int);
extern "C" bool func_8006E5A4(void* self) { return func_800755B0(self, 0); }
extern "C" void func_8006E5AC(int* dst, int* src) {
    *dst = *src;
}
extern "C" int func_8006E5B8() { return 0x10; }
extern "C" void func_8006E5C0(unsigned* dst, const unsigned* src) {
    *dst = *src;
}
extern "C" void func_8006E5CC(u16* dest, const u16* src) {
    *dest = *src;
}
extern unsigned int lbl_eu_80663E28;

extern "C" int func_8006EEE4() {
    return (lbl_eu_80663E28 >> 24) & 1;
}
extern "C" void* func_8006EEF0(void* self) { return static_cast<char*>(self) + 0x3d8; }
extern "C" int func_8006EEF8(unsigned int* arg0) {
    return (arg0[1] >> 9) & 1;
}
extern "C" void func_8006EF04__Fi() {}
extern "C" void func_8006EF1C() {}
extern "C" void func_8006F9D4() {}
extern "C" void func_80071694() {}
extern "C" void func_80071730(void *r3, const void *r4) {
    const unsigned int *src = (const unsigned int *)r4;
    unsigned int *dst = (unsigned int *)r3;
    dst[0] = src[0];
    dst[1] = src[1];
    dst[2] = src[2];
    dst[3] = src[3];
}
extern "C" int func_80071A90(void* ptr) {
    return (*(unsigned int*)((char*)ptr + 4) >> 12) & 1;
}
extern "C" void func_80071A9C(void* self) { *(reinterpret_cast<unsigned int*>(reinterpret_cast<unsigned char*>(self) + 4)) &= 0xFFFFEDFFu; }
extern "C" bool func_8049EB60(void* obj);

extern "C" bool func_80071B6C(void* obj, float val) {
    *(float*)((char*)obj + 0x1e0) = val;
    return func_8049EB60(obj);
}
extern "C" void func_80071B74() {}
extern "C" void func_80071B78() {}
extern "C" int func_80071CDC() {
    extern int lbl_eu_80665958;
    return lbl_eu_80665958;
}
extern "C" void* func_80071CE4(void* self) { return static_cast<char*>(self) + 0x60; }
extern "C" void* func_80071CEC(void* ptr) {
    return (char*)ptr + 0x6c;
}
extern "C" void func_80071CF4() {}
extern "C" int func_80073390(void* _this) {
    return ((*(unsigned*)((char*)_this + 0xc)) >> 19) & 1;
}
extern "C" unsigned int lbl_eu_80663E24;

extern "C" int func_8007339C() {
    return (lbl_eu_80663E24 >> 6) & 1;
}
extern "C" float func_800733B0(const void* arg) { return *(const float*)((const char*)arg + 0x14); }
extern "C" void func_8004B3F0(void*);

extern "C" void func_80073C74(void* p) {
    func_8004B3F0((char*)p + 0x10);
}
extern "C" void func_8007420C() {}
extern "C" void func_800743A4(float* dst, const float* src) {
    dst[3] = src[0];
    dst[7] = src[1];
    dst[11] = src[2];
}
extern "C" void func_800743C0() {}
extern "C" bool func_80074CD4(const unsigned* value, unsigned mask) { return (*value & mask) != 0; }
extern "C" int func_80074D44(void* self) { return 0x0; }
extern "C" void* func_80074D4C(void* p) {
    return (void*)((char*)*(void**)((char*)p + 0xC) + 0x118);
}
extern "C" float func_80074D58(void* self) { return *(float*)((char*)self + 0x1e0); }
extern "C" void func_80074D60(u32* self, const u32* src)
{
    self[10] = src[0];
    self[11] = src[1];
    self[12] = src[2];
}
extern "C" void sinit_80074D7C() {}

extern "C" void func_8006B720() {}
extern "C" void func_8006B8E4() {}
extern "C" float lbl_eu_80527230[];
extern "C" float lbl_eu_80661BA8;

struct Class_80296898 {
    static void* getInstance();
};

extern "C" void func_8006B948() {
    unsigned char idx = reinterpret_cast<unsigned char*>(Class_80296898::getInstance())[4];
    lbl_eu_80661BA8 = lbl_eu_80527230[idx];
}
extern "C" void func_8006B980() {}
extern "C" void func_8006BB20() {}
extern "C" void func_8006BFDC() {}
extern "C" void func_8006C16C() {}
extern "C" void func_8006C1C8() {}
extern "C" void func_8006C740() {}
extern "C" void func_8006CA2C() {}
extern "C" void func_8006CB0C() {}
extern "C" void func_8006CC0C() {}
extern "C" void func_8006CC68() {}
extern "C" void func_8006CE24() {}
extern "C" void func_8006D380() {}
extern "C" void func_8006D3D0() {}
extern "C" void func_8006D41C() {}
extern "C" void func_8006D450() {}
extern "C" void func_8006D6A8() {}
extern "C" void func_8006D700() {}
extern "C" void func_8006D734() {}
extern "C" void func_8006D7A8() {}
extern "C" void func_8006D804() {}
extern "C" void func_8006D8D0() {}
extern "C" void func_8006DBD4() {}
extern "C" void func_8006DCA0() {}
extern "C" void func_8006DD58() {}
extern "C" void func_8006DFE0() {}
extern "C" void func_8006E0C8() {}
extern "C" void func_8006E18C() {}
extern "C" void func_8006E2FC() {}
extern "C" void func_8006E5D8() {}
extern "C" void func_8006E884() {}
extern "C" void func_8006EFA0() {}
extern "C" void func_8006F5C8() {}
extern "C" void func_8006F9EC() {}
extern "C" void func_8006FC44() {}
extern "C" void func_8006FD3C() {}
extern "C" void func_8006FFA8() {}
extern "C" void func_80070088() {}
extern "C" void func_80070518() {}
extern "C" void func_800705D8() {}
extern "C" void func_80070674() {}
extern "C" void func_804BE4B4(void*, int);
extern "C" void* func_804BE4E0(void*, int);

extern "C" void* func_80070784(void* a, void* b) {
    func_804BE4B4(a, 0);
    return func_804BE4E0(b, 0);
}
extern "C" void func_800707C0() {}
extern "C" void func_80070EBC() {}
extern "C" void func_80070FB8() {}
extern "C" void func_80071364() {}
extern "C" void func_80071398() {}
extern "C" void func_80071754() {}
extern "C" void func_80071AB0() {}
extern "C" void func_80071B7C() {}
extern "C" void func_80071D50() {}
extern "C" void func_80071ED0() {}
extern "C" void func_80071F14() {}
extern "C" void func_80071F74() {}
extern "C" void func_80072194() {}
extern "C" void func_800733B8() {}
extern "C" void func_8007351C() {}
extern "C" void func_80073640() {}
extern "C" void func_80073C7C() {}
extern "C" void func_80073D8C() {}
extern "C" void func_80073DDC() {}
extern "C" void func_80073E74() {}
extern "C" void func_80073F88() {}
extern "C" void func_80074010() {}
extern "C" void func_80074090() {}
extern "C" void func_80074230() {}
extern "C" void func_800742FC() {}
extern "C" void func_80074488() {}
extern "C" void func_80074534() {}
extern "C" void func_80074900() {}
extern "C" void func_800749AC() {}
extern "C" void func_80074A3C() {}
extern "C" void func_80074A74() {}
extern "C" void func_80074AA4() {}
extern "C" void func_80074CEC() {}
extern "C" int CfObject_UnkVirtualFunc48__Q22cf8CfObjectFv() { return 0; }
extern "C" int CfObject_UnkVirtualFunc55__Q22cf8CfObjectFv() { return 0; }
extern "C" int CfObject_UnkVirtualFunc52__Q22cf8CfObjectFv() { return 0; }
