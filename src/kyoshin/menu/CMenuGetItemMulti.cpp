// Auto-scaffolded catalog TU for kyoshin/menu/CMenuGetItemMulti
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/menu/CMenuGetItemMulti.hpp"

#include "kyoshin/code_80135FDC.hpp"
#include "kyoshin/cf/CfGameManager.hpp"
#include "monolib/device/CDeviceFile.hpp"
#include "monolib/lib/UnkClass_8045F564.hpp"
#include "monolib/scn/CScn.hpp"
#include "monolib/scn/IScnRender.hpp"
#include "monolib/util/MemManager.hpp"
#include "monolib/util/FixStr.hpp"
#include "monolib/work/CWorkThreadSystem.hpp"

#include <nw4r/lyt.h>
#include <stdio.h>

struct CMenuGetItemFourShorts {
    s16 a;
    s16 b;
    s16 c;
    s16 d;
};

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
extern CMenuGetItemFourShorts lbl_eu_806643A0;
extern CMenuGetItemFourShorts lbl_eu_806643A8;
extern CMenuGetItemFourShorts lbl_eu_806643B0;
extern CMenuGetItemFourShorts lbl_eu_806643B8;
extern CMenuGetItemFourShorts lbl_eu_806643C0;
extern CMenuGetItemFourShorts lbl_eu_806643C8;
extern CMenuGetItemFourShorts lbl_eu_806643D0;
extern CMenuGetItemFourShorts lbl_eu_806643D8;
extern CMenuGetItemFourShorts lbl_eu_806643E0;
extern CMenuGetItemFourShorts lbl_eu_806643E8;
extern CMenuGetItemFourShorts lbl_eu_806643F0;
extern CMenuGetItemFourShorts lbl_eu_806643F8;
extern CMenuGetItemFourShorts lbl_eu_80664400;
extern CMenuGetItemFourShorts lbl_eu_80664408;

void code80135FDC_postIncByte_64080();
nw4r::lyt::ArcResourceAccessor* func_801355F4();
u32 func_801355BC();
char* func_80138F78(u32);
CMenuGetItemFourShorts func_801397AC(void*, u32);
CMenuGetItemImpl* CItem_initItemImplInstances(CMenuGetItemMultiEntry* entry);
u16 func_801392E4(u16);
u16 func_80139358(u16);
char* func_801393CC(u16);
char* func_801394D4(u16);
u8 func_80157CD0(u16);
int func_80158068(u16);
char* func_eu_802B1474();
char* func_eu_802B148C();
u16 func_80136254(const char*, const char*, u16);
void func_80137B44(nw4r::lyt::Layout*, char*, u32);
void* func_80452C10__11CDeviceFontFUlPQ34nw4r3lyt6Layout(u32, nw4r::lyt::Layout*);
CBaseCur* __ct__CCur18(CBaseCur*, nw4r::lyt::ArcResourceAccessor*);
void* __dt__6CCur18Fv(CBaseCur*, int);
CSysWin* __ct__CSysWin(CSysWin*, int);
CSysWin* __dt__7CSysWinFv(CSysWin*, int);
void func_801B5630(CMenuGetItemMulti*);
void func_801B5860(CMenuGetItemMulti*, int, int);
}

extern "C" int func_80086F9C__Q22cf13CfGameManagerFv(int);
extern "C" void func_8008294C__Q22cf13CfGameManagerFv(int);

static bool CMenuGetItemMultiUsesSlot(u8 category) {
    return category == 2 || (category >= 4 && category <= 8);
}

static bool CMenuGetItemMultiIsSpecial(u8 category, u16 itemId) {
    if (category == 3 || category == 9) {
        return true;
    }
    if (func_801361E8(lbl_eu_806640EC, &lbl_eu_80504A3C[0x394], itemId) != 0) {
        return true;
    }
    return category == 12;
}

static bool CMenuGetItemMultiIsHidden(u16 category, u16 itemId) {
    if (category >= 2 && category <= 9) {
        return func_80157CD0(category) == 0;
    }
    if (category >= 10 && category <= 13) {
        int y = func_80158068(itemId);
        if (y < 1) {
            return func_80157CD0(category) == 0;
        }
        return y >= 0x63;
    }
    return false;
}

extern "C" void __dt__17CMenuGetItemMultiFv(void*, int);
extern "C" void cbRenderBefore__17CMenuGetItemMultiFv(void*);

void __ct__CMenuGetItemMulti(){}

void __dt__801B2754(){}

void __ct__801B2794(){}

