
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
// Virtual dispatch through manually managed vtable at offset 0x70.
// Loads vtable entry 18 (offset 0x48) and tail-calls it with self and *arg.
void func_8028246C(cf::CChainActorPc* self, u32* arg) {
    ((void(*)(cf::CChainActorPc*, u32))((u32*)self->mVTable)[18])(self, *arg);
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
void func_80281958(){}
// Resets the chain effect and calls func_80279DC0 on this actor.
extern "C" void func_80281CB8(cf::CChainActorPc* self) {
    func_802A0904(&self->mChainEffect);
    func_80279DC0(self);
}
void func_80281CF0(){}
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
