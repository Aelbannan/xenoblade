#pragma once

#include <types.h>

// Thin owner-API header for kyoshin/menu/CMenuBattleCommu (declarations mirror
// the canonical copies in CMenuBattleCommu.hpp).
class CMenuBattleCommu;
class CProcess;
class CScn;

CMenuBattleCommu* battleCommuCreateRegister(CProcess* parent, CScn* scene);
void battleCommuDisableAnims();
bool battleCommuIsActive();
