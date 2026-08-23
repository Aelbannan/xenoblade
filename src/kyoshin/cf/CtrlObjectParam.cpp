// Auto-scaffolded catalog TU for kyoshin/cf/CtrlObjectParam
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/harness_catalog.hpp"
#include "kyoshin/cf/CtrlObjectParam.hpp"
#include "kyoshin/cf/CfGameManager.hpp"        // cf::CfGameManager static accessors
#include "kyoshin/cf/object/CActorParam.hpp"
#include "kyoshin/cf/object/CObjectParam.hpp"
#include "kyoshin/CItemBoxInfo.hpp"             // CItem_initItemImplInstances shim
// func_800B8B94 (actor lookup): declared locally - CAIAction.hpp conflicts
// with CfGameManager.hpp's getInstance__Q22cf14CBattleManagerFv return type.
extern "C" void* func_800B8B94(s32);
extern "C" int func_80148778(void* obj, int id);   // battle-state status probe
extern "C" void* func_80149154(void* obj, u32 id); // battle-state status value
#include "monolib/util/MemManager.hpp"       // mtl::MemManager (work-buffer alloc)

#include <new>
#include <cstring>

// Stub declared below (func_800A145C) — called by func_8009E054.
extern "C" u8 func_800A145C(cf::CtrlObjectParamArtsLearnView* self);

// func_800A0E64 (arts-stat ramp) — defined later in this TU, called by
// func_800A0860 below.
extern "C" void func_800A0E64(u8* selfV, u16 value);

// Arts-set init / re-apply and the arts-slot re-arm ctor — defined later in
// this TU, called by the func_8009EF9C shared tail below.
extern "C" void func_800A03F4(cf::CtrlObjectParamArtsInitView* self);
extern "C" void __ct__8009F8B8(cf::CtrlObjectParamArtsSlotOwner* self);

// Entry init helper (defined below) — called by __ct__8009ED08.
extern "C" void func_800A0860(void* self, u16 val);

// Arts-list walker defined later in this TU; func_800A3594 aborts its row
// scan through it when it hits an unknown tag (noinline: retail calls it;
// extern "C": retail name is unmangled).
extern "C" __declspec(noinline) void func_800A3A6C(cf::CtrlObjectParamArtsList* list);

// Forward decls for the swap/type helpers defined later in this TU (callers
// in func_8009E168).
extern "C" int func_8009E20C(cf::CtrlObjectParamSwap* self, int firstType, int firstIndex,
                             int secondType, int secondIndex);
extern "C" int func_8009E344(const unsigned int* param_1, unsigned int param_2,
                             int* param_3, int* param_4);

// func_800A1B08 (level/rank accumulator) — defined later in this TU, called
// by func_800A1E3C below.
extern "C" void func_800A1B08(u32 rowIndex, int* outA, int* outB,
                              int unkArg4, int unkArg5, int flag);

// func_800A1CA0 (party-average level/rank) — defined later in this TU, called
// by func_800A1E3C below.
extern "C" void func_800A1CA0(u8* self, int* outLevel, int* outRank,
                              int targetLevel, int flag);

// func_800A2AF0 (arts-set re-apply) — defined later in this TU, called by
// func_800A21F8 below.
extern "C" int func_800A2AF0(cf::CtrlObjectParamTypeView* self);

// isPartySlotMatch (work-buffer slot-table probe) — defined later in this TU,
// called by func_800A21F8 below.
static inline int isPartySlotMatch(int type);

// func_800A30E4 (init helper) — defined later in this TU, called by func_8009DFC8.
extern "C" __declspec(noinline) void func_800A30E4(cf::CtrlObjectParamActorOwner* self);

// func_800A30E4 (init helper) — defined later in this TU, called by func_8009DFC8.
extern "C" __declspec(noinline) void func_800A30E4(cf::CtrlObjectParamActorOwner* self);

// func_8009E974 (work-buffer reset) — defined later in this TU.
extern "C" void func_8009E974(cf::CtrlObjectParamWork* work);

void __ct__8009D604() {
    // Work-buffer (re)initialization: allocate the 0x3A38C-byte buffer once,
    // zero the per-slot arrays, then run the per-entry init (14 entries of
    // 0x3DD4 at +0x41F0). The global is stored even on alloc failure; the
    // buffer is always reset through func_8009E974 afterwards.
    if (lbl_eu_80663E88 == 0) {
        cf::CtrlObjectParamWork* work = static_cast<cf::CtrlObjectParamWork*>(
            mtl::MemManager::allocate(0x3A38C, mtl::MemManager::getHandleMEM1()));
        if (work != 0) {
            // 1000 x 8-byte item slots at +0x58.
            u8* p = reinterpret_cast<u8*>(work) + 0x58;
            u8* pEnd = reinterpret_cast<u8*>(work) + 0x1F98;
            do {
                memset(p, 0, 8);
                p += 8;
            } while (p < pEnd);
            work->field_1F98 = 0;
            func_8009E7C8(pEnd);
            // 14 character entries of 0x3DD4 at +0x41F0.
            cf::CtrlObjectParamEntry* e = reinterpret_cast<cf::CtrlObjectParamEntry*>(
                reinterpret_cast<u8*>(work) + 0x41F0);
            cf::CtrlObjectParamEntry* eEnd = reinterpret_cast<cf::CtrlObjectParamEntry*>(
                reinterpret_cast<u8*>(work) + 0x3A388);
            do {
                e->shortArr[0] = -1;
                e->shortArr[1] = -1;
                e->shortArr[2] = -1;
                e->shortArr[3] = -1;
                e->shortArr[4] = -1;
                e->shortArr[5] = -1;
                memset(e->blob_28, 0, sizeof(e->blob_28));
                memset(e->blob_E8, 0, sizeof(e->blob_E8));
                new (&e->param_17C[0]) cf::CActorParam(
                    reinterpret_cast<UNKTYPE*>(&e->field_34FC),
                    reinterpret_cast<UNKTYPE*>(&e->big[0]));
                e->field_3500 = 0;
                e->field_3504 = 0;
                e->field_3508 = 0;
                e->field_34FC = reinterpret_cast<u32>(lbl_eu_805280E8);
                e->field_350C = 0;
                e->field_352C = 0;
                e->field_3530 = 0;
                func_8025EDC8(reinterpret_cast<CPcKizunagramBig*>(&e->big));
                ++e;
            } while (e < eEnd);
            // Reset the freshly built buffer before publishing it (retail
            // calls func_8009E974 both here and again on the shared tail).
            func_8009E974(work);
        }
        lbl_eu_80663E88 = reinterpret_cast<u32>(work);
    }
    func_8009E974(reinterpret_cast<cf::CtrlObjectParamWork*>(lbl_eu_80663E88));
}

extern "C" void __dt__8009D72C() {
    // Global destructor for the work-buffer pointer. The retail emits two
    // beq's on the same test (cmpwi r3,0; beq epi; beq epi) — the dead
    // second branch. Only the &&-form on a LOCAL keeps both beq (the same
    // duplicate disjunct on the global folds to one); MWCC CSEs the local
    // test to one cmpwi + two beq (CKizunagram/btm_sec duplicated-test
    // family).
    u32 p = lbl_eu_80663E88;
    if (p != 0 && p != 0) {
        ::operator delete(reinterpret_cast<void*>(p));
        lbl_eu_80663E88 = 0;
    }
}

extern "C" void* func_8009D764(cf::CtrlObjectParamInit* p) {
    p->field_00 = -1;
    p->field_02 = -1;
    p->field_04 = -1;
    p->field_06 = -1;
    p->field_08 = -1;
    p->field_0A = -1;
    extern void* memset(void*, int, unsigned long);
    return static_cast<char*>(memset(p->blob, 0, sizeof(p->blob)));
}

extern "C" u8* func_8009D790(s16* arr, u32 idx) {
    // Equip-table helper: when the s16 entry is valid (> -1), resolve the
    // item instance for the category and entry value; 0 otherwise.
    // The volatile read reproduces retail's reload of arr[idx] for the call
    // argument (retail lhax r4,r3,r5 reloads; the cat ternary reuses r0 and
    // clobbers the first read). Without volatile, MWCC CSEs the two reads
    // into one lhax + or r4,r0,r0 and colors cat into r3 (MWCC_CASES
    // instruction-selection: split-form reads need a volatile/walked read).
    volatile s16* q = &arr[idx];
    u8* result = 0;
    if (*q > -1) {
        result = reinterpret_cast<u8*>(
            func_80157C4C(idx > 4 ? 2 : idx + 4, *q));
    }
    return result;
}

cf::CtrlObjectParamSubEntry* cf::CtrlObjectParamData::getSubStruct(unsigned long index) {
    return &entries[index];
}

// ── func_8009D7F4 (us-8009e1cc) ───────────────────────────────────────────
// Equip-slot arts/status refresh: resolve the item instance for the slot
// (category = index+4, or 2 for index 5) and derive the arts id from the
// item's first word; without an item, fall back to the self u16 table. Then
// fill the 0x20-byte block at arr + index*0x40 + 0xC from the item bdat
// columns (+0x00..+0x61 for index 5, +0x48..+0x6F otherwise), or zero it.
extern "C" __declspec(noinline) void func_8009D7F4(s16* arr, u32 index, void* selfV) {
    u16* self = reinterpret_cast<u16*>(selfV);
    cf::CtrlObjectParamArtsBlock* blk = reinterpret_cast<cf::CtrlObjectParamArtsBlock*>(
        reinterpret_cast<u8*>(arr) + (index << 6) + 0xC);
    void* item = 0;
    if (arr[index] > -1) {
        u32 cat = index > 4 ? 2 : index + 4;
        item = func_80157C4C(cat, arr[index]);
    }
    u32 artsId = 0;
    if (item != 0) {
        artsId = (reinterpret_cast<cf::CtrlObjectParamItemWord*>(item)->word0 >> 5) & 0x7FF;
    } else if (self[0] >= 9) {
        if (index == 5) {
            artsId = self[(u16)index + 1];
        } else {
            artsId = func_80158018(self[(u16)index + 1]);
        }
    }
    if (artsId != 0) {
        if (index == 5) {
            void* bdat = reinterpret_cast<void*>(lbl_eu_806640F4);
            union { u32 w[2]; f64 d; } conv;
            conv.w[0] = 0x43300000;   // u32->f64 conversion magic (lis r29, 0x4330)
            blk->field_1E = (u8)getBdatStringColumnValue(bdat, &lbl_eu_804FBCB0[0x00], artsId);
            conv.w[1] = (u16)getBdatStringColumnValue(bdat, &lbl_eu_804FBCB0[0x05], artsId);
            blk->field_00 = lbl_eu_80666770 * (f32)(conv.d - lbl_eu_80666778);
            conv.w[1] = (u16)getBdatStringColumnValue(bdat, &lbl_eu_804FBCB0[0x14], artsId);
            blk->field_04 = lbl_eu_80666770 * (f32)(conv.d - lbl_eu_80666778);
            blk->field_0C = (s16)getBdatStringColumnValue(bdat, &lbl_eu_804FBCB0[0x22], artsId);
            blk->field_0E = (s16)getBdatStringColumnValue(bdat, &lbl_eu_804FBCB0[0x2A], artsId);
            blk->field_10 = (u8)getBdatStringColumnValue(bdat, &lbl_eu_804FBCB0[0x31], artsId);
            conv.w[1] = (u8)getBdatStringColumnValue(bdat, &lbl_eu_804FBCB0[0x39], artsId);
            blk->field_08 = lbl_eu_80666774 * (f32)(conv.d - lbl_eu_80666778);
            blk->field_14 = (u8)getBdatStringColumnValue(bdat, &lbl_eu_804FBCB0[0x3F], artsId);
            blk->field_16 = (u8)getBdatStringColumnValue(bdat, &lbl_eu_804FBCB0[0x48], artsId);
            blk->field_18 = (u8)getBdatStringColumnValue(bdat, &lbl_eu_804FBCB0[0x50], artsId);
            blk->field_1C = (u8)getBdatStringColumnValue(bdat, &lbl_eu_804FBCB0[0x58], artsId);
            blk->field_1D = (u8)getBdatStringColumnValue(bdat, &lbl_eu_804FBCB0[0x61], artsId);
        } else {
            void* bdat = reinterpret_cast<void*>(lbl_eu_806640F8);
            blk->field_16 = (u8)getBdatStringColumnValue(bdat, &lbl_eu_804FBCB0[0x48], artsId);
            blk->field_18 = (u8)getBdatStringColumnValue(bdat, &lbl_eu_804FBCB0[0x50], artsId);
            blk->field_1A = (u8)getBdatStringColumnValue(bdat, &lbl_eu_804FBCB0[0x66], artsId);
            blk->field_12 = (u8)getBdatStringColumnValue(bdat, &lbl_eu_804FBCB0[0x6F], artsId);
        }
    } else {
        memset(blk, 0, 0x20);
    }
}

void* cf::CActorParam::CActorParam_UnkVirtualFunc94() {
    return &reinterpret_cast<cf::CActorParamRetailView*>(this)->field_1650;
}

extern "C" void func_80155CD0(void* a, void* b);
extern "C" void func_8009DB1C(void* ignored, void* a, void* b) { func_80155CD0(a, b); }

extern "C" void func_8009DB28(void* selfV, u32 index) {
    cf::CtrlObjectParamEquipRow* self = (cf::CtrlObjectParamEquipRow*)selfV;
    // Remove the equip-slot entry: resolve the item instance, run the impl's
    // 0x10 hook, then clear the two s16 fields and resync the owner state.
    void* inst = 0;
    if (self->shortArr[index] > -1) {
        inst = func_80157C4C(index > 4 ? 2 : index + 4, self->shortArr[index]);
    }
    if (inst != 0) {
        reinterpret_cast<cf::CtrlObjectParamItemImplIf*>(
            CItem_initItemImplInstances(inst))->_v10(inst);
    }
    self->shortArr[index] = -1;
    self->field_0E[index] = 0;
    func_8009D7F4(&self->shortArr[0], index, self);
    func_800A30E4(reinterpret_cast<cf::CtrlObjectParamActorOwner*>(self));
}

