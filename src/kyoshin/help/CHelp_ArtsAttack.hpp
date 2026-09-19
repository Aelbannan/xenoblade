#pragma once

#include <types.h>
#include "kyoshin/help/CHelp.hpp"

namespace cf {

class __declspec(novtable) CHelp_ArtsAttack : public CHelpSwitch {
public:
    u32 checkHelpCondition();
};

} // namespace cf

// CfObjectPc+4 foreign sub-object: vtable slot 0x30 returns a u32* word holder
// (same shape as CtrlEnemyActor4 / MWCC_PATTERNS PcSub4VtIf). Never instantiated.
class PcSub4VtIf {
public:
    virtual void b008(); virtual void b00C(); virtual void b010();
    virtual void b014(); virtual void b018(); virtual void b01C();
    virtual void b020(); virtual void b024(); virtual void b028();
    virtual void b02C();
    virtual void* _q030(); // index 10 -> vtable offset 0x30
};

// C-linkage imports (retail symbol names - keep linkage/signatures verbatim)
