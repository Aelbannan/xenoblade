// Auto-scaffolded catalog TU for kyoshin/makecrystal/CMakeCrystalWin
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/harness_catalog.hpp"

#include "kyoshin/makecrystal/CMakeCrystalWin.hpp"
#include "monolib/device/CDeviceVI.hpp"
#include <revolution/gx/GXPixel.h>

/*
 * Constructor (retail unmangled `__ct__CMakeCrystalWin`). Written as a
 * C-ABI global like __ct__CSystemWindow because the retail symbol carries no
 * class-length mangling. Sets the primary + IScnRender sub-vtables, stores
 * the two ctor args, zeroes the scene/thread slots, then constructs the four
 * subobjects (CTitleAHelp/CModelDispMakeCrystal/CMCCrystalBox/CLoad) and
 * clears the two state bytes.
 */
extern "C" CMakeCrystalWin* __ct__CMakeCrystalWin(CMakeCrystalWin* _this, u32 arg4, u32 arg5) {
    _this->field_08 = arg4;
    _this->field_0C = arg5;
    _this->mVtbl = (u32)lbl_eu_80535B70;
    _this->mScnRenderVtbl = (u32)lbl_eu_80535B70 + 0x88;
    _this->mScene = 0;
    _this->mThread = 0;
    __ct__CTitleAHelp(&_this->mTitleAHelp, 0, 0);
    __ct__CModelDispMakeCrystal(reinterpret_cast<CModelDispMakeCrystal*>(&_this->mModelDispArea[0]), 0);
    __ct__CMCCrystalBox(&_this->mCrystalBox, 0);
    __ct__CLoad(&_this->mLoad, 0);
    _this->field_4360 = 0;
    _this->field_4361 = 0;
    return _this;
}

/*
 * Destructor. Written as a free function with an explicit flags parameter
 * (same scheme as CMenuSave's dtor): the embedded CModelDispMakeCrystal
 * class header declares no fields yet, so a real member destructor cannot
 * emit the retail __dt__21CModelDispMakeCrystalFv call at the right offset.
 * Subobjects are destroyed in reverse construction order with the -1
 * subobject flag, then conditional operator delete when flags > 0.
 */
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

void func_8021260C(){}

// Retail 0x802147F4: tear down the crystal window. Removes the render
// callback from the owning scene, releases all four subobjects, flags the
// scene bytes, detaches the child work thread's view and clears it.
void func_8021299C(CMakeCrystalWin* self) {
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

    func_801C40A0(&self->mTitleAHelp);
    func_8021C8B0(reinterpret_cast<CModelDispMakeCrystal*>(&self->mModelDispArea[0]));
    func_802144F4(&self->mCrystalBox);
    func_802AE62C(&self->mLoad);

    CScn* scene2 = self->mScene;
    if (scene2 != 0) {
        func_800453EC(scene2);
        // Retail re-reads mScene for every byte store (no CSE across the
        // pointer writes).
        reinterpret_cast<CScnFlagView*>(self->mScene)->field_3E4 = 1;
        reinterpret_cast<CScnFlagView*>(self->mScene)->field_39 = 1;
        self->mScene = 0;
    }

    CWorkThread* thread = self->mThread;
    if (thread != 0) {
        CTaskGame::getInstance()->unk6C->pssDetachView(thread->mWorkID);
        self->mThread->wkSetEvent(CWorkThread::EVT_NONE);
        self->mThread = 0;
    }
}


void func_80212A68(){}

u8 func_80212B68(void* self) { return ((CMakeCrystalWin*)self)->field_4361; }

// Retail 0x802149C8: returns 0 only when the game is not busy and the crystal
// box is inactive and the window state is a non-zero value below 7.
int func_80212B70(CMakeCrystalWin* self) {
    if (func_8029A658() != 0) {
        return 1;
    }
    if (func_802146F8(&self->mCrystalBox) != 0) {
        return 1;
    }
    u8 state = self->field_4360;
    int result = 1;
    if (state < 7 && state != 0) {
        result = 0;
    }
    return result;
}

u32 func_80212BE0(void* self) { return *(u32*)((u8*)self + 0x10); }

