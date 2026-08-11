#pragma once

#include <types.h>
#include <monolib/scn/CScnFilter.hpp>

// Flag-test helper shared with the CScnVirtualLight unit (retail 0x8004BAB0):
// returns (*(u32*)self & flags) != 0.
s32 func_8004B3D8(u32* self, u32 flags);

// Minimal layout views for the CScnFilter reslist helpers in this TU
// (mirror the _reslist_node / _reslist_base offsets from
// monolib/util/reslist.hpp; only the fields these helpers touch are declared).
struct CScnFilterListNode {
    CScnFilterListNode* mNext;  // 0x0
    CScnFilterListNode* mPrev;  // 0x4
};

struct CScnFilterList {
    u32 field_0x0;                 // 0x0 (vtable ptr)
    CScnFilterListNode* mStartNodePtr;  // 0x4
};

struct CScnFilterListIter {
    CScnFilterListNode* mNode;  // 0x0
};

class CScnFilterMan {
public:
    CScnFilterMan();
    virtual ~CScnFilterMan();
    void update();
    void cbRenderBefore();

    // TODO: add fields
};
