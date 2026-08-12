// Auto-scaffolded catalog TU for kyoshin/CMainMenu
// Replace stubs with high-level C/C++ during decomp.

#include <types.h>
#include "kyoshin/CMainMenu.hpp"
#include "kyoshin/IUICf.hpp"
#include "kyoshin/CArtsInfo.hpp"
#include "kyoshin/CBaseCur.hpp"
#include "kyoshin/code_80135FDC.hpp"
#include "kyoshin/menu/CMenuPassiveSkill.hpp"
#include "monolib/device/CDeviceFile.hpp"
#include "monolib/util/MemManager.hpp"
#include "monolib/work/CTTask.hpp"
#include "monolib/work/IWorkEvent.hpp"
#include "monolib/work/CWorkThreadSystem.hpp"
#include <nw4r/lyt/lyt_layout.h>
#include <nw4r/lyt/lyt_pane.h>
#include <nw4r/math/math_types.h>

// CBaseCur shared helpers (defined in CCur.cpp)
extern void func_801D21CC(CBaseCur* cur);

// Destructor for CBaseCur-derived class at vtable 0x800FEA30.
// Standard MWCC virtual dtor: null-check, call base dtor with flag 0,
// conditionally operator delete, return this.
void* __dt__800FEA30(void* _this, int flags) {
    if (_this) {
        __dt__8CBaseCurFv(_this, 0);
        if (flags > 0) {
            operator delete(_this);
        }
    }
    return _this;
}

u32 func_800FEDF8(void) {
    extern u32 lbl_eu_80663F18;
    return lbl_eu_80663F18;
}

// Destructor for CProcess-derived class at vtable 0x800FED0C.
// The nested null-check reproduces MWCC's D2-inlined-into-D1 pattern
// (D2 has its own null guard, preserved when inlined).
void* __dt__800FED0C(void* _this, int flags) {
    if (_this) {
        if (_this) {
            __dt__8CProcessFv(_this, 0);
        }
        if (flags > 0) {
            operator delete(_this);
        }
    }
    return _this;
}

// CMainMenu::~CMainMenu() - virtual destructor (D1/D2 merged).
// Destroys subobjects in reverse order: CSubCur at +0xA8, CBaseCur at +0x90,
// UnkClass_8045F564 at +0x60, CProcess at 0x00, then conditionally frees memory.
// The addic. for CBaseCur and the nested CProcess null-checks are MWCC
// D2-inlined-into-D1 artifacts.
extern "C" void* __dt__9CMainMenuFv(CMainMenu* _this, int flags) {
    if (_this) {
        CBaseCur* subCur = (CBaseCur*)((char*)_this + 0xA8);
        __dt__7CSubCurFv(subCur, -1);
        CBaseCur* baseCur = (CBaseCur*)((char*)_this + 0x90);
        if (baseCur) {
            __dt__8CBaseCurFv(baseCur, 0);
        }
        __dt__17UnkClass_8045F564Fv((char*)_this + 0x60, -1);
        if (_this) {
            if (_this) {
                __dt__8CProcessFv(_this, 0);
            }
        }
        if (flags > 0) {
            operator delete(_this);
        }
    }
    return _this;
}


extern u32 lbl_eu_8052BDF4[];

// Copies 3 words from static data lbl_eu_8052BDF4 to fields at 0x3C/0x40/0x44,
// guarded by field_0x74 to ensure one-shot initialization.
extern "C" void func_800FEF20(CMainMenu* self) {
    if (self->field_0x74 != 0) {
        return;
    }
    u32 v0;
    u32 *p = (u32 *)lbl_eu_8052BDF4;
    v0 = p[0];
    self->field_0x40 = p[1];
    self->field_0x3C = v0;
    self->field_0x44 = p[2];
}

