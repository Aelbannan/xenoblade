#pragma once

#include <types.h>
#include "kyoshin/help/CHelp.hpp"

namespace cf {
class __declspec(novtable) CHelp_EndEvent : public CHelpSwitch {
public:
    void func_802B7EC4();

    char* unk10; // 0x10 (mFlag at 0xC from CHelpSwitch)
};
}
