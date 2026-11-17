#pragma once

// Non-class helpers, overlay views and extern data used by the
// CEquipItemBox / CEquipChange units (kept out of CEquipItemBox.hpp so other
// includers do not inherit conflicting C-linkage declarations).
#include "kyoshin/CEquipItemBox.hpp"

/* CEventFile event object: the +0x4 word is the file handle compared against
   field_24/field_28/field_2C (the three bind files). */
class CEventFile;
struct CEquipItemBoxEventFileView {
    void* vtable;   // 0x0
    void* handle;   // 0x4
};

/* CFileHandle object read by OnFileEvent (r3 passed to Bdat_GetTable_AA34). */
struct CEquipItemBoxFileHandleView {
    void* vtable;   // 0x0
    void* field_4;  // 0x4
};

/* 8-byte colour-vector pair returned in r3:r4 by PaneMatGetS16QuadByName / PaneMatGetS16Quad
   and copied to the .sbss colour tables by CopyVec4s. */
struct CEquipItemBoxFourShorts {
    s16 a, b, c, d;
};

/* (font-info pane data goes through IDeviceFontInfo::getFont -- vtable
   offset 0x24; see monolib/device/CDeviceFont.hpp.) */

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

/* Pane size words at +0x4C / +0x50 (retail writes the texture dims here). */
struct CEquipItemBoxPaneSizeView {
    u8 pad00[0x4C];
    float mW;   // 0x4C
    float mH;   // 0x50
};

/* (cursor-member +0x08 dispatch goes through the real classes now:
   CEIBCur::initLayout / CEIBPageCur::initLayout / CBaseCur::initLayout.) */

/* Per-page store tables written by rebuildEIBKind2Cells: byte table at +0x2B8,
   byte table at +0x2C4 and s16 table at +0x2D0 (read by refreshEIBItemList). */
struct CEquipItemBoxPageStoreView {
    u8 pad00[0x2B8];
    u8 field_2B8[0xC];  // 0x2B8..0x2C3
    u8 field_2C4[0xC];  // 0x2C4..0x2CF
    s16 field_2D0[0xC]; // 0x2D0..0x2DF
};

// OnFileEvent helper imports (retail C-ABI names).
extern "C" void createRegion__17UnkClass_8045F564FiiPCci(void*, int, int, const char*, int);
extern "C" void __ct__14Class_8045F858FP17UnkClass_8045F564(void*, void*);
extern "C" void __dt__14Class_8045F858Fv(void*, int);
extern "C" void setMemInitFlag__Q23mtl10MemManagerFb(bool);
extern "C" bool Attach__Q34nw4r3lyt19ArcResourceAccessorFPvPCc(nw4r::lyt::ArcResourceAccessor*, void*, const char*);
extern "C" void* getFontInfo__11CDeviceFontFUlPQ34nw4r3lyt6Layout(u32, nw4r::lyt::Layout*);
// Font/line text providers: getPackedFont is the retail MANGLED name (C++
// linkage), the other two are plain C-ABI names.
char* getPackedFont();
extern "C" char* CUICfManager_getPackedFont9C();
extern "C" nw4r::lyt::ArcResourceAccessor* CUICfManager_getArcResourceAccessor();
void setLayoutTextBoxFont(nw4r::lyt::Layout*, char*, u32);
extern "C" int isClassicController__Q22cf13CfGameManagerFv(int);
extern "C" CEquipItemBoxFourShorts PaneMatGetS16QuadByName(void*, void*, u32);
extern "C" CEquipItemBoxFourShorts PaneMatGetS16Quad(void*, u32);
extern "C" void CopyVec4s(void*, const void*);
extern "C" void* Bdat_GetTable_AA34();  // bdat manager reset (no args; retail caller leaves r3 stale)
extern "C" void validateHeap__17UnkClass_8045F564Fv(void*);
// Bdat table slot written by the field_2C file handler.
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

/* Copy 8-byte CEquipItemData struct from src to dst */
void copyEquipItemData(CEquipItemData* dst, const CEquipItemData* src);

/* Item-impl vtable view exposing the slots the grid sorts dispatch: vf08
   (0x08), vf30 (0x30) and vf90 (0x90) carry the sort keys, plus the two
   slots fmtEIBItemName reads (vf08 / vf20). MWCC auto-inserts RTTI + null
   at vtable slots 0-1, so the declared virtuals land at slots 2..n.
   Declarations are padded out to 0x90 so the sort-key slot offsets match
   retail's vtable. */
