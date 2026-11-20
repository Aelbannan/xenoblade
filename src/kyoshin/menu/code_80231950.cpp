// kyoshin/menu/code_80231950 - the arts-set widget internals (CArtsList,
// SArts*-view helpers and their Arts*_ free functions).
//
// MWCC emits a TU's generated code - the static initialiser and the MI
// adjusting thunks - at the END of its .text, so retail's CMenuArtsSet .text
// stops right after [@88@cbRenderBefore / @88@__dt__] at 0x80231950. The 109
// functions that follow (0x80231950..0x80237600) are this TU: they own the
// CArtsList vtable/base list, its RTTI record, the unit's string pool
// (.rodata 0x8050AA88..0x8050B000), the .sdata2 pool and the arts-list .sbss
// word.
//
// The declarations are shared with CMenuArtsSet.cpp through
// CMenuArtsSetImports.hpp + CMenuArtsSet.hpp; only the definitions live here.

#define IWORK_EVENT_INLINE_DTOR
// Auto-scaffolded catalog TU for kyoshin/menu/CMenuArtsSet
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/harness_catalog.hpp"
#include "kyoshin/CBaseCur.hpp"
#include "kyoshin/CScrollBar.hpp"
#include "monolib/lib/CLibLayout.hpp"

#include "kyoshin/menu/CMenuArtsSet.hpp"
#include "kyoshin/menu/CMenuArtsSetImports.hpp"
#include "kyoshin/cf/CtrlObjectParamSymbols.hpp"  // CtrlObjectParam_SyncParamFromActor (owner)
#include "kyoshin/code_8028130C.hpp" // scenario-log helpers (owning header)
#include "kyoshin/CBgTex.hpp"
#include "kyoshin/CTitleAHelp.hpp"
#include "monolib/util/MemManager.hpp"        // mtl::MemManager::allocate
#include "monolib/work/CProcess.hpp"          // CProcess::Regist
#include "monolib/device/CDeviceVI.hpp"
#include "kyoshin/CTaskGame.hpp"
#include "kyoshin/cf/CfGameManager.hpp"
#include "monolib/work/CWorkThreadSystem.hpp" // CWorkThreadSystem::getWorkMem
#include <cstddef>
#include <nw4r/lyt.h>
#include "kyoshin/code_80135FDC_Lyt.hpp"
#include "kyoshin/CQuestWindow_Pane.hpp"
#include "kyoshin/code_80135FDC_State.hpp"

// Unit-private imports (shared with code_80231950.cpp): see CMenuArtsSetImports.hpp.

// Pad state import: cf::CfGameManager::getCfPadData() comes from the
// harness_catalog include chain; CfPadData.hpp supplies the struct layout.
#include "kyoshin/cf/CfPadData.hpp"
#include "kyoshin/CTitleAHelpImports.hpp"

// Struct view of the null pointer-to-member-function constant. Reading
// through named members keeps MWCC from emitting duplicate @l references.
struct PtmfNullWordsArts {
    u32 w0;
    u32 w1;
    u32 w2;
};


// C++-linkage draw helper (mangles to the retail symbol).

// Arts archive path string (accessed via sda21 small-data relocation).
extern char lbl_eu_8050AC70[];

// Sub-object ctors (retail unmangled C symbols). extern "C" keeps the
// definition symbol and call-site relocs on the retail name.
SArtsSub8022FA58Short::SArtsSub8022FA58Short(u32 a, u32 b) {
    field_0x00 = a;
    field_0x04 = b;
    field_0x08 = 0;
    field_0x0C = 0;
    field_0x10 = 0;
    field_0x14 = 0;
    field_0x15 = 0;
    field_0x16 = 1;
    field_0x17 = 0;
    field_0x18 = 0;
    field_0x21 = 0;
}

SArtsSub8022FA58Short::~SArtsSub8022FA58Short() {}

// mSubObj124 rebuild (retail C symbol): re-arms every player slot's arts
// record, reloads the info-window layout pair, binds its open/close anims,
// then refreshes all label textboxes and the cursor position.
extern "C" void artsRebuildUI(SArtsSub8022FA58* selfRaw) {
    SArtsFAD0* self = (SArtsFAD0*)selfRaw;

    // Reset per-slot state for every active player slot.
    BuildSlotItemLists(1);
    for (u8 i = 0; i < code80135FDC_getByte_64077(); i++) {
        u8 v = GetCollectedFlagByte(i);
        if (v != 0) {
            CtrlObjectParam_SyncParamFromActorEx(CtrlObjectParam_GetCharEntry(v), 1);
        }
    }

    ArtsCharList_build(selfRaw);

    buildLayout((nw4r::lyt::Layout**)&self->field_0x08, self->field_0x04,
                  lbl_eu_8050AC70);
    char* base = lbl_eu_8050AC70;
    bindLayoutAnimTransform((nw4r::lyt::Layout*)self->field_0x08, &self->field_0x0C,
                  self->field_0x04, base + 0x18);
    bindLayoutAnimTransform((nw4r::lyt::Layout*)self->field_0x08, &self->field_0x10,
                  self->field_0x04, base + 0x35);
    self->field_0x08->v11(self->field_0x10, 0);
    self->field_0x08->v11(self->field_0x0C, 1);
    self->field_0x08->v14(0);

    // Title textbox: fixed-width name buffer, no formatting.
    char* name = BdatTouchStringCell(base + 0x50, base + 0x5b, 0x17);
    LayoutSetTextBoxFmtValue((nw4r::lyt::Layout*)self->field_0x00, base + 0x60, name, 0);

    // Info textbox: formatted entry label via sprintf.
    char* label = BdatTouchStringCell(base + 0x50, base + 0x5b, 0x18);
    char buf[0x18];
    sprintf(buf, base + 0x6a, 0, label);
    LayoutSetTextBoxFmtValue((nw4r::lyt::Layout*)self->field_0x00, base + 0x6f, label, 0);

    // Static caption textboxes on the main layout.
    setPaneVisible(self->field_0x08->field_0x10->v13(base + 0x78, 1), 0);
    setPaneVisible(self->field_0x08->field_0x10->v13(base + 0x7f, 1), 0);
    setPaneVisible(self->field_0x08->field_0x10->v13(base + 0x8f, 1), 0);

    // Cursor anchor: resolve both panes on the label layout and convert the
    // first pane's position into root-pane space; retail copies the result
    // temp into pos.
    nw4r::lyt::Pane* anchor =
        (nw4r::lyt::Pane*)self->field_0x00->field_0x10->v13(base + 0x9f, 1);
    nw4r::lyt::Pane* rootPane =
        (nw4r::lyt::Pane*)self->field_0x00->field_0x10->v13(base + 0xa6, 1);
    nw4r::math::VEC3 tmp;
    ScalePaneWorldPos(&tmp, anchor, rootPane, (nw4r::lyt::Pane*)self->field_0x00->field_0x10);
    nw4r::math::VEC3 pos = tmp;
    CBaseCur::SetPaneTranslate((nw4r::lyt::Pane*)self->field_0x08->field_0x10, &pos);

    // Single-player builds also refresh the two help captions.
    if (code80135FDC_getByte_64077() <= 1) {
        setPaneVisible(self->field_0x00->field_0x10->v13(base + 0xb5, 1), 0);
        setPaneVisible(self->field_0x00->field_0x10->v13(base + 0xc0, 1), 0);
    }

    ArtsCharList_refreshDisplay(selfRaw);
}

// Switch-case handlers used by ArtsLayoutPair_advance (same TU, defined below).
extern "C" void ArtsLayoutPair_completeLayoutIn(SArts30070* self);

extern "C" void ArtsLayoutPair_release(SArts2FDF4* self) {
    if (self->field_0x8) {
        if (self->field_0x8)
            self->field_0x8->v2(1);
        self->field_0x8 = 0;
    }
}

extern "C" void ArtsLayoutPair_advance(SArts2FDF4* self) {
    switch (self->field_0x18) {
        case 0: ArtsLayoutPair_completeLayoutOut((SArts2FF74*)self); break;
        case 1: ArtsLayoutPair_completeLayoutIn((SArts30070*)self); break;
    }
    self->field_0x8->v14(0);
}


// AnimTransform frame-check helper (defined in COption.cpp / CArtsInfo.cpp).
// State-progression byte table (sdata, 8 bytes). Fixed-size decl: incomplete
// array types are not sdata-eligible and force lis/addi instead of sda21.
extern u8 sArtsDemoModeFlag[8];
// Switch-case handlers used by ArtsLayoutPair_advance (same TU, defined below).
extern "C" void ArtsLayoutPair_completeLayoutIn(SArts30070* self);

void ArtsCharList_selectNext(SArtsSub8022FA58* self) {
    self->field_0x16 = 0;
    self->field_0x17 = 1;
    u8 next = self->field_0x14 + 1;
    self->field_0x14 = next;
    if ((s8)next >= self->field_0x21) {
        self->field_0x14 = 0;
    }
    return ArtsCharList_refreshDisplay(self);
}

extern "C" void artsGoPrevItem(SArtsSub8022FA58* self) {
    self->field_0x16 = 0;
    self->field_0x17 = 0;
    self->field_0x14--;
    if ((s8)self->field_0x14 < 0) {
        self->field_0x14 = (u8)(self->field_0x21 - 1);
    }
    return ArtsCharList_refreshDisplay(self);
}

// vtable +0x200 slot on the +0x17C manager sub-object, result discarded.
// NOTE: the header's SArtsManager vtable is mid-edit -- v128 currently maps
// to +0x208 (MWCC +2 leading slots); the retail +0x200 slot needs 2 fewer
// fillers before v126. Manual casts add an extra indirection.
extern "C" int ArtsCharList_getProgress(SArtsSub8022FA58* self) {
    SArtsManagerRoot* root =
        (SArtsManagerRoot*)CtrlObjectParam_GetCharEntry(ArtsCharList_getId(self, self->field_0x14));
    SArtsManager* obj = &root->mObj17C;
    return obj->v128();
}

extern "C" void artsShiftProg(SArtsSub8022FA58* self, int arg2) {
    SArtsManagerRoot* root =
        (SArtsManagerRoot*)CtrlObjectParam_GetCharEntry(ArtsCharList_getId(self, self->field_0x14));
    SArtsManager* obj = &root->mObj17C;
    s32 prev = obj->v128();               // vtable +0x200
    obj->v126(prev - arg2);               // vtable +0x1F8
    ArtsCharList_refreshDisplay(self);
}

// Layout-out completion handler (case-0 sibling of ArtsLayoutPair_advance's switch):
// run the AnimTransform at 0x0C; when the 0x16 flag is clear, drive both
// sub-panes via the 0x08 driver, reset the 0x10 float, mark the 0x18 state,
// then load the 0x17-dependent window label through the driver's field_0x10
// pane interface and show it. (0x0C/0x10 roles are swapped vs the matched
// ArtsLayoutPair_completeLayoutIn twin.)
void ArtsLayoutPair_completeLayoutOut(SArts2FF74* self) {
    float duration = lbl_eu_80668648;
    advanceAnimTransform(self->field_0x0C, duration);
    if (self->field_0x16 == 0) {
        self->field_0x08->v11(self->field_0x0C, 0);
        self->field_0x08->v11(self->field_0x10, 1);
        self->field_0x10->field_0x10 = lbl_eu_8066864C;
        self->field_0x18 = 1;
        if (self->field_0x17 != 0) {
            setPaneVisible(((SArts3CObj*)self->field_0x08->field_0x10)->v13(lbl_eu_8050AC70 + 0x8F, 1), 1);
        } else {
            setPaneVisible(((SArts3CObj*)self->field_0x08->field_0x10)->v13(lbl_eu_8050AC70 + 0x7F, 1), 1);
        }
    }
}

// Layout-in completion handler (case-1 sibling of ArtsLayoutPair_advance's switch):
// when the AnimTransform at 0x10 finishes, drive both sub-panes via the
// 0x08 driver, reset the 0x0C float, mark the 0x18/0x16 state, then load
// both window labels through the driver's field_0x10 pane interface.
extern "C" void ArtsLayoutPair_completeLayoutIn(SArts30070* self) {
    float duration = lbl_eu_80668648;
    if (advanceAnimTransform(self->field_0x10, duration) != 0) {
        self->field_0x08->v11(self->field_0x10, 0);
        self->field_0x08->v11(self->field_0x0C, 1);
        self->field_0x0C->field_0x10 = lbl_eu_8066864C;
        self->field_0x18 = 0;
        self->field_0x16 = 1;
        setPaneVisible(((SArts3CObj*)self->field_0x08->field_0x10)->v13(lbl_eu_8050AC70 + 0x7F, 1), 0);
        setPaneVisible(((SArts3CObj*)self->field_0x08->field_0x10)->v13(lbl_eu_8050AC70 + 0x8F, 1), 0);
    }
}



// Build the arts-selection list: clear the cursor, then append each of the
// first `count` arts entries that pass the (v>8) / (v==4 && locked) filters.
void ArtsCharList_build(SArtsSub8022FA58* self) {
    self->field_0x21 = 0;
    u8 count = code80135FDC_getByte_64077();
    for (u8 i = 0; i < count; i++) {
        u8 v = GetCollectedFlagByte(i);
        if (v > 8) continue;
        if (v == 4 && CtrlObjectParam_GetCurrentRowKey(CtrlObjectParam_GetCharEntry(v)) != 0) continue;
        u8 idx = self->field_0x21;
        self->field_0x19[idx] = v;
        self->field_0x21 = idx + 1;
    }
}

extern "C" u8 ArtsCharList_getId(SArtsSub8022FA58* self, u32 idx) {
    if (idx >= self->field_0x21) {
        return 0;
    }
    return self->field_0x19[idx];
}

// Refresh the arts-entry list display (three texture swaps + info window):
// resolve a message id for the current / previous / next list entry through
// the shared BDAT table, load the named 'timg' texture through the entry's
// arc accessor (the shared one for the prev/next arms), bind it to the label
// pane, then finally write the manager's progress value and the formatted
// name into the layout.
extern "C" void ArtsCharList_refreshDisplay(SArtsSub8022FA58* self) {
    // Current entry: texture comes from the sub-object's own accessor.
    {
        u8 id = ArtsCharList_getId(self, self->field_0x14);
        char* tex =
            MakeTplNameSysFile(BdatGetU16Direct(lbl_eu_80664090, lbl_eu_8050AC70 + 0xcb,
                                        id) &
                          0xFFFF);
        void* res = ((nw4r::lyt::ArcResourceAccessor*)self->field_0x04)
                        ->GetResource(0x74696D67u, tex, 0);
        if (res != 0) {
            PaneSetTexPaletteByName((nw4r::lyt::Layout*)self->field_0x00,
                          lbl_eu_8050AC70 + 0xd6, (TPLPalette*)res);
        }
    }

    // Previous entry: wraps below 0 into the entry count (count-1). Texture
    // is fetched via the shared CLibLayout accessor.
    {
        s8 prev = self->field_0x14 - 1;
        if (prev < 0) {
            prev = self->field_0x21 - 1;
        }
        u8 id = ArtsCharList_getId(self, (u8)prev);
        char* tex =
            MakeTplNameSysFile(BdatGetU16Direct(lbl_eu_80664090, lbl_eu_8050AC70 + 0xe1,
                                        id) &
                          0xFFFF);
        void* res = CUICfManager_getArcResourceAccessor()->GetResource(0x74696D67u, tex, 0);
        if (res != 0) {
            PaneSetTexPaletteByName((nw4r::lyt::Layout*)self->field_0x00,
                          lbl_eu_8050AC70 + 0xea, (TPLPalette*)res);
        }
    }

    // Next entry: wraps past the end back to entry 0.
    {
        s8 next = self->field_0x14 + 1;
        if ((s8)next >= self->field_0x21) {
            next = 0;
        }
        u8 id = ArtsCharList_getId(self, (u8)next);
        char* tex =
            MakeTplNameSysFile(BdatGetU16Direct(lbl_eu_80664090, lbl_eu_8050AC70 + 0xe1,
                                        id) &
                          0xFFFF);
        void* res = CUICfManager_getArcResourceAccessor()->GetResource(0x74696D67u, tex, 0);
        if (res != 0) {
            PaneSetTexPaletteByName((nw4r::lyt::Layout*)self->field_0x00,
                          lbl_eu_8050AC70 + 0xf5, (TPLPalette*)res);
        }
    }

    // Info window: format the entry name and push the manager progress.
    u8 id = ArtsCharList_getId(self, self->field_0x14);
    SArtsManagerRoot* root = (SArtsManagerRoot*)CtrlObjectParam_GetCharEntry(id);
    char* buf = BdatTouchStringCell(lbl_eu_8050AC70 + 0x50, lbl_eu_8050AC70 + 0x5b,
                              0x18);
    setLayoutTextBoxNumber(
        (nw4r::lyt::Layout*)self->field_0x00, lbl_eu_8050AC70 + 0x6f,
        root->mObj17C.v128());
    LayoutSetTextBoxFmtValue((nw4r::lyt::Layout*)self->field_0x00, lbl_eu_8050AC70 + 0x100,
                  buf, 0);
}

SArts3042C::SArts3042C(u32 a, u32 b) {
    field_0x00 = a;
    field_0x04 = b;
    field_0x08 = 0;
    field_0x0C = 0;
    field_0x10 = 0;
    field_0x14 = 0;
    field_0x18 = 0;
    field_0x1C = 0;
    field_0x20 = 4;
    field_0x21 = 4;
    field_0x22 = 0;
    field_0x23 = 1;
    field_0x24 = 0;
    field_0x25 = 2;
    field_0x26 = 0;
    field_0x27 = 0;
    field_0x28 = 1;
}

SArts3042C::~SArts3042C() {}

