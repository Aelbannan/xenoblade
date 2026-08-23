// World map screen UI process (kyoshin/menu/CMenuMapSelect).

#include "kyoshin/menu/CMenuMapSelect.hpp"

#include <monolib/work/CWorkThreadSystem.hpp>
#include <monolib/util/MemManager.hpp>
#include <monolib/core/CPadManager.hpp>
#include "kyoshin/cf/CfGameManager.hpp"

// Null pointer-to-member-function constant (12 bytes, lib).
struct PtmfNull {
    u32 fn;
    u32 data;
    u32 thisAdj;
};

// View of cf::CfPadData (incomplete in CfGameManager.hpp): embedded CPad at
// +0x00 and the turbo-press flag copy at +0x104.
struct CfPadDataView {
    u8 _00[0x4];
    u32 mPadPressedFlags;       // 0x04 - CPad::mPressedButtonFlags copy
    u8 _08[0x104 - 0x8];
    u32 mTurboPressButtonFlags; // 0x104
};

extern "C" {
void __ct__8CProcessFv(CProcess* self);void __ct__CBgTex(CBgTex* self, u8 arg);
void __ct__CTitleAHelp(CTitleAHelp* self, char* arg1, u8 arg2);
void __ct__CMapSel(CMapSel* self);
void __ct__CFade(CFade* self);
void __ct__CFloorMap(CFloorMap* self);

extern PtmfNull __ptmf_null;

extern u8 lbl_eu_8052BF70[];   // temporary vtable (pre-PMF copy)
extern u8 lbl_eu_80536D28[];   // final vtable; IScnRender vtable at +0x24
extern u32 lbl_eu_80664790;    // CMenuMapSelect singleton pointer (sbss)
extern const f32 lbl_eu_80668728; // ctor timer init value
extern const f32 lbl_eu_8066872C; // timer increment per frame
extern const f32 lbl_eu_80668730; // timer cap

// Callee helpers (retail-unmangled names).
// Callee helpers (retail-unmangled names).
extern "C" int func_80086F9C__Q22cf13CfGameManagerFv(s16 arg);
int func_800FEDF8();
void func_800FF914();
int func_80244510(CFade* self);
void func_80244518(CFade* self);
void func_8024371C(CMapSel* self);
void func_80243768(CMapSel* self);
void func_80243838(CMapSel* self);
void func_8024391C(CMapSel* self);
void func_802439CC(CMapSel* self);
u8 func_80243A9C(CMapSel* self);
void func_8024F1FC(CFloorMap* self, u8 mapIdx);
void func_8024C104(CFloorMap* self);

int func_8024F538(CFloorMap* self);
int func_8024F6E0(CFloorMap* self);
u8 func_8024F630(CFloorMap* self);
u8 func_8024F6D8(CFloorMap* self);
u8 func_8024F6BC(CFloorMap* self);
int func_8024F784(CFloorMap* self);
void func_8024F5C4(CFloorMap* self, u32 arg);
u16 func_8024F54C(CFloorMap* self);
u8 func_8024F554(CFloorMap* self);
void func_8024F55C(CFloorMap* self);
void func_8024CE68(CFloorMap* self);
void func_8024D23C(CFloorMap* self);
void func_8024EE50(CFloorMap* self);
void func_8024D614(CFloorMap* self);
void func_8024DA0C(CFloorMap* self);
void func_8024DE08(CFloorMap* self);
void func_8024E2BC(CFloorMap* self);
void func_8024F658(CFloorMap* self);
void func_8024F72C(CFloorMap* self);
void func_8024E650(CFloorMap* self);
void func_8024E828(CFloorMap* self);
void func_8024EA00(CFloorMap* self);
void func_8024EC24(CFloorMap* self);
void func_8024F7CC(CFloorMap* self);

// CTitleAHelp help-bar id setter (retail-unmangled).
void func_801C41E8(CTitleAHelp* self, u8 arg);
}

// CTitleAHelp help-bar text reset (retail-unmangled name).
extern "C" void func_801C414C(CTitleAHelp* self);

// C++-linkage helper (retail emits the mangled form func_80138078__FUl).
void func_80138078(unsigned long op);

