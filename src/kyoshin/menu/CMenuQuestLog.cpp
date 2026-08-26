// Auto-scaffolded catalog TU for kyoshin/menu/CMenuQuestLog
// Replace stubs with high-level C/C++ during decomp.

// Retail func_8011C998 is a copy constructor that returns dst (r3), but the
// shared headers still declare a void-returning scaffold. Rename that
// declaration aside for this TU only so the real definition below can use the
// retail signature; other TUs are unaffected (they ignore the return value).
#define func_8011C998 questlog_scaffold_void_func_8011C998
#include "kyoshin/menu/CMenuQuestLog.hpp"
#undef func_8011C998

#include "kyoshin/CTaskGame.hpp"          // CTaskGame::getInstance / func_800426F0
#include "kyoshin/cf/CfGameManager.hpp"  // cf::CfGameManager::getCurrentPad / func_80086F9C
#include "kyoshin/code_80135FDC.hpp"     // func_8013BE50 / func_80137250
#include "monolib/core/CPadManager.hpp"   // CPad::mPressedButtonFlags
#include "monolib/util/MemManager.hpp"
#include "monolib/device/CDeviceVI.hpp"
#include "monolib/work/CWorkThreadSystem.hpp"

#include <revolution/GX.h>
#include <nw4r/lyt/lyt_drawInfo.h>

extern "C" void __dt__13CMenuQuestLogFv(void*, int);

// Retail constructor symbol (unmangled global). Out-of-line stub so the
// factory (func_8011CCE0) emits a real `bl` to it; returns `this` in r3 like
// a real constructor (retail relies on it). C linkage inherited from the
// header declaration.
__declspec(noinline) CMenuQuestLog* __ct__CMenuQuestLog(CMenuQuestLog* _this, CProcess* parent, u32 arg2) {
    __ct__8CProcessFv(_this);

    // Temp (CProcess) primary vtable.
    *(u32*)((u8*)_this + 0x10) = (u32)lbl_eu_8052BF70;

    // Null PMF callback slot groups: materialize the __ptmf_null base once
    // via an update-form first load (*n++), index remaining words relative to
    // the advanced pointer; each group stores its second slot first.
    u32* n = __ptmf_null;
    u32 a0 = *n++;
    u32 renderVt = (u32)lbl_eu_8052CC38 + 0x24;
    u32 a1 = n[0];
    _this->ptmf0[1] = a1;
    _this->ptmf0[0] = a0;
    u32 a2 = n[1];
    _this->ptmf0[2] = a2;

    a0 = n[-1];
    a1 = n[0];
    _this->ptmf1[1] = a1;
    _this->ptmf1[0] = a0;
    a2 = n[1];
    _this->ptmf1[2] = a2;

    _this->mField54 = 0;
    _this->mField55 = 0;

    *(u32*)((u8*)_this + 0x10) = (u32)lbl_eu_8052CC38;
    *(u32*)((u8*)_this + 0x58) = renderVt;
    _this->mScene = parent;

    // Embedded widgets in construction order.
    __ct__CBgTex(&_this->mBgTex, 0);
    __ct__CTitleAHelp(&_this->mTitleAHelp, 0, 0);
    __ct__CQstLogList(&_this->mQstLogList, 0);
    __ct__CQstLogInfo(&_this->mQstLogInfo);

    _this->field_2280 = arg2;
    _this->mState = 0;
    func_8008294C__Q22cf13CfGameManagerFv(1);
    code80135FDC_postIncByte_64080();
    return _this;
}

CMenuQuestLog::~CMenuQuestLog() {}

void CMenuQuestLog::Init() {}

// retail: lwz x4 from r4; stw x4 to r3 (4-word copy, const src avoids interleave)
extern "C" void __ct__UnkClass_8011C974(void* self, const u32* src) {
    ((u32*)self)[0] = src[0];
    ((u32*)self)[1] = src[1];
    ((u32*)self)[2] = src[2];
    ((u32*)self)[3] = src[3];
}

