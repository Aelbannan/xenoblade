#include "kyoshin/help/CHelp_EndEvent.hpp"
#include <cstring>
#include "kyoshin/cf/CfGameManagerData.hpp"  // H3 label-owner decl (gmScene; gmGameFlags)

extern char gmBdatNameText[0x24];

namespace cf {

void CHelp_EndEvent::checkEndEventState() {
    u32 v;

    if (unk10 != NULL) {
        v = 0;
        if (gmGameFlags & 0x00400000) {
            bool eq = strcmp(gmBdatNameText, unk10) == 0;
            if (eq) {
                v = 1;
            }
        }
    } else {
        v = (gmGameFlags >> 22) & 1;
    }

    this->func_802B7CBC(v == 0);
}

} // namespace cf
