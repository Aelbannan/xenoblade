// High-level reconstruction of cf::CfGimmickWarp.
// Retail disassembly is used only as a reference for the data layout and
// control flow; all implementation below is ordinary C/C++.

#include "kyoshin/cf/CBattleManagerApi.hpp"
#include "kyoshin/cf/CfMapItemManager.hpp"
#include "kyoshin/harness_catalog.hpp"
#include "monolib/scn/CScnTimeApi.hpp"
#include "kyoshin/cf/CfGimmickWarp.hpp"
#include "kyoshin/cf/CfGameManager.hpp"
#include "kyoshin/cf/object/CObjectState.hpp"
#include "kyoshin/cf/object/CfObject.hpp"
#include "kyoshin/cf/object/CfObjectMove.hpp"
// CObjectState is the owner for *(player+4) slot 0x30 (defined inline to avoid
// header overload conflicts; matches CfObjectPc.hpp's cf::CObjectState).

struct WarpObject;

/* The game stores a twelve-byte MWCC pointer-to-member-function table. */
typedef void (cf::CfGimmickWarp::*WarpStateProc)();
typedef int (*WarpJumpProc)(WarpVec3*, const WarpVec3*, WarpVec3*);

struct WarpColumns {
    char* c00;
    char* c04;
    char* c08;
    char* c0c;
    char* c10;
    char* c14;
    char* c18;
    char* c1c;
    char* c20;
    char* c24;
    char* c28;
    char* c2c;
    char* c30;
    char* c34;
    char* c38;
    char* c3c;
    char* c40;
    char* c44;
};

struct WarpObject {
    u8 pad00[0x8c];
    u8 field_8c[0x90 - 0x8c]; /* sub-record passed to movePcResetAndSetPos */
    u8 pad90[0xB0 - 0x90];
    cf::CfGimmickWarp* owner;
};

typedef cf::CfGimmickWarp WarpData;

// Removed fake iface - now using cf::CObjectState directly.

// Player base is the retail battle-actor object at getPlayer(i)-0x3E9C; its
// vtable prefix matches cf::CfObject so call sites use that real type
// directly (no local layout pad).

// Tiny owner-named views for foreign sub-objects.
// cf::CObjectState is the owner for *(player+4) slot 0x30 (see CfObjectPc.hpp / CHelp_ArtsAttack).
// CfObject head slots (+0xA8/+0xC8/+0x168) are spelled with the real header
// names (CfObject_syncMoveTarget / CfObject_setMoveYaw /
// CfObject_syncModelRate), so no local helper is needed.

// Removed fake iface - now using real classes.

extern "C" {
extern WarpColumns lbl_eu_805357E8;
extern char lbl_eu_8050870C[];
extern u8 lbl_eu_805359B0[];
extern WarpStateProc lbl_eu_80535938[];
extern WarpJumpProc jumptable_eu_80535830[];
extern WarpVec3 lbl_eu_805765A0;
extern void* lbl_eu_80664134;
extern u32 lbl_eu_806646BC;
extern f32 lbl_eu_806683C8;
extern f32 lbl_eu_806683CC;
extern f32 lbl_eu_806683D0;
extern f32 lbl_eu_806683D4;
extern f32 lbl_eu_806683D8;
extern f32 lbl_eu_806683DC;
extern f32 lbl_eu_806683E0;
extern f32 lbl_eu_806683E4;
extern f32 lbl_eu_806683E8;
extern f64 lbl_eu_806683F0;
extern f64 lbl_eu_806683F8;
extern const f32 lbl_eu_8066A210;
extern f32 lbl_eu_8066AF20;

void __ct__cf_CfGimmick(WarpData*);
void CfGimmick_LoadBdatAreaPosIndexed(WarpData*, WarpVec3*, void*, u32*, int);
void CfGimmick_LoadBdatAreaRotationIndexed(WarpData*, WarpVec3*, void*, u32*, int);
void CfGimmick_LoadBdatAreaExtentsIndexed(WarpData*, WarpVec3*, void*, u32*, int);
void func_802089BC(WarpVec3*, const WarpVec3*, const WarpVec3*);
void CfGimmick_ClearManagerBinding(WarpData*);
void CfGimmick_UnregisterSpawnedObject(WarpObject*);
void func_8020A6B0(WarpObject**, WarpVec3*, u16, f32, int, int);
int CfGimmick_IsMessageSystemBusy(WarpData*);
int CfGimmick_CheckPartyIdLoaded(WarpData*, WarpObject*);
void func_8020A484(u16);
int CfGimmick_CheckStateFlag2CC8(int);
int CfGimmick_CheckTriggerGated(int, WarpVec3*, WarpVec3*, WarpVec3*, WarpObject*);
u32 getResourceFromTable__Q22cf13CfGameManagerFv(u32 resourceId);
void CfGimmick_SetGlobalFlagC0002();
void CfGimmick_SetGlobalFlagD0000();
void CfGimmick_SetGlobalFlag40000();
void CfGimmick_ApplyPartyMoveSpeed(f32);
void CfGimmick_ApplyPartyMoveSpeedGated(u32 flags);
void CfSoundMan_ApplySlotStop(int, u16, int);
extern "C" void* createBattleActor__Q22cf13CfGameManagerFv(u32 value, u32 unused);
void cfCam_syncFollowD();
void setChildB59__(WarpObject*, int);
extern "C" int ScnRes_VertRayForward_E398(void* vec, int a, int b, int c, f32 d, f32 e);
extern "C" void func_804BE4B4(void* out, int a);
u16 CfGimmick_PlaySoundAtPos(u16, const WarpVec3*);
void func_8008566C__Q22cf13CfGameManagerFv(int, WarpVec4*, int);
void func_80198710(WarpVec3*, const WarpVec3*, f32, int, int, f32, f32);
int CPartsChange_ProcessPartyInfo(WarpVec3*, WarpVec3*);
void movePcResetAndSetPos(u8*, const WarpVec3*);
// getCameraDataBlock__Q22cf13CfGameManagerFv: declared (UnkClass_800821F8*
// return) by the shared cf headers; the TU-local WarpObject* decl clashed
// with it (only use casts to CObjectState* anyway).
void __dt__Q22cf9CfGimmickFv(WarpData*, int);
void __dl__FPv(void*);
}

