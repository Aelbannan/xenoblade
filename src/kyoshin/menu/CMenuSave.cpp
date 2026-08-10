// Decomp of kyoshin/menu/CMenuSave.
// Menu save/load screen process (CProcess + IScnRender MI).

#include "kyoshin/menu/CMenuSave.hpp"

#include "kyoshin/CTaskGame.hpp"
#include "kyoshin/cf/CfGameManager.hpp"
#include "monolib/device/CDeviceVI.hpp"
#include "monolib/scn/CScn.hpp"
#include "monolib/work/CWorkThreadSystem.hpp"
#include "monolib/util/MemManager.hpp"

#include <revolution/GX.h>

// Retail constructor symbol (unmangled global in US). Kept as a free function
// so the factory (func_8028E3B4) emits a real bl to it, and returns `this` in
// r3 like a real constructor (retail relies on it).
extern "C" __declspec(noinline) CMenuSave* __ct__CMenuSave(
    CMenuSave* self, CProcess* parent, u32 arg208, u32 arg209, u32 arg20A) {
    __ct__8CProcessFv((CProcess*)self);

    // vtable fixups: temp (CProcess) vtable first, then the composite vtable
    // and the IScnRender sub-vtable at +0x58.
    *(u32*)((u8*)self + 0x10) = (u32)lbl_eu_8052BF70;
    self->ptmf0[0] = __ptmf_null[0];
    self->ptmf0[1] = __ptmf_null[1];
    self->ptmf0[2] = __ptmf_null[2];
    self->ptmf1[0] = __ptmf_null[0];
    self->ptmf1[1] = __ptmf_null[1];
    self->ptmf1[2] = __ptmf_null[2];
    self->mField54 = 0;
    self->mField55 = 0;

    *(u32*)((u8*)self + 0x10) = (u32)lbl_eu_80538710;
    *(u32*)((u8*)self + 0x58) = (u32)lbl_eu_80538710 + 0x24;
    self->mParentRef = parent;

    __ct__CBgTex(&self->mBgTex, 0);
    __ct__CTitleAHelp(&self->mTitleAHelp, 0, 0);
    __ct__CSaveLoad(reinterpret_cast<CSaveLoad*>(self->mSaveLoad), 0, 0);

    self->mField208 = arg208;
    self->mField209 = arg209;
    self->mField20A = arg20A;
    self->mState = 0;
    if (arg20A != 0) {
        func_8008294C__Q22cf13CfGameManagerFv(1);
    }
    return self;
}

/*
 * Destructor (retail D2 form, symbol keeps its C++ mangling in US).
 *
 * Written with an explicit flags parameter so the base-class destruction can
 * target the retail CProcess dtor symbol __dt__800FED0C (0x800FF7F4, the
 * game-side D2 wrapper). A real member destructor would make MWCC auto-emit
 * __dt__8CProcessFv (the library D1 at 0x804474D0) for the base call, which
 * cannot match the retail reloc name at the call site. Subobjects are
 * destroyed in reverse construction order, then conditional operator delete
 * when flags > 0 (same shape as CSysWinSave's dtor).
 */
extern "C" CMenuSave* __dt__9CMenuSaveFv(CMenuSave* _this, int flags) {
    if (_this != 0) {
        __dt__9CSaveLoadFv(reinterpret_cast<CSaveLoad*>(_this->mSaveLoad), -1);
        __dt__11CTitleAHelpFv(&_this->mTitleAHelp, -1);
        __dt__6CBgTexFv(&_this->mBgTex, -1);
        __dt__800FED0C(_this, 0);
        if (flags > 0) {
            operator delete(_this);
        }
    }
    return _this;
}

/*
 * Init - (re)initialise the save screen. Resets the CfGameManager init flag
 * and the media when requested, then rebuilds each embedded widget (CBgTex /
 * CTitleAHelp / CSaveLoad) by constructing a stack temporary and copying its
 * body into the embedded storage, then registers the render callback on the
 * parent scene (see CMenuKizunaTalkList::Init for the same pattern).
 */