// Load the layout-pair object (twin of ArtsTableLayout_load): bind the 0x08/0x14
// layout drivers to the arc accessor, attach the four AnimTransforms, park
// both panes via v11, reset the state machine via v14, show the two label
// panes (v13 at +0x3C) then refresh the cursor drivers (ArtsSlotCursor_refresh).
extern "C" void ArtsLayoutPair_loadSlots(SArts304C4* self) {
    buildLayout((nw4r::lyt::Layout**)&self->field_0x08, self->field_0x04,
                  lbl_eu_8050AC70 + 0x109);
    bindLayoutAnimTransform((nw4r::lyt::Layout*)self->field_0x08, &self->field_0x0C,
                  self->field_0x04, lbl_eu_8050AC70 + 0x122);
    bindLayoutAnimTransform((nw4r::lyt::Layout*)self->field_0x08, &self->field_0x10,
                  self->field_0x04, lbl_eu_8050AC70 + 0x140);
    self->field_0x08->v11(self->field_0x10, 0);
    self->field_0x08->v11(self->field_0x0C, 1);
    self->field_0x08->v14(0);
    setPaneVisible(self->field_0x08->field_0x10->v13(lbl_eu_8050AC70 + 0x15C, 1), 0);
    setPaneVisible(self->field_0x08->field_0x10->v13(lbl_eu_8050AC70 + 0x16A, 1), 0);
    buildLayout((nw4r::lyt::Layout**)&self->field_0x14, self->field_0x04,
                  lbl_eu_8050AC70 + 0x178);
    bindLayoutAnimTransform((nw4r::lyt::Layout*)self->field_0x14, &self->field_0x18,
                  self->field_0x04, lbl_eu_8050AC70 + 0x190);
    bindLayoutAnimTransform((nw4r::lyt::Layout*)self->field_0x14, &self->field_0x1C,
                  self->field_0x04, lbl_eu_8050AC70 + 0x1AD);
    self->field_0x14->v11(self->field_0x1C, 0);
    self->field_0x14->v11(self->field_0x18, 1);
    self->field_0x14->v14(0);
    setPaneVisible((nw4r::lyt::Pane*)self->field_0x14->field_0x10, 0);
    ArtsSlotCursor_refresh((SArtsSub8022FA58*)self);
}

// Release helper for the mSubObj148 pair (called from CMenuArtsSet_teardown).
// Doubly-nested guard reproduces retail's duplicated dead beq (MWCC CSEs
// the re-tested condition into one cmpwi + two beq).
extern "C" void ArtsLayoutPair_releaseSlots(SArts3066C* self) {
    if (self->field_0x08 != 0) {
        if (self->field_0x08 != 0) {
            self->field_0x08->v2(1);
        }
        self->field_0x08 = 0;
    }
    if (self->field_0x14 != 0) {
        if (self->field_0x14 != 0) {
            self->field_0x14->v2(1);
        }
        self->field_0x14 = 0;
    }
}

// State-machine advance for the 0x148 sub-object: while enabled (0x22), run
// the current phase handler selected by 0x24 (layout-in/out pair), then the
// secondary one selected by 0x25, and finally reset both +0x38 pointees via
// their vtable slot (arg 0).
// Phase handlers driven by ArtsLayoutPair_advancePhases (defined later in this TU; C-linkage
// retail names declared in the header).
void ArtsLayoutPair_advancePhases(SArts306F0* self) {
    if (self->field_0x22 != 0) {
        switch (self->field_0x24) {
            case 0: ArtsLayout_runLayoutOut((SArts31480*)self); break;
            case 1: ArtsLayout_runLayoutIn((SArts3150C*)self); break;
        }
        switch (self->field_0x25) {
            case 2: ArtsLayout_runTwinLayoutOut((SArts315BC*)self); break;
            case 3: ArtsLayout_runTwinLayoutIn((SArts31648*)self); break;
        }
        self->field_0x08->v12(0);
        self->field_0x14->v12(0);
    }
}

extern "C" void ArtsDrawBox_draw(SArtsDrawBox* self, nw4r::lyt::DrawInfo* info) {
    if (self->field_0x22 != 0 && self->field_0x28 != 0) {
        drawLayout(self->mLayout08, info, 0, 1);
        drawLayout(self->mLayout14, info, 0, 1);
    }
}

extern "C" void ArtsSubCursor_setVisible(SArtsSub8022FA58* self, bool val) {
    if (self->field_0x27 != 0) {
        setPaneVisible((nw4r::lyt::Pane*)((SArts080C*)(self->field_0x08))->field_0x10, val);
    } else {
        self->field_0x22 = val;
        return ArtsSlotCursor_refresh(self);
    }
}

// Cursor-store helpers used by CMenuArtsSet_slotCursorUp/CMenuArtsSet_slotCursorDown etc. C-linkage +
extern "C" void ArtsSlotCursor_moveDown(SArtsSub8022FA58* self, int a) {
    self->field_0x20 = (s8)(self->field_0x20 + 1);
    if ((s8)self->field_0x20 >= 9) {
        self->field_0x20 = 0;
    }
    if (a != 0 && self->field_0x20 == 4) {
        self->field_0x20 = 5;
    }
    return ArtsSlotCursor_refresh(self);
}

extern "C" void ArtsSlotCursor_moveUp(SArtsSub8022FA58* self, int a) {
    self->field_0x20 = (s8)(self->field_0x20 - 1);
    if ((s8)self->field_0x20 < 0) {
        self->field_0x20 = 8;
    }
    if (a != 0 && self->field_0x20 == 4) {
        self->field_0x20 = 3;
    }
    return ArtsSlotCursor_refresh(self);
}

// Arts-slot pane refresh (artsSyncSlots). Stores the character id, reloads
// the object's 8-word pane-name block from lbl_eu_8050AB08 and a stack copy
// of the label table lbl_eu_8050AB88, then for each of the 8 slots asks the
// arts receiver (SArtsManager::v157 result) for the current record and binds
// or hides its panes; finally maps the character id through the message
// tables to a help-text id and binds that text onto both panes.
extern "C" void artsSyncSlots(SArts308B0View* self, u8 v) {
    self->field_0x26 = v;
    // Reload the two 8-word label tables onto the stack (MWCC counted
    // word-copy loops): the AB88 table feeds PaneSetTexPaletteByName resource binds,
    // the AB08 table feeds the pane-fetch v13 calls.
    SArts308B0Block panes = lbl_eu_8050AB08;
    SArts308B0Block names = lbl_eu_8050AB88;

    void* recv = ((SArtsManagerRoot*)CtrlObjectParam_GetCharEntry(v))->mObj17C.v157();
    char* base = lbl_eu_8050AC70;

    u32 msg;
    for (u32 i = 0; i < 8; i++) {
        // Demo gate: only character 1 with the demo flag clear uses the
        // player-specific slot view.
        int flag = 0;
        if (sArtsDemoModeFlag[0] != 0 || self->field_0x26 != 1) {
            flag = 1;
        }
        int sel = (flag == 0);
        u32 id =
            BdatGetU16ByTableKey(base + 0x1C8, base + 0x1D0, getArtsSlotRC(recv, sel, i));
        if ((u16)id != 0) {
            char* name = MakeTplNameSysFile((u16)id);
            void* res =
                ((SArtsFindObj*)self->field_0x04)->vfind(0x74696D67, name, 0);
            if (res != NULL) {
                PaneSetTexPaletteByName((nw4r::lyt::Layout*)self->field_0x00,
                              (char*)names.w[i], (TPLPalette*)res);
                setPaneVisible(
                    ((SArts308B0Layout*)self->field_0x00)->field_0x10->v13(
                        (void*)panes.w[i], 1),
                    1);
                // Second label (level/counter text): re-queries the slot
                // record and binds over the same pane.
                u32 id2 = BdatGetU16ByTableKey(base + 0x1C8, base + 0x1D5,
                                        getArtsSlotRC(recv, sel, i));
                if ((u16)id2 != 0) {
                    char* name2 = MakeTplNameSysFile((u16)id2);
                    void* res2 = ((SArtsFindObj*)self->field_0x04)
                                     ->vfind(0x74696D67, name2, 0);
                    if (res2 != NULL) {
                        PaneSetTexPaletteByName((nw4r::lyt::Layout*)self->field_0x00,
                                      (char*)panes.w[i], (TPLPalette*)res2);
                    }
                }
            } else {
                setPaneVisible(
                    ((SArts308B0Layout*)self->field_0x00)->field_0x10->v13(
                        (void*)panes.w[i], 1),
                    0);
            }
        } else {
            setPaneVisible(
                ((SArts308B0Layout*)self->field_0x00)->field_0x10->v13(
                    (void*)panes.w[i], 1),
                0);
        }
    }

    // Map the requested character id to a help-text id. Case 1 resolves the
    // category live; case 8 reads the current entry row through the BDAT
    // grid (rows 4..12 map to ids 0x77..0x7f).
    msg = 0;
    if (v <= 8U) {
        switch (v) {
        case 0:
            break;
        case 1:
            if (CtrlObjectParam_GetCurrentRowKey(CtrlObjectParam_GetCharEntry(1)) == 0) {
                msg = 1;
            } else {
                msg = 2;
            }
            break;
        case 2:
            msg = 0x13;
            break;
        case 3:
            msg = 0x24;
            break;
        case 4:
            msg = 0x29;
            break;
        case 5:
            msg = 0x3d;
            break;
        case 6:
            msg = 0x4f;
            break;
        case 7:
            msg = 0x60;
            break;
        case 8: {
            SArtsCharData24* cd = (SArtsCharData24*)CtrlObjectParam_GetCharEntry(8);
            if (cd->field_0x24 == -1) {
                msg = 0x77;
                break;
            }
            SArtsEntryWord* e = CItem_kindPtr(8);
            if (e == NULL || e->field_0x00 == 0) {
                msg = 0x77;
                break;
            }
            u32 w = e->field_0x00;
            u32 tblEntry = *(u32*)((char*)&lbl_eu_806640F8 + w);
            u32 cat = (u16)BdatGetItemId(w >> 20);
            u32 k = BdatGetU8Direct((const void*)tblEntry, (char*)lbl_eu_8050AC70 + 0x1df,
                                  cat) -
                    4;
            switch (k) {
            case 0:
                msg = 0x77;
                break;
            case 1:
                msg = 0x78;
                break;
            case 2:
                msg = 0x79;
                break;
            case 3:
                msg = 0x7a;
                break;
            case 4:
                msg = 0x7b;
                break;
            case 5:
                msg = 0x7c;
                break;
            case 6:
                msg = 0x7d;
                break;
            case 7:
                msg = 0x7e;
                break;
            case 8:
                msg = 0x7f;
                break;
            default:
                msg = 0x77;
                break;
            }
            break;
        }
        default:
            break;
        }
    }

    // Bind the selected help text onto both label panes.
    if (msg != 0) {
        u32 id = BdatGetU16ByTableKey(lbl_eu_8050AC70 + 0x1C8, lbl_eu_8050AC70 + 0x1D5,
                               msg);
        if ((u16)id != 0) {
            char* name = MakeTplNameSysFile((u16)id);
            void* res =
                ((SArtsFindObj*)self->field_0x04)->vfind(0x74696D67, name, 0);
            if (res != NULL) {
                PaneSetTexPaletteByName((nw4r::lyt::Layout*)self->field_0x00,
                              (char*)lbl_eu_8050AC70 + 0x1e8, (TPLPalette*)res);
            }
        }
        u32 id2 = BdatGetU16ByTableKey(lbl_eu_8050AC70 + 0x1C8, lbl_eu_8050AC70 + 0x1D0,
                                msg);
        if ((u16)id2 != 0) {
            char* name2 = MakeTplNameSysFile((u16)id2);
            void* res =
                ((SArtsFindObj*)self->field_0x04)->vfind(0x74696D67, name2, 0);
            if (res != NULL) {
                PaneSetTexPaletteByName((nw4r::lyt::Layout*)self->field_0x00,
                              (char*)lbl_eu_8050AC70 + 0x1f3, (TPLPalette*)res);
            }
        }
    }
}

// Drive both pointees when the 0x23 state is set: reset 0x24, show the
// first pane, hide the second.
extern "C" void ArtsBusyPane_showPrimary(SArts30D18* self) {
    if (self->field_0x23 != 0) {
        self->field_0x24 = 0;
        setPaneVisible((nw4r::lyt::Pane*)self->field_0x08->field_0x10, 1);
        setPaneVisible((nw4r::lyt::Pane*)self->field_0x14->field_0x10, 0);
    }
}

// Shared tail of both artsClaimSlot arms
// inlines it into each arm): re-arm the arts list, then if the player object
// exists and its +0x3F28 id matches the sub-object's character byte, run its
// refresh virtual (vtable +0x32C) and re-apply the slot view.
#define ARTS_D74_TAIL(self, root)                                              \
    do {                                                                       \
        CtrlObjectParam_SyncParamFromActorEx((root), 1);                                              \
        __ct__8009F8B8((root));                                                \
        void* player = getPlayer__Q22cf13CfGameManagerFi(0);                   \
        if (player != 0) {                                                     \
            SArtsPlayerRefresh* pc =                                           \
                (SArtsPlayerRefresh*)getCfObjectPc__FPQ22cf12CfObjectMove(     \
                    player);                                                   \
            if (pc != 0 && pc->field_3F28 == (self)->field_0x26) {             \
                pc->vf32C();                                                   \
            }                                                                  \
        }                                                                      \
        artsSyncSlots((SArts308B0View*)(self), (self)->field_0x26);                \
    } while (0)

// Arts-slot update: val == -1 clears the current cursor entries; otherwise
// the entry is claimed in the selected row (moving it out of any other slot
// holding a lower value first). Both arms resolve the manager themselves:
// the 8-byte data window starts at data[8] only when the demo gate is clear
// AND the character id is 1 (sel = !gate).
extern "C" void artsClaimSlot(SArtsSub8022FA58* self,
                                                   s32 val) {
    // All locals declared at function scope: retail keeps every one of them in
    // a callee-saved home across both arms (stmw r25, frame 0x30).
    s32 v;
    SArtsManagerRoot* root;
    SArtsSubDElem* elems;
    SArtsSubDElem* e;
    u8 arts;
    int flag;
    u8 sel;
    int off;
    u8 c20;
    u8 c21;
    u8 cur;
    u8 curVal;
    u32 i;

    v = val;
    if (v == -1) {
        // Clear arm: clamp both cursors (4 -> none), then write each old row
        // value back into the cursor-0x20 slot (clearing both entries).
        root = (SArtsManagerRoot*)CtrlObjectParam_GetCharEntry(self->field_0x26);
        elems = &root->mElemsE8[0];
        arts = (u8)CtrlObjectParam_GetCurrentRowKey(root);
        flag = (sArtsDemoModeFlag[0] != 0) || (self->field_0x26 != 1);
        c20 = self->field_0x20;
        sel = (flag == 0);
        if ((u32)c20 >= 4) {
            c20 -= 1;
        }
        c21 = self->field_0x21;
        if ((u32)c21 >= 4) {
            c21 -= 1;
        }
        off = sel * 8;
        e = &elems[arts];
        ArtsElem_setDataByte(elems, (&e->data[off])[c20], arts, sel, c20);
        ArtsElem_setDataByte(elems, (&e->data[off])[c21], arts, sel, c20);
        ARTS_D74_TAIL(self, root);
    } else {
        // Claim arm: pick the active cursor (busy flag selects 0x21 over
        // 0x20), clear the previous owner when it held a smaller value.
        root = (SArtsManagerRoot*)CtrlObjectParam_GetCharEntry(self->field_0x26);
        elems = &root->mElemsE8[0];
        arts = (u8)CtrlObjectParam_GetCurrentRowKey(root);
        if (self->field_0x27 != 0) {
            cur = self->field_0x21;
        } else {
            cur = self->field_0x20;
        }
        if ((u32)cur >= 4) {
            cur -= 1;
        }
        flag = (sArtsDemoModeFlag[0] != 0) || (self->field_0x26 != 1);
        sel = (flag == 0);
        off = sel * 8;
        e = &elems[arts];
        curVal = (&e->data[off])[cur];
        // Retail gates the previous-owner scan on val > 0 (not on curVal).
        if (v > 0) {
            for (i = 0; i < 8; i++) {
                if (v == (&e->data[off])[i]) {
                    ArtsElem_setDataByte(elems, curVal, arts, sel, i);
                    break;
                }
            }
        }
        ArtsElem_setDataByte(elems, (u8)v, arts, sel, cur);
        ARTS_D74_TAIL(self, root);
    }
}

void ArtsElem_setDataByte(SArtsSubDElem* self, u8 val, u32 idx, u32 sub, u32 off) {
    SArtsSubDElem* e = &self[idx];
    u8* d = e->data + sub * 8;
    d[off] = val;
    e->field_0x30 = 1;
}

// Arts-list selected-entry query (retail C symbol). Refreshes the character
// manager, then branches on the 0x20 mode. Mode 4 (locked-entry state)
// dispatches on the 0x26 character id: id 0 returns 0, ids 2-7 map to fixed
// action ids, id 1 probes character 1's category, and id 8 resolves the
// current item entry through the BDAT text table. Other modes read the
// per-character state byte from the 0x49-byte arts element row (offset 8
// when the byte-table gate and the mode byte are both valid).
extern "C" u8 artsReadSelId(SArtsSub8022FA58* self) {
    SArtsManagerRoot* root = (SArtsManagerRoot*)CtrlObjectParam_GetCharEntry(self->field_0x26);
    root->mObj17C.v157();
    SArtsSubDElem* elems = &root->mElemsE8[0];
    u8 arts = (u8)CtrlObjectParam_GetCurrentRowKey(root);
    u8 result = 0;
    u8 mode = self->field_0x20;
    if ((s32)mode == 4) {
        switch (self->field_0x26) {
        case 1:
            // Branchy form: retail emits cmpwi/bne around two constant arms.
            if (CtrlObjectParam_GetCurrentRowKey(CtrlObjectParam_GetCharEntry(1)) == 0) {
                result = 1;
            } else {
                result = 2;
            }
            break;
        case 2: result = 0x13; break;
        case 3: result = 0x24; break;
        case 4: result = 0x29; break;
        case 5: result = 0x3d; break;
        case 6: result = 0x4f; break;
        case 7: result = 0x60; break;
        case 8: {
            // Slot-8 record: -1 entry id means nothing to resolve.
            SArtsCharData24* rec = (SArtsCharData24*)CtrlObjectParam_GetCharEntry(8);
            if (rec->field_0x24 == -1) {
                result = 0x77;
                break;
            }
            SArtsEntryWord* entry = CItem_kindPtr(8);
            if (entry == 0 || entry->field_0x00 == 0) {
                result = 0x77;
                break;
            }
            // Item word top 12 bits -> category; BDAT row keyed by category.
            // tbl is loaded ahead of the category lookup (retail schedule).
            u32 tbl = (u32)lbl_eu_806640F8;
            u16 cat = (u16)BdatGetItemId(entry->field_0x00 >> 20);
            u8 row = BdatGetU8Direct((const void*)tbl, lbl_eu_8050AC70 + 0x1df, cat);
            switch (row) {
            case 4: result = 119; break;    // 119 == 0x77: distinct arm keeps
                                            // the retail 9-entry table shape
            case 5: result = 0x78; break;
            case 6: result = 0x79; break;
            case 7: result = 0x7a; break;
            case 8: result = 0x7b; break;
            case 9: result = 0x7c; break;
            case 10: result = 0x7d; break;
            case 11: result = 0x7e; break;
            case 12: result = 0x7f; break;
            default: result = 0x77; break;
            }
            break;
        }
        default:
            break;
        }
    } else {
        u8 idx = mode;
        if ((u32)idx >= 4) {
            idx -= 1;
        }
        SArtsSubDElem* e = &elems[arts];
        // Same gate idiom as ArtsSlot_isWindowEmpty: phi-stored byte flag, re-booleaned
        // so the negate-multiply extract stays on the full 8-bit lane.
        u8 flag = 0;
        if (sArtsDemoModeFlag[0] != 0 || self->field_0x26 != 1) {
            flag = 1;
        }
        flag = (flag == 0);
        int off = flag * 8;
        return (&e->data[off])[idx];
    }
    return result;
}

