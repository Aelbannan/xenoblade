#pragma once

#include <types.h>
#include <monolib/scn/CScnFilter.hpp>

// Flag-test helper shared with the CScnVirtualLight unit (retail 0x8004BAB0):
// returns (*(u32*)self & flags) != 0. C-linkage (flat retail symbol).
extern "C" s32 func_8004B3D8(u32* self, u32 flags);

// Placement-new operator (retail flat symbol 0x80497328): returns the
// placement pointer unchanged. Shared with the CScnVirtualLight unit.
extern "C" void* func_804932B4(void* self, void* param);

// Minimal layout views for the CScnFilter reslist helpers in this TU
// (mirror the _reslist_node / _reslist_base offsets from
// monolib/util/reslist.hpp; only the fields these helpers touch are declared).
struct CScnFilterListNode {
    CScnFilterListNode* mNext;  // 0x0
    CScnFilterListNode* mPrev;  // 0x4
    CScnFilter* mItem;          // 0x8
};

struct CScnFilterList {
    u32 field_0x0;                 // 0x0 (vtable ptr)
    CScnFilterListNode* mStartNodePtr;  // 0x4
};

struct CScnFilterListIter {
    CScnFilterListNode* mNode;  // 0x0
};

// _reslist_base<CScnFilter*> layout (mirror of monolib/util/reslist.hpp
// offsets: vtable@0, mStartNodePtr@4, sentinel node@8, mList@0x14,
// mCapacity@0x18, ownership flag@0x1C). Each node is 0xC bytes
// (mNext/mPrev/mItem), so mList[i] strides 0xC in the node array.
struct CScnFilterReslist {
    u32* mVtable;                     // 0x00 vtable slot
    CScnFilterListNode* mStartNodePtr;  // 0x04 sentinel pointer
    CScnFilterListNode mStartNode;    // 0x08 sentinel node (0xC bytes)
    CScnFilterListNode* mList;        // 0x14 node array
    int mCapacity;                    // 0x18
    bool field_0x1C;                  // 0x1C ownership flag
};

// reslist<CScnFilter*> vtables (retail .data). Array-typed so MWCC emits
// absolute lis/addi addressing instead of an SDA21 access.
extern u32 lbl_eu_8056EBC4[];  // _reslist_base vtable

extern u32 lbl_eu_8056EBAC[];  // reslist vtable
extern u32 lbl_eu_8056EB88[];  // 0x8049CBD4 class vtable

extern u32 lbl_eu_8056EBA0[];  // _reslist_base<_reslist_iterator<...>> vtable

// The item type stored in the iterator reslist's nodes
// (_reslist_iterator<CScnFilter,CScnFilter,CScnFilter>, retail mItem at
// node+0x8). The user-declared default ctor makes the node array cookie'd
// (retail reserve allocates count*12 + 0x10 and runs __construct_new_array);
// with no user dtor the node dtor stays trivial, so delete[] in the dtor
// frees the cookie WITHOUT a per-element dtor loop (retail __dt__ shape:
// subi r3,r3,0x10 before __dla__FPv).
struct CScnFilterIterNode;  // forward decl for the item's back-pointer

struct CScnFilterIterator {
    CScnFilterIterNode* mNode;  // 0x0
    CScnFilterIterator() : mNode(nullptr) {}
};

// _reslist_node<_reslist_iterator<CScnFilter,CScnFilter,CScnFilter>>: same
// 0xC layout as CScnFilterListNode, but the item is an iterator object.
struct CScnFilterIterNode {
    CScnFilterIterNode* mNext;   // 0x0
    CScnFilterIterNode* mPrev;   // 0x4
    CScnFilterIterator mItem;    // 0x8
};

// _reslist_base<_reslist_iterator<CScnFilter,CScnFilter,CScnFilter>> layout
// (same offsets as CScnFilterReslist; the nodes hold iterator items).
struct CScnFilterIteratorReslist {
    u32* mVtable;                      // 0x00 vtable slot
    CScnFilterIterNode* mStartNodePtr; // 0x04 sentinel pointer
    CScnFilterIterNode mStartNode;     // 0x08 sentinel node (0xC bytes)
    CScnFilterIterNode* mList;         // 0x14 node array
    int mCapacity;                     // 0x18
    bool field_0x1C;                   // 0x1C ownership flag
};

class __declspec(novtable) CScnFilterMan {
public:
    CScnFilterMan();
    virtual ~CScnFilterMan();
    void update();
    void cbRenderBefore();

    u8 field_0x00[0x04];          // 0x04 mScene ptr (vtable implicit at 0x00)
    CScnFilterReslist field_0x08; // 0x08 reslist<CScnFilter*> member
};
