#include "kyoshin/cf/CBattleManager.hpp"

struct BMIf {
    virtual void _v008(); virtual void _v00C(); virtual void vf0010();
    virtual void _v014(); virtual void _v018(); virtual void _v01C(); virtual void _v020();
    virtual void vf0024();
};
#include "kyoshin/UnkClass_805764CC.hpp"
#include "kyoshin/cf/object/CfObjectPc.hpp"
#include "kyoshin/cf/object/CfObjectEne.hpp"
#include "kyoshin/cf/CfSoundMan.hpp"
#include "kyoshin/cf/CfGameManager.hpp"
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
                            func_8009D7E4(&static_cast<UnkStruct_8009EC9C_Ret*>(r3_1)->unk1C, 1);
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
                            func_8009D7E4(&static_cast<UnkStruct_8009EC9C_Ret*>(r3_1)->unk1C, 1);
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

bool func_800DA06C(void* self, unsigned int value) { struct Node { Node* next; unsigned int unused; unsigned int value; }; struct Manager { unsigned char unused[8]; Node* list; }; Manager* manager = static_cast<Manager*>(self); Node* sentinel = manager->list; Node* current = sentinel->next; while (current != sentinel && current->value != value) current = current->next; return current != sentinel; }
void cf::CBattleManager::func_800E2584(u32 mask) {
    unk84 &= ~mask;
}
void* func_800EA384(void* self) { void* p = *reinterpret_cast<void**>(static_cast<char*>(self) + 0x8); if (*reinterpret_cast<void**>(p) == p) return nullptr; return *reinterpret_cast<void**>(*reinterpret_cast<char**>(*reinterpret_cast<void* volatile*>(static_cast<char*>(self) + 0x8)) + 0x8); }
void* func_800EA3AC(void* self, void* compareValue) {
    if (compareValue == nullptr) return nullptr;

    Func800EA384_Self* pSelf = (Func800EA384_Self*)self;
    SimpleListNode* sentinel = pSelf->listHead;
    SimpleListNode* current = sentinel->next;
    void* data;

    // goto to loop condition to match retail branch structure
    goto condition;

loop:
    current = current->next;

condition:
    if (current == sentinel) goto check_result;

    data = current->data;
    if (data != nullptr) {
        data = (u8*)data + 0x3e9c;
    }
    if (data != compareValue) goto loop;

check_result:
    if (current == sentinel) goto return_null;
    {
        SimpleListNode* nextNode = current->next;
        if (nextNode == sentinel) goto return_null;
        return nextNode->data;
    }

return_null:
    return nullptr;
}
extern "C" void func_800EA410(void* self) { reinterpret_cast<BMIf*>((u8*)self + 0x219c)->vf0010(); }
void func_800EA420(){}
unsigned int lbl_eu_80663F00;
void* func_801A8070(void*);
void* cf::CBattleManager::func_800EA444() {
    return lbl_eu_80663F00 ? func_801A8070(&mVision) : 0;
}
void cf::CBattleManager::func_800EA460(float a, float b, unsigned long c) {
    extern void func_800EA484(cf::CBattleManager*);
    unk88 = b;
    unk8C = c;
    unk90 = a;
    func_800EA484(this);
}
void func_800EA470(){}
extern "C" void func_800EA998(void* self) { reinterpret_cast<BMIf*>((u8*)self + 0x219c)->vf0024(); }
void func_800EC8FC(){}
void func_800F3958(){}
unsigned char func_800F3DC8(void* self, int key) { const unsigned char* item = static_cast<const unsigned char*>(self) + 0x94; for (int i = 0; i < 32; ++i) { if (*reinterpret_cast<const int*>(item) == key) return item[4]; item += 8; } return 0; }
void func_800F4004(void* this_) { unsigned char* self = static_cast<unsigned char*>(this_); void* anchor = *reinterpret_cast<void**>(self + 0x48); void* node = *reinterpret_cast<void**>(anchor); while (node != *reinterpret_cast<void**>(self + 0x48)) { unsigned char* object = *reinterpret_cast<unsigned char**>(static_cast<unsigned char*>(node) + 0x8); *reinterpret_cast<unsigned int*>(object + 0x3f04) |= 0x40; node = *reinterpret_cast<void**>(node); } }
void cf::CBattleManager::func_800F42A0() {
    unk84 = 0;
}

namespace cf {
    CChainCombo::~CChainCombo() {
    }

