#pragma once

#include <types.h>

#include <nw4r/lyt/lyt_animation.h>
#include <nw4r/lyt/lyt_drawInfo.h>
#include <nw4r/lyt/lyt_layout.h>

#include <decomp.h>

class CMCCylinderGauge {
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
    void updateIn();
    void updateFill();
    void updateOut();
    void updateFull();
    void bindInAnim();
    void bindUpAnim();
    void bindFullAnim();
    void bindOutAnim();

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
