// Decompilation of kyoshin/help/CHelp_Talk

#include "kyoshin/help/CHelp_Talk.hpp"
#include "kyoshin/cf/CfGameManager.hpp"
#include "kyoshin/cf/object/CfObjectPc.hpp"

extern cf::CfObjectPc* func_800BFC68(cf::CfObjectMove* objMove);
extern "C" u32 func_80174C98(void* actor, u32* outVal, u32 flags);

// func_800B708C is declared in CfObjectPc.hpp (included transitively).
// func_800BF324 converts a CObjectParam pointer to a CActorParam pointer.
extern "C" void* func_800BF324(void* objParam);

namespace cf {

// Reset the talk help: enable the switch flag and clear the state byte.
void CHelp_Talk::func_802B86BC() {
    CHelpSwitch::func_802B7CB0();
    field_0x16 = 0;
}

// Evaluate whether the talk help condition is satisfied.
// Checks voice action state, then dispatches to the virtual slot 8 handler.
// If that returns true and the voice is active, performs additional validation
// against the player's current target.
bool CHelp_Talk::func_802B86F0() {
    // Get the current voice action for player 0
    void* voiceAction = func_8016FE34(CfGameManager::getPlayer(0));

    bool voiceActive = false;
    if (voiceAction != nullptr) {
        // Read a u32 via virtual call on voiceAction->field_0x4, slot 0x30
        void** subVt = *reinterpret_cast<void***>(reinterpret_cast<u8*>(voiceAction) + 4);
        u32 localVal = *reinterpret_cast<u32*>(
            reinterpret_cast<void* (*)(void*)>(subVt[0x30 / 4])(voiceAction));

        // Query a property of the voice action
        if (func_80174C98(voiceAction, &localVal, 1) != 0) {
            voiceActive = true;
        }
    }

    // Dispatch to virtual slot 8 with the voice-active flag
    if (!reinterpret_cast<bool (*)(CHelp_Talk*, bool)>(mVtbl->mSlots[8])(this, voiceActive)) {
        return false;
    }

    // If no voice is active, return the current state byte as-is
    if (!voiceActive) {
        return field_0x16 != 0;
    }

    // Voice is active: set state and perform additional checks
    field_0x16 = 1;

    // If field_0x10 is set, validate against the player's target
    if (field_0x10 != 0) {
        void* targetObj = nullptr;
        void* targetResult = func_8016FE34(CfGameManager::getPlayer(0));
        if (targetResult != nullptr) {
            // Access CObjectParam at retail offset 0x3E9C via layout shim
            struct RetailLayout {
                u8 pad[0x3E9C];
                cf::CObjectParam objectParam;
            };
            RetailLayout* retail = reinterpret_cast<RetailLayout*>(targetResult);
            targetObj = reinterpret_cast<void* (*)(cf::CObjectParam*)>(
                reinterpret_cast<void***>(&retail->objectParam)[0][0x4C / 4])(&retail->objectParam);
            targetObj = func_800B708C(reinterpret_cast<BOOL>(targetObj));
            targetObj = func_800BF324(targetObj);
        }

        // Compare field_0x10 against the target's field_0x8C (u16)
        if (targetObj != nullptr) {
            u16 targetVal = *reinterpret_cast<u16*>(reinterpret_cast<u8*>(targetObj) + 0x8C);
            field_0x16 = (field_0x10 == targetVal) ? 1 : 0;
        } else {
            field_0x16 = 0;
        }
    }

    // If state is still set and field_0x14 is nonzero, check a global flag
    if (field_0x16 && field_0x14) {
        // func_8009CF8C(0x7D0) returns a u32; nonzero result clears state
        field_0x16 = (func_8009CF8C(0x7D0) != 0) ? 1 : 0;
    }

    // If state is still set and field_0x15 is nonzero, check actor substate
    if (field_0x16 && field_0x15) {
        void* actorResult = func_8016FE34(CfGameManager::getPlayer(0));
        bool substateOk = false;
        if (actorResult != nullptr) {
            struct RetailLayout {
                u8 pad[0x3E9C];
                cf::CObjectParam objectParam;
            };
            RetailLayout* retail = reinterpret_cast<RetailLayout*>(actorResult);
            void* actor = reinterpret_cast<void* (*)(cf::CObjectParam*)>(
                reinterpret_cast<void***>(&retail->objectParam)[0][0x4C / 4])(&retail->objectParam);
            actor = func_800B708C(reinterpret_cast<BOOL>(actor));
            actor = func_800BF324(actor);
            if (actor != nullptr) {
                // Virtual call at slot 0x228 to get substate; check if == 3
                void** actorVt = *reinterpret_cast<void***>(actor);
                int substate = reinterpret_cast<int (*)(void*)>(actorVt[0x228 / 4])(actor);
                substateOk = (substate == 3);
            }
        }
        field_0x16 = substateOk ? 1 : 0;
    }

    return field_0x16 != 0;
}

} // namespace cf
