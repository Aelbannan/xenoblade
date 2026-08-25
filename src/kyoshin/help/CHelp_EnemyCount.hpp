#pragma once

#include <types.h>
#include "kyoshin/help/CHelp.hpp"

namespace cf {

// Circular linked-list node returned by func_800B6C7C; mNext is a pointer at +0x0.
struct CHelp_EnemyListNode {
    CHelp_EnemyListNode* mNext; // 0x0
};

// Container returned by func_800B6C7C; the sentinel head lives at +0x4.
struct CHelp_EnemyList {
    u8* field_0x0;        // 0x0
    CHelp_EnemyListNode* mHead; // 0x4
};

class __declspec(novtable) CHelp_EnemyCount : public CHelp {
public:
    void func_802B7F54();
    bool func_802B7F60();

    s32 unkC; // 0xC
    u32 unk10; // 0x10
};
} // namespace cf

// C-linkage imports (retail symbol names - keep linkage/signatures verbatim)
extern "C" cf::CHelp_EnemyList* func_800B6C7C();