extern "C" void func_8009DBF4(void* selfV, unsigned long index, void* valueV) {
    // Equip-slot write: store the item id at shortArr[index], then resolve
    // the item instance for the slot and derive the arts id from its first
    // word (extrwi 11,16 / srwi 20). The index-5 (arts) row additionally
    // copies the 0x49-stride arts row selected by the item/weapon bdat
    // column when the old and new row selectors differ.
    cf::CtrlObjectParamSlotView* self = reinterpret_cast<cf::CtrlObjectParamSlotView*>(selfV);
    self->shortArr[index] = (s16)(s32)valueV;
    u16 oldSlot = self->field_00[index + 1];
    if ((s32)valueV == -1) {
        self->field_0E[index] = 0;
        self->field_00[index + 1] = 0;
    } else {
        void* item = 0;
        if (self->shortArr[index] > -1) {
            item = func_80157C4C(index > 4 ? 2 : index + 4, self->shortArr[index]);
        }
        if (item != 0) {
            u32 word = reinterpret_cast<cf::CtrlObjectParamItemWord*>(item)->word0;
            self->field_0E[index] = (u16)((word >> 16) & 0x7FF);
            self->field_00[index + 1] = (u16)(word >> 20);
        }
        if (index == 5 && self->field_00[0] == 1) {
            u32 word = reinterpret_cast<cf::CtrlObjectParamItemWord*>(item)->word0;
            void* bdat = reinterpret_cast<void*>(lbl_eu_806640F4);
            const char* strBase = lbl_eu_804FBCB0;
            u32 col1 = getBdatStringColumnValue(bdat, &strBase[0x58], oldSlot);
            u32 col2 = getBdatStringColumnValue(bdat, &strBase[0x58], (u32)(word >> 20));
            if ((u8)col1 != (u8)col2) {
                cf::CtrlObjectParamArtsRowCopy* src =
                    reinterpret_cast<cf::CtrlObjectParamArtsRowCopy*>(
                        reinterpret_cast<u8*>(self) + (u32)(u8)col1 * 0x49);
                cf::CtrlObjectParamArtsRowCopy* dst =
                    reinterpret_cast<cf::CtrlObjectParamArtsRowCopy*>(
                        reinterpret_cast<u8*>(self) + (u32)(u8)col2 * 0x49);
                dst->field_E8[0] = src->field_E8[0];
                dst->field_119[0] = src->field_119[0];
                dst->field_E8[1] = src->field_E8[1];
                dst->field_119[1] = src->field_119[1];
                dst->field_E8[2] = src->field_E8[2];
                dst->field_119[2] = src->field_119[2];
                dst->field_E8[3] = src->field_E8[3];
                dst->field_119[3] = src->field_119[3];
                dst->field_E8[4] = src->field_E8[4];
                dst->field_119[4] = src->field_119[4];
                dst->field_E8[5] = src->field_E8[5];
                dst->field_119[5] = src->field_119[5];
                dst->field_E8[6] = src->field_E8[6];
                dst->field_119[6] = src->field_119[6];
                dst->field_E8[7] = src->field_E8[7];
                dst->field_119[7] = src->field_119[7];
            }
        }
    }
    func_8009D7F4(&self->shortArr[0], index, self);
    if (index == 5 && (s32)valueV != -1) {
        void* item2 = 0;
        if (self->shortArr[5] > -1) {
            item2 = func_80157C4C(2, self->shortArr[5]);
        }
        if (item2 != 0) {
            u16 w0 = (u16)(reinterpret_cast<cf::CtrlObjectParamWordView*>(
                self->mParam.CActorParam_UnkVirtualFunc94())->word0);
            if ((self->field_E6 & 4) != 0) {
                const char* strBase = lbl_eu_804FBCB0;
                union { u32 w[2]; f64 d; } c1;
                union { u32 w[2]; f64 d; } c2;
                u16 v1 = reinterpret_cast<cf::CtrlObjectParamVt028If*>(
                    CItem_initItemImplInstances(item2))->_v028(item2, &strBase[0x22]);
                c1.w[0] = 0x43300000;
                c1.w[1] = (u32)(w0 * v1) ^ 0x80000000;
                self->field_D4 = (s16)(lbl_eu_80666780 * (f32)(c1.d - lbl_eu_80666788));
                u16 v2 = reinterpret_cast<cf::CtrlObjectParamVt028If*>(
                    CItem_initItemImplInstances(item2))->_v028(item2, &strBase[0x2A]);
                c2.w[0] = 0x43300000;
                c2.w[1] = (u32)(w0 * v2) ^ 0x80000000;
                self->field_D6 = (s16)(lbl_eu_80666784 * (f32)(c2.d - lbl_eu_80666788));
                if (self->field_D4 > 0x3e7) self->field_D4 = 0x3e7;
                if (self->field_D6 > 0x3e7) self->field_D6 = 0x3e7;
            }
        }
    }
    func_800A30E4(reinterpret_cast<cf::CtrlObjectParamActorOwner*>(self));
    reinterpret_cast<cf::CtrlObjectParamVt0A4If*>(&self->mParam)->_v0A4(
        reinterpret_cast<cf::CtrlObjectParamActorOwner*>(self));
    if (lbl_eu_80663E24 & (1 << 20)) {
        reinterpret_cast<cf::CtrlObjectParamVt0A4If*>(&self->mParam)->_v0A8(1);
    } else {
        f32 v38 = self->mParam.CActorParam_UnkVirtualFunc38();
        if (self->mParam.CActorParam_UnkVirtualFunc37() > v38) {
            self->mParam.CActorParam_UnkVirtualFunc33(
                self->mParam.CActorParam_UnkVirtualFunc38());
        }
        reinterpret_cast<cf::CtrlObjectParamVt014If*>(
            reinterpret_cast<u8*>(&self->mParam) + 8)->_v14(0x35);
    }
}

void cf::CActorParam::CActorParam_UnkVirtualFunc33(float val) {
    reinterpret_cast<float&>(unk17E4.unk0[4]) = val;
}

void func_8009DFC8(cf::CtrlObjectParamActorOwner* self) {
    // Activate the owner: init via func_800A30E4, then dispatch virtual
    // slots 0xA4 / 0xA8 through the CActorParam embedded at +0x17C.
    func_800A30E4(self);
    reinterpret_cast<cf::CtrlObjectParamVt0A8If*>(&self->mParam)->_v0A4(self);
    reinterpret_cast<cf::CtrlObjectParamVt0A8If*>(&self->mParam)->_v0A8(1);
}

extern "C" void func_8009E024(void* a, void* c) { func_8009DBF4(a, 0, c); }

extern "C" void func_8009E030(void* a, void* c) { func_8009DBF4(a, 1, c); }

void cf::CtrlObjectParamData::setArgType2(void* arg) {
    extern void func_8009DBF4(cf::CtrlObjectParamData*, unsigned long, void*);
    func_8009DBF4(this, 2, arg);
}

void cf::CtrlObjectParamData::setArgType3(void* arg) {
    extern void func_8009DBF4(cf::CtrlObjectParamData*, unsigned long, void*);
    func_8009DBF4(this, 3, arg);
}

extern "C" void func_8009E054(cf::CtrlObjectParamData* self, u8* arg) {
    // Forward the arg as type 4, then run the type-8 init path when the
    // object's type tag is 8 and no actor exists for it.
    extern void func_8009DBF4(cf::CtrlObjectParamData*, unsigned long, u8*);
    func_8009DBF4(self, 4, arg);
    if (self->field_00 == 8) {
        if (func_800B8B94(self->field_00) == 0) {
            func_800A145C(reinterpret_cast<cf::CtrlObjectParamArtsLearnView*>(self));
        }
    }
}

void cf::CtrlObjectParamData::setArgType5(void* arg) {
    extern void func_8009DBF4(cf::CtrlObjectParamData*, unsigned long, void*);
    func_8009DBF4(this, 5, arg);
}

long cf::CtrlObjectParamData::getShortAt1C(unsigned long index) {
    return entries[0].shortArr[index];
}

void func_8009E0C4(cf::CtrlObjectParamU16RowTable* table, u16 index, u16 value) {
    // Resolve the item family (outputs discarded), then store value into the
    // u16 row table at +2.
    u16 a;
    u16 b;
    func_80157F04(value, &a, &b);
    table->rows[index] = value;
}

extern "C" u32 func_8009E120(cf::CtrlObjectParamRowView* p, u32 value) {
    // Arts/row lookup: read the u16 table at +0x02 at index (u16)value; a
    // 0xFFFF table entry means "empty" and is folded to 0 before dispatch.
    // The explicit re-truncation after the fold reproduces the retail
    // clrlwi at the func_80142074 call site (cf. MWCC_CASES wpad noise-filter).
    int v = (u16)p->field_02[(u16)value];
    if (v == 0xFFFF) v = 0;
    v = (u16)v;
    if (v != 0)
        return func_80142074(p->field_00, v, 0);
    return func_80141E90(p->field_00, (s16)v, (u16)(value + 1), 0);
}

// Locate the (type, index) pair for value a and for value b in the swap
// object; when both are present, swap them via func_8009E20C.
int func_8009E168(cf::CtrlObjectParamSwap* self, unsigned int a, unsigned int b) {
    int t1 = 0;
    int i1 = 0;
    int t2 = 0;
    int i2 = 0;
    int r1 = func_8009E344(reinterpret_cast<const unsigned int*>(self), a, &t1, &i1);
    int r2 = func_8009E344(reinterpret_cast<const unsigned int*>(self), b, &t2, &i2);
    if (r1 != 0 && r2 != 0) {
        return func_8009E20C(self, t1, i1, t2, i2);
    }
    return 0;
}

extern "C" int func_8009E20C(cf::CtrlObjectParamSwap* self, int firstType, int firstIndex,
                             int secondType, int secondIndex) {
    int* first = 0;
    int* second = 0;
    if (firstType == 1)
        first = &self->intArr1[firstIndex];
    else if (firstType == 2)
        first = &self->intArr2[firstIndex];
    if (secondType == 1)
        second = &self->intArr1[secondIndex];
    else if (secondType == 2)
        second = &self->intArr2[secondIndex];
    int value = *first;
    *first = *second;
    *second = value;
    return 1;
}

// Membership test: 1 if value matches any of the 9 slot words (arr1[3] +
// arr2[6]) starting at +4, 0 otherwise. The tail reads go through a +8 base
// like the retail (p = &arr[2]; p[4..7]).
int func_8009E284(const int* arr, int value) {
    const int* p = arr + 2;
    if (value == arr[1]) return 1;
    if (value == arr[2]) return 1;
    if (value == arr[3]) return 1;
    if (value == arr[4]) return 1;
    if (value == arr[5]) return 1;
    if (value == p[4]) return 1;
    if (value == p[5]) return 1;
    if (value == p[6]) return 1;
    if (value == p[7]) return 1;
    return 0;
}

extern "C" int func_8009E344(const unsigned int* param_1, unsigned int param_2,
                              int* param_3, int* param_4) {
    for (int i = 0; i < 3; ++i) {
        if (param_1[i + 1] == param_2) {
            *param_3 = 1;
            *param_4 = i;
            return 1;
        }
    }
    for (int i = 0; i < 6; ++i) {
        if (param_1[i + 4] == param_2) {
            *param_3 = 2;
            *param_4 = i;
            return 1;
        }
    }
    return 0;
}

// Slot-validity check: all 9 slot words (arr1 + arr2) must be in [0, 0xD].
// Retail keeps a 3-trip mtctr/bdnz loop with the three checks unrolled inside
// each trip (plus a dead +2 counter, part of the original loop update).
int func_8009E3C0(const int* arr) {
    int ok = 1;
    int j = 0;
    int flag;
    int v;
    int i = 3;
    do {
        // goto-chain mirrors the retail bodies-after-tests layout so MWCC
        // keeps the two signed range compares unfused.
        flag = 0;
        v = arr[j + 1];
        if (v < 0) goto test;
        if (v > 0xD) goto test;
        flag = 1;
    test:
        if (flag == 0) {
            ok = 0;
        }
        j += 1;

        flag = 0;
        v = arr[j + 1];
        if (v < 0) goto test2;
        if (v > 0xD) goto test2;
        flag = 1;
    test2:
        if (flag == 0) {
            ok = 0;
        }
        j += 1;

        flag = 0;
        v = arr[j + 1];
        if (v < 0) goto test3;
        if (v > 0xD) goto test3;
        flag = 1;
    test3:
        if (flag == 0) {
            ok = 0;
        }
        j += 1;
    } while (--i != 0);
    return ok;
}

extern "C" int func_8009E474(cf::CtrlObjectParamSwap* self, unsigned int value) {
    // Move value to the head slot: probe the list, then swap the head entry
    // with value's current (type, index); when either lookup misses, just
    // store value at the head slot. The first probe's result is discarded.
    int t1 = 0, i1 = 0;
    u32 other = self->intArr1[0];
    func_8009E344(reinterpret_cast<const unsigned int*>(self), value, &t1, &i1);
    if (other != 0) {
        int i3, t3, i2, t2;
        t2 = 0;
        i2 = 0;
        t3 = 0;
        i3 = 0;
        int f2 = func_8009E344(reinterpret_cast<const unsigned int*>(self), value, &t2, &i2);
        int f3 = func_8009E344(reinterpret_cast<const unsigned int*>(self), other, &t3, &i3);
        int result;
        if (f2 != 0 && f3 != 0) {
            result = func_8009E20C(self, t2, i2, t3, i3);
        } else {
            result = 0;
        }
        if (result != 0) {
            return 1;
        }
        self->intArr1[0] = value;
        return 1;
    }
    self->intArr1[0] = value;
    return 1;
}

extern "C" int func_8009E574(cf::CtrlObjectParamSlots* self, int value, int type, int index) {
    // Slot insertion: refuse when value already sits in one of the 9 slot
    // words (arr1[3] + arr2[6]), then store it at the requested (type,
    // index) slot, or the first free slot when index is -1. The tail reads
    // go through a +8 base like the retail (p = &arr1[2]; p[4..7]).
    const int* p = &self->arr1[2];
    int found;
    if (value == self->arr1[0]) found = 1;
    else if (value == self->arr1[1]) found = 1;
    else if (value == self->arr1[2]) found = 1;
    else if (value == self->arr2[0]) found = 1;
    else if (value == self->arr2[1]) found = 1;
    else if (value == p[4]) found = 1;
    else if (value == p[5]) found = 1;
    else if (value == p[6]) found = 1;
    else if (value == p[7]) found = 1;
    else found = 0;
    if (found != 0) return 0;
    if (type == 1) {
        if (index == -1) {
            for (int i = 0; i < 3; ++i) {
                if (self->arr1[i] == 0) {
                    self->arr1[i] = value;
                    return 1;
                }
            }
            return 0;
        }
        if (self->arr1[index] != 0) return 0;
        self->arr1[index] = value;
        return 1;
    } else if (type == 2) {
        if (index == -1) {
            for (int i = 0; i < 6; ++i) {
                if (self->arr2[i] == 0) {
                    self->arr2[i] = value;
                    return 1;
                }
            }
            return 0;
        }
        if (self->arr2[index] != 0) return 0;
        self->arr2[index] = value;
        return 1;
    }
    return 0;
}

extern "C" int func_8009E56C(cf::CtrlObjectParamSlots* a, int b, int c) { return func_8009E574(a, b, c, -1); }

extern "C" int func_8009E740(cf::CtrlObjectParamSlots* self, int value) {
    // Clear the first occurrence of value in arr1 (3 entries), else in
    // arr2 (6 entries); return 1 on a hit, 0 otherwise.
    for (int i = 0; i < 3; ++i) {
        if (self->arr1[i] == value) {
            self->arr1[i] = 0;
            return 1;
        }
    }
    for (int i = 0; i < 6; ++i) {
        if (self->arr2[i] == value) {
            self->arr2[i] = 0;
            return 1;
        }
    }
    return 0;
}

int cf::CtrlObjectParamClear::clearStruct() {
    firstByte = 0;
    for (int i = 0; i < 24; ++i) words[i] = 0;
    return 1;
}

extern "C" void func_8009E838(u8* self) {
    for (int a = 0; a < 3; a++) {
        void* cur = (u8*)self + a * 4;
        if (*(u32*)((u8*)cur + 4) == 0) {
            int found = 0;
            for (int i = a + 1; i < 3; i++) {
                if (*(u32*)((u8*)self + (i * 4) + 4) != 0) {
                    *(u32*)((u8*)cur + 4) = *(u32*)((u8*)self + (i * 4) + 4);
                    *(u32*)((u8*)self + (i * 4) + 4) = 0;
                    found = 1;
                    break;
                }
            }
            if (!found) {
                int done = 0;
                for (int j = 0; j < 6; j++) {
                    void* cur2 = (u8*)self + j * 4;
                    if (*(u32*)((u8*)cur2 + 0x10) == 0) {
                        for (int i = j + 1; i < 6; i++) {
                            if (*(u32*)((u8*)self + (i * 4) + 0x10) != 0) {
                                *(u32*)((u8*)cur2 + 0x10) = *(u32*)((u8*)self + (i * 4) + 0x10);
                                *(u32*)((u8*)self + (i * 4) + 0x10) = 0;
                                done = 1;
                                break;
                            }
                        }
                    }
                    if (done) break;
                }
                return;
            }
        }
    }
}

extern "C" void func_8009E974(cf::CtrlObjectParamWork* work) {
    // Reset the work buffer: zero the header/copy area, the two bitmaps and
    // the per-entry s16 fields, then re-run the per-slot and entry init and
    // refresh the event data table.
    memset(work, 0, 0x24);
    memset(reinterpret_cast<u8*>(work) + 0x24, 0, 0x24);
    memset(reinterpret_cast<u8*>(work) + 0x48, 0, 8);
    memset(work->bitmap_1FFC, 0, sizeof(work->bitmap_1FFC));
    memset(work->bitmap_3866, 0, sizeof(work->bitmap_3866));
    for (u32 i = 0; i < 14; ++i) {
        cf::CtrlObjectParamEntry* e = &work->entries[i];
        for (u32 j = 0; j < 7; ++j) e->field_00[j] = 0;
        for (u32 j = 0; j < 6; ++j) e->field_0E[j] = 0;
        for (u32 j = 0; j < 6; ++j) e->shortArr[j] = -1;
        memset(e->blob_28, 0, sizeof(e->blob_28));
    }
    for (u32 i = 0; i < 0x3E8; ++i) {
        memset(work->slots_58[i], 0, sizeof(work->slots_58[i]));
    }
    work->field_3A388 = 0;
    work->field_50 = 0;
    work->field_52 = 0;
    work->field_54 = 1;
    work->field_56 = 1;
    func_8016455C(reinterpret_cast<CEventDataTable*>(&work->eventTable));
}

