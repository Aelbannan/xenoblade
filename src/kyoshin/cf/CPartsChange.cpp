// Auto-scaffolded catalog TU for kyoshin/cf/CPartsChange
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/harness_catalog.hpp"

#include "monolib/util/reslist.hpp"
#include "monolib/util/CPathUtil.hpp"
#include "kyoshin/code_802B8A3C.hpp"
#include "kyoshin/cf/CfResPcImpl.hpp"
#include "kyoshin/CUIWindowManager.hpp"
#include <nw4r/math/math_triangular.h>
#include "monolib/math/Random.hpp"

#include "kyoshin/cf/CPartsChange.hpp"

using cf::CfPartyInfo;
using cf::CfPartyInfoSortKey;
using cf::CfActorAccessors;
using cf::CfObjectPcExt;
using cf::CPartsChange;

// .sdata2 pool plant (ocUnit.cpp / CfMapEffectManager convention): the retail
// magic double at 0x80667B58 (2^52 + 2^31 = 4503601774854144.0) is referenced
// by MWCC's direct (f32)(s32) conversion idiom. The conversion's lfd reloc
// stays a TU-local @N pool entry (verified 2026: MWCC does not unify the
// builtin conversion magic with a same-value named global - see
// MWCC_CASES §1d/§1n, which classify the i2f magic pool as un-nameable
// in high-level C; the manual union form breaks the retail's early-magic
// schedule). The definition is kept for the retail-named .sdata2 slot.
f64 lbl_eu_80667B58 = 4503601774854144.0;

// u32 -> f32 conversion: builds the 0x43300000-prefixed bit pattern in
// memory and subtracts the shared sdata2 magic double (CfMapEffectManager.hpp
// convention) so the pool reloc names the retail constant instead of an
// MWCC-synthesised @N entry.
union F64Conv {
    u32 w[2];
    f64 d;
};

// Retail symbols for the same-TU helpers below are unmangled (the retail
// build compiled them with C linkage). Declaring them extern "C" here gives
// the definitions C linkage, so call relocs name the retail symbols instead
// of the mangled C++ forms (PLAN.md §17.6 reloc-name drift; the hexdiff-/
// harness-approved fix). func_80193CC8's real body stays the mangled member
// below; this free declaration exists only to emit the retail reloc name.
extern "C" void func_80192F94(CPartsChange* self);
extern "C" CfElemA4* func_80193AB0(CfTableA4* self, u32 id);
extern "C" u16 func_80193CC8(const CfElemA4* self);
extern "C" void func_80193C74(CfPartState90* self);
extern "C" CfPartsElem4C* func_8019747C(CfPartsElemArray* self, u32 id);
extern "C" u32 func_80197B28(const reslist<cf::CfPartyInfo>* self);
extern "C" void func_801986CC(reslist<cf::CfPartyInfo>* self, const CfPartyInfo* item);
extern "C" int func_801953E8(CfPartsElemArray* arr, CfPartsElem4C* elem, u8* flag, int arg4);

// Retail ctor symbol `__ct__cf_CPartsChange` is a splitter-renamed form that
// a real member ctor cannot reproduce (MWCC mangles member ctors as
// __ct__Q22cf12CPartsChangeFv), so the ctor is written as a C-style function
// carrying the retail symbol (same approach as __ct__CMenuLvUp). The vtable
// is stored explicitly so the reloc names the retail .data label
// lbl_eu_80532AA8 instead of the compiler __vt__ symbol.
extern "C" cf::CPartsChange* __ct__cf_CPartsChange(cf::CPartsChange* self) {
    *reinterpret_cast<void**>(self) = lbl_eu_80532AA8;
    self->mField04 = 0;
    self->mField08 = 0;
    self->mField2C = 0;
    memset(self->mData, 0xff, 0x20);
    return self;
}

cf::CPartsChange::~CPartsChange() {}

// Populate the party-change slots from a name table: for each of the 0x20
// slots, query the object's vf20 for its table size, format "prefix + 2-digit
// index" via itoa/format, then scan vf24 entries for a strstr match on the
// formatted name; store the matching index (or -1) into mData[i] and mark
// mField2C. Then run the final pass func_80192F94.
void func_80192E80(CPartsChange* self, u8 r4, PartsChangeIf* obj) {
    if (obj == 0) return;
    self->mField08 = obj;
    self->mField04 = r4;
    int result; // declared before the loop -> higher saved reg (retail r27)
    for (u32 i = 0; i < 0x20; i++) {
        if (self->mField08 == 0) {
            result = -1;
        } else {
            u32 count = self->mField08->vf20();
            ml::FixStr<16> buf;
            ml::FixStr<16> buf2;
            ml::CPathUtil::itoa(buf, (int)i, 2);
            buf2.format(lbl_eu_80503BFC, lbl_eu_806624E8, buf.c_str());
            result = 0;
            while ((u32)result < count) {
                if (strstr(self->mField08->vf24(result), buf2.c_str()) != 0) break;
                result++;
            }
            if ((u32)result >= count) result = -1;
        }
        if (result >= 0) {
            self->mData[i] = (u8)result;
            self->mField2C = 1;
        }
    }
    func_80192F94(self);
}

// First pass over the 0x20 change slots: for each non-0xff entry, either
// force-apply it (vf30(c, 0)) once an accept has been found, or ask whether
// it is acceptable (vf34(c)) to set the accept flag. If nothing was accepted,
// a second pass force-applies the first non-0xff entry with flag 1.
void func_80192F94(CPartsChange* self) {
    if (self->mField2C == 0) return;
    if (self->mField08 == 0) return;
    if (self->mField04 != 0) return;
    int hit = 0;
    for (u32 i = 0; i < 0x20; i++) {
        u8 c = self->mData[i];
        if (c == 0xff) continue;
        if (hit) {
            self->mField08->vf30(c, 0);
        } else {
            if (self->mField08->vf34(c)) hit = 1;
        }
    }
    if (hit) return;
    for (u32 i = 0; i < 0x20; i++) {
        u8 c = self->mData[i];
        if (c != 0xff) {
            self->mField08->vf30(c, 1);
            return;
        }
    }
}

// Party-change slot update: when the change list is active, force-apply every
// non-0xff slot entry (vf30(c, 0)), then apply the indexed entry with the
// given flag. When the list is active, the indexed entry exists and the flag
// is 0, scan for the first other non-0xff entry and force-apply it with
// flag 1. (MWCC emits the dead second beq pair for the inner loop guard -
// the goto-gate chain pattern seen in the matched reslist dtors.)
void func_801930A0(CPartsChange* self, u32 idx, u32 flag) {
    if (self->mField2C != 0 && self->mField08 != 0) {
        if (self->mField04 == 0 && self->mField2C != 0 && self->mField08 != 0) {
            for (u32 i = 0; i < 0x20; i++) {
                u8 c = self->mData[i];
                if (c != 0xff) self->mField08->vf30(c, 0);
            }
        }
        u8 c = self->mData[idx];
        if (c != 0xff) {
            self->mField08->vf30(c, flag);
            if (self->mField04 == 0 && flag == 0) {
                for (u32 i = 0; i < 0x20; i++) {
                    u8 c2 = self->mData[i];
                    if (c2 == 0xff) continue;
                    if (i == idx) continue;
                    self->mField08->vf30(c2, 1);
                    break;
                }
            }
        }
    }
}

// Initialize an element from the BDAT table: clear the busy bit, derive a
// scale from a column value (its first byte, forced to 1 when zero), and
// clear the +0x42 sub-block. The u8->f32 cast uses the builtin 2^52 trick;
// lbl_eu_80667AC8 is defined above so the pool reloc matches retail.
void func_801931D0(CfPartsElem4C* self) {
    self->field_1E &= ~4u;
    u32 val = getBdatStringColumnValue(lbl_eu_806640A8, lbl_eu_80503C48,
                                       lbl_eu_80664184);
    u8 b = *(const u8*)&val;
    if (b == 0) b = 1;
    self->field_14 = lbl_eu_80667AC0;
    self->field_10 = lbl_eu_80667AB8 * (lbl_eu_80667ABC * (f32)(u32)b);
    memset(&self->field_42[0], 0, 9);
}

// Manager constructor (retail splitter name __ct__80193270): inline the
// reslist<CfPartyInfo> ctor (the _reslist_base vtable store is a dead store
// MWCC keeps - the manager's own vtable store then overwrites it), init the
// tail floats/counters and the two manager globals, zero the 0xA4 table,
// then allocate the 16-node list array and clear the nodes: node 0 is
// cleared through the fresh allocation local (retail reuses the call result
// register), the rest through mList so MWCC reloads it per store
// (CTaskGameEff::Init shape). Returns self (retail `mr r3,r30` before the
// node loop is the early return-value setup).
extern "C" CfPartsManager* __ct__80193270(CfPartsManager* self) {
    self->mPartyList.vtable = (void*)lbl_eu_80532AE4;
    self->mElems.mCount = 0;
    self->field_A804 = 0;
    self->mPartyList.mList = 0;
    self->mPartyList.mCapacity = 0;
    self->mPartyList.mOwnsList = 0;
    self->mPartyList.mStartNodePtr = &self->mPartyList.mStartNode;
    self->mPartyList.mStartNodePtr->mNext = &self->mPartyList.mStartNode;
    self->mPartyList.mStartNodePtr->mPrev = self->mPartyList.mStartNodePtr;
    self->mPartyList.vtable = (void*)lbl_eu_80532ACC;
    self->field_B268 = lbl_eu_80667AD0;
    self->field_B26C = lbl_eu_80667AD0;
    self->field_B270 = 1;
    self->field_B276 = 15;
    self->field_B278 = 1;
    lbl_eu_80664308 = (u32)self;
    lbl_eu_8066430C = (u32)self;
    lbl_eu_80664310 = 0;
    lbl_eu_80664314 = 0;
    memset(self->mTable, 0, 0xa40);
    CfPartyListNode* arr = (CfPartyListNode*)allocate_array__Q23mtl10MemManagerFUlUl(0xc0, func_80061FFC());
    self->mPartyList.mList = arr;
    arr[0].mNext = 0;
    // Clear the remaining nodes through mList so MWCC reloads it per store.
    for (u32 j = 1; j < 16; j++) {
        self->mPartyList.mList[j].mNext = 0;
    }
    self->mPartyList.mCapacity = 16;
    return self;
}

// _reslist_base<cf::CfPartyInfo> deleting destructor (retail symbol uses the
// old flat template mangling). The retail vtable store names lbl_eu_80532AE4
// rather than the compiler __vt__ symbol, so the dtor is written as a plain
// global function over the layout mirror - same approach as the matched
// CUIBattleManager / IResInfo reslist dtors.
void* __dt___reslist_base_cf_CfPartyInfo(CfPartyListBase* self, int mode) {
    if (self != NULL) {
        self->vtable = lbl_eu_80532AE4;
        {
            CfPartyListNode* cur = self->mStartNodePtr->mNext;
            while (cur != self->mStartNodePtr) {
                CfPartyListNode* prev = cur;
                cur = cur->mNext;
                prev->mNext = NULL;
            }
            self->mStartNodePtr->mNext = self->mStartNodePtr;
            self->mStartNodePtr->mPrev = self->mStartNodePtr;
        }
        if (self->mOwnsList == 0 && self->mList != NULL) {
            delete[] self->mList;
            self->mList = NULL;
        }
        if (mode > 0) {
            delete self;
        }
    }
    return self;
}

void* __dt__reslist_cf_CfPartyInfo(CfPartyListBase* self, int mode) {
    CfPartyListNode* cur;
    CfPartyListNode* prev;
    // Doubled null check mirrors retail: MWCC keeps the dead second beq
    // (same shape as the matched IResInfo / CfMapMineManager reslist dtors).
    if (self != NULL) {
        if (self != NULL) {
            self->vtable = lbl_eu_80532AE4;
            cur = self->mStartNodePtr->mNext;
            while (cur != self->mStartNodePtr) {
                prev = cur;
                cur = cur->mNext;
                prev->mNext = NULL;
            }
            self->mStartNodePtr->mNext = self->mStartNodePtr;
            self->mStartNodePtr->mPrev = self->mStartNodePtr;
            if (self->mOwnsList == 0 && self->mList != NULL) {
                delete[] self->mList;
                self->mList = NULL;
            }
        }
    }
    if (mode > 0) {
        delete self;
    }
    return self;
}

// Deleting destructor of the parts-manager object (retail splitter name
// __dt__80193538). Nulls the two manager globals, unlinks the party-info
// reslist and frees its backing array, then runs the inlined
// _reslist_base<cf::CfPartyInfo> dtor body on the same member (retail emits
// the reslist teardown twice; the inner doubled null check mirrors the
// matched __dt__reslist_cf_CfPartyInfo).
void* __dt__80193538(CfPartsManager* self, int mode) {
    if (self != NULL) {
        lbl_eu_80664308 = 0;
        lbl_eu_8066430C = 0;
        {
            CfPartyListNode* cur = self->mPartyList.mStartNodePtr->mNext;
            while (cur != self->mPartyList.mStartNodePtr) {
                CfPartyListNode* prev = cur;
                cur = cur->mNext;
                prev->mNext = NULL;
            }
            self->mPartyList.mStartNodePtr->mNext = self->mPartyList.mStartNodePtr;
            self->mPartyList.mStartNodePtr->mPrev = self->mPartyList.mStartNodePtr;
        }
        if (self->mPartyList.mOwnsList == 0 && self->mPartyList.mList != NULL) {
            delete[] self->mPartyList.mList;
            self->mPartyList.mList = NULL;
        }
        self->mPartyList.mCapacity = 0;
        CfPartyListBase* list = &self->mPartyList;
        if (list != NULL) {
            if (list != NULL) {
                list->vtable = lbl_eu_80532AE4;
                {
                    CfPartyListNode* cur = list->mStartNodePtr->mNext;
                    while (cur != list->mStartNodePtr) {
                        CfPartyListNode* prev = cur;
                        cur = cur->mNext;
                        prev->mNext = NULL;
                    }
                    list->mStartNodePtr->mNext = list->mStartNodePtr;
                    list->mStartNodePtr->mPrev = list->mStartNodePtr;
                }
                if (list->mOwnsList == 0 && list->mList != NULL) {
                    delete[] list->mList;
                    list->mList = NULL;
                }
            }
        }
        if (mode > 0) delete self;
    }
    return self;
}