static inline cf::CfObject* playerFromRaw(void* raw) {
    if (raw == 0) return 0;
    return reinterpret_cast<cf::CfObject*>(reinterpret_cast<u8*>(raw) - 0x3E9C);
}

static void clearObject(WarpObject*& object) {
    if (object != 0) {
        setChildB59__(object, 1);
        object->owner = 0;
        object = 0;
    }
}

static void clearWarpObjects(WarpData* self) {
    clearObject(self->object7c);
    clearObject(self->object100);
    clearObject(self->object104);
}

static void updateWarp(WarpData* self) {
    (self->*lbl_eu_80535938[self->state])();
}

/* Overlay of the CBattleManager fields read by the warp request gating. */
struct BattleMgrWarpState {
    u8 pad00[0x1aa];
    u8 phase;                        /* 0x1aa */
    u8 pad1ab[0x20c8 - 0x1ab];
    s16 chainValue;                  /* 0x20c8 */
};

extern "C" void __ct__cf_CfGimmickWarp(WarpData* self, u16 rowId) {
    __ct__cf_CfGimmick(self);
    *(void**)self = (void*)lbl_eu_805359B0;
    self->typeId = 4;

    void* manager = Bdat_GetTable_AA34();
    u32 tableValue = (u32)lbl_eu_80664134;
    self->rowId = rowId;

    u32 valueE4 = getBdatStringColumnValue((void*)tableValue, lbl_eu_805357E8.c2c, rowId);
    self->unkE4 = (u16)valueE4;
    u32 valueE7 = getBdatStringColumnValue((void*)tableValue, lbl_eu_8050870C, rowId);
    self->flagE7 = (u8)valueE7;
    u32 valueE8 = getBdatStringColumnValue((void*)tableValue, lbl_eu_8050870C + 7, rowId);
    self->unkE8 = (u16)valueE8;
    u32 value6A = getBdatStringColumnValue((void*)tableValue, lbl_eu_805357E8.c30, rowId);

    CfGimmick_LoadBdatAreaPosIndexed(self, &self->position, manager, &tableValue, 0);
    CfGimmick_LoadBdatAreaRotationIndexed(self, &self->rotation, manager, &tableValue, 0);
    CfGimmick_LoadBdatAreaExtentsIndexed(self, &self->scale, manager, &tableValue, 0);
    CfGimmick_LoadBdatAreaPosIndexed(self, &self->destination, manager, &tableValue, 1);
    CfGimmick_LoadBdatAreaRotationIndexed(self, &self->destination2, manager, &tableValue, 1);
    CfGimmick_LoadBdatAreaExtentsIndexed(self, &self->destination3, manager, &tableValue, 1);

    char* changingColumn = lbl_eu_805357E8.c28;
    *changingColumn = 0x41;
    u32 value66 = getBdatStringColumnValue((void*)tableValue, changingColumn, rowId);
    self->configFlags = (u16)value66;
    *changingColumn = *changingColumn + 1;
    u32 valueE6 = getBdatStringColumnValue((void*)tableValue, changingColumn, rowId);
    self->flagE6 = (u8)valueE6;
    u32 valueEA = getBdatStringColumnValue((void*)tableValue, lbl_eu_8050870C + 0x0e, rowId);
    self->unkEA = (s16)valueEA;
    u32 valueEC = getBdatStringColumnValue((void*)tableValue, lbl_eu_8050870C + 0x15, rowId);
    self->unkEC = (s16)valueEC;
    u32 valueEE = getBdatStringColumnValue((void*)tableValue, lbl_eu_805357E8.c3c, rowId);
    self->unkEE = (u16)valueEE;
    u32 valueF0 = getBdatStringColumnValue((void*)tableValue, lbl_eu_805357E8.c40, rowId);
    self->unkF0 = (u16)valueF0;
    u32 value6c = getBdatStringColumnValue((void*)tableValue, lbl_eu_805357E8.c34, rowId);
    self->minValue = (u16)value6c;
    u32 value6e = getBdatStringColumnValue((void*)tableValue, lbl_eu_805357E8.c38, rowId);
    self->maxValue = (u16)value6e;
    self->resourceId = (u16)value6A;

    if ((self->configFlags & 8) == 0) {
        self->flags |= 0x40;
    }
    if ((self->flagE6 & 8) == 0) {
        self->flags |= 0x80;
    }

    self->unk_20();

    self->state = 0;
    self->phase = 0;
    self->timer = lbl_eu_806683C8;
    self->object108 = 0;
    self->object104 = 0;
    self->object100 = 0;
    self->flags |= 0x100;
}