// Arts-list availability query: ask the arts manager to refresh, then pick
// the arts element for the current character, clamp the 0x20 cursor
// (4 -> none), read the state byte from the element's data window (offset 8
// when the byte-table entry and mode byte are both valid), translate it
// through the 0x1C8/0x1FE string table, and - when the learn-arts flag grid
// reports the entry set - return the high byte of the 16-bit value at the
// flagged offset.
extern "C" u8 artsAvailFlag(SArtsSub8022FA58* self) {
    SArtsManagerRoot* root = (SArtsManagerRoot*)CtrlObjectParam_GetCharEntry(self->field_0x26);
    root->mObj17C.v157();
    SArtsSubDElem* elems = &root->mElemsE8[0];
    u8 arts = (u8)CtrlObjectParam_GetCurrentRowKey(root);
    u8 result = 0;
    u8 idx = self->field_0x20;
    if ((s32)idx != 4) {
        if ((u32)idx >= 4) {
            idx -= 1;
        }
        // Constant [0]: MWCC fuses the sda21 displacement, leaving clamped
        // idx as the dead d-form base register.
        int flag = (sArtsDemoModeFlag[0] != 0) || (self->field_0x26 != 1);
        int off = (flag == 0) * 8;
        u8 v = elems[arts].data[off + idx];
        char* base = lbl_eu_8050AC70;
        u32 r = BdatGetU8ByTableKey(base + 0x1C8, base + 0x1FE, v);
        if (ResTbox_IsCellBitSet((u8*)elems, arts, (u8)r) != 0) {
            u16 w = *(const u16*)((u8*)&elems[arts] + ((u8)r << 1));
            result = *(const u8*)&w;
        }
    }
    return result;
}

// Arts-list availability check: ask the arts manager (vtable +0x27C) to
// recompute the list, then scan the 8-byte window at data[off..off+7] of the
// current arts element for all-zero state. The window starts at data[8] when
// the arts id is valid (byte-table entry 0 and mode byte 1), else at data[0].
// Returns 1 when all scanned bytes are zero.
extern "C" u32 ArtsSlot_isWindowEmpty(SArtsSub8022FA58* self) {
    SArtsManagerRoot* root = (SArtsManagerRoot*)CtrlObjectParam_GetCharEntry(self->field_0x26);
    root->mObj17C.v157();
    SArtsSubDElem* base = &root->mElemsE8[0];
    u8 arts = (u8)CtrlObjectParam_GetCurrentRowKey(root);
    u8 b = 0;
    if (sArtsDemoModeFlag[0] != 0 || self->field_0x26 != 1) {
        b = 1;
    }
    b = (b == 0);
    SArtsSubDElem* e = &base[arts];
    int off = b * 8;
    for (u8 i = 0; i < 8; i++) {
        if (e->data[off + i] != 0) return 0;
    }
    return 1;
}

// Toggle the 0x27 busy flag: while busy, clear it and park both panes
// (first visible, second hidden); otherwise set it, stash the old 0x20
// cursor into 0x21, and run the layout-out animation.
extern "C" void ArtsBusyPane_toggle(SArts313E0* self) {
    if (self->field_0x27 != 0) {
        self->field_0x27 = 0;
        setPaneVisible((nw4r::lyt::Pane*)self->field_0x08->field_0x10, 1);
        setPaneVisible((nw4r::lyt::Pane*)self->field_0x14->field_0x10, 0);
    } else {
        self->field_0x27 = 1;
        self->field_0x21 = self->field_0x20;
        ArtsBusyPane_refreshCursor(self);
        setPaneVisible((nw4r::lyt::Pane*)self->field_0x14->field_0x10, 1);
    }
}

extern "C" void ArtsSlotCursor_clampFromEmpty(SArtsSub8022FA58* self) {
    if (self->field_0x20 != 4) {
        return;
    }
    self->field_0x20 = 3;
    return ArtsSlotCursor_refresh(self);
}

// CMenuArtsSet_setPaneScale is defined later in this TU (writes the 0x44/0x48 floats of
// the pane-like object passed to ArtsSlotCursor_refresh). C-linkage retail name.
extern "C" void CMenuArtsSet_setPaneScale(SArtsPaneScaleObj* self, const SArtsVec2* src);

// Refresh the arts-list cursor: copy the 9-word label block from
// lbl_eu_8050ABB4, look up the pane for the current 0x20 cursor entry, scale
// its translate by the 0xA6 label pane's scale, move the 0x08 sub-pane onto
// it, then write the precomputed 9-entry position table (entry 4 uses the
// smaller lbl_eu_80668654 value) into the pane's layout offsets via
// CMenuArtsSet_setPaneScale.
extern "C" void ArtsSlotCursor_refresh(SArtsSub8022FA58* self) {
    nw4r::math::VEC3 pos;
    SArts316F8Block tmp;
    tmp = lbl_eu_8050ABB4;
    // Retail lays 9 fully-initialized entries (entry 4 is the 654 pair);
    // a short list makes MWCC emit a zero-fill loop instead of stores.
    SArtsVec2 vecs[9] = {
        {lbl_eu_80668650, lbl_eu_80668650}, {lbl_eu_80668650, lbl_eu_80668650},
        {lbl_eu_80668650, lbl_eu_80668650}, {lbl_eu_80668650, lbl_eu_80668650},
        {lbl_eu_80668654, lbl_eu_80668654},
        {lbl_eu_80668650, lbl_eu_80668650}, {lbl_eu_80668650, lbl_eu_80668650},
        {lbl_eu_80668650, lbl_eu_80668650}, {lbl_eu_80668650, lbl_eu_80668650},
    };
    nw4r::lyt::Pane* pane =
        (nw4r::lyt::Pane*)((SArts3CObj*)((SArts3150CDriver*)self->field_0x00)->field_0x10)
            ->v13((void*)tmp.w[self->field_0x20], 1);
    AccumPaneWorldPos(&pos, pane);
    pane = (nw4r::lyt::Pane*)((SArts3CObj*)((SArts3150CDriver*)self->field_0x00)->field_0x10)
               ->v13((void*)(lbl_eu_8050AC70 + 0xA6), 1);
    pos.x *= pane->GetScale().x;
    CBaseCur::SetPaneTranslate((nw4r::lyt::Pane*)((SArts3150CDriver*)self->field_0x08)->field_0x10, &pos);
    CMenuArtsSet_setPaneScale((SArtsPaneScaleObj*)((SArts3150CDriver*)self->field_0x08)->field_0x10,
                  &vecs[self->field_0x20]);
}

// Run the layout-out animation; when the busy flag (0x23) is clear, drive
// both sub-panels via vtable slot 0x2C (v11) then bump the 0x24 state.
// Layout-in driver twin of ArtsLayout_runLayoutIn's shape (0x08/0x0C/0x10 fields).
void ArtsLayout_runLayoutOut(SArts31480* self) {
    float duration = lbl_eu_80668648;
    advanceAnimTransform(self->field_0x0C, duration);
    if (self->field_0x23 == 0) {
        self->field_0x08->v11(self->field_0x0C, 0);
        self->field_0x08->v11(self->field_0x10, 1);
        self->field_0x10->field_0x10 = lbl_eu_8066864C;
        self->field_0x24 = 1;
    }
}

// Twin driver: run the AnimTransform at 0x10; when the frame check reports
// the animation finished, drive both sub-panes (v11 at +0x2C), reset the
// 0x0C float, show the panes, and bump the 0x23/0x24 state.
void ArtsLayout_runLayoutIn(SArts3150C* self) {
    float duration = lbl_eu_80668648;
    if (advanceAnimTransform(self->field_0x10, duration) != 0) {
        self->field_0x08->v11(self->field_0x10, 0);
        self->field_0x08->v11(self->field_0x0C, 1);
        self->field_0x0C->field_0x10 = lbl_eu_8066864C;
        setPaneVisible((nw4r::lyt::Pane*)self->field_0x08->field_0x10, 0);
        setPaneVisible((nw4r::lyt::Pane*)self->field_0x14->field_0x10, 1);
        self->field_0x24 = 2;
        self->field_0x23 = 1;
    }
}

// Same driver as ArtsLayout_runLayoutOut but for the offset-0x14 sub-object; the
// 0x24 state is bumped to 3 instead of 1.
void ArtsLayout_runTwinLayoutOut(SArts315BC* self) {
    float duration = lbl_eu_80668648;
    advanceAnimTransform(self->field_0x18, duration);
    if (self->field_0x23 == 0) {
        self->field_0x14->v11(self->field_0x18, 0);
        self->field_0x14->v11(self->field_0x1C, 1);
        self->field_0x1C->field_0x10 = lbl_eu_8066864C;
        self->field_0x24 = 3;
    }
}

// Layout-out completion driver for the mSubObj148+0x2C twin: run the
// AnimTransform at 0x1C; when finished, drive both panes via the 0x14
// driver's v11 slot, reset the 0x18 float, show/hide via the 0x08/0x14 pane
// pointees, and mark state 0x24=0 / busy 0x23=1.
void ArtsLayout_runTwinLayoutIn(SArts31648* self) {
    float duration = lbl_eu_80668648;
    if (advanceAnimTransform(self->field_0x1C, duration) != 0) {
        self->field_0x14->v11(self->field_0x1C, 0);
        self->field_0x14->v11(self->field_0x18, 1);
        self->field_0x18->field_0x10 = lbl_eu_8066864C;
        setPaneVisible((nw4r::lyt::Pane*)self->field_0x08->field_0x10, 1);
        setPaneVisible((nw4r::lyt::Pane*)self->field_0x14->field_0x10, 0);
        self->field_0x24 = 0;
        self->field_0x23 = 1;
    }
}

// Writes the 0x44/0x48 floats (pane scale view) of the object passed in.
extern "C" void CMenuArtsSet_setPaneScale(SArtsPaneScaleObj* self, const SArtsVec2* src) {
    self->mField44 = src->x;
    self->mField48 = src->y;
}

extern "C" void ArtsBusyPane_refreshCursor(SArts313E0* self) {
    nw4r::math::VEC3 pos;
    SArts316F8Block tmp;
    tmp = lbl_eu_8050ABD8;
    // Retail lays 9 fully-initialized entries (entry 4 is the 654 pair);
    // a short list makes MWCC emit a zero-fill loop instead of stores.
    SArtsVec2 vecs[9] = {
        {lbl_eu_80668650, lbl_eu_80668650}, {lbl_eu_80668650, lbl_eu_80668650},
        {lbl_eu_80668650, lbl_eu_80668650}, {lbl_eu_80668650, lbl_eu_80668650},
        {lbl_eu_80668654, lbl_eu_80668654},
        {lbl_eu_80668650, lbl_eu_80668650}, {lbl_eu_80668650, lbl_eu_80668650},
        {lbl_eu_80668650, lbl_eu_80668650}, {lbl_eu_80668650, lbl_eu_80668650},
    };
    nw4r::lyt::Pane* pane =
        (nw4r::lyt::Pane*)((SArts3CObj*)((SArts3150CDriver*)self->field_0x00)->field_0x10)
            ->v13((void*)tmp.w[self->field_0x21], 1);
    AccumPaneWorldPos(&pos, pane);
    pane = (nw4r::lyt::Pane*)((SArts3CObj*)((SArts3150CDriver*)self->field_0x00)->field_0x10)
               ->v13((void*)(lbl_eu_8050AC70 + 0xA6), 1);
    pos.x *= pane->GetScale().x;
    CBaseCur::SetPaneTranslate((nw4r::lyt::Pane*)self->field_0x14->field_0x10, &pos);
    CMenuArtsSet_setPaneScale((SArtsPaneScaleObj*)self->field_0x14->field_0x10, &vecs[self->field_0x21]);
}

SArts319AC::SArts319AC(u32 a, u32 b, u32 c) {
    field_0x00 = a;
    field_0x04 = b;
    field_0x08 = 0;
    field_0x0C = 0;
    field_0x10 = 0;
    field_0x14 = 0;
    field_0x18 = 0;
    field_0x1C = 0;
    field_0x20 = 0;
    field_0x21 = 0;
    field_0x22 = 0;
    field_0x23 = 1;
    field_0x24 = 0;
    field_0x25 = 2;
    field_0x26 = 0;
    field_0x28 = c;
    field_0x12C = 0;
    field_0x12D = 0;
    field_0x12E = 1;
}

SArts319AC::~SArts319AC() {}

// Layout-pair init twin of ArtsLayoutPair_loadSlots with different label strings
// (+0x202/+0x210): after parking both panes, reset the pane translate of
// each layout's SArts3CObj to (0,0) via CMenuArtsSet_setPaneScale, then refresh the
// cursor drivers (ArtsTableCursor_refresh).
extern "C" void ArtsTableLayout_load(SArts304C4* self) {
    buildLayout((nw4r::lyt::Layout**)&self->field_0x08, self->field_0x04,
                  lbl_eu_8050AC70 + 0x109);
    bindLayoutAnimTransform((nw4r::lyt::Layout*)self->field_0x08, &self->field_0x0C,
                  self->field_0x04, lbl_eu_8050AC70 + 0x122);
    bindLayoutAnimTransform((nw4r::lyt::Layout*)self->field_0x08, &self->field_0x10,
                  self->field_0x04, lbl_eu_8050AC70 + 0x140);
    self->field_0x08->v11(self->field_0x10, 0);
    self->field_0x08->v11(self->field_0x0C, 1);
    self->field_0x08->v14(0);
    setPaneVisible(self->field_0x08->field_0x10->v13(lbl_eu_8050AC70 + 0x202, 1), 0);
    setPaneVisible(self->field_0x08->field_0x10->v13(lbl_eu_8050AC70 + 0x210, 1), 0);
    SArtsVec2 v1 = {lbl_eu_80668658, lbl_eu_80668658};
    CMenuArtsSet_setPaneScale((SArtsPaneScaleObj*)self->field_0x08->field_0x10, &v1);
    buildLayout((nw4r::lyt::Layout**)&self->field_0x14, self->field_0x04,
                  lbl_eu_8050AC70 + 0x178);
    bindLayoutAnimTransform((nw4r::lyt::Layout*)self->field_0x14, &self->field_0x18,
                  self->field_0x04, lbl_eu_8050AC70 + 0x190);
    bindLayoutAnimTransform((nw4r::lyt::Layout*)self->field_0x14, &self->field_0x1C,
                  self->field_0x04, lbl_eu_8050AC70 + 0x1AD);
    self->field_0x14->v11(self->field_0x1C, 0);
    self->field_0x14->v11(self->field_0x18, 1);
    self->field_0x14->v14(0);
    setPaneVisible((nw4r::lyt::Pane*)self->field_0x14->field_0x10, 0);
    SArtsVec2 v2 = {lbl_eu_80668658, lbl_eu_80668658};
    CMenuArtsSet_setPaneScale((SArtsPaneScaleObj*)self->field_0x14->field_0x10, &v2);
    ArtsTableCursor_refresh((SArts327B0*)self);
}

// Release both SArts2FObj pointees via their vtable slot 0x08 (v2), then
// null each field (retail keeps the redundant null-check branch).
extern "C" void ArtsTableLayout_release(SArts3066C* self) {
    if (self->field_0x08 != 0) {
        if (self->field_0x08 != 0) {
            self->field_0x08->v2(1);
        }
        self->field_0x08 = 0;
    }
    if (self->field_0x14 != 0) {
        if (self->field_0x14 != 0) {
            self->field_0x14->v2(1);
        }
        self->field_0x14 = 0;
    }
}

// Same-TU phase handlers (defined below); retail keeps real bl calls.
extern "C" void ArtsTableLayout_runLayoutOut(SArts31480* self);
extern "C" void ArtsTableLayout_runTwinLayoutOut(SArts315BC* self);

// State-machine advance for the +0x2C twin of ArtsLayoutPair_advancePhases: while enabled
// (0x22), run the current phase handler selected by 0x24 (layout-in/out pair,
// ArtsTableLayout_runLayoutOut/ArtsTableLayout_runLayoutIn), then the secondary one selected by 0x25
// (ArtsTableLayout_runTwinLayoutOut/ArtsTableLayout_runTwinLayoutIn), and finally reset both +0x38 pointees via
// their vtable slot (arg 0).
void ArtsTableLayout_advancePhases(SArts306F0* self) {
    if (self->field_0x22 != 0) {
        switch (self->field_0x24) {
            case 0: ArtsTableLayout_runLayoutOut((SArts31480*)self); break;
            case 1: ArtsTableLayout_runLayoutIn((SArts3150C*)self); break;
        }
        switch (self->field_0x25) {
            case 2: ArtsTableLayout_runTwinLayoutOut((SArts315BC*)self); break;
            case 3: ArtsTableLayout_runTwinLayoutIn((SArts31648*)self); break;
        }
        self->field_0x08->v12(0);
        self->field_0x14->v12(0);
    }
}

// Draw both layouts when the box is enabled (0x22) and the arts table is
// visible (0x12E).
extern "C" void ArtsTableDrawBox_draw(SArtsDrawBox* self, nw4r::lyt::DrawInfo* info) {
    if (self->field_0x22 != 0 && self->field_0x12E != 0) {
        drawLayout(self->mLayout08, info, 0, 1);
        drawLayout(self->mLayout14, info, 0, 1);
    }
}

// Cursor-driver refresh helpers (defined below in this TU; C-linkage retail
// names). Declared here because ArtsTable_scrollUp / ArtsTable_pageDown call them.
extern "C" void ArtsTableCursor_refresh(SArts327B0* self);
extern "C" void artsRenderRow(SArts327B0* self);

// Scroll the arts-table cursor up one entry: decrement the 0x20 row cursor,
// and when it wraps past 0 borrow into the 0x21 page offset. When both wrap,
// jump to the previous 5-row page (0x20 = 4, 0x21 = count-5) or, for counts
// below 5, park the cursor with a saturating 0x20 = count-1. Then refresh the
// scrollbar and both cursor drivers.
// The unit is -O4,s: the saturating decrement lowers to addic/subfe.
extern "C" void ArtsTable_scrollUp(SArts322BC* self) {
    // Assignment-in-condition: MWCC keeps the stored value in-register and
    // re-truncates it to u8 for the sign test (clrlwi/extsb idiom).
    if ((s8)(u8)(self->field_0x20 = self->field_0x20 - 1) < 0) {
        self->field_0x20 = 0;
        if ((s8)(u8)(self->field_0x21 = self->field_0x21 - 1) < 0) {
            u8 c = self->field_0x12C;
            if (c > 5) {
                self->field_0x20 = 4;
                self->field_0x21 = c - 5;
            } else {
                self->field_0x21 = 0;
                // Saturating decrement; size-opt lowers to addic/subfe/andc.
                self->field_0x21 = 0;
                // Saturating decrement; size-opt lowers to the addic/subfe mask.
                self->field_0x20 = c == 0 ? 0 : c - 1;
            }
        }
    }
    // Args evaluated right-to-left: cursor byte first, scrollbar second.
    reinterpret_cast<CScrollBar*>(self->field_0x28)->PlaceThumb(self->field_0x21);
    ArtsTableCursor_refresh((SArts327B0*)self);
    artsRenderRow((SArts327B0*)self);
}

