// Auto-scaffolded catalog TU for monolib/src/coli/code_804B2FF0
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>
#include "monolib/coli/CTaskColiManager.hpp"

extern "C" void func_804B2FF0() {}

extern "C" void func_804B30CC() {}

extern "C" void func_804B31EC() {}

extern "C" void func_804B33C8() {}

extern "C" void func_804B34F4() {}

extern "C" void func_804B3658() {}

extern "C" void func_804B37E4() {}

extern "C" void func_804B3970() {}

extern "C" void func_804B3B18() {}

extern "C" void func_804B3D1C() {}

extern "C" void func_804B3EA8() {}

extern "C" void func_804B4020() {}

extern "C" void func_804B41FC() {}

extern "C" void func_804B43B4() {}

extern "C" void func_804B4478() {}

extern "C" void func_804B453C() {}

extern "C" void func_804B45E4() {}

extern "C" void func_804B46A8() {}

extern "C" void func_804B476C() {}

extern "C" void func_804B4854() {}

extern "C" void func_804B49B8() {}

extern "C" void __dt__16CTaskColiManagerFv() {}

extern "C" void func_804B4BDC() {}

extern "C" void func_804B4C7C() {}

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

extern "C" void Move__16CTaskColiManagerFv() {}

void CTaskColiManager::Draw() {
    // Empty override — no draw-time work required.
}

extern "C" void func_804B4E10() {}

extern "C" void func_804B5088() {}

extern "C" void func_804B526C() {}

extern "C" void func_804B54D4() {}

extern "C" void func_804B5658() {}

extern "C" void create__16CTaskColiManagerFv() {}

// --- Explicit template specializations for CTTask<CTaskColiManager> ---
template<> CTTask<CTaskColiManager>::~CTTask() {}
template<> void CTTask<CTaskColiManager>::Move() {}
template<> void CTTask<CTaskColiManager>::Draw() {}
extern "C" void sinit_804B598C() {}
