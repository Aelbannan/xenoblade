// Auto-scaffolded catalog TU for kyoshin/CPresentWin
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/harness_catalog.hpp"
#include "kyoshin/CPresentWin.hpp"
#include "kyoshin/code_80135FDC.hpp"

extern void func_80138078(u32);

// Destructor — base object destructor (D2); MWCC emits D1 with delete check
CPresentWin::~CPresentWin() {
}

// Draw the present window layout if active and in the right state
void func_8022DAD8(CPresentWin* self, nw4r::lyt::DrawInfo* drawInfo) {
    if (self->mField30 == 0) return;
    if (self->_pad_37[0] == 0) return;
    func_80137038(self->mpLayout, drawInfo, 0, 1);
}

u8 func_8022DB6C(void* self) { return static_cast<CPresentWin*>(self)->mField30; }







u8 func_8022DB74(void* self) { return static_cast<CPresentWin*>(self)->mField38; }

// State-machine transition: if state == 2, advance to state 3 and clear sub-state
void func_8022DD68(CPresentWin* self) {
    if (self->_pad_37[0] != 2) return;
    self->_pad_37[0] = 3;
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

void func_8022E498(){}

u8 func_8022E4FC(void* self){ return static_cast<CPresentWin*>(self)->mField32; }

u8 func_8022E504(void* self){ return static_cast<CPresentWin*>(self)->mField33; }

void func_8022E50C(){}

void func_8022E558(){}

void func_8022E5B0(){}

void func_8022E698(){}

void func_8022E744(){}

void func_8022E7F0(CPresentWin* self) {
    self->mDataCount = 0;
    u8 count = code80135FDC_getByte_64077();
    for (u8 i = 0; i < count; i++) {
        u8 val = func_801392B4(i);
        if (val <= 8) {
            self->mDataArray[self->mDataCount] = val;
            self->mDataCount++;
        }
    }
}

extern "C" u8 func_8022E868(void* self, u32 r4) {
    CPresentWin* p = static_cast<CPresentWin*>(self);
    u8 limit = p->mDataCount;
    if (r4 >= limit) return 0;
    return p->mDataArray[r4];
}
