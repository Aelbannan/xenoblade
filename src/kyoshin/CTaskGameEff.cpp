// Auto-scaffolded catalog TU for kyoshin/CTaskGameEff
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/harness_catalog.hpp"

extern "C" void __ct__CTaskGameEff() {}

extern "C" void __dt__80044BB0() {}

extern "C" void __dt__Q212CTaskGameEff18CEffRenderHighPrioFv() {}

extern "C" void __dt___reslist_base_CScn() {}

extern "C" void __dt__reslist_CScn() {}

extern "C" void __dt__12CTaskGameEffFv() {}

extern "C" bool func_80044DF4() { return false; }

extern "C" void Init__12CTaskGameEffFv() {}

extern "C" void Term__12CTaskGameEffFv() {}

extern "C" void Move__12CTaskGameEffFv(void* self) {
    extern void func_804CBB14(void*, float);
    extern u8 lbl_eu_8065FC18[];
    extern float lbl_eu_80665D94;
    if (*(int*)((char*)self + 104) & 2) {
        func_804CBB14(lbl_eu_8065FC18, lbl_eu_80665D94);
    } else {
        func_804CBB14(lbl_eu_8065FC18, *(float*)((char*)self + 108));
    }
}

extern "C" void func_80044FBC__FUl() {}

extern "C" void func_804CBB60(void*);
extern u8 lbl_eu_8065FC18[];
extern "C" void cbRenderBefore__12CTaskGameEffFv() {
    func_804CBB60(lbl_eu_8065FC18);
}

extern "C" void func_80045044() {}

extern "C" void func_800450C8() {}

extern "C" void func_800450CC() {}

extern "C" void func_8004513C() {}

extern "C" void func_800451D8() {}

extern "C" void func_80045284() {}

extern "C" void func_800452EC() {}

extern "C" void func_800453EC() {}

extern "C" void Draw__12CTaskGameEffFv() {}

extern "C" bool func_80045540() { return false; }

extern "C" bool func_80045548() { return false; }

extern "C" bool func_80045550() { return false; }

extern "C" bool func_80045558() { return false; }

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

class CTaskGameEff;
template<> CTTask<CTaskGameEff>::~CTTask() {}
template<> void CTTask<CTaskGameEff>::Move() {}
template<> void CTTask<CTaskGameEff>::Draw() {}
