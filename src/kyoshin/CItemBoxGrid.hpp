#pragma once

#include <types.h>
class CBaseCur;
class UnkClass_8045F564;
namespace nw4r { namespace lyt {
class AnimTransform; class Layout; class ArcResourceAccessor; class DrawInfo;
} }

// Retail vtable (lbl_eu_80534818): dtor at +0x08, Rebuild at +0x0C
// (retail func_801C4BB4). novtable: the table lives in the data blob and
// the ctor stores the label manually, so no TU emits a __vt__ (same shape
// as CBaseCur/CItemBoxObjVt08 delete sites); Rebuild has no body here -
// virtual calls land on the blob's slot.
class __declspec(novtable) CQuestItem {
public:
    virtual ~CQuestItem();  // +0x08
    virtual void Rebuild();  // +0x0C - retail func_801C4BB4

    // TODO: add fields
};

// Retail vtable (lbl_eu_805347F8): dtor at +0x08, Rebuild at +0x0C
// (retail func_801C5254). novtable, no Rebuild body (see CQuestItem).
class __declspec(novtable) CVisionItem {
public:
    virtual ~CVisionItem();  // +0x08
    virtual void Rebuild();  // +0x0C - retail func_801C5254

    // TODO: add fields
};

// Retail vtable (lbl_eu_805347D8): dtor at +0x08, Rebuild at +0x0C
// (retail func_801C53D8). novtable, no Rebuild body (see CQuestItem).
class __declspec(novtable) CArtsBookItem {
public:
    CArtsBookItem();
    virtual ~CArtsBookItem();  // +0x08
    virtual void Rebuild();  // +0x0C - retail func_801C53D8

    // TODO: add fields
};

// 7-float price scale row copied out of the lbl_eu_8050560C pool by the
// price functions; retail lowers the copy as an inline struct assignment.
struct CItemPriceScaleRow {
    float v[7];
};

// 7-word per-language slot-name table copied wholesale by func_801C6A44's
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

// Plain 3-float vector passed by value into func_801D24E8 (the caller-side
// struct copies land in the argument area).
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

// 24-byte category-filter row copied around by value in func_801C56D8
// (MWCC lowers the struct copies to unrolled lwz/lwzu pairs).
struct GridFilter24 {
    u32 w[6];
};

// 10-byte grid cell: stored item id + 8 per-cell flag bytes
struct CItemBoxGridEntry {
    s16 id;        // +0x00 item id (or -1 for an empty cell)
    u8 flags[8];   // +0x02 per-cell flag bytes
};

// Sort view over a fetched item object: the first word feeds the generic
// comparison key (word >> 20 through BdatGetItemId), the flag byte at +0x07
// feeds the mode-9 tie-break (bits 24..29).
struct CItemSortView {
    u32 word00;
    u8 _04[3];
    u8 flags7;
};

// Full object layout for CItemBoxGrid (used by C-linkage accessors)
struct CItemBoxGridFull {
    union {
        CItemBoxGridEntry entries[0x280];  // cells 0x0000..0x2800
        struct {
            u8 _00[0x61];
            u8 field_61;
            u8 _62[0x52D - 0x62];
            u8 field_52D;
            u8 _52E[0x549 - 0x52E];
            u8 field_549;
            u8 _54A[0x2800 - 0x54A];
        } bytes;
    };
    u16 field_2800;  // entry count
    u8 field_2802;   // current category
    u8 field_2803;   // row count
    u8 field_2804;   // page index
};

class CItemBoxGrid {
public:
    CItemBoxGrid();
    virtual ~CItemBoxGrid();
    void OnFileEvent();

    u8 GetField61();
    u8 GetField549();
    u8 GetField52D();
    void PushToList(unsigned char val);

    // TODO: add fields
};