extern "C" void ArtsTable_scrollDown(SArts322BC* self) {
    u8 count = self->field_0x12C;
    if (count >= 5) {
        // Row cursor forward; on overflow park at row 4 and step the page
        // offset, resetting both when the page runs past count-5.
        u8 a = self->field_0x20 + 1;
        self->field_0x20 = a;
        if ((s8)a >= 5) {
            self->field_0x20 = 4;
            u8 b = self->field_0x21;
            // Double use of b: MWCC CSEs to one increment while keeping the
            // old value in its own register (retail r4 old / r5 sum).
            self->field_0x21 = b + 1;
            if ((s8)(u8)(b + 1) > (s32)(count - 5)) {
                self->field_0x20 = 0;
                self->field_0x21 = 0;
            }
        }
    } else {
        u8 a = self->field_0x20 + 1;
        self->field_0x20 = a;
        if ((s8)a >= (s32)count) {
            self->field_0x20 = 0;
            self->field_0x21 = 0;
        }
    }
    // Named in retail evaluation order: cursor byte first (lands in the
    // arg-2 register), scrollbar pointer second.
    u16 c = self->field_0x21;
    u8* sb = self->field_0x28;
    reinterpret_cast<CScrollBar*>(sb)->PlaceThumb(c);
    ArtsTableCursor_refresh((SArts327B0*)self);
    artsRenderRow((SArts327B0*)self);
}

// Cursor-driver refresh helpers (defined below in this TU; C-linkage retail
// names). Declared here because ArtsTable_pageUp calls them.
extern "C" void ArtsTableCursor_refresh(SArts327B0* self);
extern "C" void artsRenderRow(SArts327B0* self);

// Scroll the arts-table cursor up one 5-row page: when the 0x12C count is
// below 5 the cursor is cleared entirely; otherwise step 0x21 back by 5 and
// wrap a negative offset into the previous page via 0x20. Then refresh the
// scrollbar and both cursor drivers.
extern "C" void ArtsTable_pageUp(SArts322BC* self) {
    if (self->field_0x12C >= 5) {
        u8 a = self->field_0x21 - 5;
        self->field_0x21 = a;
        if ((s8)a < 0) {
            u8 t = a + 4;
            self->field_0x20 = t;
            self->field_0x21 = 0;
            if ((s8)self->field_0x20 < 0) {
                self->field_0x20 = 0;
            }
        }
    } else {
        self->field_0x20 = 0;
        self->field_0x21 = 0;
    }
    // Named in retail evaluation order: cursor byte first (lands in the
    // arg-2 register), scrollbar pointer second.
    u16 c = self->field_0x21;
    u8* sb = self->field_0x28;
    reinterpret_cast<CScrollBar*>(sb)->PlaceThumb(c);
    ArtsTableCursor_refresh((SArts327B0*)self);
    artsRenderRow((SArts327B0*)self);
}

// Scroll the arts-table cursor down one 5-row page: when the 0x12C count is
// at least 5, step the 0x21 page offset forward by 5 and wrap any overshoot
// back into the 0x20 row cursor (clamped at 4); otherwise the cursor is
// simply cleared. Then refresh the scrollbar and both cursor drivers.
extern "C" void ArtsTable_pageDown(SArts322BC* self) {
    u8 c = self->field_0x12C;
    if (c >= 5) {
        u8 a = self->field_0x21 + 5;
        self->field_0x21 = a;
        if ((s8)a > (s32)(c - 5)) {
            u8 t = (u8)(a - (c - 5));
            self->field_0x20 = t;
            self->field_0x21 = (u8)(c - 5);
            if ((s8)t >= 5) self->field_0x20 = 4;
        }
    } else {
        self->field_0x20 = c - 1;
        self->field_0x21 = 0;
        if ((s8)(u8)(c - 1) < 0) self->field_0x20 = 0;
    }
    // Sibling 80231F60: cursor byte first so the (s8) load births in r4.
    s8 page = self->field_0x21;
    u8* sb = self->field_0x28;
    reinterpret_cast<CScrollBar*>(sb)->PlaceThumb((u16)page);
    ArtsTableCursor_refresh((SArts327B0*)self);
    artsRenderRow((SArts327B0*)self);
}

// Rebuild the arts table for a character: clear the cursor and row count,
// fetch the character's arts-availability list (copied per-character block),
// then for each available art append a 16-byte row to the 0x2C table with
// its id/name/flag fields, finally refreshing the scrollbar and cursors.
extern "C" void artsFillTable(SArts322BC* self, u8 arg) {
    self->field_0x20 = 0;
    self->field_0x21 = 0;
    self->field_0x26 = arg;
    self->field_0x12C = 0;
    SArtsManagerRoot* root = (SArtsManagerRoot*)CtrlObjectParam_GetCharEntry(arg);
    SArtsSubDElem* elems = &root->mElemsE8[0];
    SArts320C0Block tmp;
    tmp = lbl_eu_8050ABFC;
    u32 charIdx = CtrlObjectParam_GetCurrentRowKey(root);
    SArts320C0Entry* p = (SArts320C0Entry*)&tmp + arg;
    // Demo/default override: first character gets a fixed starting art.
    if (sArtsDemoModeFlag[0] == 0 && arg == 1) {
        p->id = 3;
    }
    for (u8 i = 0; i < p->count; i++) {
        u32 artId = BdatGetU8ByTableKey(lbl_eu_8050AC70 + 0x1C8, lbl_eu_8050AC70 + 0x1FE,
                                  p->id + i);
        if (ResTbox_IsCellBitSet((u8*)elems, charIdx, (u8)artId) == 0) {
            continue;
        }
        const u8* elemBytes = (const u8*)&elems[charIdx];
        u16 w = *(const u16*)&elemBytes[(u8)artId << 1];
        if (*(const u8*)&w == 0) {
            continue;
        }
        u8 n = self->field_0x12C++;
        u8* row = &self->mTable[n << 4];
        row[0] = p->id + i;
        row[1] = artId;
        row[2] = *(const u8*)&w;
        *(u16*)&row[4] = BdatGetU16ByTableKey(lbl_eu_8050AC70 + 0x1C8, lbl_eu_8050AC70 + 0x1D0,
                                       p->id + i);
        *(u16*)&row[6] = BdatGetU16ByTableKey(lbl_eu_8050AC70 + 0x1C8, lbl_eu_8050AC70 + 0x1D5,
                                       p->id + i);
        *(u32*)&row[0xC] =
            (u32)BdatTouchStringCell((char*)lbl_eu_8050AC70 + 0x1C8, lbl_eu_8050AC70 + 0x5B,
                               p->id + i);
        // Signed base blocks the bool-proof on the single-bit mask, so the
        // setnz normalisation survives alongside the narrow extract.
        const u32 lo = ((const u8*)&w)[1];
        // Deferred normalisation: shifts materialise wide values first, the
        // !=0 tests then emit the setnz pairs.
        u32 f7 = lo >> 7;
        u32 f6 = lo >> 6;
        row[8] = f7 != 0;
        row[9] = f6 != 0;
    }
    reinterpret_cast<CScrollBar*>(self->field_0x28)->UpdateThumb(5, self->field_0x12C);
    reinterpret_cast<CScrollBar*>(self->field_0x28)->PlaceThumb((u16)self->field_0x21);
    artsRenderRow((SArts327B0*)self);
    ArtsTableCursor_refresh((SArts327B0*)self);
}

extern "C" u8 ArtsTable_getSelectedId(SArts322BC* self) {
    if (self->field_0x12C != 0) {
        s32 off = (self->field_0x21 + self->field_0x20) << 4;
        return self->mTable[off];
    }
    return 0;
}

extern "C" u8 ArtsTable_getRowValue(SArts322BC* self, int key) {
    u8 count = self->field_0x12C;
    if (count == 0) return 0;
    if (key == -1) {
        int o = (((int)self->field_0x21 + (int)self->field_0x20) << 4);
        return *(self->mTable + o + 2);
    }
    for (u8 i = 0; i < count; i++) {
        if ((int)self->mTable[i * 16] == key) {
            return self->mTable[i * 16 + 2];
        }
    }
    return 0;
}

// Arts-table percent query: for the current cursor row (key == -1) or the
// row whose id byte matches key, compute a percentage from the row's value
// byte (BdatGetU16ByTableKey string-table lookup, +1) times the arts count byte
// (BdatGetU8ByTableKey lookup keyed by the buffer formatted from the row's
// second byte), divided by 100 when positive.
extern "C" u16 artsGetRowPct(SArts322BC* self, int key) {
    u8 count = self->field_0x12C;
    u16 result = 0;
    if (count != 0) {
        if (key == -1) {
            char buf[0x20];
            u8* row = &self->mTable[((int)self->field_0x21 + (int)self->field_0x20) * 16];
            u16 v = BdatGetU16ByTableKey(lbl_eu_8050AC70 + 0x21E, lbl_eu_8050AC70 + 0x22D, row[2] + 1);
            sprintf(buf, lbl_eu_8050AC70 + 0x233, row[1]);
            u32 raw = BdatGetU8ByTableKey(lbl_eu_8050AC70 + 0x23C, buf, self->field_0x26);
            s32 prod = (int)(u16)v * (int)(u8)raw;
            if (prod > 0) prod /= 100;
            result = (u16)prod;
        } else {
            char buf[0x20];
            for (u8 i = 0; i < count; i++) {
                u8* row = &self->mTable[i * 16];
                if (key == (int)row[0]) {
                    u16 v = BdatGetU16ByTableKey(lbl_eu_8050AC70 + 0x21E, lbl_eu_8050AC70 + 0x22D, row[2] + 1);
                    sprintf(buf, lbl_eu_8050AC70 + 0x233, row[1]);
                    u32 raw = BdatGetU8ByTableKey(lbl_eu_8050AC70 + 0x23C, buf, self->field_0x26);
                    s32 prod = (int)(u16)v * (int)(u8)raw;
                    if (prod > 0) prod /= 100;
                    result = (u16)prod;
                    break;
                }
            }
        }
    }
    return result;
}

// Arts-list usage-counter bump: pick a row (current cursor row when
// key == -1, else the first row whose id byte equals key), then ask the arts
// manager (vtable +0x27C) to recompute, increment the counter pair indexed
// by row[1] inside the current arts element, mirror the new count into
// row[2], and refresh the character data plus both list panes.
// The whole body is written out per arm (manager fetch included): retail
// duplicates it instead of hoisting a shared prelude/tail.
extern "C" void ArtsTable_bumpUsage(SArts322BC* self, int key) {
    if (key == -1) {
        // base declared ahead of root: MWCC hands out callee-saved regs to
        // pointer locals top-down, so this ordering yields root=r29/base=r30.
        SArtsSubDElem* base;
        SArtsManagerRoot* root = (SArtsManagerRoot*)CtrlObjectParam_GetCharEntry(self->field_0x26);
        root->mObj17C.v157();
        base = &root->mElemsE8[0];
        u8 arts = (u8)CtrlObjectParam_GetCurrentRowKey(root);
        u8* row = &self->mTable[((int)self->field_0x21 + (int)self->field_0x20) << 4];
        // Usage counters live as byte pairs inside the arts element block,
        // indexed by arts*0x49 + row-slot*2; low byte mirrors into the row.
        u8* p = (u8*)base + arts * 0x49 + ((u8)row[1] << 1);
        *p += 1;
        row[2] = *p;
        SysWinLog_ScanGrid((u8*)base);
        CtrlObjectParam_SyncParamFromActor((cf::CtrlObjectParamArtsView*)root);
        artsRenderRow((SArts327B0*)self);
        ArtsTableCursor_refresh((SArts327B0*)self);
    } else {
        SArtsManagerRoot* root;
        SArtsSubDElem* base;
        root = (SArtsManagerRoot*)CtrlObjectParam_GetCharEntry(self->field_0x26);
        root->mObj17C.v157();
        base = &root->mElemsE8[0];
        // Call first, truncate after reading count: retail emits the count
        // load ahead of the arts clrlwi.
        int raw = CtrlObjectParam_GetCurrentRowKey(root);
        u8 count = self->field_0x12C;
        u8 arts = (u8)raw;
        u8* row = 0;
        for (u8 i = 0; i < count; i++) {
            u8* r = &self->mTable[i << 4];
            if (key == (int)r[0]) {
                row = r;
                break;
            }
        }
        u8* p = (u8*)base + arts * 0x49 + ((u8)row[1] << 1);
        *p += 1;
        row[2] = *p;
        SysWinLog_ScanGrid((u8*)base);
        CtrlObjectParam_SyncParamFromActor((cf::CtrlObjectParamArtsView*)root);
        artsRenderRow((SArts327B0*)self);
        ArtsTableCursor_refresh((SArts327B0*)self);
    }
}

// Arts-table row availability check: for key -1 use the current cursor row
// ((0x21 + 0x20) << 4), else scan for the row whose id byte equals key.
// A row qualifies when its state byte (row[2]) is 4 with a clear flag at
// row[8], or 7 with a clear flag at row[9].
int ArtsTable_isRowSelectable(SArts322BC* self, int key) {
    if (key == -1) {
        u8* row = &self->mTable[((int)self->field_0x21 + (int)self->field_0x20) << 4];
        if (row[2] == 4) {
            if (row[8] == 0) return 1;
        } else if (row[2] == 7) {
            if (row[9] == 0) return 1;
        }
    } else {
        u8 count = self->field_0x12C;
        for (u8 i = 0; i < count; i++) {
            u8* row = &self->mTable[i << 4];
            if (key == (int)row[0]) {
                if (row[2] == 4) {
                    if (row[8] == 0) return 1;
                } else if (row[2] == 7) {
                    if (row[9] == 0) return 1;
                }
                break;
            }
        }
    }
    return 0;
}

extern "C" int ArtsTable_isAtTop(SArts3270C* self) {
    int r = 0;
    if (self->field_0x20 == 0 && self->field_0x21 == 0) {
        r = 1;
    }
    return r;
}

extern "C" void ArtsTableCursor_refresh(SArts327B0* self);
extern "C" void artsRenderRow(SArts327B0* self);

// -O4,s: the saturating decrement uses the subic/subfe carry idiom.
extern "C" int ArtsTable_isAtBottom(SArts322BC* self) {
    u8 count = self->field_0x12C;
    if (count > 5) {
        s32 r = 0;
        if (self->field_0x20 == 4 && (s8)self->field_0x21 == (s32)count - 5) {
            r = 1;
        }
        return r;
    }
    // Saturating decrement of the page cursor (count-1 clamped at 0).
    s32 r = 0;
    u8 last = count == 0 ? 0 : count - 1;
    if ((s8)self->field_0x20 == last && !(s8)self->field_0x21) {
        r = 1;
    }
    return r;
}

extern "C" void ArtsTable_resetCursor(SArts327B0* self) {
    self->field_0x20 = 0;
    self->field_0x21 = 0;
    reinterpret_cast<CScrollBar*>(self->field_0x28)->PlaceThumb(0);
    ArtsTableCursor_refresh(self);
    artsRenderRow(self);
}

// Clamp the arts-table cursor: for counts above 5 the cursor sits at row 4
// with a 0x21 offset; otherwise 0x21 is cleared and 0x20 is the count
// saturated at 0. Then refresh the scrollbar and both cursor drivers.
extern "C" void artsCursorFix(SArts322BC* self) {
    u8 count = self->field_0x12C;
    if (count > 5) {
        self->field_0x20 = 4;
        self->field_0x21 = count - 5;
    } else {
        self->field_0x20 = count == 0 ? 0 : count - 1;
        self->field_0x21 = 0;
    }
    reinterpret_cast<CScrollBar*>(self->field_0x28)->PlaceThumb((u16)(s8)self->field_0x21);
    ArtsTableCursor_refresh((SArts327B0*)self);
    artsRenderRow((SArts327B0*)self);
}

// Toggle the 0x12D busy flag and drive both pointees: when clearing, the
// first pane is shown and the second hidden; when setting, the reverse.
extern "C" void ArtsTable_toggleBusyPanes(SArts32888* self) {
    if (self->field_0x12D != 0) {
        self->field_0x12D = 0;
        setPaneVisible((nw4r::lyt::Pane*)self->field_0x08->field_0x10, 1);
        setPaneVisible((nw4r::lyt::Pane*)self->field_0x14->field_0x10, 0);
    } else {
        self->field_0x12D = 1;
        setPaneVisible((nw4r::lyt::Pane*)self->field_0x08->field_0x10, 0);
        setPaneVisible((nw4r::lyt::Pane*)self->field_0x14->field_0x10, 1);
    }
}

// Twin of ArtsLayout_runLayoutOut (same 0x08/0x0C/0x10 driver layout).
extern "C" void ArtsTableLayout_runLayoutOut(SArts31480* self) {
    float duration = lbl_eu_80668648;
    advanceAnimTransform(self->field_0x0C, duration);
    if (self->field_0x23 == 0) {
        self->field_0x08->v11(self->field_0x0C, 0);
        self->field_0x08->v11(self->field_0x10, 1);
        self->field_0x10->field_0x10 = lbl_eu_8066864C;
        self->field_0x24 = 1;
    }
}

// Twin of ArtsLayout_runLayoutIn (identical object view): run the AnimTransform at
// 0x10; when finished, drive both panes via the 0x08 driver, reset the 0x0C
// float, show the 0x14 pane, and mark state 0x24=2 / busy 0x23=1.
extern "C" void ArtsTableLayout_runLayoutIn(SArts3150C* self) {
    float duration = lbl_eu_80668648;
    if (advanceAnimTransform(self->field_0x10, duration) != 0) {
        self->field_0x08->v11(self->field_0x10, 0);
        self->field_0x08->v11(self->field_0x0C, 1);
        self->field_0x0C->field_0x10 = lbl_eu_8066864C;
        setPaneVisible((nw4r::lyt::Pane*)self->field_0x08->field_0x10, 0);
        setPaneVisible((nw4r::lyt::Pane*)self->field_0x14->field_0x10, 1);
        self->field_0x24 = 2;
        self->field_0x23 = 1;
    }
}

