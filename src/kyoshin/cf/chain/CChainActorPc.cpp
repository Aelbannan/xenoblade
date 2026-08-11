
#include "kyoshin/cf/chain/CChainActorPc.hpp"

namespace cf { class CBattleManager; class CfObjectMove; class CfGameManager; class CChain; }

// Retail symbol: cf::CBattleManager::getInstance()
cf::CBattleManager* getInstance__Q22cf14CBattleManagerFv();

// Forward decl: cf::CfGameManager::getPlayer(int)
namespace cf { class CfGameManager { public: static cf::CfObjectMove* getPlayer(int); }; }

void func_80282020(){}
int func_802A0804(int, int);

int func_80282048(int arg) {
    return func_802A0804(0xb3, arg);
}
struct ChainActorInnerData {
    u8 _pad00[0x3f28];
    unsigned short field_3f28;
};

int func_80282174(void* self) {
    cf::CChainActor* actor = static_cast<cf::CChainActor*>(self);
    ChainActorInnerData* inner = reinterpret_cast<ChainActorInnerData*>(static_cast<uintptr_t>(actor->unk0));
    int value = inner->field_3f28;
    if (value == 8) return 3;
    if (value == 9) return 8;
    if (value == 10) return 9;
    if (value == 11) return 10;
    if (value == 12) return 11;
    if (value == 13) return 12;
    return value;
}
bool func_802A0AA0(void*);
bool func_8028245C(void* self) { return func_802A0AA0((void*)((char*)self + 0x74)); }
bool func_80282464() { return true; }
// obj+0x48 holds a fn ptr; call with (self, *(arg)) (retail: lwz r12,0x70; lwz r4,0; lwz r12,0x48; mtctr; bctr)
extern "C" void func_8028246C(void* self, void* arg) {
    void* obj = *(void**)((char*)self + 0x70);
    (*(void (**)(void*, void*))((char*)obj + 0x48))(self, *(void**)arg);
}
int func_80282480(void*, void* p) {
    return ((*(int*)((char*)p + 0x3f00) >> 1) & 1);
}
void func_80277154();
void func_8028248C() {
    func_80277154();
}
void func_80282490(){}

// Resets chain state (setFieldAndClear) and clears the chain effect.
extern "C" void func_80281924(cf::CChainActorPc* self, int val) {
    CChain_setFieldAndClear(self, val);
    func_802A08F4(&self->mChainEffect);
}
// Local struct for accessing known fields within the large object at self->unk0
struct CChainBigObj {
    u8 _pad00[4];
    u32 field_04;       // 0x04 - sub-object pointer
    u8 _pad08[0x44a0];  // 0x08 .. 0x44a8
    u8 field_44a8[4];   // 0x44a8
    u8 _pad44ac[0xae];  // 0x44ac .. 0x455a
    u16 field_455a;     // 0x455a
};

// Shared external declarations
extern void func_804B1DC0(void*, int);
extern void func_80279B34(cf::CChainActorPc*);
extern u8* func_80282380(cf::CChainActorPc*);
extern int func_80174C98(u8*, int*, int);
extern void func_80279E48(cf::CChainActorPc*, int);
extern f32 lbl_eu_80668AEC;
extern f32 lbl_eu_80668AF0;
extern f32 lbl_eu_80668AE8;

