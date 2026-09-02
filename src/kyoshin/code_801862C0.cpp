// Auto-scaffolded catalog TU for kyoshin/code_801862C0
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/code_801862C0.hpp"
#include "kyoshin/cf/CfMapMineManager.hpp" // func_800B8920 / func_800B9404
#include "kyoshin/cf/object/CfObjectMoveApi.hpp" // func_800BE12C
#include "kyoshin/cf/object/CfObject.hpp"
#include "libs/monolib/src/scn/CScnItemModel.hpp"
#include "monolib/math/CVec3.hpp"

// CArtsSelectSlot / CArtsSelectBucket are defined in code_801862C0.hpp.

// Source object whose field_0x74 holds the arts id.
struct CArtsSelectSrc {
    u8 pad[0x74];
    u32 id;
};

// Returned singleton object backing func_801862C0's accessor.
// Sized to land in normal .bss (not SDA) so its address materializes as lis+addi.
u8 lbl_eu_80574090[0x1708];
s8 lbl_eu_806642C8;
u32 lbl_eu_806642D0;

/* One-time init guard: set the SDA flag on first call, then hand out the
   singleton object's address. Sign-extension in retail matches a signed byte. */
void* func_801862C0(void) {
    if (lbl_eu_806642C8 == 0) {
        lbl_eu_806642C8 = 1;
    }
    return &lbl_eu_80574090;
}

/* Clears the whole arts-select container: all 16 hash buckets (46 slots of
   (id, ptr) each) and the fast-path cache pair at 0x1700/0x1704. Returns p.
   Note: tbl/bucket declared before i/j pins register colors (bucket=r6,
   counter=r7). */
void* func_801862E0(void* p) {
    CArtsSelectContainer* tbl = (CArtsSelectContainer*)p;
    CArtsSelectBucket* bucket = tbl->buckets;
    int i;
    int j;
    for (i = 0; i < 16; i++, bucket++) {
        for (j = 0; j < 46; j++) {
            bucket->slots[j].unk00 = 0;
            bucket->slots[j].unk04 = 0;
        }
    }
    tbl->field_1700 = 0;
    tbl->field_1704 = 0;
    return p;
}

void* func_801863F4(void* self, void* src) {
    // Insert (srcId, src) into the first free slot of the id's hash bucket.
    u32 srcId = *reinterpret_cast<const u32*>(static_cast<const u8*>(src) + 0x74);

    // Bucket index: t = rotl((id << 28) - bit31(id), 4) + bit31(id).
    u32 t = srcId << 28;
    u32 sign = srcId >> 31;
    t -= sign;
    t = (t << 4) | (t >> 28);
    t += sign;

    u32 off = t * 0x170;
    CArtsSelectBucket* bucket = &static_cast<CArtsSelectBucket*>(self)[t];
    CArtsSelectSlot* slot = bucket->slots;
    int i;
    for (i = 0; i < 46; i++) {
        if (slot->unk00 == 0) {
            bucket->slots[i].unk00 = srcId;
            CArtsSelectSlot* hit = reinterpret_cast<CArtsSelectSlot*>(
                static_cast<u8*>(self) + (i * 8 + off));
            hit->unk04 = reinterpret_cast<u32>(src);
            return reinterpret_cast<void*>(srcId);
        }
        slot++;
    }
    return reinterpret_cast<void*>(srcId);
}

extern "C" void* func_80186460(void* dst, void* src) {
    const u8* srcBytes = static_cast<const u8*>(src);
    u32 id = *reinterpret_cast<const u32*>(srcBytes + 0x74);
    *(u32*)((u8*)dst + 0x1700) = id;
    *(u32*)((u8*)dst + 0x1704) = reinterpret_cast<u32>(src);
    return reinterpret_cast<void*>(id);
}

