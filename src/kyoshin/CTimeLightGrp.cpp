// Auto-scaffolded catalog TU for kyoshin/CTimeLightGrp
// High-level C++ reconstruction from retail ASM.

#include "kyoshin/harness_catalog.hpp"
#include "monolib/util/reslist.hpp"

// -- External vtable/data symbols --
extern "C" u8 lbl_eu_80526418[];
extern "C" u8 lbl_eu_80526430[];
extern "C" u8 lbl_eu_80526448[];
extern "C" u32 lbl_eu_80666038; // float zero

// -- External functions --
extern "C" void* __dl__FPv(void* ptr);
extern "C" void* __dla__FPv(void* ptr);
extern "C" void* func_80496004(void* ptr);
extern "C" void* allocate_array__Q23mtl10MemManagerFUlUl(u32 size, u32 handle);

// CVirtualLightObj is an opaque pointer type in this TU.
typedef void* CVirtualLightObjPtr;

// -- CTimeLightGrp layout --
// Inherits from _reslist_base<CVirtualLightObjPtr> as a secondary base at +0x08.
// For matching, we model it as a flat struct.
struct CTimeLightGrp {
    // +0x00: vtable (CTimeLightGrp)
    void* vtbl;
    // +0x04: parent pointer
    void* mParent;
    // _reslist_base<CVirtualLightObjPtr> subobject at +0x08
    // +0x08: reslist_base vtable pointer
    void* resVtbl;
    // +0x0C: mStartNodePtr
    _reslist_node<CVirtualLightObjPtr>* mStartNodePtr;
    // +0x10: embedded mStartNode (mNext, mPrev, mItem)
    _reslist_node<CVirtualLightObjPtr> mStartNode;
    // +0x1C: mList (allocated node array)
    _reslist_node<CVirtualLightObjPtr>* mList;
    // +0x20: mCapacity
    int mCapacity;
    // +0x24: unk1C (byte flag)
    u8 mFlag;
    u8 pad_25[3];
    // +0x28: vector/colour components
    float mVal0;
    float mVal1;
    float mVal2;
    float mVal3;
    // +0x38: scale factor applied in traversal
    float mScale;
};

// ================== Function implementations ==================

// __ct__CTimeLightGrp: Constructor.
// Initialises base subobject, allocates node array, fills list slots.
extern "C" void __ct__CTimeLightGrp(CTimeLightGrp* self, void* parent) {
    _reslist_node<CVirtualLightObjPtr>* startNode;
    int i;
    int count;

    // Base initialisation
    self->vtbl = lbl_eu_80526418;
    self->mParent = parent;
    self->resVtbl = lbl_eu_80526448; // base vtable (overwritten later)

    // reslist_base inline constructor
    self->mList = nullptr;
    self->mCapacity = 0;
    self->mFlag = 0;
    startNode = &self->mStartNode; // &self->+0x10
    self->mStartNodePtr = startNode;
    startNode->mNext = startNode;
    startNode->mPrev = startNode;

    // Overwrite secondary vtable to the derived vtable
    self->resVtbl = lbl_eu_80526418;

    // Zero 5 float fields
    self->mVal0 = 0.0f;
    self->mVal1 = 0.0f;
    self->mVal2 = 0.0f;
    self->mVal3 = 0.0f;
    self->mScale = 0.0f;

    // Allocate node array: 32 * 0x0C = 0x180 bytes
    // func_80496004 converts parent to alloc handle
    self->mList = (decltype(self->mList))allocate_array__Q23mtl10MemManagerFUlUl(
        0x180, (u32)func_80496004(parent));

    // Initialise all 32 slot mNext pointers to null
    self->mCapacity = 32;
    count = 2; // unrolled outer loop
    i = 0;
    while (count--) {
        // Block 1 (8 entries)
        self->mList[i + 0].mNext = nullptr;
        self->mList[i + 1].mNext = nullptr;
        self->mList[i + 2].mNext = nullptr;
        self->mList[i + 3].mNext = nullptr;
        self->mList[i + 4].mNext = nullptr;
        self->mList[i + 5].mNext = nullptr;
        self->mList[i + 6].mNext = nullptr;
        self->mList[i + 7].mNext = nullptr;
        i += 8;
        // Block 2 (8 entries)
        self->mList[i + 0].mNext = nullptr;
        self->mList[i + 1].mNext = nullptr;
        self->mList[i + 2].mNext = nullptr;
        self->mList[i + 3].mNext = nullptr;
        self->mList[i + 4].mNext = nullptr;
        self->mList[i + 5].mNext = nullptr;
        self->mList[i + 6].mNext = nullptr;
        self->mList[i + 7].mNext = nullptr;
        i += 8;
    }
}

