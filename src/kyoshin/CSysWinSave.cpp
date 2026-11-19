// kyoshin/CSysWinSave - system window "save" process (singleton).

#define IWORK_EVENT_INLINE_DTOR
#define CTTASK_INLINE_DTOR
#define IUIWINDOW_INLINE_DTOR
#include "kyoshin/cf/CfGameManager.hpp"
#include "kyoshin/CSysWinSave.hpp"
#include "kyoshin/CSysWin.hpp"
#include "kyoshin/code_80135FDC_State.hpp"
// cf::CChainCombo's
// "cf::CChainCombo" type-name string belong to CChainCombo.o (retail .data
// 0x80538994 / .sdata 0x80662B08 / .rodata 0x8050FD50): the class is
// polymorphic with an inline dtor, so MWCC emits that weak vtable block next to
// the out-of-line constructor in CChainCombo.cpp. Nothing to spell here -- this
// unit's own .data run starts at 0x805389A0 with __vt__11CSysWinSave.
//
//
//
//
#include "monolib/util/MemManager.hpp"
#include "monolib/work/CWorkThreadSystem.hpp"
#include "monolib/device/CDeviceVI.hpp"
#include <new>
#include <revolution/gx/GXPixel.h>

// ---------------------------------------------------------------------------
// Producers of the save-window singleton.
// targets in this batch); must still emit their retail symbols.
// ---------------------------------------------------------------------------
extern "C" unsigned long SysWinSaveIsCreated() {
    return lbl_eu_80664A08 != 0;
}

// ---------------------------------------------------------------------------
// Target 3: SysWinSaveHandleInput - input handling for the save dialog's open state
// (called from Move while mFlagDC == 2). Reads the current cf pad data; the
// two confirm/cancel triggers come from different button bits depending on
// co-op vs single-player; an up/down D-pad input wraps the save-slot flag.
// ---------------------------------------------------------------------------
// C-linkage (retail symbol is unmangled SysWinSaveHandleInput), so the definition
// carries extern "C" and the call site in Move emits the retail reloc name
// (same convention as SysWinSelectHandleInput in CSysWinSelect.cpp).
// (memberized: SysWinSaveHandleInput -> CSysWinSave::HandleInput; declaration moved into class CSysWinSave)

void CSysWinSave::HandleInput() {
    CSysPadData* pad = (CSysPadData*)getCfPadData__Q22cf13CfGameManagerFv();
    int in1, in2;
    int sels, confirm;
    if (cf::CfGameManager::isClassicController(-1) != 0) {
        in1     = (pad->field_04 >> 21) & 1;
        in2     = (pad->field_04 >> 22) & 1;
        sels    = (pad->field_104 & 0x8004) != 0;
        confirm = (pad->field_104 >> 16) & 1;
        confirm |= (pad->field_104 >> 3) & 1;
        confirm = confirm != 0;
    } else {
        in1     = (pad->field_04 >> 4) & 1;
        in2     = (pad->field_04 >> 5) & 1;
        sels    = (pad->field_104 & 0x8004) != 0;
        confirm = (pad->field_104 >> 16) & 1;
        confirm |= (pad->field_104 >> 3) & 1;
        confirm = confirm != 0;
    }

    if (in1) {
        // Close the dialog without advancing the save slot.
        this->mFlagDC = 3;
        this->mSysWin.advancePhase3();
        this->mCur18.SetVisible(0);
        playUISound(3);
    } else if (in2) {
        // Close the dialog, marking the save slot as re-selected.
        this->mFlagDC = 3;
        this->mSysWin.advancePhase3();
        this->mCur18.SetVisible(0);
        this->mFlagDD = 1;
        playUISound(6);
    } else if (sels) {
        // Cursor up: decrement the slot index, wrapping to 1 when below 0.
        this->mFlagDD = this->mFlagDD - 1;
        if (this->mFlagDD < 0) this->mFlagDD = 1;
        u8 tmp[0x18]; // retail frame keeps a 0x18-byte position buffer
        CSysWin::getPaneScreenPos(reinterpret_cast<nw4r::math::VEC3*>(tmp), &this->mSysWin, this->mFlagDD);
        this->mCur18.setRootPaneTranslate(reinterpret_cast<nw4r::math::VEC3*>(tmp));
        playUISound(1);
    } else if (confirm) {
        // Cursor down: increment the slot index, wrapping over 1 to 0.
        this->mFlagDD = this->mFlagDD + 1;
        if (this->mFlagDD > 1) this->mFlagDD = 0;
        u8 tmp[0x18]; // retail frame keeps a 0x18-byte position buffer
        CSysWin::getPaneScreenPos(reinterpret_cast<nw4r::math::VEC3*>(tmp), &this->mSysWin, this->mFlagDD);
        this->mCur18.setRootPaneTranslate(reinterpret_cast<nw4r::math::VEC3*>(tmp));
        playUISound(1);
    }
}

