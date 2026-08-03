#include <nw4hbm/ut.h>

namespace nw4hbm {
namespace ut {

void List_Init(List* pList, u16 offset) {
    pList->headObject = NULL;
    pList->tailObject = NULL;
    pList->numObjects = 0;
    pList->offset = offset;
}

static void SetFirstObject(List* pList, void* pObject) {
    Link* pLink = NW4R_UT_LIST_GET_LINK(*pList, pObject);
    pLink->nextObject = NULL;
    pLink->prevObject = NULL;

    pList->headObject = pObject;
    pList->tailObject = pObject;
    pList->numObjects++;
}

void List_Append(List* pList, void* pObject) {
    if (pList->headObject == NULL) {
        SetFirstObject(pList, pObject);
        return;
    }

    // Old tail <- New tail relationship
    Link* pLink = NW4R_UT_LIST_GET_LINK(*pList, pObject);
    pLink->prevObject = pList->tailObject;
    pLink->nextObject = NULL;

    // Old tail -> New tail relationship
    NW4R_UT_LIST_GET_LINK(*pList, pList->tailObject)->nextObject = pObject;
    pList->tailObject = pObject;

    pList->numObjects++;
}

// unused in Xenoblade retail: List_Prepend, List_Insert, List_GetPrev

void List_Remove(List* pList, void* pObject) {
    Link* pLink = NW4R_UT_LIST_GET_LINK(*pList, pObject);

    // Fix previous node relationship
    if (pLink->prevObject == NULL) {
        pList->headObject = pLink->nextObject;
    } else {
        NW4R_UT_LIST_GET_LINK(*pList, pLink->prevObject)->nextObject =
            pLink->nextObject;
    }

    // Fix next node relationship
    if (pLink->nextObject == NULL) {
        pList->tailObject = pLink->prevObject;
    } else {
        NW4R_UT_LIST_GET_LINK(*pList, pLink->nextObject)->prevObject =
            pLink->prevObject;
    }

    pLink->prevObject = NULL;
    pLink->nextObject = NULL;

    pList->numObjects--;
}

void* List_GetNext(const List* pList, const void* pObject) {
    if (pObject == NULL) {
        return pList->headObject;
    }
    return NW4R_UT_LIST_GET_LINK(*pList, pObject)->nextObject;
}

void* List_GetNth(const List* pList, u16 n) {
    int i;
    void* pIt = NULL;

    for (i = 0, pIt = NULL; (pIt = List_GetNext(pList, pIt)) != NULL; i++) {
        if (n == i) {
            return pIt;
        }
    }

    return NULL;
}

} // namespace ut
} // namespace nwhbm
