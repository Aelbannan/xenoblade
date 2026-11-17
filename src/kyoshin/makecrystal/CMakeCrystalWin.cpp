// Auto-scaffolded catalog TU for kyoshin/makecrystal/CMakeCrystalWin
// Replace stubs with high-level C/C++ during decomp.

// NOTE: do not include kyoshin/harness_catalog.hpp here - its CTaskGameEff ->
// CfGameManager chain declares lbl_eu_80664090 as void*, which conflicts with
// the char* declaration in CMCCrystalBox.hpp. Nothing in this TU needs it.
#include <nw4r/lyt/lyt_drawInfo.h>
#include "kyoshin/makecrystal/CMakeCrystalWin.hpp"
#include "monolib/device/CDeviceVI.hpp"
#include <revolution/gx/GXPixel.h>
#include "kyoshin/CTitleAHelpImports.hpp"
void playUISound(u32);

/*
 * Constructor (retail unmangled `__ct__CMakeCrystalWin`). Written as a
 * C-ABI global like __ct__CSystemWindow because the retail symbol carries no
 * class-length mangling. Sets the primary + IScnRender sub-vtables, stores
 * the two ctor args, zeroes the scene/thread slots, then constructs the four
 * subobjects (CTitleAHelp/CModelDispMakeCrystal/CMCCrystalBox/CLoad) and
 * clears the two state bytes.
 */
#pragma push
#pragma optimize_for_size on
extern "C" CMakeCrystalWin* __ct__CMakeCrystalWin(CMakeCrystalWin* _this, u32 arg4, u32 arg5) {
    _this->field_08 = arg4;
    _this->field_0C = arg5;
    _this->mVtbl = (u32)lbl_eu_80535B70;
    _this->mScnRenderVtbl = (u32)lbl_eu_80535B70 + 0x88;
    _this->mScene = 0;
    _this->mThread = 0;
    __ct__11CTitleAHelpFPcUc(&_this->mTitleAHelp, 0, 0);
    __ct__CModelDispMakeCrystal(reinterpret_cast<CModelDispMakeCrystal*>(&_this->mModelDispArea[0]), 0);
    __ct__13CMCCrystalBoxFUc(&_this->mCrystalBox, 0);
    __ct__CLoad(&_this->mLoad, 0);
    _this->field_4360 = 0;
    _this->field_4361 = 0;
    return _this;
}
#pragma pop

/*
 * Destructor. Written as a free function with an explicit flags parameter
 * (same scheme as CMenuSave's dtor): the embedded CModelDispMakeCrystal
 * class header declares no fields yet, so a real member destructor cannot
 * emit the retail __dt__21CModelDispMakeCrystalFv call at the right offset.
 * Subobjects are destroyed in reverse construction order with the -1
 * subobject flag, then conditional operator delete when flags > 0.
 */
#pragma push
#pragma optimize_for_size on
extern "C" CMakeCrystalWin* __dt__15CMakeCrystalWinFv(CMakeCrystalWin* _this, int flags) {
    if (_this != 0) {
        __dt__5CLoadFv(&_this->mLoad, -1);
        __dt__13CMCCrystalBoxFv(&_this->mCrystalBox, -1);
        __dt__21CModelDispMakeCrystalFv(reinterpret_cast<CModelDispMakeCrystal*>(&_this->mModelDispArea), -1);
        __dt__11CTitleAHelpFv(&_this->mTitleAHelp, -1);
        if (flags > 0) {
            operator delete(_this);
        }
    }
    return _this;
}
#pragma pop

void mkCrystalBuildWin(){}

// Retail 0x802147F4: tear down the crystal window. Removes the render
// callback from the owning scene, releases all four subobjects, flags the
// scene bytes, detaches the child work thread's view and clears it.
void mkCrystalTeardown(CMakeCrystalWin* self) {
    CDeviceVI::waitForDrawDone();

    CScn* scene = self->mScene;
    if (scene != 0) {
        // The `if (self)` splits mr r4,r30 / beq / addi r4,+0x4 (MWCC idiom
        // for the IScnRender subobject at +0x04).
        IScnRender* render = reinterpret_cast<IScnRender*>(self);
        if (self != 0) {
            render = reinterpret_cast<IScnRender*>(&self->mScnRenderVtbl);
        }
        scene->removeRenderCB(render);
    }

    teardown(&self->mTitleAHelp);
    MakeCrystal_CleanupFiles(reinterpret_cast<CModelDispMakeCrystal*>(&self->mModelDispArea[0]));
    self->mCrystalBox.teardownCrystalBox();
    CLoadTeardownLayout(&self->mLoad);

    CScn* scene2 = self->mScene;
    if (scene2 != 0) {
        unregisterEffectScene(scene2);
        // Retail re-reads mScene for every byte store (no CSE across the
        // pointer writes).
        self->mScene->unk_3E4 = 1;
        self->mScene->SetRemove();
        self->mScene = 0;
    }

    CWorkThread* thread = self->mThread;
    if (thread != 0) {
        // CTaskGame+0x6C holds the parent process passed to the ctor (typed
        // CWorkThread* by __ct__9CTaskGameFP5CViewP11CWorkThreadi).
        static_cast<CProc*>(CTaskGame::getInstance()->unk6C)->pssDetachView(thread->mWorkID);
        self->mThread->wkSetEvent(CWorkThread::EVT_NONE);
        self->mThread = 0;
    }
}


