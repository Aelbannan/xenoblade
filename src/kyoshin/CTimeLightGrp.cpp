// Auto-scaffolded catalog TU for kyoshin/CTimeLightGrp
// High-level C++ reconstruction from retail ASM.

#include "kyoshin/harness_catalog.hpp"
#include "monolib/util/reslist.hpp"

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
    // _reslist_base<CVirtualLightObjPtr> starts here at offset +0x08
    _reslist_base<CVirtualLightObjPtr> reslistBase;                  // +0x08..+0x27
    // CTimeLightGrp extra fields beyond the base
    float mVal0;                                                     // +0x28
    float mVal1;                                                     // +0x2C
    float mVal2;                                                     // +0x30
    float mVal3;                                                     // +0x34
    float mScale;                                                    // +0x38
};

extern "C" u8 lbl_eu_80526418[];
extern "C" u8 lbl_eu_80526430[];
extern "C" u8 lbl_eu_80526448[];

extern "C" void* __dl__FPv(void* ptr);
extern "C" void* __dla__FPv(void* ptr);
extern "C" void* func_80496004(void* ptr);
extern "C" void* allocate_array__Q23mtl10MemManagerFUlUl(u32 size, u32 handle);

// ================== __ct__CTimeLightGrp ==================
extern "C" void __ct__CTimeLightGrp(void* self, void* parent) {
    CTimeLightGrp_BaseLayout* p = (CTimeLightGrp_BaseLayout*)self;
    _reslist_base<CVirtualLightObjPtr>* base = &p->reslistBase;
    int i;

    // CTimeLightGrp fields
    p->vtbl = lbl_eu_80526418;
    p->parent = parent;

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
    p->mVal0 = 0.0f;
    p->mVal1 = 0.0f;
    p->mVal2 = 0.0f;
    p->mVal3 = 0.0f;
    p->mScale = 0.0f;

    // Allocate and initialise node array
    base->mList = (_reslist_node<CVirtualLightObjPtr>*)
        allocate_array__Q23mtl10MemManagerFUlUl(
            0x180, (u32)func_80496004(parent));
    base->mCapacity = 32;

    for (i = 0; i < 32; i++) {
        base->mList[i].mNext = nullptr;
    }
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

        head = p->reslistBase.mStartNodePtr;
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
    base = &p->reslistBase;
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
extern "C" void func_8005A2F0(void* self, void* item) {
    CTimeLightGrp_BaseLayout* p = (CTimeLightGrp_BaseLayout*)self;
    _reslist_base<CVirtualLightObjPtr>* base = &p->reslistBase;
    _reslist_node<CVirtualLightObjPtr>* entry;
    _reslist_node<CVirtualLightObjPtr>* head;
    int i;

    head = base->mStartNodePtr;
    i = 0;

    while (i < base->mCapacity) {
        if (base->mList[i].mNext == nullptr) break;
        i++;
    }

    entry = &base->mList[i];
    if (&entry->mItem != nullptr) {
        entry->mItem = (CVirtualLightObjPtr)item;
    }

    // push_back
    entry->mNext = head;
    entry->mPrev = head->mPrev;
    head->mPrev->mNext = entry;
    head->mPrev = entry;
}

// ================== func_8005A374 ==================
extern "C" void func_8005A374(void* self) {
    CTimeLightGrp_BaseLayout* p = (CTimeLightGrp_BaseLayout*)self;
    _reslist_base<CVirtualLightObjPtr>* base = &p->reslistBase;
    _reslist_node<CVirtualLightObjPtr>* head;
    _reslist_node<CVirtualLightObjPtr>* cur;
    float sx, sy, sz, sw;

    sx = p->mVal0 * p->mScale;
    sy = p->mVal1 * p->mScale;
    sz = p->mVal2 * p->mScale;
    sw = p->mVal3;

    head = base->mStartNodePtr;
    cur = head->mNext;

    while (cur != head) {
        u8* data = (u8*)cur->mItem;
        if (data != nullptr && data[0x19] == 0) {
            *(u32*)(data + 0x1C) = *(u32*)&sx;
            *(u32*)(data + 0x20) = *(u32*)&sy;
            *(u32*)(data + 0x24) = *(u32*)&sz;
            *(u32*)(data + 0x28) = *(u32*)&sw;
        }
        cur = cur->mNext;
    }
}
