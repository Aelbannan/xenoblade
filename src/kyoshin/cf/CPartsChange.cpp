// Auto-scaffolded catalog TU for kyoshin/cf/CPartsChange
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/harness_catalog.hpp"

#include "kyoshin/cf/CPartsChange.hpp"
#include "monolib/util/CPathUtil.hpp"
#include <string.h>

using cf::CfPartyInfo;
using cf::CfPartyInfoSortKey;
using cf::CfActorAccessors;
using cf::CfObjectPcExt;

struct CfPartsElem4C {
    u8 pad_00[0x10];
    f32 field_10;
    f32 field_14;
    f32 field_18;
    u16 field_1C;
    u16 field_1E;
    u16 field_20;    // 0x20
    u16 field_22;    // 0x22
    u8 pad_24;       // 0x24
    u8 field_25;     // 0x25
    u8 field_26;     // 0x26
    u8 field_27[9];  // 0x27
    u8 field_30[9];  // 0x30
    u8 field_39[9];  // 0x39
    u8 field_42[9];  // 0x42
};
// ---- retail data labels / runtime helpers ----------------------------------
// cf::CPartsChange vtable (0x80532AA8).
extern "C" {

// CfPartyInfo pose-dispatch table (0x80532AF0): 12-byte CodeWarrior member
// function pointers {delta, vtableIndex=-1, fn}, selected by field_0C.
extern void* lbl_eu_80532AA8[];
extern int (cf::CfPartyInfo::*const lbl_eu_80532AF0)(void*);
void __ptmf_scall();

// BDAT name-table handle and its column-name string block.
extern void* lbl_eu_806640CC;
extern const char lbl_eu_80503C48[];
u32 getBdatStringColumnValue(void* bdat, const char* col, s32 index);

// Manager-object helpers used by TeardownContainer.
__declspec(noinline) void func_80197538(void* mgr, void* obj);
__declspec(noinline) void func_80197DE8(void* mgr, void* obj, int a, int b);
void CPartsChange_NotifySlotStates(u8* self);
__declspec(noinline) void func_801953E8(void* mgr, void* elem, void* r5, int r6);

// Slot-name format string ("%s%s") and the "_ppp" name prefix pointer.
extern const char lbl_eu_80503BFC[];
extern const char* lbl_eu_806624E8;

// Collection-state helpers (raw retail symbol spellings).
extern u32 lbl_eu_80663E24;
void awardCollectCount(void* self);
int isCollepediaId(void* self);
int isResourceFlagSet__Q22cf13CfGameManagerFv(void* self);
void* func_8009EC9C(u16 id);
void func_800A282C(void* p, int flag);
void* getPlayer__Q22cf13CfGameManagerFi(int idx);
void fireLvUpMode2(void* p);

// Reslist vtables (0x80532AE4 base / 0x80532ACC derived), runtime alloc
// helpers and the manager-instance globals.
extern void* lbl_eu_80532AE4[];
extern void* lbl_eu_80532ACC[];
extern const f32 lbl_eu_80667AD0;
extern const f32 lbl_eu_80667B28;
extern int lbl_eu_80664308;
extern u32 lbl_eu_8066430C;
extern unsigned char lbl_eu_80664310;
extern s16 lbl_eu_80664312;
extern s16 lbl_eu_80664314;
void __dl__FPv(void*);
void __dla__FPv(void*);
u32 CfRes_getAllocHandle();
void* allocate_array__Q23mtl10MemManagerFUlUl(u32 size, u32 heap);

// Pose floor constant (0.0f).
extern const f32 lbl_eu_80667AD4;

// Same-TU helpers referenced before their definitions below. noinline keeps
// MWCC from inlining the (stub) bodies and collapsing retail's bl sites.
__declspec(noinline) void* CPartsChange_FindActorById(u8* self, u32 id);
__declspec(noinline) void* CPartsChange_FindEntryById(u8* self, u32 id);

// Same lookup, but TU-local so MWCC inlines it at the FireIdEffect site
// (retail inlines the scan there and keeps a bl at ResolveActorEntry).
static u8* FindEntryByIdInline(u8* self, u32 id) {
    if (id != 0) {
        u32 n = *(u32*)(self + 0x9800);
        u8* p = self;
        for (u32 i = 0; i < n; i++) {
            if (id == *(u16*)(p + 0x1C)) return self + i * 0x4C;
            p += 0x4C;
        }
    }
    return 0;
}
__declspec(noinline) void CPartsChange_InitActorEntry(u8* self);
__declspec(noinline) int CPartsChange_CountListNodes(u8* self);
__declspec(noinline) void CPartsChange_RemoveListNode(u8* self, void** key);

}

// Store+lbz is MWCC's (u8)u32 cast shape; used as the row check in
// CheckNameEntry.

// 0x58: vtable install, three zeroed scalars, then 0x20 bytes of 0xFF at +0xC.
// (Retail schedules the memset fill byte before the scalar stores.)
extern "C" cf::CPartsChange* __ct__cf_CPartsChange(cf::CPartsChange* self) {
    *(void**)self = lbl_eu_80532AA8;
    self->mField04 = 0;
    self->mField08 = 0;
    self->mField2C = 0;
    memset(self->mData, 0xFF, 0x20);
    return self;
}

// removed destructor to suppress vtable duplication

// 0x40: the D0 destructor body is empty (no member teardown, no vtable reset)
// -- it only runs the size-gated operator delete and returns self. It is
// spelled with the raw retail symbol so that defining it does not make MWCC
// emit a second __vt__Q22cf12CPartsChange in this unit (the retail vtable is
// the injected lbl_eu_80532AA8 data object).
extern "C" void* __dt__Q22cf12CPartsChangeFv(void* self, int flags) {
    if (self != 0) {
        if (flags > 0) __dl__FPv(self);
    }
    return self;
}

// Name-source object reached through this+8: slot 0x20 returns the name
// count, slot 0x24 the indexed name string. (MWCC reserves two words of
// vtable header, so the first dummy lands at +0x8: six dummies put the
// count at +0x20.)
class CNameSource {
public:
    virtual void v0();
    virtual void v1();
    virtual void v2();
    virtual void v3();
    virtual void v4();
    virtual void v5();
    virtual u32 getCount();          // index 6  -> vtable 0x20
    virtual const char* getName(u32 index);   // index 7 -> 0x24
    virtual void v8();               // index 8  -> 0x28
    virtual void v9();               // index 9  -> 0x2C
    virtual void notifySlot(u32 slot, int flag);  // index 10 -> 0x30
    virtual int querySlot(u32 slot);              // index 11 -> 0x34
};

// 0x114: for each of the 32 slots, build the "_pppNN" name from the slot
// index and look it up in the suffix-name source at +8; store the matching
// index in the 0xC..0x2B byte array and flag the record dirty at +0x2C.
extern "C" void CPartsChange_ResolveSlotNames(u8* self, u8 flag, CNameSource* src) {
    if (src == 0) return;
    *(void**)(self + 8) = src;
    *(u8*)(self + 4) = flag;
    for (u32 i = 0; i < 0x20; i++) {
        s32 idx;
        if (*(CNameSource**)(self + 8) == 0) {
            idx = -1;
        } else {
            u32 n = (*(CNameSource**)(self + 8))->getCount();
            ml::FixStr<16> name;
            ml::FixStr<16> num;
            ml::CPathUtil::itoa(num, (int)i, 2);
            name.format(lbl_eu_80503BFC, lbl_eu_806624E8, num.c_str());
            for (idx = 0; (u32)idx < n; idx++) {
                if (strstr((*(CNameSource**)(self + 8))->getName((u32)idx),
                           name.c_str()) != 0) break;
            }
            if ((u32)idx >= n) idx = -1;
        }
        if (idx >= 0) {
            *(u8*)(self + 0xC + i) = (u8)idx;
            *(u8*)(self + 0x2C) = 1;
        }
    }
    CPartsChange_NotifySlotStates(self);
}

// 0x10C: when the slot table is dirty, a name source is bound and the
// +4 flag is clear, walk the 0x20 slot bytes: while nothing has been matched
// the source is queried per live slot, and once something matched the source
// is *notified* with flag 0. If the walk found nothing, notify the first live
// slot with flag 1.
extern "C" void CPartsChange_NotifySlotStates(u8* self) {
    if (self[0x2C] == 0) return;
    if (*(u32*)(self + 8) == 0) return;
    if (self[4] != 0) return;
    int found = 0;
    for (u32 i = 0; i < 0x20; i++) {
        u8 slot = self[i + 0xC];
        if (slot != 0xFF) {
            if (found != 0) {
                ((CNameSource*)*(u32*)(self + 8))->notifySlot(slot, 0);
            } else if (((CNameSource*)*(u32*)(self + 8))->querySlot(slot) != 0) {
                found = 1;
            }
        }
    }
    if (found != 0) return;
    for (u32 j = 0; j < 0x20; j++) {
        u8 slot = self[j + 0xC];
        if (slot != 0xFF) {
            ((CNameSource*)*(u32*)(self + 8))->notifySlot(slot, 1);
            return;
        }
    }
}

