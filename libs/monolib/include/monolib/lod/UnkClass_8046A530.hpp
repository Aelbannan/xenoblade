#pragma once

#include <types.h>

// Forward declaration: LODMemMan is defined in LODMemMan.hpp, which includes
// this header.  Only pointer/reference access is needed here.
namespace LOD { class LODMemMan; }

namespace LOD {

/**
 * Unknown LOD sub-object.  This class and LODMemMan form an overlapping pair
 * in the owning container: LODMemMan contains an embedded UnkClass_8046A530
 * at offset 0xabc, and UnkClass_8046A530 has a LODMemMan sub-object embedded
 * at offset 0xa44 (i.e. the two share the same backing memory, viewed from
 * different base offsets).
 *
 * Methods are listed in symbol order.  Everything before offset 0xa44 is
 * opaque padding until those fields are recovered.
 */
class UnkClass_8046A530 {
public:
    /* 8046E500 */ void func_8046A530();
    /* 8046E594 */ void func_8046A5C4();
    /* 8046EAA8 */ void func_8046AAD8();
    /* 8046EAAC */ void func_8046AADC();
    /* 8046EB24 */ void func_8046AB54();
    /* 8046ECFC */ void func_8046AD2C();
    /* 8046F07C */ void func_8046B0AC();
    /* 8046FAB0 */ void func_8046BAE0();
    /* 80470550 */ void func_8046C580();
    /* 80470F84 */ void func_8046CFB4();
    /* 80470FA8 */ void func_8046CFD8();

    /** Access the LODMemMan sub-object embedded at offset 0xa44. */
    LODMemMan& GetMemMan();

private:
    // Opaque padding up to offset 0xa44 where the LODMemMan sub-object starts.
    u8 mPad_00[0xa44];
    // LOD::LODMemMan sub-object lives here at offset 0xa44.
};

} // namespace LOD