// CScrollBar copy helper (inlined copy constructor): constructs the
// mem-region sub-object at +0x04 from the source, then copies the remaining
// scalar fields one at a time. Retail keeps `this` in r3, so this is written
// with a pointer return; src is read through a volatile ref so the compiler
// cannot hoist the loads (retail reloads the source per field).
extern "C" CScrollBarData* func_8011C998(CScrollBarData* dst, CScrollBarData* src) {
    __ct__UnkClass_8011C974(&dst->mMemRegion[0], &src->mMemRegion[0]);
    dst->mFileHandle = src->mFileHandle;
    dst->mAccessor = src->mAccessor;
    dst->mLayout = src->mLayout;
    dst->mAnimTrans = src->mAnimTrans;
    dst->mReady = src->mReady;
    dst->mVisible = src->mVisible;
    dst->mState = src->mState;
    dst->mActive = src->mActive;
    dst->mAnimOffset = src->mAnimOffset;
    dst->mScrollPosY = src->mScrollPosY;
    dst->mScrollRatio = src->mScrollRatio;
    dst->mThumbHeight = src->mThumbHeight;
    dst->mContentHeight = src->mContentHeight;
    dst->mDirection = src->mDirection;
    return dst;
}

// ---------------------------------------------------------------------------
// ---- CMenuQuestLog::Term (us-8011d528) -------------------------------------
// Wait for the VI, deregister the render callback (this-adjusting IScnRender
// view at +0x58), tear down the embedded widgets, clear the singleton, then
// re-enable the cf load once if the boot-time byte flag is still clear.
// ---------------------------------------------------------------------------
void CMenuQuestLog::Term() {
    CDeviceVI::waitForDrawDone();
    IScnRender* render = reinterpret_cast<IScnRender*>(this);
    if (this) render = reinterpret_cast<IScnRender*>(&mIScnRender);
    removeRenderCB__4CScnFP10IScnRender(reinterpret_cast<CScn*>(mScene), render);
    func_801C3D9C(&mBgTex);
    func_801C40A0(&mTitleAHelp);
    func_80227BD8(&mQstLogList);
    func_80229620(&mQstLogInfo);
    lbl_eu_80663FC0 = 0;
    func_8013B980();
    if (code80135FDC_getByte_64080() == 0)
        func_8008294C__Q22cf13CfGameManagerFv(false);
    func_80135550();
}

void CMenuQuestLog::Move() {
    CTaskGame::getInstance();
    if (CTaskGame::func_800426F0()) {
        goto exit;
    }
    // Branch-over-branch guard (same shape as cbRenderBefore): exit label
    // before body label emits retail's `beq body; b exit`.
    if (!(lbl_eu_80663E28 & (1u << 21))) {
        goto body;
    }
    goto exit;
exit:
    return;
body:
    // Close request: classic controller bit 23 / wii bit 10 plays the close
    // sound and flags the closing state.
    CPad* pad = cf::CfGameManager::getCurrentPad();
    u32 close;
    if (func_80086F9C__Q22cf13CfGameManagerFv(-1) != 0) {
        close = (pad->mPressedButtonFlags >> 23) & 1;
    } else {
        close = (pad->mPressedButtonFlags >> 10) & 1;
    }
    if (close != 0) {
        if (func_800FEDF8()) {
            func_800FF914();
        }
        func_80138078(6);
        mState = 0xa;
        mField54 = 1;
    }

    // State machine (jumptable dispatch on the state byte).
    switch (mState) {
    case 0:
        func_8011CD6C(this);
        break;
    case 1:
        func_8011CDF4(reinterpret_cast<unsigned char*>(this));
        break;
    case 2:
        func_8011CE44(this);
        break;
    case 3:
        func_8011D03C(reinterpret_cast<unsigned char*>(this));
        break;
    case 4:
        func_8011D08C(reinterpret_cast<unsigned char*>(this));
        break;
    case 5:
        func_8011D0FC(this);
        break;
    case 6:
        func_8011D158(this);
        break;
    case 7:
        func_8011D1A8(this);
        break;
    case 8:
        func_8011D22C(this);
        break;
    case 9:
        func_8011D298(this);
        break;
    }

    func_801C3D54(&mBgTex);
    func_801C3FF0(&mTitleAHelp);
    func_80227AC4(&mQstLogList);
    func_80229570(&mQstLogInfo);
}