extern "C" void func_8009EABC() {
    // Zero the 1000 8-byte entry slots at +0x58, then the 50000-bit
    // bitmap (0x186A bytes) at +0x1FFC.
    u8* work = reinterpret_cast<u8*>(lbl_eu_80663E88);
    for (u32 i = 0; i < 0x3E8; ++i) {
        memset(work + 0x58 + i * 8, 0, 8);
    }
    memset(work + 0x1FFC, 0, 0x186A);
}

extern "C" char* func_8009EB2C(u16 arg1, u16 arg2, const char* srcStr) {
    // Copy the 36-byte header block forward, write the two s16 ids, then
    // strncpy the caller's string into the (already copied) source area.
    // The copy goes through a u32-first view so the whole block copies as
    // words (retail lwz 0->36 then word pairs); a plain struct assign emits
    // lha/sth for the s16 pair at +0/+2 (0x70 vs retail 0x68).
    cf::CtrlObjectParamCopyView* work =
        reinterpret_cast<cf::CtrlObjectParamCopyView*>(lbl_eu_80663E88);
    struct CopyWords {
        u32 field_00;
        char str[0x20];
    };
    *(CopyWords*)&work->dst = *(CopyWords*)&work->src;
    work->src.field_00 = arg2;
    work->src.field_02 = arg1;
    return strncpy(work->src.str, srcStr, 0x1f);
}

void func_8009EB94(unsigned int idx, int flag) {
    // Set (flag != 0) or clear (flag == 0) bit (idx & 7) of the 50000-bit
    // bitmap at work-buffer +0x1FFC.
    if (idx >= 50000) return;
    int mask = 1 << (idx & 7);
    u8* p = reinterpret_cast<u8*>(lbl_eu_80663E88 + (idx >> 3));
    if (flag != 0) {
        p[0x1FFC] |= (u8)mask;
    } else {
        p[0x1FFC] &= (u8)~mask;
    }
}

extern "C" int func_8009EBE8(unsigned int idx) {
    extern u32 lbl_eu_80663E88;
    if (idx >= 50000) return 0;
    const u8* bitmap = reinterpret_cast<const u8*>(lbl_eu_80663E88 + 8188);
    return (bitmap[idx >> 3] >> (idx & 7)) & 1;
}

void func_8009EC18(unsigned int idx, int flag) {
    // Set (flag != 0) or clear (flag == 0) bit (idx & 7) of the 320-bit
    // bitmap at work-buffer +0x3866.
    if (idx >= 0x140) return;
    int mask = 1 << (idx & 7);
    u8* p = reinterpret_cast<u8*>(lbl_eu_80663E88 + (idx >> 3));
    if (flag != 0) {
        p[0x3866] |= (u8)mask;
    } else {
        p[0x3866] &= (u8)~mask;
    }
}

extern "C" int func_8009EC6C(unsigned int idx) {
    // Flag-bit lookup: bit idx of the 320-bit bitmap at work-buffer +0x3866.
    if (idx >= 0x140) return 0;
    const u8* bitmap = reinterpret_cast<const u8*>(lbl_eu_80663E88 + 0x3866);
    return (bitmap[idx >> 3] >> (idx & 7)) & 1;
}

// Matches the declaration in kyoshin/code_800B06A4.hpp (u32 vs u16 would be
// an illegal overload there).
extern "C" void* func_8009EC9C(unsigned long idx) {
    int i = (int)idx;
    extern u32 lbl_eu_80663E88;
    return reinterpret_cast<void*>(lbl_eu_80663E88 + i * 15828 + 16880);
}

extern "C" void* func_8009ECB0() {
    extern unsigned long lbl_eu_80663E88;
    return reinterpret_cast<void*>(lbl_eu_80663E88 + 0x1f98);
}

extern "C" void* func_8009ECBC(int idx) {
    extern u32 lbl_eu_80663E88;
    return reinterpret_cast<void*>(lbl_eu_80663E88 + (idx * 8) + 88);
}

extern "C" void func_8009ECD0(unsigned long val) {
    extern unsigned long lbl_eu_80663E88;
    unsigned long addr = lbl_eu_80663E88;
    *reinterpret_cast<unsigned long*>(addr + 0x3A388) = val;
}

extern "C" u32 func_8009ECE0() {
    extern u32 lbl_eu_80663E88;
    return *reinterpret_cast<u32*>(lbl_eu_80663E88 + 0x3A388);
}

extern "C" unsigned long func_8009ECF0() {
    extern unsigned long lbl_eu_80663E88;
    return *reinterpret_cast<unsigned short*>(lbl_eu_80663E88 + 0x50);
}

extern "C" void func_8009ECFC(unsigned short value) {
    extern unsigned long lbl_eu_80663E88;
    *reinterpret_cast<unsigned short*>(lbl_eu_80663E88 + 0x50) = value;
}

// ── __ct__8009ED08 (us-8009f6e0) ─────────────────────────────────────────
// Character-row copy: resolve the row entries for the two u16 row indices
// (work + idx*0x3DD4), init entryA via func_800A0860, snapshot entryB's
// param sub-region (blocks at +0x188/+0x16AC/+0x1730) to stack locals, copy
// entryB's CActorParam into entryA's via func_80175A50, then restore the
// snapshots. The dead lbl_eu_8052E9B0 address store is kept by retail.
void __ct__8009ED08(u32 rowA, u32 rowB) {
    const char* strBase = lbl_eu_804FBCB0;   // materialized early (retail lis/addi)
    void* fp = getFP__FPCc(&strBase[0x78]);
    u32 v = getBdatStringColumnValue(fp, &strBase[0x83], rowB);
    cf::CtrlObjectParamEntry* entryA = reinterpret_cast<cf::CtrlObjectParamEntry*>(
        reinterpret_cast<u8*>(lbl_eu_80663E88) + ((u16)rowA * 0x3DD4) + 0x41F0);
    func_800A0860(reinterpret_cast<cf::CtrlObjectParamSlotView*>(entryA), v);
    cf::CtrlObjectParamEntry* entryB = reinterpret_cast<cf::CtrlObjectParamEntry*>(
        reinterpret_cast<u8*>(lbl_eu_80663E88) + ((u16)rowB * 0x3DD4) + 0x41F0);
    volatile u32 lblAddr = (u32)lbl_eu_8052E9B0;  // dead store kept by retail

    // Snapshot entryB's param sub-region (inside the CActorParam area),
    // refresh entryA's CActorParam from entryB's, then restore the snapshot.
    cf::CtrlObjectParamCopyBlock1 t1 = *reinterpret_cast<cf::CtrlObjectParamCopyBlock1*>(
        reinterpret_cast<u8*>(entryB) + 0x188);
    cf::CtrlObjectParamCopyBlock2 t2 = *reinterpret_cast<cf::CtrlObjectParamCopyBlock2*>(
        reinterpret_cast<u8*>(entryB) + 0x16AC);
    cf::CtrlObjectParamCopyBlock3 t3 = *reinterpret_cast<cf::CtrlObjectParamCopyBlock3*>(
        reinterpret_cast<u8*>(entryB) + 0x1730);
    func_80175A50(&entryB->param_17C[0], &entryA->param_17C[0]);
    *reinterpret_cast<cf::CtrlObjectParamCopyBlock1*>(
        reinterpret_cast<u8*>(entryB) + 0x188) = t1;
    *reinterpret_cast<cf::CtrlObjectParamCopyBlock2*>(
        reinterpret_cast<u8*>(entryB) + 0x16AC) = t2;
    *reinterpret_cast<cf::CtrlObjectParamCopyBlock3*>(
        reinterpret_cast<u8*>(entryB) + 0x1730) = t3;
}

// ── func_8009EF9C (us-8009f974) ───────────────────────────────────────────
// Character-entry refresh: when arg2 is 0, snapshot the type id and run the
// init flow — the arts-stat gate (slot 0x108) splits into the >0 branch
// (slot 0xD4 level write + 0x170/0x188/0x198/0x1A4/0x1BC arts-set refresh,
// slot 0xCC byte write, then the equip-slot/column block) and the <=0 branch
// (same refresh plus the slot 0x264 float write, then the arts-stat record
// init from the slot 0x20C result and func_800A0860). Both branches finish
// with the 0x17A5..0x17A8 byte columns, the 0x34D4 row-sync helpers and the
// 0x176C flag. The shared tail re-arms the arts list (__ct__8009F8B8,
// func_800A03F4, func_800A2AF0) and refreshes the actor param.
void func_8009EF9C(cf::CtrlObjectParamEF9C* self, u32 arg2) {
    void* actor = func_800B8B94(self->field_00);
    reinterpret_cast<cf::CtrlObjectParamEF9CTail*>(&self->mParam)->field_3DA0 = self->field_00;
    if (arg2 == 0) {
        void* bdat = reinterpret_cast<void*>(lbl_eu_80664090);
        int result = self->mParam.CActorParam_UnkVirtualFunc29();
        if (result > 0) {
            const char* strBase = lbl_eu_804FBCB0;
            cf::CtrlObjectParamEF9CTail* tail =
                reinterpret_cast<cf::CtrlObjectParamEF9CTail*>(&self->mParam);
            u32 v86 = getBdatStringColumnValue(bdat, &strBase[0x86], self->field_00);
            reinterpret_cast<cf::CtrlObjectParamVt34FCIf*>(&tail->field_34FC)->_v034(v86);
            union { u32 w[2]; f64 d; } conv;
            conv.w[0] = 0x43300000;   // u32->f64 conversion magic
            volatile u32 v8B = getBdatStringColumnValue(bdat, &strBase[0x8B], self->field_00);
            conv.w[1] = (u8)v8B;
            self->mParam.CActorParam_UnkVirtualFunc16(
                (f32)(conv.d - lbl_eu_80666778) / lbl_eu_80666790);
            self->mParam.CActorParam_UnkVirtualFunc55(1);
            self->mParam.CActorParam_UnkVirtualFunc61(1);
            self->mParam.CActorParam_UnkVirtualFunc65(lbl_eu_80666794);
            self->mParam.CActorParam_UnkVirtualFunc68(lbl_eu_80666794);
            self->mParam.CActorParam_UnkVirtualFunc74(
                lbl_eu_80666798 * (lbl_eu_8066A1F8 / lbl_eu_8066679C));
            volatile u32 v98 = getBdatStringColumnValue(bdat, &strBase[0x98], self->field_00);
            self->mParam.CActorParam_UnkVirtualFunc14((u8)v98);
            *reinterpret_cast<u16*>(self->mParam.CActorParam_UnkVirtualFunc165()) |= 7;
            int rowType = self->field_00;
            if (rowType == 4) {
                volatile u32 v = getBdatStringColumnValue(
                    reinterpret_cast<void*>(lbl_eu_806640F4), &strBase[0x58], self->field_0C);
                if ((u8)v == 1) rowType = 12;
            }
            cf::CActorParamRetailView* view =
                reinterpret_cast<cf::CActorParamRetailView*>(&self->mParam);
            volatile u32 vA2 = getBdatStringColumnValue(bdat, &strBase[0xA2], rowType);
            view->unk1629 = (u8)vA2;
            volatile u32 vA9 = getBdatStringColumnValue(bdat, &strBase[0xA9], rowType);
            view->unk162A = (u8)vA9;
            volatile u32 vB1 = getBdatStringColumnValue(bdat, &strBase[0xB1], rowType);
            view->unk162B = (u8)vB1;
            volatile u32 vBB = getBdatStringColumnValue(bdat, &strBase[0xBB], rowType);
            view->unk162C = (u8)vBB;
            volatile u32 vC6 = getBdatStringColumnValue(bdat, &strBase[0xC6], self->field_00);
            volatile u32 vCE = getBdatStringColumnValue(bdat, &strBase[0xCE], self->field_00);
            volatile u32 vD5 = getBdatStringColumnValue(bdat, &strBase[0xD5], self->field_00);
            func_80174B3C(&view->field_3358, (u8)vD5, (u8)vCE, (u8)vC6);
            func_80174AE8(&view->field_3358);
            volatile u32 vDC = getBdatStringColumnValue(bdat, &strBase[0xDC], self->field_00);
            if ((u8)vDC != 0) view->unk15F0 = 1;
            func_80174B4C(&self->mParam, 3);
        } else {
            const char* strBase = lbl_eu_804FBCB0;
            cf::CtrlObjectParamEF9CTail* tail =
                reinterpret_cast<cf::CtrlObjectParamEF9CTail*>(&self->mParam);
            u32 v86 = getBdatStringColumnValue(bdat, &strBase[0x86], self->field_00);
            reinterpret_cast<cf::CtrlObjectParamVt34FCIf*>(&tail->field_34FC)->_v034(v86);
            union { u32 w[2]; f64 d; } conv;
            conv.w[0] = 0x43300000;   // u32->f64 conversion magic
            volatile u32 v8B = getBdatStringColumnValue(bdat, &strBase[0x8B], self->field_00);
            conv.w[1] = (u8)v8B;
            self->mParam.CActorParam_UnkVirtualFunc16(
                (f32)(conv.d - lbl_eu_80666778) / lbl_eu_80666790);
            self->mParam.CActorParam_UnkVirtualFunc116(lbl_eu_80663EC8);
            self->mParam.CActorParam_UnkVirtualFunc55(1);
            self->mParam.CActorParam_UnkVirtualFunc61(1);
            self->mParam.CActorParam_UnkVirtualFunc65(lbl_eu_80666794);
            self->mParam.CActorParam_UnkVirtualFunc68(lbl_eu_80666794);
            self->mParam.CActorParam_UnkVirtualFunc74(
                lbl_eu_80666798 * (lbl_eu_8066A1F8 / lbl_eu_8066679C));
            volatile u32 v98 = getBdatStringColumnValue(bdat, &strBase[0x98], self->field_00);
            self->mParam.CActorParam_UnkVirtualFunc14((u8)v98);
            cf::CtrlObjectParamStats* record =
                reinterpret_cast<cf::CtrlObjectParamStats*>(
                    self->mParam.CActorParam_UnkVirtualFunc94());
            // Arts-stat record init (column offsets mirror func_800A0860).
            record->field_00 = (u8)getBdatStringColumnValue(bdat, &strBase[0x83], self->field_00);
            union { u32 w[2]; f64 d; } conv2;
            conv2.w[0] = 0x43300000;
            conv2.w[1] = (u16)getBdatStringColumnValue(bdat, &strBase[0xE2], self->field_00);
            record->field_10 = (f32)(conv2.d - lbl_eu_80666778);
            volatile u32 vE5 = getBdatStringColumnValue(bdat, &strBase[0xE5], self->field_00);
            record->field_1C = (s16)vE5;
            volatile u32 vE9 = getBdatStringColumnValue(bdat, &strBase[0xE9], self->field_00);
            record->field_1E = (s16)vE9;
            volatile u32 vED = getBdatStringColumnValue(bdat, &strBase[0xED], self->field_00);
            record->field_20 = (s16)vED;
            record->field_44 = lbl_eu_806667A0;
            record->field_48 = lbl_eu_806667A0;
            volatile u32 vF1 = getBdatStringColumnValue(bdat, &strBase[0xF1], self->field_00);
            record->field_56 = (u8)vF1;
            volatile u32 vF9 = getBdatStringColumnValue(bdat, &strBase[0xF9], self->field_00);
            record->field_57 = (u8)vF9;
            *reinterpret_cast<u16*>(self->mParam.CActorParam_UnkVirtualFunc165()) |= 7;
            int rowType = self->field_00;
            if (rowType == 4) {
                volatile u32 v = getBdatStringColumnValue(
                    reinterpret_cast<void*>(lbl_eu_806640F4), &strBase[0x58], self->field_0C);
                if ((u8)v == 1) rowType = 12;
            }
            cf::CActorParamRetailView* view =
                reinterpret_cast<cf::CActorParamRetailView*>(&self->mParam);
            volatile u32 vA2 = getBdatStringColumnValue(bdat, &strBase[0xA2], rowType);
            view->unk1629 = (u8)vA2;
            volatile u32 vA9 = getBdatStringColumnValue(bdat, &strBase[0xA9], rowType);
            view->unk162A = (u8)vA9;
            volatile u32 vB1 = getBdatStringColumnValue(bdat, &strBase[0xB1], rowType);
            view->unk162B = (u8)vB1;
            volatile u32 vBB = getBdatStringColumnValue(bdat, &strBase[0xBB], rowType);
            view->unk162C = (u8)vBB;
            volatile u32 vC6 = getBdatStringColumnValue(bdat, &strBase[0xC6], self->field_00);
            volatile u32 vCE = getBdatStringColumnValue(bdat, &strBase[0xCE], self->field_00);
            volatile u32 vD5 = getBdatStringColumnValue(bdat, &strBase[0xD5], self->field_00);
            func_80174B3C(&view->field_3358, (u8)vD5, (u8)vCE, (u8)vC6);
            func_80174AE8(&view->field_3358);
            volatile u32 vDC = getBdatStringColumnValue(bdat, &strBase[0xDC], self->field_00);
            if ((u8)vDC != 0) view->unk15F0 = 1;
            func_800A0860(reinterpret_cast<cf::CtrlObjectParamSlotView*>(self),
                          record->field_00);
        }
    }
    // Shared tail: re-arm the arts list and refresh the actor param.
    __ct__8009F8B8(reinterpret_cast<cf::CtrlObjectParamArtsSlotOwner*>(self));
    func_800A03F4(reinterpret_cast<cf::CtrlObjectParamArtsInitView*>(self));
    func_800A2AF0(reinterpret_cast<cf::CtrlObjectParamTypeView*>(self));
    if (actor != 0) {
        void* a3 = func_800B8B94(self->field_00);
        if (a3 != 0) {
            u8* value = reinterpret_cast<cf::CtrlObjectParamVt028CIf*>(a3)->_v028C();
            func_80175A50(value, reinterpret_cast<u8*>(&self->mParam));
        }
    }
}

