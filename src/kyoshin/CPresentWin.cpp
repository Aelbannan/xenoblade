// Auto-scaffolded catalog TU for kyoshin/CPresentWin
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/harness_catalog.hpp"
#include "kyoshin/CPresentWin.hpp"
#include "kyoshin/code_80135FDC.hpp"

extern void func_80138078(u32);
extern void* func_801394D4(u32);

// Destructor — base object destructor (D2); MWCC emits D1 with delete check
CPresentWin::~CPresentWin() {
}

// Draw the present window layout if active and in the right state
void func_8022DAD8(CPresentWin* self, nw4r::lyt::DrawInfo* drawInfo) {
    if (self->mField30 == 0) return;
    if (self->mField37 == 0) return;
    func_80137038(self->mpLayout, drawInfo, 0, 1);
}

u8 func_8022DB6C(void* self) { return static_cast<CPresentWin*>(self)->mField30; }







u8 func_8022DB74(void* self) { return static_cast<CPresentWin*>(self)->mField38; }

// State-machine transition: if state == 2, advance to state 3 and clear sub-state
void func_8022DD68(CPresentWin* self) {
    if (self->mField37 != 2) return;
    self->mField37 = 3;
    self->mField38 = 0;
    func_80138078(0xe);
}

void func_8022DD90(){}

// Store a u16 param, look up its string, and set it on a layout pane
void func_8022E204(CPresentWin* self, u16 param) {
    extern char lbl_eu_8050A84C[];
    self->mField34 = param;
    char* str = (char*)func_801394D4(param);
    func_80136B4C(self->mpLayout, &lbl_eu_8050A84C[0xb7], str, 0);
}

void func_8022E254(){}

void func_8022E2F8(){}

void func_8022E3A8(void){}

void func_8022E3AC(){}

u8 func_8022E488(void* self) { return static_cast<CPresentWin*>(self)->mField31; }

u8 func_8022E490(void* self) { return static_cast<CPresentWin*>(self)->mField36; }

void func_8022E498(){}

u8 func_8022E4FC(void* self){ return static_cast<CPresentWin*>(self)->mField32; }

u8 func_8022E504(void* self){ return static_cast<CPresentWin*>(self)->mField33; }

// Check if an animation has completed; if so, set state to 2 and mark active
void func_8022E50C(CPresentWin* self) {
    extern float lbl_eu_8066862C;
    float f = lbl_eu_8066862C;
    if (func_80137444(self->mpAnimTransform, f)) {
        self->mField37 = 2;
        self->mField38 = 1;
    }
}

// Check if an animation has finished; if so, reset state fields to idle
void func_8022E558(CPresentWin* self) {
    extern float lbl_eu_8066862C;
    float f = lbl_eu_8066862C;
    u32 r = func_80137510(self->mpAnimTransform, f);
    if (r) {
        self->mField37 = 0;
        self->mField38 = 1;
        self->mField31 = 0;
        self->mField36 = 0;
        self->mField30 = 0;
    }
}

void func_8022E5B0(){}

void func_8022E698(){}

void func_8022E744(){}

void func_8022E7F0(){}

extern "C" u8 func_8022E868(void* self, u32 r4) {
    CPresentWin* p = static_cast<CPresentWin*>(self);
    u8 limit = p->mDataCount;
    if (r4 >= limit) return 0;
    return p->mDataArray[r4];
}
