// TU for kyoshin/CItemBoxGridSubMenu
// Submenu overlay widget for the item grid screen.

#include "kyoshin/CItemBoxGridSubMenu.hpp"

extern "C" u8 func_80208358(CItemBoxGridSubMenu* self) {
    return self->mIsVisible;
}

extern "C" u8 func_80208360(CItemBoxGridSubMenu* self) {
    return self->mIsOpened;
}

// LLM-HARNESS-BEGIN: us-8020a1ac
extern "C" void func_802083CC(unsigned char* self) {
    if (self[0x22] != 2)
        return;

    switch (self[0x23]) {
    case 1:
        self[0x24] = 0;
        break;
    case 2:
        self[0x24] = (unsigned char)(self[0x24] - 1);
        if ((signed char)self[0x24] < 0)
            self[0x24] = 1;
        break;
    case 3:
        self[0x24] = (unsigned char)(self[0x24] - 1);
        if ((signed char)self[0x24] < 0)
            self[0x24] = 2;
        break;
    }
}
// LLM-HARNESS-END: us-8020a1ac

// LLM-HARNESS-BEGIN: us-8020a22c
extern "C" void func_8020844C() {}
// LLM-HARNESS-END: us-8020a22c

// LLM-HARNESS-BEGIN: us-8020a2b4
extern "C" void func_802084D4() {}
// LLM-HARNESS-END: us-8020a2b4

// LLM-HARNESS-BEGIN: us-8020a540
extern "C" void func_80208760() {}
// LLM-HARNESS-END: us-8020a540

// LLM-HARNESS-BEGIN: us-8020a598
extern "C" void func_802087B8() {}
// LLM-HARNESS-END: us-8020a598

// LLM-HARNESS-BEGIN: us-8020a618
extern "C" void func_80208838(void) {}
// LLM-HARNESS-END: us-8020a618

// LLM-HARNESS-BEGIN: us-8020a624
extern "C" void func_80208844() {}
// LLM-HARNESS-END: us-8020a624

// LLM-HARNESS-BEGIN: us-8020a670
extern "C" void func_80208890() {}
// LLM-HARNESS-END: us-8020a670

// LLM-HARNESS-BEGIN: us-8020a0b0
extern "C" void func_80137038__FPQ34nw4r3lyt6LayoutPQ34nw4r3lyt8DrawInfoii(void*, void*, int, int);
extern "C" void func_802082D0(void* self) {
    if (*(u8*)((u8*)self + 0x20) == 0) return;
    func_80137038__FPQ34nw4r3lyt6LayoutPQ34nw4r3lyt8DrawInfoii(*(void**)((u8*)self + 8), 0, 0, 1);
}
// LLM-HARNESS-END: us-8020a0b0
