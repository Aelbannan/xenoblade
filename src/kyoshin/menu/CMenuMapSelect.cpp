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
// Use the C++ member -- a local extern "C" Fv(s16) overload conflicts with
// CfGameManager.hpp's bool isClassicController().
int CMainMenu_GetInstancePtr();
void ArtsInfo_SetReadyFlag();
int CFade_IsVisible(CFade* self);
// (memberized: CFade_FadeIn -> CFade::In; declaration moved into class CFade)
void beginMapSelClose(CMapSel* self);
void moveMapSelUp(CMapSel* self);
void moveMapSelSelection(CMapSel* self);
void moveMapSelPageUp(CMapSel* self);
void moveMapSelPageDown(CMapSel* self);
// (memberized: getMapGridCell -> CMapSelFull::getMapGridCell; declaration moved into class CMapSelFull)
// (memberized: FloorMap_SetMapTable -> CFloorMap::SetMapTable; declaration moved into class CFloorMap)
void FloorMap_LoadMapFiles(CFloorMap* self);

int FloorMap_ClassifyByte41(CFloorMap* self);
int FloorMap_CheckRowVisited(CFloorMap* self);
u8 FloorMap_GetBdatMaxRow(CFloorMap* self);
u8 FloorMap_GetField208Plain(CFloorMap* self);
u8 FloorMap_GetField208Gated(CFloorMap* self);
int FloorMap_IsSysWinBusy(CFloorMap* self);
void FloorMap_SetPaneFlag(CFloorMap* self, u32 arg);
u16 FloorMap_GetU16Field5A(CFloorMap* self);
// (memberized: FloorMap_GetActiveFlag58 -> CFloorMapFull::GetActiveFlag58; declaration moved into class CFloorMapFull)
void FloorMap_ConfirmSelect(CFloorMap* self);
void FloorMap_UpdateRowPanePos(CFloorMap* self);
void FloorMap_CursorNextFloor(CFloorMap* self);
void FloorMap_OpenRowWindow(CFloorMap* self);
void FloorMap_UpdateCursorMove(CFloorMap* self);
void FloorMap_UpdateWorldMap(CFloorMap* self);
void FloorMap_HandleMapDown(CFloorMap* self);
void FloorMap_NormalizeMapPos(CFloorMap* self);
void FloorMap_Log2Field208(CFloorMap* self);
void FloorMap_Log2Field333C(CFloorMap* self);
void FloorMap_ScrollDown(CFloorMap* self);
void FloorMap_ScrollUp(CFloorMap* self);
void FloorMap_ScrollRight(CFloorMap* self);
void FloorMap_ScrollLeft(CFloorMap* self);
void FloorMap_MoveToCursorRow(CFloorMap* self);

// CTitleAHelp help-bar id setter (retail-unmangled).
void func_801C41E8(CTitleAHelp* self, u8 arg);
}

// CTitleAHelp help-bar text reset (retail-unmangled name).
extern "C" void beginClose(CTitleAHelp* self);

// C++-linkage helper (retail emits the mangled form playUISound__FUl).
void playUISound(unsigned long op);

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
// us-802443d4 -- CMenuMapCreate: singleton factory constructor.
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
// isMapSelectActive -- singleton guard
// ---------------------------------------------------------------------------
extern "C" u32 isMapSelectActive() {
    return lbl_eu_80664790 != 0;
}

