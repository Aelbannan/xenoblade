#include "kyoshin/cf/CfBdat.hpp"
#include "kyoshin/makecrystal/CMCEffStart.hpp"
#include "kyoshin/CQuestWindow_Pane.hpp"

// NOTE: CfBdat.hpp is intentionally NOT included. It declares
// `extern void* ::cf::CfBdat::spItmWpnListFileData;` while code_80135FDC.hpp declares the same
// symbol as `extern u32 ::cf::CfBdat::spItmWpnListFileData;` -- including both is a redeclaration
// error (10563). This TU uses none of CfBdat's declarations (all symbols
// below come from CMCEffStart.hpp / code_80135FDC.hpp / local externs), so
// the include is dropped to keep the two extern forms apart.
#include "kyoshin/code_80135FDC.hpp"
#include "monolib/device/CDeviceFont.hpp"

#include "nw4r/lyt/lyt_pane.h"
#include "kyoshin/CUICfManager_Res.hpp"

// (data_vtables.hpp is intentionally NOT included: its extern "C" __RTTI__*
// decls trip MWCC 10322 under -ipa file. Slots are declared locally in the
// sibling-TU style instead.)
// The typeinfo object is declared inside a namespace: at global scope MWCC
// 10322 ("illegal name overloading") rejects the __RTTI__* spelling under
// -ipa file. extern "C" names are not mangled, so the table below still
// references __RTTI__10IWorkEvent exactly (the same workaround as CView.cpp).
namespace RTTIExterns {
extern "C" const void* __RTTI__10IWorkEvent[];
}

extern "C" {
extern void WorkEvent1__10IWorkEventFPvPCc();
extern void WorkEvent3__10IWorkEventFPv();
extern void WorkEvent4__10IWorkEventFv();
extern void OnPauseTrigger__10IWorkEventFb();
extern void WorkEvent6__10IWorkEventFv();
extern void WorkEvent7__10IWorkEventFv();
extern void WorkEvent8__10IWorkEventFv();
extern void WorkEvent9__10IWorkEventFv();
extern void WorkEvent10__10IWorkEventFv();
extern void WorkEvent11__10IWorkEventFv();
extern void WorkEvent12__10IWorkEventFv();
extern void WorkEvent13__10IWorkEventFv();
extern void WorkEvent14__10IWorkEventFv();
extern void WorkEvent15__10IWorkEventFv();
extern void WorkEvent16__10IWorkEventFv();
extern void WorkEvent17__10IWorkEventFv();
extern void WorkEvent18__10IWorkEventFv();
extern void WorkEvent19__10IWorkEventFv();
extern void WorkEvent20__10IWorkEventFv();
extern void WorkEvent21__10IWorkEventFv();
extern void WorkEvent22__10IWorkEventFv();
extern void WorkEvent23__10IWorkEventFv();
extern void WorkEvent24__10IWorkEventFv();
extern void WorkEvent25__10IWorkEventFv();
extern void WorkEvent26__10IWorkEventFv();
extern void WorkEvent27__10IWorkEventFv();
extern void WorkEvent28__10IWorkEventFv();
extern void WorkEvent29__10IWorkEventFv();
extern void WorkEvent30__10IWorkEventFv();
extern void WorkEvent31__10IWorkEventFv();
}

// Retail data-slice imports for the hand-built prefix below: CMCCrystalList's
// key functions (defined in CMCCrystalList.cpp) and its RTTI name string
// (defined by CMCCrystalList.o's .rodata when that TU emits it).
extern "C" void __dt__14CMCCrystalListFv();
extern "C" int OnFileEvent__14CMCCrystalListFP10CEventFile(void*);
extern "C" char lbl_eu_805092B0[];
// Forward declaration: the .sdata pair below is referenced by the .data prefix.
extern "C" const void* __RTTI__14CMCCrystalList[2];

// ---------------------------------------------------------------------------
// Retail data-slice definitions owned by this TU.
//
// splits.txt gives this TU .data 0x80535F80-0x80536128, .rodata
// 0x80509548-0x80509A00, .sdata 0x806627F8-0x80662858 and .sdata2
// 0x80668550-0x80668568. MWCC auto-emits only this TU's own 11 vtables /
// typeinfo pairs / RTTI names / literal pool, so the slice heads (the
// CMCCrystalList vtable + IWorkEvent base pair, the shared layout-name
// pool, the sdata2 constants) are defined here explicitly in retail order;
// the compiler-generated run trails them exactly as in retail.
// ---------------------------------------------------------------------------
extern "C" {
// .data prefix: IWorkEvent RTTI base table (0x10).
__declspec(section ".data") __attribute__((used))
const void* lbl_eu_80536008[4] = {
    (const void*)RTTIExterns::__RTTI__10IWorkEvent,
    (const void*)0,
    (const void*)0,
    (const void*)0
};
} // extern "C"

