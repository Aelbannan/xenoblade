#pragma once

#include <types.h>

// A node in the scene-id circular linked list (single link at +0x00).
struct CScnIdNode {
    CScnIdNode* next;   // +0x00
};

// Pool slot returned by the CScnItemPool accessor (pool + 0x8C);
// holds the circular list sentinel at +0x04.
struct CScnIdPoolSlot {
    u32 mField_0x0;         // +0x00 (not read here)
    CScnIdNode* mAnchor;    // +0x04 list sentinel
};

// The CScnIdManager's parameter object; pool pointer at +0x60.
struct CScnItemPool {};   // opaque item-pool handle (fields not read here)
struct CScnIdParam {
    u8 _00[0x60];           // +0x00
    CScnItemPool* mPool;    // +0x60
};

// Scene ID manager: registers a render callback with CScn.
struct __declspec(novtable) CScnIdMan {
    virtual ~CScnIdMan();
    virtual void cbRenderBefore();

    // +0x00: vtable pointer (auto/compiler-managed)
    CScnIdParam* mParam;    // +0x04
    u8 mFlag;               // +0x08
};
