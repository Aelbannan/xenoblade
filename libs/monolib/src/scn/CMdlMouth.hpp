#pragma once

#include <types.h>

// Owner model object referenced by CMdlMouth::field_0x04. It is a
// CScnItemModelNw4r instance, viewed through the minimal typed layout defined
// in CMdlMouth.cpp. Forward declarations keep this header dependency-free.
struct CMdlOwnerCtx;
class CScnItemModelNw4r;

// Frame-table add + ascending bubble-sort for the owner model's frame table
// (defined in CScnItemModelNw4r.cpp). C++ linkage matches the definition's
// mangled symbol; the retail symbol is the unmangled func_* fragment.
void func_80488F44(CScnItemModelNw4r* self, u32 value);

// Shared .sdata2 constants referenced by CMdlMouth (retail linker names).
extern const f32 lbl_eu_8066B310; // reset mouth-open angle (1.0f)
extern const f32 lbl_eu_8066B314; // startup mouth-open scalar
extern const f32 lbl_eu_8066B318; // random-range base
extern const f32 lbl_eu_8066B31C; // random-range multiplier (0x14)
extern const f32 lbl_eu_8066B320; // random-range multiplier (0x18)
extern const f32 lbl_eu_8066B330; // angle -> sin index scale
extern const f32 lbl_eu_8066B334; // rotate delta scale
extern const f32 lbl_eu_8066B338; // final-angle -> fctiwz scale
// SRT offset table (.rodata, 3 entries), indexed by the mouth-open stage.
extern const f32 lbl_eu_805247B4[];

extern const f32 lbl_eu_8066A1F8; // pi

// .sdata pointers to the node/material-name strings (loaded via r13, SDA21).
extern const char* lbl_eu_80663C58; // node name passed to ResMdl::GetResNode
extern const char* lbl_eu_80663C5C; // material-name needle for strstr
// Resource/assert strings referenced by func_804E65CC. Sized so MWCC picks the
// retail addressing: sda21 for the <=8-byte .sdata args, absolute lis/addi for
// the .data file/fmt strings.
extern const char lbl_eu_8056E1C8[0x1D0]; // Panic file (GetResNode assert)
extern const char lbl_eu_8056E1A8[0x20];  // Panic fmt
extern const char lbl_eu_8056E194[0x11];  // Panic file (node-null assert)
extern const char lbl_eu_8056E178[0x1A];  // Panic fmt
extern const char lbl_eu_80663910[0x8];   // Panic arg (.sdata)
extern const char lbl_eu_80663C6C[0x4];   // Panic arg (.sdata)
extern const char lbl_eu_8056E068[0x10];  // Panic file (material assert)
extern const char lbl_eu_8056E04C[0x1A];  // Panic fmt
extern const char lbl_eu_806638E8[0x7];   // Panic arg (.sdata)
extern const char lbl_eu_80663C68[0x4];   // Panic arg (.sdata)

class CMdlMouth {
public:
    CMdlMouth();
    virtual ~CMdlMouth();

    void func_804E679C(int, int);
    int  func_804E68A0(int, void*);
    void func_804E6A28();

    /* 0x04 */ CMdlOwnerCtx* field_0x04;  // owner model (view in CMdlMouth.cpp)
    /* 0x08 */ s32   field_0x08;
    /* 0x0C */ s32   field_0x0C;    // node id
    /* 0x10 */ f32   field_0x10;    // mouth-open angle
    /* 0x14 */ f32   field_0x14;    // open speed X
    /* 0x18 */ f32   field_0x18;    // open speed Y
    /* 0x1C */ f32   field_0x1C;    // last computed open amount
    /* 0x20 */ f32   field_0x20;
    /* 0x24 */ s32   field_0x24;    // material index (-1 = none)
    /* 0x28 */ s32   field_0x28;    // close timer
    /* 0x2C */ f32   value2C;
};