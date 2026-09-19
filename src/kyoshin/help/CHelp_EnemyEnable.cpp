#include "kyoshin/help/CHelp_EnemyEnable.hpp"
#include "kyoshin/cf/CfGimmick.hpp"

// Retail circular-object-list accessor (the "active objects" registry).
// main.dol ships it as the unmangled C-style symbol getReslistB48.
extern "C" CfGimmickList* getReslistB48();

// Resolves a CfGimmickListNode's object slot into the live object. Retail
// linker name is getEffOwner____FPv (extra empty qualifier); a plain
// getEffOwner(void*) declaration mangles to getEffOwner__FPv and misses.
extern "C" cf::CHelp_EnemyEnableObj* getEffOwner____FPv(void* obj);

namespace cf {

// Active-state gate used by the Help subsystem: returns true only when every
// currently-spawned object reports its "enable" flag (the sub-object's vtable
// slot 0x74) as set. Walks the circular CfGimmickList sentinel-headed list.
bool CHelp_EnemyEnable::areAllEnemiesEnabled() {
    CfGimmickList* list = getReslistB48();
    CfGimmickListNode* cur = list->head->next;

    while (cur != list->head) {
        CHelp_EnemyEnableObj* obj = getEffOwner____FPv(cur->object);
        if (!obj->mSub.func74()) {
            return false;
        }
        cur = cur->next;
    }

    return true;
}

} // namespace cf