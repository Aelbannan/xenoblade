// Auto-scaffolded catalog TU for kyoshin/menu/CMenuSymbolMark
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/harness_catalog.hpp"

void func_8011D338(){}

void __ct__CMenuSymbolMark(){}

void CMenuSymbolMark::~CMenuSymbolMark() {}

void CMenuSymbolMark::Init() {}

void CMenuSymbolMark::Term() {}

void CMenuSymbolMark::Move() {}

void CMenuSymbolMark::cbRenderBefore() {}

void func_8011E4C4(){}

void func_8011E540(){}

void func_8011E778(){}

void func_8011EA98(){}

void func_8011EBA8(){}

void func_8011EC94(){}

void func_8011EDDC(){}

void func_8011EFB0(){}

void func_8011F8F8(){}

void func_8011FB68(){}

void func_801209BC(){}

void CArrow3D::~CArrow3D() {}

void CArrow3D::cbRenderBefore() {}

extern "C" void Move__8CArrow3DFv(void) {}

void CArrow3D::Term() {}

void CArrow3D::Init() {}

extern "C" void func_8012213C(void* self) { ((void(*)(void*))__dt__15CMenuSymbolMarkFv)((char*)self - 0x58); }

extern "C" void func_80122144(void* self) { ((void(*)(void*))cbRenderBefore__15CMenuSymbolMarkFv)((char*)self - 0x5c); }

extern "C" void func_8012214C(void* self) { ((void(*)(void*))__dt__15CMenuSymbolMarkFv)((char*)self - 0x5c); }

extern "C" void func_80122154(void* self) { ((void(*)(void*))cbRenderBefore__8CArrow3DFv)((char*)self - 0x54); }

extern "C" void func_8012215C(void* self) { ((void(*)(void*))__dt__8CArrow3DFv)((char*)self - 0x54); }

// --- hard-symbol stubs (scaffold_hard_symbols) ---
// Local CTTask (out-of-line Move/Draw/dtor) for harness stubs.
// Do not include monolib/work/CTTask.hpp here — its inline methods collide.
template <typename T>
class CTTask {
public:
    CTTask();
    virtual ~CTTask();
    virtual void Move();
    virtual void Draw();
};

class CArrow3D;
template<> CTTask<CArrow3D>::~CTTask() {}
template<> void CTTask<CArrow3D>::Move() {}
template<> void CTTask<CArrow3D>::Draw() {}
