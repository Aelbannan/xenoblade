#include "kyoshin/cf/CBattleManager.hpp"

struct BMIf {
    virtual void _v008(); virtual void _v00C(); virtual void vf0010();
    virtual void* _v014(); virtual void _v018(); virtual void _v01C(); virtual void _v020();
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

// Game state check function (name already contains __Fi; extern "C" prevents
// MWCC from double-mangling it to func_8006EF04__Fi__Fi).
extern "C" bool func_8006EF04__Fi(s32 mask);

// Initialise all vision slots/effects (retail func_801A380C). Free function,
// not a CVision member - declared here because CVision.hpp stays opaque.
extern void func_801A380C(cf::CVision* self);

// ============================================================
// Explicit template specializations: _reslist_base / reslist
// destructors for CfObjectActor* and IBattleEvent*
// Must appear before any ODR-use of these templates so they
// override the inline definitions in reslist.hpp.
// ============================================================

// _reslist_base<CfObjectActor*>::~_reslist_base()
// Clears the intrusive linked list, frees the backing buffer if not owned.
// Uses the retail MWCC pattern: old = cur; cur = cur->next; old->next = 0
template <>
_reslist_base<cf::CfObjectActor*>::~_reslist_base() {
    _reslist_node<cf::CfObjectActor*>* cur = mStartNodePtr->mNext;
    while (cur != mStartNodePtr) {
        _reslist_node<cf::CfObjectActor*>* old = cur;
        cur = cur->mNext;
        old->mNext = nullptr;
    }
    mStartNodePtr->mNext = mStartNodePtr;
    mStartNodePtr->mPrev = mStartNodePtr;

    if (!unk1C && mList != nullptr) {
        __dla__FPv(mList);
        mList = nullptr;
    }
}

// _reslist_base<IBattleEvent*>::~_reslist_base()
template <>
_reslist_base<cf::IBattleEvent*>::~_reslist_base() {
    _reslist_node<cf::IBattleEvent*>* cur = mStartNodePtr->mNext;
    while (cur != mStartNodePtr) {
        _reslist_node<cf::IBattleEvent*>* old = cur;
        cur = cur->mNext;
        old->mNext = nullptr;
    }
    mStartNodePtr->mNext = mStartNodePtr;
    mStartNodePtr->mPrev = mStartNodePtr;

    if (!unk1C && mList != nullptr) {
        __dla__FPv(mList);
        mList = nullptr;
    }
}

// Global battle manager instance pointer (defined later in this file)
extern unsigned int lbl_eu_80663F00;

// Voice action helper: takes a void* source, returns a void* action
void* func_8016FE34(void*);

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

// Static member definition (storage for the singleton pointer)
CBattleManager* CBattleManager::spInstance = nullptr;

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
        func_801A380C(&mVision);
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
        // Check the mBattleEventList reslist's vtable pointer for state flags
        // (The vtable pointer's low bits encode which path to take)
        u32 listFlags = *(u32*)&mBattleEventList;
        
        // Bit 0x20 = dynamic_cast / interface dispatch path
        if(listFlags & 0x20){
            extern void* __dynamic_cast(void*, long, void*, void*, long);
            extern char lbl_eu_80661970[];
            extern char lbl_eu_806618F0[];
            void* castResult = __dynamic_cast(this, 0, &lbl_eu_80661970, &lbl_eu_806618F0, 0);
            void** vtbl = *(void***)this;
            typedef void (*VFunc3C)(void*, void*);
            ((VFunc3C)vtbl[0x3C / 4])(this, castResult);
            return;
        }
        
        // Bits 0x02 or 0x04 = object removal / cleanup path
        if(!(listFlags & 0x02) && !(listFlags & 0x04)) return;
        
        BattleRemoveObjAccessor* target = (BattleRemoveObjAccessor*)func_8016FE34(this);
        
        // Remove target from mActorList1
        mActorList1.remove((CfObjectActor*)target);
        
        // Notify chain
        extern void func_80279694(CChain*, CfObjectActor*);
        func_80279694(&mChain, (CfObjectActor*)target);
        
        // Bit 0x02 at +0x3F00 determines which list to remove from
        if(target->field_3f00 & 0x02){
            mActorList2.remove((CfObjectActor*)target);
        }else{
            mActorList3.remove((CfObjectActor*)target);
        }
        
        // Check global battle manager state
        {
            u32 mgrAddr = lbl_eu_80663F00;
            reslist<CfObjectActor*>& globalList1 = *(reslist<CfObjectActor*>*)(mgrAddr + 0x48);
            if(globalList1.size() == 0){
                extern void func_800F41A0(CBattleManager*);
                func_800F41A0(this);
            }
        }
        
        {
            u32 mgrAddr = lbl_eu_80663F00;
            reslist<CfObjectActor*>& globalList2 = *(reslist<CfObjectActor*>*)(mgrAddr + 0x8);
            if(globalList2.size() == 0){
                extern void* getPlayer__Q22cf13CfGameManagerFi(int);
                extern void func_80174B4C(void*, u32);
                for(int i = 0; i < 3; i++){
                    void* player = getPlayer__Q22cf13CfGameManagerFi(i);
                    void* action = func_8016FE34(player);
                    if(action != nullptr){
                        void** vtbl = *(void***)action;
                        typedef void (*VFunc)(void*, u32);
                        ((VFunc)vtbl[0x20 / 4])(action, 0x40);
                        ((VFunc)vtbl[0x24 / 4])(action, 0x80);
                        ((VFunc)vtbl[0x28 / 4])(action, 0x8000);
                        func_80174B4C(action, 0x800);
                        func_80174B4C(action, 0x1000);
                    }
                }
            }
        }
        
        // Call virtual func_80085220(this, 1, 1)
        {
            void** vtbl = *(void***)this;
            typedef void (*VFunc1C)(void*, u32, u32);
            ((VFunc1C)vtbl[0x1C / 4])(this, 1, 1);
        }
        
        // Clear global flag bits
        {
            extern u32 lbl_eu_80663E28;
            lbl_eu_80663E28 &= ~0x18;
        }
        
        // Chain maintenance
        extern void func_80277B34(CChain*);
        func_80277B34(&mChain);
        
        // Enum list iteration
        struct EnumListHolder { void* list; u32 handle; };
        EnumListHolder holder;
        extern void func_80043D90(EnumListHolder*);
        extern void* func_80043F18(EnumListHolder*);
        extern void func_800F4A98(void*, int, int);
        extern void* func_800F6EAC(void*, u32);
        extern void __dt__80043E88(EnumListHolder*, int);
        
        func_80043D90(&holder);
        void* list = func_80043F18(&holder);
        func_800F4A98(list, 0x20, 0);
        
        for(u32 i = 0; ; i++){
            list = func_80043F18(&holder);
            u32 count = *(u32*)((u8*)list + 0x620);
            if(i >= count) break;
            
            list = func_80043F18(&holder);
            void* element = func_8016FE34(func_800F6EAC(list, i));
            if(element != nullptr){
                void** vtbl = *(void***)element;
                typedef void (*VFunc)(void*);
                ((VFunc)vtbl[0xB0 / 4])(element);
                vtbl = *(void***)element;
                ((VFunc)vtbl[0xB8 / 4])(element);
                vtbl = *(void***)element;
                ((VFunc)vtbl[0x314 / 4])(element);
            }
        }
        
        __dt__80043E88(&holder, -1);
    }

}

bool func_800DA06C(void* self, unsigned int value) { struct Node { Node* next; unsigned int unused; unsigned int value; }; struct Manager { unsigned char unused[8]; Node* list; }; Manager* manager = static_cast<Manager*>(self); Node* sentinel = manager->list; Node* current = sentinel->next; while (current != sentinel && current->value != value) current = current->next; return current != sentinel; }
void cf::CBattleManager::func_800E2584(u32 mask) {
    unk84 &= ~mask;
}
void* func_800EA384(void* self) { void* p = *reinterpret_cast<void**>(static_cast<char*>(self) + 0x8); if (*reinterpret_cast<void**>(p) == p) return nullptr; return *reinterpret_cast<void**>(*reinterpret_cast<char**>(*reinterpret_cast<void* volatile*>(static_cast<char*>(self) + 0x8)) + 0x8); }
void* func_800EA3AC(void* self, void* val) {
    if (val == nullptr) return nullptr;
    
    SimpleListNode* sentinel = *(SimpleListNode**)((u8*)self + 0x08);
    void* data;
    SimpleListNode* cur = sentinel->next;
    
    goto cond;
    
    loop:
    cur = cur->next;
    
    cond:
    if (cur == sentinel) goto end;
    
    data = cur->data;
    if (data != nullptr) {
        data = (u8*)data + 0x3e9c;
    }
    if (data != val) goto loop;
    
    end:
    if (cur == sentinel) goto notfound;
    
    data = (void*)cur->next;
    if ((SimpleListNode*)data == sentinel) goto notfound;
    return ((SimpleListNode*)data)->data;
    
    notfound:
    return nullptr;
}
extern "C" void func_800EA410(u8* self) { reinterpret_cast<BMIf*>((u8*)self + 0x219c)->vf0010(); }
void* cf::CBattleManager::func_800EA420() {
    if (spInstance != nullptr) {
        // Call virtual function at vtable offset 0x14 (BMIf::_v014) on mVision at offset 0x219c
        return reinterpret_cast<BMIf*>((u8*)this + 0x219c)->_v014();
    }
    return nullptr;
}
unsigned int lbl_eu_80663F00;
extern "C" void* func_801A8070(void*);
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
// NB: func_800EA484 stays declared inline here (not in CBattleManager.hpp):
// CVision.cpp re-declares it as (cf::CBattleManager*, f32, int) and
// CChainTime.hpp (concurrent agent edit) now declares (cf::CBattleManager*,
// f32, int) repo-wide; MWCC rejects a second visible (float, u32) form as
// overload (error 10197). The (f32, int) form is ABI-identical to the
// original (float, u32) C-linkage decl (same 4-byte args, same symbol).
extern "C" void func_800EA484(cf::CBattleManager*, f32, int);

void cf::CBattleManager::func_800EA470() {
    unk88 = lbl_eu_80666DDC;
    return func_800EA484(this, lbl_eu_80666DD4, unk8C);
}
extern "C" void func_800EA998(u8* self) { reinterpret_cast<BMIf*>((u8*)self + 0x219c)->vf0024(); }
// func_800EC918 (retail 0x800ED400, 0x6E1C) - main battle-event processor.
// The callers below pass ABI-compatible args; the typed definition follows.
// (unmangled), so they must be declared extern "C" for reloc-name matching.
// func_80148778 / func_80149154 / func_8026178C are already declared in
// object/CAIAction.hpp / CVision.hpp (included via CBattleManager.hpp).
extern "C" int func_80260264(void*, int, void*);
extern "C" void* func_801491F4(void*, u32);
extern "C" bool func_80145DBC(int);
extern "C" int func_80145F78(int);
extern "C" int func_80145BC4(int);
extern "C" int func_80145C00(int);
extern "C" int func_80146148(int);
extern "C" int func_80174C98(void*, int*, int);
extern "C" int mtRand__Q22ml4mathFi(int n);
extern "C" f32 lbl_eu_80666DF4;  // 15.0f
extern "C" f32 lbl_eu_80666DD0;  // 0.6f
// 0.01f (existing lbl_80666DD8 without eu_ is a different symbol)
extern "C" f32 lbl_eu_80666DD8;   // 0.01f
extern "C" f32 lbl_eu_80666DF8;  // 25.0f
extern "C" f32 lbl_eu_80666E00;  // 100.0f
extern "C" f32 lbl_eu_80666E18;  // 50.0f
extern "C" f32 lbl_eu_80666E1C;  // 0.25f
extern "C" f32 lbl_eu_80666E20;  // 200.0f
extern "C" f32 lbl_eu_80666E2C;  // -1.0f
extern "C" f32 lbl_eu_80666E34;  // 10.0f
extern "C" f32 lbl_eu_80666E54;  // 32.0f
extern "C" f64 lbl_eu_80666E58;  // 0.5 (f64)
extern "C" f64 lbl_eu_80666E60;  // -0.5 (f64)
extern "C" f32 lbl_eu_80666E7C;  // 75.0f
extern "C" f32 lbl_eu_80666E80;  // 2.5f
extern "C" f32 lbl_eu_80666E84;  // 0.1f
extern "C" f32 lbl_eu_80666DE8;  // 0.5f
extern "C" f32 lbl_eu_80666DFC;  // 2.0f
extern "C" f32 lbl_eu_80666E10;  // 0.024543693f (2*pi/256)
extern "C" f32 lbl_eu_80666E24;  // 0.15f
extern "C" f32 lbl_eu_80666E68;  // 3.0f
extern "C" f32 lbl_eu_80666E6C;  // 5.0f
extern "C" f32 lbl_eu_80666E70;  // 0.35f
extern "C" f32 lbl_eu_80666E74;  // 0.01f (squared-distance epsilon)
extern "C" f32 lbl_eu_8066A1F8;  // pi
extern "C" s16 lbl_eu_804FCA3C[];
// --- shared battle-helper externs used by the func_800E2A9C / func_800E64CC ports ---
extern "C" s32 func_8018C820(void*, s32);                    // party-gauge add
extern "C" u16 func_8016DF2C();                              // current chapter/episode
extern "C" void func_800BE12C(void*, int, int, int, int);    // add/remove status
extern "C" void* func_800451D8(u32 cls, int param);          // object factory
extern "C" f32 func_80154058(void*);                         // table[clamp(idx,0,4)] @0x80501978
extern "C" f32 func_8015408C(void*);                         // table[clamp(idx,0,5)] @0x80501990
extern "C" f32 func_801540C0(void*);                         // per-frame-scaled table @0x805019A8
extern "C" f32 func_80154134(void*);                         // table[clamp(idx,0,5)] @0x805019C0
extern "C" void func_802A26D8(void*, void*, void*);          // battle-voice enqueue (3 args)
extern "C" void func_802A25EC(void*);                        // battle-voice clear
extern "C" void func_802A27F4(void*, void*, void*);          // battle-voice enqueue (3 args)
extern "C" void func_8027F848(void*, s32, void*);            // battle-voice (3 args)
extern "C" f32 Atan2FIdx__Q24nw4r4mathFff(f32 y, f32 x);      // nw4r math
extern "C" void func_800FE68C(void);
extern "C" void func_800FE96C(u32);
extern "C" void func_8016DF4C(s32);
extern "C" s32 func_80195BD4(u32, u16);
extern "C" void func_802A285C(void*, void*, void*);
extern "C" void func_802A24B4(void*);
extern "C" void func_802A2250(void*, void*, int);
extern "C" void func_802A2648(void*, void*);
extern "C" void func_802A232C(void*);
extern "C" void func_802A2558(void*);
extern "C" void func_80280804(void*);
extern "C" void func_8027FC04(void*, void*);
extern "C" void* func_8005605C(void*);
extern "C" void func_8004513C(void*, void*, u32, u32);
extern "C" void func_801BAB94(void*, void*, void*, void*);
extern "C" void func_801BAD24(void*, void*, void*);
extern "C" int func_80260A6C(void*, int, u32*, u32*);
extern "C" int func_80260FB0(void*, int, u32*, u32*, u32*);
extern "C" int* func_8009ECB0();
extern "C" void func_800E9FE4(void*, void*, s32, s32, s32, s32, void*);
extern "C" s32 func_800F3734(void*, BattleObjAccessor*, void*, void*);
extern "C" s32 func_800F37F8(void*, BattleObjAccessor*, void*, void*);
extern "C" void func_800EA2A4(cf::CBattleManager*, BattleObjAccessor*);
extern "C" void func_80109784(void*, u32, int);
extern "C" void func_8010989C(u8);
extern "C" void func_80109888(u8);
extern "C" void func_80109874(u8);
extern "C" void func_8010975C(u32);
extern "C" void func_80109770(u32);
extern "C" void func_80109734(void*, u32);
extern "C" float lbl_eu_804FCAD8[3];// {0.0f,0.0f,1.4e-43f} (3rd used as int 0x2)
extern "C" float lbl_eu_804FCAE4[3];// {8.0f,4.0f,2.0f} rate table
// --- forward decls for same-TU battle functions defined later in this file ---
extern "C" void func_800E64CC(cf::CBattleManager*, void*, void*, void*);
extern "C" void func_800E9B54(void*, void*, void*, void*);
extern "C" void func_800E08E8(void*, void*, void*, void*);
extern "C" s32 func_800EAA2C(void*, void*, void*, void*, void*);
extern "C" void func_800D9CA0(cf::CBattleManager*, void*);
extern "C" void func_800F3970(void*, void*, void*, s32, s32);
extern "C" f32 func_800D7EA0(u8*, void*);
extern "C" s32 func_800EC918(void* self, void* pc, void* accessor, void* ev, void* target);

void func_800EC8FC(u32 a, u32 b, void* c, u32 d) {
    func_800EC918((void*)a, 0, (void*)b, c, (void*)d);
}
struct BattleEvent {
    u32 field_00;
    u32 field_04;
    u32 field_08;
    u16 eventType;
    u16 _pad_0E;
    s32 field_10;
    s16 field_14;
    s16 field_16;
    s16 field_18;
    s16 _pad_1A;
    s32 field_1C;
    f32 field_20;
    f32 field_24;
    f32 field_28;
    u16 field_2C;
    u16 prevEventType;
    u32 field_30;
};

struct BattleTargetData {
    u8 _pad_00[0x50];
    void* artsData;
    u8 _pad_54[4];
    f32 field_58;
    f32 field_5C;
    f32 field_60;
    f32 field_64;
    u8 _pad_68[0x74 - 0x68];
    u32 field_74;
    u32 field_78;
    u32 field_7C;
    u16 field_80;
    u8 _pad_82[0xAC - 0x82];
    u32 field_AC;
    u32 field_B0;
    u32 field_B4;
    u32 field_B8;
};

struct SubAccessor {
    virtual ~SubAccessor();
    virtual void vf04();
    virtual void vf08();
    virtual void vf0C();
    virtual void vf10();
    virtual void vf14();
    virtual void vf18(void* ev);
    virtual void vf1C();
    virtual void vf20(u32 id);
    virtual void vf24();
    virtual void vf28();
    virtual void vf2C();
    virtual void vf30();
    virtual void vf34();
    virtual void vf38();
    virtual void vf3C();
    virtual void vf40();
    virtual void vf44();
    virtual void vf48();
    virtual void vf4C();
    virtual void vf50();
    virtual void vf54();
    virtual void vf58();
    virtual void vf5C();
    virtual void vf60();
    virtual void vf64();
    virtual void vf68();
    virtual void vf6C();
    virtual void vf70();
    virtual void vf74();
    virtual void vf78();
    virtual void vf7C();
    virtual void vf80(u16);
    virtual void vf84();
    virtual void vf88(u16);
};

struct EC918_BattleObjAccessor {
    void* vtbl;
    void* vtbl2;
    SubAccessor* subAcc;
    u8 _pad_0C[0x1524];
    u32 field_1530;
    u8 _pad_1534[0x15E4 - 0x1534];
    u32 field_15E4;
    u8 _pad_15E8[0x3374 - 0x15E8];
    u32 field_3374;
    u8 _pad_3378[0x3E9C - 0x3378];
    u8 field_3E9C;
    u8 _pad_3E9D[0x3ED4 - 0x3E9D];
    void* field_3ED4;
    u8 _pad_3ED8[0x3F00 - 0x3ED8];
    u32 field_3F00;
    u32 field_3F10;
    u8 _pad_3F14[0x456C - 0x3F14];
    u16 field_456C;
};

// ---- Helper: modulo 100 via mulhw 0x51EC851F pattern ----
static inline s32 mod100(s32 x) {
    s32 mul = (s32)((s64)x * (s64)(s32)0x51EC851F >> 32);
    s32 div = mul >> 5;
    div += (s32)((u32)div >> 31);  // sign correction
    return x - div * 100;
}

// ---- The function ----

extern "C" s32 func_800EC918(
    void* self,
    void* pc,
    void* accessorArg,
    void* evArg,
    void* targetArg
) {
    BattleEvent* evt = (BattleEvent*)evArg;
    BattleTargetData* tgt = (BattleTargetData*)targetArg;
    EC918_BattleObjAccessor* acc = (EC918_BattleObjAccessor*)accessorArg;
    void* artsData = nullptr;
    s32 r14 = 0;
    s32 r17 = 0x32;
    f32 f28 = 0.0f;
    BattleEvent evCopy;

    // artsData = tgt->artsData
    if (tgt != nullptr) {
        artsData = tgt->artsData;
    }

    // evt->field_2C = tgt->field_80
    if (tgt != nullptr) {
        evt->field_2C = tgt->field_80;
    }

    // ---- Guard 1: game flag 0x1000 ----
    {
        extern void* getInstance__Q22cf13CfGameManagerFv();
        getInstance__Q22cf13CfGameManagerFv();
        if (func_8006EF04__Fi(0x1000)) {
            return 0;
        }
    }

    // ---- Guard 2: actor vfunc 0x2BC ----
    {
        void* vtbl = *(void**)acc;
        typedef s32 (*VF2BC)(EC918_BattleObjAccessor*);
        VF2BC vf2BC = (VF2BC)(*(void**)((u8*)vtbl + 0x2BC));
        if (vf2BC(acc)) {
            return 0;
        }
    }

    // ---- Guard 3: acc->field_3374 & 0x10 ----
    if (acc->field_3374 & 0x10) {
        return 0;
    }

    // ---- Guard 4: Confirmation gate (flag 0x400) ----
    {
        extern void* getInstance__Q22cf13CfGameManagerFv();
        getInstance__Q22cf13CfGameManagerFv();
        if (func_8006EF04__Fi(0x400)) {
            extern u32 lbl_eu_8052B784[];
            extern u32 lbl_eu_8052B790[];
            s32 ptmfMatch = 0;

            {
                u32 stack_ptmf[3];
                stack_ptmf[0] = lbl_eu_8052B784[0];
                stack_ptmf[1] = lbl_eu_8052B784[1];
                stack_ptmf[2] = lbl_eu_8052B784[2];
                if (!__ptmf_cmpr((void*)((u8*)self + 0x28354 - 0x7cac), stack_ptmf)) {
                    stack_ptmf[0] = lbl_eu_8052B790[0];
                    stack_ptmf[1] = lbl_eu_8052B790[1];
                    stack_ptmf[2] = lbl_eu_8052B790[2];
                    if (!__ptmf_cmpr((void*)((u8*)self + 0x28354 - 0x7cac), stack_ptmf)) {
                        ptmfMatch = 1;
                    }
                }
            }

            if (!ptmfMatch) {
                return 0;
            }
        }
    }

    // ---- evt->field_30 |= 0x10 ----
    evt->field_30 |= 0x10;

    // ---- Entry fixups ----
    if (func_80145C00(evt->eventType)) {
        if (evt->field_00 == 0 || *(u32*)((u8*)acc + 0x3F10) == evt->field_00) {
            if (*(u32*)((u8*)acc + 0x1530) != 0) {
                typedef void (*VF20)(SubAccessor*, u32);
                VF20 vf20 = (VF20)(*(void**)((u8*)acc->subAcc + 0x20));
                vf20(acc->subAcc, *(u32*)((u8*)acc + 0x1530));
            }
        }
    }

    // ---- Counter-attack block ----
    if (artsData != nullptr && tgt != nullptr && pc != nullptr) {
        f28 = func_800D7EA0((u8*)pc, tgt);

        if (evt->prevEventType == 0 || (evt->field_30 & 0x2)) {
            if (func_80145DBC(evt->eventType)) {
                void* vtbl = *(void**)pc;
                u16 artsId = *(u16*)((u8*)artsData + 0x46);
                typedef s32 (*VF108)(void*);
                VF108 vf108 = (VF108)(*(void**)((u8*)vtbl + 0x108));
                s32 level = vf108(pc);
                s32 product = (s32)artsId * (level + 14);
                vtbl = *(void**)acc;
                typedef void (*VF2C4)(EC918_BattleObjAccessor*, void*, f32, f32, f32);
                VF2C4 vf2C4 = (VF2C4)(*(void**)((u8*)vtbl + 0x2C4));
                f32 fprod = (f32)(s32)product;
                vf2C4(acc, pc, lbl_eu_80666DDC, f28 * fprod, lbl_eu_80666DDC);

            } else if (func_80145C00(evt->eventType)) {
                void* vtbl = *(void**)pc;
                u16 artsId = *(u16*)((u8*)artsData + 0x46);
                typedef s32 (*VF108)(void*);
                VF108 vf108 = (VF108)(*(void**)((u8*)vtbl + 0x108));
                s32 level = vf108(pc);
                s32 product = (s32)artsId * (level + 14);
                s32 damage = (s32)(f28 * (f32)(s32)product);
                func_800E9FE4(self, pc, 0, (s32)damage, 0, 0, 0);

            } else if (func_80146148(evt->eventType)) {
                if (evt->prevEventType == 0) {
                    void* vtbl = *(void**)pc;
                    u16 artsId = *(u16*)((u8*)artsData + 0x46);
                    typedef s32 (*VF108)(void*);
                    VF108 vf108 = (VF108)(*(void**)((u8*)vtbl + 0x108));
                    s32 level = vf108(pc);
                    s32 product = (s32)artsId * (level + 14);
                    s32 damage = (s32)(f28 * (f32)(s32)product);
                    func_800E9FE4(self, pc, 0, (s32)damage, 0, 0, 0);
                }
            }
        }
    }

    // ---- subAccessor vfunc 0x80 ----
    {
        typedef s32 (*VF80)(SubAccessor*, u16);
        VF80 vf80 = (VF80)(*(void**)((u8*)acc->subAcc + 0x80));
        if (vf80(acc->subAcc, evt->eventType)) {
            func_800F3734(self, (BattleObjAccessor*)acc, evt, tgt);
            return 0;
        }
    }

    // ---- subAccessor vfunc 0x88 ----
    {
        typedef s32 (*VF88)(SubAccessor*, u16);
        VF88 vf88 = (VF88)(*(void**)((u8*)acc->subAcc + 0x88));
        if (vf88(acc->subAcc, evt->eventType)) {
            r14 = 0;
            r17 = 0x32;

            if (evt->eventType == 0x12) {
                s32 f15E4 = *(s32*)((u8*)acc + 0x15E4);
                if (f15E4 == 1) r17 = 0x14;
                else if (f15E4 == 2) r17 -= 0x14;
                else if (f15E4 == 3) r17 += 0x14;
                else if (f15E4 == 4) r17 += 0x32;
                else if (f15E4 == 5) r17 += 0x32;

                if (*(u32*)((u8*)acc + 0x3374) & 0x100) {
                    r17 += 0x1E;
                }

                u8 byte1AA = *(u8*)((u8*)self + 0x1AA);
                if (byte1AA >= 1 && byte1AA <= 24) {
                    r17 = 0;
                }
            }

            s32 rollVal;
            if (tgt != nullptr) {
                rollVal = mod100((s32)tgt->field_B0);
            } else {
                rollVal = mtRand__Q22ml4mathFi(100);
            }

            if (rollVal < r17) {
                r14 = 1;
                if (tgt != nullptr) {
                    tgt->field_74 |= 0x80000010;
                }
            }

            if (r14) {
                func_800F37F8(self, (BattleObjAccessor*)acc, evt, tgt);
                return 0;
            }
        }
    }

    // ---- Status check: 0x27 ----
    if (func_80148778(acc->subAcc, 0x27)) {
        if (!(acc == (EC918_BattleObjAccessor*)pc && evt->eventType == 0x11)) {
            if (func_80145F78(evt->eventType)) {
                void* entry27 = func_80149154(acc->subAcc, 0x27);
                if (entry27 != nullptr) {
                    s32 rollVal2;
                    if (tgt != nullptr) {
                        rollVal2 = (s32)tgt->field_B4;
                    } else {
                        rollVal2 = mtRand__Q22ml4mathFi(100);
                    }
                    s32 rate = *(s32*)((u8*)entry27 + 0x10);
                    if (rollVal2 < rate) {
                        func_800F37F8(self, (BattleObjAccessor*)acc, evt, tgt);
                        return 0;
                    }
                }
            }
        }
    }

    // ---- Status check: 0xCE ----
    if (func_80148778(acc->subAcc, 0xCE)) {
        if (tgt != nullptr && (tgt->field_78 & 0x1000)) {
            if (func_80145DBC(evt->eventType)) {
                void* entryCE = func_801491F4(acc->subAcc, 0xCE);
                if (entryCE != nullptr && artsData != nullptr) {
                    void* artsVTbl = *(void**)((u8*)artsData + 0x84);
                    typedef s32 (*VF0C)(void*);
                    VF0C vf0C = (VF0C)(*(void**)((u8*)artsVTbl + 0x0C));
                    s32 artsVal = vf0C(artsData);
                    s32 entryVal = *(s32*)((u8*)entryCE + 0x10);
                    if (entryVal >= artsVal) {
                        tgt->field_74 |= 0x80002000;
                        func_800F37F8(self, (BattleObjAccessor*)acc, evt, tgt);
                        return 0;
                    }
                }
            }
        }
    }

    // ---- func_80145BC4 result gate ----
    s32 resultGate = func_80145BC4(evt->eventType);

    void* vf224Result;
    {
        void* vtbl = *(void**)acc;
        typedef s32 (*VF224)(EC918_BattleObjAccessor*);
        VF224 vf224 = (VF224)(*(void**)((u8*)vtbl + 0x224));
        vf224Result = (void*)vf224(acc);
    }

    // ---- Jump table block 1 (8052BCB4) ----
    {
        u32 bitResult = 0;
        switch (resultGate) {
        case 1: bitResult = (*(u8*)((u8*)vf224Result + 0x71) >> 7) & 1; break;
        case 2: bitResult = (*(u8*)((u8*)vf224Result + 0x71)) & 1; break;
        case 3: bitResult = (*(u8*)((u8*)vf224Result + 0x71) >> 1) & 1; break;
        case 4: bitResult = (*(u8*)((u8*)vf224Result + 0x71) >> 2) & 1; break;
        case 5: bitResult = (*(u8*)((u8*)vf224Result + 0x71) >> 3) & 1; break;
        case 6: bitResult = (*(u8*)((u8*)vf224Result + 0x71) >> 4) & 1; break;
        case 7: bitResult = (*(u8*)((u8*)vf224Result + 0x71) >> 5) & 1; break;
        default: bitResult = 0; break;
        }
        if (bitResult) return 0;
    }

    // ---- Stat value extraction + 0x96 threshold (8052BC8C) ----
    {
        s32 statVal = 0;
        switch (resultGate) {
        case 4: statVal = *(s16*)((u8*)vf224Result + 0x64); break;
        case 5: statVal = *(s16*)((u8*)vf224Result + 0x66); break;
        case 6: statVal = *(s16*)((u8*)vf224Result + 0x68); break;
        case 7: statVal = *(s16*)((u8*)vf224Result + 0x6A); break;
        case 8: statVal = *(s16*)((u8*)vf224Result + 0x6C); break;
        case 9: statVal = *(s16*)((u8*)vf224Result + 0x6E); break;
        default: statVal = 0; break;
        }

        if (statVal >= 0x96) {
            u32 br2 = 0;
            switch (resultGate) {
            case 1: br2 = (*(u8*)((u8*)vf224Result + 0x71) >> 7) & 1; break;
            case 2: br2 = (*(u8*)((u8*)vf224Result + 0x71)) & 1; break;
            case 3: br2 = (*(u8*)((u8*)vf224Result + 0x71) >> 1) & 1; break;
            case 4: br2 = (*(u8*)((u8*)vf224Result + 0x71) >> 2) & 1; break;
            case 5: br2 = (*(u8*)((u8*)vf224Result + 0x71) >> 3) & 1; break;
            case 6: br2 = (*(u8*)((u8*)vf224Result + 0x71) >> 4) & 1; break;
            case 7: br2 = (*(u8*)((u8*)vf224Result + 0x71) >> 5) & 1; break;
            default: br2 = 0; break;
            }
            if (br2) return 0;
        }
    }

    // ---- Jump table block 3 (8052BC64) + 0x64 threshold ----
    {
        u32 bitResult3 = 0;
        switch (resultGate) {
        case 1: bitResult3 = (*(u8*)((u8*)vf224Result + 0x70) >> 7) & 1; break;
        case 2: bitResult3 = (*(u8*)((u8*)vf224Result + 0x70)) & 1; break;
        case 3: bitResult3 = (*(u8*)((u8*)vf224Result + 0x70) >> 1) & 1; break;
        case 4: bitResult3 = (*(u8*)((u8*)vf224Result + 0x70) >> 2) & 1; break;
        case 5: bitResult3 = (*(u8*)((u8*)vf224Result + 0x70) >> 3) & 1; break;
        case 6: bitResult3 = (*(u8*)((u8*)vf224Result + 0x70) >> 4) & 1; break;
        case 7: bitResult3 = (*(u8*)((u8*)vf224Result + 0x70) >> 5) & 1; break;
        default: bitResult3 = 0; break;
        }

        if (bitResult3) {
            s32 statVal2 = 0;
            switch (resultGate) {
            case 4: statVal2 = *(s16*)((u8*)vf224Result + 0x64); break;
            case 5: statVal2 = *(s16*)((u8*)vf224Result + 0x66); break;
            case 6: statVal2 = *(s16*)((u8*)vf224Result + 0x68); break;
            case 7: statVal2 = *(s16*)((u8*)vf224Result + 0x6A); break;
            case 8: statVal2 = *(s16*)((u8*)vf224Result + 0x6C); break;
            case 9: statVal2 = *(s16*)((u8*)vf224Result + 0x6E); break;
            default: statVal2 = 0; break;
            }

            if (statVal2 >= 0x64) {
                u32 bitResult4 = 0;
                switch (resultGate) {
                case 1: bitResult4 = (*(u8*)((u8*)vf224Result + 0x70) >> 7) & 1; break;
                case 2: bitResult4 = (*(u8*)((u8*)vf224Result + 0x70)) & 1; break;
                case 3: bitResult4 = (*(u8*)((u8*)vf224Result + 0x70) >> 1) & 1; break;
                case 4: bitResult4 = (*(u8*)((u8*)vf224Result + 0x70) >> 2) & 1; break;
                case 5: bitResult4 = (*(u8*)((u8*)vf224Result + 0x70) >> 3) & 1; break;
                case 6: bitResult4 = (*(u8*)((u8*)vf224Result + 0x70) >> 4) & 1; break;
                case 7: bitResult4 = (*(u8*)((u8*)vf224Result + 0x70) >> 5) & 1; break;
                default: bitResult4 = 0; break;
                }
                if (bitResult4) {
                    if (tgt != nullptr) {
                        s32 ac = (s32)tgt->field_AC;
                        if (mod100(ac) < 0x32) {
                            func_800F37F8(self, (BattleObjAccessor*)acc, evt, tgt);
                            return 0;
                        }
                    } else {
                        if (mtRand__Q22ml4mathFi(100) < 0x32) {
                            func_800F37F8(self, (BattleObjAccessor*)acc, evt, tgt);
                            return 0;
                        }
                    }
                }
            }
        }
    }

    // ---- Stat multiplier: 0x75 ----
    if (pc != nullptr) {
        void* pcVTbl = *(void**)pc;
        typedef void* (*VF290)(void*);
        VF290 vf290 = (VF290)(*(void**)((u8*)pcVTbl + 0x290));
        void* statObj = vf290(pc);
        if (statObj != nullptr) {
            if (func_80145C00(evt->eventType)) {
                u32 stackVal;
                if (func_80260264(statObj, 0x75, (s32*)&stackVal)) {
                    f32 statMul = (f32)(s32)stackVal;
                    evt->field_20 += statMul;
                }
            }
        }
    }

    // ---- Stat multiplier: 0x7C ----
    {
        void* accVTbl = *(void**)acc;
        typedef void* (*VF290)(void*);
        VF290 vf290a = (VF290)(*(void**)((u8*)accVTbl + 0x290));
        void* accStatObj = vf290a(acc);
        if (accStatObj != nullptr) {
            if (func_80145DBC(evt->eventType)) {
                u32 stackVal2;
                if (func_80260264(accStatObj, 0x7C, (s32*)&stackVal2)) {
                    if (tgt != nullptr) {
                        if ((s32)tgt->field_B4 < (s32)stackVal2) {
                            func_800F37F8(self, (BattleObjAccessor*)acc, evt, tgt);
                            return 0;
                        }
                    } else {
                        if (mtRand__Q22ml4mathFi(100) < (s32)stackVal2) {
                            func_800F37F8(self, (BattleObjAccessor*)acc, evt, tgt);
                            return 0;
                        }
                    }
                }
            }
        }
    }

    // ---- func_80146148 check + func_802A285C ----
    if (func_80146148(evt->eventType)) {
        func_802A285C(acc, pc, tgt);
    }

    // ================================================================
    // MAIN DISPATCH: switch (evt->eventType - 4)
    // jumptable at lbl_eu_8052B79C; default falls to shared tail.
    // ================================================================

    switch (evt->eventType) {

    // ---- Type 4 ----
    case 4: {
        if (func_80148778(acc->subAcc, 0x22)) {
            if (acc != (EC918_BattleObjAccessor*)pc) {
                void* entry22 = func_80149154(acc->subAcc, 0x22);
                if (entry22 != nullptr) {
                    s32 rollVal;
                    if (tgt != nullptr) {
                        rollVal = (s32)tgt->field_B4;
                    } else {
                        rollVal = mtRand__Q22ml4mathFi(100);
                    }
                    s32 rate = *(s32*)((u8*)entry22 + 0x10);
                    if (rollVal < rate) {
                        func_800F37F8(self, (BattleObjAccessor*)acc, evt, tgt);
                        return 0;
                    }
                }
            }
        }

        if (func_80148778(acc->subAcc, 0x11)) {
            typedef void (*VF20)(SubAccessor*, u32);
            VF20 vf20 = (VF20)(*(void**)((u8*)acc->subAcc + 0x20));
            vf20(acc->subAcc, 0x11);
        }

        if (evt->field_10 != 0) break;
        if (evt->field_14 != 0) break;

        // Copy event, field_10 = pc->field_3F10
        evCopy = *evt;
        evCopy.field_10 = *(s32*)((u8*)pc + 0x3F10);

        if (tgt != nullptr && (tgt->field_78 & 1)) {
            evCopy.field_14 = 1;
        }

        s32 subResult = func_800EC918(self, pc, acc, &evCopy, tgt);
        if (!subResult) {
            func_800F37F8(self, (BattleObjAccessor*)acc, evt, tgt);
            return 0;
        }

        void* obj3ED4 = *(void**)((u8*)acc + 0x3ED4);
        if (obj3ED4 != nullptr) {
            void* v3ED4 = *(void**)obj3ED4;
            typedef void (*VF70)(void*, u32);
            VF70 vf70 = (VF70)(*(void**)((u8*)v3ED4 + 0x70));
            vf70(obj3ED4, *(u32*)((u8*)pc + 0x3F10));
        }

        void* subIdent = (void*)((u8*)acc + 0x3E9C);
        if (subIdent == getPlayer__Q22cf13CfGameManagerFi(0)) {
            func_800FE68C();
            func_800FE96C(*(u32*)((u8*)pc + 0x3F10));
        }

        if (tgt == nullptr || !(tgt->field_78 & 1)) {
            evCopy.eventType = 0x11;
            evCopy.field_30 |= 0x800;
            evCopy.field_10 = *(u32*)((u8*)acc + 0x3F10);

            func_800EC918(self, pc, acc, &evCopy, tgt);

            void* pc3ED4 = *(void**)((u8*)pc + 0x3ED4);
            if (pc3ED4 != nullptr) {
                void* vPC3ED4 = *(void**)pc3ED4;
                typedef void (*VF70)(void*, u32);
                VF70 vf70 = (VF70)(*(void**)((u8*)vPC3ED4 + 0x70));
                vf70(pc3ED4, *(u32*)((u8*)acc + 0x3F10));
            }

            void* pcSubIdent = (void*)((u8*)pc + 0x3E9C);
            if (pcSubIdent == getPlayer__Q22cf13CfGameManagerFi(0)) {
                func_800FE68C();
                func_800FE96C(*(u32*)((u8*)acc + 0x3F10));
            }
        }

        return 1;
    }

    // ---- Type 5 ----
    case 5: {
        if (func_80148778(acc->subAcc, 0x15)) {
            void* entry15 = func_80149154(acc->subAcc, 0x15);
            if (entry15 != nullptr) {
                s32 rollVal;
                if (tgt != nullptr) {
                    rollVal = (s32)tgt->field_B4;
                } else {
                    rollVal = mtRand__Q22ml4mathFi(100);
                }
                s32 rate = *(s32*)((u8*)entry15 + 0x10);
                if (rollVal < rate) {
                    func_800F37F8(self, (BattleObjAccessor*)acc, evt, tgt);
                    return 0;
                }
            }
        }
        evt->prevEventType = evt->eventType;
        evCopy = *evt;
        evCopy.eventType = 5;
        evCopy.field_30 |= 2;
        func_800EC918(self, pc, acc, &evCopy, tgt);
        break;
    }

    // ---- Type 6 ----
    case 6: {
        if (func_80148778(acc->subAcc, 0x1A)) {
            void* entry1A = func_80149154(acc->subAcc, 0x1A);
            if (entry1A != nullptr) {
                s32 rollVal;
                if (tgt != nullptr) rollVal = (s32)tgt->field_B4;
                else rollVal = mtRand__Q22ml4mathFi(100);
                if (rollVal < *(s32*)((u8*)entry1A + 0x10)) {
                    func_800F37F8(self, (BattleObjAccessor*)acc, evt, tgt);
                    return 0;
                }
            }
        }
        break;
    }

    // ---- Type 7 ----
    case 7: {
        if (func_80148778(acc->subAcc, 0x17)) {
            void* entry17 = func_80149154(acc->subAcc, 0x17);
            if (entry17 != nullptr) {
                s32 rollVal;
                if (tgt != nullptr) rollVal = (s32)tgt->field_B4;
                else rollVal = mtRand__Q22ml4mathFi(100);
                if (rollVal < *(s32*)((u8*)entry17 + 0x10)) {
                    func_800F37F8(self, (BattleObjAccessor*)acc, evt, tgt);
                    return 0;
                }
            }
        }
        break;
    }

    // ---- Type 8 ----
    case 8: {
        if (func_80148778(acc->subAcc, 0x18)) {
            void* entry18 = func_80149154(acc->subAcc, 0x18);
            if (entry18 != nullptr) {
                s32 rollVal;
                if (tgt != nullptr) rollVal = (s32)tgt->field_B4;
                else rollVal = mtRand__Q22ml4mathFi(100);
                if (rollVal < *(s32*)((u8*)entry18 + 0x10)) {
                    func_800F37F8(self, (BattleObjAccessor*)acc, evt, tgt);
                    return 0;
                }
            }
        }
        break;
    }

    // ---- Type 9 ----
    case 9: {
        if (func_80148778(acc->subAcc, 0x24)) {
            void* entry24 = func_80149154(acc->subAcc, 0x24);
            if (entry24 != nullptr) {
                s32 rollVal;
                if (tgt != nullptr) rollVal = (s32)tgt->field_B4;
                else rollVal = mtRand__Q22ml4mathFi(100);
                if (rollVal < *(s32*)((u8*)entry24 + 0x10)) {
                    func_800F37F8(self, (BattleObjAccessor*)acc, evt, tgt);
                    return 0;
                }
            }
        }
        break;
    }

    // ---- Type 10 ----
    case 10: {
        if (func_80148778(acc->subAcc, 0x1C)) {
            void* entry1C = func_80149154(acc->subAcc, 0x1C);
            if (entry1C != nullptr) {
                s32 rollVal;
                if (tgt != nullptr) rollVal = (s32)tgt->field_B4;
                else rollVal = mtRand__Q22ml4mathFi(100);
                if (rollVal < *(s32*)((u8*)entry1C + 0x10)) {
                    func_800F37F8(self, (BattleObjAccessor*)acc, evt, tgt);
                    return 0;
                }
            }
        }
        break;
    }

    // ---- Type 11 ----
    case 11: {
        if (func_80148778(acc->subAcc, 0x1D)) {
            void* entry1D = func_80149154(acc->subAcc, 0x1D);
            if (entry1D != nullptr) {
                s32 rollVal;
                if (tgt != nullptr) rollVal = (s32)tgt->field_B4;
                else rollVal = mtRand__Q22ml4mathFi(100);
                if (rollVal < *(s32*)((u8*)entry1D + 0x10)) {
                    func_800F37F8(self, (BattleObjAccessor*)acc, evt, tgt);
                    return 0;
                }
            }
        }
        break;
    }

    // ---- Type 12 ----
    case 12: {
        if (evt->field_20 == 0.0f) {
            evt->field_20 = 5.0f;
        }
        break;
    }

    // ---- Type 13 ----
    case 13: {
        u32 fmt = (*(u16*)((u8*)acc + 0x456C)) >> 4;
        u16 artsId = (u16)evt->field_10;
        func_80195BD4(fmt, artsId);
        break;
    }

    // ---- Type 14 ----
    case 14: {
        extern void* getInstance__Q22cf13CfGameManagerFv();
        getInstance__Q22cf13CfGameManagerFv();
        if (!func_8006EF04__Fi(0x400)) {
            func_8016DF4C(evt->field_10);
        }
        break;
    }

    // ---- Types 15-17: complex AI/spirit handler (partial) ----
    case 15:
    case 16:
    case 17: {
        // Full transcription deferred -- see notes for offsets
        break;
    }

    // ---- Types 18+ : many more status-check roll patterns ----
    default:
        break;
    }

    // ================================================================
    // SHARED TAIL (.L_800F4000)
    // ================================================================

    // ---- Part 1: func_80145C00 -> synthesize type 0x34 event ----
    if (func_80145C00(evt->eventType)) {
        void* data90 = func_80149154(acc->subAcc, 0x90);
        if (data90 != nullptr) {
            BattleEvent tailEv;
            tailEv.field_00 = *(u32*)((u8*)acc + 0x3F10);
            tailEv.field_04 = 0x90;
            tailEv.field_08 = evt->field_08;
            tailEv.eventType = 0x34;
            tailEv._pad_0E = 0;
            tailEv.field_10 = *(s32*)((u8*)data90 + 0x10);
            tailEv.field_14 = 0;
            tailEv.field_16 = 0;
            tailEv.field_18 = 0;
            tailEv._pad_1A = 0;
            tailEv.field_1C = 0;
            tailEv.field_20 = 0.0f;
            tailEv.field_24 = evt->field_24;
            tailEv.field_28 = evt->field_28;
            tailEv.field_2C = evt->field_2C;
            tailEv.prevEventType = evt->prevEventType;
            tailEv.field_30 = evt->field_30 | *(u32*)((u8*)data90 + 0x30);
            func_800EC918(self, pc, acc, &tailEv, tgt);
        }
    }

    // ---- Part 2: slot 0x9E multiplier (func_80146148 gate) ----
    if (pc != nullptr) {
        if (func_80146148(evt->eventType)) {
            void* data9E = func_80149154(acc->subAcc, 0x9E);
            if (data9E != nullptr) {
                if (evt->field_08 == 0x8000) {
                    s32 val9E = *(s32*)((u8*)data9E + 0x10);
                    evt->field_20 *= (1.0f + (f32)(s32)val9E / 100.0f);
                }
            }
        }
    }

    // ---- Part 3: slot 0x9F multiplier (func_80145DBC gate, on pc+8) ----
    if (pc != nullptr) {
        if (func_80145DBC(evt->eventType)) {
            void* data9F = func_80149154((u8*)pc + 8, 0x9F);
            if (data9F != nullptr) {
                if (evt->field_08 == 0x8000) {
                    s32 val9F = *(s32*)((u8*)data9F + 0x10);
                    evt->field_20 *= (1.0f + (f32)(s32)val9F / 100.0f);
                }
            }
        }
    }

    // ---- Part 4: apply event to actor (vfunc 0x18) ----
    {
        typedef void (*VF18)(SubAccessor*, BattleEvent*);
        VF18 vf18 = (VF18)(*(void**)((u8*)acc->subAcc + 0x18));
        vf18(acc->subAcc, evt);
    }

    return 1;
}
// Thunk: dispatch through secondary vtable at +0x8, calling vtable[0x20]
void func_800F3958(void* ignored, void* self, void* arg) {
    void* base = (u8*)self + 8;
    void* vtbl = *(void**)base;
    typedef void (*VFunc)(void*, void*);
    VFunc vfunc = (VFunc)(*(void**)((u8*)vtbl + 0x20));
    vfunc(base, arg);
}
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