// Twin of ArtsLayout_runTwinLayoutOut (offset-0x14 driver layout, state bump to 3).
extern "C" void ArtsTableLayout_runTwinLayoutOut(SArts315BC* self) {
    float duration = lbl_eu_80668648;
    advanceAnimTransform(self->field_0x18, duration);
    if (self->field_0x23 == 0) {
        self->field_0x14->v11(self->field_0x18, 0);
        self->field_0x14->v11(self->field_0x1C, 1);
        self->field_0x1C->field_0x10 = lbl_eu_8066864C;
        self->field_0x24 = 3;
    }
}

// Twin of ArtsLayout_runTwinLayoutIn (identical body, separate retail symbol).
extern "C" void ArtsTableLayout_runTwinLayoutIn(SArts31648* self) {
    float duration = lbl_eu_80668648;
    if (advanceAnimTransform(self->field_0x1C, duration) != 0) {
        self->field_0x14->v11(self->field_0x1C, 0);
        self->field_0x14->v11(self->field_0x18, 1);
        self->field_0x18->field_0x10 = lbl_eu_8066864C;
        setPaneVisible((nw4r::lyt::Pane*)self->field_0x08->field_0x10, 1);
        setPaneVisible((nw4r::lyt::Pane*)self->field_0x14->field_0x10, 0);
        self->field_0x24 = 0;
        self->field_0x23 = 1;
    }
}

// Refresh the arts-table cursor: copy the 5-word label block from
// lbl_eu_8050AC4C, look up the pane for the current 0x20 cursor entry, scale
// its translate by the 0xA6 label pane's scale, then position both the
// 0x08/0x14 sub-panes onto it.
extern "C" void ArtsTableCursor_refresh(SArts327B0* self) {
    nw4r::math::VEC3 pos;
    SArtsB88Block tmp;
    tmp = lbl_eu_8050AC4C;
    nw4r::lyt::Pane* pane =
        (nw4r::lyt::Pane*)((SArts3CObj*)self->field_0x00->field_0x10)
            ->v13((void*)tmp.w[self->field_0x20], 1);
    AccumPaneWorldPos(&pos, pane);
    pane = (nw4r::lyt::Pane*)((SArts3CObj*)self->field_0x00->field_0x10)
               ->v13((void*)(lbl_eu_8050AC70 + 0xA6), 1);
    pos.x *= pane->GetScale().x;
    CBaseCur::SetPaneTranslate((nw4r::lyt::Pane*)self->field_0x08->field_0x10, &pos);
    CBaseCur::SetPaneTranslate((nw4r::lyt::Pane*)self->field_0x14->field_0x10, &pos);
}

// Arts-list 5-row refresh (artsRenderRow). The /100 stays a literal divw
// under -O4,s (retail li r31,0x64; divw). For each visible row: bind the name/sub resources onto per-row
// panes, set the description text, locate which slot holds the row's art
// and re-show the slot pane, draw the level gauge percentage, then position
// three panes from the row's flags.
//
// Residual vs retail (structural 280): decomp frame is 0xF0 vs retail 0xC0
// (48B extra spill slots shift every sp-relative operand), self lands in
// r27 vs retail r25, and _savegpr/_restgpr start at r22 vs r21. Needs
// regalloc-level iteration with full asm visibility.
extern "C" void artsRenderRow(SArts327B0* selfRaw) {
    SArts32C78* self = (SArts32C78*)selfRaw;
    float pos[2];
    char buf[0x60];
    char* base = lbl_eu_8050AC70;

    for (int i = 0; i < 5; i++) {
        sprintf(buf, base + 0x24b, i + 9);
        nw4r::lyt::Pane* pane = ((SArts308B0Layout*)self->field_0x00)->field_0x10->v13(buf, 1);
        if ((s8)self->field_0x21 + i >= (int)self->field_0x12C) {
            // Row past the end of the table: hide its label pane entirely.
            setPaneVisible(pane, 0);
            continue;
        }
        setPaneVisible(pane, 1);
        SArts32C78Row* row = &self->mRows[(s8)self->field_0x21 + i];

        char* name = MakeTplNameSysFile(row->field_0x04);
        void* res = self->field_0x04->vfind(0x74696D67, name, 0);
        if (res != NULL) {
            sprintf(buf, base + 0x257, i + 9);
            PaneSetTexPaletteByName((nw4r::lyt::Layout*)self->field_0x00, (const char*)buf, (TPLPalette*)res);
        }
        char* name2 = MakeTplNameSysFile(row->field_0x06);
        void* res2 = self->field_0x04->vfind(0x74696D67, name2, 0);
        if (res2 != NULL) {
            sprintf(buf, base + 0x264, i + 9);
            PaneSetTexPaletteByName((nw4r::lyt::Layout*)self->field_0x00, (const char*)buf, (TPLPalette*)res2);
        }

        sprintf(buf, base + 0x271, i + 9);
        LayoutSetTextBoxFmtValue(
            (nw4r::lyt::Layout*)self->field_0x00, buf, row->field_0x0C, 0);

        sprintf(buf, base + 0x27e, i + 9);
        nw4r::lyt::Pane* pane2 = ((SArts308B0Layout*)self->field_0x00)->field_0x10->v13(buf, 1);
        if (pane2 != NULL) {
            setPaneVisible(pane2, 0);
        }

        // Find which of the 8 slots currently holds this row's art; when
        // found, re-show the slot pane fetched above.
        void* recv =
            ((SArtsManagerRoot*)CtrlObjectParam_GetCharEntry(self->field_0x26))->mObj17C.v157();
        for (int j = 0; j < 8; j++) {
            int flag = (sArtsDemoModeFlag[0] != 0 || self->field_0x26 != 1);
            int sel = (flag == 0);
            if (row->field_0x00 == getArtsSlotRC(recv, sel, j)) {
                if (pane2 != NULL) {
                    setPaneVisible(pane2, 1);
                }
                break;
            }
        }

        if (row->field_0x02 < 10) {
            u16 msgId = BdatGetU16ByTableKey(
                base + 0x21e, base + 0x22d,
                row->field_0x02 + 1);
            sprintf(buf, base + 0x233, msgId);
            s32 n = BdatGetU8ByTableKey(
                base + 0x23c, buf, self->field_0x26);
            s32 pct = (s32)(u16)msgId * (s32)(n & 0xff);
            if (pct > 0) {
                pct /= 100;
            }
            sprintf(buf, base + 0x28a, i + 9);
            setLayoutTextBoxNumber(
                (nw4r::lyt::Layout*)self->field_0x00, buf, pct);
            char* txt = BdatTouchStringCell(
                base + 0x50, base + 0x5b, 0x18);
            sprintf(buf, base + 0x295, i + 9);
            LayoutSetTextBoxFmtValue(
                (nw4r::lyt::Layout*)self->field_0x00, buf, txt, 0);
        } else {
            sprintf(buf, base + 0x28a, i + 9);
            char* big = BdatTouchStringCell(
                base + 0x50, base + 0x5b, 0x1e);
            LayoutSetTextBoxFmtValue(
                (nw4r::lyt::Layout*)self->field_0x00, buf, big, 0);
            char* mid = BdatTouchStringCell(
                base + 0x50, base + 0x5b, 0x18);
            sprintf(buf, base + 0x295, i + 9);
            LayoutSetTextBoxFmtValue(
                (nw4r::lyt::Layout*)self->field_0x00, buf, mid, 0);
        }

        // Position pass: read y from the first pane, derive x from the row
        // level via the sdata2 scale, write it back to two panes, then apply
        // the flag-selected offsets to a third.
        sprintf(buf, base + 0x2a0, i + 9);
        SArts32C78Pane* p3 =
            (SArts32C78Pane*)(((SArts308B0Layout*)self->field_0x00)
                                  ->field_0x10->v13(buf, 1));
        pos[1] = p3->xy[1];
        pos[0] = (float)(
            (double)(row->field_0x02 - 1) * lbl_eu_8066865C + lbl_eu_8066865C);
        copyVEC2(p3->xy, pos);
        sprintf(buf, base + 0x2ab, i + 9);
        SArts32C78Pane* p4 =
            (SArts32C78Pane*)(((SArts308B0Layout*)self->field_0x00)
                                  ->field_0x10->v13(buf, 1));
        copyVEC2(p4->xy, pos);
        pos[0] = lbl_eu_80668660;
        if (row->field_0x08 != 0) {
            pos[0] = lbl_eu_80668664;
        }
        if (row->field_0x09 != 0) {
            pos[0] = lbl_eu_80668668;
        }
        SArts32C78Pane* p5 =
            (SArts32C78Pane*)(((SArts308B0Layout*)self->field_0x00)
                                  ->field_0x10->v13(buf, 1));
        copyVEC2(p5->xy, pos);
    }
}

// CArtsList constructor (retail symbol __ct__CArtsList). Stores the retail
// vtable, builds the sub-objects, then re-copies the CScrollBar/CArtsInfo/
// CSysWin bodies from stack temps (each copy skips the vtable: a 16-byte
// mem-region copy via __as__17UnkClass_8045F564FRC17UnkClass_8045F564 plus memberwise fields), and
// finally primes the shared arts-mode byte table. Temp buffers are declared
// largest-first so MWCC assigns them the retail stack slots (0x88/0x48/0x8).
CArtsList::CArtsList()
    : field_0x14(0), field_0x18(0), field_0x1C(0), field_0x20(0), field_0x24(0),
      field_0x28(0), field_0x2C(0), field_0x30(0), field_0x31(1), mScrollBar(0),
      mSubObj74(), mSysWinE8(0), mSubObj124(0, 0), mSubObj148(0, 0), mSubObj174(0, 0, 0),
      field_0x2A4(0), field_0x2A5(0), field_0x2A6(0) {
    // Temp CScrollBar(4): copy its body into the member (re-inits the
    // direction byte, which the member ctor above set to 0).
    u8 tempSb[0x40];
    __ct__10CScrollBarFUc((CScrollBar*)tempSb, 4);
    __as__17UnkClass_8045F564FRC17UnkClass_8045F564(&this->mScrollBar.mMemRegion,
                            &((CScrollBar*)tempSb)->mMemRegion);
    this->mScrollBar.mFileHandle = ((CScrollBar*)tempSb)->mFileHandle;
    this->mScrollBar.mAccessor = ((CScrollBar*)tempSb)->mAccessor;
    this->mScrollBar.mLayout = ((CScrollBar*)tempSb)->mLayout;
    this->mScrollBar.mAnimTransform = ((CScrollBar*)tempSb)->mAnimTransform;
    this->mScrollBar.mReady = ((CScrollBar*)tempSb)->mReady;
    this->mScrollBar.mVisible = ((CScrollBar*)tempSb)->mVisible;
    this->mScrollBar.mState = ((CScrollBar*)tempSb)->mState;
    this->mScrollBar.mActive = ((CScrollBar*)tempSb)->mActive;
    this->mScrollBar.mAnimOffset = ((CScrollBar*)tempSb)->mAnimOffset;
    this->mScrollBar.mScrollPosY = ((CScrollBar*)tempSb)->mScrollPosY;
    this->mScrollBar.mScrollRatio = ((CScrollBar*)tempSb)->mScrollRatio;
    this->mScrollBar.mThumbHeight = ((CScrollBar*)tempSb)->mThumbHeight;
    this->mScrollBar.mContentHeight = ((CScrollBar*)tempSb)->mContentHeight;
    this->mScrollBar.mDirection = ((CScrollBar*)tempSb)->mDirection;
    __dt__10CScrollBarFv((CScrollBar*)tempSb, -1);

    // Temp CArtsInfo: copy its body (after the vtable) into the member.
    u8 tempInfo[0x74];
    __ct__CArtsInfo(tempInfo);
    __as__17UnkClass_8045F564FRC17UnkClass_8045F564(&this->mSubObj74.data[4], tempInfo + 4);
        ((SArtsInfoBody*)((u8*)&this->mSubObj74 + 4))->field_0x14 =
            ((SArtsInfoBody*)(tempInfo + 4))->field_0x14;
        ((SArtsInfoBody*)((u8*)&this->mSubObj74 + 4))->field_0x18 =
            ((SArtsInfoBody*)(tempInfo + 4))->field_0x18;
        ((SArtsInfoBody*)((u8*)&this->mSubObj74 + 4))->field_0x1C =
            ((SArtsInfoBody*)(tempInfo + 4))->field_0x1C;
        ((SArtsInfoBody*)((u8*)&this->mSubObj74 + 4))->field_0x20 =
            ((SArtsInfoBody*)(tempInfo + 4))->field_0x20;
        ((SArtsInfoBody*)((u8*)&this->mSubObj74 + 4))->field_0x24 =
            ((SArtsInfoBody*)(tempInfo + 4))->field_0x24;
        ((SArtsInfoBody*)((u8*)&this->mSubObj74 + 4))->field_0x28 =
            ((SArtsInfoBody*)(tempInfo + 4))->field_0x28;
        ((SArtsInfoBody*)((u8*)&this->mSubObj74 + 4))->field_0x2C =
            ((SArtsInfoBody*)(tempInfo + 4))->field_0x2C;
        ((SArtsInfoBody*)((u8*)&this->mSubObj74 + 4))->field_0x30 =
            ((SArtsInfoBody*)(tempInfo + 4))->field_0x30;
        ((SArtsInfoBody*)((u8*)&this->mSubObj74 + 4))->field_0x34 =
            ((SArtsInfoBody*)(tempInfo + 4))->field_0x34;
        ((SArtsInfoBody*)((u8*)&this->mSubObj74 + 4))->field_0x38 =
            ((SArtsInfoBody*)(tempInfo + 4))->field_0x38;
        ((SArtsInfoBody*)((u8*)&this->mSubObj74 + 4))->field_0x3C =
            ((SArtsInfoBody*)(tempInfo + 4))->field_0x3C;
        ((SArtsInfoBody*)((u8*)&this->mSubObj74 + 4))->field_0x40 =
            ((SArtsInfoBody*)(tempInfo + 4))->field_0x40;
        ((SArtsInfoBody*)((u8*)&this->mSubObj74 + 4))->field_0x44 =
            ((SArtsInfoBody*)(tempInfo + 4))->field_0x44;
        ((SArtsInfoBody*)((u8*)&this->mSubObj74 + 4))->field_0x48 =
            ((SArtsInfoBody*)(tempInfo + 4))->field_0x48;
        ((SArtsInfoBody*)((u8*)&this->mSubObj74 + 4))->field_0x49 =
            ((SArtsInfoBody*)(tempInfo + 4))->field_0x49;
        ((SArtsInfoBody*)((u8*)&this->mSubObj74 + 4))->field_0x4C =
            ((SArtsInfoBody*)(tempInfo + 4))->field_0x4C;
        ((SArtsInfoBody*)((u8*)&this->mSubObj74 + 4))->field_0x50 =
            ((SArtsInfoBody*)(tempInfo + 4))->field_0x50;
        ((SArtsInfoBody*)((u8*)&this->mSubObj74 + 4))->field_0x54 =
            ((SArtsInfoBody*)(tempInfo + 4))->field_0x54;
        ((SArtsInfoBody*)((u8*)&this->mSubObj74 + 4))->field_0x55 =
            ((SArtsInfoBody*)(tempInfo + 4))->field_0x55;
        ((SArtsInfoBody*)((u8*)&this->mSubObj74 + 4))->field_0x56 =
            ((SArtsInfoBody*)(tempInfo + 4))->field_0x56;
        ((SArtsInfoBody*)((u8*)&this->mSubObj74 + 4))->field_0x58 =
            ((SArtsInfoBody*)(tempInfo + 4))->field_0x58;
        ((SArtsInfoBody*)((u8*)&this->mSubObj74 + 4))->field_0x5A =
            ((SArtsInfoBody*)(tempInfo + 4))->field_0x5A;
        ((SArtsInfoBody*)((u8*)&this->mSubObj74 + 4))->mCursor.field_0x04 =
            ((SArtsInfoBody*)(tempInfo + 4))->mCursor.field_0x04;
        ((SArtsInfoBody*)((u8*)&this->mSubObj74 + 4))->mCursor.field_0x08 =
            ((SArtsInfoBody*)(tempInfo + 4))->mCursor.field_0x08;
        ((SArtsInfoBody*)((u8*)&this->mSubObj74 + 4))->mCursor.field_0x0C =
            ((SArtsInfoBody*)(tempInfo + 4))->mCursor.field_0x0C;
        ((SArtsInfoBody*)((u8*)&this->mSubObj74 + 4))->mCursor.field_0x10 =
            ((SArtsInfoBody*)(tempInfo + 4))->mCursor.field_0x10;
        ((SArtsInfoBody*)((u8*)&this->mSubObj74 + 4))->mCursor.field_0x14 =
            ((SArtsInfoBody*)(tempInfo + 4))->mCursor.field_0x14;
        ((SArtsInfoBody*)((u8*)&this->mSubObj74 + 4))->mCursor.field_0x15 =
            ((SArtsInfoBody*)(tempInfo + 4))->mCursor.field_0x15;
    __dt__9CArtsInfoFv(tempInfo, -1);

    // Temp CSysWin(0): copy its body (after the vtable) into the member.
    u8 tempW[0x3C];
    __ct__7CSysWinFUc(tempW, 0);
    __as__17UnkClass_8045F564FRC17UnkClass_8045F564(&this->mSysWinE8.mMemRegion, &((CSysWin*)tempW)->mMemRegion);
    this->mSysWinE8.mFileHandle = ((CSysWin*)tempW)->mFileHandle;
    this->mSysWinE8.mTagProcessor = ((CSysWin*)tempW)->mTagProcessor;
    this->mSysWinE8.mArcAccessor = ((CSysWin*)tempW)->mArcAccessor;
    this->mSysWinE8.mLayout = ((CSysWin*)tempW)->mLayout;
    this->mSysWinE8.mAnimTrans = ((CSysWin*)tempW)->mAnimTrans;
    this->mSysWinE8.field_28 = ((CSysWin*)tempW)->field_28;
    this->mSysWinE8.field_2C = ((CSysWin*)tempW)->field_2C;
    this->mSysWinE8.field_30 = ((CSysWin*)tempW)->field_30;
    this->mSysWinE8.field_34 = ((CSysWin*)tempW)->field_34;
    this->mSysWinE8.field_35 = ((CSysWin*)tempW)->field_35;
    this->mSysWinE8.field_36 = ((CSysWin*)tempW)->field_36;
    this->mSysWinE8.field_37 = ((CSysWin*)tempW)->field_37;
    this->mSysWinE8.field_38 = ((CSysWin*)tempW)->field_38;
    this->mSysWinE8.field_39 = ((CSysWin*)tempW)->field_39;
    __dt__7CSysWinFv(tempW, -1);

    sArtsDemoModeFlag[0] = 1;
}

