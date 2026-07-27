// Auto-scaffolded catalog TU for kyoshin/CScrollBar
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/harness_catalog.hpp"
#include "kyoshin/CScrollBar.hpp"

u8 CScrollBar_isVisible(CScrollBar* self) { return self->mVisible; }







u8 func_801F3668(CScrollBar* self) { return self->mActive; }

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
