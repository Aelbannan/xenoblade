// Auto-scaffolded catalog TU for kyoshin/plugin/ocUnit
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/plugin/ocUnit.hpp"

extern "C" void func_8003BC10() {}

extern "C" void CfObject_UnkVirtualFunc50__Q22cf12CfObjectMoveFv() {}

extern "C" void CfObject_UnkVirtualFunc51__Q22cf12CfObjectMoveFv() {}

extern "C" void func_8003BD7C() {}

extern "C" void func_8003C044() {}

extern "C" bool CfObject_UnkVirtualFunc23__Q22cf8CfObjectFv() { return false; }

extern "C" void func_8003C0D0() {}

extern "C" void func_8003C154() {}

extern "C" void func_8003C1D8() {}

extern "C" float CfObject_UnkVirtualFunc34__Q22cf8CfObjectFv(void* self) {
    float f = *(float*)((char*)self + 0x4c);
    return f * 0.019592438f;
}

extern "C" void func_8003C260() {}

extern "C" bool CObjectParam_UnkVirtualFunc2__Q22cf12CObjectParamFv() { return false; }

extern "C" void func_8003C2F4() {}

extern "C" void func_8003C354() {}

extern "C" void func_8003C3D0() {}

extern "C" bool CActorParam_UnkVirtualFunc37__Q22cf11CActorParamFv() { return false; }

extern "C" void func_8003C480() {}

extern "C" void CfObject_UnkVirtualFunc22__Q22cf8CfObjectFv(void* self, void* src) {
    u32 a = *(u32*)((u8*)src + 0);
    u32 b = *(u32*)((u8*)src + 4);
    u32 c = *(u32*)((u8*)src + 8);
    *(u32*)((u8*)self + 0x3C) = a;
    *(u32*)((u8*)self + 0x40) = b;
    *(u32*)((u8*)self + 0x44) = c;
}

extern "C" void func_8003C560() {}

extern "C" void func_8003C624() {}

extern "C" void func_8003C6E8() {}

extern "C" void CfObject_UnkVirtualFunc30__Q22cf8CfObjectFv(void* self) {
    void** vtable = *(void***)self;
    void (*func)(void*) = (void (*)(void*))vtable[0xC4 / 4];
    func(self);
}

extern "C" bool CfObject_UnkVirtualFunc29__Q22cf8CfObjectFv() { return false; }

extern "C" void func_8003C78C() {}

extern "C" void CObjectParam_UnkVirtualFunc1__Q22cf12CObjectParamFv() {}

extern "C" bool isValid() { return false; }

extern "C" void func_8003C84C() {}

extern "C" void CfObject_UnkVirtualFunc26__Q22cf8CfObjectFv() {}

extern "C" void CfObject_UnkVirtualFunc19__Q22cf8CfObjectFv(void* self, void* src) {
    u32 a = *(u32*)((u8*)src + 0);
    u32 b = *(u32*)((u8*)src + 4);
    u32 c = *(u32*)((u8*)src + 8);
    *(u32*)((u8*)self + 0x3C) = a;
    *(u32*)((u8*)self + 0x40) = b;
    *(u32*)((u8*)self + 0x44) = c;
}

extern "C" void dispOn() {}

extern "C" void dispOff() {}

extern "C" void CObjectState_UnkVirtualFunc8__Q22cf12CObjectStateFv() {}

extern "C" int CObjectState_UnkVirtualFunc2__Q22cf12CObjectStateFv(void* self, int mask) {
    return (*(int*)((char*)self + 4) & mask) != 0 ? 1 : 0;
}

extern "C" void func_8003CB70() {}

extern "C" void func_8003CC0C() {}

extern "C" void func_8003CC9C() {}

extern "C" void CfObject_UnkVirtualFunc49__Q22cf8CfObjectFv() {}

extern "C" void func_8003CD6C() {}

extern "C" bool CfObject_UnkVirtualFunc50__Q22cf8CfObjectFv() { return false; }

extern "C" void func_8003CDE0() {}

extern "C" void walkR() {}

extern "C" void func_8003CED0() {}

extern "C" void func_8003CF48() {}

extern "C" void func_8003D060() {}

extern "C" void func_8003D2B8() {}

