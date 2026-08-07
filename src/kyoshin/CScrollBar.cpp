// Auto-scaffolded catalog TU for kyoshin/CScrollBar
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/harness_catalog.hpp"
#include "kyoshin/CScrollBar.hpp"

u8 CScrollBar::isVisible() { return mVisible; }


u8 CScrollBar::func_801F3668() { return mActive; }

void func_801F3670(void){}


void func_801F3850(){}

void func_801F38FC(){}

void func_801F3960(){}

extern "C" void func_801F39B4(void* self) {
    CScrollBar* bar = static_cast<CScrollBar*>(self);
    if (bar->mLayout != 0) {
        bar->mVisible = 1;
        bar->mReady = 1;
    }
}

bool CScrollBar::OnFileEvent(CEventFile*) { return true; }

extern "C" void func_801F34F4() {}
extern "C" void func_801F3540() {}
extern "C" void func_801F35B0() {}
extern "C" void func_801F35DC() {}
extern "C" void func_801F367C() {}
extern "C" void func_801F369C() {}
extern "C" void func_801F36BC() {}
