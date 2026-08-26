#pragma once

#include <types.h>

class CScnEnvLgtCtrl;  // full definition in CScnEnvLgtCtrl.hpp

// A word-packed 4-component vector (color/parameters) read by func_8049DE74
// as raw 32-bit words.
struct SWordVec {
    u32 v0;
    u32 v1;
    u32 v2;
    u32 v3;
};

// Scene-view object behind CScnFogMan+0x04: func_8048ECD8 resolves the owning
// nw4r::g3d::ScnRoot from it, and +0x7C holds the fog-controller pointer
// consumed by fog mode 2.
struct SFogRootView {
    u8 pad_0x00[0x7c];
    CScnEnvLgtCtrl* field_0x7c;
};

class CScnFogMan {
public:
    virtual ~CScnFogMan();

    union {                           // 0x4 scene-view object
        u32 field_0x04;               // raw view (ctor param)
        SFogRootView* rootView;       // typed view
    };
    s32 value08;      // 0x8  (fog mode)
    f32 field_0xC;    // 0xc
    f32 field_0x10;   // 0x10
    f32 field_0x14;   // 0x14
    f32 field_0x18;   // 0x18
    f32 field_0x1c;   // 0x1c
    f32 field_0x20;   // 0x20
    u32 field_0x24;   // 0x24
    u8 field_0x28;    // 0x28
    u32 field_0x2c;   // 0x2c
    f32 field_0x30;   // 0x30
    f32 field_0x34;   // 0x34
    f32 field_0x38;   // 0x38
    f32 field_0x3c;   // 0x3c
    f32 field_0x40;   // 0x40
    f32 field_0x44;   // 0x44
    f32 field_0x48;   // 0x48
    f32 field_0x4c;   // 0x4c
    u32 field_0x50;   // 0x50
    f32 field_0x54;   // 0x54
    f32 field_0x58;   // 0x58
    f32 field_0x5c;   // 0x5c
    f32 field_0x60;   // 0x60
    f32 field_0x64;   // 0x64
    f32 field_0x68;   // 0x68
    f32 field_0x6c;   // 0x6c
    f32 field_0x70;   // 0x70
};

// Cross-TU import (definition in CScnEnvLgtCtrl.cpp): set +0x00 bit 0x20 /
// clear bit 0x10 on the environment-light controller, then walk its
// light-object ring applying the fog-manager state.
void func_804C1674(CScnEnvLgtCtrl* self);
