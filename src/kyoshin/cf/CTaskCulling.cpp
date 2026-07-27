#include "kyoshin/cf/CTaskCulling.hpp"
#include "kyoshin/cf/CfGameManager.hpp"
#include "kyoshin/code_800AA008.hpp"
#include "monolib/scn.hpp"
#include <cstring>

// Buffer header for spInstance->unk94 (occ culling data)
#pragma pack(push, 1)
struct OccBufferHead {
    char magic[4];   // "occ\0"
    u8 _04[2];       // padding
    u16 count;        // at offset 6
    u8 _08[8];        // 0x08-0x0F
    // OccFrustumEntry entries[] at offset 0x10
};

struct OccFrustumEntry {
    ml::CVec3 vec0;   // 0x00
    ml::CVec3 vec1;   // 0x0C
    ml::CVec3 vec2;   // 0x18
    u32 field_24;      // 0x24
    u8 _28[0x0C];      // 0x28-0x33 padding
};
#pragma pack(pop)

// Retail sbss singleton (config symbols.txt); mangled spInstance fails reloc name match.
cf::CTaskCulling* lbl_eu_80664328;

namespace cf{
    CTaskCulling* CTaskCulling::spInstance;

    //unused
    ml::CVec3 CTaskCulling::lbl_80579018_0 = ml::CVec3(5,30,50);
    ml::CVec3 CTaskCulling::lbl_80579018_C = ml::CVec3(5,30,50);
    ml::CVec3 CTaskCulling::lbl_80579018_18 = ml::CVec3(5,30,0);
    ml::CVec4 CTaskCulling::lbl_80579018_28 = ml::CVec4(0,1,0,0.2f);

    CTaskCulling::CTaskCulling(CScn* pScene):
    mpScene(pScene),
    unk94(nullptr),
    unk98(),
    unkDC(),
    unk120(0){
        spInstance = this;
        pScene->unkB4 = this;
        unk94 = mtl::MemManager::allocate_head(mtl::MemManager::getHandleMEM2(), 0x1000, 4);
    }

    CTaskCulling::~CTaskCulling(){
        spInstance = nullptr;
    }

void CTaskCulling::func_801A2BD0(unsigned long r3){
    CTaskCulling* instance = spInstance;
    
    if(instance != nullptr){
        if(r3 != 0){
            instance->unk120 |= 8;
        }else{
            instance->unk120 &= ~8;
        }
    }
}

    UNKTYPE* CTaskCulling::func_801A2C04(){
        if(lbl_eu_80664328 == nullptr) return nullptr;
        return lbl_eu_80664328->unk94;
    }

    bool CTaskCulling::ICulling_UnkVirtualFunc1(ml::CFrustum* r4){
        if(lbl_eu_80664328 == nullptr) return false;
        return mOccCulling.func_801A0F04(r4);
    }

bool CTaskCulling::ICulling_UnkVirtualFunc2(const ml::CVec3& r4, float r5){
        if(spInstance == nullptr) return false;
        return (unk120 & 8) ? false : mOccCulling.func_801A1444(r4, r5);
    }

bool CTaskCulling::ICulling_UnkVirtualFunc3(const ml::CVec3& r4, const ml::CVec3& r5, int r6){
        if(spInstance == nullptr) return false;
        return mOccCulling.func_801A1550(r4, r5, r6);
    }

void CTaskCulling::func_801A2C94(){
    CTaskCulling* instance = spInstance;
    if (instance == nullptr) return;
    instance->mOccCulling.func_801A0794();
}

    void CTaskCulling::func_801A2CAC(){
        if(spInstance == nullptr) return;

        func_801A2C94();

        cf::CfGameManager* gm = cf::CfGameManager::func_80083298();
        u32 r4, r5, r6, r7;

        func_800AA318(gm->unk70, &r4, &r5, &r6, &r7);
        func_800AA33C(spInstance->unk98, gm->unk70, 0, 0);
        func_800AA33C(spInstance->unkDC, gm->unk70, 1, 0);

        spInstance->unkDC.unkInline1("\\");

        OccBufferHead* head = (OccBufferHead*)spInstance->unk94;

        if(std::strcmp("occ", head->magic) == 0){
            OccFrustumEntry* entry = (OccFrustumEntry*)(head + 1); // at offset 0x10

            for (int i = 0; i < head->count; i++) {
                if(spInstance != nullptr){
                    spInstance->mOccCulling.addFrustum(entry->vec0, entry->vec1, entry->vec2, entry->field_24);
                }
                entry++;
            }
        }
    }

CTaskCulling* CTaskCulling::getInstance() {
    return lbl_eu_80664328;
}

    void CTaskCulling::Init(){
        cf::CfGameManager::spScene->addRenderCB(this, 1, 0);
        mOccCulling.func_801A06F8(mtl::MemManager::getHandleMEM2(), 0x8000);
    }
    void CTaskCulling::Term(){
        CScnNw4r* scene = cf::CfGameManager::spScene;
        if(scene != nullptr){
            scene->removeRenderCB(this);
        }

        mpScene->unkB4 = nullptr;
        //???
        if(spInstance != nullptr) spInstance->mOccCulling.func_801A0794();

        DELETE_OBJ(unk94);
    }
void CTaskCulling::Move(){}
void CTaskCulling::Draw(){}

void CTaskCulling::cbRenderBefore(){}

    CTaskCulling* CTaskCulling::create(CProcess* pParent, CScn* pScene){
        CTaskCulling* taskCulling = new (CWorkThreadSystem::getWorkMem()) CTaskCulling(pScene);
        taskCulling->Regist(pParent, false);
        return taskCulling;
    }

} //namespace cf