extern "C" void* __dt__Q22cf13CfGimmickWarpFv(WarpData* self, int deleteFlag) {
    if (self != 0) {
        *(void**)self = (void*)lbl_eu_805359B0;
        CfGimmick_ClearManagerBinding(self);
        CfGimmick_UnregisterSpawnedObject((WarpObject*)((u8*)self + 0x7c));
        CfGimmick_UnregisterSpawnedObject((WarpObject*)((u8*)self + 0x108));
        __dt__Q22cf9CfGimmickFv(self, 0);
        if (deleteFlag > 0) {
            __dl__FPv(self);
        }
    }
    return self;
}

extern "C" void CfGimmickWarp_UpdateState(WarpData* self) {
    (self->*lbl_eu_80535938[self->state])();
    if ((self->flags & 0x10) != 0) {
        if ((self->configFlags & 1) != 0) {
            func_8020A6B0(&self->object7cState, &self->position,
                          self->resourceId, lbl_eu_806683CC, 0, 0);
        }
        if ((self->flagE6 & 1) != 0) {
            func_8020A6B0(&self->object108, &self->destination,
                          self->resourceId, lbl_eu_806683CC, 0, 0);
        }
        self->flags &= ~0x10u;
    } else {
        CfGimmick_UnregisterSpawnedObject((WarpObject*)(&self->object7cState));
        CfGimmick_UnregisterSpawnedObject((WarpObject*)(&self->object108));
    }
}

extern "C" void CfGimmickWarp_ClearState(WarpData* self) {
    u32 flags = self->flags;
    self->object104 = 0;
    self->object100 = 0;
    self->object7c = 0;
    self->flags = flags & ~1u;
}

extern "C" void CfGimmickWarp_RefreshMatrices(WarpData* self) {
    func_802089BC(&self->scale, &self->position, &self->rotation);
    func_802089BC(&self->destination3, &self->destination, &self->destination2);
}

extern "C" void CfGimmickWarp_TryFire(WarpData* self) {
    if ((lbl_eu_806646BC & 2) == 0) {
        return;
    }

    bool objectReady = (self->flags >> 8) & 1;
    self->flags = (*(volatile u32*)&self->flags) & ~0x100u;
    if ((self->flags & 0x20) != 0) {
        if (CfGimmick_IsMessageSystemBusy(self) != 0) {
            return;
        }
        self->flags &= ~0x20u;
    } else {
        int allReady = 0;
        if ((self->configFlags & 1) != 0) {
            allReady = (allReady | CfGimmick_CheckPartyIdLoaded(self, self->object7cState)) != 0;
        }
        if ((self->flagE6 & 1) != 0) {
            allReady = (allReady | CfGimmick_CheckPartyIdLoaded(self, self->object108)) != 0;
        }
        if (allReady != 0) {
            func_8020A484(self->resourceId);
            self->flags |= 0x20;
            return;
        }
    }

    self->flags |= 0x10;
    if (self->maxValue != 0) {
        u32 current = cf::CfGameManager::getQueuedFileEventCount();
        if (self->minValue > current || current > self->maxValue) {
            return;
        }
    }
    if (self->unkEE != 0 &&
        getResourceFromTable__Q22cf13CfGameManagerFv(self->unkEE) < self->unkF0) {
        return;
    }
    if (self->unkE4 != 0 && CfGimmick_CheckStateFlag2CC8(self->unkE4) == 0) {
        return;
    }
    if (objectReady != 0) {
        self->state = 9;
    } else {
        self->state = 1;
    }
}

