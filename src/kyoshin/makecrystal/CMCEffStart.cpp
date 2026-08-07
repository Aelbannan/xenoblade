#include "kyoshin/makecrystal/CMCEffStart.hpp"

#include "kyoshin/cf/CfBdat.hpp"
#include "kyoshin/code_80135FDC.hpp"
#include "monolib/device/CDeviceFont.hpp"

#include "nw4r/lyt/lyt_pane.h"

extern u32 func_80137444(nw4r::lyt::AnimTransform*, float);
extern void func_80137038(nw4r::lyt::Layout*, nw4r::lyt::DrawInfo*, int, int);
extern void func_80138078(u32);
extern u32 func_801355BC();
extern "C" void func_80124270(nw4r::lyt::Pane*, u32);
extern u32 func_801355A0();

// Retail string pool (SDA-data base for pooled string literals).
extern "C" char lbl_eu_805095EC[];
extern "C" float lbl_eu_80668550;
extern "C" const float lbl_eu_80668554;
// func_80136910's retail symbol is the Itanium-mangled C++ name, so it must be
// referenced by its literal mangled identifier under C linkage (declaring it as
// a C++ function would re-mangle the name).
extern "C" void func_80136910__FPQ34nw4r3lyt6LayoutPcUc(nw4r::lyt::Layout*, char*, u8);

CMCEffStart::CMCEffStart(nw4r::lyt::ArcResourceAccessor* arcResourceAccessor)
    : unk4(0), unk5(1), mArcResourceAccessor(arcResourceAccessor), mLayout(nullptr), mAnimTrans(nullptr), unk14(0) {}

CMCEffStart::~CMCEffStart() {}