extern "C" u32 func_80193670() { return (u32)lbl_eu_8066430C; }

// Clear the busy bit and apply the fallback speed to an element of the
// global parts-manager array (id lookup via the +0x1C u16), then reset the
// +0x30 sub-block when the +0x14 speed is still at/below the fallback.
// The found path jumps over the shared `result = 0` so MWCC emits retail's
// single li r3,0 at the exit (an initializer would hoist early).
void func_80193678(u32 id) {
    const CfPartsElem4C* p; // declared first -> lower reg (retail r5)
    CfPartsElemArray* arr = (CfPartsElemArray*)lbl_eu_8066430C;
    if (arr == 0) return;
    CfPartsElem4C* result;
    if (id != 0) {
        p = arr->mElems;
        for (u32 i = 0; i < arr->mCount; p++, i++) {
            if (id == p->field_1C) {
                result = &arr->mElems[i];
                goto found;
            }
        }
    }
    result = 0;
found:
    if (result == 0) return;
    result->field_1E &= ~4u;
    result->field_10 = lbl_eu_80667AD4;
    if (!(result->field_14 <= lbl_eu_80667AD4)) return;
    memset(&result->field_30[0], 0, 9);
}

// Party-part update driven by the +0x45C0 id: gate on the manager global,
// the +0x3F00 flags bit 2, and the id, then dispatch the vtable slot +0xA8
// with 1; scan the element array for the shifted +0x456C id. On a hit clear
// the busy bit, reset the speed to the fallback, and when the speed is still
// at/below the fallback zero the +0x30 sub-block. The scan uses the
// func_80193678 goto shape (found path jumps over the shared result = 0).
void func_80193710(CfPartsChgObj* self) {
    CfPartsElemArray* arr = (CfPartsElemArray*)lbl_eu_8066430C;
    if (arr == 0) return;
    if (!(self->field_3F00 & 0x4)) return;
    if (self->field_45C0 == 0) return;
    int h = self->field_456C >> 4;
    self->vt->_v0A8(1);
    u16 id = (u16)h;
    CfPartsElem4C* result;
    if (id != 0) {
        const CfPartsElem4C* p = arr->mElems;
        for (u32 i = 0; i < arr->mCount; p++, i++) {
            if (id == p->field_1C) {
                result = &arr->mElems[i];
                goto found;
            }
        }
    }
    result = 0;
found:
    if (result == 0) return;
    result->field_1E &= ~4u;
    result->field_10 = lbl_eu_80667AD4;
    if (!(result->field_14 <= lbl_eu_80667AD4)) return;
    memset(&result->field_30[0], 0, 9);
}

extern "C" u32 func_80193804() { return (u32)lbl_eu_80664308; }

// Refresh pass over the registered parts elements: walk the reslist nodes,
// and for each element whose +0xA0 in-use flag is set, resolve each of the
// 16 +0x00 sub-entries through func_800B708C (de-biasing the embedded
// +0x3E9C sub-object), re-bias, validate with func_800B8920 and collect the
// surviving pointers into a stack list; then free each collected pointer via
// func_800B9404. Finally reset the manager: clear the count and +0xA804
// word, unlink every node, re-link the sentinel, zero the 0xA4 table, and
// restore the tail counters/global. The de/re-bias steps are written as
// guarded increments so MWCC emits the retail mr + dead-beq + addi chains.
extern "C" void func_80193810(CfPartsManager* self);
extern "C" void func_8019380C(u32 self) { func_80193810((CfPartsManager*)self); }

#pragma push
#pragma auto_inline off
extern "C" void func_80193810(CfPartsManager* self) {
    u32 p;
    CfPartsCollectList pl;
    pl.count = 0;
    CfPartyListNode* node = self->mPartyList.mStartNodePtr->mNext;
    while (node != self->mPartyList.mStartNodePtr) {
        CfElemA4Full* elem = (CfElemA4Full*)node->field_08;
        if (elem->field_A0 & 1) {
            for (int i = 0; i < 16; i++) {
                p = (u32)func_800B708C(elem->mEntries[i].field_00);
                if (p != 0) p -= 0x3E9C;
                if (p != 0) {
                    u32 arg = p;
                    if (p != 0) arg = p + 0x3E9C;
                    if (func_800B8920((void*)arg) != 0) {
                        pl.arr[pl.count++] = (p != 0) ? (p + 0x3E9C) : p;
                    }
                }
            }
        }
        node = node->mNext;
    }
    for (u32* q = &pl.arr[0]; q != &pl.arr[pl.count]; q++) {
        func_800B9404((void*)*q);
    }
    self->mElems.mCount = 0;
    self->field_A804 = 0;
    CfPartyListNode* cur = self->mPartyList.mStartNodePtr->mNext;
    while (cur != self->mPartyList.mStartNodePtr) {
        CfPartyListNode* prev = cur;
        cur = cur->mNext;
        prev->mNext = 0;
    }
    self->mPartyList.mStartNodePtr->mNext = self->mPartyList.mStartNodePtr;
    self->mPartyList.mStartNodePtr->mPrev = self->mPartyList.mStartNodePtr;
    memset(self->mTable, 0, 0xa40);
    self->field_B272 = 5;
    self->field_B270 = 1;
    self->field_B274 = 0;
    lbl_eu_80664314 = 0;
}
#pragma pop

// Append a new 0x4C-byte element to the parts array if no existing element
// carries the id: scan the live range, then build the element on the stack
// (three fallback floats, id/flags words, four zeroed 9-byte sub-blocks),
// clear the busy bit, bump the count, and memcpy it into the array tail.
void func_8019397C(CfPartsElemArray* self, u32 id) {
    func_8003AA34();
    if (id == 0) return;
    CfPartsElem4C* end = self->mElems + self->mCount;
    for (CfPartsElem4C* e = self->mElems; e != end; e++) {
        if (id == e->field_1C) return;
    }
    CfPartsElem4C elem;
    elem.field_10 = lbl_eu_80667AD4;
    elem.field_14 = lbl_eu_80667AD4;
    elem.field_18 = lbl_eu_80667AD4;
    elem.field_1C = (u16)id;
    elem.field_1E = 1;
    elem.field_22 = 0;
    elem.field_25 = 0;
    elem.field_26 = 0;
    memset(&elem.field_27[0], 0, 9);
    memset(&elem.field_30[0], 0, 9);
    memset(&elem.field_39[0], 0, 9);
    memset(&elem.field_42[0], 0, 9);
    elem.field_1E &= ~0x400u;
    u32 count = self->mCount;
    self->mCount = count + 1;
    memcpy(&self->mElems[count], &elem, 0x4c);
}

void CfActorAccessors::func_80193A88(int enable) { if (enable) mFlags1E |= 0x400; else mFlags1E &= ~0x400; }

// Linear scan of the 16-entry 0xA4-stride table: return the element whose u32
// key at +0x94 matches id (null if id == 0 or not found).
// auto_inline off: retail keeps the `bl func_80193AB0` from callers (the
// retail build had this helper in a different TU); -ipa file would inline it.
#pragma push
#pragma auto_inline off
CfElemA4* func_80193AB0(CfTableA4* self, u32 id) {
    CfElemA4* result = 0;
    if (id != 0) {
        for (u32 i = 0; i < 16; i++) {
            if (self->mElems[i].field_94 == id) {
                result = &self->mElems[i];
                break;
            }
        }
    }
    return result;
}
#pragma pop

u32 CfActorAccessors::func_80193B04() { return mField94; }

// Register a parts element: scan the 16-entry table for the first element
// whose +0xA0 in-use flag (bit 0) is clear; initialize its state block
// (0x80 memset + the func_80193C74 field sequence, then the +0x94 word
// re-written from the manager's +0xB270 counter and the +0xA0 flag set),
// wrap the counter past zero to 1, then claim the first free 0xc-byte node
// and link it just before the list head (the setItem-style guarded item
// store forces retail's mr r31,r1 frame anchor + addic. null check). The
// state stores are written through the array index so MWCC keeps the
// i*0xA4 scale in a saved register across the memset (retail r27). Returns
// the initialized element, or null when all 16 are busy.
CfElemA4Full* func_80193B0C(CfPartsManager* self, u16 arg2) {
    CfElemA4Full* result = 0;
    for (int i = 0; i < 16; i++) {
        CfElemA4Full* elem = &((CfElemA4Full*)self->mTable)[i];
        if ((elem->field_A0 & 1) == 0) {
            memset(elem, 0, 0x80);
            ((CfElemA4Full*)self->mTable)[i].field_A2 = 0;
            ((CfElemA4Full*)self->mTable)[i].field_A0 = 0;
            ((CfElemA4Full*)self->mTable)[i].field_90 = lbl_eu_80667AD4;
            ((CfElemA4Full*)self->mTable)[i].field_94 = 0;
            ((CfElemA4Full*)self->mTable)[i].field_98 = 0;
            ((CfElemA4Full*)self->mTable)[i].field_9A = 0;
            ((CfElemA4Full*)self->mTable)[i].field_94 = self->field_B270;
            ((CfElemA4Full*)self->mTable)[i].field_A0 = 1;
            ((CfElemA4Full*)self->mTable)[i].field_9E = arg2;
            u16 v = self->field_B270 + 1;
            self->field_B270 = v;
            if (v == 0) self->field_B270 = v + 1;
            CfPartyListNode* head = self->mPartyList.mStartNodePtr;
            int cap = self->mPartyList.mCapacity;
            int slot = 0;
            while (slot < cap) {
                if (self->mPartyList.mList[slot].mNext == 0) break;
                slot++;
            }
            CfPartyListNode* node = &self->mPartyList.mList[slot];
            u32* ptr = &node->field_08;
            if (ptr != nullptr) {
                try {
                    *ptr = (u32)elem;
                } catch (...) {
                    throw;
                }
            }
            node->mNext = head;
            node->mPrev = head->mPrev;
            head->mPrev->mNext = node;
            head->mPrev = node;
            result = elem;
            break;
        }
    }
    return result;
}

// Zero the first 0x80 bytes, then write the tail fields in retail store order
// (A2, A0, 90, 94, 98, 9A).
#pragma push
#pragma auto_inline off
void func_80193C74(CfPartState90* self) {
    memset(self, 0, 0x80);
    self->field_A2 = 0;
    self->field_A0 = 0;
    self->field_90 = lbl_eu_80667AD4;
    self->field_94 = 0;
    self->field_98 = 0;
    self->field_9A = 0;
}

u16 CfActorAccessors::func_80193CC8() { return mField9E; }
#pragma pop

// Walk the +0xA80C linked list for a node whose element key (+0x94) matches
// the second argument's +0x45C0 id; resolve the matched element's actor id
// via func_800B708C and de-bias the embedded +0x3E9C sub-object (null-safe).
// The id==0 path jumps straight to the shared epilogue in retail, so the scan
// is nested in `if (id != 0)` rather than an early return.
u32 func_80193CD0(CfPartsListA80C* self, CfPartsIdView* other) {
    u32 result = 0;
    u16 id = other->field_45C0;
    if (id != 0) {
        CfPartsNode* head = self->head;
        CfPartsNode* cur = head->next;
        while (cur != head) {
            if (cur->data->field_94 == id) {
                result = (u32)func_800B708C(cur->data->field_00);
                if (result != 0) result -= 0x3E9C;
                break;
            }
            cur = cur->next;
        }
    }
    return result;
}

// Party-part rebuild: reset the manager, then walk the BDAT table rows
// [func_8003B41C, func_8003B1EC) rebuilding each 0x4C element: position from
// three s16 columns, a scale factor from a u16 column, flag bytes, and the
// 5-equipment pass that builds column names in the two shared string buffers
// (lbl_eu_806624F8 / lbl_eu_806624FC), accumulates a level total/max into
// field_25/field_26 and collects resolved object ids into the stack array.
// The final element is memcpy'd into the manager's element array.
void func_80193D48(CfPartsManager* mgr) {
    func_8003AA34();
    void* bdat = (void*)func_80086B24__Q22cf13CfGameManagerFv();
    u32 bdat3 = lbl_eu_80664094;
    void* bdat2 = lbl_eu_806640CC;
    func_80193810(mgr);
    if (bdat == 0) return;
    int n0 = func_8003B41C(bdat);
    int n1 = func_8003B1EC(bdat);
    f64 f28 = lbl_eu_80667AF0;
    f64 f30 = lbl_eu_80667AC8;
    f32 f31 = lbl_eu_80667AEC;
    f32 f27 = lbl_eu_80667AE8;
    f32 f29 = lbl_eu_80667AD4;
    const char* cols = lbl_eu_80503C48;
    int count = 0;
    u32 arr[0x60];
    u32 r15 = 0;
    u8 r14 = 0, r18 = 0;
    int r17 = 0;
    f32 f26 = 0;
    ml::CVec3 tmp;
    for (int i = 0; i < n1; i++) {
        int id = i + n0;
        u32 v = getBdatStringColumnValue(bdat, cols + 0x0c, id);
        tmp.x = (f32)(s16)*(const s16*)&v;
        u32 v1 = getBdatStringColumnValue(bdat, cols + 0x11, id);
        tmp.y = (f32)(s16)*(const s16*)&v1;
        u32 v2 = getBdatStringColumnValue(bdat, cols + 0x16, id);
        tmp.z = (f32)(s16)*(const s16*)&v2;
        u32 v3 = getBdatStringColumnValue(bdat, cols + 0x1b, id);
        f26 = (f32)(u16)*(const u16*)&v3;
        u32 v4 = getBdatStringColumnValue(bdat, cols + 0x22, id);
        r14 = *(const u8*)&v4;
        u32 v5 = getBdatStringColumnValue(bdat, cols + 0x2b, id);
        r18 = *(const u8*)&v5;
        getBdatStringColumnValue(bdat, cols + 0x34, id);
        u32 v6 = getBdatStringColumnValue(bdat, cols + 0x39, id);
        u8 type = *(const u8*)&v6;
        if (type == 1) {
            nw4r::math::VEC3Scale(tmp, tmp, f27);
        } else if (type == 2) {
            nw4r::math::VEC3Scale(tmp, tmp, f31);
        }
        CfPartsElem4C elem;
        elem.field_10 = f29;
        elem.field_14 = f29;
        elem.field_18 = f29;
        elem.field_1C = (u16)id;
        elem.field_1E = 0;
        elem.field_20 = r18;
        elem.field_22 = 0;
        elem.field_24 = r14;
        elem.field_25 = 0;
        elem.field_26 = 0;
        memset(elem.field_27, 0, 9);
        memset(elem.field_30, 0, 9);
        memset(elem.field_39, 0, 9);
        memset(elem.field_42, 0, 9);
        for (int k = 1; k <= 5; k++) {
            lbl_eu_806624F8[3] = (char)(k + 0x30);
            u32 a1 = getBdatStringColumnValue(bdat, lbl_eu_806624F8, id);
            lbl_eu_806624FC[3] = (char)(k + 0x30);
            u32 a2 = getBdatStringColumnValue(bdat, lbl_eu_806624FC, id);
            int a = (int)(f32)(u16)*(const u16*)&a1;
            int b = (int)(f32)(u16)*(const u16*)&a2;
            elem.field_27[k] = 0;
            elem.field_42[k] = 0;
            if (a == 0 || b == 0) continue;
            r17 = 1;
            u32 t2v = getBdatStringColumnValue(bdat2, cols + 0x40, id);
            u8 type2 = *(const u8*)&t2v;
            if (type2 >= 4) {
                elem.field_1E |= 0x200;
                u32 r3v = getBdatStringColumnValue((void*)bdat3, cols + 0x46, (u16)a);
                r15 = (u32)func_800AA714((void*)r3v, (u32)(cols + 0x46));
                if (r15 != 0) {
                    int found = 0;
                    if (count > 1) {
                        for (int q = 1; q < count; q++) {
                            if (arr[q] == r15) {
                                found = 1;
                                break;
                            }
                        }
                    }
                    if (!found) {
                        int lvl = (int)func_80063A60((void*)r15);
                        int q = lvl / 201314304;
                        if (lvl % 201314304 != 0) q++;
                        elem.field_25 = (u8)(elem.field_25 + (u8)q);
                        if (q > elem.field_26) elem.field_26 = (u8)q;
                        arr[count++] = r15;
                    }
                }
            } else if ((u16)(type2 - 2) <= 1) {
                elem.field_1E |= 0x100;
            }
        }
        if (r17 != 0) {
            ml::CVec3 t2 = tmp;
            elem.mPos = tmp;
            elem.field_0C = f26;
            elem.field_24 = r14;
            elem.field_20 = r18;
            elem.field_18 = f29;
            if (r18 & 1) elem.field_1E |= 8;
            u32 c = mgr->mElems.mCount;
            mgr->mElems.mCount = c + 1;
            memcpy(&mgr->mElems.mElems[c], &elem, 0x4c);
        }
    }
}

