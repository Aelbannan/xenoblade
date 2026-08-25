// Auto-scaffolded catalog TU for kyoshin/cf/CPartsChange
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/harness_catalog.hpp"

#include "monolib/util/reslist.hpp"
#include "monolib/util/CPathUtil.hpp"
#include "kyoshin/code_802B8A3C.hpp"
#include "kyoshin/cf/CfResPcImpl.hpp"
#include <nw4r/math/math_triangular.h>
#include "monolib/math/Random.hpp"


#include "kyoshin/cf/CPartsChange.hpp"

using cf::CfPartyInfo;
using cf::CfPartyInfoSortKey;
using cf::CfActorAccessors;
using cf::CfObjectPcExt;
using cf::CPartsChange;

// Same-TU helpers called by func_80196E04 whose retail symbols are
// unmangled: the early C-linkage declarations give the definitions below
// their retail symbol names (func_80198284 precedent). auto_inline off keeps
// the retail `bl` boundaries (MWCC would otherwise inline the tiny bodies).
#pragma push
#pragma auto_inline off
extern "C" CfElemA4Full* func_80193B0C(CfPartsManager* self, u16 arg2);
extern "C" int func_80198138(CfPartsSlots* self, CfPartsSlotChgView* other,
                              u32 unused, u32 flag);
