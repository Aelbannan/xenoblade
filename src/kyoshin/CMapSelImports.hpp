#pragma once

// TU-private retail imports and layout views for CMapSel.cpp (and the
// CMenuMapSelect implementation in code_802405F4.cpp). Kept out of
// kyoshin/CMapSel.hpp so the class header can be included next to
// kyoshin/CFloorMap.hpp.

#include "kyoshin/CMapSel.hpp"
#include "kyoshin/code_80135FDC_Bdat.hpp"
#include "kyoshin/code_80135FDC_Lyt.hpp"
#include "kyoshin/CQuestWindow_Pane.hpp"
#include "kyoshin/code_80135FDC_State.hpp"
#include "kyoshin/CUICfManager_Res.hpp"

// Scrollbar destroy / layout-read helpers (unmangled retail symbols).

// C-linkage imports (retail symbol names - keep linkage/signatures verbatim)
extern "C" u32 Bdat_GetMaxRow_B1EC(void*);
extern "C" u32 CtrlRemote_TouchBitByArg(u32);
extern "C" void Cur_DrawLayout(CBaseCur*, nw4r::lyt::DrawInfo*);
// Cursor / scrollbar / layout C-ABI helpers (unmangled retail symbols).
extern "C" void Cur_SetVisible(void*, int);
extern "C" void Cur_Update(void*);                  // cursor per-frame update
// Layout/text C-ABI helpers normally declared by code_80135FDC.hpp. CMapSel.cpp
// defines CODE_80135FDC_CPP before including that header, so those exports are
// skipped there and the declarations below apply (retail CMapSel callers pass
// the texture resource as PaneSetTexPaletteByName's third argument). Same guard as the legacy
// AnimRewindFrame pseudo-import so code_802405F4.cpp's define stays a no-op.
#ifndef KYOSHIN_SKIP_CMAPSEL_LEGACY_LAYOUT_IMPORTS
#endif
// Cursor position helper: fill output with the point derived from two panes.
// Grid-text helpers / cursor construction (C-ABI retail symbols).
extern "C" int sprintf(char*, const char*, ...);
// Map-select string pool (split1 .rodata): pane-name format at +0x1F, fixed
// pane name at +0x88.
extern "C" char lbl_eu_8050B4A8[];
// BDAT map-table pointers (.sbss, sda21-relative).
extern void* lbl_eu_806640A0;
extern void* lbl_eu_806640A8;
// Current map id (.sbss) - initial grid selection target.
extern u32 lbl_eu_80664184;
extern "C" float lbl_eu_80668738;
extern "C" float lbl_eu_80668740;
extern "C" float lbl_eu_80668744;
// Canonical AnimRewindFrame (nw4r::lyt::AnimTransform*, float) is declared in
// code_80135FDC.hpp. CMapSel.cpp includes that header and passes proper
// AnimTransform* pointers; the guard below stays for TUs that include both
// headers (identical redeclarations are harmless, but keep the old macro
// contract so code_802405F4.cpp's define stays a no-op).
#ifndef KYOSHIN_SKIP_CMAPSEL_LEGACY_LAYOUT_IMPORTS
#endif
extern "C" const float lbl_eu_8066873C;