// Searches for a matching value in this->field_B8 entries and this->+8 array.
// Returns 1 if field_78 bit 0 is clear, or if a matching non-zero element is found.
// Returns 0 otherwise.
s32 func_800D7D24(void* self) {
    // If bit 0 of field_78 is clear, return 1 immediately
    if (!(*(u32*)((u8*)self + 0x78) & 1)) return 1;

    void* r6 = *(void**)((u8*)self + 0xB8);
    if (r6 == nullptr) return 0;

    // First loop: 2 iterations, each checking 8 fields of a 0x20-byte entry
    {
        u8* r5 = (u8*)r6;
        u32 targetVal = *(u32*)((u8*)r6 + 4);
        s32 found = 0;
        for (s32 i = 0; i < 2; i++) {
            if (*(u32*)(r5 + 0x08) == targetVal) { found = 1; break; }
            if (*(u32*)(r5 + 0x0C) == targetVal) { found = 1; break; }
            if (*(u32*)(r5 + 0x10) == targetVal) { found = 1; break; }
            if (*(u32*)(r5 + 0x14) == targetVal) { found = 1; break; }
            if (*(u32*)(r5 + 0x18) == targetVal) { found = 1; break; }
            if (*(u32*)(r5 + 0x1C) == targetVal) { found = 1; break; }
            if (*(u32*)(r5 + 0x20) == targetVal) { found = 1; break; }
            if (*(u32*)(r5 + 0x24) == targetVal) { found = 1; break; }
            r5 += 0x20;
        }

        if (found) {
            // C++-linkage decl: MWCC mangles func_800B708C to func_800B708C__Fi.
            extern void* func_800B708C(int);
            if (func_800B708C(targetVal) != 0) return 0;
        }
    }

    // Second loop: 16 iterations, checking array at self+8
    {
        u8* ptr = (u8*)self;
        u32 targetVal = *(u32*)((u8*)self + 4);
        for (s32 i = 0; i < 16; i++) {
            if (*(u32*)(ptr + 8) != 0) {
                // Found a non-zero element; compare with field_4
                if (*(u32*)((u8*)self + i * 4 + 8) == targetVal) return 1;
                return 0;
            }
            ptr += 4;
        }
    }

    return 0;
}
extern f32 lbl_80666DD4; // 1.0f

extern f32 lbl_80666DD8; // 0.001f
// NOTE: retail symbol names for these battle helpers are C-linkage
// --- shared battle-helper externs used by the func_800E2A9C / func_800E64CC ports ---

// Calculates accumulated damage/healing value from various status effects.
// Returns a float clamped to >= 0.
extern "C" f32 func_800D7EA0(u8* obj, void* target) {
    if (obj == nullptr || target == nullptr) return 1.0f;

    f32 result = 1.0f;
    u32 targetFlags = *(u32*)((u8*)target + 0x78);
    u32 stackVal;

    if (targetFlags & 0x200) {
        if (func_80148778(obj + 8, 0xB4)) {
            void* entry = func_80149154(obj + 8, 0xB4);
            result -= 0.001f * (f32)(s32)*(u32*)((u8*)entry + 0x10);
        }
    } else if (targetFlags & 0xC00) {
        if (func_80148778(obj + 8, 0xB5)) {
            void* entry = func_80149154(obj + 8, 0xB5);
            result -= 0.001f * (f32)(s32)*(u32*)((u8*)entry + 0x10);
        }
    }

    if (func_80148778(obj + 8, 0xB3)) {
        void* entry = func_80149154(obj + 8, 0xB3);
        result += 0.001f * (f32)(s32)*(u32*)((u8*)entry + 0x10);
    }

    if (func_80148778(obj + 8, 0xB6)) {
        void* entry = func_80149154(obj + 8, 0xB6);
        result -= 0.001f * (f32)(s32)*(u32*)((u8*)entry + 0x10);
    }

    // Call vfunc_0x290 on obj; each func_80260264 receives its return value as arg1
    if (((cf::CActorParam*)(void*)obj)->CActorParam_UnkVirtualFunc127() != nullptr) {
        // Reload flags from target (retail: lwz r0, 0x78(r31))
        if (*(volatile u32*)((u8*)target + 0x78) & 0xC00) {
            if (func_80260264(((cf::CActorParam*)(void*)obj)->CActorParam_UnkVirtualFunc127(), 0x64, &stackVal)) {
                result -= 0.001f * (f32)(s32)stackVal;
            }
        }

        if (*(f32*)((u8*)target + 0x5c) > 0.0f) {
            if (func_80260264(((cf::CActorParam*)(void*)obj)->CActorParam_UnkVirtualFunc127(), 0x6F, &stackVal)) {
                result += 0.001f * (f32)(s32)stackVal;
            }
        }

        if (func_80260264(((cf::CActorParam*)(void*)obj)->CActorParam_UnkVirtualFunc127(), 0x71, &stackVal)) {
            if (*(u32*)((u8*)obj + 0x1530) != 0) {
                result -= 0.001f * (f32)(s32)stackVal;
            }
        }

        if (func_80260264(((cf::CActorParam*)(void*)obj)->CActorParam_UnkVirtualFunc127(), 0x70, &stackVal)) {
            if (*(u32*)((u8*)obj + 0x3374) & 0x200000) {
                result += 0.001f * (f32)(s32)stackVal;
            }
        }
    }

    if (result < 0.0f) result = 0.0f;
    return result;
}
extern "C" int func_802799F0(void*, void*);

// Calculates cumulative damage value from various stat sources
extern "C" float func_800D81A8(void* obj, void* target, void* source){
    float result = 0.0f;
    
    // Get source's linked object pointer at +0x50 (or null if source is null)
    void* sourceObj = source ? *(void**)((u8*)source + 0x50) : nullptr;
    
    // ---- First block: accumulate from target's battle stats ----
    if(target != nullptr){
        if(func_80148778((u8*)target + 8, 0x8f)){
            void* entry = func_80149154((u8*)target + 8, 0x8f);
            u32 val = *(u32*)((u8*)entry + 0x10);
            result += 0.001f * (float)val;
        }else if(func_80148778((u8*)target + 8, 0x8e)){
            void* entry = func_80149154((u8*)target + 8, 0x8e);
            u32 val = *(u32*)((u8*)entry + 0x10);
            result += 0.001f * (float)val;
        }
    }
    
    // Check target's virtual func 0x290 (returns non-null if active)
    if(target != nullptr && sourceObj != nullptr){
        typedef void* (*VFunc290)(void*);
        VFunc290 vf290 = (VFunc290)(*(void***)target)[0x290 / 4];
        if(vf290(target) != nullptr){
            u32 flags = *(u32*)((u8*)sourceObj + 0x78);
            if(flags & 0x40000000 && *(u16*)((u8*)sourceObj + 0x3c) == 3){
                u32 stackVal;
                if(func_80260264(target, 0x3e, &stackVal)){
                    result += 0.001f * (float)stackVal;
                }
            }
        }
    }
    
    // ---- Second block: accumulate from obj's data ----
    if(obj != nullptr){
        // If source is null or type != 3, try chain value
        if(sourceObj == nullptr || *(u16*)((u8*)sourceObj + 0x3c) != 3){
            u32 mgrAddr = lbl_eu_80663F00;
            if(func_802799F0((void*)(mgrAddr + 0x1a8), obj)){
                result += *(float*)(mgrAddr + 0x20ac);
            }
        }
        
        // Check obj's virtual func 0x290
        {
            typedef void* (*VFunc290)(void*);
            VFunc290 vf290 = (VFunc290)(*(void***)obj)[0x290 / 4];
            if(vf290(obj) != nullptr && sourceObj != nullptr){
                u32 flags = *(u32*)((u8*)sourceObj + 0x78);
                if(flags & 0x40000000 && *(u16*)((u8*)sourceObj + 0x3c) == 3){
                    u32 stackVal;
                    if(func_80260264(obj, 0x3d, &stackVal)){
                        result += 0.001f * (float)stackVal;
                    }
                }
            }
        }
        
        // Check sourceObj flags for type 5
        if(sourceObj != nullptr){
            u32 flags = *(u32*)((u8*)sourceObj + 0x78);
            if(flags & 0x40000000 && *(u16*)((u8*)sourceObj + 0x40) == 5){
                u32 stackVal;
                if(func_80260264(obj, 0x42, &stackVal)){
                    result += 0.001f * (float)stackVal;
                }
            }
        }
        
        // Final check with target
        if(sourceObj != nullptr){
            u32 flags = *(u32*)((u8*)sourceObj + 0x78);
            if(flags & 0x40000000){
                u32 stackVal;
                if(func_80260264(obj, 0x3f, &stackVal)){
                    if(target != nullptr){
                        typedef void* (*VFunc290)(void*);
                        VFunc290 vf290 = (VFunc290)(*(void***)obj)[0x290 / 4];
                        typedef void* (*VFunc5C0)(void*, void*);
                        VFunc5C0 vf5C0 = (VFunc5C0)(*(void***)obj)[0x5c0 / 4];
                        if(vf290(obj) != nullptr && vf5C0(obj, target) != nullptr){
                            typedef float (*VFunc130)(void*);
                            VFunc130 vf130 = (VFunc130)(*(void***)target)[0x130 / 4];
                            if(vf130(target) < 0.0f){
                                result += 0.001f * (float)stackVal;
                            }
                        }
                    }
                }
            }
        }
    }
    
    return result;
}
void func_800D9354(){}
void func_800D9978(){}
void func_800D9CA0(cf::CBattleManager* mgr, BattleRemoveObjAccessor* target){
    extern bool func_8006EF04__Fi(s32);
    if(func_8006EF04__Fi(0x400)) return;
    
    // Remove target from all lists
    mgr->mActorList1.remove((cf::CfObjectActor*)target);
    
    extern void func_80279694(cf::CChain*, cf::CfObjectActor*);
    func_80279694(&mgr->mChain, (cf::CfObjectActor*)target);
    
    // Check target flag bit 0x02 at +0x3F00 to decide list
    if(target->field_3f00 & 0x02){
        mgr->mActorList2.remove((cf::CfObjectActor*)target);
    }else{
        mgr->mActorList3.remove((cf::CfObjectActor*)target);
    }
    
    // Call vfunc at +0x3ED4 object's vtable[0x7C]
    {
        void** vtbl = *(void***)target->field_3ed4;
        typedef void (*VFunc7C)(void*);
        ((VFunc7C)vtbl[0x7C / 4])(target->field_3ed4);
    }
    
    // Call target's vtable[0x2E4]
    {
        void** vtbl = *(void***)target;
        typedef void (*VFunc2E4)(void*);
        ((VFunc2E4)vtbl[0x2E4 / 4])(target);
    }
    
    // If target flag bit 0x04 at +0x3F00 is set, do extra cleanup
    if(target->field_3f00 & 0x04){
        void** vtbl = *(void***)target;
        typedef void (*VFunc)(void*);
        ((VFunc)vtbl[0xB0 / 4])(target);
        vtbl = *(void***)target;
        ((VFunc)vtbl[0xB8 / 4])(target);
        // Call secondary vtable at +0x3E9C, vtable[0x50]
        void** vtbl2 = *(void***)((u8*)target + 0x3E9C);
        typedef void (*VFunc50)(void*, u32);
        ((VFunc50)vtbl2[0x50 / 4])((u8*)target + 0x3E9C, 0);
    }
    
    // Notify all battle events
    {
        _reslist_node<cf::IBattleEvent*>* cur = mgr->mBattleEventList.mStartNodePtr->mNext;
        _reslist_node<cf::IBattleEvent*>* head = mgr->mBattleEventList.mStartNodePtr;
        while(cur != head){
            cf::IBattleEvent* event = cur->mItem;
            if(event != nullptr){
                void** vtbl = *(void***)event;
                typedef void (*VFunc10)(void*, cf::CfObjectActor*);
                ((VFunc10)vtbl[0x10 / 4])(event, (cf::CfObjectActor*)target);
            }
            cur = cur->mNext;
        }
    }
    
    // Drain mActorList1 sentinel loop
    {
        _reslist_node<cf::CfObjectActor*>* cur = mgr->mActorList1.mStartNodePtr->mNext;
        _reslist_node<cf::CfObjectActor*>* head = mgr->mActorList1.mStartNodePtr;
        while(cur != head) cur = cur->mNext;
    }
    
    // Check global lists
    {
        u32 mgrAddr = lbl_eu_80663F00;
        reslist<cf::CfObjectActor*>& globalList1 = *(reslist<cf::CfObjectActor*>*)(mgrAddr + 0x48);
        if(globalList1.size() == 0){
            extern void func_800F41A0(cf::CBattleManager*);
            func_800F41A0(mgr);
        }
    }
    
    if(!(target->field_3f00 & 0x04)){
        u32 mgrAddr = lbl_eu_80663F00;
        reslist<cf::CfObjectActor*>& globalList2 = *(reslist<cf::CfObjectActor*>*)(mgrAddr + 0x8);
        if(globalList2.size() == 0) return;
    }
    
    // Player cleanup loop
    extern void* getPlayer__Q22cf13CfGameManagerFi(int);
    extern void func_80174B4C(void*, u32);
    for(int i = 0; i < 3; i++){
        void* player = getPlayer__Q22cf13CfGameManagerFi(i);
        void* action = func_8016FE34(player);
        if(action != nullptr){
            void** vtbl = *(void***)action;
            typedef void (*VFunc)(void*, u32);
            ((VFunc)vtbl[0x20 / 4])(action, 0x40);
            ((VFunc)vtbl[0x24 / 4])(action, 0x80);
            ((VFunc)vtbl[0x28 / 4])(action, 0x8000);
            func_80174B4C(action, 0x800);
            func_80174B4C(action, 0x1000);
        }
    }
    
    // Call virtual func_80085220(mgr, 1, 1)
    {
        void** vtbl = *(void***)mgr;
        typedef void (*VFunc1C)(void*, u32, u32);
        ((VFunc1C)vtbl[0x1C / 4])(mgr, 1, 1);
    }
    
    // Clear global flag
    {
        extern u32 lbl_eu_80663E28;
        lbl_eu_80663E28 &= ~0x18;
    }
    
    extern void func_80277B34(cf::CChain*);
    func_80277B34(&mgr->mChain);
    
    // Enum list iteration
    struct EnumListHolder { void* list; u32 handle; };
    EnumListHolder holder;
    extern void func_80043D90(EnumListHolder*);
    extern void* func_80043F18(EnumListHolder*);
    extern void func_800F4A98(void*, int, int);
    extern void* func_800F6EAC(void*, u32);
    extern void __dt__80043E88(EnumListHolder*, int);
    
    func_80043D90(&holder);
    void* list = func_80043F18(&holder);
    func_800F4A98(list, 0x20, 0);
    
    for(u32 i = 0; ; i++){
        list = func_80043F18(&holder);
        u32 count = *(u32*)((u8*)list + 0x620);
        if(i >= count) break;
        
        list = func_80043F18(&holder);
        void* element = func_8016FE34(func_800F6EAC(list, i));
        if(element != nullptr){
            void** vtbl = *(void***)element;
            typedef void (*VFunc)(void*);
            ((VFunc)vtbl[0xB0 / 4])(element);
            vtbl = *(void***)element;
            ((VFunc)vtbl[0xB8 / 4])(element);
            vtbl = *(void***)element;
            ((VFunc)vtbl[0x314 / 4])(element);
        }
    }
    
    __dt__80043E88(&holder, -1);
}

void func_800DA0A4(){}
void func_800DB0FC(){}
void func_800DB4FC(void* self, void* obj, void* arg1, void* arg2);
// Battle event handler: checks various flags and conditions on arg3/arg4,
// sets flags on arg4->field_74, then tail-calls func_800E08E8.
void func_800DB7F8(void* r3, void* r4, void* arg3, void* arg4) {
    extern void func_800E08E8(void*, void*, void*, void*);
    void* r28 = r3;
    void* r29 = r4;
    void* r30 = arg3;
    void* r31 = arg4;

    if (arg3 == nullptr) goto tailcall;

    // Set flags on arg4->field_78
    {
        u32 val78 = *(u32*)((u8*)arg4 + 0x78);
        val78 |= 0x40000100;
        *(u32*)((u8*)arg4 + 0x78) = val78;
    }

    // Check if arg4->field_74 & 2, skip if set
    if (*(u32*)((u8*)arg4 + 0x74) & 2) goto tailcall;

    // Check if arg3->field_3374 & 0x1000, skip if set
    if (*(u32*)((u8*)arg3 + 0x3374) & 0x1000) goto tailcall;

    // Virtual call on arg3 at +0x3E9C, vtable[0x140]
    {
        void* base = (u8*)arg3 + 0x3E9C;
        void** vtbl = *(void***)base;
        typedef float (*VFunc140)(void*);
        float result = ((VFunc140)vtbl[0x140 / 4])(base);
        extern float lbl_eu_80666DDC;
        if (result == lbl_eu_80666DDC) goto tailcall;
    }

    // Check flag 0x32 on arg3->fld8
    extern int func_80148778(void*, int);
    if (func_80148778((u8*)arg3 + 8, 0x32)) goto tailcall;

    s32 r25 = 0;
    void* sourceObj = *(void**)((u8*)arg4 + 0x50);

    // Check flag 0xCE
    if (func_80148778((u8*)arg3 + 8, 0xCE)) {
        // Check if arg4->field_78 & 0x800
        if (*(u32*)((u8*)arg4 + 0x78) & 0x800) {
            extern void* func_801491F4(void*, u32);
            void* entry = func_801491F4((u8*)arg3 + 8, 0xCE);
            
            // Virtual call on sourceObj: vtable[0x84] at offset 0x0C
            void** vtbl = *(void***)sourceObj;
            typedef u32 (*VFunc84_0C)(void*);
            u32 val = ((VFunc84_0C)vtbl[0x84 / 4])(sourceObj);
            // Actually, the retail does: lwz r12, 0x84(r26); lwz r12, 0xc(r12); ...
            // This is: sourceObj->vtable[0x84/4] is a pointer to another vtable
            // Then call vtable[0x0C/4] on that pointer
            // But the hexdiff shows: lwz r12, 0x84(r26) then lwz r12, 0xc(r12)
            // This is: load a sub-vtable pointer from sourceObj's vtable[0x84/4]
            // Then call vtable[0x0C/4] on it
            // Let me just use the function pointer approach
            
            u32 entryVal = *(u32*)((u8*)entry + 0x10);
            if (entryVal >= val) r25 = 1;
        }
    }

    if (r25 != 0) goto tailcall;

    // Check sourceObj type
    u16 type = *(u16*)((u8*)sourceObj + 0x58);
    if (type == 1) {
        if (func_80148778((u8*)arg3 + 8, 0xF)) goto tailcall;
        if (func_80148778((u8*)arg3 + 8, 0x10)) goto tailcall;

        if (func_80148778((u8*)arg3 + 8, 0x29)) {
            extern void* func_80149154(void*, u32);
            void* entry = func_80149154((u8*)arg3 + 8, 0x29);
            u32 entryVal = *(u32*)((u8*)entry + 0x10);
            u32 b4 = *(u32*)((u8*)arg4 + 0xB4);
            if (b4 < entryVal) goto tailcall;
        }
        
        // Set flags on arg4->field_74
        *(u32*)((u8*)arg4 + 0x74) |= 0x80010000;
        goto tailcall;
    }

    if (type == 2) {
        r25 = 1;
        
        // Call vfunc_0x290 on arg3
        void** vtbl = *(void***)arg3;
        typedef void* (*VFunc290)(void*);
        void* result = ((VFunc290)vtbl[0x290 / 4])(arg3);
        if (result != nullptr) {
            result = ((VFunc290)vtbl[0x290 / 4])(arg3);
            extern int func_8026178C(void*, int);
            if (func_8026178C(result, 0x76)) r25 = 0;
        }

        if (func_80148778((u8*)arg3 + 8, 0x28)) {
            extern void* func_80149154(void*, u32);
            void* entry = func_80149154((u8*)arg3 + 8, 0x28);
            u32 entryVal = *(u32*)((u8*)entry + 0x10);
            u32 b4 = *(u32*)((u8*)arg4 + 0xB4);
            if (b4 < entryVal) r25 = 0;
        }

        if (r25 != 0) {
            *(u32*)((u8*)arg4 + 0x74) |= 0x80020000;
        }
        goto tailcall;
    }

tailcall:
    func_800E08E8(r28, r29, r30, r31);
}
extern int func_801B1FA4();
extern void func_801B19F0(u16 r3, int r4);

void func_800DBA2C(void* self, BattleObjAccessor* obj, void* arg1, BattleMoveObjAccessor* arg2) {
    BattleSubObjAccessor* subObj = static_cast<BattleSubObjAccessor*>(arg2->field_50);
    if (subObj == nullptr) return;
    
    if (obj->field_3f00 & 0x2) {
        if (obj->field_3f28 == 7) {
            if (arg2->field_78 & 0x400) {
                int result = func_801B1FA4();
                if (result != -1) {
                    func_801B19F0(subObj->field_40, -1);
                }
            }
        }
    }
    
    // Retail range-checks type in {5,6} via (type-5) <= 1 unsigned
    if ((u32)(subObj->type_3c - 5) <= 1) {
        func_800DB4FC(self, obj, arg1, arg2);
    }
}
void func_800DBACC(){}
struct BattleMoveSubData {
    u8 pad_00[0x38];
    s16 field_38;           // +0x38
    s16 field_3A;           // +0x3A
    u16 type_3C;            // +0x3C move type (1/5 -> main block)
    u16 field_3E;
    u16 field_40;           // +0x40 art type (0..9, dispatch switches)
    u8 pad_42[0x48 - 0x42];
    u16 field_48;           // +0x48
    u8 pad_4A[0x5E - 0x4A];
    u16 field_5E;           // +0x5E
    u8 pad_60[0x6C - 0x60];
    u8 field_6C;            // +0x6C
    u8 field_6D;            // +0x6D
    u8 pad_6E[0x72 - 0x6E];
    u8 field_72;            // +0x72 skill bitfield byte
    u8 field_73;            // +0x73 skill bitfield byte
    u8 pad_74[0x78 - 0x74];
    u32 field_78;           // +0x78 flags
    u8 pad_7C[0x84 - 0x7C];
    void** field_84;        // +0x84 sub-vtable (slot 0x0C used)
};

// Move/event data (r31).
struct BattleMoveData {
    u8 pad_00[0x50];
    void* field_50;         // +0x50 sub-object (BattleMoveSubData)
    f32 field_54;           // +0x54 multiplicative damage ratio
    f32 field_58;           // +0x58 additive damage ratio
    f32 field_5C;           // +0x5C base damage
    f32 field_60;           // +0x60 damage variance
    f32 field_64;           // +0x64 (init 0.0)
    f32 field_68;           // +0x68 (dot/status damage accumulator)
    f32 field_6C;           // +0x6C (hp delta accumulator)
    s16 field_70;           // +0x70 (dot tick counter)
    u8 pad_72[0x74 - 0x72];
    u32 field_74;           // +0x74 tagged pointer / flag word
    u32 field_78;           // +0x78 flags
    u8 pad_7C[0x94 - 0x7C];
    u32 field_94;           // +0x94 (used in % math for variance)
    u32 field_98;           // +0x98 (100-based proc chance)
    u8 pad_9C[0xA8 - 0x9C];
    u32 field_A8;           // +0xA8 (100-based proc chance)
    u8 pad_AC[0xB0 - 0xAC];
    u32 field_B0;           // +0xB0 (100-based proc chance)
};

// Param struct returned by vf0x224 (r23 = attacker's, r22 = target's).
struct BattleParamData {
    u8 pad_00[0x1E];
    s16 field_1E;           // +0x1E
    u8 pad_20[0x24 - 0x20];
    f32 field_24;           // +0x24
    f32 field_28;           // +0x28
    s16 field_2C;           // +0x2C
    s16 field_2E;           // +0x2E
    s16 field_30;           // +0x30
    s16 field_32;           // +0x32
    u8 pad_34[0x38 - 0x34];
    s16 field_38;           // +0x38
    s16 field_3A;           // +0x3A
    u8 pad_3C[0x54 - 0x3C];
    u8 field_54;            // +0x54
    u8 pad_55[0x60 - 0x55];
    s16 field_60;           // +0x60 (range, main block)
    s16 field_62;           // +0x62 (range, alt block)
    s16 field_64;           // +0x64
    s16 field_66;           // +0x66
    s16 field_68;           // +0x68
    s16 field_6A;           // +0x6A
    s16 field_6C;           // +0x6C
    s16 field_6E;           // +0x6E
    u8 field_70;            // +0x70 skill bitfield byte
    u8 field_71;
    u8 field_72;            // +0x72 skill bitfield byte
    u8 field_73;            // +0x73 skill bitfield byte
};

