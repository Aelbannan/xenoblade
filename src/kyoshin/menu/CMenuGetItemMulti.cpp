// Auto-scaffolded catalog TU for kyoshin/menu/CMenuGetItemMulti
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/menu/CMenuGetItemMulti.hpp"

#include "kyoshin/code_80135FDC.hpp"
// The battle-manager singleton getter has ONE canonical declaration in
// kyoshin/cf/CBattleManagerApi.hpp; no rename guard is needed anymore.
#include "kyoshin/cf/CBattleManagerApi.hpp"
#include "kyoshin/cf/CfGameManager.hpp"
#include "kyoshin/cf/CfPadData.hpp"
#include "monolib/device/CDeviceFile.hpp"
#include "monolib/device/CDeviceVI.hpp"
#include "monolib/lib/UnkClass_8045F564.hpp"
#include "monolib/scn/CScn.hpp"
#include "monolib/scn/IScnRender.hpp"
#include "monolib/util/MemManager.hpp"
#include "monolib/util/FixStr.hpp"
#include "monolib/work/CProcess.hpp"
#include "monolib/work/CWorkThreadSystem.hpp"

#include <nw4r/lyt.h>
#include <stdio.h>
#include "revolution/gx/GXPixel.h"

// CfGameManagerUnityHelpers.hpp (via the include chain) already declares a
// no-arg extern "C" CItem_initItemImplInstances(); MWCC rejects this TU's
// item-pointer form at global scope as an illegal overload, so keep it in a
// namespace and shim the call sites through the macro (same pattern as
// CItemBoxInfo.hpp).
namespace getMenuMultiItemImplShim {
extern "C" CMenuGetItemImpl* CItem_initItemImplInstances(CMenuGetItemMultiEntry*);
}
using getMenuMultiItemImplShim::CItem_initItemImplInstances;
#define CItem_initItemImplInstances(entry) \
    getMenuMultiItemImplShim::CItem_initItemImplInstances(entry)

// Wide-return views of the BDAT lookups used by func_801B78B4: retail
// stores each result in a raw u32 register (plain mr, no zero-extend) and
// applies explicit (u8) masks only at the use sites - the u8-returning
// global decls would force an rlwinm at every assignment.
namespace getItemMultiE8WideShim {
extern "C" u32 func_801361E8(u32, const char*, u32);
}
using getItemMultiE8WideShim::func_801361E8;
#define func_801361E8(a, b, c) getItemMultiE8WideShim::func_801361E8(a, b, c)

namespace getItemMulti600CWideShim {
extern "C" u32 func_8013600C(const void*, const void*, u32);
}
using getItemMulti600CWideShim::func_8013600C;
#define func_8013600C(a, b, c) getItemMulti600CWideShim::func_8013600C(a, b, c)

struct CMenuGetItemPaneWord {
    CMenuGetItemPaneWord() {}
    CMenuGetItemPaneWord(const CMenuGetItemPaneWord& other) : value(other.value) {}
    u32 value;
};

class CMenuGetItemFontObject {
public:
    virtual void v08() = 0;
    virtual void v0C() = 0;
    virtual void v10() = 0;
    virtual void v14() = 0;
    virtual void v18() = 0;
    virtual void v1C() = 0;
    virtual void v20() = 0;
    virtual u32 getPane() = 0;
};

class CMenuGetItemPaneExtra {
public:
    virtual void v08() = 0;
    virtual void v0C() = 0;
    virtual void v10() = 0;
    virtual void v14() = 0;
    virtual void v18() = 0;
    virtual void v1C() = 0;
    virtual void v20() = 0;
    virtual CMenuGetItemPaneWord getPaneWord(u32) = 0;
};

// Wide (u16-returning) view of the item impl's +0x30 slot: func_801B7A58
// gates its three rank slots on this count kept as a full halfword (retail
// clrlwi 16), while the byte-returning view used by Init masks to 8 bits.
class CMenuGetItemImplCount {
public:
    virtual u16 getCount(CMenuGetItemMultiEntry*) = 0;      // +0x08
    virtual void v0C() = 0;
    virtual void v10() = 0;
    virtual void v14() = 0;
    virtual void v18() = 0;
    virtual void v1C() = 0;
    virtual char* getName(CMenuGetItemMultiEntry*) = 0;     // +0x20
    virtual void v24() = 0;
    virtual void v28() = 0;
    virtual void* getSlot(CMenuGetItemMultiEntry*, u8) = 0; // +0x2C
    virtual u16 getCount30(CMenuGetItemMultiEntry*) = 0;    // +0x30
};

class CMenuGetItemImpl;

struct CMenuGetItemTextureHeader {
    u16 height;
    u16 width;
};

struct CMenuGetItemTextureData {
    CMenuGetItemTextureHeader* header;
};

struct CMenuGetItemTextureResource {
    u8 _00[8];
    CMenuGetItemTextureData* data;
};

extern "C" {
extern char lbl_eu_80504A3C[];
extern f32 lbl_eu_80667E00;
extern u32 lbl_eu_806640EC;
extern u32 lbl_eu_80664108;
extern u32 lbl_eu_80664410;

// Ctor-only imports (see __ct__CMenuGetItemMulti below).
extern char lbl_eu_8052D238[]; // interim composite vtable
extern char lbl_eu_805338BC[]; // final composite vtable base
extern u32 __ptmf_null[3];     // null pointer-to-member-function constant
void __ct__8CProcessFv(CProcess*);

void code80135FDC_postIncByte_64080();
nw4r::lyt::ArcResourceAccessor* func_801355F4();
u32 func_801355BC();
char* func_80138F78(u32);
CMenuGetItemFourShorts func_801397AC(void*, u32);
u32 func_80139358(u32);
char* func_801393CC(u32);
char* func_801394D4(u16);
u8 func_80157CD0(u16);
// Retail call sites consume the +6 slot-sum as a raw byte (mr r5, r3, no
// clrlwi), so the call-site type is u8 despite the CItem.cpp def using u32;
// the arg is passed raw (mr r3, rN) so the visible param is u32.
u8 func_80158068(u32);
// func_80136910 is declared by the included code_80135FDC.hpp / unit header.
// Retail passes the raw id to func_801392E4 without a halfword mask (mr r3,
// r4 directly), so the visible param is u32 here.
u32 func_801392E4(u32);
char* func_eu_802B1474();
char* func_eu_802B148C();
u16 func_80136254(const void*, const char*, u16);
void func_80137B44(nw4r::lyt::Layout*, char*, u32);
void* func_80452C10__11CDeviceFontFUlPQ34nw4r3lyt6Layout(u32, nw4r::lyt::Layout*);
CBaseCur* __ct__CCur18(CBaseCur*, nw4r::lyt::ArcResourceAccessor*);
void* __dt__6CCur18Fv(CBaseCur*, int);
CSysWin* __ct__CSysWin(CSysWin*, int);
CSysWin* __dt__7CSysWinFv(CSysWin*, int);
void func_801B59F4(CMenuGetItemMulti*);
void func_801B5860(CMenuGetItemMulti*, int, CMenuGetItemMultiEntry*);
void func_801B6184(CMenuGetItemMulti*, int, CMenuGetItemMultiEntry*);
void func_801B69F4(CMenuGetItemMulti*, int, CMenuGetItemMultiEntry*);
void func_801B70BC(CMenuGetItemMulti*, int, CMenuGetItemMultiEntry*);
void func_801B7440(CMenuGetItemMulti*, CMenuGetItemMultiEntry*);
void func_801B76CC(CMenuGetItemMulti*, int);
void func_801B78B4(CMenuGetItemMulti*, int);
void func_801B4830(CMenuGetItemMulti*);
void func_801B7A58(CMenuGetItemMulti*, int, CMenuGetItemMultiEntry*);
void func_801B82E8(CMenuGetItemMulti*);
void func_801B8E2C(CMenuGetItemMulti*);
void func_801B9864(CMenuGetItemMulti*);
void func_801B9C1C(CMenuGetItemMulti*);
void __dt__8CProcessFv(CProcess*, int);
u8* __ct__801B2794(u8*, u32, u32);
}

extern "C" int func_80086F9C__Q22cf13CfGameManagerFv(int);
extern "C" void func_8008294C__Q22cf13CfGameManagerFv(int);

// Pane layout adjustment for the visible-item count (defined below; called by Init).
void func_801B5630(CMenuGetItemMulti* self);


extern "C" void __dt__17CMenuGetItemMultiFv(void*, int);
extern "C" void cbRenderBefore__17CMenuGetItemMultiFv(void*);

// Constructor (retail unmangled global `__ct__CMenuGetItemMulti`, written as a
// C-ABI free function like CTaskGameEvt/CMenuMakeCrystal so no class-length
// mangling is emitted). Builds the CProcess base, installs the interim
// composite vtable, copies __ptmf_null into both callback slots (+0x3C/+0x48),
// clears the scalar block, swaps in the final composite vtable and the
// IWorkEvent/IScnRender sub-vtables (+0x24/+0xac from the same base table),
// then constructs the UnkClass_8045F564 render region (+0x78), zero-inits the
// entry array and cursor/system-window sub-objects, and arms the state bytes.
CMenuGetItemMulti* __ct__CMenuGetItemMulti(
    CMenuGetItemMulti* pThis, CScn* scn, u32 item0, u32 item1, u32 item2,
    u32 item3, u8 argF6, u8 arg201, u8 argSpecial) {
    // Zero constant declared up front: its live range then starts at entry
    // (like MWCC's implicit member-init sequence) and wins the lowest
    // callee-saved register, matching retail r22.
    u32 zero = 0;

    __ct__8CProcessFv(reinterpret_cast<CProcess*>(pThis));

    // Interim composite vtable label first (retail materializes its @ha
    // before the final base's @ha, folding the @l into the store), then the
    // final composite vtable base + its two interface sub-vtable offsets
    // (retail holds base in r5, +0x24 in r4, +0xac in r0).
    char* interimVt = lbl_eu_8052D238;
    char* vtBase = lbl_eu_805338BC;
    u32 workEvtVt = (u32)(vtBase + 0x24);
    u32 scnVt = (u32)(vtBase + 0xac);

    pThis->field_10 = reinterpret_cast<u32>(interimVt);

    // NULL PTMF -> callback slots 0x3C / 0x48 in the retail store order
    // 0x40,0x3C,0x44 then 0x4C,0x48,0x50. Post-increment derefs fold the
    // first access of each slot into `lwzu`; re-fetching src between slots
    // forces the retail reloads.
    const u32* src = __ptmf_null;
    u32 w0 = *src++;
    u32 w1 = *src++;
    pThis->mPtmfSlot3C[1] = w1; // 0x40
    pThis->mPtmfSlot3C[0] = w0; // 0x3C
    u32 w2 = *src++;
    pThis->mPtmfSlot3C[2] = w2; // 0x44
    src = __ptmf_null;
    w1 = *src++;
    w0 = *src++;
    pThis->mPtmfSlot48[1] = w1; // 0x4C
    pThis->mPtmfSlot48[0] = w0; // 0x48
    w2 = *src++;
    pThis->mPtmfSlot48[2] = w2; // 0x50

    // Zero constants drive most of the clear stores (retail keeps it in a
    // callee-saved reg across the UnkClass_8045F564 ctor call).

    pThis->mLayout = reinterpret_cast<nw4r::lyt::Layout*>(zero); // 0x54
    pThis->field_58 = zero;
    pThis->field_5C = zero;
    pThis->field_60 = -1;
    pThis->field_64 = 0;
    pThis->field_65 = 0;
    pThis->field_66 = 0;
    pThis->field_67 = 1;
    pThis->field_68 = zero;
    pThis->field_10 = reinterpret_cast<u32>(vtBase); // final composite vtable
    pThis->mIWorkEventVtable = workEvtVt; // 0x6C IWorkEvent sub-vtable
    pThis->mIScnRenderVtable = scnVt;     // 0x70 IScnRender sub-vtable
    pThis->mScn = scn;

    __ct__17UnkClass_8045F564Fv(reinterpret_cast<UnkClass_8045F564*>(&pThis->mRegion[0]));

    pThis->mAnim = reinterpret_cast<nw4r::lyt::AnimTransform*>(zero);
    pThis->mInitialItems[0] = item0;
    pThis->mInitialItems[1] = item1;
    pThis->mInitialItems[2] = item2;
    pThis->mInitialItems[3] = item3;
    pThis->mVisibleItemCount = zero;

    // Entry 0 is a standalone member (ctor inlined); entries 1..3 form the
    // array MWCC clears via its guarded divwu/bdnz array-init loop.
    pThis->mFirstEntry.value = (u16)zero;
    pThis->mFirstEntry.packed = zero;
    for (CMenuGetItemMultiEntry* cur = &pThis->mRestEntries[0];
         cur < &pThis->mRestEntries[3]; cur++) {
        cur->value = (u16)zero;
        cur->packed = zero;
    }

    pThis->mEntryCount = zero;

    __ct__CCur18(&pThis->mCursor, NULL);
    __ct__CSysWin(reinterpret_cast<CSysWin*>(&pThis->mSystemWindow[0]), 0);

    // Second zero constant for the tail clear (retail reuses the freed scn
    // register slot as a fresh zero, r24).
    u32 tailZero = 0;
    pThis->mMaxVisibleItems = 4;
    pThis->mHasSpecialItem = argSpecial;
    pThis->field_1F4 = 1;
    pThis->field_1F5 = (u8)tailZero;
    pThis->field_1F6 = argF6;
    pThis->field_1F8 = tailZero;
    pThis->field_1FC = tailZero;
    pThis->field_200 = (u8)tailZero;
    pThis->field_201 = arg201;
    pThis->mFileHandle = 0;
    pThis->field_208 = tailZero;
    pThis->field_20C = (u8)tailZero;
    pThis->field_20D = (u8)tailZero;
    pThis->field_20E = (u8)tailZero;
    pThis->field_20F = argSpecial;
    // Retail fully unrolls this clear (interleaved stb/stw/sth per slot).
    pThis->mPaneVisible[0] = (u8)tailZero;
    pThis->mVisibleEntries[0] = reinterpret_cast<CMenuGetItemMultiEntry*>(tailZero);
    pThis->mVisibleItemIds[0] = (u16)tailZero;
    pThis->mPaneVisible[1] = (u8)tailZero;
    pThis->mVisibleEntries[1] = reinterpret_cast<CMenuGetItemMultiEntry*>(tailZero);
    pThis->mVisibleItemIds[1] = (u16)tailZero;
    pThis->mPaneVisible[2] = (u8)tailZero;
    pThis->mVisibleEntries[2] = reinterpret_cast<CMenuGetItemMultiEntry*>(tailZero);
    pThis->mVisibleItemIds[2] = (u16)tailZero;
    pThis->mPaneVisible[3] = (u8)tailZero;
    pThis->mVisibleEntries[3] = reinterpret_cast<CMenuGetItemMultiEntry*>(tailZero);
    pThis->mVisibleItemIds[3] = (u16)tailZero;
    pThis->mPaneVisible[4] = (u8)tailZero;
    pThis->mVisibleEntries[4] = reinterpret_cast<CMenuGetItemMultiEntry*>(tailZero);
    pThis->mVisibleItemIds[4] = (u16)tailZero;
    pThis->mPaneVisible[5] = (u8)tailZero;
    pThis->mVisibleEntries[5] = reinterpret_cast<CMenuGetItemMultiEntry*>(tailZero);
    pThis->mVisibleItemIds[5] = (u16)tailZero;
    pThis->mEntryCount = tailZero;
    pThis->mEntryCount = 0;
    return pThis;
}

// Retail __dt__801B2754: deleting destructor of the 0x36c get-item-multi
// task object (empty body - the ctor __ct__801B2794 owns all state init).
u8* __dt__801B2754(u8* _this, int flags) {
    if (_this != 0 && flags > 0) {
        operator delete(_this);
    }
    return _this;
}

void __ct__801B2794(){}

CMenuGetItemMulti::~CMenuGetItemMulti() {
    // Sub-objects destroyed in reverse declaration order via their retail
    // dtor symbols (r4=-1); the CProcess base region at +0x00 is destroyed
    // last with flags=0 behind the nested double null-check (MWCC
    // D2-inlined-into-D1 artifact - CMenuGetItem idiom).
    __dt__7CSysWinFv(reinterpret_cast<CSysWin*>(&mSystemWindow[0]), -1);
    __dt__6CCur18Fv(&mCursor, -1);
    reinterpret_cast<UnkClass_8045F564*>(&mRegion[0])->~UnkClass_8045F564();
    if (this != 0) {
        if (this != 0) {
            __dt__8CProcessFv(reinterpret_cast<CProcess*>(this), 0);
        }
    }
}

extern u32 lbl_eu_80664418;
extern "C" void func_801B29E0() { lbl_eu_80664418 = 0; }

