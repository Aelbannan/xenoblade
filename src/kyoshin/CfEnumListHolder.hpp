#pragma once

#include <types.h>

namespace cf {
class CfObjEnumList;
}

// 8-byte enum-list holder (list pointer + MEM2 alloc handle). Retail
// CTaskGame_enumListCtor / CTaskGame_enumListFill / __dt__80043E88 /
// CTaskGame_enumListGet, defined in CTaskGame.cpp.
class CfEnumListHolder {
public:
    CfEnumListHolder();
    CfEnumListHolder(u32 type, u32 filter);
    ~CfEnumListHolder();
    cf::CfObjEnumList* get();

    cf::CfObjEnumList* list; // +0x00
    u32 handle;              // +0x04
};