void func_800FEF4C(CMainMenu* self) {
    // Main-menu frame update: gate on the system window, refresh the menu
    // availability flags, then dispatch on the current state.
    if (func_8013BE50() == 0) {
        return;
    }
    if (lbl_eu_80663E24 & 0xAFE40000) {
        self->field_0x54 = 1;
        return;
    }
    if (func_8027EA64() != 0) return;
    if (getInstance__11CSysWinBuffFv() != 0) return;
    if (func_80192BD0() != 0) return;
    if (func_80167A18() != 0) return;
    if (func_8011CD5C() != 0) return;
    if (func_80212480() != 0) return;
    if (func_8022F530() != 0) return;
    if (func_80242354() != 0) return;
    if (func_80252CD4() != 0) return;
    if (func_80257308() != 0) return;
    if (func_80263944() != 0) return;
    if (func_8027037C() != 0) return;
    if (func_80272488() != 0) return;
    if (func_8028E440() != 0) return;
    if (func_8029BBA0() != 0) return;
    if (func_8029EE58() != 0) return;
    if (func_802AC510() != 0) return;

    // Refresh cursor/sub-menu availability flags.
    func_80101BF8(self);

    switch ((u32)self->field_0xE0) {
    case 0:
        // Enter: open sound + cursor-select state.
        func_80138078__FUl(8);
        self->field_0xE0 = 1;
        break;
    case 1:
        // Cursor-select: once the intro animation finishes, set the current
        // cursor number (or its placeholder) and activate the cursor.
        if (func_80137444__FPQ34nw4r3lyt13AnimTransformf(self->field_0x80, lbl_eu_80666F18) == 0) {
            break;
        }
        {
            int n = self->field_0xC0 + 1;
            if (n > 0) {
                char* s1 =
                    func_80136190(lbl_eu_804FCEBC + 0x5e, lbl_eu_804FCEBC + 0x67, n);
                char* s2 =
                    func_80136190(lbl_eu_804FCEBC + 0x5e, lbl_eu_804FCEBC + 0x6c, n);
                func_80136B4C(self->field_0x7C, lbl_eu_804FCEBC + 0x71, s1, 0);
                func_80136B4C(self->field_0x7C, lbl_eu_804FCEBC + 0x7c, s2, 0);
            } else {
                func_80136B4C(self->field_0x7C, lbl_eu_804FCEBC + 0x71,
                              lbl_eu_804FCEBC + 0x89, 0);
                func_80136B4C(self->field_0x7C, lbl_eu_804FCEBC + 0x7c,
                              lbl_eu_804FCEBC + 0x89, 0);
            }
            func_801D216C(&self->_90[0], 1);
            self->field_0xE0 = 2;
        }
        break;
    case 2:
        // Cursor-active: run the input handler for the selected menu.
        func_800FF920(self);
        break;
    case 3:
        // Exit: once the close animation finishes, mark the menu idle.
        if (func_80137444__FPQ34nw4r3lyt13AnimTransformf(self->field_0x84, lbl_eu_80666F18) == 0) {
            break;
        }
        self->field_0x54 = 1;
        self->field_0xE0 = 0;
        break;
    case 4:
        // Screen-open dispatch: act on the cursor while no other window is up.
        if (func_80101A88() == 0 && func_8029A658() == 0) {
            switch (self->field_0xC0) {
            case 0:
                func_80134460();
                break;
            case 3:
                func_801341D8();
                break;
            }
        }
        self->field_0xE0 = 2;
        break;
    case 5:
        func_80100E14(self);
        break;
    case 6:
        func_801010B8(self);
        break;
    case 7:
        // Cursor-exit: like case 1 but driven by the exit animation.
        if (func_80137444__FPQ34nw4r3lyt13AnimTransformf(self->field_0x8C, lbl_eu_80666F18) == 0) {
            break;
        }
        {
            int n = self->field_0xC0 + 1;
            if (n > 0) {
                char* s1 =
                    func_80136190(lbl_eu_804FCEBC + 0x5e, lbl_eu_804FCEBC + 0x67, n);
                char* s2 =
                    func_80136190(lbl_eu_804FCEBC + 0x5e, lbl_eu_804FCEBC + 0x6c, n);
                func_80136B4C(self->field_0x7C, lbl_eu_804FCEBC + 0x71, s1, 0);
                func_80136B4C(self->field_0x7C, lbl_eu_804FCEBC + 0x7c, s2, 0);
            } else {
                func_80136B4C(self->field_0x7C, lbl_eu_804FCEBC + 0x71,
                              lbl_eu_804FCEBC + 0x89, 0);
                func_80136B4C(self->field_0x7C, lbl_eu_804FCEBC + 0x7c,
                              lbl_eu_804FCEBC + 0x89, 0);
            }
            self->field_0xE0 = 2;
        }
        break;
    case 8:
        func_801018F4(self);
        break;
    }

    // Epilogue: animate the layout and advance both cursors.
    self->field_0x7C->Animate(0);
    func_801D202C(&self->_90[0]);
    func_801D202C(&self->subCur);
}

void __ct__800FF300(void* self){}

// rodata block containing resource names:
// offset 0x00: brlyt name (e.g. "mf00_menu.brlyt")
// offset 0x19: brlan name (e.g. "mf00_menu_in.brlan")
// offset 0x37: brlan name (e.g. "mf00_menu_out.brlan")
// offset 0x53: pane name "Param"

