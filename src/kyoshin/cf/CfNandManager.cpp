// Auto-scaffolded catalog TU for kyoshin/cf/CfNandManager
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/harness_catalog.hpp"

extern "C" void __ct__cf_CfNandManager() {}

extern "C" void __dt__8023BFCC() {}

extern "C" void __dt__Q22cf13CfNandManagerFv(void* self) {}

extern "C" void Init__Q22cf13CfNandManagerFv(void) {
    extern void func_804DA1B4(void*);
    extern char lbl_eu_8065FD00[];
    func_804DA1B4((void*)lbl_eu_8065FD00);
}

extern "C" void Term__Q22cf13CfNandManagerFv() {}

extern "C" void Move__Q22cf13CfNandManagerFv() {}

extern "C" void func_8023C1B4() {}

extern "C" void func_8023C1C0() {}

extern "C" void func_8023C1F0() {}

extern "C" void func_8023C2E4() {}

extern "C" void func_8023C68C() {}

extern "C" void func_8023C7C4() {}

extern "C" void func_8023C93C() {}

extern "C" void func_8023CD9C() {}

extern "C" void func_8023D3D8() {}

extern "C" void __dt__8023E448() {}

extern "C" void func_8023E4D4__Q22cf13CfNandManagerFv(void* self) {}

extern "C" void func_8023E544() {}

extern "C" void __dt__8023E63C() {}

extern "C" void func_8023EABC() {}

extern "C" void func_8023EB78() {}

extern "C" void func_8023F288() {}

extern "C" void func_8023F2F4() {}

extern "C" void func_8023F3C0() {}

extern "C" void func_8023F5CC() {}

extern "C" void func_8023F690() {}

extern "C" void func_8023F860() {}

extern "C" void func_8023FA64() {}

extern "C" void func_8023FB28() {}

extern "C" void func_8023FBA0() {}

extern "C" void func_8023FC18() {}

extern "C" void func_8023FCCC() {}

extern "C" void func_8023FD4C() {}

extern "C" void func_8023FEDC() {}

extern "C" void create__Q22cf13CfNandManagerFv() {}

extern "C" void func_8024005C() {}

extern "C" void func_80240084() {}

extern "C" void func_80240360() {}

extern "C" void func_80240420() {}

extern "C" void cbRenderBefore__Q22cf13CfNandManagerFv(void* self) {}

extern "C" void OnFileEvent__Q22cf13CfNandManagerFv(void* self) { ((void(*)(void*))func_8023E4D4__Q22cf13CfNandManagerFv)((char*)self - 0x54); }

extern "C" void func_802405FC__Q22cf13CfNandManagerFv(void* self) { ((void(*)(void*))__dt__Q22cf13CfNandManagerFv)((char*)self - 0x54); }

extern "C" void func_80240604__Q22cf13CfNandManagerFv(void* self) { ((void(*)(void*))cbRenderBefore__Q22cf13CfNandManagerFv)((char*)self - 0x58); }

extern "C" void func_8024060C__Q22cf13CfNandManagerFv(void* self) { ((void(*)(void*))__dt__Q22cf13CfNandManagerFv)((char*)self - 0x58); }

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

namespace cf { class CfNandManager; }
template<> CTTask<cf::CfNandManager>::~CTTask() {}
template<> void CTTask<cf::CfNandManager>::Move() {}
template<> void CTTask<cf::CfNandManager>::Draw() {}
extern "C" void sinit_802405F0(void) {}