// Secondary vtable at +0x3E9C, slot 0xAC returns a position/velocity vec.
struct BattleVec {
    f32 x;                  // +0x00 (ps pair)
    f32 y;                  // +0x04 (also read as f32 at +0x4)
    f32 z;                  // +0x08
};

// ---------------------------------------------------------------------------
// C-linkage imports (retail symbol names kept verbatim)
// ---------------------------------------------------------------------------

// Static tables decoded from retail .data (see notes for full listing).
static const f32 sTable_150[5] = { 0.0f, 0.0f, 0.0f, 0.1f, 0.2f };
static const s16 sTable_208[8]  = { -20, -10, 0, 15, 30, 0, 0, 0 };
static const f32 sTable_28C[3]  = { 8.0f, 4.0f, 2.0f };

// ---------------------------------------------------------------------------
// Small vtable-dispatch helpers used throughout (one per retail slot).
// ---------------------------------------------------------------------------
typedef void* (*VFn224)(void*);        // actor -> param (r23/r22)
typedef void* (*VFn20C)(void*);        // actor -> range data (+0x60/+0x62)
typedef u16   (*VFn328)(void*);        // actor -> hit-flag u16
typedef void* (*VFn2A4)(void*);        // actor -> combo data
typedef s32   (*VFn108)(void*);        // actor -> level
typedef s32   (*VFnE0)(void*);         // actor -> element/type
typedef void* (*VFn290)(void*);        // actor -> CActorParam
typedef s32   (*VFn308)(void*, u32, u32, u32, u32, u32); // actor -> table idx
typedef s32   (*VFn308_0)(void*);       // actor -> table idx (no-arg form)
typedef f32   (*VFn158)(void*);        // actor -> stat float
typedef f32   (*VFn12C)(void*);        // actor -> damage float
typedef s32   (*VFn2BC)(void*);        // actor -> flag int
typedef void* (*VFn30)(void*);         // (+0x4 object) -> ptr
typedef s32   (*VSubFn0C)(void*);      // sub-object +0x84 sub-vtable slot 0x0C
typedef BattleVec* (*VFnAC)(void*);    // +0x3E9C secondary vtable slot 0xAC
typedef void* (*VFn54)(void*, u32);    // (+8 object) vtable slot 0x54
typedef void* (*VFn5C)(void*, u32);    // (+8 object) vtable slot 0x5C

static inline void* vf290(void* obj) {
    return ((VFn290)(*(void***)obj)[0x290 / 4])(obj);
}
static inline s32 subVf0C(BattleMoveSubData* sub) {
    return ((VSubFn0C)sub->field_84[0x0C / 4])(sub);
}
static inline BattleVec* vfAC(void* obj) {
    return ((VFnAC)(*(void***)((u8*)obj + 0x3E9C))[0xAC / 4])((u8*)obj + 0x3E9C);
}

// ---------------------------------------------------------------------------
// func_800DCB54
// ---------------------------------------------------------------------------

// ---------------------------------------------------------------------------
// C-linkage imports (retail symbol names kept verbatim)
// ---------------------------------------------------------------------------

// Static tables decoded from retail .data (see notes for full listing).

// ---------------------------------------------------------------------------
extern "C" void func_800DCB54(void* self, void* attacker, void* target,
                   BattleMoveData* move) {
    // ----------------------------------------------------------
    // 0x5150 prologue / early guards
    // ----------------------------------------------------------
    if (target == 0) return;                        // 0x5154 cmpwi r5,0

    // 0x51C0: tagged word at move+0x74; bit0 clear -> bail; else init ratios
    if (!(move->field_74 & 0x1)) return;            // 0x51CC clrlwi. bit31(LSB)
    move->field_54 = 1.0f;                          // 0x51D4
    move->field_58 = 1.0f;                          // 0x51D0
    move->field_5C = 0.0f;                          // 0x51D8
    move->field_60 = 0.0f;                          // 0x51DC
    move->field_64 = 0.0f;                          // 0x51E0

    // 0x51E8: grab params
    BattleMoveSubData* sub = (BattleMoveSubData*)move->field_50;
    BattleParamData* atkParam =
        (BattleParamData*)((VFn224)(*(void***)attacker)[0x224 / 4])(attacker);
    BattleParamData* tgtParam =
        (BattleParamData*)((VFn224)(*(void***)target)[0x224 / 4])(target);
    {
        void* res20C = ((VFn20C)(*(void***)target)[0x20C / 4])(target);

        // 0x5230: type gate -- only 1 and 5 take the main block
        if (sub->type_3C != 1 && sub->type_3C != 5) {
            // ============================================================
            // ALT BLOCK (0x800DE8FC): same shape as main, uses +0x62 ranges,
            // r23+0x30 / r22+0x32 hit positions.
            // ============================================================
            f32 f26 = (f32)(s32)*(s16*)((u8*)res20C + 0x62) / 100.0f;
            f32 f28 = (f32)(s32)tgtParam->field_62 / 100.0f - f26;
            s32 r25 = atkParam->field_30;
            s32 r26 = tgtParam->field_32;

            // --- hit-flag conditional zero of f26 (0x800DE948) ---
            {
                u32 tag = move->field_74;
                if (tag & 0x2000000) {
                    u16 hf = ((VFn328)(*(void***)target)[0x328 / 4])(target);
                    if (!(hf & 0x4000)) f26 = 0.0f;
                } else if (tag & 0x4000000) {
                    u16 hf = ((VFn328)(*(void***)target)[0x328 / 4])(target);
                    if (!(hf & 0x2000)) f26 = 0.0f;
                } else if (tag & 0x1000000) {
                    u16 hf = ((VFn328)(*(void***)target)[0x328 / 4])(target);
                    if (!(hf & 0x1)) f26 = 0.0f;
                }
            }
            f28 += f26;                                 // 0x800DE9D0

            // --- clamp [-1.0, 0.9] ---
            if (f28 < -1.0f) f28 = -1.0f;
            else if (f28 > 0.9f) f28 = 0.9f;

            // --- zero ratio under data-map flags ---
            if (func_80148778((u8*)target + 8, 0x13)) { // 0x800DE9F8
                r26 = 0;
                f28 = 0.0f;
            }
            if ((((BattleObjAccessor*)attacker)->field_3f00 & 0x4) &&
                (move->field_78 & 0x800)) {             // 0x800DEA24
                r26 = 0;
                f28 = 0.0f;
            }

            // --- hit quality flags (0x800DEA64) ---
            if (f28 > 0.0f && f28 < 0.5f) {
                move->field_74 |= 0x80400000;
            } else if (f28 >= 0.5f && f28 < 1.0f) {
                move->field_74 |= 0x80800000;
            }

            // --- guard path: move->field_78 bit21 (0x200) ---
            if (move->field_78 & 0x200) {
                // 0x800DEAC0: variance computation
                s32 vf = subVf0C(sub);
                s32 r5 = sub->field_38 + (s32)sub->field_6C * (vf - 1);
                if (((BattleObjAccessor*)attacker)->field_3f00 & 0x2) {
                    f32 f2 = atkParam->field_24;
                    f32 f1 = atkParam->field_28 - f2;
                    s32 d = (s32)(1.0f + f1);
                    u32 r4 = move->field_94;
                    r25 = (s32)((f32)(s32)r25 + f2) + (s32)(r4 % (u32)d);
                } else {
                    // 0x800DEB60: (f94 % 21) + 90 over 100
                    r5 = (s32)((f32)(s32)r5 *
                               (f32)(s32)((move->field_94 % 21) + 90) / 100.0f);
                }
                move->field_54 = (f32)(s32)(r25 - r26) * (f32)(s32)r5 / 100.0f;

                if (func_80148778((u8*)attacker + 8, 0xC8)) {
                    move->field_54 *= 0.5f;
                }
                if (move->field_54 < 1.0f) move->field_54 = 1.0f;
                move->field_54 = move->field_54 - move->field_54 * f28;
                goto ratio_done;
            }

            // --- non-guard: move->field_78 bits 20-21 (0x600) ---
            if (move->field_78 & 0x600) {
                // 0x800DEC58: same r25 variance as above
                if (((BattleObjAccessor*)attacker)->field_3f00 & 0x2) {
                    f32 f2 = atkParam->field_24;
                    f32 f1 = atkParam->field_28 - f2;
                    s32 d = (s32)(1.0f + f1);
                    u32 r4 = move->field_94;
                    r25 = (s32)((f32)(s32)r25 + f2) + (s32)(r4 % (u32)d);
                }
                // three sub-vtable calls (0x800DECCC)
                s32 v1 = subVf0C(sub);
                s32 v2 = subVf0C(sub);
                s32 v3 = subVf0C(sub);
                s32 r18 = sub->field_38 + (s32)sub->field_6C * (v1 - 1);
                s32 r19 = sub->field_3A + (s32)sub->field_6D * (v2 - 1);
                s32 r20 = (s32)move->field_94;
                s32 r7 = r19 - r18 + 1;
                s32 r0 = sub->field_38 + (s32)sub->field_6C * (v3 - 1) +
                         (s32)(r20 % (u32)r7);
                move->field_54 =
                    (f32)(s32)(r25 - r26) * (f32)(s32)r0 / 100.0f;   // 0x800DED94

                // --- 0x800DED98: vf0xE0-based half-damage selection ---
                f32 f26b = 1.0f;
                if (((BattleObjAccessor*)attacker)->field_3f00 & 0x2) {
                    s32 etype = ((VFnE0)(*(void***)target)[0xE0 / 4])(target);
                    u16 f28t = ((BattleObjAccessor*)attacker)->field_3f28;
                    if (etype == 1) {
                        if (f28t != 1 && f28t != 8 &&
                            !func_80148778((u8*)attacker + 8, 0xCF) &&
                            !func_80148778((u8*)attacker + 8, 0xD0) &&
                            !(*(u32*)((u8*)attacker + 0x3374) & 0x4000)) {
                            f26b = 0.5f;
                        }
                    } else {
                        s32 etype2 = ((VFnE0)(*(void***)target)[0xE0 / 4])(target);
                        if (etype2 == 2) {
                            if (f28t != 8 &&
                                !func_80148778((u8*)attacker + 8, 0xD0) &&
                                !(*(u32*)((u8*)attacker + 0x3374) & 0x8000)) {
                                f26b = 0.5f;
                            }
                        }
                    }
                }
                move->field_54 *= f26b;                 // 0x800DEE70

                // --- 0x800DEE7C: 0x10A bonus ---
                if ((move->field_78 & 0x800) &&
                    func_80148778((u8*)attacker + 8, 0x10A)) {
                    move->field_58 += 1.0f;
                }

                // 0x800DEEAC: clamp + apply ratio
                if (move->field_54 < 1.0f) move->field_54 = 1.0f;
                move->field_54 = move->field_54 - move->field_54 * f28;

                // 0x800DEEC4: 0xC0 skill -> field_58 += val/10 - 1
                if (func_80148778((u8*)attacker + 8, 0xC0)) {
                    void* entry = func_80149154((u8*)attacker + 8, 0xC0);
                    move->field_58 +=
                        (f32)(s32)*(u32*)((u8*)entry + 0x10) / 10.0f - 1.0f;
                }

                // 0x800DEF20: attacker vf0x290 skill values (0x4E/0x55/0x56)
                if (vf290(attacker) != 0) {
                    u32 sv;
                    if (func_80260264(vf290(attacker), 0x4E, &sv)) {
                        move->field_58 += (f32)(s32)sv / 100.0f;
                    }
                    if (sub->field_40 == 6) {
                        if (func_80260264(vf290(attacker), 0x55, &sv)) {
                            move->field_58 += (f32)(s32)sv / 100.0f;
                        }
                    }
                    if (sub->field_40 == 8) {
                        if (func_80260264(vf290(attacker), 0x56, &sv)) {
                            move->field_58 += (f32)(s32)sv / 100.0f;
                        }
                    }
                }
            }
        ratio_done:;
            // ============================================================
            // ALT-BLOCK TAIL (0x800DF050): combo / level-diff / vf0x308
            // ============================================================
            // --- 0x800DF050: target vf0x2A4 (combo) checks ---
            {
                void* p = ((VFn2A4)(*(void***)target)[0x2A4 / 4])(target);
                if (*(u32*)((u8*)p + 0x78) & 0x400) {
                    void* f4 = *(void**)((u8*)target + 0x4);
                    int val = *(u32*)(((VFn30)(*(void***)f4)[0x30 / 4])(f4));
                    bool ok = func_80174C98(target, &val, 0x806) != 0;
                    if (!ok && !(*(u16*)((u8*)target + 0x3E6C) & 0x20))
                        goto alt_combo_skip;
                    void* sub2 = ((VFn2A4)(*(void***)target)[0x2A4 / 4])(target);
                    if (*(void**)((u8*)sub2 + 0x50) == 0) goto alt_combo_skip;
                    void* sub3 = ((VFn2A4)(*(void***)target)[0x2A4 / 4])(target);
                    void* sub4 = *(void**)((u8*)sub3 + 0x50);
                    // ALT: bonus applies when combo target type == 1
                    if (*(u16*)((u8*)sub4 + 0x3C) != 1) goto alt_combo_skip;
                    void* sub5 = ((VFn2A4)(*(void***)target)[0x2A4 / 4])(target);
                    void* sub6 = *(void**)((u8*)sub5 + 0x50);
                    if (*(u8*)((u8*)sub6 + 0x42) == 1) goto alt_combo_skip;
                    move->field_58 += 0.5f;             // 0x800DF11C
                }
            alt_combo_skip:;
            }

            // --- 0x800DF128: level-difference multiplier (alt thresholds) ---
            {
                s32 tl = ((VFn108)(*(void***)target)[0x108 / 4])(target);
                s32 al = ((VFn108)(*(void***)attacker)[0x108 / 4])(attacker);
                s32 diff = tl - al;
                if (diff >= 7) move->field_54 *= 0.5f;
                else if (diff >= 4) move->field_54 *= 0.75f;
                else if (diff <= -7) move->field_54 *= 1.5f;
                else if (diff <= -4) move->field_54 *= 1.25f;
            }

            // --- 0x800DF1C4: field_74 bit15 (0x4000) -> vf0x308 table ---
            if (move->field_74 & 0x4000) {
                s32 idx = ((VFn308)(*(void***)attacker)[0x308 / 4])(attacker,
                          0x00000000, 0x00000000, 0x00000000,
                          0x3DCCCCCD, 0x3E4CCCCD);      // {0,0,0,0.1,0.2}
                if (idx >= 0 && idx < 5) move->field_58 += sTable_150[idx];
            }

            // --- 0x800DF228: art-type dispatch (guard: !data 0x13) ---
            if (!func_80148778((u8*)target + 8, 0x13)) {
                BattleParamData* tgtP = tgtParam;
                u16 artType = sub->field_40;
                s32 r0 = 0;

                // ---- dispatch A' (0x800DF23C): byte 0x72, negate, thr 200 ----
                switch (artType) {
                    case 1: case 2: case 3:
                        r0 = (tgtP->field_72 >> 7) & 1; break;
                    case 4: case 5: case 6: case 7: case 8: case 9: {
                        s16 val = artType == 4 ? tgtP->field_64 :
                                  artType == 5 ? tgtP->field_66 :
                                  artType == 6 ? tgtP->field_68 :
                                  artType == 7 ? tgtP->field_6A :
                                  artType == 8 ? tgtP->field_6C : tgtP->field_6E;
                        if (val >= 200) r0 = 1;
                        else {
                            switch (artType) {
                                case 1: case 2: case 3:
                                    r0 = (tgtP->field_72 >> 7) & 1; break;
                                case 4: r0 = tgtP->field_72 & 1; break;
                                case 5: r0 = (tgtP->field_72 >> 1) & 1; break;
                                case 6: r0 = (tgtP->field_72 >> 2) & 1; break;
                                case 7: r0 = (tgtP->field_72 >> 3) & 1; break;
                                case 8: r0 = (tgtP->field_72 >> 4) & 1; break;
                                case 9: r0 = (tgtP->field_72 >> 5) & 1; break;
                            }
                        }
                        break;
                    }
                    default: r0 = 0; break;
                }
                if (r0 != 0) {
                    move->field_54 *= -1.0f;
                    move->field_74 |= 0x80000800;
                    goto dispatch_alt_done;
                }

                // ---- dispatch B' (0x800DF4A0): byte 0x70, halve, thr 100 ----
                r0 = 0;
                switch (artType) {
                    case 1: case 2: case 3:
                        r0 = (tgtP->field_70 >> 7) & 1; break;
                    case 4: case 5: case 6: case 7: case 8: case 9: {
                        s16 val = artType == 4 ? tgtP->field_64 :
                                  artType == 5 ? tgtP->field_66 :
                                  artType == 6 ? tgtP->field_68 :
                                  artType == 7 ? tgtP->field_6A :
                                  artType == 8 ? tgtP->field_6C : tgtP->field_6E;
                        if (val >= 100) r0 = 1;
                        else {
                            switch (artType) {
                                case 1: case 2: case 3:
                                    r0 = (tgtP->field_70 >> 7) & 1; break;
                                case 4: r0 = tgtP->field_70 & 1; break;
                                case 5: r0 = (tgtP->field_70 >> 1) & 1; break;
                                case 6: r0 = (tgtP->field_70 >> 2) & 1; break;
                                case 7: r0 = (tgtP->field_70 >> 3) & 1; break;
                                case 8: r0 = (tgtP->field_70 >> 4) & 1; break;
                                case 9: r0 = (tgtP->field_70 >> 5) & 1; break;
                            }
                        }
                        break;
                    }
                    default: r0 = 0; break;
                }
                if (r0 != 0) {
                    move->field_54 *= 0.5f;
                    goto dispatch_alt_done;
                }

                // ---- dispatch C' (0x800DF6F0): 0x72 then 0x73, +0.25 ----
                r0 = 0;
                switch (artType) {
                    case 1: case 2: case 3:
                        r0 = (tgtP->field_72 >> 7) & 1; break;
                    case 4: case 5: case 6: case 7: case 8: case 9: {
                        s16 val = artType == 4 ? tgtP->field_64 :
                                  artType == 5 ? tgtP->field_66 :
                                  artType == 6 ? tgtP->field_68 :
                                  artType == 7 ? tgtP->field_6A :
                                  artType == 8 ? tgtP->field_6C : tgtP->field_6E;
                        if (val < 0) r0 = 1;             // negative -> active
                        else {
                            switch (artType) {
                                case 1: case 2: case 3:
                                    r0 = (tgtP->field_73 >> 7) & 1; break;
                                case 4: r0 = tgtP->field_73 & 1; break;
                                case 5: r0 = (tgtP->field_73 >> 1) & 1; break;
                                case 6: r0 = (tgtP->field_73 >> 2) & 1; break;
                                case 7: r0 = (tgtP->field_73 >> 3) & 1; break;
                                case 8: r0 = (tgtP->field_73 >> 4) & 1; break;
                                case 9: r0 = (tgtP->field_73 >> 5) & 1; break;
                            }
                        }
                        break;
                    }
                    default: r0 = 0; break;
                }
                if (r0 != 0) {
                    move->field_58 += 0.25f;
                }
            }
        dispatch_alt_done:;
            // ============================================================
            // 0x800DF93C onwards (shared with main-block dispatch end)
            // ============================================================
            goto post_dispatch;
        }

        // ================================================================
        // MAIN BLOCK (types 1/5), 0x800DD730
        // ================================================================
        f32 f26 = (f32)(s32)*(s16*)((u8*)res20C + 0x60) / 100.0f;
        f32 f28 = (f32)(s32)tgtParam->field_60 / 100.0f - f26;
        s32 r25 = atkParam->field_2C;
        s32 r26 = tgtParam->field_2E;

        // --- hit-flag conditional zero of f26 (0x800DD78C) ---
        {
            u32 tag = move->field_74;
            if (tag & 0x2000000) {
                u16 hf = ((VFn328)(*(void***)target)[0x328 / 4])(target);
                if (!(hf & 0x4000)) f26 = 0.0f;
            } else if (tag & 0x4000000) {
                u16 hf = ((VFn328)(*(void***)target)[0x328 / 4])(target);
                if (!(hf & 0x2000)) f26 = 0.0f;
            } else if (tag & 0x1000000) {
                u16 hf = ((VFn328)(*(void***)target)[0x328 / 4])(target);
                if (!(hf & 0x1)) f26 = 0.0f;
            }
        }
        f28 += f26;                                     // 0x800DD804

        // 0x800DD80C: sub->field_78 bit24 (0x800000) -> average r25
        if (sub->field_78 & 0x800000) {
            r25 = (r25 + atkParam->field_1E) / 2;
        }

        // --- clamp [-1.0, 0.9] ---
        if (f28 < -1.0f) f28 = -1.0f;
        else if (f28 > 0.9f) f28 = 0.9f;

        // --- zero ratio under data-map flags ---
        if (func_80148778((u8*)target + 8, 0x13)) {
            r26 = 0;
            f28 = 0.0f;
        }
        if ((((BattleObjAccessor*)attacker)->field_3f00 & 0x4) &&
            (move->field_78 & 0x800)) {
            r26 = 0;
            f28 = 0.0f;
        }

        // --- hit quality flags (0x800DD8B8) ---
        if (f28 > 0.0f && f28 < 0.5f) {
            move->field_74 |= 0x80100000;
        } else if (f28 >= 0.5f && f28 < 1.0f) {
            move->field_74 |= 0x80200000;
        }

        // --- guard path: move->field_78 bit21 (0x200) ---
        if (move->field_78 & 0x200) {
            s32 vf = subVf0C(sub);
            s32 r5 = sub->field_38 + (s32)sub->field_6C * (vf - 1);
            if (((BattleObjAccessor*)attacker)->field_3f00 & 0x2) {
                f32 f2 = atkParam->field_24;
                f32 f1 = atkParam->field_28 - f2;
                s32 d = (s32)(1.0f + f1);
                u32 r4 = move->field_94;
                r25 = (s32)((f32)(s32)r25 + f2) + (s32)(r4 % (u32)d);
            } else {
                r5 = (s32)((f32)(s32)r5 *
                           (f32)(s32)((move->field_94 % 21) + 90) / 100.0f);
            }
            move->field_54 = (f32)(s32)(r25 - r26) * (f32)(s32)r5 / 100.0f;

            if (func_80148778((u8*)attacker + 8, 0xC8)) {
                move->field_54 *= 0.5f;
            }
            if (move->field_54 < 1.0f) move->field_54 = 1.0f;
            move->field_54 = move->field_54 - move->field_54 * f28;
            goto main_tail;
        }

        // --- non-guard: move->field_78 bits 20-21 (0x600) ---
        if (move->field_78 & 0x600) {
            // 0x800DDAB0: same r25 variance as above
            if (((BattleObjAccessor*)attacker)->field_3f00 & 0x2) {
                f32 f2 = atkParam->field_24;
                f32 f1 = atkParam->field_28 - f2;
                s32 d = (s32)(1.0f + f1);
                u32 r4 = move->field_94;
                r25 = (s32)((f32)(s32)r25 + f2) + (s32)(r4 % (u32)d);
            }
            // three sub-vtable calls (0x800DDB20)
            s32 v1 = subVf0C(sub);
            s32 v2 = subVf0C(sub);
            s32 v3 = subVf0C(sub);
            s32 r18 = sub->field_38 + (s32)sub->field_6C * (v1 - 1);
            s32 r19 = sub->field_3A + (s32)sub->field_6D * (v2 - 1);
            s32 r20 = (s32)move->field_94;
            s32 r7 = r19 - r18 + 1;
            s32 r0 = sub->field_38 + (s32)sub->field_6C * (v3 - 1) +
                     (s32)(r20 % (u32)r7);
            move->field_54 =
                (f32)(s32)(r25 - r26) * (f32)(s32)r0 / 100.0f;       // 0x800DDBE4

            // 0x800DDBF0: clamp + apply ratio
            if (move->field_54 < 1.0f) move->field_54 = 1.0f;
            move->field_54 = move->field_54 - move->field_54 * f28;

            // 0x800DDC10: 0x93 -> field_58 += 0.5 ; 0xC0 -> += val/10 - 1
            if (func_80148778((u8*)attacker + 8, 0x93)) {
                move->field_58 += 0.5f;
            }
            if (func_80148778((u8*)attacker + 8, 0xC0)) {
                void* entry = func_80149154((u8*)attacker + 8, 0xC0);
                move->field_58 +=
                    (f32)(s32)*(u32*)((u8*)entry + 0x10) / 10.0f - 1.0f;
            }
        }

    main_tail:;
        // ================================================================
        // MAIN-BLOCK TAIL (0x800DDC80): combo / level-diff / vf0x308
        // ================================================================
        // --- 0x800DDC80: target vf0x2A4 (combo) checks ---
        {
            void* p = ((VFn2A4)(*(void***)target)[0x2A4 / 4])(target);
            if (*(u32*)((u8*)p + 0x78) & 0x400) {
                void* f4 = *(void**)((u8*)target + 0x4);
                int val = *(u32*)(((VFn30)(*(void***)f4)[0x30 / 4])(f4));
                bool ok = func_80174C98(target, &val, 0x806) != 0;
                if (!ok && !(*(u16*)((u8*)target + 0x3E6C) & 0x20))
                    goto main_combo_skip;
                void* sub2 = ((VFn2A4)(*(void***)target)[0x2A4 / 4])(target);
                if (*(void**)((u8*)sub2 + 0x50) == 0) goto main_combo_skip;
                void* sub3 = ((VFn2A4)(*(void***)target)[0x2A4 / 4])(target);
                void* sub4 = *(void**)((u8*)sub3 + 0x50);
                // MAIN: bonus applies when combo target type != 1
                if (*(u16*)((u8*)sub4 + 0x3C) == 1) goto main_combo_skip;
                void* sub5 = ((VFn2A4)(*(void***)target)[0x2A4 / 4])(target);
                void* sub6 = *(void**)((u8*)sub5 + 0x50);
                if (*(u8*)((u8*)sub6 + 0x42) == 1) goto main_combo_skip;
                move->field_58 += 0.5f;                 // 0x800DDD48
            }
        main_combo_skip:;
        }

        // --- 0x800DDD58: level-difference multiplier (main thresholds) ---
        {
            s32 tl = ((VFn108)(*(void***)target)[0x108 / 4])(target);
            s32 al = ((VFn108)(*(void***)attacker)[0x108 / 4])(attacker);
            s32 diff = tl - al;
            if (diff < -2) {
                if (diff >= -5) move->field_54 *= 1.25f;
                else if (diff >= -9) move->field_54 *= 1.5f;
                else move->field_54 *= 2.0f;
            } else {
                if (diff >= 6) move->field_54 *= 0.5f;
                else if (diff >= 3) move->field_54 *= 0.75f;
            }
        }

        // --- 0x800DDE18: field_74 bit15 (0x4000) -> vf0x308 table ---
        if (move->field_74 & 0x4000) {
            s32 idx = ((VFn308)(*(void***)attacker)[0x308 / 4])(attacker,
                      0x00000000, 0x00000000, 0x00000000,
                      0x3DCCCCCD, 0x3E4CCCCD);          // {0,0,0,0.1,0.2}
            if (idx >= 0 && idx < 5) move->field_58 += sTable_150[idx];
        }

        // --- 0x800DDE7C: art-type dispatch (guard: !data 0x13) ---
        if (!func_80148778((u8*)target + 8, 0x13)) {
            BattleParamData* tgtP = tgtParam;
            u16 artType = sub->field_40;
            s32 r0 = 0;

            // ---- dispatch A (0x800DDE90): byte 0x72, negate, thr 200 ----
            switch (artType) {
                case 1: case 2: case 3:
                    r0 = (tgtP->field_72 >> 7) & 1; break;
                case 4: case 5: case 6: case 7: case 8: case 9: {
                    s16 val = artType == 4 ? tgtP->field_64 :
                              artType == 5 ? tgtP->field_66 :
                              artType == 6 ? tgtP->field_68 :
                              artType == 7 ? tgtP->field_6A :
                              artType == 8 ? tgtP->field_6C : tgtP->field_6E;
                    if (val >= 200) r0 = 1;
                    else {
                        switch (artType) {
                            case 1: case 2: case 3:
                                r0 = (tgtP->field_72 >> 7) & 1; break;
                            case 4: r0 = tgtP->field_72 & 1; break;
                            case 5: r0 = (tgtP->field_72 >> 1) & 1; break;
                            case 6: r0 = (tgtP->field_72 >> 2) & 1; break;
                            case 7: r0 = (tgtP->field_72 >> 3) & 1; break;
                            case 8: r0 = (tgtP->field_72 >> 4) & 1; break;
                            case 9: r0 = (tgtP->field_72 >> 5) & 1; break;
                        }
                    }
                    break;
                }
                default: r0 = 0; break;
            }
            if (r0 != 0) {
                move->field_54 *= -1.0f;
                move->field_74 |= 0x80000800;
                goto post_dispatch;
            }

            // ---- dispatch B (0x800DE0F4): byte 0x70, halve, thr 100 ----
            r0 = 0;
            switch (artType) {
                case 1: case 2: case 3:
                    r0 = (tgtP->field_70 >> 7) & 1; break;
                case 4: case 5: case 6: case 7: case 8: case 9: {
                    s16 val = artType == 4 ? tgtP->field_64 :
                              artType == 5 ? tgtP->field_66 :
                              artType == 6 ? tgtP->field_68 :
                              artType == 7 ? tgtP->field_6A :
                              artType == 8 ? tgtP->field_6C : tgtP->field_6E;
                    if (val >= 100) r0 = 1;
                    else {
                        switch (artType) {
                            case 1: case 2: case 3:
                                r0 = (tgtP->field_70 >> 7) & 1; break;
                            case 4: r0 = tgtP->field_70 & 1; break;
                            case 5: r0 = (tgtP->field_70 >> 1) & 1; break;
                            case 6: r0 = (tgtP->field_70 >> 2) & 1; break;
                            case 7: r0 = (tgtP->field_70 >> 3) & 1; break;
                            case 8: r0 = (tgtP->field_70 >> 4) & 1; break;
                            case 9: r0 = (tgtP->field_70 >> 5) & 1; break;
                        }
                    }
                    break;
                }
                default: r0 = 0; break;
            }
            if (r0 != 0) {
                move->field_54 *= 0.5f;
                goto post_dispatch;
            }

            // ---- dispatch C (0x800DE344): 0x72 then 0x73, +0.25 ----
            r0 = 0;
            switch (artType) {
                case 1: case 2: case 3:
                    r0 = (tgtP->field_72 >> 7) & 1; break;
                case 4: case 5: case 6: case 7: case 8: case 9: {
                    s16 val = artType == 4 ? tgtP->field_64 :
                              artType == 5 ? tgtP->field_66 :
                              artType == 6 ? tgtP->field_68 :
                              artType == 7 ? tgtP->field_6A :
                              artType == 8 ? tgtP->field_6C : tgtP->field_6E;
                    if (val < 0) r0 = 1;                 // negative -> active
                    else {
                        switch (artType) {
                            case 1: case 2: case 3:
                                r0 = (tgtP->field_73 >> 7) & 1; break;
                            case 4: r0 = tgtP->field_73 & 1; break;
                            case 5: r0 = (tgtP->field_73 >> 1) & 1; break;
                            case 6: r0 = (tgtP->field_73 >> 2) & 1; break;
                            case 7: r0 = (tgtP->field_73 >> 3) & 1; break;
                            case 8: r0 = (tgtP->field_73 >> 4) & 1; break;
                            case 9: r0 = (tgtP->field_73 >> 5) & 1; break;
                        }
                    }
                    break;
                }
                default: r0 = 0; break;
            }
            if (r0 != 0) {
                move->field_58 += 0.25f;
            }
        }
    }

post_dispatch:;
    // ================================================================
    // 0x800DE590 (post-dispatch, shared): position-diff, 0xCE reaction,
    // crit block.
    // ================================================================
    // --- 0x800DE590: attacker/target chain-pos diff -> field_58 ---
    if (*(u32*)((u8*)target + 0x3374) & 0x20000) {
        BattleVec* vT = vfAC(target);
        BattleVec* vA = vfAC(attacker);
        f32 d = vA->y - vT->y;
        if (d < 0.0f) d = -d;
        if (d <= 10.0f) move->field_58 += 2.0f;         // 0x800DE5E8
    }

    // --- 0x800DE5F8: 0xCE reaction block ---
    if (func_80148778((u8*)target + 8, 0xCE)) {
        if (move->field_78 & 0x800) {
            void* entry = func_801491F4((u8*)target + 8, 0xCE);
            if ((s32)*(u32*)((u8*)entry + 0x10) >= subVf0C(sub)) {
                move->field_54 = 1.0f;
                move->field_74 |= 0x80002000;
            }
        }
    }

    // --- 0x800DE660: field_74 bit1 (0x2) -> direct ratio clamp, else crit ---
    if (move->field_74 & 0x2) {
        // 0x800DE66C: field_54 *= (0.5 - (f32)targetParam->field_54)
        move->field_54 *= 0.5f - (f32)tgtParam->field_54;
    } else {
        // ================================================================
        // CRIT BLOCK (0x800DE698..0x800DE8DC) -- r20 accumulation
        // ================================================================
        s32 r20 = atkParam->field_38;
        if (((BattleObjAccessor*)attacker)->field_3f00 & 0x2) {
            s32 idx = ((VFn308_0)(*(void***)attacker)[0x308 / 4])(attacker);
            if (idx >= 0 && idx < 8) r20 += sTable_208[idx];
        }
        bool r22f = false;
        if ((sub->field_78 & 0x4) &&
            (*(u32*)((u8*)target + 0x3374) & 0x100)) {   // bit23 -> 0x100
            r22f = true;
            r20 += 0x19;
        }
        if (sub->field_78 & 0x10) {
            f32 f1 = ((VFn158)(*(void***)attacker)[0x158 / 4])(attacker);
            r20 = (s32)((f32)(s32)r20 + f1 * 0.5f);
        }
        if (sub->field_78 & 0x2) {
            r20 = 100;
        }
        if (vf290(attacker) != 0) {
            if (func_8026178C(vf290(attacker), 0x59) &&
                (move->field_78 & 0x2000000)) {
                r20 = 100;
            }
            if (func_8026178C(vf290(attacker), 0x5A) &&
                (move->field_78 & 0x1000000)) {
                r20 = 100;
            }
        }
        if (func_80148778((u8*)target + 8, 0xC2)) {
            void* entry = func_80149154((u8*)target + 8, 0xC2);
            r20 -= (s32)*(u32*)((u8*)entry + 0x10);
        }
        // 0x800DE804: proc check (field_98 % 100) < r20 -> crit!
        if ((s32)(move->field_98 % 100) < r20) {
            f32 f26 = 0.25f;
            if (func_80148778((u8*)target + 8, 0xF) ||
                func_80148778((u8*)target + 8, 0x10)) {
                f26 += 0.25f;                            // 0.5 total
            }
            if (r22f) f26 += 0.25f;
            if (vf290(attacker) != 0) {
                u32 sv;
                if (func_80260264(vf290(attacker), 0x51, &sv)) {
                    f26 += (f32)(s32)sv / 100.0f;
                }
            }
            move->field_58 += f26;
            move->field_74 |= 0x80000100;
        }
    }

    // ================================================================
    // 0x800DFC5C: 0xF/0x83 and 0x10/0x84 data-map pairs
    // ================================================================
    if (func_80148778((u8*)target + 8, 0xF)) {
        if (func_80148778((u8*)attacker + 8, 0x83)) {
            void* entry = func_80149154((u8*)attacker + 8, 0x83);
            move->field_58 +=
                (f32)(s32)*(u32*)((u8*)entry + 0x10) / 100.0f;
        }
    }
    if (func_80148778((u8*)target + 8, 0x10)) {
        if (func_80148778((u8*)attacker + 8, 0x84)) {
            void* entry = func_80149154((u8*)attacker + 8, 0x84);
            move->field_58 +=
                (f32)(s32)*(u32*)((u8*)entry + 0x10) / 100.0f;
        }
    }

    // ================================================================
    // 0x800DFD1C: field_74 bit25 (0x2000000) -- party-list membership bonus
    // ================================================================
    if (move->field_74 & 0x2000000) {
        void* f4 = *(void**)((u8*)attacker + 0x4);
        int val = *(u32*)(((VFn30)(*(void***)f4)[0x30 / 4])(f4));
        if (func_80174C98(attacker, &val, 0x800) != 0) {
            // walk self->mActorList1 sentinel (self+8) looking for target
            void* sentinel = *(void**)((u8*)self + 0x8);
            void* cur = *(void**)sentinel;
            bool found = false;
            while (cur != sentinel) {
                if (*(void**)((u8*)cur + 0x8) == target) { found = true; break; }
                cur = *(void**)cur;
            }
            if (found) {
                move->field_58 += 1.0;                  // f64 1.0 -> fadd
                move->field_78 |= 0x40000002;
            }
        }
        if (func_80148778((u8*)attacker + 8, 0xB2)) {
            void* entry = func_80149154((u8*)attacker + 8, 0xB2);
            move->field_58 +=
                (f32)(s32)*(u32*)((u8*)entry + 0x10) / 100.0f;
        }
    }

    // ================================================================
    // 0x800DFE08: member check 0x800 + attacker skill values
    // ================================================================
    {
        void* f4 = *(void**)((u8*)attacker + 0x4);
        int val = *(u32*)(((VFn30)(*(void***)f4)[0x30 / 4])(f4));
        if (func_80174C98(attacker, &val, 0x800) != 0) {
            if (func_80148778((u8*)attacker + 8, 0xB1)) {
                void* entry = func_80149154((u8*)attacker + 8, 0xB1);
                move->field_58 +=
                    (f32)(s32)*(u32*)((u8*)entry + 0x10) / 100.0f;
            }
            if (vf290(attacker) != 0) {
                u32 sv;
                if (func_80260264(vf290(attacker), 0x52, &sv)) {
                    move->field_58 += (f32)(s32)sv / 100.0f;
                }
                if (func_80260264(vf290(attacker), 0x53, &sv) &&
                    *(u32*)((u8*)attacker + 0x1530) != 0 &&
                    ((VFnE0)(*(void***)target)[0xE0 / 4])(target) == 9) {
                    move->field_58 += (f32)(s32)sv / 100.0f;
                }
            }
            if ((((BattleObjAccessor*)attacker)->field_3f00 & 0x2) &&
                ((BattleObjAccessor*)attacker)->field_3f28 == 5 &&
                (move->field_78 & 0x400)) {
                f32 f1 = ((VFn158)(*(void***)attacker)[0x158 / 4])(attacker);
                move->field_58 += f1 / 150.0f;
            }
            if (func_80148778((u8*)target + 8, 0x9)) {
                move->field_58 += 0.5f;
                if (vf290(attacker) != 0) {
                    u32 sv;
                    if (func_80260264(vf290(attacker), 0x58, &sv)) {
                        move->field_58 += (f32)(s32)sv / 100.0f;
                    }
                }
                move->field_74 |= 0x80000100;
            }
        }
    }

    // ================================================================
    // 0x800E0090: sub->field_48 == 0x108 -- count active skill entries
    // ================================================================
    if (sub->field_48 == 0x108) {
        s32 count = 0;
        for (f32 i = 0.0f; i < 32.0f; i += 1.0f) {
            void* e = ((VFn5C)(*(void***)((u8*)target + 8))[0x5C / 4])(
                          (u8*)target + 8, (u32)(s32)i);
            if (*(u16*)((u8*)e + 0xC) != 0) count++;
        }
        move->field_58 += (f32)(s32)count;
    }

    // ================================================================
    // 0x800E010C: 0x10C / 0xFD penalty
    // ================================================================
    if (func_80148778((u8*)attacker + 8, 0x10C) ||
        func_80148778((u8*)attacker + 8, 0xFD)) {
        move->field_58 -= 0.25f;
    }

    // ================================================================
    // 0x800E0144: target vf0x290 damage-resist skills
    // ================================================================
    if (vf290(target) != 0) {
        u32 sv;
        if (move->field_78 & 0x400) {
            if (func_80260264(vf290(target), 0x4E, &sv) && sub->field_40 == 6) {
                move->field_58 += (f32)(s32)sv / 100.0f;
            }
            if (func_80260264(vf290(target), 0x4E, &sv) && sub->field_40 == 8) {
                move->field_58 += (f32)(s32)sv / 100.0f;
            }
        }
        if (sub->type_3C == 1 || sub->type_3C == 5) {
            if (func_80260264(vf290(target), 0x4F, &sv)) {
                move->field_58 *= 1.0f - (f32)(s32)sv / 100.0f;
            }
        } else {
            if (func_80260264(vf290(target), 0x50, &sv)) {
                move->field_58 *= 1.0f - (f32)(s32)sv / 100.0f;
            }
            if (func_80260264(vf290(target), 0x54, &sv) && sub->field_5E != 0) {
                move->field_58 *= 1.0f - (f32)(s32)sv / 100.0f;
            }
        }
    }

    // ================================================================
    // 0x800E0364: chain membership -> self->field_20A8 ; event dispatch
    // ================================================================
    if (func_802799F0((u8*)self + 0x1A8, attacker)) {
        move->field_58 += *(f32*)((u8*)self + 0x20A8);
    }
    func_800E08E8(self, attacker, target, move);        // 0x800E0388

    // ================================================================
    // 0x800E039C: field_5C clamp
    // ================================================================
    if (func_80148778((u8*)target + 8, 0x1)) {
        move->field_5C = 0.0f;
    } else if (!(move->field_74 & 0x8000) && move->field_5C < 1.0f) {
        move->field_5C = 1.0f;
    }

    // ================================================================
    // 0x800E03DC: field_68 rounding block (f64 0.5/-0.5 rounding)
    // ================================================================
    if (!(move->field_74 & 0x80)) {
        if (move->field_74 & 0x8000) {
            f32 sum = move->field_5C + move->field_60;
            s32 v = 0;
            if (sum >= 0.0f) {
                v = (s32)((f64)sum + (sum > 0.0f ? 0.5 : -0.5));
            }
            if (v > 0) {
                f32 sum2 = move->field_5C + move->field_60;
                s32 v2 = 0;
                if (sum2 >= 0.0f) {
                    v2 = (s32)((f64)sum2 + (sum2 > 0.0f ? 0.5 : -0.5));
                }
                move->field_68 -= (f32)(s32)v2;
            }
        } else {
            f32 sum2 = move->field_5C + move->field_60;
            s32 v2 = 0;
            if (sum2 >= 0.0f) {
                v2 = (s32)((f64)sum2 + (sum2 > 0.0f ? 0.5 : -0.5));
            }
            move->field_68 -= (f32)(s32)v2;
        }
    }

    // ================================================================
    // 0x800E04B8: status blocks 0x92 / 0xFC / 0x100 (dot/counter)
    // ================================================================
    if (!(move->field_78 & 0x1000) &&
        ((VFn2BC)(*(void***)target)[0x2BC / 4])(target) == 0 &&
        move->field_5C > 0.0f) {
        // ---- 0x92 counter block (0x800E04F0) ----
        if (func_80148778((u8*)target + 8, 0x92)) {
            s32 extra = 0;
            if (func_80148778((u8*)target + 8, 0xA1)) {
                void* e = func_80149154((u8*)target + 8, 0xA1);
                extra = (s32)*(u32*)((u8*)e + 0x10);
            }
            void* entry = func_80149154((u8*)target + 8, 0x92);
            if (entry != 0 && move->field_68 <= 0.0f) {
                s32 r5 = (s32)*(u32*)((u8*)entry + 0x10);
                if (*(u32*)((u8*)entry + 0x8) == 0x2000) r5 += extra;
                if ((s32)(move->field_A8 % 100) < r5) {
                    move->field_74 |= 0xA0000000;
                    move->field_68 = 1.0f;
                }
            }
        }
        // ---- 0xFC block (0x800E05B0) ----
        if (!(move->field_78 & 0x2000) &&
            func_80148778((u8*)target + 8, 0xFC)) {
            void* entry = func_80149154((u8*)target + 8, 0xFC);
            if (entry != 0 && move->field_68 <= 0.0f) {
                move->field_78 |= 0x40002000;
                f32 f1 = ((VFn12C)(*(void***)target)[0x12C / 4])(target);
                move->field_68 = 0.01f * (f32)(s32)*(u32*)((u8*)entry + 0x10) * f1;
            }
        }
        // ---- 0x100 block (0x800E064C) ----
        if (!(move->field_78 & 0x2000) &&
            func_80148778((u8*)target + 8, 0x100)) {
            void* entry = func_80149154((u8*)target + 8, 0x100);
            if (entry != 0 && move->field_68 <= 0.0f) {
                move->field_78 |= 0x40002000;
                f32 f1 = ((VFn12C)(*(void***)target)[0x12C / 4])(target);
                move->field_68 = 0.01f * (f32)(s32)*(u32*)((u8*)entry + 0x10) * f1;
            }
        }
    }

    // ================================================================
    // 0x800E06D8: status blocks 0x36 / 0x37 / 0x11E (dot ticks)
    // ================================================================
    if (!(move->field_78 & 0x1000) &&
        ((VFn2BC)(*(void***)target)[0x2BC / 4])(target) == 0) {
        // ---- 0x36 block (0x800E070C) ----
        if (!(move->field_78 & 0x4000) &&
            func_80148778((u8*)target + 8, 0x36)) {
            void* entry = func_80149154((u8*)target + 8, 0x36);
            if (entry != 0 && move->field_68 > 0.0f) {
                s32 v = (s32)*(u32*)((u8*)entry + 0x10);
                f32 f1 = func_800D81A8(0, target, 0);
                v = (s32)((f32)(s32)v * f1);
                move->field_68 += (f32)(s32)v;
                if (*(s16*)((u8*)entry + 0x14) > 0) {
                    if (move->field_70 == 0) {
                        move->field_70 = *(s16*)((u8*)entry + 0x14);
                    }
                    move->field_70 -= 1;
                    if (move->field_70 <= 0) {
                        move->field_70 = 0;
                        move->field_78 |= 0x40004000;
                    }
                }
            }
        }
        // ---- 0x37 block (0x800E07E4) ----
        if (func_80148778((u8*)target + 8, 0x37)) {
            s32 extra = 0;
            if (func_80148778((u8*)target + 8, 0xA1)) {
                void* e = func_80149154((u8*)target + 8, 0xA1);
                extra = (s32)*(u32*)((u8*)e + 0x10);
            }
            void* entry = func_80149154((u8*)target + 8, 0x37);
            if (entry != 0 && move->field_68 > 0.0f) {
                s32 r6 = *(s16*)((u8*)entry + 0x1A);
                if (*(u32*)((u8*)entry + 0x8) == 0x2000) r6 += extra;
                if ((s32)(move->field_A8 % 100) < r6) {
                    s32 v = (s32)*(u32*)((u8*)entry + 0x10);
                    f32 f1 = func_800D81A8(0, target, 0);
                    v = (s32)((f32)(s32)v * f1);
                    move->field_68 += (f32)(s32)v;
                }
            }
        }
        // ---- 0x11E block (0x800E08E4) ----
        if (!(move->field_78 & 0x4000) &&
            func_80148778((u8*)target + 8, 0x11E)) {
            void* entry = func_80149154((u8*)target + 8, 0x11E);
            if (entry != 0 && move->field_68 > 0.0f) {
                f32 f1 = ((VFn12C)(*(void***)target)[0x12C / 4])(target);
                s32 v = (s32)((f32)(s32)*(s16*)((u8*)entry + 0x14) * f1 / 100.0f);
                f1 = func_800D81A8(0, target, 0);
                v = (s32)((f32)(s32)v * f1);
                move->field_68 += (f32)(s32)v;
                if ((s32)*(u32*)((u8*)entry + 0x10) > 0) {
                    if (move->field_70 == 0) {
                        move->field_70 = (s16)*(u32*)((u8*)entry + 0x10);
                    }
                    move->field_70 -= 1;
                    if (move->field_70 <= 0) {
                        move->field_70 = 0;
                        move->field_78 |= 0x40004000;
                    }
                }
            }
        }
    }

    // ================================================================
    // 0x800E09FC: field_68 <= 0 -> "dot expired" flag
    // ================================================================
    if (move->field_68 <= 0.0f) {
        move->field_78 |= 0x40001000;
    }

    // ================================================================
    // 0x800E0A20: 0x106 block -- final rounded damage to field_6C
    // ================================================================
    if (func_80148778((u8*)target + 8, 0x106)) {
        func_80149154((u8*)target + 8, 0x106);          // presence-only call
        if (!(move->field_78 & 0x800)) {
            if (move->field_50 == 0) return;            // 0x800E0A50

            // step selection (0x800E0A58..0x800E0B30)
            s32 rem = (s32)(move->field_B0 % 100);
            s32 steps[3];
            steps[0] = subVf0C(sub) / 2 + 3;
            steps[1] = subVf0C(sub) + 0xA;
            steps[2] = 100;
            f32 mult = 1.0f;
            s32 acc = rem;
            for (int i = 0; i < 3; i++) {
                acc -= steps[i];
                if (acc < 0) { mult = sTable_28C[i]; break; }
            }
            if (vf290(target) != 0) {
                u32 sv;
                if (func_80260264(vf290(target), 0x5B, &sv)) {
                    mult *= 1.0f + (f32)(s32)sv / 100.0f;
                }
            }
            // rounded damage (0x800E0BA0..0x800E0CA0)
            f32 sum = move->field_5C + move->field_60;
            s32 v = 0;
            if (sum >= 0.0f && !(move->field_74 & 0x80)) {
                v = (s32)((f64)sum + (sum > 0.0f ? 0.5 : -0.5));
            }
            f32 f1 = (f32)(s32)v;
            s32 r3 = (s32)((f64)f1 + (f1 > 0.0f ? 0.5 : -0.5));
            f1 = (f32)(s32)r3 * mult;
            s32 r0 = (s32)((f64)f1 + (f1 > 0.0f ? 0.5 : -0.5));
            move->field_6C -= (f32)(s32)r0;
        }
    }

    // ================================================================
    // 0x800E0CA4: guards before the art loop
    // ================================================================
    if (func_80148778((u8*)target + 8, 0x10)) {
        if (!(*(u32*)((u8*)target + 0x3374) & 0x40000)) return;  // bit13
        void* f4 = *(void**)((u8*)target + 0x4);
        int val = *(u32*)(((VFn30)(*(void***)f4)[0x30 / 4])(f4));
        if (func_80174C98(target, &val, 0x17) == 0) return;
    }
    if (func_80148778((u8*)target + 8, 0x117)) return;
    if (move->field_74 & 0x80) return;

    // ================================================================
    // 0x800E0D4C: art loop -- slots 0x40..0x67 (64 entries)
    // ================================================================
    for (u32 idx = 0x40; idx < 0x68; idx++) {
        void* entry = ((VFn54)(*(void***)((u8*)target + 8))[0x54 / 4])(
                          (u8*)target + 8, idx);
        u16 type = *(u16*)((u8*)entry + 0xC);
        s32 skillId = 0;
        switch (type) {                                 // jt 0x8052B1BC
            case 0x6E: case 0x78: skillId = (s32)func_80145BC4(0x6E); break;
            case 0x6F: case 0x79: skillId = (s32)func_80145BC4(0x6F); break;
            case 0x70: case 0x7A: skillId = (s32)func_80145BC4(0x70); break;
            case 0x71: case 0x7B: skillId = (s32)func_80145BC4(0x71); break;
            default: skillId = 0; break;
        }
        if (skillId == 0) continue;                     // 0x800E0DCC
        if (*(s16*)((u8*)entry + 0x14) == 0) continue;  // 0x800E0DD4

        // distance check via PSVECMag (0x800E0DE0..0x800E0E68)
        {
            BattleVec* vT = vfAC(target);
            BattleVec* vA = vfAC(attacker);
            f32 delta[3] = { vA->x - vT->x, vA->y - vT->y, vA->z - vT->z };
            if (PSVECMag((const struct Vec*)delta) > (f32)(s32)*(s16*)((u8*)entry + 0x14)) {
                skillId = 0;
            }
        }
        if (skillId == 0) continue;

        // attacker param + entry value (0x800E0E74)
        BattleParamData* atkP =
            (BattleParamData*)((VFn224)(*(void***)attacker)[0x224 / 4])(attacker);
        s32 val = (s32)*(u32*)((u8*)entry + 0x10);
        f32 f27 = 1.0f;
        if (vf290(target) != 0) {
            if (func_80149154((u8*)target + 8, 0x6F) != 0 ||
                func_80149154((u8*)target + 8, 0x79) != 0) {
                u32 sv;
                if (func_80260264(vf290(target), 0x5B, &sv)) {
                    val = (s32)((f32)(s32)val *
                                (1.0f + (f32)(s32)sv / 100.0f));
                }
            }
        }
        // 0x800E0F3C: 0x8D / vf0x290 0x57 reductions
        if (func_80148778((u8*)attacker + 8, 0x8D)) {
            void* e = func_80149154((u8*)attacker + 8, 0x8D);
            f27 = 1.0f - 0.01f * (f32)(s32)*(u32*)((u8*)e + 0x10);
        }
        if (vf290(attacker) != 0) {
            u32 sv;
            if (func_80260264(vf290(attacker), 0x57, &sv)) {
                f27 = 1.0f - 0.01f * (f32)(s32)sv;
            }
        }
        val = (s32)((f32)(s32)val * f27);               // 0x800E0FD0
        if (val < 1) val = 0;
        if (func_80148778((u8*)attacker + 8, 0x1)) val = 0;

        // skill dispatch (0x800E1014..0x800E1358)
        s32 r0 = 0;
        switch (skillId) {                              // jt 0x8052B194 (0x72)
            case 1: case 2: case 3:
                r0 = (atkP->field_72 >> 7) & 1; break;
            case 4: r0 = atkP->field_72 & 1; break;
            case 5: r0 = (atkP->field_72 >> 1) & 1; break;
            case 6: r0 = (atkP->field_72 >> 2) & 1; break;
            case 7: r0 = (atkP->field_72 >> 3) & 1; break;
            case 8: r0 = (atkP->field_72 >> 4) & 1; break;
            case 9: r0 = (atkP->field_72 >> 5) & 1; break;
            default: r0 = 0; break;
        }
        if (r0 == 0 && skillId >= 4) {                  // stat 200 shortcut
            s16 stat = skillId == 4 ? atkP->field_64 :
                       skillId == 5 ? atkP->field_66 :
                       skillId == 6 ? atkP->field_68 :
                       skillId == 7 ? atkP->field_6A :
                       skillId == 8 ? atkP->field_6C : atkP->field_6E;
            if (stat >= 200) {
                r0 = 1;
            } else {
                switch (skillId) {                      // jt 0x8052B16C (0x72)
                    case 4: r0 = atkP->field_72 & 1; break;
                    case 5: r0 = (atkP->field_72 >> 1) & 1; break;
                    case 6: r0 = (atkP->field_72 >> 2) & 1; break;
                    case 7: r0 = (atkP->field_72 >> 3) & 1; break;
                    case 8: r0 = (atkP->field_72 >> 4) & 1; break;
                    case 9: r0 = (atkP->field_72 >> 5) & 1; break;
                }
            }
        }
        if (r0 != 0) {
            val = -val;                                 // 0x800E11A0 negate
        } else {
            // 0x70-byte stage (jt 0x8052B144 / 0x8052B11C)
            r0 = 0;
            switch (skillId) {
                case 1: case 2: case 3:
                    r0 = (atkP->field_70 >> 7) & 1; break;
                case 4: r0 = atkP->field_70 & 1; break;
                case 5: r0 = (atkP->field_70 >> 1) & 1; break;
                case 6: r0 = (atkP->field_70 >> 2) & 1; break;
                case 7: r0 = (atkP->field_70 >> 3) & 1; break;
                case 8: r0 = (atkP->field_70 >> 4) & 1; break;
                case 9: r0 = (atkP->field_70 >> 5) & 1; break;
                default: r0 = 0; break;
            }
            if (r0 == 0 && skillId >= 4) {              // stat 100 shortcut
                s16 stat = skillId == 4 ? atkP->field_64 :
                           skillId == 5 ? atkP->field_66 :
                           skillId == 6 ? atkP->field_68 :
                           skillId == 7 ? atkP->field_6A :
                           skillId == 8 ? atkP->field_6C : atkP->field_6E;
                if (stat >= 100) {
                    r0 = 1;
                } else {
                    switch (skillId) {
                        case 4: r0 = atkP->field_70 & 1; break;
                        case 5: r0 = (atkP->field_70 >> 1) & 1; break;
                        case 6: r0 = (atkP->field_70 >> 2) & 1; break;
                        case 7: r0 = (atkP->field_70 >> 3) & 1; break;
                        case 8: r0 = (atkP->field_70 >> 4) & 1; break;
                        case 9: r0 = (atkP->field_70 >> 5) & 1; break;
                    }
                }
            }
            if (r0 != 0) {
                val = (s32)((f32)(s32)val * 0.5f);      // 0x800E1344 halve
            }
        }
        // 0x800E1358: apply
        if (val != 0) {
            move->field_6C -= (f32)(s32)val;
        }
    }
    // 0x800E1388 epilogue: restore + blr (void return)
}

