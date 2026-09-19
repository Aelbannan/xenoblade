// Auto-scaffolded catalog TU for kyoshin/cf/chain/CChainActorEne
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/cf/CBattleManagerApi.hpp"
#include "kyoshin/cf/CfMapItemManager.hpp"
#include "kyoshin/harness_catalog.hpp"
#include "kyoshin/cf/chain/CChainActorEne.hpp"
#include "kyoshin/cf/chain/CChainActorPc.hpp"
#include "kyoshin/cf/chain/CChainEffect.hpp"
#include "kyoshin/cf/object/CObjectState.hpp"

// ---- Cross-TU helper declarations ---------------------------------------
// getEffOwner / findObjectById are mangled C++ retail symbols; the rest are
// plain (unmangled) C-ABI functions and are declared with C linkage.
extern "C" void* chainResolveMemberFromSrc(u32 cls, void* src);
extern "C" void PartyGaugeSetClamped(u8* self, u32 ptg);
extern "C" void* CPartsChange_GetActorTable(void);
extern "C" void* CPartsChange_FindActorByObj(void* list, void* obj);
extern "C" void* CPartsChange_FindActorById(void* list, u32 id);
extern "C" void* getReslistB48(void);
extern "C" void* func_8016FE34(void* src);
void* getEffOwner__(void* obj);
extern void* findObjectById(int);

// Layout view of the enemy chain actor's timeline object (the object reached
// through this->unk0 (rebased +0x3E9C) and passed through getEffOwner).
struct EneChainObj {
    u8  _pad0[0x3f00];
    u32 flag;   //0x3f00 - bit2 indicates a valid/vital timeline itself
    u8  _pad1[0x24];
    u16 type;   //0x3f28
    u8  _pad2[0x696];
    u16 id;     //0x45c0
};

// Battle-actor mash returned by CPartsChange_FindActorById; only the low flag word is read.
struct BattleActor {
    u8  _pad0[0xa0];
    u16 flagA0;   //0xa0
};

// Node in the circular gimmick object list returned by getReslistB48.
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



// Operates on the CChainEffect at offset 0x74; compares r4 against effect.unk8's target
// Retail calls it by its flat name (config/us/symbols.txt), so C linkage
// keeps the call-site reloc unmangled like retail.
extern "C" void chainUnlinkOnOwnerMatch(cf::CChainEffect* effect);

// Resets chain state and clears the chain effect.
extern "C" void ChainEne_InitClear_1308(cf::CChainActorEne* self, int val) {
    CChain_setFieldAndClear(self, val);
    chainClearTwoWords(self->mChainEffectRaw);
}

extern "C" void ChainEne_Teardown_133C(cf::CChainActorEne* self) {
    self->CChain_noop_A9E8();
    chainTeardownLinkClear(self->mChainEffectRaw);
    CChainActor_ClearTargetRef(self);
}

// Validates an enemy-chain candidate: non-null source, resolves the parts
// object id, and only then runs the chain logic.
extern "C" int ChainEne_ValidateCand_1384(cf::CChainActorEne* self, int arg) {
    if (arg == 0) return 0;
    void* handle = findObjectById(arg);
    void* obj = func_8016FE34(handle);
    if (obj == 0) return 0;
    void* sub = *(void**)((u8*)obj + 4);
    int v = *(int*)static_cast<cf::CObjectState*>(sub)->CObjectState_getStateData();
    if (func_80174C98(obj, &v, 0x803) != 0) {
        return func_8027A024((void*)self, (void*)arg);
    }
    return 0;
}

// 4-arg effect bind: fills the constant 0xb9 class slot, forwards the other
// three args, and drives the 6th param from a non-zero flag sentinel.
extern "C" void ChainEne_BindEffectB9_1438(cf::CChainActorEne* self, int p1, int p2, int p3) {
    chainBindEffectLink(reinterpret_cast<cf::CChainEffect*>(self->mChainEffectRaw), p1, 0xb9, (int)self, p2, p3 != 0 ? 0x5f : 0);
}

// Tail-calls chainResolveMemberFromSrc binding the actor to the 0xba effect class.
extern "C" void* ChainEne_MakeEffectBa_1460(cf::CChainActorEne* self) {
    return chainResolveMemberFromSrc(0xba, self);
}

// True if this actor's timeline object is the one currently wired into the
// battle parts system, or if it carries the special 0x96b enemy-chain type.
extern "C" int ChainEne_IsWired_146C(const cf::CChainActorEne* self) {
    u32 addr = self->unk0;
    if (addr != 0) addr += 0x3e9c;
    EneChainObj* obj = (EneChainObj*)getEffOwner__((void*)addr);
    if (obj != 0) {
        if (obj == (EneChainObj*)CPartsChange_FindActorByObj(CPartsChange_GetActorTable(), (void*)obj)) return 1;
        if (obj->type == 0x96b) return 1;
    }
    return 0;
}