// .sdata prefix: IWorkEvent typeinfo pair { name, base }.
__declspec(section ".sdata") __attribute__((used))
const void* __RTTI__14CMCCrystalList[2] = {
    (const void*)lbl_eu_805092B0,
    (const void*)lbl_eu_80536008
};



// Kept inline (not promoted to CMCEffStart.hpp): see the NOTE in the header.


// Minimal abstract mirror of the font object returned by
// CDeviceFont::getFontInfo. Its slot 9 sits at vtable offset 0x24 and
// returns the u32 bound into the crystal's font pane. Never instantiated (no
// ctor), so no vtable is emitted; declaring the call as a genuine virtual
// member makes MWCC emit retail's native r12 dispatch sequence.
struct CFontPanel {
    virtual void sf2() = 0;
    virtual void sf3() = 0;
    virtual void sf4() = 0;
    virtual void sf5() = 0;
    virtual void sf6() = 0;
    virtual void sf7() = 0;
    virtual void sf8() = 0;
    virtual u32 sf9() = 0;
};

// The effect classes are real classes: their constructors are the member
// constructors below and MWCC installs the class vtable (the retail labels
// __vt__11CMCEffStart / __vt__11CMCEffUpRed / ... own that store).

CMCEffStart::CMCEffStart(nw4r::lyt::ArcResourceAccessor* arcResourceAccessor)
    : unk4(0),
      unk5(1),
      mArcResourceAccessor(arcResourceAccessor),
      mLayout(nullptr),
      mAnimTrans(nullptr),
      unk14(0) {}

CMCEffStart::~CMCEffStart() {}

void CMCEffStart::init() {
    buildLayout(&mLayout, mArcResourceAccessor, "mf10_cry06_start.brlyt");
    bindLayoutAnimTransform(mLayout, &mAnimTrans, mArcResourceAccessor, "mf10_cry06_start_in.brlan");
    mLayout->SetAnimationEnable(mAnimTrans, true);
    mLayout->Animate(false);
    unk14 = 1;
}

void CMCEffStart::update() {
    if(unk14) {
        if((s8)unk4 == 1) {
            updateInAnim();
        }
        mLayout->Animate(false);
    }
}

void draw(CMCEffStart* obj, nw4r::lyt::DrawInfo* drawInfo) {
    if (!obj->unk14) return;
    drawLayout(obj->mLayout, drawInfo, 0, 1);
}

void CMCEffStart::destroy() {
    unk14 = 0;
    if(mLayout != nullptr) {
        delete mLayout;
        mLayout = nullptr;
    }
}

u8 CMCEffStart::isFinished() {
    return unk5;
}

void CMCEffStart::startInAnim() {
    unk4 = 1;
    mAnimTrans->SetFrame(0.0f);
    unk5 = 0;
}

void CMCEffStart::updateInAnim() {
    if(advanceAnimTransform(mAnimTrans, 1.0f)) {
        unk4 = 0;
        unk5 = 1;
    }
}
/******************************************************************************
 *
 * CMCEffUpRed
 *
 ******************************************************************************/
CMCEffUpRed::CMCEffUpRed(nw4r::lyt::ArcResourceAccessor* pArcResourceAccessor)
    : CMCEffStart(pArcResourceAccessor) {}

CMCEffUpRed::~CMCEffUpRed() {}

void CMCEffUpRed::init() {
    buildLayout(&mLayout, mArcResourceAccessor, "mf10_cry02_prt00.brlyt");
    bindLayoutAnimTransform(mLayout, &mAnimTrans, mArcResourceAccessor, "mf10_cry02_prt00_in.brlan");
    mLayout->SetAnimationEnable(mAnimTrans, true);
    mLayout->Animate(false);
    unk14 = 1;
}

void CMCEffUpRed::play() {
    CMCEffStart::startInAnim();
    playUISound(0x8c); //140 Dec
}
/******************************************************************************
 *
 * CMCEffUpBlue
 *
 ******************************************************************************/
CMCEffUpBlue::CMCEffUpBlue(nw4r::lyt::ArcResourceAccessor* pArcResourceAccessor)
    : CMCEffStart(pArcResourceAccessor) {}

CMCEffUpBlue::~CMCEffUpBlue() {}

