// Auto-scaffolded catalog TU for kyoshin/menu/CMenuMapSelectSC
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/cf/CfGameManager.hpp"
#include <nw4r/lyt/lyt_drawInfo.h>
#include "kyoshin/menu/CMenuMapSelectSC.hpp"
#include "kyoshin/CSysWin.hpp"
#include "kyoshin/code_80135FDC_Bdat.hpp"
#include "kyoshin/code_80135FDC_State.hpp"
#include "kyoshin/code_80135FDC_Lyt.hpp"

// Retail-unmangled callee names (US strips mangling for these func_ helpers);
// declared extern "C" so call sites emit the literal retail reloc names.
// Mirrors the CMenuOption/CMenuGCItem pattern for the same helpers. Return
// types are int (not u8) so callers compare with cmpwi directly (no byte
// mask), matching retail.

extern "C" int CFade_IsReady(CFade* self);
extern "C" int CFade_IsVisible(CFade* self);
// (memberized: CFade_FadeIn -> CFade::In; declaration moved into class CFade)
extern "C" void CFade_FadeOut(CFade* self);
extern "C" void CFade_StartLoad(CFade* self);
extern "C" void CFade_Update(CFade* self);

// func_8008413C is declared in CfGameManager.hpp with its retail (u16,u32) form.



// World-map phase helpers (retail-unmangled callee names).
extern "C" void MapSelectSCResetFloorMapIdle(CMenuMapSelectSC* self);
extern "C" void MapSelectSCCommitCursorIdle(CMenuMapSelectSC* self);
extern "C" void MapSelectSCBeginFadeIn(CMenuMapSelectSC* self);
extern "C" void MapSelectSCPrepFloorMap(CMenuMapSelectSC* self);
extern "C" void MapSelectSCShowMapText(CMenuMapSelectSC* self);
extern "C" void MapSelectSCHandleInput(CMenuMapSelectSC* self);

// ---------------------------------------------------------------------------
// Additional retail-unmangled callee names for the world-map (SC) screen.
// ---------------------------------------------------------------------------
// CScn-side helpers: scene enable flag + render-callback management.
// int return (mismatched vs the (void*, u8) definition) blocks -ipa inlining
// of the 1-line body, keeping the retail `bl Scn_SetPauseFlag` call site.
extern "C" int Scn_SetPauseFlag(CScn* scn, int flag);
// CFloorMap helpers (retail unmangles these member helpers).
// (memberized: FloorMap_SetMapTable -> CFloorMap::SetMapTable; declaration moved into class CFloorMap)
// CTitleAHelp / CFade draw + reset helpers.
extern "C" void CFade_Draw(CFade* self, nw4r::lyt::DrawInfo* drawInfo);
// (memberized: CFade_Unload -> CFade::Unload; declaration moved into class CFade)
// UI/sound + scene-active helpers.
extern "C" void UIWin_ControlWindowLists(u32 a, u32 b);
// setPresentationFlag: canonical setPresentationFlag__Q22cf13CfGameManagerFv(bool).
// Ctor/re-init helpers for the embedded widgets (retail-unmangled).
extern "C" void __ct__11CTitleAHelpFPcUc(CTitleAHelp* self, char* name, u8 arg);
extern "C" void __as__17UnkClass_8045F564FRC17UnkClass_8045F564(void* dest, void* src);
// CTitleAHelp::CTitleAHelp_load() is declared in kyoshin/CTitleAHelp.hpp.
extern "C" void __dt__11CTitleAHelpFv(CTitleAHelp* _this, int flags);

#include "kyoshin/CScrollBar.hpp"
// playUISound__FUl / SetWideLocAdjust__FPQ34nw4r3lyt8DrawInfo.

// Data imports (MWCC does not mangle global-scope data names).
extern u32 lbl_eu_80664184;   // current map id (bdat row) -> floor map select
extern u32 gmPauseFlags;   // global mode bitfield (bit 0x400 = world-map gate)
extern u32 __ptmf_null[3];    // null pointer-to-member-function constant

#include "kyoshin/CTaskGame.hpp"

#include <new>
#include <cstring>
#include <revolution/gx/GXPixel.h>
#include "kyoshin/CTitleAHelp.hpp"
#include "kyoshin/CTitleAHelpImports.hpp"

// Out-of-line constructor (retail keeps it as its own function; the member
// initialisers run in declaration order: parent ref, CTitleAHelp, CFade,
// CFloorMap, then the state byte and timer).
CMenuMapSelectSC::CMenuMapSelectSC(CProcess* parent)
    : mParentRef(parent), mTitleAHelp(0, 0), mState(0), mTimer(0.0f) {}

