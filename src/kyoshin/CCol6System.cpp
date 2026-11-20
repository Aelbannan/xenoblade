#define IWORK_EVENT_INLINE_DTOR
#define IUIWINDOW_INLINE_DTOR
#define CTTASK_INLINE_DTOR
// Auto-scaffolded catalog TU for kyoshin/CCol6System
// Cleaned-up C++ for CCol6CheckBat; other stubs pending decomp.

#include "kyoshin/cf/CfBdat.hpp"
#include "kyoshin/cf/CfGameManager.hpp"
#include "kyoshin/cf/CItem.hpp"  // canonical CItemImpl instances accessor
#include "kyoshin/CCol6CheckBat.hpp"
#include "kyoshin/CSysWin.hpp"
#include "kyoshin/CScrollBar.hpp"
#include "kyoshin/cf/CfSoundMan.hpp"
#include "kyoshin/CTaskGame.hpp"
#include "kyoshin/code_80135FDC_Lyt.hpp"
// Forward-declare cf::CfGameManager before the include chain pulls in
// CfGameManager.hpp (its UnkClass_8007DAE0::init declaration references the
// type before the namespace block that declares it).
namespace cf {
class CfGameManager;
}
#include "kyoshin/CCol6System.hpp"
#include "kyoshin/harness_catalog.hpp"
#include "monolib/device/CDeviceFile.hpp"

#include <string.h>
#include "monolib/scn/code_804BC9EC.hpp"
#include "kyoshin/plugin/ocBdat.hpp"

// CItem_initItemImplInstances uses its canonical typed decl from
// include/kyoshin/cf/CItem.hpp; the item pointer is cast at the call site.

// Resolve ml::FixStr<128>::format calls to the explicit specialization that
// CfScript.cpp defines (retail symbol format__Q22ml10FixStr<128>FPCce); a
// plain generic-template call would mangle to ...11FixStr... instead (reloc
// name drift at the call site).
template <> void ml::FixStr<128>::format(const char* fmt, ...);

extern "C" void __dt__8CProcessFv(void* self, int flags);
extern "C" void __dt__10CScrollBarFv(void* self, int flags);

// Forward decls for the layout-build helpers defined below (extern "C" so the
// call sites in Col6_HintOnFile_DB08 / Col6_SysOnFile_0118 bind to the unmangled retail
// symbols). Col6_HintOnFile_DB08 is the CCol6Hint file-load completion handler (its
// layout build col6HintBuild operates on hint offsets 0x9C/0x11C/0x124).
extern "C" void col6HintBuild(CCol6Hint* self);
extern "C" void col6SysLayout(CCol6System* self);
extern "C" void col6FillSlots(CCol6System* self, s32 playerIdx);
// Item-box input handlers defined later in this TU (called by Move).
extern "C" void col6PadHandle(CCol6System* self);
extern "C" void col6SwitchBox(CCol6System* self);
extern "C" void col6WinScroll(CCol6System* self);
extern "C" void Col6_SyncBoxSlot_0EE4(u32 unused, s32 arg);
extern "C" void col6DoOpenBox(CCol6System* self);
extern "C" void Col6Win2_2C40(CCol6System* self);
extern "C" void Col6_OpenBox_2DB4(CCol6System* self);
extern "C" void col6Win2Count(CCol6System* self);
extern "C" void Col6_Win2CountLadder_3614(CCol6System* self);
extern "C" void Col6_Win2Interact_378C(CCol6System* self);
extern "C" void col6Win2PerPl(CCol6System* self);
extern "C" void col6JoinCheck(CCol6System* self);
extern "C" void col6HintInput(CCol6Hint* self);

// Singleton instance (retail: sCol6CheckBatInstance).
CCol6CheckBat* gCol6CheckBat;

int sCol6CheckBatInstance;
int sCol6SystemInstance;
int sCol6InviteInstance;
int sCol6QueuedWindowId;

char* sCol6ArcPath; // .sdata arc-path pointer

void Col6_ClearGlobals_D0B8() {
    sCol6CheckBatInstance = 0;
    sCol6SystemInstance = 0;
    sCol6InviteInstance = 0;
    sCol6QueuedWindowId = -1;
}

// CCol6CheckBat::~CCol6CheckBat()
// extern "C" free-function form (CSysWin/CMenuQstCnt precedent): the retail
// dtor's outer null-check covers the delete, and the nested double null-check
// is the documented D2-inlined-into-D1 MWCC artifact (same as
// ~CSimpleEveTalkWin / ~CSystemWindow) guarding the CProcess base dtor.
// NOTE: default -O4,p (NOT optimize_for_size) - this retail keeps the
// separate stw r31/stw r30 saves instead of the stmw pair.
extern "C" void* __dt__13CCol6CheckBatFv(CCol6CheckBat* self, int flags) {
    if (self != 0) {
        if (self != 0) {
            if (self != 0) {
                __dt__8CProcessFv(reinterpret_cast<CProcess*>(self), 0);
            }
        }
        if (flags > 0) {
            operator delete(self);
        }
    }
    return self;
}

// CCol6CheckBat::Init() - no-op; override satisfies CProcess pure-virtual.
void CCol6CheckBat::Init() {}

// CCol6CheckBat::Term() - clears singleton pointer.
void CCol6CheckBat::Term() {
    *(CCol6CheckBat**)&sCol6CheckBatInstance = nullptr;
}

// CCol6CheckBat::Move() - once the game is running, shows the two check-bat
// messages (indices 0x7f/0x80 of the shared string pool) and opens the queued
// window (sCol6QueuedWindowId + 0x100) when a window id is queued; either way the
// bat ends up flagged done at +0x64.
void CCol6CheckBat::Move() {
    if (IsMenuState621F0() == 0) {
        return;
    }

    if (sCol6QueuedWindowId >= 0) {
        CUICfManager_setMessageWindowVisible(1);
        char* msg = BdatTouchStringCell(lbl_eu_80502050, &lbl_eu_80502050[9], 0x7f);
        UIWin_CreateSysWin0(msg, 0, 0);
        msg = BdatTouchStringCell(lbl_eu_80502050, &lbl_eu_80502050[9], 0x80);
        UIWin_CreateSysWin0(msg, 0, 0);

        UIWin_CreateQuestWin(sCol6QueuedWindowId + 0x100, 0, 0);
        sCol6QueuedWindowId = -1;
        CUICfManager_setMessageWindowVisible(0);

        // Retail keeps a copy of this store in BOTH arms (the b-over-else
        // emits the extra branch); do not hoist it out of the if/else.
        mField64 = 1;
    } else {
        mField64 = 1;
    }
}


// CCol6CheckBat ctor - self-allocating factory (retail symbol
// __ct__CCol6CheckBat). Returns NULL if the singleton already exists;
// otherwise allocates 0x74 bytes from work memory, constructs the CProcess
// base, fills the callback/field block, stores itself as the singleton and
// registers as a child of `parent`. Mirrors retail control flow: the
// singleton store + Regist run even when the allocation failed (obj stays
// NULL) and the (possibly NULL) object is returned.
extern "C" CCol6CheckBat* __ct__CCol6CheckBat(CProcess* parent) {
    if (sCol6CheckBatInstance != 0) {
        return 0;
    }

    CCol6CheckBat* obj = reinterpret_cast<CCol6CheckBat*>(
        mtl::MemManager::allocate(0x74, CWorkThreadSystem::getWorkMem()));

    new (obj) CCol6CheckBat(parent);

    sCol6CheckBatInstance = (int)(uintptr_t)obj;
    ((CProcess*)obj)->Regist(parent, false);
    return (CCol6CheckBat*)(uintptr_t)sCol6CheckBatInstance;
}

CCol6CheckBat::CCol6CheckBat(CProcess* parent) {
    mFlag70 = 0;
}

int Col6_ScanItemSlots_D310() {
    union {
        struct {
            u32 w;
            u8 b;
        };
        u8 bytes[5];
    } data;
    data.w = lbl_eu_80667540;
    data.b = lbl_eu_80667544;

    u8 result = CtrlRemote_TouchBitByArg(0x7fc);

    for (u32 i = 0; i < 5; i++) {
        u8 idx = (u8)i;
        if (result >= data.bytes[idx] && CtrlRemote_TouchBitByArg(idx + 0x804) == 0) {
            return 1;
        }
    }
    return 0;
}

// Col6_ReserveBoxSlot_D3A0 - reserve the first free item-box slot whose owned-count
// threshold is met: for each of 5 slots, if the shared counter (id 0x7fc)
// covers the slot's threshold byte and the slot count (id 0x804+i) is zero,
// queue the window index and increment both counters by one/threshold delta.
void Col6_ReserveBoxSlot_D3A0() {
    // Retail stack layout: t2 (7548/754C) at +0x10 (r31), t1 (7550/7554) at
    // +0x08 (r30); declaring t2 first gives it the higher address.
    union {
        struct {
            u32 w;
            u8 b;
        };
        u8 bytes[5];
    } t2, t1;
    t2.w = lbl_eu_80667548;
    t2.b = lbl_eu_8066754C;
    t1.w = lbl_eu_80667550;
    t1.b = lbl_eu_80667554;

    // Retail keeps the running count as a 16-bit variable (re-extended via
    // extsh on every read).
    s16 total = CtrlRemote_TouchBitByArg(0x7fc);
    for (s32 i = 0; i < 5; i++) {
        u8 need = t2.bytes[i];
        if (total >= need && CtrlRemote_TouchBitByArg(i + 0x804) == 0) {
            sCol6QueuedWindowId = i;
            CtrlRemote_SetSharedBit(i + 0x804, 1);
            // Retail sign-extends the t1 byte before adding to the running
            // total, then re-sign-extends the halfword argument.
            total += (s8)t1.bytes[i];
            CtrlRemote_SetSharedBit(0x7fc, total);
        }
    }
}


// CCol6Hint::CCol6Hint - real member ctor. The IUIWindow/CTTask/IWorkEvent/
// IScnRender base ctors emit the primary, +0x6C and +0x70 vptr stores and the
// null PTMF slots; the body only initializes this class's own members.
CCol6Hint::CCol6Hint(CProcess* parent)
    : mParentPtr(parent), mField88(0), mField90(0), mField94(0), mFlag98(0),
      mState(0), mField11C(0), mField120(0), mField121(0), mCur18(0),
      mScrollBar(0) {
    memset(mUnk9C, 0, 0x80);
}

// CCol6Hint dtor (extern "C" free-function form, default -O4,p - retail keeps
// separate stw r31/stw r30 saves): sub-object dtors in +0x13C (CScrollBar),
// +0x124 (CCur18), +0x74 (UnkClass_8045F564) order with flags -1, then the
// CProcess base dtor behind the double-null guard (retail re-checks r30 and
// emits two beq's - the D2-inlined-into-D1 artifact), then flags-based delete.
extern "C" void* __dt__9CCol6HintFv(CCol6Hint* self, int flags) {
    if (self != 0) {
        __dt__10CScrollBarFv(&self->mScrollBar, -1);
        __dt__6CCur18Fv(&self->mCur18, -1);
        __dt__17UnkClass_8045F564Fv(&self->mMemRegion, -1);
        if (self != 0) {
            if (self != 0) {
                __dt__8CProcessFv(reinterpret_cast<CProcess*>(self), 0);
            }
        }
        if (flags > 0) {
            operator delete(self);
        }
    }
    return self;
}

// CCol6Hint::Init() - load the hint arc file through the embedded IWorkEvent
// interface (the +0x6C subobject; null-this-safe), then install the scroll bar
// by constructing a temp (direction 3), memberwise-copying everything except
// the vptr into the embedded bar, and kicking off the arc read.
void CCol6Hint::Init() {
    // Null-this guard: MWCC keeps r31 = this and conditionally adds the
    // subobject offset, so mirror that single-branch shape here.
    IWorkEvent* evt = reinterpret_cast<IWorkEvent*>(this);
    if (this != 0) {
        evt = &static_cast<IWorkEvent&>(*this);
    }
    mField88 = CDeviceFile::readFile(
        mtl::MemManager::getHandleMEM2(), sCol6ArcPath, evt, 0, 0);

    mScrollBar = CScrollBar(3);

    mScrollBar.loadLayoutArc();
}

void CCol6Hint::Term() {
    closeFileHandle(&mField88);

    // The `if (this)` is the MWCC idiom that splits mr r4 / beq / addi +0x70
    // for the IScnRender subobject passed to removeRenderCB.
    IScnRender* render = reinterpret_cast<IScnRender*>(this);
    if (this != 0) {
        render = &static_cast<IScnRender&>(*this);
    }
    reinterpret_cast<CScn*>(mParentPtr)->removeRenderCB(render);

    mFlag98 = 0;
    mScrollBar.Teardown();
    mCur18.cleanup();

    // Release the anim host through its virtual deleting destructor; MWCC's
    // delete expansion supplies the second null check retail shows here.
    if (mLayout != 0) {
        delete mLayout;
        mLayout = 0;
    }

    releaseArcResourceAccessor(reinterpret_cast<nw4r::lyt::ArcResourceAccessor*>(mField8C));
    mMemRegion.deleteRegion();
    sCol6SystemInstance = 0;
}

// CCol6Hint::Move() - hint-bubble state machine (states 0-5) driving the
// layout anim host (mField54), the scroll bar and the cursor:
//   0 start, 1 slide-in, 2 show label, 3 handoff, 4 slide-out, 5 pick target.
// Every state falls through to the common tail (anim-host update, cursor
// update, scroll-bar update).
void CCol6Hint::Move() {
    // Three-term short-circuit gate: retail emits beq/beq for the first two
    // conditions and an inverted bne-body / b-over-exit pair for the last.
    if (IsMenuState621F0() == 0 || mFlag98 == 0 ||
        mScrollBar.isVisible() == 0) {
        return;
    }
    switch (mState) {
        case 0:
            mState = 1;
            playUISound(0x6d);
            break;

        case 1:
            // Wait for the out anim to finish, then swap both anims and
            // kick the scroll bar in (open sound).
            if (advanceAnimTransform(mField90, lbl_eu_80667558) == 0) break;
            mLayout->SetAnimationEnable(mField90, 0);
            mLayout->SetAnimationEnable(mField94, 1);
            mState = 2;
            {
                ml::CVec3 vec3(lbl_eu_8066755C, lbl_eu_80667560, lbl_eu_80667564);
                mScrollBar.InitRootPane(reinterpret_cast<const nw4r::math::VEC3*>(&vec3));
            }
            mScrollBar.UpdateThumb(5, (u16)mField11C);
            // Retail issues requestScrollIn twice in a row (two consecutive
            // bls at +0x0124/+0x012c); not a cleanup candidate.
            mScrollBar.requestScrollIn();
            mScrollBar.requestScrollIn();
            break;

        case 2:
            // Scroll bar fully in: label the pane with the player index and
            // position the cursor between the label panes.
            if (advanceAnimTransform(mField94, lbl_eu_80667558) == 0) break;
            if (mScrollBar.isActive() == 0) break;
            mState = 3;
            mCur18.SetVisible(1);
            {
                char buf[0x20];
                sprintf(buf, &lbl_eu_80502050[0xe], (s8)mField120 + 1);
                nw4r::lyt::Pane* pane1 =
                    mLayout->GetRootPane()->FindPaneByName(buf, true);
                nw4r::lyt::Pane* pane2 =
                    mLayout->GetRootPane()->FindPaneByName(&lbl_eu_80502050[0x17], true);
                nw4r::math::VEC3 vec;
                ScalePaneWorldPos(&vec, pane1, pane2, mLayout->GetRootPane());
                mCur18.setRootPaneTranslate(&vec);
            }
            break;

        case 3:
            col6HintInput(this);
            break;

        case 4:
            // Scroll bar sliding out: swap the anims back and wait for done.
            if (AnimRewindFrame(mField94, lbl_eu_80667558) == 0) break;
            if (mScrollBar.isActive() == 0) break;
            mLayout->SetAnimationEnable(mField94, 0);
            mLayout->SetAnimationEnable(mField90, 1);
            mState = 5;
            break;

        case 5:
            // Out anim done: resolve the hint target for the current player
            // pair (mField120 + mField121) and post the index.
            if (AnimRewindFrame(mField90, lbl_eu_80667558) == 0) break;
            {
                s32 idx = (s8)mField120 + (s8)mField121;
                u32 entry = mUnk9C[idx];
                if (entry != 0) {
                    mField60 = (u8)BdatGetU8ByTableKey(
                        &lbl_eu_80502050[0x26], &lbl_eu_80502050[0x33], entry);
                } else {
                    mField60 = 0xff;
                }
            }
            mField64 = 1;
            break;

        default:
            break;
        }

        // Common tail: per-frame updates.
        mLayout->Animate(0);
        mCur18.Update();
        mScrollBar.UpdateDispatch();
}

// Col6_HintOnFile_DB08 - CCol6Hint file-load completion handler: create the scratch
// region + host, detach the arc buffer into a fresh accessor, build the hint
// layout (col6HintBuild), flag it ready and register the render callback with
// the parent scene (the +0x84 slot holds the stored parent pointer).
extern "C" int Col6_HintOnFile_DB08(CCol6Hint* self, CEventFile* event) {
    // Retail branches to a late failure label (bne) rather than skipping the
    // body with a forward beq.
    if (self->mField88 != event->mFileHandle) goto fail;

    // Host's scope ends before the failure label so the fail path skips the
    // Class_8045F858 destructor (retail has no dtor call on that path).
    {
        // Retail holds three values in callee-saved regs across the body
        // (self r29, detached buffer r30, string-pool base r31); name them
        // as locals. The pool base is materialized after getHandleMEM2.
        u8* data;

        mtl::ALLOC_HANDLE mem = mtl::MemManager::getHandleMEM2();
        char* pool = lbl_eu_80502050;
        self->mMemRegion.createRegion(mem, 0x20000, &pool[0x38], 0);
        Class_8045F858 host(&self->mMemRegion);

        CFileHandle* handle = self->mField88;
        data = handle->mData;
        handle->mData = 0;
        mtl::MemManager::setMemInitFlag(false);

        self->mField8C =
            (u32)(uintptr_t)CLibLayout::createArcResourceAccessor();
        reinterpret_cast<nw4r::lyt::ArcResourceAccessor*>(self->mField8C)
            ->Attach(data, &pool[0x42]);
        col6HintBuild(self);
        self->mFlag98 = 1;

        IScnRender* render = reinterpret_cast<IScnRender*>(self);
        if (self != 0) {
            render = &static_cast<IScnRender&>(*self);
        }
        reinterpret_cast<CScn*>(self->mParentPtr)->addRenderCB(render, 0xd, 0);

        self->mField88 = 0;
        self->mMemRegion.validateHeap();
    }
    return 1;
fail:
    return 0;
}

