// Auto-scaffolded catalog TU for kyoshin/cf/CfMapItemManager
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/cf/CBattleManagerApi.hpp"
#include "kyoshin/harness_catalog.hpp"
#include "monolib/scn/CScnTimeApi.hpp"

#include "kyoshin/cf/CfGameManagerData.hpp"
#include <math.h>
#include "monolib/math.hpp"

extern unsigned long lbl_eu_806640C0;
extern unsigned long lbl_eu_806640D0;

// ---------------------------------------------------------------------------
// Type context for the field map item manager (retail symbol names).
//
// The manager's item array starts at object offset 0x00 and overlaps the
// vtable pointer: slot 0 is never stored into (mCount is 1-based), so the
// vtable stays intact.  Each slot is a 0x1C-byte CfMapItem record.
// ---------------------------------------------------------------------------

// C-linkage runtime imports (retail symbol names - keep linkage/signatures verbatim)
extern "C" void* func_802B262C();
extern "C" void* func_8003AA34();
extern "C" u32 func_8003B1EC(void* file);
extern "C" u32 func_8003B41C(void* file);
// getBdatStringColumnValue comes from kyoshin/code_801862C0.hpp (harness_catalog).
extern "C" u32 func_8009CF8C(u32 index);
extern "C" void func_802808AC(s32 mode);

// Cross-TU imports for func_801742D4 (mangled retail symbols; headers are
// outside this session's writable scope).
extern "C" bool func_80083118__Q22cf13CfGameManagerFv(s32 self);
extern "C" void func_80086E6C__Q22cf13CfGameManagerFv(int self);
extern "C" void* getPlayer__Q22cf13CfGameManagerFi(s32 idx);
// These CfGameManager helpers ignore their incoming r3 in retail; they are
// declared argument-less so MWCC leaves the caller's r3 untouched (matching
// the stale-register chain in the retail asm).
extern "C" int func_80084BF4__Q22cf13CfGameManagerFv();
extern "C" int func_8008585C__Q22cf13CfGameManagerFv();
extern "C" int func_80085840__Q22cf13CfGameManagerFv();
extern "C" u8* func_8016FE34(void* obj);
extern "C" bool func_800FF8B0();
extern "C" bool func_80251550();
extern "C" void* func_801586D4(unsigned short id);
extern "C" bool func_802B37F4();
extern "C" void* func_801351C4(int idx);
extern "C" void func_8009D018(unsigned long index, unsigned long value);
extern "C" int func_80140E00(int arg1, int arg2, int arg3, int arg4);
extern "C" void func_801BFC38__Q22cf10CfSoundManFUlUlUlUlf(unsigned long a, unsigned long b, unsigned long c, unsigned long d, float e);
// Extra imports for func_80173CA0.
extern "C" int CfRes_getD80Flag();
extern "C" u32 func_8016DF2C();
extern "C" u32 func_80086DBC__Q22cf13CfGameManagerFv();
extern "C" u32 func_800822F4__Q22cf13CfGameManagerFv();
extern "C" u32 func_80082354__Q22cf13CfGameManagerFv(u32 cond);
extern "C" int func_8020971C(u16 id);
extern "C" u32 func_802B2894(u8* self, const u32* src, u16 value);

// sdata2 constants used by func_80173CA0's range thresholds.
extern f32 lbl_eu_806677B8;
extern f32 lbl_eu_806677BC;
extern f32 lbl_eu_806677C0;
extern f32 lbl_eu_806677C4;
extern f32 lbl_eu_806677C8;
extern f32 lbl_eu_806677CC;
extern f32 lbl_eu_806677D0;
extern u32 lbl_eu_806640A8;   // bdat table handle
extern f32 lbl_eu_806677D4;
extern f32 lbl_eu_806677D8;
extern f32 lbl_eu_80667790;

// func_80174C98 / func_80174B4C: single unified decls live in
// kyoshin/cf/CfMapItemManager.hpp (owner header).
#include "kyoshin/cf/CfMapItemManager.hpp"

// Read-only view of the player object up to the position getter at vtable
// slot 0xAC (returns ml::CVec3*).
class CfPlayerPosView {
public:
    virtual void _v000();
    virtual void _v004();
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
    virtual void _v09C();
    virtual void _v0A0();
    virtual void _v0A4();
    virtual void _v0A8();
    virtual ml::CVec3* getPos();  // vtable +0xAC
};

// External data (retail linker symbols)
extern u8 lbl_eu_80664298;
extern int lbl_eu_80664184;
extern f32 lbl_eu_80667780;
extern f32 lbl_eu_80667794;
extern f32 lbl_eu_806677A8;
extern f64 lbl_eu_806677B0;
extern f32 lbl_eu_806677E8;
extern f32 lbl_eu_806677E4;
extern f32 lbl_eu_806677EC;
extern f64 lbl_eu_806677F0;
extern char lbl_eu_805033C0[];

// One map-item record; the manager's item array is 0x1C-byte strided.
struct CfMapItem {
    s16 field_00;              // 0x00
    s16 field_02;              // 0x02 type selector (1..3), indexes data[]
    union {
        u32 field_04;          // 0x04 raw word / scene-object pointer
        u8 data[4];            // 0x04 per-type bytes (data[1..3])
    };
    u32 field_08;              // 0x08
    union {
        u32 field_0C;          // 0x0C
        f32 field_0C_f;        // 0x0C (float view)
    };
    union {
        u32 field_10;          // 0x10
        f32 field_10_f;        // 0x10 (float view)
    };
    f32 field_14;              // 0x14
    u16 field_18;              // 0x18
    s16 field_1A;              // 0x1A
};

// Buffer filled by CfMapItemManagerIf::getMapItem (vt +0x10 slot).
struct MapItemBuffer {
    u32 field_00;              // 0x00
    u32 field_04;              // 0x04: pos x
    u32 field_08;              // 0x08: pos y
    u32 field_0C;              // 0x0C: pos z
    f32 field_10;              // 0x10
    u16 field_14;              // 0x14
    s16 field_16;              // 0x16
    u32 field_18;              // 0x18
};