void CMCEffUpBlue::init() {
    buildLayout(&mLayout, mArcResourceAccessor, "mf10_cry02_prt01.brlyt");
    bindLayoutAnimTransform(mLayout, &mAnimTrans, mArcResourceAccessor, "mf10_cry02_prt01_in.brlan");
    mLayout->SetAnimationEnable(mAnimTrans, true);
    mLayout->Animate(false);
    unk14 = 1;
}

void CMCEffUpBlue::play() {
    CMCEffStart::startInAnim();
    playUISound(0x8c); //140 Dec
}
/******************************************************************************
 *
 * CMCEffUpGreen
 *
 ******************************************************************************/
CMCEffUpGreen::CMCEffUpGreen(nw4r::lyt::ArcResourceAccessor* pArcResourceAccessor)
    : CMCEffStart(pArcResourceAccessor) {}

CMCEffUpGreen::~CMCEffUpGreen() {}

void CMCEffUpGreen::init() {
    buildLayout(&mLayout, mArcResourceAccessor, "mf10_cry02_prt02.brlyt");
    bindLayoutAnimTransform(mLayout, &mAnimTrans, mArcResourceAccessor, "mf10_cry02_prt02_in.brlan");
    mLayout->SetAnimationEnable(mAnimTrans, true);
    mLayout->Animate(false);
    unk14 = 1;
}

void CMCEffUpGreen::play() {
    CMCEffStart::startInAnim();
    playUISound(0x8c); //140 Dec
}
/******************************************************************************
 *
 * CMCEffUpPrm
 *
 ******************************************************************************/
CMCEffUpPrm::CMCEffUpPrm(nw4r::lyt::ArcResourceAccessor* pArcResourceAccessor) : unk4(1), mArcResourceAccessor(pArcResourceAccessor) {
    UnkStruct* i = unkStruct;
    do {
        i->layout = nullptr;
        i->animTrans = nullptr;
        i->unk8 = 0;
        i += 1;
    } while(i < &unkStruct[8]);
    unk6c = 0;
}

CMCEffUpPrm::~CMCEffUpPrm() {}

void CMCEffUpPrm::init() {
    for(u8 i = 0; i < 8; ++i) {
        UnkStruct* unkStructure = &unkStruct[i];
        // String pool at 0xc4 / 0xdb.
        buildLayout(&unkStructure->layout, mArcResourceAccessor, "mf10_cry02_prt03.brlyt");
        bindLayoutAnimTransform(unkStructure->layout, &unkStructure->animTrans, mArcResourceAccessor, "mf10_cry02_prt03_in.brlan");
        nw4r::lyt::Pane* rootPane = unkStructure->layout->GetRootPane();
        ApplyFontToTxtPanes(rootPane, (const nw4r::ut::Font*)((u32)CUICfManager_getPackedFont9C()));
        unkStructure->layout->SetAnimationEnable(unkStructure->animTrans, true);
        unkStructure->layout->Animate(0);
    }
    unk6c = 1;
}

void CMCEffUpPrm::update() {
    if(unk6c == 0) {
        return;
    }
    for(u8 i = 0; i < 8; ++i) {
        if((s8)unkStruct[i].unk8 == 1 && advanceAnimTransform(unkStruct[i].animTrans, 1.0f) != 0) {
            unkStruct[i].unk8 = 0;
        }
        unkStruct[i].layout->Animate(0);
    }
}

void CMCEffUpPrm::draw(nw4r::lyt::DrawInfo* drawInfo) {
    if(unk6c == 0) {
        return;
    }
    for(u16 i = 0; i < 8; ++i) {
        drawLayout(unkStruct[(u8)i].layout, drawInfo, 0, 1);
    }
}

void CMCEffUpPrm::destroy() {
    unk6c = 0;
    for(u8 i = 0; i < 8; i++) {
        UnkStruct* unk_struct = &unkStruct[i];
        if(unk_struct->layout != nullptr) {
            delete unk_struct->layout;
            unk_struct->layout = nullptr;
        }
    }
}

void CMCEffUpPrm::activateSlots(u32 arg) {
    for(u8 i = 0; i < 8; ++i) {
        unkStruct[i].unk8 = 0;
        // Pooled 0.0f (not named): MWCC hoists the pool load out of the loop
        // into f31 (retail shape); a named extern load stays in the loop and
        // the function grows to 0xA0 (see postprocess pool_patterns rename).
        unkStruct[i].animTrans->SetFrame(0.0f);
        unkStruct[i].layout->Animate(0);
        if(i < arg) {
            unkStruct[i].unk8 = 1;
        }
    }
}