    CChainEffect::~CChainEffect() {
    }

    CChainTime::~CChainTime() {
        func_8027CE30();
    }

    UnkClass_800D8DBC::~UnkClass_800D8DBC() {
    }
}

void func_800D7D24(){}
void func_800D7EA0(){}
void func_800D81A8(){}
void func_800D9354(){}
void func_800D9978(){}
void func_800D9CA0(){}
void func_800DA0A4(){}
void func_800DB0FC(){}
void func_800DB4FC(){}
void func_800DB7F8(){}
void func_800DBA2C(){}
void func_800DBACC(){}
void func_800DCB54(){}
void func_800E08E8(){}
void func_800E1B5C(){}
void func_800E2594(){}
void func_800E2A9C(){}
void func_800E64CC(){}
void func_800E85F0(){}
void CBattleManager_preCalcTotalDamage(){}
void func_800E921C(){}
void func_800E9B54(){}
void func_800E9FE4(){}
// Declare functions used by func_800EA2A4 and func_800F3734
extern "C" bool func_8006EF04__Fi(int mask);

void func_800EA2A4(cf::CBattleManager* mgr, cf::CfObjectActor* actor) {
    if (actor == nullptr) return;

    cf::CfGameManager::getInstance();
    if (func_8006EF04__Fi(0x4000000)) return;

    CfObjectActor_Fields_3F00* actorFields = (CfObjectActor_Fields_3F00*)((u8*)actor);
    _reslist_node<cf::CfObjectActor*>* current;

    if (actorFields->field_0x3F00 & 0x2) {
        // Iterate over mActorList3 (all actors)
        current = mgr->mActorList3.mStartNodePtr->mNext;
        while (current != mgr->mActorList3.mStartNodePtr) {
            cf::CfObjectActor* obj = current->mItem;
            if (obj != actor) {
                // Call virtual function at vtable offset 0x2C8
                // Use proper virtual call through VirtualFunc2C8 to force r12 dispatch
                reinterpret_cast<VirtualFunc2C8*>(obj)->targetFunc(actorFields->field_0x3F10);
            }
            current = current->mNext;
        }
    } else {
        // Iterate over mActorList2 (team actors)
        current = mgr->mActorList2.mStartNodePtr->mNext;
        while (current != mgr->mActorList2.mStartNodePtr) {
            cf::CfObjectActor* obj = current->mItem;
            if (obj != actor) {
                // Call virtual function at vtable offset 0x2C8
                reinterpret_cast<VirtualFunc2C8*>(obj)->targetFunc(actorFields->field_0x3F10);
            }
            current = current->mNext;
        }
    }
}

void func_800EA484(){}
void func_800EA9A8(){}
void func_800EAA2C(){}
void func_800EC918(){}

s32 func_800F3734(u8* arg, cf::CfObjectActor* actorA, cf::CfObjectActor* actorB, cf::CfObjectActor* actorC) {
    if (actorC != nullptr) {
        // actorC->field_74 |= 0x80001000 (mFlagsArray[0].flags)
        ((cf::CActorParam_UnkStruct1*)((u8*)actorC))->mFlagsArray[0].flags |= 0x80001000;
    }

    // Check actorB flags at +0x30
    u32 flags = ((Func800F3734_Param5_Fields*)((u8*)actorB))->field_0x30;
    if (flags & 0x800) return 0;
    if (flags & 0x2000) return 0;

    // Get obj from actorC->field_50 (unk50)
    cf::CActorParam_UnkStruct2* obj = (actorC != nullptr) ? ((cf::CActorParam_UnkStruct1*)((u8*)actorC))->unk50 : nullptr;
    if (obj == nullptr || (int)((Func800EA2A4_Obj*)obj)->field_0x3C == 3) {
        func_8010989C(0);
        func_80109888(0);
        func_80109874(0);
        func_8010975C(9);
        func_80109770(0);
        func_80109734(((CfObjectActor_Fields_3F00*)((u8*)actorA))->field_0x3F10, 0);
    }

    return 0;
}
void func_800F37F8(){}
void func_800F38E0(){}
void func_800F3970(){}
void func_800F3C08(cf::CBattleManager* mgr, u32 arg) {
    mgr->func_80085220(2, arg);
    if (arg != 0) {
        std::memset(&mgr->unk94, 0, sizeof(cf::CBattleManager_Struct2));
    }
}
// Searches the 32-entry table at unk94 for a matching key.
// If found, increments the count byte for that entry and returns.
// If not found but there's an empty slot (key==0), inserts into the first empty slot.
void func_800F3C6C(cf::CBattleManager* this_, s32 key) {
    int ret = this_->func_800885F0(2);
    if (ret == 0) return;
    
    cf::CBattleManager_Struct1* pEntry = this_->unk94.unk0;
    s32 emptySlot = -1;
    int i = 0;
    
    for (i = 0; i < 32; pEntry++, i++) {
        if (pEntry->key == key) {
            pEntry->count++;
            return;
        }
        if (pEntry->key == 0 && emptySlot < 0) {
            emptySlot = i;
        }
    }
    
    if (emptySlot != -1) {
        this_->unk94.unk0[emptySlot].key = key;
        this_->unk94.unk0[emptySlot].count++;
    }
}
void func_800F3E8C(){}
extern u32 lbl_eu_80663E24;

