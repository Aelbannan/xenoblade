#pragma once

#include <types.h>

// Thin owner-API header for cf::CBattleManager::getInstance()
// (retail symbol getInstance__Q22cf14CBattleManagerFv).
//
// The real declaration is the static member in kyoshin/cf/CBattleManager.hpp
// (defined in kyoshin/cf/CBattleManager.cpp, singleton pointer
// lbl_eu_80663F00). Most caller TUs treat the singleton opaquely and cannot
// include the full CBattleManager class (heavy transitive types / include
// cycles), so this header carries the ONE shared import under the retail
// pre-mangled name with a void* return; callers cast at the use site.
//
// This is the only copy: do not re-declare this symbol in other headers or
// TUs (divergent extern "C" return types trip MWCC 10505, which is what the
// old per-TU #define-renames papered over).
extern "C" void* getInstance__Q22cf14CBattleManagerFv();

namespace cf { class CBattleManager; }

// Per-frame battle-manager update (retail-unmangled func_800D9354, defined
// in kyoshin/cf/CBattleManager.cpp). One shared import; the singleton getter
// above already returns void*, so callers bind without casts.
extern "C" void func_800D9354(cf::CBattleManager* self);

// Battle event dispatch (retail-unmangled func_800F3970, defined in
// kyoshin/cf/CBattleManager.cpp). One shared import; params are opaque
// pointers so caller TUs with minimal CBattleManager views bind without
// conversions (word-sized args -> identical call-site codegen).
extern "C" void func_800F3970(void* self, void* obj1, void* obj2, s32 idx, s32 addVal);
