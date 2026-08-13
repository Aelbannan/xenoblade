// Auto-scaffolded catalog TU for kyoshin/cf/CPartsChange
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/harness_catalog.hpp"

#include "monolib/util/reslist.hpp"
#include "kyoshin/code_802B8A3C.hpp"
#include <nw4r/math/math_triangular.h>

extern unsigned long lbl_eu_8066430C;
extern unsigned long lbl_eu_80664308;

#include "kyoshin/cf/CPartsChange.hpp"

using cf::CfPartyInfo;
using cf::CfPartyInfoSortKey;
using cf::CfActorAccessors;
using cf::CfObjectPcExt;

// s32->f32 conversion magic (2^52 + 2^31) for func_80198D44's `(f32)(s32)`
// cast: MWCC's constant pool reuses this named .sdata2 symbol instead of a
// TU-local @N label (CfResReloadImpl.cpp / CMenuArtsSelect.cpp idiom).
extern const double lbl_eu_80667B58 = 0x4330000080000000ll;

// Retail symbols for the same-TU helpers below are unmangled (the retail
// build compiled them with C linkage). Declaring them extern "C" here gives
// the definitions C linkage, so call relocs name the retail symbols instead
// of the mangled C++ forms (PLAN.md §17.6 reloc-name drift; the hexdiff-/
// harness-approved fix). func_80193CC8's real body stays the mangled member
// below; this free declaration exists only to emit the retail reloc name.
extern "C" CfElemA4* func_80193AB0(CfTableA4* self, u32 id);
extern "C" u16 func_80193CC8(const CfElemA4* self);
extern "C" void func_80193C74(CfPartState90* self);
extern "C" CfPartsElem4C* func_8019747C(CfPartsElemArray* self, u32 id);
extern "C" u32 func_80197B28(const reslist<cf::CfPartyInfo>* self);
extern "C" void func_801986CC(reslist<cf::CfPartyInfo>* self, const CfPartyInfo* item);

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

void func_80192E80(){}

struct PartsChangeIf {
    virtual void _v008(); virtual void _v00C(); virtual void _v010(); virtual void _v014();
    virtual void _v018(); virtual void _v01C(); virtual void _v020(); virtual void _v024();
    virtual void _v028(); virtual void _v02C(); virtual void vf30(u8 a, u32 b);
    virtual void _v034(); virtual int vf34();
};
extern "C" void func_80192F94(u8* self) {
    if (!((u8*)self)[0x2c]) return;
    if (!*(void**)((u8*)self + 8)) return;
    if (((u8*)self)[4]) return;
    int hit = 0;
    for (int i = 0; i < 0x20; i++) {
        u8 c = ((u8*)self)[0xc + i];
        if (c == 0xff) continue;
        if (hit) {
            ((PartsChangeIf*)*(void**)((u8*)self + 8))->vf30(c, 0);
        } else {
            if (((PartsChangeIf*)*(void**)((u8*)self + 8))->vf34()) hit = 1;
        }
    }
    if (hit) return;
    for (int i = 0; i < 0x20; i++) {
        if (((u8*)self)[0xc + i] != 0xff) {
            ((PartsChangeIf*)*(void**)((u8*)self + 8))->vf30(((u8*)self)[0xc + i], 1);
            return;
        }
    }
}

void func_801930A0(){}

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

