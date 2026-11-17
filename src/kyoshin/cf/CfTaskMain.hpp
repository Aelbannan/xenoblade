#pragma once

#include <types.h>
#include "monolib/work/CTTask.hpp"
#include "monolib/work/IWorkEvent.hpp"

namespace cf {

class CfTaskMain : public CTTask<CfTaskMain>, public IWorkEvent {
public:
    virtual ~CfTaskMain();
    void Move();
    void Draw();
    void Tail();

    void Init();
    void Term();

    /* 0x54: IWorkEvent vptr */
    /* 0x58 */ u32 field_0x58;
};
} // namespace cf

// C-linkage imports (retail symbol names - keep linkage/signatures verbatim)
extern "C" void __ct__8CProcessFv(void* self);
extern "C" void func_8007F9C4__Q22cf13CfGameManagerFv(void);
extern "C" void refreshCameraIfCurrentView__Q22cf13CfGameManagerFv(void);
extern "C" void resetCameraIfCurrentView__Q22cf13CfGameManagerFv(void);
extern "C" unsigned long lbl_eu_80663E28;