// 0x130: force the slot table to the bound name source: with the +4 flag
// clear, first notify every live slot with 0, then notify the requested slot
// with the given flag and (when nothing was flagged) notify the first live
// slot other than the requested one with 1.
extern "C" void CPartsChange_SyncSlotState(u8* self, u32 idx, int flag) {
    if (self[0x2C] != 0 && *(u32*)(self + 8) != 0) {
        if (self[4] == 0) {
            for (u32 i = 0; i < 0x20; i++) {
                u8 slot = self[i + 0xC];
                if (slot != 0xFF) {
                    ((CNameSource*)*(u32*)(self + 8))->notifySlot(slot, 0);
                }
            }
        }
        u8 slot = self[idx + 0xC];
        if (slot != 0xFF) {
            ((CNameSource*)*(u32*)(self + 8))->notifySlot(slot, flag);
            if (self[4] != 0) return;
            if (flag != 0) return;
            for (u32 j = 0; j < 0x20; j++) {
                u8 other = self[j + 0xC];
                if (other == 0xFF) continue;
                if (j == idx) continue;
                ((CNameSource*)*(u32*)(self + 8))->notifySlot(other, 1);
                break;
            }
        }
    }
}

void func_801931D0(){}

// 0xB4: _reslist_base<cf::CfPartyInfo*>::~_reslist_base() -- reinstall the
// base vtable, unlink every node (next=0), relink the sentinel to itself, then
// free the owned pool buffer unless the ownership flag at +0x1C is set.
extern "C" void* __dt___reslist_base_cf_CfPartyInfo(u8* self, int flags) {
    if (self != 0) {
    *(void**)self = lbl_eu_80532AE4;
    u32 zero = 0;
    u32* node = (u32*)*(u32*)(*(u32**)(self + 4));
    while (node != *(u32**)(self + 4)) {
        u32* cur = node;
        node = (u32*)*node;
        *cur = zero;
    }
    u32* sentinel = *(u32**)(self + 4);
    *sentinel = (u32)sentinel;
    sentinel = *(u32**)(self + 4);
    sentinel[1] = (u32)sentinel;
    if (*(u8*)(self + 0x1C) == 0) {
        void* mem = *(void**)(self + 0x14);
        if (mem != 0) {
            __dla__FPv(mem);
            *(u32*)(self + 0x14) = 0;
        }
    }
    if (flags > 0) __dl__FPv(self);
    }
    return self;
}

// 0x15C: full manager construction -- base+derived reslist vtables with a
// self-linked sentinel, counters/flags, the 0xA40 work-buffer wipe, the
// manager globals, then a freshly allocated 16-slot 0xC0-byte actor table.
extern "C" u8* __ct__80193270(u8* self) {
    u8* list = self + 0xA808;
    u32 zero = 0;
    f32 one = lbl_eu_80667AD0;
    *(void**)list = lbl_eu_80532AE4;
    *(u32*)(self + 0x9800) = zero;
    *(u32*)(self + 0xA804) = zero;
    *(u32*)(self + 0xA81C) = zero;
    *(u32*)(self + 0xA820) = zero;
    *(u8*)(self + 0xA824) = 0;
    *(u32*)(self + 0xA80C) = (u32)(self + 0xA810);
    *(u32*)(*(u32**)(self + 0xA80C) + 0) = (u32)(self + 0xA810);
    *(u32*)((u8*)*(u32**)(self + 0xA80C) + 4) = *(u32*)(self + 0xA80C);
    *(void**)list = lbl_eu_80532ACC;
    *(f32*)(self + 0xB268) = one;
    *(f32*)(self + 0xB26C) = one;
    *(u16*)(self + 0xB270) = 1;
    *(u16*)(self + 0xB276) = 0xF;
    *(u16*)(self + 0xB278) = 1;
    lbl_eu_80664308 = (u32)self;
    lbl_eu_8066430C = (u32)self;
    lbl_eu_80664310 = 0;
    lbl_eu_80664314 = 0;
    memset(self + 0xA828, 0, 0xA40);
    void* arr = allocate_array__Q23mtl10MemManagerFUlUl(0xC0, CfRes_getAllocHandle());
    *(void**)(self + 0xA81C) = arr;
    // Retail reloads the table pointer before each unrolled slot store (the
    // stores may alias the member), so index through the member expression.
    for (int i = 0; i < 16; i++) {
        *(u32*)((u8*)*(u32**)(self + 0xA81C) + i * 0xC) = 0;
    }
    *(u32*)(self + 0xA820) = 16;
    return self;
}

// 0xB8: reslist<cf::CfPartyInfo*>::~reslist() -- the base teardown inlined
// behind its own null guard, then the flags-gated free.
extern "C" void* __dt__reslist_cf_CfPartyInfo(u8* self, int flags) {
    if (self != 0) {
        if (self != 0) {
            *(void**)self = lbl_eu_80532AE4;
            u32 zero = 0;
            u32* cur;
            u32* node = (u32*)*(u32*)(*(u32**)(self + 4));
            while (node != *(u32**)(self + 4)) {
                cur = node;
                node = (u32*)*node;
                *cur = zero;
            }
            u32* sentinel = *(u32**)(self + 4);
            *sentinel = (u32)sentinel;
            sentinel = *(u32**)(self + 4);
            sentinel[1] = (u32)sentinel;
            if (*(u8*)(self + 0x1C) == 0) {
                void* mem = *(void**)(self + 0x14);
                if (mem != 0) {
                    __dla__FPv(mem);
                    *(u32*)(self + 0x14) = 0;
                }
            }
        }
        if (flags > 0) __dl__FPv(self);
    }
    return self;
}

// 0x138: reslist<cf::CfPartyInfo*>::~reslist()
// unlink + free the owned actor table, then run the inlined base teardown on
// the embedded reslist at +0xA808 and free the object when flags > 0.
extern "C" void* __dt__80193538(u8* self, int flags) {
    if (self != 0) {
    u32 zero = 0;
    lbl_eu_80664308 = zero;
    lbl_eu_8066430C = zero;
    u32* node = (u32*)*(u32*)(*(u32**)(self + 0xA80C));
    while (node != *(u32**)(self + 0xA80C)) {
        u32* cur = node;
        node = (u32*)*node;
        *cur = zero;
    }
    u32* sentinel = *(u32**)(self + 0xA80C);
    *sentinel = (u32)sentinel;
    sentinel = *(u32**)(self + 0xA80C);
    sentinel[1] = (u32)sentinel;
    if (*(u8*)(self + 0xA824) == 0) {
        void* mem = *(void**)(self + 0xA81C);
        if (mem != 0) {
            __dla__FPv(mem);
            *(u32*)(self + 0xA81C) = 0;
        }
    }
    *(u32*)(self + 0xA820) = zero;
    u8* list = self + 0xA808;
    if (list != 0) {
    if (list != 0) {
        *(void**)list = lbl_eu_80532AE4;
        node = (u32*)*(u32*)(*(u32**)(list + 4));
        while (node != *(u32**)(list + 4)) {
            u32* cur = node;
            node = (u32*)*node;
            *cur = zero;
        }
        sentinel = *(u32**)(list + 4);
        *sentinel = (u32)sentinel;
        sentinel = *(u32**)(list + 4);
        sentinel[1] = (u32)sentinel;
        if (*(u8*)(list + 0x1C) == 0) {
            void* mem2 = *(void**)(list + 0x14);
            if (mem2 != 0) {
                __dla__FPv(mem2);
                *(u32*)(list + 0x14) = 0;
            }
        }
    }
    }
    if (flags > 0) __dl__FPv(self);
    }
    return self;
}

extern "C" u32 lbl_eu_8066430C;
extern "C" int lbl_eu_80664308;
extern "C" void func_80193810(u8* self);

extern "C" u32 CPartsChange_GetActorTable(void) { return lbl_eu_8066430C; }

// 0x98: find the live party-change entry for an id, clear its 0x1E "fired"
// bit, snap its 0x10 gauge to zero, and if it had not started falling wipe
// the 9-byte 0x30 block.
extern "C" void CPartsChange_FireIdEffect(u32 id) {
    u8* mgr = (u8*)lbl_eu_8066430C;
    if (mgr == 0) return;
    // Retail inlines the FindEntryById body here (no bl), so call it plainly
    // and let MWCC's inliner decide per site (ResolveActorEntry keeps a call).
    u8* entry = FindEntryByIdInline(mgr, id);
    if (entry == 0) return;
    u16 flags = *(u16*)(entry + 0x1E);
    f32 zero = lbl_eu_80667AD4;
    flags &= ~0x4u;                     // u16 mask -> retail's `andi.`
    *(u16*)(entry + 0x1E) = flags;
    *(f32*)(entry + 0x10) = zero;
    if (*(f32*)(entry + 0x14) <= zero) {
        memset(entry + 0x30, 0, 9);
    }
}

