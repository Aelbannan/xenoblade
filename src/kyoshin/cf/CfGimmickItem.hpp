#pragma once

/**
 * @file CfGimmickItem.hpp
 * @brief cf::CfGimmickItem -- item gimmick entity class.
 *
 * CfGimmickItem extends CfGimmick to represent items that can be
 * picked up / collected in the game world (recovery items, key items,
 * etc.). Fields are populated from BDAT columns at construction.
 *
 * FULL_MATCH symbols in this TU:
 *   func_80210C1C -- vtable override CfGimmickItem[+?] (empty no-op)
 */

#include "types.h"

namespace cf {

class CfGimmick; // Forward decl -- CfGimmick.hpp TODO

class CfGimmickItem {
public:
    CfGimmickItem();
    ~CfGimmickItem();
};

} // namespace cf

// Extern "C" exports -- linker names must match retail symbols.
extern "C" void func_80210C1C();
