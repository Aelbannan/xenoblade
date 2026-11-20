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

// Thin shared iface for CBattleManager vtable slot +0x28 (func_800885F0 /
// mask probe). Same layout as CBattleManagerSlot28 in CBattleManager.hpp; kept here so
// caller TUs need not include the heavy manager header.
struct CBattleManagerSlot28 {
    virtual void v000() = 0;
    virtual void v001() = 0;
    virtual void v002() = 0;
    virtual void v003() = 0;
    virtual void v004() = 0;
    virtual void v005() = 0;
    virtual void v006() = 0;
    virtual void v007() = 0;
    virtual s32 v008(u32 mask) = 0;   // slot 0x28
};

// Per-frame battle-manager update (retail-unmangled updateBattleManager, defined
// in kyoshin/cf/CBattleManager.cpp). One shared import; the singleton getter
// above already returns void*, so callers bind without casts.
extern "C" void updateBattleManager(cf::CBattleManager* self);

// Battle event dispatch (retail-unmangled func_800F3970, defined in
// kyoshin/cf/CBattleManager.cpp). One shared import; params are opaque
// pointers so caller TUs with minimal CBattleManager views bind without
// conversions (word-sized args -> identical call-site codegen).
extern "C" void func_800F3970(void* self, void* obj1, void* obj2, s32 idx, s32 addVal);

// C++-linkage battle helpers defined in CBattleManager.cpp. Their retail
// symbols are mangled, so callers take these declarations from this owner API
// header instead of keeping per-TU extern copies.
struct BattleObjAccessor;
struct BattleMoveObjAccessor;
struct Func800EA9A8_Arg5;

bool CBattleMan_ListHasValue(void* mgr, unsigned int value);
void* CBattleMan_NextListItem(void* self);
void* CBattleMan_FindNextActorRef(void* self, void* value);
void CBattleMan_HandleMoveType45(void* self, BattleObjAccessor* obj, void* enemy,
                                 BattleMoveObjAccessor* move);
void CBattleMan_FireActorEvent918(void* self, BattleObjAccessor* actor,
                                  Func800EA9A8_Arg5* arg, void* id);
void CBattleMan_RunBattleEvent(u32 a, u32 b, void* c, u32 d);
void CBattleMan_ClearEventNotify(void* self, u32 id, u16 subId);
void CBattleMan_ClearActorStatus(void* ignored, void* actor, void* arg);
int CBattleMan_GetUnk94Count(void* self, int key);
void CBattleMan_SetPartyFlagReset(cf::CBattleManager* mgr, u32 value);
void CBattleMan_BumpUnk94Key(cf::CBattleManager* mgr, s32 key);
s32 CBattleMan_FindActorByKind(cf::CBattleManager* mgr, s32 kind);
void CBattleMan_RaiseBattleFlag10(cf::CBattleManager* mgr);
void CBattleMan_DropBattleFlag10(cf::CBattleManager* mgr);
void CBattleMan_MarkList48Flag40(void* self);
void CBattleMan_RefreshChainEnum(cf::CBattleManager* mgr);