void CMenuSave::Init() {
    if (mField20A != 0) {
        func_8008294C__Q22cf13CfGameManagerFv(1);
    }
    if (mField208 == 0) {
        func_80240360();
    }

    // --- Re-initialise the background CBgTex via a temporary ---
    u8 tempBgTex[0x20];
    __ct__CBgTex(tempBgTex, 0);

    __ct__UnkClass_8011C974((u8*)this + 0x64, tempBgTex + 0x4);
    *(u32*)((u8*)this + 0x74) = *(u32*)(tempBgTex + 0x14);
    *(u32*)((u8*)this + 0x78) = *(u32*)(tempBgTex + 0x18);
    *(u8*)((u8*)this + 0x7c)  = *(u8*)(tempBgTex + 0x1c);
    *(u8*)((u8*)this + 0x7d)  = *(u8*)(tempBgTex + 0x1d);
    *(u8*)((u8*)this + 0x7e)  = *(u8*)(tempBgTex + 0x1e);
    __dt__6CBgTexFv(reinterpret_cast<CBgTex*>(tempBgTex), -1);

    func_801C3C14(&mBgTex);

    // --- Re-initialise the embedded CTitleAHelp ---
    char* name = func_80136190(lbl_eu_8050F7B0, lbl_eu_8050F7B0 + 0xb,
                                mField208 != 0 ? 0x49 : 0x48);

    u8 tempTitle[0x38];
    __ct__CTitleAHelp(tempTitle, name, mField208 != 0 ? 0x6b : 0x69);

    __ct__UnkClass_8011C974((u8*)this + 0x84, tempTitle + 0x4);
    *(u32*)((u8*)this + 0x94) = *(u32*)(tempTitle + 0x14);
    *(u32*)((u8*)this + 0x98) = *(u32*)(tempTitle + 0x18);
    *(u32*)((u8*)this + 0x9c) = *(u32*)(tempTitle + 0x1c);
    *(u32*)((u8*)this + 0xa0) = *(u32*)(tempTitle + 0x20);
    *(u32*)((u8*)this + 0xa4) = *(u32*)(tempTitle + 0x24);
    *(u8*)((u8*)this + 0xa8)  = *(u8*)(tempTitle + 0x28);
    *(u32*)((u8*)this + 0xac) = *(u32*)(tempTitle + 0x2c);
    *(u32*)((u8*)this + 0xb0) = *(u32*)(tempTitle + 0x30);
    *(u8*)((u8*)this + 0xb4)  = *(u8*)(tempTitle + 0x34);
    *(u8*)((u8*)this + 0xb5)  = *(u8*)(tempTitle + 0x35);
    *(u8*)((u8*)this + 0xb6)  = *(u8*)(tempTitle + 0x36);
    *(u8*)((u8*)this + 0xb7)  = *(u8*)(tempTitle + 0x37);
    __dt__11CTitleAHelpFv(reinterpret_cast<CTitleAHelp*>(tempTitle), -1);

    CTitleAHelp_load(&mTitleAHelp);

    // --- Re-initialise the embedded CSaveLoad via a temporary ---
    u8 tempSave[0x150];
    __ct__CSaveLoad(tempSave, mField208, mField209);

    __ct__UnkClass_8011C974((u8*)this + 0xbc, tempSave + 0x4);
    *(u32*)((u8*)this + 0xcc) = *(u32*)(tempSave + 0x14);
    *(u32*)((u8*)this + 0xd0) = *(u32*)(tempSave + 0x18);
    *(u32*)((u8*)this + 0xd4) = *(u32*)(tempSave + 0x1c);
    *(u32*)((u8*)this + 0xd8) = *(u32*)(tempSave + 0x20);
    *(u32*)((u8*)this + 0xdc) = *(u32*)(tempSave + 0x24);
    *(u32*)((u8*)this + 0xe4) = *(u32*)(tempSave + 0x2c);
    *(u32*)((u8*)this + 0xe8) = *(u32*)(tempSave + 0x30);
    *(u32*)((u8*)this + 0xec) = *(u32*)(tempSave + 0x34);
    *(u32*)((u8*)this + 0xf0) = *(u32*)(tempSave + 0x38);
    *(u8*)((u8*)this + 0xf4)  = *(u8*)(tempSave + 0x3c);
    *(u8*)((u8*)this + 0xf5)  = *(u8*)(tempSave + 0x3d);
    *(u32*)((u8*)this + 0xfc) = *(u32*)(tempSave + 0x44);
    *(u32*)((u8*)this + 0x100) = *(u32*)(tempSave + 0x48);
    *(u32*)((u8*)this + 0x104) = *(u32*)(tempSave + 0x4c);
    *(u32*)((u8*)this + 0x108) = *(u32*)(tempSave + 0x50);
    *(u8*)((u8*)this + 0x10c)  = *(u8*)(tempSave + 0x54);
    *(u8*)((u8*)this + 0x10d)  = *(u8*)(tempSave + 0x55);

    __ct__UnkClass_8011C974((u8*)this + 0x114, tempSave + 0x5c);
    *(u32*)((u8*)this + 0x124) = *(u32*)(tempSave + 0x6c);
    *(u32*)((u8*)this + 0x128) = *(u32*)(tempSave + 0x70);
    *(u32*)((u8*)this + 0x12c) = *(u32*)(tempSave + 0x74);
    *(u32*)((u8*)this + 0x130) = *(u32*)(tempSave + 0x78);
    *(u8*)((u8*)this + 0x134)  = *(u8*)(tempSave + 0x7c);
    *(u8*)((u8*)this + 0x135)  = *(u8*)(tempSave + 0x7d);
    *(u8*)((u8*)this + 0x136)  = *(u8*)(tempSave + 0x7e);
    *(u8*)((u8*)this + 0x137)  = *(u8*)(tempSave + 0x7f);
    *(float*)((u8*)this + 0x138) = *(float*)(tempSave + 0x80);
    *(float*)((u8*)this + 0x13c) = *(float*)(tempSave + 0x84);
    *(float*)((u8*)this + 0x140) = *(float*)(tempSave + 0x88);
    *(float*)((u8*)this + 0x144) = *(float*)(tempSave + 0x8c);
    *(float*)((u8*)this + 0x148) = *(float*)(tempSave + 0x90);
    *(u8*)((u8*)this + 0x14c)  = *(u8*)(tempSave + 0x94);

    // Two embedded CSysWin-like regions are copied via their copy helper.
    func_8016742C((u8*)this + 0x150, tempSave + 0x98);
    func_8016742C((u8*)this + 0x18c, tempSave + 0xd4);

    // Tail state region (scrollbar/cursor/slot state).
    *(u32*)((u8*)this + 0x1c8) = *(u32*)(tempSave + 0x110);
    *(u32*)((u8*)this + 0x1cc) = *(u32*)(tempSave + 0x114);
    *(u32*)((u8*)this + 0x1d0) = *(u32*)(tempSave + 0x118);
    *(u8*)((u8*)this + 0x1d4)  = *(u8*)(tempSave + 0x11c);
    *(u8*)((u8*)this + 0x1d5)  = *(u8*)(tempSave + 0x11d);
    *(u8*)((u8*)this + 0x1d6)  = *(u8*)(tempSave + 0x11e);
    *(u8*)((u8*)this + 0x1d8)  = *(u8*)(tempSave + 0x120);
    *(u8*)((u8*)this + 0x1d9)  = *(u8*)(tempSave + 0x121);
    *(u8*)((u8*)this + 0x1da)  = *(u8*)(tempSave + 0x122);
    *(u8*)((u8*)this + 0x1db)  = *(u8*)(tempSave + 0x123);
    *(u8*)((u8*)this + 0x1dc)  = *(u8*)(tempSave + 0x124);
    *(s16*)((u8*)this + 0x1de) = *(s16*)(tempSave + 0x126);
    *(u8*)((u8*)this + 0x1e0)  = *(u8*)(tempSave + 0x128);
    *(u8*)((u8*)this + 0x1e1)  = *(u8*)(tempSave + 0x129);
    *(u8*)((u8*)this + 0x1e2)  = *(u8*)(tempSave + 0x12a);
    *(u8*)((u8*)this + 0x1e3)  = *(u8*)(tempSave + 0x12b);
    *(u8*)((u8*)this + 0x1e4)  = *(u8*)(tempSave + 0x12c);
    *(u8*)((u8*)this + 0x1e5)  = *(u8*)(tempSave + 0x12d);
    *(u8*)((u8*)this + 0x1e6)  = *(u8*)(tempSave + 0x12e);
    *(u8*)((u8*)this + 0x1e7)  = *(u8*)(tempSave + 0x12f);
    *(u32*)((u8*)this + 0x1e8) = *(u32*)(tempSave + 0x130);
    *(u32*)((u8*)this + 0x1ec) = *(u32*)(tempSave + 0x134);
    *(u32*)((u8*)this + 0x1f0) = *(u32*)(tempSave + 0x138);
    *(u32*)((u8*)this + 0x1f4) = *(u32*)(tempSave + 0x13c);
    *(u32*)((u8*)this + 0x1f8) = *(u32*)(tempSave + 0x140);
    *(u32*)((u8*)this + 0x1fc) = *(u32*)(tempSave + 0x144);
    *(u16*)((u8*)this + 0x200) = *(u16*)(tempSave + 0x148);
    *(u8*)((u8*)this + 0x202)  = *(u8*)(tempSave + 0x14a);
    *(u16*)((u8*)this + 0x203) = *(u16*)(tempSave + 0x14b);
    *(u8*)((u8*)this + 0x205)  = *(u8*)(tempSave + 0x14d);
    __dt__9CSaveLoadFv(reinterpret_cast<CSaveLoad*>(tempSave), -1);

    func_8028F23C__9CSaveLoadFv(reinterpret_cast<CSaveLoad*>(mSaveLoad));

    // Register this screen as a render callback on the parent scene (the
    // `if (this)` is the MWCC idiom that splits mr r4,r31 / beq / addi r4,+0x58).
    IScnRender* renderCB = reinterpret_cast<IScnRender*>(this);
    if (this != NULL) {
        renderCB = &mIScnRender;
    }
    reinterpret_cast<CScn*>(mParentRef)->addRenderCB(renderCB, 0xd, 0);
}