// ---------------------------------------------------------------------------
// us-80244520 -- handleWorldMapInput: world map input handler (phase gate).
// Reads turbo-press flags plus one controller-dependent trigger bit triple,
// then dispatches cursor movement / cancel / help / confirm actions.
// ---------------------------------------------------------------------------
void CMenuMapSelect::handleWorldMapInput() {
    CfPadDataView* padData = (CfPadDataView*)cf::CfGameManager::getCfPadData();

    // Trigger bits and masked-button booleans are extracted identically for
    // both layouts except for the three pressed-flag bit positions:
    // Classic controller uses bits 21-23, Wiimote/Nunchuk bits 4/5/10.
    u32 triggerBit1, triggerBit2, triggerBit3;
    u32 cancel, confirm, stickDir, menu;
    u32 turboFlags, pressedFlags, cancelVal, confirmVal, menuVal, stickVal;
    int classic = cf::CfGameManager::isClassicController(-1);
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
        (&this->mFade)->In();
        this->mState = 4;
        playUISound(3);
    } else if (triggerBit2 != 0) {
        // Show help overlay.
        beginClose(&this->mTitleAHelp);
        beginMapSelClose(&this->mMapSel);
        this->mState = 3;
    } else if (cancel != 0) {
        moveMapSelUp(&this->mMapSel);
    } else if (stickDir != 0) {
        moveMapSelSelection(&this->mMapSel);
    } else if (confirm != 0) {
        moveMapSelPageUp(&this->mMapSel);
    } else if (menu != 0) {
        moveMapSelPageDown(&this->mMapSel);
    } else if (triggerBit3 != 0) {
        // Confirm selection.
        if (CMainMenu_GetInstancePtr() != 0) {
            ArtsInfo_SetReadyFlag();
            playUISound(6);
        }
        this->mState = 11;
    }
}

// ---------------------------------------------------------------------------
// us-802446dc -- setupFloorMapPhase: world map phase 2 -> floor map setup.
// Once the fade reports ready, build a fresh CFloorMap in a stack temporary,
// assign it over the member (implicit operator= skips the vptr), select the
// current map and advance to phase 5.
// ---------------------------------------------------------------------------
void setupFloorMapPhase(CMenuMapSelect* self) {
    if (CFade_IsVisible(&self->mFade) != 0) {
        self->mFloorMap = CFloorMap();

        u8 sel = (&self->mMapSel)->getMapGridCell();
        (&self->mFloorMap)->SetMapTable(sel);
        FloorMap_LoadMapFiles(&self->mFloorMap);
        self->mState = 5;
    }
}

