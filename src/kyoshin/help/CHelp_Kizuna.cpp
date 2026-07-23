#include "kyoshin/help/CHelp_Kizuna.hpp"

struct CHelpFlagBag {
    u8 pad[0x14];
    u8 mItemCole; // +0x14
    u8 mKizuna; // +0x15
};

extern "C" CHelpFlagBag* lbl_eu_80664A10;

namespace cf {
u8 CHelp_Kizuna::func_802B818C() {
    return lbl_eu_80664A10->mKizuna;
}
} // namespace cf