// Retail dtor (stmw/lmw frame, -O4,s); the four member dtors
// (+0xE8 CSysWin, +0x74 CArtsInfo, +0x34 CScrollBar, +0x4 UnkClass_8045F564)
// are emitted by the auto-generated body now that the member classes declare
// their destructors.
CArtsList::~CArtsList() {}

void CArtsList_startLoad(CArtsList* self) {
    void* handle = getHandleMEM2__Q23mtl10MemManagerFv();
    self->field_0x14 = (CFileHandle*)readFile__11CDeviceFileFUlPCcP10IWorkEventii(
        (u32)handle, lbl_eu_8050AC70 + 0x2ba, self, 0, 0);
    self->mScrollBar.loadLayoutArc();
    CArtsInfo_LoadFiles(&self->mSubObj74);
    ((SArtsWinE8*)&self->mSysWinE8)->v32();
}

void artsStepState(CArtsList* self) {
    if (self->field_0x28 != 0 && self->field_0x2C != 0) {
        switch (self->field_0x2C) {
            case 1: CMenuArtsSet_completeLayoutIn(self); break;
            case 2: CMenuArtsSet_enterInteract(self); break;
            case 4: CMenuArtsSet_completeLayoutOut(self); break;
            case 5: CMenuArtsSet_finishCloseAnim((SArts3506C*)self); break;
            case 6: CMenuArtsSet_returnFromInfo(self); break;
        }
        self->field_0x1C->v14(0);
        self->mScrollBar.UpdateDispatch();
        CArtsInfo_UpdateAnims(&self->mSubObj74);
        reinterpret_cast<CSysWin*>(&self->mSysWinE8)->dispatchPhase();
        ArtsLayoutPair_advance((SArts2FDF4*)&self->mSubObj124);
        ArtsLayoutPair_advancePhases((SArts306F0*)&self->mSubObj148);
        ArtsTableLayout_advancePhases((SArts306F0*)((u8*)&self->mSubObj148 + 0x2C));
    }
}

// Draw the arts-selection menu: when the menu is active (0x28) and a state
// machine is running (0x2C), draw the main layout, the draw object at 0x34,
// then - when the arts info and syswin are idle and the list cursor is not
// armed (0x139 clear) - mark the 0x170/0x2A2 flags. The 0x12C layout is
// drawn when the 0x139 cursor byte is set, followed by the 0x148/0x174
// box pair, the arts info, and the syswin.
void CMenuArtsSet_draw(CArtsList* self, nw4r::lyt::DrawInfo* info) {
    if (self->field_0x28 == 0) return;
    if (self->field_0x2C == 0) return;
    drawLayout((nw4r::lyt::Layout*)self->field_0x1C, info, 0, 1);
    self->mScrollBar.draw(info);
    u8 v = 0;
    if (reinterpret_cast<CSysWin*>(&self->mSysWinE8)->isOpened() == 0) {
        if (self->mSubObj124.field_0x15 == 0) {
            if (CArtsInfo_IsStateGE6(&self->mSubObj74) == 0) {
                v = 1;
            }
        }
    }
    self->mSubObj148.field_0x28 = v;
    self->mSubObj174.field_0x12E = v;
    if (self->mSubObj124.field_0x15 != 0) {
        drawLayout((nw4r::lyt::Layout*)self->mSubObj124.field_0x08, info, 0, 1);
    }
    ArtsDrawBox_draw((SArtsDrawBox*)&self->mSubObj148, info);
    ArtsTableDrawBox_draw((SArtsDrawBox*)((u8*)&self->mSubObj148 + 0x2C), info);
    CArtsInfo_Draw(&self->mSubObj74, info);
    reinterpret_cast<CSysWin*>(&self->mSysWinE8)->drawLayout(reinterpret_cast<nw4r::lyt::DrawInfo*>(info));
}

// Teardown: release the file handle, reset the ready flag, drop the three
// sub-objects, release the field_0x1C pane pair (nested null guards reproduce
// retail's double beq on one CR0 test - same D2-inline artifact as COption),
// release the arc accessor, and clean up the mem region / scroll bar /
// info / syswin sub-objects.
void CMenuArtsSet_teardown(CArtsList* self) {
    closeFileHandle(&self->field_0x14);
    self->field_0x28 = 0;
    ArtsLayoutPair_release((SArts2FDF4*)&self->mSubObj124);
    ArtsLayoutPair_releaseSlots((SArts3066C*)&self->mSubObj148);
    ArtsTableLayout_release((SArts3066C*)(SArts322BC*)&self->mSubObj174);
    if (self->field_0x1C != 0) {
        if (self->field_0x1C != 0) {
            self->field_0x1C->v0(1);
        }
        self->field_0x1C = 0;
    }
    releaseArcResourceAccessor(self->field_0x18);
    self->field_0x18 = 0;
    self->mMemRegion.deleteRegion();
    self->mScrollBar.Teardown();
    CArtsInfo_Cleanup(&self->mSubObj74);
    reinterpret_cast<CSysWin*>(&self->mSysWinE8)->termLayout();
}

int CArtsList_isReady(CArtsList* self) {
    if (self->mScrollBar.isVisible() == 0) return 0;
    if (CArtsInfo_GetFlag48(&self->mSubObj74) == 0) return 0;
    if (reinterpret_cast<CSysWin*>(&self->mSysWinE8)->isReady() != 0) {
        return self->field_0x30;
    }
    return 0;
}

int CArtsList::CMenuArtsSet_getReadyFlag() { return field_0x31; }

// True while the arts menu is busy: either the 0x2A1/0x16F flags are set,
// the CArtsInfo or CSysWin sub-object is active, or the raw 0x2A6 flag.
int CMenuArtsSet_isBusy(SArts33888* self) {
    if (self->field_0x2A1 != 0) return 1;
    if (self->field_0x16F != 0) return 1;
    if (CArtsInfo_IsStateGE6(&self->field_0x74) != 0) return 1;
    if (reinterpret_cast<CSysWin*>(&self->field_0xE8)->isOpened() != 0) return 1;
    return self->field_0x2A6;
}

int CMenuArtsSet_isWindowBusy(CArtsList* self) {
    if (CArtsInfo_IsStateGE6(&self->mSubObj74) != 0) return 1;
    if (reinterpret_cast<CSysWin*>(&self->mSysWinE8)->isOpened() != 0) return 1;
    return self->field_0x2A6;
}

u32 CArtsList::CMenuArtsSet_isArtsInfoActive() { return CArtsInfo_IsStateGE6(&mSubObj74); }

void CMenuArtsSet_beginOpen(CArtsList* self) {
    if (self->field_0x2C != 0) return;
    if (reinterpret_cast<CSysWin*>(&self->mSysWinE8)->isOpened() != 0) return;
    self->field_0x2C = 1;
    self->field_0x31 = 0;
    CArtsInfo_ReqState1(&self->mSubObj74);
    CMenuArtsSet_refreshArtsInfo(self);
}

void CMenuArtsSet_beginClose(CArtsList* self) {
    if (self->field_0x2C != 3) return;
    if (reinterpret_cast<CSysWin*>(&self->mSysWinE8)->isOpened() != 0) return;
    self->field_0x2C = 4;
    self->field_0x31 = 0;
    ArtsSubCursor_setVisible((SArtsSub8022FA58*)&self->mSubObj148, 0);
    self->mSubObj174.field_0x22 = 0;
    ArtsTableCursor_refresh((SArts327B0*)(SArts322BC*)&self->mSubObj174);
    CArtsInfo_ReqState4(&self->mSubObj74);
    playUISound(6);
}

// Arts-menu cursor-up handler (list-page twin of CMenuArtsSet_slotCursorUp, gated by
// the 0x2A1 busy flag and the CArtsInfo window instead of the 0x16B armed
// flag): when the arts-info window is running, step it backwards and play
// the cursor sound. Otherwise, when the list busy flag (0x16F) is set and
// the entry pane is visible, clear the 0x148 busy state and re-clamp the
// 0x174 list cursor; when the cursor is already at the top, re-arm the
// entry; otherwise page the list up. When 0x16F is clear, the 0x16A armed
// flag picks the same three-way step on the list cursor. Each step ends
// with the cursor sound (1).
void CMenuArtsSet_cursorUp(CArtsList* self) {
    if (reinterpret_cast<CSysWin*>(&self->mSysWinE8)->isOpened() != 0) return;
    if (self->mSubObj174.field_0x12D != 0) return;
    if (CArtsInfo_IsStateGE6(&self->mSubObj74) != 0) {
        if (CArtsInfo_GetFlag49(&self->mSubObj74) != 0) {
            CArtsInfo_CursorPrev(&self->mSubObj74);
            playUISound(1);
        }
        return;
    }
    if (CArtsInfo_IsStateGE6(&self->mSubObj74) != 0) return;
    if (self->mSubObj148.field_0x27 != 0) {
        if (isPaneVisible((nw4r::lyt::Pane*)((SArts313E0*)&self->mSubObj148)
                              ->field_0x08->field_0x10) != 0) {
            ArtsSubCursor_setVisible((SArtsSub8022FA58*)&self->mSubObj148, 0);
            artsCursorFix((SArts322BC*)&self->mSubObj174);
            self->mSubObj174.field_0x22 = 1;
            ArtsTableCursor_refresh((SArts327B0*)(SArts322BC*)&self->mSubObj174);
            CMenuArtsSet_refreshArtsInfo(self);
        } else if (ArtsTable_isAtTop((SArts3270C*)(SArts322BC*)&self->mSubObj174) != 0) {
            self->mSubObj174.field_0x22 = 0;
            ArtsTableCursor_refresh((SArts327B0*)(SArts322BC*)&self->mSubObj174);
            ArtsSubCursor_setVisible((SArtsSub8022FA58*)&self->mSubObj148, 1);
            CMenuArtsSet_refreshArtsInfo(self);
        } else {
            ArtsTable_scrollUp((SArts322BC*)&self->mSubObj174);
            CMenuArtsSet_refreshArtsInfo(self);
        }
        playUISound(1);
        return;
    }
    if (self->mSubObj148.field_0x22 != 0) {
        ArtsSubCursor_setVisible((SArtsSub8022FA58*)&self->mSubObj148, 0);
        artsCursorFix((SArts322BC*)&self->mSubObj174);
        self->mSubObj174.field_0x22 = 1;
        ArtsTableCursor_refresh((SArts327B0*)(SArts322BC*)&self->mSubObj174);
        CMenuArtsSet_refreshArtsInfo(self);
    } else if (ArtsTable_isAtTop((SArts3270C*)(SArts322BC*)&self->mSubObj174) != 0) {
        self->mSubObj174.field_0x22 = 0;
        ArtsTableCursor_refresh((SArts327B0*)(SArts322BC*)&self->mSubObj174);
        ArtsSubCursor_setVisible((SArtsSub8022FA58*)&self->mSubObj148, 1);
        CMenuArtsSet_refreshArtsInfo(self);
    } else {
        ArtsTable_scrollUp((SArts322BC*)&self->mSubObj174);
        CMenuArtsSet_refreshArtsInfo(self);
    }
    playUISound(1);
}

// Arts-menu cursor-down handler: mirror of CMenuArtsSet_cursorUp, but the arts-info
// window is stepped forwards (CArtsInfo_CursorNext) and the list cursor is
// re-clamped with ArtsTable_resetCursor, checked against the page end with
// ArtsTable_isAtBottom, and paged with ArtsTable_scrollDown.
void CMenuArtsSet_cursorDown(CArtsList* self) {
    if (reinterpret_cast<CSysWin*>(&self->mSysWinE8)->isOpened() != 0) return;
    if (self->mSubObj174.field_0x12D != 0) return;
    if (CArtsInfo_IsStateGE6(&self->mSubObj74) != 0) {
        if (CArtsInfo_GetFlag49(&self->mSubObj74) != 0) {
            CArtsInfo_CursorNext(&self->mSubObj74);
            playUISound(1);
        }
        return;
    }
    if (CArtsInfo_IsStateGE6(&self->mSubObj74) != 0) return;
    if (self->mSubObj148.field_0x27 != 0) {
        if (isPaneVisible((nw4r::lyt::Pane*)((SArts313E0*)&self->mSubObj148)
                              ->field_0x08->field_0x10) != 0) {
            ArtsSubCursor_setVisible((SArtsSub8022FA58*)&self->mSubObj148, 0);
            ArtsTable_resetCursor((SArts327B0*)(SArts322BC*)&self->mSubObj174);
            self->mSubObj174.field_0x22 = 1;
            ArtsTableCursor_refresh((SArts327B0*)(SArts322BC*)&self->mSubObj174);
            CMenuArtsSet_refreshArtsInfo(self);
        } else if (ArtsTable_isAtBottom((SArts322BC*)(SArts322BC*)&self->mSubObj174) != 0) {
            self->mSubObj174.field_0x22 = 0;
            ArtsTableCursor_refresh((SArts327B0*)(SArts322BC*)&self->mSubObj174);
            ArtsSubCursor_setVisible((SArtsSub8022FA58*)&self->mSubObj148, 1);
            CMenuArtsSet_refreshArtsInfo(self);
        } else {
            ArtsTable_scrollDown((SArts322BC*)&self->mSubObj174);
            CMenuArtsSet_refreshArtsInfo(self);
        }
        playUISound(1);
        return;
    }
    if (self->mSubObj148.field_0x22 != 0) {
        ArtsSubCursor_setVisible((SArtsSub8022FA58*)&self->mSubObj148, 0);
        ArtsTable_resetCursor((SArts327B0*)(SArts322BC*)&self->mSubObj174);
        self->mSubObj174.field_0x22 = 1;
        ArtsTableCursor_refresh((SArts327B0*)(SArts322BC*)&self->mSubObj174);
        CMenuArtsSet_refreshArtsInfo(self);
    } else if (ArtsTable_isAtBottom((SArts322BC*)(SArts322BC*)&self->mSubObj174) != 0) {
        self->mSubObj174.field_0x22 = 0;
        ArtsTableCursor_refresh((SArts327B0*)(SArts322BC*)&self->mSubObj174);
        ArtsSubCursor_setVisible((SArtsSub8022FA58*)&self->mSubObj148, 1);
        CMenuArtsSet_refreshArtsInfo(self);
    } else {
        ArtsTable_scrollDown((SArts322BC*)&self->mSubObj174);
        CMenuArtsSet_refreshArtsInfo(self);
    }
    playUISound(1);
}

// Cursor-up handler: when the arts info and syswin are idle and the list is
// armed (0x16B), step the arts cursor backwards via the 0x148 sub-object
// (ArtsSlotCursor_moveUp) or, when the cursor is at the top (pane check) or the
// 0x16A flag is clear, page the list up (ArtsTable_pageUp). Plays the cursor
// sound after any move.
void CMenuArtsSet_slotCursorUp(CArtsList* self) {
    if (CArtsInfo_IsStateGE6(&self->mSubObj74) != 0) return;
    if (self->mSubObj148.field_0x23 == 0) return;
    if (reinterpret_cast<CSysWin*>(&self->mSysWinE8)->isOpened() != 0) return;
    if (self->mSubObj148.field_0x27 != 0) {
        if (isPaneVisible((nw4r::lyt::Pane*)((SArts080C*)self->mSubObj148.field_0x08)->field_0x10) != 0) {
            ArtsSlotCursor_moveUp((SArtsSub8022FA58*)&self->mSubObj148, self->mSubObj148.field_0x27);
            CMenuArtsSet_refreshArtsInfo(self);
            goto L_Sound;
        }
        ArtsTable_pageUp((SArts322BC*)&self->mSubObj174);
        CMenuArtsSet_refreshArtsInfo(self);
        goto L_Sound;
    }
    if (self->mSubObj148.field_0x22 != 0) {
        ArtsSlotCursor_moveUp((SArtsSub8022FA58*)&self->mSubObj148, self->mSubObj174.field_0x12D);
        CMenuArtsSet_refreshArtsInfo(self);
        goto L_Sound;
    }
    ArtsTable_pageUp((SArts322BC*)&self->mSubObj174);
    CMenuArtsSet_refreshArtsInfo(self);
L_Sound:
    playUISound(1);
}

// Cursor-down handler: mirror of CMenuArtsSet_slotCursorUp but stepping the arts cursor
// forwards (ArtsSlotCursor_moveDown) and paging the list down (ArtsTable_pageDown).
void CMenuArtsSet_slotCursorDown(CArtsList* self) {
    if (CArtsInfo_IsStateGE6(&self->mSubObj74) != 0) return;
    if (self->mSubObj148.field_0x23 == 0) return;
    if (reinterpret_cast<CSysWin*>(&self->mSysWinE8)->isOpened() != 0) return;
    if (self->mSubObj148.field_0x27 != 0) {
        if (isPaneVisible((nw4r::lyt::Pane*)((SArts080C*)self->mSubObj148.field_0x08)->field_0x10) != 0) {
            ArtsSlotCursor_moveDown((SArtsSub8022FA58*)&self->mSubObj148, self->mSubObj148.field_0x27);
            CMenuArtsSet_refreshArtsInfo(self);
            goto L_Sound;
        }
        ArtsTable_pageDown((SArts322BC*)&self->mSubObj174);
        CMenuArtsSet_refreshArtsInfo(self);
        goto L_Sound;
    }
    if (self->mSubObj148.field_0x22 != 0) {
        ArtsSlotCursor_moveDown((SArtsSub8022FA58*)&self->mSubObj148, self->mSubObj174.field_0x12D);
        CMenuArtsSet_refreshArtsInfo(self);
        goto L_Sound;
    }
    ArtsTable_pageDown((SArts322BC*)&self->mSubObj174);
    CMenuArtsSet_refreshArtsInfo(self);
L_Sound:
    playUISound(1);
}

