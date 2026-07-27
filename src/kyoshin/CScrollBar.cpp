// Auto-scaffolded catalog TU for kyoshin/CScrollBar
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/harness_catalog.hpp"
#include "kyoshin/CScrollBar.hpp"

extern "C" u8 CScrollBar_isVisible(CScrollBar* self) { return self->mVisible; }







extern "C" u8 func_801F3668(CScrollBar* self) { return self->mActive; }

void func_801F3670(void){}




void func_801F3850(){}

void func_801F38FC(){}

void func_801F3960(){}

extern "C" void func_801F39B4(void* self) {
    if (*(u32*)((u8*)self + 0x1C) != 0) {
        *(u8*)((u8*)self + 0x25) = 1;
        *(u8*)((u8*)self + 0x24) = 1;
    }
}

void CScrollBar::OnFileEvent() {}