// Average position of the gimmick objects (func_800B6BC8 list) whose
// sphere-vs-sphere push-apart against `in` (func_800A5488 with the radius
// args f / vfE0) reports a hit; the accumulated tmp position is added into
// out. With no hits the input position is copied verbatim. The average is
// computed as 1/count (2^52 u32->f32 conversion) times each component, and
// the return value is (count > 0).
int func_80194264(f32 f, ml::CVec3* out, const ml::CVec3* in) {
    *out = ml::CVec3::zero;
    CfActorList* list = (CfActorList*)func_800B6BC8();
    CfActorListNode* head = list->mHead;
    CfActorListNode* node = head->mNext;
    int count = 0;
    while (node != head) {
        CfPartsElemObj* elem = (CfPartsElemObj*)node->mElem;
        node = node->mNext;
        f32 a = elem->vt->vfE0(elem);
        ml::CVec3* p = (ml::CVec3*)elem->vt->vfAC(elem);
        ml::CVec3 tmp;
        if (func_800A5488(*in, *p, &tmp, f, a)) {
            out->x += tmp.x;
            out->y += tmp.y;
            out->z += tmp.z;
            count++;
        }
    }
    if (count > 0) {
        f32 inv = lbl_eu_80667AD0 / (f32)(u32)count;
        out->x *= inv;
        out->y *= inv;
        out->z *= inv;
    } else {
        *out = *in;
    }
    return count > 0;
}

// Try up to 5 randomized spawn offsets: each retry picks a random facing
// (mtRand(0x168) = 0..359) and a jitter (mtRand(0x64)), nudges the input
// position by table[i] * jitter along the facing (CosFIdx/SinFIdx rotation
// with the two scale constants), then ground-probes it via func_800A7094
// (scene query 0x44A11) when flag is set. On a successful probe the position
// is snapped to the probe result, averaged against nearby actors
// (func_80194264), and the facing index is returned; otherwise the retry
// loop continues. flag == 0 returns the first nudge without probing.
u32 func_801943E4(ml::CVec3* out, const ml::CVec3* in, int flag, f32 fA, f32 fB) {
    F64Conv c1, c2;
    ml::CVec3 tmp14;
    ml::CVec3 tmp8;
    const f32* tbl = lbl_eu_80532AB8;
    f64 magic = lbl_eu_80667AF0;
    f32 c27 = lbl_eu_80667AF8;
    f32 c26 = lbl_eu_8066A210;
    f32 c25 = lbl_eu_80667AEC;
    f32 f30 = lbl_eu_80667AFC * c26;
    u32 rnd = 0;
    int i = 0;
    do {
        rnd = (u32)ml::math::mtRand(0x168);
        u32 r3 = (u32)ml::math::mtRand(0x64);
        c1.w[1] = r3 ^ 0x80000000;
        c1.w[0] = 0x43300000;
        c2.w[1] = rnd ^ 0x80000000;
        c2.w[0] = 0x43300000;
        f32 f2 = (f32)(c1.d - magic) * c25;
        f32 t1 = *tbl * f2;
        f32 f24 = (f32)(c2.d - magic) * c26;
        f32 f23 = fA * t1;
        out->x = in->x;
        out->y = in->y;
        out->z = in->z;
        out->x += f23 * nw4r::math::CosFIdx(c27 * f24);
        out->z += f23 * nw4r::math::SinFIdx(c27 * f24);
        if (flag != 0) {
            if (func_800A7094(out, &tmp14, 0x44A11, lbl_eu_80667B00,
                              nw4r::math::SinFIdx(c27 * f30)) != 0) {
                *out = tmp14;
                if (func_80194264(lbl_eu_80667B04 * fB, &tmp8, out) != 0) {
                    *out = tmp8;
                }
                break;
            }
        } else {
            break;
        }
        i++;
        tbl++;
    } while (i < 5);
    return rnd;
}

// Party-change apply: when the flag gate allows it, randomize the spawn
// offset via func_801943E4 (or read the +0x54 u16 column directly), resolve
// the player position, then resolve the target object via func_80081A40
// (de-biased by 0x3E9C) and apply the per-flag-bit updates: the +0x3F00/
// +0x3F04 flag words, the element flags via func_800BE12C / func_800BC4B8,
// the facing via the sub-object vfD0, and the +0x456C/+0x45C2 ids. Returns
// the de-biased object or null.
CfPartsChgObj3F04* func_80194610(CfPartsManager* mgr, u32 arg2, u32 arg3,
                                 u32 arg4, CfPartsElem4C* src) {
    void* bdat = (void*)func_80086B24__Q22cf13CfGameManagerFv();
    u32 val = getBdatStringColumnValue(bdat, lbl_eu_80503C48 + 0x4f, arg3);
    ml::CVec3 local14;
    local14.x = src->mPos.x;
    local14.y = src->mPos.y;
    local14.z = src->mPos.z;
    u32 flag = 1;
    u8 b = src->field_24;
    u8 bval = *(const u8*)&val;
    if (b != 0) {
        int t = 0;
        if ((b & 1) != 0 && (b & 0x10) == 0) t = 1;
        if (t == 0) flag = 0;
    }
    float local18;
    CfCollideOut local8;
    u32 r28;
    if (flag != 0) {
        f32 fA = src->field_0C;
        if (src->field_0C <= lbl_eu_8066A208 && arg4 > 1) fA = lbl_eu_80667B04;
        r28 = func_801943E4(&local14, (const ml::CVec3*)src, (bval != 0), fA, lbl_eu_80667AD0);
    } else {
        u32 v2 = getBdatStringColumnValue(bdat, lbl_eu_80503C48 + 0x54, arg3);
        r28 = *(const u16*)&v2;
    }
    CfPlayerPosView* player = (CfPlayerPosView*)getPlayer__Q22cf13CfGameManagerFi(0);
    int ok = func_800B99BC(player->vt->vfAC(player), &local14, 1, (src->field_1E >> 6) & 1,
                           &local18, &local8, lbl_eu_80667B08 + mgr->field_B268);
    if (src->field_20 & 0x10) {
        ok = 1;
    } else if (src->field_20 & 0x40) {
        f32 thresh = lbl_eu_80667B0C * lbl_eu_80663ED0;
        ml::CVec3* p2 = player->vt->vfAC(player);
        ok = ml::math::abs(p2->y - local14.y) < thresh;
    }
    if (!ok) return 0;
    CfPartsChgObj3F04* obj = (CfPartsChgObj3F04*)func_80081A40__Q22cf13CfGameManagerFv(
        lbl_eu_80503C48 + 0x58, arg2, arg3, arg4);
    if (obj != 0) obj = (CfPartsChgObj3F04*)((u8*)obj - 0x3E9C);
    if (obj != 0) {
        if (bval != 0) {
            obj->mSub.vt->vfB8(&obj->mSub, &local14, lbl_eu_80667B10);
        } else {
            obj->field_3F00 |= 0x00080000;
            obj->mSub.vt->vf150(&obj->mSub, 1);
            obj->mSub.vt->vfA8(&obj->mSub, &local14);
        }
        if (flag == 0) {
            if ((src->field_24 & 0x10) != 0 && (src->field_1E & 0x10) == 0) {
                func_800BE12C(&obj->mSub, 0x1c, 0, -1, 1);
                obj->field_45CA |= 2;
            } else if (src->field_24 & 0x20) {
                func_800BE12C(&obj->mSub, 1, 1, 0, 1);
                obj->field_45CA |= 4;
            } else if (src->field_24 & 0x80) {
                func_800BE12C(&obj->mSub, 1, 1, 1, 1);
                obj->field_45CA |= 8;
            }
            if (src->field_24 & 0x40) {
                func_800BC4B8(&obj->mSub, lbl_eu_80667B14);
            }
        }
        if (src->field_24 & 0x04) {
            obj->field_3F04 |= 0x00080000;
            obj->field_3F00 |= 0x00020000;
        }
        if (flag == 0) {
            obj->field_3F04 |= 0x00040000;
        }
        obj->mSub.vt->vfD0(&obj->mSub, (f32)(s32)r28);
        obj->field_456C = (u16)(((u16)arg3 << 4) + arg4);
        u32 v3 = getBdatStringColumnValue(bdat, lbl_eu_80503C48 + 0x64, arg3);
        obj->field_45C2 = *(const u16*)&v3;
    }
    return obj;
}

int func_801949E0(u32 a, u8 b) { return 0; }

// Party-drop scan: walk the actor list (func_800B6BC8) and for each element
// whose +0x6D0 id (shifted right 4) is nonzero, verify the party-count
// range from the BDAT columns (+0x6C/+0x74 level gates, +0x80/+0x8A bounds)
// against the CfGameManager counters. Failing the bounds sets the +0x40
// busy bit; passing them re-checks the +0x20 flag and, when clear, asks the
// actor's arts-state object (func_8016FE34 + sub-slot 0x30) whether to set
// the +0x60 bit. Returns whether any element was processed.
int func_80194AFC() {
    CfActorList* list = (CfActorList*)func_800B6BC8();
    if (list->mHead->mNext == list->mHead) return 0;
    u32 bdat = func_80086B24__Q22cf13CfGameManagerFv();
    const char* cols = lbl_eu_80503C48;
    int result = 0;
    CfActorListNode* node = list->mHead->mNext;
    while (node != list->mHead) {
        CfPartyScanElem* elem = (CfPartyScanElem*)node->mElem;
        s32 shifted = (s32)elem->field_6D0 >> 4;
        if (shifted != 0) {
            u16 id = (u16)shifted;
            u32 val = getBdatStringColumnValue((void*)bdat, cols + 0x6c, id);
            int lo = 1;
            int hi = 0xfc;
            int cnt = func_80082354__Q22cf13CfGameManagerFv((u16)val);
            if (cnt != 0) {
                u32 w = getBdatStringColumnValue((void*)bdat, cols + 0x74, id);
                if ((u8)w != 0) {
                    lo = (u8)w;
                    hi = (u8)w;
                }
            }
            int ok1;
            if ((u16)val == 0) ok1 = 1;
            else if ((u32)(u16)lo > (u32)cnt) ok1 = 0;
            else if ((u32)cnt <= (u32)(u16)hi) ok1 = 1;
            else ok1 = 0;
            int ok2 = 0;
            if (ok1) {
                u16 cnt2 = (u16)func_800822F4__Q22cf13CfGameManagerFv();
                u32 c2 = getBdatStringColumnValue((void*)bdat, cols + 0x80, id);
                u32 d2 = getBdatStringColumnValue((void*)bdat, cols + 0x8a, id);
                if ((u16)c2 <= cnt2 && cnt2 <= (u16)d2) ok2 = 1;
            }
            if (!ok2) {
                u16 id2 = (u16)shifted;
                u16 cnt3 = (u16)func_800822F4__Q22cf13CfGameManagerFv();
                u32 c3 = getBdatStringColumnValue((void*)bdat, cols + 0x80, id2);
                u32 d3 = getBdatStringColumnValue((void*)bdat, cols + 0x8a, id2);
                int ok3 = 0;
                if ((u16)c3 <= cnt3 && cnt3 <= (u16)d3) ok3 = 1;
                if (ok3) {
                    if (!(elem->field_68 & 0x20)) {
                        void* obj = func_8016FE34(elem);
                        void* sub = *(void**)((u8*)obj + 4);
                        u32 word = ((CfVt30If*)sub)->vf30()->field_00;
                        if (func_80174C98(obj, &word, 0x803) == 0) {
                            if (((cf::CfResPcPlayerVtIf*)obj)->_v2BC() == 0) {
                                elem->field_68 |= 0x60;
                            }
                        }
                    }
                } else {
                    elem->field_68 |= 0x40;
                }
                result = 1;
            }
        }
        node = node->mNext;
    }
    return result;
}

