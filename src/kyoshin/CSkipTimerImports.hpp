#pragma once

// Unit-private imports for kyoshin/CSkipTimer. Function declarations owned by
// this TU live in CSkipTimer.hpp; only external callees/data are listed here.
#include "kyoshin/CSkipTimer.hpp"
#include "kyoshin/CMainMenu.hpp"
#include "kyoshin/CQuestWindow_Pane.hpp"

extern "C" char lbl_eu_80510568[];

// .sdata2/.sbss objects used by the static initializer and slot-frame helper.
extern "C" float lbl_eu_80664A50;
extern "C" const float lbl_eu_80668C4C;
extern "C" const float lbl_eu_80662C78;
extern "C" const float lbl_eu_80668C48;

// cf::CfGameManager and menu-singleton helpers from other units.
// Declared by kyoshin/CMainMenu.hpp.
extern "C" void ArtsInfo_SetReadyFlag();
extern "C" void func_80086B5C__Q22cf13CfGameManagerFv(u32, u32, u32);
