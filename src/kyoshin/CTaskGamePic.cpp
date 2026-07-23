// Auto-scaffolded catalog TU for kyoshin/CTaskGamePic
// Mangled extern stubs for llm-harness / coop selection.
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/harness_catalog.hpp"

// LLM-HARNESS-BEGIN: us-802972b0
extern "C" void __ct__CTaskGamePic() {}
// LLM-HARNESS-END: us-802972b0

// LLM-HARNESS-BEGIN: us-80297400
extern "C" void __dt__12CTaskGamePicFv() {}
// LLM-HARNESS-END: us-80297400

// LLM-HARNESS-BEGIN: us-80297454
extern "C" void func_80294E58() {}
// LLM-HARNESS-END: us-80297454

// LLM-HARNESS-BEGIN: us-802974bc
extern "C" void func_80294EC0() {}
// LLM-HARNESS-END: us-802974bc

// LLM-HARNESS-BEGIN: us-80297520
extern "C" void Init__12CTaskGamePicFv() {}
// LLM-HARNESS-END: us-80297520

// LLM-HARNESS-BEGIN: us-80297540
extern "C" void Term__12CTaskGamePicFv() {}
// LLM-HARNESS-END: us-80297540

// LLM-HARNESS-BEGIN: us-802975b4
extern "C" void Move__12CTaskGamePicFv() {}
// LLM-HARNESS-END: us-802975b4

// LLM-HARNESS-BEGIN: us-802976f0
extern "C" void cbRenderBefore__12CTaskGamePicFv() {}
// LLM-HARNESS-END: us-802976f0

// LLM-HARNESS-BEGIN: us-80297984
extern "C" void func_80295388() {}
// LLM-HARNESS-END: us-80297984

// LLM-HARNESS-BEGIN: us-80297998
extern "C" void func_8029539C() {}
// LLM-HARNESS-END: us-80297998

// LLM-HARNESS-BEGIN: us-80297a44
extern "C" void create__12CTaskGamePicFv() {}
// LLM-HARNESS-END: us-80297a44

// LLM-HARNESS-BEGIN: us-80297aac
extern "C" void Draw__12CTaskGamePicFv() {}
// LLM-HARNESS-END: us-80297aac

// LLM-HARNESS-BEGIN: us-80297b40
extern "C" bool OnFileEvent__12CTaskGamePicFP10CEventFile() { return false; }
// LLM-HARNESS-END: us-80297b40

// LLM-HARNESS-BEGIN: us-80297b48
extern "C" bool func_8029554C() { return false; }
// LLM-HARNESS-END: us-80297b48

// LLM-HARNESS-BEGIN: us-80297b50
extern "C" bool func_80295554() { return false; }
// LLM-HARNESS-END: us-80297b50

// LLM-HARNESS-BEGIN: us-80297b58
extern "C" bool func_8029555C() { return false; }
// LLM-HARNESS-END: us-80297b58

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

class CTaskGamePic;
// LLM-HARNESS-BEGIN: us-802973b0
template<> CTTask<CTaskGamePic>::~CTTask() {}
// LLM-HARNESS-END: us-802973b0
// LLM-HARNESS-BEGIN: us-80297ab0
template<> void CTTask<CTaskGamePic>::Move() {}
// LLM-HARNESS-END: us-80297ab0
// LLM-HARNESS-BEGIN: us-80297af8
template<> void CTTask<CTaskGamePic>::Draw() {}
// LLM-HARNESS-END: us-80297af8
