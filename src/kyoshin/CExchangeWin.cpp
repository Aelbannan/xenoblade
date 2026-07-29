// Auto-scaffolded catalog TU for kyoshin/CExchangeWin
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/harness_catalog.hpp"
#include <stdio.h>
#include "kyoshin/CExchangeWin.hpp"
#include "kyoshin/code_80135FDC.hpp"
#include "monolib/device/CDeviceFile.hpp"
#include "monolib/util/MemManager.hpp"
extern const char lbl_eu_8050A740[];
extern void func_80137924(void*, void*, void*, void*);
extern void func_80138078(u32);
extern float lbl_eu_80668610;

extern "C" u8 func_8022D08C(void* self) { return ((CExchangeWinFull*)self)->field_25; }







extern "C" u8 func_8022D094(void* self) { return ((CExchangeWinFull*)self)->field_24; }

extern "C" u8 func_8022D09C(void* self) { return ((CExchangeWinFull*)self)->field_27; }

// If _26 is already non-zero, do nothing; otherwise initialize state and fire event 0xd
extern "C" void func_8022D0A4(CExchangeWinFull* self) {
    if (self->_26 != 0) {
        return;
    }
    self->_26 = 1;
    self->field_27 = 0;
    self->field_24 = 1;
    func_80138078(0xd);
}

extern "C" void func_8022D1F8(CExchangeWinFull* self) {
    float f = lbl_eu_80668610;
    u32 r = func_80137444(self->mAnimTransform, f);
    if (r) {
        self->_26 = 2;
        self->field_27 = 1;
    }
}

void func_8022D244(CExchangeWinFull* self) {
    float f = lbl_eu_80668610;
    u32 r = func_80137510(self->mAnimTransform, f);
    if (r) {
        self->_26 = 0;
        self->field_27 = 1;
        self->field_24 = 0;
    }
}

void CExchangeWin::OnFileEvent() {}

CExchangeWin::~CExchangeWin() {}

// Helper struct for layout sub-object at mLayout+0x10 (vtable dispatch target)
struct LayoutSubObj {
    u8 _00[0x10];
    u8* subObj;  // 0x10
};

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
    CExchangeWinFull* s = (CExchangeWinFull*)src;
    LayoutSubObj* lso = (LayoutSubObj*)s->mLayout;
    u8* pane = lso->subObj;
    u8* r1 = (u8*)((void*(*)(void*, char*, int))((void**)*(u32*)pane)[0x3C / 4])((void*)pane, buf, 1);
    lso = (LayoutSubObj*)s->mLayout;
    pane = lso->subObj;
    u8* r2 = (u8*)((void*(*)(void*, char*, int))((void**)*(u32*)pane)[0x3C / 4])((void*)pane, (char*)&lbl_eu_8050A740[0x25], 1);
    lso = (LayoutSubObj*)s->mLayout;
    func_80137924(dst, r1, r2, lso->subObj);
}
extern "C" void func_8022CF2C(CExchangeWinFull* self) {
    self->mFileHandle = CDeviceFile::readFile(
        mtl::MemManager::getHandleMEM2(),
        lbl_eu_8050A740,
        (IWorkEvent*)self,
        0,
        0
    );
    self->field_25 = 0;
}
extern "C" void func_8022CF7C(void* self) { }
extern "C" void func_8022CFEC(void* self, nw4r::lyt::DrawInfo* drawInfo) {
    CExchangeWinFull* s = (CExchangeWinFull*)self;
    if (s->field_24 == 0) {
        return;
    }
    if (s->_26 == 0) {
        return;
    }
    func_80137038(s->mLayout, drawInfo, 0, 1);
}

// Sets two text fields on the layout: one at string offset 0x34 with param2,
// and one at string offset 0x41 with param3.
void func_8022D19C(CExchangeWinFull* self, char* param2, char* param3) {
    func_80136B4C(self->mLayout, (char*)&lbl_eu_8050A740[0x34], param2, 0);
    func_80136B4C(self->mLayout, (char*)&lbl_eu_8050A740[0x41], param3, 0);
}
