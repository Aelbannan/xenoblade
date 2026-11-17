#pragma once

// TU-private retail imports and layout views for CMapSel.cpp (and the
// CMenuMapSelect implementation in code_802405F4.cpp). Kept out of
// kyoshin/CMapSel.hpp so the class header can be included next to
// kyoshin/CFloorMap.hpp.

#include "kyoshin/CMapSel.hpp"

// View into the object returned by CDeviceFont::getFontInfo: vtable+0x24
// (index 7, no args) yields the u32 passed to ApplyFontToTxtPanes. All-pure so no
// vtable is emitted.
class CDeviceFontView {
public:
    virtual void vf0() = 0; // index 0 -> +0x08
    virtual void vf1() = 0; // index 1 -> +0x0C
    virtual void vf2() = 0; // index 2 -> +0x10
    virtual void vf3() = 0; // index 3 -> +0x14
    virtual void vf4() = 0; // index 4 -> +0x18
    virtual void vf5() = 0; // index 5 -> +0x1C
    virtual void vf6() = 0; // index 6 -> +0x20
    virtual u32 vf7() = 0;  // index 7 -> +0x24
};

// Opaque object whose first user virtual (vtable offset 0x08 after the RTTI
// prefix slots) acts like a deleting destructor taking an int flag. Abstract
// (all-pure) so MWCC emits no vtable for the view.
class CMapSelLayoutObj {
public:
    virtual void vfRelease(int) = 0;
};

// Scrollbar destroy / layout-read helpers (unmangled retail symbols).

// C-linkage imports (retail symbol names - keep linkage/signatures verbatim)
extern "C" void __dt__10CScrollBarFv(void*, int);
extern "C" void __dt__17UnkClass_8045F564Fv(UnkClass_8045F564*, int);
void closeFileHandle(CFileHandle**);
extern "C" void __ct__10CScrollBarFUc(void*, int);
extern "C" u8 BdatGetU8Direct(u32, const char*, u32);
extern "C" u32 Bdat_GetMaxRow_B1EC(void*);
extern "C" u32 CtrlRemote_TouchBitByArg(u32);
extern "C" void Cur_DrawLayout(CBaseCur*, nw4r::lyt::DrawInfo*);
// Cursor / scrollbar / layout C-ABI helpers (unmangled retail symbols).
extern "C" void Cur_Update(void*);                  // cursor per-frame update
// Layout/text C-ABI helpers normally declared by code_80135FDC.hpp. CMapSel.cpp
// defines CODE_80135FDC_CPP before including that header, so those exports are
// skipped there and the declarations below apply (retail CMapSel callers pass
// the texture resource as PaneSetTexPaletteByName's third argument). Same guard as the legacy
// AnimRewindFrame pseudo-import so code_802405F4.cpp's define stays a no-op.
#ifndef KYOSHIN_SKIP_CMAPSEL_LEGACY_LAYOUT_IMPORTS
extern "C" char* BdatGetPtrDirect(const void*, const void*, int);
extern "C" void ApplyFontToTxtPanes(nw4r::lyt::Pane*, u32);
extern "C" void PaneSetTexPaletteByName(void*, const char*, const void*);
extern "C" char* MakeTplNameSysFile(u32);
#endif
// Cursor position helper: fill output with the point derived from two panes.
extern "C" void ScalePaneWorldPos(nw4r::math::VEC3*, nw4r::lyt::Pane*,
                               nw4r::lyt::Pane*, nw4r::lyt::Pane*);
// Grid-text helpers / cursor construction (C-ABI retail symbols).
extern "C" u16 BdatGetU16Direct(const void*, const void*, int);
extern "C" void setPaneVisible(void*, u32);
extern "C" nw4r::lyt::ArcResourceAccessor* CUICfManager_getArcResourceAccessor();
extern "C" int sprintf(char*, const char*, ...);
// Map-select string pool (split1 .rodata): pane-name format at +0x1F, fixed
// pane name at +0x88.
extern "C" char lbl_eu_8050B4A8[];
// BDAT map-table pointers (.sbss, sda21-relative).
extern void* lbl_eu_806640A0;
extern void* lbl_eu_806640A8;
// Current map id (.sbss) - initial grid selection target.
extern u32 lbl_eu_80664184;
// IWorkEvent vtable for CMapSel (.data) - set by the constructor. Declared as
// an array so MWCC uses full 32-bit (lis/addi) addressing like the retail
// (a scalar extern would fall back to sda21 and produce a different reloc).
extern "C" void* lbl_eu_80536E10[];
extern "C" float lbl_eu_80668738;
extern "C" float lbl_eu_80668740;
extern "C" float lbl_eu_80668744;
// Canonical AnimRewindFrame (nw4r::lyt::AnimTransform*, float) is declared in
// code_80135FDC.hpp. CMapSel.cpp includes that header and passes proper
// AnimTransform* pointers; the guard below stays for TUs that include both
// headers (identical redeclarations are harmless, but keep the old macro
// contract so code_802405F4.cpp's define stays a no-op).
#ifndef KYOSHIN_SKIP_CMAPSEL_LEGACY_LAYOUT_IMPORTS
extern "C" u32 AnimRewindFrame(nw4r::lyt::AnimTransform*, float);
#endif
extern "C" const float lbl_eu_8066873C;