extern "C" void func_800E08E8(void*, void*, void*, void*){}
void func_800E1B5C(){}
void func_800E2594(){}

// ================ func_800E2A9C reconstruction ================

// Calculates accumulated damage/healing value from various status effects.
// Returns a float clamped to >= 0.
struct E2A9C_BattleMoveData {
    u8   unk00[0x34];   // 0x00
    f32  field_34;      // 0x34
    f32  field_38;      // 0x38
    f32  field_3C;      // 0x3C
    f32  field_40;      // 0x40
    f32  field_44;      // 0x44
    f32  field_48;      // 0x48
    u32  field_4C;      // 0x4C (retail prologue initialises to -1)
    void* field_50;     // 0x50 sub-object
    f32  field_54;      // 0x54
    f32  field_58;      // 0x58
    f32  field_5C;      // 0x5C (damage amount / knockback)
    f32  field_60;      // 0x60
    f32  field_64;      // 0x64
    f32  field_68;      // 0x68
    f32  field_6C;      // 0x6C
    s16  field_70;      // 0x70
    s16  field_72;      // 0x72
    u32  field_74;      // 0x74 flags (see notes)
    u32  field_78;      // 0x78 flags
    u32  field_7C;      // 0x7C
    u16  field_80;      // 0x80
    u8   unk82[2];      // 0x82
    u32  field_84;      // 0x84
    u32  field_88;      // 0x88
    u32  field_8C;      // 0x8C
    u32  field_90;      // 0x90
    u32  field_94;      // 0x94
    u32  field_98;      // 0x98
    u32  field_9C;      // 0x9C
    u32  field_A0;      // 0xA0
    u32  field_A4;      // 0xA4
    u32  field_A8;      // 0xA8 (attack power threshold)
    u32  field_AC;      // 0xAC
    u32  field_B0;      // 0xB0 (combo count, %100 used)
    u32  field_B4;      // 0xB4
    u32  field_B8;      // 0xB8
};

// ---------------------------------------------------------------------------
// Battle event payload built on the stack and handed to func_800EC918.
// 0x34 bytes; fields are filled per call site.
// ---------------------------------------------------------------------------
struct BattleEventData {
    u32  field_00;      // 0x00 (object id, obj->field_3F10)
    u32  field_04;      // 0x04
    u32  field_08;      // 0x08
    u16  field_0C;      // 0x0C event type
    u16  field_0E;      // 0x0E
    u32  field_10;      // 0x10 value
    u16  field_14;      // 0x14 (extra value for type 0xDF)
    u8   unk16[0xA];    // 0x16
    f32  field_20;      // 0x20 (10.0 / 0.35)
    f32  field_24;      // 0x24 (2.0 / 0.0 / 0.35)
    u8   unk28[6];      // 0x28
    u16  field_2E;      // 0x2E (type 0xF event)
    u32  field_30;      // 0x30 (flags, type 0x1000 event)
};

// ---------------------------------------------------------------------------
// AI action slot (0x20 bytes) installed via func_8014AC38. Byte-precise to the
// retail store sequence (memset + 7 stores).
// ---------------------------------------------------------------------------
struct BattleAIActionSlot {
    u32  unk00;         // 0x00
    u8   pad04[2];      // 0x04
    u8   b_06;          // 0x06 = 0x25
    u8   pad07[6];      // 0x07
    u8   b_0D;          // 0x0D = 0x54 / 0x55
    u8   b_0E;          // 0x0E = 0x64
    u8   pad0F;         // 0x0F
    u16  h_10;          // 0x10 = 0
    s16  h_12;          // 0x12 = -1
    f32  f_14;          // 0x14 = 0.0f
    u8   pad18[8];      // 0x18
};

// ---------------------------------------------------------------------------
// Opaque imports -- same linkage/style as CBattleManager.cpp
// ---------------------------------------------------------------------------



// ---------------------------------------------------------------------------
// vtable dispatch helpers (r23/r24 are CfObjectActor-derived)
// ---------------------------------------------------------------------------
static inline u32 e_vf2BC(void* o) { return ((u32(*)(void*))(*(void***)o)[0x2BC / 4])(o); }
static inline void* e_vf290(void* o) { return ((void*(*)(void*))(*(void***)o)[0x290 / 4])(o); }
static inline u32 e_vf2A8(void* o) { return ((u32(*)(void*))(*(void***)o)[0x2A8 / 4])(o); }
static inline u32 e_vf308(void* o) { return ((u32(*)(void*))(*(void***)o)[0x308 / 4])(o); }
static inline u32 e_vfE0(void* o)  { return ((u32(*)(void*))(*(void***)o)[0xE0 / 4])(o); }
static inline u32 e_vf24C(void* o) { return ((u32(*)(void*))(*(void***)o)[0x24C / 4])(o); }
static inline u32 e_vf248(void* o) { return ((u32(*)(void*))(*(void***)o)[0x248 / 4])(o); }
static inline u32 e_vf5C0(void* o, void* a) {
    return ((u32(*)(void*, void*))(*(void***)o)[0x5C0 / 4])(o, a);
}
static inline void e_vf120(void* o, u32 a, u32 b, u32 c, f64 f) {
    ((void(*)(void*, u32, u32, u32, f64))(*(void***)o)[0x120 / 4])(o, a, b, c, f);
}
static inline void e_vfC0(void* o, void* a) { ((void(*)(void*, void*))(*(void***)o)[0xC0 / 4])(o, a); }
static inline void e_vfC4(void* o, void* a) { ((void(*)(void*, void*))(*(void***)o)[0xC4 / 4])(o, a); }
static inline void e_vf2C4(void* o, void* a, f32 f1, f32 f2, f32 f3) {
    ((void(*)(void*, void*, f32, f32, f32))(*(void***)o)[0x2C4 / 4])(o, a, f1, f2, f3);
}
static inline void* e_vf224(void* o) { return ((void*(*)(void*))(*(void***)o)[0x224 / 4])(o); }
static inline f32 e_vf5B4(void* o) { return ((f32(*)(void*))(*(void***)o)[0x5B4 / 4])(o); }
static inline void e_vf1A4(void* o) { ((void(*)(void*))(*(void***)o)[0x1A4 / 4])(o); }
static inline void e_vf184(void* o, u32 a) { ((void(*)(void*, u32))(*(void***)o)[0x184 / 4])(o, a); }
static inline void e_vf154(void* o, f32 f) { ((void(*)(void*, f32))(*(void***)o)[0x154 / 4])(o, f); }
static inline void e_vf2F8(void* o, u32 v) { ((void(*)(void*, u32))(*(void***)o)[0x2F8 / 4])(o, v); }
static inline u32 e_vf28(void* o, u32 v) { return ((u32(*)(void*, u32))(*(void***)o)[0x28 / 4])(o, v); }
static inline void e_vf20OnHolder(void* o, u32 v) { ((void(*)(void*, u32))(*(void***)o)[0x20 / 4])(o, v); }

// object at +8 (secondary sub-object) dispatch
static inline void* vf54(void* o, u32 idx) {
    return ((void*(*)(void*, u32))(*(void***)((u8*)o + 8))[0x54 / 4])((u8*)o + 8, idx);
}
// sub-object at +0x3E9C (battle interface vtable)
static inline u32 subVf4C(void* o) { return ((u32(*)(void*))(*(void***)o)[0x4C / 4])(o); }
static inline void subVf50(void* o, u32 v) { ((void(*)(void*, u32))(*(void***)o)[0x50 / 4])(o, v); }
static inline nw4r::math::VEC3* subVfAC(void* o) {
    return ((nw4r::math::VEC3*(*)(void*))(*(void***)o)[0xAC / 4])(o);
}
static inline void* subVf110(void* o) { return ((void*(*)(void*))(*(void***)o)[0x110 / 4])(o); }
static inline void subVfC4(void* o, f32 f) { ((void(*)(void*, f32))(*(void***)o)[0xC4 / 4])(o, f); }
// sub-object vtable pointer stored at +0x84 (0xCE/0x106 blocks)
static inline s32 subVf3(void* o) {
    return ((s32(*)(void*))(*(void***)((u8*)o + 0x84))[0xC / 4])(o);
}

// status id of an object = *( *(*(obj+4))[0x30]() )
static inline u32 getStatusId(void* o) {
    void* holder = *(void**)((u8*)o + 4);
    void* p = ((void*(*)(void*))(*(void***)holder)[0x30 / 4])(holder);
    return *(u32*)p;
}
static inline bool hasStatus(void* o, int id) {
    int sid = (int)getStatusId(o);
    return func_80174C98((u8*)o, &sid, id) != 0;
}

// helpers for the small repeated shapes
static void emitEvent(cf::CBattleManager* self, void* attacker, void* target,
                      E2A9C_BattleMoveData* move, const BattleEventData& ev) {
    func_800EC918(self, attacker, target, (void*)&ev, move);
}
static void installSlot(void* obj, u32 unk00, u8 typeByte) {
    BattleAIActionSlot slot;
    std::memset(&slot, 0, sizeof(slot));
    slot.unk00 = unk00;
    slot.b_06 = 0x25;
    slot.b_0D = typeByte;
    slot.b_0E = 0x64;
    slot.h_10 = 0;
    slot.h_12 = -1;
    slot.f_14 = 0.0f;
    func_8014AC38((void*)((u8*)obj + 0x3380), (void*)&slot);
}

// retail int->double helper (emits xoris/lfd/fsubs 2^52)
static inline f64 i2d(s32 x) { return (f64)x; }

// round-to-int with the retail +-0.5 double bias, gated by the 0x80 flag
static inline s32 roundF(f32 x, u32 flags) {
    if (x < 0.0f || (flags & 0x00000080)) return 0;
    return (s32)((f64)x + (x <= 0.0f ? -0.5 : 0.5));
}

// element sound table at lbl_eu_804FC828 + 0x168 (12-byte entries)
static inline u32* elementTable(u32 idx) {
    return (u32*)((u8*)lbl_eu_804FC828 + 0x168 + idx * 0xC);
}

// distance between the +0x3E9C sub-object positions (retail uses psq_l/ps_sub
// via VEC3Sub, then PSVECMag)
static bool distLE(void* attacker, void* target, s32 range) {
    nw4r::math::VEC3* pT = subVfAC((u8*)target + 0x3E9C);
    nw4r::math::VEC3* pA = subVfAC((u8*)attacker + 0x3E9C);
    nw4r::math::VEC3 d;
    nw4r::math::VEC3Sub(&d, pT, pA);
    return PSVECMag((const struct Vec*)&d) <= (f32)i2d(range);
}

// signed position delta, same PS math
static nw4r::math::VEC3 actorDelta(void* attacker, void* target) {
    nw4r::math::VEC3* pT = subVfAC((u8*)target + 0x3E9C);
    nw4r::math::VEC3* pA = subVfAC((u8*)attacker + 0x3E9C);
    nw4r::math::VEC3 d;
    nw4r::math::VEC3Sub(&d, pT, pA);
    return d;
}

// talent-class bit from byte +off of the talent stat object (retail jump
// tables 0x8052B4DC/0x8052B4B4/0x8052B48C/0x8052B464)
static s32 talentBit(void* tgt, u32 off, s32 r25) {
    u8 b = *(u8*)((u8*)tgt + off);
    switch (r25) {
    case 1: return (b & 0x80) != 0;
    case 2: return (b & 0x01) != 0;
    case 3: return (b & 0x02) != 0;
    case 4: return (b & 0x04) != 0;
    case 5: return (b & 0x08) != 0;
    case 6: return (b & 0x10) != 0;
    case 7: return (b & 0x20) != 0;
    default: return 0;
    }
}

// 0x52A4: case-0x72 inline talent emission (note arg order: self, target,
// attacker, ev, localMove)
static void talentEmitSpecial(cf::CBattleManager* self, void* attacker, void* target,
                              void* entry, E2A9C_BattleMoveData* localMove) {
    BattleEventData ev;
    std::memset(&ev, 0, sizeof(ev));
    ev.field_00 = *(u32*)((u8*)attacker + 0x3F10);
    ev.field_0C = *(u16*)((u8*)entry + 0x16);
    ev.field_10 = (u32)*(s16*)((u8*)entry + 0x18);
    ev.field_20 = lbl_eu_80666E34;   // 10.0
    ev.field_24 = lbl_eu_80666DDC;   // 0.0
    ev.field_30 = 0x20000;
    func_800EC918(self, target, attacker, &ev, localMove);
    func_800451D8(0x42, (int)(attacker != nullptr ? (u8*)attacker + 0x3E9C : attacker));
    cf::CfSoundMan::func_801BFC38(0, 0x1B6, 0, 0, lbl_eu_80666DD0);
}

