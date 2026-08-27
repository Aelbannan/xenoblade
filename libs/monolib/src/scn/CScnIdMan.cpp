// Decompiled for monolib/src/scn/CScnIdMan

#include <types.h>
#include "libs/monolib/src/scn/CScnIdMan.hpp"

extern "C" {
    extern void __dl__FPv(void* ptr);
    extern u32 lbl_eu_8056EC80[];   // CScnIdMan vtable (defined below in this TU)
    extern void addRenderCB__4CScnFP10IScnRenderUlUl(void* r3, u32 r4, u32 r5, u32 r6);
    extern void removeRenderCB__4CScnFP10IScnRender(void* r3, u32 r4);
    extern CScnIdPoolSlot* func_8048C698(void* r3, int r4);
    extern void* func_80496264(void* r3, u32 r4);
    extern void* func_804961D4(void* r3, u32 r4);
    extern void func_8049B764(void* r3, void* r4);
    extern void* getScnHandle__Fv();
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
        func_804BCC78(getScnHandle__Fv(), b, stackbuf);
    }
}

// ===== Dissolved monolibdata2 (blob surgery) data owned by this TU =====
// [.data] 0x8056EC80-0x8056EC90 (16 bytes) — CScnIdMan vtable + RTTI typeinfo.
extern "C" u32 lbl_eu_80663A78;  // .sdata RTTI typeinfo (foreign TU)
extern "C" void __dt__9CScnIdManFv();   // this-TU dtor symbol
extern "C" void cbRenderBefore__9CScnIdManFv();  // this-TU member symbol
// [.data] 0x8056EC80-0x8056EC90 (16 bytes) — CScnIdMan vtable + RTTI typeinfo.
extern "C" u32 lbl_eu_8056EC80[0x4] = {
    (u32)&lbl_eu_80663A78,
    0x00000000,
    (u32)&__dt__9CScnIdManFv,
    (u32)&cbRenderBefore__9CScnIdManFv,
};

// [.rodata] 0x80524338-0x80524348 (16 bytes) "CScnIdMan" string (RTTI name).
extern "C" const char lbl_eu_80524338[0x10] = {
    0x43,0x53,0x63,0x6e,0x49,0x64,0x4d,0x61,0x6e,0x00,0x00,0x00,0x00,0x00,0x00,0x00 };

// (retired DECOMP_FORCEACTIVE keep-alives: the vtable is referenced by the
// ctor/dtor above; the RTTI name string is a section-defined global.)

