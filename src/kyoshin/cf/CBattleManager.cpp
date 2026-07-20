#include "kyoshin/cf/CBattleManager.hpp"
#include "kyoshin/UnkClass_805764CC.hpp"
#include "kyoshin/cf/object/CfObjectPc.hpp"
#include "kyoshin/cf/object/CfObjectEne.hpp"
#include "kyoshin/cf/CfSoundMan.hpp"
#include "monolib/work.hpp"

extern UNKTYPE* func_8009EC9C(u16 r3);
extern void func_8009D7E4(UNKTYPE* r3, u32 r4);

namespace cf{
    u32 CBattleManager::lbl_804F8228[] = {
        0,
        0x1AD,
        0x1AA,
        0x1B0,
        0x1B6,
        0x1BA,
        0x1B5,
        0x1B7,
        0x1B8,
        0x1B9
    };

    //Plays attack sound effects when a player character hits an enemy?
    void func_800D7A04(CfObjectPc* pObjectPc, CfObjectEne* pObjectEne){
        CActorParam_UnkStruct1* r3 = pObjectPc->CActorParam_UnkVirtualFunc129();
        u32 flags = r3->mFlagsArray[0].flags;

        if(flags & CActorParam_UnkStruct1::FLAG_BIT_1){
            CfSoundMan::func_801BFC38(0, 0x192, 0, 0, 0.6f);
            CfSoundMan::func_801BFC38(0, 0x191, 0, 0, 0.6f);
        }else if(flags & 0x2000){
            CfSoundMan::func_801BFC38(0, 0x1C5, 0, 0, 0.6f);
        }else{
            CActorParam_UnkStruct2* r30 = r3->unk50;

            if(r30 != nullptr){
                if(r30->unk78 & CActorParam_UnkStruct1::FLAG_BIT_24){
                    if(pObjectEne->CActorParam_UnkVirtualFunc19() == 1 ||
                    pObjectEne->CActorParam_UnkVirtualFunc19() == 2){
                        if(!(r30->unk78 & CActorParam_UnkStruct1::FLAG_BIT_9)){
                            CfSoundMan::func_801BFC38(0, 0x1B4, 0, 0, 0.6f);
                            return;
                        }
                    }else if(!(r30->unk78 & CActorParam_UnkStruct1::FLAG_BIT_9)){
                        CfSoundMan::func_801BFC38(0, 0x1B5, 0, 0, 0.6f);
                        return;
                    }
                }else if((r30->unk78 & CActorParam_UnkStruct1::FLAG_BIT_25)
                && !(r30->unk78 & CActorParam_UnkStruct1::FLAG_BIT_9)){
                    CfSoundMan::func_801BFC38(0, 0x1B5, 0, 0, 0.6f);
                    return;
                }
                
                if((s32)r30->unk40 == 1){
                    if(flags & CActorParam_UnkStruct1::FLAG_BIT_8){
                        CfSoundMan::func_801BFC38(0, 0x1AF, 0, 0, 0.6f);
                    }else{
                        //likely an inline
                        if(pObjectEne != nullptr && pObjectEne->unk64 & 0x2){
                            UNKTYPE* r3_1 = func_8009EC9C(pObjectEne->unk8C_3);
                            func_8009D7E4((UNKTYPE*)((u32)r3_1 + 0x1C), 1);
                        }

                        if(pObjectEne->CActorParam_UnkVirtualFunc19() == 1 ||
                        pObjectEne->CActorParam_UnkVirtualFunc19() == 2){
                            CfSoundMan::func_801BFC38(0, 0x1AE, 0, 0, 0.6f);
                        }else{
                            CfSoundMan::func_801BFC38(0, 0x1AD, 0, 0, 0.6f);
                        }
                    }
                }else if((s32)r30->unk40 == 2){
                    if(flags & CActorParam_UnkStruct1::FLAG_BIT_8){
                        CfSoundMan::func_801BFC38(0, 0x1AC, 0, 0, 0.6f);
                    }else{
                        //likely an inline
                        if(pObjectEne != nullptr && pObjectEne->unk64 & 0x2){
                            UNKTYPE* r3_1 = func_8009EC9C(pObjectEne->unk8C_3);
                            func_8009D7E4((UNKTYPE*)((u32)r3_1 + 0x1C), 1);
                        }

                        if(pObjectEne->CActorParam_UnkVirtualFunc19() == 1 ||
                        pObjectEne->CActorParam_UnkVirtualFunc19() == 2){
                            CfSoundMan::func_801BFC38(0, 0x1AB, 0, 0, 0.6f);
                        }else{
                            CfSoundMan::func_801BFC38(0, 0x1AA, 0, 0, 0.6f);
                        }
                    }
                }else{
                    u32 r4 = CBattleManager::lbl_804F8228[r30->unk40];
                    CfSoundMan::func_801BFC38(0, r4, 0, 0, 0.6f);
                }
            }
        }
    }

