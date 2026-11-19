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
#include "kyoshin/CUICfManager_Res.hpp"  // getPackedFont
#include "kyoshin/CItemBoxGrid_Color.hpp"  // CopyVec4s, GXColorS10 helpers


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

// Item-impl accessor: canonical signature from kyoshin/cf/CItem.hpp.
// Returns the per-category implementation object for an item record.
struct CItemData;
class CItemImpl;
extern "C" CItemImpl* CItem_initItemImplInstances(CItemData* self);

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

// ---------------------------------------------------------------------------
// CEquipItemBox-unit declarations and retail imports. These used to live in a
// side imports header; they now belong to the unit's real header so every
// caller sees the same declarations and the faithful counter sees them too.
// ---------------------------------------------------------------------------

/* Texture object returned by ArcResourceAccessor::GetResource for 'timg':
   mChain (+0x08) -> dims (+0x00) carries the 2D dimension header. */
struct CEquipItemBoxTexDimsView {
    u16 field_0;    // 0x0
    u16 field_2;    // 0x2
};
struct CEquipItemBoxTexChainView {
    CEquipItemBoxTexDimsView* mDims;  // 0x0
};
struct CEquipItemBoxTexObjView {
    void* mVtable;                 // 0x0
    void* _pad_04;                 // 0x4
    CEquipItemBoxTexChainView* mChain;  // 0x8
};

// OnFileEvent helper imports (retail C-ABI names). The MemManager flag,
// ArcResourceAccessor::Attach and CDeviceFont::getFontInfo are called through
// their real class headers (monolib/util/MemManager.hpp, nw4r/lyt, and
// monolib/device/CDeviceFont.hpp) rather than pre-mangled extern "C" aliases.
extern "C" void* Bdat_GetTable_AA34();

extern void* lbl_eu_806649E0;

/* CSysWin vtable view exposing the layout-build virtual at +0x88 (slot 34 =
   declared index 32 after the MWCC offset-to-top + RTTI prefix), dispatched by
   loadEIBFiles after the file loads. Same shape as the CSysWinView classes in
   CSysWinSave.hpp / COption.hpp / CCollepedia.hpp. */
class CEquipItemBoxSysWinView {
public:
    virtual void v00() = 0;
    virtual void v01() = 0;
    virtual void v02() = 0;
    virtual void v03() = 0;
    virtual void v04() = 0;
    virtual void v05() = 0;
    virtual void v06() = 0;
    virtual void v07() = 0;
    virtual void v08() = 0;
    virtual void v09() = 0;
    virtual void v10() = 0;
    virtual void v11() = 0;
    virtual void v12() = 0;
    virtual void v13() = 0;
    virtual void v14() = 0;
    virtual void v15() = 0;
    virtual void v16() = 0;
    virtual void v17() = 0;
    virtual void v18() = 0;
    virtual void v19() = 0;
    virtual void v20() = 0;
    virtual void v21() = 0;
    virtual void v22() = 0;
    virtual void v23() = 0;
    virtual void v24() = 0;
    virtual void v25() = 0;
    virtual void v26() = 0;
    virtual void v27() = 0;
    virtual void v28() = 0;
    virtual void v29() = 0;
    virtual void v30() = 0;
    virtual void v31() = 0;
    virtual void v32() = 0; // vtable +0x88 (layout build)
};

/* u32->float conversion scratch (CPartsChange.cpp convention): builds the
   0x43300000-prefixed bit pattern by hand so the magic subtraction below
   references the retail .sdata2 constants (lbl_eu_80668B18/80668B10)
   instead of MWCC's un-nameable pool entry. */
union CEquipItemBoxF64Conv {
    u32 w[2];
    double d;
};

/* 6-byte equipment-slot definition used by findEIBEquipSlot's slot scan:
   category byte, item-id short and a per-category base offset byte. */
struct CEquipItemBoxSlotDef {
    u8 cat;     // 0x0
    u8 _pad1;   // 0x1
    s16 item;   // 0x2
    u8 extra;   // 0x4 (base offset added to the matched instance index)
    u8 _pad5;   // 0x5
};

/* 36-byte table of 6 slot definitions (data at lbl_eu_8050EF90). Copied to a
   stack local by findEIBEquipSlot before the item ids are patched in. */
struct CEquipItemBoxSlotTable {
    CEquipItemBoxSlotDef slots[6];
};

/* The CtrlObjectParam_GetCharEntry item list is CItemBoxCharEntry in
   CItemBoxInfo.hpp; its six s16 slotIds at +0x1C..+0x27 supply the ids
   patched into the slot table by findEIBEquipSlot. */

// 36-byte equipment-slot table (data import, same .data block as the string pool).
extern CEquipItemBoxSlotTable lbl_eu_8050EF90;

/* 24-byte equipment-category table (6 u32 entries; the low byte of each word
   is a category id). Copied to a stack local by rebuildEIBGridPage. */
struct CEquipItemBoxCatWordTable {
    u32 w[6];   // 0x00..0x17
};
extern CEquipItemBoxCatWordTable lbl_eu_8050EF50;