// C-linkage imports (retail symbol names - keep linkage/signatures verbatim)
extern "C" int func_8022DB74(void*);
extern "C" void __dl__FPv(void*);
extern "C" void* func_801394D4(u32);
extern "C" void func_801D4AE0(void*, int, void*);
extern "C" void RebuildGridCat(void*);
extern "C" int CSysWin_isActive(void*);
extern "C" int CSysWin_getUnk34(void*);
extern "C" void func_801D216C(void*, u8);
extern "C" __declspec(noinline) void UpdateCursors(void*);
extern "C" u32 AnimRewindFrame(nw4r::lyt::AnimTransform*, float);
extern "C" void* func_80157C4C(u32, s16);
extern "C" u32 BdatGetItemType(u32);
extern "C" u32 BdatGetU8Direct(u32, const char*, u32);
extern "C" u32 BdatGetItemId(u32);
// CfGameManager.hpp (included transitively) now declares the unified
// pointer-arg form, matching every call site here directly - no cast needed.
extern "C" u32 func_801D3320(void*);
extern "C" void CItemData_initFromFamily(void*, u32);
extern "C" u32 func_80208360(void*);
extern "C" void func_80208760(void*, void*);
extern "C" void func_8022E498(void*, void*);
extern "C" void func_8022D0F8(void*, void*, u8);
extern "C" u32 func_801D32DC(void*);
extern "C" u32 getItemBoxState__FP12CItemBoxInfo(void*);
extern "C" u32 func_801EB018(void*);
extern "C" u32 CSysWin_isReady(void*);
extern "C" u32 func_801EB020(void*);
extern "C" u32 func_80208358(void*);
extern "C" u32 func_8022DB6C(void*);
extern "C" void advanceItemBoxState__FP12CItemBoxInfo(void*);
extern "C" void func_801EB410(void*, int);
extern "C" void func_801EB0D4(void*);
extern "C" void func_801D0328(void*);
extern "C" void func_801CFFEC(void*);
extern "C" u32 GetCatLabelMsg(void*);
extern "C" char* func_801C6A44(void*, u16);
extern "C" void func_801D05D4(void*, int);
extern "C" void playUISound__FUl(u32);
extern "C" u16 CItemBlock_getFlag120EC(int);
extern "C" int getLanguage__9CDeviceSCFv();
extern "C" u32 func_801D3328(void*);
extern "C" void func_801D3454(void*, void*);
extern "C" void func_801D3430(void*, void*);
extern "C" void func_801D353C(void*, u8);
extern "C" void func_801D3330(void*);
extern "C" u32 func_801D3C74(void*, u32);
extern "C" void func_801D4BDC(void*, u32, u32);
extern "C" void func_801D3620(void*);
extern "C" void func_801D3698(void*);
extern "C" void func_801D3724(void*);
extern "C" void func_801D377C(void*);
extern "C" void func_801D3408(void*);
extern "C" u32 func_801EB028(void*);
extern "C" void func_801EB064(void*, u32);
extern "C" void func_801EB04C(void*, u8);
extern "C" u32 func_801EB218(void*);
extern "C" u32 func_801EB314(void*);
extern "C" void func_801EB178(void*);
extern "C" void func_8020844C(void*);
extern "C" u32 func_802087B8(void*);
extern "C" char* MakeTplNameSysFile(u32);
extern "C" int* func_8009ECB0(void);
extern "C" void CItemBlock_setFlag120EC(u8, u32);
extern "C" void* CItemBlock_getPtr20E8(void);
extern "C" void CItemBlock_setCount(int);
extern "C" void func_801EB030(void*, void*);
extern "C" void incrementEventCounter__FUl(u32);
extern "C" void* getFontInfo__11CDeviceFontFUlPQ34nw4r3lyt6Layout(u32, nw4r::lyt::Layout*);
extern "C" void getEntry__5CBdatFUl(u32);
extern "C" void closeFileHandle__FPP11CFileHandle(void**);
extern "C" void releaseArcResourceAccessor__FPQ34nw4r3lyt19ArcResourceAccessor(void*);
extern "C" void deleteRegion__17UnkClass_8045F564Fv(void*);
extern "C" void func_8022B7F4(void*);
extern "C" void func_802083A4(void*);
extern "C" void func_80208838(void*);
extern "C" void func_8022E3A8(void*);
extern "C" int func_8022E490(void*);
extern "C" void func_8022E2F8(void*);
extern "C" void func_8022E3AC(void*);
extern "C" u32 func_8022E488(void*);
extern "C" void func_8022DD68(void*);
extern "C" void func_8022D0D0(void*);
extern "C" void func_8022B8E4(void*);
extern "C" void func_8022C1B4(void*, void*, u8);
extern "C" void func_801375A0(void*, void*);
extern "C" void func_801D47D4(void*, u32, u32, u32);
extern "C" u32 func_801D421C(void*);
extern "C" void func_801D4C3C(void*, u32);
extern "C" void func_80124270(void*, u32);
extern "C" void PaneSetTexPaletteByName(void*, void*, void*);
extern "C" void func_80137924(void*, void*, void*, void*);
extern "C" u8 BdatGetU8ByTableKey(const void*, const void*, u32);
extern "C" void LayoutSetTextBoxFmtValue(nw4r::lyt::Layout*, char*, char*, u32);
extern "C" void func_80136A1C(nw4r::lyt::Layout*, char*, char*, u32);
extern "C" char* BdatGetPtrDirect(const void*, const void*, int);
extern "C" u32 func_800A32BC();
extern "C" void copyVEC3(void*, const void*);
extern "C" u32 func_8003B1EC(void*);
extern "C" u32 func_8009CF8C(u32);
extern "C" void CTaskGame_enumListCtor(void*);
extern "C" void* CTaskGame_enumListGet(void*);
extern "C" void func_800F4A98(void*, u32, u32);
extern "C" void* func_800F6EC0(void*, u32);
extern "C" void* getCfObjectPc__FPQ22cf12CfObjectMove(void*);
extern "C" void func_800BFDE0(void*, u32);
extern "C" void func_80043E88(void*, int);
extern "C" void func_8009E0A8(void*, int);
extern "C" void func_8009E024(void*, int);
extern "C" void func_8009E030(void*, int);
extern "C" void func_8009E03C(void*, int);
extern "C" void func_8009E048(void*, int);
extern "C" void func_8009E054(void*, int);
extern "C" void PaneMatSetTevColorsByName(void*, void*, void*, void*);
extern "C" char* BdatTouchStringCell(const void*, const void*, int);
extern "C" void func_801C5158(void*, u32);
extern "C" u8 GetCollectedFlagByte(u32);
extern "C" u32 func_801393CC(u32);
extern "C" int func_8026178C(void*, u32);
extern "C" u32 func_8025FB10(void*, u32);
extern "C" u32 func_802083CC(void*);
extern "C" void* func_8009EC9C(u32);
extern "C" u32 func_800A082C(void*);
extern "C" u8 code80135FDC_getByte_64077();
extern "C" void func_801C4B60(void*, s16, s16, s16, s16);
extern "C" void SplitU32ToS16s(short*, unsigned long);
extern "C" u32 GetCellKind(CItemBoxGridFull*, u16);
extern "C" void* GetCellItemPtr(CItemBoxGridFull*, u16);
extern "C" u16 ArrayGet12(const unsigned short*, unsigned char);
extern "C" char* func_eu_802B148C(void);
extern "C" char* func_eu_802B1474(void);
extern "C" int HasSlotIdMatch(void* obj, u32 id);
extern "C" u8 CountMarkedCells(CItemBoxGridFull* self);
extern "C" void func_8022B90C(void*, int);
extern "C" void func_8022B9B4(void*, u32, int);
extern "C" void func_8022BFC8(void*, int);
extern "C" void func_8022B8B8(void*);
extern "C" int IsSkillItem(void*);
extern "C" u32 func_801D4260(void*, u8);
extern "C" void func_801D0BD8(void*);
extern "C" void func_801CFD2C(void*);
extern "C" u32 CItemBlock_countKindSlots(u8);
extern "C" void FinishCloseAnim(void*);
extern "C" float func_801C9F88(void*, void*);
extern "C" u32 HasEquippedSlot(void*, void*);
extern "C" u32 func_801C7958(void*, void*);
extern "C" u32 advanceAnimTransform__FPQ34nw4r3lyt13AnimTransformf(void*, float);
extern "C" char* CopyEntry9Bytes(char*, const char*);
extern "C" void* getHandleMEM2__Q23mtl10MemManagerFv();
extern "C" void* getHandleMEM1__Q23mtl10MemManagerFv();
extern "C" u32 getMaxAllocSize__Q23mtl10MemManagerFUl(u32);
extern "C" void deallocate__Q23mtl10MemManagerFPv(void*);
extern "C" u32 func_80212480();
extern "C" int KyoshinHeap_GetField44();
extern "C" void* readFile__11CDeviceFileFUlPCcP10IWorkEventii(u32, const char*, void*, int, int);
extern "C" void* readCommonArchiveFile__11CDeviceFileFUlPCcP10IWorkEventii(u32, const char*, void*, int, int);
extern "C" void func_801D4154(void*);
extern "C" void drawLayout__FPQ34nw4r3lyt6LayoutPQ34nw4r3lyt8DrawInfoii(nw4r::lyt::Layout*, nw4r::lyt::DrawInfo*, int, int);
extern "C" void func_801D20B0(void*, void*);
extern "C" void func_801D31F8(void*, void*);
extern "C" void func_802082D0(void*, int);
extern "C" void func_8022CFEC(void*, int);
extern "C" void func_8022B7C8(void*, void*);
extern "C" void func_8022DAD8(void*, int);
extern "C" void func_801EAF7C(void*, void*);
extern "C" void func_801D202C(void*);
extern "C" void func_801D3160(void*);
extern "C" void func_801D40C4(void*);
extern "C" void func_801EAED4(void*);
extern "C" void func_80208260(void*);
extern "C" void func_8022CF7C(void*);
extern "C" void func_8022DA58(void*);
extern "C" void func_8022B748(void*);
extern "C" void func_801D3064(void*);
extern "C" void func_801D4054(void*);
extern "C" void func_801EAE8C(void*);
extern "C" void func_8022CF2C(void*);
extern "C" void func_801D3258(void*);
extern "C" void func_801D4174(void*);
extern "C" void func_801EAF9C(void*);
extern "C" void func_802082F0(void*);
extern "C" void func_8022D018(void*);
extern "C" void func_8022DB04(void*);
extern "C" void* getFP__FPCc(const char*);
extern "C" u16 BdatGetU16Direct(const void*, const void*, int);
extern "C" void __ct__CVisionItem(void*);
extern "C" void __ct__CArtsBookItem(void*);
extern "C" unsigned char* SetEntry9Bytes(unsigned char*, short, unsigned char, unsigned char, unsigned char, unsigned char, unsigned char, unsigned char, unsigned char);  // returns p (retail ctor reuses caller-preserved r3)
extern "C" void CopyGridEntry9(void*, void*);
extern "C" void func_801C7EF0(CItemBoxGridFull*, u32);
extern "C" void GroupBySlotStat(CItemBoxGridFull*, u32);
extern "C" void GroupBySlotAlt(CItemBoxGridFull*, u32);
extern "C" void GroupByCapKey(CItemBoxGridFull*, u32);
extern "C" void GroupByKindName(CItemBoxGridFull*, u32);
extern "C" u32 func_801576C8(u8);
extern "C" void func_8022E204(void*, u32);
extern "C" void func_8022DB7C(void*);
extern "C" void func_8022DD90(void*);
extern "C" u32 func_8022E4FC(void*);
extern "C" u32 func_8022E504(void*);
extern "C" u32 func_801D37F4(void*);
extern "C" u32 func_801D3808(void*);
extern "C" u32 func_801D3810(void*);
extern "C" void func_8022D0A4(void*);
extern "C" void func_8022D19C(void*, char*, u32);
extern "C" void func_80208368(void*);
extern "C" void func_802084D4(void*, u32);
extern "C" void DispatchItemEvt(void*, void*, int);
extern "C" void func_8022BF6C(void*, u32, u32);
extern "C" u32 func_801D12D4(void*, u32);
extern "C" void CItem_thunkAllocRecord(u32, u32);
extern "C" void func_80140E00(u32, u32, u32);
extern "C" u32 isResourceFlagSet__Q22cf13CfGameManagerFv(u32);
extern "C" void func_800A18A4(void*, u8);
extern "C" u32 func_801380A0(u16);
extern "C" u32 func_80138138(u16);
// UIWin_BuildFlagBuf is owned by kyoshin/CUIWindowManager.hpp.