CMenuMapSelectSC::~CMenuMapSelectSC() {}

void CMenuMapSelectSC::Init() {
    setPresentationFlag__Q22cf13CfGameManagerFv(1);

    // --- Re-initialise the embedded CTitleAHelp via a temporary ---
    char* name = BdatTouchStringCell("MNU_map", "name", 1);

    u8 tempTitle[0x38];
    __ct__11CTitleAHelpFPcUc(reinterpret_cast<CTitleAHelp*>(tempTitle), name, 0x4a);

    __as__17UnkClass_8045F564FRC17UnkClass_8045F564(&mTitleAHelp.unk4, &reinterpret_cast<CTitleAHelp*>(tempTitle)->unk4);
    CTitleAHelp* tmpTitle = reinterpret_cast<CTitleAHelp*>(tempTitle);
    mTitleAHelp.mFileHandle = tmpTitle->mFileHandle;
    mTitleAHelp.mArcResourceAccessor = tmpTitle->mArcResourceAccessor;
    mTitleAHelp.mLayout = tmpTitle->mLayout;
    mTitleAHelp.mAnimTrans20 = tmpTitle->mAnimTrans20;
    mTitleAHelp.mAnimTrans24 = tmpTitle->mAnimTrans24;
    mTitleAHelp.unk28 = tmpTitle->unk28;
    mTitleAHelp.unk2c = tmpTitle->unk2c;
    mTitleAHelp.mName = tmpTitle->mName;
    mTitleAHelp.unk34 = tmpTitle->unk34;
    mTitleAHelp.unk35 = tmpTitle->unk35;
    mTitleAHelp.unk36 = tmpTitle->unk36;
    mTitleAHelp.unk37 = tmpTitle->unk37;
    __dt__11CTitleAHelpFv(tmpTitle, -1);

    mTitleAHelp.CTitleAHelp_load();

    // --- Re-initialise the embedded CFade via a temporary ---
    {
        CFade tmpFade;
        __as__17UnkClass_8045F564FRC17UnkClass_8045F564(&mFade.mMemRegion, &tmpFade.mMemRegion);
        mFade.mFileHandle = tmpFade.mFileHandle;
        mFade.mArcResAcc = tmpFade.mArcResAcc;
        mFade.mLayout = tmpFade.mLayout;
        mFade.mAnimTrans = tmpFade.mAnimTrans;
        mFade.mIsLoaded = tmpFade.mIsLoaded;
        mFade.mFadeState = tmpFade.mFadeState;
        mFade.mReady = tmpFade.mReady;
        mFade.mVisible = tmpFade.mVisible;
    }

    CFade_StartLoad(&mFade);

    // --- Re-initialise the embedded CFloorMap via a temporary ---
    mFloorMap = CFloorMap();

    // Register the widget as an IScnRender render callback on its parent scene.
    reinterpret_cast<CScn*>(mParentRef)->addRenderCB(this, 0xd, 1);
}

void CMenuMapSelectSC::Term() {
    CDeviceVI::waitForDrawDone();
    Scn_SetPauseFlag(reinterpret_cast<CScn*>(mParentRef), 1);

    reinterpret_cast<CScn*>(mParentRef)->removeRenderCB(this);

    mTitleAHelp.teardown();
    (&mFade)->Unload();
    mFloorMap.Teardown();

    sInstance = 0;
    setPresentationFlag__Q22cf13CfGameManagerFv(0);
}

void CMenuMapSelectSC::Move() {
    CTaskGame::getInstance();
    // Same gate idiom as cbRenderBefore (see comment there).
    if (CTaskGame::isFlag01Set() == 0 &&
        (gmPauseFlags & 0x200000) == 0) {
        goto body;
    }
    goto end;
end:
    return;
body:
    switch (mState) {
    case 0:
        MapSelectSCBeginFadeIn(this);
        break;
    case 1:
        MapSelectSCPrepFloorMap(this);
        break;
    case 2:
        MapSelectSCShowMapText(this);
        break;
    case 3:
        if (CFade_IsVisible(&mFade) != 0) {
            mState = 4;
        }
        break;
    case 4:
        MapSelectSCHandleInput(this);
        break;
    case 5:
        MapSelectSCResetFloorMapIdle(this);
        break;
    case 6:
        MapSelectSCCommitCursorIdle(this);
        break;
    case 7:
        mField54 = 1;
        break;
    }
    mTitleAHelp.updateHelp();
    CFade_Update(&mFade);
    mFloorMap.UpdateFrame();
}

