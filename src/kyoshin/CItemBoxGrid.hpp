#pragma once
extern "C" int Counter_TestBit(void*, u32);
#include "kyoshin/code_8025FB10.hpp"

#include <types.h>
#include "kyoshin/code_80135FDC_Bdat.hpp"
#include "kyoshin/code_80135FDC_Lyt.hpp"
#include "monolib/work/IWorkEvent.hpp"
#include "monolib/lib/UnkClass_8045F564.hpp"
#include "kyoshin/CBaseCur.hpp"
#include "kyoshin/CSortMenu.hpp"
#include "kyoshin/CItemBoxInfo.hpp"
#include "kyoshin/CNumSelect.hpp"
#include "kyoshin/CItemBoxGridSubMenu.hpp"
#include "kyoshin/CExchangeWin.hpp"
#include "kyoshin/CPresentWinClass.hpp"
#include "kyoshin/CSysWin.hpp"
#include "kyoshin/CQuestWindow_Pane.hpp"
#include "kyoshin/CItemBoxGrid_Color.hpp"
#include "kyoshin/code_80135FDC_State.hpp"
#include "kyoshin/CUICfManager_Res.hpp"
class CFileHandle;
class CEventFile;
namespace nw4r { namespace lyt {
class AnimTransform; class Layout; class ArcResourceAccessor; class DrawInfo;
} }

// Id-pool lists embedded in the item grid (retail RTTI: CVisionItem and
// CArtsBookItem list __RTTI__10CQuestItem as their base). Each holds up to
// 0x400 u16 item ids and a count; Rebuild() refills the pool (retail
// RegisterTeachableItems / PushGatedTypeIds / RegisterLearnedSkills).
// The copy assignment is the out-of-line retail MenuShopSellCopyData
// (defined in the CMenuShopSell TU); the inline wrapper folds to that bl.
extern "C" void MenuShopSellCopyData(void*, void*);
class CQuestItem {
public:
    CQuestItem() { mCount = 0; }
    virtual ~CQuestItem() {}  // +0x08
    virtual void Rebuild();    // +0x0C - retail RegisterTeachableItems

    int HasSlotIdMatch(u32 id);
    void PushUniqueItemId(u32 id);

    CQuestItem& operator=(const CQuestItem& rhs) {
        MenuShopSellCopyData(this, (void*)&rhs);
        return *this;
    }

    u16 mIds[0x400]; // +0x004
    u16 mCount;      // +0x804
};

class CVisionItem : public CQuestItem {
public:
    CVisionItem();
    virtual ~CVisionItem();  // +0x08
    virtual void Rebuild();  // +0x0C - retail PushGatedTypeIds
};

class CArtsBookItem : public CQuestItem {
public:
    CArtsBookItem();
    virtual ~CArtsBookItem();  // +0x08
    virtual void Rebuild();    // +0x0C - retail RegisterLearnedSkills
};

// 7-float price scale row copied out of the itemBoxGridPriceScaleRow pool by the
// price functions; retail lowers the copy as an inline struct assignment.
struct CItemPriceScaleRow {
    float v[7];
};

// 7-word per-language slot-name table copied wholesale by FormatCellItemName's
// '$2' tag handler; retail lowers the assignment as a 3x8-byte word-copy
// loop plus a trailing word (a 28-byte struct assignment).
struct CItemBoxLangWordTable {
    u32 w[7];
};

// Cast-only view of the object returned by the vt+0x2C equipped-state
// fetch: bit 0 of the +0x04 halfword marks the slot as equipped.
struct CItemBoxSlotState {
    u16 field_00;
    u16 field_02;
    u16 state;
};

// (Fake-vtable cleanup: CItemBoxObjVt08 deleted - the +0x44 delete is now
// `delete (nw4r::lyt::Layout*)` and the +0x5C delete goes through the
// nw4r::ut::TagProcessorBase<wchar_t> base, both with the virtual dtor at
// +0x08.)

