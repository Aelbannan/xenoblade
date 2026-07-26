// Auto-scaffolded catalog TU for kyoshin/cf/CfTaskMain
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/harness_catalog.hpp"

extern "C" void __dt__Q22cf10CfTaskMainFv(void* self) {}

extern "C" void Init__Q22cf10CfTaskMainFv() {}

extern "C" void Term__Q22cf10CfTaskMainFv() {}

extern "C" void Move__Q22cf10CfTaskMainFv() {
    extern void func_8007F9C4__Q22cf13CfGameManagerFv(void);
    extern unsigned long lbl_eu_80663E28;
    if ((lbl_eu_80663E28 & 0x4000) != 0) return;
    func_8007F9C4__Q22cf13CfGameManagerFv();
}

extern "C" void Draw__Q22cf10CfTaskMainFv(void) {}

extern "C" void Tail__Q22cf10CfTaskMainFv(void) {}

extern "C" void __ct__800697E8() {}

extern "C" void func_80069944(void* self) { ((void(*)(void*))__dt__Q22cf10CfTaskMainFv)((char*)self - 0x54); }

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

namespace cf { class CfTaskMain; }
template<> CTTask<cf::CfTaskMain>::~CTTask() {}
template<> void CTTask<cf::CfTaskMain>::Move() {}
template<> void CTTask<cf::CfTaskMain>::Draw() {}
