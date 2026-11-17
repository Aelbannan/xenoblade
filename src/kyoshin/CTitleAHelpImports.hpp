#pragma once

// Unit-private import declarations split out of CTitleAHelp.hpp so the class header
// stays includable next to other headers.
#include "kyoshin/CTitleAHelp.hpp"
#include "kyoshin/CUICfManager_Res.hpp"


// View into the object returned by CDeviceFont::getFontInfo: vtable+0x24
// (slot 9, no args) yields the u32 font handle bound by ApplyFontToTxtPanes.
// All-pure so no vtable is emitted.
class CTitleAHelpFontView {
public:
    virtual void vf0() = 0; // +0x08
    virtual void vf1() = 0; // +0x0C
    virtual void vf2() = 0; // +0x10
    virtual void vf3() = 0; // +0x14
    virtual void vf4() = 0; // +0x18
    virtual void vf5() = 0; // +0x1C
    virtual void vf6() = 0; // +0x20
    virtual u32 vf7() = 0;  // +0x24
};

bool isPaneVisible(nw4r::lyt::Pane*);

// One row of the title-A help table: 7 u16 cells indexed by button id.
struct HelpRow {
    u16 cell[7];
};

extern HelpRow lbl_eu_80533E60[];

// Global (retail-unmangled) helper returning the UI layout resource accessor.

// Retail-unmangled CfGameManager static helper (takes a selector mask).
extern "C" int isClassicController__Q22cf13CfGameManagerFv(int selector);

// Vector copy helper (defined in CTagProcessor.cpp).
extern "C" void copyVEC3(void* dst, const void* src);
