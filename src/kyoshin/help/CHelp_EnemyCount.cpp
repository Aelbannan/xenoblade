#include "kyoshin/help/CHelp_EnemyCount.hpp"
#include "kyoshin/cf/CfGameManager.hpp"

extern u32 lbl_eu_80663E24;

namespace cf {
void CHelp_EnemyCount::func_802B7F54() {
    unkC = 0;
}

bool CHelp_EnemyCount::func_802B7F60() {
    if (unkC <= 0) {
        // Count active enemy entries in the circular list from func_800B6C7C.
        CHelp_EnemyList* list = func_800B6C7C();
        CHelp_EnemyListNode* node;      // node reuses the func-return register (r3)
        int count = 0;                   // r4
        CHelp_EnemyListNode* head = list->mHead; // r5
        node = head->mNext;
        while (node != head) {
            node = node->mNext;
            count++;
        }

        if (count == 0) {
            return false;
        }

        unk10 = CfGameManager::getEnabledInputFlags();
        CfGameManager::enablePadFlags(-1, false);
        lbl_eu_80663E24 |= 0x2000;
        unkC = 60;
        return false;
    }

    unkC = unkC - 1;
    if (unkC > 0) {
        return false;
    }
    CfGameManager::enablePadFlags(unk10, true);
    lbl_eu_80663E24 &= ~0x2000;
    return true;
}
}
