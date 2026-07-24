// Auto-scaffolded catalog TU for kyoshin/menu/CMenuKizunaTalkList
// Mangled extern stubs for llm-harness / coop selection.
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/menu/CMenuKizunaTalkList.hpp"

// LLM-HARNESS-BEGIN: us-80274258
extern "C" void __ct__CMenuKizunaTalkList() {}
// LLM-HARNESS-END: us-80274258

// LLM-HARNESS-BEGIN: us-8027431c
extern "C" void __dt__19CMenuKizunaTalkListFv(CMenuKizunaTalkList* self, int deletionFlag) {}
// LLM-HARNESS-END: us-8027431c

// LLM-HARNESS-BEGIN: us-80274394
extern "C" void Init__19CMenuKizunaTalkListFv() {}
// LLM-HARNESS-END: us-80274394

// LLM-HARNESS-BEGIN: us-80274678
extern "C" void Term__19CMenuKizunaTalkListFv() {}
// LLM-HARNESS-END: us-80274678

// LLM-HARNESS-BEGIN: us-802746e4
extern "C" void Move__19CMenuKizunaTalkListFv() {}
// LLM-HARNESS-END: us-802746e4

// LLM-HARNESS-BEGIN: us-802747f4
extern "C" void cbRenderBefore__19CMenuKizunaTalkListFv(CMenuKizunaTalkList* self) {}
// LLM-HARNESS-END: us-802747f4

// LLM-HARNESS-BEGIN: us-80274898
extern "C" void func_80272414() {}
// LLM-HARNESS-END: us-80274898

// LLM-HARNESS-BEGIN: us-8027490c
extern "C" void func_80272488() {}
// LLM-HARNESS-END: us-8027490c

// LLM-HARNESS-BEGIN: us-8027491c
extern "C" void func_80272498() {}
// LLM-HARNESS-END: us-8027491c

// LLM-HARNESS-BEGIN: us-80274994
extern "C" void func_80272510() {}
// LLM-HARNESS-END: us-80274994

// LLM-HARNESS-BEGIN: us-802749e4
extern "C" void func_80272560() {}
// LLM-HARNESS-END: us-802749e4

// LLM-HARNESS-BEGIN: us-80274b18
extern "C" void func_80272694() {}
// LLM-HARNESS-END: us-80274b18

// Forward declarations for member functions called by adjustor thunks
// (used via direct function pointer to guarantee tail-call match).
extern "C" void cbRenderBefore__19CMenuKizunaTalkListFv(CMenuKizunaTalkList* self);
extern "C" void __dt__19CMenuKizunaTalkListFv(CMenuKizunaTalkList* self, int deletionFlag);

// LLM-HARNESS-BEGIN: us-80274b68
// IScnRender vtable adjustor thunk for cbRenderBefore.
// When IScnRender virtual functions dispatch through IScnRender*,
// 'this' points to the IScnRender subobject at offset +0x58 within
// CMenuKizunaTalkList. This thunk adjusts it back and tail-calls the
// real implementation.
extern "C" void func_802726E4(IScnRender* self) {
    ((void(*)(CMenuKizunaTalkList*))cbRenderBefore__19CMenuKizunaTalkListFv)(
        reinterpret_cast<CMenuKizunaTalkList*>(
            reinterpret_cast<uintptr_t>(self) - 0x58));
}
// LLM-HARNESS-END: us-80274b68

// LLM-HARNESS-BEGIN: us-80274b70
// IScnRender vtable adjustor thunk for ~CMenuKizunaTalkList.
// Same pointer adjustment as func_802726E4. r4 (deletion flag)
// is passed through from the IScnRender vtable caller unchanged.
// The single-arg function pointer cast avoids MWCC zeroing r4.
extern "C" void func_802726EC(IScnRender* self) {
    ((void(*)(CMenuKizunaTalkList*))__dt__19CMenuKizunaTalkListFv)(
        reinterpret_cast<CMenuKizunaTalkList*>(
            reinterpret_cast<uintptr_t>(self) - 0x58));
}
// LLM-HARNESS-END: us-80274b70
