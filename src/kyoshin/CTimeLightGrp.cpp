// Auto-scaffolded catalog TU for kyoshin/CTimeLightGrp
// High-level C++ reconstruction from retail ASM.

#include "kyoshin/harness_catalog.hpp"
#include "monolib/util/reslist.hpp"
#include "kyoshin/CTimeLightGrp.hpp"

// Forward declare the opaque pointer type used in this TU
class CVirtualLightObj;
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
extern "C" void __dt__8005A03C(void* obj, int mode) {
    if (obj == nullptr) return;
    if (mode > 0) __dl__FPv(obj);
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
extern "C" void func_8005A2F0(u8* self, CVirtualLightObjPtr item) {
    // reslist<CVirtualLightObj*> push_back, inlined through the template so the
    // try/catch setItem materialises the mr r31,r1 / stw r1 frame (retail ABI).
    // This reproduces the retail text structure exactly (structural=0); the
    // residual is a pure MWCC-internal register rotation (startNode r7 vs retail
    // r9). Declaring startNode last would fix the register but breaks the loop's
    // prologue load hoisting (structural 17), so the template is the best shape.
    reslist<CVirtualLightObjPtr>* base =
        (reslist<CVirtualLightObjPtr>*)((u8*)self + 8);
    base->push_back((CVirtualLightObjPtr)item);
}

// ================== func_8005A374 ==================
extern "C" void func_8005A374(CTimeLightGrp_BaseLayout* self) {
    CTimeLightGrp_BaseLayout* p = self;
    _reslist_base<CVirtualLightObjPtr>* base =
        (_reslist_base<CVirtualLightObjPtr>*)((u8*)self + 8);
    _reslist_node<CVirtualLightObjPtr>* head;
    _reslist_node<CVirtualLightObjPtr>* cur;

    // Retail loads mScale first (kept in f3), then mVal2,mVal1,mVal0, computes
    // muls sz,sy,(sw),sx, and stores the bit reinterprets before the loop.
    float scale = p->mScale;
    float sz = p->mVal2 * scale;
    float sy = p->mVal1 * scale;
    float sw = p->mVal3;
    float sx = p->mVal0 * scale;

    u32 szBits = *(u32*)&sz;
    u32 sxBits = *(u32*)&sx;
    u32 syBits = *(u32*)&sy;
    u32 swBits = *(u32*)&sw;

    head = base->mStartNodePtr;
    cur = head->mNext;

    while (cur != head) {
        u8* data = (u8*)cur->mItem;
        if (data[0x19] == 0) {
            *(u32*)(data + 0x1C) = sxBits;
            *(u32*)(data + 0x20) = syBits;
            *(u32*)(data + 0x24) = szBits;
            *(u32*)(data + 0x28) = swBits;
        }
        cur = cur->mNext;
    }
}
