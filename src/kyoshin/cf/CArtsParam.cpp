#include "kyoshin/cf/CArtsSet.hpp"

#include "kyoshin/cf/CArtsParam.hpp"
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

void func_8015403C(){}
float func_80154058(const u8* this_) { unsigned short raw = *(const unsigned short*)(this_ + 0x5a); int value = raw; if (value < 0) value = 0; else if (value > 4) value = 4; extern const float lbl_eu_80501978[]; return lbl_eu_80501978[value]; }
void func_8015408C(){}
extern const float lbl_eu_805019C0[];
float func_80154134(const u8* self) {
    int index = *reinterpret_cast<const unsigned short*>(self + 0x5a);
    if (index < 0)
        index = 0;
    else if (index > 5)
        index = 5;
    return lbl_eu_805019C0[index];
}
int func_80154168(const u8* self) { const unsigned short value = *reinterpret_cast<const unsigned short*>(self + 0x5c); return value == 3 || value == 4 || value == 11; }
bool func_8015419C(u8* self) {
    return *(unsigned short*)(self + 0x5C) == 11;
}

void func_801540C0(){}
void func_801541B0(){}
void func_80154280(){}
void func_801554DC(){}

void cf::CArtsParam::vtableFunc3(u8 val) {
    u8* o = *(u8**)((u8*)this + 0x88);
    if (o) *(u8*)((u8*)o + 0) = val;
}