    CBattleManager::CBattleManager() : unk84(0) {
        mtl::ALLOC_HANDLE heapIndex = CWorkThreadSystem::getWorkMem();
        mActorList1.reserve(heapIndex, 64);
        mActorList2.reserve(heapIndex, 8);
        mActorList3.reserve(heapIndex, 56);
        mBattleEventList.reserve(heapIndex, 4);
        UnkClass_805764CC* classPtr = UnkClass_805764CC::func_800B07E8();
        classPtr->func_800B8804(this);
        mVision.unk261C4.unk74 = 0;
    }

    CBattleManager::~CBattleManager(){
        UnkClass_805764CC* classPtr = UnkClass_805764CC::func_800B07E8();
        classPtr->func_800B88E0(this);
    }

CBattleManager* CBattleManager::getInstance() {
    return spInstance;
}

    void CBattleManager::func_800D9190(){
        spInstance = new (mtl::MemManager::getHandleMEM2()) CBattleManager();
    }

    void CBattleManager::func_800D91D0(){
        if(spInstance != nullptr){
            delete spInstance;
            spInstance = nullptr;
        }
    }

    void CBattleManager::func_800D9218(){
        mActorList1.clear();
        mActorList2.clear();
        mActorList3.clear();
        mVision.func_801A380C();
        unk19C.__ct__80192C10();
        mChain.func_8027728C();
        unk20C8.func_8027D1A4();
        mSuddenCommu.func_801BA1DC();
        mSuddenCommu.unk28 = 0;
        mSuddenCommu.unk2C = 0;
        unk88 = 0;
        unk8C = 0;
        unk90 = 0;
        mVision.unk261C4.unk70 = 0;
        unk94.clear();
        func_80085220(2, 0);
    }

    void CBattleManager::FactoryEvent2(){
    }

}

// LLM-HARNESS-BEGIN: us-800dab54
extern "C" void func_800DA06C() {}
// LLM-HARNESS-END: us-800dab54
// LLM-HARNESS-BEGIN: us-800e306c
extern "C" void func_800E2584(void* p, u32 mask) {
    *(u32*)((u8*)p + 0x84) &= ~mask;
}
// LLM-HARNESS-END: us-800e306c
// LLM-HARNESS-BEGIN: us-800eae6c
extern "C" void func_800EA384() {}
// LLM-HARNESS-END: us-800eae6c
// LLM-HARNESS-BEGIN: us-800eae94
extern "C" void func_800EA3AC() {}
// LLM-HARNESS-END: us-800eae94
// LLM-HARNESS-BEGIN: us-800eaef8
extern "C" void func_800EA410() {}
// LLM-HARNESS-END: us-800eaef8
// LLM-HARNESS-BEGIN: us-800eaf08
extern "C" void func_800EA420() {}
// LLM-HARNESS-END: us-800eaf08
// LLM-HARNESS-BEGIN: us-800eaf2c
extern "C" void func_800EA444() {}
// LLM-HARNESS-END: us-800eaf2c
// LLM-HARNESS-BEGIN: us-800eaf48
extern "C" void func_800EA460() {}
// LLM-HARNESS-END: us-800eaf48
// LLM-HARNESS-BEGIN: us-800eaf58
extern "C" void func_800EA470() {}
// LLM-HARNESS-END: us-800eaf58
// LLM-HARNESS-BEGIN: us-800eb480
extern "C" void func_800EA998() {}
// LLM-HARNESS-END: us-800eb480
// LLM-HARNESS-BEGIN: us-800ed3e4
extern "C" void func_800EC8FC() {}
// LLM-HARNESS-END: us-800ed3e4
// LLM-HARNESS-BEGIN: us-800f4440
extern "C" void func_800F3958() {}
// LLM-HARNESS-END: us-800f4440
// LLM-HARNESS-BEGIN: us-800f48b0
extern "C" void func_800F3DC8() {}
// LLM-HARNESS-END: us-800f48b0
// LLM-HARNESS-BEGIN: us-800f4aec
extern "C" void func_800F4004() {}
// LLM-HARNESS-END: us-800f4aec
// LLM-HARNESS-BEGIN: us-800f4d88
extern "C" void func_800F42A0(void* _this) {
    *(int*)((char*)_this + 0x84) = 0;
}
// LLM-HARNESS-END: us-800f4d88

