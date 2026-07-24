#pragma once

/**
 * @file code_800F42AC.hpp
 * @brief Class definition for the TU kyoshin/cf/code_800F42AC.
 *
 * This class contains methods from func_800F42AC through func_800F4798,
 * all of which are in the cf namespace and precede CfObjEnumList.
 *
 * FULL_MATCH symbols in this TU:
 *   func_800F477C -- getter for sub-object pointer at offset 0x0C
 */

#include "types.h"

namespace cf {

// Forward declaration for the sub-object type returned by func_800F477C.
// The returned object has fields at 0x42 (byte) and 0x78 (word flags).
struct CfUnknownSub;

/// Object class for code_800F42AC translation unit methods.
/// TODO: Rename once class identity is recovered via symbol recovery.
struct CfCode800F42AC {
    u8 pad_00[0xC];              // 0x00 - unknown padding
    CfUnknownSub* subObject;     // 0x0C - pointer to sub-object (CfGimmick or similar)
};

} // namespace cf