extern "C" void func_8003D32C() {}

extern "C" void moveTo() {}

extern "C" void func_8003D570() {}

extern "C" void func_8003D5DC() {}

extern "C" void isTalk() {}

extern "C" bool CObjectState_UnkVirtualFunc11__Q22cf12CObjectStateFv() { return false; }

extern "C" void onEvent() {}

extern "C" void CObjectState_UnkVirtualFunc10__Q22cf12CObjectStateFv() {}

extern "C" void CObjectState_UnkVirtualFunc9__Q22cf12CObjectStateFv() {}

extern "C" void func_8003D9C4() {}

extern "C" void winTalk() {}

extern "C" void CObjectState_UnkVirtualFunc1__Q22cf12CObjectStateFv(void* self, unsigned long bits) {
    *(unsigned long*)((char*)self + 4) |= bits;
}

extern "C" void CObjectParam_UnkVirtualFunc6__Q22cf12CObjectParamFv(void* self, u32 val) { *(u32*)((u8*)self + 0x34) = val; }

extern "C" void func_8003DC7C() {}

extern "C" void CObjectState_UnkVirtualFunc3__Q22cf12CObjectStateFv(void* self, unsigned long mask) {
    *(unsigned long*)((char*)self + 4) &= ~mask;
}

extern "C" void func_8003DD44() {}

extern "C" void func_8003DDAC() {}

extern "C" void func_8003DDF4() {}

extern "C" void setAct() {}

extern "C" void func_8003DFE4() {}

extern "C" void lookAt() {}

extern "C" void CfObjectModel_UnkVirtualFunc16__Q22cf13CfObjectModelFv(void* self, u8 val) { ((u8*)self)[0xBD] = val; }

extern "C" void turn() {}

extern "C" void func_8003E478() {}

extern "C" void func_8003E4D0() {}

extern "C" void func_8003E528() {}

extern "C" u32 CObjectParam_UnkVirtualFunc5__Q22cf12CObjectParamFv(void* self) { return *(u32*)((u8*)self + 0x34); }

extern "C" void func_8003E66C() {}

extern "C" void isPC() {}

extern "C" void isNPC() {}

extern "C" void isENE() {}

extern "C" void isPT() {}

extern "C" void invin() {}

extern "C" void func_8003E974() {}

extern "C" void func_8003EB64() {}

extern "C" void setColi() {}

extern "C" void setEye() {}

extern "C" void func_8003ECD8() {}

extern "C" void func_8003ED50() {}

extern "C" void func_8003EE04() {}

extern "C" void func_8003EE74() {}

extern "C" void func_8003EEE0() {}

extern "C" void setRot() {}

extern "C" void CfObject_UnkVirtualFunc27__Q22cf8CfObjectFv(void* self, void* src) {
    u32 a = *(u32*)((u8*)src + 0);
    u32 b = *(u32*)((u8*)src + 4);
    u32 c = *(u32*)((u8*)src + 8);
    *(u32*)((u8*)self + 0x48) = a;
    *(u32*)((u8*)self + 0x4C) = b;
    *(u32*)((u8*)self + 0x50) = c;
}

extern "C" void gravity() {}

extern "C" void CfObject_UnkVirtualFunc64__Q22cf8CfObjectFv(void* self, int flag) {
    u32* field = (u32*)((char*)self + 0x68);
    if (flag) {
        *field |= 0x01000000;
    } else {
        *field &= ~0x01000000;
    }
}

extern "C" void func_8003F210() {}

extern "C" void func_8003F398() {}

extern "C" void func_8003F418() {}

extern "C" void func_8003F498() {}

extern "C" void func_8003F64C() {}

extern "C" void func_8003F6CC() {}

extern "C" void func_8003F74C() {}

extern "C" void func_8003F7CC() {}

extern "C" void func_8003F870() {}

extern "C" void func_8003F97C() {}

extern "C" void func_8003FA1C() {}

extern "C" void func_8003FB18() {}

extern "C" void func_8003FB98() {}

extern "C" void func_8003FC18() {}

extern "C" void func_8003FD48() {}

extern "C" void func_8003FEDC() {}

extern "C" void func_8003FFF4() {}

extern "C" void ocUnitRegist() {}