// Retail 0x80212BE8: when the banner, crystal box and loading overlay are all
// ready, dismiss them, mark the window state as 1 and play the crystal sound.
void func_80212BE8(CMakeCrystalWin* self) {
    if (func_801C4114(&self->mTitleAHelp) != 0) {
        if (func_80214634(&self->mCrystalBox) != 0) {
            if (func_802AE6AC(&self->mLoad) != 0) {
                func_801C412C(&self->mTitleAHelp);
                func_80214700(&self->mCrystalBox);
                self->field_4360 = 1;
                func_80138078__FUl(0x6d);
            }
        }
    }
}

// Retail 0x80214AB8: once the title-help banner is idle and the crystal box
// is active, mark the window state as 2.
void func_80212C60(CMakeCrystalWin* self) {
    if (isIdle__11CTitleAHelpFv(&self->mTitleAHelp) != 0) {
        if (func_802146C0(&self->mCrystalBox) != 0) {
            self->field_4360 = 2;
        }
    }
}

void func_80212CB0(){}

// Retail 0x80214D2C: same gate as func_80212C60 but flags the secondary
// state byte (field_4361) instead.
void func_80212ED4(CMakeCrystalWin* self) {
    if (isIdle__11CTitleAHelpFv(&self->mTitleAHelp) != 0) {
        if (func_802146C0(&self->mCrystalBox) != 0) {
            self->field_4361 = 1;
        }
    }
}

// Retail 0x80214D7C: when the crystal box is active, switch the title-help
// banner to mode 0x37 and set the window state to 5.
void func_80212F24(CMakeCrystalWin* self) {
    if (func_802146C0(&self->mCrystalBox) != 0) {
        func_801C41E8(&self->mTitleAHelp, 0x37);
        self->field_4360 = 5;
    }
}

void func_80212F70(){}

// Retail 0x80214F58: same gate as func_80212F24 but with banner mode 0x34
// and window state 2.
void func_80213100(CMakeCrystalWin* self) {
    if (func_802146C0(&self->mCrystalBox) != 0) {
        func_801C41E8(&self->mTitleAHelp, 0x34);
        self->field_4360 = 2;
    }
}

// Retail 0x80214FA4: once the title-help banner is idle and the crystal box
// is active, sync the crystal cursor, update the display selection, and show
// the load overlay when either selection index reaches 3.
void func_8021314C(CMakeCrystalWin* self) {
    if (isIdle__11CTitleAHelpFv(&self->mTitleAHelp) != 0) {
        if (func_802146C0(&self->mCrystalBox) != 0) {
            func_802144F4(&self->mCrystalBox);
            u32 cursor = func_8021625C(&self->mCrystalBox);
            setCrystalCursor(reinterpret_cast<CModelDispMakeCrystal*>(&self->mModelDispArea[0]), cursor);
            u8 a = func_8021624C(&self->mCrystalBox);
            u8 b = func_80216254(&self->mCrystalBox);
            func_80221B90(reinterpret_cast<CModelDispMakeCrystal*>(&self->mModelDispArea[0]), a, b);
            if ((u32)a >= 3 || (u32)b >= 3) {
                func_802AE6C4(&self->mLoad);
            }
            self->field_4360 = 8;
        }
    }
}


// Retail 0x80215058: when the model display is ready, show it, advance the
// loading overlay animation, and set the window state to 9.
void func_80213200(CMakeCrystalWin* self) {
    if (func_8021CA3C(reinterpret_cast<CModelDispMakeCrystal*>(&self->mModelDispArea)) != 0) {
        func_8021CB20(reinterpret_cast<CModelDispMakeCrystal*>(&self->mModelDispArea));
        func_802AE758(&self->mLoad);
        self->field_4360 = 9;
    }
}

// Retail 0x802150A8: once the title-help banner is idle and the model
// display is ready, mark the window state as 0xa.
void func_80213250(CMakeCrystalWin* self) {
    if (isIdle__11CTitleAHelpFv(&self->mTitleAHelp) != 0) {
        if (getCrystalStateA(reinterpret_cast<CModelDispMakeCrystal*>(&self->mModelDispArea)) != 0) {
            self->field_4360 = 0xa;
        }
    }
}

// Retail 0x802150F8: when the model display is charging (state B), flag the
// window as 0xb and start the banner; always forward the display mode to the
// banner afterwards.
void func_802132A0(CMakeCrystalWin* self) {
    if (getCrystalStateB(reinterpret_cast<CModelDispMakeCrystal*>(&self->mModelDispArea)) != 0) {
        self->field_4360 = 0xb;
        func_801C414C(&self->mTitleAHelp);
    }
    int mode = func_80221CD0(reinterpret_cast<CModelDispMakeCrystal*>(&self->mModelDispArea));
    func_801C41E8(&self->mTitleAHelp, (u8)mode);
}

