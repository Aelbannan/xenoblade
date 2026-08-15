#pragma once

#include <types.h>

/*
Doubly-linked list node
*/
class __declspec(novtable) CDoubleListNode {
public:
    CDoubleListNode* mPrev; //0x0
    CDoubleListNode* mNext; //0x4
    u8 unk8;
    int unkC;

public:
    CDoubleListNode();
    virtual ~CDoubleListNode() {}
    virtual void Reset();

    //Node links
    CDoubleListNode* GetPrev() const { return mPrev; }
    CDoubleListNode* GetNext() const { return mNext; }
    void SetPrev(CDoubleListNode* prev) { mPrev = prev; }
    void SetNext(CDoubleListNode* next) { mNext = next; }
}; // size = 0x10

// C-linkage imports (retail symbol names - keep linkage/signatures verbatim)
// Retail CDoubleListNode vtable data (monolibdata1d.s @0x8056BB90). The class
// is __declspec(novtable), so the ctor assigns the retail label explicitly
// instead of the compiler-generated __vt__15CDoubleListNode (which would add
// .data/.rodata/RTTI to a retail-empty TU).
extern "C" void* lbl_eu_8056BB90[];

/*
Doubly-linked, circular list
*/
class CDoubleListHeader {
private:
    /* Circular list which begins *AFTER* mEnd:
    mEnd ("End") -> X ("Top") -> Y -> Z -> mEnd */
    CDoubleListNode* mEnd; //0x0

public:
    CDoubleListHeader() { Reset(); }
    
    void Reset() { mEnd = nullptr; }

    //List iterators
    CDoubleListNode* Begin() const {
        return mEnd != nullptr ? mEnd->GetNext() : nullptr;
    }
    CDoubleListNode* End() const { return mEnd; }

    //Add/remove nodes
    CDoubleListNode* InsertTop(CDoubleListNode* node);
    CDoubleListNode* InsertEnd(CDoubleListNode* node);
    CDoubleListNode* Remove(CDoubleListNode* node);
};