// (Fake-vtable cleanup: CItemBoxAccVt0C deleted - accessor calls are now
// ArcResourceAccessor::GetResource.)

// (Fake-vtable cleanup: CItemBoxObjVt0C deleted - cursor calls are now
// CBaseCur::cleanup/initLayout; the +0x2ca8/+0x34b0/+0x3cb8 resets are now
// CQuestItem/CVisionItem/CArtsBookItem::Rebuild.)

// (Fake-vtable cleanup: item-impl +0x2C view deleted - now CItemImpl::vf2C.)

// (Fake-vtable cleanup: CItemInstVt2CInt deleted - layout animation-flag
// calls are now Layout::SetAnimationEnable.)
// (Fake-vtable cleanup: item-impl +0x40 view deleted - now CItemImpl::vf40.)

// (Fake-vtable cleanup: CItemBoxLayoutVt38 deleted - now Layout::Animate.)

// (Fake-vtable cleanup: CItemBoxObjA0Vt deleted - cursor calls are now
// CBaseCur::setRootPaneTranslate.)

// (Fake-vtable cleanup: CItemPaneObjVt deleted - now Pane::FindPaneByName.)

// (Fake-vtable cleanup: CItemPaneAnimVt deleted - now
// Layout::SetAnimationEnable.)

// (Fake-vtable cleanup: item-impl +0x08 view deleted - now CItemImpl::vf08.)

// (Fake-vtable cleanup: item-impl +0x10 view deleted - now CItemImpl::vf10.)

// (Fake-vtable cleanup: item-impl +0x30 view deleted - now CItemImpl::vf30.)

// (Fake-vtable cleanup: item-impl +0x90 view deleted - now CItemImpl::vf90.)

// (Fake-vtable cleanup: item-impl +0x20 view deleted - now CItemImpl::vf20.)

// (Fake-vtable cleanup: item-impl +0x4C view deleted - now CItemImpl::vf4C.)

// (Fake-vtable cleanup: item-impl +0x80 view deleted - now CItemImpl::vf80.)

// (Fake-vtable cleanup: item-impl +0x54 view deleted - now CItemImpl::vf54.)

// Plain 3-float view of the two cursor-09 move bounds (the caller-side struct
// copies land in the argument area; CCur09::SetTwoPanes takes them by address).
struct CibgVec3 {
    float x, y, z;
};

// Cast-only view of the texture resource entry returned by the sys-win
// texture lookup: material-list pointer at +0x08.
struct CibgTexRes {
    u8 _00[8];
    void* field_08;
};
struct CibgTexMat {
    void* field_00;
};
struct CibgTexData {
    u16 field_00;
    u16 field_02;
};

// (Fake-vtable cleanup: CibgFontVt24 deleted - font calls are now
// IDeviceFontInfo::getFont.)

// (Fake-vtable cleanup: CibgSysWinVt0C deleted - texture lookups are now
// ArcResourceAccessor::GetResource.)

// 24-byte category-filter row copied around by value in PopulateGridEntries
// (MWCC lowers the struct copies to unrolled lwz/lwzu pairs).
struct GridFilter24 {
    u32 w[6];
};

// 10-byte grid cell (9 data bytes): stored item id + 7 per-cell flag bytes.
// The ctor is retail SetEntry9Bytes (out of line; the grid ctor's array
// construction calls it with the default arguments).
struct CItemBoxGridEntry {
    s16 id;        // +0x00 item id (or -1 for an empty cell)
    u8 flags[6];   // +0x02 per-cell flag bytes
    bool mMarked;  // +0x08 cell marked (toggled by ToggleCellMark)

    CItemBoxGridEntry(s16 id = -1, u8 f0 = 0, u8 f1 = 0, u8 f2 = 0, u8 f3 = 0, u8 f4 = 0, u8 f5 = 0, bool marked = false);
};

// Sort keys come from CItemBoxItem (CItemBoxInfo.hpp).

