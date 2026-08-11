// Auto-scaffolded catalog TU for kyoshin/menu/CMenuTitle
// Replace stubs with high-level C/C++ during decomp.

#include <types.h>

#include "kyoshin/cf/CfGameManager.hpp"
#include "kyoshin/menu/CMenuTitle.hpp"

// CfPadData access shim - only holds the flags this handler reads (see CfPadData.hpp).
struct CfPadDataLocal {
    u32 mPadHeldFlags;          // 0x00 - CPad::mHeldButtonFlags
    u32 mPadPressedFlags;       // 0x04 - CPad::mPressedButtonFlags
    u8 _pad[0xF0];              // 0x08-0xF7 - rest of CPad
    u32 mHeldButtonFlags;       // 0xF8
    u32 mPrevHeldButtonFlags;   // 0xFC
    u32 mPressedButtonFlags;    // 0x100
    u32 mTurboPressButtonFlags; // 0x104
    u8 mTimers[0x20];           // 0x108
};

void __ct__CMenuTitle(){}

// CfGameManager one-arg controller-type query, kept as the retail mangled
// C symbol (extern "C" stops C++ `__Fi` param mangling). The inline header
// wrapper drops the -1 argument, so it cannot be used here.
extern "C" int func_80086F9C__Q22cf13CfGameManagerFv(int controllerType);

extern "C" void func_802B5F0C(void* self) { *(u8*)((u8*)self + 0xE8) = 1; }

void func_802B5F18(CMenuTitleInput* self) {
    // Opening menu: query the +0x60 sub-menu, flag this menu when it opens.
    if (func_802B7564(&self->mSub)) {
        self->field_e8 = 0x2;
    }
}

void func_802B5F58(CMenuTitleInput* self) {
    extern void func_802B75B8(CMenuTitleSub*);
    ((unsigned char*)self)[0xe8] = 3;
    func_802B75B8(&self->mSub);
}

void func_802B5F68(CMenuTitleInput* self) {
    // Non-zero sub-menu query -> open the sub-menu transition state.
    if (func_802B7590(&self->mSub)) {
        self->field_e8 = 0x4;
    }
}

void func_802B5FA8(CMenuTitleInput* self) {
    CfPadDataLocal* pad =
        (CfPadDataLocal*)cf::CfGameManager::getCfPadData();

    // The activation mask differs between controller types: classic controllers
    // add the right-stick trigger bits (0x1fe0060f) to the common button bits.
    u32 mask = (func_80086F9C__Q22cf13CfGameManagerFv(-1) != 0) ? 0x1fe0060fU : 0x1effU;
    if (pad->mPadPressedFlags & mask) {
        self->field_e8 = 5;
        func_802B7630(&self->mSub);
        self->field_ec = lbl_eu_80668FD0;
    }
}

void func_802B6020(CMenuTitleInput* self) {
    // Sub-menu still closed: poll input and open it on the activation mask.
    if (func_802B7590(&self->mSub) == 0) {
        CfPadDataLocal* pad =
            (CfPadDataLocal*)cf::CfGameManager::getCfPadData();
        u32 mask = (func_80086F9C__Q22cf13CfGameManagerFv(-1) != 0)
                       ? 0x1fe0060fU
                       : 0x1effU;
        if (pad->mPadPressedFlags & mask) {
            func_802B7800(&self->mSub);
        }
        self->field_ec = lbl_eu_80668FD4;
    } else {
        // Sub-menu open: advance the progress float toward its threshold.
        self->field_ec += lbl_eu_80668FD8;
        if (self->field_ec < lbl_eu_80668FD0) {
            return;
        }
        self->field_ec = lbl_eu_80668FD0;
        self->field_e8 = 6;
    }
}

