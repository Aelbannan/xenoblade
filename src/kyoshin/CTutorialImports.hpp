#pragma once

// Unit-private import declarations split out of CTutorial.hpp so the class header
// stays includable next to other headers.
#include "kyoshin/CTutorial.hpp"


// Layout draw helper (retail reloc is the mangled
// drawLayout__FPQ34nw4r3lyt6LayoutPQ34nw4r3lyt8DrawInfoii).
void drawLayout(nw4r::lyt::Layout*, nw4r::lyt::DrawInfo*, int, int);
// Fire a per-window sound/effect event by id (retail reloc
// playUISound__FUl; declared with C++ linkage so MWCC mangles it).
void playUISound(u32);

// Shared loaded-BDAT pointer used by the tutorial data loader.
extern u32 lbl_eu_80664A30;

// Font/sound helpers (retail reloc getPackedFont__Fv; C++ linkage so MWCC
// mangles the free function).
u32 getPackedFont();

// Remaining helpers from the MenuStateInitFlags translation unit (same signatures
// as code_80135FDC.hpp so the mangled/unmangled reloc names are unchanged).
u32 advanceAnimTransform(nw4r::lyt::AnimTransform*, float);
extern "C" u32 AnimRewindFrame(nw4r::lyt::AnimTransform*, float);
extern "C" void closeFileHandle__FPP11CFileHandle(CFileHandle**);
extern "C" void CtrlRemote_SetSharedBit(u32 destination, u32 value);
void releaseArcResourceAccessor(nw4r::lyt::ArcResourceAccessor*);

// Unmangled retail callees - C ABI. The retail reloc names for these are the
// literal unmangled identifiers, so they must be declared with C linkage.
extern "C" void* Bdat_GetTable_AA34();


// Layout build helpers (retail relocs are the mangled C++ names).
void buildLayout(nw4r::lyt::Layout**, nw4r::lyt::ArcResourceAccessor*,
                   const char*);
void bindLayoutAnimTransform(nw4r::lyt::Layout*, nw4r::lyt::AnimTransform**,
                   nw4r::lyt::ArcResourceAccessor*, char*);
void setLayoutTextBoxFont(nw4r::lyt::Layout*, char*, u32);
// Root-pane font bind (retail reloc is the literal unmangled name).
extern "C" void ApplyFontToTxtPanes(void*, u32);
// BDAT field reader (retail reloc is the literal unmangled name).
extern "C" u8 BdatGetU8Direct(u32, const char*, u32);

// Retail calls the language-flag query through its unmangled Fv symbol with a
// dummy -1 argument that the callee ignores.
extern "C" int isClassicController__Q22cf13CfGameManagerFv(int arg);