// Item-cell grid embedded in CItemBoxGrid at +0x54C (retail ctor
// __ct__801C5514, dtor __dt__801C5670): 0x400 cells plus count/category/
// row/page bytes, three lookup windows and the three id-pool lists.
class CItemBoxGridFull {
public:
    CItemBoxGridFull();
    ~CItemBoxGridFull();

    void PopulateGridEntries(u8 cat, int ownerIdx, int refItemId, int valueMode);
    u32 CalcGridRowCount();
    void IncGridPageIndex();
    void DecGridPageIndex();
    s16 GetCellItemId(u16 idx);
    u32 GetCellSellPrice(u16 idx);
    u32 GetScaledItemValue(u32 id, void* item, int reserved);
    u32 GetCellKind(u16 idx);
    void* GetCellItemPtr(u16 idx);
    s32 CheckCellState(u16 idx);
    u8 GetCellFlag0(u16 idx);
    u8 GetCellFlag2(u16 idx);
    u8 GetCellFlag3(u16 idx);
    int GetCellFlag1(u16 idx);  // int: retail callers test the full word (no clrlwi)
    u8 GetCellByte7(u16 idx);
    bool GetCellByte8(u16 idx);
    void ToggleCellMark(u16 idx);
    u8 CountMarkedCells();
    u8 GetCategoryCap();
    int LookupIndexedByte();
    void InitGridItems();
    u32 FormatCellName(u32 idx);
    char* FormatCellItemName(u16 idx);
    void ResortGridEntries(u16 idx);
    void CompactGridEntries();
    u32 GetItemBasePrice(void* item);
    s32 GetShopItemValue(u32 id, void* item);
    void SortGridEntries(u32 mode);
    void GroupBySlotStat(u32 target);
    void GroupBySlotAlt(u32 target);
    void SortGridByValue();
    void SortGridBySlotStatus();
    void SortGridByKindName();
    void SortGridByKindNameAlt();
    void GroupByCapKey(u32 target);
    void SortGridByMsgKeyDesc();
    void GroupByKindName(u32 target);
    float GetItemValueFloat(void* entry);
    u32 HasEquippedSlot(void* item);
    u32 FindKindPoolIdx(void* entry);
    void SortByItemU16();
    void SortByPriceDesc();
    void SortByKindNameD();
    void SortByItemByte6();
    void SortByCellFlags();
    void SortByFlag6Desc();
    void SortByFlag6Asc();
    void SortByItemVf08();
    void SortByItemVf30();
    void SortByEquipAsc();
    void SortByEquipDesc();
    void SortByCapKeyAsc();
    void SortByCapKey2Asc();
    void SortByCapKeyDesc();
    void SortByItemVf90();

    CItemBoxGridEntry entries[0x400]; // +0x0000
    u16 field_2800;                   // +0x2800 entry count
    u8 field_2802;                    // +0x2802 current category
    u8 field_2803;                    // +0x2803 row count
    u8 field_2804;                    // +0x2804 page index
    u8 field_2805[0x20];              // +0x2805
    u8 field_2825[0x80];              // +0x2825
    u8 field_28A5[0x400];             // +0x28A5 per-page lookup window
    CQuestItem mQuestItems;           // +0x2CA8
    CVisionItem mVisionItems;         // +0x34B0
    CArtsBookItem mArtsBookItems;     // +0x3CB8
};

// Item-box grid screen widget (retail __vt__12CItemBoxGrid, IWorkEvent base
// list); receives its archives through IWorkEvent::OnFileEvent.
class CItemBoxGrid : public IWorkEvent {
public:
    CItemBoxGrid(u8 mode, u16 unk5, u32 unk6, u8 unk7);
    virtual ~CItemBoxGrid();
    virtual bool OnFileEvent(CEventFile* pEventFile);