// VUpdate() override for the CBaseCur-derived class embedded at CMainMenu+0x90.
// Loads the layout and two animation transforms, then initializes via func_801D21CC.
extern "C" void func_800FEA88(CBaseCur* self) {
    func_80136E84(&self->mpLayout, self->mArcResAcc, &lbl_eu_804FCEBC[0]);
    func_80136F08(self->mpLayout, &self->mpAnimTrans0, self->mArcResAcc, &lbl_eu_804FCEBC[0x19]);
    func_80136F08(self->mpLayout, &self->mpAnimTrans1, self->mArcResAcc, &lbl_eu_804FCEBC[0x37]);
    self->mpLayout->UnbindAllAnimation();
    func_801D21CC(self);
}

void CMainMenu::cbRenderBefore() {}

// Finds the "Param" pane in the layout's root pane and sets its translate
// to the given position (3 floats: x, y, z).
void CMainMenu::func_800FEB14(float* pos) {
    nw4r::lyt::Pane* pane = field_0x7C->GetRootPane()->FindPaneByName(lbl_eu_804FCEBC + 0x53, true);
    if (pane != NULL) {
        pane->SetSRTElement(0, pos[0]);
        pane->SetSRTElement(1, pos[1]);
        pane->SetSRTElement(2, pos[2]);
    }
}

extern u32 lbl_eu_80663F18;

// Creates the CMainMenu singleton: allocates from work memory via placement new,
// stores in lbl_eu_80663F18, and registers as a child of the given CProcess.
// Returns NULL if already created.
extern "C" void* func_800FF6BC(void* parent, void* param) {
    if (lbl_eu_80663F18) {
        return NULL;
    }
    CMainMenu* menu = new (CWorkThreadSystem::getWorkMem()) CMainMenu(param);
    lbl_eu_80663F18 = (u32)menu;
    Regist__8CProcessFP8CProcessb(menu, parent, false);
    return (void*)lbl_eu_80663F18;
}

extern u32 lbl_eu_80663F18;

// Returns 1 if func_800FF778() is non-zero, else booleanizes lbl_eu_80663F18.
extern "C" u32 func_800FF738(CMainMenu* self) {
    if (func_800FF778__9CMainMenuFv() != 0) {
        return 1;
    }
    return lbl_eu_80663F18 != 0 ? 1 : 0;
}

int CMainMenu::func_800FF778() {
    // Menu-open gate: returns 1 while any menu screen is active (each callee
    // is a singleton/state guard), otherwise the tutorial-list state.
    if (func_80192BD0()) return 1;
    if (func_80167A18()) return 1;
    if (func_8011CD5C()) return 1;
    if (func_80212480()) return 1;
    if (func_8022F530()) return 1;
    if (func_80242354()) return 1;
    if (func_80252CD4()) return 1;
    if (func_80257308()) return 1;
    if (func_80263944()) return 1;
    if (func_8027037C()) return 1;
    if (func_80272488()) return 1;
    if (func_8028E440()) return 1;
    if (func_8029BBA0()) return 1;
    if (func_8029EE58()) return 1;
    return func_802AC510();
}

// Returns 1 if func_800FF778() is non-zero, else checks the global CMainMenu
// singleton's field_0xE0 for states 4 or 8 (active/invite).
extern "C" u32 func_800FF8B0() {
    if (func_800FF778__9CMainMenuFv() != 0) {
        return 1;
    }
    CMainMenu* menu = (CMainMenu*)(uintptr_t)lbl_eu_80663F18;
    if (menu == NULL) {
        return 0;
    }
    if (menu->field_0xE0 == 4) {
        return 1;
    }
    // cntlzw trick: returns 1 if field_0xE0 == 8, else 0
    return (menu->field_0xE0 == 8) ? 1 : 0;
}

// CMainMenu::Init - loads the menu layout file via CDeviceFile::readFile.
// The IWorkEvent at offset 0x58 receives OnFileEvent when the load completes.
extern u32 lbl_eu_80661DC0;

void CMainMenu::Init() {
    // Get IWorkEvent pointer at offset 0x58 (NULL-safe: if this is NULL, workEvent stays NULL)
    void* workEvent = this;
    if (this) {
        workEvent = (char*)this + 0x58;
    }
    mtl::ALLOC_HANDLE handle = mtl::MemManager::getHandleMEM2();
    field_0x74 = CDeviceFile::readFile(handle, (const char*)lbl_eu_80661DC0, (IWorkEvent*)workEvent, 0, 0);
}

extern "C" void func_800FF914(CArtsInfo* self) {
    self->field_0x54 = 1;
}

void func_800FF920(CMainMenu* self){}

