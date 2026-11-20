#pragma once

// Real retail MI class: IUIBattle (a CTTask<IUIBattle> at +0x00 with the
// +0x54/+0x55 tail), IWorkEvent at +0x58 and IScnRender at +0x5C.
#define CTTASK_INLINE_DTOR
#define IWORK_EVENT_INLINE_DTOR

#include <types.h>
#include "monolib/work/CTTask.hpp"
#include "monolib/work/IWorkEvent.hpp"
#include "monolib/scn/IScnRender.hpp"
#include "kyoshin/CUIBattleManager.hpp"
#include <nw4r/lyt.h>
#include "monolib/lib/UnkClass_8045F564.hpp"
#include "monolib/scn/CScn.hpp"
#include "kyoshin/CUICfManager_Res.hpp"

struct CMenuVisionEntry {
    nw4r::lyt::Layout* mLayout;
    nw4r::lyt::AnimTransform* mAnim1;
    nw4r::lyt::AnimTransform* mAnim2;
    nw4r::lyt::AnimTransform* mAnim3;
    nw4r::lyt::AnimTransform* mAnim4;
    nw4r::lyt::AnimTransform* mAnim5;
    nw4r::lyt::AnimTransform* mAnim6;
    nw4r::lyt::AnimTransform* mAnim7;
    nw4r::lyt::AnimTransform* mAnim8;
    int mState;
    u8 field_0x28;
    u8 _pad29[3];
    f32 mTimer;
};

class CMenuVision : public IUIBattle,
                    public IWorkEvent,
                    public IScnRender {
public:
    CMenuVision(CScn* scn);
    virtual ~CMenuVision();
    virtual void Init();
    virtual void Term();
    virtual void Move();
    virtual void cbRenderBefore();

    CScn* mScn;                                   // 0x60
    UnkClass_8045F564 mLayoutMem;                 // 0x64
    CMenuVisionEntry mEntries[6];                 // 0x74, stride 0x30
};
