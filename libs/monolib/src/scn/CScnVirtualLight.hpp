#pragma once

#include <types.h>

struct CScnVirtualLightData {
    u32 value00;
    u8 _04[4];
    u32 value08;
    u8 _0C[8];
    f32 value14;
    u8 _18[0x14];
    u32 value2C;
    u8 _30[4];
    f32 value34;
    f32 value38;
    f32 value3C;
    f32 value40;
    f32 value44;
    f32 value48;
    f32 value4C;
    u8 _50[0x14];
    u32 value64;
    u32 value68;
    u8 _6C[0x64];
    f32 valueD0;
    f32 valueD4;
    u8 enabled;
    u8 _D9[0xA7];
    u32 value480;
};

class CScnVirtualLight {
public:
    CScnVirtualLight();
    virtual ~CScnVirtualLight();

    u32 value00;
    u32 value08;
    u8 _0C[0x08];
    f32 value14;
    u8 _18[0x1C];
    f32 value34;
    f32 value38;
    f32 value3C;
    f32 value40;
    f32 value44;
    f32 value48;
    f32 value4C;
    u8 _50[0x14];
    u32 value64;
    u8 _68[0x5C];
    u32 valueC4;
    u8 _C8[0x0C];
    f32 valueD4;
    u8 enabled;
    u8 _D9[0xA7];
    u32 value480;
};

class CLight {
public:
    virtual ~CLight();
};

class CVirtualLightObj {
public:
    virtual ~CVirtualLightObj();
};