// CCol6Hint::cbRenderBefore() - draw the hint layout, scroll bar and cursor
// through a stack DrawInfo. The gate chain mirrors retail: task-game ready /
// mode bit 0x400000 / game running / arc loaded / bar visible; the goto-body
// form emits the bne-end + beq-body/b-end pair (CSysWinSelect precedent).
void CCol6Hint::cbRenderBefore() {
    // First two gates use the if-&&-goto-body / goto-end chain (bne end +
    // beq body/b end); the running/loaded checks are early returns; the
    // visibility check positively wraps the draw block.
    if (CTaskGame::getInstance()->isFlag01Set() == 0 &&
        (gmPauseFlags & 0x200000) == 0) {
        goto body;
    }
    goto end;
end:
    return;
body:
    // Splitting running/loaded checks from the visibility check keeps the
    // visibility branch as bne-draw / b-over-return (a flat ||-chain would
    // collapse it to a single beq like the other gates).
    if (IsMenuState621F0() != 0 && mFlag98 != 0) {
        if (mScrollBar.isVisible() != 0) {
            goto draw;
        }
    }
    return;
draw:
    GXSetZMode(GX_FALSE, GX_NEVER, GX_FALSE);

    nw4r::lyt::DrawInfo drawInfo;
    SetWideLocAdjust(&drawInfo);
    drawLayout(reinterpret_cast<nw4r::lyt::Layout*>(mLayout),
                  &drawInfo, 0, 1);
    mScrollBar.draw(&drawInfo);
    mCur18.DrawLayout(&drawInfo);
}

// IUIWindow return type for the window factories below (declared extern "C"
// in CUIWindowManager.hpp).
class IUIWindow;

// col6HintAlloc - hint-window factory: allocate 0x17c bytes of work memory,
// construct the CCol6Hint into it and register it under pParent. The store
// to the singleton global and Regist run even when allocation failed (obj
// stays NULL); returns the (possibly NULL) singleton.
extern "C" IUIWindow* col6HintAlloc(CProcess* pParent, CScn* pScene) {
    if (sCol6SystemInstance != 0) {
        return 0;
    }

    CCol6Hint* obj = reinterpret_cast<CCol6Hint*>(
        mtl::MemManager::allocate(0x17c, CWorkThreadSystem::getWorkMem()));
    // Reassign the (self-returning) ctor result so MWCC keeps obj in r3
    // across the call instead of spilling it to a callee-saved register.
    obj = new (obj) CCol6Hint(reinterpret_cast<CProcess*>(pScene));
    sCol6SystemInstance = (int)(uintptr_t)obj;
    reinterpret_cast<CProcess*>((uintptr_t)sCol6SystemInstance)->Regist(pParent, false);
    return reinterpret_cast<IUIWindow*>((uintptr_t)sCol6SystemInstance);
}

// col6HintBuild - builds the CCol6Hint layout: attaches the arc, loads the two
// anim transforms, binds the device font into the root pane, rebuilds the
// embedded cursor from a stack temp, labels the count panes, then scans the
// item-box threshold tables to fill the per-slot entries (mUnk9C) and label
// the six slots with the resolved strings.
extern "C" void col6HintBuild(CCol6Hint* self) {
    // Build the layout into the +0x54 slot and load the two anim transforms
    // (the +0x54 anim host IS the layout object).
    buildLayout(reinterpret_cast<nw4r::lyt::Layout**>(&self->mLayout), reinterpret_cast<nw4r::lyt::ArcResourceAccessor*>(self->mField8C), (char*)"mf35_rv00.brlyt");
    bindLayoutAnimTransform(self->mLayout, &self->mField90, reinterpret_cast<nw4r::lyt::ArcResourceAccessor*>(self->mField8C), (char*)"mf35_rv00_in.brlan");
    bindLayoutAnimTransform(self->mLayout, &self->mField94, reinterpret_cast<nw4r::lyt::ArcResourceAccessor*>(self->mField8C), (char*)"mf35_rv00_info_in.brlan");

    // Bind the device font into the layout's root pane.
    nw4r::lyt::Pane* rootPane = self->mLayout->GetRootPane();
    CDeviceFontInfo* fontObj = getFontInfo__11CDeviceFontFUlPQ34nw4r3lyt6Layout(
        1, self->mLayout);
    ApplyFontToTxtPanes(rootPane,
                  fontObj->getFont());

    // Start the two anims (setAnim at +0x2C) and tick once (+0x38).
    self->mLayout->SetAnimationEnable(self->mField94, 0);
    self->mLayout->SetAnimationEnable(self->mField90, 1);
    self->mLayout->Animate(0);

    // Rebuild the embedded cursor from a stack temp (data fields only).
    self->mCur18 = CCur18(CUICfManager_getArcResourceAccessor());
    self->mCur18.initLayout();

    // Label the two count panes.
    char* s1 = BdatTouchStringCell((char*)"MNU_col6", (char*)"name", 0x1c);
    LayoutSetTextBoxFmtValue(self->mLayout,
                  (char*)"txt_title", s1, 0);
    char* s2 = BdatTouchStringCell((char*)"MNU_col6", (char*)"name", 0xb);
    LayoutSetTextBoxFmtValue(self->mLayout,
                  (char*)"txt_help", s2, 0);

    // Label the item pane and clear its visibility flag.
    nw4r::lyt::Pane* pane = self->mLayout->GetRootPane()->FindPaneByName((char*)"nul_inf01", true);
    pane->SetVisible(false);

    Bdat_GetTable_AA34();
    void* fp = getFP((char*)"CL6_hintlist");
    Bdat_GetMaxRow_B1EC(fp);

    // Two 7-byte threshold tables (u32 + u16 + u8, read as byte arrays).
    struct {
        u32 w;
        u16 h;
        u8 b;
    } tbl1, tbl2;
    tbl1.w = lbl_eu_80667568;
    tbl1.h = lbl_eu_8066756C;
    tbl1.b = lbl_eu_8066756E;
    tbl2.w = lbl_eu_80667570;
    tbl2.h = lbl_eu_80667574;
    tbl2.b = lbl_eu_80667576;

    self->mField11C = 0;

    // For each of the 7 count buckets, find the largest value whose owned
    // count fits the bucket, storing it into the entry table.
    for (u32 i = 0; i < 7; i++) {
        u8 val1 = ((u8*)&tbl1)[(u8)i];
        u32 rid = (u8)i + 0x7fc;
        u8 val2 = ((u8*)&tbl2)[(u8)i];
        while (val1 >= val2) {
            s32 c = BdatGetU8Direct(fp, (char*)"flag", val1);
            if ((c & 0xff) <= (s32)CtrlRemote_TouchBitByArg(rid)) {
                self->mUnk9C[self->mField11C] = val1;
                self->mField11C++;
                break;
            }
            val1--;
        }
    }

    // Reserved-slot entries (0x804 count == 1).
    for (u32 i = 0; i < 5; i++) {
        if (CtrlRemote_TouchBitByArg((u8)i + 0x804) == 1) {
            self->mUnk9C[self->mField11C] = i + 0x2e;
            self->mField11C++;
        }
    }

    self->mField11C++;

    // Label the six slots: entry table row = i + mField121; an empty row uses
    // the fallback string when the previous row is populated, else no text.
    for (s32 i = 0; i < 6; i++) {
        char buf[0x20];
        sprintf(buf, (char*)"txt_%02d", i + 1);
        char* str = 0;
        u32 idx = i + (s8)self->mField121;
        s32 entry = (s32)self->mUnk9C[idx];
        if (entry != 0) {
            str = BdatTouchStringCell((char*)"CL6_hintlist",
                                (char*)"title", entry);
        } else if (self->mUnk9C[idx - 1] != 0) {
            str = BdatTouchStringCell((char*)"MNU_col6", (char*)"name", 5);
        }
        LayoutSetTextBoxFmtValue(self->mLayout,
                      buf, str, 0);
    }
}

// col6HintInput - CCol6Hint::Move state-3 pad handler: held up/down switch the
// state and reposition the scroll position; the four short-press buttons
// scroll the hint selection with wrap (single row down/up via the row pair,
// page down/up via the page pair), re-labelling the six slot panes and
// repositioning the cursor each time.
extern "C" void col6HintInput(CCol6Hint* self) {
    CCol6PadData* pad = (CCol6PadData*)getCfPadData__Q22cf13CfGameManagerFv();
    int sels, confirm, up2, down2, up, down;
    if (cf::CfGameManager::isClassicController(-1) != 0) {
        up      = (pad->mField04 >> 21) & 1;
        sels    = (pad->mField104 & 0x8004) != 0;
        confirm = (pad->mField104 & 0x2001) != 0;
        up2     = (pad->mField104 & 0x4002) != 0;
        down2   = ((pad->mField104 & 0x8000) | (pad->mField104 & 0x10000000)) != 0;
        down    = (pad->mField04 >> 22) & 1;
    } else {
        up      = (pad->mField04 >> 4) & 1;
        sels    = (pad->mField104 & 0x8004) != 0;
        confirm = (pad->mField104 & 0x2001) != 0;
        up2     = (pad->mField104 & 0x4002) != 0;
        down2   = ((pad->mField104 & 0x8000) | (pad->mField104 & 0x10000000)) != 0;
        down    = (pad->mField04 >> 5) & 1;
    }

    if (up) {
        self->mState = 4;
        self->mCur18.SetVisible(0);
        self->mScrollBar.requestScrollOut();
        playUISound(3);
    } else if (down) {
        self->mState = 4;
        self->mCur18.SetVisible(0);
        self->mScrollBar.requestScrollOut();
        if (self->mField11C >= 5) {
            self->mField120 = 4;
            self->mField121 = (u8)(self->mField11C - 5);
        } else {
            self->mField120 = (u8)(self->mField11C - 1);
            self->mField121 = 0;
        }
        playUISound(6);
    } else if (sels) {
        // Row down with wrap: decrement the row pair, wrapping to the
        // (mField11C-based) page end when both go negative.
        self->mField120 = (u8)(self->mField120 - 1);
        if ((s8)self->mField120 < 0) {
            self->mField120 = 0;
            self->mField121 = (u8)(self->mField121 - 1);
            if ((s8)self->mField121 < 0) {
                if (self->mField11C >= 5) {
                    self->mField120 = 4;
                    self->mField121 = (u8)(self->mField11C - 5);
                } else {
                    self->mField120 = (u8)(self->mField11C - 1);
                    self->mField121 = 0;
                }
            }
        }
        // Label the six slots and reposition the cursor.
        for (s32 i = 0; i < 6; i++) {
            char buf[0x20];
            sprintf(buf, (char*)"txt_%02d", i + 1);
            char* str = 0;
            s32 idx = i + (s8)self->mField121;
            s32 entry = (s32)self->mUnk9C[idx];
            // Nested form: retail tests entry==0 on the fall-through path.
            if (entry == 0) {
                if (self->mUnk9C[idx - 1] != 0) {
                    str = BdatTouchStringCell((char*)"MNU_col6",
                                        (char*)"name", 5);
                }
            } else {
                str = BdatTouchStringCell((char*)"CL6_hintlist",
                                    (char*)"title", entry);
            }
            LayoutSetTextBoxFmtValue(self->mLayout,
                          buf, str, 0);
        }
        char buf2[0x20];
        sprintf(buf2, (char*)"txt_%02d", (s8)self->mField120 + 1);
        // Root pane re-derived for every use - keeping it in a named local
        // across the virtual calls costs an extra callee-saved register.
        nw4r::lyt::Pane* pane1 = self->mLayout->GetRootPane()->FindPaneByName(buf2, true);
        nw4r::lyt::Pane* pane2 = self->mLayout->GetRootPane()->FindPaneByName((char*)"nul_proportion", true);
        nw4r::math::VEC3 vec;
        ScalePaneWorldPos(&vec, pane1, pane2,
                      self->mLayout->GetRootPane());
        self->mCur18.setRootPaneTranslate(&vec);
        self->mScrollBar.PlaceThumb((u16)(s8)self->mField121);
        playUISound(1);
    } else if (down2) {
        // Row up with wrap: increment the row pair, resetting to 0/0 when the
        // page end is reached.
        if (self->mField11C >= 5) {
            self->mField120 = (u8)(self->mField120 + 1);
            if ((s8)self->mField120 >= 5) {
                self->mField120 = 4;
                self->mField121 = (u8)(self->mField121 + 1);
                if ((s8)self->mField121 >= (s32)self->mField11C - 4) {
                    self->mField120 = 0;
                    self->mField121 = 0;
                }
            }
        } else {
            self->mField120 = (u8)(self->mField120 + 1);
            if ((s8)self->mField120 >= (s32)self->mField11C) {
                self->mField120 = 0;
            }
        }
        for (s32 i = 0; i < 6; i++) {
            char buf[0x20];
            sprintf(buf, (char*)"txt_%02d", i + 1);
            char* str = 0;
            s32 idx = i + (s8)self->mField121;
            s32 entry = (s32)self->mUnk9C[idx];
            if (entry == 0) {
                if (self->mUnk9C[idx - 1] != 0) {
                    str = BdatTouchStringCell((char*)"MNU_col6",
                                        (char*)"name", 5);
                }
            } else {
                str = BdatTouchStringCell((char*)"CL6_hintlist",
                                    (char*)"title", entry);
            }
            LayoutSetTextBoxFmtValue(self->mLayout,
                          buf, str, 0);
        }
        char buf2[0x20];
        sprintf(buf2, (char*)"txt_%02d", (s8)self->mField120 + 1);
        nw4r::lyt::Pane* pane1 = self->mLayout->GetRootPane()->FindPaneByName(buf2, true);
        nw4r::lyt::Pane* pane2 = self->mLayout->GetRootPane()->FindPaneByName((char*)"nul_proportion", true);
        nw4r::math::VEC3 vec;
        ScalePaneWorldPos(&vec, pane1, pane2,
                      self->mLayout->GetRootPane());
        self->mCur18.setRootPaneTranslate(&vec);
        self->mScrollBar.PlaceThumb((u16)(s8)self->mField121);
        playUISound(1);
    } else if (confirm) {
        // Page down: jump one page back with wrap.
        if (self->mField11C >= 5) {
            self->mField121 = (u8)(self->mField121 - 5);
            if ((s8)self->mField121 < 0) {
                self->mField120 = (u8)((u8)self->mField121 + 4);
                self->mField121 = 0;
                if ((s8)self->mField120 < 0) {
                    self->mField120 = 0;
                }
            }
        } else {
            self->mField120 = 0;
            self->mField121 = 0;
        }
        for (s32 i = 0; i < 6; i++) {
            char buf[0x20];
            sprintf(buf, (char*)"txt_%02d", i + 1);
            char* str = 0;
            s32 idx = i + (s8)self->mField121;
            s32 entry = (s32)self->mUnk9C[idx];
            if (entry == 0) {
                if (self->mUnk9C[idx - 1] != 0) {
                    str = BdatTouchStringCell((char*)"MNU_col6",
                                        (char*)"name", 5);
                }
            } else {
                str = BdatTouchStringCell((char*)"CL6_hintlist",
                                    (char*)"title", entry);
            }
            LayoutSetTextBoxFmtValue(self->mLayout,
                          buf, str, 0);
        }
        char buf2[0x20];
        sprintf(buf2, (char*)"txt_%02d", (s8)self->mField120 + 1);
        nw4r::lyt::Pane* pane1 = self->mLayout->GetRootPane()->FindPaneByName(buf2, true);
        nw4r::lyt::Pane* pane2 = self->mLayout->GetRootPane()->FindPaneByName((char*)"nul_proportion", true);
        nw4r::math::VEC3 vec;
        ScalePaneWorldPos(&vec, pane1, pane2,
                      self->mLayout->GetRootPane());
        self->mCur18.setRootPaneTranslate(&vec);
        self->mScrollBar.PlaceThumb((u16)(s8)self->mField121);
        playUISound(1);
    } else if (up2) {
        // Page up: jump one page forward with wrap.
        if (self->mField11C >= 5) {
            self->mField121 = (u8)(self->mField121 + 5);
            if ((s8)self->mField121 > (s32)self->mField11C - 5) {
                self->mField120 = (u8)((u8)self->mField121 -
                                      ((s32)self->mField11C - 5));
                self->mField121 = (u8)(self->mField11C - 5);
                if ((s8)self->mField120 >= 5) {
                    self->mField120 = 4;
                }
            }
        } else {
            self->mField120 = (u8)(self->mField11C - 1);
            self->mField121 = 0;
            if ((s8)self->mField120 < 0) {
                self->mField120 = 0;
            }
        }
        for (s32 i = 0; i < 6; i++) {
            char buf[0x20];
            sprintf(buf, (char*)"txt_%02d", i + 1);
            char* str = 0;
            s32 idx = i + (s8)self->mField121;
            s32 entry = (s32)self->mUnk9C[idx];
            if (entry == 0) {
                if (self->mUnk9C[idx - 1] != 0) {
                    str = BdatTouchStringCell((char*)"MNU_col6",
                                        (char*)"name", 5);
                }
            } else {
                str = BdatTouchStringCell((char*)"CL6_hintlist",
                                    (char*)"title", entry);
            }
            LayoutSetTextBoxFmtValue(self->mLayout,
                          buf, str, 0);
        }
        char buf2[0x20];
        sprintf(buf2, (char*)"txt_%02d", (s8)self->mField120 + 1);
        nw4r::lyt::Pane* pane1 = self->mLayout->GetRootPane()->FindPaneByName(buf2, true);
        nw4r::lyt::Pane* pane2 = self->mLayout->GetRootPane()->FindPaneByName((char*)"nul_proportion", true);
        nw4r::math::VEC3 vec;
        ScalePaneWorldPos(&vec, pane1, pane2,
                      self->mLayout->GetRootPane());
        self->mCur18.setRootPaneTranslate(&vec);
        self->mScrollBar.PlaceThumb((u16)(s8)self->mField121);
        playUISound(1);
    }
}


