#pragma once

#include <types.h>

namespace LOD {

// Minimal definition for the embedded sub-object at LODMemMan+0xabc.
// Full layout is in code_8046A530 TU; only methods called from LODMemMan
// are declared here.
class UnkClass_8046A530 {
public:
    void func_8046AADC();
};

/**
 * LOD memory manager.  Large object; only fields used by matched
 * functions are declared here.  Remaining layout is unknown padding.
 */
class LODMemMan {
public:
    float func_8046F01C();
    void func_8046F088();

private:
    u8  _00[0x7c];                    // unknown leading fields
    f32 mFloat_7C;                    // offset 0x7c
    u8  _80[0xabc - 0x80];            // unknown padding
    UnkClass_8046A530 mSubObject;     // offset 0xabc (embedded sub-object)
};

} // namespace LOD
