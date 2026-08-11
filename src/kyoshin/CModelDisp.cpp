// Auto-scaffolded catalog TU for kyoshin/CModelDisp
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/harness_catalog.hpp"
#include "kyoshin/CModelDisp.hpp"

// Forward declarations for cross-TU calls
void* func_8004B9B8(void* self);
void func_8004B9D4(void* self, int a2, int a3, int a4, int a5);

// Sub-object ctor/dtor stubs (retail func_801FBEB8 / __dt__801FBF0C). Their
// bodies are separate match targets; they exist here only so the constructor's
// __construct_array call carries the exact retail reloc names.
void func_801FBEB8(CModelDispSub* sub) {
    (void)sub;
}

u8* __dt__801FBF0C(CModelDispSub* obj, int flag) {
    (void)obj;
    (void)flag;
    return (u8*)obj;
}

// Static helper inlined by -ipa file: its inner loops carry PARAMETER bounds,
// so MWCC keeps them as mtctr/bdnz countdown loops even after the call-site
// constants 2/8 are folded (direct constant-bound loops would fully unroll).
static void ClearSubWords(CModelDispSub* sub, int c1, int c2) {
    u8 j;
    for (j = 0; j < c1; j++) {
        (&sub->mFlagFD0)[j] = 0;
    }
    for (j = 0; j < c2; j++) {
        // Retail zeroes 8 words at 0xFD8, spilling 8 bytes past the 0xFF0
        // stride (the tail member is only 0x18 bytes) - reproduced verbatim.
        ((u32*)sub->_FD8)[j] = 0;
    }
}

CModelDisp::CModelDisp(u8* initParam) {
    mVtbl = lbl_eu_80535390;
    mInitParam = initParam;
    __construct_array(&mSubs[0], func_801FBEB8, __dt__801FBF0C, 0xff0, 3);

    field_2FD8 = 0;
    field_2FDC = lbl_eu_806681E8;
    field_2FE0 = lbl_eu_806681EC;
    field_2FE4 = 1;

    for (u8 i = 0; i < 3; i++) {
        CModelDispSub* sub = &mSubs[i];
        sub->field_00 = NULL;
        sub->field_04 = NULL;
        sub->mpController = NULL;
        sub->field_554 = NULL;
        sub->field_550 = NULL;

        // Pointer-walk + counter form keeps these as mtctr/bdnz loops
        // (direct subscripting would be auto-unrolled by -O4,p).
        u32* p0 = &sub->mFlagFD0;
        u8 j;
        for (j = 0; j < 2; p0++, j++) {
            *p0 = 0;
        }
        u32* p1 = (u32*)sub->_FD8;
        for (j = 0; j < 8; p1++, j++) {
            // Retail zeroes 8 words at 0xFD8, spilling 8 bytes past the 0xFF0
            // stride (the tail member is only 0x18 bytes) - reproduced verbatim.
            *p1 = 0;
        }
    }
}

u8 func_801FC114(void* self) { return ((CModelDisp*)self)->field_2FE4; }

// retail: if (field_2FD8 == 2) { field_2FD8 = 3; field_2FE4 = 0; }
extern "C" void func_801FC13C(CModelDisp* self) {
    if (self->field_2FD8 == 2) {
        self->field_2FD8 = 3;
        self->field_2FE4 = 0;
    }
}

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
