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
    void func_80221EF4();
    void func_80221FE0();
    void func_80222070(nw4r::lyt::DrawInfo*);
    void func_80222090();
    bool func_802220F0();
    void func_802220F8();
    void func_80222118();
    void func_802221A4(float);
    void func_80222234();
    u8 func_80222258();
    void func_802222A4(u8);
    DECOMP_DONT_INLINE void func_802222C4();
    DECOMP_DONT_INLINE void func_80222318();
    DECOMP_DONT_INLINE void func_8022246C();
    DECOMP_DONT_INLINE void func_802224B0();
    DECOMP_DONT_INLINE void func_80222520();
    DECOMP_DONT_INLINE void func_802225A8();
    DECOMP_DONT_INLINE void func_80222630();
    DECOMP_DONT_INLINE void func_802226B8();

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