void CMenuGetItemMulti::Init() {
    int memHandle = (int)mtl::MemManager::getHandleMEM2();
    reinterpret_cast<UnkClass_8045F564*>(&mRegion[0])->createRegion(
        memHandle, 0x10000, lbl_eu_80504A3C, 0);
    Class_8045F858 regionHost(
        reinterpret_cast<UnkClass_8045F564*>(&mRegion[0]));

    char setupPaneName[32];
    char itemPaneName[32];
    char initialTextPaneName[32];
    char initialItemPaneName[32];
    char slotPaneName[32];
    char fullPaneName[32];
    u32 initialItems[4];

    if (mEntryCount != 0 || field_1F6 != 0) {
        func_8008294C__Q22cf13CfGameManagerFv(1);
        code80135FDC_postIncByte_64080();
    }

    IWorkEvent* workEvent = reinterpret_cast<IWorkEvent*>(this);
    if (this != NULL) {
        workEvent = reinterpret_cast<IWorkEvent*>(&mIWorkEventVtable);
    }
    mFileHandle = CDeviceFile::readFile(memHandle, &lbl_eu_80504A3C[0x12], workEvent, 0, 0);

    func_80136E84__FPPQ34nw4r3lyt6LayoutPQ34nw4r3lyt19ArcResourceAccessorPCc(
        &mLayout, func_801355F4(), &lbl_eu_80504A3C[0x2e]);
    func_80136F08__FPQ34nw4r3lyt6LayoutPPQ34nw4r3lyt13AnimTransformPQ34nw4r3lyt19ArcResourceAccessorPc(
        mLayout, &mAnim, func_801355F4(), &lbl_eu_80504A3C[0x47]);

    nw4r::lyt::Pane* rootPane = mLayout->GetRootPane();
    CMenuGetItemFontObject* fontObject = reinterpret_cast<CMenuGetItemFontObject*>(
        func_80452C10__11CDeviceFontFUlPQ34nw4r3lyt6Layout(1, mLayout));
    func_8013676C(rootPane, fontObject->getPane());

    u32 font = func_801355BC();
    if (font != 0) {
        func_801368C0__FPQ34nw4r3lyt6LayoutPcUl(mLayout, &lbl_eu_80504A3C[0x63], font);
        func_801368C0__FPQ34nw4r3lyt6LayoutPcUl(mLayout, &lbl_eu_80504A3C[0x6d], font);
        func_801368C0__FPQ34nw4r3lyt6LayoutPcUl(mLayout, &lbl_eu_80504A3C[0x77], font);
        func_801368C0__FPQ34nw4r3lyt6LayoutPcUl(mLayout, &lbl_eu_80504A3C[0x81], font);

        font = func_801355BC();
        func_801368C0__FPQ34nw4r3lyt6LayoutPcUl(mLayout, &lbl_eu_80504A3C[0x8b], font);
        func_801368C0__FPQ34nw4r3lyt6LayoutPcUl(mLayout, &lbl_eu_80504A3C[0x97], font);
        func_801368C0__FPQ34nw4r3lyt6LayoutPcUl(mLayout, &lbl_eu_80504A3C[0xa3], font);
        func_801368C0__FPQ34nw4r3lyt6LayoutPcUl(mLayout, &lbl_eu_80504A3C[0xaf], font);
        func_801368C0__FPQ34nw4r3lyt6LayoutPcUl(mLayout, &lbl_eu_80504A3C[0xbb], font);
        func_801368C0__FPQ34nw4r3lyt6LayoutPcUl(mLayout, &lbl_eu_80504A3C[0xc7], font);
        func_801368C0__FPQ34nw4r3lyt6LayoutPcUl(mLayout, &lbl_eu_80504A3C[0xd3], font);
        func_801368C0__FPQ34nw4r3lyt6LayoutPcUl(mLayout, &lbl_eu_80504A3C[0xdf], font);
        func_801368C0__FPQ34nw4r3lyt6LayoutPcUl(mLayout, &lbl_eu_80504A3C[0xeb], font);
        func_801368C0__FPQ34nw4r3lyt6LayoutPcUl(mLayout, &lbl_eu_80504A3C[0x63], font);
        func_801368C0__FPQ34nw4r3lyt6LayoutPcUl(mLayout, &lbl_eu_80504A3C[0x6d], font);
        func_801368C0__FPQ34nw4r3lyt6LayoutPcUl(mLayout, &lbl_eu_80504A3C[0x77], font);
        func_801368C0__FPQ34nw4r3lyt6LayoutPcUl(mLayout, &lbl_eu_80504A3C[0x81], font);
        func_801368C0__FPQ34nw4r3lyt6LayoutPcUl(mLayout, &lbl_eu_80504A3C[0xf7], font);
        func_801368C0__FPQ34nw4r3lyt6LayoutPcUl(mLayout, &lbl_eu_80504A3C[0x103], font);
        func_801368C0__FPQ34nw4r3lyt6LayoutPcUl(mLayout, &lbl_eu_80504A3C[0x10f], font);
        func_801368C0__FPQ34nw4r3lyt6LayoutPcUl(mLayout, &lbl_eu_80504A3C[0x11b], font);
        func_801368C0__FPQ34nw4r3lyt6LayoutPcUl(mLayout, &lbl_eu_80504A3C[0x127], font);

    CMenuGetItemPaneView* pane = reinterpret_cast<CMenuGetItemPaneView*>(
        mLayout->GetRootPane()->FindPaneByName(&lbl_eu_80504A3C[0x63], true));
    pane->flags = (pane->flags & 0xfe) | 1;
    pane = reinterpret_cast<CMenuGetItemPaneView*>(
        mLayout->GetRootPane()->FindPaneByName(&lbl_eu_80504A3C[0x6d], true));
    pane->flags = (pane->flags & 0xfe) | 1;
    pane = reinterpret_cast<CMenuGetItemPaneView*>(
        mLayout->GetRootPane()->FindPaneByName(&lbl_eu_80504A3C[0x77], true));
    pane->flags = (pane->flags & 0xfe) | 1;
    pane = reinterpret_cast<CMenuGetItemPaneView*>(
        mLayout->GetRootPane()->FindPaneByName(&lbl_eu_80504A3C[0x81], true));
    pane->flags = (pane->flags & 0xfe) | 1;

    func_80136B4C(mLayout, &lbl_eu_80504A3C[0x146],
                   func_80136190(&lbl_eu_80504A3C[0x133], &lbl_eu_80504A3C[0x141], 43), 0);

    char* fileName;
    if (func_80086F9C__Q22cf13CfGameManagerFv(-1) != 0) {
        fileName = &lbl_eu_80504A3C[0x14f];
    } else {
        fileName = &lbl_eu_80504A3C[0x158];
    }
    u16 keyTextId = func_8013606C(&lbl_eu_80504A3C[0x133], fileName, 43);
    char* buttonTexture = func_80138F78(keyTextId);
    nw4r::lyt::ArcResourceAccessor* accessor = func_801355F4();
    CMenuGetItemTextureResource* texture = reinterpret_cast<CMenuGetItemTextureResource*>(
        accessor->GetResource(nw4r::lyt::ArcResourceAccessor::RES_TYPE_TEXTURE,
                              buttonTexture, NULL));
    if (texture != NULL) {
        func_80137E7C(mLayout, &lbl_eu_80504A3C[0x161], texture);
        u16 textureWidth = texture->data->header->width;
        u16 textureHeight = texture->data->header->height;
        nw4r::lyt::Pane* buttonPane = mLayout->GetRootPane()->FindPaneByName(
            &lbl_eu_80504A3C[0x161], true);
        if (buttonPane != NULL) {
            buttonPane->SetSize(nw4r::lyt::Size((f32)textureWidth,
                                                (f32)textureHeight));
        }
    }
    }

    mLayout->SetAnimationEnable(mAnim, true);
    mAnim->SetFrame(lbl_eu_80667E00);
    mLayout->Animate(0);

    nw4r::lyt::Pane* closePane = mLayout->GetRootPane()->FindPaneByName(
        &lbl_eu_80504A3C[0x16b], true);
    CMenuGetItemPaneWord closeWord =
        reinterpret_cast<CMenuGetItemPaneExtra*>(closePane)->getPaneWord(0);
    lbl_eu_80664410 = closeWord.value;

    func_80136B4C(mLayout, &lbl_eu_80504A3C[0x187],
                   func_80136190(&lbl_eu_80504A3C[0x177], &lbl_eu_80504A3C[0x182], 6), 0);
    func_80136B4C(mLayout, &lbl_eu_80504A3C[0x16b],
                   func_80136190(&lbl_eu_80504A3C[0x133], &lbl_eu_80504A3C[0x141], 79), 0);

    for (u8 i = 0; i < 4; ++i) {
        u32 paneIndex = (u8)i + 1;
        sprintf(setupPaneName, &lbl_eu_80504A3C[0x192], paneIndex);
        func_80136B4C(mLayout, setupPaneName, &lbl_eu_80504A3C[0x19f], 0);
        sprintf(setupPaneName, &lbl_eu_80504A3C[0x1a0], paneIndex);
        func_80136B4C(mLayout, setupPaneName, &lbl_eu_80504A3C[0x19f], 0);
    }

    func_80136B4C(mLayout, &lbl_eu_80504A3C[0x1b8],
                   func_80136190(&lbl_eu_80504A3C[0x1ac], &lbl_eu_80504A3C[0x182], 16), 0);

    void* itemFP = getFP__FPCc(&lbl_eu_80504A3C[0x1c1]);
    func_80136B4C(mLayout, &lbl_eu_80504A3C[0x1ca],
                   func_8013639C(itemFP, &lbl_eu_80504A3C[0x182], 10), 0);
    func_80136B4C(mLayout, &lbl_eu_80504A3C[0x1d5],
                   func_8013639C(itemFP, &lbl_eu_80504A3C[0x182], 13), 0);
    func_80136B4C(mLayout, &lbl_eu_80504A3C[0x1e0],
                   func_8013639C(itemFP, &lbl_eu_80504A3C[0x182], 17), 0);
    func_80136B4C(mLayout, &lbl_eu_80504A3C[0x1eb],
                   func_8013639C(itemFP, &lbl_eu_80504A3C[0x182], 18), 0);
    func_80136B4C(mLayout, &lbl_eu_80504A3C[0x1f6],
                   func_8013639C(itemFP, &lbl_eu_80504A3C[0x182], 12), 0);
    func_80136B4C(mLayout, &lbl_eu_80504A3C[0x201],
                   func_8013639C(itemFP, &lbl_eu_80504A3C[0x182], 19), 0);
    func_80136B4C(mLayout, &lbl_eu_80504A3C[0x20c],
                   func_8013639C(itemFP, &lbl_eu_80504A3C[0x182], 24), 0);
    func_80136B4C(mLayout, &lbl_eu_80504A3C[0x217],
                   func_8013639C(itemFP, &lbl_eu_80504A3C[0x182], 36), 0);
    func_80136B4C(mLayout, &lbl_eu_80504A3C[0x222],
                   func_8013639C(itemFP, &lbl_eu_80504A3C[0x182], 37), 0);
    func_80136B4C(mLayout, &lbl_eu_80504A3C[0x22d],
                   func_8013639C(itemFP, &lbl_eu_80504A3C[0x182], 24), 0);
    func_80136B4C(mLayout, &lbl_eu_80504A3C[0x238],
                   func_8013639C(itemFP, &lbl_eu_80504A3C[0x182], 47), 0);
    func_80136B4C(mLayout, &lbl_eu_80504A3C[0x24c],
                   func_80136190(&lbl_eu_80504A3C[0x243], &lbl_eu_80504A3C[0x182], 30), 0);

    nw4r::lyt::Pane* rectPane = mLayout->GetRootPane()->FindPaneByName(
        &lbl_eu_80504A3C[0x63], true);
    lbl_eu_806643E0 = func_801397AC(rectPane, 0);
    lbl_eu_806643E8 = func_801397AC(rectPane, 1);
    lbl_eu_806643F0.d = lbl_eu_806643E0.d;
    lbl_eu_806643F8.d = lbl_eu_806643E8.d;
    lbl_eu_80664400.d = lbl_eu_806643E0.d;
    lbl_eu_80664408.d = lbl_eu_806643E8.d;

    rectPane = mLayout->GetRootPane()->FindPaneByName(&lbl_eu_80504A3C[0x1ca], true);
    lbl_eu_806643A0 = func_801397AC(rectPane, 0);
    lbl_eu_806643A8 = func_801397AC(rectPane, 1);
    lbl_eu_806643B0.d = lbl_eu_806643A0.d;
    lbl_eu_806643B8.d = lbl_eu_806643A8.d;

    rectPane = mLayout->GetRootPane()->FindPaneByName(&lbl_eu_80504A3C[0x8b], true);
    lbl_eu_806643C0 = func_801397AC(rectPane, 0);
    lbl_eu_806643C8 = func_801397AC(rectPane, 1);
    lbl_eu_806643D0.d = lbl_eu_806643C0.d;
    lbl_eu_806643D8.d = lbl_eu_806643C8.d;

    if (mEntryCount != 0) {
        CMenuGetItemMultiEntry* entry = &mFirstEntry;
        while (entry != &mFirstEntry + mEntryCount) {
            if (entry != NULL) {
                sprintf(itemPaneName, &lbl_eu_80504A3C[0x192], mVisibleItemCount + 1);
                char* itemName;
                if (((entry->packed >> 16) & 0xf) == 3) {
                    ml::FixStr<32> rankText(false);
                    rankText.mString[0] = 0;
                    rankText.mLength = 0;
                    u16 rankCount = CItem_initItemImplInstances(entry)->getRankCount(entry);
                    itemName = CItem_initItemImplInstances(entry)->getName(entry);
                    char* rankName = func_80136190(&lbl_eu_80504A3C[0x1c1],
                                                   &lbl_eu_80504A3C[0x182],
                                                   30 - (rankCount - 1));
                    rankText.format(&lbl_eu_80504A3C[0x259], itemName, rankName);
                    func_80136B4C(mLayout, itemPaneName, rankText.mString, 0);
                } else {
                    itemName = CItem_initItemImplInstances(entry)->getName(entry);
                    func_80136B4C(mLayout, itemPaneName, itemName, 0);
                }

                sprintf(itemPaneName, &lbl_eu_80504A3C[0x25e], mVisibleItemCount + 1);
                CMenuGetItemPaneView* pane = reinterpret_cast<CMenuGetItemPaneView*>(
                    mLayout->GetRootPane()->FindPaneByName(itemPaneName, true));
                if (pane != NULL) {
                    pane->flags = (pane->flags & 0xfe) | 1;
                }

                char* textureName = &lbl_eu_80504A3C[0x26b];
                switch ((entry->packed >> 16) & 0xf) {
                case 2: textureName = &lbl_eu_80504A3C[0x27e]; break;
                case 3: textureName = &lbl_eu_80504A3C[0x293]; break;
                case 4: textureName = &lbl_eu_80504A3C[0x2a8]; break;
                case 5: textureName = &lbl_eu_80504A3C[0x2bd]; break;
                case 6: textureName = &lbl_eu_80504A3C[0x2d2]; break;
                case 7: textureName = &lbl_eu_80504A3C[0x2e7]; break;
                case 8: textureName = &lbl_eu_80504A3C[0x2fc]; break;
                case 9: textureName = &lbl_eu_80504A3C[0x311]; break;
                case 10: textureName = &lbl_eu_80504A3C[0x326]; break;
                case 11: {
                    u32 fontCheck = lbl_eu_80664108;
                    textureName = &lbl_eu_80504A3C[0x355];
                    if (func_801361E8(fontCheck, &lbl_eu_80504A3C[0x33b],
                                     func_80139358((u16)(entry->packed >> 20)))) {
                        textureName = &lbl_eu_80504A3C[0x340];
                    }
                    break;
                }
                case 12: textureName = &lbl_eu_80504A3C[0x36a]; break;
                case 13: textureName = &lbl_eu_80504A3C[0x37f]; break;
                }
                nw4r::lyt::ArcResourceAccessor* accessor = func_801355F4();
                void* itemTexture = accessor->GetResource(
                    nw4r::lyt::ArcResourceAccessor::RES_TYPE_TEXTURE, textureName, NULL);
                if (itemTexture == NULL) {
                    accessor = func_801355F4();
                    itemTexture = accessor->GetResource(
                        nw4r::lyt::ArcResourceAccessor::RES_TYPE_TEXTURE,
                        &lbl_eu_80504A3C[0x26b], NULL);
                }
                if (itemTexture != NULL) {
                    func_80137E7C(mLayout, itemPaneName, itemTexture);
                }

                sprintf(itemPaneName, &lbl_eu_80504A3C[0x1a0], mVisibleItemCount + 1);
                func_80136B4C(mLayout, itemPaneName, &lbl_eu_80504A3C[0x19f], 0);
                func_80139A18(mLayout, itemPaneName, &lbl_eu_806643E0, &lbl_eu_806643E8);

                if ((((entry->packed >> 16) & 0xf) >= 4 &&
                     ((entry->packed >> 16) & 0xf) <= 8) ||
                    ((entry->packed >> 16) & 0xf) == 2) {
                    u8 slotCount = CItem_initItemImplInstances(entry)->hasSlot(entry);
                    if (slotCount != 0) {
                        func_80136B4C(mLayout, itemPaneName, func_eu_802B148C(), 0);
                        func_80139A18(mLayout, itemPaneName, &lbl_eu_806643F0,
                                     &lbl_eu_806643F8);
                        for (u8 slot = 0; slot < slotCount; ++slot) {
                            if (CItem_initItemImplInstances(entry)->getSlotId(entry, slot) == -1) {
                                u16* slotData = reinterpret_cast<u16*>(
                                    CItem_initItemImplInstances(entry)->getSlot(entry, slot));
                                if (slotData != NULL && (slotData[2] & 1) != 0) {
                                    func_80136B4C(mLayout, itemPaneName, func_eu_802B1474(), 0);
                                    func_80139A18(mLayout, itemPaneName, &lbl_eu_80664400,
                                                 &lbl_eu_80664408);
                                    break;
                                }
                            }
                        }
                    }
                } else if (((entry->packed >> 16) & 0xf) == 3 ||
                           ((entry->packed >> 16) & 0xf) == 9) {
                    u16 rankCount = CItem_initItemImplInstances(entry)->getRankCount(entry);
                    func_80136B4C(mLayout, itemPaneName,
                                   func_80136190(&lbl_eu_80504A3C[0x1c1],
                                                &lbl_eu_80504A3C[0x182],
                                                30 - ((u8)rankCount - 1)), 0);
                    func_80139A18(mLayout, itemPaneName, &lbl_eu_806643E0,
                                 &lbl_eu_806643E8);
                }

                bool special = false;
                if (((entry->packed >> 16) & 0xf) != 3 &&
                    ((entry->packed >> 16) & 0xf) != 9) {
                    special = func_801361E8(lbl_eu_806640EC,
                                            &lbl_eu_80504A3C[0x394],
                                            (u16)(entry->packed >> 20)) != 0;
                }
                if ((((entry->packed >> 16) & 0xf) == 12) || special) {
                    mHasSpecialItem = 1;
                }
                u16 cat2 = (entry->packed >> 16) & 0xf;
                if (cat2 == 0) {
                    cat2 = func_801392E4((u16)(entry->packed >> 20));
                }
                func_80139358((u16)(entry->packed >> 20));
                int special2 = 0;
                if (cat2 >= 2 && cat2 <= 9) {
                    if (func_80157CD0(cat2) != 0) {
                        special2 = 1;
                    }
                } else if (cat2 >= 10 && cat2 <= 13) {
                    int y = func_80158068((u16)(entry->packed >> 20));
                    if (y < 1) {
                        if (func_80157CD0(cat2) != 0) {
                            special2 = 1;
                        }
                    } else if (y < 0x63) {
                        special2 = 1;
                    }
                }
                if (special2 == 0) {
                    mPaneVisible[mVisibleItemCount] = 1;
                }
                mVisibleEntries[mVisibleItemCount] = entry;
                ++mVisibleItemCount;
            }
            ++entry;
        }
    } else {
        initialItems[0] = mInitialItems[0];
        initialItems[1] = mInitialItems[1];
        initialItems[2] = mInitialItems[2];
        initialItems[3] = mInitialItems[3];
        for (u8 sourceIndex = 0; sourceIndex < 4; ++sourceIndex) {
            u16 itemId = (u16)initialItems[sourceIndex];
            char* itemTable = func_801393CC(itemId);
            u8 category = (u8)func_801392E4(itemId);
            u16 tableId = func_80139358(itemId);
            if (itemId != 0) {
                sprintf(initialTextPaneName, &lbl_eu_80504A3C[0x192],
                        mVisibleItemCount + 1);
                func_80136B4C(mLayout, initialTextPaneName, func_801394D4(itemId), 0);
                bool special = false;
                if (category != 3 && category != 9) {
                    special = func_801361E8(lbl_eu_806640EC,
                                            &lbl_eu_80504A3C[0x394],
                                            itemId) != 0;
                }
                if (category == 12 || special) {
                    mHasSpecialItem = 1;
                }
                u16 cat2 = func_801392E4(itemId);
                func_80139358(itemId);
                int special2 = 0;
                if (cat2 >= 2 && cat2 <= 9) {
                    if (func_80157CD0(cat2) != 0) {
                        special2 = 1;
                    }
                } else if (cat2 >= 10 && cat2 <= 13) {
                    int y = func_80158068(itemId);
                    if (y < 1) {
                        if (func_80157CD0(cat2) != 0) {
                            special2 = 1;
                        }
                    } else if (y < 0x63) {
                        special2 = 1;
                    }
                }
                if (special2 == 0) {
                    mPaneVisible[mVisibleItemCount] = 1;
                }
                mVisibleItemIds[mVisibleItemCount] = itemId;
                ++mVisibleItemCount;
            }

            sprintf(initialItemPaneName, &lbl_eu_80504A3C[0x25e], sourceIndex + 1);
            CMenuGetItemPaneView* pane = reinterpret_cast<CMenuGetItemPaneView*>(
                mLayout->GetRootPane()->FindPaneByName(initialItemPaneName, true));
            if (pane != NULL) {
                u8 visible = itemId != 0;
                pane->flags = (pane->flags & 0xfe) | visible;
            }

            char* textureName = &lbl_eu_80504A3C[0x26b];
            switch (category) {
            case 2: textureName = &lbl_eu_80504A3C[0x27e]; break;
            case 3: textureName = &lbl_eu_80504A3C[0x293]; break;
            case 4: textureName = &lbl_eu_80504A3C[0x2a8]; break;
            case 5: textureName = &lbl_eu_80504A3C[0x2bd]; break;
            case 6: textureName = &lbl_eu_80504A3C[0x2d2]; break;
            case 7: textureName = &lbl_eu_80504A3C[0x2e7]; break;
            case 8: textureName = &lbl_eu_80504A3C[0x2fc]; break;
            case 9: textureName = &lbl_eu_80504A3C[0x311]; break;
            case 10: textureName = &lbl_eu_80504A3C[0x326]; break;
            case 11: {
                u32 fontCheck = lbl_eu_80664108;
                textureName = &lbl_eu_80504A3C[0x355];
                if (func_801361E8(fontCheck, &lbl_eu_80504A3C[0x33b], tableId)) {
                    textureName = &lbl_eu_80504A3C[0x340];
                }
                break;
            }
            case 12: textureName = &lbl_eu_80504A3C[0x36a]; break;
            case 13: textureName = &lbl_eu_80504A3C[0x37f]; break;
            }
            nw4r::lyt::ArcResourceAccessor* accessor = func_801355F4();
            void* itemTexture = accessor->GetResource(
                nw4r::lyt::ArcResourceAccessor::RES_TYPE_TEXTURE, textureName, NULL);
            if (itemTexture == NULL) {
                accessor = func_801355F4();
                itemTexture = accessor->GetResource(
                    nw4r::lyt::ArcResourceAccessor::RES_TYPE_TEXTURE,
                    &lbl_eu_80504A3C[0x26b], NULL);
            }
            if (itemTexture != NULL) {
                func_80137E7C(mLayout, initialItemPaneName, itemTexture);
            }

            sprintf(initialItemPaneName, &lbl_eu_80504A3C[0x1a0], sourceIndex + 1);
            func_80136B4C(mLayout, initialItemPaneName, &lbl_eu_80504A3C[0x19f], 0);
            func_80139A18(mLayout, initialItemPaneName, &lbl_eu_806643E0,
                         &lbl_eu_806643E8);

            if ((category >= 4 && category <= 8) || category == 2) {
                u8 slotCount = func_801361E8((u32)itemTable, &lbl_eu_80504A3C[0x39e], tableId);
                if (slotCount != 0) {
                    func_80136B4C(mLayout, initialItemPaneName, func_eu_802B148C(), 0);
                    func_80139A18(mLayout, initialItemPaneName, &lbl_eu_806643F0,
                                 &lbl_eu_806643F8);
                    for (u8 slot = 0; slot < slotCount; ++slot) {
                        sprintf(slotPaneName, &lbl_eu_80504A3C[0x3a7], slot + 1);
                        if (func_80136254(itemTable, slotPaneName, tableId) != 0) {
                            func_80136B4C(mLayout, initialItemPaneName, func_eu_802B1474(), 0);
                            func_80139A18(mLayout, initialItemPaneName, &lbl_eu_80664400,
                                         &lbl_eu_80664408);
                            break;
                        }
                    }
                }
            } else if (category == 3 || category == 9) {
                u8 rankCount = func_801361E8(lbl_eu_806640EC,
                                             &lbl_eu_80504A3C[0x3b3], itemId);
                func_80136B4C(mLayout, initialItemPaneName,
                               func_80136190(&lbl_eu_80504A3C[0x1c1],
                                            &lbl_eu_80504A3C[0x182],
                                            30 - (rankCount - 1)), 0);
                func_80139A18(mLayout, initialItemPaneName, &lbl_eu_806643E0,
                             &lbl_eu_806643E8);
            }
        }
    }

    func_801B5630(this);
    if (mHasSpecialItem != 0) {
        func_80137B44(mLayout, &lbl_eu_80504A3C[0x16b], 0x777777ff);
    }
    for (u8 i = 0; i < 4; ++i) {
        sprintf(fullPaneName, &lbl_eu_80504A3C[0x3bc], i + 1);
        u8 visible = mPaneVisible[i];
        CMenuGetItemPaneView* pane = reinterpret_cast<CMenuGetItemPaneView*>(
            mLayout->GetRootPane()->FindPaneByName(fullPaneName, true));
        pane->flags = (pane->flags & 0xfe) | visible;
    }

    func_801B5860(this, 0, 0);

    CBaseCur cursorTemp;
    __ct__CCur18(&cursorTemp, func_801355F4());
    mCursor.mArcResAcc = cursorTemp.mArcResAcc;
    mCursor.mpLayout = cursorTemp.mpLayout;
    mCursor.mpAnimTrans0 = cursorTemp.mpAnimTrans0;
    mCursor.mpAnimTrans1 = cursorTemp.mpAnimTrans1;
    mCursor.mActive = cursorTemp.mActive;
    mCursor.mVisible = cursorTemp.mVisible;
    __dt__6CCur18Fv(&cursorTemp, -1);
    ((void (*)(CBaseCur*))reinterpret_cast<void**>(mCursor.mVtable)[2])(&mCursor);

    u8 systemWindowStorage[sizeof(CSysWin)];
    CSysWin* systemWindowTemp = reinterpret_cast<CSysWin*>(&systemWindowStorage[0]);
    __ct__CSysWin(systemWindowTemp, 2);
    CSysWin* systemWindow = reinterpret_cast<CSysWin*>(&mSystemWindow[0]);
    systemWindow->mMemRegion.unk0 = systemWindowTemp->mMemRegion.unk0;
    systemWindow->mMemRegion.unk4 = systemWindowTemp->mMemRegion.unk4;
    systemWindow->mMemRegion.unk8 = systemWindowTemp->mMemRegion.unk8;
    systemWindow->mMemRegion.unkC = systemWindowTemp->mMemRegion.unkC;
    systemWindow->mFileHandle = systemWindowTemp->mFileHandle;
    systemWindow->mTagProcessor = systemWindowTemp->mTagProcessor;
    systemWindow->mArcAccessor = systemWindowTemp->mArcAccessor;
    systemWindow->mLayout = systemWindowTemp->mLayout;
    systemWindow->mAnimTrans = systemWindowTemp->mAnimTrans;
    systemWindow->field_28 = systemWindowTemp->field_28;
    systemWindow->field_2C = systemWindowTemp->field_2C;
    systemWindow->field_30 = systemWindowTemp->field_30;
    systemWindow->field_34 = systemWindowTemp->field_34;
    systemWindow->field_35 = systemWindowTemp->field_35;
    systemWindow->field_36 = systemWindowTemp->field_36;
    systemWindow->field_37 = systemWindowTemp->field_37;
    systemWindow->field_38 = systemWindowTemp->field_38;
    systemWindow->field_39 = systemWindowTemp->field_39;
    __dt__7CSysWinFv(systemWindowTemp, -1);
    ((void (*)(CSysWin*))reinterpret_cast<void**>(systemWindow->mVtbl)[34])(systemWindow);

    if ((*reinterpret_cast<u32*>(cf::CfGameManager::getCurrentPad()) & 0x0001e000) != 0) {
        field_200 = 1;
    }

    IScnRender* render = reinterpret_cast<IScnRender*>(this);
    if (this != NULL) {
        render = reinterpret_cast<IScnRender*>(&mIScnRenderVtable);
    }
    mScn->addRenderCB(render, 13, 0);
    reinterpret_cast<UnkClass_8045F564*>(&mRegion[0])->func_8045F810();
}