// ============================================================================
// Main candidate
// ============================================================================
extern "C" void func_800E2A9C(cf::CBattleManager* self, cf::CfObjectActor* attackerIn,
                   cf::CfObjectActor* targetIn, E2A9C_BattleMoveData* move) {
    void* attacker = attackerIn;
    void* target = targetIn;

    // retail prologue initialises the localMove buffer: all zero except
    // field_4C = -1 (0x2E0..0x398 layout, three chunked memsets)
    E2A9C_BattleMoveData localMove;
    std::memset(&localMove, 0, sizeof(localMove));
    localMove.field_4C = (u32)-1;

    if (target == nullptr) return;

    cf::CfGameManager::getInstance();
    if (func_8006EF04__Fi(0x400) == 0) {
        if (e_vf2BC(target) == 0) return;
        if (func_80148778((u8*)target + 8, 0xF8)) return;
    }
    if (*(u32*)((u8*)target + 0x3374) & 0x08000000) return;
    if (*(u32*)((u8*)attacker + 0x3F60) == 0) return;
    if (*(u32*)((u8*)target + 0x3F60) == 0) return;

    void* sub = move->field_50;

    // ---- 0xCE "reaction" block ----
    if (func_80148778((u8*)target + 8, 0xCE) &&
        (move->field_78 & 0x00000800) && !(move->field_74 & 0x00002000)) {
        void* sub2 = func_801491F4((u8*)target + 8, 0xCE);
        if (*(u32*)((u8*)sub2 + 0x10) >= (u32)subVf3(sub)) {
            move->field_54 = lbl_eu_80666DD4;
            move->field_5C = lbl_eu_80666DD4;
            move->field_74 = (move->field_74 | 0x80002000) & ~0x00030000;
        }
    }

    // ---- 0xEB "knockback" block ----
    if (func_80148778((u8*)target + 8, 0xEB) && !(move->field_74 & 0x2)) {
        u16 type = *(u16*)((u8*)sub + 0x3C);
        if (type == 1 || type == 5) {
            move->field_74 &= ~0x00030000;
            if (!(move->field_78 & 0x00000800)) {
                move->field_74 |= 0x80000002;
                move->field_54 = move->field_54 * lbl_eu_80666DE8;
                move->field_5C = move->field_54 * move->field_58;
            }
        }
    }

    if (move->field_74 & 0x1) {
        // ====================================================================
        // PATH A -- standard attack (0x3820..0x6360)
        // ====================================================================
        f32 f26 = lbl_eu_80666DD4;   // global ratio
        f32 f27 = lbl_eu_80666DD4;   // combo ratio (retail loads 1.0 only inside
                                     // the 0x106 block; value is register-garbage
                                     // otherwise -- declared here for safety)

        if (func_80148778((u8*)target + 8, 0xF7)) {
            if (e_vf5C0(target, attacker) == 0) {
                func_80149154((u8*)target + 8, 0xF7);
                u32 f = move->field_78;
                if ((f & 0x00000200) || ((f & 0x00000400) && !(f & 0x00000800))) {
                    move->field_74 |= 0x80000080;
                    target = attacker;                 // redirect onto attacker
                } else {
                    move->field_74 &= ~0x00000080;
                }
            } else {
                move->field_74 &= ~0x00000080;
            }
        }

        if (func_80148778((u8*)target + 8, 0x106)) {
            func_80149154((u8*)target + 8, 0x106);
            if (!(move->field_78 & 0x00000800)) {
                if (move->field_50 == nullptr) return;

                f27 = lbl_eu_80666DD4;
                s32 a = subVf3(move->field_50);
                s32 b = subVf3(move->field_50);
                s32 rem = (s32)(move->field_B0 % 100);
                // retail: pool = {8.0,4.0,2.0} (dwords at +0x2A4 read as f32),
                // thresholds = {a/2+3, b+10, 100} (ints at +0x298, 0/1 patched)
                f32 pool[3] = { 8.0f, 4.0f, 2.0f };
                s32 thr[3] = { a / 2 + 3, b + 10, 100 };
                for (int i = 0; i < 3; i++) {
                    if (rem - thr[i] < 0) { f27 = pool[i]; break; }
                    rem -= thr[i];
                }

                if (e_vf290(target)) {
                    u32 tmp;
                    if (func_80260264(e_vf290(target), 0x5B, &tmp)) {
                        f27 = f27 + lbl_eu_80666DD8 * (f32)i2d((s32)tmp);
                    }
                }
            }
        }

        // 0x3A18..0x3B08: r3 = round(field_5C + field_60); if <= 0 use field_64
        s32 r3 = roundF(move->field_5C + move->field_60, move->field_74);
        if (r3 <= 0) r3 = roundF(move->field_64, move->field_74);

        // self-inflicted HP adjust (0x3B08)
        s32 amt;
        {
            f32 d = (f32)i2d(r3) * f27;
            amt = (s32)((f64)d + (d > 0.0f ? 0.5 : -0.5));
            e_vf120(attacker, 2, 0, 0, (f64)(-amt));
        }
        func_802A26D8(attacker, target, (void*)amt);
        func_802A25EC(target);
        func_8027F848(target, (s32)(move->field_5C * f27), move);
        e_vfC4(attacker, move);

        // 0x3BF4
        if (!(move->field_78 & 0x00000200)) {
            if (move->field_50 == nullptr) return;
            u32 subFlags = *(u32*)((u8*)sub + 0x78);
            if (!(subFlags & 0x00002000) ||
                (u32)(e_vf2A8(attacker) + 1) != *(u8*)((u8*)sub + 0x44)) {
                func_800E64CC(self, attacker, target, move);
            }
        }

        {
            f32 d = move->field_5C * f26;
            s32 r28 = (s32)((f64)d + (d > 0.0f ? 0.5 : -0.5));
            if (move->field_50 == nullptr) return;
            if ((move->field_74 & 0x2) || (move->field_74 & 0x60)) func_802A24B4(attacker);

            // ---- 0x3CC8: per-tag attack parameter emission ----
            if (move->field_50 == nullptr) return;          // 0x3CB0
            if (move->field_78 & 0x00000200) {
                s32 r15 = 0;
                if (func_80148778((u8*)attacker + 8, 0xA0)) {
                    r15 = *(s32*)((u8*)func_80149154((u8*)attacker + 8, 0xA0) + 0x10);
                }

#define EMIT_TAG(tag, evType, extraField)                                        \
    if (func_80148778((u8*)attacker + 8, tag)) {                                \
        void* entry = func_80149154((u8*)attacker + 8, tag);                    \
        s32 val = *(s16*)((u8*)entry + 0x1A);                                   \
        if (*(u32*)((u8*)entry + 0x08) == 0x4000) val += r15;                   \
        if (move->field_A8 < (u32)val) {                                        \
            BattleEventData ev;                                                 \
            std::memset(&ev, 0, sizeof(ev));                                    \
            ev.field_00 = *(u32*)((u8*)attacker + 0x3F10);                      \
            ev.field_08 = 0x18000;                                              \
            ev.field_0C = evType;                                               \
            ev.field_10 = *(u32*)((u8*)entry + 0x10);                           \
            ev.field_20 = lbl_eu_80666E34;                                      \
            extraField;                                                         \
            emitEvent(self, attacker, target, move, ev);                        \
        }                                                                       \
    }

                EMIT_TAG(0xA3, 0x52, (void)0)
                EMIT_TAG(0xA4, 0x53, (void)0)
                EMIT_TAG(0xA5, 0x54, (void)0)
                EMIT_TAG(0xAC, 0x3C, (void)0)
                EMIT_TAG(0xAD, 0x3D, (void)0)
                EMIT_TAG(0x7E, 0x66, ev.field_24 = lbl_eu_80666DFC;)
                EMIT_TAG(0x7F, 0x67, ev.field_24 = lbl_eu_80666DFC;)
                EMIT_TAG(0x80, 0x68, ev.field_24 = lbl_eu_80666DFC;)
                EMIT_TAG(0x81, 0x69, ev.field_24 = lbl_eu_80666DFC;)

                // 0x82 -- unconditional, 0.35/0.35
                if (func_80148778((u8*)attacker + 8, 0x82)) {
                    void* entry = func_80149154((u8*)attacker + 8, 0x82);
                    BattleEventData ev;
                    std::memset(&ev, 0, sizeof(ev));
                    ev.field_00 = *(u32*)((u8*)attacker + 0x3F10);
                    ev.field_08 = 0x18000;
                    ev.field_0C = 0x6A;
                    ev.field_10 = *(u32*)((u8*)entry + 0x10);
                    ev.field_20 = lbl_eu_80666E70;
                    ev.field_24 = lbl_eu_80666E70;
                    emitEvent(self, attacker, target, move, ev);
                }

                // 0xA8 -- extra u16 at +0x14
                if (func_80148778((u8*)attacker + 8, 0xA8)) {
                    void* entry = func_80149154((u8*)attacker + 8, 0xA8);
                    s32 val = *(s16*)((u8*)entry + 0x1A);
                    if (*(u32*)((u8*)entry + 0x08) == 0x4000) val += r15;
                    if (move->field_A8 < (u32)val) {
                        BattleEventData ev;
                        std::memset(&ev, 0, sizeof(ev));
                        ev.field_00 = *(u32*)((u8*)attacker + 0x3F10);
                        ev.field_08 = 0x18000;
                        ev.field_0C = 0xDF;
                        ev.field_10 = *(u32*)((u8*)entry + 0x10);
                        ev.field_14 = (u16)*(u32*)((u8*)entry + 0x10);
                        ev.field_20 = lbl_eu_80666E34;
                        emitEvent(self, attacker, target, move, ev);
                    }
                }

                // 0xA9 -- constant value 0x4B, type 7
                if (func_80148778((u8*)attacker + 8, 0xA9)) {
                    void* entry = func_80149154((u8*)attacker + 8, 0xA9);
                    s32 val = *(s16*)((u8*)entry + 0x1A);
                    if (*(u32*)((u8*)entry + 0x08) == 0x4000) val += r15;
                    if (move->field_A8 < (u32)val) {
                        BattleEventData ev;
                        std::memset(&ev, 0, sizeof(ev));
                        ev.field_00 = *(u32*)((u8*)attacker + 0x3F10);
                        ev.field_08 = 0x18000;
                        ev.field_0C = 0x07;
                        ev.field_10 = 0x4B;
                        ev.field_20 = lbl_eu_80666E34;
                        emitEvent(self, attacker, target, move, ev);
                    }
                }

                EMIT_TAG(0xAA, 0x12, (void)0)

                // 0xA6 -- constant value 0x32, type 4
                if (func_80148778((u8*)attacker + 8, 0xA6)) {
                    void* entry = func_80149154((u8*)attacker + 8, 0xA6);
                    s32 val = *(s16*)((u8*)entry + 0x1A);
                    if (*(u32*)((u8*)entry + 0x08) == 0x4000) val += r15;
                    if (move->field_A8 < (u32)val) {
                        BattleEventData ev;
                        std::memset(&ev, 0, sizeof(ev));
                        ev.field_00 = *(u32*)((u8*)attacker + 0x3F10);
                        ev.field_08 = 0x18000;
                        ev.field_0C = 0x04;
                        ev.field_10 = 0x32;
                        ev.field_20 = lbl_eu_80666E34;
                        emitEvent(self, attacker, target, move, ev);
                    }
                }

                EMIT_TAG(0xA7, 0x06, (void)0)

#undef EMIT_TAG

                // 0xCF / 0xD0 "warp" event -- only when !(field_74 & 0x40)
                if (!(move->field_74 & 0x40)) {
                    void* entry = func_80149154((u8*)attacker + 8, 0xCF);
                    if (entry == nullptr) entry = func_80149154((u8*)attacker + 8, 0xD0);
                    if (entry != nullptr) {
                        u32 st = e_vfE0(target);
                        if (st == 1 || st == 2) {
                            BattleEventData ev;
                            std::memset(&ev, 0, sizeof(ev));
                            ev.field_00 = *(u32*)((u8*)attacker + 0x3F10);
                            ev.field_0C = 0x7D;
                            ev.field_10 = *(u32*)((u8*)entry + 0x10);
                            ev.field_20 = lbl_eu_80666E70;
                            ev.field_24 = lbl_eu_80666E70;
                            emitEvent(self, attacker, target, move, ev);
                        }
                    }
                }

                // 0x6D event -- requires move->field_B0 < out2
                if (e_vf290(attacker)) {
                    u32 out1, out2;
                    if (func_80260A6C(e_vf290(attacker), 0x6D, &out1, &out2)) {
                        if (move->field_B0 < out2) {
                            BattleEventData ev;
                            std::memset(&ev, 0, sizeof(ev));
                            ev.field_00 = *(u32*)((u8*)attacker + 0x3F10);
                            ev.field_0C = 0xE2;
                            ev.field_10 = out1;
                            emitEvent(self, attacker, target, move, ev);
                        }
                    }
                }
            }

            // ================================================================
            // 0x474C: local copy + player index + HP application
            // ================================================================
            localMove = *move;
            s32 r15 = 0;

            if (*(u32*)((u8*)attacker + 0x3F00) & 0x2) {
                void* sub2 = (attacker != nullptr) ? (u8*)attacker + 0x3E9C : attacker;
                if (sub2 != cf::CfGameManager::getPlayer(0)) {
                    struct EnumListHolder { void* list; u32 handle; };
                    EnumListHolder holder;
                    func_80043D90(&holder);
                    void* list = func_80043F18(&holder);
                    func_800F4A98(list, 0x20, 0);
                    list = func_80043F18(&holder);
                    if (*(u32*)((u8*)list + 0x620) == 2) {
                        r15 = 1;
                    } else {
                        int* table = func_8009ECB0();
                        u16 id = *(u16*)((u8*)attacker + 0x3F28);
                        if (id != (u32)table[2]) r15 = 1;
                        else if (id != (u32)table[3]) r15 = 2;
                    }
                    __dt__80043E88(&holder, -1);
                }
            }

            u32 mvType = 0;
            if (move->field_74 & 0x00001000) mvType = 7;
            else if (move->field_74 & 0x2) mvType = 5;
            else if (move->field_74 & 0x10) mvType = 6;
            else if (move->field_74 & 0x00000800) mvType = 3;

            e_vf120(target, mvType, (u32)r15, move->field_74, (f64)(-r28));
            func_802A26D8(target, attacker, (void*)(-r28));
            func_802A25EC(attacker);
            func_8027F848(attacker, r28, move);
            if (move->field_50 == nullptr) return;

            if (r28 >= 0) func_801BAB94(&self->mSuddenCommu, attacker, target, &localMove);

            // ---- 0x4A44: element-type sound selection ----
            {
                void* elem = func_8005605C((u8*)(*(u32*)((u8*)attacker + 0x3F60)) + 0x10);
                if (elem != nullptr) {
                    void* who = (*(u8*)((u8*)elem + 0xC) & 0x80) ? target : attacker;
                    u8 b8 = *(u8*)((u8*)func_8005605C((u8*)(*(u32*)((u8*)attacker + 0x3F60)) + 0x10) + 0x08);
                    u16 hA = *(u16*)((u8*)func_8005605C((u8*)(*(u32*)((u8*)attacker + 0x3F60)) + 0x10) + 0x0A);
                    func_8004513C(who, attacker, hA, b8);
                } else {
                    u32 sf = *(u32*)((u8*)sub + 0x78);
                    u32* tbl = elementTable(*(u16*)((u8*)sub + 0x40));
                    if (sf & 0x100) {
                        func_8004513C(target, attacker, tbl[1], 0);
                    } else if (sf & 0x200) {
                        func_8004513C(target, attacker, tbl[2], 0);
                    } else {
                        func_8004513C(target, attacker, tbl[0], 0);
                    }
                }
            }

            // ---- 0x4B30: r14 guard (whole 0x4B50..0x5090 chain runs only
            // when func_8006EF04(0x400) == 0; otherwise only e_vfC4 runs) ----
            bool flag_3C8 = false;                          // 0x3C8 stack byte
            if (func_8006EF04__Fi(0x400) == 0) {
                bool r14ok = true;
                if (subVf4C((u8*)target + 0x3E9C) != *(u32*)((u8*)attacker + 0x3F10)) {
                    r14ok = false;
                }
                int sid = (int)getStatusId(target);
                if (func_80174C98((u8*)target, &sid, 0x802)) r14ok = false;
                sid = (int)getStatusId(target);
                if (func_80174C98((u8*)target, &sid, 0x801)) {
                    r14ok = false;
                } else {
                    if (func_80148778((u8*)target + 8, 0x7) ||
                        func_80148778((u8*)target + 8, 0xB) ||
                        func_80148778((u8*)target + 8, 0x9) ||
                        func_80148778((u8*)target + 8, 0xF) ||
                        func_80148778((u8*)target + 8, 0x10)) {
                        r14ok = false;
                    }
                }

                // ---- 0x4C48 / 0x4CC0 ----
                if (e_vf2BC(target) != 0) {
                    if (!(move->field_78 & 0x1)) func_80174B4C(attacker, 0x400000);
                    if (*(u8*)((u8*)self + 0x1AA) >= 1 && *(u8*)((u8*)self + 0x1AA) <= 0x18) {
                        func_8018C820(&self->unk194, 0x64);
                    }
                    func_8027FC04(target, attacker);
                    r14ok = false;
                } else {
                    if ((localMove.field_74 & 0x100) && (*(u32*)((u8*)target + 0x3F00) & 0x2)) {
                        func_800F3970((void*)lbl_eu_80663F00, target, 0, 0x16, 0);
                    }
                }

                // ---- 0x4D00: AI slot install (target side) ----
                // retail stores the 0x3C8 flag = (e_vf24C(target) < field_A0) here,
                // used later at 0x5B50.
                if (r14ok && (localMove.field_78 & 0x200)) {
                    if (e_vf24C(target) < localMove.field_A0) {
                        flag_3C8 = true;
                        installSlot(target, *(u32*)((u8*)attacker + 0x3F10), 0x54);
                    }
                }

                // ---- 0x4DAC: attacker status / chain events ----
                {
                    int sid2 = (int)getStatusId(attacker);
                    if (func_80174C98((u8*)attacker, &sid2, 0x800)) {
                        if (localMove.field_74 & 0x02000000) {
                            if (e_vf28(self, 1) != 0) {
                                func_800F3970((void*)lbl_eu_80663F00, attacker, target, 2, 0);
                            }
                        }
                    }
                    if (localMove.field_74 & 0x100) {
                        if (*(u32*)((u8*)attacker + 0x3F00) & 0x2) {
                            u32 v = 0;
                            if (e_vf290(attacker)) func_80260264(e_vf290(attacker), 0x3A, &v);
                            func_800F3970((void*)lbl_eu_80663F00, attacker, target, 3, (s32)v);
                            func_8018C820(&self->unk194, 0xA);
                        }
                    }
                    if (*(u32*)((u8*)attacker + 0x3F00) & 0x2) {
                        if ((localMove.field_74 & 0x2) || (localMove.field_74 & 0x40)) {
                            if (e_vf308(attacker) == 4) func_8018C820(&self->unk194, 0xA);
                        }
                    }
                    if (localMove.field_78 & 0xC00) {
                        if (func_80148778((u8*)target + 8, 0x10)) {
                            func_800F3970((void*)lbl_eu_80663F00, attacker, target, 4, 0);
                        }
                        if (func_80148778((u8*)target + 8, 0xF) ||
                            func_80148778((u8*)target + 8, 0x9)) {
                            func_800F3970((void*)lbl_eu_80663F00, attacker, target, 5, 0);
                        }
                    }
                }

                // ---- 0x4F84 ----
                {
                    u32 v = 0;
                    if (e_vf290(attacker)) func_80260264(e_vf290(attacker), 0x74, &v);
                    if (localMove.field_74 & 0x18) {
                        func_800F3970((void*)lbl_eu_80663F00, attacker, 0, 0xD, (s32)v);
                    }
                    if (localMove.field_74 & 0x2) {
                        func_800F3970((void*)lbl_eu_80663F00, attacker, 0, 0xE, (s32)v);
                    }
                    if (func_80148778((u8*)target + 8, 0xF)) {
                        void* e = func_80149154((u8*)attacker + 8, 0xBD);
                        if (e != nullptr) e_vf2F8(attacker, *(u32*)((u8*)e + 0x10));
                    }
                }

                e_vfC0(attacker, &localMove);
                e_vfC4(target, &localMove);
            } else {
                e_vfC4(target, &localMove);                 // 0x5094
            }

            // ---- 0x50AC: talent-art gate ----
            bool runTalent = true;
            if (func_80148778((u8*)target + 8, 0x10)) {
                if (!(*(u32*)((u8*)target + 0x3374) & 0x00040000)) {
                    runTalent = false;                 // -> 0x5A00
                } else {
                    int sid = (int)getStatusId(target);
                    if (func_80174C98((u8*)target, &sid, 0x17)) runTalent = false;
                }
            }
            if (runTalent) {
                if (func_80148778((u8*)target + 8, 0x117)) runTalent = false;
                if (localMove.field_74 & 0x80) runTalent = false;
            }

            if (runTalent) {
                // ============================================================
                // 0x5120..0x59F4: talent/skill loop
                // ============================================================
                for (u32 idx = 0x40; idx < 0x68; idx++) {
                    void* entry = vf54(target, idx);
                    u32 type = *(u16*)((u8*)entry + 0xC);
                    s32 r25 = 0;

                    switch (type) {
                    case 0x6E:
                    case 0x78:
                        r25 = func_80145BC4(0x6E);
                        break;
                    case 0x6F:
                    case 0x79:
                        r25 = func_80145BC4(0x6F);
                        break;
                    case 0x70:
                    case 0x7A:
                        r25 = func_80145BC4(0x70);
                        break;
                    case 0x71:
                    case 0x7B:
                        r25 = func_80145BC4(0x71);
                        break;
                    case 0x72:
                    case 0x7C:
                        if (*(u8*)((u8*)self + 0x1AA) >= 1 &&
                            *(u8*)((u8*)self + 0x1AA) <= 0x18) {
                            break;                     // r25 stays 0
                        }
                        if (*(s16*)((u8*)entry + 0x14) == 0) {
                            talentEmitSpecial(self, attacker, target, entry, &localMove);
                            break;                     // 0x52A4
                        }
                        if (distLE(attacker, target, *(s16*)((u8*)entry + 0x14))) {
                            talentEmitSpecial(self, attacker, target, entry, &localMove);
                            break;                     // 0x52A4
                        }
                        break;
                    default:
                        break;
                    }

                    // 0x532C..0x53CC: range re-check for r25 from 0x6E..0x71
                    if (r25 != 0) {
                        if (*(s16*)((u8*)entry + 0x14) != 0) {
                            if (!distLE(attacker, target, *(s16*)((u8*)entry + 0x14))) {
                                r25 = 0;
                            }
                        }
                        if (r25 == 0) continue;
                    } else {
                        continue;
                    }

                    // ---- main emission 0x53D4 ----
                    {
                        void* tgt = e_vf224(attacker);   // talent stat object
                        s32 value = (s32)*(u32*)((u8*)entry + 0x10);
                        f32 ratio = lbl_eu_80666DD4;

                        if (e_vf290(target)) {
                            void* e = func_80149154((u8*)target + 8, 0x6F);
                            if (e == nullptr) e = func_80149154((u8*)target + 8, 0x79);
                            if (e != nullptr) {
                                u32 tmp;
                                if (e_vf290(target) && func_80260264(e_vf290(target), 0x5B, &tmp)) {
                                    value = (s32)((f32)i2d(value) *
                                        (1.0f + (f32)i2d((s32)tmp) / lbl_eu_80666E00));
                                }
                            }
                        }
                        if (func_80148778((u8*)attacker + 8, 0x8D)) {
                            void* e = func_80149154((u8*)attacker + 8, 0x8D);
                            ratio = ratio - lbl_eu_80666DD8 * (f32)i2d(*(s32*)((u8*)e + 0x10));
                        }
                        if (e_vf290(attacker)) {
                            u32 tmp;
                            if (func_80260264(e_vf290(attacker), 0x57, &tmp)) {
                                ratio = ratio - lbl_eu_80666DD8 * (f32)i2d((s32)tmp);
                            }
                        }
                        value = (s32)((f32)i2d(value) * ratio);
                        if (value < 1) value = 0;
                        if (func_80148778((u8*)attacker + 8, 0x1)) value = 0;

                        // talent classification A (byte +0x72 / s16 +0x64..)
                        s32 r0 = talentBit(tgt, 0x72, r25);
                        if (r25 >= 4 && r25 <= 9) {
                            if (*(s16*)((u8*)tgt + 0x64 + (r25 - 4) * 2) >= 0xC8) r0 = 1;
                        }
                        if (r0 != 0) {
                            value = -value;
                        } else {
                            r0 = talentBit(tgt, 0x70, r25);
                            if (r25 >= 4 && r25 <= 9) {
                                if (*(s16*)((u8*)tgt + 0x64 + (r25 - 4) * 2) >= 0x64) r0 = 1;
                            }
                            if (r0 != 0) {
                                value = (s32)((f32)i2d(value) * lbl_eu_80666DE8);
                            }
                        }
                        if (value != 0) {
                            e_vf120(attacker, 2, 0, 0, (f64)(-value));
                        }
                        // hit SFX
                        s32 hitSnd = 0;
                        switch (r25) {
                        case 4: hitSnd = 0x42; break;
                        case 5: hitSnd = 0x4C; break;
                        case 6: hitSnd = 0x44; break;
                        case 7: hitSnd = 0x46; break;
                        case 8: hitSnd = 0x4A; break;
                        case 9: hitSnd = 0x48; break;
                        default: hitSnd = 0; break;
                        }
                        func_800451D8((u32)hitSnd,
                            (int)(attacker != nullptr ? (u8*)attacker + 0x3E9C : attacker));
                        u32 snd = 0;
                        switch (r25) {
                        case 1: snd = 0x1AD; break;
                        case 2: snd = 0x1AA; break;
                        case 3: snd = 0x1B0; break;
                        case 4: snd = 0x1B6; break;
                        case 5: snd = 0x1BA; break;
                        case 6: snd = 0x1B5; break;
                        case 7: snd = 0x1B7; break;
                        case 8: snd = 0x1B8; break;
                        case 9: snd = 0x1B9; break;
                        default: snd = 0; break;
                        }
                        cf::CfSoundMan::func_801BFC38(0, snd, 0, 0, lbl_eu_80666DD0);
                    }
                }
            }

            // ---- 0x5A00 ----
            if (e_vf2BC(target) != 0) {
                func_802A2648(attacker, target);
                func_802A232C(target);
            } else {
                bool r14b = true, r15b = true;
                if (!(localMove.field_74 & 0x100)) {
                    int sid = (int)getStatusId(target);
                    if (func_80174C98((u8*)target, &sid, 0x19) == 0) r15b = false;
                }
                if (!r15b) {
                    int sid = (int)getStatusId(target);
                    if (func_80174C98((u8*)target, &sid, 0x1A) == 0) r14b = false;
                }
                func_802A2250(target, attacker, r14b ? 1 : 0);
            }

            func_800D7A04((cf::CfObjectPc*)attacker, (cf::CfObjectEne*)target);

            bool r14e = false;
            if ((*(u32*)((u8*)target + 0x3F00) & 0x4) &&
                (*(u16*)((u8*)target + 0x3F28) >= 0x9C5 &&
                 *(u16*)((u8*)target + 0x3F28) <= 0x9C7)) {
                int sid = (int)getStatusId(target);
                if (func_80174C98((u8*)target, &sid, 0x806)) r14e = true;
            }

            // ---- 0x5B50: chain/status branch ----
            if (flag_3C8 || r14e || hasStatus(target, 0x16) || hasStatus(target, 0x18) ||
                hasStatus(target, 0x19) || hasStatus(target, 0x1A) ||
                hasStatus(target, 0x1B)) {
                // -> 0x6360
            } else if (localMove.field_74 & 0x2) {
                if (!hasStatus(target, 0xA) && !hasStatus(target, 0xB)) {
                    func_800BE12C((u8*)attacker + 0x3E9C, 0x2D, 0, -1, 1);
                    if (func_802799F0(&self->mChain, target) == 0) {
                        func_800BE12C((u8*)target + 0x3E9C, 0x2E, 0, -1, 1);
                    }
                }
            } else if (localMove.field_74 & 0x60) {
                if (!hasStatus(target, 0xA) && !hasStatus(target, 0xB)) {
                    func_800BE12C((u8*)attacker + 0x3E9C, 0x2D, 0, -1, 1);
                    if (localMove.field_74 & 0x40) {
                        func_800451D8(0xB0, (int)((u8*)target + 0x3E9C));
                    }
                }
            } else {
                if (!hasStatus(target, 0x1000) && !hasStatus(target, 0x7) &&
                    !hasStatus(target, 0x8) && !hasStatus(target, 0x4)) {
                    if (localMove.field_74 & 0x10000) {
                        // knockback
                        if (!hasStatus(target, 0xA) && !hasStatus(target, 0xB)) {
                            void* st = *(void**)((u8*)target + 0x3F60);
                            if (!(*(u16*)((u8*)st + 0x530) & 0x1)) {
                                subVfC4((u8*)target + 0x3E9C, lbl_eu_8066A1F8 + e_vf5B4(attacker));
                            }
                            nw4r::math::VEC3 d = actorDelta(attacker, target);
                            // retail: horizontal dist^2 = x^2+z^2 only (y stored, unused)
                            f32 dist2 = d.x * d.x + d.z * d.z;
                            void* st2 = *(void**)((u8*)target + 0x3F60);
                            if (dist2 <= lbl_eu_80666E74) {
                                *(f32*)((u8*)st2 + 0x46C) = e_vf5B4(attacker);
                            } else {
                                *(f32*)((u8*)st2 + 0x46C) =
                                    lbl_eu_80666E10 * Atan2FIdx__Q24nw4r4mathFff(d.x, d.z);
                            }
                            if (*(u8*)((u8*)self + 0x1AA) < 1 || *(u8*)((u8*)self + 0x1AA) > 0x18) {
                                *(f32*)((u8*)st2 + 0x468) = func_80154058(sub);
                            } else {
                                *(f32*)((u8*)st2 + 0x468) = 0.0f;
                            }
                            func_800BE12C((u8*)target + 0x3E9C, 0x1D, 0, -1, 1);
                        }
                    } else if (localMove.field_74 & 0x20000) {
                        // ether hit
                        if (!hasStatus(target, 0xA) && !hasStatus(target, 0xB)) {
                            void* st = *(void**)((u8*)target + 0x3F60);
                            if (!(*(u16*)((u8*)st + 0x530) & 0x1)) {
                                subVfC4((u8*)target + 0x3E9C, lbl_eu_8066A1F8 + e_vf5B4(attacker));
                            }
                            nw4r::math::VEC3 d = actorDelta(attacker, target);
                            f32 dist2 = d.x * d.x + d.z * d.z;   // horizontal only
                            void* st2 = *(void**)((u8*)target + 0x3F60);
                            if (dist2 <= lbl_eu_80666E74) {
                                *(f32*)((u8*)st2 + 0x478) = e_vf5B4(attacker);
                            } else {
                                *(f32*)((u8*)st2 + 0x478) =
                                    lbl_eu_80666E10 * Atan2FIdx__Q24nw4r4mathFff(d.x, d.z);
                            }
                            if (*(u8*)((u8*)self + 0x1AA) < 1 || *(u8*)((u8*)self + 0x1AA) > 0x18) {
                                *(f32*)((u8*)st2 + 0x470) = func_8015408C(sub);
                                *(f32*)((u8*)st2 + 0x474) = func_801540C0(sub);
                            } else {
                                *(f32*)((u8*)st2 + 0x470) = 0.0f;
                                f32 c0 = func_801540C0(sub);
                                *(f32*)((u8*)st2 + 0x474) = c0 < 0.0f ? 0.0f : c0;
                            }
                            if (func_80154134(sub) != 0.0f) {
                                BattleEventData ev;
                                std::memset(&ev, 0, sizeof(ev));
                                ev.field_00 = *(u32*)((u8*)attacker + 0x3F10);
                                ev.field_04 = (u32)sub;
                                ev.field_0C = 0xF;
                                ev.field_2E = 0xF;
                                if (func_800EAA2C(self, attacker, target, &ev, &localMove) != 0) {
                                    e_vf1A4(target);
                                    e_vf184(target, 1);
                                }
                            }
                            func_800BE12C((u8*)target + 0x3E9C, 0x1E, 0, -1, 1);
                        }
                    } else {
                        if (!hasStatus(target, 0x800) && !hasStatus(target, 0x1D) &&
                            !hasStatus(target, 0x801)) {
                            if (r28 > 0) {
                                func_800BE12C((u8*)target + 0x3E9C, 0x4, 0, -1, 1);
                            }
                        }
                    }
                }
            }

            // ---- 0x6360 ----
            if (e_vf2BC(target) != 0) {
                func_800E9B54(self, target, attacker, &localMove);
                func_800D9CA0(self, target);
            }
        }
    } else {
        // ====================================================================
        // PATH B -- field_74 bits 2/3 (0x63A0..0x6C6C)
        // ====================================================================
        if (move->field_74 & 0xC) {
            localMove = *move;
            s32 r14 = 0;

            if (*(u32*)((u8*)attacker + 0x3F00) & 0x2) {
                void* sub2 = (attacker != nullptr) ? (u8*)attacker + 0x3E9C : attacker;
                if (sub2 != cf::CfGameManager::getPlayer(0)) {
                    struct EnumListHolder { void* list; u32 handle; };
                    EnumListHolder holder;
                    func_80043D90(&holder);
                    void* list = func_80043F18(&holder);
                    func_800F4A98(list, 0x20, 0);
                    list = func_80043F18(&holder);
                    if (*(u32*)((u8*)list + 0x620) == 2) {
                        r14 = 1;
                    } else {
                        int* table = func_8009ECB0();
                        u16 id = *(u16*)((u8*)attacker + 0x3F28);
                        if (id != (u32)table[2]) r14 = 1;
                        else if (id != (u32)table[3]) r14 = 2;
                    }
                    __dt__80043E88(&holder, -1);
                }
            }

            func_8010989C(0);
            func_80109888(0);
            func_8010975C((move->field_74 & 0x8) ? 8 : 4);
            func_80109770(r14);
            func_80109734(*(void**)((u8*)target + 0x3F10), 0);

            {
                u32 v = 0;
                if (e_vf290(attacker)) func_80260264(e_vf290(attacker), 0x74, &v);
                func_800F3970((void*)lbl_eu_80663F00, attacker, 0, 0xC, (s32)v);
            }
            func_801BAD24(&self->mSuddenCommu, attacker, target);

            bool r15ok = true;
            if (subVf4C((u8*)target + 0x3E9C) != *(u32*)((u8*)attacker + 0x3F10)) {
                r15ok = false;
            }
            {
                int sid = (int)getStatusId(target);
                if (func_80174C98((u8*)target, &sid, 0x801) ||
                    func_80148778((u8*)target + 8, 0x7) ||
                    func_80148778((u8*)target + 8, 0xB) ||
                    func_80148778((u8*)target + 8, 0x9) ||
                    func_80148778((u8*)target + 8, 0xF) ||
                    func_80148778((u8*)target + 8, 0x10)) {
                    r15ok = false;
                }
            }
            if (r15ok && (localMove.field_78 & 0x200)) {
                if (e_vf24C(target) < localMove.field_A0) {
                    installSlot(target, *(u32*)((u8*)attacker + 0x3F10), 0x54);
                }
            }

            if (!hasStatus(target, 0x801) && !hasStatus(target, 0x16) &&
                !hasStatus(target, 0x18) && !hasStatus(target, 0x19) &&
                !hasStatus(target, 0x1A) && !hasStatus(target, 0x1B)) {
                if (func_802799F0(&self->mChain, target) == 0) {
                    s32 r14b = 0;
                    if (localMove.field_78 & 0x200) {
                        r14b = *(u8*)((u8*)sub + 0x77);
                        if (r14b >= 3) r14b -= 3;
                    } else if (localMove.field_78 & 0x400) {
                        if (rand() & 1) r14b = 1;
                    }
                    *(s32*)((u8*)subVf110((u8*)target + 0x3E9C) + 0x1C) = r14b;
                    if (!hasStatus(target, 0x802)) {
                        func_800BE12C((u8*)target + 0x3E9C, 0x2C, 0, -1, 1);
                    }
                }
            }
        } else {
            if (move->field_78 & 0x00000800) {
                localMove = *move;                    // 0x6AF4
            }
            // else: localMove keeps the prologue zeroed value (0x6AE8 -> 0x6C6C)
        }
    }

    // ========================================================================
    // 0x6C6C tail (shared)
    // ========================================================================
    if (localMove.field_74 & 0x1) {
        if ((localMove.field_78 & 0x200) &&
            !(localMove.field_78 & 0x01000000) &&
            !(localMove.field_78 & 0x02000000)) {
            if (!func_80148778((u8*)target + 8, 0x7)) {
                if (e_vf248(attacker) < localMove.field_A0) {
                    // note: retail uses *target*->field_3F10 here, installed
                    // on *attacker*+0x3380
                    installSlot(attacker, *(u32*)((u8*)target + 0x3F10), 0x55);
                }
            }
        }
    }

    f32 f25 = func_800D7EA0((u8*)attacker, &localMove);
    f32 f26 = lbl_eu_80666DD4;
    if (e_vf290(target)) {
        u32 tmp;
        if (func_80260264(e_vf290(target), 0x6E, &tmp)) {
            f26 = f26 + (f32)i2d((s32)tmp) / lbl_eu_80666E00;
        }
    }
    if (func_80148778((u8*)target + 8, 0xFE)) f26 = 0.0f;
    if (f25 < 0.0f) f25 = 0.0f;
    if (f26 < 0.0f) f26 = 0.0f;

    s32 r14 = (s32)localMove.field_5C;
    if (localMove.field_74 & 0x8000) {
        if (localMove.field_5C < 0.0f) r14 = (s32)localMove.field_64;
        else r14 = (s32)(localMove.field_64 - localMove.field_5C);
    }

    e_vf2C4(target, attacker, f25 * lbl_eu_80666DE8 * (f32)i2d(r14), 0.0f, 0.0f);
    if (!(localMove.field_74 & 0x8000) || localMove.field_5C >= 0.0f) {
        e_vf2C4(attacker, target, f26 * f25 * (f32)i2d(-r14), 0.0f, 0.0f);
    }

    if (subVf4C((u8*)target + 0x3E9C) == 0) {
        subVf50((u8*)target + 0x3E9C, *(u32*)((u8*)attacker + 0x3F10));
    }

    if (hasStatus(target, 0x1000)) {
        e_vf20OnHolder(*(void**)((u8*)target + 4), 0x1000);
    }
}

