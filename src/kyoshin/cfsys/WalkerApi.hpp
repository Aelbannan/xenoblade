#pragma once

#include <types.h>

namespace cf {
class CfObjectImplWalker;
}

// Defined in kyoshin/cfsys/CfObjectImplWalker.cpp. C++ linkage: these
// signatures produce the retail mangled symbol names.
void Walker_ResetBattleHook(cf::CfObjectImplWalker* self);
void Walker_ForwardBattleEntry(cf::CfObjectImplWalker* self, u32 battleId);
void Walker_UpdatePartyBattle(cf::CfObjectImplWalker* self);
void Walker_LeaveBattle(cf::CfObjectImplWalker* self);
void Walker_SwitchTargetParty(cf::CfObjectImplWalker* self, u32 arg);
void Walker_FanfareBattleStart(cf::CfObjectImplWalker* self, u32 flag);
void Walker_VtNoopF0();
void Walker_VtNoopA0();
void Walker_Adjust0C_6EC0(char* self);
void Walker_Adjust0C_Dtor(char* self);
void Walker_Adjust10_FFA0(char* self);
void Walker_Adjust10_Dtor(char* self);
