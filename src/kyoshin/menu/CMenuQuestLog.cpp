// Auto-scaffolded catalog TU for kyoshin/menu/CMenuQuestLog
// Replace stubs with high-level C/C++ during decomp.

#define ISCN_RENDER_NO_DTOR // see CMenuQuestLog.hpp

// Retail copyScrollBarData is a copy constructor that returns dst (r3), but the
// shared headers still declare a void-returning scaffold. Rename that
// declaration aside for this TU only so the real definition below can use the
// retail signature; other TUs are unaffected (they ignore the return value).
#define copyScrollBarData questlog_scaffold_void_copyScrollBarData
// CQstLogList.hpp declares __ct__UnkClass_8011C974(void*, const u32*); rename
// that aside so this TU can define the typed u32* dest form without overload clash.
#define __ct__UnkClass_8011C974 questlog_void_ct_UnkClass_8011C974
#include "kyoshin/menu/CMenuQuestLog.hpp"
#undef copyScrollBarData
#undef __ct__UnkClass_8011C974

#include "kyoshin/CTaskGame.hpp"
#include "kyoshin/cf/CfGameManager.hpp"  // cf::CfGameManager::getCurrentPad / isClassicController
#include "kyoshin/code_80135FDC.hpp"     // IsMenuState621F0 / SetWideLocAdjust
#include "monolib/core/CPadManager.hpp"   // CPad::mPressedButtonFlags
#include "monolib/util/MemManager.hpp"
#include "monolib/device/CDeviceVI.hpp"
#include "monolib/work/CWorkThreadSystem.hpp"

#include <revolution/GX.h>
#include <nw4r/lyt/lyt_drawInfo.h>

extern "C" CMenuQuestLog* __dt__13CMenuQuestLogFv(CMenuQuestLog* ths, int);
extern "C" void __ct__UnkClass_8011C974(u32* dest, const u32* src);
extern "C" void __dt__800FED0C(CProcess* self, int flags);

// CProcess primary vptr @ +0x10 (novtable base; same slot as other menus).
struct CProcessPrimaryVptr {
    u8 _00[0x10];
    u32* mVtbl;
};


// Retail constructor symbol (unmangled global). Out-of-line stub so the
// factory (createQuestLogMenu) emits a real `bl` to it; returns `this` in r3 like
// a real constructor (retail relies on it). C linkage inherited from the
// header declaration.
__declspec(noinline) CMenuQuestLog* __ct__CMenuQuestLog(CMenuQuestLog* _this, CProcess* parent, u32 arg2) {
    __ct__8CProcessFv(_this);

    // Temp (CProcess) primary vtable.
    reinterpret_cast<CProcessPrimaryVptr*>(_this)->mVtbl =
        reinterpret_cast<u32*>(lbl_eu_8052BF70);

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

    reinterpret_cast<CProcessPrimaryVptr*>(_this)->mVtbl =
        reinterpret_cast<u32*>(lbl_eu_8052CC38);
    *(u32*)&_this->mIScnRender = renderVt;
    _this->mScene = parent;

    // Embedded widgets in construction order.
    __ct__CBgTex(&_this->mBgTex, 0);
    __ct__CTitleAHelp(&_this->mTitleAHelp, 0, 0);
    __ct__CQstLogList(&_this->mQstLogList, 0);
    __ct__CQstLogInfo(&_this->mQstLogInfo);

    _this->field_2280 = arg2;
    _this->mState = 0;
    setPresentationFlag__Q22cf13CfGameManagerFv(1);
    code80135FDC_postIncByte_64080();
    return _this;
}

/*
 * Destructor (retail D2 form). Free-function with flags so the base call
 * relocates to __dt__800FED0C (game D2), not library __dt__8CProcessFv.
 * Subobjects destroyed in reverse construction order.
 */
extern "C" CMenuQuestLog* __dt__13CMenuQuestLogFv(CMenuQuestLog* ths, int flags) {
    if (ths != 0) {
        __dt__11CQstLogInfoFv(&ths->mQstLogInfo, -1);
        __dt__11CQstLogListFv(&ths->mQstLogList, -1);
        __dt__11CTitleAHelpFv(&ths->mTitleAHelp, -1);
        __dt__6CBgTexFv(&ths->mBgTex, -1);
        __dt__800FED0C(ths, 0);
        if (flags > 0) {
            operator delete(ths);
        }
    }
    return ths;
}

// ---------------------------------------------------------------------------
// ---- CMenuQuestLog::Init (us-8011d08c) -------------------------------------
// Rebuild each embedded widget by constructing a stack temporary and copying
// its body into the embedded storage (same pattern as CMenuCollepedia::Init),
// then register the render-callback sub-object on the owning scene.
// ---------------------------------------------------------------------------
// Retail signature copy helper (the scaffold decl is renamed aside at include
// time; this prototype serves the Init() call sites).
extern "C" CScrollBarData* copyScrollBarData(CScrollBarData* dst, CScrollBarData* src);

