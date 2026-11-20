#define IWORK_EVENT_INLINE_DTOR
// Auto-scaffolded catalog TU for kyoshin/CItemBoxInfo
// Replace stubs with high-level C/C++ during decomp.
//
// Unit shape (verified): retail built this TU with -O4,s (optimize for size),
// set once in configure.py - that is what keeps the same-TU helpers out of line
// (real `bl`s) and produces the retail stmw/lmw frames. No noinline scaffolding
// is needed or wanted here; the single optimize_for_size pair below marks the
// one function retail demonstrably compiled in speed mode (see its comment).

#include "kyoshin/cf/CfBdat.hpp"
#include <cstring>
extern "C" int Counter_TestBit(void*, u32);
#include "kyoshin/code_8025FB10.hpp"
#include "kyoshin/cf/CItem.hpp"
#include "kyoshin/CTitleAHelp.hpp"
#include "kyoshin/CItemBoxInfo.hpp"
#include "kyoshin/CQuestWindow_Pane.hpp"
#include "kyoshin/code_80135FDC_State.hpp"
#include "kyoshin/CUICfManager_Res.hpp"

// Unit-local imports (kept out of the class header so includers only see the classes).
extern "C" void LayoutSetTextBoxFmtValue(nw4r::lyt::Layout*, const char*, const char*, u32);
extern "C" void* CtrlObjectParam_GetCharEntry(u32);
extern "C" void SplitU32ToS16s(void*, u32);

extern "C" void __as__11_GXColorS10FRC11_GXColorS10(void* dst, const void* src);
extern "C" char lbl_eu_8052E590[];
extern "C" char lbl_eu_8052E568[];
extern "C" void copyVEC3(void*, const void*);
// C-linkage position-set helper (retail-unmangled ScalePaneWorldPos; same
// signature as CSysWin.hpp / CMenuPlayAward.hpp).
// More flat retail (unmangled) helpers used by the item-box renderers.
extern "C" u32 func_801392C0();
extern "C" void setItemBoxVtxColor(void*, u32, const void*);
// 9-byte flag-record builder: (out, unused, data) - param2 is never read in
// retail (callers pass the item-box pointer through it).
extern "C" u32 CtrlObjectParam_GetCurrentRowKey(void*);
extern "C" int IsSkillItem(void*);
extern "C" u32 isItemBoxType9Flagged(void*);

// 0xA8-byte detail record built by buildItemBoxNameRec for renderItemBoxKind3; only the
extern "C" void* CItemBlock_getPtr20E8();

// Layout-build helpers for OnFileEvent (retail mangled C++ symbols; keep
// signature shapes verbatim so the mangled names line up).


#include "kyoshin/code_80135FDC_Bdat.hpp"
#include "kyoshin/code_80135FDC_Lyt.hpp"

// MWCC 8-packs
// get real symbols; interior names keep UNDEF extern reads (words stay zero,
// relocs gate-skipped), exactly like the pre-typify linkage.


// Rank names as individual constants; their retail labels point at the
// separate 10-byte strings, not at a single packed character blob.
static const char itemBoxRankE[] = "percent_E"; // name-guess: retail lbl_eu_805062E8 string
static const char itemBoxRankD[] = "percent_D";
static const char itemBoxRankC[] = "percent_C";
static const char itemBoxRankB[] = "percent_B";
static const char itemBoxRankA[] = "percent_A";
static const char itemBoxRankS[] = "percent_S";

// Category label tables: the second is the first reversed after its empty
// header entry. MWCC emits them in definition order in .rodata.
static const char* const itemBoxInfoCategoryStringTables[7] = {
    "", itemBoxRankE, itemBoxRankD, itemBoxRankC, itemBoxRankB, itemBoxRankA, itemBoxRankS
};
static const char* const itemBoxCategoryLabelStrings[7] = { // name-guess: retail lbl_eu_8050634C table
    "", itemBoxRankS, itemBoxRankA, itemBoxRankB, itemBoxRankC, itemBoxRankD, itemBoxRankE
};

static const u32 itemBoxInfoEquippedTypeIds[6] = { 2, 4, 5, 6, 7, 8 };
static const char* const itemBoxLabelStrings[7] = { // name-guess: retail lbl_eu_80506380 table
    "", itemBoxRankS, itemBoxRankA, itemBoxRankB, itemBoxRankC, itemBoxRankD, itemBoxRankE
};
// Interior blob
// Code must relocate against the retail symbol name with plain immediates.

// .sdata: two RTTI-ish pairs (class-name string + .data RTTI pad). Both
// halves stay UNDEF reads: the strings are struct members below and the
// pads live in the hand-typed .data at file end.


// .sbss 0xC0: palette globals in retail order (NOBITS: size+align gated).
// 4-byte entries are u32, 8-byte entries GXColorS10; first carries aligned(8)
// for the section align, the rest pack without pads.
__attribute__((aligned(8))) __attribute__((used)) u32 itemBoxInfo_labelNeutralTop;
__attribute__((used)) u32 itemBoxInfo_labelNeutralTopPad;
__attribute__((used)) u32 itemBoxInfo_labelNeutralBottom;
__attribute__((used)) u32 itemBoxInfo_labelNeutralBottomPad;
__attribute__((used)) GXColorS10 itemBoxInfo_labelUpTop;
__attribute__((used)) GXColorS10 itemBoxInfo_labelUpBottom;
__attribute__((used)) GXColorS10 itemBoxInfo_labelDownTop;
__attribute__((used)) GXColorS10 itemBoxInfo_labelDownBottom;
__attribute__((used)) GXColorS10 itemBoxInfo_labelSpecialTop;
__attribute__((used)) GXColorS10 itemBoxInfo_labelSpecialBottom;
__attribute__((used)) u32 itemBoxInfo_valueNeutralTop;
__attribute__((used)) u32 itemBoxInfo_valueNeutralTopPad;
__attribute__((used)) u32 itemBoxInfo_valueNeutralBottom;
__attribute__((used)) u32 itemBoxInfo_valueNeutralBottomPad;
__attribute__((used)) GXColorS10 itemBoxInfo_valueUpTop;
__attribute__((used)) GXColorS10 itemBoxInfo_valueUpBottom;
__attribute__((used)) GXColorS10 itemBoxInfo_valueDownTop;
__attribute__((used)) GXColorS10 itemBoxInfo_valueDownBottom;
__attribute__((used)) GXColorS10 itemBoxInfo_valueSpecialTop;
__attribute__((used)) GXColorS10 itemBoxInfo_valueSpecialBottom;
__attribute__((used)) GXColorS10 itemBoxInfo_slotLabelTop;
__attribute__((used)) GXColorS10 itemBoxInfo_slotLabelBottom;
__attribute__((used)) u32 itemBoxInfo_slotValueTop;
__attribute__((used)) u32 itemBoxInfo_slotValueTopPad;
__attribute__((used)) u32 itemBoxInfo_slotValueBottom;
__attribute__((used)) u32 itemBoxInfo_slotValueBottomPad;
__attribute__((used)) GXColorS10 itemBoxInfo_nameLabelTop;
__attribute__((used)) GXColorS10 itemBoxInfo_nameLabelBottom;
__attribute__((used)) u32 itemBoxInfo_nameValueTop;
__attribute__((used)) u32 itemBoxInfo_nameValueTopPad;
__attribute__((used)) u32 itemBoxInfo_nameValueBottom;
__attribute__((used)) u32 itemBoxInfo_nameValueBottomPad;




#include "decomp.h"





#include "monolib/util.hpp"
#include "monolib/device/CDeviceFile.hpp"
#include <nw4r/lyt/lyt_layout.h>
#include <nw4r/lyt/lyt_pane.h>
#include <nw4r/lyt/lyt_arcResourceAccessor.h>
#include "monolib/work/CEventFile.hpp"
#include "monolib/device/CFileHandle.hpp"
#include "monolib/device/CDeviceFont.hpp"  // IDeviceFontInfo::getFont (+0x24)
#include "monolib/device/CDeviceSC.hpp" // CDeviceSC::getLanguage
#include "monolib/lib/CLibLayout.hpp"  // CLibLayout::createArcResourceAccessor
#include <nw4r/db/db_assert.h>
#include "kyoshin/cf/object/CActorParam.hpp"  // CActorParam slots (+0xA4/+0x108/...) for char-data stats
#include <new>

// Retail is 2-arg (category index + s16 item id); the real declaration now
// comes from kyoshin/cf/CItem.hpp (CItemExt* return).
// Legacy 1-arg form (category 0) used by not-yet-matched reconstructions.
static inline void* CItem_kindPtr_1(u32 id) { return CItem_kindPtr(0, (s16)id); }
// --- Forward declarations ---
namespace nw4r { namespace lyt { class Layout; class DrawInfo; class AnimTransform; } }
void func_80127BD8(void*, float*);
u32 func_801392C0();

// Root-pane view of nw4r::lyt::Layout (mpRootPane at +0x10). The two
// GetSlotPos bodies still need this spelling: Layout::GetRootPane() is a
// pure reg-swap regression there (8 reg-swaps, 100% -> 80%); measured.
struct LayoutRootPaneView {
    u8 _00[0x10];
    nw4r::lyt::Pane* rootPane10;
};
extern "C" u32 CtrlObjectParam_GetArtsDataWord(void*);
extern "C" void* CtrlObjectParam_GetSlotTableBase();
extern "C" void* findObjB28ById(u32);
extern "C" void CtrlObjectParam_SyncParamFromActorEx(void*, u32);
extern "C" void CtrlObjectParam_GetArtsStatsRow(void*, u32);
extern "C" s32 RoundHalfAway0(float);
extern void* CItemBlock_getPtr20E8();
// 6-byte item-box slot tables (pair of u32+u16 .sdata2 constants).



struct ItemBoxInfoCopy;
void copyItemBoxEntry(CItemBoxInfoEntry*, const CItemBoxInfoEntry*);
void setItemBoxEntry(CItemBoxInfoEntry*, u16, u32, u8);
void copyItemBoxCopy(ItemBoxInfoCopy*, const ItemBoxInfoCopy*);
void setItemBoxCopy(ItemBoxInfoCopy*, unsigned short, unsigned int, unsigned char);

// --- CItemBoxInfo methods ---