void CMenuSave::Term() {
    CDeviceVI::waitForDrawDone();

    // The `if (this)` is the MWCC idiom that splits mr r4,r31 / beq / addi r4,+0x58.
    IScnRender* renderCB = reinterpret_cast<IScnRender*>(this);
    if (this != NULL) {
        renderCB = &mIScnRender;
    }
    reinterpret_cast<CScn*>(mParentRef)->removeRenderCB(renderCB);

    func_801C3D9C(&mBgTex);
    func_801C40A0(&mTitleAHelp);
    func_8028F4AC(reinterpret_cast<CSaveLoad*>(mSaveLoad));

    lbl_eu_806649E8 = 0;

    if (mField208 == 0) {
        func_80240420();
    }
    if (mField20A != 0) {
        func_8008294C__Q22cf13CfGameManagerFv(0);
    }
}

// Per-frame update: gate on the task/busy flags, drive the 4-state FSM
// (phase handlers func_8028E450/E4E0/E530/E768), then refresh the background,
// title/help bar and save-load panel each frame.
void CMenuSave::Move() {
    CTaskGame::getInstance();
    if (CTaskGame::func_800426F0() || (lbl_eu_80663E28 & 0x200000))
        return;

    switch (mState) {
    case 0:
        func_8028E450(this);
        break;
    case 1:
        func_8028E4E0(this);
        break;
    case 2:
        func_8028E530(this);
        break;
    case 3:
        func_8028E768(this);
        break;
    }

    func_801C3D54(&mBgTex);
    func_801C3FF0(&mTitleAHelp);
    func_8028F2CC(reinterpret_cast<CSaveLoad*>(mSaveLoad));
}