extern "C" void* getHandleMEM2__Q23mtl10MemManagerFv(void);
extern "C" void* createRegion__17UnkClass_8045F564FiiPCci(void*, void*, int, const char*, int);
extern "C" void __ct__14Class_8045F858FP17UnkClass_8045F564(void*, void*);
extern "C" void setMemInitFlag__Q23mtl10MemManagerFb(bool);
extern "C" void* getAllocHandle__10CLibLayoutFv(void);
extern "C" void* allocate__Q23mtl10MemManagerFUlUl(u32, u32);
extern "C" void* __ct__CTagProcessor(void*);
extern "C" nw4r::lyt::ArcResourceAccessor* createArcResourceAccessor__10CLibLayoutFv();
extern "C" bool Attach__Q34nw4r3lyt19ArcResourceAccessorFPvPCc(nw4r::lyt::ArcResourceAccessor*, void*, const char*);
extern "C" void buildLayout__FPPQ34nw4r3lyt6LayoutPQ34nw4r3lyt19ArcResourceAccessorPCc(nw4r::lyt::Layout**, nw4r::lyt::ArcResourceAccessor*, const char*);
extern "C" void bindLayoutAnimTransform__FPQ34nw4r3lyt6LayoutPPQ34nw4r3lyt13AnimTransformPQ34nw4r3lyt19ArcResourceAccessorPc(nw4r::lyt::Layout*, nw4r::lyt::AnimTransform**, nw4r::lyt::ArcResourceAccessor*, char*);
extern "C" void func_8013676C(void*, u32);
extern "C" void* getPackedFont__Fv(void);
extern "C" void* CUICfManager_getPackedFont9C(void);
extern "C" void setLayoutTextBoxFont__FPQ34nw4r3lyt6LayoutPcUl(nw4r::lyt::Layout*, char*, u32);
extern "C" void func_80139198(u32);
extern "C" u16 BdatGetU16ByTableKey(const void*, const void*, u32);
extern "C" nw4r::lyt::ArcResourceAccessor* CUICfManager_getArcResourceAccessor();
// Pane-colour vector returned by value from func_801397AC/func_80139658
// (r3:r4 pair); same layout as CEquipItemBox.hpp's CEquipBoxFourShorts,
// defined locally because CEquipItemBox.hpp is not in this TU's include closure.
struct CEquipBoxFourShorts {
    s16 a, b, c, d;
};
extern "C" CEquipBoxFourShorts func_80139658(void*, const char*, u32);  // returns 4x s16 colour by value (r3:r4 pair)
extern "C" void CopyVec4s(void*, void*);
extern "C" CBaseCur* __ct__CCur07(void*, void*);
extern "C" void __ct__CSortMenu(void*);
extern "C" void __ct__CItemBoxInfo(void*, int, int);
extern "C" void __ct__CNumSelect(void*, int);
extern "C" void __ct__CItemBoxGridSubMenu(void*);
extern "C" void __ct__CExchangeWin(void*);
extern "C" void __ct__CPresentWin(void*);
extern "C" void __ct__CSysWin(void*, int);
extern "C" void __ct__UnkClass_8011C974(void*, void*);
extern "C" void func_8016742C(void*, void*);
extern "C" void ItemBlockSetDefaultFlags();
extern "C" void* __ct__801C5514(void*);
extern "C" CBaseCur* __ct__CCur09(void*, void*);
extern "C" CBaseCur* __ct__CCur18(void*, void*);
extern "C" CBaseCur* __ct__CCur16(void*, void*);
extern "C" CBaseCur* __ct__CCur11(void*, void*);
extern "C" void __dt__6CCur07Fv(void*, int);
extern "C" void __dt__6CCur09Fv(void*, int);
extern "C" void __dt__6CCur18Fv(void*, int);
extern "C" void __dt__6CCur16Fv(void*, int);
extern "C" void __dt__6CCur11Fv(void*, int);
extern "C" void __dt__7CSysWinFv(void*, int);
extern "C" void __dt__11CPresentWinFv(void*, int);
extern "C" void __dt__12CExchangeWinFv(void*, int);
extern "C" void __dt__19CItemBoxGridSubMenuFv(void*, int);
extern "C" void __dt__10CNumSelectFv(void*, int);
extern "C" void __dt__12CItemBoxInfoFv(void*, int);
extern "C" void __dt__9CSortMenuFv(void*, int);
extern "C" void __dt__17UnkClass_8045F564Fv(void*, int);
extern "C" void func_8018B0FC(void*, void*);
extern "C" void func_8018B130(void*, void*);
extern "C" void code80135FDC_setVec3(float*, float, float, float);
extern "C" void func_801D24E8(void*, CibgVec3, CibgVec3);
extern "C" void setBdatEntry__5CBdatFUlPv(u32, void*);
extern "C" void __dt__14Class_8045F858Fv(void*, int);
extern "C" void validateHeap__17UnkClass_8045F564Fv(void*);
extern "C" void func_8022D614(void*, void*);
extern "C" void func_80207FC8(void*, void*);
extern "C" CEquipBoxFourShorts func_801397AC(void*, u32);
extern "C" int isClassicController__Q22cf13CfGameManagerFv(int);

