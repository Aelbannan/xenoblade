#pragma once

#include <types.h>
#include "monolib/work/CTTask.hpp"

// C-linkage runtime imports (retail symbol names - keep linkage/signatures verbatim)
extern "C" long __ptmf_test(void* ptmf);
// Raw CProcess constructor (CProcess is abstract, so it cannot be placement-\n\'d).
extern "C" void __ct__8CProcessFv(CProcess* self);

// Global null pointer-to-member-function constant (3 words).
extern u32 __ptmf_null[3];

// Retail vtables in .data. lbl_eu_80526650 is the interim CTTask<CTaskGameEffAfter>
// vtable written after the base ctor; lbl_eu_80526608 is the final
// CTaskGameEffAfter vtable written at the end of construction.
extern const u8 lbl_eu_80526650[];
extern const u8 lbl_eu_80526608[];

// CRTP task base - canonical monolib template (declared-only members so the
// unit cpp can emit the retail out-of-line Move/Draw/dtor symbols via explicit
// `template<>` specializations). Member-function-pointer callbacks are
// dispatched via the retail __ptmf_test/__ptmf_scall machinery.

class CTaskGameEffAfter : public CTTask<CTaskGameEffAfter> {
public:
    virtual ~CTaskGameEffAfter();

    // Empty overrides (retail emits 4-byte `blr` bodies).
    virtual void Draw();
    virtual void Term();
    virtual void Init();
};