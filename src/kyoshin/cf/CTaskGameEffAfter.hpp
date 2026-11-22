#pragma once

#include <types.h>
#include "monolib/work/CTTask.hpp"

// Runtime imports still used by CTaskREvtSequence.cpp, which includes this
// header for them (that unit hand-builds its constructor).
extern "C" long __ptmf_test(void* ptmf);

// Global null pointer-to-member-function constant (3 words).
extern u32 __ptmf_null[3];

class CTaskGameEffAfter : public CTTask<CTaskGameEffAfter> {
public:
    virtual ~CTaskGameEffAfter();

    static CTaskGameEffAfter* create(CProcess* parent);

    // Empty overrides (retail emits 4-byte `blr` bodies).
    virtual void Draw();
    virtual void Term();
    virtual void Init();
};