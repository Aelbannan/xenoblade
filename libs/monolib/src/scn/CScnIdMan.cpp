// Decompiled for monolib/src/scn/CScnIdMan

#include <types.h>
#include "libs/monolib/src/scn/CScnIdMan.hpp"

extern "C" {
    extern void __dl__FPv(void* ptr);
    extern char lbl_eu_8056EC80[];   // CScnIdMan vtable
    extern void addRenderCB__4CScnFP10IScnRenderUlUl(void* r3, u32 r4, u32 r5, u32 r6);
    extern void removeRenderCB__4CScnFP10IScnRender(void* r3, u32 r4);
    extern CScnIdPoolSlot* func_8048C698(void* r3, int r4);
    extern void* func_80496264(void* r3, u32 r4);
    extern void* func_804961D4(void* r3, u32 r4);
    extern void func_8049B764(void* r3, void* r4);
    extern void* func_804BC9EC__Fv();
    extern void func_804BCC78(void* r3, void* r4, void* r5);
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
    CScnIdPoolSlot* slot = func_8048C698(self->mParam->mPool, 5);

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
        void* a = func_80496264(mParam, -1);
        u8 stackbuf[0x38];
        func_8049B764(stackbuf, a);
        void* b = func_804961D4(mParam, -1);
        func_804BCC78(func_804BC9EC__Fv(), b, stackbuf);
    }
}