// Party-part distance pass: walk the 0x4C-stride element array; per element
// compute a per-flag speed f1 (the raw +0x0C, |f - field_0C|, or f - fallback),
// drive func_800B998C, gate on the +0x20 flags / collision byte, then when the
// BDAT level/bounds checks pass re-key the element through func_801949E0 and
// append (element, squared distance) to the +0x9804 distance array while the
// squared distance stays below f*f.
void func_80194D5C(CfPartsManager* mgr, const ml::CVec3* pos, f32 f) {
    u32 r28 = func_8006A6D0();
    void* bdat = (void*)func_80086B24__Q22cf13CfGameManagerFv();
    getPlayer__Q22cf13CfGameManagerFi(0);
    u32 w = lbl_eu_80663E28;
    mgr->field_B272 = (u16)r28;
    mgr->field_A804 = 0;
    int r29 = (w >> 4) & 1;
    f32 f30 = lbl_eu_80667B18;
    f32 f29 = lbl_eu_80667B0C;
    f32 f28 = lbl_eu_80667AD0;
    f32 f27 = lbl_eu_80667B14;
    f32 f26 = f * f;
    const char* cols = lbl_eu_80503C48;
    float local20;
    CfCollideOut local8;
    for (CfPartsElem4C* e = mgr->mElems.mElems; e != &mgr->mElems.mElems[mgr->mElems.mCount]; e++) {
        u8 b = e->field_24;
        f32 f1 = f;
        int flag2 = 1;
        if (b != 0) {
            int t = 0;
            if ((b & 1) != 0 && (b & 0x10) == 0) t = 1;
            if (t == 0) flag2 = 0;
        }
        e->field_1E &= ~1u;
        u8 b2 = e->field_24;
        int b48 = (b2 & 0x48) != 0;
        if (b2 & 0x48) {
            f1 = e->field_0C;
        } else if (flag2) {
            f1 = ml::math::abs(f - e->field_0C);
        } else if ((b2 & 2) != 0 && (e->field_20 & 1) != 0 && f > f27) {
            f1 = f - f27;
        }
        u16 field20 = e->field_20;
        int r24 = 0;
        if ((field20 & 4) != 0 && r29 == 0 && b48 == 0) r24 = 1;
        if (r24) {
            e->field_1E |= 0x40;
        } else {
            e->field_1E &= ~0x40u;
        }
        e->field_18 = f1;
        int ret = func_800B998C((void*)pos, e, 1, r24, &local20, &local8);
        int ok = (ret != 0) && (local8.data[0] == 0);
        if (field20 & 0x10) {
            if (ok) {
                local20 = f28;
                ok = 1;
            } else if (field20 & 0x40) {
                local20 = f28;
                ok = ml::math::abs(pos->y - e->mPos.y) < f29 * lbl_eu_80663ED0;
            }
        } else if (field20 & 0x40) {
            local20 = f28;
            ok = ml::math::abs(pos->y - e->mPos.y) < f29 * lbl_eu_80663ED0;
        }
        if (!ok) continue;
        u16 id = e->field_1C;
        u32 val = getBdatStringColumnValue(bdat, cols + 0x6c, id);
        int lo = 1;
        int hi = 0xfc;
        int cnt = func_80082354__Q22cf13CfGameManagerFv(*(const u16*)&val);
        if (cnt != 0) {
            u32 w74 = getBdatStringColumnValue(bdat, cols + 0x74, id);
            u8 w74b = *(const u8*)&w74;
            if (w74b != 0) {
                lo = w74b;
                hi = w74b;
            }
        }
        int ok1;
        if (*(const u16*)&val == 0) ok1 = 1;
        else if ((u16)lo > (u16)cnt) ok1 = 0;
        else if ((u16)cnt <= (u16)hi) ok1 = 1;
        else ok1 = 0;
        int ok2 = 0;
        if (ok1) {
            u16 cnt2 = (u16)func_800822F4__Q22cf13CfGameManagerFv();
            u32 c2 = getBdatStringColumnValue(bdat, cols + 0x80, id);
            u32 d2 = getBdatStringColumnValue(bdat, cols + 0x8a, id);
            if (*(const u16*)&c2 <= cnt2 && cnt2 <= *(const u16*)&d2) ok2 = 1;
        }
        if (!ok2) continue;
        u32 v94 = getBdatStringColumnValue(bdat, cols + 0x94, id);
        if (func_801949E0(r28, *(const u8*)&v94) == 0) continue;
        if (r24) {
            local20 *= f30;
        }
        e->field_1E |= 1;
        f32 d = local20 * local20;
        if (d >= f26) continue;
        u32 count = mgr->field_A804;
        mgr->field_A804 = count + 1;
        mgr->mDist[count].elem = e;
        mgr->mDist[count].dist = d;
    }
}

// Frame update over the parts element array: decay the +0x14 speed toward 0
// (or reset it to the busy speed when bit 0x400 is set), then when the +0x10
// speed is above the fallback and neither busy bit is set, decay it too;
// once it hits the fallback, clamp it, set the re-arm flag bits and clear
// the +0x30 sub-block.
void func_8019514C(CfPartsManager* self) {
    CfRes_getD80Flag();
    f32 step = func_80496288();
    CfPartsElemArray* arr = &self->mElems;
    for (CfPartsElem4C* e = arr->mElems; e != arr->mElems + arr->mCount; e++) {
        if (e->field_1E & 0x400) {
            e->field_14 = lbl_eu_80667AC0;
        } else if (e->field_14 > lbl_eu_80667AD4) {
            e->field_14 -= step;
            if (e->field_14 < lbl_eu_80667AD4) e->field_14 = lbl_eu_80667AD4;
        }
        if ((e->field_1E & 0x4) == 0 && e->field_10 > lbl_eu_80667AD4 &&
            (e->field_1E & 0x800) == 0) {
            e->field_10 -= step;
            if (e->field_10 <= lbl_eu_80667AD4) {
                e->field_10 = lbl_eu_80667AD4;
                if ((e->field_1E & 0x200) && (e->field_1E & 0x80)) {
                    e->field_1E |= 0x20;
                }
                memset(&e->field_30[0], 0, 9);
            }
        }
    }
}

u32 CfActorAccessors::func_80195284() { return (mFlags1E >> 10) & 0x1u; }

// Return whether the resource slot at +0x1D44 resolves to a non-zero resource
// id (func_8009CF8C is the resource getter; the slot address is the id). Null
// self -> 0.
u32 func_80195290(CfResSlot* self) {
    if (self == 0) return 0;
    return func_8009CF8C((u32)&self->field_1D44) == 0;
}

// Notify the party-info manager when a character id changes: refresh the
// (+0x1D44) resource slot, then validate/replay the entry list. Guarded by
// the global presentation-flag word (bit 0x400000) and the id.
extern "C" void func_801952CC(int id) {
    if (id == 0) return;
    if (lbl_eu_80663E24 & 0x400000) return;
    func_8009D018(id + 0x1D44, 1);
    func_802918AC(id);
    if (func_80291C60(id) == 0) return;
    for (int i = 1; i <= 8; i++) {
        if (func_8008235C__Q22cf13CfGameManagerFv(i) != 0) {
            func_800A282C(func_8009EC9C((u16)i), 1);
        }
    }
    for (int i = 0; i < 3; i++) {
        void* player = getPlayer__Q22cf13CfGameManagerFi(i);
        if (player != 0) {
            func_802761C0(((CfGamePlayerView*)player)->field_74);
        }
    }
}

// Column lookup gate: when the actor's +0x1E flags have bit 5 set, fetch the
// indexed BDAT value and clear the result if its low byte exceeds 3. The low
// byte is extracted with a deref of the value's storage so MWCC emits the
// retail stw+lbz round-trip (MWCC_CASES: cast would fold to rlwinm).
// Retail callers pass four args (r3, r4, r5, r6); the r5 arg is dead, but its
// slot lets MWCC move index out of r3 early (the retail `mr r5, r3`).
u8 func_80195384(int index, int unused, int unused2, CfActorAccessors* actor) {
    u8 result = 1;
    if (actor->mFlags1E & 0x20) {
        u32 val = getBdatStringColumnValue(lbl_eu_806640CC, lbl_eu_80503C48 + 0x40, index);
        u8 v = *(const u8*)&val;
        if (v > 3) result = 0;
    }
    return result;
}

// Party-element update helper called by func_80195B04 (retail `bl` kept
// opaque with the same auto_inline-off pattern as func_80197538; the C
// linkage from the declaration above makes the call reloc name the retail
// symbol).
// Party-element update helper called by func_80195B04 / func_80195BD4 (retail
// `bl` kept opaque with the same auto_inline-off pattern; the C linkage from
// the declaration above makes the call reloc name the retail symbol). The
// retail body returns a success flag (callers branch on r3).
#pragma push
#pragma auto_inline off
extern "C" int func_801953E8(CfPartsElemArray* arr, CfPartsElem4C* elem, u8* flag, int arg4) { return 0; }
#pragma pop

u32 CfActorAccessors::func_80195AC0() { return (mFlags1E >> 3) & 0x1u; }

void CfActorAccessors::func_80195ACC(float val) { mField8C = val; }

void CfObjectPcExt::func_80195AD4(u16 val) { mField45C4 = val; }

void CfObjectPcExt::func_80195ADC(u16 val) { mField45C8 = val; }

u16 CfObjectPcExt::func_80195AE4() { return mField45C6; }

void* CfActorAccessors::func_80195AEC() { return &mField30; }

void* CfObjectPcExt::func_80195AF4() { return (void*)((u8*)this + 0x60c); }

void CfPartyInfo::func_80195AFC(u8 val) { field_2D = val; }

// Spawn helper (called from CfGimmickObject.cpp func_801F72A4): look up the
// element whose u16 id matches the masked argument, mark it active, reset
// its speeds, refresh it through func_801953E8, and set the +0x20/+0x1E
// flag bits. Returns whether an element was found. The found path jumps
// over the shared `result = 0` so MWCC emits retail's single li r3,0
// (func_80193678 shape).
int func_80195B04(int id) {
    u16 id16 = (u16)id;
    CfPartsElemArray* arr = (CfPartsElemArray*)lbl_eu_8066430C;
    CfPartsElem4C* result;
    if (id16 != 0) {
        const CfPartsElem4C* p = arr->mElems;
        for (u32 i = 0; i < arr->mCount; p++, i++) {
            if (id16 == p->field_1C) {
                result = &arr->mElems[i];
                goto found;
            }
        }
    }
    result = 0;
found:
    u8 flag = 0;
    if (result != 0) {
        result->field_1E |= 1;
        result->field_10 = lbl_eu_80667AD4;
        result->field_14 = lbl_eu_80667AD4;
        func_801953E8(arr, result, &flag, 0);
        result->field_20 &= ~8u;
        result->field_1E |= 0x810;
    }
    return result != 0;
}

// Party-slot activation: for up to 8 slots, resolve each slot's part id
// through the BDAT column +0xBC (id is re-derived each iteration), find the
// element carrying that id in the manager's array, and when the element's
// +0x28 state bytes are all clear, reset its busy/speed state (clearing the
// +0x400 re-arm bit and zeroing the +0x30 sub-block when the +0x14 speed is
// still at/below the fallback). Then refresh it via func_801953E8 and count
// the processed elements. arg2 restricts the pass to one slot (i == arg2) and
// stops after the first success; the presentation flag bit 0x04000000 gates
// the whole pass. Returns whether any element was processed.
int func_80195BD4(u16 id, int arg2) {
    int count = 0;
    u32 bdat = func_80086B24__Q22cf13CfGameManagerFv();
    CfPartsManager* mgr = (CfPartsManager*)lbl_eu_8066430C;
    if (lbl_eu_80663E24 & 0x04000000) return 0;
    f32 fallback = lbl_eu_80667AD4;
    for (u32 i = 1; i <= 8; i++) {
        u32 v = getBdatStringColumnValue((void*)bdat, lbl_eu_80503C48 + 0xbc, id);
        // Deref the result's storage so MWCC emits the retail stw+lhz
        // round-trip (cast would fold to rlwinm).
        id = *(u16*)&v;
        if (id == 0) break;
        if (arg2 != 0 && (s32)i != arg2) continue;
        CfPartsElem4C* found;
        if (id != 0) {
            const CfPartsElem4C* p = mgr->mElems.mElems;
            for (u32 j = 0; j < mgr->mElems.mCount; p++, j++) {
                if (id == p->field_1C) {
                    found = &mgr->mElems.mElems[j];
                    goto scan1;
                }
            }
        }
        found = 0;
    scan1:
        if (found == 0) break;
        int hasData = 0;
        if (found->field_27[1] != 0) hasData = 1;
        else if (found->field_27[2] != 0) hasData = 1;
        else if (found->field_27[3] != 0) hasData = 1;
        else if (found->field_27[4] != 0) hasData = 1;
        else if (found->field_27[5] != 0) hasData = 1;
        else if (found->field_27[6] != 0) hasData = 1;
        else if (found->field_27[7] != 0) hasData = 1;
        else if (found->field_27[8] != 0) hasData = 1;
        if (!hasData) {
            CfPartsElem4C* e2;
            if (id != 0) {
                const CfPartsElem4C* p = mgr->mElems.mElems;
                for (u32 j = 0; j < mgr->mElems.mCount; p++, j++) {
                    if (id == p->field_1C) {
                        e2 = &mgr->mElems.mElems[j];
                        goto scan2;
                    }
                }
            }
            e2 = 0;
        scan2:
            if (e2 != 0) {
                e2->field_1E &= ~4u;
                e2->field_10 = fallback;
                if (e2->field_14 <= fallback) {
                    memset(&e2->field_30[0], 0, 9);
                }
            }
            found->field_1E &= ~0x400u;
        }
        found->field_10 = fallback;
        found->field_14 = fallback;
        u8 flag;
        if (func_801953E8(&mgr->mElems, found, &flag, 0) == 0) return 0;
        count++;
        if (arg2 != 0) break;
    }
    return count != 0;
}