__declspec(noinline) void CMenuQuestLog::cbRenderBefore() {
    CTaskGame::getInstance();
    if (CTaskGame::func_800426F0()) {
        goto exit;
    }
    // Branch-over-branch guard keeps MWCC from folding the bit test into a
    // single branch -- emits retail's `beq body; b exit`.
    if (!(lbl_eu_80663E28 & (1u << 21))) {
        goto body;
    }
    goto exit;
exit:
    return;
body:
    if (func_8013BE50() == 0) {
        goto exit;
    }
    GXSetZMode(GX_FALSE, GX_NEVER, GX_FALSE);
    {
        nw4r::lyt::DrawInfo drawInfo;
        func_80137250(&drawInfo);
        func_801C3D7C(&mBgTex, &drawInfo);
        func_80227B6C(&mQstLogList, &drawInfo);
        func_80229600(&mQstLogInfo, &drawInfo);
        func_801C4080(&mTitleAHelp, &drawInfo);
    }
}

// ---------------------------------------------------------------------------
// ---- func_8011CCE0 (us-8011d7bc) ------------------------------------------
// Factory: lazily allocate + construct the single quest-log menu instance and
// register it as a child of `self`. Returns the stored singleton (or 0 if it
// already exists). Regist is called even when the allocation failed, matching
// retail.
// ---------------------------------------------------------------------------
extern "C" void Regist__8CProcessFP8CProcessb(void* self, CProcess* parent, bool b);

CMenuQuestLog* func_8011CCE0(CProcess* self, CProcess* parent, u32 arg2) {
    if (lbl_eu_80663FC0 != 0) {
        return 0;
    }
    CMenuQuestLog* obj = (CMenuQuestLog*)mtl::MemManager::allocate(
        0x2288, CWorkThreadSystem::getWorkMem());
    if (obj != 0) {
        obj = __ct__CMenuQuestLog(obj, parent, arg2);
    }
    lbl_eu_80663FC0 = obj;
    Regist__8CProcessFP8CProcessb(obj, self, false);
    return lbl_eu_80663FC0;
}

// (lbl_eu_80663FC0 != 0) - retail lwz sda21; subic; subfe
extern "C" bool func_8011CD5C() { return lbl_eu_80663FC0 != 0; }

// State-machine step for state 1: once the background layout, title help,
// list and info widget are all ready, open the title help + list, set state 1
// and play sound 0x6d.
extern "C" void func_8011CD6C(CMenuQuestLog* self) {
    if (func_801C3E34(&self->mBgTex)) {
        if (func_801C4114(&self->mTitleAHelp)) {
            if (func_80227C70(&self->mQstLogList)) {
                if (func_802296D0(&self->mQstLogInfo)) {
                    func_801C412C(&self->mTitleAHelp);
                    func_80227CDC(&self->mQstLogList);
                    self->mState = 1;
                    func_80138078(0x6d);
                }
            }
        }
    }
}

// Gate the quest-log list state: when the help sub-object is idle and the
// list accepts input, raise the state byte to 2.
extern "C" void func_8011CDF4(unsigned char* self) {
    if (isIdle__11CTitleAHelpFv(self + 0x80) && func_80227CCC(self + 0xB8))
        self[0x2284] = 2;
}