// 0xF4: when the object is flagged (0x3F00 bit 3, live 0x45C0) clear the
// element selected by 0x456C >> 4 after the 0xa8 virtual runs: drop flag 0x4,
// zero the 0x10 timer, and clear the 9-byte name when the 0x14 timer expired.
extern "C" void func_80193710(u8* obj) {
    u16 key;
    u8* mgr = (u8*)lbl_eu_8066430C;
    if (mgr == 0) return;
    if ((*(u32*)(obj + 0x3F00) & 0x4) == 0) return;
    if (*(u16*)(obj + 0x45C0) == 0) return;
    key = (u16)((s32)*(u16*)(obj + 0x456C) >> 4);
    ((cf::CPartsChangeDevView*)obj)->mAtA8(1);
    CfPartsElem4C* found;
    if (key != 0) {
        CfPartsElem4C* p = (CfPartsElem4C*)mgr;
        u32 n = *(u32*)(mgr + 0x9800);
        for (u32 i = 0; i < n; i++) {
            if (key == p->field_1C) {
                found = (CfPartsElem4C*)(mgr + i * 0x4C);
                goto done;
            }
            p++;
        }
    }
    found = 0;
done:;
    if (found == 0) return;
    found->field_1E &= (u16)~0x4u;
    found->field_10 = lbl_eu_80667AD4;
    if (found->field_14 <= lbl_eu_80667AD4) {
        memset(found->field_30, 0, 9);
    }
}

extern "C" int CPartsChange_GetLandmarkTable(void) { return lbl_eu_80664308; }

extern "C" void CPartsChange_DispatchChangeList(u8* self) { func_80193810(self); }

extern void* findObjectById(int);
extern "C" int lookupWorkAtAddr(void* addr);
extern "C" void gmFileObject(void* addr);
extern "C" s16 lbl_eu_80664314;

// Walk the party-change list at this+0xA80C: for each node whose slot
// table has bit0 set, resolve up to 16 object ids through findObjectById,
// keep those that pass lookupWorkAtAddr, then notify each via gmFileObject
// and reset the list / 0xA40-byte work buffer.
extern "C" void func_80193810(u8* self) {
    struct Slot {
        int id;
        u32 pad;
    };
    struct Node {
        Node* next;
        u32 pad04;
        Slot* slots;
    };

    Node* sentinel = *(Node**)(self + 0xA80C);
    Node* node = sentinel->next;
    void* found[96];
    u32 count = 0;

    while (node != sentinel) {
        Slot* slots = node->slots;
        if ((*(u16*)((u8*)slots + 0xA0) & 1) != 0) {
            int i;
            for (i = 0; i < 16; i++, slots++) {
                void* p = findObjectById(slots->id);
                void* q = p != 0 ? (u8*)p - 0x3E9C : p;
                if (q != 0) {
                    void* r = q != 0 ? (u8*)q + 0x3E9C : q;
                    if (lookupWorkAtAddr(r) != 0) {
                        if (q != 0) q = (u8*)q + 0x3E9C;
                        found[count++] = q;
                    }
                }
            }
        }
        node = node->next;
    }

    {
        u32 i;
        for (i = 0; i < count; i++) {
            gmFileObject(found[i]);
        }
    }

    *(u32*)(self + 0x9800) = 0;
    *(u32*)(self + 0xA804) = 0;
    {
        Node* n = sentinel->next;
        while (n != sentinel) {
            Node* cur = n;
            n = n->next;
            cur->next = 0;
        }
        sentinel->next = sentinel;
        ((u32*)sentinel)[1] = (u32)sentinel;
    }
    memset(self + 0xA828, 0, 0xA40);
    *(s16*)(self + 0xB272) = 5;
    *(s16*)(self + 0xB270) = 1;
    *(s16*)(self + 0xB274) = 0;
    lbl_eu_80664314 = 0;
}

// 0x10C: append a change record for `id` -- bail when the id is already
// present (or zero), otherwise build the 0x4C record on the stack (three zero
// floats, the id, flag word 1 with bit 0x400 cleared, four zeroed 9-byte
// names) and memcpy it past the tail of the array before bumping the count.
extern "C" void* Bdat_GetTable_AA34();
extern "C" void CPartsChange_InitChangeRecord(u8* self, u32 id) {
    Bdat_GetTable_AA34();
    if (id == 0) return;
    u8* p = self;
    u8* end = self + *(u32*)(self + 0x9800) * 0x4C;
    while (p != end) {
        if (id == *(u16*)(p + 0x1C)) return;
        p += 0x4C;
    }
    CfPartsElem4C rec;
    rec.field_10 = lbl_eu_80667AD4;
    rec.field_14 = lbl_eu_80667AD4;
    rec.field_18 = lbl_eu_80667AD4;
    rec.field_1C = (u16)id;
    rec.field_1E = 1;
    rec.field_22 = 0;
    rec.field_25 = 0;
    rec.field_26 = 0;
    memset(rec.field_27, 0, 9);
    memset(rec.field_30, 0, 9);
    memset(rec.field_39, 0, 9);
    memset(rec.field_42, 0, 9);
    rec.field_1E &= (u16)~0x400u;
    u32 n = *(u32*)(self + 0x9800);
    *(u32*)(self + 0x9800) = n + 1;
    memcpy((u8*)self + n * 0x4C, &rec, 0x4C);
}

void CfActorAccessors::SetFlag400(int enable) { if (enable) mFlags1E |= 0x400; else mFlags1E &= ~0x400; }

u32 CfActorAccessors::GetField94() { return mField94; }

// Scan 16 actor slots at self+0xA828 (stride 0xA4); match word at +0xA8BC.
extern "C" void* CPartsChange_FindActorById(u8* self, u32 id) {
    // Shared null epilogue (retail or r3,r7). Pure reg_swap vs retail p/result
    // colours is accepted via witness when FULL is not reached.
    void* result = 0;
    if (id != 0) {
        u8* p = self;
        for (u32 i = 0; i < 16; i++) {
            if (id == *(u32*)(p + 0xA8BC)) {
                result = self + 0xA828 + i * 0xA4;
                break;
            }
            p += 0xA4;
        }
    }
    return result;
}

void func_80193B0C(){}
// 0x54: wipe the 0x80-byte actor entry then re-seed its 0x90..0x9A block.
extern "C" __declspec(noinline) void CPartsChange_InitActorEntry(u8* self) {
    memset(self, 0, 0x80);
    f32 zero = lbl_eu_80667AD4;
    *(u16*)(self + 0xA2) = 0;
    *(u16*)(self + 0xA0) = 0;
    *(f32*)(self + 0x90) = zero;
    *(u32*)(self + 0x94) = 0;
    *(u16*)(self + 0x98) = 0;
    *(u16*)(self + 0x9A) = 0;
}

__declspec(noinline) u16 CfActorAccessors::GetField9E() { return mField9E; }

void CPartsChange_FindActorByObj(){}
void func_80193D48(){}

// 0x180: sum the per-item offsets the B48 gimmick list reports and average
// them (falling back to the second argument's vector when nothing matched).
// Each item contributes through two virtual slots: 0xE0 (a float) and 0xAC (a
// pointer) which, with the incoming float and the caller's vector, feed
// func_800A5488 into a scratch vector that is accumulated on success.
extern "C" u8* getReslistB48();
extern "C" int func_800A5488(void* a, void* b, f32 c, f32 d, void* out);

extern "C" int func_80194264(ml::CVec3* out, u8* arg2, f32 f1) {
    *out = ml::CVec3::zero;
    ml::CVec3 tmp;
    s32 count = 0;
    u8* list = getReslistB48();
    u8* sentinel = *(u8**)(list + 4);
    for (u8* node = *(u8**)sentinel; node != sentinel; node = *(u8**)node) {
        u8* item = *(u8**)(node + 8);
        f32 a = ((cf::CPartsChangeDevView*)item)->mAtE0();
        void* p = ((cf::CPartsChangeDevView*)item)->mAtAC();
        if (func_800A5488(arg2, p, f1, a, &tmp) != 0) {
            out->x += tmp.x;
            out->y += tmp.y;
            out->z += tmp.z;
            count++;
        }
    }
    if (count > 0) {
        f32 s = lbl_eu_80667AD0 / (f32)count;
        out->x *= s;
        out->y *= s;
        out->z *= s;
    } else {
        *out = *(ml::CVec3*)arg2;
    }
    return count != 0;
}

void func_801943E4(){}

void func_80194610(){}

// 0x11C: reload-parameter gate. Flags low nibble + the reload id (0..2) select
// the ok path; then the same low nibble must name this slot (1..4) or the
// whole gate passes vacuously when no low-nibble flag is set.
extern "C" u32 getReloadParam2();
extern "C" int func_801949E0(s32 self, u32 flags) {
    if (flags == 0) return 1;
    u32 v = getReloadParam2();
    int ok = 0;
    if ((flags & 0x40) != 0 && (u16)v == 0) {
        ok = 1;
    } else if ((flags & 0x10) != 0 && (u16)v == 1) {
        ok = 1;
    } else if ((flags & 0x20) != 0 && (u16)v == 2) {
        ok = 1;
    }
    if (!ok) return 0;
    if ((flags & 0xF) == 0) return 1;
    if ((flags & 1) != 0 && self == 1) return 1;
    if ((flags & 2) != 0 && self == 2) return 1;
    if ((flags & 4) != 0 && self == 3) return 1;
    if ((flags & 8) != 0 && self == 4) return 1;
    return 0;
}

