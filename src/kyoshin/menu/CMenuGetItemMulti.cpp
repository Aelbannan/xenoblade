// Auto-scaffolded catalog TU for kyoshin/menu/CMenuGetItemMulti
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/menu/CMenuGetItemMulti.hpp"

#include "kyoshin/code_80135FDC.hpp"
#include "kyoshin/cf/CfGameManager.hpp"
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

class CMenuGetItemImpl {
public:
    virtual u16 getRankCount(CMenuGetItemMultiEntry*) = 0;
    virtual void v0C() = 0;
    virtual void v10() = 0;
    virtual void v14() = 0;
    virtual void v18() = 0;
    virtual void v1C() = 0;
    virtual char* getName(CMenuGetItemMultiEntry*) = 0;
    virtual void v24() = 0;
    virtual void v28() = 0;
    virtual void* getSlot(CMenuGetItemMultiEntry*, u8) = 0;
    virtual u8 hasSlot(CMenuGetItemMultiEntry*) = 0;
    virtual void v34() = 0;
    virtual void v38() = 0;
    virtual void v3C() = 0;
    virtual s16 getSlotId(CMenuGetItemMultiEntry*, u8) = 0;
};

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
extern f64 lbl_eu_80667E08;
extern u32 lbl_eu_806640EC;
extern u32 lbl_eu_80664108;
extern u32 lbl_eu_80664410;

void code80135FDC_postIncByte_64080();
nw4r::lyt::ArcResourceAccessor* func_801355F4();
u32 func_801355BC();
char* func_80138F78(u32);
CMenuGetItemFourShorts func_801397AC(void*, u32);
CMenuGetItemImpl* CItem_initItemImplInstances(CMenuGetItemMultiEntry* entry);
u16 func_80139358(u16);
char* func_801393CC(u16);
char* func_801394D4(u16);
u8 func_80157CD0(u16);
// Retail call sites consume the +6 slot-sum as a raw byte (mr r5, r3, no
// clrlwi), so the call-site type is u8 despite the CItem.cpp def using u32;
// the arg is passed raw (mr r3, rN) so the visible param is u32.
u8 func_80158068(u32);
// Retail passes the raw id to func_801392E4 without a halfword mask (mr r3,
// r4 directly), so the visible param is u32 here.
u32 func_801392E4(u32);
char* func_eu_802B1474();
char* func_eu_802B148C();
u16 func_80136254(const char*, const char*, u16);
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
void __dt__8CProcessFv(CProcess*, int);
u8* __ct__801B2794(u8*, u32, u32);
}

extern "C" int func_80086F9C__Q22cf13CfGameManagerFv(int);
extern "C" void func_8008294C__Q22cf13CfGameManagerFv(int);

// Named .sdata2 conversion magic: defining it lets MWCC's constant pool reuse
// the retail symbol for the (f32)(s32) casts in func_801B5630 instead of
// emitting a TU-local @N label (CMiniMap idiom).
extern const f64 lbl_eu_80667E18 = 0x4330000080000000ll;

// Pane layout adjustment for the visible-item count (defined below; called by Init).
void func_801B5630(CMenuGetItemMulti* self);


extern "C" void __dt__17CMenuGetItemMultiFv(void*, int);
extern "C" void cbRenderBefore__17CMenuGetItemMultiFv(void*);

void __ct__CMenuGetItemMulti(){}

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
        CMenuGetItemMultiEntry* entry = &mEntries[0];
        while (entry != &mEntries[mEntryCount]) {
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
    }
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

void CMenuGetItemMulti::Move() {}

