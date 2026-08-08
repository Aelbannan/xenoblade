// Auto-scaffolded catalog TU for kyoshin/menu/CMenuTitle
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/harness_catalog.hpp"

#include "kyoshin/menu/CMenuTitle.hpp"
void __ct__CMenuTitle(){}

void func_802B5F0C(){}

void func_802B5F18(){}

void func_802B5F58(void* self) {
    extern void func_802B75B8(void*);
    ((unsigned char*)self)[0xe8] = 3;
    func_802B75B8((char*)self + 0x60);
}

void func_802B5F68(){}

void func_802B5FA8(){}

void func_802B6020(){}

void func_802B60CC(){}

void func_802B6200(){}

void func_802B6240(){}

extern "C" void func_802B6280(void) {}

extern "C" void func_802B6314(void* self) { ((void(*)(void*))__dt__10CMenuTitleFv)((char*)self - 0x54); }

extern "C" void func_802B631C(void* self) { ((void(*)(void*))cbRenderBefore__10CMenuTitleFv)((char*)self - 0x58); }

extern "C" void func_802B6324(void* self) { ((void(*)(void*))__dt__10CMenuTitleFv)((char*)self - 0x58); }

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

class CMenuTitle;
template<> void CTTask<CMenuTitle>::Move() {}
template<> void CTTask<CMenuTitle>::Draw() {}
