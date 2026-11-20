#pragma once

#include <types.h>

struct CScnItem;
class CScnItemPool;

// Canonical C++ declarations for helpers defined in CScnItemPool.cpp.
CScnItem* CScnItemPool_allocSmallSlot(u8* self);
u32 CScnItemPool_hasFreeSlot(u8* self, s32 kind);
u32 CScnItemPool_registerOtherList(u8* self, u8* other);
void CScnItemPool_releaseItemByKey(CScnItemPool* self, u32 key);
void CScnItemPool_forEachCallVfunc14(CScnItemPool* self);
