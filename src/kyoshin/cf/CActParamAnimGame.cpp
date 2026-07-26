// Auto-scaffolded catalog TU for kyoshin/cf/CActParamAnimGame
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/cf/CActParamAnimGame.hpp"

extern "C" void __ct__8005A3FC() {}

extern "C" void __ct__Q22cf17CActParamAnimGameFv() {}

extern "C" void __dt__Q22cf17CActParamAnimGameFv() {}

extern "C" void func_8005A524__Q22cf17CActParamAnimGame() {}

extern "C" void func_8005A594() {}

extern "C" void func_8005A5B0() {}

extern "C" void func_8005B820() {}

extern "C" void func_8005BC14() {}

extern "C" void func_8005D2C4__Q22cf17CActParamAnimGame() {}

extern "C" void func_8005D608__Q22cf17CActParamAnimGame() {}

extern "C" void func_8005D67C__Q22cf17CActParamAnimGame() {}

extern "C" void func_8005D6C0__Q22cf17CActParamAnimGame() {}

extern "C" bool func_8004ECF4__13CActParamAnimFv(void* self);
extern "C" bool func_8005D70C__Q22cf17CActParamAnimGame(void* self) {
    if (*(unsigned int*)((unsigned char*)self + 0x4ec) & 2) {
        return false;
    }
    return func_8004ECF4__13CActParamAnimFv(self);
}

extern "C" void func_8005D728__Q22cf17CActParamAnimGame() {}

extern "C" void func_8005D76C__Q22cf17CActParamAnimGame() {}

extern "C" void func_8005D84C__Q22cf17CActParamAnimGame() {}

extern "C" void func_8005D99C__Q22cf17CActParamAnimGame() {}

extern "C" void func_8005DA44__Q22cf17CActParamAnimGame() {}

extern "C" void func_8004BC94(void*);
extern "C" bool func_8005DAE4__Q22cf17CActParamAnimGame(void* self) {
    if (*(unsigned int*)((unsigned char*)self + 0x4ec) & 0x00040000) {
        func_8004BC94(self);
        return true;
    }
    return false;
}

extern "C" void func_8005DB1C__Q22cf17CActParamAnimGame() {}

extern "C" bool CActorParam_UnkVirtualFunc38__Q22cf11CActorParamFv() { return false; }

extern "C" void func_8005DC30__Q22cf17CActParamAnimGame() {}

// cf::CActParamAnimGame::func_8005DCA0 — trivial leaf, returns false
// Retail symbol lacks Fv suffix; extern "C" preserves exact mangling for FULL_MATCH
extern "C" bool func_8005DCA0__Q22cf17CActParamAnimGame() { return false; }

extern "C" void func_8005DCA8__Q22cf17CActParamAnimGame() {}

extern "C" void func_8005DE68() {}

extern "C" void func_8005E28C() {}

extern "C" void func_8005E60C() {}

extern "C" void func_8005E7C4() {}

extern "C" void func_8005E990() {}

extern "C" void func_8005EEB4__Q22cf17CActParamAnimGame() {}

extern "C" void func_80060110__Q22cf17CActParamAnimGame() {}

extern "C" void func_80060268__Q22cf17CActParamAnimGame() {}

extern "C" bool func_80060290(void* r3) {
    int r0 = *(int*)((char*)r3 + 0x270);
    int r4 = 0;
    if ((r0 & 0x800000) != 0) {
        r4 = 1;
    } else {
        r0 = *(int*)((char*)r3 + 0x2f0);
        if ((r0 & 0x800000) != 0) {
            r4 = 1;
        }
    }
    return r4 != 0;
}
