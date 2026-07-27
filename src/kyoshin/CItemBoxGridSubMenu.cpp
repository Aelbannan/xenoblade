// TU for kyoshin/CItemBoxGridSubMenu
// Submenu overlay widget for the item grid screen.

#include "kyoshin/CItemBoxGridSubMenu.hpp"

u8 CItemBoxGridSubMenu::func_80208358() {
    return mIsVisible;
}

u8 CItemBoxGridSubMenu::func_80208360() {
    return mIsOpened;
}

void CItemBoxGridSubMenu::func_802083CC() {
    if (mAnimState != 2)
        return;

    switch (mSubState) {
    case 1:
        mSelectedIdx = 0;
        break;
    case 2:
        mSelectedIdx = (unsigned char)(mSelectedIdx - 1);
        if ((signed char)mSelectedIdx < 0)
            mSelectedIdx = 1;
        break;
    case 3:
        mSelectedIdx = (unsigned char)(mSelectedIdx - 1);
        if ((signed char)mSelectedIdx < 0)
            mSelectedIdx = 2;
        break;
    }
}

void func_8020844C(){}

void func_802084D4(){}

void func_80208760(){}

void func_802087B8(){}

void func_80208838(void){}

void func_80208844(){}

void func_80208890(){}

extern "C" void func_80137038__FPQ34nw4r3lyt6LayoutPQ34nw4r3lyt8DrawInfoii(void*, void*, int, int);
void func_802082D0(void* self){
    if (*(u8*)((u8*)self + 0x20) == 0) return;
    func_80137038__FPQ34nw4r3lyt6LayoutPQ34nw4r3lyt8DrawInfoii(*(void**)((u8*)self + 8), 0, 0, 1);
}