class CEquipItemBoxItemImplView {
public:
    virtual u32 vf08(CItemInstance* p);  // 0x08
    virtual void vf0C(CItemInstance* p); // 0x0C
    virtual void vf10(CItemInstance* p); // 0x10
    virtual void vf14();                 // 0x14
    virtual void vf18();                 // 0x18
    virtual void vf1C(CItemInstance* p); // 0x1C
    virtual u32 vf20(CItemInstance* p);  // 0x20
    virtual void vf24(CItemInstance* p); // 0x24
    virtual void vf28(CItemInstance* p); // 0x28
    virtual CItemInstance* vf2C(CItemInstance* p, u8 idx); // 0x2C (slot entry getter)
    virtual u32 vf30(CItemInstance* p);  // 0x30 (item-count sort key)
    virtual void vf34(CItemInstance* p); // 0x34
    virtual void vf38(CItemInstance* p); // 0x38
    virtual void vf3C(CItemInstance* p); // 0x3C
    virtual s16 vf40(CItemInstance* p, u8 idx);  // 0x40 (slot id, -1 = empty)
    virtual void vf44(CItemInstance* p, u8 idx, s32 val); // 0x44 (slot-id setter)
    virtual void vf48(CItemInstance* p); // 0x48
    virtual void vf4C(CItemInstance* p); // 0x4C
    virtual void vf50(CItemInstance* p); // 0x50
    virtual u32 vf54(CItemInstance* p);  // 0x54 (name-key sort)
    virtual void vf58(CItemInstance* p); // 0x58
    virtual void vf5C(CItemInstance* p); // 0x5C
    virtual void vf60(CItemInstance* p); // 0x60
    virtual void vf64(CItemInstance* p); // 0x64
    virtual void vf68(CItemInstance* p); // 0x68
    virtual void vf6C(CItemInstance* p); // 0x6C
    virtual void vf70(CItemInstance* p); // 0x70
    virtual void vf74(CItemInstance* p); // 0x74
    virtual void vf78(CItemInstance* p); // 0x78
    virtual void vf7C(CItemInstance* p); // 0x7C
    virtual u32 vf80(CItemInstance* p);  // 0x80
    virtual void vf84(CItemInstance* p); // 0x84
    virtual void vf88(CItemInstance* p); // 0x88
    virtual void vf8C(CItemInstance* p); // 0x8C
    virtual u32 vf90(CItemInstance* p);  // 0x90 (name-key sort)
};

/* (CCur18 position updates go through CBaseCur::setRootPaneTranslate --
   vtable offset 0x10; see kyoshin/CBaseCur.hpp.) */

/* Layout-object view: the pane-finder sub-object sits at +0x10 (same shape
   as CLayoutView in CEquipChange.hpp; local copy because this TU does not
   include that header). */
struct CEquipItemBoxLayoutView {
    u8 _pad[0x10];
    void* field_10;   // 0x10
};

/* (pane lookup goes through nw4r::lyt::Pane::FindPaneByName -- vtable offset
   0x3C; the layout object is reached through CEquipItemBoxLayoutView below.) */

/* Pane-layout view exposing the translate at +0x2C (Pane::mTranslate is
   protected in the nw4r header, so the equip-box helpers reach it through
   this fixed-offset mirror). */
struct CEquipItemBoxPaneView {
    u8 pad00[0x2C];
    float mTranslate[3];  // 0x2C
};

/* u32->float conversion scratch (CPartsChange.cpp convention): builds the
   0x43300000-prefixed bit pattern by hand so the magic subtraction below
   references the retail .sdata2 constants (lbl_eu_80668B18/80668B10)
   instead of MWCC's un-nameable pool entry. */
union CEquipItemBoxF64Conv {
    u32 w[2];
    double d;
};

/* Base position of the equip-box page cursor: the 3 words at +0x200 are
   copied by refreshEIBPageDisp and shifted by the row offset. Read as u32 so the
   copy uses lwz/stw (retail shape). */
