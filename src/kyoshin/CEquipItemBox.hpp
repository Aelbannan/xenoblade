#pragma once

#include <types.h>
#include "kyoshin/CBaseCur.hpp"
#include "kyoshin/CSortMenu.hpp"
#include "kyoshin/CSysWin.hpp"
#include "kyoshin/CItemBoxInfo.hpp"
#include "monolib/lib/UnkClass_8045F564.hpp"
#include "monolib/work/IWorkEvent.hpp"
#include <monolib/work/CEventFile.hpp>  // CEventFile (OnFileEvent param)
#include "kyoshin/code_80135FDC_Bdat.hpp"
#include "kyoshin/code_80135FDC_Lyt.hpp"



namespace nw4r {
namespace math {
    struct VEC3;
}
namespace lyt {
    class Layout;
    class DrawInfo;
    class AnimTransform;
    class ArcResourceAccessor;
    class Pane;
}
}

/* CEIBCur - equip-box layout cursor. Retail vtable lbl_eu_80538704
   [RTTI CEIBCur, 0, setupEIBCur]: a single virtual (initLayout) and no
   destructor slot; no base list (not a CBaseCur). Its out-of-line empty
   destructor (retail __dt__80285954) is named by the CEquipItemBox ctor's
   DESTROYMEMBER action for +0x44. */
class CEIBCur {
public:
    CEIBCur(nw4r::lyt::ArcResourceAccessor* arcResAcc);
    ~CEIBCur();
    virtual void initLayout();

    void Update();
    void Draw(nw4r::lyt::DrawInfo* drawInfo);
    void Reset();
    void StepAnim();
    void Idle();

    nw4r::lyt::ArcResourceAccessor* mArcResAcc; // 0x04
    nw4r::lyt::Layout* mpLayout;                // 0x08
    nw4r::lyt::AnimTransform* mpAnimTrans0;     // 0x0C
    nw4r::lyt::AnimTransform* mpAnimTrans1;     // 0x10
    u8 mActive;                                 // 0x14
    u8 mVisible;                                // 0x15
};

/* Layout draw w/ visible flag. Retail symbol IS the mangled name; declare
   with C++ linkage so MWCC mangles drawLayout to the retail symbol. */

/* Bind a named pane to a resource (code_80135FDC unit, 3-arg form). */

/* Layout/anim builders (code_80135FDC unit). C++ linkage so MWCC mangles
   the unmangled identifiers to the retail names. */

/* CEIBPageCur - page cursor (retail vtable lbl_eu_805386EC, base list
   names CEIBCur); overrides initLayout with setupEIBPageCur. */
class CEIBPageCur : public CEIBCur {
public:
    CEIBPageCur(nw4r::lyt::ArcResourceAccessor* arcResAcc);
    ~CEIBPageCur();
    virtual void initLayout();
};

/* 8-byte item data struct copied by copyEquipItemData */
struct CEquipItemData {
    s16 unk0;   // 0x0
    u8 unk2;    // 0x2
    u8 unk3;    // 0x3
    u8 unk4;    // 0x4
    u8 unk5;    // 0x5
    u8 unk6;    // 0x6
    u8 unk7;    // 0x7
};

/* Item-list region of the CItemBoxInfo object at unk_20c + 0xB0, written by
   refreshEIBItemList's list refresh: u16 ids, 12 VEC3 records (i*0xC), two per-item
   byte tables (+0xA8 / +0xB4) and a per-item s16 table (+0xC0). */
struct CEquipItemBoxItemListView {
    u16 field_00[0xC];            // 0x00..0x17 u16 item ids
    u8 field_18[0x90];            // 0x18..0xA7 12 x 0xC records (VEC3 copies)
    u8 field_A8[0x18];            // 0xA8..0xBF per-item bytes (+0xA8 / +0xB4)
    s16 field_C0[0xC];            // 0xC0..0xD7 per-item shorts
    u8 field_D8;                  // 0xD8 scratch byte (selected column)
    u8 _padD9[0x100 - 0xD9];
};

/* Overlay for the per-page u16 item array at +0x210 with the per-index u8
   table at +0x2C4 and s16 table at +0x2D0 (read by refreshEIBItemList). */
