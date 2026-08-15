// Auto-scaffolded catalog TU for monolib/src/scn/CScnItemId
#include <harness_catalog.h>
#include <cstring>
#include "libs/monolib/src/scn/CScnItemId.hpp"
#include "monolib/util/MemManager.hpp"
#include "monolib/util/CPathUtil.hpp"

// ===========================================================================
// CScnItemId::~CScnItemId - unloads the active scene resource and frees the
// item when the delete flag is positive. MWCC emits the this!=0 guard, the
// vtable re-point and the flag>0 delete logic automatically for member dtors;
// the vptr is re-pointed manually (novtable) to the retail vtable label.
// ===========================================================================
CScnItemId::~CScnItemId() {
    *(void**)this = (void*)lbl_eu_8056DCD8;
    func_804BCC1C(func_804BC9EC__Fv());
}

// ===========================================================================
// __ct__804820F8 - scene-id item factory (retail placeholder "constructor(8)").
// Allocates a CScnItemId (kind 5) from the host's item pool, seeds it from
// (value, name), loads the scene resource data for `value`, and registers the
// item back into the pool. Returns the item, or null when the pool is full or
// the id list is already occupied.
// ===========================================================================
CScnItemId* __ct__804820F8(CScnItemIdHost* self, u32 value, const char* name) {
    CScnItemId* item;
    if (func_8048C5B8(self->mPool, 5) == 0) {
        return 0;
    }

    // The id list (kind 5) must be empty before a new id item can be created.
    CScnItemIdListSlot* slot = (CScnItemIdListSlot*)func_8048C698(self->mPool, 5);
    CScnItemIdNode* node;
    u32 count;
    CScnItemIdNode* anchor;
    anchor = slot->mAnchor;
    count = 0;
    node = anchor->next;
    while (node != anchor) {
        node = node->next;
        count++;
    }
    if (count != 0) {
        return 0;
    }

    if (name != NULL) {
        item = (CScnItemId*)mtl::MemManager::allocate(0x54, func_80496018(self));
        if (item != NULL) {
            // Retail does not default-construct the FixStr before
            // getNoPathExtName fills it (no ctor call in the retail body); the
            // pointer is declared AFTER the call so the buffer address reuses
            // `name`'s saved register instead of extending liveness backward.
            u8 nameBuf[0x44];
            ml::CPathUtil::getNoPathExtName(*(ml::FixStr<64>*)nameBuf, name);
            ml::FixStr<64>* localName = (ml::FixStr<64>*)nameBuf;

            item->mParent = self;
            item->mType = 5;
            *(void**)item = (void*)lbl_eu_8056DCD8;
            item->mNameLen = std::strlen(localName->mString);
            std::strcpy(item->mName, localName->mString);
            item->mValue = value;
            func_804BC9F4(func_804BC9EC__Fv(), value);
        }
    } else {
        item = (CScnItemId*)mtl::MemManager::allocate(0x54, func_80496018(self));
        if (item != NULL) {
            item->mParent = self;
            item->mType = 5;
            *(void**)item = (void*)lbl_eu_8056DCD8;
            item->mNameLen = std::strlen(NULL);
            std::strcpy(item->mName, NULL);
            item->mValue = value;
            func_804BC9F4(func_804BC9EC__Fv(), value);
        }
    }

    func_8048C630(self->mPool, item, value);
    return item;
}

extern "C" void func_80482288(void) {}
extern "C" void func_8048228C(void) {}
extern "C" void* func_80482290(u8* self) { return self + 0xC; }
extern "C" u32 func_80482298(u8* self) { return ((CScnItemId*)self)->mValue; }