void mkCrystalTick(){}

u8 mkCrystalGetFlag4361(void* self) { return ((CMakeCrystalWin*)self)->field_4361; }

// Retail 0x802149C8: returns 0 only when the game is not busy and the crystal
// box is inactive and the window state is a non-zero value below 7.
int CMakeCrystalWin::mkCrystalIsBusyState() {
    if (MenuTutorialIsCreated() != 0) {
        return 1;
    }
    if (this->mCrystalBox.getCrystalBoxSysWin() != 0) {
        return 1;
    }
    u8 state = this->field_4360;
    int result = 1;
    if (state < 7 && state != 0) {
        result = 0;
    }
    return result;
}

u32 mkCrystalGetWord10(void* self) { return (u32)((CMakeCrystalWin*)self)->mScene; }

// Retail 0x80212BE8: when the banner, crystal box and loading overlay are all
// ready, dismiss them, mark the window state as 1 and play the crystal sound.
void mkCrystalDismissToState1(CMakeCrystalWin* self) {
    if (isInitialized(&self->mTitleAHelp) != 0) {
        if (self->mCrystalBox.isCrystalBoxReady() != 0) {
            if (CLoadIsLoadReady(&self->mLoad) != 0) {
                func_801C412C(&self->mTitleAHelp);
                self->mCrystalBox.openCrystalBox();
                self->field_4360 = 1;
                playUISound(0x6d);
            }
        }
    }
}

// Retail 0x80214AB8: once the title-help banner is idle and the crystal box
// is active, mark the window state as 2.
void mkCrystalToState2Idle(CMakeCrystalWin* self) {
    if (self->mTitleAHelp.isIdle() != 0) {
        if (self->mCrystalBox.getCrystalBoxActive() != 0) {
            self->field_4360 = 2;
        }
    }
}

void mkCrystalToState3Box(){}

// Retail 0x80214D2C: same gate as mkCrystalToState2Idle but flags the secondary
// state byte (field_4361) instead.
void mkCrystalSetFlag4361(CMakeCrystalWin* self) {
    if (self->mTitleAHelp.isIdle() != 0) {
        if (self->mCrystalBox.getCrystalBoxActive() != 0) {
            self->field_4361 = 1;
        }
    }
}

// Retail 0x80214D7C: when the crystal box is active, switch the title-help
// banner to mode 0x37 and set the window state to 5.
void mkCrystalToState5Banner(CMakeCrystalWin* self) {
    if (self->mCrystalBox.getCrystalBoxActive() != 0) {
        func_801C41E8(&self->mTitleAHelp, 0x37);
        self->field_4360 = 5;
    }
}

void mkCrystalToState6Box(){}

// Retail 0x80214F58: same gate as mkCrystalToState5Banner but with banner mode 0x34
// and window state 2.
void mkCrystalToState2Banner(CMakeCrystalWin* self) {
    if (self->mCrystalBox.getCrystalBoxActive() != 0) {
        func_801C41E8(&self->mTitleAHelp, 0x34);
        self->field_4360 = 2;
    }
}

// Retail 0x80214FA4: once the title-help banner is idle and the crystal box
// is active, sync the crystal cursor, update the display selection, and show
// the load overlay when either selection index reaches 3.
void mkCrystalToState8Box(CMakeCrystalWin* self) {
    if (self->mTitleAHelp.isIdle() != 0) {
        if (self->mCrystalBox.getCrystalBoxActive() != 0) {
            self->mCrystalBox.teardownCrystalBox();
            u32 cursor = (u32)self->mCrystalBox.rebuildCrystalBoxSelTable();
            setCrystalCursor(reinterpret_cast<CModelDispMakeCrystal*>(&self->mModelDispArea[0]), cursor);
            u8 a = self->mCrystalBox.getCrystalBoxKindA();
            u8 b = self->mCrystalBox.getCrystalBoxKindB();
            MakeCrystal_UnlinkFileSlot(reinterpret_cast<CModelDispMakeCrystal*>(&self->mModelDispArea[0]), a, b);
            if ((u32)a >= 3 || (u32)b >= 3) {
                CLoadStartFadeInStep(&self->mLoad);
            }
            self->field_4360 = 8;
        }
    }
}


// Retail 0x80215058: when the model display is ready, show it, advance the
// loading overlay animation, and set the window state to 9.
void mkCrystalToState9Ready(CMakeCrystalWin* self) {
    if (MakeCrystal_CheckReady(reinterpret_cast<CModelDispMakeCrystal*>(&self->mModelDispArea)) != 0) {
        MakeCrystal_StartCharge(reinterpret_cast<CModelDispMakeCrystal*>(&self->mModelDispArea));
        CLoadStartRetryStep(&self->mLoad);
        self->field_4360 = 9;
    }
}