// Cast-only view of the manager's retail vtable (RTTI-on layout: typeinfo @0,
// toffset @4, first declared virtual @+0x08). Never constructed.
class CfMapItemManagerIf {
public:
    virtual void v008();
    virtual bool unk00C();
    virtual int getMapItem(int row, MapItemBuffer* buf);  // vtable +0x10
    virtual void* getBdatTable();                         // vtable +0x14
};

// Field map item manager.  The item array starts at offset 0x00 and overlaps
// the vtable pointer: slot 0 is never stored into (mCount is 1-based).
class CfMapItemManager {
public:
    // 0x00: item array overlaps the vtable pointer (slot 0 is never stored
    // into - mCount is 1-based - so the vtable stays intact).
    CfMapItem mItems[512];       // 0x00 (0x1C * 512 = 0x3800)
    u8 field_3800[4];             // 0x3800
    u16 mCount;                   // 0x3804
    u16 field_3806;               // 0x3806
};

void __ct__cf_CfMapItemManager(){}

// __dt__801732F8: deleting destructor - free self when mode > 0, return self.
extern "C" void __dl__FPv(void*);
extern "C" void* __dt__801732F8(void* self, int mode) {
    if (self != 0 && mode > 0) {
        __dl__FPv(self);
    }
    return self;
}

// Clear every loaded map item (releasing each through the hikari-item
// manager singleton), then reset the manager's bookkeeping and item array.
extern "C" void func_802B2938(void* mgr, u32 handle);

void func_80173338(CfMapItemManager* self) {
    u32 zero = 0;
    for (u32 i = 1; i < self->mCount; i++) {
        u32* handle = &self->mItems[i].field_04;
        u32 h = *handle;
        if (h != 0) {
            func_802B2938(func_802B262C(), h);
            *handle = zero;
        }
    }
    self->mCount = 0;
    self->field_3806 = 0;
    // Zero the whole item array but keep the vtable pointer at +0 intact.
    memset(&self->mItems[0].field_04, 0, 0x3800);
}

// Column-name scratch keys and sdata2 constants used by func_801733C0.
extern u8 lbl_eu_80662420[8];      // "itm?Per" key (byte 3 rewritten per pick)
extern char lbl_eu_805033C0[];     // bdat column-name blob
extern f32 lbl_eu_80667784;
extern f32 lbl_eu_80667788;
extern f32 lbl_eu_8066778C;
extern f32 lbl_eu_80667798;
// lbl_eu_80667790 (line ~70) and lbl_eu_8066A210 come from earlier decls/headers.
// s16 -> float conversion magic double (0x4330000080000000): defining it lets
// MWCC's constant pool reuse the retail symbol for the implicit conversions
// instead of emitting a TU-local @N label (CFloorMap.cpp idiom).
extern const double lbl_eu_806677A0 = 0x4330000080000000ll;

// Random-angle table filler (monolib scn unit; defining TU code_800A3B24.cpp).
// Retail call site uses the plain unmangled linker name.
extern "C" void func_800A3B24(ml::CVec3* out, int seed);
// Segment ground-probe helper (defining TU code_800A3B24.cpp; retail call site
// passes a scene-query id as the third argument).
extern "C" int func_800A7094(ml::CVec3* a, ml::CVec3* b, int query, float f,
                             float g);

// BDAT cell accessor: raw word in, consumed through its signed/unsigned half.
union BdatCell {
    u32 raw;
    s16 s;
    u16 u;
    u8 b;
};

#pragma push
#pragma auto_inline off
int func_801733C0(CfMapItemManager* self, u32 row, void* c, int pick) {
    MapItemBuffer* buf = static_cast<MapItemBuffer*>(c);

    void* table = reinterpret_cast<CfMapItemManagerIf*>(self)->getBdatTable();
    // Build the "itm<pick>Per" weight-column key for this lottery slot.
    char* key = (char*)lbl_eu_80662420;
    char* strBase = lbl_eu_805033C0;
    key[3] = '1' + pick;

    // Spawn-offset columns (x/y/z) arrive as signed shorts converted to float.
    ml::CVec3 offs;
    BdatCell cellX;
    cellX.raw = getBdatStringColumnValue(table, strBase, row);
    offs.x = cellX.s;

    BdatCell cellY;
    cellY.raw = getBdatStringColumnValue(table, strBase + 5, row);
    offs.y = cellY.s;

    BdatCell cellZ;
    cellZ.raw = getBdatStringColumnValue(table, strBase + 0xa, row);
    offs.z = cellZ.s;

    BdatCell cellCount;
    cellCount.raw = getBdatStringColumnValue(table, strBase + 0xf, row);
    u32 count = cellCount.u;

    BdatCell cellFlag;
    cellFlag.raw = getBdatStringColumnValue(table, strBase + 0x16, row);
    u8 flag = cellFlag.b;

    BdatCell cellSel;
    cellSel.raw = getBdatStringColumnValue(table, key, row);
    u32 sel = cellSel.u;

    // Clear the previous lottery-slot field, insert the new pick, and drop the
    // "position resolved" bit.
    u32 flags = buf->field_18;
    flags = (flags & ~0x7000u) | ((pick << 17) & 0x7000u);
    flags &= ~0x40000u;
    buf->field_18 = flags;

    if (sel == 0) {
        // No weight for this pick: fall back to the plain itm1Per column.
        lbl_eu_80662420[3] = '1';
        cellSel.raw = getBdatStringColumnValue(table, key, row);
        sel = cellSel.u;
        if (sel != 0) {
            buf->field_18 &= ~0x7000u;
        }
    }

    if (sel != 0) {
        // Try up to 16 randomized offsets around the spawn point until a
        // ground probe succeeds.
        int placed = 0;
        // Loop-invariant spread/sin constants; retail caches these in
        // callee-saved f27-f31 across the probe calls.
        const f32 sinA = lbl_eu_80667788;
        const f32 sinB = lbl_eu_8066A210;
        const f32 sinC = lbl_eu_8066778C;
        const f32 angStep = lbl_eu_80667790;
        const f32 spread = lbl_eu_80667784;
        ml::CVec3 rnd;      // raw random offset
        ml::CVec3 scaled;   // random offset scaled by the spread factor
        ml::CVec3 center;   // column offsets as a vector
        ml::CVec3 pos;      // candidate position
        ml::CVec3 cur;      // accepted position
        ml::CVec3 probe;    // probe segment endpoint
        for (int i = 0; i < 16; i++) {
            func_800A3B24(&rnd, count * 100);
            scaled = rnd * spread;
            center.set(scaled);
            pos = center + offs;
            cur = pos;
            probe = pos;
            if (flag == 0) {
                // No facing flag: accept whatever offset was drawn.
                placed = 1;
                continue;
            }
            float ang;
            if (reinterpret_cast<CfMapItemManagerIf*>(self)->unk00C()) {
                ang = lbl_eu_80667780;
            } else {
                ang = nw4r::math::SinFIdx(sinC * (sinA * sinB));
            }
            probe.y -= angStep;
            if (func_800A7094(&probe, &cur, 0x4a05, lbl_eu_80667794, ang)) {
                placed = 1;
                break;
            }
            // Shrink the random-offset table window between attempts.
            count = ((count << 4) - count) >> 4;
        }

        // Snap to the ground height when the object supports it.
        if (reinterpret_cast<CfMapItemManagerIf*>(self)->unk00C() && placed == 0) {
            placed = 1;
            cur = offs;
            buf->field_18 |= 0x2000;
        }

        if (placed != 0) {
            // Commit the resolved position: y is lifted by the ground offset,
            // x/z are transferred as raw bit patterns.
            cur.y += lbl_eu_80667798;
            buf->field_08 = *(u32*)&cur.y;

            buf->field_04 = *(u32*)&cur.x;
            buf->field_0C = *(u32*)&cur.z;
            buf->field_14 = (u16)sel;
            buf->field_00 = 0;
            buf->field_18 =
                (buf->field_18 | ((row >> 12) & 0xFFFu)) & ~0x38000u;
            buf->field_10 = lbl_eu_80667780;
            buf->field_16 = 0;
        } else {
            reinterpret_cast<CfMapItemManagerIf*>(self)->unk00C();
            sel = 0;
        }
    }

    return (u16)sel;
}
#pragma pop