void CMenuQuestLog::Init() {
    // Stack temporaries packed into one aggregate so the slot layout matches
    // retail exactly: bg @sp+0x8, info @sp+0x28, title @sp+0x68, list
    // @sp+0xa0 (frame 0x2230).
    struct {
        u8 bg[0x20];
        u8 info[0x40];
        u8 title[0x38];
        u8 list[0x2188];
    } tmp;

    // --- CBgTex ---
    __ct__CBgTex((CBgTex*)tmp.bg, 0);
    __ct__UnkClass_8011C974(reinterpret_cast<u32*>(&mBgTex.mMemRegion), (const u32*)(tmp.bg + 0x4));
    mBgTex.mFileHandle = *(CFileHandle**)(tmp.bg + 0x14);
    mBgTex.mLayout = *(nw4r::lyt::Layout**)(tmp.bg + 0x18);
    mBgTex.mLayoutReady = *(bool*)(tmp.bg + 0x1c);
    mBgTex.mLoaded = tmp.bg[0x1d];
    mBgTex.mPtmMode = tmp.bg[0x1e];
    __dt__6CBgTexFv((CBgTex*)tmp.bg, -1);
    BgTex_Acquire_3C14(&mBgTex);

    // --- CTitleAHelp ---
    char* name = BdatTouchStringCell(lbl_eu_804FE518, lbl_eu_804FE518 + 0xa, 1);
    __ct__CTitleAHelp((CTitleAHelp*)tmp.title, name, 0x3c);
    __ct__UnkClass_8011C974(reinterpret_cast<u32*>(&mTitleAHelp.unk4), (const u32*)(tmp.title + 0x4));
    mTitleAHelp.mFileHandle = *(CFileHandle**)(tmp.title + 0x14);
    mTitleAHelp.mArcResourceAccessor =
        *(nw4r::lyt::ArcResourceAccessor**)(tmp.title + 0x18);
    mTitleAHelp.mLayout = *(nw4r::lyt::Layout**)(tmp.title + 0x1c);
    mTitleAHelp.mAnimTrans20 = *(nw4r::lyt::AnimTransform**)(tmp.title + 0x20);
    mTitleAHelp.mAnimTrans24 = *(nw4r::lyt::AnimTransform**)(tmp.title + 0x24);
    mTitleAHelp.unk28 = tmp.title[0x28];
    mTitleAHelp.unk2c = *(s32*)(tmp.title + 0x2c);
    mTitleAHelp.mName = *(char**)(tmp.title + 0x30);
    mTitleAHelp.unk34 = tmp.title[0x34];
    mTitleAHelp.unk35 = tmp.title[0x35];
    mTitleAHelp.unk36 = tmp.title[0x36];
    mTitleAHelp.unk37 = tmp.title[0x37];
    __dt__11CTitleAHelpFv((CTitleAHelp*)tmp.title, -1);
    CTitleAHelp_load(&mTitleAHelp);

    // --- CQstLogList ---
    __ct__CQstLogList((CQstLogList*)tmp.list, (u16)field_2280);
    __ct__UnkClass_8011C974(reinterpret_cast<u32*>(&mQstLogList.mUnk04[0]), (const u32*)(tmp.list + 0x4));
    mQstLogList.mFileHandle = *(CFileHandle**)(tmp.list + 0x14);
    mQstLogList.mArcResAcc = *(nw4r::lyt::ArcResourceAccessor**)(tmp.list + 0x18);
    mQstLogList.mpLayout = *(nw4r::lyt::Layout**)(tmp.list + 0x1c);
    mQstLogList.mpAnim0 = *(nw4r::lyt::AnimTransform**)(tmp.list + 0x20);
    mQstLogList.mpAnim1 = *(nw4r::lyt::AnimTransform**)(tmp.list + 0x24);
    mQstLogList.mCur18 = reinterpret_cast<CQstLogList*>(tmp.list)->mCur18;
    copyScrollBarData(&mQstLogList.mScrollBar, (CScrollBarData*)(tmp.list + 0x40));
    __ct__UnkClass_8011C974(reinterpret_cast<u32*>(&mQstLogList.mSortMenuData.mUnk04[0]), (const u32*)(tmp.list + 0x84));
    mQstLogList.mSortMenuData.mFileHandle = *(u32*)(tmp.list + 0x94);
    mQstLogList.mSortMenuData.mArcResAcc = *(u32*)(tmp.list + 0x98);
    mQstLogList.mSortMenuData.mpLayout = *(u32*)(tmp.list + 0x9c);
    mQstLogList.mSortMenuData.mpAnimTrans0 = *(u32*)(tmp.list + 0xa0);
    mQstLogList.mSortMenuData.mpAnimTrans1 = *(u32*)(tmp.list + 0xa4);
    mQstLogList.mSortMenuData.field_0x28 = tmp.list[0xa8];
    mQstLogList.mSortMenuData.field_0x29 = tmp.list[0xa9];
    mQstLogList.mSortMenuData.field_0x2A = tmp.list[0xaa];
    mQstLogList.mSortMenuData.field_0x2B = tmp.list[0xab];
    copyScrollBarData(&mQstLogList.mSortMenuData.mScrollBar,
                  (CScrollBarData*)(tmp.list + 0xac));
    // Sort-menu entry table + quest-info buffer: aggregate assignments let
    // MWCC emit its canonical copies (paired-update word loop; fieldwise
    // record loop + halfword/byte tail).
    {
        struct QstArrBlock { u32 words[32]; };
        // mSortMenuData.mArray @ list+0xEC = this+0x1A4
        *(QstArrBlock*)mQstLogList.mSortMenuData.mArray =
            *(QstArrBlock*)(tmp.list + 0xec);
    }
    // Sort/list state scalars.
    mQstLogList.mSortMenuData.mCount = tmp.list[0x16c];
    mQstLogList.mSortMenuData.mPage = tmp.list[0x16d];
    mQstLogList.mSortMenuData.mSubPage = tmp.list[0x16e];
    mQstLogList.field_0x170 = tmp.list[0x170];
    mQstLogList.field_0x174 = *(s32*)(tmp.list + 0x174);
    mQstLogList.field_0x178 = tmp.list[0x178];
    mQstLogList.mSortEnabled = tmp.list[0x179];
    mQstLogList.mSortDescending = tmp.list[0x17a];
    mQstLogList.field_0x17B = tmp.list[0x17b];
    mQstLogList.field_0x17C = tmp.list[0x17c];
    mQstLogList.field_0x17D = tmp.list[0x17d];
    mQstLogList.field_0x17E = *(s16*)(tmp.list + 0x17e);
    mQstLogList.field_0x180 = *(u16*)(tmp.list + 0x180);
    // Quest-info records copied fieldwise (u16 + 6 bytes each).
    // Residual vs retail (pure reg-swap, see attempts.jsonl): retail colors
    // the scalar temporaries r3 and the loop pointers qDst=qSrc=r5/qEnd=r0
    // (qEnd reuses the dead r0 from the preceding block-copy loop); this
    // shape yields temps=r0, qDst=r3, qSrc=r4, qEnd=r5.
    CQstLogListQstInfo* qDst =
        (CQstLogListQstInfo*)&mQstLogList.mQstData.mList[0];
    CQstLogList* srcList = (CQstLogList*)tmp.list;
    CQstLogListQstInfo* qSrc =
        (CQstLogListQstInfo*)&srcList->mQstData.mList[0];
    CQstLogListQstInfo* qEnd =
        (CQstLogListQstInfo*)&mQstLogList.mQstData.field_2000;
    do {
        *qDst = *qSrc;
        qDst++;
        qSrc++;
    } while (qDst < qEnd);
    mQstLogList.mQstData.field_2000 = *(u16*)(tmp.list + 0x2182);
    mQstLogList.mQstData.field_2002 = *(s16*)(tmp.list + 0x2184);
    mQstLogList.mQstData.field_2004 = tmp.list[0x2186];
    mQstLogList.mQstData.field_2005 = tmp.list[0x2187];
    __dt__11CQstLogListFv((CQstLogList*)tmp.list, -1);
    QstLogList_LoadArc(&mQstLogList);

    // --- CQstLogInfo ---
    __ct__CQstLogInfo((CQstLogInfo*)tmp.info);
    __ct__UnkClass_8011C974(reinterpret_cast<u32*>(&mQstLogInfo.mMemRegion), (const u32*)(tmp.info + 0x4));
    mQstLogInfo.mFileHandle = *(CFileHandle**)(tmp.info + 0x14);
    mQstLogInfo.field_0x18 = *(CFileHandle**)(tmp.info + 0x18);
    mQstLogInfo.field_0x1C = *(u32*)(tmp.info + 0x1c);
    mQstLogInfo.mUnk20 = *(nw4r::lyt::Layout**)(tmp.info + 0x20);
    mQstLogInfo.field_0x24 = *(nw4r::lyt::AnimTransform**)(tmp.info + 0x24);
    mQstLogInfo.field_0x28 = *(nw4r::lyt::AnimTransform**)(tmp.info + 0x28);
    mQstLogInfo.field_0x2C = *(u32*)(tmp.info + 0x2c);
    mQstLogInfo.mUnk30 = tmp.info[0x30];
    mQstLogInfo.field_0x34 = *(s32*)(tmp.info + 0x34);
    mQstLogInfo.mField38 = tmp.info[0x38];
    mQstLogInfo.mField39 = tmp.info[0x39];
    mQstLogInfo.mField3A = *(u16*)(tmp.info + 0x3a);
    mQstLogInfo.field_0x3C = *(u32*)(tmp.info + 0x3c);
    __dt__11CQstLogInfoFv((CQstLogInfo*)tmp.info, -1);
    QstLogInfo_LoadArcHandle(&mQstLogInfo);

    // Register the render callback (this-adjusting IScnRender view at +0x58).
    IScnRender* renderCb = reinterpret_cast<IScnRender*>(this);
    if (this != 0) renderCb = reinterpret_cast<IScnRender*>(&mIScnRender);
    reinterpret_cast<CScn*>(mScene)->addRenderCB(renderCb, 0x10, 0);
}

