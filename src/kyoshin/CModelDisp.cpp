// Auto-scaffolded catalog TU for kyoshin/CModelDisp
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/harness_catalog.hpp"
#include "kyoshin/CModelDisp.hpp"

// Forward declarations for cross-TU calls
void* func_8004B9B8(void* self);
void func_8004B9D4(void* self, int a2, int a3, int a4, int a5);

u8 func_801FC114(void* self) { return ((CModelDisp*)self)->field_2FE4; }

// Advances field_2FE0 by 1.0 each call. When it reaches 5.0, decrements
// field_2FDC by 0.2 (clamped to 0.0) and calls each sub-object's vmethod.
void func_801FC15C(CModelDisp* self) {
    self->field_2FE0 += 1.0f;
    if (self->field_2FE0 >= 5.0f) {
        self->field_2FDC -= 0.2f;
        if (self->field_2FDC < 0.0f) {
            self->field_2FDC = 0.0f;
            self->field_2FD8 = 2;
            self->field_2FE4 = 1;
        }

        // Iterate over 3 sub-objects and call vmethod on controller pointer
        for (int i = 0; i < 3; i++) {
            CModelDispSub* sub = (CModelDispSub*)((u8*)self + i * 0xFF0);
            void* ctrl = sub->mpController;
            if (ctrl != NULL) {
                // vcall: vtable[0x48/4 = 18] - takes field_2FDC as float arg
                typedef void (*VMethod48)(void*, f32);
                VMethod48* vtbl = *(VMethod48**)ctrl;
                vtbl[18](ctrl, self->field_2FDC);
            }
        }
    }
}

void func_801FC218(){}

void func_801FC2B4(){}

void func_801FC3B0(){}

int func_801FCAC0(void* self) { return 0; }

void func_801FCAC8(){}

void func_801FCB4C(){}

int func_801FCBEC(void* self) { return 0; }

void func_801FCBF4(){}

// Scans sub-objects for one whose mpController matches param's field_0x3A0,
// then dispatches getNextChainObj / setParam calls for active slots.
void func_801FCDB4(CModelDisp* self, void* param, int r5) {
    u32 matchVal = *(u32*)((u8*)param + 0x3A0);

    for (int i = 0; i < 3; i++) {
        CModelDispSub* sub = (CModelDispSub*)((u8*)self + i * 0xFF0);
        if ((u32)sub->mpController != matchVal) {
            continue;
        }

        if (sub->mFlagFD0 != 0) {
            void* result = func_8004B9B8((u8*)sub + 0x14);
            sub->mResultA = result;
            func_8004B9D4(sub->mBuffer, r5, 0, -1, 0);
        }

        if (sub->mFlagFD4 != 0) {
            void* result2 = func_8004B9B8((u8*)sub + 0x14);
            sub->mResultB = result2;
            func_8004B9D4(sub->_A94, r5, 0, -1, 0);
        }

        break;
    }
}

extern "C" void func_801FC11C(void* self) {
    if (*(u8*)((u8*)self + 0x2FD8) != 0) return;
    *(u8*)((u8*)self + 0x2FD8) = 1;
    *(u8*)((u8*)self + 0x2FE4) = 0;
}