void CMCEffUpPrm::setTranslate(u32 arg1, nw4r::math::VEC3 arg2) {
    if(arg1 >= 8) {
        return;
    }
    unkStruct[arg1].layout->GetRootPane()->SetTranslate(arg2);
}

void CMCEffUpPrm::setNumber(u32 arg1, u8 arg2) {
    if(arg1 >= 8) {
        return;
    }
    setLayoutTextBoxNumber(unkStruct[arg1].layout, "txt_bns", arg2);
}
/******************************************************************************
 *
 * CMCEffSuccess
 *
 ******************************************************************************/
CMCEffSuccess::CMCEffSuccess(nw4r::lyt::ArcResourceAccessor* pArcResourceAccessor)
    : unk4(0),
      unk5(1),
      mArcResourceAccessor(pArcResourceAccessor),
      mLayout(nullptr),
      mAnimTrans10(nullptr),
      mAnimTrans14(nullptr),
      unk18(0) {}

CMCEffSuccess::~CMCEffSuccess() {}

void CMCEffSuccess::init() {
    buildLayout(&mLayout, mArcResourceAccessor, "mf10_cry06_comp.brlyt");
    bindLayoutAnimTransform(mLayout, &mAnimTrans10, mArcResourceAccessor, "mf10_cry06_comp_in.brlan");
    bindLayoutAnimTransform(mLayout, &mAnimTrans14, mArcResourceAccessor, "mf10_cry06_comp_out.brlan");
    bindInAnim();
    mLayout->Animate(0);
    unk18 = 1;
}

void CMCEffSuccess::update() {
    if(!unk18) {
        return;
    }
    switch(unk4) {
        case 1:
            updateIn();
            break;
        case 3:
            updateOut();
            break;
        default:
    }
    mLayout->Animate(0);
}

void CMCEffSuccess::draw(nw4r::lyt::DrawInfo* drawInfo) {
    if (!unk18) {
        return;
    }
    drawLayout(mLayout, drawInfo, 0, 1);
}

void CMCEffSuccess::destroy() {
    unk18 = 0;
    if(mLayout != nullptr) {
        delete mLayout;
        mLayout = nullptr;
    }
}

u8 isFinished(void* effect) {
    return static_cast<CMCEffSuccess*>(effect)->unk5;
}

void CMCEffSuccess::startIn() {
    if(unk4 != 0) {
        return;
    }
    unk4 = 1;
    bindInAnim();
    mAnimTrans10->SetFrame(0.0f);
    unk5 = 0;
    playUISound(0x91);
}

void CMCEffSuccess::startOut() {
    if(unk4 == 2) {
        unk4 = 3;
        bindOutAnim();
        mAnimTrans14->SetFrame(0.0f);
        unk5 = 0;
    }
}

void CMCEffSuccess::updateIn() {
    if(advanceAnimTransform(mAnimTrans10, 1.0f) != 0) {
        unk4 = 2;
        unk5 = 1;
    }
}

void CMCEffSuccess::updateOut() {
    if(advanceAnimTransform(mAnimTrans14, 1.0f) != 0) {
        unk4 = 0;
        unk5 = 1;
    }
}

void CMCEffSuccess::bindInAnim() {
    mLayout->UnbindAllAnimation();
    mLayout->BindAnimation(mAnimTrans10);
    mLayout->SetAnimationEnable(mAnimTrans10, true);
    mLayout->Animate(0);
}

void CMCEffSuccess::bindOutAnim() {
    mLayout->UnbindAllAnimation();
    mLayout->BindAnimation(mAnimTrans14);
    mLayout->SetAnimationEnable(mAnimTrans14, true);
    mLayout->Animate(0);
}
/******************************************************************************
 *
 * CMCEffFailure
 *
 ******************************************************************************/
CMCEffFailure::CMCEffFailure(nw4r::lyt::ArcResourceAccessor* pArcResourceAccessor)
    : CMCEffStart(pArcResourceAccessor) {}

CMCEffFailure::~CMCEffFailure() {}

void CMCEffFailure::init() {
    buildLayout(&mLayout, mArcResourceAccessor, "mf10_cry06_fail.brlyt");
    bindLayoutAnimTransform(mLayout, &mAnimTrans, mArcResourceAccessor, "mf10_cry06_fail_in.brlan");
    mLayout->SetAnimationEnable(mAnimTrans, true);
    mLayout->Animate(false);
    unk14 = 1;
}
/******************************************************************************
 *
 * CMCEffCrystal
 *
 ******************************************************************************/