void cf::CActorParam::CActorParam_UnkVirtualFunc16(float val) {
    unk15F8 = val;
}

void cf::CActorParam::CActorParam_UnkVirtualFunc55(u16 val) {
    unk160E = val;
}

void cf::CActorParam::CActorParam_UnkVirtualFunc61(u16 val) {
    unk1616 = val;
}

void cf::CActorParam::CActorParam_UnkVirtualFunc65(float val) {
    unk1610 = val;
}

void cf::CActorParam::CActorParam_UnkVirtualFunc68(float val) {
    unk1618 = val;
}

void cf::CActorParam::CActorParam_UnkVirtualFunc74(float val) {
    unk17E4.unk4C = val;
}

void cf::CActorParam::CActorParam_UnkVirtualFunc14(u8 val) {
    unk15F4[0] = val;
}

void* cf::CActorParam::CActorParam_UnkVirtualFunc165() {
    return &reinterpret_cast<cf::CActorParamRetailView*>(this)->field_164C;
}

void cf::CActorParam::CActorParam_UnkVirtualFunc116(float val) {
    unk1620 = val;
}

extern "C" void func_8009F6D4(void* selfV) {
    cf::CtrlObjectParamEntry9F6D4* self = reinterpret_cast<cf::CtrlObjectParamEntry9F6D4*>(selfV);
    // Character-row activation: run the 0xAC/0xB0/0xB8 init chain on the
    // actor (or the embedded CActorParam when no actor exists), refresh the
    // param via the 0x28C label hook, then pass the halved (level * byte)
    // value into vtable slot 0x2FC. Note the first func_80175A50 call passes
    // (&mParam, value) while the tail passes (value, &mParam) - retail order.
    void* actor2;
    void* actor = func_800B8B94(self->field_00);
    if (actor != 0) {
        reinterpret_cast<cf::CtrlObjectParamVt0A4If*>(actor)->_v0AC(0);
        reinterpret_cast<cf::CtrlObjectParamVt0A4If*>(actor)->_v0B0();
        reinterpret_cast<cf::CtrlObjectParamVt0A4If*>(actor)->_v0B8();
    } else {
        reinterpret_cast<cf::CtrlObjectParamVt0A4If*>(&self->mParam)->_v0AC(0);
        reinterpret_cast<cf::CtrlObjectParamVt0A4If*>(&self->mParam)->_v0B0();
        reinterpret_cast<cf::CtrlObjectParamVt0A4If*>(&self->mParam)->_v0B8();
    }
    actor2 = func_800B8B94(self->field_00);
    if (actor2 != 0) {
        func_801765A4(actor2, lbl_eu_806667A0, 1);
        u8* value = reinterpret_cast<cf::CtrlObjectParamVt0A4If*>(actor2)->_v028C();
        func_80175A50(reinterpret_cast<u8*>(&self->mParam), value);
    } else {
        reinterpret_cast<cf::CtrlObjectParamVt0A4If*>(&self->mParam)->_v0A4(self);
    }
    reinterpret_cast<cf::CtrlObjectParamVt0A4If*>(&self->mParam)->_v0304(2);
    cf::CtrlObjectParamF2F4View* rec = reinterpret_cast<cf::CtrlObjectParamF2F4View*>(
        reinterpret_cast<cf::CtrlObjectParamVt0A4If*>(&self->mParam)->_v02F4());
    union { u32 w[2]; double d; } c;
    c.w[0] = 0x43300000;
    const u8* p = reinterpret_cast<const u8*>(rec) + rec->field_02;
    u8 b = p[4];
    c.w[1] = b;
    s32 x = (s32)(rec->field_10 * (f32)(c.d - lbl_eu_80666778));
    reinterpret_cast<cf::CtrlObjectParamVt0A4If*>(&self->mParam)->_v02FC(x / 2);
    if (actor != 0) {
        void* actor3 = func_800B8B94(self->field_00);
        if (actor3 != 0) {
            u8* value = reinterpret_cast<cf::CtrlObjectParamVt0A4If*>(actor3)->_v028C();
            func_80175A50(value, reinterpret_cast<u8*>(&self->mParam));
        }
    }
}

void* cf::CActorParam::CActorParam_UnkVirtualFunc152() {
    return &reinterpret_cast<cf::CActorParamRetailView*>(this)->field_3358;
}

// Arts-list / slot re-arm hook (called by func_800A18A4). noinline: the
// empty stub body would otherwise fold into the caller and the retail
// `bl __ct__8009F8B8` call site would vanish.
extern "C" __declspec(noinline) void __ct__8009F8B8(cf::CtrlObjectParamArtsSlotOwner* self){}

// ── func_800A03F4 (us-800a0dcc) ───────────────────────────────────────────
// Arts-set init: refresh the six u16 arts-slot words of the UnkVirtualFunc125
// result through the arts-stat / item bdat columns. The type-0 path writes
// the three header words from stat columns and zeroes the rest; the main path
// fills each slot from the column named by a digit-suffixed prefix, then
// populates the per-slot attack-parameter record (getAtkParam) with the arts
// name / level / flag bytes and the E6-derived flag bits.
extern "C" void func_800A03F4(cf::CtrlObjectParamArtsInitView* self) {
    u16 artsKey = self->field_0C;
    void* v125 = self->mParam.CActorParam_UnkVirtualFunc125();
    reinterpret_cast<cf::CtrlObjectParamVt008If*>(
        reinterpret_cast<u8*>(v125) + 0xC)->_v08();
    if (artsKey == 0) {
        void* bdat = reinterpret_cast<void*>(lbl_eu_80664090);
        const char* strBase = lbl_eu_804FBCB0;
        u32 c1 = getBdatStringColumnValue(bdat, &strBase[0x228], self->field_00);
        cf::CtrlObjectParamArtsDataV* data = reinterpret_cast<cf::CtrlObjectParamArtsDataV*>(
            self->mParam.CActorParam_UnkVirtualFunc125());
        data->arr[0] = (u8)c1;
        u32 c2 = getBdatStringColumnValue(bdat, &strBase[0x22D], self->field_00);
        data = reinterpret_cast<cf::CtrlObjectParamArtsDataV*>(
            self->mParam.CActorParam_UnkVirtualFunc125());
        data->arr[1] = (u8)c2;
        u32 c3 = getBdatStringColumnValue(bdat, &strBase[0x232], self->field_00);
        data = reinterpret_cast<cf::CtrlObjectParamArtsDataV*>(
            self->mParam.CActorParam_UnkVirtualFunc125());
        data->arr[2] = (u8)c3;
        data = reinterpret_cast<cf::CtrlObjectParamArtsDataV*>(
            self->mParam.CActorParam_UnkVirtualFunc125());
        data->arr[3] = 0;
        data = reinterpret_cast<cf::CtrlObjectParamArtsDataV*>(
            self->mParam.CActorParam_UnkVirtualFunc125());
        data->arr[4] = 0;
        data = reinterpret_cast<cf::CtrlObjectParamArtsDataV*>(
            self->mParam.CActorParam_UnkVirtualFunc125());
        data->arr[5] = 0;
    } else {
        u32 prefixA = lbl_eu_806667A8;
        u32 prefixB = lbl_eu_806667AC;
        void* bdat = reinterpret_cast<void*>(lbl_eu_806640F4);
        int loop = 0;
        do {
            if (loop >= 3) {
                cf::CtrlObjectParamArtsDataV* data = reinterpret_cast<cf::CtrlObjectParamArtsDataV*>(
                    self->mParam.CActorParam_UnkVirtualFunc125());
                data->arr[loop] = 0;
            } else {
                cf::CtrlObjectParamColName col;
                col.words[0] = prefixA;
                col.words[1] = prefixB;
                col.bytes[3] = (u8)((loop + 1) - ((loop + 1) / 10) * 10 + 0x30);
                u32 c = getBdatStringColumnValue(bdat, reinterpret_cast<const char*>(&col), artsKey);
                cf::CtrlObjectParamArtsDataV* data = reinterpret_cast<cf::CtrlObjectParamArtsDataV*>(
                    self->mParam.CActorParam_UnkVirtualFunc125());
                data->arr[loop] = (u8)c;
            }
            loop++;
        } while (loop < 6);
        func_8003AA34();
        const char* strBase = lbl_eu_804FBCB0;
        void* fp = getFP__FPCc(&strBase[0x237]);
        f32 f30 = lbl_eu_806667A0;
        f32 f31 = lbl_eu_8066A20C;
        for (int i = 0; i < 6; i++) {
            cf::CtrlObjectParamArtsDataV* data1 = reinterpret_cast<cf::CtrlObjectParamArtsDataV*>(
                self->mParam.CActorParam_UnkVirtualFunc125());
            u16 slot = data1->arr[i];
            cf::CtrlObjectParamArtsDataV* data2 = reinterpret_cast<cf::CtrlObjectParamArtsDataV*>(
                self->mParam.CActorParam_UnkVirtualFunc125());
            cf::CtrlObjectParamAtkView* atk = reinterpret_cast<cf::CtrlObjectParamAtkView*>(
                getAtkParam(data2, i));
            if (slot == 0) {
                reinterpret_cast<cf::CtrlObjectParamVt008If*>(
                    reinterpret_cast<u8*>(atk) + 0x84)->_v08();
            } else {
                char* name = reinterpret_cast<char*>(
                    getBdatStringColumnValue(fp, &strBase[0x86], slot));
                atk->field_20 = strlen(name);
                strcpy(atk->name, name);
                u32 v2 = getBdatStringColumnValue(fp, &strBase[0x00], slot);
                atk->field_78 = (u16)v2;
                atk->field_2C = f30;
                atk->field_30 = f30;
                u32 v3 = getBdatStringColumnValue(fp, &strBase[0x11C], slot);
                atk->field_36 = (s16)(s8)(u8)v3;
                u32 v4 = getBdatStringColumnValue(fp, &strBase[0x23E], slot);
                atk->field_38 = (u8)v4;
                atk->field_3A = 0;
                atk->field_40 = 0;
                atk->field_42 = 0;
                atk->field_3C = 0;
                atk->field_43 = 1;
                atk->field_44 = 1;
                atk->field_46 = 0;
                atk->field_58 = 0;
                atk->field_5A = 0;
                atk->field_5C = 1;
                atk->field_5E = 0;
                f32 v = *(const f32*)self->mParam.CActorParam_UnkVirtualFunc76();
                atk->field_64 = (s16)(v * f31);
                u32 v5 = getBdatStringColumnValue(fp, &strBase[0x220], slot);
                atk->field_76 = (u8)v5;
                atk->field_77 = (u8)v5;
                if (self->field_E6 & 8) atk->field_78 |= 0x200;
                if (self->field_E6 & 7) atk->field_78 |= 0x100;
            }
        }
        void* actor = func_800B8B94(self->field_00);
        if (actor != 0) {
            void* actor2 = func_800B8B94(self->field_00);
            if (actor2 != 0) {
                u8* value = reinterpret_cast<cf::CtrlObjectParamVt028CIf*>(actor2)->_v028C();
                func_80175A50(value, reinterpret_cast<u8*>(&self->mParam));
            }
        }
    }
}

void* cf::CActorParam::CActorParam_UnkVirtualFunc125() {
    return reinterpret_cast<cf::CActorParamRetailView*>(this)->field_2740;
}

void* cf::CActorParam::CActorParam_UnkVirtualFunc76() {
    return &reinterpret_cast<cf::CActorParamRetailView*>(this)->field_1830;
}

extern "C" u32 func_800A082C(void* selfV) {
    cf::CtrlObjectParamActorOwner* self = reinterpret_cast<cf::CtrlObjectParamActorOwner*>(selfV);
    // Virtual dispatch to CActorParam_UnkVirtualFunc94 (vtable slot 0x20C)
    // through the param embedded at +0x17C; the caller uses the low 16 bits
    // of the returned struct's first word.
    cf::CtrlObjectParamWordView* p =
        reinterpret_cast<cf::CtrlObjectParamWordView*>(self->mParam.CActorParam_UnkVirtualFunc94());
    return p->word0 & 0xFFFF;
}