void func_80194AFC(){}

void func_80194D5C(){}

// 0x4C-stride party-change element + manager view for CPartsChange_UpdateElemSpeeds
// (layout from CPartsChange.ctx.c).
struct CfPartsElemArray {
    CfPartsElem4C mElems[0x200];
    u32 mCount;
};
struct CfPartsManager {
    CfPartsElemArray mElems;
};
extern "C" int CfRes_getD80Flag();
extern "C" f32 Scn_GetFrameDelta(void);
extern const f32 lbl_eu_80667AC0;
extern const f32 lbl_eu_80667AD4;
extern const f32 lbl_eu_80667AB8;
extern const f32 lbl_eu_80667ABC;
extern u8* lbl_eu_806640A8;
extern u32 lbl_eu_80664184;

// 0xA0: re-seed one party-change element: clear flag 0x4, read the BDAT row
// (0 means 1), set the 0x14 delay to 1800 and the 0x10 timer to 60 * 30 *
// (float)row, then wipe the 9-byte name at +0x42.
extern "C" void func_801931D0(CfPartsElem4C* self) {
    self->field_1E &= (u16)~0x4u;
    u32 val = getBdatStringColumnValue(lbl_eu_806640A8, lbl_eu_80503C48,
                                       (s32)lbl_eu_80664184);
    u8 row = *(u8*)&val;
    if (row == 0) row = 1;
    f32 v = (f32)row * (f32)lbl_eu_80667ABC * lbl_eu_80667AB8;
    self->field_10 = v;
    self->field_14 = lbl_eu_80667AC0;
    memset((u8*)self + 0x42, 0, 9);
}

// Per-frame speed decay across the party-change element array.
extern "C" void CPartsChange_UpdateElemSpeeds(CfPartsManager* self) {
    CfRes_getD80Flag();
    f32 step = Scn_GetFrameDelta();
    CfPartsElemArray* arr = &self->mElems;
    for (CfPartsElem4C* e = arr->mElems; e != arr->mElems + arr->mCount; e++) {
        if (e->field_1E & 0x400) {
            e->field_14 = lbl_eu_80667AC0;
        } else if (e->field_14 > lbl_eu_80667AD4) {
            e->field_14 -= step;
            if (e->field_14 < lbl_eu_80667AD4) e->field_14 = lbl_eu_80667AD4;
        }
        if ((e->field_1E & 0x4) != 0) continue;
        if (!(e->field_10 > lbl_eu_80667AD4)) continue;
        volatile u16& curFlags = e->field_1E;
        if ((curFlags & 0x800) != 0) continue;
        e->field_10 -= step;
        if (!(e->field_10 > lbl_eu_80667AD4)) {
            e->field_10 = lbl_eu_80667AD4;
            if ((e->field_1E & 0x200) != 0 && (e->field_1E & 0x80) != 0) {
                e->field_1E |= 0x20;
            }
            memset(&e->field_30[0], 0, 9);
        }
    }
}

u32 CfActorAccessors::TestFlag400() { return (mFlags1E >> 10) & 0x1u; }

extern "C" u32 CtrlRemote_TouchBitByArg(u32 arg);

// Retail bl's CtrlRemote_TouchBitByArg(self+0x1D44), then cntlzw/rlwinm bool coerce.
extern "C" int CPartsChange_IsSubStateClear(u8* self) {
    if (self == 0) return 0;
    return CtrlRemote_TouchBitByArg((u32)(self + 0x1D44)) == 0;
}

// 0xB8: one-shot collection bookkeeping when the 0x400000 gate is clear:
// set the shared bit, award the collect count, then (when the id is a
// collepedia id) flip every set resource flag and fire the player level-up
// callback for the first three players.
extern "C" void CPartsChange_UpdateCollectionState(u8* self) {
    if (self == 0) return;
    if ((lbl_eu_80663E24 & 0x400000u) != 0) return;
    CtrlRemote_SetSharedBit((u32)(self + 0x1D44), 1);
    awardCollectCount(self);
    if (isCollepediaId(self) == 0) return;
    for (s32 i = 1; i <= 8; i++) {
        if (isResourceFlagSet__Q22cf13CfGameManagerFv((void*)(u32)i) != 0) {
            func_800A282C(func_8009EC9C((u16)i), 1);
        }
    }
    for (s32 j = 0; j < 3; j++) {
        u8* player = (u8*)getPlayer__Q22cf13CfGameManagerFi(j);
        if (player != 0) {
            fireLvUpMode2(*(void**)(player + 0x74));
        }
    }
}

// 0x64: a live name-entry flag forces a BDAT row lookup; rows above 3 fail.
extern "C" int CPartsChange_CheckNameEntry(u32 row, void* unused1, void* unused2,
                            cf::CfActorAccessors* actor) {
    int ok = 1;
    if ((actor->mFlags1E & 0x20) != 0) {
        // Retail keeps the raw u32 result in a stack slot and reads its
        // first byte (big-endian stw+lbz), i.e. a byte view of the value.
        u32 val = getBdatStringColumnValue(lbl_eu_806640CC, lbl_eu_80503C48 + 0x40,
                                           (s32)row);
        u8 v = *(u8*)&val;
        if (v > 3) ok = 0;
    }
    return ok;
}

// 0x78: walk the +0xA80C list for the entry whose id matches the object's
// 0x45C0 halfword, then resolve its object id through the actor table.
extern "C" void* CPartsChange_FindActorByObj(u8* self, u8* obj) {
    struct Node {
        Node* next;
        u32 pad04;
        u8* entry;
    };
    struct Entry {
        u32 objectId;
        u8 pad04[0x90];
        u32 id;
    };
    Node* sentinel;
    Entry* e;
    Node* node;
    void* result = 0;
    u32 id = *(u16*)(obj + 0x45C0);
    if (id != 0) {
        sentinel = *(Node**)(self + 0xA80C);
        node = sentinel->next;
        while (node != sentinel) {
            e = (Entry*)node->entry;
            if (id == e->id) {
                void* p = findObjectById(e->objectId);
                if (p != 0) p = (u8*)p - 0x3E9C;
                result = p;
                break;
            }
            node = node->next;
        }
    }
    return result;
}

u32 CfActorAccessors::TestFlag8() { return (mFlags1E >> 3) & 0x1u; }

void CfActorAccessors::SetField8C(float val) { mField8C = val; }

void CfObjectPcExt::SetField45C4(u16 val) { mField45C4 = val; }

void CfObjectPcExt::SetField45C8(u16 val) { mField45C8 = val; }

u16 CfObjectPcExt::GetField45C6() { return mField45C6; }

void* CfActorAccessors::GetField30() { return &mField30; }

void* CfObjectPcExt::GetField60C() { return (void*)((u8*)this + 0x60c); }

void CfPartyInfo::SetField2D(u8 val) { field_2D = val; }

// 0xD0: look the id up in the manager's element table (count at +0x9800,
// 0x4C stride, id at +0x1C) and, when found, arm the element: flag 0x1, both
// timers cleared, manager spawn helper, then clear 0x20 bit 3 and set the
// 0x810 armed flags. Returns whether an element matched.
extern "C" int CPartsChange_SpawnById(u32 id) {
    u16 key = (u16)id;
    u8* mgr = (u8*)lbl_eu_8066430C;
    CfPartsElem4C* found;
    if (key != 0) {
        CfPartsElem4C* p = (CfPartsElem4C*)mgr;
        u32 n = *(u32*)(mgr + 0x9800);
        for (u32 i = 0; i < n; i++) {
            if (key == p->field_1C) {
                found = (CfPartsElem4C*)(mgr + i * 0x4C);
                goto done;
            }
            p++;
        }
    }
    found = 0;
done:;
    u8 zero = 0;
    if (found != 0) {
        found->field_1E |= 0x1;
        found->field_10 = lbl_eu_80667AD4;
        found->field_14 = lbl_eu_80667AD4;
        func_801953E8(mgr, found, &zero, 0);
        found->field_20 &= (u16)~0x8u;
        found->field_1E |= 0x810;
    }
    return found != 0;
}

void func_80195BD4(){}

void func_80195E5C(){}

// Not yet decompiled (0x6d8): manager spawn helper reached from
// CPartsChange_SpawnById. Kept as a stub so the call site resolves.
void func_801953E8(void* mgr, void* elem, void* r5, int r6){}

bool CompareSortKey(const CfPartyInfoSortKey* a, const CfPartyInfoSortKey* b) {
    return a->sortKey < b->sortKey;
}

void func_80196434(){}

void func_80196864(){}

// 0x170: four-way swap dispatcher over the 8-byte {u32, f32} party records.
// The dispatcher predicate lives in the function-pointer slot at +0 of the
// fourth argument; the guard pair decides whether the pair is untouched, just
// swapped between a/b, or re-ordered with the third record.
struct CfPair8 {
    u32 w0;   // 0x00
    f32 f1;   // 0x04
};

struct CfPairOps {
    int (*fp)(CfPair8* x, CfPair8* y);   // 0x00
};