// Frame update over the party slots (retail func_80195E5C, called from
// outside this TU): gate on the BDAT table / party count / presentation
// flags, decay the +0xB276 retry timer, then run the element scan
// (func_8019514C) and the per-frame pass: recompute the player speed f
// from the settings flags (80663ED8 vs epsilon, the 80663E28 bits, the
// settings-object 0x8000 bit), and when the speed changed or the actor
// list is under 0x20 nodes, sort the distance array (func_80196434 with
// func_8019641C), rebuild the slots (func_80196E04), then walk the sorted
// entries applying the party-count range checks (func_801953E8 spawns
// each passing element). The +0xB274 index wraps past the count.
extern "C" void func_80195E5C(CfPartsManager* mgr, f32 f) {
    func_8003AA34();
    void* bdat = (void*)func_80086B24__Q22cf13CfGameManagerFv();
    u32 partyCount = func_80086B34__Q22cf13CfGameManagerFv();
    void* player = getPlayer__Q22cf13CfGameManagerFi(0);
    mgr->field_B268 = f;
    if (bdat == 0) return;
    if (partyCount == 0) return;
    if (lbl_eu_80663E28 & 0x20) {
        if (mgr->field_B276 > 0) mgr->field_B276 = 0;
    }
    if (mgr->field_B276 > 0) {
        mgr->field_B276 -= 1;
        mgr->field_B278 = 1;
        return;
    }
    if (func_80084BAC__Q22cf13CfGameManagerFv() != 0) return;
    if ((lbl_eu_80663E24 & 0x02000000) != 0 || (lbl_eu_80663E24 & 0x400) != 0) return;
    if ((lbl_eu_80663E24 & 0x00400000) != 0 && (lbl_eu_80663E28 & 0x30) == 0) return;
    if (func_800829B8__Q22cf13CfGameManagerFv() != 0) return;
    if (player == 0) return;
    func_8019514C(mgr);
    u32 t = (lbl_eu_80663E28 >> 6) & 3;
    int notT = (t == 0) ? 1 : 0;
    u32 bit23 = (lbl_eu_80663EE0 >> 8) & 1;
    u32 r28 = func_8006A6D0();
    u32 r16 = lbl_eu_80663D90;
    u32 bit27 = (lbl_eu_80663E28 >> 4) & 1;
    u32 bit25 = (lbl_eu_80663E28 >> 6) & 1;
    CfGlobalGimmickView* g = getUnk80664658();
    u32 unkBit = 0;
    if (g != 0) {
        unkBit = (getUnk80664658()->field_214 >> 15) & 1;
    }
    int gt = lbl_eu_80663ED8 > lbl_eu_8066A208;
    u32 bit26 = (lbl_eu_80663E28 >> 5) & 1;
    if (bit27 != 0 && bit25 != 0) {
        f = lbl_eu_80667B08;
    } else if (gt) {
        f = lbl_eu_80667ABC;
    } else if (unkBit != 0) {
        f *= lbl_eu_80667B20;
    } else if (bit27 != 0) {
        f *= lbl_eu_80667B20;
    } else if (bit26 != 0 && bit25 != 0) {
        f = lbl_eu_80667ABC;
    } else if (bit26 != 0) {
        f *= lbl_eu_80667B20;
    }
    int changed = mgr->field_B26C != f;
    mgr->field_B26C = f;
    if (mgr->field_B278 == 0 && (r16 & 3) != 0 && r28 == mgr->field_B272 &&
        notT == 0 && bit23 == 0 && changed == 0) {
        goto afterLoop;
    }
    for (u32 i = 0; i < 16; i++) {
        CfElemA4Full* t = &((CfElemA4Full*)mgr->mTable)[i];
        if (!(t->field_A0 & 1)) continue;
        if (t->field_A2 != 0) continue;
        u16 id = t->field_9E;
        CfPartsElem4C* elem;
        if (id != 0) {
            CfPartsManager* gm = (CfPartsManager*)lbl_eu_8066430C;
            const CfPartsElem4C* p = gm->mElems.mElems;
            for (u32 k = 0; k < gm->mElems.mCount; p++, k++) {
                if (id == p->field_1C) {
                    elem = &gm->mElems.mElems[k];
                    goto foundInList;
                }
            }
        }
        elem = 0;
    foundInList:
        if (elem != 0 && elem->field_22 != 0) elem->field_22 = 0;
        t->field_94 = 0;
        t->field_A0 &= ~1u;
    }
    {
        CfPlayerPosView* pv = (CfPlayerPosView*)player;
        ml::CVec3* pos = pv->vt->vfAC(player);
        func_80194D5C(mgr, pos, f);
    }
    if (notT == 0 && bit23 == 0 && mgr->field_B278 == 0) return;
    mgr->field_B278 = 0;
afterLoop:
    ((int (*)(void*))func_80194AFC)(mgr);
    if (r28 != mgr->field_B272 && bit23 == 0) return;
    {
        CfActorList* list = (CfActorList*)func_800B6BC8();
        int n = 0;
        CfActorListNode* node = list->mHead->mNext;
        while (node != list->mHead) {
            node = node->mNext;
            n++;
        }
        if ((u32)n >= 0x20) return;
    }
    u32 cnt = mgr->field_A804;
    func_80196434((CfPartsSwapEntry*)mgr->mDist, (CfPartsSwapEntry*)&mgr->mDist[cnt],
                  (CfPartsSwapCmp)func_8019641C);
    func_80196E04(mgr, f);
    u32 count = mgr->field_A804;
    int hit = 0;
    int limit = 0x10;
    if (bit23 != 0) {
        mgr->field_B274 = 0;
        limit = (int)count;
        lbl_eu_80663EE0 &= ~0x80u;
    } else if (notT != 0 && (count >> 1) > 0x10) {
        limit = (int)(count >> 1);
    }
    const char* cols = lbl_eu_80503C48;
    for (int i = 0; i < (int)count && i < limit; i++) {
        u16 idx = mgr->field_B274;
        idx++;
        mgr->field_B274 = idx;
        if (idx >= count) mgr->field_B274 = 0;
        idx = mgr->field_B274;
        CfPartsElem4C* elem = (CfPartsElem4C*)mgr->mDist[idx].elem;
        u16 eid = elem->field_1C;
        u32 val = getBdatStringColumnValue(bdat, cols + 0x6c, eid);
        int lo = 1;
        int hi = 0xfc;
        int cnt3 = func_80082354__Q22cf13CfGameManagerFv(*(const u16*)&val);
        if (cnt3 != 0) {
            u32 w = getBdatStringColumnValue(bdat, cols + 0x74, eid);
            u8 wb = *(const u8*)&w;
            if (wb != 0) {
                lo = wb;
                hi = wb;
            }
        }
        int ok1;
        if (*(const u16*)&val == 0) ok1 = 1;
        else if ((u32)(u16)lo > (u32)cnt3) ok1 = 0;
        else if ((u32)cnt3 <= (u32)(u16)hi) ok1 = 1;
        else ok1 = 0;
        int ok2 = 0;
        if (ok1) {
            u16 cnt2 = (u16)func_800822F4__Q22cf13CfGameManagerFv();
            u32 c2 = getBdatStringColumnValue(bdat, cols + 0x80, eid);
            u32 d2 = getBdatStringColumnValue(bdat, cols + 0x8a, eid);
            if ((u16)c2 <= cnt2 && cnt2 <= (u16)d2) ok2 = 1;
        }
        if (ok2) {
            if (!(elem->field_20 & 8) && (bit23 == 0 || (elem->field_1E & 0x200))) {
                u8 flag;
                func_801953E8((CfPartsElemArray*)mgr, elem, &flag, 1);
                if (flag != 0) {
                    hit++;
                    if (notT == 0 || hit > 8) return;
                }
            }
        }
    }
}

// 8-byte entry swap used by the three-way sort of func_80196C94 and the
// quicksorts. The old entry is homed in a u32 pair on the stack: field_00
// keeps its value in a register (its slot store is the dead sibling of the
// bitcast slot), while field_04 is re-read as float through its own slot
// (the MWCC lwz/stw/lfs round-trip) so the value travels as f32 on the y
// side.
static inline void swapPartsEntry(CfPartsSwapEntry* x, CfPartsSwapEntry* y) {
    struct { u32 field_00; u32 field_04; } t;
    t.field_00 = x->field_00;
    t.field_04 = *(u32*)&x->field_04;
    x->field_00 = y->field_00;
    x->field_04 = y->field_04;
    y->field_00 = t.field_00;
    y->field_04 = *(f32*)&t.field_04;
}

bool func_8019641C(const CfPartyInfoSortKey* a, const CfPartyInfoSortKey* b) {
    return a->sortKey < b->sortKey;
}

// Introspective quicksort over the 8-byte swap entries [a, b), driven by the
// comparator fn pointer (kept in a local slot whose address doubles as the
// comparator object for func_80196C94 / func_80196864). Small ranges use a
// selection sort; larger ones pick two pseudo-random pivots from the +0x18
// state global, three-way sort them with the last element via func_80196C94,
// then partition around the last element, swap the pivot to the boundary when
// it ends up at the front (equal-element scans), and tail-recurse on the
// smaller half.
void func_80196434(CfPartsSwapEntry* a, CfPartsSwapEntry* b, CfPartsSwapCmp cmp) {
    for (;;) {
        s32 count = (s32)(b - a);
        if (count <= 1) return;
        if (count <= 20) {
            CfPartsSwapEntry* i;
            for (i = a; i != b - 1; i++) {
                CfPartsSwapEntry* min = i;
                if (i != b) {
                    CfPartsSwapEntry* j;
                    for (j = i + 1; j != b; j++) {
                        if (cmp(j, min) != 0) min = j;
                    }
                }
                if (min != i) swapPartsEntry(i, min);
            }
            return;
        }
        s32 state = lbl_eu_80662518;
        s32 q1 = count / 4;
        s32 s = state + 1;
        s32 p1i = q1 + (state % 10);
        if (s >= 5) s = -4;
        s32 p2r = s % 10;
        s32 s2 = s + 1;
        lbl_eu_80662518 = s2;
        if (s2 >= 5) lbl_eu_80662518 = -4;
        s32 p2i = (count * 3) / 4 + p2r;
        CfPartsSwapEntry* pivot = b - 1;
        func_80196C94(&a[p1i], &a[p2i], pivot, (CfPartsSwapCmpObj*)&cmp);
        CfPartsSwapEntry* lo = a;
        CfPartsSwapEntry* hi = pivot;
        while (cmp(lo, pivot) != 0) lo++;
        do {
            hi--;
        } while (lo != hi && cmp(hi, pivot) == 0);
        while (lo < hi) {
            swapPartsEntry(lo, hi);
            lo++;
            while (cmp(lo, pivot) != 0) lo++;
            do {
                hi--;
            } while (cmp(hi, pivot) == 0);
        }
        if (lo == a) {
            swapPartsEntry(a, pivot);
            lo = a + 1;
            if (cmp(a, pivot) == 0) {
                while (lo != b && cmp(a, lo) == 0) lo++;
                if (lo < pivot) swapPartsEntry(lo, pivot);
            }
            while (lo < pivot) {
                while (cmp(a, lo) == 0) lo++;
                while (cmp(a, pivot) != 0) pivot--;
                if (lo >= pivot) break;
                swapPartsEntry(lo, pivot);
                lo++;
            }
        } else {
            if ((lo - a) < (b - lo)) {
                func_80196864(a, lo, (CfPartsSwapCmpObj*)&cmp);
                a = lo;
            } else {
                func_80196864(lo, b, (CfPartsSwapCmpObj*)&cmp);
                b = lo;
            }
        }
    }
}

// Same introspective quicksort as func_80196434 but the comparator is a
// CfPartsSwapCmpObj* (fn pointer at +0 loaded per call) and the pseudo-random
// pivot state uses lbl_eu_8066251C.
void func_80196864(CfPartsSwapEntry* a, CfPartsSwapEntry* b, CfPartsSwapCmpObj* f) {
    for (;;) {
        s32 count = (s32)(b - a);
        if (count <= 1) return;
        if (count <= 20) {
            CfPartsSwapEntry* i;
            for (i = a; i != b - 1; i++) {
                CfPartsSwapEntry* min = i;
                if (i != b) {
                    CfPartsSwapEntry* j;
                    for (j = i + 1; j != b; j++) {
                        if (f->cmp(j, min) != 0) min = j;
                    }
                }
                if (min != i) swapPartsEntry(i, min);
            }
            return;
        }
        s32 state = lbl_eu_8066251C;
        s32 q1 = count / 4;
        s32 s = state + 1;
        s32 p1i = q1 + (state % 10);
        if (s >= 5) s = -4;
        s32 p2r = s % 10;
        s32 s2 = s + 1;
        lbl_eu_8066251C = s2;
        if (s2 >= 5) lbl_eu_8066251C = -4;
        s32 p2i = (count * 3) / 4 + p2r;
        CfPartsSwapEntry* pivot = b - 1;
        func_80196C94(&a[p1i], &a[p2i], pivot, f);
        CfPartsSwapEntry* lo = a;
        CfPartsSwapEntry* hi = pivot;
        while (f->cmp(lo, pivot) != 0) lo++;
        do {
            hi--;
        } while (lo != hi && f->cmp(hi, pivot) == 0);
        while (lo < hi) {
            swapPartsEntry(lo, hi);
            lo++;
            while (f->cmp(lo, pivot) != 0) lo++;
            do {
                hi--;
            } while (f->cmp(hi, pivot) == 0);
        }
        if (lo == a) {
            swapPartsEntry(a, pivot);
            lo = a + 1;
            if (f->cmp(a, pivot) == 0) {
                while (lo != b && f->cmp(a, lo) == 0) lo++;
                if (lo < pivot) swapPartsEntry(lo, pivot);
            }
            while (lo < pivot) {
                while (f->cmp(a, lo) == 0) lo++;
                while (f->cmp(a, pivot) != 0) pivot--;
                if (lo >= pivot) break;
                swapPartsEntry(lo, pivot);
                lo++;
            }
        } else {
            if ((lo - a) < (b - lo)) {
                func_80196864(a, lo, f);
                a = lo;
            } else {
                func_80196864(lo, b, f);
                b = lo;
            }
        }
    }
}

