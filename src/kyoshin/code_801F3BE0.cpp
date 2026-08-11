// Auto-scaffolded catalog TU for kyoshin/code_801F3BE0
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/harness_catalog.hpp"

// ---------------------------------------------------------------------------
// Singleton gimmick container (the object getUnk80664658() returns; see
// cf/CfGimmick.hpp for the shared CfGimmickGlobal subset used by other TUs).
//
// The retail ctor symbol __ct__801F3BE8 is an address-derived recovery name
// (the function sat at 0x801F3BE8 on JP) that no C++ class mangling can
// reproduce, so the constructor is written as a free function below.
// ---------------------------------------------------------------------------

// 3-component position vector embedded in every gimmick object at +0x04.
struct CGimmickVec3 {
    f32 x;  // 0x00
    f32 y;  // 0x04
    f32 z;  // 0x08
};

// Common layout of the gimmick objects stored in the container's mGimmicks
// array (shared base of cf::CfGimmick; concrete types are CfGimmickLock,
// CfGimmickElvData, CfGimmickEne, CfGimmickObject, ...).
struct CGimmickEntry {
    void** vtable;            // 0x00
    CGimmickVec3 pos;         // 0x04 - base position
    u8 gap[0x64 - 0x10];      // 0x10
    u16 bdatRowId;            // 0x64
    u8 gap2[0x82 - 0x66];     // 0x66
    u16 typeId;               // 0x82
};

// Singleton gimmick container (ctor __ct__801F3BE8, accessor getUnk80664658).
struct CGimmickGlobal {
    CGimmickEntry* mGimmicks[0x80];  // 0x000 - up to 0x80 gimmick objects
    s32 field_0x200;                 // 0x200 - iteration start index
    s32 field_0x204;                 // 0x204 - iteration end index
    s32 field_0x208;                 // 0x208 - object sub-count
    s32 mGimmickCount;               // 0x20C - number of gimmicks in mGimmicks
    u32 field_0x210;                 // 0x210
    u32 mFlags;                      // 0x214 - flag word
    u32 field_0x218;                 // 0x218 - fade counter
    f32 field_0x21C;                 // 0x21C - fade value
};

// Singleton instance (retail .sbss). Global-scope name - no extern "C" needed
// (MWCC does not mangle global variable names).
extern u32 lbl_eu_80664658;

// Shared 0.0f float-pool constant (retail .sdata2). Referenced by name so the
// reloc lands on lbl_eu_80668158 instead of MWCC's auto pool label.
extern f32 lbl_eu_80668158;

// Cross-TU callees (defined in the cf/ gimmick units; no header declares them).
struct CfGimmickElvData;
namespace cf {
class CfGimmickLock;
class CfGimmickEne;
struct CfGimmickEneActor;
}
void func_8020B264(CfGimmickElvData* self, int show);
void func_8020CAAC(cf::CfGimmickLock* self);
void func_8026E5C0(cf::CfGimmickEne* self, cf::CfGimmickEneActor* actor);

u32 getUnk80664658(void) {
    extern u32 lbl_eu_80664658;
    return lbl_eu_80664658;
}

// Constructor: register the singleton and clear the load-state fields.
CGimmickGlobal* __ct__801F3BE8(CGimmickGlobal* self) {
    lbl_eu_80664658 = (u32)self;
    self->mGimmickCount = 0;
    self->field_0x21C = lbl_eu_80668158;
    self->field_0x218 = 0;
    self->mFlags = 0;
    return self;
}

void __dt__801F3C08(){}

void func_801F3CCC(){}

void func_801F3E80(){}

void func_801F3F98(){}

void func_801F4078(){}

void func_801F4158(){}

void func_801F4238(){}

void func_801F4318(){}

void func_801F43F8(){}

void func_801F4504(){}

void func_801F45B4(){}

void func_801F4994(void) {}

void func_801F4998(){}

void func_801F4AD4(){}

void func_801F4B64(void) {}

void func_801F4B68(){}

void func_801F4BF8(void) {}

void func_801F4BFC(){}

void func_801F4C8C(void) {}

// Look up the type-3 (elevator) gimmick with the given bdat row id and toggle
// its LOD visibility (tail-call into func_8020B264).
void func_801F4C90(CGimmickGlobal* self, u32 id, int show) {
    for (s32 i = 0; i < self->mGimmickCount; i++) {
        CGimmickEntry* g = self->mGimmicks[i];
        if (g->typeId == 3 && g->bdatRowId == id) {
            return func_8020B264((CfGimmickElvData*)self->mGimmicks[i], show);
        }
    }
}

// Deactivate every type-2 (lock) gimmick in the container.
void func_801F4CE4(CGimmickGlobal* self) {
    for (s32 i = 0; i < self->mGimmickCount; i++) {
        CGimmickEntry* g = self->mGimmicks[i];
        if (g->typeId == 2)
            func_8020CAAC((cf::CfGimmickLock*)g);
    }
}

// Bind an actor to the enemy (type-7) gimmicks in the [0x200, 0x204) range;
// iteration stops at the first non-enemy gimmick.
void func_801F4D50(CGimmickGlobal* self, cf::CfGimmickEneActor* actor) {
    if (self->mFlags & 0x4) {
        for (s32 i = self->field_0x200; i < self->field_0x204; i++) {
            CGimmickEntry* g = self->mGimmicks[i];
            if (g->typeId != 7)
                break;
            func_8026E5C0((cf::CfGimmickEne*)g, actor);
        }
    }
}

void func_801F4DDC(){}

// Find the gimmick whose bdat row id matches and return its base position;
// falls back to a zero vector.
CGimmickVec3* func_801F4E68(CGimmickGlobal* self, u32 id) {
    if (self->mFlags & 0x100) {
        for (s32 i = 0; i < self->field_0x208; i++) {
            CGimmickEntry* g = self->mGimmicks[i];
            if (g->bdatRowId == id)
                return &self->mGimmicks[i]->pos;
        }
    }
    CGimmickVec3 zero = { lbl_eu_80668158, lbl_eu_80668158, lbl_eu_80668158 };
    return &zero;
}

void func_801F4ED8(){}
