#pragma once

#include "kyoshin/cf/CfBdat.hpp"

#include <types.h>

#include <nw4r/lyt/lyt_animation.h>
#include <nw4r/lyt/lyt_drawInfo.h>
#include <nw4r/lyt/lyt_layout.h>

#include <decomp.h>
#include "kyoshin/code_80135FDC_State.hpp"
#include "kyoshin/CUICfManager_Res.hpp"
/******************************************************************************
 *
 * CMCEffStart
 *
 ******************************************************************************/
class CMCEffStart {
public:
    CMCEffStart(nw4r::lyt::ArcResourceAccessor*);
    virtual ~CMCEffStart();
    virtual void init();
    void update();
    void destroy();
    u8 isFinished();
    // Retail vtable lbl_eu_80536114 word 4 (+0x10) is startInAnim (the slot
    // dispatched by CModelDispMakeCrystal's state runners; the UpRed/UpBlue/
    // UpGreen/UpRank/Divide leaves carry their play() at the same +0x10 word).
    virtual void startInAnim();
    void updateInAnim();

    u8 unk4;
    u8 unk5;
    nw4r::lyt::ArcResourceAccessor* mArcResourceAccessor; //0x8
    nw4r::lyt::Layout* mLayout;                           //0xc
    nw4r::lyt::AnimTransform* mAnimTrans;                 //0x10
    u8 unk14;
    CMCEffStart();
};
/******************************************************************************
 *
 * CMCEffUpRed
 *
 ******************************************************************************/
class CMCEffUpRed : public CMCEffStart {
public:
    CMCEffUpRed(nw4r::lyt::ArcResourceAccessor*);
    virtual ~CMCEffUpRed();
    virtual void init();
    void play();
    CMCEffUpRed();
};
/******************************************************************************
 *
 * CMCEffUpBlue
 *
 ******************************************************************************/
class CMCEffUpBlue : public CMCEffStart {
public:
    CMCEffUpBlue(nw4r::lyt::ArcResourceAccessor*);
    virtual ~CMCEffUpBlue();
    virtual void init();
    void play();
    CMCEffUpBlue();
};
/******************************************************************************
 *
 * CMCEffUpGreen
 *
 ******************************************************************************/
class CMCEffUpGreen : public CMCEffStart {
public:
    CMCEffUpGreen(nw4r::lyt::ArcResourceAccessor*);
    virtual ~CMCEffUpGreen();
    virtual void init();
    void play();
    CMCEffUpGreen();
};
/******************************************************************************
 *
 * CMCEffUpPrm
 *
 ******************************************************************************/
struct UnkStruct {
    nw4r::lyt::Layout* layout;           //0x0
    nw4r::lyt::AnimTransform* animTrans; //0x4
    u8 unk8;
};

class CMCEffUpPrm {
public:
    CMCEffUpPrm(nw4r::lyt::ArcResourceAccessor*);
    virtual ~CMCEffUpPrm();

    u8 unk4;
    nw4r::lyt::ArcResourceAccessor* mArcResourceAccessor; //0x8
    UnkStruct unkStruct[8];                               //0xc
    u8 unk6c;
    void init();
    void update();
    void draw(nw4r::lyt::DrawInfo*);
    void destroy();
    void activateSlots(u32);
    void setTranslate(u32, nw4r::math::VEC3);
    void setNumber(u32, u8);
    CMCEffUpPrm();
};
/******************************************************************************
 *
 * CMCEffSuccess
 *
 ******************************************************************************/
class CMCEffSuccess {
public:
    CMCEffSuccess(nw4r::lyt::ArcResourceAccessor*);
    virtual ~CMCEffSuccess();

    u8 unk4;
    u8 unk5;
    nw4r::lyt::ArcResourceAccessor* mArcResourceAccessor; //0x8
    nw4r::lyt::Layout* mLayout;                           //0xc
    nw4r::lyt::AnimTransform* mAnimTrans10;
    nw4r::lyt::AnimTransform* mAnimTrans14;
    u8 unk18;
    virtual void init();
    void update();
    void draw(nw4r::lyt::DrawInfo*);
    void destroy();
    void startIn();
    void startOut();
    void updateIn();
    void updateOut();
    void bindInAnim();
    void bindOutAnim();
    CMCEffSuccess();
};
/******************************************************************************
 *
 * CMCEffFailure
 *
 ******************************************************************************/
class CMCEffFailure : public CMCEffStart {
public:
    CMCEffFailure(nw4r::lyt::ArcResourceAccessor*);
    virtual ~CMCEffFailure();
    virtual void init();
    CMCEffFailure();
};
/******************************************************************************
 *
 * CMCEffCrystal
 *
 ******************************************************************************/
