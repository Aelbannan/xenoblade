// Auto-scaffolded catalog TU for kyoshin/cf/CfMapItemManager
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/harness_catalog.hpp"
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
extern "C" void* func_8003AA34();
extern "C" u32 func_8003B1EC(void* file);
extern "C" u32 func_8003B41C(void* file);
extern "C" u32 getBdatStringColumnValue(void* file, const char* column, s32 index);
extern "C" u32 func_8009CF8C(u32 index);
extern "C" void func_802808AC(s32 mode);

// External data (retail linker symbols)
extern u8 lbl_eu_80664298;
extern int lbl_eu_80664184;
extern f32 lbl_eu_80667780;
extern f32 lbl_eu_80667794;
extern f32 lbl_eu_806677A8;
extern f64 lbl_eu_806677B0;
extern f32 lbl_eu_806677E8;
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
    u32 field_04;              // 0x04
    u32 field_08;              // 0x08
    u32 field_0C;              // 0x0C
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
    virtual void v00C();
    virtual int getMapItem(int row, MapItemBuffer* buf);  // vtable +0x10
    virtual void* getBdatTable();                         // vtable +0x14
};

// Field map item manager.  The item array starts at offset 0x00 and overlaps
// the vtable pointer: slot 0 is never stored into (mCount is 1-based).
class CfMapItemManager {
public:
    union {
        void* vtable;             // 0x00
        CfMapItem mItems[512];    // 0x00 (0x1C * 512 = 0x3800)
    };
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

void func_80173338(){}

#pragma push
#pragma auto_inline off
extern "C" void func_801733C0(void* a, void* b, void* c, int d) {}
#pragma pop

int func_801737CC(void* self) { return 0; }

void func_801737D4(){}

extern "C" void func_80173894(void* self, void* b, void* c) { func_801733C0(self, b, c, 0); }

extern "C" u32 func_8017389C() { return (u32)lbl_eu_806640C0; }

extern "C" u32 func_801738A4() { return (u32)lbl_eu_806640D0; }

// func_801738AC - collect the current map's item entries into mItems.  When
// lbl_eu_80664298 (fixed-position flag) is set, synthesize each entry from
// ml::CVec3::zero instead of asking the vt callback.
void func_801738AC(CfMapItemManager* self) {
    func_8003AA34();
    CfMapItemManagerIf* iface = reinterpret_cast<CfMapItemManagerIf*>(self);
    void* table = iface->getBdatTable();
    s32 row = (s32)func_8003B41C(table);
    s32 end = row + (s32)func_8003B1EC(table);
    f32 scale = lbl_eu_80667780;
    if (lbl_eu_80664298 != 0) {
        scale = lbl_eu_806677A8 * (lbl_eu_80667794 * (f64)(u32)1);
    }
    self->mCount = 1;

    while (row < end) {
        u32 n = getBdatStringColumnValue(table, &lbl_eu_805033C0[0x1b], row);
        MapItemBuffer buf;
        u8 count = (u8)n;
        buf.field_10 = lbl_eu_80667780;
        buf.field_14 = 0;
        for (u16 i = 0; i < count; i++) {
            if (lbl_eu_80664298 != 0) {
                // Fixed-position path: zero position, synthesized flags.
                u32 v18 = buf.field_18 & 0xFFFF;
                v18 = (v18 & ~0x7C000);
                v18 = (v18 & ~0xFFF) | ((u32)(row >> 12) & 0xFFF);
                v18 |= 0x10000;
                buf.field_00 = 0;
                buf.field_04 = *(u32*)&ml::CVec3::zero.x;
                buf.field_08 = *(u32*)&ml::CVec3::zero.y;
                buf.field_0C = *(u32*)&ml::CVec3::zero.z;
                buf.field_16 = 0;
                buf.field_14 = 0;
                buf.field_18 = v18;
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
                *(u32*)&self->mItems[n2 + 1].field_00 = buf.field_18;
            } else if (iface->getMapItem(row, &buf)) {
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
                *(u32*)&self->mItems[n2 + 1].field_00 = buf.field_18;
            }
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

void func_80173CA0(){}

void func_801742D4(){}

int func_80174650(void* self) { return 1; }

void func_80174658(){}

s16 func_801748B8(CfMapItem* self, s16 delta);

// func_801746B4 - advance a map item's timer; when it crosses the threshold,
// move the item one "notch" (per its type) toward the mid position.
void func_801746B4(CfMapItem* self, f32 delta) {
    f32 v = self->field_0C_f + delta / lbl_eu_806677EC;
    self->field_0C_f = v;
    if (v < lbl_eu_806677E8) return;
    self->field_0C_f = v - lbl_eu_806677E8;
    switch (self->field_02) {
    case 3:
        func_801748B8(self, -2);
        break;
    case 1:
        func_801748B8(self, 4);
        break;
    case 2: {
        s32 half = (s32)(self->field_10_f * (f64)self->data[self->field_02]) / 2;
        if (self->field_00 > half) {
            func_801748B8(self, -2);
            s32 half2 = (s32)(self->field_10_f * (f64)self->data[self->field_02]) / 2;
            if (self->field_00 < half2) {
                self->field_00 = (s16)half2;
            }
        } else {
            s32 half2b = (s32)(self->field_10_f * (f64)self->data[self->field_02]) / 2;
            if (self->field_00 < half2b) {
                func_801748B8(self, 4);
                s32 half3 = (s32)(self->field_10_f * (f64)self->data[self->field_02]) / 2;
                if (self->field_00 > half3) {
                    self->field_00 = (s16)half3;
                }
            }
        }
        break;
    }
    }
}

// func_801748B8 - move a map item by delta; crossing the item's half-range
// steps its type up/down and resets the position.  Returns field_00.
s16 func_801748B8(CfMapItem* self, s16 delta) {
    s16 f02 = self->field_02;
    if (delta > 0) {
        if ((u32)(f02 - 1) <= 1) {
            s16 nx = (s16)(self->field_00 + delta);
            self->field_00 = nx;
            s32 half = (s32)(self->field_10_f * (f64)self->data[f02]);
            s32 rem = nx - half;
            if (rem > 0) {
                self->field_02 = (s16)(f02 + 1);
                if (self->field_02 < 0) self->field_02 = 0;
                else if (self->field_02 > 4) self->field_02 = 4;
                func_802808AC(1);
                self->field_00 = 0;
                func_801748B8(self, (s16)rem);
            }
        } else if (f02 == 3) {
            s16 nx = (s16)(self->field_00 + delta);
            self->field_00 = nx;
            s32 half = (s32)(self->field_10_f * (f64)self->data[f02]);
            if (nx - half > 0) {
                s32 half2 = (s32)(self->field_10_f * (f64)self->data[f02]);
                self->field_00 = (s16)half2;
            }
        }
    } else {
        if ((u32)(f02 - 2) <= 1) {
            s16 nx = (s16)(self->field_00 + delta);
            self->field_00 = nx;
            if (nx < 0) {
                self->field_02 = (s16)(f02 - 1);
                if (self->field_02 < 0) self->field_02 = 0;
                else if (self->field_02 > 4) self->field_02 = 4;
                func_802808AC(1);
                s32 half = (s32)(self->field_10_f * (f64)self->data[self->field_02]);
                self->field_00 = (s16)half;
                func_801748B8(self, nx);
            }
        } else if (f02 == 1) {
            s16 nx = (s16)(self->field_00 + delta);
            self->field_00 = nx;
            if (nx < 0) {
                self->field_00 = 0;
            }
        }
    }
    return self->field_00;
}

void func_80174AE8(){}

void func_80174B3C(void* self, unsigned char a, unsigned char b, unsigned char c) {
    ((unsigned char*)self)[5] = a;
    ((unsigned char*)self)[6] = b;
    ((unsigned char*)self)[7] = c;
}

void func_80174B4C(){}

void func_80174C24(){}

void func_80174C98(){}

void func_801751DC(){}
