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

// LLM-HARNESS-BEGIN: us-8017798c
extern "C" void CActorParam_UnkVirtualFunc1__Q22cf11CActorParamFv() {}
// LLM-HARNESS-END: us-8017798c
// LLM-HARNESS-BEGIN: us-80177ee4
extern "C" int CActorParam_UnkVirtualFunc2__Q22cf11CActorParamFv(void* self) { return 0x0; }
// LLM-HARNESS-END: us-80177ee4
// LLM-HARNESS-BEGIN: us-80179e28
extern "C" void CActorParam_UnkVirtualFunc35__Q22cf11CActorParamFv() {}
// LLM-HARNESS-END: us-80179e28
// LLM-HARNESS-BEGIN: us-8017e404
extern "C" void CActorParam_UnkVirtualFunc54__Q22cf11CActorParamFv() {}
// LLM-HARNESS-END: us-8017e404
// LLM-HARNESS-BEGIN: us-8017e43c
extern "C" void CActorParam_UnkVirtualFunc60__Q22cf11CActorParamFv() {}
// LLM-HARNESS-END: us-8017e43c
// LLM-HARNESS-BEGIN: us-8017e998
extern "C" void CActorParam_UnkVirtualFunc40__Q22cf11CActorParamFv() {}
// LLM-HARNESS-END: us-8017e998
// LLM-HARNESS-BEGIN: us-8017e9ac
extern "C" void CActorParam_UnkVirtualFunc46__Q22cf11CActorParamFv() {}
// LLM-HARNESS-END: us-8017e9ac
// LLM-HARNESS-BEGIN: us-8017e9c0
extern "C" void CActorParam_UnkVirtualFunc52__Q22cf11CActorParamFv() {}
// LLM-HARNESS-END: us-8017e9c0
// LLM-HARNESS-BEGIN: us-8017ea9c
extern "C" void CActorParam_UnkVirtualFunc58__Q22cf11CActorParamFv() {}
// LLM-HARNESS-END: us-8017ea9c
// LLM-HARNESS-BEGIN: us-8017eaa8
extern "C" void CActorParam_UnkVirtualFunc64__Q22cf11CActorParamFv() {}
// LLM-HARNESS-END: us-8017eaa8
// LLM-HARNESS-BEGIN: us-8017f098
extern "C" void CActorParam_UnkVirtualFunc141__Q22cf11CActorParamFv() {}
// LLM-HARNESS-END: us-8017f098
// LLM-HARNESS-BEGIN: us-8017f0dc
extern "C" void CActorParam_UnkVirtualFunc142__Q22cf11CActorParamFv() {}
// LLM-HARNESS-END: us-8017f0dc
// LLM-HARNESS-BEGIN: us-8017f188
extern "C" void CActorParam_UnkVirtualFunc143__Q22cf11CActorParamFv() {}
// LLM-HARNESS-END: us-8017f188
// LLM-HARNESS-BEGIN: us-8017f1d0
extern "C" void CActorParam_UnkVirtualFunc144__Q22cf11CActorParamFv() {}
// LLM-HARNESS-END: us-8017f1d0
// LLM-HARNESS-BEGIN: us-8017f258
extern "C" void CActorParam_UnkVirtualFunc149__Q22cf11CActorParamFv() {}
// LLM-HARNESS-END: us-8017f258
// LLM-HARNESS-BEGIN: us-8017f334
extern "C" void CActorParam_UnkVirtualFunc150__Q22cf11CActorParamFv() {}
// LLM-HARNESS-END: us-8017f334
// LLM-HARNESS-BEGIN: us-8017f674
extern "C" s16 CActorParam_UnkVirtualFunc155__Q22cf11CActorParamFv(void* self) { return *(s16*)((u8*)self + 0x3358); }
// LLM-HARNESS-END: us-8017f674
// LLM-HARNESS-BEGIN: us-8017f850
extern "C" s16 CActorParam_UnkVirtualFunc157__Q22cf11CActorParamFv(void* self) { return *(s16*)((u8*)self + 0x335a); }
// LLM-HARNESS-END: us-8017f850
// LLM-HARNESS-BEGIN: us-8017fb40
extern "C" void CActorParam_UnkVirtualFunc160__Q22cf11CActorParamFv() {}
// LLM-HARNESS-END: us-8017fb40
// LLM-HARNESS-BEGIN: us-80180158
extern "C" float CActorParam_UnkVirtualFunc23__Q22cf11CActorParamFv(void* self) { return *(float*)((u8*)self + 0x15e8); }
// LLM-HARNESS-END: us-80180158
// LLM-HARNESS-BEGIN: us-80180160
extern "C" void CActorParam_UnkVirtualFunc21__Q22cf11CActorParamFv(void* self, float val) { *(float*)((u8*)self + 0x15e8) = val; }
// LLM-HARNESS-END: us-80180160

