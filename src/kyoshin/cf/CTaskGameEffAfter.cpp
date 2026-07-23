// Auto-scaffolded catalog TU for kyoshin/cf/CTaskGameEffAfter
// Mangled extern stubs for llm-harness / coop selection.
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/harness_catalog.hpp"

// LLM-HARNESS-BEGIN: us-80061bf4
extern "C" void __ct__CTaskGameEffAfter() {}
// LLM-HARNESS-END: us-80061bf4

// LLM-HARNESS-BEGIN: us-80061cf0
extern "C" void Draw__17CTaskGameEffAfterFv() {}
// LLM-HARNESS-END: us-80061cf0

// LLM-HARNESS-BEGIN: us-80061cf4
extern "C" void Term__17CTaskGameEffAfterFv() {}
// LLM-HARNESS-END: us-80061cf4

// LLM-HARNESS-BEGIN: us-80061cf8
extern "C" void Init__17CTaskGameEffAfterFv() {}
// LLM-HARNESS-END: us-80061cf8

// LLM-HARNESS-BEGIN: us-80061cfc
extern "C" void __dt__17CTaskGameEffAfterFv() {}
// LLM-HARNESS-END: us-80061cfc

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
// LLM-HARNESS-BEGIN: us-80061ca0
template<> CTTask<CTaskGameEffAfter>::~CTTask() {}
// LLM-HARNESS-END: us-80061ca0
// LLM-HARNESS-BEGIN: us-80061d50
template<> void CTTask<CTaskGameEffAfter>::Move() {}
// LLM-HARNESS-END: us-80061d50
// LLM-HARNESS-BEGIN: us-80061d98
template<> void CTTask<CTaskGameEffAfter>::Draw() {}
// LLM-HARNESS-END: us-80061d98
