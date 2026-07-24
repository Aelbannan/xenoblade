// Auto-scaffolded catalog TU for kyoshin/CTaskGamePic
// Mangled extern stubs for llm-harness / coop selection.
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/harness_catalog.hpp"

// LLM-HARNESS-BEGIN: us-802972b0
extern "C" void Draw__12CTaskGamePicFv() {}
// LLM-HARNESS-END: us-802972b0









// LLM-HARNESS-BEGIN: us-80297998
extern "C" void func_8029539C(void* self) {}
// LLM-HARNESS-END: us-80297998

// LLM-HARNESS-BEGIN: us-80297a44
extern "C" void create__12CTaskGamePicFv() {}
// LLM-HARNESS-END: us-80297a44


// LLM-HARNESS-BEGIN: us-80297b40
extern "C" void OnFileEvent__12CTaskGamePicFP10CEventFile(void* self) { ((void(*)(void*))func_8029539C)((char*)self - 0x54); }
// LLM-HARNESS-END: us-80297b40

// LLM-HARNESS-BEGIN: us-80297b48
extern "C" void func_8029554C(void* self) { ((void(*)(void*))__dt__12CTaskGamePicFv)((char*)self - 0x54); }
// LLM-HARNESS-END: us-80297b48

// LLM-HARNESS-BEGIN: us-80297b50
extern "C" void func_80295554(void* self) { ((void(*)(void*))cbRenderBefore__12CTaskGamePicFv)((char*)self - 0x58); }
// LLM-HARNESS-END: us-80297b50

// LLM-HARNESS-BEGIN: us-80297b58
extern "C" void func_8029555C(void* self) { ((void(*)(void*))__dt__12CTaskGamePicFv)((char*)self - 0x58); }
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
// LLM-HARNESS-BEGIN: us-80297ab0
template<> void CTTask<CTaskGamePic>::Move() {}
// LLM-HARNESS-END: us-80297ab0
// LLM-HARNESS-BEGIN: us-80297af8
template<> void CTTask<CTaskGamePic>::Draw() {}
// LLM-HARNESS-END: us-80297af8
