#include "kyoshin/help/CHelp_CloseItemMenu.hpp"

extern "C" u32 func_801B481C();

namespace cf {

struct CHelpVtblInterface {
    virtual void slot0() = 0;
    virtual void slot1() = 0;
    virtual void slot2() = 0;
    virtual void slot3() = 0;
    virtual void slot4() = 0;
    virtual void slot5() = 0;
    virtual void slot6() = 0;
    virtual void slot7(CHelp* self, u32 arg) = 0;
};

void CHelp_CloseItemMenu::func_802B7E34() {
    u32 funcResult = func_801B481C();
    reinterpret_cast<CHelpVtblInterface*>(mVtbl)->slot7(this, funcResult == 0);
}

} // namespace cf
