// Auto-scaffolded catalog TU for kyoshin/menu/CMenuSave
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/harness_catalog.hpp"
#include "kyoshin/menu/CMenuSave.hpp"

extern "C" void __ct__CMenuSave() {}

extern "C" void __dt__9CMenuSaveFv(CMenuSave* self, int dtorFlag) {}

extern "C" void Init__9CMenuSaveFv() {}

extern "C" void Term__9CMenuSaveFv() {}

extern "C" void Move__9CMenuSaveFv() {}

extern "C" void cbRenderBefore__9CMenuSaveFv(CMenuSave* self) {}

extern "C" void func_8028E3B4() {}

extern "C" void stub_us_802908bc() {}

extern "C" void func_8028E450() {}

extern "C" int func_8028F664(void*);

extern "C" int isIdle__11CTitleAHelpFv(void*);

extern "C" void func_8028E4E0(void* this_) {
    if (isIdle__11CTitleAHelpFv((void*)((char*)this_ + 0x80)) != 0 &&
        func_8028F664((void*)((char*)this_ + 0xb8)) != 0) {
        *(unsigned char*)((char*)this_ + 0x20b) = 2;
    }
}

extern "C" void func_8028E530() {}

extern "C" void func_8028E768() {}

/// Adjusting thunk: called when IScnRender subobject (at CMenuSave+0x58)
/// receives cbRenderBefore. Adjusts this back to full CMenuSave*.
extern "C" void func_8028E7B8(CMenuSave* self) {
    ((void(*)(CMenuSave*))cbRenderBefore__9CMenuSaveFv)(
        reinterpret_cast<CMenuSave*>(reinterpret_cast<char*>(self) - 0x58));
}

/// Adjusting destructor thunk: called when IScnRender subobject (at CMenuSave+0x58)
/// receives ~CMenuSave. The delete flag is passed through unchanged.
extern "C" void func_8028E7C0(CMenuSave* self, int dtorFlag) {
    ((void(*)(CMenuSave*, int))__dt__9CMenuSaveFv)(
        reinterpret_cast<CMenuSave*>(reinterpret_cast<char*>(self) - 0x58),
        dtorFlag);
}