// Three-way ordered insertion of entry c into the pair (a, b) driven by a
// comparator functor (raw fn pointer at +0). The functor is queried as
// f(c,a), f(b,c) and, on the tie paths, f(b,a); the resulting swap chain
// keeps the pair ordered. res1/res2 are bound to the (res==0) booleans so
// MWCC keeps res1 in a saved reg across the second call (cntlzw before it).
void func_80196C94(CfPartsSwapEntry* a, CfPartsSwapEntry* b,
                   CfPartsSwapEntry* c, CfPartsSwapCmpObj* f) {
    bool b1 = f->cmp(c, a) == 0;
    bool b2 = f->cmp(b, c) == 0;
    if (b1 && b2) return;
    if (!b1 && !b2) {
        swapPartsEntry(a, b);
        return;
    }
    if (f->cmp(b, a) != 0) swapPartsEntry(a, b);
    if (b1) swapPartsEntry(b, c);
    else swapPartsEntry(a, c);
}

extern "C" void func_80196E04(CfPartsManager* mgr, f32 f) {}

// Linear scan of the 0x4C-stride element array: return the element whose u16
// id at +0x1C matches, or null (count at +0x9800).
#pragma push
#pragma auto_inline off
CfPartsElem4C* func_8019747C(CfPartsElemArray* self, u32 id) {
    if (id != 0) {
        for (u32 i = 0; i < self->mCount; i++) {
            if (self->mElems[i].field_1C == id) return &self->mElems[i];
        }
    }
    return 0;
}
#pragma pop

// Scan the 0x4C-stride element array for the element whose u16 id equals the
// (flags-gated) shifted id from the second argument; return it or null. The
// nested-if shape matches retail's two separate return-0 blocks (the flags
// gate jumps past the whole scan; the zero-id gate jumps to the scan's own
// trailing return).
CfPartsElem4C* func_801974CC(CfPartsElemArray* self, CfPartsFlagView* other) {
    if (other->field_3F00 & 0x4) {
        int h = other->field_456C >> 4;
        if (h != 0) {
            u16 id = (u16)h;
            const CfPartsElem4C* p = self->mElems;
            for (u32 i = 0; i < self->mCount; i++, p++) {
                if (p->field_1C == id) return &self->mElems[i];
            }
        }
        return 0;
    }
    return 0;
}

// Slot removal helper (same-TU sibling, retail unmangled symbol); forward
// declared here for func_80197538 (the definition below inherits C linkage).
extern "C" int func_80198284(CfPartsSlots* self, CfPartsSlots* other);

// Party-change refresh (retail func_80197538, called by func_80197B4C /
// func_80197BA4 with the global manager): when the actor is active (busy
// flag or +0x3F08 activity bit) or idle with bit 0x20 of +0x3F00 set, walk
// the 16 table slots whose +0x45C0 id matches, resolve each slot's actor
// (func_800B708C, de-biased by 0x3E9C), skip battle members owned by the
// battle manager (func_800DA06C) unless it is the actor itself, then apply
// the equipment-slot update: find the +0x4C element matching the shifted
// +0x456C id, decrement its field_27[lo] counter, bump field_30/field_39
// by the busy state, set the +0x40 bit on +0x3F04 and release the slot
// (func_80198284). The actor itself then gets the same element update, and
// when active the field_30[] vs field_42[] comparison gate re-initializes
// the element speed from the BDAT columns; the field_27[1..8] data check
// clears the +0x400 re-arm bit. auto_inline off keeps the retail `bl` from
// callers (same pattern as func_80193810).
#pragma push
#pragma auto_inline off
extern "C" void func_80197538(unsigned long manager, int arg) {
    CfPartsChgObjFull* obj;         // r31
    int active;                     // r30
    int busy;                       // r29
    const CfPartsListEntry* p;      // r28 (mEntries walk)
    void* bm;                       // r27
    CfElemA4Full* tbl;              // r26
    int j;                          // r25
    CfPartsChgObjFull* actor;       // r24
    CfPartsManager* mgr;            // r23
    CfPartsElem4C* elem;            // second part (reuses obj's r31 once dead)
    actor = (CfPartsChgObjFull*)arg;
    mgr = (CfPartsManager*)manager;
    if (!(actor->field_3F00 & 4)) return;
    active = 1;
    if (((cf::CfResPcPlayerVtIf*)actor)->_v2BC() == 0 && (actor->field_3F08 & 0x08000000) == 0) {
        active = 0;
    }
    if (actor->field_3F00 & 0x04000000) {
        bm = getInstance__Q22cf14CBattleManagerFv();
        if (active == 0) {
            tbl = 0;
            u16 id = actor->field_45C0;
            if (id != 0) {
                for (u32 i = 0; i < 16; i++) {
                    if (mgr->mTable[i].field_94 == id) {
                        tbl = (CfElemA4Full*)&mgr->mTable[i];
                        break;
                    }
                }
            }
            if (tbl != 0) {
                p = tbl->mEntries;
                for (j = 0; j < 16; j++, p++) {
                    obj = (CfPartsChgObjFull*)func_800B708C(p->field_00);
                    if (obj != 0) obj = (CfPartsChgObjFull*)((u8*)obj - 0x3E9C);
                    if (obj == 0) continue;
                    if (bm != 0 && func_800DA06C(bm, obj) != 0 && actor != obj) continue;
                    busy = 1;
                    if (((cf::CfResPcPlayerVtIf*)obj)->_v2BC() == 0 && (obj->field_3F08 & 0x08000000) == 0) {
                        busy = 0;
                    }
                    CfPartsElem4C* elem1;   // per-iteration, short-lived (retail r4)
                    if (obj->field_3F00 & 4) {
                        u16 shifted = (u16)(obj->field_456C >> 4);
                        if (shifted != 0) {
                            u32 k = 0;
                            const CfPartsElem4C* p = mgr->mElems.mElems;
                            u32 count = mgr->mElems.mCount;
                            for (; k < count; p++, k++) {
                                if (shifted == p->field_1C) {
                                    elem1 = &mgr->mElems.mElems[k];
                                    goto elemFound1;
                                }
                            }
                        }
                        elem1 = 0;
                    } else {
                        elem1 = 0;
                    }
                elemFound1:
                    if (elem1 != 0) {
                        u16 lo = obj->field_456C & 0xF;
                        u16 o = lo > 8 ? 8 : lo;
                        u8 n = elem1->field_27[o];
                        if (n != 0) {
                            elem1->field_27[o] = n - 1;
                            if (busy != 0) {
                                elem1->field_30[o] += 1;
                            } else {
                                elem1->field_39[o] += 1;
                            }
                        }
                    }
                    obj->field_3F04 |= 0x40;
                    if (obj != actor) {
                        func_80198284((CfPartsSlots*)tbl, (CfPartsSlots*)obj);
                    }
                }
            }
        }
    }
    busy = 1;
    if (((cf::CfResPcPlayerVtIf*)actor)->_v2BC() == 0 && (actor->field_3F08 & 0x08000000) == 0) {
        busy = 0;
    }
    if (actor->field_3F00 & 4) {
        u16 shifted = (u16)(actor->field_456C >> 4);
        if (shifted != 0) {
            u32 k = 0;
            const CfPartsElem4C* p = mgr->mElems.mElems;
            u32 count = mgr->mElems.mCount;
            for (; k < count; p++, k++) {
                if (shifted == p->field_1C) {
                    elem = &mgr->mElems.mElems[k];
                    goto elemFound2;
                }
            }
        }
        elem = 0;
    } else {
        elem = 0;
    }
elemFound2:
    if (elem != 0) {
        u16 lo = actor->field_456C & 0xF;
        u16 o = lo > 8 ? 8 : lo;
        u8 n = elem->field_27[o];
        if (n != 0) {
            elem->field_27[o] = n - 1;
            if (busy != 0) {
                elem->field_30[o] += 1;
            } else {
                elem->field_39[o] += 1;
            }
        }
    }
    if (elem != 0 && active != 0) {
        int ok = 1;
        if (elem->field_30[1] < elem->field_42[1]) ok = 0;
        else if (elem->field_30[2] < elem->field_42[2]) ok = 0;
        else if (elem->field_30[3] < elem->field_42[3]) ok = 0;
        else if (elem->field_30[4] < elem->field_42[4]) ok = 0;
        else if (elem->field_30[5] < elem->field_42[5]) ok = 0;
        else if (elem->field_30[6] < elem->field_42[6]) ok = 0;
        else if (elem->field_30[7] < elem->field_42[7]) ok = 0;
        else if (elem->field_30[8] < elem->field_42[8]) ok = 0;
        if (ok != 0) {
            elem->field_1E &= ~4u;
            u32 val = getBdatStringColumnValue(lbl_eu_806640A8, lbl_eu_80503C48, lbl_eu_80664184);
            u8 b = *(const u8*)&val;
            if (b == 0) b = 1;
            elem->field_10 = lbl_eu_80667AB8 * (lbl_eu_80667ABC * (f32)(u32)b);
            elem->field_14 = lbl_eu_80667AC0;
            memset(&elem->field_42[0], 0, 9);
        } else {
            elem->field_14 = lbl_eu_80667AC0;
        }
    }
    if (elem != 0) {
        int has = 0;
        if (elem->field_27[1] != 0) has = 1;
        else if (elem->field_27[2] != 0) has = 1;
        else if (elem->field_27[3] != 0) has = 1;
        else if (elem->field_27[4] != 0) has = 1;
        else if (elem->field_27[5] != 0) has = 1;
        else if (elem->field_27[6] != 0) has = 1;
        else if (elem->field_27[7] != 0) has = 1;
        else if (elem->field_27[8] != 0) has = 1;
        if (has == 0) elem->field_1E &= ~0x400u;
    }
}
#pragma pop

// Scan the 16-entry 0xA4 table for the id, then use the found element's
// +0x9E flag to re-scan the 0x4C array, clear the match's +0x22 word, reset
// the element's state block (func_80193C74), and finally unlink every live
// party-info node whose item's first word equals the element address.
void func_80197AA0(CfPartsManager* self, u32 id) {
    CfElemA4* elem = func_80193AB0((CfTableA4*)self, id);
    if (elem != 0) {
        u16 flags = func_80193CC8(elem);
        CfPartsElem4C* found = func_8019747C((CfPartsElemArray*)self, flags);
        if (found != 0) {
            found->field_22 = 0;
        }
        func_80193C74((CfPartState90*)elem);
        if (func_80197B28((reslist<cf::CfPartyInfo>*)&self->mPartyList) != 0) {
            func_801986CC((reslist<cf::CfPartyInfo>*)&self->mPartyList,
                          (const CfPartyInfo*)&elem);
        }
    }
}

// Count of live nodes in the reslist sentinel ring: walk mNext from the
// sentinel's successor back to the sentinel (retail lwz/cmplw loop).
#pragma push
#pragma auto_inline off
u32 func_80197B28(const reslist<cf::CfPartyInfo>* self) {
    _reslist_node<cf::CfPartyInfo>* curNode; // declared first -> lower reg (retail r4)
    _reslist_node<cf::CfPartyInfo>* endNode; // retail r5
    u32 length = 0;                          // retail r3
    endNode = self->mStartNodePtr;
    curNode = endNode->mNext;
    while (curNode != endNode) {
        curNode = curNode->mNext;
        length++;
    }
    return length;
}
#pragma pop

// Party-info manager helpers (defined later in retail order). Retail symbols
// are unmangled (bl func_80197538 / func_80197DE8), so C linkage is required
// for the call relocs to match.
extern "C" void func_80197538(unsigned long manager, int arg);
extern "C" void func_80197DE8(CfPartsManager* mgr, CfPartsChgObjFull* actor, int opt1, int opt2);

// Notify the global party-info manager (lbl_eu_8066430C) about an actor
// change: refresh the entry, then re-evaluate it with both option args 0.
void func_80197B4C(int arg) {
    if (arg == 0) return;
    if (lbl_eu_8066430C == 0) return;
    func_80197538(lbl_eu_8066430C, arg);
    func_80197DE8((CfPartsManager*)lbl_eu_8066430C, (CfPartsChgObjFull*)arg, 0, 0);
}

// Party-change entry point from CfObjectPc: when the actor's +0x2BC virtual
// (busy flag) or the +0x3F08 flags word indicates activity, re-arm the
// +0x45C8 id through func_801952CC; then refresh and re-evaluate the actor in
// the global party-info manager. The flag is declared first so MWCC keeps it
// in r31 (retail `li r31,0` before the virtual call).
extern "C" void func_80197BA4(CfPartsChangeObj* self, int opt1, int opt2) {
    if (self == 0) return;
    if (lbl_eu_8066430C == 0) return;
    int flag = 0;
    if (((cf::CfResPcPlayerVtIf*)self)->_v2BC() != 0 ||
        (self->field_3F08 & 0x08000000) != 0) {
        flag = 1;
    }
    if (flag != 0) {
        if (self->field_45C8 != 0) {
            func_801952CC(self->field_45C8);
            self->field_45C8 = 0;
        }
    }
    func_80197538(lbl_eu_8066430C, (int)self);
    func_80197DE8((CfPartsManager*)lbl_eu_8066430C, (CfPartsChgObjFull*)self, opt1, opt2);
}

// Party-change spawn: when the actor id is not already in the party list
// (func_800B6C7C nodes' +0x73C) and the presentation gate (bit 0x04000000)
// is clear, take the queued drop id (lbl_eu_80664312, falling back to
// func_80155660), clear the re-arm flag, then create the party object via
// func_800B957C at the spawn position (Y offset by lbl_eu_80667AE8), drive
// its vf9C/vfC4/vfDC slots, publish the actor id at +0x73C, register it via
// func_800B71CC and play the slot's sound. The second arg is matched against
// the list elements' +0x73C before anything runs.
extern "C" void func_80197C6C(f32 f, u32 id, u32 arg3, const ml::CVec3* pos, u32 arg5);
void func_80197C6C(f32 f, u32 id, u32 arg3, const ml::CVec3* pos, u32 arg5) {
    CfActorList* list = func_800B6C7C();
    CfActorListNode* head = list->mHead;
    CfActorListNode* node = head->mNext;
    int found;
    while (node != head) {
        CfSpawnObj* elem = (CfSpawnObj*)node->mElem;
        if (elem->field_73C == id) {
            found = 1;
            goto spawn;
        }
        node = node->mNext;
    }
    found = 0;
spawn:
    if (found != 0) return;
        if (lbl_eu_80663E24 & 0x04000000) return;
        int w = lbl_eu_80664312;
        w = (w != 0) ? w : func_80155660(arg3, arg5);
        lbl_eu_80664310 = 0;
        if (w == 0) return;
        func_800B75EC();
        CfSpawnObj* obj = (CfSpawnObj*)func_800B957C(w, arg3, pos, f);
        ml::CVec3 v;
        v.x = pos->x;
        v.y = pos->y;
        v.z = pos->z;
        v.y += lbl_eu_80667AE8;
        ((CfSpawnIf*)obj)->vf9C(&v.x);
        ((CfSpawnIf*)obj)->vfC4(f);
        ((CfSpawnIf*)obj)->vfDC(lbl_eu_80667B24);
        obj->field_73C = id;
        func_800B71CC(obj);
        func_801BFC38__Q22cf10CfSoundManFUlUlUlUlf(0, lbl_eu_80662528[w], 0, 0, lbl_eu_80667AD0);
}

