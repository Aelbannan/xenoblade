#define IWORK_EVENT_INLINE_DTOR
#include "kyoshin/CTaskGameEvt.hpp"

#include "kyoshin/cf/CTaskGameCf.hpp"
#include "monolib/scn/CScn.hpp"
#include "monolib/work/CWorkThreadSystem.hpp"
#include "monolib/util/MemManager.hpp"

// Event-sequence entry points defined in cf/CTaskREvent.cpp.
extern "C" {
void evtClearSeqStateBit9();
void evtTeardownActive();
int evtIsFullyIdle();
int evtBeginShuffledSeq();
}

CTaskGameEvt::CTaskGameEvt(CScn* scene) : mScene(scene), mFlags(0) {}

// CTTask<CTaskGameEvt> out-of-line specializations (see CTTask.hpp).
template <>
CTTask<CTaskGameEvt>::~CTTask() {}

CTaskGameEvt::~CTaskGameEvt() {}

void CTaskGameEvt::clearSeqState() { evtClearSeqStateBit9(); }

void CTaskGameEvt::teardownSeq() { evtTeardownActive(); }

void CTaskGameEvt::Init() {
    mScene->addRenderCB(this, 11, 0);
}

void CTaskGameEvt::Term() {
    mScene->removeRenderCB(this);
}

void CTaskGameEvt::Move() {
    u32 flags = mFlags;
    if ((flags & FLAG_ENABLED) && !(flags & FLAG_RUNNING) &&
        cf::CTaskGameCf::getInstance() != 0 &&
        evtIsFullyIdle() != 0) {
        if (evtBeginShuffledSeq()) {
            mFlags |= FLAG_RUNNING;
        } else {
            mFlags &= ~FLAG_RUNNING;
        }
    }
}

void CTaskGameEvt::cbRenderBefore() {}

bool CTaskGameEvt::OnFileEvent(CEventFile* event) {
    return false;
}

CTaskGameEvt* CTaskGameEvt::create(CProcess* parent, CScn* scene) {
    CTaskGameEvt* task = new (CWorkThreadSystem::getWorkMem()) CTaskGameEvt(scene);
    task->Regist(parent, false);
    return task;
}

void CTaskGameEvt::Draw() {}

template <>
void CTTask<CTaskGameEvt>::Move() {
    if (mMoveFunc) {
        (static_cast<CTaskGameEvt*>(this)->*mMoveFunc)();
    }
}

template <>
void CTTask<CTaskGameEvt>::Draw() {
    if (mDrawFunc) {
        (static_cast<CTaskGameEvt*>(this)->*mDrawFunc)();
    }
}