CMCEffCrystal::CMCEffCrystal(nw4r::lyt::ArcResourceAccessor* pArcResourceAccessor)
    : unk4(0),
      unk5(1),
      mArcResourceAccessor(pArcResourceAccessor),
      mLayoutC(nullptr),
      mAnimTrans10(nullptr),
      mAnimTrans14(nullptr),
      mLayout18(nullptr),
      mAnimTrans1c(nullptr),
      mAnimTrans20(nullptr),
      mAnimTrans24(nullptr),
      mAnimTrans28(nullptr),
      mLayout2c(nullptr),
      mAnimTrans30(nullptr),
      unk34(0),
      unk36(0),
      unk38(0) {}

CMCEffCrystal::~CMCEffCrystal() {}

void CMCEffCrystal::MCCrystal_BuildLayouts(){
    // String pool: trace 0x175/0x18a/0x1a2, crystal 0x1bb/0x1d1/0x1ea/0x207/0x221, list 0x23b/0x250.
    buildLayout(&mLayoutC, mArcResourceAccessor, "mf10_cry02_trc.brlyt");
    bindLayoutAnimTransform(mLayoutC, &mAnimTrans10, mArcResourceAccessor, "mf10_cry02_trc_in.brlan");
    bindLayoutAnimTransform(mLayoutC, &mAnimTrans14, mArcResourceAccessor, "mf10_cry02_trc_out.brlan");
    buildLayout(&mLayout18, mArcResourceAccessor, "mf10_cry03_crys.brlyt");
    bindLayoutAnimTransform(mLayout18, &mAnimTrans1c, mArcResourceAccessor, "mf10_cry03_crys_in.brlan");
    bindLayoutAnimTransform(mLayout18, &mAnimTrans20, mArcResourceAccessor, "mf10_cry03_crys_change.brlan");
    bindLayoutAnimTransform(mLayout18, &mAnimTrans24, mArcResourceAccessor, "mf10_cry03_crys_spl.brlan");
    bindLayoutAnimTransform(mLayout18, &mAnimTrans28, mArcResourceAccessor, "mf10_cry03_crys_out.brlan");

    nw4r::lyt::Pane* rootPane = mLayout18->GetRootPane();
    CFontPanel* fontObj =
        static_cast<CFontPanel*>(CDeviceFont::getFontInfo(1, mLayout18));
    // Virtual slot 9 (offset 0x24) on the font object yields the u32 the
    // crystal's font pane is set from.
    u32 result = fontObj->sf9();
    ApplyFontToTxtPanes(rootPane, (const nw4r::ut::Font*)result);
    buildLayout(&mLayout2c, mArcResourceAccessor, "mf10_cry04_lst.brlyt");
    bindLayoutAnimTransform(mLayout2c, &mAnimTrans30, mArcResourceAccessor, "mf10_cry04_lst_roop.brlan");
    bindInAnimTrace();
    bindInAnimCrystal();
    bindLoopAnim();
    unk34 = 1;
}

void CMCEffCrystal::update() {
    if(unk34 == 0) return;
    switch(unk4) {
        case 1:
            updateIn();
            break;
        case 2:
            checkInDone();
            break;
        case 4:
            updateOut();
            break;
        case 5:
            updateChange();
            break;
        case 6:
            updateSpecial();
            break;
        default:
            break;
    }
    advanceAnimTransform(mAnimTrans30, 1.0f);
    mLayoutC->Animate(0);
    mLayout18->Animate(0);
    mLayout2c->Animate(0);
}

void CMCEffCrystal::draw(nw4r::lyt::DrawInfo* drawInfo) {
    if(unk34 == 0) return;
    drawLayout(mLayoutC, drawInfo, 0, 1);
    drawLayout(mLayout18, drawInfo, 0, 1);
    if(unk4 == 2 || unk4 == 3 || unk4 == 5) {
        drawLayout(mLayout2c, drawInfo, 0, 1);
    }
}

void CMCEffCrystal::destroy() {
    unk34 = 0;
    if(mLayoutC != nullptr) {
        delete mLayoutC;
        mLayoutC = nullptr;
    }
    if(mLayout18 != nullptr) {
        delete mLayout18;
        mLayout18 = nullptr;
    }
    if(mLayout2c != nullptr) {
        delete mLayout2c;
        mLayout2c = nullptr;
    }
}

u8 CMCEffCrystal::isFinished() {
    return unk5;
}

