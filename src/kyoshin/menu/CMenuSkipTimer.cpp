// Skip-timer widget: shown during cutscene skips.
// Most functions are still skeleton stubs; only the IScnRender vtable
// adjustment thunks (func_8029F098, func_8029F0A0) are FULL_MATCH.

#include "kyoshin/menu/CMenuSkipTimer.hpp"


// ---------- CMenuSkipTimer member stubs (NonMatching) ----------
// Marked never_inline to prevent the thunks from collapsing into blr.

__attribute__((never_inline)) void __ct__CMenuSkipTimer(){}
__attribute__((never_inline)) void __dt__14CMenuSkipTimerFv(void* self){}
__attribute__((never_inline)) void Init__14CMenuSkipTimerFv(){}
__attribute__((never_inline)) void Term__14CMenuSkipTimerFv(){}
__attribute__((never_inline)) void Move__14CMenuSkipTimerFv(){}
__attribute__((never_inline)) void cbRenderBefore__14CMenuSkipTimerFv(void* self){}


// ---------- Non-member helper stubs (same-TU, not yet identified) ----------

__attribute__((never_inline)) void func_8029EDE4(){}


__attribute__((never_inline)) void func_8029EE68(){}

__attribute__((never_inline)) void func_8029EEE0(){}

__attribute__((never_inline)) void func_8029EF30(){}

__attribute__((never_inline)) void func_8029F048(){}


// ---------- FULL_MATCH: vtable adjustment thunks ----------

// Vtable adjustment thunk: IScnRender::cbRenderBefore → CMenuSkipTimer::cbRenderBefore.
// The IScnRender vtable subobject is at +0x58 within CMenuSkipTimer.  When virtual
// dispatch arrives through that subobject, the IScnRender* in r3 needs to be adjusted
// back to the enclosing CMenuSkipTimer* before calling the real implementation.
// Tail-call via direct function symbol to let MWCC optimise to `b` (not `bl` + `blr`).
//
void CMenuSkipTimer::func_8029F098() {
    cbRenderBefore();
}

// Vtable adjustment thunk: IScnRender::~IScnRender → CMenuSkipTimer::~CMenuSkipTimer.
// Same +0x58 this-adjustment as func_8029F098.
//
void CMenuSkipTimer::func_8029F0A0() {
    this->~CMenuSkipTimer();
}

extern unsigned long lbl_eu_80664A48;
extern "C" unsigned long func_8029EE58(void) { return lbl_eu_80664A48 != 0; }
