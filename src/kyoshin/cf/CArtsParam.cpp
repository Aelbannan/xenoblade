#include "kyoshin/cf/CArtsSet.hpp"

namespace cf {
    CArtsParam lbl_80577580;

    CAttackParam::CAttackParam(){
        unk0 = 0;
        unk20 = 0;
        unk78 = 0;
        CAttackParam_UnkVirtualFunc1();
    }

    CArtsParam::CArtsParam(){
        static_cast<CAttackParam*>(this)->CAttackParam_UnkVirtualFunc1();
    }

    void CArtsParam::CArtsParam_UnkVirtualFunc1(){
        CAttackParam::CAttackParam_UnkVirtualFunc1();
        unk88 = 0;
    }

    void CArtsParam::CArtsParam_UnkVirtualFunc3(u8 r4){
        if(unk88 != nullptr){
            unk0 = r4;
        }
    }

u8 CArtsParam::CArtsParam_UnkVirtualFunc2(){
    if(unk88 != nullptr){
        return *(u8*)unk88;
    }
    return unk2A;
}
}

// LLM-HARNESS-BEGIN: us-80154a80
extern "C" void func_8015403C() {}
// LLM-HARNESS-END: us-80154a80
// LLM-HARNESS-BEGIN: us-80154a9c
extern "C" float func_80154058(void* this_) { unsigned short raw = *(const unsigned short*)((const unsigned char*)this_ + 0x5a); int value = raw; if (value < 0) value = 0; else if (value > 4) value = 4; extern const float lbl_eu_80501978[]; return lbl_eu_80501978[value]; }
// LLM-HARNESS-END: us-80154a9c
// LLM-HARNESS-BEGIN: us-80154ad0
extern "C" void func_8015408C() {}
// LLM-HARNESS-END: us-80154ad0
// LLM-HARNESS-BEGIN: us-80154b78
extern "C" const float lbl_eu_805019C0[];
extern "C" float func_80154134(const void* self) {
    int index = *reinterpret_cast<const unsigned short*>(reinterpret_cast<const unsigned char*>(self) + 0x5a);
    if (index < 0)
        index = 0;
    else if (index > 5)
        index = 5;
    return lbl_eu_805019C0[index];
}
// LLM-HARNESS-END: us-80154b78
// LLM-HARNESS-BEGIN: us-80154bac
extern "C" int func_80154168(const void* self) { const unsigned short value = *reinterpret_cast<const unsigned short*>(static_cast<const unsigned char*>(self) + 0x5c); return value == 3 || value == 4 || value == 11; }
// LLM-HARNESS-END: us-80154bac
// LLM-HARNESS-BEGIN: us-80154be0
extern "C" bool func_8015419C(void* self) {
    return *(unsigned short*)((char*)self + 0x5C) == 11;
}
// LLM-HARNESS-END: us-80154be0

// LLM-HARNESS-BEGIN: us-80154b04
extern "C" void func_801540C0() {}
// LLM-HARNESS-END: us-80154b04
// LLM-HARNESS-BEGIN: us-80154bf4
extern "C" void func_801541B0() {}
// LLM-HARNESS-END: us-80154bf4
// LLM-HARNESS-BEGIN: us-80154cc4
extern "C" void func_80154280() {}
// LLM-HARNESS-END: us-80154cc4
// LLM-HARNESS-BEGIN: us-80155fa8
extern "C" void func_801554DC() {}
// LLM-HARNESS-END: us-80155fa8
