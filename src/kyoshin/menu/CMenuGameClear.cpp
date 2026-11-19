// Auto-scaffolded catalog TU for kyoshin/menu/CMenuGameClear
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/menu/CMenuGameClear.hpp"
#include "kyoshin/cf/CtrlObjectParam.hpp"

#include "kyoshin/CBaseCur.hpp"
#include "kyoshin/CTaskGame.hpp"
#include "monolib/device/CDeviceVI.hpp"
#include "monolib/scn/CScn.hpp"
#include "monolib/scn/IScnRender.hpp"
#include "monolib/util/MemManager.hpp"
#include "monolib/work/CWorkThreadSystem.hpp"

#include <nw4r/lyt/lyt_drawInfo.h>
#include <revolution/GX.h>
#include "kyoshin/code_80135FDC_State.hpp"
#include "kyoshin/code_80135FDC_Lyt.hpp"

// CSysWin.hpp declares typed __ct__/__dt__17UnkClass_8045F564Fv forms that
// clash with the void* forms in CItemBoxInfo.hpp (via CtrlObjectParam.hpp,
// included by the header); hide them while CSysWin.hpp is processed so the
// void* forms own the real names (same trick as CItemBoxGrid.cpp). This TU only
// touches UnkClass_8045F564 storage through the void* free functions.
#include "kyoshin/CSysWin.hpp"
#include "kyoshin/cf/CfGameManager.hpp"

// cf pad data view used by the input handlers (fields at +0x04/+0x104).
struct CfPadDataView {
    u8 _00[4];
    u32 field_04;               // direction-flag word
    u8 _08[0x104 - 0x08];
    u32 field_104;              // button word
};

// DrawInfo helper (retail mangled name SetWideLocAdjust__FPQ34nw4r3lyt8DrawInfo).

// C-ABI imports (retail symbols are unmangled / pre-mangled C names).
extern "C" {
void deleteRegion__17UnkClass_8045F564Fv(void* region);
void* CtrlObjectParam_InitEquipSlots(cf::CtrlObjectParamInit* p);
// setPresentationFlag: canonical setPresentationFlag__Q22cf13CfGameManagerFv(bool).
}

// Global data imports (MWCC does not mangle global-scope data names).
extern u32 lbl_eu_80664C10;              // game-clear state word (.sbss)
extern u32 lbl_eu_80663E28;              // task-state flag word (bit 21 gate)


// CMenuGameClear singleton storage.
CMenuGameClear* CMenuGameClear::sInstance;

// The member-initialiser list covers the compiler-built members (scene, region,
// state bytes, cursor); the CSysWin storage and the 14 CtrlObjectParam entries
// are built here through the C-ABI ctor / init helpers.
inline CMenuGameClear::CMenuGameClear(CScn* scene)
    : mScene(scene), mMemRegion(), mField70(0), mField71(0), mSysWin(0), mCursor(nullptr) {
    cf::CtrlObjectParamInit* p = &mParams[0];
    cf::CtrlObjectParamInit* end = &mParams[14];
    do {
        CtrlObjectParam_InitEquipSlots(p);
        p++;
    } while (p < end);
}

/*
 * Singleton factory. Allocates the game-clear screen through the work-thread
 * placement new (the ctor builds the IUICf base, the members and the
 * CtrlObjectParam entry array), registers it under `parent` and returns the
 * stored instance (0 if one already exists).
 */
CMenuGameClear* CMenuGameClear::create(CProcess* parent, CScn* scene) {
    if (sInstance != 0) {
        return 0;
    }

    CMenuGameClear* menu = new (CWorkThreadSystem::getWorkMem()) CMenuGameClear(scene);
    sInstance = menu;
    menu->Regist(parent, false);
    return sInstance;
}

/*
 * Complete-object destructor. CCur18 and the region are destroyed by the
 * compiler; the CSysWin storage is destroyed here through its C-ABI dtor.
 */
CMenuGameClear::~CMenuGameClear() {}

void CMenuGameClear::Init() {}

/*
 * Tear the game-clear screen down: wait for the draw callback to finish,
 * remove the render callback from the owning scene, release the CSysWin /
 * CCur18 / region widgets, clear the singleton and re-enable the cf pad
 * flags.
 */
void CMenuGameClear::Term() {
    CDeviceVI::waitForDrawDone();

    mScene->removeRenderCB(this);

    mSysWin.termLayout();
    ((CBaseCur*)&mCursor)->cleanup();
    deleteRegion__17UnkClass_8045F564Fv(&mMemRegion);

    sInstance = 0;

    cf::CfGameManager::enablePadFlags(-1, false);
    setPresentationFlag__Q22cf13CfGameManagerFv(0);
}

void CMenuGameClear::Move() {}

/*
 * Render the game-clear screen through a stack DrawInfo. The empty if/else-if
 * branches reproduce the retail `bne`/`beq + b` gate shape (task busy bit,
 * then the global flag word, then the scene-active check).
 */
