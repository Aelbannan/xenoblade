#pragma once

/*
 * CBaseCur.hpp
 * Base class for cursor/layout overlay widgets.
 *
 * Manages a nw4r layout with animation transforms and visibility state.
 * Derived classes (CCur07..CCur22, CSubCur) each load a specific layout.
 *
 * Layout (size 0x16):
 *   +0x00: vtable pointer (set explicitly by constructor / derived classes)
 *   +0x04: arc resource accessor (passed at construction)
 *   +0x08: nw4r layout pointer (loaded by derived Init methods)
 *   +0x0C: animation transform 0
 *   +0x10: animation transform 1
 *   +0x14: mActive - active flag (0=idle, 1=running, 2=fading)
 *   +0x15: mVisible - visibility flag (0=hidden, 1=visible)
 *
 * Virtual methods (in vtable at runtime, not defined in this TU):
 *   vtable[0]: VUpdate() - pure virtual in base (slot = 0x00000000)
 *   vtable[1]: func_801D20DC()
 *   vtable[2]: func_801D2144()
 *   vtable[3]: func_801D2180()
 */

#include <types.h>
#include <nw4r/lyt.h>

// Vtable symbol for CBaseCur (6 entries, .data)
extern "C" void* lbl_eu_805349A0[];

class CBaseCur {
public:
    // Explicit vtable pointer (not relying on MWCC implicit generation)
    void* mVtable;                               // +0x00
    nw4r::lyt::ArcResourceAccessor* mArcResAcc;  // +0x04
    nw4r::lyt::Layout* mpLayout;                  // +0x08
    nw4r::lyt::AnimTransform* mpAnimTrans0;       // +0x0C
    nw4r::lyt::AnimTransform* mpAnimTrans1;       // +0x10
    u8 mActive;                                    // +0x14
    u8 mVisible;                                   // +0x15
};