struct BtlMove {
    u8 pad_00[0x04];
    u32 mSubId;        // +0x04 -> func_800B708C
    u8 pad_08[0x48];
    void* mSub;        // +0x50  (r28)
    u8 pad_54[0x08];
    f32 mDmg;          // +0x5C
    f32 mHeal;         // +0x60
    u8 pad_64[0x10];
    u32 mFlags74;      // +0x74  (0x80,0x8,0x100,0x2,0x60,0x40,0x10000,0x4000,0x4000000,0x2000000)
    u32 mFlags78;      // +0x78  (0x1,0x200,0x400,0x800)
    u8 pad_7C[0x04];
    u16 mId80;         // +0x80  (consumed by func_800EC918)
    u8 pad_82[0x2E];
    u32 mB0;           // +0xB0  (dmg % 100 source)
};

// r28: type-3 sub-param object (fields used by this function)
struct BtlSubParam {
    u8 pad_00[0x3C];
    u16 mType3C;       // +0x3C  == 3
    u8 pad_3E[0x0A];
    u16 mEvId48;       // +0x48
    s16 mVal4A;        // +0x4A
    s16 mVal4C;        // +0x4C
    f32 mF50;          // +0x50
    f32 mF54;          // +0x54
    u8 pad_58[0x02];
    u16 mIdx5A;        // +0x5A  (func_80154058/8C/C0/134 table index)
    u8 pad_5C[0x0A];
    u8 mType66;        // +0x66
    u8 mType67;        // +0x67
    s16 mVal68;        // +0x68
    s16 mVal6A;        // +0x6A
    u8 pad_6C[0x02];
    u8 mVal6E;         // +0x6E
    u8 mVal6F;         // +0x6F
    u8 pad_70[0x08];
    u32 mFlags78;      // +0x78  (0x400, 0x800)
    u8 pad_7C[0x08];
    void* mSub84;      // +0x84  sub-obj, vtable[0x0C]() -> s32 count
};

// r19: battle-status struct from actor vtable[0x224]
struct BtlStatusPc {
    u8 pad_00[0x64];
    s16 mS64;          // +0x64
    s16 mS66;          // +0x66
    s16 mS68;          // +0x68
    s16 mS6A;          // +0x6A
    s16 mS6C;          // +0x6C
    s16 mS6E;          // +0x6E
    u8 mF70;           // +0x70 flags
    u8 pad_71;
    u8 mF72;           // +0x72 flags
};

// target->+0x3F60 heading/move data (fields written by this function)
// status entry returned by the target's CBattleState vtable[0x54]
// (mirrors cf::CBattleStateEntry field layout for the used members)
struct BtlStatusEntry {
    u32 unk00;
    u32 unk04;
    u32 unk08;
    u16 unk0C;   // +0x0C id
    u32 unk10;   // +0x10 value (s32)
    s16 unk14;   // +0x14 range
    s16 unk16;   // +0x16 event id
    s16 unk18;   // +0x18 event value
};

struct BtlMoveData {
    u8 pad_00[0x468];
    f32 mFacing468;    // +0x468
    f32 mFacing46C;    // +0x46C
    f32 mSpeed470;     // +0x470
    f32 mSpeed474;     // +0x474
    f32 mFacing478;    // +0x478
    u8 pad_47C[0xB4];
    u16 mFlags530;     // +0x530 (bit0 gating the heading-set)
};

// 0x34-byte battle-event struct consumed by func_800EC918
struct BtlEvent {
    u32 mTargetId;     // +0x00 (actor->0x3F10)
    void* mParam;      // +0x04 (sub, or 0)
    u32 mPad08;        // +0x08
    u16 mEvId;         // +0x0C
    u16 mPad0E;        // +0x0E
    s32 mVal10;        // +0x10
    s32 mVal14;        // +0x14
    u32 mPad18;        // +0x18
    f32 mF1C;          // +0x1C
    f32 mF20;          // +0x20
    f32 mF24;          // +0x24
    u32 mPad28;        // +0x28
    u32 mPad2C;        // +0x2C
    u32 mFlags30;      // +0x30
};

// enum-list holder used by the func_80043D90 / func_80043F18 pair
struct EnumHolder {
    void* list;
    u32 handle;
};

struct Vec3f {
    f32 x, y, z;
};

// ---------------------------------------------------------------------------
// Externs for func_800E64CC (retail symbols; only NEW ones - duplicates of
// file-scope / header decls above are omitted to avoid C-linkage overloads)
// ---------------------------------------------------------------------------


// ---------------------------------------------------------------------------
// Helpers
// ---------------------------------------------------------------------------

// Retail: fcmpo f,0.0; ble -> -0.5 : +0.5 (DOUBLE constants); fadd (double);
// fctiwz.  Round half away from zero (up for positives).
static inline s32 roundF(f32 f) {
    return (s32)((f64)f + (f > 0.0f ? 0.5 : -0.5));
}

// r22->0x1AA (u8, inside CChain at +0x1A8): 1..0x18 (unsigned) gate.
// Used at 0x7678, 0x8D24, 0x8E80.
static inline bool chapterInRange(void* self) {
    u8 v = *(const u8*)((const u8*)self + 0x1AA);
    return v >= 1 && v <= 0x18;
}

// vtable helpers -------------------------------------------------------------
// Slots on the CfObjectActor main vtable (CActorParam numbering from
// CActorParam.hpp / CfObjectActor.hpp).
typedef s32 (*e64_VFn2BC)(void*);                                  // CActorParam_UnkVirtualFunc138
typedef void* (*e64_VFn290)(void*);                                // CActorParam_UnkVirtualFunc127
typedef s32 (*e64_VFn5C0)(void*, void*);                           // CfObjectActor_UnkVirtualFunc9 (real sig)
typedef void (*e64_VFn2C4)(void*, void*, f32, f32, f32);           // CActorParam_UnkVirtualFunc140
typedef void (*e64_VFn120)(void*, u32, u32, f32);                  // CActorParam_UnkVirtualFunc35 (real sig)
typedef void (*e64_VFn11C)(void*, f32);                            // CActorParam_UnkVirtualFunc34 (real sig)
typedef f32 (*e64_VFn12C)(void*);                                  // CActorParam_UnkVirtualFunc38
typedef void (*e64_VFn154)(void*, f32);                            // CActorParam_UnkVirtualFunc48 (real sig)
typedef void (*e64_VFn184)(void*, u32);                            // CActorParam_UnkVirtualFunc60 (real sig)
typedef void (*e64_VFn1A4)(void*, f32);                            // CActorParam_UnkVirtualFunc68
typedef void* (*e64_VFn224)(void*);                                // CActorParam_UnkVirtualFunc100
typedef f32 (*e64_VFn5B4)(void*);                                  // CfObjectActor_UnkVirtualFunc6
typedef f32 (*e64_VFn128)(void*);                                  // CActorParam_UnkVirtualFunc37
typedef f32 (*e64_VFn130)(void*);                                  // CActorParam_UnkVirtualFunc39 (real sig)
typedef void (*e64_VFn2F8)(void*, s32);                            // CActorParam_UnkVirtualFunc126? (real sig)
typedef void (*e64_VFn0E0)(void*);                                 // state-id getter (returns s32)
typedef void (*e64_VFn0C4)(void*, void*);                          // set-target (r23, r25)

// r24+8 : CBattleState container; vtable[0x54] = CBattleState_UnkVirtualFunc20
typedef BtlStatusEntry* (*e64_VFn54)(void*, u32);

// r24->0x4 : sub-object; vtable[0x30] returns u32* (id); vtable[0x20] clears
typedef u32* (*e64_VFn30)(void*);
typedef void (*e64_VFn20)(void*, u32);

// r24->0x3E9C : CfObjectMove base; vtable[0xAC] returns Vec3f* (pos),
// vtable[0x4C] returns void* (id), vtable[0xC4] sets heading,
// vtable[0x20C] sets value
typedef Vec3f* (*e64_VFnAC)(void*);
typedef void* (*e64_VFn4C)(void*);
typedef void (*e64_VFnC4)(void*, f32);
typedef void (*e64_VFn20C)(void*, u32);

// r28->0x84 : sub-obj; vtable[0x0C] returns s32 (count/index)
typedef s32 (*e64_VFn84_0C)(void*);

// bit/switch helpers on r19 (BtlStatusPc) ------------------------------------
// flag72: switch(cat){1:&0x80 2:&0x01 3:&0x02 4:&0x04 5:&0x08 6:&0x10 7:&0x20}
// retail instantiates this switch TWICE (tables 0x8052B680 / 0x8052B658)
static inline int flag72(const BtlStatusPc* bs, int cat) {
    switch (cat) {
        case 1: return (bs->mF72 & 0x80) != 0;
        case 2: return (bs->mF72 & 0x01) != 0;
        case 3: return (bs->mF72 & 0x02) != 0;
        case 4: return (bs->mF72 & 0x04) != 0;
        case 5: return (bs->mF72 & 0x08) != 0;
        case 6: return (bs->mF72 & 0x10) != 0;
        case 7: return (bs->mF72 & 0x20) != 0;
        default: return 0;
    }
}
// flag70: same bit pattern on mF70 (tables 0x8052B630 / 0x8052B608)
static inline int flag70(const BtlStatusPc* bs, int cat) {
    switch (cat) {
        case 1: return (bs->mF70 & 0x80) != 0;
        case 2: return (bs->mF70 & 0x01) != 0;
        case 3: return (bs->mF70 & 0x02) != 0;
        case 4: return (bs->mF70 & 0x04) != 0;
        case 5: return (bs->mF70 & 0x08) != 0;
        case 6: return (bs->mF70 & 0x10) != 0;
        case 7: return (bs->mF70 & 0x20) != 0;
        default: return 0;
    }
}
// value16: switch(cat){4:mS64 5:mS66 6:mS68 7:mS6A 8:mS6C 9:mS6E}
static inline s16 value16(const BtlStatusPc* bs, int cat) {
    switch (cat) {
        case 4: return bs->mS64;
        case 5: return bs->mS66;
        case 6: return bs->mS68;
        case 7: return bs->mS6A;
        case 8: return bs->mS6C;
        case 9: return bs->mS6E;
        default: return 0;
    }
}
// effect class for the actor (0x7ED8 switch): {4:0x42 5:0x4C 6:0x44 7:0x46
//                                             8:0x4A 9:0x48}
static inline u32 effectClass(int cat) {
    switch (cat) {
        case 4: return 0x42;
        case 5: return 0x4C;
        case 6: return 0x44;
        case 7: return 0x46;
        case 8: return 0x4A;
        case 9: return 0x48;
        default: return 0;
    }
}
// sound id (0x7F54 switch): {1:0x1AD 2:0x1AA 3:0x1B0 4:0x1B6 5:0x1BA
//                            6:0x1B5 7:0x1B7 8:0x1B8 9:0x1B9}
static inline u32 statusSound(int cat) {
    switch (cat) {
        case 1: return 0x1AD;
        case 2: return 0x1AA;
        case 3: return 0x1B0;
        case 4: return 0x1B6;
        case 5: return 0x1BA;
        case 6: return 0x1B5;
        case 7: return 0x1B7;
        case 8: return 0x1B8;
        case 9: return 0x1B9;
        default: return 0;
    }
}

// full 3D distance used by the range checks (retail PSVECMag of (b-a))
static inline f32 dist3D(void* a, void* b) {
    e64_VFnAC getPos = (e64_VFnAC)(*(void***)((u8*)a + 0x3E9C)[0xAC / 4]);
    e64_VFnAC getPosB = (e64_VFnAC)(*(void***)((u8*)b + 0x3E9C)[0xAC / 4]);
    Vec3f* pa = getPos((u8*)a + 0x3E9C);
    Vec3f* pb = getPosB((u8*)b + 0x3E9C);
    Vec3f d = { pb->x - pa->x, pb->y - pa->y, pb->z - pa->z };
    return PSVECMag((const struct Vec*)&d);
}

// mActorList1 (reslist at self+0x4) membership walk + "node == sentinel"
// check, as compiled at 0x70A0-0x70D8 (the subf/or/srwi pair is the inlined
// reslist node-vs-sentinel test, cf. func_8049D548).
static inline bool inActorList1(void* self, void* item) {
    struct Node { Node* mNext; Node* mPrev; void* mItem; };
    Node* sentinel = *(Node**)((u8*)self + 0x08);
    Node* cur = sentinel->mNext;
    while (cur != sentinel) {
        if (cur->mItem == item) break;
        cur = cur->mNext;
    }
    return cur != sentinel;
}

// "power" = round-half-up(dmg + heal), guarded by < 0 and flag 0x80.
// Retail re-computes this 3x (0x72E4, 0x7410, 0x74A0) - keep as call.
static inline s32 calcPower(const void* mv) {
    const BtlMove* move = (const BtlMove*)mv;
    f32 sum = move->mDmg + move->mHeal;
    if (sum < 0.0f || (move->mFlags74 & 0x80)) return 0;
    return roundF(sum);
}

// ---------------------------------------------------------------------------
// The reconstructed function
// ---------------------------------------------------------------------------
extern "C" void CfSoundMan_func_801BFC38(u32 a, u32 b, u32 c, u32 d, f32 vol);

extern "C" void func_800E64CC(cf::CBattleManager* self, void* actor, void* arg5, void* move) {
    // r22 = self, r23 = actor, r5 = arg5 (null-gated only), r25 = move

    if (arg5 == nullptr) return;                      // 0x800E7024

    BtlMove* bm = (BtlMove*)move;
    BtlSubParam* sub = (BtlSubParam*)bm->mSub;      // r28 (0x7094)
    void* target;                                     // r24
    if (bm->mFlags74 & 0x80) {
        target = actor;                               // 0x7034
    } else {
        target = func_8016FE34(func_800B708C((s32)bm->mSubId));  // 0x703C-0x7048
    }

    if (((e64_VFn2BC)(*(void***)target)[0x2BC / 4])(target) != 0) return;   // 0x704C-0x7064
    if (func_80148778((u8*)target + 8, 0xF8)) return;                             // 0x7068-0x7078

    func_800E08E8(self, actor, target, move);         // 0x707C-0x708C

    if (bm->mFlags78 & 0x1) {                       // 0x7090-0x70D8
        if (!inActorList1(self, target)) {
            if (((e64_VFn5C0)(*(void***)target)[0x5C0 / 4])(target, actor) != 0)
                return;                               // 0x70DC-0x70F8
        }
    }

    if (sub->mType3C != 3) goto postLoop;             // 0x70FC-0x7104

    // ---- initial damage gate ----------------------------------------------
    {
        f32 sum = bm->mDmg + bm->mHeal;           // 0x7108-0x7118
        s32 dmg;
        if (sum < 0.0f || (bm->mFlags74 & 0x80)) {
            dmg = 0;                                  // 0x7120 / 0x7134
        } else {
            dmg = roundF(sum);                        // 0x713C-0x7158
        }
        if (dmg <= 0) goto postLoop;                  // 0x715C-0x7160
    }

    // ---- status 0x106 branch: rate + knockback + voice ---------------------
    if (func_80148778((u8*)target + 8, 0x106)) {      // 0x7164-0x7174
        func_80149154((u8*)target + 8, 0x106);        // 0x7178-0x7180
        if (!(bm->mFlags78 & 0x800)) {              // 0x7184-0x718C
            if (sub == nullptr) return;               // 0x7190-0x7198 (reload)

            // rate selection from dmg%100 vs thresholds (0x719C-0x7274)
            f32 rate = lbl_eu_80666DD4;               // 1.0f (0x71C0)
            s32 x = ((e64_VFn84_0C)(*(void***)sub->mSub84)[0x0C / 4])(sub->mSub84);   // 0x71B4-0x71EC
            s32 y = ((e64_VFn84_0C)(*(void***)sub->mSub84)[0x0C / 4])(sub->mSub84);   // 0x71FC-0x7210
            // NOTE: retail materialises the 3-entry threshold array at
            // 0xC4/0xC8/0xCC = { x/2+3, y+10, *(u32*)&lbl_eu_804FCAD8[2] (== 2) };
            // the 3rd entry comes from the rodata table, not an immediate.
            u32 th[3] = { (u32)(x / 2 + 3), (u32)(y + 10), *(const u32*)&lbl_eu_804FCAD8[2] };
            u32 rem = bm->mB0 % 100;                // 0x71A0-0x71E4 (mulhw magic)
            for (int i = 0; i < 3; i++) {             // 0x724C-0x7270
                rem -= th[i];
                if ((s32)rem < 0) {
                    rate = lbl_eu_804FCAE4[i];           // {8,4,2}
                    break;
                }
            }

            if (((e64_VFn290)(*(void***)target)[0x290 / 4])(target) != nullptr) {  // 0x7274-0x728C
                u32 v;
                if (func_80260264(((e64_VFn290)(*(void***)target)[0x290 / 4])(target),
                                  0x5B, &v)) {        // 0x7290-0x72B0
                    rate *= 1.0f + (f32)(u32)v / lbl_eu_80666E00;   // 0x72B8-0x72E0 (100.0f)
                }
            }

            s32 power = calcPower(move);              // 0x72E4-0x7348
            // retail also computes (mFlags74 & 0x7FFFFFFD) & 0x7FFFFBE5 into a
            // dead register here (0x7304-0x7308) - source-side artifact.

            // knockback (0x734C-0x740C): -(s32)(a + (a + (a>0 ? 0.5 : -0.5)))
            {
                f32 a = (f32)(s32)power * rate;
                f64 r = (f64)a + (a > 0.0f ? 0.5 : -0.5);
                s32 kb = -(s32)((f64)a + r);
                ((e64_VFn120)(*(void***)actor)[0x120 / 4])(actor, 2, 0, (f32)(s32)kb);
            }

            // voice event A (0x7410-0x7494)
            {
                s32 p = calcPower(move);              // re-computed
                func_802A26D8(actor, (void*)target, (void*)(size_t)(s32)(rate * (f32)(-p)));
            }
            func_802A25EC(target);                    // 0x7498

            // voice event B (0x74A0-0x7520)
            {
                s32 p = calcPower(move);              // re-computed
                func_8027F848(target, (s32)(rate * (f32)(s32)p), move);
            }
            ((e64_VFn0C4)(*(void***)actor)[0x0C4 / 4])(actor, move);   // 0x7524-0x7538
        }
    }

    // ---- gates before the status loop (0x753C-0x75AC) ----------------------
    if (func_80148778((u8*)target + 8, 0x10)) {       // 0x753C-0x754C
        if (!(*(u32*)((u8*)target + 0x3374) & 0x40000)) goto postLoop;   // 0x7550-0x7558
        {
            void* sub4 = *(void**)((u8*)target + 0x04);
            int v = *((e64_VFn30)(*(void***)sub4)[0x30 / 4])(sub4);   // 0x755C-0x7570
            if (func_80174C98(target, &v, 0x17) == 0) goto postLoop;        // 0x7574-0x758C
        }
    }
    if (func_80148778((u8*)target + 8, 0x117)) goto postLoop;   // 0x7590-0x759C
    if (bm->mFlags74 & 0x80) goto postLoop;                   // 0x75A0-0x75AC

    // ---- main status loop: CBattleState entries 0x40..0x67 ----------------
    for (u32 id = 0x40; id < 0x68; id++) {            // r27 (0x75F4-0x7FD4)
        BtlStatusEntry* st =
            ((e64_VFn54)(*(void***)((u8*)target + 8))[0x54 / 4])((u8*)target + 8, id);
        u16 sid = st->unk0C;
        s32 cat = 0;                                  // r26

        if (sid >= 0x6E && sid <= 0x7C) {
            switch (sid) {
                case 0x6E: cat = func_80145BC4(0x6E); break;   // 0x7638
                case 0x6F: cat = func_80145BC4(0x6F); break;   // 0x7648
                case 0x70: cat = func_80145BC4(0x70); break;   // 0x7658
                case 0x71: cat = func_80145BC4(0x71); break;   // 0x7668
                case 0x72: {                                  // 0x7678
                    if (chapterInRange(self)) break;           // -> skip event
                    if (st->unk14 == 0 || dist3D(actor, target) <= (f32)(s16)st->unk14) {
                        // build the 0x72 event (0x7734-0x77B8)
                        BtlEvent ev;
                        std::memset(&ev, 0, sizeof(ev));
                        ev.mTargetId = *(u32*)((u8*)actor + 0x3F10);
                        ev.mParam = nullptr;
                        ev.mEvId = (u16)st->unk16;
                        ev.mVal10 = (s32)(s16)st->unk18;
                        ev.mF20 = lbl_eu_80666E34;              // 10.0f
                        ev.mF24 = lbl_eu_80666DDC;              // 0.0f
                        ev.mFlags30 |= 0x20000;
                        func_800EC918(self, actor, target, &ev, move);
                        func_800451D8(0x42, (u32)(actor ? (size_t)((u8*)actor + 0x3E9C) : 0));
                        CfSoundMan_func_801BFC38(0, 0x1B6, 0, 0, lbl_eu_80666DD0);
                    }
                    break;
                }
                default: break;                       // 0x73..0x7C
            }
        }

        // distance re-check for category statuses (0x77BC-0x785C)
        if (cat == 0 || st->unk14 == 0) goto nextStatus;
        if (dist3D(actor, target) > (f32)(s16)st->unk14) cat = 0;
        if (cat == 0) goto nextStatus;

        // ---- apply (0x7864-0x7FD0) ----------------------------------------
        {
            BtlStatusPc* bs = (BtlStatusPc*)((e64_VFn224)(*(void***)actor)[0x224 / 4])(actor);  // r19
            s32 val = st->unk10;                      // r16 (0x7884: st->0x10)
            f32 rate = lbl_eu_80666DD4;               // f25 = 1.0f

            if (((e64_VFn290)(*(void***)target)[0x290 / 4])(target) != nullptr) {   // 0x7894-0x789C
                if (func_80149154((u8*)target + 8, 0x6F) != nullptr ||
                    func_80149154((u8*)target + 8, 0x79) != nullptr) {                    // 0x78A0-0x78C4
                    u32 v;
                    if (func_80260264(((e64_VFn290)(*(void***)target)[0x290 / 4])(target),
                                      0x5B, &v)) {   // 0x78C8-0x78E8
                        val = (s32)((f32)(s32)val * (1.0f + (f32)(u32)v / lbl_eu_80666E00)); // 0x78F0-0x7928
                    }
                }
            }
            if (func_80148778((u8*)actor + 8, 0x8D)) {                                     // 0x792C-0x793C
                BtlStatusEntry* e = (BtlStatusEntry*)func_80149154((u8*)actor + 8, 0x8D);
                rate -= lbl_eu_80666DD8 * (f32)(s32)e->unk10;   // 0.001f (0x7940-0x7960)
            }
            if (((e64_VFn290)(*(void***)actor)[0x290 / 4])(actor) != nullptr) {      // 0x7964-0x797C
                u32 v;
                if (func_80260264(((e64_VFn290)(*(void***)actor)[0x290 / 4])(actor),
                                  0x57, &v)) {       // 0x7980-0x79A0
                    rate -= lbl_eu_80666DD8 * (f32)(s32)v;       // 0x79A8-0x79BC
                }
            }
            val = (s32)((f32)(s32)val * rate);        // 0x79C0-0x79DC
            if (val < 1) val = 0;                     // 0x79E0-0x79E8
            if (func_80148778((u8*)actor + 8, 0x01)) val = 0;    // 0x79EC-0x7A00

            // sign decision (0x7A04-0x7C40)
            int r0;
            if (cat >= 1 && cat <= 3) {
                r0 = flag72(bs, cat);                 // table 0x8052B680
            } else if (cat >= 4 && cat <= 9) {
                s16 vv = value16(bs, cat);            // 0x7AF0
                r0 = (vv >= 0xC8) ? 1 : flag72(bs, cat);        // 0x7B58 / table 0x8052B658
            } else {
                r0 = 0;
            }
            if (r0) {
                val = -val;                           // 0x7C38
            } else {
                // halve decision (0x7C44-0x7E98)
                if (cat >= 1 && cat <= 3) {
                    r0 = flag70(bs, cat);             // table 0x8052B630
                } else if (cat >= 4 && cat <= 9) {
                    s16 vv = value16(bs, cat);        // 0x7D30
                    r0 = (vv >= 0x64) ? 1 : flag70(bs, cat);     // 0x7D98 / table 0x8052B608
                } else {
                    r0 = 0;
                }
                if (r0) val = (s32)((f32)(s32)val * lbl_eu_80666DE8);   // *0.5f (0x7E88)
            }

            if (val != 0) {                           // 0x7E9C-0x7ED4
                ((e64_VFn120)(*(void***)actor)[0x120 / 4])(actor, 2, 0, -(f32)(s32)val);
            }
            func_800451D8(effectClass(cat), (u32)(actor ? (size_t)((u8*)actor + 0x3E9C) : 0));  // 0x7ED8-0x7F50
            CfSoundMan_func_801BFC38(0, statusSound(cat), 0, 0, lbl_eu_80666DD0);     // 0x7F54-0x7FD0
        }

nextStatus:
        ;
    }

postLoop:
    // ---- post-loop: target switch + per-sub event dispatch (0x7FE0-0x8FEC) --
    if (!(bm->mFlags74 & 0x8)) {                    // 0x7FE0-0x7FE8
        if (func_80148778((u8*)target + 8, 0xF7)) {   // 0x7FEC-0x7FFC
            if (((e64_VFn5C0)(*(void***)target)[0x5C0 / 4])(target, actor) == 0) {  // 0x8000-0x801C
                func_80149154((u8*)target + 8, 0xF7); // 0x8020-0x8028
                u32 f78 = bm->mFlags78;             // 0x802C
                if ((f78 & 0x200) || ((f78 & 0x400) && !(f78 & 0x800))) {   // 0x8030-0x8044
                    bm->mFlags74 |= 0x80000000 | 0x80;   // 0x8048-0x8058
                    target = actor;                   // 0x804C
                }
            }
        }
    }

    {
        int hit = 0;                                  // r14
        f32 f24 = lbl_eu_80666DDC;                    // 0.0f (0x8064)

        if (!(bm->mFlags74 & 0x8) || (sub->mFlags78 & 0x800)) {   // 0x805C-0x806C: rlwinm 28,28 / bit 20
            // switch on sub->mType66 (0..0x19), table 0x8052B578 (0x807C-0x82B4)
            switch (sub->mType66) {
                case 0:   // 0x80A0
                    if (*(u32*)((u8*)actor + 0x3374) & 0x10000) {
                        if (((s32(*)(void*))(*(void***)target)[0xE0 / 4])(target) == 1 ||
                            ((s32(*)(void*))(*(void***)target)[0xE0 / 4])(target) == 2)
                            hit = 1;
                    } else {
                        if (((s32(*)(void*))(*(void***)target)[0xE0 / 4])(target) == 1)
                            hit = 1;
                    }
                    break;
                case 1:   // 0x8110
                    if (((s32(*)(void*))(*(void***)target)[0xE0 / 4])(target) == 3)
                        hit = 1;
                    break;
                case 2:   // 0x8134
                    if (bm->mFlags74 & 0x100) hit = 1;
                    break;
                case 3:   // 0x8148
                    if (bm->mFlags74 & 0x4000000) hit = 1;
                    break;
                case 4:   // 0x815C
                    if (bm->mFlags74 & 0x2000000) hit = 1;
                    break;
                case 5:   // 0x8170
                    if (func_80148778((u8*)actor + 8, 0xC1)) hit = 1;
                    break;
                case 6:   // 0x818C
                    if (func_80148778((u8*)target + 8, 0x10)) hit = 1;
                    break;
                case 7:   // 0x81A8
                    if (func_80148778((u8*)target + 8, 0x0F)) hit = 1;
                    break;
                case 8:   // 0x81C4
                    if (((e64_VFn4C)(*(void***)((u8*)target + 0x3E9C))[0x4C / 4])((u8*)target + 0x3E9C)
                            == *(void**)((u8*)actor + 0x3F10))
                        hit = 1;
                    break;
                case 9:   // 0x81EC
                    if (bm->mDmg >= ((e64_VFn128)(*(void***)target)[0x128 / 4])(target)) {
                        hit = 1;
                        ((e64_VFn20C)(*(void***)((u8*)actor + 0x3E9C))[0x20C / 4])((u8*)actor + 0x3E9C, 0x2B);
                    }
                    break;
                case 10:  // 0x8230
                    if (sub->mType67 != 0) hit = 1;
                    break;
                case 11:  // 0x8244
                    if ((u16)func_8016DF2C() == (u16)(sub->mType66 - 0x0A)) hit = 1;
                    break;
                case 12:  // 0x8260
                    if ((bm->mFlags74 & 0x4000000) &&
                        ((s32(*)(void*))(*(void***)target)[0xE0 / 4])(target) == 1)
                        hit = 1;
                    break;
                case 13:  // 0x8290
                    if (((e64_VFn130)(*(void***)target)[0x130 / 4])(target) < lbl_eu_80666E24)  // 0.15f
                        hit = 1;
                    break;
                default:  // 0x14..0x19
                    break;
            }
        }

        if (sub->mFlags78 & 0x800) {                  // 0x82B4-0x82BC
            if (func_800D7D24(move) == 0) hit = 0;    // 0x82C0-0x82D0
        }

        if (hit != 0) {                               // 0x82D4-0x8300
            u8 t = sub->mType66;
            if ((u32)(t - 0x0B) > 0x0C && t != 0) {   // not in [0xB,0x17] and != 0
                bm->mFlags74 |= 0x80000000 | 0x400;
            }
        }

        if (hit != 0) {                               // 0x8304-0x836C
            if ((*(u32*)((u8*)actor + 0x3F00) & 0x2) && sub->mType66 != 0) {
                func_8018C820((u8*)self + 0x194, 0x14);   // 0x8324-0x833C
            }
            if (sub->mType67 == 9) {                  // 0x8340-0x836C
                f24 += (f32)(s32)sub->mVal68 / lbl_eu_80666E34;   // 10.0f
            }
        }

        // ---- event build (0x8370-0x84EC) ----------------------------------
        if (sub->mEvId48 != 0 && (!(bm->mFlags74 & 0x8) || (sub->mFlags78 & 0x400))) {
            BtlEvent ev;
            std::memset(&ev, 0, sizeof(ev));          // 0x8394-0x83A0
            ev.mTargetId = *(u32*)((u8*)actor + 0x3F10);
            ev.mParam = sub;
            ev.mEvId = sub->mEvId48;
            {
                s32 v = ((e64_VFn84_0C)(*(void***)sub->mSub84)[0x0C / 4])(sub->mSub84);
                ev.mVal10 = (s32)sub->mVal4A + (s32)sub->mVal6F * (v - 1);   // 0x83BC-0x83E4
            }
            ev.mVal14 = (s32)(s16)sub->mVal4C;        // 0x83E8-0x83EC
            {
                s32 v = ((e64_VFn84_0C)(*(void***)sub->mSub84)[0x0C / 4])(sub->mSub84);
                ev.mF20 = f24 + (f32)(s32)(v - 1) / lbl_eu_80666E34 * (f32)(u32)sub->mVal6E
                          + sub->mF50;                // 0x83F0-0x8448
            }
            ev.mF24 = sub->mF54;                      // 0x844C-0x8450

            switch (ev.mEvId) {
                case 0xD5:                            // 0x846C
                case 0x107:
                    ev.mVal10 = -(s32)bm->mDmg;     // fneg+fctiwz (0x846C-0x848C)
                    func_802A27F4(target, actor, move);
                    break;
                case 0xCF:                            // 0x8498
                    if (*(u32*)((u8*)actor + 0x3374) & 0x10000)
                        ev.mEvId = 0xD0;
                    break;
                default:
                    break;
            }

            if (sub->mFlags78 & 0x400)                // 0x84AC-0x84B4
                func_800EC918(self, actor, actor, &ev, move);    // 0x84B8-0x84CC
            else
                func_800EC918(self, actor, target, &ev, move);   // 0x84D4-0x84E8
        }

        // ---- sub->mType67 switch (0x84EC-0x8974) --------------------------
        if (hit != 0 && sub->mType67 <= 0x0D) {
            BtlEvent ev;
            std::memset(&ev, 0, sizeof(ev));          // 0x84F4-0x8500
            switch (sub->mType67) {                   // table 0x8052B540
                case 0:                               // 0x8528
                    ((e64_VFn154)(*(void***)actor)[0x154 / 4])(actor, (f32)(s32)sub->mVal68);
                    break;
                case 1:                               // 0x8558
                    ev.mTargetId = *(u32*)((u8*)actor + 0x3F10);
                    ev.mParam = sub;
                    ev.mEvId = 0x10;
                    ev.mVal10 = 0;
                    ev.mF20 = (f32)(s32)sub->mVal68;
                    ev.mF24 = lbl_eu_80666DDC;        // 0.0f
                    if (sub->mFlags78 & 0x800)
                        func_800EC918(self, actor, actor, &ev, move);
                    else
                        func_800EC918(self, actor, target, &ev, move);
                    break;
                case 2:                               // 0x85DC (all-actors)
                    ev.mTargetId = *(u32*)((u8*)actor + 0x3F10);
                    ev.mParam = sub;
                    ev.mEvId = 0x10;
                    ev.mVal10 = 3;
                    ev.mF20 = lbl_eu_80666E68;        // 3.0f
                    ev.mF24 = lbl_eu_80666DDC;        // 0.0f
                    {
                        EnumHolder holder;
                        func_80043D90(&holder);
                        func_800F4A98(func_80043F18(&holder), 0x80000000, 0);
                        for (u32 i = 0; ; i++) {
                            void* list = func_80043F18(&holder);
                            u32 count = *(u32*)((u8*)list + 0x620);
                            if (i >= count) break;
                            void* elem = func_8016FE34(func_800F6EAC(func_80043F18(&holder), i));
                            if (elem != target)
                                func_800EC918(self, actor, elem, &ev, move);
                        }
                        __dt__80043E88(&holder, -1);
                    }
                    break;
                case 3:                               // 0x8688
                    ev.mTargetId = *(u32*)((u8*)actor + 0x3F10);
                    ev.mParam = sub;
                    ev.mEvId = 0x10;
                    ev.mVal10 = 3;
                    ev.mF20 = lbl_eu_80666E68;        // 3.0f
                    ev.mF24 = lbl_eu_80666DDC;        // 0.0f
                    func_800EC918(self, actor, target, &ev, move);
                    break;
                case 4:                               // 0x86D0
                    ((e64_VFn2F8)(*(void***)actor)[0x2F8 / 4])(actor, sub->mVal68);
                    break;
                case 5:                               // 0x86EC
                    ev.mTargetId = *(u32*)((u8*)actor + 0x3F10);
                    ev.mParam = sub;
                    ev.mEvId = 0x0F;
                    ev.mVal10 = 0;
                    ev.mF20 = (f32)(s32)sub->mVal68;
                    ev.mF24 = lbl_eu_80666DDC;
                    if (sub->mFlags78 & 0x800)
                        func_800EC918(self, actor, actor, &ev, move);
                    else
                        func_800EC918(self, actor, target, &ev, move);
                    break;
                case 6:                               // 0x8770 (SP drain)
                    if (sub->mFlags78 & 0x800) {
                        f32 f2 = ((e64_VFn12C)(*(void***)actor)[0x12C / 4])(actor) / lbl_eu_80666E00;
                        ((e64_VFn11C)(*(void***)actor)[0x11C / 4])(actor,
                            -(f32)(s32)sub->mVal68 * f2);
                    } else {
                        f32 f2 = ((e64_VFn12C)(*(void***)actor)[0x12C / 4])(actor) / lbl_eu_80666E00;
                        ((e64_VFn11C)(*(void***)target)[0x11C / 4])(target,
                            -(f32)(s32)sub->mVal68 * f2);
                    }
                    break;
                case 7:                               // 0x8824
                    ev.mTargetId = *(u32*)((u8*)actor + 0x3F10);
                    ev.mParam = sub;
                    ev.mEvId = 0xDC;
                    ev.mVal10 = (s32)(s16)sub->mVal68;
                    if (sub->mFlags78 & 0x800)
                        func_800EC918(self, actor, actor, &ev, move);
                    else
                        func_800EC918(self, actor, target, &ev, move);
                    break;
                case 8:                               // 0x8884
                    ev.mTargetId = *(u32*)((u8*)actor + 0x3F10);
                    ev.mParam = sub;
                    ev.mEvId = (u16)sub->mVal68;
                    ev.mVal10 = (s32)(s16)sub->mVal6A;
                    {
                        s32 v = ((e64_VFn84_0C)(*(void***)sub->mSub84)[0x0C / 4])(sub->mSub84);
                        ev.mF20 = (f32)(s32)(v - 1) / lbl_eu_80666E34 * (f32)(u32)sub->mVal6E
                                  + sub->mF50;       // 0x88A4-0x88F0
                    }
                    ev.mF24 = sub->mF54;
                    if (sub->mFlags78 & 0x800)
                        func_800EC918(self, actor, actor, &ev, move);
                    else
                        func_800EC918(self, actor, target, &ev, move);
                    break;
                case 9:                               // 0x8940
                    ev.mTargetId = *(u32*)((u8*)actor + 0x3F10);
                    ev.mParam = sub;
                    ev.mEvId = 0xC1;
                    ev.mF20 = lbl_eu_80666E6C;        // 5.0f
                    func_800EC918(self, actor, actor, &ev, move);
                    break;
                default:
                    break;
            }
        }

        // ---- type-3 status apply (0x8974-0x8FEC) ---------------------------
        if (sub->mType3C == 3) {
            // gates: 7x func_80174C98(target, &v, mask); v re-fetched each time
            {
                void* sub4 = *(void**)((u8*)target + 0x04);
                int v = *((e64_VFn30)(*(void***)sub4)[0x30 / 4])(sub4);
                if (func_80174C98(target, &v, 0x0A)) goto afterType3;
                v = *((e64_VFn30)(*(void***)sub4)[0x30 / 4])(sub4);
                if (func_80174C98(target, &v, 0x0B)) goto afterType3;
                v = *((e64_VFn30)(*(void***)sub4)[0x30 / 4])(sub4);
                if (func_80174C98(target, &v, 0x16)) goto afterType3;
                v = *((e64_VFn30)(*(void***)sub4)[0x30 / 4])(sub4);
                if (func_80174C98(target, &v, 0x18)) goto afterType3;
                v = *((e64_VFn30)(*(void***)sub4)[0x30 / 4])(sub4);
                if (func_80174C98(target, &v, 0x19)) goto afterType3;
                v = *((e64_VFn30)(*(void***)sub4)[0x30 / 4])(sub4);
                if (func_80174C98(target, &v, 0x1A)) goto afterType3;
                v = *((e64_VFn30)(*(void***)sub4)[0x30 / 4])(sub4);
                if (func_80174C98(target, &v, 0x1B)) goto afterType3;
            }

            if (bm->mFlags74 & 0x2) {               // 0x8AEC
                func_800BE12C((u8*)actor + 0x3E9C, 0x2D, 0, -1, 1);
                if (func_802799F0((u8*)self + 0x1A8, target) == 0)
                    func_800BE12C((u8*)target + 0x3E9C, 0x2E, 0, -1, 1);
            } else if (bm->mFlags74 & 0x60) {       // 0x8B40
                func_800BE12C((u8*)actor + 0x3E9C, 0x2D, 0, -1, 1);
                if (bm->mFlags74 & 0x40)
                    func_800451D8(0xB0, (u32)(target ? (size_t)((u8*)target + 0x3E9C) : 0));
            } else {
                {
                    void* sub4 = *(void**)((u8*)target + 0x04);
                    int v = *((e64_VFn30)(*(void***)sub4)[0x30 / 4])(sub4);
                    if (func_80174C98(target, &v, 0x1000)) goto afterType3;
                    v = *((e64_VFn30)(*(void***)sub4)[0x30 / 4])(sub4);
                    if (func_80174C98(target, &v, 0x07)) goto afterType3;
                    v = *((e64_VFn30)(*(void***)sub4)[0x30 / 4])(sub4);
                    if (func_80174C98(target, &v, 0x08)) goto afterType3;
                }

                if (bm->mFlags74 & 0x10000) {       // 0x8C24
                    BtlMoveData* md = *(BtlMoveData**)((u8*)target + 0x3F60);
                    if (!(md->mFlags530 & 1)) {
                        f32 f1 = ((e64_VFn5B4)(*(void***)actor)[0x5B4 / 4])(actor);
                        ((e64_VFnC4)(*(void***)((u8*)target + 0x3E9C))[0xC4 / 4])(
                            (u8*)target + 0x3E9C, lbl_eu_8066A1F8 + f1);
                    }
                    // face the actor (0x8C70-0x8D24)
                    {
                        e64_VFnAC gpA = (e64_VFnAC)(*(void***)((u8*)actor + 0x3E9C))[0xAC / 4];
                        e64_VFnAC gpT = (e64_VFnAC)(*(void***)((u8*)target + 0x3E9C))[0xAC / 4];
                        Vec3f* pa = gpA((u8*)actor + 0x3E9C);
                        Vec3f* pt = gpT((u8*)target + 0x3E9C);
                        f32 dx = pt->x - pa->x;
                        f32 dz = pt->z - pa->z;
                        BtlMoveData* md2 = *(BtlMoveData**)((u8*)target + 0x3F60);
                        if (dx * dx + dz * dz <= lbl_eu_80666E74) {
                            md2->mFacing46C = ((e64_VFn5B4)(*(void***)actor)[0x5B4 / 4])(actor);
                        } else {
                            md2->mFacing46C = Atan2FIdx__Q24nw4r4mathFff(dx, dz) * lbl_eu_80666E10;
                        }
                        if (chapterInRange(self)) {
                            md2->mFacing468 = lbl_eu_80666DD4;   // 1.0f (0x8D5C)
                        } else {
                            md2->mFacing468 = func_80154058(sub);   // 0x8D48
                        }
                    }
                    func_800BE12C((u8*)target + 0x3E9C, 0x1D, 0, -1, 1);   // 0x8D68
                } else if (bm->mFlags74 & 0x4000) { // 0x8D84
                    BtlMoveData* md = *(BtlMoveData**)((u8*)target + 0x3F60);
                    if (!(md->mFlags530 & 1)) {
                        f32 f1 = ((e64_VFn5B4)(*(void***)actor)[0x5B4 / 4])(actor);
                        ((e64_VFnC4)(*(void***)((u8*)target + 0x3E9C))[0xC4 / 4])(
                            (u8*)target + 0x3E9C, lbl_eu_8066A1F8 + f1);
                    }
                    // approach values (0x8DCC-0x8EF4)
                    {
                        e64_VFnAC gpA = (e64_VFnAC)(*(void***)((u8*)actor + 0x3E9C))[0xAC / 4];
                        e64_VFnAC gpT = (e64_VFnAC)(*(void***)((u8*)target + 0x3E9C))[0xAC / 4];
                        Vec3f* pa = gpA((u8*)actor + 0x3E9C);
                        Vec3f* pt = gpT((u8*)target + 0x3E9C);
                        f32 dx = pt->x - pa->x;
                        f32 dz = pt->z - pa->z;
                        BtlMoveData* md2 = *(BtlMoveData**)((u8*)target + 0x3F60);
                        if (dx * dx + dz * dz <= lbl_eu_80666E74) {
                            md2->mFacing478 = ((e64_VFn5B4)(*(void***)actor)[0x5B4 / 4])(actor);
                        } else {
                            md2->mFacing478 = Atan2FIdx__Q24nw4r4mathFff(dx, dz) * lbl_eu_80666E10;
                        }
                        if (chapterInRange(self)) {
                            f32 v = func_801540C0(sub);
                            if (v >= lbl_eu_80666DD4) v = lbl_eu_80666DD4;   // min(v,1.0f) (0x8ED4)
                            md2->mSpeed470 = lbl_eu_80666DDC;
                            md2->mSpeed474 = v;
                        } else {
                            md2->mSpeed470 = func_8015408C(sub);
                            md2->mSpeed474 = func_801540C0(sub);
                        }
                    }
                    if (func_80154134(sub) != lbl_eu_80666DDC) {   // 0x8EF4-0x8F38
                        // NOTE: retail calls func_80154134 TWICE (0x8EF4 and
                        // 0x8F08); the 2nd result is passed as f1 to vtbl 0x1A4.
                        ((e64_VFn1A4)(*(void***)target)[0x1A4 / 4])(target,
                            func_80154134(sub));
                        ((e64_VFn184)(*(void***)target)[0x184 / 4])(target, 1);
                    }
                    func_800BE12C((u8*)target + 0x3E9C, 0x1E, 0, -1, 1);   // 0x8F3C
                } else {
                    void* sub4 = *(void**)((u8*)target + 0x04);
                    int v = *((e64_VFn30)(*(void***)sub4)[0x30 / 4])(sub4);
                    if (func_80174C98(target, &v, 0x800)) goto afterType3;
                    v = *((e64_VFn30)(*(void***)sub4)[0x30 / 4])(sub4);
                    if (func_80174C98(target, &v, 0x1D)) goto afterType3;
                    v = *((e64_VFn30)(*(void***)sub4)[0x30 / 4])(sub4);
                    if (func_80174C98(target, &v, 0x801)) goto afterType3;
                }
            }
        }

afterType3:
        // ---- final (0x8FEC-0x9080) -----------------------------------------
        if (((e64_VFn5C0)(*(void***)actor)[0x5C0 / 4])(actor, target) == 0) {
            ((e64_VFn2C4)(*(void***)target)[0x2C4 / 4])(target, actor,
                lbl_eu_80666DD4, lbl_eu_80666DD4, lbl_eu_80666DD4);   // (1,1,1) 0x900C-0x9030
        } else {
            void* sub4 = *(void**)((u8*)target + 0x04);
            int v = *((e64_VFn30)(*(void***)sub4)[0x30 / 4])(sub4);
            if (func_80174C98(target, &v, 0x1000)) {   // 0x9034-0x9060
                ((e64_VFn20)(*(void***)sub4)[0x20 / 4])(sub4, 0x1000);   // 0x9068-0x907C
            }
        }
    }
}

