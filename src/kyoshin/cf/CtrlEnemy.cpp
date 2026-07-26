// Auto-scaffolded catalog TU for kyoshin/cf/CtrlEnemy
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/harness_catalog.hpp"

extern "C" void __ct__cf_CtrlEnemy() {}

extern "C" void __dt__Q22cf7CtrlActFv() {}

extern "C" void* __dt__Q22cf8CtrlBaseFv(void* self, int deleteFlag) {
    if (self != nullptr) {
        if (deleteFlag > 0) {
            operator delete(self);
        }
    }
    return self;
}

extern "C" void __dt__80087688() {}

extern "C" void __dt__Q22cf9CtrlEnemyFv() {}

extern "C" void func_8008772C() {}

extern "C" void func_800877A8() {}

extern "C" float CfObject_UnkVirtualFunc60__Q22cf12CfObjectMoveFv(void* self) { return *(float*)((u8*)self + 0x6ec); }

extern "C" void func_80087EEC() {}

extern "C" void* CActorParam_UnkVirtualFunc113__Q22cf11CActorParamFv(void* self) { return (void*)((u8*)self + 0x161c); }

extern "C" bool func_800885F0(void* obj, unsigned int mask)
{
    return (*(unsigned int*)((char*)obj + 0x84) & mask) != 0;
}

extern "C" void* CActorParam_UnkVirtualFunc117__Q22cf11CActorParamFv(void* self) { return (void*)((u8*)self + 0x1620); }

extern "C" void* CActorParam_UnkVirtualFunc119__Q22cf11CActorParamFv(void* self) { return (void*)((u8*)self + 0x1624); }

extern "C" u32 CActorParam_UnkVirtualFunc29__Q22cf11CActorParamFv(void* self) { return *(u32*)((u8*)self + 0x17E4); }

extern "C" void func_80088620() {}

extern "C" unsigned long CActorParam_UnkVirtualFunc115__Q22cf11CActorParamFv(void* self) {
    return !!((unsigned char*)self)[0x1628];
}

extern "C" u32 CActorParam_UnkVirtualFunc19__Q22cf11CActorParamFv(void* self) { return *(u32*)((u8*)self + 0x15EC); }

extern "C" void func_80088800() {}

extern "C" void func_8008884C() {}

extern "C" void func_80089B24(void* self);
extern "C" void func_80088858(void* self) { ((void(*)(void*))func_80089B24)((char*)self + 0x84); }

extern "C" void func_80089E88(void* self);
extern "C" void func_80088860(void* self) { ((void(*)(void*))func_80089E88)((char*)self + 0x84); }

extern "C" void func_80089F68(void* self);
extern "C" void func_80088868(void* self) { ((void(*)(void*))func_80089F68)((char*)self + 0x84); }

extern "C" int func_80088870(void* self) { return 0; }

extern "C" int func_80088878(void* self) { return 0; }

extern "C" int func_80088880(void* self) { return 0; }

extern "C" int func_80088888(void* self) { return 0; }

extern "C" int func_80088890(void* self) { return 0; }

extern "C" int func_80088898(void* self) { return 0; }

extern "C" int func_800888A0(void* self) { return 0; }

extern "C" int func_800888A8(void* self) { return 0; }

extern "C" int func_800888B0(void* self) { return 0; }

extern "C" int func_800888B8(void* self) { return 0; }

extern "C" int func_800888C0(void* self) { return 0; }

extern "C" int func_800888C8(void* self) { return 0; }

extern "C" int func_800888D0(void* self) { return 0; }

extern "C" int func_800888D8(void* self) { return 0; }

extern "C" int func_800888E0(void* self) { return 0; }

extern "C" int func_800888E8(void* self) { return 0; }

extern "C" void func_800888F0() {}

extern "C" int func_800888F4(void* self) { return 1; }

extern "C" int func_800888FC(void* self) { return 1; }
