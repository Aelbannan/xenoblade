#include "kyoshin/help/CHelp_EnemyCount.hpp"
#include "kyoshin/cf/CfGameManager.hpp"

extern "C" void* func_800B6C7C();
extern u32 lbl_eu_80663E24;

namespace cf {
void CHelp_EnemyCount::func_802B7F54() {
    unkC = 0;
}

bool CHelp_EnemyCount::func_802B7F60() {
    if (unkC > 0) goto timerpath;

    {
        void* list = func_800B6C7C();
        void* head = *(void**)((u8*)list + 4);
        void* node = *(void**)head;
        int count = 0;

        goto cond;
        do {
            node = *(void**)node;
            count++;
cond:
            ;
        } while (node != head);

        if (count == 0) {
            return false;
        }

        unk10 = CfGameManager::getEnabledInputFlags();
        CfGameManager::enablePadFlags(-1, false);
        lbl_eu_80663E24 |= 0x2000;
        unkC = 60;
        return false;
    }

timerpath:
    {
        s32 newVal = unkC - 1;
        unkC = newVal;
        if (newVal <= 0) {
            CfGameManager::enablePadFlags(unk10, true);
            lbl_eu_80663E24 &= ~0x2000;
            return true;
        }
        return false;
    }
}
}
