#pragma once

#include <types.h>
#include "kyoshin/code_80135FDC_Bdat.hpp"
#include <nw4r/lyt.h>
#include "kyoshin/CBaseCur.hpp"
#include "kyoshin/CScrollBar.hpp"
#include "monolib/lib/UnkClass_8045F564.hpp"
#include "monolib/work/IWorkEvent.hpp"
#include "monolib/work/CEventFile.hpp"

// The shared header's CSortMenu is a mirror of a class whose ctor/dtor are
// C-linkage free functions (see CSortMenu.cpp); embedding it by value would
// reference a ctor symbol that does not exist in retail. It is renamed aside
// here and a CSortMenu with the right dtor symbol (__dt__9CSortMenuFv) is
// declared below. Remove once CSortMenu is a real class.
#define CSortMenu CSortMenuSharedView
#include "kyoshin/CSortMenu.hpp"  // CScrollBarData (0x40-byte scrollbar), CScrollBar_isVisible
#undef CSortMenu

namespace nw4r { namespace math { struct VEC3; } }

/* 0x22-byte quest-log entry record, copied wholesale by QstLogList_CopyEntry.
   Layout matches the retail copy: 2 head bytes + 4x8-byte word loop. */
struct CQstLogListEntry {
    s8 mField0;          // 0x0 (retail li r5,-1 store - signed)
    u8 mField1;          // 0x1
    u8 mData[0x20];      // 0x2..0x21
};

// 8-byte quest-info record (mirror of QstInfo in CMenuQstCnt.hpp).
struct CQstLogListQstInfo {
    u16 f0;              // 0x0 quest id
    u8  f2;              // 0x2
    u8  f3;              // 0x3
    u8  f4;              // 0x4
    u8  f5;              // 0x5
    u8  f6;              // 0x6
    u8  f7;              // 0x7
};

struct CQstLogListQstData;
struct CSortMenu;
// Imports used by the member wrappers below (retail C-linkage names).
extern "C" void QstCnt_InitRecords_6FAC(u8*);               // QstData construct (8x0x400)
extern "C" void copyQstInfo(u8*, const u8*);      // 8-byte QstInfo copy
extern "C" void __destroy_arr(void*, void*, int, int);
extern "C" void* __dt__80227030(void*, int);      // 0x400-byte QstInfo element dtor
extern "C" void __ct__CSortMenu(CSortMenu*);
extern "C" CScrollBar* copyScrollBarData(CScrollBar* dst, const CScrollBar* src);  // CScrollBar copy (out of line)
extern "C" void __as__17UnkClass_8045F564FRC17UnkClass_8045F564(void* self, const void* src);

// Quest-info buffer maintained by QstCnt_InitRecords_6FAC (mirror of QstData in
// CMenuQstCnt.cpp): an 8x0x400 array whose elements are destroyed via
// __dt__80227030, plus a 6-byte tail.
struct CQstLogListQstData {
    u8   mList[8 * 0x400];  // 0x0000 (0x2000 bytes)
    u16  field_2000;        // 0x2000
    s16  field_2002;        // 0x2002
    u8   field_2004;        // 0x2004
    u8   field_2005;        // 0x2005

    // QstData is its own class in retail (ctor QstCnt_InitRecords_6FAC, element
    // dtor __dt__80227030 inlined here with the member-array null guard).
    CQstLogListQstData() { QstCnt_InitRecords_6FAC(&mList[0]); }
    ~CQstLogListQstData() {
        __destroy_arr(&mList[0], (void*)&__dt__80227030, 8, 0x400);
    }
    // Record-wise copy (copyQstInfo per 8-byte record, then the 6-byte tail).
#pragma always_inline on
    CQstLogListQstData& operator=(const CQstLogListQstData& rhs) {
        u8* pDst = &mList[0];
        const u8* pSrc = &rhs.mList[0];
        do {
            copyQstInfo(pDst, pSrc);
            pDst += 8;
            pSrc += 8;
        } while (pDst < &mList[8 * 0x400]);
        field_2000 = rhs.field_2000;
        field_2002 = rhs.field_2002;
        field_2004 = rhs.field_2004;
        field_2005 = rhs.field_2005;
        return *this;
    }
#pragma always_inline off
};