void CMCEffCrystal::startIn() {
    if(unk4 == 0) {
        unk4 = 1;
        bindInAnimTrace();
        mAnimTrans10->SetFrame(0.0f);
        bindInAnimCrystal();
        mAnimTrans1c->SetFrame(0.0f);
        unk5 = 0;
        setPaneVisible(mLayout2c->GetRootPane(), 1);
    }
}

void CMCEffCrystal::startOut() {
    if(unk4 == 3) {
        unk4 = 4;
        bindOutAnimTrace();
        mAnimTrans14->SetFrame(0.0f);
        bindOutAnim();
        if(mLayout2c->GetRootPane()->IsVisible()) {
            mAnimTrans28->SetFrame(0.0f);
        } else {
            mAnimTrans28->SetFrame(mAnimTrans28->GetFrameSize() - 1);
        }
        unk5 = 0;
    }
}

void CMCEffCrystal::startChange() {
    if(unk4 == 3) {
        unk4 = 5;
        bindChangeAnim();
        mAnimTrans20->SetFrame(0.0f);
        unk5 = 0;
    }
}

void CMCEffCrystal::startSpecial() {
    if(unk4 == 3) {
        unk4 = 6;
        bindSpecialAnim();
        mAnimTrans24->SetFrame(0.0f);
        unk5 = 0;
        setPaneVisible(mLayout2c->GetRootPane(), 0);
    }
}

void CMCEffCrystal::setTraceVisible(bool arg1) {
    setPaneVisible(mLayoutC->GetRootPane()->FindPaneByName("nul_trc01", true), arg1);
}

void CMCEffCrystal::setCrystalInfo(u16 arg1, u8 arg2) {
    unk36 = arg1;
    unk38 = arg2;
    char* name = BdatGetPtrDirect(::cf::CfBdat::spBtlSkillListFileData, "name", arg1);
    char* itemName = (char*)BdatTouchStringCell("MNU_item", "name", 0x1e - (arg2 - 1));
    char buffer[0x20];
    sprintf(buffer, "%s%s", name, itemName);
    LayoutSetTextBoxText(mLayout18, "txt_listname01", buffer, 0);
    void* resource = nullptr;
    u8 result = BdatGetU8Direct((const void*)::cf::CfBdat::spBtlSkillListFileData, "atr_type", arg1);
    switch(result) {
        case 0:
            resource = mArcResourceAccessor->GetResource(mArcResourceAccessor->RES_TYPE_TEXTURE, "mf00_com00_dmy.tpl", 0);
            break;
        case 4:
            resource = mArcResourceAccessor->GetResource(mArcResourceAccessor->RES_TYPE_TEXTURE, "mf10_cry00_crys01.tpl", 0);
            break;
        case 5:
            resource = mArcResourceAccessor->GetResource(mArcResourceAccessor->RES_TYPE_TEXTURE, "mf10_cry00_crys02.tpl", 0);
            break;
        case 6:
            resource = mArcResourceAccessor->GetResource(mArcResourceAccessor->RES_TYPE_TEXTURE, "mf10_cry00_crys03.tpl", 0);
            break;
        case 7:
            resource = mArcResourceAccessor->GetResource(mArcResourceAccessor->RES_TYPE_TEXTURE, "mf10_cry00_crys04.tpl", 0);
            break;
        case 8:
            resource = mArcResourceAccessor->GetResource(mArcResourceAccessor->RES_TYPE_TEXTURE, "mf10_cry00_crys05.tpl", 0);
            break;
        case 9:
            resource = mArcResourceAccessor->GetResource(mArcResourceAccessor->RES_TYPE_TEXTURE, "mf10_cry00_crys06.tpl", 0);
            break;
        default:
            break;
    }
    if(resource != nullptr) {
        PaneSetTexPaletteByName(mLayout18, "pic_crs", (TPLPalette*)resource);
        PaneSetTexPaletteByName(mLayout18, "pic_crs01", (TPLPalette*)resource);
    }
}

void CMCEffCrystal::updateIn() {
    advanceAnimTransform(mAnimTrans1c, 1.0f);
    if(advanceAnimTransform(mAnimTrans10, 1.0f) != 0) {
        unk4 = 2;
    }
}

void CMCEffCrystal::checkInDone() {
    if(advanceAnimTransform(mAnimTrans1c, 1.0f) != 0) {
        unk4 = 3;
        unk5 = 1;
    }
}

void CMCEffCrystal::updateOut() {
    u32 unk1 = advanceAnimTransform(mAnimTrans14, 1.0f);
    u32 unk2 = advanceAnimTransform(mAnimTrans28, 1.0f);
    if(unk1 != 0 && unk2 != 0) {
        unk4 = 0;
        unk5 = 1;
    }
}

