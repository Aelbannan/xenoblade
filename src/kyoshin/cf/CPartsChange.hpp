#pragma once

#include <types.h>

namespace cf {

class CPartsChange {
public:
    virtual ~CPartsChange();

    /* 0x04 */ u8 mField04;
    /* 0x08 */ u32 mField08;
    /* 0x0C */ u8 mData[0x20];
    /* 0x2C */ u8 mField2C;
};

// Struct filled by func_80198710 (likely CfPartyInfo)
struct CfPartyInfo {
    /* 0x00 */ u32 field_00;
    /* 0x04 */ u32 field_04;
    /* 0x08 */ u32 field_08;
    /* 0x0C */ s32 field_0C;
    /* 0x10 */ u32 field_10;
    /* 0x14 */ s32 field_14;
    /* 0x18 */ f32 field_18;
    /* 0x1C */ f32 field_1C;
    /* 0x20 */ f32 field_20;
    /* 0x24 */ u32 field_24;
    /* 0x28 */ f32 field_28;
    /* 0x2C */ u8 field_2C;
    /* 0x2D */ u8 field_2D;
    /* 0x2E */ u8 field_2E;

    void func_80195AFC(u8 val);
    void func_80198710(void* r4, float f1, int r5, int r6, float f2, float f3);
};

// Comparator key struct for func_8019641C
struct CfPartyInfoSortKey {
    /* 0x00 */ u32 field_00;
    /* 0x04 */ f32 sortKey;
};

// Wrapper for CActorParam fields accessed by CPartsChange accessors
struct CfActorAccessors {
    u8 pad_00[0x1E];
    /* 0x1E */ u16 mFlags1E;
    u8 pad_20[0x10];
    /* 0x30 */ void* mField30;
    u8 pad_34[0x58];
    /* 0x8C */ f32 mField8C;
    u8 pad_90[4];
    /* 0x94 */ u32 mField94;
    u8 pad_98[6];
    /* 0x9E */ u16 mField9E;

    void func_80193A88(int enable);
    u32 func_80193B04();
    u16 func_80193CC8();
    u32 func_80195284();
    u32 func_80195AC0();
    void func_80195ACC(float val);
    void* func_80195AEC();
};

// Wrapper for CfObjectPc fields at 0x45C4+
struct CfObjectPcExt {
    u8 pad[0x45C4];
    /* 0x45C4 */ u16 mField45C4;
    /* 0x45C6 */ u16 mField45C6;
    /* 0x45C8 */ u16 mField45C8;

    void func_80195AD4(u16 val);
    void func_80195ADC(u16 val);
    u16 func_80195AE4();
    void* func_80195AF4();
};

} // namespace cf