void CMenuGetItemMulti::Term() {
    CDeviceVI::waitForDrawDone();
    IScnRender* render = reinterpret_cast<IScnRender*>(this);
    if (this != 0) {
        render = reinterpret_cast<IScnRender*>(&mIScnRenderVtable);
    }
    mScn->removeRenderCB(render);
    func_8003AA8C__5CBdatFUl(5);
    func_801390E0(&mFileHandle);
    if (field_208 != 0) {
        mtl::MemManager::deallocate(reinterpret_cast<void*>(field_208));
        field_208 = 0;
    }
    reinterpret_cast<CMenuGetItemMultiCur*>(&mCursor)->func_801D2180();
    func_8022B7F4(&mSystemWindow[0]);
    if (mLayout != 0) {
        delete mLayout;
        mLayout = 0;
    }
    reinterpret_cast<UnkClass_8045F564*>(&mRegion[0])->func_8045F778();
    lbl_eu_80664414 = 0;
    lbl_eu_80664418 = 0;
    if (mEntryCount != 0 || field_1F6 != 0) {
        func_8013B980();
        if (code80135FDC_getByte_64080() == 0) {
            func_8008294C__Q22cf13CfGameManagerFv(0);
        }
        // Nested: when both outer conditions fail MWCC jumps the
        // field_1F6 test straight to the epilogue (retail beq .L_801B5980).
        if (mEntryCount != 0) {
            lbl_eu_80663E24 &= ~0x200000u;
            CfGameManagerTermFields* mgrView = reinterpret_cast<CfGameManagerTermFields*>(
                cf::CfGameManager::getInstance());
            CfGameManagerTermFields* src = reinterpret_cast<CfGameManagerTermFields*>(
                func_800B708C(mgrView->field_80));
            if (src != 0 && (src->field_64 & 0x100) != 0) {
                reinterpret_cast<CfGameManagerTermFields*>(
                    cf::CfGameManager::getInstance())->field_80 = 0;
                func_800B7320(reinterpret_cast<u32>(src));
                src->field_68 |= 0x60;
            }
        }
    }
}

void CMenuGetItemMulti::Move() {
    CTaskGame::getInstance();
    if (CTaskGame::func_800426F0() != 0) goto exit;
    if (lbl_eu_80663E28 & (1u << 21)) goto exit;
    // Branch-over-branch guard: `goto body` with the `exit` label + return
    // placed BEFORE `body` keeps MWCC from folding the bit test to a single
    // `bne` -- it emits retail's `beq body; b exit`.
    goto body;
exit:
    return;
body:
    if (!func_8013BE50()) goto exit;

    // Per-frame state machine: field_1F8 selects the menu sub-state; the
    // dense 0..11 range is emitted as a jump table, and every case falls
    // out to the common per-frame update tail below.
    switch (field_1F8) {
    case 0:  // window ready: show the item-select cursor
        if (CSysWin_isReady(&mSystemWindow[0]) == 0) break;
        if (lbl_eu_80664418 == 0) break;
        field_1F8 = 1;
        func_80138078__FUl(0x2d);
        break;
    case 1:  // opening animation: position the cursor on the active item
        if (func_80137444(mAnim, lbl_eu_80667E10) == 0) break;
        func_801D216C(&mCursor, 1);
        {
            char buf[32];
            s8 n = (s8)mMaxVisibleItems;
            if (n == 5) {
                sprintf(buf, &lbl_eu_80504A3C[0x16b]);
            } else if (n == 4) {
                sprintf(buf, &lbl_eu_80504A3C[0x192], n + 1);
            } else {
                sprintf(buf, &lbl_eu_80504A3C[0x3c9], n + 1);
            }
            nw4r::lyt::Pane* pane = mLayout->GetRootPane()->FindPaneByName(buf, true);
            nw4r::lyt::Pane* refPane = mLayout->GetRootPane()->FindPaneByName(
                &lbl_eu_80504A3C[0x3d6], true);
            nw4r::math::VEC3 pos;
            func_80137924(&pos, pane, refPane, mLayout->GetRootPane());
            reinterpret_cast<CMenuGetItemMultiCur*>(&mCursor)->vfn4(&pos);
        }
        field_1F8 = 2;
        break;
    case 2:  // idle: pad-input handler
        func_801B82E8(this);
        break;
    case 3:  // closing animation: flag the fade-out
        if (func_80137510(mAnim, lbl_eu_80667E10) == 0) break;
        field_64 = 1;
        break;
    case 4:  // closing: teardown handler
        func_801B8E2C(this);
        break;
    case 5:  // window active: cursor follows the slot selection
        if (CSysWin_isActive(&mSystemWindow[0]) == 0) break;
        func_801D216C(&mCursor, 1);
        {
            nw4r::math::VEC3 pos;
            func_8022C1B4(&pos, &mSystemWindow[0], field_1F4);
            reinterpret_cast<CMenuGetItemMultiCur*>(&mCursor)->vfn4(&pos);
        }
        field_1F8 = 6;
        break;
    case 6:  // window selection: pad-input handler
        func_801B9864(this);
        break;
    case 7:  // confirm selection: open the item menu for the active item
        if (CSysWin_isActive(&mSystemWindow[0]) == 0) break;
        if (field_1F5 != 0) {
            u32 cat;
            if (mEntryCount != 0) {
                cat = (mVisibleEntries[(s8)mMaxVisibleItems]->packed >> 16) & 0xF;
            } else {
                cat = func_801392E4(mVisibleItemIds[(s8)mMaxVisibleItems]) & 0xff;
            }
            func_80133E58((u8)cat, 0, field_201);
            field_1F8 = 0xb;
            field_1F5 = 0;
        } else {
            func_801D216C(&mCursor, 1);
            field_1F8 = 2;
        }
        break;
    case 8:  // window active (alt): cursor follows the slot selection
        if (CSysWin_isActive(&mSystemWindow[0]) == 0) break;
        func_801D216C(&mCursor, 1);
        {
            nw4r::math::VEC3 pos;
            func_8022C1B4(&pos, &mSystemWindow[0], field_1F4);
            reinterpret_cast<CMenuGetItemMultiCur*>(&mCursor)->vfn4(&pos);
        }
        field_1F8 = 9;
        break;
    case 9:  // window selection (alt): pad-input handler
        func_801B9C1C(this);
        break;
    case 10: // confirm from slot state: reopen or confirm
        if (CSysWin_isActive(&mSystemWindow[0]) == 0) break;
        if ((s8)field_1F4 == 0) {
            field_1F8 = 3;
            func_80138078__FUl(0x89);
        } else {
            func_801D216C(&mCursor, 1);
            field_1F8 = 2;
        }
        break;
    case 11: // menu closing: reposition the cursor on the active item
        if (func_80167A18() != 0) break;
        func_801D216C(&mCursor, 1);
        func_801B4830(this);
        {
            char buf[32];
            s8 n = (s8)mMaxVisibleItems;
            if (n == 5) {
                sprintf(buf, &lbl_eu_80504A3C[0x16b]);
            } else if (n == 4) {
                sprintf(buf, &lbl_eu_80504A3C[0x192], n + 1);
            } else {
                sprintf(buf, &lbl_eu_80504A3C[0x3c9], n + 1);
            }
            nw4r::lyt::Pane* pane = mLayout->GetRootPane()->FindPaneByName(buf, true);
            nw4r::lyt::Pane* refPane = mLayout->GetRootPane()->FindPaneByName(
                &lbl_eu_80504A3C[0x3d6], true);
            nw4r::math::VEC3 pos;
            func_80137924(&pos, pane, refPane, mLayout->GetRootPane());
            reinterpret_cast<CMenuGetItemMultiCur*>(&mCursor)->vfn4(&pos);
        }
        field_1F8 = 2;
        break;
    }

    // Per-frame update tail (all states fall through to here).
    mLayout->Animate(0);
    func_801D202C(&mCursor);
    func_8022B748(&mSystemWindow[0]);
}

// Adjust the item-multi pane layout for the visible-item count: each hidden
// item (4 - mVisibleItemCount, clamped at 0) shrinks the item-text pane
// height and shifts the close/other panes down by lbl_eu_80667E14 per step.
//
// Residual note: the (f32)(s32) casts pool MWCC's internal 0x43300000_80000000
// magic as a TU-local @12792 label; retail resolves the same bytes to the
// named .sdata2 symbol lbl_eu_80667E18. No source form references the named
// symbol without rescheduling the whole block (union forms verified +16B /
// frame-slot flips), so this needs the standard CMenuGetItemMulti.o
// pool_patterns rule ((>II 43300000,80000000) -> lbl_eu_80667E18).
void func_801B5630(CMenuGetItemMulti* self) {
    s32 count = 4 - (s32)self->mVisibleItemCount;
    if (count < 0) {
        count = 0;
    }

    nw4r::lyt::Pane* pane = self->mLayout->GetRootPane()->FindPaneByName(
        &lbl_eu_80504A3C[0x3ef], true);
    if (pane != 0) {
        nw4r::lyt::Size size = pane->GetSize();
        size.height -= lbl_eu_80667E14 * (f32)count;
        reinterpret_cast<CMenuGetItemPaneView*>(pane)->height = size.height;
    }
    pane = self->mLayout->GetRootPane()->FindPaneByName(
        &lbl_eu_80504A3C[0x187], true);
    if (pane != 0) {
        nw4r::math::VEC3 pos = pane->GetTranslate();
        pos.y = pos.y + lbl_eu_80667E14 * (f32)count;
        pane->SetTranslate(pos);
    }
    pane = self->mLayout->GetRootPane()->FindPaneByName(
        &lbl_eu_80504A3C[0x16b], true);
    if (pane != 0) {
        nw4r::math::VEC3 pos = pane->GetTranslate();
        pos.y = pos.y + lbl_eu_80667E14 * (f32)count;
        pane->SetTranslate(pos);
    }
    pane = self->mLayout->GetRootPane()->FindPaneByName(
        &lbl_eu_80504A3C[0x3f8], true);
    if (pane != 0) {
        nw4r::math::VEC3 pos = pane->GetTranslate();
        pos.y = pos.y + lbl_eu_80667E14 * (f32)count;
        pane->SetTranslate(pos);
    }
}

// File-event completion handler: OnFileEvent routes the CEventFile through
// here once the owning CFileHandle matches (event->mFileHandle == this->mFileHandle).
// Steals the loaded buffer, publishes it to field_208, then re-reads the
// BDAT file pointer and clears the handle. Returns 1 on success.
int func_801B45A0(CMenuGetItemMulti* self, CEventFile* event) {
    if (self->mFileHandle == event->mFileHandle) {
        u8* data = self->mFileHandle->mData;
        self->mFileHandle->mData = 0;
        self->field_208 = reinterpret_cast<u32>(data);
        func_8003AA34();
        if (getFP__FPCc(&lbl_eu_80504A3C[0x3e5]) == 0) {
            func_8003AA78__5CBdatFUlPv(5, reinterpret_cast<u8*>(self->field_208));
        }
        lbl_eu_80664418 = reinterpret_cast<u32>(getFP__FPCc(&lbl_eu_80504A3C[0x3e5]));
        self->mFileHandle = 0;
        return 1;
    }
    return 0;
}

// Render-callback: gate prefix (task busy / global bit 21 / scene active),
// then draw the layout, system window and cursor through a stack DrawInfo.
// The DrawInfo's scope-exit dtor is auto-emitted as the retail direct
// `bl __dt__Q34nw4r3lyt8DrawInfoFv(drawInfo, -1)`; an explicit
// `drawInfo.~DrawInfo()` would make MWCC ALSO emit a virtual-dispatched
// scope-exit dtor, so none is written here.
void CMenuGetItemMulti::cbRenderBefore() {
    CTaskGame::getInstance();
    if (CTaskGame::func_800426F0() != 0) goto exit;
    if (lbl_eu_80663E28 & (1u << 21)) goto exit;
    // Branch-over-branch guard: `goto body` with the `exit` label + return
    // placed BEFORE `body` keeps MWCC from folding the bit test to a single
    // `bne` -- it emits retail's `beq body; b exit`.
    goto body;
exit:
    return;
body:
    if (!func_8013BE50()) goto exit;
    {
        GXSetZMode(GX_FALSE, GX_NEVER, GX_FALSE);
        nw4r::lyt::DrawInfo drawInfo;
        func_80137250(&drawInfo);
        func_80137038(mLayout, &drawInfo, 0, 1);
        func_8022B7C8(&mSystemWindow[0], &drawInfo);
        func_801D20B0(&mCursor, &drawInfo);
    }
}

// Item-multi window factory: creates the single get-item-multi task object
// (retail SDA slot lbl_eu_80664414) under the given parent process and
// returns it, or 0 when one already exists. The 8 ctor arguments are
// forwarded verbatim (last is a byte flag on the stack).
IUIWindow* func_801B46E4(CProcess* pParent, CScn* pScene, u32 a, u32 b, u32 c,
                         u32 d, u32 e, u32 f, u8 g) {
    if (lbl_eu_80664414 != 0) {
        return 0;
    }
    u8* obj = reinterpret_cast<u8*>(
        mtl::MemManager::allocate(0x36c, CWorkThreadSystem::getWorkMem()));
    if (obj != 0) {
        // Ctor returns `this`, re-establishing obj in r3 across the call
        // (keeps the object in a volatile register, matching retail).
        obj = __ct__CMenuGetItemMulti(obj, pScene, a, b, c, d, e, f, g);
    }
    lbl_eu_80664414 = reinterpret_cast<u32>(obj);
    reinterpret_cast<CProcess*>(obj)->Regist(pParent, false);
    return reinterpret_cast<IUIWindow*>(lbl_eu_80664414);
}

// Create the single get-item-multi task object (retail SDA slot
// lbl_eu_80664414) under the given parent process. Returns the instance
// pointer, or 0 when one already exists.
u8* func_801B4790(CProcess* parent, u32 arg2, u32 arg3) {
    if (lbl_eu_80664414 != 0) {
        return 0;
    }
    u8* obj = (u8*)mtl::MemManager::allocate(0x36c, CWorkThreadSystem::getWorkMem());
    if (obj != 0) {
        // ctor returns `this`, re-establishing obj in r3 across the call
        // (keeps the object in a volatile register, matching retail).
        obj = __ct__801B2794(obj, arg2, arg3);
    }
    lbl_eu_80664414 = (u32)obj;
    reinterpret_cast<CProcess*>(obj)->Regist(parent, false);
    return (u8*)lbl_eu_80664414;
}

extern "C" unsigned long func_801B481C() { return lbl_eu_80664414 != 0; }

