#pragma once

#include <types.h>

namespace cf {

// Minimal layout for cf::CfCamFollow ctor (__ct__cf_CfCamFollow).
// Base cf::CfCam occupies +0x00..+0x10; follow extends through ~0x25A.
struct CfCamFollow {
    void* vtable; // 0x00
    u32 unk04;    // 0x04
    u32 unk08;    // 0x08
    void* unk0C;  // 0x0C base ctor arg
    u8 unk10[0x1C - 0x10];
    u8 unk1C[0x70 - 0x1C];
    u8 unk70[0xC0]; // 0x70..0x130
    u8 unk130[0x160 - 0x130];
    void* unk160; // 0x160 ctor arg2
    void* unk164; // 0x164 ctor arg1
    u8 unk168[0x1D4 - 0x168];
    u32 unk1D4; // 0x1D4
    u16 unk1D8; // 0x1D8
    u8 unk1DA[0x1DC - 0x1DA];
    f32 unk1DC;
    f32 unk1E0;
    f32 unk1E4;
    f32 unk1E8;
    f32 unk1EC;
    f32 unk1F0;
    f32 unk1F4;
    f32 unk1F8;
    f32 unk1FC;
    f32 unk200;
    f32 unk204;
    u8 unk208[0x21C - 0x208];
    f32 unk21C;
    f32 unk220;
    f32 unk224;
    f32 unk228;
    f32 unk22C;
    f32 unk230;
    f32 unk234;
    f32 unk238;
    u8 unk23C[0x24C - 0x23C];
    u32 unk24C;
    u16 unk250;
    u16 unk252;
    u16 unk254;
    u16 unk256;
    u16 unk258;
};

} // namespace cf

extern "C" cf::CfCamFollow* __ct__cf_CfCamFollow(cf::CfCamFollow* self, void* arg1, void* arg2);