void __ct__80193270(){}

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
    if (self != NULL) {
        self->vtable = lbl_eu_80532AE4;
        {
            // Explicit birth order: cur declared before prev so MWCC
            // allocates cur to the lower scratch register (retail r4).
            CfPartyListNode* cur = self->mStartNodePtr->mNext;
            CfPartyListNode* prev;
            while (cur != self->mStartNodePtr) {
                prev = cur;
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

void __dt__80193538(){}

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

void func_80193710(){}

extern "C" u32 func_80193804() { return (u32)lbl_eu_80664308; }

extern "C" void func_80193810(u32 self);
extern "C" void func_8019380C(u32 self) { func_80193810(self); }

#pragma push
#pragma auto_inline off
extern "C" void func_80193810(u32 self){}
#pragma pop

void func_8019397C(){}

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

void func_80193B0C(){}

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

void func_80193D48(){}

void func_80194264(){}

void func_801943E4(){}

void func_80194610(){}

void func_801949E0(){}

void func_80194AFC(){}

void func_80194D5C(){}

void func_8019514C(){}

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
// the global presentation-flag word (bit 0x200000) and the id.
extern "C" void func_801952CC(int id) {
    if (id == 0) return;
    if (lbl_eu_80663E24 & 0x200000) return;
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
// retail stw+lbz round-trip (MWCC_REFERENCE: cast would fold to rlwinm).
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

void func_801953E8(){}

u32 CfActorAccessors::func_80195AC0() { return (mFlags1E >> 3) & 0x1u; }

void CfActorAccessors::func_80195ACC(float val) { mField8C = val; }

void CfObjectPcExt::func_80195AD4(u16 val) { mField45C4 = val; }

void CfObjectPcExt::func_80195ADC(u16 val) { mField45C8 = val; }

u16 CfObjectPcExt::func_80195AE4() { return mField45C6; }

void* CfActorAccessors::func_80195AEC() { return &mField30; }

void* CfObjectPcExt::func_80195AF4() { return (void*)((u8*)this + 0x60c); }

void CfPartyInfo::func_80195AFC(u8 val) { field_2D = val; }

void func_80195B04(){}

void func_80195BD4(){}

void func_80195E5C(){}

bool func_8019641C(const CfPartyInfoSortKey* a, const CfPartyInfoSortKey* b) {
    return a->sortKey < b->sortKey;
}

void func_80196434(){}

void func_80196864(){}

void func_80196C94(){}

void func_80196E04(){}

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

// Stub bodies are inlined into func_80197B4C by MWCC (they are defined in
// this TU), which would delete the retail `bl` calls - keep them opaque with
// the same auto_inline-off pattern used for func_80193810 below.
#pragma push
#pragma auto_inline off
extern "C" void func_80197538(unsigned long manager, int arg) {}
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
extern "C" void func_80197DE8(unsigned long manager, int arg, int opt1, int opt2);

// Notify the global party-info manager (lbl_eu_8066430C) about an actor
// change: refresh the entry, then re-evaluate it with both option args 0.
void func_80197B4C(int arg) {
    if (arg == 0) return;
    if (lbl_eu_8066430C == 0) return;
    func_80197538(lbl_eu_8066430C, arg);
    func_80197DE8(lbl_eu_8066430C, arg, 0, 0);
}

// Party-change entry point from CfObjectPc: when the actor's +0x2BC virtual
// (busy flag) or the +0x3F08 flags word indicates activity, re-arm the
// +0x45C8 id through func_801952CC; then refresh and re-evaluate the actor in
// the global party-info manager.
extern "C" void func_80197BA4(CfPartsChangeObj* self, int opt1, int opt2) {
    if (self == 0) return;
    if (lbl_eu_8066430C == 0) return;
    int busy = ((CfPartsVt2BCTable*)((CfPartsVtPtr*)self)->vt)->fn2BC(self);
    int flag = 0;
    if (busy != 0 || (self->field_3F08 & 0x08000000) != 0) flag = 1;
    if (flag != 0) {
        if (self->field_45C8 != 0) {
            func_801952CC(self->field_45C8);
            self->field_45C8 = 0;
        }
    }
    func_80197538(lbl_eu_8066430C, (int)self);
    func_80197DE8(lbl_eu_8066430C, (int)self, opt1, opt2);
}

void func_80197C6C(){}

extern "C" void func_80197DE8(unsigned long manager, int arg, int opt1, int opt2);

#pragma push
#pragma auto_inline off
extern "C" void func_80197DE8(unsigned long manager, int arg, int opt1, int opt2) {}
#pragma pop
void func_80198108(int arg) {
    if (lbl_eu_8066430C == 0) return;
    unsigned char* p = (unsigned char*)lbl_eu_8066430C + 0x10000;
    *(unsigned short*)(p - 0x4d88) = (unsigned short)arg;
    if (arg == 0) return;
    unsigned char* p2 = (unsigned char*)lbl_eu_8066430C + 0x10000;
    *(unsigned short*)(p2 - 0x4d8a) = 0;
}

void func_80198138(){}

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

// Scan 16 8-byte slots in two 8-slot batches for any non-zero first word;
// returns 1 on the first hit, 0 if all are clear. The inner scan unrolls to
// 8 straight-line checks; `unused` is a dead induction variable the retail
// build kept (addi r4, r4, 7 per batch).
int func_80198340(CfPartsListEntry* list) {
    int unused = 0;
    for (int i = 0; i < 2; i++) {
        for (int k = 0; k < 8; k++) {
            if (list[k].field_00 != 0) return 1;
        }
        list += 8;
        unused += 7;
    }
    return 0;
}

void func_80198400(){}

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

void func_80198524(){}

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
#pragma push
#pragma auto_inline off
extern "C" void func_801987A4(CfPartsTri* src, CfPartsTri* dst) {}
#pragma pop

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

void func_80198AE0(){}

void func_80198C24(){}

// Copy the 12-byte triple, then rotate the destination x/z by the source's
// turn index: angle = FIdx-scale * field_18, magnitude = (s32)field_14 *
// field_1C, x -= mag*sin(angle), z -= mag*cos(angle). The destination words
// are adjusted in-place as f32 (retail lfs/stfs at +0/+8).
extern "C" void func_80198D44(CfPartsMoveSrc* src, CfPartsTri* dst) {
    u32 w0 = src->field_00;
    u32 w1 = src->field_04;
    dst->field_04 = w1;
    dst->field_00 = w0;
    dst->field_08 = src->field_08;
    f32 angle = lbl_eu_80667B50 * src->field_18;
    f32 mag = (f32)(s32)src->field_14 * src->field_1C;
    *(f32*)&dst->field_00 -= mag * nw4r::math::SinFIdx(angle);
    *(f32*)&dst->field_08 -= mag * nw4r::math::CosFIdx(angle);
    func_801987A4((CfPartsTri*)src, dst);
}

void func_80198E0C(){}

void func_80198EE8(){}

void func_80198FC4(){}

void func_801990F0(){}

void func_8019922C(){}
