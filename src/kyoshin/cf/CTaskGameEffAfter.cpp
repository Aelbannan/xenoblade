// Auto-scaffolded catalog TU for kyoshin/cf/CTaskGameEffAfter
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/harness_catalog.hpp"

extern "C" void __ct__CTaskGameEffAfter() {}

extern "C" void Draw__17CTaskGameEffAfterFv() {}

extern "C" void Term__17CTaskGameEffAfterFv() {}

extern "C" void Init__17CTaskGameEffAfterFv() {}

extern "C" void __dt__17CTaskGameEffAfterFv() {}

// --- hard-symbol stubs (scaffold_hard_symbols) ---
// Local CTTask (out-of-line Move/Draw/dtor) for harness stubs.
// Do not include monolib/work/CTTask.hpp here — its inline methods collide.
template <typename T>
class CTTask {
public:
    CTTask();
    virtual ~CTTask();
    virtual void Move();
    virtual void Draw();
};

class CTaskGameEffAfter;
template<> CTTask<CTaskGameEffAfter>::~CTTask() {}
template<> void CTTask<CTaskGameEffAfter>::Move() {}
template<> void CTTask<CTaskGameEffAfter>::Draw() {}