// Compares two timeline objects by id; falls back to comparing their enemy
// chain types when the ids differ. Requires the flag bit at 0x3f00 on the
// second object.
extern "C" int ChainEne_MatchOwner_14E4(cf::CChainActorEne* self, void* arg2) {
    if (((u32*)arg2)[0x3f00 / 4] & 4) {
        u32 a1 = self->unk0;
        if (a1 != 0) a1 += 0x3e9c;
        EneChainObj* e1 = (EneChainObj*)getEffOwner__((void*)a1);
        u32 a2 = (u32)arg2;
        if (a2 != 0) a2 += 0x3e9c;
        EneChainObj* e2 = (EneChainObj*)getEffOwner__((void*)a2);
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
    EneChainObj* o0 = (EneChainObj*)getEffOwner__((void*)a0);
    int special;
    if (o0 == 0) {
        special = 0;
    } else {
        int t = o0->type;
        special = 1;
        if (t != 0x96b && t != 0x96c) special = 0;
    }
    if (special != 0) {
        GlistList* list = (GlistList*)getReslistB48();
        EneChainObj* found = (EneChainObj*)list;
        GlistNode* node = list->field_04->next;
        while (node != list->field_04) {
            EneChainObj* o = (EneChainObj*)getEffOwner__(node->obj);
            if (o != 0 && o->type == 0x96b) goto foundB8;
            node = node->next;
        }
        found = 0;
    foundB8:;
        if (found != 0) {
            BattleActor* act = (BattleActor*)CPartsChange_FindActorById(CPartsChange_GetActorTable(), found->id);
            if (act != 0) return (act->flagA0 >> 1) & 1;
        }
    }
    u32 a2 = self->unk0;
    if (a2 != 0) a2 += 0x3e9c;
    EneChainObj* o2 = (EneChainObj*)getEffOwner__((void*)a2);
    if (o2 != 0) {
        BattleActor* act = (BattleActor*)CPartsChange_FindActorById(CPartsChange_GetActorTable(), o2->id);
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
    EneChainObj* obj = (EneChainObj*)getEffOwner__((void*)a1);

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
        GlistList* list = (GlistList*)getReslistB48();
        EneChainObj* found = (EneChainObj*)list;
        GlistNode* node = list->field_04->next;
        while (node != list->field_04) {
            EneChainObj* o = (EneChainObj*)getEffOwner__(node->obj);
            if (o != 0 && o->type == 0x96b) goto matched;
            node = node->next;
        }
        found = 0;
    matched:;
        BattleActor* act = 0;
        if (found != 0) {
            act = (BattleActor*)CPartsChange_FindActorById(CPartsChange_GetActorTable(), found->id);
        }
        if (act != 0) act->flagA0 &= ~2;
    }

    // Always also clear the bit on the actor found through our own timeline.
    u32 a2 = self->unk0;
    if (a2 != 0) a2 += 0x3e9c;
    EneChainObj* obj2 = (EneChainObj*)getEffOwner__((void*)a2);
    if (obj2 != 0) {
        BattleActor* act2 = (BattleActor*)CPartsChange_FindActorById(CPartsChange_GetActorTable(), obj2->id);
        if (act2 != 0) act2->flagA0 &= ~2;
    }
}

// Classifies the actor's timeline object: 0x96b -> 0, 0x96c -> 1, missing -> 4,
// otherwise 3 minus (vtable state hook result truth).
extern "C" int ChainEne_CheckChainType_183C(cf::CChainActorEne* self) {
    u32 addr = self->unk0;
    if (addr != 0) addr += 0x3e9c;
    EneChainObj* obj = (EneChainObj*)getEffOwner__((void*)addr);
    if (obj != 0) {
        if (obj->type == 0x96b) return 0;
        if (obj->type == 0x96c) return 1;
        int v = self->CChain_getZero_AA04();
        // 3 when hook false, 2 when true - a small-constant ternary if-converts
        // to MWCC's branchless neg/or/srawi/addi (sign mask of v plus 3).
        return (v != 0) ? 2 : 3;
    }
    return 4;
}

// Tail-calls chainUnlinkOnOwnerMatch with &this->mChainEffect, forwarding remaining arguments
void cf::CChainActorEne::ChainEne_ReleaseEffect_18D4() {
    chainUnlinkOnOwnerMatch(reinterpret_cast<cf::CChainEffect*>(this->mChainEffectRaw));
}

// Returns whether the enemy chain actor is valid/active
s32 cf::CChainActorEne::ChainEne_AlwaysTrue_18DC() {
    return 1;
}

extern "C" int ChainEne_GetZeroVia_18E4(void* self, void* arg) { return static_cast<cf::CChainActorEne*>(self)->CChain_getZero_A9FC(*(void**)arg); }

// Address into the battle manager's 0x194 field; returns whether it reached 300.
extern "C" void ChainEne_TouchBattle194_18F8(void) {
    PartyGaugeSetClamped((u8*)getInstance__Q22cf14CBattleManagerFv() + 0x194, 0x12c);
}
