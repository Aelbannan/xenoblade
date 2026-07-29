// Auto-scaffolded catalog TU for kyoshin/CPresentWin
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/harness_catalog.hpp"
#include "kyoshin/CPresentWin.hpp"
#include "kyoshin/code_80135FDC.hpp"
#include "monolib/device/CDeviceVI.hpp"

extern void func_80138078(u32);
extern void func_801375A0(float*, void*);

// Destructor - base object destructor (D2); MWCC emits D1 with delete check
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

void func_8022E204(){}

void func_8022E254(){}

void func_8022E2F8(){}

void func_8022E3A8(void){}

void func_8022E3AC(){}

u8 func_8022E488(void* self) { return static_cast<CPresentWin*>(self)->mField31; }

u8 func_8022E490(void* self) { return static_cast<CPresentWin*>(self)->mField36; }

// Constructor - zero-initializes all fields except mField38 which starts at 1
CPresentWin::CPresentWin() {
    mpField04 = NULL;
    mpLayout = NULL;
    mpField0C = NULL;
    mpField10 = NULL;
    mpField14 = NULL;
    mpField18 = NULL;
    mpField1C = NULL;
    mpField20 = NULL;
    mpField24 = NULL;
    mpField28 = NULL;
    mpField2C = NULL;
    mField30 = 0;
    mField31 = 0;
    mField32 = 0;
    mField33 = 0;
    mField34 = 0;
    mField36 = 0;
    mField37 = 0;
    mField38 = 1;
    mDataCount = 0;
}

// Cleanup: wait for draw, delete the layout, and clear fields
void func_8022DB04(CPresentWin* self) {
    CDeviceVI::waitForDrawDone();
    self->mField30 = 0;
    if (self->mpLayout != NULL) {
        delete self->mpLayout;
        self->mpLayout = NULL;
    }
    self->mpField04 = NULL;
}

// Multiply *val by a scale factor from the object at mpField14, after passing
// one of two layout/transform pointers to func_801375A0 based on mField31.
void func_8022E498(float* val, CPresentWin* self) {
    u8* ptr;
    if (self->mField31 != 0) {
        ptr = self->mpField1C;
    } else {
        ptr = self->mpField18;
    }
    func_801375A0(val, ptr);
    *val = *val * self->mpField14->mScale;
}

u8 func_8022E4FC(void* self){ return static_cast<CPresentWin*>(self)->mField32; }

u8 func_8022E504(void* self){ return static_cast<CPresentWin*>(self)->mField33; }

void func_8022E50C(){}

void func_8022E558(){}

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
