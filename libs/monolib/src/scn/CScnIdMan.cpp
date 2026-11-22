// Decompiled for monolib/src/scn/CScnIdMan

#include <types.h>
#include "libs/monolib/src/scn/CScnIdMan.hpp"
#include "monolib/scn/code_804BC9EC.hpp"

extern "C" {
    extern CScnIdPoolSlot* CScnItemPool_lookupSubPool(void* pPool, int slotIdx);
    extern void* Scn_FindCamItem(void* pParam, u32 camIdx);
    extern void* Scn_GetCamItem9C(void* pParam, u32 camIdx);
    extern void CamMan_BuildPerspective(void* pOut, void* pCamItem);
    extern void Coli_RenderChunks(void* pScnHandle, void* pCamItem, void* pStack);
}

CScnIdMan::CScnIdMan(CScn* pScn) {
    mScn = pScn;
    mFlag = 0;
    pScn->addRenderCB(this, 6, 0);
}

CScnIdMan::~CScnIdMan() {
    mScn->removeRenderCB(this);
}

extern "C" int func_8049E51C(const CScnIdMan* self) {
    // Fetch the scene-id pool slot and count its live nodes; return 1 if empty.
    // const self: MWCC hoists the first load (lwz r3,4(r3)) above the LR save
    // (load-hoist family lever; non-const emits stw-first - 3 structural).
    CScnIdNode* node;
    u32 count;
    CScnIdNode* anchor;
    CScnIdPoolSlot* slot = CScnItemPool_lookupSubPool(self->mScn->mItemPool, 5);

    anchor = slot->mAnchor;
    count = 0;
    node = anchor->next;
    while (node != anchor) {
        node = node->next;
        count++;
    }
    return static_cast<u32>(__cntlzw(count)) >> 5;
}

void CScnIdMan::cbRenderBefore(CScn*) {
    if (mFlag) {
        void* a = Scn_FindCamItem(mScn, -1);
        u8 stackbuf[0x38];
        CamMan_BuildPerspective(stackbuf, a);
        void* b = Scn_GetCamItem9C(mScn, -1);
        Coli_RenderChunks(getScnHandle(), b, stackbuf);
    }
}
