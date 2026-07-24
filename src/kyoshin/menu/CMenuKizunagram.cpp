// Auto-scaffolded catalog TU for kyoshin/menu/CMenuKizunagram
// Mangled extern stubs for llm-harness / coop selection.
// Replace stubs with high-level C/C++ during decomp.

#include <types.h>
#include "kyoshin/menu/CMenuKizunagram.hpp"

// LLM-HARNESS-BEGIN: us-80258ba4
extern "C" void __ct__CMenuKizunagram() {}
// LLM-HARNESS-END: us-80258ba4

// LLM-HARNESS-BEGIN: us-80258c90
extern "C" void __dt__15CMenuKizunagramFv(void* self) {}
// LLM-HARNESS-END: us-80258c90

// LLM-HARNESS-BEGIN: us-80258d20
extern "C" void Init__15CMenuKizunagramFv() {}
// LLM-HARNESS-END: us-80258d20

// LLM-HARNESS-BEGIN: us-802591dc
extern "C" void Term__15CMenuKizunagramFv() {}
// LLM-HARNESS-END: us-802591dc

// LLM-HARNESS-BEGIN: us-80259278
extern "C" void Move__15CMenuKizunagramFv() {}
// LLM-HARNESS-END: us-80259278

// LLM-HARNESS-BEGIN: us-802593f4
extern "C" void cbRenderBefore__15CMenuKizunagramFv(void* self) {}
// LLM-HARNESS-END: us-802593f4

// LLM-HARNESS-BEGIN: us-802594c8
extern "C" void func_8025728C() {}
// LLM-HARNESS-END: us-802594c8

// LLM-HARNESS-BEGIN: us-80259544
extern "C" void func_80257308() {}
// LLM-HARNESS-END: us-80259544

// LLM-HARNESS-BEGIN: us-80259554
extern "C" void func_80257318() {}
// LLM-HARNESS-END: us-80259554

// LLM-HARNESS-BEGIN: us-8025959c
extern "C" void func_80257360() {}
// LLM-HARNESS-END: us-8025959c

// LLM-HARNESS-BEGIN: us-802595f4
extern "C" void func_802573B8() {}
// LLM-HARNESS-END: us-802595f4

// LLM-HARNESS-BEGIN: us-80259684
extern "C" void func_80257448() {}
// LLM-HARNESS-END: us-80259684

// LLM-HARNESS-BEGIN: us-802596d4
extern "C" void func_80257498() {}
// LLM-HARNESS-END: us-802596d4

// LLM-HARNESS-BEGIN: us-80259940
extern "C" void func_80257704() {}
// LLM-HARNESS-END: us-80259940

// LLM-HARNESS-BEGIN: us-80259990
extern "C" void func_80257754() {}
// LLM-HARNESS-END: us-80259990

// LLM-HARNESS-BEGIN: us-80259a2c
extern "C" void func_802577F0() {}
// LLM-HARNESS-END: us-80259a2c

// LLM-HARNESS-BEGIN: us-80259a7c
extern "C" void func_80257840() {}
// LLM-HARNESS-END: us-80259a7c

// LLM-HARNESS-BEGIN: us-80259bd0
extern "C" void func_80257994() {}
// LLM-HARNESS-END: us-80259bd0

// LLM-HARNESS-BEGIN: us-80259c68
extern "C" void func_80257A2C() {}
// LLM-HARNESS-END: us-80259c68

// Forward declarations for member functions called by adjustor thunks
// (used via direct function pointer to guarantee tail-call match).
extern "C" void cbRenderBefore__15CMenuKizunagramFv(CMenuKizunagram* self);
extern "C" void __dt__15CMenuKizunagramFv(CMenuKizunagram* self, int deletionFlag);

// LLM-HARNESS-BEGIN: us-80259cb8
// IScnRender vtable adjustor thunk for cbRenderBefore.
// When IScnRender virtual functions dispatch through IScnRender*,
// 'this' points to the IScnRender subobject at offset +0x58 within
// CMenuKizunagram. This thunk adjusts it back and tail-calls the
// real implementation.
extern "C" void func_80257A7C(IScnRender* self) {
    ((void(*)(CMenuKizunagram*))cbRenderBefore__15CMenuKizunagramFv)(
        reinterpret_cast<CMenuKizunagram*>(
            reinterpret_cast<uintptr_t>(self) - 0x58));
}
// LLM-HARNESS-END: us-80259cb8

// LLM-HARNESS-BEGIN: us-80259cc0
// IScnRender vtable adjustor thunk for ~CMenuKizunagram.
// Same pointer adjustment as func_80257A7C. r4 (deletion flag)
// is passed through from the IScnRender vtable caller unchanged.
// The single-arg function pointer cast avoids MWCC zeroing r4.
extern "C" void func_80257A84(IScnRender* self) {
    ((void(*)(CMenuKizunagram*))__dt__15CMenuKizunagramFv)(
        reinterpret_cast<CMenuKizunagram*>(
            reinterpret_cast<uintptr_t>(self) - 0x58));
}
// LLM-HARNESS-END: us-80259cc0
