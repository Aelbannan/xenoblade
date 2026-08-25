// Auto-scaffolded catalog TU for kyoshin/cf/chain/CChainActorEne
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/cf/CBattleManagerApi.hpp"
#include "kyoshin/cf/CfMapItemManager.hpp"
#include "kyoshin/harness_catalog.hpp"
#include "kyoshin/cf/chain/CChainActorEne.hpp"
#include "kyoshin/cf/chain/CChainActorPc.hpp"
#include "kyoshin/cf/chain/CChainEffect.hpp"

// ---- Cross-TU helper declarations ---------------------------------------
// func_800AD860 / func_800B708C are mangled C++ retail symbols; the rest are
// plain (unmangled) C-ABI functions and are declared with C linkage.
extern "C" void* func_802A0804(u32 cls, void* src);
extern "C" void func_8018C8F4(u8* self, u32 ptg);
extern "C" void* func_80193670(void);
extern "C" void* func_80193CD0(void* list, void* obj);
extern "C" void* func_80193AB0(void* list, u32 id);
extern "C" void* func_800B6BC8(void);
extern "C" void* func_8016FE34(void* src);
extern "C" int func_80174C98(void* actor, int* outVal, int flags);
extern void* func_800AD860(void*);
extern void* func_800B708C(int);

// Layout view of the enemy chain actor's timeline object (the object reached
// through this->unk0 (rebased +0x3E9C) and passed through func_800AD860).
struct EneChainObj {
    u8  _pad0[0x3f00];
    u32 flag;   //0x3f00 - bit2 indicates a valid/vital timeline itself
    u8  _pad1[0x24];
    u16 type;   //0x3f28
    u8  _pad2[0x696];
    u16 id;     //0x45c0
};

// Battle-actor mash returned by func_80193AB0; only the low flag word is read.
struct BattleActor {
    u8  _pad0[0xa0];
    u16 flagA0;   //0xa0
};

// Node in the circular gimmick object list returned by func_800B6BC8.
struct GlistNode {
    struct GlistNode* next;   //0x00
    u32  field_04;            //0x04
    void* obj;                //0x08
};

// List header whose 0x04 field is the sentinel node of the circular list.
struct GlistList {
    u32 field_00;             //0x00
    GlistNode* field_04;      //0x04
};

struct CEIfShift { char pad[0x70]; };
struct CEIf : CEIfShift {
    virtual void _v0008();
    virtual void _v000C();
    virtual void _v0010();
    virtual void _v0014();
    virtual void _v0018();
    virtual void _v001C();
    virtual void _v0020();
    virtual void _v0024();
    virtual void _v0028();
    virtual void _v002C();
    virtual void _v0030();
    virtual void _v0034();
    virtual void _v0038();
    virtual void _v003C();
    virtual void _v0040();
    virtual void _v0044();
    virtual void vf0048(void* a);
};

// Operates on the CChainEffect at offset 0x74; compares r4 against effect.unk8's target
void func_802A0AA0(cf::CChainEffect* effect);

// Resets chain state and clears the chain effect.
extern "C" void func_80281308(cf::CChainActorEne* self, int val) {
    CChain_setFieldAndClear(self, val);
    func_802A08F4(self->mChainEffectRaw);
}

// Runs the manual vtable enter-hook (offset 0x50), releases the effect, then
// notifies the chain scheduler via func_80279DC0.
// Resource view of the chain actor used to reach the manually-managed vtable
// pointer at +0x70 (CChainActor::mVTable is declared as a raw u32, which
// disturbs dispatch codegen; a typed local view keeps the indirect call
// allocation identical to retail).
struct VtCast { u8 pad[0x70]; void** vt; };

extern "C" void func_8028133C(cf::CChainActorEne* self) {
    // Dispatch through the manual vtable at +0x70, slot +0x50 (index 20).
    reinterpret_cast<cf::CChainActorEneVtDispatch*>(self)->wf50();
    func_802A0904(self->mChainEffectRaw);
    func_80279DC0(self);
}

