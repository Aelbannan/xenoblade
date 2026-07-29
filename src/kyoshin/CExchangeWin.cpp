// Auto-scaffolded catalog TU for kyoshin/CExchangeWin
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/harness_catalog.hpp"
#include <stdio.h>
#include "kyoshin/CExchangeWin.hpp"
#include "kyoshin/code_80135FDC.hpp"
extern const char lbl_eu_8050A740[];
extern void func_80137924(void*, void*, void*, void*);
extern void func_80138078(u32);

extern "C" u8 func_8022D08C(void* self) { return ((CExchangeWinFull*)self)->field_25; }







extern "C" u8 func_8022D094(void* self) { return ((CExchangeWinFull*)self)->field_24; }

extern "C" u8 func_8022D09C(void* self) { return ((CExchangeWinFull*)self)->field_27; }





void func_8022D1F8(){}

void func_8022D244(){}

void CExchangeWin::OnFileEvent() {}

// Stub functions needed by CItemBoxGrid
extern "C" void func_8022D0D0(void* self) {
    CExchangeWinFull* s = (CExchangeWinFull*)self;
    if (s->_26 != 2) {
        return;
    }
    s->_26 = 3;
    s->field_27 = 0;
    func_80138078(0xe);
}
extern "C" void func_8022D0F8(void* dst, void* src, u8 val) {
    char buf[64];
    sprintf(buf, &lbl_eu_8050A740[0x18], val + 1);
    u32 obj = *(u32*)((u8*)src + 0x1c);
    u32 sub = *(u32*)(obj + 0x10);
    void** vtbl = *(void***)sub;
    void* r1 = ((void*(*)(void*, char*, int))vtbl[0x3C / 4])((void*)sub, buf, 1);
    u32 obj2 = *(u32*)((u8*)src + 0x1c);
    u32 sub2 = *(u32*)(obj2 + 0x10);
    void** vtbl2 = *(void***)sub2;
    void* r2 = ((void*(*)(void*, char*, int))vtbl2[0x3C / 4])((void*)sub2, (char*)&lbl_eu_8050A740[0x25], 1);
    u32 obj3 = *(u32*)((u8*)src + 0x1c);
    u32 sub3 = *(u32*)(obj3 + 0x10);
    func_80137924(dst, r1, r2, (void*)sub3);
}
extern "C" void func_8022CF2C(void* self) { }
extern "C" void func_8022CF7C(void* self) { }
extern "C" void func_8022CFEC(void* self) {
    CExchangeWinFull* s = (CExchangeWinFull*)self;
    if (s->field_24 == 0) {
        return;
    }
    if (s->_26 == 0) {
        return;
    }
    func_80137038(s->mLayout, NULL, 0, 1);
}