void func_80100E14(CMainMenu* self) {
    // Per-frame menu advance: update the sub-cursor pane names/positions from
    // the current (state, sub-state) pair, then park the state at 6 (idle).
    if (func_80137444__FPQ34nw4r3lyt13AnimTransformf(self->field_0x88, lbl_eu_80666F18) == 0) {
        return;
    }
    self->field_0xC4 = 0;
    int idx = 0;
    switch (self->field_0xC0) {
    case 1: idx = 0x8; break;
    case 2: idx = 0xb; break;
    case 4: idx = 0xd; break;
    case 5: idx = 0x11; break;
    case 6: idx = 0x13; break;
    }
    if (idx == 0x13) {
        // All-states cursor: while any presentation/player condition holds the
        // cursor index is forced to 0x16 (no free roam).
        int cond;
        if (getUnk80664658() != 0 && (getUnk80664658()->field_214 & 0x8)) {
            cond = 1;
        } else if (lbl_eu_80663E28 & 0x40000000) {
            cond = 1;
        } else {
            CMainMenuPlayerSpot* spot = getPlayer__Q22cf13CfGameManagerFi(0);
            if (spot == 0) goto math;
            if (getPlayer__Q22cf13CfGameManagerFi(0)->field_0xC4 == 0) goto math;
            u32 flags = getPlayer__Q22cf13CfGameManagerFi(0)->field_0xC4->field_4EC;
            if ((flags & 0x1000000) || (flags & 0x8000) == 0 || (flags & 0x100)) {
                cond = 1;
            } else {
            math:
                // Odd free-roam gate computed from the player-state resource
                // (retail bit-twiddling reproduced verbatim).
                u32 x = func_8009CF8C(0x20);
                cond = ((x | ~0x18E) - ((x - 0x18E) >> 1)) >> 31;
            }
        }
        if (cond != 0) idx = 0x16;
    }
    // Hoisted rodata base: retail keeps it in r29 and offsets from it.
    char* base = lbl_eu_804FCEBC;
    char buf[0x20];
    sprintf(buf, base + 0x121, self->field_0xC0 + 1, self->field_0xC4 + 1);
    nw4r::lyt::Pane* pane1 =
        self->field_0x7C->GetRootPane()->FindPaneByName(buf, true);
    nw4r::lyt::Pane* pane2 =
        self->field_0x7C->GetRootPane()->FindPaneByName(base + 0x133, true);
    nw4r::math::VEC3 vec;
    func_80137924(&vec, pane1, pane2, self->field_0x7C->GetRootPane());
    ((CMainMenuCurVt*)&self->subCur)->vfn_0x10(&vec);
    if (idx > 0) {
        char* s1 = func_80136190(base + 0x5e, base + 0x67, idx);
        char* s2 = func_80136190(base + 0x5e, base + 0x6c, idx);
        func_80136B4C(self->field_0x7C, base + 0x71, s1, 0);
        func_80136B4C(self->field_0x7C, base + 0x7c, s2, 0);
    } else {
        func_80136B4C(self->field_0x7C, base + 0x71, base + 0x89, 0);
        func_80136B4C(self->field_0x7C, base + 0x7c, base + 0x89, 0);
    }
    func_801D216C(&self->_90[0], 0);
    func_801D216C(&self->subCur, 1);
    self->field_0xE0 = 6;
}