// ---------------------------------------------------------------------------
// Byte-range shim over the CProcess header + PTMF zone, so the factory
// constructor can write the vtable (+0x10) and callback slots (+0x3C..0x53)
// without raw pointer arithmetic. Layout matches CProcess (0x00-0x3B).
// ---------------------------------------------------------------------------
struct CMenuMapSelectCtorShim {
    u8 _00[0x10];
    void* vtable;              // 0x10 -- overwritten by this ctor
    u8 _14[0x28];              // 0x14-0x3B -- rest of CProcess
    u32 callbacks[6];          // 0x3C-0x53 -- null PMF slot words
    u8 field54;                // 0x54
    u8 field55;                // 0x55
};

// View of the IScnRender subobject vptr slot (+0x58).
struct CMenuMapSelectRenderShim {
    u8 _00[0x58];
    void* iscnVtbl;            // 0x58 -- IScnRender vtable
};

// ---------------------------------------------------------------------------
// Target: us-802443d4 -- CMenuMapCreate: singleton factory constructor.
// Allocates the process from the work heap, constructs the base and members,
// registers under the parent and returns the singleton (null if it exists).
// ---------------------------------------------------------------------------
extern "C" CMenuMapSelect* __ct__CMenuMapSelect(CProcess* parent, CProcess* parentRef) {
    if (lbl_eu_80664790 != 0) {
        return NULL;
    }

    mtl::ALLOC_HANDLE handle = CWorkThreadSystem::getWorkMem();
    CMenuMapSelectCtorShim* shim =
        (CMenuMapSelectCtorShim*)mtl::MemManager::allocate(0x34E4, handle);

    if (shim != NULL) {
        CProcess* proc = (CProcess*)shim;
        __ct__8CProcessFv(proc);
        shim->vtable = lbl_eu_8052BF70;

        // Word-exact copy of the two null-PMF slot groups (0x3C..0x53).
        u32* ptmf = &__ptmf_null.fn;
        u32 w1 = ptmf[1];
        u32 w0 = ptmf[0];
        shim->callbacks[0] = w0;
        shim->callbacks[1] = w1;
        shim->callbacks[2] = ptmf[2];

        w1 = ptmf[1];
        w0 = ptmf[0];
        shim->callbacks[3] = w0;
        shim->callbacks[4] = w1;
        shim->callbacks[5] = ptmf[2];

        shim->field54 = 0;
        shim->field55 = 0;

        CMenuMapSelect* self = (CMenuMapSelect*)shim;
        shim->vtable = lbl_eu_80536D28;
        ((CMenuMapSelectRenderShim*)shim)->iscnVtbl = lbl_eu_80536D28 + 0x24;
        self->mParent = parentRef;

        CBgTex* bgTex = &self->mBgTex;
        __ct__CBgTex(bgTex, 0);
        CTitleAHelp* title = &self->mTitleAHelp;
        __ct__CTitleAHelp(title, 0, 0);
        CMapSel* mapSel = &self->mMapSel;
        __ct__CMapSel(mapSel);
        CFade* fade = &self->mFade;
        __ct__CFade(fade);
        CFloorMap* floorMap = &self->mFloorMap;
        __ct__CFloorMap(floorMap);

        self->mState = 0;
        self->mTimer = lbl_eu_80668728;
    }

    lbl_eu_80664790 = (u32)shim;
    ((CProcess*)shim)->Regist(parent, false);
    return (CMenuMapSelect*)lbl_eu_80664790;
}

// ---------------------------------------------------------------------------
// Target: func_80242354 -- singleton guard
// ---------------------------------------------------------------------------
extern "C" u32 func_80242354() {
    return lbl_eu_80664790 != 0;
}

