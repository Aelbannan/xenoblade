#include "monolib/util/CChildListNode.hpp"

CChildListNode::CChildListNode()
    : mParent(NULL) {

    // vptr slots (novtable): CChildListNode vtable at +0x10 (overrides the
    // base CDoubleListNode vptr), TChildListHeader vtable at +0x34 (mChildren).
    // Retail store order: mParent, vptr@0x10, vptr@0x34, mChildren.Reset().
    *(void**)((char*)this + 0x10) = (void*)lbl_eu_8056BBA0;
    *(void**)((char*)this + 0x34) = (void*)lbl_eu_8056BBB0;
    mChildren.Reset();
}

void CChildListNode::Reset() {
    CDoubleListNode::Reset();
    mParent = NULL;
    mChildren.Reset();
}