extern "C" void func_8020D998(WarpData* self) {
    self->flags |= 0x10;
    if ((self->flags & 0x40) != 0 &&
        CfGimmick_CheckTriggerGated(self->configFlags, &self->scale, &self->position,
                      &self->rotation, self->object7cState) != 0) {
        self->phase = 0;
        if ((self->configFlags & 1) != 0) {
            CfGimmick_SetGlobalFlagC0002();
            CfGimmick_SetGlobalFlagD0000();
            cf::CfObject* player = playerFromRaw(cf::CfGameManager::getPlayer(0));
            if (player != 0) {
                cf::CObjectState* sub = *reinterpret_cast<cf::CObjectState**>(reinterpret_cast<u8*>(player) + 4);
                u32 value = *static_cast<u32*>(sub->CObjectState_getStateData());
                if (func_80174C98(player, &value, 0x803) != 0) {
                    cf::CfObjectMove* tail = reinterpret_cast<cf::CfObjectMove*>(reinterpret_cast<u8*>(player) + 0x3E9C);
                    if (!tail->CObjectState_checkStateFlags(0x200)) {
                        tail->CObjectState_setStateBitMask(0x200);
                    }
                    self->state = 4;
                    self->timer = lbl_eu_806683C8;
                    return;
                }
            }

            self->state = 5;
            if (self->flagE7 != 0) {
                if (self->object7c != 0) {
                    setChildB59__(self->object7c, 1);
                    self->object7c->owner = 0;
                    self->object7c = 0;
                }
                if (self->object100 != 0) {
                    setChildB59__(self->object100, 1);
                    self->object100->owner = 0;
                    self->object100 = 0;
                }
                if (self->object104 != 0) {
                    setChildB59__(self->object104, 1);
                    self->object104->owner = 0;
                    self->object104 = 0;
                }
                if (self->soundHandle != 0) {
                    CfSoundMan_ApplySlotStop(1, self->soundHandle, 0xa);
                    self->soundHandle = 0;
                }
                self->flags &= ~1u;

                WarpObject* first = (WarpObject*)createBattleActor__Q22cf13CfGameManagerFv(self->flagE7, 0);
                self->object7c = first;
                if (first != 0) {
                    bindPartnerO_(first, cf::CfGameManager::getPlayer(0), 0);
                }
                WarpObject* second = (WarpObject*)createBattleActor__Q22cf13CfGameManagerFv(self->flagE7, 0);
                self->object100 = second;
                if (second != 0) {
                    bindPartnerO_(second, cf::CfGameManager::getPlayer(1), 0);
                }
                WarpObject* third = (WarpObject*)createBattleActor__Q22cf13CfGameManagerFv(self->flagE7, 0);
                self->object104 = third;
                if (third != 0) {
                    bindPartnerO_(third, cf::CfGameManager::getPlayer(2), 0);
                }

                if (self->object7c != 0) {
                    self->object7c->owner = self;
                }
                if (self->object100 != 0) {
                    self->object100->owner = self;
                }
                if (self->object104 != 0) {
                    self->object104->owner = self;
                }
                if (self->unkE8 != 0) {
                    self->soundHandle = CfGimmick_PlaySoundAtPos(self->unkE8, &lbl_eu_805765A0);
                }
                self->flags |= 1;
            }
            CfGimmick_ApplyPartyMoveSpeed(lbl_eu_806683D0);
        } else {
            self->state = 2;
            self->timer = lbl_eu_806683D4;
        }
    } else if ((self->flags & 0x80) != 0 &&
               CfGimmick_CheckTriggerGated(self->flagE6, &self->destination3, &self->destination,
                             &self->destination2, self->object108) != 0) {
        self->phase = 1;
        if ((self->flagE6 & 1) != 0) {
            CfGimmick_SetGlobalFlagC0002();
            CfGimmick_SetGlobalFlagD0000();
            cf::CfObject* player = playerFromRaw(cf::CfGameManager::getPlayer(0));
            if (player != 0) {
                cf::CObjectState* sub = *reinterpret_cast<cf::CObjectState**>(reinterpret_cast<u8*>(player) + 4);
                u32 value = *static_cast<u32*>(sub->CObjectState_getStateData());
                if (func_80174C98(player, &value, 0x803) != 0) {
                    cf::CfObjectMove* tail = reinterpret_cast<cf::CfObjectMove*>(reinterpret_cast<u8*>(player) + 0x3E9C);
                    if (!tail->CObjectState_checkStateFlags(0x200)) {
                        tail->CObjectState_setStateBitMask(0x200);
                    }
                    self->state = 4;
                    self->timer = lbl_eu_806683C8;
                    return;
                }
            }

            u8 flagE7 = self->flagE7;
            self->state = 5;
            if (self->flagE7 != 0) {
                if (self->object7c != 0) {
                    setChildB59__(self->object7c, 1);
                    self->object7c->owner = 0;
                    self->object7c = 0;
                }
                if (self->object100 != 0) {
                    setChildB59__(self->object100, 1);
                    self->object100->owner = 0;
                    self->object100 = 0;
                }
                if (self->object104 != 0) {
                    setChildB59__(self->object104, 1);
                    self->object104->owner = 0;
                    self->object104 = 0;
                }
                if (self->soundHandle != 0) {
                    CfSoundMan_ApplySlotStop(1, self->soundHandle, 0xa);
                    self->soundHandle = 0;
                }
                self->flags &= ~1u;

                WarpObject* first = (WarpObject*)createBattleActor__Q22cf13CfGameManagerFv(self->flagE7, 0);
                self->object7c = first;
                if (first != 0) {
                    bindPartnerO_(first, cf::CfGameManager::getPlayer(0), 0);
                }
                WarpObject* second = (WarpObject*)createBattleActor__Q22cf13CfGameManagerFv(self->flagE7, 0);
                self->object100 = second;
                if (second != 0) {
                    bindPartnerO_(second, cf::CfGameManager::getPlayer(1), 0);
                }
                WarpObject* third = (WarpObject*)createBattleActor__Q22cf13CfGameManagerFv(self->flagE7, 0);
                self->object104 = third;
                if (third != 0) {
                    bindPartnerO_(third, cf::CfGameManager::getPlayer(2), 0);
                }

                if (self->object7c != 0) {
                    self->object7c->owner = self;
                }
                if (self->object100 != 0) {
                    self->object100->owner = self;
                }
                if (self->object104 != 0) {
                    self->object104->owner = self;
                }
                if (self->unkE8 != 0) {
                    self->soundHandle = CfGimmick_PlaySoundAtPos(self->unkE8, &lbl_eu_805765A0);
                }
                self->flags |= 1;
            }
            CfGimmick_ApplyPartyMoveSpeed(lbl_eu_806683D0);
        } else {
            self->state = 2;
            self->timer = lbl_eu_806683D4;
        }
    }
}