// ---------------------------------------------------------------------------
// Target: us-80244520 -- func_80242368: world map input handler (phase gate).
// Reads turbo-press flags plus one controller-dependent trigger bit triple,
// then dispatches cursor movement / cancel / help / confirm actions.
// ---------------------------------------------------------------------------
void CMenuMapSelect::func_80242368() {
    CfPadDataView* padData = (CfPadDataView*)cf::CfGameManager::getCfPadData();

    // Trigger bits and masked-button booleans are extracted identically for
    // both layouts except for the three pressed-flag bit positions:
    // Classic controller uses bits 21-23, Wiimote/Nunchuk bits 4/5/10.
    u32 triggerBit1, triggerBit2, triggerBit3;
    u32 cancel, confirm, stickDir, menu;
    u32 turboFlags, pressedFlags, cancelVal, confirmVal, menuVal, stickVal;
    int classic = func_80086F9C__Q22cf13CfGameManagerFv(-1);
    if (classic != 0) {
        turboFlags = padData->mTurboPressButtonFlags;
        pressedFlags = padData->mPadPressedFlags;

        u32 cancelVal = turboFlags & 0x8004;
        u32 confirmVal = turboFlags & 0x2001;
        u32 menuVal = turboFlags & 0x4002;
        u32 stickVal = (turboFlags & 0x10000) | (turboFlags & 0x8);

        triggerBit1 = (pressedFlags >> 21) & 1;
        // (x | -x) >> 31 normalizes any non-zero mask to 1.
        cancel = (cancelVal | -(s32)cancelVal) >> 31;
        confirm = (confirmVal | -(s32)confirmVal) >> 31;
        menu = (menuVal | -(s32)menuVal) >> 31;
        stickDir = (stickVal | -(s32)stickVal) >> 31;
        triggerBit2 = (pressedFlags >> 22) & 1;
        triggerBit3 = (pressedFlags >> 23) & 1;
    } else {
        turboFlags = padData->mTurboPressButtonFlags;
        pressedFlags = padData->mPadPressedFlags;

        u32 cancelVal = turboFlags & 0x8004;
        u32 confirmVal = turboFlags & 0x2001;
        u32 menuVal = turboFlags & 0x4002;
        u32 stickVal = (turboFlags & 0x10000) | (turboFlags & 0x8);

        triggerBit1 = (pressedFlags >> 4) & 1;
        cancel = (cancelVal | -(s32)cancelVal) >> 31;
        confirm = (confirmVal | -(s32)confirmVal) >> 31;
        menu = (menuVal | -(s32)menuVal) >> 31;
        stickDir = (stickVal | -(s32)stickVal) >> 31;
        triggerBit2 = (pressedFlags >> 5) & 1;
        triggerBit3 = (pressedFlags >> 10) & 1;
    }

    if (triggerBit1 != 0) {
        // Cancel/back: fade out and leave the world map.
        func_80244518(&this->mFade);
        this->mState = 4;
        func_80138078(3);
    } else if (triggerBit2 != 0) {
        // Show help overlay.
        func_801C414C(&this->mTitleAHelp);
        func_8024371C(&this->mMapSel);
        this->mState = 3;
    } else if (cancel != 0) {
        func_80243768(&this->mMapSel);
    } else if (stickDir != 0) {
        func_80243838(&this->mMapSel);
    } else if (confirm != 0) {
        func_8024391C(&this->mMapSel);
    } else if (menu != 0) {
        func_802439CC(&this->mMapSel);
    } else if (triggerBit3 != 0) {
        // Confirm selection.
        if (func_800FEDF8() != 0) {
            func_800FF914();
            func_80138078(6);
        }
        this->mState = 11;
    }
}

// ---------------------------------------------------------------------------
// Target: us-802446dc -- func_80242524: world map phase 2 -> floor map setup.
// Once the fade reports ready, build a fresh CFloorMap in a stack temporary,
// assign it over the member (implicit operator= skips the vptr), select the
// current map and advance to phase 5.
// ---------------------------------------------------------------------------
void func_80242524(CMenuMapSelect* self) {
    if (func_80244510(&self->mFade) != 0) {
        self->mFloorMap = CFloorMap();

        u8 sel = func_80243A9C(&self->mMapSel);
        func_8024F1FC(&self->mFloorMap, sel);
        func_8024C104(&self->mFloorMap);
        self->mState = 5;
    }
}

