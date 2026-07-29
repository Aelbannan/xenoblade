// Auto-scaffolded catalog TU for kyoshin/CTaskGameEff
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/harness_catalog.hpp"
#include "monolib/work/CProcess.hpp"

void __ct__CTaskGameEff(){}

void __dt__80044BB0(){}

void __dt__Q212CTaskGameEff18CEffRenderHighPrioFv(){}

template <> _reslist_base<CScn>::~_reslist_base() {}
template <> reslist<CScn>::~reslist() {}

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
class CTaskGameEff;

template <typename T>
class CTTask : public CProcess {
public:
    typedef void (CProcess::*MoveFunc)();
    typedef void (CProcess::*DrawFunc)();

    CTTask();
    virtual ~CTTask();
    virtual void Move();
    virtual void Draw();

protected:
    MoveFunc mMoveFunc;  // 0x3C — pointer-to-member-function (12 bytes)
    DrawFunc mDrawFunc;  // 0x48 — pointer-to-member-function (12 bytes)
};

// CTTask<CTaskGameEff> constructor — out-of-line
template<>
CTTask<CTaskGameEff>::CTTask() : mMoveFunc(nullptr), mDrawFunc(nullptr) {}

// CTTask<CTaskGameEff>::~CTTask — dtor body is empty; compiler emits:
//   null check → CProcess::~CProcess(this, 0) → conditional operator delete
// #pragma optimize_for_size on keeps stmw r30 instead of individual stw.
#pragma optimize_for_size on
template<>
CTTask<CTaskGameEff>::~CTTask() {}
#pragma optimize_for_size off

// CTTask<CTaskGameEff>::Move — test PTMF at +0x3C, call if non-null
template<>
void CTTask<CTaskGameEff>::Move() {
    if (mMoveFunc) {
        (this->*mMoveFunc)();
    }
}

// CTTask<CTaskGameEff>::Draw — test PTMF at +0x48, call if non-null
template<>
void CTTask<CTaskGameEff>::Draw() {
    if (mDrawFunc) {
        (this->*mDrawFunc)();
    }
}
