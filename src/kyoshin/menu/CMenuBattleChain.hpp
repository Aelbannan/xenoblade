#pragma once

#include <types.h>

namespace cf {
class CfObjectMove;
}

// Battle-chain menu state shared by the func_802AB3D0..func_802AB5E4 helpers:
// the current player reference plus two pending toggle flags consumed when the
// arts-select menu becomes interactable.
struct CBattleChainMenuState {
    cf::CfObjectMove* mPlayer; // +0x00 current player (cf::CfGameManager::getPlayer(0))
    u8 mFlag4;                 // +0x04 pending flag
    u8 mFlag5;                 // +0x05 pending flag
};

class CMenuBattleChain {
public:
    CMenuBattleChain();
    virtual ~CMenuBattleChain();
    void Init();
    void Term();
    void Move();
    void cbRenderBefore();

    // TODO: add fields
};

// C-linkage imports from the CMenuArtsSelect unit (retail unmangled names).
// Proper home: kyoshin/menu/CMenuArtsSelect.hpp - move there when that unit
// exports them. Declared here because CMenuArtsSelect.hpp pulls the full
// monolib CScn, which conflicts with the minimal CScn from
// kyoshin/harness_catalog.hpp -> CTaskGameEff.hpp.
extern "C" void CMenuArtsSelect_setDisabled();
extern "C" int CMenuArtsSelect_isInteractable();
extern "C" int CMenuArtsSelect_isNotReady();
extern "C" void func_8010433C();
extern "C" void func_801043BC();
