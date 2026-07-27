// Auto-scaffolded catalog TU for kyoshin/CTaskGameEff
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/harness_catalog.hpp"

void __ct__CTaskGameEff(){}

void __dt__80044BB0(){}

void __dt__Q212CTaskGameEff18CEffRenderHighPrioFv(){}

void __dt___reslist_base_CScn(){}

void __dt__reslist_CScn(){}

CTaskGameEff::~CTaskGameEff() {}

bool func_80044DF4() { return false; }

void CTaskGameEff::Init() {}

void CTaskGameEff::Term() {}



void func_80044FBC__FUl(){}

void func_804CBB60(void*);
extern u8 lbl_eu_8065FC18[];
void cbRenderBefore__12CTaskGameEffFv() {
    func_804CBB60(lbl_eu_8065FC18);
}

void func_80045044(){}

void func_800450C8() {}

void func_800450CC(){}

void func_8004513C(){}

void func_800451D8(){}

void func_80045284(){}

void func_800452EC(){}

void func_800453EC(){}



bool func_80045540(){ return false; }

bool func_80045548(){ return false; }

bool func_80045550(){ return false; }

bool func_80045558(){ return false; }

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
