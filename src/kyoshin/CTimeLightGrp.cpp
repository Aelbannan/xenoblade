// Auto-scaffolded catalog TU for kyoshin/CTimeLightGrp
// High-level C++ reconstruction from retail ASM.

#include "kyoshin/harness_catalog.hpp"
#include "monolib/util/reslist.hpp"
#include "kyoshin/CTimeLightGrp.hpp"

// Element type of the reslist managed by CTimeLightGrp (pre-existing alias).
typedef void* CVirtualLightObjPtr;

// CTimeLightGrp inherits from _reslist_base<CVirtualLightObjPtr>.
// The retail layout has _reslist_base fields starting at +0x08, with
// a CTimeLightGrp-level vtable and parent pointer at +0x00/+0x04.
//
// MWCC places the secondary base at +0x08 and stores lbl_eu_80526430
// as a "data pointer" in the vtable slot during normal operation,
// restoring the true base vtable (lbl_eu_80526448) only during destruction.
struct CTimeLightGrp_BaseLayout {
    void* vtbl;                                                      // +0x00
    void* parent;                                                    // +0x04
    void* resVtbl;                                                   // +0x08
    _reslist_node<CVirtualLightObjPtr>* mStartNodePtr;               // +0x0C
    _reslist_node<CVirtualLightObjPtr> mStartNode;                   // +0x10
    _reslist_node<CVirtualLightObjPtr>* mList;                       // +0x1C
    int mCapacity;                                                   // +0x20
    u8 unk1C;                                                        // +0x24
    u8 pad_25[3];                                                    // +0x25..+0x27
    float mVal0;                                                     // +0x28
    float mVal1;                                                     // +0x2C
    float mVal2;                                                     // +0x30
    float mVal3;                                                     // +0x34
    float mScale;                                                    // +0x38
};

// ================== __ct__CTimeLightGrp ==================
extern "C" void __ct__CTimeLightGrp(CTimeLightGrp_BaseLayout* self, void* parent) {
    CTimeLightGrp_BaseLayout* p = self;
    _reslist_base<CVirtualLightObjPtr>* base =
        (_reslist_base<CVirtualLightObjPtr>*)((u8*)self + 8);
    int i;

    // CTimeLightGrp fields
    p->vtbl = lbl_eu_80526418;
    p->parent = parent;
    p->resVtbl = lbl_eu_80526448;

    // Base init (inlined _reslist_base ctor)
    base->mList = nullptr;
    base->mCapacity = 0;
    base->unk1C = false;
    base->mStartNodePtr = &base->mStartNode;
    base->mStartNodePtr->mNext = &base->mStartNode;
    base->mStartNodePtr->mPrev = base->mStartNode.mNext;

    // Override base vtable with CTimeLightGrp vtable at +0x08
    // Retail stores lbl_eu_80526430 here (data pointer).
    // But the destructor restores lbl_eu_80526448.
    // MWCC: this is a secondary vtable optimization.
    *(void**)((u8*)self + 8) = lbl_eu_80526430;

    // Zero extra float fields
    p->mVal0 = lbl_eu_80666038;
    p->mVal1 = lbl_eu_80666038;
    p->mVal2 = lbl_eu_80666038;
    p->mVal3 = lbl_eu_80666038;
    p->mScale = lbl_eu_80666038;

    // Allocate and initialise node array
    base->mList = (_reslist_node<CVirtualLightObjPtr>*)
        allocate_array__Q23mtl10MemManagerFUlUl(
            0x180, (u32)func_80496004(parent));

    for (i = 0; i < 32; i++) {
        base->mList[i].mNext = nullptr;
    }
    base->mCapacity = 32;
}

// ================== __dt__8005A03C ==================
extern "C" void* __dt__8005A03C(void* obj, int mode) {
    if (obj != 0 && mode > 0) {
        __dl__FPv(obj);
    }
    return obj;
}

// ================== __dt___reslist_base_CVirtualLightObj ==================
extern "C" void __dt___reslist_base_CVirtualLightObj(void* self, int mode) {
    _reslist_base<CVirtualLightObjPtr>* base = (_reslist_base<CVirtualLightObjPtr>*)self;

    if (base == nullptr) return;

    // Set base vtable
    *(void**)base = lbl_eu_80526448;

    // Clear list: null each node's mNext, re-link head to itself
    {
        _reslist_node<CVirtualLightObjPtr>* cur;
        _reslist_node<CVirtualLightObjPtr>* head;

        head = base->mStartNodePtr;
        cur = head->mNext;
        while (cur != head) {
            _reslist_node<CVirtualLightObjPtr>* prev = cur;
            cur = cur->mNext;
            prev->mNext = nullptr;
        }
        head->mNext = head;
        head->mPrev = head;
    }

    // Free backing array if flag is clear and pointer is non-null
    if (!base->unk1C && base->mList != nullptr) {
        __dla__FPv(base->mList);
        base->mList = nullptr;
    }

    // Free self if mode > 0
    if (mode > 0) __dl__FPv(base);
}

