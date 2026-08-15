// Auto-scaffolded catalog TU for kyoshin/menu/CMenuTitle
// Replace stubs with high-level C/C++ during decomp.

#include <types.h>

#include "monolib/util/MemManager.hpp"
#include "monolib/work/CWorkThreadSystem.hpp"

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

// ---------------------------------------------------------------------------
// __ct__CMenuTitle: allocating factory constructor (retail unmangled symbol).
// Allocates a fresh 0xF0-byte CMenuTitle, builds it (CProcess base, the two
// null task-callback ptmf records, the two secondary-base vtable pointers,
// the embedded CTitle), registers it under `_this` and returns the stored
// singleton. Returns 0 when a singleton already exists.
// ---------------------------------------------------------------------------
CMenuTitle* __ct__CMenuTitle(CMenuTitle* _this, CProcess* parent, u32 arg2) {
    if (lbl_eu_80664C30 != 0) {
        return 0;
    }

    CMenuTitleCtorShim* shim = (CMenuTitleCtorShim*)mtl::MemManager::allocate(
        0xf0, CWorkThreadSystem::getWorkMem());

    if (shim != 0) {
        // CProcess base first; the vtable slot is then overwritten with the
        // final CMenuTitle vtable (retail stores the intermediate vtable
        // before the callbacks, the final one last).
        __ct__8CProcessFv((CProcess*)shim);
        shim->vtable = lbl_eu_8053B1AC;

        // Two null member-function-pointer records (task callbacks), built
        // from the compiler's __ptmf_null table. Statement order mirrors the
        // retail load/store interleave (ptmf2 is loaded only after cb1).
        u32* ptmf = __ptmf_null;
        char* vtFinal = lbl_eu_8053B0B8;
        u32 ptmf1 = ptmf[1];
        u32 ptmf0 = ptmf[0];
        char* vt54 = vtFinal + 0x24; // secondary-base vtable slot +0x54
        shim->callbacks[0] = ptmf0;
        char* vt58 = vtFinal + 0xac; // secondary-base vtable slot +0x58
        shim->callbacks[1] = ptmf1;
        u32 ptmf2 = ptmf[2];
        shim->callbacks[2] = ptmf2;
        ptmf1 = ptmf[1];
        ptmf0 = ptmf[0];
        shim->callbacks[3] = ptmf0;
        shim->callbacks[4] = ptmf1;
        ptmf2 = ptmf[2];
        shim->callbacks[5] = ptmf2;

        shim->vtable = vtFinal;
        shim->field54 = vt54;
        shim->field58 = vt58;
        shim->parent = parent;

        __ct__CTitle(&shim->mTitle);
        shim->field_e8 = 0;
        shim->field_e9 = 1;
        shim->field_ea = 1;
        shim->field_ec = lbl_eu_80668FD0;
        shim->mTitle.field_0x20 = (CTitleAction*)arg2;
    }

    // Store the singleton (possibly null) and register as a child of `_this`;
    // the return value is the stored singleton, reloaded after the call.
    lbl_eu_80664C30 = (CMenuTitle*)shim;
    ((CProcess*)shim)->Regist(_this, false);
    return lbl_eu_80664C30;
}

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
    // field_ea state (e.g. during an animation/transition). The if/else-if
    // form is what makes MWCC emit the retail two-branch guard (beq/beq/b).
    if (self->field_e9 == 0) {
        return;
    } else if (self->field_ea != 0) {
        return;
    }

    CfPadDataLocal* pad =
        (CfPadDataLocal*)cf::CfGameManager::getCfPadData();

    // Direction/stick trigger bits differ between controller types; retail
    // re-runs the whole extraction block per branch (both flag words reloaded).
    u32 trigger1, trigger2, dirButton, cancelButton;
    if (func_80086F9C__Q22cf13CfGameManagerFv(-1) != 0) {
        // Classic controller: trigger bits 21, 22 from right.
        u32 f = pad->mTurboPressButtonFlags;   // lwz r0
        u32 p = pad->mPadPressedFlags;          // lwz r4
        // 0x10008 = bit16|bit3; exceeds the andi immediate, so MWCC masks it
        // via rlwinm+rlwimi keeping the bit positions.
        u32 dirVal = f & 0x8004;
        u32 cancelVal = f & 0x10008;
        trigger1 = (p >> 21) & 1;
        trigger2 = (p >> 22) & 1;
        // (x | -x) >> 31 converts any non-zero value to 1, zero to 0.
        cancelButton = (u32)(-(s32)cancelVal | cancelVal) >> 31;
        dirButton = (u32)(-(s32)dirVal | dirVal) >> 31;
    } else {
        // Wiimote/Nunchuk: trigger bits 4, 5 from right.
        u32 f = pad->mTurboPressButtonFlags;
        u32 p = pad->mPadPressedFlags;
        u32 dirVal = f & 0x8004;
        u32 cancelVal = f & 0x10008;
        trigger1 = (p >> 4) & 1;
        trigger2 = (p >> 5) & 1;
        cancelButton = (u32)(-(s32)cancelVal | cancelVal) >> 31;
        dirButton = (u32)(-(s32)dirVal | dirVal) >> 31;
    }

    // Each branch handles one input; every path falls through to the exit.
    if (trigger1 != 0) {
        if (func_802B775C(&self->mSub) != 0) {
            self->field_e8 = 8;
        }
    } else if (trigger2 != 0) {
        self->field_e8 = 7;
        func_802B75D8(&self->mSub);
    } else if (dirButton != 0) {
        func_802B7650(&self->mSub);
    } else if (cancelButton != 0) {
        func_802B76D4(&self->mSub);
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
