#include "kyoshin/help/CHelp_Sp.hpp"
#include "kyoshin/cf/CfGameManager.hpp"
#include "kyoshin/cf/object/CfObjectPc.hpp"

extern cf::CfObjectPc* func_800BFC68(cf::CfObjectMove* objMove);

namespace cf {
bool CHelp_Sp::func_802B8654() {
    CfObjectPc* objPc = func_800BFC68(CfGameManager::func_80082D54(0));
    if (objPc != nullptr) {
        void** vt = *reinterpret_cast<void***>(objPc);
        f32 value = reinterpret_cast<f32 (*)(CfObjectPc*)>(vt[0x160 / 4])(objPc);
        return mThreshold <= value;
    }
    return false;
}
} // namespace cf
