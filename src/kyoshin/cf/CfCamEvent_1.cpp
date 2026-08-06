// Auto-scaffolded catalog TU for kyoshin/cf/CfCamEvent_1
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/harness_catalog.hpp"
#include "kyoshin/cf/object/CActorParam.hpp"

#include "kyoshin/cf/CfCamEvent_1.hpp"
void func_80074F4C(){}

void __ct__80075298(){}

void __dt__8007540C(){}

void func_800754C0(){}

void func_80075540(){}

void* func_800755B0(void* self, unsigned long idx) {
    return *(void**)((char*)self + (idx << 2));
}

// Resolve the shared global 'cam state' object; external C-function tail-call
// targets used by the shake helpers below.
extern "C" CfCamEventGlobal* lbl_eu_80663DF0;
extern "C" int func_8024125C(int state, int val);
extern "C" int func_80241344(int state, int val);
extern "C" int func_80240C98(int state, int arg0, int arg1);

// Categorise an environment/prompt id (r4) into a 0/1/2 bucket. The
// manager pointer in r3 is unused by this helper.
int func_800755BC(CfCamEventManager* /*unused*/, u32 idx) {
    if (idx >= 0x10 && idx <= 0x2b) return 1;
    if (idx >= 0x8 && idx <= 0xa)   return 2;
    if (idx <= 1) return 0;
    if (idx == 0xb) return 2;
    return 0;
}

// True when the global cam state exists and is in a "busy" frame range.
int func_8007560C() {
    if (lbl_eu_80663DF0 == nullptr) return 0;
    if (lbl_eu_80663DF0->field_0x3E < 0x10 || lbl_eu_80663DF0->field_0x3E > 0x2b) return 0;
    return 1;
}

// True when the global cam state exists and is in the "intro" frame range.
int func_80075640() {
    if (lbl_eu_80663DF0 == nullptr) return 0;
    if (lbl_eu_80663DF0->field_0x3E < 0 || lbl_eu_80663DF0->field_0x3E > 1) return 0;
    return 1;
}

void func_80075674(){}

void func_800756D0(){}

void func_80075934(){}

void func_800762A0(){}

void func_80076C08(){}

void func_80076CE4(){}

void func_80076D8C(){}

cf::CActorParam_UnkStruct1* cf::CActorParam::CActorParam_UnkVirtualFunc129() {
    return (cf::CActorParam_UnkStruct1*)((u8*)this + *(u8*)((u8*)this + 0x3354) * 0xbc + 0x2a84);
}

void func_80076F88(){}

void func_800778E4(){}

void func_80077F20(){}

void func_80078400(){}

void func_800784A0(){}

void func_80078B60(){}

void func_80078C08(){}

void func_80078D08(){}

void func_8007990C(){}

void func_80079B34(){}

void func_80079D6C(){}

// True while any of three "active" flags are set on the manager.
bool func_80079DBC(CfCamEventManager* manager) {
    if (manager->field_0x1DE) return true;
    CfCamEventSub1F4* sub = &manager->sub_0x1F4;
    if (sub->field_0x162) return true;
    if (sub->field_0x2DA) return true;
    return false;
}

void func_80079E04(){}

void func_8007AA4C(){}

extern "C" void func_8007B030(void* self) {
    *(u8*)((u8*)self + 0x1de) = 0;
    *(u8*)((u8*)self + 0x356) = 0;
    *(u8*)((u8*)self + 0x4ce) = 0;
}

// Forward shake state to the shared handler if the global cam state is alive.
int func_8007B044(int arg0, int arg1) {
    if (lbl_eu_80663DF0 == nullptr) return 0;
    if (lbl_eu_80663DF0->field_0x38 == 0) return 0;
    return func_80240C98(lbl_eu_80663DF0->field_0x38, arg0, arg1);
}

int func_8007B078(int val) {
    if (lbl_eu_80663DF0 == nullptr) return val;
    if (lbl_eu_80663DF0->field_0x38 == 0) return 0;
    return func_8024125C(lbl_eu_80663DF0->field_0x38, val);
}

int func_8007B0A0(int val) {
    if (lbl_eu_80663DF0 == nullptr) return val;
    if (lbl_eu_80663DF0->field_0x38 == 0) return 0;
    return func_80241344(lbl_eu_80663DF0->field_0x38, val);
}

void func_8007B0C8(){}

u8 func_8007BAE4() {
    u8 result = 0;
    if (lbl_eu_80663DF0) {
        result = lbl_eu_80663DF0->field_0x46;
    }
    return result;
}

void func_8007BAFC(){}

// --- hard-symbol stubs (scaffold_hard_symbols) ---
void sinit_8007BE74(){}
