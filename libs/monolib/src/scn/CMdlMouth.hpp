#pragma once

#include <types.h>

namespace nw4r {
namespace g3d {
struct ChrAnmResult;
}
}

// Owner model object referenced by CMdlMouth::field_0x04. It is a
// CScnItemModelNw4r instance, viewed through the minimal typed layout defined
// in CMdlMouth.cpp. Forward declarations keep this header dependency-free.
struct CMdlOwnerCtx;
class CScnItemModelNw4r;
class CMdlMouth;

// Frame-table add + ascending bubble-sort for the owner model's frame table

extern "C" void scnImN4AddFrameId(CScnItemModelNw4r* ths, u32 value);

// Shared .sdata2 constants referenced by CMdlMouth (retail linker names).
// SRT offset table (.rodata, 3 entries), indexed by the mouth-open stage.
extern const f32 lbl_eu_805247B4[];


// .sdata pointers to the node/material-name strings (loaded via r13, SDA21).
extern const char* lbl_eu_80663C58; // node name passed to ResMdl::GetResNode
extern const char* lbl_eu_80663C5C; // material-name needle for strstr
// Resource/assert strings referenced by MdlMouthBindModel. Sized so MWCC picks the
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

// Mouth-anim helpers are C-linkage free functions in retail (unmangled names
// in config/*/symbols.txt); the defining CMdlMouth.cpp owns these one true
// declarations, and callers include this header.
extern "C" void MdlMouthBindModel(CMdlMouth* ths, CMdlOwnerCtx* model);
extern "C" void MdlMouthSetMode(CMdlMouth* ths, int mode, int arg3);
extern "C" void MdlMouthSetValue(CMdlMouth* ths, float val);
extern "C" int  MdlMouthPollAnim(CMdlMouth* ths, u32 nodeId,
                                 nw4r::g3d::ChrAnmResult* result);
extern "C" void MdlMouthTeardown(CMdlMouth* ths);

class CMdlMouth {
public:
    CMdlMouth();
    virtual ~CMdlMouth();

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