// Retail formatItemBoxName: fills the item-name buffer at item_data+0xD9 from
// the item table, expanding the $1/$2 format tags in place: $1 is replaced
// by the item name (format select from the BdatGetU8Direct byte value), $2 by
// the category entry text. Both insertions re-splice the tail via the
// three-sprintf sequence (tail backup, insert, tail restore). Retail uses
// the stmw frame and keeps the mtctr table-copy loop (size-optimized).
char* CItemBoxPage::GetItemName(u8 index) {
    if (index >= 12) return NULL;
    u16 item_id = mItemIds[index];
    if (item_id == 0) return NULL;
    u8 category = mBytesB4[index];
    s16 val = mValues[index];
    char tailBuf[0x40];
    char tmp[0x40];
    char buf[0x40];
    sprintf(buf, "%d", val);
    u8 fmt = (u8)BdatGetU8Direct((const void*)::cf::CfBdat::spBtlSkillListFileData, "rvs_type", item_id);
    char* name = ((char*(*)(void*, const char*, u32))&BdatGetPtrDirect)(::cf::CfBdat::spBtlSkillListFileData, "rvs_caption", item_id);
    char* out = mText.mName;
    sprintf(out, "%s", name);
    u8 is_jp = 1;
    u8 lang = CDeviceSC::getLanguage();
    if (lang != 3 && CDeviceSC::getLanguage() != 2) is_jp = 0;
    char* scan = out;
    while (*scan) {
        if (*scan == '$') {
            char c = *++scan;
            memset(tmp, 0, 0x40);
            if (c == '1') {
                scan++;
                switch (fmt) {
                    case 1: sprintf(tmp, "<col=red>%s<col=def>", buf); break;
                    case 2: sprintf(tmp, "<col=red>%s<col=def>", buf); break;
                    case 3:
                        if (is_jp) sprintf(tmp, "<col=red>%s %%<col=def>", buf);
                        else sprintf(tmp, "<col=red>%s%%<col=def>", buf);
                        break;
                    case 4:
                        if (is_jp) sprintf(tmp, "<col=red>%s %%<col=def>", buf);
                        else sprintf(tmp, "<col=red>%s%%<col=def>", buf);
                        break;
                }
            } else if (c == '2') {
                scan++;
                // 28-byte table copy (retail mtctr 3-pair loop + tail) from
                // the category string table; entry [1+category] is the text id.
                u32 tbl[7];
                u32* d = tbl;
                u32* s = (u32*)itemBoxInfoCategoryStringTables;
                for (u32 k = 0; k < 3; k++) {
                    *d++ = *s++;
                }
                *d = *s;
                u8 r = (u8)BdatGetU8Direct((const void*)::cf::CfBdat::spBtlSkillListFileData, (const char*)tbl[1 + category], item_id);
                if (is_jp) sprintf(tmp, "<col=red>%d %%<col=def>", r);
                else sprintf(tmp, "<col=red>%d%%<col=def>", r);
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
            sprintf(tailBuf, "%s", scan);
            scan -= 2;
            sprintf(scan, "%s", tmp);
            scan += len;
            scan -= 1;
            sprintf(scan, "%s", tailBuf);
        } else {
            s8 c = (s8)*scan;
            if ((c >= 0x81 && c <= 0x9F) || (c >= 0xE0 && c <= 0xEF)) scan += 2;
            else scan += 1;
        }
    }
    return out;
}

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

void CItemBoxPage::Reset() {}

CItemBoxInfo::~CItemBoxInfo() {}


// The setHandleFlag1 call reuses the readFile result in r3.
void CItemBoxInfo::LoadFiles() {
    mtl::ALLOC_HANDLE handle = mtl::MemManager::getHandleMEM2();
    CFileHandle* fh1 = CDeviceFile::readFile(handle, "menu/jp/ItemBoxInfo.arc", reinterpret_cast<IWorkEvent*>(this), 0, 0);
    mFileHandle1 = fh1;
    CDeviceFile::setHandleFlag1(fh1);
    handle = mtl::MemManager::getHandleMEM2();
    CFileHandle* fh2 = CDeviceFile::readFile(handle, "menu/jp/tpl/CrystalIcon.arc", reinterpret_cast<IWorkEvent*>(this), 0, 0);
    mFileHandle2 = fh2;
    CDeviceFile::setHandleFlag1(fh2);
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

void CItemBoxInfo::DrawLayout(nw4r::lyt::DrawInfo* drawInfo) {
    if (mActive != 0) {
        drawLayout((nw4r::lyt::Layout*)mpLayout, drawInfo, 0, 1);
    }
}

void func_80127BD8(void*, float*);

// TU-local cast-only views (CItemBoxInfo.hpp is shared, so these live here):
// - CItemBoxVecTable: the info+0xB0 table (the header keeps the raw _B0 u8
//   view): 12 s16 ids followed by twelve 3-float VEC3 slots at +0x18.
// The CtrlObjectParam_GetCharEntry record is CItemBoxCharEntry in the header;
// its six s16 slotIds and counterArea are shared with the other item-box UIs.
struct CItemBoxVecTable {
    s16 ids[12];       // +0x00 twelve slot ids
    float vecs[12][3]; // +0x18 twelve VEC3 slots
};

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
u32 CItemBoxInfo::GetState() {
    return mCurrent;
}

// --- Remaining CItemBoxInfo stubs ---


void CItemBoxInfo::Open() {
    if (mState != 0) return;
    mState = 1;
    mVisible = 0;
    StampSlotTex();
}

void CItemBoxInfo::Close() {
    if (mState == 3) {
        mState = 4;
        mVisible = 0;
    }
}

// Retail hideItemBoxPanes: the layout child) and clears it, then re-labels panes
// according to the item-box state byte (info+0x9A) and the equip byte (info+0xAE) and
// the category index arg2: 2/4-8 -> two panes, 3 -> one, 9/0xA/0xD -> one each. The
// switch is emitted twice (both the state==4/byte!=0 and the state!=4 paths run the
// identical category dispatch).
void CItemBoxInfo::HidePanes(u16 arg2) {
    setPaneVisible(mpLayout->GetRootPane()->FindPaneByName("nul_para_01", true), 0x0);
    setPaneVisible(mpLayout->GetRootPane()->FindPaneByName("nul_para_30", true), 0x0);
    setPaneVisible(mpLayout->GetRootPane()->FindPaneByName("nul_para_50", true), 0x0);
    setPaneVisible(mpLayout->GetRootPane()->FindPaneByName("nul_para_60", true), 0x0);
    setPaneVisible(mpLayout->GetRootPane()->FindPaneByName("nul_para_80", true), 0x0);
    setPaneVisible(mpLayout->GetRootPane()->FindPaneByName("nul_para_700", true), 0x0);
    setPaneVisible(mpLayout->GetRootPane()->FindPaneByName("nul_eth", true), 0x0);
    setPaneVisible(mpLayout->GetRootPane()->FindPaneByName("nul_infbcln01", true), 0x0);
    setPaneVisible(mpLayout->GetRootPane()->FindPaneByName("nul_infbcln700", true), 0x0);
    if (mKind == 4) {
        if (mFlagAE == 0) {
            setPaneVisible(mpLayout->GetRootPane()->FindPaneByName("nul_para_700", true), 0x1);
            setPaneVisible(mpLayout->GetRootPane()->FindPaneByName("nul_infbcln700", true), 0x1);
        } else {
            setPaneVisible(mpLayout->GetRootPane()->FindPaneByName("nul_infbcln01", true), 0x1);
            // (was switch: retail compiles this dispatch as compares)
            if ((u32)arg2 - 4 > 4) {
                if ((s16)arg2 != 2) {
                    if ((s16)arg2 != 3) {
                        if ((s16)arg2 != 9) {
                            if ((s16)arg2 != 0xa) {
                                if ((s16)arg2 == 0xd) {
                                    setPaneVisible(mpLayout->GetRootPane()->FindPaneByName("nul_para_80", true), 0x1);
                                }
                            } else {
                                setPaneVisible(mpLayout->GetRootPane()->FindPaneByName("nul_para_60", true), 0x1);
                            }
                        } else {
                            setPaneVisible(mpLayout->GetRootPane()->FindPaneByName("nul_para_30", true), 0x1);
                        }
                    } else {
                        setPaneVisible(mpLayout->GetRootPane()->FindPaneByName("nul_para_50", true), 0x1);
                    }
                } else {
                    setPaneVisible(mpLayout->GetRootPane()->FindPaneByName("nul_para_01", true), 0x1);
                    setPaneVisible(mpLayout->GetRootPane()->FindPaneByName("nul_eth", true), 0x1);
                }
            } else {
                    setPaneVisible(mpLayout->GetRootPane()->FindPaneByName("nul_para_01", true), 0x1);
                    setPaneVisible(mpLayout->GetRootPane()->FindPaneByName("nul_eth", true), 0x1);
            }
        }
    } else {
        setPaneVisible(mpLayout->GetRootPane()->FindPaneByName("nul_infbcln01", true), 0x1);
        // (was switch: retail compiles this dispatch as compares)
        if ((u32)arg2 - 4 > 4) {
                if ((s16)arg2 != 2) {
                    if ((s16)arg2 != 3) {
                        if ((s16)arg2 != 9) {
                            if ((s16)arg2 != 0xa) {
                                if ((s16)arg2 == 0xd) {
                                    setPaneVisible(mpLayout->GetRootPane()->FindPaneByName("nul_para_80", true), 0x1);
                                }
                            } else {
                                setPaneVisible(mpLayout->GetRootPane()->FindPaneByName("nul_para_60", true), 0x1);
                            }
                        } else {
                            setPaneVisible(mpLayout->GetRootPane()->FindPaneByName("nul_para_30", true), 0x1);
                        }
                    } else {
                        setPaneVisible(mpLayout->GetRootPane()->FindPaneByName("nul_para_50", true), 0x1);
                    }
                } else {
                    setPaneVisible(mpLayout->GetRootPane()->FindPaneByName("nul_para_01", true), 0x1);
                    setPaneVisible(mpLayout->GetRootPane()->FindPaneByName("nul_eth", true), 0x1);
                }
            } else {
                    setPaneVisible(mpLayout->GetRootPane()->FindPaneByName("nul_para_01", true), 0x1);
                    setPaneVisible(mpLayout->GetRootPane()->FindPaneByName("nul_eth", true), 0x1);
            }
    }
}


u8 func_801E13D8(CItemBoxInfo2* info) {
    return info->mActive;
}

// GXColorS10::operator=
// implicit 4x s16 struct copy emits the batched lha/sth sequence (all loads
// first, then all stores) that matches the retail body exactly.
struct GXColorS10Copy {
    s16 r, g, b, a;
};

// Retail renderItemBox2ByKind: ItemBox2 info dispatch. Zeroes the slot VEC3 block,
// renders the base panel, derives the item id (or the candidate's own), then
// dispatches on the item category to the per-category renderers. Ends by formatting a
// header string into a FixStr and pushing it to the layout.
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
    char* label = BdatTouchStringCell("MNU_item", "name", 3);
    // Default ctor clears inline (mString[0]=0 / mLength=0) -- MWCC outlines the
    // FixStr(bool) ctor once it has 2+ call sites in a TU, the default one it
    // always inlines.
    ml::FixStr<32> text;
    text.format("%d%s", CItemBlock_getPtr20E8(), label);
    LayoutSetTextBoxFmtValue((nw4r::lyt::Layout*)mpLayout, "txt_gold01_00",
                  text.c_str(), NULL);
}
//

void CItemBoxPage::Clear() {
    float f = 0.0f;
    for (u8 i = 0; i < 12; i++) {
        ((CItemBoxVecTable*)this)->ids[i] = 0;
        float temp[3] = {f, f, f};
        copyVEC3(((CItemBoxVecTable*)this)->vecs[i], temp);
    }
}

u32 isItemBoxType9Flagged(void* arg) {
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

void CItemBoxInfo::SetNamedText(int index, char* text) {
    char buf[0x20];
    sprintf(buf, "txt_excange%02d", index);
    LayoutSetTextBoxFmtValue((nw4r::lyt::Layout*)mpLayout, buf, text, 0);
}


nw4r::math::VEC3 CItemBoxInfo::GetSlotPos(u32 arg2) {
    char buf[0x20];
    // Declare `first` before `child`: low-degree webs color in reverse birth
    // order, so child (declared later) colors first into this's dead r30 and
    // first reuses the pool base's r31 (matches retail r29/r30/r31 layout).
    nw4r::lyt::Pane* first;
    nw4r::lyt::Pane* child;
    nw4r::lyt::Pane* second;
    nw4r::math::VEC3 pos;
    sprintf(buf, "pic_pcbs%02d", arg2 + 1);
    child = ((LayoutRootPaneView*)mpLayout)->rootPane10;
    first = child->FindPaneByName("nul_proportion", true);
    second = child->FindPaneByName(buf, true);
    ScalePaneWorldPos(&pos, second, first, child);
    return pos;
}

void CItemBoxInfo::SetCounts(u8 arg2, u8 arg3) {
    if (mpLayout != 0) {
        setLayoutTextBoxNumber((nw4r::lyt::Layout*)mpLayout, "txt_scnd01", arg2);
        setLayoutTextBoxNumber((nw4r::lyt::Layout*)mpLayout, "txt_scnd03", arg3);
    }
}

void CItemBoxInfo::SetResultPane(bool visible) {
    if (mpLayout == 0) return;
    nw4r::lyt::Pane* child = mpLayout->GetRootPane();
    nw4r::lyt::Pane* result = child->FindPaneByName("nul_scnd", true);
    setPaneVisible(result, visible);
}

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

// Per-slot banks: +0x16 holds a name-resolved flag (forced for slot 0xB when cat ==
// 0x4C), +0x1D/+0x24/ +0x2B hold {1,2,0} comparisons of the looked-up item's table
// values against the candidate item's.
CItemBoxCompRecord CItemBoxInfo::BuildCmpRecord(void* arg3) {
    // Scratch doubles for MWCC's int->float magic (2^52 unsigned repair,
    // 2^63 sign-repair); retail materializes both high words up front.
    CItemBoxCompRecord rec;
    void* global = ::cf::CfBdat::spItmWpnListFileData;
    BdatGetItemType((u32)arg3);
    // v2 is a narrow-typed local: MWCC keeps it zero-extended and re-applies
    // the u16->u32 promotion per call site (retail clrlwi r5,r29,16 each
    // time, no shared masked temp).
    u16 v2 = BdatGetItemId((u32)arg3);
    rec.s2C =(u16)BdatGetU16Direct((const void*)global, (const char*)((const void*)("dmg_low")), v2);
    rec.s2E = (u16)BdatGetU16Direct((const void*)global, (const char*)((const void*)("dmg_hi")), v2);
    rec.s30 = (u8)BdatGetU8Direct((const void*)global, "arm_phy", v2);
    rec.s32 = (u8)BdatGetU8Direct((const void*)global, "arm_eth", v2);
    rec.s34 = (u8)BdatGetU8Direct((const void*)global, "att_lev", v2);
    rec.f38 = (f32)((u8)BdatGetU8Direct((const void*)global, "speed", v2) / 10.0f);
    rec.s3C = (u8)BdatGetU8Direct((const void*)global, "grd_rate", v2);
    if (BdatGetU8Direct((const void*)global, "flag", v2) & 4) {
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
        sprintf(buf, "equip_pc%d", (u8)slot);
        u16 flag = BdatGetU8Direct((const void*)global, (const char*)buf, v2);
        // name-flag bank (+0x16): set when the slot tag resolves; slot 0xB
        // is forced on for candidate category 0x4C.
        rec.flags[0][i] = (flag != 0);
        if ((u8)slot == 0xb && v2 == 0x4c) rec.flags[0][i] = 1;
        if (rec.flags[0][i] == 0) continue;
        rec.f38 += GetFloatTableEntry((u8)slot);
        void* lookup = CtrlObjectParam_GetCharEntry((u8)slot);
        void* item = CItem_kindPtr(2, ((CItemBoxCharEntry*)lookup)->slotIds[5]);
        if (item == NULL || *(u32*)item == 0) continue;
        // cat is likewise narrow-typed for per-site promotion.
        u16 cat = BdatGetItemId(*(u32*)item >> 20);
        u16 a = (u16)BdatGetU16Direct((const void*)global, (const char*)((const void*)("dmg_low")), cat);
        u16 b = (u16)BdatGetU16Direct((const void*)global, (const char*)((const void*)("dmg_hi")), cat);
        f32 ratio = (f32)((u8)BdatGetU8Direct((const void*)global, "speed", cat) / 10.0f);
        rec.f28 = BlendFloatAvgScale((u32)a, (u32)b, 0, ratio);
        u8 v1ab = (u8)BdatGetU8Direct((const void*)global, "arm_phy", cat);
        u8 v1b3 = (u8)BdatGetU8Direct((const void*)global, "arm_eth", cat);
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
// The null path memsets the record, copies, and returns early; the filled path falls
// through to its own copy of the same loop. Per-slot flag banks are addressed through
// one walked pointer (&flags[i], +0/+7/+14).
CItemBoxSlotFlags CItemBoxInfo::BuildSlotFlags(u32 item) {
    CItemBoxSlotFlags rec;
    if (item == 0) {
        memset(&rec, 0, 0x1C);
        return rec;
    }
    void* global = ::cf::CfBdat::spItmEquipListFileData;
    u16 type = BdatGetItemType(item);
    u16 id = BdatGetItemId(item);
    rec.v[0] = (u8)BdatGetU8Direct((const void*)global, "arm_phy", id);
    rec.v[1] = (u8)BdatGetU8Direct((const void*)global, "arm_eth", id);
    rec.v[2] = (u8)BdatGetU8Direct((const void*)global, "eva_rate", id);
    u32 reqKind = BdatGetU8Direct((const void*)::cf::CfBdat::spItmEquipListFileData, "arm_type", id);
    for (u32 i = 0; (u8)i < (u8)code80135FDC_getByte_64077(); i++) {
        u8 slot = GetCollectedFlagByte((u8)i);
        char buf[0x20];
        sprintf(buf, "pc%d", (u8)slot);
        rec.equipable[(u8)i] = (u8)BdatGetU8Direct((const void*)::cf::CfBdat::spItmEquipListFileData, (const char*)buf, id) != 0;
        if (rec.equipable[(u8)i] == 0) continue;
        CItemBoxCharEntry* member = (CItemBoxCharEntry*)CtrlObjectParam_GetCharEntry((u8)slot);
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
                    u8 n1 = (u8)BdatGetU8Direct((const void*)global, "arm_phy", cat);
                    u8 n2 = (u8)BdatGetU8Direct((const void*)global, "arm_eth", cat);
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
// The name/category tables are resolved through the item vtable (arg3) or the raw data
// pointers (arg3 == NULL); the text buffer is then scanned for '$' markers ('$1'
// switches on the equip-state byte, '$2' copies the static label-pointer table at
// lbl_eu_8050634C and formats the count-indexed label); other bytes advance
// Shift-JIS-aware.
CItemBoxNameRecord CItemBoxInfo::BuildNameRec(void* data, void* arg3) {
    u32 cat;
    u32 count;
    void* item = arg3 != NULL ? arg3 : NULL;
    // Retail hoists this global into a register (r26) across all calls.
    void* gd8 = ::cf::CfBdat::spBtlSkillListFileData;
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
        count = BdatGetU8Direct((const void*)::cf::CfBdat::spItmItemListFileData, "rankType", (u32)data);
    }
    CItemBoxNameRecord rec;
    rec.count = (u8)count;
    rec.str = (u32)BdatTouchStringCell("MNU_item", "name", 0x1e - ((u8)count - 1));
    char* s2 = BdatTouchStringCell("MNU_shop", "name", 0xf);
    if (arg3 != NULL) {
        CItemImpl* inst = CItem_initItemImplInstances((CItemData*)item);
        sprintf(rec.name, "%d", inst->vf90((CItemData*)item));
    } else {
        sprintf(rec.name, "%s", s2);
    }
    rec.e0 = (u8)BdatGetU8Direct((const void*)gd8, "rvs_type", (u16)cat);
    rec.e1 = (u8)BdatGetU8Direct((const void*)gd8, "atr_type", (u16)cat);
    rec.color = 0xFFFFFFFF;
    switch (rec.e1) {
        case 4: rec.color = 0xFF00FFFF; break;
        case 5: rec.color = 0x0000FFFF; break;
        case 6: rec.color = 0xFFFFFFFF; break;
        case 7: rec.color = 0x0000FFFF; break;
        case 8: rec.color = 0x00FFFFFF; break;
        case 9: rec.color = 0x775544FF; break;
    }
    sprintf(rec.text, "%s",
            ((char* (*)(void*, const char*, u16))&BdatGetPtrDirect)(gd8, "rvs_caption", (u16)cat));
    u32 wide = 1;
    if (CDeviceSC::getLanguage() != 3) {
        if (CDeviceSC::getLanguage() != 2) {
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
                        sprintf(buf28, "<col=red>%s<col=def>", rec.name);
                        break;
                    case 2:
                        sprintf(buf28, "<col=red>%s<col=def>", rec.name);
                        break;
                    case 3:
                        if (wide != 0) {
                            sprintf(buf28, "<col=red>%s %%<col=def>", rec.name);
                        } else {
                            sprintf(buf28, "<col=red>%s%%<col=def>", rec.name);
                        }
                        break;
                    case 4:
                        if (wide != 0) {
                            sprintf(buf28, "<col=red>%s %%<col=def>", rec.name);
                        } else {
                            sprintf(buf28, "<col=red>%s%%<col=def>", rec.name);
                        }
                        break;
                    default:
                        break;
                }
            } else if (*cur == '2') {
                // 0x1C-byte move of the static label-pointer table into the
                // stack copy (retail mtctr 3-pair loop + 4-byte tail).
                {
                    u32* s = (u32*)itemBoxCategoryLabelStrings - 1;
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
                    sprintf(buf28, "<col=red>%d %%<col=def>", (u8)name);
                } else {
                    sprintf(buf28, "<col=red>%d%%<col=def>", (u8)name);
                }
            }
            // strlen of buf28: retail's counter runs one past the terminator
            // (len+1), folded back out by the len-1 splice offset below.
            u32 len = 1;
            while (buf28[len - 1] != 0) {
                len++;
            }
            sprintf(buf48, "%s", cur);
            cur -= 2;
            sprintf(cur, "%s", buf28);
            cur = cur + len - 1;
            sprintf(cur, "%s", buf48);
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

// Retail keeps the lookup-table pointer live in r29 across all calls and spills the
// four results to stack slots, reloading them for the final four output-word stores.
CItemBoxLabelRec CItemBoxInfo::GetLabelRec(u32 item) {
    CItemBoxLabelRec r;
    void* tbl = ::cf::CfBdat::spItmCollectListFileData;
    BdatGetItemType(item);
    // cat stays a raw u16 in a register; each call site re-applies the
    // u16->u32 argument conversion (retail clrlwi per site, no shared temp).
    u16 cat = BdatGetItemId(item);
    // base is anchored at the first lookup so its definition schedules after
    // the cat move (retail order: or r30,r3 then lis/addi).
    r.a = (u8)BdatGetU8Direct((const void*)tbl, "type", cat);
    r.b = BdatTouchStringCell("MNU_collect", "name", r.a);
    // The label ID is also the row key passed to BdatGetPtrDirect.
    r.c = (u8)BdatGetU8Direct((const void*)tbl, "mapID", cat);
    if (r.c == 0x1A) {
        r.d = BdatTouchStringCell("MNU_collect", "name", 0x14);
    } else {
        r.d = BdatGetPtrDirect(::cf::CfBdat::spFldMapListFileData, "name", r.c);
    }
    // Returned by value: the caller's hidden result pointer receives the
    // record as four lwz/stw pairs.
    return r;
}

CItemBoxSelRec CItemBoxInfo::BuildSelRec(u32 item) {
    void* global = ::cf::CfBdat::spItmArtsListFileData;
    u16 v1 = BdatGetItemType(item);
    u16 v2 = BdatGetItemId(item);
    CItemBoxSelRec rec;
    rec.category = (u8)BdatGetU8Direct((const void*)global, (char*)"memory_type", v2);
    // count stays u32: retail keeps the raw call result (r28) and masks at
    // each (u8)count use (loop mask hoisted to r29).
    u8 count = BdatGetU8Direct((const void*)global, (char*)"pc_type", v2);
    for (u32 i = 0; (u8)i < (u8)code80135FDC_getByte_64077(); i++) {
        u8 a = (u8)GetCollectedFlagByte((u8)i);
        rec.selected[(u8)i] = a == (u8)count;
    }
    u8 key = BdatGetU8Direct((const void*)global, (char*)"get_arts", v2);
    u8 val = BdatGetU8ByTableKey((char*)"pc_arts", (char*)"idx", (u8)key);
    void* lookup = CtrlObjectParam_GetCharEntry((u8)count);
    u8 cat = (u8)CtrlObjectParam_GetCurrentRowKey(lookup);
    rec.flag2 = 0;
    u8* entry = (u8*)lookup + cat * 0x49 + val * 2;
    switch (rec.category) {
        case 1: if (entry[0xE8] != 0) rec.flag2 = 1; break;
        case 2: if ((entry[0xE9] >> 7) & 1) rec.flag2 = 1; break;
        case 3: if ((entry[0xE9] >> 6) & 1) rec.flag2 = 1; break;
    }
    return rec;
}
CItemBoxSlotRecord1 CItemBoxInfo::BuildSlotRec(u32 item, void* arg3) {
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
    rec.str = (u32)BdatTouchStringCell("MNU_item", "name", 0x1e - ((u8)v08 - 1));
    rec.counter = 0;
    for (u32 i = 0; i < 4; i++) {
        u32 n = (u16)CItem_initItemImplInstances((CItemData*)p)->vf4C((CItemData*)p, (u8)i);
        // Signed >0 test: folds into the record-form clrlwi as retail's ble.
        if ((s32)n > 0) {
            // Retail passes the pair count n as a third arg to BdatGetPtrDirect.
            rec.text[rec.counter] =
                (u32)((char*(*)(void*, char*, u32))&BdatGetPtrDirect)(::cf::CfBdat::spBtlSkillListFileData, "name", n);
            rec.vals[rec.counter] = CItem_initItemImplInstances((CItemData*)p)->vf64((CItemData*)p, (u8)i);
            rec.counts[rec.counter] = n;
            rec.counter++;
        }
    }
    // Returned by value: the 0x2C-byte copy to the hidden result pointer is
    // the retail mtctr 8-byte-pair loop (li r0,5 + 4-byte tail).
    return rec;
}
// Retail renderItemBoxKind2: item-box renderer for selection kind 2. Builds the
// 0x34 comparison record (buildItemBoxCmpRecord), formats three stat panes from it
// through an ml::FixStr<32>, resets the highlight panes, then per visible
// slot colours the row pane and stamps the candidate-item texture.
void CItemBoxInfo::RenderKind2(u16 arg2, void* arg3, u32 arg4) {
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
    // Raw-storage binding: retail inlines the bool-ctor clear as bare stb/stw
    // stores; format() is invoked through a cast so no reference web occupies
    // a register.
    text.mString[0] = 0;
    text.mLength = 0;
    // NOTE (open item): spelling the label-pool base inline at every use
    // re-anchors it MORE than retail (which uses r19/r19/r28) and regressed
    // the diff; one cached base pointer is the closest shape tried so far.
    char* lbl = BdatTouchStringCell("MNU_item", "name", 0xb);
    ((ml::FixStr<32>*)&text)->format("%d%s%d", local.s2C, lbl, local.s2E);
    LayoutSetTextBoxFmtValue((nw4r::lyt::Layout*)mpLayout, "txt_value02", text.mString, 0);
    setLayoutTextBoxNumber((nw4r::lyt::Layout*)mpLayout, "txt_value08", (u8)local.s30);
    setLayoutTextBoxNumber((nw4r::lyt::Layout*)mpLayout, "txt_value09", (u8)local.s32);
    char* str7f = BdatTouchStringCell("MNU_item", "name", 0x7f);
    char* str80 = BdatTouchStringCell("MNU_item", "name", 0x80);
    if (local.s34 != 0) {
        ((ml::FixStr<32>*)&text)->format("%s%d%s", str7f, str80, local.s34);
    } else {
        ((ml::FixStr<32>*)&text)->format("%d%s", local.s34, str80);
    }
    LayoutSetTextBoxFmtValue((nw4r::lyt::Layout*)mpLayout, "txt_value04", text.mString, 0);
    str7f = BdatTouchStringCell("MNU_item", "name", 0x7f);
    str80 = BdatTouchStringCell("MNU_item", "name", 0x80);
    if (local.s3C != 0) {
        ((ml::FixStr<32>*)&text)->format("%s%d%s", str7f, str80, local.s3C);
    } else {
        ((ml::FixStr<32>*)&text)->format("%d%s", local.s3C, str80);
    }
    LayoutSetTextBoxFmtValue((nw4r::lyt::Layout*)mpLayout, "txt_value07", text.mString, 0);
    LayoutSetTextBoxFmtValue((nw4r::lyt::Layout*)mpLayout, "txt_value22", "", 0);
    LayoutSetTextBoxFmtValue((nw4r::lyt::Layout*)mpLayout, "txt_para25", "", 0);
    PaneMatSetTevColorsByName((nw4r::lyt::Layout*)mpLayout, "txt_para01", &itemBoxInfo_slotLabelTop, &itemBoxInfo_slotLabelBottom);
    PaneMatSetTevColorsByName((nw4r::lyt::Layout*)mpLayout, "txt_para10", &itemBoxInfo_slotLabelTop, &itemBoxInfo_slotLabelBottom);
    PaneMatSetTevColorsByName((nw4r::lyt::Layout*)mpLayout, "txt_para11", &itemBoxInfo_slotLabelTop, &itemBoxInfo_slotLabelBottom);
    PaneMatSetTevColorsByName((nw4r::lyt::Layout*)mpLayout, "txt_para04", &itemBoxInfo_slotLabelTop, &itemBoxInfo_slotLabelBottom);
    PaneMatSetTevColorsByName((nw4r::lyt::Layout*)mpLayout, "txt_para08", &itemBoxInfo_slotLabelTop, &itemBoxInfo_slotLabelBottom);
    PaneMatSetTevColorsByName((nw4r::lyt::Layout*)mpLayout, "txt_para22", (const GXColorS10*)&itemBoxInfo_slotValueTop, (const GXColorS10*)&itemBoxInfo_slotValueBottom);
    PaneMatSetTevColorsByName((nw4r::lyt::Layout*)mpLayout, "txt_para23", (const GXColorS10*)&itemBoxInfo_slotValueTop, (const GXColorS10*)&itemBoxInfo_slotValueBottom);
    u32 playerIdx = (arg4 >> 16) & 0xFF;
    // Hoisted halves: MWCC materializes these once (lis) and rebuilds each
    // constant at its use site with a single addi.
    u32 darkBase = 0x77770000;
    u32 tagHi = 0x74690000;
    for (u32 i = 0; (u8)i < (u8)code80135FDC_getByte_64077(); i++) {
        u8 flag = local.flags[0][(u8)i];
        u32 color = darkBase + 0x77ff;  // 0x777777ff
        if (flag != 0) color = 0xFFFFFFFF;
        sprintf(paneName, "pic_pc%02d", (u8)i + 1);
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
        sprintf(paneName, "pic_pcbs%02d", (u8)i + 1, args[0], args[1], args[2], args[3]);
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
                void* item = CItem_kindPtr(2, ((CItemBoxCharEntry*)member)->slotIds[5]);
                if (item != NULL && *(u32*)item != 0 && arg3 == item) {
                    sprintf(label, "pic_eq%02d", (u8)i + 1);
                    u32 tex;
                    if (slot == (u8)GetCollectedFlagByte(playerIdx)) {
                        tex = (u32)mArcResAcc->GetResource(tagHi + 0x6d67, "mf00_reg00_eq01.tpl", NULL);
                    } else {
                        tex = (u32)mArcResAcc->GetResource(tagHi + 0x6d67, "mf00_reg00_eq00.tpl", NULL);
                    }
                    if (tex == 0) {
                        tex = (u32)mArcResAcc->GetResource(tagHi + 0x6d67, "mf00_com00_dmy.tpl", NULL);
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

// Retail compiles this one helper in speed mode: it saves r31/r30/r29 with
// separate stw (each move scheduled between the saves) while every other
// function in this size-mode TU keeps the stmw/lmw block frame. Measured and
// ruled out as flag-level causes: 10 mw_versions (all give stmw under -O4,s),
// whole-TU -O4,p (34/110) and -O4,s + -inline auto/all, -use_lmw_stmw off
// (38/110, and it still does not produce retail's 0x78 frame).
#pragma push
#pragma optimize_for_size off
void setItemBoxVtxColor(void* arr, u32 index, const void* color) {
    if (index >= 3) {
        nw4r::db::Panic(lbl_eu_8052E590, 0x8f, lbl_eu_8052E568);
    }
    __as__11_GXColorS10FRC11_GXColorS10((u8*)arr + index * 8 + 0x10, color);
}
#pragma pop
extern "C" void __as__11_GXColorS10FRC11_GXColorS10(void* dst, const void* src) {
    s16* d = (s16*)dst;
    const s16* s = (const s16*)src;
    d[0] = s[0];
    d[1] = s[1];
    d[2] = s[2];
    d[3] = s[3];
}



// Retail renderItemBoxDetail: item-box detail renderer. Builds a per-slot comparison
// record for the candidate item (buildItemBoxSlotFlags), lays out the label/stat
// panes, then per-slot colours the row panes and stamps the item name onto
// the slot whose record matches the candidate.
void CItemBoxInfo::RenderDetail(u32 itemId, void* record, u32 arg4) {
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
    LayoutSetTextBoxFmtValue((nw4r::lyt::Layout*)mpLayout, "txt_value02", "", 0);
    LayoutSetTextBoxFmtValue((nw4r::lyt::Layout*)mpLayout, "txt_value04", "", 0);
    LayoutSetTextBoxFmtValue((nw4r::lyt::Layout*)mpLayout, "txt_value07", "", 0);
    setLayoutTextBoxNumber((nw4r::lyt::Layout*)mpLayout, "txt_value08", (u8)flags.v[0]);
    setLayoutTextBoxNumber((nw4r::lyt::Layout*)mpLayout, "txt_value09", (u8)flags.v[1]);
    setLayoutTextBoxNumber((nw4r::lyt::Layout*)mpLayout, "txt_value22", (u8)flags.v[2]);
    u32 v1 = BdatGetU8Direct((const void*)::cf::CfBdat::spItmEquipListFileData, "arm_type", (u16)BdatGetItemId(itemId));
    char* s1;
    switch ((u8)v1) {
        case 3: s1 = BdatTouchStringCell("MNU_item", "name", 0x32); break;
        case 2: s1 = BdatTouchStringCell("MNU_item", "name", 0x31); break;
        case 1: s1 = BdatTouchStringCell("MNU_item", "name", 0x30); break;
        case 4: case 5: case 6: case 7: case 8:
        case 9: case 10: case 11: case 12: case 13:
            s1 = BdatTouchStringCell("MNU_item", "name", 0x2e); break;
        default: s1 = 0; break;
    }
    LayoutSetTextBoxFmtValue((nw4r::lyt::Layout*)mpLayout, "txt_para24", s1, 0);
    char* s2;
    switch ((u8)v1) {
        case 4: s2 = BdatTouchStringCell("pc_arts", "name", 0x77); break;
        case 5: s2 = BdatTouchStringCell("pc_arts", "name", 0x78); break;
        case 6: s2 = BdatTouchStringCell("pc_arts", "name", 0x79); break;
        case 7: s2 = BdatTouchStringCell("pc_arts", "name", 0x7a); break;
        case 8: s2 = BdatTouchStringCell("pc_arts", "name", 0x7b); break;
        case 9: s2 = BdatTouchStringCell("pc_arts", "name", 0x7c); break;
        case 10: s2 = BdatTouchStringCell("pc_arts", "name", 0x7d); break;
        case 11: s2 = BdatTouchStringCell("pc_arts", "name", 0x7e); break;
        case 12: s2 = BdatTouchStringCell("pc_arts", "name", 0x7f); break;
    }
    LayoutSetTextBoxFmtValue((nw4r::lyt::Layout*)mpLayout, "txt_para25", s2, 0);
    PaneMatSetTevColorsByName((nw4r::lyt::Layout*)mpLayout, "txt_para01", (const GXColorS10*)&itemBoxInfo_slotValueTop, (const GXColorS10*)&itemBoxInfo_slotValueBottom);
    PaneMatSetTevColorsByName((nw4r::lyt::Layout*)mpLayout, "txt_para10", &itemBoxInfo_slotLabelTop, &itemBoxInfo_slotLabelBottom);
    PaneMatSetTevColorsByName((nw4r::lyt::Layout*)mpLayout, "txt_para11", &itemBoxInfo_slotLabelTop, &itemBoxInfo_slotLabelBottom);
    PaneMatSetTevColorsByName((nw4r::lyt::Layout*)mpLayout, "txt_para04", (const GXColorS10*)&itemBoxInfo_slotValueTop, (const GXColorS10*)&itemBoxInfo_slotValueBottom);
    PaneMatSetTevColorsByName((nw4r::lyt::Layout*)mpLayout, "txt_para08", (const GXColorS10*)&itemBoxInfo_slotValueTop, (const GXColorS10*)&itemBoxInfo_slotValueBottom);
    PaneMatSetTevColorsByName((nw4r::lyt::Layout*)mpLayout, "txt_para22", &itemBoxInfo_slotLabelTop, &itemBoxInfo_slotLabelBottom);
    PaneMatSetTevColorsByName((nw4r::lyt::Layout*)mpLayout, "txt_para23", &itemBoxInfo_slotLabelTop, &itemBoxInfo_slotLabelBottom);
    u32 cur = (arg4 >> 16) & 0xFF;
    u32 tag = 0x74696D67;
    for (u32 i = 0; (u8)i < (u8)code80135FDC_getByte_64077(); i++) {
        u8 flag = flags.equipable[(u8)i];
        u32 color = 0x777777FF;
        if (flag != 0) color = 0xFFFFFFFF;
        u32 idx = (u8)i + 1;
        sprintf(buf, (char*)"pic_pc%02d", idx);
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
        sprintf(buf2, "pic_pcbs%02d", idx);
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
            case 4: value = ((CItemBoxCharEntry*)lookup)->slotIds[0]; break;
            case 5: value = ((CItemBoxCharEntry*)lookup)->slotIds[1]; break;
            case 6: value = ((CItemBoxCharEntry*)lookup)->slotIds[2]; break;
            case 7: value = ((CItemBoxCharEntry*)lookup)->slotIds[3]; break;
            case 8: value = ((CItemBoxCharEntry*)lookup)->slotIds[4]; break;
        }
        void* r = CItem_kindPtr(cat & 0xFFFF, value);
        if (r == NULL || *(u32*)r == 0 || r != record) continue;
        sprintf(buf2, "pic_eq%02d", idx);
        int curSlot = GetCollectedFlagByte(cur);
        u32 tex;
        if ((u8)slot == (u8)curSlot) {
            tex = (u32)mArcResAcc->GetResource(tag, "mf00_reg00_eq01.tpl", NULL);
        } else {
            tex = (u32)mArcResAcc->GetResource(tag, "mf00_reg00_eq00.tpl", NULL);
        }
        if (tex == 0) {
            tex = (u32)mArcResAcc->GetResource(tag, "mf00_com00_dmy.tpl", NULL);
        }
        if (tex != 0) {
            PaneSetTexPaletteByName((nw4r::lyt::Layout*)mpLayout, (const char*)buf2, (TPLPalette*)tex);
        }
    }
    RenderTripleRows(itemId, record);
}
// Retail renderItemBoxTripleRows: item-box slot renderer. With a candidate record it
// walks the three slot panes showing per-slot item info (regular items via the item
// table, equipped gear via the sub-record); with no record it shows the plain
// item-name list. Ends by stamping the per-slot data into info.
void CItemBoxInfo::RenderTripleRows(u32 itemId, void* record) {
    ml::FixStr<32> text(true);
    ClearTripleRows();
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
            sprintf(buf, "pic_ethcol%02d", idx);
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
                        if (_e == 0) tex = (u32)mTexArcResAcc->GetResource(tag, "mf00_reg30_crys00.tpl", NULL);
                        else if (_e == 4) tex = (u32)mTexArcResAcc->GetResource(tag, "mf00_reg30_crys01.tpl", NULL);
                        else if (_e == 5) tex = (u32)mTexArcResAcc->GetResource(tag, "mf00_reg30_crys02.tpl", NULL);
                        else if (_e == 6) tex = (u32)mTexArcResAcc->GetResource(tag, "mf00_reg30_crys03.tpl", NULL);
                        else if (_e == 7) tex = (u32)mTexArcResAcc->GetResource(tag, "mf00_reg30_crys04.tpl", NULL);
                        else if (_e == 8) tex = (u32)mTexArcResAcc->GetResource(tag, "mf00_reg30_crys05.tpl", NULL);
                        else if (_e == 9) tex = (u32)mTexArcResAcc->GetResource(tag, "mf00_reg30_crys06.tpl", NULL);
                    }
                    if (rec2b.name[0] == '0') {
                        sprintf(buf, "txt_ethvalue%02d", idx);
                        LayoutSetTextBoxFmtValue(mpLayout, buf, "", 0);
                    } else {
                        if ((u8)(rec2b.e0 - 3) <= 1) {
                            text.format("%s%s", rec2b.name, BdatTouchStringCell("MNU_item", "name", 0x21));
                        } else if (rec2b.e0 == 0) {
                            text.format("%s ", rec2b.name);
                        } else {
                            text.format("%s ", rec2b.name);
                        }
                        sprintf(buf, "txt_ethvalue%02d", idx);
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
                        tex = (u32)mTexArcResAcc->GetResource(tag, "mf00_reg30_crys00.tpl", NULL);
                        text.format("%s", BdatTouchStringCell("MNU_item", "name", 0x2a));
                    } else {
                        u16 equip = (u16)((sub->field_04 >> 16) & 0xFFF);
                        u32 w = sub->_00;
                        char* label = BdatTouchStringCell("MNU_item", "name", 0x1e - ((w >> 22 & 7) - 1));
                        u32 st = BdatGetU8Direct((const void*)::cf::CfBdat::spBtlSkillListFileData, "atr_type", equip);
                        {
                            // (was switch: retail compiles this dispatch as compares)
                            u32 _e = (u32)(st & 0xFF);
                            if (_e == 0) tex = (u32)mTexArcResAcc->GetResource(tag, "mf00_reg30_crys00.tpl", NULL);
                            else if (_e == 4) tex = (u32)mTexArcResAcc->GetResource(tag, "mf00_reg30_crys01.tpl", NULL);
                            else if (_e == 5) tex = (u32)mTexArcResAcc->GetResource(tag, "mf00_reg30_crys02.tpl", NULL);
                            else if (_e == 6) tex = (u32)mTexArcResAcc->GetResource(tag, "mf00_reg30_crys03.tpl", NULL);
                            else if (_e == 7) tex = (u32)mTexArcResAcc->GetResource(tag, "mf00_reg30_crys04.tpl", NULL);
                            else if (_e == 8) tex = (u32)mTexArcResAcc->GetResource(tag, "mf00_reg30_crys05.tpl", NULL);
                            else if (_e == 9) tex = (u32)mTexArcResAcc->GetResource(tag, "mf00_reg30_crys06.tpl", NULL);
                        }
                        s16 val = (s16)((w >> 11) & 0x7FF);
                        if (val != 0) {
                            u32 v2 = BdatGetU8Direct((const void*)::cf::CfBdat::spBtlSkillListFileData, "rvs_type", equip);
                            if (v2 != 0 && (u8)(v2 - 3) <= 1) {
                                text.format("%d%s", val, BdatTouchStringCell("MNU_item", "name", 0x21));
                            } else {
                                text.format("%d ", val);
                            }
                            sprintf(buf, "txt_ethvalue%02d", idx);
                            LayoutSetTextBoxFmtValue(mpLayout, buf, text.c_str(), 0);
                        } else {
                            sprintf(buf, "txt_ethvalue%02d", idx);
                            LayoutSetTextBoxFmtValue(mpLayout, buf, "", 0);
                        }
                        char* s = BdatGetPtrDirect((const void*)::cf::CfBdat::spBtlSkillListFileData, "name", equip);
                        text.format("%s%s", s, label);
                        itemCount = (u8)((w >> 22) & 7);
                        value = (s16)((w >> 11) & 0x7FF);
                    }
                }
                sprintf(buf, "txt_eth%02d", idx);
                LayoutSetTextBoxFmtValue(mpLayout, buf, text.c_str(), 0);
                if (tex != 0) PaneSetTexPalette(pane, (TPLPalette*)tex);
                if (((u8*)this)[0x9A] != 4) {
                    nw4r::lyt::Pane* pane2 = mpLayout->GetRootPane()->FindPaneByName("nul_proportion", true);
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
        u8 sel = (u8)BdatGetU8Direct((const void*)obj, "jwl_slot", (u16)cat);
        u32 tag = 0x74696D67;
        for (u32 i = 0; i < 3; i++) {
            u32 idx = (u8)i + 1;
            sprintf(buf2, "pic_ethcol%02d", idx);
            nw4r::lyt::Pane* pane = mpLayout->GetRootPane()->FindPaneByName(buf2, true);
            if ((u8)i < sel) {
                setPaneVisible(pane, 1);
                u32 itemVal = 0;
                u32 itemCount = 0;
                u8 val = 0;
                u32 tex = 0;
                text.format("jwl_skill%d", idx);
                u16 nameId = (u16)BdatGetU16Direct((void*)obj, text.c_str(), (u16)cat);
                if (nameId == 0) {
                    tex = (u32)mTexArcResAcc->GetResource(tag, "mf00_reg30_crys00.tpl", NULL);
                    text.format("%s", BdatTouchStringCell("MNU_item", "name", 0x2a));
                } else {
                    CItemBoxNameRecord rec3;
                    rec3 = BuildNameRec((void*)nameId, 0);
                    CItemBoxNameRecord rec3b = rec3;
                    {
                        // (was switch: retail compiles this dispatch as compares)
                        u32 _e = (u32)(rec3b.e1);
                        if (_e == 0) tex = (u32)mTexArcResAcc->GetResource(tag, "mf00_reg30_crys00.tpl", NULL);
                        else if (_e == 4) tex = (u32)mTexArcResAcc->GetResource(tag, "mf00_reg30_crys01.tpl", NULL);
                        else if (_e == 5) tex = (u32)mTexArcResAcc->GetResource(tag, "mf00_reg30_crys02.tpl", NULL);
                        else if (_e == 6) tex = (u32)mTexArcResAcc->GetResource(tag, "mf00_reg30_crys03.tpl", NULL);
                        else if (_e == 7) tex = (u32)mTexArcResAcc->GetResource(tag, "mf00_reg30_crys04.tpl", NULL);
                        else if (_e == 8) tex = (u32)mTexArcResAcc->GetResource(tag, "mf00_reg30_crys05.tpl", NULL);
                        else if (_e == 9) tex = (u32)mTexArcResAcc->GetResource(tag, "mf00_reg30_crys06.tpl", NULL);
                    }
                    if (rec3b.name[0] == '0') {
                        sprintf(buf2, "txt_ethvalue%02d", idx);
                        LayoutSetTextBoxFmtValue(mpLayout, buf2, "", 0);
                    } else {
                        if (rec3b.e0 == 0) {
                            text.format("%s ", rec3b.name);
                        } else if ((u8)(rec3b.e0 - 3) <= 1) {
                            text.format("%s%s", rec3b.name, BdatTouchStringCell("MNU_item", "name", 0x21));
                        } else {
                            text.format("%s ", rec3b.name);
                        }
                        sprintf(buf2, "txt_ethvalue%02d", idx);
                        LayoutSetTextBoxFmtValue(mpLayout, buf2, text.c_str(), 0);
                    }
                    char* s = BuildItemNameStr(nameId);
                    text.format("%s%s", rec3b.str, s);
                    itemVal = BdatGetItemId(nameId);
                    itemCount = BdatGetU8Direct((const void*)::cf::CfBdat::spItmItemListFileData, "rankType", nameId);
                    val = (u8)BdatGetU8Direct((const void*)::cf::CfBdat::spItmItemListFileData, "percent", nameId);
                }
                sprintf(buf2, "txt_eth%02d", idx);
                LayoutSetTextBoxFmtValue(mpLayout, buf2, text.c_str(), 0);
                if (tex != 0) PaneSetTexPalette(pane, (TPLPalette*)tex);
                if (((u8*)this)[0x9A] != 4) {
                    nw4r::lyt::Pane* pane2 = mpLayout->GetRootPane()->FindPaneByName("nul_proportion", true);
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
}

void CItemBoxPage::SetValue(unsigned char index, short value) {
    if (index >= 12) return;
    mValues[index] = value;
}

void CItemBoxInfo::RefreshSlotRows(u16 arg2) {
    ApplySelColors();
    ResetRowPanes();
    char buf[0x20];
    // 9-byte per-slot selection record: sel[1..8] are per-slot flags, flag2
    // also selects the row-height label size.
    CItemBoxSelRec local = BuildSelRec(arg2);
    u32 rowHeight = 0x2c;
    if (local.flag2 != 0) rowHeight = 0x2b;
    char* label = BdatTouchStringCell("MNU_item", "name", rowHeight);
    LayoutSetTextBoxFmtValue((nw4r::lyt::Layout*)mpLayout, "txt_para80", label, 0);
    for (u32 i = 0; (u8)i < (u8)code80135FDC_getByte_64077(); i++) {
        u8 flag = local.selected[(u8)i];
        u32 color = 0x777777ff;
        if (flag) color = 0xFFFFFFFF;
        sprintf(buf, "pic_pc%02d", (u8)i + 1);
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
        sprintf(buf, "pic_pcbs%02d", (u8)i + 1);
        nw4r::lyt::Pane* pane = mpLayout->GetRootPane()->FindPaneByName(buf, true);
        if (pane != NULL) {
            nw4r::lyt::Material* material = pane->GetMaterial();
            for (u32 j = 0; j < 2; j++) {
                setItemBoxVtxColor(material, (u8)j, &colors[(u8)j]);
            }
        }
    }
}
// Retail renderItemBoxKind3 (kind-3 detail renderer): rebuild the detail record
// (buildItemBoxNameRec), print the description/caption text, position the cursor
// row from the layout panes, stash the equip summary at info+0xB0.., then per
// player slot recolor the row panes and stamp the owning player's name onto
// equipment entries that resolve back to the selected item.
void CItemBoxInfo::RenderKind3(u16 arg2, void* arg3, u32 arg4) {
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


    LayoutSetTextBoxFmtValue((nw4r::lyt::Layout*)mpLayout, "txt_value50", (char*)rec.str, 0);

    caption.mString[0] = 0;
    caption.mLength = 0;
    void* item = NULL;
    if (arg3 != NULL) {
        item = arg3;
    }
    if (rec.name[0] == 0x30) {
        LayoutSetTextBoxFmtValue((nw4r::lyt::Layout*)mpLayout, "txt_value51", "", 0);
    } else {
        if (rec.e0 != 0) {
            // Flag values 3/4 pick a count-formatted caption label.
            if ((u32)(rec.e0 - 3) <= 1) {
                char* label = BdatTouchStringCell("MNU_item", "name", 0x21);
                caption.format("%s%s", &rec.name[0], label);
            } else {
                caption.format("%s ", &rec.name[0]);
            }
        } else {
            caption.format("%s ", &rec.name[0]);
        }
        LayoutSetTextBoxFmtValue((nw4r::lyt::Layout*)mpLayout, "txt_value51", caption.mString, 0);
    }

    // Ternaries kept inline at their use sites so the results stay in the
    // ABI return register instead of living across the whole state!=4 block.
    LayoutSetTextBoxFmtValue((nw4r::lyt::Layout*)mpLayout, "txt_para51",
                  (arg3 != NULL)
                      ? CItem_initItemImplInstances((CItemData*)item)->vf20((CItemData*)item)
                      : BuildItemNameStr(arg2),
                  0);

    if (mKind != 4) {
        nw4r::lyt::Pane* pane =
            mpLayout->GetRootPane()->FindPaneByName("txt_para51", true);
        nw4r::lyt::Pane* pane2 =
            mpLayout->GetRootPane()->FindPaneByName("nul_proportion", true);
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
    u8 rows = (u8)BdatGetU8Direct((const void*)((u32)(&::cf::CfBdat::spBtlSkillListFileData)[cat2]), "attach", cat2);
    LayoutSetTextBoxFmtValue((nw4r::lyt::Layout*)mpLayout, "txt_para52",
                  BdatTouchStringCell("MNU_item", "name", rows + 0x15), 0);

    // The slot-kind table initializer is retained by retail; keep the real
    // local value in the same source position.
    CItemBoxTagStage stage = { { 2, 4, 5, 6, 7, 8 } };
    u8 playerByte = (u8)(arg4 >> 16);

    for (u32 i = 0; (u32)(u8)i < (u32)(u8)code80135FDC_getByte_64077(); i++) {
        u32 idx = (u8)i + 1;
        sprintf(caption.mString, "pic_pc%02d", idx);
        PaneSetVtxColorByName((nw4r::lyt::Layout*)mpLayout, caption.mString, -1);
        cols.w[0] = 0;
        cols.w[1] = 0;
        cols.w[2] = 0;
        cols.w[3] = 0;
        cols.w[0] = mColorA.w[0];
        cols.w[1] = mColorA.w[1];
        cols.w[2] = mColorB.w[0];
        cols.w[3] = mColorB.w[1];
        sprintf(caption.mString, "pic_pcbs%02d", idx);
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
        cur.tbl = ((CItemBoxCharEntry*)lookup)->mBdatTable;
        cur.h = ((CItemBoxCharEntry*)lookup)->mBdatRow;
        vals.v26 = ((CItemBoxCharEntry*)lookup)->slotIds[5];
        vals.v1C = ((CItemBoxCharEntry*)lookup)->slotIds[0];
        vals.v1E = ((CItemBoxCharEntry*)lookup)->slotIds[1];
        vals.v20 = ((CItemBoxCharEntry*)lookup)->slotIds[2];
        vals.v22 = ((CItemBoxCharEntry*)lookup)->slotIds[3];
        vals.v24 = ((CItemBoxCharEntry*)lookup)->slotIds[4];
        for (u32 k = 0; k < 6; k++) {
            void* hit = CItem_kindPtr(((u8*)&cur)[k], vals.s[k]);
            if (hit == NULL || *(u32*)hit == 0) continue;
            u8 n = CItem_initItemImplInstances((CItemData*)hit)->vf30((CItemData*)hit);
            for (u32 m = 0; (u32)(u8)m < (u32)n; m++) {
                s16 v40 = CItem_initItemImplInstances((CItemData*)hit)->vf40((CItemData*)hit, m);
                void* rec3 = CItem_kindPtr(3, v40);
                if (rec3 == NULL || *(u32*)rec3 == 0 || rec3 != arg3) continue;
                sprintf(caption.mString, "pic_eq%02d", idx);
                u8 owner = GetCollectedFlagByte((u32)playerByte);
                u32 name;
                if (flag == owner) {
                    name = (u32)mArcResAcc->GetResource(0x74696d67, "mf00_reg00_eq01.tpl", NULL);
                } else {
                    name = (u32)mArcResAcc->GetResource(0x74696d67, "mf00_reg00_eq00.tpl", NULL);
                }
                if (name == 0) {
                    name = (u32)mArcResAcc->GetResource(0x74696d67, "mf00_com00_dmy.tpl", NULL);
                }
                if (name != 0) {
                    PaneSetTexPaletteByName((nw4r::lyt::Layout*)mpLayout, (const char*)caption.mString, (TPLPalette*)name);
                }
            }
        }
    }
}

void CItemBoxInfo::RenderLabels(u32 arg2) {
    ClearPairTexts();
    ResetRowPanes();
    CItemBoxLabelRec rec = GetLabelRec(arg2);
    LayoutSetTextBoxFmtValue((nw4r::lyt::Layout*)mpLayout, "txt_value60", rec.b, 0);
    LayoutSetTextBoxFmtValue((nw4r::lyt::Layout*)mpLayout, "txt_value61", rec.d, 0);
}

// Retail renderItemBoxSlotList: item-box slot-list renderer. Rebuilds the per-slot
// record (buildItemBoxSlotRec), stamps the header pane with the formatted item
// string, then per filled pair sets the row-name pane text and (unless mode
// 0x9A==4) measures the row position and caches value/count/pos per slot.
void CItemBoxInfo::RenderSlotList(u16 arg2, void* arg3) {
    // Frame layout (descending): cur > text > call temp > buf > pos > tmp.
    // The 9 constant claims r31 first (Rule A).
    u8 slotState = 9;
    char buf[0x20];
    nw4r::math::_VEC3 pos;
    ClearSlotTexts();
    ResetRowPanes();
    CItemBoxSlotRecord1 cur = BuildSlotRec(arg2, arg3);
    ml::FixStr<32> text(false);
    LayoutSetTextBoxFmtValue((nw4r::lyt::Layout*)mpLayout, "txt_value30", (char*)cur.str, 0);
    u8 count = cur.counter;
    for (u8 i = 0; (u8)i < count; i++) {
        sprintf(buf, "txt_para%02d", (u32)((u8)i * 2) + 0x1f);
        LayoutSetTextBoxFmtValue((nw4r::lyt::Layout*)mpLayout, buf, (char*)cur.text[(u8)i], 0);
        if (((u8*)this)[0x9A] != 4) {
            nw4r::lyt::Pane* pane =
                mpLayout->GetRootPane()->FindPaneByName(buf, true);
            nw4r::lyt::Pane* pane2 =
                mpLayout->GetRootPane()->FindPaneByName("nul_proportion", true);
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
        sprintf(buf, "txt_value%02d", (u32)(u8)i + 0x1f);
        text.format("%d%s", cur.vals[(u8)i],
                    BdatTouchStringCell("MNU_item", "name", 0x21));
        LayoutSetTextBoxFmtValue((nw4r::lyt::Layout*)mpLayout, buf, text.c_str(), 0);
    }
}
void CItemBoxInfo::SetupSlotPanes() {
    // No local base pointer: MWCC keeps the repeated symbol offsets in one
    // register, splitting the address into a pre-stmw lis plus a post-stmw
    // callee-saved addi exactly like retail.
    char buf[0x20];
    u32 i;
    LayoutSetTextBoxFmtValue((nw4r::lyt::Layout*)mpLayout, "txt_value02", "", 0);
    LayoutSetTextBoxFmtValue((nw4r::lyt::Layout*)mpLayout, "txt_value08", "", 0);
    LayoutSetTextBoxFmtValue((nw4r::lyt::Layout*)mpLayout, "txt_value09", "", 0);
    LayoutSetTextBoxFmtValue((nw4r::lyt::Layout*)mpLayout, "txt_value04", "", 0);
    LayoutSetTextBoxFmtValue((nw4r::lyt::Layout*)mpLayout, "txt_value07", "", 0);
    LayoutSetTextBoxFmtValue((nw4r::lyt::Layout*)mpLayout, "txt_value22", "", 0);
    LayoutSetTextBoxFmtValue((nw4r::lyt::Layout*)mpLayout, "txt_para24", "", 0);
    LayoutSetTextBoxFmtValue((nw4r::lyt::Layout*)mpLayout, "txt_para25", "", 0);
    PaneMatSetTevColorsByName((nw4r::lyt::Layout*)mpLayout, "txt_para01", (const GXColorS10*)&itemBoxInfo_slotValueTop, (const GXColorS10*)&itemBoxInfo_slotValueBottom);
    PaneMatSetTevColorsByName((nw4r::lyt::Layout*)mpLayout, "txt_para10", (const GXColorS10*)&itemBoxInfo_slotValueTop, (const GXColorS10*)&itemBoxInfo_slotValueBottom);
    PaneMatSetTevColorsByName((nw4r::lyt::Layout*)mpLayout, "txt_para11", (const GXColorS10*)&itemBoxInfo_slotValueTop, (const GXColorS10*)&itemBoxInfo_slotValueBottom);
    PaneMatSetTevColorsByName((nw4r::lyt::Layout*)mpLayout, "txt_para04", (const GXColorS10*)&itemBoxInfo_slotValueTop, (const GXColorS10*)&itemBoxInfo_slotValueBottom);
    PaneMatSetTevColorsByName((nw4r::lyt::Layout*)mpLayout, "txt_para08", (const GXColorS10*)&itemBoxInfo_slotValueTop, (const GXColorS10*)&itemBoxInfo_slotValueBottom);
    PaneMatSetTevColorsByName((nw4r::lyt::Layout*)mpLayout, "txt_para22", (const GXColorS10*)&itemBoxInfo_slotValueTop, (const GXColorS10*)&itemBoxInfo_slotValueBottom);
    PaneMatSetTevColorsByName((nw4r::lyt::Layout*)mpLayout, "txt_para23", (const GXColorS10*)&itemBoxInfo_slotValueTop, (const GXColorS10*)&itemBoxInfo_slotValueBottom);
    for (i = 0; (u8)i < (u8)code80135FDC_getByte_64077(); i++) {
        u32 idx = (u8)i + 1;
        sprintf(buf, "pic_pc%02d", idx);
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
        sprintf(buf, "pic_pcbs%02d", idx);
        nw4r::lyt::Pane* pane = mpLayout->GetRootPane()->FindPaneByName(buf, true);
        if (pane != NULL) {
            nw4r::lyt::Material* material = pane->GetMaterial();
            for (u32 j = 0; j < 2; j++) {
                setItemBoxVtxColor(material, (u8)j, (u8*)args + (u8)j * 8);
            }
        }
        // Item-name lookup on the shared arc resource accessor (vtable+0x0C),
        // then push the texture name onto the slot pane.
        sprintf(buf, "pic_eq%02d", idx);
        u32 tex = (u32)mArcResAcc->GetResource(0x74696D67, "mf00_com00_dmy.tpl", NULL);
        if (tex != 0) {
            PaneSetTexPaletteByName((nw4r::lyt::Layout*)mpLayout, (const char*)buf, (TPLPalette*)tex);
        }
    }
}
void CItemBoxInfo::ResetRowPanes() {
    u32 i;
    u32 idx;
    u32 tag;
    u32 white;
    LayoutSetTextBoxFmtValue((nw4r::lyt::Layout*)mpLayout, "txt_value08", "", 0);
    LayoutSetTextBoxFmtValue((nw4r::lyt::Layout*)mpLayout, "txt_value09", "", 0);
    LayoutSetTextBoxFmtValue((nw4r::lyt::Layout*)mpLayout, "txt_value22", "", 0);
    LayoutSetTextBoxFmtValue((nw4r::lyt::Layout*)mpLayout, "txt_para24", "", 0);
    LayoutSetTextBoxFmtValue((nw4r::lyt::Layout*)mpLayout, "txt_para25", "", 0);
    PaneMatSetTevColorsByName((nw4r::lyt::Layout*)mpLayout, "txt_para01", (const GXColorS10*)&itemBoxInfo_slotValueTop, (const GXColorS10*)&itemBoxInfo_slotValueBottom);
    PaneMatSetTevColorsByName((nw4r::lyt::Layout*)mpLayout, "txt_para10", (const GXColorS10*)&itemBoxInfo_slotValueTop, (const GXColorS10*)&itemBoxInfo_slotValueBottom);
    PaneMatSetTevColorsByName((nw4r::lyt::Layout*)mpLayout, "txt_para11", (const GXColorS10*)&itemBoxInfo_slotValueTop, (const GXColorS10*)&itemBoxInfo_slotValueBottom);
    PaneMatSetTevColorsByName((nw4r::lyt::Layout*)mpLayout, "txt_para04", (const GXColorS10*)&itemBoxInfo_slotValueTop, (const GXColorS10*)&itemBoxInfo_slotValueBottom);
    PaneMatSetTevColorsByName((nw4r::lyt::Layout*)mpLayout, "txt_para08", (const GXColorS10*)&itemBoxInfo_slotValueTop, (const GXColorS10*)&itemBoxInfo_slotValueBottom);
    PaneMatSetTevColorsByName((nw4r::lyt::Layout*)mpLayout, "txt_para22", (const GXColorS10*)&itemBoxInfo_slotValueTop, (const GXColorS10*)&itemBoxInfo_slotValueBottom);
    PaneMatSetTevColorsByName((nw4r::lyt::Layout*)mpLayout, "txt_para23", (const GXColorS10*)&itemBoxInfo_slotValueTop, (const GXColorS10*)&itemBoxInfo_slotValueBottom);
    char buf[0x20];
    tag = 0x74696D67u;
    white = 0x777777FFu;
    for (i = 0; (u8)i < (u8)code80135FDC_getByte_64077(); i++) {
        idx = (u32)((u8)i + 1);
        sprintf(buf, (char*)"pic_pc%02d", idx);
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
        sprintf(buf, (char*)"pic_pcbs%02d", idx);
        nw4r::lyt::Pane* pane = mpLayout->GetRootPane()->FindPaneByName(buf, true);
        if (pane != NULL) {
            nw4r::lyt::Material* material = pane->GetMaterial();
            for (u32 j = 0; j < 2; j++) {
                setItemBoxVtxColor(material, (u8)j, (u8*)args + (u8)j * 8);
            }
        }
        // Item-name lookup on the shared arc resource accessor (vtable+0x0C),
        // then push the texture name onto the slot pane.
        sprintf(buf, "pic_eq%02d", idx);
        u32 tex = (u32)mArcResAcc->GetResource(tag, "mf00_com00_dmy.tpl", NULL);
        if (tex != 0) {
            PaneSetTexPaletteByName((nw4r::lyt::Layout*)mpLayout, (const char*)buf, (TPLPalette*)tex);
        }
    }
}

void CItemBoxInfo::ClearTripleRows() {
    char buf[0x20];
    // Declaration order fixes the r29/r30 coloring: retail claims index (r30)
    // before the loop counter i (r29) - saved regs go high->low by birth order.
    u32 index;
    u32 i;
    for (i = 0; i < 3; i++) {
        index = (u8)i + 1;
        sprintf(buf, "pic_ethcol%02d", index);
        nw4r::lyt::Pane* r = mpLayout->GetRootPane()->FindPaneByName(buf, true);
        setPaneVisible(r, 0);
        sprintf(buf, "txt_ethvalue%02d", index);
        LayoutSetTextBoxFmtValue((nw4r::lyt::Layout*)mpLayout, buf, "", 0);
        sprintf(buf, "txt_eth%02d", index);
        LayoutSetTextBoxFmtValue((nw4r::lyt::Layout*)mpLayout, buf, "", 0);
    }
}
// Retail applyItemBoxSelColors: label every slot pane, then build two 8-byte
// selection colours (from the s16s at +0xA2/+0xAA), format the pane name,
// look the pane up, and push both colours into its
// vertex buffer via setItemBoxVtxColor. layout is reloaded per use (lwz 0x34).
void CItemBoxInfo::ApplySelColors() {
    LayoutSetTextBoxFmtValue((nw4r::lyt::Layout*)mpLayout, "txt_para80", "", 0);
    char buf[0x20];
    u32 idx;
    // Retail re-evaluates the slot count on every iteration (bl in the
    // loop condition), so do not hoist the call.
    for (u32 i = 0; (u8)i < (u8)code80135FDC_getByte_64077(); i++) {
        idx = (u32)((u8)i + 1);
        sprintf(buf, "pic_pc%02d", idx);
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
        sprintf(buf, "pic_pcbs%02d", idx);
        nw4r::lyt::Pane* pane = mpLayout->GetRootPane()->FindPaneByName(buf, true);
        if (pane != NULL) {
            void* obj = pane->GetMaterial();
            for (u32 j = 0; j < 2; j++) {
                setItemBoxVtxColor(obj, (u8)j, (u8*)args + (u8)j * 8);
            }
        }
    }
}

void CItemBoxInfo::ClearQuadTexts() {
    LayoutSetTextBoxFmtValue((nw4r::lyt::Layout*)mpLayout,
                  "txt_value50",
                  "", 0);
    LayoutSetTextBoxFmtValue((nw4r::lyt::Layout*)mpLayout,
                  "txt_value51",
                  "", 0);
    LayoutSetTextBoxFmtValue((nw4r::lyt::Layout*)mpLayout,
                  "txt_para52",
                  "", 0);
    LayoutSetTextBoxFmtValue((nw4r::lyt::Layout*)mpLayout,
                  "txt_para51",
                  "", 0);
}

void CItemBoxInfo::ClearPairTexts() {
    LayoutSetTextBoxFmtValue((nw4r::lyt::Layout*)mpLayout,
                  "txt_value60",
                  "", 0);
    LayoutSetTextBoxFmtValue((nw4r::lyt::Layout*)mpLayout,
                  "txt_value61",
                  "", 0);
}

void CItemBoxInfo::ClearSlotTexts() {
    char buf[0x20];
    LayoutSetTextBoxFmtValue((nw4r::lyt::Layout*)mpLayout,
                  "txt_value30",
                  "", 0);
    for (u32 i = 0; i < 4; i++) {
        sprintf(buf, "txt_para%02d", (u8)i * 2 + 0x1f);
        LayoutSetTextBoxFmtValue((nw4r::lyt::Layout*)mpLayout, buf,
                      "", 0);
        sprintf(buf, "txt_value%02d", (u8)i + 0x1f);
        LayoutSetTextBoxFmtValue((nw4r::lyt::Layout*)mpLayout, buf,
                      "", 0);
    }
}

// Retail stampItemBoxSlotTex: per-slot item-box1 renderer. Loops 7 slots; for slots
// below the active count it resolves the item name via
// CUICfManager_getArcResourceAccessor/vtable[3] and colors the pane; slots at/above
// the count get a plain label pane. The two sprintf buffers stay separate locals
// (retail sp+0x48 / sp+0x28).
void CItemBoxInfo::StampSlotTex() {
    // Shape mirrors the proven stampItemBox2SlotTex (ItemBox2 twin): if/else instead
    // of continue-guards, hoisted i/idx, direct lbl refs, typed name lookup.
    u32 i;
    char bufElse[0x20];
    char buf[0x20];
    u8 count = code80135FDC_getByte_64077();
    u32 idx;
    for (i = 0; i < 7; i++) {
        if ((u8)i >= count) {
            sprintf(bufElse, "pic_pcbs%02d", (u8)i + 1);
            nw4r::lyt::Pane* pane = mpLayout->GetRootPane()->FindPaneByName(bufElse, true);
            setPaneVisible(pane, 0);
        } else {
            u8 slot = (u8)GetCollectedFlagByte((u8)i);
            u32 tag = 0x74696D67u;
            u32 nameId = (u16)BdatGetU16Direct(::cf::CfBdat::spBtlPcListFileData, "tag_icon", slot);
            u32 itemId = (u32)MakeTplNameSysFile(nameId);
            u32 found = (u32)CUICfManager_getArcResourceAccessor()->GetResource(tag, (const char*)itemId, NULL);
            if (found != 0) {
                idx = (u32)((u8)i + 1);
                sprintf(buf, "pic_pc%02d", idx);
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
                sprintf(buf, "pic_pcbs%02d", idx);
                nw4r::lyt::Pane* pane3 = mpLayout->GetRootPane()->FindPaneByName(buf, true);
                void* vertexObj = pane3->GetMaterial();
                for (u32 j = 0; j < 2; j++) {
                    setItemBoxVtxColor(vertexObj, (u8)j, (u8*)args + (u8)j * 8);
                }
            }
        }
    }
}
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
    D8EQuad labelTop = *(D8EQuad*)&itemBoxInfo_labelNeutralTop;
    D8EQuad labelBottom = *(D8EQuad*)&itemBoxInfo_labelNeutralBottom;
    D8EQuad valueTop = *(D8EQuad*)&itemBoxInfo_valueNeutralTop;
    D8EQuad valueBottom = *(D8EQuad*)&itemBoxInfo_valueNeutralBottom;
    if (delta < 0) {
        __as__11_GXColorS10FRC11_GXColorS10(&labelTop, &itemBoxInfo_labelDownTop);
        __as__11_GXColorS10FRC11_GXColorS10(&labelBottom, &itemBoxInfo_labelDownBottom);
        __as__11_GXColorS10FRC11_GXColorS10(&valueTop, &itemBoxInfo_valueDownTop);
        __as__11_GXColorS10FRC11_GXColorS10(&valueBottom, &itemBoxInfo_valueDownBottom);
    } else if (delta > 0) {
        __as__11_GXColorS10FRC11_GXColorS10(&labelTop, &itemBoxInfo_labelUpTop);
        __as__11_GXColorS10FRC11_GXColorS10(&labelBottom, &itemBoxInfo_labelUpBottom);
        __as__11_GXColorS10FRC11_GXColorS10(&valueTop, &itemBoxInfo_valueUpTop);
        __as__11_GXColorS10FRC11_GXColorS10(&valueBottom, &itemBoxInfo_valueUpBottom);
    }
    PaneMatSetTevColors((nw4r::lyt::Pane*)valuePane, (const GXColorS10*)&valueTop, (const GXColorS10*)&valueBottom);
    PaneMatSetTevColors((nw4r::lyt::Pane*)labelPane, (const GXColorS10*)&labelTop, (const GXColorS10*)&labelBottom);
}

static inline void applyItemBoxDeltaColorAt(CItemBoxInfo* info, u32 valueIndex,
                                             u32 labelIndex, s16 delta) {
    D8EQuad labelTop = *(D8EQuad*)&itemBoxInfo_labelNeutralTop;
    D8EQuad labelBottom = *(D8EQuad*)&itemBoxInfo_labelNeutralBottom;
    D8EQuad valueTop = *(D8EQuad*)&itemBoxInfo_valueNeutralTop;
    D8EQuad valueBottom = *(D8EQuad*)&itemBoxInfo_valueNeutralBottom;
    if (delta < 0) {
        __as__11_GXColorS10FRC11_GXColorS10(&labelTop, &itemBoxInfo_labelDownTop);
        __as__11_GXColorS10FRC11_GXColorS10(&labelBottom, &itemBoxInfo_labelDownBottom);
        __as__11_GXColorS10FRC11_GXColorS10(&valueTop, &itemBoxInfo_valueDownTop);
        __as__11_GXColorS10FRC11_GXColorS10(&valueBottom, &itemBoxInfo_valueDownBottom);
    } else if (delta > 0) {
        __as__11_GXColorS10FRC11_GXColorS10(&labelTop, &itemBoxInfo_labelUpTop);
        __as__11_GXColorS10FRC11_GXColorS10(&labelBottom, &itemBoxInfo_labelUpBottom);
        __as__11_GXColorS10FRC11_GXColorS10(&valueTop, &itemBoxInfo_valueUpTop);
        __as__11_GXColorS10FRC11_GXColorS10(&valueBottom, &itemBoxInfo_valueUpBottom);
    }
    nw4r::lyt::Pane** panes = &info->mSlotPane0;
    PaneMatSetTevColors(panes[valueIndex], (const GXColorS10*)&valueTop, (const GXColorS10*)&valueBottom);
    PaneMatSetTevColors(panes[labelIndex], (const GXColorS10*)&labelTop, (const GXColorS10*)&labelBottom);
}

static inline void applyItemBoxDeltaColorPair(void* firstValuePane, void* firstLabelPane,
                                               void* secondValuePane, void* secondLabelPane,
                                               s16 delta) {
    D8EQuad labelTop = *(D8EQuad*)&itemBoxInfo_labelNeutralTop;
    D8EQuad labelBottom = *(D8EQuad*)&itemBoxInfo_labelNeutralBottom;
    D8EQuad valueTop = *(D8EQuad*)&itemBoxInfo_valueNeutralTop;
    D8EQuad valueBottom = *(D8EQuad*)&itemBoxInfo_valueNeutralBottom;
    if (delta < 0) {
        __as__11_GXColorS10FRC11_GXColorS10(&labelTop, &itemBoxInfo_labelDownTop);
        __as__11_GXColorS10FRC11_GXColorS10(&labelBottom, &itemBoxInfo_labelDownBottom);
        __as__11_GXColorS10FRC11_GXColorS10(&valueTop, &itemBoxInfo_valueDownTop);
        __as__11_GXColorS10FRC11_GXColorS10(&valueBottom, &itemBoxInfo_valueDownBottom);
    } else if (delta > 0) {
        __as__11_GXColorS10FRC11_GXColorS10(&labelTop, &itemBoxInfo_labelUpTop);
        __as__11_GXColorS10FRC11_GXColorS10(&labelBottom, &itemBoxInfo_labelUpBottom);
        __as__11_GXColorS10FRC11_GXColorS10(&valueTop, &itemBoxInfo_valueUpTop);
        __as__11_GXColorS10FRC11_GXColorS10(&valueBottom, &itemBoxInfo_valueUpBottom);
    }
    PaneMatSetTevColors((nw4r::lyt::Pane*)firstValuePane, (const GXColorS10*)&valueTop, (const GXColorS10*)&valueBottom);
    PaneMatSetTevColors((nw4r::lyt::Pane*)firstLabelPane, (const GXColorS10*)&labelTop, (const GXColorS10*)&labelBottom);
    PaneMatSetTevColors((nw4r::lyt::Pane*)secondValuePane, (const GXColorS10*)&valueTop, (const GXColorS10*)&valueBottom);
    PaneMatSetTevColors((nw4r::lyt::Pane*)secondLabelPane, (const GXColorS10*)&labelTop, (const GXColorS10*)&labelBottom);
}

void CItemBoxInfo::Activate() {
    // Goto-gate (PLAN 17.6), same shape as activateItemBox2.
    if (mpLayout != 0 && mTexArcResAcc != 0) {
        goto body;
    }
    return;
body:
    mCurrent = 1;
    mActive = 1;
}

void copyItemBoxEntry(CItemBoxInfoEntry*, const CItemBoxInfoEntry*);
void setItemBoxEntry(CItemBoxInfoEntry*, u16, u32, u8);
extern const u32 itemBoxInfoEquippedTypeIds[6];

// Named views keep the item-stat calculations readable while leaving the
// backing entry as a plain 13-word record, which is how MWCC copies it.
#define w04 words[1]
#define _0C words[3]
#define _10 words[4]
#define w14 halfWords[10]

// Prefer labeled SDA symbols over float literals / parent+offset member decays
// so bar formulas emit retail lfs relocs (0.01f/100.0f).
void CItemBoxInfo::SetupDetail(u32 arg2, void* arg3, u32 arg4) {
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
    cf::CActorParam* stats = &((CItemBoxCharEntry*)charObj)->mParam;

    ml::FixStr<32> textBuffer(false);
    ml::FixStr<32> textBufferB(false);

    // ---- HP values (clamped to 9999) ----
    // String-pool base before HP so it takes r14 (retail) and HP lands in r18/r19.
    // Both virtuals before either clamp -- retail interleaves the two
    // fctiwz conversions around the second vcall, not the first clamp.
    s32 hp1 = (s32)stats->CActorParam_getHp();
    s32 hp2 = (s32)stats->CActorParam_getDamageScale();
    if (hp1 > 9999) hp1 = 9999;
    if (hp2 > 9999) hp2 = 9999;

    // ---- name / pane text ----
    // Retail: BdatGetPtrDirect(::cf::CfBdat::spBtlPcListFileData, "name", member) -- 3-arg.
    // The literal is compiler-pooled at the retail .rodata offset.
    LayoutSetTextBoxFmtValue(mpLayout, "txt_para736",
        ((char*(*)(void*, char*, u32))&BdatGetPtrDirect)(::cf::CfBdat::spBtlPcListFileData, "name", member), 0);
    setLayoutTextBoxNumber(mpLayout, "txt_value700", (u8)stats->CActorParam_getActorLevel());
    setLayoutTextBoxNumber(mpLayout, "txt_value702", (u8)stats->CActorParam_getTotalCurrency());
    LayoutSetTextBoxInt((nw4r::lyt::Layout*)mSlotPane0, hp1);
    LayoutSetTextBoxInt((nw4r::lyt::Layout*)mSlotPane2, hp2);
    SetLayoutSelfText((nw4r::lyt::Layout*)mSlotPane3, BdatTouchStringCell("MNU_item", "name", 0x82), 0);

    // ---- stat sub-objects ----
    D8EStatA* stA = (D8EStatA*)stats->CActorParam_getBaseStats();
    D8EStatB* stB = (D8EStatB*)stats->CActorParam_getArtsDataBlock();
    D8EStatC* stC = (D8EStatC*)stats->CActorParam_getBonusStats();
    CtrlObjectParam_GetArtsStatsRow((u8*)((CItemBoxCharEntry*)charObj)->slotIds, 5);

    // ---- arts sum ----
    s32 artsSum = (s32)SumTypeCounts(member, NULL);
    u32 w = GetCondStat((void*)(u32)member, 0x97);
    if (w != 0) {
        artsSum = artsSum - (s32)w;
        if (artsSum < 0) artsSum = 0;
    }

    // ---- stat bars (display formula) ----
    s32 effect = GetEffect(member, 0x21, NULL);
// Float literals here match the ~7% HighFrame baseline; SDA 68040/44 names elsewhere
// still open.
    s16 bar1 = (s16)(s32)(0.01f * ((100.0f + (f32)stC->s10) * (f32)(stA->s20 + effect)));
    effect = GetEffect(member, 0x1, NULL);
    s16 bar2 = (s16)(s32)(0.01f * ((100.0f + (f32)stC->s0C) * (f32)(stA->s1C + effect)));
    effect = GetEffect(member, 0x31, NULL);
    s16 bar3 = (s16)(s32)(0.01f * ((100.0f + (f32)stC->s1C) * (f32)(stA->s32 + effect)));
    effect = GetEffect(member, 0x41, NULL);
    s16 bar4 = (s16)(s32)(0.01f * ((100.0f + (f32)stC->s0E) * (f32)(stA->s1E - (s16)artsSum + effect)));
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
            void* item2 = CItem_kindPtr(2, ((CItemBoxCharEntry*)charObj)->slotIds[5]);
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
                    "MNU_item", "name", 0xB);
                textBuffer.format("%d%s%d", atk1, rangeSeparator, atk2);
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
                "MNU_item", "name", 0x80);
            textBuffer.format("%d%s", (s16)bar6, percentSuffix);
            textBufferB.format("%d%s", (s16)(stA->b55), percentSuffix);
            SetLayoutSelfText((nw4r::lyt::Layout*)mSlotPane16, (const char*)textBuffer.mString, 0);
            SetLayoutSelfText((nw4r::lyt::Layout*)mSlotPane18, (const char*)textBufferB.mString, 0);

            // ---- 20x color application ----
            D8EQuad q1 = *(D8EQuad*)&itemBoxInfo_labelNeutralTop;
            D8EQuad q2 = *(D8EQuad*)&itemBoxInfo_labelNeutralBottom;
            D8EQuad q3 = *(D8EQuad*)&itemBoxInfo_valueNeutralTop;
            D8EQuad q4 = *(D8EQuad*)&itemBoxInfo_valueNeutralBottom;
            PaneMatSetTevColors(mSlotPane0, (const GXColorS10*)&q3, (const GXColorS10*)&q4);
            PaneMatSetTevColors(mSlotPane2, (const GXColorS10*)&q3, (const GXColorS10*)&q4);
            PaneMatSetTevColors(mSlotPane3, (const GXColorS10*)&q3, (const GXColorS10*)&q4);
            PaneMatSetTevColors(mSlotPane1, (const GXColorS10*)&q1, (const GXColorS10*)&q2);
            PaneMatSetTevColors(mSlotPane4, (const GXColorS10*)&q3, (const GXColorS10*)&q4);
            PaneMatSetTevColors(mSlotPane5, (const GXColorS10*)&q1, (const GXColorS10*)&q2);
            PaneMatSetTevColors(mSlotPane6, (const GXColorS10*)&q3, (const GXColorS10*)&q4);
            PaneMatSetTevColors(mSlotPane7, (const GXColorS10*)&q1, (const GXColorS10*)&q2);
            PaneMatSetTevColors(mSlotPane8, (const GXColorS10*)&q3, (const GXColorS10*)&q4);
            PaneMatSetTevColors(mSlotPane9, (const GXColorS10*)&q1, (const GXColorS10*)&q2);
            PaneMatSetTevColors(mSlotPane10, (const GXColorS10*)&q3, (const GXColorS10*)&q4);
            PaneMatSetTevColors(mSlotPane11, (const GXColorS10*)&q1, (const GXColorS10*)&q2);
            PaneMatSetTevColors(mSlotPane12, (const GXColorS10*)&q3, (const GXColorS10*)&q4);
            PaneMatSetTevColors(mSlotPane13, (const GXColorS10*)&q1, (const GXColorS10*)&q2);
            PaneMatSetTevColors(mSlotPane14, (const GXColorS10*)&q3, (const GXColorS10*)&q4);
            PaneMatSetTevColors(mSlotPane15, (const GXColorS10*)&q1, (const GXColorS10*)&q2);
            PaneMatSetTevColors(mSlotPane16, (const GXColorS10*)&q3, (const GXColorS10*)&q4);
            PaneMatSetTevColors(mSlotPane17, (const GXColorS10*)&q1, (const GXColorS10*)&q2);
            PaneMatSetTevColors(mSlotPane18, (const GXColorS10*)&q3, (const GXColorS10*)&q4);
            PaneMatSetTevColors(mSlotPane19, (const GXColorS10*)&q1, (const GXColorS10*)&q2);
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
            D8EQuad q1 = *(D8EQuad*)&itemBoxInfo_labelNeutralTop;
            D8EQuad q2 = *(D8EQuad*)&itemBoxInfo_labelNeutralBottom;
            D8EQuad q3 = *(D8EQuad*)&itemBoxInfo_valueNeutralTop;
            D8EQuad q4 = *(D8EQuad*)&itemBoxInfo_valueNeutralBottom;
            if (d < 0) {
                __as__11_GXColorS10FRC11_GXColorS10(&q1, &itemBoxInfo_labelDownTop);
                __as__11_GXColorS10FRC11_GXColorS10(&q2, &itemBoxInfo_labelDownBottom);
                __as__11_GXColorS10FRC11_GXColorS10(&q3, &itemBoxInfo_valueDownTop);
                __as__11_GXColorS10FRC11_GXColorS10(&q4, &itemBoxInfo_valueDownBottom);
            } else if (d > 0) {
                __as__11_GXColorS10FRC11_GXColorS10(&q1, &itemBoxInfo_labelUpTop);
                __as__11_GXColorS10FRC11_GXColorS10(&q2, &itemBoxInfo_labelUpBottom);
                __as__11_GXColorS10FRC11_GXColorS10(&q3, &itemBoxInfo_valueUpTop);
                __as__11_GXColorS10FRC11_GXColorS10(&q4, &itemBoxInfo_valueUpBottom);
            }
            PaneMatSetTevColors(mSlotPane8, (const GXColorS10*)&q3, (const GXColorS10*)&q4);
            PaneMatSetTevColors(mSlotPane9, (const GXColorS10*)&q1, (const GXColorS10*)&q2);
        }

        // row B: 0x1 -> panes 0x50/0x54
        {
            s16 pb = (s16)(s32)(0.01f * ((100.0f + (f32)stC->s0C + (f32)e1) * (f32)(stA->s1C + GetEffect(member, 0x1, NULL))));
            s16 nb = (s16)(s32)(0.01f * ((100.0f + (f32)stC->s0C + (f32)e2) * (f32)(stA->s1C + GetEffect(member, 0x1, arg3))));
            dB = (s16)(nb - pb);
            LayoutSetTextBoxInt((nw4r::lyt::Layout*)mSlotPane4, nb);
            D8EQuad q1 = *(D8EQuad*)&itemBoxInfo_labelNeutralTop;
            D8EQuad q2 = *(D8EQuad*)&itemBoxInfo_labelNeutralBottom;
            D8EQuad q3 = *(D8EQuad*)&itemBoxInfo_valueNeutralTop;
            D8EQuad q4 = *(D8EQuad*)&itemBoxInfo_valueNeutralBottom;
            if (dB < 0) {
                __as__11_GXColorS10FRC11_GXColorS10(&q1, &itemBoxInfo_labelDownTop);
                __as__11_GXColorS10FRC11_GXColorS10(&q2, &itemBoxInfo_labelDownBottom);
                __as__11_GXColorS10FRC11_GXColorS10(&q3, &itemBoxInfo_valueDownTop);
                __as__11_GXColorS10FRC11_GXColorS10(&q4, &itemBoxInfo_valueDownBottom);
            } else if (dB > 0) {
                __as__11_GXColorS10FRC11_GXColorS10(&q1, &itemBoxInfo_labelUpTop);
                __as__11_GXColorS10FRC11_GXColorS10(&q2, &itemBoxInfo_labelUpBottom);
                __as__11_GXColorS10FRC11_GXColorS10(&q3, &itemBoxInfo_valueUpTop);
                __as__11_GXColorS10FRC11_GXColorS10(&q4, &itemBoxInfo_valueUpBottom);
            }
            PaneMatSetTevColors(mSlotPane4, (const GXColorS10*)&q3, (const GXColorS10*)&q4);
            PaneMatSetTevColors(mSlotPane5, (const GXColorS10*)&q1, (const GXColorS10*)&q2);
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
            D8EQuad q1 = *(D8EQuad*)&itemBoxInfo_labelNeutralTop;
            D8EQuad q2 = *(D8EQuad*)&itemBoxInfo_labelNeutralBottom;
            D8EQuad q3 = *(D8EQuad*)&itemBoxInfo_valueNeutralTop;
            D8EQuad q4 = *(D8EQuad*)&itemBoxInfo_valueNeutralBottom;
            if (dC < 0) {
                __as__11_GXColorS10FRC11_GXColorS10(&q1, &itemBoxInfo_labelDownTop);
                __as__11_GXColorS10FRC11_GXColorS10(&q2, &itemBoxInfo_labelDownBottom);
                __as__11_GXColorS10FRC11_GXColorS10(&q3, &itemBoxInfo_valueDownTop);
                __as__11_GXColorS10FRC11_GXColorS10(&q4, &itemBoxInfo_valueDownBottom);
            } else if (dC > 0) {
                __as__11_GXColorS10FRC11_GXColorS10(&q1, &itemBoxInfo_labelUpTop);
                __as__11_GXColorS10FRC11_GXColorS10(&q2, &itemBoxInfo_labelUpBottom);
                __as__11_GXColorS10FRC11_GXColorS10(&q3, &itemBoxInfo_valueUpTop);
                __as__11_GXColorS10FRC11_GXColorS10(&q4, &itemBoxInfo_valueUpBottom);
            }
            PaneMatSetTevColors(mSlotPane6, (const GXColorS10*)&q3, (const GXColorS10*)&q4);
            PaneMatSetTevColors(mSlotPane7, (const GXColorS10*)&q1, (const GXColorS10*)&q2);
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
            D8EQuad q1 = *(D8EQuad*)&itemBoxInfo_labelNeutralTop;
            D8EQuad q2 = *(D8EQuad*)&itemBoxInfo_labelNeutralBottom;
            D8EQuad q3 = *(D8EQuad*)&itemBoxInfo_valueNeutralTop;
            D8EQuad q4 = *(D8EQuad*)&itemBoxInfo_valueNeutralBottom;
            if (dD < 0) {
                __as__11_GXColorS10FRC11_GXColorS10(&q1, &itemBoxInfo_labelDownTop);
                __as__11_GXColorS10FRC11_GXColorS10(&q2, &itemBoxInfo_labelDownBottom);
                __as__11_GXColorS10FRC11_GXColorS10(&q3, &itemBoxInfo_valueDownTop);
                __as__11_GXColorS10FRC11_GXColorS10(&q4, &itemBoxInfo_valueDownBottom);
            } else if (dD > 0) {
                __as__11_GXColorS10FRC11_GXColorS10(&q1, &itemBoxInfo_labelUpTop);
                __as__11_GXColorS10FRC11_GXColorS10(&q2, &itemBoxInfo_labelUpBottom);
                __as__11_GXColorS10FRC11_GXColorS10(&q3, &itemBoxInfo_valueUpTop);
                __as__11_GXColorS10FRC11_GXColorS10(&q4, &itemBoxInfo_valueUpBottom);
            }
            PaneMatSetTevColors(mSlotPane0, (const GXColorS10*)&q3, (const GXColorS10*)&q4);
            PaneMatSetTevColors(mSlotPane2, (const GXColorS10*)&q3, (const GXColorS10*)&q4);
            PaneMatSetTevColors(mSlotPane3, (const GXColorS10*)&q3, (const GXColorS10*)&q4);
            PaneMatSetTevColors(mSlotPane1, (const GXColorS10*)&q1, (const GXColorS10*)&q2);
        }
        // ---- type switch: slot id + item ----
        s16 slotId = 0;
        // (was switch: retail compiles this dispatch as compares)
        if (type == 2) slotId = ((CItemBoxCharEntry*)charObj)->slotIds[5];
        else if (type == 4) slotId = ((CItemBoxCharEntry*)charObj)->slotIds[0];
        else if (type == 5) slotId = ((CItemBoxCharEntry*)charObj)->slotIds[1];
        else if (type == 6) slotId = ((CItemBoxCharEntry*)charObj)->slotIds[2];
        else if (type == 7) slotId = ((CItemBoxCharEntry*)charObj)->slotIds[3];
        else if (type == 8) slotId = ((CItemBoxCharEntry*)charObj)->slotIds[4];
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
                "MNU_item", "name", 0xB);
            textBuffer.format("%d%s%d", atkD, rangeSeparator, atkC);
            SetLayoutSelfText((nw4r::lyt::Layout*)mSlotPane10, (const char*)textBuffer.mString, 0);
            LayoutSetTextBoxInt((nw4r::lyt::Layout*)mSlotPane12, (s16)disp1);
            LayoutSetTextBoxInt((nw4r::lyt::Layout*)mSlotPane14, (s16)disp2);
            char* percentSuffix2 = BdatTouchStringCell(
                "MNU_item", "name", 0x80);
            textBuffer.format("%d%s", (s16)disp3, percentSuffix2);
            SetLayoutSelfText((nw4r::lyt::Layout*)mSlotPane16, (const char*)textBuffer.mString, 0);
            char* percentSuffix3 = BdatTouchStringCell(
                "MNU_item", "name", 0x80);
            textBuffer.format("%d%s",
                           (s32)stA->b55 + (s32)d3, percentSuffix3);
            SetLayoutSelfText((nw4r::lyt::Layout*)mSlotPane18, (const char*)textBuffer.mString, 0);
            // delta colors (5 rows)
            {
                D8EQuad q1 = *(D8EQuad*)&itemBoxInfo_labelNeutralTop;
                D8EQuad q2 = *(D8EQuad*)&itemBoxInfo_labelNeutralBottom;
                D8EQuad q3 = *(D8EQuad*)&itemBoxInfo_valueNeutralTop;
                D8EQuad q4 = *(D8EQuad*)&itemBoxInfo_valueNeutralBottom;
                if (dW < 0) {
                    __as__11_GXColorS10FRC11_GXColorS10(&q1, &itemBoxInfo_labelDownTop);
                    __as__11_GXColorS10FRC11_GXColorS10(&q2, &itemBoxInfo_labelDownBottom);
                    __as__11_GXColorS10FRC11_GXColorS10(&q3, &itemBoxInfo_valueDownTop);
                    __as__11_GXColorS10FRC11_GXColorS10(&q4, &itemBoxInfo_valueDownBottom);
                } else if (dW > 0) {
                    __as__11_GXColorS10FRC11_GXColorS10(&q1, &itemBoxInfo_labelUpTop);
                    __as__11_GXColorS10FRC11_GXColorS10(&q2, &itemBoxInfo_labelUpBottom);
                    __as__11_GXColorS10FRC11_GXColorS10(&q3, &itemBoxInfo_valueUpTop);
                    __as__11_GXColorS10FRC11_GXColorS10(&q4, &itemBoxInfo_valueUpBottom);
                }
                PaneMatSetTevColors(mSlotPane10, (const GXColorS10*)&q3, (const GXColorS10*)&q4);
                PaneMatSetTevColors(mSlotPane11, (const GXColorS10*)&q1, (const GXColorS10*)&q2);
            }
            {
                D8EQuad q1 = *(D8EQuad*)&itemBoxInfo_labelNeutralTop;
                D8EQuad q2 = *(D8EQuad*)&itemBoxInfo_labelNeutralBottom;
                D8EQuad q3 = *(D8EQuad*)&itemBoxInfo_valueNeutralTop;
                D8EQuad q4 = *(D8EQuad*)&itemBoxInfo_valueNeutralBottom;
                if (d1 < 0) {
                    __as__11_GXColorS10FRC11_GXColorS10(&q1, &itemBoxInfo_labelDownTop);
                    __as__11_GXColorS10FRC11_GXColorS10(&q2, &itemBoxInfo_labelDownBottom);
                    __as__11_GXColorS10FRC11_GXColorS10(&q3, &itemBoxInfo_valueDownTop);
                    __as__11_GXColorS10FRC11_GXColorS10(&q4, &itemBoxInfo_valueDownBottom);
                } else if (d1 > 0) {
                    __as__11_GXColorS10FRC11_GXColorS10(&q1, &itemBoxInfo_labelUpTop);
                    __as__11_GXColorS10FRC11_GXColorS10(&q2, &itemBoxInfo_labelUpBottom);
                    __as__11_GXColorS10FRC11_GXColorS10(&q3, &itemBoxInfo_valueUpTop);
                    __as__11_GXColorS10FRC11_GXColorS10(&q4, &itemBoxInfo_valueUpBottom);
                }
                PaneMatSetTevColors(mSlotPane12, (const GXColorS10*)&q3, (const GXColorS10*)&q4);
                PaneMatSetTevColors(mSlotPane13, (const GXColorS10*)&q1, (const GXColorS10*)&q2);
            }
            {
                D8EQuad q1 = *(D8EQuad*)&itemBoxInfo_labelNeutralTop;
                D8EQuad q2 = *(D8EQuad*)&itemBoxInfo_labelNeutralBottom;
                D8EQuad q3 = *(D8EQuad*)&itemBoxInfo_valueNeutralTop;
                D8EQuad q4 = *(D8EQuad*)&itemBoxInfo_valueNeutralBottom;
                if (d2 < 0) {
                    __as__11_GXColorS10FRC11_GXColorS10(&q1, &itemBoxInfo_labelDownTop);
                    __as__11_GXColorS10FRC11_GXColorS10(&q2, &itemBoxInfo_labelDownBottom);
                    __as__11_GXColorS10FRC11_GXColorS10(&q3, &itemBoxInfo_valueDownTop);
                    __as__11_GXColorS10FRC11_GXColorS10(&q4, &itemBoxInfo_valueDownBottom);
                } else if (d2 > 0) {
                    __as__11_GXColorS10FRC11_GXColorS10(&q1, &itemBoxInfo_labelUpTop);
                    __as__11_GXColorS10FRC11_GXColorS10(&q2, &itemBoxInfo_labelUpBottom);
                    __as__11_GXColorS10FRC11_GXColorS10(&q3, &itemBoxInfo_valueUpTop);
                    __as__11_GXColorS10FRC11_GXColorS10(&q4, &itemBoxInfo_valueUpBottom);
                }
                PaneMatSetTevColors(mSlotPane14, (const GXColorS10*)&q3, (const GXColorS10*)&q4);
                PaneMatSetTevColors(mSlotPane15, (const GXColorS10*)&q1, (const GXColorS10*)&q2);
            }
            {
                D8EQuad q1 = *(D8EQuad*)&itemBoxInfo_labelNeutralTop;
                D8EQuad q2 = *(D8EQuad*)&itemBoxInfo_labelNeutralBottom;
                D8EQuad q3 = *(D8EQuad*)&itemBoxInfo_valueNeutralTop;
                D8EQuad q4 = *(D8EQuad*)&itemBoxInfo_valueNeutralBottom;
                if (d4 < 0) {
                    __as__11_GXColorS10FRC11_GXColorS10(&q1, &itemBoxInfo_labelDownTop);
                    __as__11_GXColorS10FRC11_GXColorS10(&q2, &itemBoxInfo_labelDownBottom);
                    __as__11_GXColorS10FRC11_GXColorS10(&q3, &itemBoxInfo_valueDownTop);
                    __as__11_GXColorS10FRC11_GXColorS10(&q4, &itemBoxInfo_valueDownBottom);
                } else if (d4 > 0) {
                    __as__11_GXColorS10FRC11_GXColorS10(&q1, &itemBoxInfo_labelUpTop);
                    __as__11_GXColorS10FRC11_GXColorS10(&q2, &itemBoxInfo_labelUpBottom);
                    __as__11_GXColorS10FRC11_GXColorS10(&q3, &itemBoxInfo_valueUpTop);
                    __as__11_GXColorS10FRC11_GXColorS10(&q4, &itemBoxInfo_valueUpBottom);
                }
                PaneMatSetTevColors(mSlotPane16, (const GXColorS10*)&q3, (const GXColorS10*)&q4);
                PaneMatSetTevColors(mSlotPane17, (const GXColorS10*)&q1, (const GXColorS10*)&q2);
            }
            {
                D8EQuad q1 = *(D8EQuad*)&itemBoxInfo_labelNeutralTop;
                D8EQuad q2 = *(D8EQuad*)&itemBoxInfo_labelNeutralBottom;
                D8EQuad q3 = *(D8EQuad*)&itemBoxInfo_valueNeutralTop;
                D8EQuad q4 = *(D8EQuad*)&itemBoxInfo_valueNeutralBottom;
                if (d3 < 0) {
                    __as__11_GXColorS10FRC11_GXColorS10(&q1, &itemBoxInfo_labelDownTop);
                    __as__11_GXColorS10FRC11_GXColorS10(&q2, &itemBoxInfo_labelDownBottom);
                    __as__11_GXColorS10FRC11_GXColorS10(&q3, &itemBoxInfo_valueDownTop);
                    __as__11_GXColorS10FRC11_GXColorS10(&q4, &itemBoxInfo_valueDownBottom);
                } else if (d3 > 0) {
                    __as__11_GXColorS10FRC11_GXColorS10(&q1, &itemBoxInfo_labelUpTop);
                    __as__11_GXColorS10FRC11_GXColorS10(&q2, &itemBoxInfo_labelUpBottom);
                    __as__11_GXColorS10FRC11_GXColorS10(&q3, &itemBoxInfo_valueUpTop);
                    __as__11_GXColorS10FRC11_GXColorS10(&q4, &itemBoxInfo_valueUpBottom);
                }
                PaneMatSetTevColors(mSlotPane18, (const GXColorS10*)&q3, (const GXColorS10*)&q4);
                PaneMatSetTevColors(mSlotPane19, (const GXColorS10*)&q1, (const GXColorS10*)&q2);
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
                (const void*)::cf::CfBdat::spItmEquipListFileData, "arm_type",
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
            u8 arm2 = BdatGetU8Direct((const void*)::cf::CfBdat::spItmEquipListFileData, "arm_type", BdatGetItemId((u32)(*(u32*)arg3 >> 20)));
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
            s32 ssum = (s32)((CItemBoxCharEntry*)charObj)->slotIds[3] + ((CItemBoxCharEntry*)charObj)->slotIds[2] + ((CItemBoxCharEntry*)charObj)->slotIds[4] + ((CItemBoxCharEntry*)charObj)->slotIds[0] + ((CItemBoxCharEntry*)charObj)->slotIds[1];
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
            void* item2 = CItem_kindPtr(2, ((CItemBoxCharEntry*)charObj)->slotIds[5]);
            if (item2 != NULL && *(u32*)item2 != 0) {
                D8EEntry previewWeapon;
                previewWeapon.rec = BuildCmpRecord((void*)(u32)(u16)(*(u32*)item2 >> 20));
                s16 newPercent = (s16)(s32)(0.01f * ((100.0f + (f32)stC->s22) *
                    (f32)(stA->s38 + GetEffect(member, 0x54, arg3))));
                s16 oldPercent = (s16)(s32)(0.01f * ((100.0f + (f32)stC->s22) *
                    (f32)(stA->s38 + GetEffect(member, 0x54, NULL))));
                percentDelta = (s16)(newPercent - oldPercent);
                char* percentSuffix = BdatTouchStringCell(
                    "MNU_item", "name", 0x80);
                textBuffer.format("%d%s", newPercent,
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
                    "MNU_item", "name", 0xB);
                textBuffer.format("%d%s%d", wmin, rangeSeparator, wd);
                SetLayoutSelfText((nw4r::lyt::Layout*)mSlotPane10, (const char*)textBuffer.mString, 0);

            }
            // numbers
            LayoutSetTextBoxInt((nw4r::lyt::Layout*)mSlotPane12, c_new.physicalDefense);
            LayoutSetTextBoxInt((nw4r::lyt::Layout*)mSlotPane14, c_new.etherDefense);
            LayoutSetTextBoxInt((nw4r::lyt::Layout*)mSlotPane6, (s16)b14);
            // delta colors (4 rows)
            {
                D8EQuad q1 = *(D8EQuad*)&itemBoxInfo_labelNeutralTop;
                D8EQuad q2 = *(D8EQuad*)&itemBoxInfo_labelNeutralBottom;
                D8EQuad q3 = *(D8EQuad*)&itemBoxInfo_valueNeutralTop;
                D8EQuad q4 = *(D8EQuad*)&itemBoxInfo_valueNeutralBottom;
                if (attackDelta < 0) {
                    __as__11_GXColorS10FRC11_GXColorS10(&q1, &itemBoxInfo_labelDownTop);
                    __as__11_GXColorS10FRC11_GXColorS10(&q2, &itemBoxInfo_labelDownBottom);
                    __as__11_GXColorS10FRC11_GXColorS10(&q3, &itemBoxInfo_valueDownTop);
                    __as__11_GXColorS10FRC11_GXColorS10(&q4, &itemBoxInfo_valueDownBottom);
                } else if (attackDelta > 0) {
                    __as__11_GXColorS10FRC11_GXColorS10(&q1, &itemBoxInfo_labelUpTop);
                    __as__11_GXColorS10FRC11_GXColorS10(&q2, &itemBoxInfo_labelUpBottom);
                    __as__11_GXColorS10FRC11_GXColorS10(&q3, &itemBoxInfo_valueUpTop);
                    __as__11_GXColorS10FRC11_GXColorS10(&q4, &itemBoxInfo_valueUpBottom);
                }
                PaneMatSetTevColors(mSlotPane10, (const GXColorS10*)&q3, (const GXColorS10*)&q4);
                PaneMatSetTevColors(mSlotPane11, (const GXColorS10*)&q1, (const GXColorS10*)&q2);
            }
            {
                D8EQuad q1 = *(D8EQuad*)&itemBoxInfo_labelNeutralTop;
                D8EQuad q2 = *(D8EQuad*)&itemBoxInfo_labelNeutralBottom;
                D8EQuad q3 = *(D8EQuad*)&itemBoxInfo_valueNeutralTop;
                D8EQuad q4 = *(D8EQuad*)&itemBoxInfo_valueNeutralBottom;
                if (d2 < 0) {
                    __as__11_GXColorS10FRC11_GXColorS10(&q1, &itemBoxInfo_labelDownTop);
                    __as__11_GXColorS10FRC11_GXColorS10(&q2, &itemBoxInfo_labelDownBottom);
                    __as__11_GXColorS10FRC11_GXColorS10(&q3, &itemBoxInfo_valueDownTop);
                    __as__11_GXColorS10FRC11_GXColorS10(&q4, &itemBoxInfo_valueDownBottom);
                } else if (d2 > 0) {
                    __as__11_GXColorS10FRC11_GXColorS10(&q1, &itemBoxInfo_labelUpTop);
                    __as__11_GXColorS10FRC11_GXColorS10(&q2, &itemBoxInfo_labelUpBottom);
                    __as__11_GXColorS10FRC11_GXColorS10(&q3, &itemBoxInfo_valueUpTop);
                    __as__11_GXColorS10FRC11_GXColorS10(&q4, &itemBoxInfo_valueUpBottom);
                }
                PaneMatSetTevColors(mSlotPane12, (const GXColorS10*)&q3, (const GXColorS10*)&q4);
                PaneMatSetTevColors(mSlotPane13, (const GXColorS10*)&q1, (const GXColorS10*)&q2);
            }
            {
                D8EQuad q1 = *(D8EQuad*)&itemBoxInfo_labelNeutralTop;
                D8EQuad q2 = *(D8EQuad*)&itemBoxInfo_labelNeutralBottom;
                D8EQuad q3 = *(D8EQuad*)&itemBoxInfo_valueNeutralTop;
                D8EQuad q4 = *(D8EQuad*)&itemBoxInfo_valueNeutralBottom;
                if (d1 < 0) {
                    __as__11_GXColorS10FRC11_GXColorS10(&q1, &itemBoxInfo_labelDownTop);
                    __as__11_GXColorS10FRC11_GXColorS10(&q2, &itemBoxInfo_labelDownBottom);
                    __as__11_GXColorS10FRC11_GXColorS10(&q3, &itemBoxInfo_valueDownTop);
                    __as__11_GXColorS10FRC11_GXColorS10(&q4, &itemBoxInfo_valueDownBottom);
                } else if (d1 > 0) {
                    __as__11_GXColorS10FRC11_GXColorS10(&q1, &itemBoxInfo_labelUpTop);
                    __as__11_GXColorS10FRC11_GXColorS10(&q2, &itemBoxInfo_labelUpBottom);
                    __as__11_GXColorS10FRC11_GXColorS10(&q3, &itemBoxInfo_valueUpTop);
                    __as__11_GXColorS10FRC11_GXColorS10(&q4, &itemBoxInfo_valueUpBottom);
                }
                PaneMatSetTevColors(mSlotPane14, (const GXColorS10*)&q3, (const GXColorS10*)&q4);
                PaneMatSetTevColors(mSlotPane15, (const GXColorS10*)&q1, (const GXColorS10*)&q2);
            }
            {
                D8EQuad q1 = *(D8EQuad*)&itemBoxInfo_labelNeutralTop;
                D8EQuad q2 = *(D8EQuad*)&itemBoxInfo_labelNeutralBottom;
                D8EQuad q3 = *(D8EQuad*)&itemBoxInfo_valueNeutralTop;
                D8EQuad q4 = *(D8EQuad*)&itemBoxInfo_valueNeutralBottom;
                if (percentDelta < 0) {
                    __as__11_GXColorS10FRC11_GXColorS10(&q1, &itemBoxInfo_labelDownTop);
                    __as__11_GXColorS10FRC11_GXColorS10(&q2, &itemBoxInfo_labelDownBottom);
                    __as__11_GXColorS10FRC11_GXColorS10(&q3, &itemBoxInfo_valueDownTop);
                    __as__11_GXColorS10FRC11_GXColorS10(&q4, &itemBoxInfo_valueDownBottom);
                } else if (percentDelta > 0) {
                    __as__11_GXColorS10FRC11_GXColorS10(&q1, &itemBoxInfo_labelUpTop);
                    __as__11_GXColorS10FRC11_GXColorS10(&q2, &itemBoxInfo_labelUpBottom);
                    __as__11_GXColorS10FRC11_GXColorS10(&q3, &itemBoxInfo_valueUpTop);
                    __as__11_GXColorS10FRC11_GXColorS10(&q4, &itemBoxInfo_valueUpBottom);
                }
                PaneMatSetTevColors(mSlotPane6, (const GXColorS10*)&q3, (const GXColorS10*)&q4);
                PaneMatSetTevColors(mSlotPane7, (const GXColorS10*)&q1, (const GXColorS10*)&q2);
            }
            {
        D8EQuad labelTop = *(D8EQuad*)&itemBoxInfo_labelNeutralTop;
        D8EQuad labelBottom = *(D8EQuad*)&itemBoxInfo_labelNeutralBottom;
        D8EQuad valueTop = *(D8EQuad*)&itemBoxInfo_valueNeutralTop;
        D8EQuad valueBottom = *(D8EQuad*)&itemBoxInfo_valueNeutralBottom;
        if ((agilityDelta) < 0) {
            __as__11_GXColorS10FRC11_GXColorS10(&labelTop, &itemBoxInfo_labelDownTop);
            __as__11_GXColorS10FRC11_GXColorS10(&labelBottom, &itemBoxInfo_labelDownBottom);
            __as__11_GXColorS10FRC11_GXColorS10(&valueTop, &itemBoxInfo_valueDownTop);
            __as__11_GXColorS10FRC11_GXColorS10(&valueBottom, &itemBoxInfo_valueDownBottom);
        } else if ((agilityDelta) > 0) {
            __as__11_GXColorS10FRC11_GXColorS10(&labelTop, &itemBoxInfo_labelUpTop);
            __as__11_GXColorS10FRC11_GXColorS10(&labelBottom, &itemBoxInfo_labelUpBottom);
            __as__11_GXColorS10FRC11_GXColorS10(&valueTop, &itemBoxInfo_valueUpTop);
            __as__11_GXColorS10FRC11_GXColorS10(&valueBottom, &itemBoxInfo_valueUpBottom);
        }
        nw4r::lyt::Pane** panes = &mSlotPane0;
        PaneMatSetTevColors(panes[(8)], (const GXColorS10*)&valueTop, (const GXColorS10*)&valueBottom);
        PaneMatSetTevColors(panes[(9)], (const GXColorS10*)&labelTop, (const GXColorS10*)&labelBottom);
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
        D8EQuad labelTop = *(D8EQuad*)&itemBoxInfo_labelNeutralTop;
        D8EQuad labelBottom = *(D8EQuad*)&itemBoxInfo_labelNeutralBottom;
        D8EQuad valueTop = *(D8EQuad*)&itemBoxInfo_valueNeutralTop;
        D8EQuad valueBottom = *(D8EQuad*)&itemBoxInfo_valueNeutralBottom;
        if ((strengthDelta) < 0) {
            __as__11_GXColorS10FRC11_GXColorS10(&labelTop, &itemBoxInfo_labelDownTop);
            __as__11_GXColorS10FRC11_GXColorS10(&labelBottom, &itemBoxInfo_labelDownBottom);
            __as__11_GXColorS10FRC11_GXColorS10(&valueTop, &itemBoxInfo_valueDownTop);
            __as__11_GXColorS10FRC11_GXColorS10(&valueBottom, &itemBoxInfo_valueDownBottom);
        } else if ((strengthDelta) > 0) {
            __as__11_GXColorS10FRC11_GXColorS10(&labelTop, &itemBoxInfo_labelUpTop);
            __as__11_GXColorS10FRC11_GXColorS10(&labelBottom, &itemBoxInfo_labelUpBottom);
            __as__11_GXColorS10FRC11_GXColorS10(&valueTop, &itemBoxInfo_valueUpTop);
            __as__11_GXColorS10FRC11_GXColorS10(&valueBottom, &itemBoxInfo_valueUpBottom);
        }
        nw4r::lyt::Pane** panes = &mSlotPane0;
        PaneMatSetTevColors(panes[(8)], (const GXColorS10*)&valueTop, (const GXColorS10*)&valueBottom);
        PaneMatSetTevColors(panes[(9)], (const GXColorS10*)&labelTop, (const GXColorS10*)&labelBottom);
}

        s16 oldPhysical = (s16)(s32)(0.01f * ((100.0f + (f32)(stC->s0C + currentEquipBonus)) * (f32)((stA->s1C) + (GetEffect(member, 0x01, NULL)))));
        s16 newPhysical = (s16)(s32)(0.01f * ((100.0f + (f32)(stC->s0C + currentEquipBonus)) * (f32)((stA->s1C) + (SumGemBonus((void*)(u32)member, 0x01, arg3, slot)))));
        s16 physicalDelta = (s16)(newPhysical - oldPhysical);
        LayoutSetTextBoxInt((nw4r::lyt::Layout*)mSlotPane4, newPhysical);
        {
        D8EQuad labelTop = *(D8EQuad*)&itemBoxInfo_labelNeutralTop;
        D8EQuad labelBottom = *(D8EQuad*)&itemBoxInfo_labelNeutralBottom;
        D8EQuad valueTop = *(D8EQuad*)&itemBoxInfo_valueNeutralTop;
        D8EQuad valueBottom = *(D8EQuad*)&itemBoxInfo_valueNeutralBottom;
        if ((physicalDelta) < 0) {
            __as__11_GXColorS10FRC11_GXColorS10(&labelTop, &itemBoxInfo_labelDownTop);
            __as__11_GXColorS10FRC11_GXColorS10(&labelBottom, &itemBoxInfo_labelDownBottom);
            __as__11_GXColorS10FRC11_GXColorS10(&valueTop, &itemBoxInfo_valueDownTop);
            __as__11_GXColorS10FRC11_GXColorS10(&valueBottom, &itemBoxInfo_valueDownBottom);
        } else if ((physicalDelta) > 0) {
            __as__11_GXColorS10FRC11_GXColorS10(&labelTop, &itemBoxInfo_labelUpTop);
            __as__11_GXColorS10FRC11_GXColorS10(&labelBottom, &itemBoxInfo_labelUpBottom);
            __as__11_GXColorS10FRC11_GXColorS10(&valueTop, &itemBoxInfo_valueUpTop);
            __as__11_GXColorS10FRC11_GXColorS10(&valueBottom, &itemBoxInfo_valueUpBottom);
        }
        nw4r::lyt::Pane** panes = &mSlotPane0;
        PaneMatSetTevColors(panes[(4)], (const GXColorS10*)&valueTop, (const GXColorS10*)&valueBottom);
        PaneMatSetTevColors(panes[(5)], (const GXColorS10*)&labelTop, (const GXColorS10*)&labelBottom);
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
            D8EQuad labelTop = *(D8EQuad*)&itemBoxInfo_labelNeutralTop;
            D8EQuad labelBottom = *(D8EQuad*)&itemBoxInfo_labelNeutralBottom;
            D8EQuad valueTop = *(D8EQuad*)&itemBoxInfo_valueNeutralTop;
            D8EQuad valueBottom = *(D8EQuad*)&itemBoxInfo_valueNeutralBottom;
            if (hpDelta < 0) {
                __as__11_GXColorS10FRC11_GXColorS10(&labelTop, &itemBoxInfo_labelDownTop);
                __as__11_GXColorS10FRC11_GXColorS10(&labelBottom, &itemBoxInfo_labelDownBottom);
                __as__11_GXColorS10FRC11_GXColorS10(&valueTop, &itemBoxInfo_valueDownTop);
                __as__11_GXColorS10FRC11_GXColorS10(&valueBottom, &itemBoxInfo_valueDownBottom);
            } else if (hpDelta > 0) {
                __as__11_GXColorS10FRC11_GXColorS10(&labelTop, &itemBoxInfo_labelUpTop);
                __as__11_GXColorS10FRC11_GXColorS10(&labelBottom, &itemBoxInfo_labelUpBottom);
                __as__11_GXColorS10FRC11_GXColorS10(&valueTop, &itemBoxInfo_valueUpTop);
                __as__11_GXColorS10FRC11_GXColorS10(&valueBottom, &itemBoxInfo_valueUpBottom);
            }
            PaneMatSetTevColors(mSlotPane0,
                          (const GXColorS10*)&valueTop, (const GXColorS10*)&valueBottom);
            PaneMatSetTevColors(mSlotPane1,
                          (const GXColorS10*)&labelTop, (const GXColorS10*)&labelBottom);
            PaneMatSetTevColors(mSlotPane2,
                          (const GXColorS10*)&valueTop, (const GXColorS10*)&valueBottom);
            PaneMatSetTevColors(mSlotPane3,
                          (const GXColorS10*)&labelTop, (const GXColorS10*)&labelBottom);
        }

        s16 oldEther = (s16)(s32)(0.01f * ((100.0f + (f32)(stC->s1C)) * (f32)((stA->s32) + (GetEffect(member, 0x31, NULL)))));
        s16 newEther = (s16)(s32)(0.01f * ((100.0f + (f32)(stC->s1C)) * (f32)((stA->s32) + (SumGemBonus((void*)(u32)member, 0x31, arg3, slot)))));
        s16 etherDelta = (s16)(newEther - oldEther);
        LayoutSetTextBoxInt((nw4r::lyt::Layout*)mSlotPane12, newEther);
        {
        D8EQuad labelTop = *(D8EQuad*)&itemBoxInfo_labelNeutralTop;
        D8EQuad labelBottom = *(D8EQuad*)&itemBoxInfo_labelNeutralBottom;
        D8EQuad valueTop = *(D8EQuad*)&itemBoxInfo_valueNeutralTop;
        D8EQuad valueBottom = *(D8EQuad*)&itemBoxInfo_valueNeutralBottom;
        if ((etherDelta) < 0) {
            __as__11_GXColorS10FRC11_GXColorS10(&labelTop, &itemBoxInfo_labelDownTop);
            __as__11_GXColorS10FRC11_GXColorS10(&labelBottom, &itemBoxInfo_labelDownBottom);
            __as__11_GXColorS10FRC11_GXColorS10(&valueTop, &itemBoxInfo_valueDownTop);
            __as__11_GXColorS10FRC11_GXColorS10(&valueBottom, &itemBoxInfo_valueDownBottom);
        } else if ((etherDelta) > 0) {
            __as__11_GXColorS10FRC11_GXColorS10(&labelTop, &itemBoxInfo_labelUpTop);
            __as__11_GXColorS10FRC11_GXColorS10(&labelBottom, &itemBoxInfo_labelUpBottom);
            __as__11_GXColorS10FRC11_GXColorS10(&valueTop, &itemBoxInfo_valueUpTop);
            __as__11_GXColorS10FRC11_GXColorS10(&valueBottom, &itemBoxInfo_valueUpBottom);
        }
        nw4r::lyt::Pane** panes = &mSlotPane0;
        PaneMatSetTevColors(panes[(12)], (const GXColorS10*)&valueTop, (const GXColorS10*)&valueBottom);
        PaneMatSetTevColors(panes[(13)], (const GXColorS10*)&labelTop, (const GXColorS10*)&labelBottom);
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
        D8EQuad labelTop = *(D8EQuad*)&itemBoxInfo_labelNeutralTop;
        D8EQuad labelBottom = *(D8EQuad*)&itemBoxInfo_labelNeutralBottom;
        D8EQuad valueTop = *(D8EQuad*)&itemBoxInfo_valueNeutralTop;
        D8EQuad valueBottom = *(D8EQuad*)&itemBoxInfo_valueNeutralBottom;
        if ((agilityDelta) < 0) {
            __as__11_GXColorS10FRC11_GXColorS10(&labelTop, &itemBoxInfo_labelDownTop);
            __as__11_GXColorS10FRC11_GXColorS10(&labelBottom, &itemBoxInfo_labelDownBottom);
            __as__11_GXColorS10FRC11_GXColorS10(&valueTop, &itemBoxInfo_valueDownTop);
            __as__11_GXColorS10FRC11_GXColorS10(&valueBottom, &itemBoxInfo_valueDownBottom);
        } else if ((agilityDelta) > 0) {
            __as__11_GXColorS10FRC11_GXColorS10(&labelTop, &itemBoxInfo_labelUpTop);
            __as__11_GXColorS10FRC11_GXColorS10(&labelBottom, &itemBoxInfo_labelUpBottom);
            __as__11_GXColorS10FRC11_GXColorS10(&valueTop, &itemBoxInfo_valueUpTop);
            __as__11_GXColorS10FRC11_GXColorS10(&valueBottom, &itemBoxInfo_valueUpBottom);
        }
        nw4r::lyt::Pane** panes = &mSlotPane0;
        PaneMatSetTevColors(panes[(6)], (const GXColorS10*)&valueTop, (const GXColorS10*)&valueBottom);
        PaneMatSetTevColors(panes[(7)], (const GXColorS10*)&labelTop, (const GXColorS10*)&labelBottom);
}

        s16 oldDefense = (s16)(s32)(0.01f * ((100.0f + (f32)(stC->s18)) * (f32)((stA->s2E) + (GetEffect(member, 0x51, NULL)))));
        s16 newDefense = (s16)(s32)(0.01f * ((100.0f + (f32)(stC->s18)) * (f32)((stA->s2E) + (SumGemBonus((void*)(u32)member, 0x51, arg3, slot)))));
        s16 defenseDelta = (s16)(newDefense - oldDefense);
        LayoutSetTextBoxInt((nw4r::lyt::Layout*)mSlotPane14, newDefense);
        {
        D8EQuad labelTop = *(D8EQuad*)&itemBoxInfo_labelNeutralTop;
        D8EQuad labelBottom = *(D8EQuad*)&itemBoxInfo_labelNeutralBottom;
        D8EQuad valueTop = *(D8EQuad*)&itemBoxInfo_valueNeutralTop;
        D8EQuad valueBottom = *(D8EQuad*)&itemBoxInfo_valueNeutralBottom;
        if ((defenseDelta) < 0) {
            __as__11_GXColorS10FRC11_GXColorS10(&labelTop, &itemBoxInfo_labelDownTop);
            __as__11_GXColorS10FRC11_GXColorS10(&labelBottom, &itemBoxInfo_labelDownBottom);
            __as__11_GXColorS10FRC11_GXColorS10(&valueTop, &itemBoxInfo_valueDownTop);
            __as__11_GXColorS10FRC11_GXColorS10(&valueBottom, &itemBoxInfo_valueDownBottom);
        } else if ((defenseDelta) > 0) {
            __as__11_GXColorS10FRC11_GXColorS10(&labelTop, &itemBoxInfo_labelUpTop);
            __as__11_GXColorS10FRC11_GXColorS10(&labelBottom, &itemBoxInfo_labelUpBottom);
            __as__11_GXColorS10FRC11_GXColorS10(&valueTop, &itemBoxInfo_valueUpTop);
            __as__11_GXColorS10FRC11_GXColorS10(&valueBottom, &itemBoxInfo_valueUpBottom);
        }
        nw4r::lyt::Pane** panes = &mSlotPane0;
        PaneMatSetTevColors(panes[(14)], (const GXColorS10*)&valueTop, (const GXColorS10*)&valueBottom);
        PaneMatSetTevColors(panes[(15)], (const GXColorS10*)&labelTop, (const GXColorS10*)&labelBottom);
}

        s16 oldResistance = (s16)(s32)(0.01f * ((100.0f + (f32)(stC->s22)) * (f32)((stA->s38) + (GetEffect(member, 0x54, NULL)))));
        s16 newResistance = (s16)(s32)(0.01f * ((100.0f + (f32)(stC->s22)) * (f32)((stA->s38) + (SumGemBonus((void*)(u32)member, 0x54, arg3, slot)))));
        char* percentSuffix = BdatTouchStringCell(
            "MNU_item", "name", 0x80);
        textBuffer.format("%d%s", newResistance,
                              percentSuffix);
        SetLayoutSelfText((nw4r::lyt::Layout*)mSlotPane16, (const char*)textBuffer.mString, 0);
        s16 resistanceDelta = (s16)(newResistance - oldResistance);
        {
        D8EQuad labelTop = *(D8EQuad*)&itemBoxInfo_labelNeutralTop;
        D8EQuad labelBottom = *(D8EQuad*)&itemBoxInfo_labelNeutralBottom;
        D8EQuad valueTop = *(D8EQuad*)&itemBoxInfo_valueNeutralTop;
        D8EQuad valueBottom = *(D8EQuad*)&itemBoxInfo_valueNeutralBottom;
        if ((resistanceDelta) < 0) {
            __as__11_GXColorS10FRC11_GXColorS10(&labelTop, &itemBoxInfo_labelDownTop);
            __as__11_GXColorS10FRC11_GXColorS10(&labelBottom, &itemBoxInfo_labelDownBottom);
            __as__11_GXColorS10FRC11_GXColorS10(&valueTop, &itemBoxInfo_valueDownTop);
            __as__11_GXColorS10FRC11_GXColorS10(&valueBottom, &itemBoxInfo_valueDownBottom);
        } else if ((resistanceDelta) > 0) {
            __as__11_GXColorS10FRC11_GXColorS10(&labelTop, &itemBoxInfo_labelUpTop);
            __as__11_GXColorS10FRC11_GXColorS10(&labelBottom, &itemBoxInfo_labelUpBottom);
            __as__11_GXColorS10FRC11_GXColorS10(&valueTop, &itemBoxInfo_valueUpTop);
            __as__11_GXColorS10FRC11_GXColorS10(&valueBottom, &itemBoxInfo_valueUpBottom);
        }
        nw4r::lyt::Pane** panes = &mSlotPane0;
        PaneMatSetTevColors(panes[(16)], (const GXColorS10*)&valueTop, (const GXColorS10*)&valueBottom);
        PaneMatSetTevColors(panes[(17)], (const GXColorS10*)&labelTop, (const GXColorS10*)&labelBottom);
}

        void* currentWeaponItem = CItem_kindPtr(2, ((CItemBoxCharEntry*)charObj)->slotIds[5]);
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
                "MNU_item", "name", 0xB);
            textBuffer.format("%d%s%d", displayLow,
                              rangeSeparator, newAttackHigh);
            SetLayoutSelfText((nw4r::lyt::Layout*)mSlotPane10,
                          (const char*)textBuffer.mString, 0);
        }
        {
        D8EQuad labelTop = *(D8EQuad*)&itemBoxInfo_labelNeutralTop;
        D8EQuad labelBottom = *(D8EQuad*)&itemBoxInfo_labelNeutralBottom;
        D8EQuad valueTop = *(D8EQuad*)&itemBoxInfo_valueNeutralTop;
        D8EQuad valueBottom = *(D8EQuad*)&itemBoxInfo_valueNeutralBottom;
        if ((attackDelta) < 0) {
            __as__11_GXColorS10FRC11_GXColorS10(&labelTop, &itemBoxInfo_labelDownTop);
            __as__11_GXColorS10FRC11_GXColorS10(&labelBottom, &itemBoxInfo_labelDownBottom);
            __as__11_GXColorS10FRC11_GXColorS10(&valueTop, &itemBoxInfo_valueDownTop);
            __as__11_GXColorS10FRC11_GXColorS10(&valueBottom, &itemBoxInfo_valueDownBottom);
        } else if ((attackDelta) > 0) {
            __as__11_GXColorS10FRC11_GXColorS10(&labelTop, &itemBoxInfo_labelUpTop);
            __as__11_GXColorS10FRC11_GXColorS10(&labelBottom, &itemBoxInfo_labelUpBottom);
            __as__11_GXColorS10FRC11_GXColorS10(&valueTop, &itemBoxInfo_valueUpTop);
            __as__11_GXColorS10FRC11_GXColorS10(&valueBottom, &itemBoxInfo_valueUpBottom);
        }
        nw4r::lyt::Pane** panes = &mSlotPane0;
        PaneMatSetTevColors(panes[(10)], (const GXColorS10*)&valueTop, (const GXColorS10*)&valueBottom);
        PaneMatSetTevColors(panes[(11)], (const GXColorS10*)&labelTop, (const GXColorS10*)&labelBottom);
}
        }

    }

    // ---- equip-detail / simple path (arg4 == 0, type in {2,4,5,6,7,8}) ----
    if (arg4 == 0 && (type == 2 || (type >= 4 && type <= 8))) {
        if (type != 2 && TestBdatFlag(member, arg3)) {
            // ---- equip detail (0x801E8C88) ----
            s16 slotId = 0;
            // (was switch: retail compiles this dispatch as compares)
            if (type == 2) slotId = ((CItemBoxCharEntry*)charObj)->slotIds[5];
            else if (type == 4) slotId = ((CItemBoxCharEntry*)charObj)->slotIds[0];
            else if (type == 5) slotId = ((CItemBoxCharEntry*)charObj)->slotIds[1];
            else if (type == 6) slotId = ((CItemBoxCharEntry*)charObj)->slotIds[2];
            else if (type == 7) slotId = ((CItemBoxCharEntry*)charObj)->slotIds[3];
            else if (type == 8) slotId = ((CItemBoxCharEntry*)charObj)->slotIds[4];
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
                (const void*)::cf::CfBdat::spItmEquipListFileData, "arm_type",
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
            u8 arm2 = BdatGetU8Direct((const void*)::cf::CfBdat::spItmEquipListFileData, "arm_type", BdatGetItemId((u32)(*(u32*)arg3 >> 20)));
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
            s32 ssum = (s32)((CItemBoxCharEntry*)charObj)->slotIds[3] + ((CItemBoxCharEntry*)charObj)->slotIds[2] + ((CItemBoxCharEntry*)charObj)->slotIds[4] + ((CItemBoxCharEntry*)charObj)->slotIds[0] + ((CItemBoxCharEntry*)charObj)->slotIds[1];
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
            s16 currentPhysicalDefense = (s16)(s32)(0.01f *
                ((100.0f + (f32)stC->s18) * (f32)(stA->s2E + c1)));
            GetEffect(member, 0x41, NULL);
            s32 c2 = GetEffect(member, 0x41, arg3);
            s16 b14 = (s16)(s32)(0.01f * ((100.0f + (f32)eq2) *
                (f32)((stA->s1E - (s16)v430) + c2)));
            if (b14 <= 0) b14 = 1;
            // embedded weapon block
            void* item2 = CItem_kindPtr(2, ((CItemBoxCharEntry*)charObj)->slotIds[5]);
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
                    "MNU_item", "name", 0xB);
                textBuffer.format("%d%s%d", wmin, rangeSeparator, wd);
                SetLayoutSelfText((nw4r::lyt::Layout*)mSlotPane10, (const char*)textBuffer.mString, 0);
            }
            // numbers
            LayoutSetTextBoxInt((nw4r::lyt::Layout*)mSlotPane12, c_new.physicalDefense);
            LayoutSetTextBoxInt((nw4r::lyt::Layout*)mSlotPane14, c_new.etherDefense);
            LayoutSetTextBoxInt((nw4r::lyt::Layout*)mSlotPane6, (s16)b14);
            // 20x color application (selected palette A8/B0/C8/D0)
            D8EQuad q1 = *(D8EQuad*)&itemBoxInfo_slotValueTop;
            D8EQuad q2 = *(D8EQuad*)&itemBoxInfo_slotValueBottom;
            D8EQuad q3 = *(D8EQuad*)&itemBoxInfo_nameValueTop;
            D8EQuad q4 = *(D8EQuad*)&itemBoxInfo_nameValueBottom;
            PaneMatSetTevColors(mSlotPane0, (const GXColorS10*)&q3, (const GXColorS10*)&q4);
            PaneMatSetTevColors(mSlotPane2, (const GXColorS10*)&q3, (const GXColorS10*)&q4);
            PaneMatSetTevColors(mSlotPane3, (const GXColorS10*)&q3, (const GXColorS10*)&q4);
            PaneMatSetTevColors(mSlotPane1, (const GXColorS10*)&q1, (const GXColorS10*)&q2);
            PaneMatSetTevColors(mSlotPane4, (const GXColorS10*)&q3, (const GXColorS10*)&q4);
            PaneMatSetTevColors(mSlotPane5, (const GXColorS10*)&q1, (const GXColorS10*)&q2);
            PaneMatSetTevColors(mSlotPane6, (const GXColorS10*)&q3, (const GXColorS10*)&q4);
            PaneMatSetTevColors(mSlotPane7, (const GXColorS10*)&q1, (const GXColorS10*)&q2);
            PaneMatSetTevColors(mSlotPane8, (const GXColorS10*)&q3, (const GXColorS10*)&q4);
            PaneMatSetTevColors(mSlotPane9, (const GXColorS10*)&q1, (const GXColorS10*)&q2);
            PaneMatSetTevColors(mSlotPane10, (const GXColorS10*)&q3, (const GXColorS10*)&q4);
            PaneMatSetTevColors(mSlotPane11, (const GXColorS10*)&q1, (const GXColorS10*)&q2);
            PaneMatSetTevColors(mSlotPane12, (const GXColorS10*)&q3, (const GXColorS10*)&q4);
            PaneMatSetTevColors(mSlotPane13, (const GXColorS10*)&q1, (const GXColorS10*)&q2);
            PaneMatSetTevColors(mSlotPane14, (const GXColorS10*)&q3, (const GXColorS10*)&q4);
            PaneMatSetTevColors(mSlotPane15, (const GXColorS10*)&q1, (const GXColorS10*)&q2);
            PaneMatSetTevColors(mSlotPane16, (const GXColorS10*)&q3, (const GXColorS10*)&q4);
            PaneMatSetTevColors(mSlotPane17, (const GXColorS10*)&q1, (const GXColorS10*)&q2);
            PaneMatSetTevColors(mSlotPane18, (const GXColorS10*)&q3, (const GXColorS10*)&q4);
            PaneMatSetTevColors(mSlotPane19, (const GXColorS10*)&q1, (const GXColorS10*)&q2);

        }
        // ---- simple path ----
        D8EQuad q1 = *(D8EQuad*)&itemBoxInfo_slotValueTop;
        D8EQuad q2 = *(D8EQuad*)&itemBoxInfo_slotValueBottom;
        D8EQuad q3 = *(D8EQuad*)&itemBoxInfo_nameValueTop;
        D8EQuad q4 = *(D8EQuad*)&itemBoxInfo_nameValueBottom;
        SetLayoutSelfText((nw4r::lyt::Layout*)mSlotPane0, "", 0);
        SetLayoutSelfText((nw4r::lyt::Layout*)mSlotPane2, "", 0);
        SetLayoutSelfText((nw4r::lyt::Layout*)mSlotPane3, "", 0);
        PaneMatSetTevColors(mSlotPane1, (const GXColorS10*)&q1, (const GXColorS10*)&q2);
        SetLayoutSelfText((nw4r::lyt::Layout*)mSlotPane4, "", 0);
        PaneMatSetTevColors(mSlotPane4, (const GXColorS10*)&q3, (const GXColorS10*)&q4);
        PaneMatSetTevColors(mSlotPane5, (const GXColorS10*)&q1, (const GXColorS10*)&q2);
        SetLayoutSelfText((nw4r::lyt::Layout*)mSlotPane6, "", 0);
        PaneMatSetTevColors(mSlotPane6, (const GXColorS10*)&q3, (const GXColorS10*)&q4);
        PaneMatSetTevColors(mSlotPane7, (const GXColorS10*)&q1, (const GXColorS10*)&q2);
        SetLayoutSelfText((nw4r::lyt::Layout*)mSlotPane8, "", 0);
        PaneMatSetTevColors(mSlotPane8, (const GXColorS10*)&q3, (const GXColorS10*)&q4);
        PaneMatSetTevColors(mSlotPane9, (const GXColorS10*)&q1, (const GXColorS10*)&q2);
        SetLayoutSelfText((nw4r::lyt::Layout*)mSlotPane10, "", 0);
        PaneMatSetTevColors(mSlotPane10, (const GXColorS10*)&q3, (const GXColorS10*)&q4);
        PaneMatSetTevColors(mSlotPane11, (const GXColorS10*)&q1, (const GXColorS10*)&q2);
        SetLayoutSelfText((nw4r::lyt::Layout*)mSlotPane12, "", 0);
        PaneMatSetTevColors(mSlotPane12, (const GXColorS10*)&q3, (const GXColorS10*)&q4);
        PaneMatSetTevColors(mSlotPane13, (const GXColorS10*)&q1, (const GXColorS10*)&q2);
        SetLayoutSelfText((nw4r::lyt::Layout*)mSlotPane14, "", 0);
        PaneMatSetTevColors(mSlotPane14, (const GXColorS10*)&q3, (const GXColorS10*)&q4);
        PaneMatSetTevColors(mSlotPane15, (const GXColorS10*)&q1, (const GXColorS10*)&q2);
        SetLayoutSelfText((nw4r::lyt::Layout*)mSlotPane16, "", 0);
        PaneMatSetTevColors(mSlotPane16, (const GXColorS10*)&q3, (const GXColorS10*)&q4);
        PaneMatSetTevColors(mSlotPane17, (const GXColorS10*)&q1, (const GXColorS10*)&q2);
        SetLayoutSelfText((nw4r::lyt::Layout*)mSlotPane18, "", 0);
        PaneMatSetTevColors(mSlotPane18, (const GXColorS10*)&q3, (const GXColorS10*)&q4);
        PaneMatSetTevColors(mSlotPane19, (const GXColorS10*)&q1, (const GXColorS10*)&q2);
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
        s16 weaponId = ((CItemBoxCharEntry*)charObj)->slotIds[5];
        s16 headId = ((CItemBoxCharEntry*)charObj)->slotIds[0];
        s16 torsoId = ((CItemBoxCharEntry*)charObj)->slotIds[1];
        s16 armId = ((CItemBoxCharEntry*)charObj)->slotIds[2];
        s16 legId = ((CItemBoxCharEntry*)charObj)->slotIds[3];
        s16 footId = ((CItemBoxCharEntry*)charObj)->slotIds[4];
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
                *(const EquippedTypes*)itemBoxInfoEquippedTypeIds;
            s16 equippedIds[6] = {
                ((CItemBoxCharEntry*)charObj)->slotIds[5], ((CItemBoxCharEntry*)charObj)->slotIds[0],
                ((CItemBoxCharEntry*)charObj)->slotIds[1], ((CItemBoxCharEntry*)charObj)->slotIds[2],
                ((CItemBoxCharEntry*)charObj)->slotIds[3], ((CItemBoxCharEntry*)charObj)->slotIds[4]
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
            } else if (BdatGetU8Direct((const void*)::cf::CfBdat::spBtlSkillListFileData, "accum", id) != 0) {
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
            } else if (BdatGetU8Direct((const void*)::cf::CfBdat::spBtlSkillListFileData, "accum", id) != 0) {
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
                u32 cap = BdatGetU16Direct(::cf::CfBdat::spBtlSkillListFileData,
                                       "max", ids[i]);
                if (vals[i] > cap) vals[i] = cap;
            }
            if (ids2[i] != 0) {
                u32 cap = BdatGetU16Direct(::cf::CfBdat::spBtlSkillListFileData, "max", ids2[i]);
                if (vals2[i] > cap) vals2[i] = cap;
            }
        }

        // better/worse flags comparing listB vs listA
        u8 cmp[8] = {0};
        for (u8 i = 0; i < 8; i++) {
            u16 compareId = ids2[i];
            s32 compareValue = (s32)vals2[i];
            if (compareId != 0 && BdatGetU8Direct((const void*)::cf::CfBdat::spBtlSkillListFileData, "accum", compareId) != 0) {
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
            if (ids2[i] != 0 && BdatGetU8Direct((const void*)::cf::CfBdat::spBtlSkillListFileData, "accum", ids2[i]) != 0) {
                for (u8 j = i + 1; j < 8; j++) {
                    if (ids2[i] == ids2[j]) ids2[j] = 0;
                }
            }
        }

        // pane name setup
        nw4r::lyt::Pane* root = mpLayout->GetRootPane()->FindPaneByName("nul_proportion", true);
        for (u8 i = 0; i < 8; i++) {
            char buf1[0x20];
            char buf2[0x20];
            sprintf(buf1, "txt_para7%02d", i * 2 + 0x13);
            sprintf(buf2, "txt_value7%02d", i + 0x10);
            LayoutSetTextBoxFmtValue(mpLayout, buf1, "", 0);
            LayoutSetTextBoxFmtValue(mpLayout, buf2, "", 0);
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
            sprintf(buf1, "txt_para7%02d", i * 2 + 0x13);
            sprintf(buf2, "txt_value7%02d", i + 0x10);
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
            LayoutSetTextBoxFmtValue(mpLayout, buf1, BdatGetPtrDirect(::cf::CfBdat::spBtlSkillListFileData, "name", id), 0);
            if (val == 0) {
                LayoutSetTextBoxFmtValue(mpLayout,
                              buf2, "", 0);
            }
            u8 rvs = BdatGetU8Direct((const void*)::cf::CfBdat::spBtlSkillListFileData,
                                       "rvs_type", id);
            if (rvs != 0) {
                if (rvs == 0xFF || rvs == 0xFE) {
                    textBuffer.format("%d%s", (s32)val,
                        BdatTouchStringCell("MNU_item",
                                      "name", 0x21));
                } else {
                    textBuffer.format("%d ", (s32)val);
                }
            } else {
                textBuffer.format("%d ", (s32)val);
            }
            LayoutSetTextBoxFmtValue(mpLayout,
                          buf2, textBuffer.mString, 0);
            if (arg3 != NULL && arg4 == 0 &&
                (cf != 0 || type == 2 || !TestBdatFlag(member, arg3))) {
                cf = 0;
                LayoutSetTextBoxFmtValue(mpLayout,
                              buf2, "", 0);
            }
            D8EQuad q1 = *(D8EQuad*)&itemBoxInfo_labelNeutralTop;
            D8EQuad q2 = *(D8EQuad*)&itemBoxInfo_labelNeutralBottom;
            D8EQuad q3 = *(D8EQuad*)&itemBoxInfo_valueNeutralTop;
            D8EQuad q4 = *(D8EQuad*)&itemBoxInfo_valueNeutralBottom;
            u32 cap = BdatGetU16Direct(::cf::CfBdat::spBtlSkillListFileData, "max", id);
            if ((u32)val >= cap) {
                __as__11_GXColorS10FRC11_GXColorS10(&q1, &itemBoxInfo_labelSpecialTop);
                __as__11_GXColorS10FRC11_GXColorS10(&q2, &itemBoxInfo_labelSpecialBottom);
                __as__11_GXColorS10FRC11_GXColorS10(&q3, &itemBoxInfo_valueSpecialTop);
                __as__11_GXColorS10FRC11_GXColorS10(&q4, &itemBoxInfo_valueSpecialBottom);
            } else if (cf == 1) {
                __as__11_GXColorS10FRC11_GXColorS10(&q1, &itemBoxInfo_labelUpTop);
                __as__11_GXColorS10FRC11_GXColorS10(&q2, &itemBoxInfo_labelUpBottom);
                __as__11_GXColorS10FRC11_GXColorS10(&q3, &itemBoxInfo_valueUpTop);
                __as__11_GXColorS10FRC11_GXColorS10(&q4, &itemBoxInfo_valueUpBottom);
            } else if (cf == 2) {
                __as__11_GXColorS10FRC11_GXColorS10(&q1, &itemBoxInfo_labelDownTop);
                __as__11_GXColorS10FRC11_GXColorS10(&q2, &itemBoxInfo_labelDownBottom);
                __as__11_GXColorS10FRC11_GXColorS10(&q3, &itemBoxInfo_valueDownTop);
                __as__11_GXColorS10FRC11_GXColorS10(&q4, &itemBoxInfo_valueDownBottom);
            }
            if (arg4 == 0) {
                __as__11_GXColorS10FRC11_GXColorS10(&q1, &itemBoxInfo_slotValueTop);
                __as__11_GXColorS10FRC11_GXColorS10(&q2, &itemBoxInfo_slotValueBottom);
                __as__11_GXColorS10FRC11_GXColorS10(&q3, &itemBoxInfo_nameValueTop);
                __as__11_GXColorS10FRC11_GXColorS10(&q4, &itemBoxInfo_nameValueBottom);
            }
            PaneMatSetTevColorsByName(mpLayout, buf1, (const GXColorS10*)&q1, (const GXColorS10*)&q2);
            PaneMatSetTevColorsByName(mpLayout, buf2, (const GXColorS10*)&q3, (const GXColorS10*)&q4);
        }
    }
}


#undef w04
#undef _0C
#undef _10
#undef w14

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
    slotIds[0] = ((CItemBoxCharEntry*)lookup)->slotIds[5];
    slotIds[1] = ((CItemBoxCharEntry*)lookup)->slotIds[0];
    slotIds[2] = ((CItemBoxCharEntry*)lookup)->slotIds[1];
    slotIds[3] = ((CItemBoxCharEntry*)lookup)->slotIds[2];
    slotIds[4] = ((CItemBoxCharEntry*)lookup)->slotIds[3];
    slotIds[5] = ((CItemBoxCharEntry*)lookup)->slotIds[4];
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
    u16 cap = (u16)BdatGetU16Direct((char*)::cf::CfBdat::spBtlSkillListFileData, (char*)"max", category);
    if ((s32)cap < (s32)result) result = cap;
    return result;
}

// The slot argument maps 1..3 to type 2 with a sub-index, and 4..8 to direct types;
// the candidate pointer is nulled out by the first pass when one of its gems matches,
// so the sum pass treats it as unequipped.
u32 CItemBoxInfo::SumGemBonus(void* member, u32 category, void* candidate, s32 slot) {
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
    ids[0] = ((CItemBoxCharEntry*)lookup)->slotIds[5];
    ids[1] = ((CItemBoxCharEntry*)lookup)->slotIds[0];
    ids[2] = ((CItemBoxCharEntry*)lookup)->slotIds[1];
    ids[3] = ((CItemBoxCharEntry*)lookup)->slotIds[2];
    ids[4] = ((CItemBoxCharEntry*)lookup)->slotIds[3];
    ids[5] = ((CItemBoxCharEntry*)lookup)->slotIds[4];
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
    u16 max = (u16)BdatGetU16Direct(::cf::CfBdat::spBtlSkillListFileData, "max", category);
    if (max < sum) sum = max;
    return (u32)sum;
}
u32 CItemBoxInfo::GetCondStat(void* b, u32 arg2) {
    void* obj;
    u32 result = 0;
    obj = (u8*)CtrlObjectParam_GetCharEntry((u32)b) + 0x3534;
    if (Counter_TestBit(obj, arg2) != 0) {
        result = IdTable_SumValues((CUnkObj8025FB10*)obj, arg2);
    }
    return result;
}


u32 CItemBoxInfo::TestBdatFlag(u32 arg1, void* arg2) {
    if (arg1 == 0 || arg2 == 0) return 0;
    char buf[0x20];
    sprintf(buf, "pc%d", arg1);
    u32 g = (u32)::cf::CfBdat::spItmEquipListFileData;
    u8 result = BdatGetU8Direct((const void*)g, (const char*)buf, (u16)BdatGetItemId(((CItemData*)arg2)->field_00 >> 20));
    return result != 0;
}
u32 CItemBoxInfo::SumTypeCounts(u16 arg2, void* arg3) {
    if (arg2 == 0) return 0;
    void* g = ::cf::CfBdat::spItmEquipListFileData;
    u16 v1 = arg3 ? (u16)BdatGetItemType((u32)arg3) : 0;
    u16 v2 = arg3 ? (u16)BdatGetItemId((u32)arg3) : 0;
    void* lookup = CtrlObjectParam_GetCharEntry(arg2);
    u32 result = 0;
    for (u32 i = 4; i <= 8; i++) {
        u8 ii = (u8)i;
        if (ii == v1) continue;
        s16 val = -1;
        switch (ii) {
            case 4: val = ((CItemBoxCharEntry*)lookup)->slotIds[0]; break;
            case 5: val = ((CItemBoxCharEntry*)lookup)->slotIds[1]; break;
            case 6: val = ((CItemBoxCharEntry*)lookup)->slotIds[2]; break;
            case 7: val = ((CItemBoxCharEntry*)lookup)->slotIds[3]; break;
            case 8: val = ((CItemBoxCharEntry*)lookup)->slotIds[4]; break;
        }
        if (val == -1) continue;
        // Retail passes the slot id (u8)i and the slot value val (2-arg form).
        void* r = CItem_kindPtr((u8)i, val);
        if (r == NULL) continue;
        u32 v = ((CItemData*)r)->field_00;
        if (v == 0) continue;
        result += (u8)BdatGetU8Direct((const void*)g, "eva_rate", (u16)BdatGetItemId(v >> 20));
    }
    if (arg3 != NULL) {
        result += (u8)BdatGetU8Direct((const void*)g, "eva_rate", v2);
    }
    return result;
}
u32 CItemBoxInfo::CheckGemChain(u16 lookup_key, void* arg3, void* unused2) {
    void* lookup = CtrlObjectParam_GetCharEntry(lookup_key);
    s16 ids[6];
    // ids[0] is the halfword at lookup+0x26; the remaining five are the packed
    // block at 0x1C..0x24 (retail load order).
    ids[0] = ((CItemBoxCharEntry*)lookup)->slotIds[5];
    ids[1] = ((CItemBoxCharEntry*)lookup)->slotIds[0];
    ids[2] = ((CItemBoxCharEntry*)lookup)->slotIds[1];
    ids[3] = ((CItemBoxCharEntry*)lookup)->slotIds[2];
    ids[4] = ((CItemBoxCharEntry*)lookup)->slotIds[3];
    ids[5] = ((CItemBoxCharEntry*)lookup)->slotIds[4];
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

// Retail CItemBoxInfo::OnFileEvent: two file-load branches. The line-arc
// branch builds the whole layout (0x14000 scratch region), seeds every label
// textbox with the shared text object, then runs the item-box1 reset pass:
// pane clears gated on the mode byte (+0x9A), fixed label texts, per-mode
// caption, and the four vertex-colour blocks whose results are written both
// into this (+0x9C/+0xA4) and into the .sbss colour globals (518/520/558/
// 560/598/5A0/5B8/5C0) with their alpha halves propagated to the paired
// globals. Finally caches the 20 slot panes at +0x40..+0x8C.
bool CItemBoxInfo::OnFileEvent(CEventFile* file) {
    if (mFileHandle1 == file->mFileHandle) {
        mRegion1.createRegion(mtl::MemManager::getHandleMEM2(), 0x14000,
                                      "CItemBoxInfo", 1);
        Class_8045F858 host(&mRegion1);
        u8* data = (u8*)mFileHandle1->getData();
        mtl::MemManager::setMemInitFlag(false);
        nw4r::lyt::ArcResourceAccessor* acc = CLibLayout::createArcResourceAccessor();
        mArcResAcc = acc;
        acc->Attach(data, "arc");
        buildLayout(&mpLayout, acc, "mf02_box02_inf.brlyt");
        bindLayoutAnimTransform(mpLayout, &mAnimIn, acc, "mf02_box02_inf_in.brlan");
        bindLayoutAnimTransform(mpLayout, &mAnimInfoIn, acc, "mf02_box02_inf_info_in.brlan");
        nw4r::lyt::Pane* root = mpLayout->GetRootPane();
        void* fontObj = CDeviceFont::getFontInfo(1, mpLayout);
        ApplyFontToTxtPanes(root, (const nw4r::ut::Font*)(static_cast<IDeviceFontInfo*>(fontObj)->getFont()));

        // Seed the label textboxes with the shared text object.
        char* text = (char*)CUICfManager_getPackedFont9C();
        setLayoutTextBoxFont(mpLayout, "txt_value02", (u32)text);
        setLayoutTextBoxFont(mpLayout, "txt_value08", (u32)text);
        setLayoutTextBoxFont(mpLayout, "txt_value09", (u32)text);
        setLayoutTextBoxFont(mpLayout, "txt_value04", (u32)text);
        setLayoutTextBoxFont(mpLayout, "txt_value07", (u32)text);
        setLayoutTextBoxFont(mpLayout, "txt_value22", (u32)text);
        setLayoutTextBoxFont(mpLayout, "txt_value50", (u32)text);
        setLayoutTextBoxFont(mpLayout, "txt_value51", (u32)text);
        setLayoutTextBoxFont(mpLayout, "txt_value30", (u32)text);
        setLayoutTextBoxFont(mpLayout, "txt_value31", (u32)text);
        setLayoutTextBoxFont(mpLayout, "txt_value32", (u32)text);
        setLayoutTextBoxFont(mpLayout, "txt_value33", (u32)text);
        setLayoutTextBoxFont(mpLayout, "txt_value34", (u32)text);
        setLayoutTextBoxFont(mpLayout, "txt_ethvalue01", (u32)text);
        setLayoutTextBoxFont(mpLayout, "txt_ethvalue02", (u32)text);
        setLayoutTextBoxFont(mpLayout, "txt_ethvalue03", (u32)text);
        setLayoutTextBoxFont(mpLayout, "txt_gold01_00", (u32)text);
        setLayoutTextBoxFont(mpLayout, "txt_value700", (u32)text);
        setLayoutTextBoxFont(mpLayout, "txt_value702", (u32)text);
        setLayoutTextBoxFont(mpLayout, "txt_value703", (u32)text);
        setLayoutTextBoxFont(mpLayout, "txt_value704", (u32)text);
        setLayoutTextBoxFont(mpLayout, "txt_value705", (u32)text);
        setLayoutTextBoxFont(mpLayout, "txt_value708", (u32)text);
        setLayoutTextBoxFont(mpLayout, "txt_value710", (u32)text);
        setLayoutTextBoxFont(mpLayout, "txt_value706", (u32)text);
        setLayoutTextBoxFont(mpLayout, "txt_value709", (u32)text);
        setLayoutTextBoxFont(mpLayout, "txt_value712", (u32)text);
        setLayoutTextBoxFont(mpLayout, "txt_value714", (u32)text);
        setLayoutTextBoxFont(mpLayout, "txt_value715", (u32)text);
        setLayoutTextBoxFont(mpLayout, "txt_value716", (u32)text);
        setLayoutTextBoxFont(mpLayout, "txt_value717", (u32)text);
        setLayoutTextBoxFont(mpLayout, "txt_value718", (u32)text);
        setLayoutTextBoxFont(mpLayout, "txt_value719", (u32)text);
        setLayoutTextBoxFont(mpLayout, "txt_value720", (u32)text);
        setLayoutTextBoxFont(mpLayout, "txt_value721", (u32)text);
        setLayoutTextBoxFont(mpLayout, "txt_value722", (u32)text);
        setLayoutTextBoxFont(mpLayout, "txt_value723", (u32)text);
        setLayoutTextBoxFont(mpLayout, "txt_para704", (u32)text);
        setLayoutTextBoxFont(mpLayout, "txt_scnd01", (u32)text);
        setLayoutTextBoxFont(mpLayout, "txt_scnd02", (u32)text);
        setLayoutTextBoxFont(mpLayout, "txt_scnd03", (u32)text);

        mpLayout->SetAnimationEnable(mAnimInfoIn, false);
        mpLayout->SetAnimationEnable(mAnimIn, true);
        mpLayout->Animate(0);

        u8 mode = mKind;
        if (mode != 1 && mode != 2) {
            setPaneVisible(root->FindPaneByName("nul_shop", true), 0);
        }
        if (mode != 3) {
            setPaneVisible(root->FindPaneByName("nul_excange", true), 0);
        }

        // Fixed label texts for the closed item-box view.
        LayoutSetTextBoxFmtValue((nw4r::lyt::Layout*)mpLayout, "txt_para01",
                      BdatTouchStringCell("MNU_item", "name", 0xa), 0);
        LayoutSetTextBoxFmtValue((nw4r::lyt::Layout*)mpLayout, "txt_para04",
                      BdatTouchStringCell("MNU_item", "name", 0xd), 0);
        LayoutSetTextBoxFmtValue((nw4r::lyt::Layout*)mpLayout, "txt_para08",
                      BdatTouchStringCell("MNU_item", "name", 0x11), 0);
        LayoutSetTextBoxFmtValue((nw4r::lyt::Layout*)mpLayout, "txt_para10",
                      BdatTouchStringCell("MNU_item", "name", 0x12), 0);
        LayoutSetTextBoxFmtValue((nw4r::lyt::Layout*)mpLayout, "txt_para11",
                      BdatTouchStringCell("MNU_item", "name", 0xc), 0);
        LayoutSetTextBoxFmtValue((nw4r::lyt::Layout*)mpLayout, "txt_para22",
                      BdatTouchStringCell("MNU_item", "name", 0x13), 0);
        LayoutSetTextBoxFmtValue((nw4r::lyt::Layout*)mpLayout, "txt_para30",
                      BdatTouchStringCell("MNU_item", "name", 0x18), 0);
        LayoutSetTextBoxFmtValue((nw4r::lyt::Layout*)mpLayout, "txt_para50",
                      BdatTouchStringCell("MNU_item", "name", 0x18), 0);
        LayoutSetTextBoxFmtValue((nw4r::lyt::Layout*)mpLayout, "txt_para60",
                      BdatTouchStringCell("MNU_item", "name", 0x24), 0);
        LayoutSetTextBoxFmtValue((nw4r::lyt::Layout*)mpLayout, "txt_para61",
                      BdatTouchStringCell("MNU_item", "name", 0x25), 0);
        LayoutSetTextBoxFmtValue((nw4r::lyt::Layout*)mpLayout, "txt_para700",
                      BdatTouchStringCell("MNU_item", "name", 0x70), 0);
        LayoutSetTextBoxFmtValue((nw4r::lyt::Layout*)mpLayout, "txt_para702",
                      BdatTouchStringCell("MNU_item", "name", 0x72), 0);
        LayoutSetTextBoxFmtValue((nw4r::lyt::Layout*)mpLayout, "txt_para703",
                      BdatTouchStringCell("MNU_item", "name", 0x73), 0);
        LayoutSetTextBoxFmtValue((nw4r::lyt::Layout*)mpLayout, "txt_para705",
                      BdatTouchStringCell("MNU_item", "name", 0x74), 0);
        LayoutSetTextBoxFmtValue((nw4r::lyt::Layout*)mpLayout, "txt_para706",
                      BdatTouchStringCell("MNU_item", "name", 0x79), 0);
        LayoutSetTextBoxFmtValue((nw4r::lyt::Layout*)mpLayout, "txt_para708",
                      BdatTouchStringCell("MNU_item", "name", 0x75), 0);
        LayoutSetTextBoxFmtValue((nw4r::lyt::Layout*)mpLayout, "txt_para709",
                      BdatTouchStringCell("MNU_item", "name", 0x7b), 0);
        LayoutSetTextBoxFmtValue((nw4r::lyt::Layout*)mpLayout, "txt_para710",
                      BdatTouchStringCell("MNU_item", "name", 0x76), 0);
        LayoutSetTextBoxFmtValue((nw4r::lyt::Layout*)mpLayout, "txt_para713",
                      BdatTouchStringCell("MNU_item", "name", 0x77), 0);
        LayoutSetTextBoxFmtValue((nw4r::lyt::Layout*)mpLayout, "txt_para716",
                      BdatTouchStringCell("MNU_item", "name", 0x78), 0);
        LayoutSetTextBoxFmtValue((nw4r::lyt::Layout*)mpLayout, "txt_para717",
                      BdatTouchStringCell("MNU_item", "name", 0x7e), 0);
        LayoutSetTextBoxFmtValue((nw4r::lyt::Layout*)mpLayout, "txt_para23",
                      BdatTouchStringCell("MNU_item", "name", 0x2f), 0);

        // Per-mode caption: modes 0/1 show the owned-item count; mode 3 shows
        // the category name resolved from the selected item id.
        if ((s8)mode <= 1) {
            ml::FixStr<32> caption(true);
            caption.format("%d%s", CItemBlock_getPtr20E8(),
                           BdatTouchStringCell("MNU_item", "name", 3));
            LayoutSetTextBoxFmtValue((nw4r::lyt::Layout*)mpLayout, "txt_gold01_00",
                          caption.c_str(), 0);
        } else if (mode == 3) {
            u16 itemId = mMode;
            u8 kind = (u8)MapSexFlagGroup(itemId);
            char* s;
            if (kind == 0) {
                s = BdatTouchStringCell("MNU_item", "name", 0x24);
            } else {
                s = BdatTouchStringCell("MNU_item", "name", kind + 0x1F);
            }
            LayoutSetTextBoxFmtValue((nw4r::lyt::Layout*)mpLayout, "txt_npctype", s, 0);
            char* name = ((char* (*)(void*, const char*, u16))&BdatGetPtrDirect)(
                &::cf::CfBdat::spFldNpcListFileData, "name", itemId);
            LayoutSetTextBoxFmtValue((nw4r::lyt::Layout*)mpLayout, "txt_npcname",
                          GetBdatNameByNumId(name), 0);
            LayoutSetTextBoxFmtValue((nw4r::lyt::Layout*)mpLayout, "txt_npcframe",
                          BdatTouchStringCell("MNU_item", "name", 0x2d), 0);
            LayoutSetTextBoxFmtValue((nw4r::lyt::Layout*)mpLayout, "txt_excange00",
                          "", 0);
            LayoutSetTextBoxFmtValue((nw4r::lyt::Layout*)mpLayout, "txt_excange01",
                          "", 0);
        }

        // Vertex colours: the highlight pair goes onto this (+0x9C/+0xA4); the
        // three named panes' pairs go into the .sbss colour globals with their
        // alpha halves propagated to the paired globals.
        nw4r::lyt::Pane* hl = root->FindPaneByName("pic_pcbs01", true);
        GXColorS10 c1 = PaneMatGetS16Quad(hl, 0);
        __as__11_GXColorS10FRC11_GXColorS10(&mColorA, &c1);
        GXColorS10 c2 = PaneMatGetS16Quad(hl, 1);
        __as__11_GXColorS10FRC11_GXColorS10(&mColorB, &c2);

        nw4r::lyt::Pane* p71e = root->FindPaneByName("txt_para700", true);
        GXColorS10 c3 = PaneMatGetS16Quad(p71e, 0);
        __as__11_GXColorS10FRC11_GXColorS10(&itemBoxInfo_labelNeutralTop, &c3);
        GXColorS10 c4 = PaneMatGetS16Quad(p71e, 1);
        __as__11_GXColorS10FRC11_GXColorS10(&itemBoxInfo_labelNeutralBottom, &c4);
        {
            s16 a0 = *(s16*)((char*)&itemBoxInfo_labelNeutralTop + 6);
            s16 a1 = *(s16*)((char*)&itemBoxInfo_labelNeutralBottom + 6);
            *(s16*)((char*)&itemBoxInfo_labelUpTop + 6) = a0;
            *(s16*)((char*)&itemBoxInfo_labelUpBottom + 6) = a1;
            *(s16*)((char*)&itemBoxInfo_labelDownTop + 6) = a0;
            *(s16*)((char*)&itemBoxInfo_labelDownBottom + 6) = a1;
            *(s16*)((char*)&itemBoxInfo_labelSpecialTop + 6) = a0;
            *(s16*)((char*)&itemBoxInfo_labelSpecialBottom + 6) = a1;
        }

        nw4r::lyt::Pane* p4e3 = root->FindPaneByName("txt_value700", true);
        GXColorS10 c5 = PaneMatGetS16Quad(p4e3, 0);
        __as__11_GXColorS10FRC11_GXColorS10(&itemBoxInfo_valueNeutralTop, &c5);
        GXColorS10 c6 = PaneMatGetS16Quad(p4e3, 1);
        __as__11_GXColorS10FRC11_GXColorS10(&itemBoxInfo_valueNeutralBottom, &c6);

        nw4r::lyt::Pane* p2b6 = root->FindPaneByName("txt_para01", true);
        GXColorS10 c7 = PaneMatGetS16Quad(p2b6, 0);
        __as__11_GXColorS10FRC11_GXColorS10(&itemBoxInfo_slotLabelTop, &c7);
        GXColorS10 c8 = PaneMatGetS16Quad(p2b6, 1);
        __as__11_GXColorS10FRC11_GXColorS10(&itemBoxInfo_slotLabelBottom, &c8);
        *(s16*)((char*)&itemBoxInfo_slotValueTop + 6) = *(s16*)((char*)&itemBoxInfo_slotLabelTop + 6);
        *(s16*)((char*)&itemBoxInfo_slotValueBottom + 6) = *(s16*)((char*)&itemBoxInfo_slotLabelBottom + 6);

        nw4r::lyt::Pane* p25b = root->FindPaneByName("txt_value02", true);
        GXColorS10 c9 = PaneMatGetS16Quad(p25b, 0);
        __as__11_GXColorS10FRC11_GXColorS10(&itemBoxInfo_nameLabelTop, &c9);
        GXColorS10 c10 = PaneMatGetS16Quad(p25b, 1);
        __as__11_GXColorS10FRC11_GXColorS10(&itemBoxInfo_nameLabelBottom, &c10);
        *(s16*)((char*)&itemBoxInfo_nameValueTop + 6) = *(s16*)((char*)&itemBoxInfo_nameLabelTop + 6);
        *(s16*)((char*)&itemBoxInfo_nameValueBottom + 6) = *(s16*)((char*)&itemBoxInfo_nameLabelBottom + 6);

        StampSlotTex();

        LayoutSetTextBoxFmtValue((nw4r::lyt::Layout*)mpLayout, "txt_scnd00",
                      BdatTouchStringCell("MNU_item", "name", 6), 0);
        LayoutSetTextBoxFmtValue((nw4r::lyt::Layout*)mpLayout, "txt_scnd02",
                      BdatTouchStringCell("MNU_item", "name", 9), 0);
        setLayoutTextBoxNumber((nw4r::lyt::Layout*)mpLayout, "txt_scnd01", (u8)0);
        setLayoutTextBoxNumber((nw4r::lyt::Layout*)mpLayout, "txt_scnd03", (u8)0);
        setPaneVisible(root->FindPaneByName("nul_scnd", true), 1);

        // Cache the slot panes at +0x40..+0x8C for the renderer (retail keeps
        // the layout's root pane live in r28 across all 20 lookups).
        nw4r::lyt::Pane* cacheRoot = mpLayout->GetRootPane();
        mSlotPane0 = cacheRoot->FindPaneByName("txt_value703", true);
        mSlotPane1 = cacheRoot->FindPaneByName("txt_para703", true);
        mSlotPane2 = cacheRoot->FindPaneByName("txt_value704", true);
        mSlotPane3 = cacheRoot->FindPaneByName("txt_para704", true);
        mSlotPane4 = cacheRoot->FindPaneByName("txt_value705", true);
        mSlotPane5 = cacheRoot->FindPaneByName("txt_para705", true);
        mSlotPane6 = cacheRoot->FindPaneByName("txt_value708", true);
        mSlotPane7 = cacheRoot->FindPaneByName("txt_para708", true);
        mSlotPane8 = cacheRoot->FindPaneByName("txt_value710", true);
        mSlotPane9 = cacheRoot->FindPaneByName("txt_para710", true);
        mSlotPane10 = cacheRoot->FindPaneByName("txt_value706", true);
        mSlotPane11 = cacheRoot->FindPaneByName("txt_para706", true);
        mSlotPane12 = cacheRoot->FindPaneByName("txt_value712", true);
        mSlotPane13 = cacheRoot->FindPaneByName("txt_para713", true);
        mSlotPane14 = cacheRoot->FindPaneByName("txt_value714", true);
        mSlotPane15 = cacheRoot->FindPaneByName("txt_para716", true);
        mSlotPane16 = cacheRoot->FindPaneByName("txt_value709", true);
        mSlotPane17 = cacheRoot->FindPaneByName("txt_para709", true);
        mSlotPane18 = cacheRoot->FindPaneByName("txt_value715", true);
        mSlotPane19 = cacheRoot->FindPaneByName("txt_para717", true);

        Activate();
        mFileHandle1 = 0;
        mRegion1.validateHeap();
        return true;
    }
    if (mFileHandle2 == file->mFileHandle) {
        mRegion2.createRegion(mtl::MemManager::getHandleMEM2(), 0x100,
                                      "CItemBoxInfoTex", 1);
        Class_8045F858 host(&mRegion2);
        u8* data = (u8*)mFileHandle2->getData();
        mtl::MemManager::setMemInitFlag(false);
        nw4r::lyt::ArcResourceAccessor* acc2 = CLibLayout::createArcResourceAccessor();
        mTexArcResAcc = acc2;
        acc2->Attach(data, "arc");
        Activate();
        mFileHandle2 = 0;
        mRegion2.validateHeap();
        return true;
    }
    return false;
}

// Retail __ct__CItemBoxInfo2 uses the stmw/lmw frame prologue merge) with arg2/arg3
// saved in r30/r31; plain -O4,p splits the saves into separate stw's.
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

// Same pattern as the matched kyoshin dtors.
CItemBoxInfo2::~CItemBoxInfo2() {}

void CItemBoxInfo2::LoadFiles() {
    mtl::ALLOC_HANDLE handle = mtl::MemManager::getHandleMEM2();
    mFileHandle1 = CDeviceFile::readFile(handle, "menu/jp/ItemBoxInfo.arc", reinterpret_cast<IWorkEvent*>(this), 0, 0);
    handle = mtl::MemManager::getHandleMEM2();
    mFileHandle2 = CDeviceFile::readFile(handle, "menu/jp/tpl/CrystalIcon.arc", reinterpret_cast<IWorkEvent*>(this), 0, 0);
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


void CItemBoxInfo2::DrawLayout(nw4r::lyt::DrawInfo* drawInfo) {
    if (mActive != 0) {
        drawLayout((nw4r::lyt::Layout*)mpLayout, drawInfo, 0, 1);
    }
}

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

u32 CItemBoxInfo2::GetState() {
    return mCurrent;
}


void CItemBoxInfo2::Open() {
    if (mState != 0) return;
    mState = 1;
    mVisible = 0;
    StampSlotTex();
}

void CItemBoxInfo2::Close() {
    if (mState == 3) {
        mState = 4;
        mVisible = 0;
    }
}

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
    char* s = BdatTouchStringCell("MNU_item", "name", 3);
    // Not named textBuffer: the textBuffer raw-storage macro from
    // setupItemBoxDetail is still in scope until its #undef below.
    ml::FixStr<32> caption;
    caption.format("%d%s", CItemBlock_getPtr20E8(), s);
    LayoutSetTextBoxFmtValue((nw4r::lyt::Layout*)mpLayout, "txt_gold01_00", caption.c_str(), 0);
}
void CItemBoxInfo2::SetNamedText(int index, char* text) {
    char buf[0x20];
    sprintf(buf, "txt_excange%02d", index);
    LayoutSetTextBoxFmtValue((nw4r::lyt::Layout*)mpLayout, buf, text, 0);
}

// Retail calcItemBox2PaneVec (CItemBoxInfo2 variant) uses the same stmw/lmw frame.
nw4r::math::VEC3 CItemBoxInfo2::GetSlotPos(u32 arg2) {
    nw4r::math::VEC3 pos;
    char buf[0x20];
    // Same declaration-order trick as calcItemBoxPaneVec: `first` before `child`
    // so child colors into dead this's r30 and first reuses base's r31.
    nw4r::lyt::Pane* second;
    nw4r::lyt::Pane* first;
    nw4r::lyt::Pane* child;
    sprintf(buf, "pic_pcbs%02d", arg2 + 1);
    child = ((LayoutRootPaneView*)mpLayout)->rootPane10;
    first = child->FindPaneByName("nul_proportion", true);
    second = child->FindPaneByName(buf, true);
    ScalePaneWorldPos(&pos, second, first, child);
    return pos;
}
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

// On a null item it zeroes the 0x34-byte record and copies it out; otherwise it builds
// the record like buildItemBoxCmpRecord (same lookup chain, clamp and per-slot flag
// banks) and copies it out.
CItemBoxCompRecord CItemBoxInfo2::BuildCmpRecord(void* arg3) {
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
        void* global = ::cf::CfBdat::spItmWpnListFileData;
        BdatGetItemType((u32)arg3);
        u32 v2 = BdatGetItemId((u32)arg3);
        rec.s2C = (u16)BdatGetU16Direct((const void*)global, (const char*)((const void*)("dmg_low")), (int)(u16)v2);
        rec.s2E = (u16)BdatGetU16Direct((const void*)global, (const char*)((const void*)("dmg_hi")), (int)(u16)v2);
        rec.s30 = (u8)BdatGetU8Direct((const void*)global, "arm_phy", (u16)v2);
        rec.s32 = (u8)BdatGetU8Direct((const void*)global, "arm_eth", (u16)v2);
        rec.s34 = (u8)BdatGetU8Direct((const void*)global, "att_lev", (u16)v2);
        u1.w[1] = (u8)BdatGetU8Direct((const void*)global, "speed", (u16)v2);
        rec.f38 = (f32)(u1.d - 4503599627370496.0) / 10.0f;
        rec.s3C = (u8)BdatGetU8Direct((const void*)global, "grd_rate", (u16)v2);
        if (BdatGetU8Direct((const void*)global, "flag", (u16)v2) & 4) {
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
            sprintf(buf, "equip_pc%d", slot);
            u8 v = (u8)BdatGetU8Direct((const void*)global, (const char*)buf, (u16)v2);
            // +0x16 flag bank: slot has a resolvable entry at all.
            rec.flags[0][(u8)i] = (v != 0);
            if (rec.flags[0][(u8)i] == 0) continue;
            // Accumulate the slot's stat contribution into the ratio total.
            rec.f38 += GetFloatTableEntry(slot);
            void* lookup = CtrlObjectParam_GetCharEntry(slot);
            void* item = CItem_kindPtr(2, ((CItemBoxCharEntry*)lookup)->slotIds[5]);
            if (item == NULL || *(u32*)item == 0) continue;
            u16 cat = (u16)BdatGetItemId(*(u32*)item >> 20);
            s16 n1 = (u16)BdatGetU16Direct((const void*)global, (const char*)((const void*)("dmg_low")), (int)cat);
            s16 n2 = (u16)BdatGetU16Direct((const void*)global, (const char*)((const void*)("dmg_hi")), (int)cat);
            u2.w[1] = (u8)BdatGetU8Direct((const void*)global, "speed", cat);
            f32 slotRatio =
                BlendFloatAvgScale(n1, n2, 0, (f32)(u2.d - 4503599627370496.0) / 10.0f);
            u8 v1ab = (u8)BdatGetU8Direct((const void*)global, "arm_phy", cat);
            u8 v1b3 = (u8)BdatGetU8Direct((const void*)global, "arm_eth", cat);
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
// Retail buildItemBox2SlotFlags builds the 0x1C-byte comparison record like
// buildItemBoxSlotFlags, with two differences: the per-slot flag is the boolean (name
// resolved ? 1 : 0) via subic/subfe, and when the category check (0x85/0x84) SUCCEEDS
// the value-switch path still runs (the switch body is shared through the else-block
// fall-through, so it is emitted once). buildItemBoxSlotFlags, with two differences:
// the per-slot flag is the boolean (name resolved ? 1 : 0) via subic/subfe, and when
// the category check (0x85/0x84) SUCCEEDS the value-switch path still runs (the switch
// body is shared through the else-block fall-through, so it is emitted once).
CItemBoxSlotFlags CItemBoxInfo2::BuildSlotFlags(u32 item) {
    CItemBoxSlotFlags rec;
    if (item == 0) {
        memset(&rec, 0, 0x1C);
        return rec;
    }
    void* global = ::cf::CfBdat::spItmEquipListFileData;
    u16 type = BdatGetItemType(item);
    u16 id = BdatGetItemId(item);
    rec.v[0] = (u8)BdatGetU8Direct((const void*)global, "arm_phy", id);
    rec.v[1] = (u8)BdatGetU8Direct((const void*)global, "arm_eth", id);
    rec.v[2] = (u8)BdatGetU8Direct((const void*)global, "eva_rate", id);
    u32 reqKind = BdatGetU8Direct((const void*)::cf::CfBdat::spItmEquipListFileData, "arm_type", id);
    for (u32 i = 0; (u8)i < (u8)code80135FDC_getByte_64077(); i++) {
        u8 slot = GetCollectedFlagByte((u8)i);
        char buf[0x20];
        sprintf(buf, "pc%d", (u8)slot);
        rec.equipable[(u8)i] = (u8)BdatGetU8Direct((const void*)::cf::CfBdat::spItmEquipListFileData, (const char*)buf, id) != 0;
        if (rec.equipable[(u8)i] == 0) continue;
        CItemBoxCharEntry* member = (CItemBoxCharEntry*)CtrlObjectParam_GetCharEntry((u8)slot);
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
                    u8 n1 = (u8)BdatGetU8Direct((const void*)global, "arm_phy", cat);
                    u8 n2 = (u8)BdatGetU8Direct((const void*)global, "arm_eth", cat);
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
// Retail buildItemBox2NameRec (ItemBox2 twin of buildItemBoxNameRec): builds the
// 0xA4-byte item-name record (count byte at +0, label string pointer, name buffer, two
// state bytes, equip colour, text buffer) then copies it out. The label table lives in
// a 0x34-byte stack region that is later copied into the record tail, so the 6-pair
// mtctr copy loop reads it directly.
CItemBoxNameRecord CItemBoxInfo2::BuildNameRec(void* data, void* arg3) {
    void* item = arg3 != NULL ? arg3 : NULL;
    // Retail hoists this global into a register (r26) across all the calls;
    // a local copy reproduces the single sda21 load in the prologue.
    void* gd8 = ::cf::CfBdat::spBtlSkillListFileData;
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
        count = (u8)BdatGetU8Direct((const void*)::cf::CfBdat::spItmItemListFileData, "rankType", (u32)data);
    }
    // base is only materialized after the cat/count joins (retail lis/addi at
    // the join point, not in the prologue).
    CItemBoxNameRecord rec;
    rec.count = (u8)count;
    rec.str = (u32)BdatTouchStringCell("MNU_item", "name", 0x1e - ((u8)count - 1));
    char* s2 = BdatTouchStringCell("MNU_shop", "name", 0xf);
    if (arg3 != NULL) {
        CItemImpl* inst = CItem_initItemImplInstances((CItemData*)item);
        sprintf(rec.name, "%d", inst->vf90((CItemData*)item));
    } else {
        sprintf(rec.name, "%s", s2);
    }
    rec.e0 = (u8)BdatGetU8Direct((const void*)gd8, "rvs_type", (u16)cat);
    rec.e1 = (u8)BdatGetU8Direct((const void*)gd8, "atr_type", (u16)cat);
    rec.color = 0xFFFFFFFF;
    switch (rec.e1) {
        case 4: rec.color = 0xFF00FFFF; break;
        case 5: rec.color = 0x0000FFFF; break;
        case 6: rec.color = 0xFFFFFFFF; break;
        case 7: rec.color = 0x0000FFFF; break;
        case 8: rec.color = 0x00FFFFFF; break;
        case 9: rec.color = 0x775544FF; break;
    }
    sprintf(rec.text, "%s", ((char*(*)(void*, const char*, u16))&BdatGetPtrDirect)(gd8, "rvs_caption", (u16)cat));
    u32 wide = 1;
    if (CDeviceSC::getLanguage() != 3) {
        if (CDeviceSC::getLanguage() != 2) {
            wide = 0;
        }
    }
    // Marker-replacement scan over rec.text: a '$' followed by '1'/'2' is
    // replaced by a localized string (the '1' marker switches on the equip
    // state byte, the '2' marker copies the static label-pointer table into
    // the local array and formats the count-indexed label); every other char
    // advances the scan Shift-JIS-aware (lead-byte ranges advance 2). After
    // a marker, the remaining text is spliced after the replacement.
    char* labels = (char*)itemBoxLabelStrings;
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
                        sprintf(buf28, "<col=red>%s<col=def>", rec.name);
                        break;
                    case 2:
                        sprintf(buf28, "<col=red>%s<col=def>", rec.name);
                        break;
                    case 3:
                        if (wide != 0) {
                            sprintf(buf28, "<col=red>%s %%<col=def>", rec.name);
                        } else {
                            sprintf(buf28, "<col=red>%s%%<col=def>", rec.name);
                        }
                        break;
                    case 4:
                        if (wide != 0) {
                            sprintf(buf28, "<col=red>%s %%<col=def>", rec.name);
                        } else {
                            sprintf(buf28, "<col=red>%s%%<col=def>", rec.name);
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
                    sprintf(buf28, "<col=red>%d %%<col=def>", (u8)name);
                } else {
                    sprintf(buf28, "<col=red>%d%%<col=def>", (u8)name);
                }
            }
            // strlen of buf28 (post-increment counter: retail counts len+1).
            char* p = buf28;
            u32 len = 0;
            while (*p != 0) {
                len++;
                p++;
            }
            sprintf(buf48, "%s", cur);
            cur = cur - 2;
            sprintf(cur, "%s", buf28);
            cur = cur + len - 1;
            sprintf(cur, "%s", buf48);
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

// Twin of getItemBoxLabelRec (see comment there).
CItemBoxLabelRec CItemBoxInfo2::GetLabelRec(u32 item) {
    CItemBoxLabelRec r;
    void* tbl = ::cf::CfBdat::spItmCollectListFileData;
    BdatGetItemType(item);
    // cat stays a raw u16 in a register; each call site re-applies the
    // u16->u32 argument conversion (retail clrlwi per site, no shared temp).
    u16 cat = BdatGetItemId(item);
    // base is anchored at the first lookup so its definition schedules after
    // the cat move (retail order: or r30,r3 then lis/addi).
    r.a = (u8)BdatGetU8Direct((const void*)tbl, "type", cat);
    r.b = BdatTouchStringCell("MNU_collect", "name", r.a);
    // The label ID is also the row key passed to BdatGetPtrDirect.
    r.c = (u8)BdatGetU8Direct((const void*)tbl, "mapID", cat);
    if (r.c == 0x1A) {
        r.d = BdatTouchStringCell("MNU_collect", "name", 0x14);
    } else {
        r.d = BdatGetPtrDirect(::cf::CfBdat::spFldMapListFileData, "name", r.c);
    }
    // Returned by value: the caller's hidden result pointer receives the
    // record as four lwz/stw pairs.
    return r;
}

// Retail buildItemBox2SelTable (ItemBox2 variant of buildItemBoxSelRec): fills the
// 8-byte slot table (flag byte + per-slot selected flags) and a flag2 result from the
// item-table lookup, then writes all three to the info block.
CItemBoxSelRec CItemBoxInfo2::BuildSelRec(u32 item) {
    void* global = ::cf::CfBdat::spItmArtsListFileData;
    u16 v1 = BdatGetItemType(item);
    u16 v2 = BdatGetItemId(item);
    CItemBoxSelRec rec;
    rec.category = (u8)BdatGetU8Direct((const void*)global, (char*)"memory_type", v2);
    // count stays u32: retail keeps the raw call result (r28) and masks at
    // each (u8)count use (loop mask hoisted to r29).
    u8 count = BdatGetU8Direct((const void*)global, (char*)"pc_type", v2);
    for (u32 i = 0; (u8)i < (u8)code80135FDC_getByte_64077(); i++) {
        u8 a = (u8)GetCollectedFlagByte((u8)i);
        rec.selected[(u8)i] = a == (u8)count;
    }
    u8 key = BdatGetU8Direct((const void*)global, (char*)"get_arts", v2);
    u8 val = BdatGetU8ByTableKey((char*)"pc_arts", (char*)"idx", (u8)key);
    void* lookup = CtrlObjectParam_GetCharEntry((u8)count);
    u8 cat = (u8)CtrlObjectParam_GetCurrentRowKey(lookup);
    rec.flag2 = 0;
    u8* entry = (u8*)lookup + cat * 0x49 + val * 2;
    switch (rec.category) {
        case 1: if (entry[0xE8] != 0) rec.flag2 = 1; break;
        case 2: if ((entry[0xE9] >> 7) & 1) rec.flag2 = 1; break;
        case 3: if ((entry[0xE9] >> 6) & 1) rec.flag2 = 1; break;
    }
    return rec;
}
// Retail buildItemBox2SlotRec fills a 0x24-byte slot record (count byte, string ptr,
// per-item text ptrs at +8, per-item values at +0x1C, counter at +0x21) then
// copies it out. The BdatGetItemType/80139358 results are discarded (dead calls
// kept because MWCC never elides calls). Param 2 (info) is unused in retail.
CItemBoxSlotRecord CItemBoxInfo2::BuildSlotRec(u32 item, void* arg4) {
    CItemBoxSlotRecord rec;
    void* p = arg4 != 0 ? arg4 : 0;
    BdatGetItemType(item);
    BdatGetItemId(item);
    u32 v08 = CItem_initItemImplInstances((CItemData*)p)->vf08((CItemData*)p);
    rec.count = (u8)v08;
    rec.str = (u32)BdatTouchStringCell("MNU_item", "name", 0x1e - ((u8)v08 - 1));
    rec.tail[1] = 0;
    for (u32 i = 0; i < 4; i++) {
        u32 n = (u16)CItem_initItemImplInstances((CItemData*)p)->vf4C((CItemData*)p, (u8)i);
        if ((s32)n > 0) {
            rec.text[rec.tail[1]] =
                (u32)((char*(*)(void*, char*, u32))&BdatGetPtrDirect)(::cf::CfBdat::spBtlSkillListFileData, "name", n);
            rec.vals[rec.tail[1]] = CItem_initItemImplInstances((CItemData*)p)->vf64((CItemData*)p, (u8)i);
            rec.tail[1]++;
        }
    }
    // Returned by value: the 0x24-byte copy to the hidden result pointer is
    // the retail mtctr 8-byte-pair loop (li r0,4 + 4-byte tail).
    return rec;
}

// Render the item-box slot info (equip-category 2 items): for each slot (count from
// the slot-table byte getter) set up a labelled layout pane, per-slot vertex colours,
// and (when a selection applies) a texture/name tag.
void CItemBoxInfo2::RenderKind2(u16 arg1, void* arg2, u16 arg3) {
    SetupSlotPanes();
    // Declaration order follows the retail frame layout (high -> low):
    // copied record, pane-name/label buffers, source record, colour defaults.
    CItemBoxCompRecord local = BuildCmpRecord((void*)arg1);
    char paneName[0x20];
    char label[0x20];
    for (u32 i = 0; (u8)i < (u8)code80135FDC_getByte_64077(); i++) {
        u8 flag = local.flags[0][(u8)i];
        u32 color = 0x777777FF;
        if (flag != 0) color = 0xFFFFFFFF;
        sprintf(paneName, "pic_pc%02d", (u8)i + 1);
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
        sprintf(paneName, "pic_pcbs%02d", (u8)i + 1);
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
                void* item = CItem_kindPtr(2, ((CItemBoxCharEntry*)member)->slotIds[5]);
                if (item != NULL && *(u32*)item != 0) {
                    if (arg2 == item || arg1 == (u32)(*(u32*)item >> 20)) {
                        sprintf(label, "pic_eq%02d", (u8)i + 1);
                        u8 otherSlot = (u8)GetCollectedFlagByte(arg3);
                        u32 tex;
                        if ((s32)slot == (s32)otherSlot) {
                            tex = (u32)mArcResAcc->GetResource(0x74696D67, "mf00_reg00_eq01.tpl", NULL);
                        } else {
                            tex = (u32)mArcResAcc->GetResource(0x74696D67, "mf00_reg00_eq00.tpl", NULL);
                        }
                        if (tex == 0) {
                            tex = (u32)mArcResAcc->GetResource(0x74696D67, "mf00_com00_dmy.tpl", NULL);
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

void CItemBoxInfo2::RenderDetail(u16 arg2, void* arg3, u16 arg4) {
    ResetRowPanes();
    // Frame-slot order: record > buf > bufName > local > args > cols >
    // selB/selA (the staged colour temps; retail keeps its copies above the
    // record, ours sit lowest - offsets inside the staging block differ).
    char buf[0x20];
    char bufName[0x20];
    CItemBoxSlotFlags record = BuildSlotFlags(arg2);
    // Hoisted halves: MWCC materializes these once (lis) and rebuilds each
    // constant at its use site with a single addi.
    u32 darkBase = 0x77770000;
    u32 tagHi = 0x74690000;
    for (u32 i = 0; (u8)i < (u8)code80135FDC_getByte_64077(); i++) {
        u8 flag = record.equipable[(u8)i];
        u32 color = darkBase + 0x77ff;  // 0x777777ff
        if (flag != 0) color = 0xFFFFFFFF;
        sprintf(buf, "pic_pc%02d", (u8)i + 1);
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
        sprintf(buf, "pic_pcbs%02d", (u8)i + 1);
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
                    case 4: value = ((CItemBoxCharEntry*)member)->slotIds[0]; break;
                    case 5: value = ((CItemBoxCharEntry*)member)->slotIds[1]; break;
                    case 6: value = ((CItemBoxCharEntry*)member)->slotIds[2]; break;
                    case 7: value = ((CItemBoxCharEntry*)member)->slotIds[3]; break;
                    case 8: value = ((CItemBoxCharEntry*)member)->slotIds[4]; break;
                }
                void* item = CItem_kindPtr(category, value);
                if (item != NULL && *(u32*)item != 0) {
                    if (arg3 == item || arg2 == (*(u32*)item >> 20)) {
                        sprintf(bufName, "pic_eq%02d", (u8)i + 1);
                        u32 tex;
                        if (slot == (u8)GetCollectedFlagByte(arg4)) {
                            tex = (u32)mArcResAcc->GetResource(tagHi + 0x6d67, "mf00_reg00_eq01.tpl", NULL);
                        } else {
                            tex = (u32)mArcResAcc->GetResource(tagHi + 0x6d67, "mf00_reg00_eq00.tpl", NULL);
                        }
                        if (tex == 0) {
                            tex = (u32)mArcResAcc->GetResource(tagHi + 0x6d67, "mf00_com00_dmy.tpl", NULL);
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
// end renderItemBox2Detail
// Retail refreshItemBox2SlotRows: refresh the item-box slot rows. Fetches the per-slot
// selection table via buildItemBox2SelTable into a stack record, sets the row label to
// the category name (length 0x2c vs 0x2b depending on flag2), then for each
// visible slot colours the pane (white when selected) and pushes two vertex
// colours - the item-specific pair from info+0x9c when selected, otherwise
// alpha-only defaults built from info+0xa2/0xaa - through setItemBoxVtxColor.
void CItemBoxInfo2::RefreshSlotRows(u16 arg2) {
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
    u32 rowHeight = 0x2c;
    if (local.flag2 != 0) rowHeight = 0x2b;
    char* label = BdatTouchStringCell("MNU_item", "name", rowHeight);
    LayoutSetTextBoxFmtValue(mpLayout, "txt_para80", label, 0);
    for (u32 i = 0; (u8)i < (u8)code80135FDC_getByte_64077(); i++) {
        u8 flag = local.selected[(u8)i];
        u32 color = 0x777777ff;
        if (flag != 0) color = 0xFFFFFFFF;
        sprintf(buf, "pic_pc%02d", (u8)i + 1);
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
        sprintf(buf, "pic_pcbs%02d", (u8)i + 1, cols[0]);
        nw4r::lyt::Pane* pane = mpLayout->GetRootPane()->FindPaneByName(buf, true);
        if (pane != NULL) {
            void* obj = pane->GetMaterial();
            for (u32 j = 0; j < 2; j++) {
                setItemBoxVtxColor(obj, (u8)j, &out[j]);
            }
        }
    }
}
// Pipeline: two layout preps, then buildItemBox2NameRec fills a 0xA0-byte label record
// that is struct-copied to a local (20-pair mtctr loop) and stamped onto the fixed
// panes; the candidate item's name/category labels follow. The per-slot loop colours
// each row (red when unselected, white + the info-block vertex colours when selected)
// and, for slots whose six equip values resolve to the candidate item, stamps its
// texture.
void CItemBoxInfo2::RenderKind3(u16 arg2, void* arg3, u16 arg4) {
    ClearQuadTexts();
    ResetRowPanes();
    CItemBoxNameRecord vals = BuildNameRec((void*)(u32)arg2, arg3);
    char textBuf[0x20];
    char buf2[0x20];
    CItemBoxVertexColors cols;
    CItemBoxSlotEntry entry;

    // Retail materializes the pool base only after the record copy.

    LayoutSetTextBoxFmtValue((nw4r::lyt::Layout*)mpLayout, "txt_value50", (char*)vals.str, 0);
    if ((s8)vals.name[0] == 0x30) {
        LayoutSetTextBoxFmtValue((nw4r::lyt::Layout*)mpLayout, "txt_value51", "", 0);
    } else {
        if (vals.e0 != 0) {
            if ((u8)(vals.e0 - 3) <= 1) {
                sprintf(textBuf, "%s%s", vals.name,
                        BdatTouchStringCell("MNU_item", "name", 0x21));
            } else {
                sprintf(textBuf, "%s ", vals.name);
            }
        } else {
            sprintf(textBuf, "%s ", vals.name);
        }
        LayoutSetTextBoxText((nw4r::lyt::Layout*)mpLayout, "txt_value51", textBuf, 0);
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
    LayoutSetTextBoxFmtValue((nw4r::lyt::Layout*)mpLayout, "txt_para51", txt, 0);

    u16 cat;
    if (arg3 != NULL) {
        cat = (u16)CItem_initItemImplInstances((CItemData*)item)->vf54((CItemData*)item);
    } else {
        cat = BdatGetItemId(arg2);
    }
    u8 st = BdatGetU8Direct((const void*)::cf::CfBdat::spBtlSkillListFileData, "attach", cat);
    char* label = BdatTouchStringCell("MNU_item", "name", (u8)st + 0x15);
    LayoutSetTextBoxFmtValue((nw4r::lyt::Layout*)mpLayout, "txt_para52", label, 0);

    // Table handle pair is loop-invariant: staged once before the loop.
    CItemBoxTagStage stage = { { 2, 4, 5, 6, 7, 8 } };
    for (u32 i = 0; (u8)i < (u8)code80135FDC_getByte_64077(); i++) {
        u32 idx = (u8)i + 1;
        sprintf(textBuf, "pic_pc%02d", idx);
        PaneSetVtxColorByName((nw4r::lyt::Layout*)mpLayout, textBuf, -1);
        // Row colour pair: zeroed memberwise, then filled from the this
        // selection block; passed to sprintf as one 0x10-byte vararg.
        cols.w[0] = 0; cols.w[1] = 0; cols.w[2] = 0; cols.w[3] = 0;
        cols.w[0] = mColorA.w[0];
        cols.w[1] = mColorA.w[1];
        cols.w[2] = mColorB.w[0];
        cols.w[3] = mColorB.w[1];
        sprintf(textBuf, "pic_pcbs%02d", idx, cols);
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
        entry.vals[0] = ((CItemBoxCharEntry*)lookup)->slotIds[5];
        entry.vals[1] = ((CItemBoxCharEntry*)lookup)->slotIds[0];
        entry.vals[2] = ((CItemBoxCharEntry*)lookup)->slotIds[1];
        entry.vals[3] = ((CItemBoxCharEntry*)lookup)->slotIds[2];
        entry.vals[4] = ((CItemBoxCharEntry*)lookup)->slotIds[3];
        entry.vals[5] = ((CItemBoxCharEntry*)lookup)->slotIds[4];
        for (u32 j = 0; (u8)j < 6; j++) {
            void* cand = CItem_kindPtr((u8)j, entry.vals[(u8)j]);
            if (cand == NULL || *(u32*)cand == 0) continue;
            u8 cnt = CItem_initItemImplInstances((CItemData*)cand)->vf30((CItemData*)cand);
            for (u32 k = 0; (u8)k < cnt; k++) {
                s16 itemVal =
                    CItem_initItemImplInstances((CItemData*)cand)->vf40((CItemData*)cand, (u8)k);
                void* hit = CItem_kindPtr(3, itemVal);
                if (hit == NULL || *(u32*)hit == 0 || hit != arg3) continue;
                sprintf(buf2, "pic_eq%02d", idx);
                u32 tex;
                if (slot == (u8)GetCollectedFlagByte(arg4)) {
                    tex = (u32)mArcResAcc->GetResource(0x74696d67, "mf00_reg00_eq01.tpl", NULL);
                } else {
                    tex = (u32)mArcResAcc->GetResource(0x74696d67, "mf00_reg00_eq00.tpl", NULL);
                }
                if (tex == 0) {
                    tex = (u32)mArcResAcc->GetResource(0x74696d67, "mf00_com00_dmy.tpl", NULL);
                }
                if (tex != 0) {
                    PaneSetTexPaletteByName((nw4r::lyt::Layout*)mpLayout, (const char*)buf2, (TPLPalette*)tex);
                }
            }
        }
    }
}
void CItemBoxInfo2::RenderLabels(u32 arg2) {
    ClearPairTexts();
    ResetRowPanes();
    CItemBoxLabelRec rec = GetLabelRec(arg2);
    LayoutSetTextBoxFmtValue((nw4r::lyt::Layout*)mpLayout, "txt_value60", rec.b, 0);
    LayoutSetTextBoxFmtValue((nw4r::lyt::Layout*)mpLayout, "txt_value61", rec.d, 0);
}
// Retail renderItemBox2SlotList: run the two layout preps, build a 0x24-byte slot
// record via buildItemBox2SlotRec, struct-copy it to a local (retail mtctr 8-byte
// pair loop), then label every slot: a count-driven loop of pane-name
// sprintf + per-item text/value colour labels (inlined FixStr<32> ctor).
// Word-array view: assigning the union copies the whole object as one block
// (reproduces the retail mtctr lwzu/stwu pair loop) instead of expanding
// field-by-field.
union CItemBoxSlotRecAny {
    CItemBoxSlotRecord rec;
    u32 w[9];
};
void CItemBoxInfo2::RenderSlotList(void* arg1, void* arg2) {
    // Retail runs the two layout preps before the FixStr ctor bl.
    char paneName[0x20];
    ClearSlotTexts();
    ResetRowPanes();
    CItemBoxSlotRecord cur = BuildSlotRec((u32)arg1, arg2);
    ml::FixStr<32> text(false);
    LayoutSetTextBoxFmtValue((nw4r::lyt::Layout*)mpLayout, "txt_value30", (char*)cur.str, 0);
    u8 count = cur.tail[1];
    for (u8 i = 0; i < count; i++) {
        sprintf(paneName, "txt_para%02d", (u8)i * 2 + 0x1f);
        LayoutSetTextBoxFmtValue((nw4r::lyt::Layout*)mpLayout, paneName, (char*)cur.text[i], 0);
        text.clear();
        sprintf(paneName, "txt_value%02d", (u8)i + 0x1f);
        text.format("%d%s", cur.vals[i],
                    BdatTouchStringCell("MNU_item", "name", 0x21));
        LayoutSetTextBoxFmtValue((nw4r::lyt::Layout*)mpLayout, paneName, text.c_str(), 0);
    }
}

void CItemBoxInfo2::SetupSlotPanes() {
    // i declared first: retail claims r31 for the loop counter before any
    // other saved local (register_mapping.md Rule A).
    // No named base local: retail holds the pooled literal base purely as a
    // compiler CSE temporary (lis into a scratch register during prologue
    // argument prep), LICM-promoted to a saved register for the loop.
    char buf[0x20];
    u32 i;
    u32 idx;
    s16 zero = 0;
    LayoutSetTextBoxFmtValue((nw4r::lyt::Layout*)mpLayout, "txt_value02", "", 0);
    LayoutSetTextBoxFmtValue((nw4r::lyt::Layout*)mpLayout, "txt_value08", "", 0);
    LayoutSetTextBoxFmtValue((nw4r::lyt::Layout*)mpLayout, "txt_value09", "", 0);
    LayoutSetTextBoxFmtValue((nw4r::lyt::Layout*)mpLayout, "txt_value04", "", 0);
    LayoutSetTextBoxFmtValue((nw4r::lyt::Layout*)mpLayout, "txt_value07", "", 0);
    LayoutSetTextBoxFmtValue((nw4r::lyt::Layout*)mpLayout, "txt_value22", "", 0);
    PaneMatSetTevColorsByName((nw4r::lyt::Layout*)mpLayout, "txt_para01", (const GXColorS10*)&itemBoxInfo_slotValueTop, (const GXColorS10*)&itemBoxInfo_slotValueBottom);
    PaneMatSetTevColorsByName((nw4r::lyt::Layout*)mpLayout, "txt_para10", (const GXColorS10*)&itemBoxInfo_slotValueTop, (const GXColorS10*)&itemBoxInfo_slotValueBottom);
    PaneMatSetTevColorsByName((nw4r::lyt::Layout*)mpLayout, "txt_para11", (const GXColorS10*)&itemBoxInfo_slotValueTop, (const GXColorS10*)&itemBoxInfo_slotValueBottom);
    PaneMatSetTevColorsByName((nw4r::lyt::Layout*)mpLayout, "txt_para04", (const GXColorS10*)&itemBoxInfo_slotValueTop, (const GXColorS10*)&itemBoxInfo_slotValueBottom);
    PaneMatSetTevColorsByName((nw4r::lyt::Layout*)mpLayout, "txt_para08", (const GXColorS10*)&itemBoxInfo_slotValueTop, (const GXColorS10*)&itemBoxInfo_slotValueBottom);
    PaneMatSetTevColorsByName((nw4r::lyt::Layout*)mpLayout, "txt_para22", (const GXColorS10*)&itemBoxInfo_slotValueTop, (const GXColorS10*)&itemBoxInfo_slotValueBottom);
    for (i = 0; (u8)i < (u8)code80135FDC_getByte_64077(); i++) {
        idx = (u8)i + 1;
        sprintf(buf, "pic_pc%02d", idx);
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
        sprintf(buf, "pic_pcbs%02d", idx);
        nw4r::lyt::Pane* pane = mpLayout->GetRootPane()->FindPaneByName(buf, true);
        if (pane != NULL) {
            nw4r::lyt::Material* material = pane->GetMaterial();
            for (u32 j = 0; j < 2; j++) {
                setItemBoxVtxColor(material, (u8)j, (u8*)args + (u8)j * 8);
            }
        }
        // Item-name lookup on the shared arc resource accessor (vtable+0x0C),
        // then push the texture name onto the slot pane.
        sprintf(buf, "pic_eq%02d", idx);
        u32 tex = (u32)mArcResAcc->GetResource(0x74696D67, "mf00_com00_dmy.tpl", NULL);
        if (tex != 0) {
            PaneSetTexPaletteByName((nw4r::lyt::Layout*)mpLayout, (const char*)buf, (TPLPalette*)tex);
        }
    }
}

void CItemBoxInfo2::ResetRowPanes() {
    // No named base local: retail holds the pooled literal base purely as a
    // compiler CSE temporary (lis into a scratch register during prologue
    // argument prep), which fixes both its schedule and its register.
    char text[0x20];
    LayoutSetTextBoxFmtValue((nw4r::lyt::Layout*)mpLayout, "txt_value08", "", 0);
    LayoutSetTextBoxFmtValue((nw4r::lyt::Layout*)mpLayout, "txt_value09", "", 0);
    LayoutSetTextBoxFmtValue((nw4r::lyt::Layout*)mpLayout, "txt_value22", "", 0);
    PaneMatSetTevColorsByName((nw4r::lyt::Layout*)mpLayout, "txt_para01", (const GXColorS10*)&itemBoxInfo_slotValueTop, (const GXColorS10*)&itemBoxInfo_slotValueBottom);
    PaneMatSetTevColorsByName((nw4r::lyt::Layout*)mpLayout, "txt_para10", (const GXColorS10*)&itemBoxInfo_slotValueTop, (const GXColorS10*)&itemBoxInfo_slotValueBottom);
    PaneMatSetTevColorsByName((nw4r::lyt::Layout*)mpLayout, "txt_para11", (const GXColorS10*)&itemBoxInfo_slotValueTop, (const GXColorS10*)&itemBoxInfo_slotValueBottom);
    PaneMatSetTevColorsByName((nw4r::lyt::Layout*)mpLayout, "txt_para04", (const GXColorS10*)&itemBoxInfo_slotValueTop, (const GXColorS10*)&itemBoxInfo_slotValueBottom);
    PaneMatSetTevColorsByName((nw4r::lyt::Layout*)mpLayout, "txt_para08", (const GXColorS10*)&itemBoxInfo_slotValueTop, (const GXColorS10*)&itemBoxInfo_slotValueBottom);
    PaneMatSetTevColorsByName((nw4r::lyt::Layout*)mpLayout, "txt_para22", (const GXColorS10*)&itemBoxInfo_slotValueTop, (const GXColorS10*)&itemBoxInfo_slotValueBottom);
    // Zero lives outside the loop: retail materializes it once in the
    // preheader (a loop-invariant CSE), not per-iteration.
    s16 shade = 0;
    for (u32 index = 0; (u8)index < (u8)code80135FDC_getByte_64077(); index++) {
        u32 slot = (u8)index + 1;
        sprintf(text, "pic_pc%02d", slot);
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
        sprintf(text, "pic_pcbs%02d", slot);
        nw4r::lyt::Pane* pPane = mpLayout->GetRootPane()->FindPaneByName(text, true);
        if (pPane != NULL) {
            nw4r::lyt::Material* pMaterial = pPane->GetMaterial();
            for (u32 k = 0; k < 2; k++) {
                setItemBoxVtxColor(pMaterial, (u8)k, (u8*)args + (u8)k * 8);
            }
        }
        // Item-name lookup on the shared arc resource accessor (vtable+0x0C),
        // then push the texture name onto the slot pane.
        sprintf(text, "pic_eq%02d", slot);
        u32 texId = (u32)mArcResAcc->GetResource(0x74696D67, "mf00_com00_dmy.tpl", NULL);
        if (texId != 0) {
            PaneSetTexPaletteByName((nw4r::lyt::Layout*)mpLayout, (const char*)text, (TPLPalette*)texId);
        }
    }
}
void CItemBoxInfo2::ClearTripleRows() {
    char buf[0x20];
    // Declaration order fixes r29/r30: retail claims index (r30) before i (r29).
    u32 index;
    u32 i;
    for (i = 0; i < 3; i++) {
        index = (u8)i + 1;
        sprintf(buf, "pic_ethcol%02d", index);
        nw4r::lyt::Pane* r = mpLayout->GetRootPane()->FindPaneByName(buf, true);
        setPaneVisible(r, 0);
        sprintf(buf, "txt_ethvalue%02d", index);
        LayoutSetTextBoxFmtValue(mpLayout, buf, "", 0);
        sprintf(buf, "txt_eth%02d", index);
        LayoutSetTextBoxFmtValue(mpLayout, buf, "", 0);
    }
}
void CItemBoxInfo2::ApplySelColors() {
    LayoutSetTextBoxFmtValue((nw4r::lyt::Layout*)mpLayout, "txt_para80", "", 0);
    char buf[0x20];
    u32 idx;
    // Retail re-evaluates the slot count on every iteration (bl in the
    // loop condition), so do not hoist the call.
    for (u32 i = 0; (u8)i < (u8)code80135FDC_getByte_64077(); i++) {
        idx = (u32)((u8)i + 1);
        sprintf(buf, "pic_pc%02d", idx);
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
        sprintf(buf, "pic_pcbs%02d", idx);
        nw4r::lyt::Pane* pane = mpLayout->GetRootPane()->FindPaneByName(buf, true);
        if (pane != NULL) {
            void* obj = pane->GetMaterial();
            for (u32 j = 0; j < 2; j++) {
                setItemBoxVtxColor(obj, (u8)j, (u8*)args + (u8)j * 8);
            }
        }
    }
}

void CItemBoxInfo2::ClearQuadTexts() {
    LayoutSetTextBoxFmtValue((nw4r::lyt::Layout*)mpLayout,
                  "txt_value50",
                  "", 0);
    LayoutSetTextBoxFmtValue((nw4r::lyt::Layout*)mpLayout,
                  "txt_value51",
                  "", 0);
    LayoutSetTextBoxFmtValue((nw4r::lyt::Layout*)mpLayout,
                  "txt_para52",
                  "", 0);
    LayoutSetTextBoxFmtValue((nw4r::lyt::Layout*)mpLayout,
                  "txt_para51",
                  "", 0);
}

void CItemBoxInfo2::ClearPairTexts() {
    LayoutSetTextBoxFmtValue((nw4r::lyt::Layout*)mpLayout,
                  "txt_value60",
                  "", 0);
    LayoutSetTextBoxFmtValue((nw4r::lyt::Layout*)mpLayout,
                  "txt_value61",
                  "", 0);
}

void CItemBoxInfo2::ClearSlotTexts() {
    char buf[0x20];
    LayoutSetTextBoxFmtValue((nw4r::lyt::Layout*)mpLayout,
                  "txt_value30",
                  "", 0);
    for (u32 i = 0; i < 4; i++) {
        sprintf(buf, "txt_para%02d", (u8)i * 2 + 0x1f);
        LayoutSetTextBoxFmtValue((nw4r::lyt::Layout*)mpLayout, buf,
                      "", 0);
        sprintf(buf, "txt_value%02d", (u8)i + 0x1f);
        LayoutSetTextBoxFmtValue((nw4r::lyt::Layout*)mpLayout, buf,
                      "", 0);
    }
}

// Retail stampItemBox2SlotTex: ItemBox2 variant of stampItemBoxSlotTex (same body, same
// stmw r22 frame). Loops 7 slots; below the active count it resolves the item
// name via CUICfManager_getArcResourceAccessor/vtable+0x0C and colors the pane; otherwise a plain
// label pane. The two alpha-only vertex colours are packed into a 4-word block
// shared by sprintf and the per-vertex setItemBoxVtxColor loop.
void CItemBoxInfo2::StampSlotTex() {
    u32 i;
    char bufElse[0x20];
    char buf[0x20];
    u8 count = code80135FDC_getByte_64077();
    u32 idx;
    for (i = 0; i < 7; i++) {
        if ((u8)i >= count) {
            sprintf(bufElse, "pic_pcbs%02d", (u8)i + 1);
            nw4r::lyt::Pane* pane = mpLayout->GetRootPane()->FindPaneByName(bufElse, true);
            setPaneVisible(pane, 0);
        } else {
            u8 slot = (u8)GetCollectedFlagByte((u8)i);
            u32 tag = 0x74696D67u;
            u32 nameId = (u16)BdatGetU16Direct(::cf::CfBdat::spBtlPcListFileData, "tag_icon", slot);
            u32 itemId = (u32)MakeTplNameSysFile(nameId);
            // Item-name lookup through the shared name system (vtable+0x0C).
            u32 found = (u32)CUICfManager_getArcResourceAccessor()->GetResource(tag, (const char*)itemId, NULL);
            if (found != 0) {
                idx = (u32)((u8)i + 1);
                sprintf(buf, "pic_pc%02d", idx);
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
                sprintf(buf, "pic_pcbs%02d", idx);
                nw4r::lyt::Pane* pane3 = mpLayout->GetRootPane()->FindPaneByName(buf, true);
                void* vertexObj = pane3->GetMaterial();
                for (u32 j = 0; j < 2; j++) {
                    setItemBoxVtxColor(vertexObj, (u8)j, (u8*)args + (u8)j * 8);
                }
            }
        }
    }
}
// Activates the panel: once both the layout (+0x34) and its resource
// accessor (+0x30) are present, set the current/active bytes. Goto-gate
// layout matches retail's branch-over-branch shape (bne over a blr with
// the body placed after the return - see CTitle CTitleLogo_draw family).
void CItemBoxInfo2::Activate() {
    if (mpLayout != 0 && mTexArcResAcc != 0) {
        goto body;
    }
    return;
body:
    mCurrent = 1;
    mActive = 1;
}
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
    cf::CActorParam* stats = &((CItemBoxCharEntry*)charObj)->mParam;

    // ---- HP values (clamped to 9999) ----
    s32 hp1 = (s32)stats->CActorParam_getHp();
    if (hp1 > 9999) hp1 = 9999;
    s32 hp2 = (s32)stats->CActorParam_getDamageScale();
    if (hp2 > 9999) hp2 = 9999;

    // ---- name / pane text ----
    LayoutSetTextBoxFmtValue(mpLayout, "txt_para736", BdatGetPtrDirect(::cf::CfBdat::spBtlPcListFileData, "name", member), 0);
    setLayoutTextBoxNumber(mpLayout, "txt_value700", (u8)stats->CActorParam_getActorLevel());
setLayoutTextBoxNumber(mpLayout, "txt_value702", (u8)stats->CActorParam_getTotalCurrency());
    LayoutSetTextBoxInt((nw4r::lyt::Layout*)mSlotPane0, hp1);
    LayoutSetTextBoxInt((nw4r::lyt::Layout*)mSlotPane2, hp2);
    SetLayoutSelfText((nw4r::lyt::Layout*)mSlotPane3, BdatTouchStringCell("MNU_item", "name", 0x82), 0);

    // ---- stat sub-objects ----
    E43StatA* stA = (E43StatA*)stats->CActorParam_getBaseStats();
    E43StatB* stB = (E43StatB*)stats->CActorParam_getArtsDataBlock();
    E43StatC* stC = (E43StatC*)stats->CActorParam_getBonusStats();
    CtrlObjectParam_GetArtsStatsRow((u8*)((CItemBoxCharEntry*)charObj)->slotIds, 5);

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
            void* item2 = CItem_kindPtr(2, ((CItemBoxCharEntry*)charObj)->slotIds[5]);
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
                fmtBuf.format("%d%s%d", atk1, BdatTouchStringCell("MNU_item", "name", 0xB), atk2);
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
            fmtBuf2.format("%d%s", (s16)bar6, BdatTouchStringCell("MNU_item", "name", 0x80));
            SetLayoutSelfText((nw4r::lyt::Layout*)mSlotPane16, fmtBuf2.c_str(), 0);
            ml::FixStr<32> fmtBuf3;
            fmtBuf3.format("%d%s", (s16)(stA->b55), BdatTouchStringCell("MNU_item", "name", 0x80));
            SetLayoutSelfText((nw4r::lyt::Layout*)mSlotPane18, fmtBuf3.c_str(), 0);

            // ---- 20x color application ----
            GXColorS10 q1 = *(GXColorS10*)&itemBoxInfo_labelNeutralTop;
            GXColorS10 q2 = *(GXColorS10*)&itemBoxInfo_labelNeutralBottom;
            GXColorS10 q3 = *(GXColorS10*)&itemBoxInfo_valueNeutralTop;
            GXColorS10 q4 = *(GXColorS10*)&itemBoxInfo_valueNeutralBottom;
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
            GXColorS10 q1 = *(GXColorS10*)&itemBoxInfo_labelNeutralTop;
            GXColorS10 q2 = *(GXColorS10*)&itemBoxInfo_labelNeutralBottom;
            GXColorS10 q3 = *(GXColorS10*)&itemBoxInfo_valueNeutralTop;
            GXColorS10 q4 = *(GXColorS10*)&itemBoxInfo_valueNeutralBottom;
            if (d < 0) {
                __as__11_GXColorS10FRC11_GXColorS10(&q1, &itemBoxInfo_labelDownTop);
                __as__11_GXColorS10FRC11_GXColorS10(&q2, &itemBoxInfo_labelDownBottom);
                __as__11_GXColorS10FRC11_GXColorS10(&q3, &itemBoxInfo_valueDownTop);
                __as__11_GXColorS10FRC11_GXColorS10(&q4, &itemBoxInfo_valueDownBottom);
            } else if (d > 0) {
                __as__11_GXColorS10FRC11_GXColorS10(&q1, &itemBoxInfo_labelUpTop);
                __as__11_GXColorS10FRC11_GXColorS10(&q2, &itemBoxInfo_labelUpBottom);
                __as__11_GXColorS10FRC11_GXColorS10(&q3, &itemBoxInfo_valueUpTop);
                __as__11_GXColorS10FRC11_GXColorS10(&q4, &itemBoxInfo_valueUpBottom);
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
            GXColorS10 q1 = *(GXColorS10*)&itemBoxInfo_labelNeutralTop;
            GXColorS10 q2 = *(GXColorS10*)&itemBoxInfo_labelNeutralBottom;
            GXColorS10 q3 = *(GXColorS10*)&itemBoxInfo_valueNeutralTop;
            GXColorS10 q4 = *(GXColorS10*)&itemBoxInfo_valueNeutralBottom;
            if (dB < 0) {
                __as__11_GXColorS10FRC11_GXColorS10(&q1, &itemBoxInfo_labelDownTop);
                __as__11_GXColorS10FRC11_GXColorS10(&q2, &itemBoxInfo_labelDownBottom);
                __as__11_GXColorS10FRC11_GXColorS10(&q3, &itemBoxInfo_valueDownTop);
                __as__11_GXColorS10FRC11_GXColorS10(&q4, &itemBoxInfo_valueDownBottom);
            } else if (dB > 0) {
                __as__11_GXColorS10FRC11_GXColorS10(&q1, &itemBoxInfo_labelUpTop);
                __as__11_GXColorS10FRC11_GXColorS10(&q2, &itemBoxInfo_labelUpBottom);
                __as__11_GXColorS10FRC11_GXColorS10(&q3, &itemBoxInfo_valueUpTop);
                __as__11_GXColorS10FRC11_GXColorS10(&q4, &itemBoxInfo_valueUpBottom);
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
            GXColorS10 q1 = *(GXColorS10*)&itemBoxInfo_labelNeutralTop;
            GXColorS10 q2 = *(GXColorS10*)&itemBoxInfo_labelNeutralBottom;
            GXColorS10 q3 = *(GXColorS10*)&itemBoxInfo_valueNeutralTop;
            GXColorS10 q4 = *(GXColorS10*)&itemBoxInfo_valueNeutralBottom;
            if (dC < 0) {
                __as__11_GXColorS10FRC11_GXColorS10(&q1, &itemBoxInfo_labelDownTop);
                __as__11_GXColorS10FRC11_GXColorS10(&q2, &itemBoxInfo_labelDownBottom);
                __as__11_GXColorS10FRC11_GXColorS10(&q3, &itemBoxInfo_valueDownTop);
                __as__11_GXColorS10FRC11_GXColorS10(&q4, &itemBoxInfo_valueDownBottom);
            } else if (dC > 0) {
                __as__11_GXColorS10FRC11_GXColorS10(&q1, &itemBoxInfo_labelUpTop);
                __as__11_GXColorS10FRC11_GXColorS10(&q2, &itemBoxInfo_labelUpBottom);
                __as__11_GXColorS10FRC11_GXColorS10(&q3, &itemBoxInfo_valueUpTop);
                __as__11_GXColorS10FRC11_GXColorS10(&q4, &itemBoxInfo_valueUpBottom);
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
            GXColorS10 q1 = *(GXColorS10*)&itemBoxInfo_labelNeutralTop;
            GXColorS10 q2 = *(GXColorS10*)&itemBoxInfo_labelNeutralBottom;
            GXColorS10 q3 = *(GXColorS10*)&itemBoxInfo_valueNeutralTop;
            GXColorS10 q4 = *(GXColorS10*)&itemBoxInfo_valueNeutralBottom;
            if (dD < 0) {
                __as__11_GXColorS10FRC11_GXColorS10(&q1, &itemBoxInfo_labelDownTop);
                __as__11_GXColorS10FRC11_GXColorS10(&q2, &itemBoxInfo_labelDownBottom);
                __as__11_GXColorS10FRC11_GXColorS10(&q3, &itemBoxInfo_valueDownTop);
                __as__11_GXColorS10FRC11_GXColorS10(&q4, &itemBoxInfo_valueDownBottom);
            } else if (dD > 0) {
                __as__11_GXColorS10FRC11_GXColorS10(&q1, &itemBoxInfo_labelUpTop);
                __as__11_GXColorS10FRC11_GXColorS10(&q2, &itemBoxInfo_labelUpBottom);
                __as__11_GXColorS10FRC11_GXColorS10(&q3, &itemBoxInfo_valueUpTop);
                __as__11_GXColorS10FRC11_GXColorS10(&q4, &itemBoxInfo_valueUpBottom);
            }
            PaneMatSetTevColors(mSlotPane0, &q3, &q4);
            PaneMatSetTevColors(mSlotPane2, &q3, &q4);
            PaneMatSetTevColors(mSlotPane3, &q3, &q4);
            PaneMatSetTevColors(mSlotPane1, &q1, &q2);
        }
        // ---- type switch: slot id + item ----
        s16 slotId = 0;
        // (was switch: retail compiles this dispatch as compares)
        if (type == 2) slotId = ((CItemBoxCharEntry*)charObj)->slotIds[5];
        else if (type == 4) slotId = ((CItemBoxCharEntry*)charObj)->slotIds[0];
        else if (type == 5) slotId = ((CItemBoxCharEntry*)charObj)->slotIds[1];
        else if (type == 6) slotId = ((CItemBoxCharEntry*)charObj)->slotIds[2];
        else if (type == 7) slotId = ((CItemBoxCharEntry*)charObj)->slotIds[3];
        else if (type == 8) slotId = ((CItemBoxCharEntry*)charObj)->slotIds[4];
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
            fmtBuf.format("%d%s%d", atkD, BdatTouchStringCell("MNU_item", "name", 0xB), atkC);
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
            fmtBuf2.format("%d%s", (s16)disp3, BdatTouchStringCell("MNU_item", "name", 0x80));
            SetLayoutSelfText((nw4r::lyt::Layout*)mSlotPane16, fmtBuf2.c_str(), 0);
            ml::FixStr<32> fmtBuf3;
            fmtBuf3.format("%d%s", (s32)stA->b55 + (s32)d3, BdatTouchStringCell("MNU_item", "name", 0x80));
            SetLayoutSelfText((nw4r::lyt::Layout*)mSlotPane18, fmtBuf3.c_str(), 0);
            // delta colors (5 rows)
            {
                GXColorS10 q1 = *(GXColorS10*)&itemBoxInfo_labelNeutralTop;
                GXColorS10 q2 = *(GXColorS10*)&itemBoxInfo_labelNeutralBottom;
                GXColorS10 q3 = *(GXColorS10*)&itemBoxInfo_valueNeutralTop;
                GXColorS10 q4 = *(GXColorS10*)&itemBoxInfo_valueNeutralBottom;
                if (dW < 0) {
                    __as__11_GXColorS10FRC11_GXColorS10(&q1, &itemBoxInfo_labelDownTop);
                    __as__11_GXColorS10FRC11_GXColorS10(&q2, &itemBoxInfo_labelDownBottom);
                    __as__11_GXColorS10FRC11_GXColorS10(&q3, &itemBoxInfo_valueDownTop);
                    __as__11_GXColorS10FRC11_GXColorS10(&q4, &itemBoxInfo_valueDownBottom);
                } else if (dW > 0) {
                    __as__11_GXColorS10FRC11_GXColorS10(&q1, &itemBoxInfo_labelUpTop);
                    __as__11_GXColorS10FRC11_GXColorS10(&q2, &itemBoxInfo_labelUpBottom);
                    __as__11_GXColorS10FRC11_GXColorS10(&q3, &itemBoxInfo_valueUpTop);
                    __as__11_GXColorS10FRC11_GXColorS10(&q4, &itemBoxInfo_valueUpBottom);
                }
                PaneMatSetTevColors(mSlotPane10, &q3, &q4);
                PaneMatSetTevColors(mSlotPane11, &q1, &q2);
            }
            {
                GXColorS10 q1 = *(GXColorS10*)&itemBoxInfo_labelNeutralTop;
                GXColorS10 q2 = *(GXColorS10*)&itemBoxInfo_labelNeutralBottom;
                GXColorS10 q3 = *(GXColorS10*)&itemBoxInfo_valueNeutralTop;
                GXColorS10 q4 = *(GXColorS10*)&itemBoxInfo_valueNeutralBottom;
                if (d1 < 0) {
                    __as__11_GXColorS10FRC11_GXColorS10(&q1, &itemBoxInfo_labelDownTop);
                    __as__11_GXColorS10FRC11_GXColorS10(&q2, &itemBoxInfo_labelDownBottom);
                    __as__11_GXColorS10FRC11_GXColorS10(&q3, &itemBoxInfo_valueDownTop);
                    __as__11_GXColorS10FRC11_GXColorS10(&q4, &itemBoxInfo_valueDownBottom);
                } else if (d1 > 0) {
                    __as__11_GXColorS10FRC11_GXColorS10(&q1, &itemBoxInfo_labelUpTop);
                    __as__11_GXColorS10FRC11_GXColorS10(&q2, &itemBoxInfo_labelUpBottom);
                    __as__11_GXColorS10FRC11_GXColorS10(&q3, &itemBoxInfo_valueUpTop);
                    __as__11_GXColorS10FRC11_GXColorS10(&q4, &itemBoxInfo_valueUpBottom);
                }
                PaneMatSetTevColors(mSlotPane12, &q3, &q4);
                PaneMatSetTevColors(mSlotPane13, &q1, &q2);
            }
            {
                GXColorS10 q1 = *(GXColorS10*)&itemBoxInfo_labelNeutralTop;
                GXColorS10 q2 = *(GXColorS10*)&itemBoxInfo_labelNeutralBottom;
                GXColorS10 q3 = *(GXColorS10*)&itemBoxInfo_valueNeutralTop;
                GXColorS10 q4 = *(GXColorS10*)&itemBoxInfo_valueNeutralBottom;
                if (d2 < 0) {
                    __as__11_GXColorS10FRC11_GXColorS10(&q1, &itemBoxInfo_labelDownTop);
                    __as__11_GXColorS10FRC11_GXColorS10(&q2, &itemBoxInfo_labelDownBottom);
                    __as__11_GXColorS10FRC11_GXColorS10(&q3, &itemBoxInfo_valueDownTop);
                    __as__11_GXColorS10FRC11_GXColorS10(&q4, &itemBoxInfo_valueDownBottom);
                } else if (d2 > 0) {
                    __as__11_GXColorS10FRC11_GXColorS10(&q1, &itemBoxInfo_labelUpTop);
                    __as__11_GXColorS10FRC11_GXColorS10(&q2, &itemBoxInfo_labelUpBottom);
                    __as__11_GXColorS10FRC11_GXColorS10(&q3, &itemBoxInfo_valueUpTop);
                    __as__11_GXColorS10FRC11_GXColorS10(&q4, &itemBoxInfo_valueUpBottom);
                }
                PaneMatSetTevColors(mSlotPane14, &q3, &q4);
                PaneMatSetTevColors(mSlotPane15, &q1, &q2);
            }
            {
                GXColorS10 q1 = *(GXColorS10*)&itemBoxInfo_labelNeutralTop;
                GXColorS10 q2 = *(GXColorS10*)&itemBoxInfo_labelNeutralBottom;
                GXColorS10 q3 = *(GXColorS10*)&itemBoxInfo_valueNeutralTop;
                GXColorS10 q4 = *(GXColorS10*)&itemBoxInfo_valueNeutralBottom;
                if (d4 < 0) {
                    __as__11_GXColorS10FRC11_GXColorS10(&q1, &itemBoxInfo_labelDownTop);
                    __as__11_GXColorS10FRC11_GXColorS10(&q2, &itemBoxInfo_labelDownBottom);
                    __as__11_GXColorS10FRC11_GXColorS10(&q3, &itemBoxInfo_valueDownTop);
                    __as__11_GXColorS10FRC11_GXColorS10(&q4, &itemBoxInfo_valueDownBottom);
                } else if (d4 > 0) {
                    __as__11_GXColorS10FRC11_GXColorS10(&q1, &itemBoxInfo_labelUpTop);
                    __as__11_GXColorS10FRC11_GXColorS10(&q2, &itemBoxInfo_labelUpBottom);
                    __as__11_GXColorS10FRC11_GXColorS10(&q3, &itemBoxInfo_valueUpTop);
                    __as__11_GXColorS10FRC11_GXColorS10(&q4, &itemBoxInfo_valueUpBottom);
                }
                PaneMatSetTevColors(mSlotPane16, &q3, &q4);
                PaneMatSetTevColors(mSlotPane17, &q1, &q2);
            }
            {
                GXColorS10 q1 = *(GXColorS10*)&itemBoxInfo_labelNeutralTop;
                GXColorS10 q2 = *(GXColorS10*)&itemBoxInfo_labelNeutralBottom;
                GXColorS10 q3 = *(GXColorS10*)&itemBoxInfo_valueNeutralTop;
                GXColorS10 q4 = *(GXColorS10*)&itemBoxInfo_valueNeutralBottom;
                if (d3 < 0) {
                    __as__11_GXColorS10FRC11_GXColorS10(&q1, &itemBoxInfo_labelDownTop);
                    __as__11_GXColorS10FRC11_GXColorS10(&q2, &itemBoxInfo_labelDownBottom);
                    __as__11_GXColorS10FRC11_GXColorS10(&q3, &itemBoxInfo_valueDownTop);
                    __as__11_GXColorS10FRC11_GXColorS10(&q4, &itemBoxInfo_valueDownBottom);
                } else if (d3 > 0) {
                    __as__11_GXColorS10FRC11_GXColorS10(&q1, &itemBoxInfo_labelUpTop);
                    __as__11_GXColorS10FRC11_GXColorS10(&q2, &itemBoxInfo_labelUpBottom);
                    __as__11_GXColorS10FRC11_GXColorS10(&q3, &itemBoxInfo_valueUpTop);
                    __as__11_GXColorS10FRC11_GXColorS10(&q4, &itemBoxInfo_valueUpBottom);
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
            u8 arm1 = (u8)BdatGetU8Direct((const void*)::cf::CfBdat::spItmEquipListFileData, "arm_type", BdatGetItemId((u32)w0));
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
            u8 arm2 = (u8)BdatGetU8Direct((const void*)::cf::CfBdat::spItmEquipListFileData, "arm_type", BdatGetItemId((u32)(*(u32*)arg3 >> 20)));
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
            s32 ssum = (s32)((CItemBoxCharEntry*)charObj)->slotIds[3] + ((CItemBoxCharEntry*)charObj)->slotIds[2] + ((CItemBoxCharEntry*)charObj)->slotIds[4] + ((CItemBoxCharEntry*)charObj)->slotIds[0] + ((CItemBoxCharEntry*)charObj)->slotIds[1];
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
            void* item2 = CItem_kindPtr(2, ((CItemBoxCharEntry*)charObj)->slotIds[5]);
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
                fmtBuf.format("%d%s%d", wmin, BdatTouchStringCell("MNU_item", "name", 0xB), wd);
                SetLayoutSelfText((nw4r::lyt::Layout*)mSlotPane10, fmtBuf.c_str(), 0);
            }
            // numbers
            LayoutSetTextBoxInt((nw4r::lyt::Layout*)mSlotPane12, (s16)(u16)c_new.w04);
            LayoutSetTextBoxInt((nw4r::lyt::Layout*)mSlotPane14, (s16)c_new.w14);
            LayoutSetTextBoxInt((nw4r::lyt::Layout*)mSlotPane6, (s16)b14);
            // delta colors (4 rows)
            {
                GXColorS10 q1 = *(GXColorS10*)&itemBoxInfo_labelNeutralTop;
                GXColorS10 q2 = *(GXColorS10*)&itemBoxInfo_labelNeutralBottom;
                GXColorS10 q3 = *(GXColorS10*)&itemBoxInfo_valueNeutralTop;
                GXColorS10 q4 = *(GXColorS10*)&itemBoxInfo_valueNeutralBottom;
                if (dW < 0) {
                    __as__11_GXColorS10FRC11_GXColorS10(&q1, &itemBoxInfo_labelDownTop);
                    __as__11_GXColorS10FRC11_GXColorS10(&q2, &itemBoxInfo_labelDownBottom);
                    __as__11_GXColorS10FRC11_GXColorS10(&q3, &itemBoxInfo_valueDownTop);
                    __as__11_GXColorS10FRC11_GXColorS10(&q4, &itemBoxInfo_valueDownBottom);
                } else if (dW > 0) {
                    __as__11_GXColorS10FRC11_GXColorS10(&q1, &itemBoxInfo_labelUpTop);
                    __as__11_GXColorS10FRC11_GXColorS10(&q2, &itemBoxInfo_labelUpBottom);
                    __as__11_GXColorS10FRC11_GXColorS10(&q3, &itemBoxInfo_valueUpTop);
                    __as__11_GXColorS10FRC11_GXColorS10(&q4, &itemBoxInfo_valueUpBottom);
                }
                PaneMatSetTevColors(mSlotPane10, &q3, &q4);
                PaneMatSetTevColors(mSlotPane11, &q1, &q2);
            }
            {
                GXColorS10 q1 = *(GXColorS10*)&itemBoxInfo_labelNeutralTop;
                GXColorS10 q2 = *(GXColorS10*)&itemBoxInfo_labelNeutralBottom;
                GXColorS10 q3 = *(GXColorS10*)&itemBoxInfo_valueNeutralTop;
                GXColorS10 q4 = *(GXColorS10*)&itemBoxInfo_valueNeutralBottom;
                if (d2 < 0) {
                    __as__11_GXColorS10FRC11_GXColorS10(&q1, &itemBoxInfo_labelDownTop);
                    __as__11_GXColorS10FRC11_GXColorS10(&q2, &itemBoxInfo_labelDownBottom);
                    __as__11_GXColorS10FRC11_GXColorS10(&q3, &itemBoxInfo_valueDownTop);
                    __as__11_GXColorS10FRC11_GXColorS10(&q4, &itemBoxInfo_valueDownBottom);
                } else if (d2 > 0) {
                    __as__11_GXColorS10FRC11_GXColorS10(&q1, &itemBoxInfo_labelUpTop);
                    __as__11_GXColorS10FRC11_GXColorS10(&q2, &itemBoxInfo_labelUpBottom);
                    __as__11_GXColorS10FRC11_GXColorS10(&q3, &itemBoxInfo_valueUpTop);
                    __as__11_GXColorS10FRC11_GXColorS10(&q4, &itemBoxInfo_valueUpBottom);
                }
                PaneMatSetTevColors(mSlotPane12, &q3, &q4);
                PaneMatSetTevColors(mSlotPane13, &q1, &q2);
            }
            {
                GXColorS10 q1 = *(GXColorS10*)&itemBoxInfo_labelNeutralTop;
                GXColorS10 q2 = *(GXColorS10*)&itemBoxInfo_labelNeutralBottom;
                GXColorS10 q3 = *(GXColorS10*)&itemBoxInfo_valueNeutralTop;
                GXColorS10 q4 = *(GXColorS10*)&itemBoxInfo_valueNeutralBottom;
                if (d1 < 0) {
                    __as__11_GXColorS10FRC11_GXColorS10(&q1, &itemBoxInfo_labelDownTop);
                    __as__11_GXColorS10FRC11_GXColorS10(&q2, &itemBoxInfo_labelDownBottom);
                    __as__11_GXColorS10FRC11_GXColorS10(&q3, &itemBoxInfo_valueDownTop);
                    __as__11_GXColorS10FRC11_GXColorS10(&q4, &itemBoxInfo_valueDownBottom);
                } else if (d1 > 0) {
                    __as__11_GXColorS10FRC11_GXColorS10(&q1, &itemBoxInfo_labelUpTop);
                    __as__11_GXColorS10FRC11_GXColorS10(&q2, &itemBoxInfo_labelUpBottom);
                    __as__11_GXColorS10FRC11_GXColorS10(&q3, &itemBoxInfo_valueUpTop);
                    __as__11_GXColorS10FRC11_GXColorS10(&q4, &itemBoxInfo_valueUpBottom);
                }
                PaneMatSetTevColors(mSlotPane14, &q3, &q4);
                PaneMatSetTevColors(mSlotPane15, &q1, &q2);
            }
            {
                GXColorS10 q1 = *(GXColorS10*)&itemBoxInfo_labelNeutralTop;
                GXColorS10 q2 = *(GXColorS10*)&itemBoxInfo_labelNeutralBottom;
                GXColorS10 q3 = *(GXColorS10*)&itemBoxInfo_valueNeutralTop;
                GXColorS10 q4 = *(GXColorS10*)&itemBoxInfo_valueNeutralBottom;
                if (dW < 0) {
                    __as__11_GXColorS10FRC11_GXColorS10(&q1, &itemBoxInfo_labelDownTop);
                    __as__11_GXColorS10FRC11_GXColorS10(&q2, &itemBoxInfo_labelDownBottom);
                    __as__11_GXColorS10FRC11_GXColorS10(&q3, &itemBoxInfo_valueDownTop);
                    __as__11_GXColorS10FRC11_GXColorS10(&q4, &itemBoxInfo_valueDownBottom);
                } else if (dW > 0) {
                    __as__11_GXColorS10FRC11_GXColorS10(&q1, &itemBoxInfo_labelUpTop);
                    __as__11_GXColorS10FRC11_GXColorS10(&q2, &itemBoxInfo_labelUpBottom);
                    __as__11_GXColorS10FRC11_GXColorS10(&q3, &itemBoxInfo_valueUpTop);
                    __as__11_GXColorS10FRC11_GXColorS10(&q4, &itemBoxInfo_valueUpBottom);
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
                case 4: slotId = ((CItemBoxCharEntry*)charObj)->slotIds[0]; break;
                case 5: slotId = ((CItemBoxCharEntry*)charObj)->slotIds[1]; break;
                case 6: slotId = ((CItemBoxCharEntry*)charObj)->slotIds[2]; break;
                case 7: slotId = ((CItemBoxCharEntry*)charObj)->slotIds[3]; break;
                case 8: slotId = ((CItemBoxCharEntry*)charObj)->slotIds[4]; break;
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
            u8 arm1 = (u8)BdatGetU8Direct((const void*)::cf::CfBdat::spItmEquipListFileData, "arm_type", BdatGetItemId((u32)w0));
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
            u8 arm2 = (u8)BdatGetU8Direct((const void*)::cf::CfBdat::spItmEquipListFileData, "arm_type", BdatGetItemId((u32)(*(u32*)arg3 >> 20)));
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
            s32 ssum = (s32)((CItemBoxCharEntry*)charObj)->slotIds[3] + ((CItemBoxCharEntry*)charObj)->slotIds[2] + ((CItemBoxCharEntry*)charObj)->slotIds[4] + ((CItemBoxCharEntry*)charObj)->slotIds[0] + ((CItemBoxCharEntry*)charObj)->slotIds[1];
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
            void* item2 = CItem_kindPtr(2, ((CItemBoxCharEntry*)charObj)->slotIds[5]);
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
                fmtBuf.format("%d%s%d", wmin, BdatTouchStringCell("MNU_item", "name", 0xB), wd);
                SetLayoutSelfText((nw4r::lyt::Layout*)mSlotPane10, fmtBuf.c_str(), 0);
            }
            // numbers
            LayoutSetTextBoxInt((nw4r::lyt::Layout*)mSlotPane12, (s16)(u16)c_new.w04);
            LayoutSetTextBoxInt((nw4r::lyt::Layout*)mSlotPane14, (s16)c_new.w14);
            LayoutSetTextBoxInt((nw4r::lyt::Layout*)mSlotPane6, (s16)b14);
            // 20x color application (selected palette A8/B0/C8/D0)
            GXColorS10 q1 = *(GXColorS10*)&itemBoxInfo_slotValueTop;
            GXColorS10 q2 = *(GXColorS10*)&itemBoxInfo_slotValueBottom;
            GXColorS10 q3 = *(GXColorS10*)&itemBoxInfo_nameValueTop;
            GXColorS10 q4 = *(GXColorS10*)&itemBoxInfo_nameValueBottom;
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
        GXColorS10 q1 = *(GXColorS10*)&itemBoxInfo_slotValueTop;
        GXColorS10 q2 = *(GXColorS10*)&itemBoxInfo_slotValueBottom;
        GXColorS10 q3 = *(GXColorS10*)&itemBoxInfo_nameValueTop;
        GXColorS10 q4 = *(GXColorS10*)&itemBoxInfo_nameValueBottom;
        SetLayoutSelfText((nw4r::lyt::Layout*)mSlotPane0, "", 0);
        SetLayoutSelfText((nw4r::lyt::Layout*)mSlotPane2, "", 0);
        SetLayoutSelfText((nw4r::lyt::Layout*)mSlotPane3, "", 0);
        PaneMatSetTevColors(mSlotPane1, &q1, &q2);
        SetLayoutSelfText((nw4r::lyt::Layout*)mSlotPane4, "", 0);
        PaneMatSetTevColors(mSlotPane4, &q3, &q4);
        PaneMatSetTevColors(mSlotPane5, &q1, &q2);
        SetLayoutSelfText((nw4r::lyt::Layout*)mSlotPane6, "", 0);
        PaneMatSetTevColors(mSlotPane6, &q3, &q4);
        PaneMatSetTevColors(mSlotPane7, &q1, &q2);
        SetLayoutSelfText((nw4r::lyt::Layout*)mSlotPane8, "", 0);
        PaneMatSetTevColors(mSlotPane8, &q3, &q4);
        PaneMatSetTevColors(mSlotPane9, &q1, &q2);
        SetLayoutSelfText((nw4r::lyt::Layout*)mSlotPane10, "", 0);
        PaneMatSetTevColors(mSlotPane10, &q3, &q4);
        PaneMatSetTevColors(mSlotPane11, &q1, &q2);
        SetLayoutSelfText((nw4r::lyt::Layout*)mSlotPane12, "", 0);
        PaneMatSetTevColors(mSlotPane12, &q3, &q4);
        PaneMatSetTevColors(mSlotPane13, &q1, &q2);
        SetLayoutSelfText((nw4r::lyt::Layout*)mSlotPane14, "", 0);
        PaneMatSetTevColors(mSlotPane14, &q3, &q4);
        PaneMatSetTevColors(mSlotPane15, &q1, &q2);
        SetLayoutSelfText((nw4r::lyt::Layout*)mSlotPane16, "", 0);
        PaneMatSetTevColors(mSlotPane16, &q3, &q4);
        PaneMatSetTevColors(mSlotPane17, &q1, &q2);
        SetLayoutSelfText((nw4r::lyt::Layout*)mSlotPane18, "", 0);
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
        void* itemW = CItem_kindPtr(2, ((CItemBoxCharEntry*)charObj)->slotIds[5]);
        void* items[5];
        items[0] = CItem_kindPtr(4, ((CItemBoxCharEntry*)charObj)->slotIds[0]);
        items[1] = CItem_kindPtr(5, ((CItemBoxCharEntry*)charObj)->slotIds[1]);
        items[2] = CItem_kindPtr(6, ((CItemBoxCharEntry*)charObj)->slotIds[2]);
        items[3] = CItem_kindPtr(7, ((CItemBoxCharEntry*)charObj)->slotIds[3]);
        items[4] = CItem_kindPtr(8, ((CItemBoxCharEntry*)charObj)->slotIds[4]);
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
            u8 cnt = (u8)BdatGetU8Direct((const void*)::cf::CfBdat::spItmWpnListFileData, "jwl_slot", BdatGetItemId(*(u32*)arg3 >> 20));
            for (u8 i = 0; i < 3; i++) {
                ItemBoxInfoCopy tmp;
                tmp.itemId = 0;
                tmp.value = 0;
                tmp.state = 0;
                if (i < cnt) {
                    char buf[0x20];
                    sprintf(buf, "jwl_skill%d", i + 1);
                    u32 id = BdatGetU16Direct(::cf::CfBdat::spItmWpnListFileData, buf, BdatGetItemId(*(u32*)arg3 >> 20));
                    u16 nameId = (u16)BdatGetU16Direct(::cf::CfBdat::spItmItemListFileData, "itemID", id);
                    u8 icon = (u8)BdatGetU8Direct((const void*)::cf::CfBdat::spItmItemListFileData, "percent", id);
                    u8 val = (u8)BdatGetU8Direct((const void*)::cf::CfBdat::spItmItemListFileData, "rankType", id);
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
                        if (BdatGetU8Direct((const void*)::cf::CfBdat::spItmEquipListFileData, "jwl_slot", BdatGetItemId(*(u32*)arg3 >> 20)) != 0) {
                            u16 nameId = (u16)BdatGetU16Direct(::cf::CfBdat::spItmEquipListFileData, "jwl_skill1", BdatGetItemId(*(u32*)arg3 >> 20));
                            u16 capId = (u16)BdatGetU16Direct(::cf::CfBdat::spItmItemListFileData, "itemID", nameId);
                            u8 icon = (u8)BdatGetU8Direct((const void*)::cf::CfBdat::spItmItemListFileData, "percent", nameId);
                            u8 val = (u8)BdatGetU8Direct((const void*)::cf::CfBdat::spItmItemListFileData, "rankType", nameId);
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
            } else if (BdatGetU8Direct((const void*)::cf::CfBdat::spBtlSkillListFileData, "accum", id) != 0) {
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
                u32 cap = BdatGetU16Direct(::cf::CfBdat::spBtlSkillListFileData, "max", ids[i]);
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
            } else if (BdatGetU8Direct((const void*)::cf::CfBdat::spBtlSkillListFileData, "accum", id) != 0) {
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
                u32 cap = BdatGetU16Direct(::cf::CfBdat::spBtlSkillListFileData, "max", ids2[i]);
                if (vals2[i] > cap) vals2[i] = cap;
            }
        }

        // better/worse flags comparing listB vs listA
        u8 cmp[8];
        for (u8 i = 0; i < 8; i++) {
            if (ids2[i] != 0 && BdatGetU8Direct((const void*)::cf::CfBdat::spBtlSkillListFileData, "accum", ids2[i]) != 0) {
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
            if (ids2[i] != 0 && BdatGetU8Direct((const void*)::cf::CfBdat::spBtlSkillListFileData, "accum", ids2[i]) != 0) {
                for (u8 j = i + 1; j < 8; j++) {
                    if (ids2[i] == ids2[j]) ids2[j] = 0;
                }
            }
        }

        // pane name setup
        nw4r::lyt::Pane* root = mpLayout->GetRootPane()->FindPaneByName("nul_proportion", true);
        for (u8 i = 0; i < 8; i++) {
            char buf1[0x20];
            char buf2[0x20];
            sprintf(buf1, "txt_para7%02d", i * 2 + 0x13);
            sprintf(buf2, "txt_value7%02d", i + 0x10);
            LayoutSetTextBoxFmtValue(mpLayout, buf1, "", 0);
            LayoutSetTextBoxFmtValue(mpLayout, buf2, "", 0);
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
            sprintf(buf1, "txt_para7%02d", i * 2 + 0x13);
            sprintf(buf2, "txt_value7%02d", i + 0x10);
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
            LayoutSetTextBoxFmtValue(mpLayout, buf1, BdatGetPtrDirect(::cf::CfBdat::spBtlSkillListFileData, "name", id), 0);
            if (val == 0) {
                LayoutSetTextBoxFmtValue(mpLayout, buf2, "", 0);
            } else {
                u8 rvs = (u8)BdatGetU8Direct((const void*)::cf::CfBdat::spBtlSkillListFileData, "rvs_type", id);
                if (rvs != 0 && (rvs == 0xFF || rvs == 0xFE)) {
                    ml::FixStr<32> fmtBuf;
                    fmtBuf.format("%d%s", (s32)val, BdatTouchStringCell("MNU_item", "name", 0x21));
                    LayoutSetTextBoxFmtValue(mpLayout, buf2, fmtBuf.c_str(), 0);
                } else {
                    ml::FixStr<32> fmtBuf;
                    fmtBuf.format("%d ", (s32)val);
                    LayoutSetTextBoxFmtValue(mpLayout, buf2, fmtBuf.c_str(), 0);
                }
            }
            if (arg5 == 0) {
                LayoutSetTextBoxFmtValue(mpLayout, buf2, "", 0);
            }
            GXColorS10 q1 = *(GXColorS10*)&itemBoxInfo_labelNeutralTop;
            GXColorS10 q2 = *(GXColorS10*)&itemBoxInfo_labelNeutralBottom;
            GXColorS10 q3 = *(GXColorS10*)&itemBoxInfo_valueNeutralTop;
            GXColorS10 q4 = *(GXColorS10*)&itemBoxInfo_valueNeutralBottom;
            u32 cap = BdatGetU16Direct(::cf::CfBdat::spBtlSkillListFileData, "max", id);
            if ((u32)val >= cap) {
                __as__11_GXColorS10FRC11_GXColorS10(&q1, &itemBoxInfo_labelSpecialTop);
                __as__11_GXColorS10FRC11_GXColorS10(&q2, &itemBoxInfo_labelSpecialBottom);
                __as__11_GXColorS10FRC11_GXColorS10(&q3, &itemBoxInfo_valueSpecialTop);
                __as__11_GXColorS10FRC11_GXColorS10(&q4, &itemBoxInfo_valueSpecialBottom);
            } else if (cf == 1) {
                __as__11_GXColorS10FRC11_GXColorS10(&q1, &itemBoxInfo_labelUpTop);
                __as__11_GXColorS10FRC11_GXColorS10(&q2, &itemBoxInfo_labelUpBottom);
                __as__11_GXColorS10FRC11_GXColorS10(&q3, &itemBoxInfo_valueUpTop);
                __as__11_GXColorS10FRC11_GXColorS10(&q4, &itemBoxInfo_valueUpBottom);
            } else if (cf == 2) {
                __as__11_GXColorS10FRC11_GXColorS10(&q1, &itemBoxInfo_labelDownTop);
                __as__11_GXColorS10FRC11_GXColorS10(&q2, &itemBoxInfo_labelDownBottom);
                __as__11_GXColorS10FRC11_GXColorS10(&q3, &itemBoxInfo_valueDownTop);
                __as__11_GXColorS10FRC11_GXColorS10(&q4, &itemBoxInfo_valueDownBottom);
            }
            if (arg5 == 0) {
                __as__11_GXColorS10FRC11_GXColorS10(&q1, &itemBoxInfo_slotValueTop);
                __as__11_GXColorS10FRC11_GXColorS10(&q2, &itemBoxInfo_slotValueBottom);
                __as__11_GXColorS10FRC11_GXColorS10(&q3, &itemBoxInfo_nameValueTop);
                __as__11_GXColorS10FRC11_GXColorS10(&q4, &itemBoxInfo_nameValueBottom);
            }
            PaneMatSetTevColors(pane1, &q1, &q2);
            PaneMatSetTevColors(pane2, &q3, &q4);
        }
    }
}
void copyItemBoxCopy(ItemBoxInfoCopy* dst, const ItemBoxInfoCopy* src) {
    dst->itemId = src->itemId;
    dst->value = src->value;
    dst->state = src->state;
}

void setItemBoxCopy(ItemBoxInfoCopy* self, unsigned short a, unsigned int b, unsigned char c) {
    self->itemId = a;
    self->value = b;
    self->state = c;
}




s32 CItemBoxInfo2::CalcBonus1(u16 b, s32 arg2, void* d) {
    // Single-expression sum; builtin s32->f32 cast emits MWCC's
    // 0x4330/xoris double-trick.
    // b is the u16 character id: the u16 param fixes the param-save order.
    s32 prod = arg2 * (s32)(GetEffect(b, 0x52, d) + GetCondStat2D((void*)(u32)b) + 0x64);
    return (s32)(0.01f * (f32)prod);
}

s32 CItemBoxInfo2::CalcBonus2(u16 b, s32 arg2, void* d) {
    // Standalone SDA (not sdata2_ItemBox members) so lfd/lfs use r0@sda21
    // instead of li r3,0 / lfs 56(r3); u16 b gives retail or r28,r4 first.
    s32 prod = (s32)(GetEffect(b, 0x53, d) + GetCondStat2D((void*)(u32)b) + 0x64) * arg2;
    return (s32)(0.01f * (f32)prod);
}

u32 CItemBoxInfo2::GetCondStat2D(void* arg1) {
    void* obj = (u8*)CtrlObjectParam_GetCharEntry((u8)(u32)arg1) + 0x3534;
    if (Counter_TestBit(obj, 0x2d) != 0) {
        return IdTable_SumValues((CUnkObj8025FB10*)obj, 0x2d);
    }
    return 0;
}
// Retail getItemBox2Effect: the ItemBox2 variant of getItemBoxEffect. Six slots are
// scanned via the lookup table (s16 ids at +0x26/+0x1C..0x24, 6-byte slot
// table from .sdata2). When the slot's byte equals the candidate's equip id
// ((u8)BdatGetItemType((u32)arg3)), a name-based lookup runs instead of the item
// walk: count = BdatGetU8Direct(v1, "jwl_slot", v2), then for each index the
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
    s16 slotIds[6];
    slotIds[0] = ((CItemBoxCharEntry*)lookup)->slotIds[5];
    slotIds[1] = ((CItemBoxCharEntry*)lookup)->slotIds[0];
    slotIds[2] = ((CItemBoxCharEntry*)lookup)->slotIds[1];
    slotIds[3] = ((CItemBoxCharEntry*)lookup)->slotIds[2];
    slotIds[4] = ((CItemBoxCharEntry*)lookup)->slotIds[3];
    slotIds[5] = ((CItemBoxCharEntry*)lookup)->slotIds[4];
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
                u8 count = (u8)BdatGetU8Direct((const void*)nameObj, "jwl_slot", (u16)nameId);
                for (u8 j = 0; (u8)j < count; j++) {
                    sprintf(buf, "jwl_skill%d", (u8)j + 1);
                    u32 v = BdatGetU16Direct((char*)nameObj, buf, nameId);
                    if ((u16)v != 0) {
                        u16 cat = (u16)BdatGetItemId(v);
                        if (c == cat) {
                            result += (u8)BdatGetU8Direct((const void*)::cf::CfBdat::spItmItemListFileData, "percent", (u16)v);
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
                u8 count = (u8)BdatGetU8Direct((const void*)nameObj, "jwl_slot", (u16)nameId);
                for (u8 j = 0; (u8)j < count; j++) {
                    sprintf(buf, "jwl_skill%d", (u8)j + 1);
                    u32 v = BdatGetU16Direct((char*)nameObj, buf, nameId);
                    if ((u16)v != 0) {
                        u16 cat = (u16)BdatGetItemId(v);
                        if (c == cat) {
                            result += (u8)BdatGetU8Direct((const void*)::cf::CfBdat::spItmItemListFileData, "percent", (u16)v);
                        }
                    }
                }
            }
        }
    }
    u16 cap = (u16)BdatGetU16Direct(::cf::CfBdat::spBtlSkillListFileData, (char*)"max", c);
    if ((s16)cap < (s32)result) result = cap;
    return result;
}

u32 CItemBoxInfo2::GetCondStatById(u32 arg1, u32 arg2) {
    // Retail keeps obj in r31 and result in r30 (obj declared first gets the
    // highest saved register); the result=0 init lands before the call.
    void* obj;
    u32 result = 0;
    obj = ((CItemBoxCharEntry*)CtrlObjectParam_GetCharEntry(arg1))->counterArea;
    if (Counter_TestBit(obj, arg2) != 0) {
        result = IdTable_SumValues((CUnkObj8025FB10*)obj, arg2);
    }
    return result;
}
u32 CItemBoxInfo2::TestBdatFlag(u32 arg1, u32 arg2) {
    if (arg1 == 0 || arg2 == 0) return 0;
    char buf[0x20];
    sprintf(buf, "pc%d", arg1);
    u32 g = (u32)::cf::CfBdat::spItmEquipListFileData;
    return (u8)BdatGetU8Direct((const void*)g, (const char*)buf, (u16)BdatGetItemId(arg2)) != 0;
}
u32 CItemBoxInfo2::SumTypeCounts(u16 arg2, void* arg3) {
    if (arg2 == 0) return 0;
    void* g = ::cf::CfBdat::spItmEquipListFileData;
    u16 v1 = arg3 ? (u16)BdatGetItemType((u32)arg3) : (u16)0;
    u16 v2 = arg3 ? (u16)BdatGetItemId((u32)arg3) : (u16)0;
    void* lookup = CtrlObjectParam_GetCharEntry(arg2);
    u32 result = 0;
    for (u32 i = 4; i <= 8; i++) {
        u8 ii = (u8)i;
        if (ii == v1) continue;
        s16 val = -1;
        switch (ii) {
            case 4: val = ((CItemBoxCharEntry*)lookup)->slotIds[0]; break;
            case 5: val = ((CItemBoxCharEntry*)lookup)->slotIds[1]; break;
            case 6: val = ((CItemBoxCharEntry*)lookup)->slotIds[2]; break;
            case 7: val = ((CItemBoxCharEntry*)lookup)->slotIds[3]; break;
            case 8: val = ((CItemBoxCharEntry*)lookup)->slotIds[4]; break;
        }
        if (val == -1) continue;
        // Retail passes the slot id (u8)i and the slot value val (2-arg form).
        void* r = CItem_kindPtr((u8)i, val);
        if (r == NULL) continue;
        u32 v = ((CItemData*)r)->field_00;
        if (v == 0) continue;
        result += (u8)BdatGetU8Direct((const void*)g, "eva_rate", BdatGetItemId(v >> 20));
    }
    if (arg3 != NULL) {
        result += (u8)BdatGetU8Direct((const void*)g, "eva_rate", v2);
    }
    return result;
}
bool CItemBoxInfo2::TestEquip(u16 b, void* c) {
    // Validates every equip slot against candidate item c before an exchange:
    // for each of the six slots either the indexed item names must still
    // resolve (when the slot holds the equipped item) or no indexed entry may
    // be occupied/in-use.
    CItemBoxCharEntry* lookup = (CItemBoxCharEntry*)CtrlObjectParam_GetCharEntry(b);
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
                u8 j; u8 count = (u8)BdatGetU8Direct((const void*)nameObj, "jwl_slot", nameId);
                for (j = 0; j < count; j++) {
                    sprintf(buf, "jwl_skill%d", j + 1);
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
                u8 j; u8 count = (u8)BdatGetU8Direct((const void*)nameObj, "jwl_slot", nameId);
                for (j = 0; j < count; j++) {
                    sprintf(buf, "jwl_skill%d", j + 1);
                    if ((u16)BdatGetU16Direct((void*)nameObj, buf, nameId) != 0) return false;
                }
            }
        }
    }
    return true;
}

// Retail CItemBoxInfo2::OnFileEvent: two file-load branches. The line-arc
// branch (fileHandle1) builds the whole layout in a 0x18000 scratch region;
// the common-arc branch (fileHandle2) only re-attaches its accessor into a
// 0x100 scratch region and refreshes via activateItemBox2.
bool CItemBoxInfo2::OnFileEvent(CEventFile* file) {
    if (mFileHandle1 == file->mFileHandle) {
        mRegion1.createRegion(mtl::MemManager::getHandleMEM2(), 0x18000,
                                      "CItemBoxInfo2", 1);
        Class_8045F858 host(&mRegion1);
        u8* data = (u8*)mFileHandle1->getData();
        mtl::MemManager::setMemInitFlag(false);
        nw4r::lyt::ArcResourceAccessor* acc = CLibLayout::createArcResourceAccessor();
        mArcResAcc = acc;
        acc->Attach(data, "arc");
        buildLayout(&mpLayout, acc, "mf02_box02_inf.brlyt");
        bindLayoutAnimTransform(mpLayout, &mAnimIn, acc, "mf02_box02_inf_in.brlan");
        bindLayoutAnimTransform(mpLayout, &mAnimInfoIn, acc, "mf02_box02_inf_info_in.brlan");
        nw4r::lyt::Pane* root = mpLayout->GetRootPane();
        void* fontObj = CDeviceFont::getFontInfo(1, mpLayout);
        ApplyFontToTxtPanes(root, (const nw4r::ut::Font*)(static_cast<IDeviceFontInfo*>(fontObj)->getFont()));

        // Seed the label textboxes with the shared text object.
        char* text = (char*)CUICfManager_getPackedFont9C();
        setLayoutTextBoxFont(mpLayout, "txt_value02", (u32)text);
        setLayoutTextBoxFont(mpLayout, "txt_value08", (u32)text);
        setLayoutTextBoxFont(mpLayout, "txt_value09", (u32)text);
        setLayoutTextBoxFont(mpLayout, "txt_value04", (u32)text);
        setLayoutTextBoxFont(mpLayout, "txt_value07", (u32)text);
        setLayoutTextBoxFont(mpLayout, "txt_value22", (u32)text);
        setLayoutTextBoxFont(mpLayout, "txt_value50", (u32)text);
        setLayoutTextBoxFont(mpLayout, "txt_value51", (u32)text);
        setLayoutTextBoxFont(mpLayout, "txt_value30", (u32)text);
        setLayoutTextBoxFont(mpLayout, "txt_value31", (u32)text);
        setLayoutTextBoxFont(mpLayout, "txt_value32", (u32)text);
        setLayoutTextBoxFont(mpLayout, "txt_value33", (u32)text);
        setLayoutTextBoxFont(mpLayout, "txt_value34", (u32)text);
        setLayoutTextBoxFont(mpLayout, "txt_ethvalue01", (u32)text);
        setLayoutTextBoxFont(mpLayout, "txt_ethvalue02", (u32)text);
        setLayoutTextBoxFont(mpLayout, "txt_ethvalue03", (u32)text);
        setLayoutTextBoxFont(mpLayout, "txt_gold01_00", (u32)text);
        setLayoutTextBoxFont(mpLayout, "txt_value700", (u32)text);
        setLayoutTextBoxFont(mpLayout, "txt_value702", (u32)text);
        setLayoutTextBoxFont(mpLayout, "txt_value703", (u32)text);
        setLayoutTextBoxFont(mpLayout, "txt_value704", (u32)text);
        setLayoutTextBoxFont(mpLayout, "txt_value705", (u32)text);
        setLayoutTextBoxFont(mpLayout, "txt_value708", (u32)text);
        setLayoutTextBoxFont(mpLayout, "txt_value710", (u32)text);
        setLayoutTextBoxFont(mpLayout, "txt_value706", (u32)text);
        setLayoutTextBoxFont(mpLayout, "txt_value709", (u32)text);
        setLayoutTextBoxFont(mpLayout, "txt_value712", (u32)text);
        setLayoutTextBoxFont(mpLayout, "txt_value714", (u32)text);
        setLayoutTextBoxFont(mpLayout, "txt_value715", (u32)text);
        setLayoutTextBoxFont(mpLayout, "txt_value716", (u32)text);
        setLayoutTextBoxFont(mpLayout, "txt_value717", (u32)text);
        setLayoutTextBoxFont(mpLayout, "txt_value718", (u32)text);
        setLayoutTextBoxFont(mpLayout, "txt_value719", (u32)text);
        setLayoutTextBoxFont(mpLayout, "txt_value720", (u32)text);
        setLayoutTextBoxFont(mpLayout, "txt_value721", (u32)text);
        setLayoutTextBoxFont(mpLayout, "txt_value722", (u32)text);
        setLayoutTextBoxFont(mpLayout, "txt_value723", (u32)text);
        setLayoutTextBoxFont(mpLayout, "txt_para704", (u32)text);

        mpLayout->SetAnimationEnable(mAnimInfoIn, false);
        mpLayout->SetAnimationEnable(mAnimIn, true);
        mpLayout->Animate(0);

        u8 mode = mKind;

        // Mode-gated pane clears (retail: mode!=1&&mode!=2 / mode!=3).
        if (mode != 1 && mode != 2) {
            setPaneVisible(root->FindPaneByName("nul_shop", true), 0);
        }
        if (mode != 3) {
            setPaneVisible(root->FindPaneByName("nul_excange", true), 0);
        }

        // Fixed label texts: each pane gets string-table entry [len].
        LayoutSetTextBoxFmtValue((nw4r::lyt::Layout*)mpLayout, "txt_para01", BdatTouchStringCell("MNU_item", "name", 10), 0);
        LayoutSetTextBoxFmtValue((nw4r::lyt::Layout*)mpLayout, "txt_para04", BdatTouchStringCell("MNU_item", "name", 13), 0);
        LayoutSetTextBoxFmtValue((nw4r::lyt::Layout*)mpLayout, "txt_para08", BdatTouchStringCell("MNU_item", "name", 17), 0);
        LayoutSetTextBoxFmtValue((nw4r::lyt::Layout*)mpLayout, "txt_para10", BdatTouchStringCell("MNU_item", "name", 18), 0);
        LayoutSetTextBoxFmtValue((nw4r::lyt::Layout*)mpLayout, "txt_para11", BdatTouchStringCell("MNU_item", "name", 12), 0);
        LayoutSetTextBoxFmtValue((nw4r::lyt::Layout*)mpLayout, "txt_para22", BdatTouchStringCell("MNU_item", "name", 19), 0);
        LayoutSetTextBoxFmtValue((nw4r::lyt::Layout*)mpLayout, "txt_para30", BdatTouchStringCell("MNU_item", "name", 24), 0);
        LayoutSetTextBoxFmtValue((nw4r::lyt::Layout*)mpLayout, "txt_para50", BdatTouchStringCell("MNU_item", "name", 24), 0);
        LayoutSetTextBoxFmtValue((nw4r::lyt::Layout*)mpLayout, "txt_para60", BdatTouchStringCell("MNU_item", "name", 36), 0);
        LayoutSetTextBoxFmtValue((nw4r::lyt::Layout*)mpLayout, "txt_para61", BdatTouchStringCell("MNU_item", "name", 37), 0);
        LayoutSetTextBoxFmtValue((nw4r::lyt::Layout*)mpLayout, "txt_para700", BdatTouchStringCell("MNU_item", "name", 112), 0);
        LayoutSetTextBoxFmtValue((nw4r::lyt::Layout*)mpLayout, "txt_para702", BdatTouchStringCell("MNU_item", "name", 114), 0);
        LayoutSetTextBoxFmtValue((nw4r::lyt::Layout*)mpLayout, "txt_para703", BdatTouchStringCell("MNU_item", "name", 115), 0);
        LayoutSetTextBoxFmtValue((nw4r::lyt::Layout*)mpLayout, "txt_para705", BdatTouchStringCell("MNU_item", "name", 116), 0);
        LayoutSetTextBoxFmtValue((nw4r::lyt::Layout*)mpLayout, "txt_para706", BdatTouchStringCell("MNU_item", "name", 121), 0);
        LayoutSetTextBoxFmtValue((nw4r::lyt::Layout*)mpLayout, "txt_para708", BdatTouchStringCell("MNU_item", "name", 117), 0);
        LayoutSetTextBoxFmtValue((nw4r::lyt::Layout*)mpLayout, "txt_para709", BdatTouchStringCell("MNU_item", "name", 123), 0);
        LayoutSetTextBoxFmtValue((nw4r::lyt::Layout*)mpLayout, "txt_para710", BdatTouchStringCell("MNU_item", "name", 118), 0);
        LayoutSetTextBoxFmtValue((nw4r::lyt::Layout*)mpLayout, "txt_para713", BdatTouchStringCell("MNU_item", "name", 119), 0);
        LayoutSetTextBoxFmtValue((nw4r::lyt::Layout*)mpLayout, "txt_para716", BdatTouchStringCell("MNU_item", "name", 126), 0);
        LayoutSetTextBoxFmtValue((nw4r::lyt::Layout*)mpLayout, "txt_para717", BdatTouchStringCell("MNU_item", "name", 130), 0);
        LayoutSetTextBoxFmtValue((nw4r::lyt::Layout*)mpLayout, "txt_para23", BdatTouchStringCell("MNU_item", "name", 47), 0);
        LayoutSetTextBoxFmtValue((nw4r::lyt::Layout*)mpLayout, "txt_para704", BdatTouchStringCell("MNU_item", "name", 130), 0);

        // Per-mode caption / detail text.
        if (mode == 1 || mode == 2) {
            ml::FixStr<32> caption(true);
            caption.format("%d%s", CItemBlock_getPtr20E8(),
                           BdatTouchStringCell("MNU_item", "name", 3));
            LayoutSetTextBoxFmtValue((nw4r::lyt::Layout*)mpLayout, "txt_gold01_00",
                          caption.c_str(), 0);
        } else if (mode == 3) {
            u16 itemId = mMode;
            u8 kind = (u8)MapSexFlagGroup(itemId);
            char* s;
            if (kind == 0) {
                s = BdatTouchStringCell("MNU_relate", "name", 0x24);
            } else {
                s = BdatTouchStringCell("MNU_relate", "name", kind + 0x1F);
            }
            LayoutSetTextBoxFmtValue((nw4r::lyt::Layout*)mpLayout, "txt_npctype", s, 0);
            char* name = ((char* (*)(void*, const char*, u16))&BdatGetPtrDirect)(
                &::cf::CfBdat::spFldNpcListFileData, "name", itemId);
            LayoutSetTextBoxFmtValue((nw4r::lyt::Layout*)mpLayout, "txt_npcname",
                          GetBdatNameByNumId(name), 0);
            LayoutSetTextBoxFmtValue((nw4r::lyt::Layout*)mpLayout, "txt_npcframe",
                          BdatTouchStringCell("MNU_item", "name", 0x2d), 0);
            LayoutSetTextBoxFmtValue((nw4r::lyt::Layout*)mpLayout, "txt_excange00",
                          "", 0);
            LayoutSetTextBoxFmtValue((nw4r::lyt::Layout*)mpLayout, "txt_excange01",
                          "", 0);
        }

        // Vertex colours: highlight pair onto this (+0x9C/+0xA4); the named
        // panes' pairs go into the .sbss colour globals with their alpha
        // halves propagated to the paired globals.
        nw4r::lyt::Pane* hl = root->FindPaneByName("pic_pcbs01", true);
        GXColorS10 c1 = PaneMatGetS16Quad(hl, 0);
        __as__11_GXColorS10FRC11_GXColorS10(&mColorA, &c1);
        GXColorS10 c2 = PaneMatGetS16Quad(hl, 1);
        __as__11_GXColorS10FRC11_GXColorS10(&mColorB, &c2);

        nw4r::lyt::Pane* p71e = root->FindPaneByName("txt_para700", true);
        GXColorS10 c3 = PaneMatGetS16Quad(p71e, 0);
        __as__11_GXColorS10FRC11_GXColorS10(&itemBoxInfo_labelNeutralTop, &c3);
        GXColorS10 c4 = PaneMatGetS16Quad(p71e, 1);
        __as__11_GXColorS10FRC11_GXColorS10(&itemBoxInfo_labelNeutralBottom, &c4);
        {
            s16 a0 = *(s16*)((char*)&itemBoxInfo_labelNeutralTop + 6);
            s16 a1 = *(s16*)((char*)&itemBoxInfo_labelNeutralBottom + 6);
            *(s16*)((char*)&itemBoxInfo_labelUpTop + 6) = a0;
            *(s16*)((char*)&itemBoxInfo_labelUpBottom + 6) = a1;
            *(s16*)((char*)&itemBoxInfo_labelDownTop + 6) = a0;
            *(s16*)((char*)&itemBoxInfo_labelDownBottom + 6) = a1;
            *(s16*)((char*)&itemBoxInfo_labelSpecialTop + 6) = a0;
            *(s16*)((char*)&itemBoxInfo_labelSpecialBottom + 6) = a1;
        }

        nw4r::lyt::Pane* p4e3 = root->FindPaneByName("txt_value700", true);
        GXColorS10 c5 = PaneMatGetS16Quad(p4e3, 0);
        __as__11_GXColorS10FRC11_GXColorS10(&itemBoxInfo_valueNeutralTop, &c5);
        GXColorS10 c6 = PaneMatGetS16Quad(p4e3, 1);
        __as__11_GXColorS10FRC11_GXColorS10(&itemBoxInfo_valueNeutralBottom, &c6);
        {
            s16 a0 = *(s16*)((char*)&itemBoxInfo_valueNeutralTop + 6);
            s16 a1 = *(s16*)((char*)&itemBoxInfo_valueNeutralBottom + 6);
            *(s16*)((char*)&itemBoxInfo_valueUpTop + 6) = a0;
            *(s16*)((char*)&itemBoxInfo_valueUpBottom + 6) = a1;
            *(s16*)((char*)&itemBoxInfo_valueDownTop + 6) = a0;
            *(s16*)((char*)&itemBoxInfo_valueDownBottom + 6) = a1;
            *(s16*)((char*)&itemBoxInfo_valueSpecialTop + 6) = a0;
            *(s16*)((char*)&itemBoxInfo_valueSpecialBottom + 6) = a1;
        }

        nw4r::lyt::Pane* p2b6 = root->FindPaneByName("txt_para01", true);
        GXColorS10 c7 = PaneMatGetS16Quad(p2b6, 0);
        __as__11_GXColorS10FRC11_GXColorS10(&itemBoxInfo_slotLabelTop, &c7);
        GXColorS10 c8 = PaneMatGetS16Quad(p2b6, 1);
        __as__11_GXColorS10FRC11_GXColorS10(&itemBoxInfo_slotLabelBottom, &c8);
        *(s16*)((char*)&itemBoxInfo_slotValueTop + 6) = *(s16*)((char*)&itemBoxInfo_slotLabelTop + 6);
        *(s16*)((char*)&itemBoxInfo_slotValueBottom + 6) = *(s16*)((char*)&itemBoxInfo_slotLabelBottom + 6);

        nw4r::lyt::Pane* p25b = root->FindPaneByName("txt_value02", true);
        GXColorS10 c9 = PaneMatGetS16Quad(p25b, 0);
        __as__11_GXColorS10FRC11_GXColorS10(&itemBoxInfo_nameLabelTop, &c9);
        GXColorS10 c10 = PaneMatGetS16Quad(p25b, 1);
        __as__11_GXColorS10FRC11_GXColorS10(&itemBoxInfo_nameLabelBottom, &c10);
        *(s16*)((char*)&itemBoxInfo_nameValueTop + 6) = *(s16*)((char*)&itemBoxInfo_nameLabelTop + 6);
        *(s16*)((char*)&itemBoxInfo_nameValueBottom + 6) = *(s16*)((char*)&itemBoxInfo_nameLabelBottom + 6);

        StampSlotTex();

        // Pane clears for the selection view.
        setPaneVisible(root->FindPaneByName("nul_para_01", true), 0);
        setPaneVisible(root->FindPaneByName("nul_para_30", true), 0);
        setPaneVisible(root->FindPaneByName("nul_para_50", true), 0);
        setPaneVisible(root->FindPaneByName("nul_para_60", true), 0);
        setPaneVisible(root->FindPaneByName("nul_para_80", true), 0);
        setPaneVisible(root->FindPaneByName("nul_para_700", true), 1);
        setPaneVisible(root->FindPaneByName("nul_eth", true), 0);
        setPaneVisible(root->FindPaneByName("nul_infbcln01", true), 0);
        setPaneVisible(root->FindPaneByName("nul_infbcln700", true), 1);
        setPaneVisible(root->FindPaneByName("nul_scnd", true), 0);

        // Cache the 20 slot panes at +0x40..+0x8C for the renderer (retail
        // keeps the layout's root pane live in r28 across all lookups).
        mSlotPane0 = root->FindPaneByName("txt_value703", true);
        mSlotPane1 = root->FindPaneByName("txt_para703", true);
        mSlotPane2 = root->FindPaneByName("txt_value704", true);
        mSlotPane3 = root->FindPaneByName("txt_para704", true);
        mSlotPane4 = root->FindPaneByName("txt_value705", true);
        mSlotPane5 = root->FindPaneByName("txt_para705", true);
        mSlotPane6 = root->FindPaneByName("txt_value708", true);
        mSlotPane7 = root->FindPaneByName("txt_para708", true);
        mSlotPane8 = root->FindPaneByName("txt_value710", true);
        mSlotPane9 = root->FindPaneByName("txt_para710", true);
        mSlotPane10 = root->FindPaneByName("txt_value706", true);
        mSlotPane11 = root->FindPaneByName("txt_para706", true);
        mSlotPane12 = root->FindPaneByName("txt_value712", true);
        mSlotPane13 = root->FindPaneByName("txt_para713", true);
        mSlotPane14 = root->FindPaneByName("txt_value714", true);
        mSlotPane15 = root->FindPaneByName("txt_para716", true);
        mSlotPane16 = root->FindPaneByName("txt_value709", true);
        mSlotPane17 = root->FindPaneByName("txt_para709", true);
        mSlotPane18 = root->FindPaneByName("txt_value715", true);
        mSlotPane19 = root->FindPaneByName("txt_para717", true);

        Activate();
        mFileHandle1 = 0;
        mRegion1.validateHeap();
        return true;
    }
    if (mFileHandle2 == file->mFileHandle) {
        mRegion2.createRegion(mtl::MemManager::getHandleMEM2(), 0x100,
                                      "CItemBoxInfo2Tex", 1);
        Class_8045F858 host(&mRegion2);
        u8* data = (u8*)mFileHandle2->getData();
        mtl::MemManager::setMemInitFlag(false);
        nw4r::lyt::ArcResourceAccessor* acc2 = CLibLayout::createArcResourceAccessor();
        mTexArcResAcc = acc2;
        acc2->Attach(data, "arc");
        Activate();
        mFileHandle2 = 0;
        mRegion2.validateHeap();
        return true;
    }
    return false;
}


void sinit_801EABC4() {
    SplitU32ToS16s(&itemBoxInfo_labelNeutralTop, 0);
    SplitU32ToS16s(&itemBoxInfo_labelNeutralBottom, 0);
    setGXColorS10(&itemBoxInfo_labelUpTop, 0x25, 0x8a, 0xce, 0);
    setGXColorS10(&itemBoxInfo_labelUpBottom, 0x25, 0x8a, 0xce, 0);
    setGXColorS10(&itemBoxInfo_labelDownTop, 0xd2, 0x28, 0x14, 0);
    setGXColorS10(&itemBoxInfo_labelDownBottom, 0xd2, 0x28, 0x14, 0);
    setGXColorS10(&itemBoxInfo_labelSpecialTop, 0x1f, 0xb0, 0x1a, 0);
    setGXColorS10(&itemBoxInfo_labelSpecialBottom, 0x1f, 0xb0, 0x1a, 0);
    SplitU32ToS16s(&itemBoxInfo_valueNeutralTop, 0);
    SplitU32ToS16s(&itemBoxInfo_valueNeutralBottom, 0);
    setGXColorS10(&itemBoxInfo_valueUpTop, 0xff, 0xff, 0xfa, 0);
    setGXColorS10(&itemBoxInfo_valueUpBottom, 0x25, 0x8a, 0xce, 0);
    setGXColorS10(&itemBoxInfo_valueDownTop, 0xff, 0xff, 0xfa, 0);
    setGXColorS10(&itemBoxInfo_valueDownBottom, 0xd2, 0x28, 0x14, 0);
    setGXColorS10(&itemBoxInfo_valueSpecialTop, 0xff, 0xff, 0xfa, 0);
    setGXColorS10(&itemBoxInfo_valueSpecialBottom, 0x1f, 0xa6, 0x1a, 0);
    SplitU32ToS16s(&itemBoxInfo_slotLabelTop, 0);
    SplitU32ToS16s(&itemBoxInfo_slotLabelBottom, 0);
    setGXColorS10((GXColorS10*)&itemBoxInfo_slotValueTop, 0x80, 0x80, 0x80, 0);
    setGXColorS10((GXColorS10*)&itemBoxInfo_slotValueBottom, 0x80, 0x80, 0x80, 0);
    SplitU32ToS16s(&itemBoxInfo_nameLabelTop, 0);
    SplitU32ToS16s(&itemBoxInfo_nameLabelBottom, 0);
    setGXColorS10((GXColorS10*)&itemBoxInfo_nameValueTop, 0xff, 0xff, 0xfa, 0);
    setGXColorS10((GXColorS10*)&itemBoxInfo_nameValueBottom, 0x80, 0x80, 0x80, 0);
}
