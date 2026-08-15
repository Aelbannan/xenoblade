#include "monolib/util.hpp"

// ---- Retail data (blob monolibdata1d dissolve): CDoubleListNode vtable ----
// 0x8056BB90: [RTTI CDoubleListNode (lbl_eu_80661958), 0, ~dtor, Reset].
extern "C" void* lbl_eu_80661958;   // RTTI CDoubleListNode (sdata)
extern "C" void __dt__15CDoubleListNodeFv();
extern "C" void Reset__15CDoubleListNodeFv();

extern "C" void* lbl_eu_8056BB90[4] = {
    (void*)&lbl_eu_80661958, 0,
    (void*)&__dt__15CDoubleListNodeFv,
    (void*)&Reset__15CDoubleListNodeFv,
};

CDoubleListNode::CDoubleListNode() {
    // vptr sits at +0x10 (virtuals declared after the data members)
    *(void**)((char*)this + 0x10) = (void*)lbl_eu_8056BB90;
    CDoubleListNode::Reset();
}

void CDoubleListNode::Reset() {
    mPrev = NULL;
    mNext = NULL;
    unk8 = 0;
    unkC = -1;
}

CDoubleListNode* CDoubleListHeader::InsertTop(CDoubleListNode* node) {
    CDoubleListNode* end;
    CDoubleListNode* first;

    if (node != NULL) {
        end = End();
        
        //No list, create a new one
        if (end == NULL) {
            mEnd = node;

            //List is circular
            node->SetPrev(node);
            node->SetNext(node);
        }
        //Insert node at top of existing list
        else {
            first = mEnd->GetNext();

            //Fix link
            node->SetNext(first);
            node->SetPrev(end);
            first->SetPrev(node);
            mEnd->SetNext(node);
        }
    }

    return node;
}

CDoubleListNode* CDoubleListHeader::InsertEnd(CDoubleListNode* node) {
    if (node != NULL) {
        InsertTop(node);
        //List is circular
        mEnd = node;
    }

    return node;
}

CDoubleListNode* CDoubleListHeader::Remove(CDoubleListNode* child) {
    CDoubleListNode* end;

    //Nothing to remove
    if (child == NULL) {
        return NULL;
    }

    end = End();

    //List is not empty
    if (end != NULL) {
        //Only one element
        if (end == end->GetNext()) {
            //Child is not that one element
            if (end != child) {
                //Therefore it is not in this list
                return NULL;
            }

            //The child was the only element
            mEnd = NULL;
        }
        //More than one element
        else {
            //Remove link from prev child
            if (child->GetPrev() != NULL) {
                child->GetPrev()->SetNext(child->GetNext());
            }
            //Remove link from next child
            if (child->GetNext() != NULL) {
                child->GetNext()->SetPrev(child->GetPrev());
            }

            //Fix end if child was the end
            if (child == mEnd) {
                mEnd = child->GetPrev();
            }
        }
    
        //Isolate child
        child->SetPrev(NULL);
        child->SetNext(NULL);
    }

    return child;
}
