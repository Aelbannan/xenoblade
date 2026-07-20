#include "kyoshin/menu/CMenuFade.hpp"

CMenuFade::CMenuFade(){

}

// LLM-HARNESS-BEGIN: us-801148f8
extern int lbl_eu_80663FA0;

extern "C" int func_80113E1C() {
    return lbl_eu_80663FA0;
}
// LLM-HARNESS-END: us-801148f8
// LLM-HARNESS-BEGIN: us-80114900
extern "C" u8 func_80113E24(void* pthis) {
    return *(u8*)((char*)pthis + 0x94);
}
// LLM-HARNESS-END: us-80114900
// LLM-HARNESS-BEGIN: us-80114908
extern "C" void func_80113E2C(CMenuFade* self) {
    *(u8*)((char*)self + 0x54) = 1;
}
// LLM-HARNESS-END: us-80114908
// LLM-HARNESS-BEGIN: us-80114914
extern "C" void __dt__9CMenuFadeFv(CMenuFade*);

extern "C" void func_80113E38(CMenuFade* p) {
    __dt__9CMenuFadeFv((CMenuFade*)((char*)p - 0x58));
}
// LLM-HARNESS-END: us-80114914
// LLM-HARNESS-BEGIN: us-8011491c
extern "C" bool func_80113E40() { return false; }
// LLM-HARNESS-END: us-8011491c
// LLM-HARNESS-BEGIN: us-80114924
extern "C" void func_80113E48(void* arg0) {
    __dt__9CMenuFadeFv((char*)arg0 - 0x5C);
}
// LLM-HARNESS-END: us-80114924

// LLM-HARNESS-BEGIN: us-80114760
extern "C" void func_80113C84() {}
// LLM-HARNESS-END: us-80114760