extern "C" void func_80197DE8(CfPartsManager* mgr, CfPartsChgObjFull* actor, int opt1, int opt2);
// Slot removal helper (same-TU sibling, retail unmangled symbol); declared
// here so the definition below inherits C linkage and call relocs name it.
extern "C" int func_80198284(CfPartsSlots* self, CfPartsSlots* other);

// Party-slot removal / spawn dispatch (retail unmangled symbol; the extern
// "C" declaration above names the call relocs). Removes the actor from the
// table slot (func_80198284) and unlinks its list node; when the slot was
// free, resets the matching element's +0x22 word and re-arms its +0x1E bit
// (0x80) for active-flag 0x200 elements. When idle (flag == 0) and no opt
// args are given, spawns the party-drop via func_80197C6C with the table's
// +0x9A part (or the actor's +0x3F28 when no table slot matched), the
// actor's +0x3F10 id, and the +0x3E9C sub-object position (vfAC).
extern "C" void func_80197DE8(CfPartsManager* mgr, CfPartsChgObjFull* actor, int opt1, int opt2) {
    int flag = 1;
    if (((cf::CfResPcPlayerVtIf*)actor)->_v2BC() == 0 && !(actor->field_3F08 & 0x08000000)) {
        flag = 0;
    }
    u16 id = actor->field_45C0;
    CfElemA4Full* tbl = 0;
    if (id != 0) {
        const u32* p = &((CfElemA4Full*)mgr->mTable)[0].field_94;
        for (u32 i = 0; i < 16; i++) {
            if (*p == id) {
                tbl = &((CfElemA4Full*)mgr->mTable)[i];
                break;
            }
            p = (const u32*)((const u8*)p + 0xA4);
        }
    }
    if (tbl != 0) {
        if (func_80198284((CfPartsSlots*)tbl, (CfPartsSlots*)actor) == 0) return;
        if (!(tbl->field_A0 & 1)) {
            CfPartsElem4C* elem;
            if (actor->field_3F00 & 0x4) {
                s32 shifted = (s32)actor->field_456C >> 4;
                if (shifted != 0) {
                    u16 sid = (u16)shifted;
                    const CfPartsElem4C* p = mgr->mElems.mElems;
                    for (u32 j = 0; j < mgr->mElems.mCount; p++, j++) {
                        if (sid == p->field_1C) {
                            elem = &mgr->mElems.mElems[j];
                            goto elemFound1;
                        }
                    }
                }
            }
            elem = 0;
        elemFound1:
            if (elem != 0) {
                elem->field_22 = 0;
                if (flag && (elem->field_1E & 0x200)) {
                    elem->field_1E |= 0x80;
                }
            }
            // Unlink every +0xA80C list node whose element pointer matches tbl.
            CfPartsDNode* head = (CfPartsDNode*)((CfPartsListA80C*)mgr)->head;
            CfPartsDNode* node = head->next;
            while (node != head) {
                CfPartsDNode* next = node->next;
                if (node->data == (CfElemA4*)tbl) {
                    node->prev->next = next;
                    next->prev = node->prev;
                    node->next = 0;
                }
                node = next;
            }
            if (flag && opt1 == 0 && opt2 != 0) {
                f32 f = actor->vt->fn_0x5B4(actor);
                ml::CVec3* pos =
                    (ml::CVec3*)((cf::CfResPcParentVtIf*)&actor->mSubVt)->_v0AC();
                func_80197C6C(f, actor->field_3F10, tbl->field_9A, pos, 1);
            }
        }
    } else {
        if (id == 0 && flag && opt1 == 0 && opt2 != 0) {
            f32 f = actor->vt->fn_0x5B4(actor);
            ml::CVec3* pos =
                (ml::CVec3*)((cf::CfResPcParentVtIf*)&actor->mSubVt)->_v0AC();
            func_80197C6C(f, actor->field_3F10, actor->field_3F28, pos, 0);
        }
    }
    if (tbl == 0) {
        CfPartsElem4C* elem;
        if (actor->field_3F00 & 0x4) {
            s32 shifted = (s32)actor->field_456C >> 4;
            if (shifted != 0) {
                u16 sid = (u16)shifted;
                const CfPartsElem4C* p = mgr->mElems.mElems;
                for (u32 j = 0; j < mgr->mElems.mCount; p++, j++) {
                    if (sid == p->field_1C) {
                        elem = &mgr->mElems.mElems[j];
                        goto elemFound2;
                    }
                }
            }
        }
        elem = 0;
    elemFound2:
        if (elem != 0 && flag && (elem->field_1E & 0x200)) {
            elem->field_1E |= 0x80;
        }
    }
}
void func_80198108(int arg) {
    if (lbl_eu_8066430C == 0) return;
    unsigned char* p = (unsigned char*)lbl_eu_8066430C + 0x10000;
    *(unsigned short*)(p - 0x4d88) = (unsigned short)arg;
    if (arg == 0) return;
    unsigned char* p2 = (unsigned char*)lbl_eu_8066430C + 0x10000;
    *(unsigned short*)(p2 - 0x4d8a) = 0;
}

// Allocate the actor's first free slot: scan the 16 8-byte slots for a zero
// first word (MWCC unrolls the fixed-trip scan to the 2x8 mtctr/bdnz shape
// with the dead shadow counter). If no free slot, return 0. When the flag
// argument is 1 and the free slot isn't slot 0, compact slot 0 into it.
// Then fill the slot with other->field_3F10, publish the parts id and slot
// index into other, bump the +0xA2 counter and return 1. The unused 4th
// parameter keeps the flag in r6 (retail cmplwi r6,1) and mirrors the
// func_80195384 dead-slot pattern.
int func_80198138(CfPartsSlots* self, CfPartsSlotChgView* other, u32 unused, u32 flag) {
    int idx = -1;
    for (int i = 0; i < 16; i++) {
        if (self->mEntries[i].field_00 == 0) {
            idx = i;
            break;
        }
    }
    if (idx >= 0) {
        if (idx > 0 && flag == 1) {
            self->mEntries[idx].field_00 = self->mEntries[0].field_00;
            self->mEntries[idx].field_04 = self->mEntries[0].field_04;
            idx = 0;
        }
        self->mEntries[idx].field_00 = other->field_3F10;
        self->mEntries[idx].field_04 = 0;
        other->field_45C0 = self->field_94;
        other->field_45C6 = (u16)idx;
        self->field_A2 = (s16)(self->field_A2 + 1);
        return 1;
    }
    return 0;
}

// Look up the actor id (+0x3F10 of the second arg) among the 16 8-byte
// slots; on match, clear the slot, decrement the +0xA2 counter, and when it
// reaches zero also clear the +0xA0 activity bit and +0x94 state. The two
// guards are combined with && so MWCC shares one trailing `return 0` (retail
// jumps both guards to the same li r3,0 epilogue). The loop re-reads
// field_3F10 through a character-cast so MWCC's range/alias analysis cannot
// hoist it out of the loop (retail reloads it every iteration).
int func_80198284(CfPartsSlots* self, CfPartsSlots* other) {
    if (other != 0 && other->field_3F10 != 0) {
        for (int i = 0; i < 16; i++) {
            u32 entry = self->mEntries[i].field_00;
            if (entry == ((CfPartsSlots*)(u8*)other)->field_3F10) {
                self->mEntries[i].field_00 = 0;
                self->mEntries[i].field_04 = 0;
                s16 count = self->field_A2 - 1;
                self->field_A2 = count;
                if (count <= 0) {
                    self->field_A2 = 0;
                    self->field_94 = 0;
                    self->field_A0 &= 0xFFFE;
                }
                return 1;
            }
        }
    }
    return 0;
}

// Resolve the actor id at +0x00 to its action source (func_800B708C), then
// de-bias the embedded +0x3E9C sub-object back to the owner base (null-safe).
u8* func_80198310(CfActorIdRef* self) {
    u8* base = (u8*)func_800B708C(self->field_00);
    if (base != 0) base -= 0x3E9C;
    return base;
}

// Scan 16 8-byte slots for any non-zero first word; returns 1 on the first
// hit, 0 if all are clear. MWCC's fixed-trip-count unroll turns the linear
// 16-iteration scan into the retail 2x8 mtctr/bdnz loop with the dead
// +(checks-1)=+7 shadow counter (CBattleState func_801490A0 shape,
// MWCC_CASES inlined search helpers).
int func_80198340(CfPartsListEntry* list) {
    for (int j = 0; j < 16; j++) {
        if (list[j].field_00 != 0) return 1;
    }
    return 0;
}

// Scan the 16 8-byte slots for the id; return its index or -1. The fixed
// trip count makes MWCC emit the retail 2x8 mtctr/bdnz unrolled loop with
// the counter returned on match (func_80198340 shape).
int func_80198400(CfPartsListEntry* list, u32 id) {
    if (id != 0) {
        for (int i = 0; i < 16; i++) {
            if (list[i].field_00 == id) return i;
        }
    }
    return -1;
}

void* func_801984E4(void* self, unsigned long idx) {
    return *(void**)((char*)self + (idx << 3));
}

// Resolve the idx-th list entry's actor id to its action source, then de-bias
// the embedded +0x3E9C sub-object back to the owner base (null-safe).
u8* func_801984F0(const CfPartsListEntry* list, u32 idx) {
    u8* base = (u8*)func_800B708C(list[idx].field_00);
    if (base != 0) base -= 0x3E9C;
    return base;
}

// Party-slot refresh: for each of the 16 slots, resolve the actor id through
// func_800B708C (de-biasing the embedded +0x3E9C sub-object). Live actors get
// the 0x04000000 busy bit cleared; unresolvable slots are re-checked and
// cleared when the re-check also resolves. Slot 0 then gets the bit set, its
// BDAT id propagated into field_98 and (unless the 0x4 slot flag is already
// set with a pending field_9A) field_9A. A second pass walks the slots again:
// slots whose BDAT column byte is 2 set field_9A + flag and stop; otherwise a
// 1/3 random pick re-uses field_9A; dead slots are zeroed.
void func_80198524(CfPartsSlots* slots) {
    int i;
    for (i = 0; i < 16; i++) {
        u32 p = (u32)func_800B708C(slots->mEntries[i].field_00);
        if (p != 0) p -= 0x3E9C;
        if (p != 0) {
            ((CfActorPartsView*)p)->field_3F00 &= ~0x04000000u;
        } else {
            u32 q = (u32)func_800B708C(slots->mEntries[i].field_00);
            if (q != 0) q -= 0x3E9C;
            if (q != 0) slots->mEntries[i].field_00 = 0;
        }
    }
    u32 p0 = (u32)func_800B708C(slots->mEntries[0].field_00);
    if (p0 != 0) p0 -= 0x3E9C;
    if (p0 != 0) {
        ((CfActorPartsView*)p0)->field_3F00 |= 0x04000000u;
        slots->field_98 = ((CfActorPartsView*)p0)->field_3F28;
        if (!(slots->field_A0 & 4) || slots->field_9A == 0) {
            slots->field_9A = ((CfActorPartsView*)p0)->field_3F28;
        }
    } else {
        slots->field_98 = 0;
    }
    void* bdat = lbl_eu_806640CC;
    const char* col = lbl_eu_80503C48 + 0x40;
    for (int j = 0; j < 16; j++) {
        u32 p = (u32)func_800B708C(slots->mEntries[j].field_00);
        if (p != 0) p -= 0x3E9C;
        if (p == 0) {
            if (slots->mEntries[j].field_00 != 0) slots->mEntries[j].field_00 = 0;
            continue;
        }
        u16 id = ((CfActorPartsView*)p)->field_3F28;
        u32 val = getBdatStringColumnValue(bdat, col, id);
        if (*(const u8*)&val == 2) {
            slots->field_9A = id;
            slots->field_A0 |= 4;
            break;
        }
        if (ml::math::mtRand(100) <= 30) {
            if (!(slots->field_A0 & 4) || slots->field_9A == 0) {
                slots->field_9A = id;
            }
        }
    }
}

// reslist-style unlink-by-first-word: walk the sentinel ring and splice out
// every node whose mItem first word equals item->field_00 (retail compares
// only the first word, then nulls the spliced node's mNext).
#pragma push
#pragma auto_inline off
void func_801986CC(reslist<cf::CfPartyInfo>* self, const CfPartyInfo* item) {
    _reslist_node<cf::CfPartyInfo>* head = self->mStartNodePtr;
    _reslist_node<cf::CfPartyInfo>* next; // declared before curr -> lower reg (retail r7)
    _reslist_node<cf::CfPartyInfo>* curr = head->mNext;
    while (curr != head) {
        next = curr->mNext;
        if (curr->mItem.field_00 == item->field_00) {
            _reslist_node<cf::CfPartyInfo>* prev = curr->mPrev;
            prev->mNext = next;
            next->mPrev = prev;
            curr->mNext = 0;
        }
        curr = next;
    }
}
#pragma pop

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
    extern float lbl_eu_80667B28;
    field_28 = lbl_eu_80667B28;
    field_2C = 0;
    field_2D = 1;
    field_2E = 0;
}

// Dispatch the party-info state handler selected by the +0x0C state word
// through the retail pmf table lbl_eu_80532AF0; the goal pointer passes
// through in r4. MWCC lowers the pmf call to lis/addi + mulli + bl __ptmf_scall.
int func_8019876C(CfPartyInfoState* self, CfPartsTri* goal) {
    return (self->*lbl_eu_80532AF0[self->field_0C])(goal);
}