// Remove an arts entry matching src->field_74 from its hash bucket.
// Bucket index = rotl((id<<28) - bit31(id), 4) + bit31(id); each of the 46
// slots holds an (id, ptr) pair. On hit both words are cleared and the slot
// address is returned; a miss returns the container itself.
void* func_80186474(void* self, void* src){
    CArtsSelectSlot* slot;
    CArtsSelectSlot* slots;
    u32 srcId = *reinterpret_cast<const u32*>(static_cast<const u8*>(src) + 0x74);
    u32 sign;
    u32 t;
    u32 off;
    int i;

    t = srcId << 28;
    sign = srcId >> 31;
    t -= sign;
    t = (t << 4) | (t >> 28);
    t += sign;
    off = t * 0x170;
    slots = reinterpret_cast<CArtsSelectSlot*>(static_cast<u8*>(self) + off);
    slot = slots;
    for (i = 0; i < 46; i++) {
        if (slot->unk00 == srcId) {
            CArtsSelectSlot* hit = reinterpret_cast<CArtsSelectSlot*>(
                static_cast<u8*>(self) + off + i * 8);
            slots[i].unk00 = 0;
            hit->unk04 = 0;
            return hit;
        }
        slot++;
    }
    return self;
}

// Constructor: clears structure and stores this pointer to singleton
extern "C" void* __ct__80186578(void* self){
    lbl_eu_806642D0 = reinterpret_cast<u32>(self);
    memset(self, 0, 0x100);
    memset(reinterpret_cast<u8*>(self) + 0x100, 0, 0x80);
    return self;
}

// func_801864DC: widget id lookup. Fast path: field_1700 == id returns
// field_1704. Otherwise a hash-indexed table search: index =
// ((((id>>4)&0xF) - sign) << 4) + sign (sign = bit 31), stride 0x170, 46
// entries of 8 bytes; miss falls back to findObjectById(id).
void* func_801864DC(void* pObj, int slot) {
    u8* self = (u8*)pObj;
    u32 id = (u32)slot;
    u32 t;
    u32 sign;
    u32 off;
    if (*(u32*)(self + 0x1700) == id) {
        return *(void**)(self + 0x1704);
    }
    // Retail hash: t = rotl((id << 28) - bit31(id), 4) + bit31(id).
    sign = (id >> 31) & 1;
    t = (id << 28) - sign;
    t = (t << 4) | (t >> 28);
    t += sign;
    off = t * 0x170;
    u8* base = self + off;
    int j;
    for (j = 0; j < 46; j++) {
        if (id == *(u32*)(base + j * 8)) {
            // Recompute the entry address from the base (retail add chain).
            u8* hit = (u8*)self + off;
            return *(void**)(hit + (j << 3) + 4);
        }
    }
    void* r = findObjectById(slot);
    if (r) {
        return r;
    }
    return 0;
}

// Destructor for the arts-select container: same teardown walk as
// func_80186664 (free armed slots, clear pointer + halfword flag at
// +0x100), then clear the singleton pointer and delete the object when the
// scalar-deleting flag is set (> 0). Body guarded by a null-this check.
extern "C" void* __dt__801865C4(void* self, int flags) {
    s32 i;
    void** p;
    u16* f;
    if (self != NULL) {
        p = (void**)self;
        f = (u16*)self;
        for (i = 0; i < 64; i++) {
            void* ptr = *p;
            if (ptr != 0) {
                if (func_800B8920(ptr) != 0) {
                    // Reload the slot pointer for the free call (retail
                    // re-reads through the slot address each time).
                    func_800B9404(*p);
                    *p = (void*)0;
                }
                *p = (void*)0;
                *(f + 0x80) = (u16)0;
            }
            p++;
            f++;
        }
        lbl_eu_806642D0 = 0;
        if (flags > 0) {
            __dl__FPv(self);
        }
    }
    return self;
}

// func_80186664 (recovered): teardown loop over 64 widget slots.  For each
// armed slot: run the check helper (func_800B8920) and, when it passes, the
// free helper (func_800B9404); both the slot pointer and the slot's halfword
// flag at +0x100 are cleared on the armed path only.
void func_80186664(u8* self) {
    void** p = (void**)self;
    u16* f = (u16*)self;
    s32 i;
    // Retail clears the flag only when the slot was armed (the ptr==0 branch
    // skips both stores).
    for (i = 0; i < 64; i++) {
        void* ptr = *p;
        if (ptr != 0) {
            if (func_800B8920(ptr) != 0) {
                // Retail re-reads the slot pointer for the second call (no
                // local kept across calls; the calls may alias the array).
                func_800B9404(*p);
                *p = (void*)0;
            }
            *p = (void*)0;
            *(f + 0x80) = (u16)0;
        }
        p++;
        f++;
    }
}