// Adjust the item-multi pane layout for the visible-item count: each hidden
// item (4 - mVisibleItemCount, clamped at 0) shrinks the item-text pane
// height and shifts the close/other panes down by lbl_eu_80667E14 per step.
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
        pos.y += (f32)count * lbl_eu_80667E14;
        pane->SetTranslate(pos);
    }
    pane = self->mLayout->GetRootPane()->FindPaneByName(
        &lbl_eu_80504A3C[0x16b], true);
    if (pane != 0) {
        nw4r::math::VEC3 pos = pane->GetTranslate();
        pos.y += (f32)count * lbl_eu_80667E14;
        pane->SetTranslate(pos);
    }
    pane = self->mLayout->GetRootPane()->FindPaneByName(
        &lbl_eu_80504A3C[0x3f8], true);
    if (pane != 0) {
        nw4r::math::VEC3 pos = pane->GetTranslate();
        pos.y += (f32)count * lbl_eu_80667E14;
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

void func_801B4830(){}

// noinline: the retail keeps real bl relocs to these dispatch helpers; empty
// stub bodies must not be inlined away at call sites.
__declspec(noinline) void func_801B59F4(CMenuGetItemMulti* self) {}

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
        CMenuGetItemPaneView* view = reinterpret_cast<CMenuGetItemPaneView*>(pane);
        view->flags = (view->flags & 0xfe) | (arg2 != 0);
    }
    if (arg2 != 0) {
        // Inline ternary: branches compute into the arg register (r3), the
        // u16 assignment masks once at the join (retail clrlwi r3, r3, 16).
        u16 id = (arg3 != 0) ? (arg3->packed >> 20) : (u32)arg2;
        func_80136910(self->mLayout, &lbl_eu_80504A3C[0xeb], func_80158068(id));
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

__declspec(noinline) void func_801B6184(CMenuGetItemMulti* self, int arg2, CMenuGetItemMultiEntry* arg3) {}

__declspec(noinline) void func_801B69F4(CMenuGetItemMulti* self, int arg2, CMenuGetItemMultiEntry* arg3) {}

__declspec(noinline) void func_801B70BC(CMenuGetItemMulti* self, int arg2, CMenuGetItemMultiEntry* arg3) {}

__declspec(noinline) void func_801B7440(CMenuGetItemMulti* self, CMenuGetItemMultiEntry* arg3) {}

__declspec(noinline) void func_801B76CC(CMenuGetItemMulti* self, int arg2) {}

__declspec(noinline) void func_801B78B4(CMenuGetItemMulti* self, int arg2) {}

void func_801B7A58(){}

void func_801B82E8(){}

void func_801B8E2C(){}

void func_801B9864(){}

void func_801B9C1C(){}

void OnFileEvent__17CMenuGetItemMultiFP10CEventFile(u8* self) { ((void (*)(char*))func_801B45A0)((char*)self - 0x6c); }

void func_801BA134(void* self) { ((void(*)(void*))__dt__17CMenuGetItemMultiFv)((char*)self - 0x6c); }

void func_801BA13C(void* self) { ((void(*)(void*))cbRenderBefore__17CMenuGetItemMultiFv)((char*)self - 0x70); }

void func_801BA144(void* self) { ((void(*)(void*))__dt__17CMenuGetItemMultiFv)((char*)self - 0x70); }

// --- hard-symbol stubs (scaffold_hard_symbols) ---
// Static constructor: initialise the get-item-multi pane colour/position
// defaults (.sbss). Each entry is an RGBA 16-bit quadruple; alpha stays 0.
void sinit_801B9FC8() {
    lbl_eu_806643A0.a = 0;
    lbl_eu_806643A0.b = 0;
    lbl_eu_806643A0.c = 0;
    lbl_eu_806643A0.d = 0;
    lbl_eu_806643A8.a = 0;
    lbl_eu_806643A8.b = 0;
    lbl_eu_806643A8.c = 0;
    lbl_eu_806643A8.d = 0;
    lbl_eu_806643B0.a = 0x80;
    lbl_eu_806643B0.b = 0x80;
    lbl_eu_806643B0.c = 0;
    lbl_eu_806643B0.d = 0;
    lbl_eu_806643B8.a = 0x80;
    lbl_eu_806643B8.b = 0x80;
    lbl_eu_806643B8.c = 0;
    lbl_eu_806643B8.d = 0;
    lbl_eu_806643C0.a = 0;
    lbl_eu_806643C0.b = 0;
    lbl_eu_806643C0.c = 0;
    lbl_eu_806643C0.d = 0;
    lbl_eu_806643C8.a = 0;
    lbl_eu_806643C8.b = 0;
    lbl_eu_806643C8.c = 0;
    lbl_eu_806643C8.d = 0;
    lbl_eu_806643D0.a = 0xff;
    lbl_eu_806643D0.b = 0xff;
    lbl_eu_806643D0.c = 0xfa;
    lbl_eu_806643D0.d = 0;
    lbl_eu_806643D8.a = 0x80;
    lbl_eu_806643D8.b = 0x80;
    lbl_eu_806643D8.c = 0x80;
    lbl_eu_806643D8.d = 0;
    lbl_eu_806643E0.a = 0;
    lbl_eu_806643E0.b = 0;
    lbl_eu_806643E0.c = 0;
    lbl_eu_806643E0.d = 0;
    lbl_eu_806643E8.a = 0;
    lbl_eu_806643E8.b = 0;
    lbl_eu_806643E8.c = 0;
    lbl_eu_806643E8.d = 0;
    lbl_eu_806643F0.a = 0x12;
    lbl_eu_806643F0.b = 0xa3;
    lbl_eu_806643F0.c = 0xe7;
    lbl_eu_806643F0.d = 0;
    lbl_eu_806643F8.a = 0xff;
    lbl_eu_806643F8.b = 0xff;
    lbl_eu_806643F8.c = 0xff;
    lbl_eu_806643F8.d = 0;
    lbl_eu_80664400.a = 0xb3;
    lbl_eu_80664400.b = 0x9;
    lbl_eu_80664400.c = 0xc0;
    lbl_eu_80664400.d = 0;
    lbl_eu_80664408.a = 0xff;
    lbl_eu_80664408.b = 0xff;
    lbl_eu_80664408.c = 0xff;
    lbl_eu_80664408.d = 0;
}
