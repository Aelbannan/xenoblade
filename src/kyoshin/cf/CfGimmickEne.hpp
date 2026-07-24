#pragma once

/**
 * @file CfGimmickEne.hpp
 * @brief cf::CfGimmickEne -- enemy gimmick entity class.
 *
 * CfGimmickEne extends CfGimmick for enemy-specific gimmick behaviour.
 * Fields are populated from BDAT columns at construction.
 *
 * FULL_MATCH symbols in this TU:
 *   func_8026E5BC -- vtable override CfGimmickEne[+0x20] (empty no-op)
 *   func_8026F31C -- dispatch table entry [5]           (empty no-op)
 */

#include "types.h"

namespace cf {

class CfGimmick; // Forward decl -- CfGimmick.hpp TODO

class CfGimmickEne {
public:
    CfGimmickEne();
    ~CfGimmickEne();
};

} // namespace cf

// Extern "C" exports -- linker names must match retail symbols.
extern "C" void func_8026E5BC();
extern "C" void func_8026F31C();