// Stub - the retail symbol is unmangled and callers emit `bl func_801987A4`;
// auto_inline off keeps the call opaque (same pattern as func_80197538).
// Party-position selector (retail unmangled symbol; callers tail-call it
// with the 12-byte triple views). When +0x2D is set, probe the scene at the
// destination offset by the (B28, B2C, B28) vector (paired-single VEC3Add)
// and pick the closest probe result around the destination Y, updating the
// +0x24/+0x2C state and the destination Y. Returns 1 normally, 0 when the
// probe fails or the chosen result is inside the +0x2E gate radius.
extern "C" int func_801987A4(CfPartsTri* src, CfPartsTri* dstTri) {
    CfPartyPosSel* self = (CfPartyPosSel*)src;
    ml::CVec3* dst = (ml::CVec3*)dstTri;
    if (self->field_2D == 0) return 1;
    ml::CVec3 out;
    ml::CVec3 tmp14;
    ml::CVec3 tmp8;
    tmp14.x = lbl_eu_80667B28;
    tmp14.y = lbl_eu_80667B2C;
    tmp14.z = lbl_eu_80667B28;
    tmp8 = *dst + tmp14;
    out.x = tmp8.x;
    out.y = tmp8.y;
    out.z = tmp8.z;
    if (func_804BE398(&out, 0x4a05, 0, 1, lbl_eu_80667B30 - self->field_28,
                      lbl_eu_80667B34) == 0) {
        return 0;
    }
    f32 yThresh = lbl_eu_80667B28;
    f32 bestDist = lbl_eu_80667B38;
    f32 secondDist = bestDist;
    f32 secondY = dst->y;
    f32 bestY = secondY;
    f32 bound = secondY - lbl_eu_80667B3C;
    f32 hiBound = lbl_eu_80667B40 + secondY;
    int bestIdx = 0;
    int secondIdx = 0;
    u32 i = 0;
    while (i < func_804BE4AC()) {
        ml::CVec3* obj = (ml::CVec3*)func_804BE520(i);
        if (obj->y > yThresh) {
            ml::CVec3* obj2 = (ml::CVec3*)func_804BE50C(i);
            f32 dist = ml::math::abs(obj2->y - dst->y);
            if (func_804BE5A4(0x40000, i) != 0) {
                if (bestDist <= dist) {
                    bestDist = dist;
                    bestY = obj2->y;
                    bestIdx = (int)i + 1;
                }
            } else {
                if (obj2->y > bound && obj2->y <= hiBound) {
                    secondDist = dist;
                    secondY = obj2->y;
                    secondIdx = (int)i + 1;
                    bound = secondY;
                } else if (secondDist <= dist) {
                    secondDist = dist;
                    secondY = obj2->y;
                    secondIdx = (int)i + 1;
                    bound = secondY;
                }
            }
        }
        i++;
    }
    dst->y = secondY;
    if (bestIdx != 0) {
        if (secondIdx == 0) {
            self->field_2C = 1;
            secondIdx = bestIdx;
            dst->y = bestY;
            self->field_24 = bestY - lbl_eu_80667B3C;
        } else if (bestDist < secondDist && secondY - bestY <= lbl_eu_80667B44) {
            self->field_2C = 1;
            secondIdx = bestIdx;
            dst->y = bestY;
            self->field_24 = secondY;
        }
    }
    if (self->field_2E != 0 && secondIdx != 0) {
        ml::CVec3 out2;
        if (func_804BE53C(&out2, (u32)secondIdx - 1) != 0) {
            if (lbl_eu_80667B48 <= out2.x * out2.x + out2.z * out2.z) {
                return 0;
            }
        }
    }
    return 1;
}

// Copy the 12-byte id/state triple from src to dst (pair of u32 locals so the
// stores land reversed, matching retail), then tail-call the party-info
// processor with both pointers.
void func_80198AC4(CfPartsTri* src, CfPartsTri* dst) {
    u32 w0 = src->field_00;
    u32 w1 = src->field_04;
    dst->field_04 = w1;
    dst->field_00 = w0;
    dst->field_08 = src->field_08;
    func_801987A4(src, dst);
}

// Rotate helper (retail func_80198AE0): copy the 12-byte triple, then
// decompose the s32 turn value field_14: the angle is
// FIdx-scale * (field_18 + (f32)(s32)(field_14 % 360) * 1/256) and the
// magnitude is (f32)(s32)(field_14 % 100) * lbl_eu_80667B4C * field_1C +
// field_20, both added onto the dst x/z words. The cos argument repeats the
// full product so MWCC recomputes it after the sin call (retail reloads the
// constant and the f31-held angle basis instead of spilling the angle). base
// is declared before mag so the saved-FPR claim order lands base in f31 /
// mag in f30 (retail fadds f31 then fmadds f30).
void func_80198AE0(CfPartsMoveSrc* src, CfPartsTri* dst) {
    f32 base = src->field_18 + (f32)(s32)(src->field_14 % 360) * lbl_eu_8066A210;
    f32 mag = (f32)(s32)(src->field_14 % 100) * lbl_eu_80667B4C * src->field_1C + src->field_20;
    dst->field_00 = src->field_00;
    dst->field_04 = src->field_04;
    dst->field_08 = src->field_08;
    *(f32*)&dst->field_00 += mag * nw4r::math::SinFIdx(lbl_eu_80667B50 * base);
    *(f32*)&dst->field_08 += mag * nw4r::math::CosFIdx(lbl_eu_80667B50 * base);
    func_801987A4((CfPartsTri*)src, dst);
}

void func_80198C24(){}

// Copy the 12-byte triple, then rotate the destination x/z by the source's
// turn index: angle = FIdx-scale * field_18, magnitude = (s32)field_14 *
// field_1C, x -= mag*sin(angle), z -= mag*cos(angle). The destination words
// are adjusted in-place as f32 (retail lfs/stfs at +0/+8). The cos argument
// repeats the product so MWCC recomputes it after the sin call (retail
// re-loads the constant and field_18 instead of spilling angle to f30).
extern "C" void func_80198D44(CfPartsMoveSrc* src, CfPartsTri* dst) {
    u32 w0 = src->field_00;
    u32 w1 = src->field_04;
    dst->field_04 = w1;
    dst->field_00 = w0;
    dst->field_08 = src->field_08;
    f32 angle = lbl_eu_80667B50 * src->field_18;
    f32 mag = (f32)(s32)src->field_14 * src->field_1C;
    *(f32*)&dst->field_00 -= mag * nw4r::math::SinFIdx(angle);
    *(f32*)&dst->field_08 -= mag * nw4r::math::CosFIdx(lbl_eu_80667B50 * src->field_18);
    func_801987A4((CfPartsTri*)src, dst);
}

// Same rotate helper as func_80198D44 but with the turn angle offset by
// +lbl_eu_8066A204 (retail fadds): copy the triple, then x -= mag*sin(angle)
// and z -= mag*cos(angle), then tail into the party-info processor. The cos
// argument repeats the product so MWCC recomputes it after the sin call
// (retail reloads the constant instead of spilling the offset angle). mag is
// declared before t so the saved-FPR claim order lands mag in f31 / t in
// f30 (retail fadds f30 then fmuls f31), while t is computed first.
void func_80198E0C(CfPartsMoveSrc* src, CfPartsTri* dst) {
    u32 w0 = src->field_00;
    u32 w1 = src->field_04;
    dst->field_04 = w1;
    dst->field_00 = w0;
    dst->field_08 = src->field_08;
    f32 mag;
    f32 t = src->field_18 + lbl_eu_8066A204;
    mag = (f32)(s32)src->field_14 * src->field_1C;
    *(f32*)&dst->field_00 -= mag * nw4r::math::SinFIdx(lbl_eu_80667B50 * t);
    *(f32*)&dst->field_08 -= mag * nw4r::math::CosFIdx(lbl_eu_80667B50 * t);
    func_801987A4((CfPartsTri*)src, dst);
}

// Same rotate helper as func_80198D44 but with the turn angle offset by
// -lbl_eu_8066A204 (retail fsubs): copy the triple, then x -= mag*sin(angle)
// and z -= mag*cos(angle), then tail into the party-info processor. The cos
// argument repeats the product so MWCC recomputes it after the sin call
// (retail reloads the constant instead of spilling the offset angle). mag is
// declared before t so the saved-FPR claim order lands mag in f31 / t in
// f30 (retail fsubs f30 then fmuls f31), while t is computed first.
void func_80198EE8(CfPartsMoveSrc* src, CfPartsTri* dst) {
    u32 w0 = src->field_00;
    u32 w1 = src->field_04;
    dst->field_04 = w1;
    dst->field_00 = w0;
    dst->field_08 = src->field_08;
    f32 mag;
    f32 t = src->field_18 - lbl_eu_8066A204;
    mag = (f32)(s32)src->field_14 * src->field_1C;
    *(f32*)&dst->field_00 -= mag * nw4r::math::SinFIdx(lbl_eu_80667B50 * t);
    *(f32*)&dst->field_08 -= mag * nw4r::math::CosFIdx(lbl_eu_80667B50 * t);
    func_801987A4((CfPartsTri*)src, dst);
}

// Rotate helper with a parity-dependent turn offset: copy the triple, then
// compute mag = ((f32)(s32)((field_14 - 1) >> 1)) * field_1C + field_1C and
// rotate x/z by +/-lbl_eu_8066A204 depending on the parity of (field_14 - 1)
// (even -> -offset, odd -> +offset), then tail into the party-info processor.
extern "C" void func_80198FC4(CfPartsMoveSrc* src, CfPartsTri* dst) {
    u32 w0 = src->field_00;
    u32 w1 = src->field_04;
    dst->field_04 = w1;
    dst->field_00 = w0;
    dst->field_08 = src->field_08;
    s32 n = src->field_14 - 1;
    f32 t = (f32)(n >> 1);
    f32 mag = t * src->field_1C + src->field_1C;
    if (n & 1) {
        f32 angle = src->field_18 + lbl_eu_8066A204;
        *(f32*)&dst->field_00 -= mag * nw4r::math::SinFIdx(lbl_eu_80667B50 * angle);
        *(f32*)&dst->field_08 -= mag * nw4r::math::CosFIdx(lbl_eu_80667B50 * angle);
    } else {
        f32 angle = src->field_18 - lbl_eu_8066A204;
        *(f32*)&dst->field_00 -= mag * nw4r::math::SinFIdx(lbl_eu_80667B50 * angle);
        *(f32*)&dst->field_08 -= mag * nw4r::math::CosFIdx(lbl_eu_80667B50 * angle);
    }
    func_801987A4((CfPartsTri*)src, dst);
}

// Rotate helper with an odd-turn add pass: copy the triple, then for odd
// turn indices accumulate field_1C * sin/cos(scale * (field_18 + pi/2)) into
// the x/z accumulators, then always subtract
// mag = (f32)(s32)(field_14 >> 1) * field_1C times sin/cos(scale * field_18),
// add the accumulators into the destination words, and tail into the
// party-info processor. The cos argument repeats the product so MWCC
// recomputes it after the sin call (retail reloads the constant).
extern "C" void func_801990F0(CfPartsMoveSrc* src, CfPartsTri* dst) {
    u32 w0 = src->field_00;
    u32 w1 = src->field_04;
    dst->field_04 = w1;
    dst->field_00 = w0;
    dst->field_08 = src->field_08;
    f32 sz = lbl_eu_80667B28;  // declared first -> f31 (cos accumulator)
    f32 sx = sz;               // second -> f30 (sin accumulator, retail fmr)
    if (src->field_14 & 1) {
        f32 angle = src->field_18 + lbl_eu_8066A200;
        sx = sx + src->field_1C * nw4r::math::SinFIdx(lbl_eu_80667B50 * angle);
        sz = sz + src->field_1C * nw4r::math::CosFIdx(lbl_eu_80667B50 * angle);
    }
    f32 mag = (f32)(s32)(src->field_14 >> 1) * src->field_1C;
    sx = sx - mag * nw4r::math::SinFIdx(lbl_eu_80667B50 * src->field_18);
    sz = sz - mag * nw4r::math::CosFIdx(lbl_eu_80667B50 * src->field_18);
    *(f32*)&dst->field_00 += sx;
    *(f32*)&dst->field_08 += sz;
    func_801987A4((CfPartsTri*)src, dst);
}

// Turn-indexed rotation helper (CfPartsMoveSrc -> CfPartsTri, same family as
// func_80198D44): copy the triple, then split (field_14 - 1) into q/3 and
// the %3 remainder. Remainder 0/1 offset the turn angle by +/-lbl_eu_8066A204;
// remainder 2 normalizes the magnitude (nw4r FSqrt warning + FrSqrt path).
// The magnitude is then scaled by the (f32)(u32) quotient + 1 before the
// destination x/z are rotated by -mag*sin/cos(FIdx-scale * angle). The
// quotient/remainder divisions are re-derived from memory so MWCC emits the
// two separate mulhw sequences (the %3 stays in r0 across the else-if chain).
void func_8019922C(CfPartsMoveSrc* src, CfPartsTri* dst) {
    u32 w0 = src->field_00;
    u32 w1 = src->field_04;
    dst->field_04 = w1;
    dst->field_00 = w0;
    dst->field_08 = src->field_08;
    f32 ang = src->field_18;
    f32 mag = src->field_1C;
    if ((src->field_14 - 1) % 3 == 0) {
        ang -= lbl_eu_8066A204;
    } else if ((src->field_14 - 1) % 3 == 1) {
        ang += lbl_eu_8066A204;
    } else {
        f32 d = mag * mag + mag * mag;
        if (!(d >= lbl_eu_80667B28)) {
            nw4r::db::Warning(lbl_eu_80526324, 0x273, lbl_eu_80526300);
        }
        mag = d <= lbl_eu_80667B28 ? lbl_eu_80667B28
                                   : d * nw4r::math::FrSqrt(d);
    }
    mag = (f32)(u32)((src->field_14 - 1) / 3) * mag + mag;
    *(f32*)&dst->field_00 -= mag * nw4r::math::SinFIdx(lbl_eu_80667B50 * ang);
    *(f32*)&dst->field_08 -= mag * nw4r::math::CosFIdx(lbl_eu_80667B50 * ang);
    func_801987A4((CfPartsTri*)src, dst);
}
