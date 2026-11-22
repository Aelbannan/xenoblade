// Auto-scaffolded catalog TU for kyoshin/menu/CMenuGetItemMulti
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/menu/CMenuGetItemMulti.hpp"
#include "kyoshin/cf/CfBdat.hpp"

#include "kyoshin/code_80135FDC_Bdat.hpp"
#include "kyoshin/CItemBoxInfo.hpp"
#include "kyoshin/code_80135FDC_State.hpp"
#include "kyoshin/CUICfManager_Res.hpp"

// code_80135FDC.hpp declares ml::epsilon as u32, conflicting with the
// const-float ml::math epsilon copy in monolib FloatUtils.hpp (pulled in via
// the CfPadData.hpp include chain). Rename it away; this TU never uses it.
#include "kyoshin/code_80135FDC.hpp"
// The battle-manager singleton getter has ONE canonical declaration in
// kyoshin/cf/CBattleManager.hpp; no rename guard is needed anymore.
#include "kyoshin/cf/CBattleManager.hpp"
#include "kyoshin/cf/CfGameManager.hpp"
#include "kyoshin/cf/CItemImpl.hpp"   // real item-impl vtable interface
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
#include "decomp.h"
#include "kyoshin/code_80135FDC_Lyt.hpp"
#include "kyoshin/plugin/ocBdat.hpp"

// Owner definitions of this unit's .sbss block (0x806643A0..0x80664418).
// Declared in CMenuGetItemMulti.hpp for other units.
GXColorS10 gItemMultiPaneColor00;
GXColorS10 gItemMultiPaneColor01;
GXColorS10 gItemMultiPaneColor02;
GXColorS10 gItemMultiPaneColor03;
GXColorS10 gItemMultiPaneColor04;
GXColorS10 gItemMultiPaneColor05;
GXColorS10 gItemMultiPaneColor06;
GXColorS10 gItemMultiPaneColor07;
GXColorS10 gItemMultiPaneColor08;
GXColorS10 gItemMultiPaneColor09;
GXColorS10 gItemMultiPaneColor0A;
GXColorS10 gItemMultiPaneColor0B;
GXColorS10 gItemMultiPaneColor0C;
GXColorS10 gItemMultiPaneColor0D;
u32 gItemMultiClosePaneWord;
u32 gpItemMultiInstance;
u32 gpItemMultiSkillBdat;



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

// Match CItem.cpp defs: bare cmpi on r3 in Init special2 (u8 view inserts clrlwi).
s32 CItemBlock_countEmpty(u32);
// Slot-sum call sites want a raw mr of r3; u32 return matches the def and
// still allows mr without an extra clrlwi when passed through.
u32 CItem_sumFamilyByte6(u32);
// setLayoutTextBoxNumber is declared by the included code_80135FDC.hpp / unit header.
// Retail passes the raw id to BdatGetItemType without a halfword mask (mr r3,
// r4 directly), so the visible param is u32 here.
char* getErrMesText15();
char* getErrMesText16();
void* getFontInfo__11CDeviceFontFUlPQ34nw4r3lyt6Layout(u32, nw4r::lyt::Layout*);
CSysWin* __ct__7CSysWinFUc(CSysWin*, int);
CSysWin* __dt__7CSysWinFv(CSysWin*, int);
void __ct__14Class_8045F858FP17UnkClass_8045F564(Class_8045F858*, UnkClass_8045F564*);
void __dt__14Class_8045F858Fv(Class_8045F858*, int);
void GetItemMulti_ClearRanks(CMenuGetItemMulti*);
void GetItemMulti_ShowDetail(CMenuGetItemMulti*, int, CItemExt*);
void GetItemMulti_ShowEquipped(CMenuGetItemMulti*, int, CItemExt*);
void GetItemMulti_ShowEntryDetailPanes(CMenuGetItemMulti*, int, CItemExt*);
void GetItemMulti_ShowItemRank(CMenuGetItemMulti*, int, CItemExt*);
void GetItemMulti_FillEntryRankPane(CMenuGetItemMulti*, CItemExt*);
void GetItemMulti_ShowIdCategoryPanes(CMenuGetItemMulti*, int);
void GetItemMulti_ShowCat13(CMenuGetItemMulti*, int);
void GetItemMulti_RefreshList(CMenuGetItemMulti*);
void GetItemMulti_FillRankSlots(CMenuGetItemMulti*, int, CItemExt*);
void GetItemMulti_HandlePad(CMenuGetItemMulti*);
void GetItemMulti_PruneEntries(CMenuGetItemMulti*);
void GetItemMulti_HandleWindowPadOnce(CMenuGetItemMulti*);
void GetItemMulti_HandleWindowPadAuto(CMenuGetItemMulti*);
void GetItemMulti_FitPanes(CMenuGetItemMulti*);
}

// setPresentationFlag: canonical setPresentationFlag__Q22cf13CfGameManagerFv(bool).
extern "C" void __dt__17CMenuGetItemMultiFv(void*, int);
extern "C" void cbRenderBefore__17CMenuGetItemMultiFP4CScn(void*);

// Constructor (retail unmangled global `__ct__CMenuGetItemMulti`, written as a
// C-ABI free function like CTaskGameEvt/CMenuMakeCrystal so no class-length
// mangling is emitted). Builds the CProcess base, installs the interim
// composite vtable, copies __ptmf_null into both callback slots (+0x3C/+0x48),
// clears the scalar block, swaps in the final composite vtable and the
// IWorkEvent/IScnRender sub-vtables (+0x24/+0xac from the same base table),
// then constructs the UnkClass_8045F564 render region (+0x78), zero-inits the
// entry array and cursor/system-window sub-objects, and arms the state bytes.
CMenuGetItemMulti::CMenuGetItemMulti(CScn* scn, u32 item0, u32 item1,
                                     u32 item2, u32 item3, u32 argF6,
                                     u32 arg201, u8 argSpecial)
    : mSystemWindow(0) {
    // Zero constant declared up front: its live range then starts at entry
    // (like MWCC's implicit member-init sequence) and wins the lowest
    // callee-saved register, matching retail r22.
    u32 zero = 0;

    // Zero constants drive most of the clear stores (retail keeps it in a
    // callee-saved reg across the UnkClass_8045F564 ctor call).

    this->mScn = scn;

    this->mAnim = reinterpret_cast<nw4r::lyt::AnimTransform*>(zero);
    this->mInitialItems[0] = item0;
    this->mInitialItems[1] = item1;
    this->mInitialItems[2] = item2;
    this->mInitialItems[3] = item3;
    this->mVisibleItemCount = zero;

    this->mItems.mCount = zero;


    // Second zero constant for the tail clear (retail reuses the freed scn
    // register slot as a fresh zero, r24).
    u32 tailZero = 0;
    this->mMaxVisibleItems = 4;
    this->mHasSpecialItem = argSpecial;
    this->field_1F4 = 1;
    this->field_1F5 = (u8)tailZero;
    this->field_1F6 = argF6;
    this->field_1F8 = tailZero;
    this->field_1FC = tailZero;
    this->field_200 = (u8)tailZero;
    this->field_201 = arg201;
    this->mFileHandle = 0;
    this->field_208 = tailZero;
    this->field_20C = (u8)tailZero;
    this->field_20D = (u8)tailZero;
    this->field_20E = (u8)tailZero;
    this->field_20F = argSpecial;
    // Retail fully unrolls this clear (interleaved stb/stw/sth per slot).
    this->mPaneVisible[0] = (u8)tailZero;
    this->mVisibleEntries[0] = reinterpret_cast<CItemExt*>(tailZero);
    this->mVisibleItemIds[0] = (u16)tailZero;
    this->mPaneVisible[1] = (u8)tailZero;
    this->mVisibleEntries[1] = reinterpret_cast<CItemExt*>(tailZero);
    this->mVisibleItemIds[1] = (u16)tailZero;
    this->mPaneVisible[2] = (u8)tailZero;
    this->mVisibleEntries[2] = reinterpret_cast<CItemExt*>(tailZero);
    this->mVisibleItemIds[2] = (u16)tailZero;
    this->mPaneVisible[3] = (u8)tailZero;
    this->mVisibleEntries[3] = reinterpret_cast<CItemExt*>(tailZero);
    this->mVisibleItemIds[3] = (u16)tailZero;
    this->mPaneVisible[4] = (u8)tailZero;
    this->mVisibleEntries[4] = reinterpret_cast<CItemExt*>(tailZero);
    this->mVisibleItemIds[4] = (u16)tailZero;
    this->mPaneVisible[5] = (u8)tailZero;
    this->mVisibleEntries[5] = reinterpret_cast<CItemExt*>(tailZero);
    this->mVisibleItemIds[5] = (u16)tailZero;
    this->mItems.mCount = tailZero;
    this->mItems.mCount = 0;
}

// Second constructor (retail __ct__801B2794): seeds the item list from a
// prepared CItemFour (copied record-by-record) instead of four item ids.
CMenuGetItemMulti::CMenuGetItemMulti(CScn* scn, const CItemFour& items)
    : mScn(scn), mAnim(0), mInitialItems(), mVisibleItemCount(0),
      mItems(items), mCursor(NULL), mSystemWindow(0) {
    mMaxVisibleItems = 4;
    mHasSpecialItem = 0;
    field_1F4 = 1;
    field_1F5 = 0;
    field_1F6 = 0;
    field_1F8 = 0;
    field_1FC = 0;
    field_200 = 0;
    field_201 = 1;
    mFileHandle = 0;
    field_208 = 0;
    field_20C = 0;
    field_20D = 0;
    field_20E = 0;
    field_20F = 0;
    for (int i = 0; i < 6; i++) {
        mPaneVisible[i] = 0;
        mVisibleEntries[i] = 0;
        mVisibleItemIds[i] = 0;
    }
}

CMenuGetItemMulti::~CMenuGetItemMulti() {
    // CSysWin is raw storage, so its dtor is explicit here; the real CCur18
    // and UnkClass_8045F564 members are destroyed implicitly in reverse
    // declaration order, followed by the IUIWindow/CProcess base dtor that
    // MWCC emits with the retail double null-check.
}

extern "C" void GetItemMulti_ClearDoneLatch() { gpItemMultiSkillBdat = 0; }

