// Auto-scaffolded catalog TU for kyoshin/cf/CtrlNpc
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/harness_catalog.hpp"

#include "kyoshin/cf/CtrlNpc.hpp"
void __ct__CtrlNpc(){}

void func_8009377C(){}

void CfObjectMove_UnkVirtualFunc6__Q22cf12CfObjectMoveFv(void* self, unsigned long val) {
    void* ptr = *(void**)((char*)self + 0xc4);
    if (ptr != 0)
        *(unsigned long*)((char*)ptr + 0x37c) = val;
}

void func_80093938(){}

void func_8009398C(){}

void func_80093F28(){}

void cf::CObjectState::CObjectState_UnkVirtualFunc5() {}

void func_800948F8(){}

void func_80094A9C(){}

void func_80094CE8(){}

void func_80094D1C(){}

void func_80094DF4(){}

void func_80094E44(){}

void func_80094EDC(){}

void func_80094FC8(){}

float CfObject_UnkVirtualFunc73__Q22cf13CfObjectModelFv(void* self) { return *(float*)((u8*)self + 0xa4); }

float CfObject_UnkVirtualFunc71__Q22cf13CfObjectModelFv(void* self) { return *(float*)((u8*)self + 0xa0); }

void func_80095000(){}

void func_8009505C(){}

void func_800950E8(){}

void func_8009519C() {}

void func_800951A0(){}

void func_80095224(){}

void func_80095450(){}

struct CtrlNpcData {
    u8 _pad00[0xBA];
    short field_BA;
    u8 _padBC[2];
    short field_BE;
};

void func_8009563C(char* p) {
    CtrlNpcData* data = reinterpret_cast<CtrlNpcData*>(p);
    short v = data->field_BA - 1;
    data->field_BA = v;
    if (v <= 0) {
        data->field_BE = 1;
    }
}

void func_8009565C(){}

void func_80095F44(){}

void func_80096488(){}

void func_800964EC(){}

void func_800966E8(){}

void func_800967F8(){}

extern "C" int func_8009684C(void* self) { return 1; }

extern "C" int func_80096854(void* self) { return 0; }

void cf::CtrlNpc::~CtrlNpc() {}