// Refresh the get-item-multi display after a sweep: re-compact the visible
// item list (packed entries or plain ids), rebuild every item pane (name,
// rank text, texture, slots), restore the pane visibility and the special-item
// colour, then re-centre the cursor on the active item. Called by the other
// pad handlers whenever the selection changes.
void func_801B4830(CMenuGetItemMulti* self) {
    self->mHasSpecialItem = 0;
    for (u8 i = 0; i < 4; ++i) {
        char buf[32];
        sprintf(buf, &lbl_eu_80504A3C[0x192], i + 1);
        func_80136B4C(self->mLayout, buf, &lbl_eu_80504A3C[0x19f], 0);
        sprintf(buf, &lbl_eu_80504A3C[0x1a0], i + 1);
        func_80136B4C(self->mLayout, buf, &lbl_eu_80504A3C[0x19f], 0);
        sprintf(buf, &lbl_eu_80504A3C[0x25e], i + 1);
        nw4r::lyt::Pane* pane = self->mLayout->GetRootPane()->FindPaneByName(buf, true);
        if (pane != 0) {
            reinterpret_cast<CMenuGetItemPaneView*>(pane)->flags &= 0xfe;
        }
        self->mPaneVisible[i] = 0;
    }

    if (self->mEntryCount != 0) {
        // Compact the non-null visible entries into the front of the list.
        CMenuGetItemMultiEntry* tmp[4] = {0, 0, 0, 0};
        u8 n = 0;
        for (u8 i = 0; i < self->mVisibleItemCount; ++i) {
            if (self->mVisibleEntries[i] != 0) {
                tmp[n++] = self->mVisibleEntries[i];
            }
        }
        self->mVisibleEntries[0] = tmp[0];
        self->mVisibleEntries[1] = tmp[1];
        self->mVisibleEntries[2] = tmp[2];
        self->mVisibleEntries[3] = tmp[3];
        self->mVisibleItemCount = n;

        for (u8 i = 0; i < self->mVisibleItemCount; ++i) {
            CMenuGetItemMultiEntry* entry = self->mVisibleEntries[i];
            if (entry == 0) {
                self->mPaneVisible[i] = 0;
                continue;
            }
            char itemPaneName[32];
            sprintf(itemPaneName, &lbl_eu_80504A3C[0x192], i + 1);
            char* itemName;
            if (((entry->packed >> 12) & 0xF) == 3) {
                ml::FixStr<32> rankText(false);
                rankText.mString[0] = 0;
                rankText.mLength = 0;
                u16 rankCount = CItem_initItemImplInstances(entry)->getRankCount(entry);
                itemName = CItem_initItemImplInstances(entry)->getName(entry);
                char* rankName = func_80136190(&lbl_eu_80504A3C[0x1c1],
                                               &lbl_eu_80504A3C[0x182],
                                               30 - (rankCount - 1));
                rankText.format(&lbl_eu_80504A3C[0x259], itemName, rankName);
                func_80136B4C(self->mLayout, itemPaneName, rankText.mString, 0);
            } else {
                itemName = CItem_initItemImplInstances(entry)->getName(entry);
                func_80136B4C(self->mLayout, itemPaneName, itemName, 0);
            }

            u32 id = entry->packed >> 20;
            u16 cat = (entry->packed >> 12) & 0xF;
            if (cat == 0) {
                cat = (u16)func_801392E4(id);
            }
            func_80139358(id);
            int special = 0;
            if (cat >= 2 && cat <= 9) {
                if (func_80157CD0(cat) != 0) {
                    special = 1;
                }
            } else if (cat >= 10 && cat <= 13) {
                int lvl = func_80158068(id);
                if (lvl < 1) {
                    if (func_80157CD0(cat) != 0) {
                        special = 1;
                    }
                } else if (lvl < 0x63) {
                    special = 1;
                }
            }
            if (special == 0) {
                self->mPaneVisible[i] = 1;
            } else {
                self->mPaneVisible[i] = 0;
            }

            u32 raw = entry->packed;
            u32 cat2 = (raw >> 12) & 0xF;
            int special2 = 0;
            if (cat2 != 3 && cat2 != 9) {
                special2 = func_801361E8(lbl_eu_806640EC, &lbl_eu_80504A3C[0x394],
                                         (u16)(raw >> 20)) != 0;
            }
            if (cat2 == 12 || special2 != 0) {
                self->mHasSpecialItem = 1;
            }

            sprintf(itemPaneName, &lbl_eu_80504A3C[0x25e], i + 1);
            nw4r::lyt::Pane* pane = self->mLayout->GetRootPane()->FindPaneByName(itemPaneName, true);
            if (pane != 0) {
                reinterpret_cast<CMenuGetItemPaneView*>(pane)->flags =
                    (reinterpret_cast<CMenuGetItemPaneView*>(pane)->flags & 0xfe) |
                    (entry != 0);
            }

            char* textureName = &lbl_eu_80504A3C[0x26b];
            switch ((entry->packed >> 12) & 0xF) {
            case 2: textureName = &lbl_eu_80504A3C[0x27e]; break;
            case 3: textureName = &lbl_eu_80504A3C[0x293]; break;
            case 4: textureName = &lbl_eu_80504A3C[0x2a8]; break;
            case 5: textureName = &lbl_eu_80504A3C[0x2bd]; break;
            case 6: textureName = &lbl_eu_80504A3C[0x2d2]; break;
            case 7: textureName = &lbl_eu_80504A3C[0x2e7]; break;
            case 8: textureName = &lbl_eu_80504A3C[0x2fc]; break;
            case 9: textureName = &lbl_eu_80504A3C[0x311]; break;
            case 10: textureName = &lbl_eu_80504A3C[0x326]; break;
            case 11: {
                u32 fontCheck = lbl_eu_80664108;
                textureName = &lbl_eu_80504A3C[0x355];
                if (func_801361E8(fontCheck, &lbl_eu_80504A3C[0x33b],
                                 func_80139358((u16)(entry->packed >> 20)))) {
                    textureName = &lbl_eu_80504A3C[0x340];
                }
                break;
            }
            case 12: textureName = &lbl_eu_80504A3C[0x36a]; break;
            case 13: textureName = &lbl_eu_80504A3C[0x37f]; break;
            }
            nw4r::lyt::ArcResourceAccessor* accessor = func_801355F4();
            void* itemTexture = accessor->GetResource(
                nw4r::lyt::ArcResourceAccessor::RES_TYPE_TEXTURE, textureName, NULL);
            if (itemTexture == NULL) {
                accessor = func_801355F4();
                itemTexture = accessor->GetResource(
                    nw4r::lyt::ArcResourceAccessor::RES_TYPE_TEXTURE,
                    &lbl_eu_80504A3C[0x26b], NULL);
            }
            if (itemTexture != NULL) {
                func_80137E7C(self->mLayout, itemPaneName, itemTexture);
            }

            sprintf(itemPaneName, &lbl_eu_80504A3C[0x1a0], i + 1);
            func_80136B4C(self->mLayout, itemPaneName, &lbl_eu_80504A3C[0x19f], 0);
            func_80139A18(self->mLayout, itemPaneName, &lbl_eu_806643E0,
                         &lbl_eu_806643E8);

            u32 cat3 = (entry->packed >> 12) & 0xF;
            if ((cat3 >= 4 && cat3 <= 8) || cat3 == 2) {
                u8 slotCount = CItem_initItemImplInstances(entry)->hasSlot(entry);
                if (slotCount != 0) {
                    func_80136B4C(self->mLayout, itemPaneName, func_eu_802B148C(), 0);
                    func_80139A18(self->mLayout, itemPaneName, &lbl_eu_806643F0,
                                 &lbl_eu_806643F8);
                    for (u8 slot = 0; slot < slotCount; ++slot) {
                        if (CItem_initItemImplInstances(entry)->getSlotId(entry, slot) == -1) {
                            u16* slotData = reinterpret_cast<u16*>(
                                CItem_initItemImplInstances(entry)->getSlot(entry, slot));
                            if (slotData != NULL && (slotData[2] & 1) != 0) {
                                func_80136B4C(self->mLayout, itemPaneName, func_eu_802B1474(), 0);
                                func_80139A18(self->mLayout, itemPaneName, &lbl_eu_80664400,
                                             &lbl_eu_80664408);
                                break;
                            }
                        }
                    }
                }
            } else if (cat3 == 3 || cat3 == 9) {
                u16 rankCount = CItem_initItemImplInstances(entry)->getRankCount(entry);
                func_80136B4C(self->mLayout, itemPaneName,
                               func_80136190(&lbl_eu_80504A3C[0x1c1],
                                            &lbl_eu_80504A3C[0x182],
                                            30 - ((u8)rankCount - 1)), 0);
                func_80139A18(self->mLayout, itemPaneName, &lbl_eu_806643E0,
                             &lbl_eu_806643E8);
            }
        }
    } else {
        // Compact the non-null visible item ids into the front of the list.
        u16 tmp[4] = {0, 0, 0, 0};
        u8 n = 0;
        for (u8 i = 0; i < self->mVisibleItemCount; ++i) {
            if (self->mVisibleItemIds[i] != 0) {
                tmp[n++] = self->mVisibleItemIds[i];
            }
        }
        self->mVisibleItemIds[0] = tmp[0];
        self->mVisibleItemIds[1] = tmp[1];
        self->mVisibleItemIds[2] = tmp[2];
        self->mVisibleItemIds[3] = tmp[3];
        self->mVisibleItemCount = n;

        for (u8 i = 0; i < self->mVisibleItemCount; ++i) {
            u16 itemId = self->mVisibleItemIds[i];
            char* itemTable = func_801393CC(itemId);
            u8 category = (u8)func_801392E4(itemId);
            u16 tableId = func_80139358(itemId);
            if (itemId == 0) {
                continue;
            }
            char initialTextPaneName[32];
            sprintf(initialTextPaneName, &lbl_eu_80504A3C[0x192], i + 1);
            func_80136B4C(self->mLayout, initialTextPaneName, func_801394D4(itemId), 0);

            u16 cat = (u16)func_801392E4(itemId);
            func_80139358(itemId);
            int special = 0;
            if (cat >= 2 && cat <= 9) {
                if (func_80157CD0(cat) != 0) {
                    special = 1;
                }
            } else if (cat >= 10 && cat <= 13) {
                int lvl = func_80158068(itemId);
                if (lvl < 1) {
                    if (func_80157CD0(cat) != 0) {
                        special = 1;
                    }
                } else if (lvl < 0x63) {
                    special = 1;
                }
            }
            self->mPaneVisible[i] = (special == 0) ? 1 : 0;

            int special2 = 0;
            if (category != 3 && category != 9) {
                special2 = func_801361E8(lbl_eu_806640EC, &lbl_eu_80504A3C[0x394],
                                         itemId) != 0;
            }
            if (category == 12 || special2 != 0) {
                self->mHasSpecialItem = 1;
            }

            char initialItemPaneName[32];
            sprintf(initialItemPaneName, &lbl_eu_80504A3C[0x25e], i + 1);
            nw4r::lyt::Pane* pane = self->mLayout->GetRootPane()->FindPaneByName(
                initialItemPaneName, true);
            if (pane != 0) {
                u8 visible = itemId != 0;
                reinterpret_cast<CMenuGetItemPaneView*>(pane)->flags =
                    (reinterpret_cast<CMenuGetItemPaneView*>(pane)->flags & 0xfe) | visible;
            }

            char* textureName = &lbl_eu_80504A3C[0x26b];
            switch (category) {
            case 2: textureName = &lbl_eu_80504A3C[0x27e]; break;
            case 3: textureName = &lbl_eu_80504A3C[0x293]; break;
            case 4: textureName = &lbl_eu_80504A3C[0x2a8]; break;
            case 5: textureName = &lbl_eu_80504A3C[0x2bd]; break;
            case 6: textureName = &lbl_eu_80504A3C[0x2d2]; break;
            case 7: textureName = &lbl_eu_80504A3C[0x2e7]; break;
            case 8: textureName = &lbl_eu_80504A3C[0x2fc]; break;
            case 9: textureName = &lbl_eu_80504A3C[0x311]; break;
            case 10: textureName = &lbl_eu_80504A3C[0x326]; break;
            case 11: {
                u32 fontCheck = lbl_eu_80664108;
                textureName = &lbl_eu_80504A3C[0x355];
                if (func_801361E8(fontCheck, &lbl_eu_80504A3C[0x33b], tableId)) {
                    textureName = &lbl_eu_80504A3C[0x340];
                }
                break;
            }
            case 12: textureName = &lbl_eu_80504A3C[0x36a]; break;
            case 13: textureName = &lbl_eu_80504A3C[0x37f]; break;
            }
            nw4r::lyt::ArcResourceAccessor* accessor = func_801355F4();
            void* itemTexture = accessor->GetResource(
                nw4r::lyt::ArcResourceAccessor::RES_TYPE_TEXTURE, textureName, NULL);
            if (itemTexture == NULL) {
                accessor = func_801355F4();
                itemTexture = accessor->GetResource(
                    nw4r::lyt::ArcResourceAccessor::RES_TYPE_TEXTURE,
                    &lbl_eu_80504A3C[0x26b], NULL);
            }
            if (itemTexture != NULL) {
                func_80137E7C(self->mLayout, initialItemPaneName, itemTexture);
            }

            sprintf(initialItemPaneName, &lbl_eu_80504A3C[0x1a0], i + 1);
            func_80136B4C(self->mLayout, initialItemPaneName, &lbl_eu_80504A3C[0x19f], 0);
            func_80139A18(self->mLayout, initialItemPaneName, &lbl_eu_806643E0,
                         &lbl_eu_806643E8);

            if ((category >= 4 && category <= 8) || category == 2) {
                u8 slotCount = func_801361E8((u32)itemTable, &lbl_eu_80504A3C[0x39e],
                                             tableId);
                if (slotCount != 0) {
                    func_80136B4C(self->mLayout, initialItemPaneName, func_eu_802B148C(), 0);
                    func_80139A18(self->mLayout, initialItemPaneName, &lbl_eu_806643F0,
                                 &lbl_eu_806643F8);
                    for (u8 slot = 0; slot < slotCount; ++slot) {
                        char slotPaneName[32];
                        sprintf(slotPaneName, &lbl_eu_80504A3C[0x3a7], slot + 1);
                        if (func_80136254(itemTable, slotPaneName, tableId) != 0) {
                            func_80136B4C(self->mLayout, initialItemPaneName, func_eu_802B1474(), 0);
                            func_80139A18(self->mLayout, initialItemPaneName, &lbl_eu_80664400,
                                         &lbl_eu_80664408);
                            break;
                        }
                    }
                }
            } else if (category == 3 || category == 9) {
                u8 rankCount = func_801361E8(lbl_eu_806640EC, &lbl_eu_80504A3C[0x3b3],
                                             itemId);
                func_80136B4C(self->mLayout, initialItemPaneName,
                               func_80136190(&lbl_eu_80504A3C[0x1c1],
                                            &lbl_eu_80504A3C[0x182],
                                            30 - (rankCount - 1)), 0);
                func_80139A18(self->mLayout, initialItemPaneName, &lbl_eu_806643E0,
                             &lbl_eu_806643E8);
            }
        }
    }

    // Full-pane visibility + special-item colour, then re-centre the cursor.
    for (u8 i = 0; i < 4; ++i) {
        char buf[32];
        sprintf(buf, &lbl_eu_80504A3C[0x3bc], i + 1);
        u8 visible = self->mPaneVisible[i];
        nw4r::lyt::Pane* pane = self->mLayout->GetRootPane()->FindPaneByName(buf, true);
        reinterpret_cast<CMenuGetItemPaneView*>(pane)->flags =
            (reinterpret_cast<CMenuGetItemPaneView*>(pane)->flags & 0xfe) | visible;
    }
    if (self->mHasSpecialItem != 0) {
        func_80137B44(self->mLayout, &lbl_eu_80504A3C[0x16b], 0x777777ff);
    } else {
        func_80137B44(self->mLayout, &lbl_eu_80504A3C[0x16b], lbl_eu_80664410);
    }
    self->mMaxVisibleItems = 0;
    {
        char buf[32];
        s8 m = (s8)self->mMaxVisibleItems;
        if (m == 5) {
            sprintf(buf, &lbl_eu_80504A3C[0x16b]);
        } else if (m == 4) {
            sprintf(buf, &lbl_eu_80504A3C[0x192], m + 1);
        } else {
            sprintf(buf, &lbl_eu_80504A3C[0x3c9], m + 1);
        }
        nw4r::lyt::Pane* pane = self->mLayout->GetRootPane()->FindPaneByName(buf, true);
        nw4r::lyt::Pane* refPane = self->mLayout->GetRootPane()->FindPaneByName(
            &lbl_eu_80504A3C[0x3d6], true);
        nw4r::math::VEC3 pos;
        func_80137924(&pos, pane, refPane, self->mLayout->GetRootPane());
        reinterpret_cast<CMenuGetItemMultiCur*>(&self->mCursor)->vfn4(&pos);
    }

    if (self->mEntryCount != 0) {
        CMenuGetItemMultiEntry* entry = self->mVisibleEntries[(s8)self->mMaxVisibleItems];
        if (entry != 0) {
            func_801B5860(self, entry->packed >> 20, entry);
        } else {
            func_801B5860(self, 0, 0);
        }
    } else {
        u16 id = self->mVisibleItemIds[(s8)self->mMaxVisibleItems];
        if (id != 0) {
            func_801B5860(self, id, 0);
        } else {
            func_801B5860(self, 0, 0);
        }
    }
}

// noinline: the retail keeps real bl relocs to these dispatch helpers; empty
// stub bodies must not be inlined away at call sites.
// Reset the rank-item slot display: clear all slot ids/positions, hide the
// rank panes, then blank the per-rank text panes. Called before any
// rank-item category refresh.
__declspec(noinline) void func_801B59F4(CMenuGetItemMulti* self) {
    // Single 0.0f load is reused for all slot position clears (retail lfs f0
    // once from lbl_eu_80667E00).
    f32 zero = lbl_eu_80667E00;
    // Clear the 12 rank slots in two unrolled halves (ids + positions).
    for (int i = 0; i < 6; i++) {
        self->mRankSlotIds[i] = 0;
        self->mRankSlotPos[i].x = zero;
        self->mRankSlotPos[i].y = zero;
        self->mRankSlotPos[i].z = zero;
    }
    for (int i = 6; i < 12; i++) {
        self->mRankSlotIds[i] = 0;
        self->mRankSlotPos[i].x = zero;
        self->mRankSlotPos[i].y = zero;
        self->mRankSlotPos[i].z = zero;
    }

    // Hide every rank/slot pane; the two name panes are also blanked.
    nw4r::lyt::Pane* pane;
    pane = self->mLayout->GetRootPane()->FindPaneByName(&lbl_eu_80504A3C[0x418], true);
    reinterpret_cast<CMenuGetItemPaneView*>(pane)->flags &= 0xfe;
    pane = self->mLayout->GetRootPane()->FindPaneByName(&lbl_eu_80504A3C[0x424], true);
    reinterpret_cast<CMenuGetItemPaneView*>(pane)->flags &= 0xfe;
    pane = self->mLayout->GetRootPane()->FindPaneByName(&lbl_eu_80504A3C[0x430], true);
    reinterpret_cast<CMenuGetItemPaneView*>(pane)->flags &= 0xfe;
    pane = self->mLayout->GetRootPane()->FindPaneByName(&lbl_eu_80504A3C[0x43c], true);
    reinterpret_cast<CMenuGetItemPaneView*>(pane)->flags &= 0xfe;
    pane = self->mLayout->GetRootPane()->FindPaneByName(&lbl_eu_80504A3C[0xeb], true);
    reinterpret_cast<CMenuGetItemPaneView*>(pane)->flags &= 0xfe;
    func_80136B4C(self->mLayout, &lbl_eu_80504A3C[0xeb], &lbl_eu_80504A3C[0x19f], 0);
    pane = self->mLayout->GetRootPane()->FindPaneByName(&lbl_eu_80504A3C[0x1ca], true);
    reinterpret_cast<CMenuGetItemPaneView*>(pane)->flags &= 0xfe;
    pane = self->mLayout->GetRootPane()->FindPaneByName(&lbl_eu_80504A3C[0x1d5], true);
    reinterpret_cast<CMenuGetItemPaneView*>(pane)->flags &= 0xfe;
    pane = self->mLayout->GetRootPane()->FindPaneByName(&lbl_eu_80504A3C[0x1e0], true);
    reinterpret_cast<CMenuGetItemPaneView*>(pane)->flags &= 0xfe;
    pane = self->mLayout->GetRootPane()->FindPaneByName(&lbl_eu_80504A3C[0x1eb], true);
    reinterpret_cast<CMenuGetItemPaneView*>(pane)->flags &= 0xfe;
    pane = self->mLayout->GetRootPane()->FindPaneByName(&lbl_eu_80504A3C[0x1f6], true);
    reinterpret_cast<CMenuGetItemPaneView*>(pane)->flags &= 0xfe;
    pane = self->mLayout->GetRootPane()->FindPaneByName(&lbl_eu_80504A3C[0x201], true);
    reinterpret_cast<CMenuGetItemPaneView*>(pane)->flags &= 0xfe;
    pane = self->mLayout->GetRootPane()->FindPaneByName(&lbl_eu_80504A3C[0x238], true);
    reinterpret_cast<CMenuGetItemPaneView*>(pane)->flags &= 0xfe;
    pane = self->mLayout->GetRootPane()->FindPaneByName(&lbl_eu_80504A3C[0x448], true);
    reinterpret_cast<CMenuGetItemPaneView*>(pane)->flags &= 0xfe;
    pane = self->mLayout->GetRootPane()->FindPaneByName(&lbl_eu_80504A3C[0x453], true);
    reinterpret_cast<CMenuGetItemPaneView*>(pane)->flags &= 0xfe;
    pane = self->mLayout->GetRootPane()->FindPaneByName(&lbl_eu_80504A3C[0x8b], true);
    reinterpret_cast<CMenuGetItemPaneView*>(pane)->flags &= 0xfe;
    pane = self->mLayout->GetRootPane()->FindPaneByName(&lbl_eu_80504A3C[0xaf], true);
    reinterpret_cast<CMenuGetItemPaneView*>(pane)->flags &= 0xfe;
    pane = self->mLayout->GetRootPane()->FindPaneByName(&lbl_eu_80504A3C[0xbb], true);
    reinterpret_cast<CMenuGetItemPaneView*>(pane)->flags &= 0xfe;
    pane = self->mLayout->GetRootPane()->FindPaneByName(&lbl_eu_80504A3C[0x97], true);
    reinterpret_cast<CMenuGetItemPaneView*>(pane)->flags &= 0xfe;
    pane = self->mLayout->GetRootPane()->FindPaneByName(&lbl_eu_80504A3C[0xa3], true);
    reinterpret_cast<CMenuGetItemPaneView*>(pane)->flags &= 0xfe;
    pane = self->mLayout->GetRootPane()->FindPaneByName(&lbl_eu_80504A3C[0xc7], true);
    reinterpret_cast<CMenuGetItemPaneView*>(pane)->flags &= 0xfe;
    pane = self->mLayout->GetRootPane()->FindPaneByName(&lbl_eu_80504A3C[0x45e], true);
    reinterpret_cast<CMenuGetItemPaneView*>(pane)->flags &= 0xfe;
    pane = self->mLayout->GetRootPane()->FindPaneByName(&lbl_eu_80504A3C[0x46b], true);
    reinterpret_cast<CMenuGetItemPaneView*>(pane)->flags &= 0xfe;
    pane = self->mLayout->GetRootPane()->FindPaneByName(&lbl_eu_80504A3C[0x478], true);
    reinterpret_cast<CMenuGetItemPaneView*>(pane)->flags &= 0xfe;
    pane = self->mLayout->GetRootPane()->FindPaneByName(&lbl_eu_80504A3C[0x485], true);
    reinterpret_cast<CMenuGetItemPaneView*>(pane)->flags &= 0xfe;
    pane = self->mLayout->GetRootPane()->FindPaneByName(&lbl_eu_80504A3C[0x48f], true);
    reinterpret_cast<CMenuGetItemPaneView*>(pane)->flags &= 0xfe;
    pane = self->mLayout->GetRootPane()->FindPaneByName(&lbl_eu_80504A3C[0x499], true);
    reinterpret_cast<CMenuGetItemPaneView*>(pane)->flags &= 0xfe;
    pane = self->mLayout->GetRootPane()->FindPaneByName(&lbl_eu_80504A3C[0x20c], true);
    reinterpret_cast<CMenuGetItemPaneView*>(pane)->flags &= 0xfe;
    pane = self->mLayout->GetRootPane()->FindPaneByName(&lbl_eu_80504A3C[0x4a3], true);
    reinterpret_cast<CMenuGetItemPaneView*>(pane)->flags &= 0xfe;
    pane = self->mLayout->GetRootPane()->FindPaneByName(&lbl_eu_80504A3C[0xd3], true);
    reinterpret_cast<CMenuGetItemPaneView*>(pane)->flags &= 0xfe;
    pane = self->mLayout->GetRootPane()->FindPaneByName(&lbl_eu_80504A3C[0xdf], true);
    reinterpret_cast<CMenuGetItemPaneView*>(pane)->flags &= 0xfe;
    pane = self->mLayout->GetRootPane()->FindPaneByName(&lbl_eu_80504A3C[0x217], true);
    reinterpret_cast<CMenuGetItemPaneView*>(pane)->flags &= 0xfe;
    pane = self->mLayout->GetRootPane()->FindPaneByName(&lbl_eu_80504A3C[0x222], true);
    reinterpret_cast<CMenuGetItemPaneView*>(pane)->flags &= 0xfe;
    pane = self->mLayout->GetRootPane()->FindPaneByName(&lbl_eu_80504A3C[0x4ae], true);
    reinterpret_cast<CMenuGetItemPaneView*>(pane)->flags &= 0xfe;
    pane = self->mLayout->GetRootPane()->FindPaneByName(&lbl_eu_80504A3C[0x4ba], true);
    reinterpret_cast<CMenuGetItemPaneView*>(pane)->flags &= 0xfe;
    pane = self->mLayout->GetRootPane()->FindPaneByName(&lbl_eu_80504A3C[0xf7], true);
    reinterpret_cast<CMenuGetItemPaneView*>(pane)->flags &= 0xfe;
    func_80136B4C(self->mLayout, &lbl_eu_80504A3C[0xf7], &lbl_eu_80504A3C[0x19f], 0);

    // Blank the four per-rank text/slot panes.
    for (u8 i = 0; i < 4; ++i) {
        char buf[32];
        sprintf(buf, &lbl_eu_80504A3C[0x4c6], i * 2 + 0x1f);
        func_80136B4C(self->mLayout, buf, &lbl_eu_80504A3C[0x19f], 0);
        sprintf(buf, &lbl_eu_80504A3C[0x4d3], i + 0x1f);
        func_80136B4C(self->mLayout, buf, &lbl_eu_80504A3C[0x19f], 0);
    }
}

// Refresh the item-multi display for one item. arg2 = item id (or 0 to
// clear), arg3 = optional packed entry (when non-null its packed word
// supplies the id and category instead of arg2). Category dispatch:
//   2 -> equipped items, 3 -> rank item, 4-8 -> slotted items,
//   9 -> key item, 10 -> art/talent, 13 -> quest item.
void func_801B5860(CMenuGetItemMulti* self, int arg2, CMenuGetItemMultiEntry* arg3) {
    func_801B59F4(self);
    nw4r::lyt::Pane* pane = self->mLayout->GetRootPane()->FindPaneByName(
        &lbl_eu_80504A3C[0x404], true);
    if (pane != 0) {
        // Retail shape: neg+or builds the non-zero test, the flags byte is
        // loaded in parallel (r5), and bit 0 is merged with a single rlwimi.
        CMenuGetItemPaneView* view = reinterpret_cast<CMenuGetItemPaneView*>(pane);
        view->flags = (arg2 != 0) | (view->flags & 0xfe);
    }
    if (arg2 != 0) {
        // Inline ternary: branches compute into the arg register (r3), the
        // u16 assignment masks once at the join (retail clrlwi r3, r3, 16).
        u16 id = (arg3 != 0) ? (arg3->packed >> 20) : (u32)arg2;
        func_80136910(self->mLayout, &lbl_eu_80504A3C[0xeb], func_80158068(id));
        // Arms kept uncast: with the u8-typed helper return the ternary is
        // signed-typed, which keeps MWCC's clrlwi+cmpwi dispatch chain.
        u8 cat = (arg3 != 0) ? ((arg3->packed >> 16) & 0xf)
                             : func_801392E4(arg2);
        // Goto-chain dispatch: MWCC emits the comparison chain first, then the
        // bodies in this order (2, 4-8, 3, 9, 10, 13) - the retail layout.
        if (cat >= 4 && cat <= 8) goto case4_8;
        if (cat == 2) goto case2;
        if (cat == 3) goto case3;
        if (cat == 9) goto case9;
        if (cat == 10) goto case10;
        if (cat == 13) goto case13;
        goto done;
    case2:
        func_801B6184(self, arg2, arg3);
        goto done;
    case4_8:
        func_801B69F4(self, arg2, arg3);
        goto done;
    case3:
        func_801B70BC(self, arg2, arg3);
        goto done;
    case9:
        func_801B7440(self, arg3);
        goto done;
    case10:
        func_801B76CC(self, arg2);
        goto done;
    case13:
        func_801B78B4(self, arg2);
    done:;
    }
}