void func_801010B8(CMainMenu* self) {
    // Cursor-select sub-menu input: gate on the global input lock, then
    // handle sub-cursor movement / confirmation / cancel.

    if (func_80101A88() != 0) {
        // Input locked: stop the intro animations and move to the exit state.
        self->field_0x7C->SetAnimationEnable(self->field_0x80, false);
        self->field_0x7C->SetAnimationEnable(self->field_0x84, false);
        self->field_0x7C->SetAnimationEnable(self->field_0x88, false);
        self->field_0x7C->SetAnimationEnable(self->field_0x8C, true);
        self->field_0x8C->SetFrame(lbl_eu_80666F1C);
        func_801D216C(&self->_90[0], 1);
        func_801D216C(&self->subCur, 0);
        func_80138078__FUl(6);
        self->field_0xE0 = 7;
        return;
    }
    if (func_8029A658() != 0) {
        return;
    }

    CMainMenuPad* pad = getCurrentPad__Q22cf13CfGameManagerFv();
    u32 down, up, aPressed, bPressed, confirm;
    if (func_80086F9C__Q22cf13CfGameManagerFv(-1) != 0) {
        if (pad->mHeldButtonFlags & 0x08000000) return;
        u32 p = pad->mPressedButtonFlags;
        u32 t = pad->mTurboPressButtonFlags;
        aPressed = (p >> 21) & 1;
        down = t & 1;
        up = (t >> 1) & 1;
        bPressed = (p >> 26) & 1;
        confirm = (((p >> 22) & 1) | ((p >> 10) & 1)) != 0;
    } else {
        if (pad->mHeldButtonFlags & 0x800) return;
        u32 p = pad->mPressedButtonFlags;
        u32 t = pad->mTurboPressButtonFlags;
        aPressed = (p >> 4) & 1;
        down = t & 1;
        up = (t >> 1) & 1;
        bPressed = (p >> 12) & 1;
        confirm = (p & 0x420) != 0;
    }

    if (down != 0) {
        // Down: wrap the sub-cursor backward, then move the cursor pane.
        switch (self->field_0xC0) {
        case 1:
            if (--self->field_0xC4 < 0) self->field_0xC4 = 2;
            break;
        case 2:
            if (--self->field_0xC4 < 0) self->field_0xC4 = 1;
            break;
        case 4:
            if (--self->field_0xC4 < 0) self->field_0xC4 = 3;
            break;
        case 5:
            if (--self->field_0xC4 < 0) self->field_0xC4 = 1;
            break;
        case 6:
            if (--self->field_0xC4 < 0) self->field_0xC4 = 2;
            break;
        }
        char buf[0x20];
        sprintf(buf, lbl_eu_804FCEBC + 0x121, self->field_0xC0 + 1, self->field_0xC4 + 1);
        nw4r::lyt::Pane* pane1 =
            self->field_0x7C->GetRootPane()->FindPaneByName(buf, true);
        nw4r::lyt::Pane* pane2 =
            self->field_0x7C->GetRootPane()->FindPaneByName(lbl_eu_804FCEBC + 0x133, true);
        nw4r::math::VEC3 vec;
        func_80137924(&vec, pane1, pane2, self->field_0x7C->GetRootPane());
        ((CMainMenuCurVt*)&self->subCur)->vfn_0x10(&vec);
        func_80138078__FUl(0x6a);
        goto tail;
    }
    if (up != 0) {
        // Up: wrap the sub-cursor forward, then move the cursor pane.
        switch (self->field_0xC0) {
        case 1:
            if (++self->field_0xC4 > 2) self->field_0xC4 = 0;
            break;
        case 2:
            if (++self->field_0xC4 > 1) self->field_0xC4 = 0;
            break;
        case 4:
            if (++self->field_0xC4 > 3) self->field_0xC4 = 0;
            break;
        case 5:
            if (++self->field_0xC4 > 1) self->field_0xC4 = 0;
            break;
        case 6:
            if (++self->field_0xC4 > 2) self->field_0xC4 = 0;
            break;
        }
        char buf[0x20];
        sprintf(buf, lbl_eu_804FCEBC + 0x121, self->field_0xC0 + 1, self->field_0xC4 + 1);
        nw4r::lyt::Pane* pane1 =
            self->field_0x7C->GetRootPane()->FindPaneByName(buf, true);
        nw4r::lyt::Pane* pane2 =
            self->field_0x7C->GetRootPane()->FindPaneByName(lbl_eu_804FCEBC + 0x133, true);
        nw4r::math::VEC3 vec;
        func_80137924(&vec, pane1, pane2, self->field_0x7C->GetRootPane());
        ((CMainMenuCurVt*)&self->subCur)->vfn_0x10(&vec);
        func_80138078__FUl(0x6a);
        goto tail;
    }
    if (aPressed != 0) {
        // Confirm: reject blocked items with a buzz, otherwise open the
        // selected sub-screen (party state for the special "free roam" item).
        u32 idx;
        switch (self->field_0xC0) {
        case 1: idx = (u8)self->field_0xC4; break;
        case 2: idx = (u8)(self->field_0xC4 + 3); break;
        case 4: idx = (u8)(self->field_0xC4 + 5); break;
        case 5: idx = (u8)(self->field_0xC4 + 9); break;
        case 6: idx = (u8)(self->field_0xC4 + 0xb); break;
        default: idx = 0; break;
        }
        if (self->field_0xC8[(u8)idx + 7] != 0) {
            func_80138078__FUl(5);
            return;
        }
        if (self->field_0xC0 == 6 && self->field_0xC4 == 0) {
            func_8008294C__Q22cf13CfGameManagerFv(true);
            CMainMenuPlayer* player =
                (CMainMenuPlayer*)getPlayer__Q22cf13CfGameManagerFi(0);
            if (player != 0) {
                player = (CMainMenuPlayer*)((char*)player - 0x3E9C);
            }
            if (player != 0 && player->field_0x3F60 != 0) {
                ((CMainMenuSpotVt*)&player->spot)
                    ->vfn_0x9C(player->field_0x3F60->field_0x3B4);
            }
        }
        func_80138078__FUl(0x6b);
        self->field_0xE0 = 8;
        func_801D2174((CBaseCur*)&self->subCur);
        goto tail;
    }
    if (confirm != 0) {
        // Cancel: stop the animations and move to the exit state.
        self->field_0x7C->SetAnimationEnable(self->field_0x80, false);
        self->field_0x7C->SetAnimationEnable(self->field_0x84, false);
        self->field_0x7C->SetAnimationEnable(self->field_0x88, false);
        self->field_0x7C->SetAnimationEnable(self->field_0x8C, true);
        self->field_0x8C->SetFrame(lbl_eu_80666F1C);
        func_801D216C(&self->_90[0], 1);
        func_801D216C(&self->subCur, 0);
        func_80138078__FUl(6);
        self->field_0xE0 = 7;
    } else if (bPressed != 0) {
        func_8013D8A0();
    }

tail:
    // Common tail: pick the sub-cursor index for the selected menu and set
    // its text panes (free-roam gate identical to func_80100E14).
    int idx = 0;
    switch (self->field_0xC0) {
    case 1: idx = self->field_0xC4 + 0x8; break;
    case 2: idx = self->field_0xC4 + 0xb; break;
    case 4: idx = self->field_0xC4 + 0xd; break;
    case 5: idx = self->field_0xC4 + 0x11; break;
    case 6:
        idx = self->field_0xC4 + 0x13;
        if (idx == 0x13) {
            int cond;
            if (getUnk80664658() != 0 && (getUnk80664658()->field_214 & 0x8)) {
                cond = 1;
            } else if (lbl_eu_80663E28 & 0x40000000) {
                cond = 1;
            } else {
                CMainMenuPlayerSpot* spot = getPlayer__Q22cf13CfGameManagerFi(0);
                if (spot == 0) goto math;
                if (getPlayer__Q22cf13CfGameManagerFi(0)->field_0xC4 == 0) goto math;
                u32 flags = getPlayer__Q22cf13CfGameManagerFi(0)->field_0xC4->field_4EC;
                if ((flags & 0x1000000) || (flags & 0x8000) == 0 || (flags & 0x100)) {
                    cond = 1;
                } else {
                math:
                    u32 x = func_8009CF8C(0x20);
                    u32 a = x | ~0x18E;
                    u32 b = (x - 0x18E) >> 1;
                    cond = (a - b) >> 31;
                }
            }
            if (cond != 0) idx = 0x16;
            if (lbl_eu_80663E28 & 0x40000000) idx = 0x17;
        } else if (idx == 0x14 && (lbl_eu_80663E28 & 0x40000000)) {
            idx = 0x18;
        }
        break;
    }
    if (idx > 0) {
        char* s1 = func_80136190(lbl_eu_804FCEBC + 0x5e, lbl_eu_804FCEBC + 0x67, idx);
        char* s2 = func_80136190(lbl_eu_804FCEBC + 0x5e, lbl_eu_804FCEBC + 0x6c, idx);
        func_80136B4C(self->field_0x7C, lbl_eu_804FCEBC + 0x71, s1, 0);
        func_80136B4C(self->field_0x7C, lbl_eu_804FCEBC + 0x7c, s2, 0);
    } else {
        func_80136B4C(self->field_0x7C, lbl_eu_804FCEBC + 0x71, lbl_eu_804FCEBC + 0x89, 0);
        func_80136B4C(self->field_0x7C, lbl_eu_804FCEBC + 0x7c, lbl_eu_804FCEBC + 0x89, 0);
    }
}

