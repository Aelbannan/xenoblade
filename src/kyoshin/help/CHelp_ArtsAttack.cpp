#include "kyoshin/help/CHelp_ArtsAttack.hpp"
#include "kyoshin/cf/CfGameManager.hpp"
#include "kyoshin/cf/object/CfObjectPc.hpp"

extern cf::CfObjectPc* func_800BFC68(cf::CfObjectMove* objMove);

extern "C" u32 func_80174C98(void* actor, u32* outVal, u32 flags);

namespace cf {

typedef void* (*GetPtrFn)(void*);

bool CHelp_ArtsAttack::func_802B7D00() {
    CfObjectPc* objPc = func_800BFC68(CfGameManager::func_80082D54(0));

    if (objPc != nullptr) {
        void* sub = *reinterpret_cast<void**>(reinterpret_cast<u8*>(objPc) + 4);
        u32 localVal = *reinterpret_cast<u32*>(
            reinterpret_cast<GetPtrFn>((*reinterpret_cast<void***>(sub))[0x30 / 4])(sub));

        u32 funcResult = func_80174C98(objPc, &localVal, 0xa);

        return reinterpret_cast<bool (*)(CHelp*, u32)>(mVtbl->mSlots[7])(
            this, static_cast<u32>(funcResult == 0));
    }

    return false;
}

} // namespace cf