// Category-2 (equipped item) display update: show the equipped-item panes,
// look up the item's name/level strings from the font tables, scale the
// window size by the active character, then fill the stat text panes and
// refresh the slot colours. arg3 is the packed entry (unused here beyond
// forwarding to func_801B7A58).
__declspec(noinline) void func_801B6184(CMenuGetItemMulti* self, int arg2,
                                        CMenuGetItemMultiEntry* arg3) {
    nw4r::lyt::Pane* pane;
    pane = self->mLayout->GetRootPane()->FindPaneByName(&lbl_eu_80504A3C[0x40c], true);
    reinterpret_cast<CMenuGetItemPaneView*>(pane)->flags =
        (reinterpret_cast<CMenuGetItemPaneView*>(pane)->flags & 0xfe) | 1;
    pane = self->mLayout->GetRootPane()->FindPaneByName(&lbl_eu_80504A3C[0x1ca], true);
    reinterpret_cast<CMenuGetItemPaneView*>(pane)->flags =
        (reinterpret_cast<CMenuGetItemPaneView*>(pane)->flags & 0xfe) | 1;
    pane = self->mLayout->GetRootPane()->FindPaneByName(&lbl_eu_80504A3C[0x1d5], true);
    reinterpret_cast<CMenuGetItemPaneView*>(pane)->flags =
        (reinterpret_cast<CMenuGetItemPaneView*>(pane)->flags & 0xfe) | 1;
    pane = self->mLayout->GetRootPane()->FindPaneByName(&lbl_eu_80504A3C[0x1e0], true);
    reinterpret_cast<CMenuGetItemPaneView*>(pane)->flags =
        (reinterpret_cast<CMenuGetItemPaneView*>(pane)->flags & 0xfe) | 1;
    pane = self->mLayout->GetRootPane()->FindPaneByName(&lbl_eu_80504A3C[0x1eb], true);
    reinterpret_cast<CMenuGetItemPaneView*>(pane)->flags =
        (reinterpret_cast<CMenuGetItemPaneView*>(pane)->flags & 0xfe) | 1;
    pane = self->mLayout->GetRootPane()->FindPaneByName(&lbl_eu_80504A3C[0x1f6], true);
    reinterpret_cast<CMenuGetItemPaneView*>(pane)->flags =
        (reinterpret_cast<CMenuGetItemPaneView*>(pane)->flags & 0xfe) | 1;
    pane = self->mLayout->GetRootPane()->FindPaneByName(&lbl_eu_80504A3C[0x201], true);
    reinterpret_cast<CMenuGetItemPaneView*>(pane)->flags =
        (reinterpret_cast<CMenuGetItemPaneView*>(pane)->flags & 0xfe) | 1;
    pane = self->mLayout->GetRootPane()->FindPaneByName(&lbl_eu_80504A3C[0x238], true);
    reinterpret_cast<CMenuGetItemPaneView*>(pane)->flags =
        (reinterpret_cast<CMenuGetItemPaneView*>(pane)->flags & 0xfe) | 1;
    pane = self->mLayout->GetRootPane()->FindPaneByName(&lbl_eu_80504A3C[0x448], true);
    reinterpret_cast<CMenuGetItemPaneView*>(pane)->flags =
        (reinterpret_cast<CMenuGetItemPaneView*>(pane)->flags & 0xfe) | 1;
    pane = self->mLayout->GetRootPane()->FindPaneByName(&lbl_eu_80504A3C[0x453], true);
    reinterpret_cast<CMenuGetItemPaneView*>(pane)->flags =
        (reinterpret_cast<CMenuGetItemPaneView*>(pane)->flags & 0xfe) | 1;
    pane = self->mLayout->GetRootPane()->FindPaneByName(&lbl_eu_80504A3C[0x8b], true);
    reinterpret_cast<CMenuGetItemPaneView*>(pane)->flags =
        (reinterpret_cast<CMenuGetItemPaneView*>(pane)->flags & 0xfe) | 1;
    pane = self->mLayout->GetRootPane()->FindPaneByName(&lbl_eu_80504A3C[0xaf], true);
    reinterpret_cast<CMenuGetItemPaneView*>(pane)->flags =
        (reinterpret_cast<CMenuGetItemPaneView*>(pane)->flags & 0xfe) | 1;
    pane = self->mLayout->GetRootPane()->FindPaneByName(&lbl_eu_80504A3C[0xbb], true);
    reinterpret_cast<CMenuGetItemPaneView*>(pane)->flags =
        (reinterpret_cast<CMenuGetItemPaneView*>(pane)->flags & 0xfe) | 1;
    pane = self->mLayout->GetRootPane()->FindPaneByName(&lbl_eu_80504A3C[0x97], true);
    reinterpret_cast<CMenuGetItemPaneView*>(pane)->flags =
        (reinterpret_cast<CMenuGetItemPaneView*>(pane)->flags & 0xfe) | 1;
    pane = self->mLayout->GetRootPane()->FindPaneByName(&lbl_eu_80504A3C[0xa3], true);
    reinterpret_cast<CMenuGetItemPaneView*>(pane)->flags =
        (reinterpret_cast<CMenuGetItemPaneView*>(pane)->flags & 0xfe) | 1;
    pane = self->mLayout->GetRootPane()->FindPaneByName(&lbl_eu_80504A3C[0x45e], true);
    reinterpret_cast<CMenuGetItemPaneView*>(pane)->flags =
        (reinterpret_cast<CMenuGetItemPaneView*>(pane)->flags & 0xfe) | 1;
    pane = self->mLayout->GetRootPane()->FindPaneByName(&lbl_eu_80504A3C[0x46b], true);
    reinterpret_cast<CMenuGetItemPaneView*>(pane)->flags =
        (reinterpret_cast<CMenuGetItemPaneView*>(pane)->flags & 0xfe) | 1;
    pane = self->mLayout->GetRootPane()->FindPaneByName(&lbl_eu_80504A3C[0x478], true);
    reinterpret_cast<CMenuGetItemPaneView*>(pane)->flags =
        (reinterpret_cast<CMenuGetItemPaneView*>(pane)->flags & 0xfe) | 1;
    pane = self->mLayout->GetRootPane()->FindPaneByName(&lbl_eu_80504A3C[0x485], true);
    reinterpret_cast<CMenuGetItemPaneView*>(pane)->flags =
        (reinterpret_cast<CMenuGetItemPaneView*>(pane)->flags & 0xfe) | 1;
    pane = self->mLayout->GetRootPane()->FindPaneByName(&lbl_eu_80504A3C[0x48f], true);
    reinterpret_cast<CMenuGetItemPaneView*>(pane)->flags =
        (reinterpret_cast<CMenuGetItemPaneView*>(pane)->flags & 0xfe) | 1;
    pane = self->mLayout->GetRootPane()->FindPaneByName(&lbl_eu_80504A3C[0x499], true);
    reinterpret_cast<CMenuGetItemPaneView*>(pane)->flags =
        (reinterpret_cast<CMenuGetItemPaneView*>(pane)->flags & 0xfe) | 1;

    // Item-window font pointer + id -> table id (retail keeps both in saved
    // regs across all the lookups; results stay full-width u32 with masks
    // only at the use sites).
    u32 font = lbl_eu_806640F4;
    func_801392E4(arg2);
    u32 tableId = func_80139358(arg2);

    u32 w1 = func_80136254((const void*)font, &lbl_eu_80504A3C[0x4e1], (u16)tableId);
    u32 w2 = func_80136254((const void*)font, &lbl_eu_80504A3C[0x4e9], (u16)tableId);
    u32 v97 = func_801361E8(font, &lbl_eu_80504A3C[0x4f0], (u16)tableId);
    u32 va3 = func_801361E8(font, &lbl_eu_80504A3C[0x4f8], (u16)tableId);
    u32 vaf = func_801361E8(font, &lbl_eu_80504A3C[0x500], (u16)tableId);
    // Window scale factor: byte lookup converted via the int->double magic
    // constant, normalized by a per-window constant.
    u32 vscale = func_801361E8(font, &lbl_eu_80504A3C[0x508], (u16)tableId);
    f32 total = (f32)((f64)(u8)vscale - lbl_eu_80667E08) / lbl_eu_80667E20;
    u32 vbb = func_801361E8(font, &lbl_eu_80504A3C[0x517], (u16)tableId);

    if ((vbb & 4) != 0) {
        // Scale the name/level window by the active character's id-derived
        // factors, clamped to 999.
        void* mgr = func_8009EC9C(1);
        u32 cid = func_800A082C(mgr);
        w1 = (s32)(lbl_eu_80667E24 * (f64)((u16)w1 * (u16)cid));
        if ((u16)w1 >= 999) {
            w1 = 999;
        }
        cid = func_800A082C(mgr);
        w2 = (s32)(lbl_eu_80667E28 * (f64)((u16)w2 * (u16)cid));
        if ((u16)w2 >= 999) {
            w2 = 999;
        }
    }

    // First non-empty per-level entry contributes its height to the total.
    for (u32 i = 1; i <= 10; ++i) {
        char buf[0x20];
        sprintf(buf, &lbl_eu_80504A3C[0x51c], (u8)i);
        if (func_801361E8(font, buf, (u16)tableId) != 0) {
            total += func_8013B380((u8)i);
            break;
        }
    }

    func_80139C98((u16)w1, (u16)w2, 0, total);

    ml::FixStr<32> str;
    char* midText = func_80136190(&lbl_eu_80504A3C[0x1c1], &lbl_eu_80504A3C[0x182], 11);
    str.format(&lbl_eu_80504A3C[0x527], (u16)w1, midText, (u16)w2);
    func_80136B4C(self->mLayout, &lbl_eu_80504A3C[0x8b], str.mString, 0);
    func_80136910(self->mLayout, &lbl_eu_80504A3C[0x97], (u8)v97);
    func_80136910(self->mLayout, &lbl_eu_80504A3C[0xa3], (u8)va3);

    // Two more formatted lines: when the lookup byte is set they show the
    // 127/128 rank strings, otherwise the raw byte value.
    char* s7f = func_80136190(&lbl_eu_80504A3C[0x1c1], &lbl_eu_80504A3C[0x182], 0x7f);
    char* s80 = func_80136190(&lbl_eu_80504A3C[0x1c1], &lbl_eu_80504A3C[0x182], 0x80);
    if ((u8)vaf != 0) {
        str.format(&lbl_eu_80504A3C[0x52e], s7f, s80);
    } else {
        str.format(&lbl_eu_80504A3C[0x535], (u8)vaf, s80);
    }
    func_80136B4C(self->mLayout, &lbl_eu_80504A3C[0xaf], str.mString, 0);

    char* t7f = func_80136190(&lbl_eu_80504A3C[0x1c1], &lbl_eu_80504A3C[0x182], 0x7f);
    char* t80 = func_80136190(&lbl_eu_80504A3C[0x1c1], &lbl_eu_80504A3C[0x182], 0x80);
    if ((u8)vbb != 0) {
        str.format(&lbl_eu_80504A3C[0x52e], t7f, t80);
    } else {
        str.format(&lbl_eu_80504A3C[0x535], (u8)vbb, t80);
    }
    func_80136B4C(self->mLayout, &lbl_eu_80504A3C[0xbb], str.mString, 0);

    // Clear the description panes.
    func_80136B4C(self->mLayout, &lbl_eu_80504A3C[0xc7], &lbl_eu_80504A3C[0x19f], 0);
    func_80136B4C(self->mLayout, &lbl_eu_80504A3C[0x448], &lbl_eu_80504A3C[0x19f], 0);
    func_80136B4C(self->mLayout, &lbl_eu_80504A3C[0x453], &lbl_eu_80504A3C[0x19f], 0);

    // Slot colour/position refresh: shared slots use the default palette,
    // the two item-specific ones use the alt palette.
    func_80139A18(self->mLayout, &lbl_eu_80504A3C[0x1ca], &lbl_eu_806643A0, &lbl_eu_806643A8);
    func_80139A18(self->mLayout, &lbl_eu_80504A3C[0x1eb], &lbl_eu_806643A0, &lbl_eu_806643A8);
    func_80139A18(self->mLayout, &lbl_eu_80504A3C[0x1f6], &lbl_eu_806643A0, &lbl_eu_806643A8);
    func_80139A18(self->mLayout, &lbl_eu_80504A3C[0x1d5], &lbl_eu_806643A0, &lbl_eu_806643A8);
    func_80139A18(self->mLayout, &lbl_eu_80504A3C[0x1e0], &lbl_eu_806643A0, &lbl_eu_806643A8);
    func_80139A18(self->mLayout, &lbl_eu_80504A3C[0x201], &lbl_eu_806643B0, &lbl_eu_806643B8);
    func_80139A18(self->mLayout, &lbl_eu_80504A3C[0x238], &lbl_eu_806643B0, &lbl_eu_806643B8);

    func_801B7A58(self, arg2, arg3);
}

// Category-4/8 (slotted item) display update: show the slotted-item panes,
// push the per-slot values into the item-text panes, then refresh the
// rank-window slots via func_801B7A58. arg3 is the packed entry; with no
// entry the id comes from arg2.
__declspec(noinline) void func_801B69F4(CMenuGetItemMulti* self, int arg2, CMenuGetItemMultiEntry* arg3) {
    nw4r::lyt::Pane* pane;
    pane = self->mLayout->GetRootPane()->FindPaneByName(&lbl_eu_80504A3C[0x40c], true);
    reinterpret_cast<CMenuGetItemPaneView*>(pane)->flags =
        (reinterpret_cast<CMenuGetItemPaneView*>(pane)->flags & 0xfe) | 1;
    pane = self->mLayout->GetRootPane()->FindPaneByName(&lbl_eu_80504A3C[0x1ca], true);
    reinterpret_cast<CMenuGetItemPaneView*>(pane)->flags =
        (reinterpret_cast<CMenuGetItemPaneView*>(pane)->flags & 0xfe) | 1;
    pane = self->mLayout->GetRootPane()->FindPaneByName(&lbl_eu_80504A3C[0x1d5], true);
    reinterpret_cast<CMenuGetItemPaneView*>(pane)->flags =
        (reinterpret_cast<CMenuGetItemPaneView*>(pane)->flags & 0xfe) | 1;
    pane = self->mLayout->GetRootPane()->FindPaneByName(&lbl_eu_80504A3C[0x1e0], true);
    reinterpret_cast<CMenuGetItemPaneView*>(pane)->flags =
        (reinterpret_cast<CMenuGetItemPaneView*>(pane)->flags & 0xfe) | 1;
    pane = self->mLayout->GetRootPane()->FindPaneByName(&lbl_eu_80504A3C[0x1eb], true);
    reinterpret_cast<CMenuGetItemPaneView*>(pane)->flags =
        (reinterpret_cast<CMenuGetItemPaneView*>(pane)->flags & 0xfe) | 1;
    pane = self->mLayout->GetRootPane()->FindPaneByName(&lbl_eu_80504A3C[0x1f6], true);
    reinterpret_cast<CMenuGetItemPaneView*>(pane)->flags =
        (reinterpret_cast<CMenuGetItemPaneView*>(pane)->flags & 0xfe) | 1;
    pane = self->mLayout->GetRootPane()->FindPaneByName(&lbl_eu_80504A3C[0x201], true);
    reinterpret_cast<CMenuGetItemPaneView*>(pane)->flags =
        (reinterpret_cast<CMenuGetItemPaneView*>(pane)->flags & 0xfe) | 1;
    pane = self->mLayout->GetRootPane()->FindPaneByName(&lbl_eu_80504A3C[0x238], true);
    reinterpret_cast<CMenuGetItemPaneView*>(pane)->flags =
        (reinterpret_cast<CMenuGetItemPaneView*>(pane)->flags & 0xfe) | 1;
    pane = self->mLayout->GetRootPane()->FindPaneByName(&lbl_eu_80504A3C[0x448], true);
    reinterpret_cast<CMenuGetItemPaneView*>(pane)->flags =
        (reinterpret_cast<CMenuGetItemPaneView*>(pane)->flags & 0xfe) | 1;
    pane = self->mLayout->GetRootPane()->FindPaneByName(&lbl_eu_80504A3C[0x453], true);
    reinterpret_cast<CMenuGetItemPaneView*>(pane)->flags =
        (reinterpret_cast<CMenuGetItemPaneView*>(pane)->flags & 0xfe) | 1;
    pane = self->mLayout->GetRootPane()->FindPaneByName(&lbl_eu_80504A3C[0x97], true);
    reinterpret_cast<CMenuGetItemPaneView*>(pane)->flags =
        (reinterpret_cast<CMenuGetItemPaneView*>(pane)->flags & 0xfe) | 1;
    pane = self->mLayout->GetRootPane()->FindPaneByName(&lbl_eu_80504A3C[0xa3], true);
    reinterpret_cast<CMenuGetItemPaneView*>(pane)->flags =
        (reinterpret_cast<CMenuGetItemPaneView*>(pane)->flags & 0xfe) | 1;
    pane = self->mLayout->GetRootPane()->FindPaneByName(&lbl_eu_80504A3C[0xc7], true);
    reinterpret_cast<CMenuGetItemPaneView*>(pane)->flags =
        (reinterpret_cast<CMenuGetItemPaneView*>(pane)->flags & 0xfe) | 1;
    pane = self->mLayout->GetRootPane()->FindPaneByName(&lbl_eu_80504A3C[0x45e], true);
    reinterpret_cast<CMenuGetItemPaneView*>(pane)->flags =
        (reinterpret_cast<CMenuGetItemPaneView*>(pane)->flags & 0xfe) | 1;
    pane = self->mLayout->GetRootPane()->FindPaneByName(&lbl_eu_80504A3C[0x46b], true);
    reinterpret_cast<CMenuGetItemPaneView*>(pane)->flags =
        (reinterpret_cast<CMenuGetItemPaneView*>(pane)->flags & 0xfe) | 1;
    pane = self->mLayout->GetRootPane()->FindPaneByName(&lbl_eu_80504A3C[0x478], true);
    reinterpret_cast<CMenuGetItemPaneView*>(pane)->flags =
        (reinterpret_cast<CMenuGetItemPaneView*>(pane)->flags & 0xfe) | 1;
    pane = self->mLayout->GetRootPane()->FindPaneByName(&lbl_eu_80504A3C[0x485], true);
    reinterpret_cast<CMenuGetItemPaneView*>(pane)->flags =
        (reinterpret_cast<CMenuGetItemPaneView*>(pane)->flags & 0xfe) | 1;
    pane = self->mLayout->GetRootPane()->FindPaneByName(&lbl_eu_80504A3C[0x48f], true);
    reinterpret_cast<CMenuGetItemPaneView*>(pane)->flags =
        (reinterpret_cast<CMenuGetItemPaneView*>(pane)->flags & 0xfe) | 1;
    pane = self->mLayout->GetRootPane()->FindPaneByName(&lbl_eu_80504A3C[0x499], true);
    reinterpret_cast<CMenuGetItemPaneView*>(pane)->flags =
        (reinterpret_cast<CMenuGetItemPaneView*>(pane)->flags & 0xfe) | 1;

    // Cache the item-window font pointer in a saved register (retail keeps it
    // in r24 across the pane calls); the rank block below re-reads the global.
    // Results stay full-width (retail saves each with a plain mr; the byte
    // mask happens only when passed as the u8 arg of func_80136910).
    u32 itemFont = lbl_eu_806640F8;
    func_801392E4(arg2);
    u32 tableId = func_80139358(arg2);
    u32 v1 = func_801361E8(itemFont, &lbl_eu_80504A3C[0x4f0], (u16)tableId);
    u32 v2 = func_801361E8(itemFont, &lbl_eu_80504A3C[0x4f8], (u16)tableId);
    u32 v3 = func_801361E8(itemFont, &lbl_eu_80504A3C[0x53a], (u16)tableId);
    func_80136910(self->mLayout, &lbl_eu_80504A3C[0x97], (u8)v1);
    func_80136910(self->mLayout, &lbl_eu_80504A3C[0xa3], (u8)v2);
    func_80136910(self->mLayout, &lbl_eu_80504A3C[0xc7], (u8)v3);

    // Rank-category name for the slotted-item list header. Retail keeps the
    // raw lookup result in a saved reg AND takes a byte view once: the
    // comparison chain runs on the byte view (unsigned range test), while the
    // suffix switch below re-masks the raw value.
    u32 rankFont = lbl_eu_806640F8;
    u32 rankVal = func_801361E8(rankFont, &lbl_eu_80504A3C[0x543],
                                (u16)func_80139358(arg2));
    // Signed local: MWCC rewrites the 4..13 range test as an unsigned
    // subi/cmplwi pair but keeps signed cmpwi for the ==3/2/1 tests.
    s32 rank = rankVal & 0xff;
    char* rankName;
    if (rank >= 4 && rank <= 13) goto rank4_13;
    if (rank == 3) goto rank3;
    if (rank == 2) goto rank2;
    if (rank == 1) goto rank1;
    goto rankDefault;
rank3:
    rankName = func_80136190(&lbl_eu_80504A3C[0x1c1], &lbl_eu_80504A3C[0x182], 0x32);
    goto rankDone;
rank2:
    rankName = func_80136190(&lbl_eu_80504A3C[0x1c1], &lbl_eu_80504A3C[0x182], 0x31);
    goto rankDone;
rank1:
    rankName = func_80136190(&lbl_eu_80504A3C[0x1c1], &lbl_eu_80504A3C[0x182], 0x30);
    goto rankDone;
rank4_13:
    rankName = func_80136190(&lbl_eu_80504A3C[0x1c1], &lbl_eu_80504A3C[0x182], 0x2e);
    goto rankDone;
rankDefault:
    rankName = 0;
rankDone:
    func_80136B4C(self->mLayout, &lbl_eu_80504A3C[0x448], rankName, 0);

    // Per-rank suffix string (0x77 + rank-4), table-dispatched for ranks 4-12.
    char* rankSuffix = 0;
    switch ((u8)rankVal - 4) {
    case 0:
        rankSuffix = func_80136190(&lbl_eu_80504A3C[0x54c], &lbl_eu_80504A3C[0x182], 0x77);
        break;
    case 1:
        rankSuffix = func_80136190(&lbl_eu_80504A3C[0x54c], &lbl_eu_80504A3C[0x182], 0x78);
        break;
    case 2:
        rankSuffix = func_80136190(&lbl_eu_80504A3C[0x54c], &lbl_eu_80504A3C[0x182], 0x79);
        break;
    case 3:
        rankSuffix = func_80136190(&lbl_eu_80504A3C[0x54c], &lbl_eu_80504A3C[0x182], 0x7a);
        break;
    case 4:
        rankSuffix = func_80136190(&lbl_eu_80504A3C[0x54c], &lbl_eu_80504A3C[0x182], 0x7b);
        break;
    case 5:
        rankSuffix = func_80136190(&lbl_eu_80504A3C[0x54c], &lbl_eu_80504A3C[0x182], 0x7c);
        break;
    case 6:
        rankSuffix = func_80136190(&lbl_eu_80504A3C[0x54c], &lbl_eu_80504A3C[0x182], 0x7d);
        break;
    case 7:
        rankSuffix = func_80136190(&lbl_eu_80504A3C[0x54c], &lbl_eu_80504A3C[0x182], 0x7e);
        break;
    case 8:
        rankSuffix = func_80136190(&lbl_eu_80504A3C[0x54c], &lbl_eu_80504A3C[0x182], 0x7f);
        break;
    }
    func_80136B4C(self->mLayout, &lbl_eu_80504A3C[0x453], rankSuffix, 0);

    // Slot pane colour/position refresh; the shared entries use the default
    // palette, the two item-specific slots use the alt palette.
    func_80139A18(self->mLayout, &lbl_eu_80504A3C[0x1ca], &lbl_eu_806643B0, &lbl_eu_806643B8);
    func_80139A18(self->mLayout, &lbl_eu_80504A3C[0x1eb], &lbl_eu_806643A0, &lbl_eu_806643A8);
    func_80139A18(self->mLayout, &lbl_eu_80504A3C[0x1f6], &lbl_eu_806643A0, &lbl_eu_806643A8);
    func_80139A18(self->mLayout, &lbl_eu_80504A3C[0x1d5], &lbl_eu_806643B0, &lbl_eu_806643B8);
    func_80139A18(self->mLayout, &lbl_eu_80504A3C[0x1e0], &lbl_eu_806643B0, &lbl_eu_806643B8);
    func_80139A18(self->mLayout, &lbl_eu_80504A3C[0x201], &lbl_eu_806643A0, &lbl_eu_806643A8);
    func_80139A18(self->mLayout, &lbl_eu_80504A3C[0x238], &lbl_eu_806643A0, &lbl_eu_806643A8);

    func_801B7A58(self, arg2, arg3);
}