// Main per-frame update for chain actor PC.
// Uses multi-exit guard pattern to match retail control flow.
void func_80281958(cf::CChainActorPc* self) {
    CChainBigObj* big = (CChainBigObj*)self->unk0;
    func_80279B34(self);
    if (!(self->unk6C & 1)) goto tail_check;
    if (((int(*)(u8*))((u8**)self->mVTable)[16])((u8*)self) == 0) goto state5_check;
    if (((int(*)(u8*))((u8**)self->mVTable)[26])((u8*)self) != 0) goto state5_check;
    ((void(*)(u8*, u8*))((u8**)self->mVTable)[27])((u8*)self, func_80282380(self));
state5_check:
    if (((int(*)(u8*))((u8**)self->mVTable)[22])((u8*)self) != 5) goto state6_check;
    if (func_80148778((u8*)big + 8, 0xf0)) {
        if (func_80148778((u8*)big + 8, 0xf0)) {
            ((void(*)(u8*, int))(*(u8***)((u8*)big + 8))[8])((u8*)big + 8, 0xf0);
        }
    }
    if (func_80148778((u8*)big + 8, 0xf1)) {
        if (func_80148778((u8*)big + 8, 0xf1)) {
            ((void(*)(u8*, int))(*(u8***)((u8*)big + 8))[8])((u8*)big + 8, 0xf1);
        }
    }
    {
        u8* obj = (u8*)big;
        u8* sub = *(u8**)(obj + 4);
        int v = *(int*)((int(*)(u8*))(*(u8***)sub)[12])(sub);
        if (func_80174C98(obj, &v, 0xb)) {
            v = *(int*)((int(*)(u8*))(*(u8***)sub)[12])(sub);
            if (func_80174C98(obj, &v, 0xb)) {
                ((void(*)(u8*, int))(*(u8***)sub)[8])(sub, 0xb);
            }
        }
    }
    {
        u8* obj = (u8*)big;
        float f86 = ((float(*)(u8*))(*(u8***)obj)[86])(obj);
        float f87 = ((float(*)(u8*))(*(u8***)obj)[87])(obj);
        if (f87 <= f86) {
            f87 = ((float(*)(u8*))(*(u8***)obj)[87])(obj);
            ((void(*)(u8*, float))(*(u8***)obj)[84])(obj, f87 - lbl_eu_80668AE8);
        }
    }
state6_check:
    if (((int(*)(u8*))((u8**)self->mVTable)[22])((u8*)self) != 6) goto store_455a;
    if (func_80148778((u8*)big + 8, 0xf8)) {
        if (func_80148778((u8*)big + 8, 0xf8)) {
            ((void(*)(u8*, int))(*(u8***)((u8*)big + 8))[8])((u8*)big + 8, 0xf8);
        }
    }
store_455a:
    {
        u8* obj = (u8*)big;
        u8* sub = *(u8**)(obj + 4);
        int v = *(int*)((int(*)(u8*))(*(u8***)sub)[12])(sub);
        if (func_80174C98(obj, &v, 6)) {
            big->field_455a = 0xec;
        } else {
            big->field_455a = 0x64;
        }
    }
tail_check:
    if ((self->unk6C & 1) || (self->unk6C & 2)) {
        float f1 = ((float(*)(u8*))(*(u8***)((u8*)big + 0x3e9c))[35])((u8*)big + 0x3e9c);
        if (f1 > lbl_eu_80668AEC) {
            func_804B1DC0(big->field_44a8, 1);
        } else {
            func_804B1DC0(big->field_44a8, 0);
        }
    }
}
// Resets the chain effect and calls func_80279DC0 on this actor.
extern "C" void func_80281CB8(cf::CChainActorPc* self) {
    func_802A0904(&self->mChainEffect);
    func_80279DC0(self);
}
// External declarations specific to func_80281CF0
extern u8* func_8009EC9C(u16);
extern int func_800A32BC(u8*);
extern u32 func_8025FB10(u8*, u32);
extern u8* func_800B6BA4();
extern float func_800D81A8(int, u8*, int);