// CCol6System::CCol6System - real member ctor; base ctors emit all three vptrs
// and the PTMF slots. The body initializes only this class's own members.
CCol6System::CCol6System(CProcess* parent)
    : mScn(reinterpret_cast<CScn*>(parent)), mFileHandle(0), mAnim90(0),
      mAnim94(0), mAnim98(0), mAnim9C(0), mFlagA0(0), mFlagA1(0),
      mFieldA2(0), mFieldA4(0), mPadA5_0(0), mPadA5_1(0), mCur1(0), mCur2(0) {
    __ct__7CSysWinFUc(mSysWin1, 0);
    __ct__7CSysWinFUc(mSysWin2, 0);

    // Camera/vector block: every CVec3 is built from a stack temp
    // CVec3(f,f,f) and copied memberwise (MWCC routes the bits through GPRs,
    // lfs/stfs-to-stack + lwz/stw), while the trailing float/byte pairs are
    // stored directly.
    f32 f = lbl_eu_80667564;
    // Camera/vector block: every CVec3 is built from a stack temp
    // CVec3(f,f,f) and copied memberwise (MWCC routes the bits through GPRs,
    // lfs/stfs-to-stack + lwz/stw), while the trailing float/byte pairs are
    // stored directly.
    // NOTE (open item): retail assigns the eight anon-temp slots non-
    // monotonically (sp+20,2c,08,14 then +50,5c,38,44 - two passes of
    // [e2,e3,e0,e1] over one contiguous 0x60 block); every source form tried
    // (statement reorder, named locals both decl orders, local CVec3[8],
    // placement-new element ctors, record-pointer loop) yields descending
    // slots or extra out-of-line/round-trip code instead.
    mCamPos = ml::CVec3(f, f, f);
    mCamLookAt = ml::CVec3(f, f, f);
    mVec168 = ml::CVec3(f, f, f);
    mVec174 = ml::CVec3(f, f, f);
    mField180 = f;
    mField184 = 0;
    mVec188 = ml::CVec3(f, f, f);
    mVec194 = ml::CVec3(f, f, f);
    mVec1A0 = ml::CVec3(f, f, f);
    mVec1AC = ml::CVec3(f, f, f);
    mField1B8 = f;
    mField1BC = 0;
}

// extern "C" free-function form (CCol6Hint precedent): sub-dtors in retail
// order (mSysWin2/mSysWin1/mCur2/mCur1/mMemRegion, flags -1), double-null-
// guarded CProcess base dtor, flags-based delete; default -O4,p.
extern "C" void* __dt__11CCol6SystemFv(CCol6System* self, int flags) {
    if (self != 0) {
        __dt__7CSysWinFv(&self->mSysWin2, -1);
        __dt__7CSysWinFv(&self->mSysWin1, -1);
        __dt__6CCur18Fv(&self->mCur2, -1);
        __dt__6CCur18Fv(&self->mCur1, -1);
        __dt__17UnkClass_8045F564Fv(&self->mMemRegion, -1);
        if (self != 0) {
            if (self != 0) {
                __dt__8CProcessFv(reinterpret_cast<CProcess*>(self), 0);
            }
        }
        if (flags > 0) {
            operator delete(self);
        }
    }
    return self;
}

// CCol6System::Init() - load the System.arc file through the embedded IWorkEvent
// interface (the +0x6C subobject; null-this-safe), then rebuild both embedded
// CSysWin bodies: construct a stack temp, memberwise-copy the non-vtable fields,
// destroy the temp, and dispatch the layout-build virtual at vtable +0x88.
void CCol6System::Init() {
    // Null-this-safe IWorkEvent subobject: default to `this` (0 when null) and
    // conditionally add +0x6C (retail: mr r30,r31 / beq / addi r30,r3,0x6c).
    IWorkEvent* evt = reinterpret_cast<IWorkEvent*>(this);
    if (this != 0) {
        evt = &static_cast<IWorkEvent&>(*this);
    }
    mFileHandle = CDeviceFile::readFile(
        mtl::MemManager::getHandleMEM2(), sCol6ArcPath, evt, 0, 0);

    // Rebuild window 1 (direction 2): copy every non-vtable field so MWCC
    // emits the retail lwz/stw + lbz/stb sequence (skips +0x29..0x2B padding).
    u8 tempW[0x3C];
    __ct__7CSysWinFUc(tempW, 2);
    CSysWinData* w1 = reinterpret_cast<CSysWinData*>(mSysWin1);
    CSysWinData* t1 = reinterpret_cast<CSysWinData*>(tempW);
    w1->f_04 = t1->f_04;
    w1->f_08 = t1->f_08;
    w1->f_0c = t1->f_0c;
    w1->f_10 = t1->f_10;
    w1->f_14 = t1->f_14;
    w1->f_18 = t1->f_18;
    w1->f_1c = t1->f_1c;
    w1->f_20 = t1->f_20;
    w1->f_24 = t1->f_24;
    w1->f_28 = t1->f_28;
    w1->f_2c = t1->f_2c;
    w1->f_30 = t1->f_30;
    w1->f_34 = t1->f_34;
    w1->f_35 = t1->f_35;
    w1->f_36 = t1->f_36;
    w1->f_37 = t1->f_37;
    w1->f_38 = t1->f_38;
    w1->f_39 = t1->f_39;
    __dt__7CSysWinFv(tempW, -1);
    reinterpret_cast<CSysWin*>(mSysWin1)->loadSystemArc();

    // Rebuild window 2 the same way (direction 0).
    u8 tempW2[0x3C];
    __ct__7CSysWinFUc(tempW2, 0);
    CSysWinData* w2 = reinterpret_cast<CSysWinData*>(mSysWin2);
    CSysWinData* t2 = reinterpret_cast<CSysWinData*>(tempW2);
    w2->f_04 = t2->f_04;
    w2->f_08 = t2->f_08;
    w2->f_0c = t2->f_0c;
    w2->f_10 = t2->f_10;
    w2->f_14 = t2->f_14;
    w2->f_18 = t2->f_18;
    w2->f_1c = t2->f_1c;
    w2->f_20 = t2->f_20;
    w2->f_24 = t2->f_24;
    w2->f_28 = t2->f_28;
    w2->f_2c = t2->f_2c;
    w2->f_30 = t2->f_30;
    w2->f_34 = t2->f_34;
    w2->f_35 = t2->f_35;
    w2->f_36 = t2->f_36;
    w2->f_37 = t2->f_37;
    w2->f_38 = t2->f_38;
    w2->f_39 = t2->f_39;
    __dt__7CSysWinFv(tempW2, -1);
    reinterpret_cast<CSysWin*>(mSysWin2)->loadSystemArc();
}

// CCol6System::Term() - wait for the VI draw to finish, detach the render
// callback, release both embedded windows/cursors and the layout/accessor/
// scratch region, then clear the CCol6System singleton global.
void CCol6System::Term() {
    CDeviceVI::waitForDrawDone();
    closeFileHandle(&mFileHandle);

    // The `if (this)` is the MWCC idiom that splits mr r4 / beq / addi +0x70
    // for the IScnRender subobject passed to removeRenderCB.
    IScnRender* render = reinterpret_cast<IScnRender*>(this);
    if (this != 0) {
        render = &static_cast<IScnRender&>(*this);
    }
    mScn->removeRenderCB(render);

    mFlagA0 = 0;
    mCur1.cleanup();
    mCur2.cleanup();
    reinterpret_cast<CSysWin*>(&mSysWin1)->termLayout();
    reinterpret_cast<CSysWin*>(&mSysWin2)->termLayout();

    // The outer guard plus delete's own null-check reproduce retail's two beq's
    // on the same CR0 test (MWCC D2-inlined-into-D1 artifact).
    if (mLayout != 0) {
        delete mLayout;
        mLayout = 0;
    }

    releaseArcResourceAccessor(mArcAccessor);
    mArcAccessor = 0;
    mMemRegion.deleteRegion();
    sCol6InviteInstance = 0;
}

// Cursor/window position sync, shared by the anim-complete states of Move().
// Retail inlines this helper at each call site (each copy gets its own stack
// slots for pos/buf/vec), which drives Move()'s 0x1f0 frame layout.
inline void CCol6System::updateCursorPos() {
    if (reinterpret_cast<CSysWin*>(&mSysWin1)->isOpened() != 0) {
        nw4r::math::VEC3 pos;
        CSysWin::getPaneScreenPos(reinterpret_cast<nw4r::math::VEC3*>(reinterpret_cast<u8*>(&pos)), reinterpret_cast<CSysWin*>(&mSysWin1), mPadA5_1);
        mCur2.setRootPaneTranslate(&pos);
    } else {
        char buf[0x20];
        if (mFieldA4 >= 6) {
            sprintf(buf, &lbl_eu_80502050[0xa9], (s8)mPadA5_0 + 1);
        } else {
            sprintf(buf, &lbl_eu_80502050[0xe], (s8)mPadA5_0 + 1);
        }
        nw4r::lyt::Pane* rootPane =
            mLayout->GetRootPane();
        nw4r::lyt::Pane* pane1 =
            rootPane->FindPaneByName(buf, true);
        nw4r::lyt::Pane* pane2 = rootPane->FindPaneByName(&lbl_eu_80502050[0x17], true);
        nw4r::math::VEC3 vec;
        ScalePaneWorldPos(&vec, pane1, pane2, rootPane);
        mCur1.setRootPaneTranslate(&vec);
    }
}

// CCol6System::Move() - main state machine (mFieldA4 switch, states 0..0x27):
// anim-wait states drive the four layout anims and reposition the cursors,
// input states dispatch to the item-box handlers, and the camera/fade states
// post the camera moves through the CfGameManager API. All states fall
// through to the common tail (anim-host tick, cursor/window updates).
void CCol6System::Move() {
    if (IsMenuState621F0() == 0) return;
    if (mFlagA0 == 0) return;
    if (reinterpret_cast<CSysWin*>(&mSysWin1)->isReady() == 0) return;
    // Retail splits gate 4 into a two-instruction bne-body / b-exit pair
    // (same shape as cbRenderBefore), so use the goto split here too.
    if (reinterpret_cast<CSysWin*>(&mSysWin2)->isReady() != 0) goto body;
    return;
body:

    switch (mFieldA4) {
    case 0:
        mFieldA4 = 1;
        playUISound(0x6d);
        break;

    case 1:
        // Out anim finished: swap the pair and open the box.
        if (advanceAnimTransform(mAnim90, lbl_eu_80667558) == 0) break;
        mFieldA4 = 2;
        mLayout->SetAnimationEnable(mAnim98, 0);
        mLayout->SetAnimationEnable(mAnim9C, 0);
        mLayout->SetAnimationEnable(mAnim90, 0);
        mLayout->SetAnimationEnable(mAnim94, 1);
        break;

    case 2:
        // In anim finished: position the player-selection cursor.
        if (advanceAnimTransform(mAnim94, lbl_eu_80667558) == 0) break;
        mFieldA4 = 3;
        updateCursorPos();
        break;

    case 3:
        col6PadHandle(this);
        break;

    case 4:
        // In anim done: swap back and close.
        if (AnimRewindFrame(mAnim94, lbl_eu_80667558) == 0) break;
        mFieldA4 = 5;
        mLayout->SetAnimationEnable(mAnim94, 0);
        mLayout->SetAnimationEnable(mAnim98, 0);
        mLayout->SetAnimationEnable(mAnim9C, 0);
        mLayout->SetAnimationEnable(mAnim90, 1);
        break;

    case 5:
        // Out anim done: resolve the target index from the player slot.
        if (AnimRewindFrame(mAnim90, lbl_eu_80667558) == 0) break;
        mField60 = 0xff;
        if ((s8)mPadA5_0 == 2) mField60 = 1;
        else if ((s8)mPadA5_0 == 1) mField60 = 2;
        mPadA5_0 = 0;
        mField64 = 1;
        break;

    case 6:
        // Second-layer out anim: switch to the box anim and reset the cursor.
        if (advanceAnimTransform(mAnim98, lbl_eu_80667558) == 0) break;
        mFieldA4 = 7;
        mLayout->SetAnimationEnable(mAnim90, 0);
        mLayout->SetAnimationEnable(mAnim94, 0);
        mLayout->SetAnimationEnable(mAnim98, 0);
        mLayout->SetAnimationEnable(mAnim9C, 1);
        mPadA5_0 = 0;
        updateCursorPos();
        break;

    case 7:
        col6SwitchBox(this);
        break;

    case 8:
        // Box anim done: resolve target.
        if (advanceAnimTransform(mAnim9C, lbl_eu_80667558) == 0) break;
        mField60 = 0xff;
        mField64 = 1;
        break;

    case 9:
        // Close the box: reset the player and position the cursor.
        if (AnimRewindFrame(mAnim98, lbl_eu_80667558) == 0) break;
        mFieldA4 = 3;
        mPadA5_0 = 0;
        updateCursorPos();
        break;

    case 0xa:
        // Window 1 active: switch to the slot-selection state.
        if (reinterpret_cast<CSysWin*>(&mSysWin1)->isActive() == 0) break;
        mFieldA4 = 0xb;
        mPadA5_1 = 0;
        updateCursorPos();
        break;

    case 0xb:
        col6WinScroll(this);
        break;

    case 0xc:
        // Window 1 active: confirm posts the item-box sync, up re-opens.
        if (reinterpret_cast<CSysWin*>(&mSysWin1)->isActive() == 0) break;
        if ((s8)mPadA5_1 == 0) {
            mFieldA4 = 0xd;
            CUICfManager_queueFadeMenu(0, 0, lbl_eu_80667578, lbl_eu_80667578,
                          lbl_eu_80667578);
            Col6_SyncBoxSlot_0EE4((u32)(uintptr_t)this, (s8)mPadA5_0);
        } else {
            mFieldA4 = 7;
            updateCursorPos();
        }
        break;

    case 0xd:
        col6DoOpenBox(this);
        break;

    case 0xe:
        // Fade idle: resolve target and flag.
        if (getFadeMenu() != 0) break;
        mField60 = 0xff;
        mField64 = 1;
        break;

    case 0xf:
        // Sound done: fade out and move the camera to the stored position.
        if (CfSoundMan_TouchSlotById(mFieldA2) != 0) break;
        mFieldA4 = 0x10;
        CUICfManager_queueFadeMenu(2, 0, lbl_eu_80667578, lbl_eu_80667578,
                      lbl_eu_80667578);
        notifyBattleSystem__Q22cf13CfGameManagerFv(9, 0, 0, 0, 0);
        isEffectReady__Q22cf13CfGameManagerFv(
            0, &mCamPos, &mCamLookAt, 0, lbl_eu_80667564);
        notifyCameraManager__Q22cf13CfGameManagerFv();
        break;

    case 0x10:
        mFieldA4 = 0x11;
        notifyBattleSystem__Q22cf13CfGameManagerFv(
            9, 0, (u32)(s32)mField180, mField184, 0);
        isEffectReady__Q22cf13CfGameManagerFv(
            (u16)(s32)mField180, &mVec168, &mVec174, mField184,
            lbl_eu_80667564);
        notifyCameraManager__Q22cf13CfGameManagerFv();
        break;

    case 0x11:
        if (getFadeMenu() != 0) break;
        mFieldA4 = 0x12;
        break;

    case 0x12:
        if (isCameraReady__Q22cf13CfGameManagerFv() != 0) break;
        mFieldA4 = 0x13;
        notifyBattleSystem__Q22cf13CfGameManagerFv(9, 0, 0, 0, 0);
        isEffectReady__Q22cf13CfGameManagerFv(
            0, &mVec188, &mVec194, 0, lbl_eu_80667564);
        notifyCameraManager__Q22cf13CfGameManagerFv();
        break;

    case 0x13:
        if (isCameraReady__Q22cf13CfGameManagerFv() != 0) break;
        mFieldA4 = 0x14;
        notifyBattleSystem__Q22cf13CfGameManagerFv(
            9, 0, (u32)(s32)mField1B8, mField1BC, 0);
        isEffectReady__Q22cf13CfGameManagerFv(
            (u16)(s32)mField1B8, &mVec1A0, &mVec1AC, mField1BC,
            lbl_eu_80667564);
        notifyCameraManager__Q22cf13CfGameManagerFv();
        break;

    case 0x14:
        Col6Win2_2C40(this);
        break;

    case 0x15:
        // Fade idle: fade out and clear the block flags.
        if (getFadeMenu() == 0) break;
        if (isFadeActive(getFadeMenu()) != 0) break;
        mFieldA4 = 0xe;
        CUICfManager_queueFadeMenu(2, 0, lbl_eu_80667578, lbl_eu_80667578,
                      lbl_eu_80667578);
        lookupEffectForResource__Q22cf13CfGameManagerFv(0, 0, 0);
        break;

    case 0x16:
        Col6_OpenBox_2DB4(this);
        break;

    case 0x17:
        mFieldA4 = 0x18;
        notifyBattleSystem__Q22cf13CfGameManagerFv(
            9, 0, (u32)(s32)mField180, mField184, 0);
        isEffectReady__Q22cf13CfGameManagerFv(
            (u16)(s32)mField180, &mVec168, &mVec174, mField184,
            lbl_eu_80667564);
        notifyCameraManager__Q22cf13CfGameManagerFv();
        break;

    case 0x18:
        if (getFadeMenu() != 0) break;
        mFieldA4 = 0x19;
        break;

    case 0x19:
        if (isCameraReady__Q22cf13CfGameManagerFv() != 0) break;
        mFieldA4 = 0x1a;
        notifyBattleSystem__Q22cf13CfGameManagerFv(9, 0, 0, 0, 0);
        isEffectReady__Q22cf13CfGameManagerFv(
            0, &mVec188, &mVec194, 0, lbl_eu_80667564);
        notifyCameraManager__Q22cf13CfGameManagerFv();
        break;

    case 0x1a:
        if (isCameraReady__Q22cf13CfGameManagerFv() != 0) break;
        mFieldA4 = 0x1b;
        notifyBattleSystem__Q22cf13CfGameManagerFv(
            9, 0, (u32)(s32)mField1B8, mField1BC, 0);
        isEffectReady__Q22cf13CfGameManagerFv(
            (u16)(s32)mField1B8, &mVec1A0, &mVec1AC, mField1BC,
            lbl_eu_80667564);
        notifyCameraManager__Q22cf13CfGameManagerFv();
        break;

    case 0x1b:
        // Window-2 join banner for the current player count.
        if (isCameraReady__Q22cf13CfGameManagerFv() != 0) break;
        mFieldA4 = 0x20;
        {
            u8 cnt = (u8)CtrlRemote_TouchBitByArg(0x802);
            char* s1 = BdatTouchStringCell((char*)"MNU_col6", (char*)"name",
                                     cnt + 0x67);
            char* s2 = BdatTouchStringCell((char*)"MNU_col6", (char*)"name",
                                     cnt + 0x6c);
            ml::FixStr<128> buf;
            buf.format((char*)"%s<n>%s", s1, s2);
            reinterpret_cast<CSysWin*>(&mSysWin2)->setKindContent(reinterpret_cast<char*>(buf.mString), 0);
            reinterpret_cast<CSysWin*>(&mSysWin2)->openPhase1();
            cf::CfSoundMan::playActorSound(1, 0x134, 0, 0, lbl_eu_80667558);
        }
        break;

    case 0x1c:
        col6Win2Count(this);
        break;

    case 0x1d:
        // Window 2 active: pad-confirm closes it; otherwise reopen.
        if (reinterpret_cast<CSysWin*>(&mSysWin2)->isOpened() != 0) {
            if (reinterpret_cast<CSysWin*>(&mSysWin2)->isActive() != 0) {
                CCol6Pad* pad = getCurrentPad__Q22cf13CfGameManagerFv();
                u32 cond;
                if (cf::CfGameManager::isClassicController(-1) != 0) {
                    cond = (pad->mPressedButtonFlags & 0x200000) != 0;
                } else {
                    cond = (pad->mPressedButtonFlags & 0x10) != 0;
                }
                if (cond != 0) reinterpret_cast<CSysWin*>(&mSysWin2)->advancePhase3();
            }
        } else {
            mFieldA4 = 7;
        }
        break;

    case 0x1e:
        Col6_Win2CountLadder_3614(this);
        break;

    case 0x1f:
        Col6_Win2Interact_378C(this);
        break;

    case 0x20:
        col6Win2PerPl(this);
        break;

    case 0x21:
        if (reinterpret_cast<CSysWin*>(&mSysWin2)->isOpened() != 0) {
            if (reinterpret_cast<CSysWin*>(&mSysWin2)->isActive() != 0) {
                CCol6Pad* pad = getCurrentPad__Q22cf13CfGameManagerFv();
                u32 cond;
                if (cf::CfGameManager::isClassicController(-1) != 0) {
                    cond = (pad->mPressedButtonFlags & 0x200000) != 0;
                } else {
                    cond = (pad->mPressedButtonFlags & 0x10) != 0;
                }
                if (cond != 0) reinterpret_cast<CSysWin*>(&mSysWin2)->advancePhase3();
            }
        } else {
            mFieldA4 = 0x24;
            char* s = BdatTouchStringCell((char*)"MNU_col6", (char*)"name",
                                     0x7c);
            u16 id = BdatGetU16ByTableKey((char*)"CL6_rwdlist",
                                   (char*)"itemID",
                                   (s8)mPadA5_0 + 1);
            UIWin_CreateItemMulti(id, 0, 0, 0, 0, 1, 0, 1, 0);
        }
        break;

    case 0x22:
        col6JoinCheck(this);
        break;

    case 0x23:
        if (reinterpret_cast<CSysWin*>(&mSysWin2)->isOpened() != 0) {
            if (reinterpret_cast<CSysWin*>(&mSysWin2)->isActive() != 0) {
                CCol6Pad* pad = getCurrentPad__Q22cf13CfGameManagerFv();
                u32 cond;
                if (cf::CfGameManager::isClassicController(-1) != 0) {
                    cond = (pad->mPressedButtonFlags & 0x200000) != 0;
                } else {
                    cond = (pad->mPressedButtonFlags & 0x10) != 0;
                }
                if (cond != 0) reinterpret_cast<CSysWin*>(&mSysWin2)->advancePhase3();
            }
        } else {
            mFieldA4 = 0x26;
            char* s = BdatTouchStringCell((char*)"MNU_col6", (char*)"name",
                                     0x7c);
            u16 id = BdatGetU16ByTableKey((char*)"CL6_rwdlist",
                                   (char*)"itemID", 5);
            UIWin_CreateItemMulti(id, 0, 0, 0, 0, 1, 0, 1, 0);
        }
        break;

    case 0x24:
        // Per-slot threshold scan (same shape as CCol6Invite::Move).
        if (GetItemMulti_IsActiveFlag() != 0) break;
        {
            union {
                struct {
                    u32 w;
                    u8 b;
                };
                u8 bytes[5];
            } data;
            data.w = lbl_eu_80667540;
            data.b = lbl_eu_80667544;

            u8 result = (u8)CtrlRemote_TouchBitByArg(0x7fc);

            u8 found = 0;
            for (u32 i = 0; i < 5; i++) {
                u8 idx = (u8)i;
                if (result >= data.bytes[idx] &&
                    CtrlRemote_TouchBitByArg(idx + 0x804) == 0) {
                    found = 1;
                    break;
                }
            }

            if (found != 0) {
                union {
                    struct {
                        u32 w;
                        u8 b;
                    };
                    u8 bytes[5];
                } data2;
                union {
                    struct {
                        u32 w;
                        u8 b;
                    };
                    u8 bytes[5];
                } data3;
                data2.w = lbl_eu_80667548;
                data2.b = lbl_eu_8066754C;
                data3.w = lbl_eu_80667550;
                data3.b = lbl_eu_80667554;

                s16 val = (s16)CtrlRemote_TouchBitByArg(0x7fc);
                for (u32 i = 0; i < 5; i++) {
                    if (val >= data2.bytes[i] &&
                        CtrlRemote_TouchBitByArg(i + 0x804) == 0) {
                        sCol6QueuedWindowId = (s32)i;
                        CtrlRemote_SetSharedBit(i + 0x804, 1);
                        val = (s16)(val + (s8)data3.bytes[i]);
                        CtrlRemote_SetSharedBit(0x7fc, val);
                    }
                }
            }
            mFieldA4 = 0x15;
            CUICfManager_queueFadeMenu(0, 0, lbl_eu_80667578, lbl_eu_80667578,
                          lbl_eu_80667578);
        }
        break;

    case 0x25:
        if (reinterpret_cast<CSysWin*>(&mSysWin2)->isOpened() != 0) {
            if (reinterpret_cast<CSysWin*>(&mSysWin2)->isActive() != 0) {
                CCol6Pad* pad = getCurrentPad__Q22cf13CfGameManagerFv();
                u32 cond;
                if (cf::CfGameManager::isClassicController(-1) != 0) {
                    cond = (pad->mPressedButtonFlags & 0x200000) != 0;
                } else {
                    cond = (pad->mPressedButtonFlags & 0x10) != 0;
                }
                if (cond != 0) reinterpret_cast<CSysWin*>(&mSysWin2)->advancePhase3();
            }
        } else {
            mFieldA4 = 0x28;
            char* s = BdatTouchStringCell((char*)"MNU_col6", (char*)"name",
                                     0x80);
            reinterpret_cast<CSysWin*>(&mSysWin2)->setKindContent(reinterpret_cast<char*>(s), 0);
            reinterpret_cast<CSysWin*>(&mSysWin2)->openPhase1();
        }
        break;

    case 0x26:
    case 0x27:
    case 0x28:
        // Window 2 active: pad-confirm closes it; otherwise fade out.
        if (reinterpret_cast<CSysWin*>(&mSysWin2)->isOpened() != 0) {
            if (reinterpret_cast<CSysWin*>(&mSysWin2)->isActive() != 0) {
                CCol6Pad* pad = getCurrentPad__Q22cf13CfGameManagerFv();
                u32 cond;
                if (cf::CfGameManager::isClassicController(-1) != 0) {
                    cond = (pad->mPressedButtonFlags & 0x200000) != 0;
                } else {
                    cond = (pad->mPressedButtonFlags & 0x10) != 0;
                }
                if (cond != 0) reinterpret_cast<CSysWin*>(&mSysWin2)->advancePhase3();
            }
        } else {
            mFieldA4 = 0x15;
            CUICfManager_queueFadeMenu(0, 0, lbl_eu_80667578, lbl_eu_80667578,
                          lbl_eu_80667578);
        }
        break;

    default:
        break;
    }

    // Common tail: per-frame updates.
    mLayout->Animate(0);
    mCur1.Update();
    mCur2.Update();
    reinterpret_cast<CSysWin*>(&mSysWin1)->dispatchPhase();
    reinterpret_cast<CSysWin*>(&mSysWin2)->dispatchPhase();
}