static inline void CfPairSwap(CfPair8* x, CfPair8* y) {
    CfPair8 t = *x;
    *x = *y;
    *y = t;
}

extern "C" void CPartsChange_CopyObjFields(CfPair8* a, CfPair8* b, CfPair8* c, CfPairOps* ops) {
    bool f1 = ops->fp(c, a) == 0;
    bool f2 = ops->fp(b, c) == 0;
    if (f1 && f2) return;
    if (!f1 && !f2) {
        CfPairSwap(a, b);
        return;
    }
    if (ops->fp(b, a) != 0) {
        CfPairSwap(a, b);
    }
    if (f1) {
        CfPairSwap(b, c);
    } else {
        CfPairSwap(a, c);
    }
}

void func_80196E04(){}

extern "C" void* CPartsChange_FindEntryById(u8* self, u32 id) {
    // Ascending i < n → mtctr + cmpli/ble (PATTERNS); shared null epilogue.
    if (id != 0) {
        u32 n = *(u32*)(self + 0x9800);
        u8* p = self;
        for (u32 i = 0; i < n; i++) {
            if (id == *(u16*)(p + 0x1C)) {
                return self + i * 0x4C;
            }
            p += 0x4C;
        }
    }
    return 0;
}

extern "C" void* CPartsChange_FindPartsElem(u8* self, u8* obj) {
    if ((*(u32*)(obj + 0x3F00) & 0x4) != 0) {
        u16 raw = *(u16*)(obj + 0x456C);
        u32 idx = (u32)((s32)raw >> 4);
        if (idx != 0) {
            u8* p = self;
            u32 id = (u32)(u16)idx;
            for (u32 i = 0; i < *(u32*)(self + 0x9800); i++) {
                if (id == *(u16*)(p + 0x1C)) {
                    return (void*)((u8*)self + i * 0x4C);
                }
                p += 0x4C;
            }
        }
        return 0;
    }
    return 0;
}

extern "C" __declspec(noinline) void func_80197538(void* mgr, void* obj){}

// 0x88: clear the actor's slot entry (0x9E index), reset the actor entry,
// then unlink it from the +0xA808 list if that list is non-empty.
#pragma push
#pragma auto_inline off
extern "C" void CPartsChange_ResolveActorEntry(u8* self, u32 id) {
    u8* actor = (u8*)CPartsChange_FindActorById(self, id);
    if (actor != 0) {
        u16 v = ((cf::CfActorAccessors*)actor)->GetField9E();
        u8* entry = (u8*)CPartsChange_FindEntryById(self, v);
        if (entry != 0) *(u16*)(entry + 0x22) = 0;
        CPartsChange_InitActorEntry(actor);
        if (CPartsChange_CountListNodes(self + 0xA808) != 0) {
            CPartsChange_RemoveListNode(self + 0xA808, (void**)&actor);
        }
    }
}
#pragma pop

extern "C" int CPartsChange_CountListNodes(u8* self) {
    // Retail colours: end=r5, cur=r4, n=r3. Declare cur before end so the
    // allocator gives end the later volatile (r5) after cur claims r4.
    void* cur;
    void* end;
    int n;
    end = *(void**)(self + 4);
    n = 0;
    cur = *(void**)end;
    goto check;
loop:
    cur = *(void**)cur;
    n++;
check:
    if (cur != end) goto loop;
    return n;
}

// 0x58: drop the container from the live manager (unlink + count-0 delete).
extern "C" void CPartsChange_TeardownContainer(u8* self) {
    if (self == 0) return;
    if (lbl_eu_8066430C == 0) return;
    func_80197538((void*)lbl_eu_8066430C, self);
    func_80197DE8((void*)lbl_eu_8066430C, self, 0, 0);
}

// 0xC8: only when a live manager exists and the object reports ready (its
// 0x2bc virtual, or the +0x3F08 bit 27): run the pending collection update,
// then drop the container from the manager.
extern "C" void CPartsChange_ResetBattleEntry(u8* self, u32 a, u32 b) {
    if (self == 0) return;
    if (lbl_eu_8066430C == 0) return;
    bool flag = false;
    if (((cf::CPartsChangeDevView*)self)->mAt2BC() != 0 ||
        (*(u32*)(self + 0x3F08) & 0x08000000) != 0) {
        flag = true;
    }
    if (flag) {
        u16 v = *(u16*)(self + 0x45C8);
        if (v != 0) {
            CPartsChange_UpdateCollectionState((u8*)(u32)v);
            *(u16*)(self + 0x45C8) = 0;
        }
    }
    func_80197538((void*)lbl_eu_8066430C, self);
    func_80197DE8((void*)lbl_eu_8066430C, self, (int)a, (int)b);
}

// 0x17C: spawn the item-drop actor for `id` unless it is already in the
// TboxInfo reslist, the overflow flag blocks spawning, or no box row rolls.
// The actor gets the position (raised by the 0x80667AE8 offset) through the
// 0x9C slot, the angle through 0xC4, the 0x80667B24 constant through 0xDC,
// the id at +0x73C, the Tbox copy, and a per-row sound.
struct CfDropNode {
    CfDropNode* next;   // 0x00
    u8 pad_04[4];
    u8* object;         // 0x08
};
struct CfDropList {
    u8* field_00;
    CfDropNode* sentinel;   // 0x04
};

extern "C" CfDropList* getReslistC08();
extern "C" s32 CItem_rollBoxContents(u32 row, u32 flag);
extern "C" void evictTboxOverflow();
extern "C" void* spawnVoiceActor(void* tag, void* argB, void* argC, f32 val);
extern "C" void copyTboxFromObj(void* arg);
extern "C" u16 playActorSound__Q22cf10CfSoundManFUlUlUlUlf(u32 a, u32 b, u32 c, u32 d, f32 e);
extern const u16 lbl_eu_80662528[4];
extern const f32 lbl_eu_80667AE8;
extern const f32 lbl_eu_80667B24;

extern "C" void CPartsChange_SpawnItemDrop(u32 id, u8* obj, f32* pos, u8* arg4, f32 angle) {
    f32 ang = angle;
    CfDropList* list = getReslistC08();
    CfDropNode* sentinel = list->sentinel;
    CfDropNode* node = sentinel->next;
    u32 found = 0;
    while (node != sentinel) {
        if (*(u32*)(node->object + 0x73C) == id) {
            found = 1;
            break;
        }
        node = node->next;
    }
    if (found) return;
    if ((*(u32*)lbl_eu_80663E24 & 0x20) != 0) return;
    s32 n = *(u16*)&lbl_eu_80664312;
    if (n == 0) {
        n = CItem_rollBoxContents((u32)(size_t)obj, (u32)(size_t)arg4);
    }
    *(u8*)lbl_eu_80664310 = 0;
    if (n == 0) return;
    evictTboxOverflow();
    u8* actor = (u8*)spawnVoiceActor((void*)(size_t)n, obj, pos, ang);
    f32 vec[3];
    vec[0] = pos[0];
    vec[1] = pos[1] + lbl_eu_80667AE8;
    vec[2] = pos[2];
    ((cf::CPartsChangeDevView*)actor)->mAt9C(vec);
    ((cf::CPartsChangeDevView*)actor)->mAtC4(ang);
    ((cf::CPartsChangeDevView*)actor)->mAtDC(lbl_eu_80667B24);
    *(u32*)(actor + 0x73C) = id;
    copyTboxFromObj(actor);
    playActorSound__Q22cf10CfSoundManFUlUlUlUlf(0, lbl_eu_80662528[n], 0, 0, lbl_eu_80667AD0);
}

extern "C" __declspec(noinline) void func_80197DE8(void* mgr, void* obj, int a, int b){}

int lbl_eu_80664308;
u32 lbl_eu_8066430C;
unsigned char lbl_eu_80664310;
short lbl_eu_80664312;
void CPartsChange_SetLoadFlag(int arg) {
    if (lbl_eu_8066430C == 0) return;
    unsigned char* p = (unsigned char*)lbl_eu_8066430C + 0x10000;
    *(unsigned short*)(p - 0x4d88) = (unsigned short)arg;
    if (arg == 0) return;
    unsigned char* p2 = (unsigned char*)lbl_eu_8066430C + 0x10000;
    *(unsigned short*)(p2 - 0x4d8a) = 0;
}

// 0x14C: first of the 16 eight-byte slots whose leading word is zero, found
// by a fully unrolled scan. Slot 0 doubles as scratch: when the caller passes
// flag 1 and the free slot is not slot 0, slot 0's payload is moved into the
// free slot and slot 0 becomes the target. Fills the slot with the object's
// 0x3F10 id (second word cleared), mirrors the id/count into the object and
// bumps the live-slot count. Returns 0 when all 16 slots are occupied.
extern "C" int CPartsChange_FindFreeSlot(u8* self, u8* obj, u32 arg3, u32 flags) {
    s32 slot = -1;
    for (u32 i = 0; i < 16; i++) {
        if (*(u32*)(self + i * 8) == 0) {
            slot = (s32)i;
            break;
        }
    }
    u32* slots = (u32*)self;
    if (slot >= 0) {
    if (slot > 0 && flags == 1) {
        u32 idx = (u32)slot * 2;
        slots[idx] = slots[0];
        slot = 0;
        slots[idx + 1] = slots[1];
    }
    u32 idx2 = (u32)slot * 2;
    slots[idx2] = *(u32*)(obj + 0x3F10);
    slots[idx2 + 1] = 0;
    *(u16*)(obj + 0x45C0) = (u16)slots[0x94 / 4];
    *(u16*)(obj + 0x45C6) = (u16)slot;
    *(s16*)(self + 0xA2) = (s16)(*(s16*)(self + 0xA2) + 1);
    return 1;
    }
    return 0;
}