// Render the save screen through a stack DrawInfo: gate on the task/busy
// flags and the scene-active check, then draw background, save-load panel and
// title/help bar in that order.
void CMenuSave::cbRenderBefore() {
    CTaskGame::getInstance();
    if (CTaskGame::func_800426F0() || (lbl_eu_80663E28 & 0x200000))
        return;
    if (func_8013BE50() == 0) return;

    GXSetZMode(GX_FALSE, GX_NEVER, GX_FALSE);
    // Raw-storage DrawInfo built/destroyed via C-ABI pre-mangled ct/dt calls
    // (a C++ local would virtual-dispatch its scope-exit destructor).
    u8 drawInfo[0x54];
    __ct__Q34nw4r3lyt8DrawInfoFv(&drawInfo[0]);
    func_80137250((nw4r::lyt::DrawInfo*)&drawInfo[0]);
    func_801C3D7C(&mBgTex, (nw4r::lyt::DrawInfo*)&drawInfo[0]);
    func_8028F3D4(reinterpret_cast<CSaveLoad*>(mSaveLoad),
                  (nw4r::lyt::DrawInfo*)&drawInfo[0]);
    func_801C4080(&mTitleAHelp, (nw4r::lyt::DrawInfo*)&drawInfo[0]);
    __dt__Q34nw4r3lyt8DrawInfoFv(&drawInfo[0], -1);
}