// Col6_SysOnFile_0118 - second layout file-load completion handler: create the
// scratch region + host, detach the arc buffer into a fresh accessor, build
// the layout (col6SysLayout), flag it ready and register the render callback.
extern "C" int Col6_SysOnFile_0118(CCol6System* self, CEventFile* event) {
    // Same shape as Col6_HintOnFile_DB08: late failure label (no dtor on that
    // path), host scoped to the success block, pool base materialized after
    // getHandleMEM2.
    if (self->mFileHandle != event->mFileHandle) goto fail;

    {
        u8* data;

        mtl::ALLOC_HANDLE mem = mtl::MemManager::getHandleMEM2();
        char* pool = lbl_eu_80502050;
        self->mMemRegion.createRegion(mem, 0x20000, &pool[0xd0], 0);
        Class_8045F858 host(&self->mMemRegion);

        CFileHandle* handle = self->mFileHandle;
        data = handle->mData;
        handle->mData = 0;
        mtl::MemManager::setMemInitFlag(false);

        self->mArcAccessor = CLibLayout::createArcResourceAccessor();
        self->mArcAccessor->Attach(data, &pool[0x42]);
        col6SysLayout(self);
        self->mFlagA0 = 1;

        IScnRender* render = reinterpret_cast<IScnRender*>(self);
        if (self != 0) {
            render = &static_cast<IScnRender&>(*self);
        }
        self->mScn->addRenderCB(render, 0xd, 0);

        self->mFileHandle = 0;
        self->mMemRegion.validateHeap();
    }
    return 1;
fail:
    return 0;
}

// cbRenderBefore - draw both embedded windows through a stack DrawInfo.
// Gates 1-3 are single beq-to-exit branches; gate 4 must stay as the
// two-instruction bne-body / b-exit pair, so use the goto-body/goto-end
// split (CCol6Hint::cbRenderBefore precedent).
void CCol6System::cbRenderBefore() {
    if (IsMenuState621F0() == 0) goto end;
    if (mFlagA0 == 0) goto end;
    if (reinterpret_cast<CSysWin*>(&mSysWin1)->isReady() == 0) goto end;
    if (reinterpret_cast<CSysWin*>(&mSysWin2)->isReady() != 0) goto draw;
    goto end;
end:
    return;
draw:
    GXSetZMode((GXBool)0, GX_NEVER, (GXBool)0);
    nw4r::lyt::DrawInfo drawInfo;
    SetWideLocAdjust(&drawInfo);
    if (mFlagA1 == 0) {
        drawLayout(mLayout, &drawInfo, 0, 1);
        mCur1.DrawLayout(&drawInfo);
        reinterpret_cast<CSysWin*>(&mSysWin1)->drawLayout(reinterpret_cast<nw4r::lyt::DrawInfo*>(&drawInfo));
        mCur2.DrawLayout(&drawInfo);
    }
    reinterpret_cast<CSysWin*>(&mSysWin2)->drawLayout(reinterpret_cast<nw4r::lyt::DrawInfo*>(&drawInfo));
}

// col6SysCreate - item-box system-window factory (same shape as
// col6HintAlloc, but constructs a 0x240-byte CCol6System and uses its own
// singleton global sCol6InviteInstance).
extern "C" IUIWindow* col6SysCreate(CProcess* pParent, CScn* pScene) {
    if (sCol6InviteInstance != 0) {
        return 0;
    }

    CCol6System* obj = reinterpret_cast<CCol6System*>(
        mtl::MemManager::allocate(0x240, CWorkThreadSystem::getWorkMem()));
    obj = new (obj) CCol6System(reinterpret_cast<CProcess*>(pScene));
    sCol6InviteInstance = (int)(uintptr_t)obj;
    reinterpret_cast<CProcess*>((uintptr_t)sCol6InviteInstance)->Regist(pParent, false);
    return reinterpret_cast<IUIWindow*>((uintptr_t)sCol6InviteInstance);
}

// col6SysLayout - builds the second CCol6System layout: attaches the arc
// ("out" + four anims), binds the device font, labels the 21 count panes when
// the font value is present, rebuilds both embedded cursors, labels the
// player-count rows and summary panes, then refreshes the per-player item box
// (col6FillSlots) and writes the reserved-slot count string.
extern "C" void col6SysLayout(CCol6System* self) {
    self->mPadA5_0 = 0;

    buildLayout(&self->mLayout, self->mArcAccessor, (char*)"mf35_rv00.brlyt");
    bindLayoutAnimTransform(self->mLayout, &self->mAnim90, self->mArcAccessor, (char*)"mf35_rv00_in.brlan");
    bindLayoutAnimTransform(self->mLayout, &self->mAnim94, self->mArcAccessor, (char*)"mf35_rv00_info_in.brlan");
    bindLayoutAnimTransform(self->mLayout, &self->mAnim98, self->mArcAccessor, (char*)"mf35_rv00_next.brlan");
    bindLayoutAnimTransform(self->mLayout, &self->mAnim9C, self->mArcAccessor, (char*)"mf35_rv00_out.brlan");

    // Bind the device font into the layout's root pane.
    nw4r::lyt::Pane* rootPane =
        self->mLayout->GetRootPane();
    CDeviceFontInfo* fontObj = getFontInfo__11CDeviceFontFUlPQ34nw4r3lyt6Layout(
        1, self->mLayout);
    ApplyFontToTxtPanes(rootPane, fontObj->getFont());

    // If the font value is present, label all 21 count panes with it.
    u32 fontVal = (u32)CUICfManager_getPackedFont9C();
    if (fontVal != 0) {
        setLayoutTextBoxFont(self->mLayout, (char*)"txt_now02", fontVal);
        setLayoutTextBoxFont(self->mLayout, (char*)"txt_now_val01", fontVal);
        setLayoutTextBoxFont(self->mLayout, (char*)"txt_now_val02", fontVal);
        setLayoutTextBoxFont(self->mLayout, (char*)"txt_num01", fontVal);
        setLayoutTextBoxFont(self->mLayout, (char*)"txt_num02", fontVal);
        setLayoutTextBoxFont(self->mLayout, (char*)"txt_num03", fontVal);
        setLayoutTextBoxFont(self->mLayout, (char*)"txt_num04", fontVal);
        setLayoutTextBoxFont(self->mLayout, (char*)"txt_mat_val01", fontVal);
        setLayoutTextBoxFont(self->mLayout, (char*)"txt_mat_val02", fontVal);
        setLayoutTextBoxFont(self->mLayout, (char*)"txt_mat_val03", fontVal);
        setLayoutTextBoxFont(self->mLayout, (char*)"txt_mat_val04", fontVal);
        setLayoutTextBoxFont(self->mLayout, (char*)"txt_mat_val05", fontVal);
        setLayoutTextBoxFont(self->mLayout, (char*)"txt_gold00", fontVal);
        setLayoutTextBoxFont(self->mLayout, (char*)"txt_gold01", fontVal);
        setLayoutTextBoxFont(self->mLayout, (char*)"txt_mat_val01_01", fontVal);
        setLayoutTextBoxFont(self->mLayout, (char*)"txt_mat_val02_01", fontVal);
        setLayoutTextBoxFont(self->mLayout, (char*)"txt_mat_val03_01", fontVal);
        setLayoutTextBoxFont(self->mLayout, (char*)"txt_mat_val04_01", fontVal);
        setLayoutTextBoxFont(self->mLayout, (char*)"txt_mat_val05_01", fontVal);
        setLayoutTextBoxFont(self->mLayout, (char*)"txt_gold00_01", fontVal);
        setLayoutTextBoxFont(self->mLayout, (char*)"txt_gold01_01", fontVal);
    }

    // Start the four anims (setAnim at +0x2C) and tick once (+0x38).
    self->mLayout->SetAnimationEnable(self->mAnim94, 0);
    self->mLayout->SetAnimationEnable(self->mAnim98, 0);
    self->mLayout->SetAnimationEnable(self->mAnim9C, 0);
    self->mLayout->SetAnimationEnable(self->mAnim90, 1);
    self->mLayout->Animate(0);

    // Rebuild cursor 1 (mCur1 at +0xA8) from a stack temp (data fields only).
    self->mCur1 = CCur18(CUICfManager_getArcResourceAccessor());
    self->mCur1.initLayout();

    // Rebuild cursor 2 (mCur2 at +0xC0) from a stack temp (data fields only).
    self->mCur2 = CCur18(CUICfManager_getArcResourceAccessor());
    self->mCur2.initLayout();

    // Label the two count panes.
    LayoutSetTextBoxFmtValue(self->mLayout, (char*)"txt_title",
                  BdatTouchStringCell((char*)"MNU_col6", (char*)"name", 0x1c),
                  0);
    LayoutSetTextBoxFmtValue(self->mLayout, (char*)"txt_help",
                  BdatTouchStringCell((char*)"MNU_col6", (char*)"name", 0x1),
                  0);

    // Three slot labels: formatted pane name, then the per-slot name char.
    for (s32 i = 0; i < 3; i++) {
        char buf[0x20];
        sprintf(buf, (char*)"txt_%02d", i + 1);
        LayoutSetTextBoxFmtValue(self->mLayout, buf,
                      BdatTouchStringCell((char*)"MNU_col6", (char*)"name",
                                    i + 2),
                      0);
    }

    // Blank rows and static summary labels.
    LayoutSetTextBoxFmtValue(self->mLayout, (char*)"txt_04",
                  (char*)"", 0);
    LayoutSetTextBoxFmtValue(self->mLayout, (char*)"txt_05",
                  (char*)"", 0);
    char* s1e = BdatTouchStringCell((char*)"MNU_col6", (char*)"name", 0x1e);
    LayoutSetTextBoxFmtValue(self->mLayout, (char*)"txt_mes", s1e, 0);
    LayoutSetTextBoxFmtValue(self->mLayout, (char*)"txt_sel05",
                  BdatTouchStringCell((char*)"MNU_col6", (char*)"name", 5), 0);
    LayoutSetTextBoxFmtValue(self->mLayout, (char*)"txt_material",
                  BdatTouchStringCell((char*)"MNU_col6", (char*)"name", 0x43),
                  0);
    LayoutSetTextBoxFmtValue(self->mLayout, (char*)"txt_material_01",
                  BdatTouchStringCell((char*)"MNU_col6", (char*)"name", 0x47),
                  0);

    char* s46 = BdatTouchStringCell((char*)"MNU_col6", (char*)"name", 0x46);

    // Four player-count rows: count pane + two sub-labels per row.
    for (s32 i = 0; i < 4; i++) {
        u8 idx = (u8)(i + 1);
        u32 c = CtrlRemote_TouchBitByArg(i + 0x7fe);
        char buf[0x20];
        sprintf(buf, (char*)"txt_num%02d", idx);
        setLayoutTextBoxNumber(self->mLayout, buf, c);
        sprintf(buf, (char*)"txt_lv%02d", idx);
        LayoutSetTextBoxFmtValue(self->mLayout, buf, s46, 0);
        sprintf(buf, (char*)"txt_sel%02d", idx);
        LayoutSetTextBoxFmtValue(self->mLayout, buf,
                      BdatTouchStringCell((char*)"MNU_col6", (char*)"name",
                                    i + 0x3d),
                      0);
    }

    col6FillSlots(self, (s8)self->mPadA5_0);

    // Remaining summary rows.
    LayoutSetTextBoxFmtValue(self->mLayout, (char*)"txt_now",
                  BdatTouchStringCell((char*)"MNU_col6", (char*)"name", 0x15),
                  0);
    LayoutSetTextBoxFmtValue(self->mLayout, (char*)"txt_now01",
                  BdatTouchStringCell((char*)"MNU_col6", (char*)"name", 0x16),
                  0);
    LayoutSetTextBoxFmtValue(self->mLayout, (char*)"txt_now02",
                  BdatTouchStringCell((char*)"help",
                                (char*)"name", 0x16),
                  0);
    LayoutSetTextBoxFmtValue(self->mLayout, (char*)"txt_now03",
                  BdatTouchStringCell((char*)"MNU_col6", (char*)"name", 0x17),
                  0);
    LayoutSetTextBoxFmtValue(self->mLayout, (char*)"txt_now04",
                  BdatTouchStringCell((char*)"help",
                                (char*)"name", 0x17),
                  0);
    LayoutSetTextBoxFmtValue(self->mLayout, (char*)"txt_now05",
                  BdatTouchStringCell((char*)"MNU_col6", (char*)"name", 0x18),
                  0);

    // Player counts on the two slots.
    setLayoutTextBoxNumber(self->mLayout,
                                            (char*)"txt_now_val01",
                                            CtrlRemote_TouchBitByArg(0x7fc));
    setLayoutTextBoxNumber(self->mLayout,
                                            (char*)"txt_now_val02",
                                            CtrlRemote_TouchBitByArg(0x7fd));

    // Reserved-slot count string: 0x19 if any reserved slot is populated.
    s32 cnt = 0x1a;
    for (u32 i = 0; i < 5; i++) {
        u8 idx = (u8)i;
        if (CtrlRemote_TouchBitByArg(idx + 0x804) == 1) {
            cnt = 0x19;
            break;
        }
    }
    char* s2b6 = BdatTouchStringCell((char*)"MNU_col6", (char*)"name", cnt);
    LayoutSetTextBoxFmtValue(self->mLayout, (char*)"txt_now06", s2b6, 0);
}