    int GetField61();
    u8 GetField549();
    int GetField52D();
    void PushToList(unsigned char val);
    void LoadItemBoxFiles();
    void UpdateItemBox();
    void DrawItemBoxGrid(nw4r::lyt::DrawInfo* drawInfo);
    void UnloadItemBox();
    u32 IsItemBoxReady();
    u32 IsItemBoxActive();
    int GetIdleFlag542();
    u32 IsSubWinActive();
    void InitItemDisplay();
    void AdvanceBoxState();
    void ClearListSlots();
    void IncListIndex();
    void DecListIndex();
    void GridMoveCursorLeft();
    void GridMoveCursorRight();
    void GridMoveCursorDown();
    void GridMoveCursorUp();
    void NextGridPage();
    void PrevGridPage();
    void OpenSortMenu();
    void HandleCancelBtn(int confirmed);
    void func_801CCAF0();
    void SetInfoMsgId(u16 val);
    int GetExchFlag52C();
    void TickItemBoxGrid();
    void SelectCatRow();
    u8 GetPromptState();
    void AdvanceOpenAnim();
    void FinishOpenAnim();
    void CheckState4_Animate();
    void AdvanceCloseAnim();
    void FinishCloseAnim();
    void FinishNumSelect();
    void ShowSubMenuCur();
    void DispatchSubmenuConfirm();
    void ShowExchangeCur();
    void FinishExchange();
    void ShowPresentCur();
    void FinishPresent();
    void OnSysWinOpen();
    void HandleSysWinOK();
    void FinishSysWin();
    void SetGridReady();
    void SetCellCaption(s8 kind, void* item, u16 idx);
    void ShowCellCount(u32 kind, void* flag2, void* flag3, u32 idx);
    void SetTabHeader(int cat, int idx);
    void SetCellMarkVis(bool val, u32 idx);
    void RefreshCategoryTabs();
    void RebuildGridCat();
    void RebuildGridUI();
    void RefreshSelectedCellText();
    void MoveTabPage(int val);
    void UpdateCursors();
    void func_801D0BD8();
    void UnequipPartyItem(int kind, int id);
    void DispatchItemEvt(void* item, int eventType);
    u32 GetCatLabelMsg();
    u32 IsItemTeachable(u32 kind);
    void ShowCellIcon(u32 kind, void* item, u16 idx, u32 bt);
    void ShowCellBadge(u32 kind, void* item, u16 idx);

    u32 field_04;                         // +0x04
    UnkClass_8045F564 mRegion08;          // +0x08
    UnkClass_8045F564 mRegion18;          // +0x18
    CFileHandle* field_28;                // +0x28 file handle
    CFileHandle* field_2C;                // +0x2C file handle
    CFileHandle* field_30;                // +0x30 file handle
    CFileHandle* field_34;                // +0x34 file handle
    void* field_38;                       // +0x38 heap block
    nw4r::lyt::ArcResourceAccessor* field_3C; // +0x3C
    nw4r::lyt::ArcResourceAccessor* field_40; // +0x40
    nw4r::lyt::Layout* field_44;          // +0x44
    nw4r::lyt::AnimTransform* field_48;   // +0x48
    nw4r::lyt::AnimTransform* field_4C;   // +0x4C
    nw4r::lyt::AnimTransform* field_50;   // +0x50
    u8 field_54;                          // +0x54 ready byte
    u32 field_58;                         // +0x58 state word
    void* field_5C;                       // +0x5C tag processor
    u8 field_60;                          // +0x60
    u8 field_61;                          // +0x61
    u8 field_62[0xC];                     // +0x62 list slots
    u8 field_6E;                          // +0x6E
    u8 field_6F;                          // +0x6F list index
    CCur07 mCur07;                        // +0x70
    CCur09 mCur09;                        // +0x88
    CCur18 mCur18;                        // +0xA0
    CCur16 mCur16;                        // +0xB8
    CCur11 mCur11;                        // +0xD0
    CSortMenu mSortMenu;                  // +0xE8
    CItemBoxInfo mInfo;                   // +0x1D8
    CNumSelect mNumSelect;                // +0x3E4
    CItemBoxGridSubMenu mSubMenu;         // +0x418
    CExchangeWin mExchangeWin;            // +0x440
    CPresentWin mPresentWin;              // +0x468
    CSysWin mSysWin4AC;                   // +0x4AC
    CSysWin mSysWin4E8;                   // +0x4E8
    u8 field_524;                         // +0x524
    s8 field_525;                         // +0x525 signed category byte
    u8 field_526;                         // +0x526
    u8 field_527;                         // +0x527 mode byte
    u8 field_528;                         // +0x528
    u8 field_529;                         // +0x529
    u16 field_52A;                        // +0x52A
    u8 field_52C;                         // +0x52C
    u8 field_52D;                         // +0x52D
    u16 field_52E;                        // +0x52E
    u8 field_530;                         // +0x530
    u8 _531[0x540 - 0x531];
    s8 field_540;                         // +0x540
    u8 field_541;                         // +0x541
    u8 field_542;                         // +0x542
    u8 field_543;                         // +0x543 file-present flag
    u8 field_544;                         // +0x544
    u8 field_545;                         // +0x545
    u8 field_546;                         // +0x546
    u8 field_547;                         // +0x547
    u8 field_548;                         // +0x548
    u8 field_549;                         // +0x549
    CItemBoxGridFull mGrid;               // +0x54C
};


