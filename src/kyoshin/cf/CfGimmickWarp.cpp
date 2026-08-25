// High-level reconstruction of cf::CfGimmickWarp.
// Retail disassembly is used only as a reference for the data layout and
// control flow; all implementation below is ordinary C/C++.

#include "kyoshin/cf/CBattleManagerApi.hpp"
#include "kyoshin/harness_catalog.hpp"
#include "monolib/scn/CScnTimeApi.hpp"
#include "kyoshin/cf/CfGimmickWarp.hpp"
#include "kyoshin/cf/CfGameManager.hpp"

struct WarpData;
struct WarpObject;

struct WarpVec3 {
    f32 x;
    f32 y;
    f32 z;
};

struct WarpVec4 {
    f32 x;
    f32 y;
    f32 z;
    f32 w;
};

/* The game stores a twelve-byte MWCC pointer-to-member-function table. */
typedef void (WarpData::*WarpStateProc)();
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
    u8 field_8c[0x90 - 0x8c]; /* sub-record passed to func_80199810 */
    u8 pad90[0xB0 - 0x90];
    WarpData* owner;
};

/* This is the portion of a player object used by this translation unit. */
struct WarpPlayerHead {
    void* vtable;
    void* subObject;
    u8 pad08[0x3E94];
};

struct WarpPlayerTail {
    void* vtable;
};

struct WarpPlayer : WarpPlayerHead, WarpPlayerTail {
};

/* Cast-only interfaces retain the retail vtable offsets without inventing
 * storage or a second object hierarchy. */
struct WarpPlayerSubIf {
    virtual void v00();
    virtual void v04();
    virtual void v08();
    virtual void v0c();
    virtual void v10();
    virtual void v14();
    virtual void v18();
    virtual void v1c();
    virtual void v20();
    virtual void v24();
    virtual void v28();
    virtual void v2c();
    virtual u32* v30();
};

struct WarpPlayerHeadIf {
    virtual void v00();
    virtual void v04();
    virtual void v08();
    virtual void v0c();
    virtual void v10();
    virtual void v14();
    virtual void v18();
    virtual void v1c();
    virtual void v20();
    virtual void v24();
    virtual void v28();
    virtual void v2c();
    virtual void v30();
    virtual void v34();
    virtual void v38();
    virtual void v3c();
    virtual void v40();
    virtual void v44();
    virtual void v48();
    virtual void v4c();
    virtual void v50();
    virtual void v54();
    virtual void v58();
    virtual void v5c();
    virtual void v60();
    virtual void v64();
    virtual void v68();
    virtual void v6c();
    virtual void v70();
    virtual void v74();
    virtual void v78();
    virtual void v7c();
    virtual void v80();
    virtual void v84();
    virtual void v88();
    virtual void v8c();
    virtual void v90();
    virtual void v94();
    virtual void v98();
    virtual void v9c();
    virtual void va0();
    virtual void va4();
    virtual void va8(const WarpVec3*);
    virtual void vac();
    virtual void vb0();
    virtual void vb4();
    virtual void vb8();
    virtual void vbc();
    virtual void vc0();
    virtual void vc4();
    virtual void vc8(f32);
    virtual void vcc();
    virtual void vd0();
    virtual void vd4();
    virtual void vd8();
    virtual void vdc();
    virtual void ve0();
    virtual void ve4();
    virtual void ve8();
    virtual void vec();
    virtual void vf0();
    virtual void vf4();
    virtual void vf8();
    virtual void vfc();
    virtual void v100();
    virtual void v104();
    virtual void v108();
    virtual void v10c();
    virtual void v110();
    virtual void v114();
    virtual void v118();
    virtual void v11c();
    virtual void v120();
    virtual void v124();
    virtual void v128();
    virtual void v12c();
    virtual void v130();
    virtual void v134();
    virtual void v138();
    virtual void v13c();
    virtual void v140();
    virtual void v144();
    virtual void v148();
    virtual void v14c();
    virtual void v150();
    virtual void v154();
    virtual void v158();
    virtual void v15c();
    virtual void v160();
    virtual void v164();
    virtual void v168(f32);
};

