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

extern "C" void CActorParam_UnkVirtualFunc1__Q22cf11CActorParamFv() {}
extern "C" int CActorParam_UnkVirtualFunc2__Q22cf11CActorParamFv(void* self) { return 0x0; }
extern "C" void CActorParam_UnkVirtualFunc35__Q22cf11CActorParamFv() {}
extern "C" void CActorParam_UnkVirtualFunc54__Q22cf11CActorParamFv() {}
extern "C" void CActorParam_UnkVirtualFunc60__Q22cf11CActorParamFv() {}
extern "C" void CActorParam_UnkVirtualFunc40__Q22cf11CActorParamFv() {}
extern "C" void CActorParam_UnkVirtualFunc46__Q22cf11CActorParamFv() {}
extern "C" void CActorParam_UnkVirtualFunc52__Q22cf11CActorParamFv() {}
extern "C" void CActorParam_UnkVirtualFunc58__Q22cf11CActorParamFv() {}
extern "C" void CActorParam_UnkVirtualFunc64__Q22cf11CActorParamFv() {}
extern "C" void CActorParam_UnkVirtualFunc141__Q22cf11CActorParamFv() {}
extern "C" void CActorParam_UnkVirtualFunc142__Q22cf11CActorParamFv() {}
extern "C" void CActorParam_UnkVirtualFunc143__Q22cf11CActorParamFv() {}
extern "C" void CActorParam_UnkVirtualFunc144__Q22cf11CActorParamFv() {}
extern "C" void CActorParam_UnkVirtualFunc149__Q22cf11CActorParamFv() {}
extern "C" void CActorParam_UnkVirtualFunc150__Q22cf11CActorParamFv() {}
extern "C" s16 CActorParam_UnkVirtualFunc155__Q22cf11CActorParamFv(void* self) { return *(s16*)((u8*)self + 0x3358); }
extern "C" s16 CActorParam_UnkVirtualFunc157__Q22cf11CActorParamFv(void* self) { return *(s16*)((u8*)self + 0x335a); }
extern "C" void CActorParam_UnkVirtualFunc160__Q22cf11CActorParamFv() {}
extern "C" float CActorParam_UnkVirtualFunc23__Q22cf11CActorParamFv(void* self) { return *(float*)((u8*)self + 0x15e8); }
extern "C" void CActorParam_UnkVirtualFunc21__Q22cf11CActorParamFv(void* self, float val) { *(float*)((u8*)self + 0x15e8) = val; }

extern "C" void func_80175A50() {}
extern "C" void func_801765A4() {}
extern "C" void CActorParam_UnkVirtualFunc177__Q22cf11CActorParamFv() {}
extern "C" void CActorParam_UnkVirtualFunc4__Q22cf11CActorParamFv() {}
extern "C" void CActorParam_UnkVirtualFunc174__Q22cf11CActorParamFv() {}
extern "C" void CActorParam_UnkVirtualFunc175__Q22cf11CActorParamFv() {}
extern "C" void CActorParam_UnkVirtualFunc176__Q22cf11CActorParamFv() {}
extern "C" void CActorParam_UnkVirtualFunc10__Q22cf11CActorParamFv() {}
extern "C" void CActorParam_UnkVirtualFunc11__Q22cf11CActorParamFv() {}
extern "C" void CActorParam_UnkVirtualFunc12__Q22cf11CActorParamFv() {}
extern "C" void CActorParam_UnkVirtualFunc13__Q22cf11CActorParamFv() {}
extern "C" void CActorParam_UnkVirtualFunc179__Q22cf11CActorParamFv() {}
// Forward declaration - body kept in separate section
extern "C" void CActorParam_UnkVirtualFunc180__Q22cf11CActorParamFv();
extern "C" void CActorParam_UnkVirtualFunc5__Q22cf11CActorParamFv() {}
extern "C" void CActorParam_UnkVirtualFunc6__Q22cf11CActorParamFv() {}
extern "C" void CActorParam_UnkVirtualFunc9__Q22cf11CActorParamFv() {}
extern "C" void CActorParam_UnkVirtualFunc48__Q22cf11CActorParamFv() {}
extern "C" void CActorParam_UnkVirtualFunc67__Q22cf11CActorParamFv() {}
extern "C" void CActorParam_UnkVirtualFunc70__Q22cf11CActorParamFv() {}
extern "C" void CActorParam_UnkVirtualFunc7__Q22cf11CActorParamFv() {}
extern "C" void CActorParam_UnkVirtualFunc8__Q22cf11CActorParamFv() {}
extern "C" void CActorParam_UnkVirtualFunc138__Q22cf11CActorParamFv() {}
extern "C" void CActorParam_UnkVirtualFunc140__Q22cf11CActorParamFv() {}
extern "C" void CActorParam_UnkVirtualFunc153__Q22cf11CActorParamFv() {}
extern "C" void CActorParam_UnkVirtualFunc154__Q22cf11CActorParamFv() {}
extern "C" void CActorParam_UnkVirtualFunc156__Q22cf11CActorParamFv() {}
extern "C" void CActorParam_UnkVirtualFunc158__Q22cf11CActorParamFv() {}
extern "C" void CActorParam_UnkVirtualFunc159__Q22cf11CActorParamFv() {}
extern "C" void CActorParam_UnkVirtualFunc86__Q22cf11CActorParamFv() {}
extern "C" void CActorParam_UnkVirtualFunc87__Q22cf11CActorParamFv() {}
extern "C" void CActorParam_UnkVirtualFunc135__Q22cf11CActorParamFv() {}
extern "C" void CActorParam_UnkVirtualFunc136__Q22cf11CActorParamFv() {}
extern "C" void CActorParam_UnkVirtualFunc137__Q22cf11CActorParamFv() {}

extern "C" void CActorParam_UnkVirtualFunc180__Q22cf11CActorParamFv();
extern "C" void CBattleState_UnkVirtualFunc18__Q22cf11CActorParamFv() {
    CActorParam_UnkVirtualFunc180__Q22cf11CActorParamFv();
}

// BattleState thunks: adjust `this` by -8 and tail-call CActorParam methods.
typedef void (*CActorFn)(void*);

// us-80180170
extern "C" void CActorParam_UnkVirtualFunc179__Q22cf11CActorParamFv();
extern "C" void CBattleState_UnkVirtualFunc17__Q22cf11CActorParamFv(void* self) {
    ((CActorFn)CActorParam_UnkVirtualFunc179__Q22cf11CActorParamFv)((void*)((char*)self - 8));
}

// us-80180178
extern "C" void CActorParam_UnkVirtualFunc1__Q22cf11CActorParamFv();
extern "C" void CBattleState_UnkVirtualFunc3__Q22cf11CActorParamFv(void* self) {
    ((CActorFn)CActorParam_UnkVirtualFunc1__Q22cf11CActorParamFv)((void*)((char*)self - 8));
}

// us-80180180
extern "C" int CActorParam_UnkVirtualFunc2__Q22cf11CActorParamFv(void* self);
extern "C" int CBattleState_UnkVirtualFunc1__Q22cf11CActorParamFv(void* self) {
    return ((int(*)(void*))CActorParam_UnkVirtualFunc2__Q22cf11CActorParamFv)((char*)self - 8);
}

// us-80180188
extern "C" void CActorParam_UnkVirtualFunc3__Q22cf11CActorParamFv();
extern "C" void CBattleState_UnkVirtualFunc2__Q22cf11CActorParamFv(void* self) {
    ((CActorFn)CActorParam_UnkVirtualFunc3__Q22cf11CActorParamFv)((char*)self - 8);
}