extern "C" void func_8020DF04(WarpData* self) {
    bool valid;
    u8 phase = ((BattleMgrWarpState*)getInstance__Q22cf14CBattleManagerFv())->phase;
    if (phase >= 1 && phase <= 0x18) {
        valid = true;
    } else {
        valid = ((BattleMgrWarpState*)getInstance__Q22cf14CBattleManagerFv())->chainValue != 0;
    }
    if (valid) {
        self->timer = lbl_eu_806683D4;
        return;
    }

    bool ready;
    if (self->phase != 0) {
        ready = jumptable_eu_80535830[self->dispatchIndex](&self->destination3,
                                                            &lbl_eu_805765A0,
                                                            &self->destination) != 0;
    } else {
        ready = jumptable_eu_80535830[self->stateIndex](&self->scale,
                                                         &lbl_eu_805765A0,
                                                         &self->position) != 0;
    }
    if (!ready) {
        self->state = 1;
        return;
    }

    CfGimmick_SetGlobalFlag40000();
    self->timer -= Scn_GetFrameDelta(lbl_eu_80663E14);
    if (self->timer > lbl_eu_806683C8) {
        return;
    }

    CfGimmick_SetGlobalFlagC0002();
    CfGimmick_SetGlobalFlagD0000();
    cf::CfObject* player = playerFromRaw(cf::CfGameManager::getPlayer(0));
    if (player != 0) {
        cf::CObjectState* sub = *reinterpret_cast<cf::CObjectState**>(reinterpret_cast<u8*>(player) + 4);
        u32 value = *static_cast<u32*>(sub->CObjectState_getStateData());
        if (func_80174C98(player, &value, 0x803) != 0) {
            cf::CfObjectMove* tail = reinterpret_cast<cf::CfObjectMove*>(reinterpret_cast<u8*>(player) + 0x3E9C);
            if (!tail->CObjectState_checkStateFlags(0x200)) {
                tail->CObjectState_setStateBitMask(0x200);
            }
            self->state = 4;
            self->timer = lbl_eu_806683C8;
            return;
        }
    }

    self->state = 5;
    if (self->flagE7 != 0) {
        if (self->object7c != 0) {
            setChildB59__(self->object7c, 1);
            self->object7c->owner = 0;
            self->object7c = 0;
        }
        if (self->object100 != 0) {
            setChildB59__(self->object100, 1);
            self->object100->owner = 0;
            self->object100 = 0;
        }
        if (self->object104 != 0) {
            setChildB59__(self->object104, 1);
            self->object104->owner = 0;
            self->object104 = 0;
        }
        if (self->soundHandle != 0) {
            CfSoundMan_ApplySlotStop(1, self->soundHandle, 0xa);
            self->soundHandle = 0;
        }
        self->flags &= ~1u;

        WarpObject* first = (WarpObject*)createBattleActor__Q22cf13CfGameManagerFv(self->flagE7, 0);
        self->object7c = first;
        if (first != 0) {
            bindPartnerO_(first, cf::CfGameManager::getPlayer(0), 0);
        }
        WarpObject* second = (WarpObject*)createBattleActor__Q22cf13CfGameManagerFv(self->flagE7, 0);
        self->object100 = second;
        if (second != 0) {
            bindPartnerO_(second, cf::CfGameManager::getPlayer(1), 0);
        }
        WarpObject* third = (WarpObject*)createBattleActor__Q22cf13CfGameManagerFv(self->flagE7, 0);
        self->object104 = third;
        if (third != 0) {
            bindPartnerO_(third, cf::CfGameManager::getPlayer(2), 0);
        }

        if (self->object7c != 0) {
            self->object7c->owner = self;
        }
        if (self->object100 != 0) {
            self->object100->owner = self;
        }
        if (self->object104 != 0) {
            self->object104->owner = self;
        }
        if (self->unkE8 != 0) {
            self->soundHandle = CfGimmick_PlaySoundAtPos(self->unkE8, &lbl_eu_805765A0);
        }
        self->flags |= 1;
    }
    CfGimmick_ApplyPartyMoveSpeed(lbl_eu_806683D0);
}