extern "C" void func_80198524(CfPartsSlots* slots);
extern "C" int func_8019876C(CfPartyInfoState* self, CfPartsTri* goal);
#pragma pop

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
// Scan the vf24 entries for the formatted name; inlined by MWCC so the
// retail branch-over-branch (beq/b) layout is reproduced (btm_bda_to_acl
// pattern, MWCC_CASES line 840).
static inline int findNameEntry(CPartsChange* self, u32 count, const char* name) {
    for (int j = 0; (u32)j < count; j++) {
        if (strstr(self->mField08->vf24(j), name) != 0)
            return j;
    }
    return -1;
}

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
            // FixStr(false)'s ctor emits no stores, so the visible ctor
            // store pairs land in retail order: str's pair first, then
            // num's pair right before the itoa call.
            ml::FixStr<16> num(false);
            ml::FixStr<16> buf2;
            num.clear();
            ml::CPathUtil::itoa(num, (int)i, 2);
            buf2.format(lbl_eu_80503BFC, lbl_eu_806624E8, num.c_str());
            // Exhausted scans return -1 from the inlined helper; a hit
            // returns straight to the shared tail check.
            result = findNameEntry(self, count, buf2.c_str());
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
// clear the +0x42 sub-block. The u8->f32 cast uses the builtin stack
// conversion; its 2^52 magic pools to a TU-local @N entry (retail names it
// lbl_eu_80667AC8 - name-only reloc drift, MWCC_CASES 7i class; the manual
// union form that names it adds an fsub+frsp and breaks byte-identity).
#pragma push
#pragma auto_inline off
void func_801931D0(CfPartsElem4C* self) {
    self->field_1E &= ~4u;
    u32 val = getBdatStringColumnValue(lbl_eu_806640A8, lbl_eu_80503C48,
                                       lbl_eu_80664184);
    u32 b = *(const u8*)&val;
    if (b == 0) b = 1;
    f32 scale = lbl_eu_80667ABC * (f32)b;
    self->field_14 = lbl_eu_80667AC0;
    self->field_10 = lbl_eu_80667AB8 * scale;
    memset(&self->field_42[0], 0, 9);
}
#pragma pop

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
    if (self != 0) {
        if (self != 0) {
            self->vtable = lbl_eu_80532AE4;
            cur = self->mStartNodePtr->mNext;
            while (cur != self->mStartNodePtr) {
                prev = cur;
                cur = cur->mNext;
                prev->mNext = 0;
            }
            self->mStartNodePtr->mNext = self->mStartNodePtr;
            self->mStartNodePtr->mPrev = self->mStartNodePtr;
            if (self->mOwnsList == 0 && self->mList != 0) {
                delete[] self->mList;
                self->mList = 0;
            }
        }
        if (mode > 0) {
            delete self;
        }
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
    int h = self->field_456C; // load hoisted above the vtable call (retail)
    self->vt->_v0A8(1);
    u16 id = (u16)(h >> 4);
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
    CfPartyListNode* cur;
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
    cur = self->mPartyList.mStartNodePtr->mNext;
    for (; self->mPartyList.mStartNodePtr != cur;) {
        CfPartyListNode* prev = cur;
        cur = cur->mNext;
        prev->mNext = 0;
    }
    self->mPartyList.mStartNodePtr->mNext = self->mPartyList.mStartNodePtr;
    self->mPartyList.mStartNodePtr->mPrev = self->mPartyList.mStartNodePtr;
    memset(self->mTable, 0, 0xa40);
    // MWCC reverses adjacent u16 stores, so source order B270-then-B272
    // lands retail's B272-then-B270 store sequence.
    self->field_B270 = 1;
    self->field_B272 = 5;
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
// and link it at the tail of the party list. Every state store is spelled
// through self->mTable[i] so MWCC rematerializes the address from `self`
// and keeps only the i*0xA4 scale in a saved register (retail r27); the
// element pointer doubles as the result (retail r30, pre-initialized 0).
// Returns the initialized element, or null when all 16 are busy.
CfElemA4Full* func_80193B0C(CfPartsManager* self, u16 arg2) {
    CfElemFlagWalk* walk = (CfElemFlagWalk*)self;
    CfElemA4* result = 0;
    for (int i = 0; i < 16; i++) {
        if ((walk->field_A8C8 & 1) == 0) {
            result = &self->mTable[i];
            memset(result, 0, 0x80);
            self->mTable[i].field_A2 = 0;
            self->mTable[i].field_A0 = 0;
            self->mTable[i].field_90 = lbl_eu_80667AD4;
            self->mTable[i].field_94 = 0;
            self->mTable[i].field_98 = 0;
            self->mTable[i].field_9A = 0;
            self->mTable[i].field_94 = self->field_B270;
            self->mTable[i].field_A0 = 1;
            self->mTable[i].field_9E = arg2;
            u16 v = self->field_B270 + 1;
            self->field_B270 = v;
            if (v == 0) self->field_B270 = v + 1;
            int slot = 0;
            CfPartyListNode* head = self->mPartyList.mStartNodePtr;
            int cap = self->mPartyList.mCapacity;
            while (slot < cap) {
                if (self->mPartyList.mList[slot].mNext == 0) break;
                slot++;
            }
            CfPartyListNode* node = &self->mPartyList.mList[slot];
            try {
                u32* itemPtr = &node->field_08;
                if (itemPtr != 0) {
                    *itemPtr = (u32)result;
                }
                node->mNext = head;
                node->mPrev = head->mPrev;
                head->mPrev->mNext = node;
                head->mPrev = node;
            } catch (...) {
                throw;
            }
            break;
        }
        walk = (CfElemFlagWalk*)((char*)walk + sizeof(CfElemA4));
    }
    return (CfElemA4Full*)result;
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
    CfPartsNode* cur;
    u32 result = 0;
    u16 id = other->field_45C0;
    if (id != 0) {
        // head/data are loop-invariant member reads; writing them through
        // self/cur lets MWCC hoist them into the dead arg registers (r3/r4).
        for (cur = self->head->next; cur != self->head; cur = cur->next) {
            if (id == cur->data->field_94) {
                u32 p = (u32)func_800B708C(cur->data->field_00);
                if (p != 0) p -= 0x3E9C;
                result = p;
                break;
            }
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

// Gimmick/actor object in the func_800B6BC8 list (func_80194264): a real
// virtual class so MWCC emits the retail r12 vtable dispatch (cf::CfWalkObjAC
// pattern). Position getter at 0xAC, f32 query at 0xE0.
struct CfPartsElemObj {
    virtual void _v008() = 0; virtual void _v00C() = 0; virtual void _v010() = 0; virtual void _v014() = 0;
    virtual void _v018() = 0; virtual void _v01C() = 0; virtual void _v020() = 0; virtual void _v024() = 0;
    virtual void _v028() = 0; virtual void _v02C() = 0; virtual void _v030() = 0; virtual void _v034() = 0;
    virtual void _v038() = 0; virtual void _v03C() = 0; virtual void _v040() = 0; virtual void _v044() = 0;
    virtual void _v048() = 0; virtual void _v04C() = 0; virtual void _v050() = 0; virtual void _v054() = 0;
    virtual void _v058() = 0; virtual void _v05C() = 0; virtual void _v060() = 0; virtual void _v064() = 0;
    virtual void _v068() = 0; virtual void _v06C() = 0; virtual void _v070() = 0; virtual void _v074() = 0;
    virtual void _v078() = 0; virtual void _v07C() = 0; virtual void _v080() = 0; virtual void _v084() = 0;
    virtual void _v088() = 0; virtual void _v08C() = 0; virtual void _v090() = 0; virtual void _v094() = 0;
    virtual void _v098() = 0; virtual void _v09C() = 0; virtual void _v0A0() = 0; virtual void _v0A4() = 0;
    virtual void _v0A8() = 0;
    virtual void* vfAC() = 0;                // 0xAC (position)
    virtual void _v0B0() = 0; virtual void _v0B4() = 0; virtual void _v0B8() = 0; virtual void _v0BC() = 0;
    virtual void _v0C0() = 0; virtual void _v0C4() = 0; virtual void _v0C8() = 0; virtual void _v0CC() = 0;
    virtual void _v0D0() = 0; virtual void _v0D4() = 0; virtual void _v0D8() = 0; virtual void _v0DC() = 0;
    virtual f32 vfE0() = 0;                  // 0xE0 (float)
};

// Average position of the gimmick objects (func_800B6BC8 list) whose
// sphere-vs-sphere push-apart against `in` (func_800A5488 with the radius
// args f / vfE0) reports a hit; the accumulated tmp position is added into
// out. With no hits the input position is copied verbatim. The average is
// computed as 1/count (2^52 u32->f32 conversion) times each component, and
// the return value is (count > 0).
int func_80194264(f32 f, ml::CVec3* out, const ml::CVec3* in) {
    *out = ml::CVec3::zero;
    int count = 0;
    F64Conv conv;
    ml::CVec3 push;
    CfActorList* list = (CfActorList*)func_800B6BC8();
    CfActorListNode* node = list->mHead->mNext;
    // The sentinel is re-read from the list each pass (the virtual calls
    // clobber memory, so MWCC cannot cache it).
    while (node != list->mHead) {
        CfPartsElemObj* elem = (CfPartsElemObj*)node->mElem;
        node = node->mNext;
        f32 radius = elem->vfE0();
        ml::CVec3* pos = (ml::CVec3*)elem->vfAC();
        if (func_800A5488(*in, *pos, &push, f, radius)) {
            nw4r::math::VEC3Add(*out, *out, push);
            count++;
        }
    }
    if (count > 0) {
        // Named-magic u32->f32 conversion: assembling the 0x43300000-biased
        // double and subtracting the named sdata2 magic keeps the reloc on
        // lbl_eu_80667AF0 (CSelShopWin convention).
        conv.w[1] = count ^ 0x80000000;
        conv.w[0] = 0x43300000;
        nw4r::math::VEC3Scale(*out, *out,
                              lbl_eu_80667AD0 / (f32)(conv.d - lbl_eu_80667AF0));
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
    ml::CVec3 tmp8;
    ml::CVec3 tmp14;
    F64Conv c1, c2;
    f64 magic = lbl_eu_80667AF0;
    f32 c26 = lbl_eu_8066A210;
    f32 f30 = lbl_eu_80667AFC * c26;
    f32 c25 = lbl_eu_80667AEC;
    f32 c27 = lbl_eu_80667AF8;
    const f32* tbl = lbl_eu_80532AB8;
    int i = 0;
    u32 rnd = 0;
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
        *out = *in;
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
    CfPlayerVtACIf* player = (CfPlayerVtACIf*)getPlayer__Q22cf13CfGameManagerFi(0);
    int ok = func_800B99BC(player->vfAC(), &local14, 1, (src->field_1E >> 6) & 1,
                           &local18, &local8, lbl_eu_80667B08 + mgr->field_B268);
    if (src->field_20 & 0x10) {
        ok = 1;
    } else if (src->field_20 & 0x40) {
        f32 thresh = lbl_eu_80667B0C * lbl_eu_80663ED0;
        ml::CVec3* p2 = player->vfAC();
        ok = ml::math::abs(p2->y - local14.y) < thresh;
    }
    if (!ok) return 0;
    CfPartsChgObj3F04* obj = (CfPartsChgObj3F04*)func_80081A40__Q22cf13CfGameManagerFv(
        lbl_eu_80503C48 + 0x58, arg2, arg3, arg4);
    if (obj != 0) obj = (CfPartsChgObj3F04*)((u8*)obj - 0x3E9C);
    if (obj != 0) {
        if (bval != 0) {
            obj->mSub.vfB8(&local14, lbl_eu_80667B10);
        } else {
            obj->field_3F00 |= 0x00080000;
            obj->mSub.vf150(1);
            obj->mSub.vfA8(&local14);
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
        obj->mSub.vfD0((f32)(s32)r28);
        obj->field_456C = (u16)(((u16)arg3 << 4) + arg4);
        u32 v3 = getBdatStringColumnValue(bdat, lbl_eu_80503C48 + 0x64, arg3);
        obj->field_45C2 = *(const u16*)&v3;
    }
    return obj;
}

// Stage/flag gate used by func_80194D5C: stage 0 always passes. Otherwise
// the presentation mode from func_8016E08C must match the flag-selected
// expectation (bit 0x40 -> mode 0, bit 0x10 -> mode 1, bit 0x20 -> mode 2),
// and then the low flag nibble must contain the bit matching the stage
// (stage n -> bit 1 << (n-1)); an empty nibble passes unconditionally.
extern "C" int func_801949E0(u32 flags, u8 stage) {
    if (stage == 0) return 1;
    u32 mode = func_8016E08C();
    int ok = 0;
    if (flags & 0x40) {
        if ((u16)mode == 0) ok = 1;
    } else if (flags & 0x10) {
        if ((u16)mode == 1) ok = 1;
    } else if (flags & 0x20) {
        if ((u16)mode == 2) ok = 1;
    }
    if (ok == 0) return 0;
    if ((flags & 0xF) == 0) return 1;
    if ((flags & 1) && stage == 1) return 1;
    if ((flags & 2) && stage == 2) return 1;
    if ((flags & 4) && stage == 3) return 1;
    if ((flags & 8) && stage == 4) return 1;
    return 0;
}

// Party-drop scan: walk the actor list (func_800B6BC8) and for each element
// whose +0x6D0 id (shifted right 4) is nonzero, verify the party-count
// range from the BDAT columns (+0x6C/+0x74 level gates, +0x80/+0x8A bounds)
// against the CfGameManager counters. Failing the bounds sets the +0x40
// busy bit; passing them re-checks the +0x20 flag and, when clear, asks the
// actor's arts-state object (func_8016FE34 + sub-slot 0x30) whether to set
// the +0x60 bit. Returns whether any element was processed.
int func_80194AFC() {
    CfActorList* alist;
    u32 bdat;
    int result;
    CfActorListNode* node;
    alist = (CfActorList*)func_800B6BC8();
    if (alist->mHead->mNext == alist->mHead) return 0;
    bdat = func_80086B24__Q22cf13CfGameManagerFv();
    result = 0;
    node = alist->mHead->mNext;
    while (node != alist->mHead) {
        CfPartyScanElem* elem = (CfPartyScanElem*)node->mElem;
        s32 shifted;
        // Inline shift-in-condition so MWCC fuses to the record-form srawi.
        if ((shifted = (s32)elem->field_6D0 >> 4) == 0) {
            node = node->mNext;
            continue;
        }
            // Narrow re-reads of the column values go through memory so MWCC
            // emits the retail stw+lhz/lbz round-trips.
            u16 id = (u16)shifted;
            u32 val = getBdatStringColumnValue((void*)bdat, lbl_eu_80503C48 + 0x6c, id);
            u16 lower = 1;
            u16 upper = 0xfc;
            u32 cnt = func_80082354__Q22cf13CfGameManagerFv(*(const u16*)&val);
            if (cnt != 0) {
                u32 lim = getBdatStringColumnValue((void*)bdat, lbl_eu_80503C48 + 0x74, id);
                u8 limByte = *(const u8*)&lim;
                if (limByte != 0) {
                    lower = limByte;
                    upper = limByte;
                }
            }
            // Party-count gate: pass when the slot has no count column or
            // the count sits inside [lower, upper].
            int ok1;
            if (*(const u16*)&val == 0) {
                ok1 = 1;
            } else if ((u32)lower > cnt) {
                ok1 = 0;
            } else {
                ok1 = cnt <= (u32)upper;
            }
            // Retail evaluates this bounds check twice (two independent
            // variable sets); keep them separate so each gets its own
            // registers/spill slots.
            int ok2 = 0;
            if (ok1) {
                u16 curCount = (u16)func_800822F4__Q22cf13CfGameManagerFv();
                u32 colMin = getBdatStringColumnValue((void*)bdat, lbl_eu_80503C48 + 0x80, id);
                u32 colMax = getBdatStringColumnValue((void*)bdat, lbl_eu_80503C48 + 0x8a, id);
                if (*(const u16*)&colMin <= curCount && curCount <= *(const u16*)&colMax)
                    ok2 = 1;
            }
            if (!ok2) {
                u16 idB = (u16)shifted;
                u16 curCountB = (u16)func_800822F4__Q22cf13CfGameManagerFv();
                u32 colMinB = getBdatStringColumnValue((void*)bdat, lbl_eu_80503C48 + 0x80, idB);
                u32 colMaxB = getBdatStringColumnValue((void*)bdat, lbl_eu_80503C48 + 0x8a, idB);
                if (*(const u16*)&colMinB <= curCountB && curCountB <= *(const u16*)&colMaxB) {
                    if (!(elem->field_68 & 0x20)) {
                        void* obj = func_8016FE34(elem);
                        void* sub = *(void**)((u8*)obj + 4);
                        u32 word = ((CfVt30If*)sub)->vf30()->field_00;
                        if (func_80174C98(obj, (int*)&word, 0x803) == 0) {
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
    f32 fv = f;
    u32 r28 = func_8006A6D0();
    void* bdat = (void*)func_80086B24__Q22cf13CfGameManagerFv();
    getPlayer__Q22cf13CfGameManagerFi(0);
    u32 w = lbl_eu_80663E28;
    f32 f26 = f * f;
    const char* cols = lbl_eu_80503C48;
    mgr->field_B272 = (u16)r28;
    f32 f27 = lbl_eu_80667B14;
    mgr->field_A804 = 0;
    int r29 = (w >> 4) & 1;
    f32 f28 = lbl_eu_80667AD0;
    f32 f29 = lbl_eu_80667B0C;
    f32 f30 = lbl_eu_80667B18;
    f32 f31 = f;
    float local20;
    CfCollideOut local8;
    for (CfPartsElem4C* e = mgr->mElems.mElems; e != &mgr->mElems.mElems[mgr->mElems.mCount]; e++) {
        u8 b = e->field_24;
        f32 f1 = fv;
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
            f1 = ml::math::abs(fv - e->field_0C);
        } else if ((b2 & 2) && (e->field_20 & 1) && fv > f27) {
            f1 = fv - f27;
        }
        u16 field20 = e->field_20;
        int r24 = 0;
        if ((field20 & 4) && r29 == 0 && b48 == 0) r24 = 1;
        if (r24) {
            e->field_1E |= 0x40;
        } else {
            e->field_1E &= ~0x40u;
        }
        e->field_18 = f1;
        int ret = func_800B998C((void*)pos, e, 1, r24, &local20, &local8);
        int ok = (ret != 0) && (local8.data[0] == 0);
        // Retail only applies the height fallback inside the bit-0x10 case.
        if (field20 & 0x10) {
            if (ok) {
                local20 = f28;
                ok = 1;
            } else if (field20 & 0x40) {
                local20 = f28;
                ok = ml::math::abs(pos->y - e->mPos.y) < f29 * lbl_eu_80663ED0;
            }
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
            if (w74b) {
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
        if ((e->field_1E & 0x4) != 0) continue;
        if (!(e->field_10 > lbl_eu_80667AD4)) continue;
        // Volatile reference forces a fresh halfword load here (the retail
        // code re-reads the flag word instead of reusing the value from the
        // first bit test).
        volatile u16& curFlags = e->field_1E;
        if ((curFlags & 0x800) != 0) continue;
        // Decay the +0x10 speed; once it reaches the fallback: clamp it,
        // set the re-arm flag when both gate bits are present, and clear
        // the +0x30 sub-block.
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
#pragma push
#pragma auto_inline off
u8 func_80195384(int index, int unused, int unused2, CfActorAccessors* actor) {
    u8 result = 1;
    if (actor->mFlags1E & 0x20) {
        u32 val = getBdatStringColumnValue(lbl_eu_806640CC, lbl_eu_80503C48 + 0x40, index);
        u8 v = *(const u8*)&val;
        if (v > 3) result = 0;
    }
    return result;
}
#pragma pop

// Party-element update helper called by func_80195B04 (retail `bl` kept
// opaque with the same auto_inline-off pattern as func_80197538; the C
// linkage from the declaration above makes the call reloc name the retail
// symbol).
// Party-element update helper called by func_80195B04 / func_80195BD4 (retail
// `bl` kept opaque with the same auto_inline-off pattern; the C linkage from
// the declaration above makes the call reloc name the retail symbol). The
// retail body returns a success flag (callers branch on r3).
// Forward declarations for same-TU helpers defined below.
int func_80198340(CfPartsListEntry* list);
void func_80197AA0(CfPartsManager* self, u32 id);

// Party-element refresh: resolve/allocate the actor-table entry for the
// element, read its BDAT spawn columns, and roll the dice to (re)spawn the
// party-change objects for each of the five character slots. Two paths:
// bit 3 of +0x1E selects the "full refresh" path (speed reset, table entry
// update, per-slot spawn with effect/sound side effects) and the fallback
// path (count-limited random spawns only). *flag receives whether anything
// spawned; the return value is the actor-table entry handle.
#pragma push
#pragma auto_inline off
extern "C" int func_801953E8(CfPartsElemArray* arr, CfPartsElem4C* elem, u8* flag, int arg4) {
    CfElemA4Full* handle = 0;
    u32 bdat = func_80086B24__Q22cf13CfGameManagerFv();
    u32 spawned = 0;
    (void)func_8006A6D0();
    *flag = 0;
    if (elem->field_1E & 0x10) return 0;
    u32 id = elem->field_1C;
    if ((id & 1) == 0 && arg4 != 0) return 0;
    // Column +0x9d lookup result is discarded (write-side effect only).
    getBdatStringColumnValue((void*)bdat, lbl_eu_80503C48 + 0x9d, id);
    if (((CfActorAccessors*)elem)->func_80195AC0() != 0) {
        // Full-refresh path.
        if (elem->field_10 > lbl_eu_80667AD4) return 0;
        u32 v34 = getBdatStringColumnValue((void*)bdat, lbl_eu_80503C48 + 0x34, id);
        u8 col34 = *(u8*)&v34;
        u32 vA7 = getBdatStringColumnValue((void*)bdat, lbl_eu_80503C48 + 0xa7, id);
        // Plain u32->f64 conversion: MWCC builds the 0x43300000-prefixed
        // bit pattern in memory and subtracts its 2^52 magic (pooled under
        // the retail name lbl_eu_80667AC8). Do NOT spell the magic
        // subtraction explicitly - that duplicates the conversion.
        // The spawn speed falls back to the shared default when it matches
        // the column value.
        double spd = (double)(u32)*(u8*)&vA7;
        if (spd == lbl_eu_80667AD4) spd = lbl_eu_80667B04;
        if (elem->field_22 != 0)
            handle = (CfElemA4Full*)func_80193AB0((CfTableA4*)arr, elem->field_22);
        else
            handle = func_80193B0C((CfPartsManager*)arr, id);
        if (handle == 0) return 0;
        elem->field_22 = ((CfActorAccessors*)handle)->func_80193B04();
        if (elem->field_14 <= lbl_eu_80667AD4) {
            if (((CfActorAccessors*)elem)->func_80195284() != 0)
                elem->field_14 = lbl_eu_80667AC0;
            else
                memset(elem->field_30, 0, 9);
        }
        for (int i = 1; i <= 5; i++) {
            // Build the per-slot column names by patching the digit in the
            // shared 4-byte name buffers.
            lbl_eu_80662500[3] = '0' + i;
            u32 colId = getBdatStringColumnValue((void*)bdat, lbl_eu_80662500, id);
            if (*(u16*)&colId == 0) break;
            lbl_eu_80662510[3] = '0' + i;
            u32 cnt = getBdatStringColumnValue((void*)bdat, lbl_eu_80662510, id);
            int limit = *(u8*)&cnt - (elem->field_27[i] + elem->field_30[i]);
            for (int j = 0; j < limit; j++) {
                lbl_eu_80662508[3] = '0' + i;
                u32 vB = getBdatStringColumnValue((void*)bdat, lbl_eu_80662508, id);
                int force = 0;
                u32 altId = 0;
                u8 pct = *(u8*)&vB;
                if (i == 1) {
                    // Slot 1 consults the extra resource column: a live
                    // resource forces the spawn chance to 100% (or forces the
                    // reset helper when the roll fails).
                    u32 vB2 = getBdatStringColumnValue((void*)bdat, lbl_eu_80503C48 + 0xb2, id);
                    u8 rid = *(u8*)&vB2;
                    if (rid != 0) {
                        if (func_80195290((CfResSlot*)rid)) {
                            altId = rid;
                            pct = 100;
                        } else {
                            force = 1;
                        }
                    }
                }
                if ((s32)ml::math::mtRand(100) < (s32)pct) {
                    if (func_80195384(*(u16*)&colId, id, i, (CfActorAccessors*)elem) != 0) {
                        CfPartsChgObj3F04* obj = func_80194610((CfPartsManager*)arr, *(u16*)&colId, id, i, elem);
                        if (obj != 0) {
                            spawned = 1;
                            func_80198138((CfPartsSlots*)handle, (CfPartsSlotChgView*)obj, id, i);
                            ((CfActorAccessors*)handle)->func_80195ACC((float)spd);
                            ((CfObjectPcExt*)obj)->func_80195AD4(col34);
                            ((CfObjectPcExt*)obj)->func_80195ADC(altId);
                            ++elem->field_27[i];
                            ((CfActorAccessors*)elem)->func_80193A88(1);
                            elem->field_42[i] = elem->field_27[i];
                            if (elem->field_39[i] != 0) elem->field_39[i]--;
                            if ((elem->field_24 & 0x40) != 0 || col34 != 0) {
                                if (i > 1) {
                                    CfObjectPcExt* sub = (CfObjectPcExt*)&obj->mSub;
                                    CfPartsChgSub* subIf = &obj->mSub;
                                    f32 s1 = *(f32*)sub->func_80195AF4();
                                    f32 s2 = *(f32*)((CfActorAccessors*)sub)->func_80195AEC();
                                    f32 sum = s2 + s1;
                                    u32 dir = sub->func_80195AE4();
                                    f32 base = obj->vt->fn_0x5B4(obj);
                                    ml::CVec3 vec34;
                                    func_8004B79C(&vec34, subIf->vfAC());
                                    CfPartyInfo info;
                                    func_80198710(&info, &vec34, base, spd, col34, dir);
                                    if (func_8006DFBC(sub)) info.func_80195AFC(0);
                                    ml::CVec3 vec40;
                                    func_8004B0B0(&vec40);
                                    if (func_8019876C((CfPartyInfoState*)&info, (CfPartsTri*)&vec40) != 0) {
                                        if (func_8006DFBC(sub))
                                            subIf->vf9C((const f32*)&vec40);
                                        else
                                            subIf->vfB8(&vec40, lbl_eu_80667B1C);
                                    }
                                }
                            }
                        }
                    }
                } else {
                    if (force != 0) func_801931D0(elem);
                }
            }
        }
        if (func_80198340((CfPartsListEntry*)handle) != 0) {
            elem->field_1E |= 4;
            func_80198524((CfPartsSlots*)handle);
        } else {
            func_80197AA0((CfPartsManager*)arr, elem->field_22);
        }
    } else {
        // Fallback path: random spawns only, bounded by the remaining counts.
        if (elem->field_10 > lbl_eu_80667AD4) return 0;
        for (int i = 1; i <= 5; i++) {
            lbl_eu_80662500[3] = '0' + i;
            u32 colId = getBdatStringColumnValue((void*)bdat, lbl_eu_80662500, id);
            if (*(u16*)&colId == 0) break;
            u8 cnt27 = elem->field_27[i];
            lbl_eu_80662510[3] = '0' + i;
            u32 v14 = getBdatStringColumnValue((void*)bdat, lbl_eu_80662510, id);
            int limit = *(u8*)&v14 - elem->field_30[i];
            for (int j = 0; j < limit; j++) {
                if (cnt27 >= limit) break;
                u32 rnd = ml::math::mtRand(100);
                lbl_eu_80662508[3] = '0' + i;
                u32 v10 = getBdatStringColumnValue((void*)bdat, lbl_eu_80662508, id);
                int force = 0;
                u32 altId = 0;
                u8 pct = *(u8*)&v10;
                if (i == 1) {
                    u32 vc = getBdatStringColumnValue((void*)bdat, lbl_eu_80503C48 + 0xb2, id);
                    u8 rid = *(u8*)&vc;
                    if (rid != 0) {
                        if (func_80195290((CfResSlot*)rid)) {
                            altId = rid;
                            pct = 100;
                        } else {
                            force = 1;
                        }
                    }
                }
                if ((s32)rnd >= (s32)pct) {
                    if (force != 0) func_801931D0(elem);
                    continue;
                }
                if (func_80195384(*(u16*)&colId, id, i, (CfActorAccessors*)elem) == 0) continue;
                CfPartsChgObj3F04* obj = func_80194610((CfPartsManager*)arr, *(u16*)&colId, id, i, elem);
                if (obj == 0) continue;
                ((CfObjectPcExt*)obj)->func_80195ADC(altId);
                ++elem->field_27[i];
                ((CfActorAccessors*)elem)->func_80193A88(1);
                elem->field_42[i] = elem->field_27[i];
                if (elem->field_39[i] != 0) elem->field_39[i]--;
                spawned = 1;
            }
        }
        if (spawned != 0) elem->field_1E |= 4;
    }
    *flag = spawned;
    return (int)handle;
}
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
        if (found->field_27[1] != 0) { hasData = 1; }
        else if (found->field_27[2] != 0) { hasData = 1; }
        else if (found->field_27[3] != 0) { hasData = 1; }
        else if (found->field_27[4] != 0) { hasData = 1; }
        else if (found->field_27[5] != 0) { hasData = 1; }
        else if (found->field_27[6] != 0) { hasData = 1; }
        else if (found->field_27[7] != 0) { hasData = 1; }
        else if (found->field_27[8] != 0) { hasData = 1; }
        if (hasData == 0) {
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
        s16 retry = mgr->field_B276;
        if (retry > 0) mgr->field_B276 = 0;
    }
    if (mgr->field_B276 > 0) {
        mgr->field_B276 -= 1;
        mgr->field_B278 = 1;
        return;
    }
    if (func_80084BAC__Q22cf13CfGameManagerFv() != 0) return;
    // Two || mask terms straight off the global: branch context blocks the
    // mask fold, yielding two loads + rlwinm/rlwimi. record test.
    if ((lbl_eu_80663E24 & 0x20) != 0 || (lbl_eu_80663E24 & 0x400) != 0) return;
    u32 e24b = lbl_eu_80663E24;
    if ((e24b & 0x400000) != 0 && (lbl_eu_80663E28 & 0x60) == 0) return;
    if (func_800829B8__Q22cf13CfGameManagerFv() != 0) return;
    if (player == 0) return;
    func_8019514C(mgr);
    int r27 = (lbl_eu_80663E28 & 0x60) != 0;
    int bit23 = (lbl_eu_80663EE0 & 0x100) != 0;
    u32 sceneFlag = func_8006A6D0();
    u32 d90 = lbl_eu_80663D90;
    int bit10 = (lbl_eu_80663E28 & 0x10) != 0;
    int bit40 = (lbl_eu_80663E28 & 0x40) != 0;
    int unkBit;
    if (getUnk80664658() != 0) {
        unkBit = (getUnk80664658()->field_214 & 0x8000) != 0;
    } else {
        unkBit = 0;
    }
    int gt = lbl_eu_80663ED8 > lbl_eu_8066A208;
    int bit20 = (lbl_eu_80663E28 & 0x20) != 0;
    if (bit10 && bit40) {
        f = lbl_eu_80667B08;
    } else if (gt) {
        f = lbl_eu_80667ABC;
    } else if (unkBit) {
        f = f * lbl_eu_80667B20;
    } else if (bit10) {
        f = f * lbl_eu_80667B20;
    } else if (bit20 && bit40) {
        f = lbl_eu_80667ABC;
    } else if (bit20) {
        f = f * lbl_eu_80667B20;
    }
    int changed = mgr->field_B26C != f;
    mgr->field_B26C = f;
    // Skip the reset pass when nothing moved: speed unchanged, D90 low bits
    // clear, scene flag matches the stored slot count, and both gate bits 0.
    if (changed == 0 && (d90 & 3) == 0 && sceneFlag == mgr->field_B272 &&
        r27 == 0 && bit23 == 0) {
        goto afterLoop;
    }
    {
        u32 i = 0;
        CfElemA4Full* it = (CfElemA4Full*)mgr->mTable;
        for (; i < 0x10; i++, it++) {
            if (!(it->field_A0 & 1)) continue;
            if (it->field_A2 != 0) continue;
            u16 id = it->field_9E;
            CfPartsElem4C* elem;
            CfPartsManager* gm = (CfPartsManager*)lbl_eu_8066430C;
            if (id != 0) {
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
            it->field_94 = 0;
            it->field_A0 &= ~1u;
        }
    }
    {
        CfPlayerPosView* pv = (CfPlayerPosView*)player;
        ml::CVec3* pos = pv->vt->vfAC(player);
        func_80194D5C(mgr, pos, f);
    }
    if (r27 == 0 && bit23 == 0 && mgr->field_B278 == 0) return;
    mgr->field_B278 = 0;
afterLoop:
    func_80194AFC();
    if (sceneFlag != mgr->field_B272 && bit23 == 0) return;
    {
        CfActorList* list = (CfActorList*)func_800B6BC8();
        u32 n = 0;
        CfActorListNode* head = list->mHead;
        CfActorListNode* node = head->mNext;
        while (node != head) {
            node = node->mNext;
            n++;
        }
        if (n >= 0x20) return;
    }
    u32 cnt = mgr->field_A804;
    func_80196434((CfPartsSwapEntry*)mgr->mDist, (CfPartsSwapEntry*)&mgr->mDist[cnt],
                  (CfPartsSwapCmp)func_8019641C);
    func_80196E04(mgr, f);
    // Rebuild refreshes the distance array; reload the count.
    u32 count = mgr->field_A804;
    int hit = 0;
    int limit = 0x10;
    if (bit23 != 0) {
        mgr->field_B274 = 0;
        limit = (int)count;
        lbl_eu_80663EE0 &= ~0x100u;
    } else if (r27 != 0) {
        u32 half = count >> 1;
        if (half > 0x10) limit = (int)half;
    }
    const char* cols = lbl_eu_80503C48;
    for (u32 i = 0; i < count && i < (u32)limit; i++) {
        u16 idx = mgr->field_B274 + 1;
        mgr->field_B274 = idx;
        if (idx >= count) mgr->field_B274 = 0;
        idx = mgr->field_B274;
        CfPartsElem4C* elem = (CfPartsElem4C*)mgr->mDist[idx].elem;
        u16 eid = elem->field_1C;
        // Column value goes to a stack slot so the narrow re-read below is a
        // fresh halfword load (retail stw+lhz).
        u32 val = getBdatStringColumnValue(bdat, cols + 0x6c, eid);
        u16 lo = 1;
        u16 hi = 0xfc;
        u32 slotCnt = func_80082354__Q22cf13CfGameManagerFv(*(const u16*)&val);
        if (slotCnt != 0) {
            u32 w = getBdatStringColumnValue(bdat, cols + 0x74, eid);
            u8 wb = *(const u8*)&w;
            if (wb != 0) {
                hi = wb;
                lo = wb;
            }
        }
        int ok1;
        if (*(const u16*)&val == 0) {
            ok1 = 1;
        } else if ((u32)lo > slotCnt) {
            ok1 = 0;
        } else if (slotCnt <= (u32)hi) {
            ok1 = 1;
        } else {
            ok1 = 0;
        }
        int ok2 = 0;
        if (ok1) {
            u16 curCnt = (u16)func_800822F4__Q22cf13CfGameManagerFv();
            u32 colMin = getBdatStringColumnValue(bdat, cols + 0x80, eid);
            u32 colMax = getBdatStringColumnValue(bdat, cols + 0x8a, eid);
            if (*(const u16*)&colMin <= curCnt && curCnt <= *(const u16*)&colMax)
                ok2 = 1;
        }
        if (ok2 == 0) continue;
        if (elem->field_20 & 8) continue;
        // When re-arming (bit23), only elements with the 0x200 flag spawn.
        if (bit23 != 0 && !(elem->field_1E & 0x200)) continue;
        u8 flag;
        func_801953E8((CfPartsElemArray*)mgr, elem, &flag, 1);
        if (flag == 0) continue;
        hit++;
        if (r27 == 0) return;
        if (hit > 8) return;
    }
}

// 8-byte entry swap used by the three-way sort of func_80196C94 and the
// quicksorts (kept for func_80196864).
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
    // Taking the parameter's address homes it in a stack slot (retail
    // stw r5, 0x8(r1)); the slot doubles as the comparator object passed
    // to the helpers.
    for (;;) {
        // Element count as a signed byte-distance division (retail srawi/addze).
        s32 count = (s32)((u8*)b - (u8*)a) / 8;
        if (count <= 1) return;
        if (count <= 20) {
            CfPartsSwapEntry* i;
            for (i = a;; i++) {
                CfPartsSwapEntry* min = i;
                if (i != b) {
                    CfPartsSwapEntry* j;
                    for (j = i + 1; j != b; j++) {
                        if (cmp(j, min) != 0) min = j;
                    }
                }
                if (min != i) swapPartsEntry(i, min);
                if (i == b - 1) return;
            }
        }
        // Pseudo-random pivot offsets from the wrap-around state in [-4, 4];
        // offsets use state % 5 (MWCC 0x66666667 magic, single-shift idiom).
        s32 state = lbl_eu_80662518;
        s32 s = state + 1;
        s32 p1i = count / 4 + state % 5;
        s = (s >= 5) ? -4 : s;
        s32 p2i = (count * 3) / 4 + s % 5;
        s32 s2 = s + 1;
        lbl_eu_80662518 = (s2 >= 5) ? -4 : s2;
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
            // Everything equaled the pivot: swap it to the front and rotate
            // the equal run toward the back, shrinking [lo, pivot).
            pivot = b - 1;
            swapPartsEntry(a, pivot);
            lo = a + 1;
            lo = a + 1;
            if (cmp(a, lo) == 0) {
                while (lo != b && cmp(a, lo) == 0) lo++;
                if (lo < pivot) swapPartsEntry(lo, pivot);
            }
            while (lo < pivot) {
                while (cmp(a, lo) == 0) lo++;
                do {
                    pivot--;
                } while (cmp(a, pivot) != 0);
                if (lo >= pivot) break;
                swapPartsEntry(lo, pivot);
                lo++;
            }
        } else {
            // Tail-recurse on the smaller half.
            if ((s32)((u8*)lo - (u8*)a) < (s32)((u8*)b - (u8*)lo)) {
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
            CfPartsSwapEntry* i = a;
            // Selection sort; count >= 2 guarantees at least one iteration.
            do {
                CfPartsSwapEntry* j;
                CfPartsSwapEntry* min = i;
                if (i != b) {
                    for (j = i + 1; j != b; j++) {
                        if (f->cmp(j, min) != 0) min = j;
                    }
                }
                if (min != i) swapPartsEntry(i, min);
                i++;
            } while (i != b - 1);
            return;
        }
        // Pivot pick: two pseudo-random indices derived from the cycling
        // state global (-4..4); count/4 is computed once and 3*count/4 is
        // folded as (half<<2)-half. The state advances twice per call.
        s32 state = lbl_eu_8066251C;
        s32 half = count / 4;
        CfPartsSwapEntry* p1 = &a[half + state % 5];
        s32 s = state + 1;
        if (s >= 5) s = -4;
        s32 s2 = s + 1;
        lbl_eu_8066251C = (s2 < 5) ? s2 : -4;
        CfPartsSwapEntry* p2 = &a[4 * half - half + s % 5];
        CfPartsSwapEntry* pivot = b - 1;
        func_80196C94(p1, p2, pivot, f);
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

// Slot rebuild pass over the sorted distance array (called from
// func_80195E5C right after the sort): for each of the first 16 entries with
// the active bit, refresh the equipment counters and respawn the party
// members listed in the element's +0x39 counts. Each spawn re-keys the actor
// into a 16-slot busy table (allocating through func_80193B0C when free),
// rebuilds a CfPartyInfo via func_80198710 and picks a goal position through
// the pmf dispatcher func_8019876C. Afterwards, if any element spawned while
// its 0x10 flag was set, the element is rebound to a slot: when all eight of
// the slot's entries are free the slot is reset (func_80198524) and the
// element's re-arm bit is set.
void func_80196E04(CfPartsManager* mgr, f32 f) {
    // All state declared at function top (C-style); declaration order drives
    // MWCC's callee-saved register allocation.
    u32 bdat;
    CfPartsTailView* view;
    f64 convMagic;
    f32 fallback;
    u32 count;
    u32 i;
    CfPartsElem4C* elem;
    u16 id;
    u32 v34;
    u8 col34;
    u32 va7;
    u8 va7b;
    F64Conv conv;
    f64 speed;
    u16 flags;
    int k;
    int spawned;
    int j;
    s32 cnt;
    u32 colv;
    u16 colh;
    u8 resId;
    u32 vb2;
    int n;
    CfPartsChgObj3F04* obj;
    CfPartsSlots* slot;
    u16 key;
    CfElemA4Full* p;
    int t;
    u16 slotIdx;
    CfPartsChgSub* sub;
    ml::CVec3* srcPos;
    ml::CVec3 posLocal;
    cf::CfPartyInfo info;
    f32 f44;
    CfPartsTri goal;
    bool occupied;

    bdat = func_80086B24__Q22cf13CfGameManagerFv();
    func_8006A6D0();
    // Single tail view over the distance array + its live count.
    view = (CfPartsTailView*)(void*)mgr->mDist;
    convMagic = lbl_eu_80667AC8;
    count = view->count;
    fallback = lbl_eu_80667AD4;
    for (i = 0; i < count && i < 0x10; i++) {
        elem = (CfPartsElem4C*)view->mDist[i].elem;
        if (!(elem->field_1E & 1)) continue;
        id = elem->field_1C;
        // Column values are deref'd out of storage so MWCC emits the retail
        // stw+lbz round-trips instead of folding to rlwinm.
        v34 = getBdatStringColumnValue((void*)bdat, lbl_eu_80503C48 + 0x34, id);
        col34 = *(const u8*)&v34;
        va7 = getBdatStringColumnValue((void*)bdat, lbl_eu_80503C48 + 0xa7, id);
        va7b = *(const u8*)&va7;
        // Named-magic u32->f32 conversion; kept as double so MWCC schedules
        // the fcmpu before any frsp (retail rounds only at the stfs sites).
        conv.w[0] = 0x43300000;
        conv.w[1] = va7b;
        speed = conv.d - convMagic;
        if (fallback == speed) speed = lbl_eu_80667B04;
        flags = elem->field_1E;
        if ((flags & 1) && elem->field_14 <= fallback &&
            elem->field_10 <= fallback && !(flags & 0x400)) {
            // Fold each equipment counter into its accumulator and clear it.
            for (k = 1; k <= 8; k++) {
                if (elem->field_30[k] != 0) {
                    elem->field_39[k] += elem->field_30[k];
                    elem->field_30[k] = 0;
                }
            }
        }
        spawned = 0;
        for (j = 1; j <= 8; j++) {
            cnt = elem->field_39[j];
            if (cnt <= 0) continue;
            lbl_eu_80662520[3] = (char)(j + 0x30);
            colv = getBdatStringColumnValue((void*)bdat, lbl_eu_80662520, id);
            colh = *(const u16*)&colv;
            resId = 0;
            if (j == 1) {
                vb2 = getBdatStringColumnValue((void*)bdat, lbl_eu_80503C48 + 0xb2, id);
                resId = *(const u8*)&vb2;
                if (resId != 0 && func_8009CF8C(resId + 0x1d44) == 0) resId = 0;
            }
            for (n = 0; n < cnt; n++) {
                obj = func_80194610(mgr, colh, id, j, elem);
                if (obj == 0) continue;
                obj->field_45C8 = resId;
                spawned = 1;
                elem->field_27[j]++;
                elem->field_1E |= 0x400;
                elem->field_39[j]--;
                if (!(elem->field_1E & 0x10)) continue;
                // Re-key the actor into the busy-slot table: allocate a fresh
                // slot when unbound, otherwise look the key up.
                key = elem->field_22;
                if (key == 0) {
                    slot = (CfPartsSlots*)func_80193B0C(mgr, id);
                    elem->field_22 = slot->field_94;
                } else {
                    slot = 0;
                    p = (CfElemA4Full*)mgr->mTable;
                    for (t = 0; t < 16; t++, p++) {
                        if (key == p->field_94) {
                            slot = (CfPartsSlots*)p;
                            break;
                        }
                    }
                }
                if (slot == 0) continue;
                func_80198138(slot, (CfPartsSlotChgView*)obj, id, j);
                slot->field_8C = speed;
                obj->field_45C4 = col34;
                if ((elem->field_24 & 2) || col34 != 0) {
                    if (j > 1) {
                        slotIdx = obj->field_45C6;
                        sub = &obj->mSub;
                        srcPos = sub->vfAC();
                        posLocal.x = srcPos->x;
                        posLocal.y = srcPos->y;
                        posLocal.z = srcPos->z;
                        obj->vt->fn_0x5B4(obj);
                        f44 = obj->field_44D8;
                        func_80198710(&info, &posLocal, f44 + f44, speed,
                                      col34, slotIdx);
                        if (obj->field_3F00 & 8) info.field_2D = 0;
                        if (func_8019876C((CfPartyInfoState*)&info, &goal) != 0) {
                            if (obj->field_3F00 & 8) {
                                sub->vf9C((const f32*)&goal);
                            } else {
                                sub->vfB8((const ml::CVec3*)&goal, lbl_eu_80667B1C);
                            }
                        }
                    }
                }
            }
        }
        if (spawned == 0) continue;
        if (!(elem->field_1E & 0x10)) return;
        key = elem->field_22;
        if (key == 0) {
            slot = (CfPartsSlots*)func_80193B0C(mgr, id);
        } else {
            slot = 0;
            p = (CfElemA4Full*)mgr->mTable;
            for (t = 0; t < 16; t++, p++) {
                if (key == p->field_94) {
                    slot = (CfPartsSlots*)p;
                    break;
                }
            }
        }
        if (slot == 0) return;
        elem->field_22 = slot->field_94;
        // Only rebind when at least one of the first eight entries is live.
        occupied = false;
        for (k = 0; k < 8; k++) {
            if (slot->mEntries[k].field_00 != 0) {
                occupied = true;
                break;
            }
        }
        if (!occupied) return;
        func_80198524(slot);
        elem->field_1E |= 4;
        return;
    }
}

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
            for (u32 i = 0; i < self->mCount; p++, i++) {
                if (id == p->field_1C) return &self->mElems[i];
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
// Register-coloring notes: the two busy computations are separate variables
// (disjoint live ranges) so MWCC colors them independently; declaring them
// adjacently after `active` lands both on retail's r29. elem is declared
// first so it shares obj's r31 after obj dies.
#pragma push
#pragma auto_inline off
extern "C" void func_80197538(unsigned long manager, int arg) {
    CfPartsElem4C* elem;            // actor-half element
    CfPartsChgObjFull* obj;         // r31
    int active;                     // r30
    int busy;                       // inner-loop busy
    int busy2;                      // outer busy
    const CfPartsListEntry* p;      // r28 (mEntries walk)
    void* bm;                       // r27
    CfElemA4Full* tbl;              // r26
    int j;                          // r25
    CfPartsChgObjFull* actor;       // r24
    CfPartsManager* mgr;            // r23
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
            u32 id = actor->field_45C0;
            if (id != 0) {
                u32 i = 0;
                for (; i < 16; i++) {
                    if (id == mgr->mTable[i].field_94) {
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
    busy2 = 1;
    if (((cf::CfResPcPlayerVtIf*)actor)->_v2BC() == 0 && (actor->field_3F08 & 0x08000000) == 0) {
        busy2 = 0;
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
            if (busy2 != 0) {
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
// Party-slot removal / spawn dispatch. See the comment above the forward
// declarations for the full behavior description.
extern "C" void func_80197DE8(CfPartsManager* mgr, CfPartsChgObjFull* actor, int opt1, int opt2) {
    // Linear scan of the 16-entry 0xA4 table for the actor id (+0x94 key).
    CfElemA4* tbl;
    u16 id;
    int flag = 1;
    if (((cf::CfResPcPlayerVtIf*)actor)->_v2BC() == 0 && !(actor->field_3F08 & 0x08000000)) {
        flag = 0;
    }
    id = actor->field_45C0;
    tbl = 0;
    if (id != 0) {
        for (u32 i = 0; i < 16; i++) {
            if (id == mgr->mTable[i].field_94) {
                tbl = &mgr->mTable[i];
                break;
            }
        }
    }
    if (tbl != 0) {
        if (func_80198284((CfPartsSlots*)tbl, (CfPartsSlots*)actor) == 0) return;
        if (!(tbl->field_A0 & 1)) {
            // Element lookup duplicated inline (retail emits the identical
            // scan twice); the two elem = 0 sites mirror retail's separate
            // li blocks (bit-clear vs shifted-zero/exhausted-scan).
            CfPartsElem4C* elem;
            if (actor->field_3F00 & 0x4) {
                if (((s32)actor->field_456C >> 4) != 0) {
                    u16 sid = (u16)((s32)actor->field_456C >> 4);
                    u32 count = mgr->mElems.mCount;
                    for (u32 j = 0; j < count; j++) {
                        if (sid == mgr->mElems.mElems[j].field_1C) {
                            elem = &mgr->mElems.mElems[j];
                            goto merged1;
                        }
                    }
                }
                elem = 0;
            } else {
                elem = 0;
            }
        merged1:
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
                if (node->data == tbl) {
                    CfPartsDNode* prev = node->prev;
                    prev->next = next;
                    next->prev = prev;
                    node->next = 0;
                }
                node = next;
            }
            if (flag && opt1 == 0 && opt2 != 0) {
                u16 part = tbl->field_9A;
                u32 objId = actor->field_3F10;
                f32 f = actor->vt->fn_0x5B4(actor);
                ml::CVec3* pos =
                    (ml::CVec3*)((cf::CfResPcParentVtIf*)&actor->mSubVt)->_v0AC();
                func_80197C6C(f, objId, part, pos, 1);
            }
        }
    } else {
        if (id == 0 && flag && opt1 == 0 && opt2 != 0) {
            u16 part = actor->field_3F28;
            u32 objId = actor->field_3F10;
            f32 f = actor->vt->fn_0x5B4(actor);
            ml::CVec3* pos =
                (ml::CVec3*)((cf::CfResPcParentVtIf*)&actor->mSubVt)->_v0AC();
            func_80197C6C(f, objId, part, pos, 0);
        }
    }
    if (tbl == 0) {
        CfPartsElem4C* elem;
        if (actor->field_3F00 & 0x4) {
            if (((s32)actor->field_456C >> 4) != 0) {
                u16 sid = (u16)((s32)actor->field_456C >> 4);
                u32 count = mgr->mElems.mCount;
                for (u32 j = 0; j < count; j++) {
                    if (sid == mgr->mElems.mElems[j].field_1C) {
                        elem = &mgr->mElems.mElems[j];
                        goto merged2;
                    }
                }
            }
            elem = 0;
        } else {
            elem = 0;
        }
    merged2:
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
    const char* col = lbl_eu_80503C48 + 0x40;
    void* bdat = lbl_eu_806640CC;
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
#pragma push
#pragma auto_inline off
int func_8019876C(CfPartyInfoState* self, CfPartsTri* goal) {
    return (self->*lbl_eu_80532AF0[self->field_0C])(goal);
}
#pragma pop

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
    if (self->field_2D != 0) {
        ml::CVec3* dst = (ml::CVec3*)dstTri;
        // MWCC packs these in reverse declaration order: this yields the
        // retail sp+0x08(sum)/0x14(offset)/0x20(pos)/0x2c(res) layout.
        ml::CVec3 res;
        ml::CVec3 pos;
        ml::CVec3 offset;
        ml::CVec3 sum;
        offset.x = lbl_eu_80667B28;
        offset.y = lbl_eu_80667B2C;
        offset.z = lbl_eu_80667B28;
        nw4r::math::VEC3Add((nw4r::math::VEC3*)&sum, (const nw4r::math::VEC3*)dst,
                            (const nw4r::math::VEC3*)&offset);
    pos.x = sum.x;
    pos.y = sum.y;
    pos.z = sum.z;
    if (func_804BE398(&pos, 0x4a05, 0, 1, lbl_eu_80667B30 - self->field_28,
                      lbl_eu_80667B34) != 0) {
    // Homes mirror declaration order; assignments are ordered to match
    // the retail load/copy schedule.
    int bestIdx;
    int secondIdx;
    int i;
    f32 bestY, yThresh, secondDist, secondY, bound, hiBound, bestDist;
    secondDist = lbl_eu_80667B38;
    bestIdx = 0;
    secondY = dst->y;
    secondIdx = 0;
    bestDist = secondDist;
    bestY = secondY;
    bound = secondY - lbl_eu_80667B3C;
    yThresh = lbl_eu_80667B28;
    hiBound = lbl_eu_80667B40 + secondY;
    i = 0;
    while (i < (int)func_804BE4AC()) {
        ml::CVec3* obj = (ml::CVec3*)func_804BE520(i);
        if (obj->y > yThresh) {
            f32 dist = ml::math::abs(((ml::CVec3*)func_804BE50C(i))->y - dst->y);
            if (func_804BE5A4(0x40000, i) != 0) {
                if (bestDist >= dist) {
                    bestDist = dist;
                    bestY = ((ml::CVec3*)func_804BE50C(i))->y;
                    bestIdx = i + 1;
                }
            } else if (bound < ((ml::CVec3*)func_804BE50C(i))->y
                       && ((ml::CVec3*)func_804BE50C(i))->y <= hiBound) {
                secondDist = dist;
                secondY = ((ml::CVec3*)func_804BE50C(i))->y;
                secondIdx = i + 1;
                bound = secondY;
            } else if (secondDist >= dist) {
                secondDist = dist;
                secondY = ((ml::CVec3*)func_804BE50C(i))->y;
                secondIdx = i + 1;
                bound = secondY;
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
        } else if (bestDist < secondDist) {
            secondIdx = bestIdx;
            if (secondY - bestY <= lbl_eu_80667B44) {
                self->field_2C = 1;
                dst->y = bestY;
                self->field_24 = secondY;
            }
        }
    }
    if (self->field_2E != 0 && secondIdx != 0
        && func_804BE53C(&res, secondIdx - 1) != 0
        && lbl_eu_80667B48 <= res.x * res.x + res.z * res.z) {
        return 0;
    }
    return 1;
        } else {
            return 0;
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

// Rotate helper (parity-split turn offset, retail func_80198C24): copy the
// triple, then mag = (f32)(s32)((field_14 - 1) >> 1) * field_1C + field_1C.
// Odd field_14 adds mag*sin/cos(scale*(field_18 + pi/2)) onto x/z; even
// subtracts it; then tail into the party-info processor.
extern "C" void func_80198C24(CfPartsMoveSrc* src, CfPartsTri* dst) {
    u32 w0 = src->field_00;
    u32 w1 = src->field_04;
    dst->field_04 = w1;
    dst->field_00 = w0;
    dst->field_08 = src->field_08;
    f32 t = (f32)((src->field_14 - 1) >> 1);
    f32 mag = t * src->field_1C + src->field_1C;
    f32 angle = src->field_18 + lbl_eu_8066A200;
    if (src->field_14 & 1) {
        *(f32*)&dst->field_00 += mag * nw4r::math::SinFIdx(lbl_eu_80667B50 * angle);
        *(f32*)&dst->field_08 += mag * nw4r::math::CosFIdx(lbl_eu_80667B50 * angle);
    } else {
        *(f32*)&dst->field_00 -= mag * nw4r::math::SinFIdx(lbl_eu_80667B50 * angle);
        *(f32*)&dst->field_08 -= mag * nw4r::math::CosFIdx(lbl_eu_80667B50 * angle);
    }
    func_801987A4((CfPartsTri*)src, dst);
}

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
    f32 fv = (f32)(s32)src->field_14;
    f32 mag = fv * src->field_1C;
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
// Rotate helper with an odd-turn add pass: copy the triple, then for odd
// turn indices accumulate field_1C * sin/cos(scale * (field_18 + pi/2)) into
// the x/z accumulators, then always subtract
// mag = (f32)(s32)(field_14 >> 1) * field_1C times sin/cos(scale * field_18),
// add the accumulators into the destination words, and tail into the
// party-info processor. The cos argument repeats the product so MWCC
// recomputes it after the sin call (retail reloads the constant).
extern "C" void func_801990F0(CfPartsMoveSrc* src, CfPartsTri* dst) {
    f32 sz = lbl_eu_80667B28;
    f32 sx = sz;
    u32 w0 = src->field_00;
    u32 w1 = src->field_04;
    dst->field_04 = w1;
    dst->field_00 = w0;
    dst->field_08 = src->field_08;
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
    f32 mag;
    f32 ang = src->field_18;
    mag = src->field_1C;
    if ((src->field_14 - 1) % 3 == 0) {
        ang -= lbl_eu_8066A204;
    } else if ((src->field_14 - 1) % 3 == 1) {
        ang += lbl_eu_8066A204;
    } else {
        // d is re-evaluated inline at each use (retail recomputes mag*mag+mag*mag
        // before the warning check, the clamp test, and inside the FrSqrt arm).
        if (!(mag * mag + mag * mag >= lbl_eu_80667B28)) {
            nw4r::db::Warning(lbl_eu_80526324, 0x273, lbl_eu_80526300);
        }
        mag = mag * mag + mag * mag <= lbl_eu_80667B28
                  ? lbl_eu_80667B28
                  : (mag * mag + mag * mag) * nw4r::math::FrSqrt(mag * mag + mag * mag);
    }
    mag = (f32)((src->field_14 - 1) / 3) * mag + mag;
    *(f32*)&dst->field_00 -= mag * nw4r::math::SinFIdx(lbl_eu_80667B50 * ang);
    *(f32*)&dst->field_08 -= mag * nw4r::math::CosFIdx(lbl_eu_80667B50 * ang);
    func_801987A4((CfPartsTri*)src, dst);
}
