
#include "kyoshin/cf/chain/CChainActorPc.hpp"

namespace cf { class CBattleManager; class CfObjectMove; class CfGameManager; class CChain; }

// Retail symbol: cf::CBattleManager::getInstance()
cf::CBattleManager* getInstance__Q22cf14CBattleManagerFv();

// Forward: free function operating on the CChain/CChainActor memory layout
extern "C" void CChain_setFieldAndClear(void*, int);

// Forward decl: cf::CfGameManager::getPlayer(int)
namespace cf { class CfGameManager { public: static cf::CfObjectMove* getPlayer(int); }; }

void func_80282020(){}
int func_802A0804(int, int);
extern "C" void func_802A08F4(void*);
extern "C" void func_802A0904(void*);
extern "C" void func_80279DC0(void*);
extern "C" int func_80148778(void*, int);
extern "C" int func_8027A024(void*, void*);
extern "C" int* func_8009ECB0();

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
void func_8028246C(){}
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
void func_80281958(){}
// Resets the chain effect and calls func_80279DC0 on this actor.
extern "C" void func_80281CB8(cf::CChainActorPc* self) {
    func_802A0904(&self->mChainEffect);
    func_80279DC0(self);
}
// External declarations needed for func_80281CF0
extern "C" void func_804B1DC0(void*, int);
extern "C" void func_8009EC9C(int);
extern "C" int func_800A32BC();
extern "C" int func_8025FB10(int, int);
extern "C" void* func_800B6BA4();
extern "C" float func_800D81A8(int, void*, int);
extern "C" void func_80279E48(void*, int);
extern f32 lbl_eu_80668AEC;
extern f32 lbl_eu_80668AF0;

// Processes chain actor damage/healing based on arg.
// arg == 0: iterates battle actor list applying scaled damage to each valid target
// arg != 0: checks battle-manager flag 0xeb and vtable state; may trigger chain-end
extern "C" void func_80281CF0(cf::CChainActorPc* self, int arg) {
    u32 bit = self->unk6C & 1;
    if ((u32)arg != bit) {
        if (arg != 0) {
            // arg != 0, arg != bit: check flag and vtable state
            if (func_80148778((void*)(self->unk0 + 8), 0xeb) != 0) {
                void* obj = (void*)(self->unk0 + 8);
                void** vt = *(void***)(self->unk0 + 8);
                ((void(*)(void*, int))vt[8])(obj, 0xeb);
            }
            int state = ((int(*)(void*))((void**)self->mVTable)[22])(self);
            int cond = 0;
            if (state == 4) {
                func_8009EC9C(4);
                cond = (func_800A32BC() == 1);
            }
            if (cond) {
                void** vt = *(void***)(self->unk0);
                ((void(*)(void*))vt[89])((void*)self->unk0);
            }
        } else {
            // arg == 0: healing/damage loop over all battle actors
            *(u16*)(self->unk0 + 0x455a) = 100;
            func_804B1DC0((void*)(self->unk0 + 0x44a8), 1);
            void* unk0obj = (void*)self->unk0;
            void** unk0vt = *(void***)unk0obj;
            float f30;
            if (((int(*)(void*))unk0vt[164])(unk0obj) != 0) {
                int val = ((int(*)(void*))unk0vt[164])(unk0obj);
                f30 = lbl_eu_80668AF0 * (float)(s16)func_8025FB10(val, 0x44);
            } else {
                f30 = lbl_eu_80668AEC;
            }
            if (lbl_eu_80668AEC < f30) {
                if (((int(*)(void*))unk0vt[175])(unk0obj) == 0) {
                    void* list = func_800B6BA4();
                    void* sentinel = *(void**)((u8*)list + 4);
                    void* node = *(void**)sentinel;
                    while (node != sentinel) {
                        void* actor = *(void**)((u8*)node + 8);
                        if (actor != 0) {
                            actor = (void*)((u8*)actor - 0x3e9c);
                        }
                        void** actorVt = *(void***)actor;
                        if (((int(*)(void*))actorVt[175])(actor) == 0) {
                            getInstance__Q22cf14CBattleManagerFv();
                            float f31 = func_800D81A8(0, actor, 0);
                            float dmg = f30 * ((float(*)(void*))actorVt[75])(actor);
                            ((void(*)(void*, float))actorVt[71])(actor, dmg * f31);
                        }
                        node = *(void**)node;
                    }
                }
            }
        }
    }
    func_80279E48(self, arg);
}
// Retail symbol: func_804B1DC0
extern "C" void func_804B1DC0(void*, int);
// Retail symbol: func_80279F6C
extern "C" void func_80279F6C(void*, int);

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
