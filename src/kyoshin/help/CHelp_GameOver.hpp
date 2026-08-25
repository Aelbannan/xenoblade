#pragma once

#include <types.h>
#include "kyoshin/help/CHelp.hpp"

namespace cf {
class __declspec(novtable) CHelp_GameOver : public CHelp {
public:
    bool func_802B80F4();

    s32 unkC; // 0xC
    u8 mFlag10; // 0x10
};
} // namespace cf

struct CHelpFlagBag;

// C-linkage imports (retail symbol names - keep linkage/signatures verbatim)
extern "C" CHelpFlagBag* lbl_eu_80664A10;
extern "C" UNKWORD func_80295D30(CHelpFlagBag* bag);