// Arts-menu idle/advance handler: dispatch on the sub-object busy states.
// While the syswin or arts-info window is armed, drive its progress;
// otherwise handle the list-cursor busy flags (0x2A1 / 0x16F) or the armed
// window state (0x2A6/0x16B), playing the advance sound (6) on each menu
// step. The sound is shared by the 0x16B body and the 0x2A6-clear path;
// the 0x16B-clear path exits without it (retail L_80235FA0 block).
void CMenuArtsSet_advanceIdle(CArtsList* self) {
    if (reinterpret_cast<CSysWin*>(&self->mSysWinE8)->isOpened() != 0) {
        if (reinterpret_cast<CSysWin*>(&self->mSysWinE8)->isActive() != 0) {
            reinterpret_cast<CSysWin*>(&self->mSysWinE8)->advancePhase3();
            self->field_0x2A6 = 0;
        }
        return;
    }
    if (CArtsInfo_IsStateGE6(&self->mSubObj74) != 0) {
        if (CArtsInfo_GetFlag49(&self->mSubObj74) != 0) {
            CArtsInfo_ToStateA(&self->mSubObj74);
            self->field_0x2A6 = 0;
        }
        return;
    }
    if (self->mSubObj174.field_0x12D != 0) {
        ArtsTable_toggleBusyPanes((SArts32888*)(SArts322BC*)&self->mSubObj174);
        if (self->mSubObj174.field_0x22 != 0) {
            self->mSubObj174.field_0x22 = 0;
            ArtsTableCursor_refresh((SArts327B0*)(SArts322BC*)&self->mSubObj174);
        }
        playUISound(6);
        return;
    }
    if (self->mSubObj148.field_0x27 != 0) {   // absolute 0x16F busy flag
        ArtsBusyPane_toggle((SArts313E0*)&self->mSubObj148);
        if (self->mSubObj174.field_0x22 != 0) {
            ArtsSubCursor_setVisible((SArtsSub8022FA58*)&self->mSubObj148, 0);
        }
        playUISound(6);
        return;
    }
    if (self->field_0x2A6 != 0) {
        if (self->mSubObj148.field_0x23 == 0) {   // absolute 0x16B armed flag
            return;   // 0x16B clear: no sound, exit
        }
        ArtsBusyPane_showPrimary((SArts30D18*)&self->mSubObj148);
            self->mSubObj174.field_0x22 = 1;
            ArtsTableCursor_refresh((SArts327B0*)(SArts322BC*)&self->mSubObj174);
            self->field_0x2A6 = 0;
            CMenuArtsSet_refreshArtsInfo(self);
    }
    playUISound(6);
}

// Arts-menu per-frame advance: dispatch on the help-window / arts-info /
// list busy states and the 0x16A/0x16F mode flags, arming help strings or
// stepping the 0x174 list as each state resolves. The final help-id dispatch
// is duplicated in retail (once for a valid selection key, once for key -1),
// so it is written out twice here instead of being shared.
void artsDriveMenu(CArtsList* self) {
    // NOTE: no persistent SArts34D14* local - retail keeps r29/r30 free for
    // the per-block cursor/matched locals instead of a cached cast pointer.
    if (self->field_0x2C != 3) return;
    if (reinterpret_cast<CSysWin*>(&self->mSysWinE8)->isOpened() != 0) {
        if (reinterpret_cast<CSysWin*>(&self->mSysWinE8)->isActive() != 0) {
            reinterpret_cast<CSysWin*>(&self->mSysWinE8)->advancePhase3();
            self->field_0x2A6 = 0;
            playUISound(3);
        }
        return;
    }
    if (CArtsInfo_IsStateGE6(&self->mSubObj74) != 0) {
        if (CArtsInfo_GetFlag49(&self->mSubObj74) != 0) {
            if (CArtsInfo_IsCursorZero(&self->mSubObj74) != 0) {
                if (((SArts34D14*)self)->field_0x16A != 0) {
                    // Result of artsGetRowPct feeds artsShiftProg directly
                    // (zero-extended into the argument register, no temp).
                    artsShiftProg((SArtsSub8022FA58*)&self->mSubObj124,
                                  artsGetRowPct((SArts322BC*)&self->mSubObj174,
                                                artsReadSelId((SArtsSub8022FA58*)&self->mSubObj148)));
                    ArtsTable_bumpUsage((SArts322BC*)&self->mSubObj174,
                                  artsReadSelId((SArtsSub8022FA58*)&self->mSubObj148));
                } else {
                    artsShiftProg((SArtsSub8022FA58*)&self->mSubObj124,
                                  artsGetRowPct((SArts322BC*)&self->mSubObj174, -1));
                    ArtsTable_bumpUsage((SArts322BC*)&self->mSubObj174, -1);
                }
                playUISound(0x96);
                CArtsInfo_ToStateC(&self->mSubObj74);
            } else {
                playUISound(6);
                CArtsInfo_ToStateA(&self->mSubObj74);
            }
            self->field_0x2A6 = 0;
            self->field_0x31 = 0;
            self->field_0x2C = 6;
        }
        return;
    }
    if (((SArts34C84*)self)->field_0x2A1 != 0) {
        if ((s8)((SArts34D14*)self)->field_0x168 == 4) return;
        ArtsTable_toggleBusyPanes((SArts32888*)(SArts322BC*)&self->mSubObj174);
        self->mSubObj174.field_0x22 = 0;
        ArtsTableCursor_refresh((SArts327B0*)(SArts322BC*)&self->mSubObj174);
        // u32 cursor: masked once on assignment, compared directly after.
        u32 cur = ArtsTable_getSelectedId((SArts322BC*)&self->mSubObj174);
        if (cur == (u8)artsReadSelId((SArtsSub8022FA58*)&self->mSubObj148)) {
            playUISound(6);
            return;
        }
        artsClaimSlot((SArtsSub8022FA58*)&self->mSubObj148, ArtsTable_getSelectedId((SArts322BC*)&self->mSubObj174));
        CMenuArtsSet_refreshArtsInfo(self);
        playUISound(0x15);
        return;
    }
    if (((SArts34C84*)self)->field_0x16F != 0) {   // abs 0x16F busy flag
        if ((s8)((SArts34D14*)self)->field_0x168 == 4) return;
        int matched = 0;
        u32 cur = ArtsTable_getSelectedId((SArts322BC*)&self->mSubObj174);
        if (cur == (u8)artsReadSelId((SArtsSub8022FA58*)&self->mSubObj148) &&
            isPaneVisible(
                (nw4r::lyt::Pane*)((SArts313E0*)&self->mSubObj148)
                    ->field_0x14->field_0x10) == 0) {
            matched = 1;
        }
        if (matched == 0) {
            if (self->mSubObj174.field_0x22 != 0) {
                artsClaimSlot((SArtsSub8022FA58*)&self->mSubObj148,
                              ArtsTable_getSelectedId((SArts322BC*)&self->mSubObj174));
            } else {
                artsClaimSlot((SArtsSub8022FA58*)&self->mSubObj148, -1);
            }
        }
        ArtsBusyPane_toggle((SArts313E0*)&self->mSubObj148);
        self->mSubObj174.field_0x22 = 0;
        ArtsTableCursor_refresh((SArts327B0*)(SArts322BC*)&self->mSubObj174);
        if (matched == 0) {
            CMenuArtsSet_refreshArtsInfo(self);
            playUISound(0x15);
            return;
        }
        playUISound(6);
        return;
    }
    if (self->field_0x2A6 != 0) {
        if (self->mSubObj148.field_0x23 == 0) return;   // abs 0x16B armed flag
        self->mSubObj148.field_0x23 = 0;                // abs 0x16B
        self->mSubObj174.field_0x22 = 1;
        ArtsTableCursor_refresh((SArts327B0*)(SArts322BC*)&self->mSubObj174);
        self->field_0x2A6 = 0;
        artsClaimSlot((SArtsSub8022FA58*)&self->mSubObj148, ArtsTable_getSelectedId((SArts322BC*)&self->mSubObj174));
        CMenuArtsSet_refreshArtsInfo(self);
        playUISound(0x15);
        return;
    }
    if (((SArts34D14*)self)->field_0x16A != 0) {
        // Signed-typed read: retail emits cmpi (not cmpli) for this check.
        if ((s8)((SArts34D14*)self)->field_0x168 == 4) {
            CMenuArtsSet_confirmLockedArt(self);
            return;
        }
        u8 key = artsReadSelId((SArtsSub8022FA58*)&self->mSubObj148);
        if (key == 0) return;
        if (ArtsTable_getRowValue((SArts322BC*)(SArts322BC*)&self->mSubObj174, key) >= 10) {
            char* name = BdatTouchStringCell(lbl_eu_8050AC70 + 0x50,
                                       lbl_eu_8050AC70 + 0x5b, 0x35);
            reinterpret_cast<CSysWin*>(&self->mSysWinE8)->setKindContent(reinterpret_cast<char*>(name), 0);
            reinterpret_cast<CSysWin*>(&self->mSysWinE8)->applyKind(1);
            reinterpret_cast<CSysWin*>(&self->mSysWinE8)->openPhase1();
        } else if (ArtsTable_isRowSelectable((SArts322BC*)(SArts322BC*)&self->mSubObj174, key) != 0) {
            // Selectable entry: pick the help id from the learn-state table.
            u16 id;
            if (sArtsDemoModeFlag[0] == 0 &&
                ArtsCharList_getId((SArtsSub8022FA58*)&self->mSubObj124,
                              ((SArts34D14*)self)->mSubObj124.field_0x14) == 1) {
                id = 0x38;
            } else {
                id = 0x36;
                if (ArtsTable_getRowValue((SArts322BC*)(SArts322BC*)&self->mSubObj174, key) == 7)
                    id = 0x37;
            }
            char* name = BdatTouchStringCell(lbl_eu_8050AC70 + 0x50,
                                       lbl_eu_8050AC70 + 0x5b, id);
            reinterpret_cast<CSysWin*>(&self->mSysWinE8)->setKindContent(reinterpret_cast<char*>(name), 0);
            reinterpret_cast<CSysWin*>(&self->mSysWinE8)->applyKind(1);
            reinterpret_cast<CSysWin*>(&self->mSysWinE8)->openPhase1();
        } else {
            u16 pct = artsGetRowPct((SArts322BC*)(SArts322BC*)&self->mSubObj174, key);
            // Retail computes the advance arm as the fall-through (>=).
            if ((u32)ArtsCharList_getProgress((SArtsSub8022FA58*)&self->mSubObj124) >=
                pct) {
                CArtsInfo_OpenDetail6(&self->mSubObj74);
            } else {
                char* name = BdatTouchStringCell(lbl_eu_8050AC70 + 0x50,
                                           lbl_eu_8050AC70 + 0x5b, 0x34);
                reinterpret_cast<CSysWin*>(&self->mSysWinE8)->setKindContent(reinterpret_cast<char*>(name), 0);
                reinterpret_cast<CSysWin*>(&self->mSysWinE8)->applyKind(1);
                reinterpret_cast<CSysWin*>(&self->mSysWinE8)->openPhase1();
            }
        }
        playUISound(3);
        return;
    }
    // Duplicated tail for key -1 (retail does not share this chain).
    if (ArtsTable_getRowValue((SArts322BC*)(SArts322BC*)&self->mSubObj174, -1) >= 10) {
        char* name = BdatTouchStringCell(lbl_eu_8050AC70 + 0x50,
                                   lbl_eu_8050AC70 + 0x5b, 0x35);
        reinterpret_cast<CSysWin*>(&self->mSysWinE8)->setKindContent(reinterpret_cast<char*>(name), 0);
        reinterpret_cast<CSysWin*>(&self->mSysWinE8)->applyKind(1);
        reinterpret_cast<CSysWin*>(&self->mSysWinE8)->openPhase1();
    } else if (ArtsTable_isRowSelectable((SArts322BC*)(SArts322BC*)&self->mSubObj174, -1) != 0) {
        u16 id;
        if (sArtsDemoModeFlag[0] == 0 &&
            ArtsCharList_getId((SArtsSub8022FA58*)&self->mSubObj124,
                          ((SArts34D14*)self)->mSubObj124.field_0x14) == 1) {
            id = 0x38;
        } else {
            id = 0x36;
            if (ArtsTable_getRowValue((SArts322BC*)(SArts322BC*)&self->mSubObj174, -1) == 7)
                id = 0x37;
        }
        char* name = BdatTouchStringCell(lbl_eu_8050AC70 + 0x50,
                                   lbl_eu_8050AC70 + 0x5b, id);
        reinterpret_cast<CSysWin*>(&self->mSysWinE8)->setKindContent(reinterpret_cast<char*>(name), 0);
        reinterpret_cast<CSysWin*>(&self->mSysWinE8)->applyKind(1);
        reinterpret_cast<CSysWin*>(&self->mSysWinE8)->openPhase1();
    } else {
        u16 pct = artsGetRowPct((SArts322BC*)(SArts322BC*)&self->mSubObj174, -1);
        if ((u32)ArtsCharList_getProgress((SArtsSub8022FA58*)&self->mSubObj124) >= pct) {
            CArtsInfo_OpenDetail6(&self->mSubObj74);
        } else {
            char* name = BdatTouchStringCell(lbl_eu_8050AC70 + 0x50,
                                       lbl_eu_8050AC70 + 0x5b, 0x34);
            reinterpret_cast<CSysWin*>(&self->mSysWinE8)->setKindContent(reinterpret_cast<char*>(name), 0);
            reinterpret_cast<CSysWin*>(&self->mSysWinE8)->applyKind(1);
            reinterpret_cast<CSysWin*>(&self->mSysWinE8)->openPhase1();
        }
    }
    playUISound(3);
}

// Arts-menu advance (page-down): when the menu is idle (0x2A6/0x2A1/0x16F
// clear, CArtsInfo inactive, state 3, 0x124 sub-object armed at 0x16), step
// the list cursor with ArtsCharList_selectNext, drive both list sub-panels with the
// current entry, refresh the driver state, and play the advance sound.
void artsStepAhead(CArtsList* self) {
    if (self->field_0x2A6 != 0) return;
    if (CArtsInfo_IsStateGE6(&self->mSubObj74) != 0) return;
    if (self->field_0x2C != 3) return;
    if (self->mSubObj124.field_0x16 == 0) return;
    if (self->mSubObj174.field_0x12D != 0) return;
    if (self->mSubObj148.field_0x27 != 0) return;
    ArtsCharList_selectNext((SArtsSub8022FA58*)&self->mSubObj124);
    u8 v = ArtsCharList_getId((SArtsSub8022FA58*)&self->mSubObj124, self->mSubObj124.field_0x14);
    artsSyncSlots((SArts308B0View*)&self->mSubObj148, v);
    v = ArtsCharList_getId((SArtsSub8022FA58*)&self->mSubObj124, self->mSubObj124.field_0x14);
    artsFillTable((SArts322BC*)((u8*)&self->mSubObj148 + 0x2C), v);
    CMenuArtsSet_refreshArtsInfo(self);
    playUISound(10);
}

// Twin of artsStepAhead but stepping the cursor with artsGoPrevItem
// (backwards instead of forwards).
// Scroll-down input handler (cursor-back twin of artsStepAhead): guarded by
// the shared busy flags, then step the arts cursor back and refresh both
// list sub-panels with the entry under the new cursor.
void CMenuArtsSet_selectPrevChar(CArtsList* self) {
    if (self->field_0x2A6 != 0) return;
    if (CArtsInfo_IsStateGE6(&self->mSubObj74) != 0) return;
    if (self->field_0x2C != 3) return;
    if (self->mSubObj124.field_0x16 == 0) return;
    if (self->mSubObj174.field_0x12D != 0) return;
    if (self->mSubObj148.field_0x27 != 0) return;
    artsGoPrevItem((SArtsSub8022FA58*)&self->mSubObj124);
    u8 v = ArtsCharList_getId((SArtsSub8022FA58*)&self->mSubObj124, self->mSubObj124.field_0x14);
    artsSyncSlots((SArts308B0View*)&self->mSubObj148, v);
    v = ArtsCharList_getId((SArtsSub8022FA58*)&self->mSubObj124, self->mSubObj124.field_0x14);
    artsFillTable((SArts322BC*)&self->mSubObj174, v);
    CMenuArtsSet_refreshArtsInfo(self);
    playUISound(10);
}

// Arts-list confirm: when the menu is idle and the current arts entry
// (index 0x138) is the locked arts id (1), arm the window if the arts
// manager reports the entry unavailable, else toggle the shared
// availability byte, refresh both list sub-panels with the entry, and play
// the confirm sound.
extern "C" void CMenuArtsSet_armSysWin(CArtsList* self); // defined below (window-arm chain)
void CMenuArtsSet_confirmLockedArt(CArtsList* self) {
    if (self->field_0x2A6 != 0) return;
    if (CArtsInfo_IsStateGE6(&self->mSubObj74) != 0) return;
    if (self->mSubObj174.field_0x12D != 0) return;
    if (self->mSubObj148.field_0x27 != 0) return;
    u8 v = ArtsCharList_getId((SArtsSub8022FA58*)&self->mSubObj124, self->mSubObj124.field_0x14);
    if (v != 1) return;
    if (CtrlObjectParam_GetCurrentRowKey(CtrlObjectParam_GetCharEntry(1)) != 0) return;
    if (ArtsSlot_isWindowEmpty((SArtsSub8022FA58*)&self->mSubObj148) != 0) {
        CMenuArtsSet_armSysWin(self);
        return;
    }
    sArtsDemoModeFlag[0] = (sArtsDemoModeFlag[0] ^ 1) != 0;
    artsSyncSlots((SArts308B0View*)&self->mSubObj148, v);
    artsFillTable((SArts322BC*)&self->mSubObj174, v);
    CMenuArtsSet_refreshArtsInfo(self);
    playUISound(0xA);
}

// Scroll-up input handler: guarded by the shared busy flags plus the
// 0x168 mode / 0x16A flag pair, skip when the current entry is the locked
// arts id while the byte-table gate is clear, then play either the
// list-end sound or the move sound, reset the sub-list, and refresh.
void CMenuArtsSet_resetSubList(CArtsList* self) {
    if (self->field_0x2A6 != 0) return;
    if (CArtsInfo_IsStateGE6(&self->mSubObj74) != 0) return;
    SArts34D14* ext = (SArts34D14*)self;
    if (ext->field_0x16A == 0) return;                 // abs 0x16A
    if ((s8)ext->field_0x168 == 4) return;             // abs 0x168 (signed cmp)
    if (((SArts34C84*)self)->field_0x2A1 != 0) return;
    if (((SArts34C84*)self)->field_0x16F != 0) return;
    u8 v = ArtsCharList_getId((SArtsSub8022FA58*)&self->mSubObj124, self->mSubObj124.field_0x14);
    // Byte-table probe: retail fuses tbl[0] into lbz @sda21(r0), r0 being the
    // dead index register left over from the (v == 1) test.
    if (v == 1 && sArtsDemoModeFlag[0] == 0) return;
    if (artsReadSelId((SArtsSub8022FA58*)&self->mSubObj148) != 0) {
        playUISound(0x77);
    } else {
        playUISound(5);
    }
    artsClaimSlot((SArtsSub8022FA58*)&self->mSubObj148, 0);
    CMenuArtsSet_refreshArtsInfo(self);
}

void CArtsList::CMenuArtsSet_setArmedFlag(u8 val) { mSubObj124.field_0x15 = val; }

u32 CArtsList::CMenuArtsSet_isSlotWindowEmpty() { return ArtsSlot_isWindowEmpty((SArtsSub8022FA58*)&mSubObj148); }

