#pragma once

#include <types.h>
#include "kyoshin/CSysWin.hpp"
#include "kyoshin/CBaseCur.hpp"

namespace nw4r {
namespace lyt {
    class Layout;
    class Pane;
    class AnimTransform;
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

// Cursor class derived from CBaseCur, used by CCollepedia for cursor display.
// Vtable at lbl_eu_80537474 (3 entries, size 0x18 with padding).
class CCLPCur : public CBaseCur {
public:
    CCLPCur(nw4r::lyt::ArcResourceAccessor* pAccessor);
};

// CCollepedia is a non-virtual class in the decomp (layout is flat, no vtable ptr).
// Virtual destructor semantics are handled by __dt__<addr> thunks.
// The vtable pointer is stored at +0x00 as a regular field.
// CCollepedia is a non-virtual class in the decomp (layout is flat, no vtable ptr).
// Virtual destructor semantics are handled by __dt__<addr> thunks.
// The vtable pointer is stored at +0x00 as a regular field.
//
// NOTE: offset 0x0C is used as both a 4-byte state machine (via individual byte
// stores in state functions) and as a pointer in some rendering functions.
// The struct exposes it as a pointer; state functions access individual bytes
// via reinterpret_cast.
struct CCollepedia {
    /* 0x00 */ int field_0; // vtable ptr (set by ctor)
    /* 0x04 */ nw4r::lyt::Layout* field_4;
    /* 0x08 */ nw4r::lyt::Layout* field_8; // used as Layout* in rendering, AnimTransform* in state checks
    /* 0x0C */ nw4r::lyt::AnimTransform* field_c_ptr; // state machine (low bytes) or AnimTransform*
    /* 0x10 */ nw4r::lyt::AnimTransform* field_10_ptr;
    /* 0x14 */ u8 _14[0x34 - 0x14];
    /* 0x34 */ void* field_34;
    /* 0x38 */ nw4r::lyt::Layout* field_38;
    /* 0x3C */ nw4r::lyt::AnimTransform* field_3C;
    /* 0x40 */ nw4r::lyt::AnimTransform* field_40; // AnimTransform* at 0x40
    /* 0x44 */ nw4r::lyt::AnimTransform* field_44; // AnimTransform* at 0x44
    /* 0x48 */ u8 field_48;
    /* 0x49 */ u8 field_49;
    /* 0x4A */ u8 _4A[0x4C - 0x4A];
    /* 0x4C */ u32 field_4C;
    /* 0x50 */ u8 field_50;
    /* 0x51 */ u8 field_51;
    /* 0x52 */ u8 _52[0x54 - 0x52];
    /* 0x54 */ u8 field_54[0x9C - 0x54]; // sub-object at 0x54 (used by func_801D216C)
    /* 0x9C */ u8 field_9C; // CSysWin sub-object starts here
    /* 0x9D */ u8 _9D[0xD8 - 0x9D];
    /* 0xD8 */ u8 field_D8;
    /* 0xD9 */ u8 field_D9;
    /* 0xDA */ u8 field_DA;
    /* 0xDB */ u8 _DB[0xE8 - 0xDB];
    /* 0xE8 */ u8 field_E8; // sub-array starts here
    /* 0xE9 */ u8 _E9[0x28F0 - 0xE9];
    /* 0x28F0 */ nw4r::lyt::Layout* field_28F0; // Layout* at 0x28F0 (used by func_80254B64)
    /* 0x28F4 */ u8 _28F4[0x28F9 - 0x28F4];
    /* 0x28F9 */ u8 field_28F9;
    /* 0x28FA */ u8 field_28FA;

    ~CCollepedia();
};