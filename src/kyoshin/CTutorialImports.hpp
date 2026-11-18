#pragma once

// Unit-private import declarations split out of CTutorial.hpp so the class header
// stays includable next to other headers.
#include "kyoshin/CTutorial.hpp"
#include "kyoshin/code_80135FDC_Bdat.hpp"
#include "kyoshin/code_80135FDC_Lyt.hpp"
#include "kyoshin/code_80135FDC_State.hpp"
#include "kyoshin/CUICfManager_Res.hpp"


// Layout draw helper (retail reloc is the mangled
// drawLayout__FPQ34nw4r3lyt6LayoutPQ34nw4r3lyt8DrawInfoii).
// Fire a per-window sound/effect event by id (retail reloc
// playUISound__FUl; declared with C++ linkage so MWCC mangles it).

// Shared loaded-BDAT pointer used by the tutorial data loader.
extern u32 lbl_eu_80664A30;

// Font/sound helpers (retail reloc getPackedFont__Fv; C++ linkage so MWCC
// mangles the free function).

// Remaining helpers from the MenuStateInitFlags translation unit (same signatures
// as code_80135FDC.hpp so the mangled/unmangled reloc names are unchanged).
extern "C" void CtrlRemote_SetSharedBit(u32 destination, u32 value);

// Unmangled retail callees - C ABI. The retail reloc names for these are the
// literal unmangled identifiers, so they must be declared with C linkage.
extern "C" void* Bdat_GetTable_AA34();


// Layout build helpers (retail relocs are the mangled C++ names).
// Root-pane font bind (retail reloc is the literal unmangled name).
// BDAT field reader (retail reloc is the literal unmangled name).

// Retail calls the language-flag query through its unmangled Fv symbol with a
// dummy -1 argument that the callee ignores.
