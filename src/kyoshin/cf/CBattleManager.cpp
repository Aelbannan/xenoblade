// (func_800BE12C now has a single unified decl on CfObjectMove.hpp; only one
// form exists, so no pre-include rename is needed here.)
// getArtsSlotRC: CAIAction.hpp and CChainActorList.hpp declare conflicting
// C-linkage signatures; this TU calls neither, so rename it out of the way
// for the duration of the include block.
// getArtsSlotRC: CAIAction.hpp and CChainActorList.hpp declare conflicting
// C-linkage signatures; this TU calls neither, so rename CAIAction.hpp's
// variant out of the way for just that include.
#define getArtsSlotRC cbmgr_artsSlotRcUnused
#define getArtsParamRC2 cbmgr_artsParamRc2Unused
#define func_8009EC9C cbmgr_func8009EC9CUnused
#include "kyoshin/cf/object/CAIAction.hpp"
#undef getArtsSlotRC
#undef getArtsParamRC2
#undef func_8009EC9C
// func_8025FB10: u32 form (CChainActorList.hpp) is canonical; CChainTimer.hpp's
// int form conflicts. Same pre-include + rename recipe as CMenuBattleMode.cpp.
// func_800824FC: renames CChainActorList's int form (this TU uses the s32
// spelling shared with CfGameManagerUnityHelpers.hpp).
#define func_800824FC__Q22cf13CfGameManagerFv cbmgr_800824FCUnused
#include "kyoshin/cf/chain/CChainActorList.hpp"
#undef func_800824FC__Q22cf13CfGameManagerFv
#define func_8025FB10 cbmgr_8025FB10Unused
// lbl_eu_8066A1F8: CChain.hpp declares a const f32 variant that conflicts
// with this TU's plain-float decls (CfObjectEne.hpp / local). Rename it for
// the duration of this aggregate include.
#define lbl_eu_8066A1F8 cbmgr_pi8066A1F8Unused
#include "kyoshin/cf/CBattleManager.hpp"
#undef lbl_eu_8066A1F8
#undef func_8025FB10
#include "kyoshin/cf/CfMapItemManager.hpp"
#include "monolib/scn/CScnTimeApi.hpp"
#include "kyoshin/cf/CBattleManager.hpp"
#include "monolib/math/CVec3.hpp"

struct BMIf {
    virtual void _v008(); virtual void _v00C(); virtual void vf0010();
    virtual void* _v014(); virtual void _v018(); virtual void _v01C();
    virtual void _v020(void* arg);
    virtual void vf0024();
};
#include "kyoshin/UnkClass_805764CC.hpp"
#include "kyoshin/cf/object/CfObjectPc.hpp"
// (func_800BB618/func_800BCFA0/getUnk80664658/BasicSound + the C-ABI import
// names below now resolve via their single owner-header decls; only the
// other agents' guards (lbl_eu_*) remain.)
#include "kyoshin/cf/object/CfObjectEne.hpp"
// (func_800821F8__Q22cf13CfGameManagerFv: CfSoundMan.hpp now carries the
// single winning decl from CfGameManagerApi.hpp - no pre-include rename.)
#include "kyoshin/cf/CfSoundMan.hpp"
// The include/kyoshin/cf/CfGameManager.hpp C-ABI import tail conflicts with
// the cfsys/chain walker decls already visible in this TU for the few names
// it shares; guard only those (call sites bind to the global-scope decls,
// same retail symbol names). (func_80496288 now has a single unified decl.)
// (func_800D9354: CfGameManager.hpp's copy is gone; single decl on
// kyoshin/cf/CBattleManagerApi.hpp, included via CBattleManager.hpp above.)
// (func_800AD860: CfObjectMove.hpp and CfGameManager.hpp now carry the
// identical void*(void*) C++-linkage form - no guard needed.)
#include "kyoshin/cf/CfGameManager.hpp"
// (func_801412D0 is owned by kyoshin/CUIWindowManager.hpp; single decl.)
#include "monolib/work.hpp"

// Arts-data row returned by func_8009EC9C: the per-entry table scanned by
// func_800C0174 starts at +0x1C (passed to func_8009D7E4). C-linkage so the
// call reloc binds to the retail-unmangled name (declared in CArtsInfo.hpp).
extern "C" void* func_8009EC9C(u32);
extern void func_8009D7E4(UNKTYPE* r3, u32 r4);

// Status add/remove helper (retail func_800BE12C). The shared headers carry
// two incompatible extern "C" forms ((u8*,...) and (void*,...)); this TU uses
// the u8* form throughout, declared once here.

// Game state check function (name already contains __Fi; extern "C" prevents
// MWCC from double-mangling it to func_8006EF04__Fi__Fi).
extern "C" bool func_8006EF04__Fi(int mask);

// Initialise all vision slots/effects (retail func_801A380C). Free function,
// not a CVision member - declared here because CVision.hpp stays opaque.
// C linkage: the retail symbols for these reset helpers are unmangled, and
// they are called as free functions (not member calls) so the call relocs
// keep the retail names.
extern "C" void func_801A380C(cf::CVision* self);
extern "C" void __ct__80192C10(cf::UnkClass_80192BF4* self);
extern "C" void func_8027728C(cf::CChain* self);
extern "C" void func_8027D1A4(cf::UnkClass_800D8DBC* self);
extern "C" void func_801BA1DC(cf::CSuddenCommu* self);
// Generic-object holder register/deregister (retail unmangled: the static
// member UnkClass_805764CC::func_800B07E8 would mangle to
// func_800B07E8__17UnkClass_805764CCFv; the Fv-suffix here is the retail
// symbol as-is).
extern "C" UnkClass_805764CC* func_800B07E8__Fv();
extern "C" void func_800B8804__FPvPQ22cf13IFactoryEvent(UnkClass_805764CC* holder, cf::IFactoryEvent* ev);
extern "C" void func_800B88E0(UnkClass_805764CC* holder, cf::IFactoryEvent* ev);
// Cross-actor id lookup used by func_800F3970's dispatch table (/100 scale).
extern "C" s32 func_800824FC__Q22cf13CfGameManagerFv(s32 first, s32 second);

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
    // Restore the base-class vtable under the retail symbol name (the
    // compiler's auto vptr store would emit an auto-mangled __vt__ name).
    *(void**)this = (void*)lbl_eu_8052BD68;
    _reslist_node<cf::CfObjectActor*>* old;
    _reslist_node<cf::CfObjectActor*>* cur = mStartNodePtr->mNext;
    while (cur != mStartNodePtr) {
        old = cur;
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
    // Restore the base-class vtable under the retail symbol name.
    *(void**)this = (void*)lbl_eu_8052BD44;
    _reslist_node<cf::IBattleEvent*>* old;
    _reslist_node<cf::IBattleEvent*>* cur = mStartNodePtr->mNext;
    while (cur != mStartNodePtr) {
        old = cur;
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

// Attack sound-id table (retail .rodata 0x804FCA08; 10 known entries of the
// 0x34-byte symbol). Defined with the retail name so the lwzx reloc binds.
u32 lbl_eu_804FCA08[] = {
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

// 8-byte holder for the enum-list helpers (func_80043D90 ctor /
// func_80043F18 / __dt__80043E88); the list returned by func_80043F18 has
// its element count at +0x620.
struct EnumListHolder {
    void* list;
    u32 handle;
};

namespace cf{
    //Plays attack sound effects when a player character hits an enemy?
    void func_800D7A04(CfObjectPc* pObjectPc, CfObjectEne* pObjectEne){
        CActorParam_UnkStruct1* param = pObjectPc->CActorParam_UnkVirtualFunc129();
        BattleObjAccessor* ene = (BattleObjAccessor*)pObjectEne;

        if(param->mFlagsArray[0].flags & CActorParam_UnkStruct1::FLAG_BIT_1){
            CfSoundMan::func_801BFC38(0, 0x192, 0, 0, lbl_eu_80666DD0);
            CfSoundMan::func_801BFC38(0, 0x191, 0, 0, lbl_eu_80666DD0);
        }else if(param->mFlagsArray[0].flags & 0x2000){
            CfSoundMan::func_801BFC38(0, 0x1C5, 0, 0, lbl_eu_80666DD0);
        }else{
            CActorParam_UnkStruct2* sub = param->unk50;

            if(sub != nullptr){
                if(sub->unk78 & CActorParam_UnkStruct1::FLAG_BIT_24){
                    if((s32)pObjectEne->CActorParam_UnkVirtualFunc19() == 1 ||
                    (s32)pObjectEne->CActorParam_UnkVirtualFunc19() == 2){
                        if(!(sub->unk78 & CActorParam_UnkStruct1::FLAG_BIT_9)){
                            CfSoundMan::func_801BFC38(0, 0x1B4, 0, 0, lbl_eu_80666DD0);
                            return;
                        }
                    }else if(!(sub->unk78 & CActorParam_UnkStruct1::FLAG_BIT_9)){
                        CfSoundMan::func_801BFC38(0, 0x1B5, 0, 0, lbl_eu_80666DD0);
                        return;
                    }
                }else if((sub->unk78 & CActorParam_UnkStruct1::FLAG_BIT_25)
                && !(sub->unk78 & CActorParam_UnkStruct1::FLAG_BIT_9)){
                    CfSoundMan::func_801BFC38(0, 0x1B5, 0, 0, lbl_eu_80666DD0);
                    return;
                }
                
                u16 type = sub->unk40;
                if((s32)type == 1){
                    if(sub->unk78 & CActorParam_UnkStruct1::FLAG_BIT_8){
                        CfSoundMan::func_801BFC38(0, 0x1AF, 0, 0, lbl_eu_80666DD0);
                    }else{
                        //likely an inline
                        if(pObjectEne != nullptr && (ene->field_3f00 & 0x2)){
                            void* arts = func_8009EC9C(ene->field_3f28);
                            func_8009D7E4((u8*)arts + 0x1c, 1);
                        }

                        if((s32)pObjectEne->CActorParam_UnkVirtualFunc19() == 1 ||
                        (s32)pObjectEne->CActorParam_UnkVirtualFunc19() == 2){
                            CfSoundMan::func_801BFC38(0, 0x1AE, 0, 0, lbl_eu_80666DD0);
                        }else{
                            CfSoundMan::func_801BFC38(0, 0x1AD, 0, 0, lbl_eu_80666DD0);
                        }
                    }
                }else if((s32)type == 2){
                    if(sub->unk78 & CActorParam_UnkStruct1::FLAG_BIT_8){
                        CfSoundMan::func_801BFC38(0, 0x1AC, 0, 0, lbl_eu_80666DD0);
                    }else{
                        //likely an inline
                        if(pObjectEne != nullptr && (ene->field_3f00 & 0x2)){
                            void* arts = func_8009EC9C(ene->field_3f28);
                            func_8009D7E4((u8*)arts + 0x1c, 1);
                        }

                        if((s32)pObjectEne->CActorParam_UnkVirtualFunc19() == 1 ||
                        (s32)pObjectEne->CActorParam_UnkVirtualFunc19() == 2){
                            CfSoundMan::func_801BFC38(0, 0x1AB, 0, 0, lbl_eu_80666DD0);
                        }else{
                            CfSoundMan::func_801BFC38(0, 0x1AA, 0, 0, lbl_eu_80666DD0);
                        }
                    }
                }else{
                    u32 r4 = lbl_eu_804FCA08[type];
                    CfSoundMan::func_801BFC38(0, r4, 0, 0, lbl_eu_80666DD0);
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
        UnkClass_805764CC* classPtr = func_800B07E8__Fv();
        func_800B8804__FPvPQ22cf13IFactoryEvent(classPtr, (cf::IFactoryEvent*)this);
        // Retail tail of the ctor: zero the trailing f32 timer slot.
        ((CBattleManagerTail283D4*)this)->field_283D4 = lbl_eu_80666DDC;
    }

    CBattleManager::~CBattleManager(){
        // Manual vptr store under the retail symbol: MWCC's auto store would
        // emit __vt__Q22cf14CBattleManager. Then deregister from the holder
        // (retail calls func_800B07E8__Fv / func_800B88E0 with C linkage).
        *(void**)this = (void*)lbl_eu_8052BCE0;
        func_800B88E0(func_800B07E8__Fv(), this);
    }

// Static member definition (storage for the singleton pointer)
extern "C" CBattleManager* lbl_eu_80663F00 = nullptr;

CBattleManager* CBattleManager::getInstance() {
    return lbl_eu_80663F00;
}

    void CBattleManager::func_800D9190(){
        lbl_eu_80663F00 = new (mtl::MemManager::getHandleMEM1()) CBattleManager();
    }

    void CBattleManager::func_800D91D0(){
        if(lbl_eu_80663F00 != nullptr){
            delete lbl_eu_80663F00;
            lbl_eu_80663F00 = nullptr;
        }
    }

    void CBattleManager::func_800D9218(){
        mActorList1.clear();
        mActorList2.clear();
        mActorList3.clear();
        func_801A380C(&mVision);
        __ct__80192C10(&unk19C);
        func_8027728C(&mChain);
        func_8027D1A4(&unk20C8);
        func_801BA1DC(&mSuddenCommu);
        // Single read of the named 0.0f global (retail does one lfs from
        // lbl_eu_80666DDC, then reuses f0 for all three stfs). Naming the
        // global (instead of writing 0) keeps the sda21 reloc on the retail
        // symbol rather than an MWCC pool constant @N.
        const f32 zero = lbl_eu_80666DDC;
        mSuddenCommu.unk28 = 0;
        mSuddenCommu.unk2C = zero;
        unk88 = zero;
        unk8C = 0;
        unk90 = zero;
        mVision.unk261C4.unk70 = 0;
        unk94.clear();
        func_80085220(2, 0);
    }

    void CBattleManager::FactoryEvent2(){
        // The mBattleEventList reslist's first word carries the dispatch flags.
        u32 listFlags = *(u32*)&mBattleEventList;

        // Bit 0x20 = dynamic_cast / interface dispatch path.
        if(listFlags & 0x20){
            void* castResult = __dynamic_cast(this, 0, &lbl_eu_80661970, &lbl_eu_806618F0, 0);
            void** vtbl = *(void***)this;
            typedef void (*VFunc3C)(void*, void*);
            ((VFunc3C)vtbl[0x3C / 4])(this, castResult);
            return;
        }

        // Bits 0x02 or 0x04 = object removal / cleanup path.
        if(!(listFlags & 0x02) && !(listFlags & 0x04)) return;

        BattleRemoveObjAccessor* target = (BattleRemoveObjAccessor*)func_8016FE34(this);

        // If the target is a member of mActorList1, unregister it everywhere.
        if(mActorList1.find((CfObjectActor*)target) != mActorList1.end()){
            mActorList1.remove((CfObjectActor*)target);
            func_80279694(&mChain, (CfObjectActor*)target);

            if(target->field_3f00 & 0x02){
                mActorList2.remove((CfObjectActor*)target);
            }else{
                mActorList3.remove((CfObjectActor*)target);
            }

            if(((cf::CBattleManager*)lbl_eu_80663F00)->mActorList3.size() != 0) return;
            func_800F41A0(this);
            if(((cf::CBattleManager*)lbl_eu_80663F00)->mActorList1.size() != 0) return;

            for(int i = 0; i < 3; i++){
                void* player = getPlayer__Q22cf13CfGameManagerFi(i);
                void* action = func_8016FE34(player);
                if(action != nullptr){
                    void** vtbl = *(void***)((u8*)action + 0x4);
                    typedef void (*VFunc)(void*, u32);
                    ((VFunc)vtbl[0x20 / 4])((u8*)action + 0x4, 0x40);
                    ((VFunc)vtbl[0x24 / 4])((u8*)action + 0x4, 0x80);
                    ((VFunc)vtbl[0x28 / 4])((u8*)action + 0x4, 0x8000);
                    func_80174B4C(action, 0x800);
                    func_80174B4C(action, 0x1000);
                }
            }

            func_80085220(1, 1);
            // Clear the bit-28 global state flag (rlwinm mask 29..27).
            lbl_eu_80663E28 &= ~0x8;
            func_80277B34(&mChain);

            EnumListHolder holder;
            func_80043D90(&holder);
            func_800F4A98(func_80043F18(&holder), 0x20, 0);

            for(u32 i = 0; i < ((CVisionEnumList*)func_80043F18(&holder))->count; i++){
                void* action = func_8016FE34(func_800F6EAC(func_80043F18(&holder), i));
                void** vtbl = *(void***)action;
                typedef void (*VFunc)(void*);
                ((VFunc)vtbl[0xB0 / 4])(action);
                vtbl = *(void***)action;
                ((VFunc)vtbl[0xB8 / 4])(action);
                vtbl = *(void***)action;
                ((VFunc)vtbl[0x314 / 4])(action);
            }
            __dt__80043E88(&holder, -1);
        }
    }

}
#pragma schedule on

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
    if (lbl_eu_80663F00 != nullptr) {
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

#pragma schedule off
void cf::CBattleManager::func_800EA470() {
    unk88 = lbl_eu_80666DDC;
    func_800EA484(this, lbl_eu_80666DD4, unk8C);
}
#pragma schedule on
extern "C" void func_800EA998(u8* self) { reinterpret_cast<BMIf*>((u8*)self + 0x219c)->vf0024(); }
// func_800EC918 (retail 0x800ED400, 0x6E1C) - main battle-event processor.
// The callers below pass ABI-compatible args; the typed definition follows.
// (unmangled), so they must be declared extern "C" for reloc-name matching.
// func_80148778 / func_80149154 / func_8026178C are already declared in
// object/CAIAction.hpp / CVision.hpp (included via CBattleManager.hpp).
extern "C" int func_80260264(void*, int, void*);
extern "C" bool CChain_chkActorList(void*);  // retail C wrapper symbol
extern "C" void* func_801491F4(void*, u32);
extern "C" bool func_80145DBC(int);
extern "C" int func_80145F78(int);
extern "C" int func_80145BC4(int);
extern "C" int func_80145C00(int);
extern "C" int func_80146148(int);
extern "C" int mtRand__Q22ml4mathFi(int n);
extern "C" f32 lbl_eu_80666DF4;  // 15.0f
extern "C" f32 lbl_eu_80666DD0;  // 0.6f
// 0.01f (existing lbl_80666DD8 without eu_ is a different symbol)
extern "C" f32 lbl_eu_80666DD8;   // 0.01f

// --- func_800D9354 imports (retail-unmangled names) ---
extern "C" void func_801A39D8(cf::CVision* self);
extern "C" void func_8018C610(cf::UnkClass_8018C5FC* self);
extern "C" void func_80192CB0(cf::UnkClass_80192BF4* self);
extern "C" void func_80192C2C(cf::UnkClass_80192BF4* self);
extern "C" void func_8027732C(cf::CChain* self);
extern "C" void func_8027D20C(cf::UnkClass_800D8DBC* self);
extern "C" void func_801BA25C(cf::CSuddenCommu* self);
extern "C" void func_80274B20(void* self);
extern "C" void func_8027EF50();
extern "C" s32 func_80086B18__Q22cf13CfGameManagerFv(void* self);
// --- func_800D9978 imports (retail-unmangled names) ---
extern "C" void func_802A2D84();
extern "C" void func_8027F0A0();
extern "C" void func_802A2210(void* actor);
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
extern "C" f32 lbl_eu_80666E30;  // 0.9f
extern "C" f32 lbl_eu_80666DFC;  // 2.0f
extern "C" f32 lbl_eu_80666E10;  // 0.024543693f (2*pi/256)
extern "C" f32 lbl_eu_80666E24;  // 0.15f
extern "C" f32 lbl_eu_80666E68;  // 3.0f
extern "C" f32 lbl_eu_80666E6C;  // 5.0f
extern "C" f32 lbl_eu_80666E70;  // 0.35f
extern "C" f32 lbl_eu_80666E74;  // 0.01f (squared-distance epsilon)
// (lbl_eu_8066A1F8 comes from CfObjectEne.hpp; an extra extern "C" copy here
// breaks the C++-linkage declaration when the header is included twice.)
extern "C" s16 lbl_eu_804FCA3C[];
// --- shared battle-helper externs used by the func_800E2A9C / func_800E64CC ports ---
extern "C" void func_8018C820(void*, int);                   // party-gauge add
extern "C" u16 func_8016DF2C();                              // current chapter/episode
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
extern "C" void* func_800FE68C(void);
extern "C" void func_800FE96C(void*, u32);
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
extern "C" void func_8010975C(u8);
extern "C" void func_80109770(u8);
extern "C" void func_80109734(void*, u32);
extern "C" float lbl_eu_804FCAD8[3];// {0.0f,0.0f,1.4e-43f} (3rd used as int 0x2)
extern "C" float lbl_eu_804FCAE4[3];// {8.0f,4.0f,2.0f} rate table
// --- forward decls for same-TU battle functions defined later in this file ---
extern "C" void func_800E64CC(cf::CBattleManager*, void*, void*, void*);
extern "C" void func_800E9B54(void*, void*, void*, void*);
extern "C" void func_800E08E8(void*, void*, void*, void*);
extern "C" s32 func_800EAA2C(void*, void*, void*, void*, void*);
extern "C" void func_800D9CA0(cf::CBattleManager*, void*);
extern "C" f32 func_800D7EA0(u8*, void*);
struct BattleEvent;
struct BattleTargetData;
struct EC918_BattleObjAccessor;
extern "C" s32 func_800EC918(
    void* self,
    void* pc,
    EC918_BattleObjAccessor* accessor,
    BattleEvent* event,
    BattleTargetData* target
);

void func_800EC8FC(u32 a, u32 b, void* c, u32 d) {
    func_800EC918(
        (void*)a,
        0,
        (EC918_BattleObjAccessor*)b,
        (BattleEvent*)c,
        (BattleTargetData*)d
    );
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

struct BattleEventWorkspace {
    BattleEvent tailEvent;
    BattleEvent case291Event;
    BattleEvent case290Event;
    BattleEvent case267SecondaryEvent;
    BattleEvent case267PrimaryEvent;
    BattleEvent case251Event;
    BattleEvent case250Event;
    BattleEvent case287Event;
    BattleEvent case261Event;
    BattleEvent case260PlayerEvent;
    BattleEvent case260ActorEvent;
    BattleEvent case249Event;
    BattleEvent case259Event;
    BattleEvent case258Event;
    BattleEvent case257Event;
    BattleEvent case236Event;
    BattleEvent case275Event;
    BattleEvent case274Event;
    BattleEvent case246Event;
    BattleEvent case242Event;
    BattleEvent case241Event;
    BattleEvent case240Event;
    BattleEvent case239Event;
    BattleEvent case244ActorEvent;
    BattleEvent case244PlayerEvent;
    BattleEvent case254Event;
    BattleEvent case243Event;
    BattleEvent case253Event;
    BattleEvent case238ActorEvent;
    BattleEvent case238PlayerEvent;
    BattleEvent case237Event;
    BattleEvent case235Event;
    BattleEvent case215Event;
    BattleEvent case227Event;
    BattleEvent case4Followup;
    BattleEvent case17Followup;
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
    void* vtbl;
};

struct EC918_BattleObjAccessor {
    void* vtbl;
    void* vtbl2;
    SubAccessor subObject;
    u8 _pad_0C[0x1524];
    u32 field_1530;
    u8 _pad_1534[0x15E4 - 0x1534];
    u32 field_15E4;
    u8 _pad_15E8[0x15F0 - 0x15E8];
    u32 field_15F0;
    u8 _pad_15F4[0x3374 - 0x15F4];
    u32 field_3374;
    u8 _pad_3378[0x3E9C - 0x3378];
    u8 field_3E9C;
    u8 _pad_3E9D[0x3ED4 - 0x3E9D];
    void* field_3ED4;
    u8 _pad_3ED8[0x3F00 - 0x3ED8];
    u32 field_3F00;
    u8 _pad_3F04[0x3F10 - 0x3F04];
    u32 field_3F10;
    u8 _pad_3F14[0x3F28 - 0x3F14];
    u16 field_3F28;
    u8 _pad_3F2A[0x456C - 0x3F2A];
    u16 field_456C;
};

// ---- Helper: modulo 100 via mulhw 0x51EC851F pattern ----
static inline s32 mod100(s32 x) {
    return x % 100;
}

// ================================================================
// vcall helpers + constants + externs for func_800EC918 switch cases
// ================================================================
#pragma push
#pragma inline_max_size(10000)
#pragma inline_max_total_size(100000)

static __inline s32 vcall_i(void* o, u32 slot) { void* vt = *(void**)o; return ((s32(*)(void*))(*(void**)((u8*)vt + slot)))(o); }
static __inline void* vcall_p(void* o, u32 slot) { void* vt = *(void**)o; return ((void*(*)(void*))(*(void**)((u8*)vt + slot)))(o); }
static __inline f32 vcall_f(void* o, u32 slot) { void* vt = *(void**)o; return ((f32(*)(void*))(*(void**)((u8*)vt + slot)))(o); }
static __inline void* vcall_p1(void* o, u32 slot, u32 a) { void* vt = *(void**)o; return ((void*(*)(void*, u32))(*(void**)((u8*)vt + slot)))(o, a); }
static __inline void vcall_v1(void* o, u32 slot, u32 a) { void* vt = *(void**)o; return ((void(*)(void*, u32))(*(void**)((u8*)vt + slot)))(o, a); }
static __inline void vcall_f1(void* o, u32 slot, f32 a) { void* vt = *(void**)o; return ((void(*)(void*, f32))(*(void**)((u8*)vt + slot)))(o, a); }
static __inline void vcall_d1(void* o, u32 slot, double d) { void* vt = *(void**)o; return ((void(*)(void*, double))(*(void**)((u8*)vt + slot)))(o, d); }
static __inline void vcall_v3f(void* o, u32 slot, void* a, f32 x, f32 y, f32 z) { void* vt = *(void**)o; return ((void(*)(void*, void*, f32, f32, f32))(*(void**)((u8*)vt + slot)))(o, a, x, y, z); }

static __inline cf::CfObjectMove* getObjectMove(void* actorAccessor) {
    cf::CfObjectMove* move = (cf::CfObjectMove*)actorAccessor;
    if (move != nullptr) {
        move = (cf::CfObjectMove*)((u8*)move + 0x3E9C);
    }
    return move;
}

extern "C" int mtRand__Q22ml4mathFii(int a, int b);
extern "C" void* getFP__FPCc(const char* str);
// func_8003AA34: already declared (void form) by a visible shared header.
extern "C" u8 lbl_eu_80573EEC[];
extern "C" void func_80135380(u8 v);
extern "C" void* func_80149330(void*, u32, u32, u32, u32);
extern "C" s32 func_8015B130(s32 a, u16 b);
extern "C" s32 func_801B1C5C(void);
extern "C" void func_802A30DC(void* pc, s32 v);
extern void func_800BFC68(cf::CfObjectMove* moveObj);

static const f32 k0_0f = 0.0f;     // lbl_eu_80666DDC
static const f32 k0_01f = 0.01f;   // lbl_eu_80666DD8
static const f32 k0_25f = 0.25f;   // lbl_eu_80666E1C
static const f64 k0_5 = 0.5;       // lbl_eu_80666E58
static const f64 kM0_5 = -0.5;     // lbl_eu_80666E60
static const f32 k1_0f = 1.0f;     // lbl_eu_80666DD4
static const f32 k1_25f = 1.25f;   // lbl_eu_80666E40
static const f32 k2_0f = 2.0f;     // lbl_eu_80666DFC
static const f32 k2_5f = 2.5f;     // lbl_eu_80666E80
static const f32 k5_0f = 5.0f;     // lbl_eu_80666E6C
static const f32 k10_0f = 10.0f;   // lbl_eu_80666E34
static const f32 k15_0f = 15.0f;   // lbl_eu_80666DF4
static const f32 k25_0f = 25.0f;   // lbl_eu_80666DF8
static const f32 k50_0f = 50.0f;   // lbl_eu_80666E18
static const f32 k75_0f = 75.0f;   // lbl_eu_80666E7C
static const f32 k99_0f = 99.0f;   // lbl_eu_80666E88
static const f32 k100_0f = 100.0f; // lbl_eu_80666E00
extern "C" void* __ct__800FB044(void* list, f32 f, void* obj, int kind);
extern "C" float func_800D81A8(void* obj, void* target, void* source);



static __inline s32 vf0C(void* o) { void* vt = *(void**)o; return ((s32(*)(void*))(*(void**)((u8*)vt + 0x0C)))(o); }
static __inline void vf20(void* o, u32 a) { void* vt = *(void**)o; return ((void(*)(void*, u32))(*(void**)((u8*)vt + 0x20)))(o, a); }
static __inline void vf24(void* o, void* a) { void* vt = *(void**)o; return ((void(*)(void*, void*))(*(void**)((u8*)vt + 0x24)))(o, a); }
static __inline void* vf30(void* o) { void* vt = *(void**)o; return ((void*(*)(void*))(*(void**)((u8*)vt + 0x30)))(o); }
static __inline u32 vf4C(void* o) { void* vt = *(void**)o; return ((u32(*)(void*))(*(void**)((u8*)vt + 0x4C)))(o); }
static __inline void* vf5C(void* o, u32 a) { void* vt = *(void**)o; return ((void*(*)(void*, u32))(*(void**)((u8*)vt + 0x5C)))(o, a); }
static __inline void vf70(void* o, u32 a) { void* vt = *(void**)o; return ((void(*)(void*, u32))(*(void**)((u8*)vt + 0x70)))(o, a); }
static __inline void* vfAC(void* o) { void* vt = *(void**)o; return ((void*(*)(void*))(*(void**)((u8*)vt + 0xAC)))(o); }
static __inline s32 vfE0(void* o) { void* vt = *(void**)o; return ((s32(*)(void*))(*(void**)((u8*)vt + 0xE0)))(o); }
static __inline s32 vf108(void* o) { void* vt = *(void**)o; return ((s32(*)(void*))(*(void**)((u8*)vt + 0x108)))(o); }
static __inline void vf150(void* o, f32 a) { void* vt = *(void**)o; return ((void(*)(void*, f32))(*(void**)((u8*)vt + 0x150)))(o, a); }
static __inline void vf154(void* o, f32 a) { void* vt = *(void**)o; return ((void(*)(void*, f32))(*(void**)((u8*)vt + 0x154)))(o, a); }
static __inline f32 vf158(void* o) { void* vt = *(void**)o; return ((f32(*)(void*))(*(void**)((u8*)vt + 0x158)))(o); }
static __inline void* vf224(void* o) { void* vt = *(void**)o; return ((void*(*)(void*))(*(void**)((u8*)vt + 0x224)))(o); }
static __inline void* vf290(void* o) { void* vt = *(void**)o; return ((void*(*)(void*))(*(void**)((u8*)vt + 0x290)))(o); }
static __inline s32 vf2BC(void* o) { void* vt = *(void**)o; return ((s32(*)(void*))(*(void**)((u8*)vt + 0x2BC)))(o); }
static __inline void vf2C4(void* o, void* a, f32 x, f32 y, f32 z) { void* vt = *(void**)o; return ((void(*)(void*, void*, f32, f32, f32))(*(void**)((u8*)vt + 0x2C4)))(o, a, x, y, z); }
static __inline void* vf2D4(void* o, u32 a) { void* vt = *(void**)o; return ((void*(*)(void*, u32))(*(void**)((u8*)vt + 0x2D4)))(o, a); }
static __inline void vf5B0(void* o, f32 a, u32 b) { void* vt = *(void**)o; return ((void(*)(void*, f32, u32))(*(void**)((u8*)vt + 0x5B0)))(o, a, b); }
static __inline void* vf5C0(void* o, void* a) { void* vt = *(void**)o; return ((void*(*)(void*, void*))(*(void**)((u8*)vt + 0x5C0)))(o, a); }
static __inline f32 vf130(void* o) { void* vt = *(void**)o; return ((f32(*)(void*))(*(void**)((u8*)vt + 0x130)))(o); }
// battle-status list base at actor +0x08
static __inline void* D81A8_list(void* t) { return &((D81A8_StatusListView*)t)->statusList; }

// VF* vtable slot constants used by the EC918 switch cases
#define SLOT_VF0C 0x0C
#define SLOT_VF108 0x108
#define SLOT_VF118 0x118
#define SLOT_VF11C 0x11C
#define SLOT_VF128 0x128
#define SLOT_VF12C 0x12C
#define SLOT_VF150 0x150
#define SLOT_VF154 0x154
#define SLOT_VF15C 0x15C
#define SLOT_VF1E8 0x1E8
#define SLOT_VF20 0x20
#define SLOT_VF200 0x200
#define SLOT_VF20C 0x20C
#define SLOT_VF24 0x24
#define SLOT_VF27C 0x27C
#define SLOT_VF290 0x290
#define SLOT_VF298 0x298
#define SLOT_VF2BC 0x2BC
#define SLOT_VF2C4 0x2C4
#define SLOT_VF2D4 0x2D4
#define SLOT_VF2F8 0x2F8
#define SLOT_VF308 0x308
#define SLOT_VF320 0x320
#define SLOT_VF324 0x324
#define SLOT_VF4C 0x4C
#define SLOT_VF58 0x58
#define SLOT_VF5B0 0x5B0
#define SLOT_VF5C 0x5C
#define SLOT_VFAC 0xAC

// ---- The function ----

// MWCC declines to inline the small dispatch helpers above into this unusually
// large routine.  The original routine performs these ordinary virtual calls
// directly, so keep the case bodies readable while spelling out the dispatch
// at each call site for this function only.
#define vcall_i(object, slot)                                                  \
    ((s32 (*)(void*))(*(void**)((u8*)*(void**)(object) + (slot))))(object)
#define vcall_p(object, slot)                                                  \
    ((void* (*)(void*))(*(void**)((u8*)*(void**)(object) + (slot))))(object)
#define vcall_f(object, slot)                                                  \
    ((f32 (*)(void*))(*(void**)((u8*)*(void**)(object) + (slot))))(object)
#define vcall_p1(object, slot, argument)                                       \
    ((void* (*)(void*, u32))(*(void**)((u8*)*(void**)(object) + (slot))))(     \
        object, argument)
#define vcall_v1(object, slot, argument)                                       \
    ((void (*)(void*, u32))(*(void**)((u8*)*(void**)(object) + (slot))))(      \
        object, argument)
#define vcall_f1(object, slot, argument)                                       \
    ((void (*)(void*, f32))(*(void**)((u8*)*(void**)(object) + (slot))))(      \
        object, argument)
#define vcall_d1(object, slot, argument)                                       \
    ((void (*)(void*, double))(*(void**)((u8*)*(void**)(object) + (slot))))(   \
        object, argument)
#define vcall_v3f(object, slot, argument, x, y, z)                             \
    ((void (*)(void*, void*, f32, f32, f32))                                  \
        (*(void**)((u8*)*(void**)(object) + (slot))))(object, argument, x, y, z)

#define ecVf0C(object) vcall_i(object, 0x0C)
#define ecVf20(object, argument) vcall_v1(object, 0x20, argument)
#define ecVf24(object, argument)                                                 \
    ((void (*)(void*, void*))(*(void**)((u8*)*(void**)(object) + 0x24)))(      \
        object, argument)
#define ecVf30(object) vcall_p(object, 0x30)
#define ecVf4C(object) ((u32)vcall_i(object, 0x4C))
#define ecVf5C(object, argument) vcall_p1(object, 0x5C, argument)
#define ecVf54(object, argument) vcall_p1(object, 0x54, argument)
#define ecVf58(object, argument) vcall_p1(object, 0x58, argument)
#define ecVf60(object, argument) vcall_p1(object, 0x60, argument)
#define ecVf70(object, argument) vcall_v1(object, 0x70, argument)
#define ecVfAC(object) vcall_p(object, 0xAC)
#define ecVfE0(object) vcall_i(object, 0xE0)
#define ecVf108(object) vcall_i(object, 0x108)
#define ecVf150(object, argument) vcall_f1(object, 0x150, argument)
#define ecVf154(object, argument) vcall_f1(object, 0x154, argument)
#define ecVf158(object) vcall_f(object, 0x158)
#define ecVf224(object) vcall_p(object, 0x224)
#define ecVf290(object) vcall_p(object, 0x290)
#define ecVf2BC(object) vcall_i(object, 0x2BC)
#define ecVf2C4(object, argument, x, y, z)                                       \
    vcall_v3f(object, 0x2C4, argument, x, y, z)
#define ecVf2D4(object, argument) vcall_p1(object, 0x2D4, argument)
#define ecVf5B0(object, argument, flags)                                         \
    ((void (*)(void*, f32, u32))                                              \
        (*(void**)((u8*)*(void**)(object) + 0x5B0)))(object, argument, flags)
#define ecVfStatus(object, slot, eventType)                                    \
    ((s32 (*)(void*, u16))(*(void**)((u8*)*(void**)(object) + (slot))))(       \
        object, eventType)
#define ecVfEvent(object, event)                                               \
    ((void (*)(void*, BattleEvent*))                                          \
        (*(void**)((u8*)*(void**)(object) + 0x18)))(object, event)

// BattleEvent is copied as thirteen words in the original routine. MWCC
// otherwise outlines its generated assignment operator because this caller is
// so large. Copying word pairs keeps the generated code in the same pipelined
// shape while leaving the case bodies self-explanatory.
static __inline u32 copyBattleEvent(
    BattleEvent& destination,
    const BattleEvent& source
) {
    u32* destinationWords = (u32*)&destination;
    const u32* sourceWords = (const u32*)&source;

    for (s32 i = 0; i < 12; i += 2) {
        u32 first = sourceWords[i];
        u32 second = sourceWords[i + 1];
        destinationWords[i + 1] = second;
        destinationWords[i] = first;
    }

    u32 last = sourceWords[12];
    destinationWords[12] = last;
    return last;
}

static __inline void copyBattleEventPair(
    BattleEvent& first,
    BattleEvent& second,
    BattleEvent& source,
    u32 secondFlags
) {
    u32* words = (u32*)&source;
    u32 word00 = words[0];
    u32 word04 = words[1];
    u32 word08 = words[2];
    u32 word0C = words[3];
    u32 word10 = words[4];
    u32 word14 = words[5];
    u32 word18 = words[6];
    u32 word1C = words[7];
    u32 word20 = words[8];
    u32 word24 = words[9];
    u32 word28 = words[10];
    u32 word2C = words[11];
    u32 word30 = words[12];

    u32* firstWords = (u32*)&first;
    firstWords[0] = word00; firstWords[1] = word04; firstWords[2] = word08;
    firstWords[3] = word0C; firstWords[4] = word10; firstWords[5] = word14;
    firstWords[6] = word18; firstWords[7] = word1C; firstWords[8] = word20;
    firstWords[9] = word24; firstWords[10] = word28; firstWords[11] = word2C;
    firstWords[12] = word30;

    u32 secondWord30 = word30 | secondFlags;
    if (secondFlags != 0) {
        words[12] = secondWord30;
    }

    u32* secondWords = (u32*)&second;
    secondWords[0] = word00; secondWords[1] = word04; secondWords[2] = word08;
    secondWords[3] = word0C; secondWords[4] = word10; secondWords[5] = word14;
    secondWords[6] = word18; secondWords[7] = word1C; secondWords[8] = word20;
    secondWords[9] = word24; secondWords[10] = word28; secondWords[11] = word2C;
    secondWords[12] = secondWord30;
}

static __inline u32 battleCategoryFlag70(void* categoryData, u32 category) {
    switch (category) {
    case 1:
    case 2:
    case 3: return (*(u8*)((u8*)categoryData + 0x70) >> 7) & 1;
    case 4: return (*(u8*)((u8*)categoryData + 0x70)) & 1;
    case 5: return (*(u8*)((u8*)categoryData + 0x70) >> 1) & 1;
    case 6: return (*(u8*)((u8*)categoryData + 0x70) >> 2) & 1;
    case 7: return (*(u8*)((u8*)categoryData + 0x70) >> 3) & 1;
    case 8: return (*(u8*)((u8*)categoryData + 0x70) >> 4) & 1;
    case 9: return (*(u8*)((u8*)categoryData + 0x70) >> 5) & 1;
    default:
    case 0: return 0;
    }
}

static __inline u32 battleCategoryFlag71(void* categoryData, u32 category) {
    switch (category) {
    case 1:
    case 2:
    case 3: return (*(u8*)((u8*)categoryData + 0x71) >> 7) & 1;
    case 4: return (*(u8*)((u8*)categoryData + 0x71)) & 1;
    case 5: return (*(u8*)((u8*)categoryData + 0x71) >> 1) & 1;
    case 6: return (*(u8*)((u8*)categoryData + 0x71) >> 2) & 1;
    case 7: return (*(u8*)((u8*)categoryData + 0x71) >> 3) & 1;
    case 8: return (*(u8*)((u8*)categoryData + 0x71) >> 4) & 1;
    case 9: return (*(u8*)((u8*)categoryData + 0x71) >> 5) & 1;
    default:
    case 0: return 0;
    }
}

static __inline s32 battleCategoryStat(void* categoryData, u32 category) {
    switch (category) {
    case 4: return *(s16*)((u8*)categoryData + 0x64);
    case 5: return *(s16*)((u8*)categoryData + 0x66);
    case 6: return *(s16*)((u8*)categoryData + 0x68);
    case 7: return *(s16*)((u8*)categoryData + 0x6A);
    case 8: return *(s16*)((u8*)categoryData + 0x6C);
    case 9: return *(s16*)((u8*)categoryData + 0x6E);
    default: return 0;
    }
}

extern "C" s32 func_800EC918(
    void* self,
    void* pc,
    EC918_BattleObjAccessor* acc,
    BattleEvent* evt,
    BattleTargetData* tgt
) {
    f32 f28 = 0.0f;
    void* artsData = nullptr;
    BattleEventWorkspace eventWorkspace;

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
        if (func_8006EF04__Fi(0x10000000)) {
            return 0;
        }
    }

    // ---- Guard 2: actor vfunc 0x2BC ----
    {
        void* vtbl = *(void**)acc;
        typedef s32 (*VF2BC)(EC918_BattleObjAccessor*);
        VF2BC vf2BC = (VF2BC)(*(void**)((u8*)vtbl + 0x2BC));
        if (ecVf2BC(acc)) {
            return 0;
        }
    }

    // ---- Guard 3: actor state disallows battle-event processing ----
    if (acc->field_3374 & 0x08000000) {
        return 0;
    }

    // ---- Guard 4: Confirmation gate (flag 0x04000000) ----
    {
        extern void* getInstance__Q22cf13CfGameManagerFv();
        getInstance__Q22cf13CfGameManagerFv();
        if (func_8006EF04__Fi(0x04000000)) {
            extern u32 lbl_eu_8052B784[];
            extern u32 lbl_eu_8052B790[];
            bool isConfirmationHandler = false;

            {
                u32 firstHandler[3];
                firstHandler[0] = lbl_eu_8052B784[0];
                firstHandler[1] = lbl_eu_8052B784[1];
                firstHandler[2] = lbl_eu_8052B784[2];
                if (__ptmf_cmpr((u8*)self + 0x28354, firstHandler)) {
                    u32 confirmationHandler[3];
                    confirmationHandler[0] = lbl_eu_8052B790[0];
                    confirmationHandler[1] = lbl_eu_8052B790[1];
                    confirmationHandler[2] = lbl_eu_8052B790[2];
                    if (__ptmf_cmpr((u8*)self + 0x28354, confirmationHandler)) {
                        isConfirmationHandler = true;
                    }
                }
            }

            if (!isConfirmationHandler) {
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
                ecVf20(&acc->subObject, *(u32*)((u8*)acc + 0x1530));
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
                s32 level = ecVf108(pc);
                s32 product = (s32)artsId * (level + 14);
                vtbl = *(void**)acc;
                typedef void (*VF2C4)(EC918_BattleObjAccessor*, void*, f32, f32, f32);
                VF2C4 vf2C4 = (VF2C4)(*(void**)((u8*)vtbl + 0x2C4));
                f32 fprod = (f32)(s32)product;
                ecVf2C4(acc, pc, lbl_eu_80666DDC, f28 * fprod, lbl_eu_80666DDC);

            } else if (func_80145C00(evt->eventType)) {
                void* vtbl = *(void**)pc;
                u16 artsId = *(u16*)((u8*)artsData + 0x46);
                typedef s32 (*VF108)(void*);
                VF108 vf108 = (VF108)(*(void**)((u8*)vtbl + 0x108));
                s32 level = ecVf108(pc);
                s32 product = (s32)artsId * (level + 14);
                s32 damage = (s32)(f28 * (f32)(s32)product);
                func_800E9FE4(
                    self,
                    pc,
                    0,
                    damage,
                    0,
                    1,
                    (void*)(uintptr_t)acc->field_3F10
                );

            } else if (func_80146148(evt->eventType)) {
                if (evt->prevEventType == 0) {
                    void* vtbl = *(void**)pc;
                    u16 artsId = *(u16*)((u8*)artsData + 0x46);
                    typedef s32 (*VF108)(void*);
                    VF108 vf108 = (VF108)(*(void**)((u8*)vtbl + 0x108));
                    s32 level = ecVf108(pc);
                    s32 product = (s32)artsId * (level + 14);
                    s32 damage = (s32)(f28 * (f32)(s32)product);
                    func_800E9FE4(
                        self,
                        pc,
                        0,
                        damage,
                        0,
                        1,
                        (void*)(uintptr_t)acc->field_3F10
                    );
                }
            }
        }
    }

    // ---- subAccessor vfunc 0x80 ----
    {
        if (ecVfStatus(&acc->subObject, 0x80, evt->eventType)) {
            return func_800F3734(
                self, (BattleObjAccessor*)acc, evt, tgt);
        }
    }

    // ---- subAccessor vfunc 0x88 ----
    {
        if (ecVfStatus(&acc->subObject, 0x88, evt->eventType)) {
            bool resisted = false;
            s32 resistanceChance = 0x32;

            if (evt->eventType == 0x12) {
                s32 f15E4 = *(s32*)((u8*)acc + 0x15E4);
                if (f15E4 == 1) resistanceChance = 0x14;
                if (f15E4 == 2) resistanceChance -= 0x14;
                if (f15E4 == 3) resistanceChance += 0x14;
                if (f15E4 == 4) resistanceChance += 0x32;
                if (f15E4 == 5) resistanceChance += 0x32;

                if (*(u32*)((u8*)acc + 0x3374) & 0x100) {
                    resistanceChance += 0x1E;
                }

                u8 byte1AA = *(u8*)((u8*)self + 0x1AA);
                if (byte1AA >= 1) {
                    if (byte1AA <= 24) {
                        resistanceChance = 0;
                    }
                }
            }

            s32 rollVal;
            if (tgt != nullptr) {
                rollVal = mod100((s32)tgt->field_B0);
            } else {
                rollVal = mtRand__Q22ml4mathFi(100);
            }

            if (rollVal < resistanceChance) {
                resisted = true;
                if (tgt != nullptr) {
                    tgt->field_74 |= 0x80000010;
                }
            }

            if (resisted) {
                return func_800F37F8(
                    self, (BattleObjAccessor*)acc, evt, tgt);
            }
        }
    }

    // ---- Status check: 0x27 ----
    if (func_80148778((&acc->subObject), 0x27)) {
        if (!(acc == (EC918_BattleObjAccessor*)pc && evt->eventType == 0x11)) {
            if (func_80145F78(evt->eventType)) {
                void* entry27 = func_80149154((&acc->subObject), 0x27);
                if (entry27 != nullptr) {
                    s32 rollVal2;
                    if (tgt != nullptr) {
                        rollVal2 = (s32)tgt->field_B4;
                    } else {
                        rollVal2 = mtRand__Q22ml4mathFi(100);
                    }
                    s32 rate = *(s32*)((u8*)entry27 + 0x10);
                    if (rollVal2 < rate) {
                        return func_800F37F8(
                            self, (BattleObjAccessor*)acc, evt, tgt);
                    }
                }
            }
        }
    }

    // ---- Status check: 0xCE ----
    if (func_80148778((&acc->subObject), 0xCE)) {
        if (tgt != nullptr && (tgt->field_78 & 0x1000)) {
            if (func_80145DBC(evt->eventType)) {
                void* entryCE = func_801491F4((&acc->subObject), 0xCE);
                if (entryCE != nullptr && artsData != nullptr) {
                    void* artsVTbl = *(void**)((u8*)artsData + 0x84);
                    typedef s32 (*VF0C)(void*);
                    VF0C vf0C = (VF0C)(*(void**)((u8*)artsVTbl + 0x0C));
                    s32 artsVal = ecVf0C(artsData);
                    s32 entryVal = *(s32*)((u8*)entryCE + 0x10);
                    if (entryVal >= artsVal) {
                        tgt->field_74 |= 0x80002000;
                        return func_800F37F8(
                            self, (BattleObjAccessor*)acc, evt, tgt);
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
        vf224Result = (void*)ecVf224(acc);
    }

    // Categories 1-3 share the high bit. Categories 4-9 use their own
    // stat/flag slots; a sufficiently high stat forces the gate on.
    {
        u32 blocked = 0;
        if ((u32)(resultGate - 1) <= 2) {
            blocked = battleCategoryFlag71(vf224Result, resultGate);
        } else if (resultGate != 0) {
            if (battleCategoryStat(vf224Result, resultGate) >= 0x96) {
                blocked = 1;
            } else {
                blocked = battleCategoryFlag71(vf224Result, resultGate);
            }
        }

        if (blocked) return 0;
    }

    {
        u32 triggersResistance = 0;
        if ((u32)(resultGate - 1) <= 2) {
            triggersResistance = battleCategoryFlag70(vf224Result, resultGate);
        } else if (resultGate != 0) {
            if (battleCategoryStat(vf224Result, resultGate) >= 0x64) {
                triggersResistance = 1;
            } else {
                triggersResistance = battleCategoryFlag70(vf224Result, resultGate);
            }
        }

        if (triggersResistance) {
            s32 roll = tgt != nullptr
                ? mod100((s32)tgt->field_AC)
                : mtRand__Q22ml4mathFi(100);
            if (roll < 0x32) {
                return func_800F37F8(
                    self, (BattleObjAccessor*)acc, evt, tgt);
            }
        }
    }

    // ---- Stat multiplier: 0x75 ----
    if (pc != nullptr) {
        void* pcVTbl = *(void**)pc;
        typedef void* (*VF290)(void*);
        VF290 vf290 = (VF290)(*(void**)((u8*)pcVTbl + 0x290));
        void* statObj = ecVf290(pc);
        if (statObj != nullptr) {
            if (func_80145C00(evt->eventType)) {
                u32 stackVal;
                if (func_80260264(ecVf290(pc), 0x75, (s32*)&stackVal)) {
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
                if (func_80260264(ecVf290(acc), 0x7C, (s32*)&stackVal2)) {
                    if (tgt != nullptr) {
                        if ((s32)tgt->field_B4 < (s32)stackVal2) {
                            return func_800F37F8(
                                self, (BattleObjAccessor*)acc, evt, tgt);
                        }
                    } else {
                        if (mtRand__Q22ml4mathFi(100) < (s32)stackVal2) {
                            return func_800F37F8(
                                self, (BattleObjAccessor*)acc, evt, tgt);
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

    case 17: {
        // Skill roll: status 0x22 (resist/evade gate)
        if (func_80148778((&acc->subObject), 0x22)) {
            if (acc != (EC918_BattleObjAccessor*)pc) {  // cmplw r24,r23; beq
                void* entry22 = func_80149154((&acc->subObject), 0x22);
                s32 rollVal;
                if (tgt != nullptr) rollVal = (s32)tgt->field_B4;
                else rollVal = mtRand__Q22ml4mathFi(100);
                s32 rate = *(s32*)((u8*)entry22 + 0x10);
                if (rollVal < rate) {
                    return func_800F37F8(self, (BattleObjAccessor*)acc, evt, tgt);
                }
            }
        }

        // Clear status 0x11 through the sub-acc
        if (func_80148778((&acc->subObject), 0x11)) {
            ecVf20(&acc->subObject, 0x11);   // lwz r12,8(r24); lwz r12,0x20(r12); bctrl
        }

        s32 initialValue = evt->field_10;
        if (initialValue != 0) break;         // bne .L_800F4000
        if (evt->field_14 != 0) break;       // bne .L_800F4000

        // Preserve the original for the follow-up, then process the primary
        // event in place.
        copyBattleEvent(eventWorkspace.case17Followup, *evt);
        evt->field_10 = *(s32*)((u8*)pc + 0x3F10);

        if (tgt != nullptr && (tgt->field_78 & 1)) {  // clrlwi. 0,0,31
            evt->field_14 = 1;
        }

        s32 subResult = func_800EC918(self, pc, acc, evt, tgt);
        if (!subResult) {                       // beq 0x800EE0B4
            return func_800F37F8(self, (BattleObjAccessor*)acc, evt, tgt);
        }

        // acc->3ED4 ecVf70(pc->3F10)
        void* obj3ED4 = acc->field_3ED4;
        void* v3ED4 = *(void**)obj3ED4;
        typedef void (*VF70)(void*, u32);
        VF70 vf70 = (VF70)(*(void**)((u8*)v3ED4 + 0x70));
        ecVf70(obj3ED4, *(u32*)((u8*)pc + 0x3F10));

        // If the acc's sub-identifier object == player 0 -> HUD refresh
        cf::CfObjectMove* subIdent = getObjectMove(acc);
        if (subIdent == getPlayer__Q22cf13CfGameManagerFi(0)) {
            void* hud = func_800FE68C();
            func_800FE96C(hud, *(u32*)((u8*)pc + 0x3F10));
        }

        // Second half: only when the tgt lacks bit0 of field_78
        if (tgt == nullptr || !(tgt->field_78 & 1)) {
            eventWorkspace.case17Followup.eventType = 0x11;
            eventWorkspace.case17Followup.field_30 |= 0x800;
            eventWorkspace.case17Followup.field_10 = *(u32*)((u8*)acc + 0x3F10);

            func_800EC918(
                self,
                pc,
                (EC918_BattleObjAccessor*)pc,
                &eventWorkspace.case17Followup,
                tgt
            );

            // pc->3ED4 ecVf70(acc->3F10)
            void* pc3ED4 = *(void**)((u8*)pc + 0x3ED4);
            void* vPC3ED4 = *(void**)pc3ED4;
            typedef void (*VF70)(void*, u32);
            VF70 vf70 = (VF70)(*(void**)((u8*)vPC3ED4 + 0x70));
            ecVf70(pc3ED4, *(u32*)((u8*)acc + 0x3F10));

            cf::CfObjectMove* pcSubIdent = getObjectMove(pc);
            if (pcSubIdent == getPlayer__Q22cf13CfGameManagerFi(0)) {
                void* hud = func_800FE68C();
                func_800FE96C(hud, *(u32*)((u8*)acc + 0x3F10));
            }
        }
        return 1;                               // li r3,1; b .L_800F41D4
    }

    case 4: {
        if (func_80148778((&acc->subObject), 0x15)) {
            void* entry15 = func_80149154((&acc->subObject), 0x15);
            s32 rollVal;
            if (tgt != nullptr) rollVal = (s32)tgt->field_B4;
            else rollVal = mtRand__Q22ml4mathFi(100);
            s32 rate = *(s32*)((u8*)entry15 + 0x10);
            if (rollVal < rate) {
                return func_800F37F8(self, (BattleObjAccessor*)acc, evt, tgt);
            }
        }
        // .L_800EE200
        evt->prevEventType = evt->eventType;      // lhz r3,0xc(r25); sth r3,0x2e(r25)
        evt->field_30 = copyBattleEvent(eventWorkspace.case4Followup, *evt) | 2;
        eventWorkspace.case4Followup.eventType = 5;
        func_800EC918(self, pc, acc, &eventWorkspace.case4Followup, tgt);
        break;                                  // b .L_800F4000
    }

    case 9: {
        if (func_80148778((&acc->subObject), 0x1A)) {
            void* entry1A = func_80149154((&acc->subObject), 0x1A);
            s32 rollVal;
            if (tgt != nullptr) rollVal = (s32)tgt->field_B4;
            else rollVal = mtRand__Q22ml4mathFi(100);
            if (rollVal < *(s32*)((u8*)entry1A + 0x10)) {
                return func_800F37F8(self, (BattleObjAccessor*)acc, evt, tgt);
            }
        }
        break;                                  // b .L_800F4000
    }

    case 6: {
        if (func_80148778((&acc->subObject), 0x17)) {
            void* entry17 = func_80149154((&acc->subObject), 0x17);
            s32 rollVal;
            if (tgt != nullptr) rollVal = (s32)tgt->field_B4;
            else rollVal = mtRand__Q22ml4mathFi(100);
            if (rollVal < *(s32*)((u8*)entry17 + 0x10)) {
                return func_800F37F8(self, (BattleObjAccessor*)acc, evt, tgt);
            }
        }
        break;                                  // b .L_800F4000
    }

    case 7: {
        if (func_80148778((&acc->subObject), 0x18)) {
            void* entry18 = func_80149154((&acc->subObject), 0x18);
            s32 rollVal;
            if (tgt != nullptr) rollVal = (s32)tgt->field_B4;
            else rollVal = mtRand__Q22ml4mathFi(100);
            if (rollVal < *(s32*)((u8*)entry18 + 0x10)) {
                return func_800F37F8(self, (BattleObjAccessor*)acc, evt, tgt);
            }
        }
        break;                                  // b .L_800F4000
    }

    case 19: {
        if (func_80148778((&acc->subObject), 0x24)) {
            void* entry24 = func_80149154((&acc->subObject), 0x24);
            s32 rollVal;
            if (tgt != nullptr) rollVal = (s32)tgt->field_B4;
            else rollVal = mtRand__Q22ml4mathFi(100);
            if (rollVal < *(s32*)((u8*)entry24 + 0x10)) {
                return func_800F37F8(self, (BattleObjAccessor*)acc, evt, tgt);
            }
        }
        break;                                  // b .L_800F4000
    }

    case 11: {
        if (func_80148778((&acc->subObject), 0x1C)) {
            void* entry1C = func_80149154((&acc->subObject), 0x1C);
            s32 rollVal;
            if (tgt != nullptr) rollVal = (s32)tgt->field_B4;
            else rollVal = mtRand__Q22ml4mathFi(100);
            if (rollVal < *(s32*)((u8*)entry1C + 0x10)) {
                return func_800F37F8(self, (BattleObjAccessor*)acc, evt, tgt);
            }
        }
        break;                                  // b .L_800F4000
    }

    case 12: {
        if (func_80148778((&acc->subObject), 0x1D)) {
            void* entry1D = func_80149154((&acc->subObject), 0x1D);
            s32 rollVal;
            if (tgt != nullptr) rollVal = (s32)tgt->field_B4;
            else rollVal = mtRand__Q22ml4mathFi(100);
            if (rollVal < *(s32*)((u8*)entry1D + 0x10)) {
                return func_800F37F8(self, (BattleObjAccessor*)acc, evt, tgt);
            }
        }
        break;                                  // b .L_800F4000
    }

    case 147: {
        if (evt->field_20 == k0_0f) {            // fcmpu cr0, f1(0.0), f0; bne tail
            evt->field_20 = k5_0f;               // lfs f0, 80666E6C; stfs 0x20(r25)
        }
        break;                                  // b .L_800F4000
    }

    case 280: {
        u32 fmt = (*(u16*)((u8*)acc + 0x456C)) >> 4;   // lhz + srawi r3,r3,4
        u16 artsId = (u16)evt->field_10;                     // clrlwi r4, r0, 16
        func_80195BD4(fmt, artsId);
        break;                                  // b .L_800F4000
    }

    case 299: {
        getInstance__Q22cf13CfGameManagerFv();  // bl (result unused in this case)
        if (!func_8006EF04__Fi(0x04000000)) {   // lis r3, 0x400  (0x04000000)
            func_8016DF4C(evt->field_10);        // lwz r3,0x10(r25); bl func_8016DF4C
        }
        break;                                  // b .L_800F4000
    }

    case 227: {
        if (pc == nullptr) break;               // cmpwi r23,0; beq .L_800F4000

        // r15 = pc->vf298() (stat object); r16 = flag
        void* pcStat = vcall_p(pc, SLOT_VF298);      // 0x800EE538
        s32 flag = 0;                           // li r16, 0

        // acc->vf324()[0] == 7 (u16)
        if (*(u16*)vcall_p(acc, SLOT_VF324) == 7) flag = 1;  // lhz + cmplwi 7

        // acc->vf108() - pc->vf108() > 10
        if ((vcall_i(acc, SLOT_VF108) - vcall_i(pc, SLOT_VF108)) > 10) flag = 1;  // subf + cmpwi 0xa

        if (!flag) {
            // Gate: obj4 = acc->+0x4; val = obj4->vf30()[0]
            int val;
            {
                void* obj4 = *(void**)((u8*)acc + 4);
                void* v4 = *(void**)obj4;
                typedef void* (*VF30)(void*);
                VF30 vf30 = (VF30)(*(void**)((u8*)v4 + 0x30));
                val = *(u32*)ecVf30(obj4);
            }
            if (func_80174C98((void*)acc, &val, 0x802) == 0 &&   // bl func_80174C98; cmpwi; bne skip
                !func_80148778((&acc->subObject), 0x32)) {      // has 0x32 -> skip gate
                // f28 base by acc->field_15E4 (0 -> 0.0, 6+ -> 0.0)
                f32 f28 = k0_0f;
                s32 state = acc->field_15E4;
                if (state == 1) {
                    f28 = k75_0f;
                } else if (state == 2) {
                    f28 = k50_0f;
                } else if (state == 3) {
                    f28 = k25_0f;
                } else if (state == 4) {
                    f28 = k15_0f;
                } else if (state == 5) {
                    f28 = k10_0f;
                }
                // pcStat->+0x50 artsData: f28 += 2.5 * (f32)artsLevel
                void* pcArts = *(void**)((u8*)pcStat + 0x50);   // lwz r3, 0x50(r15)
                if (pcArts != nullptr) {
                    void* artsVTbl = *(void**)((u8*)pcArts + 0x84);
                    typedef s32 (*VF0C)(void*);
                    VF0C vf0C = (VF0C)(*(void**)((u8*)artsVTbl + 0x0C));
                    f28 += k2_5f * (f32)ecVf0C(pcArts);   // fmadds f28, f0(2.5), f1, f28
                }
                s32 rollVal;
                if (tgt != nullptr) rollVal = (s32)tgt->field_B4;
                else rollVal = mtRand__Q22ml4mathFi(100);
                if ((s32)f28 <= rollVal) flag = 1;      // fctiwz; cmpw; bgt skip; li r16,1
            } else {
                flag = 1;                               // .L_800EE6CC
            }
        }

        copyBattleEvent(eventWorkspace.case227Event, *evt);
        eventWorkspace.case227Event.eventType = 0x10;
        eventWorkspace.case227Event.field_20 = (f32)(s32)evt->field_10;
        eventWorkspace.case227Event.field_10 = 3;

        if (flag) {                             // cmpwi r16,0; beq .L_800EE798
            eventWorkspace.case227Event.eventType = 6;
            if (func_800EC918(self, pc, acc, &eventWorkspace.case227Event, tgt)) {
                break;
            }
            return 0;
        }

        // .L_800EE798 -- flag == 0 path
        eventWorkspace.case227Event.field_30 |= 0x2000;
        if (func_800EC918(self, pc, acc, &eventWorkspace.case227Event, tgt)) {
            break;
        }
        eventWorkspace.case227Event.eventType = 6;
        if (func_800EC918(self, pc, acc, &eventWorkspace.case227Event, tgt)) {
            break;
        }
        return 0;                               // li r3,0; b .L_800F41D4
    }

    case 16: {
        // obj4 = acc->+0x4; val = obj4->vf30()[0]; func_80174C98((void*)acc, &val, 0xA)
        int val;
        {
            void* obj4 = *(void**)((u8*)acc + 4);
            void* v4 = *(void**)obj4;
            typedef void* (*VF30)(void*);
            VF30 vf30 = (VF30)(*(void**)((u8*)v4 + 0x30));
            val = *(u32*)ecVf30(obj4);
        }
        if (func_80174C98((void*)acc, &val, 0xA)) {       // beq .L_800EE84C
            void* accStat = vcall_p(acc, SLOT_VF298);   // acc->vf298()
            if (*(u32*)((u8*)accStat + 0x78) & 0x08000000) {  // rlwinm. 0,0,4,4
                return 0;                               // li r3,0; b .L_800F41D4
            }
        }

        // acc->field_3F00 & 0x2 -> walk self->0x48 list; empty -> return 0
        if (acc->field_3F00 & 0x2) {               // rlwinm. 0,0,30,30; beq
            void* head = *(void**)((u8*)(uintptr_t)self + 0x48);   // lwz r5, 0x48(r22)
            s32 count = 0;                              // li r4, 0
            void* node = *(void**)head;                 // lwz r3, 0(r5)
            while (node != head) {                      // .L_800EE868/.L_800EE870
                node = *(void**)node;                   // lwz r3, 0(r3)
                count++;                                // addi r4, r4, 1
            }
            if (count == 0) {                           // cmpwi r4,0; bne
                return 0;                               // li r3,0; b .L_800F41D4
            }
        }

        // .L_800EE888 -- pc != nullptr main branch; r14 = tgt ? tgt : pc->vf298()
        if (pc != nullptr) {
            void* r14;
            if (tgt != nullptr) r14 = (void*)tgt; // mr r14, r26
            else r14 = vcall_p(pc, SLOT_VF298);              // pc->vf298()

            if ((*(u32*)((u8*)r14 + 0x74) & 0x4) ||     // rlwinm. 0,3,29,29; bne .L_800EEADC
                (*(u32*)((u8*)r14 + 0x74) & 0x2)) {     // rlwinm. 0,3,30,30; bne .L_800EEADC
                return 0;                               // .L_800EEADC: li r3,0
            }

            if (!(evt->field_10 & 0x4)) {                // rlwinm. 0,0,29,29; bne .L_800EE950
                // Menu gate: vf324()[0] & 0x8, status 0x3A, status 0x32
                s32 menuFlag = 0;                       // li r15, 0
                if (*(u16*)vcall_p(acc, SLOT_VF324) & 0x8) menuFlag = 1;  // rlwinm. 0,0,28,28
                if (func_80148778((&acc->subObject), 0x3A)) menuFlag = 1;
                if (func_80148778((&acc->subObject), 0x32)) menuFlag = 1;
                if (menuFlag) {                         // cmpwi r15,0; beq .L_800EE950
                    return func_800F3734(
                        self, (BattleObjAccessor*)acc, evt, (BattleTargetData*)r14);
                }
            }

            // .L_800EE950 -- roll gate (runs even when field_10 & 0x4)
            s32 rollFlag = 0;                           // li r15, 0
            if (!(evt->field_10 & 0x5)) {                // andi. r0, r0, 0x5; bne .L_800EE9C8
                if (func_80148778((&acc->subObject), 0x21)) {
                    void* entry21 = func_80149154((&acc->subObject), 0x21);
                    s32 rollVal;
                    if (tgt != nullptr) rollVal = (s32)tgt->field_B4;
                    else rollVal = mtRand__Q22ml4mathFi(100);
                    if (rollVal < *(s32*)((u8*)entry21 + 0x10)) rollFlag = 1;
                }
                if ((acc->field_3374 & 0x80) &&    // rlwinm. 0,0,24,24; beq
                    !(*(u32*)((u8*)r14 + 0x74) & 0x4000)) {  // rlwinm. 0,0,17,17; bne
                    rollFlag = 1;
                }
            }
            if (!(evt->field_10 & 0x6)) {                // rlwinm. 0,0,29,30; bne .L_800EEA2C
                if (!func_80148778((&acc->subObject), 0x12) &&
                    !func_80148778((&acc->subObject), 0x10)) {
                    if (acc->field_3F00 & 0x4) rollFlag = 1;  // rlwinm. 0,0,29,29
                }
            }
            if (rollFlag) {                             // cmpwi r15,0; beq .L_800EEA2C
                return func_800F37F8(
                    self, (BattleObjAccessor*)acc, evt, (BattleTargetData*)r14);
            }

            // .L_800EEA2C -- pc skills 0x9C (multiply) / 0x9D (divide)
            if (func_80148778(&((EC918_BattleObjAccessor*)pc)->subObject, 0x9C)) {
                void* entry9C = func_80149154(&((EC918_BattleObjAccessor*)pc)->subObject, 0x9C);
                evt->field_20 *= 1.0f + (f32)(s32)(*(s32*)((u8*)entry9C + 0x10)) / k100_0f;
            } else if (func_80148778(&((EC918_BattleObjAccessor*)pc)->subObject, 0x9D)) {
                void* entry9D = func_80149154(&((EC918_BattleObjAccessor*)pc)->subObject, 0x9D);
                evt->field_20 /= 1.0f + (f32)(s32)(*(s32*)((u8*)entry9D + 0x10)) / k100_0f;
            }
        } else {
            // .L_800EEAE4 -- pc == nullptr branch
            if (!(evt->field_10 & 0x4)) {                // rlwinm. 0,0,29,29; bne .L_800EEB68
                s32 menuFlag = 0;                       // li r14, 0
                if (*(u16*)vcall_p(acc, SLOT_VF324) & 0x8) menuFlag = 1;
                if (func_80148778((&acc->subObject), 0x3A)) menuFlag = 1;
                if (func_80148778((&acc->subObject), 0x32)) menuFlag = 1;
                if (menuFlag) {                         // cmpwi r14,0; beq .L_800EEB68
                    return func_800F3734(self, (BattleObjAccessor*)acc, evt, tgt);
                }
            }

            // .L_800EEB68 -- roll gate (no 0x3374 check here)
            s32 rollFlag = 0;                           // li r15, 0
            if (!(evt->field_10 & 0x5)) {                // andi. r0, r0, 0x5; bne .L_800EEBC4
                if (func_80148778((&acc->subObject), 0x21)) {
                    void* entry21 = func_80149154((&acc->subObject), 0x21);
                    s32 rollVal;
                    if (tgt != nullptr) rollVal = (s32)tgt->field_B4;
                    else rollVal = mtRand__Q22ml4mathFi(100);
                    if (rollVal < *(s32*)((u8*)entry21 + 0x10)) rollFlag = 1;
                }
            }
            if (!(evt->field_10 & 0x6)) {                // rlwinm. 0,0,29,30; bne .L_800EEC28
                if (!func_80148778((&acc->subObject), 0x12) &&
                    !func_80148778((&acc->subObject), 0x10)) {
                    if (acc->field_3F00 & 0x4) rollFlag = 1;
                }
            }
            if (rollFlag) {                             // cmpwi r15,0; beq .L_800EEC28
                return func_800F37F8(self, (BattleObjAccessor*)acc, evt, tgt);
            }
        }

        // .L_800EEC28 -- clear event head, fall to shared tail
        evt->field_04 = 0;                               // li r0,0; stw 0x4(r25)
        evt->field_00 = 0;                               // stw r0, 0x0(r25)
        break;                                          // b .L_800F4000
    }

    case 15: {
        int val;
        {
            void* obj4 = *(void**)((u8*)acc + 4);
            void* v4 = *(void**)obj4;
            typedef void* (*VF30)(void*);
            VF30 vf30 = (VF30)(*(void**)((u8*)v4 + 0x30));
            val = *(u32*)ecVf30(obj4);
        }
        if (func_80174C98((void*)acc, &val, 0xA)) {       // beq .L_800EEC94
            void* accStat = vcall_p(acc, SLOT_VF298);
            if (*(u32*)((u8*)accStat + 0x78) & 0x08000000) {
                return 0;                               // li r3,0; b .L_800F41D4
            }
        }

        if (acc->field_3F00 & 0x2) {               // rlwinm. 0,0,30,30; beq .L_800EECD0
            void* head = *(void**)((u8*)(uintptr_t)self + 0x48);
            s32 count = 0;
            void* node = *(void**)head;
            while (node != head) {
                node = *(void**)node;
                count++;
            }
            if (count == 0) {
                return 0;                               // li r3,0; b .L_800F41D4
            }
        }

        // .L_800EECD0 -- pc != nullptr main branch
        if (pc != nullptr) {
            void* r14;
            if (tgt != nullptr) r14 = (void*)tgt; // mr r14, r26
            else r14 = vcall_p(pc, SLOT_VF298);              // pc->vf298()

            if ((*(u32*)((u8*)r14 + 0x74) & 0x4) ||     // rlwinm. 0,3,29,29; bne .L_800EEF2C
                (*(u32*)((u8*)r14 + 0x74) & 0x2)) {     // rlwinm. 0,3,30,30; bne .L_800EEF2C
                return 0;                               // .L_800EEF2C: li r3,0
            }

            // field_30 & 0x80 -> skip everything, clear + tail (.L_800EF0A4)
            if (!(evt->field_30 & 0x80)) {               // rlwinm. 0,0,24,24; bne .L_800EF0A4
                if (!(evt->field_10 & 0x4)) {            // rlwinm. 0,0,29,29; bne .L_800EEDA4
                    // Menu gate: vf320()[0] & 0x8, status 0x3B, status 0x32
                    s32 menuFlag = 0;                   // li r15, 0
                    if (*(u16*)vcall_p(acc, SLOT_VF320) & 0x8) menuFlag = 1;  // rlwinm. 0,0,28,28
                    if (func_80148778((&acc->subObject), 0x3B)) menuFlag = 1;
                    if (func_80148778((&acc->subObject), 0x32)) menuFlag = 1;
                    if (menuFlag) {                     // cmpwi r15,0; beq .L_800EEDA4
                        return func_800F3734(
                            self, (BattleObjAccessor*)acc, evt, (BattleTargetData*)r14);
                    }
                }

                // .L_800EEDA4 -- roll gate (runs even when field_10 & 0x4)
                s32 rollFlag = 0;                       // li r15, 0
                if (!(evt->field_10 & 0x5)) {            // andi. r0, r0, 0x5; bne .L_800EEE1C
                    if (func_80148778((&acc->subObject), 0x20)) {
                        void* entry20 = func_80149154((&acc->subObject), 0x20);
                        s32 rollVal;
                        if (tgt != nullptr) rollVal = (s32)tgt->field_B4;
                        else rollVal = mtRand__Q22ml4mathFi(100);
                        if (rollVal < *(s32*)((u8*)entry20 + 0x10)) rollFlag = 1;
                    }
                    if ((acc->field_3374 & 0x80) &&      // .L_800EEE00
                        !(*(u32*)((u8*)r14 + 0x74) & 0x4000)) {
                        rollFlag = 1;
                    }
                }
                if (!(evt->field_10 & 0x6)) {            // rlwinm. 0,0,29,30; bne .L_800EEE7C
                    if (!func_80148778((&acc->subObject), 0x10)) {
                        if (acc->field_3F00 & 0x4) rollFlag = 1;
                    }
                }
                if (rollFlag) {                         // cmpwi r15,0; beq .L_800EEE7C
                    *(u32*)((u8*)r14 + 0x74) |= 0x80000010;  // oris 0x8000; ori 0x10
                    return func_800F37F8(self, (BattleObjAccessor*)acc, evt, tgt);
                }

                // .L_800EEE7C -- pc skills 0x9A (multiply) / 0x9B (divide)
                if (func_80148778(&((EC918_BattleObjAccessor*)pc)->subObject, 0x9A)) {
                    void* entry9A = func_80149154(&((EC918_BattleObjAccessor*)pc)->subObject, 0x9A);
                    evt->field_20 *= 1.0f + (f32)(s32)(*(s32*)((u8*)entry9A + 0x10)) / k100_0f;
                } else if (func_80148778(&((EC918_BattleObjAccessor*)pc)->subObject, 0x9B)) {
                    void* entry9B = func_80149154(&((EC918_BattleObjAccessor*)pc)->subObject, 0x9B);
                    evt->field_20 /= 1.0f + (f32)(s32)(*(s32*)((u8*)entry9B + 0x10)) / k100_0f;
                }
            }
        } else {
            // .L_800EEF34 -- pc == nullptr branch
            if (func_80148778((&acc->subObject), 0x32)) {
                return 0;                               // bne .L_800EF09C
            }
            if (!(evt->field_30 & 0x80)) {               // rlwinm. 0,0,24,24; bne .L_800EF0A4
                if (!(evt->field_10 & 0x4)) {            // rlwinm. 0,0,29,29; bne .L_800EEFD8
                    s32 menuFlag = 0;                   // li r14, 0
                    if (*(u16*)vcall_p(acc, SLOT_VF320) & 0x8) menuFlag = 1;
                    if (func_80148778((&acc->subObject), 0x3B)) menuFlag = 1;
                    if (func_80148778((&acc->subObject), 0x32)) menuFlag = 1;
                    if (menuFlag) {                     // cmpwi r14,0; beq .L_800EEFD8
                        return func_800F3734(self, (BattleObjAccessor*)acc, evt, tgt);
                    }
                }

                // .L_800EEFD8 -- roll gate (no 0x3374 check)
                s32 rollFlag = 0;                       // li r15, 0
                if (!(evt->field_10 & 0x5)) {            // andi. r0, r0, 0x5; bne .L_800EF034
                    if (func_80148778((&acc->subObject), 0x20)) {
                        void* entry20 = func_80149154((&acc->subObject), 0x20);
                        s32 rollVal;
                        if (tgt != nullptr) rollVal = (s32)tgt->field_B4;
                        else rollVal = mtRand__Q22ml4mathFi(100);
                        if (rollVal < *(s32*)((u8*)entry20 + 0x10)) rollFlag = 1;
                    }
                }
                if (!(evt->field_10 & 0x6)) {            // rlwinm. 0,0,29,30; bne .L_800EF0A4
                    if (!func_80148778((&acc->subObject), 0x10)) {
                        if (acc->field_3F00 & 0x4) rollFlag = 1;
                    }
                }
                if (rollFlag) {                         // cmpwi r15,0; beq .L_800EF0A4
                    if (tgt != nullptr) {            // cmpwi r26,0; beq .L_800EF084
                        tgt->field_74 |= 0x80000010;  // oris 0x8000; ori 0x10; stw
                    }
                    return func_800F37F8(self, (BattleObjAccessor*)acc, evt, tgt);
                }
            }
        }

        // .L_800EF0A4 -- clear event head, fall to shared tail
        evt->field_04 = 0;                               // li r0,0; stw 0x4(r25)
        evt->field_00 = 0;                               // stw r0, 0x0(r25)
        break;                                          // b .L_800F4000
    }

    case 102: {
        if (func_80148778((&acc->subObject), 0x86)) {
            void* entry86 = func_80149154((&acc->subObject), 0x86);
            if (*(s32*)((u8*)entry86 + 0x10) >= 0x64) {
                return func_800F37F8(self, (BattleObjAccessor*)acc, evt, tgt);
            }
        }

        // .L_800EF0F8 -- field_10 = (s32)((f64)field_10 * round(sum)/100)
        if (evt->field_08 != 0x8000 && tgt != nullptr) {  // cmplwi 0x8000; beq / cmpwi r26; beq
            f32 sum = tgt->field_5C + tgt->field_60;  // fadds f1, f2, f1
            s32 roundVal;
            if (sum < 0.0f) {
                roundVal = 0;
            } else if (tgt->field_74 & 0x80) {
                roundVal = 0;                               // li r0, 0
            } else {
                roundVal = (s32)((f64)sum + (sum > k0_0f ? k0_5 : kM0_5));
            }
            evt->field_10 = (s32)((f32)(s32)evt->field_10 *
                                 ((f32)(s32)roundVal / k100_0f));  // fsubs+fdivs+fmuls+fctiwz
        }

        // .L_800EF1A4 -- pc skill 0x85: field_10 += field_10/100 * entry
        if (pc != nullptr && func_80148778(&((EC918_BattleObjAccessor*)pc)->subObject, 0x85)) {
            void* entry85 = func_80149154(&((EC918_BattleObjAccessor*)pc)->subObject, 0x85);
            s32 entryVal = *(s32*)((u8*)entry85 + 0x10);
            evt->field_10 = (s32)(((f32)(s32)evt->field_10 / k100_0f) *
                                 (f32)(s32)entryVal + (f32)(s32)evt->field_10);  // fmadds + fctiwz
        }
        break;                                          // b .L_800F4000
    }

    case 103: {
        if (func_80148778((&acc->subObject), 0x88)) {
            void* entry88 = func_80149154((&acc->subObject), 0x88);
            if (*(s32*)((u8*)entry88 + 0x10) >= 0x64) {
                return func_800F37F8(self, (BattleObjAccessor*)acc, evt, tgt);
            }
        }

        if (evt->field_08 != 0x8000 && tgt != nullptr) {
            f32 sum = tgt->field_5C + tgt->field_60;
            s32 roundVal;
            if (sum < 0.0f) {
                roundVal = 0;
            } else if (tgt->field_74 & 0x80) {
                roundVal = 0;
            } else {
                roundVal = (s32)((f64)sum + (sum > k0_0f ? k0_5 : kM0_5));
            }
            evt->field_10 = (s32)((f32)(s32)evt->field_10 *
                                 ((f32)(s32)roundVal / k100_0f));  // fsubs+fdivs+fmuls+fctiwz
        }

        if (pc != nullptr && func_80148778(&((EC918_BattleObjAccessor*)pc)->subObject, 0x87)) {
            void* entry87 = func_80149154(&((EC918_BattleObjAccessor*)pc)->subObject, 0x87);
            s32 entryVal = *(s32*)((u8*)entry87 + 0x10);
            evt->field_10 = (s32)(((f32)(s32)evt->field_10 / k100_0f) *
                                 (f32)(s32)entryVal + (f32)(s32)evt->field_10);
        }
        if (pc != nullptr && vcall_p(pc, SLOT_VF290) != nullptr) {
            s32 bonus;
            if (func_80260264(vcall_p(pc, SLOT_VF290), 0x80, &bonus)) {
                evt->field_20 += (f32)bonus;
            }
        }
        break;                                          // b .L_800F4000
    }

    case 104: {
        if (func_80148778((&acc->subObject), 0x8A)) {
            void* entry8A = func_80149154((&acc->subObject), 0x8A);
            if (*(s32*)((u8*)entry8A + 0x10) >= 0x64) {
                return func_800F37F8(self, (BattleObjAccessor*)acc, evt, tgt);
            }
        }

        if (evt->field_08 != 0x8000 && tgt != nullptr) {
            f32 sum = tgt->field_5C + tgt->field_60;
            s32 roundVal;
            if (sum < 0.0f) {
                roundVal = 0;
            } else if (tgt->field_74 & 0x80) {
                roundVal = 0;
            } else {
                roundVal = (s32)((f64)sum + (sum > k0_0f ? k0_5 : kM0_5));
            }
            evt->field_10 = (s32)((f32)(s32)evt->field_10 *
                                 ((f32)(s32)roundVal / k100_0f));  // fsubs+fdivs+fmuls+fctiwz
        }

        if (pc != nullptr && func_80148778(&((EC918_BattleObjAccessor*)pc)->subObject, 0x89)) {
            void* entry89 = func_80149154(&((EC918_BattleObjAccessor*)pc)->subObject, 0x89);
            s32 entryVal = *(s32*)((u8*)entry89 + 0x10);
            evt->field_10 = (s32)(((f32)(s32)evt->field_10 / k100_0f) *
                                 (f32)(s32)entryVal + (f32)(s32)evt->field_10);
        }
        if (pc != nullptr && vcall_p(pc, SLOT_VF290) != nullptr) {
            s32 bonus;
            if (func_80260264(vcall_p(pc, SLOT_VF290), 0x7F, &bonus)) {
                evt->field_20 += (f32)bonus;
            }
        }
        break;                                          // b .L_800F4000
    }

    case 105: {
        if (func_80148778((&acc->subObject), 0x8C)) {
            void* entry8C = func_80149154((&acc->subObject), 0x8C);
            if (*(s32*)((u8*)entry8C + 0x10) >= 0x64) {
                return func_800F37F8(self, (BattleObjAccessor*)acc, evt, tgt);
            }
        }

        if (evt->field_08 != 0x8000 && tgt != nullptr) {
            f32 sum = tgt->field_5C + tgt->field_60;
            s32 roundVal;
            if (sum < 0.0f) {
                roundVal = 0;
            } else if (tgt->field_74 & 0x80) {
                roundVal = 0;
            } else {
                roundVal = (s32)((f64)sum + (sum > k0_0f ? k0_5 : kM0_5));
            }
            evt->field_10 = (s32)((f32)(s32)evt->field_10 *
                                 ((f32)(s32)roundVal / k100_0f));  // fsubs+fdivs+fmuls+fctiwz
        }

        if (pc != nullptr && func_80148778(&((EC918_BattleObjAccessor*)pc)->subObject, 0x8B)) {
            void* entry8B = func_80149154(&((EC918_BattleObjAccessor*)pc)->subObject, 0x8B);
            s32 entryVal = *(s32*)((u8*)entry8B + 0x10);
            evt->field_10 = (s32)(((f32)(s32)evt->field_10 / k100_0f) *
                                 (f32)(s32)entryVal + (f32)(s32)evt->field_10);
        }
        if (pc != nullptr && vcall_p(pc, SLOT_VF290) != nullptr) {
            s32 bonus;
            if (func_80260264(vcall_p(pc, SLOT_VF290), 0x81, &bonus)) {
                evt->field_20 += (f32)bonus;
            }
        }
        break;                                          // b .L_800F4000
    }

    case 106: {
        if (pc != nullptr && func_80148778(&((EC918_BattleObjAccessor*)pc)->subObject, 0x4E)) {
            void* entry4E = func_80149154(&((EC918_BattleObjAccessor*)pc)->subObject, 0x4E);
            s32 entryVal = *(s32*)((u8*)entry4E + 0x10);
            evt->field_10 = (s32)(((f32)(s32)evt->field_10 / k100_0f) *
                                 (f32)(s32)entryVal + (f32)(s32)evt->field_10);  // fmadds + fctiwz
        }
        break;                                          // b .L_800F4000
    }

    case 51: {
        if (artsData != nullptr) {                    // cmpwi r16,0; beq .L_800EF880
            // (chunk 0) s32 artsLevel = ecVf0C(artsData);
            void* artsVTbl = *(void**)((u8*)artsData + 0x84);
            typedef s32 (*ArtsVf0C)(void*);
            ArtsVf0C vf0C = (ArtsVf0C)(*(void**)((u8*)artsVTbl + 0x0C));
            s32 artsLevel = ecVf0C(artsData);

            // 0x800EF860..0x800EF87C (chunk 1):
            //   lbz r4, 0x6f(r16); subi r3, r3, 1; lha r0, 0x4a(r16)
            //   mullw r3, r4, r3; add r0, r0, r3; mulli r0, r0, 0xa
            //   sth r0, 0x14(r25); b .L_800F4000
            evt->field_14 = (s16)((*(s16*)((u8*)artsData + 0x4A) +
                                  (s32)(*(u8*)((u8*)artsData + 0x6F)) *
                                      (artsLevel - 1)) * 10);
        } else {
            // .L_800EF880: acc->vf12C() float scale -> field_14
            //   f1 = vf12C(acc); (f32)(s32)field_10 / 100.0f * f1; fctiwz
            evt->field_14 = (s16)((f32)(s32)evt->field_10 / k100_0f *
                                 vcall_f(acc, SLOT_VF12C));
        }
        break;                                      // b .L_800F4000
    }

    case 206: {
        if (evt->field_10 != 0) break;               // bne .L_800F4000
        if (artsData != nullptr) {                  // beq .L_800F4000
            void* artsVTbl = *(void**)((u8*)artsData + 0x84);
            typedef s32 (*ArtsVf0C)(void*);
            ArtsVf0C vf0C = (ArtsVf0C)(*(void**)((u8*)artsVTbl + 0x0C));
            evt->field_10 = ecVf0C(artsData);          // stw r3, 0x10(r25)
        }
        break;                                      // b .L_800F4000
    }

    case 54: {
        if (artsData != nullptr) {                  // beq .L_800F4000
            void* artsVTbl = *(void**)((u8*)artsData + 0x84);
            typedef s32 (*ArtsVf0C)(void*);
            ArtsVf0C vf0C = (ArtsVf0C)(*(void**)((u8*)artsVTbl + 0x0C));
            // subi r3, r3, 1; lha r0, 0x14(r25); extsh; add; sth
            evt->field_14 = (s16)((s32)evt->field_14 + (ecVf0C(artsData) - 1));
        }
        break;                                      // b .L_800F4000
    }

    case 82: {
        if (vcall_p(acc, SLOT_VF290) != nullptr) {  // beq .L_800F4000
            if (func_8026178C(vcall_p(acc, SLOT_VF290), 0x7B)) {
                return func_800F37F8(self, (BattleObjAccessor*)acc, evt, tgt);
            }
            if (func_8026178C(vcall_p(acc, SLOT_VF290), 0x66)) {
                return func_800F37F8(self, (BattleObjAccessor*)acc, evt, tgt);
            }
        }
        break;                                      // b .L_800F4000
    }

    case 84: {
        if (vcall_p(acc, SLOT_VF290) != nullptr) {  // beq .L_800F4000
            if (func_8026178C(vcall_p(acc, SLOT_VF290), 0x7D)) {
                return func_800F37F8(self, (BattleObjAccessor*)acc, evt, tgt);
            }
            if (func_8026178C(vcall_p(acc, SLOT_VF290), 0x66)) {
                return func_800F37F8(self, (BattleObjAccessor*)acc, evt, tgt);
            }
        }
        break;                                      // b .L_800F4000
    }

    case 83:
    case 87: {
        if (vcall_p(acc, SLOT_VF290) != nullptr) {  // beq .L_800F4000
            if (func_8026178C(vcall_p(acc, SLOT_VF290), 0x66)) {
                return func_800F37F8(self, (BattleObjAccessor*)acc, evt, tgt);
            }
        }
        break;                                      // b .L_800F4000
    }

    case 60:

    case 61: {
        if (vcall_p(acc, SLOT_VF290) != nullptr) {  // beq .L_800F4000
            if (func_8026178C(vcall_p(acc, SLOT_VF290), 0x66)) {
                return func_800F37F8(self, (BattleObjAccessor*)acc, evt, tgt);
            }
        }
        break;                                      // b .L_800F4000
    }

    case 197: {
        if (acc != nullptr &&                  // cmpwi r24,0; beq tail
            vcall_i(acc, SLOT_VF2BC) == 0) {        // bne tail
            f32 factor = k100_0f * func_800D7EA0((u8*)pc, tgt);  // fmuls f4, f0(100), f1
            // fctiwz((f64)(s32)field * factor) for each field
            evt->field_10 = (s32)((f32)(s32)evt->field_10 * factor);  // stw
            evt->field_14 = (s16)((f32)(s32)evt->field_14 * factor);  // sth
            evt->field_16 = (s16)((f32)(s32)evt->field_16 * factor);  // sth
        }
        break;                                      // b .L_800F4000
    }

    case 198: {
        f32 factor = k100_0f * func_800D7EA0((u8*)pc, tgt);
        evt->field_10 = (s32)((f32)(s32)evt->field_10 * factor);
        evt->field_14 = (s16)((f32)(s32)evt->field_14 * factor);
        evt->field_16 = (s16)((f32)(s32)evt->field_16 * factor);
        break;                                      // b .L_800F4000
    }

    case 212: {
        if (evt->field_10 != 0) {                    // beq .L_800EFD60
            // field_10 != 0 path (0x800EFC60..0x800EFD5C)
            EnumListHolder holder;
            func_80043D90(&holder);
            func_800F4A98(func_80043F18(&holder), 0x80000000, 0);  // lis r4,0x8000
            for (u32 i = 0; i < *(u32*)((u8*)func_80043F18(&holder) + 0x620); i++) {
                void* actorAcc = func_8016FE34(func_800F6EAC(func_80043F18(&holder), i));
                // ecVf2D4(actorAcc, acc->3F10) -> hit result (or 0)
                void* res = vcall_p1(actorAcc, SLOT_VF2D4, acc->field_3F10);
                if (res != nullptr) {               // beq .L_800EFD38
                    // Apply the percentage independently to each vector component.
                    *(f32*)((u8*)res + 0x10) *=
                        k1_0f - (f32)(s32)evt->field_10 / k100_0f;
                    *(f32*)((u8*)res + 0x00) *=
                        k1_0f - (f32)(s32)evt->field_10 / k100_0f;
                    *(f32*)((u8*)res + 0x04) *=
                        k1_0f - (f32)(s32)evt->field_10 / k100_0f;
                }
            }
            __dt__80043E88(&holder, -1);            // li r4,-1; bl __dt__80043E88
        } else {
            // .L_800EFD60: field_10 == 0 path
            func_800EA2A4((cf::CBattleManager*)self, (BattleObjAccessor*)pc);                // mr r3,r22; mr r4,r23; bl
        }
        break;                                      // b .L_800F4000
    }

    case 213:
    case 263: {
        if (vcall_i(acc, SLOT_VF2BC) != 0) break;   // bne .L_800F4000
        if (func_80148778((&acc->subObject), 0x120)) break;  // bne .L_800F4000

        s32 r17 = evt->field_10;                     // lwz r17, 0x10(r25)
        if (evt->field_08 == 0x8000) {               // cmplwi r0,0x8000; bne .L_800EFDE4
            // func_800D81A8(pc, acc, tgt); r17 = (s32)((f32)(s32)field_10 * f1)
            r17 = (s32)((f32)(s32)evt->field_10 * func_800D81A8(pc, acc, tgt));
        }

        f32 f28 = func_800D7EA0((u8*)pc, tgt);        // .L_800EFDE4: fmr f28, f1
        if (pc != nullptr) {                        // cmpwi r23,0; beq .L_800EFE98
            u32 r14 = acc->field_3F10;         // lwz r14, 0x3f10(r24)
            f32 prod = k0_0f;
            if (artsData != nullptr) {              // beq .L_800EFE44
                // lhz r15, 0x46(r16); ecVf108(pc); addi +0xe; mullw
                u16 artsId = *(u16*)((u8*)artsData + 0x46);
                s32 level = vcall_i(pc, SLOT_VF108);
                prod = f28 * (f32)(s32)((s32)artsId * (level + 14));
            }
            // .L_800EFE44: prod = 0.0f (when artsData == null)
            s32 r6 = (s32)prod;                     // fctiwz; lwz 0x854(r1)
            // r5 = (s32)(1.25f * (f32)(s32)r17 * f28)
            s32 r5 = (s32)(k1_25f * (f32)(s32)r17 * f28);
            // func_800E9FE4(self, pc, r5, r6, 0, 1, acc->3F10)
            func_800E9FE4((void*)(uintptr_t)self, pc, r5, r6, 0, 1,
                          (void*)(uintptr_t)r14);
        }

        // .L_800EFE98: ecVf5B0(acc, (f32)(s32)r17, tgt->field_74)
        {
            void* vt = *(void**)acc;
            typedef void (*Vf5B0)(void*, f32, u32);
            Vf5B0 vf5B0 = (Vf5B0)(*(void**)((u8*)vt + 0x5B0));
            ecVf5B0(acc, (f32)(s32)r17, tgt->field_74);  // lwz r4, 0x74(r26)
        }
        break;                                      // b .L_800F4000
    }

    case 214: {
        if (vcall_i(acc, SLOT_VF2BC) != 0) {        // beq .L_800F4000 (body runs when != 0)
            // r4 = acc != 0 ? acc + 0x3E9C : acc(0); func_800451D8(0x4C, r4)
            void* obj = (acc != nullptr) ? (void*)((u8*)acc + 0x3E9C)
                                              : (void*)acc;
            func_800451D8(0x4C, (int)(obj));

            s32 r14 = (s32)vcall_f(acc, SLOT_VF12C);  // fctiwz; lwz 0x85c(r1)
            vcall_p1(acc, SLOT_VFAC, 0);            // ecVfAC(acc, 0)

            func_8010975C(3);                       // li r3,3; bl
            func_80109770(0);                       // li r3,0; bl
            func_80109734((void*)(uintptr_t)acc->field_3F10, (u32)r14);  // lwz 3f10; mr r4,r14; bl
        }
        break;                                      // b .L_800F4000
    }

    case 215: {
        if (vcall_i(acc, SLOT_VF2BC) != 0) break;   // bne .L_800F4000

        evt->prevEventType = evt->eventType;          // lhz + sth 0x2e
        evt->field_30 = copyBattleEvent(eventWorkspace.case215Event, *evt) | 2;

        for (u32 i = 0; i < 0x20; i++) {            // li r14,0; cmpwi r14,0x20; blt
            void* evt2 = vcall_p1((&acc->subObject), SLOT_VF5C, i);  // slot 0x5C
            // prevEventType classifier 0 AND (field_08 & 0x7000) == 0
            if (func_80145C00(*(u16*)((u8*)evt2 + 0x2E)) == 0 &&
                (*(u32*)((u8*)evt2 + 0x08) & 0x7000) == 0) {   // rlwinm 17-19
                vcall_v1((&acc->subObject), SLOT_VF24, (u32)(uintptr_t)evt2);   // slot 0x24
            }
        }

        eventWorkspace.case215Event.eventType = 0x27;                    // li r3,0x27; sth 0x770(r1)
        eventWorkspace.case215Event.field_10 = 0x64;                     // li r0,0x64; stw 0x774(r1)
        func_800EC918(self, pc, acc, &eventWorkspace.case215Event, tgt);
        break;                                      // b .L_800F4000
    }

    case 216: {
        if (vcall_i(acc, SLOT_VF2BC) != 0) break;   // bne .L_800F4000

        s32 f10 = evt->field_10;                     // lwz r4, 0x10(r25)
        if (f10 == -1) {                            // cmpwi r4,-1; bne .L_800F019C
            // Start at a random slot and wrap through all 32 signed indices.
            s32 start = rand() % 32;
            for (s32 i = 0; i < 0x20; i++) {        // cmpwi r14,0x20; blt
                void* evt2 = vcall_p1((&acc->subObject), SLOT_VF58,
                                      (start + i) % 32);
                if (*(u16*)((u8*)evt2 + 0x0C) != 0 &&
                    (*(u16*)((u8*)evt2 + 0x2E) == 0 ||
                     func_80145C00(*(u16*)((u8*)evt2 + 0x2E)) == 0) &&
                    (*(u32*)((u8*)evt2 + 0x08) & 0x7000) == 0) {
                    vcall_v1((&acc->subObject), SLOT_VF24, (u32)(uintptr_t)evt2);   // slot 0x24
                    if (evt->field_14 != 0) {        // lha 0x14; beq tail
                        // ecVf154(pc, (f32)(s32)field_14 * (vf15C(pc)/100))
                        f32 f = vcall_f(pc, SLOT_VF15C) / k100_0f;
                        vcall_f1(pc, SLOT_VF154, (f32)(s32)evt->field_14 * f);
                    }
                    break;   // retail: b .L_800F4000 (success exits the case)
                }
            }
        } else if (f10 == 0) {                     // cmpwi r4,0; bne .L_800F0274
            for (u32 i = 0; i < 0x20; i++) {
                void* evt2 = vcall_p1((&acc->subObject), SLOT_VF58, i);
                if (*(u16*)((u8*)evt2 + 0x0C) != 0 &&
                    (*(u16*)((u8*)evt2 + 0x2E) == 0 ||
                     func_80145C00(*(u16*)((u8*)evt2 + 0x2E)) == 0) &&
                    (*(u32*)((u8*)evt2 + 0x08) & 0x7000) == 0) {
                    if (evt->field_14 != 0) {        // lha 0x14; beq .L_800F024C
                        f32 f = vcall_f(pc, SLOT_VF15C) / k100_0f;
                        vcall_f1(pc, SLOT_VF154, (f32)(s32)evt->field_14 * f);
                    }
                    vcall_v1((&acc->subObject), SLOT_VF24, (u32)(uintptr_t)evt2);   // slot 0x24
                }
            }
        } else {
            // .L_800F0274: ecVf20(sub, field_10) -- clear that status id
            vcall_v1((&acc->subObject), SLOT_VF20, (u32)f10);  // slot 0x20
            if (evt->field_14 != 0) {                // lha 0x14; beq tail
                f32 f = vcall_f(pc, SLOT_VF15C) / k100_0f;
                vcall_f1(pc, SLOT_VF154, (f32)(s32)evt->field_14 * f);
            }
        }
        break;                                      // b .L_800F4000
    }

    case 217: {
        if (vcall_i(acc, SLOT_VF2BC) != 0) break;   // bne .L_800F4000
        ecVf20(&acc->subObject, 0x111);              // slot 0x20, li r4,0x111
        ecVf20(&acc->subObject, 0x112);
        ecVf20(&acc->subObject, 0x113);
        ecVf20(&acc->subObject, 0x114);
        ecVf20(&acc->subObject, 0x115);
        break;                                      // b .L_800F4000
    }

    case 218: {
        void* r14 = func_8016FE34(func_800B708C(evt->field_00));  // bl 800B708C; bl 8016FE34
        f32 f28 = func_800D7EA0((u8*)pc, tgt);        // fmr f28, f1
        s32 level = vcall_i(pc, SLOT_VF108);             // addi r4, r3, 0xe
        s32 product = evt->field_10 * (level + 14);  // mullw
        // ecVf2C4(acc, r14, 0.0f, f28 * (f32)(s32)product, 0.0f)
        vcall_v3f(acc, SLOT_VF2C4, r14, k0_0f,
                  f28 * (f32)(s32)product, k0_0f);
        break;                                      // b .L_800F4000
    }

    case 219: {
        void* r14 = func_8016FE34(func_800B708C(evt->field_00));
        s32 level = vcall_i(pc, SLOT_VF108);
        // r17 = field_10 * 100 + (level - 1) * 120   (mulli 0x64 / mulli 0x78)
        s32 r17 = evt->field_10 * 100 + (level - 1) * 120;

        f32 f28 = func_800D7EA0((u8*)pc, tgt);        // fmr f28, f1
        if (pc != nullptr) {                        // cmpwi r23,0; beq .L_800F051C
            if (vcall_p(pc, SLOT_VF290) != nullptr) {    // beq .L_800F051C
                s32 sv;
                // func_80260264(ecVf290(pc), 0x67, &sv) -> f28 += sv/100
                if (func_80260264(vcall_p(pc, SLOT_VF290), 0x67, &sv)) {
                    f28 += (f32)(s32)sv / k100_0f;
                }
                // .L_800F04B0: 0x68 bonus only when acc->3E9C ecVf4C() != pc->3F10
                if (func_80260264(vcall_p(pc, SLOT_VF290), 0x68, &sv)) {
                    void* subVtbl = *(void**)((u8*)acc + 0x3E9C);
                    typedef u32 (*Vf4C)(void*);
                    Vf4C vf4C = (Vf4C)(*(void**)((u8*)subVtbl + 0x4C));
                    if (ecVf4C((u8*)acc + 0x3E9C) != *(u32*)((u8*)pc + 0x3F10)) {
                        f28 += (f32)(s32)sv / k100_0f;
                    }
                }
            }
            // .L_800F051C: status 0xEE on pc -> +0.25
            if (func_80148778((SubAccessor*)((u8*)pc + 8), 0xEE)) {
                f28 += k0_25f;                      // lfs 80666E1C; fadds
            }
        }

        // .L_800F0538: r15 = (s32)((f32)(s32)r17 * f28)
        s32 r15 = (s32)((f32)(s32)r17 * f28);

        if (func_80148778((SubAccessor*)((u8*)pc + 8), 0xED)) {
            // .L_800F0570: status 0xED path -- AOE via filtered actor list
            f32 f28b = (artsData != nullptr) ? *(f32*)((u8*)artsData + 0x60)
                                             : k0_0f;
            EnumListHolder holder;
            func_80043D90(&holder);
            func_800F4A98(func_80043F18(&holder), 0x100, 0);  // li r4,0x100; li r5,0

            void* subVtbl = *(void**)((u8*)pc + 0x3E9C);
            typedef void* (*VfAC)(void*);
            VfAC vfAC = (VfAC)(*(void**)((u8*)subVtbl + 0xAC));
            void* idObj = ecVfAC((u8*)pc + 0x3E9C);   // slot 0xAC

            // __ct__800FB044(list, f28b, idObj, 8)
            __ct__800FB044(func_80043F18(&holder), f28b, idObj, 8);

            for (u32 i = 0; i < *(u32*)((u8*)func_80043F18(&holder) + 0x620); i++) {
                void* actorAcc = func_8016FE34(func_800F6EAC(func_80043F18(&holder), i));
                // ecVf2C4(actorAcc, pc, 0.0f, 0.0f, (f32)(s32)r15)
                vcall_v3f(actorAcc, SLOT_VF2C4, pc, k0_0f, k0_0f, (f32)(s32)r15);
            }
            __dt__80043E88(&holder, -1);
        } else {
            // .L_800F0644: ecVf2C4(acc, r14, 0.0f, 0.0f, (f32)(s32)r15)
            vcall_v3f(acc, SLOT_VF2C4, r14, k0_0f, k0_0f, (f32)(s32)r15);
        }

        // .L_800F0678: ecVf150(pc, 0.0f)
        vcall_f1(pc, SLOT_VF150, k0_0f);
        break;                                      // b .L_800F4000
    }

    case 220: {
        // Skill roll: status 0x26
        if (func_80148778((&acc->subObject), 0x26)) {
            void* entry26 = func_80149154((&acc->subObject), 0x26);
            s32 rollVal;
            if (tgt != nullptr) rollVal = (s32)tgt->field_B4;  // cmpwi r26,0
            else rollVal = mtRand__Q22ml4mathFi(100);
            if (rollVal < *(s32*)((u8*)entry26 + 0x10)) {  // cmpw; bge skip
                return func_800F37F8(self, (BattleObjAccessor*)acc, evt, tgt);
            }
        }

        // .L_800F06F4: tgt != 0 path
        if (tgt != nullptr) {
            // r5 = field_B0; 0x51EB851F fixed-point -> (s32)(field_B0 % 100)
            if ((s32)(tgt->field_B0 % 100) >= evt->field_10) {  // cmpw; blt .L_800F0734
                return 0;                           // li r3,0; b .L_800F41D4
            }
            // .L_800F0734
            if (artsData != nullptr && *(u16*)((u8*)artsData + 0x3C) == 3) {
                if (pc != acc) {               // cmplw r23,r24; beq .L_800F0780
                    // vf11C(acc, -vf12C(acc))
                    vcall_f1(acc, SLOT_VF11C, -vcall_f(acc, SLOT_VF12C));
                } else {
                    vcall_f1(acc, SLOT_VF118, k0_0f);  // lfs 0.0; slot 0x118
                }
            } else {
                // .L_800F079C
                if (pc != acc) {               // beq .L_800F07E4
                    if (evt->field_30 & 0x20000) {   // rlwinm 0,14,14; beq .L_800F084C
                        // ecVf5B0(acc, -vf12C(acc), 0x90000000)
                        void* vt = *(void**)acc;
                        typedef void (*Vf5B0)(void*, f32, u32);
                        Vf5B0 vf5B0 = (Vf5B0)(*(void**)((u8*)vt + 0x5B0));
                        ecVf5B0(acc, -vcall_f(acc, SLOT_VF12C), 0x90000000);
                    }
                } else {
                    vcall_f1(acc, SLOT_VF118, k0_0f);
                }
            }
        } else {
            // .L_800F0800: tgt == 0 path -- plain rand(100) gate
            if (mtRand__Q22ml4mathFi(100) >= evt->field_10) {
                return 0;                           // li r3,0; b .L_800F41D4
            }
            // .L_800F081C: ecVf5B0(acc, -vf128(acc), 0x90000000)
            void* vt = *(void**)acc;
            typedef void (*Vf5B0)(void*, f32, u32);
            Vf5B0 vf5B0 = (Vf5B0)(*(void**)((u8*)vt + 0x5B0));
            ecVf5B0(acc, -vcall_f(acc, SLOT_VF128), 0x90000000);
        }

        // .L_800F084C: status 0x33 clear
        if (func_80148778((&acc->subObject), 0x33)) {
            ecVf20(&acc->subObject, 0x33);           // slot 0x20
        }
        if (tgt != nullptr) {
            tgt->field_74 |= 0x90000000;         // oris r0,r0,0x9000; stw 0x74
            func_80148778((&acc->subObject), 0xFC);  // results unused
            func_80148778((&acc->subObject), 0x100);
        }
        // .L_800F08A4: when acc == pc, flag 0x04000000 + event bit 0x800
        if (acc == pc) {                       // cmplw r24,r23; bne tail
            acc->field_3374 |= 0x04000000;     // oris 0x400
            evt->field_30 |= 0x800;                  // ori 0x800
        }
        break;                                      // b .L_800F4000
    }

    case 221: {
        if (pc == nullptr) break;                   // beq .L_800F4000

        // f28 = tgt->field_58 + func_800D81A8(pc, acc, tgt)
        f32 f28 = tgt->field_58 + func_800D81A8(pc, acc, tgt);

        // r17 = (s32)((f32)(s32)field_10 * vf12C(pc) / 100)
        s32 r17 = (s32)((f32)(s32)evt->field_10 * vcall_f(pc, SLOT_VF12C) / k100_0f);
        // r14 = (s32)((f32)(s32)field_14 * vf12C(pc) / 100)  (vf12C called again)
        s32 r14 = (s32)((f32)(s32)evt->field_14 * vcall_f(pc, SLOT_VF12C) / k100_0f);
        // retail 0x800F097C: r17 is re-scaled by f28 (fmuls f0, f0, f28; fctiwz)
        r17 = (s32)((f32)(s32)r17 * f28);

        vcall_f1(acc, SLOT_VF11C, (f32)(s32)r17);   // slot 0x11C on acc
        vcall_f1(pc, SLOT_VF11C, (f32)(s32)(-r14));      // neg r0,r14 on pc

        f32 dmg = func_800D7EA0((u8*)pc, tgt);        // fmr f28, f1 (overwrites f28)
        if (pc != nullptr) {                        // cmpwi r23,0; beq tail
            u32 r14b = acc->field_3F10;        // lwz r14, 0x3f10(r24)
            f32 prod = k0_0f;
            if (artsData != nullptr) {              // beq .L_800F0A40
                u16 artsId = *(u16*)((u8*)artsData + 0x46);  // lhz r15, 0x46(r16)
                s32 level = vcall_i(pc, SLOT_VF108);
                prod = dmg * (f32)(s32)((s32)artsId * (level + 14));
            }
            s32 r6 = (s32)prod;                     // fctiwz; lwz 0x854(r1)
            s32 r5 = (s32)(k1_25f * (f32)(s32)r17 * dmg);
            func_800E9FE4((void*)(uintptr_t)self, pc, r5, r6, 0, 1,
                          (void*)(uintptr_t)r14b);
        }
        break;                                      // b .L_800F4000
    }

    case 282: {
        if (pc == nullptr) break;                   // beq .L_800F4000
        vcall_f1(acc, SLOT_VF154, (f32)(s32)evt->field_10);  // slot 0x154
        break;                                      // b .L_800F4000
    }

    case 222: {
        // fmuls f1, f2(field_14), f1(vf12C); fdivs /100; fneg
        vcall_f1(acc, SLOT_VF11C,
                 -((f32)(s32)evt->field_14 * vcall_f(acc, SLOT_VF12C) / k100_0f));
        vcall_f1(acc, SLOT_VF154, (f32)(s32)evt->field_10);
        break;                                      // b .L_800F4000
    }

    case 269: {
        vcall_f1(acc, SLOT_VF11C,
                 -((f32)(s32)evt->field_14 * vcall_f(acc, SLOT_VF128) / k100_0f));
        vcall_f1(acc, SLOT_VF154, (f32)(s32)evt->field_10);
        break;                                      // b .L_800F4000
    }

    case 223: {
        if (pc != nullptr && tgt != nullptr) {   // beq .L_800F0CD0
            if (evt->field_08 == 0x8000) {           // cmplwi r0,0x8000; bne .L_800F0C98
                // r14 = field_10 + rand() % (field_14 - field_10 + 1)
                s32 r14 = evt->field_10 + (rand() % (evt->field_14 - evt->field_10 + 1));
                vcall_f1(acc, SLOT_VF11C, (f32)(s32)(-r14));  // neg r0,r14
                f32 f = func_800D81A8(pc, pc, tgt);
                r14 = (s32)((f32)(s32)r14 * f);    // fctiwz
                vcall_f1(pc, SLOT_VF11C, (f32)(s32)r14);
            } else {
                // .L_800F0C98: vf11C(pc, field_64); vf11C(acc, -field_60)
                vcall_f1(pc, SLOT_VF11C, tgt->field_64);   // lfs f1, 0x64(r26)
                vcall_f1(acc, SLOT_VF11C, -tgt->field_60);  // fneg f1, f0
            }
        } else {
            // .L_800F0CD0 (pc==0 or tgt==0): rand range + field_58 add
            s32 r14 = evt->field_10 + (rand() % (evt->field_14 - evt->field_10 + 1));
            vcall_f1(acc, SLOT_VF11C, (f32)(s32)(-r14));
            f32 f = func_800D81A8(pc, pc, tgt);
            // r14 = (s32)((f32)(s32)r14 * (tgt->field_58 + f))
            r14 = (s32)((f32)(s32)r14 * (tgt->field_58 + f));
            vcall_f1(pc, SLOT_VF11C, (f32)(s32)r14);
        }
        break;                                      // b .L_800F4000
    }

    case 224: {
        if (pc != nullptr && tgt != nullptr) {   // beq .L_800F0DCC
            vcall_f1(pc, SLOT_VF11C, tgt->field_64);
            vcall_f1(acc, SLOT_VF11C, -tgt->field_60);
        } else {
            // .L_800F0DCC: r14 = (s32)(0.01f * (f32)(s32)field_10 * vf12C(acc))
            s32 r14 = (s32)(k0_01f * (f32)(s32)evt->field_10 *
                           vcall_f(acc, SLOT_VF12C));
            vcall_f1(acc, SLOT_VF11C, (f32)(s32)(-r14));
            f32 f = func_800D81A8(pc, pc, tgt);
            r14 = (s32)((f32)(s32)r14 * (tgt->field_58 + f));
            vcall_f1(pc, SLOT_VF11C, (f32)(s32)r14);
        }
        break;                                      // b .L_800F4000
    }

    case 225: {
        if (tgt != nullptr) {                    // beq .L_800F4000
            // f1 = 0.01f * field_5C * (f32)(s32)field_10
            f32 v = k0_01f * tgt->field_5C * (f32)(s32)evt->field_10;
            // fcmpo vs 0.0; fadd +/-0.5 (f64); fctiwz -- round to nearest
            s32 r14 = (s32)((f64)v + ((v > k0_0f) ? k0_5 : kM0_5));
            f32 f = func_800D81A8(pc, pc, tgt);
            s32 r14b = (s32)((f32)(s32)r14 * f);
            vcall_f1(pc, SLOT_VF11C, (f32)(s32)r14b);
        }
        break;                                      // b .L_800F4000
    }

    case 226: {
        if (pc == nullptr) break;                   // beq .L_800F4000
        if (acc == pc) break;                  // cmplw r24,r23; beq tail
        s32 r14;
        if (tgt != nullptr) {                    // cmpwi r26,0; beq .L_800F0F90
            // divw remainder: r14 = field_10 + (field_B0 % (field_14-field_10+1))
            r14 = evt->field_10 +
                  ((s32)tgt->field_B0 % (evt->field_14 - evt->field_10 + 1));
        } else {
            r14 = evt->field_10 + (rand() % (evt->field_14 - evt->field_10 + 1));
        }
        // .L_800F0FB4
        vcall_f1(acc, SLOT_VF154, (f32)(s32)(-r14));  // neg r0,r14; slot 0x154
        vcall_f1(pc, SLOT_VF154, (f32)(s32)r14);
        break;                                      // b .L_800F4000
    }

    case 234: {
        evt->prevEventType = evt->eventType;          // sth r4, 0x2e(r25)  (r4 = eventType)
        evt->field_30 |= 2;                          // ori r0,r0,2; stw 0x30(r25)
        void* res = vcall_p(acc, SLOT_VF27C);       // slot 0x27C
        *(u16*)res = 1;                             // li r0,1; sth r0, 0x0(r3)
        break;                                      // b .L_800F4000
    }

    case 235: {
        if (func_80148778((&acc->subObject), 0xEB)) {
            ecVf20(&acc->subObject, 0xEB);           // slot 0x20
            return 0;                               // li r3,0; b .L_800F41D4
        }
        // .L_800F1070
        evt->prevEventType = evt->eventType;          // lhz + sth 0x2e
        evt->field_30 = copyBattleEvent(eventWorkspace.case235Event, *evt) | 2;
        eventWorkspace.case235Event.eventType = 0x32;                    // li r8,0x32; sth 0x73c(r1)
        eventWorkspace.case235Event.field_10 = 0;                        // li r0,0; stw 0x740(r1)
        func_800EC918(self, pc, acc, &eventWorkspace.case235Event, tgt);
        break;                                      // b .L_800F4000
    }

    case 237: {
        evt->prevEventType = evt->eventType;
        evt->field_30 = copyBattleEvent(eventWorkspace.case237Event, *evt) | 2;

        eventWorkspace.case237Event.eventType = 0x58;                 // sth r0(0x58), +0x0C
        func_800EC918(self, pc, acc, &eventWorkspace.case237Event, tgt);

        eventWorkspace.case237Event.eventType = 0x95;                 // li r3,0x95
        eventWorkspace.case237Event.field_10 = 0x78;                  // li r0,0x78
        func_800EC918(self, pc, acc, &eventWorkspace.case237Event, tgt);

        eventWorkspace.case237Event.eventType = 0x3C;                 // li r3,0x3C
        eventWorkspace.case237Event.field_10 = 0x19;                  // li r0,0x19
        func_800EC918(self, pc, acc, &eventWorkspace.case237Event, tgt);
        break; // b .L_800F4000 (shared tail)
    }

    case 238: {
        if (pc == nullptr) {
            break; // cmpwi r23,0; beq .L_800F4000
        }
        evt->prevEventType = evt->eventType;

        // eventWorkspace.case238PlayerEvent (stack 0x6C8): plain copy; field_30 keeps the pre-|2 value.
        copyBattleEventPair(eventWorkspace.case238PlayerEvent,
                            eventWorkspace.case238ActorEvent, *evt, 2);

        // eventWorkspace.case238ActorEvent (stack 0x694): copy with field_30 |= 2, eventType 0x11,
        // field_10 = pc->field_3F10.
        eventWorkspace.case238ActorEvent.eventType = 0x11;                    // li r14,0x11
        eventWorkspace.case238ActorEvent.field_10 = *(s32*)((u8*)pc + 0x3F10);
        if (!func_800EC918(self, pc, acc, &eventWorkspace.case238ActorEvent, tgt)) {
            return 0; // cmpwi r3,0; beq -> li r3,0; b .L_800F41D4
        }

        // acc->field_3ED4 vfunc 0x70 called TWICE with pc->field_3F10
        // (retail has no null check here).
        void* obj3ED4 = *(void**)((u8*)acc + 0x3ED4);
        {
            void* vtbl = *(void**)obj3ED4;
            typedef void (*VF70)(void*, u32);
            VF70 vf70 = (VF70)(*(void**)((u8*)vtbl + 0x70));
            ecVf70(obj3ED4, *(u32*)((u8*)pc + 0x3F10));
            ecVf70(obj3ED4, *(u32*)((u8*)pc + 0x3F10));
        }

        // eventType 0xEE recursion with pc=0 and acc=pc (!).
        eventWorkspace.case238PlayerEvent.eventType = 0xEE;                    // li r0,0xEE
        func_800EC918(self, 0, (EC918_BattleObjAccessor*)pc, &eventWorkspace.case238PlayerEvent, tgt);

        // eventType 0x11 recursion with field_10 = acc->field_3F10,
        // field_30 |= 0x800; acc=pc again.
        eventWorkspace.case238PlayerEvent.eventType = 0x11;                    // sth r14, +0x0C
        eventWorkspace.case238PlayerEvent.field_10 = *(s32*)((u8*)acc + 0x3F10);
        eventWorkspace.case238PlayerEvent.field_30 |= 0x800;                   // ori r0,r0,0x800
        func_800EC918(self, pc, (EC918_BattleObjAccessor*)pc, &eventWorkspace.case238PlayerEvent, tgt);
        return 1; // li r3,1; b .L_800F41D4
    }

    case 253: {
        evt->prevEventType = evt->eventType;
        s16 savedField16 = evt->field_16;         // lha r14, 0x16(r25)
        evt->field_30 = copyBattleEvent(eventWorkspace.case253Event, *evt) | 2;
        eventWorkspace.case253Event.eventType = 0x44;                 // li r0,0x44
        func_800EC918(self, pc, acc, &eventWorkspace.case253Event, tgt);

        // eventType 0x6F with field_10 = pcStat->+0x30 (s16) * 2
        eventWorkspace.case253Event.eventType = 0x6F;                 // li r0,0x6F
        {
            void* vtbl = *(void**)pc;
            typedef void* (*VF224)(void*);
            VF224 vf224 = (VF224)(*(void**)((u8*)vtbl + 0x224));
            void* pcStat = ecVf224(pc);
            eventWorkspace.case253Event.field_10 = (s32)(*(s16*)((u8*)pcStat + 0x30)) * 2; // lha+slwi
        }
        func_800EC918(self, pc, acc, &eventWorkspace.case253Event, tgt);

        // eventType 0x97 with field_10=0x14, field_14=2,
        // field_16=original field_16, field_24=1.0f
        eventWorkspace.case253Event.eventType = 0x97;                 // li r3,0x97
        eventWorkspace.case253Event.field_10 = 0x14;                  // li r6,0x14
        eventWorkspace.case253Event.field_14 = 2;                     // li r0,0x2
        eventWorkspace.case253Event.field_16 = savedField16;          // sth r14, +0x16
        eventWorkspace.case253Event.field_24 = k1_0f;                 // stfs f0(1.0f), +0x24
        func_800EC918(self, pc, acc, &eventWorkspace.case253Event, tgt);
        break; // b .L_800F4000 (shared tail)
    }

    case 243: {
        // f28 = artsData ? artsData->+0x60 : 0.0f   (r16 = artsData)
        // r14 = artsData ? artsData->+0x46 : 0      (arts id)
        f32 f28;
        u32 artsId;
        void* artsData = (tgt != nullptr) ? tgt->artsData : nullptr;
        if (artsData != nullptr) {
            f28 = *(f32*)((u8*)artsData + 0x60);
            artsId = *(u16*)((u8*)artsData + 0x46);
        } else {
            f28 = k0_0f;
            artsId = 0;
        }

        evt->prevEventType = evt->eventType;
        evt->field_30 = copyBattleEvent(eventWorkspace.case243Event, *evt) | 2;
        eventWorkspace.case243Event.eventType = 0x58;                 // li r0,0x58

        // Single enum-list holder reused by all three passes (retail r1+0x58);
        // destructor once at the end.
        EnumListHolder holder;
        func_80043D90(&holder);

        // Pass 1 (0x800F14B4-0x800F15C0): filter (0x20, 0x800) then
        // __ct__800FB044(list, f28, pc->subIdent->vfAC(), 0);
        // apply event 0x58 to every listed actor via recursion.
        {
            void* list = func_80043F18(&holder);
            func_800F4A98(list, 0x20, 0x800);

            void* subVtbl = *(void**)((u8*)pc + 0x3E9C);
            typedef void* (*VFAC)(void*);
            VFAC vfAC = (VFAC)(*(void**)((u8*)subVtbl + 0xAC));
            void* idObj = ecVfAC((u8*)pc + 0x3E9C);

            list = func_80043F18(&holder);
            __ct__800FB044(list, f28, idObj, 0);

            for (u32 i = 0;; i++) {
                void* l = func_80043F18(&holder);
                u32 count = *(u32*)((u8*)l + 0x620);
                if (i >= count) break;
                void* actorAcc = func_8016FE34(func_800F6EAC(func_80043F18(&holder), i));
                func_800EC918(self, pc, (EC918_BattleObjAccessor*)actorAcc,
                              &eventWorkspace.case243Event, tgt);
            }
        }

        // Pass 2 (0x800F15C4-0x800F1610): same list, subAcc->vf20(0x0B) on
        // every actor except the acc itself.
        {
            for (u32 i = 0;; i++) {
                void* l = func_80043F18(&holder);
                u32 count = *(u32*)((u8*)l + 0x620);
                if (i >= count) break;
                void* actorAcc = func_8016FE34(func_800F6EAC(func_80043F18(&holder), i));
                if (actorAcc == acc) continue; // cmplw r3,r24; beq
                void* subVtbl = *(void**)((u8*)actorAcc + 0x08);
                typedef void (*VF20)(void*, u32);
                VF20 vf20 = (VF20)(*(void**)((u8*)subVtbl + 0x20));
                ecVf20((u8*)actorAcc + 0x08, 0x0B);
            }
        }

        // Damage factor: f29 = func_800D7EA0((u8*)pc, tgt).
        f32 f29 = func_800D7EA0((u8*)pc, tgt);

        // Pass 3 (0x800F1624-0x800F1714): refilter (0x100, 0) then
        // __ct__800FB044(list, f28, pc->subIdent->vfAC(), 8);
        // apply scaled arts damage to each listed actor.
        {
            void* list = func_80043F18(&holder);
            func_800F4A98(list, 0x100, 0);

            void* subVtbl = *(void**)((u8*)pc + 0x3E9C);
            typedef void* (*VFAC)(void*);
            VFAC vfAC = (VFAC)(*(void**)((u8*)subVtbl + 0xAC));
            void* idObj = ecVfAC((u8*)pc + 0x3E9C);

            list = func_80043F18(&holder);
            __ct__800FB044(list, f28, idObj, 8);

            for (u32 i = 0;; i++) {
                void* l = func_80043F18(&holder);
                u32 count = *(u32*)((u8*)l + 0x620);
                if (i >= count) break;
                void* actorAcc = func_8016FE34(func_800F6EAC(func_80043F18(&holder), i));
                if (actorAcc == acc) continue; // cmplw r3,r24; beq .L_800F1700

                // skip actors whose subIdent->vf4C() == pc->field_3F10
                {
                    void* subVtbl = *(void**)((u8*)actorAcc + 0x3E9C);
                    typedef u32 (*VF4C)(void*);
                    VF4C vf4C = (VF4C)(*(void**)((u8*)subVtbl + 0x4C));
                    u32 pcId = *(u32*)((u8*)pc + 0x3F10);
                    if (ecVf4C((u8*)actorAcc + 0x3E9C) == pcId) continue; // beq .L_800F1700

                    // level = pc->vf108(); product = artsId * (level + 14)
                    void* pcVtbl = *(void**)pc;
                    typedef s32 (*VF108)(void*);
                    VF108 vf108 = (VF108)(*(void**)((u8*)pcVtbl + 0x108));
                    s32 level = ecVf108(pc);
                    s32 product = (s32)artsId * (level + 14); // mullw r0,r14,r0

                    // actor->vf2C4(pc, 0.0f, f29 * product, 0.0f)
                    void* actorVtbl = *(void**)actorAcc;
                    typedef void (*VF2C4)(void*, void*, f32, f32, f32);
                    VF2C4 vf2C4 = (VF2C4)(*(void**)((u8*)actorVtbl + 0x2C4));
                    ecVf2C4(actorAcc, pc, k0_0f, f29 * (f32)product, k0_0f);
                }
            }
        }
        __dt__80043E88(&holder, -1); // destructor before tail (retail order)
        break; // b .L_800F4000 (shared tail)
    }

    case 254: {
        evt->prevEventType = evt->eventType;
        evt->field_30 = copyBattleEvent(eventWorkspace.case254Event, *evt) | 2;
        eventWorkspace.case254Event.eventType = 0x28;                 // li r0,0x28
        func_800EC918(self, pc, acc, &eventWorkspace.case254Event, tgt);

        eventWorkspace.case254Event.eventType = 0x29;                 // li r0,0x29
        func_800EC918(self, pc, acc, &eventWorkspace.case254Event, tgt);

        eventWorkspace.case254Event.eventType = 0xC6;                 // li r3,0xC6
        eventWorkspace.case254Event.field_10 = 2;                     // li r6,0x2
        eventWorkspace.case254Event.field_14 = 0;                     // li r0,0x0
        eventWorkspace.case254Event.field_24 = k1_0f;                 // stfs f0, +0x24
        func_800EC918(self, pc, acc, &eventWorkspace.case254Event, tgt);
        break; // b .L_800F4000 (shared tail)
    }

    case 244: {
        if (pc == nullptr) {
            break; // cmpwi r23,0; beq .L_800F4000
        }
        evt->prevEventType = evt->eventType;

        copyBattleEventPair(eventWorkspace.case244PlayerEvent,
                            eventWorkspace.case244ActorEvent, *evt, 2);

        eventWorkspace.case244ActorEvent.eventType = 0x11;                    // li r14,0x11
        eventWorkspace.case244ActorEvent.field_10 = *(s32*)((u8*)pc + 0x3F10);
        if (!func_800EC918(self, pc, acc, &eventWorkspace.case244ActorEvent, tgt)) {
            return 0; // li r3,0; b .L_800F41D4
        }

        // acc->field_3ED4 vfunc 0x70 called ONCE with pc->field_3F10.
        void* obj3ED4 = *(void**)((u8*)acc + 0x3ED4);
        {
            void* vtbl = *(void**)obj3ED4;
            typedef void (*VF70)(void*, u32);
            VF70 vf70 = (VF70)(*(void**)((u8*)vtbl + 0x70));
            ecVf70(obj3ED4, *(u32*)((u8*)pc + 0x3F10));
        }

        // eventType 0x60 recursion with pc=0, acc=pc.
        eventWorkspace.case244PlayerEvent.eventType = 0x60;                    // li r3,0x60
        eventWorkspace.case244PlayerEvent.field_10 = 0x19;                     // li r0,0x19
        func_800EC918(self, 0, (EC918_BattleObjAccessor*)pc, &eventWorkspace.case244PlayerEvent, tgt);

        // eventType 0xF4 recursion with pc=0, acc=pc.
        eventWorkspace.case244PlayerEvent.eventType = 0xF4;                    // li r0,0xF4
        func_800EC918(self, 0, (EC918_BattleObjAccessor*)pc, &eventWorkspace.case244PlayerEvent, tgt);

        // eventType 0x11 recursion: field_10 = acc->field_3F10,
        // field_30 |= 0x800, acc=pc.
        eventWorkspace.case244PlayerEvent.eventType = 0x11;                    // sth r14, +0x0C
        eventWorkspace.case244PlayerEvent.field_10 = *(s32*)((u8*)acc + 0x3F10);
        eventWorkspace.case244PlayerEvent.field_30 |= 0x800;                   // ori r0,r0,0x800
        func_800EC918(self, pc, (EC918_BattleObjAccessor*)pc, &eventWorkspace.case244PlayerEvent, tgt);
        return 1; // li r3,1; b .L_800F41D4
    }

    case 239: {
        evt->prevEventType = evt->eventType;
        evt->field_30 = copyBattleEvent(eventWorkspace.case239Event, *evt) | 2;
        eventWorkspace.case239Event.eventType = 0x2D;                 // li r0,0x2D
        func_800EC918(self, pc, acc, &eventWorkspace.case239Event, tgt);

        eventWorkspace.case239Event.eventType = 0xCA;                 // li r0,0xCA
        eventWorkspace.case239Event.field_10 = 0;                     // li r14,0x0
        func_800EC918(self, pc, acc, &eventWorkspace.case239Event, tgt);

        eventWorkspace.case239Event.eventType = 0xCD;                 // li r0,0xCD
        eventWorkspace.case239Event.field_10 = 0;
        func_800EC918(self, pc, acc, &eventWorkspace.case239Event, tgt);
        break; // b .L_800F4000 (shared tail)
    }

    case 240: {
        // Status 0xF1 gate: if the actor has this status, reject the event.
        if (func_80148778((&acc->subObject), 0xF1)) {
            return 0; // li r3,0; b .L_800F41D4
        }

        // amount = 0x23 / ((0x64 - stat[0x83]) / 100.0f), single precision
        s32 amount = 0x23;                       // li r14,0x23
        {
            void* vtbl = *(void**)acc;
            typedef void* (*VF290)(EC918_BattleObjAccessor*);
            VF290 vf290 = (VF290)(*(void**)((u8*)vtbl + 0x290));
            void* statObj = ecVf290(acc);
            if (statObj != nullptr) {
                s32 sv;
                statObj = ecVf290(acc);
                if (func_80260264(statObj, 0x83, &sv)) {
                    f32 num = (f32)amount;
                    f32 den = (f32)(0x64 - sv) / k100_0f; // subfic + fdivs
                    amount = (s32)(num / den);         // fdivs + fctiwz
                }
            }
        }

        evt->prevEventType = evt->eventType;
        evt->field_30 = copyBattleEvent(eventWorkspace.case240Event, *evt) | 2;
        eventWorkspace.case240Event.eventType = 0x94;                 // li r8,0x94
        eventWorkspace.case240Event.field_10 = amount;                // stw r14, +0x10
        eventWorkspace.case240Event.field_14 = 0xF0;                  // li r0,0xF0
        eventWorkspace.case240Event.field_24 = k1_0f;                 // stfs f0, +0x24
        func_800EC918(self, pc, acc, &eventWorkspace.case240Event, tgt);

        eventWorkspace.case240Event.eventType = 0x34;                 // li r3,0x34
        eventWorkspace.case240Event.field_10 = 0;                     // li r14,0x0
        eventWorkspace.case240Event.field_14 = 3;                     // li r0,0x3
        eventWorkspace.case240Event.field_24 = k1_0f;
        func_800EC918(self, pc, acc, &eventWorkspace.case240Event, tgt);

        eventWorkspace.case240Event.eventType = 0x2E;                 // li r0,0x2E
        eventWorkspace.case240Event.field_10 = 0;
        func_800EC918(self, pc, acc, &eventWorkspace.case240Event, tgt);
        break; // b .L_800F4000 (shared tail)
    }

    case 241: {
        if (func_80148778((&acc->subObject), 0xF1)) {
            return 0; // li r3,0; b .L_800F41D4
        }

        // amount = 0x0F / ((0x64 - stat[0x83]) / 100.0f), single precision
        s32 amount = 0x0F;                       // li r14,0x0F
        {
            void* vtbl = *(void**)acc;
            typedef void* (*VF290)(EC918_BattleObjAccessor*);
            VF290 vf290 = (VF290)(*(void**)((u8*)vtbl + 0x290));
            void* statObj = ecVf290(acc);
            if (statObj != nullptr) {
                s32 sv;
                if (func_80260264(statObj, 0x83, &sv)) {
                    f32 num = (f32)0x0F;
                    f32 den = (f32)(0x64 - sv) / k100_0f;
                    amount = (s32)(num / den);
                }
            }
        }

        evt->prevEventType = evt->eventType;
        evt->field_30 = copyBattleEvent(eventWorkspace.case241Event, *evt) | 2;
        eventWorkspace.case241Event.eventType = 0x94;                 // li r3,0x94
        eventWorkspace.case241Event.field_10 = amount;
        eventWorkspace.case241Event.field_14 = 0xF1;                  // li r0,0xF1
        eventWorkspace.case241Event.field_24 = k1_0f;
        func_800EC918(self, pc, acc, &eventWorkspace.case241Event, tgt);

        eventWorkspace.case241Event.eventType = 0x34;                 // li r3,0x34
        eventWorkspace.case241Event.field_10 = 0;
        eventWorkspace.case241Event.field_14 = 3;                     // li r0,0x3
        eventWorkspace.case241Event.field_24 = k1_0f;
        func_800EC918(self, pc, acc, &eventWorkspace.case241Event, tgt);

        eventWorkspace.case241Event.eventType = 0x2E;                 // li r0,0x2E
        eventWorkspace.case241Event.field_10 = 0;
        func_800EC918(self, pc, acc, &eventWorkspace.case241Event, tgt);

        eventWorkspace.case241Event.eventType = 0xCB;                 // li r0,0xCB
        func_800EC918(self, pc, acc, &eventWorkspace.case241Event, tgt);

        eventWorkspace.case241Event.eventType = 0xCC;                 // li r0,0xCC
        func_800EC918(self, pc, acc, &eventWorkspace.case241Event, tgt);
        break; // b .L_800F4000 (shared tail)
    }

    case 242: {
        evt->prevEventType = evt->eventType;
        evt->field_30 = copyBattleEvent(eventWorkspace.case242Event, *evt) | 2;
        eventWorkspace.case242Event.eventType = 0xD4;                 // li r0,0xD4
        func_800EC918(self, pc, acc, &eventWorkspace.case242Event, tgt);

        eventWorkspace.case242Event.eventType = 0xC7;                 // li r3,0xC7
        eventWorkspace.case242Event.field_10 = 0;                     // li r0,0x0
        func_800EC918(self, pc, acc, &eventWorkspace.case242Event, tgt);
        break; // b .L_800F4000 (shared tail)
    }

    case 246: {
        // rnd = mtRand(evt->field_14, evt->field_10)  [lha +0x14 / lwz +0x10]
        s32 rnd = mtRand__Q22ml4mathFii(evt->field_14, evt->field_10);

        // acc->vf154((f32)(-rnd));  pc->vf154((f32)rnd);
        {
            void* vtbl = *(void**)acc;
            typedef void (*VF154)(void*, f32);
            VF154 vf154 = (VF154)(*(void**)((u8*)vtbl + 0x154));
            ecVf154(acc, (f32)(-rnd));
        }
        {
            void* vtbl = *(void**)pc;
            typedef void (*VF154)(void*, f32);
            VF154 vf154 = (VF154)(*(void**)((u8*)vtbl + 0x154));
            ecVf154(pc, (f32)rnd);
        }

        // cap: if (pc->vf158() > 99.0f) pc->vf150(99.0f);
        {
            void* vtbl = *(void**)pc;
            typedef f32 (*VF158)(void*);
            VF158 vf158 = (VF158)(*(void**)((u8*)vtbl + 0x158));
            if (ecVf158(pc) > k99_0f) {
                typedef void (*VF150)(void*, f32);
                VF150 vf150 = (VF150)(*(void**)((u8*)vtbl + 0x150));
                ecVf150(pc, k99_0f);
            }
        }

        evt->prevEventType = evt->eventType;
        evt->field_20 = (f32)rnd;                 // stfs f0, 0x20(r25)
        evt->field_30 = copyBattleEvent(eventWorkspace.case246Event, *evt) | 2;
        eventWorkspace.case246Event.eventType = 0xC3;                 // li r0,0xC3
        eventWorkspace.case246Event.field_10 = 0;                     // li r14,0x0
        func_800EC918(self, pc, acc, &eventWorkspace.case246Event, tgt);

        eventWorkspace.case246Event.eventType = 0x120;                // li r0,0x120
        eventWorkspace.case246Event.field_10 = 0;
        func_800EC918(self, pc, acc, &eventWorkspace.case246Event, tgt);
        break; // b .L_800F4000 (shared tail)
    }

    case 247: {
        evt->field_10 = 0;                        // li r0,0; stw r0, 0x10(r25)
        break; // b .L_800F4000 (shared tail)
    }

    case 274: {
        if (func_80148778((&acc->subObject), 0x117)) {
            return 0; // li r3,0; b .L_800F41D4
        }
        evt->prevEventType = evt->eventType;
        evt->field_30 = copyBattleEvent(eventWorkspace.case274Event, *evt) | 2;
        eventWorkspace.case274Event.eventType = 0x58;                 // li r8,0x58
        eventWorkspace.case274Event.field_10 = 0x19;                  // li r0,0x19
        func_800EC918(self, pc, acc, &eventWorkspace.case274Event, tgt);

        eventWorkspace.case274Event.eventType = 0x02;                 // li r3,0x2
        eventWorkspace.case274Event.field_10 = 0x32;                  // li r0,0x32
        func_800EC918(self, pc, acc, &eventWorkspace.case274Event, tgt);

        eventWorkspace.case274Event.eventType = 0x3B;                 // li r0,0x3B
        eventWorkspace.case274Event.field_10 = 0;                     // li r14,0x0
        func_800EC918(self, pc, acc, &eventWorkspace.case274Event, tgt);

        eventWorkspace.case274Event.eventType = 0x3A;                 // li r0,0x3A
        eventWorkspace.case274Event.field_10 = 0;
        func_800EC918(self, pc, acc, &eventWorkspace.case274Event, tgt);

        eventWorkspace.case274Event.eventType = 0x15;                 // li r3,0x15
        eventWorkspace.case274Event.field_10 = 0x64;                  // li r0,0x64
        func_800EC918(self, pc, acc, &eventWorkspace.case274Event, tgt);

        // acc->vf150(0.0f)
        {
            void* vtbl = *(void**)acc;
            typedef void (*VF150)(void*, f32);
            VF150 vf150 = (VF150)(*(void**)((u8*)vtbl + 0x150));
            ecVf150(acc, k0_0f);
        }

        // Target the actor owning the accessory/unit id; if unavailable,
        // pick a random actor from the party list.
        {
            void* subVtbl = *(void**)((u8*)acc + 0x3E9C);
            typedef u32 (*VF4C)(void*);
            VF4C vf4C = (VF4C)(*(void**)((u8*)subVtbl + 0x4C));
            void* obj = func_800B708C(ecVf4C((u8*)acc + 0x3E9C));
            void* acc = func_8016FE34(obj);
            if (acc != nullptr) {
                void* accVtbl = *(void**)acc;
                typedef s32 (*VF2BC)(void*);
                VF2BC vf2BC = (VF2BC)(*(void**)((u8*)accVtbl + 0x2BC));
                if (ecVf2BC(acc) == 0) {
                    evt->field_10 = *(u32*)((u8*)acc + 0x3F10);
                    break; // b .L_800F4000 (shared tail)
                }
            }
        }

        // fallback: random party member from enum list
        {
            EnumListHolder holder;
            func_80043D90(&holder);
            void* list = func_80043F18(&holder);
            func_800F4A98(list, 0x20, 0x800);

            void* l = func_80043F18(&holder);
            s32 count = *(s32*)((u8*)l + 0x620);
            if (count > 0) {
                s32 idx = rand() % count;        // divwu/mullw/subf
                void* item = func_800F6EAC(func_80043F18(&holder), (u32)idx);
                evt->field_10 = *(u32*)((u8*)item + 0x74);
            }
            __dt__80043E88(&holder, -1);
        }
        break; // b .L_800F4000 (shared tail)
    }

    case 273: {
        if (func_80148778((&acc->subObject), 0x117)) {
            return 0; // li r3,0; b .L_800F41D4
        }
        evt->prevEventType = evt->eventType;
        evt->field_30 |= 2;

        // level = acc->vf108(); pick a count from level bands.
        {
            void* vtbl = *(void**)acc;
            typedef s32 (*VF108)(void*);
            VF108 vf108 = (VF108)(*(void**)((u8*)vtbl + 0x108));
            s32 level = ecVf108(acc);
            s32 count;
            if (level < 0x0A) {
                count = 0;
            } else if (level < 0x14) {
                count = rand() % 2 + 1;
            } else if (level < 0x1E) {
                count = rand() % 2 + 2;
            } else if (level < 0x28) {
                count = rand() % 2 + 2;
            } else if (level < 0x32) {
                count = rand() % 2 + 2;
            } else if (level < 0x3C) {
                count = rand() % 3 + 2;
            } else if (level < 0x46) {
                count = rand() % 3 + 2;
            } else if (level < 0x50) {
                count = rand() % 3 + 2;
            } else if (level < 0x5A) {
                count = rand() % 3 + 2;
            } else {
                s32 randomValue = rand();
                count = randomValue % 3;
                count += 2;
            }

            if (count != 0) {
                // if (acc->vfE0() == 5) --count;
                {
                    void* v2 = *(void**)acc;
                    typedef s32 (*VFE0)(void*);
                    VFE0 vfE0 = (VFE0)(*(void**)((u8*)v2 + 0xE0));
                    if (ecVfE0(acc) == 5) {
                        count--;
                    }
                }

                func_8003AA34();
                void* file = getFP__FPCc((const char*)((u8*)acc + 0x3F14));
                u16 colId = *(u16*)((u8*)acc + 0x3F28);

                // The BDAT columns are 16-bit growth values.
                u16 c0 = getBdatStringColumnValue(file, "Lv_up_str", colId);
                u16 c1 = getBdatStringColumnValue(file, "Lv_up_agi", colId);
                u16 c2 = getBdatStringColumnValue(file, "Lv_up_eth", colId);

                evt->field_10 = count;
                evt->field_14 = (s16)(count * c0);  // mullw + sth
                evt->field_16 = (s16)(count * c1);
                evt->field_18 = (s16)(count * c2);
            }
        }

        // acc->vf150(0.0f)
        {
            void* vtbl = *(void**)acc;
            typedef void (*VF150)(void*, f32);
            VF150 vf150 = (VF150)(*(void**)((u8*)vtbl + 0x150));
            ecVf150(acc, k0_0f);
        }
        break; // b .L_800F4000 (shared tail)
    }

    case 275: {
        if (func_80148778((&acc->subObject), 0x117)) {
            return 0; // li r3,0; b .L_800F41D4
        }
        evt->prevEventType = evt->eventType;
        evt->field_30 = copyBattleEvent(eventWorkspace.case275Event, *evt) | 2;
        eventWorkspace.case275Event.eventType = 0x34;                 // li r9,0x34
        eventWorkspace.case275Event.field_10 = 0;                     // li r8,0x0
        eventWorkspace.case275Event.field_14 = 4;                     // li r0,0x4
        eventWorkspace.case275Event.field_24 = k2_0f;                 // stfs f0(2.0f), +0x24
        func_800EC918(self, pc, acc, &eventWorkspace.case275Event, tgt);
        break; // b .L_800F4000 (shared tail)
    }

    case 276: {
        if (func_80148778((&acc->subObject), 0x117)) {
            return 0; // li r3,0; b .L_800F41D4
        }
        break; // beq .L_800F4000 (shared tail)
    }

    case 277: {
        if (func_80148778((&acc->subObject), 0x117)) {
            return 0; // li r3,0; b .L_800F41D4
        }
        evt->prevEventType = evt->eventType;
        evt->field_30 |= 2;
        break; // b .L_800F4000 (shared tail)
    }

    case 278: {
        if (func_80148778((&acc->subObject), 0x117)) {
            return 0; // li r3,0; b .L_800F41D4
        }
        break; // beq .L_800F4000 (shared tail)
    }

    case 236: {
        evt->prevEventType = evt->eventType;
        evt->field_30 = copyBattleEvent(eventWorkspace.case236Event, *evt) | 2;
        eventWorkspace.case236Event.eventType = 0x44;
        eventWorkspace.case236Event.field_30 |= 0x800;
        func_800EC918(self, pc, acc, &eventWorkspace.case236Event, tgt);

        eventWorkspace.case236Event.eventType = 0x45;
        func_800EC918(self, pc, acc, &eventWorkspace.case236Event, tgt);
        break; // b .L_800F4000 (shared tail)
    }

    case 281: {
        func_8018C820((void*)((u8*)self + 0x194), evt->field_10);
        break; // b .L_800F4000 (shared tail)
    }

    case 255: {
        return 1; // li r3,1; b .L_800F41D4
    }

    case 193: {
        evt->field_20 = k5_0f;                    // stfs f0(5.0f), 0x20(r25)
        break; // b .L_800F4000 (shared tail)
    }

    case 153: {
        // acc->vf154((f32)evt->field_10)
        {
            void* vtbl = *(void**)acc;
            typedef void (*VF154)(void*, f32);
            VF154 vf154 = (VF154)(*(void**)((u8*)vtbl + 0x154));
            ecVf154(acc, (f32)evt->field_10);
        }
        break; // b .L_800F4000 (shared tail)
    }

    case 257: {
        evt->prevEventType = evt->eventType;
        evt->field_30 = copyBattleEvent(eventWorkspace.case257Event, *evt) | 2;
        eventWorkspace.case257Event.eventType = 0xD4;                 // li r8,0xD4
        eventWorkspace.case257Event.field_10 = 0;                     // li r0,0x0
        func_800EC918(self, pc, acc, &eventWorkspace.case257Event, tgt);

        eventWorkspace.case257Event.eventType = 0x62;                 // li r3,0x62
        eventWorkspace.case257Event.field_10 = 0x64;                  // li r0,0x64
        func_800EC918(self, pc, acc, &eventWorkspace.case257Event, tgt);
        break; // b .L_800F4000 (shared tail)
    }

    case 258: {
        evt->prevEventType = evt->eventType;
        evt->field_30 = copyBattleEvent(eventWorkspace.case258Event, *evt) | 2;
        eventWorkspace.case258Event.eventType = 0x02;                 // li r0,0x2
        func_800EC918(self, pc, acc, &eventWorkspace.case258Event, tgt);

        // sweep slots 0..31: purge events that are not class-0x45C00 and
        // whose field_08 lacks the 0x1C00 bits.
        for (s32 i = 0; i < 0x20; i++) {
            void* subVtbl = *(void**)((u8*)acc + 0x08);
            typedef void* (*VF5C)(void*, u32);
            VF5C vf5C = (VF5C)(*(void**)((u8*)subVtbl + 0x5C));
            void* r = ecVf5C((u8*)acc + 0x08, (u32)i);

            if (func_80145C00(*(u16*)((u8*)r + 0x2E))) continue; // bne
            if (*(u32*)((u8*)r + 0x08) & 0x1C00) continue;       // rlwinm 17-19

            typedef void (*VF24)(void*, void*);
            VF24 vf24 = (VF24)(*(void**)((u8*)subVtbl + 0x24));
            ecVf24((u8*)acc + 0x08, r);
        }
        break; // b .L_800F4000 (shared tail)
    }

    case 259: {
        if (pc == nullptr) {
            return 0; // beq .L_800F29CC
        }
        if (*(u32*)((u8*)pc + 0x1530) == 0) {
            return 0; // lwz r0, 0x1530(r23); cmpwi r0,0; beq .L_800F29CC
        }
        copyBattleEvent(eventWorkspace.case259Event, *evt);                            // (no prevEventType, no |2)
        eventWorkspace.case259Event.eventType = 0x0F;                 // li r8,0x0F
        eventWorkspace.case259Event.field_10 = 0x03;                  // li r0,0x3
        func_800EC918(self, pc, acc, &eventWorkspace.case259Event, tgt);
        return 1; // .L_800F29D4: li r3,1; b .L_800F41D4
    }

    case 283: {
        EnumListHolder actorList;
        func_80043D90(&actorList);                           // walker ctor @stack 0x48
        func_800F4A98(func_80043F18(&actorList), 0x80000000, 0);  // lis r4,0x8000 ; li r5,0
        for (s32 i = 0; i < *(s32*)((u8*)func_80043F18(&actorList) + 0x620); i++) {
            void* actor = func_8016FE34(func_800F6EAC(func_80043F18(&actorList), i));
            void* result = vcall_p1(actor, SLOT_VF2D4, *(u32*)((u8*)acc + 0x3F10));
            if (result != nullptr) {
                // Each vector component is evaluated independently in the original.
                vcall_v3f(actor, SLOT_VF2C4, pc,
                          *(f32*)((u8*)result + 0x10) * ((f32)(s32)evt->field_10 / 100.0f),
                          *(f32*)((u8*)result + 0x00) * ((f32)(s32)evt->field_10 / 100.0f),
                          *(f32*)((u8*)result + 0x04) * ((f32)(s32)evt->field_10 / 100.0f));
                *(f32*)((u8*)result + 0x10) *=
                    1.0f - ((f32)(s32)evt->field_10 / 100.0f);
                *(f32*)((u8*)result + 0x00) *=
                    1.0f - ((f32)(s32)evt->field_10 / 100.0f);
                *(f32*)((u8*)result + 0x04) *=
                    1.0f - ((f32)(s32)evt->field_10 / 100.0f);
            }
        }
        __dt__80043E88(&actorList, -1);                      // walker dtor
        break;                                         // b .L_800F4000
    }

    case 284: {
        if (acc->field_1530 != 0) {               // lwz r4,0x1530 ; beq tail
            void* entry = func_80149154((&acc->subObject), acc->field_1530);
            u16 prevType = *(u16*)((u8*)entry + 0x2E); // lhz r15, 0x2e
            if (prevType != 0) {
                for (s32 i = 0; i < 0x68; i++) {      // cmpwi 0x68
                    void* item = ecVf54(&acc->subObject, i);
                    if (*(u16*)((u8*)item + 0x2E) == prevType) {
                        if (*(f32*)((u8*)item + 0x20) > 0.0f) {   // fcmpo vs 0.0
                            *(f32*)((u8*)item + 0x20) += evt->field_20;
                            if (*(u16*)((u8*)item + 0x0C) == 0x11) {  // cmplwi 0x11
                                void* actor = func_8016FE34(
                                    func_800B708C(*(s32*)((u8*)item + 0x10)));
                                if (actor != nullptr &&
                                    func_80148778((u8*)actor + 8, 0x11)) {
                                    void* entry2 = func_80149154((u8*)actor + 8, 0x11);
                                    if (entry2 != nullptr) {
                                        *(f32*)((u8*)entry2 + 0x20) += evt->field_20;
                                    }
                                }
                            }
                        }
                    }
                }
            } else {
                if (*(f32*)((u8*)entry + 0x20) > 0.0f) {
                    *(f32*)((u8*)entry + 0x20) += evt->field_20;
                }
            }
            // actor-list walker, filter (0x20, 0x800)
            EnumListHolder actorList;
            func_80043D90(&actorList);
            func_800F4A98(func_80043F18(&actorList), 0x20, 0x800);
            for (s32 i = 0; i < *(s32*)((u8*)func_80043F18(&actorList) + 0x620); i++) {
                void* actor = func_8016FE34(func_800F6EAC(func_80043F18(&actorList), i));
                if (actor != nullptr && actor != acc) {   // cmplw r3,r24 ; beq
                    SubAccessor* sub = (SubAccessor*)((u8*)actor + 8);
                    for (s32 j = 0; j < 0x20; j++) {  // vfunc 0x58 list
                        void* item = ecVf58(sub, j);
                        if (*(u16*)((u8*)item + 0x2E) == prevType) {
                            // Reacquire the matching entry before mutating it.
                            item = ecVf58(sub, j);
                            *(f32*)((u8*)item + 0x20) += evt->field_20;
                        }
                    }
                    for (s32 j = 0; j < 0x20; j++) {  // vfunc 0x60 list
                        void* item = ecVf60(sub, j);
                        if (*(u16*)((u8*)item + 0x2E) == prevType) {
                            item = ecVf60(sub, j);
                            *(f32*)((u8*)item + 0x20) += evt->field_20;
                        }
                    }
                }
            }
            __dt__80043E88(&actorList, -1);
        }
        break;                                         // b .L_800F4000
    }

    case 285: {
        vcall_v1(acc, SLOT_VF2F8, (u32)evt->field_10);
        break;                                         // b .L_800F4000
    }

    case 249: {
        evt->prevEventType = evt->eventType;      // sth r4, 0x2e(r25)
        evt->field_30 = copyBattleEvent(eventWorkspace.case249Event, *evt) | 2;
        if (pc == acc) {                   // cmplw r23,r24 ; bne .L_800F2E58
            const s32 eventTypes[5] = { 0x58, 0x59, 0x5A, 0x5B, 0x5C };
            eventWorkspace.case249Event.eventType = eventTypes[mtRand__Q22ml4mathFi(5)];
            func_800EC918(self, pc, acc, &eventWorkspace.case249Event, tgt);
        }
        // .L_800F2E58
        eventWorkspace.case249Event.eventType = 0xBF;
        eventWorkspace.case249Event.field_10 = 0;
        func_800EC918(self, pc, acc, &eventWorkspace.case249Event, tgt);
        if (pc == acc) {
            break;                              // beq .L_800F4000 -> shared tail
        }
        return 1;                               // li r3,1 ; b .L_800F41D4
    }

    case 260: {
        s32 roll = mtRand__Q22ml4mathFi(100);          // r16
        s32 chosen = 0;                                // r15
        s32 i = 3;                                     // r17 start index
        u32 f15F0 = acc->field_15F0;              // lwz r0,0x15f0(r24)
        if ((f15F0 == 0 || f15F0 == 2) && *(u32*)(lbl_eu_80573EEC + 0xD0) < 4) {
            i = 0;                                     // blt .L_800F2F04
        }
        s32 diff = vcall_i(acc, SLOT_VF108) - vcall_i(pc, SLOT_VF108);  // subf r0,r3,r14
        if (diff > 10) i = 5;                          // cmpwi 0xa ; ble skip
        const s32 weights[6] = { 2, 5, 40, 25, 25, 100 };
        for (; i < 6; i++) {                           // mtctr(6-i) ; bdnz
            roll -= weights[i];                        // subf. r16,r0,r16
            if (roll < 0) { chosen = i; break; }       // bge next ; mr r15,r17
        }
        s32 clearedValue = 0;
        u32 inheritedFlag = 0x10000;
        s32 r27 = 0;                                   // selected arts id
        s32 r28 = -1;                                  // selected chain result
        s32 r29 = 0;                                   // selected chain level
    reselect:                                          // .L_800F2FAC
        switch (chosen) {
        case 0: r29 = 3; r27 = 0; break;               // .L_800F2FEC
        case 1: r29 = 2; r27 = 1; break;               // .L_800F2FF8
        case 2: r29 = 1; r27 = 2; break;               // .L_800F3004
        case 3: {                                      // .L_800F3010 arts radar
            if (pc != nullptr) {
                void* statObj = vcall_p(pc, SLOT_VF290);
                if (statObj != nullptr) {
                    statObj = vcall_p(pc, SLOT_VF290);      // retail calls vf290 twice
                    if (func_8026178C(statObj, 0x78)) {
                        for (s32 n = 1; n <= 8; n++) { // li r16,1 ; ble 8
                            void* obj = func_8009EC9C((u16)n);
                            func_800A26A4(obj, (u32)vcall_i(acc, SLOT_VF1E8) >> 1, 0,
                                          vcall_i(acc, SLOT_VF108), 0, 1, 0);
                        }
                        r28 = 1; r27 = 3;
                        break;
                    }
                }
            }
            chosen = 5;                                // .L_800F30CC: li r15,5
            goto reselect;                             // b .L_800F2FAC
        }
        case 4: {                                      // .L_800F30D4 arts radar (2)
            if (pc != nullptr) {
                void* statObj = vcall_p(pc, SLOT_VF290);
                if (statObj != nullptr) {
                    statObj = vcall_p(pc, SLOT_VF290);
                    if (func_8026178C(statObj, 0x79)) {
                        for (s32 n = 1; n <= 8; n++) {
                            void* obj = func_8009EC9C((u16)n);
                            func_800A26A4(obj, 0, (void*)(uintptr_t)((u32)vcall_i(acc, SLOT_VF200) >> 1),
                                          vcall_i(acc, SLOT_VF108), 0, 1, 0);
                        }
                        r28 = 2; r27 = 4;
                        break;
                    }
                }
            }
            chosen = 5;                                // .L_800F3190: li r15,5
            goto reselect;
        }
        case 5: {                                      // .L_800F3198
            // ---- build eventWorkspace.case260ActorEvent (stack 0x2EC) and eventWorkspace.case260PlayerEvent (stack 0x2B8) ----
            copyBattleEvent(eventWorkspace.case260ActorEvent, *evt);
            eventWorkspace.case260ActorEvent.eventType = clearedValue;              // sth r30(0), 0x2f8
            eventWorkspace.case260ActorEvent.field_10 = clearedValue;               // stw r30(0), 0x2fc
            eventWorkspace.case260ActorEvent.field_20 = 0.0f;                       // stfs f29(0.0), 0x30c
            eventWorkspace.case260ActorEvent.field_08 = inheritedFlag;              // stw r31(0x10000), 0x2f4
            eventWorkspace.case260ActorEvent.field_30 |= 0x500;                     // stw r6(evt->30|0x500), 0x31c
            eventWorkspace.case260PlayerEvent.field_30 =
                copyBattleEvent(eventWorkspace.case260PlayerEvent,
                                eventWorkspace.case260ActorEvent) | 0x4;
            func_800BFC68(getObjectMove(pc));
            switch (rand() % 4) {                      // bl rand ; %4 idiom
            case 0: {                                  // .L_800F32A0 knockback
                s32 k = (s32)(0.5f * vcall_f(pc, SLOT_VF12C));  // f30 = 0.5 (80666DE8)
                vcall_d1(acc, SLOT_VF11C, (f64)(s32)(-k));  // neg r0,r15
                f32 d = func_800D81A8(pc, pc, tgt);
                vcall_d1(acc, SLOT_VF11C, (f64)(s32)((s32)((f32)(s32)k * d)));
                r28 = 3; r27 = 8;
                break;
            }
            case 1: {                                  // .L_800F334C
                eventWorkspace.case260ActorEvent.eventType = 0x52;                  // r17 = 0x52
                eventWorkspace.case260ActorEvent.field_10 = 10;                     // r18 = 0xa
                eventWorkspace.case260PlayerEvent.eventType = 0x58;                  // r19 = 0x58
                eventWorkspace.case260PlayerEvent.field_18 = 50;                     // r21 = 0x32
                {
                    void* accArts = vcall_p(acc, SLOT_VF20C);
                    s32 v1 = *(s16*)((u8*)accArts + 0x1C);
                    f32 t = 0.1f * (f32)(s32)v1;       // f28 = 0.1 (80666E84)
                    s32 pct = (s32)((f64)t + (t > 0.0f ? 0.5 : -0.5));  // E58/E60
                    void* pcArts = vcall_p(pc, SLOT_VF20C);
                    s32 v2 = *(s16*)((u8*)pcArts + 0x1C);
                    eventWorkspace.case260PlayerEvent.field_10 = pct * 100 / v2;     // mulli 0x64 ; divw
                }
                r28 = 4; r27 = 5;
                break;
            }
            case 2: {                                  // .L_800F33DC
                eventWorkspace.case260ActorEvent.eventType = 0x53;                  // r20 = 0x53
                eventWorkspace.case260ActorEvent.field_10 = 10;
                eventWorkspace.case260PlayerEvent.eventType = 0x59;                  // r14 = 0x59
                eventWorkspace.case260PlayerEvent.field_18 = 50;
                {
                    void* accArts = vcall_p(acc, SLOT_VF20C);
                    s32 v1 = *(s16*)((u8*)accArts + 0x1E);
                    f32 t = 0.1f * (f32)(s32)v1;
                    s32 pct = (s32)((f64)t + (t > 0.0f ? 0.5 : -0.5));
                    void* pcArts = vcall_p(pc, SLOT_VF20C);
                    s32 v2 = *(s16*)((u8*)pcArts + 0x1E);
                    eventWorkspace.case260PlayerEvent.field_10 = pct * 100 / v2;
                }
                r28 = 5; r27 = 7;
                break;
            }
            case 3: {                                  // .L_800F346C
                eventWorkspace.case260ActorEvent.eventType = 0x54;
                eventWorkspace.case260ActorEvent.field_10 = 10;
                eventWorkspace.case260PlayerEvent.eventType = 0x5A;
                eventWorkspace.case260PlayerEvent.field_18 = 50;
                {
                    void* accArts = vcall_p(acc, SLOT_VF20C);
                    s32 v1 = *(s16*)((u8*)accArts + 0x20);
                    f32 t = 0.1f * (f32)(s32)v1;
                    s32 pct = (s32)((f64)t + (t > 0.0f ? 0.5 : -0.5));
                    void* pcArts = vcall_p(pc, SLOT_VF20C);
                    s32 v2 = *(s16*)((u8*)pcArts + 0x20);
                    eventWorkspace.case260PlayerEvent.field_10 = pct * 100 / v2;
                }
                r28 = 6; r27 = 6;
                break;
            }
            }
            // .L_800F3500
            if (eventWorkspace.case260ActorEvent.eventType != 0) {                  // lhz r0,0x2f8 ; beq .L_800F355C
                if (!func_800EC918(self, pc, acc, &eventWorkspace.case260ActorEvent, tgt)) return 0;
                if (!func_800EC918(self, pc, (EC918_BattleObjAccessor*)pc, &eventWorkspace.case260PlayerEvent, tgt)) return 0;  // acc=pc
            }
            break;
        }
        }
        // .L_800F355C
        if (r29 != 0) {
            if (!func_8015B130(r29, acc->field_3F28)) {
                chosen = 5;                            // li r15,5 ; b .L_800F2FAC
                goto reselect;
            }
            r28 = 0;
        }
        func_80135380((u8)r28);
        func_802A30DC(pc, r27);
        return 1;                                      // li r3,1 ; b .L_800F41D4
    }

    case 261: {
        evt->prevEventType = evt->eventType;
        evt->field_30 |= 0x2;
        if (func_80148778((&acc->subObject), 0x112)) {
            void* artsData = *(void**)((u8*)tgt + 0x50);
            s32 artsVal;
            {
                void* artsVtbl = *(void**)((u8*)artsData + 0x84);
                artsVal = ((s32(*)(void*))(*(void**)((u8*)artsVtbl + 0x0C)))(artsData);
            }
            s32 rate = artsVal * 5 + 50;               // slwi 2 ; add ; addi 0x32
            if (vcall_i(pc, SLOT_VF308) == 1) rate -= 25;   // subi r14,r14,0x19
            if (vcall_i(pc, SLOT_VF308) == 0) rate = 0;     // li r14,0
            if (mtRand__Q22ml4mathFi(100) < rate) {    // cmpw ; bge skip
                ecVf20(&acc->subObject, 0x112);         // vfunc 0x20: clear status
            }
        }
        copyBattleEvent(eventWorkspace.case261Event, *evt);
        eventWorkspace.case261Event.eventType = 7;                             // sth r0(7), 0x290
        eventWorkspace.case261Event.field_10 = *(s32*)((u8*)pc + 0x3F10);      // stw pc->3F10, 0x294
        if (!func_800EC918(self, pc, acc, &eventWorkspace.case261Event, tgt)) return 0;
        break;                                         // b .L_800F4000
    }

    case 264: {
        for (s32 i = 0; i < 0x20; i++) {
            void* item = ecVf5C(&acc->subObject, i);
            if (!func_80145C00(*(u16*)((u8*)item + 0x2E))) {
                if (!(*(u32*)((u8*)item + 0x08) & 0xE000)) {   // rlwinm 0,17,19
                    ecVf24(&acc->subObject, item);
                }
            }
        }
        break;                                         // b .L_800F4000
    }

    case 287: {
        s32 artsNo = func_801B1C5C();                  // bl func_801B1C5C
        if (artsNo < 0) return 0;                      // bge .L_800F3768 ; li r3,0
        if (mtRand__Q22ml4mathFi(100) < evt->field_10) {
            func_801B1C5C();                           // bl again; return ignored
        }
        const s32 artsNumbers[6] = { 6, 4, 7, 8, 9, 5 };
        const s32 eventTypes[6] = { 0x125, 0x126, 0x127, 0x128, 0x129, 0x12A };
        memset(&eventWorkspace.case287Event, 0, sizeof(eventWorkspace.case287Event));                  // memset(0x250, 0, 0x34)
        eventWorkspace.case287Event.field_00 = *(u32*)((u8*)pc + 0x3F10);      // stw pc->3F10, 0x250
        for (s32 i = 0; i < 6; i++) {                  // mtctr 6 ; bdnz
            if (artsNo != artsNumbers[i]) continue;           // cmpw r14,r0 ; bne .L_800F38EC
            if (func_80148778((u8*)pc + 8, eventTypes[i])) break;   // bne -> return 1
            for (s32 idx = 0; idx <= 1; idx++) {       // r17=0 ; cmpwi 1 ; ble
                for (s32 slot = 0; slot < 8; slot++) { // r18=0 ; cmpwi 8 ; blt
                    void* artsParam = getArtsParamRC2(vcall_p(pc, SLOT_VF27C), idx, slot);
                    if (*(u16*)((u8*)artsParam + 0x3C) == 4 &&
                        *(u16*)((u8*)artsParam + 0x40) == (u16)artsNo) {
                        eventWorkspace.case287Event.eventType = eventTypes[i];       // lwzx ; sth 0x25c
                        {
                            void* artsVtbl = *(void**)((u8*)artsParam + 0x84);
                            s32 vf = ((s32(*)(void*))(*(void**)((u8*)artsVtbl + 0x0C)))(artsParam);
                            eventWorkspace.case287Event.field_10 = (s32)*(s16*)((u8*)artsParam + 0x4A)
                                         + (s32)*(u8*)((u8*)artsParam + 0x6F) * (vf - 1);
                        }
                        eventWorkspace.case287Event.field_14 = *(s16*)((u8*)artsParam + 0x4C);   // sth 0x264
                        func_800EC918((void*)(uintptr_t)lbl_eu_80663F00, pc, acc, &eventWorkspace.case287Event, nullptr);
                        break;                         // b .L_800F38DC (outer cont.)
                    }
                }
            }
            break;                                     // b .L_800F38F8 (return 1)
        }
        return 1;                                      // li r3,1 ; b .L_800F41D4
    }

    case 250: {
        for (s32 i = 0; i < 0x20; i++) {        // li r14,0 ; cmpwi 0x20 ; blt
            void* item = ecVf5C(&acc->subObject, i);
            if (!func_80145C00(*(u16*)((u8*)item + 0x2E))) {
                if (!(*(u32*)((u8*)item + 0x08) & 0xE000)) {   // rlwinm 0,17,19
                    ecVf24(&acc->subObject, item);
                }
            }
        }
        evt->prevEventType = evt->eventType;      // lhz r3,0xc(r25) ; sth r3,0x2e(r25)
        evt->field_30 = copyBattleEvent(eventWorkspace.case250Event, *evt) | 2;
        eventWorkspace.case250Event.eventType = 0x34;                   // sth r0(0x34), 0x228(r1)
        func_800EC918(self, pc, acc, &eventWorkspace.case250Event, tgt);
        break;                                  // b .L_800F4000
    }

    case 251: {
        evt->prevEventType = evt->eventType;
        evt->field_30 = copyBattleEvent(eventWorkspace.case251Event, *evt) | 2;
        eventWorkspace.case251Event.eventType = 2;
        func_800EC918(self, pc, acc, &eventWorkspace.case251Event, tgt);
        eventWorkspace.case251Event.eventType = 0x60;
        eventWorkspace.case251Event.field_10 = 25;                      // li r14,0x19 ; stw r14,0x1f8
        func_800EC918(self, pc, acc, &eventWorkspace.case251Event, tgt);
        eventWorkspace.case251Event.eventType = 0x3C;
        eventWorkspace.case251Event.field_10 = 25;
        func_800EC918(self, pc, acc, &eventWorkspace.case251Event, tgt);
        break;                                  // b .L_800F4000
    }

    case 267: {
        evt->prevEventType = evt->eventType;
        copyBattleEventPair(eventWorkspace.case267PrimaryEvent,
                            eventWorkspace.case267SecondaryEvent, *evt, 0);
        evt->field_30 |= 0x2;
        eventWorkspace.case267SecondaryEvent.eventType = 0x11;                          // sth r14(0x11), 0x18c
        eventWorkspace.case267SecondaryEvent.field_10 = *(s32*)((u8*)pc + 0x3F10);      // stw pc->3F10, 0x190
        if (!func_800EC918(self, pc, acc, &eventWorkspace.case267SecondaryEvent, tgt)) return 0;
        // .L_800F3BC0
        vcall_v1(acc->field_3ED4, 0x70, *(u32*)((u8*)pc + 0x3F10));
        eventWorkspace.case267PrimaryEvent.eventType = 0x62;                          // sth r3(0x62), 0x1c0
        eventWorkspace.case267PrimaryEvent.field_10 = 0x32;                           // stw r0(0x32), 0x1c4
        func_800EC918(self, pc, (EC918_BattleObjAccessor*)pc, &eventWorkspace.case267PrimaryEvent, tgt);     // acc arg = pc
        eventWorkspace.case267PrimaryEvent.eventType = 0x121;
        eventWorkspace.case267PrimaryEvent.field_10 = 5;
        func_800EC918(self, pc, (EC918_BattleObjAccessor*)pc, &eventWorkspace.case267PrimaryEvent, tgt);
        eventWorkspace.case267PrimaryEvent.eventType = 0x11;
        eventWorkspace.case267PrimaryEvent.field_10 = *(s32*)((u8*)acc + 0x3F10);  // ORIGINAL acc
        eventWorkspace.case267PrimaryEvent.field_30 |= 0x800;                         // ori 0x800, 0x1e4
        func_800EC918(self, pc, (EC918_BattleObjAccessor*)pc, &eventWorkspace.case267PrimaryEvent, tgt);
        acc = (EC918_BattleObjAccessor*)pc;       // mr r24,r23
        break;                                         // b .L_800F4000
    }

    case 290: {
        evt->prevEventType = evt->eventType;
        evt->field_30 = copyBattleEvent(eventWorkspace.case290Event, *evt) | 2;
        eventWorkspace.case290Event.eventType = 0x54;                          // sth r0(0x54), 0x158
        eventWorkspace.case290Event.field_30 |= 0x400;                         // ori 0x400, 0x17c
        if (!func_800EC918(self, pc, acc, &eventWorkspace.case290Event, tgt)) return 0;
        // .L_800F3D10
        eventWorkspace.case290Event.eventType = 0x5A;
        void* entry = func_80149330((u8*)pc + 8, 0x5A, evt->field_00, evt->field_04, 0);
        if (entry != nullptr) {
            *(s32*)((u8*)entry + 0x10) += 5;           // addi r0,r4,0x5
            *(f32*)((u8*)entry + 0x20) = evt->field_20; // stfs evt->20
            break;                                     // b .L_800F4000
        }
        func_800EC918(self, pc, (EC918_BattleObjAccessor*)pc, &eventWorkspace.case290Event, tgt);     // acc arg = pc
        break;                                         // b .L_800F4000
    }

    case 291: {
        evt->prevEventType = evt->eventType;
        evt->field_30 = copyBattleEvent(eventWorkspace.case291Event, *evt) | 2;
        eventWorkspace.case291Event.eventType = 0x52;                          // sth r0(0x52), 0x124
        eventWorkspace.case291Event.field_30 |= 0x400;                         // ori 0x400, 0x148
        if (!func_800EC918(self, pc, acc, &eventWorkspace.case291Event, tgt)) return 0;
        // .L_800F3E1C
        eventWorkspace.case291Event.eventType = 0x58;
        void* entry = func_80149330((u8*)pc + 8, 0x58, evt->field_00, evt->field_04, 0);
        if (entry != nullptr) {
            *(s32*)((u8*)entry + 0x10) += 5;
            *(f32*)((u8*)entry + 0x20) = evt->field_20;
            break;                                     // b .L_800F4000
        }
        func_800EC918(self, pc, (EC918_BattleObjAccessor*)pc, &eventWorkspace.case291Event, tgt);     // acc arg = pc
        break;                                         // b .L_800F4000
    }

    case 292: {
        for (s32 i = 0; i < 0x20; i++) {               // vfunc 0x58 list
            if (!func_80145C00(*(u16*)((u8*)ecVf58(&acc->subObject, i) + 0x2E))) {
                if (tgt != nullptr &&
                    *(u16*)((u8*)ecVf58(&acc->subObject, i) + 0x2C) == tgt->field_80) {
                    continue;                          // beq .L_800F3F04
                }
                ecVf20(&acc->subObject,
                       *(u16*)((u8*)ecVf58(&acc->subObject, i) + 0x0C));
            }
        }
        for (s32 i = 0; i < 0x20; i++) {               // vfunc 0x5C list
            if (!func_80145C00(*(u16*)((u8*)ecVf5C(&acc->subObject, i) + 0x2E))) {
                if (tgt != nullptr &&
                    *(u16*)((u8*)ecVf5C(&acc->subObject, i) + 0x2C) == tgt->field_80) {
                    continue;                          // beq .L_800F3F9C
                }
                ecVf20(&acc->subObject,
                       *(u16*)((u8*)ecVf5C(&acc->subObject, i) + 0x0C));
            }
        }
        break;                                         // b .L_800F4000
    }

    case 50: {
        if (pc != nullptr) {
            if ((*(u32*)((u8*)pc + 0x3F00) & 0x2) && (*(u16*)((u8*)pc + 0x3F28) == 0x2)) {
                if (func_80148778(
                        &((EC918_BattleObjAccessor*)pc)->subObject, 0x32)) {
                    SubAccessor* playerStatus =
                        &((EC918_BattleObjAccessor*)pc)->subObject;
                    ecVf20(playerStatus, 0x32);
                    return 0;   // li r3,0 ; b .L_800F41D4
                }
            }
        }
        break;   // beq .L_800F4000 -> shared tail
    }

    default:
        break;
    }

    // ================================================================
    // SHARED TAIL (.L_800F4000)
    // ================================================================

    // ---- Part 1: func_80145C00 -> synthesize type 0x34 event ----
    if (func_80145C00(evt->eventType)) {
        void* data90 = func_80149154((&acc->subObject), 0x90);
        if (data90 != nullptr) {
            copyBattleEvent(eventWorkspace.tailEvent, *evt);
            eventWorkspace.tailEvent.field_00 = *(u32*)((u8*)acc + 0x3F10);
            eventWorkspace.tailEvent.field_04 = 0x90;
            eventWorkspace.tailEvent.eventType = 0x34;
            eventWorkspace.tailEvent.field_10 = *(s32*)((u8*)data90 + 0x10);
            eventWorkspace.tailEvent.field_14 = 0;
            eventWorkspace.tailEvent.field_20 = k2_0f;
            eventWorkspace.tailEvent.field_30 |= *(u32*)((u8*)data90 + 0x30);
            func_800EC918(self, pc, acc, &eventWorkspace.tailEvent, tgt);
        }
    }

    // ---- Part 2: slot 0x9E multiplier (func_80146148 gate) ----
    if (pc != nullptr) {
        if (func_80146148(evt->eventType)) {
            void* data9E = func_80149154((&acc->subObject), 0x9E);
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
        ecVfEvent(&acc->subObject, evt);
    }

    return 1;
}

#undef ecVfEvent
#undef ecVfStatus
#undef ecVf5B0
#undef ecVf2D4
#undef ecVf2C4
#undef ecVf2BC
#undef ecVf290
#undef ecVf224
#undef ecVf158
#undef ecVf154
#undef ecVf150
#undef ecVf108
#undef ecVfE0
#undef ecVfAC
#undef ecVf70
#undef ecVf5C
#undef ecVf58
#undef ecVf60
#undef ecVf54
#undef ecVf4C
#undef ecVf30
#undef ecVf24
#undef ecVf20
#undef ecVf0C
#undef vcall_v3f
#undef vcall_d1
#undef vcall_f1
#undef vcall_v1
#undef vcall_p1
#undef vcall_f
#undef vcall_p
#undef vcall_i

#pragma pop

// The remaining functions in this translation unit still use provisional
// pointer types for battle data. Keep their call sites concise while routing
// them through func_800EC918's recovered, strongly typed interface.
#define func_800EC918(manager, player, accessor, event, target)                 \
    func_800EC918(                                                             \
        (manager),                                                             \
        (player),                                                              \
        (EC918_BattleObjAccessor*)(accessor),                                  \
        (BattleEvent*)(event),                                                 \
        (BattleTargetData*)(target)                                            \
    )

// Thunk: dispatch through secondary vtable at +0x8, calling vtable[0x20] on
// the sub-object, passing (subobj, arg).
void func_800F3958(void* ignored, void* self, void* arg) {
    reinterpret_cast<BMIf*>((u8*)self + 8)->_v020(arg);
}
unsigned char func_800F3DC8(void* self, int key) { const unsigned char* item = static_cast<const unsigned char*>(self) + 0x94; for (int i = 0; i < 32; ++i) { if (*reinterpret_cast<const int*>(item) == key) return item[4]; item += 8; } return 0; }
void func_800F4004(void* this_) { unsigned char* self = static_cast<unsigned char*>(this_); void* anchor = *reinterpret_cast<void**>(self + 0x48); void* node = *reinterpret_cast<void**>(anchor); while (node != *reinterpret_cast<void**>(self + 0x48)) { unsigned char* object = *reinterpret_cast<unsigned char**>(static_cast<unsigned char*>(node) + 0x8); *reinterpret_cast<unsigned int*>(object + 0x3f04) |= 0x40; node = *reinterpret_cast<void**>(node); } }
void cf::CBattleManager::func_800F42A0() {
    unk84 = 0;
}

namespace cf {
    CChainCombo::~CChainCombo() {
    }

    CChainMember::~CChainMember() {
    }

    CChainChance::~CChainChance() {
    }

    CChainEffect::~CChainEffect() {
    }

    CChainTime::~CChainTime() {
        // Manual vptr store under the retail symbol (auto store would emit
        // __vt__Q22cf10CChainTime). The vtable pointer sits at +0x14.
        *(void**)((u8*)this + 0x14) = (void*)lbl_eu_8052BD74;
        resetChainTime();
    }

    UnkClass_800D8DBC::~UnkClass_800D8DBC() {
    }
}

// Searches for a matching value in this->field_B8 entries and this->+8 array.
// Returns 1 if field_78 bit 0 is clear, or if a matching non-zero element is
// found without a live actor bound to it. Returns 0 otherwise.
s32 func_800D7D24(D7D24_Obj* self) {
    s32 result = 0;

    // If bit 0 of field_78 is clear, report 1
    if ((self->field_78 & 1) == 0) {
        result = 1;
    } else {
        u8* r6 = (u8*)self->field_B8;
        if (r6 != nullptr) {
            // First loop: 2 iterations x 8 fields of a 0x20-byte entry.
            // Target word re-read at the top of every iteration (retail shape).
            // Comparand is [arrayBase+4], NOT self->field_4 (retail lwz r0,4(r6)).
            u32 found = 0;
            s32 idx;
            u8* p = r6;
            for (idx = 0; idx != 14; idx += 7) {
                if (*(u32*)(p + 0x08) == *(u32*)(r6 + 4)) { found = 1; break; }
                if (*(u32*)(p + 0x0C) == *(u32*)(r6 + 4)) { found = 1; break; }
                if (*(u32*)(p + 0x10) == *(u32*)(r6 + 4)) { found = 1; break; }
                if (*(u32*)(p + 0x14) == *(u32*)(r6 + 4)) { found = 1; break; }
                if (*(u32*)(p + 0x18) == *(u32*)(r6 + 4)) { found = 1; break; }
                if (*(u32*)(p + 0x1C) == *(u32*)(r6 + 4)) { found = 1; break; }
                if (*(u32*)(p + 0x20) == *(u32*)(r6 + 4)) { found = 1; break; }
                if (*(u32*)(p + 0x24) == *(u32*)(r6 + 4)) { found = 1; break; }
                p += 0x20;
            }

            // Matching entry only counts if its id resolves to an actor.
            if (found && func_800B708C(*(s32*)(r6 + 4)) != nullptr) return result;

            // Second pass: walk the u32 array at self+8 (up to 16 entries); stop at
            // the first non-zero word and report whether it equals self->field_4.
            {
                u8* scan = (u8*)self;
                for (s32 i = 0; i < 16; i++) {
                    if (*(u32*)(scan + 8) != 0) {
                        if (*(u32*)((u8*)self + i * 4 + 8) == *(u32*)((u8*)self + 4)) {
                            result = 1;
                        }
                        return result;
                    }
                    scan += 4;
                }
            }
        }
    }

    return result;
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
    // Retail starts the accumulator at 1.0f (lbl_eu_80666DD4).
    float result = lbl_eu_80666DD4;

    // Linked source object at +0x50 (or null if source is null)
    BattleSrcObjAccessor* src = nullptr;
    if (source != nullptr) src = *(BattleSrcObjAccessor**)((u8*)source + 0x50);

    // ---- First block: accumulate from target's battle status values ----
    if(target != nullptr){
        if(func_80148778((u8*)target + 8, 0x8f) != 0){
            void* entry = func_80149154((u8*)target + 8, 0x8f);
            result += lbl_eu_80666DD8 * (f32)((BDSessionEntry*)entry)->field_10;
        }else if(func_80148778((u8*)target + 8, 0x8e) != 0){
            void* entry = func_80149154((u8*)target + 8, 0x8e);
            result += lbl_eu_80666DD8 * (f32)((BDSessionEntry*)entry)->field_10;
        }

        // vfunc 0x290 on the target + a 0x3e-flagged source entry
        if(((BMVtIfD81A8*)target)->vf290() != nullptr && src != nullptr &&
           (src->field_78 & 0x40000000) && src->type_3c == 3){
            s32 v;
            if(func_80260264(((BMVtIfD81A8*)target)->vf290(), 0x3e, &v) != 0){
                result += lbl_eu_80666DD8 * (f32)v;
            }
        }
    }

    // ---- Second block: accumulate from obj's data ----
    if(obj != nullptr){
        // Chain gauge when the source is a chain-type object (or absent)
        if(src == nullptr || src->type_3c == 3){
            cf::CBattleManager* mgr = (cf::CBattleManager*)lbl_eu_80663F00;
            if(func_802799F0((u8*)mgr + 0x1A8, obj) != 0){
                result += ((CBattleManager_20AC*)mgr)->field_20AC;
            }
        }

        // vfunc 0x290 on obj + a 0x3d-flagged source entry
        if(((BMVtIfD81A8*)obj)->vf290() != nullptr && src != nullptr &&
           (src->field_78 & 0x40000000) && src->type_3c == 3){
            s32 v;
            if(func_80260264(((BMVtIfD81A8*)obj)->vf290(), 0x3d, &v) != 0){
                result += lbl_eu_80666DD8 * (f32)v;
            }
        }

        // Art type 5 on the source: 0x42 value
        if(src != nullptr && (src->field_78 & 0x40000000) && src->field_40 == 5){
            s32 v;
            if(func_80260264(((BMVtIfD81A8*)obj)->vf290(), 0x42, &v) != 0){
                result += lbl_eu_80666DD8 * (f32)v;
            }
        }

        // Generic 0x3f value against the target's vfunc 0x5C0 / 0x130 gate
        if(src != nullptr && (src->field_78 & 0x40000000)){
            s32 v;
            if(func_80260264(((BMVtIfD81A8*)obj)->vf290(), 0x3f, &v) != 0){
                if(target != nullptr && ((BMVtIfD81A8*)obj)->vf5C0(target) != nullptr &&
                   ((BMVtIfD81A8*)target)->vf130() < lbl_eu_80666DE8){
                    result += lbl_eu_80666DD8 * (f32)v;
                }
            }
        }
    }

    return result;
}
// Per-frame battle-manager update: drains the art-countdown timer (unk88),
// resets the sub-systems, and every full second (accumulator at +0x283D4)
// sweeps mActorList1, unregistering every dead actor from all three lists
// and handing it to func_800D9CA0 when the actor-id gate passes.
extern "C" void func_800D9354(cf::CBattleManager* self) {
    // Frame-delta scaled by the art-recovery rate.
    f32 delta = func_80496288(lbl_eu_80663E14) * lbl_eu_80666DEC;

    if (!((BMVtIf828*)self)->v008(0x10)) {
        self->func_800E2584(0x10);
        lbl_eu_80663E24 |= 0x10000000;
    }

    func_801A39D8(&self->mVision);
    func_8018C610(&self->unk194);
    func_80192CB0(&self->unk19C);
    func_8027732C(&self->mChain);
    func_8027D20C(&self->unk20C8);
    func_801BA25C(&self->mSuddenCommu);
    func_80274B20((u8*)&self->mSuddenCommu + 0x28);
    func_8027EF50();

    if (self->unk88 > lbl_eu_80666DDC) {
        f32 v = self->unk88 - delta;
        self->unk88 = v;
        if (v < lbl_eu_80666DDC) {
            self->func_800EA470();
        } else {
            func_800EA484(self, self->unk90, self->unk8C);
        }
    }

    cf::CfGameManager::getInstance();
    if (func_8006EF04__Fi(0x4000000) == 0) {
        f32* ticker = &((CBattleManagerTail283D4*)self)->field_283D4;
        f32 tv = *ticker + delta;
        *ticker = tv;
        if (tv >= lbl_eu_80666DD4) {
            _reslist_node<cf::CfObjectActor*>* node = self->mActorList1.mStartNodePtr->mNext;
            while (node != self->mActorList1.mStartNodePtr) {
                cf::CfObjectActor* obj = node->mItem;
                void* sub = obj ? (void*)((u8*)obj + 0x3e9c) : 0;
                if (func_80086B18__Q22cf13CfGameManagerFv(sub) == 0) {
                    self->mActorList1.remove(obj);
                    self->mActorList2.remove(obj);
                    self->mActorList3.remove(obj);

                    if (obj != 0) {
                        int id = 0;
                        u8* sub4 = ((cf::CfActorField04*)obj)->field_0x04;
                        u32* holder = ((cf::CfActorUnk4Vt30*)sub4)->vf30();
                        id = (int)*holder;
                        if (func_80174C98(holder, &id, 0x802)) {
                            func_800D9CA0(self, (BattleRemoveObjAccessor*)obj);
                            break;
                        }
                    }
                }
                node = node->mNext;
            }
            *ticker = lbl_eu_80666DDC;
        }
    }
}

// Battle-actor registration: actors already in mActorList1 only refresh the
// busy check; unregistered idle actors are pushed into the lists, notified
// through their vtable holders and the battle-event chain, and reset the
// sudden-commu voice state when they become the lone list-2 member.
extern "C" void func_800D9978(cf::CBattleManager* self, cf::CfObjectActor* actor) {
    cf::CfGameManager::getInstance();
    if (func_8006EF04__Fi(0x4000000) != 0) return;

    BattleRegObjAccessor* acc = (BattleRegObjAccessor*)actor;
    if (acc->field_3374 & 0x08000000) return;
    if (acc->field_3374 & 0x00002000) return;

    // Is the actor already registered in mActorList1?
    _reslist_node<cf::CfObjectActor*>* head1 = self->mActorList1.mStartNodePtr;
    _reslist_node<cf::CfObjectActor*>* cur = head1->mNext;
    while (cur != head1 && cur->mItem != actor) cur = cur->mNext;

    if (cur != head1) {
        // Already registered: refresh the busy check.
        ((cf::CVisionBattleObj*)actor)->vf2BC();
    } else if (((cf::CVisionBattleObj*)actor)->vf2BC() != 0) {
        // Busy and unregistered: retail re-runs the check call.
        ((cf::CVisionBattleObj*)actor)->vf2BC();
    } else {
        // Register the actor in the lists.
        self->mActorList1.push_back(actor);
        {
            CChain_chkActorList(&self->mChain);
        }
        if (acc->field_3F00 & 0x2) {
            self->mActorList2.push_back(actor);
        } else {
            self->mActorList3.push_back(actor);
        }

        // Notify the vtable-holder sub-object.
        ((BMSub3ED4Vt*)acc->field_3ED4)->vf78();
        func_800E9FE4(self, actor, 0, 0, 0, 0, 0);

        // Notify every registered battle event (vtable slot 0xC).
        _reslist_node<cf::IBattleEvent*>* ev =
            self->mBattleEventList.mStartNodePtr->mNext;
        while (ev != self->mBattleEventList.mStartNodePtr) {
            ((BattleEventVtIf0C*)(ev->mItem))->v00C(actor);
            ev = ev->mNext;
        }

        // If mActorList2 holds at most one actor, reset the voice state.
        // Retail walks via a 2-slot pointer table spilled to the frame.
        const _reslist_node<cf::CfObjectActor*>* nodes[2];
        nodes[0] = self->mActorList2.mStartNodePtr;
        nodes[1] = nodes[0]->mNext;
        u32 count = 0;
        while (nodes[1] != nodes[0]) {
            count++;
            nodes[1] = nodes[1]->mNext;
        }
        if (count <= 1 && (acc->field_3F00 & 0x2)) {
            func_802A2D84();
            func_8027F0A0();
            self->mSuddenCommu.unk28 = 0;
            self->mSuddenCommu.unk2C = lbl_eu_80666DDC;
        }

        if (acc->field_3F00 & 0x4) {
            ((BMSub3ED4Vt*)acc->field_3ED4)->vf100();
            func_802A2210(actor);
        }
    }
}
void func_800D9CA0(cf::CBattleManager* mgr, BattleRemoveObjAccessor* target){
    cf::CfGameManager::getInstance();
    if(func_8006EF04__Fi(0x400) != 0) return;

    // Only proceed if the target is actually a member of mActorList1.
    if(mgr->mActorList1.find((cf::CfObjectActor*)target) != mgr->mActorList1.end()){
        mgr->mActorList1.remove((cf::CfObjectActor*)target);
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

        // Notify all battle events (retail re-reads the sentinel each pass)
        {
            _reslist_node<cf::IBattleEvent*>* cur = mgr->mBattleEventList.mStartNodePtr->mNext;
            while(cur != mgr->mBattleEventList.mStartNodePtr){
                cf::IBattleEvent* event = cur->mItem;
                void** vtbl = *(void***)event;
                typedef void (*VFunc10)(void*, cf::CfObjectActor*);
                ((VFunc10)vtbl[0x10 / 4])(event, (cf::CfObjectActor*)target);
                cur = cur->mNext;
            }
        }

        // Drain mActorList1 sentinel loop
        {
            _reslist_node<cf::CfObjectActor*>* cur = mgr->mActorList1.mStartNodePtr->mNext;
            _reslist_node<cf::CfObjectActor*>* head = mgr->mActorList1.mStartNodePtr;
            while(cur != head) cur = cur->mNext;
        }

        // Global list state gates (sequential, with func_800F41A0 between)
        if(((cf::CBattleManager*)lbl_eu_80663F00)->mActorList3.size() != 0) return;
        func_800F41A0(mgr);
        if(((cf::CBattleManager*)lbl_eu_80663F00)->mActorList1.size() != 0) return;

        if(!(target->field_3f00 & 0x04)){
            if(((cf::CBattleManager*)lbl_eu_80663F00)->mActorList1.size() != 0) return;
        }

        // Player cleanup loop
        for(int i = 0; i < 3; i++){
            void* player = getPlayer__Q22cf13CfGameManagerFi(i);
            void* action = func_8016FE34(player);
            if(action != nullptr){
                void** vtbl = *(void***)((u8*)action + 0x4);
                typedef void (*VFunc)(void*, u32);
                ((VFunc)vtbl[0x20 / 4])((u8*)action + 0x4, 0x40);
                ((VFunc)vtbl[0x24 / 4])((u8*)action + 0x4, 0x80);
                ((VFunc)vtbl[0x28 / 4])((u8*)action + 0x4, 0x8000);
                func_80174B4C(action, 0x800);
                func_80174B4C(action, 0x1000);
            }
        }

        // Call virtual func_80085220(mgr, 1, 1)
        mgr->func_80085220(1, 1);

        // Clear global flag (rlwinm mask 29..27 clears bit 28)
        lbl_eu_80663E28 &= ~0x8;
        func_80277B34(&mgr->mChain);

        // Enum list iteration
        EnumListHolder holder;
        func_80043D90(&holder);
        func_800F4A98(func_80043F18(&holder), 0x20, 0);

        for(u32 i = 0; i < ((cf::CVisionEnumList*)func_80043F18(&holder))->count; i++){
            void* element = func_8016FE34(func_800F6EAC(func_80043F18(&holder), i));
            void** vtbl = *(void***)element;
            typedef void (*VFunc)(void*);
            ((VFunc)vtbl[0xB0 / 4])(element);
            vtbl = *(void***)element;
            ((VFunc)vtbl[0xB8 / 4])(element);
            vtbl = *(void***)element;
            ((VFunc)vtbl[0x314 / 4])(element);
        }

        __dt__80043E88(&holder, -1);
    }
}

// --- func_800DA0A4 accessors (retail 0x800DAB8C) ---
// Actor object: flags word +0x3F00, target ptr +0x3F10, id u16 +0x3F28 and the
// embedded move sub-object at +0x3E9C.
struct DA0A4_Actor {
    u8 pad_00[0x08];
    void* embeddedVt;              // +0x08 (vtable holder, slot 0x20)
    u8 pad_0C[0x3E9C - 0x0C];
    u8 moveSub;                    // +0x3E9C
    u8 pad_3E9D[0x3F00 - 0x3E9D];
    u32 flags_3F00;                // +0x3F00
    u8 pad_3F04[0xC];
    void* target_3F10;             // +0x3F10
    u8 pad_3F14[0x14];
    u16 id_3F28;                   // +0x3F28
};

// Move record returned by the actor's vt 0x2A4 slot.
struct DA0A4_Sub {
    u32 field_00;
    u32 field_04;
    u32 field_08;
    u8 pad_0C[0x28 - 0x0C];
    u16 field_28;
    u8 pad_2A[0x48 - 0x2A];
    u32 field_48;
    void* subObj;                  // +0x50
    u8 pad_54[0x68 - 0x54];
    f32 field_68;
    u8 pad_6C[0x74 - 0x6C];
    u32 field_74;
    u32 field_78;
};

// Payload of DA0A4_Sub (+0x50).
struct DA0A4_SubObj {
    u8 pad_00[0x28];
    u16 field_28;
    u8 pad_2A[0x2C - 0x2A];
    f32 field_2C;
    u8 pad_30[0x34 - 0x30];
    s16 field_34;
    u8 pad_36[0x5C - 0x36];
    u16 field_5C;
    u16 field_5E;
    f32 field_60;
    u16 field_64;
    u8 pad_66[0x78 - 0x66];
    u32 field_78;
    u8 pad_7C[0x80 - 0x7C];
    f32 field_80;
    u8 pad_84[4];
    void* vtbl_84;                 // +0x84 pseudo-vtable (slot 0x14 -> f32)
};

// 0xBC state block copied around via vt 0x298 / vt 0x2A0 results.
struct DA0A4_Block {
    u32 w[21];                     // 0x00..0x53
    f32 f54[7];                    // 0x54..0x6F
    s16 s70;
    s16 s72;
    u32 w74;
    u32 w78;
    u32 w7C;
    u16 w80;
    u16 pad82;
    u32 w84[13];                   // 0x84..0xBB
};

extern "C" void* func_800BFC68__FPQ22cf12CfObjectMove(void*);
extern "C" void func_802A1C68(void*);
extern "C" void func_802773EC(void*, void*);
extern "C" void func_802A2EEC(void*);
extern "C" void func_8027F2DC(void*);
extern "C" void func_802A2F54(void*);
extern "C" void func_800FB270(void*, void*, f32, f32, f32, s32);
extern "C" void __ct__800FBA18(void*, f32, f32, f32, s32);
extern "C" void func_800FB5AC(void*, void*, f32, f32, f32, f32, f32, s32);
extern "C" void func_800DB0FC(void*, void*, void*, void*);
extern "C" f32 FrSqrt__Q24nw4r4mathFf(f32);
extern "C" void Warning__Q24nw4r2dbFPCciPCce(const char*, int, const char*, ...);
extern "C" void* func_800F6E98(void*, int);
extern f64 lbl_eu_80666E08;
extern f64 lbl_eu_80666DE0;
extern f32 lbl_eu_80666DF0;
extern f32 lbl_eu_80666DF4;
extern f32 lbl_eu_80666DF8;
extern f32 lbl_eu_80666DFC;
extern f32 lbl_eu_80666E14;
extern f32 lbl_eu_80666E28;
extern f32 lbl_eu_80666E18;
extern f32 lbl_eu_8066A1F8;
extern "C" s32 func_80086DBC__Q22cf13CfGameManagerFv(void);
extern "C" u32 func_8004C5EC(void*);

void func_800DA0A4(void* self_, void* actor_) {
    cf::CBattleManager* mgr = (cf::CBattleManager*)self_;
    DA0A4_Actor* actor = (DA0A4_Actor*)actor_;
    extern u32 lbl_eu_8052B110[];
    extern u32 lbl_eu_804FCA48[];
    // int->double conversion temporaries (0x4330 trick), pre-seeded once.
    cf::CfActorF64Conv convA;
    cf::CfActorF64Conv convB;
    convA.w[0] = 0x43300000;
    convB.w[0] = 0x43300000;

    // ptmf gate: process only while the manager's 0x28354 event handler is not
    // the default handler recorded in lbl_eu_8052B110.
    u32 defaultHandler[3];
    defaultHandler[0] = lbl_eu_8052B110[0];
    defaultHandler[1] = lbl_eu_8052B110[1];
    defaultHandler[2] = lbl_eu_8052B110[2];
    if (__ptmf_cmpr((u8*)mgr + 0x28354, defaultHandler) == 0) {
        return;
    }

    // Register the actor into the manager list when missing.
    {
        SimpleListNode* head = *(SimpleListNode**)((u8*)mgr + 0x08);
        SimpleListNode* cur = head->next;
        while (cur != head && cur->data != actor) cur = cur->next;
        if (cur == head) {
            func_800D9978(mgr, (cf::CfObjectActor*)actor);
        }
    }

    // Linked move record (vt 0x2A4) and its payload at +0x50.
    DA0A4_Sub* sub = (DA0A4_Sub*)vcall_p(actor, 0x2A4);
    DA0A4_SubObj* subObj = (DA0A4_SubObj*)sub->subObj;
    if (subObj == nullptr) return;
    if (sub->field_48 == 0) return;

    // Dispatch target: actor flag bit1 selects the arts-data path.
    void* action;
    if (actor->flags_3F00 & 0x02) {
        DA0A4_Actor* artsOwner = (DA0A4_Actor*)func_800BFC68__FPQ22cf12CfObjectMove(
            actor ? (void*)&actor->moveSub : (void*)actor);
        if (sub->field_04 != 0) {
            action = (void*)sub->field_04;
        } else {
            action = vcall_p(artsOwner ? (void*)&artsOwner->moveSub : (void*)artsOwner, 0x4C);
        }
        if (action == nullptr) return;
        if ((artsOwner ? (void*)&artsOwner->moveSub : (void*)artsOwner) ==
            getPlayer__Q22cf13CfGameManagerFi(0)) {
            vcall_v1(mgr, 0x28, 1);
        }
    } else {
        action = vcall_p(&actor->moveSub, 0x4C);
    }

    // The action's stats must carry one of the two active bits.
    void* stats = func_800B708C((int)(u32)action);
    if (stats == nullptr) return;
    u32 statFlags = *(u32*)((u8*)stats + 0x64);
    if (!(statFlags & 0x04) && !(statFlags & 0x02)) return;
    void* res = func_8016FE34(stats);

    // Reset the move record when the 0x08000000 flag is clear.
    if (!(sub->field_78 & 0x08000000)) {
        sub->field_74 = 0;
        sub->field_78 = (sub->field_78 & 0xFFFF) | (sub->field_78 & 0x00F00000);
        std::memset((u8*)sub + 0x08, 0, 0x40);
        *(f32*)((u8*)sub + 0x68) = vcall_f(res, 0x128);
    }

    func_802A1C68(actor);
    func_802773EC((u8*)mgr + 0x1A8, actor);
    func_800DB0FC((void*)lbl_eu_80663F00, actor, res, sub);

    // 24-word splash parameter table copied pairwise from .rodata.
    u32 splash[24];
    {
        u32* src = lbl_eu_804FCA48;
        u32* dst = splash;
        for (s32 i = 0; i < 12; i++) {
            dst[0] = src[0];
            dst[1] = src[1];
            dst += 2;
            src += 2;
        }
    }

    EnumListHolder holder;
    func_80043D90(&holder);

    // Range value: scaled target-id (u16 at subObj+0x64).
    convA.w[1] = (u32)subObj->field_64 ^ 0x80000000;
    f32 range = (f32)(convA.d - lbl_eu_80666E08) * lbl_eu_8066A210;
    f32 power = subObj->field_60;
    u32 tblWord = *(u32*)((u8*)splash + subObj->field_5C * 8 +
                          ((actor->flags_3F00 & 0x02) ? 0 : 8));
    u16 alt5E = subObj->field_5E;

    // Skill modifier 0x77 raises the base power inside its window.
    void* skill = vcall_p(actor, 0x290);
    if (skill != nullptr) {
        u32 sv;
        if (func_80260264(skill, 0x77, &sv) != 0) {
            if (subObj->field_78 & 0x40000000) {
                if (power <= lbl_eu_80666DF0 && power >= lbl_eu_80666DF4) {
                    convB.w[1] = (u32)(s32)sv ^ 0x80000000;
                    power = power + (f32)(convB.d - lbl_eu_80666DE0);
                    if (power > lbl_eu_80666DF8) {
                        power = lbl_eu_80666DF8;
                    }
                }
            }
        }
    }

    // Doubled power for the flagged actor class.
    if ((subObj->field_78 & 0x40000000) && (actor->flags_3F00 & 0x04) &&
        subObj->field_2C != lbl_eu_80666DDC) {
        power *= lbl_eu_80666DFC;
    }

    // Hit position: vt 0x12C(0x64) sample, else vt 0xAC vector. Ease the y
    // component halfway toward the sub-record's y, then measure the drop.
    ml::CVec3 hitPos;
    {
        void* sample = vcall_p1(&actor->moveSub, 0x12C, 0x64);
        if (sample != nullptr) {
            hitPos.x = *(f32*)((u8*)sample + 0x0C);
            hitPos.y = *(f32*)((u8*)sample + 0x1C);
            hitPos.z = *(f32*)((u8*)sample + 0x2C);
        } else {
            hitPos = *(ml::CVec3*)vcall_p(&actor->moveSub, 0xAC);
        }
        void* q = vcall_p(&actor->moveSub, 0xAC);
        hitPos.y = hitPos.y - (hitPos.y - *(f32*)((u8*)q + 0x04)) * lbl_eu_80666DE8;
        void* q2 = vcall_p(&actor->moveSub, 0xAC);
        f32 dy = hitPos.y - *(f32*)((u8*)q2 + 0x04);
        if (dy != lbl_eu_80666DDC) {
            f32 distSq = power * power + dy * dy;
            if (distSq < lbl_eu_80666DDC) {
                Warning__Q24nw4r2dbFPCciPCce(lbl_eu_80526324, 0x273,
                                             lbl_eu_80526300);
            }
            if (distSq > lbl_eu_80666DDC) {
                distSq = distSq * FrSqrt__Q24nw4r4mathFf(distSq);
            }
            power = distSq;
        }
    }

    // Chapter-gated chain gauge overrides the hit position with the facing vec.
    f32 gauge = vcall_f(actor, 0x5B8);
    {
        u8 chapter = *(u8*)((u8*)lbl_eu_80663F00 + 0x1AA);
        if (chapter >= 1 && chapter <= 0x18) {
            hitPos = *(ml::CVec3*)vcall_p(&actor->moveSub, 0xAC);
        }
    }

    // Splash-kind selection: entry 0x95 of the actor's status table forces
    // kind 1 and rescales the range by its entry value.
    s32 splashKind = 0;
    if (!(subObj->field_78 & 0x40000000)) {
        void* entry = func_80149154((u8*)actor + 0x08, 0x95);
        if (entry != nullptr) {
            convA.w[1] = (u32)*(s32*)((u8*)entry + 0x10) ^ 0x80000000;
            range = (f32)(convA.d - lbl_eu_80666DE0) * lbl_eu_8066A210;
            splashKind = 1;
        }
    }
    // Damage variant: 8 unless flagged actor / special ids say otherwise.
    s32 dmgKind = 8;
    if (actor->flags_3F00 & 0x02) {
        dmgKind = 0;
    }
    if (actor->flags_3F00 & 0x04) {
        switch (actor->id_3F28) {
        case 0x8B3:
        case 0x8B4:
        case 0x8A0:
        case 0x6C5:
        case 0x5F2:
        case 0x607:
        case 0x60B:
        case 0x11C:
        case 0x9C5:
            dmgKind = 0;
            break;
        }
    }

    switch (splashKind) {
    case 1:
        func_800F4A98(func_80043F18(&holder), tblWord, 0x1000);
        func_800FB270(func_80043F18(&holder), &hitPos, power, range, gauge, dmgKind);
        __ct__800FB044(func_80043F18(&holder), power, &hitPos, 0);
        break;
    case 2:
        func_800F4A98(func_80043F18(&holder), tblWord, 0x1000);
        func_800FB270(func_80043F18(&holder), &hitPos, power, range,
                      gauge + lbl_eu_8066A1F8, dmgKind);
        __ct__800FB044(func_80043F18(&holder), power, &hitPos, 0);
        break;
    case 3:
        func_800F4A98(func_80043F18(&holder), tblWord, 0x1000);
        func_800FB5AC(func_80043F18(&holder), &hitPos, power, range,
                      gauge + lbl_eu_8066A1F8 * lbl_eu_80666DE8, range,
                      gauge - lbl_eu_8066A1F8 * lbl_eu_80666DE8, dmgKind);
        __ct__800FB044(func_80043F18(&holder), power, &hitPos, 0);
        break;
    case 4:
        func_800F4A98(func_80043F18(&holder), tblWord, 0x1000);
        __ct__800FB044(func_80043F18(&holder), power, &hitPos, dmgKind);
        break;
    case 5: {
        convB.w[1] = (u32)subObj->field_64 ^ 0x80000000;
        range = (f32)(convB.d - lbl_eu_80666E08) / lbl_eu_80666E00;
        func_800F4A98(func_80043F18(&holder), tblWord, 0x1000);
        __ct__800FBA18(func_80043F18(&holder), power, range, gauge, dmgKind);
        __ct__800FB044(func_80043F18(&holder), power, &hitPos, 0);
        break;
    }
    case 6: {
        convB.w[1] = (u32)subObj->field_64 ^ 0x80000000;
        range = (f32)(convB.d - lbl_eu_80666E08) / lbl_eu_80666E00;
        void* pos = vcall_p((u8*)res + 0x3E9C, 0xAC);
        func_800F4A98(func_80043F18(&holder), tblWord, 0x1000);
        __ct__800FB044(func_80043F18(&holder), range, pos, dmgKind);
        break;
    }
    case 7:
        func_800F4A98(func_80043F18(&holder), tblWord, 0x1000);
        break;
    default: {
        // empty the enum list.
        void* list = func_80043F18(&holder);
        *(u32*)((u8*)list + 0x620) = 0;
        *(u32*)((u8*)list + 0x3030) = 0;
        if (subObj->field_5C == 2) {
            func_800F6D50(list, (u32)actor->target_3F10);
        }
        break;
    }
    }

    // Target-type 2: make sure the actor itself ends up on the enum list.
    if (subObj->field_5C == 2) {
        bool registered = false;
        for (u32 i = 0;; i++) {
            void* list = func_80043F18(&holder);
            if (i >= *(u32*)((u8*)list + 0x620)) break;
            if (func_800F6E98(list, i) == actor->target_3F10) {
                registered = true;
                break;
            }
        }
        if (!registered) {
            func_800F6D50(func_80043F18(&holder), (u32)actor->target_3F10);
        }
    }

    __dt__80043E88(&holder, -1);

    // Snapshot the 0xBC state block through vt 0x298, then publish it back via
    // vt 0x2A0 (vt 0x2B4 fires for side effects only when its flag is clear).
    if (!(sub->field_78 & 0x08000000)) {
        vcall_p(actor, 0x2B4);
    }
    DA0A4_Block* snapshot = (DA0A4_Block*)vcall_p(actor, 0x298);
    *snapshot = *(DA0A4_Block*)sub;
    *(DA0A4_Block*)vcall_p(actor, 0x2A0) = *snapshot;

    getInstance__Q22cf13CfGameManagerFv();
    if (!func_8006EF04__Fi(0x04000000)) {
        if (!func_80148778((u8*)actor + 0x08, 0x2F)) {
            *(f32*)((u8*)subObj + 0x80) =
                ((f32 (*)(void*))(*(void**)((u8*)subObj->vtbl_84 + 0x14)))(subObj);
        }
        if (!func_80148778((u8*)actor + 0x08, 0x30)) {
            vcall_f1(actor, 0x13C, lbl_eu_80666DDC);
        }
        if (!func_80148778((u8*)actor + 0x08, 0x31)) {
            convB.w[1] = (u32)(s32)subObj->field_34 ^ 0x80000000;
            f32 penalty = (f32)(convB.d - lbl_eu_80666DE0);
            if (penalty < lbl_eu_80666DDC) {
                if (func_80148778((u8*)actor + 0x08, 0x12C)) {
                    void* entry = func_80149154((u8*)actor + 0x08, 0x12C);
                    if (entry != nullptr) {
                        convA.w[1] = (u32)*(s32*)((u8*)entry + 0x10) ^ 0x80000000;
                        penalty *= lbl_eu_80666DD4 -
                                   (f32)(convA.d - lbl_eu_80666DE0) / lbl_eu_80666E00;
                    }
                }
            }
            vcall_f1(actor, 0x154, -penalty);
        }
    }

    if (actor->id_3F28 == 1 && (actor->flags_3F00 & 0x02) &&
        (subObj->field_78 & 0x8000)) {
        *(u16*)vcall_p(actor, 0x27C) = 0;
        vcall_v1((u8*)actor + 0x08, 0x20, 0xEA);
    }

    if (subObj->field_78 & 0x40000000) {
        if (actor->flags_3F00 & 0x02) {
            if (subObj->field_28 != 1) {
                func_800451D8(0x7A, (int)(actor ? (void*)&actor->moveSub : (void*)actor));
                u32 voiceId = 0;
                if (vcall_i(actor, 0x290) != 0) {
                    void* skill2 = vcall_p(actor, 0x290);
                    func_80260264(skill2, 0x3B, &voiceId);
                }
                func_800F3970((void*)lbl_eu_80663F00, actor, nullptr, 0x0B, (s32)voiceId);
            }
        } else {
            if (subObj->field_78 & 0x01) {
                func_800451D8(0x7D, (int)(actor ? (void*)&actor->moveSub : (void*)actor));
            } else {
                func_800451D8(0x7C, (int)(actor ? (void*)&actor->moveSub : (void*)actor));
            }
        }
        func_8027F2DC(actor);
        func_802A2F54(actor);
    } else {
        func_802A2EEC(actor);
        if (sub->field_78 & 0x02000000) {
            func_80109784(actor->target_3F10, 2, 0x10);
        } else if (sub->field_78 & 0x01000000) {
            func_80109784(actor->target_3F10, 1, 0x10);
        }
    }

    if (vcall_i(actor, 0x290) != 0) {
        if (func_800D7D24((D7D24_Obj*)sub) != 0) {
            void* skill3 = vcall_p(actor, 0x290);
            if (func_8026178C(skill3, 0x3C) != 0) {
                if (sub->field_78 & 0x00100000) {
                    if (subObj->field_28 != 1) {
                        vcall_v1(actor, 0x304, vcall_i(actor, 0x308) + 1);
                    }
                } else if (sub->field_78 & 0x00200000) {
                    if (subObj->field_78 & 0x8000) {
                        vcall_v1(actor, 0x304, vcall_i(actor, 0x308) + 1);
                    }
                }
            }
        }
    }
}
// func_800DB0FC: per-move ai dispatch. Fills the 13-entry ai table in arg4,
// computes the bearing to the enemy (nw4r paired-single vector math), sets
// the aim-sector flags, then dispatches to the arts-specific helpers by the
// linked sub-object's type.
extern "C" void func_800DB0FC(void* self, void* obj, void* enemy, void* arg4){
    if(enemy == nullptr) return;

    DB0FC_MoveTable* move = (DB0FC_MoveTable*)arg4;
    if(move->field_78 & 0x10) return;

    // 13-entry random ai table; the two vt[0x28] flags pin entries 0 and 4.
    for(int i = 0; i < 13; i++){
        if(move->field_78 & 0x40) continue;
        move->table[i] = mtRand__Q22ml4mathFi(100);
        if(((BMVtIf828*)self)->v008(0x2000) != 0){
            if(i == 4 || i == 0) move->table[i] = 0x63;
        }else if(((BMVtIf828*)self)->v008(0x4000) != 0){
            if(i == 4 || i == 0) move->table[i] = 0;
        }
    }

    move->field_78 |= 0x40000040;
    void* sub = move->field_50;
    if(sub == nullptr) return;

    // Bearing from the enemy to the move source (paired-single subtract).
    void* objData = vcall_p((u8*)obj + 0x3E9C, 0xAC);
    void* enemyData = vcall_p((u8*)enemy + 0x3E9C, 0xAC);
    ml::CVec3 diff = *(ml::CVec3*)enemyData - *(ml::CVec3*)objData;

    f32 f31;
    if(((BattleRemoveObjAccessor*)enemy)->field_3f00 & 0x02){
        f32 f30 = lbl_eu_80666E10 * Atan2FIdx__Q24nw4r4mathFff(diff.x, diff.z);
        f31 = f30 - vcall_f(enemy, 0x5B4);
    }else{
        void* subRet = vcall_p((u8*)enemy + 0x3E9C, 0x110);
        f32 f30 = *(f32*)((u8*)subRet + 0xC);
        f31 = lbl_eu_80666E10 * Atan2FIdx__Q24nw4r4mathFff(diff.x, diff.z) - f30;
    }

    // Wrap the angle into [-pi, pi).
    while(lbl_eu_8066A1F8 <= f31) f31 -= lbl_eu_8066A1FC;
    while(f31 < -lbl_eu_8066A1F8) f31 += lbl_eu_8066A1FC;

    // Aim-sector flags from the enemy's facing cone.
    if(-(lbl_eu_80666DE8 * vcall_f(enemy, 0x340)) <= f31 &&
       f31 <= lbl_eu_80666DE8 * vcall_f(enemy, 0x340)){
        move->field_74 |= 0x82000000;
    }else{
        f32 t = vcall_f(enemy, 0x33C);
        if(t - lbl_eu_80666DE8 * vcall_f(enemy, 0x340) <= f31 &&
           f31 <= t + lbl_eu_80666DE8 * vcall_f(enemy, 0x340)){
            move->field_74 |= 0x84000000;
        }else{
            move->field_74 |= 0x81000000;
        }
    }

    // Dispatch by the linked sub-object's move type.
    u16 type = ((BattleSubObjAccessor*)sub)->type_3c;
    if((u32)(type - 4) <= 2){
        func_800DBA2C(self, obj, enemy, arg4);
    }else if((u32)(type - 1) <= 1){
        func_800DB4FC(self, obj, enemy, arg4);
    }else if(type == 3){
        func_800DB7F8(self, obj, enemy, arg4);
    }

    move->field_78 |= 0x40000010;
    CBattleManagerTail283D4* tail = (CBattleManagerTail283D4*)self;
    // u16 dispatch counter at +0x80 of the move block.
    *(u16*)&move->pad_80 = tail->field_283D0;
    tail->field_283D0 = (u16)(tail->field_283D0 + 1);
}

// Same-TU battle helper (retail-unmangled name; defined later in this file).
extern "C" void func_800DBACC(void*, void*, void*, void*);

// func_800DCB54 is defined later in this TU with a BattleMoveData* param;
// forward-declare the incomplete type so the declaration matches and the
// call site below stays consistent with the retail tail.
struct BattleMoveData;
extern "C" void func_800DCB54(void* self, void* attacker, void* target, BattleMoveData* move);

// Battle move/arts setup: initialises the move block, routes to func_800DBACC
// or the combo path from the arts object's flag, then runs the damage-gap
// checks (status windows, arts-type gates) that set the counter/knockback
// flags on move->field_74 before the shared func_800DCB54 tail.
extern "C" void func_800DB4FC(void* self, void* obj, void* enemyArg, void* arg4) {
    if (enemyArg == 0) return;

    BattleObjAccessor* bob = (BattleObjAccessor*)obj;
    DB4FC_EnemyObj* enemy = (DB4FC_EnemyObj*)enemyArg;
    DB4FC_MoveBlock* move = (DB4FC_MoveBlock*)arg4;

    // Cache the pool floats in locals first (retail loads f0=0.0f, f1=1.0f
    // once instead of reloading the globals around each store).
    f32 zero = lbl_eu_80666DDC;
    f32 one = lbl_eu_80666DD4;
    move->field_58 = one;
    DB4FC_ArtsObj* arts = (DB4FC_ArtsObj*)move->artsData;
    move->field_54 = one;
    move->field_5C = zero;
    move->field_60 = zero;
    move->field_64 = zero;

    if (bob->field_3f00 & 0x2) {
        if (move->field_78 & 0x400) {
            func_80192C2C(&((cf::CBattleManager*)self)->unk19C);
        }
    }

    if (arts->field_78 & 0x8) {
        move->field_74 |= 0x80000001;
    } else {
        func_800DBACC(self, obj, enemyArg, arg4);
    }

    if (func_802799F0(&((cf::CBattleManager*)self)->mChain, obj) != 0) {
        move->field_74 |= 0x80004000;
    }

    if (move->field_78 & 0x20) return;

    if ((move->field_74 & 1) && !(move->field_74 & 2) &&
        !(enemy->field_3374 & 0x1000) &&
        ((BMSubVtIf140*)((u8*)enemy + 0x3E9C))->vf140() != lbl_eu_80666DDC &&
        func_80148778(&enemy->statusBase, 0x32) == 0) {
        s32 flag = 0;

            if (func_80148778(&enemy->statusBase, 0xCE) && (move->field_78 & 0x800)) {
                void* entry = func_801491F4(&enemy->statusBase, 0xCE);
                s32 val = ((BMSubVtIf0C*)arts)->vf0C();
                if (*(s32*)((u8*)entry + 0x10) >= val) flag = 1;
            }

            if (flag == 0) {
                if (!(arts->field_78 & 0x2000) ||
                    arts->field_44 == (s32)(((BMVtIf2A8*)bob)->vf2A8() + 1)) {
                    int type58 = arts->field_58;
                    if (type58 == 1) {
                        if (!func_80148778(&enemy->statusBase, 0xF) &&
                            !func_80148778(&enemy->statusBase, 0x10)) {
                            if (func_80148778(&enemy->statusBase, 0x29)) {
                                void* e = func_80149154(&enemy->statusBase, 0x29);
                                if (move->field_B4 >= *(s32*)((u8*)e + 0x10)) {
                                    move->field_74 |= 0x80010000;
                                }
                            } else {
                                move->field_74 |= 0x80010000;
                            }
                        }
                    } else if (type58 == 2) {
                                flag = 1;
                                if (((BMVtIf290*)enemy)->vf290()) {
                                    if (func_8026178C(((BMVtIf290*)enemy)->vf290(), 0x76)) flag = 0;
                                }
                                if (func_80148778(&enemy->statusBase, 0x28)) {
                                    void* e = func_80149154(&enemy->statusBase, 0x28);
                                    if (move->field_B4 < *(s32*)((u8*)e + 0x10)) flag = 0;
                                }
                                if (flag) move->field_74 |= 0x80020000;
                            }
                        }
                    }
        }

    func_800DCB54(self, obj, enemyArg, (BattleMoveData*)arg4);
}
// Battle event handler: checks various flags and conditions on arg3/arg4,
// sets flags on arg4->field_74, then tail-calls func_800E08E8.
void func_800DB7F8(void* r3, void* r4, void* arg3, void* arg4) {
    s32 flag;
    if (arg3 == nullptr) return;    // retail: beq straight to the epilogue

    BattleTargetData* move = (BattleTargetData*)arg4;
    DB7F8_MoveSub* sourceObj = (DB7F8_MoveSub*)move->artsData;

    // Set flags on move->field_78
    move->field_78 |= 0x40000100;

    // Check if move->field_74 & 2, skip if set
    if (move->field_74 & 2) goto tailcall;

    // Check if arg3->field_3374 & 0x1000, skip if set
    if (*(u32*)((u8*)arg3 + 0x3374) & 0x1000) goto tailcall;

    // Virtual call on arg3's embedded sub-object at +0x3E9C, slot 0x140
    if (((BMSubVtIf140*)((u8*)arg3 + 0x3E9C))->vf140() == lbl_eu_80666DDC) goto tailcall;

    // Check flag 0x32 on arg3+8
    if (func_80148778((u8*)arg3 + 8, 0x32)) goto tailcall;

    // Check flag 0xCE
    flag = 0;
    if (func_80148778((u8*)arg3 + 8, 0xCE)) {
        // Check if move->field_78 & 0x800
        if (move->field_78 & 0x800) {
            void* entry = func_801491F4((u8*)arg3 + 8, 0xCE);
            // Dispatch the sub-vtable at sourceObj+0x84, slot 0xC, this = sourceObj
            s32 val = ((BMSubVtIf0C*)sourceObj)->vf0C();
            if (*(s32*)((u8*)entry + 0x10) >= val) flag = 1;
        }
    }

    if (flag != 0) goto tailcall;

    // Check sourceObj type
    u16 type = sourceObj->type_58;
    if (type == 1) {
        if (func_80148778((u8*)arg3 + 8, 0xF)) goto tailcall;
        if (func_80148778((u8*)arg3 + 8, 0x10)) goto tailcall;

        if (func_80148778((u8*)arg3 + 8, 0x29)) {
            void* entry = func_80149154((u8*)arg3 + 8, 0x29);
            if (move->field_B4 < *(u32*)((u8*)entry + 0x10)) goto tailcall;
        }

        // Set flags on move->field_74
        move->field_74 |= 0x80010000;
        goto tailcall;
    }

    if (type == 2) {
        flag = 1;

        // Call vfunc_0x290 on arg3 (twice, as retail does)
        if (((BMVtIf290*)arg3)->vf290() != nullptr) {
            if (func_8026178C(((BMVtIf290*)arg3)->vf290(), 0x76)) flag = 0;
        }

        if (func_80148778((u8*)arg3 + 8, 0x28)) {
            void* entry = func_80149154((u8*)arg3 + 8, 0x28);
            if (move->field_B4 < *(u32*)((u8*)entry + 0x10)) flag = 0;
        }

        if (flag != 0) {
            move->field_74 |= 0x80020000;
        }
    }

tailcall:
    func_800E08E8(r3, r4, arg3, arg4);
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
// func_800DBACC (retail 0x800DBACC, 0x838 bytes): target-selection / hit-intent
// gate for the shared move dispatchers. Runs the status + art-type gates on the
// target, computes hit / critical intent from the sub-object stats and
// level/party modifiers, then sets the appropriate field_74 flag (1 = hit,
// 2 = miss, 3 = dodge-class) or falls through to the damage-splash tail.

// Target/actor battle-object view for func_800DBACC.
struct DBACC_Target {
    u8 pad_00[0x04];
    void* field_04;                     // status-list provider (vtable slot 0x30)
    u8 pad_08[0x3374 - 0x08];
    u32 field_3374;                     // state flags (bit17/bit16 probed)
    u8 pad_78[0x3F00 - 0x3378];
    u32 field_3F00;                     // controller flags (bits 31/30/29)
    u8 pad_F04[0x3F60 - 0x3F04];
    void* field_3F60;                   // optional extra object
};

struct DBACC_Sub {
    u8 pad_00[0x1E];
    s16 field_1E;           // +0x1E
    u8 pad_20[0x36 - 0x20];
    s16 field_36;           // +0x36
    u8 pad_38[0x3C - 0x38];
    u16 type_3C;            // +0x3C
    u8 pad_3E[0x55 - 0x3E];
    u8 field_55;            // +0x55 base critical threshold
    u8 pad_56[0x78 - 0x56];
    u32 field_78;           // +0x78 flags
    u8 pad_7C[0x84 - 0x7C];
    void** field_84;        // +0x84 alternate vtable (called with this = sub)
};

// Move block view for func_800DBACC.
struct DBACC_Move {
    u8 pad_00[0x50];
    void* field_50;         // +0x50 sub-object (DBACC_Sub)
    f32 field_54;
    f32 field_58;
    f32 field_5C;
    u8 pad_60[0x74 - 0x60];
    u32 field_74;           // tagged flag word
    u32 field_78;           // flags
    u8 pad_7C[0x80 - 0x7C];
    u16 field_80;
    u8 pad_82[0x88 - 0x82];
    s32 field_88;           // hit roll
    s32 field_8C;           // critical roll
};

// Slot-0xC dispatch on the sub-object's auxiliary vtable (this = sub).
static __inline s32 dbaccSlotC(DBACC_Sub* sub) {
    return ((s32 (*)(void*))(*(void**)((u8*)sub->field_84 + 0xC)))(sub);
}

void func_800DBACC(void* self, BattleObjAccessor* arg1, void* arg2, void* move_) {
    DBACC_Move* move = (DBACC_Move*)move_;
    if (arg2 == nullptr) return;

    DBACC_Target* atk = (DBACC_Target*)arg1;
    DBACC_Target* tgt = (DBACC_Target*)arg2;

    DBACC_Sub* prm1 = (DBACC_Sub*)vcall_p(arg1, 0x224);
    DBACC_Sub* prm2 = (DBACC_Sub*)vcall_p(arg2, 0x224);
    DBACC_Sub* sub = (DBACC_Sub*)move->field_50;

    s32 res = 0;
    if (sub->field_78 & 0x8) res = 2;
    if ((atk->field_3F00 & 0x4) && (sub->field_78 & 0x1)) res = 2;
    if (move->field_78 & 0x1000) res = 2;

    s32 hitVal = 0;
    if (res == 0) {
        // Status gate chain: any check firing blocks the action outright.
        int sv;
        bool blocked;
        sv = *(s32*)vcall_p(tgt->field_04, 0x30);
        blocked = func_80174C98(tgt, &sv, 0x805) != 0;
        if (!blocked) blocked = func_80148778((u8*)arg2 + 8, 0x9) != 0;
        if (!blocked) blocked = func_80148778((u8*)arg2 + 8, 0xb) != 0;
        if (!blocked) blocked = func_80148778((u8*)arg2 + 8, 0xf) != 0;
        if (!blocked) { sv = *(s32*)vcall_p(tgt->field_04, 0x30); blocked = func_80174C98(tgt, &sv, 0x4) != 0; }
        if (!blocked) { sv = *(s32*)vcall_p(tgt->field_04, 0x30); blocked = func_80174C98(tgt, &sv, 0x19) != 0; }
        if (!blocked) { sv = *(s32*)vcall_p(tgt->field_04, 0x30); blocked = func_80174C98(tgt, &sv, 0x1a) != 0; }
        if (!blocked) { sv = *(s32*)vcall_p(tgt->field_04, 0x30); blocked = func_80174C98(tgt, &sv, 0x1b) != 0; }
        if (!blocked) blocked = func_80148778((u8*)arg2 + 8, 0x32) != 0;
        if (!blocked) blocked = func_80148778((u8*)arg2 + 8, 0xf8) != 0;
        if (!blocked) blocked = func_80148778((u8*)arg2 + 8, 0xeb) != 0;
        if (!blocked) blocked = func_80148778((u8*)arg2 + 8, 0xf0) != 0;
        if (!blocked) blocked = func_80148778((u8*)arg2 + 8, 0xf1) != 0;
        if (!blocked) blocked = func_80148778((u8*)arg2 + 8, 0xf8) != 0;
        if (!blocked) blocked = func_80148778((u8*)arg2 + 8, 0xf7) != 0;
        if (!blocked) blocked = func_80148778((u8*)arg2 + 8, 0x106) != 0;
        // Extra-object page gate.
        if (!blocked && tgt->field_3F60 == nullptr) blocked = true;
        if (!blocked && func_8004C5EC(tgt->field_3F60) == 0x31) blocked = true;

        if (!blocked) {
            f32 chance = lbl_eu_80666DDC;
            if (sub->type_3C == 1 || sub->type_3C == 5) {
                f32 lo = lbl_eu_80666DDC;   // floor
                f32 hi = lbl_eu_80666E14;   // cap
                f32 mod = lbl_eu_80666DD4;  // party modifier
                chance = lo;
                if (vcall_i(arg1, 0x290) != 0) {
                    u32 q1;
                    if (func_80260264(vcall_p(arg1, 0x290), 0x1D, &q1)) {
                        mod -= (f32)q1 / lbl_eu_80666E00;
                    }
                    u32 q2;
                    if (func_80260264(vcall_p(arg1, 0x290), 0x7E, &q2) && (move->field_74 & 0x40)) {
                        chance = lo - (f32)q2;
                    }
                }
                if ((atk->field_3F00 & 0x2) && vcall_i(arg1, 0x308) == 0) res = 1;
                if ((atk->field_3F00 & 0x2) && vcall_i(arg1, 0x308) == 1) chance += lbl_eu_80666E18 * mod;
                if (func_80148778((u8*)arg2 + 8, 0x116)) res = 1;
                s32 lv = vcall_i(arg2, 0x108) - vcall_i(arg1, 0x108);
                if (lv < -10) lv = -10;
                if (lv > 10) lv = 10;
                if ((u32)(lv + 5) > 10) chance += (f32)(lv * 20);
                else if ((u32)(lv + 2) > 4) chance += (f32)(lv * 8);
                chance -= (f32)prm2->field_36;
                if (atk->field_3F00 & 0x2) chance += lbl_eu_80666E18 * (f32)prm1->field_36;
                if (func_80086DBC__Q22cf13CfGameManagerFv() == 4) {
                    if (atk->field_3F00 & 0x2) {
                        if (func_80148778((u8*)arg1 + 8, 0xa2)) {
                            void* e = func_80149154((u8*)arg1 + 8, 0xa2);
                            chance -= (f32)*(s32*)((u8*)e + 0x10);
                        }
                    } else if (tgt->field_3F00 & 0x2) {
                        if (func_80148778((u8*)arg2 + 8, 0xa2)) {
                            void* e = func_80149154((u8*)arg2 + 8, 0xa2);
                            s32 ev = *(s32*)((u8*)e + 0x10);
                            chance += (f32)(ev / 2);
                        }
                    }
                }
                if (func_80148778((u8*)arg2 + 8, 0xd1)) chance += lbl_eu_80666E20;
                if (func_80148778((u8*)arg2 + 8, 0xf5)) {
                    chance += lbl_eu_80666E18;
                    if (vcall_f(arg2, 0x130) < lbl_eu_80666E24) chance += lbl_eu_80666DF8;
                } else if (func_80148778((u8*)arg1 + 8, 0xf5)) {
                    chance -= lbl_eu_80666E18;
                    if (vcall_f(arg2, 0x130) < lbl_eu_80666E24) chance -= lbl_eu_80666DF8;
                }
                if (func_80148778((u8*)arg2 + 8, 0x114)) chance += lbl_eu_80666E20;
                f32 f1 = chance - (f32)(sub->field_36 * dbaccSlotC(sub));
                f1 = f1 + (f32)(prm2->field_1E - prm1->field_1E);
                if (f1 < lo) f1 = lo;
                if (f1 > hi) f1 = hi;
                if (res == 0) {
                    if (move->field_88 % 100 < (s32)f1) res = 1;
                }
                hitVal = (s32)f1;
            } else {
                f32 lo = chance;              // floor
                f32 hi = lbl_eu_80666E28;     // cap
                s32 e0 = vcall_i(arg2, 0xE0);
                if (e0 == 2 || e0 == 0xa) {
                    u32 f3 = atk->field_3374;
                    if ((f3 & 0x4000) && !(f3 & 0x8000)) res = 3;
                }
                if ((atk->field_3F00 & 0x2) && vcall_i(arg1, 0x308) == 0) res = 3;
                if (func_80148778((u8*)arg2 + 8, 0x116)) res = 3;
                if ((atk->field_3F00 & 0x2) && vcall_i(arg1, 0x308) == 1) chance += lbl_eu_80666DF8;
                void* v290 = vcall_p(arg1, 0x290);
                if (v290 != nullptr) {
                    if (func_8026178C(vcall_p(arg1, 0x290), 0x82)) {
                        chance -= (f32)(sub->field_36 * dbaccSlotC(sub));
                    }
                }
                s32 lv = vcall_i(arg2, 0x108) - vcall_i(arg1, 0x108);
                if (lv < -10) lv = -10;
                if (lv > 10) lv = 10;
                if ((u32)(lv + 5) > 10) chance += (f32)(lv * 11);
                else if ((u32)(lv + 2) > 4) chance += (f32)(lv * 6);
                if (atk->field_3F00 & 0x2) chance -= (f32)(sub->field_36 * dbaccSlotC(sub));
                if (chance < lo) chance = lo;
                if (chance > hi) chance = hi;
                if (res == 0) {
                    if (move->field_88 % 100 < (s32)chance) res = 3;
                }
                hitVal = (s32)chance;
            }
        }
    }
    if (res == 0) res = 2;
    // Apply the intent flag: 1 = hit, 2 = miss/blocked, 3 = dodge-class.
    if (res == 1) move->field_74 |= 0x80000004;
    else if (res == 2) move->field_74 |= 0x80000001;
    else if (res == 3) move->field_74 |= 0x80000008;

    if (move->field_78 & 0x1000) return;

    bool splash = false;
    if (sub->type_3C == 1 || sub->type_3C == 5) {
        if (!(sub->field_78 & 0x8) && (move->field_74 & 1)) {
            s32 e0 = vcall_i(arg2, 0xE0);
            u32 f3374 = atk->field_3374;
            if (e0 == 1) {
                if (!func_80148778((u8*)arg2 + 8, 0x9) && !func_80148778((u8*)arg2 + 8, 0xf) &&
                    !func_80148778((u8*)arg2 + 8, 0x10)) {
                    if (!func_80148778((u8*)arg1 + 8, 0xea) && !func_80148778((u8*)arg1 + 8, 0xcf) &&
                        !func_80148778((u8*)arg1 + 8, 0xd0) && !(f3374 & 0x4000)) {
                        if (move->field_88 < 100) splash = true;
                    }
                }
            } else if (e0 == 2) {
                if (!func_80148778((u8*)arg2 + 8, 0x9) && !func_80148778((u8*)arg2 + 8, 0xf) &&
                    !func_80148778((u8*)arg2 + 8, 0x10)) {
                    if (!(f3374 & 0x8000) && !func_80148778((u8*)arg1 + 8, 0xd0)) {
                        if (move->field_88 < 100) splash = true;
                    }
                }
            } else if (e0 == 0xa) {
                if ((f3374 & 0x4000) && !(f3374 & 0x8000) && move->field_88 < 100) splash = true;
            }
        }
    } else if (sub->type_3C == 2 || sub->type_3C == 6) {
        if (func_80148778((u8*)arg2 + 8, 0xf7)) {
            if (vcall_p1(arg2, 0x5C0, (u32)arg1) == 0) {
                if (move->field_74 & 1) {
                    func_80149154((u8*)arg2 + 8, 0xf7);
                    u32 f78 = move->field_78;
                    if ((f78 & 0x200) || (!(f78 & 0x400) && !(f78 & 0x800))) {
                        move->field_74 |= 0x80000080;
                    }
                }
            }
        }
    }

    if (splash) {
        // Guaranteed-damage tail: set ratio fields, tag the move, bump the
        // shared dispatch counter.
        move->field_54 = lbl_eu_80666DD4;
        move->field_74 = (move->field_74 | 0x80000000) | 0x41;
        move->field_5C = move->field_54 * move->field_58;
        if (func_80148778((u8*)arg2 + 8, 0x1)) move->field_5C = lbl_eu_80666DDC;
        move->field_78 |= 0x40000020;
        CBattleManagerTail283D4* tail = (CBattleManagerTail283D4*)self;
        move->field_80 = tail->field_283D0;
        tail->field_283D0 = (u16)(tail->field_283D0 + 1);
        return;
    }

    if (!(move->field_74 & 1)) return;

    // Critical-proc gate chain.
    s32 res2 = 0;
    int sv;
    bool blocked2;
    sv = *(s32*)vcall_p(tgt->field_04, 0x30);
    blocked2 = func_80174C98(tgt, &sv, 0x805) != 0;
    if (!blocked2) { sv = *(s32*)vcall_p(tgt->field_04, 0x30); blocked2 = func_80174C98(tgt, &sv, 0x4) != 0; }
    if (!blocked2) { sv = *(s32*)vcall_p(tgt->field_04, 0x30); blocked2 = func_80174C98(tgt, &sv, 0x19) != 0; }
    if (!blocked2) { sv = *(s32*)vcall_p(tgt->field_04, 0x30); blocked2 = func_80174C98(tgt, &sv, 0x1a) != 0; }
    if (!blocked2) { sv = *(s32*)vcall_p(tgt->field_04, 0x30); blocked2 = func_80174C98(tgt, &sv, 0x1b) != 0; }
    if (!blocked2) blocked2 = func_80148778((u8*)arg2 + 8, 0x9) != 0;
    if (!blocked2) blocked2 = func_80148778((u8*)arg2 + 8, 0xb) != 0;
    if (!blocked2) blocked2 = func_80148778((u8*)arg2 + 8, 0xf) != 0;
    if (!blocked2) blocked2 = func_80148778((u8*)arg2 + 8, 0xf0) != 0;
    if (!blocked2) blocked2 = func_80148778((u8*)arg2 + 8, 0xf1) != 0;
    if (!blocked2) blocked2 = func_80148778((u8*)arg2 + 8, 0xf8) != 0;
    if (blocked2) res2 = 1;
    if (res2 == 0 && sub->type_3C != 1) res2 = 1;
    if (res2 == 0 && func_80148778((u8*)arg2 + 8, 0xeb) && !(move->field_78 & 0x800)) res2 = 2;
    if (res2 == 0 && func_80148778((u8*)arg2 + 8, 0x10c) && !(move->field_78 & 0x800)) res2 = 2;
    if (res2 == 0 && (atk->field_3F00 & 0x4) && (sub->field_78 & 0x1)) res2 = 1;
    if (sub->field_78 & 0x8) res2 = 1;
    if (res2 == 0) {
        s32 thr;
        if (tgt->field_3F00 & 0x2) {
            s32 d = vcall_i(arg2, 0x108) - vcall_i(arg1, 0x108);
            if ((u32)(d + 2) > 4) d = d * 5;
            thr = sub->field_55 + d;
        } else {
            thr = sub->field_55 + hitVal;
        }
        if (thr < 0) thr = 0;
        if (thr > 0x5f) thr = 0x5f;
        if (move->field_8C % 100 < thr) res2 = 2;
    }
    if (res2 == 2) move->field_74 |= 0x80000002;
}
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

static inline void* d_vf290(void* obj) {
    return ((VFn290)(*(void***)obj)[0x290 / 4])(obj);
}
static inline s32 d_subVf0C(BattleMoveSubData* sub) {
    return ((VSubFn0C)sub->field_84[0x0C / 4])(sub);
}
static inline BattleVec* d_vfAC(void* obj) {
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
    move->field_54 = lbl_eu_80666DD4;               // 0x51D4
    move->field_58 = lbl_eu_80666DD4;               // 0x51D0
    move->field_5C = lbl_eu_80666DDC;               // 0x51D8
    move->field_60 = lbl_eu_80666DDC;               // 0x51DC
    move->field_64 = lbl_eu_80666DDC;               // 0x51E0

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
            f32 f26;
            f32 f28;
            {
                cf::CfActorF64Conv convA;
                convA.w[0] = 0x43300000;
                convA.w[1] = (u32)*(s16*)((u8*)res20C + 0x62) ^ 0x80000000;
                f26 = (f32)(convA.d - lbl_eu_80666DE0) / lbl_eu_80666E00;
                convA.w[1] = (u32)tgtParam->field_62 ^ 0x80000000;
                f28 = (f32)(convA.d - lbl_eu_80666DE0) / lbl_eu_80666E00 - f26;
            }
            s32 r25 = atkParam->field_30;
            s32 r26 = tgtParam->field_32;

            // --- hit-flag conditional zero of f26 (0x800DE948) ---
            {
                u32 tag = move->field_74;
                if (tag & 0x2000000) {
                    u16 hf = ((VFn328)(*(void***)target)[0x328 / 4])(target);
                    if (!(hf & 0x4000)) f26 = lbl_eu_80666DDC;
                } else if (tag & 0x4000000) {
                    u16 hf = ((VFn328)(*(void***)target)[0x328 / 4])(target);
                    if (!(hf & 0x2000)) f26 = lbl_eu_80666DDC;
                } else if (tag & 0x1000000) {
                    u16 hf = ((VFn328)(*(void***)target)[0x328 / 4])(target);
                    if (!(hf & 0x1)) f26 = lbl_eu_80666DDC;
                }
            }
            f28 += f26;                                 // 0x800DE9D0

            // --- clamp [-1.0, 0.9] ---
            if (f28 < lbl_eu_80666E2C) f28 = lbl_eu_80666E2C;
            else if (f28 > lbl_eu_80666E30) f28 = lbl_eu_80666E30;

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
                s32 vf = d_subVf0C(sub);
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
                s32 v1 = d_subVf0C(sub);
                s32 v2 = d_subVf0C(sub);
                s32 v3 = d_subVf0C(sub);
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
                if (d_vf290(attacker) != 0) {
                    u32 sv;
                    if (func_80260264(d_vf290(attacker), 0x4E, &sv)) {
                        move->field_58 += (f32)(s32)sv / 100.0f;
                    }
                    if (sub->field_40 == 6) {
                        if (func_80260264(d_vf290(attacker), 0x55, &sv)) {
                            move->field_58 += (f32)(s32)sv / 100.0f;
                        }
                    }
                    if (sub->field_40 == 8) {
                        if (func_80260264(d_vf290(attacker), 0x56, &sv)) {
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
        f32 f26;
        f32 f28;
        {
            cf::CfActorF64Conv convA;
            convA.w[0] = 0x43300000;
            convA.w[1] = (u32)*(s16*)((u8*)res20C + 0x60) ^ 0x80000000;
            f26 = (f32)(convA.d - lbl_eu_80666DE0) / lbl_eu_80666E00;
            convA.w[1] = (u32)tgtParam->field_60 ^ 0x80000000;
            f28 = (f32)(convA.d - lbl_eu_80666DE0) / lbl_eu_80666E00 - f26;
        }
        s32 r25 = atkParam->field_2C;
        s32 r26 = tgtParam->field_2E;

        // --- hit-flag conditional zero of f26 (0x800DD78C) ---
        {
            u32 tag = move->field_74;
            if (tag & 0x2000000) {
                u16 hf = ((VFn328)(*(void***)target)[0x328 / 4])(target);
                if (!(hf & 0x4000)) f26 = lbl_eu_80666DDC;
            } else if (tag & 0x4000000) {
                u16 hf = ((VFn328)(*(void***)target)[0x328 / 4])(target);
                if (!(hf & 0x2000)) f26 = lbl_eu_80666DDC;
            } else if (tag & 0x1000000) {
                u16 hf = ((VFn328)(*(void***)target)[0x328 / 4])(target);
                if (!(hf & 0x1)) f26 = lbl_eu_80666DDC;
            }
        }
        f28 += f26;                                     // 0x800DD804

        // 0x800DD80C: sub->field_78 bit24 (0x800000) -> average r25
        if (sub->field_78 & 0x800000) {
            r25 = (r25 + atkParam->field_1E) / 2;
        }

        // --- clamp [-1.0, 0.9] ---
        if (f28 < lbl_eu_80666E2C) f28 = lbl_eu_80666E2C;
        else if (f28 > lbl_eu_80666E30) f28 = lbl_eu_80666E30;

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
            s32 vf = d_subVf0C(sub);
            s32 r5 = sub->field_38 + (s32)sub->field_6C * (vf - 1);
            if (((BattleObjAccessor*)attacker)->field_3f00 & 0x2) {
                f32 f2 = atkParam->field_24;
                f32 f1 = atkParam->field_28 - f2;
                s32 d = (s32)(lbl_eu_80666DD4 + f1);
                u32 r4 = move->field_94;
                r25 = (s32)((f32)(s32)r25 + f2) + (s32)(r4 % (u32)d);
            } else {
                r5 = (s32)((f32)(s32)r5 *
                           (f32)(s32)((move->field_94 % 21) + 90) /
                           lbl_eu_80666E00);
            }
            move->field_54 =
                (f32)(s32)(r25 - r26) * (f32)(s32)r5 / lbl_eu_80666E00;

            if (func_80148778((u8*)attacker + 8, 0xC8)) {
                move->field_54 *= lbl_eu_80666DE8;
            }
            if (move->field_54 < lbl_eu_80666DD4)
                move->field_54 = lbl_eu_80666DD4;
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
            s32 v1 = d_subVf0C(sub);
            s32 v2 = d_subVf0C(sub);
            s32 v3 = d_subVf0C(sub);
            s32 r18 = sub->field_38 + (s32)sub->field_6C * (v1 - 1);
            s32 r19 = sub->field_3A + (s32)sub->field_6D * (v2 - 1);
            s32 r20 = (s32)move->field_94;
            s32 r7 = r19 - r18 + 1;
            s32 r0 = sub->field_38 + (s32)sub->field_6C * (v3 - 1) +
                     (s32)(r20 % (u32)r7);
            move->field_54 =
                (f32)(s32)(r25 - r26) * (f32)(s32)r0 / 100.0f;       // 0x800DDBE4

            // 0x800DDBF0: clamp + apply ratio
            if (move->field_54 < lbl_eu_80666DD4)
                move->field_54 = lbl_eu_80666DD4;
            move->field_54 = move->field_54 - move->field_54 * f28;

            // 0x800DDC10: 0x93 -> field_58 += 0.5 ; 0xC0 -> += val/10 - 1
            if (func_80148778((u8*)attacker + 8, 0x93)) {
                move->field_58 += lbl_eu_80666DE8;
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
        BattleVec* vT = d_vfAC(target);
        BattleVec* vA = d_vfAC(attacker);
        f32 d = vA->y - vT->y;
        if (d < 0.0f) d = -d;
        if (d <= 10.0f) move->field_58 += 2.0f;         // 0x800DE5E8
    }

    // --- 0x800DE5F8: 0xCE reaction block ---
    if (func_80148778((u8*)target + 8, 0xCE)) {
        if (move->field_78 & 0x800) {
            void* entry = func_801491F4((u8*)target + 8, 0xCE);
            if ((s32)*(u32*)((u8*)entry + 0x10) >= d_subVf0C(sub)) {
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
        if (d_vf290(attacker) != 0) {
            if (func_8026178C(d_vf290(attacker), 0x59) &&
                (move->field_78 & 0x2000000)) {
                r20 = 100;
            }
            if (func_8026178C(d_vf290(attacker), 0x5A) &&
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
            if (d_vf290(attacker) != 0) {
                u32 sv;
                if (func_80260264(d_vf290(attacker), 0x51, &sv)) {
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
            if (d_vf290(attacker) != 0) {
                u32 sv;
                if (func_80260264(d_vf290(attacker), 0x52, &sv)) {
                    move->field_58 += (f32)(s32)sv / 100.0f;
                }
                if (func_80260264(d_vf290(attacker), 0x53, &sv) &&
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
                if (d_vf290(attacker) != 0) {
                    u32 sv;
                    if (func_80260264(d_vf290(attacker), 0x58, &sv)) {
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
    if (d_vf290(target) != 0) {
        u32 sv;
        if (move->field_78 & 0x400) {
            if (func_80260264(d_vf290(target), 0x4E, &sv) && sub->field_40 == 6) {
                move->field_58 += (f32)(s32)sv / 100.0f;
            }
            if (func_80260264(d_vf290(target), 0x4E, &sv) && sub->field_40 == 8) {
                move->field_58 += (f32)(s32)sv / 100.0f;
            }
        }
        if (sub->type_3C == 1 || sub->type_3C == 5) {
            if (func_80260264(d_vf290(target), 0x4F, &sv)) {
                move->field_58 *= 1.0f - (f32)(s32)sv / 100.0f;
            }
        } else {
            if (func_80260264(d_vf290(target), 0x50, &sv)) {
                move->field_58 *= 1.0f - (f32)(s32)sv / 100.0f;
            }
            if (func_80260264(d_vf290(target), 0x54, &sv) && sub->field_5E != 0) {
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
            steps[0] = d_subVf0C(sub) / 2 + 3;
            steps[1] = d_subVf0C(sub) + 0xA;
            steps[2] = 100;
            f32 mult = 1.0f;
            s32 acc = rem;
            for (int i = 0; i < 3; i++) {
                acc -= steps[i];
                if (acc < 0) { mult = sTable_28C[i]; break; }
            }
            if (d_vf290(target) != 0) {
                u32 sv;
                if (func_80260264(d_vf290(target), 0x5B, &sv)) {
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
            BattleVec* vT = d_vfAC(target);
            BattleVec* vA = d_vfAC(attacker);
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
        if (d_vf290(target) != 0) {
            if (func_80149154((u8*)target + 8, 0x6F) != 0 ||
                func_80149154((u8*)target + 8, 0x79) != 0) {
                u32 sv;
                if (func_80260264(d_vf290(target), 0x5B, &sv)) {
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
        if (d_vf290(attacker) != 0) {
            u32 sv;
            if (func_80260264(d_vf290(attacker), 0x57, &sv)) {
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

extern f64 lbl_eu_80666E08;   // 0x4330000000000000 sibling magic
extern f32 lbl_eu_80666E24;   // 0.15f

// C-linkage route: retail references func_800D7D24 by its unmangled symbol.
extern "C" s32 func_800D7D24(void*);

// Status-list entry overlay (func_80149154 result): flags word +0x08,
// payload word +0x10, tick s16 +0x14, aux s16 +0x1A.
struct E08E8_StatusEntry {
    u8 pad00[0x08];
    u32 w08;
    u32 pad0C;
    s32 w10;
    s16 s14;
    u8 pad16[0x1A - 0x16];
    s16 s1A;
};

// Move-state block overlay carried in arg4.
struct E08E8_Move {
    u8 pad00[0x50];
    void* subObj;   // +0x50 arts sub-object
    f32 f54;
    f32 f58;
    f32 f5C;
    f32 f60;
    f32 f64;
    f32 f68;
    u8 pad6C[0x70 - 0x6C];
    s16 s70;
    s16 s72;
    u32 w74;
    u32 w78;
    u8 pad7C[0xA8 - 0x7C];
    u32 wA8;
};

// Battle-object overlay (attacker/target views).
struct E08E8_Actor {
    u8 pad00[0x08];
    u8 statusBase;                    // +0x08 status list root
    u8 pad09[0x3374 - 0x09];
    u32 w3374;
    u8 pad78[0x3E9C - 0x3378];
    u8 moveSub;                       // +0x3E9C embedded move sub-object
    u8 padA0[0x3F10 - 0x3EA0];
    void* ptr3F10;
};

// Arts sub-object overlay (the pointer at move->0x50).
struct E08E8_SubObj {
    u8 pad00[0x3C];
    u16 u3C;
    u8 pad3E[0x44 - 0x3E];
    u8 b44;
    u8 pad45[0x48 - 0x45];
    u16 u48;
    s16 s4A;
    s16 s4C;
    u8 pad4E[0x50 - 0x4E];
    f32 f50;
    f32 f54;
    u8 pad58[0x66 - 0x58];
    u8 b66;
    u8 b67;
    s16 s68;
    s16 s6A;
    u8 pad6C[0x6E - 0x6C];
    u8 b6E;
    u8 b6F;
    u8 pad70[0x78 - 0x70];
    u32 w78;
    u8 pad7C[0x84 - 0x7C];
    void* vt84;
};

// Actor vtable mirror exposing exactly the slots retail dispatches through
// (0xE0/0x128/0x12C/0x130/0x224/0x290/0x2A8/0x2BC/0x5C0); the placeholder
// slots keep MWCC's dispatch routed through r12 like retail.
struct E08E8_Vt {
    virtual ~E08E8_Vt() {}
    virtual void q008(); virtual void q00C(); virtual void q010(); virtual void q014();
    virtual void q018(); virtual void q01C(); virtual void q020(); virtual void q024();
    virtual void q028(); virtual void q02C(); virtual void q030(); virtual void q034();
    virtual void q038(); virtual void q03C(); virtual void q040(); virtual void q044();
    virtual void q048(); virtual void q04C(); virtual void q050(); virtual void q054();
    virtual void q058(); virtual void q05C(); virtual void q060(); virtual void q064();
    virtual void q068(); virtual void q06C(); virtual void q070(); virtual void q074();
    virtual void q078(); virtual void q07C(); virtual void q080(); virtual void q084();
    virtual void q088(); virtual void q08C(); virtual void q090(); virtual void q094();
    virtual void q098(); virtual void q09C(); virtual void q0A0(); virtual void q0A4();
    virtual void q0A8(); virtual void q0AC(); virtual void q0B0(); virtual void q0B4();
    virtual void q0B8(); virtual void q0BC(); virtual void q0C0(); virtual void q0C4();
    virtual void q0C8(); virtual void q0CC(); virtual void q0D0(); virtual void q0D4();
    virtual void q0D8(); virtual void q0DC();
    virtual s32 vE0();
    virtual void q0E4(); virtual void q0E8(); virtual void q0EC(); virtual void q0F0();
    virtual void q0F4(); virtual void q0F8(); virtual void q0FC(); virtual void q100();
    virtual void q104(); virtual void q108(); virtual void q10C(); virtual void q110();
    virtual void q114(); virtual void q118(); virtual void q11C(); virtual void q120();
    virtual void q124();
    virtual f32 v128();
    virtual f32 v12C();
    virtual f32 v130();
    virtual void q134(); virtual void q138(); virtual void q13C(); virtual void q140();
    virtual void q144(); virtual void q148(); virtual void q14C(); virtual void q150();
    virtual void q154(); virtual void q158(); virtual void q15C(); virtual void q160();
    virtual void q164(); virtual void q168(); virtual void q16C(); virtual void q170();
    virtual void q174(); virtual void q178(); virtual void q17C(); virtual void q180();
    virtual void q184(); virtual void q188(); virtual void q18C(); virtual void q190();
    virtual void q194(); virtual void q198(); virtual void q19C(); virtual void q1A0();
    virtual void q1A4(); virtual void q1A8(); virtual void q1AC(); virtual void q1B0();
    virtual void q1B4(); virtual void q1B8(); virtual void q1BC(); virtual void q1C0();
    virtual void q1C4(); virtual void q1C8(); virtual void q1CC(); virtual void q1D0();
    virtual void q1D4(); virtual void q1D8(); virtual void q1DC(); virtual void q1E0();
    virtual void q1E4(); virtual void q1E8(); virtual void q1EC(); virtual void q1F0();
    virtual void q1F4(); virtual void q1F8(); virtual void q1FC(); virtual void q200();
    virtual void q204(); virtual void q208(); virtual void q20C(); virtual void q210();
    virtual void q214(); virtual void q218(); virtual void q21C(); virtual void q220();
    virtual void v224();
    virtual void q228(); virtual void q22C(); virtual void q230(); virtual void q234();
    virtual void q238(); virtual void q23C(); virtual void q240(); virtual void q244();
    virtual void q248(); virtual void q24C(); virtual void q250(); virtual void q254();
    virtual void q258(); virtual void q25C(); virtual void q260(); virtual void q264();
    virtual void q268(); virtual void q26C(); virtual void q270(); virtual void q274();
    virtual void q278(); virtual void q27C(); virtual void q280(); virtual void q284();
    virtual void q288();
    virtual s32 v290();
    virtual void q294(); virtual void q298(); virtual void q29C(); virtual void q2A0();
    virtual void q2A4();
    virtual s32 v2A8();
    virtual void q2AC(); virtual void q2B0(); virtual void q2B4(); virtual void q2B8();
    virtual s32 v2BC();
    virtual void q2C0(); virtual void q2C4(); virtual void q2C8(); virtual void q2CC();
    virtual void q2D0(); virtual void q2D4(); virtual void q2D8(); virtual void q2DC();
    virtual void q2E0(); virtual void q2E4(); virtual void q2E8(); virtual void q2EC();
    virtual void q2F0(); virtual void q2F4(); virtual void q2F8(); virtual void q2FC();
    virtual void q300(); virtual void q304(); virtual void q308(); virtual void q30C();
    virtual void q310(); virtual void q314(); virtual void q318(); virtual void q31C();
    virtual void q320(); virtual void q324(); virtual void q328(); virtual void q32C();
    virtual void q330(); virtual void q334(); virtual void q338(); virtual void q33C();
    virtual void q340(); virtual void q344(); virtual void q348(); virtual void q34C();
    virtual void q350(); virtual void q354(); virtual void q358(); virtual void q35C();
    virtual void q360(); virtual void q364(); virtual void q368(); virtual void q36C();
    virtual void q370(); virtual void q374(); virtual void q378(); virtual void q37C();
    virtual void q380(); virtual void q384(); virtual void q388(); virtual void q38C();
    virtual void q390(); virtual void q394(); virtual void q398(); virtual void q39C();
    virtual void q3A0(); virtual void q3A4(); virtual void q3A8(); virtual void q3AC();
    virtual void q3B0(); virtual void q3B4(); virtual void q3B8(); virtual void q3BC();
    virtual void q3C0(); virtual void q3C4(); virtual void q3C8(); virtual void q3CC();
    virtual void q3D0(); virtual void q3D4(); virtual void q3D8(); virtual void q3DC();
    virtual void q3E0(); virtual void q3E4(); virtual void q3E8(); virtual void q3EC();
    virtual void q3F0(); virtual void q3F4(); virtual void q3F8(); virtual void q3FC();
    virtual void q400(); virtual void q404(); virtual void q408(); virtual void q40C();
    virtual void q410(); virtual void q414(); virtual void q418(); virtual void q41C();
    virtual void q420(); virtual void q424(); virtual void q428(); virtual void q42C();
    virtual void q430(); virtual void q434(); virtual void q438(); virtual void q43C();
    virtual void q440(); virtual void q444(); virtual void q448(); virtual void q44C();
    virtual void q450(); virtual void q454(); virtual void q458(); virtual void q45C();
    virtual void q460(); virtual void q464(); virtual void q468(); virtual void q46C();
    virtual void q470(); virtual void q474(); virtual void q478(); virtual void q47C();
    virtual void q480(); virtual void q484(); virtual void q488(); virtual void q48C();
    virtual void q490(); virtual void q494(); virtual void q498(); virtual void q49C();
    virtual void q4A0(); virtual void q4A4(); virtual void q4A8(); virtual void q4AC();
    virtual void q4B0(); virtual void q4B4(); virtual void q4B8(); virtual void q4BC();
    virtual void q4C0(); virtual void q4C4(); virtual void q4C8(); virtual void q4CC();
    virtual void q4D0(); virtual void q4D4(); virtual void q4D8(); virtual void q4DC();
    virtual void q4E0(); virtual void q4E4(); virtual void q4E8(); virtual void q4EC();
    virtual void q4F0(); virtual void q4F4(); virtual void q4F8(); virtual void q4FC();
    virtual void q500(); virtual void q504(); virtual void q508(); virtual void q50C();
    virtual void q510(); virtual void q514(); virtual void q518(); virtual void q51C();
    virtual void q520(); virtual void q524(); virtual void q528(); virtual void q52C();
    virtual void q530(); virtual void q534(); virtual void q538(); virtual void q53C();
    virtual void q540(); virtual void q544(); virtual void q548(); virtual void q54C();
    virtual void q550(); virtual void q554(); virtual void q558(); virtual void q55C();
    virtual void q560(); virtual void q564(); virtual void q568(); virtual void q56C();
    virtual void q570(); virtual void q574(); virtual void q578(); virtual void q57C();
    virtual void q580(); virtual void q584(); virtual void q588(); virtual void q58C();
    virtual void q590(); virtual void q594(); virtual void q598(); virtual void q59C();
    virtual void q5A0(); virtual void q5A4(); virtual void q5A8(); virtual void q5AC();
    virtual void q5B0(); virtual void q5B4(); virtual void q5B8(); virtual void q5BC();
    virtual s32 v5C0(void* actor);
};

// Vtable of the object at *(subObj->vt84): value slot 0xC.
struct E08E8_SubVtIf {
    virtual void y000();
    virtual void y004();
    virtual void y008();
    virtual s32 v0C();
};

// Vtable of the embedded move sub-object at actor+0x3E9C: id slot 0x4C.
struct E08E8_MoveSubVtIf {
    virtual void m000(); virtual void m004(); virtual void m008(); virtual void m00C();
    virtual void m010(); virtual void m014(); virtual void m018(); virtual void m01C();
    virtual void m020(); virtual void m024(); virtual void m028(); virtual void m02C();
    virtual void m030(); virtual void m034(); virtual void m038(); virtual void m03C();
    virtual void m040(); virtual void m044(); virtual void m048();
    virtual void* v4C();
};

// func_800E08E8 (retail 0x800E13D0, 0x125C bytes): battle-event emission and
// move-state update shared by the move-type dispatchers. Resolves the event
// source (arg1 vs arg2 via the 0xF7 status / 0x5C0 self-check), runs the
// arts-type jump table, emits the event via func_800EAA2C, then applies the
// counter / dot / proc-rate damage math on the move block.
struct E08E8_EventData {
    u32  v00;       // +0x00 object id (arg1->0x3F10)
    u32  v04;       // +0x04 sub-object ptr (r31)
    u8   pad08[4];
    u16  v0C;       // +0x0C event type
    u16  pad0E;
    u32  v10;       // +0x10 value
    u16  v14;       // +0x14 extra u16 (sub event blocks)
    u16  pad16;
    u8   pad18[8];
    f32  v20;       // +0x20 float
    f32  v24;       // +0x24 float
};

// Superseded draft kept as dead code (never called; dropped by the linker).
static void func_800E08E8_stale(void* self, void* arg1, void* arg2, void* move) {
    if (arg2 == nullptr) return;

    // 0xF7 status + 0x5C0 self-target check: when no confirmation is pending
    // the event source falls back to arg1.
    if (!(*(u32*)((u8*)move + 0x74) & 0x08)) {
        if (func_80148778((u8*)arg2 + 8, 0xF7)) {
            if (((s32 (*)(void*, void*))(*(void***)arg2)[0x5C0 / 4])(arg2, arg1) == 0) {
                func_80149154((u8*)arg2 + 8, 0xF7);
                u32 f78 = *(u32*)((u8*)move + 0x78);
                if ((f78 & 0x200) || ((f78 & 0x400) && !(f78 & 0x800))) {
                    *(u32*)((u8*)move + 0x74) |= 0x80000080;
                }
            }
        }
    }

    // Event-source swap happens inside the above block (retail keeps arg2
    // until the source-swap condition, then reuses r28).
    u32 f74 = *(u32*)((u8*)move + 0x74);
    if (!(f74 & 0x08)) {
        u32 f78b = *(u32*)((u8*)move + 0x78);
        if ((f78b & 0x200) || ((f78b & 0x400) && !(f78b & 0x800))) {
            *(u32*)((u8*)move + 0x74) |= 0x80000080;
        }
    }

    *(f32*)((u8*)move + 0x60) = lbl_eu_80666DDC;
    *(f32*)((u8*)move + 0x64) = lbl_eu_80666DDC;
    void* subObj = *(void**)((u8*)move + 0x50);      // r31

    ((void* (*)(void*))(*(void***)arg1)[0x224 / 4])(arg1);
    ((void* (*)(void*))(*(void***)arg2)[0x224 / 4])(arg2);

    if (*(u32*)((u8*)move + 0x74) & 0x08) {
        if (!(*(u32*)((u8*)subObj + 0x78) & 0x1000)) goto done1744_1B8C;
    }

    s32 r30 = 0;
    if (*(u32*)((u8*)subObj + 0x78) & 0x2000) {
        u8 slot = *(u8*)((u8*)subObj + 0x44);
        if (slot != (u8)(((s32 (*)(void*))(*(void***)arg1)[0x2A8 / 4])(arg1) + 1)) {
            goto done1744_1B8C;
        }
    }

    // Arts-type jump table (byte +0x66 of the sub-object).
    u8 type = *(u8*)((u8*)subObj + 0x66);
    if (type <= 0x19) {
        switch (type) {
        case 0x00: {
            if (*(u32*)((u8*)arg1 + 0x3374) & 0x8000) {
                s32 e0 = ((s32 (*)(void*))(*(void***)arg2)[0xE0 / 4])(arg2);
                if (e0 == 1 || e0 == 2) r30 = 1;
            } else {
                if (((s32 (*)(void*))(*(void***)arg2)[0xE0 / 4])(arg2) == 1) r30 = 1;
            }
            break;
        }
        case 0x01:
            if (((s32 (*)(void*))(*(void***)arg2)[0xE0 / 4])(arg2) == 3) r30 = 1;
            break;
        case 0x02:
            if (*(u32*)((u8*)move + 0x74) & 0x1000000) r30 = 1;
            break;
        case 0x03:
            if (*(u32*)((u8*)move + 0x74) & 0x4000000) r30 = 1;
            break;
        case 0x04:
            if (*(u32*)((u8*)move + 0x74) & 0x2000000) r30 = 1;
            break;
        case 0x05:
            if (func_80148778((u8*)arg1 + 8, 0xC1)) r30 = 1;
            break;
        case 0x06:
            if (func_80148778((u8*)arg2 + 8, 0x10)) r30 = 1;
            break;
        case 0x07:
            if (func_80148778((u8*)arg2 + 8, 0x0F)) r30 = 1;
            break;
        case 0x08: {
            void* m = (u8*)arg2 + 0x3E9C;
            void* r = ((void* (*)(void*))(*(void***)m)[0x4C / 4])(m);
            if (r == *(void**)((u8*)arg1 + 0x3F10)) r30 = 1;
            break;
        }
        case 0x09:
            if (*(f32*)((u8*)move + 0x5C) >= ((f32 (*)(void*))(*(void***)arg2)[0x128 / 4])(arg2)) r30 = 1;
            break;
        case 0x0A:
            if (*(u8*)((u8*)subObj + 0x67) != 0) r30 = 1;
            break;
        case 0x0B:
            if (((u16)func_8016DF2C()) == (u16)(type - 0x0A)) r30 = 1;
            break;
        case 0x0C: {
            u32 m74 = *(u32*)((u8*)move + 0x74);
            if (m74 & 0x4000000) {
                s32 e0 = ((s32 (*)(void*))(*(void***)arg2)[0xE0 / 4])(arg2);
                if (e0 == 1 || e0 == 2) r30 = 1;
            }
            break;
        }
        case 0x0D:
            if (((f32 (*)(void*))(*(void***)arg2)[0x130 / 4])(arg2) < lbl_eu_80666E24) r30 = 1;
            break;
        default:
            break;
        }
    }

done1744_1B8C:;
    if (*(u32*)((u8*)subObj + 0x78) & 0x1000) {
        if (func_800D7D24((D7D24_Obj*)move) == 0) r30 = 0;
    }
    if (!(*(u32*)((u8*)move + 0x74) & 0x10)) {
        *(f32*)((u8*)move + 0x5C) = *(f32*)((u8*)move + 0x54) * *(f32*)((u8*)move + 0x58);
    }

    if (r30 == 0) goto postEmit;

    // ---- event emission (dispatch on subObj->0x67) ----
    E08E8_EventData ev;
    std::memset(&ev, 0, sizeof(ev));
    u8 emitType = *(u8*)((u8*)subObj + 0x67);
    switch (emitType) {
    case 0x03: {
        if (*(u32*)((u8*)move + 0x78) & 0x10) break;
        cf::CfActorF64Conv conv;
        conv.w[0] = 0x43300000;
        s16 hp = *(s16*)((u8*)subObj + 0x68);
        conv.w[1] = (u32)(hp - 1) ^ 0x80000000;
        f32 add = (f32)(conv.d - lbl_eu_80666DE0);
        *(f32*)((u8*)move + 0x58) = *(f32*)((u8*)move + 0x58) + add;
        *(f32*)((u8*)move + 0x5C) = *(f32*)((u8*)move + 0x54) * *(f32*)((u8*)move + 0x58);
        if (((void* (*)(void*))(*(void***)arg1)[0x290 / 4])(arg1) != 0) {
            void* p = ((void* (*)(void*))(*(void***)arg1)[0x290 / 4])(arg1);
            if (func_8026178C(p, 0x9C) != 0) {
                if (*(u8*)((u8*)subObj + 0x66) == 3) {
                    *(f32*)((u8*)move + 0x58) = *(f32*)((u8*)move + 0x58) + 1.0f;
                }
            }
        }
        break;
    }
    case 0x07:
        ev.v00 = *(u32*)((u8*)arg1 + 0x3F10);
        ev.v04 = (u32)(uintptr_t)subObj;
        ev.v0C = 0x10;
        ev.v10 = 0;
        {
            cf::CfActorF64Conv conv;
            conv.w[0] = 0x43300000;
            conv.w[1] = (u32)(*(s16*)((u8*)subObj + 0x68)) ^ 0x80000000;
            ev.v20 = (f32)(conv.d - lbl_eu_80666DE0);
        }
        ev.v24 = lbl_eu_80666DDC;
        if (*(u32*)((u8*)subObj + 0x78) & 0x1000) {
            func_800EAA2C(self, arg1, arg1, &ev, move);
        } else {
            func_800EAA2C(self, arg1, arg2, &ev, move);
        }
        break;
    case 0x08:
        ev.v00 = *(u32*)((u8*)arg1 + 0x3F10);
        ev.v04 = (u32)(uintptr_t)subObj;
        ev.v0C = 0x0F;
        ev.v10 = 0;
        {
            cf::CfActorF64Conv conv;
            conv.w[0] = 0x43300000;
            conv.w[1] = (u32)(*(s16*)((u8*)subObj + 0x68)) ^ 0x80000000;
            ev.v20 = (f32)(conv.d - lbl_eu_80666DE0);
        }
        ev.v24 = lbl_eu_80666DDC;
        if (*(u32*)((u8*)subObj + 0x78) & 0x1000) {
            func_800EAA2C(self, arg1, arg1, &ev, move);
        } else {
            func_800EAA2C(self, arg1, arg2, &ev, move);
        }
        break;
    case 0x0B: {
        ev.v00 = *(u32*)((u8*)arg1 + 0x3F10);
        ev.v04 = (u32)(uintptr_t)subObj;
        ev.v0C = 0x10;
        ev.v10 = 3;
        ev.v20 = lbl_eu_80666E68;
        ev.v24 = lbl_eu_80666DDC;
        EnumListHolder holder;
        func_80043D90(&holder);
        func_800F4A98(func_80043F18(&holder), 0x8000, 0);
        for (u32 i = 0; i < *(u32*)((u8*)func_80043F18(&holder) + 0x620); i++) {
            void* o = func_8016FE34(func_800F6EAC(func_80043F18(&holder), i));
            if (o != arg2) func_800EAA2C(self, arg1, o, &ev, move);
        }
        __dt__80043E88(&holder, -1);
        break;
    }
    case 0x0D:
        ev.v00 = *(u32*)((u8*)arg1 + 0x3F10);
        ev.v04 = (u32)(uintptr_t)subObj;
        ev.v0C = 0x10;
        ev.v10 = 3;
        ev.v20 = lbl_eu_80666E68;
        ev.v24 = lbl_eu_80666DDC;
        func_800EAA2C(self, arg1, arg2, &ev, move);
        break;
    case 0x06:
        ev.v00 = *(u32*)((u8*)arg1 + 0x3F10);
        ev.v04 = (u32)(uintptr_t)subObj;
        ev.v0C = 0xDC;
        ev.v10 = *(s16*)((u8*)subObj + 0x68);
        if (*(u32*)((u8*)subObj + 0x78) & 0x1000) {
            func_800EAA2C(self, arg1, arg1, &ev, move);
        } else {
            func_800EAA2C(self, arg1, arg2, &ev, move);
        }
        break;
    case 0x01: {
        ev.v00 = *(u32*)((u8*)arg1 + 0x3F10);
        ev.v04 = (u32)(uintptr_t)subObj;
        ev.v0C = *(s16*)((u8*)subObj + 0x68);
        ev.v10 = *(s16*)((u8*)subObj + 0x6A);
        void* subVt = *(void**)((u8*)subObj + 0x84);
        s32 sv = ((s32 (*)(void*))(*(void**)((u8*)subVt + 0x0C)))(subObj) - 1;
        cf::CfActorF64Conv conv;
        conv.w[0] = 0x43300000;
        conv.w[1] = (u32)(u8)(*(u8*)((u8*)subObj + 0x6E)) ^ 0x80000000;
        f32 f1 = (f32)(conv.d - lbl_eu_80666E08);
        conv.w[1] = (u32)sv ^ 0x80000000;
        f32 f2 = (f32)(conv.d - lbl_eu_80666DE0);
        ev.v20 = *(f32*)((u8*)subObj + 0x50) + f1 / 10.0f * f2;
        ev.v24 = *(f32*)((u8*)subObj + 0x54);
        if (*(u32*)((u8*)subObj + 0x78) & 0x1000) {
            func_800EAA2C(self, arg1, arg1, &ev, move);
        } else {
            func_800EAA2C(self, arg1, arg2, &ev, move);
        }
        break;
    }
    default:
        break;
    }

postEmit:
    // Recompute the multiplier after emission.
    if (!(*(u32*)((u8*)move + 0x74) & 0x10)) {
        *(f32*)((u8*)move + 0x5C) = *(f32*)((u8*)move + 0x54) * *(f32*)((u8*)move + 0x58);
    }

    // Second sub-object event block (+0x48 id).
    if (!(*(u32*)((u8*)move + 0x74) & 0x08) || (*(u32*)((u8*)subObj + 0x78) & 0x400)) {
        if (!(*(u32*)((u8*)subObj + 0x78) & 0x2000) ||
            (*(u8*)((u8*)subObj + 0x44) ==
             (u8)(((s32 (*)(void*))(*(void***)arg1)[0x2A8 / 4])(arg1) + 1))) {
            if (*(u16*)((u8*)subObj + 0x48) != 0) {
                E08E8_EventData ev2;
                std::memset(&ev2, 0, sizeof(ev2));
                ev2.v00 = *(u32*)((u8*)arg1 + 0x3F10);
                ev2.v04 = (u32)(uintptr_t)subObj;
                ev2.v0C = *(u16*)((u8*)subObj + 0x48);
                void* subVt2 = *(void**)((u8*)subObj + 0x84);
                s32 sv2 = ((s32 (*)(void*))(*(void**)((u8*)subVt2 + 0x0C)))(subObj) - 1;
                ev2.v10 = *(s16*)((u8*)subObj + 0x4A) + (u32)(*(u8*)((u8*)subObj + 0x6F)) * (u32)sv2;
                ev2.v14 = (u16)*(s16*)((u8*)subObj + 0x4C);
                cf::CfActorF64Conv conv;
                conv.w[0] = 0x43300000;
                conv.w[1] = (u32)(u8)(*(u8*)((u8*)subObj + 0x6E)) ^ 0x80000000;
                f32 f1 = (f32)(conv.d - lbl_eu_80666E08);
                conv.w[1] = (u32)sv2 ^ 0x80000000;
                f32 f2 = (f32)(conv.d - lbl_eu_80666DE0);
                ev2.v20 = *(f32*)((u8*)subObj + 0x50) + f1 / 10.0f * f2;
                ev2.v24 = *(f32*)((u8*)subObj + 0x54);
                if (*(u32*)((u8*)subObj + 0x78) & 0x400) {
                    func_800EAA2C(self, arg1, arg1, &ev2, move);
                } else {
                    func_800EAA2C(self, arg1, arg2, &ev2, move);
                }
            }
        }
    }

    // Counter damage gate: protect against hitting an already-active enemy.
    {
        s32 ival = 0;
        f32 sum = *(f32*)((u8*)move + 0x5C) + *(f32*)((u8*)move + 0x60);
        if (sum >= 0.0f && !(*(u32*)((u8*)move + 0x74) & 0x80)) {
            ival = (s32)((f64)sum + (sum <= 0.0f ? lbl_eu_80666E60 : lbl_eu_80666E58));
        }
        if (ival > 0) {
            if (!(*(u32*)((u8*)move + 0x74) & 0x10)) {
                if (func_80148778((u8*)arg2 + 8, 0xF7)) {
                    if (((s32 (*)(void*, void*))(*(void***)arg2)[0x5C0 / 4])(arg2, arg1) == 0) {
                        u32 m78 = *(u32*)((u8*)move + 0x78);
                        if ((m78 & 0x200) || ((m78 & 0x400) && !(m78 & 0x800))) {
                            *(u32*)((u8*)move + 0x74) |= 0x80000080;
                        }
                    }
                }
            }

            // X/Y gate: both the move and the sub-state must agree.
            u32 g74 = *(u32*)((u8*)move + 0x74);
            if (g74 & 0x8000) {
                if (*(u32*)((u8*)move + 0x78) & 0x8000) goto counterHit;
            } else {
                if (*(u32*)((u8*)move + 0x78) & 0x8000) {
                    if (g74) goto counterHit;
                } else {
                    goto counterHit;
                }
            }
            // (fallthrough = blocked)
            if (g74 & 0x8000) {
                if (!(*(u32*)((u8*)move + 0x78) & 0x8000)) goto counterHit;
            } else {
                if (*(u32*)((u8*)move + 0x78) & 0x8000) goto counterHit;
                goto blocked;
            }
        blocked:
            goto afterCounter;
        counterHit:
            // 0x33 status: per-hit counter bleed from the move block.
            if (!func_80148778((u8*)arg2 + 8, 0x33)) goto afterCounter;
            void* entry = func_80149154((u8*)arg2 + 8, 0x33);
            if (entry == nullptr) goto afterCounter;

            if (!(*(u32*)((u8*)move + 0x78) & 0x4000000)) {
                s32 steps = *(s16*)((u8*)entry + 0x14);
                if (steps <= 0) goto afterCounter;
                if (*(s16*)((u8*)move + 0x72) == -1) goto afterCounter;
                if (*(s16*)((u8*)move + 0x72) == 0) {
                    *(s16*)((u8*)move + 0x72) = (s16)steps;
                }
                s32 ival2 = 0;
                f32 sum2 = *(f32*)((u8*)move + 0x5C) + *(f32*)((u8*)move + 0x60);
                if (sum2 >= 0.0f && !(*(u32*)((u8*)move + 0x74) & 0x80)) {
                    ival2 = (s32)((f64)sum2 + (sum2 <= 0.0f ? lbl_eu_80666E60 : lbl_eu_80666E58));
                }
                *(u32*)((u8*)move + 0x74) |= 0x80008000;
                cf::CfActorF64Conv conv;
                conv.w[0] = 0x43300000;
                conv.w[1] = (u32)(s16)(*(s16*)((u8*)move + 0x72)) ^ 0x80000000;
                f32 f72 = (f32)(conv.d - lbl_eu_80666DE0);
                conv.w[1] = (u32)ival2 ^ 0x80000000;
                f32 fiv = (f32)(conv.d - lbl_eu_80666DE0);
                *(f32*)((u8*)move + 0x64) = fiv;
                f32 sum3 = *(f32*)((u8*)move + 0x5C) - f72 + *(f32*)((u8*)move + 0x60);
                *(f32*)((u8*)move + 0x5C) = *(f32*)((u8*)move + 0x5C) - f72;
                s32 ival3 = 0;
                if (sum3 >= 0.0f && !(*(u32*)((u8*)move + 0x74) & 0x80)) {
                    ival3 = (s32)((f64)sum3 + (sum3 <= 0.0f ? lbl_eu_80666E60 : lbl_eu_80666E58));
                }
                if (ival3 > 0) {
                    *(s16*)((u8*)move + 0x72) = -1;
                    *(u32*)((u8*)move + 0x78) |= 0x40008000;
                } else {
                    f32 neg = -*(f32*)((u8*)move + 0x5C);
                    *(s16*)((u8*)move + 0x72) = (s16)(s32)neg;
                }
            } else {
                s32 steps = *(s16*)((u8*)entry + 0x14);
                if (steps <= 0) goto afterCounter;
                s32 ival = 0;
                f32 sum = *(f32*)((u8*)move + 0x5C) + *(f32*)((u8*)move + 0x60);
                if (sum >= 0.0f && !(*(u32*)((u8*)move + 0x74) & 0x80)) {
                    ival = (s32)((f64)sum + (sum <= 0.0f ? lbl_eu_80666E60 : lbl_eu_80666E58));
                }
                *(u32*)((u8*)move + 0x74) |= 0x80008000;
                cf::CfActorF64Conv conv;
                conv.w[0] = 0x43300000;
                conv.w[1] = (u32)ival ^ 0x80000000;
                *(f32*)((u8*)move + 0x64) = (f32)(conv.d - lbl_eu_80666DE0);
                conv.w[1] = (u32)(s16)steps ^ 0x80000000;
                f32 fs = (f32)(conv.d - lbl_eu_80666DE0);
                *(f32*)((u8*)move + 0x5C) = *(f32*)((u8*)move + 0x5C) - fs;
            }
        }
    }

afterCounter:
    // Arts-type 3 dot/regeneration block.
    if (*(u16*)((u8*)subObj + 0x3C) == 3) {
        s32 ival = 0;
        f32 sum = *(f32*)((u8*)move + 0x5C) + *(f32*)((u8*)move + 0x60);
        if (sum >= 0.0f && !(*(u32*)((u8*)move + 0x74) & 0x80)) {
            ival = (s32)((f64)sum + (sum <= 0.0f ? lbl_eu_80666E60 : lbl_eu_80666E58));
        }
        if (ival > 0) {
            if (!(*(u32*)((u8*)move + 0x78) & 0x10)) {
                cf::CfActorF64Conv conv;
                conv.w[0] = 0x43300000;
                conv.w[1] = (u32)ival ^ 0x80000000;
                f32 fv = (f32)(conv.d - lbl_eu_80666DE0);
                *(f32*)((u8*)move + 0x68) = *(f32*)((u8*)move + 0x68) - fv;
            }
            if (!(*(u32*)((u8*)move + 0x78) & 0x1000)) {
                if (((s32 (*)(void*))(*(void***)arg2)[0x2BC / 4])(arg2) == 0) {
                    // 0x92 proc-rate block.
                    if (func_80148778((u8*)arg2 + 8, 0x92)) {
                        s32 bonus = 0;
                        if (func_80148778((u8*)arg2 + 8, 0xA1)) {
                            void* eA1 = func_80149154((u8*)arg2 + 8, 0xA1);
                            bonus = *(s32*)((u8*)eA1 + 0x10);
                        }
                        void* e92 = func_80149154((u8*)arg2 + 8, 0x92);
                        if (e92 != nullptr && *(f32*)((u8*)move + 0x68) <= 0.0f) {
                            u32 need = *(u32*)((u8*)e92 + 0x10);
                            if (*(u32*)((u8*)e92 + 0x08) == 0x2000) need += bonus;
                            if (need > *(u32*)((u8*)move + 0xA8) % 100) {
                                *(u32*)((u8*)move + 0x74) |= 0xA0000000;
                                *(f32*)((u8*)move + 0x68) = lbl_eu_80666DD4;
                            }
                        }
                    }
                    if (!(*(u32*)((u8*)move + 0x78) & 0x2000)) {
                        // 0xFC / 0x100 fixed-rate bleeds.
                        if (func_80148778((u8*)arg2 + 8, 0xFC)) {
                            void* eFC = func_80149154((u8*)arg2 + 8, 0xFC);
                            if (eFC != nullptr && *(f32*)((u8*)move + 0x68) <= 0.0f) {
                                *(u32*)((u8*)move + 0x78) |= 0x40002000;
                                ((f32 (*)(void*))(*(void***)arg2)[0x12C / 4])(arg2);
                                *(f32*)((u8*)move + 0x68) =
                                    *(f32*)((u8*)move + 0x68) * (f32)(*(s32*)((u8*)eFC + 0x10)) * lbl_eu_80666DD8;
                            }
                        }
                        if (func_80148778((u8*)arg2 + 8, 0x100)) {
                            void* e100 = func_80149154((u8*)arg2 + 8, 0x100);
                            if (e100 != nullptr && *(f32*)((u8*)move + 0x68) <= 0.0f) {
                                *(u32*)((u8*)move + 0x78) |= 0x40002000;
                                ((f32 (*)(void*))(*(void***)arg2)[0x12C / 4])(arg2);
                                *(f32*)((u8*)move + 0x68) =
                                    *(f32*)((u8*)move + 0x68) * (f32)(*(s32*)((u8*)e100 + 0x10)) * lbl_eu_80666DD8;
                            }
                        }
                        // 0x36 growth block.
                        if (func_80148778((u8*)arg2 + 8, 0x36)) {
                            void* e36 = func_80149154((u8*)arg2 + 8, 0x36);
                            if (e36 != nullptr && *(f32*)((u8*)move + 0x68) > 0.0f) {
                                s32 v36 = *(s32*)((u8*)e36 + 0x10);
                                func_800D81A8((void*)0, arg2, (void*)0);
                                cf::CfActorF64Conv conv;
                                conv.w[0] = 0x43300000;
                                conv.w[1] = (u32)v36 ^ 0x80000000;
                                f32 fv = (f32)(conv.d - lbl_eu_80666DE0);
                                f32 prod = fv * *(f32*)((u8*)move + 0x68);
                                f32 rnd = (f32)(s32)prod;
                                conv.w[1] = (u32)(s32)rnd ^ 0x80000000;
                                *(f32*)((u8*)move + 0x68) =
                                    *(f32*)((u8*)move + 0x68) + (f32)(conv.d - lbl_eu_80666DE0);
                            }
                        }
                    }
                    if (!(*(u32*)((u8*)move + 0x78) & 0x4000)) {
                        // 0x11E protection block.
                        if (func_80148778((u8*)arg2 + 8, 0x11E)) {
                            void* e11E = func_80149154((u8*)arg2 + 8, 0x11E);
                            if (e11E != nullptr && *(f32*)((u8*)move + 0x68) > 0.0f) {
                                f32 f68 = *(f32*)((u8*)move + 0x68);
                                ((f32 (*)(void*))(*(void***)arg2)[0x12C / 4])(arg2);
                                s32 s14 = *(s16*)((u8*)e11E + 0x14);
                                cf::CfActorF64Conv conv;
                                conv.w[0] = 0x43300000;
                                conv.w[1] = (u32)s14 ^ 0x80000000;
                                f32 fs = (f32)(conv.d - lbl_eu_80666DE0);
                                s32 rnd = (s32)(fs * f68 / lbl_eu_80666E00);
                                conv.w[1] = (u32)rnd ^ 0x80000000;
                                f32 fadd = (f32)(conv.d - lbl_eu_80666DE0);
                                *(f32*)((u8*)move + 0x68) = *(f32*)((u8*)move + 0x68) + fadd;
                                if (*(s32*)((u8*)e11E + 0x10) > 0) {
                                    if (*(s16*)((u8*)move + 0x70) == 0) {
                                        *(s16*)((u8*)move + 0x70) = (s16)*(s32*)((u8*)e11E + 0x10);
                                    }
                                    s32 tick = *(s16*)((u8*)move + 0x70) - 1;
                                    *(s16*)((u8*)move + 0x70) = (s16)tick;
                                    if (tick <= 0) {
                                        *(s16*)((u8*)move + 0x70) = 0;
                                        *(u32*)((u8*)move + 0x78) |= 0x40004000;
                                    }
                                }
                            }
                        }
                        // 0x37 proc-rate block (same shape as 0x92).
                        if (func_80148778((u8*)arg2 + 8, 0x37)) {
                            s32 bonus = 0;
                            if (func_80148778((u8*)arg2 + 8, 0xA1)) {
                                void* eA1 = func_80149154((u8*)arg2 + 8, 0xA1);
                                bonus = *(s32*)((u8*)eA1 + 0x10);
                            }
                            void* e37 = func_80149154((u8*)arg2 + 8, 0x37);
                            if (e37 != nullptr && *(f32*)((u8*)move + 0x68) > 0.0f) {
                                s32 need = *(s16*)((u8*)e37 + 0x1A);
                                if (*(u32*)((u8*)e37 + 0x08) == 0x2000) need += bonus;
                                u32 r25 = *(s32*)((u8*)e37 + 0x10);
                                func_800D81A8((void*)0, arg2, (void*)0);
                                cf::CfActorF64Conv conv;
                                conv.w[0] = 0x43300000;
                                conv.w[1] = (u32)r25 ^ 0x80000000;
                                f32 fv = (f32)(conv.d - lbl_eu_80666DE0);
                                f32 prod = fv * *(f32*)((u8*)move + 0x68);
                                f32 rnd = (f32)(s32)prod;
                                conv.w[1] = (u32)(s32)rnd ^ 0x80000000;
                                *(f32*)((u8*)move + 0x68) =
                                    *(f32*)((u8*)move + 0x68) + (f32)(conv.d - lbl_eu_80666DE0);
                            }
                        }
                    }
                }
            }
        }
    }

    // Final clamp: move stays active until the accumulated dot drains.
    if (*(f32*)((u8*)move + 0x68) <= 0.0f) {
        *(u32*)((u8*)move + 0x78) |= 0x40001000;
    }
}

// func_800E08E8 (retail 0x800E13D0, 0x125C bytes): battle-event emission and
// move-state update shared by the move-type dispatchers. Resolves the event
// source via the 0xF7 status / 0x5C0 self-check, runs the arts-type jump
// table, emits events through func_800EAA2C, then applies counter / dot /
// proc-rate damage math on the move block.
void func_800E08E8(void* self_, void* attacker_, void* target_, void* move_) {
    // Two int->float conversion slots (Gekko 2^52-bias trick). Only the
    // sign-flipped low words change per conversion; the 0x43300000 high words
    // are written once up front and both slots stay live for the whole body.
    cf::CfActorF64Conv cvtA;
    cvtA.w[0] = 0x43300000;

    void* self = self_;          // r26
    void* attacker = attacker_;  // r27
    void* target = target_;      // r28

    cf::CfActorF64Conv cvtB;
    cvtB.w[0] = 0x43300000;

    E08E8_Move* move = (E08E8_Move*)move_;   // r29

    if (target == nullptr) return;

    if (!(move->w74 & 0x08)) {
        if (func_80148778(&((E08E8_Actor*)target)->statusBase, 0xF7)) {
            if (((E08E8_Vt*)target)->v5C0(attacker) == 0) {
                func_80149154(&((E08E8_Actor*)target)->statusBase, 0xF7);
                if ((move->w78 & 0x200) ||
                    ((move->w78 & 0x400) && !(move->w78 & 0x800))) {
                    move->w74 |= 0x80000080;
                }
            }
        }
    }

    move->f60 = lbl_eu_80666DDC;
    E08E8_SubObj* subObj = (E08E8_SubObj*)move->subObj;   // r31
    move->f64 = lbl_eu_80666DDC;
    ((E08E8_Vt*)attacker)->v224();
    ((E08E8_Vt*)target)->v224();

    s32 hit;
    if (move->w74 & 0x08) {
        if (!(subObj->w78 & 0x800)) goto lateBlock;
    }
    hit = 0;
    if (subObj->w78 & 0x2000) {
        if (subObj->b44 != (u8)(((E08E8_Vt*)attacker)->v2A8() + 1)) {
            goto lateBlock;
        }
    }

    // Arts-type jump table (byte +0x66 of the sub-object).
    switch (subObj->b66) {
    case 0x00:
        if (((E08E8_Actor*)attacker)->w3374 & 0x8000) {
            s32 e0 = ((E08E8_Vt*)target)->vE0();
            if (e0 == 1 || e0 == 2) hit = 1;
        } else {
            if (((E08E8_Vt*)target)->vE0() == 1) hit = 1;
        }
        break;
    case 0x01:
        if (((E08E8_Vt*)target)->vE0() == 3) hit = 1;
        break;
    case 0x02:
        if (move->w74 & 0x01000000) hit = 1;
        break;
    case 0x03:
        if (move->w74 & 0x04000000) hit = 1;
        break;
    case 0x04:
        if (move->w74 & 0x02000000) hit = 1;
        break;
    case 0x05:
        if (func_80148778(&((E08E8_Actor*)attacker)->statusBase, 0xC1)) hit = 1;
        break;
    case 0x06:
        if (func_80148778(&((E08E8_Actor*)target)->statusBase, 0x10)) hit = 1;
        break;
    case 0x07:
        if (func_80148778(&((E08E8_Actor*)target)->statusBase, 0x0F)) hit = 1;
        break;
    case 0x08:
        if (((E08E8_MoveSubVtIf*)&((E08E8_Actor*)target)->moveSub)->v4C() ==
            ((E08E8_Actor*)attacker)->ptr3F10) {
            hit = 1;
        }
        break;
    case 0x09:
        if (move->f5C >= ((E08E8_Vt*)target)->v128()) hit = 1;
        break;
    case 0x0A:
        if (subObj->b67 != 0) hit = 1;
        break;
    case 0x0B:
        if ((u16)func_8016DF2C() == (u16)(subObj->b66 - 0x0A)) hit = 1;
        break;
    case 0x0C:
        if (move->w74 & 0x04000000) {
            s32 e0 = ((E08E8_Vt*)target)->vE0();
            if (e0 == 1 || e0 == 2) hit = 1;
        }
        break;
    case 0x0D:
        if (((E08E8_Vt*)target)->v130() < lbl_eu_80666E24) hit = 1;
        break;
    default:
        break;
    }

postSwitch:
    if (subObj->w78 & 0x800) {
        if (func_800D7D24((void*)move) == 0) hit = 0;
    }
    if (!(move->w74 & 0x10000000)) {
        move->f5C = move->f54 * move->f58;
    }

    if (hit == 0) goto lateBlock;

    {
        // ---- event emission (dispatch on subObj->0x67) ----
        E08E8_EventData ev;
        std::memset(&ev, 0, sizeof(ev));
        switch (subObj->b67) {
        case 0x03: {
            // HP adjustment path: no event broadcast, just the move update.
            if (move->w78 & 0x10) break;
            cvtA.w[1] = (u32)(subObj->s68 - 1) ^ 0x80000000;
            move->f58 = move->f58 + (f32)(cvtA.d - lbl_eu_80666DE0);
            move->f5C = move->f54 * move->f58;
            if (((E08E8_Vt*)attacker)->v290() != 0) {
                void* chain = (void*)((E08E8_Vt*)attacker)->v290();
                if (func_8026178C(chain, 0x9C) != 0) {
                    if (subObj->b66 == 3) {
                        move->f58 = move->f58 + lbl_eu_80666DD4;
                    }
                }
            }
            break;
        }
        case 0x07:
            ev.v00 = (u32)((E08E8_Actor*)attacker)->ptr3F10;
            ev.v04 = (u32)subObj;
            ev.v0C = 0x10;
            ev.v10 = 0;
            ev.v24 = lbl_eu_80666DDC;
            cvtB.w[1] = (u32)subObj->s68 ^ 0x80000000;
            ev.v20 = (f32)(cvtB.d - lbl_eu_80666DE0);
            if (subObj->w78 & 0x1000) {
                func_800EAA2C(self, attacker, attacker, &ev, move);
            } else {
                func_800EAA2C(self, attacker, target, &ev, move);
            }
            break;
        case 0x08:
            ev.v00 = (u32)((E08E8_Actor*)attacker)->ptr3F10;
            ev.v04 = (u32)subObj;
            ev.v0C = 0x0F;
            ev.v10 = 0;
            ev.v24 = lbl_eu_80666DDC;
            cvtB.w[1] = (u32)subObj->s68 ^ 0x80000000;
            ev.v20 = (f32)(cvtB.d - lbl_eu_80666DE0);
            if (subObj->w78 & 0x1000) {
                func_800EAA2C(self, attacker, attacker, &ev, move);
            } else {
                func_800EAA2C(self, attacker, target, &ev, move);
            }
            break;
        case 0x0B: {
            // Broadcast to every enumerated actor except the target.
            ev.v00 = (u32)((E08E8_Actor*)attacker)->ptr3F10;
            ev.v04 = (u32)subObj;
            ev.v0C = 0x10;
            ev.v10 = 3;
            ev.v20 = lbl_eu_80666E68;
            ev.v24 = lbl_eu_80666DDC;
            EnumListHolder holder;
            func_80043D90(&holder);
            func_800F4A98(func_80043F18(&holder), 0x8000, 0);
            for (u32 i = 0; i < ((cf::CVisionEnumList*)func_80043F18(&holder))->count; i++) {
                void* o = func_8016FE34(func_800F6EAC(func_80043F18(&holder), i));
                if (o != target) {
                    func_800EAA2C(self, attacker, o, &ev, move);
                }
            }
            __dt__80043E88(&holder, -1);
            break;
        }
        case 0x0D:
            ev.v00 = (u32)((E08E8_Actor*)attacker)->ptr3F10;
            ev.v04 = (u32)subObj;
            ev.v0C = 0x10;
            ev.v10 = 3;
            ev.v20 = lbl_eu_80666E68;
            ev.v24 = lbl_eu_80666DDC;
            func_800EAA2C(self, attacker, target, &ev, move);
            break;
        case 0x06:
            ev.v00 = (u32)((E08E8_Actor*)attacker)->ptr3F10;
            ev.v04 = (u32)subObj;
            ev.v0C = 0xDC;
            ev.v10 = (u32)subObj->s68;
            if (subObj->w78 & 0x1000) {
                func_800EAA2C(self, attacker, attacker, &ev, move);
            } else {
                func_800EAA2C(self, attacker, target, &ev, move);
            }
            break;
        case 0x01: {
            ev.v00 = (u32)((E08E8_Actor*)attacker)->ptr3F10;
            ev.v04 = (u32)subObj;
            ev.v0C = (u16)subObj->s68;
            ev.v10 = (u32)subObj->s6A;
            s32 sv = ((E08E8_SubVtIf*)subObj->vt84)->v0C() - 1;
            cvtA.w[1] = (u32)subObj->b6E ^ 0x80000000;
            f32 t1 = (f32)(cvtA.d - lbl_eu_80666E08);
            cvtB.w[1] = (u32)sv ^ 0x80000000;
            f32 t2 = (f32)(cvtB.d - lbl_eu_80666DE0);
            ev.v20 = subObj->f50 + t1 / lbl_eu_80666E34 * t2;
            ev.v24 = subObj->f54;
            if (subObj->w78 & 0x1000) {
                func_800EAA2C(self, attacker, attacker, &ev, move);
            } else {
                func_800EAA2C(self, attacker, target, &ev, move);
            }
            break;
        }
        default:
            break;
        }
    }

lateBlock:
    if (!(move->w74 & 0x10000000)) {
        move->f5C = move->f54 * move->f58;
    }

    // Second sub-object event block (+0x48 id).
    if (move->w74 & 0x08) {
        if (!(subObj->w78 & 0x400)) goto tail2;
    }
    if (subObj->w78 & 0x2000) {
        if (subObj->b44 != (u8)(((E08E8_Vt*)attacker)->v2A8() + 1)) goto tail2;
    }
    if (subObj->u48 == 0) goto tail2;

    {
        E08E8_EventData ev2;
        std::memset(&ev2, 0, sizeof(ev2));
        ev2.v00 = (u32)((E08E8_Actor*)attacker)->ptr3F10;
        ev2.v04 = (u32)subObj;
        ev2.v0C = subObj->u48;
        s32 sv2 = ((E08E8_SubVtIf*)subObj->vt84)->v0C() - 1;
        ev2.v10 = (u32)subObj->s4A + (u32)subObj->b6F * (u32)sv2;
        ev2.v14 = (u16)subObj->s4C;
        cvtA.w[1] = (u32)subObj->b6E ^ 0x80000000;
        f32 u1 = (f32)(cvtA.d - lbl_eu_80666E08);
        cvtB.w[1] = (u32)sv2 ^ 0x80000000;
        f32 u2 = (f32)(cvtB.d - lbl_eu_80666DE0);
        ev2.v20 = subObj->f50 + u1 / lbl_eu_80666E34 * u2;
        ev2.v24 = subObj->f54;
        if (subObj->w78 & 0x400) {
            func_800EAA2C(self, attacker, attacker, &ev2, move);
        } else {
            func_800EAA2C(self, attacker, target, &ev2, move);
        }
    }

tail2:
    // Counter damage gate: protect against hitting an already-active enemy.
    {
        s32 cnt = 0;
        f32 csum = move->f5C + move->f60;
        if (csum >= 0.0f && !(move->w74 & 0x80)) {
            cnt = (s32)((f64)csum + (csum <= 0.0f ? lbl_eu_80666E60 : lbl_eu_80666E58));
        }
        if (cnt > 0) {
            if (!(move->w74 & 0x10000000)) {
                if (func_80148778(&((E08E8_Actor*)target)->statusBase, 0xF7)) {
                    if (((E08E8_Vt*)target)->v5C0(attacker) == 0) {
                        if ((move->w78 & 0x200) ||
                            ((move->w78 & 0x400) && !(move->w78 & 0x800))) {
                            move->w74 |= 0x80000080;
                        }
                    }
                }
            }
            if ((move->w74 & 0x8000) || !(move->w78 & 0x8000)) {
                if (!func_80148778(&((E08E8_Actor*)target)->statusBase, 0x33)) {
                    goto dotBlock;
                }
                E08E8_StatusEntry* entry = (E08E8_StatusEntry*)func_80149154(
                    &((E08E8_Actor*)target)->statusBase, 0x33);
                if (entry == nullptr) goto dotBlock;

                if (move->w78 & 0x04000000) {
                    // per-hit counter bleed tracked in move->s72
                    s16 steps = entry->s14;
                    if (steps <= 0) goto dotBlock;
                    if (move->s72 == -1) goto dotBlock;
                    if (move->s72 == 0) {
                        move->s72 = steps;
                    }
                    s32 cnt2 = 0;
                    f32 s2 = move->f5C + move->f60;
                    if (s2 >= 0.0f && !(move->w74 & 0x80)) {
                        cnt2 = (s32)((f64)s2 + (s2 <= 0.0f ? lbl_eu_80666E60 : lbl_eu_80666E58));
                    }
                    cvtB.w[1] = (u32)cnt2 ^ 0x80000000;
                    cvtA.w[1] = (u32)(s16)move->s72 ^ 0x80000000;
                    move->f64 = (f32)(cvtB.d - lbl_eu_80666DE0);
                    f32 dec = move->f5C - (f32)(cvtA.d - lbl_eu_80666DE0);
                    f32 news = dec + move->f60;
                    move->f5C = dec;
                    move->w74 |= 0x80008000;
                    s32 cnt3 = 0;
                    if (news >= 0.0f && !(move->w74 & 0x80)) {
                        cnt3 = (s32)((f64)news + (news <= 0.0f ? lbl_eu_80666E60 : lbl_eu_80666E58));
                    }
                    if (cnt3 > 0) {
                        move->s72 = -1;
                        move->w78 |= 0x40008000;
                    } else {
                        move->s72 = (s16)(s32)-move->f5C;
                    }
                } else {
                    s16 steps = entry->s14;
                    if (steps <= 0) goto dotBlock;
                    s32 cntB = 0;
                    f32 sB = move->f5C + move->f60;
                    if (sB >= 0.0f && !(move->w74 & 0x80)) {
                        cntB = (s32)((f64)sB + (sB <= 0.0f ? lbl_eu_80666E60 : lbl_eu_80666E58));
                    }
                    cvtB.w[1] = (u32)cntB ^ 0x80000000;
                    move->f64 = (f32)(cvtB.d - lbl_eu_80666DE0);
                    cvtA.w[1] = (u32)steps ^ 0x80000000;
                    move->w74 |= 0x80008000;
                    move->f5C = move->f5C - (f32)(cvtA.d - lbl_eu_80666DE0);
                }
            }
        }
    }

dotBlock:
    {
        // Arts-type 3 dot/regeneration block.
        if (subObj->u3C != 3) return;
        s32 dv = 0;
        f32 ds = move->f5C + move->f60;
        if (ds >= 0.0f && !(move->w74 & 0x80)) {
            dv = (s32)((f64)ds + (ds <= 0.0f ? lbl_eu_80666E60 : lbl_eu_80666E58));
        }
        if (dv <= 0) return;

        if (!(move->w78 & 0x10)) {
            cvtB.w[1] = (u32)dv ^ 0x80000000;
            move->f68 = move->f68 - (f32)(cvtB.d - lbl_eu_80666DE0);
        }
        if (move->w78 & 0x1000) goto finalClamp;
        if (((E08E8_Vt*)target)->v2BC() != 0) goto finalClamp;

        // 0x92 proc-rate block.
        if (func_80148778(&((E08E8_Actor*)target)->statusBase, 0x92)) {
            s32 bonus = 0;
            if (func_80148778(&((E08E8_Actor*)target)->statusBase, 0xA1)) {
                bonus = ((E08E8_StatusEntry*)func_80149154(
                    &((E08E8_Actor*)target)->statusBase, 0xA1))->w10;
            }
            E08E8_StatusEntry* e92 = (E08E8_StatusEntry*)func_80149154(
                &((E08E8_Actor*)target)->statusBase, 0x92);
            if (e92 != nullptr && move->f68 <= 0.0f) {
                s32 need = e92->w10;
                if (e92->w08 == 0x2000) need += bonus;
                if ((s32)(move->wA8 % 100) < need) {
                    move->w74 |= 0xA0000000;
                    move->f68 = lbl_eu_80666DD4;
                }
            }
        }
        if (!(move->w78 & 0x2000)) {
            // 0xFC fixed-rate bleed.
            if (func_80148778(&((E08E8_Actor*)target)->statusBase, 0xFC)) {
                E08E8_StatusEntry* eFC = (E08E8_StatusEntry*)func_80149154(
                    &((E08E8_Actor*)target)->statusBase, 0xFC);
                if (eFC != nullptr && move->f68 <= 0.0f) {
                    move->w78 |= 0x40002000;
                    f32 mult = ((E08E8_Vt*)target)->v12C();
                    cvtA.w[1] = (u32)eFC->w10 ^ 0x80000000;
                    move->f68 = move->f68 *
                        ((f32)(cvtA.d - lbl_eu_80666DE0) * mult);
                }
            }
        }
        if (!(move->w78 & 0x2000)) {
            // 0x100 fixed-rate bleed.
            if (func_80148778(&((E08E8_Actor*)target)->statusBase, 0x100)) {
                E08E8_StatusEntry* e100 = (E08E8_StatusEntry*)func_80149154(
                    &((E08E8_Actor*)target)->statusBase, 0x100);
                if (e100 != nullptr && move->f68 <= 0.0f) {
                    move->w78 |= 0x40002000;
                    f32 mult = ((E08E8_Vt*)target)->v12C();
                    cvtB.w[1] = (u32)e100->w10 ^ 0x80000000;
                    move->f68 = move->f68 *
                        ((f32)(cvtB.d - lbl_eu_80666DE0) * mult);
                }
            }
        }
        if (!(move->w78 & 0x4000)) {
            // 0x36 growth block.
            if (func_80148778(&((E08E8_Actor*)target)->statusBase, 0x36)) {
                E08E8_StatusEntry* e36 = (E08E8_StatusEntry*)func_80149154(
                    &((E08E8_Actor*)target)->statusBase, 0x36);
                if (e36 != nullptr && move->f68 > 0.0f) {
                    s32 v36 = e36->w10;
                    func_800D81A8(nullptr, target, nullptr);
                    cvtA.w[1] = (u32)v36 ^ 0x80000000;
                    f32 fv = (f32)(cvtA.d - lbl_eu_80666DE0);
                    f32 prod = fv * move->f68;
                    cvtB.w[1] = (u32)(s32)prod ^ 0x80000000;
                    move->f68 = move->f68 + (f32)(cvtB.d - lbl_eu_80666DE0);
                }
            }
        }
        // 0x37 proc-rate block (same shape as 0x92).
        if (func_80148778(&((E08E8_Actor*)target)->statusBase, 0x37)) {
            s32 bonus = 0;
            if (func_80148778(&((E08E8_Actor*)target)->statusBase, 0xA1)) {
                bonus = ((E08E8_StatusEntry*)func_80149154(
                    &((E08E8_Actor*)target)->statusBase, 0xA1))->w10;
            }
            E08E8_StatusEntry* e37 = (E08E8_StatusEntry*)func_80149154(
                &((E08E8_Actor*)target)->statusBase, 0x37);
            if (e37 != nullptr && move->f68 > 0.0f) {
                s32 need = e37->s1A;
                if (e37->w08 == 0x2000) need += bonus;
                if ((s32)(move->wA8 % 100) < need) {
                    s32 v37 = e37->w10;
                    func_800D81A8(nullptr, target, nullptr);
                    cvtA.w[1] = (u32)v37 ^ 0x80000000;
                    f32 fv = (f32)(cvtA.d - lbl_eu_80666DE0);
                    f32 prod = fv * move->f68;
                    cvtB.w[1] = (u32)(s32)prod ^ 0x80000000;
                    move->f68 = move->f68 + (f32)(cvtB.d - lbl_eu_80666DE0);
                }
            }
        }
        if (!(move->w78 & 0x4000)) {
            // 0x11E protection block.
            if (func_80148778(&((E08E8_Actor*)target)->statusBase, 0x11E)) {
                E08E8_StatusEntry* e11E = (E08E8_StatusEntry*)func_80149154(
                    &((E08E8_Actor*)target)->statusBase, 0x11E);
                if (e11E != nullptr && move->f68 > 0.0f) {
                    f32 mult = ((E08E8_Vt*)target)->v12C();
                    cvtA.w[1] = (u32)e11E->s14 ^ 0x80000000;
                    f32 fsm = (f32)(cvtA.d - lbl_eu_80666DE0);
                    s32 q1 = (s32)((fsm * mult) / lbl_eu_80666E00);
                    func_800D81A8(nullptr, target, nullptr);
                    cvtB.w[1] = (u32)q1 ^ 0x80000000;
                    f32 prod2 = (f32)(cvtB.d - lbl_eu_80666DE0) * (fsm * mult);
                    cvtA.w[1] = (u32)(s32)prod2 ^ 0x80000000;
                    move->f68 = move->f68 + (f32)(cvtA.d - lbl_eu_80666DE0);
                    if (e11E->w10 > 0) {
                        if (move->s70 == 0) {
                            move->s70 = (s16)e11E->w10;
                        }
                        s32 tick = move->s70 - 1;
                        move->s70 = (s16)tick;
                        if (tick <= 0) {
                            move->s70 = 0;
                            move->w78 |= 0x40004000;
                        }
                    }
                }
            }
        }
    }

finalClamp:
    // Final clamp: move stays active until the accumulated dot drains.
    if (move->f68 <= 0.0f) {
        move->w78 |= 0x40001000;
    }
}

// Field accessor over the move slot returned by the object's 0x298/0x29C
// dispatch slots (never instantiated; overlay view only).
struct E921SlotAcc {
    u8 pad_00[0x50];
    u32 field_50;
    f32 f54;
    f32 f58;
    f32 f5C;
    f32 f60;
    f32 f64;
    f32 f68;
    f32 f6C;
    s16 s70;
    s16 s72;
    u32 w74;
    u32 w78;
};

// 0xBC-byte move snapshot block copied by value across the damage-preview
// paths. The retail copy codegen is: single words at +0x00/+0x04, eight
// 8-byte chunks +0x08..+0x44, three singles +0x48..+0x50, seven floats
// +0x54..+0x6C, two s16s, a word pair +0x74/+0x78, a single +0x7C, a u16
// +0x80, six word pairs +0x84..+0xB0, then two singles +0xB4/+0xB8.
struct E921C_MoveData {
    u32 w00;    // +0x00
    u32 w04;    // +0x04
    u64 q08;    // +0x08
    u64 q10;    // +0x10
    u64 q18;    // +0x18
    u64 q20;    // +0x20
    u64 q28;    // +0x28
    u64 q30;    // +0x30
    u64 q38;    // +0x38
    u64 q40;    // +0x40
    u32 w48;    // +0x48
    u32 w4C;    // +0x4C
    u32 w50;    // +0x50
    f32 f54;    // +0x54
    f32 f58;    // +0x58
    f32 f5C;    // +0x5C
    f32 f60;    // +0x60
    f32 f64;    // +0x64
    f32 f68;    // +0x68
    f32 f6C;    // +0x6C
    s16 s70;    // +0x70
    s16 s72;    // +0x72
    u64 q74;    // +0x74
    u32 w7C;    // +0x7C
    u16 u80;    // +0x80
    u16 pad82;  // +0x82
    u64 q84;    // +0x84
    u64 q8C;    // +0x8C
    u64 q94;    // +0x94
    u64 q9C;    // +0x9C
    u64 qA4;    // +0xA4
    u64 qAC;    // +0xAC
    u32 wB4;    // +0xB4
    u32 wB8;    // +0xB8
};

// Vtable mirror for the battle-actor dispatch slots used below. Never
// instantiated -- casting a retail object to this type and calling the named
// virtuals makes MWCC route the dispatch through r12 (retail order) instead of
// a general scratch register (cf. ActorVtableMirror / CVisionBattleObj).
struct BMVtMirror {
    virtual ~BMVtMirror() {}                       // i0  = 0x04
    virtual void f01(); virtual void f02(); virtual void f03(); virtual void f04();
    virtual void f05(); virtual void f06(); virtual void f07(); virtual void f08();
    virtual u32 v28(u32 a);                        // i9  = 0x28
    virtual void* v30();                           // i10 = 0x2C
    virtual void f12(); virtual void f13(); virtual void f14(); virtual void f15();
    virtual void f16(); virtual void f17(); virtual void* v4C(); virtual void f19();
    virtual void f20(); virtual void f21(); virtual void f22(); virtual void f23();
    virtual void f24(); virtual void f25(); virtual void f26(); virtual void f27();
    virtual void f28(); virtual void f29(); virtual void f30(); virtual void f31();
    virtual void fF32();
    virtual void v88(f32 f);                       // i33 = 0x88
    virtual void f34(); virtual void f35(); virtual void f36(); virtual void f37();
    virtual void f38(); virtual void f39();
    virtual void vA4(u32 v);                       // i40 = 0xA4
    virtual void f41(); virtual void f42(); virtual void f43(); virtual void f44();
    virtual void vB8();                            // i45 = 0xB8
    virtual void vBC(void* a);                     // i46 = 0xBC
    virtual void f47(); virtual void f48();
    virtual void vC8();                            // i49 = 0xC8
    virtual void f50(); virtual void f51(); virtual void f52(); virtual void f53();
    virtual void f54(); virtual void f55(); virtual void f56(); virtual void f57();
    virtual void f58(); virtual void f59(); virtual void f60(); virtual void f61();
    virtual void f62(); virtual void f63(); virtual void v100();
    virtual void* v108();                          // i65 = 0x108
    virtual void f66(); virtual void f67(); virtual void f68();
    virtual void v118(f32 f);                      // i69 = 0x118
    virtual void f70(); virtual void f71(); virtual void f72();
    virtual f32 v128();                            // i73 = 0x128
    virtual void f74(); virtual void f75(); virtual void f76(); virtual void f77();
    virtual void f78(); virtual void f79(); virtual void f80(); virtual void f81();
    virtual void f82();
    virtual void v150(f32 f);                      // i83 = 0x150
    virtual void v154(f32 f);                      // i84 = 0x154
    virtual void f85(); virtual void f86(); virtual void f87(); virtual void f88();
    virtual void f89(); virtual void f90(); virtual void f91(); virtual void f92();
    virtual void f93(); virtual void f94(); virtual void f95(); virtual void f96();
    virtual void f97(); virtual void f98(); virtual void f99(); virtual void f100();
    virtual void f101(); virtual void f102(); virtual void f103(); virtual void f104();
    virtual void f105(); virtual void f106(); virtual void f107(); virtual void f108();
    virtual void f109(); virtual void f110(); virtual void f111(); virtual void f112();
    virtual void f113(); virtual void f114(); virtual void f115(); virtual void f116();
    virtual void f117(); virtual void f118(); virtual void f119(); virtual void f120();
    virtual void* v1E8();                          // i121 = 0x1E8
    virtual void f122(); virtual void f123(); virtual void f124(); virtual void f125();
    virtual void f126();
    virtual void* v200();                          // i127 = 0x200
    virtual void f128(); virtual void f129(); virtual void f130(); virtual void f131();
    virtual void f132(); virtual void f133(); virtual void f134(); virtual void f135();
    virtual void f136(); virtual void f137(); virtual void f138(); virtual void f139();
    virtual void f140(); virtual void f141(); virtual void f142(); virtual void f143();
    virtual void f144(); virtual void f145(); virtual void f146(); virtual void f147();
    virtual void f148(); virtual void f149(); virtual void f150(); virtual void f151();
    virtual void f152(); virtual void f153(); virtual void f154(); virtual void f155();
    virtual void f156(); virtual void f157(); virtual void f158(); virtual void f159();
    virtual void f160(); virtual void f161(); virtual void f162();
    virtual s32 v290();                            // i163 = 0x290
    virtual void f164();
    virtual void* v298();                          // i165 = 0x298
    virtual void* v29C(u32 idx);                   // i166 = 0x29C
    virtual void* v2A0();                          // i167 = 0x2A0
    virtual void* v2A4();                          // i168 = 0x2A4
    virtual s32 v2A8();                            // i169 = 0x2A8
    virtual void v2AC();                           // i170 = 0x2AC
    virtual void f171();
    virtual void v2B4();                           // i172 = 0x2B4
    virtual void v2B8();                           // i173 = 0x2B8
    virtual s32 v2BC();                            // i174 = 0x2BC
    virtual void f175(); virtual void f176(); virtual void f177();
    virtual void f178(); virtual void f179(); virtual void f180(); virtual void f181();
    virtual void f182(); virtual void f183(); virtual void f184(); virtual void f185();
    virtual void f186(); virtual void f187(); virtual void f188(); virtual void f189();
    virtual void f190(); virtual void f191();
    virtual void v304(u32 v);                      // i192 = 0x304
    virtual s32 v308();                            // i193 = 0x308
};

// func_800E921C (retail 0x800E9D04, 0x934 bytes): battle damage-preview setup.
// Computes the damage preview for the party's selected arts: resolves the
// target sub-object/action, clears all player selection flags, fills the
// actor stat block and per-member move snapshots, then accumulates the
// rounded damage sum into *outDamage.
void func_800E921C(void* self, void* actor, void* obj, f32* outDamage, s32* outFlag) {
    BMVtMirror* objM = (BMVtMirror*)obj;

    // r23: selected target sub-object from obj's move slot; r31: its action.
    void* subObj = *(void**)((u8*)objM->v298() + 0x50);
    BMVtMirror* action = (BMVtMirror*)func_8016FE34((void*)(intptr_t)func_800B708C__Fi(
        *(s32*)((u8*)objM->v298() + 4)));

    *outDamage = lbl_eu_80666DDC;
    *outFlag = 0;
    std::memset((u8*)actor + 0x828, 0, 8);

    if (action == nullptr) return;
    if (subObj == nullptr) return;

    // Talent-gauge-full one-round path: the preview becomes the computed value.
    if (*(u32*)((u8*)actor + 0x824) & 0x20000) {  // bit14 (rlwinm 14,14)
        cf::CfActorF64Conv conv;
        conv.w[0] = 0x43300000;
        conv.w[1] = (u32)func_800F4648(actor) ^ 0x80000000;
        *outDamage = (f32)(conv.d - lbl_eu_80666DE0);
        return;
    }

    // Party size / arts-count gate for the preview loop.
    if (*(u8*)((u8*)subObj + 0x43) == 1) {
        *outFlag = *(u8*)((u8*)subObj + 0x44);
    } else {
        *outFlag = 1;
    }

    // Clear the selection flag on the action, the 3 party members, and obj.
    action->vA4(0);
    for (s32 i = 0; i < 3; i++) {
        BMVtMirror* player =
            (BMVtMirror*)func_8016FE34(getPlayer__Q22cf13CfGameManagerFi(i));
        if (player != nullptr && player != action) {
            player->vA4(0);
        }
    }
    objM->vA4(0);
    action->vA4(0);

    *(u32*)((u8*)actor + 0x88) &= ~0x1000;

    // Store the base damage into the target move slots (0x6C/0x68) and zero
    // the tick counters (each store re-fetches the slot via 0x298).
    {
        f32 dmg = objM->v128();
        ((E921SlotAcc*)objM->v298())->f6C = dmg;
    }
    {
        f32 dmg = action->v128();
        ((E921SlotAcc*)objM->v298())->f68 = dmg;
    }
    ((E921SlotAcc*)objM->v298())->s70 = 0;
    ((E921SlotAcc*)objM->v298())->s72 = 0;

    // Reset each party member's move slot.
    for (s32 i = 0; i < *outFlag; i++) {
        E921SlotAcc* slot = (E921SlotAcc*)objM->v29C(i);
        slot->w74 = 0;
        slot->w78 = (slot->w78 & 0xFFFF0000) | (slot->w78 & 0x03F00000);
        slot->f54 = lbl_eu_80666DDC;
        slot->f58 = lbl_eu_80666DDC;
        slot->f5C = lbl_eu_80666DDC;
        slot->f60 = lbl_eu_80666DDC;
        slot->f64 = lbl_eu_80666DDC;
    }

    // Per-party-member preview loop: run the ai dispatcher, snapshot the
    // stat block / per-member move data, accumulate the rounded damage.
    f32 zero2 = lbl_eu_80666DDC;
    cf::CfActorF64Conv conv;    // w[0] hoisted into a register across the loop
    conv.w[0] = 0x43300000;
    s32 idx = 0;
    while (idx < *outFlag) {
        E921SlotAcc* slot = (E921SlotAcc*)objM->v298();   // r29
        slot->w78 |= 0x44000000;
        func_800DB0FC((void*)(intptr_t)lbl_eu_80663F00, obj, action, slot);
        slot->w78 &= ~0x04000000;

        if (objM->v2A8() == 0) {
            // Snapshot into the actor's displayed stat block (0x10..0xC8).
            *(E921C_MoveData*)((u8*)actor + 0x10) =
                *(const E921C_MoveData*)objM->v298();
        } else {
            u32 fl = ((E921SlotAcc*)objM->v298())->w74;
            if (fl & 0x80000000) {
                *(u32*)((u8*)actor + 0x84) |= fl;
            } else {
                *(u32*)((u8*)actor + 0x88) |= fl;
            }
        }

        // Per-member move snapshot at actor + v2A8()*0xBC + 0xCC.
        *(E921C_MoveData*)((u8*)actor + objM->v2A8() * 0xBC + 0xCC) =
            *(const E921C_MoveData*)slot;

        if (slot->w78 & 0x1000) {
            *(u32*)((u8*)actor + 0x828) = 0;
            *(u32*)((u8*)actor + 0x88) |= 0x40001000;
            *(u32*)((u8*)actor + 0x82C) = 0;
        }

        // Round base+variance to the nearest int and accumulate.
        f32 sum = slot->f5C + slot->f60;
        s32 ival = 0;
        if (sum >= 0.0f) {
            if (!(slot->w74 & 0x80)) {
                ival = (s32)((f64)sum + (sum <= 0.0f ? lbl_eu_80666E60 : lbl_eu_80666E58));
            }
        }
        conv.w[1] = (u32)ival ^ 0x80000000;
        f32 add = (f32)(conv.d - lbl_eu_80666DE0);
        *outDamage = *outDamage + add;
        objM->v2B8();

        if (slot->f6C < zero2) break;
        idx++;
    }

    // Final snapshot into the 0x2A4 target block.
    objM->v2AC();
    BMVtMirror* src = (BMVtMirror*)objM->v298();
    void* dst = objM->v2A4();
    *(E921C_MoveData*)dst = *(const E921C_MoveData*)src;

    if (*outDamage < 0.0f) *outDamage = 0.0f;
    if (*outFlag > 1) *(f32*)((u8*)actor + 0x6C) = *outDamage;
}
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

// Snapshot block laid out at sp+0xF4 in retail: the move-data copy, then the
// target pointer (+0x1B0) and the action (+0x1B4) right behind it.
struct E1B5C_Snapshot {
    E921C_MoveData data;
    void* target;
    void* action;
};

// Workspace block handed to func_800DB0FC / func_800E2594 (retail lays it
// out at sp+0x38). Field pattern: src words at +0x00..0x50, a zeroed float
// pack at +0x54..0x64, damage/variance at +0x68/+0x6C, s16s at +0x70/+0x72,
// flags at +0x74/+0x78, and the shared word tail at +0x80..0xB8.
struct E1B5C_Workspace {
    u32 v00; u32 v04; u32 v08; u32 v0C; u32 v10; u32 v14; u32 v18; u32 v1C;
    u32 v20; u32 v24; u32 v28; u32 v2C; u32 v30; u32 v34; u32 v38; u32 v3C;
    u32 v40; u32 v44; u32 v48; u32 v4C;
    void* v50;      // +0x50 src[0x50]
    f32 v54;        // +0x54 0.0
    f32 v58;        // +0x58 0.0
    f32 v5C;        // +0x5C 0.0
    f32 v60;        // +0x60 0.0
    f32 v64;        // +0x64 0.0
    f32 v68;        // +0x68 src[0x5C]
    f32 v6C;        // +0x6C src[0x60]
    s16 v70;        // +0x70 src[0x70]
    s16 v72;        // +0x72 src[0x72]
    u32 v74;        // +0x74 0
    u32 v78;        // +0x78 flags
    u32 v7C;        // +0x7C 0
    u16 v80;        // +0x80 src[0x80]
    u16 pad82;      // +0x82
    u32 v84; u32 v88; u32 v8C; u32 v90; u32 v94; u32 v98; u32 v9C;
    u32 vA0; u32 vA4; u32 vA8; u32 vAC; u32 vB0; u32 vB4;
    void* vB8;      // +0xB8 &copy
};

// func_800E1B5C (retail 0x800E1B5C, 0xAF0 bytes): party-chain / target-table
// refresh. Verifies the actor's two move objects reference the same target,
// snapshots the source move data, broadcasts the arts action to the party
// (via the vision enum list or the 2x8 target-slot scan + chain handoff), then
// re-runs the 16-slot fallback building per-target event workspaces.
extern "C" void func_8027750C(void*, void*);
extern "C" void func_8027D478(void*, void*);
extern "C" void func_80274B24(void*, void*);
extern "C" void func_802A1D04(void*, void*);
extern "C" void func_800E2594(void*, void*, void*, void*);

void func_800E1B5C(void* mgr, void* actor) {
    BMVtMirror* act = (BMVtMirror*)actor;
    E1B5C_ObjView* av = (E1B5C_ObjView*)actor;

    // NOTE: dst declared first - MWCC colors locals in declaration order and
    // retail allocates it above src.
    E921C_MoveData* dst;
    E921C_MoveData* src = (E921C_MoveData*)act->v298();     // r30: move obj A
    dst = (E921C_MoveData*)act->v2A0();                     // move obj B

    // Both move objects must name the same target id / sub-object.
    if ((s32)src->w48 != (s32)dst->w48) return;
    if (src->w50 != dst->w50) return;
    if (av->field_3F60 == 0) return;

    BMVtMirror* action =
        (BMVtMirror*)func_8016FE34(
            (void*)(intptr_t)func_800B708C__Fi((s32)src->w04));

    // Snapshot the full move block (sp+0xF4..0x1AC); the target pointer
    // (sp+0x1B0) and the action (sp+0x1B4) sit right behind it.
    E1B5C_Snapshot snap;
    snap.target = (void*)src->w50;
    snap.data = *src;
    snap.action = action;
    void* targetPtr = snap.target;
    E921C_MoveData& copy = snap.data;

    if (action == nullptr) goto skipScan;
    if (((E1B5C_ObjView*)action)->field_3F60 == 0) goto skipScan;
    if ((((E1B5C_ObjView*)action)->field_3F00 & 0x4) == 0) {
        if ((((E1B5C_ObjView*)action)->field_3F00 & 0x2) == 0) return;
    }
    if (targetPtr == nullptr) return;

    if (((BMVtMirror*)action)->v2BC() == 0) {
        getInstance__Q22cf13CfGameManagerFv();
        if (!func_8006EF04__Fi(0x400)) {
            // Broadcast path: target has the 0x8000 flag - poke every party
            // member's 0x118 hook with the shared float constant.
            if (((E1B5C_TargetObj*)targetPtr)->flags_78 & 0x8000) {
                EnumListHolder holder;
                func_80043D90(&holder);
                func_800F4A98(func_80043F18(&holder), 0x20, 0);
                for (u32 i = 0; i < *(u32*)((u8*)func_80043F18(&holder) + 0x620);
                     i++) {
                    BMVtMirror* o = (BMVtMirror*)func_8016FE34(
                        func_800F6EAC(func_80043F18(&holder), i));
                    o->v118(lbl_eu_80666DDC);
                }
                __dt__80043E88(&holder, -1);
                return;
            }
        }
    }

scan2:
    // 2x8 target-slot scan: does any slot reference src->field_04?
    {
        u32 slotId = src->w04;
        bool found = false;
        for (s32 blk = 0; blk < 2 && !found; blk++) {
            u32* base = (u32*)((u8*)src + 8 + blk * 0x20);
            for (s32 j = 0; j < 8; j++) {
                if (base[j] == slotId) {
                    found = true;
                    break;
                }
            }
        }

        if (found) {
            s32 ok = 1;
            if (av->field_3F00 & 0x2) {
                if (av->field_3F28 == 7) {
                    if (((E1B5C_MoveFlags*)src)->flags_78 & 0x1000) {
                        if (((BMVtMirror*)actor)->v2A8() != 1) ok = 0;
                    }
                }
            }
            if (ok) {
                func_8027750C((u8*)mgr + 0x1A8, actor);
                func_8027D478((u8*)mgr + 0x20C8, actor);
            }
            func_80274B24((u8*)mgr + 0x2194, actor);
            func_802A1D04(actor, action);
            func_800E2594(mgr, actor, src, action);
            if (((E1B5C_ObjView*)action)->field_3F00 & 0x4) {
                void* h = ((E1B5C_ObjView*)action)->field_3ED4;
                ((void (*)(void*))(*(void***)h)[0x100 / 4])(h);
            }
        }
    }

skipScan:
    getInstance__Q22cf13CfGameManagerFv();
    if (!func_8006EF04__Fi(0x400)) {
        // Per-target fallback: rebuild a workspace from the snapshot for each
        // of the 16 party-slot ids and run the shared move dispatchers on it.
        E1B5C_Snapshot* snapP = &snap;
        u32* idp = (u32*)((u8*)&snap + 8);
        for (s32 i = 0; i < 16; i++, idp++) {
            if (*idp == 0) continue;
            void* pl = func_8016FE34((void*)(intptr_t)func_800B708C__Fi((s32)*idp));
            E1B5C_ObjView* pv = (E1B5C_ObjView*)pl;
            if (pl == nullptr || ((BMVtMirror*)pl)->v2BC() != 0) {
                *idp = 0;
                continue;
            }
            if (pl == (void*)action) continue;

            E1B5C_Workspace ws;
            ws.v00 = copy.w00;
            ws.v04 = copy.w04;
            ws.v08 = (u32)copy.q08;
            ws.v0C = (u32)(copy.q08 >> 32);
            ws.v10 = (u32)copy.q10;
            ws.v14 = (u32)(copy.q10 >> 32);
            ws.v18 = (u32)copy.q18;
            ws.v1C = (u32)(copy.q18 >> 32);
            ws.v20 = (u32)copy.q20;
            ws.v24 = (u32)(copy.q20 >> 32);
            ws.v28 = (u32)copy.q28;
            ws.v2C = (u32)(copy.q28 >> 32);
            ws.v30 = (u32)copy.q30;
            ws.v34 = (u32)(copy.q30 >> 32);
            ws.v38 = (u32)copy.q38;
            ws.v3C = (u32)(copy.q38 >> 32);
            ws.v40 = (u32)copy.q40;
            ws.v44 = (u32)(copy.q40 >> 32);
            ws.v48 = copy.w48;
            ws.v4C = copy.w4C;
            ws.v50 = (void*)(uintptr_t)copy.w50;
            ws.v54 = 0.0f;
            ws.v58 = 0.0f;
            ws.v5C = 0.0f;
            ws.v60 = 0.0f;
            ws.v64 = 0.0f;
            ws.v68 = copy.f68;
            ws.v6C = copy.f6C;
            ws.v70 = copy.s70;
            ws.v72 = copy.s72;
            ws.v74 = 0;
            ws.v78 = ((*(u32*)&copy.f6C) & ~0xFFu) | 0x40000001;
            ws.v7C = 0;
            ws.v80 = copy.u80;
            ws.v84 = (u32)copy.q84;
            ws.v88 = (u32)(copy.q84 >> 32);
            ws.v8C = (u32)copy.q8C;
            ws.v90 = (u32)(copy.q8C >> 32);
            ws.v94 = (u32)copy.q94;
            ws.v98 = (u32)(copy.q94 >> 32);
            ws.v9C = (u32)copy.q9C;
            ws.vA0 = (u32)(copy.q9C >> 32);
            ws.vA4 = (u32)copy.qA4;
            ws.vA8 = (u32)(copy.qA4 >> 32);
            ws.vAC = (u32)copy.qAC;
            ws.vB0 = (u32)(copy.qAC >> 32);
            ws.vB4 = copy.wB4;
            ws.vB8 = snapP;                     // +0xB8 = &snapshot

            ws.v04 = (u32)pv->field_3F10;       // tag with the actor id

            func_800DB0FC(mgr, actor, pl, &ws);
            func_800E2594(mgr, actor, pl, &ws);

            // Register the target into the actor list if not present already.
            SimpleListNode* sentinel = *(SimpleListNode**)((u8*)mgr + 8);
            SimpleListNode* cur = sentinel->next;
            while (cur != sentinel && cur->data != pl) cur = cur->next;
            if (cur == sentinel && (pv->field_3F00 & 0x4) &&
                !(pv->field_3374 & 0x08000000)) {
                func_800D9978((cf::CBattleManager*)mgr, (cf::CfObjectActor*)pl);
            }
        }
    }

    // Status reset: clear status 0xC0 through the actor's status object when
    // the target is a type-1/2 object with the 0x400 move flag.
    if (func_80148778((u8*)actor + 8, 0xC0)) {
        if (targetPtr != nullptr) {
            E1B5C_TargetObj* tp = (E1B5C_TargetObj*)targetPtr;
            if ((u32)(tp->type_3c - 1) <= 1) {
                if (((E1B5C_MoveFlags*)src)->flags_78 & 0x400) {
                    ((void (*)(void*, u32))(*(void***)((u8*)actor + 8))[0x20 / 4])(
                        (u8*)actor + 8, 0xC0);
                }
            }
        }
    }

    // Player-actor special case: install an AI action slot for player 0.
    if (av->field_3F28 == 5) {
        if (av->field_3F00 & 0x2) {
            void* pcSub = actor != nullptr ? (void*)((u8*)actor + 0x3E9C) : actor;
            if (pcSub == getPlayer__Q22cf13CfGameManagerFi(0)) {
                if (targetPtr != nullptr) {
                    E1B5C_TargetObj* tp = (E1B5C_TargetObj*)targetPtr;
                    if ((tp->flags_78 & 0x40000000) && tp->id_48 == 9) {
                        BattleAIActionSlot slot;
                        std::memset((u8*)&slot + 4, 0, 0xE);
                        std::memset(&slot, 0, sizeof(slot));
                        slot.unk00 = (u32)av->field_3F10;
                        slot.b_06 = 0x25;
                        slot.b_0D = 1;
                        slot.b_0E = 0x64;
                        slot.h_10 = 0;
                        slot.h_12 = 0;
                        slot.f_14 = lbl_eu_80666E6C;
                        func_8014AC38((u8*)actor + 0x3380, &slot);
                    }
                }
            }
        }
    }

    if (av->field_3F00 & 0x2) {
        if (((u32 (*)(void*, u32))(*(void***)mgr)[0x28 / 4])(mgr, 1)) {
            ((void (*)(void*, u32))(*(void***)mgr)[0x20 / 4])(mgr, 1);
        }
    }

    // Status-id gate: run func_80174C98 against the actor's live status id.
    {
        void* stHolder = *(void**)((u8*)actor + 4);
        void* stPtr = ((void* (*)(void*))(*(void***)stHolder)[0x30 / 4])(stHolder);
        u32 stVal = *(u32*)stPtr;
        if (func_80174C98(actor, (int*)&stVal, 0x800)) {
            ((void (*)(void*, u32))(*(void***)stHolder)[0x20 / 4])(stHolder, 0x800);
        }
    }

    cf::CBattleManager* bm = (cf::CBattleManager*)mgr;
    void* vision = &((cf::CBattleManager*)mgr)->mVision;
    if (lbl_eu_80663F00 != 0) {
        if (func_801A8070(vision) != nullptr) {
            ((void (*)(void*, void*, void*))(*(void***)vision)[0x28 / 4])(
                vision, actor, action);
        }
    }

    // Re-run the dispatcher when the selected arts level exceeds the actor's.
    if (src->w04 != 0) {
        if (copy.w48 == src->w48) {
            DB4FC_ArtsObj* arts = (DB4FC_ArtsObj*)(void*)src->w50;
            act->v2B8();    // result discarded in retail
            if (arts != nullptr) {
                s32 lv = arts->field_44;
                s32 cur = act->v2A8();
                if (lv > cur) {
                    E921C_MoveData* src2 = (E921C_MoveData*)act->v298();
                    func_800DB0FC(mgr, actor, action, src2);
                }
            }
        }
    }
}

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
// vtable dispatch helpers (r23/r24 are CfObjectActor-derived)
// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
// vtable dispatch helpers (r23/r24 are CfObjectActor-derived)
// ---------------------------------------------------------------------------
#pragma schedule on
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
            if (func_800D7D24((D7D24_Obj*)move) == 0) hit = 0;    // 0x82C0-0x82D0
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

// ============================================================================
// func_800E85F0 / CBattleManager_preCalcTotalDamage / func_800E9B54 /
// func_800E2594 -- move-type dispatchers and chain-arts spread helpers
// ============================================================================

// ---- vtable slots used only by these five functions ------------------------
static inline void vf88(void* o, f32 f)  { ((void(*)(void*, f32))(*(void***)o)[0x88 / 4])(o, f); }
static inline void vf5C4(void* o, f32 f) { ((void(*)(void*, f32))(*(void***)o)[0x5C4 / 4])(o, f); }
static inline void vfBC(void* o, void* a) { ((void(*)(void*, void*))(*(void***)o)[0xBC / 4])(o, a); }
static inline void* vf29C(void* o, u32 a) { return ((void*(*)(void*, u32))(*(void***)o)[0x29C / 4])(o, a); }
static inline void e_vf304(void* o, u32 v) { ((void(*)(void*, u32))(*(void***)o)[0x304 / 4])(o, v); }
static inline void e_vf150(void* o, f32 f) { ((void(*)(void*, f32))(*(void***)o)[0x150 / 4])(o, f); }
static inline void e_vfA4(void* o, u32 v) { ((void(*)(void*, u32))(*(void***)o)[0xA4 / 4])(o, v); }
static inline void* e_vf2A4(void* o) { return ((void*(*)(void*))(*(void***)o)[0x2A4 / 4])(o); }
static inline void* e_vf298(void* o) { return ((void*(*)(void*))(*(void***)o)[0x298 / 4])(o); }
static inline void e_vf2AC(void* o) { ((void(*)(void*))(*(void***)o)[0x2AC / 4])(o); }
static inline void e_vf2B4(void* o) { ((void(*)(void*))(*(void***)o)[0x2B4 / 4])(o); }
static inline void e_vf2B8(void* o) { ((void(*)(void*))(*(void***)o)[0x2B8 / 4])(o); }
static inline f32 e_vf128(void* o) { return ((f32(*)(void*))(*(void***)o)[0x128 / 4])(o); }
static inline void vfB8(void* o)     { ((void(*)(void*))(*(void***)o)[0xB8 / 4])(o); }
static inline void vfC8(void* o)     { ((void(*)(void*))(*(void***)o)[0xC8 / 4])(o); }

// ---- small object views (byte-precise offsets) -----------------------------
struct E2594_Sub {          // move sub-object (type/h28/b44/flags)
    u8 pad_00[0x28];
    u16 h28;                // +0x28
    u8 pad_2A[0x3C - 0x2A];
    u16 type3C;             // +0x3C
    u8 pad_3E[0x44 - 0x3E];
    u8 b44;                 // +0x44
    u8 pad_45[0x78 - 0x45];
    u32 flags78;            // +0x78
};
struct E85F0_Move {         // move block view used by func_800E85F0
    u8 pad_00[0x50];
    void* sub;              // +0x50
    u8 pad_54[0x24];
    u32 flags78;            // +0x78
    u8 pad_7C[0x14];
    u32 field_90;           // +0x90
};
struct CE_Entry {           // func_801491F4 0xCE entry (u16 at +0x2C)
    u8 pad_00[0x2C];
    u16 h2C;                // +0x2C
};
struct E85F0_Entry12E {     // func_80149154 0x12E entry
    u8 pad_00[0x10];
    s32 field_10;           // +0x10
    u8 pad_14[0x0C];
    f32 field_20;           // +0x20
};
struct BMSub43View {        // party-order bytes at +0x43/+0x44
    u8 pad_00[0x43];
    u8 b43;                 // +0x43
    u8 b44;                 // +0x44
};
struct E484_TypeObj {       // enum-cast result: u16 type at +0x8C, +0x9C ptr
    u8 pad_00[0x8C];
    u16 type8C;             // +0x8C
    u8 pad_8E[0x9C - 0x8E];
    void* field_9C;         // +0x9C
};
struct E484_VisionActor {   // vision actor: partner ptr at +0x45B8
    u8 pad_00[0x45B8];
    void* field_45B8;       // +0x45B8
};
struct E484_VisionPair {    // func_801A8070 result: two u32 ids
    u32 field_00;
    u32 field_04;
};
struct E484_Global6C {      // func_80044DF4 result: f32 at +0x6C
    u8 pad_00[0x6C];
    f32 field_6C;           // +0x6C
};

// ---- imports (retail-unmangled names) --------------------------------------
extern "C" void func_802A2E08();
extern "C" void func_8027F0B8();
extern "C" void func_80295BF4(void*);
extern "C" void func_80295CC8(void*, void*);
extern "C" s32 func_801B1E74(void*, s32, void*, s32);
extern "C" void func_800BF29C(void*, int, int, u32, f32, f32);
extern "C" void func_80496294(void*);
// Retail references func_800D7D24 by its unmangled symbol; route new calls
// through a C-linkage overload (the C++-mangled definition above is untouched).
extern "C" s32 func_800D7D24(void*);
extern "C" void* func_80044DF4();
extern "C" void func_800E85F0(void*, void*, void*, void*);
extern "C" void func_800E2594(void*, void*, void*, void*);
extern "C" void CBattleManager_preCalcTotalDamage(void*, void*, f32*, u32*);
extern "C" void func_800E9B54(void*, void*, void*, void*);
// lbl_eu_80664A10 (CHelpManager-ish singleton) is declared by an included header
extern f32 lbl_eu_80666E78;     // 30.0f
extern u32 lbl_eu_804FCAF0[];   // {80,60,40,25,10,...} arts-power table

// ---- func_800E85F0 (0x800E90D8) --------------------------------------------
// Move-type 4/5/6 follow-up: validates the giver, fires the 0x12E status (or
// its default event), mirrors the first move block, then either runs the
// counter/chain logic or recurses into func_800E64CC.
extern "C" void func_800E85F0(void* self, void* actor, void* target, void* move) {
    if (target == nullptr) return;                           // 0x800E9100
    if (!(((BattleObjAccessor*)actor)->field_3f00 & 0x2)) return; // 0x800E9104
    void* sub = ((E85F0_Move*)move)->sub;                    // loaded before the +0x3f28 test
    if (((BattleObjAccessor*)actor)->field_3f28 != 7) return; // 0x800E9114

    // One scratch home serves the D7D24 probe, both 0x10A status reads and
    // the 0x12E entry pointer (retail reuses one callee-saved register for
    // all of them).
    void* stat = (void*)func_800D7D24((void*)move);
    if (((E85F0_Move*)move)->flags78 & 0x800) {              // 0x800E9130
        if (e_vf2A8(actor) == 0) {                           // 0x800E9138 chain-arts pass
            stat = (void*)func_80148778((u8*)actor + 8, 0x10A);
            // Retail parks the chain type in the move pointer's register --
            // mirrored here by reusing the parameter slot.
            move = (void*)(uintptr_t)func_801B1E74((target != nullptr) ? (u8*)target + 0x3E9C : target, -1, stat, 0);
            s32 type = (s32)(uintptr_t)move;
            // 0x800E91C4..0x800E92A0: art id picked per chain type; the
            // neg/or/srwi pair in retail computes (stat/0x10A result != 0).
            switch (type) {
            case 6:
                func_800BF29C((u8*)actor + 0x3E9C, 0x67 + (stat != nullptr), 0, 0, lbl_eu_80666DD0, lbl_eu_80666E78);
                break;
            case 4:
                func_800BF29C((u8*)actor + 0x3E9C, 0x69 + (stat != nullptr), 0, 0, lbl_eu_80666DD0, lbl_eu_80666E78);
                break;
            case 7:
                func_800BF29C((u8*)actor + 0x3E9C, 0x6B + (stat != nullptr), 0, 0, lbl_eu_80666DD0, lbl_eu_80666E78);
                break;
            case 8:
                func_800BF29C((u8*)actor + 0x3E9C, 0x6D + (stat != nullptr), 0, 0, lbl_eu_80666DD0, lbl_eu_80666E78);
                break;
            case 9:
                func_800BF29C((u8*)actor + 0x3E9C, 0x6F + (stat != nullptr), 0, 0, lbl_eu_80666DD0, lbl_eu_80666E78);
                break;
            case 5:
                func_800BF29C((u8*)actor + 0x3E9C, 0x71 + (stat != nullptr), 0, 0, lbl_eu_80666DD0, lbl_eu_80666E78);
                break;
            }

            if (func_80148778((u8*)actor + 8, 0x12E)) {      // 0x800E92C8
                stat = func_80149154((u8*)actor + 8, 0x12E); // entry ptr shares the scratch home
                E85F0_Entry12E* e = (E85F0_Entry12E*)stat;
                if (e->field_10 != type) e_vf154(actor, lbl_eu_80666E6C);
                e->field_10 = type;
                e->field_20 = lbl_eu_80666E6C;
            } else {
                BtlEvent ev;
                std::memset(&ev, 0, sizeof(ev));
                ev.mTargetId = *(u32*)((u8*)actor + 0x3F10);
                ev.mParam = sub;
                ev.mEvId = 0x12E;
                ev.mVal10 = type;
                ev.mF20 = lbl_eu_80666E6C;
                func_800EC918((void*)self, (EC918_BattleObjAccessor*)actor,
                              (EC918_BattleObjAccessor*)actor, (BattleEvent*)&ev, 0);
            }
            e_vf154(actor, lbl_eu_80666E34);                 // 10.0 (0x800E9368)
            void* srcB = vf29C(actor, 0);                    // 0x800E9384
            void* dstB = vf29C(actor, 1);
            *((E2A9C_BattleMoveData*)dstB) = *((E2A9C_BattleMoveData*)srcB);
            return;                                          // b .L_800E9644
        }
        // 0x800E9538: second pass -- the move flags are re-read here
        if (e_vf2A8(actor) == 1) {                           // 0x800E9544
            stat = (void*)func_80148778((u8*)actor + 8, 0x10A);
            if (sub != nullptr) {                            // r30 == move->field_50
                if ((u32)(((E2594_Sub*)sub)->type3C - 5) <= 1) {   // types 5,6
                    func_800E2A9C((cf::CBattleManager*)self, (cf::CfObjectActor*)actor,
                                  (cf::CfObjectActor*)target, (E2A9C_BattleMoveData*)move);
                } else {
                    func_800E64CC((cf::CBattleManager*)self, actor, target, move);
                }
                if (stat != nullptr) {                       // 0x800E95B4
                    u32 tbl[5];
                    tbl[0] = lbl_eu_804FCAF0[0];
                    tbl[1] = lbl_eu_804FCAF0[1];
                    tbl[2] = lbl_eu_804FCAF0[2];
                    tbl[3] = lbl_eu_804FCAF0[3];
                    tbl[4] = lbl_eu_804FCAF0[4];
                    s32 idx = (s32)e_vf308(actor);           // 0x800E95BC
                    if ((s32)((E85F0_Move*)move)->field_90 < (s32)tbl[idx]) { // 0x800E9600
                        e_vf150(actor, lbl_eu_80666DDC);
                    }
                }
            }
            return;
        }
        return;                                              // bne .L_800E9644
    }
    func_800E64CC((cf::CBattleManager*)self, actor, target, move); // 0x800E9630
}

// ---- CBattleManager_preCalcTotalDamage (0x800E965C) -------------------------
// Sums the pending party damage: for each of the outCount entries it lets
// func_800DB0FC finalise the 0x298 move block, adds the rounded
// field_5C+field_60 amount, then restores a clean copy of the block.
extern "C" void CBattleManager_preCalcTotalDamage(void* self, void* actor, f32* outDamage, u32* outCount) {
    // r3 (self) is not used by the retail body
    void* obj = actor;                                       // r27

    void* sub = ((E2A9C_BattleMoveData*)e_vf2A4(obj))->field_50;         // r31
    void* action = func_8016FE34(
        (void*)(intptr_t)func_800B708C((s32)((BtlMove*)e_vf2A4(obj))->mSubId)); // r30

    *outDamage = lbl_eu_80666DDC;                            // 0x800E96C4
    *outCount = 0;
    if (action == nullptr) return;
    if (sub == nullptr) return;

    // Hit count: 1 per sub-object flag, else a single hit.
    if (*(u8*)((u8*)sub + 0x43) == 1) *outCount = *(u8*)((u8*)sub + 0x44);
    else *outCount = 1;

    // Clear the selection flag on the action, the 3 party members, and obj.
    e_vfA4(action, 0);                                       // 0x800E9708
    for (s32 i = 0; i < 3; i++) {                            // 0x800E9724
        void* p = func_8016FE34(getPlayer__Q22cf13CfGameManagerFi(i));
        if (p != nullptr && p != action) e_vfA4(p, 0);
    }
    e_vfA4(obj, 0);                                          // 0x800E9760
    e_vfA4(action, 0);                                       // 0x800E9778
    e_vf2B4(obj);                                            // 0x800E9790

    // Seed the workspace block with the target's current move data.
    *(E2A9C_BattleMoveData*)e_vf298(obj) = *(E2A9C_BattleMoveData*)e_vf2A4(obj);

    // Store the base damage into the workspace (0x6C/0x68) and zero ticks.
    {
        f32 dmg = e_vf128(obj);                              // 0x800E9950
        ((E2A9C_BattleMoveData*)e_vf298(obj))->field_6C = dmg;
    }
    {
        f32 dmg = e_vf128(action);                           // 0x800E9980
        ((E2A9C_BattleMoveData*)e_vf298(obj))->field_68 = dmg;
    }
    ((E2A9C_BattleMoveData*)e_vf298(obj))->field_70 = 0;     // 0x800E99B0
    ((E2A9C_BattleMoveData*)e_vf298(obj))->field_72 = 0;

    // Accumulation loop: run the dispatcher per hit and round base+variance.
    // zero stays live in f30 across the calls, mirroring retail. The int->
    // float conversion is left to the compiler: its own 2^52 pool constant
    // is the value retail binds to lbl_eu_80666DE0.
    f32 zero = lbl_eu_80666DDC;
    s32 idx = 0;
    while (idx < (s32)*outCount) {                           // 0x800E99F8
        E2A9C_BattleMoveData* slot = (E2A9C_BattleMoveData*)e_vf298(obj);
        slot->field_78 |= 0x44000000;
        func_800DB0FC((void*)lbl_eu_80663F00, obj, action, slot);
        slot->field_78 = (slot->field_78 & ~0x04000000) | 0x48000000;

        f32 sum = slot->field_5C + slot->field_60;
        s32 ival = 0;
        // retail folds both tests into a single fcmpo against zero
        if (!(sum < zero || (slot->field_74 & 0x80))) {
            ival = (s32)((f64)sum + (sum > zero ? lbl_eu_80666E58 : lbl_eu_80666E60));
        }
        *outDamage += (f32)ival;
        e_vf2B8(obj);                                        // 0x800E9AB4
        idx++;
    }

    // Restore a clean move-data copy into the target block.
    e_vf2AC(obj);                                            // 0x800E9AD4
    E2A9C_BattleMoveData* w = (E2A9C_BattleMoveData*)e_vf298(obj);
    w->field_70 = 0;                                         // 0x800E9AE8
    w = (E2A9C_BattleMoveData*)e_vf298(obj);
    w->field_72 = 0;
    E2A9C_BattleMoveData* srcB = (E2A9C_BattleMoveData*)e_vf298(obj);
    E2A9C_BattleMoveData* dstB = (E2A9C_BattleMoveData*)e_vf2A4(obj);
    *dstB = *srcB;

    if (*outDamage < lbl_eu_80666DDC) *outDamage = lbl_eu_80666DDC;  // 0x800E9CCC
}

#pragma schedule on
// ---- func_800E9B54 (0x800EA63C) --------------------------------------------
// Chain-arts hit spread: refreshes the arts rows, then for each player action
// found in the (0x20, 0x800) enum list emits chain events (0x35 arts / 0x13
// counter) and re-installs the AI slot via func_80109784.
extern "C" void func_800E9B54(void* self, void* target, void* attacker, void* move) {
    EnumListHolder holder;
    u32 out14, out10;
    f32 out0c;
    BattleEventData ev;
    u32 sid;
    cf::CfGameManager::getInstance();
    if (func_8006EF04__Fi(0x04000000)) {                       // 0x800EA664 (lis r3,0x400)
        ((BMVtMirror*)target)->vB8();
        return;
    }

    // 0x800EA68C: status-id via the +4 holder, then the 0x2000000 gate
    {
        void* holder4 = *(void**)((u8*)target + 0x04);
        sid = *(u32*)((BMVtMirror*)holder4)->v30();
        if (func_80174C98(target, (int*)&sid, 0x02000000)) return;
    }

    ((BMVtMirror*)target)->vC8();                            // slot 0xC8 (0x800EA6C0)

    if (attacker != nullptr && (((BattleObjAccessor*)target)->field_3f00 & 0x4)) {
        // 0x800EA6E8: find target in the self+8 list, then the 0x800EA71C call
        SimpleListNode* s = *(SimpleListNode**)((u8*)self + 0x08);
        SimpleListNode* c;
        for (c = s->next; c != s && c->data != target; c = c->next) {}
        // retail: the D9978 call fires when the target was NOT found in the list
        if (c == s) {
            func_800D9978((cf::CBattleManager*)self, (cf::CfObjectActor*)target);
        }
    }

    if (attacker == nullptr) {                               // 0x800EA720
        s32 subId = (s32)(uintptr_t)((BMVtMirror*)((u8*)target + 0x3E9C))->v4C();
        attacker = func_8016FE34(func_800B708C(subId));
    }

    if (attacker != nullptr &&
        (((BattleObjAccessor*)attacker)->field_3f00 & 0x2) &&
        (((BattleObjAccessor*)target)->field_3f00 & 0x4)) {  // 0x800EA748
        if (!(((EC918_BattleObjAccessor*)target)->field_3374 & 0x04000000)) {
            // 0x800EA778: refresh the eight arts rows
            for (s32 i = 1; i <= 8; i++) {
                s32 flag = (move != nullptr)
                    ? (s32)((((E2A9C_BattleMoveData*)move)->field_74 >> 14) & 1)
                    : 0;
                void* arts = func_8009EC9C((u16)i);
                func_800A26A4(arts, (s32)(uintptr_t)((BMVtMirror*)target)->v1E8(),
                              ((BMVtMirror*)target)->v200(),
                              (s32)(uintptr_t)((BMVtMirror*)target)->v108(), flag, 0, 0);
            }
        }

        // 0x800EA80C: count the mActorList3 members (sentinel at self+0x48)
        {
            SimpleListNode* sentinel = *(SimpleListNode**)((u8*)self + 0x48);
            s32 cnt = 0;
            SimpleListNode* cur;
            for (cur = sentinel->next; cur != sentinel; cur = cur->next) { cnt++; }
            if (cnt <= 1) {
                func_802A2E08();
                func_8027F0B8();
                func_80295BF4((void*)lbl_eu_80664A10);
                goto e9B54_tail;
            }
        }

        if (!(((BattleObjAccessor*)attacker)->field_3f00 & 0x2)) goto e9B54_tail;

        // ---- 0x800EA854: enum-list walk ----
        {
            func_80043D90(&holder);
            func_800F4A98(func_80043F18(&holder), 0x20, 0x800);

            const s32 cArt35 = 0x35;
            const s32 cVal1000 = 0x1000;
            const s32 cEvType2 = 0x2;
            const s32 cSel5A = 0x5A;
            const s32 cSel13 = 0x13;

            for (u32 i = 0; i < ((cf::CVisionEnumList*)func_80043F18(&holder))->count; i++) {
                void* obj = func_8016FE34(func_800F6EAC(func_80043F18(&holder), i));
                void* r29 = obj;
                if (obj == attacker) {                       // 0x800EA8A0
                    if (((BMVtMirror*)obj)->v290() &&       // called twice (retail)
                        func_80260FB0((void*)(uintptr_t)((BMVtMirror*)obj)->v290(),
                                      0x35, &out14, &out10, (u32*)&out0c) &&
                        (s32)mtRand__Q22ml4mathFi(100) < (s32)out10) {
                        // 0x800EA908: propagate the (0x35, 0x1000) chain event
                        // to every listed actor.
                        std::memset(&ev, 0, sizeof(ev));
                        ev.field_10 = out14;
                        ev.field_20 = out0c;
                        ev.field_0C = (u16)cEvType2;
                        for (u32 j = 0; j < ((cf::CVisionEnumList*)func_80043F18(&holder))->count; j++) {
                            void* sub = func_8016FE34(func_800F6EAC(func_80043F18(&holder), j));
                            ev.field_00 = (u32)(uintptr_t)((BattleObjAccessor*)sub)->field_3f10;
                            ev.field_04 = (u32)cArt35;
                            ev.field_08 = (u32)cVal1000;
                            ev.field_30 |= 0x1800;
                            func_800EC918((void*)lbl_eu_80663F00, 0,
                                          (EC918_BattleObjAccessor*)sub,
                                          (BattleEvent*)&ev, 0);
                            func_80109784(((BattleObjAccessor*)sub)->field_3f10, cArt35, 9);
                        }
                    }
                }
                if (r29 != attacker) {                       // 0x800EA9A8
                    if (((BMVtMirror*)r29)->v290() &&
                        func_80260518((void*)(uintptr_t)((BMVtMirror*)r29)->v290(),
                                      0x13, &out14, &out0c)) {
                        // 0x800EA9F8: counter event (0x5A / 0x13)
                        std::memset(&ev, 0, sizeof(ev));
                        ev.field_10 = out14;
                        ev.field_20 = out0c;
                        ev.field_30 |= 0x1800;
                        ev.field_0C = (u16)cSel5A;
                        ev.field_00 = (u32)(uintptr_t)((BattleObjAccessor*)r29)->field_3f10;
                        ev.field_04 = (u32)cSel13;
                        ev.field_08 = (u32)cVal1000;
                        func_800EC918((void*)lbl_eu_80663F00, 0,
                                      (EC918_BattleObjAccessor*)r29,
                                      (BattleEvent*)&ev, 0);
                        func_80109784(((BattleObjAccessor*)r29)->field_3f10, cSel13, 9);
                    }
                }
            }
            __dt__80043E88(&holder, -1);
        }
        goto e9B54_tail;
    } else {
        // 0x800EAA88: target is a player -- rescue via the help manager
        if (((BattleObjAccessor*)target)->field_3f00 & 0x2) {
            func_80295CC8((void*)lbl_eu_80664A10, target);
        }
    }

e9B54_tail:                                                  // 0x800EAAA0
    func_802A232C(target);
    func_80174B4C(target, 0x2000000);
}

// ---- func_800E2594 (0x800E307C) --------------------------------------------
// Move dispatcher for sub-object types 1..6: mirrors the move block onto the
// stack, routes to func_800E85F0 / func_800E2A9C / func_800E64CC, then applies
// status/event follow-ups keyed on the local copy's field_78 bits.
extern "C" void func_800E2594(void* self, void* actor, void* target, void* move) {
    void* r28 = actor;       // the giver
    void* r29 = target;
    void* r30 = move;

    if (r29 == nullptr) return;                              // 0x800E30A4

    if (!(((BtlMove*)r30)->mFlags78 & 0x10)) {               // 0x800E30A8
        func_800DB0FC(self, actor, target, move);
    }

    void* sub = ((BtlMove*)r30)->mSub;                       // r31 (0x800E30B8)
    if (sub == nullptr) return;
    if (((E2594_Sub*)sub)->flags78 & 0x8000) return;         // 0x800E30C4

    if (e_vf28(r28, 0x10)) return;                           // 0x800E30D0
    vfBC(r28, r30);                                          // 0x800E30F0

    E2A9C_BattleMoveData local = *((E2A9C_BattleMoveData*)r30);  // 0x800E3108 copy

    u16 type = ((E2594_Sub*)sub)->type3C;                    // 0x800E3288
    if ((u32)(type - 4) <= 2) {                              // types 4,5,6
        func_800E85F0(self, actor, target, move);
    } else if ((u32)(type - 1) <= 1) {                       // types 1,2
        func_800E2A9C((cf::CBattleManager*)self, (cf::CfObjectActor*)actor,
                      (cf::CfObjectActor*)target, (E2A9C_BattleMoveData*)move);
    } else if (type == 3) {
        func_800E64CC((cf::CBattleManager*)self, actor, target, move);
    }

    if (local.field_78 & 0x2) {                              // 0x800E32F4
        if (e_vf308(r28) == 0) {
            e_vf304(r28, 2);
        }
    }

    if (local.field_78 & 0xC00) {                            // 0x800E3334
        bool canCounter = false;
        if (sub != nullptr) {
            if (((E2594_Sub*)sub)->flags78 & 0x1) {
                u16 id28 = ((E2594_Sub*)sub)->h28;
                if ((u16)(id28 - 0x2A) <= 2 || (u16)(id28 - 0x3D) <= 1 || id28 == 1) {
                    canCounter = true;
                }
            }
        }
        if (!canCounter) {
            // 0x800E3394: 0x91 counter status event
            if (func_80148778((u8*)r28 + 8, 0x91)) {
                BtlStatusEntry* entry = (BtlStatusEntry*)func_80149154((u8*)r28 + 8, 0x91);
                BattleEventData ev;
                std::memset(&ev, 0, sizeof(ev));
                ev.field_00 = *(u32*)((u8*)r28 + 0x3F10);
                ev.field_08 = 0x00008000;
                ev.field_0C = 0xD5;
                ev.field_10 = entry->unk10;
                if (((BtlMove*)r30)->mSubId != 0) {
                    func_800EC918((void*)self, (EC918_BattleObjAccessor*)r28,
                                  (EC918_BattleObjAccessor*)r28, (BattleEvent*)&ev, r30);
                } else {
                    func_800EC918((void*)self, (EC918_BattleObjAccessor*)r28,
                                  (EC918_BattleObjAccessor*)r28, (BattleEvent*)&ev, &local);
                }
            }
        }
    }

    if (local.field_78 & 0xC00) {                            // 0x800E342C
        if (func_80148778((u8*)r28 + 8, 0xC1)) {
            if ((((E2594_Sub*)sub)->flags78 & 0x2000) &&
                ((u32)((E2594_Sub*)sub)->b44 != e_vf2A8(r28) + 1) &&
                (local.field_74 & 1) && !(local.field_74 & 0x2) &&
                !(local.field_74 & 0x40)) {
                // guarded status -- leave the entry untouched
            } else {
                void* entryC1 = func_80149154((u8*)r28 + 8, 0xC1);
                if (*(u32*)((u8*)entryC1 + 0x4) != (u32)sub) {
                    e_vf20OnHolder((u8*)r28 + 8, 0xC1);
                }
            }
        }
    }

    if (local.field_78 & 0x800) {                            // 0x800E34C8
        if (func_80148778((u8*)r29 + 8, 0xCE)) {
            if (r28 != r29) {                                // 0x800E34E8
                bool keep = true;
                if (((E2594_Sub*)sub)->flags78 & 0x2000) {
                    if ((u32)((E2594_Sub*)sub)->b44 != e_vf2A8(r28) + 1) {
                        keep = false;                        // bne -> out
                    }
                }
                if (keep && ((local.field_74 & 1) || (local.field_78 & 0x100))) {
                    CE_Entry* entry = (CE_Entry*)func_801491F4((u8*)r29 + 8, 0xCE);
                    if (entry->h2C != local.field_80) {      // 0x800E3548
                        ((void(*)(void*, u32))(*(void***)((u8*)r29 + 8))[0x24 / 4])((u8*)r29 + 8, 0xCE);
                    }
                }
            }
        }
    }
}

// s32 -> f32 conversion via the Gekko 2^52 bias trick. The magic double is
// the named .sdata2 constant so the fsubs reloc binds to lbl_eu_80666DE0
// instead of an MWCC pool @N (CfGimmick.cpp convention).
static inline f32 bmS32ToF32(s32 v, cf::CfActorF64Conv& slot) {
    slot.w[0] = 0x43300000;
    slot.w[1] = (u32)v ^ 0x80000000;
    return (f32)(slot.d - lbl_eu_80666DE0);
}

// Iterates through one of the actor lists (mActorList3 for the flags&2 path,
// else mActorList2) and broadcasts the arts-damage triple (arg2/arg3/arg4 as
// floats) to every idle actor; when arg5 == 1 the broadcast is gated on the
// actor's sub-object id (vtable slot 0x4C) matching arg6.
extern "C" void func_800E9FE4(void* self, void* arg1, s32 arg2, s32 arg3, s32 arg4,
                              s32 arg5, void* arg6) {
    // Two int->float conversion slots (Gekko 2^52-bias trick). Only the
    // sign-flipped low words change per conversion; the 0x43300000 high words
    // are written once up front.
    cf::CfObjectActor* target = (cf::CfObjectActor*)arg1;
    cf::CfActorF64Conv cvt[2];
    cvt[0].w[0] = 0x43300000;
    cvt[1].w[0] = 0x43300000;

    cf::CfGameManager::getInstance();
    if (func_8006EF04__Fi(0x4000000)) {
        return;
    }

    f64 magic = lbl_eu_80666DE0;

    if (func_80148778((u8*)target + 8, 0x101)) {
        if (arg2 != 0) arg2 = 1;
        if (arg3 != 0) arg3 = 1;
        if (arg4 != 0) arg4 = 1;
    }

#define BM9FE4_LOOP(listName) \
    do { \
        /* Volatile head pointer: the sentinel is re-read from memory every */ \
        /* iteration (retail reloads it after each virtual call); MWCC keeps */ \
        /* pHead folded into rBase+0xNN addressing so no extra register is */ \
        /* consumed. The branch-top magic load lands in a callee-saved fpr. */ \
        _reslist_node<cf::CfObjectActor*>* const volatile* pHead = \
            &((cf::CBattleManager*)self)->listName.mStartNodePtr; \
        _reslist_node<cf::CfObjectActor*>* cur = *pHead; \
        magic = lbl_eu_80666DE0; \
        cf::CfObjectActor* obj; \
        while ((cur = cur->mNext) != *pHead) { \
            obj = cur->mItem; \
            if (obj != target && ((cf::CVisionBattleObj*)obj)->vf2BC() == 0) { \
                if (arg5 == 1) { \
                    if ((u32)(uintptr_t)arg6 == ((cf::CChainBattleObjB38*)obj)->mSub.v17()) { \
                        cvt[0].w[1] = (u32)arg2 ^ 0x80000000; \
                        cvt[1].w[1] = (u32)arg3 ^ 0x80000000; \
                        f32 f1 = (f32)(cvt[0].d - magic); \
                        cvt[0].w[1] = (u32)arg4 ^ 0x80000000; \
                        f32 f2 = (f32)(cvt[1].d - magic); \
                        f32 f3 = (f32)(cvt[0].d - magic); \
                        ((cf::CVisionBattleObj*)obj)->vf2C4(target, f1, f2, f3); \
                    } \
                } else { \
                    cvt[0].w[1] = (u32)arg2 ^ 0x80000000; \
                    cvt[1].w[1] = (u32)arg3 ^ 0x80000000; \
                    f32 f1 = (f32)(cvt[0].d - magic); \
                    cvt[0].w[1] = (u32)arg4 ^ 0x80000000; \
                    f32 f2 = (f32)(cvt[1].d - magic); \
                    f32 f3 = (f32)(cvt[0].d - magic); \
                    ((cf::CVisionBattleObj*)obj)->vf2C4(target, f1, f2, f3); \
                } \
            } \
        } \
    } while (0)

    if (((BattleObjAccessor*)target)->field_3f00 & 0x2) {
        BM9FE4_LOOP(mActorList3);
    } else {
        BM9FE4_LOOP(mActorList2);
    }
#undef BM9FE4_LOOP
}
// Mirror of the actor vtable used to reach slot 0x2C8 as a direct virtual
// call: MWCC then routes the dispatch through r12 (retail order) instead of a
// general temp register. The 177 placeholder slots (0x04..0x2C4) are never
// called - only the 0x2C8 slot is used, against the actor's real vtable.
struct ActorVtableMirror {
    virtual ~ActorVtableMirror() {}
    virtual void a004(); virtual void a008(); virtual void a00C(); virtual void a010();
    virtual void a014(); virtual void a018(); virtual void a01C(); virtual void a020();
    virtual void a024(); virtual void a028(); virtual void a02C(); virtual void a030();
    virtual void a034(); virtual void a038(); virtual void a03C(); virtual void a040();
    virtual void a044(); virtual void a048(); virtual void a04C(); virtual void a050();
    virtual void a054(); virtual void a058(); virtual void a05C(); virtual void a060();
    virtual void a064(); virtual void a068(); virtual void a06C(); virtual void a070();
    virtual void a074(); virtual void a078(); virtual void a07C(); virtual void a080();
    virtual void a084(); virtual void a088(); virtual void a08C(); virtual void a090();
    virtual void a094(); virtual void a098(); virtual void a09C(); virtual void a0A0();
    virtual void a0A4(); virtual void a0A8(); virtual void a0AC(); virtual void a0B0();
    virtual void a0B4(); virtual void a0B8(); virtual void a0BC(); virtual void a0C0();
    virtual void a0C4(); virtual void a0C8(); virtual void a0CC(); virtual void a0D0();
    virtual void a0D4(); virtual void a0D8(); virtual void a0DC(); virtual void a0E0();
    virtual void a0E4(); virtual void a0E8(); virtual void a0EC(); virtual void a0F0();
    virtual void a0F4(); virtual void a0F8(); virtual void a0FC(); virtual void a100();
    virtual void a104(); virtual void a108(); virtual void a10C(); virtual void a110();
    virtual void a114(); virtual void a118(); virtual void a11C(); virtual void a120();
    virtual void a124(); virtual void a128(); virtual void a12C(); virtual void a130();
    virtual void a134(); virtual void a138(); virtual void a13C(); virtual void a140();
    virtual void a144(); virtual void a148(); virtual void a14C(); virtual void a150();
    virtual void a154(); virtual void a158(); virtual void a15C(); virtual void a160();
    virtual void a164(); virtual void a168(); virtual void a16C(); virtual void a170();
    virtual void a174(); virtual void a178(); virtual void a17C(); virtual void a180();
    virtual void a184(); virtual void a188(); virtual void a18C(); virtual void a190();
    virtual void a194(); virtual void a198(); virtual void a19C(); virtual void a1A0();
    virtual void a1A4(); virtual void a1A8(); virtual void a1AC(); virtual void a1B0();
    virtual void a1B4(); virtual void a1B8(); virtual void a1BC(); virtual void a1C0();
    virtual void a1C4(); virtual void a1C8(); virtual void a1CC(); virtual void a1D0();
    virtual void a1D4(); virtual void a1D8(); virtual void a1DC(); virtual void a1E0();
    virtual void a1E4(); virtual void a1E8(); virtual void a1EC(); virtual void a1F0();
    virtual void a1F4(); virtual void a1F8(); virtual void a1FC(); virtual void a200();
    virtual void a204(); virtual void a208(); virtual void a20C(); virtual void a210();
    virtual void a214(); virtual void a218(); virtual void a21C(); virtual void a220();
    virtual void a224(); virtual void a228(); virtual void a22C(); virtual void a230();
    virtual void a234(); virtual void a238(); virtual void a23C(); virtual void a240();
    virtual void a244(); virtual void a248(); virtual void a24C(); virtual void a250();
    virtual void a254(); virtual void a258(); virtual void a25C(); virtual void a260();
    virtual void a264(); virtual void a268(); virtual void a26C(); virtual void a270();
    virtual void a274(); virtual void a278(); virtual void a27C(); virtual void a280();
    virtual void a284(); virtual void a288(); virtual void a28C(); virtual void a290();
    virtual void a294(); virtual void a298(); virtual void a29C(); virtual void a2A0();
    virtual void a2A4(); virtual void a2A8(); virtual void a2AC(); virtual void a2B0();
    virtual void a2B4(); virtual void a2B8(); virtual void a2BC();
    virtual void vf2C8(void* arg); // slot 0x2C8
};

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
                // Direct vtable[0x2C8] dispatch so MWCC uses r12 (retail order).
                reinterpret_cast<ActorVtableMirror*>(actor)->vf2C8((void*)arg1->field_3f10);
            }
            cur = cur->mNext;
        }
    } else {
        _reslist_node<cf::CfObjectActor*>* cur = mgr->mActorList2.mStartNodePtr->mNext;
        while (cur != mgr->mActorList2.mStartNodePtr) {
            cf::CfObjectActor* actor = cur->mItem;
            if (actor != (cf::CfObjectActor*)arg1) {
                // Direct vtable[0x2C8] dispatch so MWCC uses r12 (retail order).
                reinterpret_cast<ActorVtableMirror*>(actor)->vf2C8((void*)arg1->field_3f10);
            }
            cur = cur->mNext;
        }
    }
}
// Mirror vtable routing slots 0x88 and 0x5C4 through MWCC's canonical r12
// virtual-call sequence (cf. ActorVtableMirror). All other slots are unused
// placeholders declared against the enum-cast objects' real vtables.
struct E484_Mirror {
    virtual ~E484_Mirror() {}
    // NB: the destructor occupies two vtable entries (complete + deleting),
    // so the first placeholder lands at +0x08.
    virtual void p00C(); virtual void p010();
    virtual void p014(); virtual void p018(); virtual void p01C(); virtual void p020();
    virtual void p024(); virtual void p028(); virtual void p02C(); virtual void p030();
    virtual void p034(); virtual void p038(); virtual void p03C(); virtual void p040();
    virtual void p044(); virtual void p048(); virtual void p04C(); virtual void p050();
    virtual void p054(); virtual void p058(); virtual void p05C(); virtual void p060();
    virtual void p064(); virtual void p068(); virtual void p06C(); virtual void p070();
    virtual void p074(); virtual void p078(); virtual void p07C(); virtual void p080();
    virtual void p084();
    virtual void vt88(f32);            // slot 0x88
    virtual void p08C(); virtual void p090(); virtual void p094(); virtual void p098();
    virtual void p09C(); virtual void p0A0(); virtual void p0A4(); virtual void p0A8();
    virtual void p0AC(); virtual void p0B0(); virtual void p0B4(); virtual void p0B8();
    virtual void p0BC(); virtual void p0C0(); virtual void p0C4(); virtual void p0C8();
    virtual void p0CC(); virtual void p0D0(); virtual void p0D4(); virtual void p0D8();
    virtual void p0DC(); virtual void p0E0(); virtual void p0E4(); virtual void p0E8();
    virtual void p0EC(); virtual void p0F0(); virtual void p0F4(); virtual void p0F8();
    virtual void p0FC(); virtual void p100(); virtual void p104(); virtual void p108();
    virtual void p10C(); virtual void p110(); virtual void p114(); virtual void p118();
    virtual void p11C(); virtual void p120(); virtual void p124(); virtual void p128();
    virtual void p12C(); virtual void p130(); virtual void p134(); virtual void p138();
    virtual void p13C(); virtual void p140(); virtual void p144(); virtual void p148();
    virtual void p14C(); virtual void p150(); virtual void p154(); virtual void p158();
    virtual void p15C(); virtual void p160(); virtual void p164(); virtual void p168();
    virtual void p16C();
    virtual void q170(); virtual void q174(); virtual void q178(); virtual void q17C();
    virtual void q180(); virtual void q184(); virtual void q188(); virtual void q18C();
    virtual void q190(); virtual void q194(); virtual void q198(); virtual void q19C();
    virtual void q1A0(); virtual void q1A4(); virtual void q1A8(); virtual void q1AC();
    virtual void q1B0(); virtual void q1B4(); virtual void q1B8(); virtual void q1BC();
    virtual void q1C0(); virtual void q1C4(); virtual void q1C8(); virtual void q1CC();
    virtual void q1D0(); virtual void q1D4(); virtual void q1D8(); virtual void q1DC();
    virtual void q1E0(); virtual void q1E4(); virtual void q1E8(); virtual void q1EC();
    virtual void q1F0(); virtual void q1F4(); virtual void q1F8(); virtual void q1FC();
    virtual void q200(); virtual void q204(); virtual void q208(); virtual void q20C();
    virtual void q210(); virtual void q214(); virtual void q218(); virtual void q21C();
    virtual void q220(); virtual void q224(); virtual void q228(); virtual void q22C();
    virtual void q230(); virtual void q234(); virtual void q238(); virtual void q23C();
    virtual void q240(); virtual void q244(); virtual void q248(); virtual void q24C();
    virtual void q250(); virtual void q254(); virtual void q258(); virtual void q25C();
    virtual void q260(); virtual void q264(); virtual void q268(); virtual void q26C();
    virtual void q270(); virtual void q274(); virtual void q278(); virtual void q27C();
    virtual void q280(); virtual void q284(); virtual void q288(); virtual void q28C();
    virtual void q290(); virtual void q294(); virtual void q298(); virtual void q29C();
    virtual void q2A0(); virtual void q2A4(); virtual void q2A8(); virtual void q2AC();
    virtual void q2B0(); virtual void q2B4(); virtual void q2B8(); virtual void q2BC();
    virtual void q2C0(); virtual void q2C4(); virtual void q2C8(); virtual void q2CC();
    virtual void q2D0(); virtual void q2D4(); virtual void q2D8(); virtual void q2DC();
    virtual void q2E0(); virtual void q2E4(); virtual void q2E8(); virtual void q2EC();
    virtual void q2F0(); virtual void q2F4(); virtual void q2F8(); virtual void q2FC();
    virtual void q300(); virtual void q304(); virtual void q308(); virtual void q30C();
    virtual void q310(); virtual void q314(); virtual void q318(); virtual void q31C();
    virtual void q320(); virtual void q324(); virtual void q328(); virtual void q32C();
    virtual void q330(); virtual void q334(); virtual void q338(); virtual void q33C();
    virtual void q340(); virtual void q344(); virtual void q348(); virtual void q34C();
    virtual void q350(); virtual void q354(); virtual void q358(); virtual void q35C();
    virtual void q360(); virtual void q364(); virtual void q368(); virtual void q36C();
    virtual void q370(); virtual void q374(); virtual void q378(); virtual void q37C();
    virtual void q380(); virtual void q384(); virtual void q388(); virtual void q38C();
    virtual void q390(); virtual void q394(); virtual void q398(); virtual void q39C();
    virtual void q3A0(); virtual void q3A4(); virtual void q3A8(); virtual void q3AC();
    virtual void q3B0(); virtual void q3B4(); virtual void q3B8(); virtual void q3BC();
    virtual void q3C0(); virtual void q3C4(); virtual void q3C8(); virtual void q3CC();
    virtual void q3D0(); virtual void q3D4(); virtual void q3D8(); virtual void q3DC();
    virtual void q3E0(); virtual void q3E4(); virtual void q3E8(); virtual void q3EC();
    virtual void q3F0(); virtual void q3F4(); virtual void q3F8(); virtual void q3FC();
    virtual void q400(); virtual void q404(); virtual void q408(); virtual void q40C();
    virtual void q410(); virtual void q414(); virtual void q418(); virtual void q41C();
    virtual void q420(); virtual void q424(); virtual void q428(); virtual void q42C();
    virtual void q430(); virtual void q434(); virtual void q438(); virtual void q43C();
    virtual void q440(); virtual void q444(); virtual void q448(); virtual void q44C();
    virtual void q450(); virtual void q454(); virtual void q458(); virtual void q45C();
    virtual void q460(); virtual void q464(); virtual void q468(); virtual void q46C();
    virtual void q470(); virtual void q474(); virtual void q478(); virtual void q47C();
    virtual void q480(); virtual void q484(); virtual void q488(); virtual void q48C();
    virtual void q490(); virtual void q494(); virtual void q498(); virtual void q49C();
    virtual void q4A0(); virtual void q4A4(); virtual void q4A8(); virtual void q4AC();
    virtual void q4B0(); virtual void q4B4(); virtual void q4B8(); virtual void q4BC();
    virtual void q4C0(); virtual void q4C4(); virtual void q4C8(); virtual void q4CC();
    virtual void q4D0(); virtual void q4D4(); virtual void q4D8(); virtual void q4DC();
    virtual void q4E0(); virtual void q4E4(); virtual void q4E8(); virtual void q4EC();
    virtual void q4F0(); virtual void q4F4(); virtual void q4F8(); virtual void q4FC();
    virtual void q500(); virtual void q504(); virtual void q508(); virtual void q50C();
    virtual void q510(); virtual void q514(); virtual void q518(); virtual void q51C();
    virtual void q520(); virtual void q524(); virtual void q528(); virtual void q52C();
    virtual void q530(); virtual void q534(); virtual void q538(); virtual void q53C();
    virtual void q540(); virtual void q544(); virtual void q548(); virtual void q54C();
    virtual void q550(); virtual void q554(); virtual void q558(); virtual void q55C();
    virtual void q560(); virtual void q564(); virtual void q568(); virtual void q56C();
    virtual void q570(); virtual void q574(); virtual void q578(); virtual void q57C();
    virtual void q580(); virtual void q584(); virtual void q588(); virtual void q58C();
    virtual void q590(); virtual void q594(); virtual void q598(); virtual void q59C();
    virtual void q5A0(); virtual void q5A4(); virtual void q5A8(); virtual void q5AC();
    virtual void q5B0(); virtual void q5B4(); virtual void q5B8(); virtual void q5BC();
    virtual void q5C0();
    virtual void vt5C4(f32);           // slot 0x5C4
};

// ---- func_800EA484 (0x800EAF6C) --------------------------------------------
// Update dispatch for the battle-manager refresh flags (bits 0x1 player list,
// 0x2 player pairs, 0x4 vision actors, 0x8 enemy scene, 0x10 raygun, 0x20 /
// 0x40 targeted filters). Each enabled step re-filters an enum list created by
// func_80043D90 and broadcasts the given value over vtable slot 0x88 on the
// matching objects.
extern "C" void func_800EA484(cf::CBattleManager* self, f32 value, int flags) {

    if (flags & 0x8) {                                       // 0x800EAF78
        func_80496294(lbl_eu_80663E14);
    }

    EnumListHolder h1;    // sp+0x10
    EnumListHolder h2;    // sp+0x08
    func_80043D90(&h1);
    func_80043D90(&h2);

    if (flags & 0x1) {                                       // 0x800EAFAC
        func_800F4A98(func_80043F18(&h1), 0x408, 0);
        for (u32 i = 0; i < ((cf::CVisionEnumList*)func_80043F18(&h1))->count; i++) {
            void* o = func_800F6EAC(func_80043F18(&h1), i);
            void* c = __dynamic_cast(o, 0, &lbl_eu_806618E8, &lbl_eu_806618F0, 0);
            reinterpret_cast<E484_Mirror*>(c)->vt88(value);
        }
    }

    if (flags & 0x2) {                                       // 0x800EB020
        func_800F4A98(func_80043F18(&h1), (0x40 << 16) + 0x320, 1);
        func_800F4A98(func_80043F18(&h2), 0x80000, 0);
        for (u32 i = 0; i < ((cf::CVisionEnumList*)func_80043F18(&h1))->count; i++) {
            void* o = func_800F6EAC(func_80043F18(&h1), i);
            void* r28 = __dynamic_cast(o, 0, &lbl_eu_806618E8, &lbl_eu_806618F0, 0);
            reinterpret_cast<E484_Mirror*>(r28)->vt88(value);
            for (u32 j = 0; j < ((cf::CVisionEnumList*)func_80043F18(&h2))->count; j++) {
                void* o2 = func_800F6EAC(func_80043F18(&h2), j);
                void* r26 = __dynamic_cast(o2, 0, &lbl_eu_80661970, &lbl_eu_806618F0, 0);
                if (((E484_TypeObj*)r26)->field_9C == r28)
                    reinterpret_cast<E484_Mirror*>(r26)->vt88(value);
                void* a = func_8016FE34(func_800B708C((s32)(uintptr_t)r28));
                if (a != nullptr) {
                    void* b = ((E484_VisionActor*)a)->field_45B8;
                    if (b != nullptr) {
                        void* c = func_8016FE34(func_800B708C((s32)(uintptr_t)b));
                        if (c != nullptr) c = (u8*)c + 0x3E9C;
                        if (((E484_TypeObj*)r26)->field_9C == c)
                            reinterpret_cast<E484_Mirror*>(r26)->vt88(value);
                    }
                }
            }
        }
    }

    if (flags & 0x10) {                                      // 0x800EB170
        ((E484_Global6C*)func_80044DF4())->field_6C = value;
    }

    if (flags & 0x20) {                                      // 0x800EB180
        func_800F4A98(func_80043F18(&h1), 0x80000, 0);
        for (u32 i = 0; i < ((cf::CVisionEnumList*)func_80043F18(&h1))->count; i++) {
            void* o = func_800F6EAC(func_80043F18(&h1), i);
            void* r3 = __dynamic_cast(o, 0, &lbl_eu_80661970, &lbl_eu_806618F0, 0);
            // int local over the u16 field: retail emits a zero-extending
            // lhz but signed cmpi chains.
            int t = ((E484_TypeObj*)r3)->type8C;
            // 0x800EB1CC: four type ranges trigger the broadcast. Written as
            // a sparse switch so MWCC lowers it to the same binary-search
            // comparison tree as retail.
            switch (t) {
            case 0x3D: case 0x3E: case 0x3F: case 0x40: case 0x41:
            case 0x42: case 0x43: case 0x44: case 0x45: case 0x46:
            case 0x47: case 0x48: case 0x49: case 0x4A: case 0x4B:
            case 0x4C:
            case 0x51: case 0x52: case 0x53: case 0x54: case 0x55:
            case 0x56: case 0x57: case 0x58: case 0x59:
            case 0x65: case 0x66:
            case 0x79: case 0x7A: case 0x7B: case 0x7C: case 0x7D:
                reinterpret_cast<E484_Mirror*>(r3)->vt88(value);
                break;
            default:
                break;
            }
        }
    }

    if (flags & 0x40) {                                      // 0x800EB244
        func_800F4A98(func_80043F18(&h1), 0x80000, 0);
        for (u32 i = 0; i < ((cf::CVisionEnumList*)func_80043F18(&h1))->count; i++) {
            void* o = func_800F6EAC(func_80043F18(&h1), i);
            void* r3 = __dynamic_cast(o, 0, &lbl_eu_80661970, &lbl_eu_806618F0, 0);
            int t = ((E484_TypeObj*)r3)->type8C;
            if ((u32)(t - 0xA1) <= 3 || (u32)(t - 0xC0) <= 2 || t == 0xB8)
                reinterpret_cast<E484_Mirror*>(r3)->vt88(value);
        }
    }

    if (flags & 0x4) {                                       // 0x800EB2DC
        void* vision;
        if (lbl_eu_80663F00 != 0) {
            vision = func_801A8070((u8*)self + 0x219C);
        } else {
            vision = nullptr;
        }
        if (vision != nullptr) {
            E484_VisionPair* vp = (E484_VisionPair*)vision;
            void* r26 = func_8016FE34(func_800B708C((s32)vp->field_00));
            void* r27 = func_8016FE34(func_800B708C((s32)vp->field_04));
            reinterpret_cast<E484_Mirror*>(r26)->vt5C4(value);
            reinterpret_cast<E484_Mirror*>(r27)->vt5C4(value);
            func_800F4A98(func_80043F18(&h2), 0x80000, 0);
            for (u32 i = 0; i < ((cf::CVisionEnumList*)func_80043F18(&h2))->count; i++) {
                void* r29 = __dynamic_cast(func_800F6EAC(func_80043F18(&h2), i), 0, &lbl_eu_80661970, &lbl_eu_806618F0, 0);
                // Match the cast object's +0x9C pointer against either vision
                // actor (or its embedded move sub-object at +0x3E9C).
                // Re-read the pair fields through vision each iteration:
                // retail recomputes the +0x3E9C adjustment per pass (the
                // vt88 virtual call bars hoisting the memory load).
                if ((func_8016FE34(func_800B708C((s32)vp->field_00)) != nullptr &&
                     ((E484_TypeObj*)r29)->field_9C == (u8*)func_8016FE34(func_800B708C((s32)vp->field_00)) + 0x3E9C) ||
                    (func_8016FE34(func_800B708C((s32)vp->field_04)) != nullptr &&
                     ((E484_TypeObj*)r29)->field_9C == (u8*)func_8016FE34(func_800B708C((s32)vp->field_04)) + 0x3E9C))
                    reinterpret_cast<E484_Mirror*>(r29)->vt88(value);
                if (r26 != nullptr) {
                    void* b = ((E484_VisionActor*)r26)->field_45B8;
                    if (b != nullptr) {
                        void* c = func_8016FE34(func_800B708C((s32)(uintptr_t)b));
                        if (c != nullptr) c = (u8*)c + 0x3E9C;
                        if (((E484_TypeObj*)r29)->field_9C == c)
                            reinterpret_cast<E484_Mirror*>(r29)->vt88(value);
                    }
                }
            }
        }
    }

    __dt__80043E88(&h2, -1);
    __dt__80043E88(&h1, -1);
}
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
    // Bit table for sel 0..6 is {6, 31, 0, 1, 2, 3, 4}; retail pools no such
    // storage, so keep it as branch selects (a static array here would emit
    // a phantom .sdata2 object with no retail counterpart).
    u8* p = (u8*)obj;
#define KY_BITSHIFT(sel) \
    ((u8)((sel) == 0 ? 6 : (sel) == 1 ? 31 : (sel) == 2 ? 0 : (sel) == 3 ? 1 : \
                        (sel) == 4 ? 2 : (sel) == 5 ? 3 : 4))

    if (sel <= 3) {
        return (p[byteOff] >> KY_BITSHIFT(sel)) & 1;
    }
    if (sel <= 9) {
        s16 v = *(s16*)(p + 0x64 + (sel - 4) * 2);   // +100..+110
        if (v < thresh) {
            if (sel <= 6) return (p[byteOff] >> KY_BITSHIFT(sel)) & 1;
            return 0;
        }
        return 1;
    }
    return 0;
#undef KY_BITSHIFT
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
extern void func_8010975C(u8 val);
extern void func_80109770(u8 val);
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
// Dispatches battle event calls to actor objects based on the dispatch-table
// entry at lbl_eu_804FC828[idx]: selector 0 -> obj1, 1 -> obj2, 2 -> both,
// anything else -> every entry of the manager's second actor list.
// Signature matches the single shared decl on CBattleManagerApi.hpp; the
// typed views are recovered up front (pointer casts emit no code).
void func_800F3970(void* selfV, void* obj1V, void* obj2V, s32 idx, s32 addVal) {
    cf::CBattleManager* self = (cf::CBattleManager*)selfV;
    BattleObjAccessor* obj1 = (BattleObjAccessor*)obj1V;
    BattleObjAccessor* obj2 = (BattleObjAccessor*)obj2V;
    // Declaration order drives the saved-register colors. Signed selector so
    // the dispatch compares with retail cmpwi (cmpli would come from unsigned).
    const BattleTableEntry* entry = &lbl_eu_804FC828[idx];
    s32 selector = entry->selector;
    s32 val3 = entry->val;
    s32 val2 = -1;
    // Explicit copy pins the parameter homing (retail or r28,r5,r5).
    cf::CfObjectActor* o2 = (cf::CfObjectActor*)obj2;

    if (val3 == 0x3e7) {
        val2 = 4;
        val3 = 0;
    }

    val3 += addVal;

    // Read the byte once: retail keeps it in a saved register across the
    // lookup call (extsb re-uses it for both the test and the multiply).
    s8 byteVal = lbl_eu_804FC828[idx].byteVal;
    if (byteVal != 0) {
        // Cross-actor id lookup (CfGameManager); feeds a /1000 multiplier.
        s32 result = func_800824FC__Q22cf13CfGameManagerFv(
            obj1->field_3f28,
            ((BattleObjAccessor*)o2)->field_3f28);
        if (result != -1) {
            // mulhw 0x10624DD3 with >>38 is the signed /1000 magic (not /100).
            val3 += (result / 1000) * (s32)byteVal;
        }
    }

    if (selector == 0) {
        if (val2 != -1) {
            ((BMVtIfF3970*)obj1)->vf304(val2);
            ((BMVtIfF3970*)obj1)->vf2FC(val3);
        } else {
            ((BMVtIfF3970*)obj1)->vf2F8(val3);
        }
    } else if (selector == 1) {
        if (val2 != -1) {
            ((BMVtIfF3970*)o2)->vf304(val2);
            ((BMVtIfF3970*)o2)->vf2FC(val3);
        } else {
            ((BMVtIfF3970*)o2)->vf2F8(val3);
        }
    } else if (selector == 2) {
        if (val2 != -1) {
            ((BMVtIfF3970*)obj1)->vf304(val2);
            ((BMVtIfF3970*)obj1)->vf2FC(val3);
        } else {
            ((BMVtIfF3970*)obj1)->vf2F8(val3);
        }

        if (val2 != -1) {
            ((BMVtIfF3970*)o2)->vf304(val2);
            ((BMVtIfF3970*)o2)->vf2FC(val3);
        } else {
            ((BMVtIfF3970*)o2)->vf2F8(val3);
        }
    } else {
        // Iterate every node of mActorList2, dispatching to each item.
        _reslist_node<cf::CfObjectActor*>* cur =
            self->mActorList2.mStartNodePtr->mNext;
        while (cur != self->mActorList2.mStartNodePtr) {
            cf::CfObjectActor* actor = cur->mItem;
            if (val2 != -1) {
                ((BMVtIfF3970*)actor)->vf304(val2);
                ((BMVtIfF3970*)actor)->vf2FC(val3);
            } else {
                ((BMVtIfF3970*)actor)->vf2F8(val3);
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
    // Gate: vtable slot 0x28 (func_800885F0) with mask 2.
    if (!((BMVtIf828*)mgr)->v008(2)) return;

    // Scan all 32 slots for a matching key (increment its count), else
    // remember the first empty slot and insert there.
    cf::CBattleManager_Struct1* slot = mgr->unk94.unk0;
    s32 best = -1;

    for (s32 i = 0; i < 32; i++) {
        if (slot->key == key) {
            slot->count++;
            return;
        }
        if (slot->key == 0 && best < 0) {
            best = i;
        }
        slot++;
    }

    // Insert into the first empty slot: store the key and bump the count
    // (retail lbz/addi/stb read-modify-write).
    if (best == -1) return;
    cf::CBattleManager_Struct1* chosen = &mgr->unk94.unk0[best];
    chosen->key = key;
    chosen->count++;
}
// Searches for an actor with field_0x15F0 matching arg1.
// Pass 1 scans mActorList3 directly; pass 2 counts mActorList3 (empty list
// falls through); pass 3 scans mActorList1, resolving each actor's move
// sub-object probe through func_800B708C/func_8016FE34.
// Returns 1 if found, 0 otherwise.
s32 func_800F3E8C(cf::CBattleManager* mgr, s32 arg1) {
    // Pass 1: search mActorList3 for a direct type-id match.
    _reslist_node<cf::CfObjectActor*>* end = mgr->mActorList3.mStartNodePtr;
    _reslist_node<cf::CfObjectActor*>* cur = end->mNext;
    while (cur != end) {
        if ((s32)((BattleScanActorView*)cur->mItem)->field_15F0 == arg1)
            return 1;
        cur = cur->mNext;
    }

    // Pass 2: count mActorList3; a non-empty list means no candidate exists.
    // Pass 3 only runs when the list was empty; both fall through to the
    // shared `return 0`.
    s32 count = 0;
    BMSentinelView* view3 = (BMSentinelView*)mgr;
    _reslist_node<cf::CfObjectActor*>* sent3 = view3->sentinel;
    cur = sent3->mNext;
    while (cur != sent3) {
        count++;
        cur = cur->mNext;
    }

    if (count == 0) {
        // Pass 3: scan mActorList1 via each actor's move sub-object probe.
        cur = mgr->mActorList1.mStartNodePtr->mNext;
        while (cur != mgr->mActorList1.mStartNodePtr) {
            BattleScanActorView* actor = (BattleScanActorView*)cur->mItem;
            s32 vresult = actor->field_3E9C.probeId();
            if (vresult != 0) {
                BattleScanStateView* found =
                    (BattleScanStateView*)func_8016FE34(func_800B708C(vresult));
                if (found != nullptr && (found->field_3F00 & 4) &&
                    (s32)found->field_15F0 == arg1) {
                    return 1;
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
// Retail linker names for helpers called from func_800F4034 / func_800F41A0.
extern "C" void func_80197BA4(void*, u32, u32);
extern "C" void func_8015B11C();
extern "C" s32 CfRes_checkFlags_48000();
extern "C" void func_8013E424(void*, int);
extern "C" void func_802A3144(void*);

void func_800F4034(cf::CBattleManager* mgr) {
    // Chain maintenance
    extern void func_80277B34(cf::CChain*);
    func_80277B34(&mgr->mChain);

    // Setup enum list holder
    EnumListHolder holder;
    func_80043D90(&holder);
    void* list = func_80043F18(&holder);
    func_800F4A98(list, 0x20, 0);

    // Loop with check at bottom, using unsigned comparison to match retail cmplw.
    // element declared first: retail claims r31 (first-born saved) for it,
    // r30 for the counter.
    void* element = nullptr;
    u32 i = 0;
    goto check;
loop:
    {
        element = func_8016FE34(func_800F6EAC(func_80043F18(&holder), i));
        // Retail uses canonical r12 virtual dispatch (slots 0xB0/0xB8/0x314).
        ((CSuddenCommuActorVt*)element)->v42();
        ((CSuddenCommuActorVt*)element)->v44();
        ((CSuddenCommuActorVt*)element)->vf314();
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
        _reslist_node<cf::CfObjectActor*>* cur =
            mgr->mActorList3.mStartNodePtr->mNext;
        while (cur != mgr->mActorList3.mStartNodePtr) {
            cf::CfObjectActor* actor = cur->mItem;
            // Retail: store flags first, then branch over the moveBase
            // adjustment only (adjusted-this pattern: the call still happens
            // with the raw pointer when actor is null).
            *(u32*)((u8*)actor + 0x3f04) |= 0x40;

            {
                void* base = (actor != nullptr) ? (void*)((u8*)actor + 0x3e9c) : (void*)actor;
                void* result = func_800AD860(base);
                if (result != nullptr) {
                    *(u32*)((u8*)result + 0x3f08) |= 0x8000000;
                    func_80197BA4(result, 0, 0);
                }
            }

            cur = cur->mNext;
        }
    }

    // Iterate through mActorList2 (sentinel at offset 0x28)
    {
        _reslist_node<cf::CfObjectActor*>* cur =
            mgr->mActorList2.mStartNodePtr->mNext;
        while (cur != mgr->mActorList2.mStartNodePtr) {
            cf::CfObjectActor* actor = cur->mItem;
            func_800BE12C((u8*)actor + 0x3e9c, 0x31, 0, -1, 1);
            cur = cur->mNext;
        }
    }
}
// Handles battle finish / state transition logic.
// Checks various battle state flags and performs cleanup.
void func_800F41A0(cf::CBattleManager* mgr) {
    extern u8 lbl_eu_80573EEC[];
    if (*(u32*)(lbl_eu_80573EEC + 0xd0) == 0) return;

    cf::CfGameManager::getInstance();
    if (func_8006EF04__Fi(4)) goto early_return;

    cf::CfGameManager::getInstance();
    if (CfRes_checkFlags_48000()) goto early_return;

    cf::CfGameManager::getInstance();
    extern s32 CfRes_getE24Bit22();
    if (CfRes_getE24Bit22()) goto early_return;

    // Main path: set up enum list and iterate
    {
        // func_80043D90 / func_80043F18 / __dt__80043E88 come from
        // CVision.hpp as extern "C" (void* params); no local re-declaration.
        extern void func_800F4A98(void*, u32, u32);

        EnumListHolder holder;
        func_80043D90(&holder);
        void* list = func_80043F18(&holder);
        func_800F4A98(list, 0x20, 0x800);

        list = func_80043F18(&holder);
        if (*(u32*)((u8*)list + 0x620) != 0) {
            func_8013E424(lbl_eu_80573EEC, 0);

            func_8015B11C();

            for (int i = 0; i < 3; i++) {
                extern void* getPlayer__Q22cf13CfGameManagerFi(int);
                void* player = getPlayer__Q22cf13CfGameManagerFi(i);
                if (player != nullptr) {
                    if (*(u16*)((u8*)player + 0x8c) == 6) {
                        void* action = func_8016FE34(player);
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