// Validates an enemy-chain candidate: non-null source, resolves the parts
// object id, and only then runs the chain logic.
extern "C" int func_80281384(cf::CChainActorEne* self, int arg) {
    if (arg == 0) return 0;
    void* handle = func_800B708C(arg);
    void* obj = func_8016FE34(handle);
    if (obj == 0) return 0;
    // sub is the object at obj+4; its vtable-slot-12 hook (offset 0x30) returns
    // a pointer whose first field is the id matched against the style flag.
    void* sub = *(void**)((u8*)obj + 4);
    int v = *(int*)reinterpret_cast<cf::ChainSubDispatch*>(sub)->target();
    if (func_80174C98(obj, &v, 0x803) != 0) {
        return func_8027A024((void*)self, (void*)arg);
    }
    return 0;
}

// 4-arg effect bind: fills the constant 0xb9 class slot, forwards the other
// three args, and drives the 6th param from a non-zero flag sentinel.
extern "C" void func_80281438(cf::CChainActorEne* self, int p1, int p2, int p3) {
    func_802A0950(reinterpret_cast<cf::CChainEffect*>(self->mChainEffectRaw), p1, 0xb9, (int)self, p2, p3 != 0 ? 0x5f : 0);
}

// Tail-calls func_802A0804 binding the actor to the 0xba effect class.
extern "C" void* func_80281460(cf::CChainActorEne* self) {
    return func_802A0804(0xba, self);
}

// True if this actor's timeline object is the one currently wired into the
// battle parts system, or if it carries the special 0x96b enemy-chain type.
extern "C" int func_8028146C(const cf::CChainActorEne* self) {
    u32 addr = self->unk0;
    if (addr != 0) addr += 0x3e9c;
    EneChainObj* obj = (EneChainObj*)func_800AD860((void*)addr);
    if (obj != 0) {
        if (obj == (EneChainObj*)func_80193CD0(func_80193670(), (void*)obj)) return 1;
        if (obj->type == 0x96b) return 1;
    }
    return 0;
}

// Compares two timeline objects by id; falls back to comparing their enemy
// chain types when the ids differ. Requires the flag bit at 0x3f00 on the
// second object.
extern "C" int func_802814E4(cf::CChainActorEne* self, void* arg2) {
    if (((u32*)arg2)[0x3f00 / 4] & 4) {
        u32 a1 = self->unk0;
        if (a1 != 0) a1 += 0x3e9c;
        EneChainObj* e1 = (EneChainObj*)func_800AD860((void*)a1);
        u32 a2 = (u32)arg2;
        if (a2 != 0) a2 += 0x3e9c;
        EneChainObj* e2 = (EneChainObj*)func_800AD860((void*)a2);
        if (e1->id == e2->id) return 1;
        int s1 = (e1->type == 0x96b) || (e1->type == 0x96c);
        if (s1) {
            int s2 = (e2->type == 0x96b) || (e2->type == 0x96c);
            if (s2) return 1;
        }
    }
    return 0;
}

// Looks up the enemy actor wired into this chain's timeline and reads the
// "rework" flag bit (bit 1 of the 0xa0 flag word). For the special enemy-
// chain types (0x96b/0x96c) it first scans the gimmick object list for the
// 0x96b timeline and uses that id; otherwise it falls back to the actor's own
// timeline object.
extern "C" int func_802815B8(cf::CChainActorEne* self) {
    u32 a0 = self->unk0;
    if (a0 != 0) a0 += 0x3e9c;
    EneChainObj* o0 = (EneChainObj*)func_800AD860((void*)a0);
    bool special = false;
    if (o0 != 0 && (o0->type == 0x96b || o0->type == 0x96c)) special = true;
    if (special) {
        GlistList* list = (GlistList*)func_800B6BC8();
        EneChainObj* found = 0;
        for (GlistNode* node = list->field_04->next; node != list->field_04; node = node->next) {
            EneChainObj* o = (EneChainObj*)func_800AD860(node->obj);
            if (o != 0 && o->type == 0x96b) {
                found = o;
                break;
            }
        }
        if (found != 0) {
            BattleActor* act = (BattleActor*)func_80193AB0(func_80193670(), found->id);
            if (act != 0) return (act->flagA0 >> 1) & 1;
        }
    }
    u32 a2 = self->unk0;
    if (a2 != 0) a2 += 0x3e9c;
    EneChainObj* o2 = (EneChainObj*)func_800AD860((void*)a2);
    if (o2 != 0) {
        BattleActor* act = (BattleActor*)func_80193AB0(func_80193670(), o2->id);
        if (act != 0) return (act->flagA0 >> 1) & 1;
    }
    return 0;
}

