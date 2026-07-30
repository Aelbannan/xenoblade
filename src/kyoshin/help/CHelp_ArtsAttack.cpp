#include "kyoshin/help/CHelp_ArtsAttack.hpp"
#include "kyoshin/cf/CfGameManager.hpp"
#include "kyoshin/cf/object/CfObjectPc.hpp"

extern cf::CfObjectPc* func_800BFC68(cf::CfObjectMove* objMove);
extern "C" u32 func_80174C98(void* actor, u32* outVal, u32 flags);

namespace cf {

bool CHelp_ArtsAttack::func_802B7D00() {
    CfObjectPc* objPc = func_800BFC68(CfGameManager::getPlayer(0));

    if (objPc != nullptr) {
        void* sub = (void*)*(u32*)((u8*)objPc + 4);
        void* vtable = *(void**)sub;
        void* (*getFunc)(void*) = (void* (*)(void*))((void**)vtable)[0x30 / 4];
        void* ret = getFunc(sub);
        u32 localVal = *(u32*)ret;

        u32 funcResult = func_80174C98(objPc, &localVal, 0xA);
        u32 boolVal = funcResult == 0;

        return ((bool (*)(CHelp*, u32))mVtbl->mSlots[7])(this, boolVal);
    }

    return false;
}

} // namespace cf