void func_801018F4(CMainMenu* self) {
    // Per-frame menu-state dispatch: while no other screen is open, run the
    // handler for the current (state, sub-state) pair, then park the state
    // at 6 (idle) for the next frame.
    if (func_80101A88() == 0 && func_8029A658() == 0) {
        switch (self->field_0xC0) {
        case 1:
            switch (self->field_0xC4) {
            case 0: func_80133D78(); break;
            case 1: func_801342B0(); break;
            case 2: func_80134714(); break;
            }
            break;
        case 2:
            switch (self->field_0xC4) {
            case 0: func_80134388(); break;
            case 1: func_801348C8(); break;
            }
            break;
        case 4:
            switch (self->field_0xC4) {
            case 0: func_80133A08(0); break;
            case 1: func_801347EC(1); break;
            case 2: func_80134A78(); break;
            case 3: func_801349A0(); break;
            }
            break;
        case 5:
            switch (self->field_0xC4) {
            case 0: func_80133CA0(); break;
            case 1: func_80134F2C(0); break;
            }
            break;
        case 6:
            switch (self->field_0xC4) {
            case 0: func_80134B50(0, 1); break;
            case 1: func_80134C34(); break;
            case 2: func_80134E50(1); break;
            }
            break;
        }
    }
    self->field_0xE0 = 6;
}

