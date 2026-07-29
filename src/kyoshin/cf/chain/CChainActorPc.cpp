
#include "kyoshin/cf/chain/CChainActorPc.hpp"

namespace cf { class CBattleManager; class CfObjectMove; class CfGameManager; class CChain; class CfObjectSelectorObj; }

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

// Forward declarations for Target 1 (func_802822F8)
cf::CfObjectSelectorObj* func_800FE68C();
void func_800FE6A4(cf::CfObjectSelectorObj*, int, int, int);

// Forward declarations for Target 2 (func_80282380)
void func_80043D90(void*);
void* func_80043F18(void*);
void __dt__80043E88(void*, int);
void func_800F4A98(void*, int, int);
void func_800F6ED0(void*, void*);
void* func_800F6E08(void*);
void* func_800B708C(int);
void* func_800AD860(void*);

// Forward declaration for Target 3 (func_802821E0)
int func_80174C98(void*, void*, int);

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
// Checks multiple conditions to determine if the player can initiate a
// chain attack from this actor. Returns 1 if chain-ready, 0 otherwise.
int func_802821E0(cf::CChainActorPc* self) {
    // vfunc22 must return exactly 5
    if (((int(*)(cf::CChainActorPc*))((void**)self->mVTable)[22])(self) == 5) {
        // Check battle-manager flags 0xf0 and 0xf1
        if (func_80148778((void*)(self->unk0 + 8), 0xf0) != 0) return 1;
        if (func_80148778((void*)(self->unk0 + 8), 0xf1) != 0) return 1;

        // Call vfunc12 on sub-object at unk0+4, pass its field_0x0 to func_80174C98
        u32 obj = self->unk0;
        u32 subObj = *(u32*)(obj + 4);
        u32 val = *(u32*)((void*(*)(u32))(*(void***)subObj)[12])(subObj);
        if (func_80174C98((void*)obj, &val, 0xb) != 0) return 1;

        // Float comparison: vfunc87 <= vfunc86 on the object at this->unk0
        obj = self->unk0;
        float f86 = ((float(*)(u32))(*(void***)obj)[86])(obj);
        float f87 = ((float(*)(u32))(*(void***)obj)[87])(obj);
        if (!(f87 > f86)) return 1;
    }
    return 0;
}

// Calls a vfunc on the CfObjectMove subobject (this->unk0 + 0x3e9c), checks
// whether the actor is active via CChainActor vtable, then if both conditions
// pass, dispatches to the object selector with type 0x80000003.
void func_802822F8(cf::CChainActorPc* self, int arg) {
    u32 moveBase = self->unk0 + 0x3e9c;
    void** moveVtbl = *(void***)moveBase;
    void (*vfunc)(u32) = (void(*)(u32))moveVtbl[20];
    vfunc(moveBase);

    // Check whether the actor is active (vfunc16 returns non-zero)
    int (*checkFunc)(cf::CChainActorPc*) = (int(*)(cf::CChainActorPc*))((void**)self->mVTable)[16];
    if (checkFunc(self) != 0 && arg != 0) {
        cf::CfObjectSelectorObj* sel = func_800FE68C();
        func_800FE6A4(sel, 0x80000003, 0, arg);
    }
}

// Enumerates objects of type 0x80000002 via a stack-allocated holder, calls
// a vfunc at offset 0xac on the CfObjectMove subobject, then validates the
// result through func_800F6ED0/func_800F6E08 and a null check on the returned
// object. Returns the valid object or 0.
void* func_80282380(cf::CChainActorPc* self) {
    struct Holder {
        void* list;
        u32 handle;
    } holder;

    func_80043D90(&holder);
    void* list = func_80043F18(&holder);
    func_800F4A98(list, 0x80000002, 0);

    // Call vfunc at vtable offset 0xac on this->unk0 + 0x3e9c
    u32 moveBase = self->unk0 + 0x3e9c;
    void** moveVtbl = *(void***)moveBase;
    void* (*vfunc)(u32) = (void* (*)(u32))moveVtbl[0xac / 4];
    void* result = vfunc(moveBase);

    list = func_80043F18(&holder);
    func_800F6ED0(list, result);

    list = func_80043F18(&holder);
    if (*(int*)((u8*)list + 0x620) == 0) {
        __dt__80043E88(&holder, -1);
        return 0;
    }

    list = func_80043F18(&holder);
    result = func_800F6E08(list);
    void* checked = func_800B708C((int)(uintptr_t)result);
    if (func_800AD860(checked) == 0) {
        __dt__80043E88(&holder, -1);
        return 0;
    }

    __dt__80043E88(&holder, -1);
    return result;
}