// Processes chain actor damage/healing based on arg.
// Multi-exit guard pattern:
//   arg == bit  -> skip to end (call func_80279E48)
//   arg == 0    -> healing/damage loop over all battle actors
//   arg != 0    -> check battle-manager flag 0xeb; may trigger chain-end vfunc
void func_80281CF0(cf::CChainActorPc* self, int arg) {
    CChainBigObj* big = (CChainBigObj*)self->unk0;
    u32 bit = self->unk6C & 1;
    if ((u32)arg == bit) goto done;
    if (arg == 0) goto arg0;
    // arg != 0, arg != bit: check flag and vtable state
    {
    if (func_80148778((u8*)big + 8, 0xeb) != 0) {
        u8* obj = (u8*)big + 8;
        u8** vt = *(u8***)((u8*)big + 8);
        ((void(*)(u8*, int))vt[8])(obj, 0xeb);
    }
    int state = ((int(*)(u8*))((u8**)self->mVTable)[22])((u8*)self);
    int cond;
    if (state == 4) goto state4;
    cond = 0;
    goto check_cond;
state4:
    cond = (func_800A32BC(func_8009EC9C(4)) == 1);
check_cond:
    if (cond) {
        u8** vt = *(u8***)big;
        ((void(*)(u8*))vt[89])((u8*)big);
    }
    }
    goto done;
arg0:
    // arg == 0: healing/damage loop over all battle actors
    big->field_455a = 100;
    func_804B1DC0(big->field_44a8, 1);
    u8* unk0obj = (u8*)big;
    float f30;
    if (((int(*)(u8*))(*(u8***)unk0obj)[164])(unk0obj) != 0) {
        u8* param = (u8*)((int(*)(u8*))(*(u8***)unk0obj)[164])(unk0obj);
        f30 = lbl_eu_80668AF0 * (float)(s16)func_8025FB10(param, 0x44);
    } else {
        f30 = lbl_eu_80668AEC;
    }
    if (lbl_eu_80668AEC < f30) {
        if (((int(*)(u8*))(*(u8***)unk0obj)[175])(unk0obj) == 0) {
            u8* list = func_800B6BA4();
            u8* sentinel = *(u8**)(list + 4);
            u8* node = *(u8**)sentinel;
            while (node != sentinel) {
                u8* actor = *(u8**)(node + 8);
                if (actor != 0) {
                    actor = actor - 0x3e9c;
                }
                if (((int(*)(u8*))(*(u8***)actor)[175])(actor) == 0) {
                    getInstance__Q22cf14CBattleManagerFv();
                    float f31 = func_800D81A8(0, actor, 0);
                    float dmg = f30 * ((float(*)(u8*))(*(u8***)actor)[75])(actor);
                    ((void(*)(u8*, float))(*(u8***)actor)[71])(actor, dmg * f31);
                }
                node = *(u8**)node;
            }
        }
    }
done:
    func_80279E48(self, arg);
}
// Retail symbol: func_80279F6C

// Sets a chain actor flag and optionally resets chain field via func_804B1DC0
// when arg is 0 and the current bit state differs.
extern "C" void func_80281F38(cf::CChainActorPc* self, int arg) {
    // Use u32 compare to force cmpl (unsigned) to match retail
    u32 bit = (self->unk6C >> 1) & 1;
    if ((u32)arg != bit && arg == 0) {
        func_804B1DC0((void*)(self->unk0 + 0x44a8), 1);
    }
    func_80279F6C(self, arg);
}
// Checks preconditions before calling func_8027A024 to execute chain logic.
// Returns 0 if the actor is inactive (vtable check) or if a battle-manager
// flag 0xf8 is set on the unk0 object; otherwise delegates to func_8027A024.
extern "C" int func_80281FA0(cf::CChainActorPc* self, void* arg) {
    // Call vtable entry 29: check some active/in-battle condition
    int (*vfunc)(void*) = ((int(**)(void*))self->mVTable)[29];
    if (vfunc(self) != 0) return 0;
    // Check battle-manager flag 0xf8 on this->unk0 + 8
    if (func_80148778((void*)(self->unk0 + 8), 0xf8) != 0) return 0;
    return func_8027A024(self, arg);
}
// Checks if this actor is the player's current actor.
// Compares getPlayer(0) against unk0 + 0x3e9c (or 0 if unk0 is null).
extern "C" int func_80282054(cf::CChainActor* self) {
    u32 addr = self->unk0;
    if (addr != 0) {
        addr += 0x3e9c;
    }
    return (void*)addr == cf::CfGameManager::getPlayer(0);
}
// Returns true if CBattleManager+0x194 >= 300.
extern "C" int func_8028209C() {
    return *(int*)((u8*)getInstance__Q22cf14CBattleManagerFv() + 0x194) >= 0x12c;
}
void func_802820D4(){}
// Compares the halfword at this->unk0 + 0x3f28 against the result of
// func_8009ECB0() (a global config struct). Returns 0, 1, or 2 if the
// value matches fields at offsets 4, 8, or 12 respectively; returns 3
// otherwise.
extern "C" int func_80282100(cf::CChainActorPc* self) {
    int* config = func_8009ECB0();
    u16 value = *(u16*)(self->unk0 + 0x3f28);
    if (value == config[1]) return 0;
    if (value == config[2]) return 1;
    if (value == config[3]) return 2;
    return 3;
}
void func_802821E0(){}
void func_802822F8(){}
void func_80282380(){}