/*
 * Factory: lazily allocate + construct the single save-menu instance and
 * register it as a child of `parent`. Returns the stored instance (or 0 if it
 * already exists). The 4 extra args are forwarded to the CMenuSave ctor.
 */
extern "C" CMenuSave* func_8028E3B4(CProcess* parent, CProcess* arg2, u32 arg3, u32 arg4, u32 arg5) {
    if (lbl_eu_806649E8 != 0) {
        return 0;
    }
    CMenuSave* obj = (CMenuSave*)mtl::MemManager::allocate(
        sizeof(CMenuSave), CWorkThreadSystem::getWorkMem());
    if (obj != 0) {
        obj = __ct__CMenuSave(obj, arg2, arg3, arg4, arg5);
    }
    lbl_eu_806649E8 = (unsigned long)obj;
    obj->Regist(parent, 0x0);
    return (CMenuSave*)lbl_eu_806649E8;
}

void stub_us_802908bc() {}

/* Advance the save menu to phase 2 once the title bar is idle and the
 * save-load panel has finished its intro (writes the state byte at 0x20B). */
extern "C" void func_8028E4E0(CMenuSave* self) {
    if (isIdle__11CTitleAHelpFv(&self->mTitleAHelp) != 0 &&
        func_8028F664(reinterpret_cast<CSaveLoad*>(self->mSaveLoad)) != 0) {
        self->mState = 2;
    }
}

/*
 * func_8028E530 - save/load panel input handling (state 2). Reads the current
 * cf pad data; depending on the co-op/single-player controller layout,
 * different pressed-button bits map to the same actions. The button tests use
 * the (inverted) == 0 form for the turbo-flag masks, matching retail's
 * subic/subfe bool idiom.
 */
extern "C" void func_8028E530(CMenuSave* self) {
    if (func_8028F5C4(reinterpret_cast<CSaveLoad*>(self->mSaveLoad)) == 0) {
        return;
    }

    if (func_8028FEC4(reinterpret_cast<CSaveLoad*>(self->mSaveLoad)) != 0) {
        if (func_800FEDF8() != 0) {
            func_800FF914();
        }
        self->mState = 4;
        self->mField54 = 1;
    }

    MenuSavePadData* pad = (MenuSavePadData*)cf::CfGameManager::getCfPadData();
    if (func_80086F9C__Q22cf13CfGameManagerFv(-1) != 0) {
        // Co-op layout: Classic-controller style bits.
        u32 turbo = pad->mTurboPressButtonFlags;
        u32 pressed = pad->mPadPressedFlags;
        if ((pressed >> 21) & 1) {
            func_8028FC18(reinterpret_cast<CSaveLoad*>(self->mSaveLoad));
        } else if ((pressed >> 22) & 1) {
            if (func_8028F66C(reinterpret_cast<CSaveLoad*>(self->mSaveLoad)) == 0) {
                func_801C414C(&self->mTitleAHelp);
                func_8028F774(reinterpret_cast<CSaveLoad*>(self->mSaveLoad));
                self->mState = 3;
            } else {
                func_8028FE50(reinterpret_cast<CSaveLoad*>(self->mSaveLoad));
            }
        } else if ((pressed >> 28) & 1) {
            func_8028FECC(reinterpret_cast<CSaveLoad*>(self->mSaveLoad));
        } else if ((turbo & 0x8004) == 0) {
            func_8028F7D0(reinterpret_cast<CSaveLoad*>(self->mSaveLoad));
        } else if ((((turbo >> 16) & 1) | (turbo & 8)) == 0) {
            func_8028F904(reinterpret_cast<CSaveLoad*>(self->mSaveLoad));
        } else if ((turbo & 0x2001) == 0) {
            func_8028FA54(reinterpret_cast<CSaveLoad*>(self->mSaveLoad));
        } else if ((turbo & 0x4002) == 0) {
            func_8028FB20(reinterpret_cast<CSaveLoad*>(self->mSaveLoad));
        } else if ((pressed >> 23) & 1) {
            if (self->mField209 == 0 &&
                func_8028F66C(reinterpret_cast<CSaveLoad*>(self->mSaveLoad)) == 0) {
                if (func_800FEDF8() != 0) {
                    func_800FF914();
                }
                func_80138078__FUl(6);
                self->mState = 4;
                self->mField54 = 1;
            }
        }
    } else {
        // Single-player layout: Wiimote-style bits.
        u32 turbo = pad->mTurboPressButtonFlags;
        u32 pressed = pad->mPadPressedFlags;
        if ((pressed >> 4) & 1) {
            func_8028FC18(reinterpret_cast<CSaveLoad*>(self->mSaveLoad));
        } else if ((pressed >> 5) & 1) {
            if (func_8028F66C(reinterpret_cast<CSaveLoad*>(self->mSaveLoad)) == 0) {
                func_801C414C(&self->mTitleAHelp);
                func_8028F774(reinterpret_cast<CSaveLoad*>(self->mSaveLoad));
                self->mState = 3;
            } else {
                func_8028FE50(reinterpret_cast<CSaveLoad*>(self->mSaveLoad));
            }
        } else if ((pressed >> 7) & 1) {
            func_8028FECC(reinterpret_cast<CSaveLoad*>(self->mSaveLoad));
        } else if ((turbo & 0x8004) == 0) {
            func_8028F7D0(reinterpret_cast<CSaveLoad*>(self->mSaveLoad));
        } else if ((((turbo >> 16) & 1) | (turbo & 8)) == 0) {
            func_8028F904(reinterpret_cast<CSaveLoad*>(self->mSaveLoad));
        } else if ((turbo & 0x2001) == 0) {
            func_8028FA54(reinterpret_cast<CSaveLoad*>(self->mSaveLoad));
        } else if ((turbo & 0x4002) == 0) {
            func_8028FB20(reinterpret_cast<CSaveLoad*>(self->mSaveLoad));
        } else if ((pressed >> 10) & 1) {
            if (self->mField209 == 0 &&
                func_8028F66C(reinterpret_cast<CSaveLoad*>(self->mSaveLoad)) == 0) {
                if (func_800FEDF8() != 0) {
                    func_800FF914();
                }
                func_80138078__FUl(6);
                self->mState = 4;
                self->mField54 = 1;
            }
        }
    }

    func_801C41E8(&self->mTitleAHelp, func_8028FFD4(reinterpret_cast<CSaveLoad*>(self->mSaveLoad)));
}