struct WarpPlayerTailIf {
    virtual void v00();
    virtual void v04();
    virtual void v08(u32);
    virtual int v0c(u32);
    virtual void v10();
    virtual void v14();
    virtual void v18();
    virtual void v1c();
    virtual void v20();
    virtual void v24();
    virtual void v28();
    virtual void v2c();
    virtual void v30();
    virtual void v34();
    virtual void v38();
    virtual void v3c();
    virtual void v40();
    virtual void v44();
    virtual void v48();
    virtual void v4c();
    virtual void v50();
    virtual void v54();
    virtual void v58();
    virtual void v5c();
    virtual void v60();
    virtual void v64();
    virtual void v68();
    virtual void v6c();
    virtual void v70();
    virtual void v74();
    virtual void v78();
    virtual void v7c();
    virtual void v80();
    virtual void v84();
    virtual void v88();
    virtual void v8c();
    virtual void v90();
    virtual void v94();
    virtual void v98();
    virtual void v9c();
    virtual void va0();
    virtual void va4();
    virtual void va8();
    virtual void vac();
    virtual void vb0();
    virtual void vb4();
    virtual void vb8();
    virtual void vbc();
    virtual void vc0();
    virtual void vc4();
    virtual void vc8();
    virtual void vcc();
    virtual void vd0();
    virtual void vd4();
    virtual void vd8();
    virtual void vdc();
    virtual void ve0();
    virtual void ve4();
    virtual void ve8();
    virtual void vec();
    virtual void vf0();
    virtual void vf4();
    virtual void vf8();
    virtual void vfc();
    virtual void v100();
    virtual void v104();
    virtual void v108();
    virtual void v10c();
    virtual WarpObject* v110();
};

struct WarpVirtualIf {
    virtual void v00();
    virtual void v04();
    virtual void v08();
    virtual void v0c();
    virtual void v10();
    virtual void v14();
    virtual void v18();
    virtual void v1c();
    virtual void v20();
};

struct WarpData {
    void* vtable;                         // 0x00
    WarpVec3 position;                    // 0x04
    WarpVec3 rotation;                    // 0x10
    WarpVec3 scale;                       // 0x1c
    u8 basePad28[0x38];                   // 0x28
    u32 stateIndex;                       // 0x60
    u16 rowId;                            // 0x64
    u16 configFlags;                      // 0x66
    u16 unk68;                            // 0x68
    u16 resourceId;                       // 0x6a
    u16 minValue;                         // 0x6c
    u16 maxValue;                         // 0x6e
    u8 lod[4];                            // 0x70
    u32 flags;                            // 0x74
    WarpObject* object7c;                 // 0x78
    WarpObject* object7cState;            // 0x7c
    u16 soundHandle;                      // 0x80
    u16 typeId;                           // 0x82
    WarpVec3 destination;                 // 0x84
    WarpVec3 destination2;                // 0x90
    WarpVec3 destination3;                // 0x9c
    u8 derivedPadA8[0x38];                // 0xa8
    u32 dispatchIndex;                    // 0xe0
    u16 unkE4;                            // 0xe4
    u8 flagE6;                             // 0xe6
    u8 flagE7;                             // 0xe7
    u16 unkE8;                            // 0xe8
    s16 unkEA;                            // 0xea
    s16 unkEC;                            // 0xec
    u16 unkEE;                            // 0xee
    u16 unkF0;                            // 0xf0
    u8 padF2[4];                          // 0xf2
    u16 state;                            // 0xf6
    u16 phase;                            // 0xf8
    f32 timer;                            // 0xfc
    WarpObject* object100;                // 0x100
    WarpObject* object104;                // 0x104
    WarpObject* object108;                // 0x108
};