// LLM-HARNESS-BEGIN: us-80176e4c
extern "C" void func_80175A50() {}
// LLM-HARNESS-END: us-80176e4c
// LLM-HARNESS-BEGIN: us-801779a0
extern "C" void func_801765A4() {}
// LLM-HARNESS-END: us-801779a0
// LLM-HARNESS-BEGIN: us-80177eec
extern "C" void CActorParam_UnkVirtualFunc177__Q22cf11CActorParamFv() {}
// LLM-HARNESS-END: us-80177eec
// LLM-HARNESS-BEGIN: us-80179e38
extern "C" void CActorParam_UnkVirtualFunc4__Q22cf11CActorParamFv() {}
// LLM-HARNESS-END: us-80179e38
// LLM-HARNESS-BEGIN: us-8017c700
extern "C" void CActorParam_UnkVirtualFunc174__Q22cf11CActorParamFv() {}
// LLM-HARNESS-END: us-8017c700
// LLM-HARNESS-BEGIN: us-8017cdb0
extern "C" void CActorParam_UnkVirtualFunc175__Q22cf11CActorParamFv() {}
// LLM-HARNESS-END: us-8017cdb0
// LLM-HARNESS-BEGIN: us-8017cf48
extern "C" void CActorParam_UnkVirtualFunc176__Q22cf11CActorParamFv() {}
// LLM-HARNESS-END: us-8017cf48
// LLM-HARNESS-BEGIN: us-8017d594
extern "C" void CActorParam_UnkVirtualFunc10__Q22cf11CActorParamFv() {}
// LLM-HARNESS-END: us-8017d594
// LLM-HARNESS-BEGIN: us-8017d83c
extern "C" void CActorParam_UnkVirtualFunc11__Q22cf11CActorParamFv() {}
// LLM-HARNESS-END: us-8017d83c
// LLM-HARNESS-BEGIN: us-8017d9a0
extern "C" void CActorParam_UnkVirtualFunc12__Q22cf11CActorParamFv() {}
// LLM-HARNESS-END: us-8017d9a0
// LLM-HARNESS-BEGIN: us-8017e2fc
extern "C" void CActorParam_UnkVirtualFunc13__Q22cf11CActorParamFv() {}
// LLM-HARNESS-END: us-8017e2fc
// LLM-HARNESS-BEGIN: us-8017e370
extern "C" void CActorParam_UnkVirtualFunc179__Q22cf11CActorParamFv() {}
// LLM-HARNESS-END: us-8017e370
// LLM-HARNESS-BEGIN: us-8017e474
extern "C" void CActorParam_UnkVirtualFunc180__Q22cf11CActorParamFv() {}
// LLM-HARNESS-END: us-8017e474
// LLM-HARNESS-BEGIN: us-8017e5b8
extern "C" void CActorParam_UnkVirtualFunc5__Q22cf11CActorParamFv() {}
// LLM-HARNESS-END: us-8017e5b8
// LLM-HARNESS-BEGIN: us-8017e644
extern "C" void CActorParam_UnkVirtualFunc6__Q22cf11CActorParamFv() {}
// LLM-HARNESS-END: us-8017e644
// LLM-HARNESS-BEGIN: us-8017e6b8
extern "C" void CActorParam_UnkVirtualFunc9__Q22cf11CActorParamFv() {}
// LLM-HARNESS-END: us-8017e6b8
// LLM-HARNESS-BEGIN: us-8017e9d4
extern "C" void CActorParam_UnkVirtualFunc48__Q22cf11CActorParamFv() {}
// LLM-HARNESS-END: us-8017e9d4
// LLM-HARNESS-BEGIN: us-8017eab4
extern "C" void CActorParam_UnkVirtualFunc67__Q22cf11CActorParamFv() {}
// LLM-HARNESS-END: us-8017eab4
// LLM-HARNESS-BEGIN: us-8017eaec
extern "C" void CActorParam_UnkVirtualFunc70__Q22cf11CActorParamFv() {}
// LLM-HARNESS-END: us-8017eaec
// LLM-HARNESS-BEGIN: us-8017eb24
extern "C" void CActorParam_UnkVirtualFunc7__Q22cf11CActorParamFv() {}
// LLM-HARNESS-END: us-8017eb24
// LLM-HARNESS-BEGIN: us-8017ebdc
extern "C" void CActorParam_UnkVirtualFunc8__Q22cf11CActorParamFv() {}
// LLM-HARNESS-END: us-8017ebdc
// LLM-HARNESS-BEGIN: us-8017ec94
extern "C" void CActorParam_UnkVirtualFunc138__Q22cf11CActorParamFv() {}
// LLM-HARNESS-END: us-8017ec94
// LLM-HARNESS-BEGIN: us-8017ed30
extern "C" void CActorParam_UnkVirtualFunc140__Q22cf11CActorParamFv() {}
// LLM-HARNESS-END: us-8017ed30
// LLM-HARNESS-BEGIN: us-8017f410
extern "C" void CActorParam_UnkVirtualFunc153__Q22cf11CActorParamFv() {}
// LLM-HARNESS-END: us-8017f410
// LLM-HARNESS-BEGIN: us-8017f5f0
extern "C" void CActorParam_UnkVirtualFunc154__Q22cf11CActorParamFv() {}
// LLM-HARNESS-END: us-8017f5f0
// LLM-HARNESS-BEGIN: us-8017f67c
extern "C" void CActorParam_UnkVirtualFunc156__Q22cf11CActorParamFv() {}
// LLM-HARNESS-END: us-8017f67c
// LLM-HARNESS-BEGIN: us-8017f858
extern "C" void CActorParam_UnkVirtualFunc158__Q22cf11CActorParamFv() {}
// LLM-HARNESS-END: us-8017f858
// LLM-HARNESS-BEGIN: us-8017f8fc
extern "C" void CActorParam_UnkVirtualFunc159__Q22cf11CActorParamFv() {}
// LLM-HARNESS-END: us-8017f8fc
// LLM-HARNESS-BEGIN: us-8017fb94
extern "C" void CActorParam_UnkVirtualFunc86__Q22cf11CActorParamFv() {}
// LLM-HARNESS-END: us-8017fb94
// LLM-HARNESS-BEGIN: us-8017fbf0
extern "C" void CActorParam_UnkVirtualFunc87__Q22cf11CActorParamFv() {}
// LLM-HARNESS-END: us-8017fbf0
// LLM-HARNESS-BEGIN: us-8017fc50
extern "C" void CActorParam_UnkVirtualFunc135__Q22cf11CActorParamFv() {}
// LLM-HARNESS-END: us-8017fc50
// LLM-HARNESS-BEGIN: us-8017fd2c
extern "C" void CActorParam_UnkVirtualFunc136__Q22cf11CActorParamFv() {}
// LLM-HARNESS-END: us-8017fd2c
// LLM-HARNESS-BEGIN: us-8017fe00
extern "C" void CActorParam_UnkVirtualFunc137__Q22cf11CActorParamFv() {}
// LLM-HARNESS-END: us-8017fe00
