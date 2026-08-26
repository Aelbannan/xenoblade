// Auto-scaffolded catalog TU for kyoshin/CTimeLightGrp
// High-level C++ reconstruction from retail ASM.

#include "kyoshin/harness_catalog.hpp"
#include "monolib/util/reslist.hpp"
#include "kyoshin/CTimeLightGrp.hpp"

// ================== __dt__13CTimeLightGrpFv ==================
// Complete-object destructor. Installs the derived vtable, clears the light
// list, then destroys the embedded reslist subobject (restores its base
// vtable, re-clears the now-empty ring, and frees the node array when the
// list owns it). mode > 0 frees self.
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
extern "C" CTimeLightGrp_BaseLayout* __ct__CTimeLightGrp(CTimeLightGrp_BaseLayout* self, void* parent) {
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
    return self;
}

// ================== __dt__8005A03C ==================
extern "C" void* __dt__8005A03C(void* obj, int mode) {
    if (obj != 0 && mode > 0) {
        __dl__FPv(obj);
    }
    return obj;
}

// ================== __dt___reslist_base_CVirtualLightObj ==================
// Complete object destructor for the _reslist_base<CVirtualLightObj> base of
// CTimeLightGrp (+0x08 subobject). Reinstalls the base vtable, unlinks every
// chained node (nulling each node's next as walked), re-links the sentinel to
// itself, frees the pool array when the ownership flag (0x1c) is clear, and
// applies the deleting-dtor tail (operator delete) when mode > 0. Returns this.
extern "C" void* __dt___reslist_base_CVirtualLightObj(void* self, int mode) {
    _reslist_base<CVirtualLightObjPtr>* base = (_reslist_base<CVirtualLightObjPtr>*)self;
    if (base != nullptr) {
        // Reinstall the base vtable (restored during destruction).
        *(void**)base = lbl_eu_80526448;

        // Unlink every chained node; the sentinel pointer is re-read each
        // iteration (the node store may alias it).
        _reslist_node<CVirtualLightObjPtr>* node = base->mStartNodePtr->mNext;
        while (node != base->mStartNodePtr) {
            _reslist_node<CVirtualLightObjPtr>* cur = node;
            node = cur->mNext;
            cur->mNext = nullptr;
        }
        base->mStartNodePtr->mNext = base->mStartNodePtr;
        base->mStartNodePtr->mPrev = base->mStartNodePtr;

        // Free backing array if the ownership flag is clear and pointer is non-null
        if (base->unk1C == false && base->mList != nullptr) {
            __dla__FPv(base->mList);
            base->mList = nullptr;
        }

        // Deleting-dtor tail: free self if mode > 0
        if (mode > 0) {
            __dl__FPv(base);
        }
    }
    return self;
}

// Complete-object destructor of the standalone reslist<CVirtualLightObj>
// (same teardown as the embedded _reslist_base copy inside CTimeLightGrp,
// with all fields 8 bytes lower since there is no secondary-base offset).
extern "C" void* __dt__reslist_CVirtualLightObj(void* self, int mode) {
    _reslist_base<CVirtualLightObjPtr>* base = (_reslist_base<CVirtualLightObjPtr>*)self;
    if (base != nullptr) {
        // Doubly-nested guard: MWCC CSEs the duplicated test into one
        // cmpwi + two beq's with different targets (retail shows the dead
        // second branch).
        if (base != nullptr) {
            _reslist_node<CVirtualLightObjPtr>* cur;
            // Reinstall the base vtable (restored during destruction).
            *(void**)base = lbl_eu_80526448;

            // Inlined _reslist_base::clearList(): unlink every chained node.
            // The sentinel pointer is re-read from the object each iteration.
            _reslist_node<CVirtualLightObjPtr>* node = base->mStartNodePtr->mNext;
            while (base->mStartNodePtr != node) {
                _reslist_node<CVirtualLightObjPtr>* cur = node;
                node = cur->mNext;
                // func_8049CB70(cur) inlined: clear the forward link.
                cur->mNext = nullptr;
            }
            base->mStartNodePtr->mNext = base->mStartNodePtr;
            base->mStartNodePtr->mPrev = base->mStartNodePtr;

            // Free the backing array when this list owns it.
            if (base->unk1C == false && base->mList != nullptr) {
                __dla__FPv(base->mList);
                base->mList = nullptr;
            }
        }

        // Deleting-dtor tail.
        if (mode > 0) __dl__FPv(base);
    }
    return self;
}

CTimeLightGrp::~CTimeLightGrp() {
    mVtable = lbl_eu_80526418;
    mList.clearList();
    // Implicit destruction of mList follows here.
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

    // Scaled colour components. Retail keeps mScale in one FPR and computes
    // sz, sy, sx (with sw loaded in between). Declaration order matters here:
    // sw must be declared before sy so MWCC assigns sw the register freed by
    // mVal1's multiply (sy gets a fresh FPR), matching retail's allocation.
    float scale = p->mScale;
    float sz = p->mVal2 * scale;
    float sw = p->mVal3;
    float sy = p->mVal1 * scale;
    float sx = p->mVal0 * scale;

    // Materialise the four components into a stack block. The store order
    // (sz, sx, sy, sw -> slots 16, 8, 12, 20) matches the retail stfs
    // sequence; the array keeps the bit patterns so the walk below can copy
    // them to each light object (stfs + hoisted lwz + stw round-trip).
    f32 vals[4];
    vals[2] = sz;
    vals[0] = sx;
    vals[1] = sy;
    vals[3] = sw;

    // Walk the light-object list and overwrite each object's four f32 scale
    // fields with the bit patterns. obj is declared before the loop so MWCC
    // assigns cur r9 and obj r8 like retail; the byte flag gates the write.
    CVirtualLightObjFields* obj;
    _reslist_node<CVirtualLightObjPtr>* cur = self->mStartNodePtr->mNext;
    while (cur != self->mStartNodePtr) {
        obj = (CVirtualLightObjFields*)cur->mItem;
        if (obj->mByte19 == 0) {
            *(u32*)&obj->mField1C = *(u32*)&vals[0];   // sx
            *(u32*)&obj->mField20 = *(u32*)&vals[1];   // sy
            *(u32*)&obj->mField24 = *(u32*)&vals[2];   // sz
            *(u32*)&obj->mField28 = *(u32*)&vals[3];   // sw
        }
        cur = cur->mNext;
    }
}