// ---------------------------------------------------------------------------
// Target 1: CSysWinSave::Init - register the render callback, create the
// scratch region (guarded by a Class_8045F858 RAII temp), then rebuild the
// embedded CSysWin and CCur18 bodies by constructing stack temps and copying
// their non-vtable fields into the embedded storage.
// ---------------------------------------------------------------------------
void CSysWinSave::Init() {
    setPresentationFlag__Q22cf13CfGameManagerFv(true);

    IScnRender* render = this;
    mScene->addRenderCB(render, 0xd, 0);

    mMemRegion.createRegion(mtl::MemManager::getHandleMEM2(), 0x4000,
                            "CSysWinSave\0MNU_sysmes\0name", 0);

    // RAII scratch-region guard, released at the end of this scope.
    Class_8045F858 guard(&mMemRegion);

    // Rebuild the embedded CSysWin: construct a temp (+2), copy its body
    // (non-vtable fields, matching retail's memberwise lwz/stw+lbz/stb), then
    // destroy the temp and dispatch the layout-build virtual at vtable+0x88.
    {
    CSysWin tempW(2);
    CSysWinData* sw = reinterpret_cast<CSysWinData*>(&mSysWin);
    CSysWinData* tw = reinterpret_cast<CSysWinData*>(&tempW);
    sw->f_04 = tw->f_04;
    sw->f_08 = tw->f_08;
    sw->f_0c = tw->f_0c;
    sw->f_10 = tw->f_10;
    sw->f_14 = tw->f_14;
    sw->f_18 = tw->f_18;
    sw->f_1c = tw->f_1c;
    sw->f_20 = tw->f_20;
    sw->f_24 = tw->f_24;
    sw->f_28 = tw->f_28;
    sw->f_2c = tw->f_2c;
    sw->f_30 = tw->f_30;
    sw->f_34 = tw->f_34;
    sw->f_35 = tw->f_35;
    sw->f_36 = tw->f_36;
    sw->f_37 = tw->f_37;
    sw->f_38 = tw->f_38;
    sw->f_39 = tw->f_39;
    }
    reinterpret_cast<CSysWinView*>(&mSysWin)->v20();

    // Rebuild the embedded CCur18 cursor the same way (shared arc accessor).
    mCur18 = CCur18(CUICfManager_getArcResourceAccessor());
    mCur18.initLayout();

    mMemRegion.validateHeap();
}

// ---------------------------------------------------------------------------
// Target 2: CSysWinSave::Move - gate prefix (task busy / global render flag /
// scene active), then a 4-state machine controlling the save dialog:
//   0 load strings and ready the window; 1 wait active then update cursor;
//   2 dispatch input to SysWinSaveHandleInput; 3 closing transition.
// ---------------------------------------------------------------------------
void CSysWinSave::Move() {
    CTaskGame::getInstance();
    if (CTaskGame::isFlag01Set() || (gmPauseFlags & 0x200000))
        return;
    if (IsMenuState621F0() == 0) return;

    switch (mFlagDC) {
    case 0: {
        // Window just became ready: load the three save-panel messages and
        // start the open animation.
        if (mSysWin.isReady()) {
            mFlagDC = 1;
            const char* base = "CSysWinSave\0MNU_sysmes\0name";
            // s2 declared before s1 (assignments stay in retail's call order)
            // so MWCC's saved-register coloring lands s1 in r29 / s2 in r30.
            const char* s2;
            const char* s1;
            s1 = BdatTouchStringCell(base + 0xc, base + 0x17, 0x4e);
            s2 = BdatTouchStringCell(base + 0xc, base + 0x17, 0x4f);
            const char* s3 = BdatTouchStringCell(base + 0xc, base + 0x17, 0x50);
            mSysWin.setKindContent(reinterpret_cast<char*>((void*)s1), 0);
            mSysWin.setTwoTextValues(reinterpret_cast<char*>((void*)s2), reinterpret_cast<char*>((void*)s3));
            mSysWin.applyKind(0);
            mSysWin.openPhase1();
        }
        break;
    }
    case 1:
        // Window active: update the cursor from the stored slot flag.
        if (mSysWin.isActive()) {
            mFlagDC = 2;
            mCur18.SetVisible(1);
            u8 tmp[0x18]; // retail frame keeps a 0x18-byte position buffer
            CSysWin::getPaneScreenPos(reinterpret_cast<nw4r::math::VEC3*>(tmp), &mSysWin, mFlagDD);
            mCur18.setRootPaneTranslate(reinterpret_cast<nw4r::math::VEC3*>(tmp));
        }
        break;
    case 2:
        // Input handling for the open dialog.
        this->HandleInput();
        break;
    case 3:
        // Closing transition finished - hand control back to the UI.
        if (mSysWin.isActive()) {
            if (mFlagDD == 0)
                CUICfManager_queueSaveMenu(1, 1);
            mField67 = 0;
            mField64 = 1;
        }
        break;
    }

    mSysWin.dispatchPhase();
    mCur18.Update();
}