// Arm the CSysWin sub-object with a new label pair and mark the 0x2A6 flag.
// Guarded by the syswin/info busy checks; the string pair is built from the
// shared arts archive path base (lbl_eu_8050AC70).
extern "C" void CMenuArtsSet_armSysWin(CArtsList* self) {
    if (reinterpret_cast<CSysWin*>(&self->mSysWinE8)->isOpened() != 0) return;
    if (CArtsInfo_IsStateGE6(&self->mSubObj74) != 0) return;
    char* base = lbl_eu_8050AC70;
    char* name = BdatTouchStringCell(base + 0x50, base + 0x5b, 0x3c);
    reinterpret_cast<CSysWin*>(&self->mSysWinE8)->setKindContent(reinterpret_cast<char*>(name), 0);
    reinterpret_cast<CSysWin*>(&self->mSysWinE8)->applyKind(1);
    reinterpret_cast<CSysWin*>(&self->mSysWinE8)->openPhase1();
    self->field_0x2A6 = 1;
}

// Arts-menu confirm handler: when the arts-info window is idle, branch on
// the 0x16A armed flag and the 0x2A1 busy flag. In the armed state, check
// whether the current list entry (ArtsTable_getSelectedId on the 0x174 list) matches
// the selected arts entry (artsReadSelId on the 0x148 sub-object) and the
// entry pane is hidden - if so, mark it matched and, when matched, re-arm
// via ArtsBusyPane_toggle + artsClaimSlot. When 0x16A is clear or 0x2A1 is set,
// toggle the 0x2A1 busy state through ArtsTable_toggleBusyPanes on the 0x174 list
// object and either re-arm (entries differ) or re-show the entry pane.
void CMenuArtsSet_confirmSelect(CArtsList* self) {
    u32 cur;
    int matched;
    if (reinterpret_cast<CSysWin*>(&self->mSysWinE8)->isOpened() != 0) return;
    if (CArtsInfo_IsStateGE6(&self->mSubObj74) != 0) return;
    if (self->mSubObj148.field_0x22 != 0 && self->mSubObj174.field_0x12D == 0) {
        if (self->mSubObj148.field_0x20 == 4) return;
        if (self->mSubObj148.field_0x27 != 0) {
            matched = 0;
            cur = ArtsTable_getSelectedId((SArts322BC*)&self->mSubObj174);
            u8 r = artsReadSelId((SArtsSub8022FA58*)&self->mSubObj148);
            if (r == cur) {
                if (isPaneVisible((nw4r::lyt::Pane*)((SArts313E0*)&self->mSubObj148)
                                      ->field_0x14->field_0x10) == 0) {
                    matched = 1;
                }
            }
            if (matched == 0) {
                if (self->mSubObj174.field_0x22 != 0) {
                    artsClaimSlot((SArtsSub8022FA58*)&self->mSubObj148, ArtsTable_getSelectedId((SArts322BC*)&self->mSubObj174));
                } else {
                    artsClaimSlot((SArtsSub8022FA58*)&self->mSubObj148, 0xFFFFFFFF);
                }
            }
            ArtsBusyPane_toggle((SArts313E0*)&self->mSubObj148);
            self->mSubObj174.field_0x22 = 0;
            ArtsTableCursor_refresh((SArts327B0*)(SArts322BC*)&self->mSubObj174);
            if (matched == 0) {
                CMenuArtsSet_refreshArtsInfo(self);
                playUISound(0x15);
            } else {
                playUISound(6);
            }
            return;
        }
        ArtsBusyPane_toggle((SArts313E0*)&self->mSubObj148);
        playUISound(2);
        return;
    }
    if (self->mSubObj174.field_0x12D != 0) {
        if (self->mSubObj148.field_0x20 == 4) return;
        ArtsTable_toggleBusyPanes((SArts32888*)(SArts322BC*)&self->mSubObj174);
        self->mSubObj174.field_0x22 = 0;
        ArtsTableCursor_refresh((SArts327B0*)(SArts322BC*)&self->mSubObj174);
        cur = ArtsTable_getSelectedId((SArts322BC*)&self->mSubObj174);
        u8 r = artsReadSelId((SArtsSub8022FA58*)&self->mSubObj148);
        if (r == cur) {
            playUISound(6);
            return;
        }
        artsClaimSlot((SArtsSub8022FA58*)&self->mSubObj148, ArtsTable_getSelectedId((SArts322BC*)&self->mSubObj174));
        CMenuArtsSet_refreshArtsInfo(self);
        playUISound(0x15);
        return;
    }
    ArtsTable_toggleBusyPanes((SArts32888*)(SArts322BC*)&self->mSubObj174);
    ArtsSubCursor_setVisible((SArtsSub8022FA58*)&self->mSubObj148, 1);
    ArtsSlotCursor_clampFromEmpty((SArtsSub8022FA58*)&self->mSubObj148);
    CMenuArtsSet_refreshArtsInfo(self);
    playUISound(2);
}

int CMenuArtsSet_isListBusy(SArts34C84* self) {
    if (self->field_0x16F != 0) {
        return 1;
    }
    return self->field_0x2A1;
}

int CMenuArtsSet_isLockedArtOpen(SArts34D14* self) {
    if (self->field_0x16A == 0) return 0;
    if ((int)self->field_0x168 != 4) return 0;
    u8 v = ArtsCharList_getId(&self->mSubObj124, self->mSubObj124.field_0x14);
    if (v != 1) return 0;
    return CtrlObjectParam_GetCurrentRowKey((void*)CtrlObjectParam_GetCharEntry(v)) == 0;
}

int CMenuArtsSet_isDemoLockedArt(SArts34D14* self) {
    // Retail loads sArtsDemoModeFlag[0] (fused lbz @sda21(r0); the r0 there is
    // the dead index register, not the subscript - PPC d-forms cannot encode
    // SDA base + variable index). Equivalent since r==1 and tbl[1..7]==0.
    int ret = 0;
    u8 r = ArtsCharList_getId(&self->mSubObj124, self->mSubObj124.field_0x14);
    if (r == 1 && sArtsDemoModeFlag[0] == 0) {
        ret = 1;
    }
    return ret;
}

// Arts-menu action-id query: returns the sound/action id for the current
// list state. The syswin / arts-info busy guards, the 0x139 armed flag, and
// the 0x16F/0x2A1 busy pair each short-circuit to a fixed id; otherwise the
// id depends on whether more than one arts entry is unlocked and on the
// 0x148 sub-object / 0x168 mode / 0x16A flag state.
extern "C" u32 CMenuArtsSet_getHelpMode(CArtsList* self) {
    if (reinterpret_cast<CSysWin*>(&self->mSysWinE8)->isOpened() != 0) return 0;
    if (CArtsInfo_IsStateGE6(&self->mSubObj74) != 0) return 0;
    if (self->mSubObj124.field_0x15 != 0) return 0x42;
    if (CMenuArtsSet_isListBusy((SArts34C84*)self) != 0) return 0x40;
    int more = code80135FDC_getByte_64077() > 1;
    if (self->mSubObj148.field_0x22 != 0) {          // abs 0x16A
        if (artsReadSelId((SArtsSub8022FA58*)&self->mSubObj148) == 0) {
            u8 r = 0x49;
            if (more) r = 0x48;
            return r;
        }
        if ((int)((SArts34D14*)self)->field_0x168 != 4) {  // abs 0x168
            if (CMenuArtsSet_isDemoLockedArt((SArts34D14*)self) != 0) {
                u8 r = 0x47;
                if (more) r = 0x46;
                return r;
            }
            u8 r = 0x41;
            if (more) r = 0x3F;
            return r;
        }
        if (CMenuArtsSet_isLockedArtOpen((SArts34D14*)self) != 0) {
            u8 r = 0x44;
            if (more) r = 0x43;
            return r;
        }
        u8 r = 0x00;
        if (more) r = 0x45;
        return r;
    }
    u8 r = 0x47;
    if (more) r = 0x46;
    return r;
}

// Confirm/advance handler: when idle and armed, run the layout-in animation
// driver at field_0x20 via the field_0x1C interface, mark state 2, refresh
// both list sub-panels with the current entry, scroll the bar in with the
// 3-float init vector, then kick the scrollbar tick.
void CMenuArtsSet_completeLayoutIn(CArtsList* self) {
    if (advanceAnimTransform(self->field_0x20, lbl_eu_80668648) == 0) return;
    self->field_0x1C->v11(self->field_0x20, 0);
    self->field_0x1C->v11(self->field_0x24, 1);
    self->field_0x2C = 2;
    u8 v = ArtsCharList_getId((SArtsSub8022FA58*)&self->mSubObj124, self->mSubObj124.field_0x14);
    artsFillTable((SArts322BC*)&self->mSubObj174, v);
    float vec[3];
    vec[0] = lbl_eu_80668678;
    vec[1] = lbl_eu_8066867C;
    vec[2] = lbl_eu_8066864C;
    reinterpret_cast<CScrollBar*>((u8*)&self->mScrollBar)->InitRootPane(reinterpret_cast<const nw4r::math::VEC3*>(vec));
    reinterpret_cast<CScrollBar*>((u8*)&self->mScrollBar)->requestScrollIn();
}

extern "C" void CMenuArtsSet_enterInteract(CArtsList* self) {
    if (advanceAnimTransform(self->field_0x24, lbl_eu_80668648) != 0) {
        self->field_0x2C = 3;
        self->field_0x31 = 1;
        ArtsSubCursor_setVisible((SArtsSub8022FA58*)&self->mSubObj148, 1);
        CMenuArtsSet_refreshArtsInfo(self);
    }
}

// State-4 handler: when the layout-in animation has finished, drive both
// sub-panes via the field_0x1C interface, advance the state to 5, reset the
// 0x148 sub-object, and request the scroll bar scroll-out.
void CMenuArtsSet_completeLayoutOut(CArtsList* self) {
    float duration = lbl_eu_80668648;
    if (AnimRewindFrame(self->field_0x24, duration) != 0) {
        self->field_0x1C->v11(self->field_0x24, 0);
        self->field_0x1C->v11(self->field_0x20, 1);
        self->field_0x2C = 5;
        ArtsSubCursor_setVisible((SArtsSub8022FA58*)&self->mSubObj148, 0);
        self->mScrollBar.requestScrollOut();
    }
}

extern "C" void CMenuArtsSet_finishCloseAnim(SArts3506C* self) {
    if (AnimRewindFrame(self->mAnim20, lbl_eu_80668648) != 0) {
        self->field_0x31 = 1;
        self->field_0x2C = 0;
    }
}

extern "C" void CMenuArtsSet_returnFromInfo(CArtsList* self) {
    if (CArtsInfo_GetFlag49(&self->mSubObj74) != 0) {
        self->field_0x31 = 1;
        self->field_0x2C = 3;
        CMenuArtsSet_refreshArtsInfo(self);
    }
}

extern "C" void CMenuArtsSet_markLayoutsReady(SArts35108* self) {
    if (self->field_0x1C != 0) {
        self->field_0x30 = 1;
        self->field_0x28 = 1;
    }
}

// Refresh the arts-info fields from the active source: pick the selected
// arts id via the 0x124 list cursor, then - depending on the 0x16A/0x16F/
// 0x196/0x2A6 mode flags - read the id/key/percent triple either from the
// 0x148 sub-object or the 0x174 arts table, and store all four into the
// CArtsInfo before refreshing its cursor driver.
extern "C" void CMenuArtsSet_refreshArtsInfo(CArtsList* self) {
    u8 id = 0;   // arts-table row id / sub-object result
    u8 key = 0;  // key byte fed to ArtsTable_getRowValue/artsGetRowPct
    u16 pct = 0; // percent value from artsGetRowPct
    u8 sel = ArtsCharList_getId((SArtsSub8022FA58*)&self->mSubObj124,
                           self->mSubObj124.field_0x14);
    if (self->mSubObj148.field_0x22 != 0) {
        if (self->mSubObj148.field_0x27 != 0) {
            if (self->mSubObj174.field_0x22 != 0) {
                id = ArtsTable_getSelectedId((SArts322BC*)&self->mSubObj174);
                key = ArtsTable_getRowValue((SArts322BC*)&self->mSubObj174, -1);
                pct = artsGetRowPct((SArts322BC*)&self->mSubObj174, -1);
            } else {
                id = artsReadSelId((SArtsSub8022FA58*)&self->mSubObj148);
                key = artsAvailFlag((SArtsSub8022FA58*)&self->mSubObj148);
            }
        } else if (self->field_0x2A6 != 0) {
            id = ArtsTable_getSelectedId((SArts322BC*)&self->mSubObj174);
            key = ArtsTable_getRowValue((SArts322BC*)&self->mSubObj174, -1);
            pct = artsGetRowPct((SArts322BC*)&self->mSubObj174, -1);
        } else {
            id = artsReadSelId((SArtsSub8022FA58*)&self->mSubObj148);
            key = ArtsTable_getRowValue((SArts322BC*)&self->mSubObj174, id);
            pct = artsGetRowPct((SArts322BC*)&self->mSubObj174, id);
        }
    } else {
        id = ArtsTable_getSelectedId((SArts322BC*)&self->mSubObj174);
        key = ArtsTable_getRowValue((SArts322BC*)&self->mSubObj174, -1);
        pct = artsGetRowPct((SArts322BC*)&self->mSubObj174, -1);
    }
    CArtsInfo_SetCharId(&self->mSubObj74, sel);
    CArtsInfo_SetField55(&self->mSubObj74, id);
    CArtsInfo_SetField56(&self->mSubObj74, key);
    CArtsInfo_SetLevel58(&self->mSubObj74, pct);
    CArtsInfo_RefreshIfReady(&self->mSubObj74);
    artsRenderRow((SArts327B0*)(SArts322BC*)&self->mSubObj174);
}

// CArtsList::OnFileEvent (retail OnFileEvent__9CArtsListFP10CEventFile):
// file-load completion for the arts list. Resizes the mem region, attaches
// the loaded arc data to the accessor, binds the layout/anims, re-inits the
// three sub-objects (0x124 list, 0x148 layout pair, 0x174 arts table) from
// stack temps (declared largest-first so MWCC assigns the retail slots
// 0x60/0x38/0xC/0x8), then drops the file handle and frees the region guard.
bool CArtsList::OnFileEvent(CEventFile* pEventFile) {
    if (field_0x14 == pEventFile->mFileHandle) {
    mMemRegion.createRegion((int)getHandleMEM2__Q23mtl10MemManagerFv(), 0x16000,
                            lbl_eu_8050AC70 + 0x2CF, 0);
    Class_8045F858 regionGuard(&mMemRegion);
    u8* fileData = field_0x14->mData;
    field_0x14->mData = 0;
    setMemInitFlag__Q23mtl10MemManagerFb(false);
    field_0x18 = CLibLayout::createArcResourceAccessor();
    Attach__Q34nw4r3lyt19ArcResourceAccessorFPvPCc(field_0x18, fileData,
                                                   lbl_eu_8050AC70 + 0x2D9);
    buildLayout((nw4r::lyt::Layout**)&field_0x1C, field_0x18,
                  lbl_eu_8050AC70 + 0x2DD);
    bindLayoutAnimTransform((nw4r::lyt::Layout*)field_0x1C, &field_0x20, field_0x18,
                  lbl_eu_8050AC70 + 0x2F7);
    bindLayoutAnimTransform((nw4r::lyt::Layout*)field_0x1C, &field_0x24, field_0x18,
                  lbl_eu_8050AC70 + 0x314);
    SArts3CObj* pane = field_0x1C->field_0x10;
    u32 fontHandle = ((SDevFontV*)getFontInfo__11CDeviceFontFUlPQ34nw4r3lyt6Layout(
                          1, (nw4r::lyt::Layout*)field_0x1C))
                         ->v7();
    ApplyFontToTxtPanes((nw4r::lyt::Pane*)pane, (const nw4r::ut::Font*)fontHandle);
    void* text = CUICfManager_getPackedFont9C();
    if (text != 0) {
        setLayoutTextBoxFont((nw4r::lyt::Layout*)field_0x1C,
                                                lbl_eu_8050AC70 + 0x6F, (u32)text);
        setLayoutTextBoxFont((nw4r::lyt::Layout*)field_0x1C,
                                                lbl_eu_8050AC70 + 0x336, (u32)text);
        setLayoutTextBoxFont((nw4r::lyt::Layout*)field_0x1C,
                                                lbl_eu_8050AC70 + 0x33F, (u32)text);
        setLayoutTextBoxFont((nw4r::lyt::Layout*)field_0x1C,
                                                lbl_eu_8050AC70 + 0x348, (u32)text);
        setLayoutTextBoxFont((nw4r::lyt::Layout*)field_0x1C,
                                                lbl_eu_8050AC70 + 0x351, (u32)text);
        setLayoutTextBoxFont((nw4r::lyt::Layout*)field_0x1C,
                                                lbl_eu_8050AC70 + 0x35A, (u32)text);
        setLayoutTextBoxFont((nw4r::lyt::Layout*)field_0x1C,
                                                lbl_eu_8050AC70 + 0x100, (u32)text);
        setLayoutTextBoxFont((nw4r::lyt::Layout*)field_0x1C,
                                                lbl_eu_8050AC70 + 0x363, (u32)text);
        setLayoutTextBoxFont((nw4r::lyt::Layout*)field_0x1C,
                                                lbl_eu_8050AC70 + 0x36C, (u32)text);
        setLayoutTextBoxFont((nw4r::lyt::Layout*)field_0x1C,
                                                lbl_eu_8050AC70 + 0x375, (u32)text);
        setLayoutTextBoxFont((nw4r::lyt::Layout*)field_0x1C,
                                                lbl_eu_8050AC70 + 0x37E, (u32)text);
        setLayoutTextBoxFont((nw4r::lyt::Layout*)field_0x1C,
                                                lbl_eu_8050AC70 + 0x387, (u32)text);
    }
    field_0x1C->v11(field_0x24, 0);
    field_0x1C->v11(field_0x20, 1);
    field_0x1C->v14(0);
    mSubObj124 = SArtsSub8022FA58Short((u32)field_0x1C, (u32)field_0x18);
    artsRebuildUI((SArtsSub8022FA58*)&mSubObj124);
    mSubObj148 = SArts3042C((u32)field_0x1C, (u32)field_0x18);
    ArtsLayoutPair_loadSlots((SArts304C4*)&mSubObj148);
    artsSyncSlots((SArts308B0View*)&mSubObj148,
                  ArtsCharList_getId((SArtsSub8022FA58*)&mSubObj124, mSubObj124.field_0x14));
    mSubObj174 = SArts319AC((u32)field_0x1C, (u32)field_0x18, (u32)&mScrollBar);
    ArtsTableLayout_load((SArts304C4*)&mSubObj174);
    CMenuArtsSet_markLayoutsReady((SArts35108*)this);
    field_0x14 = 0;
    mMemRegion.validateHeap();
    return 1;
    }
    return 0;
}