extern "C" void func_8020E27C(WarpData* self) {
    int ready = 0;
    if (self->phase != 0) {
        if (jumptable_eu_80535830[self->dispatchIndex](&self->destination3,
                                                        &lbl_eu_805765A0,
                                                        &self->destination) != 0) {
            ready = 1;
        }
    } else {
        if (jumptable_eu_80535830[self->stateIndex](&self->scale,
                                                     &lbl_eu_805765A0,
                                                     &self->position) != 0) {
            ready = 1;
        }
    }
    if (ready) {
        if ((self->flags & 1) == 0) {
            self->state = 6;
        }
        return;
    }

    clearWarpObjects(self);
    if (self->soundHandle != 0) {
        CfSoundMan_ApplySlotStop(1, self->soundHandle, 0xa);
        self->soundHandle = 0;
    }
    u32 flags = self->flags;
    self->state = 1;
    self->flags = flags & ~1u;
}

extern "C" void func_8020E3F0(WarpData* self) {
    bool allReady = true;
    for (int i = 0; i < 3; ++i) {
        cf::CfObjectMove* player = cf::CfGameManager::getPlayer(i);
        if (player == 0) {
            continue;
        }
        cf::CfObject* ctl = reinterpret_cast<cf::CfObject*>(reinterpret_cast<char*>(player) - 0x3E9C);
        cf::CObjectState* sub = *reinterpret_cast<cf::CObjectState**>(reinterpret_cast<u8*>(ctl) + 4);
        u32 stateA = *static_cast<u32*>(sub->CObjectState_getStateData());
        if (func_80174C98(ctl, &stateA, 8) == 0) {
            u32 stateB = *static_cast<u32*>(sub->CObjectState_getStateData());
            if (func_80174C98(ctl, &stateB, 7) == 0) {
                continue;
            }
        }
        allReady = false;
        break;
    }

    self->timer += Scn_GetFrameDelta(lbl_eu_80663E14);
    if (lbl_eu_806683D8 <= self->timer ||
        (allReady && lbl_eu_806683D0 <= self->timer)) {
        CfGimmick_SetGlobalFlagC0002();
        CfGimmick_SetGlobalFlagD0000();
        self->state = 5;
        if (self->flagE7 != 0) {
            if (self->object7c != 0) {
                setChildB59__(self->object7c, 1);
                self->object7c->owner = 0;
                self->object7c = 0;
            }
            if (self->object100 != 0) {
                setChildB59__(self->object100, 1);
                self->object100->owner = 0;
                self->object100 = 0;
            }
            if (self->object104 != 0) {
                setChildB59__(self->object104, 1);
                self->object104->owner = 0;
                self->object104 = 0;
            }
            if (self->soundHandle != 0) {
                CfSoundMan_ApplySlotStop(1, self->soundHandle, 0xa);
                self->soundHandle = 0;
            }
            self->flags &= ~1u;

            self->object7c = (WarpObject*)createBattleActor__Q22cf13CfGameManagerFv(self->flagE7, 0);
            if (self->object7c != 0) {
                bindPartnerO_(self->object7c, cf::CfGameManager::getPlayer(0), 0);
            }
            self->object100 = (WarpObject*)createBattleActor__Q22cf13CfGameManagerFv(self->flagE7, 0);
            if (self->object100 != 0) {
                bindPartnerO_(self->object100, cf::CfGameManager::getPlayer(1), 0);
            }
            self->object104 = (WarpObject*)createBattleActor__Q22cf13CfGameManagerFv(self->flagE7, 0);
            if (self->object104 != 0) {
                bindPartnerO_(self->object104, cf::CfGameManager::getPlayer(2), 0);
            }

            if (self->object7c != 0) {
                self->object7c->owner = self;
            }
            if (self->object100 != 0) {
                self->object100->owner = self;
            }
            if (self->object104 != 0) {
                self->object104->owner = self;
            }
            if (self->unkE8 != 0) {
                self->soundHandle = CfGimmick_PlaySoundAtPos(self->unkE8, &lbl_eu_805765A0);
            }
            self->flags |= 1;
        }
        CfGimmick_ApplyPartyMoveSpeed(lbl_eu_806683D0);
    } else {
        CfGimmick_SetGlobalFlagC0002();
        CfGimmick_SetGlobalFlagD0000();
    }
}

