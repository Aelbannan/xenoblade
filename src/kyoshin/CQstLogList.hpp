#pragma once

#include <types.h>
#include "kyoshin/code_80135FDC_Bdat.hpp"
#include <nw4r/lyt.h>
#include "kyoshin/CBaseCur.hpp"
#include "kyoshin/CScrollBar.hpp"
#include "monolib/lib/UnkClass_8045F564.hpp"
#include "monolib/work/IWorkEvent.hpp"
#include "monolib/work/CEventFile.hpp"

#include "kyoshin/CSortMenu.hpp"
#include "kyoshin/code_80135FDC_Lyt.hpp"
#include "kyoshin/QstData.hpp"
#include "kyoshin/CQuestWindow_Pane.hpp"
#include "kyoshin/code_80135FDC_State.hpp"
#include "kyoshin/CUICfManager_Res.hpp"

namespace nw4r { namespace math { struct VEC3; } }

/* 0x22-byte quest-log entry record, copied wholesale by QstLogList_CopyEntry.
   Layout matches the retail copy: 2 head bytes + 4x8-byte word loop. */
struct CQstLogListEntry {
    s8 mField0;          // 0x0 (retail li r5,-1 store - signed)
    u8 mField1;          // 0x1
    u8 mData[0x20];      // 0x2..0x21
};

/* Quest log list widget. Owned by CMenuQuestLog.
   Sorting: mSortEnabled (0x179) gates sort/filter logic,
   mSortDescending (0x17A) flips the sort direction arrow.

   Layout (from retail ASM):
     +0x00: vtable pointer
     +0x04: UnkClass_8045F564 sub-object (0x10 bytes)
     +0x14: CFileHandle* (readFile result from QstLogList_LoadArc)
     +0x18: ArcResourceAccessor* (texture lookups in QstLogList_DrawRow)
     +0x1C: nw4r layout
     +0x20/+0x24: animation transforms
     +0x28: CCur18 cursor sub-object (0x18 bytes)
     +0x40: CScrollBar (0x40 bytes)
     +0x80: CSortMenu (0xF0 bytes)
     +0x170: layout-loaded flag
     +0x182: quest-info buffer (8x0x400 array + 6-byte tail, 0x2006 bytes) */
// Overlay structs for the 'timg' message resource consumed by OnFileEvent:
// the resource object holds a chain whose first field points at the u16
// row/column pair used to size the quest-log panes.
struct CQstLogListCoords {
    u16 c0;  // 0x0 column
    u16 c2;  // 0x2 row
};
struct CQstLogListMsgChain {
    CQstLogListCoords* pCoords;  // 0x0
};
struct CQstLogListMsgObj {
    u8 gap[0x8];
    CQstLogListMsgChain* chain;  // 0x8
};

class CQstLogList : public IWorkEvent {
public:
    CQstLogList(u16 arg2);
    virtual ~CQstLogList();
    virtual bool OnFileEvent(CEventFile* event);   // file-load completion callback

    int IsSortEnabled();
    int IsSortDescending();
    int ScrollFlag();
    void LoadArc();
    void FrameUpdate();
    void Draw(nw4r::lyt::DrawInfo* drawInfo);
    void Teardown();
    void BeginSortOpen();
    void EndSortMode(int flag);
    void ScrollUp();
    void ScrollDown();
    void PageUp();
    void PageDown();
    void ToggleSort();
    void ConfirmSort(int flag);
    void ApplySortSelection();
    u32 GetSelectedQuestLo();
    void StepOpenAnim();
    void StepSortAnim();
    void RewindSortAnim();
    void RewindOpenAnim();
    void ShowOpenAnim();
    void ShowSortAnim();
    void RefreshRows();
    void DrawRow(const char* name, int questId, int index, int mode, u8 a6, u8 a7);
    void ClearRows();
    void MoveCursor();
    void FillSortMenu();
    void RefreshHeader();

    UnkClass_8045F564 mMemRegion;    // 0x04..0x13
    CFileHandle* mFileHandle;        // 0x14 - loaded arc file handle
    nw4r::lyt::ArcResourceAccessor* mArcResAcc;  // 0x18 - resource accessor
    nw4r::lyt::Layout* mpLayout;     // 0x1C - layout (LayoutSetTextBoxText target)
    nw4r::lyt::AnimTransform* mpAnim0;  // 0x20 - animation transform 0
    nw4r::lyt::AnimTransform* mpAnim1;  // 0x24 - animation transform 1
    CCur18 mCur18;                   // 0x28..0x3F
    CScrollBar mScrollBar;           // 0x40..0x7F - quest list scroll bar
    CSortMenu mSortMenu;             // 0x80..0x16F
    u8 field_0x170;                  // 0x170 - layout loaded flag
    s32 field_0x174;                 // 0x174 - mode (0/2/5 set by anim checks)
    u8 field_0x178;                  // 0x178
    u8 mSortEnabled;                 // 0x179 - enables sorting/filtering (init=1)
    u8 mSortDescending;              // 0x17A - sort direction, 0=asc, 1=desc (init=0)
    s8 field_0x17B;                  // 0x17B - index byte (signed)
    s8 field_0x17C;                  // 0x17C - index byte (signed)
    s8 field_0x17D;                  // 0x17D - current row index byte (signed)
    s16 field_0x17E;                 // 0x17E - scroll position (signed halfword)
    u16 field_0x180;                 // 0x180 - quest-log list id (ctor arg)
    QstData mQstData;                // 0x182..0x2187 quest-info table (0x2006)
};

extern "C" CQstLogListEntry* QstLogList_CopyEntry(CQstLogListEntry* pDst, const CQstLogListEntry* pSrc);
// CCur18 per-frame update


// BDAT helpers / msg-manager imports (C-ABI retail symbols).
extern "C" u32 Bdat_GetMaxRow_B1EC(void*);              // BDAT row count
extern void* lbl_eu_806640A0;                      // BDAT table pointer (.sbss)

// Layout/text helpers with unmangled retail names (declared here; the
// code_80135FDC.hpp set is co-included for BdatGetPtrDirect/LayoutSetTextBoxFmtValue).
extern "C" u32 CtrlRemote_TouchBitByArg(u32);

// C++ free functions - MWCC mangles these to the retail names
// (drawLayout__FPQ34nw4r3lyt6LayoutPQ34nw4r3lyt8DrawInfoii,
// playUISound__FUl).

// Shared data pools (retail names; global scope - no C++ mangling).
extern char lbl_eu_80509AB4[];  // string pool (split1 .rodata)
extern char sQuestLogEntries[];   // 0x22-byte quest text entry table
extern s32 sQuestListScrollBase;      // quest-list page/scroll boundary (.sbss)
extern u32 sQuestEntryCount;      // quest-list init flag (.sbss)
extern u32 sSelectedQuestId;      // current quest id (.sbss)
extern u32 sSelectedQuestIndex;      // current quest index (.sbss)
extern u32 sSelectedQuestResource;      // current quest resource (.sbss)
extern u32 lbl_eu_80662860;      // quest pane color (.sbss)
extern u8* lbl_eu_806640A8;      // BDAT table pointer (.sbss)
extern u32 lbl_eu_80573D18[];    // quest name table (split1 .rodata)
// const routes into readonly sdata2 - MWCC hoists the lfs above the frame
// stores (MWCC_CASES:8787); plain extern float schedules it late.
extern const float lbl_eu_80668584;  // anim frame target (sda21)
// Scroll-bar init position constants (sda21 floats).
extern const float lbl_eu_80668578;
extern const float lbl_eu_8066857C;
extern const float lbl_eu_80668580;
