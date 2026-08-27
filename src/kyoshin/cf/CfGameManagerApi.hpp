#pragma once

// Thin owner-API header for cf::CfGameManager::getCameraDataBlock()
// (retail symbol getCameraDataBlock__Q22cf13CfGameManagerFv, 0x80082B7C).
//
// The real declaration is the member in include/kyoshin/cf/CfGameManager.hpp
// (defined in kyoshin/cf/CfGameManager.cpp). Caller TUs treat the result
// opaquely and cannot all include the full CfGameManager class (heavy
// transitive types / include cycles), so this header carries the ONE shared
// import under the retail pre-mangled name with the canonical
// UnkClass_800821F8* view (vtable slots 0x08-0x40, layout per
// CfGameManagerUnityHelpers.hpp); callers needing another shape cast at the
// use site.
//
// This is the only copy: do not re-declare this symbol in other headers or
// TUs (divergent extern "C" return types trip MWCC 10505, which is what the
// old per-TU #define-renames papered over).
class UnkClass_800821F8;
extern "C" UnkClass_800821F8* getCameraDataBlock__Q22cf13CfGameManagerFv();