// ── func_800A0860 (us-800a1238) ───────────────────────────────────────────
// Entry init: fill the stats record (vtable slot 0x20C result) from the
// arts-stat bdat columns (byte/float/short fields), refresh the embedded
// CActorParam from the actor, then ramp the arts level from the current
// value up to `val` (one step per outer-loop trip via func_800A0E64),
// refreshing the D4/D6 display fields from the item impl's label tables
// when the E6 bit-2 flag is set, and finally copy the record to the
// slot-0x224 result and repeat the D4/D6 refresh for the (new) item.
extern "C" void __declspec(noinline) func_800A0860(void* selfV, u16 val) {
    cf::CtrlObjectParamSlotView* self = reinterpret_cast<cf::CtrlObjectParamSlotView*>(selfV);
    union { u32 w[2]; f64 d; } cD6;   // field_10 + D6 conversion
    union { u32 w[2]; f64 d; } cD4;   // D4 conversion
    cD6.w[0] = 0x43300000;            // u32->f64 conversion magic
    void* bdat = reinterpret_cast<void*>(lbl_eu_80664090);
    cD4.w[0] = 0x43300000;
    cf::CtrlObjectParamStats* p = reinterpret_cast<cf::CtrlObjectParamStats*>(
        self->mParam.CActorParam_UnkVirtualFunc94());
    p->field_00 = (u8)getBdatStringColumnValue(bdat, &lbl_eu_804FBCB0[0x83], self->field_00[0]);
    cD6.w[1] = (u16)getBdatStringColumnValue(bdat, &lbl_eu_804FBCB0[0xE2], self->field_00[0]);
    p->field_10 = (f32)(cD6.d - lbl_eu_80666778);
    volatile u32 v2 = getBdatStringColumnValue(bdat, &lbl_eu_804FBCB0[0xE5], self->field_00[0]);
    p->field_1C = (s16)v2;
    volatile u32 v3 = getBdatStringColumnValue(bdat, &lbl_eu_804FBCB0[0xE9], self->field_00[0]);
    p->field_1E = (s16)v3;
    volatile u32 v4 = getBdatStringColumnValue(bdat, &lbl_eu_804FBCB0[0xED], self->field_00[0]);
    p->field_20 = (s16)v4;
    void* actor = func_800B8B94(self->field_00[0]);
    if (actor != 0) {
        void* actor2 = func_800B8B94(self->field_00[0]);
        if (actor2 != 0) {
            func_801765A4(actor2, lbl_eu_806667A0, 0);
            u8* value = reinterpret_cast<cf::CtrlObjectParamVt028CIf*>(actor2)->_v028C();
            func_80175A50(reinterpret_cast<u8*>(&self->mParam), value);
        } else {
            reinterpret_cast<cf::CtrlObjectParamVt0A4If*>(&self->mParam)->_v0A4(self);
        }
    }
    u16 count = (u16)p->field_00;
    f64 f29 = lbl_eu_80666788;   // 2^52 u32->f64 conversion constant
    f32 f30 = lbl_eu_80666780;   // D4 scale
    f32 f31 = lbl_eu_80666784;   // D6 scale
    int cap = 0x3E7;             // display-field cap
    while ((u32)count < val) {
        cf::CtrlObjectParamWordView* v94 = reinterpret_cast<cf::CtrlObjectParamWordView*>(
            self->mParam.CActorParam_UnkVirtualFunc94());
        u16 target = (u16)(v94->word0 + 1);
        u32 cur = v94->word0;
        while (cur < target && cur < 0x63) {
            func_800A0E64(reinterpret_cast<u8*>(self), (u16)(cur + 1));
            cur = v94->word0;
        }
        void* item = 0;
        if (self->shortArr[5] > -1) {
            item = func_80157C4C(2, self->shortArr[5]);
        }
        if (item != 0) {
            cf::CtrlObjectParamWordView* v94b = reinterpret_cast<cf::CtrlObjectParamWordView*>(
                self->mParam.CActorParam_UnkVirtualFunc94());
            u16 w0 = (u16)v94b->word0;
            if ((self->field_E6 & 4) != 0) {
                u16 v1 = reinterpret_cast<cf::CtrlObjectParamVt028If*>(
                    CItem_initItemImplInstances(item))->_v028(item, &lbl_eu_804FBCB0[0x22]);
                cD4.w[1] = (u32)(w0 * v1) ^ 0x80000000;
                self->field_D4 = (s16)(f30 * (f32)(cD4.d - f29));
                u16 v2 = reinterpret_cast<cf::CtrlObjectParamVt028If*>(
                    CItem_initItemImplInstances(item))->_v028(item, &lbl_eu_804FBCB0[0x2A]);
                cD6.w[1] = (u32)(w0 * v2) ^ 0x80000000;
                self->field_D6 = (s16)(f31 * (f32)(cD6.d - f29));
                if (self->field_D4 > cap) self->field_D4 = cap;
                if (self->field_D6 > cap) self->field_D6 = cap;
            }
        }
        if (actor != 0) {
            void* a3 = func_800B8B94(self->field_00[0]);
            if (a3 != 0) {
                u8* value = reinterpret_cast<cf::CtrlObjectParamVt028CIf*>(a3)->_v028C();
                func_80175A50(value, reinterpret_cast<u8*>(&self->mParam));
            }
        }
        count++;
        func_800A145C(reinterpret_cast<cf::CtrlObjectParamArtsLearnView*>(self));
    }
    p->field_04 = p->field_10;
    cf::CtrlObjectParamStats* other = reinterpret_cast<cf::CtrlObjectParamStats*>(
        self->mParam.CActorParam_UnkVirtualFunc100());
    *other = *p;
    void* item2 = 0;
    if (self->shortArr[5] > -1) {
        item2 = func_80157C4C(2, self->shortArr[5]);
    }
    if (item2 != 0) {
        cf::CtrlObjectParamWordView* v94 = reinterpret_cast<cf::CtrlObjectParamWordView*>(
            self->mParam.CActorParam_UnkVirtualFunc94());
        u16 w0 = (u16)v94->word0;
        if ((self->field_E6 & 4) != 0) {
            u16 v1 = reinterpret_cast<cf::CtrlObjectParamVt028If*>(
                CItem_initItemImplInstances(item2))->_v028(item2, &lbl_eu_804FBCB0[0x22]);
            cD4.w[1] = (u32)(w0 * v1) ^ 0x80000000;
            self->field_D4 = (s16)(lbl_eu_80666780 * (f32)(cD4.d - lbl_eu_80666788));
            u16 v2 = reinterpret_cast<cf::CtrlObjectParamVt028If*>(
                CItem_initItemImplInstances(item2))->_v028(item2, &lbl_eu_804FBCB0[0x2A]);
            cD6.w[1] = (u32)(w0 * v2) ^ 0x80000000;
            self->field_D6 = (s16)(lbl_eu_80666784 * (f32)(cD6.d - lbl_eu_80666788));
            if (self->field_D4 > 0x3e7) self->field_D4 = 0x3e7;
            if (self->field_D6 > 0x3e7) self->field_D6 = 0x3e7;
        }
    }
}

void* cf::CActorParam::CActorParam_UnkVirtualFunc100() {
    return &reinterpret_cast<cf::CActorParamRetailView*>(this)->field_17E4;
}

// ── func_800A0E64 (us-800a183c) ───────────────────────────────────────────
// Arts-stat ramp: resolve the stat record via vtable slot 0x20C, bail when
// the head equals the target level (or the target exceeds 99), then walk the
// 4-column stat table, ramping each stat field toward the target level with
// the f32 interpolation (2^52 u32->f64 conversions, shared by the two union
// slots), and finally copy the record to the slot-0x224 result object.
extern "C" void __declspec(noinline) func_800A0E64(u8* selfV, u16 value) {
    cf::CtrlObjectParamArtsOwner0E64* self =
        reinterpret_cast<cf::CtrlObjectParamArtsOwner0E64*>(selfV);
    // Two pre-magic-initialized u32->f64 conversion slots (retail stores
    // 0x43300000 into both stack slots once, before the slot-0x20C call).
    union { u32 w[2]; f64 d; } convDiff;     // holds diff / delta
    union { u32 w[2]; f64 d; } convCurVal;   // reused for current then value
    convCurVal.w[0] = 0x43300000;
    convDiff.w[0] = 0x43300000;
    cf::CtrlObjectParamStats* param = reinterpret_cast<cf::CtrlObjectParamStats*>(
        reinterpret_cast<cf::CtrlObjectParamVt0A4If*>(&self->mParam)->_v20C());
    if (param->field_00 == value) return;
    if (value > 99) return;
    // Loop-invariant handles/constants: declared before the loop so they are
    // live across getBdatStringColumnValue/__cvt_fp2unsigned calls and land
    // in callee-saved registers (r27/f28-f31, matching retail).
    const char* const* pCol = reinterpret_cast<const char* const*>(lbl_eu_805280D8);
    void* statBdat = lbl_eu_80664090;
    f64 convK = lbl_eu_80666778;
    // Declared in retail FPR-load order.
    f32 cScale = lbl_eu_806667BC;
    f32 cBase = lbl_eu_806667B8;
    f32 cDiv = lbl_eu_806667C0;
    u32 i = 0;
    do {
        u16 colByte = getBdatStringColumnValue(statBdat, pCol[i], self->field_00);
        u32 current = 0;
        switch (i) {
        case 0: current = (u32)param->field_10; break;
        case 1: current = param->field_1C; break;
        case 2: current = param->field_1E; break;
        case 3: current = param->field_20; break;
        }
        convCurVal.w[1] = current;
        f32 fCur = (f32)(convCurVal.d - convK);
        u32 diff = colByte - current;
        convDiff.w[1] = diff;
        convCurVal.w[1] = value;
        f32 fVal = (f32)(convCurVal.d - convK);
        f32 fDiff = (f32)(convDiff.d - convK);
        u32 newVal = (u32)(fCur + ((cBase + ((cScale * fDiff) / (cScale - fVal))) / cDiv));
        u32 delta = newVal - current;
        switch (i) {
        case 0:
            convDiff.w[1] = delta;
            param->field_10 += (f32)(convDiff.d - convK);
            break;
        case 1: param->field_1C = param->field_1C + (s16)delta; break;
        case 2: param->field_1E = param->field_1E + (s16)delta; break;
        case 3: param->field_20 = param->field_20 + (s16)delta; break;
        }
        ++i;
    } while (i < 4);
    param->field_00 = value;
    cf::CtrlObjectParamStats* other = reinterpret_cast<cf::CtrlObjectParamStats*>(
        reinterpret_cast<cf::CtrlObjectParamVt0A4If*>(&self->mParam)->_v224());
    *other = *param;
}

void func_800A11A4(cf::CtrlObjectParamEntry11A4* self, int amount) {
    // Level-up helper: raise the actor param's arts/status word by `amount`
    // (one step per loop, capped at 99), then refresh the D4/D6 display
    // fields from the item impl's label tables when the E6 bit-2 flag is set.
    int amt = amount;
    union { u32 w[2]; double d; } c1;
    union { u32 w[2]; double d; } c2;
    cf::CtrlObjectParamWordView* v94 = reinterpret_cast<cf::CtrlObjectParamWordView*>(
        self->mParam.CActorParam_UnkVirtualFunc94());
    u16 target = (u16)(v94->word0 + amt);
    while (amt != 0 && (u32)v94->word0 < target && (u32)v94->word0 < 0x63) {
        func_800A0E64(reinterpret_cast<u8*>(self), (u16)(v94->word0 + 1));
    }
    void* item = 0;
    if (self->shortArr[5] > -1) {
        item = func_80157C4C(2, self->shortArr[5]);
    }
    if (item != 0) {
        cf::CtrlObjectParamWordView* v94b = reinterpret_cast<cf::CtrlObjectParamWordView*>(
            self->mParam.CActorParam_UnkVirtualFunc94());
        u16 w0 = (u16)v94b->word0;
        if ((self->field_E6 & 4) != 0) {
            const char* s1 = &lbl_eu_804FBCB0[0x22];
            cf::CtrlObjectParamVt028If* impl1 = reinterpret_cast<cf::CtrlObjectParamVt028If*>(
                CItem_initItemImplInstances(item));
            u16 v1 = impl1->_v028(item, s1);
            c1.w[0] = 0x43300000;
            c1.w[1] = (u32)(w0 * v1) ^ 0x80000000;
            self->field_D4 = (s16)(lbl_eu_80666780 * (f32)(c1.d - lbl_eu_80666788));
            const char* s2 = &lbl_eu_804FBCB0[0x2a];
            cf::CtrlObjectParamVt028If* impl2 = reinterpret_cast<cf::CtrlObjectParamVt028If*>(
                CItem_initItemImplInstances(item));
            u16 v2 = impl2->_v028(item, s2);
            c2.w[0] = 0x43300000;
            c2.w[1] = (u32)(w0 * v2) ^ 0x80000000;
            self->field_D6 = (s16)(lbl_eu_80666784 * (f32)(c2.d - lbl_eu_80666788));
            if (self->field_D4 > 0x3e7) self->field_D4 = 0x3e7;
            if (self->field_D6 > 0x3e7) self->field_D6 = 0x3e7;
        }
    }
}

extern "C" void func_800A1370(cf::CtrlObjectParamArtsView* self) {
    // Resolve the actor for this arts type id, then hand the vtable-0x28C
    // result (a pointer) plus the arts-data write target at +0x17C to
    // func_80175A50 (retail arg order: value in r3, obj in r4).
    u8* obj = reinterpret_cast<u8*>(func_800B8B94(self->field_00));
    if (obj != 0) {
        u8* value = reinterpret_cast<cf::CtrlObjectParamVt028CIf*>(obj)->_v028C();
        func_80175A50(value, &self->field_17C);
    }
}

void CActorParam_UnkVirtualFunc126__Q22cf11CActorParamFv() {}

// Resolve the actor for this arts type id, then hand the vtable-0x28C result
// (a pointer) plus the arts-data write target at +0x17C to func_80175A50
// (retail arg order: value in r3, obj in r4). When no actor exists, activate
// the embedded CActorParam at +0x17C through its vtable-0xA4 slot instead.
void func_800A13C4(cf::CtrlObjectParamArtsView* self, u32 arg2) {
    void* actor = func_800B8B94(self->field_00);
    if (actor != 0) {
        func_801765A4(actor, lbl_eu_806667A0, arg2);
        u8* value = reinterpret_cast<cf::CtrlObjectParamVt028CIf*>(actor)->_v028C();
        func_80175A50(reinterpret_cast<u8*>(&self->field_17C), value);
    } else {
        reinterpret_cast<cf::CtrlObjectParamVt0A8If*>(
            &reinterpret_cast<cf::CtrlObjectParamActorOwner*>(self)->mParam)
            ->_v0A4(reinterpret_cast<cf::CtrlObjectParamActorOwner*>(self));
    }
}

// ── func_800A145C (us-800a1e34) ───────────────────────────────────────────
// Arts-learn flow: gate on the party-member frame count (type 3 / state
// 0x1D), refresh the embedded CActorParam from the actor (or the 0xA4
// slot), then walk the 3x8 column-name grid, claiming an empty arts slot
// in the 0x49-stride row selected by the +0x58 column byte. The claim
// scans the two offset tables (lbl_eu_804FBC70 / lbl_eu_804FBC50) and
// stamps the slot record via getArtsParamRC2 / setArtsSlotRC.
extern "C" u8 func_800A145C(cf::CtrlObjectParamArtsLearnView* self) {
    u8 learnFlag = 0;
    if (self->field_00 == 3) {
        if (cf::CfGameManager::func_800822F4() >= 0x1D) {
            return 0;
        }
    }
    void* actor1 = func_800B8B94(self->field_00);
    void* actor2 = func_800B8B94(self->field_00);
    if (actor2 != 0) {
        func_801765A4(actor2, lbl_eu_806667A0, 0);
        u8* value = reinterpret_cast<cf::CtrlObjectParamVt028CIf*>(actor2)->_v028C();
        func_80175A50(value, reinterpret_cast<u8*>(&self->mParam));
    } else {
        reinterpret_cast<cf::CtrlObjectParamVt0A4If*>(&self->mParam)->_v0A4(self);
    }
    func_8003AA34();
    const char* strBase = lbl_eu_804FBCB0;
    void* fp = getFP__FPCc(&strBase[0x101]);
    u16 rowKey = self->field_0C;
    void* itemBdat = reinterpret_cast<void*>(lbl_eu_806640F4);
    u32 colv = getBdatStringColumnValue(itemBdat, &strBase[0x58], rowKey);
    u8 rowIdx = (u8)colv;
    u32 c8 = lbl_eu_806667C8;
    u32 cc = lbl_eu_806667CC;
    const int one = 1;
    u8* rows = reinterpret_cast<u8*>(self) + rowIdx * 0x49 + 0xE8;
    // Snapshot the two slot offset tables (per-word retail lwzu/stw shape).
    u32 tbl70[8];
    u32 tbl50[8];
    tbl70[0] = lbl_eu_804FBC70[0];
    tbl70[1] = lbl_eu_804FBC70[1];
    tbl70[2] = lbl_eu_804FBC70[2];
    tbl70[3] = lbl_eu_804FBC70[3];
    tbl70[4] = lbl_eu_804FBC70[4];
    tbl70[5] = lbl_eu_804FBC70[5];
    tbl70[6] = lbl_eu_804FBC70[6];
    tbl70[7] = lbl_eu_804FBC70[7];
    tbl50[0] = lbl_eu_804FBC50[0];
    tbl50[1] = lbl_eu_804FBC50[1];
    tbl50[2] = lbl_eu_804FBC50[2];
    tbl50[3] = lbl_eu_804FBC50[3];
    tbl50[4] = lbl_eu_804FBC50[4];
    tbl50[5] = lbl_eu_804FBC50[5];
    tbl50[6] = lbl_eu_804FBC50[6];
    tbl50[7] = lbl_eu_804FBC50[7];
    for (int r19 = 0; r19 < 3; r19++) {
        u8 rowCh = (u8)(r19 + 0x31);
        for (int r18 = 0; r18 < 8; r18++) {
            cf::CtrlObjectParamColName col;
            col.words[0] = c8;
            col.words[1] = cc;
            col.bytes[4] = rowCh;
            col.bytes[5] = (u8)(r18 + 0x31);
            u32 c = getBdatStringColumnValue(itemBdat, reinterpret_cast<const char*>(&col), rowKey);
            u8 artsId = (u8)c;
            if (artsId == 0) continue;
            u32 v = getBdatStringColumnValue(fp, &strBase[0x109], artsId);
            u8 col2 = (u8)v;
            u8* slotRow = rows + col2 * 2;
            if (self->field_00 == 8 && r19 == 2) {
                u16 level = func_80158018(self->field_0A);
                void* armorBdat = reinterpret_cast<void*>(lbl_eu_806640F8);
                if ((u16)level == 0) {
                    rows[0x41] = artsId;
                    rows[0x30] = 1;
                } else {
                    u32 v2 = getBdatStringColumnValue(armorBdat, &strBase[0x66], (u16)level);
                    artsId = (u8)(artsId + (u8)v2 - 4);
                    if (*slotRow != 0) {
                        rows[0x41] = artsId;
                        rows[0x30] = 1;
                    }
                }
            }
            if (*slotRow != 0) continue;
            u32 v3 = getBdatStringColumnValue(fp, &strBase[0x243], artsId);
            u32 v4 = getBdatStringColumnValue(fp, &strBase[0x24C], artsId);
            u8 r16 = (u8)v4;
            if ((u8)v3 != 1) continue;
            if (self->mParam.CActorParam_UnkVirtualFunc26() < r16) continue;
            learnFlag = 1;
            // Copy the offset tables to slot arrays, then scan the 8 slots.
            u32 slotA[8];
            u32 slotB[8];
            for (int k = 0; k < 8; k++) {
                slotA[k] = tbl70[k];
                slotB[k] = tbl50[k];
            }
            *slotRow = 1;
            int claimed = 0;
            for (int k = 0; k < 8; k++) {
                int r16b = (r19 == 2) ? 2 : 0;
                u32 off = (r19 == 2) ? slotA[k] : slotB[k];
                if (self->field_00 == 1) r16b = r19;
                u8* slot = rows + r16b * 8 + off + 0x31;
                if (*slot == 0) {
                    *slot = artsId;
                    rows[0x30] = 1;
                    void* artsSet = self->mParam.CActorParam_UnkVirtualFunc122();
                    cf::CtrlObjectParamArtsRecRC2* rec = reinterpret_cast<cf::CtrlObjectParamArtsRecRC2*>(
                        getArtsParamRC2(artsSet, r16b, off));
                    rec->field_88 = reinterpret_cast<u32>(slotRow);
                    artsSet = self->mParam.CActorParam_UnkVirtualFunc122();
                    setArtsSlotRC(artsSet, (u16)artsId, (u16)r16b, (u16)off);
                    claimed = 1;
                    break;
                }
            }
            reinterpret_cast<cf::CtrlObjectParamHelpFlag*>(lbl_eu_80664A10)->field_16 = 1;
        }
    }
    if (actor1 != 0) {
        reinterpret_cast<cf::CtrlObjectParamVt0A4If*>(actor1)->_v032C();
    }
    return learnFlag;
}