void func_800F3F8C(cf::CBattleManager* mgr) {
    mgr->func_800E2584(0x10);
    lbl_eu_80663E24 |= 0x10000000;
}

void func_800F3FC8(cf::CBattleManager* mgr) {
    mgr->func_800E2584(0x10);
    lbl_eu_80663E24 &= ~0x10000000;
}
// Forward declarations for external standalone C functions used by func_800F4034
// (functions already declared in included headers are NOT redeclared here)
extern "C" void func_80277B34(void* chain);
extern "C" void* func_80043F18(void* holder);
extern "C" void func_800F4A98(void* list, int type, int filter);
extern "C" void* func_800F6EAC(void* list, u32 idx);
extern "C" void func_80197BA4(void* obj, u32, u32);
extern "C" void func_800BE12C(void* obj, int a, int b, int c, int d);

// Iterates over the filtered enum list, updates all active actors, then walks mActorList3 and mActorList2
void func_800F4034(cf::CBattleManager* mgr) {
    func_80277B34(&mgr->mChain);

    u8 holderBuf[8];
    func_80043D90(reinterpret_cast<CAIActionEnumHolder*>(holderBuf));
    func_800F4A98(func_80043F18(reinterpret_cast<CAIActionEnumHolder*>(holderBuf)), 0x20, 0);

    // Loop: func_80043F18 is called both in body and condition per iteration
    typedef void* (*Fe34Fn)(void*);
    typedef void* (*Fe34Fn)(void*);

    // Step 3: Iterate over filtered list
    // Note: func_80043F18 is called both in body and condition to match retail pattern
    void* list = NULL;
    u32 i = 0;
    cf::CfObjectActor* actor;
    goto cond;

loopBody:
    list = func_80043F18(reinterpret_cast<CAIActionEnumHolder*>(holderBuf));
    actor = static_cast<cf::CfObjectActor*>(
        (*reinterpret_cast<Fe34Fn>(&func_8016FE34))(
            func_800F6EAC(list, i)));
    actor->CActorParam_UnkVirtualFunc7();
    actor->CActorParam_UnkVirtualFunc9();
    actor->CActorParam_UnkVirtualFunc160();
    i++;

cond:
    list = func_80043F18(reinterpret_cast<CAIActionEnumHolder*>(holderBuf));
    if (i < *(unsigned int*)((unsigned char*)list + 0x620)) {
        goto loopBody;
    }

    __dt__80043E88(reinterpret_cast<CAIActionEnumHolder*>(holderBuf), -1);

    // Walk mActorList3 — field modification is BEFORE the null check (retail pattern)
    _reslist_node<cf::CfObjectActor*>* node = mgr->mActorList3.mStartNodePtr->mNext;
    while (node != mgr->mActorList3.mStartNodePtr) {
        cf::CfObjectActor* act = node->mItem;
        *(u32*)((u8*)act + 0x3F04) |= 0x40;
        if (act) {
            void* result = func_800AD860((u8*)act + 0x3E9C);
            if (result) {
                *(u32*)((u8*)result + 0x3F08) |= 0x08000000;
                func_80197BA4(result, 0, 0);
            }
        }
        node = node->mNext;
    }

    // Walk mActorList2
    node = mgr->mActorList2.mStartNodePtr->mNext;
    while (node != mgr->mActorList2.mStartNodePtr) {
        func_800BE12C((u8*)node->mItem + 0x3E9C, 0x31, 0, -1, 1);
        node = node->mNext;
    }
}
void func_800F41A0(){}
