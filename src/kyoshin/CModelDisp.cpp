// Auto-scaffolded catalog TU for kyoshin/CModelDisp
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/harness_catalog.hpp"
#include "kyoshin/CModelDisp.hpp"

// Forward declarations for cross-TU calls
void* func_8004B9B8(void* self);
void func_8004B9D4(void* self, int a2, int a3, int a4, int a5);
int func_800BBC04(int arg);

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
        for (u8 i = 0; i < 3; i++) {
            CModelDispSub* sub = (CModelDispSub*)((u8*)self + i * 0xFF0);
            void* ctrl = sub->mpController;
            if (ctrl != NULL) {
                // vcall: vtable[0x48/4 = 18] — takes field_2FDC as float arg
                typedef void (*VMethod48)(void*, f32);
                (*(VMethod48**)ctrl)[18](ctrl, self->field_2FDC);
            }
        }
    }
}

void func_801FC218(){}

void func_801FC2B4(){}

void func_801FC3B0(){}

int func_801FCAC0(void* self) { return 0; }

void func_801FCAC8(){}

// Dispatches vtable calls on a sub-object controller when flags are set
// and func_800BBC04 returns positive for the given action.
void func_801FCB4C(CModelDisp* self, int flags, int subIdx, int action, int ptrIdx) {
    if (flags == 0) return;
    if (func_800BBC04(action) <= 0) return;

    CModelDispSub* sub = (CModelDispSub*)((u8*)self + subIdx * 0xFF0);
    // ptrIdx selects between the two adjacent flag u32s at 0xFD0/0xFD4
    u32 flag = (&sub->mFlagFD0)[ptrIdx];
    if (flag == 0) return;

    void* ctrl = sub->mpController;

    // vcall: vtable[50] — takes controller only
    typedef void (*VMethod50)(void*);
    VMethod50* vtbl50 = *(VMethod50**)ctrl;
    vtbl50[50](ctrl);

    // vcall: vtable[49] — takes controller, flag, action, 0
    typedef void (*VMethod49)(void*, u32, int, int);
    VMethod49* vtbl49 = *(VMethod49**)ctrl;
    vtbl49[49](ctrl, flag, action, 0);
}

int func_801FCBEC(void* self) { return 0; }

void func_801FCBF4(){}

// Scans sub-objects for one whose mpController matches param's field_0x3A0,
// then dispatches getNextChainObj / setParam calls for active slots.
// Scans sub-objects for one whose mpController matches param's field_0x3A0,
// then dispatches getNextChainObj / setParam calls for active flag slots.
void func_801FCDB4(CModelDisp* self, CModelDispParam* param, int arg5) {
    u32 matchVal = param->field_0x3A0;

    for (u8 i = 0; i < 3; i++) {
        CModelDispSub* sub = (CModelDispSub*)((u8*)self + i * 0xFF0);
        if ((u32)sub->mpController == matchVal) {
            if (sub->mFlagFD0 != 0) {
                sub->mResultA = func_8004B9B8(sub->mSubObj);
                func_8004B9D4(sub->mBuffer, arg5, 0, -1, 0);
            }
            if (sub->mFlagFD4 != 0) {
                sub->mResultB = func_8004B9B8(sub->mSubObj);
                func_8004B9D4(sub->mBuffer2, arg5, 0, -1, 0);
            }
            break;
        }
    }
}

extern "C" void func_801FC11C(void* self) {
    if (*(u8*)((u8*)self + 0x2FD8) != 0) return;
    *(u8*)((u8*)self + 0x2FD8) = 1;
    *(u8*)((u8*)self + 0x2FE4) = 0;
}
