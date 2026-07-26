// Auto-scaffolded catalog TU for kyoshin/cf/CtrlPc
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/harness_catalog.hpp"

extern "C" void __ct__cf_CtrlPad() {}

extern "C" void __dt__Q22cf10CtrlRemoteFv() {}

extern "C" void func_80096974() {}

extern "C" void func_80096EE8() {}

extern "C" void func_80096EF4() {}

extern "C" void func_80096F00() {}

extern "C" u32 func_80096F0C(void* self) { return (*(u32*)((u8*)self + 44) >> 20) & 0x1u; }

extern "C" u32 func_80096F18(void* self) { return (*(u32*)((u8*)self + 44) >> 19) & 0x1u; }

extern "C" u32 func_80096F24(void* self) { return (*(u32*)((u8*)self + 44) >> 18) & 0x1u; }

extern "C" u32 func_80096F30(void* self) { return (*(u32*)((u8*)self + 44) >> 17) & 0x1u; }

extern "C" u32 func_80096F3C(void* self) { return (*(u32*)((u8*)self + 44) >> 16) & 0x1u; }

extern "C" u32 func_80096F48(void* self) { return (*(u32*)((u8*)self + 44) >> 15) & 0x1u; }

extern "C" u32 func_80096F54(void* self) { return (*(u32*)((u8*)self + 44) >> 14) & 0x1u; }

extern "C" u32 func_80096F60(void* self) { return (*(u32*)((u8*)self + 44) >> 13) & 0x1u; }

extern "C" u32 func_80096F6C(void* self) { return (*(u32*)((u8*)self + 44) >> 12) & 0x1u; }

extern "C" u32 func_80096F78(void* self) { return (*(u32*)((u8*)self + 44) >> 11) & 0x1u; }

extern "C" u32 func_80096F84(void* self) { return (*(u32*)((u8*)self + 44) >> 10) & 0x1u; }

extern "C" u32 func_80096F90(void* self) { return (*(u32*)((u8*)self + 44) >> 9) & 0x1u; }

extern "C" u32 func_80096F9C(void* self) { return (*(u32*)((u8*)self + 44) >> 8) & 0x1u; }

extern "C" void __dt__Q22cf7CtrlPadFv() {}

extern "C" void __ct__cf_CtrlPc() {}

extern "C" void __dt__Q22cf6CtrlPcFv() {}

extern "C" void func_80097134() {}

extern "C" void func_80097598() {}

extern "C" void func_80097A5C() {}

extern "C" void func_80097C74() {}

extern "C" void func_80097DFC() {}

extern "C" void func_80097E00() {}

extern "C" void func_80098194() {}

extern "C" void* CActorParam_UnkVirtualFunc122__Q22cf11CActorParamFv(void* self) { return (void*)((u8*)self + 0x19e8); }

extern "C" void func_800983B8() {}

extern "C" void func_8009868C(void* self, u32 val) { *(u32*)((u8*)self + 0x380) = val; }

extern "C" void func_80098694() {}

extern "C" void func_800987A0() {}

extern "C" void func_800987EC(void* self) {
    extern void func_800899C0(void*, float);
    extern float lbl_eu_80666728;
    func_800899C0((char*)self + 0x8c, lbl_eu_80666728);
}

extern "C" void func_80089B24(void* self);
extern "C" void func_800987F8(void* self) { ((void(*)(void*))func_80089B24)((char*)self + 0x8c); }

extern "C" void func_80089E88(void* self);
extern "C" void func_80098800(void* self) { ((void(*)(void*))func_80089E88)((char*)self + 0x8c); }

extern "C" void func_80089F68(void* self);
extern "C" void func_80098808(void* self) { ((void(*)(void*))func_80089F68)((char*)self + 0x8c); }

extern "C" void func_80098810() {}

extern "C" void CAttackParam_UnkVirtualFunc3__Q22cf12CAttackParamFv(void* self, u8 val) { ((u8*)self)[0x2A] = val; }

extern "C" u8 CAttackParam_UnkVirtualFunc2__Q22cf12CAttackParamFv(void* self) { return ((u8*)self)[0x2A]; }

extern "C" void func_80098A04() {}

extern "C" void func_80098AF0() {}

extern "C" u32 func_80098B64(void* self) { return *(u32*)((u8*)self + 0x1D4); }

extern "C" int func_80098B6C(void* self) { return 1; }

extern "C" void func_80098B74() {}

extern "C" int lbl_eu_80527E98[];

extern "C" void func_80098BD0(int idx, int val)
{
    lbl_eu_80527E98[idx] = val;
}