extern "C" void CfGimmickWarp_EnterState6(WarpData* self) {
    CfGimmick_SetGlobalFlagC0002();
    CfGimmick_SetGlobalFlagD0000();
    if ((self->flags & 1) == 0) {
        self->state = 6;
    }
}

extern "C" void func_8020E704(WarpData* self) {
    CfGimmick_SetGlobalFlagC0002();
    CfGimmick_SetGlobalFlagD0000();
    if ((self->flags & 2) != 0) {
        self->timer -= Scn_GetFrameDelta(lbl_eu_80663E14);
        if (self->timer > lbl_eu_806683C8) {
            return;
        }
        self->flags &= ~2u;
        cfCam_syncFollowD();

        cf::CfObject* first = playerFromRaw(cf::CfGameManager::getPlayer(0));
        WarpVec3 centre;
        f32 distance;
        if (first != 0) {
            centre = self->position;
            distance = (f32)self->unkEA * lbl_eu_8066A210;
        } else {
            centre = self->destination;
            distance = (f32)self->unkEC * lbl_eu_8066A210;
        }

        WarpVec3 adjusted;
        adjusted.x = lbl_eu_806683C8;
        adjusted.y = lbl_eu_806683DC;
        adjusted.z = lbl_eu_806683C8;
        WarpVec3 sum;
        sum.x = centre.x + adjusted.x;
        sum.y = centre.y + adjusted.y;
        sum.z = centre.z + adjusted.z;
        WarpVec3 effectPos = sum;
        if (ScnRes_VertRayForward_E398(&effectPos, 0, 0, 0, lbl_eu_806683E0, lbl_eu_8066AF20)) {
            func_804BE4B4(&centre, 0);
        }

        if (first != 0) {
            first->CfObject_syncMoveTarget(reinterpret_cast<const ml::CVec3*>(&centre));
            first->CfObject_setMoveYaw(distance);
        }

        for (int i = 1; i < 3; ++i) {
            cf::CfObject* player = playerFromRaw(cf::CfGameManager::getPlayer(i));
            if (player == 0) {
                continue;
            }
            WarpVec3 base = centre;
            WarpVec3 out68;
            func_80198710(&out68, &base, distance, 6, i,
                          lbl_eu_806683DC, lbl_eu_806683C8);
            WarpVec3 out4c;
            if (CPartsChange_ProcessPartyInfo(&out68, &out4c) != 0) {
                player->CfObject_syncMoveTarget(reinterpret_cast<const ml::CVec3*>(&out4c));
                player->CfObject_setMoveYaw(distance);
                WarpObject* object = reinterpret_cast<WarpObject*>(reinterpret_cast<cf::CfObject*>(reinterpret_cast<u8*>(player) + 0x3E9C)->CfObject_getCurrentTarget());
                if (object != 0) {
                    movePcResetAndSetPos(object->field_8c, &centre);
                }
            }
        }

        cf::CObjectState* manager = reinterpret_cast<cf::CObjectState*>(getCameraDataBlock__Q22cf13CfGameManagerFv());
        if (manager != 0) {
            manager->CObjectState_checkStateFlags(0);
        }
        self->state = 7;
        self->timer = lbl_eu_806683D0;
    } else {
        self->timer = lbl_eu_806683E4;
        self->flags |= 2;
        WarpVec4 area;
        area.x = lbl_eu_806683C8;
        area.y = lbl_eu_806683C8;
        area.z = lbl_eu_806683C8;
        area.w = lbl_eu_806683E8;
        func_8008566C__Q22cf13CfGameManagerFv(0x14, &area, 1);
    }
}

