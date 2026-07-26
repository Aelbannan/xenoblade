// Auto-scaffolded catalog TU for kyoshin/CMapSel
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/harness_catalog.hpp"
#include "kyoshin/CMapSel.hpp"

extern "C" void __ct__CMapSel() {}

extern "C" void __dt__7CMapSelFv() {}

extern "C" void func_8024343C() {}

extern "C" void func_802434A0() {}

extern "C" void func_80243560() {}

extern "C" void func_802435CC() {}

extern "C" void func_80243680() {}

// FULL_MATCH: reads the initial-setup flag at +0x33 (set to 1 in ctor, cleared on play/close)
extern "C" u8 func_802436C4(CMapSel* self) { return self->mFlag33; }

extern "C" void func_802436CC() {}

extern "C" void func_8024371C() {}

extern "C" void func_80243768() {}

extern "C" void func_80243838() {}

extern "C" void func_8024391C() {}

extern "C" void func_802439CC() {}

extern "C" u8 func_80243A9C(void* self) {
    u8* s = (u8*)self;
    s8 x = *(s8*)(s + 0x8D);
    s8 y = *(s8*)(s + 0x8C);
    return *(u8*)(s + x + y + 0x8E);
}

extern "C" void func_80243ABC() {}

extern "C" void func_80243B88() {}

extern "C" void func_80243BE8() {}

extern "C" void func_80243C6C() {}

extern "C" void func_80243CB8() {}

extern "C" void func_80243CFC() {}

extern "C" void func_80243E08() {}

extern "C" void func_80243ED8() {}

extern "C" void func_80243FC4() {}

extern "C" void func_80244020() {}

extern "C" void OnFileEvent__7CMapSelFP10CEventFile() {}