extern "C" {
extern WarpColumns lbl_eu_805357E8;
extern char lbl_eu_8050870C[];
extern u8 lbl_eu_805359B0[];
extern WarpStateProc lbl_eu_80535938[];
extern WarpJumpProc jumptable_eu_80535830[];
extern WarpVec3 lbl_eu_805765A0;
extern void* lbl_eu_80664134;
extern u32 lbl_eu_806646BC;
extern void* lbl_eu_80663E14;
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
void func_8020938C(WarpData*, WarpVec3*, void*, u32*, int);
void func_802095D8(WarpData*, WarpVec3*, void*, u32*, int);
void func_80209488(WarpData*, WarpVec3*, void*, u32*, int);
void func_802089BC(WarpVec3*, const WarpVec3*, const WarpVec3*);
void func_80208EE4(WarpData*);
void func_8020A434(WarpObject*);
void func_8020A6B0(WarpObject**, WarpVec3*, u16, f32, int, int);
int func_8020A5DC(WarpData*);
int func_8020A87C(WarpData*, WarpObject*);
void func_8020A484(u16);
u32 func_80082354__Q22cf13CfGameManagerFv();
int func_8020971C(int);
int func_80209754(int, WarpVec3*, WarpVec3*, WarpVec3*, WarpObject*);
void func_80209F5C();
void func_80209FB8();
void func_80209FE4();
void func_8020A124(WarpData*, f32);
void func_8020A1DC(WarpData*);
void func_801BFED0(int, u16, int);
WarpObject* func_800817BC__Q22cf13CfGameManagerFv(u8, int);
void func_8006CC4C();
u32 func_80174C98(WarpPlayer*, u32*, u32);
void func_800ACC14(WarpObject*, int);
int func_804BE398(WarpVec3*, u32, u32, u32, f32, f32);
void func_804BE4B4(WarpVec3*, int);
u16 func_80208C48(u16, const WarpVec3*);
void func_8008566C__Q22cf13CfGameManagerFv(int, WarpVec4*, int);
void func_80198710(WarpVec3*, const WarpVec3*, f32, int, int, f32, f32);
int func_8019876C(WarpVec3*, WarpVec3*);
void func_80199810(u8*, const WarpVec3*);
WarpObject* func_800821F8__Q22cf13CfGameManagerFv();
void __dt__Q22cf9CfGimmickFv(WarpData*, int);
void __dl__FPv(void*);
}

static WarpPlayer* playerFromTail(WarpPlayerTail* tail) {
    if (tail == 0) {
        return 0;
    }
    return static_cast<WarpPlayer*>(tail);
}

static WarpPlayerTail* playerTail(WarpPlayer* player) {
    if (player == 0) {
        return 0;
    }
    return static_cast<WarpPlayerTail*>(player);
}

static void clearObject(WarpObject*& object) {
    if (object != 0) {
        func_800ACC14(object, 1);
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

static void preparePlayers(WarpData* self) {
    self->flags &= ~1u;
    WarpObject* first = func_800817BC__Q22cf13CfGameManagerFv(self->flagE7, 0);
    self->object7c = first;
    if (first != 0) {
        WarpPlayerTail* p0 = (WarpPlayerTail*)cf::CfGameManager::getPlayer(0);
        func_800ACF78(first, p0, 0);
    }

    WarpObject* second = func_800817BC__Q22cf13CfGameManagerFv(self->flagE7, 0);
    self->object100 = second;
    if (second != 0) {
        WarpPlayerTail* p1 = (WarpPlayerTail*)cf::CfGameManager::getPlayer(1);
        func_800ACF78(second, p1, 0);
    }

    WarpObject* third = func_800817BC__Q22cf13CfGameManagerFv(self->flagE7, 0);
    self->object104 = third;
    if (third != 0) {
        WarpPlayerTail* p2 = (WarpPlayerTail*)cf::CfGameManager::getPlayer(2);
        func_800ACF78(third, p2, 0);
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
        func_8020A484(self->unkE8);
    }
    self->flags |= 1;
}

extern "C" void __ct__cf_CfGimmickWarp(WarpData* self, u16 rowId) {
    __ct__cf_CfGimmick(self);
    self->vtable = &lbl_eu_805359B0;
    self->typeId = 4;

    void* manager = func_8003AA34();
    u32 tableValue = (u32)lbl_eu_80664134;
    self->rowId = rowId;

    u32 valueE4 = getBdatStringColumnValue((void*)tableValue, lbl_eu_805357E8.c2c, rowId);
    self->unkE4 = (u16)valueE4;
    u32 valueE7 = getBdatStringColumnValue((void*)tableValue, lbl_eu_8050870C, rowId);
    self->flagE7 = (u8)valueE7;
    u32 valueE8 = getBdatStringColumnValue((void*)tableValue, lbl_eu_8050870C + 7, rowId);
    self->unkE8 = (u16)valueE8;
    u32 value6A = getBdatStringColumnValue((void*)tableValue, lbl_eu_805357E8.c30, rowId);

    func_8020938C(self, &self->position, manager, &tableValue, 0);
    func_802095D8(self, &self->rotation, manager, &tableValue, 0);
    func_80209488(self, &self->scale, manager, &tableValue, 0);
    func_8020938C(self, &self->destination, manager, &tableValue, 1);
    func_802095D8(self, &self->destination2, manager, &tableValue, 1);
    func_80209488(self, &self->destination3, manager, &tableValue, 1);

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

    reinterpret_cast<WarpVirtualIf*>(self)->v20();

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
        self->vtable = (void*)lbl_eu_805359B0;
        func_80208EE4(self);
        func_8020A434((WarpObject*)((u8*)self + 0x7c));
        func_8020A434((WarpObject*)((u8*)self + 0x108));
        __dt__Q22cf9CfGimmickFv(self, 0);
        if (deleteFlag > 0) {
            __dl__FPv(self);
        }
    }
    return self;
}

extern "C" void func_8020D6FC(WarpData* self) {
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
        func_8020A434((WarpObject*)(&self->object7cState));
        func_8020A434((WarpObject*)(&self->object108));
    }
}