// __dt__8005A03C: operator-delete wrapper for a base subobject.
// Called by derived destructors to free memory.
extern "C" void __dt__8005A03C(void* obj, int mode) {
    if (obj != nullptr && mode > 0) {
        __dl__FPv(obj);
    }
}

// __dt___reslist_base_CVirtualLightObj:
// Destructor for _reslist_base<CVirtualLightObjPtr>.
// Sever list links, free backing array if flag is clear, free self if mode>0.
extern "C" void __dt___reslist_base_CVirtualLightObj(
    _reslist_base<CVirtualLightObjPtr>* self, int mode)
{
    if (self == nullptr) return;

    _reslist_node<CVirtualLightObjPtr>* cur;
    _reslist_node<CVirtualLightObjPtr>* head;
    _reslist_node<CVirtualLightObjPtr>* next;

    // Set vtable to base vtable
    self->_reslist_base<CVirtualLightObjPtr>::_reslist_base<CVirtualLightObjPtr>();
    // no, that's not right - let me just set the vtable directly:
    // Actually, in MWCC the vtable is set before the dtor body.
    // The retail code sets it to lbl_eu_80526448 via stw.

    // We model by direct vtable write since there's no dynamic_cast.
    *(void**)self = lbl_eu_80526448;

    // Clear list: null each node's mNext, re-link head to itself
    head = self->mStartNodePtr;
    cur = head->mNext;
    while (cur != head) {
        _reslist_node<CVirtualLightObjPtr>* prev = cur;
        cur = cur->mNext;
        prev->mNext = nullptr;
    }
    head->mNext = head;
    head->mPrev = head;

    // Free backing array if flag is not set
    if (!self->unk1C && self->mList != nullptr) {
        __dla__FPv(self->mList);
        self->mList = nullptr;
    }

    // Free self if mode > 0
    if (mode > 0) {
        __dl__FPv(self);
    }
}

// __dt__reslist_CVirtualLightObj:
// Destructor for reslist<CVirtualLightObjPtr>.
// Same body as base destructor; MWCC emits a redundant beq for
// the derived-class null check.
extern "C" void __dt__reslist_CVirtualLightObj(
    _reslist_base<CVirtualLightObjPtr>* self, int mode)
{
    if (self == nullptr) return;

    // (redundant beq here in retail — falls through since self != nullptr)

    _reslist_node<CVirtualLightObjPtr>* cur;
    _reslist_node<CVirtualLightObjPtr>* head;
    _reslist_node<CVirtualLightObjPtr>* next;

    *(void**)self = lbl_eu_80526448;

    // Clear list
    head = self->mStartNodePtr;
    cur = head->mNext;
    while (cur != head) {
        _reslist_node<CVirtualLightObjPtr>* prev = cur;
        cur = cur->mNext;
        prev->mNext = nullptr;
    }
    head->mNext = head;
    head->mPrev = head;

    // Free backing array if flag is not set
    if (!self->unk1C && self->mList != nullptr) {
        __dla__FPv(self->mList);
        self->mList = nullptr;
    }

    // Free self if mode > 0
    if (mode > 0) {
        __dl__FPv(self);
    }
}