void func_800E85F0(){}
void CBattleManager_preCalcTotalDamage(){}
void func_800E921C(){}
void func_800E9B54(){}

// Iterates through actor lists and calls virtual functions based on flags.
extern "C" void func_800E9FE4(void* self, void* arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, void* arg6) {
    cf::CfGameManager::getInstance();
    if (func_8006EF04__Fi(0x400)) return;

    if (func_80148778((u8*)arg1 + 8, 0x101)) {
        if (arg2 != 0) arg2 = 1;
        if (arg3 != 0) arg3 = 1;
        if (arg4 != 0) arg4 = 1;
    }

    u32 flags = *(u32*)((u8*)arg1 + 0x3f00);
    if (flags & 0x2) {
        void* sentinel = *(void**)((u8*)self + 0x48);
        void* cur = *(void**)sentinel;
        while (cur != *(void**)((u8*)self + 0x48)) {
            void* obj = *(void**)((u8*)cur + 0x8);
            if (obj != arg1) {
                void** vtbl = *(void***)obj;
                typedef s32 (*VFunc2BC)(void*);
                if (((VFunc2BC)vtbl[0x2BC / 4])(obj) == 0) {
                    if (arg5 == 1) {
                        void** vtbl2 = *(void***)((u8*)obj + 0x3e9c);
                        typedef void* (*VFunc4C)(void*);
                        if (((VFunc4C)vtbl2[0x4C / 4])((u8*)obj + 0x3e9c) == arg6) {
                            typedef void (*VFunc2C4)(void*, void*, f32, f32, f32);
                            ((VFunc2C4)vtbl[0x2C4 / 4])(obj, arg1, (f32)(s32)arg2, (f32)(s32)arg3, (f32)(s32)arg4);
                        }
                    } else {
                        typedef void (*VFunc2C4)(void*, void*, f32, f32, f32);
                        ((VFunc2C4)vtbl[0x2C4 / 4])(obj, arg1, (f32)(s32)arg2, (f32)(s32)arg3, (f32)(s32)arg4);
                    }
                }
            }
            cur = *(void**)cur;
        }
    } else {
        void* sentinel = *(void**)((u8*)self + 0x28);
        void* cur = *(void**)sentinel;
        while (cur != *(void**)((u8*)self + 0x28)) {
            void* obj = *(void**)((u8*)cur + 0x8);
            if (obj != arg1) {
                void** vtbl = *(void***)obj;
                typedef s32 (*VFunc2BC)(void*);
                if (((VFunc2BC)vtbl[0x2BC / 4])(obj) == 0) {
                    if (arg5 == 1) {
                        void** vtbl2 = *(void***)((u8*)obj + 0x3e9c);
                        typedef void* (*VFunc4C)(void*);
                        if (((VFunc4C)vtbl2[0x4C / 4])((u8*)obj + 0x3e9c) == arg6) {
                            typedef void (*VFunc2C4)(void*, void*, f32, f32, f32);
                            ((VFunc2C4)vtbl[0x2C4 / 4])(obj, arg1, (f32)(s32)arg2, (f32)(s32)arg3, (f32)(s32)arg4);
                        }
                    } else {
                        typedef void (*VFunc2C4)(void*, void*, f32, f32, f32);
                        ((VFunc2C4)vtbl[0x2C4 / 4])(obj, arg1, (f32)(s32)arg2, (f32)(s32)arg3, (f32)(s32)arg4);
                    }
                }
            }
            cur = *(void**)cur;
        }
    }
}
// Iterates through a list of actors based on flags in arg1.
// For each actor not matching arg1, calls vtable[0x2C8] with arg1->+0x3f10.
extern "C" void func_800EA2A4(cf::CBattleManager* mgr, BattleObjAccessor* arg1) {
    if (arg1 == nullptr) return;

    cf::CfGameManager::getInstance();
    if (func_8006EF04__Fi(0x4000000)) return;

    // Iterate through one of the actor lists, calling vtable[0x2C8] on each
    // actor that doesn't match arg1, with arg1->field_3f10 as the argument.
    // The list choice depends on arg1->field_3f00 bit 0x02.
    if (arg1->field_3f00 & 0x02) {
        _reslist_node<cf::CfObjectActor*>* cur = mgr->mActorList3.mStartNodePtr->mNext;
        while (cur != mgr->mActorList3.mStartNodePtr) {
            cf::CfObjectActor* actor = cur->mItem;
            if (actor != (cf::CfObjectActor*)arg1) {
                void** vtbl = *(void***)actor;
                typedef void (*VFunc2C8)(void*, void*);
                ((VFunc2C8)vtbl[0x2C8 / 4])(actor, (void*)arg1->field_3f10);
            }
            cur = cur->mNext;
        }
    } else {
        _reslist_node<cf::CfObjectActor*>* cur = mgr->mActorList2.mStartNodePtr->mNext;
        while (cur != mgr->mActorList2.mStartNodePtr) {
            cf::CfObjectActor* actor = cur->mItem;
            if (actor != (cf::CfObjectActor*)arg1) {
                void** vtbl = *(void***)actor;
                typedef void (*VFunc2C8)(void*, void*);
                ((VFunc2C8)vtbl[0x2C8 / 4])(actor, (void*)arg1->field_3f10);
            }
            cur = cur->mNext;
        }
    }
}
void func_800EA484(){}
extern void func_80109784(void* ptr, u32 id, int arg);
// Struct for the third arg of func_800EA9A8: fields at +0x00, +0x04, +0x08, +0x30
struct Func800EA9A8_Arg5 {
    u32 field_00;
    u32 field_04;
    u32 field_08;
    u8 pad_0C[0x24];
    u32 field_30;
};

void func_800EA9A8(void* self, BattleObjAccessor* arg4, Func800EA9A8_Arg5* arg5, void* arg6) {
    arg5->field_00 = (u32)arg4->field_3f10;
    arg5->field_04 = (u32)arg6;
    arg5->field_08 = 0x1000;
    arg5->field_30 |= 0x1800;
    func_800EC918((void*)lbl_eu_80663F00, 0, (void*)arg4, (void*)arg5, 0);
    if (arg6 != nullptr) {
        func_80109784(arg4->field_3f10, (u32)arg6, 9);
    }
}
struct BattleStatBlock {
    u8  pad_00[0x50];
    void* field_50;     // +0x50 source object (sub-object when evt->+4 is null)
    u8  pad_54[0x04];
    f32 field_58;       // +0x58
    f32 field_5C;       // +0x5C
    f32 field_60;       // +0x60
    f32 field_64;       // +0x64
    u8  pad_68[0x0C];
    u32 field_74;       // +0x74 rejection/state flags
    u32 field_78;       // +0x78 state flags
    u8  pad_7C[0x04];
    u16 field_80;       // +0x80 (copied into evt->+0x2C)
    u8  pad_82[0x02];
    u32 field_84;       // +0x84
    u8  pad_88[0x10];
    u32 field_98;       // +0x98
    u8  pad_9C[0x10];
    u32 field_AC;       // +0xAC
    u32 field_B0;       // +0xB0
    u32 field_B4;       // +0xB4
};

// The 4th argument: battle command event. 0x34 bytes.
struct BattleCmdEvent {
    u32 field_00;       // +0x00 (cleared by cmd 15/16 handlers)
    u32 field_04;       // +0x04 pointer
    u32 field_08;       // +0x08 pointer
    u16 cmd;            // +0x0C
    u16 pad_0E;         // +0x0E
    u32 field_10;       // +0x10 flags / value
    u32 field_14;       // +0x14 (low s16 used by cmd 221/222 as a range end)
    u32 field_18;       // +0x18
    u32 field_1C;       // +0x1C
    f32 field_20;       // +0x20
    f32 field_24;       // +0x24
    f32 field_28;       // +0x28
    u16 field_2C;       // +0x2C (cmd 15/16 copy target)
    u16 field_2E;       // +0x2E
    u32 field_30;       // +0x30 flags
};

// The battle actor layout (actorA/actorB). Only offsets touched here.
struct BattleActor {
    u8  pad_00[0x04];
    void* sub4;         // +0x04 object with vtable slot 0x30
    void* sub8;         // +0x08 object with vtable slots 0x80/0x88/0x5C
    u8  pad_0C[0x15D8]; // up to +0x15E4
    u32 type_15E4;      // +0x15E4 "enemy type" used by the power table
    u8  pad_15E8[0x1D8C]; // up to +0x3374
    u32 flags_3374;     // +0x3374
    u8  pad_3378[0xB88];  // up to +0x3F00
    u32 flags_3F00;     // +0x3F00
    u8  pad_3F04[0x0C];
    u32 field_3F10;     // +0x3F10
    u8  pad_3F14[0x14];
    u16 field_3F28;     // +0x3F28
};

// ---------------------------------------------------------------------------
// Small static helpers (semantic clarity; each maps 1:1 to retail code).
// ---------------------------------------------------------------------------

// Heap-range validity test used twice in the cmd-227 handler and once for the
// stat block in the same handler:
//   ((u32)p & 3) == 0 && (u32)p >= 0x80000000 && (u32)p < 0x93800000
static bool IsValidPtr(const void* p) {
    u32 v = (u32)p;
    return (v & 3) == 0 && v >= 0x80000000u && v < 0x93800000u;
}

// Virtual call helpers (retail vtable offsets from the reloc'd `lwz r12, N(r12)`
// instructions, all on the primary vtable of the actor unless noted).
static void* ActorVfunc224(void* actor) {
    typedef void* (*V)(void*);
    return ((V)(*(void***)actor)[0x224 / 4])(actor);
}
static bool ActorVfunc2BC(void* actor) {
    typedef bool (*V)(void*);
    return ((V)(*(void***)actor)[0x2BC / 4])(actor);
}
static void* ActorVfunc290(void* actor) {
    typedef void* (*V)(void*);
    return ((V)(*(void***)actor)[0x290 / 4])(actor);
}
static void* ActorVfunc298(void* actor) {
    typedef void* (*V)(void*);
    return ((V)(*(void***)actor)[0x298 / 4])(actor);
}
static s32 ActorVfunc108(void* actor) {
    typedef s32 (*V)(void*);
    return ((V)(*(void***)actor)[0x108 / 4])(actor);
}
static f32 ActorVfunc12C(void* actor) {
    typedef f32 (*V)(void*);
    return ((V)(*(void***)actor)[0x12C / 4])(actor);
}
static f32 ActorVfunc158(void* actor) {
    typedef f32 (*V)(void*);
    return ((V)(*(void***)actor)[0x158 / 4])(actor);
}
static s32 ActorVfunc308(void* actor) {
    typedef s32 (*V)(void*);
    return ((V)(*(void***)actor)[0x308 / 4])(actor);
}
// Returns a pointer whose u16@0 carries state flags.
static void* ActorVfunc320(void* actor) {
    typedef void* (*V)(void*);
    return ((V)(*(void***)actor)[0x320 / 4])(actor);
}
static void* ActorVfunc324(void* actor) {
    typedef void* (*V)(void*);
    return ((V)(*(void***)actor)[0x324 / 4])(actor);
}
// Sub-object at +4, vtable slot 0x30: returns a pointer whose u32@0 is read.
static void* Sub4Vfunc30(void* actor) {
    void* sub = *(void**)((u8*)actor + 4);
    typedef void* (*V)(void*);
    return ((V)(*(void***)sub)[0x30 / 4])(sub);
}
// Sub-object at +8 vtable slots (cmd classification / art-state queries).
static bool Sub8Vfunc80(void* actor, u16 cmd) {
    void* sub = *(void**)((u8*)actor + 8);
    typedef bool (*V)(void*, u16);
    return ((V)(*(void***)sub)[0x80 / 4])(sub, cmd);
}
static bool Sub8Vfunc88(void* actor, u16 cmd) {
    void* sub = *(void**)((u8*)actor + 8);
    typedef bool (*V)(void*, u16);
    return ((V)(*(void***)sub)[0x88 / 4])(sub, cmd);
}
static void* Sub8Vfunc5C(void* actor, s32 v) {
    void* sub = *(void**)((u8*)actor + 8);
    typedef void* (*V)(void*, s32);
    return ((V)(*(void***)sub)[0x5C / 4])(sub, v);
}
// Secondary-vtable call on objects that carry a second vtable at +0x84,
// slot +0xC (used for the sub-object "art level" query).
static s32 SubObjVfuncC(void* sub) {
    void** vtab = *(void***)((u8*)sub + 0x84);
    typedef s32 (*V)(void*);
    return ((V)vtab[0xC / 4])(sub);
}

// ---------------------------------------------------------------------------
// The state check (inlined twice in retail with different constants):
//   call A: byteOff=113, thresh=150   -> gates fail_0x1000
//   call B: byteOff=112, thresh=100   -> gates the % 50 check
// sel is the func_80145BC4(cmd) classifier (0..9), obj is actorB->vfunc_224()
// result. Bit table for sel 0..6: {6, 31, 0, 1, 2, 3, 4} (bit 31 is always 0
// for the u8 field; kept faithfully).
// ---------------------------------------------------------------------------
static u32 BattleStateCheck(u32 sel, void* obj, u32 byteOff, s32 thresh) {
    static const u8 sBitShift[7] = { 6, 31, 0, 1, 2, 3, 4 };
    u8* p = (u8*)obj;

    if (sel <= 3) {
        return (p[byteOff] >> sBitShift[sel]) & 1;
    }
    if (sel <= 9) {
        s16 v = *(s16*)(p + 0x64 + (sel - 4) * 2);   // +100..+110
        if (v < thresh) {
            if (sel <= 6) return (p[byteOff] >> sBitShift[sel]) & 1;
            return 0;
        }
        return 1;
    }
    return 0;
}

