#pragma once

#include <types.h>
#include "monolib/work/CTTask.hpp"

namespace cf {

// CfTaskMain also inherits from IWorkEvent (secondary vtable at 0x54),
// but that base is omitted from the C++ declaration to avoid compiler-generated
// ~IWorkEvent() call in the dtor (retail only calls CProcess::~CProcess()).
// The IWorkEvent vtable data comes from the retail assembly (split1.s).
class CfTaskMain : public CTTask<CfTaskMain> {
public:
    virtual ~CfTaskMain();
    void Move();
    void Draw();
    void Tail();

    void Init();
    void Term();

    /* 0x54 */ void* mIWorkEventVtable;
    /* 0x58 */ u32 field_0x58;
};
} // namespace cf

