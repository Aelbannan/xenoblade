// Auto-scaffolded catalog TU for kyoshin/CTitle
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/harness_catalog.hpp"
#include "kyoshin/CTitle.hpp"

// TU-local plain functions (defined below in address order) called by the
// state-machine helpers in this file. noinline keeps real bl branches
// (retail calls them, so the call must survive).
void __declspec(noinline) func_802B6724(CTitleLogo* self);
void __declspec(noinline) func_802B6854(CTitleLogo* self);
void __declspec(noinline) func_802B6F64(CTitleMenu* self);
void __declspec(noinline) func_802B7094(CTitleMenu* self);
void __declspec(noinline) func_802B725C(CTitleMenu* self);

void __ct__CTitleLogo(){}

CTitleLogo::~CTitleLogo() {}

void func_802B63A4(){}

void func_802B6434(){}

void func_802B64AC(){}

void func_802B64DC(){}

void func_802B6534(CTitleLogo* self) {
    if (self->field_0x1A == 0) {
        self->field_0x1A = 1;
        func_802B6724(self);
        self->field_0x18 = 1;
        self->field_0x19 = 0;
    }
}

void func_802B6580(CTitleLogo* self) {
    if (self->field_0x1A == 2) {
        self->field_0x1A = 3;
        func_802B6854(self);
        self->field_0x19 = 0;
    }
}

void func_802B65C8(){}

void func_802B6660() {}

void func_802B6664(){}

extern float lbl_eu_80668FE0;
extern "C" void func_80137444__FPQ34nw4r3lyt13AnimTransformf(void* a, float f);
extern "C" void func_802B66B8(void* self) { func_80137444__FPQ34nw4r3lyt13AnimTransformf(*(void**)((u8*)self + 0x10), lbl_eu_80668FE0); }

void func_802B66C4(){}

void __declspec(noinline) func_802B6724(CTitleLogo* self){}

void func_802B67BC(){}

void __declspec(noinline) func_802B6854(CTitleLogo* self){}

void __ct__CTitleMenu(){}

CTitleMenu::~CTitleMenu() {}

void func_802B6970(){}

void func_802B6A90(){}

void func_802B6B08(){}

void func_802B6B38(){}

void func_802B6B90(CTitleMenu* self) {
    if (self->field_0x26 == 0) {
        self->field_0x26 = 1;
        func_802B6F64(self);
        self->field_0x24 = 1;
        self->field_0x25 = 0;
    }
}

void func_802B6BDC(CTitleMenu* self) {
    if (self->field_0x26 == 2) {
        self->field_0x26 = 3;
        func_802B7094(self);
        self->field_0x25 = 0;
    }
}

void func_802B6C24(){}

void func_802B6C74(CTitleMenu* self) {
    if (self->field_0x26 == 5) {
        self->field_0x26 = 6;
        func_802B725C(self);
        self->field_0x25 = 0;
    }
}

void func_802B6CBC(){}

void func_802B6D5C(){}

void func_802B6DF4() {}

void func_802B6DF8(){}

extern "C" void func_802B6E4C(void* self) { func_80137444__FPQ34nw4r3lyt13AnimTransformf(*(void**)((u8*)self + 0x10), lbl_eu_80668FE0); }

void func_802B6E58(){}

void func_802B6EA4(){}

extern "C" void func_802B6EF8(void* self) { func_80137444__FPQ34nw4r3lyt13AnimTransformf(*(void**)((u8*)self + 0x1C), lbl_eu_80668FE0); }

void func_802B6F04(){}

void __declspec(noinline) func_802B6F64(CTitleMenu* self){}

void func_802B6FFC(){}

void __declspec(noinline) func_802B7094(CTitleMenu* self){}

void func_802B712C(){}

void func_802B71C4(){}

void __declspec(noinline) func_802B725C(CTitleMenu* self){}

void __ct__CTitle(){}

CTitle::~CTitle() {}

void func_802B73D4(){}

void func_802B744C(){}

void func_802B74A8(){}

void func_802B74F4(){}

void func_802B7564(){}

void func_802B7590(){}

void func_802B75B8(){}

void func_802B75D8(){}

void func_802B7630(){}

void func_802B7650(){}

void func_802B76D4(){}

void func_802B775C(){}

void func_802B7800(){}

void func_802B7838() {}

void func_802B783C(u8* thisPtr) {
    if (thisPtr[0x6d] == 0) {
        return;
    }
    thisPtr[0x24] = 2;
}

void func_802B7854() {}

void func_802B7858(){}

void func_802B78A4(){}

void func_802B7920() {}

void func_802B7924(){}

extern "C" void func_802B7948(void* a, unsigned int b, unsigned char v) {
    extern void* lbl_eu_80664C38;
    void* p = lbl_eu_80664C38;
    if (p) *(unsigned char*)((u8*)p + 0x25) = v;
}

void CTitle::OnFileEvent() {}

void cf::CHelp_ToAttack::func_802B7A90() {}
