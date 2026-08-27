#include "kyoshin/help/CHelp_Sp.hpp"
#include "kyoshin/cf/CfGameManager.hpp"
#include "kyoshin/cf/object/CfObjectPc.hpp"

extern cf::CfObjectPc* getCfObjectPc(cf::CfObjectMove* objMove);

namespace cf {
bool CHelp_Sp::isSpThresholdMet() {
    CfObjectPc* objPc = getCfObjectPc(CfGameManager::getPlayer(0));
    if (objPc != nullptr) {
        // Retail CfObjectPc vtable +0x160 is CActorParam_UnkVirtualFunc51
        // (float ratio: field 0x17F0 / 0x17FC). Call the owning-class virtual
        // directly; no TU-local pad.
        f32 value = objPc->CActorParam_UnkVirtualFunc51();
        return mThreshold <= value;
    }
    return false;
}
} // namespace cf