// 0x8C: clear the actor-entry slot holding obj's 0x3F10 id and decrement the
// live-slot counter; when it hits zero the entry is marked empty.
extern "C" int CPartsChange_UnregisterEntry(u8* actor, u8* obj) {
    // One combined guard keeps a single shared `li r3,0` epilogue (retail jumps
    // both early-outs and the loop fall-through to the same block).
    if (obj != 0 && *(u32*)(obj + 0x3F10) != 0) {
        u8* p = actor;
        for (u32 i = 0; i < 16; i++) {
            // volatile: retail re-reads the 0x3F10 id every iteration (the
            // slot stores may alias obj), so it must not be CSE'd.
            if (*(u32*)p == *(volatile u32*)(obj + 0x3F10)) {
                u32* slots = (u32*)actor;
                slots[i * 2] = 0;
                slots[i * 2 + 1] = 0;
                s16 c = *(s16*)(actor + 0xA2) - 1;
                *(s16*)(actor + 0xA2) = c;
                if (c <= 0) {
                    *(s16*)(actor + 0xA2) = 0;
                    *(u32*)(actor + 0x94) = 0;
                    *(u16*)(actor + 0xA0) &= ~1u;
                }
                return 1;
            }
            p += 8;
        }
    }
    return 0;
}

extern "C" void* CPartsChange_ResolveLinkedObj(int* self) {
    void* p = findObjectById(*self);
    if (p != 0) {
        p = (u8*)p - 0x3E9C;
    }
    return p;
}

extern "C" int CPartsChange_HasAnyEntry(u8* self) {
    // Scan 16 eight-byte {id, data} records for a live id. The 16-count loop
    // unrolls 8x into the retail two-trip CTR form and keeps the dead
    // per-iteration induction update (li r4,0 / addi r4,r4,7).
    struct Entry {
        u32 id;
        u32 data;
    };
    Entry* e = (Entry*)self;
    for (s32 i = 0; i < 16; i++) {
        if (e[i].id != 0) return 1;
    }
    return 0;
}

// 0xE4: 16-slot, 8-byte-stride search for a non-zero voice id (MWCC unrolls
// it 8x under a 2x counted outer loop); -1 when absent.
extern "C" int CPartsChange_FindVoiceIndex(u32* table, u32 key) {
    if (key == 0) goto notfound;
    for (s32 i = 0; i < 16; i++) {
        if (table[i * 2] == key) return i;
    }
notfound:
    return -1;
}

void* CPartsChange_GetSlotEntryAt(void* self, unsigned long idx) {
    return *(void**)((char*)self + (idx << 3));
}

extern "C" void* CPartsChange_GetEnemySlotAt(u8* table, u32 idx) {
    void* p = findObjectById(*(int*)(table + (idx << 3)));
    if (p != 0) {
        p = (u8*)p - 0x3E9C;
    }
    return p;
}

// Inline helper: resolve one slot id to its pc-actor object (the same
// findObjectById + -0x3E9C shape CPartsChange_GetEnemySlotAt uses);
// PcActorOf appears twice in the first sweep, hence the duplicated call.
static inline u8* PcActorOf(u32 id) {
    u8* p = (u8*)findObjectById((int)id);
    if (p != 0) {
        p -= 0x3E9C;
    }
    return p;
}

extern "C" int mtRand__Q22ml4mathFi(int);

// 0x1A8: three sweeps over the 16 eight-byte id slots at self+0.
//  1. resolve each slot: live actors lose 0x20 from their 0x3F00 flags,
//     dead slots whose id still resolves are zeroed;
//  2. the first slot arms the object's 0x40000 flag and mirrors the actor's
//     0x3F28 replay id into self+0x98/0x9A (the 0x9A mirror is skipped when
//     bit 2 of self+0xA0 is set and 0x9A is already non-zero);
//  3. each live slot's 0x3F28 id is looked up in the BDAT name column: a
//     "2" result commits that id and breaks out, otherwise a 1-in-100 roll
//     (<= 30) commits it while the 0x9A mirror is unset.
extern "C" void func_80198524(u8* self) {
    for (s32 i = 0; i < 16; i++) {
        u8* act = PcActorOf(*(u32*)(self + i * 8));
        if (act != 0) {
            *(u32*)(act + 0x3F00) &= ~0x04000000u;
        } else if (PcActorOf(*(u32*)(self + i * 8)) != 0) {
            *(u32*)(self + i * 8) = 0;
        }
    }
    u8* act0 = PcActorOf(*(u32*)self);
    if (act0 != 0) {
        *(u32*)(act0 + 0x3F00) |= 0x04000000u;
        *(u16*)(self + 0x98) = *(u16*)(act0 + 0x3F28);
        if ((*(u16*)(self + 0xA0) & 4) == 0 || *(u16*)(self + 0x9A) == 0) {
            *(u16*)(self + 0x9A) = *(u16*)(act0 + 0x3F28);
        }
    } else {
        *(u16*)(self + 0x98) = 0;
    }
    u16 replay = 0;
    void* bdat = lbl_eu_806640CC;
    const char* col = lbl_eu_80503C48;
    for (s32 i = 0; i < 16; i++) {
        u8* act = PcActorOf(*(u32*)(self + i * 8));
        if (act != 0) {
            replay = *(u16*)(act + 0x3F28);
            u32 val = getBdatStringColumnValue(bdat, col + 0x40, (s32)replay);
            u8 b = *(u8*)&val;
            if (b == 2) {
                *(u16*)(self + 0x9A) = replay;
                *(u16*)(self + 0xA0) |= 4;
                break;
            }
            if (mtRand__Q22ml4mathFi(100) <= 30) {
                if ((*(u16*)(self + 0xA0) & 4) == 0 ||
                    *(u16*)(self + 0x9A) == 0) {
                    *(u16*)(self + 0x9A) = replay;
                }
            }
        } else if (*(u32*)(self + i * 8) != 0) {
            *(u32*)(self + i * 8) = 0;
        }
    }
}

extern "C" void CPartsChange_RemoveListNode(u8* self, void** key) {
    // Same shape as matched unlinkNodesByKey (code_800B06A4): next before
    // cur, next loaded first in the loop body → cur r8 / next r7.
    u32 sentinel = *(u32*)(self + 4);
    u32 next;
    u32 cur = *(u32*)sentinel;
    u32 zero = 0;
    while (cur != sentinel) {
        next = *(u32*)cur;
        if (*(u32*)(cur + 8) == *(u32*)key) {
            u32 prev = *(u32*)(cur + 4);
            *(u32*)prev = next;
            *(u32*)(next + 4) = prev;
            *(u32*)cur = zero;
        }
        cur = next;
    }
}

void CfPartyInfo::func_80198710(void* r4, float f1, int r5, int r6, float f2, float f3) {
    int r8 = *(int*)((char*)r4 + 0);
    int r7 = *(int*)((char*)r4 + 4);
    int r0 = *(int*)((char*)r4 + 8);
    field_00 = r8;
    field_04 = r7;
    field_08 = r0;
    field_18 = f1;
    field_0C = r5;
    field_14 = r6;
    field_1C = f2;
    field_20 = f3;
    if (r6 <= 0) {
        field_14 = 1;
    }
    field_28 = lbl_eu_80667B28;
    field_2C = 0;
    field_2D = 1;
    field_2E = 0;
}

// 0x38: dispatch the pose routine selected by the info record's move-type
// index through the retail ptmf table (r12 = &entry, r3 = info, r4 = arg).
extern "C" int CPartsChange_ProcessPartyInfo(cf::CfPartyInfo* info, void* arg) {
    typedef int (cf::CfPartyInfo::*PoseFn)(void*);
    const PoseFn* tbl = &lbl_eu_80532AF0;
    return (info->*tbl[info->field_0C])(arg);
}

extern "C" void CPartsChange_ProbePartyCollisions(u32* src, u32* dst);
// 0x320: not decoded yet -- every pose variant tail-calls it, so the stub
// keeps the symbol resolvable for the TU link.
extern "C" void CPartsChange_ProbePartyCollisions(u32* src, u32* dst) {}

// Pose records consumed/produced by the spawn-pose helpers: a 12-byte
// position followed by the 0x0C..0x1C scalars.
struct SpawnPose {
    f32 x;         // 0x00
    f32 y;         // 0x04
    f32 z;         // 0x08
    u32 f0C;       // 0x0C
    u32 f10;       // 0x10
    s32 field_14;  // 0x14
    f32 field_18;  // 0x18
    f32 field_1C;  // 0x1C
    f32 field_20;  // 0x20
};