// __dt__13CTimeLightGrpFv: CTimeLightGrp destructor.
// Tears down embedded reslist subobject at +0x08, then frees self.
extern "C" void __dt__13CTimeLightGrpFv(CTimeLightGrp* self, int mode) {
    _reslist_node<CVirtualLightObjPtr>* cur;
    _reslist_node<CVirtualLightObjPtr>* head;
    _reslist_node<CVirtualLightObjPtr>* next;

    if (self == nullptr) goto done;

    // Step 1: clear CTimeLightGrp's own list at +0x0C
    self->vtbl = lbl_eu_80526418;

    head = self->mStartNodePtr;
    cur = head->mNext;
    while (cur != head) {
        _reslist_node<CVirtualLightObjPtr>* prev = cur;
        cur = cur->mNext;
        prev->mNext = nullptr;
    }
    head->mNext = head;
    head->mPrev = head;

    // Step 2: destroy the reslist_base subobject at +0x08
    {
        _reslist_base<CVirtualLightObjPtr>* sub = 
            reinterpret_cast<_reslist_base<CVirtualLightObjPtr>*>(&self->resVtbl);
        // This is actually just re-doing the same clear on the shared list:
        if (sub != nullptr) {
            *(void**)sub = lbl_eu_80526448;
            head = self->mStartNodePtr;
            cur = head->mNext;
            while (cur != head) {
                _reslist_node<CVirtualLightObjPtr>* prev = cur;
                cur = cur->mNext;
                prev->mNext = nullptr;
            }
            head->mNext = head;
            head->mPrev = head;

            if (!sub->unk1C && sub->mList != nullptr) {
                __dla__FPv(sub->mList);
                sub->mList = nullptr;
            }
        }
    }

    // Step 3: free self if mode > 0
    if (mode > 0) {
        __dl__FPv(self);
    }

done:
    return;
}

// func_8005A2F0: Find first empty slot, store item, link into circular list.
// Equivalent to reslist<CVirtualLightObjPtr>::push_back.
extern "C" void func_8005A2F0(CTimeLightGrp* self, CVirtualLightObjPtr item) {
    int i;
    int count;
    _reslist_node<CVirtualLightObjPtr>* entry;
    _reslist_node<CVirtualLightObjPtr>* head;

    head = self->mStartNodePtr;
    count = self->mCapacity;

    // Find first empty slot (mNext == null)
    for (i = 0; i < count; i++) {
        if (self->mList[i].mNext == nullptr) break;
    }

    entry = &self->mList[i];
    // Store item at entry->mItem (+0x08)
    if (&entry->mItem != nullptr) {
        entry->mItem = item;
    }

    // Link into circular list (insert before head = push_back)
    entry->mNext = head;
    entry->mPrev = head->mPrev;
    head->mPrev->mNext = entry;
    head->mPrev = entry;
}

// func_8005A374: Traverse all nodes, update node data when attribute flag clear.
// Applies scaled vector from CTimeLightGrp floats.
extern "C" void func_8005A374(CTimeLightGrp* self) {
    _reslist_node<CVirtualLightObjPtr>* head;
    _reslist_node<CVirtualLightObjPtr>* cur;
    float sx, sy, sz;

    sx = self->mVal0 * self->mScale;
    sy = self->mVal1 * self->mScale;
    sz = self->mVal2 * self->mScale;
    float sw = self->mVal3;

    // Traverse list
    head = self->mStartNodePtr;
    cur = head->mNext;
    while (cur != head) {
        // Check flag at +0x19 of the data object
        u8* data = (u8*)cur->mItem;
        if (data != nullptr && data[0x19] == 0) {
            // Write scaled components as u32 bit patterns
            // (MWCC stores to stack as float, then loads as u32 via lwz)
            *(u32*)&data[0x1C] = *(u32*)&sx;
            *(u32*)&data[0x20] = *(u32*)&sy;
            *(u32*)&data[0x24] = *(u32*)&sz;
            *(u32*)&data[0x28] = *(u32*)&sw;
        }
        cur = cur->mNext;
    }
}