// retail: lwz x4 from r4; stw x4 to r3 (4-word copy, const src avoids interleave)
extern "C" void __ct__UnkClass_8011C974(u32* dest, const u32* src) {
    dest[0] = src[0];
    dest[1] = src[1];
    dest[2] = src[2];
    dest[3] = src[3];
}

// CScrollBar copy helper (inlined copy constructor): constructs the
// mem-region sub-object at +0x04 from the source, then copies the remaining
// scalar fields one at a time. Retail keeps `this` in r3, so this is written
// with a pointer return; src is read through a volatile ref so the compiler
// cannot hoist the loads (retail reloads the source per field).
extern "C" CScrollBarData* copyScrollBarData(CScrollBarData* dst, CScrollBarData* src) {
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
    reinterpret_cast<CScn*>(mScene)->removeRenderCB(render);
    BgTex_Release_3D9C(&mBgTex);
    teardown(&mTitleAHelp);
    QstLogList_Teardown(&mQstLogList);
    QstLogInfo_TeardownLayout(&mQstLogInfo);
    lbl_eu_80663FC0[0] = 0;
    DecMenuCounter64080();
    if (code80135FDC_getByte_64080() == 0)
        setPresentationFlag__Q22cf13CfGameManagerFv(false);
    CUICfManager_setTimeout30();
}