// C-linkage imports (retail symbol names - keep linkage/signatures verbatim)
extern "C" u32 GridSubMenu_GetOptionCount(void*);  // not yet decompiled (CItemBoxGridSubMenu TU stub)

// pointer-arg form, matching every call site here directly - no cast needed.
extern "C" void CItemData_initFromFamily(void*, u32);
extern "C" void GridSubMenu_GetCursorPos(void*, CItemBoxGridSubMenu*);

extern "C" int* CtrlObjectParam_GetSlotTableBase(void);
extern "C" void CItemBlock_setFlag120EC(u8, u32);
extern "C" void* CItemBlock_getPtr20E8(void);
extern "C" void CItemBlock_setCount(int);
extern "C" void* getFontInfo__11CDeviceFontFUlPQ34nw4r3lyt6Layout(u32, nw4r::lyt::Layout*);
extern "C" void deleteRegion__17UnkClass_8045F564Fv(void*);
extern "C" u32 CtrlObjectParam_GetCurrentRowKey(void*);


extern "C" void copyVEC3(void*, const void*);
extern "C" u32 Bdat_GetMaxRow_B1EC(void*);
#include "kyoshin/cf/CtrlRemoteApi.hpp"
extern "C" void startEnumObjects(void*, u32, u32);
extern "C" void* getEntryAt(void*, u32);
extern "C" void* getCfObjectPc__FPQ22cf12CfObjectMove(void*);
namespace cf { class CfObjectPc; }
void CfObjectPc_setupFrame(cf::CfObjectPc* obj);
extern "C" void func_80043E88(void*, int);
extern "C" void CtrlObjectParam_SetEquipSlot5(void*, int);
extern "C" void CtrlObjectParam_SetEquipSlot0(void*, int);
extern "C" void CtrlObjectParam_SetEquipSlot1(void*, int);
extern "C" void CtrlObjectParam_SetEquipSlot4(void*, int);
extern "C" void* CtrlObjectParam_GetCharEntry(u32);
extern "C" u32 CtrlObjectParam_GetArtsDataWord(void*);
extern "C" u16 ArrayGet12(const unsigned short*, unsigned char);
extern "C" char* getErrMesText16(void);
extern "C" char* getErrMesText15(void);
extern "C" int IsSkillItem(void*);
extern "C" u16 CItemBlock_getFlag120EC(int);
extern "C" u32 CItemBlock_countKindSlots(u8);
extern "C" char* CopyEntry9Bytes(char*, const char*);
extern "C" void* getHandleMEM2__Q23mtl10MemManagerFv();
extern "C" void* getHandleMEM1__Q23mtl10MemManagerFv();
extern "C" u32 getMaxAllocSize__Q23mtl10MemManagerFUl(u32);
extern "C" void deallocate__Q23mtl10MemManagerFPv(void*);
extern "C" int KyoshinHeap_GetField44();
extern "C" void* readFile__11CDeviceFileFUlPCcP10IWorkEventii(u32, const char*, void*, int, int);
extern "C" void* readCommonArchiveFile__11CDeviceFileFUlPCcP10IWorkEventii(u32, const char*, void*, int, int);