int func_801737CC(void* self) { return 0; }

// sdata scratch key: "itm?Per" - byte 3 is rewritten each iteration with
// '1'+i to select lottery weight column itm1Per..itm8Per.
extern u8 lbl_eu_80662428[8];

// Weighted lottery over up to 8 columns: roll mtRand(100), walk cumulative
// weights from the bdat row, and forward the winning column index.
#pragma push
#pragma auto_inline off
int func_801737D4(CfMapItemManager* self, u32 row, CfMapItem* item) {
    // Declaration order controls callee-saved register assignment; this
    // ordering reproduces retail's r26-r31 allocation.
    int pick;
    int rnd;
    CfMapItemManagerIf* iface;
    void* table;
    int i;
    int total;
    // Column values are consumed as their low byte; keeping the raw return
    // in a union reproduces retail's stw-then-lbz memory temp.
    union {
        u32 raw;
        u8 b;
    } col;

    total = 0;
    rnd = ml::math::mtRand(100);
    pick = 0;
    iface = reinterpret_cast<CfMapItemManagerIf*>(self);
    table = iface->getBdatTable();
    for (i = 0; i < 8; i++) {
        lbl_eu_80662428[3] = '1' + i;
        col.raw = getBdatStringColumnValue(table, (const char*)lbl_eu_80662428, row);
        int next = col.b + total;
        if (rnd >= total && rnd < next) {
            pick = i;
            break;
        }
        total = next;
    }
    return func_801733C0(self, row, item, pick);
}
#pragma pop

extern "C" void func_80173894(void* self, u32 b, void* c) { func_801733C0((CfMapItemManager*)self, b, c, 0); }

extern "C" u32 func_8017389C() { return (u32)lbl_eu_806640C0; }

extern "C" u32 func_801738A4() { return (u32)lbl_eu_806640D0; }

// func_801738AC - collect the current map's item entries into mItems.  When
// lbl_eu_80664298 (fixed-position flag) is set, synthesize each entry from
// ml::CVec3::zero instead of asking the vt callback.
void func_801738AC(CfMapItemManager* self) {
    func_8003AA34();
    // Declaration order drives callee-saved coloring (first -> highest reg):
    // retail maps zeroW>r30, cols>r29, table>r28, end>r27, row>r26,
    // count>r25, i>r24.
    void* table = reinterpret_cast<CfMapItemManagerIf*>(self)->getBdatTable();
    s32 end;
    s32 row = (s32)func_8003B41C(table);
    end = row + (s32)func_8003B1EC(table);    f32 one;
    f32 scale = lbl_eu_80667780;
    if (lbl_eu_80664298 != 0) {
        // Runtime-shaped int->double conversion via the 0x43300000 scratch
        // double, minus the shared magic constant (retail keeps the full
        // store/lfd/fsub sequence).
        union { double d; u32 w[2]; } cv;
        cv.w[1] = 1;
        cv.w[0] = 0x43300000;
        scale = lbl_eu_806677A8 * (lbl_eu_80667794 * (cv.d - lbl_eu_806677B0));
    }
    self->mCount = 1;
    const char* cols = lbl_eu_805033C0;
    const u32* zeroW = reinterpret_cast<const u32*>(&ml::CVec3::zero);
    one = lbl_eu_80667780;
    MapItemBuffer buf;

    while (row < end) {
        // Column value consumed as its low byte.
        u8 count = (u8)getBdatStringColumnValue(table, cols + 0x1b, row);
        u16 i = 0;
        buf.field_10 = one;
        buf.field_14 = 0;
        while (i < count) {
            if (lbl_eu_80664298 != 0) {
                // Fixed-position path: synthesize flags from the row number
                // (top 12 bits) plus the active bit, zero the position.
                u32 fl = buf.field_18 & 0xFFFF;
                fl &= ~0x40000u;
                fl |= (row << 20) & 0xFFF00000u;
                fl |= 0x10000u;
                buf.field_04 = zeroW[0];
                buf.field_08 = zeroW[1];
                buf.field_0C = zeroW[2];
                buf.field_16 = 0;
                buf.field_14 = 0;
                buf.field_00 = 0;
                buf.field_18 = fl;
                buf.field_10 = scale;

                u16 n2 = self->mCount;
                CfMapItem* item = &self->mItems[n2];
                self->mCount = n2 + 1;
                item->field_04 = buf.field_00;
                item->field_08 = buf.field_04;
                item->field_0C = buf.field_08;
                item->field_10 = buf.field_0C;
                item->field_14 = buf.field_10;
                item->field_18 = buf.field_14;
                item->field_1A = buf.field_16;
                ((u32*)item)[7] = buf.field_18;
            } else if (reinterpret_cast<CfMapItemManagerIf*>(self)->getMapItem(row, &buf)) {
                u16 n2 = self->mCount;
                CfMapItem* item = &self->mItems[n2];
                self->mCount = n2 + 1;
                item->field_04 = buf.field_00;
                item->field_08 = buf.field_04;
                item->field_0C = buf.field_08;
                item->field_10 = buf.field_0C;
                item->field_14 = buf.field_10;
                item->field_18 = buf.field_14;
                item->field_1A = buf.field_16;
                ((u32*)item)[7] = buf.field_18;
            }
            i++;
        }
        row++;
    }
}

