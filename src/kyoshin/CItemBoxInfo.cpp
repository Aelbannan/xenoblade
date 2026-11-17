#define IWORK_EVENT_INLINE_DTOR
// Auto-scaffolded catalog TU for kyoshin/CItemBoxInfo
// Replace stubs with high-level C/C++ during decomp.

// harness_catalog dropped: this TU uses none of its VM/script-helper
// headers, and it drags in the stale void*/void* CItem_initItemImplInstances
// declaration (via CTaskGameEff -> CfGameManager) that conflicts with the
// real one in kyoshin/cf/CItem.hpp (see below).
#include <cstring>
// Real item-impl owner BEFORE CItemBoxInfo.hpp: that header defines a
// CItem_initItemImplInstances function-like macro that would rewrite the
// declaration inside this header.
#include "kyoshin/cf/CItem.hpp"
#include "kyoshin/CItemBoxInfo.hpp"

// Unit-local imports (kept out of the class header so includers only see the classes).
extern "C" char lbl_eu_805063BC[];
extern "C" char lbl_eu_8050634C[];
extern "C" char lbl_eu_80506380[];extern "C" void LayoutSetTextBoxFmtValue(nw4r::lyt::Layout*, const char*, const char*, u32);
extern "C" int Counter_TestBit(void*, u32);extern "C" u32 IdTable_SumValues(void*, u32);
extern "C" void* CtrlObjectParam_GetCharEntry(u32);
extern "C" void setPaneVisible(void*, u32);
extern "C" u16 BdatGetItemId(u32);
extern "C" u32 BdatGetItemType(u32);
extern "C" u32 LookupItemTypeTable(void*);
extern "C" void SplitU32ToS16s(void*, u32);
extern "C" void setGXColorS10(void*, u32, u32, u32, u32);
extern "C" void __as__11_GXColorS10FRC11_GXColorS10(void* dst, const void* src);
extern "C" void Panic__Q24nw4r2dbFPCciPCce(const char*, int, const char*, ...);
extern "C" char lbl_eu_8052E590[];
extern "C" char lbl_eu_8052E568[];
extern "C" u8 code80135FDC_getByte_64077();
extern "C" void copyVEC3(void*, const void*);
// C-linkage position-set helper (retail-unmangled ScalePaneWorldPos; same
// signature as CSysWin.hpp / CMenuPlayAward.hpp).
// More flat retail (unmangled) helpers used by the item-box renderers.
extern "C" void PaneMatSetTevColorsByName(nw4r::lyt::Layout*, char*, void*, void*);
extern "C" u32 func_801392C0();
extern "C" int GetCollectedFlagByte(u8);
extern "C" f32 BlendFloatAvgScale(u32 a, u32 b, u32 c, f32 d);
// cf/CfGameManager.hpp (pulled in via harness_catalog) already declares the
// singleton no-arg form of this import at global scope; MWCC rejects a second,
// differently-prototyped extern "C" declaration as an illegal overload. Keep
// the item-pointer form (same unmangled symbol) in a namespace and shim the
// call sites through it.
namespace itemimplshim {
extern "C" void* CItem_initItemImplInstances(void*);
}
#define CItem_initItemImplInstances(item) itemimplshim::CItem_initItemImplInstances(item)
extern "C" void setItemBoxVtxColor(void*, u32, const void*);
// 9-byte flag-record builder: (out, unused, data) - param2 is never read in
// retail (callers pass the item-box pointer through it).
extern "C" u32 CtrlObjectParam_GetCurrentRowKey();
extern "C" int IsSkillItem(void*);
extern "C" u32 isItemBoxType9Flagged(void*);

// 0xA8-byte detail record built by buildItemBoxNameRec for renderItemBoxKind3; only the
extern "C" char* BuildItemNameStr(u32);
extern "C" void* CItemBlock_getPtr20E8();
extern "C" char* MakeTplNameSysFile(u32);
extern "C" nw4r::lyt::ArcResourceAccessor* CUICfManager_getArcResourceAccessor();

// Layout-build helpers for OnFileEvent (retail mangled C++ symbols; keep
// signature shapes verbatim so the mangled names line up).
extern "C" nw4r::lyt::ArcResourceAccessor* createArcResourceAccessor__10CLibLayoutFv();
extern "C" void* getFontInfo__11CDeviceFontFUlPQ34nw4r3lyt6Layout(u32, nw4r::lyt::Layout*);
extern "C" char* CUICfManager_getPackedFont9C(void);
extern "C" char* GetBdatNameByNumId(const char* msg);
void buildLayout(nw4r::lyt::Layout**, nw4r::lyt::ArcResourceAccessor*, const char*);
void bindLayoutAnimTransform(nw4r::lyt::Layout*, nw4r::lyt::AnimTransform**, nw4r::lyt::ArcResourceAccessor*, char*);
void setLayoutTextBoxFont(nw4r::lyt::Layout*, char*, u32);

extern "C" u32 MapSexFlagGroup(u16);
extern u32 lbl_eu_80664098;

extern "C" CItemBoxGXColor PaneMatGetS16Quad(void* obj, int index);
#include "kyoshin/code_80135FDC_Bdat.hpp"
#include "kyoshin/code_80135FDC_Lyt.hpp"
// Real item-impl accessor (CItem.hpp) for this TU only: undo the
// itemimplshim macro from CItemBoxInfo.hpp (other TUs keep using it).
#undef CItem_initItemImplInstances

// ===== typified split1 retail data (wave6) =====
// MWCC 8-packs .rodata/.data objects: only labels at 8-aligned boundaries
// get real symbols; interior names keep UNDEF extern reads (words stay zero,
// relocs gate-skipped), exactly like the pre-typify linkage.

// POD quad color (moved up from below: the .sbss defs need the complete type).
struct E43Quad {
    s16 r, g, b, a;
};

// .rodata [0..0x48]: six "percent_X" rank strings, one 8-aligned object.
__declspec(section ".rodata") __attribute__((aligned(8))) __attribute__((used)) char lbl_eu_805062E8[0x48] =
    "percent_E\000\000\000percent_D\000\000\000percent_C\000\000\000percent_B\000\000\000percent_A\000\000\000percent_S\000";

// .rodata [0x48..0x80]: category string tables (7-word pointer tables, one
// 8-aligned struct). Contents are UNDEF extern reads: the percent strings
// (except the base) have no 8-aligned address for real symbols.
extern const char lbl_eu_805062F4[];
extern const char lbl_eu_80506300[];
extern const char lbl_eu_8050630C[];
extern const char lbl_eu_80506318[];
extern const char lbl_eu_80506324[];
struct Rodata_ItemBoxTbls { const void* t1[7]; const void* t2[7]; };
__declspec(section ".rodata") __attribute__((aligned(8))) __attribute__((used)) struct Rodata_ItemBoxTbls lbl_eu_80506330 = {
    { "", lbl_eu_805062E8, lbl_eu_805062F4, lbl_eu_80506300, lbl_eu_8050630C, lbl_eu_80506318, lbl_eu_80506324 },
    { "", lbl_eu_80506324, lbl_eu_80506318, lbl_eu_8050630C, lbl_eu_80506300, lbl_eu_805062F4, lbl_eu_805062E8 }
};
#define lbl_eu_8050634C (lbl_eu_80506330.t2)

// .rodata [0x80..0x920]: single 8-aligned tail struct (int table + reversed
// string table + class names + format/path pool). Interior labels have no
// 8-aligned address: code reaches the pool through member macros.
struct Rodata_ItemBoxTail {
    u32 ints[6];
    const void* t[7];
    char blob[0x84C];
};
__declspec(section ".rodata") __attribute__((aligned(8))) __attribute__((used)) struct Rodata_ItemBoxTail rodata_ItemBoxTail = {
    { 2, 4, 5, 6, 7, 8 },
    { "", lbl_eu_80506324, lbl_eu_80506318, lbl_eu_8050630C, lbl_eu_80506300, lbl_eu_805062F4, lbl_eu_805062E8 },
   
    "%d\000rvs_type\000rvs_caption\000%s\000<col=red>%s<col=def>\000<col=red>%s %%<col=def>\000<col=red>%s%%<col=def>\000<col=red>%d %%<col=def>\000<col=red>%d%%<col=def>\000menu/jp/ItemBoxInfo.arc\000menu/jp/tpl/CrystalIcon.arc\000nul_para_01\000nul_para_30\000nul_para_50\000nul_para_60\000nul_para_80\000nul_para_700\000nul_eth\000nul_infbcln01\000nul_infbcln700\000MNU_item\000name\000%d%s\000txt_gold01_00\000txt_excange%02d\000pic_pcbs%02d\000nul_proportion\000txt_scnd01\000txt_scnd03\000nul_scnd\000dmg_low\000dmg_hi\000arm_phy\000arm_eth\000att_lev\000speed\000grd_rate\000flag\000equip_pc%d\000eva_rate\000arm_type\000pc%d\000rankType\000MNU_shop\000atr_type\000type\000MNU_collect\000mapID\000memory_type\000pc_type\000get_arts\000pc_arts\000idx\000%d%s%d\000txt_value02\000txt_value08\000txt_value09\000%s%d%s\000txt_value04\000txt_value07\000txt_value22\000\000txt_para25\000txt_para01\000txt_para10\000txt_para11\000txt_para04\000txt_para08\000txt_para22\000txt_para23\000pic_pc%02d\000pic_eq%02d\000mf00_reg00_eq01.tpl\000mf00_reg00_eq00.tpl\000mf00_com00_dmy.tpl\000txt_para24\000pic_ethcol%02d\000mf00_reg30_crys00.tpl\000mf00_reg30_crys01.tpl\000mf00_reg30_crys02.tpl\000mf00_reg30_crys03.tpl\000mf00_reg30_crys04.tpl\000mf00_reg30_crys05.tpl\000mf00_reg30_crys06.tpl\000txt_ethvalue%02d\000%s%s\000%s \000%d \000txt_eth%02d\000jwl_slot\000jwl_skill%d\000percent\000txt_para80\000txt_value50\000txt_value51\000txt_para51\000attach\000txt_para52\000txt_value60\000txt_value61\000txt_value30\000txt_para%02d\000txt_value%02d\000tag_icon\000txt_para736\000txt_value700\000txt_value702\000accum\000max\000txt_para7%02d\000txt_value7%02d\000CItemBoxInfo\000arc\000mf02_box02_inf.brlyt\000mf02_box02_inf_in.brlan\000mf02_box02_inf_info_in.brlan\000txt_value31\000txt_value32\000txt_value33\000txt_value34\000txt_ethvalue01\000txt_ethvalue02\000txt_ethvalue03\000txt_value703\000txt_value704\000txt_value705\000txt_value708\000txt_value710\000txt_value706\000txt_value709\000txt_value712\000txt_value714\000txt_value715\000txt_value716\000txt_value717\000txt_value718\000txt_value719\000txt_value720\000txt_value721\000txt_value722\000txt_value723\000txt_para704\000txt_scnd02\000nul_shop\000nul_excange\000txt_para30\000txt_para50\000txt_para60\000txt_para61\000txt_para700\000txt_para702\000txt_para703\000txt_para705\000txt_para706\000txt_para708\000txt_para709\000txt_para710\000txt_para713\000txt_para716\000txt_para717\000MNU_relate\000txt_npctype\000txt_npcname\000txt_npcframe\000txt_excange00\000txt_excange01\000pic_pcbs01\000txt_scnd00\000CItemBoxInfoTex\000itemID\000jwl_skill1\000CItemBoxInfo2\000CItemBoxInfo2Tex\000",
};
#define lbl_eu_80506380 (rodata_ItemBoxTail.t)
// Interior blob is at +0x54 (non-8-aligned): member decay emits parent+0x54+imm.
// Code must relocate against the retail symbol name with plain immediates.
extern "C" char lbl_eu_805063BC[];

// .sdata: two RTTI-ish pairs (class-name string + .data RTTI pad). Both
// halves stay UNDEF reads: the strings are struct members below and the
// pads live in the hand-typed .data at file end.
extern "C" char lbl_eu_8050639C[];
extern "C" char lbl_eu_805063AC[];


// .sbss 0xC0: palette globals in retail order (NOBITS: size+align gated).
// 4-byte entries are u32, 8-byte entries E43Quad; first carries aligned(8)
// for the section align, the rest pack without pads.
__attribute__((aligned(8))) __attribute__((used)) u32 lbl_eu_80664518;
__attribute__((used)) u32 lbl_eu_8066451C;
__attribute__((used)) u32 lbl_eu_80664520;
__attribute__((used)) u32 lbl_eu_80664524;
__attribute__((used)) E43Quad lbl_eu_80664528;
__attribute__((used)) E43Quad lbl_eu_80664530;
__attribute__((used)) E43Quad lbl_eu_80664538;
__attribute__((used)) E43Quad lbl_eu_80664540;
__attribute__((used)) E43Quad lbl_eu_80664548;
__attribute__((used)) E43Quad lbl_eu_80664550;
__attribute__((used)) u32 lbl_eu_80664558;
__attribute__((used)) u32 lbl_eu_8066455C;
__attribute__((used)) u32 lbl_eu_80664560;
__attribute__((used)) u32 lbl_eu_80664564;
__attribute__((used)) E43Quad lbl_eu_80664568;
__attribute__((used)) E43Quad lbl_eu_80664570;
__attribute__((used)) E43Quad lbl_eu_80664578;
__attribute__((used)) E43Quad lbl_eu_80664580;
__attribute__((used)) E43Quad lbl_eu_80664588;
__attribute__((used)) E43Quad lbl_eu_80664590;
__attribute__((used)) E43Quad lbl_eu_80664598;
__attribute__((used)) E43Quad lbl_eu_806645A0;
__attribute__((used)) u32 lbl_eu_806645A8;
__attribute__((used)) u32 lbl_eu_806645AC;
__attribute__((used)) u32 lbl_eu_806645B0;
__attribute__((used)) u32 lbl_eu_806645B4;
__attribute__((used)) E43Quad lbl_eu_806645B8;
__attribute__((used)) E43Quad lbl_eu_806645C0;
__attribute__((used)) u32 lbl_eu_806645C8;
__attribute__((used)) u32 lbl_eu_806645CC;
__attribute__((used)) u32 lbl_eu_806645D0;
__attribute__((used)) u32 lbl_eu_806645D4;




#include "decomp.h"





#include "monolib/util.hpp"
#include "monolib/device/CDeviceFile.hpp"
#include <nw4r/lyt/lyt_layout.h>
#include <nw4r/lyt/lyt_pane.h>
#include <nw4r/lyt/lyt_arcResourceAccessor.h>
#include "monolib/work/CEventFile.hpp"
#include "monolib/device/CFileHandle.hpp"
#include "monolib/device/CDeviceFont.hpp"  // IDeviceFontInfo::getFont (+0x24)
#include "kyoshin/cf/object/CActorParam.hpp"  // CActorParam slots (+0xA4/+0x108/...) for char-data stats
#include <new>

// Retail is 2-arg (category index + s16 item id); the real declaration now
// comes from kyoshin/cf/CItem.hpp (CItemExt* return).
// Legacy 1-arg form (category 0) used by not-yet-matched reconstructions.
static inline void* CItem_kindPtr_1(u32 id) { return CItem_kindPtr(0, (s16)id); }
// --- Forward declarations ---
namespace nw4r { namespace lyt { class Layout; class DrawInfo; class AnimTransform; } }
void setLayoutTextBoxNumber(nw4r::lyt::Layout*, char*, unsigned int);
void setLayoutTextBoxNumber(nw4r::lyt::Layout*, char*, u8);
u32 advanceAnimTransform(nw4r::lyt::AnimTransform*, float);
void drawLayout(nw4r::lyt::Layout*, nw4r::lyt::DrawInfo*, int, int);
void func_80127BD8(void*, float*);
u32 func_801392C0();

// Root-pane view of nw4r::lyt::Layout (mpRootPane at +0x10). Spelled through
// this view rather than GetRootPane(): the auto_inline-off regions would
// emit an out-of-line call for the accessor.
struct LayoutRootPaneView {
    u8 _00[0x10];
    nw4r::lyt::Pane* rootPane10;
};
u32 CtrlObjectParam_GetCurrentRowKey();
extern "C" u32 CtrlObjectParam_GetArtsDataWord(void*);
extern "C" f32 GetFloatTableEntry(u32);
extern "C" void* CtrlObjectParam_GetSlotTableBase();
extern "C" void* findObjB28ById(u32);
extern "C" void CtrlObjectParam_SyncParamFromActorEx(void*, u32);
extern "C" void CtrlObjectParam_GetArtsStatsRow(void*, u32);
extern "C" s32 RoundHalfAway0(float);
extern "C" void PaneMatSetTevColors(void*, void*, void*);
char* BuildItemNameStr(u32);
extern void* lbl_eu_80664104;
extern void* lbl_eu_806640A8;
extern void* lbl_eu_806640F4;
extern void* lbl_eu_806640EC;
extern void* CItemBlock_getPtr20E8();
extern void* lbl_eu_806640F8;
extern void* lbl_eu_806640D8;
// 6-byte item-box slot tables (pair of u32+u16 .sdata2 constants).
// getLanguage__9CDeviceSCFv: real int-returning declaration comes from
// kyoshin/cf/CItem.hpp (stale local u8 prototype deleted).
extern void* lbl_eu_80664110;
extern void* lbl_eu_80664090;


// --- CItemBoxInfo methods ---

u32 CItemBoxInfo::GetState() {
    return mCurrent;
}

u32 CItemBoxInfo2::GetState() {
    return mCurrent;
}

void CItemBoxInfo::Close() {
    if (mState == 3) {
        mState = 4;
        mVisible = 0;
    }
}

#pragma push
__declspec(noinline) void CItemBoxInfo::Activate() {
    // Goto-gate (PLAN 17.6), same shape as activateItemBox2.
    if (mpLayout != 0 && mTexArcResAcc != 0) {
        goto body;
    }
    return;
body:
    mCurrent = 1;
    mActive = 1;
}
#pragma pop

#pragma push
__declspec(noinline) u32 isItemBoxType9Flagged(void* arg) {
    u32 v = ((CItemData*)arg)->field_00;
    u32 result = 0;
    if (((v >> 16) & 0xF) == 9) {
        u32 flags = ((CItemData*)arg)->field_07;
        if ((flags & 3) == 3u) {
            result = 1;
        }
    }
    return result;
}
#pragma pop

#pragma push
#pragma optimize_for_size on
void CItemBoxInfo::SetNamedText(int index, char* text) {
    char buf[0x20];
    sprintf(buf, &lbl_eu_805063BC[0x151], index);
    LayoutSetTextBoxFmtValue((nw4r::lyt::Layout*)mpLayout, buf, text, 0);
}
#pragma pop

#pragma push
#pragma optimize_for_size on
void CItemBoxInfo::SetCounts(u8 arg2, u8 arg3) {
    if (mpLayout != 0) {
        setLayoutTextBoxNumber((nw4r::lyt::Layout*)mpLayout, &lbl_eu_805063BC[0x17d], arg2);
        setLayoutTextBoxNumber((nw4r::lyt::Layout*)mpLayout, &lbl_eu_805063BC[0x188], arg3);
    }
}
#pragma pop

// --- Remaining CItemBoxInfo stubs ---


void CItemBoxInfo::Open() {
    if (mState != 0) return;
    mState = 1;
    mVisible = 0;
    // Retail tail-calls stampItemBoxSlotTex (b, not bl) - dont_inline pragma keeps it a call.
    StampSlotTex();
}

void CItemBoxInfo::DrawLayout(nw4r::lyt::DrawInfo* drawInfo) {
    if (mActive != 0) {
        drawLayout((nw4r::lyt::Layout*)mpLayout, drawInfo, 0, 1);
    }
}

void func_80127BD8(void*, float*);

// TU-local cast-only views (CItemBoxInfo.hpp is shared, so these live here):
// - CItemBoxVecTable: the info+0xB0 table (the header keeps the raw _B0 u8
//   view): 12 s16 ids followed by twelve 3-float VEC3 slots at +0x18.
// - CItemBoxCharEntryView: the CtrlObjectParam_GetCharEntry record; the
//   equip-slot ids are the s16[6] at +0x1C (cf. CtrlObjectParamEquipRow::
//   shortArr in cf/CtrlObjectParam.hpp; shortArr[5] sits at +0x26), and the
//   counter/arts area at +0x3534 feeds Counter_TestBit/IdTable_SumValues.
struct CItemBoxVecTable {
    s16 ids[12];       // +0x00 twelve slot ids
    float vecs[12][3]; // +0x18 twelve VEC3 slots
};
struct CItemBoxCharEntryView {
    u8 _00[0x1C];
    s16 slotIds[6];        // +0x1C..+0x27 equipped-item ids by slot
    u8 _28[0x3534 - 0x28];
    u8 counterArea[0x8A0]; // +0x3534 counter/arts storage
};

#pragma push
#pragma optimize_for_size on
__declspec(noinline) void CItemBoxPage::Clear() {
    float f = 0.0f;
    for (u8 i = 0; i < 12; i++) {
        ((CItemBoxVecTable*)this)->ids[i] = 0;
        float temp[3] = {f, f, f};
        copyVEC3(((CItemBoxVecTable*)this)->vecs[i], temp);
    }
}
#pragma pop


// Retail loadItemBoxFiles uses the stmw/lmw frame (MWCC optimize_for_size
// prologue merge). The setHandleFlag1 call reuses the readFile result in r3.
#pragma push
#pragma optimize_for_size on
void CItemBoxInfo::LoadFiles() {
    mtl::ALLOC_HANDLE handle = mtl::MemManager::getHandleMEM2();
    CFileHandle* fh1 = CDeviceFile::readFile(handle, &lbl_eu_805063BC[0x8e], reinterpret_cast<IWorkEvent*>(this), 0, 0);
    mFileHandle1 = fh1;
    CDeviceFile::setHandleFlag1(fh1);
    handle = mtl::MemManager::getHandleMEM2();
    CFileHandle* fh2 = CDeviceFile::readFile(handle, &lbl_eu_805063BC[0xa6], reinterpret_cast<IWorkEvent*>(this), 0, 0);
    mFileHandle2 = fh2;
    CDeviceFile::setHandleFlag1(fh2);
}
#pragma pop
// Retail closeItemBox1 uses the stmw/lmw frame (MWCC optimize_for_size
// prologue merge): r30 holds `this` and r31 the zero constant for the four
// post-call pointer clears, so both survive the closeFileHandle/80139124 calls.
#pragma push
#pragma optimize_for_size on
void CItemBoxInfo::UnloadFiles() {
    closeFileHandle(&mFileHandle1);
    closeFileHandle(&mFileHandle2);
    void* layout = mpLayout;
    mActive = 0;
    if (layout != 0) {
        // Retail calls the Layout deleting destructor (vtable +0x08,
        // delete flag in r4); plain `delete` reproduces the r12/CTR
        // dispatch (CCur.cpp cleanup__8CBaseCurFv precedent). The
        // single manual check plus delete's own null check give the
        // retail two-branch shape.
        delete (nw4r::lyt::Layout*)layout;
        mpLayout = 0;
    }
    mAnimIn = 0;
    mAnimInfoIn = 0;
    releaseArcResourceAccessor(mArcResAcc);
    releaseArcResourceAccessor(mTexArcResAcc);
    mArcResAcc = 0;
    mTexArcResAcc = 0;
    mRegion1.deleteRegion();
    mRegion2.deleteRegion();
}
#pragma pop



// Retail calcItemBoxPaneVec uses the stmw/lmw frame (MWCC optimize_for_size
// prologue merge) for arg0/child/first-result in r29/r30/r31.
#pragma push
#pragma optimize_for_size on
nw4r::math::VEC3 CItemBoxInfo::GetSlotPos(u32 arg2) {
    char buf[0x20];
    // Declare `first` before `child`: low-degree webs color in reverse birth
    // order, so child (declared later) colors first into this's dead r30 and
    // first reuses the pool base's r31 (matches retail r29/r30/r31 layout).
    nw4r::lyt::Pane* first;
    nw4r::lyt::Pane* second;
    nw4r::lyt::Pane* child;
    char* base = lbl_eu_805063BC;
    nw4r::math::VEC3 pos;
    sprintf(buf, base + 0x161, arg2 + 1);
    child = ((LayoutRootPaneView*)mpLayout)->rootPane10;
    first = child->FindPaneByName(base + 0x16e, true);
    second = child->FindPaneByName(buf, true);
    ScalePaneWorldPos(&pos, second, first, child);
    return pos;
}
#pragma pop

void CItemBoxInfo::SetResultPane(u32 visible) {
    if (mpLayout == 0) return;
    nw4r::lyt::Pane* child = ((LayoutRootPaneView*)mpLayout)->rootPane10;
    nw4r::lyt::Pane* result = child->FindPaneByName((char*)&lbl_eu_805063BC[0x193], true);
    setPaneVisible(result, visible);
}

#pragma push
#pragma auto_inline off
void CItemBoxInfo::OpenPhase1() {
    float arg = 1.0f;
    if (advanceAnimTransform((nw4r::lyt::AnimTransform*)mAnimIn, arg) != 0) {
        ((nw4r::lyt::Layout*)mpLayout)->SetAnimationEnable((nw4r::lyt::AnimTransform*)mAnimIn, false);
        ((nw4r::lyt::Layout*)mpLayout)->SetAnimationEnable((nw4r::lyt::AnimTransform*)mAnimInfoIn, true);
        mState = 2;
    }
}

void CItemBoxInfo::OpenPhase2() {
    if (advanceAnimTransform((nw4r::lyt::AnimTransform*)mAnimInfoIn, 1.0f) != 0) {
        mState = 3;
        mVisible = 1;
    }
}

void CItemBoxInfo::ClosePhase1() {
    if (AnimRewindFrame((nw4r::lyt::AnimTransform*)mAnimInfoIn, 1.0f) != 0) {
        ((nw4r::lyt::Layout*)mpLayout)->SetAnimationEnable((nw4r::lyt::AnimTransform*)mAnimInfoIn, false);
        ((nw4r::lyt::Layout*)mpLayout)->SetAnimationEnable((nw4r::lyt::AnimTransform*)mAnimIn, true);
        mState = 5;
    }
}

void CItemBoxInfo::ClosePhase2() {
    if (AnimRewindFrame((nw4r::lyt::AnimTransform*)mAnimIn, 1.0f) != 0) {
        mVisible = 1;
        mState = 0;
    }
}
#pragma pop

// Retail buildItemBoxCmpRecord uses the _savegpr_20 + f29/f30/f31 frame (MWCC
// optimize_for_size prologue merge) and keeps the mtctr copy loop for the
// 0x34-byte record copy (li r0,6 + 4-byte tail). Per-slot banks: +0x16 holds
// a name-resolved flag (forced for slot 0xB when cat == 0x4C), +0x1D/+0x24/
// +0x2B hold {1,2,0} comparisons of the looked-up item's table values against
// the candidate item's.
#pragma push
#pragma optimize_for_size on
__declspec(noinline) CItemBoxCompRecord CItemBoxInfo::BuildCmpRecord(void* arg3) {
    // Scratch doubles for MWCC's int->float magic (2^52 unsigned repair,
    // 2^63 sign-repair); retail materializes both high words up front.
    CItemBoxCompRecord rec;
    void* global = lbl_eu_806640F4;
    BdatGetItemType((u32)arg3);
    // v2 is a narrow-typed local: MWCC keeps it zero-extended and re-applies
    // the u16->u32 promotion per call site (retail clrlwi r5,r29,16 each
    // time, no shared masked temp).
    u16 v2 = BdatGetItemId((u32)arg3);
    char* base = (char*)&lbl_eu_805063BC;
    rec.s2C =(u16)BdatGetU16Direct((const void*)global, (const char*)((const void*)(base + 0x19c)), v2);
    rec.s2E = (u16)BdatGetU16Direct((const void*)global, (const char*)((const void*)(base + 0x1a4)), v2);
    rec.s30 = (u8)BdatGetU8Direct((const void*)global, base + 0x1ab, v2);
    rec.s32 = (u8)BdatGetU8Direct((const void*)global, base + 0x1b3, v2);
    rec.s34 = (u8)BdatGetU8Direct((const void*)global, base + 0x1bb, v2);
    rec.f38 = (f32)((u8)BdatGetU8Direct((const void*)global, base + 0x1c3, v2) / 10.0f);
    rec.s3C = (u8)BdatGetU8Direct((const void*)global, base + 0x1c9, v2);
    if (BdatGetU8Direct((const void*)global, base + 0x1d2, v2) & 4) {
        void* lookup = CtrlObjectParam_GetCharEntry(1);
        u32 r = CtrlObjectParam_GetArtsDataWord(lookup);
        rec.s2C = (s16)(1.4f * (f32)(rec.s2C * (u16)r));
        u32 r2 = CtrlObjectParam_GetArtsDataWord(lookup);
        rec.s2E = (s16)(1.5f * (f32)(rec.s2E * (u16)r2));
        if (rec.s2C >= 999) rec.s2C = 999;
        if (rec.s2E >= 999) rec.s2E = 999;
    }
    for (u32 i = 0; (u8)i < (u8)code80135FDC_getByte_64077(); i++) {
        // slot stays raw and re-masks per use, matching retail r21.
        u32 slot = GetCollectedFlagByte(i);
        char buf[0x20];
        sprintf(buf, base + 0x1d7, (u8)slot);
        u16 flag = BdatGetU8Direct((const void*)global, (const char*)buf, v2);
        // name-flag bank (+0x16): set when the slot tag resolves; slot 0xB
        // is forced on for candidate category 0x4C.
        rec.flags[0][i] = (flag != 0);
        if ((u8)slot == 0xb && v2 == 0x4c) rec.flags[0][i] = 1;
        if (rec.flags[0][i] == 0) continue;
        rec.f38 += GetFloatTableEntry((u8)slot);
        void* lookup = CtrlObjectParam_GetCharEntry((u8)slot);
        void* item = CItem_kindPtr(2, *(s16*)((u8*)lookup + 0x26));
        if (item == NULL || *(u32*)item == 0) continue;
        // cat is likewise narrow-typed for per-site promotion.
        u16 cat = BdatGetItemId(*(u32*)item >> 20);
        u16 a = (u16)BdatGetU16Direct((const void*)global, (const char*)((const void*)(base + 0x19c)), cat);
        u16 b = (u16)BdatGetU16Direct((const void*)global, (const char*)((const void*)(base + 0x1a4)), cat);
        f32 ratio = (f32)((u8)BdatGetU8Direct((const void*)global, base + 0x1c3, cat) / 10.0f);
        rec.f28 = BlendFloatAvgScale((u32)a, (u32)b, 0, ratio);
        u8 v1ab = (u8)BdatGetU8Direct((const void*)global, base + 0x1ab, cat);
        u8 v1b3 = (u8)BdatGetU8Direct((const void*)global, base + 0x1b3, cat);
        if (rec.f28 > ratio) rec.flags[1][i] = 1;
        else if (rec.f28 < ratio) rec.flags[1][i] = 2;
        else rec.flags[1][i] = 0;
        if (rec.s30 > v1ab) rec.flags[2][i] = 1;
        else if (rec.s30 < v1ab) rec.flags[2][i] = 2;
        else rec.flags[2][i] = 0;
        if (rec.s32 > v1b3) rec.flags[3][i] = 1;
        else if (rec.s32 < v1b3) rec.flags[3][i] = 2;
        else rec.flags[3][i] = 0;
    }
    // 0x34-byte copy: pair-copy with s[1]/s[2] accesses reproduces the retail
    // mtctr lwzu/stwu 8-byte-pair loop (li r0,6 + 4-byte tail).
    return rec;
}
#pragma pop
#pragma push
#pragma optimize_for_size on
// Retail keeps the stmw/lmw frame (optimize_for_size prologue merge) and the
// mtctr pair-copy loop for the 0x1C-byte record (li r0,3 + 4-byte tail). The
// null path memsets the record, copies, and returns early; the filled path
// falls through to its own copy of the same loop. Per-slot flag banks are
// addressed through one walked pointer (&flags[i], +0/+7/+14).
__declspec(noinline) CItemBoxSlotFlags CItemBoxInfo::BuildSlotFlags(u32 item) {
    CItemBoxSlotFlags rec;
    if (item == 0) {
        memset(&rec, 0, 0x1C);
        return rec;
    }
    void* global = lbl_eu_806640F8;
    u32 type = BdatGetItemType(item);
    u16 id = BdatGetItemId(item);
    char* base = (char*)&lbl_eu_805063BC;
    rec.v[0] = (u8)BdatGetU8Direct((const void*)global, base + 0x1ab, id);
    rec.v[1] = (u8)BdatGetU8Direct((const void*)global, base + 0x1b3, id);
    rec.v[2] = (u8)BdatGetU8Direct((const void*)global, base + 0x1e2, id);
    u32 reqKind = BdatGetU8Direct((const void*)lbl_eu_806640F8, base + 0x1eb, id);
    for (u32 i = 0; (u8)i < (u8)code80135FDC_getByte_64077(); i++) {
        u32 slot = GetCollectedFlagByte((u8)i);
        char buf[0x20];
        sprintf(buf, base + 0x1f4, (u8)slot);
        rec.equipable[(u8)i] = (u8)BdatGetU8Direct((const void*)lbl_eu_806640F8, (const char*)buf, id) != 0;
        if (rec.equipable[(u8)i] == 0) continue;
        CItemBoxCharEntryView* member = (CItemBoxCharEntryView*)CtrlObjectParam_GetCharEntry((u8)slot);
        void* counters = member->counterArea;
        if ((u8)reqKind == 3) {
            if (Counter_TestBit(counters, 0x85) == 0) rec.equipable[(u8)i] = 0;
        } else if ((u8)reqKind == 2) {
            if (Counter_TestBit(counters, 0x84) == 0) rec.equipable[(u8)i] = 0;
        } else {
            s16 value = -1;
            switch ((u16)type) {
                case 4: value = member->slotIds[0]; break;
                case 5: value = member->slotIds[1]; break;
                case 6: value = member->slotIds[2]; break;
                case 7: value = member->slotIds[3]; break;
                case 8: value = member->slotIds[4]; break;
            }
            if (value >= 0) {
                void* equipped = CItem_kindPtr((u16)type, value);
                if (equipped != NULL && *(u32*)equipped != 0) {
                    u32 cat = BdatGetItemId(*(u32*)equipped >> 20);
                    u8 n1 = (u8)BdatGetU8Direct((const void*)global, base + 0x1ab, cat);
                    u8 n2 = (u8)BdatGetU8Direct((const void*)global, base + 0x1b3, cat);
                    if (rec.v[0] > n1) rec.cmp0[(u8)i] = 1;
                    else if (rec.v[0] < n1) rec.cmp0[(u8)i] = 2;
                    else rec.cmp0[(u8)i] = 0;
                    if (rec.v[1] > n2) rec.cmp1[(u8)i] = 1;
                    else if (rec.v[1] < n2) rec.cmp1[(u8)i] = 2;
                    else rec.cmp1[(u8)i] = 0;
                }
            }
        }
    }
    return rec;
}
#pragma pop
// Retail buildItemBoxNameRec uses the stmw/lmw frame (MWCC optimize_for_size
// prologue merge) and keeps the mtctr copy loop for the 0xA4-byte record
// copy (li r0,0x14 + 4-byte tail). The name/category tables are resolved
// through the item vtable (arg3) or the raw data pointers (arg3 == NULL);
// the text buffer is then scanned for '$' markers ('$1' switches on the
// equip-state byte, '$2' copies the static label-pointer table at
// lbl_eu_8050634C and formats the count-indexed label); other bytes advance
// Shift-JIS-aware.
#pragma push
#pragma optimize_for_size on
__declspec(noinline) CItemBoxNameRecord CItemBoxInfo::BuildNameRec(void* data, void* arg3) {
    u32 cat;
    u32 count;
    void* item = arg3 != NULL ? arg3 : NULL;
    // Retail hoists this global into a register (r26) across all calls.
    void* gd8 = lbl_eu_806640D8;
    if (arg3 == NULL) {
        BdatGetItemType((u32)data);
    }
    if (arg3 != NULL) {
        CItemImpl* inst = CItem_initItemImplInstances((CItemData*)item);
        cat = inst->vf54((CItemData*)item);
    } else {
        // Retail keeps the raw result live in r28 (no early narrowing).
        cat = ((u32 (*)(u32))BdatGetItemId)((u32)data);
    }
    if (arg3 != NULL) {
        CItemImpl* inst = CItem_initItemImplInstances((CItemData*)item);
        count = (u16)inst->vf08((CItemData*)item);
    } else {
        count = BdatGetU8Direct((const void*)lbl_eu_806640EC, &lbl_eu_805063BC[0x1f9], (u32)data);
    }
    char* base = (char*)&lbl_eu_805063BC;
    CItemBoxNameRecord rec;
    rec.count = (u8)count;
    rec.str = (u32)BdatTouchStringCell(base + 0x130, base + 0x139, 0x1e - ((u8)count - 1));
    char* s2 = BdatTouchStringCell(base + 0x202, base + 0x139, 0xf);
    if (arg3 != NULL) {
        CItemImpl* inst = CItem_initItemImplInstances((CItemData*)item);
        sprintf(rec.name, base, inst->vf90((CItemData*)item));
    } else {
        sprintf(rec.name, base + 0x18, s2);
    }
    rec.e0 = (u8)BdatGetU8Direct((const void*)gd8, base + 0x3, (u16)cat);
    rec.e1 = (u8)BdatGetU8Direct((const void*)gd8, base + 0x20b, (u16)cat);
    rec.color = 0xFFFFFFFF;
    switch (rec.e1) {
        case 4: rec.color = 0xFF00FFFF; break;
        case 5: rec.color = 0x0000FFFF; break;
        case 6: rec.color = 0xFFFFFFFF; break;
        case 7: rec.color = 0x0000FFFF; break;
        case 8: rec.color = 0x00FFFFFF; break;
        case 9: rec.color = 0x775544FF; break;
    }
    sprintf(rec.text, base + 0x18,
            ((char* (*)(void*, const char*, u16))&BdatGetPtrDirect)(gd8, base + 0xc, (u16)cat));
    u32 wide = 1;
    if (getLanguage__9CDeviceSCFv() != 3) {
        if (getLanguage__9CDeviceSCFv() != 2) {
            wide = 0;
        }
    }
    CItemBoxLabelTable tbl;
    char buf28[0x20];
    char buf48[0x20];
    char* cur = rec.text;
    while (*cur != 0) {
        if (*cur == '$') {
            memset(buf28, 0, 0x20);
            cur++;
            if (*cur == '1') {
                cur++;
                switch (rec.e0) {
                    case 1:
                        sprintf(buf28, base + 0x1b, rec.name);
                        break;
                    case 2:
                        sprintf(buf28, base + 0x1b, rec.name);
                        break;
                    case 3:
                        if (wide != 0) {
                            sprintf(buf28, base + 0x30, rec.name);
                        } else {
                            sprintf(buf28, base + 0x48, rec.name);
                        }
                        break;
                    case 4:
                        if (wide != 0) {
                            sprintf(buf28, base + 0x30, rec.name);
                        } else {
                            sprintf(buf28, base + 0x48, rec.name);
                        }
                        break;
                    default:
                        break;
                }
            } else if (*cur == '2') {
                // 0x1C-byte move of the static label-pointer table into the
                // stack copy (retail mtctr 3-pair loop + 4-byte tail).
                {
                    u32* s = (u32*)&lbl_eu_8050634C - 1;
                    u32* d = (u32*)&tbl - 1;
                    for (u32 k = 0; k < 3; k++) {
                        d[1] = s[1];
                        d[2] = s[2];
                        s += 2;
                        d += 2;
                    }
                    d[1] = s[1];
                }
                cur++;
                u8 name = BdatGetU8Direct((const void*)gd8, tbl.labels[(u8)count], (u16)cat);
                if (wide != 0) {
                    sprintf(buf28, base + 0x5f, (u8)name);
                } else {
                    sprintf(buf28, base + 0x77, (u8)name);
                }
            }
            // strlen of buf28: retail's counter runs one past the terminator
            // (len+1), folded back out by the len-1 splice offset below.
            u32 len = 1;
            while (buf28[len - 1] != 0) {
                len++;
            }
            sprintf(buf48, base + 0x18, cur);
            cur -= 2;
            sprintf(cur, base + 0x18, buf28);
            cur = cur + len - 1;
            sprintf(cur, base + 0x18, buf48);
        } else {
            s8 sc = (s8)*cur;
            if ((sc >= 0x81 && sc <= 0x9f) || (sc >= 0xe0 && sc <= 0xef)) {
                cur += 2;
            } else {
                cur += 1;
            }
        }
    }
    // 0xA4-byte copy: pair-copy with s[1]/s[2] accesses reproduces the retail
    // mtctr lwzu/stwu 8-byte-pair loop (li r0,0x14 + 4-byte tail).
    return rec;
}
#pragma pop

#pragma push
#pragma optimize_for_size on
// Retail keeps the lookup-table pointer live in r29 across all calls and
// spills the four results to stack slots, reloading them for the final four
// output-word stores. dont_inline keeps IPA from folding this body into
// renderItemBoxLabels/renderItemBox2Labels, which lets them keep both stack arrays.
__declspec(noinline) CItemBoxLabelRec CItemBoxInfo::GetLabelRec(u32 item) {
    CItemBoxLabelRec r;
    void* tbl = lbl_eu_80664104;
    BdatGetItemType(item);
    // cat stays a raw u16 in a register; each call site re-applies the
    // u16->u32 argument conversion (retail clrlwi per site, no shared temp).
    u16 cat = BdatGetItemId(item);
    // base is anchored at the first lookup so its definition schedules after
    // the cat move (retail order: or r30,r3 then lis/addi).
    r.a = (u8)BdatGetU8Direct((const void*)tbl, (char*)&lbl_eu_805063BC + 0x214, cat);
    char* base = (char*)&lbl_eu_805063BC;
    r.b = BdatTouchStringCell(base + 0x219, base + 0x139, r.a);
    // The label ID is also the row key passed to BdatGetPtrDirect.
    r.c = (u8)BdatGetU8Direct((const void*)tbl, base + 0x225, cat);
    if (r.c == 0x1A) {
        r.d = BdatTouchStringCell(base + 0x219, base + 0x139, 0x14);
    } else {
        r.d = BdatGetPtrDirect(lbl_eu_806640A8, base + 0x139, r.c);
    }
    // Returned by value: the caller's hidden result pointer receives the
    // record as four lwz/stw pairs.
    return r;
}
#pragma pop

#pragma push
#pragma optimize_for_size on
__declspec(noinline) CItemBoxSelRec CItemBoxInfo::BuildSelRec(u32 item) {
    void* global = lbl_eu_80664110;
    u16 v1 = BdatGetItemType(item);
    u16 v2 = BdatGetItemId(item);
    CItemBoxSelRec rec;
    rec.category = (u8)BdatGetU8Direct((const void*)global, (char*)&lbl_eu_805063BC[0x22b], v2);
    // count stays u32: retail keeps the raw call result (r28) and masks at
    // each (u8)count use (loop mask hoisted to r29).
    u8 count = BdatGetU8Direct((const void*)global, (char*)&lbl_eu_805063BC[0x237], v2);
    for (u32 i = 0; (u8)i < (u8)code80135FDC_getByte_64077(); i++) {
        u8 a = (u8)GetCollectedFlagByte((u8)i);
        rec.selected[(u8)i] = a == (u8)count;
    }
    u8 key = BdatGetU8Direct((const void*)global, (char*)&lbl_eu_805063BC[0x23f], v2);
    u8 val = BdatGetU8ByTableKey((char*)&lbl_eu_805063BC[0x248], (char*)&lbl_eu_805063BC[0x250], (u8)key);
    void* lookup = CtrlObjectParam_GetCharEntry((u8)count);
    u8 cat = (u8)CtrlObjectParam_GetCurrentRowKey();
    rec.flag2 = 0;
    u8* entry = (u8*)lookup + cat * 0x49 + val * 2;
    switch (rec.category) {
        case 1: if (entry[0xE8] != 0) rec.flag2 = 1; break;
        case 2: if ((entry[0xE9] >> 7) & 1) rec.flag2 = 1; break;
        case 3: if ((entry[0xE9] >> 6) & 1) rec.flag2 = 1; break;
    }
    return rec;
}
#pragma pop
#pragma push
#pragma optimize_for_size on
__declspec(noinline) CItemBoxSlotRecord1 CItemBoxInfo::BuildSlotRec(u32 item, void* arg3) {
    void* p;
    if (arg3 != NULL)
        p = arg3;
    else
        p = NULL;
    BdatGetItemType(item);
    BdatGetItemId(item);
    CItemBoxSlotRecord1 rec;
    // Chained call + virtual dispatch: no named intermediate keeps the result
    // web dead after each use, matching retail's direct r3 consumption.
    u32 v08 = CItem_initItemImplInstances((CItemData*)p)->vf08((CItemData*)p);
    rec.count = (u8)v08;
    rec.str = (u32)BdatTouchStringCell(&lbl_eu_805063BC[0x130], &lbl_eu_805063BC[0x139], 0x1e - ((u8)v08 - 1));
    rec.counter = 0;
    for (u32 i = 0; i < 4; i++) {
        u32 n = (u16)CItem_initItemImplInstances((CItemData*)p)->vf4C((CItemData*)p, (u8)i);
        // Signed >0 test: folds into the record-form clrlwi as retail's ble.
        if ((s32)n > 0) {
            // Retail passes the pair count n as a third arg to BdatGetPtrDirect.
            rec.text[rec.counter] =
                (u32)((char*(*)(void*, char*, u32))&BdatGetPtrDirect)(lbl_eu_806640D8, &lbl_eu_805063BC[0x139], n);
            rec.vals[rec.counter] = CItem_initItemImplInstances((CItemData*)p)->vf64((CItemData*)p, (u8)i);
            rec.counts[rec.counter] = n;
            rec.counter++;
        }
    }
    // Returned by value: the 0x2C-byte copy to the hidden result pointer is
    // the retail mtctr 8-byte-pair loop (li r0,5 + 4-byte tail).
    return rec;
}
#pragma pop
// Retail renderItemBoxKind2: item-box renderer for selection kind 2. Builds the
// 0x34 comparison record (buildItemBoxCmpRecord), formats three stat panes from it
// through an ml::FixStr<32>, resets the highlight panes, then per visible
// slot colours the row pane and stamps the candidate-item texture.
#pragma push
#pragma optimize_for_size on
__declspec(noinline) void CItemBoxInfo::RenderKind2(u16 arg2, void* arg3, u32 arg4) {
    SetupSlotPanes();
    // Declaration order follows the retail frame layout (high -> low):
    // staged colour temps > copied record > FixStr storage > source record >
    // pane-name buffers.
    CItemBoxColorBlock selB;
    CItemBoxColorBlock selA;
    CItemBoxCompRecord local = BuildCmpRecord((void*)(u32)arg2);
    // Layout-compatible POD stand-in for ml::FixStr<32>: the format() calls
    // cast its address inline so no FixStr pointer web stays live across the
    // lookup calls (retail touches the buffer purely through sp offsets).
    struct {
        char mString[0x20];
        u32 mLength;
    } text;
    char paneName[0x20];
    char label[0x20];
    char* base = (char*)&lbl_eu_805063BC;
    // Raw-storage binding: retail inlines the bool-ctor clear as bare stb/stw
    // stores; format() is invoked through a cast so no reference web occupies
    // a register.
    text.mString[0] = 0;
    text.mLength = 0;
    // NOTE (open item): spelling the label-pool base inline at every use
    // re-anchors it MORE than retail (which uses r19/r19/r28) and regressed
    // the diff; one cached base pointer is the closest shape tried so far.
    char* lbl = BdatTouchStringCell(&base[0x130], &base[0x139], 0xb);
    ((ml::FixStr<32>*)&text)->format(&base[0x254], local.s2C, lbl, local.s2E);
    LayoutSetTextBoxFmtValue((nw4r::lyt::Layout*)mpLayout, &base[0x25b], text.mString, 0);
    setLayoutTextBoxNumber((nw4r::lyt::Layout*)mpLayout, &base[0x267], (u8)local.s30);
    setLayoutTextBoxNumber((nw4r::lyt::Layout*)mpLayout, &base[0x273], (u8)local.s32);
    char* str7f = BdatTouchStringCell(&base[0x130], &base[0x139], 0x7f);
    char* str80 = BdatTouchStringCell(&base[0x130], &base[0x139], 0x80);
    if (local.s34 != 0) {
        ((ml::FixStr<32>*)&text)->format(&base[0x27f], str7f, str80, local.s34);
    } else {
        ((ml::FixStr<32>*)&text)->format(&base[0x13e], local.s34, str80);
    }
    LayoutSetTextBoxFmtValue((nw4r::lyt::Layout*)mpLayout, &base[0x286], text.mString, 0);
    str7f = BdatTouchStringCell(&base[0x130], &base[0x139], 0x7f);
    str80 = BdatTouchStringCell(&base[0x130], &base[0x139], 0x80);
    if (local.s3C != 0) {
        ((ml::FixStr<32>*)&text)->format(&base[0x27f], str7f, str80, local.s3C);
    } else {
        ((ml::FixStr<32>*)&text)->format(&base[0x13e], local.s3C, str80);
    }
    LayoutSetTextBoxFmtValue((nw4r::lyt::Layout*)mpLayout, &base[0x292], text.mString, 0);
    LayoutSetTextBoxFmtValue((nw4r::lyt::Layout*)mpLayout, &base[0x29e], &base[0x2aa], 0);
    LayoutSetTextBoxFmtValue((nw4r::lyt::Layout*)mpLayout, &base[0x2ab], &base[0x2aa], 0);
    PaneMatSetTevColorsByName((nw4r::lyt::Layout*)mpLayout, &base[0x2b6], &lbl_eu_80664598, &lbl_eu_806645A0);
    PaneMatSetTevColorsByName((nw4r::lyt::Layout*)mpLayout, &base[0x2c1], &lbl_eu_80664598, &lbl_eu_806645A0);
    PaneMatSetTevColorsByName((nw4r::lyt::Layout*)mpLayout, &base[0x2cc], &lbl_eu_80664598, &lbl_eu_806645A0);
    PaneMatSetTevColorsByName((nw4r::lyt::Layout*)mpLayout, &base[0x2d7], &lbl_eu_80664598, &lbl_eu_806645A0);
    PaneMatSetTevColorsByName((nw4r::lyt::Layout*)mpLayout, &base[0x2e2], &lbl_eu_80664598, &lbl_eu_806645A0);
    PaneMatSetTevColorsByName((nw4r::lyt::Layout*)mpLayout, &base[0x2ed], &lbl_eu_806645A8, &lbl_eu_806645B0);
    PaneMatSetTevColorsByName((nw4r::lyt::Layout*)mpLayout, &base[0x2f8], &lbl_eu_806645A8, &lbl_eu_806645B0);
    u32 playerIdx = (arg4 >> 16) & 0xFF;
    // Hoisted halves: MWCC materializes these once (lis) and rebuilds each
    // constant at its use site with a single addi.
    u32 darkBase = 0x77770000;
    u32 tagHi = 0x74690000;
    for (u32 i = 0; (u8)i < (u8)code80135FDC_getByte_64077(); i++) {
        u8 flag = local.flags[0][(u8)i];
        u32 color = darkBase + 0x77ff;  // 0x777777ff
        if (flag != 0) color = 0xFFFFFFFF;
        sprintf(paneName, base + 0x303, (u8)i + 1);
        PaneSetVtxColorByName((nw4r::lyt::Layout*)mpLayout, paneName, color);
        // Default vertex colours carry data only in the alpha channel
        // (this+0xA2 / this+0xAA); the selected-slot pair comes from
        // this+0x9C/0xA4 when the slot is active.
        s16 vA2 = mColorA.s[3];
        s16 vAA = mColorB.s[3];
        // Loop-scope slots descend: args (sp+0x18) > defA2 (sp+0x10) >
        // defAA (sp+0x08); POD types keep the builds as bare halfword stores.
        u32 args[4];
        CItemBoxColorRaw defA2;
        CItemBoxColorRaw defAA;
        defA2.b = 0;
        defA2.g = 0;
        defA2.r = 0;
        defA2.a = vA2;
        defAA.b = 0;
        defAA.g = 0;
        defAA.r = 0;
        defAA.a = vAA;
        args[0] = 0;
        args[1] = 0;
        args[2] = 0;
        args[3] = 0;
        // Selected colour is staged through the named temp then reloaded into
        // the sprintf argument block (retail stores to sp+0xf8/sp+0x100 first).
        if (flag != 0) {
            selA.w[0] = mColorA.w[0];
            selA.w[1] = mColorA.w[1];
        } else {
            selA.w[0] = *(u32*)&defA2.r;
            selA.w[1] = *(u32*)&defA2.a;
        }
        args[0] = selA.w[0];
        args[1] = selA.w[1];
        if (flag != 0) {
            selB.w[0] = mColorB.w[0];
            selB.w[1] = mColorB.w[1];
        } else {
            selB.w[0] = *(u32*)&defAA.r;
            selB.w[1] = *(u32*)&defAA.a;
        }
        args[2] = selB.w[0];
        args[3] = selB.w[1];
        sprintf(paneName, base + 0x161, (u8)i + 1, args[0], args[1], args[2], args[3]);
        nw4r::lyt::Pane* pane =
            mpLayout->GetRootPane()->FindPaneByName(paneName, true);
        if (pane != NULL) {
            void* obj = pane->GetMaterial();
            for (u32 j = 0; j < 2; j++) {
                setItemBoxVtxColor(obj, (u8)j, (u8*)args + (u8)j * 8);
            }
        }
        if (flag != 0 && arg3 != NULL) {
            u8 slot = (u8)GetCollectedFlagByte((u8)i);
            if (slot != 0) {
                void* member = CtrlObjectParam_GetCharEntry(slot);
                // Fixed equip category 2: candidate value read at member+0x26.
                void* item = CItem_kindPtr(2, *(s16*)((u8*)member + 0x26));
                if (item != NULL && *(u32*)item != 0 && arg3 == item) {
                    sprintf(label, base + 0x30e, (u8)i + 1);
                    u32 tex;
                    if (slot == (u8)GetCollectedFlagByte(playerIdx)) {
                        tex = (u32)mArcResAcc->GetResource(tagHi + 0x6d67, base + 0x319, NULL);
                    } else {
                        tex = (u32)mArcResAcc->GetResource(tagHi + 0x6d67, base + 0x32d, NULL);
                    }
                    if (tex == 0) {
                        tex = (u32)mArcResAcc->GetResource(tagHi + 0x6d67, base + 0x341, NULL);
                    }
                    if (tex != 0) {
                        PaneSetTexPaletteByName((nw4r::lyt::Layout*)mpLayout, (const char*)label, (TPLPalette*)tex);
                    }
                }
            }
        }
    }
    RenderTripleRows(arg2, arg3);
}
#pragma pop

void __declspec(noinline) setItemBoxVtxColor(void* arr, u32 index, const void* color) {
    if (index >= 3) {
        Panic__Q24nw4r2dbFPCciPCce(lbl_eu_8052E590, 0x8f, lbl_eu_8052E568);
    }
    __as__11_GXColorS10FRC11_GXColorS10((u8*)arr + index * 8 + 0x10, color);
}

void CItemBoxInfo::UpdateAnims() {
    if (mActive != 0) {
        switch (mState) {
            case 1: OpenPhase1(); break;
            case 2: OpenPhase2(); break;
            case 4: ClosePhase1(); break;
            case 5: ClosePhase2(); break;
        }
        ((nw4r::lyt::Layout*)mpLayout)->Animate(0);
    }
}

void CItemBoxInfo2::UpdateAnims() {
    if (mActive != 0) {
        switch (mState) {
            case 1: OpenPhase1(); break;
            case 2: OpenPhase2(); break;
            case 4: ClosePhase1(); break;
            case 5: ClosePhase2(); break;
        }
        ((nw4r::lyt::Layout*)mpLayout)->Animate(0);
    }
}

// Retail renderItemBoxDetail: item-box detail renderer. Builds a per-slot comparison
// record for the candidate item (buildItemBoxSlotFlags), lays out the label/stat
// panes, then per-slot colours the row panes and stamps the item name onto
// the slot whose record matches the candidate.
__declspec(noinline) void CItemBoxInfo::RenderDetail(u32 itemId, void* record, u32 arg4) {
    // Locals are declared in retail stack-slot order (low -> high addresses):
    // colour pair, vertex-colour pair, comparison record, pane-name buffers,
    // record copy, per-colour word temporaries.
    CItemBoxColorPOD cA;
    CItemBoxColorPOD cB;
    CItemBoxColorPOD quadCol[2];
    char buf2[0x20];
    char buf[0x20];
    // per-slot comparison record for the candidate item, built by
    // buildItemBoxSlotFlags and block-copied to a second local.
    ResetRowPanes();
    CItemBoxSlotFlags flags = BuildSlotFlags(itemId);
    char* base = lbl_eu_805063BC;
    LayoutSetTextBoxFmtValue((nw4r::lyt::Layout*)mpLayout, base + 0x25b, base + 0x2aa, 0);
    LayoutSetTextBoxFmtValue((nw4r::lyt::Layout*)mpLayout, base + 0x286, base + 0x2aa, 0);
    LayoutSetTextBoxFmtValue((nw4r::lyt::Layout*)mpLayout, base + 0x292, base + 0x2aa, 0);
    setLayoutTextBoxNumber((nw4r::lyt::Layout*)mpLayout, base + 0x267, (u8)flags.v[0]);
    setLayoutTextBoxNumber((nw4r::lyt::Layout*)mpLayout, base + 0x273, (u8)flags.v[1]);
    setLayoutTextBoxNumber((nw4r::lyt::Layout*)mpLayout, base + 0x29e, (u8)flags.v[2]);
    u32 v1 = BdatGetU8Direct((const void*)lbl_eu_806640F8, base + 0x1eb, (u16)BdatGetItemId(itemId));
    char* s1;
    switch ((u8)v1) {
        case 3: s1 = BdatTouchStringCell(base + 0x130, base + 0x139, 0x32); break;
        case 2: s1 = BdatTouchStringCell(base + 0x130, base + 0x139, 0x31); break;
        case 1: s1 = BdatTouchStringCell(base + 0x130, base + 0x139, 0x30); break;
        case 4: case 5: case 6: case 7: case 8:
        case 9: case 10: case 11: case 12: case 13:
            s1 = BdatTouchStringCell(base + 0x130, base + 0x139, 0x2e); break;
        default: s1 = 0; break;
    }
    LayoutSetTextBoxFmtValue((nw4r::lyt::Layout*)mpLayout, base + 0x354, s1, 0);
    char* s2;
    switch ((u8)v1) {
        case 4: s2 = BdatTouchStringCell(base + 0x248, base + 0x139, 0x77); break;
        case 5: s2 = BdatTouchStringCell(base + 0x248, base + 0x139, 0x78); break;
        case 6: s2 = BdatTouchStringCell(base + 0x248, base + 0x139, 0x79); break;
        case 7: s2 = BdatTouchStringCell(base + 0x248, base + 0x139, 0x7a); break;
        case 8: s2 = BdatTouchStringCell(base + 0x248, base + 0x139, 0x7b); break;
        case 9: s2 = BdatTouchStringCell(base + 0x248, base + 0x139, 0x7c); break;
        case 10: s2 = BdatTouchStringCell(base + 0x248, base + 0x139, 0x7d); break;
        case 11: s2 = BdatTouchStringCell(base + 0x248, base + 0x139, 0x7e); break;
        case 12: s2 = BdatTouchStringCell(base + 0x248, base + 0x139, 0x7f); break;
    }
    LayoutSetTextBoxFmtValue((nw4r::lyt::Layout*)mpLayout, base + 0x2ab, s2, 0);
    PaneMatSetTevColorsByName((nw4r::lyt::Layout*)mpLayout, base + 0x2b6, &lbl_eu_806645A8, &lbl_eu_806645B0);
    PaneMatSetTevColorsByName((nw4r::lyt::Layout*)mpLayout, base + 0x2c1, &lbl_eu_80664598, &lbl_eu_806645A0);
    PaneMatSetTevColorsByName((nw4r::lyt::Layout*)mpLayout, base + 0x2cc, &lbl_eu_80664598, &lbl_eu_806645A0);
    PaneMatSetTevColorsByName((nw4r::lyt::Layout*)mpLayout, base + 0x2d7, &lbl_eu_806645A8, &lbl_eu_806645B0);
    PaneMatSetTevColorsByName((nw4r::lyt::Layout*)mpLayout, base + 0x2e2, &lbl_eu_806645A8, &lbl_eu_806645B0);
    PaneMatSetTevColorsByName((nw4r::lyt::Layout*)mpLayout, base + 0x2ed, &lbl_eu_80664598, &lbl_eu_806645A0);
    PaneMatSetTevColorsByName((nw4r::lyt::Layout*)mpLayout, base + 0x2f8, &lbl_eu_80664598, &lbl_eu_806645A0);
    u32 cur = (arg4 >> 16) & 0xFF;
    u32 tag = 0x74696D67;
    for (u32 i = 0; (u8)i < (u8)code80135FDC_getByte_64077(); i++) {
        u8 flag = flags.equipable[(u8)i];
        u32 color = 0x777777FF;
        if (flag != 0) color = 0xFFFFFFFF;
        u32 idx = (u8)i + 1;
        sprintf(buf, (char*)&lbl_eu_805063BC[0x303], idx);
        PaneSetVtxColorByName((nw4r::lyt::Layout*)mpLayout, buf, color);
        // two vertex colours: filled slots use the this stat words, empty ones
        // fall back to transparent-black quads built member-wise here.
        cB.r = 0; cB.g = 0; cB.b = 0; cB.a = mColorA.s[3];
        cA.r = 0; cA.g = 0; cA.b = 0; cA.a = mColorB.s[3];
        quadCol[0].r = 0; quadCol[0].g = 0; quadCol[0].b = 0; quadCol[0].a = 0;
        quadCol[1].r = 0; quadCol[1].g = 0; quadCol[1].b = 0; quadCol[1].a = 0;
        // filled slots take the raw stat word pair from this; empty slots fall
        // back to the (zero) RGB halves of the member-wise colour locals.
        u32 w0, w1, w2, w3;
        if (flag != 0) {
            w0 = mColorA.w[0];
            w1 = mColorA.w[1];
        } else {
            w0 = *(u32*)&cB.r;
            w1 = *(u32*)&cB.b;
        }
        *(u32*)&quadCol[0].r = w0;
        *(u32*)&quadCol[0].b = w1;
        if (flag != 0) {
            w2 = mColorB.w[0];
            w3 = mColorB.w[1];
        } else {
            w2 = *(u32*)&cA.r;
            w3 = *(u32*)&cA.b;
        }
        *(u32*)&quadCol[1].r = w2;
        *(u32*)&quadCol[1].b = w3;
        sprintf(buf2, base + 0x161, idx);
        nw4r::lyt::Pane* pane =
            mpLayout->GetRootPane()->FindPaneByName(buf, true);
        if (pane != NULL) {
            void* mat = pane->GetMaterial();
            for (u32 j = 0; j < 2; j++) {
                setItemBoxVtxColor(mat, (u8)j, (u8*)quadCol + (u8)j * 8);
            }
        }
        if (flag == 0 || record == NULL) continue;
        int slot = GetCollectedFlagByte((u8)i);
        if ((u8)slot == 0) continue;
        void* lookup = CtrlObjectParam_GetCharEntry(slot);
        u32 cat = BdatGetItemType((u32)itemId);
        s16 value = -1;
        switch (cat & 0xFFFF) {
            case 4: value = *(s16*)((u8*)lookup + 0x1C); break;
            case 5: value = *(s16*)((u8*)lookup + 0x1E); break;
            case 6: value = *(s16*)((u8*)lookup + 0x20); break;
            case 7: value = *(s16*)((u8*)lookup + 0x22); break;
            case 8: value = *(s16*)((u8*)lookup + 0x24); break;
        }
        void* r = CItem_kindPtr(cat & 0xFFFF, value);
        if (r == NULL || *(u32*)r == 0 || r != record) continue;
        sprintf(buf2, base + 0x30e, idx);
        int curSlot = GetCollectedFlagByte(cur);
        u32 tex;
        if ((u8)slot == (u8)curSlot) {
            tex = (u32)mArcResAcc->GetResource(tag, base + 0x319, NULL);
        } else {
            tex = (u32)mArcResAcc->GetResource(tag, base + 0x32d, NULL);
        }
        if (tex == 0) {
            tex = (u32)mArcResAcc->GetResource(tag, base + 0x341, NULL);
        }
        if (tex != 0) {
            PaneSetTexPaletteByName((nw4r::lyt::Layout*)mpLayout, (const char*)buf2, (TPLPalette*)tex);
        }
    }
    RenderTripleRows(itemId, record);
}
// Retail renderItemBoxTripleRows: item-box slot renderer. With a candidate record it
// walks the three slot panes showing per-slot item info (regular items via
// the item table, equipped gear via the sub-record); with no record it shows
// the plain item-name list. Ends by stamping the per-slot data into info.
// Retail uses the stmw/lmw frame (MWCC optimize_for_size prologue merge) with
// same-TU helpers kept as real bl calls.
#pragma push
#pragma optimize_for_size on
__declspec(noinline) void CItemBoxInfo::RenderTripleRows(u32 itemId, void* record) {
    ml::FixStr<32> text(true);
    ClearTripleRows();
    char* base = lbl_eu_805063BC;
    char buf[0x20];
    char buf2[0x20];
    record = record != NULL ? record : NULL;
    if (record != NULL && *(u32*)record != 0) {
        // Retail re-fetches the item interface at each use; it is never held
        // live across calls, so keep it scoped to avoid burning a
        // callee-saved register and shifting the whole allocation window.
        u16 count = (u16)CItem_initItemImplInstances((CItemData*)record)->vf30((CItemData*)record);
        u32 tag = 0x74696D67;
        for (u32 i = 0; i < 3; i++) {
            u32 idx = (u8)i + 1;
            sprintf(buf, base + 0x35f, idx);
            nw4r::lyt::Pane* pane = mpLayout->GetRootPane()->FindPaneByName(buf, true);
            if ((u8)i < count) {
                setPaneVisible(pane, 1);
                s16 itemVal = 0;
                u8 itemCount = 0;
                s16 value = 0;
                u32 tex = 0;
                CItemImpl* inst = CItem_initItemImplInstances((CItemData*)record);
                s16 v40 = (s16)inst->vf40((CItemData*)record, (u8)i);
                if (v40 != -1) {
                    // regular item path: look the item up and format its name.
                    void* rec = CItem_kindPtr(3, v40);
                    u32 cat = *(u32*)rec >> 20;
                    CItemBoxNameRecord rec2;
                    rec2 = BuildNameRec((void*)cat, rec);
                    // Retail copies the 0xA4-byte record to a second local via
                    // the memcpy mtctr 8-byte-pair loop (li r0,0x14 + tail).
                    // Whole-struct assign: MWCC expands the POD copy to the
                    // retail mtctr 8-byte-pair loop.
                    CItemBoxNameRecord rec2b = rec2;
                    {
                        // (was switch: retail compiles this dispatch as compares)
                        u32 _e = (u32)(rec2b.e1);
                        if (_e == 0) tex = (u32)mTexArcResAcc->GetResource(tag, base + 0x36e, NULL);
                        else if (_e == 4) tex = (u32)mTexArcResAcc->GetResource(tag, base + 0x384, NULL);
                        else if (_e == 5) tex = (u32)mTexArcResAcc->GetResource(tag, base + 0x39a, NULL);
                        else if (_e == 6) tex = (u32)mTexArcResAcc->GetResource(tag, base + 0x3b0, NULL);
                        else if (_e == 7) tex = (u32)mTexArcResAcc->GetResource(tag, base + 0x3c6, NULL);
                        else if (_e == 8) tex = (u32)mTexArcResAcc->GetResource(tag, base + 0x3dc, NULL);
                        else if (_e == 9) tex = (u32)mTexArcResAcc->GetResource(tag, base + 0x3f2, NULL);
                    }
                    if (rec2b.name[0] == '0') {
                        sprintf(buf, base + 0x408, idx);
                        LayoutSetTextBoxFmtValue(mpLayout, buf, base + 0x2aa, 0);
                    } else {
                        if (rec2b.e0 == 0) {
                            text.format(base + 0x41e, rec2b.name);
                        } else if ((u8)(rec2b.e0 - 3) <= 1) {
                            text.format(base + 0x419, rec2b.name, BdatTouchStringCell(base + 0x130, base + 0x139, 0x21));
                        } else {
                            text.format(base + 0x41e, rec2b.name);
                        }
                        sprintf(buf, base + 0x408, idx);
                        LayoutSetTextBoxFmtValue(mpLayout, buf, text.c_str(), 0);
                    }
                    inst = CItem_initItemImplInstances((CItemData*)rec);
                    itemVal = inst->vf54((CItemData*)rec);
                    itemCount = (u8)inst->vf08((CItemData*)rec);
                    value = (s16)inst->vf90((CItemData*)rec);
                } else {
                    // equipped-gear path via the sub-record; no-item fallback.
                    CItemBoxSubRecord* sub = (CItemBoxSubRecord*)inst->vf2C((CItemData*)record, (u8)i);
                    if (sub == NULL || (sub->field_04 & 1) == 0) {
                        tex = (u32)mTexArcResAcc->GetResource(tag, base + 0x36e, NULL);
                        text.format(base + 0x18, BdatTouchStringCell(base + 0x130, base + 0x139, 0x2a));
                    } else {
                        u16 equip = (u16)((sub->field_04 >> 16) & 0xFFF);
                        u32 w = sub->_00;
                        char* label = BdatTouchStringCell(base + 0x130, base + 0x139, 0x1e - ((w >> 22 & 7) - 1));
                        u32 st = BdatGetU8Direct((const void*)lbl_eu_806640D8, base + 0x20b, equip);
                        {
                            // (was switch: retail compiles this dispatch as compares)
                            u32 _e = (u32)(st & 0xFF);
                            if (_e == 0) tex = (u32)mTexArcResAcc->GetResource(tag, base + 0x36e, NULL);
                            else if (_e == 4) tex = (u32)mTexArcResAcc->GetResource(tag, base + 0x384, NULL);
                            else if (_e == 5) tex = (u32)mTexArcResAcc->GetResource(tag, base + 0x39a, NULL);
                            else if (_e == 6) tex = (u32)mTexArcResAcc->GetResource(tag, base + 0x3b0, NULL);
                            else if (_e == 7) tex = (u32)mTexArcResAcc->GetResource(tag, base + 0x3c6, NULL);
                            else if (_e == 8) tex = (u32)mTexArcResAcc->GetResource(tag, base + 0x3dc, NULL);
                            else if (_e == 9) tex = (u32)mTexArcResAcc->GetResource(tag, base + 0x3f2, NULL);
                        }
                        s16 val = (s16)((w >> 11) & 0x7FF);
                        if (val != 0) {
                            u32 v2 = BdatGetU8Direct((const void*)lbl_eu_806640D8, base + 0x3, equip);
                            if (v2 != 0 && (u8)(v2 - 3) <= 1) {
                                text.format(base + 0x13e, val, BdatTouchStringCell(base + 0x130, base + 0x139, 0x21));
                            } else {
                                text.format(base + 0x422, val);
                            }
                            sprintf(buf, base + 0x408, idx);
                            LayoutSetTextBoxFmtValue(mpLayout, buf, text.c_str(), 0);
                        } else {
                            sprintf(buf, base + 0x408, idx);
                            LayoutSetTextBoxFmtValue(mpLayout, buf, base + 0x2aa, 0);
                        }
                        char* s = BdatGetPtrDirect((const void*)lbl_eu_806640D8, base + 0x139, equip);
                        text.format(base + 0x419, s, label);
                        itemCount = (u8)((w >> 22) & 7);
                        value = (s16)((w >> 11) & 0x7FF);
                    }
                }
                sprintf(buf, base + 0x426, idx);
                LayoutSetTextBoxFmtValue(mpLayout, buf, text.c_str(), 0);
                if (tex != 0) PaneSetTexPalette(pane, (TPLPalette*)tex);
                if (((u8*)this)[0x9A] != 4) {
                    nw4r::lyt::Pane* pane2 = mpLayout->GetRootPane()->FindPaneByName(base + 0x16e, true);
                    // POD vectors: default/copy ctors would emit extra bl calls
                    // absent from retail; field-wise staging reproduces the
                    // three lwz/stw word pairs before copyVEC3.
                    nw4r::math::_VEC3 pos;
                    ScalePaneWorldPos((nw4r::math::VEC3*)&pos, pane, pane2, mpLayout->GetRootPane());
                    nw4r::math::_VEC3 tmp;
                    if ((u8)i < 12) *(s16*)&mPage.mItemIds[(u8)i] = itemVal;
                    if ((u8)i < 12) ((u8*)this)[0x158 + (u8)i] = 3;
                    if ((u8)i < 12) ((u8*)this)[0x164 + (u8)i] = itemCount;
                    if ((u8)i < 12) mPage.mValues[(u8)i] = value;
                    tmp.x = pos.x;
                    tmp.y = pos.y;
                    tmp.z = pos.z;
                    if ((u8)i < 12) copyVEC3(&mPage.mPos[(u8)i], &tmp);
                }
            } else {
                setPaneVisible(pane, 0);
            }
        }
    } else {
        // No candidate record: plain item-name list driven by the name table.
        void* obj = (void*)((char* (*)(u32))LookupItemTypeTable)(itemId);
        u32 cat = BdatGetItemId(itemId);
        u8 sel = (u8)BdatGetU8Direct((const void*)obj, base + 0x432, (u16)cat);
        u32 tag = 0x74696D67;
        for (u32 i = 0; i < 3; i++) {
            u32 idx = (u8)i + 1;
            sprintf(buf2, base + 0x35f, idx);
            nw4r::lyt::Pane* pane = mpLayout->GetRootPane()->FindPaneByName(buf2, true);
            if ((u8)i < sel) {
                setPaneVisible(pane, 1);
                u32 itemVal = 0;
                u32 itemCount = 0;
                u8 val = 0;
                u32 tex = 0;
                text.format(base + 0x43b, idx);
                u16 nameId = (u16)BdatGetU16Direct((void*)obj, text.c_str(), (u16)cat);
                if (nameId == 0) {
                    tex = (u32)mTexArcResAcc->GetResource(tag, base + 0x36e, NULL);
                    text.format(base + 0x18, BdatTouchStringCell(base + 0x130, base + 0x139, 0x2a));
                } else {
                    CItemBoxNameRecord rec3;
                    rec3 = BuildNameRec((void*)nameId, 0);
                    CItemBoxNameRecord rec3b = rec3;
                    {
                        // (was switch: retail compiles this dispatch as compares)
                        u32 _e = (u32)(rec3b.e1);
                        if (_e == 0) tex = (u32)mTexArcResAcc->GetResource(tag, base + 0x36e, NULL);
                        else if (_e == 4) tex = (u32)mTexArcResAcc->GetResource(tag, base + 0x384, NULL);
                        else if (_e == 5) tex = (u32)mTexArcResAcc->GetResource(tag, base + 0x39a, NULL);
                        else if (_e == 6) tex = (u32)mTexArcResAcc->GetResource(tag, base + 0x3b0, NULL);
                        else if (_e == 7) tex = (u32)mTexArcResAcc->GetResource(tag, base + 0x3c6, NULL);
                        else if (_e == 8) tex = (u32)mTexArcResAcc->GetResource(tag, base + 0x3dc, NULL);
                        else if (_e == 9) tex = (u32)mTexArcResAcc->GetResource(tag, base + 0x3f2, NULL);
                    }
                    if (rec3b.name[0] == '0') {
                        sprintf(buf2, base + 0x408, idx);
                        LayoutSetTextBoxFmtValue(mpLayout, buf2, base + 0x2aa, 0);
                    } else {
                        if (rec3b.e0 == 0) {
                            text.format(base + 0x41e, rec3b.name);
                        } else if ((u8)(rec3b.e0 - 3) <= 1) {
                            text.format(base + 0x419, rec3b.name, BdatTouchStringCell(base + 0x130, base + 0x139, 0x21));
                        } else {
                            text.format(base + 0x41e, rec3b.name);
                        }
                        sprintf(buf2, base + 0x408, idx);
                        LayoutSetTextBoxFmtValue(mpLayout, buf2, text.c_str(), 0);
                    }
                    char* s = BuildItemNameStr(nameId);
                    text.format(base + 0x419, rec3b.str, s);
                    itemVal = BdatGetItemId(nameId);
                    itemCount = BdatGetU8Direct((const void*)lbl_eu_806640EC, base + 0x1f9, nameId);
                    val = (u8)BdatGetU8Direct((const void*)lbl_eu_806640EC, base + 0x447, nameId);
                }
                sprintf(buf2, base + 0x426, idx);
                LayoutSetTextBoxFmtValue(mpLayout, buf2, text.c_str(), 0);
                if (tex != 0) PaneSetTexPalette(pane, (TPLPalette*)tex);
                if (((u8*)this)[0x9A] != 4) {
                    nw4r::lyt::Pane* pane2 = mpLayout->GetRootPane()->FindPaneByName(base + 0x16e, true);
                    nw4r::math::_VEC3 pos;
                    ScalePaneWorldPos((nw4r::math::VEC3*)&pos, pane, pane2, mpLayout->GetRootPane());
                    nw4r::math::_VEC3 tmp;
                    if ((u8)i < 12) *(s16*)&mPage.mItemIds[(u8)i] = (s16)itemVal;
                    if ((u8)i < 12) ((u8*)this)[0x158 + (u8)i] = 3;
                    if ((u8)i < 12) ((u8*)this)[0x164 + (u8)i] = (u8)itemCount;
                    if ((u8)i < 12) mPage.mValues[(u8)i] = (s16)val;
                    tmp.x = pos.x;
                    tmp.y = pos.y;
                    tmp.z = pos.z;
                    if ((u8)i < 12) copyVEC3(&mPage.mPos[(u8)i], &tmp);
                }
            } else {
                setPaneVisible(pane, 0);
            }
        }
    }
#pragma pop
}

#pragma push
__declspec(noinline) void CItemBoxPage::SetValue(unsigned char index, short value) {
    if (index >= 12) return;
    mValues[index] = value;
}
#pragma pop

// Retail uses the stmw/lmw frame (MWCC optimize_for_size prologue merge) and
// keeps the same-TU helpers as real bl calls (dont_inline blocks IPA from
// folding applyItemBoxSelColors/resetItemBoxRowPanes/buildItemBoxSelRec into this body).
#pragma push
#pragma optimize_for_size on
__declspec(noinline) void CItemBoxInfo::RefreshSlotRows(u16 arg2) {
    ApplySelColors();
    ResetRowPanes();
    char buf[0x20];
    // 9-byte per-slot selection record: sel[1..8] are per-slot flags, flag2
    // also selects the row-height label size.
    CItemBoxSelRec local = BuildSelRec(arg2);
    u32 rowHeight = 0x2c;
    if (local.flag2 != 0) rowHeight = 0x2b;
    char* label = BdatTouchStringCell(&lbl_eu_805063BC[0x130], &lbl_eu_805063BC[0x139], rowHeight);
    LayoutSetTextBoxFmtValue((nw4r::lyt::Layout*)mpLayout, &lbl_eu_805063BC[0x44f], label, 0);
    for (u32 i = 0; (u8)i < (u8)code80135FDC_getByte_64077(); i++) {
        u8 flag = local.selected[(u8)i];
        u32 color = 0x777777ff;
        if (flag) color = 0xFFFFFFFF;
        sprintf(buf, &lbl_eu_805063BC[0x303], (u8)i + 1);
        PaneSetVtxColorByName((nw4r::lyt::Layout*)mpLayout, buf, color);
        // Unselected rows keep only the alpha of the highlight colours.
        CItemBoxQuadColorView dimA;
        dimA.s[0] = dimA.s[1] = dimA.s[2] = 0;
        dimA.s[3] = mColorA.s[3];
        CItemBoxQuadColorView dimB;
        dimB.s[0] = dimB.s[1] = dimB.s[2] = 0;
        dimB.s[3] = mColorB.s[3];
        CItemBoxQuadColorView colors[2] = {
            flag ? mColorA : dimA,
            flag ? mColorB : dimB,
        };
        sprintf(buf, &lbl_eu_805063BC[0x161], (u8)i + 1);
        nw4r::lyt::Pane* pane = mpLayout->GetRootPane()->FindPaneByName(buf, true);
        if (pane != NULL) {
            nw4r::lyt::Material* material = pane->GetMaterial();
            for (u32 j = 0; j < 2; j++) {
                setItemBoxVtxColor(material, (u8)j, &colors[(u8)j]);
            }
        }
    }
}
#pragma pop
#pragma push
#pragma optimize_for_size on
// Retail renderItemBoxKind3 (kind-3 detail renderer): rebuild the detail record
// (buildItemBoxNameRec), print the description/caption text, position the cursor
// row from the layout panes, stash the equip summary at info+0xB0.., then per
// player slot recolor the row panes and stamp the owning player's name onto
// equipment entries that resolve back to the selected item.
__declspec(noinline) void CItemBoxInfo::RenderKind3(u16 arg2, void* arg3, u32 arg4) {
    ClearQuadTexts();
    ResetRowPanes();
    CItemBoxNameRecord rec = BuildNameRec((void*)(u32)arg2, arg3);
    // The record must be copy-INITIALIZED from the by-value return: MWCC
    // builds the result in a temp and emits the retail mtctr 8-byte-pair
    // copy loop into rec.
    char textStorage[sizeof(ml::FixStr<32>)];
    ml::FixStr<32>& caption = *reinterpret_cast<ml::FixStr<32>*>(textStorage);
    nw4r::math::_VEC3 pos;
    CItemBoxVertexColors cols;
    CItemBoxSlotVals vals;
    nw4r::math::_VEC3 tmp;
    CItemBoxTagStage cur;


    char* base = (char*)&lbl_eu_805063BC;
    LayoutSetTextBoxFmtValue((nw4r::lyt::Layout*)mpLayout, base + 0x45a, (char*)rec.str, 0);

    caption.mString[0] = 0;
    caption.mLength = 0;
    void* item = NULL;
    if (arg3 != NULL) {
        item = arg3;
    }
    if (rec.name[0] == 0x30) {
        LayoutSetTextBoxFmtValue((nw4r::lyt::Layout*)mpLayout, base + 0x466, base + 0x2aa, 0);
    } else {
        if (rec.e0 != 0) {
            // Flag values 3/4 pick a count-formatted caption label.
            if ((u32)(rec.e0 - 3) <= 1) {
                char* label = BdatTouchStringCell(base + 0x130, base + 0x139, 0x21);
                caption.format(base + 0x419, &rec.name[0], label);
            } else {
                caption.format(base + 0x41e, &rec.name[0]);
            }
        } else {
            caption.format(base + 0x41e, &rec.name[0]);
        }
        LayoutSetTextBoxFmtValue((nw4r::lyt::Layout*)mpLayout, base + 0x466, caption.mString, 0);
    }

    // Ternaries kept inline at their use sites so the results stay in the
    // ABI return register instead of living across the whole state!=4 block.
    LayoutSetTextBoxFmtValue((nw4r::lyt::Layout*)mpLayout, base + 0x472,
                  (arg3 != NULL)
                      ? CItem_initItemImplInstances((CItemData*)item)->vf20((CItemData*)item)
                      : BuildItemNameStr(arg2),
                  0);

    if (mKind != 4) {
        nw4r::lyt::Pane* pane =
            mpLayout->GetRootPane()->FindPaneByName(base + 0x472, true);
        nw4r::lyt::Pane* pane2 =
            mpLayout->GetRootPane()->FindPaneByName(base + 0x16e, true);
        ScalePaneWorldPos((nw4r::math::VEC3*)((nw4r::math::_VEC3*)&pos), pane, pane2,
                      mpLayout->GetRootPane());

        *(s16*)&mPage.mItemIds[0] = (arg3 != NULL)
            ? CItem_initItemImplInstances((CItemData*)item)->vf54((CItemData*)item)
            : BdatGetItemId(arg2);
        mPage.mBytesA8[0] = (arg3 != NULL) ? 3 : 9;
        mPage.mBytesB4[0] = (u8)((arg3 != NULL)
            ? (u16)CItem_initItemImplInstances((CItemData*)item)->vf08((CItemData*)item)
            : 0);
        mPage.SetValue(0, (s16)((arg3 != NULL)
                            ? (s16)CItem_initItemImplInstances((CItemData*)item)->vf90((CItemData*)item)
                            : 0));
        // Field-wise copy: retail expands this as three lwz/stw word pairs
        // (a whole-struct assign emits an out-of-line __as__ call).
        tmp.x = pos.x;
        tmp.y = pos.y;
        tmp.z = pos.z;
        copyVEC3(&mPage.mPos[0], &tmp);
    }

    u16 cat2 = (arg3 != NULL)
        ? CItem_initItemImplInstances((CItemData*)item)->vf54((CItemData*)item)
        : BdatGetItemId(arg2);
    u8 rows = (u8)BdatGetU8Direct((const void*)((u32)(&lbl_eu_806640D8)[cat2]), base + 0x47d, cat2);
    LayoutSetTextBoxFmtValue((nw4r::lyt::Layout*)mpLayout, base + 0x484,
                  BdatTouchStringCell(base + 0x130, base + 0x139, rows + 0x15), 0);

    // volatile: retail retains this never-read slot-kind table copy; a plain
    // local gets it DCE'd.
    volatile CItemBoxTagStage stage = { { 2, 4, 5, 6, 7, 8 } };
    u8 playerByte = (u8)(arg4 >> 16);

    for (u32 i = 0; (u32)(u8)i < (u32)(u8)code80135FDC_getByte_64077(); i++) {
        u32 idx = (u8)i + 1;
        sprintf(caption.mString, base + 0x303, idx);
        PaneSetVtxColorByName((nw4r::lyt::Layout*)mpLayout, caption.mString, -1);
        cols.w[0] = 0;
        cols.w[1] = 0;
        cols.w[2] = 0;
        cols.w[3] = 0;
        cols.w[0] = mColorA.w[0];
        cols.w[1] = mColorA.w[1];
        cols.w[2] = mColorB.w[0];
        cols.w[3] = mColorB.w[1];
        sprintf(caption.mString, base + 0x161, idx);
        nw4r::lyt::Pane* pane =
            mpLayout->GetRootPane()->FindPaneByName(textStorage, true);
        if (pane != NULL) {
            void* colorObj = pane->GetMaterial();
            for (u32 j = 0; j < 2; j++) {
                setItemBoxVtxColor(colorObj, j, &cols.w[j * 2]);
            }
        }
        if (arg3 == NULL) continue;
        u8 flag = GetCollectedFlagByte(i);
        if (flag == 0) continue;
        void* lookup = CtrlObjectParam_GetCharEntry(flag);
        cur.tbl = *(u32*)((u8*)lookup + 8);
        cur.h = *(u16*)((u8*)lookup + 0xc);
        vals.v26 = *(s16*)((u8*)lookup + 0x26);
        vals.v1C = *(s16*)((u8*)lookup + 0x1c);
        vals.v1E = *(s16*)((u8*)lookup + 0x1e);
        vals.v20 = *(s16*)((u8*)lookup + 0x20);
        vals.v22 = *(s16*)((u8*)lookup + 0x22);
        vals.v24 = *(s16*)((u8*)lookup + 0x24);
        for (u32 k = 0; k < 6; k++) {
            void* hit = CItem_kindPtr(((u8*)&cur)[k], vals.s[k]);
            if (hit == NULL || *(u32*)hit == 0) continue;
            u8 n = CItem_initItemImplInstances((CItemData*)hit)->vf30((CItemData*)hit);
            for (u32 m = 0; (u32)(u8)m < (u32)n; m++) {
                s16 v40 = CItem_initItemImplInstances((CItemData*)hit)->vf40((CItemData*)hit, m);
                void* rec3 = CItem_kindPtr(3, v40);
                if (rec3 == NULL || *(u32*)rec3 == 0 || rec3 != arg3) continue;
                sprintf(caption.mString, base + 0x30e, idx);
                u8 owner = GetCollectedFlagByte((u32)playerByte);
                u32 name;
                if (flag == owner) {
                    name = (u32)mArcResAcc->GetResource(0x74696d67, base + 0x319, NULL);
                } else {
                    name = (u32)mArcResAcc->GetResource(0x74696d67, base + 0x32d, NULL);
                }
                if (name == 0) {
                    name = (u32)mArcResAcc->GetResource(0x74696d67, base + 0x341, NULL);
                }
                if (name != 0) {
                    PaneSetTexPaletteByName((nw4r::lyt::Layout*)mpLayout, (const char*)caption.mString, (TPLPalette*)name);
                }
            }
        }
    }
}
#pragma pop
#pragma push
#pragma optimize_for_size on

__declspec(noinline) void CItemBoxInfo::RenderLabels(u32 arg2) {
    ClearPairTexts();
    ResetRowPanes();
    CItemBoxLabelRec rec = GetLabelRec(arg2);
    LayoutSetTextBoxFmtValue((nw4r::lyt::Layout*)mpLayout, (char*)&lbl_eu_805063BC + 0x48f, rec.b, 0);
    LayoutSetTextBoxFmtValue((nw4r::lyt::Layout*)mpLayout, (char*)&lbl_eu_805063BC + 0x49b, rec.d, 0);
}
#pragma pop

// Retail renderItemBoxSlotList: item-box slot-list renderer. Rebuilds the per-slot
// record (buildItemBoxSlotRec), stamps the header pane with the formatted item
// string, then per filled pair sets the row-name pane text and (unless mode
// 0x9A==4) measures the row position and caches value/count/pos per slot.
#pragma push
#pragma optimize_for_size on
__declspec(noinline) void CItemBoxInfo::RenderSlotList(u16 arg2, void* arg3) {
    // Frame layout (descending): cur > text > call temp > buf > pos > tmp.
    // The 9 constant claims r31 first (Rule A).
    u8 slotState = 9;
    char buf[0x20];
    nw4r::math::_VEC3 pos;
    ClearSlotTexts();
    ResetRowPanes();
    CItemBoxSlotRecord1 cur = BuildSlotRec(arg2, arg3);
    ml::FixStr<32> text(false);
    LayoutSetTextBoxFmtValue((nw4r::lyt::Layout*)mpLayout, &lbl_eu_805063BC[0x4a7], (char*)cur.str, 0);
    u8 count = cur.counter;
    for (u8 i = 0; (u8)i < count; i++) {
        sprintf(buf, &lbl_eu_805063BC[0x4b3], (u32)((u8)i * 2) + 0x1f);
        LayoutSetTextBoxFmtValue((nw4r::lyt::Layout*)mpLayout, buf, (char*)cur.text[(u8)i], 0);
        if (((u8*)this)[0x9A] != 4) {
            nw4r::lyt::Pane* pane =
                mpLayout->GetRootPane()->FindPaneByName(buf, true);
            nw4r::lyt::Pane* pane2 =
                mpLayout->GetRootPane()->FindPaneByName(&lbl_eu_805063BC[0x16e], true);
            ScalePaneWorldPos((nw4r::math::VEC3*)&pos, pane, pane2,
                          mpLayout->GetRootPane());
            // Bounds-checked per-slot cache writes; the count is loaded before
            // the index check, and the _VEC3 copy-init is a lwz/stw block copy.
            {
                u16 v = cur.counts[(u8)i];
                if ((u8)i < 12) mPage.mItemIds[(u8)i] = v;
            }
            if ((u8)i < 12) ((u8*)this)[0x158 + (u8)i] = slotState;
            nw4r::math::_VEC3 tmp = pos;
            if ((u8)i < 12) copyVEC3(&mPage.mPos[(u8)i], &tmp);
        }
        text.clear();
        sprintf(buf, &lbl_eu_805063BC[0x4c0], (u32)(u8)i + 0x1f);
        text.format(&lbl_eu_805063BC[0x13e], cur.vals[(u8)i],
                    BdatTouchStringCell(&lbl_eu_805063BC[0x130], &lbl_eu_805063BC[0x139], 0x21));
        LayoutSetTextBoxFmtValue((nw4r::lyt::Layout*)mpLayout, buf, text.c_str(), 0);
    }
}
#pragma pop
#pragma push
#pragma optimize_for_size on
__declspec(noinline) void CItemBoxInfo::SetupSlotPanes() {
    // No local base pointer: MWCC CSEs the repeated symbol offsets into one
    // register, splitting the address into a pre-stmw volatile lis plus a
    // post-stmw callee-saved addi exactly like retail.
    char buf[0x20];
    u32 i;
    LayoutSetTextBoxFmtValue((nw4r::lyt::Layout*)mpLayout, &lbl_eu_805063BC[0x25b], &lbl_eu_805063BC[0x2aa], 0);
    LayoutSetTextBoxFmtValue((nw4r::lyt::Layout*)mpLayout, &lbl_eu_805063BC[0x267], &lbl_eu_805063BC[0x2aa], 0);
    LayoutSetTextBoxFmtValue((nw4r::lyt::Layout*)mpLayout, &lbl_eu_805063BC[0x273], &lbl_eu_805063BC[0x2aa], 0);
    LayoutSetTextBoxFmtValue((nw4r::lyt::Layout*)mpLayout, &lbl_eu_805063BC[0x286], &lbl_eu_805063BC[0x2aa], 0);
    LayoutSetTextBoxFmtValue((nw4r::lyt::Layout*)mpLayout, &lbl_eu_805063BC[0x292], &lbl_eu_805063BC[0x2aa], 0);
    LayoutSetTextBoxFmtValue((nw4r::lyt::Layout*)mpLayout, &lbl_eu_805063BC[0x29e], &lbl_eu_805063BC[0x2aa], 0);
    LayoutSetTextBoxFmtValue((nw4r::lyt::Layout*)mpLayout, &lbl_eu_805063BC[0x354], &lbl_eu_805063BC[0x2aa], 0);
    LayoutSetTextBoxFmtValue((nw4r::lyt::Layout*)mpLayout, &lbl_eu_805063BC[0x2ab], &lbl_eu_805063BC[0x2aa], 0);
    PaneMatSetTevColorsByName((nw4r::lyt::Layout*)mpLayout, &lbl_eu_805063BC[0x2b6], &lbl_eu_806645A8, &lbl_eu_806645B0);
    PaneMatSetTevColorsByName((nw4r::lyt::Layout*)mpLayout, &lbl_eu_805063BC[0x2c1], &lbl_eu_806645A8, &lbl_eu_806645B0);
    PaneMatSetTevColorsByName((nw4r::lyt::Layout*)mpLayout, &lbl_eu_805063BC[0x2cc], &lbl_eu_806645A8, &lbl_eu_806645B0);
    PaneMatSetTevColorsByName((nw4r::lyt::Layout*)mpLayout, &lbl_eu_805063BC[0x2d7], &lbl_eu_806645A8, &lbl_eu_806645B0);
    PaneMatSetTevColorsByName((nw4r::lyt::Layout*)mpLayout, &lbl_eu_805063BC[0x2e2], &lbl_eu_806645A8, &lbl_eu_806645B0);
    PaneMatSetTevColorsByName((nw4r::lyt::Layout*)mpLayout, &lbl_eu_805063BC[0x2ed], &lbl_eu_806645A8, &lbl_eu_806645B0);
    PaneMatSetTevColorsByName((nw4r::lyt::Layout*)mpLayout, &lbl_eu_805063BC[0x2f8], &lbl_eu_806645A8, &lbl_eu_806645B0);
    for (i = 0; (u8)i < (u8)code80135FDC_getByte_64077(); i++) {
        u32 idx = (u8)i + 1;
        sprintf(buf, &lbl_eu_805063BC[0x303], idx);
        PaneSetVtxColorByName((nw4r::lyt::Layout*)mpLayout, buf, 0x777777ff);
        u32 args[4];
        // Zero via a variable: MWCC materializes it in a register and cannot
        // fold the {zero, zero} s16-pair arrays to a constant, keeping the
        // retail stack build of the four u32 colour args.
        s16 zero = 0;
        s16 c0hi[2] = {zero, mColorA.s[3]};
        // All-zero pairs: retail stores the halfwords in DESCENDING element
        // order; the four colour words are copied to args[] in ASCENDING
        // slot order (see resetItemBox2RowPanes finding).
        s16 c0lo[2];
        c0lo[1] = zero;
        c0lo[0] = zero;
        s16 c1hi[2] = {zero, mColorB.s[3]};
        s16 c1lo[2];
        c1lo[1] = zero;
        c1lo[0] = zero;
        args[0] = *(u32*)c0lo;
        args[1] = *(u32*)c0hi;
        args[2] = *(u32*)c1lo;
        args[3] = *(u32*)c1hi;
        sprintf(buf, &lbl_eu_805063BC[0x161], idx);
        nw4r::lyt::Pane* pane = mpLayout->GetRootPane()->FindPaneByName(buf, true);
        if (pane != NULL) {
            nw4r::lyt::Material* material = pane->GetMaterial();
            for (u32 j = 0; j < 2; j++) {
                setItemBoxVtxColor(material, (u8)j, (u8*)args + (u8)j * 8);
            }
        }
        // Item-name lookup on the shared arc resource accessor (vtable+0x0C),
        // then push the texture name onto the slot pane.
        sprintf(buf, &lbl_eu_805063BC[0x30e], idx);
        u32 tex = (u32)mArcResAcc->GetResource(0x74696D67, &lbl_eu_805063BC[0x341], NULL);
        if (tex != 0) {
            PaneSetTexPaletteByName((nw4r::lyt::Layout*)mpLayout, (const char*)buf, (TPLPalette*)tex);
        }
    }
}
#pragma pop
#pragma push
#pragma optimize_for_size on
__declspec(noinline) void CItemBoxInfo::ResetRowPanes() {
    u32 i;
    char* base = (char*)&lbl_eu_805063BC;
    u32 idx;
    u32 tag;
    u32 white;
    LayoutSetTextBoxFmtValue((nw4r::lyt::Layout*)mpLayout, base + 0x267, base + 0x2aa, 0);
    LayoutSetTextBoxFmtValue((nw4r::lyt::Layout*)mpLayout, base + 0x273, base + 0x2aa, 0);
    LayoutSetTextBoxFmtValue((nw4r::lyt::Layout*)mpLayout, base + 0x29e, base + 0x2aa, 0);
    LayoutSetTextBoxFmtValue((nw4r::lyt::Layout*)mpLayout, base + 0x354, base + 0x2aa, 0);
    LayoutSetTextBoxFmtValue((nw4r::lyt::Layout*)mpLayout, base + 0x2ab, base + 0x2aa, 0);
    PaneMatSetTevColorsByName((nw4r::lyt::Layout*)mpLayout, base + 0x2b6, &lbl_eu_806645A8, &lbl_eu_806645B0);
    PaneMatSetTevColorsByName((nw4r::lyt::Layout*)mpLayout, base + 0x2c1, &lbl_eu_806645A8, &lbl_eu_806645B0);
    PaneMatSetTevColorsByName((nw4r::lyt::Layout*)mpLayout, base + 0x2cc, &lbl_eu_806645A8, &lbl_eu_806645B0);
    PaneMatSetTevColorsByName((nw4r::lyt::Layout*)mpLayout, base + 0x2d7, &lbl_eu_806645A8, &lbl_eu_806645B0);
    PaneMatSetTevColorsByName((nw4r::lyt::Layout*)mpLayout, base + 0x2e2, &lbl_eu_806645A8, &lbl_eu_806645B0);
    PaneMatSetTevColorsByName((nw4r::lyt::Layout*)mpLayout, base + 0x2ed, &lbl_eu_806645A8, &lbl_eu_806645B0);
    PaneMatSetTevColorsByName((nw4r::lyt::Layout*)mpLayout, base + 0x2f8, &lbl_eu_806645A8, &lbl_eu_806645B0);
    char buf[0x20];
    tag = 0x74696D67u;
    white = 0x777777FFu;
    for (i = 0; (u8)i < (u8)code80135FDC_getByte_64077(); i++) {
        idx = (u32)((u8)i + 1);
        sprintf(buf, (char*)&lbl_eu_805063BC[0x303], idx);
        PaneSetVtxColorByName((nw4r::lyt::Layout*)mpLayout, buf, white);
        u32 args[4];
        // zero via a variable: MWCC materializes it in a register and cannot
        // fold the {zero, zero} s16-pair arrays to a constant, keeping the
        // retail stack build of the four u32 colour args.
        s16 zero = 0;
        s16 c0hi[2] = {0, mColorA.s[3]};
        s16 c0lo[2] = {0, 0};
        s16 c1hi[2] = {0, mColorB.s[3]};
        s16 c1lo[2] = {0, 0};
        args[1] = *(u32*)c0hi;
        args[0] = *(u32*)c0lo;
        args[3] = *(u32*)c1hi;
        args[2] = *(u32*)c1lo;
        sprintf(buf, (char*)&lbl_eu_805063BC[0x161], idx);
        nw4r::lyt::Pane* pane = mpLayout->GetRootPane()->FindPaneByName(buf, true);
        if (pane != NULL) {
            nw4r::lyt::Material* material = pane->GetMaterial();
            for (u32 j = 0; j < 2; j++) {
                setItemBoxVtxColor(material, (u8)j, (u8*)args + (u8)j * 8);
            }
        }
        // Item-name lookup on the shared arc resource accessor (vtable+0x0C),
        // then push the texture name onto the slot pane.
        sprintf(buf, base + 0x30e, idx);
        u32 tex = (u32)mArcResAcc->GetResource(tag, base + 0x341, NULL);
        if (tex != 0) {
            PaneSetTexPaletteByName((nw4r::lyt::Layout*)mpLayout, (const char*)buf, (TPLPalette*)tex);
        }
    }
}
#pragma pop
// Retail clearItemBoxTripleRows uses the stmw/lmw frame (MWCC optimize_for_size
// prologue merge): info in r28, loop counter in r29, index in r30,
// string-pool base in r31.
#pragma push
#pragma optimize_for_size on
__declspec(noinline) void CItemBoxInfo::ClearTripleRows() {
    char buf[0x20];
    // Declaration order fixes the r29/r30 coloring: retail claims index (r30)
    // before the loop counter i (r29) - saved regs go high->low by birth order.
    u32 index;
    u32 i;
    for (i = 0; i < 3; i++) {
        index = (u8)i + 1;
        sprintf(buf, (char*)&lbl_eu_805063BC[0x35f], index);
        void* r = ((LayoutRootPaneView*)mpLayout)->rootPane10->FindPaneByName(buf, true);
        setPaneVisible(r, 0);
        sprintf(buf, (char*)&lbl_eu_805063BC[0x408], index);
        LayoutSetTextBoxFmtValue((nw4r::lyt::Layout*)mpLayout, buf, (char*)&lbl_eu_805063BC[0x2aa], 0);
        sprintf(buf, (char*)&lbl_eu_805063BC[0x426], index);
        LayoutSetTextBoxFmtValue((nw4r::lyt::Layout*)mpLayout, buf, (char*)&lbl_eu_805063BC[0x2aa], 0);
    }
}
#pragma pop
// Retail applyItemBoxSelColors: label every slot pane, then build two 8-byte
// selection colours (from the s16s at +0xA2/+0xAA), format the pane name,
// look the pane up, and push both colours into its
// vertex buffer via setItemBoxVtxColor. layout is reloaded per use (lwz 0x34).
#pragma push
#pragma optimize_for_size on
__declspec(noinline) void CItemBoxInfo::ApplySelColors() {
    LayoutSetTextBoxFmtValue((nw4r::lyt::Layout*)mpLayout, &lbl_eu_805063BC[0x44f], &lbl_eu_805063BC[0x2aa], 0);
    char buf[0x20];
    u32 idx;
    // Retail re-evaluates the slot count on every iteration (bl in the
    // loop condition), so do not hoist the call.
    for (u32 i = 0; (u8)i < (u8)code80135FDC_getByte_64077(); i++) {
        idx = (u32)((u8)i + 1);
        sprintf(buf, (char*)&lbl_eu_805063BC[0x303], idx);
        PaneSetVtxColorByName((nw4r::lyt::Layout*)mpLayout, buf, 0x777777ff);
        // Selection colours: two 8-byte pairs built from the s16s at +0xA2
        // and +0xAA; retail stages the four arrays at sp+0x08..0x17 and the
        // u32 varargs copy at sp+0x18..0x27.
        s16 zero = 0;
        s16 c0hi[2] = {zero, mColorA.s[3]};
        s16 c0lo[2];
        c0lo[1] = zero;
        c0lo[0] = zero;
        s16 c1hi[2] = {zero, mColorB.s[3]};
        s16 c1lo[2];
        c1lo[1] = zero;
        c1lo[0] = zero;
        u32 args[4];
        args[0] = *(u32*)c0lo;
        args[1] = *(u32*)c0hi;
        args[2] = *(u32*)c1lo;
        args[3] = *(u32*)c1hi;
        sprintf(buf, (char*)&lbl_eu_805063BC[0x161], idx);
        nw4r::lyt::Pane* pane = mpLayout->GetRootPane()->FindPaneByName(buf, true);
        if (pane != NULL) {
            void* obj = pane->GetMaterial();
            for (u32 j = 0; j < 2; j++) {
                setItemBoxVtxColor(obj, (u8)j, (u8*)args + (u8)j * 8);
            }
        }
    }
}
#pragma pop
// Retail clearItemBoxQuadTexts uses the stmw/lmw frame (MWCC optimize_for_size
// prologue merge); the first call's arg4 zero is hoisted into the prologue.
#pragma push
#pragma optimize_for_size on
__declspec(noinline) void CItemBoxInfo::ClearQuadTexts() {
    LayoutSetTextBoxFmtValue((nw4r::lyt::Layout*)mpLayout,
                  &lbl_eu_805063BC[0x45a],
                  &lbl_eu_805063BC[0x2aa], 0);
    LayoutSetTextBoxFmtValue((nw4r::lyt::Layout*)mpLayout,
                  &lbl_eu_805063BC[0x466],
                  &lbl_eu_805063BC[0x2aa], 0);
    LayoutSetTextBoxFmtValue((nw4r::lyt::Layout*)mpLayout,
                  &lbl_eu_805063BC[0x484],
                  &lbl_eu_805063BC[0x2aa], 0);
    LayoutSetTextBoxFmtValue((nw4r::lyt::Layout*)mpLayout,
                  &lbl_eu_805063BC[0x472],
                  &lbl_eu_805063BC[0x2aa], 0);
}
#pragma pop

// Retail clearItemBoxSlotTexts uses the stmw/lmw frame (MWCC optimize_for_size
// prologue merge): info in r29, loop counter in r30, string-pool base in r31.
#pragma push
#pragma optimize_for_size on
__declspec(noinline) void CItemBoxInfo::ClearSlotTexts() {
    char buf[0x20];
    LayoutSetTextBoxFmtValue((nw4r::lyt::Layout*)mpLayout,
                  &lbl_eu_805063BC[0x4a7],
                  &lbl_eu_805063BC[0x2aa], 0);
    for (u32 i = 0; i < 4; i++) {
        sprintf(buf, &lbl_eu_805063BC[0x4b3], (u8)i * 2 + 0x1f);
        LayoutSetTextBoxFmtValue((nw4r::lyt::Layout*)mpLayout, buf,
                      &lbl_eu_805063BC[0x2aa], 0);
        sprintf(buf, &lbl_eu_805063BC[0x4c0], (u8)i + 0x1f);
        LayoutSetTextBoxFmtValue((nw4r::lyt::Layout*)mpLayout, buf,
                      &lbl_eu_805063BC[0x2aa], 0);
    }
}
#pragma pop

#pragma push
#pragma optimize_for_size on
__declspec(noinline) void CItemBoxInfo::ClearPairTexts() {
    LayoutSetTextBoxFmtValue((nw4r::lyt::Layout*)mpLayout,
                  &lbl_eu_805063BC[0x48f],
                  &lbl_eu_805063BC[0x2aa], 0);
    LayoutSetTextBoxFmtValue((nw4r::lyt::Layout*)mpLayout,
                  &lbl_eu_805063BC[0x49b],
                  &lbl_eu_805063BC[0x2aa], 0);
}
#pragma pop

// Retail stampItemBoxSlotTex: per-slot item-box1 renderer. Loops 7 slots; for slots
// below the active count it resolves the item name via CUICfManager_getArcResourceAccessor/vtable[3]
// and colors the pane; slots at/above the count get a plain label pane. Uses
// the stmw/lmw frame (optimize_for_size prologue merge). The two sprintf
// buffers stay separate locals (retail sp+0x48 / sp+0x28).
#pragma push
#pragma optimize_for_size on
__declspec(noinline) void CItemBoxInfo::StampSlotTex() {
    // Shape mirrors the proven stampItemBox2SlotTex (ItemBox2 twin): if/else instead
    // of continue-guards, hoisted i/idx, direct lbl refs, typed name lookup.
    u32 i;
    char bufElse[0x20];
    char buf[0x20];
    u8 count = code80135FDC_getByte_64077();
    u32 idx;
    for (i = 0; i < 7; i++) {
        if ((u8)i >= count) {
            sprintf(bufElse, (char*)&lbl_eu_805063BC[0x161], (u8)i + 1);
            nw4r::lyt::Pane* pane = mpLayout->GetRootPane()->FindPaneByName(bufElse, true);
            setPaneVisible(pane, 0);
        } else {
            u8 slot = (u8)GetCollectedFlagByte((u8)i);
            u32 tag = 0x74696D67u;
            u32 nameId = (u16)BdatGetU16Direct(lbl_eu_80664090, (char*)&lbl_eu_805063BC[0x4ce], slot);
            u32 itemId = (u32)MakeTplNameSysFile(nameId);
            u32 found = (u32)CUICfManager_getArcResourceAccessor()->GetResource(tag, (const char*)itemId, NULL);
            if (found != 0) {
                idx = (u32)((u8)i + 1);
                sprintf(buf, (char*)&lbl_eu_805063BC[0x303], idx);
                nw4r::lyt::Pane* pane2 = mpLayout->GetRootPane()->FindPaneByName(buf, true);
                PaneSetTexPalette(pane2, (TPLPalette*)found);
                PaneSetVtxColorAll(pane2, 0x777777ffu);
                s16 zero = 0;
                s16 c0hi[2] = {zero, mColorA.s[3]};
                // All-zero pairs stored DESCENDING (retail order).
                s16 c0lo[2];
                c0lo[1] = zero;
                c0lo[0] = zero;
                s16 c1hi[2] = {zero, mColorB.s[3]};
                s16 c1lo[2];
                c1lo[1] = zero;
                c1lo[0] = zero;
                u32 args[4];
                args[0] = *(u32*)c0lo;
                args[1] = *(u32*)c0hi;
                args[2] = *(u32*)c1lo;
                args[3] = *(u32*)c1hi;
                sprintf(buf, (char*)&lbl_eu_805063BC[0x161], idx);
                nw4r::lyt::Pane* pane3 = mpLayout->GetRootPane()->FindPaneByName(buf, true);
                void* vertexObj = pane3->GetMaterial();
                for (u32 j = 0; j < 2; j++) {
                    setItemBoxVtxColor(vertexObj, (u8)j, (u8*)args + (u8)j * 8);
                }
            }
        }
    }
}
#pragma pop
// Local data shapes used by the ItemBox1 comparison renderer.
struct D8EQuad {
    s16 r, g, b, a;
};
struct D8ERenderQuad {
    f32 values[8];
};
union D8EEntry {
    CItemBoxCompRecord rec;
    u32 words[13];
    s16 halfWords[26];
};
struct D8EArmorEntry {
    union {
        u32 words[7];
        s16 halfWords[14];
        struct {
            u16 _00;
            u16 _02;
            s16 etherDefense;
            s16 physicalDefense;
            u8 _08[0x14];
        };
    };
};
union D8EComparisonStorage {
    D8EEntry weapon[4];
    D8EArmorEntry armor[4];
};
struct D8EPartyData { u32 w[12]; };
// Distinct type for the high-frame party band.
struct D8EPartySlot { u32 w[12]; };
// Retail high-frame band (butting 0x4330 temps at sp+2592):
//   party@2352 (48B) | listB@2400 (96B) | listA@2496 (96B) = 240B -> base@2352.
struct D8EHighFrame {
    D8EPartySlot party;
    CItemBoxInfoEntry listB[8];
    CItemBoxInfoEntry listA[8];
};

static inline void scaleArmorDefense(D8EArmorEntry& entry, s32 effect) {
    f32 scale = 0.01f * (100.0f + (f32)effect);
    entry.physicalDefense = (s16)(s32)((f32)entry.physicalDefense * scale);
    entry.etherDefense = (s16)(s32)((f32)entry.etherDefense * scale);
}

static inline void applyArmorEffects(CItemBoxInfo* info, u16 member,
                                     D8EArmorEntry& entry, u8 armorType) {
    switch (armorType) {
        case 3: {
            s32 effect = info->GetCondStat((void*)(u32)member, 0x0D);
            if (effect != 0) scaleArmorDefense(entry, effect);
            effect = info->GetCondStat((void*)(u32)member, 0x24);
            if (effect != 0) {
                entry.etherDefense = (s16)(entry.etherDefense + (s16)effect);
            }
            break;
        }
        case 2: {
            s32 effect = info->GetCondStat((void*)(u32)member, 0x0C);
            if (effect != 0) scaleArmorDefense(entry, effect);
            break;
        }
        case 1: {
            s32 effect = info->GetCondStat((void*)(u32)member, 0x0B);
            if (effect != 0) scaleArmorDefense(entry, effect);
            break;
        }
    }
}
struct D8EStatC {
    u8 _00[6];
    s16 s06;
    u8 _08[4];
    s16 s0C, s0E, s10;
    u8 _12[6];
    s16 s18;
    u8 _1A[2];
    s16 s1C;
    u8 _1E[4];
    s16 s22;
};
struct D8EStatA {
    u8 _00[0x10];
    f32 f10;
    u8 _14[8];
    s16 s1C, s1E, s20;
    u8 _22[0xC];
    s16 s2E;
    u8 _30[2];
    s16 s32;
    u8 _34[4];
    s16 s38;
    u8 _3A[0x1B];
    u8 b55;
};
struct D8EStatB {
    u8 _00[0x10];
    f32 f10;
};

static inline void applyItemBoxDeltaColor(void* valuePane, void* labelPane, s16 delta) {
    D8EQuad labelTop = *(D8EQuad*)&lbl_eu_80664518;
    D8EQuad labelBottom = *(D8EQuad*)&lbl_eu_80664520;
    D8EQuad valueTop = *(D8EQuad*)&lbl_eu_80664558;
    D8EQuad valueBottom = *(D8EQuad*)&lbl_eu_80664560;
    if (delta < 0) {
        __as__11_GXColorS10FRC11_GXColorS10(&labelTop, &lbl_eu_80664538);
        __as__11_GXColorS10FRC11_GXColorS10(&labelBottom, &lbl_eu_80664540);
        __as__11_GXColorS10FRC11_GXColorS10(&valueTop, &lbl_eu_80664578);
        __as__11_GXColorS10FRC11_GXColorS10(&valueBottom, &lbl_eu_80664580);
    } else if (delta > 0) {
        __as__11_GXColorS10FRC11_GXColorS10(&labelTop, &lbl_eu_80664528);
        __as__11_GXColorS10FRC11_GXColorS10(&labelBottom, &lbl_eu_80664530);
        __as__11_GXColorS10FRC11_GXColorS10(&valueTop, &lbl_eu_80664568);
        __as__11_GXColorS10FRC11_GXColorS10(&valueBottom, &lbl_eu_80664570);
    }
    PaneMatSetTevColors(valuePane, &valueTop, &valueBottom);
    PaneMatSetTevColors(labelPane, &labelTop, &labelBottom);
}

static inline void applyItemBoxDeltaColorAt(CItemBoxInfo* info, u32 valueIndex,
                                             u32 labelIndex, s16 delta) {
    D8EQuad labelTop = *(D8EQuad*)&lbl_eu_80664518;
    D8EQuad labelBottom = *(D8EQuad*)&lbl_eu_80664520;
    D8EQuad valueTop = *(D8EQuad*)&lbl_eu_80664558;
    D8EQuad valueBottom = *(D8EQuad*)&lbl_eu_80664560;
    if (delta < 0) {
        __as__11_GXColorS10FRC11_GXColorS10(&labelTop, &lbl_eu_80664538);
        __as__11_GXColorS10FRC11_GXColorS10(&labelBottom, &lbl_eu_80664540);
        __as__11_GXColorS10FRC11_GXColorS10(&valueTop, &lbl_eu_80664578);
        __as__11_GXColorS10FRC11_GXColorS10(&valueBottom, &lbl_eu_80664580);
    } else if (delta > 0) {
        __as__11_GXColorS10FRC11_GXColorS10(&labelTop, &lbl_eu_80664528);
        __as__11_GXColorS10FRC11_GXColorS10(&labelBottom, &lbl_eu_80664530);
        __as__11_GXColorS10FRC11_GXColorS10(&valueTop, &lbl_eu_80664568);
        __as__11_GXColorS10FRC11_GXColorS10(&valueBottom, &lbl_eu_80664570);
    }
    nw4r::lyt::Pane** panes = &info->mSlotPane0;
    PaneMatSetTevColors(panes[valueIndex], &valueTop, &valueBottom);
    PaneMatSetTevColors(panes[labelIndex], &labelTop, &labelBottom);
}

static inline void applyItemBoxDeltaColorPair(void* firstValuePane, void* firstLabelPane,
                                               void* secondValuePane, void* secondLabelPane,
                                               s16 delta) {
    D8EQuad labelTop = *(D8EQuad*)&lbl_eu_80664518;
    D8EQuad labelBottom = *(D8EQuad*)&lbl_eu_80664520;
    D8EQuad valueTop = *(D8EQuad*)&lbl_eu_80664558;
    D8EQuad valueBottom = *(D8EQuad*)&lbl_eu_80664560;
    if (delta < 0) {
        __as__11_GXColorS10FRC11_GXColorS10(&labelTop, &lbl_eu_80664538);
        __as__11_GXColorS10FRC11_GXColorS10(&labelBottom, &lbl_eu_80664540);
        __as__11_GXColorS10FRC11_GXColorS10(&valueTop, &lbl_eu_80664578);
        __as__11_GXColorS10FRC11_GXColorS10(&valueBottom, &lbl_eu_80664580);
    } else if (delta > 0) {
        __as__11_GXColorS10FRC11_GXColorS10(&labelTop, &lbl_eu_80664528);
        __as__11_GXColorS10FRC11_GXColorS10(&labelBottom, &lbl_eu_80664530);
        __as__11_GXColorS10FRC11_GXColorS10(&valueTop, &lbl_eu_80664568);
        __as__11_GXColorS10FRC11_GXColorS10(&valueBottom, &lbl_eu_80664570);
    }
    PaneMatSetTevColors(firstValuePane, &valueTop, &valueBottom);
    PaneMatSetTevColors(firstLabelPane, &labelTop, &labelBottom);
    PaneMatSetTevColors(secondValuePane, &valueTop, &valueBottom);
    PaneMatSetTevColors(secondLabelPane, &labelTop, &labelBottom);
}

void copyItemBoxEntry(CItemBoxInfoEntry*, const CItemBoxInfoEntry*);
void setItemBoxEntry(CItemBoxInfoEntry*, u16, u32, u8);
extern const u32 lbl_eu_80506368[6];

// Named views keep the item-stat calculations readable while leaving the
// backing entry as a plain 13-word record, which is how MWCC copies it.
#define w04 words[1]
#define _0C words[3]
#define _10 words[4]
#define w14 halfWords[10]

#pragma push
#pragma optimize_for_size on
// Prefer labeled SDA symbols over float literals / parent+offset member decays
// so bar formulas emit retail lfs relocs (0.01f/100.0f).
__declspec(noinline) void CItemBoxInfo::SetupDetail(u32 arg2, void* arg3, u32 arg4) {
    // Packed selection: retail emits slot (rlwinm.->r26/CR0), then member
    // (r18), then type (r27). Declaring member before type matches that
    // emission order; type stays longest-lived for r27.
    u8 memberRaw = (u8)((arg2 >> 8) & 0xFF);
    u8 type = (u8)((arg2 >> 4) & 0xF);
    u8 slot = (u8)(arg2 & 0xF);
    if (slot == 0) {
        void* selectedItem = arg3 != NULL ? arg3 : NULL;
        if (selectedItem != NULL) {
            type = (u8)((*(u32*)selectedItem >> 16) & 0xF);
        }
    }

    // ---- high-frame band: party + lists (retail sp+2352/2400/2496) ----
    // D8EHighFrame places party@2352. Member assign outlines __as__ in this
    // 26KB body (scratch small TUs inline mtctr/lwzu; inline_max does not help).
    // Explicit dual-word loop ~7% exact; CBattleState dest-4/stwu reorders
    // loads (lwz8 before lwz4) and drops exact to ~3%.
    D8EHighFrame highFrame;
    {
        u32* src = (u32*)((u8*)CtrlObjectParam_GetSlotTableBase() + 4);
        u32* dst = highFrame.party.w;
        u32 n = 6;
        do {
            u32 a = *src++;
            u32 b = *src++;
            *dst++ = a;
            *dst++ = b;
        } while (--n);
    }
    for (u32 row = 0; row < 2; row++) {
        for (u8 col = 0; col < 3; col++) {
            u8 id = (u8)highFrame.party.w[col];
            if (id != 0) {
                void* actor = findObjB28ById(id);
                if (actor != NULL) {
                    ((cf::CActorParam*)actor)->CActorParam_resetArtsStatus(NULL);
                }
            }
        }
    }

    // ---- character setup ----
    // Two (u8) casts from the raw return -> retail's paired rlwinm (save + arg).
    u32 memberTmp = GetCollectedFlagByte(memberRaw);
    u8 member = (u8)memberTmp;
    void* charObj = CtrlObjectParam_GetCharEntry((u8)memberTmp);
    if (findObjB28ById(member) == NULL) {
        CtrlObjectParam_SyncParamFromActorEx(charObj, 1);
    }
    cf::CActorParam* stats = (cf::CActorParam*)((u8*)charObj + 0x17C);

    ml::FixStr<32> textBuffer(false);
    ml::FixStr<32> textBufferB(false);

    // ---- HP values (clamped to 9999) ----
    // String-pool base before HP so it takes r14 (retail) and HP lands in r18/r19.
    char* base = lbl_eu_805063BC;
    // Both virtuals before either clamp -- retail interleaves the two
    // fctiwz conversions around the second vcall, not the first clamp.
    s32 hp1 = (s32)stats->CActorParam_getHp();
    s32 hp2 = (s32)stats->CActorParam_getDamageScale();
    if (hp1 > 9999) hp1 = 9999;
    if (hp2 > 9999) hp2 = 9999;

    // ---- name / pane text ----
    // Retail: BdatGetPtrDirect(lbl_eu_80664090, base+0x139, member) -- 3-arg.
    // lbl_eu_805063BC is an UNDEF extern (blob is +0x54 inside packed rodata)
    // so immediates stay plain +0x139, not parent+0x54+imm.
    LayoutSetTextBoxFmtValue(mpLayout, &base[0x4D7],
        ((char*(*)(void*, char*, u32))&BdatGetPtrDirect)(lbl_eu_80664090, &base[0x139], member), 0);
    setLayoutTextBoxNumber(mpLayout, &base[0x4E3], (u8)stats->CActorParam_getActorLevel());
    setLayoutTextBoxNumber(mpLayout, &base[0x4F0], (u8)stats->CActorParam_getTotalCurrency());
    LayoutSetTextBoxInt((nw4r::lyt::Layout*)mSlotPane0, hp1);
    LayoutSetTextBoxInt((nw4r::lyt::Layout*)mSlotPane2, hp2);
    SetLayoutSelfText((nw4r::lyt::Layout*)mSlotPane3, BdatTouchStringCell(&base[0x130], &base[0x139], 0x82), 0);

    // ---- stat sub-objects ----
    D8EStatA* stA = (D8EStatA*)stats->CActorParam_getBaseStats();
    D8EStatB* stB = (D8EStatB*)stats->CActorParam_getArtsDataBlock();
    D8EStatC* stC = (D8EStatC*)stats->CActorParam_getBonusStats();
    CtrlObjectParam_GetArtsStatsRow((u8*)charObj + 0x1C, 5);

    // ---- arts sum ----
    s32 artsSum = (s32)SumTypeCounts(member, NULL);
    u32 w = GetCondStat((void*)(u32)member, 0x97);
    if (w != 0) {
        artsSum = artsSum - (s32)w;
        if (artsSum < 0) artsSum = 0;
    }

    // ---- stat bars (display formula) ----
    s32 effect = GetEffect(member, 0x21, NULL);
    // volatile keeps early bars in distinct slots (non-volatile CSE'd ~0x10C
    // of body away under optimize_for_size). Float literals here match the
    // ~7% HighFrame baseline; SDA 68040/44 names elsewhere still open.
    volatile s16 bar1 = (s16)(s32)(0.01f * ((100.0f + (f32)stC->s10) * (f32)(stA->s20 + effect)));
    effect = GetEffect(member, 0x1, NULL);
    volatile s16 bar2 = (s16)(s32)(0.01f * ((100.0f + (f32)stC->s0C) * (f32)(stA->s1C + effect)));
    effect = GetEffect(member, 0x31, NULL);
    s16 bar3 = (s16)(s32)(0.01f * ((100.0f + (f32)stC->s1C) * (f32)(stA->s32 + effect)));
    effect = GetEffect(member, 0x41, NULL);
    volatile s16 bar4 = (s16)(s32)(0.01f * ((100.0f + (f32)stC->s0E) * (f32)(stA->s1E - (s16)artsSum + effect)));
    effect = GetEffect(member, 0x51, NULL);
    s16 bar5 = (s16)(s32)(0.01f * ((100.0f + (f32)stC->s18) * (f32)(stA->s2E + effect)));
    effect = GetEffect(member, 0x54, NULL);
    s16 bar6 = (s16)(s32)(0.01f * ((100.0f + (f32)stC->s22) * (f32)(stA->s38 + effect)));
    effect = GetEffect(member, 0x11, NULL);
    s16 hpStat = (s16)(s32)(stA->f10 + (f32)RoundHalfAway0(
        0.01f * stB->f10 * (f32)(stC->s06 + effect)));
    if (hpStat > 9999) hpStat = 9999;

    // ---- equip-mode gate: recompute bars with the equipped stat (0x30) ----
    if (CheckGemChain(member, NULL, NULL)) {
        s32 eq = GetCondStat((void*)(u32)member, 0x30);
        if (eq != 0) {
            s16 barA = (s16)(s32)(0.01f * ((100.0f + (f32)stC->s10 + (f32)eq) * (f32)(stA->s20 + GetEffect(member, 0x21, NULL))));
            s16 barB = (s16)(s32)(0.01f * ((100.0f + (f32)stC->s0C + (f32)barA) * (f32)(stA->s1C + GetEffect(member, 0x1, NULL))));
            s16 barC = (s16)(s32)(0.01f * ((100.0f + (f32)stC->s0E + (f32)eq) * (f32)(stA->s1E + GetEffect(member, 0x41, NULL) - artsSum)));
            hpStat = (s16)(s32)(stA->f10 + (f32)RoundHalfAway0(
                0.01f * stB->f10 *
                (f32)(stC->s06 + eq + GetEffect(member, 0x11, NULL))));

            // ---- current weapon block ----
            void* item2 = CItem_kindPtr(2, *(s16*)((u8*)charObj + 0x26));
            if (item2 != NULL && *(u32*)item2 != 0) {
                D8EEntry e1;
                e1.rec = BuildCmpRecord((void*)(u32)((u16)((*(u32*)item2 >> 20) & 0xFFFF)));
                D8EEntry e2 = e1;
                s32 s1 = CalcBonus1(member, (u16)e2.w04, NULL);
                s32 s2 = CalcBonus2(member, (u16)(e2.w04 >> 16), NULL);
                s32 atk1 = (s32)barB + s1;
                s32 atk2 = (s32)barB + s2;
                if (atk1 > atk2) atk1 = atk2;
                char* rangeSeparator = BdatTouchStringCell(
                    &lbl_eu_805063BC[0x130], &lbl_eu_805063BC[0x139], 0xB);
                textBuffer.format(&lbl_eu_805063BC[0x254], atk1, rangeSeparator, atk2);
                SetLayoutSelfText((nw4r::lyt::Layout*)mSlotPane10, (const char*)textBuffer.mString, 0);
            }

            // ---- numbers ----
            if (barC <= 0) barC = 1;
            LayoutSetTextBoxInt((nw4r::lyt::Layout*)mSlotPane4, (s16)barB);
            LayoutSetTextBoxInt((nw4r::lyt::Layout*)mSlotPane6, (s16)barC);
            LayoutSetTextBoxInt((nw4r::lyt::Layout*)mSlotPane8, (s16)barA);
            LayoutSetTextBoxInt((nw4r::lyt::Layout*)mSlotPane12, (s16)bar5);
            LayoutSetTextBoxInt((nw4r::lyt::Layout*)mSlotPane14, (s16)bar3);
            LayoutSetTextBoxInt((nw4r::lyt::Layout*)mSlotPane2, hpStat);
            char* percentSuffix = BdatTouchStringCell(
                &lbl_eu_805063BC[0x130], &lbl_eu_805063BC[0x139], 0x80);
            textBuffer.format(&lbl_eu_805063BC[0x13E], (s16)bar6, percentSuffix);
            textBufferB.format(&lbl_eu_805063BC[0x13E], (s16)(stA->b55), percentSuffix);
            SetLayoutSelfText((nw4r::lyt::Layout*)mSlotPane16, (const char*)textBuffer.mString, 0);
            SetLayoutSelfText((nw4r::lyt::Layout*)mSlotPane18, (const char*)textBufferB.mString, 0);

            // ---- 20x color application ----
            D8EQuad q1 = *(D8EQuad*)&lbl_eu_80664518;
            D8EQuad q2 = *(D8EQuad*)&lbl_eu_80664520;
            D8EQuad q3 = *(D8EQuad*)&lbl_eu_80664558;
            D8EQuad q4 = *(D8EQuad*)&lbl_eu_80664560;
            PaneMatSetTevColors(mSlotPane0, &q3, &q4);
            PaneMatSetTevColors(mSlotPane2, &q3, &q4);
            PaneMatSetTevColors(mSlotPane3, &q3, &q4);
            PaneMatSetTevColors(mSlotPane1, &q1, &q2);
            PaneMatSetTevColors(mSlotPane4, &q3, &q4);
            PaneMatSetTevColors(mSlotPane5, &q1, &q2);
            PaneMatSetTevColors(mSlotPane6, &q3, &q4);
            PaneMatSetTevColors(mSlotPane7, &q1, &q2);
            PaneMatSetTevColors(mSlotPane8, &q3, &q4);
            PaneMatSetTevColors(mSlotPane9, &q1, &q2);
            PaneMatSetTevColors(mSlotPane10, &q3, &q4);
            PaneMatSetTevColors(mSlotPane11, &q1, &q2);
            PaneMatSetTevColors(mSlotPane12, &q3, &q4);
            PaneMatSetTevColors(mSlotPane13, &q1, &q2);
            PaneMatSetTevColors(mSlotPane14, &q3, &q4);
            PaneMatSetTevColors(mSlotPane15, &q1, &q2);
            PaneMatSetTevColors(mSlotPane16, &q3, &q4);
            PaneMatSetTevColors(mSlotPane17, &q1, &q2);
            PaneMatSetTevColors(mSlotPane18, &q3, &q4);
            PaneMatSetTevColors(mSlotPane19, &q1, &q2);
        }
    }

    // ---- stat panel (arg4 != 0, type in {2,4,5,6,7,8}) ----
    if (arg4 != 0 && (type == 2 || (type >= 4 && type <= 8))) {
        s32 e1 = 0;
        if (CheckGemChain(member, NULL, NULL)) e1 = GetCondStat((void*)(u32)member, 0x30);
        s32 e2 = 0;
        if (CheckGemChain(member, arg3, NULL)) e2 = GetCondStat((void*)(u32)member, 0x30);

        // row A: 0x21 -> panes 0x60/0x64
        s16 dB = 0;
        {
            s16 pb = (s16)(s32)(0.01f * ((100.0f + (f32)stC->s10 + (f32)e1) * (f32)(stA->s20 + GetEffect(member, 0x21, NULL))));
            s16 nb = (s16)(s32)(0.01f * ((100.0f + (f32)stC->s10 + (f32)e2) * (f32)(stA->s20 + GetEffect(member, 0x21, arg3))));
            s16 d = (s16)(nb - pb);
            LayoutSetTextBoxInt((nw4r::lyt::Layout*)mSlotPane8, nb);
            D8EQuad q1 = *(D8EQuad*)&lbl_eu_80664518;
            D8EQuad q2 = *(D8EQuad*)&lbl_eu_80664520;
            D8EQuad q3 = *(D8EQuad*)&lbl_eu_80664558;
            D8EQuad q4 = *(D8EQuad*)&lbl_eu_80664560;
            if (d < 0) {
                __as__11_GXColorS10FRC11_GXColorS10(&q1, &lbl_eu_80664538);
                __as__11_GXColorS10FRC11_GXColorS10(&q2, &lbl_eu_80664540);
                __as__11_GXColorS10FRC11_GXColorS10(&q3, &lbl_eu_80664578);
                __as__11_GXColorS10FRC11_GXColorS10(&q4, &lbl_eu_80664580);
            } else if (d > 0) {
                __as__11_GXColorS10FRC11_GXColorS10(&q1, &lbl_eu_80664528);
                __as__11_GXColorS10FRC11_GXColorS10(&q2, &lbl_eu_80664530);
                __as__11_GXColorS10FRC11_GXColorS10(&q3, &lbl_eu_80664568);
                __as__11_GXColorS10FRC11_GXColorS10(&q4, &lbl_eu_80664570);
            }
            PaneMatSetTevColors(mSlotPane8, &q3, &q4);
            PaneMatSetTevColors(mSlotPane9, &q1, &q2);
        }

        // row B: 0x1 -> panes 0x50/0x54
        {
            s16 pb = (s16)(s32)(0.01f * ((100.0f + (f32)stC->s0C + (f32)e1) * (f32)(stA->s1C + GetEffect(member, 0x1, NULL))));
            s16 nb = (s16)(s32)(0.01f * ((100.0f + (f32)stC->s0C + (f32)e2) * (f32)(stA->s1C + GetEffect(member, 0x1, arg3))));
            dB = (s16)(nb - pb);
            LayoutSetTextBoxInt((nw4r::lyt::Layout*)mSlotPane4, nb);
            D8EQuad q1 = *(D8EQuad*)&lbl_eu_80664518;
            D8EQuad q2 = *(D8EQuad*)&lbl_eu_80664520;
            D8EQuad q3 = *(D8EQuad*)&lbl_eu_80664558;
            D8EQuad q4 = *(D8EQuad*)&lbl_eu_80664560;
            if (dB < 0) {
                __as__11_GXColorS10FRC11_GXColorS10(&q1, &lbl_eu_80664538);
                __as__11_GXColorS10FRC11_GXColorS10(&q2, &lbl_eu_80664540);
                __as__11_GXColorS10FRC11_GXColorS10(&q3, &lbl_eu_80664578);
                __as__11_GXColorS10FRC11_GXColorS10(&q4, &lbl_eu_80664580);
            } else if (dB > 0) {
                __as__11_GXColorS10FRC11_GXColorS10(&q1, &lbl_eu_80664528);
                __as__11_GXColorS10FRC11_GXColorS10(&q2, &lbl_eu_80664530);
                __as__11_GXColorS10FRC11_GXColorS10(&q3, &lbl_eu_80664568);
                __as__11_GXColorS10FRC11_GXColorS10(&q4, &lbl_eu_80664570);
            }
            PaneMatSetTevColors(mSlotPane4, &q3, &q4);
            PaneMatSetTevColors(mSlotPane5, &q1, &q2);
        }

        s32 comparisonArtsSum = (s32)SumTypeCounts(member, NULL);
        u32 comparisonWeight = GetCondStat((void*)(u32)member, 0x97);
        if (comparisonWeight != 0) {
            comparisonArtsSum -= (s32)comparisonWeight;
            if (comparisonArtsSum < 0) comparisonArtsSum = 0;
        }

        // row C: 0x41 -> panes 0x58/0x5C
        {
            s16 oldAgilityBase = (s16)(stA->s1E - (s16)comparisonArtsSum);
            if (oldAgilityBase <= 0) oldAgilityBase = 1;
            s16 pb = (s16)(s32)(0.01f * ((100.0f + (f32)stC->s0E + (f32)e1) *
                (f32)(oldAgilityBase + GetEffect(member, 0x41, NULL))));
            s16 nb = (s16)(s32)(0.01f * ((100.0f + (f32)stC->s0E + (f32)e2) *
                (f32)(oldAgilityBase + GetEffect(member, 0x41, arg3))));
            s16 dC = (s16)(nb - pb);
            LayoutSetTextBoxInt((nw4r::lyt::Layout*)mSlotPane6, nb);
            D8EQuad q1 = *(D8EQuad*)&lbl_eu_80664518;
            D8EQuad q2 = *(D8EQuad*)&lbl_eu_80664520;
            D8EQuad q3 = *(D8EQuad*)&lbl_eu_80664558;
            D8EQuad q4 = *(D8EQuad*)&lbl_eu_80664560;
            if (dC < 0) {
                __as__11_GXColorS10FRC11_GXColorS10(&q1, &lbl_eu_80664538);
                __as__11_GXColorS10FRC11_GXColorS10(&q2, &lbl_eu_80664540);
                __as__11_GXColorS10FRC11_GXColorS10(&q3, &lbl_eu_80664578);
                __as__11_GXColorS10FRC11_GXColorS10(&q4, &lbl_eu_80664580);
            } else if (dC > 0) {
                __as__11_GXColorS10FRC11_GXColorS10(&q1, &lbl_eu_80664528);
                __as__11_GXColorS10FRC11_GXColorS10(&q2, &lbl_eu_80664530);
                __as__11_GXColorS10FRC11_GXColorS10(&q3, &lbl_eu_80664568);
                __as__11_GXColorS10FRC11_GXColorS10(&q4, &lbl_eu_80664570);
            }
            PaneMatSetTevColors(mSlotPane6, &q3, &q4);
            PaneMatSetTevColors(mSlotPane7, &q1, &q2);
        }

        // row D: 0x11 + RoundHalfAway0 -> panes 0x40/0x48
        {
            s16 pb = (s16)(s32)(stA->f10 + (f32)RoundHalfAway0(0.01f * stB->f10 * (f32)(stC->s06 + e1 + GetEffect(member, 0x11, NULL))));
            s16 nb = (s16)(s32)(stA->f10 + (f32)RoundHalfAway0(0.01f * stB->f10 * (f32)(stC->s06 + e2 + GetEffect(member, 0x11, arg3))));
            if (pb > 9999) pb = 9999;
            if (nb > 9999) nb = 9999;
            s16 dD = (s16)(nb - pb);
            if (stats->CActorParam_getHp() > (f32)nb) {
                LayoutSetTextBoxInt((nw4r::lyt::Layout*)mSlotPane0, nb);
            }
            LayoutSetTextBoxInt((nw4r::lyt::Layout*)mSlotPane2, nb);
            D8EQuad q1 = *(D8EQuad*)&lbl_eu_80664518;
            D8EQuad q2 = *(D8EQuad*)&lbl_eu_80664520;
            D8EQuad q3 = *(D8EQuad*)&lbl_eu_80664558;
            D8EQuad q4 = *(D8EQuad*)&lbl_eu_80664560;
            if (dD < 0) {
                __as__11_GXColorS10FRC11_GXColorS10(&q1, &lbl_eu_80664538);
                __as__11_GXColorS10FRC11_GXColorS10(&q2, &lbl_eu_80664540);
                __as__11_GXColorS10FRC11_GXColorS10(&q3, &lbl_eu_80664578);
                __as__11_GXColorS10FRC11_GXColorS10(&q4, &lbl_eu_80664580);
            } else if (dD > 0) {
                __as__11_GXColorS10FRC11_GXColorS10(&q1, &lbl_eu_80664528);
                __as__11_GXColorS10FRC11_GXColorS10(&q2, &lbl_eu_80664530);
                __as__11_GXColorS10FRC11_GXColorS10(&q3, &lbl_eu_80664568);
                __as__11_GXColorS10FRC11_GXColorS10(&q4, &lbl_eu_80664570);
            }
            PaneMatSetTevColors(mSlotPane0, &q3, &q4);
            PaneMatSetTevColors(mSlotPane2, &q3, &q4);
            PaneMatSetTevColors(mSlotPane3, &q3, &q4);
            PaneMatSetTevColors(mSlotPane1, &q1, &q2);
        }
        // ---- type switch: slot id + item ----
        s16 slotId = 0;
        // (was switch: retail compiles this dispatch as compares)
        if (type == 2) slotId = *(s16*)((u8*)charObj + 0x26);
        else if (type == 4) slotId = *(s16*)((u8*)charObj + 0x1C);
        else if (type == 5) slotId = *(s16*)((u8*)charObj + 0x1E);
        else if (type == 6) slotId = *(s16*)((u8*)charObj + 0x20);
        else if (type == 7) slotId = *(s16*)((u8*)charObj + 0x22);
        else if (type == 8) slotId = *(s16*)((u8*)charObj + 0x24);
        void* item = CItem_kindPtr(type, slotId);
        D8EComparisonStorage comparisonStorage;
        if (type == 2) {
            // ---- weapon block (0x801E7300) ----
            // Member assign -> li r0,6 + trailing word. Open: retail e_cur@1916 /
            // c_cur@2296 (380B apart), not adjacent array slots.
            u16 w0 = (item != NULL && *(u32*)item != 0) ? (u16)(*(u32*)item >> 20) : 0;
            D8EEntry& e_cur = comparisonStorage.weapon[0];
            e_cur.rec = BuildCmpRecord((void*)(u32)w0);
            comparisonStorage.weapon[1] = e_cur;
            D8EEntry& c_cur = comparisonStorage.weapon[1];
            D8EEntry& e_new = comparisonStorage.weapon[2];
            e_new.rec = BuildCmpRecord(arg3);
            comparisonStorage.weapon[3] = e_new;
            D8EEntry& c_new = comparisonStorage.weapon[3];
            s32 eq1 = 0;
            if (CheckGemChain(member, NULL, NULL)) eq1 = GetCondStat((void*)(u32)member, 0x30);
            s32 eq2 = 0;
            if (CheckGemChain(member, arg3, NULL)) eq2 = GetCondStat((void*)(u32)member, 0x30);
            s16 pb = (s16)(s32)(0.01f * ((100.0f + (f32)stC->s0C + (f32)eq1) * (f32)(stA->s1C + GetEffect(member, 0x1, NULL))));
            s16 nb = (s16)(s32)(0.01f * ((100.0f + (f32)stC->s0C + (f32)eq2) * (f32)(stA->s1C + GetEffect(member, 0x1, arg3))));
            s32 atkA = (s32)pb + CalcBonus1(member, (u16)c_cur.w04, NULL);
            s32 atkC = (s32)pb + CalcBonus2(member, (u16)(c_cur.w04 >> 16), NULL);
            s32 atkB = (s32)nb + CalcBonus1(member, (u16)c_new.w04, arg3);
            s32 atkD = (s32)nb + CalcBonus2(member, (u16)(c_new.w04 >> 16), arg3);
            if (atkB < atkD) atkB = atkD;
            s16 dW = (s16)(atkC - atkA);
            s32 c1 = GetEffect(member, 0x31, arg3);
            s16 v612 = (s16)c_new._0C;
            s16 v646 = (s16)c_cur._0C;
            v612 = (s16)(s32)(0.01f * ((100.0f + (f32)stC->s1C) * (f32)((stA->s32 - v646) + (c1 + v612))));
            c1 = GetEffect(member, 0x31, NULL);
            v646 = (s16)(s32)(0.01f * ((100.0f + (f32)stC->s1C) * (f32)(stA->s32 + c1)));
            c1 = GetEffect(member, 0x51, arg3);
            s16 v610 = (s16)c_new._10;
            s16 v644 = (s16)c_cur._10;
            v610 = (s16)(s32)(0.01f * ((100.0f + (f32)stC->s18) * (f32)((stA->s2E - v644) + (c1 + v610))));
            c1 = GetEffect(member, 0x51, NULL);
            v644 = (s16)(s32)(0.01f * ((100.0f + (f32)stC->s18) * (f32)(stA->s2E + c1)));
            c1 = GetEffect(member, 0x54, arg3);
            s16 v614 = c_new.w14;
            s16 v648 = c_cur.w14;
            v614 = (s16)(s32)(0.01f * ((100.0f + (f32)stC->s22) * (f32)((stA->s38 - v648) + (c1 + v614))));
            c1 = GetEffect(member, 0x54, NULL);
            v648 = (s16)(s32)(0.01f * ((100.0f + (f32)stC->s22) * (f32)(stA->s38 + c1)));
            s16 snap644 = v644;
            s16 snap646 = v646;
            s16 d1 = (s16)(v610 - snap644);
            s16 d2 = (s16)(v612 - snap646);
            s16 d3 = (s16)(c_new.w14 - c_cur.w14);
            s16 d4 = (s16)(v614 - v648);
            s16 disp1 = v610;
            s16 disp2 = v612;
            s16 disp3 = v614;
            char* rangeSeparator = BdatTouchStringCell(
                &lbl_eu_805063BC[0x130], &lbl_eu_805063BC[0x139], 0xB);
            textBuffer.format(&lbl_eu_805063BC[0x254], atkD, rangeSeparator, atkC);
            SetLayoutSelfText((nw4r::lyt::Layout*)mSlotPane10, (const char*)textBuffer.mString, 0);
            LayoutSetTextBoxInt((nw4r::lyt::Layout*)mSlotPane12, (s16)disp1);
            LayoutSetTextBoxInt((nw4r::lyt::Layout*)mSlotPane14, (s16)disp2);
            char* percentSuffix2 = BdatTouchStringCell(
                &lbl_eu_805063BC[0x130], &lbl_eu_805063BC[0x139], 0x80);
            textBuffer.format(&lbl_eu_805063BC[0x13E], (s16)disp3, percentSuffix2);
            SetLayoutSelfText((nw4r::lyt::Layout*)mSlotPane16, (const char*)textBuffer.mString, 0);
            char* percentSuffix3 = BdatTouchStringCell(
                &lbl_eu_805063BC[0x130], &lbl_eu_805063BC[0x139], 0x80);
            textBuffer.format(&lbl_eu_805063BC[0x13E],
                           (s32)stA->b55 + (s32)d3, percentSuffix3);
            SetLayoutSelfText((nw4r::lyt::Layout*)mSlotPane18, (const char*)textBuffer.mString, 0);
            // delta colors (5 rows)
            {
                D8EQuad q1 = *(D8EQuad*)&lbl_eu_80664518;
                D8EQuad q2 = *(D8EQuad*)&lbl_eu_80664520;
                D8EQuad q3 = *(D8EQuad*)&lbl_eu_80664558;
                D8EQuad q4 = *(D8EQuad*)&lbl_eu_80664560;
                if (dW < 0) {
                    __as__11_GXColorS10FRC11_GXColorS10(&q1, &lbl_eu_80664538);
                    __as__11_GXColorS10FRC11_GXColorS10(&q2, &lbl_eu_80664540);
                    __as__11_GXColorS10FRC11_GXColorS10(&q3, &lbl_eu_80664578);
                    __as__11_GXColorS10FRC11_GXColorS10(&q4, &lbl_eu_80664580);
                } else if (dW > 0) {
                    __as__11_GXColorS10FRC11_GXColorS10(&q1, &lbl_eu_80664528);
                    __as__11_GXColorS10FRC11_GXColorS10(&q2, &lbl_eu_80664530);
                    __as__11_GXColorS10FRC11_GXColorS10(&q3, &lbl_eu_80664568);
                    __as__11_GXColorS10FRC11_GXColorS10(&q4, &lbl_eu_80664570);
                }
                PaneMatSetTevColors(mSlotPane10, &q3, &q4);
                PaneMatSetTevColors(mSlotPane11, &q1, &q2);
            }
            {
                D8EQuad q1 = *(D8EQuad*)&lbl_eu_80664518;
                D8EQuad q2 = *(D8EQuad*)&lbl_eu_80664520;
                D8EQuad q3 = *(D8EQuad*)&lbl_eu_80664558;
                D8EQuad q4 = *(D8EQuad*)&lbl_eu_80664560;
                if (d1 < 0) {
                    __as__11_GXColorS10FRC11_GXColorS10(&q1, &lbl_eu_80664538);
                    __as__11_GXColorS10FRC11_GXColorS10(&q2, &lbl_eu_80664540);
                    __as__11_GXColorS10FRC11_GXColorS10(&q3, &lbl_eu_80664578);
                    __as__11_GXColorS10FRC11_GXColorS10(&q4, &lbl_eu_80664580);
                } else if (d1 > 0) {
                    __as__11_GXColorS10FRC11_GXColorS10(&q1, &lbl_eu_80664528);
                    __as__11_GXColorS10FRC11_GXColorS10(&q2, &lbl_eu_80664530);
                    __as__11_GXColorS10FRC11_GXColorS10(&q3, &lbl_eu_80664568);
                    __as__11_GXColorS10FRC11_GXColorS10(&q4, &lbl_eu_80664570);
                }
                PaneMatSetTevColors(mSlotPane12, &q3, &q4);
                PaneMatSetTevColors(mSlotPane13, &q1, &q2);
            }
            {
                D8EQuad q1 = *(D8EQuad*)&lbl_eu_80664518;
                D8EQuad q2 = *(D8EQuad*)&lbl_eu_80664520;
                D8EQuad q3 = *(D8EQuad*)&lbl_eu_80664558;
                D8EQuad q4 = *(D8EQuad*)&lbl_eu_80664560;
                if (d2 < 0) {
                    __as__11_GXColorS10FRC11_GXColorS10(&q1, &lbl_eu_80664538);
                    __as__11_GXColorS10FRC11_GXColorS10(&q2, &lbl_eu_80664540);
                    __as__11_GXColorS10FRC11_GXColorS10(&q3, &lbl_eu_80664578);
                    __as__11_GXColorS10FRC11_GXColorS10(&q4, &lbl_eu_80664580);
                } else if (d2 > 0) {
                    __as__11_GXColorS10FRC11_GXColorS10(&q1, &lbl_eu_80664528);
                    __as__11_GXColorS10FRC11_GXColorS10(&q2, &lbl_eu_80664530);
                    __as__11_GXColorS10FRC11_GXColorS10(&q3, &lbl_eu_80664568);
                    __as__11_GXColorS10FRC11_GXColorS10(&q4, &lbl_eu_80664570);
                }
                PaneMatSetTevColors(mSlotPane14, &q3, &q4);
                PaneMatSetTevColors(mSlotPane15, &q1, &q2);
            }
            {
                D8EQuad q1 = *(D8EQuad*)&lbl_eu_80664518;
                D8EQuad q2 = *(D8EQuad*)&lbl_eu_80664520;
                D8EQuad q3 = *(D8EQuad*)&lbl_eu_80664558;
                D8EQuad q4 = *(D8EQuad*)&lbl_eu_80664560;
                if (d4 < 0) {
                    __as__11_GXColorS10FRC11_GXColorS10(&q1, &lbl_eu_80664538);
                    __as__11_GXColorS10FRC11_GXColorS10(&q2, &lbl_eu_80664540);
                    __as__11_GXColorS10FRC11_GXColorS10(&q3, &lbl_eu_80664578);
                    __as__11_GXColorS10FRC11_GXColorS10(&q4, &lbl_eu_80664580);
                } else if (d4 > 0) {
                    __as__11_GXColorS10FRC11_GXColorS10(&q1, &lbl_eu_80664528);
                    __as__11_GXColorS10FRC11_GXColorS10(&q2, &lbl_eu_80664530);
                    __as__11_GXColorS10FRC11_GXColorS10(&q3, &lbl_eu_80664568);
                    __as__11_GXColorS10FRC11_GXColorS10(&q4, &lbl_eu_80664570);
                }
                PaneMatSetTevColors(mSlotPane16, &q3, &q4);
                PaneMatSetTevColors(mSlotPane17, &q1, &q2);
            }
            {
                D8EQuad q1 = *(D8EQuad*)&lbl_eu_80664518;
                D8EQuad q2 = *(D8EQuad*)&lbl_eu_80664520;
                D8EQuad q3 = *(D8EQuad*)&lbl_eu_80664558;
                D8EQuad q4 = *(D8EQuad*)&lbl_eu_80664560;
                if (d3 < 0) {
                    __as__11_GXColorS10FRC11_GXColorS10(&q1, &lbl_eu_80664538);
                    __as__11_GXColorS10FRC11_GXColorS10(&q2, &lbl_eu_80664540);
                    __as__11_GXColorS10FRC11_GXColorS10(&q3, &lbl_eu_80664578);
                    __as__11_GXColorS10FRC11_GXColorS10(&q4, &lbl_eu_80664580);
                } else if (d3 > 0) {
                    __as__11_GXColorS10FRC11_GXColorS10(&q1, &lbl_eu_80664528);
                    __as__11_GXColorS10FRC11_GXColorS10(&q2, &lbl_eu_80664530);
                    __as__11_GXColorS10FRC11_GXColorS10(&q3, &lbl_eu_80664568);
                    __as__11_GXColorS10FRC11_GXColorS10(&q4, &lbl_eu_80664570);
                }
                PaneMatSetTevColors(mSlotPane18, &q3, &q4);
                PaneMatSetTevColors(mSlotPane19, &q1, &q2);
            }
        } else {
            // ---- armor block (0x801E7DA0) ----
            u16 w0 = (item != NULL && *(u32*)item != 0) ? (u16)(*(u32*)item >> 20) : 0;
            D8EArmorEntry& e_cur = comparisonStorage.armor[0];
            const CItemBoxSlotFlags& e_curFlags = BuildSlotFlags(w0);
            e_cur = *(const D8EArmorEntry*)&e_curFlags;
            comparisonStorage.armor[1] = e_cur;
            D8EArmorEntry& c_cur = comparisonStorage.armor[1];
            D8EArmorEntry& e_new = comparisonStorage.armor[2];
            const CItemBoxSlotFlags& e_newFlags = BuildSlotFlags((u32)arg3);
            e_new = *(const D8EArmorEntry*)&e_newFlags;
            comparisonStorage.armor[3] = e_new;
            D8EArmorEntry& c_new = comparisonStorage.armor[3];
            s16 v484 = (s16)SumTypeCounts(member, (item != NULL && *(u32*)item != 0)
                    ? (void*)(u32)(u16)(*(u32*)item >> 20)
                    : NULL);
            s16 v468 = (s16)SumTypeCounts(member, arg3);
            s32 eq1 = 0;
            if (CheckGemChain(member, NULL, NULL)) eq1 = GetCondStat((void*)(u32)member, 0x30);
            s32 eq2 = 0;
            if (CheckGemChain(member, arg3, NULL)) eq2 = GetCondStat((void*)(u32)member, 0x30);
            // arm-type scaling of the entry value pairs
            u8 arm1 = BdatGetU8Direct(
                (const void*)lbl_eu_806640F8, &lbl_eu_805063BC[0x1EB],
                BdatGetItemId((u32)((item != NULL && *(u32*)item != 0)
                    ? (u16)(*(u32*)item >> 20)
                    : 0)));
            {
                    switch (arm1) {
                        case 3: {
                            s32 effect_3 = GetCondStat((void*)(u32)member, 0x0D);
                            if (effect_3 != 0) {
                                f32 scale_3 = 0.01f * (100.0f + (f32)effect_3);
                                c_cur.physicalDefense = (s16)(s32)((f32)c_cur.physicalDefense * scale_3);
                                c_cur.etherDefense = (s16)(s32)((f32)c_cur.etherDefense * scale_3);
                            }
                            s32 effect_24 = GetCondStat((void*)(u32)member, 0x24);
                            if (effect_24 != 0) {
                                c_cur.etherDefense = (s16)(c_cur.etherDefense + (s16)effect_24);
                            }
                            break;
                        }
                        case 2: {
                            s32 effect_2 = GetCondStat((void*)(u32)member, 0x0C);
                            if (effect_2 != 0) {
                                f32 scale_2 = 0.01f * (100.0f + (f32)effect_2);
                                c_cur.physicalDefense = (s16)(s32)((f32)c_cur.physicalDefense * scale_2);
                                c_cur.etherDefense = (s16)(s32)((f32)c_cur.etherDefense * scale_2);
                            }
                            break;
                        }
                        case 1: {
                            s32 effect_1 = GetCondStat((void*)(u32)member, 0x0B);
                            if (effect_1 != 0) {
                                f32 scale_1 = 0.01f * (100.0f + (f32)effect_1);
                                c_cur.physicalDefense = (s16)(s32)((f32)c_cur.physicalDefense * scale_1);
                                c_cur.etherDefense = (s16)(s32)((f32)c_cur.etherDefense * scale_1);
                            }
                            break;
                        }
                    }
                }
            u8 arm2 = BdatGetU8Direct((const void*)lbl_eu_806640F8, &lbl_eu_805063BC[0x1EB], BdatGetItemId((u32)(*(u32*)arg3 >> 20)));
            {
                    switch (arm2) {
                        case 3: {
                            s32 effect_3 = GetCondStat((void*)(u32)member, 0x0D);
                            if (effect_3 != 0) {
                                f32 scale_3 = 0.01f * (100.0f + (f32)effect_3);
                                c_new.physicalDefense = (s16)(s32)((f32)c_new.physicalDefense * scale_3);
                                c_new.etherDefense = (s16)(s32)((f32)c_new.etherDefense * scale_3);
                            }
                            s32 effect_24 = GetCondStat((void*)(u32)member, 0x24);
                            if (effect_24 != 0) {
                                c_new.etherDefense = (s16)(c_new.etherDefense + (s16)effect_24);
                            }
                            break;
                        }
                        case 2: {
                            s32 effect_2 = GetCondStat((void*)(u32)member, 0x0C);
                            if (effect_2 != 0) {
                                f32 scale_2 = 0.01f * (100.0f + (f32)effect_2);
                                c_new.physicalDefense = (s16)(s32)((f32)c_new.physicalDefense * scale_2);
                                c_new.etherDefense = (s16)(s32)((f32)c_new.etherDefense * scale_2);
                            }
                            break;
                        }
                        case 1: {
                            s32 effect_1 = GetCondStat((void*)(u32)member, 0x0B);
                            if (effect_1 != 0) {
                                f32 scale_1 = 0.01f * (100.0f + (f32)effect_1);
                                c_new.physicalDefense = (s16)(s32)((f32)c_new.physicalDefense * scale_1);
                                c_new.etherDefense = (s16)(s32)((f32)c_new.etherDefense * scale_1);
                            }
                            break;
                        }
                    }
                }
            // weight
            s32 wv = GetCondStat((void*)(u32)member, 0x97);
            if (wv != 0) {
                v484 = (s16)(v484 - (s16)wv);
                v468 = (s16)(v468 - (s16)wv);
                if (v484 < 0) v484 = 0;
                if (v468 < 0) v468 = 0;
            }
            // slot sum
            s32 ssum = (s32)*(s16*)((u8*)charObj + 0x22) + *(s16*)((u8*)charObj + 0x20) + *(s16*)((u8*)charObj + 0x24) + *(s16*)((u8*)charObj + 0x1C) + *(s16*)((u8*)charObj + 0x1E);
            if (ssum <= -5) {
                GetCondStat((void*)(u32)member, 0x1A);
            }
            // Recalculate the two armor values in candidate/current pairs.  Keeping
            // each result adjacent to its query mirrors the original UI pipeline
            // and makes the current-value subtraction explicit.
            s32 c1 = GetEffect(member, 0x31, arg3);
            c_new.etherDefense = (s16)(s32)(0.01f * ((100.0f + (f32)stC->s1C) *
                (f32)((stA->s32 - c_cur.etherDefense) + (c1 + c_new.etherDefense))));
            c1 = GetEffect(member, 0x31, NULL);
            c_cur.etherDefense = (s16)(s32)(0.01f * ((100.0f + (f32)stC->s1C) *
                (f32)(stA->s32 + c1)));
            c1 = GetEffect(member, 0x51, arg3);
            c_new.physicalDefense = (s16)(s32)(0.01f * ((100.0f + (f32)stC->s18) *
                (f32)((stA->s2E - c_cur.physicalDefense) +
                      (c1 + c_new.physicalDefense))));
            c1 = GetEffect(member, 0x51, NULL);
            c_cur.physicalDefense = (s16)(s32)(0.01f * ((100.0f + (f32)stC->s18) *
                (f32)(stA->s2E + c1)));
            s16 d1;
            s16 d2;
            s16 attackDelta = 0;
            s16 percentDelta;
            s16 agilityDelta;
            s16 b14;
            // embedded weapon block
            void* item2 = CItem_kindPtr(2, *(s16*)((u8*)charObj + 0x26));
            if (item2 != NULL && *(u32*)item2 != 0) {
                D8EEntry previewWeapon;
                previewWeapon.rec = BuildCmpRecord((void*)(u32)(u16)(*(u32*)item2 >> 20));
                s16 newPercent = (s16)(s32)(0.01f * ((100.0f + (f32)stC->s22) *
                    (f32)(stA->s38 + GetEffect(member, 0x54, arg3))));
                s16 oldPercent = (s16)(s32)(0.01f * ((100.0f + (f32)stC->s22) *
                    (f32)(stA->s38 + GetEffect(member, 0x54, NULL))));
                percentDelta = (s16)(newPercent - oldPercent);
                char* percentSuffix = BdatTouchStringCell(
                    &lbl_eu_805063BC[0x130], &lbl_eu_805063BC[0x139], 0x80);
                textBuffer.format(&lbl_eu_805063BC[0x13E], newPercent,
                                   percentSuffix);
                SetLayoutSelfText((nw4r::lyt::Layout*)mSlotPane16,
                              (const char*)textBuffer.mString, 0);

                d1 = (s16)(c_new.etherDefense - c_cur.etherDefense);
                d2 = (s16)(c_new.physicalDefense - c_cur.physicalDefense);

                s16 oldAgility = (s16)(s32)(0.01f *
                    ((100.0f + (f32)stC->s0E + (f32)eq1) *
                     (f32)((stA->s1E - (s16)v484) + GetEffect(member, 0x41, NULL))));
                s16 newAgility = (s16)(s32)(0.01f *
                    ((100.0f + (f32)stC->s0E + (f32)eq2) *
                     (f32)((stA->s1E - (s16)v468) + GetEffect(member, 0x41, arg3))));
                if (oldAgility <= 0) oldAgility = 1;
                if (newAgility <= 0) newAgility = 1;
                agilityDelta = (s16)(newAgility - oldAgility);
                b14 = newAgility;

                D8EEntry ew;
                ew.rec = BuildCmpRecord((void*)(u32)(u16)(*(u32*)item2 >> 20));
                D8EEntry cw = ew;
                s32 c3 = GetEffect(member, 0x1, NULL);
                s16 wpb = (s16)(s32)(0.01f * ((100.0f + (f32)stC->s0C + (f32)eq1) * (f32)(stA->s1C + c3)));
                c3 = GetEffect(member, 0x1, arg3);
                s16 wnb = (s16)(s32)(0.01f * ((100.0f + (f32)stC->s0C + (f32)eq2) * (f32)(stA->s1C + c3)));
                s32 wa = (s32)wpb + CalcBonus1(member, (u16)cw.w04, NULL);
                s32 wb = (s32)wnb + CalcBonus2(member, (u16)(cw.w04 >> 16), NULL);
                s32 wc = (s32)wnb + CalcBonus1(member, (u16)cw.w04, arg3);
                s32 wd = (s32)wnb + CalcBonus2(member, (u16)(cw.w04 >> 16), arg3);
                s32 wmin = wc < wd ? wc : wd;
                attackDelta = (s16)(wd - wa);
                char* rangeSeparator = BdatTouchStringCell(
                    &lbl_eu_805063BC[0x130], &lbl_eu_805063BC[0x139], 0xB);
                textBuffer.format(&lbl_eu_805063BC[0x254], wmin, rangeSeparator, wd);
                SetLayoutSelfText((nw4r::lyt::Layout*)mSlotPane10, (const char*)textBuffer.mString, 0);

            }
            // numbers
            LayoutSetTextBoxInt((nw4r::lyt::Layout*)mSlotPane12, c_new.physicalDefense);
            LayoutSetTextBoxInt((nw4r::lyt::Layout*)mSlotPane14, c_new.etherDefense);
            LayoutSetTextBoxInt((nw4r::lyt::Layout*)mSlotPane6, (s16)b14);
            // delta colors (4 rows)
            {
                D8EQuad q1 = *(D8EQuad*)&lbl_eu_80664518;
                D8EQuad q2 = *(D8EQuad*)&lbl_eu_80664520;
                D8EQuad q3 = *(D8EQuad*)&lbl_eu_80664558;
                D8EQuad q4 = *(D8EQuad*)&lbl_eu_80664560;
                if (attackDelta < 0) {
                    __as__11_GXColorS10FRC11_GXColorS10(&q1, &lbl_eu_80664538);
                    __as__11_GXColorS10FRC11_GXColorS10(&q2, &lbl_eu_80664540);
                    __as__11_GXColorS10FRC11_GXColorS10(&q3, &lbl_eu_80664578);
                    __as__11_GXColorS10FRC11_GXColorS10(&q4, &lbl_eu_80664580);
                } else if (attackDelta > 0) {
                    __as__11_GXColorS10FRC11_GXColorS10(&q1, &lbl_eu_80664528);
                    __as__11_GXColorS10FRC11_GXColorS10(&q2, &lbl_eu_80664530);
                    __as__11_GXColorS10FRC11_GXColorS10(&q3, &lbl_eu_80664568);
                    __as__11_GXColorS10FRC11_GXColorS10(&q4, &lbl_eu_80664570);
                }
                PaneMatSetTevColors(mSlotPane10, &q3, &q4);
                PaneMatSetTevColors(mSlotPane11, &q1, &q2);
            }
            {
                D8EQuad q1 = *(D8EQuad*)&lbl_eu_80664518;
                D8EQuad q2 = *(D8EQuad*)&lbl_eu_80664520;
                D8EQuad q3 = *(D8EQuad*)&lbl_eu_80664558;
                D8EQuad q4 = *(D8EQuad*)&lbl_eu_80664560;
                if (d2 < 0) {
                    __as__11_GXColorS10FRC11_GXColorS10(&q1, &lbl_eu_80664538);
                    __as__11_GXColorS10FRC11_GXColorS10(&q2, &lbl_eu_80664540);
                    __as__11_GXColorS10FRC11_GXColorS10(&q3, &lbl_eu_80664578);
                    __as__11_GXColorS10FRC11_GXColorS10(&q4, &lbl_eu_80664580);
                } else if (d2 > 0) {
                    __as__11_GXColorS10FRC11_GXColorS10(&q1, &lbl_eu_80664528);
                    __as__11_GXColorS10FRC11_GXColorS10(&q2, &lbl_eu_80664530);
                    __as__11_GXColorS10FRC11_GXColorS10(&q3, &lbl_eu_80664568);
                    __as__11_GXColorS10FRC11_GXColorS10(&q4, &lbl_eu_80664570);
                }
                PaneMatSetTevColors(mSlotPane12, &q3, &q4);
                PaneMatSetTevColors(mSlotPane13, &q1, &q2);
            }
            {
                D8EQuad q1 = *(D8EQuad*)&lbl_eu_80664518;
                D8EQuad q2 = *(D8EQuad*)&lbl_eu_80664520;
                D8EQuad q3 = *(D8EQuad*)&lbl_eu_80664558;
                D8EQuad q4 = *(D8EQuad*)&lbl_eu_80664560;
                if (d1 < 0) {
                    __as__11_GXColorS10FRC11_GXColorS10(&q1, &lbl_eu_80664538);
                    __as__11_GXColorS10FRC11_GXColorS10(&q2, &lbl_eu_80664540);
                    __as__11_GXColorS10FRC11_GXColorS10(&q3, &lbl_eu_80664578);
                    __as__11_GXColorS10FRC11_GXColorS10(&q4, &lbl_eu_80664580);
                } else if (d1 > 0) {
                    __as__11_GXColorS10FRC11_GXColorS10(&q1, &lbl_eu_80664528);
                    __as__11_GXColorS10FRC11_GXColorS10(&q2, &lbl_eu_80664530);
                    __as__11_GXColorS10FRC11_GXColorS10(&q3, &lbl_eu_80664568);
                    __as__11_GXColorS10FRC11_GXColorS10(&q4, &lbl_eu_80664570);
                }
                PaneMatSetTevColors(mSlotPane14, &q3, &q4);
                PaneMatSetTevColors(mSlotPane15, &q1, &q2);
            }
            {
                D8EQuad q1 = *(D8EQuad*)&lbl_eu_80664518;
                D8EQuad q2 = *(D8EQuad*)&lbl_eu_80664520;
                D8EQuad q3 = *(D8EQuad*)&lbl_eu_80664558;
                D8EQuad q4 = *(D8EQuad*)&lbl_eu_80664560;
                if (percentDelta < 0) {
                    __as__11_GXColorS10FRC11_GXColorS10(&q1, &lbl_eu_80664538);
                    __as__11_GXColorS10FRC11_GXColorS10(&q2, &lbl_eu_80664540);
                    __as__11_GXColorS10FRC11_GXColorS10(&q3, &lbl_eu_80664578);
                    __as__11_GXColorS10FRC11_GXColorS10(&q4, &lbl_eu_80664580);
                } else if (percentDelta > 0) {
                    __as__11_GXColorS10FRC11_GXColorS10(&q1, &lbl_eu_80664528);
                    __as__11_GXColorS10FRC11_GXColorS10(&q2, &lbl_eu_80664530);
                    __as__11_GXColorS10FRC11_GXColorS10(&q3, &lbl_eu_80664568);
                    __as__11_GXColorS10FRC11_GXColorS10(&q4, &lbl_eu_80664570);
                }
                PaneMatSetTevColors(mSlotPane6, &q3, &q4);
                PaneMatSetTevColors(mSlotPane7, &q1, &q2);
            }
            {
        D8EQuad labelTop = *(D8EQuad*)&lbl_eu_80664518;
        D8EQuad labelBottom = *(D8EQuad*)&lbl_eu_80664520;
        D8EQuad valueTop = *(D8EQuad*)&lbl_eu_80664558;
        D8EQuad valueBottom = *(D8EQuad*)&lbl_eu_80664560;
        if ((agilityDelta) < 0) {
            __as__11_GXColorS10FRC11_GXColorS10(&labelTop, &lbl_eu_80664538);
            __as__11_GXColorS10FRC11_GXColorS10(&labelBottom, &lbl_eu_80664540);
            __as__11_GXColorS10FRC11_GXColorS10(&valueTop, &lbl_eu_80664578);
            __as__11_GXColorS10FRC11_GXColorS10(&valueBottom, &lbl_eu_80664580);
        } else if ((agilityDelta) > 0) {
            __as__11_GXColorS10FRC11_GXColorS10(&labelTop, &lbl_eu_80664528);
            __as__11_GXColorS10FRC11_GXColorS10(&labelBottom, &lbl_eu_80664530);
            __as__11_GXColorS10FRC11_GXColorS10(&valueTop, &lbl_eu_80664568);
            __as__11_GXColorS10FRC11_GXColorS10(&valueBottom, &lbl_eu_80664570);
        }
        nw4r::lyt::Pane** panes = &mSlotPane0;
        PaneMatSetTevColors(panes[(8)], &valueTop, &valueBottom);
        PaneMatSetTevColors(panes[(9)], &labelTop, &labelBottom);
}
        }

        // ItemBox1 also previews effects for the selected inventory slot. The
        // slot-aware helper mirrors getItemBoxEffect but includes the candidate
        // item and selected slot in its lookup.
        {
        s32 currentEquipBonus = 0;
        if (CheckGemChain(member, NULL, NULL)) {
            currentEquipBonus = GetCondStat((void*)(u32)member, 0x30);
        }
        s16 oldStrength = (s16)(s32)(0.01f * ((100.0f + (f32)(stC->s10 + currentEquipBonus)) * (f32)((stA->s20) + (GetEffect(member, 0x21, NULL)))));
        s16 newStrength = (s16)(s32)(0.01f * ((100.0f + (f32)(stC->s10 + currentEquipBonus)) * (f32)((stA->s20) + (SumGemBonus((void*)(u32)member, 0x21, arg3, slot)))));
        s16 strengthDelta = (s16)(newStrength - oldStrength);
        LayoutSetTextBoxInt((nw4r::lyt::Layout*)mSlotPane8, newStrength);
        {
        D8EQuad labelTop = *(D8EQuad*)&lbl_eu_80664518;
        D8EQuad labelBottom = *(D8EQuad*)&lbl_eu_80664520;
        D8EQuad valueTop = *(D8EQuad*)&lbl_eu_80664558;
        D8EQuad valueBottom = *(D8EQuad*)&lbl_eu_80664560;
        if ((strengthDelta) < 0) {
            __as__11_GXColorS10FRC11_GXColorS10(&labelTop, &lbl_eu_80664538);
            __as__11_GXColorS10FRC11_GXColorS10(&labelBottom, &lbl_eu_80664540);
            __as__11_GXColorS10FRC11_GXColorS10(&valueTop, &lbl_eu_80664578);
            __as__11_GXColorS10FRC11_GXColorS10(&valueBottom, &lbl_eu_80664580);
        } else if ((strengthDelta) > 0) {
            __as__11_GXColorS10FRC11_GXColorS10(&labelTop, &lbl_eu_80664528);
            __as__11_GXColorS10FRC11_GXColorS10(&labelBottom, &lbl_eu_80664530);
            __as__11_GXColorS10FRC11_GXColorS10(&valueTop, &lbl_eu_80664568);
            __as__11_GXColorS10FRC11_GXColorS10(&valueBottom, &lbl_eu_80664570);
        }
        nw4r::lyt::Pane** panes = &mSlotPane0;
        PaneMatSetTevColors(panes[(8)], &valueTop, &valueBottom);
        PaneMatSetTevColors(panes[(9)], &labelTop, &labelBottom);
}

        s16 oldPhysical = (s16)(s32)(0.01f * ((100.0f + (f32)(stC->s0C + currentEquipBonus)) * (f32)((stA->s1C) + (GetEffect(member, 0x01, NULL)))));
        s16 newPhysical = (s16)(s32)(0.01f * ((100.0f + (f32)(stC->s0C + currentEquipBonus)) * (f32)((stA->s1C) + (SumGemBonus((void*)(u32)member, 0x01, arg3, slot)))));
        s16 physicalDelta = (s16)(newPhysical - oldPhysical);
        LayoutSetTextBoxInt((nw4r::lyt::Layout*)mSlotPane4, newPhysical);
        {
        D8EQuad labelTop = *(D8EQuad*)&lbl_eu_80664518;
        D8EQuad labelBottom = *(D8EQuad*)&lbl_eu_80664520;
        D8EQuad valueTop = *(D8EQuad*)&lbl_eu_80664558;
        D8EQuad valueBottom = *(D8EQuad*)&lbl_eu_80664560;
        if ((physicalDelta) < 0) {
            __as__11_GXColorS10FRC11_GXColorS10(&labelTop, &lbl_eu_80664538);
            __as__11_GXColorS10FRC11_GXColorS10(&labelBottom, &lbl_eu_80664540);
            __as__11_GXColorS10FRC11_GXColorS10(&valueTop, &lbl_eu_80664578);
            __as__11_GXColorS10FRC11_GXColorS10(&valueBottom, &lbl_eu_80664580);
        } else if ((physicalDelta) > 0) {
            __as__11_GXColorS10FRC11_GXColorS10(&labelTop, &lbl_eu_80664528);
            __as__11_GXColorS10FRC11_GXColorS10(&labelBottom, &lbl_eu_80664530);
            __as__11_GXColorS10FRC11_GXColorS10(&valueTop, &lbl_eu_80664568);
            __as__11_GXColorS10FRC11_GXColorS10(&valueBottom, &lbl_eu_80664570);
        }
        nw4r::lyt::Pane** panes = &mSlotPane0;
        PaneMatSetTevColors(panes[(4)], &valueTop, &valueBottom);
        PaneMatSetTevColors(panes[(5)], &labelTop, &labelBottom);
}

        s16 oldHp = (s16)(s32)(stA->f10 + (f32)RoundHalfAway0(
            0.01f * stB->f10 * (f32)(stC->s06 + currentEquipBonus +
                GetEffect(member, 0x11, NULL))));
        s16 newHp = (s16)(s32)(stA->f10 + (f32)RoundHalfAway0(
            0.01f * stB->f10 * (f32)(stC->s06 + currentEquipBonus +
                SumGemBonus((void*)(u32)member, 0x11, arg3, slot))));
        if (oldHp > 9999) oldHp = 9999;
        if (newHp > 9999) newHp = 9999;
        s16 hpDelta = (s16)(newHp - oldHp);
        if (stats->CActorParam_getHp() > (f32)newHp) {
            LayoutSetTextBoxInt((nw4r::lyt::Layout*)mSlotPane0, newHp);
        }
        LayoutSetTextBoxInt((nw4r::lyt::Layout*)mSlotPane2, newHp);
        {
            D8EQuad labelTop = *(D8EQuad*)&lbl_eu_80664518;
            D8EQuad labelBottom = *(D8EQuad*)&lbl_eu_80664520;
            D8EQuad valueTop = *(D8EQuad*)&lbl_eu_80664558;
            D8EQuad valueBottom = *(D8EQuad*)&lbl_eu_80664560;
            if (hpDelta < 0) {
                __as__11_GXColorS10FRC11_GXColorS10(&labelTop, &lbl_eu_80664538);
                __as__11_GXColorS10FRC11_GXColorS10(&labelBottom, &lbl_eu_80664540);
                __as__11_GXColorS10FRC11_GXColorS10(&valueTop, &lbl_eu_80664578);
                __as__11_GXColorS10FRC11_GXColorS10(&valueBottom, &lbl_eu_80664580);
            } else if (hpDelta > 0) {
                __as__11_GXColorS10FRC11_GXColorS10(&labelTop, &lbl_eu_80664528);
                __as__11_GXColorS10FRC11_GXColorS10(&labelBottom, &lbl_eu_80664530);
                __as__11_GXColorS10FRC11_GXColorS10(&valueTop, &lbl_eu_80664568);
                __as__11_GXColorS10FRC11_GXColorS10(&valueBottom, &lbl_eu_80664570);
            }
            PaneMatSetTevColors(mSlotPane0,
                          &valueTop, &valueBottom);
            PaneMatSetTevColors(mSlotPane1,
                          &labelTop, &labelBottom);
            PaneMatSetTevColors(mSlotPane2,
                          &valueTop, &valueBottom);
            PaneMatSetTevColors(mSlotPane3,
                          &labelTop, &labelBottom);
        }

        s16 oldEther = (s16)(s32)(0.01f * ((100.0f + (f32)(stC->s1C)) * (f32)((stA->s32) + (GetEffect(member, 0x31, NULL)))));
        s16 newEther = (s16)(s32)(0.01f * ((100.0f + (f32)(stC->s1C)) * (f32)((stA->s32) + (SumGemBonus((void*)(u32)member, 0x31, arg3, slot)))));
        s16 etherDelta = (s16)(newEther - oldEther);
        LayoutSetTextBoxInt((nw4r::lyt::Layout*)mSlotPane12, newEther);
        {
        D8EQuad labelTop = *(D8EQuad*)&lbl_eu_80664518;
        D8EQuad labelBottom = *(D8EQuad*)&lbl_eu_80664520;
        D8EQuad valueTop = *(D8EQuad*)&lbl_eu_80664558;
        D8EQuad valueBottom = *(D8EQuad*)&lbl_eu_80664560;
        if ((etherDelta) < 0) {
            __as__11_GXColorS10FRC11_GXColorS10(&labelTop, &lbl_eu_80664538);
            __as__11_GXColorS10FRC11_GXColorS10(&labelBottom, &lbl_eu_80664540);
            __as__11_GXColorS10FRC11_GXColorS10(&valueTop, &lbl_eu_80664578);
            __as__11_GXColorS10FRC11_GXColorS10(&valueBottom, &lbl_eu_80664580);
        } else if ((etherDelta) > 0) {
            __as__11_GXColorS10FRC11_GXColorS10(&labelTop, &lbl_eu_80664528);
            __as__11_GXColorS10FRC11_GXColorS10(&labelBottom, &lbl_eu_80664530);
            __as__11_GXColorS10FRC11_GXColorS10(&valueTop, &lbl_eu_80664568);
            __as__11_GXColorS10FRC11_GXColorS10(&valueBottom, &lbl_eu_80664570);
        }
        nw4r::lyt::Pane** panes = &mSlotPane0;
        PaneMatSetTevColors(panes[(12)], &valueTop, &valueBottom);
        PaneMatSetTevColors(panes[(13)], &labelTop, &labelBottom);
}

        s32 oldArtsTotal = (s32)SumTypeCounts(member, NULL);
        s32 weightPenalty = (s32)GetCondStat((void*)(u32)member, 0x97);
        if (weightPenalty != 0) {
            oldArtsTotal -= weightPenalty;
            if (oldArtsTotal < 0) oldArtsTotal = 0;
        }
        s16 oldAgilityBase = (s16)(stA->s1E - (s16)oldArtsTotal);
        if (oldAgilityBase <= 0) oldAgilityBase = 1;
        s16 oldAgilityEffect = (s16)GetEffect(member, 0x41, NULL);
        s16 oldAgility = (s16)(s32)(0.01f *
            ((100.0f + (f32)stC->s0E + (f32)currentEquipBonus) *
             (f32)(oldAgilityBase + oldAgilityEffect)));

        s16 newAgility = (s16)(s32)(0.01f * ((100.0f + (f32)(stC->s0E)) * (f32)((oldAgilityBase) + (SumGemBonus((void*)(u32)member, 0x41, arg3, slot)))));
        s16 agilityDelta = (s16)(newAgility - oldAgility);
        LayoutSetTextBoxInt((nw4r::lyt::Layout*)mSlotPane6, newAgility);
        {
        D8EQuad labelTop = *(D8EQuad*)&lbl_eu_80664518;
        D8EQuad labelBottom = *(D8EQuad*)&lbl_eu_80664520;
        D8EQuad valueTop = *(D8EQuad*)&lbl_eu_80664558;
        D8EQuad valueBottom = *(D8EQuad*)&lbl_eu_80664560;
        if ((agilityDelta) < 0) {
            __as__11_GXColorS10FRC11_GXColorS10(&labelTop, &lbl_eu_80664538);
            __as__11_GXColorS10FRC11_GXColorS10(&labelBottom, &lbl_eu_80664540);
            __as__11_GXColorS10FRC11_GXColorS10(&valueTop, &lbl_eu_80664578);
            __as__11_GXColorS10FRC11_GXColorS10(&valueBottom, &lbl_eu_80664580);
        } else if ((agilityDelta) > 0) {
            __as__11_GXColorS10FRC11_GXColorS10(&labelTop, &lbl_eu_80664528);
            __as__11_GXColorS10FRC11_GXColorS10(&labelBottom, &lbl_eu_80664530);
            __as__11_GXColorS10FRC11_GXColorS10(&valueTop, &lbl_eu_80664568);
            __as__11_GXColorS10FRC11_GXColorS10(&valueBottom, &lbl_eu_80664570);
        }
        nw4r::lyt::Pane** panes = &mSlotPane0;
        PaneMatSetTevColors(panes[(6)], &valueTop, &valueBottom);
        PaneMatSetTevColors(panes[(7)], &labelTop, &labelBottom);
}

        s16 oldDefense = (s16)(s32)(0.01f * ((100.0f + (f32)(stC->s18)) * (f32)((stA->s2E) + (GetEffect(member, 0x51, NULL)))));
        s16 newDefense = (s16)(s32)(0.01f * ((100.0f + (f32)(stC->s18)) * (f32)((stA->s2E) + (SumGemBonus((void*)(u32)member, 0x51, arg3, slot)))));
        s16 defenseDelta = (s16)(newDefense - oldDefense);
        LayoutSetTextBoxInt((nw4r::lyt::Layout*)mSlotPane14, newDefense);
        {
        D8EQuad labelTop = *(D8EQuad*)&lbl_eu_80664518;
        D8EQuad labelBottom = *(D8EQuad*)&lbl_eu_80664520;
        D8EQuad valueTop = *(D8EQuad*)&lbl_eu_80664558;
        D8EQuad valueBottom = *(D8EQuad*)&lbl_eu_80664560;
        if ((defenseDelta) < 0) {
            __as__11_GXColorS10FRC11_GXColorS10(&labelTop, &lbl_eu_80664538);
            __as__11_GXColorS10FRC11_GXColorS10(&labelBottom, &lbl_eu_80664540);
            __as__11_GXColorS10FRC11_GXColorS10(&valueTop, &lbl_eu_80664578);
            __as__11_GXColorS10FRC11_GXColorS10(&valueBottom, &lbl_eu_80664580);
        } else if ((defenseDelta) > 0) {
            __as__11_GXColorS10FRC11_GXColorS10(&labelTop, &lbl_eu_80664528);
            __as__11_GXColorS10FRC11_GXColorS10(&labelBottom, &lbl_eu_80664530);
            __as__11_GXColorS10FRC11_GXColorS10(&valueTop, &lbl_eu_80664568);
            __as__11_GXColorS10FRC11_GXColorS10(&valueBottom, &lbl_eu_80664570);
        }
        nw4r::lyt::Pane** panes = &mSlotPane0;
        PaneMatSetTevColors(panes[(14)], &valueTop, &valueBottom);
        PaneMatSetTevColors(panes[(15)], &labelTop, &labelBottom);
}

        s16 oldResistance = (s16)(s32)(0.01f * ((100.0f + (f32)(stC->s22)) * (f32)((stA->s38) + (GetEffect(member, 0x54, NULL)))));
        s16 newResistance = (s16)(s32)(0.01f * ((100.0f + (f32)(stC->s22)) * (f32)((stA->s38) + (SumGemBonus((void*)(u32)member, 0x54, arg3, slot)))));
        char* percentSuffix = BdatTouchStringCell(
            &lbl_eu_805063BC[0x130], &lbl_eu_805063BC[0x139], 0x80);
        textBuffer.format(&lbl_eu_805063BC[0x13E], newResistance,
                              percentSuffix);
        SetLayoutSelfText((nw4r::lyt::Layout*)mSlotPane16, (const char*)textBuffer.mString, 0);
        s16 resistanceDelta = (s16)(newResistance - oldResistance);
        {
        D8EQuad labelTop = *(D8EQuad*)&lbl_eu_80664518;
        D8EQuad labelBottom = *(D8EQuad*)&lbl_eu_80664520;
        D8EQuad valueTop = *(D8EQuad*)&lbl_eu_80664558;
        D8EQuad valueBottom = *(D8EQuad*)&lbl_eu_80664560;
        if ((resistanceDelta) < 0) {
            __as__11_GXColorS10FRC11_GXColorS10(&labelTop, &lbl_eu_80664538);
            __as__11_GXColorS10FRC11_GXColorS10(&labelBottom, &lbl_eu_80664540);
            __as__11_GXColorS10FRC11_GXColorS10(&valueTop, &lbl_eu_80664578);
            __as__11_GXColorS10FRC11_GXColorS10(&valueBottom, &lbl_eu_80664580);
        } else if ((resistanceDelta) > 0) {
            __as__11_GXColorS10FRC11_GXColorS10(&labelTop, &lbl_eu_80664528);
            __as__11_GXColorS10FRC11_GXColorS10(&labelBottom, &lbl_eu_80664530);
            __as__11_GXColorS10FRC11_GXColorS10(&valueTop, &lbl_eu_80664568);
            __as__11_GXColorS10FRC11_GXColorS10(&valueBottom, &lbl_eu_80664570);
        }
        nw4r::lyt::Pane** panes = &mSlotPane0;
        PaneMatSetTevColors(panes[(16)], &valueTop, &valueBottom);
        PaneMatSetTevColors(panes[(17)], &labelTop, &labelBottom);
}

        void* currentWeaponItem = CItem_kindPtr(2, *(s16*)((u8*)charObj + 0x26));
        s16 attackDelta = 0;
        if (currentWeaponItem != NULL && *(u32*)currentWeaponItem != 0) {
            D8EEntry currentWeapon;
            currentWeapon.rec = BuildCmpRecord((void*)(u32)(u16)(*(u32*)currentWeaponItem >> 20));
            D8EEntry weaponStats = currentWeapon;
            s32 oldBaseEffect = GetEffect(member, 0x01, NULL);
            s16 oldAttackBase = (s16)(s32)(0.01f * ((100.0f + (f32)(stC->s0C + currentEquipBonus)) * (f32)((stA->s1C) + (oldBaseEffect))));
            s32 newBaseEffect = SumGemBonus((void*)(u32)member, 0x01, arg3, slot);
            s16 newAttackBase = (s16)(s32)(0.01f * ((100.0f + (f32)(stC->s0C)) * (f32)((stA->s1C) + (newBaseEffect))));

            // The low-bound lookup is retained for its item-effect traversal;
            // the comparison display uses the high bound as its old endpoint.
            CalcBonus1(member, (u16)weaponStats.w04, NULL);
            s32 oldAttackHigh = oldAttackBase + CalcBonus2(member, (u16)(weaponStats.w04 >> 16), NULL);

            s32 lowScale = (s32)GetCondStat((void*)(u32)member, 0x2D);
            lowScale += (s32)SumGemBonus((void*)(u32)member, 0x52, arg3, slot);
            s32 newAttackLow = newAttackBase + (s32)(0.01f *
                (f32)((u16)weaponStats.w04 * (100 + lowScale)));

            s32 highScale = (s32)GetCondStat((void*)(u32)member, 0x2D);
            highScale += (s32)SumGemBonus((void*)(u32)member, 0x53, arg3, slot);
            s32 newAttackHigh = newAttackBase + (s32)(0.01f *
                (f32)((u16)(weaponStats.w04 >> 16) * (100 + highScale)));
            s32 displayLow = newAttackLow < newAttackHigh
                ? newAttackLow : newAttackHigh;
            attackDelta = (s16)(newAttackHigh - oldAttackHigh);

            char* rangeSeparator = BdatTouchStringCell(
                &lbl_eu_805063BC[0x130], &lbl_eu_805063BC[0x139], 0xB);
            textBuffer.format(&lbl_eu_805063BC[0x254], displayLow,
                              rangeSeparator, newAttackHigh);
            SetLayoutSelfText((nw4r::lyt::Layout*)mSlotPane10,
                          (const char*)textBuffer.mString, 0);
        }
        {
        D8EQuad labelTop = *(D8EQuad*)&lbl_eu_80664518;
        D8EQuad labelBottom = *(D8EQuad*)&lbl_eu_80664520;
        D8EQuad valueTop = *(D8EQuad*)&lbl_eu_80664558;
        D8EQuad valueBottom = *(D8EQuad*)&lbl_eu_80664560;
        if ((attackDelta) < 0) {
            __as__11_GXColorS10FRC11_GXColorS10(&labelTop, &lbl_eu_80664538);
            __as__11_GXColorS10FRC11_GXColorS10(&labelBottom, &lbl_eu_80664540);
            __as__11_GXColorS10FRC11_GXColorS10(&valueTop, &lbl_eu_80664578);
            __as__11_GXColorS10FRC11_GXColorS10(&valueBottom, &lbl_eu_80664580);
        } else if ((attackDelta) > 0) {
            __as__11_GXColorS10FRC11_GXColorS10(&labelTop, &lbl_eu_80664528);
            __as__11_GXColorS10FRC11_GXColorS10(&labelBottom, &lbl_eu_80664530);
            __as__11_GXColorS10FRC11_GXColorS10(&valueTop, &lbl_eu_80664568);
            __as__11_GXColorS10FRC11_GXColorS10(&valueBottom, &lbl_eu_80664570);
        }
        nw4r::lyt::Pane** panes = &mSlotPane0;
        PaneMatSetTevColors(panes[(10)], &valueTop, &valueBottom);
        PaneMatSetTevColors(panes[(11)], &labelTop, &labelBottom);
}
        }

    }

    // ---- equip-detail / simple path (arg4 == 0, type in {2,4,5,6,7,8}) ----
    if (arg4 == 0 && (type == 2 || (type >= 4 && type <= 8))) {
        if (type != 2 && TestBdatFlag(member, arg3)) {
            // ---- equip detail (0x801E8C88) ----
            s16 slotId = 0;
            // (was switch: retail compiles this dispatch as compares)
            if (type == 2) slotId = *(s16*)((u8*)charObj + 0x26);
            else if (type == 4) slotId = *(s16*)((u8*)charObj + 0x1C);
            else if (type == 5) slotId = *(s16*)((u8*)charObj + 0x1E);
            else if (type == 6) slotId = *(s16*)((u8*)charObj + 0x20);
            else if (type == 7) slotId = *(s16*)((u8*)charObj + 0x22);
            else if (type == 8) slotId = *(s16*)((u8*)charObj + 0x24);
            void* item = CItem_kindPtr(type, slotId);
            u16 w0 = (item != NULL && *(u32*)item != 0) ? (u16)(*(u32*)item >> 20) : 0;
            D8EArmorEntry e_cur;
            const CItemBoxSlotFlags& e_curFlags = BuildSlotFlags(w0);
            e_cur = *(const D8EArmorEntry*)&e_curFlags;
            D8EArmorEntry c_cur = e_cur;
            D8EArmorEntry e_new;
            const CItemBoxSlotFlags& e_newFlags = BuildSlotFlags((u32)arg3);
            e_new = *(const D8EArmorEntry*)&e_newFlags;
            D8EArmorEntry c_new = e_new;
            s16 v44C = (s16)SumTypeCounts(member, (item != NULL && *(u32*)item != 0)
                    ? (void*)(u32)(u16)(*(u32*)item >> 20)
                    : NULL);
            s16 v430 = (s16)SumTypeCounts(member, arg3);
            s32 eq1 = 0;
            if (CheckGemChain(member, NULL, NULL)) eq1 = GetCondStat((void*)(u32)member, 0x30);
            s32 eq2 = 0;
            if (CheckGemChain(member, arg3, NULL)) eq2 = GetCondStat((void*)(u32)member, 0x30);
            GetEffect(member, 0x21, NULL);
            s16 bA = (s16)(s32)(0.01f * ((100.0f + (f32)stC->s10 + (f32)eq2) * (f32)(stA->s20 + GetEffect(member, 0x21, arg3))));
            LayoutSetTextBoxInt((nw4r::lyt::Layout*)mSlotPane8, (s16)bA);
            GetEffect(member, 0x1, NULL);
            s16 bB = (s16)(s32)(0.01f * ((100.0f + (f32)stC->s0C + (f32)eq2) * (f32)(stA->s1C + GetEffect(member, 0x1, arg3))));
            LayoutSetTextBoxInt((nw4r::lyt::Layout*)mSlotPane4, (s16)bB);
            RoundHalfAway0(0.01f * stB->f10 * (f32)(stC->s06 + eq1 + GetEffect(member, 0x11, NULL)));
            s32 statValue = RoundHalfAway0(0.01f * stB->f10 * (f32)(stC->s06 + eq2 + GetEffect(member, 0x11, arg3)));
            s16 nb = (s16)(s32)(stA->f10 + (f32)statValue);
            if (nb > 9999) nb = 9999;
            if (stats->CActorParam_getHp() > (f32)nb) {
                LayoutSetTextBoxInt((nw4r::lyt::Layout*)mSlotPane0, nb);
            }
            LayoutSetTextBoxInt((nw4r::lyt::Layout*)mSlotPane2, nb);
            // arm-type scaling of entry value pairs
            u8 arm1 = BdatGetU8Direct(
                (const void*)lbl_eu_806640F8, &lbl_eu_805063BC[0x1EB],
                BdatGetItemId((u32)((item != NULL && *(u32*)item != 0)
                    ? (u16)(*(u32*)item >> 20)
                    : 0)));
            {
                    switch (arm1) {
                        case 3: {
                            s32 effect_3 = GetCondStat((void*)(u32)member, 0x0D);
                            if (effect_3 != 0) {
                                f32 scale_3 = 0.01f * (100.0f + (f32)effect_3);
                                c_cur.physicalDefense = (s16)(s32)((f32)c_cur.physicalDefense * scale_3);
                                c_cur.etherDefense = (s16)(s32)((f32)c_cur.etherDefense * scale_3);
                            }
                            s32 effect_24 = GetCondStat((void*)(u32)member, 0x24);
                            if (effect_24 != 0) {
                                c_cur.etherDefense = (s16)(c_cur.etherDefense + (s16)effect_24);
                            }
                            break;
                        }
                        case 2: {
                            s32 effect_2 = GetCondStat((void*)(u32)member, 0x0C);
                            if (effect_2 != 0) {
                                f32 scale_2 = 0.01f * (100.0f + (f32)effect_2);
                                c_cur.physicalDefense = (s16)(s32)((f32)c_cur.physicalDefense * scale_2);
                                c_cur.etherDefense = (s16)(s32)((f32)c_cur.etherDefense * scale_2);
                            }
                            break;
                        }
                        case 1: {
                            s32 effect_1 = GetCondStat((void*)(u32)member, 0x0B);
                            if (effect_1 != 0) {
                                f32 scale_1 = 0.01f * (100.0f + (f32)effect_1);
                                c_cur.physicalDefense = (s16)(s32)((f32)c_cur.physicalDefense * scale_1);
                                c_cur.etherDefense = (s16)(s32)((f32)c_cur.etherDefense * scale_1);
                            }
                            break;
                        }
                    }
                }
            u8 arm2 = BdatGetU8Direct((const void*)lbl_eu_806640F8, &lbl_eu_805063BC[0x1EB], BdatGetItemId((u32)(*(u32*)arg3 >> 20)));
            {
                    switch (arm2) {
                        case 3: {
                            s32 effect_3 = GetCondStat((void*)(u32)member, 0x0D);
                            if (effect_3 != 0) {
                                f32 scale_3 = 0.01f * (100.0f + (f32)effect_3);
                                c_new.physicalDefense = (s16)(s32)((f32)c_new.physicalDefense * scale_3);
                                c_new.etherDefense = (s16)(s32)((f32)c_new.etherDefense * scale_3);
                            }
                            s32 effect_24 = GetCondStat((void*)(u32)member, 0x24);
                            if (effect_24 != 0) {
                                c_new.etherDefense = (s16)(c_new.etherDefense + (s16)effect_24);
                            }
                            break;
                        }
                        case 2: {
                            s32 effect_2 = GetCondStat((void*)(u32)member, 0x0C);
                            if (effect_2 != 0) {
                                f32 scale_2 = 0.01f * (100.0f + (f32)effect_2);
                                c_new.physicalDefense = (s16)(s32)((f32)c_new.physicalDefense * scale_2);
                                c_new.etherDefense = (s16)(s32)((f32)c_new.etherDefense * scale_2);
                            }
                            break;
                        }
                        case 1: {
                            s32 effect_1 = GetCondStat((void*)(u32)member, 0x0B);
                            if (effect_1 != 0) {
                                f32 scale_1 = 0.01f * (100.0f + (f32)effect_1);
                                c_new.physicalDefense = (s16)(s32)((f32)c_new.physicalDefense * scale_1);
                                c_new.etherDefense = (s16)(s32)((f32)c_new.etherDefense * scale_1);
                            }
                            break;
                        }
                    }
                }
            // weight
            s32 wv = GetCondStat((void*)(u32)member, 0x97);
            if (wv != 0) {
                v44C = (s16)(v44C - (s16)wv);
                v430 = (s16)(v430 - (s16)wv);
                if (v44C < 0) v44C = 0;
                if (v430 < 0) v430 = 0;
            }
            // slot sum
            s32 ssum = (s32)*(s16*)((u8*)charObj + 0x22) + *(s16*)((u8*)charObj + 0x20) + *(s16*)((u8*)charObj + 0x24) + *(s16*)((u8*)charObj + 0x1C) + *(s16*)((u8*)charObj + 0x1E);
            if (ssum <= -5) {
                GetCondStat((void*)(u32)member, 0x1A);
            }
            // Recalculate candidate and current armor values in matched pairs.
            s32 c1 = GetEffect(member, 0x31, arg3);
            c_new.etherDefense = (s16)(s32)(0.01f * ((100.0f + (f32)stC->s1C) *
                (f32)((stA->s32 - c_cur.etherDefense) + (c1 + c_new.etherDefense))));
            c1 = GetEffect(member, 0x31, NULL);
            c_cur.etherDefense = (s16)(s32)(0.01f * ((100.0f + (f32)stC->s1C) *
                (f32)(stA->s32 + c1)));
            c1 = GetEffect(member, 0x51, arg3);
            c_new.physicalDefense = (s16)(s32)(0.01f * ((100.0f + (f32)stC->s18) *
                (f32)((stA->s2E - c_cur.physicalDefense) +
                      (c1 + c_new.physicalDefense))));
            c1 = GetEffect(member, 0x51, NULL);
            volatile s16 currentPhysicalDefense = (s16)(s32)(0.01f *
                ((100.0f + (f32)stC->s18) * (f32)(stA->s2E + c1)));
            GetEffect(member, 0x41, NULL);
            s32 c2 = GetEffect(member, 0x41, arg3);
            s16 b14 = (s16)(s32)(0.01f * ((100.0f + (f32)eq2) *
                (f32)((stA->s1E - (s16)v430) + c2)));
            if (b14 <= 0) b14 = 1;
            // embedded weapon block
            void* item2 = CItem_kindPtr(2, *(s16*)((u8*)charObj + 0x26));
            if (item2 != NULL && *(u32*)item2 != 0) {
                D8EEntry ew;
                ew.rec = BuildCmpRecord((void*)(u32)(u16)(*(u32*)item2 >> 20));
                D8EEntry cw = ew;
                GetEffect(member, 0x1, NULL);
                s16 wpb = (s16)(s32)(0.01f * ((100.0f + (f32)stC->s0C) *
                    (f32)(stA->s1C + GetEffect(member, 0x1, arg3))));
                s32 wa = (s32)wpb + CalcBonus1(member, (u16)cw.w04, NULL);
                s32 wb = (s32)wpb + CalcBonus2(member, (u16)(cw.w04 >> 16), NULL);
                s32 wc = (s32)wpb + CalcBonus1(member, (u16)cw.w04, arg3);
                s32 wd = (s32)wpb + CalcBonus2(member, (u16)(cw.w04 >> 16), arg3);
                s32 wmin = wc < wd ? wc : wd;
                char* rangeSeparator = BdatTouchStringCell(
                    &lbl_eu_805063BC[0x130], &lbl_eu_805063BC[0x139], 0xB);
                textBuffer.format(&lbl_eu_805063BC[0x254], wmin, rangeSeparator, wd);
                SetLayoutSelfText((nw4r::lyt::Layout*)mSlotPane10, (const char*)textBuffer.mString, 0);
            }
            // numbers
            LayoutSetTextBoxInt((nw4r::lyt::Layout*)mSlotPane12, c_new.physicalDefense);
            LayoutSetTextBoxInt((nw4r::lyt::Layout*)mSlotPane14, c_new.etherDefense);
            LayoutSetTextBoxInt((nw4r::lyt::Layout*)mSlotPane6, (s16)b14);
            // 20x color application (selected palette A8/B0/C8/D0)
            D8EQuad q1 = *(D8EQuad*)&lbl_eu_806645A8;
            D8EQuad q2 = *(D8EQuad*)&lbl_eu_806645B0;
            D8EQuad q3 = *(D8EQuad*)&lbl_eu_806645C8;
            D8EQuad q4 = *(D8EQuad*)&lbl_eu_806645D0;
            PaneMatSetTevColors(mSlotPane0, &q3, &q4);
            PaneMatSetTevColors(mSlotPane2, &q3, &q4);
            PaneMatSetTevColors(mSlotPane3, &q3, &q4);
            PaneMatSetTevColors(mSlotPane1, &q1, &q2);
            PaneMatSetTevColors(mSlotPane4, &q3, &q4);
            PaneMatSetTevColors(mSlotPane5, &q1, &q2);
            PaneMatSetTevColors(mSlotPane6, &q3, &q4);
            PaneMatSetTevColors(mSlotPane7, &q1, &q2);
            PaneMatSetTevColors(mSlotPane8, &q3, &q4);
            PaneMatSetTevColors(mSlotPane9, &q1, &q2);
            PaneMatSetTevColors(mSlotPane10, &q3, &q4);
            PaneMatSetTevColors(mSlotPane11, &q1, &q2);
            PaneMatSetTevColors(mSlotPane12, &q3, &q4);
            PaneMatSetTevColors(mSlotPane13, &q1, &q2);
            PaneMatSetTevColors(mSlotPane14, &q3, &q4);
            PaneMatSetTevColors(mSlotPane15, &q1, &q2);
            PaneMatSetTevColors(mSlotPane16, &q3, &q4);
            PaneMatSetTevColors(mSlotPane17, &q1, &q2);
            PaneMatSetTevColors(mSlotPane18, &q3, &q4);
            PaneMatSetTevColors(mSlotPane19, &q1, &q2);

        }
        // ---- simple path ----
        D8EQuad q1 = *(D8EQuad*)&lbl_eu_806645A8;
        D8EQuad q2 = *(D8EQuad*)&lbl_eu_806645B0;
        D8EQuad q3 = *(D8EQuad*)&lbl_eu_806645C8;
        D8EQuad q4 = *(D8EQuad*)&lbl_eu_806645D0;
        SetLayoutSelfText((nw4r::lyt::Layout*)mSlotPane0, &lbl_eu_805063BC[0x2AA], 0);
        SetLayoutSelfText((nw4r::lyt::Layout*)mSlotPane2, &lbl_eu_805063BC[0x2AA], 0);
        SetLayoutSelfText((nw4r::lyt::Layout*)mSlotPane3, &lbl_eu_805063BC[0x2AA], 0);
        PaneMatSetTevColors(mSlotPane1, &q1, &q2);
        SetLayoutSelfText((nw4r::lyt::Layout*)mSlotPane4, &lbl_eu_805063BC[0x2AA], 0);
        PaneMatSetTevColors(mSlotPane4, &q3, &q4);
        PaneMatSetTevColors(mSlotPane5, &q1, &q2);
        SetLayoutSelfText((nw4r::lyt::Layout*)mSlotPane6, &lbl_eu_805063BC[0x2AA], 0);
        PaneMatSetTevColors(mSlotPane6, &q3, &q4);
        PaneMatSetTevColors(mSlotPane7, &q1, &q2);
        SetLayoutSelfText((nw4r::lyt::Layout*)mSlotPane8, &lbl_eu_805063BC[0x2AA], 0);
        PaneMatSetTevColors(mSlotPane8, &q3, &q4);
        PaneMatSetTevColors(mSlotPane9, &q1, &q2);
        SetLayoutSelfText((nw4r::lyt::Layout*)mSlotPane10, &lbl_eu_805063BC[0x2AA], 0);
        PaneMatSetTevColors(mSlotPane10, &q3, &q4);
        PaneMatSetTevColors(mSlotPane11, &q1, &q2);
        SetLayoutSelfText((nw4r::lyt::Layout*)mSlotPane12, &lbl_eu_805063BC[0x2AA], 0);
        PaneMatSetTevColors(mSlotPane12, &q3, &q4);
        PaneMatSetTevColors(mSlotPane13, &q1, &q2);
        SetLayoutSelfText((nw4r::lyt::Layout*)mSlotPane14, &lbl_eu_805063BC[0x2AA], 0);
        PaneMatSetTevColors(mSlotPane14, &q3, &q4);
        PaneMatSetTevColors(mSlotPane15, &q1, &q2);
        SetLayoutSelfText((nw4r::lyt::Layout*)mSlotPane16, &lbl_eu_805063BC[0x2AA], 0);
        PaneMatSetTevColors(mSlotPane16, &q3, &q4);
        PaneMatSetTevColors(mSlotPane17, &q1, &q2);
        SetLayoutSelfText((nw4r::lyt::Layout*)mSlotPane18, &lbl_eu_805063BC[0x2AA], 0);
        PaneMatSetTevColors(mSlotPane18, &q3, &q4);
        PaneMatSetTevColors(mSlotPane19, &q1, &q2);
    }

    // ---- common tail: two 8-entry item lists (live in highFrame) ----
    CItemBoxInfoEntry* listA = highFrame.listA;
    CItemBoxInfoEntry* listB = highFrame.listB;
    listA[0].itemId = 0;
    listA[0].value = 0;
    listA[0].state = 0;
    setItemBoxEntry(&listA[1], 0, 0, 0);
    setItemBoxEntry(&listA[2], 0, 0, 0);
    setItemBoxEntry(&listA[3], 0, 0, 0);
    setItemBoxEntry(&listA[4], 0, 0, 0);
    setItemBoxEntry(&listA[5], 0, 0, 0);
    setItemBoxEntry(&listA[6], 0, 0, 0);
    setItemBoxEntry(&listA[7], 0, 0, 0);
    listB[0].itemId = 0;
    listB[0].value = 0;
    listB[0].state = 0;
    setItemBoxEntry(&listB[1], 0, 0, 0);
    setItemBoxEntry(&listB[2], 0, 0, 0);
    setItemBoxEntry(&listB[3], 0, 0, 0);
    setItemBoxEntry(&listB[4], 0, 0, 0);
    setItemBoxEntry(&listB[5], 0, 0, 0);
    setItemBoxEntry(&listB[6], 0, 0, 0);
    setItemBoxEntry(&listB[7], 0, 0, 0);

    u8 listIdxA = 0;
    u8 listIdxB = 0;

    // weapon slot
    {
        s16 weaponId = *(s16*)((u8*)charObj + 0x26);
        s16 headId = *(s16*)((u8*)charObj + 0x1C);
        s16 torsoId = *(s16*)((u8*)charObj + 0x1E);
        s16 armId = *(s16*)((u8*)charObj + 0x20);
        s16 legId = *(s16*)((u8*)charObj + 0x22);
        s16 footId = *(s16*)((u8*)charObj + 0x24);
        void* itemW = CItem_kindPtr(2, weaponId);
        void* items[5];
        items[0] = CItem_kindPtr(4, headId);
        items[1] = CItem_kindPtr(5, torsoId);
        items[2] = CItem_kindPtr(6, armId);
        items[3] = CItem_kindPtr(7, legId);
        items[4] = CItem_kindPtr(8, footId);
        if (itemW != NULL) {
            CItemImpl* itemImpl = CItem_initItemImplInstances((CItemData*)itemW);
            u8 cnt = itemImpl->vf30((CItemData*)itemW);
            for (u8 i = 0; i < 3; i++) {
                CItemBoxInfoEntry tmp;
                tmp.itemId = 0;
                tmp.value = 0;
                tmp.state = 0;
                if (i < cnt) {
                    CItemImpl* recordImpl = CItem_initItemImplInstances((CItemData*)itemW);
                    void* e = recordImpl->vf2C((CItemData*)itemW, i);
                    if (e != NULL) {
                        u32 w0 = *(u32*)e;
                        u16 w1 = *(u16*)((u8*)e + 4);
                        CItemBoxInfoEntry tmp2;
                        setItemBoxEntry(&tmp2, (s16)((w0 >> 11) & 0x7FF),
                                        (w0 >> 22) & 7, (w1 >> 4) & 0xFFF);
                        copyItemBoxEntry(&tmp, &tmp2);
                    }
                }
                copyItemBoxEntry(&listA[listIdxA++], &tmp);
                if (type != 2) {
                    copyItemBoxEntry(&listB[listIdxB++], &tmp);
                }
            }
        }
        // Candidate weapon gems use two retail representations. Kind 3 stores
        // one synthesized record at the selected slot; other weapon kinds
        // expose up to three ordinary sub-records through vf30/vf2C.
        if (type == 2) {
            void* candidateWeapon = arg3 != NULL ? arg3 : NULL;
            if (candidateWeapon != NULL && *(u32*)candidateWeapon != 0) {
            bool synthesized = ((*(u32*)arg3 >> 16) & 0xF) == 3;
            if (synthesized) {
                if (arg4 != 0) {
                    for (u8 i = 0; i < 3; i++) {
                        CItemBoxInfoEntry tmp = {0, 0, 0};
                        if (i == slot - 1) {
                            CItemBoxInfoEntry built;
                            CItemImpl* idImpl = CItem_initItemImplInstances((CItemData*)arg3);
                            u16 itemId = (u16)idImpl->vf08((CItemData*)arg3);
                            CItemImpl* valueImpl = CItem_initItemImplInstances((CItemData*)arg3);
                            u32 value = valueImpl->vf90((CItemData*)arg3);
                            CItemImpl* stateImpl = CItem_initItemImplInstances((CItemData*)arg3);
                            u8 state = (u8)stateImpl->vf54((CItemData*)arg3);
                            setItemBoxEntry(&built, itemId, value, state);
                            copyItemBoxEntry(&tmp, &built);
                        }
                        copyItemBoxEntry(&listB[listIdxB++], &tmp);
                    }
                } else {
                    { CItemBoxInfoEntry blank = {0, 0, 0};
                      copyItemBoxEntry(&listB[listIdxB++], &blank); }
                    { CItemBoxInfoEntry blank = {0, 0, 0};
                      copyItemBoxEntry(&listB[listIdxB++], &blank); }
                    { CItemBoxInfoEntry blank = {0, 0, 0};
                      copyItemBoxEntry(&listB[listIdxB++], &blank); }
                }
            } else if (arg4 != 0) {
                CItemImpl* countImpl = CItem_initItemImplInstances((CItemData*)arg3);
                u8 count = countImpl->vf30((CItemData*)arg3);
                for (u8 i = 0; i < 3; i++) {
                    CItemBoxInfoEntry tmp = {0, 0, 0};
                    if (i < count) {
                        CItemImpl* recordImpl = CItem_initItemImplInstances((CItemData*)arg3);
                        void* record = recordImpl->vf2C((CItemData*)arg3, i);
                        if (record != NULL) {
                            u32 packed = *(u32*)record;
                            u16 tail = *(u16*)((u8*)record + 4);
                            CItemBoxInfoEntry built;
                            setItemBoxEntry(&built, (packed >> 11) & 0x7FF,
                                (packed >> 22) & 7, (tail >> 4) & 0xFFF);
                            copyItemBoxEntry(&tmp, &built);
                        }
                    }
                    copyItemBoxEntry(&listB[listIdxB++], &tmp);
                }
            } else {
                { CItemBoxInfoEntry blank = {0, 0, 0};
                  copyItemBoxEntry(&listB[listIdxB++], &blank); }
                { CItemBoxInfoEntry blank = {0, 0, 0};
                  copyItemBoxEntry(&listB[listIdxB++], &blank); }
                { CItemBoxInfoEntry blank = {0, 0, 0};
                  copyItemBoxEntry(&listB[listIdxB++], &blank); }
            }
            } else {
                { CItemBoxInfoEntry blank = {0, 0, 0};
                  copyItemBoxEntry(&listB[listIdxB++], &blank); }
                { CItemBoxInfoEntry blank = {0, 0, 0};
                  copyItemBoxEntry(&listB[listIdxB++], &blank); }
                { CItemBoxInfoEntry blank = {0, 0, 0};
                  copyItemBoxEntry(&listB[listIdxB++], &blank); }
            }
        }
        // armor slots
        {
            u8 slotTypes[5] = { 4, 5, 6, 7, 8 };
            for (u8 i = 0; i < 5; i++) {
                CItemBoxInfoEntry tmp;
                tmp.itemId = 0;
                tmp.value = 0;
                tmp.state = 0;
                void* item = items[i];
                if (item != NULL) {
                    CItemImpl* itemImpl = CItem_initItemImplInstances((CItemData*)item);
                    if (itemImpl->vf30((CItemData*)item) != 0) {
                        CItemImpl* recordImpl = CItem_initItemImplInstances((CItemData*)item);
                        void* e = recordImpl->vf2C((CItemData*)item, 0);
                        if (e != NULL) {
                            u32 w0 = *(u32*)e;
                            u16 w1 = *(u16*)((u8*)e + 4);
                            CItemBoxInfoEntry tmp2;
                            setItemBoxEntry(&tmp2, (s16)((w0 >> 11) & 0x7FF),
                                            (w0 >> 22) & 7, (w1 >> 4) & 0xFFF);
                            copyItemBoxEntry(&tmp, &tmp2);
                        }
                    }
                }
                copyItemBoxEntry(&listA[listIdxA++], &tmp);
                if (type != slotTypes[i]) {
                    copyItemBoxEntry(&listB[listIdxB++], &tmp);
                }
                if (type == slotTypes[i]) {
                    CItemBoxInfoEntry tmp3;
                    tmp3.itemId = 0;
                    tmp3.value = 0;
                    tmp3.state = 0;
                    if (arg3 != NULL && *(u32*)arg3 != 0 &&
                        ((*(u32*)arg3 >> 16) & 0xF) == 3) {
                            CItemBoxInfoEntry built = {0, 0, 0};
                            CItemImpl* idImpl = CItem_initItemImplInstances((CItemData*)arg3);
                            u16 itemId = (u16)idImpl->vf08((CItemData*)arg3);
                            CItemImpl* valueImpl = CItem_initItemImplInstances((CItemData*)arg3);
                            u32 value = valueImpl->vf90((CItemData*)arg3);
                            CItemImpl* stateImpl = CItem_initItemImplInstances((CItemData*)arg3);
                            u8 state = (u8)stateImpl->vf54((CItemData*)arg3);
                            setItemBoxEntry(&built, itemId, value, state);
                            copyItemBoxEntry(&tmp3, &built);
                    } else if (arg4 != 0 || arg3 != NULL) {
                            CItemImpl* countImpl = CItem_initItemImplInstances((CItemData*)arg3);
                            u8 count = countImpl->vf30((CItemData*)arg3);
                            if (count != 0) {
                                CItemImpl* recordImpl = CItem_initItemImplInstances((CItemData*)arg3);
                                void* record = recordImpl->vf2C((CItemData*)arg3, 0);
                                if (record != NULL) {
                                    u32 packed = *(u32*)record;
                                    u16 tail = *(u16*)((u8*)record + 4);
                                    CItemBoxInfoEntry built;
                                    setItemBoxEntry(&built, (packed >> 11) & 0x7FF,
                                        (packed >> 22) & 7, (tail >> 4) & 0xFFF);
                                    copyItemBoxEntry(&tmp3, &built);
                                }
                            }
                    }
                    copyItemBoxEntry(&listB[listIdxB++], &tmp3);
                }
            }
        }

        // Type 3 replaces a crystal rather than a whole equipment slot. Find
        // the matching equipped sub-record and synthesize the replacement at
        // the resolved list position.
        if (type == 3) {
            CItemBoxInfoEntry candidate = {0, 0, 0};
            void* replacement = arg3 != NULL ? arg3 : NULL;
            if (replacement != NULL) {
            struct EquippedTypes {
                u32 values[6];
            };
            EquippedTypes equippedTypes =
                *(const EquippedTypes*)lbl_eu_80506368;
            s16 equippedIds[6] = {
                *(s16*)((u8*)charObj + 0x26), *(s16*)((u8*)charObj + 0x1C),
                *(s16*)((u8*)charObj + 0x1E), *(s16*)((u8*)charObj + 0x20),
                *(s16*)((u8*)charObj + 0x22), *(s16*)((u8*)charObj + 0x24)
            };
            u8 destination = 0;
            for (u8 equipmentIndex = 0; equipmentIndex < 6 && destination == 0;
                 equipmentIndex++) {
                void* equipped = CItem_kindPtr(
                                                (u8)equippedTypes.values[equipmentIndex],
                                                equippedIds[equipmentIndex]);
                if (equipped == NULL || *(u32*)equipped == 0) continue;

                CItemImpl* countImpl = CItem_initItemImplInstances((CItemData*)equipped);
                u16 count = (u16)countImpl->vf30((CItemData*)equipped);
                for (u8 itemIndex = 0; itemIndex < count; itemIndex++) {
                    CItemImpl* indexImpl = CItem_initItemImplInstances((CItemData*)equipped);
                    s16 crystalId = (s16)indexImpl->vf40((CItemData*)equipped, itemIndex);
                    if (crystalId != -1) {
                        void* crystal = CItem_kindPtr(3, crystalId);
                        if (crystal != NULL && *(u32*)crystal != 0 &&
                            crystal == replacement) {
                            destination = equipmentIndex == 0 ? itemIndex + 1
                                                              : equipmentIndex + 3;
                            break;
                        }
                    } else if (arg4 == 0) {
                        CItemImpl* recordImpl = CItem_initItemImplInstances((CItemData*)equipped);
                        void* record = recordImpl->vf2C((CItemData*)equipped, itemIndex);
                        if (record == NULL || (*(u16*)((u8*)record + 4) & 1) == 0) continue;

                        CItemImpl* candidateStateImpl = CItem_initItemImplInstances((CItemData*)arg3);
                        u16 candidateState = (u16)candidateStateImpl->vf54((CItemData*)arg3);
                        s16 recordValue = (s16)((*(u32*)record >> 11) & 0x7FF);
                        CItemImpl* candidateValueImpl = CItem_initItemImplInstances((CItemData*)arg3);
                        u32 candidateValue = candidateValueImpl->vf90((CItemData*)arg3);
                        if (((*(u16*)((u8*)record + 4) >> 4) & 0xFFF) == candidateState &&
                            recordValue == candidateValue) {
                            destination = equipmentIndex == 0 ? itemIndex + 1
                                                              : equipmentIndex + 3;
                            break;
                        }
                    }
                }
            }

            if (destination == 0) destination = slot;
            CItemImpl* idImpl = CItem_initItemImplInstances((CItemData*)arg3);
            u16 itemId = (u16)idImpl->vf08((CItemData*)arg3);
            CItemImpl* valueImpl = CItem_initItemImplInstances((CItemData*)arg3);
            u32 value = valueImpl->vf90((CItemData*)arg3);
            CItemImpl* stateImpl = CItem_initItemImplInstances((CItemData*)arg3);
            u8 state = (u8)stateImpl->vf54((CItemData*)arg3);
            CItemBoxInfoEntry built;
            setItemBoxEntry(&built, itemId, value, state);
            copyItemBoxEntry(&candidate, &built);
            copyItemBoxEntry(&listB[destination - 1], &candidate);
            }
        }
    }

    // ---- compact listA (dedupe by stackable flag, sum values, cap) ----
    {
        u16 ids[8] = {0};
        u32 vals[8] = {0};
        u8 flags[8] = {0};
        u16 ids2[8] = {0};
        u32 vals2[8] = {0};
        u8 flags2[8] = {0};
        u8 n = 0;
        for (u8 i = 0; i < 8; i++) {
            u16 id = listA[i].itemId;
            u32 val = listA[i].value;
            u8 flag = listA[i].state;
            if (id == 0) {
                ids[n] = id;
                vals[n] = val;
                flags[n] = flag;
                n++;
            } else if (BdatGetU8Direct((const void*)lbl_eu_806640D8, &lbl_eu_805063BC[0x4FD], id) != 0) {
                u8 found = 0;
                for (u8 j = 0; j < n; j++) {
                    if (id == ids[j]) {
                        vals[j] += val;
                        for (u8 k = j + 1; k < n; k++) {
                            if (id == ids[k]) vals[k] = vals[j];
                        }
                        found = 1;
                        ids[n] = id;
                        vals[n] = vals[j];
                        flags[n] = flag;
                        n++;
                        break;
                    }
                }
                if (!found) {
                    ids[n] = id;
                    vals[n] = val;
                    flags[n] = flag;
                    n++;
                }
            }
        }
        // same for listB
        u8 n2 = 0;
        for (u8 i = 0; i < 8; i++) {
            u16 id = listB[i].itemId;
            u32 val = listB[i].value;
            u8 flag = listB[i].state;
            if (id == 0) {
                ids2[n2] = id;
                vals2[n2] = val;
                flags2[n2] = flag;
                n2++;
            } else if (BdatGetU8Direct((const void*)lbl_eu_806640D8, &lbl_eu_805063BC[0x4FD], id) != 0) {
                u8 found = 0;
                for (u8 j = 0; j < n2; j++) {
                    if (id == ids2[j]) {
                        vals2[j] += val;
                        for (u8 k = j + 1; k < n2; k++) {
                            if (id == ids2[k]) vals2[k] = vals2[j];
                        }
                        found = 1;
                        ids2[n2] = id;
                        vals2[n2] = vals2[j];
                        flags2[n2] = flag;
                        n2++;
                        break;
                    }
                }
                if (!found) {
                    ids2[n2] = id;
                    vals2[n2] = val;
                    flags2[n2] = flag;
                    n2++;
                }
            }
        }
        for (u8 i = 0; i < 8; i++) {
            if (ids[i] != 0) {
                u32 cap = BdatGetU16Direct(lbl_eu_806640D8,
                                       &lbl_eu_805063BC[0x503], ids[i]);
                if (vals[i] > cap) vals[i] = cap;
            }
            if (ids2[i] != 0) {
                u32 cap = BdatGetU16Direct(lbl_eu_806640D8, &lbl_eu_805063BC[0x503], ids2[i]);
                if (vals2[i] > cap) vals2[i] = cap;
            }
        }

        // better/worse flags comparing listB vs listA
        u8 cmp[8] = {0};
        for (u8 i = 0; i < 8; i++) {
            u16 compareId = ids2[i];
            s32 compareValue = (s32)vals2[i];
            if (compareId != 0 && BdatGetU8Direct((const void*)lbl_eu_806640D8, &lbl_eu_805063BC[0x4FD], compareId) != 0) {
                u8 done = 0;
                for (u8 j = 0; j < 8; j++) {
                    if (compareId == ids[j]) {
                        if (compareValue > (s32)vals[j]) cmp[i] = 1;
                        else if (compareValue < (s32)vals[j]) cmp[i] = 2;
                        done = 1;
                        break;
                    }
                }
                if (!done) cmp[i] = 1;
            } else {
                u8 done = 0;
                if (compareId == ids[i]) {
                    if (compareValue > (s32)vals[i]) cmp[i] = 1;
                    else if (compareValue < (s32)vals[i]) cmp[i] = 2;
                    done = 1;
                }
                if (!done) cmp[i] = 1;
            }
        }
        // clear duplicate ids in listB
        for (u8 i = 0; i < 8; i++) {
            if (ids2[i] != 0 && BdatGetU8Direct((const void*)lbl_eu_806640D8, &lbl_eu_805063BC[0x4FD], ids2[i]) != 0) {
                for (u8 j = i + 1; j < 8; j++) {
                    if (ids2[i] == ids2[j]) ids2[j] = 0;
                }
            }
        }

        // pane name setup
        nw4r::lyt::Pane* root = mpLayout->GetRootPane()->FindPaneByName(&lbl_eu_805063BC[0x16E], true);
        for (u8 i = 0; i < 8; i++) {
            char buf1[0x20];
            char buf2[0x20];
            sprintf(buf1, &lbl_eu_805063BC[0x507], i * 2 + 0x13);
            sprintf(buf2, &lbl_eu_805063BC[0x515], i + 0x10);
            LayoutSetTextBoxFmtValue(mpLayout, buf1, &lbl_eu_805063BC[0x2AA], 0);
            LayoutSetTextBoxFmtValue(mpLayout, buf2, &lbl_eu_805063BC[0x2AA], 0);
        }

        // per-slot render
        D8ERenderQuad quad;
        for (u8 i = 0; i < 8; i++) {
            u16 id = ids2[i];
            u32 val = vals2[i];
            u8 flag = flags2[i];
            u8 cf = cmp[i];
            if (id == 0) continue;
            char buf1[0x20];
            char buf2[0x20];
            sprintf(buf1, &lbl_eu_805063BC[0x507], i * 2 + 0x13);
            sprintf(buf2, &lbl_eu_805063BC[0x515], i + 0x10);
            nw4r::lyt::Pane* pane1 = mpLayout->GetRootPane()->FindPaneByName(buf1, true);
            ScalePaneWorldPos((nw4r::math::VEC3*)&quad, pane1, root, mpLayout->GetRootPane());
            // Transpose alternating UI rows into the two four-slot columns.
            s32 idx = (s32)(0.5f * (f32)i);
            if (((s8)i % 2) != 0) idx += 4;
            if ((u8)idx < 12) {
                *(s16*)&mPage.mItemIds[(u8)idx] = id;
            }
            if ((u8)idx < 12) {
                mPage.mBytesA8[(u8)idx] = 3;
            }
            if ((u8)idx < 12) {
                mPage.mBytesB4[(u8)idx] = flag;
            }
            if ((u8)idx < 12) {
                mPage.mValues[(u8)idx] = (s16)val;
            }
            u32 dstWords[3] = {
                ((u32*)&quad)[0], ((u32*)&quad)[1], ((u32*)&quad)[2]
            };
            if ((u8)idx < 12) {
                copyVEC3(&mPage.mPos[(u8)idx],
                         (void*)dstWords);
            }
            LayoutSetTextBoxFmtValue(mpLayout, buf1, BdatGetPtrDirect(lbl_eu_806640D8, &lbl_eu_805063BC[0x139], id), 0);
            if (val == 0) {
                LayoutSetTextBoxFmtValue(mpLayout,
                              buf2, &lbl_eu_805063BC[0x2AA], 0);
            }
            u8 rvs = BdatGetU8Direct((const void*)lbl_eu_806640D8,
                                       &lbl_eu_805063BC[0x3], id);
            if (rvs != 0) {
                if (rvs == 0xFF || rvs == 0xFE) {
                    textBuffer.format(&lbl_eu_805063BC[0x13E], (s32)val,
                        BdatTouchStringCell(&lbl_eu_805063BC[0x130],
                                      &lbl_eu_805063BC[0x139], 0x21));
                } else {
                    textBuffer.format(&lbl_eu_805063BC[0x422], (s32)val);
                }
            } else {
                textBuffer.format(&lbl_eu_805063BC[0x422], (s32)val);
            }
            LayoutSetTextBoxFmtValue(mpLayout,
                          buf2, textBuffer.mString, 0);
            if (arg3 != NULL && arg4 == 0 &&
                (cf != 0 || type == 2 || !TestBdatFlag(member, arg3))) {
                cf = 0;
                LayoutSetTextBoxFmtValue(mpLayout,
                              buf2, &lbl_eu_805063BC[0x2AA], 0);
            }
            D8EQuad q1 = *(D8EQuad*)&lbl_eu_80664518;
            D8EQuad q2 = *(D8EQuad*)&lbl_eu_80664520;
            D8EQuad q3 = *(D8EQuad*)&lbl_eu_80664558;
            D8EQuad q4 = *(D8EQuad*)&lbl_eu_80664560;
            u32 cap = BdatGetU16Direct(lbl_eu_806640D8, &lbl_eu_805063BC[0x503], id);
            if ((u32)val >= cap) {
                __as__11_GXColorS10FRC11_GXColorS10(&q1, &lbl_eu_80664548);
                __as__11_GXColorS10FRC11_GXColorS10(&q2, &lbl_eu_80664550);
                __as__11_GXColorS10FRC11_GXColorS10(&q3, &lbl_eu_80664588);
                __as__11_GXColorS10FRC11_GXColorS10(&q4, &lbl_eu_80664590);
            } else if (cf == 1) {
                __as__11_GXColorS10FRC11_GXColorS10(&q1, &lbl_eu_80664528);
                __as__11_GXColorS10FRC11_GXColorS10(&q2, &lbl_eu_80664530);
                __as__11_GXColorS10FRC11_GXColorS10(&q3, &lbl_eu_80664568);
                __as__11_GXColorS10FRC11_GXColorS10(&q4, &lbl_eu_80664570);
            } else if (cf == 2) {
                __as__11_GXColorS10FRC11_GXColorS10(&q1, &lbl_eu_80664538);
                __as__11_GXColorS10FRC11_GXColorS10(&q2, &lbl_eu_80664540);
                __as__11_GXColorS10FRC11_GXColorS10(&q3, &lbl_eu_80664578);
                __as__11_GXColorS10FRC11_GXColorS10(&q4, &lbl_eu_80664580);
            }
            if (arg4 == 0) {
                __as__11_GXColorS10FRC11_GXColorS10(&q1, &lbl_eu_806645A8);
                __as__11_GXColorS10FRC11_GXColorS10(&q2, &lbl_eu_806645B0);
                __as__11_GXColorS10FRC11_GXColorS10(&q3, &lbl_eu_806645C8);
                __as__11_GXColorS10FRC11_GXColorS10(&q4, &lbl_eu_806645D0);
            }
            PaneMatSetTevColorsByName(mpLayout, buf1, &q1, &q2);
            PaneMatSetTevColorsByName(mpLayout, buf2, &q3, &q4);
        }
    }
}
#pragma pop


#undef w04
#undef _0C
#undef _10
#undef w14

#pragma push
#pragma auto_inline off
void copyItemBoxEntry(CItemBoxInfoEntry* dst, const CItemBoxInfoEntry* src) {
    dst->itemId = src->itemId;
    dst->value = src->value;
    dst->state = src->state;
}

void setItemBoxEntry(CItemBoxInfoEntry* self, u16 a, u32 b, u8 c) {
    self->itemId = a;
    self->value = b;
    self->state = c;
}

s32 CItemBoxInfo::CalcBonus1(u16 b, s32 arg2, void* d) {
    // MWCC emits the 0x4330/xoris int->float trick natively for the plain
    // (f32) cast on Broadway (no fcfid); keep it as this simple cast.
    // Same shape as calcItemBoxBonus2: statement-ordered calls, builtin
    // s32->f32 cast emits MWCC's 0x4330/xoris double-trick.
    u32 t1 = GetCondStat((void*)(u32)b, 0x2d);
    u32 t2 = GetEffect(b, 0x52, d);
    return (s32)(0.01f * (f32)(arg2 * (s32)(t2 + t1 + 0x64)));
}

s32 CItemBoxInfo::CalcBonus2(u16 b, s32 arg2, void* d) {
    // Statement-ordered calls keep the retail bl order; single-expression sum
    // keeps the values in registers like retail. The builtin s32->f32 cast
    // emits MWCC's 0x4330/xoris double-trick.
    u32 t1 = GetCondStat((void*)(u32)b, 0x2d);
    u32 t2 = GetEffect(b, 0x53, d);
    return (s32)(0.01f * (float)(arg2 * (s32)(t2 + t1 + 0x64)));
}
// Retail getItemBoxEffect: sum the equipped-effect values for one stat category.
// Six slots are scanned via the lookup table (s16 ids at +0x26/+0x1C..0x24
// and a 6-byte slot table from .sdata2). For each slot, resolve the equipped
// item (or the candidate arg3 when its category nibble matches), walk its
// sub-items, and add the category-matching values (vtable+0x54 category,
// +0x90 value for equipped, or the +0x2C sub-record for passives). The id==-1
// slot accepts only the candidate item. Result is clamped by the table cap.
u32 CItemBoxInfo::GetEffect(u16 lookup_key, u32 category, void* arg3) {
    void* lookup = CtrlObjectParam_GetCharEntry(lookup_key);
    s16 slotIds[6];
    slotIds[0] = *(s16*)((u8*)lookup + 0x26);
    slotIds[1] = *(s16*)((u8*)lookup + 0x1C);
    slotIds[2] = *(s16*)((u8*)lookup + 0x1E);
    slotIds[3] = *(s16*)((u8*)lookup + 0x20);
    slotIds[4] = *(s16*)((u8*)lookup + 0x22);
    slotIds[5] = *(s16*)((u8*)lookup + 0x24);
    u8 bytes[6] = { 2, 4, 5, 6, 7, 8 };
    u32 result = 0;
    for (u32 slot = 0; slot < 6; slot++) {
        s16 id = slotIds[(u8)slot];
        u8 j;
        s16 v;
        if (id != -1) {
            void* item = CItem_kindPtr(bytes[(u8)slot], id);
            if (arg3 != NULL &&
                (s32)bytes[(u8)slot] == (s32)((*(u32*)arg3 >> 16) & 0xF))
                item = arg3;
            if (item != NULL && *(u32*)item != 0) {
                CItemImpl* inst = CItem_initItemImplInstances((CItemData*)item);
                u32 count = (u8)inst->vf30((CItemData*)item);
                for (j = 0; (u8)j < count; j++) {
                    CItemImpl* inst2 = CItem_initItemImplInstances((CItemData*)item);
                    v = inst2->vf40((CItemData*)item, (u8)j);
                    if (v != -1) {
                        void* r = CItem_kindPtr(3, v);
                        if (r != NULL && *(u32*)r != 0) {
                            CItemImpl* inst3 = CItem_initItemImplInstances((CItemData*)r);
                            u16 cat = inst3->vf54((CItemData*)r);
                            if (category == cat) {
                                CItemImpl* inst4 = CItem_initItemImplInstances((CItemData*)r);
                                result += inst4->vf90((CItemData*)r);
                            }
                        }
                    } else {
                        CItemImpl* inst5 = CItem_initItemImplInstances((CItemData*)item);
                        CItemBoxSubRecord* sub = (CItemBoxSubRecord*)inst5->vf2C((CItemData*)item, (u8)j);
                        if (sub != NULL) {
                            u16 cat2 = (sub->field_04 >> 4) & 0xFFF;
                            if (category == cat2) {
                                s32 val2 = (sub->_00 >> 10) & 0x7FF;
                                result += (s16)val2;
                            }
                        }
                    }
                }
            }
        } else {
            if (arg3 != NULL) {
                void* itemB = NULL;
                if ((s32)bytes[(u8)slot] ==
                    (s32)((*(u32*)arg3 >> 16) & 0xF))
                    itemB = arg3;
                if (itemB != NULL && *(u32*)itemB != 0) {
                    CItemImpl* instB = CItem_initItemImplInstances((CItemData*)itemB);
                    // j and v are shared with the branch above: retail
                    // colors them identically in both arms.
                    u32 countB = (u8)instB->vf30((CItemData*)itemB);
                    for (j = 0; (u8)j < countB; j++) {
                        CItemImpl* inst2B = CItem_initItemImplInstances((CItemData*)itemB);
                        v = inst2B->vf40((CItemData*)itemB, (u8)j);
                        if (v != -1) {
                            void* rB = CItem_kindPtr(3, v);
                            if (rB != NULL && *(u32*)rB != 0) {
                                CItemImpl* inst3B = CItem_initItemImplInstances((CItemData*)rB);
                                u16 catB = inst3B->vf54((CItemData*)rB);
                                if (category == catB) {
                                    CItemImpl* inst4B = CItem_initItemImplInstances((CItemData*)rB);
                                    result += inst4B->vf90((CItemData*)rB);
                                }
                            }
                        } else {
                            CItemImpl* inst5B = CItem_initItemImplInstances((CItemData*)itemB);
                            CItemBoxSubRecord* subB = (CItemBoxSubRecord*)inst5B->vf2C((CItemData*)itemB, (u8)j);
                            if (subB != NULL) {
                                u16 cat2B = (subB->field_04 >> 4) & 0xFFF;
                                if (category == cat2B) {
                                    s32 val2B = (subB->_00 >> 10) & 0x7FF;
                                    result += (s16)val2B;
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    // cap is u16: retail clrlwi's the call result and compares signed.
    u16 cap = (u16)BdatGetU16Direct((char*)lbl_eu_806640D8, (char*)&lbl_eu_805063BC[0x503], category);
    if ((s32)cap < (s32)result) result = cap;
    return result;
}

// Retail sumItemBoxGemBonus uses the stmw/lmw frame (MWCC optimize_for_size
// prologue merge). The slot argument maps 1..3 to type 2 with a sub-index,
// and 4..8 to direct types; the candidate pointer is nulled out by the first
// pass when one of its gems matches, so the sum pass treats it as unequipped.
#pragma push
#pragma optimize_for_size on
__declspec(noinline) u32 CItemBoxInfo::SumGemBonus(void* member, u32 category, void* candidate, s32 slot) {
    (void)this;
    u32 type = 0;
    u32 index = 0;
    // Consecutive cases 1..3 lower to the retail range check
    // (subi r0,slot,1; cmplwi r0,2; ble) with the shared body after the
    // 4..8 dispatch chain, matching the retail block layout.
    switch (slot) {
        case 1:
        case 2:
        case 3:
            index = (u8)(slot - 1);
            type = 2;
            break;
        case 4: type = 4; index = 0; break;
        case 5: type = 5; index = 0; break;
        case 6: type = 6; index = 0; break;
        case 7: type = 7; index = 0; break;
        case 8: type = 8; index = 0; break;
        default: break;
    }
    void* lookup = CtrlObjectParam_GetCharEntry((u32)member);
    s16 ids[6];
    // ids[0] is the halfword at lookup+0x26; the remaining five are the packed
    // block at 0x1C..0x24 (retail load order).
    ids[0] = ((CItemBoxCharEntryView*)lookup)->slotIds[5];
    ids[1] = ((CItemBoxCharEntryView*)lookup)->slotIds[0];
    ids[2] = ((CItemBoxCharEntryView*)lookup)->slotIds[1];
    ids[3] = ((CItemBoxCharEntryView*)lookup)->slotIds[2];
    ids[4] = ((CItemBoxCharEntryView*)lookup)->slotIds[3];
    ids[5] = ((CItemBoxCharEntryView*)lookup)->slotIds[4];
    // 6-byte slot table copied from two .sdata2 constants.
    u8 bytes[6] = { 2, 4, 5, 6, 7, 8 };
    s32 sum = 0;
    // First pass: if the candidate item's gem chain contains the candidate
    // pointer itself, null the candidate out (treated as unequipped below).
    u32 found = 0;
    for (u32 i = 0; i < 6; i++) {
        s16 id = ids[(u8)i];
        if (id == -1) continue;
        void* item = CItem_kindPtr(bytes[(u8)i], id);
        if (item == NULL || ((CItemData*)item)->field_00 == 0) continue;
        CItemImpl* inst = CItem_initItemImplInstances((CItemData*)item);
        u8 count = inst->vf30((CItemData*)item);
        for (u32 j = 0; (u8)j < count; j++) {
            CItemImpl* inst2 = CItem_initItemImplInstances((CItemData*)item);
            s16 v = inst2->vf40((CItemData*)item, (u8)j);
            if (v == -1) continue;
            void* r = CItem_kindPtr(3, v);
            if (r == NULL || ((CItemData*)r)->field_00 == 0) continue;
            if (r == candidate) {
                candidate = NULL;
                found = 1;
                break;
            }
        }
        if (found) break;
    }
    // Second pass: sum the gem values whose equip category matches, with the
    // candidate slot/index pair handled through its own vtable accessors.
    for (u32 i = 0; i < 6; i++) {
        s16 id = ids[(u8)i];
        if (id == -1) continue;
        u8 slotB = bytes[(u8)i];
        void* item = CItem_kindPtr(slotB, id);
        if (item == NULL || ((CItemData*)item)->field_00 == 0) continue;
        CItemImpl* inst = CItem_initItemImplInstances((CItemData*)item);
        u8 count = inst->vf30((CItemData*)item);
        for (u32 j = 0; (u8)j < count; j++) {
            if (candidate != NULL && slotB == type && (u8)j == index) {
                CItemImpl* instA = CItem_initItemImplInstances((CItemData*)candidate);
                u16 cat = instA->vf54((CItemData*)candidate);
                if (category == (u32)cat) {
                    CItemImpl* instB = CItem_initItemImplInstances((CItemData*)candidate);
                    sum += (s32)instB->vf90((CItemData*)candidate);
                }
            } else {
                CItemImpl* instC = CItem_initItemImplInstances((CItemData*)item);
                s16 v = instC->vf40((CItemData*)item, (u8)j);
                if (v != -1) {
                    void* r = CItem_kindPtr(3, v);
                    if (r != NULL && ((CItemData*)r)->field_00 != 0) {
                        CItemImpl* instD = CItem_initItemImplInstances((CItemData*)r);
                        u16 cat2 = instD->vf54((CItemData*)r);
                        if (category == (u32)cat2) {
                            CItemImpl* instE = CItem_initItemImplInstances((CItemData*)r);
                            sum += (s32)instE->vf90((CItemData*)r);
                        }
                    }
                } else {
                    CItemImpl* instF = CItem_initItemImplInstances((CItemData*)item);
                    CItemBoxSubRecord* sub = (CItemBoxSubRecord*)instF->vf2C((CItemData*)item, (u8)j);
                    if (sub != NULL && category == (u32)((sub->field_04 >> 4) & 0xFFF)) {
                        sum += (s16)((sub->_00 >> 10) & 0x7FF);
                    }
                }
            }
        }
    }
    u16 max = (u16)BdatGetU16Direct(lbl_eu_806640D8, (char*)&lbl_eu_805063BC[0x503], category);
    if (max < sum) sum = max;
    return (u32)sum;
}
#pragma pop
#pragma push
#pragma optimize_for_size on
__declspec(noinline) u32 CItemBoxInfo::GetCondStat(void* b, u32 arg2) {
    // Retail keeps obj in r31 and result in r30 (obj declared first gets the
    // highest saved register); stmw/lmw frame from optimize_for_size.
    void* obj;
    u32 result = 0;
    obj = (u8*)CtrlObjectParam_GetCharEntry((u32)b) + 0x3534;
    if (Counter_TestBit(obj, arg2) != 0) {
        result = IdTable_SumValues(obj, arg2);
    }
    return result;
}
#pragma pop

#pragma push
#pragma optimize_for_size on
__declspec(noinline) u32 CItemBoxInfo2::GetCondStatById(u32 arg1, u32 arg2) {
    // Retail keeps obj in r31 and result in r30 (obj declared first gets the
    // highest saved register); the result=0 init lands before the call.
    void* obj;
    u32 result = 0;
    obj = ((CItemBoxCharEntryView*)CtrlObjectParam_GetCharEntry(arg1))->counterArea;
    if (Counter_TestBit(obj, arg2) != 0) {
        result = IdTable_SumValues(obj, arg2);
    }
    return result;
}
#pragma pop

u32 CItemBoxInfo2::GetCondStat2D(void* arg1) {
    void* obj = (u8*)CtrlObjectParam_GetCharEntry((u8)(u32)arg1) + 0x3534;
    if (Counter_TestBit(obj, 0x2d) != 0) {
        return IdTable_SumValues(obj, 0x2d);
    }
    return 0;
}


#pragma push
#pragma optimize_for_size on
__declspec(noinline) u32 CItemBoxInfo::TestBdatFlag(u32 arg1, void* arg2) {
    if (arg1 == 0 || arg2 == 0) return 0;
    char buf[0x20];
    sprintf(buf, &lbl_eu_805063BC[0x1f4], arg1);
    u32 g = (u32)lbl_eu_806640F8;
    u8 result = BdatGetU8Direct((const void*)g, (const char*)buf, (u16)BdatGetItemId(((CItemData*)arg2)->field_00 >> 20));
    return result != 0;
}
#pragma pop
u32 CItemBoxInfo::SumTypeCounts(u16 arg2, void* arg3) {
    if (arg2 == 0) return 0;
    void* g = lbl_eu_806640F8;
    u16 v1 = arg3 ? (u16)BdatGetItemType((u32)arg3) : 0;
    u16 v2 = arg3 ? (u16)BdatGetItemId((u32)arg3) : 0;
    void* lookup = CtrlObjectParam_GetCharEntry(arg2);
    u32 result = 0;
    for (u32 i = 4; i <= 8; i++) {
        u8 ii = (u8)i;
        if (ii == v1) continue;
        s16 val = -1;
        switch (ii) {
            case 4: val = ((CItemBoxCharEntryView*)lookup)->slotIds[0]; break;
            case 5: val = ((CItemBoxCharEntryView*)lookup)->slotIds[1]; break;
            case 6: val = ((CItemBoxCharEntryView*)lookup)->slotIds[2]; break;
            case 7: val = ((CItemBoxCharEntryView*)lookup)->slotIds[3]; break;
            case 8: val = ((CItemBoxCharEntryView*)lookup)->slotIds[4]; break;
        }
        if (val == -1) continue;
        // Retail passes the slot id (u8)i and the slot value val (2-arg form).
        void* r = CItem_kindPtr((u8)i, val);
        if (r == NULL) continue;
        u32 v = ((CItemData*)r)->field_00;
        if (v == 0) continue;
        result += (u8)BdatGetU8Direct((const void*)g, (char*)&lbl_eu_805063BC[0x1e2], BdatGetItemId(v >> 20));
    }
    if (arg3 != NULL) {
        result += (u8)BdatGetU8Direct((const void*)g, (char*)&lbl_eu_805063BC[0x1e2], v2);
    }
    return result;
}
u32 CItemBoxInfo::CheckGemChain(u16 lookup_key, void* arg3, void* unused2) {
    void* lookup = CtrlObjectParam_GetCharEntry(lookup_key);
    s16 ids[6];
    // ids[0] is the halfword at lookup+0x26; the remaining five are the packed
    // block at 0x1C..0x24 (retail load order).
    ids[0] = ((CItemBoxCharEntryView*)lookup)->slotIds[5];
    ids[1] = ((CItemBoxCharEntryView*)lookup)->slotIds[0];
    ids[2] = ((CItemBoxCharEntryView*)lookup)->slotIds[1];
    ids[3] = ((CItemBoxCharEntryView*)lookup)->slotIds[2];
    ids[4] = ((CItemBoxCharEntryView*)lookup)->slotIds[3];
    ids[5] = ((CItemBoxCharEntryView*)lookup)->slotIds[4];
    // 6-byte slot table copied from two .sdata2 constants.
    u8 bytes[6] = { 2, 4, 5, 6, 7, 8 };
    for (u8 i = 0; i < 6; i++) {
        s16 id = ids[i];
        if (id != -1) {
            u8 slot = bytes[i];
            // Retail passes (slot, id): id already sits in r4 from the lhax,
            // so MWCC reuses it as the second argument (2-arg form).
            void* item = CItem_kindPtr(slot, id);
            if (arg3 != NULL && slot == (s32)((((CItemData*)arg3)->field_00 >> 16) & 0xF)) item = arg3;
            if (item != NULL && ((CItemData*)item)->field_00 != 0) {
                CItemImpl* inst = CItem_initItemImplInstances((CItemData*)item);
                u8 count = inst->vf30((CItemData*)item);
                for (u8 j = 0; j < count; j++) {
                    CItemImpl* inst2 = CItem_initItemImplInstances((CItemData*)item);
                    s16 v = inst2->vf40((CItemData*)item, j);
                    if (v != -1) {
                        void* r = CItem_kindPtr(3, v);
                        if (r != NULL && ((CItemData*)r)->field_00 != 0) return 0;
                    } else {
                        CItemImpl* inst3 = CItem_initItemImplInstances((CItemData*)item);
                        CItemBoxSubRecord* sub = (CItemBoxSubRecord*)inst3->vf2C((CItemData*)item, j);
                        if (sub != NULL && ((sub->field_04 >> 4) & 0xFFF) != 0) return 0;
                    }
                }
            }
        } else {
            if (arg3 != NULL) {
                // item defaults to NULL; retail places the init after the
                // arg3 guard and only overwrites it on a category match.
                void* item = NULL;
                if (bytes[i] == (s32)((((CItemData*)arg3)->field_00 >> 16) & 0xF)) item = arg3;
                if (item != NULL && ((CItemData*)item)->field_00 != 0) {
                    CItemImpl* inst = CItem_initItemImplInstances((CItemData*)item);
                    u8 count = inst->vf30((CItemData*)item);
                    for (u8 j = 0; j < count; j++) {
                        CItemImpl* inst2 = CItem_initItemImplInstances((CItemData*)item);
                        s16 v = inst2->vf40((CItemData*)item, j);
                        if (v != -1) {
                            void* r = CItem_kindPtr(3, v);
                            if (r != NULL && ((CItemData*)r)->field_00 != 0) return 0;
                        } else {
                            CItemImpl* inst3 = CItem_initItemImplInstances((CItemData*)item);
                            CItemBoxSubRecord* sub = (CItemBoxSubRecord*)inst3->vf2C((CItemData*)item, j);
                            if (sub != NULL && ((sub->field_04 >> 4) & 0xFFF) != 0) return 0;
                        }
                    }
                }
            }
        }
    }
    return 1;
}
#pragma pop

#pragma push
#pragma optimize_for_size on
// Retail CItemBoxInfo::OnFileEvent: two file-load branches. The line-arc
// branch builds the whole layout (0x14000 scratch region), seeds every label
// textbox with the shared text object, then runs the item-box1 reset pass:
// pane clears gated on the mode byte (+0x9A), fixed label texts, per-mode
// caption, and the four vertex-colour blocks whose results are written both
// into this (+0x9C/+0xA4) and into the .sbss colour globals (518/520/558/
// 560/598/5A0/5B8/5C0) with their alpha halves propagated to the paired
// globals. Finally caches the 20 slot panes at +0x40..+0x8C.
bool CItemBoxInfo::OnFileEvent(CEventFile* file) {
    char* base = lbl_eu_805063BC;
    if (mFileHandle1 == file->mFileHandle) {
        mRegion1.createRegion(mtl::MemManager::getHandleMEM2(), 0x14000,
                                      &lbl_eu_805063BC[0x524], 1);
        Class_8045F858 host(&mRegion1);
        u8* data = (u8*)mFileHandle1->getData();
        mtl::MemManager::setMemInitFlag(false);
        nw4r::lyt::ArcResourceAccessor* acc = createArcResourceAccessor__10CLibLayoutFv();
        mArcResAcc = acc;
        acc->Attach(data, &lbl_eu_805063BC[0x531]);
        buildLayout(&mpLayout, acc, &lbl_eu_805063BC[0x535]);
        bindLayoutAnimTransform(mpLayout, &mAnimIn, acc, &lbl_eu_805063BC[0x54a]);
        bindLayoutAnimTransform(mpLayout, &mAnimInfoIn, acc, &lbl_eu_805063BC[0x562]);
        nw4r::lyt::Pane* root = mpLayout->GetRootPane();
        void* fontObj = getFontInfo__11CDeviceFontFUlPQ34nw4r3lyt6Layout(1, mpLayout);
        ApplyFontToTxtPanes(root, (const nw4r::ut::Font*)(static_cast<IDeviceFontInfo*>(fontObj)->getFont()));

        // Seed the label textboxes with the shared text object.
        char* text = CUICfManager_getPackedFont9C();
        setLayoutTextBoxFont(mpLayout, &lbl_eu_805063BC[0x25b], (u32)text);
        setLayoutTextBoxFont(mpLayout, &lbl_eu_805063BC[0x267], (u32)text);
        setLayoutTextBoxFont(mpLayout, &lbl_eu_805063BC[0x273], (u32)text);
        setLayoutTextBoxFont(mpLayout, &lbl_eu_805063BC[0x286], (u32)text);
        setLayoutTextBoxFont(mpLayout, &lbl_eu_805063BC[0x292], (u32)text);
        setLayoutTextBoxFont(mpLayout, &lbl_eu_805063BC[0x29e], (u32)text);
        setLayoutTextBoxFont(mpLayout, &lbl_eu_805063BC[0x45a], (u32)text);
        setLayoutTextBoxFont(mpLayout, &lbl_eu_805063BC[0x466], (u32)text);
        setLayoutTextBoxFont(mpLayout, &lbl_eu_805063BC[0x4a7], (u32)text);
        setLayoutTextBoxFont(mpLayout, &lbl_eu_805063BC[0x57f], (u32)text);
        setLayoutTextBoxFont(mpLayout, &lbl_eu_805063BC[0x58b], (u32)text);
        setLayoutTextBoxFont(mpLayout, &lbl_eu_805063BC[0x597], (u32)text);
        setLayoutTextBoxFont(mpLayout, &lbl_eu_805063BC[0x5a3], (u32)text);
        setLayoutTextBoxFont(mpLayout, &lbl_eu_805063BC[0x5af], (u32)text);
        setLayoutTextBoxFont(mpLayout, &lbl_eu_805063BC[0x5be], (u32)text);
        setLayoutTextBoxFont(mpLayout, &lbl_eu_805063BC[0x5cd], (u32)text);
        setLayoutTextBoxFont(mpLayout, &lbl_eu_805063BC[0x143], (u32)text);
        setLayoutTextBoxFont(mpLayout, &lbl_eu_805063BC[0x4e3], (u32)text);
        setLayoutTextBoxFont(mpLayout, &lbl_eu_805063BC[0x4f0], (u32)text);
        setLayoutTextBoxFont(mpLayout, &lbl_eu_805063BC[0x5dc], (u32)text);
        setLayoutTextBoxFont(mpLayout, &lbl_eu_805063BC[0x5e9], (u32)text);
        setLayoutTextBoxFont(mpLayout, &lbl_eu_805063BC[0x5f6], (u32)text);
        setLayoutTextBoxFont(mpLayout, &lbl_eu_805063BC[0x603], (u32)text);
        setLayoutTextBoxFont(mpLayout, &lbl_eu_805063BC[0x610], (u32)text);
        setLayoutTextBoxFont(mpLayout, &lbl_eu_805063BC[0x61d], (u32)text);
        setLayoutTextBoxFont(mpLayout, &lbl_eu_805063BC[0x62a], (u32)text);
        setLayoutTextBoxFont(mpLayout, &lbl_eu_805063BC[0x637], (u32)text);
        setLayoutTextBoxFont(mpLayout, &lbl_eu_805063BC[0x644], (u32)text);
        setLayoutTextBoxFont(mpLayout, &lbl_eu_805063BC[0x651], (u32)text);
        setLayoutTextBoxFont(mpLayout, &lbl_eu_805063BC[0x65e], (u32)text);
        setLayoutTextBoxFont(mpLayout, &lbl_eu_805063BC[0x66b], (u32)text);
        setLayoutTextBoxFont(mpLayout, &lbl_eu_805063BC[0x678], (u32)text);
        setLayoutTextBoxFont(mpLayout, &lbl_eu_805063BC[0x685], (u32)text);
        setLayoutTextBoxFont(mpLayout, &lbl_eu_805063BC[0x692], (u32)text);
        setLayoutTextBoxFont(mpLayout, &lbl_eu_805063BC[0x69f], (u32)text);
        setLayoutTextBoxFont(mpLayout, &lbl_eu_805063BC[0x6ac], (u32)text);
        setLayoutTextBoxFont(mpLayout, &lbl_eu_805063BC[0x6b9], (u32)text);
        setLayoutTextBoxFont(mpLayout, &lbl_eu_805063BC[0x6c6], (u32)text);
        setLayoutTextBoxFont(mpLayout, &lbl_eu_805063BC[0x17d], (u32)text);
        setLayoutTextBoxFont(mpLayout, &lbl_eu_805063BC[0x6d2], (u32)text);
        setLayoutTextBoxFont(mpLayout, &lbl_eu_805063BC[0x188], (u32)text);

        mpLayout->SetAnimationEnable(mAnimInfoIn, false);
        mpLayout->SetAnimationEnable(mAnimIn, true);
        mpLayout->Animate(0);

        u8 mode = mKind;
        if (mode != 1 && mode != 2) {
            setPaneVisible(root->FindPaneByName(&lbl_eu_805063BC[0x6dd], true), 0);
        }
        if (mode != 3) {
            setPaneVisible(root->FindPaneByName(&lbl_eu_805063BC[0x6e6], true), 0);
        }

        // Fixed label texts for the closed item-box view.
        LayoutSetTextBoxFmtValue((nw4r::lyt::Layout*)mpLayout, &lbl_eu_805063BC[0x2b6],
                      BdatTouchStringCell(&lbl_eu_805063BC[0x130], &lbl_eu_805063BC[0x139], 0xa), 0);
        LayoutSetTextBoxFmtValue((nw4r::lyt::Layout*)mpLayout, &lbl_eu_805063BC[0x2d7],
                      BdatTouchStringCell(&lbl_eu_805063BC[0x130], &lbl_eu_805063BC[0x139], 0xd), 0);
        LayoutSetTextBoxFmtValue((nw4r::lyt::Layout*)mpLayout, &lbl_eu_805063BC[0x2e2],
                      BdatTouchStringCell(&lbl_eu_805063BC[0x130], &lbl_eu_805063BC[0x139], 0x11), 0);
        LayoutSetTextBoxFmtValue((nw4r::lyt::Layout*)mpLayout, &lbl_eu_805063BC[0x2c1],
                      BdatTouchStringCell(&lbl_eu_805063BC[0x130], &lbl_eu_805063BC[0x139], 0x12), 0);
        LayoutSetTextBoxFmtValue((nw4r::lyt::Layout*)mpLayout, &lbl_eu_805063BC[0x2cc],
                      BdatTouchStringCell(&lbl_eu_805063BC[0x130], &lbl_eu_805063BC[0x139], 0xc), 0);
        LayoutSetTextBoxFmtValue((nw4r::lyt::Layout*)mpLayout, &lbl_eu_805063BC[0x2ed],
                      BdatTouchStringCell(&lbl_eu_805063BC[0x130], &lbl_eu_805063BC[0x139], 0x13), 0);
        LayoutSetTextBoxFmtValue((nw4r::lyt::Layout*)mpLayout, &lbl_eu_805063BC[0x6f2],
                      BdatTouchStringCell(&lbl_eu_805063BC[0x130], &lbl_eu_805063BC[0x139], 0x18), 0);
        LayoutSetTextBoxFmtValue((nw4r::lyt::Layout*)mpLayout, &lbl_eu_805063BC[0x6fd],
                      BdatTouchStringCell(&lbl_eu_805063BC[0x130], &lbl_eu_805063BC[0x139], 0x18), 0);
        LayoutSetTextBoxFmtValue((nw4r::lyt::Layout*)mpLayout, &lbl_eu_805063BC[0x708],
                      BdatTouchStringCell(&lbl_eu_805063BC[0x130], &lbl_eu_805063BC[0x139], 0x24), 0);
        LayoutSetTextBoxFmtValue((nw4r::lyt::Layout*)mpLayout, &lbl_eu_805063BC[0x713],
                      BdatTouchStringCell(&lbl_eu_805063BC[0x130], &lbl_eu_805063BC[0x139], 0x25), 0);
        LayoutSetTextBoxFmtValue((nw4r::lyt::Layout*)mpLayout, &lbl_eu_805063BC[0x71e],
                      BdatTouchStringCell(&lbl_eu_805063BC[0x130], &lbl_eu_805063BC[0x139], 0x70), 0);
        LayoutSetTextBoxFmtValue((nw4r::lyt::Layout*)mpLayout, &lbl_eu_805063BC[0x72a],
                      BdatTouchStringCell(&lbl_eu_805063BC[0x130], &lbl_eu_805063BC[0x139], 0x72), 0);
        LayoutSetTextBoxFmtValue((nw4r::lyt::Layout*)mpLayout, &lbl_eu_805063BC[0x736],
                      BdatTouchStringCell(&lbl_eu_805063BC[0x130], &lbl_eu_805063BC[0x139], 0x73), 0);
        LayoutSetTextBoxFmtValue((nw4r::lyt::Layout*)mpLayout, &lbl_eu_805063BC[0x742],
                      BdatTouchStringCell(&lbl_eu_805063BC[0x130], &lbl_eu_805063BC[0x139], 0x74), 0);
        LayoutSetTextBoxFmtValue((nw4r::lyt::Layout*)mpLayout, &lbl_eu_805063BC[0x74e],
                      BdatTouchStringCell(&lbl_eu_805063BC[0x130], &lbl_eu_805063BC[0x139], 0x79), 0);
        LayoutSetTextBoxFmtValue((nw4r::lyt::Layout*)mpLayout, &lbl_eu_805063BC[0x75a],
                      BdatTouchStringCell(&lbl_eu_805063BC[0x130], &lbl_eu_805063BC[0x139], 0x75), 0);
        LayoutSetTextBoxFmtValue((nw4r::lyt::Layout*)mpLayout, &lbl_eu_805063BC[0x766],
                      BdatTouchStringCell(&lbl_eu_805063BC[0x130], &lbl_eu_805063BC[0x139], 0x7b), 0);
        LayoutSetTextBoxFmtValue((nw4r::lyt::Layout*)mpLayout, &lbl_eu_805063BC[0x772],
                      BdatTouchStringCell(&lbl_eu_805063BC[0x130], &lbl_eu_805063BC[0x139], 0x76), 0);
        LayoutSetTextBoxFmtValue((nw4r::lyt::Layout*)mpLayout, &lbl_eu_805063BC[0x77e],
                      BdatTouchStringCell(&lbl_eu_805063BC[0x130], &lbl_eu_805063BC[0x139], 0x77), 0);
        LayoutSetTextBoxFmtValue((nw4r::lyt::Layout*)mpLayout, &lbl_eu_805063BC[0x78a],
                      BdatTouchStringCell(&lbl_eu_805063BC[0x130], &lbl_eu_805063BC[0x139], 0x78), 0);
        LayoutSetTextBoxFmtValue((nw4r::lyt::Layout*)mpLayout, &lbl_eu_805063BC[0x796],
                      BdatTouchStringCell(&lbl_eu_805063BC[0x130], &lbl_eu_805063BC[0x139], 0x7e), 0);
        LayoutSetTextBoxFmtValue((nw4r::lyt::Layout*)mpLayout, &lbl_eu_805063BC[0x2f8],
                      BdatTouchStringCell(&lbl_eu_805063BC[0x130], &lbl_eu_805063BC[0x139], 0x2f), 0);

        // Per-mode caption: modes 0/1 show the owned-item count; mode 3 shows
        // the category name resolved from the selected item id.
        if ((s8)mode <= 1) {
            ml::FixStr<32> caption(true);
            caption.format(&lbl_eu_805063BC[0x13e], CItemBlock_getPtr20E8(),
                           BdatTouchStringCell(&lbl_eu_805063BC[0x130], &lbl_eu_805063BC[0x139], 3));
            LayoutSetTextBoxFmtValue((nw4r::lyt::Layout*)mpLayout, &lbl_eu_805063BC[0x143],
                          caption.c_str(), 0);
        } else if (mode == 3) {
            u16 itemId = mMode;
            u8 kind = (u8)MapSexFlagGroup(itemId);
            char* s;
            if (kind == 0) {
                s = BdatTouchStringCell(&lbl_eu_805063BC[0x130], &lbl_eu_805063BC[0x139], 0x24);
            } else {
                s = BdatTouchStringCell(&lbl_eu_805063BC[0x130], &lbl_eu_805063BC[0x139], kind + 0x1F);
            }
            LayoutSetTextBoxFmtValue((nw4r::lyt::Layout*)mpLayout, &lbl_eu_805063BC[0x7ad], s, 0);
            char* name = ((char* (*)(void*, const char*, u16))&BdatGetPtrDirect)(
                &lbl_eu_80664098, &lbl_eu_805063BC[0x139], itemId);
            LayoutSetTextBoxFmtValue((nw4r::lyt::Layout*)mpLayout, &lbl_eu_805063BC[0x7b9],
                          GetBdatNameByNumId(name), 0);
            LayoutSetTextBoxFmtValue((nw4r::lyt::Layout*)mpLayout, &lbl_eu_805063BC[0x7c5],
                          BdatTouchStringCell(&lbl_eu_805063BC[0x130], &lbl_eu_805063BC[0x139], 0x2d), 0);
            LayoutSetTextBoxFmtValue((nw4r::lyt::Layout*)mpLayout, &lbl_eu_805063BC[0x7d2],
                          &lbl_eu_805063BC[0x2aa], 0);
            LayoutSetTextBoxFmtValue((nw4r::lyt::Layout*)mpLayout, &lbl_eu_805063BC[0x7e0],
                          &lbl_eu_805063BC[0x2aa], 0);
        }

        // Vertex colours: the highlight pair goes onto this (+0x9C/+0xA4); the
        // three named panes' pairs go into the .sbss colour globals with their
        // alpha halves propagated to the paired globals.
        nw4r::lyt::Pane* hl = root->FindPaneByName(&lbl_eu_805063BC[0x7ee], true);
        CItemBoxGXColor c1 = PaneMatGetS16Quad(hl, 0);
        __as__11_GXColorS10FRC11_GXColorS10((u8*)this + 0x9C, &c1);
        CItemBoxGXColor c2 = PaneMatGetS16Quad(hl, 1);
        __as__11_GXColorS10FRC11_GXColorS10((u8*)this + 0xA4, &c2);

        nw4r::lyt::Pane* p71e = root->FindPaneByName(&lbl_eu_805063BC[0x71e], true);
        CItemBoxGXColor c3 = PaneMatGetS16Quad(p71e, 0);
        __as__11_GXColorS10FRC11_GXColorS10(&lbl_eu_80664518, &c3);
        CItemBoxGXColor c4 = PaneMatGetS16Quad(p71e, 1);
        __as__11_GXColorS10FRC11_GXColorS10(&lbl_eu_80664520, &c4);
        {
            s16 a0 = *(s16*)((char*)&lbl_eu_80664518 + 6);
            s16 a1 = *(s16*)((char*)&lbl_eu_80664520 + 6);
            *(s16*)((char*)&lbl_eu_80664528 + 6) = a0;
            *(s16*)((char*)&lbl_eu_80664530 + 6) = a1;
            *(s16*)((char*)&lbl_eu_80664538 + 6) = a0;
            *(s16*)((char*)&lbl_eu_80664540 + 6) = a1;
            *(s16*)((char*)&lbl_eu_80664548 + 6) = a0;
            *(s16*)((char*)&lbl_eu_80664550 + 6) = a1;
        }

        nw4r::lyt::Pane* p4e3 = root->FindPaneByName(&lbl_eu_805063BC[0x4e3], true);
        CItemBoxGXColor c5 = PaneMatGetS16Quad(p4e3, 0);
        __as__11_GXColorS10FRC11_GXColorS10(&lbl_eu_80664558, &c5);
        CItemBoxGXColor c6 = PaneMatGetS16Quad(p4e3, 1);
        __as__11_GXColorS10FRC11_GXColorS10(&lbl_eu_80664560, &c6);

        nw4r::lyt::Pane* p2b6 = root->FindPaneByName(&lbl_eu_805063BC[0x2b6], true);
        CItemBoxGXColor c7 = PaneMatGetS16Quad(p2b6, 0);
        __as__11_GXColorS10FRC11_GXColorS10(&lbl_eu_80664598, &c7);
        CItemBoxGXColor c8 = PaneMatGetS16Quad(p2b6, 1);
        __as__11_GXColorS10FRC11_GXColorS10(&lbl_eu_806645A0, &c8);
        *(s16*)((char*)&lbl_eu_806645A8 + 6) = *(s16*)((char*)&lbl_eu_80664598 + 6);
        *(s16*)((char*)&lbl_eu_806645B0 + 6) = *(s16*)((char*)&lbl_eu_806645A0 + 6);

        nw4r::lyt::Pane* p25b = root->FindPaneByName(&lbl_eu_805063BC[0x25b], true);
        CItemBoxGXColor c9 = PaneMatGetS16Quad(p25b, 0);
        __as__11_GXColorS10FRC11_GXColorS10(&lbl_eu_806645B8, &c9);
        CItemBoxGXColor c10 = PaneMatGetS16Quad(p25b, 1);
        __as__11_GXColorS10FRC11_GXColorS10(&lbl_eu_806645C0, &c10);
        *(s16*)((char*)&lbl_eu_806645C8 + 6) = *(s16*)((char*)&lbl_eu_806645B8 + 6);
        *(s16*)((char*)&lbl_eu_806645D0 + 6) = *(s16*)((char*)&lbl_eu_806645C0 + 6);

        StampSlotTex();

        LayoutSetTextBoxFmtValue((nw4r::lyt::Layout*)mpLayout, &lbl_eu_805063BC[0x7f9],
                      BdatTouchStringCell(&lbl_eu_805063BC[0x130], &lbl_eu_805063BC[0x139], 6), 0);
        LayoutSetTextBoxFmtValue((nw4r::lyt::Layout*)mpLayout, &lbl_eu_805063BC[0x6d2],
                      BdatTouchStringCell(&lbl_eu_805063BC[0x130], &lbl_eu_805063BC[0x139], 9), 0);
        setLayoutTextBoxNumber((nw4r::lyt::Layout*)mpLayout, &lbl_eu_805063BC[0x17d], (u8)0);
        setLayoutTextBoxNumber((nw4r::lyt::Layout*)mpLayout, &lbl_eu_805063BC[0x188], (u8)0);
        setPaneVisible(root->FindPaneByName(&lbl_eu_805063BC[0x193], true), 1);

        // Cache the slot panes at +0x40..+0x8C for the renderer (retail keeps
        // the layout's root pane live in r28 across all 20 lookups).
        nw4r::lyt::Pane* cacheRoot = mpLayout->GetRootPane();
        mSlotPane0 = cacheRoot->FindPaneByName(&base[0x5dc], true);
        mSlotPane1 = cacheRoot->FindPaneByName(&base[0x736], true);
        mSlotPane2 = cacheRoot->FindPaneByName(&base[0x5e9], true);
        mSlotPane3 = cacheRoot->FindPaneByName(&base[0x6c6], true);
        mSlotPane4 = cacheRoot->FindPaneByName(&base[0x5f6], true);
        mSlotPane5 = cacheRoot->FindPaneByName(&base[0x742], true);
        mSlotPane6 = cacheRoot->FindPaneByName(&base[0x603], true);
        mSlotPane7 = cacheRoot->FindPaneByName(&base[0x75a], true);
        mSlotPane8 = cacheRoot->FindPaneByName(&base[0x610], true);
        mSlotPane9 = cacheRoot->FindPaneByName(&base[0x772], true);
        mSlotPane10 = cacheRoot->FindPaneByName(&base[0x61d], true);
        mSlotPane11 = cacheRoot->FindPaneByName(&base[0x74e], true);
        mSlotPane12 = cacheRoot->FindPaneByName(&base[0x637], true);
        mSlotPane13 = cacheRoot->FindPaneByName(&base[0x77e], true);
        mSlotPane14 = cacheRoot->FindPaneByName(&base[0x644], true);
        mSlotPane15 = cacheRoot->FindPaneByName(&base[0x78a], true);
        mSlotPane16 = cacheRoot->FindPaneByName(&base[0x62a], true);
        mSlotPane17 = cacheRoot->FindPaneByName(&base[0x766], true);
        mSlotPane18 = cacheRoot->FindPaneByName(&base[0x651], true);
        mSlotPane19 = cacheRoot->FindPaneByName(&base[0x796], true);

        Activate();
        mFileHandle1 = 0;
        mRegion1.validateHeap();
        return true;
    }
    if (mFileHandle2 == file->mFileHandle) {
        mRegion2.createRegion(mtl::MemManager::getHandleMEM2(), 0x100,
                                      &lbl_eu_805063BC[0x804], 1);
        Class_8045F858 host(&mRegion2);
        u8* data = (u8*)mFileHandle2->getData();
        mtl::MemManager::setMemInitFlag(false);
        nw4r::lyt::ArcResourceAccessor* acc2 = createArcResourceAccessor__10CLibLayoutFv();
        mTexArcResAcc = acc2;
        acc2->Attach(data, &lbl_eu_805063BC[0x531]);
        Activate();
        mFileHandle2 = 0;
        mRegion2.validateHeap();
        return true;
    }
    return false;
}
#pragma pop

// Retail __ct__CItemBoxInfo uses the stmw/lmw frame (MWCC optimize_for_size
// prologue merge) with arg2/arg3 saved in r30/r31; plain -O4,p splits the
// saves into separate stw's. The trailing resetCItemBox__Fv call is a real
// bl (the callee is __declspec(noinline)).
#pragma push
#pragma optimize_for_size on
CItemBoxInfo::CItemBoxInfo(u8 kind, u16 mode)
    : mFileHandle1(NULL), mFileHandle2(NULL), mArcResAcc(NULL), mTexArcResAcc(NULL), mpLayout(NULL),
      mAnimIn(NULL), mAnimInfoIn(NULL),
      mSlotPane0(NULL), mSlotPane1(NULL), mSlotPane2(NULL), mSlotPane3(NULL), mSlotPane4(NULL),
      mSlotPane5(NULL), mSlotPane6(NULL), mSlotPane7(NULL), mSlotPane8(NULL), mSlotPane9(NULL),
      mSlotPane10(NULL), mSlotPane11(NULL), mSlotPane12(NULL), mSlotPane13(NULL), mSlotPane14(NULL),
      mSlotPane15(NULL), mSlotPane16(NULL), mSlotPane17(NULL), mSlotPane18(NULL), mSlotPane19(NULL),
      mActive(0), mState(0), mCurrent(0), mVisible(1),
      mKind(kind), mMode(mode), mFlagAE(0) {
    mPage.Reset();
}

__declspec(noinline) void CItemBoxPage::Reset() {}
#pragma pop

#pragma push
#pragma optimize_for_size on
CItemBoxInfo::~CItemBoxInfo() {}
#pragma pop

// Retail __ct__CItemBoxInfo2 uses the stmw/lmw frame
// prologue merge) with arg2/arg3 saved in r30/r31; plain -O4,p splits the
// saves into separate stw's. The trailing resetCItemBox__Fv call is a real
// bl (the callee is __declspec(noinline)).
#pragma push
#pragma optimize_for_size on
CItemBoxInfo2::CItemBoxInfo2(u8 kind, u16 mode)
    : mFileHandle1(NULL), mFileHandle2(NULL), mArcResAcc(NULL), mTexArcResAcc(NULL), mpLayout(NULL),
      mAnimIn(NULL), mAnimInfoIn(NULL),
      mSlotPane0(NULL), mSlotPane1(NULL), mSlotPane2(NULL), mSlotPane3(NULL), mSlotPane4(NULL),
      mSlotPane5(NULL), mSlotPane6(NULL), mSlotPane7(NULL), mSlotPane8(NULL), mSlotPane9(NULL),
      mSlotPane10(NULL), mSlotPane11(NULL), mSlotPane12(NULL), mSlotPane13(NULL), mSlotPane14(NULL),
      mSlotPane15(NULL), mSlotPane16(NULL), mSlotPane17(NULL), mSlotPane18(NULL), mSlotPane19(NULL),
      mActive(0), mState(0), mCurrent(0), mVisible(1),
      mKind(kind), mMode(mode) {
    mPage.Reset();
}
#pragma pop

// Retail __dt__13CItemBoxInfo2Fv uses the stmw/lmw frame (MWCC
// optimize_for_size prologue merge); plain -O4,p splits the r30/r31 saves
// into separate stw's. Same pattern as the matched kyoshin dtors.
#pragma push
#pragma optimize_for_size on
CItemBoxInfo2::~CItemBoxInfo2() {}
#pragma pop

// Retail loadItemBox2Files uses the stmw/lmw frame (MWCC optimize_for_size
// prologue merge) like the sibling ItemBox1/2 dtors.
#pragma push
#pragma optimize_for_size on
void CItemBoxInfo2::LoadFiles() {
    mtl::ALLOC_HANDLE handle = mtl::MemManager::getHandleMEM2();
    mFileHandle1 = CDeviceFile::readFile(handle, &lbl_eu_805063BC[0x8e], reinterpret_cast<IWorkEvent*>(this), 0, 0);
    handle = mtl::MemManager::getHandleMEM2();
    mFileHandle2 = CDeviceFile::readFile(handle, &lbl_eu_805063BC[0xa6], reinterpret_cast<IWorkEvent*>(this), 0, 0);
}
#pragma pop


void CItemBoxInfo2::DrawLayout(nw4r::lyt::DrawInfo* drawInfo) {
    if (mActive != 0) {
        drawLayout((nw4r::lyt::Layout*)mpLayout, drawInfo, 0, 1);
    }
}

// Retail closeItemBox2 (CItemBoxInfo2 teardown) keeps a plain single-save
// frame (only r31 = `this` survives the calls; zeros are re-loaded per store),
// so unlike closeItemBox1 it must NOT use optimize_for_size, and it does not
// clear the 0x2C/0x30 accessor/resource pointers after releasing them.
void CItemBoxInfo2::UnloadFiles() {
    closeFileHandle(&mFileHandle1);
    closeFileHandle(&mFileHandle2);
    void* layout = mpLayout;
    mActive = 0;
    if (layout != 0) {
        // Layout deleting destructor via plain delete (see closeItemBox1).
        delete (nw4r::lyt::Layout*)layout;
        mpLayout = 0;
    }
    mAnimIn = 0;
    mAnimInfoIn = 0;
    releaseArcResourceAccessor(mArcResAcc);
    releaseArcResourceAccessor(mTexArcResAcc);
    mRegion1.deleteRegion();
    mRegion2.deleteRegion();
}


#pragma push
#pragma auto_inline off
void CItemBoxInfo2::Open() {
    if (mState != 0) return;
    mState = 1;
    mVisible = 0;
    StampSlotTex();
}
#pragma pop

void CItemBoxInfo2::Close() {
    if (mState == 3) {
        mState = 4;
        mVisible = 0;
    }
}

// Retail renderItemBox2ByKind: ItemBox2 info dispatch. Zeroes the slot VEC3 block,
// renders the base panel, derives the item id (or the candidate's own), then
// dispatches on the item category to the per-category renderers. Ends by
// formatting a header string into a FixStr and pushing it to the layout.
// stmw/lmw frame (optimize_for_size prologue merge).
#pragma push
#pragma optimize_for_size on
void CItemBoxInfo::SetItem(u16 arg2, void* arg3, u16 arg4) {
    mPage.Clear();
    if (mKind == 4) {
        SetupDetail(arg2, arg3, arg4);
    }
    CItemData* item = arg3 != NULL ? (CItemData*)arg3 : NULL;
    u16 id = item != NULL ? (u16)(item->field_00 >> 20) : 0;
    u16 slot = mKind == 4 ? id : arg2;
    int kind = (u8)BdatGetItemType(slot);
    if (item != NULL && (IsSkillItem(item) != 0 || isItemBoxType9Flagged(item) != 0)) {
        kind = 9;
    }
    switch (kind) {
    case 2:
        RenderKind2(slot, arg3, (u16)(mKind > 2 ? (s32)arg2 : -1));
        break;
    case 3:
        RenderKind3(slot, arg3, (u16)(mKind > 2 ? (s32)arg2 : -1));
        break;
    case 4:
    case 5:
    case 6:
    case 7:
    case 8:
        RenderDetail(slot, arg3, (u16)(mKind > 2 ? (s32)arg2 : -1));
        break;
    case 9:
        RenderSlotList(slot, arg3);
        break;
    case 10:
        RenderLabels(slot);
        break;
    case 13:
        RefreshSlotRows(slot);
        break;
    default:
        SetupSlotPanes();
        ResetRowPanes();
        ClearTripleRows();
        ApplySelColors();
        ClearQuadTexts();
        ClearPairTexts();
        ClearSlotTexts();
        break;
    }
    char* label = BdatTouchStringCell(&lbl_eu_805063BC[0x130], &lbl_eu_805063BC[0x139], 3);
    // The bool ctor clears inline (mString[0]=0 / mLength=0) exactly like the
    // retail stb/stw pair; the default ctor would emit a __ct__ call instead.
    ml::FixStr<32> text(true);
    text.format(&lbl_eu_805063BC[0x13e], CItemBlock_getPtr20E8(), label);
    LayoutSetTextBoxFmtValue((nw4r::lyt::Layout*)mpLayout, &lbl_eu_805063BC[0x143],
                  text.c_str(), NULL);
}
#pragma pop

#pragma push
#pragma optimize_for_size on
void CItemBoxInfo2::SetItem(u16 arg2, void* arg3, u16 arg4, u32 arg5) {
    mPage.Clear();
    SetupDetail(arg2, arg3, arg4, arg5);
    CItemData* item = arg3 != NULL ? (CItemData*)arg3 : NULL;
    u16 id = item != NULL ? (u16)(item->field_00 >> 20) : 0;
    if (mKind == 4) arg2 = id;
    int kind = (u8)BdatGetItemType(arg2);
    if (item != NULL && (IsSkillItem(item) != 0 || isItemBoxType9Flagged(item) != 0)) kind = 9;
    switch (kind) {
    case 2:
        RenderKind2(arg2, arg3, arg4);
        break;
    case 3:
        RenderKind3(arg2, arg3, arg4);
        break;
    case 4:
    case 5:
    case 6:
    case 7:
    case 8:
        RenderDetail(arg2, arg3, arg4);
        break;
    case 9:
        RenderSlotList((void*)(u32)arg2, arg3);
        break;
    case 10:
        RenderLabels(arg2);
        break;
    case 13:
        RefreshSlotRows(arg2);
        break;
    default:
        SetupSlotPanes();
        ResetRowPanes();
        ClearTripleRows();
        ApplySelColors();
        ClearQuadTexts();
        ClearPairTexts();
        ClearSlotTexts();
        break;
    }
    char* s = BdatTouchStringCell((char*)&lbl_eu_805063BC[0x130], (char*)&lbl_eu_805063BC[0x139], 3);
    // Not named textBuffer: the textBuffer raw-storage macro from
    // setupItemBoxDetail is still in scope until its #undef below.
    ml::FixStr<32> caption(true);
    caption.format(&lbl_eu_805063BC[0x13e], CItemBlock_getPtr20E8(), s);
    LayoutSetTextBoxFmtValue((nw4r::lyt::Layout*)mpLayout, &lbl_eu_805063BC[0x143], caption.c_str(), 0);
}
#pragma pop
#pragma push
#pragma optimize_for_size on
void CItemBoxInfo2::SetNamedText(int index, char* text) {
    char buf[0x20];
    sprintf(buf, &lbl_eu_805063BC[0x151], index);
    LayoutSetTextBoxFmtValue((nw4r::lyt::Layout*)mpLayout, buf, text, 0);
}
#pragma pop

// Retail calcItemBox2PaneVec (CItemBoxInfo2 variant) uses the same stmw/lmw frame.
#pragma push
#pragma optimize_for_size on
nw4r::math::VEC3 CItemBoxInfo2::GetSlotPos(u32 arg2) {
    nw4r::math::VEC3 pos;
    char buf[0x20];
    // Same declaration-order trick as calcItemBoxPaneVec: `first` before `child`
    // so child colors into dead this's r30 and first reuses base's r31.
    nw4r::lyt::Pane* first;
    nw4r::lyt::Pane* second;
    nw4r::lyt::Pane* child;
    char* base = lbl_eu_805063BC;
    sprintf(buf, base + 0x161, arg2 + 1);
    child = ((LayoutRootPaneView*)mpLayout)->rootPane10;
    first = child->FindPaneByName(base + 0x16e, true);
    second = child->FindPaneByName(buf, true);
    ScalePaneWorldPos(&pos, second, first, child);
    return pos;
}
#pragma pop
#pragma push
#pragma auto_inline off
void CItemBoxInfo2::OpenPhase1() {
    if (advanceAnimTransform(mAnimIn, 1.0f) != 0) {
        (mpLayout)->SetAnimationEnable(mAnimIn, false);
        (mpLayout)->SetAnimationEnable(mAnimInfoIn, true);
        mState = 2;
    }
}

void CItemBoxInfo2::OpenPhase2() {
    if (advanceAnimTransform((nw4r::lyt::AnimTransform*)mAnimInfoIn, 1.0f) != 0) {
        mState = 3;
        mVisible = 1;
    }
}

void CItemBoxInfo2::ClosePhase1() {
    if (AnimRewindFrame((nw4r::lyt::AnimTransform*)mAnimInfoIn, 1.0f) != 0) {
        ((nw4r::lyt::Layout*)mpLayout)->SetAnimationEnable((nw4r::lyt::AnimTransform*)mAnimInfoIn, false);
        ((nw4r::lyt::Layout*)mpLayout)->SetAnimationEnable((nw4r::lyt::AnimTransform*)mAnimIn, true);
        mState = 5;
    }
}
void CItemBoxInfo2::ClosePhase2() {
    if (AnimRewindFrame((nw4r::lyt::AnimTransform*)mAnimIn, 1.0f) != 0) {
        mVisible = 1;
        mState = 0;
    }
}
#pragma pop

// Retail buildItemBox2CmpRecord uses the _savegpr_20 + f29/f30/f31 frame (MWCC
// optimize_for_size prologue merge). On a null item it zeroes the 0x34-byte
// record and copies it out; otherwise it builds the record like buildItemBoxCmpRecord
// (same lookup chain, clamp and per-slot flag banks) and copies it out.
#pragma push
#pragma optimize_for_size on
#pragma auto_inline off
__declspec(noinline) CItemBoxCompRecord CItemBoxInfo2::BuildCmpRecord(void* arg3) {
    CItemBoxCompRecord rec;
    // 0x43300000 integer->double conversion scratch; retail stores both high
    // words before the null-item branch.
    union { double d; u32 w[2]; } u1;
    union { double d; u32 w[2]; } u2;
    u1.w[0] = 0x43300000;
    u2.w[0] = 0x43300000;
    // Retail emits the 0x34-byte pair-copy once per branch (null path copies
    // immediately and returns; the build path has its own copy at the end).
    if (arg3 == NULL) {
        memset(&rec, 0, 0x34);
        return rec;
    }
    {
        void* global = lbl_eu_806640F4;
        BdatGetItemType((u32)arg3);
        u32 v2 = BdatGetItemId((u32)arg3);
        char* base = (char*)&lbl_eu_805063BC;
        rec.s2C = (u16)BdatGetU16Direct((const void*)global, (const char*)((const void*)(base + 0x19c)), (int)(u16)v2);
        rec.s2E = (u16)BdatGetU16Direct((const void*)global, (const char*)((const void*)(base + 0x1a4)), (int)(u16)v2);
        rec.s30 = (u8)BdatGetU8Direct((const void*)global, base + 0x1ab, (u16)v2);
        rec.s32 = (u8)BdatGetU8Direct((const void*)global, base + 0x1b3, (u16)v2);
        rec.s34 = (u8)BdatGetU8Direct((const void*)global, base + 0x1bb, (u16)v2);
        u1.w[1] = (u8)BdatGetU8Direct((const void*)global, base + 0x1c3, (u16)v2);
        rec.f38 = (f32)(u1.d - 4503599627370496.0) / 10.0f;
        rec.s3C = (u8)BdatGetU8Direct((const void*)global, base + 0x1c9, (u16)v2);
        if (BdatGetU8Direct((const void*)global, base + 0x1d2, (u16)v2) & 4) {
            void* lookup = CtrlObjectParam_GetCharEntry(1);
            u32 r = CtrlObjectParam_GetArtsDataWord(lookup);
            u2.w[1] = (u32)((s32)rec.s2C * (s32)r) ^ 0x80000000;
            rec.s2C = (s16)(s32)(1.4f * (f32)(u2.d - 4503601774854144.0));
            u32 r2 = CtrlObjectParam_GetArtsDataWord(lookup);
            u1.w[1] = (u32)((s32)rec.s2E * (s32)r2) ^ 0x80000000;
            rec.s2E = (s16)(s32)(1.5f * (f32)(u1.d - 4503601774854144.0));
            if (rec.s2C > 999) rec.s2C = 999;
            if (rec.s2E > 999) rec.s2E = 999;
        }
        // MWCC hoists these loop-invariant .sdata2 loads into callee-saved
        // f29/f30 when the globals are referenced directly.
        for (u32 i = 0; (u8)i < (u8)code80135FDC_getByte_64077(); i++) {
            u8 slot = (u8)GetCollectedFlagByte((u8)i);
            char buf[0x20];
            sprintf(buf, base + 0x1d7, slot);
            u8 v = (u8)BdatGetU8Direct((const void*)global, (const char*)buf, (u16)v2);
            // +0x16 flag bank: slot has a resolvable entry at all.
            rec.flags[0][(u8)i] = (v != 0);
            if (rec.flags[0][(u8)i] == 0) continue;
            // Accumulate the slot's stat contribution into the ratio total.
            rec.f38 += GetFloatTableEntry(slot);
            void* lookup = CtrlObjectParam_GetCharEntry(slot);
            void* item = CItem_kindPtr(2, *(s16*)((u8*)lookup + 0x26));
            if (item == NULL || *(u32*)item == 0) continue;
            u16 cat = (u16)BdatGetItemId(*(u32*)item >> 20);
            s16 n1 = (u16)BdatGetU16Direct((const void*)global, (const char*)((const void*)(base + 0x19c)), (int)cat);
            s16 n2 = (u16)BdatGetU16Direct((const void*)global, (const char*)((const void*)(base + 0x1a4)), (int)cat);
            u2.w[1] = (u8)BdatGetU8Direct((const void*)global, base + 0x1c3, cat);
            f32 slotRatio =
                BlendFloatAvgScale(n1, n2, 0, (f32)(u2.d - 4503599627370496.0) / 10.0f);
            u8 v1ab = (u8)BdatGetU8Direct((const void*)global, base + 0x1ab, cat);
            u8 v1b3 = (u8)BdatGetU8Direct((const void*)global, base + 0x1b3, cat);
            f32 curRatio = rec.f28;
            if (curRatio > slotRatio) rec.flags[1][(u8)i] = 1;
            else if (curRatio < slotRatio) rec.flags[1][(u8)i] = 2;
            else rec.flags[1][(u8)i] = 0;
            if ((s32)rec.s30 > (s32)v1ab) rec.flags[2][(u8)i] = 1;
            else if ((s32)rec.s30 < (s32)v1ab) rec.flags[2][(u8)i] = 2;
            else rec.flags[2][(u8)i] = 0;
            if ((s32)rec.s32 > (s32)v1b3) rec.flags[3][(u8)i] = 1;
            else if ((s32)rec.s32 < (s32)v1b3) rec.flags[3][(u8)i] = 2;
            else rec.flags[3][(u8)i] = 0;
        }
        rec.f28 = BlendFloatAvgScale((u32)rec.s2C, (u32)rec.s2E, 0, rec.f38);
    }
    // 0x34-byte copy: pair-copy with s[1]/s[2] accesses reproduces the retail
    // mtctr lwzu/stwu 8-byte-pair loop (li r0,6 + 4-byte tail).
    return rec;
}
#pragma pop
// Retail buildItemBox2SlotFlags builds the 0x1C-byte comparison record like
// buildItemBoxSlotFlags, with two differences: the per-slot flag is the boolean
// (name resolved ? 1 : 0) via subic/subfe, and when the category check
// (0x85/0x84) SUCCEEDS the value-switch path still runs (the switch body is
// shared through the else-block fall-through, so it is emitted once).
// buildItemBoxSlotFlags, with two differences: the per-slot flag is the boolean
// (name resolved ? 1 : 0) via subic/subfe, and when the category check
// (0x85/0x84) SUCCEEDS the value-switch path still runs (the switch body is
// shared through the else-block fall-through, so it is emitted once).
// Retail uses the stmw/lmw frame and keeps the mtctr copy loops (the whole
// retail unit is size-optimized), so the function is wrapped in
// optimize_for_size like the other matched stmw-frame functions here.
#pragma push
#pragma optimize_for_size on
__declspec(noinline) CItemBoxSlotFlags CItemBoxInfo2::BuildSlotFlags(u32 item) {
    CItemBoxSlotFlags rec;
    if (item == 0) {
        memset(&rec, 0, 0x1C);
        return rec;
    }
    void* global = lbl_eu_806640F8;
    u32 type = BdatGetItemType(item);
    u16 id = BdatGetItemId(item);
    char* base = (char*)&lbl_eu_805063BC;
    rec.v[0] = (u8)BdatGetU8Direct((const void*)global, base + 0x1ab, id);
    rec.v[1] = (u8)BdatGetU8Direct((const void*)global, base + 0x1b3, id);
    rec.v[2] = (u8)BdatGetU8Direct((const void*)global, base + 0x1e2, id);
    u32 reqKind = BdatGetU8Direct((const void*)lbl_eu_806640F8, base + 0x1eb, id);
    for (u32 i = 0; (u8)i < (u8)code80135FDC_getByte_64077(); i++) {
        u32 slot = GetCollectedFlagByte((u8)i);
        char buf[0x20];
        sprintf(buf, base + 0x1f4, (u8)slot);
        rec.equipable[(u8)i] = (u8)BdatGetU8Direct((const void*)lbl_eu_806640F8, (const char*)buf, id) != 0;
        if (rec.equipable[(u8)i] == 0) continue;
        CItemBoxCharEntryView* member = (CItemBoxCharEntryView*)CtrlObjectParam_GetCharEntry((u8)slot);
        void* counters = member->counterArea;
        if ((u8)reqKind == 3) {
            if (Counter_TestBit(counters, 0x85) == 0) rec.equipable[(u8)i] = 0;
        } else if ((u8)reqKind == 2) {
            if (Counter_TestBit(counters, 0x84) == 0) rec.equipable[(u8)i] = 0;
        } else {
            s16 value = -1;
            switch ((u16)type) {
                case 4: value = member->slotIds[0]; break;
                case 5: value = member->slotIds[1]; break;
                case 6: value = member->slotIds[2]; break;
                case 7: value = member->slotIds[3]; break;
                case 8: value = member->slotIds[4]; break;
            }
            if (value >= 0) {
                void* equipped = CItem_kindPtr((u16)type, value);
                if (equipped != NULL && *(u32*)equipped != 0) {
                    u32 cat = BdatGetItemId(*(u32*)equipped >> 20);
                    u8 n1 = (u8)BdatGetU8Direct((const void*)global, base + 0x1ab, cat);
                    u8 n2 = (u8)BdatGetU8Direct((const void*)global, base + 0x1b3, cat);
                    if (rec.v[0] > n1) rec.cmp0[(u8)i] = 1;
                    else if (rec.v[0] < n1) rec.cmp0[(u8)i] = 2;
                    else rec.cmp0[(u8)i] = 0;
                    if (rec.v[1] > n2) rec.cmp1[(u8)i] = 1;
                    else if (rec.v[1] < n2) rec.cmp1[(u8)i] = 2;
                    else rec.cmp1[(u8)i] = 0;
                }
            }
        }
    }
    return rec;
}
#pragma pop
// Retail buildItemBox2NameRec (ItemBox2 twin of buildItemBoxNameRec): builds the 0xA4-byte
// item-name record (count byte at +0, label string pointer, name buffer, two
// state bytes, equip colour, text buffer) then copies it out. Uses the
// stmw/lmw frame (optimize_for_size prologue merge). The label table lives
// in a 0x34-byte stack region that is later copied into the record tail, so
// the 6-pair mtctr copy loop reads it directly.
#pragma push
#pragma optimize_for_size on
__declspec(noinline) CItemBoxNameRecord CItemBoxInfo2::BuildNameRec(void* data, void* arg3) {
    void* item = arg3 != NULL ? arg3 : NULL;
    // Retail hoists this global into a register (r26) across all the calls;
    // a local copy reproduces the single sda21 load in the prologue.
    void* gd8 = lbl_eu_806640D8;
    u32 cat;
    u16 count;
    if (arg3 == NULL) {
        BdatGetItemType((u32)data);
    }
    if (arg3 != NULL) {
        CItemImpl* inst = CItem_initItemImplInstances((CItemData*)item);
        cat = inst->vf54((CItemData*)item);
    } else {
        cat = BdatGetItemId((u32)data);
    }
    if (arg3 != NULL) {
        CItemImpl* inst = CItem_initItemImplInstances((CItemData*)item);
        count = (u16)inst->vf08((CItemData*)item);
    } else {
        count = (u8)BdatGetU8Direct((const void*)lbl_eu_806640EC, &lbl_eu_805063BC[0x1f9], (u32)data);
    }
    // base is only materialized after the cat/count joins (retail lis/addi at
    // the join point, not in the prologue).
    char* base = (char*)&lbl_eu_805063BC;
    CItemBoxNameRecord rec;
    rec.count = (u8)count;
    rec.str = (u32)BdatTouchStringCell(base + 0x130, base + 0x139, 0x1e - ((u8)count - 1));
    char* s2 = BdatTouchStringCell(base + 0x202, base + 0x139, 0xf);
    if (arg3 != NULL) {
        CItemImpl* inst = CItem_initItemImplInstances((CItemData*)item);
        sprintf(rec.name, base, inst->vf90((CItemData*)item));
    } else {
        sprintf(rec.name, base + 0x18, s2);
    }
    rec.e0 = (u8)BdatGetU8Direct((const void*)gd8, base + 0x3, (u16)cat);
    rec.e1 = (u8)BdatGetU8Direct((const void*)gd8, base + 0x20b, (u16)cat);
    rec.color = 0xFFFFFFFF;
    switch (rec.e1) {
        case 4: rec.color = 0xFF00FFFF; break;
        case 5: rec.color = 0x0000FFFF; break;
        case 6: rec.color = 0xFFFFFFFF; break;
        case 7: rec.color = 0x0000FFFF; break;
        case 8: rec.color = 0x00FFFFFF; break;
        case 9: rec.color = 0x775544FF; break;
    }
    sprintf(rec.text, base + 0x18, ((char*(*)(void*, const char*, u16))&BdatGetPtrDirect)(gd8, base + 0xc, (u16)cat));
    u32 wide = 1;
    if (getLanguage__9CDeviceSCFv() != 3) {
        if (getLanguage__9CDeviceSCFv() != 2) {
            wide = 0;
        }
    }
    // Marker-replacement scan over rec.text: a '$' followed by '1'/'2' is
    // replaced by a localized string (the '1' marker switches on the equip
    // state byte, the '2' marker copies the static label-pointer table into
    // the local array and formats the count-indexed label); every other char
    // advances the scan Shift-JIS-aware (lead-byte ranges advance 2). After
    // a marker, the remaining text is spliced after the replacement.
    char* labels = (char*)lbl_eu_80506380;
    u32 labels2[4];
    char buf28[0x20];
    char buf48[0x20];
    char* cur = rec.text;
    while (*cur != 0) {
        if (*cur == '$') {
            memset(buf28, 0, 0x20);
            cur++;
            if (*cur == '1') {
                cur++;
                switch (rec.e0) {
                    case 1:
                        sprintf(buf28, base + 0x1b, rec.name);
                        break;
                    case 2:
                        sprintf(buf28, base + 0x1b, rec.name);
                        break;
                    case 3:
                        if (wide != 0) {
                            sprintf(buf28, base + 0x30, rec.name);
                        } else {
                            sprintf(buf28, base + 0x48, rec.name);
                        }
                        break;
                    case 4:
                        if (wide != 0) {
                            sprintf(buf28, base + 0x30, rec.name);
                        } else {
                            sprintf(buf28, base + 0x48, rec.name);
                        }
                        break;
                    default:
                        break;
                }
            } else if (*cur == '2') {
                // 0x24-byte copy of the static label-pointer table into the
                // local array (retail mtctr 4-pair loop + 4-byte tail).
                {
                    u32* s = (u32*)labels - 1;
                    u32* d = (u32*)labels2 - 1;
                    for (u32 k = 0; k < 4; k++) {
                        d[1] = s[1];
                        d[2] = s[2];
                        s += 2;
                        d += 2;
                    }
                    d[1] = s[1];
                }
                cur++;
                u8 name = BdatGetU8Direct((const void*)gd8, (char*)labels2[rec.count], (u16)cat);
                if (wide != 0) {
                    sprintf(buf28, base + 0x5f, (u8)name);
                } else {
                    sprintf(buf28, base + 0x77, (u8)name);
                }
            }
            // strlen of buf28 (post-increment counter: retail counts len+1).
            char* p = buf28;
            u32 len = 0;
            while (*p != 0) {
                len++;
                p++;
            }
            sprintf(buf48, base + 0x18, cur);
            cur = cur - 2;
            sprintf(cur, base + 0x18, buf28);
            cur = cur + len - 1;
            sprintf(cur, base + 0x18, buf48);
        } else {
            s8 sc = (s8)*cur;
            if ((sc >= 0x81 && sc <= 0x9f) || (sc >= 0xe0 && sc <= 0xef)) {
                cur += 2;
            } else {
                cur += 1;
            }
        }
    }
    // 0xA4-byte copy: pair-copy with s[1]/s[2] accesses reproduces the retail
    // mtctr lwzu/stwu 8-byte-pair loop (li r0,0x14 + 4-byte tail).
    return rec;
}
#pragma pop

#pragma push
#pragma optimize_for_size on
// Twin of getItemBoxLabelRec (see comment there). dont_inline keeps IPA from
// folding this body into renderItemBox2Labels.
__declspec(noinline) CItemBoxLabelRec CItemBoxInfo2::GetLabelRec(u32 item) {
    CItemBoxLabelRec r;
    void* tbl = lbl_eu_80664104;
    BdatGetItemType(item);
    // cat stays a raw u16 in a register; each call site re-applies the
    // u16->u32 argument conversion (retail clrlwi per site, no shared temp).
    u16 cat = BdatGetItemId(item);
    // base is anchored at the first lookup so its definition schedules after
    // the cat move (retail order: or r30,r3 then lis/addi).
    r.a = (u8)BdatGetU8Direct((const void*)tbl, (char*)&lbl_eu_805063BC + 0x214, cat);
    char* base = (char*)&lbl_eu_805063BC;
    r.b = BdatTouchStringCell(base + 0x219, base + 0x139, r.a);
    // The label ID is also the row key passed to BdatGetPtrDirect.
    r.c = (u8)BdatGetU8Direct((const void*)tbl, base + 0x225, cat);
    if (r.c == 0x1A) {
        r.d = BdatTouchStringCell(base + 0x219, base + 0x139, 0x14);
    } else {
        r.d = BdatGetPtrDirect(lbl_eu_806640A8, base + 0x139, r.c);
    }
    // Returned by value: the caller's hidden result pointer receives the
    // record as four lwz/stw pairs.
    return r;
}
#pragma pop

// Retail buildItemBox2SelTable (ItemBox2 variant of buildItemBoxSelRec): fills the 8-byte
// slot table (flag byte + per-slot selected flags) and a flag2 result from the
// item-table lookup, then writes all three to the info block. Retail uses the
// stmw/lmw frame (optimize_for_size prologue merge).
#pragma push
#pragma optimize_for_size on
__declspec(noinline) CItemBoxSelRec CItemBoxInfo2::BuildSelRec(u32 item) {
    void* global = lbl_eu_80664110;
    u16 v1 = BdatGetItemType(item);
    u16 v2 = BdatGetItemId(item);
    CItemBoxSelRec rec;
    rec.category = (u8)BdatGetU8Direct((const void*)global, (char*)&lbl_eu_805063BC[0x22b], v2);
    // count stays u32: retail keeps the raw call result (r28) and masks at
    // each (u8)count use (loop mask hoisted to r29).
    u8 count = BdatGetU8Direct((const void*)global, (char*)&lbl_eu_805063BC[0x237], v2);
    for (u32 i = 0; (u8)i < (u8)code80135FDC_getByte_64077(); i++) {
        u8 a = (u8)GetCollectedFlagByte((u8)i);
        rec.selected[(u8)i] = a == (u8)count;
    }
    u8 key = BdatGetU8Direct((const void*)global, (char*)&lbl_eu_805063BC[0x23f], v2);
    u8 val = BdatGetU8ByTableKey((char*)&lbl_eu_805063BC[0x248], (char*)&lbl_eu_805063BC[0x250], (u8)key);
    void* lookup = CtrlObjectParam_GetCharEntry((u8)count);
    u8 cat = (u8)CtrlObjectParam_GetCurrentRowKey();
    rec.flag2 = 0;
    u8* entry = (u8*)lookup + cat * 0x49 + val * 2;
    switch (rec.category) {
        case 1: if (entry[0xE8] != 0) rec.flag2 = 1; break;
        case 2: if ((entry[0xE9] >> 7) & 1) rec.flag2 = 1; break;
        case 3: if ((entry[0xE9] >> 6) & 1) rec.flag2 = 1; break;
    }
    return rec;
}
#pragma pop
// Retail buildItemBox2SlotRec fills a 0x24-byte slot record (count byte, string ptr,
// per-item text ptrs at +8, per-item values at +0x1C, counter at +0x21) then
// copies it out. The BdatGetItemType/80139358 results are discarded (dead calls
// kept because MWCC never elides calls). Param 2 (info) is unused in retail.
#pragma push
#pragma optimize_for_size on
__declspec(noinline) CItemBoxSlotRecord CItemBoxInfo2::BuildSlotRec(u32 item, void* arg4) {
    CItemBoxSlotRecord rec;
    void* p = arg4 != 0 ? arg4 : 0;
    BdatGetItemType(item);
    BdatGetItemId(item);
    u32 v08 = CItem_initItemImplInstances((CItemData*)p)->vf08((CItemData*)p);
    rec.count = (u8)v08;
    rec.str = (u32)BdatTouchStringCell(&lbl_eu_805063BC[0x130], &lbl_eu_805063BC[0x139], 0x1e - ((u8)v08 - 1));
    rec.tail[1] = 0;
    for (u32 i = 0; i < 4; i++) {
        u32 n = (u16)CItem_initItemImplInstances((CItemData*)p)->vf4C((CItemData*)p, (u8)i);
        if ((s32)n > 0) {
            rec.text[rec.tail[1]] =
                (u32)((char*(*)(void*, char*, u32))&BdatGetPtrDirect)(lbl_eu_806640D8, &lbl_eu_805063BC[0x139], n);
            rec.vals[rec.tail[1]] = CItem_initItemImplInstances((CItemData*)p)->vf64((CItemData*)p, (u8)i);
            rec.tail[1]++;
        }
    }
    // Returned by value: the 0x24-byte copy to the hidden result pointer is
    // the retail mtctr 8-byte-pair loop (li r0,4 + 4-byte tail).
    return rec;
}
#pragma pop

// Render the item-box slot info (equip-category 2 items): for each slot
// (count from the slot-table byte getter) set up a labelled layout pane,
// per-slot vertex colours, and (when a selection applies) a texture/name tag.
// Retail keeps the stmw/lmw frame (optimize_for_size prologue merge) and the
// mtctr copy loop for the 0x34-byte comparison record.
#pragma push
#pragma optimize_for_size on
__declspec(noinline) void CItemBoxInfo2::RenderKind2(u16 arg1, void* arg2, u16 arg3) {
    SetupSlotPanes();
    // Declaration order follows the retail frame layout (high -> low):
    // copied record, pane-name/label buffers, source record, colour defaults.
    CItemBoxCompRecord local = BuildCmpRecord((void*)arg1);
    char paneName[0x20];
    char label[0x20];
    char* base = (char*)&lbl_eu_805063BC;
    for (u32 i = 0; (u8)i < (u8)code80135FDC_getByte_64077(); i++) {
        u8 flag = local.flags[0][(u8)i];
        u32 color = 0x777777FF;
        if (flag != 0) color = 0xFFFFFFFF;
        sprintf(paneName, base + 0x303, (u8)i + 1);
        PaneSetVtxColorByName((nw4r::lyt::Layout*)mpLayout, paneName, color);
        // Unselected rows keep only the alpha of the highlight colours.
        CItemBoxQuadColorView dimA;
        dimA.s[0] = dimA.s[1] = dimA.s[2] = 0;
        dimA.s[3] = mColorA.s[3];
        CItemBoxQuadColorView dimB;
        dimB.s[0] = dimB.s[1] = dimB.s[2] = 0;
        dimB.s[3] = mColorB.s[3];
        CItemBoxQuadColorView colors[2] = {
            flag ? mColorA : dimA,
            flag ? mColorB : dimB,
        };
        sprintf(paneName, &lbl_eu_805063BC[0x161], (u8)i + 1);
        nw4r::lyt::Pane* pane = mpLayout->GetRootPane()->FindPaneByName(paneName, true);
        if (pane != NULL) {
            void* obj = pane->GetMaterial();
            for (u32 j = 0; j < 2; j++) {
                setItemBoxVtxColor(obj, (u8)j, &colors[(u8)j]);
            }
        }
        if (flag != 0 && (arg2 != NULL || arg1 != 0)) {
            u8 slot = (u8)GetCollectedFlagByte((u8)i);
            if (slot != 0) {
                void* member = CtrlObjectParam_GetCharEntry(slot);
                // Fixed equip category 2: candidate value read at member+0x26.
                void* item = CItem_kindPtr(2, *(s16*)((u8*)member + 0x26));
                if (item != NULL && *(u32*)item != 0) {
                    if (arg2 == item || arg1 == (u32)(*(u32*)item >> 20)) {
                        sprintf(label, base + 0x30e, (u8)i + 1);
                        u8 otherSlot = (u8)GetCollectedFlagByte(arg3);
                        u32 tex;
                        if (slot == otherSlot) {
                            tex = (u32)mArcResAcc->GetResource(0x74696D67, base + 0x319, NULL);
                        } else {
                            tex = (u32)mArcResAcc->GetResource(0x74696D67, base + 0x32d, NULL);
                        }
                        if (tex == 0) {
                            tex = (u32)mArcResAcc->GetResource(0x74696D67, base + 0x341, NULL);
                        }
                        if (tex != 0) {
                            PaneSetTexPaletteByName((nw4r::lyt::Layout*)mpLayout, (const char*)label, (TPLPalette*)tex);
                        }
                    }
                }
            }
        }
    }
}
#pragma pop
// Retail renderItemBox2Detail uses the stmw/lmw frame (MWCC optimize_for_size
// prologue merge) and keeps the mtctr copy loop for the 0x1C-byte record
// copy, so it is wrapped in optimize_for_size like the other stmw-frame
// functions in this unit.
// dont_inline: retail keeps this out-of-line; MWCC would otherwise inline it
// into renderItemBox2ByKind (forward declaration makes the body visible).
// dont_inline is NOT used here: retail emits the record copy and colour
// staging as inline sequences, and renderItemBox2ByKind's auto_inline-off region
// already keeps this body out of its caller.
#pragma push
#pragma optimize_for_size on
// dont_inline: retail keeps this out-of-line; MWCC would otherwise inline it
// into renderItemBox2ByKind (forward declaration makes the body visible).
__declspec(noinline) void CItemBoxInfo2::RenderDetail(u16 arg2, void* arg3, u16 arg4) {
    ResetRowPanes();
    // Frame-slot order: record > buf > bufName > local > args > cols >
    // selB/selA (the staged colour temps; retail keeps its copies above the
    // record, ours sit lowest - offsets inside the staging block differ).
    char buf[0x20];
    char bufName[0x20];
    CItemBoxSlotFlags record = BuildSlotFlags(arg2);
    char* base = (char*)&lbl_eu_805063BC;
    // Hoisted halves: MWCC materializes these once (lis) and rebuilds each
    // constant at its use site with a single addi.
    u32 darkBase = 0x77770000;
    u32 tagHi = 0x74690000;
    for (u32 i = 0; (u8)i < (u8)code80135FDC_getByte_64077(); i++) {
        u8 flag = record.equipable[(u8)i];
        u32 color = darkBase + 0x77ff;  // 0x777777ff
        if (flag != 0) color = 0xFFFFFFFF;
        sprintf(buf, base + 0x303, (u8)i + 1);
        PaneSetVtxColorByName((nw4r::lyt::Layout*)mpLayout, buf, color);
        // Unselected rows keep only the alpha of the highlight colours.
        CItemBoxQuadColorView dimA;
        dimA.s[0] = dimA.s[1] = dimA.s[2] = 0;
        dimA.s[3] = mColorA.s[3];
        CItemBoxQuadColorView dimB;
        dimB.s[0] = dimB.s[1] = dimB.s[2] = 0;
        dimB.s[3] = mColorB.s[3];
        CItemBoxQuadColorView colors[2] = {
            flag ? mColorA : dimA,
            flag ? mColorB : dimB,
        };
        sprintf(buf, &lbl_eu_805063BC[0x161], (u8)i + 1);
        nw4r::lyt::Pane* pane = mpLayout->GetRootPane()->FindPaneByName(buf, true);
        if (pane != NULL) {
            void* obj = pane->GetMaterial();
            for (u32 j = 0; j < 2; j++) {
                setItemBoxVtxColor(obj, (u8)j, &colors[(u8)j]);
            }
        }
        if (flag != 0 && (arg3 != NULL || arg2 != 0)) {
            u8 slot = (u8)GetCollectedFlagByte((u8)i);
            if (slot != 0) {
                void* member = CtrlObjectParam_GetCharEntry(slot);
                u16 category = (u16)BdatGetItemType((u32)arg2);
                s16 value = -1;
                switch (category) {
                    case 4: value = *(s16*)((u8*)member + 0x1C); break;
                    case 5: value = *(s16*)((u8*)member + 0x1E); break;
                    case 6: value = *(s16*)((u8*)member + 0x20); break;
                    case 7: value = *(s16*)((u8*)member + 0x22); break;
                    case 8: value = *(s16*)((u8*)member + 0x24); break;
                }
                void* item = CItem_kindPtr(category, value);
                if (item != NULL && *(u32*)item != 0) {
                    if (arg3 == item || arg2 == (*(u32*)item >> 20)) {
                        sprintf(bufName, base + 0x30e, (u8)i + 1);
                        u32 tex;
                        if (slot == (u8)GetCollectedFlagByte(arg4)) {
                            tex = (u32)mArcResAcc->GetResource(tagHi + 0x6d67, base + 0x319, NULL);
                        } else {
                            tex = (u32)mArcResAcc->GetResource(tagHi + 0x6d67, base + 0x32d, NULL);
                        }
                        if (tex == 0) {
                            tex = (u32)mArcResAcc->GetResource(tagHi + 0x6d67, base + 0x341, NULL);
                        }
                        if (tex != 0) {
                            PaneSetTexPaletteByName((nw4r::lyt::Layout*)mpLayout, (const char*)bufName, (TPLPalette*)tex);
                        }
                    }
                }
            }
        }
    }
}
#pragma pop
// end renderItemBox2Detail
#pragma push
#pragma optimize_for_size on
// Retail refreshItemBox2SlotRows: refresh the item-box slot rows. Fetches the per-slot
// selection table via buildItemBox2SelTable into a stack record, sets the row label to
// the category name (length 0x2c vs 0x2b depending on flag2), then for each
// visible slot colours the pane (white when selected) and pushes two vertex
// colours - the item-specific pair from info+0x9c when selected, otherwise
// alpha-only defaults built from info+0xa2/0xaa - through setItemBoxVtxColor.
__declspec(noinline) void CItemBoxInfo2::RefreshSlotRows(u16 arg2) {
    ApplySelColors();
    ResetRowPanes();
    // Selection-colour staging: the aggregate ternaries below force MWCC to
    // materialize each selected 8-byte colour into a stack temp (retail
    // sp+0x68..0x74) before copying it into out[].
    char buf[0x20];
    CItemBoxColorBlock out[2];
    CItemBoxColorRaw cols[2];
    // 9-byte per-slot selection record: bytes [1..8] are the per-slot flags,
    // byte [8] also selects the row-height label size.
    CItemBoxSelRec local = BuildSelRec(arg2);
    char* base = (char*)&lbl_eu_805063BC;
    u32 rowHeight = 0x2c;
    if (local.flag2 != 0) rowHeight = 0x2b;
    char* label = BdatTouchStringCell(base + 0x130, base + 0x139, rowHeight);
    LayoutSetTextBoxFmtValue(mpLayout, base + 0x44f, label, 0);
    for (u32 i = 0; (u8)i < (u8)code80135FDC_getByte_64077(); i++) {
        u8 flag = local.selected[(u8)i];
        u32 color = 0x777777ff;
        if (flag != 0) color = 0xFFFFFFFF;
        sprintf(buf, base + 0x303, (u8)i + 1);
        PaneSetVtxColorByName(mpLayout, buf, color);
        s16 vA2 = mColorA.s[3];
        s16 vAA = mColorB.s[3];
        // Retail builds cols[1] first, then cols[0], each as three zero
        // halfwords plus the alpha from the this block.
        cols[1].r = 0; cols[1].g = 0; cols[1].b = 0; cols[1].a = vA2;
        cols[0].r = 0; cols[0].g = 0; cols[0].b = 0; cols[0].a = vAA;
        out[0].w[0] = 0;
        out[0].w[1] = 0;
        out[1].w[0] = 0;
        out[1].w[1] = 0;
        // Aggregate ternaries: selected 8-byte colour is staged through a
        // stack temp then copied as one block (retail lwz/stw pairs).
        out[0] = (flag != 0) ? *(CItemBoxColorBlock*)&mColorA
                             : *(CItemBoxColorBlock*)&cols[1];
        out[1] = (flag != 0) ? *(CItemBoxColorBlock*)&mColorB
                             : *(CItemBoxColorBlock*)&cols[0];
        sprintf(buf, base + 0x161, (u8)i + 1, cols[0]);
        nw4r::lyt::Pane* pane = mpLayout->GetRootPane()->FindPaneByName(buf, true);
        if (pane != NULL) {
            void* obj = pane->GetMaterial();
            for (u32 j = 0; j < 2; j++) {
                setItemBoxVtxColor(obj, (u8)j, &out[j]);
            }
        }
    }
}
#pragma pop
// Retail renderItemBox2Kind3 uses the stmw/lmw 0x200 frame (optimize_for_size
// prologue merge). Pipeline: two layout preps, then buildItemBox2NameRec fills a
// 0xA0-byte label record that is struct-copied to a local (20-pair mtctr
// loop) and stamped onto the fixed panes; the candidate item's name/category
// labels follow. The per-slot loop colours each row (red when unselected,
// white + the info-block vertex colours when selected) and, for slots whose
// six equip values resolve to the candidate item, stamps its texture.
#pragma push
#pragma optimize_for_size on
__declspec(noinline) void CItemBoxInfo2::RenderKind3(u16 arg2, void* arg3, u16 arg4) {
    ClearQuadTexts();
    ResetRowPanes();
    CItemBoxNameRecord vals = BuildNameRec((void*)(u32)arg2, arg3);
    char textBuf[0x20];
    char buf2[0x20];
    CItemBoxVertexColors cols;
    CItemBoxSlotEntry entry;

    // Retail materializes the pool base only after the record copy.
    char* base = (char*)&lbl_eu_805063BC;

    LayoutSetTextBoxFmtValue((nw4r::lyt::Layout*)mpLayout, base + 0x45a, (char*)vals.str, 0);
    if ((s8)vals.name[0] == 0x30) {
        LayoutSetTextBoxFmtValue((nw4r::lyt::Layout*)mpLayout, base + 0x466, base + 0x2aa, 0);
    } else {
        if (vals.e0 != 0) {
            if ((u8)(vals.e0 - 3) <= 1) {
                sprintf(textBuf, base + 0x419, vals.name,
                        BdatTouchStringCell(base + 0x130, base + 0x139, 0x21));
            } else {
                sprintf(textBuf, base + 0x41e, vals.name);
            }
        } else {
            sprintf(textBuf, base + 0x41e, vals.name);
        }
        LayoutSetTextBoxText((nw4r::lyt::Layout*)mpLayout, base + 0x466, textBuf, 0);
    }

    // Candidate item name (via the item vtable) or the raw item-name lookup.
    void* item;
    char* txt;
    if (arg3 != NULL) {
        item = arg3;
        txt = (char*)CItem_initItemImplInstances((CItemData*)item)->vf20((CItemData*)item);
    } else {
        item = NULL;
        txt = ((char* (*)(u32))BuildItemNameStr)(arg2);
    }
    LayoutSetTextBoxFmtValue((nw4r::lyt::Layout*)mpLayout, base + 0x472, txt, 0);

    u16 cat;
    if (arg3 != NULL) {
        cat = (u16)CItem_initItemImplInstances((CItemData*)item)->vf54((CItemData*)item);
    } else {
        cat = BdatGetItemId(arg2);
    }
    u8 st = BdatGetU8Direct((const void*)lbl_eu_806640D8, base + 0x47d, cat);
    char* label = BdatTouchStringCell(base + 0x130, base + 0x139, (u8)st + 0x15);
    LayoutSetTextBoxFmtValue((nw4r::lyt::Layout*)mpLayout, base + 0x484, label, 0);

    // Table handle pair is loop-invariant: staged once before the loop.
    CItemBoxTagStage stage = { { 2, 4, 5, 6, 7, 8 } };
    for (u32 i = 0; (u8)i < (u8)code80135FDC_getByte_64077(); i++) {
        u32 idx = (u8)i + 1;
        sprintf(textBuf, base + 0x303, idx);
        PaneSetVtxColorByName((nw4r::lyt::Layout*)mpLayout, textBuf, -1);
        // Row colour pair: zeroed memberwise, then filled from the this
        // selection block; passed to sprintf as one 0x10-byte vararg.
        cols.w[0] = 0; cols.w[1] = 0; cols.w[2] = 0; cols.w[3] = 0;
        cols.w[0] = mColorA.w[0];
        cols.w[1] = mColorA.w[1];
        cols.w[2] = mColorB.w[0];
        cols.w[3] = mColorB.w[1];
        sprintf(textBuf, base + 0x161, idx, cols);
        nw4r::lyt::Pane* pane =
            mpLayout->GetRootPane()->FindPaneByName(textBuf, true);
        if (pane != NULL) {
            void* obj = pane->GetMaterial();
            for (u32 j = 0; j < 2; j++) {
                setItemBoxVtxColor(obj, (u8)j, (u8*)&cols + (u8)j * 8);
            }
        }
        if (arg3 == NULL) continue;
        // Slot tag must resolve; then stage the slot's equip-value record.
        u8 slot = (u8)GetCollectedFlagByte((u8)i);
        if (slot == 0) continue;
        void* lookup = CtrlObjectParam_GetCharEntry(slot);
        entry.tbl = stage.tbl;
        entry.h = stage.h;
        entry.vals[0] = *(s16*)((u8*)lookup + 0x26);
        entry.vals[1] = *(s16*)((u8*)lookup + 0x1C);
        entry.vals[2] = *(s16*)((u8*)lookup + 0x1E);
        entry.vals[3] = *(s16*)((u8*)lookup + 0x20);
        entry.vals[4] = *(s16*)((u8*)lookup + 0x22);
        entry.vals[5] = *(s16*)((u8*)lookup + 0x24);
        for (u32 j = 0; (u8)j < 6; j++) {
            void* cand = CItem_kindPtr((u8)j, entry.vals[(u8)j]);
            if (cand == NULL || *(u32*)cand == 0) continue;
            u8 cnt = CItem_initItemImplInstances((CItemData*)cand)->vf30((CItemData*)cand);
            for (u32 k = 0; (u8)k < cnt; k++) {
                s16 itemVal =
                    CItem_initItemImplInstances((CItemData*)cand)->vf40((CItemData*)cand, (u8)k);
                void* hit = CItem_kindPtr(3, itemVal);
                if (hit == NULL || *(u32*)hit == 0 || hit != arg3) continue;
                sprintf(buf2, base + 0x30e, idx);
                u32 tex;
                if (slot == (u8)GetCollectedFlagByte(arg4)) {
                    tex = (u32)mArcResAcc->GetResource(0x74696d67, base + 0x319, NULL);
                } else {
                    tex = (u32)mArcResAcc->GetResource(0x74696d67, base + 0x32d, NULL);
                }
                if (tex == 0) {
                    tex = (u32)mArcResAcc->GetResource(0x74696d67, base + 0x341, NULL);
                }
                if (tex != 0) {
                    PaneSetTexPaletteByName((nw4r::lyt::Layout*)mpLayout, (const char*)buf2, (TPLPalette*)tex);
                }
            }
        }
    }
}
#pragma pop
#pragma push
#pragma optimize_for_size on
__declspec(noinline) void CItemBoxInfo2::RenderLabels(u32 arg2) {
    ClearPairTexts();
    ResetRowPanes();
    CItemBoxLabelRec rec = GetLabelRec(arg2);
    LayoutSetTextBoxFmtValue((nw4r::lyt::Layout*)mpLayout, (char*)&lbl_eu_805063BC + 0x48f, rec.b, 0);
    LayoutSetTextBoxFmtValue((nw4r::lyt::Layout*)mpLayout, (char*)&lbl_eu_805063BC + 0x49b, rec.d, 0);
}
#pragma pop
// Retail renderItemBox2SlotList: run the two layout preps, build a 0x24-byte slot
// record via buildItemBox2SlotRec, struct-copy it to a local (retail mtctr 8-byte
// pair loop), then label every slot: a count-driven loop of pane-name
// sprintf + per-item text/value colour labels (inlined FixStr<32> ctor).
#pragma push
#pragma optimize_for_size on
#pragma auto_inline off
// Word-array view: assigning the union copies the whole object as one block
// (reproduces the retail mtctr lwzu/stwu pair loop) instead of expanding
// field-by-field.
union CItemBoxSlotRecAny {
    CItemBoxSlotRecord rec;
    u32 w[9];
};
__declspec(noinline) void CItemBoxInfo2::RenderSlotList(void* arg1, void* arg2) {
    // Retail runs the two layout preps before the FixStr ctor bl.
    char paneName[0x20];
    ClearSlotTexts();
    ResetRowPanes();
    CItemBoxSlotRecord cur = BuildSlotRec((u32)arg1, arg2);
    ml::FixStr<32> text(false);
    LayoutSetTextBoxFmtValue((nw4r::lyt::Layout*)mpLayout, &lbl_eu_805063BC[0x4a7], (char*)cur.str, 0);
    u8 count = cur.tail[1];
    for (u8 i = 0; i < count; i++) {
        sprintf(paneName, &lbl_eu_805063BC[0x4b3], (u8)i * 2 + 0x1f);
        LayoutSetTextBoxFmtValue((nw4r::lyt::Layout*)mpLayout, paneName, (char*)cur.text[i], 0);
        text.clear();
        sprintf(paneName, &lbl_eu_805063BC[0x4c0], (u8)i + 0x1f);
        text.format(&lbl_eu_805063BC[0x13e], cur.vals[i],
                    BdatTouchStringCell(&lbl_eu_805063BC[0x130], &lbl_eu_805063BC[0x139], 0x21));
        LayoutSetTextBoxFmtValue((nw4r::lyt::Layout*)mpLayout, paneName, text.c_str(), 0);
    }
}
#pragma auto_inline reset
#pragma pop
#pragma push
#pragma optimize_for_size on
// dont_inline keeps IPA from folding this body into its callers
// (renderItemBox2ByKind etc.) - retail always calls it.
__declspec(noinline) void CItemBoxInfo2::SetupSlotPanes() {
    // i declared first: retail claims r31 for the loop counter before any
    // other saved local (register_mapping.md Rule A).
    // No named base local: retail holds &lbl_eu_805063BC purely as a compiler
    // CSE temporary (lis into a scratch register during prologue argument
    // prep), LICM-promoted to a saved register for the loop.
    char buf[0x20];
    u32 i;
    u32 idx;
    s16 zero = 0;
    LayoutSetTextBoxFmtValue((nw4r::lyt::Layout*)mpLayout, &lbl_eu_805063BC[0x25b], &lbl_eu_805063BC[0x2aa], 0);
    LayoutSetTextBoxFmtValue((nw4r::lyt::Layout*)mpLayout, &lbl_eu_805063BC[0x267], &lbl_eu_805063BC[0x2aa], 0);
    LayoutSetTextBoxFmtValue((nw4r::lyt::Layout*)mpLayout, &lbl_eu_805063BC[0x273], &lbl_eu_805063BC[0x2aa], 0);
    LayoutSetTextBoxFmtValue((nw4r::lyt::Layout*)mpLayout, &lbl_eu_805063BC[0x286], &lbl_eu_805063BC[0x2aa], 0);
    LayoutSetTextBoxFmtValue((nw4r::lyt::Layout*)mpLayout, &lbl_eu_805063BC[0x292], &lbl_eu_805063BC[0x2aa], 0);
    LayoutSetTextBoxFmtValue((nw4r::lyt::Layout*)mpLayout, &lbl_eu_805063BC[0x29e], &lbl_eu_805063BC[0x2aa], 0);
    PaneMatSetTevColorsByName((nw4r::lyt::Layout*)mpLayout, &lbl_eu_805063BC[0x2b6], &lbl_eu_806645A8, &lbl_eu_806645B0);
    PaneMatSetTevColorsByName((nw4r::lyt::Layout*)mpLayout, &lbl_eu_805063BC[0x2c1], &lbl_eu_806645A8, &lbl_eu_806645B0);
    PaneMatSetTevColorsByName((nw4r::lyt::Layout*)mpLayout, &lbl_eu_805063BC[0x2cc], &lbl_eu_806645A8, &lbl_eu_806645B0);
    PaneMatSetTevColorsByName((nw4r::lyt::Layout*)mpLayout, &lbl_eu_805063BC[0x2d7], &lbl_eu_806645A8, &lbl_eu_806645B0);
    PaneMatSetTevColorsByName((nw4r::lyt::Layout*)mpLayout, &lbl_eu_805063BC[0x2e2], &lbl_eu_806645A8, &lbl_eu_806645B0);
    PaneMatSetTevColorsByName((nw4r::lyt::Layout*)mpLayout, &lbl_eu_805063BC[0x2ed], &lbl_eu_806645A8, &lbl_eu_806645B0);
    for (i = 0; (u8)i < (u8)code80135FDC_getByte_64077(); i++) {
        idx = (u8)i + 1;
        sprintf(buf, &lbl_eu_805063BC[0x303], idx);
        PaneSetVtxColorByName((nw4r::lyt::Layout*)mpLayout, buf, 0x777777ff);
        u32 args[4];
        s16 c0hi[2] = {zero, mColorA.s[3]};
        // All-zero pairs stored DESCENDING; colour words copied to args[]
        // ASCENDING (retail order - see resetItemBox2RowPanes finding).
        s16 c0lo[2];
        c0lo[1] = zero;
        c0lo[0] = zero;
        s16 c1hi[2] = {zero, mColorB.s[3]};
        s16 c1lo[2];
        c1lo[1] = zero;
        c1lo[0] = zero;
        args[0] = *(u32*)c0lo;
        args[1] = *(u32*)c0hi;
        args[2] = *(u32*)c1lo;
        args[3] = *(u32*)c1hi;
        sprintf(buf, &lbl_eu_805063BC[0x161], idx);
        nw4r::lyt::Pane* pane = mpLayout->GetRootPane()->FindPaneByName(buf, true);
        if (pane != NULL) {
            nw4r::lyt::Material* material = pane->GetMaterial();
            for (u32 j = 0; j < 2; j++) {
                setItemBoxVtxColor(material, (u8)j, (u8*)args + (u8)j * 8);
            }
        }
        // Item-name lookup on the shared arc resource accessor (vtable+0x0C),
        // then push the texture name onto the slot pane.
        sprintf(buf, &lbl_eu_805063BC[0x30e], idx);
        u32 tex = (u32)mArcResAcc->GetResource(0x74696D67, &lbl_eu_805063BC[0x341], NULL);
        if (tex != 0) {
            PaneSetTexPaletteByName((nw4r::lyt::Layout*)mpLayout, (const char*)buf, (TPLPalette*)tex);
        }
    }
}
#pragma pop
#pragma push
#pragma optimize_for_size on
// dont_inline keeps IPA from folding this body into its callers
// (renderItemBox2Labels etc.) - retail always calls it.
__declspec(noinline) void CItemBoxInfo2::ResetRowPanes() {
    // No named base local: retail holds &lbl_eu_805063BC purely as a
    // compiler CSE temporary (lis into a scratch register during prologue
    // argument prep), which fixes both its schedule and its register.
    char text[0x20];
    LayoutSetTextBoxFmtValue((nw4r::lyt::Layout*)mpLayout, &lbl_eu_805063BC[0x267], &lbl_eu_805063BC[0x2aa], 0);
    LayoutSetTextBoxFmtValue((nw4r::lyt::Layout*)mpLayout, &lbl_eu_805063BC[0x273], &lbl_eu_805063BC[0x2aa], 0);
    LayoutSetTextBoxFmtValue((nw4r::lyt::Layout*)mpLayout, &lbl_eu_805063BC[0x29e], &lbl_eu_805063BC[0x2aa], 0);
    PaneMatSetTevColorsByName((nw4r::lyt::Layout*)mpLayout, &lbl_eu_805063BC[0x2b6], &lbl_eu_806645A8, &lbl_eu_806645B0);
    PaneMatSetTevColorsByName((nw4r::lyt::Layout*)mpLayout, &lbl_eu_805063BC[0x2c1], &lbl_eu_806645A8, &lbl_eu_806645B0);
    PaneMatSetTevColorsByName((nw4r::lyt::Layout*)mpLayout, &lbl_eu_805063BC[0x2cc], &lbl_eu_806645A8, &lbl_eu_806645B0);
    PaneMatSetTevColorsByName((nw4r::lyt::Layout*)mpLayout, &lbl_eu_805063BC[0x2d7], &lbl_eu_806645A8, &lbl_eu_806645B0);
    PaneMatSetTevColorsByName((nw4r::lyt::Layout*)mpLayout, &lbl_eu_805063BC[0x2e2], &lbl_eu_806645A8, &lbl_eu_806645B0);
    PaneMatSetTevColorsByName((nw4r::lyt::Layout*)mpLayout, &lbl_eu_805063BC[0x2ed], &lbl_eu_806645A8, &lbl_eu_806645B0);
    // Zero lives outside the loop: retail materializes it once in the
    // preheader (a loop-invariant CSE), not per-iteration.
    s16 shade = 0;
    for (u32 index = 0; (u8)index < (u8)code80135FDC_getByte_64077(); index++) {
        u32 slot = (u8)index + 1;
        sprintf(text, &lbl_eu_805063BC[0x303], slot);
        PaneSetVtxColorByName((nw4r::lyt::Layout*)mpLayout, text, 0x777777ff);
        u32 args[4];
        s16 hiA[2] = {shade, mColorA.s[3]};
        // All-shade pairs: retail emits the two halfword stores in DESCENDING
        // address order (element 1 before element 0); explicit assignments
        // reproduce that order where the initializer list emits ascending.
        s16 loA[2];
        loA[1] = shade;
        loA[0] = shade;
        s16 hiB[2] = {shade, mColorB.s[3]};
        s16 loB[2];
        loB[1] = shade;
        loB[0] = shade;
        // Ascending assignment order: retail copies the four colour words
        // to the stack in slot order (24, 28, 32, 36).
        args[0] = *(u32*)loA;
        args[1] = *(u32*)hiA;
        args[2] = *(u32*)loB;
        args[3] = *(u32*)hiB;
        sprintf(text, &lbl_eu_805063BC[0x161], slot);
        nw4r::lyt::Pane* pPane = mpLayout->GetRootPane()->FindPaneByName(text, true);
        if (pPane != NULL) {
            nw4r::lyt::Material* pMaterial = pPane->GetMaterial();
            for (u32 k = 0; k < 2; k++) {
                setItemBoxVtxColor(pMaterial, (u8)k, (u8*)args + (u8)k * 8);
            }
        }
        // Item-name lookup on the shared arc resource accessor (vtable+0x0C),
        // then push the texture name onto the slot pane.
        sprintf(text, &lbl_eu_805063BC[0x30e], slot);
        u32 texId = (u32)mArcResAcc->GetResource(0x74696D67, &lbl_eu_805063BC[0x341], NULL);
        if (texId != 0) {
            PaneSetTexPaletteByName((nw4r::lyt::Layout*)mpLayout, (const char*)text, (TPLPalette*)texId);
        }
    }
}
#pragma pop
#pragma push
#pragma optimize_for_size on
__declspec(noinline) void CItemBoxInfo2::ClearTripleRows() {
    char buf[0x20];
    // Declaration order fixes r29/r30: retail claims index (r30) before i (r29).
    u32 index;
    u32 i;
    for (i = 0; i < 3; i++) {
        index = (u8)i + 1;
        sprintf(buf, (char*)&lbl_eu_805063BC[0x35f], index);
        void* r = ((LayoutRootPaneView*)mpLayout)->rootPane10->FindPaneByName(buf, true);
        setPaneVisible(r, 0);
        sprintf(buf, (char*)&lbl_eu_805063BC[0x408], index);
        LayoutSetTextBoxFmtValue(mpLayout, buf, (char*)&lbl_eu_805063BC[0x2aa], 0);
        sprintf(buf, (char*)&lbl_eu_805063BC[0x426], index);
        LayoutSetTextBoxFmtValue(mpLayout, buf, (char*)&lbl_eu_805063BC[0x2aa], 0);
    }
}
#pragma pop
#pragma push
#pragma optimize_for_size on
__declspec(noinline) void CItemBoxInfo2::ApplySelColors() {
    LayoutSetTextBoxFmtValue((nw4r::lyt::Layout*)mpLayout, &lbl_eu_805063BC[0x44f], &lbl_eu_805063BC[0x2aa], 0);
    char buf[0x20];
    u32 idx;
    // Retail re-evaluates the slot count on every iteration (bl in the
    // loop condition), so do not hoist the call.
    for (u32 i = 0; (u8)i < (u8)code80135FDC_getByte_64077(); i++) {
        idx = (u32)((u8)i + 1);
        sprintf(buf, (char*)&lbl_eu_805063BC[0x303], idx);
        PaneSetVtxColorByName((nw4r::lyt::Layout*)mpLayout, buf, 0x777777ff);
        // Selection colours: two 8-byte pairs built from the s16s at +0xA2
        // and +0xAA; retail stages the four arrays at sp+0x08..0x17 and the
        // u32 varargs copy at sp+0x18..0x27.
        s16 zero = 0;
        s16 c0hi[2] = {zero, mColorA.s[3]};
        s16 c0lo[2];
        c0lo[1] = zero;
        c0lo[0] = zero;
        s16 c1hi[2] = {zero, mColorB.s[3]};
        s16 c1lo[2];
        c1lo[1] = zero;
        c1lo[0] = zero;
        u32 args[4];
        args[0] = *(u32*)c0lo;
        args[1] = *(u32*)c0hi;
        args[2] = *(u32*)c1lo;
        args[3] = *(u32*)c1hi;
        sprintf(buf, (char*)&lbl_eu_805063BC[0x161], idx);
        nw4r::lyt::Pane* pane = mpLayout->GetRootPane()->FindPaneByName(buf, true);
        if (pane != NULL) {
            void* obj = pane->GetMaterial();
            for (u32 j = 0; j < 2; j++) {
                setItemBoxVtxColor(obj, (u8)j, (u8*)args + (u8)j * 8);
            }
        }
    }
}
#pragma pop
// Retail uses the stmw/lmw frame (MWCC optimize_for_size prologue merge);
// the first call's arg4 zero and the string base are hoisted into the prologue.
#pragma push
#pragma optimize_for_size on
__declspec(noinline) void CItemBoxInfo2::ClearQuadTexts() {
    LayoutSetTextBoxFmtValue((nw4r::lyt::Layout*)mpLayout,
                  &lbl_eu_805063BC[0x45a],
                  &lbl_eu_805063BC[0x2aa], 0);
    LayoutSetTextBoxFmtValue((nw4r::lyt::Layout*)mpLayout,
                  &lbl_eu_805063BC[0x466],
                  &lbl_eu_805063BC[0x2aa], 0);
    LayoutSetTextBoxFmtValue((nw4r::lyt::Layout*)mpLayout,
                  &lbl_eu_805063BC[0x484],
                  &lbl_eu_805063BC[0x2aa], 0);
    LayoutSetTextBoxFmtValue((nw4r::lyt::Layout*)mpLayout,
                  &lbl_eu_805063BC[0x472],
                  &lbl_eu_805063BC[0x2aa], 0);
}
#pragma pop

#pragma push
#pragma optimize_for_size on
__declspec(noinline) void CItemBoxInfo2::ClearPairTexts() {
    LayoutSetTextBoxFmtValue((nw4r::lyt::Layout*)mpLayout,
                  &lbl_eu_805063BC[0x48f],
                  &lbl_eu_805063BC[0x2aa], 0);
    LayoutSetTextBoxFmtValue((nw4r::lyt::Layout*)mpLayout,
                  &lbl_eu_805063BC[0x49b],
                  &lbl_eu_805063BC[0x2aa], 0);
}
#pragma pop
// Retail clearItemBox2SlotTexts uses the stmw/lmw frame (MWCC optimize_for_size
// prologue merge): info in r29, loop counter in r30, string-pool base in r31.
// dont_inline keeps IPA from folding this body into its callers
// (renderItemBox2SlotList etc.) - retail always calls it.
#pragma push
#pragma optimize_for_size on
__declspec(noinline) void CItemBoxInfo2::ClearSlotTexts() {
    char buf[0x20];
    LayoutSetTextBoxFmtValue((nw4r::lyt::Layout*)mpLayout,
                  &lbl_eu_805063BC[0x4a7],
                  &lbl_eu_805063BC[0x2aa], 0);
    for (u32 i = 0; i < 4; i++) {
        sprintf(buf, &lbl_eu_805063BC[0x4b3], (u8)i * 2 + 0x1f);
        LayoutSetTextBoxFmtValue((nw4r::lyt::Layout*)mpLayout, buf,
                      &lbl_eu_805063BC[0x2aa], 0);
        sprintf(buf, &lbl_eu_805063BC[0x4c0], (u8)i + 0x1f);
        LayoutSetTextBoxFmtValue((nw4r::lyt::Layout*)mpLayout, buf,
                      &lbl_eu_805063BC[0x2aa], 0);
    }
}
#pragma pop

// Retail stampItemBox2SlotTex: ItemBox2 variant of stampItemBoxSlotTex (same body, same
// stmw r22 frame). Loops 7 slots; below the active count it resolves the item
// name via CUICfManager_getArcResourceAccessor/vtable+0x0C and colors the pane; otherwise a plain
// label pane. The two alpha-only vertex colours are packed into a 4-word block
// shared by sprintf and the per-vertex setItemBoxVtxColor loop.
#pragma push
#pragma optimize_for_size on
__declspec(noinline) void CItemBoxInfo2::StampSlotTex() {
    u32 i;
    char bufElse[0x20];
    char buf[0x20];
    u8 count = code80135FDC_getByte_64077();
    u32 idx;
    for (i = 0; i < 7; i++) {
        if ((u8)i >= count) {
            sprintf(bufElse, (char*)&lbl_eu_805063BC[0x161], (u8)i + 1);
            nw4r::lyt::Pane* pane = mpLayout->GetRootPane()->FindPaneByName(bufElse, true);
            setPaneVisible(pane, 0);
        } else {
            u8 slot = (u8)GetCollectedFlagByte((u8)i);
            u32 tag = 0x74696D67u;
            u32 nameId = (u16)BdatGetU16Direct(lbl_eu_80664090, (char*)&lbl_eu_805063BC[0x4ce], slot);
            u32 itemId = (u32)MakeTplNameSysFile(nameId);
            // Item-name lookup through the shared name system (vtable+0x0C).
            u32 found = (u32)CUICfManager_getArcResourceAccessor()->GetResource(tag, (const char*)itemId, NULL);
            if (found != 0) {
                idx = (u32)((u8)i + 1);
                sprintf(buf, (char*)&lbl_eu_805063BC[0x303], idx);
                nw4r::lyt::Pane* pane2 = mpLayout->GetRootPane()->FindPaneByName(buf, true);
                PaneSetTexPalette(pane2, (TPLPalette*)found);
                PaneSetVtxColorAll(pane2, 0x777777ffu);
                // Selection colours: two 8-byte pairs built from the s16s at +0xA2
                // and +0xAA. All-zero pairs: retail stores the halfwords in
                // DESCENDING element order (see resetItemBox2RowPanes finding).
                s16 zero = 0;
                s16 c0hi[2] = {zero, mColorA.s[3]};
                s16 c0lo[2];
                c0lo[1] = zero;
                c0lo[0] = zero;
                s16 c1hi[2] = {zero, mColorB.s[3]};
                s16 c1lo[2];
                c1lo[1] = zero;
                c1lo[0] = zero;
                u32 args[4];
                args[0] = *(u32*)c0lo;
                args[1] = *(u32*)c0hi;
                args[2] = *(u32*)c1lo;
                args[3] = *(u32*)c1hi;
                sprintf(buf, (char*)&lbl_eu_805063BC[0x161], idx);
                nw4r::lyt::Pane* pane3 = mpLayout->GetRootPane()->FindPaneByName(buf, true);
                void* vertexObj = pane3->GetMaterial();
                for (u32 j = 0; j < 2; j++) {
                    setItemBoxVtxColor(vertexObj, (u8)j, (u8*)args + (u8)j * 8);
                }
            }
        }
    }
}
#pragma pop
#pragma push
// Activates the panel: once both the layout (+0x34) and its resource
// accessor (+0x30) are present, set the current/active bytes. Goto-gate
// layout matches retail's branch-over-branch shape (bne over a blr with
// the body placed after the return - see CTitle CTitleLogo_draw family).
__declspec(noinline) void CItemBoxInfo2::Activate() {
    if (mpLayout != 0 && mTexArcResAcc != 0) {
        goto body;
    }
    return;
body:
    mCurrent = 1;
    mActive = 1;
}
#pragma pop
// ============================================================================
// setupItemBox2Detail - ItemBox2 info-panel render (retail 0x801E5FB8, 0x4DA8 bytes)
// Reconstructed from retail disassembly (build/us/asm/kyoshin/CItemBoxInfo.s).
// NOTE: arg3 (r5) is dead in retail - clobbered by the prologue (lis r5, 0x4330).
// ============================================================================
// TU-local stat-object shapes (retail lha/lfs/lbz offsets from setupItemBox2Detail).
union E43Entry {          // 0x34-byte item-stat entry (BuildCmpRecord output)
    CItemBoxCompRecord rec;
    struct {
        u16 w00;
        u16 _02;
        u32 w04;                       // +0x04 value (u16 pairs read via lhz at +4/+6)
        u8 _08[4];
        u32 _0C;
        u32 _10;
        s16 w14;                       // +0x14 (delta-compared)
        s16 _16;
        u8 _18[0x34 - 0x18];
    };
};
struct E43StatC {          // vtable[0x22C] result - s16 stat block
    u8 _00[6];
    s16 s06;
    u8 _08[4];
    s16 s0C;
    s16 s0E;
    s16 s10;
    u8 _12[6];
    s16 s18;
    u8 _1A[2];
    s16 s1C;
    u8 _1E[4];
    s16 s22;
};
struct E43StatA {          // vtable[0x228] result - s16 stat block + f32
    u8 _00[0x10];
    f32 f10;
    u8 _14[8];
    s16 s1C;
    s16 s1E;
    s16 s20;
    u8 _22[0xC];
    s16 s2E;
    u8 _30[2];
    s16 s32;
    u8 _34[4];
    s16 s38;
    u8 _3A[0x1B];
    u8 b55;
};
struct E43StatB {          // vtable[0x20C] result - f32 block
    u8 _00[0x10];
    f32 f10;
};

struct ItemBoxInfoCopy {
    unsigned short itemId;
    unsigned short _02;
    unsigned int value;
    unsigned char state;
};
#pragma push
__declspec(noinline) void copyItemBoxCopy(ItemBoxInfoCopy* dst, const ItemBoxInfoCopy* src) {
    dst->itemId = src->itemId;
    dst->value = src->value;
    dst->state = src->state;
}
#pragma pop

#pragma push
__declspec(noinline) void setItemBoxCopy(ItemBoxInfoCopy* self, unsigned short a, unsigned int b, unsigned char c) {
    self->itemId = a;
    self->value = b;
    self->state = c;
}
#pragma pop

void CItemBoxInfo2::SetupDetail(u16 arg2, void* arg3, u16 arg4, u32 arg5) {
    // ---- type (main switch key) ----
    u8 type = (u8)BdatGetItemType((u32)arg2);

    // ---- party-slot ping: 12-word copy of party struct + 2x3 vtable[0xA4] ----
    struct PartyData { u32 w[12]; };
    PartyData partyData = *(PartyData*)((u8*)CtrlObjectParam_GetSlotTableBase() + 4);
    for (u32 row = 0; row < 2; row++) {
        for (u32 col = 0; col < 3; col++) {
            u8 id = (u8)partyData.w[col];
            if (id != 0) {
                void* actor = findObjB28ById(id);
                if (actor != NULL) {
                    ((cf::CActorParam*)actor)->CActorParam_resetArtsStatus(NULL);
                }
            }
        }
    }

    // ---- character setup ----
    u8 member = (u8)GetCollectedFlagByte(arg4);
    void* charObj = CtrlObjectParam_GetCharEntry(member);
    if (findObjB28ById(member) == NULL) {
        CtrlObjectParam_SyncParamFromActorEx(charObj, 1);
    }
    cf::CActorParam* stats = (cf::CActorParam*)((u8*)charObj + 0x17C);

    // ---- HP values (clamped to 9999) ----
    s32 hp1 = (s32)stats->CActorParam_getHp();
    if (hp1 > 9999) hp1 = 9999;
    s32 hp2 = (s32)stats->CActorParam_getDamageScale();
    if (hp2 > 9999) hp2 = 9999;

    // ---- name / pane text ----
    LayoutSetTextBoxFmtValue(mpLayout, &lbl_eu_805063BC[0x4D7], BdatGetPtrDirect(lbl_eu_80664090, &lbl_eu_805063BC[0x139], member), 0);
    setLayoutTextBoxNumber(mpLayout, &lbl_eu_805063BC[0x4E3], (u8)stats->CActorParam_getActorLevel());
setLayoutTextBoxNumber(mpLayout, &lbl_eu_805063BC[0x4F0], (u8)stats->CActorParam_getTotalCurrency());
    LayoutSetTextBoxInt((nw4r::lyt::Layout*)mSlotPane0, hp1);
    LayoutSetTextBoxInt((nw4r::lyt::Layout*)mSlotPane2, hp2);
    SetLayoutSelfText((nw4r::lyt::Layout*)mSlotPane3, BdatTouchStringCell(&lbl_eu_805063BC[0x130], &lbl_eu_805063BC[0x139], 0x82), 0);

    // ---- stat sub-objects ----
    E43StatA* stA = (E43StatA*)stats->CActorParam_getBaseStats();
    E43StatB* stB = (E43StatB*)stats->CActorParam_getArtsDataBlock();
    E43StatC* stC = (E43StatC*)stats->CActorParam_getBonusStats();
    CtrlObjectParam_GetArtsStatsRow((u8*)charObj + 0x1C, 5);

    // ---- arts sum ----
    s32 artsSum = (s32)SumTypeCounts(member, NULL);
    u32 w = GetCondStatById(member, 0x97);
    if (w != 0) {
        artsSum = artsSum - (s32)w;
        if (artsSum < 0) artsSum = 0;
    }

    // ---- stat bars (display formula) ----
    s16 bar1 = (s16)(s32)(0.01f * ((100.0f + (f32)stC->s10) * (f32)(stA->s20 + GetEffect(member, 0x21, NULL))));
    s16 bar2 = (s16)(s32)(0.01f * ((100.0f + (f32)stC->s0C) * (f32)(stA->s1C + GetEffect(member, 0x31, NULL))));
    s16 bar3 = (s16)(s32)(0.01f * ((100.0f + (f32)stC->s1C) * (f32)(stA->s32 + GetEffect(member, 0x41, NULL))));
    s16 bar4 = (s16)(s32)(0.01f * ((100.0f + (f32)stC->s0E) * (f32)(stA->s1E + GetEffect(member, 0x51, NULL) - artsSum)));
    s16 bar5 = (s16)(s32)(0.01f * ((100.0f + (f32)stC->s18) * (f32)(stA->s2E + GetEffect(member, 0x54, NULL))));
    s16 bar6 = (s16)(s32)(0.01f * ((100.0f + (f32)stC->s22) * (f32)(stA->s38 + GetEffect(member, 0x11, NULL))));
    RoundHalfAway0(0.01f * stB->f10 * (f32)(stC->s06 + GetEffect(member, 0x11, NULL)));

    // ---- equip-mode gate: recompute bars with the equipped stat (0x30) ----
    if (TestEquip(member, NULL)) {
        s32 eq = GetCondStatById(member, 0x30);
        if (eq != 0) {
            s16 barA = (s16)(s32)(0.01f * ((100.0f + (f32)stC->s10 + (f32)eq) * (f32)(stA->s20 + GetEffect(member, 0x21, NULL))));
            s16 barB = (s16)(s32)(0.01f * ((100.0f + (f32)stC->s0C + (f32)barA) * (f32)(stA->s1C + GetEffect(member, 0x1, NULL))));
            s16 barC = (s16)(s32)(0.01f * ((100.0f + (f32)stC->s0E + (f32)eq) * (f32)(stA->s1E + GetEffect(member, 0x41, NULL) - artsSum)));
            RoundHalfAway0(0.01f * stB->f10 * (f32)(stC->s06 + eq + GetEffect(member, 0x11, NULL)));

            // ---- current weapon block ----
            void* item2 = CItem_kindPtr(2, *(s16*)((u8*)charObj + 0x26));
            if (item2 != NULL && *(u32*)item2 != 0) {
                E43Entry e1;
                e1.rec = BuildCmpRecord((void*)(u32)((u16)((*(u32*)item2 >> 20) & 0xFFFF)));
                E43Entry e2 = e1;
                s32 s1 = CalcBonus1(member, e2.w04 & 0xFFFF, NULL);
                s32 s2 = CalcBonus2(member, *(u16*)((u8*)&e2 + 6), NULL);
                s32 atk1 = (s32)barB + s1;
                s32 atk2 = (s32)barB + s2;
                if (atk1 > atk2) atk1 = atk2;
                ml::FixStr<32> fmtBuf;
                fmtBuf.format(&lbl_eu_805063BC[0x254], atk1, BdatTouchStringCell(&lbl_eu_805063BC[0x130], &lbl_eu_805063BC[0x139], 0xB), atk2);
                SetLayoutSelfText((nw4r::lyt::Layout*)mSlotPane10, fmtBuf.c_str(), 0);
            }

            // ---- numbers ----
            if (barC <= 0) barC = 1;
            LayoutSetTextBoxInt((nw4r::lyt::Layout*)mSlotPane4, (s16)barB);
            LayoutSetTextBoxInt((nw4r::lyt::Layout*)mSlotPane6, (s16)barC);
            LayoutSetTextBoxInt((nw4r::lyt::Layout*)mSlotPane8, (s16)barA);
            LayoutSetTextBoxInt((nw4r::lyt::Layout*)mSlotPane12, (s16)bar5);
            LayoutSetTextBoxInt((nw4r::lyt::Layout*)mSlotPane14, (s16)bar3);
            ml::FixStr<32> fmtBuf2;
            fmtBuf2.format(&lbl_eu_805063BC[0x13E], (s16)bar6, BdatTouchStringCell(&lbl_eu_805063BC[0x130], &lbl_eu_805063BC[0x139], 0x80));
            SetLayoutSelfText((nw4r::lyt::Layout*)mSlotPane16, fmtBuf2.c_str(), 0);
            ml::FixStr<32> fmtBuf3;
            fmtBuf3.format(&lbl_eu_805063BC[0x13E], (s16)(stA->b55), BdatTouchStringCell(&lbl_eu_805063BC[0x130], &lbl_eu_805063BC[0x139], 0x80));
            SetLayoutSelfText((nw4r::lyt::Layout*)mSlotPane18, fmtBuf3.c_str(), 0);

            // ---- 20x color application ----
            E43Quad q1 = *(E43Quad*)&lbl_eu_80664518;
            E43Quad q2 = *(E43Quad*)&lbl_eu_80664520;
            E43Quad q3 = *(E43Quad*)&lbl_eu_80664558;
            E43Quad q4 = *(E43Quad*)&lbl_eu_80664560;
            PaneMatSetTevColors(mSlotPane0, &q3, &q4);
            PaneMatSetTevColors(mSlotPane2, &q3, &q4);
            PaneMatSetTevColors(mSlotPane3, &q3, &q4);
            PaneMatSetTevColors(mSlotPane1, &q1, &q2);
            PaneMatSetTevColors(mSlotPane4, &q3, &q4);
            PaneMatSetTevColors(mSlotPane5, &q1, &q2);
            PaneMatSetTevColors(mSlotPane6, &q3, &q4);
            PaneMatSetTevColors(mSlotPane7, &q1, &q2);
            PaneMatSetTevColors(mSlotPane8, &q3, &q4);
            PaneMatSetTevColors(mSlotPane9, &q1, &q2);
            PaneMatSetTevColors(mSlotPane10, &q3, &q4);
            PaneMatSetTevColors(mSlotPane11, &q1, &q2);
            PaneMatSetTevColors(mSlotPane12, &q3, &q4);
            PaneMatSetTevColors(mSlotPane13, &q1, &q2);
            PaneMatSetTevColors(mSlotPane14, &q3, &q4);
            PaneMatSetTevColors(mSlotPane15, &q1, &q2);
            PaneMatSetTevColors(mSlotPane16, &q3, &q4);
            PaneMatSetTevColors(mSlotPane17, &q1, &q2);
            PaneMatSetTevColors(mSlotPane18, &q3, &q4);
            PaneMatSetTevColors(mSlotPane19, &q1, &q2);
        }
    }

    // ---- stat panel (arg5 != 0, type in {2,4,5,6,7,8}) ----
    if (arg5 != 0 && (type == 2 || (type >= 4 && type <= 8))) {
        s32 e1 = 0;
        if (TestEquip(member, NULL)) e1 = GetCondStatById(member, 0x30);
        s32 e2 = 0;
        if (TestEquip(member, arg3)) e2 = GetCondStatById(member, 0x30);

        // row A: 0x21 -> panes 0x60/0x64
        s16 dB = 0;
        {
            s16 pb = (s16)(s32)(0.01f * ((100.0f + (f32)stC->s10 + (f32)e1) * (f32)(stA->s20 + GetEffect(member, 0x21, NULL))));
            s16 nb = (s16)(s32)(0.01f * ((100.0f + (f32)stC->s10 + (f32)e2) * (f32)(stA->s20 + GetEffect(member, 0x21, arg3))));
            s16 d = (s16)(nb - pb);
            LayoutSetTextBoxInt((nw4r::lyt::Layout*)mSlotPane8, nb);
            E43Quad q1 = *(E43Quad*)&lbl_eu_80664518;
            E43Quad q2 = *(E43Quad*)&lbl_eu_80664520;
            E43Quad q3 = *(E43Quad*)&lbl_eu_80664558;
            E43Quad q4 = *(E43Quad*)&lbl_eu_80664560;
            if (d < 0) {
                __as__11_GXColorS10FRC11_GXColorS10(&q1, &lbl_eu_80664538);
                __as__11_GXColorS10FRC11_GXColorS10(&q2, &lbl_eu_80664540);
                __as__11_GXColorS10FRC11_GXColorS10(&q3, &lbl_eu_80664578);
                __as__11_GXColorS10FRC11_GXColorS10(&q4, &lbl_eu_80664580);
            } else if (d > 0) {
                __as__11_GXColorS10FRC11_GXColorS10(&q1, &lbl_eu_80664528);
                __as__11_GXColorS10FRC11_GXColorS10(&q2, &lbl_eu_80664530);
                __as__11_GXColorS10FRC11_GXColorS10(&q3, &lbl_eu_80664568);
                __as__11_GXColorS10FRC11_GXColorS10(&q4, &lbl_eu_80664570);
            }
            PaneMatSetTevColors(mSlotPane8, &q3, &q4);
            PaneMatSetTevColors(mSlotPane9, &q1, &q2);
        }

        // row B: 0x1 -> panes 0x50/0x54
        {
            s16 pb = (s16)(s32)(0.01f * ((100.0f + (f32)stC->s0C + (f32)e1) * (f32)(stA->s1C + GetEffect(member, 0x1, NULL))));
            s16 nb = (s16)(s32)(0.01f * ((100.0f + (f32)stC->s0C + (f32)e2) * (f32)(stA->s1C + GetEffect(member, 0x1, arg3))));
            dB = (s16)(nb - pb);
            LayoutSetTextBoxInt((nw4r::lyt::Layout*)mSlotPane4, nb);
            E43Quad q1 = *(E43Quad*)&lbl_eu_80664518;
            E43Quad q2 = *(E43Quad*)&lbl_eu_80664520;
            E43Quad q3 = *(E43Quad*)&lbl_eu_80664558;
            E43Quad q4 = *(E43Quad*)&lbl_eu_80664560;
            if (dB < 0) {
                __as__11_GXColorS10FRC11_GXColorS10(&q1, &lbl_eu_80664538);
                __as__11_GXColorS10FRC11_GXColorS10(&q2, &lbl_eu_80664540);
                __as__11_GXColorS10FRC11_GXColorS10(&q3, &lbl_eu_80664578);
                __as__11_GXColorS10FRC11_GXColorS10(&q4, &lbl_eu_80664580);
            } else if (dB > 0) {
                __as__11_GXColorS10FRC11_GXColorS10(&q1, &lbl_eu_80664528);
                __as__11_GXColorS10FRC11_GXColorS10(&q2, &lbl_eu_80664530);
                __as__11_GXColorS10FRC11_GXColorS10(&q3, &lbl_eu_80664568);
                __as__11_GXColorS10FRC11_GXColorS10(&q4, &lbl_eu_80664570);
            }
            PaneMatSetTevColors(mSlotPane4, &q3, &q4);
            PaneMatSetTevColors(mSlotPane5, &q1, &q2);
        }

        // row C: 0x41 -> panes 0x58/0x5C
        {
            s16 pb = (s16)(s32)(0.01f * ((100.0f + (f32)stC->s0E + (f32)e1) * (f32)(dB + GetEffect(member, 0x41, NULL))));
            s16 nb = (s16)(s32)(0.01f * ((100.0f + (f32)stC->s0E + (f32)e2) * (f32)(dB + GetEffect(member, 0x41, arg3))));
            s16 dC = (s16)(nb - pb);
            LayoutSetTextBoxInt((nw4r::lyt::Layout*)mSlotPane6, nb);
            E43Quad q1 = *(E43Quad*)&lbl_eu_80664518;
            E43Quad q2 = *(E43Quad*)&lbl_eu_80664520;
            E43Quad q3 = *(E43Quad*)&lbl_eu_80664558;
            E43Quad q4 = *(E43Quad*)&lbl_eu_80664560;
            if (dC < 0) {
                __as__11_GXColorS10FRC11_GXColorS10(&q1, &lbl_eu_80664538);
                __as__11_GXColorS10FRC11_GXColorS10(&q2, &lbl_eu_80664540);
                __as__11_GXColorS10FRC11_GXColorS10(&q3, &lbl_eu_80664578);
                __as__11_GXColorS10FRC11_GXColorS10(&q4, &lbl_eu_80664580);
            } else if (dC > 0) {
                __as__11_GXColorS10FRC11_GXColorS10(&q1, &lbl_eu_80664528);
                __as__11_GXColorS10FRC11_GXColorS10(&q2, &lbl_eu_80664530);
                __as__11_GXColorS10FRC11_GXColorS10(&q3, &lbl_eu_80664568);
                __as__11_GXColorS10FRC11_GXColorS10(&q4, &lbl_eu_80664570);
            }
            PaneMatSetTevColors(mSlotPane6, &q3, &q4);
            PaneMatSetTevColors(mSlotPane7, &q1, &q2);
        }

        // row D: 0x11 + RoundHalfAway0 -> panes 0x40/0x48
        {
            s16 pb = (s16)(s32)(stA->f10 + (f32)RoundHalfAway0(0.01f * stB->f10 * (f32)(stC->s06 + e1 + GetEffect(member, 0x11, NULL))));
            s16 nb = (s16)(s32)(stA->f10 + (f32)RoundHalfAway0(0.01f * stB->f10 * (f32)(stC->s06 + e2 + GetEffect(member, 0x11, arg3))));
            if (pb > 9999) pb = 9999;
            if (nb > 9999) nb = 9999;
            s16 dD = (s16)(nb - pb);
            if (stats->CActorParam_getHp() > (f32)nb) {
                LayoutSetTextBoxInt((nw4r::lyt::Layout*)mSlotPane0, nb);
            }
            LayoutSetTextBoxInt((nw4r::lyt::Layout*)mSlotPane2, nb);
            E43Quad q1 = *(E43Quad*)&lbl_eu_80664518;
            E43Quad q2 = *(E43Quad*)&lbl_eu_80664520;
            E43Quad q3 = *(E43Quad*)&lbl_eu_80664558;
            E43Quad q4 = *(E43Quad*)&lbl_eu_80664560;
            if (dD < 0) {
                __as__11_GXColorS10FRC11_GXColorS10(&q1, &lbl_eu_80664538);
                __as__11_GXColorS10FRC11_GXColorS10(&q2, &lbl_eu_80664540);
                __as__11_GXColorS10FRC11_GXColorS10(&q3, &lbl_eu_80664578);
                __as__11_GXColorS10FRC11_GXColorS10(&q4, &lbl_eu_80664580);
            } else if (dD > 0) {
                __as__11_GXColorS10FRC11_GXColorS10(&q1, &lbl_eu_80664528);
                __as__11_GXColorS10FRC11_GXColorS10(&q2, &lbl_eu_80664530);
                __as__11_GXColorS10FRC11_GXColorS10(&q3, &lbl_eu_80664568);
                __as__11_GXColorS10FRC11_GXColorS10(&q4, &lbl_eu_80664570);
            }
            PaneMatSetTevColors(mSlotPane0, &q3, &q4);
            PaneMatSetTevColors(mSlotPane2, &q3, &q4);
            PaneMatSetTevColors(mSlotPane3, &q3, &q4);
            PaneMatSetTevColors(mSlotPane1, &q1, &q2);
        }
        // ---- type switch: slot id + item ----
        s16 slotId = 0;
        // (was switch: retail compiles this dispatch as compares)
        if (type == 2) slotId = *(s16*)((u8*)charObj + 0x26);
        else if (type == 4) slotId = *(s16*)((u8*)charObj + 0x1C);
        else if (type == 5) slotId = *(s16*)((u8*)charObj + 0x1E);
        else if (type == 6) slotId = *(s16*)((u8*)charObj + 0x20);
        else if (type == 7) slotId = *(s16*)((u8*)charObj + 0x22);
        else if (type == 8) slotId = *(s16*)((u8*)charObj + 0x24);
        void* item = CItem_kindPtr(type, slotId);
        if (type == 2) {
            // ---- weapon block (0x801E7300) ----
            u16 w0 = (item != NULL && *(u32*)item != 0) ? (u16)(*(u32*)item >> 20) : 0;
            E43Entry e_cur;
            e_cur.rec = BuildCmpRecord((void*)(u32)w0);
            E43Entry c_cur = e_cur;
            E43Entry e_new;
            e_new.rec = BuildCmpRecord(arg3);
            E43Entry c_new = e_new;
            s32 eq1 = 0;
            if (TestEquip(member, NULL)) eq1 = GetCondStatById(member, 0x30);
            s32 eq2 = 0;
            if (TestEquip(member, arg3)) eq2 = GetCondStatById(member, 0x30);
            s16 pb = (s16)(s32)(0.01f * ((100.0f + (f32)stC->s0C + (f32)eq1) * (f32)(stA->s1C + GetEffect(member, 0x1, NULL))));
            s16 nb = (s16)(s32)(0.01f * ((100.0f + (f32)stC->s0C + (f32)eq2) * (f32)(stA->s1C + GetEffect(member, 0x1, NULL))));
            s32 atkA = (s32)pb + CalcBonus1(member, (u16)c_cur.w04, NULL);
            s32 atkB = (s32)nb + CalcBonus1(member, (u16)c_new.w04, arg3);
            s32 atkC = (s32)atkA + CalcBonus2(member, (u16)(c_cur.w04 >> 16), NULL);
            s32 atkD = atkB < atkC ? atkB : atkC;
            s16 dW = (s16)(atkC - atkA);
            s16 v610, v612, v614, v644, v646, v648;
            s32 c1 = GetEffect(member, 0x31, arg3);
            v612 = (s16)(s32)(0.01f * ((100.0f + (f32)stC->s1C) * (f32)((stA->s32 - v646) + (c1 + v612))));
            c1 = GetEffect(member, 0x51, arg3);
            v646 = (s16)(s32)(0.01f * ((100.0f + (f32)stC->s1C) * (f32)(stA->s32 + c1)));
            c1 = GetEffect(member, 0x51, NULL);
            v610 = (s16)(s32)(0.01f * ((100.0f + (f32)stC->s18) * (f32)((stA->s2E - v644) + (c1 + v610))));
            c1 = GetEffect(member, 0x54, arg3);
            v644 = (s16)(s32)(0.01f * ((100.0f + (f32)stC->s18) * (f32)(stA->s2E + c1)));
            c1 = GetEffect(member, 0x54, NULL);
            v614 = (s16)(s32)(0.01f * ((100.0f + (f32)stC->s22) * (f32)((stA->s38 - v648) + (c1 + v614))));
            c1 = GetEffect(member, 0x54, NULL);
            v648 = (s16)(s32)(0.01f * ((100.0f + (f32)stC->s22) * (f32)(stA->s38 + c1)));
            s16 snap644 = v644;
            s16 snap646 = v646;
            s16 d1 = (s16)(v610 - snap644);
            s16 d2 = (s16)(v612 - snap646);
            s16 d3 = (s16)(c_new.w14 - c_cur.w14);
            s16 d4 = (s16)(v614 - v648);
            if (arg5 == 0) {
                dW = 0;
                d1 = 0;
                d2 = 0;
                d3 = 0;
                d4 = 0;
            }
            ml::FixStr<32> fmtBuf;
            fmtBuf.format(&lbl_eu_805063BC[0x254], atkD, BdatTouchStringCell(&lbl_eu_805063BC[0x130], &lbl_eu_805063BC[0x139], 0xB), atkC);
            SetLayoutSelfText((nw4r::lyt::Layout*)mSlotPane10, fmtBuf.c_str(), 0);
            s16 disp1 = v610;
            s16 disp2 = v612;
            s16 disp3 = v614;
            if (arg5 == 0) {
                disp1 = snap644;
                disp2 = snap646;
                disp3 = v648;
            }
            LayoutSetTextBoxInt((nw4r::lyt::Layout*)mSlotPane12, (s16)disp1);
            LayoutSetTextBoxInt((nw4r::lyt::Layout*)mSlotPane14, (s16)disp2);
            ml::FixStr<32> fmtBuf2;
            fmtBuf2.format(&lbl_eu_805063BC[0x13E], (s16)disp3, BdatTouchStringCell(&lbl_eu_805063BC[0x130], &lbl_eu_805063BC[0x139], 0x80));
            SetLayoutSelfText((nw4r::lyt::Layout*)mSlotPane16, fmtBuf2.c_str(), 0);
            ml::FixStr<32> fmtBuf3;
            fmtBuf3.format(&lbl_eu_805063BC[0x13E], (s32)stA->b55 + (s32)d3, BdatTouchStringCell(&lbl_eu_805063BC[0x130], &lbl_eu_805063BC[0x139], 0x80));
            SetLayoutSelfText((nw4r::lyt::Layout*)mSlotPane18, fmtBuf3.c_str(), 0);
            // delta colors (5 rows)
            {
                E43Quad q1 = *(E43Quad*)&lbl_eu_80664518;
                E43Quad q2 = *(E43Quad*)&lbl_eu_80664520;
                E43Quad q3 = *(E43Quad*)&lbl_eu_80664558;
                E43Quad q4 = *(E43Quad*)&lbl_eu_80664560;
                if (dW < 0) {
                    __as__11_GXColorS10FRC11_GXColorS10(&q1, &lbl_eu_80664538);
                    __as__11_GXColorS10FRC11_GXColorS10(&q2, &lbl_eu_80664540);
                    __as__11_GXColorS10FRC11_GXColorS10(&q3, &lbl_eu_80664578);
                    __as__11_GXColorS10FRC11_GXColorS10(&q4, &lbl_eu_80664580);
                } else if (dW > 0) {
                    __as__11_GXColorS10FRC11_GXColorS10(&q1, &lbl_eu_80664528);
                    __as__11_GXColorS10FRC11_GXColorS10(&q2, &lbl_eu_80664530);
                    __as__11_GXColorS10FRC11_GXColorS10(&q3, &lbl_eu_80664568);
                    __as__11_GXColorS10FRC11_GXColorS10(&q4, &lbl_eu_80664570);
                }
                PaneMatSetTevColors(mSlotPane10, &q3, &q4);
                PaneMatSetTevColors(mSlotPane11, &q1, &q2);
            }
            {
                E43Quad q1 = *(E43Quad*)&lbl_eu_80664518;
                E43Quad q2 = *(E43Quad*)&lbl_eu_80664520;
                E43Quad q3 = *(E43Quad*)&lbl_eu_80664558;
                E43Quad q4 = *(E43Quad*)&lbl_eu_80664560;
                if (d1 < 0) {
                    __as__11_GXColorS10FRC11_GXColorS10(&q1, &lbl_eu_80664538);
                    __as__11_GXColorS10FRC11_GXColorS10(&q2, &lbl_eu_80664540);
                    __as__11_GXColorS10FRC11_GXColorS10(&q3, &lbl_eu_80664578);
                    __as__11_GXColorS10FRC11_GXColorS10(&q4, &lbl_eu_80664580);
                } else if (d1 > 0) {
                    __as__11_GXColorS10FRC11_GXColorS10(&q1, &lbl_eu_80664528);
                    __as__11_GXColorS10FRC11_GXColorS10(&q2, &lbl_eu_80664530);
                    __as__11_GXColorS10FRC11_GXColorS10(&q3, &lbl_eu_80664568);
                    __as__11_GXColorS10FRC11_GXColorS10(&q4, &lbl_eu_80664570);
                }
                PaneMatSetTevColors(mSlotPane12, &q3, &q4);
                PaneMatSetTevColors(mSlotPane13, &q1, &q2);
            }
            {
                E43Quad q1 = *(E43Quad*)&lbl_eu_80664518;
                E43Quad q2 = *(E43Quad*)&lbl_eu_80664520;
                E43Quad q3 = *(E43Quad*)&lbl_eu_80664558;
                E43Quad q4 = *(E43Quad*)&lbl_eu_80664560;
                if (d2 < 0) {
                    __as__11_GXColorS10FRC11_GXColorS10(&q1, &lbl_eu_80664538);
                    __as__11_GXColorS10FRC11_GXColorS10(&q2, &lbl_eu_80664540);
                    __as__11_GXColorS10FRC11_GXColorS10(&q3, &lbl_eu_80664578);
                    __as__11_GXColorS10FRC11_GXColorS10(&q4, &lbl_eu_80664580);
                } else if (d2 > 0) {
                    __as__11_GXColorS10FRC11_GXColorS10(&q1, &lbl_eu_80664528);
                    __as__11_GXColorS10FRC11_GXColorS10(&q2, &lbl_eu_80664530);
                    __as__11_GXColorS10FRC11_GXColorS10(&q3, &lbl_eu_80664568);
                    __as__11_GXColorS10FRC11_GXColorS10(&q4, &lbl_eu_80664570);
                }
                PaneMatSetTevColors(mSlotPane14, &q3, &q4);
                PaneMatSetTevColors(mSlotPane15, &q1, &q2);
            }
            {
                E43Quad q1 = *(E43Quad*)&lbl_eu_80664518;
                E43Quad q2 = *(E43Quad*)&lbl_eu_80664520;
                E43Quad q3 = *(E43Quad*)&lbl_eu_80664558;
                E43Quad q4 = *(E43Quad*)&lbl_eu_80664560;
                if (d4 < 0) {
                    __as__11_GXColorS10FRC11_GXColorS10(&q1, &lbl_eu_80664538);
                    __as__11_GXColorS10FRC11_GXColorS10(&q2, &lbl_eu_80664540);
                    __as__11_GXColorS10FRC11_GXColorS10(&q3, &lbl_eu_80664578);
                    __as__11_GXColorS10FRC11_GXColorS10(&q4, &lbl_eu_80664580);
                } else if (d4 > 0) {
                    __as__11_GXColorS10FRC11_GXColorS10(&q1, &lbl_eu_80664528);
                    __as__11_GXColorS10FRC11_GXColorS10(&q2, &lbl_eu_80664530);
                    __as__11_GXColorS10FRC11_GXColorS10(&q3, &lbl_eu_80664568);
                    __as__11_GXColorS10FRC11_GXColorS10(&q4, &lbl_eu_80664570);
                }
                PaneMatSetTevColors(mSlotPane16, &q3, &q4);
                PaneMatSetTevColors(mSlotPane17, &q1, &q2);
            }
            {
                E43Quad q1 = *(E43Quad*)&lbl_eu_80664518;
                E43Quad q2 = *(E43Quad*)&lbl_eu_80664520;
                E43Quad q3 = *(E43Quad*)&lbl_eu_80664558;
                E43Quad q4 = *(E43Quad*)&lbl_eu_80664560;
                if (d3 < 0) {
                    __as__11_GXColorS10FRC11_GXColorS10(&q1, &lbl_eu_80664538);
                    __as__11_GXColorS10FRC11_GXColorS10(&q2, &lbl_eu_80664540);
                    __as__11_GXColorS10FRC11_GXColorS10(&q3, &lbl_eu_80664578);
                    __as__11_GXColorS10FRC11_GXColorS10(&q4, &lbl_eu_80664580);
                } else if (d3 > 0) {
                    __as__11_GXColorS10FRC11_GXColorS10(&q1, &lbl_eu_80664528);
                    __as__11_GXColorS10FRC11_GXColorS10(&q2, &lbl_eu_80664530);
                    __as__11_GXColorS10FRC11_GXColorS10(&q3, &lbl_eu_80664568);
                    __as__11_GXColorS10FRC11_GXColorS10(&q4, &lbl_eu_80664570);
                }
                PaneMatSetTevColors(mSlotPane18, &q3, &q4);
                PaneMatSetTevColors(mSlotPane19, &q1, &q2);
            }
        } else {
            // ---- armor block (0x801E7DA0) ----
            u16 w0 = (item != NULL && *(u32*)item != 0) ? (u16)(*(u32*)item >> 20) : 0;
            E43Entry e_cur;
            const CItemBoxSlotFlags& e_curFlags = BuildSlotFlags(w0);
            e_cur = *(const E43Entry*)&e_curFlags;
            E43Entry c_cur = e_cur;
            E43Entry e_new;
            const CItemBoxSlotFlags& e_newFlags = BuildSlotFlags((u32)arg3);
            e_new = *(const E43Entry*)&e_newFlags;
            E43Entry c_new = e_new;
            s32 eq1 = 0;
            if (TestEquip(member, NULL)) eq1 = GetCondStatById(member, 0x30);
            s32 eq2 = 0;
            if (TestEquip(member, arg3)) eq2 = GetCondStatById(member, 0x30);
            u16 v484 = (u16)SumTypeCounts(member, (void*)(u32)w0);
            u16 v468 = (u16)SumTypeCounts(member, arg3);
            // arm-type scaling of the entry value pairs
            u8 arm1 = (u8)BdatGetU8Direct((const void*)lbl_eu_806640F8, &lbl_eu_805063BC[0x1EB], BdatGetItemId((u32)w0));
            if (arm1 == 3) {
                s32 v = GetCondStatById(member, 0xD);
                if (v != 0) {
                    c_cur.w04 = (u32)(s32)((f32)(s32)c_cur.w04 * (0.01f * (100.0f + (f32)v)));
                    c_cur.w14 = (s16)(s32)((f32)c_cur.w14 * (0.01f * (100.0f + (f32)v)));
                }
                v = GetCondStatById(member, 0x24);
                if (v != 0) {
                    c_cur.w14 = (s16)((s32)c_cur.w14 + (s16)v);
                }
            } else if (arm1 == 2) {
                s32 v = GetCondStatById(member, 0xC);
                if (v != 0) {
                    c_cur.w04 = (u32)(s32)((f32)(s32)c_cur.w04 * (0.01f * (100.0f + (f32)v)));
                    c_cur.w14 = (s16)(s32)((f32)c_cur.w14 * (0.01f * (100.0f + (f32)v)));
                }
            } else if (arm1 == 1) {
                s32 v = GetCondStatById(member, 0xB);
                if (v != 0) {
                    c_cur.w04 = (u32)(s32)((f32)(s32)c_cur.w04 * (0.01f * (100.0f + (f32)v)));
                    c_cur.w14 = (s16)(s32)((f32)c_cur.w14 * (0.01f * (100.0f + (f32)v)));
                }
            }
            u8 arm2 = (u8)BdatGetU8Direct((const void*)lbl_eu_806640F8, &lbl_eu_805063BC[0x1EB], BdatGetItemId((u32)(*(u32*)arg3 >> 20)));
            if (arm2 == 3) {
                s32 v = GetCondStatById(member, 0xD);
                if (v != 0) {
                    c_new.w04 = (u32)(s32)((f32)(s32)c_new.w04 * (0.01f * (100.0f + (f32)v)));
                    c_new.w14 = (s16)(s32)((f32)c_new.w14 * (0.01f * (100.0f + (f32)v)));
                }
                v = GetCondStatById(member, 0x24);
                if (v != 0) {
                    c_new.w14 = (s16)((s32)c_new.w14 + (s16)v);
                }
            } else if (arm2 == 2) {
                s32 v = GetCondStatById(member, 0xC);
                if (v != 0) {
                    c_new.w04 = (u32)(s32)((f32)(s32)c_new.w04 * (0.01f * (100.0f + (f32)v)));
                    c_new.w14 = (s16)(s32)((f32)c_new.w14 * (0.01f * (100.0f + (f32)v)));
                }
            } else if (arm2 == 1) {
                s32 v = GetCondStatById(member, 0xB);
                if (v != 0) {
                    c_new.w04 = (u32)(s32)((f32)(s32)c_new.w04 * (0.01f * (100.0f + (f32)v)));
                    c_new.w14 = (s16)(s32)((f32)c_new.w14 * (0.01f * (100.0f + (f32)v)));
                }
            }
            // weight
            s32 wv = GetCondStatById(member, 0x97);
            if (wv != 0) {
                s32 t1 = (s32)v484 - wv;
                if (t1 < 0) t1 = 0;
                v484 = (u16)t1;
                s32 t2 = (s32)v468 - wv;
                if (t2 < 0) t2 = 0;
                v468 = (u16)t2;
            }
            // slot sum
            s32 ssum = (s32)*(s16*)((u8*)charObj + 0x22) + *(s16*)((u8*)charObj + 0x20) + *(s16*)((u8*)charObj + 0x24) + *(s16*)((u8*)charObj + 0x1C) + *(s16*)((u8*)charObj + 0x1E);
            if (ssum <= -5) {
                GetCondStatById(member, 0x1A);
            }
            // bars (armor stat pair updates)
            s32 c1 = GetEffect(member, 0x31, arg3);
            c_new.w14 = (s16)(s32)(0.01f * ((100.0f + (f32)stC->s1C) * (f32)((stA->s32 - (s16)(u16)c_cur.w04) + (c1 + c_new.w14))));
            c1 = GetEffect(member, 0x51, NULL);
            c_new.w04 = (u32)(s16)(s32)(0.01f * ((100.0f + (f32)stC->s18) * (f32)((stA->s2E - (s16)(u16)c_cur.w04) + (c1 + (s16)(u16)c_new.w04))));
            c1 = GetEffect(member, 0x41, NULL);
            c_cur.w04 = (u32)(s16)(s32)(0.01f * ((100.0f + (f32)stC->s18) * (f32)(stA->s2E + c1)));
            s16 d1 = (s16)(c_new.w14 - (s16)(u16)c_cur.w14);
            s16 d2 = (s16)((s16)(u16)c_new.w04 - (s16)(u16)c_cur.w04);
            s32 c2 = GetEffect(member, 0x41, arg3);
            s16 b27 = (s16)(s32)(0.01f * ((100.0f + (f32)stC->s0E + (f32)eq1) * (f32)((stA->s1E - (s16)v484) + c2)));
            c2 = GetEffect(member, 0x41, NULL);
            s16 b14 = (s16)(s32)(0.01f * ((100.0f + (f32)stC->s0E + (f32)eq2) * (f32)((stA->s1E - (s16)v468) + c2)));
            if (b27 <= 0) b27 = 1;
            if (b14 <= 0) b14 = 1;
            s16 dW = (s16)(b14 - b27);
            // embedded weapon block
            void* item2 = CItem_kindPtr(2, *(s16*)((u8*)charObj + 0x26));
            if (item2 != NULL && *(u32*)item2 != 0) {
                E43Entry ew;
                ew.rec = BuildCmpRecord((void*)(u32)(u16)(*(u32*)item2 >> 20));
                E43Entry cw = ew;
                s32 c3 = GetEffect(member, 0x1, NULL);
                s16 wpb = (s16)(s32)(0.01f * ((100.0f + (f32)stC->s0C + (f32)eq1) * (f32)(stA->s1C + c3)));
                c3 = GetEffect(member, 0x1, NULL);
                s16 wnb = (s16)(s32)(0.01f * ((100.0f + (f32)stC->s0C + (f32)eq2) * (f32)(stA->s1C + c3)));
                s32 wa = (s32)wpb + CalcBonus1(member, (u16)cw.w04, NULL);
                s32 wb = (s32)wnb + CalcBonus2(member, (u16)(cw.w04 >> 16), NULL);
                s32 wc = (s32)wnb + CalcBonus1(member, (u16)cw.w04, arg3);
                s32 wd = (s32)wnb + CalcBonus2(member, (u16)(cw.w04 >> 16), arg3);
                s32 wmin = wc < wd ? wc : wd;
                dW = (s16)(wd - wa);
                ml::FixStr<32> fmtBuf;
                fmtBuf.format(&lbl_eu_805063BC[0x254], wmin, BdatTouchStringCell(&lbl_eu_805063BC[0x130], &lbl_eu_805063BC[0x139], 0xB), wd);
                SetLayoutSelfText((nw4r::lyt::Layout*)mSlotPane10, fmtBuf.c_str(), 0);
            }
            // numbers
            LayoutSetTextBoxInt((nw4r::lyt::Layout*)mSlotPane12, (s16)(u16)c_new.w04);
            LayoutSetTextBoxInt((nw4r::lyt::Layout*)mSlotPane14, (s16)c_new.w14);
            LayoutSetTextBoxInt((nw4r::lyt::Layout*)mSlotPane6, (s16)b14);
            // delta colors (4 rows)
            {
                E43Quad q1 = *(E43Quad*)&lbl_eu_80664518;
                E43Quad q2 = *(E43Quad*)&lbl_eu_80664520;
                E43Quad q3 = *(E43Quad*)&lbl_eu_80664558;
                E43Quad q4 = *(E43Quad*)&lbl_eu_80664560;
                if (dW < 0) {
                    __as__11_GXColorS10FRC11_GXColorS10(&q1, &lbl_eu_80664538);
                    __as__11_GXColorS10FRC11_GXColorS10(&q2, &lbl_eu_80664540);
                    __as__11_GXColorS10FRC11_GXColorS10(&q3, &lbl_eu_80664578);
                    __as__11_GXColorS10FRC11_GXColorS10(&q4, &lbl_eu_80664580);
                } else if (dW > 0) {
                    __as__11_GXColorS10FRC11_GXColorS10(&q1, &lbl_eu_80664528);
                    __as__11_GXColorS10FRC11_GXColorS10(&q2, &lbl_eu_80664530);
                    __as__11_GXColorS10FRC11_GXColorS10(&q3, &lbl_eu_80664568);
                    __as__11_GXColorS10FRC11_GXColorS10(&q4, &lbl_eu_80664570);
                }
                PaneMatSetTevColors(mSlotPane10, &q3, &q4);
                PaneMatSetTevColors(mSlotPane11, &q1, &q2);
            }
            {
                E43Quad q1 = *(E43Quad*)&lbl_eu_80664518;
                E43Quad q2 = *(E43Quad*)&lbl_eu_80664520;
                E43Quad q3 = *(E43Quad*)&lbl_eu_80664558;
                E43Quad q4 = *(E43Quad*)&lbl_eu_80664560;
                if (d2 < 0) {
                    __as__11_GXColorS10FRC11_GXColorS10(&q1, &lbl_eu_80664538);
                    __as__11_GXColorS10FRC11_GXColorS10(&q2, &lbl_eu_80664540);
                    __as__11_GXColorS10FRC11_GXColorS10(&q3, &lbl_eu_80664578);
                    __as__11_GXColorS10FRC11_GXColorS10(&q4, &lbl_eu_80664580);
                } else if (d2 > 0) {
                    __as__11_GXColorS10FRC11_GXColorS10(&q1, &lbl_eu_80664528);
                    __as__11_GXColorS10FRC11_GXColorS10(&q2, &lbl_eu_80664530);
                    __as__11_GXColorS10FRC11_GXColorS10(&q3, &lbl_eu_80664568);
                    __as__11_GXColorS10FRC11_GXColorS10(&q4, &lbl_eu_80664570);
                }
                PaneMatSetTevColors(mSlotPane12, &q3, &q4);
                PaneMatSetTevColors(mSlotPane13, &q1, &q2);
            }
            {
                E43Quad q1 = *(E43Quad*)&lbl_eu_80664518;
                E43Quad q2 = *(E43Quad*)&lbl_eu_80664520;
                E43Quad q3 = *(E43Quad*)&lbl_eu_80664558;
                E43Quad q4 = *(E43Quad*)&lbl_eu_80664560;
                if (d1 < 0) {
                    __as__11_GXColorS10FRC11_GXColorS10(&q1, &lbl_eu_80664538);
                    __as__11_GXColorS10FRC11_GXColorS10(&q2, &lbl_eu_80664540);
                    __as__11_GXColorS10FRC11_GXColorS10(&q3, &lbl_eu_80664578);
                    __as__11_GXColorS10FRC11_GXColorS10(&q4, &lbl_eu_80664580);
                } else if (d1 > 0) {
                    __as__11_GXColorS10FRC11_GXColorS10(&q1, &lbl_eu_80664528);
                    __as__11_GXColorS10FRC11_GXColorS10(&q2, &lbl_eu_80664530);
                    __as__11_GXColorS10FRC11_GXColorS10(&q3, &lbl_eu_80664568);
                    __as__11_GXColorS10FRC11_GXColorS10(&q4, &lbl_eu_80664570);
                }
                PaneMatSetTevColors(mSlotPane14, &q3, &q4);
                PaneMatSetTevColors(mSlotPane15, &q1, &q2);
            }
            {
                E43Quad q1 = *(E43Quad*)&lbl_eu_80664518;
                E43Quad q2 = *(E43Quad*)&lbl_eu_80664520;
                E43Quad q3 = *(E43Quad*)&lbl_eu_80664558;
                E43Quad q4 = *(E43Quad*)&lbl_eu_80664560;
                if (dW < 0) {
                    __as__11_GXColorS10FRC11_GXColorS10(&q1, &lbl_eu_80664538);
                    __as__11_GXColorS10FRC11_GXColorS10(&q2, &lbl_eu_80664540);
                    __as__11_GXColorS10FRC11_GXColorS10(&q3, &lbl_eu_80664578);
                    __as__11_GXColorS10FRC11_GXColorS10(&q4, &lbl_eu_80664580);
                } else if (dW > 0) {
                    __as__11_GXColorS10FRC11_GXColorS10(&q1, &lbl_eu_80664528);
                    __as__11_GXColorS10FRC11_GXColorS10(&q2, &lbl_eu_80664530);
                    __as__11_GXColorS10FRC11_GXColorS10(&q3, &lbl_eu_80664568);
                    __as__11_GXColorS10FRC11_GXColorS10(&q4, &lbl_eu_80664570);
                }
                PaneMatSetTevColors(mSlotPane6, &q3, &q4);
                PaneMatSetTevColors(mSlotPane7, &q1, &q2);
            }
        }

    }

    // ---- equip-detail / simple path (arg5 == 0, type in {2,4,5,6,7,8}) ----
    if (arg5 == 0 && (type == 2 || (type >= 4 && type <= 8))) {
        if (type != 2 && TestBdatFlag(member, (u32)arg3)) {
            // ---- equip detail (0x801E8C88) ----
            s16 slotId = 0;
            switch (type) {
                case 4: slotId = *(s16*)((u8*)charObj + 0x1C); break;
                case 5: slotId = *(s16*)((u8*)charObj + 0x1E); break;
                case 6: slotId = *(s16*)((u8*)charObj + 0x20); break;
                case 7: slotId = *(s16*)((u8*)charObj + 0x22); break;
                case 8: slotId = *(s16*)((u8*)charObj + 0x24); break;
            }
            void* item = CItem_kindPtr(type, slotId);
            u16 w0 = (item != NULL && *(u32*)item != 0) ? (u16)(*(u32*)item >> 20) : 0;
            E43Entry e_cur;
            const CItemBoxSlotFlags& e_curFlags = BuildSlotFlags(w0);
            e_cur = *(const E43Entry*)&e_curFlags;
            E43Entry c_cur = e_cur;
            E43Entry e_new;
            const CItemBoxSlotFlags& e_newFlags = BuildSlotFlags((u32)arg3);
            e_new = *(const E43Entry*)&e_newFlags;
            E43Entry c_new = e_new;
            u16 v44C = (u16)SumTypeCounts(member, (void*)(u32)w0);
            u16 v430 = (u16)SumTypeCounts(member, arg3);
            s32 eq1 = 0;
            if (TestEquip(member, NULL)) eq1 = GetCondStatById(member, 0x30);
            s32 eq2 = 0;
            if (TestEquip(member, arg3)) eq2 = GetCondStatById(member, 0x30);
            GetEffect(member, 0x21, NULL);
            s16 bA = (s16)(s32)(0.01f * ((100.0f + (f32)stC->s10 + (f32)eq2) * (f32)(stA->s20 + GetEffect(member, 0x21, arg3))));
            LayoutSetTextBoxInt((nw4r::lyt::Layout*)mSlotPane8, (s16)bA);
            GetEffect(member, 0x1, NULL);
            s16 bB = (s16)(s32)(0.01f * ((100.0f + (f32)stC->s0C + (f32)eq2) * (f32)(stA->s1C + GetEffect(member, 0x1, arg3))));
            LayoutSetTextBoxInt((nw4r::lyt::Layout*)mSlotPane4, (s16)bB);
            RoundHalfAway0(0.01f * stB->f10 * (f32)(stC->s06 + eq1 + GetEffect(member, 0x11, NULL)));
            s32 statValue = RoundHalfAway0(0.01f * stB->f10 * (f32)(stC->s06 + eq2 + GetEffect(member, 0x11, arg3)));
            s16 nb = (s16)(s32)(stA->f10 + (f32)statValue);
            if (nb > 9999) nb = 9999;
            if (stats->CActorParam_getHp() > (f32)nb) {
                LayoutSetTextBoxInt((nw4r::lyt::Layout*)mSlotPane0, nb);
            }
            LayoutSetTextBoxInt((nw4r::lyt::Layout*)mSlotPane2, nb);
            // arm-type scaling of entry value pairs
            u8 arm1 = (u8)BdatGetU8Direct((const void*)lbl_eu_806640F8, &lbl_eu_805063BC[0x1EB], BdatGetItemId((u32)w0));
            if (arm1 == 3) {
                s32 v = GetCondStatById(member, 0xD);
                if (v != 0) {
                    c_cur.w04 = (u32)(s32)((f32)(s32)c_cur.w04 * (0.01f * (100.0f + (f32)v)));
                    c_cur.w14 = (s16)(s32)((f32)c_cur.w14 * (0.01f * (100.0f + (f32)v)));
                }
                v = GetCondStatById(member, 0x24);
                if (v != 0) {
                    c_cur.w14 = (s16)((s32)c_cur.w14 + (s16)v);
                }
            } else if (arm1 == 2) {
                s32 v = GetCondStatById(member, 0xC);
                if (v != 0) {
                    c_cur.w04 = (u32)(s32)((f32)(s32)c_cur.w04 * (0.01f * (100.0f + (f32)v)));
                    c_cur.w14 = (s16)(s32)((f32)c_cur.w14 * (0.01f * (100.0f + (f32)v)));
                }
            } else if (arm1 == 1) {
                s32 v = GetCondStatById(member, 0xB);
                if (v != 0) {
                    c_cur.w04 = (u32)(s32)((f32)(s32)c_cur.w04 * (0.01f * (100.0f + (f32)v)));
                    c_cur.w14 = (s16)(s32)((f32)c_cur.w14 * (0.01f * (100.0f + (f32)v)));
                }
            }
            u8 arm2 = (u8)BdatGetU8Direct((const void*)lbl_eu_806640F8, &lbl_eu_805063BC[0x1EB], BdatGetItemId((u32)(*(u32*)arg3 >> 20)));
            if (arm2 == 3) {
                s32 v = GetCondStatById(member, 0xD);
                if (v != 0) {
                    c_new.w04 = (u32)(s32)((f32)(s32)c_new.w04 * (0.01f * (100.0f + (f32)v)));
                    c_new.w14 = (s16)(s32)((f32)c_new.w14 * (0.01f * (100.0f + (f32)v)));
                }
                v = GetCondStatById(member, 0x24);
                if (v != 0) {
                    c_new.w14 = (s16)((s32)c_new.w14 + (s16)v);
                }
            } else if (arm2 == 2) {
                s32 v = GetCondStatById(member, 0xC);
                if (v != 0) {
                    c_new.w04 = (u32)(s32)((f32)(s32)c_new.w04 * (0.01f * (100.0f + (f32)v)));
                    c_new.w14 = (s16)(s32)((f32)c_new.w14 * (0.01f * (100.0f + (f32)v)));
                }
            } else if (arm2 == 1) {
                s32 v = GetCondStatById(member, 0xB);
                if (v != 0) {
                    c_new.w04 = (u32)(s32)((f32)(s32)c_new.w04 * (0.01f * (100.0f + (f32)v)));
                    c_new.w14 = (s16)(s32)((f32)c_new.w14 * (0.01f * (100.0f + (f32)v)));
                }
            }
            // weight
            s32 wv = GetCondStatById(member, 0x97);
            if (wv != 0) {
                s32 t1 = (s32)v44C - wv;
                if (t1 < 0) t1 = 0;
                v44C = (u16)t1;
                s32 t2 = (s32)v430 - wv;
                if (t2 < 0) t2 = 0;
                v430 = (u16)t2;
            }
            // slot sum
            s32 ssum = (s32)*(s16*)((u8*)charObj + 0x22) + *(s16*)((u8*)charObj + 0x20) + *(s16*)((u8*)charObj + 0x24) + *(s16*)((u8*)charObj + 0x1C) + *(s16*)((u8*)charObj + 0x1E);
            if (ssum <= -5) {
                GetCondStatById(member, 0x1A);
            }
            // bars (armor stat pair updates)
            s32 c1 = GetEffect(member, 0x31, arg3);
            c_new.w14 = (s16)(s32)(0.01f * ((100.0f + (f32)stC->s1C) * (f32)((stA->s32 - (s16)(u16)c_cur.w04) + (c1 + c_new.w14))));
            c1 = GetEffect(member, 0x51, arg3);
            c_new.w04 = (u32)(s16)(s32)(0.01f * ((100.0f + (f32)stC->s18) * (f32)(stA->s2E + c1)));
            c1 = GetEffect(member, 0x51, NULL);
            c_new.w04 = (u32)(s16)(s32)(0.01f * ((100.0f + (f32)stC->s18) * (f32)((stA->s2E - (s16)(u16)c_cur.w04) + (c1 + (s16)(u16)c_new.w04))));
            c1 = GetEffect(member, 0x41, NULL);
            c_cur.w04 = (u32)(s16)(s32)(0.01f * ((100.0f + (f32)stC->s18) * (f32)(stA->s2E + c1)));
            s32 c2 = GetEffect(member, 0x41, arg3);
            s16 b14 = (s16)(s32)(0.01f * ((100.0f + (f32)stC->s0E + (f32)eq2) * (f32)((stA->s1E - (s16)v430) + c2)));
            if (b14 <= 0) b14 = 1;
            // embedded weapon block
            void* item2 = CItem_kindPtr(2, *(s16*)((u8*)charObj + 0x26));
            if (item2 != NULL && *(u32*)item2 != 0) {
                E43Entry ew;
                ew.rec = BuildCmpRecord((void*)(u32)(u16)(*(u32*)item2 >> 20));
                E43Entry cw = ew;
                GetEffect(member, 0x1, NULL);
                s16 wpb = (s16)(s32)(0.01f * ((100.0f + (f32)stC->s0C + (f32)eq2) * (f32)(stA->s1C + GetEffect(member, 0x1, arg3))));
                s32 wa = (s32)wpb + CalcBonus1(member, (u16)cw.w04, NULL);
                s32 wb = (s32)wpb + CalcBonus2(member, (u16)(cw.w04 >> 16), NULL);
                s32 wc = (s32)wpb + CalcBonus1(member, (u16)cw.w04, arg3);
                s32 wd = (s32)wpb + CalcBonus2(member, (u16)(cw.w04 >> 16), arg3);
                s32 wmin = wc < wd ? wc : wd;
                ml::FixStr<32> fmtBuf;
                fmtBuf.format(&lbl_eu_805063BC[0x254], wmin, BdatTouchStringCell(&lbl_eu_805063BC[0x130], &lbl_eu_805063BC[0x139], 0xB), wd);
                SetLayoutSelfText((nw4r::lyt::Layout*)mSlotPane10, fmtBuf.c_str(), 0);
            }
            // numbers
            LayoutSetTextBoxInt((nw4r::lyt::Layout*)mSlotPane12, (s16)(u16)c_new.w04);
            LayoutSetTextBoxInt((nw4r::lyt::Layout*)mSlotPane14, (s16)c_new.w14);
            LayoutSetTextBoxInt((nw4r::lyt::Layout*)mSlotPane6, (s16)b14);
            // 20x color application (selected palette A8/B0/C8/D0)
            E43Quad q1 = *(E43Quad*)&lbl_eu_806645A8;
            E43Quad q2 = *(E43Quad*)&lbl_eu_806645B0;
            E43Quad q3 = *(E43Quad*)&lbl_eu_806645C8;
            E43Quad q4 = *(E43Quad*)&lbl_eu_806645D0;
            PaneMatSetTevColors(mSlotPane0, &q3, &q4);
            PaneMatSetTevColors(mSlotPane2, &q3, &q4);
            PaneMatSetTevColors(mSlotPane3, &q3, &q4);
            PaneMatSetTevColors(mSlotPane1, &q1, &q2);
            PaneMatSetTevColors(mSlotPane4, &q3, &q4);
            PaneMatSetTevColors(mSlotPane5, &q1, &q2);
            PaneMatSetTevColors(mSlotPane6, &q3, &q4);
            PaneMatSetTevColors(mSlotPane7, &q1, &q2);
            PaneMatSetTevColors(mSlotPane8, &q3, &q4);
            PaneMatSetTevColors(mSlotPane9, &q1, &q2);
            PaneMatSetTevColors(mSlotPane10, &q3, &q4);
            PaneMatSetTevColors(mSlotPane11, &q1, &q2);
            PaneMatSetTevColors(mSlotPane12, &q3, &q4);
            PaneMatSetTevColors(mSlotPane13, &q1, &q2);
            PaneMatSetTevColors(mSlotPane14, &q3, &q4);
            PaneMatSetTevColors(mSlotPane15, &q1, &q2);
            PaneMatSetTevColors(mSlotPane16, &q3, &q4);
            PaneMatSetTevColors(mSlotPane17, &q1, &q2);
            PaneMatSetTevColors(mSlotPane18, &q3, &q4);
            PaneMatSetTevColors(mSlotPane19, &q1, &q2);

        }
        // ---- simple path ----
        E43Quad q1 = *(E43Quad*)&lbl_eu_806645A8;
        E43Quad q2 = *(E43Quad*)&lbl_eu_806645B0;
        E43Quad q3 = *(E43Quad*)&lbl_eu_806645C8;
        E43Quad q4 = *(E43Quad*)&lbl_eu_806645D0;
        SetLayoutSelfText((nw4r::lyt::Layout*)mSlotPane0, &lbl_eu_805063BC[0x2AA], 0);
        SetLayoutSelfText((nw4r::lyt::Layout*)mSlotPane2, &lbl_eu_805063BC[0x2AA], 0);
        SetLayoutSelfText((nw4r::lyt::Layout*)mSlotPane3, &lbl_eu_805063BC[0x2AA], 0);
        PaneMatSetTevColors(mSlotPane1, &q1, &q2);
        SetLayoutSelfText((nw4r::lyt::Layout*)mSlotPane4, &lbl_eu_805063BC[0x2AA], 0);
        PaneMatSetTevColors(mSlotPane4, &q3, &q4);
        PaneMatSetTevColors(mSlotPane5, &q1, &q2);
        SetLayoutSelfText((nw4r::lyt::Layout*)mSlotPane6, &lbl_eu_805063BC[0x2AA], 0);
        PaneMatSetTevColors(mSlotPane6, &q3, &q4);
        PaneMatSetTevColors(mSlotPane7, &q1, &q2);
        SetLayoutSelfText((nw4r::lyt::Layout*)mSlotPane8, &lbl_eu_805063BC[0x2AA], 0);
        PaneMatSetTevColors(mSlotPane8, &q3, &q4);
        PaneMatSetTevColors(mSlotPane9, &q1, &q2);
        SetLayoutSelfText((nw4r::lyt::Layout*)mSlotPane10, &lbl_eu_805063BC[0x2AA], 0);
        PaneMatSetTevColors(mSlotPane10, &q3, &q4);
        PaneMatSetTevColors(mSlotPane11, &q1, &q2);
        SetLayoutSelfText((nw4r::lyt::Layout*)mSlotPane12, &lbl_eu_805063BC[0x2AA], 0);
        PaneMatSetTevColors(mSlotPane12, &q3, &q4);
        PaneMatSetTevColors(mSlotPane13, &q1, &q2);
        SetLayoutSelfText((nw4r::lyt::Layout*)mSlotPane14, &lbl_eu_805063BC[0x2AA], 0);
        PaneMatSetTevColors(mSlotPane14, &q3, &q4);
        PaneMatSetTevColors(mSlotPane15, &q1, &q2);
        SetLayoutSelfText((nw4r::lyt::Layout*)mSlotPane16, &lbl_eu_805063BC[0x2AA], 0);
        PaneMatSetTevColors(mSlotPane16, &q3, &q4);
        PaneMatSetTevColors(mSlotPane17, &q1, &q2);
        SetLayoutSelfText((nw4r::lyt::Layout*)mSlotPane18, &lbl_eu_805063BC[0x2AA], 0);
        PaneMatSetTevColors(mSlotPane18, &q3, &q4);
        PaneMatSetTevColors(mSlotPane19, &q1, &q2);
    }

    // ---- common tail: two 8-entry item lists ----
    ItemBoxInfoCopy listA[8];
    ItemBoxInfoCopy listB[8];
    setItemBoxCopy(&listA[0], 0, 0, 0);
    setItemBoxCopy(&listA[1], 0, 0, 0);
    setItemBoxCopy(&listA[2], 0, 0, 0);
    setItemBoxCopy(&listA[3], 0, 0, 0);
    setItemBoxCopy(&listA[4], 0, 0, 0);
    setItemBoxCopy(&listA[5], 0, 0, 0);
    setItemBoxCopy(&listA[6], 0, 0, 0);
    setItemBoxCopy(&listA[7], 0, 0, 0);
    setItemBoxCopy(&listB[0], 0, 0, 0);
    setItemBoxCopy(&listB[1], 0, 0, 0);
    setItemBoxCopy(&listB[2], 0, 0, 0);
    setItemBoxCopy(&listB[3], 0, 0, 0);
    setItemBoxCopy(&listB[4], 0, 0, 0);
    setItemBoxCopy(&listB[5], 0, 0, 0);
    setItemBoxCopy(&listB[6], 0, 0, 0);
    setItemBoxCopy(&listB[7], 0, 0, 0);

    u8 listIdxA = 0;
    u8 listIdxB = 0;

    // weapon slot
    {
        void* itemW = CItem_kindPtr(2, *(s16*)((u8*)charObj + 0x26));
        void* items[5];
        items[0] = CItem_kindPtr(4, *(s16*)((u8*)charObj + 0x1C));
        items[1] = CItem_kindPtr(5, *(s16*)((u8*)charObj + 0x1E));
        items[2] = CItem_kindPtr(6, *(s16*)((u8*)charObj + 0x20));
        items[3] = CItem_kindPtr(7, *(s16*)((u8*)charObj + 0x22));
        items[4] = CItem_kindPtr(8, *(s16*)((u8*)charObj + 0x24));
        if (itemW != NULL) {
            u8 cnt = (u8)((u32(*)(void*))((void**)itemW)[0x0C])(itemW);
            for (u8 i = 0; i < 3; i++) {
                ItemBoxInfoCopy tmp;
                tmp.itemId = 0;
                tmp.value = 0;
                tmp.state = 0;
                if (i < cnt) {
                    void* e = ((void*(*)(void*, u32))((void**)itemW)[0x0B])(itemW, i);
                    if (e != NULL) {
                        u32 w0 = *(u32*)e;
                        u16 w1 = *(u16*)((u8*)e + 4);
                        ItemBoxInfoCopy tmp2;
                        setItemBoxCopy(&tmp2, (s16)((w0 >> 11) & 0x7FF), (w0 >> 22) & 7, (w1 >> 16) & 0xFFF);
                        copyItemBoxCopy(&tmp, &tmp2);
                    }
                }
                copyItemBoxCopy(&listA[listIdxA++], &tmp);
                if (type != 2) {
                    copyItemBoxCopy(&listB[listIdxB++], &tmp);
                }
            }
        }
        // crystal list (type == 2 && arg5 != 0)
        if (type == 2 && arg5 != 0) {
            u8 cnt = (u8)BdatGetU8Direct((const void*)lbl_eu_806640F4, &lbl_eu_805063BC[0x432], BdatGetItemId(*(u32*)arg3 >> 20));
            for (u8 i = 0; i < 3; i++) {
                ItemBoxInfoCopy tmp;
                tmp.itemId = 0;
                tmp.value = 0;
                tmp.state = 0;
                if (i < cnt) {
                    char buf[0x20];
                    sprintf(buf, &lbl_eu_805063BC[0x43B], i + 1);
                    u32 id = BdatGetU16Direct(lbl_eu_806640F4, buf, BdatGetItemId(*(u32*)arg3 >> 20));
                    u16 nameId = (u16)BdatGetU16Direct(lbl_eu_806640EC, &lbl_eu_805063BC[0x814], id);
                    u8 icon = (u8)BdatGetU8Direct((const void*)lbl_eu_806640EC, &lbl_eu_805063BC[0x447], id);
                    u8 val = (u8)BdatGetU8Direct((const void*)lbl_eu_806640EC, &lbl_eu_805063BC[0x1F9], id);
                    if (id != 0) {
                        ItemBoxInfoCopy tmp2;
                        tmp2.itemId = nameId;
                        tmp2.value = icon;
                        tmp2.state = val;
                        copyItemBoxCopy(&tmp, &tmp2);
                    }
                }
                copyItemBoxCopy(&listB[listIdxB++], &tmp);
            }
        }
        // blank entries (type == 2 && arg5 == 0)
        if (type == 2 && arg5 == 0) {
            ItemBoxInfoCopy tmp;
            tmp.itemId = 0;
            tmp.value = 0;
            tmp.state = 0;
            copyItemBoxCopy(&listB[listIdxB++], &tmp);
        }
        // armor slots
        {
            u8 slotTypes[5] = { 4, 5, 6, 7, 8 };
            for (u8 i = 0; i < 5; i++) {
                ItemBoxInfoCopy tmp;
                tmp.itemId = 0;
                tmp.value = 0;
                tmp.state = 0;
                void* item = items[i];
                if (item != NULL) {
                    if ((u8)((u32(*)(void*))((void**)item)[0x0C])(item) != 0) {
                        void* e = ((void*(*)(void*, u32))((void**)item)[0x0B])(item, 0);
                        if (e != NULL) {
                            u32 w0 = *(u32*)e;
                            u16 w1 = *(u16*)((u8*)e + 4);
                            ItemBoxInfoCopy tmp2;
                            setItemBoxCopy(&tmp2, (s16)((w0 >> 11) & 0x7FF), (w0 >> 22) & 7, (w1 >> 16) & 0xFFF);
                            copyItemBoxCopy(&tmp, &tmp2);
                        }
                    }
                }
                copyItemBoxCopy(&listA[listIdxA++], &tmp);
                if (type != slotTypes[i]) {
                    copyItemBoxCopy(&listB[listIdxB++], &tmp);
                }
                if (type == slotTypes[i]) {
                    ItemBoxInfoCopy tmp3;
                    tmp3.itemId = 0;
                    tmp3.value = 0;
                    tmp3.state = 0;
                    if (arg5 != 0) {
                        if (BdatGetU8Direct((const void*)lbl_eu_806640F8, &lbl_eu_805063BC[0x432], BdatGetItemId(*(u32*)arg3 >> 20)) != 0) {
                            u16 nameId = (u16)BdatGetU16Direct(lbl_eu_806640F8, &lbl_eu_805063BC[0x81B], BdatGetItemId(*(u32*)arg3 >> 20));
                            u16 capId = (u16)BdatGetU16Direct(lbl_eu_806640EC, &lbl_eu_805063BC[0x814], nameId);
                            u8 icon = (u8)BdatGetU8Direct((const void*)lbl_eu_806640EC, &lbl_eu_805063BC[0x447], nameId);
                            u8 val = (u8)BdatGetU8Direct((const void*)lbl_eu_806640EC, &lbl_eu_805063BC[0x1F9], nameId);
                            if (nameId != 0) {
                                tmp3.itemId = capId;
                                tmp3.value = icon;
                                tmp3.state = val;
                            }
                        }
                    }
                    copyItemBoxCopy(&listB[listIdxB++], &tmp3);
                }
            }
        }
    }

    // ---- compact listA (dedupe by stackable flag, sum values, cap) ----
    {
        u16 ids[8];
        u32 vals[8];
        u8 flags[8];
        u8 n = 0;
        for (u8 i = 0; i < 8; i++) {
            u16 id = listA[i].itemId;
            u32 val = listA[i].value;
            u8 flag = listA[i].state;
            if (id == 0) {
                ids[n] = id;
                vals[n] = val;
                flags[n] = flag;
                n++;
            } else if (BdatGetU8Direct((const void*)lbl_eu_806640D8, &lbl_eu_805063BC[0x4FD], id) != 0) {
                u8 found = 0;
                for (u8 j = 0; j < n; j++) {
                    if (id == ids[j]) {
                        vals[j] += val;
                        for (u8 k = j + 1; k < n; k++) {
                            if (id == ids[k]) vals[k] = vals[j];
                        }
                        found = 1;
                        ids[n] = id;
                        vals[n] = vals[j];
                        flags[n] = flag;
                        n++;
                        break;
                    }
                }
                if (!found) {
                    ids[n] = id;
                    vals[n] = val;
                    flags[n] = flag;
                    n++;
                }
            }
        }
        for (u8 i = 0; i < 8; i++) {
            if (ids[i] != 0) {
                u32 cap = BdatGetU16Direct(lbl_eu_806640D8, &lbl_eu_805063BC[0x503], ids[i]);
                if (vals[i] > cap) vals[i] = cap;
            }
        }
        // same for listB
        u16 ids2[8];
        u32 vals2[8];
        u8 flags2[8];
        u8 n2 = 0;
        for (u8 i = 0; i < 8; i++) {
            u16 id = listB[i].itemId;
            u32 val = listB[i].value;
            u8 flag = listB[i].state;
            if (id == 0) {
                ids2[n2] = id;
                vals2[n2] = val;
                flags2[n2] = flag;
                n2++;
            } else if (BdatGetU8Direct((const void*)lbl_eu_806640D8, &lbl_eu_805063BC[0x4FD], id) != 0) {
                u8 found = 0;
                for (u8 j = 0; j < n2; j++) {
                    if (id == ids2[j]) {
                        vals2[j] += val;
                        for (u8 k = j + 1; k < n2; k++) {
                            if (id == ids2[k]) vals2[k] = vals2[j];
                        }
                        found = 1;
                        ids2[n2] = id;
                        vals2[n2] = vals2[j];
                        flags2[n2] = flag;
                        n2++;
                        break;
                    }
                }
                if (!found) {
                    ids2[n2] = id;
                    vals2[n2] = val;
                    flags2[n2] = flag;
                    n2++;
                }
            }
        }
        for (u8 i = 0; i < 8; i++) {
            if (ids2[i] != 0) {
                u32 cap = BdatGetU16Direct(lbl_eu_806640D8, &lbl_eu_805063BC[0x503], ids2[i]);
                if (vals2[i] > cap) vals2[i] = cap;
            }
        }

        // better/worse flags comparing listB vs listA
        u8 cmp[8];
        for (u8 i = 0; i < 8; i++) {
            if (ids2[i] != 0 && BdatGetU8Direct((const void*)lbl_eu_806640D8, &lbl_eu_805063BC[0x4FD], ids2[i]) != 0) {
                u8 done = 0;
                for (u8 j = 0; j < 8; j++) {
                    if (ids2[i] == ids[j]) {
                        if (vals2[i] > vals[j]) cmp[i] = 1;
                        else if (vals2[i] < vals[j]) cmp[i] = 2;
                        done = 1;
                        break;
                    }
                }
                if (!done) cmp[i] = 1;
            } else if (ids2[i] != 0) {
                u8 done = 0;
                for (u8 j = 0; j < 8; j++) {
                    if (ids2[i] == ids[j]) {
                        if (vals2[i] > vals[j]) cmp[i] = 1;
                        else if (vals2[i] < vals[j]) cmp[i] = 2;
                        done = 1;
                        break;
                    }
                }
                if (!done) cmp[i] = 1;
            }
        }
        // clear duplicate ids in listB
        for (u8 i = 0; i < 8; i++) {
            if (ids2[i] != 0 && BdatGetU8Direct((const void*)lbl_eu_806640D8, &lbl_eu_805063BC[0x4FD], ids2[i]) != 0) {
                for (u8 j = i + 1; j < 8; j++) {
                    if (ids2[i] == ids2[j]) ids2[j] = 0;
                }
            }
        }

        // pane name setup
        nw4r::lyt::Pane* root = mpLayout->GetRootPane()->FindPaneByName(&lbl_eu_805063BC[0x16E], true);
        for (u8 i = 0; i < 8; i++) {
            char buf1[0x20];
            char buf2[0x20];
            sprintf(buf1, &lbl_eu_805063BC[0x507], i * 2 + 0x13);
            sprintf(buf2, &lbl_eu_805063BC[0x515], i + 0x10);
            LayoutSetTextBoxFmtValue(mpLayout, buf1, &lbl_eu_805063BC[0x2AA], 0);
            LayoutSetTextBoxFmtValue(mpLayout, buf2, &lbl_eu_805063BC[0x2AA], 0);
        }

        // per-slot render
        for (u8 i = 0; i < 8; i++) {
            u16 id = ids2[i];
            u32 val = vals2[i];
            u8 flag = flags2[i];
            u8 cf = cmp[i];
            if (id == 0) continue;
            char buf1[0x20];
            char buf2[0x20];
            sprintf(buf1, &lbl_eu_805063BC[0x507], i * 2 + 0x13);
            sprintf(buf2, &lbl_eu_805063BC[0x515], i + 0x10);
            nw4r::lyt::Pane* pane1 = mpLayout->GetRootPane()->FindPaneByName(buf1, true);
            nw4r::lyt::Pane* pane2 = mpLayout->GetRootPane()->FindPaneByName(buf2, true);
            CItemBoxQuad quad;
            ScalePaneWorldPos((nw4r::math::VEC3*)&quad, pane1, root, mpLayout->GetRootPane());
            // slot index transposition: idx = (s32)(0.5f * i) + ((i & 1) ? 4 : 0)
            s32 idx = (s32)(0.5f * (f32)i) + ((i & 1) ? 4 : 0);
            if (idx < 12) {
                *(s16*)&mPage.mItemIds[idx] = id;
            }
            if (idx < 12) {
                mPage.mBytesA8[idx] = 3;
            }
            if (idx < 12) {
                mPage.mBytesB4[idx] = flag;
            }
            if (idx < 12) {
                mPage.mValues[idx] = (s16)val;
            }
            if (idx < 12) {
                f32 v0 = ((f32*)&quad)[0];
                f32 v1 = ((f32*)&quad)[1];
                f32 v2 = ((f32*)&quad)[2];
                f32 dst[3] = {v0, v1, v2};
                copyVEC3(&mPage.mPos[idx], (void*)dst);
            }
            LayoutSetTextBoxFmtValue(mpLayout, buf1, BdatGetPtrDirect(lbl_eu_806640D8, &lbl_eu_805063BC[0x139], id), 0);
            if (val == 0) {
                LayoutSetTextBoxFmtValue(mpLayout, buf2, &lbl_eu_805063BC[0x2AA], 0);
            } else {
                u8 rvs = (u8)BdatGetU8Direct((const void*)lbl_eu_806640D8, &lbl_eu_805063BC[0x3], id);
                if (rvs != 0 && (rvs == 0xFF || rvs == 0xFE)) {
                    ml::FixStr<32> fmtBuf;
                    fmtBuf.format(&lbl_eu_805063BC[0x13E], (s32)val, BdatTouchStringCell(&lbl_eu_805063BC[0x130], &lbl_eu_805063BC[0x139], 0x21));
                    LayoutSetTextBoxFmtValue(mpLayout, buf2, fmtBuf.c_str(), 0);
                } else {
                    ml::FixStr<32> fmtBuf;
                    fmtBuf.format(&lbl_eu_805063BC[0x422], (s32)val);
                    LayoutSetTextBoxFmtValue(mpLayout, buf2, fmtBuf.c_str(), 0);
                }
            }
            if (arg5 == 0) {
                LayoutSetTextBoxFmtValue(mpLayout, buf2, &lbl_eu_805063BC[0x2AA], 0);
            }
            E43Quad q1 = *(E43Quad*)&lbl_eu_80664518;
            E43Quad q2 = *(E43Quad*)&lbl_eu_80664520;
            E43Quad q3 = *(E43Quad*)&lbl_eu_80664558;
            E43Quad q4 = *(E43Quad*)&lbl_eu_80664560;
            u32 cap = BdatGetU16Direct(lbl_eu_806640D8, &lbl_eu_805063BC[0x503], id);
            if ((u32)val >= cap) {
                __as__11_GXColorS10FRC11_GXColorS10(&q1, &lbl_eu_80664548);
                __as__11_GXColorS10FRC11_GXColorS10(&q2, &lbl_eu_80664550);
                __as__11_GXColorS10FRC11_GXColorS10(&q3, &lbl_eu_80664588);
                __as__11_GXColorS10FRC11_GXColorS10(&q4, &lbl_eu_80664590);
            } else if (cf == 1) {
                __as__11_GXColorS10FRC11_GXColorS10(&q1, &lbl_eu_80664528);
                __as__11_GXColorS10FRC11_GXColorS10(&q2, &lbl_eu_80664530);
                __as__11_GXColorS10FRC11_GXColorS10(&q3, &lbl_eu_80664568);
                __as__11_GXColorS10FRC11_GXColorS10(&q4, &lbl_eu_80664570);
            } else if (cf == 2) {
                __as__11_GXColorS10FRC11_GXColorS10(&q1, &lbl_eu_80664538);
                __as__11_GXColorS10FRC11_GXColorS10(&q2, &lbl_eu_80664540);
                __as__11_GXColorS10FRC11_GXColorS10(&q3, &lbl_eu_80664578);
                __as__11_GXColorS10FRC11_GXColorS10(&q4, &lbl_eu_80664580);
            }
            if (arg5 == 0) {
                __as__11_GXColorS10FRC11_GXColorS10(&q1, &lbl_eu_806645A8);
                __as__11_GXColorS10FRC11_GXColorS10(&q2, &lbl_eu_806645B0);
                __as__11_GXColorS10FRC11_GXColorS10(&q3, &lbl_eu_806645C8);
                __as__11_GXColorS10FRC11_GXColorS10(&q4, &lbl_eu_806645D0);
            }
            PaneMatSetTevColors(pane1, &q1, &q2);
            PaneMatSetTevColors(pane2, &q3, &q4);
        }
    }
}




#pragma push
#pragma auto_inline off
s32 CItemBoxInfo2::CalcBonus1(u16 b, s32 arg2, void* d) {
    // Single-expression sum; builtin s32->f32 cast emits MWCC's
    // 0x4330/xoris double-trick.
    // b is the u16 character id: the u16 param fixes the param-save order.
    s32 prod = arg2 * (s32)(GetEffect(b, 0x52, d) + GetCondStat2D((void*)(u32)b) + 0x64);
    return (s32)(0.01f * (f32)prod);
}
#pragma pop

#pragma push
#pragma auto_inline off
s32 CItemBoxInfo2::CalcBonus2(u16 b, s32 arg2, void* d) {
    // Standalone SDA (not sdata2_ItemBox members) so lfd/lfs use r0@sda21
    // instead of li r3,0 / lfs 56(r3); u16 b gives retail or r28,r4 first.
    s32 prod = (s32)(GetEffect(b, 0x53, d) + GetCondStat2D((void*)(u32)b) + 0x64) * arg2;
    return (s32)(0.01f * (f32)prod);
}
#pragma pop
#pragma push
#pragma auto_inline off
// Retail getItemBox2Effect: the ItemBox2 variant of getItemBoxEffect. Six slots are
// scanned via the lookup table (s16 ids at +0x26/+0x1C..0x24, 6-byte slot
// table from .sdata2). When the slot's byte equals the candidate's equip id
// ((u8)BdatGetItemType((u32)arg3)), a name-based lookup runs instead of the item
// walk: count = BdatGetU8Direct(v1, base+0x432, v2), then for each index the
// name v = BdatGetU16Direct(v1, buf, v2) resolves a category (BdatGetItemId)
// matching the requested item id, and the name-count byte is added. The
// id==-1 slot only runs the name lookup. Result is clamped by the table cap.
u32 CItemBoxInfo2::GetEffect(u32 b, u32 c, void* d) {
    void* lookup = CtrlObjectParam_GetCharEntry(b);
    u32 nameObj = (u32)((char* (*)(void*))LookupItemTypeTable)(d);
    // u16 local: MWCC saves the raw callee return and re-extracts per
    // argument site (see testItemBox2Equip note).
    u16 nameId = BdatGetItemId((u32)d);
    u8 equipSlot = (u8)BdatGetItemType((u32)d);
    char* base = lbl_eu_805063BC;
    s16 slotIds[6];
    slotIds[0] = *(s16*)((u8*)lookup + 0x26);
    slotIds[1] = *(s16*)((u8*)lookup + 0x1C);
    slotIds[2] = *(s16*)((u8*)lookup + 0x1E);
    slotIds[3] = *(s16*)((u8*)lookup + 0x20);
    slotIds[4] = *(s16*)((u8*)lookup + 0x22);
    slotIds[5] = *(s16*)((u8*)lookup + 0x24);
    u8 bytes[6] = { 2, 4, 5, 6, 7, 8 };
    char buf[0x20];
    u32 result = 0;
    for (u32 slot = 0; slot < 6; slot++) {
        s16 id = slotIds[(u8)slot];
        if (id != -1) {
            u8 slotByte = bytes[(u8)slot];
            void* item = CItem_kindPtr(slotByte, id);
            if (d != NULL && slotByte == equipSlot) {
                // candidate slot: name-based lookup over the item names
                u8 count = (u8)BdatGetU8Direct((const void*)nameObj, base + 0x432, (u16)nameId);
                for (u8 j = 0; (u8)j < count; j++) {
                    sprintf(buf, base + 0x43b, (u8)j + 1);
                    u32 v = BdatGetU16Direct((char*)nameObj, buf, nameId);
                    if ((u16)v != 0) {
                        u16 cat = (u16)BdatGetItemId(v);
                        if (c == cat) {
                            result += (u8)BdatGetU8Direct((const void*)lbl_eu_806640EC, base + 0x447, (u16)v);
                        }
                    }
                }
            } else if (item != NULL && *(u32*)item != 0) {
                CItemImpl* inst = CItem_initItemImplInstances((CItemData*)item);
                u8 count = inst->vf30((CItemData*)item);
                for (u8 j = 0; (u8)j < count; j++) {
                    CItemImpl* inst2 = CItem_initItemImplInstances((CItemData*)item);
                    s16 v = inst2->vf40((CItemData*)item, (u8)j);
                    if (v != -1) {
                        void* r = CItem_kindPtr(3, v);
                        if (r != NULL && *(u32*)r != 0) {
                            CItemImpl* inst3 = CItem_initItemImplInstances((CItemData*)r);
                            u16 cat = (u16)inst3->vf54((CItemData*)r);
                            if (c == cat) {
                                CItemImpl* inst4 = CItem_initItemImplInstances((CItemData*)r);
                                result += inst4->vf90((CItemData*)r);
                            }
                        }
                    } else {
                        CItemImpl* inst5 = CItem_initItemImplInstances((CItemData*)item);
                        CItemBoxSubRecord* sub = (CItemBoxSubRecord*)inst5->vf2C((CItemData*)item, (u8)j);
                        if (sub != NULL) {
                            u16 cat2 = (sub->field_04 >> 4) & 0xFFF;
                            if (c == cat2) {
                                // retail sign-extends the 11-bit field (rlwinm+extsh)
                                s32 val2 = (sub->_00 >> 10) & 0x7FF;
                                result += (s16)val2;
                            }
                        }
                    }
                }
            }
        } else {
            if (d != NULL && bytes[(u8)slot] == equipSlot) {
                // candidate slot: name-based lookup
                u8 count = (u8)BdatGetU8Direct((const void*)nameObj, base + 0x432, (u16)nameId);
                for (u8 j = 0; (u8)j < count; j++) {
                    sprintf(buf, base + 0x43b, (u8)j + 1);
                    u32 v = BdatGetU16Direct((char*)nameObj, buf, nameId);
                    if ((u16)v != 0) {
                        u16 cat = (u16)BdatGetItemId(v);
                        if (c == cat) {
                            result += (u8)BdatGetU8Direct((const void*)lbl_eu_806640EC, base + 0x447, (u16)v);
                        }
                    }
                }
            }
        }
    }
    u16 cap = (u16)BdatGetU16Direct(lbl_eu_806640D8, (char*)&lbl_eu_805063BC[0x503], c);
    if ((s16)cap < (s32)result) result = cap;
    return result;
}
#pragma pop
#pragma push
#pragma auto_inline off
#pragma optimize_for_size on
__declspec(noinline) u32 CItemBoxInfo2::TestBdatFlag(u32 arg1, u32 arg2) {
    if (arg1 == 0 || arg2 == 0) return 0;
    char buf[0x20];
    sprintf(buf, &lbl_eu_805063BC[0x1f4], arg1);
    u32 g = (u32)lbl_eu_806640F8;
    return (u8)BdatGetU8Direct((const void*)g, (const char*)buf, (u16)BdatGetItemId(arg2)) != 0;
}
#pragma pop
#pragma push
#pragma auto_inline off
u32 CItemBoxInfo2::SumTypeCounts(u16 arg2, void* arg3) {
    if (arg2 == 0) return 0;
    void* g = lbl_eu_806640F8;
    u16 v1 = arg3 ? (u16)BdatGetItemType((u32)arg3) : (u16)0;
    u16 v2 = arg3 ? (u16)BdatGetItemId((u32)arg3) : (u16)0;
    void* lookup = CtrlObjectParam_GetCharEntry(arg2);
    u32 result = 0;
    for (u32 i = 4; i <= 8; i++) {
        u8 ii = (u8)i;
        if (ii == v1) continue;
        s16 val = -1;
        switch (ii) {
            case 4: val = ((CItemBoxCharEntryView*)lookup)->slotIds[0]; break;
            case 5: val = ((CItemBoxCharEntryView*)lookup)->slotIds[1]; break;
            case 6: val = ((CItemBoxCharEntryView*)lookup)->slotIds[2]; break;
            case 7: val = ((CItemBoxCharEntryView*)lookup)->slotIds[3]; break;
            case 8: val = ((CItemBoxCharEntryView*)lookup)->slotIds[4]; break;
        }
        if (val == -1) continue;
        // Retail passes the slot id (u8)i and the slot value val (2-arg form).
        void* r = CItem_kindPtr((u8)i, val);
        if (r == NULL) continue;
        u32 v = ((CItemData*)r)->field_00;
        if (v == 0) continue;
        result += (u8)BdatGetU8Direct((const void*)g, (char*)&lbl_eu_805063BC[0x1e2], BdatGetItemId(v >> 20));
    }
    if (arg3 != NULL) {
        result += (u8)BdatGetU8Direct((const void*)g, (char*)&lbl_eu_805063BC[0x1e2], v2);
    }
    return result;
}
#pragma pop
#pragma push
#pragma auto_inline off
__declspec(noinline) bool CItemBoxInfo2::TestEquip(u16 b, void* c) {
    // Validates every equip slot against candidate item c before an exchange:
    // for each of the six slots either the indexed item names must still
    // resolve (when the slot holds the equipped item) or no indexed entry may
    // be occupied/in-use.
    CItemBoxCharEntryView* lookup = (CItemBoxCharEntryView*)CtrlObjectParam_GetCharEntry(b);
    u32 nameObj = (u32)((char* (*)(void*))LookupItemTypeTable)(c);
    // u16 local: MWCC saves the raw callee return (or-copy) and re-extracts
    // (u16) per argument site, matching retail's def-then-per-site rlwinm.
    u16 nameId = BdatGetItemId((u32)c);
    u8 slotId = (u8)BdatGetItemType((u32)c);
    // 6-byte slot table from two .sdata2 constants (different pair from
    // checkItemBoxGemChain).
    s16 ids[6];
    // ids[0] is the halfword at lookup+0x26; the remaining five are the
    // packed block at 0x1C..0x24 (retail load order).
    ids[0] = lookup->slotIds[5];
    ids[1] = lookup->slotIds[0];
    ids[2] = lookup->slotIds[1];
    ids[3] = lookup->slotIds[2];
    ids[4] = lookup->slotIds[3];
    ids[5] = lookup->slotIds[4];
    u8 tbl[6] = { 2, 4, 5, 6, 7, 8 };
    char buf[0x20];
    for (u8 i = 0; i < 6; i++) {
        if (ids[i] != -1) {
            u8 slot = tbl[i];
            void* item = CItem_kindPtr(slot, ids[i]);
            if (c != NULL && slot == slotId) {
                // candidate matches the equipped slot: verify every indexed
                // item name still resolves.
                u8 j; u8 count = (u8)BdatGetU8Direct((const void*)nameObj, &lbl_eu_805063BC[0x432], nameId);
                for (j = 0; j < count; j++) {
                    sprintf(buf, &lbl_eu_805063BC[0x43b], j + 1);
                    if ((u16)BdatGetU16Direct((void*)nameObj, buf, nameId) != 0) return false;
                }
            } else if (item != NULL && *(u32*)item != 0) {
                CItemImpl* inst = CItem_initItemImplInstances((CItemData*)item);
                u8 count = inst->vf30((CItemData*)item);
                for (u8 j = 0; j < count; j++) {
                    CItemImpl* inst2 = CItem_initItemImplInstances((CItemData*)item);
                    s16 v = inst2->vf40((CItemData*)item, j);
                    if (v != -1) {
                        void* r = CItem_kindPtr(3, v);
                        if (r != NULL && *(u32*)r != 0) return false;
                    } else {
                        CItemImpl* inst3 = CItem_initItemImplInstances((CItemData*)item);
                        CItemBoxSubRecord* sub = (CItemBoxSubRecord*)inst3->vf2C((CItemData*)item, j);
                        if (sub != NULL && ((sub->field_04 >> 4) & 0xFFF) != 0) return false;
                    }
                }
            }
        } else {
            if (c != NULL && tbl[i] == slotId) {
                u8 j; u8 count = (u8)BdatGetU8Direct((const void*)nameObj, &lbl_eu_805063BC[0x432], nameId);
                for (j = 0; j < count; j++) {
                    sprintf(buf, &lbl_eu_805063BC[0x43b], j + 1);
                    if ((u16)BdatGetU16Direct((void*)nameObj, buf, nameId) != 0) return false;
                }
            }
        }
    }
    return true;
}
#pragma pop

// Retail CItemBoxInfo2::OnFileEvent: two file-load branches. The line-arc
// branch (fileHandle1) builds the whole layout in a 0x18000 scratch region;
// the common-arc branch (fileHandle2) only re-attaches its accessor into a
// 0x100 scratch region and refreshes via activateItemBox2.
#pragma push
#pragma optimize_for_size on
bool CItemBoxInfo2::OnFileEvent(CEventFile* file) {
    if (mFileHandle1 == file->mFileHandle) {
        mRegion1.createRegion(mtl::MemManager::getHandleMEM2(), 0x18000,
                                      &lbl_eu_805063BC[0x826], 1);
        Class_8045F858 host(&mRegion1);
        u8* data = (u8*)mFileHandle1->getData();
        mtl::MemManager::setMemInitFlag(false);
        nw4r::lyt::ArcResourceAccessor* acc = createArcResourceAccessor__10CLibLayoutFv();
        mArcResAcc = acc;
        acc->Attach(data, &lbl_eu_805063BC[0x531]);
        buildLayout(&mpLayout, acc, &lbl_eu_805063BC[0x535]);
        bindLayoutAnimTransform(mpLayout, &mAnimIn, acc, &lbl_eu_805063BC[0x54a]);
        bindLayoutAnimTransform(mpLayout, &mAnimInfoIn, acc, &lbl_eu_805063BC[0x562]);
        nw4r::lyt::Pane* root = mpLayout->GetRootPane();
        void* fontObj = getFontInfo__11CDeviceFontFUlPQ34nw4r3lyt6Layout(1, mpLayout);
        ApplyFontToTxtPanes(root, (const nw4r::ut::Font*)(static_cast<IDeviceFontInfo*>(fontObj)->getFont()));

        // Seed the label textboxes with the shared text object.
        char* text = CUICfManager_getPackedFont9C();
        setLayoutTextBoxFont(mpLayout, &lbl_eu_805063BC[0x25b], (u32)text);
        setLayoutTextBoxFont(mpLayout, &lbl_eu_805063BC[0x267], (u32)text);
        setLayoutTextBoxFont(mpLayout, &lbl_eu_805063BC[0x273], (u32)text);
        setLayoutTextBoxFont(mpLayout, &lbl_eu_805063BC[0x286], (u32)text);
        setLayoutTextBoxFont(mpLayout, &lbl_eu_805063BC[0x292], (u32)text);
        setLayoutTextBoxFont(mpLayout, &lbl_eu_805063BC[0x29e], (u32)text);
        setLayoutTextBoxFont(mpLayout, &lbl_eu_805063BC[0x45a], (u32)text);
        setLayoutTextBoxFont(mpLayout, &lbl_eu_805063BC[0x466], (u32)text);
        setLayoutTextBoxFont(mpLayout, &lbl_eu_805063BC[0x4a7], (u32)text);
        setLayoutTextBoxFont(mpLayout, &lbl_eu_805063BC[0x57f], (u32)text);
        setLayoutTextBoxFont(mpLayout, &lbl_eu_805063BC[0x58b], (u32)text);
        setLayoutTextBoxFont(mpLayout, &lbl_eu_805063BC[0x597], (u32)text);
        setLayoutTextBoxFont(mpLayout, &lbl_eu_805063BC[0x5a3], (u32)text);
        setLayoutTextBoxFont(mpLayout, &lbl_eu_805063BC[0x5af], (u32)text);
        setLayoutTextBoxFont(mpLayout, &lbl_eu_805063BC[0x5be], (u32)text);
        setLayoutTextBoxFont(mpLayout, &lbl_eu_805063BC[0x5cd], (u32)text);
        setLayoutTextBoxFont(mpLayout, &lbl_eu_805063BC[0x143], (u32)text);
        setLayoutTextBoxFont(mpLayout, &lbl_eu_805063BC[0x4e3], (u32)text);
        setLayoutTextBoxFont(mpLayout, &lbl_eu_805063BC[0x4f0], (u32)text);
        setLayoutTextBoxFont(mpLayout, &lbl_eu_805063BC[0x5dc], (u32)text);
        setLayoutTextBoxFont(mpLayout, &lbl_eu_805063BC[0x5e9], (u32)text);
        setLayoutTextBoxFont(mpLayout, &lbl_eu_805063BC[0x5f6], (u32)text);
        setLayoutTextBoxFont(mpLayout, &lbl_eu_805063BC[0x603], (u32)text);
        setLayoutTextBoxFont(mpLayout, &lbl_eu_805063BC[0x610], (u32)text);
        setLayoutTextBoxFont(mpLayout, &lbl_eu_805063BC[0x61d], (u32)text);
        setLayoutTextBoxFont(mpLayout, &lbl_eu_805063BC[0x62a], (u32)text);
        setLayoutTextBoxFont(mpLayout, &lbl_eu_805063BC[0x637], (u32)text);
        setLayoutTextBoxFont(mpLayout, &lbl_eu_805063BC[0x644], (u32)text);
        setLayoutTextBoxFont(mpLayout, &lbl_eu_805063BC[0x651], (u32)text);
        setLayoutTextBoxFont(mpLayout, &lbl_eu_805063BC[0x65e], (u32)text);
        setLayoutTextBoxFont(mpLayout, &lbl_eu_805063BC[0x66b], (u32)text);
        setLayoutTextBoxFont(mpLayout, &lbl_eu_805063BC[0x678], (u32)text);
        setLayoutTextBoxFont(mpLayout, &lbl_eu_805063BC[0x685], (u32)text);
        setLayoutTextBoxFont(mpLayout, &lbl_eu_805063BC[0x692], (u32)text);
        setLayoutTextBoxFont(mpLayout, &lbl_eu_805063BC[0x69f], (u32)text);
        setLayoutTextBoxFont(mpLayout, &lbl_eu_805063BC[0x6ac], (u32)text);
        setLayoutTextBoxFont(mpLayout, &lbl_eu_805063BC[0x6b9], (u32)text);
        setLayoutTextBoxFont(mpLayout, &lbl_eu_805063BC[0x6c6], (u32)text);

        mpLayout->SetAnimationEnable(mAnimInfoIn, false);
        mpLayout->SetAnimationEnable(mAnimIn, true);
        mpLayout->Animate(0);

        char* base = lbl_eu_805063BC;
        u8 mode = mKind;

        // Mode-gated pane clears (retail: mode!=1&&mode!=2 / mode!=3).
        if (mode != 1 && mode != 2) {
            setPaneVisible(root->FindPaneByName(&lbl_eu_805063BC[0x6dd], true), 0);
        }
        if (mode != 3) {
            setPaneVisible(root->FindPaneByName(&lbl_eu_805063BC[0x6e6], true), 0);
        }

        // Fixed label texts: each pane gets string-table entry [len].
        #define SEED_ITEMBOX2_LABEL(off, len) \
            LayoutSetTextBoxFmtValue((nw4r::lyt::Layout*)mpLayout, &lbl_eu_805063BC[off], \
                          BdatTouchStringCell(&lbl_eu_805063BC[0x130], &lbl_eu_805063BC[0x139], len), 0)
        SEED_ITEMBOX2_LABEL(0x2b6, 10);
        SEED_ITEMBOX2_LABEL(0x2d7, 13);
        SEED_ITEMBOX2_LABEL(0x2e2, 17);
        SEED_ITEMBOX2_LABEL(0x2c1, 18);
        SEED_ITEMBOX2_LABEL(0x2cc, 12);
        SEED_ITEMBOX2_LABEL(0x2ed, 19);
        SEED_ITEMBOX2_LABEL(0x6f2, 24);
        SEED_ITEMBOX2_LABEL(0x6fd, 24);
        SEED_ITEMBOX2_LABEL(0x708, 36);
        SEED_ITEMBOX2_LABEL(0x713, 37);
        SEED_ITEMBOX2_LABEL(0x71e, 112);
        SEED_ITEMBOX2_LABEL(0x72a, 114);
        SEED_ITEMBOX2_LABEL(0x736, 115);
        SEED_ITEMBOX2_LABEL(0x742, 116);
        SEED_ITEMBOX2_LABEL(0x74e, 121);
        SEED_ITEMBOX2_LABEL(0x75a, 117);
        SEED_ITEMBOX2_LABEL(0x766, 123);
        SEED_ITEMBOX2_LABEL(0x772, 118);
        SEED_ITEMBOX2_LABEL(0x77e, 119);
        SEED_ITEMBOX2_LABEL(0x78a, 126);
        SEED_ITEMBOX2_LABEL(0x796, 130);
        SEED_ITEMBOX2_LABEL(0x2f8, 47);
        SEED_ITEMBOX2_LABEL(0x6c6, 130);
        #undef SEED_ITEMBOX2_LABEL

        // Per-mode caption / detail text.
        if (mode == 1 || mode == 2) {
            ml::FixStr<32> caption(true);
            caption.format(&lbl_eu_805063BC[0x13e], CItemBlock_getPtr20E8(),
                           BdatTouchStringCell(&lbl_eu_805063BC[0x130], &lbl_eu_805063BC[0x139], 3));
            LayoutSetTextBoxFmtValue((nw4r::lyt::Layout*)mpLayout, &lbl_eu_805063BC[0x143],
                          caption.c_str(), 0);
        } else if (mode == 3) {
            u16 itemId = mMode;
            u8 kind = (u8)MapSexFlagGroup(itemId);
            char* s;
            if (kind == 0) {
                s = BdatTouchStringCell(&lbl_eu_805063BC[0x7a2], &lbl_eu_805063BC[0x139], 0x24);
            } else {
                s = BdatTouchStringCell(&lbl_eu_805063BC[0x7a2], &lbl_eu_805063BC[0x139], kind + 0x1F);
            }
            LayoutSetTextBoxFmtValue((nw4r::lyt::Layout*)mpLayout, &lbl_eu_805063BC[0x7ad], s, 0);
            char* name = ((char* (*)(void*, const char*, u16))&BdatGetPtrDirect)(
                &lbl_eu_80664098, &lbl_eu_805063BC[0x139], itemId);
            LayoutSetTextBoxFmtValue((nw4r::lyt::Layout*)mpLayout, &lbl_eu_805063BC[0x7b9],
                          GetBdatNameByNumId(name), 0);
            LayoutSetTextBoxFmtValue((nw4r::lyt::Layout*)mpLayout, &lbl_eu_805063BC[0x7c5],
                          BdatTouchStringCell(&lbl_eu_805063BC[0x130], &lbl_eu_805063BC[0x139], 0x2d), 0);
            LayoutSetTextBoxFmtValue((nw4r::lyt::Layout*)mpLayout, &lbl_eu_805063BC[0x7d2],
                          &lbl_eu_805063BC[0x2aa], 0);
            LayoutSetTextBoxFmtValue((nw4r::lyt::Layout*)mpLayout, &lbl_eu_805063BC[0x7e0],
                          &lbl_eu_805063BC[0x2aa], 0);
        }

        // Vertex colours: highlight pair onto this (+0x9C/+0xA4); the named
        // panes' pairs go into the .sbss colour globals with their alpha
        // halves propagated to the paired globals.
        nw4r::lyt::Pane* hl = root->FindPaneByName(&lbl_eu_805063BC[0x7ee], true);
        CItemBoxGXColor c1 = PaneMatGetS16Quad(hl, 0);
        __as__11_GXColorS10FRC11_GXColorS10((u8*)this + 0x9C, &c1);
        CItemBoxGXColor c2 = PaneMatGetS16Quad(hl, 1);
        __as__11_GXColorS10FRC11_GXColorS10((u8*)this + 0xA4, &c2);

        nw4r::lyt::Pane* p71e = root->FindPaneByName(&lbl_eu_805063BC[0x71e], true);
        CItemBoxGXColor c3 = PaneMatGetS16Quad(p71e, 0);
        __as__11_GXColorS10FRC11_GXColorS10(&lbl_eu_80664518, &c3);
        CItemBoxGXColor c4 = PaneMatGetS16Quad(p71e, 1);
        __as__11_GXColorS10FRC11_GXColorS10(&lbl_eu_80664520, &c4);
        {
            s16 a0 = *(s16*)((char*)&lbl_eu_80664518 + 6);
            s16 a1 = *(s16*)((char*)&lbl_eu_80664520 + 6);
            *(s16*)((char*)&lbl_eu_80664528 + 6) = a0;
            *(s16*)((char*)&lbl_eu_80664530 + 6) = a1;
            *(s16*)((char*)&lbl_eu_80664538 + 6) = a0;
            *(s16*)((char*)&lbl_eu_80664540 + 6) = a1;
            *(s16*)((char*)&lbl_eu_80664548 + 6) = a0;
            *(s16*)((char*)&lbl_eu_80664550 + 6) = a1;
        }

        nw4r::lyt::Pane* p4e3 = root->FindPaneByName(&lbl_eu_805063BC[0x4e3], true);
        CItemBoxGXColor c5 = PaneMatGetS16Quad(p4e3, 0);
        __as__11_GXColorS10FRC11_GXColorS10(&lbl_eu_80664558, &c5);
        CItemBoxGXColor c6 = PaneMatGetS16Quad(p4e3, 1);
        __as__11_GXColorS10FRC11_GXColorS10(&lbl_eu_80664560, &c6);
        {
            s16 a0 = *(s16*)((char*)&lbl_eu_80664558 + 6);
            s16 a1 = *(s16*)((char*)&lbl_eu_80664560 + 6);
            *(s16*)((char*)&lbl_eu_80664568 + 6) = a0;
            *(s16*)((char*)&lbl_eu_80664570 + 6) = a1;
            *(s16*)((char*)&lbl_eu_80664578 + 6) = a0;
            *(s16*)((char*)&lbl_eu_80664580 + 6) = a1;
            *(s16*)((char*)&lbl_eu_80664588 + 6) = a0;
            *(s16*)((char*)&lbl_eu_80664590 + 6) = a1;
        }

        nw4r::lyt::Pane* p2b6 = root->FindPaneByName(&lbl_eu_805063BC[0x2b6], true);
        CItemBoxGXColor c7 = PaneMatGetS16Quad(p2b6, 0);
        __as__11_GXColorS10FRC11_GXColorS10(&lbl_eu_80664598, &c7);
        CItemBoxGXColor c8 = PaneMatGetS16Quad(p2b6, 1);
        __as__11_GXColorS10FRC11_GXColorS10(&lbl_eu_806645A0, &c8);
        *(s16*)((char*)&lbl_eu_806645A8 + 6) = *(s16*)((char*)&lbl_eu_80664598 + 6);
        *(s16*)((char*)&lbl_eu_806645B0 + 6) = *(s16*)((char*)&lbl_eu_806645A0 + 6);

        nw4r::lyt::Pane* p25b = root->FindPaneByName(&lbl_eu_805063BC[0x25b], true);
        CItemBoxGXColor c9 = PaneMatGetS16Quad(p25b, 0);
        __as__11_GXColorS10FRC11_GXColorS10(&lbl_eu_806645B8, &c9);
        CItemBoxGXColor c10 = PaneMatGetS16Quad(p25b, 1);
        __as__11_GXColorS10FRC11_GXColorS10(&lbl_eu_806645C0, &c10);
        *(s16*)((char*)&lbl_eu_806645C8 + 6) = *(s16*)((char*)&lbl_eu_806645B8 + 6);
        *(s16*)((char*)&lbl_eu_806645D0 + 6) = *(s16*)((char*)&lbl_eu_806645C0 + 6);

        StampSlotTex();

        // Pane clears for the selection view.
        setPaneVisible(root->FindPaneByName(&lbl_eu_805063BC[0xc2], true), 0);
        setPaneVisible(root->FindPaneByName(&lbl_eu_805063BC[0xce], true), 0);
        setPaneVisible(root->FindPaneByName(&lbl_eu_805063BC[0xda], true), 0);
        setPaneVisible(root->FindPaneByName(&lbl_eu_805063BC[0xe6], true), 0);
        setPaneVisible(root->FindPaneByName(&lbl_eu_805063BC[0xf2], true), 0);
        setPaneVisible(root->FindPaneByName(&lbl_eu_805063BC[0xfe], true), 1);
        setPaneVisible(root->FindPaneByName(&lbl_eu_805063BC[0x10b], true), 0);
        setPaneVisible(root->FindPaneByName(&lbl_eu_805063BC[0x113], true), 0);
        setPaneVisible(root->FindPaneByName(&lbl_eu_805063BC[0x121], true), 1);
        setPaneVisible(root->FindPaneByName(&lbl_eu_805063BC[0x193], true), 0);

        // Cache the 20 slot panes at +0x40..+0x8C for the renderer (retail
        // keeps the layout's root pane live in r28 across all lookups).
        mSlotPane0 = root->FindPaneByName(base + 0x5dc, true);
        mSlotPane1 = root->FindPaneByName(base + 0x736, true);
        mSlotPane2 = root->FindPaneByName(base + 0x5e9, true);
        mSlotPane3 = root->FindPaneByName(base + 0x6c6, true);
        mSlotPane4 = root->FindPaneByName(base + 0x5f6, true);
        mSlotPane5 = root->FindPaneByName(base + 0x742, true);
        mSlotPane6 = root->FindPaneByName(base + 0x603, true);
        mSlotPane7 = root->FindPaneByName(base + 0x75a, true);
        mSlotPane8 = root->FindPaneByName(base + 0x610, true);
        mSlotPane9 = root->FindPaneByName(base + 0x772, true);
        mSlotPane10 = root->FindPaneByName(base + 0x61d, true);
        mSlotPane11 = root->FindPaneByName(base + 0x74e, true);
        mSlotPane12 = root->FindPaneByName(base + 0x637, true);
        mSlotPane13 = root->FindPaneByName(base + 0x77e, true);
        mSlotPane14 = root->FindPaneByName(base + 0x644, true);
        mSlotPane15 = root->FindPaneByName(base + 0x78a, true);
        mSlotPane16 = root->FindPaneByName(base + 0x62a, true);
        mSlotPane17 = root->FindPaneByName(base + 0x766, true);
        mSlotPane18 = root->FindPaneByName(base + 0x651, true);
        mSlotPane19 = root->FindPaneByName(base + 0x796, true);

        Activate();
        mFileHandle1 = 0;
        mRegion1.validateHeap();
        return true;
    }
    if (mFileHandle2 == file->mFileHandle) {
        mRegion2.createRegion(mtl::MemManager::getHandleMEM2(), 0x100,
                                      &lbl_eu_805063BC[0x834], 1);
        Class_8045F858 host(&mRegion2);
        u8* data = (u8*)mFileHandle2->getData();
        mtl::MemManager::setMemInitFlag(false);
        nw4r::lyt::ArcResourceAccessor* acc2 = createArcResourceAccessor__10CLibLayoutFv();
        mTexArcResAcc = acc2;
        acc2->Attach(data, &lbl_eu_805063BC[0x531]);
        Activate();
        mFileHandle2 = 0;
        mRegion2.validateHeap();
        return true;
    }
    return false;
}
#pragma pop


void sinit_801EABC4() {
    SplitU32ToS16s(&lbl_eu_80664518, 0);
    SplitU32ToS16s(&lbl_eu_80664520, 0);
    setGXColorS10(&lbl_eu_80664528, 0x25, 0x8a, 0xce, 0);
    setGXColorS10(&lbl_eu_80664530, 0x25, 0x8a, 0xce, 0);
    setGXColorS10(&lbl_eu_80664538, 0xd2, 0x28, 0x14, 0);
    setGXColorS10(&lbl_eu_80664540, 0xd2, 0x28, 0x14, 0);
    setGXColorS10(&lbl_eu_80664548, 0x1f, 0xb0, 0x1a, 0);
    setGXColorS10(&lbl_eu_80664550, 0x1f, 0xb0, 0x1a, 0);
    SplitU32ToS16s(&lbl_eu_80664558, 0);
    SplitU32ToS16s(&lbl_eu_80664560, 0);
    setGXColorS10(&lbl_eu_80664568, 0xff, 0xff, 0xfa, 0);
    setGXColorS10(&lbl_eu_80664570, 0x25, 0x8a, 0xce, 0);
    setGXColorS10(&lbl_eu_80664578, 0xff, 0xff, 0xfa, 0);
    setGXColorS10(&lbl_eu_80664580, 0xd2, 0x28, 0x14, 0);
    setGXColorS10(&lbl_eu_80664588, 0xff, 0xff, 0xfa, 0);
    setGXColorS10(&lbl_eu_80664590, 0x1f, 0xa6, 0x1a, 0);
    SplitU32ToS16s(&lbl_eu_80664598, 0);
    SplitU32ToS16s(&lbl_eu_806645A0, 0);
    setGXColorS10(&lbl_eu_806645A8, 0x80, 0x80, 0x80, 0);
    setGXColorS10(&lbl_eu_806645B0, 0x80, 0x80, 0x80, 0);
    SplitU32ToS16s(&lbl_eu_806645B8, 0);
    SplitU32ToS16s(&lbl_eu_806645C0, 0);
    setGXColorS10(&lbl_eu_806645C8, 0xff, 0xff, 0xfa, 0);
    setGXColorS10(&lbl_eu_806645D0, 0x80, 0x80, 0x80, 0);
}

// Retail formatItemBoxName: fills the item-name buffer at item_data+0xD9 from
// the item table, expanding the $1/$2 format tags in place: $1 is replaced
// by the item name (format select from the BdatGetU8Direct byte value), $2 by
// the category entry text. Both insertions re-splice the tail via the
// three-sprintf sequence (tail backup, insert, tail restore). Retail uses
// the stmw frame and keeps the mtctr table-copy loop (size-optimized).
#pragma push
#pragma optimize_for_size on
char* CItemBoxPage::GetItemName(u8 index) {
    if (index >= 12) return NULL;
    u16 item_id = mItemIds[index];
    if (item_id == 0) return NULL;
    u8 category = mBytesB4[index];
    s16 val = mValues[index];
    char tailBuf[0x40];
    char tmp[0x40];
    char buf[0x40];
    char* base = lbl_eu_805063BC;
    sprintf(buf, base, val);
    u8 fmt = (u8)BdatGetU8Direct((const void*)lbl_eu_806640D8, base + 3, item_id);
    char* name = ((char*(*)(void*, const char*, u32))&BdatGetPtrDirect)(lbl_eu_806640D8, base + 0xC, item_id);
    char* out = mName;
    sprintf(out, base + 0x18, name);
    u8 is_jp = 1;
    u8 lang = getLanguage__9CDeviceSCFv();
    if (lang != 3 && getLanguage__9CDeviceSCFv() != 2) is_jp = 0;
    char* scan = out;
    while (*scan) {
        if (*scan == '$') {
            char c = *++scan;
            memset(tmp, 0, 0x40);
            if (c == '1') {
                scan++;
                switch (fmt) {
                    case 1: sprintf(tmp, base + 0x1B, buf); break;
                    case 2: sprintf(tmp, base + 0x1B, buf); break;
                    case 3:
                        if (is_jp) sprintf(tmp, base + 0x30, buf);
                        else sprintf(tmp, base + 0x48, buf);
                        break;
                    case 4:
                        if (is_jp) sprintf(tmp, base + 0x30, buf);
                        else sprintf(tmp, base + 0x48, buf);
                        break;
                }
            } else if (c == '2') {
                scan++;
                // 28-byte table copy (retail mtctr 3-pair loop + tail) from
                // the category string table; entry [1+category] is the text id.
                u32 tbl[7];
                u32* d = tbl;
                u32* s = (u32*)&lbl_eu_80506330;
                for (u32 k = 0; k < 3; k++) {
                    *d++ = *s++;
                }
                *d = *s;
                u8 r = (u8)BdatGetU8Direct((const void*)lbl_eu_806640D8, (const char*)tbl[1 + category], item_id);
                if (is_jp) sprintf(tmp, base + 0x5F, r);
                else sprintf(tmp, base + 0x77, r);
            }
            // re-splice: back up the tail, insert the formatted text over
            // the 2-char tag, then restore the tail after it. The length
            // scan counts the NUL (retail lbz/extsb. loop) so the final
            // tail position is scan-2+len-1.
            u32 len = 0;
            const char* p = tmp;
            s8 b;
            do {
                b = (s8)*p;
                len++;
                p++;
            } while (b != 0);
            sprintf(tailBuf, base + 0x18, scan);
            scan -= 2;
            sprintf(scan, base + 0x18, tmp);
            scan += len;
            scan -= 1;
            sprintf(scan, base + 0x18, tailBuf);
        } else {
            s8 c = (s8)*scan;
            if ((c >= 0x81 && c <= 0x9F) || (c >= 0xE0 && c <= 0xEF)) scan += 2;
            else scan += 1;
        }
    }
    return out;
}
#pragma pop

// Retail hideItemBoxPanes:
// the layout child) and clears it, then re-labels panes according to the
// item-box state byte (info+0x9A) and the equip byte (info+0xAE) and the
// category index arg2: 2/4-8 -> two panes, 3 -> one, 9/0xA/0xD -> one each.
// The switch is emitted twice (both the state==4/byte!=0 and the state!=4
// paths run the identical category dispatch). Uses the stmw/lmw frame
// (optimize_for_size prologue merge).
#pragma push
#pragma optimize_for_size on
void CItemBoxInfo::HidePanes(u16 arg2) {
    char* base = (char*)&lbl_eu_805063BC;
#define SET_PANE(_off, _val) \
    setPaneVisible(mpLayout->GetRootPane()->FindPaneByName(base + _off, true), _val)
    SET_PANE(0xc2, 0x0);
    SET_PANE(0xce, 0x0);
    SET_PANE(0xda, 0x0);
    SET_PANE(0xe6, 0x0);
    SET_PANE(0xf2, 0x0);
    SET_PANE(0xfe, 0x0);
    SET_PANE(0x10b, 0x0);
    SET_PANE(0x113, 0x0);
    SET_PANE(0x121, 0x0);
    if (mKind == 4) {
        if (mFlagAE == 0) {
            SET_PANE(0xfe, 0x1);
            SET_PANE(0x121, 0x1);
        } else {
            SET_PANE(0x113, 0x1);
            // (was switch: retail compiles this dispatch as compares)
            if ((u32)arg2 - 4 > 4) {
                if ((s16)arg2 != 2) {
                    if ((s16)arg2 != 3) {
                        if ((s16)arg2 != 9) {
                            if ((s16)arg2 != 0xa) {
                                if ((s16)arg2 == 0xd) {
                                    SET_PANE(0xf2, 0x1);
                                }
                            } else {
                                SET_PANE(0xe6, 0x1);
                            }
                        } else {
                            SET_PANE(0xce, 0x1);
                        }
                    } else {
                        SET_PANE(0xda, 0x1);
                    }
                } else {
                    SET_PANE(0xc2, 0x1);
                    SET_PANE(0x10b, 0x1);
                }
            } else {
                    SET_PANE(0xc2, 0x1);
                    SET_PANE(0x10b, 0x1);
            }
        }
    } else {
        SET_PANE(0x113, 0x1);
        // (was switch: retail compiles this dispatch as compares)
        if ((u32)arg2 - 4 > 4) {
                if ((s16)arg2 != 2) {
                    if ((s16)arg2 != 3) {
                        if ((s16)arg2 != 9) {
                            if ((s16)arg2 != 0xa) {
                                if ((s16)arg2 == 0xd) {
                                    SET_PANE(0xf2, 0x1);
                                }
                            } else {
                                SET_PANE(0xe6, 0x1);
                            }
                        } else {
                            SET_PANE(0xce, 0x1);
                        }
                    } else {
                        SET_PANE(0xda, 0x1);
                    }
                } else {
                    SET_PANE(0xc2, 0x1);
                    SET_PANE(0x10b, 0x1);
                }
            } else {
                    SET_PANE(0xc2, 0x1);
                    SET_PANE(0x10b, 0x1);
            }
    }
#undef SET_PANE
}
#pragma pop


u8 func_801E13D8(CItemBoxInfo2* info) {
    return *(u8*)((u8*)info + 0x90);
}

// GXColorS10::operator=
// implicit 4x s16 struct copy emits the batched lha/sth sequence (all loads
// first, then all stores) that matches the retail body exactly.
struct GXColorS10Copy {
    s16 r, g, b, a;
};
#pragma push
extern "C" __declspec(noinline) void __as__11_GXColorS10FRC11_GXColorS10(void* dst, const void* src) {
    s16* d = (s16*)dst;
    const s16* s = (const s16*)src;
    d[0] = s[0];
    d[1] = s[1];
    d[2] = s[2];
    d[3] = s[3];
}
#pragma pop


