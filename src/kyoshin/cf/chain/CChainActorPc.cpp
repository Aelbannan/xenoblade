
#include "kyoshin/cf/chain/CChainActorPc.hpp"
#include "kyoshin/cf/CfMapItemManager.hpp"
#include "kyoshin/cf/CBattleManagerApi.hpp"

namespace cf { class CBattleManager; class CfObjectMove; class CfGameManager; class CChain; }

// Forward decl: cf::CfGameManager::getPlayer(int)
namespace cf { class CfGameManager { public: static cf::CfObjectMove* getPlayer(int); }; }

// retail: func_802A0950(self+0x74, r4, 0xAA, r5, (arg3!=0) ? 0x5F : 0, self)
extern "C" void func_80282020(void* self, void* a, void* b, int c, int d) {
    // retail arg order: r3=effect, r4=a, r5=0xAA, r6=self, r7=b, r8=cond
    func_802A0950((cf::CChainEffect*)((char*)self + 0x74), (int)a, 0xAA, (int)self,
                  (int)b, (c != 0) ? 0x5F : 0);
}
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
// Real class tree: CChainActorPc is novtable, vptr at +0x70 (0x70 pad), ctor writes lbl_eu_805384E0 (JP __vt__Q22cf13CChainActorPc).
// Slot +0x48 (index 18 overall, vt offset 0x48) is CChain_getZero_A9FC: int(void* p) - owned by cf::CChainActorPc (leaf override of base A9FC).
// Thunk at +0x44 (func_8028246C) forwards via this->CChain_getZero_A9FC(*(void**)arg).
namespace cf {
CChainActorPc::CChainActorPc() { vtbl() = &lbl_eu_805384E0; }
CChainActorPc::~CChainActorPc() {}
void CChainActorPc::CChain_setFieldAndClear(int) {}
void CChainActorPc::func_80279B34() {}
void CChainActorPc::func_80279DC0() {}
void CChainActorPc::func_80279E48(int) {}
void CChainActorPc::func_80279F6C(int) {}
int CChainActorPc::func_8027A024(int) { return 0; }
int CChainActorPc::func_8027A338(int) { return 0; }
int CChainActorPc::func_8027A58C() { return 0; }
int CChainActorPc::func_8027A8C8() { return 0; }
void CChainActorPc::CChain_noop_AA0C(int, cf::CChainActor*, int) {}
void CChainActorPc::CChain_noop_795D0() {}
void CChainActorPc::CChain_noop_78E00(int, int) {}
int CChainActorPc::CChain_getZero_78E04() { return 0; }
int CChainActorPc::CChain_getZero_AA04() { return 0; }
int CChainActorPc::CChain_getZero_A9F4(void* arg) { return this->CChain_getZero_A9FC(*(void**)arg); }
int CChainActorPc::CChain_getZero_A9FC(void* p) { return ((*(int*)((char*)p + 0x3f00) >> 1) & 1); }
int CChainActorPc::CChain_getZero_A9EC() { return 0; }
void CChainActorPc::CChain_noop_A9E8() {}
int CChainActorPc::CChain_getZero_A9E0() { return 0; }
int CChainActorPc::CChain_getChainCount() { return 0; }
int CChainActorPc::CChain_getZero_A584() { return 0; }
int CChainActorPc::CChain_getZero_A9D8() { return 0; }
void CChainActorPc::CChain_noop_79768(int) {}
int CChainActorPc::func_80278F70() { return 0; }
void CChainActorPc::func_80278F5C(int) {}
int CChainActorPc::func_8027A324() { return 0; }
int CChainActorPc::CChain_getZero_A9D0() { return 0; }
}
extern "C" void func_8028246C(cf::CChainActorPc* self, void* arg) {
    self->CChain_getZero_A9FC(*(void**)arg);
}
int func_80282480(void*, void* p) {
    return ((*(int*)((char*)p + 0x3f00) >> 1) & 1);
}
void func_80277154();
void func_8028248C() {
    func_80277154();
}
// retail: func_80276CAC(a, *(self), b ? *b : 0)
extern "C" void func_80276CAC(void* a, void* b, void* c);
extern "C" void func_80282490(void* self, void* a, void* b) {
    func_80276CAC(a, *(void**)self, b ? *(void**)b : 0);
}