/* 12-byte u32 table copied by useEIBSelectedItem and indexed by the matched
   category slot (three CfMove search types). */
struct CEquipItemBoxMoveTable {
    u32 w[3];
};
extern CEquipItemBoxMoveTable lbl_eu_8050EFB4;

/* 12-byte category-byte table (.sdata) copied to the stack by
   useEIBSelectedItem's slot-clearing tail. */
struct CEquipItemBoxCatTable12 {
    u8 b[0xC];
};
extern CEquipItemBoxCatTable12 lbl_eu_80668B50;

/* CfObjectMove search-result entry: the word at +0x4 must be non-zero
   before the move object is resolved. */
struct CEquipItemBoxMoveEntryView {
    void* vtable;   // 0x0
    u32 field_4;    // 0x4 validity word
};

/* Stack-allocated enum-list holder/list used by useEIBSelectedItem's CfMove
   search (__ct__16CfEnumListHolderFv ctor / get__16CfEnumListHolderFv getter / __dt__16CfEnumListHolderFv dtor;
   the list's element count sits at +0x620). */
struct CEquipItemBoxEnumList {
    u8 _pad00[0x620];
    u32 field_620;  // 0x620
};

// C-linkage imports (retail symbol names - keep linkage/signatures verbatim)
// Same-TU helpers (defined as extern "C" free functions in CEquipItemBox.cpp;
// the retail symbols are unmangled, so calls must reference the plain names).
// Forward decls for the grid/array helpers (defined in CEquipItemBox.cpp).
extern "C" CEquipItemData* setEIBItem(CEquipItemData* dst, s16 a0, u8 a2, u8 a3, u8 a4, u8 a5, u8 a6, u8 a7);
extern "C" void copyEIBItem(CEquipItemData* dst, const CEquipItemData* src);
// String pool used by getEIBPageHint (offsets 0x2d / 0x36) and others.
extern "C" char lbl_eu_8050EFDC[];

// Texture-name lookup (retail C-ABI; u16 arg, returns the resource name).
// Item-object validity/type check used by bindEIBCellIcon's texture pick.
extern "C" int IsSkillItem(void*);
extern "C" void* CtrlObjectParam_GetCharEntry(u32);
// Item randomizer: reads the low 16 bits of the first word of the object
// returned by CtrlObjectParam_GetCharEntry (cf/CtrlObjectParam TU; callers pass that obj).
extern "C" u32 CtrlObjectParam_GetArtsDataWord(void*);
// Item drop-rate helpers (code_80135FDC TU): name-table float probe and the
// (rateA + rateB) clamp divisor shared by the equip-box/line stat displays.
extern "C" void copyVEC3(void*, const void*);
// Item/bdat table handles: canonical type from the owner header (cf/CItem.hpp).
extern void* lbl_eu_806640D8;
extern void* lbl_eu_806640F8;
// Pane-visibility query (C-ABI retail symbol; extern "C" so call sites emit
// the plain name, matching eibHandleSubPage's reloc site).
extern "C" bool isPaneVisible(nw4r::lyt::Pane*);

// Item-box object type for the unk_20c pointer (forward decl; full layout in
// CItemBoxInfo.hpp, which is included by TUs that also include this header).
struct CItemBoxInfo;
// Item-box state query (retail mangled getItemBoxState__FP12CItemBoxInfo;
// u32 return avoids the u8 call-site mask; same decl as CItemBoxGrid.hpp).
// Per-index value store into the embedded CItemBoxInfo at +0x210
// (retail mangled setItemBoxIndex__FP12CItemBoxInfoUcs; C++ linkage).

// Item-box list refresh helpers (CItemBoxInfo TU, retail plain names).
extern "C" u8 ItemBoxGetTab(const u8*, unsigned int);
// Layout text/visibility bind (retail mangled setLayoutTextBoxNumber__FPQ34nw4r3lyt6LayoutPcUc).
// Same-TU grid helper (retail plain name; defined in this .cpp).
// Same-TU dispatch helpers used by sortEIBGridByPage (defined in this .cpp).
// Same-TU grid sort helpers dispatched by sortEIBGridByPage (defined below).
// Same-TU grid helpers used by the equip-list refresh (retail plain names).
extern "C" u16 CItemBlock_getFlag120EC(int);
// Per-frame update helpers (retail plain names, defined in sibling TUs).
extern "C" int KyoshinHeap_GetField44(void);
// Sub-window / cursor / sort-menu draw helpers (retail plain names).
// Layout text/colour setter used by the sort-menu page rebuild (refreshEIBSortTabs);
// the 3rd/4th args are .sbss colour-table pairs referenced via sda21.
// .sbss colour-table initialisers used by sinit_8028DAB0 (retail plain names).
extern "C" void SplitU32ToS16s(void*, u32);
extern "C" void setGXColorS10(void*, u32, u32, u32, u32);
extern s16 lbl_eu_80664920[4];
extern s16 lbl_eu_80664928[4];
extern s16 lbl_eu_80664930[4];
extern s16 lbl_eu_80664938[4];
extern s16 lbl_eu_80664940[4];
extern s16 lbl_eu_80664948[4];
extern s16 lbl_eu_80664950[4];
extern s16 lbl_eu_80664958[4];
extern s16 lbl_eu_80664960[4];
extern s16 lbl_eu_80664968[4];
extern s16 lbl_eu_80664970[4];
extern s16 lbl_eu_80664978[4];
extern s16 lbl_eu_80664980[4];
extern s16 lbl_eu_80664988[4];
extern s16 lbl_eu_80664990[4];
extern s16 lbl_eu_80664998[4];
extern s16 lbl_eu_806649A0[4];
extern s16 lbl_eu_806649A8[4];
extern s16 lbl_eu_806649B0[4];
extern s16 lbl_eu_806649B8[4];
extern s16 lbl_eu_806649C0[4];
extern s16 lbl_eu_806649C8[4];
extern s16 lbl_eu_806649D0[4];
extern s16 lbl_eu_806649D8[4];