// col6FillSlots - refresh the item-box layout for a player slot: write the
// four player-count labels, then either (count < 5) fill the per-slot item
// panes from the item table, or blank them; an out-of-range slot blanks
// everything.
extern "C" void col6FillSlots(CCol6System* self, s32 playerIdx) {
    // Four player-count labels (0x7fe + 0..3).
    for (s32 i = 0; i < 4; i++) {
        char buf[0x20];
        sprintf(buf, (char*)"txt_num%02d", i + 1);
        setLayoutTextBoxNumber(self->mLayout, buf,
                                                CtrlRemote_TouchBitByArg(i + 0x7fe));
    }

    if (playerIdx < 4) {
        s32 f = (s32)CtrlRemote_TouchBitByArg((u32)(playerIdx + 0x7fe));
        char* str = BdatTouchStringCell((char*)"MNU_col6", (char*)"help",
                                  f + playerIdx * 6 + 0x1f);
        LayoutSetTextBoxFmtValue(self->mLayout, (char*)"txt_comment", str, 0);

        if (f < 5) {
            u32 n = (u32)(f + playerIdx * 5 + 1);
            u16 id = BdatGetU16ByTableKey((char*)"CL6_materiallist",
                                   (char*)"money", n);
            setLayoutTextBoxNumber(
                self->mLayout, (char*)"txt_gold01", id * 100);
            char* s3 = BdatTouchStringCell((char*)"MNU_item",
                                     (char*)"name", 3);
            LayoutSetTextBoxFmtValue(self->mLayout, (char*)"txt_gold00", s3, 0);
            setLayoutTextBoxNumber(
                self->mLayout, (char*)"txt_gold01_01", CItemBlock_getPtr20E8());
            char* s4 = BdatTouchStringCell((char*)"MNU_item",
                                     (char*)"name", 3);
            LayoutSetTextBoxFmtValue(self->mLayout, (char*)"txt_gold00_01", s4, 0);

            // Per-slot rows: 4 format strings + a lookup for the count value.
            for (s32 i = 0; i < 5; i++) {
                u8 idx = (u8)(i + 1);
                char buf148[0x20];
                sprintf(buf148, (char*)"txt_material%02d", idx);
                char buf128[0x20];
                sprintf(buf128, (char*)"txt_mat_val%02d", idx);
                char buf108[0x20];
                sprintf(buf108, (char*)"txt_mat_val%02d_01", idx);
                char bufE8[0x20];
                sprintf(bufE8, (char*)"material%d", idx);
                u16 id2 = BdatGetU16ByTableKey((char*)"CL6_materiallist", bufE8, n);
                if (id2 != 0) {
                    BdatGetItemType(id2);
                    BdatGetItemId(id2);
                    char* name = BuildItemNameStr(id2);
                    char bufC8[0x20];
                    sprintf(bufC8, (char*)"number%d", idx);
                    u8 v = (u8)BdatGetU8ByTableKey((char*)"CL6_materiallist", bufC8,
                                             n);
                    LayoutSetTextBoxFmtValue(self->mLayout, buf148, name, 0);
                    setLayoutTextBoxNumber(self->mLayout,
                                                            buf128, v);
                    u32 cnt = CItem_sumFamilyByte6(id2);
                    setLayoutTextBoxNumber(self->mLayout,
                                                            buf108, cnt);
                } else {
                    LayoutSetTextBoxFmtValue(self->mLayout, buf148,
                                  (char*)"", 0);
                    LayoutSetTextBoxFmtValue(self->mLayout, buf128,
                                  (char*)"", 0);
                    LayoutSetTextBoxFmtValue(self->mLayout, buf108,
                                  (char*)"", 0);
                }
            }
        } else {
            LayoutSetTextBoxFmtValue(self->mLayout, (char*)"txt_gold00",
                          (char*)"", 0);
            LayoutSetTextBoxFmtValue(self->mLayout, (char*)"txt_gold01",
                          (char*)"", 0);
            LayoutSetTextBoxFmtValue(self->mLayout, (char*)"txt_gold00_01",
                          (char*)"", 0);
            LayoutSetTextBoxFmtValue(self->mLayout, (char*)"txt_gold01_01",
                          (char*)"", 0);
            for (s32 i = 0; i < 5; i++) {
                u8 idx = (u8)(i + 1);
                char bufA8[0x20];
                sprintf(bufA8, (char*)"txt_material%02d", idx);
                char buf88[0x20];
                sprintf(buf88, (char*)"txt_mat_val%02d", idx);
                char buf68[0x20];
                sprintf(buf68, (char*)"txt_mat_val%02d_01", idx);
                LayoutSetTextBoxFmtValue(self->mLayout, bufA8,
                              (char*)"", 0);
                LayoutSetTextBoxFmtValue(self->mLayout, buf88,
                              (char*)"", 0);
                LayoutSetTextBoxFmtValue(self->mLayout, buf68,
                              (char*)"", 0);
            }
        }
    } else {
        LayoutSetTextBoxFmtValue(self->mLayout, (char*)"txt_comment",
                      (char*)"", 0);
        LayoutSetTextBoxFmtValue(self->mLayout, (char*)"txt_gold00",
                      (char*)"", 0);
        LayoutSetTextBoxFmtValue(self->mLayout, (char*)"txt_gold01",
                      (char*)"", 0);
        LayoutSetTextBoxFmtValue(self->mLayout, (char*)"txt_gold00_01",
                      (char*)"", 0);
        LayoutSetTextBoxFmtValue(self->mLayout, (char*)"txt_gold01_01",
                      (char*)"", 0);
        for (s32 i = 0; i < 5; i++) {
            u8 idx = (u8)(i + 1);
            char buf48[0x20];
            sprintf(buf48, (char*)"txt_material%02d", idx);
            char buf28[0x20];
            sprintf(buf28, (char*)"txt_mat_val%02d", idx);
            char buf8[0x20];
            sprintf(buf8, (char*)"txt_mat_val%02d_01", idx);
            LayoutSetTextBoxFmtValue(self->mLayout, buf48, (char*)"", 0);
            LayoutSetTextBoxFmtValue(self->mLayout, buf28, (char*)"", 0);
            LayoutSetTextBoxFmtValue(self->mLayout, buf8, (char*)"", 0);
        }
    }
}

// Col6_SyncBoxSlot_0EE4 - item-box sync ladder: for a given item slot (<4) with a
// populated table row, format per-slot sub-ids, resolve the item instances and
// reconcile counts, calling the instance-sync virtual or the delta adjuster
// when the counted stack differs from the table value. The first parameter
// (r3) is unused by the retail body - the slot index arrives in r4.
void Col6_SyncBoxSlot_0EE4(u32 unused, s32 arg) {
    if (arg >= 4) return;
    s32 f = (s32)CtrlRemote_TouchBitByArg((u32)(arg + 0x7fe));
    if (f >= 5) return;
    // Retail accumulates arg*4 into a temp (compound +=), then forms
    // n = f + temp + 1 so the adds land in n's callee-saved home register.
    s32 tmp = arg * 4;
    tmp += arg;
    s32 n = f + tmp + 1;
    u16 id = (u16)BdatGetU16ByTableKey(
        &lbl_eu_80502050[0x2cc], &lbl_eu_80502050[0x2dd],
        (u32)n);
    CItemBlock_setCount((s32)CItemBlock_getPtr20E8() - (s32)id * 100);

    char buf1[0x20];
    char buf2[0x20];
    for (s32 i = 0; i < 5; i++) {
        u8 idx = (u8)(i + 1);
        sprintf(buf1, &lbl_eu_80502050[0x320], idx);
        u16 id2 = (u16)BdatGetU16ByTableKey(
            &lbl_eu_80502050[0x2cc], buf1, (u32)n);
        if (id2 == 0) continue;
        sprintf(buf2, &lbl_eu_80502050[0x32b], idx);
        u8 v = (u8)BdatGetU8ByTableKey(
            &lbl_eu_80502050[0x2cc], buf2, (u32)n);
        void* item = CItem_findRecByFamily(id2);
        if (item == 0) continue;
        s32 count = (s32)CItem_sumFamilyByte6(id2);
        if (count == (s32)v) {
            CItemImpl* inst =
                CItem_initItemImplInstances((CItemData*)item);
            inst->vf10((CItemData*)item);
        } else {
            CItemData_initFromFamily(item, id2, (u32)(count - v));
        }
    }
}

// Col6_ValidateBox_1024 - validate the item box for the player index at +0xA5: the
// total owned count must cover the needed quantity (table id * 100), and each
// of the five sub-slots must have its item present with a sufficient count.
extern "C" int Col6_ValidateBox_1024(CCol6System* self) {
    u8 f = (u8)CtrlRemote_TouchBitByArg((u32)((s8)self->mPadA5_0 + 0x7fe));
    u16 id = BdatGetU16ByTableKey(
        &lbl_eu_80502050[0x2cc], &lbl_eu_80502050[0x2dd],
        (u32)((s32)f + (s8)self->mPadA5_0 * 5 + 1));
    if ((u32)CItemBlock_getPtr20E8() < (u32)id * 100) {
        return 0;
    }

    char buf1[0x20];
    char buf2[0x20];
    for (s32 i = 0; i < 5; i++) {
        u8 sub = (u8)(i + 1);
        sprintf(buf1, &lbl_eu_80502050[0x320], sub);
        u16 id2 = (u16)BdatGetU16ByTableKey(
            &lbl_eu_80502050[0x2cc], buf1,
            (u32)((s32)f + (s8)self->mPadA5_0 * 5 + 1));
        if (id2 == 0) continue;
        sprintf(buf2, &lbl_eu_80502050[0x32b], sub);
        u8 v = BdatGetU8ByTableKey(
            &lbl_eu_80502050[0x2cc], buf2,
            (u32)((s32)f + (s8)self->mPadA5_0 * 5 + 1));
        void* item = CItem_findRecByFamily(id2);
        if (item == 0) return 0;
        if ((s32)CItem_sumFamilyByte6(id2) < (s32)v) return 0;
    }
    return 1;
}

// col6PadHandle - item-box input handling: up opens the box for player 1 (or
// flips the anim pair for other players), down flips the anim pair, and the
// select/confirm pad bits scroll the player index with wrap.
void col6PadHandle(CCol6System* self) {
    CCol6PadData* pad = (CCol6PadData*)getCfPadData__Q22cf13CfGameManagerFv();
    int up, down, sels, confirm;
    if (cf::CfGameManager::isClassicController(-1) != 0) {
        sels    = (pad->mField104 & 0x8004) != 0;
        confirm = (pad->mField104 & 0x10000) | (pad->mField104 & 0x8);
        up      = (pad->mField04 >> 21) & 1;
        down    = (pad->mField04 >> 22) & 1;
    } else {
        sels    = (pad->mField104 & 0x8004) != 0;
        confirm = (pad->mField104 & 0x10000) | (pad->mField104 & 0x8);
        up      = (pad->mField04 >> 4) & 1;
        down    = (pad->mField04 >> 5) & 1;
    }

    if (up) {
        if ((s8)self->mPadA5_0 == 0) {
            // Player 1: open the box (all four anims on) and refresh it.
            self->mFieldA4 = 6;
            self->mLayout->SetAnimationEnable(self->mAnim9C, 0);
            self->mLayout->SetAnimationEnable(self->mAnim90, 0);
            self->mLayout->SetAnimationEnable(self->mAnim94, 0);
            self->mLayout->SetAnimationEnable(self->mAnim98, 1);
            col6FillSlots(self, 0);
            playUISound(0x6d);
        } else {
            // Other players: switch the anim pair.
            self->mFieldA4 = 4;
            self->mLayout->SetAnimationEnable(self->mAnim98, 0);
            self->mLayout->SetAnimationEnable(self->mAnim9C, 0);
            self->mLayout->SetAnimationEnable(self->mAnim90, 0);
            self->mLayout->SetAnimationEnable(self->mAnim94, 1);
        }
        self->mCur1.SetVisible(0);
        if (self->mPadA5_0 == 2) self->mPadA5_0 = 3;
        playUISound(3);
    } else if (down) {
        self->mFieldA4 = 4;
        self->mLayout->SetAnimationEnable(self->mAnim98, 0);
        self->mLayout->SetAnimationEnable(self->mAnim9C, 0);
        self->mLayout->SetAnimationEnable(self->mAnim90, 0);
        self->mLayout->SetAnimationEnable(self->mAnim94, 1);
        self->mCur1.SetVisible(0);
        self->mPadA5_0 = 3;
        playUISound(6);
    } else if (sels) {
        // Player index down (wrap to 2).
        self->mPadA5_0 = self->mPadA5_0 - 1;
        if ((s8)self->mPadA5_0 < 0) self->mPadA5_0 = 2;
        if (reinterpret_cast<CSysWin*>(&self->mSysWin1)->isOpened() != 0) {
            u8 tmp[0x18];
            CSysWin::getPaneScreenPos(reinterpret_cast<nw4r::math::VEC3*>(tmp), reinterpret_cast<CSysWin*>(&self->mSysWin1), self->mPadA5_1);
            self->mCur2.setRootPaneTranslate(
                reinterpret_cast<nw4r::math::VEC3*>(tmp));
        } else {
            char buf[0x20];
            if (self->mFieldA4 >= 6) {
                sprintf(buf, (char*)"nul_sel%02d",
                        (s8)self->mPadA5_0 + 1);
            } else {
                sprintf(buf, (char*)"txt_%02d", (s8)self->mPadA5_0 + 1);
            }
            nw4r::lyt::Pane* pane1 = self->mLayout->GetRootPane()->FindPaneByName(buf, true);
            nw4r::lyt::Pane* pane2 = self->mLayout->GetRootPane()->FindPaneByName((char*)"nul_proportion", true);
            nw4r::math::VEC3 vec;
            ScalePaneWorldPos(
                &vec, pane1, pane2,
                self->mLayout->GetRootPane());
            self->mCur1.setRootPaneTranslate(&vec);
        }
        playUISound(1);
    } else if (confirm != 0) {
        // Player index up (wrap to 0).
        self->mPadA5_0 = self->mPadA5_0 + 1;
        if ((s8)self->mPadA5_0 > 2) self->mPadA5_0 = 0;
        if (reinterpret_cast<CSysWin*>(&self->mSysWin1)->isOpened() != 0) {
            u8 tmp[0x18];
            CSysWin::getPaneScreenPos(reinterpret_cast<nw4r::math::VEC3*>(tmp), reinterpret_cast<CSysWin*>(&self->mSysWin1), self->mPadA5_1);
            self->mCur2.setRootPaneTranslate(
                reinterpret_cast<nw4r::math::VEC3*>(tmp));
        } else {
            char buf[0x20];
            if (self->mFieldA4 >= 6) {
                sprintf(buf, (char*)"nul_sel%02d",
                        (s8)self->mPadA5_0 + 1);
            } else {
                sprintf(buf, (char*)"txt_%02d", (s8)self->mPadA5_0 + 1);
            }
            nw4r::lyt::Pane* pane1 = self->mLayout->GetRootPane()->FindPaneByName(buf, true);
            nw4r::lyt::Pane* pane2 = self->mLayout->GetRootPane()->FindPaneByName((char*)"nul_proportion", true);
            nw4r::math::VEC3 vec;
            ScalePaneWorldPos(
                &vec, pane1, pane2,
                self->mLayout->GetRootPane());
            self->mCur1.setRootPaneTranslate(&vec);
        }
        playUISound(1);
    }
}