struct CEquipItemBoxPagePosView {
    u8 pad00[0x200];
    u32 pos[3];   // 0x200..0x20B
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

/* Item object returned by CtrlObjectParam_GetCharEntry: 6 slot-id shorts at 0x1c..0x26
   supply the item ids patched into the slot table by findEIBEquipSlot. */
struct CEquipItemBoxItemView {
    u8 _pad00[0x1c];
    s16 field_1c;   // 0x1c
    s16 field_1e;   // 0x1e
    s16 field_20;   // 0x20
    s16 field_22;   // 0x22
    s16 field_24;   // 0x24
    s16 field_26;   // 0x26
};

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

/* Overlay for the per-page item-id table at +0x210 (u16) and the 12 page
   position VEC3 records at +0x228, reset by resetEIBPageView. */
struct CEquipItemBoxPageStateView {
    u8 pad00[0x210];
    u16 field_210[0xC];       // 0x210..0x227
    float field_228[0xC][3];  // 0x228..0x2FF
};

/* Stack-allocated enum-list holder/list used by useEIBSelectedItem's CfMove
   search (__ct__16CfEnumListHolderFv ctor / get__16CfEnumListHolderFv getter / __dt__16CfEnumListHolderFv dtor;
   the list's element count sits at +0x620). */
struct CEquipItemBoxEnumList {
    u8 _pad00[0x620];
    u32 field_620;  // 0x620
};

// C-linkage imports (retail symbol names - keep linkage/signatures verbatim)
extern "C" int CSysWin_isActive(void*);
extern "C" void Cur_SetVisible(void*, int);
extern "C" u32 CSysWin_isReady(void*);
extern "C" void BuildSlotItemLists(void*);
extern "C" u8 GetCollectedFlagByte(u32);
extern "C" void sysWinAdvancePhase3(void*);
// Same-TU helpers (defined as extern "C" free functions in CEquipItemBox.cpp;
// the retail symbols are unmangled, so calls must reference the plain names).
// Forward decls for the grid/array helpers (defined in CEquipItemBox.cpp).
extern "C" CEquipItemData* setEIBItem(CEquipItemData* dst, s16 a0, u8 a2, u8 a3, u8 a4, u8 a5, u8 a6, u8 a7);
extern "C" void copyEIBItem(CEquipItemData* dst, const CEquipItemData* src);
// String pool used by getEIBPageHint (offsets 0x2d / 0x36) and others.
extern "C" char lbl_eu_8050EFDC[];
extern "C" void closeFileHandle__FPP11CFileHandle(void*);
extern "C" void releaseArcResourceAccessor__FPQ34nw4r3lyt19ArcResourceAccessor(void*);
extern "C" void deleteRegion__17UnkClass_8045F564Fv(void*);
extern "C" void sysWinTermLayout(void*);
extern "C" u16 BdatGetItemId(u32);
// Texture-name lookup (retail C-ABI; u16 arg, returns the resource name).
extern "C" char* MakeTplNameSysFile(u32);
// Item-object validity/type check used by bindEIBCellIcon's texture pick.
extern "C" int IsSkillItem(void*);
extern "C" void* CtrlObjectParam_GetCharEntry(u32);
// Item randomizer: reads the low 16 bits of the first word of the object
// returned by CtrlObjectParam_GetCharEntry (cf/CtrlObjectParam TU; callers pass that obj).
extern "C" u32 CtrlObjectParam_GetArtsDataWord(void*);
// Item drop-rate helpers (code_80135FDC TU): name-table float probe and the
// (rateA + rateB) clamp divisor shared by the equip-box/line stat displays.
extern "C" f32 GetFloatTableEntry(u32 idx);
extern "C" f32 BlendFloatAvgScale(u32 a, u32 b, u32 c, f32 d);
extern "C" void copyVEC3(void*, const void*);
// Declared with C++ linkage (same as CCol6System.hpp) so the two headers
// don't create an illegal overload when a TU includes both.
void* CItem_initItemImplInstances(void*);
extern u32 lbl_eu_806640D8;
extern u32 lbl_eu_806640F8;
extern "C" int CSysWin_getUnk34(void*);
extern "C" void setPaneVisible(void*, u32);
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

/* Fields written by rebuildEIBKind3Page (kind-3 rebuild): the selected kind halfword
   at +0x220, the fixed state byte 3 at +0x2c0, the vf08 byte at +0x2cc, the
   cursor anchor position copied to +0x288, and the embedded CItemBoxInfo at
   +0x210 that receives ((CItemBoxPage*)(8))->SetValue(vf90). */
struct CEquipItemBoxKind3View {
    u8 pad00[0x220];
    u16 field_220;              // 0x220 selected item kind (u16)
    u8 pad222[0x288 - 0x222];
    float field_288[3];         // 0x288 cursor anchor VEC3
    u8 pad294[0x2c0 - 0x294];
    u8 field_2c0;               // 0x2c0 fixed state byte (3)
    u8 pad2c1[0x2cc - 0x2c1];
    u8 field_2cc;               // 0x2cc item vf08 byte
};
// Item-box list refresh helpers (CItemBoxInfo TU, retail plain names).
extern "C" u8 ItemBoxGetTab(const u8*, unsigned int);
// Layout text/visibility bind (retail mangled setLayoutTextBoxNumber__FPQ34nw4r3lyt6LayoutPcUc).
void setLayoutTextBoxNumber(nw4r::lyt::Layout*, char*, u8);
// Same-TU grid helper (retail plain name; defined in this .cpp).
// Same-TU dispatch helpers used by sortEIBGridByPage (defined in this .cpp).
// Same-TU grid sort helpers dispatched by sortEIBGridByPage (defined below).
// Same-TU grid helpers used by the equip-list refresh (retail plain names).
extern "C" u16 CItemBlock_getFlag120EC(int);
// Per-frame update helpers (retail plain names, defined in sibling TUs).
extern "C" int KyoshinHeap_GetField44(void);
extern "C" void Cur_Update(void*);
extern "C" void sysWinDispatchPhase(void*);
// Sub-window / cursor / sort-menu draw helpers (retail plain names).
extern "C" void sysWinDrawLayout(void*, nw4r::lyt::DrawInfo*);
extern "C" void Cur_DrawLayout(void*, void*);
// Layout text/colour setter used by the sort-menu page rebuild (refreshEIBSortTabs);
// the 3rd/4th args are .sbss colour-table pairs referenced via sda21.
extern "C" void PaneMatSetTevColorsByName(nw4r::lyt::Layout*, char*, void*, void*);
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
// Sort-menu page handler called once per page by refreshEIBSortTabs (this TU).
// Kind-specific equip-box rebuild tail (this TU, retail plain name).

// Same-TU grid helpers used by the equip-slot checks (defined in this .cpp;
// retail plain names). Return u32 (lbz zero-extends; avoids call-site masks).

// Shared item/bdat table id passed as BdatGetU8Direct's group arg by the
// equip-slot checks (same .sbss block as lbl_eu_806640F4).
extern u32 lbl_eu_806640EC;
// Resource/flag helper: 0 when the named resource id is absent (CQuestLog).
extern "C" u32 CtrlRemote_TouchBitByArg(u32);
// Name-table pointer globals (handleEIBConfirm).
extern "C" void incrementEventCounter(u32);
extern u32 lbl_eu_80668B3C;
extern u16 lbl_eu_80668B40;
// 12-byte texture/description table at 0x8050EF84 (3 consecutive words).
extern u8 lbl_eu_8050EF84;
// SysWin sub-window text setters used by handleEIBNamePane's name-pane confirm.
extern "C" void sysWinSwitchKindPane(void*, u32);
extern "C" void sysWinSetKindContent(void*, void*, u32);
extern "C" void sysWinSetTwoTextValues(void*, void*, void*);
extern "C" void sysWinApplyKind(void*, u32);
extern "C" void sysWinOpenPhase1(void*);
// Per-slot detail-text builder (handleEIBNamePane's ==3 lookup path).
extern const float lbl_eu_80668B24;
// Item-kind -> display-count probe (calcEIBCountKey's default band).
extern "C" u32 LookupItemTypeTable(u32);
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
// Number-of-equip-pages byte (shared .sbss flag; > 1 means multi-page).
extern "C" u8 code80135FDC_getByte_64077();
// Sort-page colour table pairs (PaneMatSetTevColorsByName 3rd/4th args, .sbss).
extern s16 lbl_eu_806649B0[4];
extern s16 lbl_eu_806649B8[4];
extern s16 lbl_eu_806649C0[4];
extern s16 lbl_eu_806649C8[4];
extern s16 lbl_eu_806649D0[4];
extern s16 lbl_eu_806649D8[4];

// Small-data global read by getEIBNameKey (item-impl table id, .sbss).
extern u32 lbl_eu_806640F4;

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
extern "C" void CtrlObjectParam_SetEquipSlot2(void*, int);
extern "C" void CtrlObjectParam_SetEquipSlot3(void*, int);
extern "C" void CtrlObjectParam_SetEquipSlot4(void*, int);
extern "C" void CtrlObjectParam_SyncParamFromActor(void*);

// Item-count / category-base readers (cf/CItem TU, retail plain names).
extern "C" u32 CItemBlock_countKindSlots(u8);
extern "C" u32 CItem_slotCnt(u8);

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

// Item-impl accessor (retail CItemImpl vtable-indexed helper).
extern "C" u32 BdatGetItemType(u32);

// Sub-object destructors invoked by ~CEquipItemBox (retail C-ABI names; the
// member-class dtors live in their own TUs as extern "C" free functions).
extern "C" void __dt__7CSysWinFv(void*, int);
extern "C" void __dt__17UnkClass_8045F564Fv(void*, int);
extern "C" void* __dt__80285C44(void*, int);

// Constructor imports (retail C-ABI names; the member ctors live in their
// own TUs as extern "C" free functions).
extern "C" void __ct__17UnkClass_8045F564Fv(void*);
extern "C" void __ct__7CSysWinFUc(void*, int);
extern "C" void __as__17UnkClass_8045F564FRC17UnkClass_8045F564(void* dst, void* src);
extern "C" void CItemBlock_setFlag120EC(u32 id, u32 arg);
extern "C" void ItemBlockSetDefaultFlags();
