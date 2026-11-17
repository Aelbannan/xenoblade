#pragma once

#include <types.h>
#include "monolib/work/CTTask.hpp"
#include "monolib/work/IWorkEvent.hpp"

class CScn;

namespace cf {

// Game-root task that drives CfGameManager once per frame.
class CfTaskMain : public CTTask<CfTaskMain>, public IWorkEvent {
public:
    CfTaskMain(CScn* scene) : mScene(scene) {}
    virtual ~CfTaskMain();

    virtual void Init();
    virtual void Term();
    virtual void Move();
    virtual void Draw();
    virtual void Tail();

    static CfTaskMain* create(CProcess* parent, CScn* scene);

    /* 0x54: IWorkEvent vptr */
    /* 0x58 */ CScn* mScene;
};

} // namespace cf