// col6SwitchBox - item-box input handling (5-player): up on player 5 opens
// the box; other up/down inputs switch players or open the per-player box;
// select/confirm scroll the player index with wrap.
void col6SwitchBox(CCol6System* self) {
    CCol6PadData* pad = (CCol6PadData*)getCfPadData__Q22cf13CfGameManagerFv();
    int up, down, sels, confirm;
    if (cf::CfGameManager::isClassicController(-1) != 0) {
        sels    = (pad->mField104 & 0x8004) != 0;
        confirm = (pad->mField104 & 0x10000) | (pad->mField104 & 0x8);
        up      = (pad->mField04 >> 21) & 1;
        down    = (pad->mField04 >> 22) & 1;
    } else {
        sels    = (pad->mField104 & 0x8004) != 0;
        confirm = (pad->mField104 & 0x10000) | (pad->mField104 & 0x8);
        up      = (pad->mField04 >> 4) & 1;
        down    = (pad->mField04 >> 5) & 1;
    }

    if (up) {
        s32 pid = (s8)self->mPadA5_0;
        if (pid == 4) {
            // Player 5: open the box (all four anims on).
            self->mFieldA4 = 9;
            self->mLayout->SetAnimationEnable(self->mAnim90, 0);
            self->mLayout->SetAnimationEnable(self->mAnim94, 0);
            self->mLayout->SetAnimationEnable(self->mAnim9C, 0);
            self->mLayout->SetAnimationEnable(self->mAnim98, 1);
            self->mCur1.SetVisible(0);
            playUISound(3);
        } else if (CtrlRemote_TouchBitByArg(pid + 0x7fe) < 5) {
            if (Col6_ValidateBox_1024(self) != 0) {
                // Fill window 1 with the player's box name and open it.
                char* str1 = BdatTouchStringCell((char*)"MNU_col6",
                                           (char*)"name",
                                           pid + 0x48);
                char* str2 = BdatTouchStringCell((char*)"MNU_col6",
                                           (char*)"name", 7);
                char* str3 = BdatTouchStringCell((char*)"MNU_col6",
                                           (char*)"name", 8);
                reinterpret_cast<CSysWin*>(&self->mSysWin1)->setKindContent(reinterpret_cast<char*>(str1), 0);
                reinterpret_cast<CSysWin*>(&self->mSysWin1)->setTwoTextValues(reinterpret_cast<char*>(str2), reinterpret_cast<char*>(str3));
                reinterpret_cast<CSysWin*>(&self->mSysWin1)->applyKind(0);
                reinterpret_cast<CSysWin*>(&self->mSysWin1)->openPhase1();
                self->mCur1.SetVisible(1);
                self->mFieldA4 = 0xa;
                playUISound(3);
            } else {
                playUISound(5);
            }
        } else {
            // Box full: show the window-2 message.
            char* str = BdatTouchStringCell((char*)"MNU_col6", (char*)"name",
                                      0xc);
            reinterpret_cast<CSysWin*>(&self->mSysWin2)->setKindContent(reinterpret_cast<char*>(str), 0);
            reinterpret_cast<CSysWin*>(&self->mSysWin2)->applyKind(1);
            reinterpret_cast<CSysWin*>(&self->mSysWin2)->openPhase1();
            self->mFieldA4 = 0x1d;
            playUISound(5);
        }
    } else if (down) {
        self->mFieldA4 = 9;
        self->mLayout->SetAnimationEnable(self->mAnim90, 0);
        self->mLayout->SetAnimationEnable(self->mAnim94, 0);
        self->mLayout->SetAnimationEnable(self->mAnim9C, 0);
        self->mLayout->SetAnimationEnable(self->mAnim98, 1);
        self->mCur1.SetVisible(0);
        playUISound(6);
    } else if (sels) {
        // Player index down (wrap to 4) and refresh the box.
        self->mPadA5_0 = self->mPadA5_0 - 1;
        if ((s8)self->mPadA5_0 < 0) self->mPadA5_0 = 4;
        col6FillSlots(self, (s8)self->mPadA5_0);
        if (reinterpret_cast<CSysWin*>(&self->mSysWin1)->isOpened() != 0) {
            u8 tmp[0x18];
            CSysWin::getPaneScreenPos(reinterpret_cast<nw4r::math::VEC3*>(tmp), reinterpret_cast<CSysWin*>(&self->mSysWin1), self->mPadA5_1);
            self->mCur2.setRootPaneTranslate(
                reinterpret_cast<nw4r::math::VEC3*>(tmp));
        } else {
            char buf[0x20];
            if (self->mFieldA4 >= 6) {
                sprintf(buf, (char*)"nul_sel%02d",
                        (s8)self->mPadA5_0 + 1);
            } else {
                sprintf(buf, (char*)"txt_%02d", (s8)self->mPadA5_0 + 1);
            }
            nw4r::lyt::Pane* pane1 = self->mLayout->GetRootPane()->FindPaneByName(buf, true);
            nw4r::lyt::Pane* pane2 = self->mLayout->GetRootPane()->FindPaneByName((char*)"nul_proportion", true);
            nw4r::math::VEC3 vec;
            ScalePaneWorldPos(
                &vec, pane1, pane2,
                self->mLayout->GetRootPane());
            self->mCur1.setRootPaneTranslate(&vec);
        }
        playUISound(1);
    } else if (confirm != 0) {
        // Player index up (wrap to 0) and refresh the box.
        self->mPadA5_0 = self->mPadA5_0 + 1;
        if ((s8)self->mPadA5_0 > 4) self->mPadA5_0 = 0;
        col6FillSlots(self, (s8)self->mPadA5_0);
        if (reinterpret_cast<CSysWin*>(&self->mSysWin1)->isOpened() != 0) {
            u8 tmp[0x18];
            CSysWin::getPaneScreenPos(reinterpret_cast<nw4r::math::VEC3*>(tmp), reinterpret_cast<CSysWin*>(&self->mSysWin1), self->mPadA5_1);
            self->mCur2.setRootPaneTranslate(
                reinterpret_cast<nw4r::math::VEC3*>(tmp));
        } else {
            char buf[0x20];
            if (self->mFieldA4 >= 6) {
                sprintf(buf, (char*)"nul_sel%02d",
                        (s8)self->mPadA5_0 + 1);
            } else {
                sprintf(buf, (char*)"txt_%02d", (s8)self->mPadA5_0 + 1);
            }
            nw4r::lyt::Pane* pane1 = self->mLayout->GetRootPane()->FindPaneByName(buf, true);
            nw4r::lyt::Pane* pane2 = self->mLayout->GetRootPane()->FindPaneByName((char*)"nul_proportion", true);
            nw4r::math::VEC3 vec;
            ScalePaneWorldPos(
                &vec, pane1, pane2,
                self->mLayout->GetRootPane());
            self->mCur1.setRootPaneTranslate(&vec);
        }
        playUISound(1);
    }
}

// col6WinScroll - item-box window interaction: the select/confirm pad bits
// scroll the slot counter (with wrap), the up/down buttons open the box for
// the current player; window-open updates reposition the cursor.
void col6WinScroll(CCol6System* self) {
    CCol6PadData* pad = (CCol6PadData*)getCfPadData__Q22cf13CfGameManagerFv();
    int up, down, sels, confirm;
    if (cf::CfGameManager::isClassicController(-1) != 0) {
        sels    = (pad->mField104 & 0x8004) != 0;
        confirm = (pad->mField104 & 0x10000) | (pad->mField104 & 0x8);
        up      = (pad->mField04 >> 21) & 1;
        down    = (pad->mField04 >> 22) & 1;
    } else {
        sels    = (pad->mField104 & 0x8004) != 0;
        confirm = (pad->mField104 & 0x10000) | (pad->mField104 & 0x8);
        up      = (pad->mField04 >> 4) & 1;
        down    = (pad->mField04 >> 5) & 1;
    }

    if (sels) {
        // Scroll the slot counter down (wrap to 1), reposition the cursor.
        self->mPadA5_1 = self->mPadA5_1 - 1;
        if ((s8)self->mPadA5_1 < 0) self->mPadA5_1 = 1;
        if (reinterpret_cast<CSysWin*>(&self->mSysWin1)->isOpened() != 0) {
            u8 tmp[0x18];
            CSysWin::getPaneScreenPos(reinterpret_cast<nw4r::math::VEC3*>(tmp), reinterpret_cast<CSysWin*>(&self->mSysWin1), self->mPadA5_1);
            self->mCur2.setRootPaneTranslate(
                reinterpret_cast<nw4r::math::VEC3*>(tmp));
        } else {
            char buf[0x20];
            if (self->mFieldA4 >= 6) {
                sprintf(buf, (char*)"nul_sel%02d",
                        (s8)self->mPadA5_0 + 1);
            } else {
                sprintf(buf, (char*)"txt_%02d", (s8)self->mPadA5_0 + 1);
            }
            nw4r::lyt::Pane* pane1 = self->mLayout->GetRootPane()->FindPaneByName(buf, true);
            nw4r::lyt::Pane* pane2 = self->mLayout->GetRootPane()->FindPaneByName((char*)"nul_proportion", true);
            nw4r::math::VEC3 vec;
            ScalePaneWorldPos(
                &vec, pane1, pane2,
                self->mLayout->GetRootPane());
            self->mCur1.setRootPaneTranslate(&vec);
        }
        playUISound(1);
    } else if (confirm != 0) {
        // Scroll the slot counter up (wrap to 0).
        self->mPadA5_1 = self->mPadA5_1 + 1;
        if ((s8)self->mPadA5_1 > 1) self->mPadA5_1 = 0;
        if (reinterpret_cast<CSysWin*>(&self->mSysWin1)->isOpened() != 0) {
            u8 tmp[0x18];
            CSysWin::getPaneScreenPos(reinterpret_cast<nw4r::math::VEC3*>(tmp), reinterpret_cast<CSysWin*>(&self->mSysWin1), self->mPadA5_1);
            self->mCur2.setRootPaneTranslate(
                reinterpret_cast<nw4r::math::VEC3*>(tmp));
        } else {
            char buf[0x20];
            if (self->mFieldA4 >= 6) {
                sprintf(buf, (char*)"nul_sel%02d",
                        (s8)self->mPadA5_0 + 1);
            } else {
                sprintf(buf, (char*)"txt_%02d", (s8)self->mPadA5_0 + 1);
            }
            nw4r::lyt::Pane* pane1 = self->mLayout->GetRootPane()->FindPaneByName(buf, true);
            nw4r::lyt::Pane* pane2 = self->mLayout->GetRootPane()->FindPaneByName((char*)"nul_proportion", true);
            nw4r::math::VEC3 vec;
            ScalePaneWorldPos(
                &vec, pane1, pane2,
                self->mLayout->GetRootPane());
            self->mCur1.setRootPaneTranslate(&vec);
        }
        playUISound(1);
    } else if (up) {
        // Open the box for the current player (reset cursor 2).
        self->mFieldA4 = 0xc;
        self->mCur2.SetVisible(0);
        reinterpret_cast<CSysWin*>(&self->mSysWin1)->advancePhase3();
        playUISound(3);
    } else if (down) {
        self->mFieldA4 = 0xc;
        self->mPadA5_1 = 1;
        self->mCur2.SetVisible(0);
        reinterpret_cast<CSysWin*>(&self->mSysWin1)->advancePhase3();
        playUISound(6);
    }
}

// col6DoOpenBox - open the item box: bump the player count and the two
// summary counters from the per-player table, seed the TaskLOD/audio slots
// (v1/v2/v3 chains identical to initMpfSystem's own seeding), write the camera
/// vector block from the s16-keyed lookup, then refresh the layout labels
// (0x10f/0x11d/0x2b6) and the item box for the current player.
extern "C" void col6DoOpenBox(CCol6System* self) {
    CCol6Fade* fade = getFadeMenu();
    if (fade == 0) return;
    if (isFadeActive(fade) != 0) return;

    // Bump this player's count and the two summary counters.
    u32 f = CtrlRemote_TouchBitByArg((u32)((s8)self->mPadA5_0 + 0x7fe));
    CtrlRemote_SetSharedBit((u32)((s8)self->mPadA5_0 + 0x7fe), f + 1);
    u32 n = f + (s8)self->mPadA5_0 * 5 + 1;
    u8 a = (u8)BdatGetU8ByTableKey((char*)"CL6_uplist", (char*)"rev_LV", n);
    CtrlRemote_SetSharedBit(0x7fc, CtrlRemote_TouchBitByArg(0x7fc) + a);
    u8 b = (u8)BdatGetU8ByTableKey((char*)"CL6_uplist", (char*)"rev_people", n);
    CtrlRemote_SetSharedBit(0x7fd, CtrlRemote_TouchBitByArg(0x7fd) + b);

    // Seed the TaskLOD/audio slots for this player's three id ranges.
    UnkClass_80083298* gm = getGameSubManager__Q22cf13CfGameManagerFv();
    if (gm != 0) {
        u8 v1 = (u8)(((s8)self->mPadA5_0 + 1) * 10 + f + 1);
        activateLOD__8CTaskLODFv(v1);
        ScnData_FwdB7D9C(getScnHandle(), v1);
        forwardMpfCallB__17UnkClass_8047BB54Fv(&gm->field_0xF0, v1);

        u8 v2 = (u8)(f + 0x3d);
        activateLOD__8CTaskLODFv(v2);
        ScnData_FwdB7D9C(getScnHandle(), v2);
        forwardMpfCallB__17UnkClass_8047BB54Fv(&gm->field_0xF0, v2);

        u8 v3 = (u8)(f + 0x47);
        deactivateLOD__8CTaskLODFv(v3);
        ScnData_FwdB7DD4(getScnHandle(), v3);
        forwardMpfCallC__17UnkClass_8047BB54Fv(&gm->field_0xF0, v3);

        if (v2 == 0x3d) {
            activateLOD__8CTaskLODFv(0x55);
            ScnData_FwdB7D9C(getScnHandle(), 0x55);
            forwardMpfCallB__17UnkClass_8047BB54Fv(&gm->field_0xF0, 0x55);
        }
        if (v2 == 0x3e) {
            activateLOD__8CTaskLODFv(0x56);
            ScnData_FwdB7D9C(getScnHandle(), 0x56);
            forwardMpfCallB__17UnkClass_8047BB54Fv(&gm->field_0xF0, 0x56);
        }
        if (v2 == 0x3d) {
            activateLOD__8CTaskLODFv(0x57);
            ScnData_FwdB7D9C(getScnHandle(), 0x57);
            forwardMpfCallB__17UnkClass_8047BB54Fv(&gm->field_0xF0, 0x57);
        }
        if (v2 == 0x3d) {
            activateLOD__8CTaskLODFv(0x58);
            ScnData_FwdB7D9C(getScnHandle(), 0x58);
            forwardMpfCallB__17UnkClass_8047BB54Fv(&gm->field_0xF0, 0x58);
        }
        if (CtrlRemote_TouchBitByArg(0x20) >= 0x16e) {
            activateLOD__8CTaskLODFv(0x59);
            ScnData_FwdB7D9C(getScnHandle(), 0x59);
            forwardMpfCallB__17UnkClass_8047BB54Fv(&gm->field_0xF0, 0x59);
            if (v1 == 0xe) {
                activateLOD__8CTaskLODFv(0x5a);
                ScnData_FwdB7D9C(getScnHandle(), 0x5a);
                forwardMpfCallB__17UnkClass_8047BB54Fv(&gm->field_0xF0, 0x5a);
            }
        }
        if (v2 == 0x3d) {
            activateLOD__8CTaskLODFv(0x5b);
            ScnData_FwdB7D9C(getScnHandle(), 0x5b);
            forwardMpfCallB__17UnkClass_8047BB54Fv(&gm->field_0xF0, 0x5b);
        }
        if (v2 == 0x3d) {
            activateLOD__8CTaskLODFv(0x5c);
            ScnData_FwdB7D9C(getScnHandle(), 0x5c);
            forwardMpfCallB__17UnkClass_8047BB54Fv(&gm->field_0xF0, 0x5c);
        }
        if (v2 == 0x3e) {
            activateLOD__8CTaskLODFv(0x5d);
            ScnData_FwdB7D9C(getScnHandle(), 0x5d);
            forwardMpfCallB__17UnkClass_8047BB54Fv(&gm->field_0xF0, 0x5d);
        }
        if (v2 == 0x3f) {
            activateLOD__8CTaskLODFv(0x5e);
            ScnData_FwdB7D9C(getScnHandle(), 0x5e);
            forwardMpfCallB__17UnkClass_8047BB54Fv(&gm->field_0xF0, 0x5e);
        }
        if (v2 == 0x40) {
            activateLOD__8CTaskLODFv(0x5f);
            ScnData_FwdB7D9C(getScnHandle(), 0x5f);
            forwardMpfCallB__17UnkClass_8047BB54Fv(&gm->field_0xF0, 0x5f);
        }
        if (v2 == 0x40) {
            activateLOD__8CTaskLODFv(0x60);
            ScnData_FwdB7D9C(getScnHandle(), 0x60);
            forwardMpfCallB__17UnkClass_8047BB54Fv(&gm->field_0xF0, 0x60);
        }
        if (v2 == 0x41) {
            activateLOD__8CTaskLODFv(0x61);
            ScnData_FwdB7D9C(getScnHandle(), 0x61);
            forwardMpfCallB__17UnkClass_8047BB54Fv(&gm->field_0xF0, 0x61);
        }
        if (v2 == 0x41) {
            activateLOD__8CTaskLODFv(0x62);
            ScnData_FwdB7D9C(getScnHandle(), 0x62);
            forwardMpfCallB__17UnkClass_8047BB54Fv(&gm->field_0xF0, 0x62);
        }

        if (getLODData__8CTaskLODFv(0x2d)) {
            deactivateLOD__8CTaskLODFv(0x50);
            ScnData_FwdB7DD4(getScnHandle(), 0x50);
            forwardMpfCallC__17UnkClass_8047BB54Fv(&gm->field_0xF0, 0x50);
        }
        if (getLODData__8CTaskLODFv(0xe)) {
            deactivateLOD__8CTaskLODFv(0x51);
            ScnData_FwdB7DD4(getScnHandle(), 0x51);
            forwardMpfCallC__17UnkClass_8047BB54Fv(&gm->field_0xF0, 0x51);
        }
        if (getLODData__8CTaskLODFv(0x2a)) {
            deactivateLOD__8CTaskLODFv(0x52);
            ScnData_FwdB7DD4(getScnHandle(), 0x52);
            forwardMpfCallC__17UnkClass_8047BB54Fv(&gm->field_0xF0, 0x52);
        }
        if (getLODData__8CTaskLODFv(0x22)) {
            deactivateLOD__8CTaskLODFv(0x53);
            ScnData_FwdB7DD4(getScnHandle(), 0x53);
            forwardMpfCallC__17UnkClass_8047BB54Fv(&gm->field_0xF0, 0x53);
        }
        if (getLODData__8CTaskLODFv(0x23)) {
            deactivateLOD__8CTaskLODFv(0x54);
            ScnData_FwdB7DD4(getScnHandle(), 0x54);
            forwardMpfCallC__17UnkClass_8047BB54Fv(&gm->field_0xF0, 0x54);
        }
        if (getLODData__8CTaskLODFv(0x3e)) {
            deactivateLOD__8CTaskLODFv(0x55);
            ScnData_FwdB7DD4(getScnHandle(), 0x55);
            forwardMpfCallC__17UnkClass_8047BB54Fv(&gm->field_0xF0, 0x55);
        }
        if (getLODData__8CTaskLODFv(0x2d)) {
            deactivateLOD__8CTaskLODFv(0x57);
            ScnData_FwdB7DD4(getScnHandle(), 0x57);
            forwardMpfCallC__17UnkClass_8047BB54Fv(&gm->field_0xF0, 0x57);
        }
        if (getLODData__8CTaskLODFv(0x2d)) {
            deactivateLOD__8CTaskLODFv(0x58);
            ScnData_FwdB7DD4(getScnHandle(), 0x58);
            forwardMpfCallC__17UnkClass_8047BB54Fv(&gm->field_0xF0, 0x58);
        }
        if (getLODData__8CTaskLODFv(0xe)) {
            deactivateLOD__8CTaskLODFv(0x59);
            ScnData_FwdB7DD4(getScnHandle(), 0x59);
            forwardMpfCallC__17UnkClass_8047BB54Fv(&gm->field_0xF0, 0x59);
        }
        if (getLODData__8CTaskLODFv(0xb)) {
            deactivateLOD__8CTaskLODFv(0x5b);
            ScnData_FwdB7DD4(getScnHandle(), 0x5b);
            forwardMpfCallC__17UnkClass_8047BB54Fv(&gm->field_0xF0, 0x5b);
        }
        if (getLODData__8CTaskLODFv(0x15)) {
            deactivateLOD__8CTaskLODFv(0x5c);
            ScnData_FwdB7DD4(getScnHandle(), 0x5c);
            forwardMpfCallC__17UnkClass_8047BB54Fv(&gm->field_0xF0, 0x5c);
        }
        if (getLODData__8CTaskLODFv(0xc)) {
            deactivateLOD__8CTaskLODFv(0x5d);
            ScnData_FwdB7DD4(getScnHandle(), 0x5d);
            forwardMpfCallC__17UnkClass_8047BB54Fv(&gm->field_0xF0, 0x5d);
        }
        if (getLODData__8CTaskLODFv(0xd)) {
            deactivateLOD__8CTaskLODFv(0x5e);
            ScnData_FwdB7DD4(getScnHandle(), 0x5e);
            forwardMpfCallC__17UnkClass_8047BB54Fv(&gm->field_0xF0, 0x5e);
        }
        if (getLODData__8CTaskLODFv(0xe)) {
            deactivateLOD__8CTaskLODFv(0x5f);
            ScnData_FwdB7DD4(getScnHandle(), 0x5f);
            forwardMpfCallC__17UnkClass_8047BB54Fv(&gm->field_0xF0, 0x5f);
        }
        if (getLODData__8CTaskLODFv(0x2c)) {
            deactivateLOD__8CTaskLODFv(0x60);
            ScnData_FwdB7DD4(getScnHandle(), 0x60);
            forwardMpfCallC__17UnkClass_8047BB54Fv(&gm->field_0xF0, 0x60);
        }
        if (getLODData__8CTaskLODFv(0xf)) {
            deactivateLOD__8CTaskLODFv(0x61);
            ScnData_FwdB7DD4(getScnHandle(), 0x61);
            forwardMpfCallC__17UnkClass_8047BB54Fv(&gm->field_0xF0, 0x61);
        }
        if (getLODData__8CTaskLODFv(0x19)) {
            deactivateLOD__8CTaskLODFv(0x62);
            ScnData_FwdB7DD4(getScnHandle(), 0x62);
            forwardMpfCallC__17UnkClass_8047BB54Fv(&gm->field_0xF0, 0x62);
        }
    }

    // Remaining audio/quest seeds from the per-player table.
    u8 c1 = (u8)BdatGetU8ByTableKey((char*)"CL6_uplist", (char*)"rev_effflag1", n);
    u8 c2 = (u8)BdatGetU8ByTableKey((char*)"CL6_uplist", (char*)"rev_selflag", n);
    if (c1 != 0) CtrlRemote_SetSharedBit(c1 + 0x278a, 1);
    if (c2 != 0) CtrlRemote_SetSharedBit(c2 + 0x798, 1);
    u16 d = BdatGetU16ByTableKey((char*)"CL6_uplist", (char*)"rev_qstflag", n);
    if (d != 0) CtrlRemote_SetSharedBit(d + 0x220, 0xc8);
    u16 e = BdatGetU16ByTableKey((char*)"CL6_uplist", (char*)"rev_sound", n);
    self->mFieldA2 = cf::CfSoundMan::playActorSound(1, e, 0, 0, 1.0f);

    // Camera/vector block: every value is the s16 lookup result cast to float
    // (MWCC emits the 2^52 int->double conversion for (f32)(s16)); two slots
    // are byte-sized lookups via BdatGetU8ByTableKey.
    self->mCamPos.x = (f32)(s16)BdatGetS16ByTableKey(
        (char*)"CL6_uplist", (char*)"rev_cam1x", n);
    self->mCamPos.y = (f32)(s16)BdatGetS16ByTableKey(
        (char*)"CL6_uplist", (char*)"rev_cam1y", n);
    self->mCamPos.z = (f32)(s16)BdatGetS16ByTableKey(
        (char*)"CL6_uplist", (char*)"rev_cam1z", n);
    self->mCamLookAt.x = (f32)(s16)BdatGetS16ByTableKey(
        (char*)"CL6_uplist", (char*)"rev_cam1rx", n);
    self->mCamLookAt.y = (f32)(s16)BdatGetS16ByTableKey(
        (char*)"CL6_uplist", (char*)"rev_cam1ry", n);
    self->mVec168.x = (f32)(s16)BdatGetS16ByTableKey(
        (char*)"CL6_uplist", (char*)"rev_cam2x", n);
    self->mVec168.y = (f32)(s16)BdatGetS16ByTableKey(
        (char*)"CL6_uplist", (char*)"rev_cam2y", n);
    self->mVec168.z = (f32)(s16)BdatGetS16ByTableKey(
        (char*)"CL6_uplist", (char*)"rev_cam2z", n);
    self->mVec174.x = (f32)(s16)BdatGetS16ByTableKey(
        (char*)"CL6_uplist", (char*)"rev_cam2rx", n);
    self->mVec174.y = (f32)(s16)BdatGetS16ByTableKey(
        (char*)"CL6_uplist", (char*)"rev_cam2ry", n);
    self->mField180 = (f32)(s16)BdatGetS16ByTableKey(
        (char*)"CL6_uplist", (char*)"rev_cam2flm", n);
    self->mField184 = (u8)BdatGetU8ByTableKey(
        (char*)"CL6_uplist", (char*)"rev_cam2key", n);
    self->mVec188.x = (f32)(s16)BdatGetS16ByTableKey(
        (char*)"CL6_uplist", (char*)"rev_cam3x", n);
    self->mVec188.y = (f32)(s16)BdatGetS16ByTableKey(
        (char*)"CL6_uplist", (char*)"rev_cam3y", n);
    self->mVec188.z = (f32)(s16)BdatGetS16ByTableKey(
        (char*)"CL6_uplist", (char*)"rev_cam3z", n);
    self->mVec194.x = (f32)(s16)BdatGetS16ByTableKey(
        (char*)"CL6_uplist", (char*)"rev_cam3rx", n);
    self->mVec194.y = (f32)(s16)BdatGetS16ByTableKey(
        (char*)"CL6_uplist", (char*)"rev_cam3ry", n);
    self->mVec1A0.x = (f32)(s16)BdatGetS16ByTableKey(
        (char*)"CL6_uplist", (char*)"rev_cam4x", n);
    self->mVec1A0.y = (f32)(s16)BdatGetS16ByTableKey(
        (char*)"CL6_uplist", (char*)"rev_cam4y", n);
    self->mVec1A0.z = (f32)(s16)BdatGetS16ByTableKey(
        (char*)"CL6_uplist", (char*)"rev_cam4z", n);
    self->mVec1AC.x = (f32)(s16)BdatGetS16ByTableKey(
        (char*)"CL6_uplist", (char*)"rev_cam4rx", n);
    self->mVec1AC.y = (f32)(s16)BdatGetS16ByTableKey(
        (char*)"CL6_uplist", (char*)"rev_cam4ry", n);
    self->mField1B8 = (f32)(s16)BdatGetS16ByTableKey(
        (char*)"CL6_uplist", (char*)"rev_cam4flm", n);
    self->mField1BC = (u8)BdatGetU8ByTableKey(
        (char*)"CL6_uplist", (char*)"rev_cam4key", n);

    // Player counts on the two slots.
    setLayoutTextBoxNumber(self->mLayout,
                                            (char*)"txt_now_val01",
                                            CtrlRemote_TouchBitByArg(0x7fc));
    setLayoutTextBoxNumber(self->mLayout,
                                            (char*)"txt_now_val02",
                                            CtrlRemote_TouchBitByArg(0x7fd));

    // Reserved-slot count string: 0x19 if any reserved slot is populated.
    s32 cnt = 0x1a;
    for (u32 i = 0; i < 5; i++) {
        u8 idx = (u8)i;
        if (CtrlRemote_TouchBitByArg(idx + 0x804) == 1) {
            cnt = 0x19;
            break;
        }
    }
    char* s2b6 = BdatTouchStringCell((char*)"MNU_col6", (char*)"name", cnt);
    LayoutSetTextBoxFmtValue(self->mLayout, (char*)"txt_now06", s2b6, 0);

    col6FillSlots(self, (s8)self->mPadA5_0);
    self->mFlagA1 = 1;
    self->mFieldA4 = 0xf;
}