extern "C" void func_8020EA2C(WarpData* self) {
    CfGimmick_SetGlobalFlagC0002();
    CfGimmick_SetGlobalFlagD0000();

    u32 flags = self->flags;
    if ((flags & 2) == 0) {
        f32 limit = lbl_eu_806683C8;
        self->timer -= Scn_GetFrameDelta(lbl_eu_80663E14);
        if (self->timer <= limit) {
            self->flags = flags | 2;
            self->timer = lbl_eu_806683F8;
            WarpVec4 area;
            area.x = limit;
            area.y = limit;
            area.z = limit;
            area.w = limit;
            func_8008566C__Q22cf13CfGameManagerFv(0x14, &area, 1);
        }
        return;
    }

    f32 t = self->timer - Scn_GetFrameDelta(lbl_eu_80663E14);
    self->timer = t;
    if ((flags & 4) == 0) {
        if (t > lbl_eu_806683CC) {
            if (t > lbl_eu_806683D4 || (flags & 8) != 0) {
            } else {
                if (self->flagE7 != 0) {
                    clearObject(self->object7c);
                    clearObject(self->object100);
                    clearObject(self->object104);
                    if (self->soundHandle != 0) {
                        CfSoundMan_ApplySlotStop(1, self->soundHandle, 0xa);
                        self->soundHandle = 0;
                    }
                    self->flags = flags & ~1u;

                    self->object7c = (WarpObject*)createBattleActor__Q22cf13CfGameManagerFv(self->flagE7, 0);
                    if (self->object7c != 0) {
                        bindPartnerO_(self->object7c, cf::CfGameManager::getPlayer(0), 0);
                    }
                    self->object100 = (WarpObject*)createBattleActor__Q22cf13CfGameManagerFv(self->flagE7, 0);
                    if (self->object100 != 0) {
                        bindPartnerO_(self->object100, cf::CfGameManager::getPlayer(1), 0);
                    }
                    self->object104 = (WarpObject*)createBattleActor__Q22cf13CfGameManagerFv(self->flagE7, 0);
                    if (self->object104 != 0) {
                        bindPartnerO_(self->object104, cf::CfGameManager::getPlayer(2), 0);
                    }

                    if (self->object7c != 0) {
                        self->object7c->owner = self;
                    }
                    if (self->object100 != 0) {
                        self->object100->owner = self;
                    }
                    if (self->object104 != 0) {
                        self->object104->owner = self;
                    }
                    if (self->unkE8 != 0) {
                        self->soundHandle = CfGimmick_PlaySoundAtPos(self->unkE8, &lbl_eu_805765A0);
                    }
                    self->flags |= 1;
                }
                self->flags |= 8;
            }
        } else {
            u32 cur = self->flags;
            self->flags = cur | 4;
            CfGimmick_ApplyPartyMoveSpeedGated(cur);
        }
        for (int i = 0; i < 3; ++i) {
            cf::CfObject* player = reinterpret_cast<cf::CfObject*>(cf::CfGameManager::getPlayer(i));
            if (player != 0) {
                player->CfObject_syncModelRate(lbl_eu_806683E8);
            }
        }
    }

    if (self->timer > lbl_eu_806683C8) {
        return;
    }
    self->timer = lbl_eu_806683C8;
    if ((self->flags & 1) != 0) {
        return;
    }
    self->state = 8;
    self->flags &= 0x1Fu;
}

extern "C" void CfGimmickWarp_CheckReady(WarpData* self) {
    int ready = 0;
    if (self->phase != 0) {
        if ((self->configFlags & 1) != 0) {
            ready = 1;
        } else if (jumptable_eu_80535830[self->stateIndex](&self->scale,
                                                            &lbl_eu_805765A0,
                                                            &self->position) == 0) {
            ready = 1;
        }
    } else if ((self->flagE6 & 1) != 0) {
        ready = 1;
    } else if (jumptable_eu_80535830[self->dispatchIndex](&self->destination3,
                                                          &lbl_eu_805765A0,
                                                          &self->destination) == 0) {
        ready = 1;
    }
    if (ready != 0) {
        self->state = 1;
    }
}

extern "C" void CfGimmickWarp_CheckReadyBoth(WarpData* self) {
    int firstReady = 0;
    int secondReady = 0;
    if ((self->configFlags & 1) != 0) {
        firstReady = 1;
    } else if (jumptable_eu_80535830[self->stateIndex](&self->scale,
                                                       &lbl_eu_805765A0,
                                                       &self->position) == 0) {
        firstReady = 1;
    }
    if ((self->flagE6 & 1) != 0) {
        secondReady = 1;
    } else if (jumptable_eu_80535830[self->dispatchIndex](&self->destination3,
                                                          &lbl_eu_805765A0,
                                                          &self->destination) == 0) {
        secondReady = 1;
    }
    if (firstReady != 0 && secondReady != 0) {
        self->state = 1;
    }
}
