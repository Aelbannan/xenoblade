// Auto-scaffolded catalog TU for kyoshin/cf/CfTaskMain
// Mangled extern stubs for llm-harness / coop selection.
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/harness_catalog.hpp"

// LLM-HARNESS-BEGIN: us-8006a17c
extern "C" void __dt__Q22cf10CfTaskMainFv() {}
// LLM-HARNESS-END: us-8006a17c

// LLM-HARNESS-BEGIN: us-8006a1d8
extern "C" void Init__Q22cf10CfTaskMainFv() {}
// LLM-HARNESS-END: us-8006a1d8

// LLM-HARNESS-BEGIN: us-8006a1dc
extern "C" void Term__Q22cf10CfTaskMainFv() {}
// LLM-HARNESS-END: us-8006a1dc

// LLM-HARNESS-BEGIN: us-8006a1e0
extern "C" void Move__Q22cf10CfTaskMainFv() {}
// LLM-HARNESS-END: us-8006a1e0

// LLM-HARNESS-BEGIN: us-8006a1f4
extern "C" void Draw__Q22cf10CfTaskMainFv() {}
// LLM-HARNESS-END: us-8006a1f4

// LLM-HARNESS-BEGIN: us-8006a1f8
extern "C" void Tail__Q22cf10CfTaskMainFv() {}
// LLM-HARNESS-END: us-8006a1f8

// LLM-HARNESS-BEGIN: us-8006a1fc
extern "C" void __ct__800697E8() {}
// LLM-HARNESS-END: us-8006a1fc

// LLM-HARNESS-BEGIN: us-8006a358
extern "C" bool func_80069944() { return false; }
// LLM-HARNESS-END: us-8006a358

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

namespace cf { class CfTaskMain; }
// LLM-HARNESS-BEGIN: us-8006a124
template<> CTTask<cf::CfTaskMain>::~CTTask() {}
// LLM-HARNESS-END: us-8006a124
// LLM-HARNESS-BEGIN: us-8006a2c8
template<> void CTTask<cf::CfTaskMain>::Move() {}
// LLM-HARNESS-END: us-8006a2c8
// LLM-HARNESS-BEGIN: us-8006a310
template<> void CTTask<cf::CfTaskMain>::Draw() {}
// LLM-HARNESS-END: us-8006a310
