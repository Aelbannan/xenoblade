// Decompiled for monolib/src/scn/CScnIdMan

#include <types.h>
#include "libs/monolib/src/scn/CScnIdMan.hpp"

extern "C" {
    extern void __dl__FPv(void* ptr);
    extern char lbl_eu_8056EC80[];   // CScnIdMan vtable
    extern void addRenderCB__4CScnFP10IScnRenderUlUl(void* r3, u32 r4, u32 r5, u32 r6);
    extern void removeRenderCB__4CScnFP10IScnRender(void* r3, u32 r4);
    extern void* func_8048C698(void* r3, u32 r4);
    extern void* func_80496264(void* r3, u32 r4);
    extern void* func_804961D4(void* r3, u32 r4);
    extern void func_8049B764(void* r3, void* r4);
    extern void* func_804BC9EC__Fv();
    extern void func_804BCC78(void* r3, void* r4, void* r5);
}

extern "C" CScnIdMan* __ct__CScnIdMan(CScnIdMan* self, void* param) {
    self->mParam = param;
    *(void**)self = (void*)lbl_eu_8056EC80;
    self->mFlag = 0;
    addRenderCB__4CScnFP10IScnRenderUlUl(param, (u32)self, 6, 0);
    return self;
}

CScnIdMan::~CScnIdMan() {
    *(void**)this = (void*)lbl_eu_8056EC80;
    removeRenderCB__4CScnFP10IScnRender(mParam, (u32)this);
}

extern "C" int func_8049E51C(CScnIdMan* self) {
    void* target = *(void**)((char*)self->mParam + 0x60);
    void* obj = func_8048C698(target, 5);
    void* list = *(void**)((char*)obj + 4);
    int count = 0;
    void* node = *(void**)list;
    while (node != list) {
        node = *(void**)node;
        count++;
    }
    return count == 0;
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
