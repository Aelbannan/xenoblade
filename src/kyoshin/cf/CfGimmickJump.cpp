#include "kyoshin/cf/CfGimmickJump.hpp"
extern char lbl_eu_80508728[];
#include "monolib/scn/CScnTimeApi.hpp"
#include "kyoshin/cf/object/CfObjectActor.hpp"
#include <nw4r/math.h>
#include "kyoshin/cf/CfGameManagerData.hpp"

struct __data_CfGimmickJump {
    unsigned char a[0x24];
    unsigned char b[0x14];
    unsigned char c[0x30];
};
__declspec(section ".data") __attribute__((used, aligned(8))) const __data_CfGimmickJump __data_CfGimmickJump_blob = {
    {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
    {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
    {0x00,0x00,0x00,0x00,0xFF,0xFF,0xFF,0xFF,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFF,0xFF,0xFF,0xFF,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFF,0xFF,0xFF,0xFF,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFF,0xFF,0xFF,0xFF,0x00,0x00,0x00,0x00}
};
#define lbl_eu_805359B0 ((char*)&__data_CfGimmickJump_blob.a)
#define lbl_eu_805359D4 ((char*)&__data_CfGimmickJump_blob.b)
#define lbl_eu_805359E8 ((char*)&__data_CfGimmickJump_blob.c)
__declspec(section ".rodata") __attribute__((aligned(8), used)) const unsigned char __absorb_kyoshin_cf_CfGimmickJump_rodata[112] = {
    0x63, 0x66, 0x3a, 0x3a, 0x43, 0x66, 0x47, 0x69, 0x6d, 0x6d, 0x69, 0x63,
    0x6b, 0x4a, 0x75, 0x6d, 0x70, 0x00, 0x00, 0x00, 0x6c, 0x6e, 0x50, 0x6f,
    0x73, 0x58, 0x00, 0x6c, 0x6e, 0x50, 0x6f, 0x73, 0x59, 0x00, 0x6c, 0x6e,
    0x50, 0x6f, 0x73, 0x5a, 0x00, 0x6c, 0x6e, 0x52, 0x6f, 0x74, 0x59, 0x00,
    0x72, 0x65, 0x63, 0x74, 0x79, 0x70, 0x65, 0x00, 0x6a, 0x70, 0x45, 0x46,
    0x00, 0x6a, 0x70, 0x53, 0x45, 0x00, 0x6a, 0x75, 0x6d, 0x70, 0x53, 0x00,
    0x77, 0x61, 0x69, 0x74, 0x00, 0x45, 0x46, 0x41, 0x54, 0x52, 0x00, 0x72,
    0x6b, 0x57, 0x61, 0x69, 0x74, 0x00, 0x74, 0x6f, 0x70, 0x00, 0x74, 0x69,
    0x6d, 0x65, 0x00, 0x65, 0x78, 0x54, 0x69, 0x6d, 0x65, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00
};
__declspec(section ".sdata2") __attribute__((aligned(8), used)) const unsigned char __absorb_kyoshin_cf_CfGimmickJump_sdata2[56] = {
    0x3c, 0x23, 0xd7, 0x0a, 0x00, 0x00, 0x00, 0x00, 0x41, 0xf0, 0x00, 0x00,
    0x3f, 0x80, 0x00, 0x00, 0x43, 0x30, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00,
    0x43, 0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0xa0, 0x00, 0x00,
    0x3e, 0x99, 0x99, 0x9a, 0x3d, 0xcc, 0xcc, 0xcd, 0xbd, 0xcc, 0xcc, 0xcd,
    0x43, 0x16, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};



static float speedScale = 1.0f;
static float nearDistSq = 1.0f;  // H3 label-owner decl (lbl_eu_80663E14; lbl_eu_80663E24)

// Warning string literals used by the jump pull-vector sanity check.
extern char lbl_eu_80526324[];
extern char lbl_eu_80526300[];

namespace cf {

extern "C" {
extern char lbl_eu_80535A18[];
extern char lbl_eu_8050873C[];
extern char lbl_eu_805357E8[];
extern char lbl_eu_80664138[];
extern const float lbl_eu_80668400;
extern const float lbl_eu_80668404;
extern const float lbl_eu_80668408;
extern const float lbl_eu_8066840C;extern const float lbl_eu_80668410;
extern const float lbl_eu_80668418;
extern const float lbl_eu_80668420;
extern const float lbl_eu_80668424;
extern const float lbl_eu_80668428;
extern const float lbl_eu_8066842C;
extern const float lbl_eu_80668430;
extern const float lbl_eu_8066A1F8;
extern const float lbl_eu_8066A1FC;
extern const float lbl_eu_8066A210;

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
extern int func_80209754(u16 flags, void* first, void* second, void* third,
                         void* effect);
extern void func_80209F5C();
extern void* func_8003AA34();
extern int getBdatStringColumnValue(void* bdat, const char* column, u16 row);
extern CfGimmickWork* createBattleActor__Q22cf13CfGameManagerFv(u16 id, int mode);
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

/*
 * Dispatched-object identities (US split1.s tables):
 * - State table lbl_eu_805359E8: four 12-byte member-function pointers over
 *   cf::CfGimmickJump itself (motion states 1-4: func_8020F8C4 / F984 /
 *   FC14 / FD2C).
 * - Player object: the cf::CfObjectMove base embedded at
 *   cf::CfObjectActor+0x3E9C (vtable lbl_eu_80529690, the cf-chain table:
 *   CObjectState .. CObjectParam .. CfObject .. CfObjectModel ..
 *   CfObjectMove). Slots dispatched here: +0x9C CfObject_UnkVirtualFunc19
 *   (hidden r4 = position vector), +0xAC CfObject_UnkVirtualFunc23 (returns
 *   the position vector), +0xC8 CfObject_UnkVirtualFunc30 (direction
 *   update). The +0xAC/+0xC8 calls go through the real cf::CfObject
 *   declarations; +0x9C still needs the minimal view below because the hot
 *   shared header declares that slot no-arg.
 * - Linked object (createBattleActor work object, back-pointer at +0xB0): owning
 *   class not yet recovered anywhere in the tree (every gimmick TU keeps a
 *   raw view: CfGimmickItemMgr / WarpObject / ...), so its +0x9C/+0xC4
 *   dispatches stay on the minimal CfGimmickJumpMgr view.
 */

// Member-function-pointer state table over the real owning class.
typedef void (CfGimmickJump::*JumpStateFn)();

// Minimal view of the actor's embedded CfObjectMove base, used only for
// vtable slot +0x9C (CfObject_UnkVirtualFunc19): retail passes the position
// vector as a hidden r4, but CfObject.hpp (hot header, not owned here) still
// declares that slot no-arg, so the call cannot go through cf::CfObject yet.
class CfObjectActorMoveVt9C {
    void* vtable;
};

// CfGimmickWork is defined in CfGimmickJump.hpp (hot-header fix for the
// createBattleActor work object). No local view needed here.

extern "C" {
extern void* jumptable_eu_80535830[];
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

static inline cf::CfObjectActor* jumpActorFromPlayer(void* player) {
    return (player == 0)
               ? 0
               : reinterpret_cast<cf::CfObjectActor*>((u8*)player - 0x3E9C);
}

static inline const CfGimmickJumpVec3* jumpPlayerPosition(int index) {
    cf::CfObjectActor* actor =
        jumpActorFromPlayer(getPlayer__Q22cf13CfGameManagerFi(index));
    if (actor == 0) {
        return 0;
    }
    // CfObject vtable +0xAC: returns the live position vector.
    return reinterpret_cast<const CfGimmickJumpVec3*>(
        actor->CfObject_UnkVirtualFunc23());
}

static inline f32 jumpPlayerAngle(cf::CfObjectActor* actor) {
    return actor->CfObjectActor_UnkVirtualFunc6();
}

static inline f32 normalizeJumpAngle(f32 angle) {
    // Operand order matters: retail compares (PI, angle).
    while (*(const float*)&lbl_eu_8066A1F8 <= angle) {
        angle -= *(const float*)&lbl_eu_8066A1FC;
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
    value = (u32)getBdatStringColumnValue(table, columns + 7, row);
    value = (u32)getBdatStringColumnValue(table, columns + 0x0E, row);

    value = (u32)getBdatStringColumnValue(table, columns + 0x15, row);
    if (value != 0) {
        self->targetAngle = lbl_eu_8066A210 * (f32)(s16)value;
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
    value = (u32)getBdatStringColumnValue(table, columns + 0x4A, row);
    self->height = (f32)(u16)value;
    value = (u32)getBdatStringColumnValue(table, columns + 0x4A, row);
    if (value != 0) {
        self->maxFrameScale = self->height / self->frameScale;
    } else {
    }

    value = (u32)getBdatStringColumnValue(table, columns + 0x4F, row);
    if (self->frameScale < lbl_eu_8066A1F8) {
        self->frameScale = lbl_eu_8066A1F8;
    } else {
    }

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
    func_802089BC(&self->savedState, &self->position, &self->rotation);
    func_802089BC(&self->transformedPosition, &self->position, &self->rotation);
}

extern "C" void func_8020F484(CfGimmickJump* self) {
    if ((lbl_eu_80663E24 & 0x4000000) == 0) {
        self->timer += func_80496288(lbl_eu_80663E14);
        // State handler dispatch: retail stores 12-byte member-function
        // pointers over CfGimmickJump (__ptmf_scall) indexed by motionState.
        JumpStateFn* stateTable = reinterpret_cast<JumpStateFn*>((void*)lbl_eu_805359E8);
        JumpStateFn& stateFn = stateTable[self->motionState];
        (self->*stateFn)();
        func_8020F540(self);
    }

    if ((self->flags & 0x400) != 0) {
        if ((self->flags66 & 1) != 0) {
            func_8020A6B0(&self->effect, &self->position, self->resourceId, 1.0f, 0, 0);
        }
        self->flags &= ~0x400u;
    } else {
        func_8020A434(&self->effect);
    }
}

extern "C" void func_8020F540(CfGimmickJump* self) {
    return;
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
    }
}

extern "C" void func_8020F984(CfGimmickJump* self) {
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
        self->flags &= ~0x2000u;
        return;
    }

    if ((self->flags & 0x2000) == 0) {
        if (self->timer < self->speed) {
            return;
        }
        self->flags |= 0x2000;
    }

    if ((self->flags & 1) == 0) {
        self->flags |= 0x400;
        if ((self->flags66 & 8) == 0 &&
            func_80209754(self->flags66, &self->initialState,
                          &self->position, &self->rotation, self->effect) == 0) {
            return;
        }
        self->flags |= 1;
    }

    if (self->timer < (f32)(self->jumpFrames * 30)) {
        return;
    }

    // Retail reuses the +0x88 flag byte for both the timer reload and the
    // work-object id (converted through the shared 0x43300000 int->float
    // magic, hence the plain integer source forms here).
    self->timer = (f32)self->effectFlags;
    self->linkedObject = createBattleActor__Q22cf13CfGameManagerFv(self->effectFlags, 0);
    if (self->linkedObject == 0) {
        return;
    }

    self->linkedObject->field_B0 = self;
    if ((self->jumpFlags & 0x10) != 0) {
        self->linkedObject->CfObject_UnkVirtualFunc19(
            reinterpret_cast<const ml::CVec3*>(&self->position));
        self->linkedObject->CfObject_UnkVirtualFunc29(self->rotation.y);
    }

    if (self->soundHandle != 0) {
        func_801BFED0(1, self->soundHandle, 10);
    }

    if (self->effectId != 0) {
        CfGimmickJumpVec3 position = self->position;
        void* player = getPlayer__Q22cf13CfGameManagerFi(0);
        if (player != 0) {
            position.y =
                reinterpret_cast<const CfGimmickJumpVec3*>(
                    static_cast<cf::CfObjectMove*>(player)
                        ->CfObject_UnkVirtualFunc23())
                    ->y;
        }
        if (position.y > self->position.y + self->height) {
            position.y = self->position.y + self->height;
        } else if (position.y < self->position.y) {
            position.y = self->position.y;
        }
        self->soundHandle = func_80208C60(self->effectId, &position, 1.0f);
    }

    self->motionState = 2;
    self->flags &= ~1u;
}

// Retail converts the frame count via the shared .sdata2 2^52 magic
// object; the int-to-float conversion sequences reference them directly.)

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
                    reinterpret_cast<const CfGimmickJumpVec3*>(
                        static_cast<cf::CfObjectMove*>(player)
                            ->CfObject_UnkVirtualFunc23())
                        ->y;
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

    if (self->timer >= (f32)(self->jumpFrames * 30 + self->waitFrames)) {
        self->motionState = 3;
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
                    reinterpret_cast<const CfGimmickJumpVec3*>(
                        static_cast<cf::CfObjectMove*>(player)
                            ->CfObject_UnkVirtualFunc23())
                        ->y;
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
            cf::CfObjectActor* actor = jumpActorFromPlayer(player);
            if (actor == 0) {
                continue;
            }
            JumpTargetData* target = reinterpret_cast<JumpTargetData*>(
                reinterpret_cast<JumpActorData*>(actor)->target);
            if (target == 0) {
                continue;
            }

            const CfGimmickJumpVec3* playerPos =
                reinterpret_cast<const CfGimmickJumpVec3*>(
                    actor->CfObject_UnkVirtualFunc23());
            typedef int (*JumpDispatchFunction)(const CfGimmickJumpVec3*,
                                                const CfGimmickJumpVec3*,
                                                const CfGimmickJumpVec3*);
            JumpDispatchFunction dispatch = reinterpret_cast<JumpDispatchFunction>(
                jumptable_eu_80535830[self->savedState.words[8]]);
            if (dispatch(&self->transformedPosition, playerPos,
                         &self->position) == 0) {
                continue;
            }
            playerPos = reinterpret_cast<const CfGimmickJumpVec3*>(
                actor->CfObject_UnkVirtualFunc23());
            if (playerPos->y <= self->verticalOffset + self->position.y) {
                self->flags |= moveBit;
                self->playerHeight[index] =
                    reinterpret_cast<const CfGimmickJumpVec3*>(
                        actor->CfObject_UnkVirtualFunc23())
                        ->y;
            }
        }
    } else if (self->linkedObject == 0) {
        finished = true;
    }

    self->timer += elapsed;

    for (int index = 0; index < 3; ++index) {
        u32 stateBit = 2u << index;
        u32 moveBit = 0x10u << index;
        if ((self->flags & stateBit) == 0 || (self->flags & moveBit) != 0) {
            continue;
        }

        finished = false;
        void* player = getPlayer__Q22cf13CfGameManagerFi(index);
        cf::CfObjectActor* actor = jumpActorFromPlayer(player);
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
        CfGimmickJumpVec3 position;
        position.z = reinterpret_cast<const CfGimmickJumpVec3*>(
            actor->CfObject_UnkVirtualFunc23())->z;
        f32 carried = self->playerHeight[index];
        position.x = reinterpret_cast<const CfGimmickJumpVec3*>(
            actor->CfObject_UnkVirtualFunc23())->x;
        position.y = carried;
        (void)actor; // setPosition stubbed
        func_8004B840(target, 0.0f);
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
                const CfGimmickJumpVec3* p = reinterpret_cast<const CfGimmickJumpVec3*>(
                    actor->CfObject_UnkVirtualFunc23());
                self->playerDeltaX[index] = self->targetX - p->x;
                p = reinterpret_cast<const CfGimmickJumpVec3*>(
                    actor->CfObject_UnkVirtualFunc23());
                self->playerDeltaZ[index] = self->targetZ - p->z;
                f32 dx = self->playerDeltaX[index];
                f32 dz = self->playerDeltaZ[index];
                f32 distSq = dx * dx + dz * dz;
                if (distSq < 0.0f) {
                    nw4r::db::Warning(lbl_eu_80526324, 0x273, lbl_eu_80526300);
                }
                f32 dist = 0.0f;
                if (distSq > 0.0f) {
                    dist = distSq * nw4r::math::FrSqrt(distSq);
                }
                if (dist != 0.0f) {
                    f32 inv = 1.0f / dist;
                    self->playerDeltaX[index] = dx * inv;
                    self->playerDeltaZ[index] = dz * inv;
                } else {
                    self->playerDeltaX[index] = 0.0f;
                    self->playerDeltaZ[index] = 0.0f;
                }
            }
        } else {
            target->flags4EC |= 0x4000000;
        }
    }

    if (finished) {
        self->motionState = 1;
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