// Retail func_8011CE44 is a large real handler (us-8011d920); keep this
// placeholder out-of-line so Move's case 2 still emits a real bl.
// State-2 input handler: decodes the pad (classic vs wiimote bit positions)
// and drives the list widget: confirm opens the info pane, up/down scroll,
// cancel/dir/confirm/menu shortcuts, and the sort trigger re-sorts + updates
// the title-help mode depending on whether sorting is available.
extern "C" __declspec(noinline) void func_8011CE44(CMenuQuestLog* self) {
    CQuestLogPadData* pad = (CQuestLogPadData*)cf::CfGameManager::getCfPadData();
    int cancel, trigger3, trigger1, trigger2, confirm, menu, dir;
    // The bit positions differ between Classic Controller and Wiimote.
    if (func_80086F9C__Q22cf13CfGameManagerFv(-1) != 0) {
        u32 f = pad->mTurboPressButtonFlags;
        u32 p = pad->mPressedButtonFlags;
        cancel = (f & 0x8004) != 0;
        dir = ((f & 0x10000) | (f & 0x8)) != 0;
        confirm = (f & 0x2001) != 0;
        menu = (f & 0x4002) != 0;
        trigger1 = (p >> 21) & 1;
        trigger2 = (p >> 22) & 1;
        trigger3 = (p >> 24) & 1;
    } else {
        u32 f = pad->mTurboPressButtonFlags;
        u32 p = pad->mPressedButtonFlags;
        cancel = (f & 0x8004) != 0;
        dir = ((f & 0x10000) | (f & 0x8)) != 0;
        confirm = (f & 0x2001) != 0;
        menu = (f & 0x4002) != 0;
        trigger1 = (p >> 4) & 1;
        trigger2 = (p >> 5) & 1;
        trigger3 = (p >> 11) & 1;
    }

    if (trigger1) {
        func_802282F8(&self->mQstLogList);
        if (!func_80227CCC(&self->mQstLogList)) {
            func_801C4198(&self->mTitleAHelp);
            func_80227D78(&self->mQstLogList, 0);
            self->mState = 4;
        } else {
            func_801C41E8(&self->mTitleAHelp, 0x3c);
        }
    } else if (trigger2) {
        if (func_80227CD4(&self->mQstLogList)) {
            func_80228280(&self->mQstLogList, 0);
            func_801C41E8(&self->mTitleAHelp, 0x3c);
        } else {
            func_801C414C(&self->mTitleAHelp);
            func_80227D78(&self->mQstLogList, 1);
            self->mState = 3;
        }
    } else if (cancel) {
        func_80227DE8(&self->mQstLogList);
    } else if (dir) {
        func_80227EC8(&self->mQstLogList);
    } else if (confirm) {
        func_80227FC0(&self->mQstLogList);
    } else if (menu) {
        func_8022807C(&self->mQstLogList);
    } else if (trigger3) {
        func_80228164(&self->mQstLogList);
        func_801C41E8(&self->mTitleAHelp, func_80227CD4(&self->mQstLogList) ? 0x3e : 0x3c);
    }
}

// Same gate as func_8011CDF4 but toggles the +0x54 byte to 1.
extern "C" void func_8011D03C(unsigned char* self) {
    if (isIdle__11CTitleAHelpFv(self + 0x80) && func_80227CCC(self + 0xB8))
        self[0x54] = 1;
}

void func_8011D08C(unsigned char* self)
{
    unsigned char* base = self;
    if (isIdle__11CTitleAHelpFv(base + 0x80) && func_80227CCC(base + 0xB8)) {
        unsigned int value = func_80228394(base + 0xB8);
        func_80229768(base + 0x2240, static_cast<unsigned short>(value));
        func_80229510(base + 0x2240);
        base[0x2284] = 5;
    }
}

