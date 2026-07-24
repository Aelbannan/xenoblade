// Skip-timer widget: shown during cutscene skips.
// Most functions are still skeleton stubs; only the IScnRender vtable
// adjustment thunks (func_8029F098, func_8029F0A0) are FULL_MATCH.

#include "kyoshin/menu/CMenuSkipTimer.hpp"


// ---------- CMenuSkipTimer member stubs (NonMatching) ----------
// Marked never_inline to prevent the thunks from collapsing into blr.

extern "C" __attribute__((never_inline)) void __ct__CMenuSkipTimer() {}
extern "C" __attribute__((never_inline)) void __dt__14CMenuSkipTimerFv(void* self) {}
extern "C" __attribute__((never_inline)) void Init__14CMenuSkipTimerFv() {}
extern "C" __attribute__((never_inline)) void Term__14CMenuSkipTimerFv() {}
extern "C" __attribute__((never_inline)) void Move__14CMenuSkipTimerFv() {}
extern "C" __attribute__((never_inline)) void cbRenderBefore__14CMenuSkipTimerFv(void* self) {}


// ---------- Non-member helper stubs (same-TU, not yet identified) ----------

// LLM-HARNESS-BEGIN: us-802a1640
extern "C" __attribute__((never_inline)) void func_8029EDE4() {}
// LLM-HARNESS-END: us-802a1640

// LLM-HARNESS-BEGIN: us-802a16b4
extern "C" __attribute__((never_inline)) void func_8029EE58() {}
// LLM-HARNESS-END: us-802a16b4

// LLM-HARNESS-BEGIN: us-802a16c4
extern "C" __attribute__((never_inline)) void func_8029EE68() {}
// LLM-HARNESS-END: us-802a16c4

// LLM-HARNESS-BEGIN: us-802a173c
extern "C" __attribute__((never_inline)) void func_8029EEE0() {}
// LLM-HARNESS-END: us-802a173c

// LLM-HARNESS-BEGIN: us-802a178c
extern "C" __attribute__((never_inline)) void func_8029EF30() {}
// LLM-HARNESS-END: us-802a178c

// LLM-HARNESS-BEGIN: us-802a18a4
extern "C" __attribute__((never_inline)) void func_8029F048() {}
// LLM-HARNESS-END: us-802a18a4


// ---------- FULL_MATCH: vtable adjustment thunks ----------

// Vtable adjustment thunk: IScnRender::cbRenderBefore → CMenuSkipTimer::cbRenderBefore.
// The IScnRender vtable subobject is at +0x58 within CMenuSkipTimer.  When virtual
// dispatch arrives through that subobject, the IScnRender* in r3 needs to be adjusted
// back to the enclosing CMenuSkipTimer* before calling the real implementation.
// Tail-call via direct function symbol to let MWCC optimise to `b` (not `bl` + `blr`).
//
// LLM-HARNESS-BEGIN: us-802a18f4
extern "C" void func_8029F098(IScnRender* self) {
    cbRenderBefore__14CMenuSkipTimerFv(
        reinterpret_cast<CMenuSkipTimer*>(
            reinterpret_cast<uintptr_t>(self) - 0x58));
}
// LLM-HARNESS-END: us-802a18f4

// Vtable adjustment thunk: IScnRender::~IScnRender → CMenuSkipTimer::~CMenuSkipTimer.
// Same +0x58 this-adjustment as func_8029F098.
//
// LLM-HARNESS-BEGIN: us-802a18fc
extern "C" void func_8029F0A0(IScnRender* self) {
    __dt__14CMenuSkipTimerFv(
        reinterpret_cast<CMenuSkipTimer*>(
            reinterpret_cast<uintptr_t>(self) - 0x58));
}
// LLM-HARNESS-END: us-802a18fc