void CMCEffCrystal::updateChange() {
    if(advanceAnimTransform(mAnimTrans20, 1.0f)) {
        unk4 = 3;
        unk5 = 1;
    }
    if(mAnimTrans20->GetFrame() >= 5.0f) {
        char* name = BdatGetPtrDirect(::cf::CfBdat::spBtlSkillListFileData, "name", unk36);
        char* itemName = (char*)BdatTouchStringCell("MNU_item", "name", 0x1e - unk38);
        char buffer[0x20];
        sprintf(buffer, "%s%s", name, itemName);
        LayoutSetTextBoxText(mLayout18, "txt_listname01", buffer, 0);
    }
}

void CMCEffCrystal::updateSpecial() {
    if(advanceAnimTransform(mAnimTrans24, 1.0f)) {
        unk4 = 3;
        unk5 = 1;
    }
}

void CMCEffCrystal::bindInAnimTrace() {
    mLayoutC->UnbindAllAnimation();
    mLayoutC->BindAnimation(mAnimTrans10);
    mLayoutC->SetAnimationEnable(mAnimTrans10, true);
    mLayoutC->Animate(0);
}

void CMCEffCrystal::bindOutAnimTrace() {
    mLayoutC->UnbindAllAnimation();
    mLayoutC->BindAnimation(mAnimTrans14);
    mLayoutC->SetAnimationEnable(mAnimTrans14, true);
    mLayoutC->Animate(0);
}

void CMCEffCrystal::bindInAnimCrystal() {
    mLayout18->UnbindAllAnimation();
    mLayout18->BindAnimation(mAnimTrans1c);
    mLayout18->SetAnimationEnable(mAnimTrans1c, true);
    mLayout18->Animate(0);
}

void CMCEffCrystal::bindChangeAnim() {
    mLayout18->UnbindAllAnimation();
    mLayout18->BindAnimation(mAnimTrans20);
    mLayout18->SetAnimationEnable(mAnimTrans20, true);
    mLayout18->Animate(0);
}

void CMCEffCrystal::bindSpecialAnim() {
    mLayout18->UnbindAllAnimation();
    mLayout18->BindAnimation(mAnimTrans24);
    mLayout18->SetAnimationEnable(mAnimTrans24, true);
    mLayout18->Animate(0);
}

void CMCEffCrystal::bindOutAnim() {
    mLayout18->UnbindAllAnimation();
    mLayout18->BindAnimation(mAnimTrans28);
    mLayout18->SetAnimationEnable(mAnimTrans28, true);
    mLayout18->Animate(0);
}

void CMCEffCrystal::bindLoopAnim() {
    mLayout2c->UnbindAllAnimation();
    mLayout2c->BindAnimation(mAnimTrans30);
    mLayout2c->SetAnimationEnable(mAnimTrans30, true);
    mLayout2c->Animate(0);
}
/******************************************************************************
 *
 * CMCEffUpRank
 *
 ******************************************************************************/
CMCEffUpRank::CMCEffUpRank(nw4r::lyt::ArcResourceAccessor* pArcResourceAccessor)
    : CMCEffStart(pArcResourceAccessor) {}

CMCEffUpRank::~CMCEffUpRank() {}

void CMCEffUpRank::init() {
    buildLayout(&mLayout, mArcResourceAccessor, "mf10_cry05_rank.brlyt");
    bindLayoutAnimTransform(mLayout, &mAnimTrans, mArcResourceAccessor, "mf10_cry05_rank_in.brlan");
    mLayout->SetAnimationEnable(mAnimTrans, true);
    mLayout->Animate(0);
    unk14 = 1;
}

void CMCEffUpRank::play() {
    CMCEffStart::startInAnim();
    playUISound(0x8d);
}
/******************************************************************************
 *
 * CMCEffDivide
 *
 ******************************************************************************/
CMCEffDivide::CMCEffDivide(nw4r::lyt::ArcResourceAccessor* pArcResourceAccessor)
    : CMCEffStart(pArcResourceAccessor) {}

CMCEffDivide::~CMCEffDivide() {}

void CMCEffDivide::init() {
    // String pool refs at retail offsets 0x377 / 0x38c.
    buildLayout(&mLayout, mArcResourceAccessor, "mf10_cry05_spl.brlyt");
    bindLayoutAnimTransform(mLayout, &mAnimTrans, mArcResourceAccessor, "mf10_cry05_spl_in.brlan");
    mLayout->SetAnimationEnable(mAnimTrans, true);
    mLayout->Animate(0);
    unk14 = 1;
}

