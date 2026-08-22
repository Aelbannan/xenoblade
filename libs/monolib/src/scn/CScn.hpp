#pragma once

#include <types.h>

// Local helpers shared by CScn.cpp matching targets.
// NOTE: this header is included by explicit path ("libs/monolib/src/scn/CScn.hpp"),
// separate from the public monolib/scn/CScn.hpp.

// Float constants block referenced by scene timing helpers (.sdata2).
extern f32 lbl_eu_8066AAB4;
extern f32 lbl_eu_8066AAB8;
extern f32 lbl_eu_8066AABC;

// Blob reached via CScn+0x84; holds per-scene floats at +0x08 / +0x0C.
struct ScnFloats {
    u8 unk00[0x08];
    float unk08; //0x08
    float unk0C; //0x0C
};
