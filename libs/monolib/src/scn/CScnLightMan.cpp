// Decompiled for monolib/src/scn/CScnLightMan

#include <types.h>
#include "libs/monolib/src/scn/CScnLightMan.hpp"

extern "C" {
    extern char lbl_eu_8056E568[];   // CScnLightMan vtable
    extern CScnItemLight* func_80482398(CScnLightParam* self, int val, CScnLightMan* owner);
    extern CScnLightList* func_8048C698(void* self, int kind);
    extern void func_804954AC(void* self, int val);
    extern void func_804959E8(void* self, int val);
}

// Selects the active light: if the current pool light is armed, deactivate
// its param block (func_804959E8) and clear the arm flag.
void func_8048D160(CScnLightMan* self) {
    CScnItemLight* light = self->mLight;
    if (light->flag121C) {
        func_804959E8(&light->_14[0], 0);
        self->mLight->flag121C = 0;
    }
}

CScnLightMan::CScnLightMan(CScnLightParam* param) {
    *(void**)this = (void*)lbl_eu_8056E568;
    mParam = param;
    mLight = func_80482398(param, 0, this);

    // Walk the scene pool's light-item list; keep the first item whose id is
    // zero as the active light (the one that gets armed by func_804954AC).
    CScnLightList* list = func_8048C698(mParam->mPool, 3);
    CScnItemLight* item;                 // declared before node -> r30 (retail)
    CScnLightNode* node = list->sentinel->next;
    while (node != list->sentinel) {
        item = node->item;
        func_804954AC(&item->_14[0], (u32)__cntlzw(item->id10) >> 5);
        if (item->id10 == 0) {
            mLight = item;
        }
        node = node->next;
    }
}

CScnLightMan::~CScnLightMan() {}

void func_8048D124(void* arg1, void* arg2) {
    extern void func_804BF944(void*);
    func_804BF944(arg2);
    char* ptr = *(char**)((char*)arg1 + 8);
    ptr[0x121c] = 1;
}

// Virtual dispatch target: v_i at vtable offset 8+4*i (MWCC RTTI header).
struct VTarget {
    virtual void v0() = 0;
    virtual void v1() = 0;
    virtual void v2() = 0;
    virtual void v3() = 0;
    virtual void v4() = 0;
    virtual void v5() = 0;
    virtual void v6() = 0;
    virtual void v7() = 0;
    virtual void v8() = 0;
    virtual void v9() = 0;
    virtual void v10() = 0;
    virtual ~VTarget() {}
};

extern "C" void func_8048D1B0(void* self) {
    VTarget* obj = (VTarget*)*(void**)((u8*)self + 8);
    obj->v3();
}