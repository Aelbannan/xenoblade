// Auto-scaffolded catalog TU for kyoshin/menu/CMenuSave
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/harness_catalog.hpp"
#include "kyoshin/menu/CMenuSave.hpp"

void __ct__CMenuSave(){}

void CMenuSave::~CMenuSave(int dtorFlag) {}

void CMenuSave::Init() {}

void CMenuSave::Term() {}

void CMenuSave::Move() {}

void CMenuSave::cbRenderBefore() {}

void func_8028E3B4(){}

void stub_us_802908bc() {}

void func_8028E450(){}

int func_8028F664(void*);

int isIdle__11CTitleAHelpFv(void*);

void func_8028E4E0(void* this_) {
    if (isIdle__11CTitleAHelpFv((void*)((char*)this_ + 0x80)) != 0 &&
        func_8028F664((void*)((char*)this_ + 0xb8)) != 0) {
        *(unsigned char*)((char*)this_ + 0x20b) = 2;
    }
}

void func_8028E530(){}

void func_8028E768(){}

/// Adjusting thunk: called when IScnRender subobject (at CMenuSave+0x58)
/// receives cbRenderBefore. Adjusts this back to full CMenuSave*.
void func_8028E7B8(IScnRender* self) {
    ((void(*)(CMenuSave*))cbRenderBefore__9CMenuSaveFv)(
        static_cast<CMenuSave*>(self));
}

/// Adjusting destructor thunk: called when IScnRender subobject (at CMenuSave+0x58)
/// receives ~CMenuSave. The delete flag is passed through unchanged.
extern "C" void func_8028E7C0(IScnRender* self, int dtorFlag) {
    ((void(*)(CMenuSave*, int))__dt__9CMenuSaveFv)(
        static_cast<CMenuSave*>(self),
        dtorFlag);
}
