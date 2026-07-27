#include "kyoshin/cf/CfCam.hpp"

void* __ct__cf_CfCam(void* self, void* arg);
void func_8006BEC4(void* self);
extern "C" {
void func_8004B0B0(void* self);
void func_8004B60C(void* self, f32 x, f32 y, f32 z);
void cfCam_stub_8006BEF8(void* self);
void cfCam_stub_8006BEC0(void* self);

void cfCam_clearUnk04(void* self);
void cfCam_clearFirstWord(void* self);
void cfCam_clearWord16(void* self);
void* cfCam_getElemStride12(void* self, int index);
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
    __ct__cf_CfCam(self, arg2);
    self->vtable = lbl_eu_80527260;

    func_8004B0B0(self->unk1C);
    func_8004B60C(self->unk1C + 12, lbl_eu_806662DC, lbl_eu_806662DC, lbl_eu_806662DC);
    func_8004B60C(self->unk1C + 24, lbl_eu_806662DC, lbl_eu_806662DC, lbl_eu_806662DC);
    func_8004B0B0(self->unk1C + 36);
    func_8004B0B0(self->unk1C + 48);
    func_8004B0B0(self->unk1C + 60);
    func_8004B60C(self->unk1C + 72, lbl_eu_806662DC, lbl_eu_806662DC, lbl_eu_806662DC);
    cfCam_stub_8006BEF8(self->unk70);
    cfCam_stub_8006BEC0(self->unk130);

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
    cfCam_clearFirstWord(&self->unk1D4);
    cfCam_clearWord16(&self->unk1D8);

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
    cfCam_clearUnk04(self);
    memset(cfCam_getElemStride12(self->unk70, 0), 0, 0xC0);

    self->unk250 = 0;
    self->unk22C = lbl_eu_806662D0;
    self->unk230 = lbl_eu_806662D0;

    {
        void** vtbl = reinterpret_cast<void**>(self->vtable);
        reinterpret_cast<CfCamVFn_ui>(vtbl[16])(self, 1);
        reinterpret_cast<CfCamVFn>(vtbl[10])(self);
    }

    return self;
}

