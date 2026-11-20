#pragma once

#include "kyoshin/CBgTex.hpp"

// Retail ABI names for the CBgTex helpers defined in kyoshin/CBgTex.cpp.
// US/EU strip the C++ member manglings from these symbols, so every caller
// binds them through these canonical C-linkage declarations instead of a
// per-TU copy. Return types follow the definitions (bool/void) and the
// load-query follows its retail cmpwi call sites (int).
extern "C" bool BgTex_Acquire_3C14(CBgTex* self);
extern "C" void BgTex_Tick_3D54(CBgTex* self);
extern "C" void BgTex_Draw_3D7C(CBgTex* self, nw4r::lyt::DrawInfo* drawInfo);
extern "C" void BgTex_Release_3D9C(CBgTex* self);
extern "C" int BgTex_IsLoaded_3E34(CBgTex* self);
