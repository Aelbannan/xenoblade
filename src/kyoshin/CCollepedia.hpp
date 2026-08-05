#pragma once

#include <types.h>
#include "nw4r/lyt/lyt_layout.h"
#include "nw4r/lyt/lyt_animation.h"
#include "kyoshin/CSysWin.hpp"

// Full `this` struct for harness decomp of CCollepedia.cpp
// Entry within the sub-block array (stride 0xA)
struct CCollepediaEntry {
    u8 _00[0x08];
    u16 field_08; // u16 at offset +0x08 within entry
};

// Sub-range of the CCollepedia class from +0x00 to +0xE8
struct CCollepediaFull {
    u8 _00[0xD8];
    u8 field_D8;
    u8 field_D9;
    u8 _DA[0xE8 - 0xDA];
    u8 field_E8_start; // sub-array starts at +0xE8
};

// CCollepedia is a non-virtual class in the decomp (layout is flat, no vtable ptr).
// Virtual destructor semantics are handled by __dt__<addr> thunks.
// The vtable pointer is stored at +0x00 as a regular field.
struct CCollepedia {
    ~CCollepedia();
    /* 0x00 */ int field_0;                         // vtable ptr (set by ctor)
    /* 0x04 */ int field_4;
    /* 0x08 */ nw4r::lyt::Layout* mpLayout1;        // first layout object
    /* 0x0C */ nw4r::lyt::AnimTransform* mpAnim1;   // first anim transform
    /* 0x10 */ nw4r::lyt::AnimTransform* mpAnim2;   // second anim transform
    /* 0x14 */ u8 _14[0x38 - 0x14];
    /* 0x38 */ nw4r::lyt::Layout* mpLayout2;        // second layout object
    /* 0x3C */ nw4r::lyt::AnimTransform* mpAnim3;   // third anim transform
    /* 0x40 */ nw4r::lyt::AnimTransform* mpAnim4;   // fourth anim transform
    /* 0x44 */ nw4r::lyt::AnimTransform* mpAnim5;   // fifth anim transform
    /* 0x48 */ u8 field_48;
    /* 0x49 */ u8 field_49;
    /* 0x4A */ u8 _4A[0x50 - 0x4A];
    /* 0x50 */ u8 field_50;
    /* 0x51 */ u8 _51[0x54 - 0x51];
    /* 0x54 */ u8 field_54;       // CBaseCur sub-object starts here
    /* 0x55 */ u8 _55[0x6C - 0x55];
    /* 0x6C */ u8 field_6C;       // sub-object
    /* 0x6D */ u8 _6D[0x84 - 0x6D];
    /* 0x84 */ u8 field_84;       // sub-object
    /* 0x85 */ u8 _85[0x9C - 0x85];
    /* 0x9C */ u8 field_9C;       // CSysWin sub-object starts here
    /* 0x9D */ u8 _9D[0xD8 - 0x9D];
    /* 0xD8 */ u8 field_D8;
    /* 0xD9 */ u8 field_D9;
    /* 0xDA */ u8 _DA[0xE8 - 0xDA];
    /* 0xE8 */ u8 field_E8;       // sub-array starts here
    /* 0xE9 */ u8 _E9[0x28F0 - 0xE9];
    /* 0x28F0 */ nw4r::lyt::Layout* mpLayout3;     // third layout object
    /* 0x28F4 */ u8 _28F4[0x28FA - 0x28F4];
    /* 0x28FA */ u8 field_28FA;
};