#include "kyoshin/cf/CfGimmickJump.hpp"
#include "monolib/scn/CScnTimeApi.hpp"
#include "kyoshin/cf/object/CfObjectActor.hpp"
#include <nw4r/math.h>
#include "kyoshin/cf/CfGameManagerData.hpp"  // H3 label-owner decl (lbl_eu_80663E14; lbl_eu_80663E24)

// Warning string literals used by the jump pull-vector sanity check.
extern const char lbl_eu_80526324[];
extern const char lbl_eu_80526300[];

namespace cf {

extern "C" {
extern void* lbl_eu_80535A18[];
extern char lbl_eu_8050873C[];
extern char lbl_eu_805357E8[];
extern void* lbl_eu_80664138;
extern const f32 lbl_eu_80668400;
extern const f32 lbl_eu_80668404;
extern const f32 lbl_eu_80668408;
extern const f32 lbl_eu_8066840C;extern const f64 lbl_eu_80668410;
extern const f64 lbl_eu_80668418;
extern const f32 lbl_eu_80668420;
extern const f32 lbl_eu_80668424;
extern const f32 lbl_eu_80668428;
extern const f32 lbl_eu_8066842C;
extern const f32 lbl_eu_80668430;
extern const f32 lbl_eu_8066A1F8;
extern const f32 lbl_eu_8066A1FC;
extern const f32 lbl_eu_8066A210;

extern void __ct__cf_CfGimmick(void* self);
extern void __dt__Q22cf9CfGimmickFv(void* self, int deleting);
extern void func_80208EE4(void* self);
extern void func_8020A434(void* value);
extern void func_802089BC(void* destination, void* position, void* rotation);
extern void func_80208F34(void* self, void* destination, void* bdat, void* table);
extern void func_80209020(void* self, void* destination, void* bdat, void* table);
extern void func_8020915C(void* self, void* destination, void* bdat, void* table);
extern void func_80209288(void* self, void* destination, void* bdat, void* table);
extern void func_8020A6B0(void* effect, void* position, u16 resourceId, f32 scale,
                          int arg5, int arg6);
extern void func_8020F540(CfGimmickJump* self);
extern void func_8020A484(u16 resourceId);
extern int func_8020A5DC(void* self);
extern int func_8020A87C(void* self, void* effect);
extern int func_8020971C(u16 duration);
extern int func_80209754(u8 flags, void* first, void* second, void* third,
                         void* effect);
extern void func_80209F5C();
extern void* func_8003AA34();
extern int getBdatStringColumnValue(void* bdat, const char* column, u16 row);
extern void* func_800817BC__Q22cf13CfGameManagerFv(u16 id, int mode);
extern int func_801BFAE4(u16 handle);
extern void func_801BFAE8(u16 handle, void* position);
extern void func_801BFED0(int kind, u16 handle, int mode);
extern u16 func_80208C60(u16 effectId, void* position, f32 distance);
extern void func_8004B840(void* target, f32 amount);
extern f32 FrSqrt__Q24nw4r4mathFf(f32 value);
extern void Warning__Q24nw4r2dbFPCciPCce(const char* file, int line,
                                          const char* format, ...);
extern void __dl__FPv(void* value);
}

/* Cast-only interfaces.  Omitting the RTTI entries keeps these calls at their
 * retail vtable offsets while leaving the implementation in normal C++. */
struct JumpMoveIf {
    virtual void _v008();
    virtual void _v00C();
    virtual void _v010();
    virtual void _v014();
    virtual void _v018();
    virtual void _v01C();
    virtual void _v020();
    virtual void _v024();
    virtual void _v028();
    virtual void _v02C();
    virtual void _v030();
    virtual void _v034();
    virtual void _v038();
    virtual void _v03C();
    virtual void _v040();
    virtual void _v044();
    virtual void _v048();
    virtual void _v04C();
    virtual void _v050();
    virtual void _v054();
    virtual void _v058();
    virtual void _v05C();
    virtual void _v060();
    virtual void _v064();
    virtual void _v068();
    virtual void _v06C();
    virtual void _v070();
    virtual void _v074();
    virtual void _v078();
    virtual void _v07C();
    virtual void _v080();
    virtual void _v084();
    virtual void _v088();
    virtual void _v08C();
    virtual void _v090();
    virtual void _v094();
    virtual void _v098();
    virtual void setPosition(const CfGimmickJumpVec3& position);
    virtual void _v0A0();
    virtual void _v0A4();
    virtual void _v0A8();
    virtual const CfGimmickJumpVec3* getPosition();
    virtual void _v0B0();
    virtual void _v0B4();
    virtual void _v0B8();
    virtual void _v0BC();
    virtual void _v0C0();
    virtual void _v0C4();
    virtual void updateDirection();
};

struct JumpLinkedObjectIf {
    virtual void _v008();
    virtual void _v00C();
    virtual void _v010();
    virtual void _v014();
    virtual void _v018();
    virtual void _v01C();
    virtual void _v020();
    virtual void _v024();
    virtual void _v028();
    virtual void _v02C();
    virtual void _v030();
    virtual void _v034();
    virtual void _v038();
    virtual void _v03C();
    virtual void _v040();
    virtual void _v044();
    virtual void _v048();
    virtual void _v04C();
    virtual void _v050();
    virtual void _v054();
    virtual void _v058();
    virtual void _v05C();
    virtual void _v060();
    virtual void _v064();
    virtual void _v068();
    virtual void _v06C();
    virtual void _v070();
    virtual void _v074();
    virtual void _v078();
    virtual void _v07C();
    virtual void _v080();
    virtual void _v084();
    virtual void _v088();
    virtual void _v08C();
    virtual void _v090();
    virtual void _v094();
    virtual void _v098();
    virtual void setPosition(const CfGimmickJumpVec3& position);
    virtual void _v0A0();
    virtual void _v0A4();
    virtual void _v0A8();
    virtual void _v0AC();
    virtual void _v0B0();
    virtual void _v0B4();
    virtual void _v0B8();
    virtual void _v0BC();
    virtual void _v0C0();
    virtual void setHeight(f32 height);
};

struct JumpDispatchTarget {};
typedef int (JumpDispatchTarget::*JumpDispatchFn)(const CfGimmickJumpVec3*,
                                                  const CfGimmickJumpVec3*);
typedef void (JumpDispatchTarget::*JumpStateFn)();

extern "C" {
extern void* jumptable_eu_80535830[];
extern void* lbl_eu_805359E8[];
}

struct JumpActorData {
    u8 padding[0x3F60];
    void* target;
};

struct JumpPlayerFieldC4 {
    u8 padding[0xC4];
    void* field_C4;
};

struct JumpTargetData {
    u8 padding0[0x0C];
    u32 flags0C;
    u8 padding10[0x4EC - 0x10];
    u32 flags4EC;
};

static inline void* jumpActorFromPlayer(void* player) {
    if (player == 0) {
        return 0;
    }
    return (u8*)player - 0x3E9C;
}

static inline const CfGimmickJumpVec3* jumpPlayerPosition(int index) {
    void* player = getPlayer__Q22cf13CfGameManagerFi(index);
    if (player == 0) {
        return 0;
    }
    return reinterpret_cast<JumpMoveIf*>(player)->getPosition();
}

static inline f32 jumpPlayerAngle(void* actor) {
    CfObjectActor* obj = reinterpret_cast<CfObjectActor*>(actor);
    return obj->CfObjectActor_UnkVirtualFunc6();
}

static inline f32 normalizeJumpAngle(f32 angle) {
    // Operand order matters: retail compares (PI, angle).
    while (lbl_eu_8066A1F8 <= angle) {
        angle -= lbl_eu_8066A1FC;
    }
    while (angle < -lbl_eu_8066A1F8) {
        angle += lbl_eu_8066A1FC;
    }
    return angle;
}

extern "C" CfGimmickJump* __ct__cf_CfGimmickJump(CfGimmickJump* self,
                                                  u16 row) {
    __ct__cf_CfGimmick(self);
    *(void**)self = lbl_eu_80535A18;
    self->gimmickType = 5;

    void* bdat = func_8003AA34();
    void* table = lbl_eu_80664138;
    self->bdatRow = row;

    func_80208F34(self, &self->position, bdat, &table);
    func_80209020(self, &self->initialState, bdat, &table);
    func_80209288(self, &self->rotation, bdat, &table);
    func_8020915C(self, &self->transformedPosition, bdat, &table);

    char* columns = lbl_eu_8050873C;
    u32 value = (u32)getBdatStringColumnValue(table, columns, row);
    self->targetX = lbl_eu_80668400 * (f32)value;
    value = (u32)getBdatStringColumnValue(table, columns + 7, row);
    self->targetY = lbl_eu_80668400 * (f32)value;
    value = (u32)getBdatStringColumnValue(table, columns + 0x0E, row);
    self->targetZ = lbl_eu_80668400 * (f32)value;

    value = (u32)getBdatStringColumnValue(table, columns + 0x15, row);
    self->targetAngle = lbl_eu_8066A210 * (f32)(s16)value;
    if (self->targetX == lbl_eu_80668404 && self->targetY == lbl_eu_80668404 &&
        self->targetZ == lbl_eu_80668404) {
        self->targetX = self->position.x;
        self->targetY = self->position.y;
        self->targetZ = self->position.z;
    } else {
        self->flags |= 0x1000;
    }

    value = (u32)getBdatStringColumnValue(table, columns + 0x1C, row);
    self->flags66 = (u8)value;
    value = (u32)getBdatStringColumnValue(table, lbl_eu_805357E8 + 0x30, row);
    self->resourceId = (u16)value;
    value = (u32)getBdatStringColumnValue(table, columns + 0x24, row);
    self->duration = (u16)value;
    value = (u32)getBdatStringColumnValue(table, columns + 0x29, row);
    self->effectFlags = (u8)value;
    value = (u32)getBdatStringColumnValue(table, columns + 0x2E, row);
    self->effectId = (u16)value;
    value = (u32)getBdatStringColumnValue(table, columns + 0x34, row);
    self->waitFrames = (u8)value;
    value = (u32)getBdatStringColumnValue(table, columns + 0x39, row);
    self->jumpFrames = (u8)value;
    value = (u32)getBdatStringColumnValue(table, columns + 0x3F, row);
    self->jumpFlags = (u8)value;

    value = (u32)getBdatStringColumnValue(table, columns + 0x46, row);
    self->speed = lbl_eu_80668408 * (f32)(u8)value;
    value = (u32)getBdatStringColumnValue(table, columns + 0x4A, row);
    self->height = (f32)(u16)value;
    value = (u32)getBdatStringColumnValue(table, columns + 0x4A, row);
    self->frameScale = lbl_eu_80668408 * (f32)(u8)value;
    if (self->frameScale != lbl_eu_80668404) {
        self->maxFrameScale = self->height / self->frameScale;
    } else {
        self->maxFrameScale = lbl_eu_80668404;
    }

    value = (u32)getBdatStringColumnValue(table, columns + 0x4F, row);
    f32 minimum = lbl_eu_80668408 * (f32)(u8)value;
    if (self->frameScale < minimum) {
        self->frameScale = minimum;
    } else {
        self->frameScale += lbl_eu_8066840C;
    }

    self->timer = lbl_eu_80668404;
    self->motionState = 0;
    return self;
}

CfGimmickJump::~CfGimmickJump() {
    *(void**)this = lbl_eu_80535A18;
    func_80208EE4(this);
    func_8020A434(&effect);
    __dt__Q22cf9CfGimmickFv(this, 0);
}

extern "C" void func_8020F38C(CfGimmickJump* self) {
    self->savedState = self->initialState;
    func_802089BC(&self->initialState, &self->position, &self->rotation);
    *(f32*)&self->savedState.words[0x0D] = self->height;
    func_802089BC(&self->savedState, &self->position, &self->rotation);
    func_802089BC(&self->transformedPosition, &self->position, &self->rotation);
}

extern "C" void func_8020F484(CfGimmickJump* self) {
    if ((lbl_eu_80663E24 & 0x4000000) == 0) {
        self->timer += func_80496288(lbl_eu_80663E14);
        // State handler dispatch: retail stores 12-byte member-function
        // pointers (__ptmf_scall) indexed by motionState.
        JumpStateFn* stateTable = reinterpret_cast<JumpStateFn*>((void*)lbl_eu_805359E8);
        JumpStateFn& stateFn = stateTable[self->motionState];
        (reinterpret_cast<JumpDispatchTarget*>(self)->*stateFn)();
        func_8020F540(self);
    }

    if ((self->flags & 0x400) != 0) {
        if ((self->flags66 & 1) != 0) {
            func_8020A6B0(&self->effect, &self->position, self->resourceId,
                          lbl_eu_80668420, 0, 0);
        }
        self->flags &= ~0x400u;
    } else {
        func_8020A434(&self->effect);
    }
}

extern "C" void func_8020F540(CfGimmickJump* self) {
    if ((self->flags & 0x380) == 0) {
        return;
    }

    // Loop-invariant constants; declaration order mirrors the retail pool
    // loads (MWCC keeps each in its own non-volatile FPR).
    const f32 twoPi = lbl_eu_8066A1FC;
    const f32 pi = lbl_eu_8066A1F8;
    const f32 speedScale = lbl_eu_80668424;
    const f32 turnLimit = lbl_eu_80668428;
    const f32 turnLimitNeg = lbl_eu_8066842C;
    const f32 nearDistSq = lbl_eu_8066840C;

    for (int index = 0; index < 3; ++index) {
        u32 bit = 0x80u << index;
        if ((self->flags & bit) == 0) {
            continue;
        }

        if (index == 0) {
            func_80209F5C();
        }

        void* player = getPlayer__Q22cf13CfGameManagerFi(index);
        void* actor = jumpActorFromPlayer(player);
        // Every failure path below still clears the request bit.
        if (actor == 0) {
            self->flags &= ~bit;
            continue;
        }
        JumpTargetData* target = reinterpret_cast<JumpTargetData*>(
            reinterpret_cast<JumpActorData*>(actor)->target);
        if (target == 0 || (target->flags0C & 2) != 0 ||
            (target->flags4EC & 2) != 0) {
            self->flags &= ~bit;
            continue;
        }

        // Mark the move target as jump-claimed.
        target->flags4EC = (target->flags4EC & ~0xA0000u) | 0x40000u;
        f32 step = speedScale * func_80496288(lbl_eu_80663E14);
        f32 delta = step * self->playerDeltaX[index];
        f32 deltaZ = step * self->playerDeltaZ[index];

        // Retail re-queries the position getter once per component (no CSE
        // across the virtual call), z first.
        JumpMoveIf* move = reinterpret_cast<JumpMoveIf*>(player);
        CfGimmickJumpVec3 result;
        result.z = deltaZ + move->getPosition()->z;
        result.y = move->getPosition()->y;
        result.x = delta + move->getPosition()->x;

        f32 dx = result.x - self->targetX;
        f32 dz = result.z - self->targetZ;
        if (dx * dx + dz * dz < nearDistSq) {
            self->flags &= ~bit;
            continue;
        }

        move->setPosition(result);
        f32 angle = jumpPlayerAngle(actor);
        if (self->targetAngle == angle) {
            self->flags &= ~bit;
            continue;
        }

        // Turn toward the target angle; the clamped result itself is unused
        // (the direction update happens inside the virtual calls above).
        f32 difference = normalizeJumpAngle(self->targetAngle - jumpPlayerAngle(actor));
        if (difference > turnLimit) {
            difference = normalizeJumpAngle(turnLimit + jumpPlayerAngle(actor));
        } else if (difference < turnLimitNeg) {
            difference = normalizeJumpAngle(jumpPlayerAngle(actor) - turnLimit);
        } else {
            difference = normalizeJumpAngle(self->targetAngle);
        }
        (void)difference;

        self->flags &= ~bit;
    }
}

extern "C" void func_8020F8C4(CfGimmickJump* self) {
    self->timer = lbl_eu_80668404;
    if ((self->flags66 & 1) != 0) {
        if ((self->flags & 0x800) != 0) {
            if (func_8020A5DC(self) != 0) {
                return;
            }
            self->flags &= ~0x800u;
        } else if (func_8020A87C(self, self->effect) != 0) {
            func_8020A484(self->resourceId);
            self->flags |= 0x800;
            return;
        }
    }

    self->flags |= 0x400;
    if (self->duration == 0 || func_8020971C(self->duration) != 0) {
        self->motionState = 1;
        self->timer = lbl_eu_80668404;
    }
}

extern "C" void func_8020F984(CfGimmickJump* self) {
    void* player = getPlayer__Q22cf13CfGameManagerFi(0);
    if (player == 0) {
        return;
    }

    const CfGimmickJumpVec3* playerPosition = jumpPlayerPosition(0);
    if (playerPosition == 0) {
        return;
    }

    typedef int (*JumpDispatchFunction)(void*, const CfGimmickJumpVec3*,
                                        const CfGimmickJumpVec3*);
    JumpDispatchFunction dispatch = reinterpret_cast<JumpDispatchFunction>(
        jumptable_eu_80535830[self->state]);
    int result = dispatch(&self->transformedPosition, playerPosition,
                          &self->position);
    if (result == 0) {
        self->timer = lbl_eu_80668404;
        self->flags &= ~0x2000u;
        return;
    }

    if ((self->flags & 0x2000) == 0) {
        if (self->timer < self->speed) {
            return;
        }
        self->flags |= 0x2000;
        self->timer = lbl_eu_80668404;
    }

    if ((self->flags & 1) == 0) {
        self->flags |= 0x400;
        if ((self->flags66 & 8) == 0 &&
            func_80209754((u8)self->flags66, &self->initialState,
                          &self->position, &self->rotation, self->effect) == 0) {
            self->timer = lbl_eu_80668404;
            return;
        }
        self->flags |= 1;
    }

    if (self->timer < (f32)self->jumpFrames * lbl_eu_80668420) {
        return;
    }

    self->timer = (f32)self->jumpFrames * lbl_eu_80668420;
    self->linkedObject = func_800817BC__Q22cf13CfGameManagerFv(self->jumpFrames, 0);
    if (self->linkedObject == 0) {
        return;
    }

    *(void**)((u8*)self->linkedObject + 0xB0) = self;
    JumpLinkedObjectIf* linked = reinterpret_cast<JumpLinkedObjectIf*>(self->linkedObject);
    if ((self->jumpFlags & 0x10) != 0) {
        linked->setPosition(self->position);
        linked->setHeight(self->position.y);
    }

    if (self->soundHandle != 0) {
        func_801BFED0(1, self->soundHandle, 10);
    }

    if (self->effectId != 0) {
        CfGimmickJumpVec3 position = self->position;
        const CfGimmickJumpVec3* playerPos = jumpPlayerPosition(0);
        if (playerPos != 0) {
            position.y = playerPos->y;
        }
        if (position.y > self->position.y + self->height) {
            position.y = self->position.y + self->height;
        } else if (position.y < self->position.y) {
            position.y = self->position.y;
        }
        self->soundHandle = func_80208C60(self->effectId, &position,
                                           lbl_eu_80668430);
    }

    self->motionState = 2;
    self->timer = lbl_eu_80668404;
    self->flags &= ~1u;
}

// Retail converts the frame count via the shared .sdata2 2^52 magic
// (retail pool doubles lbl_eu_80668410 = 0x4330000080000000 and
// lbl_eu_80668418 = 0x4330000000000000 ship from the split1 shared data
// object; the int-to-float conversion sequences reference them directly.)

static f32 jumpFrameThreshold(CfGimmickJump* self) {
    return (f32)(self->jumpFrames * 30 + self->waitFrames);
}

extern "C" void func_8020FC14(CfGimmickJump* self) {
    CfGimmickJumpVec3 position;
    if (self->soundHandle != 0) {
        if (func_801BFAE4(self->soundHandle) != 0) {
            position.x = self->position.x;
            position.y = self->position.y;
            position.z = self->position.z;
            void* player = getPlayer__Q22cf13CfGameManagerFi(0);
            if (player != 0) {
                position.y =
                    reinterpret_cast<JumpMoveIf*>(player)->getPosition()->y;
            }
            if (position.y > self->height + self->position.y) {
                position.y = self->height + self->position.y;
            } else if (position.y < self->position.y) {
                position.y = self->position.y;
            }
            func_801BFAE8(self->soundHandle, &position);
        } else {
            self->soundHandle = 0;
        }
    }

    if (self->timer >= jumpFrameThreshold(self)) {
        self->motionState = 3;
        self->timer = lbl_eu_80668404;
        self->verticalOffset = lbl_eu_80668404;
    }
}

extern "C" void func_8020FD2C(CfGimmickJump* self) {
    bool finished = false;
    if (self->soundHandle != 0) {
        if (func_801BFAE4(self->soundHandle) != 0) {
            CfGimmickJumpVec3 position = self->position;
            void* player = getPlayer__Q22cf13CfGameManagerFi(0);
            if (player != 0) {
                position.y =
                    reinterpret_cast<JumpMoveIf*>(player)->getPosition()->y;
            }
            if (position.y > self->position.y + self->height) {
                position.y = self->position.y + self->height;
            } else if (position.y < self->position.y) {
                position.y = self->position.y;
            }
            func_801BFAE8(self->soundHandle, &position);
        } else {
            self->soundHandle = 0;
        }
    }

    f32 elapsed = self->frameScale * func_80496288(lbl_eu_80663E14);
    if (self->timer <= self->frameScale) {
        self->verticalOffset += elapsed;
        if (self->verticalOffset > self->height) {
            self->verticalOffset = self->height;
        }

        // Lift players standing on the dispatch area once they are below the
        // rising platform surface.
        for (int index = 0; index < 3; ++index) {
            u32 stateBit = 2u << index;
            if ((self->flags & stateBit) == 0) {
                continue;
            }
            u32 moveBit = 0x10u << index;
            if ((self->flags & moveBit) != 0) {
                continue;
            }
            void* player = getPlayer__Q22cf13CfGameManagerFi(index);
            void* actor = jumpActorFromPlayer(player);
            if (actor == 0) {
                continue;
            }
            JumpTargetData* target = reinterpret_cast<JumpTargetData*>(
                reinterpret_cast<JumpActorData*>(actor)->target);
            if (target == 0) {
                continue;
            }

            JumpMoveIf* move = reinterpret_cast<JumpMoveIf*>(player);
            typedef int (*JumpDispatchFunction)(const CfGimmickJumpVec3*,
                                                const CfGimmickJumpVec3*,
                                                const CfGimmickJumpVec3*);
            const CfGimmickJumpVec3* playerPos = move->getPosition();
            JumpDispatchFunction dispatch = reinterpret_cast<JumpDispatchFunction>(
                jumptable_eu_80535830[self->savedState.words[8]]);
            if (dispatch(&self->transformedPosition, playerPos,
                         &self->position) == 0) {
                continue;
            }
            playerPos = move->getPosition();
            if (playerPos->y <= self->verticalOffset + self->position.y) {
                self->flags |= moveBit;
                self->playerHeight[index] = move->getPosition()->y;
            }
        }
    } else if (self->linkedObject == 0) {
        finished = true;
    }

    self->timer += elapsed;

    const f32 zero = lbl_eu_80668404;
    const f32 one = lbl_eu_8066840C;
    for (int index = 0; index < 3; ++index) {
        u32 stateBit = 2u << index;
        u32 moveBit = 0x10u << index;
        if ((self->flags & stateBit) == 0 || (self->flags & moveBit) != 0) {
            continue;
        }

        finished = false;
        void* player = getPlayer__Q22cf13CfGameManagerFi(index);
        void* actor = jumpActorFromPlayer(player);
        if (actor == 0) {
            continue;
        }

        self->playerHeight[index] += elapsed;
        JumpTargetData* target = reinterpret_cast<JumpTargetData*>(
            reinterpret_cast<JumpActorData*>(actor)->target);
        if (target == 0) {
            continue;
        }

        // Carry the player with the platform; the getter is re-queried per
        // component (no CSE across the virtual call).
        JumpMoveIf* move = reinterpret_cast<JumpMoveIf*>(player);
        CfGimmickJumpVec3 position;
        position.z = move->getPosition()->z;
        f32 carried = self->playerHeight[index];
        position.x = move->getPosition()->x;
        position.y = carried;
        move->setPosition(position);
        func_8004B840(target, zero);
        target->flags4EC |= 0x4000000;
        if (index == 0) {
            func_80209F5C();
        }

        if (self->playerHeight[index] <= self->height + self->position.y) {
            self->flags |= moveBit;
            target->flags4EC &= ~0x20u;
            if ((self->flags & 0x1000) != 0) {
                // Recompute the pull vector toward the jump target.
                self->flags |= 0x80u << index;
                const CfGimmickJumpVec3* p = move->getPosition();
                self->playerDeltaX[index] = self->targetX - p->x;
                p = move->getPosition();
                self->playerDeltaZ[index] = self->targetZ - p->z;
                f32 dx = self->playerDeltaX[index];
                f32 dz = self->playerDeltaZ[index];
                f32 distSq = dx * dx + dz * dz;
                if (distSq < zero) {
                    nw4r::db::Warning(lbl_eu_80526324, 0x273, lbl_eu_80526300);
                }
                f32 dist = zero;
                if (distSq > zero) {
                    dist = distSq * nw4r::math::FrSqrt(distSq);
                }
                if (dist != zero) {
                    f32 inv = one / dist;
                    self->playerDeltaX[index] = dx * inv;
                    self->playerDeltaZ[index] = dz * inv;
                } else {
                    self->playerDeltaX[index] = zero;
                    self->playerDeltaZ[index] = zero;
                }
            }
        } else {
            target->flags4EC |= 0x4000000;
        }
    }

    if (finished) {
        self->motionState = 1;
        self->timer = lbl_eu_80668404;
        if (self->soundHandle != 0) {
            func_801BFED0(1, self->soundHandle, 10);
            self->soundHandle = 0;
        }
        for (int index = 0; index < 3; ++index) {
            u32 stateBit = 2u << index;
            u32 moveBit = 0x10u << index;
            if ((self->flags & stateBit) != 0 && (self->flags & moveBit) == 0) {
                void* player = getPlayer__Q22cf13CfGameManagerFi(index);
                if (player != 0) {
                    JumpPlayerFieldC4* entry =
                        reinterpret_cast<JumpPlayerFieldC4*>(player);
                    if (entry->field_C4 != 0) {
                        reinterpret_cast<JumpTargetData*>(entry->field_C4)
                            ->flags4EC &= ~0x20u;
                    }
                }
            }
        }
        // Keep only the low flag bits plus the top bit.
        self->flags &= ~0x7E000000u;
    }
}

} // namespace cf