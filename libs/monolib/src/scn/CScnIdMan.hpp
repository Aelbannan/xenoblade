#pragma once

#include <types.h>
#include "monolib/scn/CScn.hpp"
#include "monolib/scn/IScnRender.hpp"

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

// Scene ID manager: registers itself as a render callback with its CScn.
// Retail RTTI (__RTTI__9CScnIdMan) has a null base list, i.e. the
// IScnRender base is not public.
class CScnIdMan : IScnRender {
public:
    CScnIdMan(CScn* pScn);
    virtual ~CScnIdMan();
    virtual void cbRenderBefore();

    // +0x00: vtable pointer (IScnRender)
    CScn* mScn;             // +0x04
    u8 mFlag;               // +0x08
};