/* func_801866F0: spawn one map object from the bdat "map" table row `row`
   and configure it. Column order (lbl_eu_805038C8 pool): model, motion,
   posX/Y/Z, rotX/Y/Z, ground, gravity, scale, disp. Position uses a 0.0001
   fixed-point scale; rotations are hundredths of a degree converted to
   radians; the scale column is tenths. The gravity column doubles as an
   invert-visibility flag, and the disp column sets/clears state bits in the
   object's 0x6C flag word. */
void func_801866F0(MapProxy** objects, int row) {
    // Refresh the bdat manager (result unused; it populates lbl_eu_806640B0).
    func_8003AA34();
    const char* cols = (const char*)lbl_eu_805038C8;
    void* bdat = lbl_eu_806640B0;

    // Column values stay memory-resident in u32 slots; the narrow reads
    // (lbz/lha/lhz through the slot) match retail's store/reload shape.
    u32 modelCol = getBdatStringColumnValue(bdat, cols + 0x00, row);
    u32 motionCol = getBdatStringColumnValue(bdat, cols + 0x06, row);
    objects[row] = reinterpret_cast<MapProxy*>(
        createPlayerEffectInstance__Q22cf13CfGameManagerFv(*(u8*)&modelCol, *(u8*)&motionCol));
    if (objects[row] == NULL) {
        return;
    }

    Vec3f pos;
    pos.x = lbl_eu_806679C0 *
            (f32)((f64)(s32)getBdatStringColumnValue(bdat, cols + 0x0d, row) - lbl_eu_806679D0);
    pos.y = lbl_eu_806679C0 *
            (f32)((f64)(s32)getBdatStringColumnValue(bdat, cols + 0x12, row) - lbl_eu_806679D0);
    pos.z = lbl_eu_806679C0 *
            (f32)((f64)(s32)getBdatStringColumnValue(bdat, cols + 0x17, row) - lbl_eu_806679D0);

    // Rotations are hundredths of a degree (s16 column) times 0.01; volatile
    // keeps them in stack slots across the intervening bdat calls.
    volatile f32 rotX;
    volatile f32 rotY;
    volatile f32 rotZ;
    u32 rotXCol = getBdatStringColumnValue(bdat, cols + 0x1c, row);
    rotX = lbl_eu_806679C4 * (f32)((f64)*(s16*)&rotXCol - lbl_eu_806679D0);
    u32 rotYCol = getBdatStringColumnValue(bdat, cols + 0x21, row);
    rotY = lbl_eu_806679C4 * (f32)((f64)*(s16*)&rotYCol - lbl_eu_806679D0);
    u32 rotZCol = getBdatStringColumnValue(bdat, cols + 0x26, row);
    rotZ = lbl_eu_806679C4 * (f32)((f64)*(s16*)&rotZCol - lbl_eu_806679D0);

    u32 airCol = getBdatStringColumnValue(bdat, cols + 0x2b, row);
    u32 groundCol = getBdatStringColumnValue(bdat, cols + 0x32, row);
    // Narrowed immediately into a register-held scalar (retail keeps this
    // byte live in a callee-saved register until the setVisible calls).
    int ground = *(u8*)&groundCol;
    u32 scaleCol = getBdatStringColumnValue(bdat, cols + 0x3a, row);
    f32 scale = lbl_eu_806679C8 * (f32)((f64)*(u16*)&scaleCol - lbl_eu_806679D8);

    Vec3f rot;
    rot.x = rotX * lbl_eu_8066A210;
    rot.y = rotY * lbl_eu_8066A210;
    rot.z = rotZ * lbl_eu_8066A210;
    if (*(u8*)&airCol == 0) {
        objects[row]->placeOnGround(reinterpret_cast<ml::CVec3*>(&pos), lbl_eu_806679CC);
    } else {
        objects[row]->placeInAir(reinterpret_cast<ml::CVec3*>(&pos), lbl_eu_806679CC);
    }
    // Retail copies the current position out (result unused) - keep it.
    Vec3f curPos = *reinterpret_cast<Vec3f*>(objects[row]->getPos());
    objects[row]->applyRot(reinterpret_cast<ml::CVec3*>(&rot));
    objects[row]->setScale(scale);
    int visible = (ground == 0);
    objects[row]->setVisible(visible);
    objects[row]->setVisible2(visible);

    // Re-derive the column-pool address here (retail rematerializes it for
    // the final call instead of keeping the pointer live past setVisible).
    u32 dispCol = getBdatStringColumnValue(bdat, (const char*)lbl_eu_805038C8 + 0x40, row);
    if (*(u8*)&dispCol != 0) {
        objects[row]->field_6C |= 0x1000;
    } else {
        objects[row]->field_6C &= ~0x8;
    }
}