void func_802B60CC(CMenuTitleInput* self) {
    // Only accept input while enabled (field_e9) and not in the transient
    // field_ea state (e.g. during an animation/transition).
    if (self->field_e9 != 0) {
        if (self->field_ea == 0) {
            CfPadDataLocal* pad =
                (CfPadDataLocal*)cf::CfGameManager::getCfPadData();

            // Direction/stick trigger bits differ between controller types;
            // the whole extraction block is re-run per branch (retail reloads
            // both flag words inside each branch).
            u32 trigger1, trigger2, dirButton, cancelButton;
            if (func_80086F9C__Q22cf13CfGameManagerFv(-1) != 0) {
                // Classic controller: trigger bits 21, 22 from right.
                u32 f = pad->mTurboPressButtonFlags;   // lwz r0
                u32 p = pad->mPadPressedFlags;          // lwz r4
                // 0x10008 = bit16|bit3; exceeds the 16-bit andi immediate, so
                // MWCC masks it via rlwinm+rlwimi keeping the bit positions.
                u32 dirVal = f & 0x8004;
                u32 cancelVal = f & 0x10008;
                trigger1 = (p >> 21) & 1;
                trigger2 = (p >> 22) & 1;
                // (x | -x) >> 31 converts any non-zero value to 1, zero to 0.
                dirButton = (u32)(-(s32)dirVal | dirVal) >> 31;
                cancelButton = (u32)(-(s32)cancelVal | cancelVal) >> 31;
            } else {
                // Wiimote/Nunchuk: trigger bits 4, 5 from right.
                u32 f = pad->mTurboPressButtonFlags;
                u32 p = pad->mPadPressedFlags;
                u32 dirVal = f & 0x8004;
                u32 cancelVal = f & 0x10008;
                trigger1 = (p >> 4) & 1;
                trigger2 = (p >> 5) & 1;
                dirButton = (u32)(-(s32)dirVal | dirVal) >> 31;
                cancelButton = (u32)(-(s32)cancelVal | cancelVal) >> 31;
            }

            // Each branch handles one input and returns.
            if (trigger1 != 0) {
                if (func_802B775C(&self->mSub) != 0) {
                    self->field_e8 = 8;
                }
                return;
            }
            if (trigger2 != 0) {
                self->field_e8 = 7;
                func_802B75D8(&self->mSub);
                return;
            }
            if (dirButton != 0) {
                func_802B7650(&self->mSub);
                return;
            }
            if (cancelButton != 0) {
                func_802B76D4(&self->mSub);
            }
        }
    }
}

void func_802B6200(CMenuTitleInput* self) {
    // Non-zero sub-menu query -> re-open the sub-menu transition state.
    if (func_802B7590(&self->mSub)) {
        self->field_e8 = 0x2;
    }
}

void func_802B6240(CMenuTitleInput* self) {
    if (func_802B7590(&self->mSub)) {
        self->field_e8 = 0x9;
    }
}

extern "C" void func_802B6280(void) {}

extern "C" void func_802B6314(void* self) { ((void(*)(void*))__dt__10CMenuTitleFv)((char*)self - 0x54); }

extern "C" void func_802B631C(void* self) { ((void(*)(void*))cbRenderBefore__10CMenuTitleFv)((char*)self - 0x58); }

extern "C" void func_802B6324(void* self) { ((void(*)(void*))__dt__10CMenuTitleFv)((char*)self - 0x58); }

// --- CTTask<CMenuTitle> out-of-line specializations ---
// MWCC lowers the PTMF dispatch to the retail __ptmf_test / __ptmf_scall calls
// (r3=this, r12=&ptmf); static_cast mirrors CTTask.hpp's inline body.
template<>
void CTTask<CMenuTitle>::Move() {
    if (mMoveFunc) {
        (static_cast<CMenuTitle*>(this)->*mMoveFunc)();
    }
}

template<>
void CTTask<CMenuTitle>::Draw() {
    if (mDrawFunc) {
        (static_cast<CMenuTitle*>(this)->*mDrawFunc)();
    }
}