// ---------------------------------------------------------------------------
// Target: us-80244be0 -- func_80242A28: floor map input handler (phase 5).
// Advances the animation timer, refreshes the help bar text, then dispatches
// zoom / cursor / selection actions based on the active controller layout.
// ---------------------------------------------------------------------------
void func_80242A28(CMenuMapSelect* self) {
    if (func_8024F538(&self->mFloorMap) != 0) {
        return;
    }

    self->mTimer += lbl_eu_8066872C;
    if (self->mTimer > lbl_eu_80668730) {
        self->mTimer = lbl_eu_80668730;
    }

    if (cf::CfGameManager::func_80086F9C(-1) != 0) {
        // Classic controller layout.
        if (func_8024F6E0(&self->mFloorMap) != 0) {
            if (func_8024F630(&self->mFloorMap) > 1 && func_8024F6D8(&self->mFloorMap) != 0) {
                func_801C41E8(&self->mTitleAHelp, 0x4b);
            } else {
                func_801C41E8(&self->mTitleAHelp, 0x4c);
            }
        } else {
            if (func_8024F630(&self->mFloorMap) > 1 && func_8024F6D8(&self->mFloorMap) != 0) {
                func_801C41E8(&self->mTitleAHelp, 0x4d);
            } else {
                func_801C41E8(&self->mTitleAHelp, 0x4e);
            }
        }

        // Zoom is only allowed while the floor map is fully shown and idle.
        u32 zoomReady = 0;
        u32 zooming = 0;
        if (func_8024F630(&self->mFloorMap) > 1 && func_8024F6BC(&self->mFloorMap) != 0) {
            zooming = 1;
        }
        if (zooming != 0 && func_8024F784(&self->mFloorMap) == 0) {
            zoomReady = 1;
        }

        func_8024F5C4(&self->mFloorMap, 0);

        CPad* pad = cf::CfGameManager::getCurrentPad();
        if ((pad->mTurboPressButtonFlags & 0x02000000) != 0 && zoomReady != 0) {
            func_8024CE68(&self->mFloorMap);
            return;
        }
        if ((pad->mTurboPressButtonFlags & 0x04000000) != 0 && zoomReady != 0) {
            func_8024D23C(&self->mFloorMap);
            return;
        }

        u32 pressed = pad->mPressedButtonFlags;
        if ((pressed & 0x00200000) != 0) {
            func_8024EE50(&self->mFloorMap);
            if (func_8024F54C(&self->mFloorMap) != 0) {
                func_80244518(&self->mFade);
                self->mState = 10;
            }
        } else if ((pressed & 0x00400000) != 0) {
            if (func_8024F554(&self->mFloorMap) != 0) {
                func_8024F55C(&self->mFloorMap);
            } else if (func_8024F784(&self->mFloorMap) == 0) {
                func_80244518(&self->mFade);
                self->mState = 8;
                func_80138078(6);
            } else {
                func_8024EE50(&self->mFloorMap);
            }
        } else if ((pad->mTurboPressButtonFlags & 0x4) != 0) {
            func_8024D614(&self->mFloorMap);
        } else if ((pad->mTurboPressButtonFlags & 0x8) != 0) {
            func_8024DA0C(&self->mFloorMap);
        } else if ((pad->mTurboPressButtonFlags & 0x80000000) != 0) {
            func_8024DE08(&self->mFloorMap);
        } else if ((pad->mTurboPressButtonFlags & 0x2) != 0) {
            func_8024E2BC(&self->mFloorMap);
        } else if ((pressed & 0x01000000) != 0) {
            func_8024F658(&self->mFloorMap);
        } else if ((pressed & 0x200) != 0) {
            func_8024F72C(&self->mFloorMap);
        } else if ((pressed & 0x800000) != 0) {
            if (func_8024F784(&self->mFloorMap) == 0) {
                if (func_800FEDF8() != 0) {
                    func_800FF914();
                    func_80138078(6);
                }
                self->mState = 11;
            } else {
                func_8024EE50(&self->mFloorMap);
            }
        }
    } else {
        // Wiimote/Nunchuk layout.
        if (func_8024F6E0(&self->mFloorMap) != 0) {
            if (func_8024F630(&self->mFloorMap) > 1 && func_8024F6D8(&self->mFloorMap) != 0) {
                func_801C41E8(&self->mTitleAHelp, 0x4b);
            } else {
                func_801C41E8(&self->mTitleAHelp, 0x4c);
            }
        } else {
            if (func_8024F630(&self->mFloorMap) > 1 && func_8024F6D8(&self->mFloorMap) != 0) {
                func_801C41E8(&self->mTitleAHelp, 0x4d);
            } else {
                func_801C41E8(&self->mTitleAHelp, 0x4e);
            }
        }

        u32 zooming = 0;
        u32 zoomReady = 0;
        if (func_8024F630(&self->mFloorMap) > 1 && func_8024F6BC(&self->mFloorMap) != 0) {
            zooming = 1;
        }
        if (zooming != 0 && func_8024F784(&self->mFloorMap) == 0) {
            zoomReady = 1;
        }

        func_8024F5C4(&self->mFloorMap, 0);

        CPad* pad = cf::CfGameManager::getCurrentPad();
        u32 pressed = pad->mPressedButtonFlags;
        if ((pressed & 0x10) != 0) {
            func_8024EE50(&self->mFloorMap);
            if (func_8024F54C(&self->mFloorMap) != 0) {
                func_80244518(&self->mFade);
                self->mState = 10;
            }
        } else if ((pressed & 0x20) != 0) {
            if (func_8024F554(&self->mFloorMap) != 0) {
                func_8024F55C(&self->mFloorMap);
            } else if (func_8024F784(&self->mFloorMap) == 0) {
                func_80244518(&self->mFade);
                self->mState = 8;
                func_80138078(6);
            } else {
                func_8024EE50(&self->mFloorMap);
            }
        } else if ((pad->mHeldButtonFlags & 0x1000) != 0 && zooming != 0 &&
                   func_8024F784(&self->mFloorMap) == 0) {
            func_801C41E8(&self->mTitleAHelp, 0x4f);
            if (self->mTimer > lbl_eu_8066872C) {
                func_80138078(2);
            }
            self->mTimer = lbl_eu_80668728;
            func_8024F5C4(&self->mFloorMap, 1);
            if ((pad->mTurboPressButtonFlags & 0x4) != 0) {
                func_8024CE68(&self->mFloorMap);
            } else if ((pad->mTurboPressButtonFlags & 0x8) != 0) {
                func_8024D23C(&self->mFloorMap);
            }
        } else if ((pressed & 0x400) != 0) {
            if (func_8024F784(&self->mFloorMap) == 0) {
                if (func_800FEDF8() != 0) {
                    func_800FF914();
                    func_80138078(6);
                }
                self->mState = 11;
            } else {
                func_8024EE50(&self->mFloorMap);
            }
        } else if ((pad->mTurboPressButtonFlags & 0x4) != 0) {
            func_8024D614(&self->mFloorMap);
        } else if ((pad->mTurboPressButtonFlags & 0x8) != 0) {
            func_8024DA0C(&self->mFloorMap);
        } else if ((pad->mTurboPressButtonFlags & 0x80000000) != 0) {
            func_8024DE08(&self->mFloorMap);
        } else if ((pad->mTurboPressButtonFlags & 0x2) != 0) {
            func_8024E2BC(&self->mFloorMap);
        } else if ((pressed & 0x800) != 0) {
            func_8024F658(&self->mFloorMap);
        } else if ((pressed & 0x200) != 0) {
            func_8024F72C(&self->mFloorMap);
        } else {
            // D-pad held: each direction moves the cursor once per press
            // transition; when no direction is held, confirm the selection.
            u32 flag = 1;
            if ((pad->mHeldButtonFlags & 0x8000) != 0) {
                func_8024E650(&self->mFloorMap);
                flag = 0;
            }
            if ((pad->mHeldButtonFlags & 0x10000) != 0) {
                func_8024E828(&self->mFloorMap);
                flag = 0;
            }
            if ((pad->mHeldButtonFlags & 0x2000) != 0) {
                func_8024EA00(&self->mFloorMap);
                flag = 0;
            }
            if ((pad->mHeldButtonFlags & 0x4000) != 0) {
                func_8024EC24(&self->mFloorMap);
                flag = 0;
            }
            if (flag != 0) {
                func_8024F7CC(&self->mFloorMap);
            }
        }
    }
}