// Category-3 (rank item) display update: show the five rank-item panes, fill
// the item-name/rank-text panes, and record the slot position for the cursor.
// arg3 is the packed entry (its id wins over arg2); with no entry the id
// comes from arg2.
void func_801B70BC(CMenuGetItemMulti* self, int arg2, CMenuGetItemMultiEntry* arg3) {
    nw4r::lyt::Pane* pane = self->mLayout->GetRootPane()->FindPaneByName(
        &lbl_eu_80504A3C[0x424], true);
    reinterpret_cast<CMenuGetItemPaneView*>(pane)->flags =
        (reinterpret_cast<CMenuGetItemPaneView*>(pane)->flags & 0xfe) | 1;
    pane = self->mLayout->GetRootPane()->FindPaneByName(&lbl_eu_80504A3C[0x20c], true);
    reinterpret_cast<CMenuGetItemPaneView*>(pane)->flags =
        (reinterpret_cast<CMenuGetItemPaneView*>(pane)->flags & 0xfe) | 1;
    pane = self->mLayout->GetRootPane()->FindPaneByName(&lbl_eu_80504A3C[0x4a3], true);
    reinterpret_cast<CMenuGetItemPaneView*>(pane)->flags =
        (reinterpret_cast<CMenuGetItemPaneView*>(pane)->flags & 0xfe) | 1;
    pane = self->mLayout->GetRootPane()->FindPaneByName(&lbl_eu_80504A3C[0xd3], true);
    reinterpret_cast<CMenuGetItemPaneView*>(pane)->flags =
        (reinterpret_cast<CMenuGetItemPaneView*>(pane)->flags & 0xfe) | 1;
    pane = self->mLayout->GetRootPane()->FindPaneByName(&lbl_eu_80504A3C[0xdf], true);
    reinterpret_cast<CMenuGetItemPaneView*>(pane)->flags =
        (reinterpret_cast<CMenuGetItemPaneView*>(pane)->flags & 0xfe) | 1;

    u32 itemId = (u32)arg2;
    if (arg3 != 0) {
        itemId = arg3->packed >> 20;
    }

    // No-entry path validates the id through the item table (result unused).
    if (arg3 == 0) {
        func_801392E4((u16)itemId);
    }

    u32 itemId2;
    if (arg3 != 0) {
        itemId2 = CItem_initItemImplInstances(arg3)->vf54(arg3);
    } else {
        itemId2 = func_80139358((u16)itemId);
    }

    // Rank count is a nested ternary temporary inside the func_80136190
    // argument (retail masks both arms into r0 and joins with a third
    // byte-mask straight into the argument register).
    char* rankName = func_80136190(
        &lbl_eu_80504A3C[0x1c1], &lbl_eu_80504A3C[0x182],
        30 - ((u8)((arg3 != 0)
                       ? (u16)CItem_initItemImplInstances(arg3)->getRankCount(arg3)
                       : (u16)func_801361E8(lbl_eu_806640EC, &lbl_eu_80504A3C[0x3b3],
                                            (u16)itemId)) -
              1));
    func_80136B4C(self->mLayout, &lbl_eu_80504A3C[0xd3], rankName, 0);

    u16 v = func_80136254(lbl_eu_806640D8, &lbl_eu_80504A3C[0x554], (u16)itemId2);
    char* rankText = func_80136190(&lbl_eu_80504A3C[0x558], &lbl_eu_80504A3C[0x182], 0xf);
    if (arg3 != 0) {
        func_80136910(self->mLayout, &lbl_eu_80504A3C[0xdf],
                      CItem_initItemImplInstances(arg3)->vf90(arg3));
    } else {
        func_80136B4C(self->mLayout, &lbl_eu_80504A3C[0xdf],
                      (v == 1) ? &lbl_eu_80504A3C[0x19f] : rankText, 0);
    }

    char* itemName = func_8013639C(lbl_eu_806640D8, &lbl_eu_80504A3C[0x182], (u16)itemId2);
    func_80136B4C(self->mLayout, &lbl_eu_80504A3C[0x4a3], itemName, 0);

    u8 n = (u8)func_801361E8((u32)lbl_eu_806640D8, &lbl_eu_80504A3C[0x561], (u16)itemId2);
    char* rankStr = func_80136190(&lbl_eu_80504A3C[0x1c1], &lbl_eu_80504A3C[0x182],
                                  n + 0x15);
    func_80136B4C(self->mLayout, &lbl_eu_80504A3C[0x568], rankStr, 0);

    nw4r::lyt::Pane* slotPane = self->mLayout->GetRootPane()->FindPaneByName(
        &lbl_eu_80504A3C[0x4a3], true);
    nw4r::lyt::Pane* refPane = self->mLayout->GetRootPane()->FindPaneByName(
        &lbl_eu_80504A3C[0x3d6], true);
    nw4r::math::VEC3 pos;
    func_80137924(&pos, slotPane, refPane, self->mLayout->GetRootPane());

    self->mRankSlotIds[8] = (u16)itemId2;
    self->mRankSlotFlag[8] = 0;
    // Materialize a copy of the position for the array write (retail keeps
    // the copy in a scratch VEC3 slot).
    nw4r::math::VEC3 posCopy = pos;
    self->mRankSlotPos[8] = posCopy;
}

__declspec(noinline) void func_801B76CC(CMenuGetItemMulti* self, int arg2) {
    // Show the five panes this category touches.
    nw4r::lyt::Pane* pane = self->mLayout->GetRootPane()->FindPaneByName(
        &lbl_eu_80504A3C[0x430], true);
    reinterpret_cast<CMenuGetItemPaneView*>(pane)->flags =
        (reinterpret_cast<CMenuGetItemPaneView*>(pane)->flags & 0xfe) | 1;
    pane = self->mLayout->GetRootPane()->FindPaneByName(&lbl_eu_80504A3C[0x217], true);
    reinterpret_cast<CMenuGetItemPaneView*>(pane)->flags =
        (reinterpret_cast<CMenuGetItemPaneView*>(pane)->flags & 0xfe) | 1;
    pane = self->mLayout->GetRootPane()->FindPaneByName(&lbl_eu_80504A3C[0x222], true);
    reinterpret_cast<CMenuGetItemPaneView*>(pane)->flags =
        (reinterpret_cast<CMenuGetItemPaneView*>(pane)->flags & 0xfe) | 1;
    pane = self->mLayout->GetRootPane()->FindPaneByName(&lbl_eu_80504A3C[0x4ae], true);
    reinterpret_cast<CMenuGetItemPaneView*>(pane)->flags =
        (reinterpret_cast<CMenuGetItemPaneView*>(pane)->flags & 0xfe) | 1;
    pane = self->mLayout->GetRootPane()->FindPaneByName(&lbl_eu_80504A3C[0x4ba], true);
    reinterpret_cast<CMenuGetItemPaneView*>(pane)->flags =
        (reinterpret_cast<CMenuGetItemPaneView*>(pane)->flags & 0xfe) | 1;

    // Item id -> table id, then fill the rank-name and description panes.
    void* tbl = lbl_eu_80664104;
    func_801392E4(arg2);
    u32 tableId = func_80139358(arg2);
    // Separate statement: retail keeps the lookup result live in r0 across
    // the func_80136190 arg setup (mr r0, r3 / clrlwi r5, r0, 24).
    u32 rankN = func_801361E8((u32)tbl, &lbl_eu_80504A3C[0x573], (u16)tableId);
    char* rankText = func_80136190(
        &lbl_eu_80504A3C[0x1ac], &lbl_eu_80504A3C[0x182], (u8)rankN);
    func_80136B4C(self->mLayout, &lbl_eu_80504A3C[0x4ae], rankText, 0);

    // Description text: fixed string 0x1a gets a formatted entry, otherwise
    // the plain key-item name from the font table.
    char* text;
    // Raw u32 kept in r0; the (u8) truncations happen at the use sites
    // (retail mr r0/r3 + rlwinm shape).
    u32 descId = func_801361E8((u32)tbl, &lbl_eu_80504A3C[0x578], (u16)tableId);
    if ((u8)descId == 0x1a) {
        text = func_80136190(&lbl_eu_80504A3C[0x1ac], &lbl_eu_80504A3C[0x182], 20);
    } else {
        // Third arg rides in r5 from the lookup above (retail leaves it live).
        text = func_8013639C(lbl_eu_806640A8, &lbl_eu_80504A3C[0x182], (u8)descId);
    }
    func_80136B4C(self->mLayout, &lbl_eu_80504A3C[0x4ba], text, 0);
}

// Category-9 (rank item) display update: show the rank pane, render the rank
// name, and populate the per-slot id/position/flag arrays for the four slots.
// Guarded by arg3 != 0 (dispatch from func_801B5860).
void func_801B7440(CMenuGetItemMulti* self, CMenuGetItemMultiEntry* arg3) {
    if (arg3 == 0) {
        return;
    }

    nw4r::lyt::Pane* pane = self->mLayout->GetRootPane()->FindPaneByName(
        &lbl_eu_80504A3C[0x418], true);
    reinterpret_cast<CMenuGetItemPaneView*>(pane)->flags =
        (reinterpret_cast<CMenuGetItemPaneView*>(pane)->flags & 0xfe) | 1;

    u16 rankCount = (u16)CItem_initItemImplInstances(arg3)->getRankCount(arg3);
    char* rankName = func_80136190(&lbl_eu_80504A3C[0x1c1],
                                   &lbl_eu_80504A3C[0x182],
                                   30 - ((u8)rankCount - 1));
    func_80136B4C(self->mLayout, &lbl_eu_80504A3C[0xf7], rankName, 0);

    // paneNo counts only the non-empty slots (its pane names are built from
    // it), while i indexes the impl slots -- MWCC keeps them as two counters.
    u8 paneNo = 0;
    for (u8 i = 0; i < 4; ++i) {
        // Signed id: MWCC records the truncation (clrlwi.) and emits the
        // signed `ble` skip (retail shape); a u16 here collapses to `beq`.
        s32 itemId = (u16)CItem_initItemImplInstances(arg3)->vf4C(arg3, i);
        if (itemId <= 0) {
            continue;
        }
        char paneName[32];
        sprintf(paneName, &lbl_eu_80504A3C[0x4c6], paneNo * 2 + 0x1f);
        char* itemName = func_8013639C(lbl_eu_806640D8, &lbl_eu_80504A3C[0x182], itemId);
        func_80136B4C(self->mLayout, paneName, itemName, 0);

        nw4r::lyt::Pane* slotPane =
            self->mLayout->GetRootPane()->FindPaneByName(paneName, true);
        nw4r::lyt::Pane* refPane = self->mLayout->GetRootPane()->FindPaneByName(
            &lbl_eu_80504A3C[0x3d6], true);
        nw4r::math::VEC3 pos;
        func_80137924(&pos, slotPane, refPane, self->mLayout->GetRootPane());

        u8 idx = (u8)(i + 8);
        if (idx < 0xc) {
            self->mRankSlotIds[idx] = itemId;
        }
        if (idx < 0xc) {
            self->mRankSlotFlag[idx] = 9;
        }
        // Materialize a copy of the position before the conditional array
        // write (retail keeps the copy in a scratch VEC3 slot).
        nw4r::math::VEC3 posCopy = pos;
        if (idx < 0xc) {
            self->mRankSlotPos[idx] = posCopy;
        }

        ml::FixStr<32> str;
        sprintf(paneName, &lbl_eu_80504A3C[0x4d3], paneNo + 0x1f);
        u32 slotId = CItem_initItemImplInstances(arg3)->vf64(arg3, i);
        str.format(&lbl_eu_80504A3C[0x535], (u16)slotId,
                   func_80136190(&lbl_eu_80504A3C[0x1c1], &lbl_eu_80504A3C[0x182], 0x21));
        func_80136B4C(self->mLayout, paneName, str.mString, 0);

        ++paneNo;
    }
}

// Category-13 display update: show the pane, then build the rank-name string
// from the art/skill record tables (lbl_eu_80664110): three lookups keyed by
// the item's table id select the rank index, the record-set and a flag byte,
// and the per-character record (+0xE8/+0xE9 flags) switches the rank suffix
// between string 44 and 43.
__declspec(noinline) void func_801B78B4(CMenuGetItemMulti* self, int arg2) {
    nw4r::lyt::Pane* pane = self->mLayout->GetRootPane()->FindPaneByName(
        &lbl_eu_80504A3C[0x43c], true);
    reinterpret_cast<CMenuGetItemPaneView*>(pane)->flags =
        (reinterpret_cast<CMenuGetItemPaneView*>(pane)->flags & 0xfe) | 1;

    void* tbl = (void*)lbl_eu_80664110;
    // No-entry path validates the id through the item table (result unused).
    func_801392E4(arg2);
    u32 tableId = func_80139358(arg2);
    // Raw results kept unmasked (retail stores each with a plain mr and
    // truncates only at the use sites).
    int kindA = func_801361E8((u32)tbl, &lbl_eu_80504A3C[0x57e], (u16)tableId);
    u32 artIdx = func_801361E8((u32)tbl, &lbl_eu_80504A3C[0x58a], (u16)tableId);
    // Record address: per-character stride 0x49 plus the doubled selector,
    // folded back into selIdx (all-u32 so MWCC targets its register).
    u32 selIdx = func_801361E8((u32)tbl, &lbl_eu_80504A3C[0x592], (u16)tableId);
    selIdx = func_8013600C(&lbl_eu_80504A3C[0x54c], &lbl_eu_80504A3C[0x59b],
                           (u8)selIdx);
    void* mgr = func_8009EC9C((u8)artIdx);
    // Record address: per-character stride 0x49 plus the doubled selector,
    // kept in u32 locals to match MWCC's register allocation.
    u32 recBase = reinterpret_cast<u32>(mgr) +
                  (u8)func_800A32BC(mgr) * 0x49;
    selIdx = (u8)selIdx * 2 + recBase;
    CMenuGetItemCat13Record* rec =
        reinterpret_cast<CMenuGetItemCat13Record*>(selIdx);
    char* rankName =
        func_80136190(&lbl_eu_80504A3C[0x1c1], &lbl_eu_80504A3C[0x182], 0x2c);

    switch ((u8)kindA) {
    case 1:
        if (rec->flagE8 != 0) {
            rankName = func_80136190(&lbl_eu_80504A3C[0x1c1],
                                     &lbl_eu_80504A3C[0x182], 0x2b);
        }
        break;
    case 2:
        if ((rec->bitsE9 >> 7) & 1) {
            rankName = func_80136190(&lbl_eu_80504A3C[0x1c1],
                                     &lbl_eu_80504A3C[0x182], 0x2b);
        }
        break;
    case 3:
        if ((rec->bitsE9 >> 6) & 1) {
            rankName = func_80136190(&lbl_eu_80504A3C[0x1c1],
                                     &lbl_eu_80504A3C[0x182], 0x2b);
        }
        break;
    }

    func_80136B4C(self->mLayout, &lbl_eu_80504A3C[0x59f], rankName, 0);
}

// Rank-window slot filler (slots 8-10) shared by the equipped/slotted-item
// handlers: for each of the three rank slots, look up the item's per-rank
// record and fill the slot's name/rank text panes, then record the id,
// position and rank data in the mRankSlot* arrays. With no packed entry the
// ids come from arg2 through the item table instead of the impl vtable.
void func_801B7A58(CMenuGetItemMulti* self, int arg2, CMenuGetItemMultiEntry* arg3) {
    // Shared pane-name buffer and formatted-string slot (retail keeps both at
    // fixed frame offsets across both paths; the ctor-less FixStr(false) is
    // inert and only mString[0] is cleared up front).
    ml::FixStr<32> str(false);
    str.mString[0] = 0;
    str.mLength = 0;
    char buf[32];
    if (arg3 != NULL && arg3->packed != 0) {
        u16 count = reinterpret_cast<CMenuGetItemImplCount*>(
            CItem_initItemImplInstances(arg3))->getCount30(arg3);
        nw4r::math::VEC3 pos;
        nw4r::math::VEC3 posCopy;
        for (u8 i = 0; i < 3; ++i) {
            sprintf(buf, &lbl_eu_80504A3C[0x5aa], (u8)i + 1);
            nw4r::lyt::Pane* pane =
                self->mLayout->GetRootPane()->FindPaneByName(buf, true);
            // Per-slot rank data (zeroed when the slot is inactive).
            u16 slotItemId = 0;
            u8 slotRank = 0;
            s16 slotVal = 0;
            if ((u8)i < count) {
                void* texture = 0;
                reinterpret_cast<CMenuGetItemPaneView*>(pane)->flags =
                    (reinterpret_cast<CMenuGetItemPaneView*>(pane)->flags & 0xfe) | 1;
                CMenuGetItemRankSlot* slot = reinterpret_cast<CMenuGetItemRankSlot*>(
                    CItem_initItemImplInstances(arg3)->getSlot(arg3, i));
                if (slot != 0 && (slot->bits04 & 1) != 0) {
                    slotItemId = (slot->bits04 >> 4) & 0xFFF;
                    void* fontTbl = lbl_eu_806640D8;
                    slotRank = (slot->bits04 >> 7) & 7;
                    slotVal = (s16)((slot->bits04 >> 10) & 0x7FF);
                    char* rankStr = func_80136190(
                        &lbl_eu_80504A3C[0x1c1], &lbl_eu_80504A3C[0x182],
                        30 - (slotRank - 1));
                    switch ((u8)func_801361E8(
                        (u32)fontTbl, &lbl_eu_80504A3C[0x5b9], slotItemId)) {
                    case 0:
                        texture = func_801355F4()->GetResource(
                            nw4r::lyt::ArcResourceAccessor::RES_TYPE_TEXTURE,
                            &lbl_eu_80504A3C[0x5c2], NULL);
                        break;
                    case 4:
                        texture = func_801355F4()->GetResource(
                            nw4r::lyt::ArcResourceAccessor::RES_TYPE_TEXTURE,
                            &lbl_eu_80504A3C[0x5d8], NULL);
                        break;
                    case 5:
                        texture = func_801355F4()->GetResource(
                            nw4r::lyt::ArcResourceAccessor::RES_TYPE_TEXTURE,
                            &lbl_eu_80504A3C[0x5ee], NULL);
                        break;
                    case 6:
                        texture = func_801355F4()->GetResource(
                            nw4r::lyt::ArcResourceAccessor::RES_TYPE_TEXTURE,
                            &lbl_eu_80504A3C[0x604], NULL);
                        break;
                    case 7:
                        texture = func_801355F4()->GetResource(
                            nw4r::lyt::ArcResourceAccessor::RES_TYPE_TEXTURE,
                            &lbl_eu_80504A3C[0x61a], NULL);
                        break;
                    case 8:
                        texture = func_801355F4()->GetResource(
                            nw4r::lyt::ArcResourceAccessor::RES_TYPE_TEXTURE,
                            &lbl_eu_80504A3C[0x630], NULL);
                        break;
                    case 9:
                        texture = func_801355F4()->GetResource(
                            nw4r::lyt::ArcResourceAccessor::RES_TYPE_TEXTURE,
                            &lbl_eu_80504A3C[0x646], NULL);
                        break;
                    }
                    str.format(&lbl_eu_80504A3C[0x65c],
                               func_8013639C(fontTbl, &lbl_eu_80504A3C[0x182],
                                             slotItemId),
                               rankStr);
                } else {
                    texture = func_801355F4()->GetResource(
                        nw4r::lyt::ArcResourceAccessor::RES_TYPE_TEXTURE,
                        &lbl_eu_80504A3C[0x5c2], NULL);
                    str.format(&lbl_eu_80504A3C[0x65c],
                               func_80136190(&lbl_eu_80504A3C[0x1c1],
                                             &lbl_eu_80504A3C[0x182], 0x2a));
                }
                sprintf(buf, &lbl_eu_80504A3C[0x65f], (u8)i + 1);
                func_80136B4C(self->mLayout, buf, str.mString, 0);
                if (texture != 0) {
                    func_80137F88(pane, texture);
                }
            } else {
                reinterpret_cast<CMenuGetItemPaneView*>(pane)->flags =
                    reinterpret_cast<CMenuGetItemPaneView*>(pane)->flags & 0xfe;
                sprintf(buf, &lbl_eu_80504A3C[0x65f], (u8)i + 1);
                func_80136B4C(self->mLayout, buf, &lbl_eu_80504A3C[0x19f], 0);
            }

            nw4r::lyt::Pane* refPane = self->mLayout->GetRootPane()->FindPaneByName(
                &lbl_eu_80504A3C[0x3d6], true);
            nw4r::math::VEC3 pos;
            func_80137924(&pos, pane, refPane, self->mLayout->GetRootPane());
            u8 idx = (u8)(i + 8);
            if (idx < 0xc) {
                self->mRankSlotIds[idx] = slotItemId;
            }
            if (idx < 0xc) {
                self->mRankSlotFlag[idx] = 3;
            }
            if (idx < 0xc) {
                self->mRankSlotFlag2[idx] = slotRank;
            }
            if (idx < 0xc) {
                self->mRankSlotIds2[idx] = slotVal;
            }
            nw4r::math::VEC3 posCopy = pos;
            if (idx < 0xc) {
                self->mRankSlotPos[idx] = posCopy;
            }
        }
    } else {
        void* itemTable = func_801393CC(arg2);
        u32 tableId = func_80139358(arg2);
        u8 count =
            (u8)func_801361E8((u32)itemTable, &lbl_eu_80504A3C[0x39e], (u16)tableId);
        u16 slotId = 0;
        u8 rankCnt = 0;
        u16 altFlag = 0;
        nw4r::math::VEC3 pos;
        nw4r::math::VEC3 posCopy;
        for (u8 j = 0; j < 3; ++j) {
            sprintf(buf, &lbl_eu_80504A3C[0x5aa], (u8)j + 1);
            nw4r::lyt::Pane* pane =
                self->mLayout->GetRootPane()->FindPaneByName(buf, true);
            if ((u8)j < count) {
                void* texture = 0;
                reinterpret_cast<CMenuGetItemPaneView*>(pane)->flags =
                    (reinterpret_cast<CMenuGetItemPaneView*>(pane)->flags & 0xfe) | 1;
                str.format(&lbl_eu_80504A3C[0x3a7], (u8)j + 1);
                u16 id = func_80136254(itemTable, str.mString, (u16)tableId);
                if (id == 0) {
                    texture = func_801355F4()->GetResource(
                        nw4r::lyt::ArcResourceAccessor::RES_TYPE_TEXTURE,
                        &lbl_eu_80504A3C[0x5c2], NULL);
                    str.format(&lbl_eu_80504A3C[0x65c],
                               func_80136190(&lbl_eu_80504A3C[0x1c1],
                                             &lbl_eu_80504A3C[0x182], 0x2a));
                } else {
                    altFlag = func_801361E8(lbl_eu_806640EC,
                                            &lbl_eu_80504A3C[0x66b], id);
                    rankCnt = func_801361E8(lbl_eu_806640EC,
                                            &lbl_eu_80504A3C[0x3b3], id);
                    void* fontTbl2 = lbl_eu_806640D8;
                    switch ((u8)func_801361E8(
                        (u32)fontTbl2, &lbl_eu_80504A3C[0x5b9],
                        (u16)func_80139358(id))) {
                    case 0:
                        texture = func_801355F4()->GetResource(
                            nw4r::lyt::ArcResourceAccessor::RES_TYPE_TEXTURE,
                            &lbl_eu_80504A3C[0x5c2], NULL);
                        break;
                    case 4:
                        texture = func_801355F4()->GetResource(
                            nw4r::lyt::ArcResourceAccessor::RES_TYPE_TEXTURE,
                            &lbl_eu_80504A3C[0x5d8], NULL);
                        break;
                    case 5:
                        texture = func_801355F4()->GetResource(
                            nw4r::lyt::ArcResourceAccessor::RES_TYPE_TEXTURE,
                            &lbl_eu_80504A3C[0x5ee], NULL);
                        break;
                    case 6:
                        texture = func_801355F4()->GetResource(
                            nw4r::lyt::ArcResourceAccessor::RES_TYPE_TEXTURE,
                            &lbl_eu_80504A3C[0x604], NULL);
                        break;
                    case 7:
                        texture = func_801355F4()->GetResource(
                            nw4r::lyt::ArcResourceAccessor::RES_TYPE_TEXTURE,
                            &lbl_eu_80504A3C[0x61a], NULL);
                        break;
                    case 8:
                        texture = func_801355F4()->GetResource(
                            nw4r::lyt::ArcResourceAccessor::RES_TYPE_TEXTURE,
                            &lbl_eu_80504A3C[0x630], NULL);
                        break;
                    case 9:
                        texture = func_801355F4()->GetResource(
                            nw4r::lyt::ArcResourceAccessor::RES_TYPE_TEXTURE,
                            &lbl_eu_80504A3C[0x646], NULL);
                        break;
                    }
                    str.format(&lbl_eu_80504A3C[0x65c], func_801394D4(id));
                    slotId = func_80139358(id);
                }
                sprintf(buf, &lbl_eu_80504A3C[0x65f], (u8)j + 1);
                func_80136B4C(self->mLayout, buf, str.mString, 0);
                if (texture != 0) {
                    func_80137F88(pane, texture);
                }
            } else {
                sprintf(buf, &lbl_eu_80504A3C[0x65f], (u8)j + 1);
                func_80136B4C(self->mLayout, buf, &lbl_eu_80504A3C[0x19f], 0);
                reinterpret_cast<CMenuGetItemPaneView*>(pane)->flags &= 0xfe;
            }

            nw4r::lyt::Pane* refPane = self->mLayout->GetRootPane()->FindPaneByName(
                &lbl_eu_80504A3C[0x3d6], true);
            nw4r::math::VEC3 pos;
            func_80137924(&pos, pane, refPane, self->mLayout->GetRootPane());
            u8 idx = (u8)(j + 8);
            if (idx < 0xc) {
                self->mRankSlotIds[idx] = slotId;
            }
            if (idx < 0xc) {
                self->mRankSlotFlag[idx] = 3;
            }
            if (idx < 0xc) {
                self->mRankSlotFlag2[idx] = rankCnt;
            }
            if (idx < 0xc) {
                self->mRankSlotIds2[idx] = altFlag;
            }
            nw4r::math::VEC3 posCopy = pos;
            if (idx < 0xc) {
                self->mRankSlotPos[idx] = posCopy;
            }
        }
    }
}