// Retail 0x80215158: when the title-help banner is idle and the crystal
// model display is charging (state C), rebuild the crystal box and model
// display from stack temps and start the banner; otherwise flag the window
// as not ready.
void func_80213300(CMakeCrystalWin* self) {
    if (isIdle__11CTitleAHelpFv(&self->mTitleAHelp) != 0) {
        if (getCrystalStateC(reinterpret_cast<CModelDispMakeCrystal*>(&self->mModelDispArea[0])) != 0) {
            self->field_4360 = 0;
            func_8021C8B0(reinterpret_cast<CModelDispMakeCrystal*>(&self->mModelDispArea[0]));

            // Rebuild the crystal box: construct a temp, copy it in, destroy.
            // MWCC assigns stack slots in reverse declaration order, so the
            // CModelDispMakeCrystal temp (retail +0x8) is declared last.
            u8 boxTemp[0x1508];    // CMCCrystalBox temp at +0x2DE0
            u8 dispTemp[0x2DD8];   // CModelDispMakeCrystal temp at +0x8
            __ct__CMCCrystalBox(reinterpret_cast<CMCCrystalBox*>(boxTemp), 0);
            func_80211CEC(&self->mCrystalBox, reinterpret_cast<const CMCCrystalBox*>(boxTemp));
            __dt__13CMCCrystalBoxFv(reinterpret_cast<CMCCrystalBox*>(boxTemp), -1);
            func_80213FE4(&self->mCrystalBox);

            // Rebuild the model display the same way (temp constructed with
            // the owning scene, copied in, destroyed).
            __ct__CModelDispMakeCrystal(reinterpret_cast<CModelDispMakeCrystal*>(dispTemp), self->mScene);
            func_80210E9C(&self->mModelDispArea[0], &dispTemp[0]);
            __dt__21CModelDispMakeCrystalFv(reinterpret_cast<CModelDispMakeCrystal*>(dispTemp), -1);
            func_8021C6E4(reinterpret_cast<CModelDispMakeCrystal*>(&self->mModelDispArea[0]));
            func_801C41E8(&self->mTitleAHelp, 0x30);
        } else {
            self->field_4361 = 1;
        }
    }
}


// Render callback: draw the crystal window's four subobjects once the task
// gate is clear. Same guard + raw-storage DrawInfo scheme as
// CSystemWindow::cbRenderBefore.
void CMakeCrystalWin::cbRenderBefore() {
    CTaskGame::getInstance();
    if (CTaskGame::func_800426F0() || (lbl_eu_80663E28 & 0x200000))
        return;
    if (func_8013BE50() == 0)
        return;
    GXSetZMode(GX_FALSE, GX_NEVER, GX_FALSE);
    // Raw-storage DrawInfo built/destroyed via C-ABI ct/dt calls to match the
    // retail direct calls (a C++ local would virtual-dispatch its dtor).
    u8 drawInfo[0x54];
    __ct__Q34nw4r3lyt8DrawInfoFv(&drawInfo[0]);
    func_80137250((nw4r::lyt::DrawInfo*)&drawInfo[0]);
    func_8021C928(reinterpret_cast<CModelDispMakeCrystal*>(&mModelDispArea[0]), (nw4r::lyt::DrawInfo*)&drawInfo[0]);
    func_80214408(&mCrystalBox, (nw4r::lyt::DrawInfo*)&drawInfo[0]);
    func_802AE5F0(&mLoad, (nw4r::lyt::DrawInfo*)&drawInfo[0]);
    func_801C4080(&mTitleAHelp, (nw4r::lyt::DrawInfo*)&drawInfo[0]);
    __dt__Q34nw4r3lyt8DrawInfoFv(&drawInfo[0], -1);
}

void cbRenderBefore__15CMakeCrystalWinFv(void*);

void func_80213478(void* self) { ((void(*)(void*))cbRenderBefore__15CMakeCrystalWinFv)((char*)self - 0x4); }

void func_80213480(void* self) { ((void(*)(void*))__dt__15CMakeCrystalWinFv)((char*)self - 0x4); }