// ---------------------------------------------------------------------------
// IScnRender vtable this-adjusting thunk for cbRenderBefore.
//
// Entered with r3 = IScnRender subobject pointer (CMenuMapSelect base + 0x58).
// Convert back to the CMenuMapSelect base and tail-call the real cbRenderBefore
// (implemented in code_802405F4.cpp).
//
// Retail: subi r3, r3, 0x58; b cbRenderBefore__14CMenuMapSelectFv
// ---------------------------------------------------------------------------
void func_8024309C(CMenuMapSelect* self) {
    CMenuMapSelect* base = (CMenuMapSelect*)((u8*)self - 0x58);
    // Static dispatch (cbRenderBefore is virtual via IScnRender); the retargeted
    // receiver is the CMenuMapSelect base where the real body lives.
    base->CMenuMapSelect::cbRenderBefore();
}

// ---------------------------------------------------------------------------
// IScnRender vtable this-adjusting thunk for destructor.
//
// Entered with r3 = IScnRender subobject pointer (CMenuMapSelect base + 0x58).
// Convert back to the CMenuMapSelect base and tail-call the real destructor.
//
// Retail: subi r3, r3, 0x58; b __dt__14CMenuMapSelectFv
// ---------------------------------------------------------------------------
extern "C" void __dt__14CMenuMapSelectFv(CMenuMapSelect* self);
void CMenuMapSelect::func_802430A4() {
    // Single-arg call leaves the delete-flag register untouched (retail thunk
    // does not set r4), producing a plain this-adjusting tail-call.
    __dt__14CMenuMapSelectFv((CMenuMapSelect*)((u8*)this - 0x58));
}
