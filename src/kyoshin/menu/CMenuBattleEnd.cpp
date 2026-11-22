// Catalog TU for kyoshin/menu/CMenuBattleEnd
// Battle-end (result) screen task reconstruction.

#define IWORK_EVENT_INLINE_DTOR
#define CTTASK_INLINE_DTOR
#define IUIWINDOW_INLINE_DTOR
#include <nw4r/lyt/lyt_drawInfo.h>
#include "kyoshin/harness_catalog.hpp"

#include "kyoshin/code_80135FDC.hpp"
#include "monolib/lib/UnkClass_8045F564.hpp"
#include "monolib/scn/IScnRender.hpp"
#include "monolib/util/MemManager.hpp"
#include "monolib/work/CWorkThreadSystem.hpp"

#include "kyoshin/menu/CMenuBattleEnd.hpp"

// ---------------------------------------------------------------------------
// CMenuBattleEnd (us-80271800)
// CProcess base ctor, then the inlined CTTask<IUIWindow>/IUIWindow ctor
// bodies (base vptr, null ptmf hooks, layout/state members), the composite +
// sub-vptrs, the embedded MEM2 region and the queue/state fields.
// ---------------------------------------------------------------------------
CMenuBattleEnd::CMenuBattleEnd(CScn* scene, u8 mode, u32 param)
    : mScene(scene), mMemRegion(), mAnim88(0), mAnim8C(0), mAnim90(0) {
    mEntries[0].flag = 0;
    mEntries[0].param = 0;
    // Pointer loop over entries[1..3]: retail computes the trip count from the
    // end/start addresses and guards with a redundant cmplw/bge pair.
    for (UnkBattleEntry* p = &mEntries[1]; p < &mEntries[4]; p++) {
        p->flag = 0;
        p->param = 0;
    }

    mState = 1;
    mMode = mode;
    mParam = param;

    // Retail re-clears the whole queue unrolled after setting the state fields.
    mEntries[0].flag = 0;
    mEntries[0].param = 0;
    mEntries[1].flag = 0;
    mEntries[1].param = 0;
    mEntries[2].flag = 0;
    mEntries[2].param = 0;
    mEntries[3].flag = 0;
    mEntries[3].param = 0;
}

// ---------------------------------------------------------------------------
// CMenuBattleEnd::~CMenuBattleEnd (us-80271950)
// Complete-object destructor: IUIWindow/CTTask/IWorkEvent/IScnRender dtors are
// inline (retail calls __dt__8CProcessFv directly), the embedded MEM2 region
// runs its member dtor, then the deleting-flag path calls operator delete.
// ---------------------------------------------------------------------------
CMenuBattleEnd::~CMenuBattleEnd() {}

void CMenuBattleEnd::Init() {
    mtl::ALLOC_HANDLE handle = mtl::MemManager::getHandleMEM2();
    nw4r::lyt::ArcResourceAccessor* accessor;

    mMemRegion.createRegion((int)handle, 0x4000, "CMenuBattleEnd", 0);
    Class_8045F858 regionGuard(&mMemRegion);

    accessor = CUICfManager_getArcResourceAccessor();
    buildLayout(&mLayout, accessor, "mf70_cf53_sideinfo.brlyt");

    accessor = CUICfManager_getArcResourceAccessor();
    bindLayoutAnimTransform(mLayout, &mAnim88, accessor,
                            "mf70_cf53_sideinfo_battle.brlan");

    accessor = CUICfManager_getArcResourceAccessor();
    bindLayoutAnimTransform(mLayout, &mAnim8C, accessor,
                            "mf70_cf53_sideinfo_renkei.brlan");

    nw4r::lyt::Pane* rootPane = mLayout->GetRootPane();
    u32 fontResult = (u32)getPackedFont();
    ApplyFontToTxtPanes(rootPane, (const nw4r::ut::Font*)fontResult);

    mLayout->UnbindAllAnimation();

    switch (mMode) {
    case 1:
        mAnim90 = mAnim88;
        setLayoutTextBoxNumber(mLayout, "txt_num_b00", (u8)mParam);
        setLayoutTextBoxNumber(mLayout, "txt_num_b01", (u8)mParam);
        break;
    case 2:
        mAnim90 = mAnim8C;
        setLayoutTextBoxNumber(mLayout, "txt_num_r00", (u8)mParam);
        setLayoutTextBoxNumber(mLayout, "txt_num_r01", (u8)mParam);
        break;
    default:
        break;
    }

    if (mAnim90 != 0) {
        mLayout->BindAnimation(mAnim90);
        mLayout->SetAnimationEnable(mAnim90, true);
        mLayout->Animate(0);
        playUISound(0x2c);
    }

    addRenderCB__4CScnFP10IScnRenderUlUl(mScene, this, 0xa, 0);

    mMemRegion.validateHeap();
}

void CMenuBattleEnd::Term() {
    waitForDrawDone__9CDeviceVIFv();
    removeRenderCB__4CScnFP10IScnRender(mScene, this);
    if (mLayout) {
        // Deleting-dtor dispatch: virtual ~Layout at vtable slot +8 (flag 1).
        delete mLayout;
        mLayout = 0;
    }
    CMenuBattleEnd::sInstance = 0;
    deleteRegion__17UnkClass_8045F564Fv(&mMemRegion);
}

void CMenuBattleEnd::Move() {
    // Single short-circuit OR so MWCC emits: func test -> bne exit;
    // bit test -> beq continue / b exit (CSystemWindow::Move shape).
    if (CTaskGame::getInstance()->isFlag01Set() ||
        (gmPauseFlags & 0x200000))
        return;
    if (!IsMenuState621F0()) return;
    if (mState == 1) {
        advanceBattleEndQueue(this);
    }
    mLayout->Animate(0);
}

