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

// C-linkage imports (retail symbol names - keep linkage/signatures verbatim)
extern "C" void __ct__8CProcessFv(void* self);
extern "C" void func_8007F9C4__Q22cf13CfGameManagerFv(void);
extern "C" void func_8007FBFC__Q22cf13CfGameManagerFv(void);
extern "C" void func_8007FC2C__Q22cf13CfGameManagerFv(void);
extern "C" unsigned long lbl_eu_80663E28;
extern "C" void __dt__Q22cf10CfTaskMainFv(void* self);