// ---------------------------------------------------------------------------
// us-80244be0 -- handleFloorMapInput: floor map input handler (phase 5).
// Advances the animation timer, refreshes the help bar text, then dispatches
// zoom / cursor / selection actions based on the active controller layout.
// ---------------------------------------------------------------------------
void handleFloorMapInput(CMenuMapSelect* self) {
    if (FloorMap_ClassifyByte41(&self->mFloorMap) != 0) {
        return;
    }

    self->mTimer += lbl_eu_8066872C;
    if (self->mTimer > lbl_eu_80668730) {
        self->mTimer = lbl_eu_80668730;
    }

    if (cf::CfGameManager::isClassicController(-1) != 0) {
        // Classic controller layout.
        if (FloorMap_CheckRowVisited(&self->mFloorMap) != 0) {
            if (FloorMap_GetBdatMaxRow(&self->mFloorMap) > 1 && FloorMap_GetField208Plain(&self->mFloorMap) != 0) {
                func_801C41E8(&self->mTitleAHelp, 0x4b);
            } else {
                func_801C41E8(&self->mTitleAHelp, 0x4c);
            }
        } else {
            if (FloorMap_GetBdatMaxRow(&self->mFloorMap) > 1 && FloorMap_GetField208Plain(&self->mFloorMap) != 0) {
                func_801C41E8(&self->mTitleAHelp, 0x4d);
            } else {
                func_801C41E8(&self->mTitleAHelp, 0x4e);
            }
        }

        // Zoom is only allowed while the floor map is fully shown and idle.
        u32 zoomReady = 0;
        u32 zooming = 0;
        if (FloorMap_GetBdatMaxRow(&self->mFloorMap) > 1 && FloorMap_GetField208Gated(&self->mFloorMap) != 0) {
            zooming = 1;
        }
        if (zooming != 0 && FloorMap_IsSysWinBusy(&self->mFloorMap) == 0) {
            zoomReady = 1;
        }

        FloorMap_SetPaneFlag(&self->mFloorMap, 0);

        CPad* pad = cf::CfGameManager::getCurrentPad();
        if ((pad->mTurboPressButtonFlags & 0x02000000) != 0 && zoomReady != 0) {
            FloorMap_UpdateRowPanePos(&self->mFloorMap);
            return;
        }
        if ((pad->mTurboPressButtonFlags & 0x04000000) != 0 && zoomReady != 0) {
            FloorMap_CursorNextFloor(&self->mFloorMap);
            return;
        }

        u32 pressed = pad->mPressedButtonFlags;
        if ((pressed & 0x00200000) != 0) {
            FloorMap_OpenRowWindow(&self->mFloorMap);
            if (FloorMap_GetU16Field5A(&self->mFloorMap) != 0) {
                (&self->mFade)->In();
                self->mState = 10;
            }
        } else if ((pressed & 0x00400000) != 0) {
            if ((&self->mFloorMap)->GetActiveFlag58() != 0) {
                FloorMap_ConfirmSelect(&self->mFloorMap);
            } else if (FloorMap_IsSysWinBusy(&self->mFloorMap) == 0) {
                (&self->mFade)->In();
                self->mState = 8;
                playUISound(6);
            } else {
                FloorMap_OpenRowWindow(&self->mFloorMap);
            }
        } else if ((pad->mTurboPressButtonFlags & 0x4) != 0) {
            FloorMap_UpdateCursorMove(&self->mFloorMap);
        } else if ((pad->mTurboPressButtonFlags & 0x8) != 0) {
            FloorMap_UpdateWorldMap(&self->mFloorMap);
        } else if ((pad->mTurboPressButtonFlags & 0x80000000) != 0) {
            FloorMap_HandleMapDown(&self->mFloorMap);
        } else if ((pad->mTurboPressButtonFlags & 0x2) != 0) {
            FloorMap_NormalizeMapPos(&self->mFloorMap);
        } else if ((pressed & 0x01000000) != 0) {
            FloorMap_Log2Field208(&self->mFloorMap);
        } else if ((pressed & 0x200) != 0) {
            FloorMap_Log2Field333C(&self->mFloorMap);
        } else if ((pressed & 0x800000) != 0) {
            if (FloorMap_IsSysWinBusy(&self->mFloorMap) == 0) {
                if (CMainMenu_GetInstancePtr() != 0) {
                    ArtsInfo_SetReadyFlag();
                    playUISound(6);
                }
                self->mState = 11;
            } else {
                FloorMap_OpenRowWindow(&self->mFloorMap);
            }
        }
    } else {
        // Wiimote/Nunchuk layout.
        if (FloorMap_CheckRowVisited(&self->mFloorMap) != 0) {
            if (FloorMap_GetBdatMaxRow(&self->mFloorMap) > 1 && FloorMap_GetField208Plain(&self->mFloorMap) != 0) {
                func_801C41E8(&self->mTitleAHelp, 0x4b);
            } else {
                func_801C41E8(&self->mTitleAHelp, 0x4c);
            }
        } else {
            if (FloorMap_GetBdatMaxRow(&self->mFloorMap) > 1 && FloorMap_GetField208Plain(&self->mFloorMap) != 0) {
                func_801C41E8(&self->mTitleAHelp, 0x4d);
            } else {
                func_801C41E8(&self->mTitleAHelp, 0x4e);
            }
        }

        u32 zooming = 0;
        u32 zoomReady = 0;
        if (FloorMap_GetBdatMaxRow(&self->mFloorMap) > 1 && FloorMap_GetField208Gated(&self->mFloorMap) != 0) {
            zooming = 1;
        }
        if (zooming != 0 && FloorMap_IsSysWinBusy(&self->mFloorMap) == 0) {
            zoomReady = 1;
        }

        FloorMap_SetPaneFlag(&self->mFloorMap, 0);

        CPad* pad = cf::CfGameManager::getCurrentPad();
        u32 pressed = pad->mPressedButtonFlags;
        if ((pressed & 0x10) != 0) {
            FloorMap_OpenRowWindow(&self->mFloorMap);
            if (FloorMap_GetU16Field5A(&self->mFloorMap) != 0) {
                (&self->mFade)->In();
                self->mState = 10;
            }
        } else if ((pressed & 0x20) != 0) {
            if ((&self->mFloorMap)->GetActiveFlag58() != 0) {
                FloorMap_ConfirmSelect(&self->mFloorMap);
            } else if (FloorMap_IsSysWinBusy(&self->mFloorMap) == 0) {
                (&self->mFade)->In();
                self->mState = 8;
                playUISound(6);
            } else {
                FloorMap_OpenRowWindow(&self->mFloorMap);
            }
        } else if ((pad->mHeldButtonFlags & 0x1000) != 0 && zooming != 0 &&
                   FloorMap_IsSysWinBusy(&self->mFloorMap) == 0) {
            func_801C41E8(&self->mTitleAHelp, 0x4f);
            if (self->mTimer > lbl_eu_8066872C) {
                playUISound(2);
            }
            self->mTimer = lbl_eu_80668728;
            FloorMap_SetPaneFlag(&self->mFloorMap, 1);
            if ((pad->mTurboPressButtonFlags & 0x4) != 0) {
                FloorMap_UpdateRowPanePos(&self->mFloorMap);
            } else if ((pad->mTurboPressButtonFlags & 0x8) != 0) {
                FloorMap_CursorNextFloor(&self->mFloorMap);
            }
        } else if ((pressed & 0x400) != 0) {
            if (FloorMap_IsSysWinBusy(&self->mFloorMap) == 0) {
                if (CMainMenu_GetInstancePtr() != 0) {
                    ArtsInfo_SetReadyFlag();
                    playUISound(6);
                }
                self->mState = 11;
            } else {
                FloorMap_OpenRowWindow(&self->mFloorMap);
            }
        } else if ((pad->mTurboPressButtonFlags & 0x4) != 0) {
            FloorMap_UpdateCursorMove(&self->mFloorMap);
        } else if ((pad->mTurboPressButtonFlags & 0x8) != 0) {
            FloorMap_UpdateWorldMap(&self->mFloorMap);
        } else if ((pad->mTurboPressButtonFlags & 0x80000000) != 0) {
            FloorMap_HandleMapDown(&self->mFloorMap);
        } else if ((pad->mTurboPressButtonFlags & 0x2) != 0) {
            FloorMap_NormalizeMapPos(&self->mFloorMap);
        } else if ((pressed & 0x800) != 0) {
            FloorMap_Log2Field208(&self->mFloorMap);
        } else if ((pressed & 0x200) != 0) {
            FloorMap_Log2Field333C(&self->mFloorMap);
        } else {
            // D-pad held: each direction moves the cursor once per press
            // transition; when no direction is held, confirm the selection.
            u32 flag = 1;
            if ((pad->mHeldButtonFlags & 0x8000) != 0) {
                FloorMap_ScrollDown(&self->mFloorMap);
                flag = 0;
            }
            if ((pad->mHeldButtonFlags & 0x10000) != 0) {
                FloorMap_ScrollUp(&self->mFloorMap);
                flag = 0;
            }
            if ((pad->mHeldButtonFlags & 0x2000) != 0) {
                FloorMap_ScrollRight(&self->mFloorMap);
                flag = 0;
            }
            if ((pad->mHeldButtonFlags & 0x4000) != 0) {
                FloorMap_ScrollLeft(&self->mFloorMap);
                flag = 0;
            }
            if (flag != 0) {
                FloorMap_MoveToCursorRow(&self->mFloorMap);
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
extern "C" void fwdMapSelectCbRender(CMenuMapSelect* self) {
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
void CMenuMapSelect::fwdMapSelectDtor() {
    // Single-arg call leaves the delete-flag register untouched (retail thunk
    // does not set r4), producing a plain this-adjusting tail-call.
    __dt__14CMenuMapSelectFv((CMenuMapSelect*)((u8*)this - 0x58));
}
