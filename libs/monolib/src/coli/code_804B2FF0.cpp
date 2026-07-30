// Auto-scaffolded catalog TU for monolib/src/coli/code_804B2FF0
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>
#include "monolib/coli/CTaskColiManager.hpp"

void func_804B2FF0(){}

void func_804B30CC(){}

void func_804B31EC(){}

void func_804B33C8(){}

void func_804B34F4(){}

void func_804B3658(){}

void func_804B37E4(){}

void func_804B3970(){}

void func_804B3B18(){}

void func_804B3D1C(){}

void func_804B3EA8(){}

void func_804B4020(){}

void func_804B41FC(){}

void func_804B43B4(){}

void func_804B4478(){}

void func_804B453C(){}

void func_804B45E4(){}

void func_804B46A8(){}

void func_804B476C(){}

void func_804B4854(){}

void func_804B49B8(){}

void func_804B4BDC(){}

void func_804B4C7C(){}

// --- CTTask local declaration ---
// Local CTTask (out-of-line Move/Draw/dtor) for harness stubs.
// Do not include monolib/work/CTTask.hpp here — its inline methods collide
// with the explicit out-of-line instantiations below.
template <typename T>
class CTTask {
public:
    CTTask();
    virtual ~CTTask();
    virtual void Move();
    virtual void Draw();
};

// --- CTaskColiManager class definition ---
// Full class definition lives here (single-TU scope) because CTTask must
// be defined locally to avoid inline-method codegen from CTTask.hpp.
class CTaskColiManager : public CTTask<CTaskColiManager> {
public:
    // Overrides from CProcess (pure virtual)
    void Init() override;
    void Term() override;
    void Move() override;
    void Draw() override;

    virtual ~CTaskColiManager();
    static CTaskColiManager* create();
};

void CTaskColiManager::Init() {
    // Empty override — no initialization required.
}

void CTaskColiManager::Term() {
    // Empty override — no termination required.
}

void CTaskColiManager::Move() {}

void CTaskColiManager::Draw() {
    // Empty override — no draw-time work required.
}

void func_804B4E10(){}

void func_804B5088(){}

void func_804B526C(){}

void func_804B54D4(){}

void func_804B5658(){}

CTaskColiManager* CTaskColiManager::create() { return 0; }

// --- Explicit template specializations for CTTask<CTaskColiManager> ---
template<> CTTask<CTaskColiManager>::~CTTask() {}
template<> void CTTask<CTaskColiManager>::Move() {}
template<> void CTTask<CTaskColiManager>::Draw() {}
void sinit_804B598C(){}