// Col6Win2_2C40 - fill and open window 2 for the current player: resolve the
// count/name strings for the player index at +0xA5, format the banner, set
// the window text, activate it and play the open sound.
void Col6Win2_2C40(CCol6System* self) {
    if (isCameraReady__Q22cf13CfGameManagerFv() != 0) {
        return;
    }

    self->mFieldA4 = 0x1c;
    char* tbl = lbl_eu_80502050;
    // Single expression so MWCC keeps p*5 in a scratch register and folds
    // the count add into the named temp (retail add r28, r0, r3 shape).
    u32 f = CtrlRemote_TouchBitByArg((u32)((s8)self->mPadA5_0 + 0x7fe));
    u32 n = (s8)self->mPadA5_0 * 5 + f;
    u8 a = (u8)BdatGetU8ByTableKey(&tbl[0x334], &tbl[0x33f], n);
    u8 b = (u8)BdatGetU8ByTableKey(&tbl[0x334], &tbl[0x346], n);

    char* str0 = BdatTouchStringCell(tbl, &tbl[0x9], (s8)self->mPadA5_0 + 0x4e);
    char* str1 =
        BdatTouchStringCell(tbl, &tbl[0x9], (s8)self->mPadA5_0 * 5 + f + 0x53);
    char* str2 = BdatTouchStringCell(tbl, &tbl[0x9], 0x78);
    char* str3 = BdatTouchStringCell(tbl, &tbl[0x9], 0x79);

    ml::FixStr<256> buf;
    Col6FormatBufferString(buf.mString, &tbl[0x480], str0, str1, str2, a, str3, b);
    reinterpret_cast<CSysWin*>(&self->mSysWin2)->setKindContent(reinterpret_cast<char*>(buf.mString), 0);
    reinterpret_cast<CSysWin*>(&self->mSysWin2)->applyKind(1);
    reinterpret_cast<CSysWin*>(&self->mSysWin2)->openPhase1();
    cf::CfSoundMan::playActorSound(1, 0x134, 0, 0, lbl_eu_80667558);
}

// Col6_OpenBox_2DB4 - "open item box" transition: when the fade is idle, set the
// window-2 state byte, play the open sound, move the camera to the stored
// position/look-at, then post the item-box entry messages for the current
// player-count bucket (0x802), optionally re-seeding the task/LOD audio.
void Col6_OpenBox_2DB4(CCol6System* self) {
    CCol6Fade* fade = getFadeMenu();
    if (fade == 0) {
        return;
    }
    if (isFadeActive(fade) != 0) {
        return;
    }

    self->mFieldA4 = 0x17;
    f32 vol = lbl_eu_80667578;
    CUICfManager_queueFadeMenu(2, 0, vol, vol, vol);
    notifyBattleSystem__Q22cf13CfGameManagerFv(9, 0, 0, 0, 0);
    isEffectReady__Q22cf13CfGameManagerFv(
        0, &self->mCamPos, &self->mCamLookAt, 0, lbl_eu_80667564);
    notifyCameraManager__Q22cf13CfGameManagerFv();

    UnkClass_80083298* gm;
    u8 count = (u8)CtrlRemote_TouchBitByArg(0x802);
    s32 n = (s32)count + 0x14;
    gm = getGameSubManager__Q22cf13CfGameManagerFv();
    if (gm != 0) {
        u8 v = (u8)(count + 0x32);
        activateLOD__8CTaskLODFv(v);
        ScnData_FwdB7D9C(getScnHandle(), v);
        forwardMpfCallB__17UnkClass_8047BB54Fv(&gm->field_0xF0, v);
    }

    u8 a = BdatGetU8ByTableKey(
        &lbl_eu_80502050[0x334], &lbl_eu_80502050[0x351], (u32)n);
    u8 b = BdatGetU8ByTableKey(
        &lbl_eu_80502050[0x334], &lbl_eu_80502050[0x35e], (u32)n);
    if (a != 0) {
        CtrlRemote_SetSharedBit((u32)a + 0x278a, 1);
    }
    if (b != 0) {
        CtrlRemote_SetSharedBit((u32)b + 0x798, 1);
    }
}

// col6Win2Count - window-2 player-count ladder: while window 2 is active,
// respond to the pad-confirm button; otherwise scan the player-count buckets
// (0x7fe..0x801 must all reach the 0x802 total), write the per-bucket camera/
// vector block from the s16-keyed lookup table, then post the 0x16 fade, the
// "player N joined" banner (count >= 5) or the per-slot threshold scan.
extern "C" void col6Win2Count(CCol6System* self) {
    if (reinterpret_cast<CSysWin*>(&self->mSysWin2)->isOpened() != 0) {
        if (reinterpret_cast<CSysWin*>(&self->mSysWin2)->isActive() != 0) {
            CCol6Pad* pad = getCurrentPad__Q22cf13CfGameManagerFv();
            u32 cond;
            if (cf::CfGameManager::isClassicController(-1) != 0) {
                cond = (pad->mPressedButtonFlags & 0x200000) != 0;
            } else {
                cond = (pad->mPressedButtonFlags & 0x10) != 0;
            }
            if (cond != 0) {
                reinterpret_cast<CSysWin*>(&self->mSysWin2)->advancePhase3();
            }
        }
    } else {
        // Player count for the current slot and the total bucket (0x802).
        s32 f = (s32)CtrlRemote_TouchBitByArg((u32)((s8)self->mPadA5_0 + 0x7fe));
        s32 ok = 1;
        s32 big = 0;
        u8 total = (u8)CtrlRemote_TouchBitByArg(0x802) + 1;

        // All four player slots must reach the total, else ok = 0.
        for (u32 i = 0; i < 4; i++) {
            if (CtrlRemote_TouchBitByArg((u8)i + 0x7fe) < (u32)total) {
                ok = 0;
                break;
            }
        }

        if (ok != 0) {
            CtrlRemote_SetSharedBit(0x802, total);
            // String-pool base held in a register for the whole block (retail
            // materializes it once; the per-pane offsets stay plain addis).
            char* base = (char*)"MNU_col6";
            u32 n = (u32)total + 0x14;

            // Camera/vector block: every value is the s16 lookup result cast to
            // float (MWCC emits the 2^52 int->double conversion for (f32)(s16));
            // two slots are byte-sized lookups via BdatGetU8ByTableKey.
            self->mCamPos.x = (f32)(s16)BdatGetS16ByTableKey(
                (char*)"CL6_uplist", (char*)"rev_cam1x", n);
            self->mCamPos.y = (f32)(s16)BdatGetS16ByTableKey(
                (char*)"CL6_uplist", (char*)"rev_cam1y", n);
            self->mCamPos.z = (f32)(s16)BdatGetS16ByTableKey(
                (char*)"CL6_uplist", (char*)"rev_cam1z", n);
            self->mCamLookAt.x = (f32)(s16)BdatGetS16ByTableKey(
                (char*)"CL6_uplist", (char*)"rev_cam1rx", n);
            self->mCamLookAt.y = (f32)(s16)BdatGetS16ByTableKey(
                (char*)"CL6_uplist", (char*)"rev_cam1ry", n);
            self->mVec168.x = (f32)(s16)BdatGetS16ByTableKey(
                (char*)"CL6_uplist", (char*)"rev_cam2x", n);
            self->mVec168.y = (f32)(s16)BdatGetS16ByTableKey(
                (char*)"CL6_uplist", (char*)"rev_cam2y", n);
            self->mVec168.z = (f32)(s16)BdatGetS16ByTableKey(
                (char*)"CL6_uplist", (char*)"rev_cam2z", n);
            self->mVec174.x = (f32)(s16)BdatGetS16ByTableKey(
                (char*)"CL6_uplist", (char*)"rev_cam2rx", n);
            self->mVec174.y = (f32)(s16)BdatGetS16ByTableKey(
                (char*)"CL6_uplist", (char*)"rev_cam2ry", n);
            self->mField180 = (f32)(s16)BdatGetS16ByTableKey(
                (char*)"CL6_uplist", (char*)"rev_cam2flm", n);
            self->mField184 = (u8)BdatGetU8ByTableKey(
                (char*)"CL6_uplist", (char*)"rev_cam2key", n);
            self->mVec188.x = (f32)(s16)BdatGetS16ByTableKey(
                (char*)"CL6_uplist", (char*)"rev_cam3x", n);
            self->mVec188.y = (f32)(s16)BdatGetS16ByTableKey(
                (char*)"CL6_uplist", (char*)"rev_cam3y", n);
            self->mVec188.z = (f32)(s16)BdatGetS16ByTableKey(
                (char*)"CL6_uplist", (char*)"rev_cam3z", n);
            self->mVec194.x = (f32)(s16)BdatGetS16ByTableKey(
                (char*)"CL6_uplist", (char*)"rev_cam3rx", n);
            self->mVec194.y = (f32)(s16)BdatGetS16ByTableKey(
                (char*)"CL6_uplist", (char*)"rev_cam3ry", n);
            self->mVec1A0.x = (f32)(s16)BdatGetS16ByTableKey(
                (char*)"CL6_uplist", (char*)"rev_cam4x", n);
            self->mVec1A0.y = (f32)(s16)BdatGetS16ByTableKey(
                (char*)"CL6_uplist", (char*)"rev_cam4y", n);
            self->mVec1A0.z = (f32)(s16)BdatGetS16ByTableKey(
                (char*)"CL6_uplist", (char*)"rev_cam4z", n);
            self->mVec1AC.x = (f32)(s16)BdatGetS16ByTableKey(
                (char*)"CL6_uplist", (char*)"rev_cam4rx", n);
            self->mVec1AC.y = (f32)(s16)BdatGetS16ByTableKey(
                (char*)"CL6_uplist", (char*)"rev_cam4ry", n);
            self->mField1B8 = (f32)(s16)BdatGetS16ByTableKey(
                (char*)"CL6_uplist", (char*)"rev_cam4flm", n);
            self->mField1BC = (u8)BdatGetU8ByTableKey(
                (char*)"CL6_uplist", (char*)"rev_cam4key", n);
        }

        // Banner if this player's count clears the 5-player threshold.
        if (f >= 5) big = 1;

        if (ok != 0) {
            self->mFieldA4 = 0x16;
            CUICfManager_queueFadeMenu(0, 0, -1.0f, -1.0f,
                          -1.0f);
        } else if (big != 0) {
            // "player N joined" banner in window 2.
            self->mFieldA4 = 0x23;
            char* s1 = BdatTouchStringCell((char*)"MNU_col6", (char*)"name",
                                     (s8)self->mPadA5_0 + 0x72);
            char* s2 = BdatTouchStringCell((char*)"MNU_col6", (char*)"name",
                                     0x77);
            ml::FixStr<128> buf;
            buf.format((char*)"%s<n>%s", s1, s2);
            reinterpret_cast<CSysWin*>(&self->mSysWin2)->setKindContent(reinterpret_cast<char*>(buf.mString), 0);
            reinterpret_cast<CSysWin*>(&self->mSysWin2)->openPhase1();
            cf::CfSoundMan::playActorSound(1, 0x134, 0, 0, lbl_eu_80667558);
        } else {
            // Per-slot threshold scan (same shape as CCol6Invite::Move).
            union {
                struct {
                    u32 w;
                    u8 b;
                };
                u8 bytes[5];
            } data;
            data.w = lbl_eu_80667540;
            data.b = lbl_eu_80667544;

            u8 result = (u8)CtrlRemote_TouchBitByArg(0x7fc);

            u8 found = 0;
            for (u32 i = 0; i < 5; i++) {
                u8 idx = (u8)i;
                if (result >= data.bytes[idx] &&
                    CtrlRemote_TouchBitByArg(idx + 0x804) == 0) {
                    found = 1;
                    break;
                }
            }

            if (found != 0) {
                union {
                    struct {
                        u32 w;
                        u8 b;
                    };
                    u8 bytes[5];
                } data2;
                union {
                    struct {
                        u32 w;
                        u8 b;
                    };
                    u8 bytes[5];
                } data3;
                data2.w = lbl_eu_80667548;
                data2.b = lbl_eu_8066754C;
                data3.w = lbl_eu_80667550;
                data3.b = lbl_eu_80667554;

                s16 val = (s16)CtrlRemote_TouchBitByArg(0x7fc);
                for (u32 i = 0; i < 5; i++) {
                    if (val >= data2.bytes[i] &&
                        CtrlRemote_TouchBitByArg(i + 0x804) == 0) {
                        sCol6QueuedWindowId = (s32)i;
                        CtrlRemote_SetSharedBit(i + 0x804, 1);
                        val = (s16)(val + (s8)data3.bytes[i]);
                        CtrlRemote_SetSharedBit(0x7fc, val);
                    }
                }
                self->mFieldA4 = 0x15;
                CUICfManager_queueFadeMenu(0, 0, -1.0f, -1.0f,
                              -1.0f);
            } else {
                self->mFieldA4 = 0x15;
                CUICfManager_queueFadeMenu(0, 0, -1.0f, -1.0f,
                              -1.0f);
            }
        }
    }
}

