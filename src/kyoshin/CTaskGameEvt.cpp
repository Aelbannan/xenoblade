// Auto-scaffolded catalog TU for kyoshin/CTaskGameEvt
// Mangled extern stubs for llm-harness / coop selection.
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/harness_catalog.hpp"

// LLM-HARNESS-BEGIN: us-80297b60
extern "C" void __ct__CTaskGameEvt() {}
// LLM-HARNESS-END: us-80297b60

// LLM-HARNESS-BEGIN: us-80297c4c
extern "C" void __dt__12CTaskGameEvtFv(void* self) {}
// LLM-HARNESS-END: us-80297c4c

// LLM-HARNESS-BEGIN: us-80297ca0
extern "C" void func_802956A4(void) {}
// LLM-HARNESS-END: us-80297ca0

// LLM-HARNESS-BEGIN: us-80297ca4
extern "C" void func_802956A8(void) {}
// LLM-HARNESS-END: us-80297ca4

// LLM-HARNESS-BEGIN: us-80297ca8
extern "C" void Init__12CTaskGameEvtFv() {}
// LLM-HARNESS-END: us-80297ca8

// LLM-HARNESS-BEGIN: us-80297cc8
extern "C" void Term__12CTaskGameEvtFv() {}
// LLM-HARNESS-END: us-80297cc8

// LLM-HARNESS-BEGIN: us-80297ce0
extern "C" void Move__12CTaskGameEvtFv() {}
// LLM-HARNESS-END: us-80297ce0

// LLM-HARNESS-BEGIN: us-80297d5c
extern "C" void cbRenderBefore__12CTaskGameEvtFv(void* self) {}
// LLM-HARNESS-END: us-80297d5c

// LLM-HARNESS-BEGIN: us-80297d60
extern "C" int func_80295764(void* self) { return 0; }
// LLM-HARNESS-END: us-80297d60

// LLM-HARNESS-BEGIN: us-80297d68
extern "C" void create__12CTaskGameEvtFv() {}
// LLM-HARNESS-END: us-80297d68

// LLM-HARNESS-BEGIN: us-80297dd0
extern "C" void Draw__12CTaskGameEvtFv() {}
// LLM-HARNESS-END: us-80297dd0

// LLM-HARNESS-BEGIN: us-80297e64
extern "C" void OnFileEvent__12CTaskGameEvtFP10CEventFile(void* self) { ((void(*)(void*))func_80295764)((char*)self - 0x54); }
// LLM-HARNESS-END: us-80297e64

// LLM-HARNESS-BEGIN: us-80297e6c
extern "C" void func_80295870(void* self) { ((void(*)(void*))__dt__12CTaskGameEvtFv)((char*)self - 0x54); }
// LLM-HARNESS-END: us-80297e6c

// LLM-HARNESS-BEGIN: us-80297e74
extern "C" void func_80295878(void* self) { ((void(*)(void*))cbRenderBefore__12CTaskGameEvtFv)((char*)self - 0x58); }
// LLM-HARNESS-END: us-80297e74

// LLM-HARNESS-BEGIN: us-80297e7c
extern "C" void func_80295880(void* self) { ((void(*)(void*))__dt__12CTaskGameEvtFv)((char*)self - 0x58); }
// LLM-HARNESS-END: us-80297e7c

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

class CTaskGameEvt;
// LLM-HARNESS-BEGIN: us-80297bfc
template<> CTTask<CTaskGameEvt>::~CTTask() {}
// LLM-HARNESS-END: us-80297bfc
// LLM-HARNESS-BEGIN: us-80297dd4
template<> void CTTask<CTaskGameEvt>::Move() {}
// LLM-HARNESS-END: us-80297dd4
// LLM-HARNESS-BEGIN: us-80297e1c
template<> void CTTask<CTaskGameEvt>::Draw() {}
// LLM-HARNESS-END: us-80297e1c
