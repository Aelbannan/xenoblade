#pragma once

#include "monolib/device/CFileHandle.hpp"
#include "monolib/work/IWorkEvent.hpp"

#include "revolution/gx/GXTypes.h"

#include <nw4r/lyt.h>

// All definitions in code_80135FDC.cpp are C-linkage (extern "C"), so the
// declarations MUST match — otherwise callers emit Itanium-mangled relocs
// (e.g. func_8013639C__FPvPcUs) while retail has the unmangled name, and the
// witness rejects the pair at the reloc gate (us-8025658c / us-8025650c were
// stuck at 99.4-99.6% on exactly this).
// Params use const void* so both const char* (CArtsInfo, CExchangeWin) and
// const void* (CCollepedia) call sites bind without conversions.
extern "C" {
u16 func_8013606C(const void*, const void*, u16);
char* func_80136190(const void*, const void*, int);
u32 func_801361E8(const void*, const char*, u32);
char* func_8013639C(const void*, const void*, int);
void func_8013676C(nw4r::lyt::Pane*, u32);
void func_801368C0(nw4r::lyt::Layout*, char*, u32);
void func_80136910(nw4r::lyt::Layout*, char*, u8);
void func_80136A1C(nw4r::lyt::Layout*, char*, char*, u32);
void func_80136B4C(nw4r::lyt::Layout*, char*, char*, u32);
void func_80136E84(nw4r::lyt::Layout**, nw4r::lyt::ArcResourceAccessor*, const char*);
void func_80136F08(nw4r::lyt::Layout*, nw4r::lyt::AnimTransform**, nw4r::lyt::ArcResourceAccessor*, char*);
}

// Retail symbols that ARE the mangled C++ names stay C++-linkage.
void func_80137038__FPQ34nw4r3lyt6LayoutPQ34nw4r3lyt8DrawInfoii(nw4r::lyt::Layout*, nw4r::lyt::DrawInfo*, int, int);
void func_80137250(nw4r::lyt::DrawInfo* pDrawInfo);
u32 func_80137444(nw4r::lyt::AnimTransform*, float);
void func_801390E0(CFileHandle**);
void func_80139124(nw4r::lyt::ArcResourceAccessor*);

// Retail symbols are unmangled (C linkage) — keep them extern "C" so calls
// bind to the unmangled name, not an Itanium-mangled re-derivation.
extern "C" {
u8 func_801372B4(u32);
u32 func_80137510(nw4r::lyt::AnimTransform*, float);
void func_80137E7C(nw4r::lyt::Layout*, char*, void*);
char* func_80138F78(u16);
void func_80139A18(nw4r::lyt::Layout*, char*, GXColorS10*, GXColorS10*);
}
extern "C" u8 code80135FDC_getByte_621F0();
extern "C" u8 code80135FDC_getByte_64077();
extern "C" u8 func_801392B4(u8);