extern "C" void termPresentWin(void*);
extern "C" void CopyGridEntry9(void*, void*);
extern "C" u32 CItem_slotCnt(u8);
extern "C" void setPresentAnimText(void*, u32);
extern "C" void openPresentWin(void*);
extern "C" void showPresentRank(void*);
extern "C" u32 getPresentRankA(void*);
extern "C" u32 getPresentRankB(void*);
extern "C" void CItem_thunkAllocRecord(u32, u32);
extern "C" void UIWin_BuildFlagBufRec(u32, u32, u32);
extern "C" void CtrlObjectParam_ClaimArtsSlotRow(void*, u8);
// UIWin_BuildFlagBuf is owned by kyoshin/CUIWindowManager.hpp.

extern "C" void* getHandleMEM2__Q23mtl10MemManagerFv(void);
extern "C" void* createRegion__17UnkClass_8045F564FiiPCci(void*, void*, int, const char*, int);
extern "C" void __ct__14Class_8045F858FP17UnkClass_8045F564(void*, void*);
extern "C" void setMemInitFlag__Q23mtl10MemManagerFb(bool);
extern "C" void* __ct__CTagProcessor(void*);
extern "C" bool Attach__Q34nw4r3lyt19ArcResourceAccessorFPvPCc(nw4r::lyt::ArcResourceAccessor*, void*, const char*);
 extern "C" CBaseCur* __ct__CCur07(void*, void*);
extern "C" void __ct__12CItemBoxInfoFUcUs(void*, int, int);
extern "C" void __ct__CNumSelect(void*, int);
extern "C" void __ct__CPresentWin(void*);
extern "C" void __ct__7CSysWinFUc(void*, int);
extern "C" void __as__17UnkClass_8045F564FRC17UnkClass_8045F564(void*, void*);
extern "C" CBaseCur* __ct__CCur09(void*, void*);
extern "C" CBaseCur* __ct__CCur16(void*, void*);
extern "C" CBaseCur* __ct__CCur11(void*, void*);
extern "C" void __dt__6CCur07Fv(void*, int);
extern "C" void __dt__6CCur09Fv(void*, int);
extern "C" void __dt__6CCur16Fv(void*, int);
extern "C" void __dt__6CCur11Fv(void*, int);
extern "C" void __dt__7CSysWinFv(void*, int);
extern "C" void __dt__11CPresentWinFv(void*, int);
extern "C" void __dt__10CNumSelectFv(void*, int);
extern "C" void __dt__17UnkClass_8045F564Fv(void*, int);
extern "C" void MenuShopSellCopyData(void*, void*);
extern "C" void __dt__14Class_8045F858Fv(void*, int);
extern "C" void validateHeap__17UnkClass_8045F564Fv(void*);
extern "C" void initPresentWin(void*, void*);

// ---- UnequipPartyItem imports/helpers ----
extern "C" void CtrlObjectParam_SyncParamFromActor(void*);

