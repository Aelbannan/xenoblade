#pragma once

#include <types.h>
#include "kyoshin/help/CHelp.hpp"

namespace cf {
class CHelp_EndEvent : public CHelp {
public:
    void func_802B7EC4();

    u32 unkC; // 0xC
    char* unk10; // 0x10
};
}