/* func_80186A70: arts-availability check for row `row` of the current bdat
   table. Reads seven columns (c1..c7): if c1 is non-zero it must fall inside
   the [c2,c3] window around the cf game manager resource counter
   (getResourceFromTable); otherwise a second counter (getQueuedFileEventCount) must fall in
   either the [c4,c5] or [c6,c7] window (a zero upper bound disables a
   window). Returns 1 when any applicable window contains the counter. */
int func_80186A70(void* p, s32 row, const char* c1, const char* c2,
                  const char* c3, const char* c4, const char* c5,
                  const char* c6, const char* c7) {
    void* bdat;
    u32 v1;
    u32 v2;
    u32 v3;
    u32 v4;
    u32 v5;
    u32 v6;
    u32 v7;
    u16 cur;
    bdat = lbl_eu_806640B0;
    v1 = getBdatStringColumnValue(bdat, c1, row);
    // Narrow reads go through the value's storage (not casts) so MWCC keeps
    // the column values memory-resident like retail (stw + lhz/lbz).
    if (*(u16*)&v1 != 0) {
        // Resource id gate: must sit inside the closed [lo,hi] byte window
        // around the cf game manager resource counter.
        cur = getResourceFromTable__Q22cf13CfGameManagerFv(*(u16*)&v1);
        v2 = getBdatStringColumnValue(bdat, c2, row);
        v3 = getBdatStringColumnValue(bdat, c3, row);
        if ((u32)cur < *(u8*)&v2 || (u32)cur > *(u8*)&v3) {
            return 0;
        }
    }
    cur = getQueuedFileEventCount__Q22cf13CfGameManagerFv();
    v4 = getBdatStringColumnValue(bdat, c4, row);
    v5 = getBdatStringColumnValue(bdat, c5, row);
    v6 = getBdatStringColumnValue(bdat, c6, row);
    v7 = getBdatStringColumnValue(bdat, c7, row);
    // Sequence counter must land in either halfword window; a zero upper
    // bound disables its window.
    if (((u32)cur >= *(u16*)&v4 && (u32)cur <= *(u16*)&v5 && *(u16*)&v5 != 0) ||
        ((u32)cur >= *(u16*)&v6 && (u32)cur <= *(u16*)&v7 && *(u16*)&v7 != 0)) {
        return 1;
    }
    return 0;
}

// func_80186BC8: look up entry `p` in the arts-select container's entry
// array, restricted to the bdat table's valid row range
// [rowBase, rowBase + rowCount). Returns NULL when the container singleton,
// the requested index, or the slot itself is absent.
void* func_80186BC8(int p) {
    void* bdat;
    void** entries;
    int rowBase;
    int rowCount;
    int i;
    if (lbl_eu_806642D0 != 0 && p != 0) {
        bdat = lbl_eu_806640B0;
        rowBase = (int)func_8003B41C(bdat);
        rowCount = (int)func_8003B1EC(bdat) + rowBase;
        entries = (void**)lbl_eu_806642D0;
        for (i = rowBase; i < rowCount; i++) {
            if (entries[i] != NULL && i == p) {
                return entries[i];
            }
        }
    }
    return 0;
}

