#pragma once

#include <types.h>
#include "kyoshin/help/CHelp.hpp"

namespace cf {
class CHelp_EnemyCount : public CHelp {
public:
    void func_802B7F54();
    bool func_802B7F60();

    s32 unkC; // 0xC
    u32 unk10; // 0x10
};
} // namespace cf
