#include <types.h>

namespace cf {

class UnkClass_8018C5FC {
public:
    UnkClass_8018C5FC();
    s32 unk0;
    f32 unk4;
};

extern "C" const f32 lbl_eu_80667A30;

UnkClass_8018C5FC::UnkClass_8018C5FC() {
    unk0 = 0;
    unk4 = lbl_eu_80667A30;
}

} // namespace cf