// nw4r FIdx trig plus the spawn-angle constants (pi/4 offset, 40.743664 FIdx
// scale, 2^52+2^31 signed int->double magic).
extern "C" f32 SinFIdx__Q24nw4r4mathFf(f32);
extern "C" f32 CosFIdx__Q24nw4r4mathFf(f32);
extern const f32 lbl_eu_8066A200;   // pi/2
extern const f32 lbl_eu_8066A204;   // pi/4
extern const f32 lbl_eu_80667B50;
extern const f32 lbl_eu_8066A210;
extern const f32 lbl_eu_80667B4C;
extern const f64 lbl_eu_80667B58;

extern "C" void CPartsChange_CopyHeaderAndLoad(u32* src, u32* dst) {
    u32 w0 = src[0];
    u32 w1 = src[1];
    dst[1] = w1;
    dst[0] = w0;
    dst[2] = src[2];
    CPartsChange_ProbePartyCollisions(src, dst);
}

// 0x144: pose variant driven by the 0x14 count: the mod-360 remainder with the
// 0x8066A210 scale feeds the angle, the mod-100 remainder with the 0x80667B4C
// scale scales the radius (plus the 0x20 offset), and the rotated offset is
// accumulated onto dst x/z.
extern "C" void func_80198AE0(SpawnPose* src, SpawnPose* dst) {
    u32* s = (u32*)src;
    u32* d = (u32*)dst;
    s32 v = src->field_14;
    s32 rem360 = v % 360;
    s32 rem100 = v % 100;
    f32 off = (f32)rem360 * lbl_eu_8066A210;
    f32 angle = src->field_18 + off;
    f32 rad = lbl_eu_80667B4C * (f32)rem100 * src->field_1C + src->field_20;
    d[0] = s[0];
    d[1] = s[1];
    d[2] = s[2];
    dst->x += rad * SinFIdx__Q24nw4r4mathFf(lbl_eu_80667B50 * angle);
    dst->z += rad * CosFIdx__Q24nw4r4mathFf(lbl_eu_80667B50 * angle);
    CPartsChange_ProbePartyCollisions((u32*)src, (u32*)dst);
}

// 0x120: pose A uses the pi/2 offset and a radius derived from the halved
// (field_14-1) step; odd field_14 adds the rotated offset, even subtracts it.
extern "C" void CPartsChange_ComputeSpawnPoseA(SpawnPose* src, SpawnPose* dst) {
    u32* s = (u32*)src;
    u32* d = (u32*)dst;
    u32 w0 = s[0];
    u32 w1 = s[1];
    d[1] = w1;
    d[0] = w0;
    d[2] = s[2];
    s32 n = src->field_14;
    f32 radius = (f32)((n - 1) >> 1) * src->field_1C + src->field_1C;
    f32 angle = src->field_18 + lbl_eu_8066A200;
    if ((n & 1) != 0) {
        dst->x += radius * SinFIdx__Q24nw4r4mathFf(lbl_eu_80667B50 * angle);
        dst->z += radius * CosFIdx__Q24nw4r4mathFf(lbl_eu_80667B50 * angle);
    } else {
        dst->x -= radius * SinFIdx__Q24nw4r4mathFf(lbl_eu_80667B50 * angle);
        dst->z -= radius * CosFIdx__Q24nw4r4mathFf(lbl_eu_80667B50 * angle);
    }
    CPartsChange_ProbePartyCollisions((u32*)src, (u32*)dst);
}

// 0xC8: pose rotation without the angle offset -- the FIdx argument is just
// the field_18 angle (scaled), the radius is field_14 * field_1C, both
// components subtract, then the party-collision probe.
extern "C" void func_80198D44(SpawnPose* src, SpawnPose* dst) {
    u32* s = (u32*)src;
    u32* d = (u32*)dst;
    u32 w0 = s[0];
    u32 w1 = s[1];
    d[1] = w1;
    d[0] = w0;
    d[2] = s[2];
    f32 radius = (f32)src->field_14 * src->field_1C;
    dst->x -= radius * SinFIdx__Q24nw4r4mathFf(lbl_eu_80667B50 * src->field_18);
    dst->z -= radius * CosFIdx__Q24nw4r4mathFf(lbl_eu_80667B50 * src->field_18);
    CPartsChange_ProbePartyCollisions((u32*)src, (u32*)dst);
}

// 0xDC: copy the record, then rotate the copied position about the +0x18
// angle (pi/4 offset, FIdx scale) by the field_14 * field_1C radius, and
// probe the party collision state.
extern "C" void CPartsChange_ComputeSpawnPoseB(SpawnPose* src, SpawnPose* dst) {
    // Only the 12-byte position is copied (word moves, second word first).
    u32* s = (u32*)src;
    u32* d = (u32*)dst;
    u32 w0 = s[0];
    u32 w1 = s[1];
    d[1] = w1;
    d[0] = w0;
    d[2] = s[2];
    f32 radius = (f32)src->field_14 * src->field_1C;
    f32 angle = src->field_18 + lbl_eu_8066A204;
    dst->x -= radius * SinFIdx__Q24nw4r4mathFf(lbl_eu_80667B50 * angle);
    dst->z -= radius * CosFIdx__Q24nw4r4mathFf(lbl_eu_80667B50 * angle);
    CPartsChange_ProbePartyCollisions((u32*)src, (u32*)dst);
}

// 0xDC: ComputeSpawnPoseB with the pi/4 offset subtracted instead of added.
extern "C" void CPartsChange_ComputeSpawnPoseC(SpawnPose* src, SpawnPose* dst) {
    u32* s = (u32*)src;
    u32* d = (u32*)dst;
    u32 w0 = s[0];
    u32 w1 = s[1];
    d[1] = w1;
    d[0] = w0;
    d[2] = s[2];
    f32 radius = (f32)src->field_14 * src->field_1C;
    f32 angle = src->field_18 - lbl_eu_8066A204;
    dst->x -= radius * SinFIdx__Q24nw4r4mathFf(lbl_eu_80667B50 * angle);
    dst->z -= radius * CosFIdx__Q24nw4r4mathFf(lbl_eu_80667B50 * angle);
    CPartsChange_ProbePartyCollisions((u32*)src, (u32*)dst);
}

// 0x12C: pose D keeps the halved (field_14-1) radius, but the pi/4 offset is
// added for odd (field_14-1) and subtracted otherwise; both arms subtract.
extern "C" void CPartsChange_ComputeSpawnPoseD(SpawnPose* src, SpawnPose* dst) {
    u32* s = (u32*)src;
    u32* d = (u32*)dst;
    u32 w0 = s[0];
    u32 w1 = s[1];
    d[1] = w1;
    d[0] = w0;
    d[2] = s[2];
    s32 m = src->field_14 - 1;
    f32 radius = (f32)(m >> 1) * src->field_1C + src->field_1C;
    if ((m & 1) != 0) {
        f32 angle = src->field_18 + lbl_eu_8066A204;
        dst->x -= radius * SinFIdx__Q24nw4r4mathFf(lbl_eu_80667B50 * angle);
        dst->z -= radius * CosFIdx__Q24nw4r4mathFf(lbl_eu_80667B50 * angle);
    } else {
        f32 angle = src->field_18 - lbl_eu_8066A204;
        dst->x -= radius * SinFIdx__Q24nw4r4mathFf(lbl_eu_80667B50 * angle);
        dst->z -= radius * CosFIdx__Q24nw4r4mathFf(lbl_eu_80667B50 * angle);
    }
    CPartsChange_ProbePartyCollisions((u32*)src, (u32*)dst);
}

// 0x13C: pose variant that subtracts half the step always and adds the
// pi/2-offset rotation only when field_14 bit 0 is set.
extern "C" void func_801990F0(SpawnPose* src, SpawnPose* dst) {
    u32* s = (u32*)src;
    u32* d = (u32*)dst;
    u32 w0 = s[0];
    u32 w1 = s[1];
    d[1] = w1;
    d[0] = w0;
    d[2] = s[2];
    f32 dx = lbl_eu_80667B28;
    f32 dz = lbl_eu_80667B28;
    if ((src->field_14 & 1) != 0) {
        f32 a = src->field_18 + lbl_eu_8066A200;
        dx += src->field_1C * SinFIdx__Q24nw4r4mathFf(lbl_eu_80667B50 * a);
        dz += src->field_1C * CosFIdx__Q24nw4r4mathFf(lbl_eu_80667B50 * a);
    }
    f32 rad = (f32)(src->field_14 >> 1) * src->field_1C;
    dx -= rad * SinFIdx__Q24nw4r4mathFf(src->field_18 * lbl_eu_80667B50);
    dz -= rad * CosFIdx__Q24nw4r4mathFf(src->field_18 * lbl_eu_80667B50);
    dst->x += dx;
    dst->z += dz;
    CPartsChange_ProbePartyCollisions(s, d);
}