// Col6_Win2CountLadder_3614 - window-2 interaction / player-count ladder: while window 2
// is active, respond to the pad-confirm button (mode-dependent bit), posting
// the window's close action; otherwise scan the player-count buckets and show
// the invite banner for the first fully-populated bucket.
void Col6_Win2CountLadder_3614(CCol6System* self) {
    if (reinterpret_cast<CSysWin*>(&self->mSysWin2)->isOpened() != 0) {
        if (reinterpret_cast<CSysWin*>(&self->mSysWin2)->isActive() != 0) {
            CCol6Pad* pad = getCurrentPad__Q22cf13CfGameManagerFv();
            u32 cond;
            if (cf::CfGameManager::isClassicController(-1) != 0) {
                cond = (pad->mPressedButtonFlags & 0x200000) != 0;
            } else {
                cond = (pad->mPressedButtonFlags & 0x10) != 0;
            }
            if (cond != 0) {
                reinterpret_cast<CSysWin*>(&self->mSysWin2)->advancePhase3();
            }
        }
    } else {
        self->mFieldA4 = 0x21;
        // Find the highest bucket i in 5..1 whose four player slots
        // (0x7fe + 0..3) are all populated; default 0 (none).
        u32 sel = 0;
        s32 i = 5;
        while (i >= 1) {
            s32 cand = (s8)i;
            s32 ok = 1;
            for (u32 j = 0; j < 4; j++) {
                if ((u32)CtrlRemote_TouchBitByArg((u8)j + 0x7fe) < (u32)cand) {
                    ok = 0;
                    break;
                }
            }
            if (ok != 0) {
                sel = (u8)i;
                break;
            }
            i--;
        }
        if (sel != 5) {
            // Banner: "player N joined" (chars 0x76/0x77 are the slot names).
            char* s76 = BdatTouchStringCell(lbl_eu_80502050, &lbl_eu_80502050[0x9], 0x76);
            char* s77 = BdatTouchStringCell(lbl_eu_80502050, &lbl_eu_80502050[0x9], 0x77);
            ml::FixStr<128> buf;
            buf.format(&lbl_eu_80502050[0xb5], s76, s77);
            reinterpret_cast<CSysWin*>(&self->mSysWin2)->setKindContent(reinterpret_cast<char*>(buf.mString), 0);
            reinterpret_cast<CSysWin*>(&self->mSysWin2)->openPhase1();
            cf::CfSoundMan::playActorSound(1, 0x134, 0, 0, lbl_eu_80667558);
        }
    }
}

// Col6_Win2Interact_378C - window 2 interaction: while the second system window is
// active, respond to the pad-confirm button (mode-dependent bit), posting the
// window's close action; otherwise run the player-count ladder scan and post
// the join message for the first count bucket that is fully populated.
void Col6_Win2Interact_378C(CCol6System* self) {
    if (reinterpret_cast<CSysWin*>(&self->mSysWin2)->isOpened() != 0) {
        if (reinterpret_cast<CSysWin*>(&self->mSysWin2)->isActive() != 0) {
            CCol6Pad* pad = getCurrentPad__Q22cf13CfGameManagerFv();
            u32 cond;
            if (cf::CfGameManager::isClassicController(-1) != 0) {
                cond = (pad->mPressedButtonFlags & 0x200000) != 0;
            } else {
                cond = (pad->mPressedButtonFlags & 0x10) != 0;
            }
            if (cond != 0) {
                reinterpret_cast<CSysWin*>(&self->mSysWin2)->advancePhase3();
            }
        }
    } else {
        self->mFieldA4 = 0x22;
        // Find the highest bucket i in 5..1 whose four player slots
        // (0x7fe + 0..3) are all populated; default 0 (none).
        u32 sel = 0;
        s32 i = 5;
        while (i >= 1) {
            s32 cand = (s8)i;
            s32 ok = 1;
            for (u32 j = 0; j < 4; j++) {
                if ((u32)CtrlRemote_TouchBitByArg((u8)j + 0x7fe) < (u32)cand) {
                    ok = 0;
                    break;
                }
            }
            if (ok != 0) {
                sel = (u8)i;
                break;
            }
            i--;
        }
        if (sel != 5) {
            u16 id = BdatGetU16ByTableKey(&lbl_eu_80502050[0xbd],
                                   &lbl_eu_80502050[0xc9], sel + 5);
            UIWin_CreateItemMulti((u32)id, 0, 0, 0, 0, 1, 0, 1, 0);
        }
    }
}

// col6Win2PerPl - per-player window-2 ladder: when the task is idle and the
// current player's count clears the 5-player threshold, post the join banner
// (player-specific string char); otherwise scan the per-slot threshold tables
// and (re)assign the join-slot global, then fade the screen.
void col6Win2PerPl(CCol6System* self) {
    if (GetItemMulti_IsActiveFlag() != 0) {
        return;
    }

    bool ge = (s32)CtrlRemote_TouchBitByArg((u32)((s8)self->mPadA5_0 + 0x7fe)) >= 5;
    if (ge != 0) {
        // Banner: "player N joined" (char 0x72+N is the player name).
        self->mFieldA4 = 0x23;
        char* s1 = BdatTouchStringCell((char*)"MNU_col6", (char*)"name",
                                 (s8)self->mPadA5_0 + 0x72);
        char* s2 = BdatTouchStringCell((char*)"MNU_col6", (char*)"name", 0x77);
        ml::FixStr<128> buf;
        buf.format((char*)"%s<n>%s", s1, s2);
        reinterpret_cast<CSysWin*>(&self->mSysWin2)->setKindContent(reinterpret_cast<char*>(buf.mString), 0);
        reinterpret_cast<CSysWin*>(&self->mSysWin2)->openPhase1();
        cf::CfSoundMan::playActorSound(1, 0x134, 0, 0, lbl_eu_80667558);
        return;
    }

    // 5-byte threshold table: u32 word + trailing byte (big-endian byte
    // order, read left-to-right).
    union {
        struct {
            u32 w;
            u8 b;
        };
        u8 bytes[5];
    } data;
    data.w = lbl_eu_80667540;
    data.b = lbl_eu_80667544;

    u8 result = (u8)CtrlRemote_TouchBitByArg(0x7fc);

    u8 found = 0;
    for (u32 i = 0; i < 5; i++) {
        u8 idx = (u8)i;
        if (result >= data.bytes[idx] && CtrlRemote_TouchBitByArg(idx + 0x804) == 0) {
            found = 1;
            break;
        }
    }

    if (found != 0) {
        // Per-slot tables: pairs of (u32 word, trailing byte) read
        // left-to-right as five bytes each.
        union {
            struct {
                u32 w;
                u8 b;
            };
            u8 bytes[5];
        } data2;
        union {
            struct {
                u32 w;
                u8 b;
            };
            u8 bytes[5];
        } data3;
        data2.w = lbl_eu_80667548;
        data2.b = lbl_eu_8066754C;
        data3.w = lbl_eu_80667550;
        data3.b = lbl_eu_80667554;

        s16 val = (s16)CtrlRemote_TouchBitByArg(0x7fc);
        for (u32 i = 0; i < 5; i++) {
            if (val >= data2.bytes[i] && CtrlRemote_TouchBitByArg(i + 0x804) == 0) {
                sCol6QueuedWindowId = (s32)i;
                CtrlRemote_SetSharedBit(i + 0x804, 1);
                val = (s16)(val + (s8)data3.bytes[i]);
                CtrlRemote_SetSharedBit(0x7fc, val);
            }
        }
        self->mFieldA4 = 0x15;
        CUICfManager_queueFadeMenu(0, 0, -1.0f, -1.0f, -1.0f);
    } else {
        // Retail keeps a copy of this store/fade in BOTH arms (the b-over-else
        // emits the extra branch); do not hoist it out of the if/else.
        self->mFieldA4 = 0x15;
        CUICfManager_queueFadeMenu(0, 0, -1.0f, -1.0f, -1.0f);
    }
}

// col6JoinCheck - window-2 player-count ladder: when the task is idle and the
// player count (0x802) clears the 5-player threshold, post the join banner;
// otherwise scan the per-slot threshold tables and (re)assign the join-slot
// global, then fade the screen (same scan shape as CCol6Invite::Move).
void col6JoinCheck(CCol6System* self) {
    if (GetItemMulti_IsActiveFlag() != 0) {
        return;
    }

    if ((s32)CtrlRemote_TouchBitByArg(0x802) >= 5) {
        // Banner: "player N joined" (chars 0x76/0x77 are the slot names).
        self->mFieldA4 = 0x25;
        char* s76 = BdatTouchStringCell((char*)"MNU_col6", (char*)"name", 0x76);
        char* s77 = BdatTouchStringCell((char*)"MNU_col6", (char*)"name", 0x77);
        ml::FixStr<128> buf;
        buf.format((char*)"%s<n>%s", s76, s77);
        reinterpret_cast<CSysWin*>(&self->mSysWin2)->setKindContent(reinterpret_cast<char*>(buf.mString), 0);
        reinterpret_cast<CSysWin*>(&self->mSysWin2)->openPhase1();
        cf::CfSoundMan::playActorSound(1, 0x134, 0, 0, lbl_eu_80667558);
        return;
    }

    // 5-byte threshold table: u32 word + trailing byte (big-endian byte
    // order, read left-to-right).
    union {
        struct {
            u32 w;
            u8 b;
        };
        u8 bytes[5];
    } data;
    data.w = lbl_eu_80667540;
    data.b = lbl_eu_80667544;

    u8 result = (u8)CtrlRemote_TouchBitByArg(0x7fc);

    // Retail keeps the "slot found" latch out of the callee-save pool: single
    // assignment per path via goto, so MWCC colors it into scratch r0.
    u32 found;
    u32 i;
    for (i = 0; i < 5; i++) {
        // (u8)index temp blocks MWCC's pointer strength-reduction; retail walks
        // the table base+index via lbzx.
        u8 idx = (u8)i;
        if (result >= data.bytes[idx] && CtrlRemote_TouchBitByArg(idx + 0x804) == 0) {
            found = 1;
            goto slotFound;
        }
    }
    found = 0;
slotFound:

    if (found != 0) {
        // Per-slot tables: pairs of (u32 word, trailing byte) read
        // left-to-right as five bytes each. (data3 declared first: MWCC
        // stacks same-size locals so the last-declared lands lowest.)
        union {
            struct {
                u32 w;
                u8 b;
            };
            u8 bytes[5];
        } data3;
        union {
            struct {
                u32 w;
                u8 b;
            };
            u8 bytes[5];
        } data2;
        data2.w = lbl_eu_80667548;
        data2.b = lbl_eu_8066754C;
        data3.w = lbl_eu_80667550;
        data3.b = lbl_eu_80667554;

        s16 val = (s16)CtrlRemote_TouchBitByArg(0x7fc);
        for (u32 j = 0; j < 5; j++) {
            if (val >= data2.bytes[j] && CtrlRemote_TouchBitByArg(j + 0x804) == 0) {
                sCol6QueuedWindowId = (s32)j;
                CtrlRemote_SetSharedBit(j + 0x804, 1);
                val = (s16)(val + (s8)data3.bytes[j]);
                CtrlRemote_SetSharedBit(0x7fc, val);
            }
        }
        self->mFieldA4 = 0x15;
        CUICfManager_queueFadeMenu(0, 0, -1.0f, -1.0f, -1.0f);
    } else {
        // Retail keeps a copy of this store/fade in BOTH arms (the b-over-else
        // emits the extra branch); do not hoist it out of the if/else.
        self->mFieldA4 = 0x15;
        CUICfManager_queueFadeMenu(0, 0, -1.0f, -1.0f, -1.0f);
    }
}

// CCol6Invite::~CCol6Invite()
// extern "C" free-function form (same retail shape as ~CCol6CheckBat):
// outer null-check covers the delete; nested double null-check (D2-inlined-
// into-D1 artifact) guards the CProcess base dtor; default -O4,p.
extern "C" void* __dt__11CCol6InviteFv(CCol6Invite* self, int flags) {
    if (self != 0) {
        if (self != 0) {
            if (self != 0) {
                __dt__8CProcessFv(reinterpret_cast<CProcess*>(self), 0);
            }
        }
        if (flags > 0) {
            operator delete(self);
        }
    }
    return self;
}

// CCol6Invite::Init() - set up the invite banner: resolve the three banner
// strings, bump the two invite counters in the global flag memory by the
// instance's byte args, then format the banner message and post it.
void CCol6Invite::Init() {
    char* str0 = BdatGetPtrDirect((const void*)::cf::CfBdat::spFldNpcListFileData,
                               &lbl_eu_80502050[0x9], mArg2);

    // Compound assignment keeps the running value in one register: mask,
    // add the byte flag in place, then mask for the store.
    u32 val = CtrlRemote_TouchBitByArg(0x7fc);
    val &= 0xFF;
    val += mArg3;
    CtrlRemote_SetSharedBit(0x7fc, (u8)val);
    val = CtrlRemote_TouchBitByArg(0x7fd);
    val &= 0xFF;
    val += mArg4;
    CtrlRemote_SetSharedBit(0x7fd, (u8)val);

    char* str1 = BdatTouchStringCell(lbl_eu_80502050, &lbl_eu_80502050[0x9], 0x7e);
    char* str2 = BdatTouchStringCell(lbl_eu_80502050, &lbl_eu_80502050[0x9], 0x78);
    char* str3 = BdatTouchStringCell(lbl_eu_80502050, &lbl_eu_80502050[0x9], 0x79);

    ml::FixStr<128> buf;
    buf.format(&lbl_eu_80502050[0x480], str0, str1, str2, mArg3, str3, mArg4);
    UIWin_CreateSysWin0(buf.mString, 0, 0);
}

// CCol6Invite::Move() - while no system menu is closing, re-run the invite
// slot scan: if the current player count (0x7fc) clears the table threshold
// and the matching slot is populated, walk the per-slot table to (re)assign
// the join-slot global (0x8066235C) and adjust the count by the signed
// per-slot delta; the 0x64 flag always ends set.
void CCol6Invite::Move() {
    if (SysWinGetSingleton() != 0) {
        return;
    }

    // 5-byte threshold tables (u32 word + trailing byte), read left-to-right
    // as five bytes each.
    union {
        struct {
            u32 w;
            u8 b;
        };
        u8 bytes[5];
    } t1, t3, t2;
    t1.w = lbl_eu_80667540;
    t1.b = lbl_eu_80667544;

    u8 found;
    for (u32 cnt = (u8)CtrlRemote_TouchBitByArg(0x7fc), i = 0; i < 5; i++) {
        if (cnt >= t1.bytes[(u8)i] && CtrlRemote_TouchBitByArg((u8)i + 0x804) == 0) {
            found = 1;
            goto scanned;
        }
    }
    found = 0;
scanned:

    if (found != 0) {
        t2.w = lbl_eu_80667548;
        t2.b = lbl_eu_8066754C;
        t3.w = lbl_eu_80667550;
        t3.b = lbl_eu_80667554;

        // Second scan: signed running total kept as s16 (re-extended on
        // every read); indexed table access (MWCC strength-reduces to
        // incrementing pointers, matching retail - same shape as
        // Col6_ReserveBoxSlot_D3A0).
        s16 total = CtrlRemote_TouchBitByArg(0x7fc);
        s32 j;
        u8* p = t2.bytes;
        for (j = 0; j < 5; j++) {
            if (total >= *p && CtrlRemote_TouchBitByArg(j + 0x804) == 0) {
                sCol6QueuedWindowId = j;
                CtrlRemote_SetSharedBit(j + 0x804, 1);
                total += (s8)t3.bytes[j];
                CtrlRemote_SetSharedBit(0x7fc, total);
            }
            p++;
        }
        mField64 = 1;
    } else {
        mField64 = 1;
    }
}