// func_80173AEC - collect item entries, filtering by the map's item-type
// column and the flag table at row + 0x2b9c.
void func_80173AEC(CfMapItemManager* self) {
    func_8003AA34();
    CfMapItemManagerIf* iface = reinterpret_cast<CfMapItemManagerIf*>(self);
    void* table = iface->getBdatTable();
    s32 row = (s32)func_8003B41C(table);
    u32 cnt = func_8003B1EC(table);
    self->mCount = 1;
    s32 end = row + (s32)cnt;
    f32 one = lbl_eu_80667780;

    while (row < end) {
        u8 type = (u8)getBdatStringColumnValue(table, &lbl_eu_805033C0[0x22], row);
        if (type == lbl_eu_80664184 && func_8009CF8C((u32)(row + 0x2b9c)) == 0) {
            u8 count = (u8)getBdatStringColumnValue(table, &lbl_eu_805033C0[0x1b], row);
            MapItemBuffer buf;
            buf.field_10 = one;
            buf.field_14 = 0;
            for (u16 i = 0; i < count; i++) {
                if (iface->getMapItem(row, &buf)) {
                    CfMapItem* item = &self->mItems[self->mCount];
                    item->field_04 = buf.field_00;
                    item->field_08 = buf.field_04;
                    item->field_0C = buf.field_08;
                    item->field_10 = buf.field_0C;
                    item->field_14 = buf.field_10;
                    item->field_18 = buf.field_14;
                    item->field_1A = buf.field_16;
                    *(u32*)&self->mItems[self->mCount + 1].field_00 = buf.field_18;
                    self->mCount++;
                }
            }
        }
        row++;
    }
}

// Toggle the item-record flag on the hikari-item manager singleton, passing
// the arg through (retail: getter result in r3 feeds the second call).
extern "C" void* func_802B262C();
extern "C" void func_802B2AB8(void* self, u32 enable);

extern "C" void func_80173C6C(void* self, void* arg) {
    func_802B2AB8(func_802B262C(), (u32)arg);
}

// Buffer view beginning at the record's handle word (base+4): the early
// timer/position section of func_80173CA0 addresses the record this way.
struct MapItemWork {
    u32 field_00;              // hikari scene-object handle
    f32 field_04;              // pos x
    f32 field_08;              // pos y
    f32 field_0C;              // pos z
    f32 field_10;              // scale / respawn timer
    u16 field_14;              // kind bits
    s16 field_16;              // respawn countdown
    u32 field_18;              // state flags
};

// Cast view of one map-item record including the trailing flags word.
struct MapItemRec {
    s16 field_00;
    s16 field_02;
    union { u32 field_04; };
    ml::CVec3 field_08;                   // 0x08: world position
    union { f32 field_14_f; u32 field_14; };
    u16 field_18;
    s16 field_1A;
    u32 field_1C;
};

// vtable gate view: slot +0xC on the manager returns the "event active" flag.
class MgrGateView {
public:
    virtual void g008();
    virtual bool g00C();
};

// Release helpers shared by func_80173CA0's despawn paths.  Two flavors exist
// in retail: one routes the scene-object handle through the hikari-item
// manager singleton, the other calls func_802B37F4 directly.
static inline void despawnItemHandle(CfMapItemManager* mgr, MapItemRec* rec) {
    if (rec->field_04 != 0) {
        func_802B2938(func_802B262C(), rec->field_04);
        rec->field_04 = 0;
        if (mgr->field_3806 != 0) mgr->field_3806--;
    }
}
static inline void despawnItemDirect(CfMapItemManager* mgr, MapItemRec* rec) {
    if (rec->field_04 != 0) {
        func_802B37F4();
        rec->field_04 = 0;
        if (mgr->field_3806 != 0) mgr->field_3806--;
    }
}