void CMenuBattleEnd::cbRenderBefore(CScn*) {
    // Same single-OR guard shape as Move / CSystemWindow::cbRenderBefore.
    if (CTaskGame::getInstance()->isFlag01Set() ||
        (gmPauseFlags & 0x200000))
        return;
    if (!IsMenuState621F0()) return;
    GXSetZMode(GX_FALSE, GX_NEVER, GX_FALSE);
    nw4r::lyt::DrawInfo drawInfo;
    SetWideLocAdjust(&drawInfo);
    drawLayout(mLayout, &drawInfo, 0, 1);
}

// ---------------------------------------------------------------------------
// createBattleEndMenu - task entry: if an instance already exists, queue the new
// request onto it; otherwise allocate (0xbc bytes on the work heap), construct
// and register the battle-end screen under `parent`.
// ---------------------------------------------------------------------------
extern "C" CMenuBattleEnd* createBattleEndMenu(CProcess* parent, CScn* scene, u8 mode,
                                         u32 param) {
    if (CMenuBattleEnd::sInstance != 0) {
        queueBattleEndEntry(CMenuBattleEnd::sInstance, mode, param);
        return 0;
    }
    CMenuBattleEnd* obj =
        new (mtl::MemManager::allocate(0xbc, CWorkThreadSystem::getWorkMem()))
            CMenuBattleEnd(scene, mode, param);
    CMenuBattleEnd::sInstance = obj;
    Regist__8CProcessFP8CProcessb(obj, parent, 0);
    return CMenuBattleEnd::sInstance;
}

// ---------------------------------------------------------------------------
// advanceBattleEndQueue - drive the currently-playing animation; once its frame
// reaches the target, drain the next queued entry (if any) to the screen.
// ---------------------------------------------------------------------------
extern "C" void advanceBattleEndQueue(CMenuBattleEnd* obj) {
    if (advanceAnimTransform(obj->mAnim90, 1.0f) == 0) return;

    obj->mLayout->Animate(0);
    obj->mLayout->UnbindAnimation(obj->mAnim90);
    obj->mAnim90->SetFrame(0.0f);
    obj->mAnim90 = 0;
    obj->mState = 1;

    // Retail walks the queue with a u8 counter (clrlslwi 24,3 index math);
    // applying an entry returns immediately, otherwise the tail clears state.
    for (u8 i = 0; i < 4; i++) {
        UnkBattleEntry* e = &obj->mEntries[i];
        if (e->flag == 0) continue;

        switch (e->flag) {
        case 1:
            obj->mAnim90 = obj->mAnim88;
            setLayoutTextBoxNumber(obj->mLayout, "txt_num_b00", e->param);
            setLayoutTextBoxNumber(obj->mLayout, "txt_num_b01", e->param);
            break;
        case 2:
            obj->mAnim90 = obj->mAnim8C;
            setLayoutTextBoxNumber(obj->mLayout, "txt_num_r00", e->param);
            setLayoutTextBoxNumber(obj->mLayout, "txt_num_r01", e->param);
            break;
        default:
            break;
        }

        if (obj->mAnim90 != 0) {
            obj->mLayout->BindAnimation(obj->mAnim90);
            obj->mLayout->SetAnimationEnable(obj->mAnim90, true);
            obj->mLayout->Animate(0);
            e->flag = 0;
            e->param = 0;
            playUISound(0x2c);
            return;
        }
    }

    obj->mField64 = 1;
    obj->mState = 0;
}

// ---------------------------------------------------------------------------
// queueBattleEndEntry - append a {flag,param} entry to the queue, compacting the
// active entries to the front. If the queue is already full, nothing changes.
// ---------------------------------------------------------------------------
extern "C" void queueBattleEndEntry(CMenuBattleEnd* obj, u8 flag, u32 param) {
    UnkBattleEntry packed[4];
    packed[0].flag = 0;
    packed[0].param = 0;
    packed[1].flag = 0;
    packed[1].param = 0;
    packed[2].flag = 0;
    packed[2].param = 0;
    packed[3].flag = 0;
    packed[3].param = 0;
    u8 count = 0;

    // Retail is this 4-iteration loop unrolled by MWCC: entry[0]'s copy
    // constant-folds to `packed[0]` / `count = 1` (no clrlslwi index math),
    // while entries[1..3] use the general computed-index form.
    for (int i = 0; i < 4; i++) {
        if (obj->mEntries[i].flag != 0) {
            packed[count].flag = obj->mEntries[i].flag;
            packed[count].param = obj->mEntries[i].param;
            count++;
        }
    }

    if (count < 4) {
        for (u32 i = 0; i < 4; i++) {
            if (i >= count) {
                obj->mEntries[i].flag = 0;
                obj->mEntries[i].param = 0;
            } else {
                obj->mEntries[i].flag = packed[i].flag;
                obj->mEntries[i].param = packed[i].param;
            }
        }
        UnkBattleEntry& ap = obj->mEntries[count];
        ap.flag = flag;
        ap.param = param;
    }
}

// Retail data: __vt__14CMenuBattleEnd (0xC0) + its 0x40 base list, both
// compiler-emitted from the real class above (the "CMenuBattleEnd" type-name
// string and the weak __RTTI__14CMenuBattleEnd record come with them).
// name-guess: 0x80664898 is the 8-byte .sbss singleton slot between
// sInstance__17CMenuPassiveSkill and sInstance__14CMenuPlayAward.
CMenuBattleEnd* CMenuBattleEnd::sInstance;
u32 battleEnd_sbss_pad9C = 0; // (retail .sbss is 8B; content ignored by gate)
