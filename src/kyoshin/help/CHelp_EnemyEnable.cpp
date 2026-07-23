#include "kyoshin/help/CHelp_EnemyEnable.hpp"

extern "C" void* func_800B6BC8();
extern "C" void* func_800AD860(void*);

namespace cf {
bool CHelp_EnemyEnable::func_802B8028() {
    void* list = func_800B6BC8();
    void* cur = *(void**)((u8*)list + 4);
    cur = *(void**)cur;

    goto check;

    do {
        void* obj = func_800AD860(*(void**)((u8*)cur + 8));
        void* subObj = (u8*)obj + 0x3E9C;
        bool (*checkFunc)(void*) = (bool (*)(void*))(*(void***)subObj)[0x74 / 4];
        if (!checkFunc(subObj)) {
            return false;
        }

        cur = *(void**)cur;
check:
        ;
    } while (cur != *(void**)((u8*)list + 4));

    return true;
}
}