CMenuGetItemMulti::~CMenuGetItemMulti() {}

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
                case 11:
                    textureName = &lbl_eu_80504A3C[0x355];
                    if (func_801361E8(lbl_eu_80664108, &lbl_eu_80504A3C[0x33b],
                                     func_80139358((u16)(entry->packed >> 20)))) {
                        textureName = &lbl_eu_80504A3C[0x340];
                    }
                    break;
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

                if (CMenuGetItemMultiUsesSlot((entry->packed >> 16) & 0xf)) {
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

                if (CMenuGetItemMultiIsSpecial((u8)((entry->packed >> 16) & 0xf),
                                               (u16)(entry->packed >> 20))) {
                    mHasSpecialItem = 1;
                }
                u16 cat2 = (entry->packed >> 16) & 0xf;
                if (cat2 == 0) {
                    cat2 = func_801392E4((u16)(entry->packed >> 20));
                }
                func_80139358((u16)(entry->packed >> 20));
                if (!CMenuGetItemMultiIsHidden(cat2, (u16)(entry->packed >> 20))) {
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
                if (CMenuGetItemMultiIsSpecial(category, itemId)) {
                    mHasSpecialItem = 1;
                }
                u16 cat2 = func_801392E4(itemId);
                func_80139358(itemId);
                if (!CMenuGetItemMultiIsHidden(cat2, itemId)) {
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
            case 11:
                textureName = &lbl_eu_80504A3C[0x355];
                if (func_801361E8(lbl_eu_80664108, &lbl_eu_80504A3C[0x33b], tableId)) {
                    textureName = &lbl_eu_80504A3C[0x340];
                }
                break;
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

            if (CMenuGetItemMultiUsesSlot(category)) {
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
        CMenuGetItemPaneView* pane = reinterpret_cast<CMenuGetItemPaneView*>(
            mLayout->GetRootPane()->FindPaneByName(fullPaneName, true));
        pane->flags = (pane->flags & 0xfe) | mPaneVisible[i];
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
    CSysWin* systemWindowTemp = __ct__CSysWin(
        reinterpret_cast<CSysWin*>(&systemWindowStorage[0]), 2);
    mSystemWindow.mMemRegion.unk0 = systemWindowTemp->mMemRegion.unk0;
    mSystemWindow.mMemRegion.unk4 = systemWindowTemp->mMemRegion.unk4;
    mSystemWindow.mMemRegion.unk8 = systemWindowTemp->mMemRegion.unk8;
    mSystemWindow.mMemRegion.unkC = systemWindowTemp->mMemRegion.unkC;
    mSystemWindow.mFileHandle = systemWindowTemp->mFileHandle;
    mSystemWindow.mTagProcessor = systemWindowTemp->mTagProcessor;
    mSystemWindow.mArcAccessor = systemWindowTemp->mArcAccessor;
    mSystemWindow.mLayout = systemWindowTemp->mLayout;
    mSystemWindow.mAnimTrans = systemWindowTemp->mAnimTrans;
    mSystemWindow.field_28 = systemWindowTemp->field_28;
    mSystemWindow.field_2C = systemWindowTemp->field_2C;
    mSystemWindow.field_30 = systemWindowTemp->field_30;
    mSystemWindow.field_34 = systemWindowTemp->field_34;
    mSystemWindow.field_35 = systemWindowTemp->field_35;
    mSystemWindow.field_36 = systemWindowTemp->field_36;
    mSystemWindow.field_37 = systemWindowTemp->field_37;
    mSystemWindow.field_38 = systemWindowTemp->field_38;
    mSystemWindow.field_39 = systemWindowTemp->field_39;
    __dt__7CSysWinFv(systemWindowTemp, -1);
    ((void (*)(CSysWin*))reinterpret_cast<void**>(mSystemWindow.mVtbl)[34])(&mSystemWindow);

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

void CMenuGetItemMulti::Term() {}

void CMenuGetItemMulti::Move() {}

void func_801B45A0(void* self){}

void CMenuGetItemMulti::cbRenderBefore() {}

void func_801B46E4(){}

void func_801B4790(){}

extern unsigned long lbl_eu_80664414;
extern "C" unsigned long func_801B481C() { return lbl_eu_80664414 != 0; }

void func_801B4830(){}

extern "C" void func_801B5630(CMenuGetItemMulti*){}

extern "C" void func_801B5860(CMenuGetItemMulti*, int, int){}

void func_801B59F4(){}

void func_801B6184(){}

void func_801B69F4(){}

void func_801B70BC(){}

void func_801B7440(){}

void func_801B76CC(){}

void func_801B78B4(){}

void func_801B7A58(){}

void func_801B82E8(){}

void func_801B8E2C(){}

void func_801B9864(){}

void func_801B9C1C(){}

extern "C" void OnFileEvent__17CMenuGetItemMultiFP10CEventFile(void* self) { func_801B45A0((char*)self - 0x6c); }

void func_801BA134(void* self) { ((void(*)(void*))__dt__17CMenuGetItemMultiFv)((char*)self - 0x6c); }

void func_801BA13C(void* self) { ((void(*)(void*))cbRenderBefore__17CMenuGetItemMultiFv)((char*)self - 0x70); }

void func_801BA144(void* self) { ((void(*)(void*))__dt__17CMenuGetItemMultiFv)((char*)self - 0x70); }

// --- hard-symbol stubs (scaffold_hard_symbols) ---
void sinit_801B9FC8(){}