struct CEquipItemBoxPageDataView {
    u8 pad00[0x210];
    u16 field_210[0x20];    // 0x210..0x24F per-page u16 ids
    u8 pad250[0x2C4 - 0x250];
    u8 field_2C4[0xC];      // 0x2C4..0x2CF per-index bytes
    s16 field_2D0[0xC];     // 0x2D0..0x2DF per-index shorts
};

/* Item instance (retail CItemBase-ish). Only the id word at +0x0, the
   flag halfword at +0x4 and the byte at +0x7 are read by this TU's helpers. */
struct CItemInstance {
    u32 word;   // 0x0 id/kind word
    u16 flags;  // 0x4
    u8 unk6;    // 0x6
    u8 unk7;    // 0x7
};

/* CEquipItemGrid - 8-byte item list used by the equip-box helpers. Header
   fields (count / category / cursor row) sit right after the element array.
   Out-of-line ctor (retail 0x80281BB8 initEIBGrid); the empty inline dtor is
   emitted weak (retail __dt__802825D0) for the CEquipItemBox ctor's
   DESTROYMEMBER action, and is a no-op at the `mGrid = CEquipItemGrid()` temp. */
class CEquipItemGrid {
public:
    CEquipItemGrid();
    ~CEquipItemGrid() {}

    double getEIBProgress(CItemInstance* item);
    u32 hasEIBSlotFlag(CItemInstance* item);
    u8 getEIBNameKey(CItemInstance* item);
    void* getEIBItemObj(u16 idx);
    u32 getEIBItemKind(u16 idx);
    s16 getEIBItemId(u16 idx);
    void eibGridNext();
    void eibGridPrev();
    s8 chkEIBEquip(u16 param);
    u8 getEIBByte3(u16 idx);
    u8 getEIBByte4(u16 idx);
    u32 getEIBByte6(u16 idx);
    u8 getEIBByte7(u32 param);
    u8 getEIBColCount();
    u8 getEIBByte2(u16 idx);
    char* fmtEIBItemName(u32 param);
    void sortEIBGridByPage();
    u32 calcEIBCountKey(CItemInstance* item);
    void sortEIBGridByCat(u32 cat);
    void sortEIBByFlags();
    void sortEIBByCount();
    void sortEIBByVf08();
    void sortEIBByNm58(int param);
    void sortEIBByNm00(int param);
    void sortEIBByProg();
    void sortEIBByVf30();
    void sortEIBFlagAsc();
    void sortEIBFlagDesc();
    void sortEIBByNameMode(u32 mode);
    void sortEIBByNameKey();
    void sortEIBByNameKey69();
    void sortEIBByNm72();
    void sortEIBByNm7A();
    void sortEIBByNm17(int key);
    void sortEIBByNm82();
    void sortEIBByVf90();
    void rebuildEIBGridPage(u8 v, u8 b, u8 hi);
    u8 countEIBPages();

    CEquipItemData data[0x400];  // 0x0000..0x2000 (8-byte elements)
    u16 count;                   // 0x2000 (index limit)
    u8 cat;                      // 0x2002 (category passed to item lookup)
    u8 _pad2003;                 // 0x2003 (count/range)
    s8 idx;                      // 0x2004 (current cursor row)
    u8 _pad2005;                 // 0x2005
    char field_2006[0x20];       // 0x2006 name buffer (sprintf dst)
    u8 field_2026[0x400];        // 0x2026..0x2425 per-cell state bytes
};

/* CEquipItemBox - equip item box UI widget (retail RTTI "CEquipItemBox",
   vtable 0x80538658, base list IWorkEvent). Member order from the ctor's
   extab DESTROYMEMBER records. */
class CEquipItemBox : public IWorkEvent {
public:
    CEquipItemBox();
    virtual ~CEquipItemBox();
    virtual bool OnFileEvent(CEventFile* ev);

