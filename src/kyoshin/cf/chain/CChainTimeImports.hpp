#pragma once

// TU-private imports for kyoshin/cf/chain/CChainTime.cpp (moved out of the public
// CChainTime.hpp, which keeps the classes).

#include "kyoshin/cf/chain/CChainTime.hpp"
#include "kyoshin/cf/chain/CChainEffectImports.hpp"
extern "C" void startChainTimer(cf::CChainTime*, u8);
extern "C" void tickChainTimer(cf::CChainTime*);

// C-linkage imports (retail symbol names - keep linkage/signatures verbatim)
namespace cf { class CBattleManager; }
extern "C" void refreshBattleActors(cf::CBattleManager*, f32, int);
extern "C" void chainUnbindMatchingObjects(int, int);
