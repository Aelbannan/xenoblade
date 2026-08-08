#pragma once

#include <types.h>
#include "monolib/work/CProcess.hpp"

// C-linkage runtime imports (retail symbol names - keep linkage/signatures verbatim)
extern "C" long __ptmf_test(void* ptmf);
// Raw CProcess constructor (CProcess is abstract, so it cannot be placement-\n\'d).
extern "C" void __ct__8CProcessFv(void* self);

// Global null pointer-to-member-function constant (3 words).
extern u32 __ptmf_null[3];

// Retail vtables in .data. lbl_eu_80526650 is the interim CTTask<CTaskGameEffAfter>
// vtable written after the base ctor; lbl_eu_80526608 is the final
// CTaskGameEffAfter vtable written at the end of construction.
extern const u8 lbl_eu_80526650[];
extern const u8 lbl_eu_80526608[];

// Local CRTP task base (mirrors monolib/work/CTTask.hpp but with out-of-line
// template specializations emitted in the unit cpp so they produce the retail
// Move/Draw/dtor symbols). Member-function-pointer callbacks are dispatched via
// the retail __ptmf_test/__ptmf_scall machinery.
template <typename TDerived>
class CTTask : public CProcess {
public:
    CTTask() : mMoveFunc(nullptr), mDrawFunc(nullptr) {}
    virtual ~CTTask();
    virtual void Move();
    virtual void Draw();

protected:
    //0x0-0x10: CDoubleListNode
    //0x10: vtable
    //0x14-0x3C: CProcess
    void (TDerived::*mMoveFunc)();  //0x3C
    void (TDerived::*mDrawFunc)();  //0x48
}; // size 0x54

class CTaskGameEffAfter : public CTTask<CTaskGameEffAfter> {
public:
    virtual ~CTaskGameEffAfter();

    // Empty overrides (retail emits 4-byte `blr` bodies).
    virtual void Draw();
    virtual void Term();
    virtual void Init();
};