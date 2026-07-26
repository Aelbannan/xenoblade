// Auto-scaffolded catalog TU for kyoshin/cf/CtrlNpc
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/harness_catalog.hpp"

extern "C" void __ct__CtrlNpc() {}

extern "C" void func_8009377C() {}

extern "C" void CfObjectMove_UnkVirtualFunc6__Q22cf12CfObjectMoveFv(void* self, unsigned long val) {
    void* ptr = *(void**)((char*)self + 0xc4);
    if (ptr != 0)
        *(unsigned long*)((char*)ptr + 0x37c) = val;
}

extern "C" void func_80093938() {}

extern "C" void func_8009398C() {}

extern "C" void func_80093F28() {}

extern "C" void CObjectState_UnkVirtualFunc5__Q22cf12CObjectStateFv() {}

extern "C" void func_800948F8() {}

extern "C" void func_80094A9C() {}

extern "C" void func_80094CE8() {}

extern "C" void func_80094D1C() {}

extern "C" void func_80094DF4() {}

extern "C" void func_80094E44() {}

extern "C" void func_80094EDC() {}

extern "C" void func_80094FC8() {}

extern "C" float CfObject_UnkVirtualFunc73__Q22cf13CfObjectModelFv(void* self) { return *(float*)((u8*)self + 0xa4); }

extern "C" float CfObject_UnkVirtualFunc71__Q22cf13CfObjectModelFv(void* self) { return *(float*)((u8*)self + 0xa0); }

extern "C" void func_80095000() {}

extern "C" void func_8009505C() {}

extern "C" void func_800950E8() {}

extern "C" void func_8009519C() {}

extern "C" void func_800951A0() {}

extern "C" void func_80095224() {}

extern "C" void func_80095450() {}

extern "C" void func_8009563C(char* p) {
    short v = *(short*)(p + 0xBA) - 1;
    *(short*)(p + 0xBA) = v;
    if (v <= 0) {
        *(short*)(p + 0xBE) = 1;
    }
}

extern "C" void func_8009565C() {}

extern "C" void func_80095F44() {}

extern "C" void func_80096488() {}

extern "C" void func_800964EC() {}

extern "C" void func_800966E8() {}

extern "C" void func_800967F8() {}

extern "C" int func_8009684C(void* self) { return 1; }

extern "C" int func_80096854(void* self) { return 0; }

extern "C" void __dt__Q22cf7CtrlNpcFv() {}
