// Auto-scaffolded catalog TU for kyoshin/cf/CfNandManager
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/harness_catalog.hpp"

void __ct__cf_CfNandManager(){}

void __dt__8023BFCC(){}

void cf::CfNandManager::~CfNandManager() {}

void Init__Q22cf13CfNandManagerFv(void) {
    extern void func_804DA1B4(void*);
    extern char lbl_eu_8065FD00[];
    func_804DA1B4((void*)lbl_eu_8065FD00);
}

void cf::CfNandManager::Term() {}

void cf::CfNandManager::Move() {}

void func_8023C1B4(){}

void func_8023C1C0(){}

void func_8023C1F0(){}

void func_8023C2E4(){}

void func_8023C68C(){}

void func_8023C7C4(){}

void func_8023C93C(){}

void func_8023CD9C(){}

void func_8023D3D8(){}

void __dt__8023E448(){}

void cf::CfNandManager::func_8023E4D4() {}

void func_8023E544(){}

void __dt__8023E63C(){}

void func_8023EABC(){}

void func_8023EB78(){}

void func_8023F288(){}

void func_8023F2F4(){}

void func_8023F3C0(){}

void func_8023F5CC(){}

void func_8023F690(){}

void func_8023F860(){}

void func_8023FA64(){}

void func_8023FB28(){}

void func_8023FBA0(){}

void func_8023FC18(){}

void func_8023FCCC(){}

void func_8023FD4C(){}

void func_8023FEDC(){}

void cf::CfNandManager::create() {}

void func_8024005C(){}

void func_80240084(){}

void func_80240360(){}

void func_80240420(){}

void cf::CfNandManager::cbRenderBefore() {}

void OnFileEvent__Q22cf13CfNandManagerFv(void* self) { ((void(*)(void*))func_8023E4D4__Q22cf13CfNandManagerFv)((char*)self - 0x54); }

void func_802405FC__Q22cf13CfNandManagerFv(void* self) { ((void(*)(void*))__dt__Q22cf13CfNandManagerFv)((char*)self - 0x54); }

void func_80240604__Q22cf13CfNandManagerFv(void* self) { ((void(*)(void*))cbRenderBefore__Q22cf13CfNandManagerFv)((char*)self - 0x58); }

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
