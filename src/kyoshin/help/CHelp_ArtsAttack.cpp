#include "kyoshin/help/CHelp_ArtsAttack.hpp"
#include "kyoshin/cf/CfMapItemManager.hpp"
#include "kyoshin/cf/CfGameManager.hpp"
#include "kyoshin/cf/object/CfObjectPc.hpp"

extern cf::CfObjectPc* getCfObjectPc(cf::CfObjectMove* objMove);

namespace cf {

u32 CHelp_ArtsAttack::checkHelpCondition() {
    CfObjectPc* objPc = getCfObjectPc(CfGameManager::getPlayer(0));

    if (objPc != nullptr) {
        PcSub4VtIf* sub = *reinterpret_cast<PcSub4VtIf**>(reinterpret_cast<u8*>(objPc) + 4);
        u32* ret = static_cast<u32*>(sub->_q030());
        u32 localVal = *ret;

        u32 funcResult = func_80174C98(objPc, &localVal, 0xA);
        u32 boolVal = funcResult == 0;

        return this->func_802B7CBC(boolVal);
    }

    return false;
}

} // namespace cf
