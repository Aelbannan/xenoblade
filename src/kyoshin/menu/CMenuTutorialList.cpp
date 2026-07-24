// Auto-scaffolded catalog TU for kyoshin/menu/CMenuTutorialList
// Mangled extern stubs for llm-harness / coop selection.
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/menu/CMenuTutorialList.hpp"
#include <types.h>

// LLM-HARNESS-BEGIN: us-802ae4e8
extern "C" void __ct__CMenuTutorialList() {}
// LLM-HARNESS-END: us-802ae4e8

// LLM-HARNESS-BEGIN: us-802ae5d4
extern "C" void __dt__17CMenuTutorialListFv(void* self) {}
// LLM-HARNESS-END: us-802ae5d4

// LLM-HARNESS-BEGIN: us-802ae658
extern "C" void Init__17CMenuTutorialListFv() {}
// LLM-HARNESS-END: us-802ae658

// LLM-HARNESS-BEGIN: us-802ae954
extern "C" void Term__17CMenuTutorialListFv() {}
// LLM-HARNESS-END: us-802ae954

// LLM-HARNESS-BEGIN: us-802ae9d8
extern "C" void Move__17CMenuTutorialListFv() {}
// LLM-HARNESS-END: us-802ae9d8

// LLM-HARNESS-BEGIN: us-802aeb1c
extern "C" void cbRenderBefore__17CMenuTutorialListFv(void* self) {}
// LLM-HARNESS-END: us-802aeb1c

// LLM-HARNESS-BEGIN: us-802aebcc
extern "C" void func_802AC494() {}
// LLM-HARNESS-END: us-802aebcc

// LLM-HARNESS-BEGIN: us-802aec48
extern "C" void func_802AC510() {}
// LLM-HARNESS-END: us-802aec48

// LLM-HARNESS-BEGIN: us-802aec58
extern "C" void func_802AC520() {}
// LLM-HARNESS-END: us-802aec58

// LLM-HARNESS-BEGIN: us-802aecd0
extern "C" void func_802AC598() {}
// LLM-HARNESS-END: us-802aecd0

// LLM-HARNESS-BEGIN: us-802aed20
extern "C" void func_802AC5E8() {}
// LLM-HARNESS-END: us-802aed20

// LLM-HARNESS-BEGIN: us-802aefdc
extern "C" void func_802AC8A4() {}
// LLM-HARNESS-END: us-802aefdc

// LLM-HARNESS-BEGIN: us-802af02c
extern "C" void func_802AC8F4() {}
// LLM-HARNESS-END: us-802af02c

// LLM-HARNESS-BEGIN: us-802af0a8
extern "C" void func_802AC970() {}
// LLM-HARNESS-END: us-802af0a8

// LLM-HARNESS-BEGIN: us-802af170
extern "C" void func_802ACA38() {}
// LLM-HARNESS-END: us-802af170

// LLM-HARNESS-BEGIN: us-802af288
extern "C" void func_802ACB50() {}
// LLM-HARNESS-END: us-802af288

// --- FULL_MATCH thunks ---
// These are IScnRender vtable trampolines.  When CMenuTutorialList inherits from
// IScnRender (which sits at offset +0x58 in the MI layout), calling a virtual
// function through the IScnRender vtable delivers an IScnRender* (pointing at
// the subobject within the full object).  Each thunk adjusts 'this' back by
// -0x58 so the real CMenuTutorialList implementation receives the correct pointer.

// LLM-HARNESS-BEGIN: us-802af304
class IScnRender;
typedef void (*CbRenderBeforeFunc)(CMenuTutorialList*);
extern "C" void func_802ACBCC(IScnRender* self) {
    ((CbRenderBeforeFunc)cbRenderBefore__17CMenuTutorialListFv)(
        reinterpret_cast<CMenuTutorialList*>(
            reinterpret_cast<char*>(self) - 0x58));
}
// LLM-HARNESS-END: us-802af304

// LLM-HARNESS-BEGIN: us-802af30c
typedef void (*DtorFunc)(CMenuTutorialList*);
extern "C" void func_802ACBD4(IScnRender* self) {
    ((DtorFunc)__dt__17CMenuTutorialListFv)(
        reinterpret_cast<CMenuTutorialList*>(
            reinterpret_cast<char*>(self) - 0x58));
}
// LLM-HARNESS-END: us-802af30c