class CMCEffCrystal {
public:
    CMCEffCrystal(nw4r::lyt::ArcResourceAccessor*);
    virtual ~CMCEffCrystal();
    virtual void MCCrystal_BuildLayouts();

    u8 unk4;
    u8 unk5;
    nw4r::lyt::ArcResourceAccessor* mArcResourceAccessor; //0x8
    nw4r::lyt::Layout* mLayoutC;
    nw4r::lyt::AnimTransform* mAnimTrans10;
    nw4r::lyt::AnimTransform* mAnimTrans14;
    nw4r::lyt::Layout* mLayout18;
    nw4r::lyt::AnimTransform* mAnimTrans1c;
    nw4r::lyt::AnimTransform* mAnimTrans20;
    nw4r::lyt::AnimTransform* mAnimTrans24;
    nw4r::lyt::AnimTransform* mAnimTrans28;
    nw4r::lyt::Layout* mLayout2c;
    nw4r::lyt::AnimTransform* mAnimTrans30;
    u8 unk34;
    u16 unk36;
    u8 unk38;
    void update();
    void draw(nw4r::lyt::DrawInfo*);
    void destroy();
    u8 isFinished();
    void startIn();
    void startOut();
    void startChange();
    void startSpecial();
    void setTraceVisible(bool);
    void setCrystalInfo(u16, u8);
    void updateIn();
    void checkInDone();
    void updateOut();
    void updateChange();
    void updateSpecial();
    void bindInAnimTrace();
    void bindOutAnimTrace();
    void bindInAnimCrystal();
    void bindChangeAnim();
    void bindSpecialAnim();
    void bindOutAnim();
    void bindLoopAnim();
    CMCEffCrystal();
};
/******************************************************************************
 *
 * CMCEffUpRank
 *
 ******************************************************************************/
class CMCEffUpRank : public CMCEffStart {
public:
    CMCEffUpRank(nw4r::lyt::ArcResourceAccessor*);
    virtual ~CMCEffUpRank();
    virtual void init();
    void play();
    CMCEffUpRank();
};
/******************************************************************************
 *
 * CMCEffDivide
 *
 ******************************************************************************/
class CMCEffDivide : public CMCEffStart {
public:
    CMCEffDivide(nw4r::lyt::ArcResourceAccessor*);
    virtual ~CMCEffDivide();
    virtual void init();
    void play();
    CMCEffDivide();
};
/******************************************************************************
 *
 * CMCEffCylinder
 *
 ******************************************************************************/
class CMCEffCylinder {
public:
    CMCEffCylinder(nw4r::lyt::ArcResourceAccessor*);
    virtual ~CMCEffCylinder();

    u8 unk4;
    u8 unk5;
    nw4r::lyt::ArcResourceAccessor* mArcResourceAccessor; //0x8
    nw4r::lyt::Layout* mLayout;                           //0xc
    nw4r::lyt::AnimTransform* mAnimTrans10;
    nw4r::lyt::AnimTransform* mAnimTrans14;
    nw4r::lyt::AnimTransform* mAnimTrans18;
    u8 unk1c;
    virtual void init();
    void update();
    void draw(nw4r::lyt::DrawInfo*);
    void destroy();
    u8 isFinished();
    void startIn();
    void startOut();
    void startChange();
    void setNumber(u8);
    void updateIn();
    void updateOut();
    void updateChange();
    void bindInAnim();
    void bindChangeAnim();
    void bindOutAnim();
    CMCEffCylinder();
};

// C-linkage imports (retail symbol names - keep linkage/signatures verbatim)
// NOTE: setPaneVisible is intentionally NOT promoted to this header. Parallel
// TU conversions declare it with conflicting signatures in their own headers
// (CItemBoxInfo.hpp: `(void*, void*)` vs CMCGetItemBox.hpp: `(nw4r::lyt::Pane*,
// u32)`), so a shared declaration here would add a third voice to that live
// conflict in TUs that include both headers (CModelDispMakeCrystal.cpp). It
// stays local to CMCEffStart.cpp (the defining TU).

// Retail string pool (SDA-data base for pooled string literals).
extern "C" const float lbl_eu_80668550;  // 0.0f (SetFrame reset)
extern "C" const float lbl_eu_80668554;   // 1.0f (anim done threshold)
extern "C" const double lbl_eu_80668558;  // int->float magic (0x4330000080000000)
extern "C" const float lbl_eu_80668560;   // 5.0f (crystal change frame)
// SDA global: retail refers to cf::CfBdat::spBtlSkillListFileData by its raw
// symbol name ::cf::CfBdat::spBtlSkillListFileData; referencing the C++ static member would emit
// the mangled reloc (spBtlSkillListFileData__Q22cf6CfBdat) which retail does
// not have.
