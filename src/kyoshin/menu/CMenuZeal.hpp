#pragma once

#include <types.h>

#include "kyoshin/IUICf.hpp"
#include "monolib/work/IWorkEvent.hpp"
#include "monolib/scn/IScnRender.hpp"
#include "monolib/lib/UnkClass_8045F564.hpp"

namespace nw4r { namespace lyt { class Layout; class AnimTransform; } }
class CScn;

// Zeal (collective) menu screen controller.
class CMenuZeal : public IUICf, public IWorkEvent, public IScnRender {
public:
    CMenuZeal(CScn* scene, int a3, float f1);
    virtual ~CMenuZeal();

    virtual void Init();
    virtual void Term();
    virtual void Move();
    virtual void cbRenderBefore();

    static CMenuZeal* create(CProcess* parent, CScn* scene, int a3, float f1);

    static CMenuZeal* sInstance;

    /* 0x60 */ CScn* mScn;                       // owning scene
    /* 0x64 */ nw4r::lyt::Layout* mLayout1;
    /* 0x68 */ nw4r::lyt::AnimTransform* mAnim1;
    /* 0x6C */ nw4r::lyt::AnimTransform* mAnim2;
    /* 0x70 */ nw4r::lyt::AnimTransform* mAnim3;
    /* 0x74 */ nw4r::lyt::AnimTransform* mAnim4;
    /* 0x78 */ nw4r::lyt::Layout* mLayout2;
    /* 0x7C */ nw4r::lyt::AnimTransform* mAnim5;
    /* 0x80 */ nw4r::lyt::AnimTransform* mAnim6;
    /* 0x84 */ nw4r::lyt::AnimTransform* mAnim7;
    /* 0x88 */ nw4r::lyt::AnimTransform* mAnim8;
    /* 0x8C */ nw4r::lyt::Layout* mLayout3;
    /* 0x90 */ nw4r::lyt::AnimTransform* mAnim9;
    /* 0x94 */ f32 mField_94;
    /* 0x98 */ f32 mField_98;
    /* 0x9C */ f32 mField_9C;                    // position (AddZealEffect)
    /* 0xA0 */ u8 mField_A0;                     // gates the mLayout3 draw
    /* 0xA1 */ u8 mField_A1;
    /* 0xA2 */ u8 _padA2[2];
    /* 0xA4 */ UnkClass_8045F564 mLayoutMem;    // scratch region
    /* 0xB4 */ f32 mField_B4;
    /* 0xB8 */ u32 mField_B8;
    /* 0xBC */ u8 mField_BC;
    /* 0xBD */ u8 mField_BD;                     // gates the whole draw
    /* 0xBE */ u8 mField_BE;
    /* 0xBF */ u8 mField_BF;                     // singleton keep flag
    /* 0xC0 */ f32 mField_C0;
    /* 0xC4 */ int mField_C4;                    // menu state machine
}; // size: 0xC8