/* Same idle+advance check as func_8028E4E0, but advances the phase flag at
 * 0x54 instead of the state byte. */
extern "C" void func_8028E768(CMenuSave* self) {
    if (isIdle__11CTitleAHelpFv(&self->mTitleAHelp) != 0 &&
        func_8028F664(reinterpret_cast<CSaveLoad*>(self->mSaveLoad)) != 0) {
        self->mField54 = 1;
    }
}

/*
 * IScnRender vtable this-adjusting thunk for cbRenderBefore.
 *
 * The IScnRender subobject sits at +0x58 within CMenuSave; vtable dispatch
 * through that subobject passes 'this' pointing at +0x58, so the thunk
 * subtracts it back and tail-calls the real implementation.
 *
 * Retail: subi r3, r3, 0x58; b cbRenderBefore__9CMenuSaveFv
 */
extern "C" void func_8028E7B8(void* self) {
    cbRenderBefore__9CMenuSaveFv((char*)self - 0x58);
}

/*
 * IScnRender vtable this-adjusting thunk for ~CMenuSave.
 *
 * Same adjustment as func_8028E7B8 but forwards to the destructor, leaving
 * r4 (delete flag) as caller leftover.
 *
 * Retail: subi r3, r3, 0x58; b __dt__9CMenuSaveFv
 */
extern "C" void func_8028E7C0(void* self) {
    ((void(*)(void*))__dt__9CMenuSaveFv)((char*)self - 0x58);
}

extern "C" unsigned long func_8028E440(void) { return lbl_eu_806649E8 != 0; }

/* Once the background, title bar and save-load panel are all ready, pull the
 * save-slot data, update the title bar, and play the open sound. */
extern "C" void func_8028E450(CMenuSave* self) {
    if (func_801C3E34(&self->mBgTex) != 0 &&
        func_801C4114(&self->mTitleAHelp) != 0 &&
        func_8028F5C4(reinterpret_cast<CSaveLoad*>(self->mSaveLoad)) != 0) {
        u8 slot = func_8028FFD4(reinterpret_cast<CSaveLoad*>(self->mSaveLoad));
        func_801C41E8(&self->mTitleAHelp, slot);
        func_801C412C(&self->mTitleAHelp);
        func_8028F6DC(reinterpret_cast<CSaveLoad*>(self->mSaveLoad));
        self->mState = 1;
        func_80138078__FUl(0x6d);
    }
}