// Clears the "rework" (break/chain-related) bit on the enemy actor found
// through the timeline object of this chain actor, for both the special
// enemy chain case and the plain self case.
extern "C" void func_802816FC(cf::CChainActorEne* self) {
    u32 a1 = self->unk0;
    if (a1 != 0) a1 += 0x3e9c;
    EneChainObj* obj = (EneChainObj*)func_800AD860((void*)a1);

    // Special enemy-chain types (0x96b/0x96c) share the 0x96b timeline.
    int special;
    if (obj == 0) {
        special = 0;
    } else {
        int t = obj->type;
        special = 1;
        if (t != 0x96b && t != 0x96c) special = 0;
    }
    if (special != 0) {
        // Scan the gimmick-object circular list for a 0x96b timeline and
        // clear the "rework" bit (bit 1 of the 0xa0 flag) on its battle
        // actor. found aliases the list header during the scan and is only
        // nulled when the scan exhausts without a match (retail reuses one
        // register for both, so the null-store sits on the fall-through path).
        GlistList* list = (GlistList*)func_800B6BC8();
        EneChainObj* found = (EneChainObj*)list;
        GlistNode* node = list->field_04->next;
        while (node != list->field_04) {
            EneChainObj* o = (EneChainObj*)func_800AD860(node->obj);
            if (o != 0 && o->type == 0x96b) goto matched;
            node = node->next;
        }
        found = 0;
    matched:;
        BattleActor* act = 0;
        if (found != 0) {
            act = (BattleActor*)func_80193AB0(func_80193670(), found->id);
        }
        if (act != 0) act->flagA0 &= ~2;
    }

    // Always also clear the bit on the actor found through our own timeline.
    u32 a2 = self->unk0;
    if (a2 != 0) a2 += 0x3e9c;
    EneChainObj* obj2 = (EneChainObj*)func_800AD860((void*)a2);
    if (obj2 != 0) {
        BattleActor* act2 = (BattleActor*)func_80193AB0(func_80193670(), obj2->id);
        if (act2 != 0) act2->flagA0 &= ~2;
    }
}

// Classifies the actor's timeline object: 0x96b -> 0, 0x96c -> 1, missing -> 4,
// otherwise 3 minus (vtable state hook result truth).
extern "C" int func_8028183C(cf::CChainActorEne* self) {
    u32 addr = self->unk0;
    if (addr != 0) addr += 0x3e9c;
    EneChainObj* obj = (EneChainObj*)func_800AD860((void*)addr);
    if (obj != 0) {
        if (obj->type == 0x96b) return 0;
        if (obj->type == 0x96c) return 1;
        // Classify via the manual vtable hook at offset 0x40: result is 3 when
        // the hook is false, 2 when the hook is true.
        int v = reinterpret_cast<cf::CChainActorEneVtDispatch40*>(self)->target();
        // 3 when hook false, 2 when true - a small-constant ternary if-converts
        // to MWCC's branchless neg/or/srawi/addi (sign mask of v plus 3).
        return (v != 0) ? 2 : 3;
    }
    return 4;
}

// Tail-calls func_802A0AA0 with &this->mChainEffect, forwarding remaining arguments
void cf::CChainActorEne::func_802818D4() {
    func_802A0AA0(reinterpret_cast<cf::CChainEffect*>(this->mChainEffectRaw));
}

// Returns whether the enemy chain actor is valid/active
s32 cf::CChainActorEne::func_802818DC() {
    return 1;
}

extern "C" void func_802818E4(void* self, void* arg) { reinterpret_cast<CEIf*>(self)->vf0048(*(void**)arg); }

// Address into the battle manager's 0x194 field; returns whether it reached 300.
extern "C" void func_802818F8(void) {
    func_8018C8F4((u8*)getInstance__Q22cf14CBattleManagerFv() + 0x194, 0x12c);
}
