#pragma once

class CProcess;
class CScn;
class CMenuBattleDamage;
class CPcSelectCursor01;

// Canonical C++ declarations for the free helpers defined in CMenuBattleDamage.cpp.
CMenuBattleDamage* BtlDmg_Create(CProcess* parent, CScn* scene);
void BtlDmg_CursorActivate(CPcSelectCursor01* self);
void BtlDmg_CursorToState3(CPcSelectCursor01* self);
void BtlDmg_CursorToState4(CPcSelectCursor01* self);
unsigned char BtlDmg_CursorGetFlag(CPcSelectCursor01* self);
