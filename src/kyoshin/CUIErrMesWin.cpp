// Auto-scaffolded catalog TU for kyoshin/CUIErrMesWin
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/harness_catalog.hpp"

#include "kyoshin/CUIErrMesWin.hpp"

void func_802B5130(void* self) { ((void(*)(void*))__dt__12CUIErrMesWinFv)((char*)self - 0x54); }


void CUIErrMesWin::Move() {}

void CUIErrMesWin::cbRenderBefore() {}

void __ct__802B4DF4(){}

void func_802B4EF4(void){}

void func_802B4F00(){}

void func_802B4F40(){}

void func_802B4FA8(){}

void func_802B4FE8(){}

void func_802B5054(){}

void func_802B5094(void){}


void func_802B5138(void* self) { ((void(*)(void*))cbRenderBefore__12CUIErrMesWinFv)((char*)self - 0x58); }

void func_802B5140(void* self) { ((void(*)(void*))__dt__12CUIErrMesWinFv)((char*)self - 0x58); }

void func_802B5148(){}

void func_802B515C(){}

void func_802B5254(){}

void func_802B58A4(){}

void func_802B5970(){}

void func_802B5AC8(){}

extern "C" void __dt__10CMenuTitleFv() {}

extern "C" void Init__10CMenuTitleFv() {}

extern "C" void Term__10CMenuTitleFv() {}

extern "C" void Move__10CMenuTitleFv() {}

extern "C" void cbRenderBefore__10CMenuTitleFv() {}

// --- hard-symbol stubs (scaffold_hard_symbols) ---
// Local CTTask (out-of-line Move/Draw/dtor) for harness stubs.
// Do not include monolib/work/CTTask.hpp here - its inline methods collide.
template <typename T>
class CTTask {
public:
    CTTask();
    virtual ~CTTask();
    virtual void Move();
    virtual void Draw();
};

class CUIErrMesWin;
template<> void CTTask<CUIErrMesWin>::Move() {}
template<> void CTTask<CUIErrMesWin>::Draw() {}
class CMenuTitle;
template<> CTTask<CMenuTitle>::~CTTask() {}

extern "C" void func_802B48A0() {}
extern "C" void func_802B48B8() {}
extern "C" void func_802B48E4() {}
extern "C" void func_802B4968() {}
extern "C" void func_802B4A68() {}
extern "C" void func_802B4B84() {}
