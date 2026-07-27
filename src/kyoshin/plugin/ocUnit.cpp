// Auto-scaffolded catalog TU for kyoshin/plugin/ocUnit
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/plugin/ocUnit.hpp"

void func_8003BC10(){}

void cf::CfObjectMove::CfObject_UnkVirtualFunc50() {}

void cf::CfObjectMove::CfObject_UnkVirtualFunc51() {}

void func_8003BD7C(){}

void func_8003C044(){}

bool cf::CfObject::CfObject_UnkVirtualFunc23() { return false; }

void func_8003C0D0(){}

void func_8003C154(){}

void func_8003C1D8(){}

float cf::CfObject::CfObject_UnkVirtualFunc34() {
    float f = *(float*)((char*)this + 0x4c);
    return f * 0.019592438f;
}

void func_8003C260(){}

bool cf::CObjectParam::CObjectParam_UnkVirtualFunc2() { return false; }

void func_8003C2F4(){}

void func_8003C354(){}

void func_8003C3D0(){}

bool cf::CActorParam::CActorParam_UnkVirtualFunc37() { return false; }

void func_8003C480(){}

extern "C" void CfObject_UnkVirtualFunc22__Q22cf8CfObjectFv(void* self, void* src) {
    u32 a = *(u32*)((u8*)src + 0);
    u32 b = *(u32*)((u8*)src + 4);
    u32 c = *(u32*)((u8*)src + 8);
    *(u32*)((u8*)self + 0x3C) = a;
    *(u32*)((u8*)self + 0x40) = b;
    *(u32*)((u8*)self + 0x44) = c;
}

void func_8003C560(){}

void func_8003C624(){}

void func_8003C6E8(){}

void cf::CfObject::CfObject_UnkVirtualFunc30() {
    void** vtable = *(void***)this;
    void (*func)(void*) = (void (*)(void*))vtable[0xC4 / 4];
    func(this);
}

bool cf::CfObject::CfObject_UnkVirtualFunc29() { return false; }

void func_8003C78C(){}

void cf::CObjectParam::CObjectParam_UnkVirtualFunc1() {}

bool isValid() { return false; }

void func_8003C84C(){}

void cf::CfObject::CfObject_UnkVirtualFunc26() {}

extern "C" void CfObject_UnkVirtualFunc19__Q22cf8CfObjectFv(void* self, void* src) {
    u32 a = *(u32*)((u8*)src + 0);
    u32 b = *(u32*)((u8*)src + 4);
    u32 c = *(u32*)((u8*)src + 8);
    *(u32*)((u8*)self + 0x3C) = a;
    *(u32*)((u8*)self + 0x40) = b;
    *(u32*)((u8*)self + 0x44) = c;
}

void dispOn(){}

void dispOff(){}

void cf::CObjectState::CObjectState_UnkVirtualFunc8() {}

extern "C" int CObjectState_UnkVirtualFunc2__Q22cf12CObjectStateFv(void* self, int mask) {
    return (*(int*)((char*)self + 4) & mask) != 0 ? 1 : 0;
}

void func_8003CB70(){}

void func_8003CC0C(){}

void func_8003CC9C(){}

extern "C" void CfObject_UnkVirtualFunc49__Q22cf8CfObjectFv() {}

void func_8003CD6C(){}

bool cf::CfObject::CfObject_UnkVirtualFunc50() { return false; }

void func_8003CDE0(){}

void walkR(){}

void func_8003CED0(){}

void func_8003CF48(){}

void func_8003D060(){}

void func_8003D2B8(){}

void func_8003D32C(){}

void moveTo(){}

void func_8003D570(){}

void func_8003D5DC(){}

void isTalk(){}

bool cf::CObjectState::CObjectState_UnkVirtualFunc11() { return false; }

void onEvent(){}

void cf::CObjectState::CObjectState_UnkVirtualFunc10() {}

void cf::CObjectState::CObjectState_UnkVirtualFunc9() {}

void func_8003D9C4(){}

void winTalk(){}

extern "C" void CObjectState_UnkVirtualFunc1__Q22cf12CObjectStateFv(void* self, unsigned long bits) {
    *(unsigned long*)((char*)self + 4) |= bits;
}

extern "C" void CObjectParam_UnkVirtualFunc6__Q22cf12CObjectParamFv(void* self, u32 val) { *(u32*)((u8*)self + 0x34) = val; }

void func_8003DC7C(){}

extern "C" void CObjectState_UnkVirtualFunc3__Q22cf12CObjectStateFv(void* self, unsigned long mask) {
    *(unsigned long*)((char*)self + 4) &= ~mask;
}

void func_8003DD44(){}

void func_8003DDAC(){}

void func_8003DDF4(){}

void setAct(){}

void func_8003DFE4(){}

void lookAt(){}

extern "C" void CfObjectModel_UnkVirtualFunc16__Q22cf13CfObjectModelFv(void* self, u8 val) { ((u8*)self)[0xBD] = val; }

extern "C" void turn() {}

void func_8003E478(){}

void func_8003E4D0(){}

void func_8003E528(){}

extern "C" u32 CObjectParam_UnkVirtualFunc5__Q22cf12CObjectParamFv(void* self) { return *(u32*)((u8*)self + 0x34); }

void func_8003E66C(){}

void isPC(){}

void isNPC(){}

void isENE(){}

void isPT(){}

void invin(){}

void func_8003E974(){}

void func_8003EB64(){}

void setColi(){}

void setEye(){}

void func_8003ECD8(){}

void func_8003ED50(){}

void func_8003EE04(){}

void func_8003EE74(){}

void func_8003EEE0(){}

void setRot(){}

extern "C" void CfObject_UnkVirtualFunc27__Q22cf8CfObjectFv(void* self, void* src) {
    u32 a = *(u32*)((u8*)src + 0);
    u32 b = *(u32*)((u8*)src + 4);
    u32 c = *(u32*)((u8*)src + 8);
    *(u32*)((u8*)self + 0x48) = a;
    *(u32*)((u8*)self + 0x4C) = b;
    *(u32*)((u8*)self + 0x50) = c;
}

void gravity(){}

extern "C" void CfObject_UnkVirtualFunc64__Q22cf8CfObjectFv(void* self, int flag) {
    u32* field = (u32*)((char*)self + 0x68);
    if (flag) {
        *field |= 0x01000000;
    } else {
        *field &= ~0x01000000;
    }
}

void func_8003F210(){}

void func_8003F398(){}

void func_8003F418(){}

void func_8003F498(){}

void func_8003F64C(){}

void func_8003F6CC(){}

void func_8003F74C(){}

void func_8003F7CC(){}

void func_8003F870(){}

void func_8003F97C(){}

void func_8003FA1C(){}

void func_8003FB18(){}

void func_8003FB98(){}

void func_8003FC18(){}

void func_8003FD48(){}

void func_8003FEDC(){}

void func_8003FFF4(){}

void ocUnitRegist(){}