    void eibPageNext();
    void refreshEIBPageDisp();
    void refreshEIBItemList();
    int getEIBOpenFlag();
    void loadEIBFiles();
    void updateEIBBox();
    void drawEIBBox(nw4r::lyt::DrawInfo* drawInfo);
    void closeEIBBox();
    int eibWindowsReady();
    int getEIBActiveMark();
    int eibInputBlocked();
    int eibMenuBusy();
    int getEIBNamePane();
    int eibSysWinBusy();
    void openEIBBox();
    void finishEIBEntry();
    void eibNavLeft();
    void eibNavDown();
    void eibNavUp();
    void eibNavRight();
    void eibPagePrev();
    void eibConfirmSort();
    void eibHandleSubPage(int param);
    void stepEIBSysWin2();
    void eibOpenPages(u32 v, void* w, int flag);
    int takeEIBAction();
    void setEIBBoxInfo(u32 val);
    void eibApplySelect(u32 param, void* item);
    void handleEIBConfirm();
    void handleEIBNamePane();
    int isEIBSelectBlocked();
    int eibRowMatches();
    int getEIBSelCat();
    u32 findEIBEquipSlot();
    void eibTryCloseRow();
    int eibHudPrompt();
    int isEIBCatMatch();
    void eibPlayEntry();
    void eibCloseAnim();
    void eibSysWin1Done();
    void closeEIBSortMenu();
    void bindEIBCellTexture(u16 kind, void* itemPtr,
                                                   u8 i, u8 b7);
    void refreshEIBCellText(s8 v,
                                                   CItemInstance* item, u8 i);
    void bindEIBCellIcon(u16 kind, void* item, u8 i);
    void bindEIBCellMark(u32 mode, int num, int flag, u32 show);
    void rebuildEIBPage(int a);
    void updateEIBCursorPos();
    void fillEIBSortMenu();
    void clearEIBCatList();
    void pushEIBCatList(u8 val);
    void eibSortPageNext();
    void eibSortPagePrev();
    void refreshEIBSortTabs();
    void bindEIBPageTexture(u8 v, u8 i);
    void refreshEIBSortPages(int a);
    void dispatchEIBKind(int a, int b);
    void resetEIBPageView();
    void rebuildEIBEquipPage(int a, int b);
    void rebuildEIBKind48Page(int kind, int item);
    void rebuildEIBKind3Page(int kind, int item);
    void rebuildEIBKind2Cells(int a, int b);
    int useEIBSelectedItem();
    char* getEIBPageHint();

    UnkClass_8045F564 mRegion04;              // 0x04
    UnkClass_8045F564 mRegion14;              // 0x14
    void* field_24;                           // 0x24 file handle 1
    void* field_28;                           // 0x28 file handle 2
    void* field_2C;                           // 0x2C file handle 3
    nw4r::lyt::ArcResourceAccessor* field_30; // 0x30 arc resource accessor
    nw4r::lyt::ArcResourceAccessor* field_34; // 0x34 arc resource accessor
    nw4r::lyt::Layout* field_38;              // 0x38 layout object
    void* field_3C;                           // 0x3C entry animation transform
    u8 unk_40;                                // 0x40
    u8 unk_41;                                // 0x41
    u8 unk_42;                                // 0x42
    u8 unk_43;                                // 0x43
    CEIBCur mCur44;                           // 0x44
    CEIBPageCur mPageCur;                     // 0x5C
    CCur18 ccur18;                            // 0x74
    CSortMenu mSortMenu;                      // 0x8C
    CSysWin mSysWin1;                         // 0x17C
    CSysWin mSysWin2;                         // 0x1B8
    u8 unk_1f4;                               // 0x1F4
    s8 unk_1f5;                               // 0x1F5
    u8 unk_1f6;                               // 0x1F6
    u16 unk_1f8;                              // 0x1F8
    u16 unk_1fa;                              // 0x1FA
    u16 unk_1fc;                              // 0x1FC
    u8 unk_1fe;                               // 0x1FE
    u32 unk_200;                              // 0x200
    u32 unk_204;                              // 0x204
    u32 unk_208;                              // 0x208
    u32 unk_20c;                              // 0x20C CItemBoxInfo*
    CItemBoxPage mPage;                       // 0x210
    u8 unk_36c[6];                            // 0x36C
    u8 unk_372;                               // 0x372
    u8 unk_373;                               // 0x373
    u8 unk_374;                               // 0x374
    u8 unk_375;                               // 0x375
    u8 unk_376;                               // 0x376
    u8 unk_377;                               // 0x377
    u8 unk_378;                               // 0x378
    u8 unk_379;                               // 0x379
    u8 unk_37a;                               // 0x37A
    u8 unk_37b;                               // 0x37B
    u8 unk_37c;                               // 0x37C
    CEquipItemGrid mGrid;                     // 0x37E
    void* field_27A4;                         // 0x27A4
};