u32 cf::CActorParam::CActorParam_UnkVirtualFunc26() {
    return *reinterpret_cast<u32*>(reinterpret_cast<cf::CActorParamRetailView*>(this)->field_1650);
}

void CActorParam_UnkVirtualFunc166__Q22cf11CActorParamFv() {}

// ── func_800A18A4 (us-800a227c) ───────────────────────────────────────────
// Arts-slot update: resolve the actor for the type id, refresh the embedded
// CActorParam (via func_80175A50 or the 0xA4 slot), then claim an arts-slot
// row in the 0x49-stride table at +0xE8: the bdat columns +0x58/+0x253/+0x109
// select the row and the (offset-table) slot index; an empty slot (byte at
// base+elem*8+flag*8+0x31 == 0) is stamped with arg2, else a 0x76 fallback
// writes the two byte slots directly. Finally re-arms the arts list via
// __ct__8009F8B8 and refreshes the actor param again.
extern "C" void func_800A18A4(cf::CtrlObjectParamArtsSlotOwner* self, int arg2) {
    u8* rows;
    if (arg2 == 0) return;
    void* actor = func_800B8B94(self->field_00);
    void* actor2 = func_800B8B94(self->field_00);
    if (actor2 != 0) {
        func_801765A4(actor2, lbl_eu_806667A0, 0);
        u8* value = reinterpret_cast<cf::CtrlObjectParamVt0A4If*>(actor2)->_v028C();
        func_80175A50(reinterpret_cast<u8*>(&self->mParam), value);
    } else {
        reinterpret_cast<cf::CtrlObjectParamVt0A4If*>(&self->mParam)->_v0A4(self);
    }
    const char* strBase = lbl_eu_804FBCB0;   // materialized before func_8003AA34 (retail lis/addi r22)
    u16 rowKey = self->field_0C;
    rows = &self->rows[0];
    func_8003AA34();
    void* fp = getFP__FPCc(&strBase[0x101]);
    volatile union { u32 w; u8 b[4]; } v;
    v.w = getBdatStringColumnValue(reinterpret_cast<void*>(lbl_eu_806640F4),
                                   &strBase[0x58], rowKey);
    u8 rowIdx = v.b[0];
    getBdatStringColumnValue(fp, &strBase[0x253], arg2);
    volatile union { u32 w; u8 b[4]; } v3;
    v3.w = getBdatStringColumnValue(fp, &strBase[0x109], arg2);
    u8 col = v3.b[0];
    if (rows[rowIdx * 0x49 + col * 2] != 0) return;

    // 8-word offset table + per-slot claim scan. The flag switches the slot
    // offset by 8 when the type id is 1 and the column byte exceeds 7.
    // base = rows + rowIdx*0x49 is re-derived per use (retail keeps rows in
    // r31 and the offset in r0).
    int found = 0;
    int i = 0;
    CtrlObjectParamOffsetTable stackArr = lbl_eu_804FBC90;
    rows[rowIdx * 0x49 + col * 2] = 1;
    for (; i < 8; i++) {
        int flag = ((u16)self->field_00 == 1 && col > 7) ? 1 : 0;
        int elem = stackArr.words[i];
        u8* slot = rows + rowIdx * 0x49 + flag * 8 + elem + 0x31;
        if (*slot == 0) {
            *(u8*)(rows + rowIdx * 0x49 + flag * 8 + stackArr.words[i] + 0x31) =
                (u8)arg2;
            rows[rowIdx * 0x49 + 0x30] = 1;
            found = 1;
            break;
        }
    }
    if (!found && arg2 == 0x76) {
        rows[rowIdx * 0x49 + 0x38] = (u8)arg2;
        rows[rowIdx * 0x49 + 0x30] = 1;
    }
    __ct__8009F8B8(self);
    if (actor != 0) {
        void* a3 = func_800B8B94(self->field_00);
        if (a3 != 0) {
            u8* value = reinterpret_cast<cf::CtrlObjectParamVt0A4If*>(a3)->_v028C();
            func_80175A50(value, reinterpret_cast<u8*>(&self->mParam));
        }
        reinterpret_cast<cf::CtrlObjectParamVt0A4If*>(actor)->_v032C();
    }
    reinterpret_cast<cf::CtrlObjectParamHelpFlag*>(lbl_eu_80664A10)->field_16 = 1;
}

// func_800A1E3C (char arts/level helper) is defined below; func_800A21F8
// (rank/level helper) is a stub in this TU but both are called by
// func_800A26A4 below; the retail call-site relocs use the plain names, so
// the stub carries the real parameter list. noinline: an empty body would
// otherwise be folded into the caller and the retail `bl func_800A21F8`
// call site would vanish.
// ── func_800A21F8 (us-800a2bd0) ───────────────────────────────────────────
// Arts rank/level helper: gate on the party-member frame count (type 3 /
// state 0x1D), forward the change to the actor (slot 0x1F4) when present,
// else apply it to the embedded CActorParam (slots 0x1DC/0x1FC) and the
// arts-set helper func_802617B8. The loop then levels the arts (slot 0xFC
// count vs the 0x263 column), refreshes the D4/D6 display fields from the
// item impl labels when the E6 bit-2 flag is set, and gates the
// arts-change write (func_8026187C) on party membership.
extern "C" __declspec(noinline) void func_800A21F8(void* selfV, u32 value, u32 a, u32 b) {
    cf::CtrlObjectParamTypeView* self = reinterpret_cast<cf::CtrlObjectParamTypeView*>(selfV);
    cf::CtrlObjectParamArtsRankView* view =
        reinterpret_cast<cf::CtrlObjectParamArtsRankView*>(self);
    if (self->field_00 == 3) {
        if (cf::CfGameManager::func_800822F4() >= 0x1D) return;
    }
    void* actor = func_800B8B94(self->field_00);
    if (actor != 0) {
        reinterpret_cast<cf::CtrlObjectParamVt0A4If*>(actor)->_v1F4(value, a, b);
        return;
    }
    self->mParam.CActorParam_UnkVirtualFunc82(value);
    self->mParam.CActorParam_UnkVirtualFunc90(a);
    func_802617B8(&self->big[0], view->field_3DD0, b);
    const char* strBase = lbl_eu_804FBCB0;
    f64 f29 = lbl_eu_80666788;
    f32 f30 = lbl_eu_80666780;
    f32 f31 = lbl_eu_80666784;
    int repeat = 0;
    u32 magic = 0x43300000;
    int cap = 0x3E7;
    for (;;) {
        void* artsBdat = reinterpret_cast<void*>(lbl_eu_806640DC);
        u32 level = self->mParam.CActorParam_UnkVirtualFunc26();
        int r28;
        if ((u32)(level + 1) > 0x63) {
            r28 = 1;
        } else {
            u32 col = getBdatStringColumnValue(artsBdat, &strBase[0x263], (u32)(level + 1));
            r28 = (int)col - (int)self->mParam.CActorParam_UnkVirtualFunc85();
        }
        if (r28 > 0) break;
        cf::CtrlObjectParamWordView* v94 = reinterpret_cast<cf::CtrlObjectParamWordView*>(
            self->mParam.CActorParam_UnkVirtualFunc94());
        u16 target = (u16)(v94->word0 + 1);
        u32 cur = v94->word0;
        while (cur < target && cur < 0x63) {
            func_800A0E64(reinterpret_cast<u8*>(self), (u16)(cur + 1));
            cur = v94->word0;
        }
        void* item = 0;
        if (self->shortArr[5] > -1) {
            item = func_80157C4C(2, self->shortArr[5]);
        }
        if (item != 0) {
            cf::CtrlObjectParamWordView* v94b = reinterpret_cast<cf::CtrlObjectParamWordView*>(
                self->mParam.CActorParam_UnkVirtualFunc94());
            u16 w0 = (u16)v94b->word0;
            if ((self->field_E6 & 4) != 0) {
                union { u32 w[2]; f64 d; } c1;
                union { u32 w[2]; f64 d; } c2;
                u16 v1 = reinterpret_cast<cf::CtrlObjectParamVt028If*>(
                    CItem_initItemImplInstances(item))->_v028(item, &strBase[0x22]);
                c1.w[0] = magic;
                c1.w[1] = (u32)(w0 * v1) ^ 0x80000000;
                self->field_D4 = (s16)(f30 * (f32)(c1.d - f29));
                u16 v2 = reinterpret_cast<cf::CtrlObjectParamVt028If*>(
                    CItem_initItemImplInstances(item))->_v028(item, &strBase[0x2A]);
                c2.w[0] = magic;
                c2.w[1] = (u32)(w0 * v2) ^ 0x80000000;
                self->field_D6 = (s16)(f31 * (f32)(c2.d - f29));
                if (self->field_D4 > cap) self->field_D4 = cap;
                if (self->field_D6 > cap) self->field_D6 = cap;
            }
        }
        u32 v1E8 = self->mParam.CActorParam_UnkVirtualFunc85();
        self->mParam.CActorParam_UnkVirtualFunc83(r28 + v1E8);
        u32 type = self->field_00;
        int found = 0;
        if (type < 9) {
            found = isPartySlotMatch(type);
        }
        if (found == 0 && func_8008235C__Q22cf13CfGameManagerFv(type) == 0) {
            // not a party member: skip the arts-change gate
        } else {
            if (self->field_00 == 3) {
                if (cf::CfGameManager::func_800822F4() >= 0x1D) {
                    // skip
                } else {
                    func_8026187C(&self->big[0], 1);
                }
            } else {
                func_8026187C(&self->big[0], 1);
            }
        }
        self->mParam.CActorParam_UnkVirtualFunc81(-r28);
        repeat = 1;
    }
    if (repeat != 0) {
        func_800A145C(reinterpret_cast<cf::CtrlObjectParamArtsLearnView*>(self));
    }
    if (b != 0) {
        func_800A2AF0(self);
    }
}

// ── func_800A1E3C (us-800a2814) ───────────────────────────────────────────
// Party-member arts-level adjust: read the member's current arts level
// through vtable slot 0x108 (and the actor's, when one exists), accumulate
// the level/rank totals via func_800A1B08, then rescale *v1/*v2 by the
// level-difference windows (125%, 150%, half, two-thirds) and, for big
// level gaps, overwrite them from the arts bdat level columns. Finally
// scale by the slot-membership/flag-derived percentages and derive *v3
// (rank * average level / 10) unless arg7 disables it.
void func_800A1E3C(cf::CtrlObjectParamTypeView* self, int* v1, int* v2, int* v3,
                   int arg5, int arg6, int arg7) {
    int result = self->mParam.CActorParam_UnkVirtualFunc29();
    void* actor = func_800B8B94(self->field_00);
    if (actor != 0) {
        reinterpret_cast<cf::CtrlObjectParamVt0A4If*>(actor)->_v108();
    }
    int a = 0;
    int b = 0;
    func_800A1B08(self->field_00, &a, &b, result, arg5, arg6);
    if (arg5 > 0) {
        int diff = arg5 - result;
        if (diff > 2 && diff < 6) {
            *v1 = *v1 * 125 / 100;
            *v2 = *v2 * 125 / 100;
        } else if (diff > 5) {
            *v1 = *v1 * 150 / 100;
            *v2 = *v2 * 150 / 100;
        } else if (diff > -6 && diff < -2) {
            *v1 = *v1 / 2;
            *v2 = *v2 / 2;
        } else if (diff < -5) {
            *v1 = *v1 * 2 / 3;
            *v2 = *v2 * 2 / 3;
        }
        if (diff > 0x15) {
            result += 20;
            if ((unsigned)result > 0x63) result = 0x63;
            void* bdat = reinterpret_cast<void*>(lbl_eu_806640DC);
            const char* strBase = lbl_eu_804FBCB0;
            u32 v1v = getBdatStringColumnValue(bdat, &strBase[0x256], result);
            *v1 = (u16)v1v;
            u32 v2v = getBdatStringColumnValue(bdat, &strBase[0x25D], result);
            *v2 = (u16)v2v;
        }
    }
    *v1 = *v1 * a / 100;
    *v2 = *v2 * b / 100;
    int tmpType = 0;
    int tmpIndex = 0;
    func_8009E344(reinterpret_cast<const unsigned int*>(lbl_eu_80663E88 + 0x1F98),
                  self->field_00, &tmpType, &tmpIndex);
    int levelA = (tmpType == 1) ? 100 : 90;
    int levelB = levelA;
    u8* lookup = reinterpret_cast<u8*>(lbl_eu_80663E88 + self->field_00 * 0x3DD4 + 0x7724);
    if (func_8026178C(lookup, 0x8B)) levelA = 100;
    if (func_8026178C(lookup, 0x8E)) levelB = 100;
    *v1 = *v1 * levelA / 100;
    *v2 = *v2 * levelB / 100;
    int rank = 10;
    int lvl = 0;
    if (arg7 == 0) {
        func_800A1CA0(reinterpret_cast<u8*>(self->field_00), &lvl, &rank, arg5, arg6);
        union { u32 w[2]; f64 d; } conv;
        conv.w[0] = 0x43300000;
        conv.w[1] = (u32)(rank * lvl) ^ 0x80000000;
        *v3 = (s32)((f32)(conv.d - lbl_eu_80666788) / lbl_eu_80666790);
        if (*v1 == 0 && *v2 == 0) {
            *v3 = 0;
        }
    } else {
        *v3 = 0;
    }
}
// Add the character's arts/status counters into *outA / *outB. The 4th/5th
// parameters (r6/r7) are unused by retail; the flag arrives in r8 (6th
// parameter) and switches the base values from 100/100 to 125/125.
extern "C" void func_800A1B08(u32 rowIndex, int* outA, int* outB,
                              int unkArg4, int unkArg5, int flag) {
    u32 work = lbl_eu_80663E88;
    u32 rowOff = (u16)rowIndex * 0x3DD4;
    u32 baseAddr = work + rowOff;
    void* lookup = reinterpret_cast<void*>(baseAddr + 0x7724);
    *outB = 100;
    *outA = 100;
    if (flag != 0) {
        *outA = 125;
        *outB += 25;
    }
    if (func_8026178C(lookup, 0x86)) {
        *outA += func_8025FB10(lookup, 0x86);
    }
    if (func_8026178C(lookup, 0x8D)) {
        *outB += func_8025FB10(lookup, 0x8D);
    }
    u32 gameState = cf::CfGameManager::func_80086DBC();
    if (func_8026178C(lookup, 0x87) && gameState != 4) {
        *outA += func_8025FB10(lookup, 0x87);
    }
    if (func_8026178C(lookup, 0x88) && gameState == 4) {
        *outA += func_8025FB10(lookup, 0x88);
    }
    u8* entryBase = reinterpret_cast<u8*>(lbl_eu_80663E88 + rowOff);
    if (func_80148778(entryBase + 0x4374, 0xB7)) {
        cf::CBattleStateEntry* entry = reinterpret_cast<cf::CBattleStateEntry*>(
            func_80149154(entryBase + 0x4374, 0xB7));
        *outA += entry->unk10;
    }
    if (func_80148778(entryBase + 0x4374, 0xB8)) {
        cf::CBattleStateEntry* entry = reinterpret_cast<cf::CBattleStateEntry*>(
            func_80149154(entryBase + 0x4374, 0xB8));
        *outB += entry->unk10;
    }
}

