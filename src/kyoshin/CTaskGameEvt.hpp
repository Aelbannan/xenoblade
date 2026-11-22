#pragma once

#include <types.h>

#include "monolib/work/CTTask.hpp"
#include "monolib/work/IWorkEvent.hpp"
#include "monolib/scn/IScnRender.hpp"

class CEventFile;
class CScn;

// Event task of the game scene: starts the queued event sequence once the
// field task is idle.
// The interface bases are private: retail's RTTI base list names only the
// CTTask chain.
class CTaskGameEvt : public CTTask<CTaskGameEvt>, IWorkEvent, IScnRender {
public:
    enum {
        FLAG_RUNNING = 1 << 0,
        FLAG_ENABLED = 1 << 1,
    };

    CTaskGameEvt(CScn* scene);
    virtual ~CTaskGameEvt();

    void clearSeqState();
    void teardownSeq();

    virtual void Init();
    virtual void Term();
    virtual void Move();
    virtual void Draw();
    virtual void cbRenderBefore(CScn* scene);
    virtual bool OnFileEvent(CEventFile* event);

    static CTaskGameEvt* create(CProcess* parent, CScn* scene);

    /* 0x5C */ CScn* mScene;
    /* 0x60 */ u32 mFlags;
}; // size: 0x64