// Mirror of the CSortMenu body (0xF0 bytes) for raw field copies in the
// constructor (the real CSortMenu class cannot be embedded - include
// conflict). Pointer fields are u32 because the region is opaque storage.
struct CSortMenu {
    u32 mVtbl;                 // +0x00
    u8  mUnk04[0x10];          // +0x04 UnkClass_8045F564
    u32 mFileHandle;           // +0x14
    u32 mArcResAcc;            // +0x18
    u32 mpLayout;              // +0x1C
    u32 mpAnimTrans0;          // +0x20
    u32 mpAnimTrans1;          // +0x24
    u8  field_0x28;            // +0x28
    u8  field_0x29;            // +0x29
    u8  field_0x2A;            // +0x2A
    u8  field_0x2B;            // +0x2B
    CScrollBarData mScrollBar; // +0x2C (0x40 bytes)
    s32 mArray[32];            // +0x6C (0x80 bytes)
    u8  mCount;                // +0xEC
    u8  mPage;                 // +0xED
    u8  mSubPage;              // +0xEE

    // The ctor goes through the retail C-linkage function __ct__CSortMenu;
    // operator= is the member-wise copy that skips the vtable word.
    CSortMenu() { __ct__CSortMenu(this); }
    ~CSortMenu();  // retail __dt__9CSortMenuFv (CSortMenu.o)
#pragma always_inline on
    CSortMenu& operator=(const CSortMenu& rhs) {
        __as__17UnkClass_8045F564FRC17UnkClass_8045F564(&mUnk04[0], &rhs.mUnk04[0]);
        mFileHandle = rhs.mFileHandle;
        mArcResAcc = rhs.mArcResAcc;
        mpLayout = rhs.mpLayout;
        mpAnimTrans0 = rhs.mpAnimTrans0;
        mpAnimTrans1 = rhs.mpAnimTrans1;
        field_0x28 = rhs.field_0x28;
        field_0x29 = rhs.field_0x29;
        field_0x2A = rhs.field_0x2A;
        field_0x2B = rhs.field_0x2B;
        copyScrollBarData(reinterpret_cast<CScrollBar*>(&mScrollBar), reinterpret_cast<const CScrollBar*>(&rhs.mScrollBar));
        // 0x80-byte block struct assign: MWCC emits its canonical
        // lwz/lwzu + stw/stwu mtctr loop for whole-struct copies.
        struct SortMenuRegion { u8 bytes[0x80]; };
        *reinterpret_cast<SortMenuRegion*>(reinterpret_cast<char*>(this) + 0x6C) =
            *reinterpret_cast<const SortMenuRegion*>(reinterpret_cast<const char*>(&rhs) + 0x6C);
        mCount = rhs.mCount;
        mPage = rhs.mPage;
        mSubPage = rhs.mSubPage;
        return *this;
    }
#pragma always_inline off
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
    CSortMenu mSortMenuData;  // 0x80..0x16F CSortMenu (mirror storage)
    u8 field_0x170;                  // 0x170 - layout loaded flag
    u8 pad_0171[0x174 - 0x171];      // 0x171..0x173
    s32 field_0x174;                 // 0x174 - mode (0/2/5 set by anim checks)
    u8 field_0x178;                  // 0x178
    u8 mSortEnabled;                 // 0x179 - enables sorting/filtering (init=1)
    u8 mSortDescending;              // 0x17A - sort direction, 0=asc, 1=desc (init=0)
    s8 field_0x17B;                  // 0x17B - index byte (signed)
    s8 field_0x17C;                  // 0x17C - index byte (signed)
    s8 field_0x17D;                  // 0x17D - current row index byte (signed)
    s16 field_0x17E;                 // 0x17E - scroll position (signed halfword)
    u16 field_0x180;                 // 0x180 - quest-log list id (ctor arg)
    CQstLogListQstData mQstData;     // 0x182..0x2187 quest-info buffer (0x2006)
};

extern "C" CQstLogListEntry* QstLogList_CopyEntry(CQstLogListEntry* pDst, const CQstLogListEntry* pSrc);

// Sort-menu scroll/page handlers (defined in CSortMenu.cpp). Retail names are
// unmangled C linkage - keep extern "C" so the bl relocs bind to them.
extern "C" void sortMenuScrollUp(CSortMenu* _this);  // scroll up
extern "C" void sortMenuScrollDown(CSortMenu* _this);  // scroll down
extern "C" void sortMenuPageUpStep(CSortMenu* _this);  // page up
extern "C" void sortMenuPageDownStep(CSortMenu* _this);  // page down
extern "C" void sortMenuFormatPaneText(nw4r::math::VEC3* out, CSortMenu* _this);  // copy sort-menu state