void CMenuMapSelectSC::cbRenderBefore(CScn*) {
    CTaskGame::getInstance();
    // Gate: skip when the task is busy or the global mode bit (0x200000) is
    // set. The combined `A == 0 && B == 0 -> goto body` chain with the exit
    // label BEFORE the body label reproduces retail's branch-over-branch
    // (`bne end`, then `beq body; b end`).
    if (CTaskGame::isFlag01Set() == 0 &&
        (gmPauseFlags & 0x200000) == 0) {
        goto body;
    }
    goto end;
end:
    return;
body:
    if (IsMenuState621F0() == 0) {
        goto end;
    }
    GXSetZMode(GX_FALSE, GX_NEVER, GX_FALSE);
    nw4r::lyt::DrawInfo drawInfo;
    SetWideLocAdjust(&drawInfo);
    mFloorMap.Draw(&drawInfo);
    mTitleAHelp.drawHelp(&drawInfo);
    CFade_Draw(&mFade, &drawInfo);
}

CMenuMapSelectSC* CMenuMapSelectSC::sInstance;

// Create the world-map (SC) singleton: returns 0 when it already exists,
// otherwise allocates the object (0x3410 bytes), constructs it in place, stores
// the singleton and registers it as a CProcess child of `parent`.
extern "C" CMenuMapSelectSC* MapSelectSCCreate(CProcess* parent, CProcess* arg) {
    if (CMenuMapSelectSC::sInstance != 0) {
        return 0;
    }

    playUISound(4);
    CMenuMapSelectSC* obj = (CMenuMapSelectSC*)mtl::MemManager::allocate(
        0x3410, CWorkThreadSystem::getWorkMem());
    obj = new (obj) CMenuMapSelectSC(arg);
    CMenuMapSelectSC::sInstance = obj;
    obj->Regist(parent, false);
    return CMenuMapSelectSC::sInstance;
}

// (sInstance != 0)
extern "C" bool MapSelectSCIsCreated() { return CMenuMapSelectSC::sInstance != 0; }

// World-map phase 0: wait for the title/help bar and fade to be ready, then
// start the fade-in animation and advance to phase 1.
extern "C" void MapSelectSCBeginFadeIn(CMenuMapSelectSC* self) {
    if (self->mTitleAHelp.isInitialized() && CFade_IsReady(&self->mFade)) {
        (&self->mFade)->In();
        self->mState = 1;
    }
}

// World-map phase 2: once the fade has finished hiding, prepare the floor map
// for the current map id, start its file loads, advance to phase 3 and play
// the confirm sound.
extern "C" void MapSelectSCPrepFloorMap(CMenuMapSelectSC* self) {
    if (CFade_IsVisible(&self->mFade) != 0) {
        Scn_SetPauseFlag(reinterpret_cast<CScn*>(self->mParentRef), 0);
        (&self->mFloorMap)->SetMapTable((u8)lbl_eu_80664184);
        self->mFloorMap.LoadMapFiles();
        self->mState = 2;
        UIWin_ControlWindowLists(1, 0);
    }
}

// World-map phase 2: once the scrollbar is visible, populate the title/help
// bar text for the current map, start its animation and advance to phase 3.
extern "C" void MapSelectSCShowMapText(CMenuMapSelectSC* self) {
    if (self->mFloorMap.GetField42Gated() != 0) {
        self->mTitleAHelp.func_801C4654(0);
        char* name = BdatTouchStringCell("MNU_map", "name", 3);
        self->mTitleAHelp.setInfoText(name);
        if (CtrlRemote_TouchBitByArg(0x3212) != 0) {
            self->mTitleAHelp.func_801C46DC(1);
        }
        self->mTitleAHelp.func_801C41E8(0);
        self->mTitleAHelp.func_801C412C();
        self->mFloorMap.LoadStageLayout();
        playUISound(0x6d);
        CFade_FadeOut(&self->mFade);
        self->mState = 3;
    }
}

