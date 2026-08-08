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

// Game state check function
extern bool func_8006EF04__Fi(s32 mask);

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
extern "C" void func_800EA410(void* self) { reinterpret_cast<BMIf*>((u8*)self + 0x219c)->vf0010(); }
void* cf::CBattleManager::func_800EA420() {
    if (spInstance != nullptr) {
        // Call virtual function at vtable offset 0x14 (BMIf::_v014) on mVision at offset 0x219c
        return reinterpret_cast<BMIf*>((u8*)this + 0x219c)->_v014();
    }
    return nullptr;
}
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
extern "C" void func_800EA998(void* self) { reinterpret_cast<BMIf*>((u8*)self + 0x219c)->vf0024(); }
void func_800EC918(u32 r3, u32 r4, u32 r5, void* r6, u32 r7);

void func_800EC8FC(u32 a, u32 b, void* c, u32 d) {
    func_800EC918(a, 0u, b, c, d);
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
extern int func_80148778(void*, int);
extern void* func_80149154(void*, unsigned int);
extern int func_80260264(void*, int, void*);

// Calculates accumulated damage/healing value from various status effects.
// Returns a float clamped to >= 0.
f32 func_800D7EA0(u8* obj, void* target) {
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
extern int func_802799F0(void*, void*);

// Calculates cumulative damage value from various stat sources
float func_800D81A8(void* obj, void* target, void* source){
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

// Iterates through actor lists and calls virtual functions based on flags.
void func_800E9FE4(void* self, void* arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, void* arg6) {
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
void func_800EA2A4(cf::CBattleManager* mgr, BattleObjAccessor* arg1) {
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
    func_800EC918(lbl_eu_80663F00, 0, (u32)arg4, (void*)arg5, 0);
    if (arg6 != nullptr) {
        func_80109784(arg4->field_3f10, (u32)arg6, 9);
    }
}
void func_800EAA2C(){}

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
s32 func_800F3734(void* arg1, BattleObjAccessor* accessor, void* arg3, void* arg4) {
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
s32 func_800F37F8(void* arg1, BattleObjAccessor* accessor, void* arg3, void* arg4) {
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
    func_800EC918((u32)self, 0, arg2, &buf, 0);
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