// Enum list returned by get__16CfEnumListHolderFv: element count at +0x620.
struct CItemBoxGridEnumList {
    u8 _pad00[0x620];
    u32 field_620;
};
// Slot from getEntryAt: move object pointer at +0x04.
struct CItemBoxGridEnumSlot {
    u32 field_00;
    void* field_04;
};
// Move object whose +0x3f28 halfword holds the player index.
struct CItemBoxGridMoveObj {
    u8 _pad00[0x3f28];
    u16 field_3f28;
};
// ---- CItemBoxGrid ctor: mirror shapes of the sub-blocks copied by value out
// of the CItemBoxInfo/CNumSelect/CExchangeWin stack temporaries and
// the 801C5514 sub-grid temp. Named scalars lower to individual moves while
// array members lower to paired lwzu/stwu bdnz loops, matching retail.
struct CibgWords16 {
    u32 w[16];
};
struct CibgWords6 { u32 w[6]; };
struct CibgWords3 { u32 w[3]; };
struct CibgBytes64 { u8 b[0x40]; };
struct CibgBytes32 { u8 b[0x20]; };
struct CibgThreeBytes {
    u8 a, b, c;
};
struct CibgInfoHdr {  // CItemBoxInfo temp +0x24: many named words, no loop
    u32 w00, w01, w02, w03, w04, w05, w06, w07, w08, w09;
    u32 w10, w11, w12, w13, w14, w15, w16, w17, w18, w19;
    u32 w20, w21, w22, w23, w24, w25, w26;
    u8 b0;
    u32 w27;
    u8 b1, b2, b3;
};
struct CibgInfoRow {  // CItemBoxInfo temp +0xAC: trailing word array loops
    u16 a;
    u8 b, c;
    CibgWords6 w;
};
struct CibgWords18 {
    u32 w[18];
};struct CibgInfoFive {
    u32 a, b, c, d, e;
};
struct CibgHeadTail3 {
    u32 head;
    CibgWords3 tail;
};
struct CibgByteArr64 {
    u8 b;
    CibgBytes64 arr;
};
struct CibgNumSel {
    u32 a, b, c, d, e, f;
    u8 g, h, i, j, k;
};
struct CibgExch {
    u32 a, b, c, d;
    u8 e, f, g, h;
};
struct CibgSubGridCells {
    u32 w[0x400];
};
struct CibgSubGridMeta {  // sub-grid temp +0x800
    u16 a;
    u8 b, c, d;
    CibgBytes32 names;
};
struct CibgByteBlock200 {
    u8 arr[0x200];
};

// 16-byte memory region whose copy goes through the retail
// __as__17UnkClass_8045F564FRC17UnkClass_8045F564 helper (same-TU inline folds the wrapper away,
// leaving the single bl the retail assignments show).
class CibgUnk16 {
public:
    u8 d[0x10];
    CibgUnk16& operator=(const CibgUnk16& o) {
        __as__17UnkClass_8045F564FRC17UnkClass_8045F564((void*)d, (void*)o.d);
        return *this;
    }
};

// Occupies layout space but emits no copy code: the empty operator= inlines
// away, so struct assignments skip these bytes exactly like retail.
struct CibgSkip4 {
    u32 w;
    CibgSkip4& operator=(const CibgSkip4&) { return *this; }
};

// Whole copied members: one struct assignment per temp region reproduces
// retail's scheduling (copy-ctor bls, early loop-pointer setup, interleaved
// scalar moves). Explicit pad fields mark the uncopied holes.
struct CibgInfoMember {   // p+0x1dc.. <- CItemBoxInfo temp +4
    CibgUnk16 unk1;
    CibgUnk16 unk2;
    CibgInfoHdr hdr;
    CibgSkip4 pad104[4];
    CibgInfoRow row;
    CibgWords18 w18;
    CibgInfoFive five;
    CibgHeadTail3 ht;
    CibgSkip4 pad158[3];
    CibgByteArr64 ba;
};
struct CibgNumSelMember { // p+0x3e8.. <- CNumSelect temp +4
    CibgUnk16 unk;
    u32 a, b, c, dd, e, f;
    u8 g, h, i, j, k;
};
struct CibgExchMember {   // p+0x444.. <- CExchangeWin temp +4
    CibgUnk16 unk;
    u32 a, b, c, d;
    u8 e, f, g, h;
};

// (Fake-vtable cleanup: item-impl +0x44 view deleted - now CItemImpl::vf44.)