void CMCEffDivide::play() {
    CMCEffStart::startInAnim();
    playUISound(0x8d);
}
/******************************************************************************
 *
 * CMCEffCylinder
 *
 ******************************************************************************/
CMCEffCylinder::CMCEffCylinder(nw4r::lyt::ArcResourceAccessor* pArcResourceAccessor)
    : unk4(0),
      unk5(1),
      mArcResourceAccessor(pArcResourceAccessor),
      mLayout(nullptr),
      mAnimTrans10(nullptr),
      mAnimTrans14(nullptr),
      mAnimTrans18(nullptr),
      unk1c(0) {}

CMCEffCylinder::~CMCEffCylinder() {}

void CMCEffCylinder::init() {
    // String pool: 0x3a4 / 0x3b9 / 0x3d1 / 0x3ed / 0x406.
    buildLayout(&mLayout, mArcResourceAccessor, "mf10_cry06_chn.brlyt");
    bindLayoutAnimTransform(mLayout, &mAnimTrans10, mArcResourceAccessor, "mf10_cry06_chn_in.brlan");
    bindLayoutAnimTransform(mLayout, &mAnimTrans14, mArcResourceAccessor, "mf10_cry06_chn_change.brlan");
    bindLayoutAnimTransform(mLayout, &mAnimTrans18, mArcResourceAccessor, "mf10_cry06_chn_out.brlan");
    u32 ul = (u32)getPackedFont();
    if(ul) {
        setLayoutTextBoxFont(mLayout, "txt_num", ul);
    }
    bindInAnim();
    unk1c = 1;
}

void CMCEffCylinder::update() {
    if(unk1c == 0) return;
    switch(unk4) {
        case 1:
            updateIn();
            break;
        case 3:
            updateOut();
            break;
        case 4:
            updateChange();
            break;
        default:
            break;
    }
    mLayout->Animate(0);
}

void CMCEffCylinder::draw(nw4r::lyt::DrawInfo* drawInfo) {
    if(unk1c == 0) return;
    drawLayout(mLayout, drawInfo, 0, 1);
}

void CMCEffCylinder::destroy() {
    unk1c = 0;
    if(mLayout != nullptr) {
        delete mLayout;
        mLayout = nullptr;
    }
}

u8 CMCEffCylinder::isFinished() {
    return unk5;
}

void CMCEffCylinder::startIn() {
    if(unk4 == 0) {
        unk4 = 1;
        bindInAnim();
        mAnimTrans10->SetFrame(0.0f);
        unk5 = 0;
    }
}

void CMCEffCylinder::startOut() {
    if(unk4 == 2) {
        unk4 = 3;
        bindOutAnim();
        mAnimTrans18->SetFrame(0.0f);
        unk5 = 0;
    }
}

void CMCEffCylinder::startChange() {
    if(unk4 == 2) {
        unk4 = 4;
        bindChangeAnim();
        mAnimTrans14->SetFrame(0.0f);
        unk5 = 0;
    }
}

void CMCEffCylinder::setNumber(u8 arg) {
    setLayoutTextBoxNumber(mLayout, "txt_num", arg);
}

void CMCEffCylinder::updateIn() {
    if(advanceAnimTransform(mAnimTrans10, 1.0f)) {
        unk4 = 2;
        unk5 = 1;
    }
}

void CMCEffCylinder::updateOut() {
    if(advanceAnimTransform(mAnimTrans18, 1.0f)) {
        unk4 = 0;
        unk5 = 1;
    }
}

void CMCEffCylinder::updateChange() {
    if(advanceAnimTransform(mAnimTrans14, 1.0f)) {
        unk4 = 2;
        unk5 = 1;
    }
}

void CMCEffCylinder::bindInAnim() {
    mLayout->UnbindAllAnimation();
    mLayout->BindAnimation(mAnimTrans10);
    mLayout->SetAnimationEnable(mAnimTrans10, true);
    mLayout->Animate(0);
}

void CMCEffCylinder::bindChangeAnim() {
    mLayout->UnbindAllAnimation();
    mLayout->BindAnimation(mAnimTrans14);
    mLayout->SetAnimationEnable(mAnimTrans14, true);
    mLayout->Animate(0);
}

void CMCEffCylinder::bindOutAnim() {
    mLayout->UnbindAllAnimation();
    mLayout->BindAnimation(mAnimTrans18);
    mLayout->SetAnimationEnable(mAnimTrans18, true);
    mLayout->Animate(0);
}