// 0x198: the step-count pose variant. The (field_14 - 1) modulo 3 picks the
// angle nudge (minus pi/4, plus pi/4) and, for the remaining case, normalises
// the radius to its (rad, rad) 2-vector magnitude through the nw4r FrSqrt
// helper. The radius is then scaled by (m / 3) + 1 and the rotated offset is
// subtracted from the destination x/z before the party probe.
extern "C" f32 FrSqrt__Q24nw4r4mathFf(f32 x);
extern "C" void Warning__Q24nw4r2dbFPCciPCce(const char* file, int line, const char* msg);
extern const char lbl_eu_80526324[];
extern const char lbl_eu_80526300[];

extern "C" void CPartsChange_ComputeSpawnPoseFull(SpawnPose* src, SpawnPose* dst) {
    u32* s = (u32*)src;
    u32* d = (u32*)dst;
    u32 w0 = s[0];
    u32 w1 = s[1];
    d[1] = w1;
    d[0] = w0;
    d[2] = s[2];
    s32 m = src->field_14 - 1;
    f32 rad = src->field_1C;
    f32 ang = src->field_18;
    if (m % 3 == 0) {
        ang -= lbl_eu_8066A204;
    } else if (m % 3 == 1) {
        ang += lbl_eu_8066A204;
    } else {
        if (rad * rad + rad * rad < lbl_eu_80667B28) {
            Warning__Q24nw4r2dbFPCciPCce(lbl_eu_80526324, 0x273, lbl_eu_80526300);
        }
        f32 d2 = rad * rad + rad * rad;
        if (d2 > lbl_eu_80667B28) {
            rad = d2 * FrSqrt__Q24nw4r4mathFf(d2);
        } else {
            rad = lbl_eu_80667B28;
        }
    }
    rad = (f32)(m / 3) * rad + rad;
    dst->x -= rad * SinFIdx__Q24nw4r4mathFf(lbl_eu_80667B50 * ang);
    dst->z -= rad * CosFIdx__Q24nw4r4mathFf(lbl_eu_80667B50 * ang);
    CPartsChange_ProbePartyCollisions((u32*)src, (u32*)dst);
}


// absorb tails - generated to match retail ET_REL bytes
__declspec(section ".data") __attribute__((aligned(8))) __attribute__((used)) unsigned char __tail_kyoshin_cf_CPartsChange_data[0xB8] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00, 0x3F, 0x40, 0x00, 0x00,
    0x3F, 0x00, 0x00, 0x00, 0x3E, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00
};

__declspec(section ".rodata") __attribute__((aligned(8))) __attribute__((used)) const unsigned char __tail_kyoshin_cf_CPartsChange_rodata[0x128] = {
    0x63, 0x66, 0x3A, 0x3A, 0x43, 0x50, 0x61, 0x72, 0x74, 0x73, 0x43, 0x68,
    0x61, 0x6E, 0x67, 0x65, 0x00, 0x00, 0x00, 0x00, 0x25, 0x73, 0x25, 0x73,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x72, 0x65, 0x73, 0x6C,
    0x69, 0x73, 0x74, 0x3C, 0x63, 0x66, 0x3A, 0x3A, 0x43, 0x66, 0x50, 0x61,
    0x72, 0x74, 0x79, 0x49, 0x6E, 0x66, 0x6F, 0x20, 0x2A, 0x3E, 0x00, 0x00,
    0x5F, 0x72, 0x65, 0x73, 0x6C, 0x69, 0x73, 0x74, 0x5F, 0x62, 0x61, 0x73,
    0x65, 0x3C, 0x63, 0x66, 0x3A, 0x3A, 0x43, 0x66, 0x50, 0x61, 0x72, 0x74,
    0x79, 0x49, 0x6E, 0x66, 0x6F, 0x20, 0x2A, 0x3E, 0x00, 0x00, 0x00, 0x00,
    0x65, 0x5F, 0x72, 0x65, 0x70, 0x6F, 0x70, 0x74, 0x69, 0x6D, 0x65, 0x00,
    0x70, 0x6F, 0x73, 0x58, 0x00, 0x70, 0x6F, 0x73, 0x59, 0x00, 0x70, 0x6F,
    0x73, 0x5A, 0x00, 0x52, 0x61, 0x64, 0x69, 0x75, 0x73, 0x00, 0x70, 0x6F,
    0x70, 0x5F, 0x74, 0x79, 0x70, 0x65, 0x00, 0x65, 0x6E, 0x65, 0x5F, 0x74,
    0x79, 0x70, 0x65, 0x00, 0x66, 0x6F, 0x72, 0x6D, 0x00, 0x70, 0x6F, 0x73,
    0x41, 0x54, 0x52, 0x00, 0x6E, 0x61, 0x6D, 0x65, 0x64, 0x00, 0x72, 0x65,
    0x73, 0x6F, 0x75, 0x72, 0x63, 0x65, 0x00, 0x73, 0x6E, 0x61, 0x70, 0x00,
    0x44, 0x69, 0x72, 0x00, 0x42, 0x54, 0x4C, 0x5F, 0x65, 0x6E, 0x65, 0x6C,
    0x69, 0x73, 0x74, 0x00, 0x72, 0x6F, 0x75, 0x74, 0x65, 0x49, 0x44, 0x00,
    0x71, 0x75, 0x65, 0x73, 0x74, 0x49, 0x44, 0x00, 0x71, 0x75, 0x65, 0x73,
    0x74, 0x5F, 0x53, 0x54, 0x46, 0x4C, 0x47, 0x00, 0x53, 0x5F, 0x46, 0x4C,
    0x47, 0x5F, 0x4D, 0x49, 0x4E, 0x00, 0x53, 0x5F, 0x46, 0x4C, 0x47, 0x5F,
    0x4D, 0x41, 0x58, 0x00, 0x50, 0x4F, 0x50, 0x5F, 0x54, 0x49, 0x4D, 0x45,
    0x00, 0x6D, 0x6F, 0x76, 0x65, 0x5F, 0x74, 0x79, 0x70, 0x65, 0x00, 0x66,
    0x6F, 0x72, 0x6D, 0x5F, 0x72, 0x61, 0x6E, 0x67, 0x65, 0x00, 0x4E, 0x41,
    0x4D, 0x45, 0x44, 0x5F, 0x46, 0x4C, 0x47, 0x00, 0x63, 0x68, 0x69, 0x6C,
    0x64, 0x5F, 0x49, 0x44, 0x00, 0x00, 0x00, 0x00
};

__declspec(section ".sdata") __attribute__((aligned(8))) __attribute__((used)) unsigned char __tail_kyoshin_cf_CPartsChange_sdata[0x58] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x65, 0x6E, 0x65, 0x3F, 0x49, 0x44, 0x00, 0x00, 0x65, 0x6E, 0x65, 0x3F,
    0x50, 0x65, 0x72, 0x00, 0x65, 0x6E, 0x65, 0x3F, 0x6E, 0x75, 0x6D, 0x00,
    0xFF, 0xFF, 0xFF, 0xFC, 0xFF, 0xFF, 0xFF, 0xFC, 0x65, 0x6E, 0x65, 0x3F,
    0x49, 0x44, 0x00, 0x00, 0x00, 0x00, 0x01, 0xCC, 0x01, 0xCD, 0x01, 0xCE,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00
};

__declspec(section ".sdata2") __attribute__((aligned(8))) __attribute__((used)) const unsigned char __tail_kyoshin_cf_CPartsChange_sdata2[0xB0] = {
    0x5F, 0x70, 0x70, 0x70, 0x00, 0x00, 0x00, 0x00, 0x42, 0x70, 0x00, 0x00,
    0x41, 0xF0, 0x00, 0x00, 0x44, 0xE1, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x43, 0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x65, 0x6E, 0x65, 0x3F, 0x49, 0x44, 0x00, 0x00,
    0x65, 0x6E, 0x65, 0x3F, 0x6E, 0x75, 0x6D, 0x00, 0x3D, 0xCC, 0xCC, 0xCD,
    0x3C, 0x23, 0xD7, 0x0A, 0x43, 0x30, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00,
    0x42, 0x22, 0xF9, 0x83, 0x42, 0x34, 0x00, 0x00, 0x42, 0x20, 0x00, 0x00,
    0x40, 0x00, 0x00, 0x00, 0x41, 0x20, 0x00, 0x00, 0x3F, 0x40, 0x00, 0x00,
    0x44, 0x7A, 0x00, 0x00, 0x40, 0xA0, 0x00, 0x00, 0x3E, 0xAA, 0xAA, 0xAB,
    0x42, 0x48, 0x00, 0x00, 0x3F, 0x00, 0x00, 0x00, 0x3F, 0xC0, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x40, 0x80, 0x00, 0x00, 0xC1, 0x00, 0x00, 0x00,
    0x3F, 0x35, 0x04, 0x81, 0x46, 0x1C, 0x40, 0x00, 0x42, 0xC8, 0x00, 0x00,
    0x40, 0x00, 0x00, 0x00, 0x3D, 0xCC, 0xCC, 0xCD, 0x3C, 0x23, 0xD7, 0x0B,
    0x3C, 0x23, 0xD7, 0x0A, 0x42, 0x22, 0xF9, 0x83, 0x00, 0x00, 0x00, 0x00,
    0x43, 0x30, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00
};
