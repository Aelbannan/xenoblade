// Auto-scaffolded catalog TU for kyoshin/menu/CMenuQuestLog
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/menu/CMenuQuestLog.hpp"

#include "kyoshin/CTaskGame.hpp"          // CTaskGame::getInstance / func_800426F0
#include "kyoshin/cf/CfGameManager.hpp"  // cf::CfGameManager::getCurrentPad / func_80086F9C
#include "kyoshin/code_80135FDC.hpp"     // func_8013BE50 / func_80137250
#include "monolib/core/CPadManager.hpp"   // CPad::mPressedButtonFlags
#include "monolib/util/MemManager.hpp"
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

    // Null PMF callback slot groups.
    _this->ptmf0[0] = __ptmf_null[0];
    _this->ptmf0[1] = __ptmf_null[1];
    _this->ptmf0[2] = __ptmf_null[2];
    _this->ptmf1[0] = __ptmf_null[0];
    _this->ptmf1[1] = __ptmf_null[1];
    _this->ptmf1[2] = __ptmf_null[2];

    _this->mField54 = 0;
    _this->mField55 = 0;

    // Composite vtable + the IScnRender sub-vtable at +0x58.
    *(u32*)((u8*)_this + 0x10) = (u32)lbl_eu_8052CC38;
    *(u32*)((u8*)_this + 0x58) = (u32)lbl_eu_8052CC38 + 0x24;
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

// CScrollBar copy helper: constructs the mem-region sub-object at +0x04 from
// the source, then copies the remaining scalar fields in retail order. The
// source pointer is de-const'ed so every load/store pair stays in program
// order (retail emits one r0/f0 load+store per field).
extern "C" void func_8011C998(CScrollBarData* dst, const CScrollBarData* src) {
    CScrollBarData* s = (CScrollBarData*)src;
    __ct__UnkClass_8011C974(&dst->mMemRegion[0], &s->mMemRegion[0]);
    dst->mFileHandle = s->mFileHandle;
    dst->mAccessor = s->mAccessor;
    dst->mLayout = s->mLayout;
    dst->mAnimTrans = s->mAnimTrans;
    dst->mReady = s->mReady;
    dst->mVisible = s->mVisible;
    dst->mState = s->mState;
    dst->mActive = s->mActive;
    dst->mAnimOffset = s->mAnimOffset;
    dst->mScrollPosY = s->mScrollPosY;
    dst->mScrollRatio = s->mScrollRatio;
    dst->mThumbHeight = s->mThumbHeight;
    dst->mContentHeight = s->mContentHeight;
    dst->mDirection = s->mDirection;
}

void CMenuQuestLog::Term() {}

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
    if (cf::CfGameManager::func_80086F9C(-1)) {
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
void func_8011CD6C(CMenuQuestLog* self) {
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

void func_8011CE44(){}

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
void func_8011D0FC(CMenuQuestLog* self) {
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
void func_8011D158(CMenuQuestLog* self) {
    if (isIdle__11CTitleAHelpFv(&self->mTitleAHelp) && func_802296D8(&self->mQstLogInfo)) {
        self->mState = 7;
    }
}

void func_8011D1A8(){}

// ---------------------------------------------------------------------------
// ---- func_8011D22C (us-8011dd08) ------------------------------------------
// When the title help is idle and the quest log info is done, set the
// title-help mode to 0x3c, close the title help, re-sort the list and advance
// to state 9.
// ---------------------------------------------------------------------------
void func_8011D22C(CMenuQuestLog* self) {
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
void func_8011D298(CMenuQuestLog* self) {
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