void CMenuQuestLog::Move() {
    CTaskGame::getInstance();
    if (CTaskGame::isFlag01Set()) {
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
    if (isClassicController__Q22cf13CfGameManagerFv(-1) != 0) {
        close = (pad->mPressedButtonFlags >> 23) & 1;
    } else {
        close = (pad->mPressedButtonFlags >> 10) & 1;
    }
    if (close != 0) {
        if (CMainMenu_GetInstancePtr()) {
            ArtsInfo_SetReadyFlag();
        }
        playUISound(6);
        mState = 0xa;
        mField54 = 1;
    }

    // State machine (jumptable dispatch on the state byte).
    switch (mState) {
    case 0:
        openQuestLogMenu(this);
        break;
    case 1:
        gateQuestLogList(this);
        break;
    case 2:
        handleQuestLogInput(this);
        break;
    case 3:
        flagQuestLogReady(this);
        break;
    case 4:
        selectQuestLogEntry(this);
        break;
    case 5:
        startQuestLogInfo(this);
        break;
    case 6:
        finishQuestLogInfo(this);
        break;
    case 7:
        closeQuestLogMenu(this);
        break;
    case 8:
        resortQuestLogList(this);
        break;
    case 9:
        reopenQuestLogList(this);
        break;
    }

    BgTex_Tick_3D54(&mBgTex);
    updateHelp(&mTitleAHelp);
    QstLogList_FrameUpdate(&mQstLogList);
    QstLogInfo_StepStateMachine(&mQstLogInfo);
}

__declspec(noinline) void CMenuQuestLog::cbRenderBefore() {
    CTaskGame::getInstance();
    if (CTaskGame::isFlag01Set()) {
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
    if (IsMenuState621F0() == 0) {
        goto exit;
    }
    GXSetZMode(GX_FALSE, GX_NEVER, GX_FALSE);
    {
        nw4r::lyt::DrawInfo drawInfo;
        SetWideLocAdjust(&drawInfo);
        BgTex_Draw_3D7C(&mBgTex, &drawInfo);
        QstLogList_Draw(&mQstLogList, &drawInfo);
        QstLogInfo_DrawLayoutTail(&mQstLogInfo, &drawInfo);
        drawHelp(&mTitleAHelp, &drawInfo);
    }
}

// ---------------------------------------------------------------------------
// ---- createQuestLogMenu (us-8011d7bc) ------------------------------------------
// Factory: lazily allocate + construct the single quest-log menu instance and
// register it as a child of `self`. Returns the stored singleton (or 0 if it
// already exists). Regist is called even when the allocation failed, matching
// retail.
// ---------------------------------------------------------------------------
extern "C" void Regist__8CProcessFP8CProcessb(CProcess* self, CProcess* parent, bool b);

extern "C" CMenuQuestLog* createQuestLogMenu(CProcess* self, CProcess* parent, u32 arg2) {
    if (lbl_eu_80663FC0[0] != 0) {
        return 0;
    }
    CMenuQuestLog* obj = (CMenuQuestLog*)mtl::MemManager::allocate(
        0x2288, CWorkThreadSystem::getWorkMem());
    if (obj != 0) {
        obj = __ct__CMenuQuestLog(obj, parent, arg2);
    }
    lbl_eu_80663FC0[0] = obj;
    Regist__8CProcessFP8CProcessb(obj, self, false);
    return lbl_eu_80663FC0[0];
}

// (lbl_eu_80663FC0 != 0) - retail lwz sda21; subic; subfe
extern "C" bool isQuestLogMenuActive() { return lbl_eu_80663FC0[0] != 0; }

// State-machine step for state 1: once the background layout, title help,
// list and info widget are all ready, open the title help + list, set state 1
// and play sound 0x6d.
extern "C" void openQuestLogMenu(CMenuQuestLog* self) {
    if (BgTex_IsLoaded_3E34(&self->mBgTex)) {
        if (isInitialized(&self->mTitleAHelp)) {
            if (QstLogList_ScrollFlag(&self->mQstLogList)) {
                if (QstLogInfo_GetLoadedMark(&self->mQstLogInfo)) {
                    func_801C412C(&self->mTitleAHelp);
                    QstLogList_BeginSortOpen(&self->mQstLogList);
                    self->mState = 1;
                    playUISound(0x6d);
                }
            }
        }
    }
}

// Gate the quest-log list state: when the help sub-object is idle and the
// list accepts input, raise the state byte to 2.
extern "C" void gateQuestLogList(CMenuQuestLog* self) {
    if (self->mTitleAHelp.isIdle() && QstLogList_IsSortEnabled(&self->mQstLogList))
        self->mState = 2;
}

// Retail handleQuestLogInput is a large real handler (us-8011d920); keep this
// placeholder out-of-line so Move's case 2 still emits a real bl.
// State-2 input handler: decodes the pad (classic vs wiimote bit positions)
// and drives the list widget: confirm opens the info pane, up/down scroll,
// cancel/dir/confirm/menu shortcuts, and the sort trigger re-sorts + updates
// the title-help mode depending on whether sorting is available.
extern "C" __declspec(noinline) void handleQuestLogInput(CMenuQuestLog* self) {
    CQuestLogPadData* pad = (CQuestLogPadData*)cf::CfGameManager::getCfPadData();
    int cancel, trigger3, trigger1, trigger2, confirm, menu, dir;
    // The bit positions differ between Classic Controller and Wiimote.
    if (isClassicController__Q22cf13CfGameManagerFv(-1) != 0) {
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
        QstLogList_ApplySortSelection(&self->mQstLogList);
        if (!QstLogList_IsSortEnabled(&self->mQstLogList)) {
            markReplayClose(&self->mTitleAHelp);
            QstLogList_EndSortMode(&self->mQstLogList, 0);
            self->mState = 4;
        } else {
            func_801C41E8(&self->mTitleAHelp, 0x3c);
        }
    } else if (trigger2) {
        if (QstLogList_IsSortDescending(&self->mQstLogList)) {
            QstLogList_ConfirmSort(&self->mQstLogList, 0);
            func_801C41E8(&self->mTitleAHelp, 0x3c);
        } else {
            beginClose(&self->mTitleAHelp);
            QstLogList_EndSortMode(&self->mQstLogList, 1);
            self->mState = 3;
        }
    } else if (cancel) {
        QstLogList_ScrollUp(&self->mQstLogList);
    } else if (dir) {
        QstLogList_ScrollDown(&self->mQstLogList);
    } else if (confirm) {
        QstLogList_PageUp(&self->mQstLogList);
    } else if (menu) {
        QstLogList_PageDown(&self->mQstLogList);
    } else if (trigger3) {
        QstLogList_ToggleSort(&self->mQstLogList);
        func_801C41E8(&self->mTitleAHelp, QstLogList_IsSortDescending(&self->mQstLogList) ? 0x3e : 0x3c);
    }
}

// Same gate as gateQuestLogList but toggles the +0x54 byte to 1.
extern "C" void flagQuestLogReady(CMenuQuestLog* self) {
    if (self->mTitleAHelp.isIdle() && QstLogList_IsSortEnabled(&self->mQstLogList))
        self->mField54 = 1;
}

extern "C" void selectQuestLogEntry(CMenuQuestLog* self)
{
    CMenuQuestLog* base = self;
    if (base->mTitleAHelp.isIdle() && QstLogList_IsSortEnabled(&base->mQstLogList)) {
        unsigned int value = QstLogList_GetSelectedQuestLo(&base->mQstLogList);
        QstLogInfo_SetQuestIdSlot(&base->mQstLogInfo, static_cast<unsigned short>(value));
        QstLogInfo_LoadCommonArchive(&base->mQstLogInfo);
        base->mState = 5;
    }
}

// ---------------------------------------------------------------------------
// ---- startQuestLogInfo (us-8011dbd8) ------------------------------------------
// When the quest log info is done, set the title-help mode to 0x3d, close the
// title help, finish the log info and advance to state 6.
// ---------------------------------------------------------------------------
extern "C" void startQuestLogInfo(CMenuQuestLog* self) {
    if (QstLogInfo_GetLoadedMark(&self->mQstLogInfo)) {
        func_801C41E8(&self->mTitleAHelp, 0x3d);
        reopenFromClose(&self->mTitleAHelp);
        QstLogInfo_Start(&self->mQstLogInfo);
        self->mState = 6;
    }
}

// ---------------------------------------------------------------------------
// ---- finishQuestLogInfo (us-8011dc34) ------------------------------------------
// When the title help is idle and the quest log info is done, advance to
// state 7.
// ---------------------------------------------------------------------------
extern "C" void finishQuestLogInfo(CMenuQuestLog* self) {
    if (self->mTitleAHelp.isIdle() && QstLogInfo_GetReadyMark(&self->mQstLogInfo)) {
        self->mState = 7;
    }
}

// ---------------------------------------------------------------------------
// ---- closeQuestLogMenu (us-8011dc84) ------------------------------------------
// State 7 handler: on a close-button press (classic bit 23 / wii bit 10),
// close the title help, finish the log info and advance to state 8.
// ---------------------------------------------------------------------------
extern "C" void closeQuestLogMenu(CMenuQuestLog* self) {
    CPad* pad = cf::CfGameManager::getCurrentPad();
    // Close-button press: classic-controller bits 22-23 vs wii bits 4-5,
    // normalized to bool (retail's rlwinm + subic/subfe shape).
    bool close;
    if (isClassicController__Q22cf13CfGameManagerFv(-1)) {
        close = pad->mPressedButtonFlags & 0x600000;
    } else {
        close = pad->mPressedButtonFlags & 0x30;
    }
    if (close) {
        markReplayClose(&self->mTitleAHelp);
        QstLogInfo_Resume(&self->mQstLogInfo);
        self->mState = 8;
    }
}

// ---------------------------------------------------------------------------
// ---- resortQuestLogList (us-8011dd08) ------------------------------------------
// When the title help is idle and the quest log info is done, set the
// title-help mode to 0x3c, close the title help, re-sort the list and advance
// to state 9.
// ---------------------------------------------------------------------------
extern "C" void resortQuestLogList(CMenuQuestLog* self) {
    if (self->mTitleAHelp.isIdle() && QstLogInfo_GetReadyMark(&self->mQstLogInfo)) {
        func_801C41E8(&self->mTitleAHelp, 0x3c);
        reopenFromClose(&self->mTitleAHelp);
        QstLogList_BeginSortOpen(&self->mQstLogList);
        self->mState = 9;
    }
}

// ---------------------------------------------------------------------------
// ---- reopenQuestLogList (us-8011dd74) ------------------------------------------
// When the title help is idle and sorting is enabled, advance to state 2.
// ---------------------------------------------------------------------------
extern "C" void reopenQuestLogList(CMenuQuestLog* self) {
    if (self->mTitleAHelp.isIdle() && QstLogList_IsSortEnabled(&self->mQstLogList)) {
        self->mState = 2;
    }
}

// Adjusting thunk: upcasts from a base sub-object (at offset +0x58 within CMenuQuestLog)
// to the full CMenuQuestLog, then tail-calls cbRenderBefore.
extern "C" void fwdQuestLogCbRender(IScnRender* self) {
    reinterpret_cast<CMenuQuestLog*>(reinterpret_cast<char*>(self) - 0x58)->cbRenderBefore();
}

// Adjusting thunk: upcasts from a base sub-object (at offset +0x58 within CMenuQuestLog)
// to the full CMenuQuestLog, then tail-calls the destructor.
extern "C" void fwdQuestLogDtor(IScnRender* self) {
    // Single-arg cast: retail thunk only adjusts this; mode stays in r4.
    ((void (*)(CMenuQuestLog*))__dt__13CMenuQuestLogFv)(
        reinterpret_cast<CMenuQuestLog*>(reinterpret_cast<char*>(self) - 0x58));
}

// __dt__8011D2F8: deleting destructor - free self when mode > 0, return self.
extern "C" CMenuQuestLog* __dt__8011D2F8(CMenuQuestLog* self, int mode) {
    if (self != 0 && mode > 0) {
        __dl__FPv(self);
    }
    return self;
}

// sbss singleton (retail 0x8: pointer + emitted zero word).
CMenuQuestLog* lbl_eu_80663FC0[2] = { 0, 0 };

// --- hand-built retail data (splits: .rodata 804FE508-804FE6F0,
// .data 8052CC10-8052CD50, .sdata 80661E70-80661E78) ---
// .rodata class-name string (referenced by the .sdata pair below).
__declspec(section ".rodata") __attribute__((used, aligned(8)))
char lbl_eu_804FE508[0x0E] = "CMenuQuestLog";
// .rodata menu-text pool (BdatTouchStringCell reads +0x0/+0xA in Init).
__declspec(section ".rodata") __attribute__((used, aligned(8)))
char lbl_eu_804FE518[0x10] = "MNU_quest\0name";
// .rodata marker-name table (each 0xF content+null, MWCC zero-pads to 0x10;
// referenced by the .data string-pointer table below).
__declspec(section ".rodata") __attribute__((used)) char lbl_eu_804FE528[0x10] = "markerlist0000";
__declspec(section ".rodata") __attribute__((used)) char lbl_eu_804FE538[0x10] = "markerlist0101";
__declspec(section ".rodata") __attribute__((used)) char lbl_eu_804FE548[0x10] = "markerlist0201";
__declspec(section ".rodata") __attribute__((used)) char lbl_eu_804FE558[0x10] = "markerlist0301";
__declspec(section ".rodata") __attribute__((used)) char lbl_eu_804FE568[0x10] = "markerlist0401";
__declspec(section ".rodata") __attribute__((used)) char lbl_eu_804FE578[0x10] = "markerlist0402";
__declspec(section ".rodata") __attribute__((used)) char lbl_eu_804FE588[0x10] = "markerlist0501";
__declspec(section ".rodata") __attribute__((used)) char lbl_eu_804FE598[0x10] = "markerlist0601";
__declspec(section ".rodata") __attribute__((used)) char lbl_eu_804FE5A8[0x10] = "markerlist0701";
__declspec(section ".rodata") __attribute__((used)) char lbl_eu_804FE5B8[0x10] = "markerlist0801";
__declspec(section ".rodata") __attribute__((used)) char lbl_eu_804FE5C8[0x10] = "markerlist0901";
__declspec(section ".rodata") __attribute__((used)) char lbl_eu_804FE5D8[0x10] = "markerlist1001";
__declspec(section ".rodata") __attribute__((used)) char lbl_eu_804FE5E8[0x10] = "markerlist1101";
__declspec(section ".rodata") __attribute__((used)) char lbl_eu_804FE5F8[0x10] = "markerlist1201";
__declspec(section ".rodata") __attribute__((used)) char lbl_eu_804FE608[0x10] = "markerlist1202";
__declspec(section ".rodata") __attribute__((used)) char lbl_eu_804FE618[0x10] = "markerlist1301";
__declspec(section ".rodata") __attribute__((used)) char lbl_eu_804FE628[0x10] = "markerlist1401";
__declspec(section ".rodata") __attribute__((used)) char lbl_eu_804FE638[0x10] = "markerlist1501";
__declspec(section ".rodata") __attribute__((used)) char lbl_eu_804FE648[0x10] = "markerlist1601";
__declspec(section ".rodata") __attribute__((used)) char lbl_eu_804FE658[0x10] = "markerlist1701";
__declspec(section ".rodata") __attribute__((used)) char lbl_eu_804FE668[0x10] = "markerlist1801";
__declspec(section ".rodata") __attribute__((used)) char lbl_eu_804FE678[0x10] = "markerlist1901";
__declspec(section ".rodata") __attribute__((used)) char lbl_eu_804FE688[0x10] = "markerlist2001";
__declspec(section ".rodata") __attribute__((used)) char lbl_eu_804FE698[0x10] = "markerlist2101";
__declspec(section ".rodata") __attribute__((used)) char lbl_eu_804FE6A8[0x10] = "markerlist2201";
__declspec(section ".rodata") __attribute__((used)) char lbl_eu_804FE6B8[0x10] = "markerlist2301";
__declspec(section ".rodata") __attribute__((used)) char lbl_eu_804FE6C8[0x10] = "markerlist2401";
// .rodata float table (read cross-TU by CMenuSymbolMark as f32s).
__declspec(section ".rodata") __attribute__((used, aligned(8)))
extern const float lbl_eu_804FE6D8[6] = { -0.2f, 1.81f, -260.26f, -0.18f, 1.76f, -338.3f };

// .data string-pointer table (28 marker addresses + 8 zero words) and the
// resource table below need the retail names visible here.
extern "C" char lbl_eu_804FE508[];
extern "C" char lbl_eu_804FE528[];
extern "C" char lbl_eu_804FE538[];
extern "C" char lbl_eu_804FE548[];
extern "C" char lbl_eu_804FE558[];
extern "C" char lbl_eu_804FE568[];
extern "C" char lbl_eu_804FE578[];
extern "C" char lbl_eu_804FE588[];
extern "C" char lbl_eu_804FE598[];
extern "C" char lbl_eu_804FE5A8[];
extern "C" char lbl_eu_804FE5B8[];
extern "C" char lbl_eu_804FE5C8[];
extern "C" char lbl_eu_804FE5D8[];
extern "C" char lbl_eu_804FE5E8[];
extern "C" char lbl_eu_804FE5F8[];
extern "C" char lbl_eu_804FE608[];
extern "C" char lbl_eu_804FE618[];
extern "C" char lbl_eu_804FE628[];
extern "C" char lbl_eu_804FE638[];
extern "C" char lbl_eu_804FE648[];
extern "C" char lbl_eu_804FE658[];
extern "C" char lbl_eu_804FE668[];
extern "C" char lbl_eu_804FE678[];
extern "C" char lbl_eu_804FE688[];
extern "C" char lbl_eu_804FE698[];
extern "C" char lbl_eu_804FE6A8[];
extern "C" char lbl_eu_804FE6B8[];
extern "C" char lbl_eu_804FE6C8[];
// Resource-table targets live in other TUs (sdata); address-taken only.
extern "C" const u8 lbl_eu_80661940[];
extern "C" const u8 lbl_eu_80661958[];
extern "C" const u8 lbl_eu_80661950[];
extern "C" const u8 lbl_eu_80661948[];
extern "C" const u8 lbl_eu_80661DE8[];
extern "C" const u8 lbl_eu_80661DE0[];

// Free-function spellings of vtable slots for the hand-built CC38 table
// below (same recipe as CScrollBar/CMenuEnemyState): these declare the exact
// retail (link) symbols; the members defined in this TU provide the
// CMenuQuestLog ones, other TUs provide Reset/Draw/Tail.
extern "C" void Reset__14CChildListNodeFv(void);
extern "C" void Init__13CMenuQuestLogFv(void);
extern "C" void Term__13CMenuQuestLogFv(void);
extern "C" void Move__13CMenuQuestLogFv(void);
extern "C" void Tail__8CProcessFv(void);
extern "C" void cbRenderBefore__13CMenuQuestLogFv(void);
// .sdata RTTI pair (defined below); referenced by the vtable.
extern void* lbl_eu_80661E70[];

// .data composite vtable (retail lbl_eu_8052CC38, 14 words): primary vtable
// (RTTI, null, dtor, Reset, Init, Term, Move, Draw, Tail) + IScnRender
// secondary vtable (RTTI, -0x58 offset-to-top, dtor/render forwards,
// cbRenderBefore). The Draw__14CTTask<5IUICf>Fv slot has no C++ spelling
// (angle brackets); it stays a zero placeholder like CMenuEnemyState's
// vtable (reloc names are gate-ignored; link carries 0 there).
__declspec(section ".data") __attribute__((used, aligned(8)))
void* lbl_eu_8052CC38[14] = {
    lbl_eu_80661E70, 0, (void*)__dt__13CMenuQuestLogFv,
    (void*)Reset__14CChildListNodeFv, (void*)Init__13CMenuQuestLogFv,
    (void*)Term__13CMenuQuestLogFv, (void*)Move__13CMenuQuestLogFv,
    0, (void*)Tail__8CProcessFv,
    lbl_eu_80661E70, (void*)0xFFFFFFA8,
    (void*)fwdQuestLogDtor, (void*)fwdQuestLogCbRender,
    (void*)cbRenderBefore__13CMenuQuestLogFv,
};
// .data resource table (retail 0x8052CC70-0x8052CCA7, no retail label):
// {sdata pointer, count} pairs (first count 88, rest 0, last pair zero).
struct QuestLogResEntry { const void* ptr; s32 count; };
__declspec(section ".data") __attribute__((used, aligned(4)))
struct QuestLogResEntry questlog_res_table[7] = {
    { lbl_eu_80661940, 88 }, { lbl_eu_80661958, 0 }, { lbl_eu_80661950, 0 },
    { lbl_eu_80661948, 0 }, { lbl_eu_80661DE8, 0 }, { lbl_eu_80661DE0, 0 },
    { 0, 0 },
};
// .data marker-pointer table (retail lbl_eu_8052CCA8, 28 addresses + zero fill).
struct QuestLogStrTable { const char* p[28]; u32 reserved[8]; };
__declspec(section ".data") __attribute__((used, aligned(4)))
struct QuestLogStrTable lbl_eu_8052CCA8 = {
    {
        lbl_eu_804FE528, lbl_eu_804FE538, lbl_eu_804FE548, lbl_eu_804FE558,
        lbl_eu_804FE568, lbl_eu_804FE578, lbl_eu_804FE588, lbl_eu_804FE598,
        lbl_eu_804FE5A8, lbl_eu_804FE5B8, lbl_eu_804FE5C8, lbl_eu_804FE5D8,
        lbl_eu_804FE5E8, lbl_eu_804FE5F8, lbl_eu_804FE608, lbl_eu_804FE618,
        lbl_eu_804FE628, lbl_eu_804FE638, lbl_eu_804FE648, lbl_eu_804FE648,
        lbl_eu_804FE658, lbl_eu_804FE668, lbl_eu_804FE678, lbl_eu_804FE688,
        lbl_eu_804FE698, lbl_eu_804FE6A8, lbl_eu_804FE6B8, lbl_eu_804FE6C8,
    },
    { 0 },
};
// .data float table (retail lbl_eu_8052CD38).
__declspec(section ".data") __attribute__((used, aligned(4)))
extern const float lbl_eu_8052CD38[6] = { 1.0f, 1.0f, 1.0f, 0.9f, 0.7f, 0.9f };
// .sdata RTTI pair (retail lbl_eu_80661E70): class-name string + resource table.
#pragma section data_type ".sdata" ".sdata"
extern __declspec(section ".sdata") void* lbl_eu_80661E70[2] = {
    lbl_eu_804FE508, questlog_res_table,
};
#pragma section data_type ".data" ".data"
