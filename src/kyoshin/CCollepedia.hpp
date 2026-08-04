#pragma once

#include <types.h>
#include "kyoshin/CSysWin.hpp"

namespace nw4r {
namespace lyt {
    class Layout;
    class Pane;
}
}

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

// Container with a layout pointer at +0x04 (used by func_80253970)
struct LayoutContainer {
    u8 _00[0x04];
    nw4r::lyt::Layout* mpLayout; // at +0x04
};

// CCollepedia is a non-virtual class in the decomp (layout is flat, no vtable ptr).
// Virtual destructor semantics are handled by __dt__<addr> thunks.
// The vtable pointer is stored at +0x00 as a regular field.
struct CCollepedia {
    // TODO: add fields
    /* 0x00 */ int field_0; // vtable ptr (set by ctor)
    /* 0x04 */ int field_4;
    /* 0x08 */ int field_8;
    /* 0x0C */ u8 field_c;
    /* 0x0D */ u8 field_d;
    /* 0x0E */ u8 field_e;
    /* 0x0F */ u8 _0F[0x49 - 0x0F];
    /* 0x49 */ u8 field_49;
    /* 0x4A */ u8 _4A[0x50 - 0x4A];
    /* 0x50 */ u8 field_50;
    /* 0x51 */ u8 _51[0x9C - 0x51];
    /* 0x9C */ u8 field_9C; // CSysWin sub-object starts here
    /* 0x9D */ u8 _9D[0xD8 - 0x9D];
    /* 0xD8 */ u8 field_D8;
    /* 0xD9 */ u8 field_D9;
    /* 0xDA */ u8 _DA[0xE8 - 0xDA];
    /* 0xE8 */ u8 field_E8; // sub-array starts here
    /* 0xE9 */ u8 _E9[0x28FA - 0xE9];
    /* 0x28FA */ u8 field_28FA;
};