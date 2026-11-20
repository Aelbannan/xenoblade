#ifndef NW4R_G3D_RES_G3D_RESUSERDATA_H
#define NW4R_G3D_RES_G3D_RESUSERDATA_H

#include <types.h>

// nw4r g3d ResUserData layout (g3d_resuser_ac.h accessor semantics).
// One named item: name string, value type, and relative offsets.
struct ResUserDataItem {
    u32 field_0x00;      // +0x00
    s32 dataOffset;      // +0x04: relative offset to the value (0 = null)
    u32 field_0x08;      // +0x08
    u32 valueType;       // +0x0C: 0 = S32 (asserted)
    s32 nameOffset;      // +0x10: relative offset to the name string
};

// A single 0x10-byte reference slot in the dic index array.
struct ResUserDataRef {
    s32 dataOffset;      // +0x00: relative offset (from the dic base) to ResUserDataItem
    u32 field_0x04;
    u32 field_0x08;
    u32 field_0x0C;
};

// Dic accessor base: sits 4 bytes into the user-data block (ud + 4).
// numData at +0x04, index array (0x10-byte refs) at +0x24.
struct ResUserDataDic {
    u32 field_0x00;
    u32 numData;             // +0x04: number of reference slots
    u32 field_0x08;
    u32 field_0x0C;
    u32 field_0x10;
    u32 field_0x14;
    u32 field_0x18;
    u32 field_0x1C;
    u32 field_0x20;
    ResUserDataRef refs[1];  // +0x24
};

#endif // NW4R_G3D_RES_G3D_RESUSERDATA_H