// ── func_800A1CA0 (us-800a2678) ───────────────────────────────────────────
// Compute the party average arts level (slotTable rows 0..2) and derive an
// out-level / out-rank pair from the caller's target level. The 0x3356
// resource lookup gates the whole computation; when no arts rows exist the
// rank falls back through a diff window chain.
extern "C" void func_800A1CA0(u8* self, int* outLevel, int* outRank,
                              int targetLevel, int flag) {
    int sum = 0;
    int count = 0;
    int i = 0;
    int off = 0;
    int tmpIndex = 0;
    int tmpType = 0;
    if (func_8009CF8C(0x3356) == 0) {
        *outLevel = 0;
        *outRank = 0;
        return;
    }
    *outLevel = 0;
    u8* base = reinterpret_cast<u8*>(lbl_eu_80663E88) + 0x1F98;
    int found = func_8009E344(reinterpret_cast<const unsigned int*>(base),
                              reinterpret_cast<unsigned int>(self), &tmpType, &tmpIndex);
    if (tmpType == 1) {
        *outLevel = 100;
    } else {
        *outLevel = 90;
    }
    *outRank = 10;
    if (targetLevel > 0 && found != 0) {
        while (i < 3) {
            cf::CtrlObjectParamSlotItem* item =
                reinterpret_cast<cf::CtrlObjectParamSlotItem*>(base + off);
            s32 id = (s32)item->field_04;
            if (id > 0) {
                cf::CtrlObjectParamArtsOwner* owner =
                    reinterpret_cast<cf::CtrlObjectParamArtsOwner*>(
                        reinterpret_cast<u8*>(lbl_eu_80663E88) + (u16)id * 0x3DD4);
                void* result = owner->mParam.CActorParam_UnkVirtualFunc94();
                ++count;
                sum += (u16)(reinterpret_cast<cf::CtrlObjectParamWordView*>(result)->word0);
            }
            ++i;
            off += 4;
        }
        int avg = (count > 0) ? sum / count : 0;
        int diff = targetLevel - avg;
        if (diff > 6) {
            *outRank = 20;
        } else if ((u32)(diff - 4) <= 2) {
            *outRank = 15;
        } else if ((u32)(diff + 6) <= 2) {
            *outRank = 5;
        } else if (diff < -6) {
            *outRank = 1;
        }
    }
    if (flag != 0) {
        *outLevel *= 2;
    }
}

void cf::CActorParam::CActorParam_UnkVirtualFunc82(u32 addend) {
    u32 cap = 0x05F60000 - 7937;
    u32 val = unk1604 + addend;
    unk1604 = val;
    if (val > cap) {
        unk1604 = cap;
    }
}

void cf::CActorParam::CActorParam_UnkVirtualFunc90(u32 addend) {
    u32 cap = 0x05F60000 - 7937;
    u32 val = unk1608 + addend;
    unk1608 = val;
    if (val > cap) {
        unk1608 = cap;
    }
}

// ── func_800A26A4 (us-800a307c) ───────────────────────────────────────────
// Party-member arts/level flow: verify the character row id is a party
// member (slotTable membership), then run the arts/level helper with the
// caller's out-pointer block; arg9 clears the two secondary outputs.
extern "C" void func_800A26A4(cf::CtrlObjectParamTypeView* self, int arg4, int arg5,
                              int arg6, int arg7, int arg8, int arg9) {
    int v1 = arg4;
    int v2 = arg5;
    int v3;
    u16 type = self->field_00;
    if (type >= 9) return;
    cf::CtrlObjectParamSlotTable* tbl =
        reinterpret_cast<cf::CtrlObjectParamSlotTable*>(lbl_eu_80663E88);
    int found = 0;
    for (int i = 0; i < 9; ++i) {
        if (type == tbl->slots[i]) {
            found = 1;
            break;
        }
    }
    if (!found) {
        if (func_8008235C__Q22cf13CfGameManagerFv(type) == 0) return;
    }
    if (self->field_00 == 3) {
        if (cf::CfGameManager::func_800822F4() >= 0x1D) return;
    }
    func_800B8B94(self->field_00);
    func_800A1E3C(self, &v1, &v2, &v3, arg6, arg7, arg8);
    if (arg9 != 0) {
        v2 = 0;
        v3 = 0;
    }
    func_800A21F8(self, v1, v2, v3);
}

void func_800A282C(){}

// ── func_800A2974 (us-800a334c) ───────────────────────────────────────────
// Arts-change gate: when the character row id is a party member (or the
// flag-bit test passes) and the sequence is past the 0x1D early frame,
// write the arts-set counter at self+0x3534.
//
// Party-membership probe shared (inlined) by func_800A282C / func_800A26A4 /
// func_800A2974 / func_800A2DE8: the slot-table chain at work+0x1F9C with
// the found flag in r0 and the second base materialized mid-chain.
static inline int isPartySlotMatch(int type) {
    cf::CtrlObjectParamSlotTable* tbl =
        reinterpret_cast<cf::CtrlObjectParamSlotTable*>(lbl_eu_80663E88);
    int* s = &tbl->slots[0];
    int found;
    if (type == s[0]) found = 1;
    else if (type == s[1]) found = 1;
    else if (type == s[2]) found = 1;
    else if (type == s[3]) found = 1;
    else if (type == s[4]) found = 1;
    else if (type == s[5]) found = 1;
    else if (type == s[6]) found = 1;
    else if (type == s[7]) found = 1;
    else found = 0;
    return found;
}

extern "C" void func_800A2974(void* selfV, u16 arg2) {
    cf::CtrlObjectParamTypeView* self = reinterpret_cast<cf::CtrlObjectParamTypeView*>(selfV);
    u16 type = self->field_00;
    if (type >= 9 || arg2 == 0) return;
    cf::CtrlObjectParamSlotTable* tbl =
        reinterpret_cast<cf::CtrlObjectParamSlotTable*>(lbl_eu_80663E88);
    int found = 0;
    for (int i = 0; i < 9; ++i) {
        if (type == tbl->slots[i]) {
            found = 1;
            break;
        }
    }
    if (!found) {
        if (func_8008235C__Q22cf13CfGameManagerFv(type) == 0) return;
    }
    if (self->field_00 == 3) {
        if (cf::CfGameManager::func_800822F4() >= 0x1D) return;
    }
    func_802618AC(self->big, arg2);
}

u32 cf::CActorParam::CActorParam_UnkVirtualFunc85() {
    return reinterpret_cast<cf::CActorParamRetailView*>(this)->field_1604;
}

void cf::CActorParam::CActorParam_UnkVirtualFunc83(u32 addend) {
    u32 cap = 0x05F60000 - 7937;
    u32 val = unk1600 + addend;
    unk1600 = val;
    if (val > cap) {
        unk1600 = cap;
    }
}

void cf::CActorParam::CActorParam_UnkVirtualFunc81(u32 val) {
    unk1604 = val;
}

// ── func_800A2AF0 (us-800a34c8) ───────────────────────────────────────
// Arts-set re-apply: walk the 11 x 0xC4 arts groups at +0x3534. For the
// group matching the selected row (field_3DD0), claim the first empty slot
// whose bdat cost (byte*100) fits the row's level cap at +0x3DBC: pay it via
// func_802617B8, fill the slot (func_8025F528), re-run this pass recursively,
// and reload the cap from the (possibly moved) selected row. Non-empty slots
// with a zero flag are re-armed via func_8025F2E8. Afterwards rebuild the
// arts set when any group is dirty or a claim happened, mirror it into the
// type-3/type-8 global tables, and tick func_80280F44.
extern "C" int func_800A2AF0(cf::CtrlObjectParamTypeView* selfV) {
    cf::CtrlObjectParamArtsRankView* view =
        reinterpret_cast<cf::CtrlObjectParamArtsRankView*>(selfV);
    int learned = 0;
    if (view->field_00 >= 9) return 0;
    if (view->field_00 == 3 && (u32)cf::CfGameManager::func_800822F4() >= 0x1D) return 0;

    // Lifetime/order mirror of the retail register map:
    //   row(r27) strBase(r22) table(r26) grpCursor(r31) cap(r28)
    //   rowSlotBase(r30) learned(r29) g(r25) s(r21)
    u32 row = view->field_3DD0;
    const char* strBase = lbl_eu_804FBCB0;
    void* table = reinterpret_cast<void*>(lbl_eu_80664158);
    u8* grpCursor = reinterpret_cast<u8*>(view) + 0x3534;
    int cap = *(const int*)(reinterpret_cast<const u8*>(view) + row * 4 + 0x3DBC);
    u32 rowSlotBase = row * 4 + row;
    u32 g = 0;
    do {
        u32 s = 0;
        u8* rec = grpCursor;
        do {
            if (*(const u32*)(rec + 0x20) == 0) {
                if (row == g) {
                    // Column index: (type-1)*25 + row*5 + slot + 1.
                    int idx = (int)((view->field_00 - 1) * 25 + rowSlotBase + s) + 1;
                    union { u32 w; u8 b[4]; } colv;   // byte round-trip (retail stw/lbz)
                    colv.w = getBdatStringColumnValue(table, &strBase[0x26D], idx);
                    int cost = (int)*(const u8*)&colv * 100;
                    if (cost <= cap) {
                        func_802617B8(reinterpret_cast<u8*>(view) + 0x3534,
                                      view->field_3DD0, -cost);
                        func_8025F528(reinterpret_cast<u8*>(view) + 0x3534,
                                      row, s + 1, (u32)idx);
                        func_800A2AF0(selfV);
                        cap = *(const int*)(reinterpret_cast<const u8*>(view) +
                                           view->field_3DD0 * 4 + 0x3DBC);
                        learned = 1;
                    }
                    break;
                }
            } else if (*(const u16*)(rec + 0x24) == 0) {
                func_8025F2E8(reinterpret_cast<u8*>(view) + 0x3534, g, s + 1);
            }
            s++;
            rec += 0x20;
        } while (s < 5);
        g++;
        grpCursor += 0xC4;
    } while (g < 11);
    if (*(const u16*)(reinterpret_cast<const u8*>(view) +
                      view->field_3DD0 * 0xC4 + 0x3538) != 0 || learned != 0) {
        func_8025EE94(reinterpret_cast<u8*>(view) + 0x3534);
    }
    if (view->field_00 == 3) {
        u8* p = reinterpret_cast<u8*>(lbl_eu_80663E88) + 0x265C4;
        func_eu_80263A24(p, reinterpret_cast<u8*>(view) + 0x3534);
        *reinterpret_cast<u32*>(p + 0x6E30) = 8;
    }
    if (view->field_00 == 8) {
        u8* p = reinterpret_cast<u8*>(lbl_eu_80663E88) + 0x130A0;
        func_eu_80263A24(p, reinterpret_cast<u8*>(view) + 0x3534);
        *reinterpret_cast<u32*>(p + 0x390C) = 3;
    }
    func_80280F44();
    return learned;
}

// ── func_800A2DE8 (us-800a36b0) ───────────────────────────────────────────
// Arts-slot write-in: read the source row's 12-bit arts type (top bits of
// the u16 at +4) plus seven bdat columns (ids/levels/flags), scan the source
// row's 2x4 slots for a matching id (honoring the two byte flags v5/v6),
// fall back to the first empty slot of the destination table, then stamp the
// slot: id, byte flags, level, a level bump from the row header, the two
// flag bits, and a clamped level field (+8 or +0xA depending on v4).
extern "C" void func_800A2DE8(cf::CtrlObjectParamArtsTable8* self,
                              cf::CtrlObjectParamArtsSrcRow* srcRow, int value) {
    // Declared in the retail claim order (saved high->low): v2->r31, v4->r30,
    // strBase->r26, type->r25, bdat->r24, v1->r23, v5->r22; v3 stays on the
    // stack and v6 reuses bdat's register.
    u8 v2;
    u8 v4;
    const char* strBase;
    u32 type;
    void* bdat;
    u16 v1;
    u8 v5;
    u8 v6;
    u32 v3;
    u16 rowHead = srcRow->field_04;
    type = (rowHead >> 4) & 0xFFF;
    if (type == 0) return;
    strBase = lbl_eu_804FBCB0;
    bdat = reinterpret_cast<void*>(lbl_eu_806640D8);
    v1 = (u16)getBdatStringColumnValue(bdat, &strBase[0x276], type);
    v2 = (u8)getBdatStringColumnValue(bdat, &strBase[0x281], type);
    v3 = getBdatStringColumnValue(bdat, &strBase[0x28A], type);
    v4 = (u8)getBdatStringColumnValue(bdat, &strBase[0x28E], type);
    v5 = (u8)getBdatStringColumnValue(bdat, &strBase[0x297], type);
    v6 = (u8)getBdatStringColumnValue(bdat, &strBase[0x29D], type);
    getBdatStringColumnValue(bdat, &strBase[0x86], type);
    if (v2 == 0) return;

    // 2-row scan: a slot matches when its id equals v1 and (v6 == 0 or
    // value equals the slot's flag bit while v5 != 0).
    u32 slotIdx = 0;
    int found = 0;
    u8* rowp = reinterpret_cast<u8*>(srcRow);
    for (int r = 0; r < 2 && !found; r++) {
        cf::CtrlObjectParamArtsSrcRow* row = reinterpret_cast<cf::CtrlObjectParamArtsSrcRow*>(rowp);
        if (row->field_04 == v1 &&
            (v6 == 0 || (value == ((row->field_0E >> 15) & 1) && v5 != 0))) {
            found = 1;
        }
        if (!found) {
            slotIdx++;
            if (row->field_10 == v1 &&
                (v6 == 0 || (value == ((row->field_1E >> 15) & 1) && v5 != 0))) {
                found = 1;
            }
        }
        if (!found) {
            slotIdx++;
            if (row->field_20 == v1 &&
                (v6 == 0 || (value == ((row->field_2E >> 15) & 1) && v5 != 0))) {
                found = 1;
            }
        }
        if (!found) {
            slotIdx++;
            if (row->field_30 == v1 &&
                (v6 == 0 || (value == ((row->field_3E >> 15) & 1) && v5 != 0))) {
                found = 1;
            }
        }
        if (!found) {
            slotIdx++;
            rowp += 0x40;
        }
    }
    if (!found) {
        // First empty destination slot (u16 id == 0), else 8.
        slotIdx = 0;
        if (self->slots[0].field_00 != 0) slotIdx = 1;
        if (self->slots[1].field_00 != 0) slotIdx = 2;
        if (self->slots[2].field_00 != 0) slotIdx = 3;
        if (self->slots[3].field_00 != 0) slotIdx = 4;
        if (self->slots[4].field_00 != 0) slotIdx = 5;
        if (self->slots[5].field_00 != 0) slotIdx = 6;
        if (self->slots[6].field_00 != 0) slotIdx = 7;
        if (self->slots[7].field_00 != 0) slotIdx = 8;
    }

    cf::CtrlObjectParamArtsSlotItem* slot = &self->slots[slotIdx];
    slot->field_00 = (u16)type;
    slot->field_02 = v2;
    slot->field_04 = v1;
    slot->field_0E = (u16)((slot->field_0E & ~0xC000u) | ((value & 1) << 15) |
                           ((srcRow->field_04 & 1) << 14));
    slot->field_0C = (u16)v3;
    slot->field_06 = (u16)(slot->field_06 + (srcRow->field_00 & 0x7F));
    s32 field = (s16)((srcRow->field_00 >> 10) & 0x7FF);
    if (v4 == 0) {
        s32 sum = (s32)slot->field_08 + field;
        slot->field_08 = (u16)(sum > (s32)v3 ? sum : (s32)v3);
    } else {
        s32 sum = (s32)slot->field_0A + field;
        slot->field_0A = (u16)(sum > (s32)v3 ? sum : (s32)v3);
    }
}

