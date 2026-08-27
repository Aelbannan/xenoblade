#pragma once

#include <types.h>

#include <nw4r/lyt/lyt_animation.h>
#include <nw4r/lyt/lyt_drawInfo.h>
#include <nw4r/lyt/lyt_layout.h>

#include <decomp.h>

class __declspec(novtable) CMCCylinderGauge {
public:
    CMCCylinderGauge(nw4r::lyt::ArcResourceAccessor* arg);
    virtual ~CMCCylinderGauge();
    void init();
    void update();
    void draw(nw4r::lyt::DrawInfo*);
    void destroy();
    bool isReady();
    void start();
    void startOut();
    void addFillValue(float);
    void decrementLevel();
    u8 getLevel();
    void setLevel(u8);
    DECOMP_DONT_INLINE void updateIn();
    DECOMP_DONT_INLINE void updateFill();
    DECOMP_DONT_INLINE void updateOut();
    DECOMP_DONT_INLINE void updateFull();
    DECOMP_DONT_INLINE void bindInAnim();
    DECOMP_DONT_INLINE void bindUpAnim();
    DECOMP_DONT_INLINE void bindFullAnim();
    DECOMP_DONT_INLINE void bindOutAnim();

    nw4r::lyt::ArcResourceAccessor* mArcResourceAccessor; //0x4
    nw4r::lyt::Layout* mLayout;                           //0x8
    nw4r::lyt::AnimTransform* mAnimTransC;
    nw4r::lyt::AnimTransform* mAnimTrans10;
    nw4r::lyt::AnimTransform* mAnimTrans14;
    nw4r::lyt::AnimTransform* mAnimTrans18;
    bool unk1c;
    u32 unk20;
    bool unk24;
    u8 unk25;
    float unk28;
    CMCCylinderGauge();
};

// ---------------------------------------------------------------------------
// C-linkage imports (retail symbol names - keep linkage/signatures verbatim)
// ---------------------------------------------------------------------------

// Retail vtable data symbol (vtable lives in split1.s at 0x80535F48).
// The class is __declspec(novtable), so the ctor assigns the retail label
// explicitly instead of the compiler-generated __vt__16CMCCylinderGauge.
extern "C" u8 lbl_eu_80535F48[];