// func_80173CA0 - per-frame map item update.  Walks every collected item,
// ticks respawn timers, evaluates bdat gating columns (area / clock / story /
// season), spawns or releases the associated hikari item, and finally picks
// the closest eligible item (or an exact proximity hit) as the return index.
// Retail resolves callers to the unmangled symbol.
extern "C" int func_80173CA0(CfMapItemManager* self, ml::CVec3* pos) {
    func_8003AA34();
    CfMapItemManagerIf* iface = reinterpret_cast<CfMapItemManagerIf*>(self);
    void* table = iface->getBdatTable();
    u32 clock = func_8016DF2C();
    u16 curArea = func_80086DBC__Q22cf13CfGameManagerFv();
    u16 curMap = func_800822F4__Q22cf13CfGameManagerFv();

    const char* cols = lbl_eu_805033C0;
    f32 bestDist = lbl_eu_806677BC;
    f32 k80 = lbl_eu_80667780;
    f32 dyLimit = bestDist;
    f32 kC4 = lbl_eu_806677C4;
    f32 kC8 = lbl_eu_806677C8;
    f32 kD0 = lbl_eu_806677D0;
    f32 kCC = lbl_eu_806677CC;
    f32 kC0 = lbl_eu_806677C0;
    f32 kB8 = lbl_eu_806677B8;
    int resultIdx = 0;
    int nearestIdx = 0;

    for (u32 i = 1; i < self->mCount; i++) {
        MapItemWork* work = reinterpret_cast<MapItemWork*>(&self->mItems[i].field_04);
        work->field_18 &= ~0x30000;
        if (work->field_14 == 0) {
            // Timer-only slot: decay field_10 back toward the default scale.
            if ((work->field_18 & 0x8000) == 0) continue;
            CfRes_getD80Flag();
            f32 dec = func_80496288(lbl_eu_80663E14);
            f32 t = work->field_10;
            if (t > k80) {
                work->field_10 = t - dec;
            } else {
                work->field_10 = k80;
                if (func_801737D4(self, work->field_18 >> 20,
                                  reinterpret_cast<CfMapItem*>(work)) == 0) {
                    work->field_10 = kB8;
                }
            }
            continue;
        }

        u32 row = work->field_18 >> 20;
        if (row == 0) continue;
        work->field_18 &= ~0x380000;

        // Player-relative offset; xz magnitude gates the far despawn.
        ml::CVec3 diff;
        diff.x = work->field_04 - pos->x;
        diff.y = work->field_08 - pos->y;
        diff.z = work->field_0C - pos->z;
        f32 dyAbs = (f32)__fabs((f64)diff.y);
        f32 dz2 = diff.z * diff.z;
        f32 distSq = diff.x * diff.x + dz2;
        MapItemRec* rec = reinterpret_cast<MapItemRec*>(&self->mItems[i]);
        if (distSq > kC0 || dyAbs > dyLimit) {
            despawnItemHandle(self, rec);
            continue;
        }

        u8 areaCol = (u8)getBdatStringColumnValue(table, cols + 0x28, row);
        u8 areaCol2 = (u8)getBdatStringColumnValue(table, cols + 0x30, row);
        if (areaCol != 0 && areaCol != curArea) {
            rec->field_1C |= 0x400;
            despawnItemDirect(self, rec);
            continue;
        }
        if (areaCol2 != 0 && areaCol2 != (u16)clock) {
            rec->field_1C |= 0x400;
            despawnItemDirect(self, rec);
            continue;
        }

        u16 condCol = (u16)getBdatStringColumnValue(table, cols + 0x38, row);
        if (condCol != 0) {
            u8 reqCol = (u8)getBdatStringColumnValue(table, cols + 0x40, row);
            if (reqCol != func_80082354__Q22cf13CfGameManagerFv(condCol)) {
                rec->field_1C |= 0x800;
                despawnItemDirect(self, rec);
                continue;
            }
        }

        if (!iface->unk00C()) {
            // Story-flag gate: item hidden until the flag engine reports it.
            u16 storyCol = (u16)getBdatStringColumnValue(table, cols + 0x4c, row);
            if (storyCol != 0 && func_8020971C(storyCol) == 0) {
                despawnItemDirect(self, rec);
                continue;
            }
        }

        u16 seasonLo = (u16)getBdatStringColumnValue(table, cols + 0x56, row);
        u16 seasonHi = (u16)getBdatStringColumnValue(table, cols + 0x60, row);
        if (curMap < seasonLo || curMap > seasonHi) {
            rec->field_1C |= 0x1000;
            despawnItemDirect(self, rec);
            continue;
        }

        rec->field_1C |= 0x8000;
        if (dyAbs > kC4 || distSq > kC8) {
            despawnItemHandle(self, rec);
            continue;
        }

        // Spawn a fresh hikari item, or tick down an armed respawn counter.
        if (rec->field_04 == 0) {
            bool occupied = iface->unk00C();
            rec->field_04 = func_802B2894(reinterpret_cast<u8*>(func_802B262C()),
                                          reinterpret_cast<u32*>(&rec->field_08.x),
                                          !occupied);
            rec->field_1A = 3;
            self->field_3806 = self->field_3806 + 1;
        } else if (rec->field_1A > 0) {
            rec->field_1A--;
        }

        if (rec->field_1A > 0) continue;
        rec->field_1C |= 0x4000;
        if (dyAbs < kCC && distSq < kD0) {
            resultIdx = i;
            continue;
        }
        if (distSq > bestDist) {
            bestDist = distSq;
            nearestIdx = i;
        }
    }

    // Too many active items: drop the farthest candidate seen this frame.
    if (self->field_3806 > 0x32 && nearestIdx != 0) {
        MapItemRec* rec = reinterpret_cast<MapItemRec*>(&self->mItems[nearestIdx]);
        despawnItemHandle(self, rec);
    }
    return resultIdx;
}

// Minimal views for the battle-manager singleton list walk.
struct CBmSingletonView {
    u8 pad_00[8];
    u32* field_8;              // sentinel node of the instance ring
};
struct CBmNode {
    u32* next;
};

// Loader object reached from func_8016FE34: +4 points at a sub-object whose
// vtable slot +0x30 returns a pointer to the current flag word.
class ProbeSub {
public:
    virtual void p000();
    virtual void p004();
    virtual void p008();
    virtual void p00C();
    virtual void p010();
    virtual void p014();
    virtual void p018();
    virtual void p01C();
    virtual void p020();
    virtual void p024();
    virtual void p028();
    virtual void* p030();  // vtable +0x30
};
class ProbeObj {
public:
    u8 pad_00[4];
    ProbeSub* field_4;
};