int func_80101A88() {
    // Gameplay-input gate: returns 1 while input should be blocked (dead
    // player, active battle list, closing menus, or presentation flags).
    CMainMenuPlayerSpot* spot = getPlayer__Q22cf13CfGameManagerFi(0);
    CMainMenuPlayer* player = (CMainMenuPlayer*)spot;
    if (spot != 0) {
        // De-bias the embedded +0x3E9C spot object back to the player base.
        player = (CMainMenuPlayer*)((char*)spot - 0x3E9C);
    }
    if (player != 0) {
        float hp = ((CMainMenuPlayerVt*)player)->vf128();
        if (lbl_eu_80666F1C == hp) return 1;
        CMainMenuPlayerSub* sub = player->field_0x3F60;
        if (sub != 0) {
            int cond = (sub->field_4EC >> 15) & 1;
            if (func_800829B8__Q22cf13CfGameManagerFv() != 0) cond = 1;
            // Retail shares one return-1 tail (L1C) for the flag/action/arts
            // gates; goto anchors that shared block at its retail position.
            if (cond == 0 && (sub->field_4EC & 2) == 0) goto ret1;
            if (sub->field_530 & 1) goto ret1;
            if ((sub->field_4EC & 0x100) == 0) goto battle;
        ret1:
            return 1;
        }
    }
battle:
    // Active battle objects (non-empty manager list). Declaration order
    // mirrors the matched func_8027F0B8 loop so MWCC colors node r3 /
    // sentinel r5 like retail.
    CMainMenuBattleMgr* bm = getInstance__Q22cf14CBattleManagerFv();
    CMainMenuBattleNode* node;
    int count;
    CMainMenuBattleNode* sentinel = bm->list;
    count = 0;
    node = sentinel->next;
    while (node != sentinel) {
        node = node->next;
        count++;
    }
    if (count != 0) return 1;
    if (func_80122450()) return 1;
    if (func_80124B78()) return 1;
    if (getUnk80664658()->field_214 & 0x80000) return 1;
    // Presentation-flag / close gates share one return-1 tail (L1BC); the
    // volatile reads defeat MWCC's load CSE and keep both loads hoisted
    // before the masks (retail lwz r0 + lwz r3).
    u32 ev0 = *(volatile u32*)&lbl_eu_80663E24;
    u32 ev1 = *(volatile u32*)&lbl_eu_80663E24;
    if ((ev0 & 0x4) != 0) goto ret2;
    if ((ev1 & 0x400000) != 0) goto ret2;
    if (func_80084BF4__Q22cf13CfGameManagerFv() == 0) goto end;
ret2:
    return 1;
end:
    return func_80135898();
}