// LLM-HARNESS-BEGIN: us-800d880c
extern "C" void func_800D7D24() {}
// LLM-HARNESS-END: us-800d880c
// LLM-HARNESS-BEGIN: us-800d8988
extern "C" void func_800D7EA0() {}
// LLM-HARNESS-END: us-800d8988
// LLM-HARNESS-BEGIN: us-800d8c90
extern "C" void func_800D81A8() {}
// LLM-HARNESS-END: us-800d8c90
// LLM-HARNESS-BEGIN: us-800d9e3c
extern "C" void func_800D9354() {}
// LLM-HARNESS-END: us-800d9e3c
// LLM-HARNESS-BEGIN: us-800da460
extern "C" void func_800D9978() {}
// LLM-HARNESS-END: us-800da460
// LLM-HARNESS-BEGIN: us-800da788
extern "C" void func_800D9CA0() {}
// LLM-HARNESS-END: us-800da788
// LLM-HARNESS-BEGIN: us-800dab8c
extern "C" void func_800DA0A4() {}
// LLM-HARNESS-END: us-800dab8c
// LLM-HARNESS-BEGIN: us-800dbbe4
extern "C" void func_800DB0FC() {}
// LLM-HARNESS-END: us-800dbbe4
// LLM-HARNESS-BEGIN: us-800dbfe4
extern "C" void func_800DB4FC() {}
// LLM-HARNESS-END: us-800dbfe4
// LLM-HARNESS-BEGIN: us-800dc2e0
extern "C" void func_800DB7F8() {}
// LLM-HARNESS-END: us-800dc2e0
// LLM-HARNESS-BEGIN: us-800dc514
extern "C" void func_800DBA2C() {}
// LLM-HARNESS-END: us-800dc514
// LLM-HARNESS-BEGIN: us-800dc5b4
extern "C" void func_800DBACC() {}
// LLM-HARNESS-END: us-800dc5b4
// LLM-HARNESS-BEGIN: us-800dd63c
extern "C" void func_800DCB54() {}
// LLM-HARNESS-END: us-800dd63c
// LLM-HARNESS-BEGIN: us-800e13d0
extern "C" void func_800E08E8() {}
// LLM-HARNESS-END: us-800e13d0
// LLM-HARNESS-BEGIN: us-800e2644
extern "C" void func_800E1B5C() {}
// LLM-HARNESS-END: us-800e2644
// LLM-HARNESS-BEGIN: us-800e307c
extern "C" void func_800E2594() {}
// LLM-HARNESS-END: us-800e307c
// LLM-HARNESS-BEGIN: us-800e3584
extern "C" void func_800E2A9C() {}
// LLM-HARNESS-END: us-800e3584
// LLM-HARNESS-BEGIN: us-800e6fb4
extern "C" void func_800E64CC() {}
// LLM-HARNESS-END: us-800e6fb4
// LLM-HARNESS-BEGIN: us-800e90d8
extern "C" void func_800E85F0() {}
// LLM-HARNESS-END: us-800e90d8
// LLM-HARNESS-BEGIN: us-800e965c
extern "C" void CBattleManager_preCalcTotalDamage() {}
// LLM-HARNESS-END: us-800e965c
// LLM-HARNESS-BEGIN: us-800e9d04
extern "C" void func_800E921C() {}
// LLM-HARNESS-END: us-800e9d04
// LLM-HARNESS-BEGIN: us-800ea63c
extern "C" void func_800E9B54() {}
// LLM-HARNESS-END: us-800ea63c
// LLM-HARNESS-BEGIN: us-800eaacc
extern "C" void func_800E9FE4() {}
// LLM-HARNESS-END: us-800eaacc
// LLM-HARNESS-BEGIN: us-800ead8c
extern "C" void func_800EA2A4() {}
// LLM-HARNESS-END: us-800ead8c
// LLM-HARNESS-BEGIN: us-800eaf6c
extern "C" void func_800EA484() {}
// LLM-HARNESS-END: us-800eaf6c
// LLM-HARNESS-BEGIN: us-800eb490
extern "C" void func_800EA9A8() {}
// LLM-HARNESS-END: us-800eb490
// LLM-HARNESS-BEGIN: us-800eb514
extern "C" void func_800EAA2C() {}
// LLM-HARNESS-END: us-800eb514
// LLM-HARNESS-BEGIN: us-800ed400
extern "C" void func_800EC918() {}
// LLM-HARNESS-END: us-800ed400
// LLM-HARNESS-BEGIN: us-800f421c
extern "C" void func_800F3734() {}
// LLM-HARNESS-END: us-800f421c
// LLM-HARNESS-BEGIN: us-800f42e0
extern "C" void func_800F37F8() {}
// LLM-HARNESS-END: us-800f42e0
// LLM-HARNESS-BEGIN: us-800f43c8
extern "C" void func_800F38E0() {}
// LLM-HARNESS-END: us-800f43c8
// LLM-HARNESS-BEGIN: us-800f4458
extern "C" void func_800F3970() {}
// LLM-HARNESS-END: us-800f4458
// LLM-HARNESS-BEGIN: us-800f46f0
extern "C" void func_800F3C08() {}
// LLM-HARNESS-END: us-800f46f0
// LLM-HARNESS-BEGIN: us-800f4754
extern "C" void func_800F3C6C() {}
// LLM-HARNESS-END: us-800f4754
// LLM-HARNESS-BEGIN: us-800f4974
extern "C" void func_800F3E8C() {}
// LLM-HARNESS-END: us-800f4974
// LLM-HARNESS-BEGIN: us-800f4a74
extern "C" void func_800F3F8C() {}
// LLM-HARNESS-END: us-800f4a74
// LLM-HARNESS-BEGIN: us-800f4ab0
extern "C" void func_800F3FC8() {}
// LLM-HARNESS-END: us-800f4ab0
// LLM-HARNESS-BEGIN: us-800f4b1c
extern "C" void func_800F4034() {}
// LLM-HARNESS-END: us-800f4b1c
// LLM-HARNESS-BEGIN: us-800f4c88
extern "C" void func_800F41A0() {}
// LLM-HARNESS-END: us-800f4c88