extern "C" void func_8020D7BC(WarpData* self) {
    u32 flags = self->flags;
    self->object104 = 0;
    self->object100 = 0;
    self->object7c = 0;
    self->flags = flags & ~1u;
}

extern "C" void func_8020D7DC(WarpData* self) {
    func_802089BC(&self->scale, &self->position, &self->rotation);
    func_802089BC(&self->destination3, &self->destination, &self->destination2);
}

extern "C" void func_8020D824(WarpData* self) {
    if ((lbl_eu_806646BC & 2) == 0) {
        return;
    }

    /* retail issues two flag-word loads here (lwz r4/r5 + extrwi); the
       volatile reference keeps MWCC from merging them into one load */
    volatile u32& flagsRef = self->flags;
    u32 flags = flagsRef;
    bool objectReady = (flagsRef >> 8) & 1;
    flagsRef = flags & ~0x100u;
    if ((flags & 0x20) != 0) {
        if (func_8020A5DC(self) != 0) {
            return;
        }
        self->flags &= ~0x20u;
    } else {
        int allReady = 0;
        if ((self->configFlags & 1) != 0) {
            allReady = (allReady | func_8020A87C(self, self->object7cState)) != 0;
        }
        if ((self->flagE6 & 1) != 0) {
            allReady = (allReady | func_8020A87C(self, self->object108)) != 0;
        }
        if (allReady != 0) {
            func_8020A484(self->resourceId);
            self->flags |= 0x20;
            return;
        }
    }

    self->flags |= 0x10;
    if (self->maxValue != 0) {
        u32 current = cf::CfGameManager::func_800822F4();
        if (self->minValue > current || current > self->maxValue) {
            return;
        }
    }
    if (self->unkEE != 0 && func_80082354__Q22cf13CfGameManagerFv() < self->unkF0) {
        return;
    }
    /* NOTE: retail passes r3 = the lhz-loaded unkE4 value here (no mr self) */
    if (self->unkE4 != 0 && func_8020971C(self->unkE4) == 0) {
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
        func_80209754(self->configFlags, &self->scale, &self->position,
                      &self->rotation, self->object7cState) != 0) {
        self->phase = 0;
        if ((self->configFlags & 1) != 0) {
            func_80209F5C();
            func_80209FB8();
            WarpPlayer* player = playerFromTail((WarpPlayerTail*)cf::CfGameManager::getPlayer(0));
            if (player != 0) {
                WarpPlayerSubIf* sub = reinterpret_cast<WarpPlayerSubIf*>(player->subObject);
                u32 value = *sub->v30();
                if (func_80174C98(player, &value, 0x803) != 0) {
                    WarpPlayerTailIf* tail = reinterpret_cast<WarpPlayerTailIf*>(playerTail(player));
                    if (!tail->v0c(0x200)) {
                        tail->v08(0x200);
                    }
                    self->state = 4;
                    self->timer = lbl_eu_806683C8;
                    return;
                }
            }

            /* Teleport requested: tear down the previous warp objects and
               re-create one per active player. */
            self->state = 5;
            if (self->flagE7 != 0) {
                if (self->object7c != 0) {
                    func_800ACC14(self->object7c, 1);
                    self->object7c->owner = 0;
                    self->object7c = 0;
                }
                if (self->object100 != 0) {
                    func_800ACC14(self->object100, 1);
                    self->object100->owner = 0;
                    self->object100 = 0;
                }
                if (self->object104 != 0) {
                    func_800ACC14(self->object104, 1);
                    self->object104->owner = 0;
                    self->object104 = 0;
                }
                if (self->soundHandle != 0) {
                    func_801BFED0(1, self->soundHandle, 0xa);
                    self->soundHandle = 0;
                }
                self->flags &= ~1u;

                WarpObject* first = func_800817BC__Q22cf13CfGameManagerFv(self->flagE7, 0);
                self->object7c = first;
                if (first != 0) {
                    func_800ACF78(first, cf::CfGameManager::getPlayer(0), 0);
                }
                WarpObject* second = func_800817BC__Q22cf13CfGameManagerFv(self->flagE7, 0);
                self->object100 = second;
                if (second != 0) {
                    func_800ACF78(second, cf::CfGameManager::getPlayer(1), 0);
                }
                WarpObject* third = func_800817BC__Q22cf13CfGameManagerFv(self->flagE7, 0);
                self->object104 = third;
                if (third != 0) {
                    func_800ACF78(third, cf::CfGameManager::getPlayer(2), 0);
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
                    self->soundHandle = func_80208C48(self->unkE8, &lbl_eu_805765A0);
                }
                self->flags |= 1;
            }
            func_8020A124(self, lbl_eu_806683D0);
        } else {
            self->state = 2;
            self->timer = lbl_eu_806683D4;
        }
    } else if ((self->flags & 0x80) != 0 &&
               func_80209754(self->flagE6, &self->destination3, &self->destination,
                             &self->destination2, self->object108) != 0) {
        self->phase = 1;
        if ((self->flagE6 & 1) != 0) {
            func_80209F5C();
            func_80209FB8();
            WarpPlayer* player = playerFromTail((WarpPlayerTail*)cf::CfGameManager::getPlayer(0));
            if (player != 0) {
                WarpPlayerSubIf* sub = reinterpret_cast<WarpPlayerSubIf*>(player->subObject);
                u32 value = *sub->v30();
                if (func_80174C98(player, &value, 0x803) != 0) {
                    WarpPlayerTailIf* tail = reinterpret_cast<WarpPlayerTailIf*>(playerTail(player));
                    if (!tail->v0c(0x200)) {
                        tail->v08(0x200);
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
                    func_800ACC14(self->object7c, 1);
                    self->object7c->owner = 0;
                    self->object7c = 0;
                }
                if (self->object100 != 0) {
                    func_800ACC14(self->object100, 1);
                    self->object100->owner = 0;
                    self->object100 = 0;
                }
                if (self->object104 != 0) {
                    func_800ACC14(self->object104, 1);
                    self->object104->owner = 0;
                    self->object104 = 0;
                }
                if (self->soundHandle != 0) {
                    func_801BFED0(1, self->soundHandle, 0xa);
                    self->soundHandle = 0;
                }
                self->flags &= ~1u;

                WarpObject* first = func_800817BC__Q22cf13CfGameManagerFv(self->flagE7, 0);
                self->object7c = first;
                if (first != 0) {
                    func_800ACF78(first, cf::CfGameManager::getPlayer(0), 0);
                }
                WarpObject* second = func_800817BC__Q22cf13CfGameManagerFv(self->flagE7, 0);
                self->object100 = second;
                if (second != 0) {
                    func_800ACF78(second, cf::CfGameManager::getPlayer(1), 0);
                }
                WarpObject* third = func_800817BC__Q22cf13CfGameManagerFv(self->flagE7, 0);
                self->object104 = third;
                if (third != 0) {
                    func_800ACF78(third, cf::CfGameManager::getPlayer(2), 0);
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
                    self->soundHandle = func_80208C48(self->unkE8, &lbl_eu_805765A0);
                }
                self->flags |= 1;
            }
            func_8020A124(self, lbl_eu_806683D0);
        } else {
            self->state = 2;
            self->timer = lbl_eu_806683D4;
        }
    }
}

extern "C" void func_8020DF04(WarpData* self) {
    /* A warp request is valid only during the battle-manager window. */
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

    func_80209FE4();
    self->timer -= func_80496288(lbl_eu_80663E14);
    if (self->timer > lbl_eu_806683C8) {
        return;
    }

    func_80209F5C();
    func_80209FB8();
    WarpPlayer* player = playerFromTail((WarpPlayerTail*)cf::CfGameManager::getPlayer(0));
    if (player != 0) {
        WarpPlayerSubIf* sub = reinterpret_cast<WarpPlayerSubIf*>(player->subObject);
        u32 value = *sub->v30();
        if (func_80174C98(player, &value, 0x803) != 0) {
            WarpPlayerTailIf* tail = reinterpret_cast<WarpPlayerTailIf*>(playerTail(player));
            if (!tail->v0c(0x200)) {
                tail->v08(0x200);
            }
            self->state = 4;
            self->timer = lbl_eu_806683C8;
            return;
        }
    }

    /* Teleport requested: tear down the previous warp objects and
       re-create one per active player. */
    self->state = 5;
    if (self->flagE7 != 0) {
        if (self->object7c != 0) {
            func_800ACC14(self->object7c, 1);
            self->object7c->owner = 0;
            self->object7c = 0;
        }
        if (self->object100 != 0) {
            func_800ACC14(self->object100, 1);
            self->object100->owner = 0;
            self->object100 = 0;
        }
        if (self->object104 != 0) {
            func_800ACC14(self->object104, 1);
            self->object104->owner = 0;
            self->object104 = 0;
        }
        if (self->soundHandle != 0) {
            func_801BFED0(1, self->soundHandle, 0xa);
            self->soundHandle = 0;
        }
        self->flags &= ~1u;

        WarpObject* first = func_800817BC__Q22cf13CfGameManagerFv(self->flagE7, 0);
        self->object7c = first;
        if (first != 0) {
            func_800ACF78(first, cf::CfGameManager::getPlayer(0), 0);
        }
        WarpObject* second = func_800817BC__Q22cf13CfGameManagerFv(self->flagE7, 0);
        self->object100 = second;
        if (second != 0) {
            func_800ACF78(second, cf::CfGameManager::getPlayer(1), 0);
        }
        WarpObject* third = func_800817BC__Q22cf13CfGameManagerFv(self->flagE7, 0);
        self->object104 = third;
        if (third != 0) {
            func_800ACF78(third, cf::CfGameManager::getPlayer(2), 0);
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
            self->soundHandle = func_80208C48(self->unkE8, &lbl_eu_805765A0);
        }
        self->flags |= 1;
    }
    func_8020A124(self, lbl_eu_806683D0);
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
        func_801BFED0(1, self->soundHandle, 0xa);
        self->soundHandle = 0;
    }
    /* retail stores state before rewriting the flag word */
    u32 flags = self->flags;
    self->state = 1;
    self->flags = flags & ~1u;
}

