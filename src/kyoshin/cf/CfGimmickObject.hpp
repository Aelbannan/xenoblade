#pragma once

/**
 * @file CfGimmickObject.hpp
 * @brief cf::CfGimmickObject -- script/OC gimmick object class.
 *
 * CfGimmickObject is a gimmick entity managed by the cf script VM.
 * Most functions are still placeholder until symbol recovery.
 *
 * FULL_MATCH symbols in this TU:
 *   func_801F8564 -- returns constant 1 (8-byte leaf)
 */

#include "types.h"

namespace cf {

class CfGimmickObject {
public:
    CfGimmickObject();
    ~CfGimmickObject();
};

} // namespace cf

// Extern "C" exports -- linker names must match retail symbols.
extern "C" int func_801F8564();