// Resets chain state (setFieldAndClear) and clears the chain effect.
extern "C" void func_80281924(cf::CChainActorPc* self, int val) {
    CChain_setFieldAndClear(self, val);
    func_802A08F4((cf::CChainEffect*)((char*)self + 0x74));
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
extern void ColiSetMoveEnableFlag(void*, int);
extern void func_80279B34(cf::CChainActorPc*);
extern u8* func_80282380(cf::CChainActorPc*);
extern void func_80279E48(cf::CChainActorPc*, int);
__declspec(section ".sdata2") __attribute__((aligned(8))) const float lbl_eu_80668AE8 = 1.0f;
__declspec(section ".sdata2") const float lbl_eu_80668AEC = 0.0f;
__declspec(section ".sdata2") const float lbl_eu_80668AF0 = 0.01f;
__declspec(section ".sdata2") __attribute__((aligned(8))) const double lbl_eu_80668AF8 = 4503601774854144.0;

// Main per-frame update for chain actor PC.
// Uses multi-exit guard pattern to match retail control flow.
void func_80281958(cf::CChainActorPc* self) {
    CChainBigObj* big = (CChainBigObj*)self->unk0;
    func_80279B34(self);
    if (!(self->unk6C & 1)) goto tail_check;
    if (((int(*)(u8*))((u8**)self->mVTable())[16])((u8*)self) == 0) goto state5_check;
    if (((int(*)(u8*))((u8**)self->mVTable())[26])((u8*)self) != 0) goto state5_check;
    ((void(*)(u8*, u8*))((u8**)self->mVTable())[27])((u8*)self, func_80282380(self));
state5_check:
    if (((int(*)(u8*))((u8**)self->mVTable())[22])((u8*)self) != 5) goto state6_check;
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
    if (((int(*)(u8*))((u8**)self->mVTable())[22])((u8*)self) != 6) goto store_455a;
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
            ColiSetMoveEnableFlag(big->field_44a8, 1);
        } else {
            ColiSetMoveEnableFlag(big->field_44a8, 0);
        }
    }
}
// Resets the chain effect and calls func_80279DC0 on this actor.
extern "C" void func_80281CB8(cf::CChainActorPc* self) {
    func_802A0904((cf::CChainEffect*)((char*)self + 0x74));
    func_80279DC0(self);
}
// External declarations specific to func_80281CF0
extern void* func_8009EC9C(u32 index);
extern int CtrlObjectParam_GetCurrentRowKey(void*);
extern u32 func_8025FB10(u8*, u32);
extern u8* getListB28();
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
    int state = ((int(*)(u8*))((u8**)self->mVTable())[22])((u8*)self);
    int cond;
    if (state == 4) goto state4;
    cond = 0;
    goto check_cond;
state4:
    cond = (CtrlObjectParam_GetCurrentRowKey(func_8009EC9C(4)) == 1);
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
    ColiSetMoveEnableFlag(big->field_44a8, 1);
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
            u8* list = getListB28();
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

// Sets a chain actor flag and optionally resets chain field via ColiSetMoveEnableFlag
// when arg is 0 and the current bit state differs.
extern "C" void func_80281F38(cf::CChainActorPc* self, int arg) {
    // Use u32 compare to force cmpl (unsigned) to match retail
    u32 bit = (self->unk6C >> 1) & 1;
    if ((u32)arg != bit && arg == 0) {
        ColiSetMoveEnableFlag((void*)(self->unk0 + 0x44a8), 1);
    }
    func_80279F6C(self, arg);
}
// Data pad so the C++ vptr lands at +0x70 (retail lwz r12, 0x70(r3)).
struct CChainActorPcV70Data {
    u8 pad[0x70];
};

// Pure-virtual view of slot 0x74. MWCC prefix + v27 = retail +0x74.
// Never constructed (CHelpManager / CQuestWindow scheme).
struct CChainActorPcV29View : CChainActorPcV70Data {
    virtual int v00() = 0;
    virtual int v01() = 0;
    virtual int v02() = 0;
    virtual int v03() = 0;
    virtual int v04() = 0;
    virtual int v05() = 0;
    virtual int v06() = 0;
    virtual int v07() = 0;
    virtual int v08() = 0;
    virtual int v09() = 0;
    virtual int v10() = 0;
    virtual int v11() = 0;
    virtual int v12() = 0;
    virtual int v13() = 0;
    virtual int v14() = 0;
    virtual int v15() = 0;
    virtual int v16() = 0;
    virtual int v17() = 0;
    virtual int v18() = 0;
    virtual int v19() = 0;
    virtual int v20() = 0;
    virtual int v21() = 0;
    virtual int v22() = 0;
    virtual int v23() = 0;
    virtual int v24() = 0;
    virtual int v25() = 0;
    virtual int v26() = 0;
    virtual int v27() = 0;  // MWCC prefix+27*4 = retail +0x74
};

// Checks preconditions before calling func_8027A024 to execute chain logic.
// Returns 0 if the actor is inactive (vtable check) or if a battle-manager
// flag 0xf8 is set on the unk0 object; otherwise delegates to func_8027A024.
extern "C" int func_80281FA0(cf::CChainActorPc* self, void* arg) {
    if (static_cast<CChainActorPcV29View*>(static_cast<void*>(self))->v27() != 0) {
        return 0;
    }
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
extern "C" void func_802820D4() {
    extern void func_8018C8F4(void* self, int a);
    func_8018C8F4((u8*)getInstance__Q22cf14CBattleManagerFv() + 0x194, 0);
}
// Compares the halfword at this->unk0 + 0x3f28 against the result of
// CtrlObjectParam_GetSlotTableBase() (a global config struct). Returns 0, 1, or 2 if the
// value matches fields at offsets 4, 8, or 12 respectively; returns 3
// otherwise.
extern "C" int func_80282100(cf::CChainActorPc* self) {
    int* config = CtrlObjectParam_GetSlotTableBase();
    u16 value = *(u16*)(self->unk0 + 0x3f28);
    if (value == config[1]) return 0;
    if (value == config[2]) return 1;
    if (value == config[3]) return 2;
    return 3;
}
void func_802821E0(){}
void func_802822F8(){}
void func_80282380(){}
