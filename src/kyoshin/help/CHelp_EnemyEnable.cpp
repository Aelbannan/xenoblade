#include "kyoshin/help/CHelp_EnemyEnable.hpp"
#include "kyoshin/cf/CfGimmick.hpp"

// Retail circular-object-list accessor (the "active objects" registry).
// main.dol ships it as the unmangled C-style symbol func_800B6BC8.
extern "C" CfGimmickList* func_800B6BC8();

// Resolves a CfGimmickListNode's object slot into the live object. Retail
// mangled C++ symbol getEffOwner____FPv (single void* parameter), so a plain
// C++ declaration (not extern "C") yields the matching linker symbol.
extern cf::CHelp_EnemyEnableObj* func_800AD860(void* obj);

namespace cf {

// Active-state gate used by the Help subsystem: returns true only when every
// currently-spawned object reports its "enable" flag (the sub-object's vtable
// slot 0x74) as set. Walks the circular CfGimmickList sentinel-headed list.
bool CHelp_EnemyEnable::areAllEnemiesEnabled() {
    CfGimmickList* list = func_800B6BC8();
    CfGimmickListNode* cur = list->head->next;

    while (cur != list->head) {
        CHelp_EnemyEnableObj* obj = func_800AD860(cur->object);
        if (!obj->mSub.func74()) {
            return false;
        }
        cur = cur->next;
    }

    return true;
}

} // namespace cf