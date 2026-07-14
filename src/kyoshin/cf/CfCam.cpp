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
