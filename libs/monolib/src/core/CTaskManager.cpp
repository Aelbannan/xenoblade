#include "monolib/core.hpp"
#include "monolib/device.hpp"
#include "monolib/work.hpp"
#include "monolib/core/CTaskManager.hpp"

// Blob monolibdata1d dissolve: sbss slots owned by this TU (retail
// lbl_eu_806655B8/BC/C0/C4). Declared extern "C" in CTaskManager.hpp; the
// initializers make these the definitions (zero-fill -> .sbss).
extern "C" CRootProc* lbl_eu_806655B8 = 0;
extern "C" CRootProc* lbl_eu_806655BC = 0;
extern "C" CRootProc* lbl_eu_806655C0 = 0;
extern "C" CRootProc* lbl_eu_806655C4 = 0;

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

// --- CTTask<CRootProc> out-of-line specializations ---
// CRootProc is anonymous-namespace-local (no retail counterpart); the
// canonical declared-only template emits no bodies, so these provide the
// definitions referenced by the emitted interim CTTask<CRootProc> vtable.
// (MWCC accepts these inside the unnamed namespace only because the template
// members are declared-only; inline bodies would raise 10333 redefined.)
template<>
void CTTask<CRootProc>::Move() {
    if (mMoveFunc) {
        (static_cast<CRootProc*>(this)->*mMoveFunc)();
    }
}

template<>
void CTTask<CRootProc>::Draw() {
    if (mDrawFunc) {
        (static_cast<CRootProc*>(this)->*mDrawFunc)();
    }
}

template<>
CTTask<CRootProc>::~CTTask() {}

} // namespace

void CTaskManager::Create() {
    CProcessMan::Init();
    CProcessMan::Reset();

    Start();
}

// Inline: retail inlines Start into Create/Reset (both 0x23C bodies contain
// the full registration sequence) and emits NO standalone Start symbol.
// Non-inline MWCC would emit an out-of-line copy (+0x234 over split budget).
inline void CTaskManager::Start() {
    //All register with NULL parent because they are root-level processes
    lbl_eu_806655BC = new (CWorkThreadSystem::getWorkMem()) CRootProc();
    lbl_eu_806655BC->Regist(nullptr, false);

    lbl_eu_806655B8 = new (CWorkThreadSystem::getWorkMem()) CRootProc();
    lbl_eu_806655B8->Regist(nullptr, false);

    lbl_eu_806655C0 = new (CWorkThreadSystem::getWorkMem()) CRootProc();
    lbl_eu_806655C0->Regist(nullptr, false);

    lbl_eu_806655C4 = new (CWorkThreadSystem::getWorkMem()) CRootProc();
    lbl_eu_806655C4->Regist(nullptr, false);
}

void CTaskManager::Release() {
    CProcessMan::Term();

    lbl_eu_806655B8 = nullptr;
    lbl_eu_806655BC = nullptr;
    lbl_eu_806655C0 = nullptr;
    lbl_eu_806655C4 = nullptr;
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
    return lbl_eu_806655B8;
}

CProcess* CTaskManager::GetRootProcRealTime() {
    return lbl_eu_806655BC;
}

CProcess* CTaskManager::GetRootProcScn() {
    return lbl_eu_806655C0;
}
