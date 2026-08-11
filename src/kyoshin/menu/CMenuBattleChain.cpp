// Auto-scaffolded catalog TU for kyoshin/menu/CMenuBattleChain
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/harness_catalog.hpp"

#include "kyoshin/menu/CMenuBattleChain.hpp"
#include "kyoshin/cf/CfGameManager.hpp"

// forward declarations for scaffold thunk references
void __dt__16CMenuBattleChainFv(void*);
void cbRenderBefore__16CMenuBattleChainFv(void*);

void __ct__CMenuBattleChain(){}

CMenuBattleChain::~CMenuBattleChain() {}

void CMenuBattleChain::Init() {}

void CMenuBattleChain::Term() {}

void CMenuBattleChain::Move() {}

void CMenuBattleChain::cbRenderBefore() {}

void func_802AA2A0(){}

void func_802AA338__Fv(){}

void func_802AA3D0(){}

void func_802AA588(){}

void func_802AAC78(){}

void func_802AB3B8(void* self) { ((void(*)(void*))__dt__16CMenuBattleChainFv)((char*)self - 0x6c); }

void func_802AB3C0(void* self) { ((void(*)(void*))cbRenderBefore__16CMenuBattleChainFv)((char*)self - 0x70); }

void func_802AB3C8(void* self) { ((void(*)(void*))__dt__16CMenuBattleChainFv)((char*)self - 0x70); }

// Track the current player and clear both pending toggle flags.
void func_802AB3D0(CBattleChainMenuState* self) {
    self->mPlayer = cf::CfGameManager::getPlayer(0);
    self->mFlag4 = 0;
    self->mFlag5 = 0;
}

// Consume the pending toggle flags once the arts-select menu is interactable.
void func_802AB410(CBattleChainMenuState* self) {
    if (CMenuArtsSelect_isInteractable()) {
        if (self->mFlag4 != 0) {
            func_8010433C();
            self->mFlag4 = 0;
        }
        if (self->mFlag5 != 0) {
            func_801043BC();
            self->mFlag5 = 0;
        }
    }
}

// Disable the arts-select menu, then (re)track the current player.
void func_802AB474(CBattleChainMenuState* self) {
    CMenuArtsSelect_setDisabled();
    self->mPlayer = cf::CfGameManager::getPlayer(0);
    self->mFlag4 = 0;
    self->mFlag5 = 0;
}

// Disable the arts-select menu only when the tracked player changed, then
// (re)track the current player.
void func_802AB4B8(CBattleChainMenuState* self) {
    if (self->mPlayer != cf::CfGameManager::getPlayer(0)) {
        CMenuArtsSelect_setDisabled();
    }
    self->mPlayer = cf::CfGameManager::getPlayer(0);
    self->mFlag4 = 0;
    self->mFlag5 = 0;
}

void func_802AB510(){}

extern "C" void func_802AB590(void* self) { *(u8*)((u8*)self + 4) = 1; }

// Whether the chain menu is busy (pending flag or arts-select not ready).
bool func_802AB59C(CBattleChainMenuState* self) {
    return self->mFlag4 != 0 || CMenuArtsSelect_isNotReady();
}

extern "C" void func_802AB5E4(void* self) { *(u8*)((u8*)self + 5) = 1; }

// --- hard-symbol stubs (scaffold_hard_symbols) ---
void sinit_802AAF24(){}