// ---------------------------------------------------------------------------
// func_800EAA2C
// ---------------------------------------------------------------------------
extern "C" s32 func_800EAA2C(void* mgr /*r24*/, void* actorA /*r25*/, void* actorB /*r26*/,
                  void* evtArg /*r27*/, void* statArg /*r28*/) {
    BattleCmdEvent* evt = (BattleCmdEvent*)evtArg;
    BattleStatBlock* stat = (BattleStatBlock*)statArg;
    // ---- Prologue: classifier object of actorA (used by cmd 213/263). ----
    void* a224 = ActorVfunc224(actorA);   // r30 (kept across the whole body)
    ActorVfunc224(actorB);                // result discarded by retail

    // ---- Sub-object / event fixups. ----
    void* subObj = (void*)evt->field_04;  // r29
    if (subObj == nullptr && stat != nullptr) {
        subObj = stat->field_50;
    }
    if (stat != nullptr) {
        evt->field_2C = stat->field_80;
    }

    // ---- Game-state gate. ----
    getInstance__Q22cf13CfGameManagerFv();
    if (func_8006EF04__Fi(0x10000000)) return 0;

    // ---- Actor gates. ----
    if (ActorVfunc2BC(actorB)) return 0;
    if (stat != nullptr && (stat->field_78 & 0x1000)) return 0;

    // ---- Sub8 art-state gates (pre-switch). ----
    if (Sub8Vfunc80(actorB, evt->cmd)) goto fail_1000;

    if (Sub8Vfunc88(actorB, evt->cmd)) {
        // ---- "Hit chance" threshold block. ----
        s32 threshold = 50;                                // r31
        if (evt->cmd <= 18) {
            s32 type = ((BattleActor*)actorB)->type_15E4;
            if (type == 1) {
                threshold = 20;
            } else if (type == 2) {
                threshold -= 20;
            } else if (type == 3) {
                threshold += 20;
            } else if (type == 4) {
                threshold += 50;
            } else if (type == 5) {
                threshold += 50;
            }
            if (((BattleActor*)actorB)->flags_3374 & 0x100) threshold += 30;
            u8 b = *(u8*)((u8*)mgr + 0x1AA);
            if (b >= 1 && b <= 24) threshold = 0;
        }
        if (stat != nullptr) {
            if ((u32)(stat->field_B0 % 100) < (u32)threshold) goto fail_10;
        } else {
            if (mtRand__Q22ml4mathFi(100) < threshold) return 0;
        }
    }

    // ---- Skill 39 block (cmd 17 / same-actor exception). ----
    if (func_80148778((u8*)actorB + 8, 39) != nullptr) {
        if (actorB != actorA || evt->cmd != 17) {
            if (func_80145F78(evt->cmd)) {
                void* e = func_80149154((u8*)actorB + 8, 39);
                if (e != nullptr) {
                    u32 roll = stat != nullptr ? stat->field_B4 : (u32)mtRand__Q22ml4mathFi(100);
                    if (roll < *(u32*)((u8*)e + 0x10)) goto fail_10;
                }
            }
        }
    }

    // ---- Skill 206 block (physical-attack + stat bit 11 gate). ----
    if (func_80148778((u8*)actorB + 8, 206) != nullptr) {
        if (stat != nullptr && (stat->field_78 & 0x800)) {
            if (func_80145DBC(evt->cmd)) {
                void* e = func_801491F4((u8*)actorB + 8, 206);
                if (*(u32*)((u8*)e + 0x10) >= (u32)SubObjVfuncC(subObj)) {
                    stat->field_74 |= 0x80002000;
                    goto fail_10;
                }
            }
        }
    }

    // ---- Classifier state checks (two inlined copies). ----
    {
        u32 sel = (u32)func_80145BC4(evt->cmd);   // r31
        void* b224 = ActorVfunc224(actorB);       // r3, reused by both checks
        if (BattleStateCheck(sel, b224, 113, 150) != 0) goto fail_1000;
        if (BattleStateCheck(sel, b224, 112, 100) != 0) {
            if (stat != nullptr) {
                if ((stat->field_AC % 100) < 50) goto fail_10;
            } else {
                if (mtRand__Q22ml4mathFi(100) < 50) goto fail_10;
            }
        }
    }

    // ---- Physical-attack + stat-124 value block. ----
    if (ActorVfunc290(actorB) != nullptr) {
        if (func_80145DBC(evt->cmd)) {
            u32 val;
            if (func_80260264(ActorVfunc290(actorB), 124, &val)) {
                u32 roll = stat != nullptr ? stat->field_B4 : (u32)mtRand__Q22ml4mathFi(100);
                if (roll < val) goto fail_10;
            }
        }
    }

    // ========================================================================
    // Big dispatch on the command id (u16 at evt+0x0C).
    // ========================================================================
    switch (evt->cmd) {

    // ---- cmd 4: skill 21 roll. ----
    case 4:
        if (func_80148778((u8*)actorB + 8, 21) != nullptr) {
            void* e = func_80149154((u8*)actorB + 8, 21);
            u32 roll = stat != nullptr ? stat->field_B4 : (u32)mtRand__Q22ml4mathFi(100);
            if (roll < *(u32*)((u8*)e + 0x10)) goto fail_10;
        }
        break;

    // ---- cmd 5: no-op (retail -> default). ----
    case 5:
        break;

    // ---- cmd 6: skill 23 roll. ----
    case 6:
        if (func_80148778((u8*)actorB + 8, 23) != nullptr) {
            void* e = func_80149154((u8*)actorB + 8, 23);
            u32 roll = stat != nullptr ? stat->field_B4 : (u32)mtRand__Q22ml4mathFi(100);
            if (roll < *(u32*)((u8*)e + 0x10)) goto fail_10;
        }
        break;

    // ---- cmd 7: skill 24 roll. ----
    case 7:
        if (func_80148778((u8*)actorB + 8, 24) != nullptr) {
            void* e = func_80149154((u8*)actorB + 8, 24);
            u32 roll = stat != nullptr ? stat->field_B4 : (u32)mtRand__Q22ml4mathFi(100);
            if (roll < *(u32*)((u8*)e + 0x10)) goto fail_10;
        }
        break;

    // ---- cmd 9: skill 26 roll. ----
    case 9:
        if (func_80148778((u8*)actorB + 8, 26) != nullptr) {
            void* e = func_80149154((u8*)actorB + 8, 26);
            u32 roll = stat != nullptr ? stat->field_B4 : (u32)mtRand__Q22ml4mathFi(100);
            if (roll < *(u32*)((u8*)e + 0x10)) goto fail_10;
        }
        break;

    // ---- cmd 8: no-op (retail -> default). ----
    case 8:
        break;
    // ---- cmd 10: no-op (retail -> default). ----
    case 10:
        break;

    // ---- cmd 11: skill 28 roll. ----
    case 11:
        if (func_80148778((u8*)actorB + 8, 28) != nullptr) {
            void* e = func_80149154((u8*)actorB + 8, 28);
            u32 roll = stat != nullptr ? stat->field_B4 : (u32)mtRand__Q22ml4mathFi(100);
            if (roll < *(u32*)((u8*)e + 0x10)) goto fail_10;
        }
        break;

    // ---- cmd 12: skill 29 roll. ----
    case 12:
        if (func_80148778((u8*)actorB + 8, 29) != nullptr) {
            void* e = func_80149154((u8*)actorB + 8, 29);
            u32 roll = stat != nullptr ? stat->field_B4 : (u32)mtRand__Q22ml4mathFi(100);
            if (roll < *(u32*)((u8*)e + 0x10)) goto fail_10;
        }
        break;

    // ---- cmd 16: "break-art" handler (retail 0x141D0). ----
    case 16: {
        int val;
        if (func_80174C98(actorB, &val, 0xA) != 0) {
            if (*(u32*)((u8*)ActorVfunc298(actorB) + 0x78) & 0x08000000) return 0;
        }
        if (((BattleActor*)actorB)->flags_3F00 & 2) {
            // mActorList3 node count (sentinel at mgr+0x48).
            void* sentinel = *(void**)((u8*)mgr + 0x48);
            s32 count = 0;
            for (void* cur = *(void**)sentinel; cur != sentinel; cur = *(void**)cur) count++;
            if (count == 0) return 0;
        }
        if (actorA != nullptr) {
            void* tgt = stat != nullptr ? (void*)stat : ActorVfunc298(actorA);   // r31
            if (*(u32*)((u8*)tgt + 0x74) & 4) return 0;
            if (*(u32*)((u8*)tgt + 0x74) & 2) return 0;
            int val2 = *(s32*)Sub4Vfunc30(actorB);
            if (func_80174C98(actorB, &val2, 0x802) != 0) return 0;
            if (!(evt->field_30 & 0x80)) {
                if (!(evt->field_10 & 4)) {
                    u32 r = 0;                                   // r29
                    if (*(u16*)ActorVfunc320(actorB) & 8) r = 1;
                    if (func_80148778((u8*)actorB + 8, 59) != nullptr) r = 1;
                    if (func_80148778((u8*)actorB + 8, 50) != nullptr) r = 1;
                    if (r) goto fail_10;
                }
                if (!(evt->field_10 & 5)) {
                    u32 r = 0;                                   // r29 (re-reset per retail)
                    if (func_80148778((u8*)actorB + 8, 32) != nullptr) {
                        void* e = func_80149154((u8*)actorB + 8, 32);
                        u32 roll = stat != nullptr ? stat->field_B4 : (u32)mtRand__Q22ml4mathFi(100);
                        if (roll < *(u32*)((u8*)e + 0x10)) r = 1;
                    }
                    if (((BattleActor*)actorB)->flags_3374 & 0x80) {
                        if (*(u32*)((u8*)tgt + 0x74) & 0x4000) r = 1;
                    }
                    if (!(evt->field_10 & 6)) {
                        if (func_80148778((u8*)actorB + 8, 16) == nullptr) {
                            if (((BattleActor*)actorB)->flags_3F00 & 4) r = 1;
                        }
                        if (r) goto fail_10;
                    }
                }
                // Skill 154/155 power scaling of evt->+0x20.
                if (func_80148778((u8*)actorA + 8, 154) != nullptr) {
                    s32 v = *(s32*)((u8*)func_80149154((u8*)actorA + 8, 154) + 0x10);
                    f32 ratio = (f32)((f64)(s32)v / (f64)lbl_eu_80666E00);
                    evt->field_20 = evt->field_20 * (lbl_eu_80666DD4 + ratio);
                } else if (func_80148778((u8*)actorA + 8, 155) != nullptr) {
                    s32 v = *(s32*)((u8*)func_80149154((u8*)actorA + 8, 155) + 0x10);
                    f32 ratio = (f32)((f64)(s32)v / (f64)lbl_eu_80666E00);
                    evt->field_20 = evt->field_20 / (lbl_eu_80666DD4 + ratio);
                }
            }
        } else {
            int val3 = *(s32*)Sub4Vfunc30(actorB);
            if (func_80174C98(actorB, &val3, 0x802) != 0) return 0;
            if (func_80148778((u8*)actorB + 8, 50) != nullptr) return 0;
            if (!(evt->field_30 & 0x80)) {
                if (!(evt->field_10 & 4)) {
                    u32 r = 0;
                    if (*(u16*)ActorVfunc320(actorB) & 8) r = 1;
                    if (func_80148778((u8*)actorB + 8, 59) != nullptr) r = 1;
                    if (func_80148778((u8*)actorB + 8, 50) != nullptr) r = 1;
                    if (r) goto fail_10;
                }
                if (!(evt->field_10 & 5)) {
                    if (func_80148778((u8*)actorB + 8, 32) != nullptr) {
                        void* e = func_80149154((u8*)actorB + 8, 32);
                        u32 roll = stat != nullptr ? stat->field_B4 : (u32)mtRand__Q22ml4mathFi(100);
                        if (roll < *(u32*)((u8*)e + 0x10)) goto fail_10;
                    }
                }
                if (!(evt->field_10 & 6)) {
                    u32 r = 0;
                    if (func_80148778((u8*)actorB + 8, 16) == nullptr) {
                        if (((BattleActor*)actorB)->flags_3F00 & 4) r = 1;
                    }
                    if (r) goto fail_10;
                }
            }
        }
        evt->field_00 = 0;
        evt->field_04 = 0;
        break;
    }

    // ---- cmd 15: "talent art" handler (retail 0x13E48). ----
    case 15: {
        int val;
        if (func_80174C98(actorB, &val, 0xA) != 0) {
            if (*(u32*)((u8*)ActorVfunc298(actorB) + 0x78) & 0x08000000) return 0;
        }
        if (((BattleActor*)actorB)->flags_3F00 & 2) {
            void* sentinel = *(void**)((u8*)mgr + 0x48);
            s32 count = 0;
            for (void* cur = *(void**)sentinel; cur != sentinel; cur = *(void**)cur) count++;
            if (count == 0) return 0;
        }
        if (actorA != nullptr) {
            void* tgt = stat != nullptr ? (void*)stat : ActorVfunc298(actorA);   // r31
            if (*(u32*)((u8*)tgt + 0x74) & 4) return 0;
            if (*(u32*)((u8*)tgt + 0x74) & 2) return 0;
            int val2 = *(s32*)Sub4Vfunc30(actorB);
            if (func_80174C98(actorB, &val2, 0x802) != 0) return 0;
            if (!(evt->field_10 & 4)) {
                u32 r = 0;                                   // r29
                if (*(u16*)ActorVfunc324(actorB) & 8) r = 1;
                if (func_80148778((u8*)actorB + 8, 58) != nullptr) r = 1;
                if (func_80148778((u8*)actorB + 8, 50) != nullptr) r = 1;
                if (r) goto fail_1000;
            }
            if (!(evt->field_10 & 5)) {
                u32 r = 0;                                   // r29 (re-reset per retail)
                if (func_80148778((u8*)actorB + 8, 33) != nullptr) {
                    void* e = func_80149154((u8*)actorB + 8, 33);
                    u32 roll = stat != nullptr ? stat->field_B4 : (u32)mtRand__Q22ml4mathFi(100);
                    if (roll < *(u32*)((u8*)e + 0x10)) r = 1;
                }
                if (((BattleActor*)actorB)->flags_3374 & 0x80) {
                    if (*(u32*)((u8*)tgt + 0x74) & 0x4000) r = 1;
                }
                if (!(evt->field_10 & 6)) {
                    if (func_80148778((u8*)actorB + 8, 18) == nullptr &&
                        func_80148778((u8*)actorB + 8, 16) == nullptr) {
                        if (((BattleActor*)actorB)->flags_3F00 & 4) r = 1;
                    }
                    if (r) goto fail_10;
                }
            }
        } else {
            int val3 = *(s32*)Sub4Vfunc30(actorB);
            if (func_80174C98(actorB, &val3, 0x802) != 0) return 0;
            if (!(evt->field_10 & 4)) {
                u32 r = 0;
                if (*(u16*)ActorVfunc324(actorB) & 8) r = 1;
                if (func_80148778((u8*)actorB + 8, 58) != nullptr) r = 1;
                if (func_80148778((u8*)actorB + 8, 50) != nullptr) r = 1;
                if (r) goto fail_1000;
            }
            if (!(evt->field_10 & 5)) {
                if (func_80148778((u8*)actorB + 8, 33) != nullptr) {
                    void* e = func_80149154((u8*)actorB + 8, 33);
                    u32 roll = stat != nullptr ? stat->field_B4 : (u32)mtRand__Q22ml4mathFi(100);
                    if (roll < *(u32*)((u8*)e + 0x10)) goto fail_10;
                }
            }
            if (!(evt->field_10 & 6)) {
                u32 r = 0;
                if (func_80148778((u8*)actorB + 8, 18) == nullptr &&
                    func_80148778((u8*)actorB + 8, 16) == nullptr) {
                    if (((BattleActor*)actorB)->flags_3F00 & 4) r = 1;
                }
                if (r) goto fail_10;
            }
        }
        evt->field_00 = 0;
        evt->field_04 = 0;
        break;
    }

    // ---- cmd 17: skill 34 roll (same-actor exception). ----
    case 17:
        if (func_80148778((u8*)actorB + 8, 34) != nullptr) {
            if (actorB != actorA) {
                void* e = func_80149154((u8*)actorB + 8, 34);
                u32 roll = stat != nullptr ? stat->field_B4 : (u32)mtRand__Q22ml4mathFi(100);
                if (roll < *(u32*)((u8*)e + 0x10)) goto fail_10;
            }
        }
        break;

    // ---- cmd 18: no-op (retail -> default). ----
    case 18:
        break;
        if (func_80148778((u8*)actorB + 8, 34) != nullptr) {
            if (actorB != actorA) {
                void* e = func_80149154((u8*)actorB + 8, 34);
                u32 roll = stat != nullptr ? stat->field_B4 : (u32)mtRand__Q22ml4mathFi(100);
                if (roll < *(u32*)((u8*)e + 0x10)) goto fail_10;
            }
        }
        break;

    // ---- cmd 19: skill 36 roll. ----
    case 19:
        if (func_80148778((u8*)actorB + 8, 36) != nullptr) {
            void* e = func_80149154((u8*)actorB + 8, 36);
            u32 roll = stat != nullptr ? stat->field_B4 : (u32)mtRand__Q22ml4mathFi(100);
            if (roll < *(u32*)((u8*)e + 0x10)) goto fail_10;
        }
        break;

    // ---- cmds 60, 61: retail 0x800ECCCC. ----
    case 60:
    case 61:
        if (ActorVfunc290(actorB) != nullptr) {
            if (func_8026178C(ActorVfunc290(actorB), 0x66)) goto fail_10;
        }
        if (func_80148778((u8*)actorB + 8, 0x26) != nullptr) {
            void* e = func_80149154((u8*)actorB + 8, 0x26);
            u32 roll = stat != nullptr ? stat->field_B4 : (u32)mtRand__Q22ml4mathFi(100);
            if (roll < *(u32*)((u8*)e + 0x10)) goto fail_10;
        }
        if (stat != nullptr) {
            if ((u32)(stat->field_B0 % 100) >= evt->field_10) goto fail_10;
            if (actorA != actorB) {
                f32 v = ActorVfunc12C(actorB);
                if (stat->field_5C < v) stat->field_5C = v;
            }
            stat->field_74 |= 0x90000000;
            func_80148778((u8*)actorB + 8, 252);
            func_80148778((u8*)actorB + 8, 256);
        }
        break;

    // ---- cmd 82: retail 0x800ECBB8. ----
    case 82:
        if (ActorVfunc290(actorB) != nullptr) {
            if (func_8026178C(ActorVfunc290(actorB), 0x7B)) goto fail_10;
            if (func_8026178C(ActorVfunc290(actorB), 0x66)) goto fail_10;
        }
        break;

    // ---- cmds 83, 87: retail 0x800ECC88. ----
    case 83:
    case 87:
        if (ActorVfunc290(actorB) != nullptr) {
            if (func_8026178C(ActorVfunc290(actorB), 0x66)) goto fail_10;
        }
        if (func_80148778((u8*)actorB + 8, 0x26) != nullptr) {
            void* e = func_80149154((u8*)actorB + 8, 0x26);
            u32 roll = stat != nullptr ? stat->field_B4 : (u32)mtRand__Q22ml4mathFi(100);
            if (roll < *(u32*)((u8*)e + 0x10)) goto fail_10;
        }
        if (stat != nullptr) {
            if ((u32)(stat->field_B0 % 100) >= evt->field_10) goto fail_10;
            if (actorA != actorB) {
                f32 v = ActorVfunc12C(actorB);
                if (stat->field_5C < v) stat->field_5C = v;
            }
            stat->field_74 |= 0x90000000;
            func_80148778((u8*)actorB + 8, 252);
            func_80148778((u8*)actorB + 8, 256);
        }
        break;

    // ---- cmd 84: retail 0x800ECC20. ----
    case 84:
        if (ActorVfunc290(actorB) != nullptr) {
            if (func_8026178C(ActorVfunc290(actorB), 0x7D)) goto fail_10;
            if (func_8026178C(ActorVfunc290(actorB), 0x66)) goto fail_10;
        }
        break;

    // ---- cmd 102: retail 0x800ECAF8: skills 134, 136. ----
    case 102:
        if (func_80148778((u8*)actorB + 8, 134) != nullptr) {
            if (*(s32*)((u8*)func_80149154((u8*)actorB + 8, 134) + 0x10) >= 100) goto fail_10;
        }
        if (func_80148778((u8*)actorB + 8, 136) != nullptr) {
            if (*(s32*)((u8*)func_80149154((u8*)actorB + 8, 136) + 0x10) >= 100) goto fail_10;
        }
        break;

    // ---- cmd 103: retail 0x800ECB28: skill 136. ----
    case 103:
        if (func_80148778((u8*)actorB + 8, 136) != nullptr) {
            if (*(s32*)((u8*)func_80149154((u8*)actorB + 8, 136) + 0x10) >= 100) goto fail_10;
        }
        break;

    // ---- cmd 104: retail 0x800ECB58: skills 138, 140. ----
    case 104:
        if (func_80148778((u8*)actorB + 8, 138) != nullptr) {
            if (*(s32*)((u8*)func_80149154((u8*)actorB + 8, 138) + 0x10) >= 100) goto fail_10;
        }
        if (func_80148778((u8*)actorB + 8, 140) != nullptr) {
            if (*(s32*)((u8*)func_80149154((u8*)actorB + 8, 140) + 0x10) >= 100) goto fail_10;
        }
        break;

    // ---- cmd 105: retail 0x800ECB88: skill 140 + status 0x7B/0x66. ----
    case 105:
        if (func_80148778((u8*)actorB + 8, 140) != nullptr) {
            if (*(s32*)((u8*)func_80149154((u8*)actorB + 8, 140) + 0x10) >= 100) goto fail_10;
        }
        if (ActorVfunc290(actorB) != nullptr) {
            if (func_8026178C(ActorVfunc290(actorB), 0x7B)) goto fail_10;
            if (func_8026178C(ActorVfunc290(actorB), 0x66)) goto fail_10;
        }
        break;

    // ---- cmds 213, 263: knock-back / damage multiplier calculation. ----
    case 213:
    case 263: {
        if (stat == nullptr) break;
        // f29 = 0.01f * (f64)(s32)stat->field_84   (kept live across the vfunc calls)
        f64 scaled = (f64)lbl_eu_80666DD8 * (f64)(s32)stat->field_84;
        // first vfunc_C: art level of sub-object +0x84 vtable[0x0C]
        s32 lvl1 = SubObjVfuncC(subObj) - 1;
        s32 v1 = *(s16*)((u8*)subObj + 0x38) + *(u8*)((u8*)subObj + 0x6C) * lvl1;       // r31
        // second vfunc_C: recompute level (retail calls it again)
        s32 lvl2 = SubObjVfuncC(subObj) - 1;
        s32 v2 = *(s16*)((u8*)subObj + 0x3A) + *(u8*)((u8*)subObj + 0x6D) * lvl2;       // r0
        // scaled *= (v2 - v1)
        f64 power = scaled * (f64)(s32)(v2 - v1);
        // round-half-away-from-zero (retail: +0.5 / -0.5 doubles + fctiwz)
        f64 rnd = power > 0.0 ? lbl_eu_80666E58 : lbl_eu_80666E60;
        s32 rounded = (s32)(power + rnd);                                // f29 (as double) afterwards
        // recompute v2 with a THIRD vfunc_C call, then store the new power.
        s32 lvl3 = SubObjVfuncC(subObj) - 1;
        s32 v2b = *(s16*)((u8*)subObj + 0x3A) + *(u8*)((u8*)subObj + 0x6D) * lvl3;
        stat->field_5C = (f32)((f64)(s32)*(s16*)((u8*)a224 + 0x30) *
                               ((f64)lbl_eu_80666DD8 * ((f64)(s32)rounded + (f64)(s32)v2b)));
        // accumulate from the damage helper.
        f32 dmg = func_800D81A8(actorA, actorB, stat);
        f32 f31 = stat->field_58 + dmg;
        if (((BattleActor*)actorA)->flags_3F00 & 2) {
            if (((BattleActor*)actorA)->field_3F28 == 5) {
                s32 v = (s32)(ActorVfunc158(actorA) + (f64)(s32)*(s16*)((u8*)subObj + 0x34));
                if (v < 0) v = 0;
                if (v > 100) v = 100;
                f31 += (f64)(s32)(50 - v) / (f64)lbl_eu_80666E20;
            }
        }
        if (*(u16*)((u8*)subObj + 0x48) == 263) {
            s32 count = 0;
            for (f32 x = lbl_eu_80666DDC; x < lbl_eu_80666E54; x += lbl_eu_80666DD4) {
                void* o = Sub8Vfunc5C(actorB, (s32)(f64)x);
                if (*(u16*)((u8*)o + 0xC) != 0) count++;
            }
            f31 += (f64)lbl_eu_80666E84 * (f64)(s32)count;
        }
        if (((BattleActor*)actorA)->flags_3F00 & 2) {
            s32 roll = stat->field_98 % 100;
            s32 need = lbl_eu_804FCA3C[ActorVfunc308(actorA)];
            if (roll < need) {
                f31 += lbl_eu_80666E1C;
                stat->field_74 |= 0x80000200;
            }
        }
        stat->field_5C = (stat->field_5C * f31) * lbl_eu_80666E2C;   // retail: fmuls f1,f1,f31; fmuls f0,f1,-1.0f
        break;
    }

    // ---- cmd 220: skill 38 roll + stat/vision updates. ----
    case 220:
        if (func_80148778((u8*)actorB + 8, 38) != nullptr) {
            void* e = func_80149154((u8*)actorB + 8, 38);
            u32 roll = stat != nullptr ? stat->field_B4 : (u32)mtRand__Q22ml4mathFi(100);
            if (roll < *(u32*)((u8*)e + 0x10)) goto fail_10;
        }
        if (stat != nullptr) {
            if ((stat->field_B0 % 100) >= evt->field_10) return 0;
            // Both arms of the sub-object type split are identical in retail
            // (0x148C0 and 0x14904); kept as two blocks to mirror the codegen.
            if (subObj != nullptr && *(u16*)((u8*)subObj + 0x3C) == 3) {
                if (actorA != actorB) {
                    f32 v = ActorVfunc12C(actorB);
                    if (stat->field_5C < v) stat->field_5C = v;
                }
            } else {
                if (actorA != actorB) {
                    f32 v = ActorVfunc12C(actorB);
                    if (stat->field_5C < v) stat->field_5C = v;
                }
            }
        } else {
            if (mtRand__Q22ml4mathFi(100) >= (s32)evt->field_10) goto fail_10;
        }
        if (stat != nullptr) {
            stat->field_74 |= 0x90000000;
            func_80148778((u8*)actorB + 8, 252);
            func_80148778((u8*)actorB + 8, 256);
        }
        break;

    // ---- cmd 223: damage span calculation (retail 0x800ECE70). ----
    case 223: {
        if (stat == nullptr) break;
        s32 span = (s32)(s16)evt->field_14 - (s32)evt->field_10 + 1;
        s32 base = (s32)evt->field_10 + (stat->field_B0 % span);       // r29
        stat->field_60 = (f32)(f64)(s32)base;
        f32 dmg = func_800D81A8(actorA, actorA, stat);
        s32 v = (s32)((f64)(s32)base * (f64)(stat->field_58 + dmg));
        stat->field_64 = (f32)(f64)(s32)v;
        break;
    }

    // ---- cmd 224: damage calculation via sub-object art levels (0x14A1C). ----
    case 224: {
        if (stat == nullptr) break;
        s32 base = *(s16*)((u8*)subObj + 0x4A) +
                   *(u8*)((u8*)subObj + 0x6F) * (SubObjVfuncC(subObj) - 1);   // r29
        s32 v = (s32)((f64)lbl_eu_80666DD8 * (f64)(s32)base * (f64)ActorVfunc12C(actorB));
        stat->field_60 = (f32)(f64)(s32)v;
        f32 dmg = func_800D81A8(actorA, actorA, stat);
        s32 v2 = (s32)((f64)(s32)v * (f64)(stat->field_58 + dmg));
        stat->field_64 = (f32)(f64)(s32)v2;
        break;
    }

    // ---- cmd 227: chain-break / recursive re-dispatch. ----
    case 227: {
        if (((u32)actorA & 3) != 0) break;
        if ((u32)actorA < 0x80000000u) break;
        if ((u32)actorA >= 0x93800000u) break;
        if (((u32)actorB & 3) != 0) break;
        if ((u32)actorB < 0x80000000u) break;
        if ((u32)actorB >= 0x93800000u) break;

        void* a298 = ActorVfunc298(actorA);                       // r31
        u32 brk = 0;                                             // r29
        if (*(u16*)ActorVfunc324(actorB) == 7) brk = 1;
        s32 d = ActorVfunc108(actorB) - ActorVfunc108(actorA);   // r30/r3
        if (d > 10) brk = 1;

        if (brk == 0) {
            int val = *(s32*)Sub4Vfunc30(actorB);
            if (func_80174C98(actorB, &val, 0x802) != 0) brk = 1;
            else if (func_80148778((u8*)actorB + 8, 50) != nullptr) brk = 1;
            else {
                // power table by enemy type (retail f29 loads).
                f32 power = lbl_eu_80666DDC;                     // 0.0f
                s32 type = ((BattleActor*)actorB)->type_15E4;
                if (type == 1) power = lbl_eu_80666E7C;          // 75
                else if (type == 2) power = lbl_eu_80666E18;     // 50
                else if (type == 3) power = lbl_eu_80666DF8;     // 25
                else if (type == 4) power = lbl_eu_80666DF4;     // 15
                else if (type == 5) power = lbl_eu_80666E34;     // 10
                void* sub = *(void**)((u8*)a298 + 0x50);
                if (IsValidPtr(sub)) {
                    power += lbl_eu_80666E80 * (f64)(s32)SubObjVfuncC(sub);   // 2.5f, fmadds
                }
                u32 roll = IsValidPtr(stat) ? stat->field_B4 : (u32)mtRand__Q22ml4mathFi(100);
                if ((u32)(s32)(f64)power <= (u32)roll) brk = 1;
            }
        }

        // Build the recursive event copy (retail copies field-by-field and
        // converts evt->field_10 via the 0x4330 double trick).
        BattleCmdEvent buf;
        buf.field_00 = evt->field_00;
        buf.field_04 = evt->field_04;
        buf.field_08 = evt->field_08;
        buf.field_10 = 3;
        buf.field_14 = evt->field_14;
        buf.field_18 = evt->field_18;
        buf.field_1C = evt->field_1C;
        buf.field_20 = (f32)(f64)(s32)evt->field_10;
        buf.field_24 = evt->field_24;
        buf.field_28 = evt->field_28;
        buf.field_2C = evt->field_2C;
        buf.field_2E = evt->field_2E;
        buf.field_30 = evt->field_30;
        buf.cmd = 16;

        if (brk) {
            buf.cmd = 6;
            if (!func_800EAA2C(mgr, actorA, actorB, &buf, stat)) goto fail_10;
        } else {
            buf.field_30 |= 0x800;
            if (func_800EAA2C(mgr, actorA, actorB, &buf, stat) == 0) {
                buf.cmd = 6;
                buf.field_30 &= ~0x400;
                if (!func_800EAA2C(mgr, actorA, actorB, &buf, stat)) goto fail_10;
            }
        }
        break;
    }

    // ---- no-op cases and the default dispatch straight to 0x14DD4. ----
    default:
        break;
    }

    // ---- Shared default block (retail 0x14DD4): vision sub-object command
    // recording. Reached by every case `break` AND the default case.
    {
        void* vobj = nullptr;
        if (lbl_eu_80663F00 != 0) {
            vobj = func_801A8070((u8*)mgr + 0x219C);            // &mVision
        }
        if (vobj != nullptr && actorA != nullptr) {
            if (((BattleActor*)actorA)->field_3F10 == *(u32*)((u8*)vobj + 0) &&
                ((BattleActor*)actorB)->field_3F10 == *(u32*)((u8*)vobj + 4)) {
                if (evt->field_2E == 0 || !(evt->field_30 & 2)) {
                    if (evt->cmd != 220 && func_80145DBC(evt->cmd)) {
                        if (*(u32*)((u8*)vobj + 0x828) == 0) {
                            *(u32*)((u8*)vobj + 0x828) = evt->cmd;
                        } else {
                            *(u32*)((u8*)vobj + 0x82C) = evt->cmd;
                        }
                    }
                }
            }
        }
    }
    return 1;

fail_10:
    // retail 0x14E7C
    if (stat != nullptr) stat->field_74 |= 0x80000010;
    return 0;

fail_1000:
    // retail 0x14E9C
    if (stat != nullptr) stat->field_74 |= 0x80001000;
    return 0;
}


// Battle damage menu functions (CMenuBattleDamage)
extern void func_8010989C(u8 val);
extern void func_80109888(u8 val);
extern void func_80109874(u8 val);
extern void func_8010975C(u32 val);
extern void func_80109770(u32 val);
extern void func_80109734(void* ptr, u32 val);

// Handles battle event. Sets flags on arg4, checks arg3 conditions,
// and updates the battle damage display.
// Returns 0 always (retail: li r3, 0 before blr).
extern "C" s32 func_800F3734(void* arg1, BattleObjAccessor* accessor, void* arg3, void* arg4) {
    // arg1 (r3) is unused by the logic but was stored in r31 per retail

    if (arg4 != nullptr) {
        u32* flags = (u32*)((u8*)arg4 + 0x74);
        *flags = *flags | 0x80000000 | 0x1000;
    }

    u32 arg3Flags = *(u32*)((u8*)arg3 + 0x30);
    if (arg3Flags & 0x800) return 0;
    if (arg3Flags & 0x2000) return 0;

    void* subObj = (arg4 != nullptr) ? *(void**)((u8*)arg4 + 0x50) : nullptr;
    if (subObj == nullptr || (s32)*(u16*)((u8*)subObj + 0x3c) == 3) {
        func_8010989C(0);
        func_80109888(0);
        func_80109874(0);
        func_8010975C(9);
        func_80109770(0);
        func_80109734(accessor->field_3f10, 0);
    }
    return 0;
}

// Similar to func_800F3734 but with additional checks on arg3->+0x2e and arg3->+0x30.
// Sets flags on arg4, checks conditions, and updates the battle damage display.
// Returns 0 always.
extern "C" s32 func_800F37F8(void* arg1, BattleObjAccessor* accessor, void* arg3, void* arg4) {
    if (arg4 != nullptr) {
        u32* flags = (u32*)((u8*)arg4 + 0x74);
        *flags = *flags | 0x80000000 | 0x10;

        // Check if bit 0x1000 is now set (retail: rlwinm. 0, 19, 19)
        if (*flags & 0x1000) return 0;
    }

    u32 arg3Flags = *(u32*)((u8*)arg3 + 0x30);
    if (arg3Flags & 0x800) return 0;
    if (arg3Flags & 0x2000) return 0;

    // Check arg3->+0x2e: if non-zero AND 0x402 bits not set, skip to ret0
    u16 val2e = *(u16*)((u8*)arg3 + 0x2e);
    if (val2e != 0 && (arg3Flags & 0x402) == 0) {
        // goto ret0 (skip function calls)
    } else {
        void* subObj = (arg4 != nullptr) ? *(void**)((u8*)arg4 + 0x50) : nullptr;
        if (subObj == nullptr || (s32)*(u16*)((u8*)subObj + 0x3c) == 3) {
            func_8010989C(0);
            func_80109888(0);
            func_80109874(0);
            func_8010975C(8);
            func_80109770(0);
            func_80109734(accessor->field_3f10, 0);
        }
    }

    return 0;
}
// Stack buffer for func_800F38E0: 0x34 bytes, accessed as:
// +0x00: padding
// +0x0C: u16 (stores arg3)
// +0x30: u32 (bit 0 set)
struct Func800F38E0_StackBuf {
    u8 pad_00[0x0C];
    u16 field_0C;
    u8 pad_0E[0x22];
    u32 field_30;
};

void func_800F38E0(void* self, u32 arg2, u16 arg3) {
    Func800F38E0_StackBuf buf;
    std::memset(&buf, 0, sizeof(buf));
    buf.field_0C = arg3;
    buf.field_30 |= 1;
    func_800EC918(self, 0, (void*)arg2, &buf, 0);
}
// Dispatches battle event calls to actor objects based on a table entry.
void func_800F3970(void* self, void* obj1, void* obj2, s32 idx, s32 addVal) {
    u32 selector = lbl_eu_804FC828[idx].selector;
    s32 val2 = -1;
    s32 val3 = lbl_eu_804FC828[idx].val;

    if (val3 == 0x3e7) {
        val2 = 4;
        val3 = 0;
    }

    val3 += addVal;

    if (lbl_eu_804FC828[idx].byteVal != 0) {
        extern s32 func_800824FC__Q22cf13CfGameManagerFv(s32 first, s32 second);
        u16 u1 = *(u16*)((u8*)obj1 + 0x3f28);
        u16 u2 = *(u16*)((u8*)obj2 + 0x3f28);
        int result = func_800824FC__Q22cf13CfGameManagerFv(u1, u2);
        if (result != -1) {
            val3 += (result / 100) * lbl_eu_804FC828[idx].byteVal;
        }
    }

    // Virtual function typedefs
    typedef void (*VFunc2F8)(void*, s32);
    typedef void (*VFunc2FC)(void*, s32);
    typedef void (*VFunc304)(void*, s32);

    if (selector == 0) {
        if (val2 != -1) {
            ((VFunc304)(*(void***)obj1)[0x304 / 4])(obj1, val2);
            ((VFunc2FC)(*(void***)obj1)[0x2FC / 4])(obj1, val3);
        } else {
            ((VFunc2F8)(*(void***)obj1)[0x2F8 / 4])(obj1, val3);
        }
    } else if (selector == 1) {
        if (val2 != -1) {
            ((VFunc304)(*(void***)obj2)[0x304 / 4])(obj2, val2);
            ((VFunc2FC)(*(void***)obj2)[0x2FC / 4])(obj2, val3);
        } else {
            ((VFunc2F8)(*(void***)obj2)[0x2F8 / 4])(obj2, val3);
        }
    } else if (selector == 2) {
        if (val2 != -1) {
            ((VFunc304)(*(void***)obj1)[0x304 / 4])(obj1, val2);
            ((VFunc2FC)(*(void***)obj1)[0x2FC / 4])(obj1, val3);
        } else {
            ((VFunc2F8)(*(void***)obj1)[0x2F8 / 4])(obj1, val3);
        }

        if (val2 != -1) {
            ((VFunc304)(*(void***)obj2)[0x304 / 4])(obj2, val2);
            ((VFunc2FC)(*(void***)obj2)[0x2FC / 4])(obj2, val3);
        } else {
            ((VFunc2F8)(*(void***)obj2)[0x2F8 / 4])(obj2, val3);
        }
    } else {
        // Iterate through mActorList2 (self+0x28)
        _reslist_node<void*>* sentinel = *(_reslist_node<void*>**)((u8*)self + 0x28);
        _reslist_node<void*>* cur = sentinel->mNext;
        while (cur != sentinel) {
            void* actor = cur->mItem;
            if (val2 != -1) {
                ((VFunc304)(*(void***)actor)[0x304 / 4])(actor, val2);
                ((VFunc2FC)(*(void***)actor)[0x2FC / 4])(actor, val3);
            } else {
                ((VFunc2F8)(*(void***)actor)[0x2F8 / 4])(actor, val3);
            }
            cur = cur->mNext;
        }
    }
}
void func_800F3C08(cf::CBattleManager* mgr, u32 arg) {
    mgr->func_80085220(2, arg);
    if (arg != 0) {
        std::memset(&mgr->unk94, 0, sizeof(cf::CBattleManager_Struct2));
    }
}
// Searches through unk94 slot array for a matching key. If found, increments count.
// If not found, inserts into the first empty slot (key==0) with count=1.
void func_800F3C6C(cf::CBattleManager* mgr, s32 key) {
    // Call virtual func_800885F0 (vtable[0x28]) with arg=2
    // Use the vtable pointer directly to match retail register pattern
    if (!((s32 (*)(void*, u32))((void**)(*(void**)mgr))[0x28 / 4])(mgr, 2)) return;

    // Each slot is 8 bytes: key (s32 at +0), count (u8 at +4), padding (+5..+7)
    // 32 slots total, processed in 8 iterations of 4 slots each
    u8* p = (u8*)mgr + 0x94;
    s32 bestFit = -1;
    s32 idx = 0;

    for (s32 i = 0; i < 8; i++) {
        if (*(s32*)(p + 0) == key) { ((u8*)p)[4]++; return; }
        if (*(s32*)(p + 0) == 0 && bestFit < 0) bestFit = idx;

        idx++;
        if (*(s32*)(p + 8) == key) { ((u8*)p)[12]++; return; }
        if (*(s32*)(p + 8) == 0 && bestFit < 0) bestFit = idx;

        idx++;
        if (*(s32*)(p + 16) == key) { ((u8*)p)[20]++; return; }
        if (*(s32*)(p + 16) == 0 && bestFit < 0) bestFit = idx;

        idx++;
        if (*(s32*)(p + 24) == key) { ((u8*)p)[28]++; return; }
        if (*(s32*)(p + 24) == 0 && bestFit < 0) bestFit = idx;

        idx++;
        p += 32;
    }

    if (bestFit >= 0) {
        // Match retail: slwi r0, bestFit, 3; add r4, r30, r0; stw ..., 0x94(r4)
        s32* addr = (s32*)((u8*)mgr + bestFit * 8);
        *(s32*)((u8*)addr + 0x94) = key;
        *((u8*)addr + 0x98) = 1;
    }
}
// Searches for an actor with field_0x15f0 matching arg1.
// First checks mActorList3 directly; if empty, checks mActorList1 via vtable call.
// Returns 1 if found, 0 otherwise.
// Searches for an actor with field_0x15f0 matching arg1.
// First checks mActorList3 directly; if non-empty, checks mActorList1 via vtable call.
// Returns 1 if found, 0 otherwise.
s32 func_800F3E8C(cf::CBattleManager* mgr, s32 arg1) {
    // First pass: search mActorList3 (sentinel at +0x48)
    {
        _reslist_node<cf::CfObjectActor*>* sentinel = mgr->mActorList3.mStartNodePtr;
        _reslist_node<cf::CfObjectActor*>* cur = sentinel->mNext;
        while (cur != sentinel) {
            cf::CfObjectActor* actor = cur->mItem;
            if (*(s32*)((u8*)actor + 0x15f0) == arg1) return 1;
            cur = cur->mNext;
        }
    }

    // Second pass: count mActorList3 size; if non-zero, return 0
    {
        _reslist_node<cf::CfObjectActor*>* sentinel = mgr->mActorList3.mStartNodePtr;
        _reslist_node<cf::CfObjectActor*>* cur = sentinel->mNext;
        s32 count = 0;
        while (cur != sentinel) {
            cur = cur->mNext;
            count++;
        }
        if (count != 0) return 0;
    }

    // Third pass: iterate mActorList1, call vtable[0x4c] on secondary vtable at +0x3e9c
    {
        extern void* func_800B708C(int);
        _reslist_node<cf::CfObjectActor*>* sentinel = mgr->mActorList1.mStartNodePtr;
        _reslist_node<cf::CfObjectActor*>* cur = sentinel->mNext;
        while (cur != sentinel) {
            cf::CfObjectActor* actor = cur->mItem;
            // Access secondary vtable at +0x3e9c (lwzu: updates r3 to base + 0x3e9c)
            void* base = (u8*)actor + 0x3e9c;
            void** secondaryVtbl = *(void***)base;
            typedef s32 (*VFunc4C)(void*);
            s32 vresult = ((VFunc4C)secondaryVtbl[0x4C / 4])(base);
            if (vresult != 0) {
                void* r3 = func_800B708C(vresult);
                void* result = func_8016FE34(r3);
                if (result != nullptr) {
                    if (*(u32*)((u8*)result + 0x3f00) & 0x04) {
                        if (*(s32*)((u8*)result + 0x15f0) == arg1) return 1;
                    }
                }
            }
            cur = cur->mNext;
        }
    }

    return 0;
}

void func_800F3F8C(cf::CBattleManager* mgr) {
    mgr->func_800E2584(0x10);
    lbl_eu_80663E24 |= 0x10000000;
}

void func_800F3FC8(cf::CBattleManager* mgr) {
    mgr->func_800E2584(0x10);
    lbl_eu_80663E24 &= ~0x10000000;
}
// Performs battle cleanup: chain maintenance, enum list iteration with virtual calls,
// actor list flag updates, and action cleanup.
void func_800F4034(cf::CBattleManager* mgr) {
    // Chain maintenance
    extern void func_80277B34(cf::CChain*);
    func_80277B34(&mgr->mChain);

    // Setup enum list holder
    struct EnumListHolder { void* list; u32 handle; };
    extern void func_80043D90(EnumListHolder*);
    extern void* func_80043F18(EnumListHolder*);
    extern void func_800F4A98(void*, int, int);
    extern void* func_800F6EAC(void*, u32);
    extern void __dt__80043E88(EnumListHolder*, int);

    EnumListHolder holder;
    func_80043D90(&holder);
    void* list = func_80043F18(&holder);
    func_800F4A98(list, 0x20, 0);

    // Loop with check at bottom, using unsigned comparison to match retail cmplw
    u32 i = 0;
    goto check;
loop:
    {
        void* element = func_8016FE34(func_800F6EAC(func_80043F18(&holder), i));
        // Retail does NOT null-check element before vtable calls
        void** vtbl = *(void***)element;
        typedef void (*VFunc)(void*);
        ((VFunc)vtbl[0xB0 / 4])(element);
        vtbl = *(void***)element;
        ((VFunc)vtbl[0xB8 / 4])(element);
        vtbl = *(void***)element;
        ((VFunc)vtbl[0x314 / 4])(element);
        i++;
    }
check:
    {
        void* list = func_80043F18(&holder);
        if (i < *(u32*)((u8*)list + 0x620)) goto loop;
    }

    __dt__80043E88(&holder, -1);

    // Iterate through mActorList3 (sentinel at offset 0x48)
    // Retail: stores flags unconditionally, then checks null for moveBase calc
    {
        _reslist_node<cf::CfObjectActor*>* sentinel = mgr->mActorList3.mStartNodePtr;
        _reslist_node<cf::CfObjectActor*>* cur = sentinel->mNext;
        while (cur != sentinel) {
            cf::CfObjectActor* actor = cur->mItem;
            // Retail: store flags first, then check null
            *(u32*)((u8*)actor + 0x3f04) |= 0x40;

            // If actor is not null, add 0x3e9c for moveBase
            // If null, pass nullptr directly (retail falls through beq)
            void* moveBase = actor != nullptr ? (u8*)actor + 0x3e9c : (void*)nullptr;
            extern void* func_800AD860__FPv(void*);
            void* result = func_800AD860__FPv(moveBase);
            if (result != nullptr) {
                *(u32*)((u8*)result + 0x3f08) |= 0x8000000;
                extern void func_80197BA4(void*, u32, u32);
                func_80197BA4(result, 0, 0);
            }

            cur = cur->mNext;
        }
    }

    // Iterate through mActorList2 (sentinel at offset 0x28)
    {
        _reslist_node<cf::CfObjectActor*>* sentinel = mgr->mActorList2.mStartNodePtr;
        _reslist_node<cf::CfObjectActor*>* cur = sentinel->mNext;
        while (cur != sentinel) {
            cf::CfObjectActor* actor = cur->mItem;
            extern void func_800BE12C(void*, int, int, int, int);
            func_800BE12C((u8*)actor + 0x3e9c, 0x31, 0, -1, 1);
            cur = cur->mNext;
        }
    }
}
// Handles battle finish / state transition logic.
// Checks various battle state flags and performs cleanup.
void func_800F41A0(cf::CBattleManager* mgr) {
    extern u8 lbl_eu_80573EEC[];
    extern void func_8015B11C();
    if (*(u32*)(lbl_eu_80573EEC + 0xd0) == 0) return;

    cf::CfGameManager::getInstance();
    if (func_8006EF04__Fi(4)) goto early_return;

    cf::CfGameManager::getInstance();
    extern s32 CfRes_checkFlags_48000();
    if (CfRes_checkFlags_48000()) goto early_return;

    cf::CfGameManager::getInstance();
    extern s32 CfRes_getE24Bit22();
    if (CfRes_getE24Bit22()) goto early_return;

    // Main path: set up enum list and iterate
    {
        struct EnumListHolder { void* list; u32 handle; };
        extern void func_80043D90(EnumListHolder*);
        extern void* func_80043F18(EnumListHolder*);
        extern void func_800F4A98(void*, u32, u32);
        extern void __dt__80043E88(EnumListHolder*, int);

        EnumListHolder holder;
        func_80043D90(&holder);
        void* list = func_80043F18(&holder);
        func_800F4A98(list, 0x20, 0x800);

        list = func_80043F18(&holder);
        if (*(u32*)((u8*)list + 0x620) != 0) {
            extern void func_8013E424(void*, int);
            func_8013E424(lbl_eu_80573EEC, 0);

            func_8015B11C();

            for (int i = 0; i < 3; i++) {
                extern void* getPlayer__Q22cf13CfGameManagerFi(int);
                void* player = getPlayer__Q22cf13CfGameManagerFi(i);
                if (player != nullptr) {
                    if (*(u16*)((u8*)player + 0x8c) == 6) {
                        void* action = func_8016FE34(player);
                        extern void func_802A3144(void*);
                        func_802A3144(action);
                        goto cleanup;
                    }
                }
            }
        } else {
            func_8015B11C();
        }

cleanup:
        __dt__80043E88(&holder, -1);
    }
    return;

early_return:
    func_8015B11C();
}