#pragma once

#include <types.h>

// Singleton gimmick container (kyoshin/code_801F3BE0): the object stored in
// lbl_eu_80664658 and returned by getUnk80664658(), plus the C-linkage Gim*
// exports that walk it. Every retail export of this unit is an unmangled C
// name, so they are declared here once with C linkage.

namespace cf {
class CActorParam;
}
namespace ml {
struct CVec3;
}

struct CGimmickEntry;

// 3-component position vector embedded in every gimmick object at +0x04.
struct CGimmickVec3 {
    f32 x;  // 0x00
    f32 y;  // 0x04
    f32 z;  // 0x08
};

// Singleton gimmick container (ctor __ct__801F3BE8, accessor getUnk80664658).
struct CGimmickGlobal {
    CGimmickEntry* mGimmicks[0x80];  // 0x000 - up to 0x80 gimmick objects
    s32 field_0x200;                 // 0x200 - iteration start index
    s32 field_0x204;                 // 0x204 - iteration end index
    s32 field_0x208;                 // 0x208 - object sub-count
    s32 mGimmickCount;               // 0x20C - number of gimmicks in mGimmicks
    u32 field_0x210;                 // 0x210 - value set by CfGimmick_SetGlobalFlag80AndValue
    u32 mFlags;                      // 0x214 - flag word
    void* field_0x218;               // 0x218 - actor latched by CfObjectImplWalker (cleared on teardown)
    f32 field_0x21C;                 // 0x21C - fade value
};

extern "C" {
CGimmickGlobal* getUnk80664658();
CGimmickGlobal* __ct__801F3BE8(CGimmickGlobal* self);
CGimmickGlobal* __dt__801F3C08(CGimmickGlobal* self, int flags);
void GimSpawnAll(CGimmickGlobal* self, void* obj);
void GimTeardownAll(CGimmickGlobal* self);
bool GimSpawnObjects(CGimmickGlobal* self);
bool GimSpawnLocks(CGimmickGlobal* self);
bool GimSpawnElevators(CGimmickGlobal* self);
bool GimSpawnWarps(CGimmickGlobal* self);
bool GimSpawnJumps(CGimmickGlobal* self);
bool GimSpawnItems(CGimmickGlobal* self);
bool GimSpawnEnemies(CGimmickGlobal* self);
void GimUpdateGlobal(CGimmickGlobal* self, void* obj);
void GimSetVisionResumeDist(CGimmickGlobal* self, f32 value);
void GimDispatch14ByRow(CGimmickGlobal* self, s32 id);
void GimDispatch18ByRow(CGimmickGlobal* self, s32 id);
void GimDispatch1CType1(CGimmickGlobal* self, s32 id);
void GimToggleElevatorLod(CGimmickGlobal* self, s32 id, int show);
void GimDeactivateLocks(CGimmickGlobal* self);
void GimBindEnemyActor(CGimmickGlobal* self, cf::CActorParam* actor);
void GimUnbindEnemyActor(CGimmickGlobal* self, cf::CActorParam* actor);
CGimmickVec3* GimFindPosByRow(const CGimmickGlobal* self, s32 id);
int GimProbeLockAccept(CGimmickGlobal* self, const ml::CVec3* pos);
}