void func_80101BF8(CMainMenu* self) {
    // Menu availability refresh: recompute cursor flags, sub-menu flags and
    // pane positions/colors from the current game state.
    self->field_0xC8[0] = 0;
    self->field_0xC8[1] = 0;
    self->field_0xC8[2] = 0;
    self->field_0xC8[3] = 0;
    self->field_0xC8[4] = 0;
    self->field_0xC8[5] = 0;
    self->field_0xC8[6] = 0;
    self->field_0xC8[7] = 0;

    // hasSave: a save-slot count other than 0xFE always enables the save
    // entry; the 0xFE case consults the shared save flag byte. The boolean is
    // materialized arithmetically (subfic/subi/or/srwi) and both branches
    // feed one store at the merge.
    u32 saveOk = func_8009CF8C(0x270) != 0xFE;
    u32 d0;
    if (saveOk != 0) {
        d0 = 1;
    } else {
        func_80139198(1);
        d0 = (u8)code80135FDC_getByte_64077() < 2;
    }
    self->field_0xD0 = d0;

    self->field_0xD1 = func_8009CF8C(0x337c) == 0 ? 1 : 0;
    self->field_0xD2 = func_8009CF8C(0x3355) == 0 ? 1 : 0;
    self->field_0xD3 = func_8009CF8C(0x3356) == 0 ? 1 : 0;
    self->field_0xD4 = func_8009CF8C(0x337f) == 0 ? 1 : 0;
    self->field_0xD5 = func_8009CF8C(0x337a) == 0 ? 1 : 0;
    self->field_0xD6 = func_8009CF8C(0x3380) == 0 ? 1 : 0;
    self->field_0xD7 = 0;
    self->field_0xD8 = 0;
    self->field_0xD9 = 0;

    // Free-roam gate (same expression shape as func_80100E14).
    int cond;
    if (getUnk80664658() != 0 && (getUnk80664658()->field_214 & 0x8)) {
        cond = 1;
    } else if (lbl_eu_80663E28 & 0x40000000) {
        cond = 1;
    } else {
        CMainMenuPlayerSpot* spot = getPlayer__Q22cf13CfGameManagerFi(0);
        if (spot == 0) goto math;
        if (getPlayer__Q22cf13CfGameManagerFi(0)->field_0xC4 == 0) goto math;
        u32 flags = getPlayer__Q22cf13CfGameManagerFi(0)->field_0xC4->field_4EC;
        if ((flags & 0x1000000) || (flags & 0x8000) == 0 || (flags & 0x100)) {
            cond = 1;
        } else {
        math:
            // Odd free-roam gate computed from the player-state resource
            // (retail bit-twiddling reproduced verbatim).
            u32 x = func_8009CF8C(0x20);
            u32 c = 0x18E;
            u32 a = x | ~c;
            u32 b = (x - c) >> 1;
            cond = (a - b) >> 31;
        }
    }
    self->field_0xDA = cond;

    self->field_0xDB = (lbl_eu_80663E28 >> 1) & 1;
    self->field_0xDC = 0;

    // Main cursor items 0..6: pane name is sprintf'ed, position is bright
    // when available ({0x3c,0x32,0x1e}) else dim ({0xff,0xff,0xff}).
    char buf[0x20];
    for (u32 i = 0; i < 7; i++) {
        sprintf(buf, lbl_eu_804FCEBC + 0x206, (u8)i + 1);
        if (self->field_0xC8[(u8)i] != 0) {
            s16 pos[4];
            pos[3] = 0xff;
            pos[2] = 0x1e;
            pos[1] = 0x32;
            pos[0] = 0x3c;
            func_801398A4(self->field_0x7C, buf, pos, 1);
        } else {
            s16 pos[4];
            pos[3] = 0xff;
            pos[2] = 0xff;
            pos[1] = 0xff;
            pos[0] = 0xff;
            func_801398A4(self->field_0x7C, buf, pos, 1);
        }
    }

    // Sub-menu items 0..13: fixed pane names copied into a stack table;
    // availability flag bytes live at 0xCF+i (field_0xC8[7+i]).
    const char* names[14];
    names[0] = (const char*)lbl_eu_804FCE50[0];
    names[1] = (const char*)lbl_eu_804FCE50[1];
    names[2] = (const char*)lbl_eu_804FCE50[2];
    names[3] = (const char*)lbl_eu_804FCE50[3];
    names[4] = (const char*)lbl_eu_804FCE50[4];
    names[5] = (const char*)lbl_eu_804FCE50[5];
    names[6] = (const char*)lbl_eu_804FCE50[6];
    names[7] = (const char*)lbl_eu_804FCE50[7];
    names[8] = (const char*)lbl_eu_804FCE50[8];
    names[9] = (const char*)lbl_eu_804FCE50[9];
    names[10] = (const char*)lbl_eu_804FCE50[10];
    names[11] = (const char*)lbl_eu_804FCE50[11];
    names[12] = (const char*)lbl_eu_804FCE50[12];
    names[13] = (const char*)lbl_eu_804FCE50[13];
    for (u32 i = 0; i < 14; i++) {
        if (self->field_0xC8[(u8)i + 7] != 0) {
            s16 pos[4];
            pos[3] = 0xff;
            pos[2] = 0x1e;
            pos[1] = 0x32;
            pos[0] = 0x3c;
            func_801398A4(self->field_0x7C, names[(u8)i], pos, 1);
        } else {
            s16 pos[4];
            pos[3] = 0xff;
            pos[2] = 0xff;
            pos[1] = 0xff;
            pos[0] = 0xff;
            func_801398A4(self->field_0x7C, names[(u8)i], pos, 1);
        }
    }
}

void OnFileEvent__9CMainMenuFP10CEventFile(void* self) { ((void(*)(void*))__ct__800FF300)((char*)self - 0x58); }

void func_80102008(void* self) { ((void(*)(void*))__dt__9CMainMenuFv)((char*)self - 0x58); }

void func_80102010(void* self) { ((void(*)(void*))cbRenderBefore__9CMainMenuFv)((char*)self - 0x5c); }

extern "C" void func_80102018(void* self) { ((void(*)(void*))__dt__9CMainMenuFv)((char*)self - 0x5c); }


// CTTask<IUICf>::Move - test PTMF at +0x3C, call if non-null
// (static_cast mirrors the canonical CTTask.hpp inline body).
template<>
void CTTask<IUICf>::Move() {
    if (mMoveFunc) {
        (static_cast<IUICf*>(this)->*mMoveFunc)();
    }
}

// CTTask<IUICf>::Draw - test PTMF at +0x48, call if non-null
template<>
void CTTask<IUICf>::Draw() {
    if (mDrawFunc) {
        (static_cast<IUICf*>(this)->*mDrawFunc)();
    }
}

// Standard MWCC virtual dtor: empty body, compiler emits base dtor call + D1 wrapper
template<>
CTTask<IUICf>::~CTTask() {
}


extern "C" void Term__9CMainMenuFv() {}