void CMCEffStart::init() {
    func_80136E84(&mLayout, mArcResourceAccessor, &lbl_eu_805095EC[0x0]);
    func_80136F08(mLayout, &mAnimTrans, mArcResourceAccessor, &lbl_eu_805095EC[0x17]);
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
    func_80137038(obj->mLayout, drawInfo, 0, 1);
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

void __declspec(noinline) CMCEffStart::startInAnim() {
    unk4 = 1;
    mAnimTrans->SetFrame(lbl_eu_80668550);
    unk5 = 0;
}

void __declspec(noinline) CMCEffStart::updateInAnim() {
    if(func_80137444(mAnimTrans, lbl_eu_80668554)) {
        unk4 = 0;
        unk5 = 1;
    }
}
/******************************************************************************
 *
 * CMCEffUpRed
 *
 ******************************************************************************/
CMCEffUpRed::CMCEffUpRed(nw4r::lyt::ArcResourceAccessor* pArcResourceAccessor) : CMCEffStart(pArcResourceAccessor) {}

CMCEffUpRed::~CMCEffUpRed() {}

void CMCEffUpRed::init() {
    func_80136E84(&mLayout, mArcResourceAccessor, &lbl_eu_805095EC[0x31]);
    func_80136F08(mLayout, &mAnimTrans, mArcResourceAccessor, &lbl_eu_805095EC[0x48]);
    mLayout->SetAnimationEnable(mAnimTrans, true);
    mLayout->Animate(false);
    unk14 = 1;
}

void CMCEffUpRed::play() {
    startInAnim();
    func_80138078(0x8c); //140 Dec
}
/******************************************************************************
 *
 * CMCEffUpBlue
 *
 ******************************************************************************/
CMCEffUpBlue::CMCEffUpBlue(nw4r::lyt::ArcResourceAccessor* pArcResourceAccessor) : CMCEffStart(pArcResourceAccessor) {}

CMCEffUpBlue::~CMCEffUpBlue() {}

void CMCEffUpBlue::init() {
    func_80136E84(&mLayout, mArcResourceAccessor, &lbl_eu_805095EC[0x62]);
    func_80136F08(mLayout, &mAnimTrans, mArcResourceAccessor, &lbl_eu_805095EC[0x79]);
    mLayout->SetAnimationEnable(mAnimTrans, true);
    mLayout->Animate(false);
    unk14 = 1;
}

void CMCEffUpBlue::play() {
    startInAnim();
    func_80138078(0x8c); //140 Dec
}
/******************************************************************************
 *
 * CMCEffUpGreen
 *
 ******************************************************************************/
CMCEffUpGreen::CMCEffUpGreen(nw4r::lyt::ArcResourceAccessor* pArcResourceAccessor) : CMCEffStart(pArcResourceAccessor) {}

CMCEffUpGreen::~CMCEffUpGreen() {}

void CMCEffUpGreen::init() {
    func_80136E84(&mLayout, mArcResourceAccessor, &lbl_eu_805095EC[0x93]);
    func_80136F08(mLayout, &mAnimTrans, mArcResourceAccessor, &lbl_eu_805095EC[0xaa]);
    mLayout->SetAnimationEnable(mAnimTrans, true);
    mLayout->Animate(false);
    unk14 = 1;
}

void CMCEffUpGreen::play() {
    startInAnim();
    func_80138078(0x8c); //140 Dec
}
/******************************************************************************
 *
 * CMCEffUpPrm
 *
 ******************************************************************************/
CMCEffUpPrm::CMCEffUpPrm(nw4r::lyt::ArcResourceAccessor* pArcResourceAccessor) : unk4(1), mArcResourceAccessor(pArcResourceAccessor) {
    UnkStruct* i = (UnkStruct*)((u8*)this + 0xc);
    do {
        i->layout = nullptr;
        i->animTrans = nullptr;
        i->unk8 = 0;
        i += 1;
    } while(i < (UnkStruct*)((u8*)this + 0x6c));
    unk6c = 0;
}

CMCEffUpPrm::~CMCEffUpPrm() {}

void CMCEffUpPrm::init() {
    for(u8 i = 0; i < 8; ++i) {
        UnkStruct* unkStructure = &unkStruct[i];
        // String pool at 0xc4 / 0xdb.
        func_80136E84(&unkStructure->layout, mArcResourceAccessor, &lbl_eu_805095EC[0xc4]);
        func_80136F08(unkStructure->layout, &unkStructure->animTrans, mArcResourceAccessor, &lbl_eu_805095EC[0xdb]);
        nw4r::lyt::Pane* rootPane = unkStructure->layout->GetRootPane();
        func_8013676C(rootPane, func_801355BC());
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
        if((s8)unkStruct[i].unk8 == 1 && func_80137444(unkStruct[i].animTrans, 1.0f) != 0) {
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
        func_80137038(unkStruct[(u8)i].layout, drawInfo, 0, 1);
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
    func_80136910__FPQ34nw4r3lyt6LayoutPcUc(unkStruct[arg1].layout, &lbl_eu_805095EC[0xf5], arg2);
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
    func_80136E84(&mLayout, mArcResourceAccessor, &lbl_eu_805095EC[0xfd]);
    func_80136F08(mLayout, &mAnimTrans10, mArcResourceAccessor, &lbl_eu_805095EC[0x113]);
    func_80136F08(mLayout, &mAnimTrans14, mArcResourceAccessor, &lbl_eu_805095EC[0x12c]);
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
    func_80137038(mLayout, drawInfo, 0, 1);
}

void CMCEffSuccess::destroy() {
    unk18 = 0;
    if(mLayout != nullptr) {
        delete mLayout;
        mLayout = nullptr;
    }
}

u8 isFinished(void* self) {
    return *(u8*)((u8*)self + 5);
}

void CMCEffSuccess::startIn() {
    if(unk4 != 0) {
        return;
    }
    unk4 = 1;
    bindInAnim();
    mAnimTrans10->SetFrame(lbl_eu_80668550);
    unk5 = 0;
    func_80138078(0x91);
}

void CMCEffSuccess::startOut() {
    if(unk4 == 2) {
        unk4 = 3;
        bindOutAnim();
        mAnimTrans14->SetFrame(lbl_eu_80668550);
        unk5 = 0;
    }
}

void __declspec(noinline) CMCEffSuccess::updateIn() {
    if(func_80137444(mAnimTrans10, lbl_eu_80668554) != 0) {
        unk4 = 2;
        unk5 = 1;
    }
}

void __declspec(noinline) CMCEffSuccess::updateOut() {
    if(func_80137444(mAnimTrans14, lbl_eu_80668554) != 0) {
        unk4 = 0;
        unk5 = 1;
    }
}

void __declspec(noinline) CMCEffSuccess::bindInAnim() {
    mLayout->UnbindAllAnimation();
    mLayout->BindAnimation(mAnimTrans10);
    mLayout->SetAnimationEnable(mAnimTrans10, true);
    mLayout->Animate(0);
}

void __declspec(noinline) CMCEffSuccess::bindOutAnim() {
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
CMCEffFailure::CMCEffFailure(nw4r::lyt::ArcResourceAccessor* pArcResourceAccessor) : CMCEffStart(pArcResourceAccessor) {}

CMCEffFailure::~CMCEffFailure() {}

void CMCEffFailure::init() {
    func_80136E84(&mLayout, mArcResourceAccessor, &lbl_eu_805095EC[0x146]);
    func_80136F08(mLayout, &mAnimTrans, mArcResourceAccessor, &lbl_eu_805095EC[0x15c]);
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

void CMCEffCrystal::func_80224CE4(){
    // String pool: trace 0x175/0x18a/0x1a2, crystal 0x1bb/0x1d1/0x1ea/0x207/0x221, list 0x23b/0x250.
    func_80136E84(&mLayoutC, mArcResourceAccessor, &lbl_eu_805095EC[0x175]);
    func_80136F08(mLayoutC, &mAnimTrans10, mArcResourceAccessor, &lbl_eu_805095EC[0x18a]);
    func_80136F08(mLayoutC, &mAnimTrans14, mArcResourceAccessor, &lbl_eu_805095EC[0x1a2]);
    func_80136E84(&mLayout18, mArcResourceAccessor, &lbl_eu_805095EC[0x1bb]);
    func_80136F08(mLayout18, &mAnimTrans1c, mArcResourceAccessor, &lbl_eu_805095EC[0x1d1]);
    func_80136F08(mLayout18, &mAnimTrans20, mArcResourceAccessor, &lbl_eu_805095EC[0x1ea]);
    func_80136F08(mLayout18, &mAnimTrans24, mArcResourceAccessor, &lbl_eu_805095EC[0x207]);
    func_80136F08(mLayout18, &mAnimTrans28, mArcResourceAccessor, &lbl_eu_805095EC[0x221]);

    nw4r::lyt::Pane* rootPane = mLayout18->GetRootPane();
    void* something = CDeviceFont::func_80452C10(1, mLayout18);
    //TODO It would help to have CDeviceFont return type and not void*
    u32 result = 0; //something -> someCall at 0x24
    func_8013676C(rootPane, result);
    func_80136E84(&mLayout2c, mArcResourceAccessor, &lbl_eu_805095EC[0x23b]);
    func_80136F08(mLayout2c, &mAnimTrans30, mArcResourceAccessor, &lbl_eu_805095EC[0x250]);
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
    func_80137444(mAnimTrans30, lbl_eu_80668554);
    mLayoutC->Animate(0);
    mLayout18->Animate(0);
    mLayout2c->Animate(0);
}

void CMCEffCrystal::draw(nw4r::lyt::DrawInfo* drawInfo) {
    if(unk34 == 0) return;
    func_80137038(mLayoutC, drawInfo, 0, 1);
    func_80137038(mLayout18, drawInfo, 0, 1);
    if(unk4 == 2 || unk4 == 3 || unk4 == 5) {
        func_80137038(mLayout2c, drawInfo, 0, 1);
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
        mAnimTrans10->SetFrame(lbl_eu_80668550);
        bindInAnimCrystal();
        mAnimTrans1c->SetFrame(lbl_eu_80668550);
        unk5 = 0;
        func_80124270(mLayout2c->GetRootPane(), 1);
    }
}

void CMCEffCrystal::startOut() {
    if(unk4 == 3) {
        unk4 = 4;
        bindOutAnimTrace();
        mAnimTrans14->SetFrame(lbl_eu_80668550);
        bindOutAnim();
        if(mLayout2c->GetRootPane()->IsVisible()) {
            mAnimTrans28->SetFrame(lbl_eu_80668550);
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
        mAnimTrans20->SetFrame(lbl_eu_80668550);
        unk5 = 0;
    }
}

void CMCEffCrystal::startSpecial() {
    if(unk4 == 3) {
        unk4 = 6;
        bindSpecialAnim();
        mAnimTrans24->SetFrame(lbl_eu_80668550);
        unk5 = 0;
        func_80124270(mLayout2c->GetRootPane(), 0);
    }
}

void CMCEffCrystal::setTraceVisible(u32 arg1) {
    func_80124270(mLayoutC->GetRootPane()->FindPaneByName(&lbl_eu_805095EC[0x26a], true), arg1);
}

void CMCEffCrystal::setCrystalInfo(u16 arg1, u8 arg2) {
    unk36 = arg1;
    unk38 = arg2;
    char* name = func_8013639C(cf::CfBdat::spBtlSkillListFileData, "name", arg1);
    char* itemName = func_80136190("MNU_item", "name", 0x1e - (arg2 - 1));
    char buffer[0x20];
    sprintf(buffer, "%s%s", name, itemName);
    func_80136A1C(mLayout18, "txt_listname01", buffer, 0);
    void* resource = nullptr;
    u8 result = func_801361E8(cf::CfBdat::spBtlSkillListFileData, "atr_type", arg1);
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
        func_80137E7C(mLayout18, "pic_crs", resource);
        func_80137E7C(mLayout18, "pic_crs01", resource);
    }
}

void __declspec(noinline) CMCEffCrystal::updateIn() {
    func_80137444(mAnimTrans1c, lbl_eu_80668554);
    if(func_80137444(mAnimTrans10, lbl_eu_80668554) != 0) {
        unk4 = 2;
    }
}

void __declspec(noinline) CMCEffCrystal::checkInDone() {
    if(func_80137444(mAnimTrans1c, lbl_eu_80668554) != 0) {
        unk4 = 3;
        unk5 = 1;
    }
}

void __declspec(noinline) CMCEffCrystal::updateOut() {
    u32 unk1 = func_80137444(mAnimTrans14, lbl_eu_80668554);
    u32 unk2 = func_80137444(mAnimTrans28, lbl_eu_80668554);
    if(unk1 != 0 && unk2 != 0) {
        unk4 = 0;
        unk5 = 1;
    }
}

void __declspec(noinline) CMCEffCrystal::updateChange() {
    if(func_80137444(mAnimTrans20, lbl_eu_80668554)) {
        unk4 = 3;
        unk5 = 1;
    }
    if(mAnimTrans20->GetFrame() >= 5.0f) {
        char* name = func_8013639C(cf::CfBdat::spBtlSkillListFileData, "name", unk36);
        char* itemName = func_80136190("MNU_item", "name", 0x1e - unk38);
        char buffer[0x20];
        sprintf(buffer, "%s%s", name, itemName);
        func_80136A1C(mLayout18, "txt_listname01", buffer, 0);
    }
}

void __declspec(noinline) CMCEffCrystal::updateSpecial() {
    if(func_80137444(mAnimTrans24, lbl_eu_80668554)) {
        unk4 = 3;
        unk5 = 1;
    }
}

void __declspec(noinline) CMCEffCrystal::bindInAnimTrace() {
    mLayoutC->UnbindAllAnimation();
    mLayoutC->BindAnimation(mAnimTrans10);
    mLayoutC->SetAnimationEnable(mAnimTrans10, true);
    mLayoutC->Animate(0);
}

void __declspec(noinline) CMCEffCrystal::bindOutAnimTrace() {
    mLayoutC->UnbindAllAnimation();
    mLayoutC->BindAnimation(mAnimTrans14);
    mLayoutC->SetAnimationEnable(mAnimTrans14, true);
    mLayoutC->Animate(0);
}

void __declspec(noinline) CMCEffCrystal::bindInAnimCrystal() {
    mLayout18->UnbindAllAnimation();
    mLayout18->BindAnimation(mAnimTrans1c);
    mLayout18->SetAnimationEnable(mAnimTrans1c, true);
    mLayout18->Animate(0);
}

void __declspec(noinline) CMCEffCrystal::bindChangeAnim() {
    mLayout18->UnbindAllAnimation();
    mLayout18->BindAnimation(mAnimTrans20);
    mLayout18->SetAnimationEnable(mAnimTrans20, true);
    mLayout18->Animate(0);
}

void __declspec(noinline) CMCEffCrystal::bindSpecialAnim() {
    mLayout18->UnbindAllAnimation();
    mLayout18->BindAnimation(mAnimTrans24);
    mLayout18->SetAnimationEnable(mAnimTrans24, true);
    mLayout18->Animate(0);
}

void __declspec(noinline) CMCEffCrystal::bindOutAnim() {
    mLayout18->UnbindAllAnimation();
    mLayout18->BindAnimation(mAnimTrans28);
    mLayout18->SetAnimationEnable(mAnimTrans28, true);
    mLayout18->Animate(0);
}

void __declspec(noinline) CMCEffCrystal::bindLoopAnim() {
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
CMCEffUpRank::CMCEffUpRank(nw4r::lyt::ArcResourceAccessor* pArcResourceAccessor) : CMCEffStart(pArcResourceAccessor) {}

CMCEffUpRank::~CMCEffUpRank() {}

void CMCEffUpRank::init() {
    func_80136E84(&mLayout, mArcResourceAccessor, &lbl_eu_805095EC[0x348]);
    func_80136F08(mLayout, &mAnimTrans, mArcResourceAccessor, &lbl_eu_805095EC[0x35e]);
    mLayout->SetAnimationEnable(mAnimTrans, true);
    mLayout->Animate(0);
    unk14 = 1;
}

void CMCEffUpRank::play() {
    startInAnim();
    func_80138078(0x8d);
}
/******************************************************************************
 *
 * CMCEffDivide
 *
 ******************************************************************************/
CMCEffDivide::CMCEffDivide(nw4r::lyt::ArcResourceAccessor* pArcResourceAccessor) : CMCEffStart(pArcResourceAccessor) {}

CMCEffDivide::~CMCEffDivide() {}

void CMCEffDivide::init() {
    // String pool refs at retail offsets 0x377 / 0x38c.
    func_80136E84(&mLayout, mArcResourceAccessor, &lbl_eu_805095EC[0x377]);
    func_80136F08(mLayout, &mAnimTrans, mArcResourceAccessor, &lbl_eu_805095EC[0x38c]);
    mLayout->SetAnimationEnable(mAnimTrans, true);
    mLayout->Animate(0);
    unk14 = 1;
}

void CMCEffDivide::play() {
    startInAnim();
    func_80138078(0x8d);
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
    func_80136E84(&mLayout, mArcResourceAccessor, &lbl_eu_805095EC[0x3a4]);
    func_80136F08(mLayout, &mAnimTrans10, mArcResourceAccessor, &lbl_eu_805095EC[0x3b9]);
    func_80136F08(mLayout, &mAnimTrans14, mArcResourceAccessor, &lbl_eu_805095EC[0x3d1]);
    func_80136F08(mLayout, &mAnimTrans18, mArcResourceAccessor, &lbl_eu_805095EC[0x3ed]);
    u32 ul = func_801355A0();
    if(ul) {
        func_801368C0(mLayout, &lbl_eu_805095EC[0x406], ul);
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
    func_80137038(mLayout, drawInfo, 0, 1);
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
        mAnimTrans10->SetFrame(lbl_eu_80668550);
        unk5 = 0;
    }
}

void CMCEffCylinder::startOut() {
    if(unk4 == 2) {
        unk4 = 3;
        bindOutAnim();
        mAnimTrans18->SetFrame(lbl_eu_80668550);
        unk5 = 0;
    }
}

void CMCEffCylinder::startChange() {
    if(unk4 == 2) {
        unk4 = 4;
        bindChangeAnim();
        mAnimTrans14->SetFrame(lbl_eu_80668550);
        unk5 = 0;
    }
}

void CMCEffCylinder::setNumber(u8 arg) {
    func_80136910__FPQ34nw4r3lyt6LayoutPcUc(mLayout, &lbl_eu_805095EC[0x406], arg);
}

void __declspec(noinline) CMCEffCylinder::updateIn() {
    if(func_80137444(mAnimTrans10, lbl_eu_80668554)) {
        unk4 = 2;
        unk5 = 1;
    }
}

void __declspec(noinline) CMCEffCylinder::updateOut() {
    if(func_80137444(mAnimTrans18, lbl_eu_80668554)) {
        unk4 = 0;
        unk5 = 1;
    }
}

void __declspec(noinline) CMCEffCylinder::updateChange() {
    if(func_80137444(mAnimTrans14, lbl_eu_80668554)) {
        unk4 = 2;
        unk5 = 1;
    }
}

void __declspec(noinline) CMCEffCylinder::bindInAnim() {
    mLayout->UnbindAllAnimation();
    mLayout->BindAnimation(mAnimTrans10);
    mLayout->SetAnimationEnable(mAnimTrans10, true);
    mLayout->Animate(0);
}

void __declspec(noinline) CMCEffCylinder::bindChangeAnim() {
    mLayout->UnbindAllAnimation();
    mLayout->BindAnimation(mAnimTrans14);
    mLayout->SetAnimationEnable(mAnimTrans14, true);
    mLayout->Animate(0);
}

void __declspec(noinline) CMCEffCylinder::bindOutAnim() {
    mLayout->UnbindAllAnimation();
    mLayout->BindAnimation(mAnimTrans18);
    mLayout->SetAnimationEnable(mAnimTrans18, true);
    mLayout->Animate(0);
}