// ================== __dt__reslist_CVirtualLightObj ==================
extern "C" void __dt__reslist_CVirtualLightObj(void* self, int mode) {
    _reslist_base<CVirtualLightObjPtr>* base = (_reslist_base<CVirtualLightObjPtr>*)self;

    if (base == nullptr) return;

    // (redundant beq in retail MWCC output)

    *(void**)base = lbl_eu_80526448;

    {
        _reslist_node<CVirtualLightObjPtr>* cur;
        _reslist_node<CVirtualLightObjPtr>* head;

        head = base->mStartNodePtr;
        cur = head->mNext;
        while (cur != head) {
            _reslist_node<CVirtualLightObjPtr>* prev = cur;
            cur = cur->mNext;
            prev->mNext = nullptr;
        }
        head->mNext = head;
        head->mPrev = head;
    }

    if (!base->unk1C && base->mList != nullptr) {
        __dla__FPv(base->mList);
        base->mList = nullptr;
    }

    if (mode > 0) __dl__FPv(base);
}

// ================== __dt__13CTimeLightGrpFv ==================
extern "C" void __dt__13CTimeLightGrpFv(void* self, int mode) {
    CTimeLightGrp_BaseLayout* p = (CTimeLightGrp_BaseLayout*)self;
    _reslist_base<CVirtualLightObjPtr>* base;

    if (self == nullptr) return;

    // Step 1: clear the CTimeLightGrp-level list
    p->vtbl = lbl_eu_80526418;
    {
        _reslist_node<CVirtualLightObjPtr>* cur;
        _reslist_node<CVirtualLightObjPtr>* head;

        head = p->mStartNodePtr;
        cur = head->mNext;
        while (cur != head) {
            _reslist_node<CVirtualLightObjPtr>* prev = cur;
            cur = cur->mNext;
            prev->mNext = nullptr;
        }
        head->mNext = head;
        head->mPrev = head;
    }

    // Step 2: destroy the reslist_base subobject
    base = (_reslist_base<CVirtualLightObjPtr>*)((u8*)self + 8);
    if (base != nullptr) {
        *(void**)base = lbl_eu_80526448;

        {
            _reslist_node<CVirtualLightObjPtr>* cur;
            _reslist_node<CVirtualLightObjPtr>* head;

            head = base->mStartNodePtr;
            cur = head->mNext;
            while (cur != head) {
                _reslist_node<CVirtualLightObjPtr>* prev = cur;
                cur = cur->mNext;
                prev->mNext = nullptr;
            }
            head->mNext = head;
            head->mPrev = head;
        }

        if (!base->unk1C && base->mList != nullptr) {
            __dla__FPv(base->mList);
            base->mList = nullptr;
        }
    }

    // Step 3: free self if mode > 0
    if (mode > 0) __dl__FPv(self);
}

// ================== func_8005A2F0 ==================
// reslist<CVirtualLightObjPtr>::push_back (inlined). Finds the first empty slot
// (mList[i].mNext == 0), stores the item (setItem try/catch reproduces the
// retail EH frame), and links the node before the head sentinel.
extern "C" void func_8005A2F0(CTimeLightGrp_BaseLayout* self, CVirtualLightObjPtr item) {
    reslist<CVirtualLightObjPtr>* base =
        (reslist<CVirtualLightObjPtr>*)((u8*)self + 8);

    _reslist_node<CVirtualLightObjPtr>* startNode = base->mStartNodePtr;
    int i = base->findFirstEmptySlotIndex();
    _reslist_node<CVirtualLightObjPtr>* temp = &base->mList[i];

    temp->setItem(item);
    temp->mNext = startNode;
    temp->mPrev = startNode->mPrev;
    startNode->mPrev->mNext = temp;
    startNode->mPrev = temp;
}

// ================== func_8005A374 ==================
extern "C" void func_8005A374(CTimeLightGrp_BaseLayout* self) {
    CTimeLightGrp_BaseLayout* p = self;

    // Scaled components. Retail CSEs mScale into one FPR, computes products
    // sz, sy, sx (sw loaded in between), then reinterprets them via stack peek
    // slots (stfs + lwz) in [sx, sy, sz, sw] order before the walk.
    float scale = p->mScale;
    float sz = p->mVal2 * scale;
    float sy = p->mVal1 * scale;
    float sw = p->mVal3;
    float sx = p->mVal0 * scale;

    // Reinterprets live in a 4-word block; MWCC fuses each component's peek
    // slot with the array element, then hoists the four lwz before the walk
    // into consecutive GPRs (retail: r7,r6,r5,r4 for sx,sy,sz,sw).
    u32 bits[4];
    bits[0] = *(u32*)&sx;
    bits[1] = *(u32*)&sy;
    bits[2] = *(u32*)&sz;
    bits[3] = *(u32*)&sw;

    // Re-read mStartNodePtr from self each iteration like retail (reloads
    // 12(r3) on every back-edge) so `self` stays live in r3.
    _reslist_node<CVirtualLightObjPtr>* cur = self->mStartNodePtr->mNext;
    while (cur != self->mStartNodePtr) {
        CVirtualLightObjFields* obj = (CVirtualLightObjFields*)cur->mItem;
        if (obj->mByte19 == 0) {
            *(u32*)&obj->mField1C = bits[0];   // sx
            *(u32*)&obj->mField20 = bits[1];   // sy
            *(u32*)&obj->mField24 = bits[2];   // sz
            *(u32*)&obj->mField28 = bits[3];   // sw
        }
        cur = cur->mNext;
    }
}