// Same-TU grid helpers used by the equip-slot checks (defined in this .cpp;
// retail plain names). Return u32 (lbz zero-extends; avoids call-site masks).

// Shared item/bdat table id passed as BdatGetU8Direct's group arg by the
// equip-slot checks (same .sbss block as lbl_eu_806640F4).
extern void* lbl_eu_806640EC;
// Resource/flag helper: 0 when the named resource id is absent (CQuestLog).
extern "C" u32 CtrlRemote_TouchBitByArg(u32);

extern const float lbl_eu_80668B24;
// Item-kind -> display-count probe (calcEIBCountKey's default band).
extern "C" u32 __cvt_fp2unsigned(double);

/* 16-byte record copied off lbl_eu_8050EF68 by calcEIBCountKey and read back
   as floats indexed by the item's vf08 id (retail indexes past the 4 words
   with a masked shift, so the float view is oversized on purpose). */
union CEquipItemBoxF32Record {
    u32 w[4];
    float f[4];
};
extern CEquipItemBoxF32Record lbl_eu_8050EF68;

// Item-name string providers (CErrMes TU, current hint/error strings).
extern "C" char* getErrMesText16(void);
extern "C" char* getErrMesText15(void);
// Layout text bind (code_80135FDC unit): (layout, pane name, text, 0).
// Page-slot word-table accessors used by the grid-cursor scans.
extern "C" u16 ArrayGet12(const u16*, u8);
extern "C" void CopyTabSlotVec(nw4r::math::VEC3*, const u16*, u32);

// Small-data global read by getEIBNameKey (item-impl table id, .sbss).
extern void* lbl_eu_806640F4;

// .sdata2 float constants used by the grid page-count math (rebuildEIBGridPage).
extern const float lbl_eu_80668B00;
extern const float lbl_eu_80668B08;

// CfMove enum-list holder helpers (retail plain names; sibling decls in
// CItemBoxGrid.hpp / CfObjectImplMove.hpp).
extern "C" void startEnumObjects(void*, u32, u32);
extern "C" void* getEntryAt(void*, u32);
extern "C" void CfObjectPc_setupFrame(void* obj, u32 arg);
extern "C" void* getCfObjectPc__FPQ22cf12CfObjectMove(void* obj);

// Equipment-slot clear helpers (retail plain names; declared here instead of
// pulling in the big cf/CtrlObjectParam headers).
extern "C" void CtrlObjectParam_SetEquipSlot5(void*, int);
extern "C" void CtrlObjectParam_SetEquipSlot0(void*, int);
extern "C" void CtrlObjectParam_SetEquipSlot1(void*, int);
extern "C" void CtrlObjectParam_SetEquipSlot4(void*, int);
extern "C" void CtrlObjectParam_SyncParamFromActor(void*);

// Item-count / category-base readers (cf/CItem TU, retail plain names).
extern "C" u32 CItemBlock_countKindSlots(u8);
extern "C" s32 CItem_slotCnt(u32);   // canonical form from cf/CItem.hpp/CItem.cpp

// Char-data status lookup (code_8025FB10 TU): tests a flag at obj+0x3534.
extern "C" int Counter_TestBit(void*, u32);

// .sdata2 float/double constants used by the equip-box helpers (sda21
// relocs). 80668B10/80668B18 are the int->float conversion magics (0x4330
// trick); 80668B2C is the drop-rate scale; 80668B30/34/38 are pane offsets.
extern const float lbl_eu_80668B2C;
extern const float lbl_eu_80668B48;
extern const float lbl_eu_80668B4C;
extern const float lbl_eu_80668B30;
extern const float lbl_eu_80668B34;
extern const float lbl_eu_80668B38;
extern const float lbl_eu_80668B44;
extern const double lbl_eu_80668B10;
extern const double lbl_eu_80668B18;
extern const float lbl_eu_80668B20;

extern "C" void CItemBlock_setFlag120EC(u32 id, u32 arg);
