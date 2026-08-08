#pragma once

#include <types.h>

// sdata2 float constants referenced by this TU (retail labels)
extern const f32 lbl_eu_8066B360; // 0.0f - anim reset / off-diagonal
extern const f32 lbl_eu_8066B364; // 1.0f - diagonal scale
extern const f32 lbl_eu_8066B368; // speed threshold (close timer)
extern const f32 lbl_eu_8066B36C; // speed threshold (open timer)

// String constant: first-eye material name prefix
extern const char lbl_eu_80663C88[];

// Eye-blink animation state machine for a material pair (two eyes).
class CMdlAnmEye {
public:
    CMdlAnmEye();
    virtual ~CMdlAnmEye();

    /* 0x04 */ void* field_04;      // Owner model object (CScnItemModelNw4r)
    /* 0x08 */ u32 field_08[2];     // Material ids (field_08[0]=left, [1]=right)
    /* 0x10 */ u32 field_10;
    /* 0x14 */ u32 field_14;
    /* 0x18 */ u8 field_18[4];      // Per-material flag (0 = left eye, 1 = right eye)
    /* 0x1C */ u32 field_1C;        // Number of eye materials registered
    /* 0x20 */ u32 field_20;        // Blink state machine (0=closed,1=opening,2=...)
    /* 0x24 */ u32 field_24;        // Blink timer counter
    /* 0x28 */ f32 field_28;        // Current anim offset
    /* 0x2C */ u32 value2C;         // Global state
};