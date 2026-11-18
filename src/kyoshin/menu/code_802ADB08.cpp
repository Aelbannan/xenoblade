// kyoshin/menu/code_802ADB08 - battle-chain menu state helpers.
//
// These eight functions form their own translation unit between
// CMenuBattleChain.cpp and CVS_THREAD_BATTLE_END_SP.cpp in the retail link
// order. MWCC emits a TU's generated code - the static initialiser and the
// MI adjusting thunks - at the END of its .text, so retail's CMenuBattleChain
// .text stops right after [__sinit_802AAF24][@108@/@112@ thunks] at
// 0x802ADB08; the block that follows (0x802ADB08..0x802ADD28) is this TU.
//
// The TU is data-free: it emits no .data/.rodata/.sdata/.sbss, which is why
// the neighbouring data slices stay contiguous across it (CMenuBattleChain
// .data ends at 0x80539F48 and CVS_THREAD_BATTLE_END_SP starts there;
// CMenuBattleChain .sdata ends at 0x80662DD8 and CVS starts there).
//
// The functions are retail's unmangled symbols (see
// kyoshin/cf/chain/CChainImports.hpp for the callers); the shared
// CBattleChainMenuState view and the CMenuArtsSelect imports live in
// kyoshin/menu/CMenuBattleChain.hpp.

#include <types.h>

#include "kyoshin/menu/CMenuBattleChain.hpp"
#include "kyoshin/cf/CfGameManager.hpp"

// Track the current player and clear both pending toggle flags.
extern "C" void trackChainPlayer(CBattleChainMenuState* self) {
    self->mPlayer = cf::CfGameManager::getPlayer(0);
    self->mFlag4 = 0;
    self->mFlag5 = 0;
}

// Consume the pending toggle flags once the arts-select menu is interactable.
extern "C" void consumeChainToggles(CBattleChainMenuState* self) {
    if (CMenuArtsSelect_isInteractable()) {
        if (self->mFlag4 != 0) {
            CMenuArtsSelect_CreateArtsRef();
            self->mFlag4 = 0;
        }
        if (self->mFlag5 != 0) {
            CMenuArtsSelect_ResetSlotAnims();
            self->mFlag5 = 0;
        }
    }
}

// Disable the arts-select menu, then (re)track the current player.
extern "C" void reinitChainPlayer(CBattleChainMenuState* self) {
    CMenuArtsSelect_setDisabled();
    self->mPlayer = cf::CfGameManager::getPlayer(0);
    self->mFlag4 = 0;
    self->mFlag5 = 0;
}

// Disable the arts-select menu only when the tracked player changed, then
// (re)track the current player.
extern "C" void updateChainPlayer(CBattleChainMenuState* self) {
    if (self->mPlayer != cf::CfGameManager::getPlayer(0)) {
        CMenuArtsSelect_setDisabled();
    }
    self->mPlayer = cf::CfGameManager::getPlayer(0);
    self->mFlag4 = 0;
    self->mFlag5 = 0;
}

/*
 * Arts-select gate: report whether the arts menu selection state is live, and
 * set *outFlag when the current selection is "chain ready" (state 0 with
 * substate 5). A missing state or a -1 substate means "not usable".
 */
extern "C" bool isChainReady(CBattleChainMenuState* self, u8* outFlag) {
    // Retail keeps the two flag stores in separate branches (no store merge).
    CArtsSelectStateView* sel = CMenuArtsSelect_getSelectState();
    if (sel == NULL)
        return false;
    s8 sub = sel->field_1;
    if (sub == -1)
        return false;
    s8 state = sel->field_0;
    // Two separate stores - MWCC does not merge them.
    if (state == 0 && sub == 5)
        *outFlag = 1;
    else
        *outFlag = 0;
    return true;
}

extern "C" void requestArtsRef(CBattleChainMenuState* self) { self->mFlag4 = 1; }

// Whether the chain menu is busy (pending flag or arts-select not ready).
extern "C" bool isChainMenuBusy(CBattleChainMenuState* self) {
    return self->mFlag4 != 0 || CMenuArtsSelect_isNotReady();
}

extern "C" void requestAnimReset(CBattleChainMenuState* self) { self->mFlag5 = 1; }