// Pad-input handler for the item-select window (Move state 2). Reads the pad
// and dispatches on five inputs: up/down (pane-list navigation), the turbo
// held bits (auto-repeat selection scan in the slot window, or the pane-list
// wrap-around) and the A button (open the slot window for the current
// selection). The classic-controller bit layout is swapped per CfGameManager.
__declspec(noinline) void func_801B82E8(CMenuGetItemMulti* self) {
    cf::CfPadData* pad = cf::CfGameManager::getCfPadData();

    if ((pad->mPad.mHeldButtonFlags & 0x1E000) == 0) {
        self->field_200 = 0;
    }
    if (self->field_200 != 0) {
        return;
    }

    u32 up, down, curHeld, maskHeld, aPressed;
    if (func_80086F9C__Q22cf13CfGameManagerFv(-1) != 0) {
        maskHeld = pad->mTurboPressButtonFlags & 0x8004;
        curHeld = pad->mTurboPressButtonFlags & 0x10000;
        curHeld |= pad->mTurboPressButtonFlags & 0x8;
        up = (pad->mPad.mPressedButtonFlags >> 10) & 1;
        down = (pad->mPad.mPressedButtonFlags >> 9) & 1;
        aPressed = (pad->mPad.mPressedButtonFlags >> 22) & 1;
        maskHeld = maskHeld != 0;
        curHeld = curHeld != 0;
    } else {
        maskHeld = pad->mTurboPressButtonFlags & 0x8004;
        curHeld = pad->mTurboPressButtonFlags & 0x10000;
        curHeld |= pad->mTurboPressButtonFlags & 0x8;
        up = (pad->mPad.mPressedButtonFlags >> 27) & 1;
        down = (pad->mPad.mPressedButtonFlags >> 26) & 1;
        aPressed = (pad->mPad.mPressedButtonFlags >> 22) & 1;
        maskHeld = maskHeld != 0;
        curHeld = curHeld != 0;
    }

    if (up) {
        // Up over the close pane: a live system window wins; otherwise open
        // the rank-up window for the selected rank item (or refuse with a
        // buzz when the pane list is not in the rank state).
        if (CSysWin_getUnk34(&self->mSystemWindow[0]) != 0) {
            if (CSysWin_isActive(&self->mSystemWindow[0]) == 0) {
                return;
            }
            func_8022B8E4(&self->mSystemWindow[0]);
            func_801D216C(&self->mCursor, 1);
            return;
        }
        if (self->field_20C != 0) {
            return;
        }
        if (self->mMaxVisibleItems != 5) {
            self->field_1F8 = 4;
            func_80138078__FUl(3);
            return;
        }
        if (self->mHasSpecialItem != 0) {
            func_80138078__FUl(5);
            return;
        }
        self->field_1F8 = 8;
        char* n1 = func_80136190(&lbl_eu_80504A3C[0x177],
                                 &lbl_eu_80504A3C[0x182], 0x1c);
        char* n2 = func_80136190(&lbl_eu_80504A3C[0x177],
                                 &lbl_eu_80504A3C[0x182], 0x1d);
        char* n3 = func_80136190(&lbl_eu_80504A3C[0x177],
                                 &lbl_eu_80504A3C[0x182], 0x1e);
        func_8022B90C(reinterpret_cast<CSysWin*>(&self->mSystemWindow[0]), 2);
        func_8022B9B4(&self->mSystemWindow[0], n1, 0);
        func_8022BF6C(&self->mSystemWindow[0], n2, n3);
        func_8022BFC8(reinterpret_cast<CSysWin*>(&self->mSystemWindow[0]), 0);
        func_8022B8B8(&self->mSystemWindow[0]);
        func_801D216C(&self->mCursor, 0);
        self->field_1F4 = 1;
        return;
    }

    if (down) {
        // Down over a selected slot closes the slot window and snaps the
        // cursor back onto the pane list.
        if (self->field_20C == 0) {
            return;
        }
        if (CSysWin_getUnk34(&self->mSystemWindow[0]) != 0) {
            if (CSysWin_isActive(&self->mSystemWindow[0]) == 0) {
                return;
            }
            func_8022B8E4(&self->mSystemWindow[0]);
            func_801D216C(&self->mCursor, 1);
            return;
        }
        self->field_20C = 0;
        {
            char buf[32];
            s8 m = (s8)self->mMaxVisibleItems;
            if (m == 5) {
                sprintf(buf, &lbl_eu_80504A3C[0x16b]);
            } else if (m == 4) {
                sprintf(buf, &lbl_eu_80504A3C[0x192], m + 1);
            } else {
                sprintf(buf, &lbl_eu_80504A3C[0x3c9], m + 1);
            }
            nw4r::lyt::Pane* pane = self->mLayout->GetRootPane()->FindPaneByName(buf, true);
            nw4r::lyt::Pane* refPane = self->mLayout->GetRootPane()->FindPaneByName(
                &lbl_eu_80504A3C[0x3d6], true);
            nw4r::math::VEC3 pos;
            func_80137924(&pos, pane, refPane, self->mLayout->GetRootPane());
            reinterpret_cast<CMenuGetItemMultiCur*>(&self->mCursor)->vfn4(&pos);
        }
        func_80138078__FUl(6);
        return;
    }

    if (maskHeld) {
        // Left/right held in the slot window: scan the slot group backwards
        // for the previous filled slot; on the pane list, wrap the selection
        // backwards and re-centre the cursor.
        if (CSysWin_getUnk34(&self->mSystemWindow[0]) != 0) {
            return;
        }
        if (self->field_20C != 0) {
            s8 cand = (s8)(self->field_20E - 1);
            s8 orig = (s8)self->field_20E;
            do {
                if (cand < 0) {
                    cand = 3;
                }
                u8 idx = (u8)((s8)self->field_20D * 4 + cand);
                u16 slotId = (idx < 0xc) ? self->mRankSlotIds[idx] : 0;
                if (slotId != 0) {
                    self->field_20E = (u8)cand;
                    break;
                }
                cand = (s8)(cand - 1);
            } while (cand != orig);
            u8 posIdx = (u8)((s8)self->field_20D * 4 + self->field_20E);
            nw4r::math::VEC3 pos;
            if (posIdx < 0xc) {
                pos = self->mRankSlotPos[posIdx];
            }
            reinterpret_cast<CMenuGetItemMultiCur*>(&self->mCursor)->vfn4(&pos);
            return;
        }

        if (self->mMaxVisibleItems == 4) {
            self->mMaxVisibleItems = (u8)(self->mVisibleItemCount - 1);
        } else {
            self->mMaxVisibleItems = self->mMaxVisibleItems - 1;
            if ((s8)self->mMaxVisibleItems < 0) {
                self->mMaxVisibleItems = 5;
            }
        }
        if (self->mEntryCount != 0) {
            CMenuGetItemMultiEntry* entry = self->mVisibleEntries[(s8)self->mMaxVisibleItems];
            if (entry != 0) {
                func_801B5860(self, entry->packed >> 20, entry);
            } else {
                func_801B5860(self, 0, 0);
            }
        } else {
            u16 id = self->mVisibleItemIds[(s8)self->mMaxVisibleItems];
            if (id != 0) {
                func_801B5860(self, id, 0);
            } else {
                func_801B5860(self, 0, 0);
            }
        }
        {
            char buf[32];
            s8 m = (s8)self->mMaxVisibleItems;
            if (m == 5) {
                sprintf(buf, &lbl_eu_80504A3C[0x16b]);
            } else if (m == 4) {
                sprintf(buf, &lbl_eu_80504A3C[0x192], m + 1);
            } else {
                sprintf(buf, &lbl_eu_80504A3C[0x3c9], m + 1);
            }
            nw4r::lyt::Pane* pane = self->mLayout->GetRootPane()->FindPaneByName(buf, true);
            nw4r::lyt::Pane* refPane = self->mLayout->GetRootPane()->FindPaneByName(
                &lbl_eu_80504A3C[0x3d6], true);
            nw4r::math::VEC3 pos;
            func_80137924(&pos, pane, refPane, self->mLayout->GetRootPane());
            reinterpret_cast<CMenuGetItemMultiCur*>(&self->mCursor)->vfn4(&pos);
        }
        func_80138078__FUl(1);
        return;
    }

    if (curHeld) {
        // Right held: scan the slot group forwards; on the pane list, wrap
        // the selection forwards.
        if (CSysWin_getUnk34(&self->mSystemWindow[0]) != 0) {
            return;
        }
        if (self->field_20C != 0) {
            s8 cand = (s8)(self->field_20E + 1);
            s8 orig = (s8)self->field_20E;
            do {
                if (cand >= 4) {
                    cand = 0;
                }
                u8 idx = (u8)((s8)self->field_20D * 4 + cand);
                u16 slotId = (idx < 0xc) ? self->mRankSlotIds[idx] : 0;
                if (slotId != 0) {
                    self->field_20E = (u8)cand;
                    break;
                }
                cand = (s8)(cand + 1);
            } while (cand != orig);
            u8 posIdx = (u8)((s8)self->field_20D * 4 + self->field_20E);
            nw4r::math::VEC3 pos;
            if (posIdx < 0xc) {
                pos = self->mRankSlotPos[posIdx];
            }
            reinterpret_cast<CMenuGetItemMultiCur*>(&self->mCursor)->vfn4(&pos);
            return;
        }

        if (self->mMaxVisibleItems == 5) {
            self->mMaxVisibleItems = 0;
        } else {
            self->mMaxVisibleItems = self->mMaxVisibleItems + 1;
            if ((s8)self->mMaxVisibleItems >= (s32)self->mVisibleItemCount &&
                (s8)self->mMaxVisibleItems != 5) {
                self->mMaxVisibleItems = 4;
            }
        }
        if (self->mEntryCount != 0) {
            CMenuGetItemMultiEntry* entry = self->mVisibleEntries[(s8)self->mMaxVisibleItems];
            if (entry != 0) {
                func_801B5860(self, entry->packed >> 20, entry);
            } else {
                func_801B5860(self, 0, 0);
            }
        } else {
            u16 id = self->mVisibleItemIds[(s8)self->mMaxVisibleItems];
            if (id != 0) {
                func_801B5860(self, id, 0);
            } else {
                func_801B5860(self, 0, 0);
            }
        }
        {
            char buf[32];
            s8 m = (s8)self->mMaxVisibleItems;
            if (m == 5) {
                sprintf(buf, &lbl_eu_80504A3C[0x16b]);
            } else if (m == 4) {
                sprintf(buf, &lbl_eu_80504A3C[0x192], m + 1);
            } else {
                sprintf(buf, &lbl_eu_80504A3C[0x3c9], m + 1);
            }
            nw4r::lyt::Pane* pane = self->mLayout->GetRootPane()->FindPaneByName(buf, true);
            nw4r::lyt::Pane* refPane = self->mLayout->GetRootPane()->FindPaneByName(
                &lbl_eu_80504A3C[0x3d6], true);
            nw4r::math::VEC3 pos;
            func_80137924(&pos, pane, refPane, self->mLayout->GetRootPane());
            reinterpret_cast<CMenuGetItemMultiCur*>(&self->mCursor)->vfn4(&pos);
        }
        func_80138078__FUl(1);
        return;
    }

    if (aPressed) {
        // A opens the slot window for the currently selected slot (or, when
        // no slot window is open yet, scans the rank slots for the first
        // filled one).
        nw4r::lyt::Pane* pane = self->mLayout->GetRootPane()->FindPaneByName(
            &lbl_eu_80504A3C[0x404], true);
        if ((reinterpret_cast<CMenuGetItemPaneView*>(pane)->flags & 1) != 0) {
            if (self->field_20C != 0) {
                if (CSysWin_getUnk34(&self->mSystemWindow[0]) != 0) {
                    if (CSysWin_isActive(&self->mSystemWindow[0]) == 0) {
                        return;
                    }
                    func_8022B8E4(&self->mSystemWindow[0]);
                    func_801D216C(&self->mCursor, 1);
                    return;
                }
                u8 idx = (u8)((s8)self->field_20D * 4 + self->field_20E);
                u16 slotId = (idx < 0xc) ? self->mRankSlotIds[idx] : 0;
                if (slotId == 0) {
                    return;
                }
                u8 flag = (idx < 0xc) ? self->mRankSlotFlag[idx] : 0;
                char* text;
                if (flag == 3) {
                    text = func_801D3C74();
                } else {
                    text = func_8013639C(reinterpret_cast<const void*>(lbl_eu_80664418),
                                         &lbl_eu_80504A3C[0x141], slotId);
                }
                func_8022B90C(reinterpret_cast<CSysWin*>(&self->mSystemWindow[0]), 0);
                func_8022B9B4(&self->mSystemWindow[0], text, 0);
                func_8022BFC8(reinterpret_cast<CSysWin*>(&self->mSystemWindow[0]), 1);
                func_8022B8B8(&self->mSystemWindow[0]);
                func_801D216C(&self->mCursor, 0);
                return;
            }

            // No slot window open: pick the first filled rank slot.
            if (CSysWin_getUnk34(&self->mSystemWindow[0]) != 0) {
                return;
            }
            u8 sel = 0;
            u32 found = 0;
            for (u8 i = 0; i < 12; ++i) {
                if (self->mRankSlotIds[i] != 0) {
                    sel = i;
                    if (self->mRankSlotIds[8] != 0) {
                        sel = 8;
                    }
                    found = 1;
                    break;
                }
            }
            if (found == 0) {
                func_80138078__FUl(5);
                return;
            }
            self->field_20C = 1;
            f64 selF = (f64)sel;
            self->field_20D = (u8)(s32)(lbl_eu_80667E2C * selF);
            self->field_20E = (u8)(sel - (s8)self->field_20D * 4);
            nw4r::math::VEC3 pos;
            if (sel < 0xc) {
                pos = self->mRankSlotPos[sel];
            }
            reinterpret_cast<CMenuGetItemMultiCur*>(&self->mCursor)->vfn4(&pos);
            func_80138078__FUl(2);
            return;
        }
        if (CSysWin_getUnk34(&self->mSystemWindow[0]) == 0) {
            func_80138078__FUl(5);
        }
    }
}

