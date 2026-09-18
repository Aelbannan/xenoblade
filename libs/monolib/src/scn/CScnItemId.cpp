// Auto-scaffolded catalog TU for monolib/src/scn/CScnItemId
#include <harness_catalog.h>
#include <cstring>
#include "libs/monolib/src/scn/CScnItemId.hpp"
#include "monolib/util/MemManager.hpp"
#include "monolib/util/CPathUtil.hpp"

// Foreign data labels referenced by the CScnItemId vtable/param block.
extern "C" u32 lbl_eu_806638B8;
extern "C" u32 lbl_eu_806624D8;
// Foreign function referenced by the dispatch table.
extern "C" void ScnCamNw4r_EmptyHook48();
// Retail destructor mangled name (CScnItemId::~CScnItemId) for the vtable reloc.
extern "C" void __dt__10CScnItemIdFv();
// Forward declarations for in-TU extern "C" functions referenced by the vtable.
extern "C" void scnItemIdNoopB();
extern "C" void scnItemIdNoopA();
extern "C" void* scnItemIdSelfPlusC(u8* self);
extern "C" u32 scnItemIdGetValue(u8* self);

// === .rodata size=0x10 align=8 ===
extern "C" __declspec(align(8)) const char lbl_eu_80523E98[16] = {
    0x43,0x53,0x63,0x6E,0x49,0x74,0x65,0x6D,0x49,0x64,0x00, 0x00,0x00,0x00,0x00,0x00,
};

// === .data size=0x30 align=8 ===
// CScnItemId vtable (32B).
extern "C" u32 lbl_eu_8056DCD8[8] = {
    (u32)&lbl_eu_806638B8, 0x00000000, (u32)&__dt__10CScnItemIdFv, (u32)&scnItemIdNoopB,
    (u32)&ScnCamNw4r_EmptyHook48, (u32)&scnItemIdNoopA, (u32)&scnItemIdSelfPlusC, (u32)&scnItemIdGetValue,
};
// CScnItemId param block (16B).
extern "C" u32 lbl_eu_8056DCF8[4] = {
    (u32)&lbl_eu_806624D8, 0x00000000, 0x00000000, 0x00000000,
};

// ===========================================================================
// CScnItemId::~CScnItemId - unloads the active scene resource and frees the
// item when the delete flag is positive. MWCC emits the this!=0 guard, the
// vtable re-point and the flag>0 delete logic automatically for member dtors;
// the vptr is re-pointed manually (novtable) to the retail vtable label.
// ===========================================================================
CScnItemId::~CScnItemId() {
    *(void**)this = (void*)lbl_eu_8056DCD8;
    resetScnData(getScnHandle__Fv());
}

// ===========================================================================
// __ct__804820F8 - scene-id item factory (retail placeholder "constructor(8)").
// Allocates a CScnItemId (kind 5) from the host's item pool, seeds it from
// (value, name), loads the scene resource data for `value`, and registers the
// item back into the pool. Returns the item, or null when the pool is full or
// the id list is already occupied.
// ===========================================================================
CScnItemId* __ct__804820F8(CScnItemIdHost* self, u32 value, const char* name) {
    CScnItemId* item = NULL;
    if (CScnItemPool_hasFreeSlot(self->mPool, 5) == 0) {
        return 0;
    }

    // The id list (kind 5) must be empty before a new id item can be created.
    CScnItemIdListSlot* slot = (CScnItemIdListSlot*)CScnItemPool_lookupSubPool(self->mPool, 5);
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
        CScnItemId* newItem = (CScnItemId*)mtl::MemManager::allocate(0x54, Scn_CallUnk8C_V8(self));
        if (newItem != NULL) {
            // Retail does not default-construct the FixStr before
            // getNoPathExtName fills it (no ctor call in the retail body); the
            // pointer is declared AFTER the call so the buffer address reuses
            // `name`'s saved register instead of extending liveness backward.
            u8 nameBuf[0x44];
            ml::CPathUtil::getNoPathExtName(*(ml::FixStr<64>*)nameBuf, name);
            const char* str = ((ml::FixStr<64>*)nameBuf)->mString;

            newItem->mParent = self;
            newItem->mType = 5;
            *(void**)newItem = (void*)lbl_eu_8056DCD8;
            newItem->mNameLen = std::strlen(str);
            std::strcpy(newItem->mName, str);
            newItem->mValue = value;
            func_804BC9F4(getScnHandle__Fv(), value);
        }
        item = newItem;
    } else {
        item = (CScnItemId*)mtl::MemManager::allocate(0x54, Scn_CallUnk8C_V8(self));
        if (item != NULL) {
            item->mParent = self;
            item->mType = 5;
            *(void**)item = (void*)lbl_eu_8056DCD8;
            item->mNameLen = std::strlen(NULL);
            std::strcpy(item->mName, NULL);
            item->mValue = value;
            func_804BC9F4(getScnHandle__Fv(), value);
        }
    }

    CScnItemPool_registerOtherList(self->mPool, item, value);
    return item;
}

extern "C" void scnItemIdNoopA(void) {}
extern "C" void scnItemIdNoopB(void) {}
extern "C" void* scnItemIdSelfPlusC(u8* self) { return self + 0xC; }
extern "C" u32 scnItemIdGetValue(u8* self) { return ((CScnItemId*)self)->mValue; }
