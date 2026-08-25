#include "kyoshin/help/CHelp_ArtsAttack.hpp"
#include "kyoshin/cf/CfMapItemManager.hpp"
#include "kyoshin/cf/CfGameManager.hpp"
#include "kyoshin/cf/object/CfObjectPc.hpp"

extern cf::CfObjectPc* func_800BFC68(cf::CfObjectMove* objMove);
struct D { u8 pad[8]; virtual void _v008(); virtual void _v00C(); virtual void _v010(); virtual void _v014(); virtual void _v018(); virtual bool _v01C(u32); };
struct A { virtual void _v008(); virtual void _v00C(); virtual void _v010(); virtual void _v014(); virtual void _v018(); virtual void _v01C(); virtual void _v020(); virtual void _v024(); virtual void _v028(); virtual void _v02C(); virtual u32* _v030(); };

namespace cf {

bool CHelp_ArtsAttack::func_802B7D00() {
    CfObjectPc* objPc = func_800BFC68(CfGameManager::getPlayer(0));

    if (objPc != nullptr) {
        A* sub = *reinterpret_cast<A**>(reinterpret_cast<u8*>(objPc) + 4);
        u32* ret = sub->_v030();
        u32 localVal = *ret;

        u32 funcResult = func_80174C98(objPc, &localVal, 0xA);
        u32 boolVal = funcResult == 0;

        return reinterpret_cast<D*>(this)->_v01C(boolVal);
    }

    return false;
}

} // namespace cf