extern "C" void func_8020E3F0(WarpData* self) {
    bool allReady = true;
    for (int i = 0; i < 3; ++i) {
        WarpPlayer* player = playerFromTail((WarpPlayerTail*)cf::CfGameManager::getPlayer(i));
        if (player == 0) {
            continue;
        }
        WarpPlayerSubIf* sub = reinterpret_cast<WarpPlayerSubIf*>(player->subObject);
        u32 value = *sub->v30();
        if (func_80174C98(player, &value, 8) == 0) {
            value = *sub->v30();
            if (func_80174C98(player, &value, 7) != 0) {
                allReady = false;
                break;
            }
        } else {
            allReady = false;
            break;
        }
    }

    self->timer += func_80496288(lbl_eu_80663E14);
    if (self->timer < lbl_eu_806683D8 && (!allReady || self->timer < lbl_eu_806683D0)) {
        func_80209F5C();
        func_80209FB8();
        if (self->flagE7 != 0) {
            self->state = 5;
            preparePlayers(self);
            self->timer = lbl_eu_806683D0;
            func_8020A124(self, self->timer);
        } else {
            self->state = 2;
            self->timer = lbl_eu_806683D4;
        }
    } else {
        func_80209F5C();
        func_80209FB8();
    }
}

extern "C" void func_8020E6C0(WarpData* self) {
    func_80209F5C();
    func_80209FB8();
    if ((self->flags & 1) == 0) {
        self->state = 6;
    }
}

