// Auto-scaffolded catalog TU for kyoshin/CSelShopWin
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/CSelShopWin.hpp"

// LLM-HARNESS-BEGIN: us-8022e7c8
/* Returns whether the shop window has finished loading resources
   and is ready for interaction. (C-linkage: func_8022C8D0) */
extern "C" u8 func_8022C8D0(CSelShopWin* self) {
    return self->mIsLoaded;
}
// LLM-HARNESS-END: us-8022e7c8

// LLM-HARNESS-BEGIN: us-8022e7d0
/* Returns whether a show/hide animation is currently active.
   (C-linkage: func_8022C8D8) */
extern "C" u8 func_8022C8D8(CSelShopWin* self) {
    return self->mAnimActive;
}
// LLM-HARNESS-END: us-8022e7d0

// LLM-HARNESS-BEGIN: us-8022e918
extern "C" void func_8022CA20() {}
// LLM-HARNESS-END: us-8022e918

// LLM-HARNESS-BEGIN: us-8022e964
extern "C" void func_8022CA6C() {}
// LLM-HARNESS-END: us-8022e964

// LLM-HARNESS-BEGIN: us-8022e980
extern "C" void OnFileEvent__11CSelShopWinFP10CEventFile() {}
// LLM-HARNESS-END: us-8022e980