extern "C" void* cfCam_getUnk10Ptr(void* self) { return static_cast<cf::CfCamFollow*>(self)->unk10; }
extern "C" u32 cfCam_getUnk164(const void* self) { return reinterpret_cast<u32>(static_cast<const cf::CfCamFollow*>(self)->unk164); }
extern "C" void* cfCam_getUnk40Ptr(void* ptr) { return static_cast<cf::CfCamFollow*>(ptr)->unk1C + 0x24; }
extern "C" void* cfCam_getUnk1CPtr(void* self) { return static_cast<cf::CfCamFollow*>(self)->unk1C; }
extern "C" bool cfCam_getTrue() { return true; }
void func_8006BA80(){}
float func_8006BAF0(void* self){
    return __fabs(*(float*)self);
}
extern "C" float PSVECMag(const float* v);
extern "C" float cfCam_vecMag(const float* v) { return PSVECMag(v); }
void func_8006BB04(){}
extern "C" void cfCam_setOrClearUnk04Bits(void* r3, int r4, int r5) {
    auto self = static_cast<cf::CfCamFollow*>(r3);
    if (r5) {
        self->unk04 |= r4;
    } else {
        self->unk04 &= ~r4;
    }
}
extern "C" void cfCam_clearUnk04Bits(unsigned int* p, unsigned int mask) {
    p[1] &= ~mask;
}
extern "C" void __ct__Q22cf5CfCamFv() {}
extern "C" void cfCam_stub_8006BEC0(void* self) {}
void func_8006BEC4(void* self){}
extern "C" void cfCam_clearUnk04(void* p) {
    static_cast<cf::CfCamFollow*>(p)->unk04 = 0;
}
extern "C" void cfCam_setUnk08(void* self, int val) { static_cast<cf::CfCamFollow*>(self)->unk08 = val; }
extern "C" void cfCam_stub_8006BEF8(void* self) {}
void cfCam_clearFirstWord(void* param_1) { *(u32*)param_1 = 0; }
extern "C" void cfCam_clearWord16(void* ptr) {
    *(short*)ptr = 0;
}
void* cfCam_getElemStride12(void* r3, int r4) { return (char*)r3 + r4 * 12; }
extern "C" u32 cfCam_getUnk08(void *self) { return static_cast<cf::CfCamFollow*>(self)->unk08; }
extern "C" bool cfCam_testUnk04Bits(const void* self, unsigned int mask) { return (static_cast<const cf::CfCamFollow*>(self)->unk04 & mask) != 0; }
extern "C" int cfCam_getBit1_0x64(void* p) {
    return (*(int*)((char*)p + 0x64) >> 1) & 1;
}
extern "C" void cfCam_setField1E0AndGlobal(void* ptr, float f) {
    lbl_eu_80661B50 = f;
    static_cast<cf::CfCamFollow*>(ptr)->unk1E0 = f;
}
extern "C" void cfCam_setOrClearUnk1D4Bits(void* obj, unsigned int bits, int set_flag) {
    auto self = static_cast<cf::CfCamFollow*>(obj);
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
extern "C" bool cfCam_testBits4_5_0x530(const void* self) { return (*(const unsigned short*)((const unsigned char*)self + 0x530) & 0x30) != 0; }
extern "C" bool cfCam_testUnk1D4Bits(void* self, unsigned int mask) { return (static_cast<cf::CfCamFollow*>(self)->unk1D4 & mask) != 0; }
void func_8006C6CC(){}
void func_8006C6E8(){}
extern "C" void cfCam_clearUnk1D4Bits(void* _this, unsigned int mask) {
    static_cast<cf::CfCamFollow*>(_this)->unk1D4 &= ~mask;
}
extern "C" int cfCam_getBit1_0x4EC(void* _this) {
    return (*(int*)((char*)_this + 0x4EC) >> 1) & 1;
}
void func_8006CBD8(){}
void func_8006CBEC(){}
void func_8006CC4C(){}
extern "C" void cfCam_zeroUnk1D4(void* arg0) {
    static_cast<cf::CfCamFollow*>(arg0)->unk1D4 = 0;
}
extern "C" u32 cfCam_getSignBitUnk04(void* p) {
    return static_cast<cf::CfCamFollow*>(p)->unk04 >> 31;
}
extern "C" u32 cfCam_getUnk04(void *self) { return static_cast<cf::CfCamFollow*>(self)->unk04; }
extern "C" float cfCam_getField1E0(void* self) { return static_cast<cf::CfCamFollow*>(self)->unk1E0; }
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
extern "C" unsigned int cfCam_getBit26_0x4EC(unsigned char* self) {
    return (*(unsigned int*)(self + 0x4ec) >> 26) & 1;
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
struct CfCam;
extern "C" float cfCam_getUnk4FC(CfCam* self) {
    return *(float *)((char *)self + 0x4FC);
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
extern "C" int cfCam_getUnk90E4(void *self) { return *(int *)((char *)self + 0x90e4); }
extern "C" unsigned int cfCam_getBit27_0x64(void *self) {
    unsigned int word = *(unsigned int *)((char *)self + 0x64);
    return (word >> 27) & 1U;
}
void func_8006DFC8(){}
extern int lbl_eu_80663DF0;

extern "C" int cfCam_getGlobal_80663DF0()
{
    return lbl_eu_80663DF0;
}
extern "C" bool func_800755B0(void*, int);
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
void func_8006EF04__Fi(){}
void func_8006EF1C(){}
void func_8006F9D4(){}
void func_80071694(){}
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
extern "C" void cfCam_clearUnk04Bits_EDFF(void* self) { static_cast<cf::CfCamFollow*>(self)->unk04 &= 0xFFFFEDFFu; }
extern "C" bool func_8049EB60(void* obj);

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
void func_80071CF4(){}
extern "C" int cfCam_getBit19_0x0C(void* _this) {
    return ((*(unsigned*)((char*)_this + 0xc)) >> 19) & 1;
}
extern "C" unsigned int lbl_eu_80663E24;

extern "C" int cfCam_getBit6_GlobalE24() {
    return (lbl_eu_80663E24 >> 6) & 1;
}
extern "C" float cfCam_getUnk14Float(const void* arg) { return *(const float*)((const char*)arg + 0x14); }
extern "C" void func_8004B3F0(void*);

extern "C" void cfCam_resetUnk10(void* p) {
    func_8004B3F0((char*)p + 0x10);
}
void func_8007420C(){}
extern "C" void cfCam_setMtxTranslation(float* dst, const float* src) {
    dst[3] = src[0];
    dst[7] = src[1];
    dst[11] = src[2];
}
void func_800743C0(){}
extern "C" bool cfCam_testBits(const unsigned* value, unsigned mask) { return (*value & mask) != 0; }
extern "C" int cfCam_getZero(void* self) { return 0x0; }
extern "C" void* cfCam_getNestedPtr118(void* p) {
    return static_cast<char*>(static_cast<cf::CfCamFollow*>(p)->unk0C) + 0x118;
}
extern "C" float cfCam_getFloat1E0(void* self) { return static_cast<cf::CfCamFollow*>(self)->unk1E0; }
extern "C" void cfCam_copyVec3U32(u32* self, const u32* src)
{
    self[10] = src[0];
    self[11] = src[1];
    self[12] = src[2];
}
void sinit_80074D7C(){}

void func_8006B720(){}
void func_8006B8E4(){}
extern "C" float lbl_eu_80527230[];
extern "C" float lbl_eu_80661BA8;

struct Class_80296898 {
    static void* getInstance();
};

extern "C" void cfCam_updateLbl61BA8() {
    unsigned char idx = reinterpret_cast<unsigned char*>(Class_80296898::getInstance())[4];
    lbl_eu_80661BA8 = lbl_eu_80527230[idx];
}
void func_8006B980(){}
void func_8006BB20(){}
void func_8006BFDC(){}
void func_8006C16C(){}
void func_8006C1C8(){}
void func_8006C740(){}
void func_8006CA2C(){}
void func_8006CB0C(){}
void func_8006CC0C(){}
void func_8006CC68(){}
void func_8006CE24(){}
void func_8006D380(){}
void func_8006D3D0(){}
void func_8006D41C(){}
void func_8006D450(){}
void func_8006D6A8(){}
void func_8006D700(){}
void func_8006D734(){}
void func_8006D7A8(){}
void func_8006D804(){}
void func_8006D8D0(){}
void func_8006DBD4(){}
void func_8006DCA0(){}
void func_8006DD58(){}
void func_8006DFE0(){}
void func_8006E0C8(){}
void func_8006E18C(){}
void func_8006E2FC(){}
void func_8006E5D8(){}
void func_8006E884(){}
void func_8006EFA0(){}
void func_8006F5C8(){}
void func_8006F9EC(){}
void func_8006FC44(){}
void func_8006FD3C(){}
void func_8006FFA8(){}
void func_80070088(){}
void func_80070518(){}
void func_800705D8(){}
void func_80070674(){}
extern "C" void func_804BE4B4(void*, int);
extern "C" void* func_804BE4E0(void*, int);

extern "C" void* cfCam_initAndGetSomething(void* a, void* b) {
    func_804BE4B4(a, 0);
    return func_804BE4E0(b, 0);
}
void func_800707C0(){}
void func_80070EBC(){}
void func_80070FB8(){}
void func_80071364(){}
void func_80071398(){}
void func_80071754(){}
void func_80071AB0(){}
void func_80071B7C(){}
void func_80071D50(){}
void func_80071ED0(){}
void func_80071F14(){}
void func_80071F74(){}
void func_80072194(){}
void func_800733B8(){}
void func_8007351C(){}
void func_80073640(){}
void func_80073C7C(){}
void func_80073D8C(){}
void func_80073DDC(){}
void func_80073E74(){}
void func_80073F88(){}
void func_80074010(){}
void func_80074090(){}
void func_80074230(){}
void func_800742FC(){}
void func_80074488(){}
void func_80074534(){}
void func_80074900(){}
void func_800749AC(){}
void func_80074A3C(){}
void func_80074A74(){}
void func_80074AA4(){}
void func_80074CEC(){}
extern "C" int CfObject_UnkVirtualFunc48__Q22cf8CfObjectFv() { return 0; }
extern "C" int CfObject_UnkVirtualFunc55__Q22cf8CfObjectFv() { return 0; }
extern "C" int CfObject_UnkVirtualFunc52__Q22cf8CfObjectFv() { return 0; }
