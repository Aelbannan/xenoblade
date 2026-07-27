#include "kyoshin/cf/object/CActorParam.hpp"
#include "monolib/math.hpp"

namespace cf {
    CActorParam::CActorParam(UNKTYPE* r4, UNKTYPE* r5) : CActorState(r4),
    unk15DC(r4), unk15E0(r5), unk1648(0), unk164A(0), unk164C(0) {
        *(u32*)((u32)unk3298.unk8 + 4) = 0;
        std::memset(unk335C, 0, sizeof(unk335C));
        *(u32*)((u32)unk3298.unk8 + 8) = 0;
        unk3358 = 0;
        unk335A = 2;
        
        float idk = 2.0f * (ml::pi/3.0f);

        unk3364 = 0;
        unk3368 = 0;
        unk3374 = 0;
        
        unk15FC = 0;
        unk15E4 = 2;
        unk15E8 = 1;
        unk15EC = 0;
        unk15F0 = 0;
        unk1600 = 0;
        unk1604 = 0;
        unk1608 = 0;
        unk1610 = 0;
        unk160E = 0;
        unk160C = 0;
        unk1618 = 0;
        unk1616 = 0; //r0
        unk1614 = 0; //r0
        unk1650.unk44 = 2;
        unk1650.unk4C = idk;
        unk161C = 0;
        unk1620 = 40;
        unk1624 = idk;
        unk1628 = 0;
        unk1630 = 3;

        unk2A80 = 0;
        unk336C = 0;

        unk1629 = 0;
        unk162A = 0;
        unk162B = 0;
        unk162C = 0;
        unk1634 = 0;
        unk1638 = 0;
        unk163C = 0;
        unk1640 = 0;
        unk1644 = 0;

        unk3370 = 0;
        
        unk3298.init();
        std::memset(unk2A84, 0, sizeof(unk2A84)); //lol sure, go ahead and wipe everything
        unk31DC.init();

        unk3354 = 0;
        unk337C = 0;

        for(int i = 0; i < ARRAY_SIZE(unk1928); i++){
            unk1928[i].init();
        }
    }
}

void cf::CActorParam::CActorParam_UnkVirtualFunc1() {}
int CActorParam_UnkVirtualFunc2__Q22cf11CActorParamFv(void* self) { return 0x0; }
void cf::CActorParam::CActorParam_UnkVirtualFunc35() {}
void cf::CActorParam::CActorParam_UnkVirtualFunc54() {}
void cf::CActorParam::CActorParam_UnkVirtualFunc60() {}
void cf::CActorParam::CActorParam_UnkVirtualFunc40() {}
void cf::CActorParam::CActorParam_UnkVirtualFunc46() {}
void cf::CActorParam::CActorParam_UnkVirtualFunc52() {}
void CActorParam_UnkVirtualFunc58__Q22cf11CActorParamFv() {}
void CActorParam_UnkVirtualFunc64__Q22cf11CActorParamFv() {}
void cf::CActorParam::CActorParam_UnkVirtualFunc141() {}
void cf::CActorParam::CActorParam_UnkVirtualFunc142() {}
void cf::CActorParam::CActorParam_UnkVirtualFunc143() {}
void cf::CActorParam::CActorParam_UnkVirtualFunc144() {}
void cf::CActorParam::CActorParam_UnkVirtualFunc149() {}
void cf::CActorParam::CActorParam_UnkVirtualFunc150() {}
s16 CActorParam_UnkVirtualFunc155__Q22cf11CActorParamFv(void* self) { return *(s16*)((u8*)self + 0x3358); }
s16 CActorParam_UnkVirtualFunc157__Q22cf11CActorParamFv(void* self) { return *(s16*)((u8*)self + 0x335a); }
void cf::CActorParam::CActorParam_UnkVirtualFunc160() {}
float CActorParam_UnkVirtualFunc23__Q22cf11CActorParamFv(void* self) { return *(float*)((u8*)self + 0x15e8); }
void CActorParam_UnkVirtualFunc21__Q22cf11CActorParamFv(void* self, float val) { *(float*)((u8*)self + 0x15e8) = val; }

