// Auto-scaffolded catalog TU for kyoshin/CTaskGameEff
// Mangled extern stubs for llm-harness / coop selection.
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/harness_catalog.hpp"

// LLM-HARNESS-BEGIN: us-80045058
extern "C" void __ct__CTaskGameEff() {}
// LLM-HARNESS-END: us-80045058

// LLM-HARNESS-BEGIN: us-80045150
extern "C" void __dt__80044BB0() {}
// LLM-HARNESS-END: us-80045150

// LLM-HARNESS-BEGIN: us-800451e0
extern "C" void __dt__Q212CTaskGameEff18CEffRenderHighPrioFv() {}
// LLM-HARNESS-END: us-800451e0

// LLM-HARNESS-BEGIN: us-80045220
extern "C" void __dt___reslist_base_CScn() {}
// LLM-HARNESS-END: us-80045220

// LLM-HARNESS-BEGIN: us-800452cc
extern "C" void __dt__reslist_CScn() {}
// LLM-HARNESS-END: us-800452cc

// LLM-HARNESS-BEGIN: us-8004531c
extern "C" void __dt__12CTaskGameEffFv() {}
// LLM-HARNESS-END: us-8004531c

// LLM-HARNESS-BEGIN: us-80045394
extern "C" bool func_80044DF4() { return false; }
// LLM-HARNESS-END: us-80045394

// LLM-HARNESS-BEGIN: us-8004539c
extern "C" void Init__12CTaskGameEffFv() {}
// LLM-HARNESS-END: us-8004539c

// LLM-HARNESS-BEGIN: us-80045498
extern "C" void Term__12CTaskGameEffFv() {}
// LLM-HARNESS-END: us-80045498

// LLM-HARNESS-BEGIN: us-8004552c
extern "C" void Move__12CTaskGameEffFv() {}
// LLM-HARNESS-END: us-8004552c

// LLM-HARNESS-BEGIN: us-8004555c
extern "C" void func_80044FBC__FUl() {}
// LLM-HARNESS-END: us-8004555c

// LLM-HARNESS-BEGIN: us-800455d8
extern "C" void cbRenderBefore__12CTaskGameEffFv() {}
// LLM-HARNESS-END: us-800455d8

// LLM-HARNESS-BEGIN: us-800455e4
extern "C" void func_80045044() {}
// LLM-HARNESS-END: us-800455e4

// LLM-HARNESS-BEGIN: us-80045668
extern "C" void func_800450C8() {}
// LLM-HARNESS-END: us-80045668

// LLM-HARNESS-BEGIN: us-8004566c
extern "C" void func_800450CC() {}
// LLM-HARNESS-END: us-8004566c

// LLM-HARNESS-BEGIN: us-800456dc
extern "C" void func_8004513C() {}
// LLM-HARNESS-END: us-800456dc

// LLM-HARNESS-BEGIN: us-80045778
extern "C" void func_800451D8() {}
// LLM-HARNESS-END: us-80045778

// LLM-HARNESS-BEGIN: us-80045824
extern "C" void func_80045284() {}
// LLM-HARNESS-END: us-80045824

// LLM-HARNESS-BEGIN: us-8004588c
extern "C" void func_800452EC() {}
// LLM-HARNESS-END: us-8004588c

// LLM-HARNESS-BEGIN: us-8004598c
extern "C" void func_800453EC() {}
// LLM-HARNESS-END: us-8004598c

// LLM-HARNESS-BEGIN: us-80045a4c
extern "C" void Draw__12CTaskGameEffFv() {}
// LLM-HARNESS-END: us-80045a4c

// LLM-HARNESS-BEGIN: us-80045ae0
extern "C" bool func_80045540() { return false; }
// LLM-HARNESS-END: us-80045ae0

// LLM-HARNESS-BEGIN: us-80045ae8
extern "C" bool func_80045548() { return false; }
// LLM-HARNESS-END: us-80045ae8

// LLM-HARNESS-BEGIN: us-80045af0
extern "C" bool func_80045550() { return false; }
// LLM-HARNESS-END: us-80045af0

// LLM-HARNESS-BEGIN: us-80045af8
extern "C" bool func_80045558() { return false; }
// LLM-HARNESS-END: us-80045af8

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

class CTaskGameEff;
// LLM-HARNESS-BEGIN: us-80045190
template<> CTTask<CTaskGameEff>::~CTTask() {}
// LLM-HARNESS-END: us-80045190
// LLM-HARNESS-BEGIN: us-80045a50
template<> void CTTask<CTaskGameEff>::Move() {}
// LLM-HARNESS-END: us-80045a50
// LLM-HARNESS-BEGIN: us-80045a98
template<> void CTTask<CTaskGameEff>::Draw() {}
// LLM-HARNESS-END: us-80045a98