extern "C" __declspec(noinline) void func_800A30E4(cf::CtrlObjectParamActorOwner* selfV) {
    // Re-collect the arts rows: clear the stack arts table, fold in every row
    // of the +0x26 arts item (flag 1), then one row per equip slot
    // (+0x1C..+0x24, flag 0), and hand the table to the +0x184 sub-object's
    // slot-0x6C hook (and the actor's, when one exists).
    cf::CtrlObjectParamTypeView* self = reinterpret_cast<cf::CtrlObjectParamTypeView*>(selfV);
    cf::CtrlObjectParamEquipRow* row = reinterpret_cast<cf::CtrlObjectParamEquipRow*>(selfV);
    cf::CtrlObjectParamArtsTable8 table;
    cf::CtrlObjectParamArtsSlotItem* tEnd = &table.slots[8];
    cf::CtrlObjectParamArtsSlotItem* t = &table.slots[0];
    do {
        memset(t, 0, 0x10);
        ++t;
    } while (t < tEnd);
    memset(&table, 0, sizeof(table));
    memset(&table, 0, sizeof(table));
    void* item = 0;
    if (self->shortArr[5] > -1) {
        item = func_80157C4C(2, row->shortArr[5]);
    }
    if (item != 0) {
        int i = 0;
        while (i < (u16)(reinterpret_cast<cf::CtrlObjectParamVt02CIf*>(
                   CItem_initItemImplInstances(item))->_v02C(item))) {
            void* srcRow = reinterpret_cast<cf::CtrlObjectParamVt02CIf*>(
                CItem_initItemImplInstances(item))->_v028(item, i);
            if (srcRow != 0) {
                func_800A2DE8(&table,
                              reinterpret_cast<cf::CtrlObjectParamArtsSrcRow*>(srcRow), 1);
            }
            ++i;
        }
    }
    for (int i = 0; i < 5; ++i) {
        void* item2 = 0;
        if (row->shortArr[i] > -1) {
            item2 = func_80157C4C(i > 4 ? 2 : i + 4, row->shortArr[i]);
        }
        if (item2 != 0) {
            void* srcRow = reinterpret_cast<cf::CtrlObjectParamVt02CIf*>(
                CItem_initItemImplInstances(item2))->_v028(item2, 0);
            if (srcRow != 0) {
                func_800A2DE8(&table,
                              reinterpret_cast<cf::CtrlObjectParamArtsSrcRow*>(srcRow), 0);
            }
        }
    }
    reinterpret_cast<cf::CtrlObjectParamVt06CIf*>(
        reinterpret_cast<u8*>(&self->mParam) + 8)->_v064(&table);
    void* actor = func_800B8B94(self->field_00);
    if (actor != 0) {
        reinterpret_cast<cf::CtrlObjectParamVt06CIf*>(actor)->_v064(&table);
    }
}

u8 cf::CtrlObjectParamByteE4::getByteE4() {
    return field_E4;
}

extern "C" u8 func_800A32C4(cf::CtrlObjectParamBdatRow* self) {
    // bdat column lookup: item/weapon table (lbl_eu_806640F4), column name
    // at lbl_eu_804FBCB0+0x58, row key at +0xC of the object; the returned
    // column value is truncated to a byte via a memory round-trip (retail
    // stw r3,8(sp); lbz r3,8(sp)) — deref-of-storage, not a cast (the cast
    // folds to rlwinm).
    u32 v = getBdatStringColumnValue(reinterpret_cast<void*>(lbl_eu_806640F4),
                                     &lbl_eu_804FBCB0[0x58], self->field_0C);
    return *(const u8*)&v;
}

extern "C" void func_800A3304() {
    // Refresh every item instance in the equip rows for character rows
    // 1..13: resolve each valid slot entry and run the impl's 0x48 hook.
    int j;
    void* inst;
    cf::CtrlObjectParamEquipRow* r;
    for (u32 row = 1; row <= 13; ++row) {
        r = reinterpret_cast<cf::CtrlObjectParamEquipRow*>(
            reinterpret_cast<u8*>(lbl_eu_80663E88) + (u16)row * 0x3DD4 + 0x41F0);
        for (j = 0; j <= 5; ++j) {
            inst = 0;
            if (r->shortArr[j] > -1) {
                inst = func_80157C4C((u32)j > 4 ? 2 : j + 4, r->shortArr[j]);
            }
            if (inst != 0) {
                reinterpret_cast<cf::CtrlObjectParamItemImplIf*>(
                    CItem_initItemImplInstances(inst))->_v48(inst);
            }
        }
    }
}

extern "C" u32 func_800A33C8(void* arg) {
    // Look a character up in the party's equip slots: walk the 9-row slot
    // table; for each non-empty row, walk the 6 equip entries and compare
    // the resolved item instance against arg.
    u32 work = lbl_eu_80663E88;
    if (arg != 0) {
        u8* slots = reinterpret_cast<u8*>(work) + 0x1F9C;
        for (int i = 0; i < 9; ++i) {
            if (*(u32*)(slots + (i << 2)) != 0) {
                cf::CtrlObjectParamEquipRow* row =
                    reinterpret_cast<cf::CtrlObjectParamEquipRow*>(
                        reinterpret_cast<u8*>(lbl_eu_80663E88) +
                        (u16)*(u32*)(slots + (i << 2)) * 0x3DD4 + 0x41F0);
                for (int j = 0; j <= 5; ++j) {
                    void* inst = 0;
                    if (row->shortArr[j] > -1) {
                        inst = func_80157C4C((u32)j > 4 ? 2 : j + 4, row->shortArr[j]);
                    }
                    if (inst == arg) {
                        return *(u32*)(slots + (i << 2));
                    }
                }
            }
        }
    }
    return 0;
}

void cf::CObjectState::CObjectState_UnkVirtualFunc13() {
    unkC = unk8;
}

void* cf::CObjectState::CObjectState_UnkVirtualFunc12() {
    return &unkC;
}

void cf::CObjectState::CObjectState_UnkVirtualFunc6() {
    unk8 = 0;
}

void cf::CObjectState::CObjectState_UnkVirtualFunc4() {
    unk4 = 0;
}

int cf::CObjectParam::CObjectParam_UnkVirtualFunc3() {
    return reinterpret_cast<cf::CObjectParamRetailView*>(this)->field_30 != 0 ? 1 : 0;
}

void cf::CtrlObjectParamClear16::clear16Bytes() {
    words[0] = 0;
    words[1] = 0;
    words[2] = 0;
    words[3] = 0;
}

extern "C" void* __dt__800A34E0(void* self, int flag) {
    // Delete-only destructor: free self when the delete flag is positive.
    if (self != 0 && flag > 0) {
        ::operator delete(self);
    }
    return self;
}

extern "C" void func_800A3520(cf::CtrlObjectParamArtsSlot* self,
                              cf::CtrlObjectParamArtsListEntry* buf,
                              u32 value, u32 other) {
    // Init an arts list entry: record the header, zero the 0x20-byte entry,
    // then stamp the 0x1111 tag and the (count-1) sentinel.
    if (buf != 0) {
        self->buf = buf;
        self->field_0C = value;
        self->field_04 = value >> 5;
        self->field_08 = other;
        memset(buf, 0, 0x20);
        self->buf->field_00 = 0x1111;
        self->buf->field_04 = self->field_04 - 1;
    }
}

extern "C" void* func_800A3594(cf::CtrlObjectParamArtsList* list, u32 value) {
    // Find the 0x1111 row whose count is the smallest fit for the (value+31)/32
    // slot need; an exact match stops the walk early. On success, re-tag the
    // row as 0xAAAA and split the surplus slots off as a new 0x1111 node.
    // best declared first: retail colors it r31 (register-mapping contract).
    cf::CtrlObjectParamArtsListEntry* best = 0;
    u32 need = (value + 31) >> 5;
    cf::CtrlObjectParamArtsListEntry* e = list->head;
    u32 bestCount = list->field_04;
    while (e != 0) {
        u16 type = e->field_00;
        if (type == 0x1111) {
            u32 count = e->field_04;
            if (count >= need && count < bestCount) {
                best = e;
                bestCount = count;
                if (count == need) break;
            }
        } else if (type == 0xAAAA) {
            // pass-through row: skip
        } else {
            func_800A3A6C(list);
            break;
        }
        e = e->next;
    }
    if (best == 0) return 0;
    best->field_00 = 0xAAAA;
    memset(best->data, 0, 0xC);
    if (best->field_04 != need) {
        cf::CtrlObjectParamArtsNode* node = reinterpret_cast<cf::CtrlObjectParamArtsNode*>(
            reinterpret_cast<u8*>(best) + (need << 5));
        node->field_20 = 0x1111;
        node->field_24 = best->field_04 - need - 1;
        node->field_28 = reinterpret_cast<u32>(best);
        cf::CtrlObjectParamArtsListEntry* oldNext = best->next;
        node->field_2C = reinterpret_cast<u32>(oldNext);
        // Advance to the split node's entry (tag address); the same variable
        // is reassigned so MWCC overwrites the register (retail addi rX,rX,0x20).
        node = reinterpret_cast<cf::CtrlObjectParamArtsNode*>(
            reinterpret_cast<u8*>(node) + 0x20);
        if (oldNext != 0) {
            oldNext->prev = reinterpret_cast<u32>(node);
        }
        best->field_04 = need;
        best->next = reinterpret_cast<cf::CtrlObjectParamArtsListEntry*>(node);
    }
    return reinterpret_cast<u8*>(best) + 0x20;
}

extern "C" void* func_800A36A4(cf::CtrlObjectParamArtsList* list, u32 value) {
    // Arts-list allocator: find the 0x1111 row with count >= need, starting
    // from the TAIL of the list and walking backward through prev; the first
    // hit wins. An exact match re-tags the whole row 0xAAAA; a larger row has
    // its surplus tail split off as a new 0xAAAA node. Returns the data area
    // (+0x20) of the winning node, or 0 when no row fits.
    u32 need = (value + 31) >> 5;
    cf::CtrlObjectParamArtsListEntry* best = 0;
    cf::CtrlObjectParamArtsListEntry* e = list->head;
    while (e->next != 0) e = e->next;
    while (e != 0) {
        if (e->field_00 == 0x1111) {
            if (e->field_04 >= need) {
                best = e;
                break;
            }
        }
        e = reinterpret_cast<cf::CtrlObjectParamArtsListEntry*>(e->prev);
    }
    if (best == 0) return 0;
    u32 count = best->field_04;
    cf::CtrlObjectParamArtsListEntry* node = best;
    if (count == need) {
        best->field_00 = 0xAAAA;
        memset(&best->data[0], 0, 0xC);
    } else {
        // Split the surplus (count - need) slots off the tail as a new node.
        node = reinterpret_cast<cf::CtrlObjectParamArtsListEntry*>(
            reinterpret_cast<u8*>(best) + ((count - need) << 5));
        node->field_00 = 0xAAAA;
        memset(&node->data[0], 0, 0xC);
        node->field_04 = need;
        node->prev = reinterpret_cast<u32>(best);
        node->next = best->next;
        if (node->next != 0) {
            node->next->prev = reinterpret_cast<u32>(node);
        }
        best->field_04 = count - need - 1;
        best->next = node;
    }
    return reinterpret_cast<u8*>(node) + 0x20;
}

// ── func_800A37CC (us-800a4094) ───────────────────────────────────────────
// Arts-list merge: locate the 0xAAAA node whose data area (+0x20) is the
// caller's target pointer, validate the list through a second walk (unknown
// tags trigger the func_800A3A6C row-scan abort), then re-tag the node
// 0x1111 and fold its adjacent 0x1111 neighbours' counts into it.
extern "C" void func_800A37CC(cf::CtrlObjectParamArtsList* list, u8* target) {
    if (target == 0) return;
    int found = 0;
    cf::CtrlObjectParamArtsListEntry* e = list->head;
    while (e != 0) {
        int type = e->field_00;
        if (type == 0x1111) {
            // arts row: keep walking
        } else if (type == 0xAAAA) {
            u8* data = (u8*)e + 0x20;
            if (data == target) {
                found = 1;
                break;
            }
        } else {
            found = 0;
            break;
        }
        e = e->next;
    }
    if (!found) return;
    // Validation walk: advance through good rows; a bad tag after any good
    // row runs the arts-list abort handler, then stop.
    cf::CtrlObjectParamArtsListEntry* prev = 0;
    cf::CtrlObjectParamArtsListEntry* e2 = list->head;
    while (e2 != 0) {
        int type = e2->field_00;
        if (type == 0x1111 || type == 0xAAAA) {
            prev = e2;
            e2 = e2->next;
        } else {
            if (prev != 0) func_800A3A6C(list);
            break;
        }
    }
    // The found node is the entry whose data area is the target pointer.
    cf::CtrlObjectParamArtsListEntry* node =
        reinterpret_cast<cf::CtrlObjectParamArtsListEntry*>(target - 0x20);
    u32 tag = node->field_00;
    if (tag == 0x1111) return;
    if (tag != 0xAAAA) return;
    node->field_00 = 0x1111;
    memset(node->data, 0, 0xC);
    cf::CtrlObjectParamArtsListEntry* nxt = node->next;
    if (nxt != 0 && (u32)nxt->field_00 == 0x1111) {
        node->field_04 = nxt->field_04 + node->field_04 + 1;
        node->next = nxt->next;
        if (node->next != 0) {
            node->next->prev = reinterpret_cast<u32>(node);
        }
    }
    cf::CtrlObjectParamArtsListEntry* prv =
        reinterpret_cast<cf::CtrlObjectParamArtsListEntry*>(node->prev);
    if (prv != 0 && (u32)prv->field_00 == 0x1111) {
        prv->field_04 = node->field_04 + prv->field_04 + 1;
        prv->next = node->next;
        if (prv->next != 0) {
            prv->next->prev = reinterpret_cast<u32>(prv);
        }
    }
}

u32 func_800A3940(cf::CtrlObjectParamArtsList* list) {
    // Walk the arts list keeping the maximum (count << 5) across 0x1111 rows;
    // 0xAAAA rows are skipped, anything else aborts with 0.
    u32 max = 0;
    cf::CtrlObjectParamArtsListEntry* e = list->head;
    while (e != 0) {
        int type = e->field_00;
        switch (type) {
        case 0x1111: {
            u32 count = e->field_04 << 5;
            if (count > max) max = count;
            break;
        }
        case 0xAAAA:
            break;
        default:
            return 0;
        }
        e = e->next;
    }
    return max;
}

extern "C" int func_800A3998(cf::CtrlObjectParamArtsList* list) {
    // Walk the arts list; 0x1111 rows contribute (count << 5), 0xAAAA rows
    // are skipped, anything else aborts with 0.
    int total = 0;
    cf::CtrlObjectParamArtsListEntry* e = list->head;
    while (e != 0) {
        switch (e->field_00) {
        case 0x1111:
            total += (int)(e->field_04 << 5);
            break;
        case 0xAAAA:
            break;
        default:
            return 0;
        }
        e = e->next;
    }
    return total;
}

extern "C" int func_800A39E8(cf::CtrlObjectParamArtsList* list, u8* target,
                             u32 index, u32 value) {
    // Walk the arts list for an 0xAAAA row whose data area (+0x20) is the
    // target; write value into row->data[index] and re-verify the row tag.
    int found = 0;
    cf::CtrlObjectParamArtsListEntry* e = list->head;
    while (e != 0) {
        u16 type = e->field_00;
        u8* dataArea = (u8*)&e->data[4];
        switch (type) {
        case 0x1111:
            goto advance;
        case 0xAAAA:
            if (dataArea == target) {
                found = 1;
                goto done;
            }
            goto advance;
        default:
            found = 0;
            goto done;
        }
    advance:
        e = e->next;
    }
    found = 0;
done:
    if (found == 0) return 0;
    cf::CtrlObjectParamArtsListEntry* entry =
        reinterpret_cast<cf::CtrlObjectParamArtsListEntry*>(target - 0x20);
    if (entry->field_00 != 0xAAAA) goto ret0;
    entry->data[index] = value;
    return 1;
ret0:
    return 0;
}

// Walk the arts list: 0x1111 rows get their data[0] slot cleared, 0xAAAA rows
// sprintf their data[1] name into a local buffer; any other tag aborts the
// walk. Rows with a non-zero data[0] after handling get it formatted via
// func_800AA33C.
extern "C" void func_800A3A6C(cf::CtrlObjectParamArtsList* list) {
    cf::CtrlObjectParamArtsListEntry* e = list->head;
    while (e != 0) {
        switch (e->field_00) {
        case 0x1111:
            e->data[0] = 0;
            break;
        case 0xAAAA: {
            char sbuf[0x40];
            sprintf(sbuf, lbl_eu_804FBF58, e->data[1]);
            break;
        }
        default:
            goto done;
        }
        if (e->data[0] != 0) {
            ml::FixStr<64> str(true);
            func_800AA33C(str, e->data[0], 0, 0);
        }
        e = e->next;
    }
done:;
}

extern "C" void func_8009E0A8(void* a, void* c) { func_8009DBF4(a, 5, c); }
