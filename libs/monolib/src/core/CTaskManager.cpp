#include "monolib/core.hpp"
#include "monolib/device.hpp"
#include "monolib/work.hpp"
#include "monolib/core/CTaskManager.hpp"

// Retail also carries RTTI name strings (.rodata 0x80522588), RTTI locators
// (.sdata 0x80663568/70) and the base lists between the vtables; this
// compiler configuration does not emit those, so they are not spelled here.
// The two vtables (CRootProc, CTTask<CRootProc>) are emitted by MWCC from the
// class definitions below.

namespace {

/*
Root (top-level) process.

Serves as an empty parent process for children tasks.
*/
class CRootProc : public CTTask<CRootProc> {
public:
    virtual void Init() {}
    virtual void Term() {}

    virtual void Move() {}
    virtual void Draw() {}
};

CRootProc* spRootProcGame = 0;
CRootProc* spRootProcRealTime = 0;
CRootProc* spRootProcScn = 0;
CRootProc* spRootProcUnknown = 0;

// --- CTTask<CRootProc> out-of-line specializations ---
// CRootProc is anonymous-namespace-local (no retail counterpart); the
// canonical declared-only template emits no bodies, so these provide the
// definitions referenced by the emitted interim CTTask<CRootProc> vtable.
// (MWCC accepts these inside the unnamed namespace only because the template
// members are declared-only; inline bodies would raise 10333 redefined.)
// Move/Draw are inline specializations: MWCC defers them to the TU's weak tail,
// after CRootProc's inline virtuals, which is the retail .text order.
template<>
inline void CTTask<CRootProc>::Move() {
    if (mMoveFunc) {
        (static_cast<CRootProc*>(this)->*mMoveFunc)();
    }
}

template<>
inline void CTTask<CRootProc>::Draw() {
    if (mDrawFunc) {
        (static_cast<CRootProc*>(this)->*mDrawFunc)();
    }
}

} // namespace

void CTaskManager::Create() {
    CProcessMan::Init();
    CProcessMan::Reset();

    Start();
}

// Retail .text places the CTTask<CRootProc> dtor right after Create.
template<>
CTTask<CRootProc>::~CTTask() {}

// Inline: retail inlines Start into Create/Reset (both 0x23C bodies contain
// the full registration sequence) and emits NO standalone Start symbol.
// Non-inline MWCC would emit an out-of-line copy (+0x234 over split budget).
inline void CTaskManager::Start() {
    //All register with NULL parent because they are root-level processes
    spRootProcRealTime = new (CWorkThreadSystem::getWorkMem()) CRootProc();
    spRootProcRealTime->Regist(nullptr, false);

    spRootProcGame = new (CWorkThreadSystem::getWorkMem()) CRootProc();
    spRootProcGame->Regist(nullptr, false);

    spRootProcScn = new (CWorkThreadSystem::getWorkMem()) CRootProc();
    spRootProcScn->Regist(nullptr, false);

    spRootProcUnknown = new (CWorkThreadSystem::getWorkMem()) CRootProc();
    spRootProcUnknown->Regist(nullptr, false);
}

void CTaskManager::Release() {
    CProcessMan::Term();

    spRootProcGame = nullptr;
    spRootProcRealTime = nullptr;
    spRootProcScn = nullptr;
    spRootProcUnknown = nullptr;
}

void CTaskManager::Move() {
    CProcessMan::Move();
    CProcessMan::Delete();
}

void CTaskManager::Draw() {
    CProcessMan::Draw();
}

void CTaskManager::Reset() {
    CDeviceVI::waitForDrawDone();
    CProcessMan::Reset();

    Start();
}

CProcess* CTaskManager::GetRootProcGame() {
    return spRootProcGame;
}

CProcess* CTaskManager::GetRootProcRealTime() {
    return spRootProcRealTime;
}

CProcess* CTaskManager::GetRootProcScn() {
    return spRootProcScn;
}
