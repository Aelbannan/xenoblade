// Auto-scaffolded catalog TU for kyoshin/CSelShopWin
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/CSelShopWin.hpp"

/* Returns whether the shop window has finished loading resources
   and is ready for interaction. (C-linkage: func_8022C8D0) */
u8 CSelShopWin::func_8022C8D0() {
    return mIsLoaded;
}

/* Returns whether a show/hide animation is currently active.
   (C-linkage: func_8022C8D8) */
u8 CSelShopWin::func_8022C8D8() {
    return mAnimActive;
}

void func_8022CA20(){}

extern "C" void func_8022CA6C(void* self) {
    CSelShopWin* window = static_cast<CSelShopWin*>(self);
    if (window->mLayout != 0) {
        window->mIsLoaded = 1;
        window->mIsLayoutBuilt = 1;
    }
}

bool CSelShopWin::OnFileEvent(CEventFile*) { return true; }