// ---------------------------------------------------------------------------
// ---- func_8011D0FC (us-8011dbd8) ------------------------------------------
// When the quest log info is done, set the title-help mode to 0x3d, close the
// title help, finish the log info and advance to state 6.
// ---------------------------------------------------------------------------
extern "C" void func_8011D0FC(CMenuQuestLog* self) {
    if (func_802296D0(&self->mQstLogInfo)) {
        func_801C41E8(&self->mTitleAHelp, 0x3d);
        func_801C416C(&self->mTitleAHelp);
        func_802296E0(&self->mQstLogInfo);
        self->mState = 6;
    }
}

// ---------------------------------------------------------------------------
// ---- func_8011D158 (us-8011dc34) ------------------------------------------
// When the title help is idle and the quest log info is done, advance to
// state 7.
// ---------------------------------------------------------------------------
extern "C" void func_8011D158(CMenuQuestLog* self) {
    if (isIdle__11CTitleAHelpFv(&self->mTitleAHelp) && func_802296D8(&self->mQstLogInfo)) {
        self->mState = 7;
    }
}

// ---------------------------------------------------------------------------
// ---- func_8011D1A8 (us-8011dc84) ------------------------------------------
// State 7 handler: on a close-button press (classic bit 23 / wii bit 10),
// close the title help, finish the log info and advance to state 8.
// ---------------------------------------------------------------------------
extern "C" void func_8011D1A8(CMenuQuestLog* self) {
    CPad* pad = cf::CfGameManager::getCurrentPad();
    // Close-button press: classic-controller bits 22-23 vs wii bits 4-5,
    // normalized to bool (retail's rlwinm + subic/subfe shape).
    bool close;
    if (func_80086F9C__Q22cf13CfGameManagerFv(-1)) {
        close = pad->mPressedButtonFlags & 0x600000;
    } else {
        close = pad->mPressedButtonFlags & 0x30;
    }
    if (close) {
        func_801C4198(&self->mTitleAHelp);
        func_80229724(&self->mQstLogInfo);
        self->mState = 8;
    }
}

// ---------------------------------------------------------------------------
// ---- func_8011D22C (us-8011dd08) ------------------------------------------
// When the title help is idle and the quest log info is done, set the
// title-help mode to 0x3c, close the title help, re-sort the list and advance
// to state 9.
// ---------------------------------------------------------------------------
extern "C" void func_8011D22C(CMenuQuestLog* self) {
    if (isIdle__11CTitleAHelpFv(&self->mTitleAHelp) && func_802296D8(&self->mQstLogInfo)) {
        func_801C41E8(&self->mTitleAHelp, 0x3c);
        func_801C416C(&self->mTitleAHelp);
        func_80227CDC(&self->mQstLogList);
        self->mState = 9;
    }
}

// ---------------------------------------------------------------------------
// ---- func_8011D298 (us-8011dd74) ------------------------------------------
// When the title help is idle and sorting is enabled, advance to state 2.
// ---------------------------------------------------------------------------
extern "C" void func_8011D298(CMenuQuestLog* self) {
    if (isIdle__11CTitleAHelpFv(&self->mTitleAHelp) && func_80227CCC(&self->mQstLogList)) {
        self->mState = 2;
    }
}

// Adjusting thunk: upcasts from a base sub-object (at offset +0x58 within CMenuQuestLog)
// to the full CMenuQuestLog, then tail-calls cbRenderBefore.
extern "C" void func_8011D2E8(void* self) {
    reinterpret_cast<CMenuQuestLog*>(static_cast<char*>(self) - 0x58)->cbRenderBefore();
}

// Adjusting thunk: upcasts from a base sub-object (at offset +0x58 within CMenuQuestLog)
// to the full CMenuQuestLog, then tail-calls the destructor.
extern "C" void func_8011D2F0(void* self) {
    ((void(*)(void*))__dt__13CMenuQuestLogFv)((char*)self - 0x58);
}

// __dt__8011D2F8: deleting destructor - free self when mode > 0, return self.
extern "C" void* __dt__8011D2F8(void* self, int mode) {
    if (self != 0 && mode > 0) {
        __dl__FPv(self);
    }
    return self;
}