void CMenuGetItemMulti::Init() {
    int memHandle = (int)mtl::MemManager::getHandleMEM2();
    mRegion.createRegion(
        memHandle, 0x10000, "CMenuGetItemMulti", 0);
    // Raw-storage construction: keeping the object as a compiler-managed
    // local extends its address liveness to the destructor and steals one
    // callee-saved register from the arc-base pointer (retail r25 vs r26).
    u32 regionHostStorage[sizeof(Class_8045F858) / sizeof(u32)];
    __ct__14Class_8045F858FP17UnkClass_8045F564(
        reinterpret_cast<Class_8045F858*>(&regionHostStorage[0]),
        &mRegion);

    char setupPaneName[32];
    char itemPaneName[32];
    char initialTextPaneName[32];
    char initialItemPaneName[32];
    char slotPaneName[32];
    char fullPaneName[32];
    u32 initialItems[4];

    if (mItems.mCount != 0 || field_1F6 != 0) {
        setPresentationFlag__Q22cf13CfGameManagerFv(1);
        code80135FDC_postIncByte_64080();
    }

    IWorkEvent* workEvent = reinterpret_cast<IWorkEvent*>(this);
    if (this != NULL) {
        workEvent = static_cast<IWorkEvent*>(this);
    }
    mFileHandle = CDeviceFile::readFile(memHandle, "common/jp/bdat_menu_mes.bin", workEvent, 0, 0);

    buildLayout(&mLayout, CUICfManager_getArcResourceAccessor(), "mf70_cf60_syswin09.brlyt");
    bindLayoutAnimTransform(mLayout, &mAnim, CUICfManager_getArcResourceAccessor(), "mf70_cf60_syswin09_in.brlan");

    nw4r::lyt::Pane* rootPane = mLayout->GetRootPane();
    CMenuGetItemFontObject* fontObject = reinterpret_cast<CMenuGetItemFontObject*>(
        getFontInfo__11CDeviceFontFUlPQ34nw4r3lyt6Layout(1, mLayout));
    ApplyFontToTxtPanes(rootPane, (const nw4r::ut::Font*)fontObject->getPane());

    u32 font = (u32)CUICfManager_getPackedFont9C();
    if (font != 0) {
        setLayoutTextBoxFont(mLayout, "txt_num01", font);
        setLayoutTextBoxFont(mLayout, "txt_num02", font);
        setLayoutTextBoxFont(mLayout, "txt_num03", font);
        setLayoutTextBoxFont(mLayout, "txt_num04", font);

        font = (u32)CUICfManager_getPackedFont9C();
        setLayoutTextBoxFont(mLayout, "txt_value02", font);
        setLayoutTextBoxFont(mLayout, "txt_value08", font);
        setLayoutTextBoxFont(mLayout, "txt_value09", font);
        setLayoutTextBoxFont(mLayout, "txt_value04", font);
        setLayoutTextBoxFont(mLayout, "txt_value07", font);
        setLayoutTextBoxFont(mLayout, "txt_value22", font);
        setLayoutTextBoxFont(mLayout, "txt_value50", font);
        setLayoutTextBoxFont(mLayout, "txt_value51", font);
        setLayoutTextBoxFont(mLayout, "txt_havenum", font);
        setLayoutTextBoxFont(mLayout, "txt_num01", font);
        setLayoutTextBoxFont(mLayout, "txt_num02", font);
        setLayoutTextBoxFont(mLayout, "txt_num03", font);
        setLayoutTextBoxFont(mLayout, "txt_num04", font);
        setLayoutTextBoxFont(mLayout, "txt_value30", font);
        setLayoutTextBoxFont(mLayout, "txt_value31", font);
        setLayoutTextBoxFont(mLayout, "txt_value32", font);
        setLayoutTextBoxFont(mLayout, "txt_value33", font);
        setLayoutTextBoxFont(mLayout, "txt_value34", font);

    nw4r::lyt::Pane* pane = mLayout->GetRootPane()->FindPaneByName(
        "txt_num01", true);
    pane->SetVisible(true);
    pane = mLayout->GetRootPane()->FindPaneByName("txt_num02", true);
    pane->SetVisible(true);
    pane = mLayout->GetRootPane()->FindPaneByName("txt_num03", true);
    pane->SetVisible(true);
    pane = mLayout->GetRootPane()->FindPaneByName("txt_num04", true);
    pane->SetVisible(true);

    char* helpCellText = BdatTouchStringCell("MNU_kyeassign", "help", 43);
    LayoutSetTextBoxFmtValue(mLayout, "txt_help", helpCellText, 0);

    char* fileName;
    if (cf::CfGameManager::isClassicController(-1) != 0) {
        fileName = "fileID_2";
    } else {
        fileName = "fileID_1";
    }
    u32 keyTextId = BdatGetU16ByTableKey("MNU_kyeassign", fileName, 43);
    char* buttonTexture = MakeTplNameSysFile((u16)keyTextId);
    nw4r::lyt::ArcResourceAccessor* accessor = CUICfManager_getArcResourceAccessor();
    CMenuGetItemTextureResource* texture = reinterpret_cast<CMenuGetItemTextureResource*>(
        accessor->GetResource(nw4r::lyt::ArcResourceAccessor::RES_TYPE_TEXTURE,
                              buttonTexture, NULL));
    if (texture != NULL) {
        PaneSetTexPaletteByName(mLayout, "pic_btn00", (TPLPalette*)texture);
        u16 textureWidth = texture->data->header->width;
        u16 textureHeight = texture->data->header->height;
        nw4r::lyt::Pane* buttonPane = mLayout->GetRootPane()->FindPaneByName(
            "pic_btn00", true);
        if (buttonPane != NULL) {
            buttonPane->SetSize(nw4r::lyt::Size((f32)textureWidth,
                                                (f32)textureHeight));
        }
    }

    mLayout->SetAnimationEnable(mAnim, true);
    mAnim->SetFrame(0.0f);
    mLayout->Animate(0);

    nw4r::lyt::Pane* closePane = mLayout->GetRootPane()->FindPaneByName(
        "txt_close00", true);
    CMenuGetItemPaneWord closeWord =
        reinterpret_cast<CMenuGetItemPaneExtra*>(closePane)->getPaneWord(0);
    gItemMultiClosePaneWord = closeWord.value;

    char* itemNameText = BdatTouchStringCell("MNU_sysmes", "name", 6);
    LayoutSetTextBoxFmtValue(mLayout, "txt_item05", itemNameText, 0);
    LayoutSetTextBoxFmtValue(mLayout, "txt_close00",
                   BdatTouchStringCell("MNU_kyeassign", "help", 79), 0);

    {
        u32 paneIndex;
        u8 i;
        for (i = 0; i < 4; ++i) {
            paneIndex = (u8)i + 1;
            sprintf(setupPaneName, "txt_item%02d", paneIndex);
            LayoutSetTextBoxFmtValue(mLayout, setupPaneName, "", 0);
            sprintf(setupPaneName, "txt_num%02d", paneIndex);
            LayoutSetTextBoxFmtValue(mLayout, setupPaneName, "", 0);
        }
    }

    char* collectNameText = BdatTouchStringCell("MNU_collect", "name", 16);
    LayoutSetTextBoxFmtValue(mLayout, "txt_have", collectNameText, 0);

    void* itemFP = getFP("MNU_item");
    LayoutSetTextBoxFmtValue(mLayout, "txt_para01",
                   BdatGetPtrDirect(itemFP, "name", 10), 0);
    LayoutSetTextBoxFmtValue(mLayout, "txt_para04",
                   BdatGetPtrDirect(itemFP, "name", 13), 0);
    LayoutSetTextBoxFmtValue(mLayout, "txt_para08",
                   BdatGetPtrDirect(itemFP, "name", 17), 0);
    LayoutSetTextBoxFmtValue(mLayout, "txt_para10",
                   BdatGetPtrDirect(itemFP, "name", 18), 0);
    LayoutSetTextBoxFmtValue(mLayout, "txt_para11",
                   BdatGetPtrDirect(itemFP, "name", 12), 0);
    LayoutSetTextBoxFmtValue(mLayout, "txt_para22",
                   BdatGetPtrDirect(itemFP, "name", 19), 0);
    LayoutSetTextBoxFmtValue(mLayout, "txt_para50",
                   BdatGetPtrDirect(itemFP, "name", 24), 0);
    LayoutSetTextBoxFmtValue(mLayout, "txt_para60",
                   BdatGetPtrDirect(itemFP, "name", 36), 0);
    LayoutSetTextBoxFmtValue(mLayout, "txt_para61",
                   BdatGetPtrDirect(itemFP, "name", 37), 0);
    LayoutSetTextBoxFmtValue(mLayout, "txt_para30",
                   BdatGetPtrDirect(itemFP, "name", 24), 0);
    LayoutSetTextBoxFmtValue(mLayout, "txt_para23",
                   BdatGetPtrDirect(itemFP, "name", 47), 0);
    char* mainNameText = BdatTouchStringCell("MNU_main", "name", 30);
    LayoutSetTextBoxFmtValue(mLayout, "txt_titlemes", mainNameText, 0);

    nw4r::lyt::Pane* rectPane = mLayout->GetRootPane()->FindPaneByName(
        "txt_num01", true);
    gItemMultiPaneColor08 = PaneMatGetS16Quad(rectPane, 0);
    gItemMultiPaneColor09 = PaneMatGetS16Quad(rectPane, 1);
    gItemMultiPaneColor0A.a = gItemMultiPaneColor08.a;
    gItemMultiPaneColor0B.a = gItemMultiPaneColor09.a;
    gItemMultiPaneColor0C.a = gItemMultiPaneColor08.a;
    gItemMultiPaneColor0D.a = gItemMultiPaneColor09.a;

    rectPane = mLayout->GetRootPane()->FindPaneByName("txt_para01", true);
    gItemMultiPaneColor00 = PaneMatGetS16Quad(rectPane, 0);
    gItemMultiPaneColor01 = PaneMatGetS16Quad(rectPane, 1);
    gItemMultiPaneColor02.a = gItemMultiPaneColor00.a;
    gItemMultiPaneColor03.a = gItemMultiPaneColor01.a;

    rectPane = mLayout->GetRootPane()->FindPaneByName("txt_value02", true);
    gItemMultiPaneColor04 = PaneMatGetS16Quad(rectPane, 0);
    gItemMultiPaneColor05 = PaneMatGetS16Quad(rectPane, 1);
    gItemMultiPaneColor06.a = gItemMultiPaneColor04.a;
    gItemMultiPaneColor07.a = gItemMultiPaneColor05.a;

    if (mItems.mCount != 0) {
        CItemExt* entry = &mItems.mItems[0];
        while (entry != &mItems.mItems[0] + mItems.mCount) {
            if (entry != NULL) {
                sprintf(itemPaneName, "txt_item%02d", mVisibleItemCount + 1);
                char* itemName;
                if (((entry->field_00 >> 16) & 0xf) == 3) {
                    ml::FixStr<32> rankText(false);
                    rankText.mString[0] = 0;
                    rankText.mLength = 0;
                    u16 rankCount = (u16)CItem_initItemImplInstances((CItemData*)entry)->vf08((CItemData*)entry);
                    itemName = (char*)CItem_initItemImplInstances((CItemData*)entry)->vf20((CItemData*)entry);
                    char* rankName = BdatTouchStringCell("MNU_item",
                                                   "name",
                                                   30 - (rankCount - 1));
                    rankText.format("%s%s", itemName, rankName);
                    LayoutSetTextBoxFmtValue(mLayout, itemPaneName, rankText.mString, 0);
                } else {
                    itemName = (char*)CItem_initItemImplInstances((CItemData*)entry)->vf20((CItemData*)entry);
                    LayoutSetTextBoxFmtValue(mLayout, itemPaneName, itemName, 0);
                }

                sprintf(itemPaneName, "pic_item%02d", mVisibleItemCount + 1);
                nw4r::lyt::Pane* pane = mLayout->GetRootPane()->FindPaneByName(
                    itemPaneName, true);
                if (pane != NULL) {
                    pane->SetVisible(true);
                }

                char* textureName = "mf00_com00_dmy.tpl";
                switch ((entry->field_00 >> 16) & 0xf) {
                case 2: textureName = "mf00_reg20_cat00.tpl"; break;
                case 3: textureName = "mf00_reg20_cat07.tpl"; break;
                case 4: textureName = "mf00_reg20_cat01.tpl"; break;
                case 5: textureName = "mf00_reg20_cat02.tpl"; break;
                case 6: textureName = "mf00_reg20_cat03.tpl"; break;
                case 7: textureName = "mf00_reg20_cat04.tpl"; break;
                case 8: textureName = "mf00_reg20_cat05.tpl"; break;
                case 9: textureName = "mf00_reg20_cat06.tpl"; break;
                case 10: textureName = "mf00_reg20_cat09.tpl"; break;
                case 11: {
                    u32 fontCheck = (u32)::cf::CfBdat::spItmMaterialListFileData;
                    // Retail: clrlwi into r3, set default texture, then
                    // neg/or/rlwinm bool-normalize (not rlwinm.+beq on a u8 local).
                    u32 fontHit = (u8)BdatGetU8Direct(
                        (const void*)fontCheck, "rare",
                        BdatGetItemId((u16)(entry->field_00 >> 20)));
                    textureName = "mf00_reg20_cat12.tpl";
                    if (((u32)(-(s32)fontHit | fontHit) >> 31) == 0) {
                        textureName = "mf00_reg20_cat10.tpl";
                    }
                    break;
                }
                case 12: textureName = "mf00_reg20_cat11.tpl"; break;
                case 13: textureName = "mf00_reg20_cat08.tpl"; break;
                }
                nw4r::lyt::ArcResourceAccessor* accessor = CUICfManager_getArcResourceAccessor();
                void* itemTexture = accessor->GetResource(
                    nw4r::lyt::ArcResourceAccessor::RES_TYPE_TEXTURE, textureName, NULL);
                if (itemTexture == NULL) {
                    accessor = CUICfManager_getArcResourceAccessor();
                    itemTexture = accessor->GetResource(
                        nw4r::lyt::ArcResourceAccessor::RES_TYPE_TEXTURE,
                        "mf00_com00_dmy.tpl", NULL);
                }
                if (itemTexture != NULL) {
                    PaneSetTexPaletteByName(mLayout, (const char*)itemPaneName, (TPLPalette*)itemTexture);
                }

                sprintf(itemPaneName, "txt_num%02d", mVisibleItemCount + 1);
                LayoutSetTextBoxFmtValue(mLayout, itemPaneName, "", 0);
                PaneMatSetTevColorsByName(mLayout, itemPaneName, &gItemMultiPaneColor08, &gItemMultiPaneColor09);

                {
                    // Commuted high bound blocks subi/cmpli fusion
                    // (CVS_THREAD_BATTLE_MAIN). Negating it yields retail's
                    // cmpi/ble; body order is slots then rank (retail layout).
                    s32 cat = (s32)((entry->field_00 >> 16) & 0xf);
                    if (cat < 4) {
                        goto check_cat_eq2;
                    }
                    if (!(8 < cat)) {
                        goto do_slots;
                    }
                check_cat_eq2:
                    if (cat == 2) {
                        goto do_slots;
                    }
                    if (cat == 3) {
                        goto do_rank;
                    }
                    if (cat == 9) {
                        goto do_rank;
                    }
                    goto after_cat_dispatch;
                do_slots:
                    {
                        // u32 + (u8) cast: retail clrlwi into the live slotCount
                        // reg (r19); a u8 local truncates to r0 then mr.
                        u32 slotCount = (u8)CItem_initItemImplInstances((CItemData*)entry)->vf30((CItemData*)entry);
                        if (slotCount != 0) {
                            LayoutSetTextBoxFmtValue(mLayout, itemPaneName, getErrMesText16(), 0);
                            PaneMatSetTevColorsByName(mLayout, itemPaneName, &gItemMultiPaneColor0A,
                                         &gItemMultiPaneColor0B);
                            for (u8 slot = 0; slot < slotCount; ++slot) {
                                if ((s16)CItem_initItemImplInstances((CItemData*)entry)->vf40((CItemData*)entry, slot) == -1) {
                                    u16* slotData = reinterpret_cast<u16*>(
                                        CItem_initItemImplInstances((CItemData*)entry)->vf2C((CItemData*)entry, slot));
                                    if (slotData != NULL && (slotData[2] & 1) != 0) {
                                        LayoutSetTextBoxFmtValue(mLayout, itemPaneName, getErrMesText15(), 0);
                                        PaneMatSetTevColorsByName(mLayout, itemPaneName, &gItemMultiPaneColor0C,
                                                     &gItemMultiPaneColor0D);
                                        break;
                                    }
                                }
                            }
                        }
                    }
                    goto after_cat_dispatch;
                do_rank:
                    {
                        u16 rankCount = (u16)CItem_initItemImplInstances((CItemData*)entry)->vf08((CItemData*)entry);
                        LayoutSetTextBoxFmtValue(mLayout, itemPaneName,
                                       BdatTouchStringCell("MNU_item",
                                                    "name",
                                                    30 - ((u8)rankCount - 1)), 0);
                        PaneMatSetTevColorsByName(mLayout, itemPaneName, &gItemMultiPaneColor08,
                                     &gItemMultiPaneColor09);
                    }
                after_cat_dispatch:;
                }

                // special=0 before cat re-extract (retail li r3,0 then clrlwi
                // into the CS cat reg). Bool-normalize the E8 hit. u32 cat
                // selects cmpli (retail); s32 would emit cmpi.
                {
                    u32 special = 0;
                    u32 catN = (entry->field_00 >> 16) & 0xf;
                    if (catN != 3 && catN != 9) {
                        u32 hit = (u8)BdatGetU8Direct(
                            (const void*)::cf::CfBdat::spItmItemListFileData, "item_keep",
                            (u16)(entry->field_00 >> 20));
                        special = (u32)(-(s32)hit | hit) >> 31;
                    }
                    if (catN == 12 || special != 0) {
                        mHasSpecialItem = 1;
                    }
                }
                {
                    u32 packed = entry->field_00;
                    u32 tmp = (packed >> 16) & 0xf;
                    u16 itemKey = (u16)(packed >> 20);
                    if (tmp != 0) {
                        goto cat2_join;
                    }
                    tmp = (u16)BdatGetItemType(itemKey);
                cat2_join:
                    {
                        u32 cat2 = (u16)tmp;
                        u32 special2;
                        BdatGetItemId(itemKey);
                        if ((u32)(cat2 - 2) <= 7) {
                            goto special2_lo;
                        }
                        if ((u32)(cat2 - 10) <= 3) {
                            goto special2_hi;
                        }
                        goto special2_zero;
                    special2_lo:
                        if (CItemBlock_countEmpty(cat2) == 0) {
                            goto special2_zero;
                        }
                        special2 = 1;
                        goto special2_test;
                    special2_hi:
                        {
                            int y = CItem_sumFamilyByte6(itemKey);
                            if (y < 1) {
                                goto special2_y_lt1;
                            }
                            if (y >= 0x63) {
                                goto special2_zero;
                            }
                            special2 = 1;
                            goto special2_test;
                        special2_y_lt1:
                            if (CItemBlock_countEmpty(cat2) == 0) {
                                goto special2_zero;
                            }
                            special2 = 1;
                            goto special2_test;
                        }
                    special2_zero:
                        special2 = 0;
                    special2_test:
                        if (special2 == 0) {
                            mPaneVisible[mVisibleItemCount] = 1;
                        }
                    }
                }
                // Postfix count++ (MWCC_CASES array-push): n→r3, idx→r0, n1→r4.
                mVisibleEntries[mVisibleItemCount++] = entry;
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
            char* itemTable = (char*)LookupItemTypeTable(itemId);
            u16 category = (u8)BdatGetItemType(itemId);
            u16 tableId = BdatGetItemId(itemId);
            if (itemId != 0) {
                sprintf(initialTextPaneName, "txt_item%02d",
                        mVisibleItemCount + 1);
                LayoutSetTextBoxFmtValue(mLayout, initialTextPaneName, BuildItemNameStr(itemId), 0);
                u32 special = 0;
                if (category != 3 && category != 9) {
                    u32 hit = (u8)BdatGetU8Direct((const void*)::cf::CfBdat::spItmItemListFileData,
                                                "item_keep",
                                                itemId);
                    special = (u32)(-(s32)hit | hit) >> 31;
                }
                if (category == 12 || special != 0) {
                    mHasSpecialItem = 1;
                }
                // Same special2 goto-chain as the entry-loop path (u32 cat2 +
                // range subi/cmpli + bottom li0 merge).
                {
                    u16 cat2 = (u16)BdatGetItemType(itemId);
                    u32 special2;
                    BdatGetItemId(itemId);
                    if ((u32)(cat2 - 2) <= 7) {
                        goto init_special2_lo;
                    }
                    if ((u32)(cat2 - 10) <= 3) {
                        goto init_special2_hi;
                    }
                    goto init_special2_zero;
                init_special2_lo:
                    if (CItemBlock_countEmpty(cat2) == 0) {
                        goto init_special2_zero;
                    }
                    special2 = 1;
                    goto init_special2_test;
                init_special2_hi:
                    {
                        int y = CItem_sumFamilyByte6(itemId);
                        if (y < 1) {
                            goto init_special2_y_lt1;
                        }
                        if (y >= 0x63) {
                            goto init_special2_zero;
                        }
                        special2 = 1;
                        goto init_special2_test;
                    init_special2_y_lt1:
                        if (CItemBlock_countEmpty(cat2) == 0) {
                            goto init_special2_zero;
                        }
                        special2 = 1;
                        goto init_special2_test;
                    }
                init_special2_zero:
                    special2 = 0;
                init_special2_test:
                    if (special2 == 0) {
                        mPaneVisible[mVisibleItemCount] = 1;
                    }
                }
                mVisibleItemIds[mVisibleItemCount++] = itemId;
            }

            sprintf(initialItemPaneName, "pic_item%02d", sourceIndex + 1);
            nw4r::lyt::Pane* pane = mLayout->GetRootPane()->FindPaneByName(
                initialItemPaneName, true);
            if (pane != NULL) {
                u32 neg = -(s32)itemId;
                u32 t = neg | itemId;
                pane->SetVisible(((t >> 30) & 1) != 0);
            }

            char* textureName = "mf00_com00_dmy.tpl";
            switch (category) {
            case 2: textureName = "mf00_reg20_cat00.tpl"; break;
            case 3: textureName = "mf00_reg20_cat07.tpl"; break;
            case 4: textureName = "mf00_reg20_cat01.tpl"; break;
            case 5: textureName = "mf00_reg20_cat02.tpl"; break;
            case 6: textureName = "mf00_reg20_cat03.tpl"; break;
            case 7: textureName = "mf00_reg20_cat04.tpl"; break;
            case 8: textureName = "mf00_reg20_cat05.tpl"; break;
            case 9: textureName = "mf00_reg20_cat06.tpl"; break;
            case 10: textureName = "mf00_reg20_cat09.tpl"; break;
            case 11: {
                u32 fontCheck = (u32)::cf::CfBdat::spItmMaterialListFileData;
                char* fontKey = "rare";
                u32 fontHit = (u8)BdatGetU8Direct(
                    (const void*)fontCheck, fontKey, (u16)tableId);
                textureName = "mf00_reg20_cat10.tpl";
                if ((u32)(-(s32)fontHit | fontHit) >> 31) {
                    textureName = "mf00_reg20_cat12.tpl";
                }
                break;
            }
            case 12: textureName = "mf00_reg20_cat11.tpl"; break;
            case 13: textureName = "mf00_reg20_cat08.tpl"; break;
            }
            nw4r::lyt::ArcResourceAccessor* accessor = CUICfManager_getArcResourceAccessor();
            void* itemTexture = accessor->GetResource(
                nw4r::lyt::ArcResourceAccessor::RES_TYPE_TEXTURE, textureName, NULL);
            if (itemTexture == NULL) {
                accessor = CUICfManager_getArcResourceAccessor();
                itemTexture = accessor->GetResource(
                    nw4r::lyt::ArcResourceAccessor::RES_TYPE_TEXTURE,
                    "mf00_com00_dmy.tpl", NULL);
            }
            if (itemTexture != NULL) {
                PaneSetTexPaletteByName(mLayout, (const char*)initialItemPaneName, (TPLPalette*)itemTexture);
            }

            sprintf(initialItemPaneName, "txt_num%02d", sourceIndex + 1);
            LayoutSetTextBoxFmtValue(mLayout, initialItemPaneName, "", 0);
            PaneMatSetTevColorsByName(mLayout, initialItemPaneName, &gItemMultiPaneColor08,
                         &gItemMultiPaneColor09);

            {
                s32 cat = (s32)category;
                if ((u32)(cat - 4) <= 4) {
                    goto do_initial_slots;
                }
                if (cat == 2) {
                    goto do_initial_slots;
                }
                if (cat == 3) {
                    goto do_initial_rank;
                }
                if (cat == 9) {
                    goto do_initial_rank;
                }
                goto after_initial_cat;
            do_initial_slots:
                {
                    u8 slotCount = BdatGetU8Direct(
                        (const void*)itemTable, "jwl_slot", (u16)tableId);
                    if (slotCount != 0) {
                        LayoutSetTextBoxFmtValue(mLayout, initialItemPaneName, getErrMesText16(), 0);
                        PaneMatSetTevColorsByName(mLayout, initialItemPaneName, &gItemMultiPaneColor0A,
                                     &gItemMultiPaneColor0B);
                        for (u8 slot = 0; slot < slotCount; ++slot) {
                            sprintf(slotPaneName, "jwl_skill%d", slot + 1);
                            if (BdatGetU16Direct(itemTable, slotPaneName,
                                              (u16)tableId) != 0) {
                                LayoutSetTextBoxFmtValue(mLayout, initialItemPaneName,
                                               getErrMesText15(), 0);
                                PaneMatSetTevColorsByName(mLayout, initialItemPaneName,
                                             &gItemMultiPaneColor0C, &gItemMultiPaneColor0D);
                                break;
                            }
                        }
                    }
                }
                goto after_initial_cat;
            do_initial_rank:
                {
                    u8 rankCount = BdatGetU8Direct((const void*)::cf::CfBdat::spItmItemListFileData,
                                                  "rankType", itemId);
                    LayoutSetTextBoxFmtValue(mLayout, initialItemPaneName,
                                   BdatTouchStringCell("MNU_item",
                                                "name",
                                                30 - (rankCount - 1)), 0);
                    PaneMatSetTevColorsByName(mLayout, initialItemPaneName, &gItemMultiPaneColor08,
                                 &gItemMultiPaneColor09);
                }
            after_initial_cat:;
            }
        }
    }

    GetItemMulti_FitPanes(this);
    if (mHasSpecialItem != 0) {
        PaneSetVtxColorByName(mLayout, "txt_close00", 0x777777ff);
    }
    {
        u8 visible;
        u8 i;
        for (i = 0; i < 4; ++i) {
            sprintf(fullPaneName, "pic_full%02d", i + 1);
            visible = mPaneVisible[i];
            nw4r::lyt::Pane* pane = mLayout->GetRootPane()->FindPaneByName(
                fullPaneName, true);
            pane->SetVisible(visible != 0);
        }
    }

    GetItemMulti_ShowDetail(this, 0, 0);

    mCursor = CCur18(CUICfManager_getArcResourceAccessor());
    mCursor.initLayout();

    u8 systemWindowStorage[sizeof(CSysWin)];
    __ct__7CSysWinFUc(reinterpret_cast<CSysWin*>(&systemWindowStorage[0]), 2);
    mSystemWindow.mMemRegion.unk0 =
        reinterpret_cast<CSysWin*>(&systemWindowStorage[0])->mMemRegion.unk0;
    mSystemWindow.mMemRegion.unk4 =
        reinterpret_cast<CSysWin*>(&systemWindowStorage[0])->mMemRegion.unk4;
    mSystemWindow.mMemRegion.unk8 =
        reinterpret_cast<CSysWin*>(&systemWindowStorage[0])->mMemRegion.unk8;
    mSystemWindow.mMemRegion.unkC =
        reinterpret_cast<CSysWin*>(&systemWindowStorage[0])->mMemRegion.unkC;
    mSystemWindow.mFileHandle =
        reinterpret_cast<CSysWin*>(&systemWindowStorage[0])->mFileHandle;
    mSystemWindow.mTagProcessor =
        reinterpret_cast<CSysWin*>(&systemWindowStorage[0])->mTagProcessor;
    mSystemWindow.mArcAccessor =
        reinterpret_cast<CSysWin*>(&systemWindowStorage[0])->mArcAccessor;
    mSystemWindow.mLayout =
        reinterpret_cast<CSysWin*>(&systemWindowStorage[0])->mLayout;
    mSystemWindow.mAnimTrans =
        reinterpret_cast<CSysWin*>(&systemWindowStorage[0])->mAnimTrans;
    mSystemWindow.field_28 =
        reinterpret_cast<CSysWin*>(&systemWindowStorage[0])->field_28;
    mSystemWindow.field_2C =
        reinterpret_cast<CSysWin*>(&systemWindowStorage[0])->field_2C;
    mSystemWindow.field_30 =
        reinterpret_cast<CSysWin*>(&systemWindowStorage[0])->field_30;
    mSystemWindow.field_34 =
        reinterpret_cast<CSysWin*>(&systemWindowStorage[0])->field_34;
    mSystemWindow.field_35 =
        reinterpret_cast<CSysWin*>(&systemWindowStorage[0])->field_35;
    mSystemWindow.field_36 =
        reinterpret_cast<CSysWin*>(&systemWindowStorage[0])->field_36;
    mSystemWindow.field_37 =
        reinterpret_cast<CSysWin*>(&systemWindowStorage[0])->field_37;
    mSystemWindow.field_38 =
        reinterpret_cast<CSysWin*>(&systemWindowStorage[0])->field_38;
    mSystemWindow.field_39 =
        reinterpret_cast<CSysWin*>(&systemWindowStorage[0])->field_39;
    __dt__7CSysWinFv(reinterpret_cast<CSysWin*>(&systemWindowStorage[0]), -1);
    mSystemWindow.loadSystemArc();

    if ((*reinterpret_cast<u32*>(cf::CfGameManager::getCurrentPad()) & 0x0001e000) != 0) {
        field_200 = 1;
    }

    IScnRender* render = reinterpret_cast<IScnRender*>(this);
    if (this != NULL) {
        render = static_cast<IScnRender*>(this);
    }
    mScn->addRenderCB(render, 13, 0);
    mRegion.validateHeap();
    __dt__14Class_8045F858Fv(
        reinterpret_cast<Class_8045F858*>(&regionHostStorage[0]), -1);
    }
}

void CMenuGetItemMulti::Term() {
    CDeviceVI::waitForDrawDone();
    IScnRender* render = static_cast<IScnRender*>(this);
    mScn->removeRenderCB(render);
    CBdat::getEntry(5);
    closeFileHandle(&mFileHandle);
    if (field_208 != 0) {
        mtl::MemManager::deallocate(reinterpret_cast<void*>(field_208));
        field_208 = 0;
    }
    mCursor.cleanup();
    mSystemWindow.termLayout();
    if (mLayout != 0) {
        delete mLayout;
        mLayout = 0;
    }
    mRegion.deleteRegion();
    gpItemMultiInstance = 0;
    gpItemMultiSkillBdat = 0;
    if (mItems.mCount != 0 || field_1F6 != 0) {
        DecMenuCounter64080();
        if (code80135FDC_getByte_64080() == 0) {
            setPresentationFlag__Q22cf13CfGameManagerFv(0);
        }
        // Nested: when both outer conditions fail MWCC jumps the
        // field_1F6 test straight to the epilogue (retail beq .L_801B5980).
        if (mItems.mCount != 0) {
            gmGameFlags &= ~0x200000u;
            CfGameManagerTermFields* mgrView = reinterpret_cast<CfGameManagerTermFields*>(
                cf::CfGameManager::getInstance());
            CfGameManagerTermFields* src = reinterpret_cast<CfGameManagerTermFields*>(
                findObjectById(mgrView->field_80));
            if (src != 0 && (src->field_64 & 0x100) != 0) {
                reinterpret_cast<CfGameManagerTermFields*>(
                    cf::CfGameManager::getInstance())->field_80 = 0;
                removeTboxById(reinterpret_cast<u32>(src));
                src->field_68 |= 0x60;
            }
        }
    }
}

void CMenuGetItemMulti::Move() {
    CTaskGame::getInstance();
    if (CTaskGame::isFlag01Set() != 0) goto exit;
    if (gmPauseFlags & (1u << 21)) goto exit;
    // Branch-over-branch guard: `goto body` with the `exit` label + return
    // placed BEFORE `body` keeps MWCC from folding the bit test to a single
    // `bne` -- it emits retail's `beq body; b exit`.
    goto body;
exit:
    return;
body:
    if (!IsMenuState621F0()) goto exit;

    // Per-frame state machine: field_1F8 selects the menu sub-state; the
    // dense 0..11 range is emitted as a jump table, and every case falls
    // out to the common per-frame update tail below.
    switch (field_1F8) {
    case 0:  // window ready: show the item-select cursor
        if (mSystemWindow.isReady() == 0) break;
        if (gpItemMultiSkillBdat == 0) break;
        field_1F8 = 1;
        playUISound(0x2d);
        break;
    case 1:  // opening animation: position the cursor on the active item
        if (advanceAnimTransform(mAnim, 1.0f) == 0) break;
        mCursor.SetVisible(1);
        {
            char buf[32];
            s8 n = (s8)mMaxVisibleItems;
            if (n == 5) {
                sprintf(buf, "txt_close00");
            } else if (n == 4) {
                sprintf(buf, "txt_item%02d", n + 1);
            } else {
                sprintf(buf, "nul_item%02d", n + 1);
            }
            nw4r::lyt::Pane* pane = mLayout->GetRootPane()->FindPaneByName(buf, true);
            nw4r::lyt::Pane* refPane = mLayout->GetRootPane()->FindPaneByName(
                "nul_proportion", true);
            nw4r::math::VEC3 pos;
            ScalePaneWorldPos(&pos, pane, refPane, mLayout->GetRootPane());
            mCursor.setRootPaneTranslate(&pos);
        }
        field_1F8 = 2;
        break;
    case 2:  // idle: pad-input handler
        GetItemMulti_HandlePad(this);
        break;
    case 3:  // closing animation: flag the fade-out
        if (AnimRewindFrame(mAnim, lbl_eu_80667E10) == 0) break;
        mField64 = 1;
        break;
    case 4:  // closing: teardown handler
        GetItemMulti_PruneEntries(this);
        break;
    case 5:  // window active: cursor follows the slot selection
        if (mSystemWindow.isActive() == 0) break;
        mCursor.SetVisible(1);
        {
            nw4r::math::VEC3 pos;
            CSysWin::getPaneScreenPos(reinterpret_cast<nw4r::math::VEC3*>(&pos), &mSystemWindow, field_1F4);
            mCursor.setRootPaneTranslate(&pos);
        }
        field_1F8 = 6;
        break;
    case 6:  // window selection: pad-input handler
        GetItemMulti_HandleWindowPadOnce(this);
        break;
    case 7:  // confirm selection: open the item menu for the active item
        if (mSystemWindow.isActive() == 0) break;
        if (field_1F5 != 0) {
            u32 cat;
            if (mItems.mCount != 0) {
                cat = (mVisibleEntries[(s8)mMaxVisibleItems]->field_00 >> 16) & 0xF;
            } else {
                cat = BdatGetItemType(mVisibleItemIds[(s8)mMaxVisibleItems]) & 0xff;
            }
            CUICfManager_queuePauseItemMenu((u8)cat, 0, field_201);
            field_1F8 = 0xb;
            field_1F5 = 0;
        } else {
            mCursor.SetVisible(1);
            field_1F8 = 2;
        }
        break;
    case 8:  // window active (alt): cursor follows the slot selection
        if (mSystemWindow.isActive() == 0) break;
        mCursor.SetVisible(1);
        {
            nw4r::math::VEC3 pos;
            CSysWin::getPaneScreenPos(reinterpret_cast<nw4r::math::VEC3*>(&pos), &mSystemWindow, field_1F4);
            mCursor.setRootPaneTranslate(&pos);
        }
        field_1F8 = 9;
        break;
    case 9:  // window selection (alt): pad-input handler
        GetItemMulti_HandleWindowPadAuto(this);
        break;
    case 10: // confirm from slot state: reopen or confirm
        if (mSystemWindow.isActive() == 0) break;
        if ((s8)field_1F4 == 0) {
            field_1F8 = 3;
            playUISound(0x89);
        } else {
            mCursor.SetVisible(1);
            field_1F8 = 2;
        }
        break;
    case 11: // menu closing: reposition the cursor on the active item
        if (ItemMenu_IsPresent() != 0) break;
        mCursor.SetVisible(1);
        GetItemMulti_RefreshList(this);
        {
            char buf[32];
            s8 n = (s8)mMaxVisibleItems;
            if (n == 5) {
                sprintf(buf, "txt_close00");
            } else if (n == 4) {
                sprintf(buf, "txt_item%02d", n + 1);
            } else {
                sprintf(buf, "nul_item%02d", n + 1);
            }
            nw4r::lyt::Pane* pane = mLayout->GetRootPane()->FindPaneByName(buf, true);
            nw4r::lyt::Pane* refPane = mLayout->GetRootPane()->FindPaneByName(
                "nul_proportion", true);
            nw4r::math::VEC3 pos;
            ScalePaneWorldPos(&pos, pane, refPane, mLayout->GetRootPane());
            mCursor.setRootPaneTranslate(&pos);
        }
        field_1F8 = 2;
        break;
    }

    // Per-frame update tail (all states fall through to here).
    mLayout->Animate(0);
    mCursor.Update();
    mSystemWindow.dispatchPhase();
}

// File-event completion handler: OnFileEvent routes the CEventFile through
// here once the owning CFileHandle matches (event->mFileHandle == this->mFileHandle).
// Steals the loaded buffer, publishes it to field_208, then re-reads the
// BDAT file pointer and clears the handle. Returns 1 on success.
bool CMenuGetItemMulti::OnFileEvent(CEventFile* event) {
    if (this->mFileHandle == event->mFileHandle) {
        u8* data = this->mFileHandle->mData;
        this->mFileHandle->mData = 0;
        this->field_208 = reinterpret_cast<u32>(data);
        Bdat_GetTable_AA34();
        if (getFP("MNU_skill") == 0) {
            CBdat::setBdatEntry(5, reinterpret_cast<u8*>(this->field_208));
        }
        gpItemMultiSkillBdat = reinterpret_cast<u32>(getFP("MNU_skill"));
        this->mFileHandle = 0;
        return true;
    }
    return false;
}

// Adjust the item-multi pane layout for the visible-item count: each hidden
// item (4 - mVisibleItemCount, clamped at 0) shrinks the item-text pane
// height and shifts the close/other panes down by lbl_eu_80667E14 per step.
//
// Residual note: the (f32)(s32) casts pool MWCC's internal 0x43300000_80000000
// magic as a TU-local @12792 label; retail resolves the same bytes to the
// named .sdata2 slot. No source form references that pool entry without
// rescheduling the whole block (union forms verified +16B / frame-slot
// flips), so the TU keeps the named pool reference here.
extern "C" void GetItemMulti_FitPanes(CMenuGetItemMulti* self) {
    s32 count = 4 - (s32)self->mVisibleItemCount;
    if (count < 0) {
        count = 0;
    }

    nw4r::lyt::Pane* pane = self->mLayout->GetRootPane()->FindPaneByName(
        "win_main", true);
    if (pane != 0) {
        nw4r::lyt::Size size = pane->GetSize();
        size.height -= 32.0f * (f32)count;
        reinterpret_cast<CMenuGetItemPaneView*>(pane)->height = size.height;
    }
    pane = self->mLayout->GetRootPane()->FindPaneByName(
        "txt_item05", true);
    if (pane != 0) {
        nw4r::math::VEC3 pos = pane->GetTranslate();
        pos.y = pos.y + 32.0f * (f32)count;
        pane->SetTranslate(pos);
    }
    pane = self->mLayout->GetRootPane()->FindPaneByName(
        "txt_close00", true);
    if (pane != 0) {
        nw4r::math::VEC3 pos = pane->GetTranslate();
        pos.y = pos.y + 32.0f * (f32)count;
        pane->SetTranslate(pos);
    }
    pane = self->mLayout->GetRootPane()->FindPaneByName(
        "nul_close00", true);
    if (pane != 0) {
        nw4r::math::VEC3 pos = pane->GetTranslate();
        pos.y = pos.y + 32.0f * (f32)count;
        pane->SetTranslate(pos);
    }
}

// Render-callback: gate prefix (task busy / global bit 21 / scene active),
// then draw the layout, system window and cursor through a stack DrawInfo.
// The DrawInfo's scope-exit dtor is auto-emitted as the retail direct
// `bl __dt__Q34nw4r3lyt8DrawInfoFv(drawInfo, -1)`; an explicit
// `drawInfo.~DrawInfo()` would make MWCC ALSO emit a virtual-dispatched
// scope-exit dtor, so none is written here.
void CMenuGetItemMulti::cbRenderBefore(CScn*) {
    CTaskGame::getInstance();
    if (CTaskGame::isFlag01Set() != 0) goto exit;
    if (gmPauseFlags & (1u << 21)) goto exit;
    // Branch-over-branch guard: `goto body` with the `exit` label + return
    // placed BEFORE `body` keeps MWCC from folding the bit test to a single
    // `bne` -- it emits retail's `beq body; b exit`.
    goto body;
exit:
    return;
body:
    if (!IsMenuState621F0()) goto exit;
    {
        GXSetZMode(GX_FALSE, GX_NEVER, GX_FALSE);
        nw4r::lyt::DrawInfo drawInfo;
        SetWideLocAdjust(&drawInfo);
        drawLayout(mLayout, &drawInfo, 0, 1);
        mSystemWindow.drawLayout(reinterpret_cast<nw4r::lyt::DrawInfo*>(&drawInfo));
        mCursor.DrawLayout(&drawInfo);
    }
}

// Item-multi window factory: creates the single get-item-multi task object
// (retail SDA slot gpItemMultiInstance) under the given parent process and
// returns it, or 0 when one already exists. The 8 ctor arguments are
// forwarded verbatim (last is a byte flag on the stack).
IUIWindow* GetItemMulti_CreateInstance(CProcess* pParent, CScn* pScene, u32 a, u32 b, u32 c,
                         u32 d, u32 e, u32 f, u8 g) {
    if (gpItemMultiInstance != 0) {
        return 0;
    }
    CMenuGetItemMulti* obj = new (CWorkThreadSystem::getWorkMem())
        CMenuGetItemMulti(pScene, a, b, c, d, e, f, g);
    gpItemMultiInstance = reinterpret_cast<u32>(obj);
    reinterpret_cast<CProcess*>(obj)->Regist(pParent, false);
    return reinterpret_cast<IUIWindow*>(gpItemMultiInstance);
}

// Create the single get-item-multi task object (retail SDA slot
// gpItemMultiInstance) under the given parent process. Returns the instance
// pointer, or 0 when one already exists.
u8* GetItemMulti_CreateSimple(CProcess* parent, u32 arg2, u32 arg3) {
    if (gpItemMultiInstance != 0) {
        return 0;
    }
    CMenuGetItemMulti* obj = new (CWorkThreadSystem::getWorkMem())
        CMenuGetItemMulti(reinterpret_cast<CScn*>(arg2),
                          *reinterpret_cast<const CItemFour*>(arg3));
    gpItemMultiInstance = reinterpret_cast<u32>(obj);
    reinterpret_cast<CProcess*>(obj)->Regist(parent, false);
    return (u8*)gpItemMultiInstance;
}

extern "C" unsigned long GetItemMulti_IsActiveFlag() { return gpItemMultiInstance != 0; }

// Refresh the get-item-multi display after a sweep: re-compact the visible
// item list (packed entries or plain ids), rebuild every item pane (name,
// rank text, texture, slots), restore the pane visibility and the special-item
// colour, then re-centre the cursor on the active item. Called by the other
// pad handlers whenever the selection changes.
void GetItemMulti_RefreshList(CMenuGetItemMulti* self) {
    self->mHasSpecialItem = 0;
    for (u8 i = 0; i < 4; ++i) {
        char buf[32];
        sprintf(buf, "txt_item%02d", i + 1);
        LayoutSetTextBoxFmtValue(self->mLayout, buf, "", 0);
        sprintf(buf, "txt_num%02d", i + 1);
        LayoutSetTextBoxFmtValue(self->mLayout, buf, "", 0);
        sprintf(buf, "pic_item%02d", i + 1);
        nw4r::lyt::Pane* pane = self->mLayout->GetRootPane()->FindPaneByName(buf, true);
        if (pane != 0) {
            pane->SetVisible(false);
        }
        self->mPaneVisible[i] = 0;
    }

    if (self->mItems.mCount != 0) {
        // Compact the non-null visible entries into the front of the list.
        CItemExt* tmp[4] = {0, 0, 0, 0};
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
            CItemExt* entry = self->mVisibleEntries[i];
            if (entry == 0) {
                self->mPaneVisible[i] = 0;
                continue;
            }
            char paneName[32];
            char texPaneName[32]; // retail keeps a second buffer for the
                                  // texture-pane sprintf (sp+0xD0 vs sp+0xF0)
            sprintf(paneName, "txt_item%02d", i + 1);
            char* itemName;
            if (((entry->field_00 >> 12) & 0xF) == 3) {
                ml::FixStr<32> rankText(false);
                rankText.mString[0] = 0;
                rankText.mLength = 0;
                u16 rankCount = (u16)CItem_initItemImplInstances((CItemData*)entry)->vf08((CItemData*)entry);
                itemName = (char*)CItem_initItemImplInstances((CItemData*)entry)->vf20((CItemData*)entry);
                char* rankName = BdatTouchStringCell("MNU_item",
                                               "name",
                                               30 - (rankCount - 1));
                rankText.format("%s%s", itemName, rankName);
                LayoutSetTextBoxFmtValue(self->mLayout, paneName, rankText.mString, 0);
            } else {
                itemName = (char*)CItem_initItemImplInstances((CItemData*)entry)->vf20((CItemData*)entry);
                LayoutSetTextBoxFmtValue(self->mLayout, paneName, itemName, 0);
            }

            u32 id = entry->field_00 >> 20;
            u16 cat = (entry->field_00 >> 12) & 0xF;
            if (cat == 0) {
                cat = (u16)BdatGetItemType(id);
            }
            BdatGetItemId(id);
            int special = 0;
            if (cat >= 2 && cat <= 9) {
                if (CItemBlock_countEmpty(cat) != 0) {
                    special = 1;
                }
            } else if (cat >= 10 && cat <= 13) {
                int lvl = CItem_sumFamilyByte6(id);
                if (lvl < 1) {
                    if (CItemBlock_countEmpty(cat) != 0) {
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

            u32 raw = entry->field_00;
            u32 cat2 = (raw >> 12) & 0xF;
            int special2 = 0;
            if (cat2 != 3 && cat2 != 9) {
                special2 = BdatGetU8Direct((const void*)::cf::CfBdat::spItmItemListFileData, "item_keep",
                                         raw >> 20) != 0;
            }
            if (cat2 == 12 || special2 != 0) {
                self->mHasSpecialItem = 1;
            }

            sprintf(texPaneName, "pic_item%02d", i + 1);
            nw4r::lyt::Pane* pane = self->mLayout->GetRootPane()->FindPaneByName(texPaneName, true);
            if (pane != 0) {
                pane->SetVisible(entry != 0);
            }

            char* textureName = "mf00_com00_dmy.tpl";
            switch ((entry->field_00 >> 12) & 0xF) {
            case 2: textureName = "mf00_reg20_cat00.tpl"; break;
            case 3: textureName = "mf00_reg20_cat07.tpl"; break;
            case 4: textureName = "mf00_reg20_cat01.tpl"; break;
            case 5: textureName = "mf00_reg20_cat02.tpl"; break;
            case 6: textureName = "mf00_reg20_cat03.tpl"; break;
            case 7: textureName = "mf00_reg20_cat04.tpl"; break;
            case 8: textureName = "mf00_reg20_cat05.tpl"; break;
            case 9: textureName = "mf00_reg20_cat06.tpl"; break;
            case 10: textureName = "mf00_reg20_cat09.tpl"; break;
            case 11: {
                u32 fontCheck = (u32)::cf::CfBdat::spItmMaterialListFileData;
                textureName = "mf00_reg20_cat10.tpl";
                if (BdatGetU8Direct((const void*)fontCheck, "rare",
                                 BdatGetItemId((u16)(entry->field_00 >> 20)))) {
                    textureName = "mf00_reg20_cat12.tpl";
                }
                break;
            }
            case 12: textureName = "mf00_reg20_cat11.tpl"; break;
            case 13: textureName = "mf00_reg20_cat08.tpl"; break;
            }
            // Retail consumes CUICfManager_getArcResourceAccessor() immediately (no cached accessor
            // live across the virtual GetResource call).
            void* itemTexture = CUICfManager_getArcResourceAccessor()->GetResource(
                nw4r::lyt::ArcResourceAccessor::RES_TYPE_TEXTURE, textureName, NULL);
            if (itemTexture == NULL) {
                itemTexture = CUICfManager_getArcResourceAccessor()->GetResource(
                    nw4r::lyt::ArcResourceAccessor::RES_TYPE_TEXTURE,
                    "mf00_com00_dmy.tpl", NULL);
            }
            if (itemTexture != NULL) {
                PaneSetTexPaletteByName(self->mLayout, (const char*)texPaneName, (TPLPalette*)itemTexture);
            }

            sprintf(paneName, "txt_num%02d", i + 1);
            LayoutSetTextBoxFmtValue(self->mLayout, paneName, "", 0);
            PaneMatSetTevColorsByName(self->mLayout, paneName, &gItemMultiPaneColor08,
                         &gItemMultiPaneColor09);

            u32 cat3 = (entry->field_00 >> 12) & 0xF;
            if ((cat3 >= 4 && cat3 <= 8) || cat3 == 2) {
                u8 slotCount = (u8)CItem_initItemImplInstances((CItemData*)entry)->vf30((CItemData*)entry);
                if (slotCount != 0) {
                    LayoutSetTextBoxFmtValue(self->mLayout, paneName, getErrMesText16(), 0);
                    PaneMatSetTevColorsByName(self->mLayout, paneName, &gItemMultiPaneColor0A,
                                 &gItemMultiPaneColor0B);
                    for (u8 slot = 0; slot < slotCount; ++slot) {
                        if ((s16)CItem_initItemImplInstances((CItemData*)entry)->vf40((CItemData*)entry, slot) == -1) {
                            u16* slotData = reinterpret_cast<u16*>(
                                CItem_initItemImplInstances((CItemData*)entry)->vf2C((CItemData*)entry, slot));
                            if (slotData != NULL && (slotData[2] & 1) != 0) {
                                LayoutSetTextBoxFmtValue(self->mLayout, paneName, getErrMesText15(), 0);
                                PaneMatSetTevColorsByName(self->mLayout, paneName, &gItemMultiPaneColor0C,
                                             &gItemMultiPaneColor0D);
                                break;
                            }
                        }
                    }
                }
            } else if (cat3 == 3 || cat3 == 9) {
                u16 rankCount = (u16)CItem_initItemImplInstances((CItemData*)entry)->vf08((CItemData*)entry);
                LayoutSetTextBoxFmtValue(self->mLayout, paneName,
                               BdatTouchStringCell("MNU_item",
                                            "name",
                                            30 - ((u8)rankCount - 1)), 0);
                PaneMatSetTevColorsByName(self->mLayout, paneName, &gItemMultiPaneColor08,
                             &gItemMultiPaneColor09);
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
            char* itemTable = (char*)LookupItemTypeTable(itemId);
            u8 category = (u8)BdatGetItemType(itemId);
            u32 tableId = BdatGetItemId(itemId);
            if (itemId == 0) {
                continue;
            }
            char paneName[32];
            sprintf(paneName, "txt_item%02d", i + 1);
            LayoutSetTextBoxFmtValue(self->mLayout, paneName, BuildItemNameStr(itemId), 0);

            u16 cat = (u16)BdatGetItemType(itemId);
            BdatGetItemId(itemId);
            int special = 0;
            if (cat >= 2 && cat <= 9) {
                if (CItemBlock_countEmpty(cat) != 0) {
                    special = 1;
                }
            } else if (cat >= 10 && cat <= 13) {
                int lvl = CItem_sumFamilyByte6(itemId);
                if (lvl < 1) {
                    if (CItemBlock_countEmpty(cat) != 0) {
                        special = 1;
                    }
                } else if (lvl < 0x63) {
                    special = 1;
                }
            }
            // Retail derives the visibility byte with a cntlzw NOT-shape.
            self->mPaneVisible[i] = (special == 0);

            int special2 = 0;
            if (category != 3 && category != 9) {
                special2 = BdatGetU8Direct((const void*)::cf::CfBdat::spItmItemListFileData, "item_keep",
                                         itemId) != 0;
            }
            if (category == 12 || special2 != 0) {
                self->mHasSpecialItem = 1;
            }

            sprintf(paneName, "pic_item%02d", i + 1);
            nw4r::lyt::Pane* pane = self->mLayout->GetRootPane()->FindPaneByName(
                paneName, true);
            if (pane != 0) {
                u8 visible = itemId != 0;
                pane->SetVisible(visible != 0);
            }

            char* textureName = "mf00_com00_dmy.tpl";
            switch (category) {
            case 2: textureName = "mf00_reg20_cat00.tpl"; break;
            case 3: textureName = "mf00_reg20_cat07.tpl"; break;
            case 4: textureName = "mf00_reg20_cat01.tpl"; break;
            case 5: textureName = "mf00_reg20_cat02.tpl"; break;
            case 6: textureName = "mf00_reg20_cat03.tpl"; break;
            case 7: textureName = "mf00_reg20_cat04.tpl"; break;
            case 8: textureName = "mf00_reg20_cat05.tpl"; break;
            case 9: textureName = "mf00_reg20_cat06.tpl"; break;
            case 10: textureName = "mf00_reg20_cat09.tpl"; break;
            case 11: {
                u32 fontCheck = (u32)::cf::CfBdat::spItmMaterialListFileData;
                textureName = "mf00_reg20_cat10.tpl";
                if (BdatGetU8Direct((const void*)fontCheck, "rare", tableId)) {
                    textureName = "mf00_reg20_cat12.tpl";
                }
                break;
            }
            case 12: textureName = "mf00_reg20_cat11.tpl"; break;
            case 13: textureName = "mf00_reg20_cat08.tpl"; break;
            }
            void* itemTexture = CUICfManager_getArcResourceAccessor()->GetResource(
                nw4r::lyt::ArcResourceAccessor::RES_TYPE_TEXTURE, textureName, NULL);
            if (itemTexture == NULL) {
                itemTexture = CUICfManager_getArcResourceAccessor()->GetResource(
                    nw4r::lyt::ArcResourceAccessor::RES_TYPE_TEXTURE,
                    "mf00_com00_dmy.tpl", NULL);
            }
            if (itemTexture != NULL) {
                PaneSetTexPaletteByName(self->mLayout, (const char*)paneName, (TPLPalette*)itemTexture);
            }

            sprintf(paneName, "txt_num%02d", i + 1);
            LayoutSetTextBoxFmtValue(self->mLayout, paneName, "", 0);
            PaneMatSetTevColorsByName(self->mLayout, paneName, &gItemMultiPaneColor08,
                         &gItemMultiPaneColor09);

            if ((category >= 4 && category <= 8) || category == 2) {
                u8 slotCount = BdatGetU8Direct((const void*)itemTable, "jwl_slot",
                                             tableId);
                if (slotCount != 0) {
                    LayoutSetTextBoxFmtValue(self->mLayout, paneName, getErrMesText16(), 0);
                    PaneMatSetTevColorsByName(self->mLayout, paneName, &gItemMultiPaneColor0A,
                                 &gItemMultiPaneColor0B);
                    for (u8 slot = 0; slot < slotCount; ++slot) {
                        char slotPaneName[32];
                        sprintf(slotPaneName, "jwl_skill%d", slot + 1);
                        if (BdatGetU16Direct(itemTable, slotPaneName, tableId) != 0) {
                            LayoutSetTextBoxFmtValue(self->mLayout, paneName, getErrMesText15(), 0);
                            PaneMatSetTevColorsByName(self->mLayout, paneName, &gItemMultiPaneColor0C,
                                         &gItemMultiPaneColor0D);
                            break;
                        }
                    }
                }
            } else if (category == 3 || category == 9) {
                u8 rankCount = BdatGetU8Direct((const void*)::cf::CfBdat::spItmItemListFileData, "rankType",
                                             itemId);
                LayoutSetTextBoxFmtValue(self->mLayout, paneName,
                               BdatTouchStringCell("MNU_item",
                                            "name",
                                            30 - (rankCount - 1)), 0);
                PaneMatSetTevColorsByName(self->mLayout, paneName, &gItemMultiPaneColor08,
                             &gItemMultiPaneColor09);
            }
        }
    }

    // Full-pane visibility + special-item colour, then re-centre the cursor.
    for (u8 i = 0; i < 4; ++i) {
        char buf[32];
        sprintf(buf, "pic_full%02d", i + 1);
        u8 visible = self->mPaneVisible[i];
        nw4r::lyt::Pane* pane = self->mLayout->GetRootPane()->FindPaneByName(buf, true);
        pane->SetVisible(visible != 0);
    }
    if (self->mHasSpecialItem != 0) {
        PaneSetVtxColorByName(self->mLayout, "txt_close00", 0x777777ff);
    } else {
        PaneSetVtxColorByName(self->mLayout, "txt_close00", gItemMultiClosePaneWord);
    }
    self->mMaxVisibleItems = 0;
    {
        char buf[32];
        // Retail keeps the compare against the just-stored value and passes a
        // literal 1 (the m+1 of the always-zero selector).
        s8& m = reinterpret_cast<s8&>(self->mMaxVisibleItems);
        if (m == 5) {
            sprintf(buf, "txt_close00");
        } else if (m == 4) {
            sprintf(buf, "txt_item%02d", 1);
        } else {
            sprintf(buf, "nul_item%02d", 1);
        }
        nw4r::lyt::Pane* pane = self->mLayout->GetRootPane()->FindPaneByName(buf, true);
        nw4r::lyt::Pane* refPane = self->mLayout->GetRootPane()->FindPaneByName(
            "nul_proportion", true);
        nw4r::math::VEC3 pos;
        ScalePaneWorldPos(&pos, pane, refPane, self->mLayout->GetRootPane());
        self->mCursor.setRootPaneTranslate(&pos);
    }

    if (self->mItems.mCount != 0) {
        CItemExt* entry = self->mVisibleEntries[(s8)self->mMaxVisibleItems];
        if (entry != 0) {
            GetItemMulti_ShowDetail(self, entry->field_00 >> 20, entry);
        } else {
            GetItemMulti_ShowDetail(self, 0, 0);
        }
    } else {
        u16 id = self->mVisibleItemIds[(s8)self->mMaxVisibleItems];
        if (id != 0) {
            GetItemMulti_ShowDetail(self, id, 0);
        } else {
            GetItemMulti_ShowDetail(self, 0, 0);
        }
    }
}

// Refresh the item-multi display for one item. arg2 = item id (or 0 to
// clear), arg3 = optional packed entry (when non-null its packed word
// supplies the id and category instead of arg2). Category dispatch:
//   2 -> equipped items, 3 -> rank item, 4-8 -> slotted items,
//   9 -> key item, 10 -> art/talent, 13 -> quest item.
void GetItemMulti_ShowDetail(CMenuGetItemMulti* self, int arg2, CItemExt* arg3) {
    GetItemMulti_ClearRanks(self);
    nw4r::lyt::Pane* pane = self->mLayout->GetRootPane()->FindPaneByName(
        "nul_inf", true);
    if (pane != 0) {
        // Retail shape: neg+or builds the non-zero test, the flags byte is
        // loaded in parallel (r5), and bit 0 is merged with a single rlwimi.
        pane->SetVisible(arg2 != 0);
    }
    if (arg2 != 0) {
        // Inline ternary: branches compute into the arg register (r3), the
        // u16 assignment masks once at the join (retail clrlwi r3, r3, 16).
        u16 id = (arg3 != 0) ? (arg3->field_00 >> 20) : (u32)arg2;
        setLayoutTextBoxNumber(self->mLayout, "txt_havenum", CItem_sumFamilyByte6(id));
        // Arms kept uncast: with the u8-typed helper return the ternary is
        // signed-typed, which keeps MWCC's clrlwi+cmpwi dispatch chain.
        u8 cat = (arg3 != 0) ? ((arg3->field_00 >> 16) & 0xf)
                             : BdatGetItemType(arg2);
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
        GetItemMulti_ShowEquipped(self, arg2, arg3);
        goto done;
    case4_8:
        GetItemMulti_ShowEntryDetailPanes(self, arg2, arg3);
        goto done;
    case3:
        GetItemMulti_ShowItemRank(self, arg2, arg3);
        goto done;
    case9:
        GetItemMulti_FillEntryRankPane(self, arg3);
        goto done;
    case10:
        GetItemMulti_ShowIdCategoryPanes(self, arg2);
        goto done;
    case13:
        GetItemMulti_ShowCat13(self, arg2);
    done:;
    }
}

// Reset the rank-item slot display: clear all slot ids/positions, hide the
// rank panes, then blank the per-rank text panes. Called before any
// rank-item category refresh.
void GetItemMulti_ClearRanks(CMenuGetItemMulti* self) {
    // Single 0.0f load is reused for all slot position clears.
    f32 zero = 0.0f;
    // Clear the 12 rank slots in two unrolled halves (ids + positions).
    for (int i = 0; i < 6; i++) {
        self->mPage.mItemIds[i] = 0;
        self->mPage.mPos[i].x = zero;
        self->mPage.mPos[i].y = zero;
        self->mPage.mPos[i].z = zero;
    }
    for (int i = 6; i < 12; i++) {
        self->mPage.mItemIds[i] = 0;
        self->mPage.mPos[i].x = zero;
        self->mPage.mPos[i].y = zero;
        self->mPage.mPos[i].z = zero;
    }

    // Hide every rank/slot pane; the two name panes are also blanked.
    nw4r::lyt::Pane* pane;
    pane = self->mLayout->GetRootPane()->FindPaneByName("nul_para_01", true);
    pane->SetVisible(false);
    pane = self->mLayout->GetRootPane()->FindPaneByName("nul_para_30", true);
    pane->SetVisible(false);
    pane = self->mLayout->GetRootPane()->FindPaneByName("nul_para_50", true);
    pane->SetVisible(false);
    pane = self->mLayout->GetRootPane()->FindPaneByName("nul_para_60", true);
    pane->SetVisible(false);
    pane = self->mLayout->GetRootPane()->FindPaneByName("nul_para_80", true);
    pane->SetVisible(false);
    // Retail blanks this pane through the text setter only (no explicit hide).
    LayoutSetTextBoxFmtValue(self->mLayout, "txt_havenum", "", 0);
    pane = self->mLayout->GetRootPane()->FindPaneByName("txt_para01", true);
    pane->SetVisible(false);
    pane = self->mLayout->GetRootPane()->FindPaneByName("txt_para04", true);
    pane->SetVisible(false);
    pane = self->mLayout->GetRootPane()->FindPaneByName("txt_para08", true);
    pane->SetVisible(false);
    pane = self->mLayout->GetRootPane()->FindPaneByName("txt_para10", true);
    pane->SetVisible(false);
    pane = self->mLayout->GetRootPane()->FindPaneByName("txt_para11", true);
    pane->SetVisible(false);
    pane = self->mLayout->GetRootPane()->FindPaneByName("txt_para22", true);
    pane->SetVisible(false);
    pane = self->mLayout->GetRootPane()->FindPaneByName("txt_para23", true);
    pane->SetVisible(false);
    pane = self->mLayout->GetRootPane()->FindPaneByName("txt_para24", true);
    pane->SetVisible(false);
    pane = self->mLayout->GetRootPane()->FindPaneByName("txt_para25", true);
    pane->SetVisible(false);
    pane = self->mLayout->GetRootPane()->FindPaneByName("txt_value02", true);
    pane->SetVisible(false);
    pane = self->mLayout->GetRootPane()->FindPaneByName("txt_value04", true);
    pane->SetVisible(false);
    pane = self->mLayout->GetRootPane()->FindPaneByName("txt_value07", true);
    pane->SetVisible(false);
    pane = self->mLayout->GetRootPane()->FindPaneByName("txt_value08", true);
    pane->SetVisible(false);
    pane = self->mLayout->GetRootPane()->FindPaneByName("txt_value09", true);
    pane->SetVisible(false);
    pane = self->mLayout->GetRootPane()->FindPaneByName("txt_value22", true);
    pane->SetVisible(false);
    pane = self->mLayout->GetRootPane()->FindPaneByName("pic_ethcol01", true);
    pane->SetVisible(false);
    pane = self->mLayout->GetRootPane()->FindPaneByName("pic_ethcol02", true);
    pane->SetVisible(false);
    pane = self->mLayout->GetRootPane()->FindPaneByName("pic_ethcol03", true);
    pane->SetVisible(false);
    pane = self->mLayout->GetRootPane()->FindPaneByName("txt_eth01", true);
    pane->SetVisible(false);
    pane = self->mLayout->GetRootPane()->FindPaneByName("txt_eth02", true);
    pane->SetVisible(false);
    pane = self->mLayout->GetRootPane()->FindPaneByName("txt_eth03", true);
    pane->SetVisible(false);
    pane = self->mLayout->GetRootPane()->FindPaneByName("txt_para50", true);
    pane->SetVisible(false);
    pane = self->mLayout->GetRootPane()->FindPaneByName("txt_para51", true);
    pane->SetVisible(false);
    pane = self->mLayout->GetRootPane()->FindPaneByName("txt_value50", true);
    pane->SetVisible(false);
    pane = self->mLayout->GetRootPane()->FindPaneByName("txt_value51", true);
    pane->SetVisible(false);
    pane = self->mLayout->GetRootPane()->FindPaneByName("txt_para60", true);
    pane->SetVisible(false);
    pane = self->mLayout->GetRootPane()->FindPaneByName("txt_para61", true);
    pane->SetVisible(false);
    pane = self->mLayout->GetRootPane()->FindPaneByName("txt_value60", true);
    pane->SetVisible(false);
    pane = self->mLayout->GetRootPane()->FindPaneByName("txt_value61", true);
    pane->SetVisible(false);
    // Retail blanks this pane through the text setter only (no explicit hide).
    LayoutSetTextBoxFmtValue(self->mLayout, "txt_value30", "", 0);

    // Blank the four per-rank text/slot panes.
    for (u8 i = 0; i < 4; ++i) {
        char buf[32];
        sprintf(buf, "txt_para%02d", i * 2 + 0x1f);
        LayoutSetTextBoxFmtValue(self->mLayout, buf, "", 0);
        sprintf(buf, "txt_value%02d", i + 0x1f);
        LayoutSetTextBoxFmtValue(self->mLayout, buf, "", 0);
    }
}

// Category-2 (equipped item) display update: show the equipped-item panes,
// look up the item's name/level strings from the font tables, scale the
// window size by the active character, then fill the stat text panes and
// refresh the slot colours. arg3 is the packed entry (unused here beyond
// forwarding to GetItemMulti_FillRankSlots).
void GetItemMulti_ShowEquipped(CMenuGetItemMulti* self, int arg2,
                                        CItemExt* arg3) {
    nw4r::lyt::Pane* pane;
    pane = self->mLayout->GetRootPane()->FindPaneByName("nul_para_01", true);
    pane->SetVisible(true);
    pane = self->mLayout->GetRootPane()->FindPaneByName("txt_para01", true);
    pane->SetVisible(true);
    pane = self->mLayout->GetRootPane()->FindPaneByName("txt_para04", true);
    pane->SetVisible(true);
    pane = self->mLayout->GetRootPane()->FindPaneByName("txt_para08", true);
    pane->SetVisible(true);
    pane = self->mLayout->GetRootPane()->FindPaneByName("txt_para10", true);
    pane->SetVisible(true);
    pane = self->mLayout->GetRootPane()->FindPaneByName("txt_para11", true);
    pane->SetVisible(true);
    pane = self->mLayout->GetRootPane()->FindPaneByName("txt_para22", true);
    pane->SetVisible(true);
    pane = self->mLayout->GetRootPane()->FindPaneByName("txt_para23", true);
    pane->SetVisible(true);
    pane = self->mLayout->GetRootPane()->FindPaneByName("txt_para24", true);
    pane->SetVisible(true);
    pane = self->mLayout->GetRootPane()->FindPaneByName("txt_para25", true);
    pane->SetVisible(true);
    pane = self->mLayout->GetRootPane()->FindPaneByName("txt_value02", true);
    pane->SetVisible(true);
    pane = self->mLayout->GetRootPane()->FindPaneByName("txt_value04", true);
    pane->SetVisible(true);
    pane = self->mLayout->GetRootPane()->FindPaneByName("txt_value07", true);
    pane->SetVisible(true);
    pane = self->mLayout->GetRootPane()->FindPaneByName("txt_value08", true);
    pane->SetVisible(true);
    pane = self->mLayout->GetRootPane()->FindPaneByName("txt_value09", true);
    pane->SetVisible(true);
    pane = self->mLayout->GetRootPane()->FindPaneByName("pic_ethcol01", true);
    pane->SetVisible(true);
    pane = self->mLayout->GetRootPane()->FindPaneByName("pic_ethcol02", true);
    pane->SetVisible(true);
    pane = self->mLayout->GetRootPane()->FindPaneByName("pic_ethcol03", true);
    pane->SetVisible(true);
    pane = self->mLayout->GetRootPane()->FindPaneByName("txt_eth01", true);
    pane->SetVisible(true);
    pane = self->mLayout->GetRootPane()->FindPaneByName("txt_eth02", true);
    pane->SetVisible(true);
    pane = self->mLayout->GetRootPane()->FindPaneByName("txt_eth03", true);
    pane->SetVisible(true);

    // Item-window font pointer + id -> table id (retail keeps both in saved
    // regs across all the lookups; results stay full-width u32 with masks
    // only at the use sites).
    u32 font = (u32)::cf::CfBdat::spItmWpnListFileData;
    BdatGetItemType(arg2);
    u16 tableId = BdatGetItemId(arg2);

    u16 w1 = BdatGetU16Direct((const void*)font, "dmg_low", (u16)tableId);
    u16 w2 = BdatGetU16Direct((const void*)font, "dmg_hi", (u16)tableId);
    u8 v97 = BdatGetU8Direct((const void*)font, "arm_phy", (u16)tableId);
    u8 va3 = BdatGetU8Direct((const void*)font, "arm_eth", (u16)tableId);
    u8 vaf = BdatGetU8Direct((const void*)font, "att_lev", (u16)tableId);
    // Window scale factor: byte lookup converted via the int->double magic
    // constant, normalized by a per-window constant.
    u8 vscale = BdatGetU8Direct((const void*)font, "speed", (u16)tableId);
    f32 total = (f32)(u32)vscale / 10.0f;
    u8 vgrd = BdatGetU8Direct((const void*)font, "grd_rate", (u16)tableId);
    u8 vbb = BdatGetU8Direct((const void*)font, "flag", (u16)tableId);

    if ((vbb & 4) != 0) {
        // Scale the name/level window by the active character's id-derived
        // factors, clamped to 999.
        void* mgr = CtrlObjectParam_GetCharEntry(1);
        u32 cid = CtrlObjectParam_GetArtsDataWord(mgr);
        w1 = (s32)(1.4f * (f64)((u16)w1 * (u16)cid));
        if ((u16)w1 >= 999) {
            w1 = 999;
        }
        cid = CtrlObjectParam_GetArtsDataWord(mgr);
        w2 = (s32)(1.5f * (f64)((u16)w2 * (u16)cid));
        if ((u16)w2 >= 999) {
            w2 = 999;
        }
    }

    // First non-empty per-level entry contributes its height to the total.
    for (u32 i = 1; i <= 10; ++i) {
        char buf[0x20];
        sprintf(buf, "equip_pc%d", (u8)i);
        if (BdatGetU8Direct((const void*)font, (const char*)buf, (u16)tableId) != 0) {
            total += GetFloatTableEntry((u8)i);
            break;
        }
    }

    BlendFloatAvgScale((u16)w1, (u16)w2, 0, total);

    ml::FixStr<32> str;
    char* midText = BdatTouchStringCell("MNU_item", "name", 11);
    str.format("%d%s%d", (u16)w1, midText, (u16)w2);
    LayoutSetTextBoxFmtValue(self->mLayout, "txt_value02", str.mString, 0);
    setLayoutTextBoxNumber(self->mLayout, "txt_value08", (u8)v97);
    setLayoutTextBoxNumber(self->mLayout, "txt_value09", (u8)va3);

    // Two more formatted lines: when the lookup byte is set they show the
    // 127/128 rank strings, otherwise the raw byte value.
    char* s7f = BdatTouchStringCell("MNU_item", "name", 0x7f);
    char* s80 = BdatTouchStringCell("MNU_item", "name", 0x80);
    if ((u8)vaf != 0) {
        str.format("%s%d%s", s7f, s80);
    } else {
        str.format("%d%s", (u8)vaf, s80);
    }
    LayoutSetTextBoxFmtValue(self->mLayout, "txt_value04", str.mString, 0);

    char* t7f = BdatTouchStringCell("MNU_item", "name", 0x7f);
    char* t80 = BdatTouchStringCell("MNU_item", "name", 0x80);
    if ((u8)vgrd != 0) {
        str.format("%s%d%s", t7f, t80);
    } else {
        str.format("%d%s", (u8)vgrd, t80);
    }
    LayoutSetTextBoxFmtValue(self->mLayout, "txt_value07", str.mString, 0);

    // Clear the description panes.
    LayoutSetTextBoxFmtValue(self->mLayout, "txt_value22", "", 0);
    LayoutSetTextBoxFmtValue(self->mLayout, "txt_para24", "", 0);
    LayoutSetTextBoxFmtValue(self->mLayout, "txt_para25", "", 0);

    // Slot colour/position refresh: shared slots use the default palette,
    // the two item-specific ones use the alt palette.
    PaneMatSetTevColorsByName(self->mLayout, "txt_para01", &gItemMultiPaneColor00, &gItemMultiPaneColor01);
    PaneMatSetTevColorsByName(self->mLayout, "txt_para10", &gItemMultiPaneColor00, &gItemMultiPaneColor01);
    PaneMatSetTevColorsByName(self->mLayout, "txt_para11", &gItemMultiPaneColor00, &gItemMultiPaneColor01);
    PaneMatSetTevColorsByName(self->mLayout, "txt_para04", &gItemMultiPaneColor00, &gItemMultiPaneColor01);
    PaneMatSetTevColorsByName(self->mLayout, "txt_para08", &gItemMultiPaneColor00, &gItemMultiPaneColor01);
    PaneMatSetTevColorsByName(self->mLayout, "txt_para22", &gItemMultiPaneColor02, &gItemMultiPaneColor03);
    PaneMatSetTevColorsByName(self->mLayout, "txt_para23", &gItemMultiPaneColor02, &gItemMultiPaneColor03);

    GetItemMulti_FillRankSlots(self, arg2, arg3);
}

// Category-4/8 (slotted item) display update: show the slotted-item panes,
// push the per-slot values into the item-text panes, then refresh the
// rank-window slots via GetItemMulti_FillRankSlots. arg3 is the packed entry; with no
// entry the id comes from arg2.
void GetItemMulti_ShowEntryDetailPanes(CMenuGetItemMulti* self, int arg2, CItemExt* arg3) {
    nw4r::lyt::Pane* pane;
    pane = self->mLayout->GetRootPane()->FindPaneByName("nul_para_01", true);
    pane->SetVisible(true);
    pane = self->mLayout->GetRootPane()->FindPaneByName("txt_para01", true);
    pane->SetVisible(true);
    pane = self->mLayout->GetRootPane()->FindPaneByName("txt_para04", true);
    pane->SetVisible(true);
    pane = self->mLayout->GetRootPane()->FindPaneByName("txt_para08", true);
    pane->SetVisible(true);
    pane = self->mLayout->GetRootPane()->FindPaneByName("txt_para10", true);
    pane->SetVisible(true);
    pane = self->mLayout->GetRootPane()->FindPaneByName("txt_para11", true);
    pane->SetVisible(true);
    pane = self->mLayout->GetRootPane()->FindPaneByName("txt_para22", true);
    pane->SetVisible(true);
    pane = self->mLayout->GetRootPane()->FindPaneByName("txt_para23", true);
    pane->SetVisible(true);
    pane = self->mLayout->GetRootPane()->FindPaneByName("txt_para24", true);
    pane->SetVisible(true);
    pane = self->mLayout->GetRootPane()->FindPaneByName("txt_para25", true);
    pane->SetVisible(true);
    pane = self->mLayout->GetRootPane()->FindPaneByName("txt_value08", true);
    pane->SetVisible(true);
    pane = self->mLayout->GetRootPane()->FindPaneByName("txt_value09", true);
    pane->SetVisible(true);
    pane = self->mLayout->GetRootPane()->FindPaneByName("txt_value22", true);
    pane->SetVisible(true);
    pane = self->mLayout->GetRootPane()->FindPaneByName("pic_ethcol01", true);
    pane->SetVisible(true);
    pane = self->mLayout->GetRootPane()->FindPaneByName("pic_ethcol02", true);
    pane->SetVisible(true);
    pane = self->mLayout->GetRootPane()->FindPaneByName("pic_ethcol03", true);
    pane->SetVisible(true);
    pane = self->mLayout->GetRootPane()->FindPaneByName("txt_eth01", true);
    pane->SetVisible(true);
    pane = self->mLayout->GetRootPane()->FindPaneByName("txt_eth02", true);
    pane->SetVisible(true);
    pane = self->mLayout->GetRootPane()->FindPaneByName("txt_eth03", true);
    pane->SetVisible(true);

    // Cache the item-window font pointer in a saved register (retail keeps it
    // in r24 across the pane calls); the rank block below re-reads the global.
    // Results stay full-width (retail saves each with a plain mr; the byte
    // mask happens only when passed as the u8 arg of setLayoutTextBoxNumber).
    u32 itemFont = (u32)::cf::CfBdat::spItmEquipListFileData;
    BdatGetItemType(arg2);
    u16 tableId = BdatGetItemId(arg2);
    u8 v1 = BdatGetU8Direct((const void*)itemFont, "arm_phy", (u16)tableId);
    u8 v2 = BdatGetU8Direct((const void*)itemFont, "arm_eth", (u16)tableId);
    u8 v3 = BdatGetU8Direct((const void*)itemFont, "eva_rate", (u16)tableId);
    setLayoutTextBoxNumber(self->mLayout, "txt_value08", (u8)v1);
    setLayoutTextBoxNumber(self->mLayout, "txt_value09", (u8)v2);
    setLayoutTextBoxNumber(self->mLayout, "txt_value22", (u8)v3);

    // Rank-category name for the slotted-item list header. Retail keeps the
    // raw lookup result in a saved reg AND takes a byte view once: the
    // comparison chain runs on the byte view (unsigned range test), while the
    // suffix switch below re-masks the raw value.
    u32 rankFont = (u32)::cf::CfBdat::spItmEquipListFileData;
    u8 rankVal = BdatGetU8Direct((const void*)rankFont, "arm_type",
                                BdatGetItemId(arg2));
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
    rankName = BdatTouchStringCell("MNU_item", "name", 0x32);
    goto rankDone;
rank2:
    rankName = BdatTouchStringCell("MNU_item", "name", 0x31);
    goto rankDone;
rank1:
    rankName = BdatTouchStringCell("MNU_item", "name", 0x30);
    goto rankDone;
rank4_13:
    rankName = BdatTouchStringCell("MNU_item", "name", 0x2e);
    goto rankDone;
rankDefault:
    rankName = 0;
rankDone:
    LayoutSetTextBoxFmtValue(self->mLayout, "txt_para24", rankName, 0);

    // Per-rank suffix string (0x77 + rank-4), table-dispatched for ranks 4-12.
    char* rankSuffix = 0;
    switch ((u8)rankVal - 4) {
    case 0:
        rankSuffix = BdatTouchStringCell("pc_arts", "name", 0x77);
        break;
    case 1:
        rankSuffix = BdatTouchStringCell("pc_arts", "name", 0x78);
        break;
    case 2:
        rankSuffix = BdatTouchStringCell("pc_arts", "name", 0x79);
        break;
    case 3:
        rankSuffix = BdatTouchStringCell("pc_arts", "name", 0x7a);
        break;
    case 4:
        rankSuffix = BdatTouchStringCell("pc_arts", "name", 0x7b);
        break;
    case 5:
        rankSuffix = BdatTouchStringCell("pc_arts", "name", 0x7c);
        break;
    case 6:
        rankSuffix = BdatTouchStringCell("pc_arts", "name", 0x7d);
        break;
    case 7:
        rankSuffix = BdatTouchStringCell("pc_arts", "name", 0x7e);
        break;
    case 8:
        rankSuffix = BdatTouchStringCell("pc_arts", "name", 0x7f);
        break;
    }
    LayoutSetTextBoxFmtValue(self->mLayout, "txt_para25", rankSuffix, 0);

    // Slot pane colour/position refresh; the shared entries use the default
    // palette, the two item-specific slots use the alt palette.
    PaneMatSetTevColorsByName(self->mLayout, "txt_para01", &gItemMultiPaneColor02, &gItemMultiPaneColor03);
    PaneMatSetTevColorsByName(self->mLayout, "txt_para10", &gItemMultiPaneColor00, &gItemMultiPaneColor01);
    PaneMatSetTevColorsByName(self->mLayout, "txt_para11", &gItemMultiPaneColor00, &gItemMultiPaneColor01);
    PaneMatSetTevColorsByName(self->mLayout, "txt_para04", &gItemMultiPaneColor02, &gItemMultiPaneColor03);
    PaneMatSetTevColorsByName(self->mLayout, "txt_para08", &gItemMultiPaneColor02, &gItemMultiPaneColor03);
    PaneMatSetTevColorsByName(self->mLayout, "txt_para22", &gItemMultiPaneColor00, &gItemMultiPaneColor01);
    PaneMatSetTevColorsByName(self->mLayout, "txt_para23", &gItemMultiPaneColor00, &gItemMultiPaneColor01);

    GetItemMulti_FillRankSlots(self, arg2, arg3);
}

// Category-3 (rank item) display update: show the five rank-item panes, fill
// the item-name/rank-text panes, and record the slot position for the cursor.
// arg3 is the packed entry (its id wins over arg2); with no entry the id
// comes from arg2.
void GetItemMulti_ShowItemRank(CMenuGetItemMulti* self, int arg2, CItemExt* arg3) {
    nw4r::lyt::Pane* pane = self->mLayout->GetRootPane()->FindPaneByName(
        "nul_para_50", true);
    pane->SetVisible(true);
    pane = self->mLayout->GetRootPane()->FindPaneByName("txt_para50", true);
    pane->SetVisible(true);
    pane = self->mLayout->GetRootPane()->FindPaneByName("txt_para51", true);
    pane->SetVisible(true);
    pane = self->mLayout->GetRootPane()->FindPaneByName("txt_value50", true);
    pane->SetVisible(true);
    pane = self->mLayout->GetRootPane()->FindPaneByName("txt_value51", true);
    pane->SetVisible(true);

    u32 itemId = (u32)arg2;
    if (arg3 != 0) {
        itemId = arg3->field_00 >> 20;
    }

    // No-entry path validates the id through the item table (result unused).
    if (arg3 == 0) {
        BdatGetItemType((u16)itemId);
    }

    u32 itemId2;
    if (arg3 != 0) {
        itemId2 = CItem_initItemImplInstances((CItemData*)arg3)->vf54((CItemData*)arg3);
    } else {
        itemId2 = BdatGetItemId((u16)itemId);
    }

    // Rank count is a nested ternary temporary inside the BdatTouchStringCell
    // argument (retail masks both arms into r0 and joins with a third
    // byte-mask straight into the argument register).
    char* rankName = BdatTouchStringCell(
        "MNU_item", "name",
        30 - ((u8)((arg3 != 0)
                       ? (u16)CItem_initItemImplInstances((CItemData*)arg3)->vf08((CItemData*)arg3)
                       : (u16)BdatGetU8Direct((const void*)::cf::CfBdat::spItmItemListFileData, "rankType",
                                            (u16)itemId)) -
              1));
    LayoutSetTextBoxFmtValue(self->mLayout, "txt_value50", rankName, 0);

    u16 v = BdatGetU16Direct(::cf::CfBdat::spBtlSkillListFileData, "max", (u16)itemId2);
    char* rankText = BdatTouchStringCell("MNU_shop", "name", 0xf);
    if (arg3 != 0) {
        setLayoutTextBoxNumber(self->mLayout, "txt_value51",
                      CItem_initItemImplInstances((CItemData*)arg3)->vf90((CItemData*)arg3));
    } else {
        LayoutSetTextBoxFmtValue(self->mLayout, "txt_value51",
                      (v == 1) ? "" : rankText, 0);
    }

    char* itemName = BdatGetPtrDirect(::cf::CfBdat::spBtlSkillListFileData, "name", (u16)itemId2);
    LayoutSetTextBoxFmtValue(self->mLayout, "txt_para51", itemName, 0);

    u8 n = (u8)BdatGetU8Direct((const void*)::cf::CfBdat::spBtlSkillListFileData, "attach", (u16)itemId2);
    char* rankStr = BdatTouchStringCell("MNU_item", "name",
                                  n + 0x15);
    LayoutSetTextBoxFmtValue(self->mLayout, "txt_para52", rankStr, 0);

    nw4r::lyt::Pane* slotPane = self->mLayout->GetRootPane()->FindPaneByName(
        "txt_para51", true);
    nw4r::lyt::Pane* refPane = self->mLayout->GetRootPane()->FindPaneByName(
        "nul_proportion", true);
    nw4r::math::VEC3 pos;
    ScalePaneWorldPos(&pos, slotPane, refPane, self->mLayout->GetRootPane());

    self->mPage.mItemIds[8] = (u16)itemId2;
    self->mPage.mBytesA8[8] = 0;
    // Materialize a copy of the position for the array write (retail keeps
    // the copy in a scratch VEC3 slot).
    nw4r::math::VEC3 posCopy = pos;
    self->mPage.mPos[8].x = posCopy.x;
    self->mPage.mPos[8].y = posCopy.y;
    self->mPage.mPos[8].z = posCopy.z;
}

void GetItemMulti_ShowIdCategoryPanes(CMenuGetItemMulti* self, int arg2) {
    // Show the five panes this category touches.
    nw4r::lyt::Pane* pane = self->mLayout->GetRootPane()->FindPaneByName(
        "nul_para_60", true);
    pane->SetVisible(true);
    pane = self->mLayout->GetRootPane()->FindPaneByName("txt_para60", true);
    pane->SetVisible(true);
    pane = self->mLayout->GetRootPane()->FindPaneByName("txt_para61", true);
    pane->SetVisible(true);
    pane = self->mLayout->GetRootPane()->FindPaneByName("txt_value60", true);
    pane->SetVisible(true);
    pane = self->mLayout->GetRootPane()->FindPaneByName("txt_value61", true);
    pane->SetVisible(true);

    // Item id -> table id, then fill the rank-name and description panes.
    void* tbl = ::cf::CfBdat::spItmCollectListFileData;
    BdatGetItemType(arg2);
    u16 tableId = BdatGetItemId(arg2);
    // Separate statement: retail keeps the lookup result live in r0 across
    // the BdatTouchStringCell arg setup (mr r0, r3 / clrlwi r5, r0, 24).
    u8 rankN = BdatGetU8Direct((const void*)tbl, "type", (u16)tableId);
    char* rankText = BdatTouchStringCell(
        "MNU_collect", "name", (u8)rankN);
    LayoutSetTextBoxFmtValue(self->mLayout, "txt_value60", rankText, 0);

    // Description text: fixed string 0x1a gets a formatted entry, otherwise
    // the plain key-item name from the font table.
    char* text;
    // Raw u32 kept in r0; the (u8) truncations happen at the use sites
    // (retail mr r0/r3 + rlwinm shape).
    u8 descId = BdatGetU8Direct((const void*)tbl, "mapID", (u16)tableId);
    if ((u8)descId == 0x1a) {
        text = BdatTouchStringCell("MNU_collect", "name", 20);
    } else {
        // Third arg rides in r5 from the lookup above (retail leaves it live).
        text = BdatGetPtrDirect(::cf::CfBdat::spFldMapListFileData, "name", (u8)descId);
    }
    LayoutSetTextBoxFmtValue(self->mLayout, "txt_value61", text, 0);
}

// Category-9 (rank item) display update: show the rank pane, render the rank
// name, and populate the per-slot id/position/flag arrays for the four slots.
// Guarded by arg3 != 0 (dispatch from GetItemMulti_ShowDetail).
void GetItemMulti_FillEntryRankPane(CMenuGetItemMulti* self, CItemExt* arg3) {
    if (arg3 == 0) {
        return;
    }

    nw4r::lyt::Pane* pane = self->mLayout->GetRootPane()->FindPaneByName(
        "nul_para_30", true);
    pane->SetVisible(true);

    u16 rankCount = (u16)CItem_initItemImplInstances((CItemData*)arg3)->vf08((CItemData*)arg3);
    char* rankName = BdatTouchStringCell("MNU_item",
                                   "name",
                                   30 - ((u8)rankCount - 1));
    LayoutSetTextBoxFmtValue(self->mLayout, "txt_value30", rankName, 0);

    // paneNo counts only the non-empty slots (its pane names are built from
    // it), while i indexes the impl slots -- MWCC keeps them as two counters.
    u8 paneNo = 0;
    for (u8 i = 0; i < 4; ++i) {
        // Signed id: MWCC records the truncation (clrlwi.) and emits the
        // signed `ble` skip (retail shape); a u16 here collapses to `beq`.
        s32 itemId = (u16)CItem_initItemImplInstances((CItemData*)arg3)->vf4C((CItemData*)arg3, i);
        if (itemId <= 0) {
            continue;
        }
        char paneName[32];
        sprintf(paneName, "txt_para%02d", paneNo * 2 + 0x1f);
        char* itemName = BdatGetPtrDirect(::cf::CfBdat::spBtlSkillListFileData, "name", itemId);
        LayoutSetTextBoxFmtValue(self->mLayout, paneName, itemName, 0);

        nw4r::lyt::Pane* slotPane =
            self->mLayout->GetRootPane()->FindPaneByName(paneName, true);
        nw4r::lyt::Pane* refPane = self->mLayout->GetRootPane()->FindPaneByName(
            "nul_proportion", true);
        nw4r::math::VEC3 pos;
        ScalePaneWorldPos(&pos, slotPane, refPane, self->mLayout->GetRootPane());

        u8 idx = (u8)(i + 8);
        if (idx < 0xc) {
            self->mPage.mItemIds[idx] = itemId;
        }
        if (idx < 0xc) {
            self->mPage.mBytesA8[idx] = 9;
        }
        // Materialize a copy of the position before the conditional array
        // write (retail keeps the copy in a scratch VEC3 slot).
        nw4r::math::VEC3 posCopy = pos;
        if (idx < 0xc) {
            self->mPage.mPos[idx].x = posCopy.x;
            self->mPage.mPos[idx].y = posCopy.y;
            self->mPage.mPos[idx].z = posCopy.z;
        }

        ml::FixStr<32> str;
        sprintf(paneName, "txt_value%02d", paneNo + 0x1f);
        u32 slotId = CItem_initItemImplInstances((CItemData*)arg3)->vf64((CItemData*)arg3, i);
        str.format("%d%s", (u16)slotId,
                   BdatTouchStringCell("MNU_item", "name", 0x21));
        LayoutSetTextBoxFmtValue(self->mLayout, paneName, str.mString, 0);

        ++paneNo;
    }
}

// Category-13 display update: show the pane, then build the rank-name string
// from the art/skill record tables (::cf::CfBdat::spItmArtsListFileData): three lookups keyed by
// the item's table id select the rank index, the record-set and a flag byte,
// and the per-character record (+0xE8/+0xE9 flags) switches the rank suffix
// between string 44 and 43.
void GetItemMulti_ShowCat13(CMenuGetItemMulti* self, int arg2) {
    nw4r::lyt::Pane* pane = self->mLayout->GetRootPane()->FindPaneByName(
        "nul_para_80", true);
    pane->SetVisible(true);

    void* tbl = (void*)::cf::CfBdat::spItmArtsListFileData;
    // No-entry path validates the id through the item table (result unused).
    BdatGetItemType(arg2);
    u16 tableId = BdatGetItemId(arg2);
    // Raw results kept unmasked (retail stores each with a plain mr and
    // truncates only at the use sites).
    u8 kindA = BdatGetU8Direct((const void*)tbl, "memory_type", (u16)tableId);
    u8 artIdx = BdatGetU8Direct((const void*)tbl, "pc_type", (u16)tableId);
    u8 selIdx = BdatGetU8Direct((const void*)tbl, "get_arts", (u16)tableId);
    CMenuGetItemCat13Record* rec;
    u8 sel = BdatGetU8ByTableKey("pc_arts", "idx",
                           selIdx);
    void* mgr = CtrlObjectParam_GetCharEntry(artIdx);
    // Record address: per-character stride 0x49 plus the doubled selector.
    u32 recBase = reinterpret_cast<u32>(mgr) +
                  (u8)CtrlObjectParam_GetCurrentRowKey(mgr) * 0x49;
    rec = reinterpret_cast<CMenuGetItemCat13Record*>(recBase + sel * 2);
    char* rankName =
        BdatTouchStringCell("MNU_item", "name", 0x2c);

    switch ((u8)kindA) {
    case 1:
        if (rec->flagE8 != 0) {
            rankName = BdatTouchStringCell("MNU_item",
                                     "name", 0x2b);
        }
        break;
    case 2:
        if ((rec->bitsE9 >> 7) & 1) {
            rankName = BdatTouchStringCell("MNU_item",
                                     "name", 0x2b);
        }
        break;
    case 3:
        if ((rec->bitsE9 >> 6) & 1) {
            rankName = BdatTouchStringCell("MNU_item",
                                     "name", 0x2b);
        }
        break;
    }

    LayoutSetTextBoxFmtValue(self->mLayout, "txt_para80", rankName, 0);
}

// Rank-window slot filler (slots 8-10) shared by the equipped/slotted-item
// handlers: for each of the three rank slots, look up the item's per-rank
// record and fill the slot's name/rank text panes, then record the id,
// position and rank data in the mPage (CItemBoxPage) arrays. With no packed entry the
// ids come from arg2 through the item table instead of the impl vtable.
void GetItemMulti_FillRankSlots(CMenuGetItemMulti* self, int arg2, CItemExt* arg3) {
    // Shared pane-name buffer and formatted-string slot (retail keeps both at
    // fixed frame offsets across both paths; the ctor-less FixStr(false) is
    // inert and only mString[0] is cleared up front).
    ml::FixStr<32> str(false);
    str.mString[0] = 0;
    str.mLength = 0;
    char buf[32];
    if (arg3 != NULL && arg3->field_00 != 0) {
        u16 count = (u16)CItem_initItemImplInstances((CItemData*)arg3)->vf30((CItemData*)arg3);
        nw4r::math::VEC3 pos;
        nw4r::math::VEC3 posCopy;
        for (u8 i = 0; i < 3; ++i) {
            sprintf(buf, "pic_ethcol%02d", (u8)i + 1);
            nw4r::lyt::Pane* pane =
                self->mLayout->GetRootPane()->FindPaneByName(buf, true);
            // Per-slot rank data (zeroed when the slot is inactive).
            u16 slotItemId = 0;
            u8 slotRank = 0;
            s16 slotVal = 0;
            if ((u8)i < count) {
                void* texture = 0;
                pane->SetVisible(true);
                CMenuGetItemRankSlot* slot = reinterpret_cast<CMenuGetItemRankSlot*>(
                    CItem_initItemImplInstances((CItemData*)arg3)->vf2C((CItemData*)arg3, i));
                if (slot != 0 && (slot->bits04 & 1) != 0) {
                    slotItemId = (slot->bits04 >> 4) & 0xFFF;
                    void* fontTbl = ::cf::CfBdat::spBtlSkillListFileData;
                    slotRank = (slot->bits04 >> 7) & 7;
                    slotVal = (s16)((slot->bits04 >> 10) & 0x7FF);
                    char* rankStr = BdatTouchStringCell(
                        "MNU_item", "name",
                        30 - (slotRank - 1));
                    switch ((u8)BdatGetU8Direct(
                        (const void*)fontTbl, "atr_type", slotItemId)) {
                    case 0:
                        texture = CUICfManager_getArcResourceAccessor()->GetResource(
                            nw4r::lyt::ArcResourceAccessor::RES_TYPE_TEXTURE,
                            "mf00_reg30_crys00.tpl", NULL);
                        break;
                    case 4:
                        texture = CUICfManager_getArcResourceAccessor()->GetResource(
                            nw4r::lyt::ArcResourceAccessor::RES_TYPE_TEXTURE,
                            "mf00_reg30_crys01.tpl", NULL);
                        break;
                    case 5:
                        texture = CUICfManager_getArcResourceAccessor()->GetResource(
                            nw4r::lyt::ArcResourceAccessor::RES_TYPE_TEXTURE,
                            "mf00_reg30_crys02.tpl", NULL);
                        break;
                    case 6:
                        texture = CUICfManager_getArcResourceAccessor()->GetResource(
                            nw4r::lyt::ArcResourceAccessor::RES_TYPE_TEXTURE,
                            "mf00_reg30_crys03.tpl", NULL);
                        break;
                    case 7:
                        texture = CUICfManager_getArcResourceAccessor()->GetResource(
                            nw4r::lyt::ArcResourceAccessor::RES_TYPE_TEXTURE,
                            "mf00_reg30_crys04.tpl", NULL);
                        break;
                    case 8:
                        texture = CUICfManager_getArcResourceAccessor()->GetResource(
                            nw4r::lyt::ArcResourceAccessor::RES_TYPE_TEXTURE,
                            "mf00_reg30_crys05.tpl", NULL);
                        break;
                    case 9:
                        texture = CUICfManager_getArcResourceAccessor()->GetResource(
                            nw4r::lyt::ArcResourceAccessor::RES_TYPE_TEXTURE,
                            "mf00_reg30_crys06.tpl", NULL);
                        break;
                    }
                    str.format("%s",
                               BdatGetPtrDirect(fontTbl, "name",
                                             slotItemId),
                               rankStr);
                } else {
                    texture = CUICfManager_getArcResourceAccessor()->GetResource(
                        nw4r::lyt::ArcResourceAccessor::RES_TYPE_TEXTURE,
                        "mf00_reg30_crys00.tpl", NULL);
                    str.format("%s",
                               BdatTouchStringCell("MNU_item",
                                             "name", 0x2a));
                }
                sprintf(buf, "txt_eth%02d", (u8)i + 1);
                LayoutSetTextBoxFmtValue(self->mLayout, buf, str.mString, 0);
                if (texture != 0) {
                    PaneSetTexPalette(pane, (TPLPalette*)texture);
                }
            } else {
                pane->SetVisible(false);
                sprintf(buf, "txt_eth%02d", (u8)i + 1);
                LayoutSetTextBoxFmtValue(self->mLayout, buf, "", 0);
            }

            nw4r::lyt::Pane* refPane = self->mLayout->GetRootPane()->FindPaneByName(
                "nul_proportion", true);
            nw4r::math::VEC3 pos;
            ScalePaneWorldPos(&pos, pane, refPane, self->mLayout->GetRootPane());
            u8 idx = (u8)(i + 8);
            if (idx < 0xc) {
                self->mPage.mItemIds[idx] = slotItemId;
            }
            if (idx < 0xc) {
                self->mPage.mBytesA8[idx] = 3;
            }
            if (idx < 0xc) {
                self->mPage.mBytesB4[idx] = slotRank;
            }
            if (idx < 0xc) {
                self->mPage.mValues[idx] = slotVal;
            }
            nw4r::math::VEC3 posCopy = pos;
            if (idx < 0xc) {
                self->mPage.mPos[idx].x = posCopy.x;
                self->mPage.mPos[idx].y = posCopy.y;
                self->mPage.mPos[idx].z = posCopy.z;
            }
        }
    } else {
        void* itemTable = (void*)LookupItemTypeTable(arg2);
        u16 tableId = BdatGetItemId(arg2);
        u8 count =
            (u8)BdatGetU8Direct((const void*)itemTable, "jwl_slot", (u16)tableId);
        u16 slotId = 0;
        u8 rankCnt = 0;
        u16 altFlag = 0;
        nw4r::math::VEC3 pos;
        nw4r::math::VEC3 posCopy;
        for (u8 j = 0; j < 3; ++j) {
            sprintf(buf, "pic_ethcol%02d", (u8)j + 1);
            nw4r::lyt::Pane* pane =
                self->mLayout->GetRootPane()->FindPaneByName(buf, true);
            if ((u8)j < count) {
                void* texture = 0;
                pane->SetVisible(true);
                str.format("jwl_skill%d", (u8)j + 1);
                u16 id = BdatGetU16Direct(itemTable, str.mString, (u16)tableId);
                if (id == 0) {
                    texture = CUICfManager_getArcResourceAccessor()->GetResource(
                        nw4r::lyt::ArcResourceAccessor::RES_TYPE_TEXTURE,
                        "mf00_reg30_crys00.tpl", NULL);
                    str.format("%s",
                               BdatTouchStringCell("MNU_item",
                                             "name", 0x2a));
                } else {
                    altFlag = BdatGetU8Direct((const void*)::cf::CfBdat::spItmItemListFileData,
                                            "percent", id);
                    rankCnt = BdatGetU8Direct((const void*)::cf::CfBdat::spItmItemListFileData,
                                            "rankType", id);
                    void* fontTbl2 = ::cf::CfBdat::spBtlSkillListFileData;
                    switch ((u8)BdatGetU8Direct(
                        (const void*)fontTbl2, "atr_type",
                        (u16)BdatGetItemId(id))) {
                    case 0:
                        texture = CUICfManager_getArcResourceAccessor()->GetResource(
                            nw4r::lyt::ArcResourceAccessor::RES_TYPE_TEXTURE,
                            "mf00_reg30_crys00.tpl", NULL);
                        break;
                    case 4:
                        texture = CUICfManager_getArcResourceAccessor()->GetResource(
                            nw4r::lyt::ArcResourceAccessor::RES_TYPE_TEXTURE,
                            "mf00_reg30_crys01.tpl", NULL);
                        break;
                    case 5:
                        texture = CUICfManager_getArcResourceAccessor()->GetResource(
                            nw4r::lyt::ArcResourceAccessor::RES_TYPE_TEXTURE,
                            "mf00_reg30_crys02.tpl", NULL);
                        break;
                    case 6:
                        texture = CUICfManager_getArcResourceAccessor()->GetResource(
                            nw4r::lyt::ArcResourceAccessor::RES_TYPE_TEXTURE,
                            "mf00_reg30_crys03.tpl", NULL);
                        break;
                    case 7:
                        texture = CUICfManager_getArcResourceAccessor()->GetResource(
                            nw4r::lyt::ArcResourceAccessor::RES_TYPE_TEXTURE,
                            "mf00_reg30_crys04.tpl", NULL);
                        break;
                    case 8:
                        texture = CUICfManager_getArcResourceAccessor()->GetResource(
                            nw4r::lyt::ArcResourceAccessor::RES_TYPE_TEXTURE,
                            "mf00_reg30_crys05.tpl", NULL);
                        break;
                    case 9:
                        texture = CUICfManager_getArcResourceAccessor()->GetResource(
                            nw4r::lyt::ArcResourceAccessor::RES_TYPE_TEXTURE,
                            "mf00_reg30_crys06.tpl", NULL);
                        break;
                    }
                    str.format("%s", BuildItemNameStr(id));
                    slotId = BdatGetItemId(id);
                }
                sprintf(buf, "txt_eth%02d", (u8)j + 1);
                LayoutSetTextBoxFmtValue(self->mLayout, buf, str.mString, 0);
                if (texture != 0) {
                    PaneSetTexPalette(pane, (TPLPalette*)texture);
                }
            } else {
                sprintf(buf, "txt_eth%02d", (u8)j + 1);
                LayoutSetTextBoxFmtValue(self->mLayout, buf, "", 0);
                pane->SetVisible(false);
            }

            nw4r::lyt::Pane* refPane = self->mLayout->GetRootPane()->FindPaneByName(
                "nul_proportion", true);
            nw4r::math::VEC3 pos;
            ScalePaneWorldPos(&pos, pane, refPane, self->mLayout->GetRootPane());
            u8 idx = (u8)(j + 8);
            if (idx < 0xc) {
                self->mPage.mItemIds[idx] = slotId;
            }
            if (idx < 0xc) {
                self->mPage.mBytesA8[idx] = 3;
            }
            if (idx < 0xc) {
                self->mPage.mBytesB4[idx] = rankCnt;
            }
            if (idx < 0xc) {
                self->mPage.mValues[idx] = altFlag;
            }
            nw4r::math::VEC3 posCopy = pos;
            if (idx < 0xc) {
                self->mPage.mPos[idx].x = posCopy.x;
                self->mPage.mPos[idx].y = posCopy.y;
                self->mPage.mPos[idx].z = posCopy.z;
            }
        }
    }
}

// Pad-input handler for the item-select window (Move state 2). Reads the pad
// and dispatches on five inputs: up/down (pane-list navigation), the turbo
// held bits (auto-repeat selection scan in the slot window, or the pane-list
// wrap-around) and the A button (open the slot window for the current
// selection). The classic-controller bit layout is swapped per CfGameManager.
void GetItemMulti_HandlePad(CMenuGetItemMulti* self) {
    cf::CfPadData* pad = cf::CfGameManager::getCfPadData();

    if ((pad->mPad.mHeldButtonFlags & 0x1E000) == 0) {
        self->field_200 = 0;
    }
    if (self->field_200 != 0) {
        return;
    }

    u32 up, down, curHeld, maskHeld, aPressed;
    if (cf::CfGameManager::isClassicController(-1) != 0) {
        curHeld = (pad->mTurboPressButtonFlags & 0x10000) |
                  (pad->mTurboPressButtonFlags & 0x8);
        maskHeld = pad->mTurboPressButtonFlags & 0x8004;
        up = (pad->mPad.mPressedButtonFlags >> 21) & 1;
        down = (pad->mPad.mPressedButtonFlags >> 22) & 1;
        aPressed = (pad->mPad.mPressedButtonFlags >> 9) & 1;
        maskHeld = maskHeld != 0;
        curHeld = curHeld != 0;
    } else {
        curHeld = (pad->mTurboPressButtonFlags & 0x10000) |
                  (pad->mTurboPressButtonFlags & 0x8);
        maskHeld = pad->mTurboPressButtonFlags & 0x8004;
        up = (pad->mPad.mPressedButtonFlags >> 4) & 1;
        down = (pad->mPad.mPressedButtonFlags >> 5) & 1;
        aPressed = (pad->mPad.mPressedButtonFlags >> 9) & 1;
        maskHeld = maskHeld != 0;
        curHeld = curHeld != 0;
    }

    if (up) {
        // Up over the close pane: a live system window wins; otherwise open
        // the rank-up window for the selected rank item (or refuse with a
        // buzz when the pane list is not in the rank state).
        if (self->mSystemWindow.isOpened() != 0) {
            if (self->mSystemWindow.isActive() == 0) {
                return;
            }
            self->mSystemWindow.advancePhase3();
            self->mCursor.SetVisible(1);
            return;
        }
        if (self->field_20C != 0) {
            return;
        }
        if ((s8)self->mMaxVisibleItems == 5) {
            if (self->mHasSpecialItem != 0) {
                playUISound(5);
                return;
            }
            // Retail materializes the string-pool base once (r30) and offsets
            // all six label arguments from it.
            self->field_1F8 = 8;
            char* n1 = BdatTouchStringCell("MNU_sysmes", "name", 0x1c);
            char* n2 = BdatTouchStringCell("MNU_sysmes", "name", 0x1d);
            char* n3 = BdatTouchStringCell("MNU_sysmes", "name", 0x1e);
            self->mSystemWindow.switchKindPane(2);
            self->mSystemWindow.setKindContent(reinterpret_cast<char*>(n1), 0);
            self->mSystemWindow.setTwoTextValues(reinterpret_cast<char*>(n2), reinterpret_cast<char*>(n3));
            self->mSystemWindow.applyKind(0);
            self->mSystemWindow.openPhase1();
            self->mCursor.SetVisible(0);
            self->field_1F4 = 1;
            return;
        }
        self->field_1F8 = 4;
        playUISound(3);
        return;
    }

    if (down) {
        // Down over a selected slot closes the slot window and snaps the
        // cursor back onto the pane list.
        if (self->field_20C == 0) {
            return;
        }
        if (self->mSystemWindow.isOpened() != 0) {
            if (self->mSystemWindow.isActive() == 0) {
                return;
            }
            self->mSystemWindow.advancePhase3();
            self->mCursor.SetVisible(1);
            return;
        }
        u8 mv = self->mMaxVisibleItems;
        self->field_20C = 0;
        {
            char buf[32];
            s8 m = (s8)mv;
            if (m == 5) {
                sprintf(buf, "txt_close00");
            } else if (m == 4) {
                sprintf(buf, "txt_item%02d", m + 1);
            } else {
                sprintf(buf, "nul_item%02d", m + 1);
            }
            nw4r::lyt::Pane* pane = self->mLayout->GetRootPane()->FindPaneByName(buf, true);
            nw4r::lyt::Pane* refPane = self->mLayout->GetRootPane()->FindPaneByName(
                "nul_proportion", true);
            nw4r::math::VEC3 pos;
            ScalePaneWorldPos(&pos, pane, refPane, self->mLayout->GetRootPane());
            self->mCursor.setRootPaneTranslate(&pos);
        }
        playUISound(6);
        return;
    }

    if (maskHeld) {
        // Left/right held in the slot window: scan the slot group backwards
        // for the previous filled slot; on the pane list, wrap the selection
        // backwards and re-centre the cursor.
        if (self->mSystemWindow.isOpened() != 0) {
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
                u16 slotId = (idx < 0xc) ? self->mPage.mItemIds[idx] : 0;
                if (slotId != 0) {
                    self->field_20E = (u8)cand;
                    break;
                }
                cand = (s8)(cand - 1);
            } while (cand != orig);
            u8 posIdx = (u8)((s8)self->field_20D * 4 + self->field_20E);
            nw4r::math::VEC3 pos;
            if (posIdx < 0xc) {
                pos.x = self->mPage.mPos[posIdx].x;
                pos.y = self->mPage.mPos[posIdx].y;
                pos.z = self->mPage.mPos[posIdx].z;
            }
            self->mCursor.setRootPaneTranslate(&pos);
            return;
        }

        if ((s8)self->mMaxVisibleItems == 4) {
            self->mMaxVisibleItems = (u8)(self->mVisibleItemCount - 1);
        } else {
            self->mMaxVisibleItems = self->mMaxVisibleItems - 1;
            if ((s8)self->mMaxVisibleItems < 0) {
                self->mMaxVisibleItems = 5;
            }
        }
        if (self->mItems.mCount != 0) {
            CItemExt* entry = self->mVisibleEntries[(s8)self->mMaxVisibleItems];
            if (entry != 0) {
                GetItemMulti_ShowDetail(self, entry->field_00 >> 20, entry);
            } else {
                GetItemMulti_ShowDetail(self, 0, 0);
            }
        } else {
            u16 id = self->mVisibleItemIds[(s8)self->mMaxVisibleItems];
            if (id != 0) {
                GetItemMulti_ShowDetail(self, id, 0);
            } else {
                GetItemMulti_ShowDetail(self, 0, 0);
            }
        }
        {
            char buf[32];
            s8 m = (s8)self->mMaxVisibleItems;
            if (m == 5) {
                sprintf(buf, "txt_close00");
            } else if (m == 4) {
                sprintf(buf, "txt_item%02d", m + 1);
            } else {
                sprintf(buf, "nul_item%02d", m + 1);
            }
            nw4r::lyt::Pane* pane = self->mLayout->GetRootPane()->FindPaneByName(buf, true);
            nw4r::lyt::Pane* refPane = self->mLayout->GetRootPane()->FindPaneByName(
                "nul_proportion", true);
            nw4r::math::VEC3 pos;
            ScalePaneWorldPos(&pos, pane, refPane, self->mLayout->GetRootPane());
            self->mCursor.setRootPaneTranslate(&pos);
        }
        playUISound(1);
        return;
    }

    if (curHeld) {
        // Right held: scan the slot group forwards; on the pane list, wrap
        // the selection forwards.
        if (self->mSystemWindow.isOpened() != 0) {
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
                u16 slotId = (idx < 0xc) ? self->mPage.mItemIds[idx] : 0;
                if (slotId != 0) {
                    self->field_20E = (u8)cand;
                    break;
                }
                cand = (s8)(cand + 1);
            } while (cand != orig);
            u8 posIdx = (u8)((s8)self->field_20D * 4 + self->field_20E);
            nw4r::math::VEC3 pos;
            if (posIdx < 0xc) {
                pos.x = self->mPage.mPos[posIdx].x;
                pos.y = self->mPage.mPos[posIdx].y;
                pos.z = self->mPage.mPos[posIdx].z;
            }
            self->mCursor.setRootPaneTranslate(&pos);
            return;
        }

        if ((s8)self->mMaxVisibleItems == 5) {
            self->mMaxVisibleItems = 0;
        } else {
            self->mMaxVisibleItems = self->mMaxVisibleItems + 1;
            if ((s8)self->mMaxVisibleItems >= (s32)self->mVisibleItemCount &&
                (s8)self->mMaxVisibleItems != 5) {
                self->mMaxVisibleItems = 4;
            }
        }
        if (self->mItems.mCount != 0) {
            CItemExt* entry = self->mVisibleEntries[(s8)self->mMaxVisibleItems];
            if (entry != 0) {
                GetItemMulti_ShowDetail(self, entry->field_00 >> 20, entry);
            } else {
                GetItemMulti_ShowDetail(self, 0, 0);
            }
        } else {
            u16 id = self->mVisibleItemIds[(s8)self->mMaxVisibleItems];
            if (id != 0) {
                GetItemMulti_ShowDetail(self, id, 0);
            } else {
                GetItemMulti_ShowDetail(self, 0, 0);
            }
        }
        {
            char buf[32];
            s8 m = (s8)self->mMaxVisibleItems;
            if (m == 5) {
                sprintf(buf, "txt_close00");
            } else if (m == 4) {
                sprintf(buf, "txt_item%02d", m + 1);
            } else {
                sprintf(buf, "nul_item%02d", m + 1);
            }
            nw4r::lyt::Pane* pane = self->mLayout->GetRootPane()->FindPaneByName(buf, true);
            nw4r::lyt::Pane* refPane = self->mLayout->GetRootPane()->FindPaneByName(
                "nul_proportion", true);
            nw4r::math::VEC3 pos;
            ScalePaneWorldPos(&pos, pane, refPane, self->mLayout->GetRootPane());
            self->mCursor.setRootPaneTranslate(&pos);
        }
        playUISound(1);
        return;
    }

    if (aPressed) {
        // A opens the slot window for the currently selected slot (or, when
        // no slot window is open yet, scans the rank slots for the first
        // filled one).
        nw4r::lyt::Pane* pane = self->mLayout->GetRootPane()->FindPaneByName(
            "nul_inf", true);
        if (pane->IsVisible()) {
            if (self->field_20C != 0) {
                if (self->mSystemWindow.isOpened() != 0) {
                    if (self->mSystemWindow.isActive() == 0) {
                        return;
                    }
                    self->mSystemWindow.advancePhase3();
                    self->mCursor.SetVisible(1);
                    return;
                }
                u8 idx = (u8)((s8)self->field_20D * 4 + self->field_20E);
                u16 slotId = (idx < 0xc) ? self->mPage.mItemIds[idx] : 0;
                if (slotId == 0) {
                    return;
                }
                u8 flag = (idx < 0xc) ? self->mPage.mBytesA8[idx] : 0;
                char* text;
                if (flag == 3) {
                    text = self->mPage.GetItemName(idx);
                } else {
                    text = BdatGetPtrDirect(reinterpret_cast<const void*>(gpItemMultiSkillBdat),
                                         "help", slotId);
                }
                self->mSystemWindow.switchKindPane(0);
                self->mSystemWindow.setKindContent(reinterpret_cast<char*>(text), 0);
                self->mSystemWindow.applyKind(1);
                self->mSystemWindow.openPhase1();
                self->mCursor.SetVisible(0);
                return;
            }

            // No slot window open: pick the first filled rank slot.
            if (self->mSystemWindow.isOpened() != 0) {
                return;
            }
            u8 sel = 0;
            u32 found = 0;
            for (u8 i = 0; i < 12; ++i) {
                if (self->mPage.mItemIds[i] != 0) {
                    sel = i;
                    self->mPage.mSelSlot = i;
                    if (self->mPage.mItemIds[8] != 0) {
                        sel = 8;
                        self->mPage.mSelSlot = 8;
                    }
                    found = 1;
                    break;
                }
            }
            if (found == 0) {
                playUISound(5);
                return;
            }
            self->field_20C = 1;
            // Retail converts the selected slot via the unsigned int->double
            // magic and multiplies in single precision.
            u8 sel2 = self->mPage.mSelSlot;
            f64 selF = (f64)(u32)sel2;
            self->field_20D = (u8)(s32)(0.25f * (f32)selF);
            self->field_20E = (u8)(sel2 - (s8)self->field_20D * 4);
            nw4r::math::VEC3 pos;
            if (sel2 < 0xc) {
                pos.x = self->mPage.mPos[sel2].x;
                pos.y = self->mPage.mPos[sel2].y;
                pos.z = self->mPage.mPos[sel2].z;
            }
            self->mCursor.setRootPaneTranslate(&pos);
            playUISound(2);
            return;
        }
        if (self->mSystemWindow.isOpened() == 0) {
            playUISound(5);
        }
    }
}

// Confirm/close handler for the item-select state (Move state 4). Sweeps the
// visible item list and drops entries that are no longer special (or the
// single currently-selected item), then refreshes the display and repositions
// the cursor. Category/level classification shared with Init's special2 logic:
//   2-9 -> special iff CItemBlock_countEmpty(cat) != 0
//   10-13 -> special iff level in [1, 99), or level 0 with CItemBlock_countEmpty(cat)
void GetItemMulti_PruneEntries(CMenuGetItemMulti* self) {
    if ((s8)self->mMaxVisibleItems == 4) {
        // All four panes visible: sweep every visible item, drop the ones
        // whose grade requirement is now satisfied.
        if (self->mItems.mCount != 0) {
            for (u8 i = 0; i < self->mVisibleItemCount; ++i) {
                CItemExt* entry = self->mVisibleEntries[i];
                if (entry == 0) {
                    continue;
                }
                if (self->mPaneVisible[i] != 0) {
                    continue;
                }
                u32 packed = entry->field_00;
                u32 id = packed >> 20;
                // Category nibble falls back to the table lookup when 0;
                // retail masks the merged value to 16 bits before use.
                u32 rawCat = (packed >> 12) & 0xF;
                if (rawCat == 0) {
                    rawCat = (u16)BdatGetItemType(id);
                }
                u32 cat = rawCat & 0xFFFF;
                BdatGetItemId(id);
                int special = 0;
                if (cat - 2 <= 7u) {
                    if (CItemBlock_countEmpty(cat) != 0) {
                        special = 1;
                    }
                } else if (cat - 10 <= 3u) {
                    int lvl = CItem_sumFamilyByte6(id);
                    if (lvl < 1) {
                        if (CItemBlock_countEmpty(cat) != 0) {
                            special = 1;
                        }
                    } else if (lvl < 0x63) {
                        special = 1;
                    }
                }
                if (special != 0) {
                    CItemData_lookupOrAlloc(entry, 0);
                    self->mVisibleEntries[i] = 0;
                    UIWin_BuildFlagBuf(2, entry->field_00 >> 20, 0);
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
                u32 cat = (u16)BdatGetItemType(id);
                BdatGetItemId(id);
                int special = 0;
                if (cat - 2 <= 7u) {
                    if (CItemBlock_countEmpty(cat) != 0) {
                        special = 1;
                    }
                } else if (cat - 10 <= 3u) {
                    int lvl = CItem_sumFamilyByte6(id);
                    if (lvl < 1) {
                        if (CItemBlock_countEmpty(cat) != 0) {
                            special = 1;
                        }
                    } else if (lvl < 0x63) {
                        special = 1;
                    }
                }
                if (special != 0) {
                    CItem_thunkAllocRecord(id, 1);
                    self->mVisibleItemIds[i] = 0;
                    UIWin_BuildFlagBuf(2, id, 0);
                }
            }
        }

        // Refresh the display and reposition the cursor on the active item.
        GetItemMulti_RefreshList(self);
        {
            char buf[32];
            s8 m = (s8)self->mMaxVisibleItems;
            if (m == 5) {
                sprintf(buf, "txt_close00");
            } else if (m == 4) {
                sprintf(buf, "txt_item%02d", m + 1);
            } else {
                sprintf(buf, "nul_item%02d", m + 1);
            }
            nw4r::lyt::Pane* pane = self->mLayout->GetRootPane()->FindPaneByName(buf, true);
            nw4r::lyt::Pane* refPane = self->mLayout->GetRootPane()->FindPaneByName(
                "nul_proportion", true);
            nw4r::math::VEC3 pos;
            ScalePaneWorldPos(&pos, pane, refPane, self->mLayout->GetRootPane());
            self->mCursor.setRootPaneTranslate(&pos);
        }
        if (self->mVisibleItemCount == 0) {
            self->mCursor.SetVisible(0);
            self->field_1F8 = 3;
            playUISound(0x89);
        } else {
            self->field_1F8 = 2;
        }
        return;
    }

    // Single-item path: the selected index is the sign-extended max-visible
    // count (re-read from the field; no calls intervene before these uses so
    // MWCC keeps it in a volatile register).
    if (self->mItems.mCount != 0) {
        CItemExt* entry = self->mVisibleEntries[(s8)self->mMaxVisibleItems];
        if (entry == 0) {
            return;
        }
        if (self->mPaneVisible[(s8)self->mMaxVisibleItems] == 0) {
            CItemData_lookupOrAlloc(entry, 0);
            self->mVisibleEntries[(s8)self->mMaxVisibleItems] = 0;
            UIWin_BuildFlagBuf(2, entry->field_00 >> 20, 0);
            GetItemMulti_RefreshList(self);
            {
                char buf[32];
                s8 m = (s8)self->mMaxVisibleItems;
                if (m == 5) {
                    sprintf(buf, "txt_close00");
                } else if (m == 4) {
                    sprintf(buf, "txt_item%02d", m + 1);
                } else {
                    sprintf(buf, "nul_item%02d", m + 1);
                }
                nw4r::lyt::Pane* pane = self->mLayout->GetRootPane()->FindPaneByName(buf, true);
                nw4r::lyt::Pane* refPane = self->mLayout->GetRootPane()->FindPaneByName(
                    "nul_proportion", true);
                nw4r::math::VEC3 pos;
                ScalePaneWorldPos(&pos, pane, refPane, self->mLayout->GetRootPane());
                self->mCursor.setRootPaneTranslate(&pos);
            }
            if (self->mVisibleItemCount == 0) {
                self->mCursor.SetVisible(0);
                self->field_1F8 = 3;
                playUISound(0x89);
            } else {
                self->field_1F8 = 2;
            }
            return;
        }
        // Selected item's pane is visible: grade-up handling. Rank categories
        // (10-13) below level 99 open the 5-window grade setup; at 99+ they
        // switch to the 3-window rank-max form. Other categories open the
        // same 5-window setup.
        u32 packedSel = entry->field_00;
        u32 cat = (packedSel >> 12) & 0xF;
        if ((u8)(cat - 10) <= 3u) {
            if (CItem_sumFamilyByte6(packedSel >> 20) >= 0x63) {
                self->field_1F8 = 2;
                char* name = BdatTouchStringCell("MNU_sysmes",
                                           "name", 3);
                self->mSystemWindow.switchKindPane(0);
                self->mSystemWindow.setKindContent(reinterpret_cast<char*>(name), 0);
                self->mSystemWindow.applyKind(1);
                self->mSystemWindow.openPhase1();
                self->mCursor.SetVisible(0);
            } else {
                self->field_1F8 = 5;
                char* n2 = BdatTouchStringCell("MNU_sysmes",
                                         "name", 2);
                char* n4 = BdatTouchStringCell("MNU_sysmes",
                                         "name", 4);
                char* n5 = BdatTouchStringCell("MNU_sysmes",
                                         "name", 5);
                self->mSystemWindow.switchKindPane(2);
                self->mSystemWindow.setKindContent(reinterpret_cast<char*>(n2), 0);
                self->mSystemWindow.setTwoTextValues(reinterpret_cast<char*>(n4), reinterpret_cast<char*>(n5));
                self->mSystemWindow.applyKind(0);
                self->mSystemWindow.openPhase1();
                self->mCursor.SetVisible(0);
                self->field_1F4 = 1;
            }
        } else {
            self->field_1F8 = 5;
            char* n2 = BdatTouchStringCell("MNU_sysmes",
                                     "name", 2);
            char* n4 = BdatTouchStringCell("MNU_sysmes",
                                     "name", 4);
            char* n5 = BdatTouchStringCell("MNU_sysmes",
                                     "name", 5);
            self->mSystemWindow.switchKindPane(2);
            self->mSystemWindow.setKindContent(reinterpret_cast<char*>(n2), 0);
            self->mSystemWindow.setTwoTextValues(reinterpret_cast<char*>(n4), reinterpret_cast<char*>(n5));
            self->mSystemWindow.applyKind(0);
            self->mSystemWindow.openPhase1();
            self->mCursor.SetVisible(0);
            self->field_1F4 = 1;
        }
        return;
    }

    u16 id = self->mVisibleItemIds[(s8)self->mMaxVisibleItems];
    if (id == 0) {
        return;
    }
    if (self->mPaneVisible[(s8)self->mMaxVisibleItems] == 0) {
        CItem_thunkAllocRecord(id, 1);
        self->mVisibleItemIds[(s8)self->mMaxVisibleItems] = 0;
        UIWin_BuildFlagBuf(2, id, 0);
        GetItemMulti_RefreshList(self);
        {
            char buf[32];
            s8 m = (s8)self->mMaxVisibleItems;
            if (m == 5) {
                sprintf(buf, "txt_close00");
            } else if (m == 4) {
                sprintf(buf, "txt_item%02d", m + 1);
            } else {
                sprintf(buf, "nul_item%02d", m + 1);
            }
            nw4r::lyt::Pane* pane = self->mLayout->GetRootPane()->FindPaneByName(buf, true);
            nw4r::lyt::Pane* refPane = self->mLayout->GetRootPane()->FindPaneByName(
                "nul_proportion", true);
            nw4r::math::VEC3 pos;
            ScalePaneWorldPos(&pos, pane, refPane, self->mLayout->GetRootPane());
            self->mCursor.setRootPaneTranslate(&pos);
        }
        if (self->mVisibleItemCount == 0) {
            self->mCursor.SetVisible(0);
            self->field_1F8 = 3;
            playUISound(0x89);
        } else {
            self->field_1F8 = 2;
        }
        return;
    }
    u32 cat = BdatGetItemType(id) & 0xFF;
    if ((u8)(cat - 10) <= 3u) {
        if (CItem_sumFamilyByte6(id) >= 0x63) {
            self->field_1F8 = 2;
            char* name = BdatTouchStringCell("MNU_sysmes",
                                       "name", 3);
            self->mSystemWindow.switchKindPane(0);
            self->mSystemWindow.setKindContent(reinterpret_cast<char*>(name), 0);
            self->mSystemWindow.applyKind(1);
            self->mSystemWindow.openPhase1();
            self->mCursor.SetVisible(0);
        } else {
            self->field_1F8 = 5;
            char* n2 = BdatTouchStringCell("MNU_sysmes",
                                     "name", 2);
            char* n4 = BdatTouchStringCell("MNU_sysmes",
                                     "name", 4);
            char* n5 = BdatTouchStringCell("MNU_sysmes",
                                     "name", 5);
            self->mSystemWindow.switchKindPane(2);
            self->mSystemWindow.setKindContent(reinterpret_cast<char*>(n2), 0);
            self->mSystemWindow.setTwoTextValues(reinterpret_cast<char*>(n4), reinterpret_cast<char*>(n5));
            self->mSystemWindow.applyKind(0);
            self->mSystemWindow.openPhase1();
            self->mCursor.SetVisible(0);
            self->field_1F4 = 1;
        }
    } else {
        self->field_1F8 = 5;
        char* n2 = BdatTouchStringCell("MNU_sysmes",
                                 "name", 2);
        char* n4 = BdatTouchStringCell("MNU_sysmes",
                                 "name", 4);
        char* n5 = BdatTouchStringCell("MNU_sysmes",
                                 "name", 5);
        self->mSystemWindow.switchKindPane(2);
        self->mSystemWindow.setKindContent(reinterpret_cast<char*>(n2), 0);
        self->mSystemWindow.setTwoTextValues(reinterpret_cast<char*>(n4), reinterpret_cast<char*>(n5));
        self->mSystemWindow.applyKind(0);
        self->mSystemWindow.openPhase1();
        self->mCursor.SetVisible(0);
        self->field_1F4 = 1;
    }
}

// Pad-input handler for the get-item-multi window. Up/down press events move
// the selection and play a sound; the held-state branches auto-repeat the
// selection while the cursor tracks a window slot. The pad bit layout
// depends on whether a Classic controller is attached.
void GetItemMulti_HandleWindowPadAuto(CMenuGetItemMulti* self) {
    cf::CfPadData* pad = cf::CfGameManager::getCfPadData();

    u32 up, down, curHeld, maskHeld;
    if (cf::CfGameManager::isClassicController(-1) != 0) {
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
        self->mSystemWindow.advancePhase3();
        self->mCursor.SetVisible(0);
        char buf[32];
        s8 n = (s8)self->mMaxVisibleItems;
        if (n == 5) {
            sprintf(buf, "txt_close00");
        } else if (n == 4) {
            sprintf(buf, "txt_item%02d", n + 1);
        } else {
            sprintf(buf, "nul_item%02d", n + 1);
        }
        nw4r::lyt::Pane* pane = self->mLayout->GetRootPane()->FindPaneByName(buf, true);
        nw4r::lyt::Pane* refPane = self->mLayout->GetRootPane()->FindPaneByName(
            "nul_proportion", true);
        nw4r::math::VEC3 out;
        ScalePaneWorldPos(&out, pane, refPane, self->mLayout->GetRootPane());
        self->mCursor.setRootPaneTranslate(&out);
        playUISound(3);
    } else if (down) {
        self->field_1F8 = 0xa;
        self->mSystemWindow.advancePhase3();
        self->mCursor.SetVisible(0);
        char buf[32];
        s8 n = (s8)self->mMaxVisibleItems;
        if (n == 5) {
            sprintf(buf, "txt_close00");
        } else if (n == 4) {
            sprintf(buf, "txt_item%02d", n + 1);
        } else {
            sprintf(buf, "nul_item%02d", n + 1);
        }
        nw4r::lyt::Pane* pane = self->mLayout->GetRootPane()->FindPaneByName(buf, true);
        nw4r::lyt::Pane* refPane = self->mLayout->GetRootPane()->FindPaneByName(
            "nul_proportion", true);
        nw4r::math::VEC3 out;
        ScalePaneWorldPos(&out, pane, refPane, self->mLayout->GetRootPane());
        self->mCursor.setRootPaneTranslate(&out);
        self->field_1F4 = 1;
        playUISound(6);
    } else if (maskHeld) {
        self->field_1F4 = self->field_1F4 - 1;
        if ((s8)self->field_1F4 < 0) {
            self->field_1F4 = 1;
        }
        nw4r::math::VEC3 out;
        CSysWin::getPaneScreenPos(reinterpret_cast<nw4r::math::VEC3*>(&out), &self->mSystemWindow, self->field_1F4);
        self->mCursor.setRootPaneTranslate(&out);
        playUISound(1);
    } else if (curHeld) {
        self->field_1F4 = self->field_1F4 + 1;
        if ((s8)self->field_1F4 > 1) {
            self->field_1F4 = 0;
        }
        nw4r::math::VEC3 out;
        CSysWin::getPaneScreenPos(reinterpret_cast<nw4r::math::VEC3*>(&out), &self->mSystemWindow, self->field_1F4);
        self->mCursor.setRootPaneTranslate(&out);
        playUISound(1);
    }
}

// Pad-input handler for the get-item-multi window (alternate variant). Same
// shape as GetItemMulti_HandleWindowPadAuto; the up branch additionally flags the auto-repeat
// state and uses a different selection count.
void GetItemMulti_HandleWindowPadOnce(CMenuGetItemMulti* self) {
    cf::CfPadData* pad = cf::CfGameManager::getCfPadData();

    u32 up, down, curHeld, maskHeld;
    if (cf::CfGameManager::isClassicController(-1) != 0) {
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
        self->mSystemWindow.advancePhase3();
        self->mCursor.SetVisible(0);
        char buf[32];
        s8 n = (s8)self->mMaxVisibleItems;
        if (n == 5) {
            sprintf(buf, "txt_close00");
        } else if (n == 4) {
            sprintf(buf, "txt_item%02d", n + 1);
        } else {
            sprintf(buf, "nul_item%02d", n + 1);
        }
        nw4r::lyt::Pane* pane = self->mLayout->GetRootPane()->FindPaneByName(buf, true);
        nw4r::lyt::Pane* refPane = self->mLayout->GetRootPane()->FindPaneByName(
            "nul_proportion", true);
        nw4r::math::VEC3 out;
        ScalePaneWorldPos(&out, pane, refPane, self->mLayout->GetRootPane());
        self->mCursor.setRootPaneTranslate(&out);
        playUISound(3);
    } else if (down) {
        self->field_1F8 = 0x7;
        self->mSystemWindow.advancePhase3();
        self->mCursor.SetVisible(0);
        char buf[32];
        s8 n = (s8)self->mMaxVisibleItems;
        if (n == 5) {
            sprintf(buf, "txt_close00");
        } else if (n == 4) {
            sprintf(buf, "txt_item%02d", n + 1);
        } else {
            sprintf(buf, "nul_item%02d", n + 1);
        }
        nw4r::lyt::Pane* pane = self->mLayout->GetRootPane()->FindPaneByName(buf, true);
        nw4r::lyt::Pane* refPane = self->mLayout->GetRootPane()->FindPaneByName(
            "nul_proportion", true);
        nw4r::math::VEC3 out;
        ScalePaneWorldPos(&out, pane, refPane, self->mLayout->GetRootPane());
        self->mCursor.setRootPaneTranslate(&out);
        playUISound(6);
    } else if (maskHeld) {
        self->field_1F4 = self->field_1F4 - 1;
        if ((s8)self->field_1F4 < 0) {
            self->field_1F4 = 1;
        }
        nw4r::math::VEC3 out;
        CSysWin::getPaneScreenPos(reinterpret_cast<nw4r::math::VEC3*>(&out), &self->mSystemWindow, self->field_1F4);
        self->mCursor.setRootPaneTranslate(&out);
        playUISound(1);
    } else if (curHeld) {
        self->field_1F4 = self->field_1F4 + 1;
        if ((s8)self->field_1F4 > 1) {
            self->field_1F4 = 0;
        }
        nw4r::math::VEC3 out;
        CSysWin::getPaneScreenPos(reinterpret_cast<nw4r::math::VEC3*>(&out), &self->mSystemWindow, self->field_1F4);
        self->mCursor.setRootPaneTranslate(&out);
        playUISound(1);
    }
}


void GetItemMulti_ThunkDtor6C(void* self) { ((void(*)(void*))__dt__17CMenuGetItemMultiFv)((char*)self - 0x6c); }

void GetItemMulti_ThunkRender70(void* self) { ((void(*)(void*))cbRenderBefore__17CMenuGetItemMultiFP4CScn)((char*)self - 0x70); }

void GetItemMulti_ThunkDtor70(void* self) { ((void(*)(void*))__dt__17CMenuGetItemMultiFv)((char*)self - 0x70); }

// --- hard-symbol stubs (scaffold_hard_symbols) ---
// Static constructor: initialise the get-item-multi pane colour/position
// defaults (.sbss). Each entry is an RGBA 16-bit quadruple; alpha stays 0.
// Statement order mirrors retail so MWCC's register allocation (shared zero
// register, long-lived gItemMultiPaneColor06 pointer) reproduces byte-identical.
void sinit_801B9FC8() {
    // Pane colour/position defaults (.sbss), one RGBA quadruple per pane.
    // Grouped per symbol in ascending address order - closest shape found
    // to retail's store/register schedule (interleaved orders reschedule
    // worse; chained assignments shift but don't close the residual).
    gItemMultiPaneColor06.b = 0xfa;
    gItemMultiPaneColor00.a = 0;
    gItemMultiPaneColor00.b = 0;
    gItemMultiPaneColor00.g = 0;
    gItemMultiPaneColor00.r = 0;
    gItemMultiPaneColor01.a = 0;
    gItemMultiPaneColor01.b = 0;
    gItemMultiPaneColor01.g = 0;
    gItemMultiPaneColor01.r = 0;
    gItemMultiPaneColor02.a = 0;
    gItemMultiPaneColor02.b = 0x80;
    gItemMultiPaneColor02.g = 0x80;
    gItemMultiPaneColor02.r = 0x80;
    gItemMultiPaneColor03.a = 0;
    gItemMultiPaneColor03.b = 0x80;
    gItemMultiPaneColor03.g = 0x80;
    gItemMultiPaneColor03.r = 0x80;
    gItemMultiPaneColor04.a = 0;
    gItemMultiPaneColor04.b = 0;
    gItemMultiPaneColor04.g = 0;
    gItemMultiPaneColor04.r = 0;
    gItemMultiPaneColor05.a = 0;
    gItemMultiPaneColor05.b = 0;
    gItemMultiPaneColor05.g = 0;
    gItemMultiPaneColor05.r = 0;
    gItemMultiPaneColor06.a = 0;
    gItemMultiPaneColor06.g = 0xff;
    gItemMultiPaneColor06.r = 0xff;
    gItemMultiPaneColor07.a = 0;
    gItemMultiPaneColor07.b = 0x80;
    gItemMultiPaneColor07.g = 0x80;
    gItemMultiPaneColor07.r = 0x80;
    gItemMultiPaneColor08.a = 0;
    gItemMultiPaneColor08.b = 0;
    gItemMultiPaneColor08.g = 0;
    gItemMultiPaneColor08.r = 0;
    gItemMultiPaneColor09.a = 0;
    gItemMultiPaneColor09.b = 0;
    gItemMultiPaneColor09.g = 0;
    gItemMultiPaneColor09.r = 0;
    gItemMultiPaneColor0A.a = 0;
    gItemMultiPaneColor0A.b = 0xe7;
    gItemMultiPaneColor0A.g = 0xa3;
    gItemMultiPaneColor0A.r = 0x12;
    gItemMultiPaneColor0B.a = 0;
    gItemMultiPaneColor0B.b = 0xff;
    gItemMultiPaneColor0B.g = 0xff;
    gItemMultiPaneColor0B.r = 0xff;
    gItemMultiPaneColor0C.a = 0;
    gItemMultiPaneColor0C.b = 0xc0;
    gItemMultiPaneColor0C.g = 0x9;
    gItemMultiPaneColor0C.r = 0xfa;
    gItemMultiPaneColor0D.a = 0;
    gItemMultiPaneColor0D.b = 0xff;
    gItemMultiPaneColor0D.g = 0xff;
    gItemMultiPaneColor0D.r = 0xff;
}
