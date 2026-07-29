// Auto-scaffolded catalog TU for kyoshin/CPresentWin
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/harness_catalog.hpp"
#include "kyoshin/CPresentWin.hpp"

// Forward declaration for the data-array accessor used by func_8022E4FC / func_8022E504
u8 func_8022E868(void* self, u32 index);

u8 func_8022DB6C(void* self) { return static_cast<CPresentWin*>(self)->mField30; }







u8 func_8022DB74(void* self) { return static_cast<CPresentWin*>(self)->mField38; }



void func_8022DD90(){}

void func_8022E204(){}

void func_8022E254(){}

void func_8022E2F8(){}

void func_8022E3A8() {
    func_8022E254();
}

void func_8022E3AC(){}

u8 func_8022E488(void* self) { return static_cast<CPresentWin*>(self)->mField31; }

u8 func_8022E490(void* self) { return static_cast<CPresentWin*>(self)->mField36; }

void func_8022E498(){}

u8 func_8022E4FC(void* self) {
    return func_8022E868(self, static_cast<CPresentWin*>(self)->mField32);
}

u8 func_8022E504(void* self) {
    return func_8022E868(self, static_cast<CPresentWin*>(self)->mField33);
}

void func_8022E50C(){}

void func_8022E558(){}

void func_8022E5B0(){}

void func_8022E698(){}

void func_8022E744(){}

void func_8022E7F0(){}

u8 func_8022E868(void* self, u32 r4) {
    CPresentWin* p = static_cast<CPresentWin*>(self);
    u8 limit = p->mDataCount;
    if (r4 >= limit) return 0;
    return p->mDataArray[r4];
}