// ---------------------------------------------------------------------------
// Target 2: CSysWinSave::Term (us-802967ec)
// Wait for VI draw completion, release the CSysWin and its scratch region, run
// the CCur18 cursor-update virtual, clear the singleton, detach the render
// callback, and re-enable the cf load if the signed flag was set.
// ---------------------------------------------------------------------------
void CSysWinSave::Term() {
    CDeviceVI::waitForDrawDone();
    mSysWin.termLayout();
    mCur18.cleanup();
    mMemRegion.deleteRegion();
    lbl_eu_80664A08 = 0;

    IScnRender* render = this;
    mScene->removeRenderCB(render);

    if (mFlagDD != 0)
        setPresentationFlag__Q22cf13CfGameManagerFv(false);
}

// ---------------------------------------------------------------------------
// Target 3: CSysWinSave::~CSysWinSave (us-802965b4)
// Complete-object destructor: the compiler emits the subobject destruction in
// reverse construction order (CCur18@0xC4, CSysWin@0x88,
// UnkClass_8045F564@0x78), the CProcess base call (through the inlined
// CTTask<IUIWindow>/IUIWindow dtors) and the conditional operator delete.
// ---------------------------------------------------------------------------
CSysWinSave::~CSysWinSave() {}

// ---------------------------------------------------------------------------
// Target 4: CSysWinSave::cbRenderBefore (us-80296a3c)
// Gate prefix (task busy / global bit / scene-active) then draw the embedded
// CSysWin and CCur18 cursor through a stack DrawInfo.
// ---------------------------------------------------------------------------
void CSysWinSave::cbRenderBefore() {
    CTaskGame::getInstance();
    // Gate: when the task game is not ready or the global mode bit (0x200000)
    // is set, skip the render. The if-&&-goto body / goto end / end: return
    // chain keeps the body off the fallthrough so MWCC emits retail's
    // branch-over-branch: `bne end` for the first disjunct, `beq body; b end`
    // for the second (same scheme as CSysWinSelect::cbRenderBefore).
    if (CTaskGame::isFlag01Set() == 0 && (gmPauseFlags & 0x200000) == 0) {
        goto body;
    }
    goto end;
end:
    return;
body:
    if (IsMenuState621F0() == 0) {
        return;
    }
    GXSetZMode(GX_FALSE, GX_NEVER, GX_FALSE);
    nw4r::lyt::DrawInfo drawInfo;
    SetWideLocAdjust(&drawInfo);
    mSysWin.drawLayout(reinterpret_cast<nw4r::lyt::DrawInfo*>(&drawInfo));
    mCur18.DrawLayout(&drawInfo);
}

// ---------------------------------------------------------------------------
// Target 5: __ct__802944D8 (us-80296ad4)
// Allocate the CSysWinSave singleton (0xE0 bytes) from work memory, run the
// CProcess ctor, lay out the IUIWindow + Move region by hand, then construct
// the embedded UnkClass_8045F564 / CSysWin / CCur18 and register as a CProcess
// child of `registerParent`. Returns 0 when the singleton already exists.
// Regist is called even when the allocation/ctor failed, matching retail.
// ---------------------------------------------------------------------------
extern "C" CSysWinSave* __ct__802944D8(CProcess* registerParent, CScn* scene) {
    if (lbl_eu_80664A08 != 0)
        return 0;

    CSysWinSave* obj = new (mtl::MemManager::allocate(0xe0, CWorkThreadSystem::getWorkMem()))
        CSysWinSave(scene);
    lbl_eu_80664A08 = obj;
    reinterpret_cast<CProcess*>(obj)->Regist(registerParent, 0);
    return lbl_eu_80664A08;
}

// Retail data: __vt__11CSysWinSave (0xC0) + its 0x40 base list, both
// compiler-emitted (the "CSysWinSave" type-name string and the weak RTTI
// record come with them). cf::CChainCombo's vtable/RTTI/name string live in
// CChainCombo.o, before this run.
CSysWinSave* lbl_eu_80664A08 = 0;
u32 save_sbss_padA0C = 0; // (retail .sbss is 8B; content ignored by gate)