// func_801742D4 - pick-up handling: find the map item under the player,
// gate it on battle/story state, then either clear the record or convert it
// to a timed respawn entry from the bdat column at +0x6a.
void func_801742D4(CfMapItemManager* self) {
    // Declaration order drives callee-saved coloring; retail maps
    // pv(player/gate)>r28, self>r29, idx/kind>r30, pick>r31, loader>r27.
    u32 pv;                 // player pointer, later the battle-gate word
    MapItemRec* pick;       // doubles as the final-ok flag (retail li r31,1)
    int idx;

    pv = (u32)getPlayer__Q22cf13CfGameManagerFi(0);
    if (pv == 0) return;
    if (lbl_eu_80663E24 & 0x00400000) return;          // bit 22: cutscene-ish gate
    if (func_80084BF4__Q22cf13CfGameManagerFv() != 0) return;
    if (lbl_eu_80663E24 & 0xAFA40000) return;
    if (func_8008585C__Q22cf13CfGameManagerFv() != 0) return;
    if (func_80085840__Q22cf13CfGameManagerFv() == 0) return;

    // Player feet position + small Y offset, fed to the item lookup.
    ml::CVec3* pp = ((CfPlayerPosView*)pv)->getPos();
    ml::CVec3 pos;
    pos.x = pp->x;
    pos.y = pp->y;
    pos.z = pp->z;
    pos.y += lbl_eu_80667790;
    idx = func_80173CA0(self, &pos);
    if (idx == 0) return;
    if (!func_80083118__Q22cf13CfGameManagerFv(0)) return;
    u8* loader = func_8016FE34((void*)pv);
    if (loader == 0) return;
    if (func_800FF8B0() != 0) return;
    if (func_80251550() != 0) return;

    // Battle-state gate: the singleton's instance ring is walked once; a
    // non-zero walk result skips the category probes and aborts the pick-up.
    void* bm = getInstance__Q22cf14CBattleManagerFv();
    if (bm != 0) {
        CBmNode* sentinel = (CBmNode*)((CBmSingletonView*)bm)->field_8;
        CBmNode* cur = *(CBmNode**)sentinel;
        u32 count = 0;
        do {
            cur = *(CBmNode**)cur;
            count++;
        } while (cur != sentinel);
        // Retail-shaped nonzero test on the walked length.
        pv = ((u32)(-(s32)count | count)) >> 31;
    }
    pick = 0;
    if (pv == 0) {
        // Probe the loader object's flag words for categories 3 then 4; both
        // must fail to abort the pick-up.
        pv = 1;
        ProbeObj* po = (ProbeObj*)loader;
        u32 flagv = *(u32*)po->field_4->p030();
        if (!func_80174C98(loader, &flagv, 3)) {
            flagv = *(u32*)po->field_4->p030();
            if (!func_80174C98(loader, &flagv, 4)) pv = 0;
        }
        if (pv != 0) pick = (MapItemRec*)1;
    }
    if (pick == 0) return;

    MapItemRec* rec = pick = (MapItemRec*)&self->mItems[idx];
    unsigned short kind = rec->field_18;
    void* helpRow = func_801586D4(kind);
    if (helpRow == 0) return;
    if (((*(u32*)helpRow >> 12) & 0xF) == 0xa) {
        *((u8*)lbl_eu_80664A10 + 0x14) = 1;
    }
    if (rec->field_04 != 0) {
        func_802B37F4();
        rec->field_04 = 0;
        if (self->field_3806 != 0) self->field_3806--;
    }
    func_801BFC38__Q22cf10CfSoundManFUlUlUlUlf(0, 0x45, 0, 0, lbl_eu_806677D4);
    if (((MgrGateView*)self)->g00C()) {
        // No event: wipe the record and bump the flag-table reset counter.
        func_801351C4(kind);
        func_8009D018((rec->field_1C >> 20) + 0x2b9c, 1);
        rec->field_18 = 0;
        rec->field_04 = 0;
        rec->field_14_f = lbl_eu_80667780;
        rec->field_1A = 0;
        rec->field_1C = (rec->field_1C & 0xFFFF) & ~0x2000;
    } else {
        // Event-driven respawn: read the timer column and arm the record.
        func_801351C4(kind);
        u8 secs = (u8)getBdatStringColumnValue((void*)lbl_eu_806640A8, &lbl_eu_805033C0[0x6a], lbl_eu_80664184);
        if (secs != 0) {
            // u32->double via the 0x43300000 scratch double.
            union { double d; u32 w[2]; } cv;
            cv.w[0] = 0x43300000;
            cv.w[1] = secs;
            rec->field_18 = 0;
            rec->field_04 = 0;
            rec->field_1C |= 0x10000;
            rec->field_14_f = lbl_eu_806677A8 * (lbl_eu_80667794 * (cv.d - lbl_eu_806677B0));
        } else {
            rec->field_18 = 0;
            rec->field_04 = 0;
            rec->field_14_f = lbl_eu_80667780;
            rec->field_1A = 0;
            rec->field_1C = (rec->field_1C & 0xFFFF) & ~0x2000;
        }
    }
    func_80086E6C__Q22cf13CfGameManagerFv(func_80140E00(2, kind, 0, 0));
}

int func_80174650(void* self) { return 1; }

// Init: zero the +0/+2 pair, memset the +4..+9 region, and load the two
// +0xC/+0x10 floats from the constants.
struct ItemWorkState {
    u16 field_00;              // 0x00
    u16 field_02;              // 0x02
    u8 field_04[5];            // 0x04: memset region (5 bytes)
    u8 pad_09[3];
    f32 field_0C;              // 0x0C
    f32 field_10;              // 0x10
};

void* func_80174658(ItemWorkState* self) {
    self->field_00 = 0;
    self->field_02 = 2;
    memset(self->field_04, 0, 5);
    // E4 is held in a temp across the mr r3,r31 self copy; E8 is consumed
    // directly. This is the closest shape to retail: loads land E4->f1 then
    // E8->f0, but MWCC's scheduler insists on emitting the f0 store first
    // (retail stores f1,0xC before f0,0x10); 10+ source shapes tried.
    f32 e4 = lbl_eu_806677E4;
    ItemWorkState* ret = self;
    self->field_10 = lbl_eu_806677E8;
    ret->field_0C = e4;
    return ret;
}

// Retail resolves this helper's calls through the unmangled symbol.
extern "C" s16 func_801748B8(CfMapItem* self, s16 delta);

