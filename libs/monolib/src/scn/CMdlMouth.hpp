#pragma once

#include <types.h>

// Shared .sdata2 constants referenced by CMdlMouth (retail linker names).
extern const f32 lbl_eu_8066B310; // reset mouth-open angle (1.0f)
extern const f32 lbl_eu_8066B314; // startup mouth-open scalar
extern const f32 lbl_eu_8066B318; // random-range base
extern const f32 lbl_eu_8066B31C; // random-range multiplier (0x14)
extern const f32 lbl_eu_8066B320; // random-range multiplier (0x18)
extern const f32 lbl_eu_8066B330; // angle -> sin index scale
extern const f32 lbl_eu_8066B334; // rotate delta scale
extern const f32 lbl_eu_8066B338; // final-angle -> fctiwz scale
// Owner context referenced by CMdlMouth::field_0x04.
struct CMdlOwnerCtx {
    void* field_0x00; // at 0x0
    void* field_0x04; // at 0x4 (view passed to func_80496288)
};

extern const f32 lbl_eu_8066A1F8; // pi

// Resource/assert string constants referenced by CMdlMouth::func_804E65CC
// (retail C-linkage names; global-scope names are not mangled by MWCC).
extern const char lbl_eu_80663C58[]; // node name passed to ResMdl::GetResNode
extern const char lbl_eu_80663C5C[]; // material-name needle for strstr
extern const char lbl_eu_8056E1C8[]; // Panic file (GetResNode assert)
extern const char lbl_eu_8056E1A8[]; // Panic fmt
extern const char lbl_eu_8056E194[]; // Panic file (node-null assert)
extern const char lbl_eu_8056E178[]; // Panic fmt
extern const char lbl_eu_80663910[]; // Panic arg
extern const char lbl_eu_80663C6C[]; // Panic arg
extern const char lbl_eu_8056E068[]; // Panic file (material assert)
extern const char lbl_eu_8056E04C[]; // Panic fmt
extern const char lbl_eu_806638E8[]; // Panic arg
extern const char lbl_eu_80663C68[]; // Panic arg

class CMdlMouth {
public:
    CMdlMouth();
    virtual ~CMdlMouth();

    void func_804E679C(int, int);
    int  func_804E68A0(int, void*);
    void func_804E6A28();

    /* 0x04 */ void* field_0x04;    // owner context (see CMdlOwnerCtx)
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