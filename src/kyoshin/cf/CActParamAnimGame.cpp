// Auto-scaffolded catalog TU for kyoshin/cf/CActParamAnimGame
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/cf/CActParamAnimGame.hpp"

void __ct__8005A3FC(){}

cf::CActParamAnimGame::CActParamAnimGame() {}

cf::CActParamAnimGame::~CActParamAnimGame() {}

void cf::CActParamAnimGame::func_8005A524() {}

void func_8005A594(){}

void func_8005A5B0(){}

void func_8005B820(){}

void func_8005BC14(){}

void cf::CActParamAnimGame::func_8005D2C4() {}

void cf::CActParamAnimGame::func_8005D608() {}

void cf::CActParamAnimGame::func_8005D67C() {}

void cf::CActParamAnimGame::func_8005D6C0() {}

bool func_8004ECF4__13CActParamAnimFv(void* self);
bool func_8005D70C__Q22cf17CActParamAnimGame(void* self) {
    if (*(unsigned int*)((unsigned char*)self + 0x4ec) & 2) {
        return false;
    }
    return func_8004ECF4__13CActParamAnimFv(self);
}

void cf::CActParamAnimGame::func_8005D728() {}

void cf::CActParamAnimGame::func_8005D76C() {}

void cf::CActParamAnimGame::func_8005D84C() {}

void cf::CActParamAnimGame::func_8005D99C() {}

void cf::CActParamAnimGame::func_8005DA44() {}

void func_8004BC94(void*);
bool func_8005DAE4__Q22cf17CActParamAnimGame(void* self) {
    if (*(unsigned int*)((unsigned char*)self + 0x4ec) & 0x00040000) {
        func_8004BC94(self);
        return true;
    }
    return false;
}

void cf::CActParamAnimGame::func_8005DB1C() {}

void cf::CActorParam::CActorParam_UnkVirtualFunc38() { }

void cf::CActParamAnimGame::func_8005DC30() {}

// cf::CActParamAnimGame::func_8005DCA0 — trivial leaf, returns false
// Retail symbol lacks Fv suffix; preserves exact mangling for FULL_MATCH
bool func_8005DCA0__Q22cf17CActParamAnimGame() { return false; }

void cf::CActParamAnimGame::func_8005DCA8() {}

void func_8005DE68(){}

void func_8005E28C(){}

void func_8005E60C(){}

void func_8005E7C4(){}

void func_8005E990(){}

void cf::CActParamAnimGame::func_8005EEB4() {}

void cf::CActParamAnimGame::func_80060110() {}

void cf::CActParamAnimGame::func_80060268() {}

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