// Retail 0x802150A8: once the title-help banner is idle and the model
// display is ready, mark the window state as 0xa.
void mkCrystalToStateAReady(CMakeCrystalWin* self) {
    if (self->mTitleAHelp.isIdle() != 0) {
        if (getCrystalStateA(reinterpret_cast<CModelDispMakeCrystal*>(&self->mModelDispArea)) != 0) {
            self->field_4360 = 0xa;
        }
    }
}

// Retail 0x802150F8: when the model display is charging (state B), flag the
// window as 0xb and start the banner; always forward the display mode to the
// banner afterwards.
void mkCrystalToStateBCharge(CMakeCrystalWin* self) {
    if (getCrystalStateB(reinterpret_cast<CModelDispMakeCrystal*>(&self->mModelDispArea)) != 0) {
        self->field_4360 = 0xb;
        beginClose(&self->mTitleAHelp);
    }
    int mode = MakeCrystal_GetPromptState(reinterpret_cast<CModelDispMakeCrystal*>(&self->mModelDispArea));
    func_801C41E8(&self->mTitleAHelp, (u8)mode);
}

// Retail 0x80215158: when the title-help banner is idle and the crystal
// model display is charging (state C), rebuild the crystal box and model
// display from stack temps and start the banner; otherwise flag the window
// as not ready.
void mkCrystalRebuildOnChargeC(CMakeCrystalWin* self) {
    if (self->mTitleAHelp.isIdle() != 0) {
        if (getCrystalStateC(reinterpret_cast<CModelDispMakeCrystal*>(&self->mModelDispArea[0])) != 0) {
            self->field_4360 = 0;
            MakeCrystal_CleanupFiles(reinterpret_cast<CModelDispMakeCrystal*>(self->mModelDispArea));

            // Rebuild the crystal box: construct a temp, copy it in, destroy.
            // MWCC assigns stack slots in reverse declaration order, so the
            // CModelDispMakeCrystal temp (retail +0x8) is declared last.
            u8 boxTemp[0x1508];    // CMCCrystalBox temp at +0x2DE0
            u8 dispTemp[0x2DD8];   // CModelDispMakeCrystal temp at +0x8
            __ct__13CMCCrystalBoxFUc(reinterpret_cast<CMCCrystalBox*>(boxTemp), 0);
            MakeCrystalCopyBox(&self->mCrystalBox, reinterpret_cast<const CMCCrystalBox*>(boxTemp));
            __dt__13CMCCrystalBoxFv(reinterpret_cast<CMCCrystalBox*>(boxTemp), -1);
            self->mCrystalBox.initCrystalBoxResources();

            // Rebuild the model display the same way (temp constructed with
            // the owning scene, copied in, destroyed).
            __ct__CModelDispMakeCrystal(reinterpret_cast<CModelDispMakeCrystal*>(dispTemp), self->mScene);
            MakeCrystalCopyDispArea(&self->mModelDispArea[0], &dispTemp[0]);
            __dt__21CModelDispMakeCrystalFv(reinterpret_cast<CModelDispMakeCrystal*>(dispTemp), -1);
            MakeCrystal_InitFiles(reinterpret_cast<CModelDispMakeCrystal*>(self->mModelDispArea));
            func_801C41E8(&self->mTitleAHelp, 0x30);
        } else {
            self->field_4361 = 1;
        }
    }
}


// Render callback: draw the crystal window's four subobjects once the task
// gate is clear. Same guard + stack DrawInfo scheme as
// CSystemWindow::cbRenderBefore.
void CMakeCrystalWin::cbRenderBefore() {
    CTaskGame::getInstance();
    if (CTaskGame::isFlag01Set() || (lbl_eu_80663E28 & 0x200000))
        return;
    if (IsMenuState621F0() == 0)
        return;
    GXSetZMode(GX_FALSE, GX_NEVER, GX_FALSE);
    nw4r::lyt::DrawInfo drawInfo;
    SetWideLocAdjust(&drawInfo);
    MakeCrystal_DrawAll(reinterpret_cast<CModelDispMakeCrystal*>(&mModelDispArea[0]), &drawInfo);
    mCrystalBox.drawCrystalBox(&drawInfo);
    CLoadDrawIfVisible(&mLoad, &drawInfo);
    drawHelp(&mTitleAHelp, &drawInfo);
}

void cbRenderBefore__15CMakeCrystalWinFv(void*);

void mkCrystalRenderBeforeThunk(void* self) { ((void(*)(void*))cbRenderBefore__15CMakeCrystalWinFv)((char*)self - 0x4); }

void mkCrystalDtorThunk04(void* self) { ((void(*)(void*))__dt__15CMakeCrystalWinFv)((char*)self - 0x4); }