extern "C" void func_8020E704(WarpData* self) {
    func_80209F5C();
    func_80209FB8();
    if ((self->flags & 2) != 0) {
        self->timer -= func_80496288(lbl_eu_80663E14);
        if (self->timer > lbl_eu_806683C8) {
            return;
        }
        self->flags &= ~2u;
        func_8006CC4C();

        WarpPlayer* first = playerFromTail((WarpPlayerTail*)cf::CfGameManager::getPlayer(0));
        WarpVec3 centre;
        f32 distance;
        if (first != 0) {
            centre = self->position;
            distance = (f32)self->unkEA * lbl_eu_8066A210;
        } else {
            centre = self->destination;
            distance = (f32)self->unkEC * lbl_eu_8066A210;
        }

        /* Effect placement: nudge a copy of the centre point and query the
           effect manager before repositioning player 0. */
        WarpVec3 adjusted;
        adjusted.x = lbl_eu_806683C8;
        adjusted.y = lbl_eu_806683DC;
        adjusted.z = lbl_eu_806683C8;
        WarpVec3 sum;
        sum.x = centre.x + adjusted.x;
        sum.y = centre.y + adjusted.y;
        sum.z = centre.z + adjusted.z;
        WarpVec3 effectPos = sum;
        if (func_804BE398(&effectPos, 0, 0, 0, lbl_eu_806683E0, lbl_eu_8066AF20)) {
            func_804BE4B4(&centre, 0);
        }

        if (first != 0) {
            reinterpret_cast<WarpPlayerHeadIf*>(first)->va8(&centre);
            reinterpret_cast<WarpPlayerHeadIf*>(first)->vc8(distance);
        }

        for (int i = 1; i < 3; ++i) {
            WarpPlayer* player = playerFromTail((WarpPlayerTail*)cf::CfGameManager::getPlayer(i));
            if (player == 0) {
                continue;
            }
            WarpVec3 base = centre;
            WarpVec3 out68;
            func_80198710(&out68, &base, distance, 6, i,
                          lbl_eu_806683DC, lbl_eu_806683C8);
            WarpVec3 out4c;
            if (func_8019876C(&out68, &out4c) != 0) {
                reinterpret_cast<WarpPlayerHeadIf*>(player)->va8(&out4c);
                reinterpret_cast<WarpPlayerHeadIf*>(player)->vc8(distance);
                WarpObject* object = reinterpret_cast<WarpPlayerTailIf*>(playerTail(player))->v110();
                if (object != 0) {
                    func_80199810(object->field_8c, &centre);
                }
            }
        }

        WarpPlayerTailIf* manager = reinterpret_cast<WarpPlayerTailIf*>(func_800821F8__Q22cf13CfGameManagerFv());
        if (manager != 0) {
            manager->v0c(0);
        }
        self->state = 7;
        self->timer = lbl_eu_806683D0;
    } else {
        /* First entry: arm the warp timer and trigger the area transition. */
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
    func_80209F5C();
    func_80209FB8();
    if ((self->flags & 2) == 0) {
        self->timer -= func_80496288(lbl_eu_80663E14);
        if (self->timer > lbl_eu_806683CC) {
            return;
        }
        self->flags |= 4;
        func_8020A1DC(self);
        return;
    }

    self->timer -= func_80496288(lbl_eu_80663E14);
    if (self->timer > lbl_eu_806683D4) {
        return;
    }
    if ((self->flags & 0x10) != 0) {
        return;
    }
    if (self->flagE7 == 0) {
        return;
    }

    clearWarpObjects(self);
    if (self->soundHandle != 0) {
        func_801BFED0(1, self->soundHandle, 0xa);
        self->soundHandle = 0;
    }
    self->flags &= ~1u;
    preparePlayers(self);
    self->flags |= 8;

    for (int i = 0; i < 3; ++i) {
        WarpPlayer* player = playerFromTail((WarpPlayerTail*)cf::CfGameManager::getPlayer(i));
        if (player != 0) {
            reinterpret_cast<WarpPlayerHeadIf*>(player)->v168(lbl_eu_806683E8);
        }
    }

    if (self->timer <= lbl_eu_806683C8) {
        self->timer = lbl_eu_806683C8;
        if ((self->flags & 1) == 0) {
            self->state = 8;
            self->flags &= ~0x10u;
        }
    }
}

extern "C" void func_8020ED2C(WarpData* self) {
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

extern "C" void func_8020EE18(WarpData* self) {
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