// Confirm/close handler for the item-select state (Move state 4). Sweeps the
// visible item list and drops entries that are no longer special (or the
// single currently-selected item), then refreshes the display and repositions
// the cursor. Category/level classification shared with Init's special2 logic:
//   2-9 -> special iff func_80157CD0(cat) != 0
//   10-13 -> special iff level in [1, 99), or level 0 with func_80157CD0(cat)
__declspec(noinline) void func_801B8E2C(CMenuGetItemMulti* self) {
    if ((s8)self->mMaxVisibleItems == 4) {
        // All four panes visible: sweep every visible item, drop the ones
        // whose grade requirement is now satisfied.
        if (self->mEntryCount != 0) {
            for (u8 i = 0; i < self->mVisibleItemCount; ++i) {
                CMenuGetItemMultiEntry* entry = self->mVisibleEntries[i];
                if (entry == 0) {
                    continue;
                }
                if (self->mPaneVisible[i] != 0) {
                    continue;
                }
                u32 packed = entry->packed;
                u32 id = packed >> 20;
                // Category nibble falls back to the table lookup when 0;
                // retail masks the merged value to 16 bits before use.
                u32 rawCat = (packed >> 12) & 0xF;
                if (rawCat == 0) {
                    rawCat = (u16)func_801392E4(id);
                }
                u32 cat = rawCat & 0xFFFF;
                func_80139358(id);
                int special = 0;
                if (cat - 2 <= 7u) {
                    if (func_80157CD0(cat) != 0) {
                        special = 1;
                    }
                } else if (cat - 10 <= 3u) {
                    int lvl = func_80158068(id);
                    if (lvl < 1) {
                        if (func_80157CD0(cat) != 0) {
                            special = 1;
                        }
                    } else if (lvl < 0x63) {
                        special = 1;
                    }
                }
                if (special != 0) {
                    func_801599D4(entry, 0);
                    self->mVisibleEntries[i] = 0;
                    func_80140E00(2, entry->packed >> 20, 0);
                }
            }
        } else {
            for (u8 i = 0; i < self->mVisibleItemCount; ++i) {
                u16 id = self->mVisibleItemIds[i];
                if (id == 0) {
                    continue;
                }
                if (self->mPaneVisible[i] != 0) {
                    continue;
                }
                u32 cat = (u16)func_801392E4(id);
                func_80139358(id);
                int special = 0;
                if (cat - 2 <= 7u) {
                    if (func_80157CD0(cat) != 0) {
                        special = 1;
                    }
                } else if (cat - 10 <= 3u) {
                    int lvl = func_80158068(id);
                    if (lvl < 1) {
                        if (func_80157CD0(cat) != 0) {
                            special = 1;
                        }
                    } else if (lvl < 0x63) {
                        special = 1;
                    }
                }
                if (special != 0) {
                    func_801586D4(id, 1);
                    self->mVisibleItemIds[i] = 0;
                    func_80140E00(2, id, 0);
                }
            }
        }

        // Refresh the display and reposition the cursor on the active item.
        func_801B4830(self);
        {
            char buf[32];
            s8 m = (s8)self->mMaxVisibleItems;
            if (m == 5) {
                sprintf(buf, &lbl_eu_80504A3C[0x16b]);
            } else if (m == 4) {
                sprintf(buf, &lbl_eu_80504A3C[0x192], m + 1);
            } else {
                sprintf(buf, &lbl_eu_80504A3C[0x3c9], m + 1);
            }
            nw4r::lyt::Pane* pane = self->mLayout->GetRootPane()->FindPaneByName(buf, true);
            nw4r::lyt::Pane* refPane = self->mLayout->GetRootPane()->FindPaneByName(
                &lbl_eu_80504A3C[0x3d6], true);
            nw4r::math::VEC3 pos;
            func_80137924(&pos, pane, refPane, self->mLayout->GetRootPane());
            reinterpret_cast<CMenuGetItemMultiCur*>(&self->mCursor)->vfn4(&pos);
        }
        if (self->mVisibleItemCount == 0) {
            func_801D216C(&self->mCursor, 0);
            self->field_1F8 = 3;
            func_80138078__FUl(0x89);
        } else {
            self->field_1F8 = 2;
        }
        return;
    }

    // Single-item path: the selected index is the sign-extended max-visible
    // count (re-read from the field; no calls intervene before these uses so
    // MWCC keeps it in a volatile register).
    if (self->mEntryCount != 0) {
        CMenuGetItemMultiEntry* entry = self->mVisibleEntries[(s8)self->mMaxVisibleItems];
        if (entry == 0) {
            return;
        }
        if (self->mPaneVisible[(s8)self->mMaxVisibleItems] == 0) {
            func_801599D4(entry, 0);
            self->mVisibleEntries[(s8)self->mMaxVisibleItems] = 0;
            func_80140E00(2, entry->packed >> 20, 0);
            func_801B4830(self);
            {
                char buf[32];
                s8 m = (s8)self->mMaxVisibleItems;
                if (m == 5) {
                    sprintf(buf, &lbl_eu_80504A3C[0x16b]);
                } else if (m == 4) {
                    sprintf(buf, &lbl_eu_80504A3C[0x192], m + 1);
                } else {
                    sprintf(buf, &lbl_eu_80504A3C[0x3c9], m + 1);
                }
                nw4r::lyt::Pane* pane = self->mLayout->GetRootPane()->FindPaneByName(buf, true);
                nw4r::lyt::Pane* refPane = self->mLayout->GetRootPane()->FindPaneByName(
                    &lbl_eu_80504A3C[0x3d6], true);
                nw4r::math::VEC3 pos;
                func_80137924(&pos, pane, refPane, self->mLayout->GetRootPane());
                reinterpret_cast<CMenuGetItemMultiCur*>(&self->mCursor)->vfn4(&pos);
            }
            if (self->mVisibleItemCount == 0) {
                func_801D216C(&self->mCursor, 0);
                self->field_1F8 = 3;
                func_80138078__FUl(0x89);
            } else {
                self->field_1F8 = 2;
            }
            return;
        }
        // Selected item's pane is visible: grade-up handling. Rank categories
        // (10-13) below level 99 open the 5-window grade setup; at 99+ they
        // switch to the 3-window rank-max form. Other categories open the
        // same 5-window setup.
        u32 packedSel = entry->packed;
        u32 cat = (packedSel >> 12) & 0xF;
        if ((u8)(cat - 10) <= 3u) {
            if (func_80158068(packedSel >> 20) >= 0x63) {
                self->field_1F8 = 2;
                char* name = func_80136190(&lbl_eu_80504A3C[0x177],
                                           &lbl_eu_80504A3C[0x182], 3);
                func_8022B90C(reinterpret_cast<CSysWin*>(&self->mSystemWindow[0]), 0);
                func_8022B9B4(&self->mSystemWindow[0], name, 0);
                func_8022BFC8(reinterpret_cast<CSysWin*>(&self->mSystemWindow[0]), 1);
                func_8022B8B8(&self->mSystemWindow[0]);
                func_801D216C(&self->mCursor, 0);
            } else {
                self->field_1F8 = 5;
                char* n2 = func_80136190(&lbl_eu_80504A3C[0x177],
                                         &lbl_eu_80504A3C[0x182], 2);
                char* n4 = func_80136190(&lbl_eu_80504A3C[0x177],
                                         &lbl_eu_80504A3C[0x182], 4);
                char* n5 = func_80136190(&lbl_eu_80504A3C[0x177],
                                         &lbl_eu_80504A3C[0x182], 5);
                func_8022B90C(reinterpret_cast<CSysWin*>(&self->mSystemWindow[0]), 2);
                func_8022B9B4(&self->mSystemWindow[0], n2, 0);
                func_8022BF6C(&self->mSystemWindow[0], n4, n5);
                func_8022BFC8(reinterpret_cast<CSysWin*>(&self->mSystemWindow[0]), 0);
                func_8022B8B8(&self->mSystemWindow[0]);
                func_801D216C(&self->mCursor, 0);
                self->field_1F4 = 1;
            }
        } else {
            self->field_1F8 = 5;
            char* n2 = func_80136190(&lbl_eu_80504A3C[0x177],
                                     &lbl_eu_80504A3C[0x182], 2);
            char* n4 = func_80136190(&lbl_eu_80504A3C[0x177],
                                     &lbl_eu_80504A3C[0x182], 4);
            char* n5 = func_80136190(&lbl_eu_80504A3C[0x177],
                                     &lbl_eu_80504A3C[0x182], 5);
            func_8022B90C(reinterpret_cast<CSysWin*>(&self->mSystemWindow[0]), 2);
            func_8022B9B4(&self->mSystemWindow[0], n2, 0);
            func_8022BF6C(&self->mSystemWindow[0], n4, n5);
            func_8022BFC8(reinterpret_cast<CSysWin*>(&self->mSystemWindow[0]), 0);
            func_8022B8B8(&self->mSystemWindow[0]);
            func_801D216C(&self->mCursor, 0);
            self->field_1F4 = 1;
        }
        return;
    }

    u16 id = self->mVisibleItemIds[(s8)self->mMaxVisibleItems];
    if (id == 0) {
        return;
    }
    if (self->mPaneVisible[(s8)self->mMaxVisibleItems] == 0) {
        func_801586D4(id, 1);
        self->mVisibleItemIds[(s8)self->mMaxVisibleItems] = 0;
        func_80140E00(2, id, 0);
        func_801B4830(self);
        {
            char buf[32];
            s8 m = (s8)self->mMaxVisibleItems;
            if (m == 5) {
                sprintf(buf, &lbl_eu_80504A3C[0x16b]);
            } else if (m == 4) {
                sprintf(buf, &lbl_eu_80504A3C[0x192], m + 1);
            } else {
                sprintf(buf, &lbl_eu_80504A3C[0x3c9], m + 1);
            }
            nw4r::lyt::Pane* pane = self->mLayout->GetRootPane()->FindPaneByName(buf, true);
            nw4r::lyt::Pane* refPane = self->mLayout->GetRootPane()->FindPaneByName(
                &lbl_eu_80504A3C[0x3d6], true);
            nw4r::math::VEC3 pos;
            func_80137924(&pos, pane, refPane, self->mLayout->GetRootPane());
            reinterpret_cast<CMenuGetItemMultiCur*>(&self->mCursor)->vfn4(&pos);
        }
        if (self->mVisibleItemCount == 0) {
            func_801D216C(&self->mCursor, 0);
            self->field_1F8 = 3;
            func_80138078__FUl(0x89);
        } else {
            self->field_1F8 = 2;
        }
        return;
    }
    u32 cat = func_801392E4(id) & 0xFF;
    if ((u8)(cat - 10) <= 3u) {
        if (func_80158068(id) >= 0x63) {
            self->field_1F8 = 2;
            char* name = func_80136190(&lbl_eu_80504A3C[0x177],
                                       &lbl_eu_80504A3C[0x182], 3);
            func_8022B90C(reinterpret_cast<CSysWin*>(&self->mSystemWindow[0]), 0);
            func_8022B9B4(&self->mSystemWindow[0], name, 0);
            func_8022BFC8(reinterpret_cast<CSysWin*>(&self->mSystemWindow[0]), 1);
            func_8022B8B8(&self->mSystemWindow[0]);
            func_801D216C(&self->mCursor, 0);
        } else {
            self->field_1F8 = 5;
            char* n2 = func_80136190(&lbl_eu_80504A3C[0x177],
                                     &lbl_eu_80504A3C[0x182], 2);
            char* n4 = func_80136190(&lbl_eu_80504A3C[0x177],
                                     &lbl_eu_80504A3C[0x182], 4);
            char* n5 = func_80136190(&lbl_eu_80504A3C[0x177],
                                     &lbl_eu_80504A3C[0x182], 5);
            func_8022B90C(reinterpret_cast<CSysWin*>(&self->mSystemWindow[0]), 2);
            func_8022B9B4(&self->mSystemWindow[0], n2, 0);
            func_8022BF6C(&self->mSystemWindow[0], n4, n5);
            func_8022BFC8(reinterpret_cast<CSysWin*>(&self->mSystemWindow[0]), 0);
            func_8022B8B8(&self->mSystemWindow[0]);
            func_801D216C(&self->mCursor, 0);
            self->field_1F4 = 1;
        }
    } else {
        self->field_1F8 = 5;
        char* n2 = func_80136190(&lbl_eu_80504A3C[0x177],
                                 &lbl_eu_80504A3C[0x182], 2);
        char* n4 = func_80136190(&lbl_eu_80504A3C[0x177],
                                 &lbl_eu_80504A3C[0x182], 4);
        char* n5 = func_80136190(&lbl_eu_80504A3C[0x177],
                                 &lbl_eu_80504A3C[0x182], 5);
        func_8022B90C(reinterpret_cast<CSysWin*>(&self->mSystemWindow[0]), 2);
        func_8022B9B4(&self->mSystemWindow[0], n2, 0);
        func_8022BF6C(&self->mSystemWindow[0], n4, n5);
        func_8022BFC8(reinterpret_cast<CSysWin*>(&self->mSystemWindow[0]), 0);
        func_8022B8B8(&self->mSystemWindow[0]);
        func_801D216C(&self->mCursor, 0);
        self->field_1F4 = 1;
    }
}

// Pad-input handler for the get-item-multi window. Up/down press events move
// the selection and play a sound; the held-state branches auto-repeat the
// selection while the cursor tracks a window slot. The pad bit layout
// depends on whether a Classic controller is attached.
void func_801B9C1C(CMenuGetItemMulti* self) {
    cf::CfPadData* pad = cf::CfGameManager::getCfPadData();

    u32 up, down, curHeld, maskHeld;
    if (func_80086F9C__Q22cf13CfGameManagerFv(-1) != 0) {
        maskHeld = (pad->mTurboPressButtonFlags & 0x8004) != 0;
        curHeld = ((pad->mTurboPressButtonFlags & 0x10000) |
                   (pad->mTurboPressButtonFlags & 0x8)) != 0;
        up = (pad->mPad.mPressedButtonFlags >> 21) & 1;
        down = (pad->mPad.mPressedButtonFlags >> 22) & 1;
    } else {
        maskHeld = (pad->mTurboPressButtonFlags & 0x8004) != 0;
        curHeld = ((pad->mTurboPressButtonFlags & 0x10000) |
                   (pad->mTurboPressButtonFlags & 0x8)) != 0;
        up = (pad->mPad.mPressedButtonFlags >> 4) & 1;
        down = (pad->mPad.mPressedButtonFlags >> 5) & 1;
    }

    if (up) {
        self->field_1F8 = 0xa;
        func_8022B8E4(&self->mSystemWindow[0]);
        func_801D216C(&self->mCursor, 0);
        char buf[32];
        s8 n = (s8)self->mMaxVisibleItems;
        if (n == 5) {
            sprintf(buf, &lbl_eu_80504A3C[0x16b]);
        } else if (n == 4) {
            sprintf(buf, &lbl_eu_80504A3C[0x192], n + 1);
        } else {
            sprintf(buf, &lbl_eu_80504A3C[0x3c9], n + 1);
        }
        nw4r::lyt::Pane* pane = self->mLayout->GetRootPane()->FindPaneByName(buf, true);
        nw4r::lyt::Pane* refPane = self->mLayout->GetRootPane()->FindPaneByName(
            &lbl_eu_80504A3C[0x3d6], true);
        nw4r::math::VEC3 out;
        func_80137924(&out, pane, refPane, self->mLayout->GetRootPane());
        reinterpret_cast<CMenuGetItemMultiCur*>(&self->mCursor)->vfn4(&out);
        func_80138078__FUl(3);
    } else if (down) {
        self->field_1F8 = 0xa;
        func_8022B8E4(&self->mSystemWindow[0]);
        func_801D216C(&self->mCursor, 0);
        char buf[32];
        s8 n = (s8)self->mMaxVisibleItems;
        if (n == 5) {
            sprintf(buf, &lbl_eu_80504A3C[0x16b]);
        } else if (n == 4) {
            sprintf(buf, &lbl_eu_80504A3C[0x192], n + 1);
        } else {
            sprintf(buf, &lbl_eu_80504A3C[0x3c9], n + 1);
        }
        nw4r::lyt::Pane* pane = self->mLayout->GetRootPane()->FindPaneByName(buf, true);
        nw4r::lyt::Pane* refPane = self->mLayout->GetRootPane()->FindPaneByName(
            &lbl_eu_80504A3C[0x3d6], true);
        nw4r::math::VEC3 out;
        func_80137924(&out, pane, refPane, self->mLayout->GetRootPane());
        reinterpret_cast<CMenuGetItemMultiCur*>(&self->mCursor)->vfn4(&out);
        self->field_1F4 = 1;
        func_80138078__FUl(6);
    } else if (maskHeld) {
        self->field_1F4 = self->field_1F4 - 1;
        if ((s8)self->field_1F4 < 0) {
            self->field_1F4 = 1;
        }
        nw4r::math::VEC3 out;
        func_8022C1B4(&out, &self->mSystemWindow[0], self->field_1F4);
        reinterpret_cast<CMenuGetItemMultiCur*>(&self->mCursor)->vfn4(&out);
        func_80138078__FUl(1);
    } else if (curHeld) {
        self->field_1F4 = self->field_1F4 + 1;
        if ((s8)self->field_1F4 > 1) {
            self->field_1F4 = 0;
        }
        nw4r::math::VEC3 out;
        func_8022C1B4(&out, &self->mSystemWindow[0], self->field_1F4);
        reinterpret_cast<CMenuGetItemMultiCur*>(&self->mCursor)->vfn4(&out);
        func_80138078__FUl(1);
    }
}

// Pad-input handler for the get-item-multi window (alternate variant). Same
// shape as func_801B9C1C; the up branch additionally flags the auto-repeat
// state and uses a different selection count.
void func_801B9864(CMenuGetItemMulti* self) {
    cf::CfPadData* pad = cf::CfGameManager::getCfPadData();

    u32 up, down, curHeld, maskHeld;
    if (func_80086F9C__Q22cf13CfGameManagerFv(-1) != 0) {
        maskHeld = (pad->mTurboPressButtonFlags & 0x8004) != 0;
        curHeld = ((pad->mTurboPressButtonFlags & 0x10000) |
                   (pad->mTurboPressButtonFlags & 0x8)) != 0;
        up = (pad->mPad.mPressedButtonFlags >> 21) & 1;
        down = (pad->mPad.mPressedButtonFlags >> 22) & 1;
    } else {
        maskHeld = (pad->mTurboPressButtonFlags & 0x8004) != 0;
        curHeld = ((pad->mTurboPressButtonFlags & 0x10000) |
                   (pad->mTurboPressButtonFlags & 0x8)) != 0;
        up = (pad->mPad.mPressedButtonFlags >> 4) & 1;
        down = (pad->mPad.mPressedButtonFlags >> 5) & 1;
    }

    if (up) {
        if ((s8)self->field_1F4 == 0) {
            self->field_1F5 = 1;
        }
        self->field_1F8 = 0x7;
        func_8022B8E4(&self->mSystemWindow[0]);
        func_801D216C(&self->mCursor, 0);
        char buf[32];
        s8 n = (s8)self->mMaxVisibleItems;
        if (n == 5) {
            sprintf(buf, &lbl_eu_80504A3C[0x16b]);
        } else if (n == 4) {
            sprintf(buf, &lbl_eu_80504A3C[0x192], n + 1);
        } else {
            sprintf(buf, &lbl_eu_80504A3C[0x3c9], n + 1);
        }
        nw4r::lyt::Pane* pane = self->mLayout->GetRootPane()->FindPaneByName(buf, true);
        nw4r::lyt::Pane* refPane = self->mLayout->GetRootPane()->FindPaneByName(
            &lbl_eu_80504A3C[0x3d6], true);
        nw4r::math::VEC3 out;
        func_80137924(&out, pane, refPane, self->mLayout->GetRootPane());
        reinterpret_cast<CMenuGetItemMultiCur*>(&self->mCursor)->vfn4(&out);
        func_80138078__FUl(3);
    } else if (down) {
        self->field_1F8 = 0x7;
        func_8022B8E4(&self->mSystemWindow[0]);
        func_801D216C(&self->mCursor, 0);
        char buf[32];
        s8 n = (s8)self->mMaxVisibleItems;
        if (n == 5) {
            sprintf(buf, &lbl_eu_80504A3C[0x16b]);
        } else if (n == 4) {
            sprintf(buf, &lbl_eu_80504A3C[0x192], n + 1);
        } else {
            sprintf(buf, &lbl_eu_80504A3C[0x3c9], n + 1);
        }
        nw4r::lyt::Pane* pane = self->mLayout->GetRootPane()->FindPaneByName(buf, true);
        nw4r::lyt::Pane* refPane = self->mLayout->GetRootPane()->FindPaneByName(
            &lbl_eu_80504A3C[0x3d6], true);
        nw4r::math::VEC3 out;
        func_80137924(&out, pane, refPane, self->mLayout->GetRootPane());
        reinterpret_cast<CMenuGetItemMultiCur*>(&self->mCursor)->vfn4(&out);
        func_80138078__FUl(6);
    } else if (maskHeld) {
        self->field_1F4 = self->field_1F4 - 1;
        if ((s8)self->field_1F4 < 0) {
            self->field_1F4 = 1;
        }
        nw4r::math::VEC3 out;
        func_8022C1B4(&out, &self->mSystemWindow[0], self->field_1F4);
        reinterpret_cast<CMenuGetItemMultiCur*>(&self->mCursor)->vfn4(&out);
        func_80138078__FUl(1);
    } else if (curHeld) {
        self->field_1F4 = self->field_1F4 + 1;
        if ((s8)self->field_1F4 > 1) {
            self->field_1F4 = 0;
        }
        nw4r::math::VEC3 out;
        func_8022C1B4(&out, &self->mSystemWindow[0], self->field_1F4);
        reinterpret_cast<CMenuGetItemMultiCur*>(&self->mCursor)->vfn4(&out);
        func_80138078__FUl(1);
    }
}

void OnFileEvent__17CMenuGetItemMultiFP10CEventFile(u8* self) { ((void (*)(char*))func_801B45A0)((char*)self - 0x6c); }

void func_801BA134(void* self) { ((void(*)(void*))__dt__17CMenuGetItemMultiFv)((char*)self - 0x6c); }

void func_801BA13C(void* self) { ((void(*)(void*))cbRenderBefore__17CMenuGetItemMultiFv)((char*)self - 0x70); }

void func_801BA144(void* self) { ((void(*)(void*))__dt__17CMenuGetItemMultiFv)((char*)self - 0x70); }

// --- hard-symbol stubs (scaffold_hard_symbols) ---
// Static constructor: initialise the get-item-multi pane colour/position
// defaults (.sbss). Each entry is an RGBA 16-bit quadruple; alpha stays 0.
// Statement order mirrors retail so MWCC's register allocation (shared zero
// register, long-lived lbl_eu_806643D0 pointer) reproduces byte-identical.
void sinit_801B9FC8() {
    // Pane colour/position defaults (.sbss), one RGBA quadruple per pane.
    // Grouped per symbol in ascending address order - closest shape found
    // to retail's store/register schedule (interleaved orders reschedule
    // worse; chained assignments shift but don't close the residual).
    lbl_eu_806643D0.c = 0xfa;
    lbl_eu_806643A0.d = 0;
    lbl_eu_806643A0.c = 0;
    lbl_eu_806643A0.b = 0;
    lbl_eu_806643A0.a = 0;
    lbl_eu_806643A8.d = 0;
    lbl_eu_806643A8.c = 0;
    lbl_eu_806643A8.b = 0;
    lbl_eu_806643A8.a = 0;
    lbl_eu_806643B0.d = 0;
    lbl_eu_806643B0.c = 0x80;
    lbl_eu_806643B0.b = 0x80;
    lbl_eu_806643B0.a = 0x80;
    lbl_eu_806643B8.d = 0;
    lbl_eu_806643B8.c = 0x80;
    lbl_eu_806643B8.b = 0x80;
    lbl_eu_806643B8.a = 0x80;
    lbl_eu_806643C0.d = 0;
    lbl_eu_806643C0.c = 0;
    lbl_eu_806643C0.b = 0;
    lbl_eu_806643C0.a = 0;
    lbl_eu_806643C8.d = 0;
    lbl_eu_806643C8.c = 0;
    lbl_eu_806643C8.b = 0;
    lbl_eu_806643C8.a = 0;
    lbl_eu_806643D0.d = 0;
    lbl_eu_806643D0.b = 0xff;
    lbl_eu_806643D0.a = 0xff;
    lbl_eu_806643D8.d = 0;
    lbl_eu_806643D8.c = 0x80;
    lbl_eu_806643D8.b = 0x80;
    lbl_eu_806643D8.a = 0x80;
    lbl_eu_806643E0.d = 0;
    lbl_eu_806643E0.c = 0;
    lbl_eu_806643E0.b = 0;
    lbl_eu_806643E0.a = 0;
    lbl_eu_806643E8.d = 0;
    lbl_eu_806643E8.c = 0;
    lbl_eu_806643E8.b = 0;
    lbl_eu_806643E8.a = 0;
    lbl_eu_806643F0.d = 0;
    lbl_eu_806643F0.c = 0xe7;
    lbl_eu_806643F0.b = 0xa3;
    lbl_eu_806643F0.a = 0x12;
    lbl_eu_806643F8.d = 0;
    lbl_eu_806643F8.c = 0xff;
    lbl_eu_806643F8.b = 0xff;
    lbl_eu_806643F8.a = 0xff;
    lbl_eu_80664400.d = 0;
    lbl_eu_80664400.c = 0xc0;
    lbl_eu_80664400.b = 0x9;
    lbl_eu_80664400.a = 0xfa;
    lbl_eu_80664408.d = 0;
    lbl_eu_80664408.c = 0xff;
    lbl_eu_80664408.b = 0xff;
    lbl_eu_80664408.a = 0xff;
}