// func_80186C7C: notify every armed widget in the arts-select container.
// For each of the 64 slots: if the slot holds a widget with a notifier, call
// the notifier's vtable+0x88 entry with 1 when the widget's 0x6C flag bit
// 0x10000000 is set AND the argument is non-zero, else 0. The container
// pointer is re-read from the singleton each iteration (retail reloads it;
// the virtual call may mutate it).
void func_80186C7C(void* p) {
    if (lbl_eu_806642D0 != 0) {
        s32 i;
        for (i = 0; i < 64; i++) {
            MapProxy** entries = reinterpret_cast<MapProxy**>(lbl_eu_806642D0);
            MapProxy* widget = entries[i];
            if (widget == NULL) {
                continue;
            }
            // Notifier at +0x98 is a CScnItemModel (vfunc88(int) at 0x88).
            CScnItemModel* notifier = *reinterpret_cast<CScnItemModel**>(reinterpret_cast<u8*>(widget) + 0x98);
            if (notifier == NULL) {
                continue;
            }
            int armed = 0;
            u32 flag6C = *reinterpret_cast<u32*>(reinterpret_cast<u8*>(widget) + 0x6C);
            if ((flag6C & 0x10000000) != 0 && p != NULL) {
                armed = 1;
            }
            notifier->vfunc88(armed);
        }
    }
}

/* func_80186D20: per-row sync of the arts-select container against the bdat
   table. For each valid row: run the availability check (func_80186A70);
   spawn the map object when the slot is empty and available, or tear down
   (flag + clear) when it is occupied but no longer available. Occupied rows
   then optionally apply a state id read from the bdat columns, gated on the
   global flag words lbl_eu_80663E24/28 and a second availability window. */
void func_80186D20(void* p) {
    void* bdat = lbl_eu_806640B0;
    const char* gateCol;
    const char* cols;
    MapProxy** slots;
    u16* slotFlags;
    void* curTable;
    s32 end;
    s32 row;

    row = func_8003B41C(bdat);
    end = row + func_8003B1EC(bdat);
    cols = (const char*)lbl_eu_805038C8;
    slots = reinterpret_cast<MapProxy**>(p) + row;
    slotFlags = reinterpret_cast<u16*>(reinterpret_cast<u8*>(p) + row * 2);
    // Retail hoists the availability-check's last column pointer into a
    // callee-saved register ahead of the loop.
    gateCol = cols + 0xe4;
    for (; row < end; row++, slots++, slotFlags++) {
        int avail = func_80186A70(p, row, cols + 0xa0, cols + 0xac,
                                  cols + 0xb9, cols + 0xc6, cols + 0xd0,
                                  cols + 0xda, gateCol);
        MapProxy* entry = *slots;
        if (entry == NULL && avail != 0) {
            func_801866F0(reinterpret_cast<MapProxy**>(p), row);
        }
        if (*slots != NULL && avail == 0) {
            (*slots)->field_68 |= 0x40;
            *slots = NULL;
            slotFlags[0x80] = 0;
        }
        if (*slots != NULL) {
            // Bits 6/10 of the global flag word gate the state refresh.
            int flag = ((((lbl_eu_80663E24 >> 6) & 1) |
                         ((lbl_eu_80663E24 >> 10) & 1)) != 0);
            curTable = lbl_eu_806640B0;
            u8 hasName = (u8)getBdatStringColumnValue(curTable, cols + 0x45, row);
            if (hasName != 0) {
                (*slots)->unk158(!flag); // retail slot 0x158, takes !flag
            }
            int doState = 1;
            if (flag != 0) {
                // Busy bit in the second flag word blocks the whole refresh.
                if ((lbl_eu_80663E28 & 0x01000000) == 0) {
                    doState = 0;
                }
            }
            if (doState) {
                u8 stateId = (u8)getBdatStringColumnValue(curTable, cols + 0x4b, row);
                if (stateId != 0 &&
                    func_80186A70(p, row, cols + 0x52, cols + 0x5e,
                                  cols + 0x6b, cols + 0x78, cols + 0x82,
                                  cols + 0x8c, cols + 0x96) != 0 &&
                    stateId != slotFlags[0x80]) {
                    func_800BE12C((u8*)*slots, stateId, 1, -1, 1);
                    slotFlags[0x80] = stateId;
                }
            }
        }
    }
}
void func_80186D20_UNUSED(void* p) {
    (void)p;
}

// func_80186C7C: notify every armed widget in the arts-select container.
// For each of the 64 slots: if the slot holds a widget with a notifier, call
// the notifier's vtable+0x88 entry with 1 when the widget's 0x6C flag bit
// 0x10000000 is set AND the argument is non-zero, else 0. The container
// (retail leaves r3 untouched on return)
