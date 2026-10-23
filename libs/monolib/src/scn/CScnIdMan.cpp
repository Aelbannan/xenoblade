// Decompiled for monolib/src/scn/CScnIdMan

#include <types.h>
#include "libs/monolib/src/scn/CScnIdMan.hpp"
#include "monolib/scn/code_804BC9EC.hpp"

extern "C" {
    extern u32 lbl_eu_8056EC80[];   // CScnIdMan vtable (defined below in this TU)
    extern void addRenderCB__4CScnFP10IScnRenderUlUl(void* pOwner, u32 scnId, u32 renderKind, u32 renderFlags);
    extern void removeRenderCB__4CScnFP10IScnRender(void* pOwner, u32 scnId);
    extern CScnIdPoolSlot* CScnItemPool_lookupSubPool(void* pPool, int slotIdx);
    extern void* Scn_FindCamItem(void* pParam, u32 camIdx);
    extern void* Scn_GetCamItem9C(void* pParam, u32 camIdx);
    extern void CamMan_BuildPerspective(void* pOut, void* pCamItem);
    extern void Coli_RenderChunks(void* pScnHandle, void* pCamItem, void* pStack);
}

extern "C" CScnIdMan* __ct__CScnIdMan(CScnIdMan* self, void* param) {
    self->mParam = (CScnIdParam*)param;
    *(void**)self = (void*)lbl_eu_8056EC80;
    self->mFlag = 0;
    addRenderCB__4CScnFP10IScnRenderUlUl(param, (u32)self, 6, 0);
    return self;
}

CScnIdMan::~CScnIdMan() {
    *(void**)this = (void*)lbl_eu_8056EC80;
    removeRenderCB__4CScnFP10IScnRender(mParam, (u32)this);
}

extern "C" int func_8049E51C(const CScnIdMan* self) {
    // Fetch the scene-id pool slot and count its live nodes; return 1 if empty.
    // const self: MWCC hoists the first load (lwz r3,4(r3)) above the LR save
    // (load-hoist family lever; non-const emits stw-first - 3 structural).
    CScnIdNode* node;
    u32 count;
    CScnIdNode* anchor;
    CScnIdPoolSlot* slot = CScnItemPool_lookupSubPool(self->mParam->mPool, 5);

    anchor = slot->mAnchor;
    count = 0;
    node = anchor->next;
    while (node != anchor) {
        node = node->next;
        count++;
    }
    return static_cast<u32>(__cntlzw(count)) >> 5;
}

void CScnIdMan::cbRenderBefore() {
    if (mFlag) {
        void* a = Scn_FindCamItem(mParam, -1);
        u8 stackbuf[0x38];
        CamMan_BuildPerspective(stackbuf, a);
        void* b = Scn_GetCamItem9C(mParam, -1);
        Coli_RenderChunks(getScnHandle(), b, stackbuf);
    }
}

// ===== Dissolved monolibdata2 (blob surgery) data owned by this TU =====
// [.data] 0x8056EC80-0x8056EC90 (16 bytes) -- CScnIdMan vtable + RTTI typeinfo.
extern "C" u32 lbl_eu_80663A78;  // .sdata RTTI typeinfo (foreign TU)
extern "C" void __dt__9CScnIdManFv();   // this-TU dtor symbol
extern "C" void cbRenderBefore__9CScnIdManFv();  // this-TU member symbol
// [.data] 0x8056EC80-0x8056EC90 (16 bytes) -- CScnIdMan vtable + RTTI typeinfo.
extern "C" u32 lbl_eu_8056EC80[0x4] = {
    (u32)&lbl_eu_80663A78,
    0x00000000,
    (u32)&__dt__9CScnIdManFv,
    (u32)&cbRenderBefore__9CScnIdManFv,
};

// [.rodata] 0x80524238-0x80524248 (16 bytes) "CScnIdMan" string (RTTI name).
extern "C" const char lbl_eu_80524238[0x10] = {
    0x43,0x53,0x63,0x6e,0x49,0x64,0x4d,0x61,0x6e,0x00,0x00,0x00,0x00,0x00,0x00,0x00 };

// (retired DECOMP_FORCEACTIVE keep-alives: the vtable is referenced by the
// ctor/dtor above; the RTTI name string is a section-defined global.)