void CMenuGameClear::cbRenderBefore() {
    CTaskGame::getInstance();
    // Gate: skip when the task is busy or the global mode bit (0x200000) is
    // set. The `if (A && B) goto body; goto end; end: return; body:` chain
    // (exit label BEFORE the body label) keeps the body off the fallthrough
    // so MWCC emits retail's branch-over-branch: `bne end` for the first
    // disjunct, `beq body; b end` for the second (MWCC_CASES
    // sec.&&-gate branch-over-branch).
    if (CTaskGame::isFlag01Set() == 0 &&
        (lbl_eu_80663E28 & 0x200000) == 0) {
        goto body;
    }
    goto end;
end:
    return;
body:
    if (IsMenuState621F0() != 0) {
        GXSetZMode(GX_FALSE, GX_NEVER, GX_FALSE);
        nw4r::lyt::DrawInfo drawInfo;
        SetWideLocAdjust(&drawInfo);
        mSysWin.drawLayout(reinterpret_cast<nw4r::lyt::DrawInfo*>(&drawInfo));
        mCursor.DrawLayout(&drawInfo);
    }
}

extern "C" unsigned long ClearMenu_IsPresent() {
    return CMenuGameClear::sInstance != 0;
}

// Shared input-handler shape (ClearMenu_ConfirmInput selects -> state 4,
// ClearMenu_CancelInput backs out -> state 8). Reads the cf pad button word:
// bit21/bit27 of +0x04 (port-dependent) is the confirm stroke, 0x8004 moves
// the cursor up, bits 15/28 move it down.
extern "C" void ClearMenu_ConfirmInput(CMenuGameClear* self) {
    CfPadDataView* pad = (CfPadDataView*)cf::CfGameManager::getCfPadData();
    u32 buttons;
    int dirBit;
    int down;
    int up;
    if (cf::CfGameManager::isClassicController(-1) != 0) {
        buttons = pad->field_104;
        dirBit = (pad->field_04 >> 21) & 1;
        down = (buttons & 0x8004) != 0;
        up = (buttons & 0x1010000) != 0;
    } else {
        buttons = pad->field_104;
        dirBit = (pad->field_04 >> 4) & 1;
        down = (buttons & 0x8004) != 0;
        up = (buttons & 0x1010000) != 0;
    }

    if (dirBit != 0) {
        self->mField70 = 4;
        self->mSysWin.advancePhase3();
        self->mCursor.SetVisible(0);
        playUISound(3);
    } else if (down != 0) {
        if (--self->mField71 < 0) {
            self->mField71 = 1;
        }
        nw4r::math::VEC3 pos;
        CSysWin::getPaneScreenPos(reinterpret_cast<nw4r::math::VEC3*>(&pos), &self->mSysWin, self->mField71);
        ((CBaseCur*)&self->mCursor)->setRootPaneTranslate(&pos);
        playUISound(1);
    } else if (up != 0) {
        if (++self->mField71 > 1) {
            self->mField71 = 0;
        }
        nw4r::math::VEC3 pos;
        CSysWin::getPaneScreenPos(reinterpret_cast<nw4r::math::VEC3*>(&pos), &self->mSysWin, self->mField71);
        ((CBaseCur*)&self->mCursor)->setRootPaneTranslate(&pos);
        playUISound(1);
    }
}

extern "C" void ClearMenu_CancelInput(CMenuGameClear* self) {
    CfPadDataView* pad = (CfPadDataView*)cf::CfGameManager::getCfPadData();
    u32 buttons;
    int dirBit;
    int down;
    int up;
    if (cf::CfGameManager::isClassicController(-1) != 0) {
        buttons = pad->field_104;
        dirBit = (pad->field_04 >> 21) & 1;
        down = (buttons & 0x8004) != 0;
        up = (buttons & 0x1010000) != 0;
    } else {
        buttons = pad->field_104;
        dirBit = (pad->field_04 >> 4) & 1;
        down = (buttons & 0x8004) != 0;
        up = (buttons & 0x1010000) != 0;
    }

    if (dirBit != 0) {
        self->mField70 = 8;
        self->mSysWin.advancePhase3();
        self->mCursor.SetVisible(0);
        playUISound(3);
    } else if (down != 0) {
        if (--self->mField71 < 0) {
            self->mField71 = 1;
        }
        nw4r::math::VEC3 pos;
        CSysWin::getPaneScreenPos(reinterpret_cast<nw4r::math::VEC3*>(&pos), &self->mSysWin, self->mField71);
        ((CBaseCur*)&self->mCursor)->setRootPaneTranslate(&pos);
        playUISound(1);
    } else if (up != 0) {
        if (++self->mField71 > 1) {
            self->mField71 = 0;
        }
        nw4r::math::VEC3 pos;
        CSysWin::getPaneScreenPos(reinterpret_cast<nw4r::math::VEC3*>(&pos), &self->mSysWin, self->mField71);
        ((CBaseCur*)&self->mCursor)->setRootPaneTranslate(&pos);
        playUISound(1);
    }
}

extern "C" u32 ClearMenu_GetGlobal10() {
    return lbl_eu_80664C10;
}
