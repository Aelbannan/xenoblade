// Auto-scaffolded catalog TU for kyoshin/menu/CMenuPlayAward
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/harness_catalog.hpp"

extern "C" void __ct__CMenuPlayAward() {}

extern "C" void __dt__14CMenuPlayAwardFv(void* self) {}

extern "C" void Init__14CMenuPlayAwardFv() {}

extern "C" void Term__14CMenuPlayAwardFv() {}

extern "C" void Move__14CMenuPlayAwardFv() {}

extern "C" void cbRenderBefore__14CMenuPlayAwardFv(void* self) {}

extern "C" void func_80270308() {}

extern "C" void stub_us_80272800() {}

extern "C" void func_8027038C() {}

extern "C" void func_80270404() {}

extern "C" void func_80270454() {}

extern "C" void func_802705F4() {}

extern "C" void func_80270644(void* self) { ((void(*)(void*))cbRenderBefore__14CMenuPlayAwardFv)((char*)self - 0x58); }

extern "C" void func_8027064C(void* self) { ((void(*)(void*))__dt__14CMenuPlayAwardFv)((char*)self - 0x58); }

extern "C" void func_80270654() {}

extern "C" void func_802706C4(void* self) {
    *(unsigned long*)((char*)self + 0) = 0;
    *(unsigned long*)((char*)self + 4) = 0;
}

extern "C" void __dt__802706D4() {}

extern "C" void __dt__80270714() {}

extern "C" void func_80270770() {}

extern "C" void func_80270AD8() {}

extern "C" void* func_80270AEC(void* self, int param) {
    if (param >= 256) return 0;
    unsigned char byte = ((unsigned char*)self)[0x100a];
    return (void*)((unsigned char*)self + (byte << 11) + ((param & 0xFF) << 3));
}

extern "C" void __ct__CPlayAwardList() {}

extern "C" void __dt__14CPlayAwardListFv() {}

extern "C" void func_80270CEC() {}

extern "C" void func_80270D64() {}

extern "C" void func_80270E04() {}

extern "C" void func_80270E64() {}

extern "C" bool CScrollBar_isVisible(void*);

extern "C" unsigned char func_80270F28(void* this_) {
    if (CScrollBar_isVisible((void*)((char*)this_ + 0x48))) {
        return *((unsigned char*)this_ + 0x8a);
    }
    return 0;
}

extern "C" u8 func_80270F6C(void* self) { return ((u8*)self)[0x8b]; }

extern "C" void func_80270F74() {}

extern "C" void func_80271070() {}

extern "C" void func_802710D4() {}

extern "C" void func_80271190() {}

extern "C" void func_80271260() {}

extern "C" void func_80271300() {}

extern "C" void func_802713BC() {}

extern "C" void func_80271468() {}

extern "C" void func_80271480() {}

extern "C" void func_802714D4() {}

extern "C" void func_80271528() {}

extern "C" void func_80271574() {}

extern "C" void func_802715C0() {}

extern "C" void func_80271620() {}

extern "C" void func_80271680() {}

extern "C" void func_80271730() {}

extern "C" void func_802717F8() {}

extern "C" void func_802719F8() {}

extern "C" void OnFileEvent__14CPlayAwardListFP10CEventFile() {}