// ---- func_801D0E88 imports/helpers ----
extern "C" void func_800A1370(void*);
extern "C" void __dt__80043E88(void*, int);

// Stack-scene enum-list holder used by func_801D0E88's camera block: the
// 12-byte name table at +0x10 is written before the CTaskGame_enumListCtor ctor call.
struct CItemBoxGridEnumHolder {
    void* field_00;
    u32 field_04;
    u32 names[3];
};
// Enum list returned by CTaskGame_enumListGet: element count at +0x620.
struct CItemBoxGridEnumList {
    u8 _pad00[0x620];
    u32 field_620;
};
// Slot from func_800F6EC0: move object pointer at +0x04.
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
// of the CSortMenu/CItemBoxInfo/CNumSelect/CExchangeWin stack temporaries and
// the 801C5514 sub-grid temp. Named scalars lower to individual moves while
// array members lower to paired lwzu/stwu bdnz loops, matching retail.
struct CibgSortBlk1 {  // CSortMenu temp +0x04, size 0x28
    u32 a, b, c, d, e;
    u8 f, g, h, i;
};
struct CibgWords16 {
    u32 w[16];
};
struct CibgWords6 { u32 w[6]; };
struct CibgWords3 { u32 w[3]; };
struct CibgBytes64 { u8 b[0x40]; };
struct CibgBytes32 { u8 b[0x20]; };
struct CibgSortBlk2 {  // CSortMenu temp +0x30, size 0xAC (tail array loops)
    u32 a, b, c, dd;
    u8 e0, e1, e2, e3;
    float f0, f1, f2, f3, f4;
    u8 last;
    CibgWords16 tail;
};
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
// __ct__UnkClass_8011C974 helper (same-TU inline folds the wrapper away,
// leaving the single bl the retail assignments show).
class CibgUnk16 {
public:
    u8 d[0x10];
    CibgUnk16& operator=(const CibgUnk16& o) {
        __ct__UnkClass_8011C974((void*)d, (void*)o.d);
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
struct CibgSortMember {   // p+0xec..0x1d3 <- CSortMenu temp +4
    CibgUnk16 unk;
    u32 a, b, c, dd, e;
    u8 f, g, h, i;
    CibgSkip4 pad28;
    CibgUnk16 unk2;
    u32 w0, w1, w2, w3;
    u8 b0, b1, b2, b3;
    float f0, f1, f2, f3, f4;
    u8 last;
    CibgWords16 tail;
};
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
