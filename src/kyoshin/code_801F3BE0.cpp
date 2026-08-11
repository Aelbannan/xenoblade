// Auto-scaffolded catalog TU for kyoshin/code_801F3BE0
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/harness_catalog.hpp"

// CChainBattleObj / CChainSub4: virtual-dispatch mirrors of the battle objects
// reached from the gimmick list (-0x3E9C) in func_801F4998. Never instantiated
// (all pure virtuals), so no vtable emits into this TU.
#include "kyoshin/cf/chain/CChainTimer.hpp"

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
    u16 field_66;             // 0x66 - state flags (bit 15 = eligible/active)
    u8 gap2[0x82 - 0x68];     // 0x68
    u16 typeId;               // 0x82
};

// Cast-only dispatch interface over the gimmick objects stored in the
// container. RTTI-on vtable layout: declared slot N dispatches at (N+2)*4,
// so slots 3/4/5 reach the retail offsets 0x14/0x18/0x1C. Never instantiated
// (no vtable is emitted into this TU).
struct CGimmickDispatch {
    virtual void d0(int);  // dispatched at vtable + 0x08 (teardown slot)
    virtual void d1();
    virtual void d2();
    virtual void vf14();   // dispatched at vtable + 0x14
    virtual void vf18();   // dispatched at vtable + 0x18
    virtual void vf1C();   // dispatched at vtable + 0x1C
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
// reloc lands on lbl_eu_80668158 instead of MWCC's auto pool label; const lets
// MWCC hoist the load like a literal (CScrollBar.hpp pattern).
extern const f32 lbl_eu_80668158;

// Cross-TU callees (defined in the cf/ gimmick units; no header declares them).
// Declared with C linkage so the call relocs keep the retail names
// (func_8020B264's own definition is extern "C").
struct CfGimmickElvData;
namespace cf {
class CfGimmickLock;
class CfGimmickEne;
struct CfGimmickEneActor;
}
extern "C" void func_8020B264(CfGimmickElvData* self, int show);
extern "C" void func_8020CAAC(cf::CfGimmickLock* self);
extern "C" void func_8026E5C0(cf::CfGimmickEne* self, cf::CfGimmickEneActor* actor);
extern "C" void func_8026E678(cf::CfGimmickEne* self, cf::CfGimmickEneActor* actor);
extern "C" int func_8020D368(cf::CfGimmickLock* self, void* target);

// Teardown / spawn helpers used by the lifecycle functions below.
// func_80208E98 resets the shared CfGimmick state globals (CfGimmick.cpp).
extern "C" void func_80208E98();
// CBdat table helpers: prepare table, first row index, row count.
extern "C" void* func_8003AA34();
extern "C" u32 func_8003B41C(void* bdat);
extern "C" u32 func_8003B1EC(void* bdat);
// Heap-handle query used as the allocation region for spawned gimmicks.
extern "C" u32 func_80061FFC();
// Concrete gimmick constructors (cf/ units). The stored object is a
// CGimmickEntry (shared CfGimmick base); the ctor fills the derived fields.
extern "C" void* __ct__cf_CfGimmickLock(CGimmickEntry* self, u16 row);
extern "C" void* __ct__cf_CfGimmickElv(CGimmickEntry* self, u16 row);
extern "C" void* __ct__cf_CfGimmickWarp(CGimmickEntry* self, u16 row);
extern "C" CGimmickEntry* __ct__cf_CfGimmickSaveOff(CGimmickEntry* self, s32 row);
// Bdat table pointers for the lock / elevator / warp tables (sda21 globals).
extern void* lbl_eu_8066412C;
extern void* lbl_eu_80664130;
extern void* lbl_eu_80664134;
// Bdat table pointers for the object / jump / item / enemy tables (sda21
// globals). u8* opaque buffers (no void* in this TU's imports).
extern u8* lbl_eu_80664128;
extern u8* lbl_eu_80664138;
extern u8* lbl_eu_8066413C;
extern u8* lbl_eu_80664140;  // save-off gimmick bdat table (sda21)
extern u8* lbl_eu_80664144;

// Circular gimmick object list returned by func_800B6BC8 (CfGimmick.hpp
// layout): +0x00 opaque, +0x04 head node, nodes link through +0x00 and carry
// the object at +0x08.
struct CGimmickListNode {
    CGimmickListNode* next;  // 0x00
    u8 gap04[4];             // 0x04
    u8* object;              // 0x08 - points at the +0x3E9C sub-object
};
struct CGimmickList {
    u8* field_00;            // 0x00
    CGimmickListNode* head;  // 0x04
};

// CfGimmick ctors for the remaining gimmick types. These are C-linkage retail
// names (no C++ mangling markers; the defining TUs emit them with extern "C"
// and their recovered u16 row signatures are not byte-matched yet), so they
// are declared with C linkage here to keep the call relocs on the retail
// names. The row is passed untruncated (32-bit) like the retail call sites.
extern "C" CGimmickEntry* __ct__cf_CfGimmickJump(CGimmickEntry* obj, int row);
extern "C" CGimmickEntry* __ct__cf_CfGimmickItem(CGimmickEntry* obj, int row);
extern "C" CGimmickEntry* __ct__cf_CfGimmickEne(CGimmickEntry* obj, int row);
extern "C" CGimmickEntry* __ct__cf_CfGimmickObject(CGimmickEntry* obj, int row,
                                                    CGimmickEntry** arr, int idx,
                                                    u8* buf);

// CfGimmick shared bdat-row reset (CfGimmick.cpp): stores the row into a
// global state slot.
extern "C" void func_80208EDC(u32 value);
// Gimmick object list accessor (func_80174C98 comes from CChainTimer.hpp).
extern "C" CGimmickList* func_800B6BC8();

// Sibling spawn / lifecycle functions defined later in this TU (func_801F3CCC
// sits before their definitions and calls them). C linkage keeps the call
// relocs on the unmangled retail names (func_801F3E80, ...).
extern "C" bool func_801F3E80(CGimmickGlobal* self);
extern "C" bool func_801F3F98(CGimmickGlobal* self);
extern "C" bool func_801F4078(CGimmickGlobal* self);
extern "C" bool func_801F4158(CGimmickGlobal* self);
extern "C" bool func_801F4238(CGimmickGlobal* self);
extern "C" bool func_801F4318(CGimmickGlobal* self);
extern "C" bool func_801F43F8(CGimmickGlobal* self);

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

// Destructor (address-derived name __dt__801F3C08): same teardown as
// func_801F4504, then unregisters the singleton and frees the container when
// the delete flag (flags > 0) is set.
CGimmickGlobal* __dt__801F3C08(CGimmickGlobal* self, int flags) {
    if (self) {
        self->mFlags = 0;
        self->field_0x200 = 0;
        self->field_0x21C = lbl_eu_80668158;
        for (s32 i = 0; i < self->mGimmickCount; i++) {
            CGimmickEntry* g = self->mGimmicks[i];
            if (g == NULL)
                continue;
            if (g)
                ((CGimmickDispatch*)g)->d0(1);
            self->mGimmicks[i] = 0;
        }
        self->mGimmickCount = 0;
        self->field_0x218 = 0;
        self->field_0x210 = 0;
        func_80208E98();
        lbl_eu_80664658 = 0;
        if (flags > 0)
            delete self;
    }
    return self;
}

// func_801F3CCC: (re)initialize the gimmick container. Tears down every stored
// gimmick (dispatch slot 0x08 with 1) and resets the counters/state, then
// re-spawns the object/lock/elevator/warp/jump/item families from their bdat
// tables in order, stopping at the first failure. The save-off family is
// spawned inline from lbl_eu_80664140 (0x88-byte objects); the enemy family
// (func_801F43F8) runs only when every other family fit in the 0x80 slots.
void func_801F3CCC(CGimmickGlobal* self) {
    self->mFlags = 0;
    self->field_0x200 = 0;
    self->field_0x21C = lbl_eu_80668158;
    for (s32 i = 0; i < self->mGimmickCount; i++) {
        CGimmickEntry* g = self->mGimmicks[i];
        if (g == NULL)
            continue;
        if (g)
            ((CGimmickDispatch*)g)->d0(1);
        self->mGimmicks[i] = 0;
    }
    self->mGimmickCount = 0;
    self->field_0x218 = 0;
    self->field_0x210 = 0;
    func_80208E98();
    if (!func_801F3E80(self))
        return;
    if (!func_801F3F98(self))
        return;
    if (!func_801F4078(self))
        return;
    if (!func_801F4158(self))
        return;
    if (!func_801F4238(self))
        return;
    if (!func_801F4318(self))
        return;
    bool ok;
    u8* bdat = lbl_eu_80664140;
    if (bdat == NULL) {
        ok = true;
    } else {
        func_8003AA34();
        s32 row = (s32)func_8003B41C(bdat);
        s32 n = (s32)func_8003B1EC(bdat);
        for (s32 i = 0; i < n; i++) {
            CGimmickEntry* obj =
                (CGimmickEntry*)mtl::MemManager::allocate(0x88, func_80061FFC());
            if (obj)
                obj = __ct__cf_CfGimmickSaveOff(obj, row);
            self->mGimmicks[self->mGimmickCount] = obj;
            self->mGimmickCount++;
            if (self->mGimmickCount >= 0x80) {
                ok = false;
                goto check;
            }
            row++;
        }
        ok = true;
    check:
        ;
    }
    if (ok)
        func_801F43F8(self);
}

// func_801F3E80: spawn the object gimmicks from the object bdat table. The
// shared state reset runs first (0 on the empty-table path, row-1 otherwise)
// and the ctor registers each object into the tail of the container via the
// (row, &mGimmicks[count0], count-count0, column-buf) argument bundle.
bool func_801F3E80(CGimmickGlobal* self) {
    self->field_0x208 = 0;
    u8* bdat = lbl_eu_80664128;
    if (bdat == NULL) {
        func_80208EDC(0);
        return true;
    }
    self->mFlags |= 0x100;
    func_8003AA34();
    u32 row = func_8003B41C(bdat);
    s32 n = (s32)func_8003B1EC(bdat);
    u8 buf[0x20];
    memset(buf, 0, 0x20);
    func_80208EDC(row - 1);
    s32 count0 = self->mGimmickCount;
    for (s32 i = 0; i < n; i++) {
        CGimmickEntry* obj =
            (CGimmickEntry*)mtl::MemManager::allocate(0x19c, func_80061FFC());
        if (obj)
            obj = __ct__cf_CfGimmickObject(obj, row, &self->mGimmicks[count0],
                                           self->mGimmickCount - count0, buf);
        self->mGimmicks[self->mGimmickCount] = obj;
        self->mGimmickCount++;
        if (self->mGimmickCount >= 0x80) {
            self->field_0x208 = self->mGimmickCount;
            return false;
        }
        row++;
    }
    self->field_0x208 = self->mGimmickCount;
    return true;
}

// func_801F3F98: spawn the lock gimmicks from the lock bdat table. Returns
// false if the 0x80-slot container fills up mid-spawn.
bool func_801F3F98(CGimmickGlobal* self) {
    void* bdat = lbl_eu_8066412C;
    if (bdat == NULL)
        return true;
    self->mFlags |= 0x200;
    func_8003AA34();
    u32 row = func_8003B41C(bdat);
    s32 n = (s32)func_8003B1EC(bdat);
    for (s32 i = 0; i < n; i++) {
        void* obj = mtl::MemManager::allocate(0x1fc, func_80061FFC());
        if (obj)
            obj = __ct__cf_CfGimmickLock((CGimmickEntry*)obj, (u16)row);
        self->mGimmicks[self->mGimmickCount] = (CGimmickEntry*)obj;
        self->mGimmickCount++;
        if (self->mGimmickCount >= 0x80)
            return false;
        row++;
    }
    return true;
}

// func_801F4078: spawn the elevator gimmicks from the elevator bdat table;
// same shape as func_801F3F98 with the 0x400 flag and 0x1d8-byte objects.
bool func_801F4078(CGimmickGlobal* self) {
    void* bdat = lbl_eu_80664130;
    if (bdat == NULL)
        return true;
    self->mFlags |= 0x400;
    func_8003AA34();
    u32 row = func_8003B41C(bdat);
    s32 n = (s32)func_8003B1EC(bdat);
    for (s32 i = 0; i < n; i++) {
        void* obj = mtl::MemManager::allocate(0x1d8, func_80061FFC());
        if (obj)
            obj = __ct__cf_CfGimmickElv((CGimmickEntry*)obj, (u16)row);
        self->mGimmicks[self->mGimmickCount] = (CGimmickEntry*)obj;
        self->mGimmickCount++;
        if (self->mGimmickCount >= 0x80)
            return false;
        row++;
    }
    return true;
}

// func_801F4158: spawn the warp gimmicks from the warp bdat table; same shape
// as func_801F3F98 with the 0x800 flag and 0x10c-byte objects.
bool func_801F4158(CGimmickGlobal* self) {
    void* bdat = lbl_eu_80664134;
    if (bdat == NULL)
        return true;
    self->mFlags |= 0x800;
    func_8003AA34();
    u32 row = func_8003B41C(bdat);
    s32 n = (s32)func_8003B1EC(bdat);
    for (s32 i = 0; i < n; i++) {
        void* obj = mtl::MemManager::allocate(0x10c, func_80061FFC());
        if (obj)
            obj = __ct__cf_CfGimmickWarp((CGimmickEntry*)obj, (u16)row);
        self->mGimmicks[self->mGimmickCount] = (CGimmickEntry*)obj;
        self->mGimmickCount++;
        if (self->mGimmickCount >= 0x80)
            return false;
        row++;
    }
    return true;
}

// func_801F4238: spawn the jump gimmicks from the jump bdat table; same
// shape as func_801F3F98 with the 0x1000 flag and 0x170-byte objects.
bool func_801F4238(CGimmickGlobal* self) {
    u8* bdat = lbl_eu_80664138;
    if (bdat == NULL)
        return true;
    self->mFlags |= 0x1000;
    func_8003AA34();
    u32 row = func_8003B41C(bdat);
    s32 n = (s32)func_8003B1EC(bdat);
    for (s32 i = 0; i < n; i++) {
        CGimmickEntry* obj =
            (CGimmickEntry*)mtl::MemManager::allocate(0x170, func_80061FFC());
        if (obj)
            obj = __ct__cf_CfGimmickJump(obj, row);
        self->mGimmicks[self->mGimmickCount] = obj;
        self->mGimmickCount++;
        if (self->mGimmickCount >= 0x80)
            return false;
        row++;
    }
    return true;
}

// func_801F4318: spawn the item gimmicks from the item bdat table; same shape
// as func_801F3F98 with the 0x2000 flag and 0xa4-byte objects.
bool func_801F4318(CGimmickGlobal* self) {
    u8* bdat = lbl_eu_8066413C;
    if (bdat == NULL)
        return true;
    self->mFlags |= 0x2000;
    func_8003AA34();
    u32 row = func_8003B41C(bdat);
    s32 n = (s32)func_8003B1EC(bdat);
    for (s32 i = 0; i < n; i++) {
        CGimmickEntry* obj =
            (CGimmickEntry*)mtl::MemManager::allocate(0xa4, func_80061FFC());
        if (obj)
            obj = __ct__cf_CfGimmickItem(obj, row);
        self->mGimmicks[self->mGimmickCount] = obj;
        self->mGimmickCount++;
        if (self->mGimmickCount >= 0x80)
            return false;
        row++;
    }
    return true;
}

// func_801F43F8: spawn the enemy gimmicks from the enemy bdat table. When the
// table is non-empty the enemy run window [0x200, 0x204) is opened at the
// current count; it is re-closed at the final count on both exit paths.
bool func_801F43F8(CGimmickGlobal* self) {
    u8* bdat = lbl_eu_80664144;
    if (bdat == NULL)
        return true;
    self->mFlags |= 0x4000;
    func_8003AA34();
    u32 row = func_8003B41C(bdat);
    s32 n = (s32)func_8003B1EC(bdat);
    if (n != 0) {
        self->mFlags |= 0x4;
        self->field_0x200 = self->mGimmickCount;
        self->field_0x204 = self->mGimmickCount;
    }
    for (s32 i = 0; i < n; i++) {
        CGimmickEntry* obj =
            (CGimmickEntry*)mtl::MemManager::allocate(0xc0, func_80061FFC());
        if (obj)
            obj = __ct__cf_CfGimmickEne(obj, row);
        self->mGimmicks[self->mGimmickCount] = obj;
        self->mGimmickCount++;
        if (self->mGimmickCount >= 0x80) {
            self->field_0x204 = self->mGimmickCount;
            return false;
        }
        row++;
    }
    self->field_0x204 = self->mGimmickCount;
    return true;
}

// func_801F4504: teardown the gimmick container. Flags and the iteration
// window are reset first, then every stored gimmick's teardown slot (retail
// vtable + 0x08) is invoked with 1 and the slot cleared, then the shared
// state reset helper runs.
void func_801F4504(CGimmickGlobal* self) {
    self->mFlags = 0;
    self->field_0x200 = 0;
    self->field_0x21C = lbl_eu_80668158;
    for (s32 i = 0; i < self->mGimmickCount; i++) {
        CGimmickEntry* g = self->mGimmicks[i];
        if (g == NULL)
            continue;
        if (g)
            ((CGimmickDispatch*)g)->d0(1);
        self->mGimmicks[i] = 0;
    }
    self->mGimmickCount = 0;
    self->field_0x218 = 0;
    self->field_0x210 = 0;
    func_80208E98();
}

void func_801F45B4(){}

void func_801F4994(void) {}

// func_801F4998: enable/disable the vision-fusion resume distance. Passing
// 0.0f sets the 0x20 flag; any other value clears it (and is a no-op when the
// flag is already clear). While enabled, every gimmick-list battle object and
// every player gets the value pushed into its vtable slot 0x5C4, gated on the
// func_80174C98 actor-id check (flag 3).
void func_801F4998(CGimmickGlobal* self, f32 value) {
    if (lbl_eu_80668158 == value) {
        self->mFlags |= 0x20;
    } else {
        if (!(self->mFlags & 0x20))
            return;
        // volatile deref keeps the retail fresh reload (retail reloads mFlags
        // for the clear instead of reusing the test's value)
        *(volatile u32*)&self->mFlags &= ~0x20;
    }
    CGimmickList* list = func_800B6BC8();
    cf::CChainBattleObj* obj;
    CGimmickListNode* node = list->head->next;
    while (node != list->head) {
        obj = (cf::CChainBattleObj*)node->object;
        if (obj)
            obj = (cf::CChainBattleObj*)((u8*)obj - 0x3E9C);
        u32 local = *(u32*)obj->field_04->f30();
        if (func_80174C98(obj, (int*)&local, 3) == 0)
            obj->v367(value);
        node = node->next;
    }
    for (s32 i = 0; i < 3; i++) {
        cf::CfObjectMove* player = cf::CfGameManager::getPlayer(i);
        if (player)
            player = (cf::CfObjectMove*)((u8*)player - 0x3E9C);
        if (player)
            ((cf::CChainBattleObj*)player)->v367(value);
    }
}

// Dispatch slot 0x14 on every eligible (state bit 15) gimmick whose bdat row
// id matches.
void func_801F4AD4(CGimmickGlobal* self, s32 id) {
    for (s32 i = 0; i < self->mGimmickCount; i++) {
        CGimmickEntry* g = self->mGimmicks[i];
        if ((g->field_66 & 0x8000) && id == g->bdatRowId)
            ((CGimmickDispatch*)g)->vf14();
    }
}

void func_801F4B64(void) {}

// Dispatch slot 0x18 on every eligible (state bit 15) gimmick whose bdat row
// id matches.
void func_801F4B68(CGimmickGlobal* self, s32 id) {
    for (s32 i = 0; i < self->mGimmickCount; i++) {
        CGimmickEntry* g = self->mGimmicks[i];
        if ((g->field_66 & 0x8000) && id == g->bdatRowId)
            ((CGimmickDispatch*)g)->vf18();
    }
}

void func_801F4BF8(void) {}

// Dispatch slot 0x1C on the type-1 gimmick whose bdat row id matches.
void func_801F4BFC(CGimmickGlobal* self, s32 id) {
    for (s32 i = 0; i < self->mGimmickCount; i++) {
        CGimmickEntry* g = self->mGimmicks[i];
        if (g->typeId == 1 && id == g->bdatRowId)
            ((CGimmickDispatch*)g)->vf1C();
    }
}

void func_801F4C8C(void) {}

// Look up the type-3 (elevator) gimmick with the given bdat row id and toggle
// its LOD visibility (tail-call into func_8020B264).
void func_801F4C90(CGimmickGlobal* self, s32 id, int show) {
    CGimmickEntry* g;
    s32 i;
    for (i = 0; i < self->mGimmickCount; i++) {
        g = self->mGimmicks[i];
        if (g->typeId == 3 && id == g->bdatRowId) {
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

// Unbind the actor from the enemy (type-7) gimmicks starting at field_0x200;
// iteration stops at the first non-enemy gimmick.
void func_801F4DDC(CGimmickGlobal* self, cf::CfGimmickEneActor* actor) {
    if (self->mFlags & 0x4) {
        for (s32 i = self->field_0x200; i < self->mGimmickCount; i++) {
            CGimmickEntry* g = self->mGimmicks[i];
            if (g->typeId != 7)
                break;
            func_8026E678((cf::CfGimmickEne*)g, actor);
        }
    }
}

// Find the gimmick whose bdat row id matches and return its base position;
// falls back to a zero vector.
CGimmickVec3* func_801F4E68(const CGimmickGlobal* self, s32 id) {
    if (self->mFlags & 0x100) {
        for (s32 i = 0; i < self->field_0x208; i++) {
            CGimmickEntry* g = self->mGimmicks[i];
            if (id == g->bdatRowId)
                return &self->mGimmicks[i]->pos;
        }
    }
    CGimmickVec3 zero = { lbl_eu_80668158, lbl_eu_80668158, lbl_eu_80668158 };
    return &zero;
}

// Probe every type-2 (lock) gimmick; return 1 as soon as one accepts the id.
int func_801F4ED8(CGimmickGlobal* self, s32 id) {
    for (s32 i = 0; i < self->mGimmickCount; i++) {
        CGimmickEntry* g = self->mGimmicks[i];
        if (g->typeId == 2 && func_8020D368((cf::CfGimmickLock*)g, (void*)id))
            return 1;
    }
    return 0;
}