// func_801746B4 - advance a map item's timer; when it crosses the threshold,
// move the item one "notch" (per its type) toward the mid position.
//
// Case 2 evaluates the half-range threshold (field_10 scaled by the type
// byte, halved) repeatedly instead of caching it - retail re-emits the whole
// float-to-int conversion at every use through two alternating scratch
// doubles (ca/cb), so the source mirrors that shape site by site.
extern "C" void func_801746B4(CfMapItem* self, f32 delta) {
    // Compound assignment on the parameter keeps the running value in f1.
    delta /= lbl_eu_806677EC;
    delta += self->field_0C_f;
    self->field_0C_f = delta;
    // Declared after the arithmetic; MWCC emits these two independent
    // stores in reverse declaration order.
    // Declared in reverse: MWCC assigns later-declared locals the lower
    // stack slots, and retail has ca at +8 / cb at +0x10.
    union { double d; u32 w[2]; } cb, ca;
    ca.w[0] = 0x43300000;
    cb.w[0] = 0x43300000;
    if (delta < lbl_eu_806677E8) return;
    self->field_0C_f = delta - lbl_eu_806677E8;
#define HALF(u) ((int)(self->field_10_f * (u.d - lbl_eu_806677F0)) / 2)
    switch (self->field_02) {
    case 3:
        func_801748B8(self, -2);
        break;
    case 1:
        func_801748B8(self, 4);
        break;
    case 2:
        ca.w[1] = self->data[self->field_02];
        if (self->field_00 > HALF(ca)) {
            func_801748B8(self, -2);
            u32 b = self->data[self->field_02];
            cb.w[1] = b;
            if (self->field_00 < HALF(cb)) {
                ca.w[1] = b;
                self->field_00 = HALF(ca);
            }
        } else {
            cb.w[1] = self->data[self->field_02];
            if (self->field_00 < HALF(cb)) {
                func_801748B8(self, 4);
                u32 b = self->data[self->field_02];
                ca.w[1] = b;
                if (self->field_00 > HALF(ca)) {
                    cb.w[1] = b;
                    self->field_00 = HALF(cb);
                }
            }
        }
        break;
    }
#undef HALF
}

// u32->double via the 0x43300000 exponent trick; the magic blob is
// subtracted from retail's shared sdata2 constant so the fsub reloc names
// lbl_eu_806677F0 instead of pooling a TU-local @N (MWCC_PATTERNS 7i).
// type is re-read inside each arm (retail emits one lha per arm, below the
// dispatch) - a single load hoisted above the branch makes MWCC sink the
// second scratch-double store out of the prologue.
extern "C" s16 func_801748B8(CfMapItem* self, s16 delta) {
    // High words preloaded in the prologue (cvA @sp+8, cvB @sp+0x10;
    // later-declared local takes the lower slot).
    union { double d; u32 w[2]; } cvB;
    union { double d; u32 w[2]; } cvA;
    cvA.w[0] = 0x43300000;
    cvB.w[0] = 0x43300000;
    if (delta > 0) {
        s16 type = self->field_02;
        if ((u32)(type - 1) <= 1) {
            // Types 1..2: advance; overshoot becomes the recursion delta.
            s16 nx = self->field_00 + delta;
            self->field_00 = nx;
            cvA.w[1] = self->data[type];
            int rem = nx - (int)(self->field_10_f * (cvA.d - lbl_eu_806677F0));
            if (rem > 0) {
                int t = type + 1;
                self->field_02 = t;
                if (t < 0) self->field_02 = 0;
                else if (t > 4) self->field_02 = 4;
                func_802808AC(1);
                self->field_00 = 0;
                func_801748B8(self, rem);
            }
        } else if (type == 3) {
            // Type 3 clamps at the half range instead of wrapping; the data
            // byte is loaded once and fed through each scratch double.
            s16 nx = self->field_00 + delta;
            self->field_00 = nx;
            u8 b = self->data[type];
            cvB.w[1] = b;
            int half = (int)(self->field_10_f * (cvB.d - lbl_eu_806677F0));
            if (nx - half > 0) {
                cvA.w[1] = b;
                self->field_00 = (int)(self->field_10_f * (cvA.d - lbl_eu_806677F0));
            }
        }
    } else {
        s16 type = self->field_02;
        if ((u32)(type - 2) <= 1) {
            // Types 2..3: advance; going below zero steps down a type.
            s16 nx = self->field_00 + delta;
            self->field_00 = nx;
            if (nx < 0) {
                int t = type - 1;
                self->field_02 = t;
                if (t < 0) self->field_02 = 0;
                else if (t > 4) self->field_02 = 4;
                func_802808AC(1);
                cvB.w[1] = self->data[self->field_02];
                self->field_00 = (int)(self->field_10_f * (cvB.d - lbl_eu_806677F0));
                func_801748B8(self, nx);
            }
        } else if (type == 1) {
            s16 nx = self->field_00 + delta;
            self->field_00 = nx;
            if (nx < 0) {
                self->field_00 = 0;
            }
        }
    }
    return self->field_00;
}

// Store the count-2 flag, then set the position index from the scaled
// byte at +6 divided by 2.  OPEN ITEM: the builtin (f32)(u32) conversion
// pools its 0x43300000/2^52 magic as TU-local @N instead of retail's
// lbl_eu_806677F0 sdata2 reference (MWCC_PATTERNS 7i; cf CActorParam
// UnkVirtualFunc160).  Body is otherwise byte-identical.
void func_80174AE8(CfMapItem* self) {
    self->field_02 = 2;
    f32 v = self->field_10_f * (f32)(u8)self->data[self->field_02];
    self->field_00 = (s16)((s32)v / 2);
}

void func_80174B3C(void* self, unsigned char a, unsigned char b, unsigned char c) {
    ((unsigned char*)self)[5] = a;
    ((unsigned char*)self)[6] = b;
    ((unsigned char*)self)[7] = c;
}

void func_80174B4C(){}

// Source-object interface reached through the holder's +4 member: load-range
// setup (vtbl +0x20, fixed 0x7c0 limit) followed by per-id load (vtbl +0x18).
class ICfMapItemLoad {
public:
    virtual void v00();
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void loadId(int id);   // vtbl +0x18
    virtual void v05();
    virtual void beginRange(int limit); // vtbl +0x20
};

class CfMapItemLoader {
public:
    u8 pad_00[4];                  // 0x00
    ICfMapItemLoad* mpLoad;        // 0x04
};

void func_80174C24(CfMapItemLoader* self, u32 id) {
    // Early returns keep the two unsigned compares separate (retail shape).
    if (id <= 0x3f) return;
    if (id >= 0x7c0) return;
    self->mpLoad->beginRange(0x7c0);
    self->mpLoad->loadId(id);
}

