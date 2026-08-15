#pragma once

#include <types.h>

// sdata2 float constants referenced by this TU (retail labels)
extern const f32 lbl_eu_8066B360; // 0.0f - anim reset / off-diagonal
extern const f32 lbl_eu_8066B364; // 1.0f - diagonal scale
extern const f32 lbl_eu_8066B368; // speed threshold (close timer)
extern const f32 lbl_eu_8066B36C; // speed threshold (open timer)

// Eye-blink angle tables (3 entries each, indexed by field_20).
extern const f32 lbl_eu_805247E8[3]; // left eye: {0.0, +0.336f, +0.672f}
extern const f32 lbl_eu_805247F4[3]; // right eye: {0.0, -0.5f, -1.0f} (negated on use)

// .sdata pointer to the first-eye material-name prefix (loaded via r13).
extern const char* lbl_eu_80663C88;

// Resource/assert strings referenced by the Panic calls. Fixed sizes so MWCC
// picks the retail addressing (sda21 for the <=8-byte .sdata strings,
// absolute lis/addi for the .data file/fmt strings).
extern const char lbl_eu_80663C98[4]; // Panic arg ("ref", .sdata)
extern const char lbl_eu_8056E068[0x10]; // Panic file (.data)
extern const char lbl_eu_8056E04C[0x1A]; // Panic fmt (.data)
extern const char lbl_eu_806638E8[7]; // Panic arg (.sdata)

// C-ABI imports (retail symbols are unmangled func_* names; declared here so
// the call relocs keep the C names verbatim).
extern "C" const char* func_804E6C78();   // second-eye material-name prefix (CMdlAnmUV.cpp)
extern "C" f32 func_80484EB0(void* model); // owner-model scale query (CScnItemModel.cpp)

// Eye-blink animation state machine for a material pair (two eyes).
class CMdlAnmEye {
public:
    CMdlAnmEye();
    virtual ~CMdlAnmEye();

    /* 0x04 */ void* field_04;              // Owner model object (CScnItemModelNw4r)
    /* 0x08 */ u32 field_08[2];             // Material ids (field_08[0]=left, [1]=right)
    /* 0x10 */ u32 field_10;
    /* 0x14 */ u32 field_14;
    /* 0x18 */ u8 field_18[4];              // Per-material flag (0 = left eye, 1 = right eye)
    /* 0x1C */ s32 field_1C;                // Number of eye materials registered
    /* 0x20 */ s32 field_20;                // Blink state machine (0=closed,1=opening,2=...)
    /* 0x24 */ s32 field_24;                // Blink timer counter
    /* 0x28 */ f32 field_28;                // Current anim offset
    /* 0x2C */ s32 value2C;                 // Global state
};