void func_80175A50(){}
void func_801765A4(){}
void cf::CActorParam::CActorParam_UnkVirtualFunc177() {}
void cf::CActorParam::CActorParam_UnkVirtualFunc4() {}
void cf::CActorParam::CActorParam_UnkVirtualFunc174() {}
void cf::CActorParam::CActorParam_UnkVirtualFunc175() {}
void cf::CActorParam::CActorParam_UnkVirtualFunc176() {}
void cf::CActorParam::CActorParam_UnkVirtualFunc10() {}
void cf::CActorParam::CActorParam_UnkVirtualFunc11() {}
void cf::CActorParam::CActorParam_UnkVirtualFunc12() {}
void cf::CActorParam::CActorParam_UnkVirtualFunc13() {}
void cf::CActorParam::CActorParam_UnkVirtualFunc179() {}
// Forward declaration - body kept in separate section
void CActorParam_UnkVirtualFunc180__Q22cf11CActorParamFv();
void cf::CActorParam::CActorParam_UnkVirtualFunc5() {}
void cf::CActorParam::CActorParam_UnkVirtualFunc6() {}
void cf::CActorParam::CActorParam_UnkVirtualFunc9() {}
void cf::CActorParam::CActorParam_UnkVirtualFunc48() {}
void cf::CActorParam::CActorParam_UnkVirtualFunc67() {}
void cf::CActorParam::CActorParam_UnkVirtualFunc70() {}
void cf::CActorParam::CActorParam_UnkVirtualFunc7() {}
void cf::CActorParam::CActorParam_UnkVirtualFunc8() {}
void cf::CActorParam::CActorParam_UnkVirtualFunc138() {}
void cf::CActorParam::CActorParam_UnkVirtualFunc140() {}
void cf::CActorParam::CActorParam_UnkVirtualFunc153() {}
void cf::CActorParam::CActorParam_UnkVirtualFunc154() {}
void cf::CActorParam::CActorParam_UnkVirtualFunc156() {}
void cf::CActorParam::CActorParam_UnkVirtualFunc158() {}
void cf::CActorParam::CActorParam_UnkVirtualFunc159() {}
void cf::CActorParam::CActorParam_UnkVirtualFunc86() {}
void cf::CActorParam::CActorParam_UnkVirtualFunc87() {}
void cf::CActorParam::CActorParam_UnkVirtualFunc135() {}
void cf::CActorParam::CActorParam_UnkVirtualFunc136() {}
void cf::CActorParam::CActorParam_UnkVirtualFunc137() {}

void CActorParam_UnkVirtualFunc180__Q22cf11CActorParamFv(); void CBattleState_UnkVirtualFunc18__Q22cf11CActorParamFv(){
    CActorParam_UnkVirtualFunc180__Q22cf11CActorParamFv();
}

// BattleState thunks: adjust `this` by -8 and tail-call CActorParam methods.
typedef void (*CActorFn)(void*);

// us-80180170
void CActorParam_UnkVirtualFunc179__Q22cf11CActorParamFv();
void CBattleState_UnkVirtualFunc17__Q22cf11CActorParamFv(void* self) {
    ((CActorFn)CActorParam_UnkVirtualFunc179__Q22cf11CActorParamFv)((void*)((char*)self - 8));
}

// us-80180178
void CActorParam_UnkVirtualFunc1__Q22cf11CActorParamFv();
extern "C" void CBattleState_UnkVirtualFunc3__Q22cf11CActorParamFv(void* self) {
    ((CActorFn)CActorParam_UnkVirtualFunc1__Q22cf11CActorParamFv)((void*)((char*)self - 8));
}

// us-80180180
int CActorParam_UnkVirtualFunc2__Q22cf11CActorParamFv(void* self);
extern "C" int CBattleState_UnkVirtualFunc1__Q22cf11CActorParamFv(void* self) {
    return ((int(*)(void*))CActorParam_UnkVirtualFunc2__Q22cf11CActorParamFv)((char*)self - 8);
}

// us-80180188
extern "C" void CActorParam_UnkVirtualFunc3__Q22cf11CActorParamFv();
extern "C" void CBattleState_UnkVirtualFunc2__Q22cf11CActorParamFv(void* self) {
    ((CActorFn)CActorParam_UnkVirtualFunc3__Q22cf11CActorParamFv)((char*)self - 8);
}