//  - id >= 0xfff or outside the [0x800,0x807] category window: raw bitmask
//    AND test.
//  - otherwise: per-category dispatch (retail jump table jumptable_eu_80531710);
//    each category ORs range equalities, category 3 mixes in recursive
//    sub-category probes.
extern "C" s32 func_80174C98(void* obj, void* outFlagsV, u32 id) {
    u32* flags = (u32*)outFlagsV;
    s32 hit;
    if (id <= 0x3f) {
        u32 v = flags[0] & 0x3F;
        return (v - id) == 0;
    }
    if (id <= 0x7bf) {
        u32 v = flags[0] & 0x7C0;
        return (v - id) == 0;
    }

    // Category dispatch: each body loads the flag word once, masks the
    // relevant field, and tests membership via the subtract-and-test-zero
    // idiom.
    switch (id) {
    case 0x800: {
        u32 low6 = flags[0] & 0x3F;
        return (low6 - 0xd) == 0 || (low6 - 0xe) == 0 || (low6 - 0xf) == 0;
    }
    case 0x801: {
        u32 low6 = flags[0] & 0x3F;
        return (low6 - 9) == 0 || (low6 - 0xa) == 0 || (low6 - 0xb) == 0;
    }
    case 0x802: {
        u32 low6 = flags[0] & 0x3F;
        return (low6 - 3) == 0 || (low6 - 4) == 0 || (low6 - 8) == 0 ||
               (low6 - 0xe) == 0 || (low6 - 5) == 0;
    }
    case 0x803: {
        // Long guarded chain: range tests mixed with recursive probes.
        u32 low6 = flags[0] & 0x3F;
        s32 hit = (low6 - 6) == 0;
        if (!hit) hit = (low6 - 7) == 0;
        if (!hit) hit = ((flags[0] & 0x7C0) - 0x1c0) == 0;
        if (!hit) {
            hit = func_80174C98(obj, flags, 9);
            if (!hit) hit = func_80174C98(obj, flags, 0xa);
            if (!hit) hit = func_80174C98(obj, flags, 0xb);
        }
        if (!hit) {
            hit = func_80174C98(obj, flags, 0x16);
            if (!hit) hit = func_80174C98(obj, flags, 0x17);
            if (!hit) hit = func_80174C98(obj, flags, 0xf);
        }
        // Re-read the flag word after the recursive probes.
        if (!hit) hit = ((flags[0] & 0x3F) - 0x13) == 0;
        if (!hit) hit = ((flags[0] & 0x3F) - 0x12) == 0;
        if (!hit) hit = ((flags[0] & 0x3F) - 0x14) == 0;
        if (!hit) hit = ((flags[0] & 0x3F) - 0x15) == 0;
        if (!hit) hit = ((flags[0] & 0x3F) - 0x18) == 0;
        if (!hit) hit = ((flags[0] & 0x3F) - 0x19) == 0;
        if (!hit) hit = ((flags[0] & 0x3F) - 0x1a) == 0;
        if (!hit) hit = ((flags[0] & 0x3F) - 0x1b) == 0;
        if (!hit) hit = ((flags[0] & 0x3F) - 0x10) == 0;
        if (!hit) hit = ((flags[0] & 0x3F) - 0xd) == 0;
        if (!hit) hit = ((flags[0] & 0x3F) - 0xf) == 0;
        if (!hit) hit = ((flags[0] & 0x3F) - 0x1f) == 0;
        return hit;
    }
    case 0x806: {
        u32 low6 = flags[0] & 0x3F;
        return (low6 - 0x10) == 0 || (low6 - 0x11) == 0;
    }
    case 0x804: {
        u32 low6 = flags[0] & 0x3F;
        return (low6 - 1) == 0 || (low6 - 2) == 0;
    }
    case 0x805: {
        u32 low6 = flags[0] & 0x3F;
        return (low6 - 0x16) == 0 || (low6 - 0x17) == 0 || (low6 - 0xf) == 0;
    }
    case 0x807: {
        // Coarse granularity list (0x80 steps of the 0x7c0 field).
        u32 mid = flags[0] & 0x7C0;
        return (mid - 0x80) == 0 || (mid - 0xc0) == 0 || (mid - 0x100) == 0 ||
               (mid - 0x140) == 0 || (mid - 0x180) == 0 || (mid - 0x1c0) == 0 ||
               (mid - 0x200) == 0 || (mid - 0x240) == 0 || (mid - 0x280) == 0 ||
               (mid - 0x2c0) == 0 || (mid - 0x300) == 0;
    }
    default:
        // Raw bitmask test for every other id.
        return (flags[0] & id) != 0;
    }
#undef MID
#undef LOW6
}

// Reset a 0x78-byte map marker record: clear tail flags, wipe the record,
// then restore the default scale (+0x5c) and counts (+0x38/+0x3a).
class CfMapMarker {
public:
    u8 field_00[0x38];
    u16 field_38;                 // 0x38
    u16 field_3A;                 // 0x3a
    u8 pad_3C[0x5C - 0x3C];
    f32 field_5C;                 // 0x5c
    u8 pad_60[0x70 - 0x60];
    u8 field_70;                  // 0x70
    u8 field_71[3];               // 0x71: cleared individually
    u32 field_74;                 // 0x74
};

// Retail keeps the tail clear as a rolled guarded bdnz byte walk; under
// unit -O4 the fill recognizer rewrites it into the aligned expansion, so
// force the size-optimized rolled form for this function only.
#pragma push
#pragma optimize_for_size on
void func_801751DC(CfMapMarker* self) {
    self->field_70 = 0;
    u8* p = self->field_71;
    u8* e = reinterpret_cast<u8*>(&self->field_74);
    if (p < e) {
        for (u32 n = (u32)(e - p); n != 0; --n) {
            *p = 0;
            p++;
        }
    }
    self->field_74 = 0;
    memset(self, 0, 0x78);
    self->field_5C = lbl_eu_806677E8;
    self->field_38 = 5;
    self->field_3A = 5;
}
#pragma pop
