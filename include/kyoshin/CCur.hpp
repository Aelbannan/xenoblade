#pragma once

#include <types.h>
#include <nw4r/lyt.h>

// ---------------------------------------------------------------------------
// C-linkage imports (retail symbol names - keep linkage/signatures verbatim)
// ---------------------------------------------------------------------------

// Pane-visibility helpers (retail C-linkage names; same convention as
// CSysWin.hpp / CKizunagram.hpp / CEquipItemBox.hpp).
extern "C" void func_80124270(nw4r::lyt::Pane*, u32);
extern "C" bool func_801C4648(nw4r::lyt::Pane*);
// Pane translate setter (defined in CCur.cpp; retail keeps C linkage).
// never_inline on the declaration so call sites keep the out-of-line bl.
extern "C" void func_801D2150(nw4r::lyt::Pane*, const nw4r::math::VEC3*)
    __attribute__((never_inline));

// Shared 0.0f constant in .sdata2 (defined in port/data_defs.cpp). Retail
// code loads it via lfs @sda21 instead of pooling a local literal.
extern f32 lbl_eu_80667FA4;

extern "C" void* lbl_eu_80534978[];  // CCur07
extern "C" void* lbl_eu_80534950[];  // CCur09
extern "C" void* lbl_eu_80534928[];  // CCur11
extern "C" void* lbl_eu_80534900[];  // CCur14
extern "C" void* lbl_eu_805348D8[];  // CCur15
extern "C" void* lbl_eu_805348B0[];  // CCur16
extern "C" void* lbl_eu_80534884[];  // CCur18
extern "C" void* lbl_eu_8053485C[];  // CCur22
extern "C" void* lbl_eu_80534838[];  // CSubCur
extern "C" const char* lbl_eu_80534828[];
extern "C" void __dl__FPv(void*);
