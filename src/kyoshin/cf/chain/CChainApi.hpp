#pragma once

#include <types.h>

// Forward declarations of the owning data views (full definitions stay in
// CChain.hpp / CChainActorListImports.hpp).
class CChainBattleObjTail;
namespace cf { class CChain; class CChainFlag; class CChainActor; }

// Canonical C++ declarations for the helpers owned by chain/CChain.cpp.
void CChain_ReloadVoiceDispatch(u8* a, CChainBattleObjTail* b, int c);
int CChain_QueryVoiceNode();
void CChain_AccumGaugeDelta(cf::CChain* self, cf::CChainFlag* flag, float value);
int CChain_TryActivateChain(cf::CChain* self, u32 param);
int CChain_CheckActivateReady(cf::CChain* self, u32 param);
int CChain_ProbeActorReady(cf::CChain* self, CChainBattleObjTail* target);
bool CChain_HasMemberEntry(cf::CChain* self, CChainBattleObjTail* target);
void CChainActor_CleanupVoiceEnd(cf::CChainActor* self);
void CChainActor_ClearTargetRef(cf::CChainActor* self);
void CChainActor_ToggleCancelVoice(cf::CChainActor* self, u32 param);
void CChainActor_ToggleMoveFlag(cf::CChainActor* self, u32 param);