// World-map phase 4: floor-map input handler. Accumulates a per-frame timer,
// then dispatches the current pad state to the cursor/selection helpers. The
// pad layout differs between Classic (bits 21-23/25-26) and Wiimote/Nunchuk
// (bits 4-5/10/12) controllers.
extern "C" void MapSelectSCHandleInput(CMenuMapSelectSC* self) {
    if (self->mFloorMap.ClassifyByte41() != 0) {
        return;
    }

    // Explicit intermediate store keeps the unconditional stfs of the
    // incremented value ahead of the clamp compare (retail order:
    // fadds; stfs; fcmpo; ble; stfs).
    f32 t = self->mTimer;
    t += 1.0f;
    f32 timerCap = 10.0f;
    self->mTimer = t;
    if (t > timerCap) {
        self->mTimer = timerCap;
    }

    if (cf::CfGameManager::isClassicController(-1) != 0) {
        // Classic controller layout.
        if (self->mFloorMap.CheckRowVisited() != 0) {
            if (self->mFloorMap.GetBdatMaxRow() > 1 && self->mFloorMap.GetField208Plain() != 0) {
                self->mTitleAHelp.func_801C41E8(0x4b);
            } else {
                self->mTitleAHelp.func_801C41E8(0x4c);
            }
        } else {
            if (self->mFloorMap.GetBdatMaxRow() > 1 && self->mFloorMap.GetField208Plain() != 0) {
                self->mTitleAHelp.func_801C41E8(0x4d);
            } else {
                self->mTitleAHelp.func_801C41E8(0x4e);
            }
        }
        self->mFloorMap.SetPaneFlag(0);

        CPad* pad = cf::CfGameManager::getCurrentPad();
        if ((pad->mTurboPressButtonFlags & 0x02000000) != 0 &&
            self->mFloorMap.GetBdatMaxRow() > 1 &&
            self->mFloorMap.GetField208Gated() != 0 &&
            self->mFloorMap.IsSysWinBusy() == 0) {
            self->mFloorMap.UpdateRowPanePos();
            return;
        }
        if ((pad->mTurboPressButtonFlags & 0x04000000) != 0 &&
            self->mFloorMap.GetBdatMaxRow() > 1 &&
            self->mFloorMap.GetField208Gated() != 0 &&
            self->mFloorMap.IsSysWinBusy() == 0) {
            self->mFloorMap.CursorNextFloor();
            return;
        }

        u32 pressed = pad->mPressedButtonFlags;
        if ((pressed & 0x00200000) != 0) {
            self->mFloorMap.OpenRowWindow();
            if (self->mFloorMap.GetU16Field5A() != 0) {
                (&self->mFade)->In();
                self->mState = 6;
            }
        } else if ((pressed & 0x00400000) != 0) {
            if (self->mFloorMap.GetActiveFlag58() != 0) {
                self->mFloorMap.ConfirmSelect();
            } else if (self->mFloorMap.IsSysWinBusy() == 0) {
                (&self->mFade)->In();
                self->mState = 5;
                playUISound(6);
            } else {
                self->mFloorMap.OpenRowWindow();
            }
        } else if ((pressed & 0x00800000) != 0) {
            if (self->mFloorMap.IsSysWinBusy() == 0) {
                (&self->mFade)->In();
                self->mState = 5;
                playUISound(6);
            }
        } else if ((pad->mTurboPressButtonFlags & 0x4) != 0) {
            self->mFloorMap.UpdateCursorMove();
        } else if ((pad->mTurboPressButtonFlags & 0x8) != 0) {
            self->mFloorMap.UpdateWorldMap();
        } else if ((pad->mTurboPressButtonFlags & 0x80000000) != 0) {
            self->mFloorMap.HandleMapDown();
        } else if ((pad->mTurboPressButtonFlags & 0x2) != 0) {
            self->mFloorMap.NormalizeMapPos();
        } else if ((pressed & 0x01000000) != 0) {
            self->mFloorMap.Log2Field208();
        } else if ((pressed & 0x200) != 0) {
            self->mFloorMap.Log2Field333C();
        } else {
            // D-pad held: each direction moves the cursor once per press
            // transition; when no direction is held, confirm the selection.
            u32 flag = 1;
            if ((pad->mHeldButtonFlags & 0x8000) != 0) {
                self->mFloorMap.ScrollDown();
                flag = 0;
            }
            if ((pad->mHeldButtonFlags & 0x10000) != 0) {
                self->mFloorMap.ScrollUp();
                flag = 0;
            }
            if ((pad->mHeldButtonFlags & 0x2000) != 0) {
                self->mFloorMap.ScrollRight();
                flag = 0;
            }
            if ((pad->mHeldButtonFlags & 0x4000) != 0) {
                self->mFloorMap.ScrollLeft();
                flag = 0;
            }
            if (flag != 0) {
                self->mFloorMap.MoveToCursorRow();
            }
        }
    } else {
        // Wiimote/Nunchuk layout.
        if (self->mFloorMap.CheckRowVisited() != 0) {
            if (self->mFloorMap.GetBdatMaxRow() > 1 && self->mFloorMap.GetField208Plain() != 0) {
                self->mTitleAHelp.func_801C41E8(0x4b);
            } else {
                self->mTitleAHelp.func_801C41E8(0x4c);
            }
        } else {
            if (self->mFloorMap.GetBdatMaxRow() > 1 && self->mFloorMap.GetField208Plain() != 0) {
                self->mTitleAHelp.func_801C41E8(0x4d);
            } else {
                self->mTitleAHelp.func_801C41E8(0x4e);
            }
        }
        self->mFloorMap.SetPaneFlag(0);

        CPad* pad = cf::CfGameManager::getCurrentPad();
        if ((pad->mHeldButtonFlags & 0x1000) != 0 &&
            self->mFloorMap.GetBdatMaxRow() > 1 &&
            self->mFloorMap.GetField208Gated() != 0 &&
            self->mFloorMap.IsSysWinBusy() == 0) {
            self->mTitleAHelp.func_801C41E8(0x4f);
            if (self->mTimer > 1.0f) {
                playUISound(2);
            }
            self->mTimer = 0.0f;
            self->mFloorMap.SetPaneFlag(1);
            if ((pad->mTurboPressButtonFlags & 0x4) != 0) {
                self->mFloorMap.UpdateRowPanePos();
            } else if ((pad->mTurboPressButtonFlags & 0x8) != 0) {
                self->mFloorMap.CursorNextFloor();
            }
        } else {
            u32 pressed = pad->mPressedButtonFlags;
            if ((pressed & 0x10) != 0) {
                self->mFloorMap.OpenRowWindow();
                if (self->mFloorMap.GetU16Field5A() != 0) {
                    (&self->mFade)->In();
                    self->mState = 6;
                }
            } else if ((pressed & 0x20) != 0) {
                if (self->mFloorMap.GetActiveFlag58() != 0) {
                    self->mFloorMap.ConfirmSelect();
                } else if (self->mFloorMap.IsSysWinBusy() == 0) {
                    (&self->mFade)->In();
                    self->mState = 5;
                    playUISound(6);
                } else {
                    self->mFloorMap.OpenRowWindow();
                }
            } else if ((pressed & 0x400) != 0) {
                if (self->mFloorMap.IsSysWinBusy() == 0) {
                    (&self->mFade)->In();
                    self->mState = 5;
                    playUISound(6);
                }
            } else if ((pad->mTurboPressButtonFlags & 0x4) != 0) {
                self->mFloorMap.UpdateCursorMove();
            } else if ((pad->mTurboPressButtonFlags & 0x8) != 0) {
                self->mFloorMap.UpdateWorldMap();
            } else if ((pad->mTurboPressButtonFlags & 0x80000000) != 0) {
                self->mFloorMap.HandleMapDown();
            } else if ((pad->mTurboPressButtonFlags & 0x2) != 0) {
                self->mFloorMap.NormalizeMapPos();
            } else if ((pressed & 0x800) != 0) {
                self->mFloorMap.Log2Field208();
            } else if ((pressed & 0x200) != 0) {
                self->mFloorMap.Log2Field333C();
            } else {
                u32 flag = 1;
                if ((pad->mHeldButtonFlags & 0x8000) != 0) {
                    self->mFloorMap.ScrollDown();
                    flag = 0;
                }
                if ((pad->mHeldButtonFlags & 0x10000) != 0) {
                    self->mFloorMap.ScrollUp();
                    flag = 0;
                }
                if ((pad->mHeldButtonFlags & 0x2000) != 0) {
                    self->mFloorMap.ScrollRight();
                    flag = 0;
                }
                if ((pad->mHeldButtonFlags & 0x4000) != 0) {
                    self->mFloorMap.ScrollLeft();
                    flag = 0;
                }
                if (flag != 0) {
                    self->mFloorMap.MoveToCursorRow();
                }
            }
        }
    }
}

// World-map phase 6: wait for the fade to finish hiding, then reset the
// floor map and move to phase 7 (idle).
extern "C" void MapSelectSCResetFloorMapIdle(CMenuMapSelectSC* self) {
    if (CFade_IsVisible(&self->mFade)) {
        self->mFloorMap.Teardown();
        self->mState = 7;
    }
}

// World-map phase 6 variant: after the fade completes, hand the selected
// floor-map cursor index to the game manager and move to phase 7 (idle).
extern "C" void MapSelectSCCommitCursorIdle(CMenuMapSelectSC* self) {
    if (CFade_IsVisible(&self->mFade)) {
        cf::CfGameManager::func_8008413C(self->mFloorMap.GetU16Field5A(), 0);
        self->mState = 7;
    }
}