// Imports with unmangled retail names (defined in CSortMenu.cpp / cursor TUs).
extern "C" int sortMenuIsScrollbarVisible(CSortMenu* _this);
extern "C" void sortMenuResetCount(void*);                  // sort-menu array reset
extern "C" void func_801D3518(void*, void*);            // sort-menu array append
extern "C" void sortMenuSetPage(void*, u8);               // sort-menu page set
extern "C" void Cur_SetVisible(void*, u8);
extern "C" void sortMenuSetLayoutPos(CSortMenu*, const nw4r::math::VEC3*);
extern "C" void sortMenuOpenInit(CSortMenu*);
extern "C" void Cur_Update(void*);                 // CCur18 per-frame update
extern "C" void sortMenuDispatchState(void*);                 // CSortMenu per-frame update
extern "C" void sortMenuTermCleanup(void*);                 // CSortMenu destroy
extern "C" u8 sortMenuGetPageIdx(void*);                   // sort-menu selected page
extern "C" u8 sortMenuGetSubPageIdx(void*);                   // sort-menu selected entry

// Imports with retail names spelled verbatim (C linkage; MWCC would mangle
// plain declarations - the bl reloc must bind to the unmangled retail name).
extern "C" void sortMenuInitFileRead(void*);
extern "C" void sortMenuDraw(void*, void*);
extern "C" void Cur_DrawLayout(void*, void*);
extern "C" void sortMenuToState4Page(void*);
extern "C" int sortMenuGetFlag2B(void*);

// Quest-info buffer helpers (defined in CMenuQstCnt.cpp).
extern "C" void QstCnt_SortRecords(u8*, u32, u32);
extern "C" u16 selectQstIndex(const u8*);
extern "C" CQstLogListQstInfo* QstCnt_GetInfoAt_76F4(u8*, u16);
extern "C" u32 SearchThresholdIdx(u16);
extern "C" void QstCnt_RefreshRecords(u8*);               // QstData refresh
// Quest-list row count for the given quest-log list id (returns u16).
extern "C" u16 QstCnt_FindInfoIndex(u8*, u16);

// BDAT helpers / msg-manager imports (C-ABI retail symbols).
extern "C" u32 Bdat_GetMaxRow_B1EC(void*);              // BDAT row count
extern "C" nw4r::lyt::ArcResourceAccessor* CUICfManager_getArcResourceAccessor();
extern "C" int isClassicController__Q22cf13CfGameManagerFv(int);
extern void* lbl_eu_806640A0;                      // BDAT table pointer (.sbss)

// Layout/text helpers with unmangled retail names (declared here; the
// code_80135FDC.hpp set is co-included for BdatGetPtrDirect/LayoutSetTextBoxFmtValue).
extern "C" u8 BdatGetU8ByTableKey(const void*, const void*, u32);
extern "C" void PaneSetVtxColorByName(nw4r::lyt::Layout*, const char*, u32);
extern "C" void PaneSetTexPalette(nw4r::lyt::Pane*, const char*);
extern "C" void setPaneVisible(void*, u32);
extern "C" u32 CtrlRemote_TouchBitByArg(u32);

// C++ free functions - MWCC mangles these to the retail names
// (drawLayout__FPQ34nw4r3lyt6LayoutPQ34nw4r3lyt8DrawInfoii,
// playUISound__FUl).
void drawLayout(nw4r::lyt::Layout*, nw4r::lyt::DrawInfo*, int, int);
void playUISound(u32);          // UI sound effect

// Shared data pools (retail names; global scope - no C++ mangling).
extern char lbl_eu_80509AB4[];  // string pool (split1 .rodata)
extern char lbl_eu_80576670[];   // 0x22-byte quest text entry table
extern s32 lbl_eu_8066472C;      // quest-list page/scroll boundary (.sbss)
extern u32 lbl_eu_80664728;      // quest-list init flag (.sbss)
extern u32 lbl_eu_80664730;      // current quest id (.sbss)
extern u32 lbl_eu_80664734;      // current quest index (.sbss)
extern u32 lbl_eu_80664738;      // current quest resource (.sbss)
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
