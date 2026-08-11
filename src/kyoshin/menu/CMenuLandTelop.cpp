// Auto-scaffolded catalog TU for kyoshin/menu/CMenuLandTelop
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/harness_catalog.hpp"

#include "kyoshin/menu/CMenuLandTelop.hpp"

#include <new>

void __ct__CMenuLandTelop(){}

// Destructor (retail __dt__14CMenuLandTelopFv). Destroy the embedded
// UnkClass_8045F564 region, then the CProcess base (guarded by the nested
// double null-check, an MWCC D2-inlined-into-D1 artifact), then conditional
// operator delete when flags > 0. CSysWinSave idiom: the hpp member dtor
// declaration is satisfied at link time by this global definition.
CMenuLandTelop* __dt__14CMenuLandTelopFv(CMenuLandTelop* _this, int flags) {
    if (_this != 0) {
        __dt__17UnkClass_8045F564Fv(&_this->mMemRegion, -1);
        if (_this != 0) {
            if (_this != 0) {
                __dt__8CProcessFv(_this, 0);
            }
        }
        if (flags > 0) {
            operator delete(_this);
        }
    }
    return _this;
}

void CMenuLandTelop::Init() {
    func_8014548C();
    field_E0 = func_80226B94();
    field_67 = 0;
    IScnRender* cb = reinterpret_cast<IScnRender*>(this);
    if (this != 0) {
        cb = reinterpret_cast<IScnRender*>(&mScnRender);
    }
    mScene->addRenderCB(cb, 0xa, 0);
}

void CMenuLandTelop::Term() {}

void CMenuLandTelop::Move() {}

void CMenuLandTelop::cbRenderBefore() {}

void func_80144EE4(){}

// Quest-log gating helper: true while the land-telop state flag at +0x8E
// of the global object is 1.
int func_80144FC8() {
    CLandTelopGlobal* p = lbl_eu_806641A0;
    if (p != 0 && p->field_0x8E == 1) {
        return 1;
    }
    return 0;
}

// Quest-log gating helper: true while the land-telop state flag at +0x8E
// of the global object is 0.
int func_80144FF0() {
    CLandTelopGlobal* p = lbl_eu_806641A0;
    if (p != 0 && p->field_0x8E == 0) {
        return 1;
    }
    return 0;
}

void func_80145018() {
    CLandTelopGlobal* p = lbl_eu_806641A0;
    if (p != 0) {
        p->field_0x64 = 1;
    }
}

unsigned char func_80145030(void) {
    if (lbl_eu_806641A0 != 0) {
        return lbl_eu_806641A0->field_0xE0;
    }
    return 0;
}

void func_8014504C(){}

void func_801453B8(){}

// Empty stub: guard so MWCC -inline auto does not fold the call in Init
// away (MWCC_REFERENCE sec 834: empty same-TU stubs inline to nothing).
#pragma push
#pragma auto_inline off
void func_8014548C(){}
#pragma pop

void func_80145A90(void* self) { ((void(*)(void*))__dt__14CMenuLandTelopFv)((char*)self - 0x6c); }

void func_80145A98(void* self) { ((void(*)(void*))cbRenderBefore__14CMenuLandTelopFv)((char*)self - 0x70); }

void func_80145AA0(void* self) { ((void(*)(void*))__dt__14CMenuLandTelopFv)((char*)self - 0x70); }

// Resolve the bdat string for the given row: read the column named by the
// string at lbl_eu_805018A8[9]; when it is missing or identical to the
// fallback name at [0xE], return the fallback name.
const char* func_80145AA8(int index) {
    const char* s = getBdatStringColumnValue(lbl_eu_806640E0, &lbl_eu_805018A8[9], index);
    if (s == 0) {
        return &lbl_eu_805018A8[0xe];
    }
    if (std::strcmp(s, &lbl_eu_805018A8[0xe]) == 0) {
        return &lbl_eu_805018A8[0xe];
    }
    return s;
}
