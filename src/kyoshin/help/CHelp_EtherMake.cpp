#include "kyoshin/help/CHelp_EtherMake.hpp"

extern "C" UNKWORD func_80212480();
extern "C" UNKWORD func_80212490();

namespace cf {
bool CHelp_EtherMake::func_802B80A4() {
    u32 result = 0;
    if (func_80212480() != 0) {
        if (func_80212490() != 0) {
            result = 1;
        }
    }
    return result != 0;
}

void CHelp_EtherMake::func_802B80EC() {
    func_80212490();
}
} // namespace cf
