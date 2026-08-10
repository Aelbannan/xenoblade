// Catalog TU for kyoshin/menu/CMenuBattleEnd
// Battle-end (result) screen task reconstruction.

#include "kyoshin/harness_catalog.hpp"

#include "kyoshin/code_80135FDC.hpp"
#include "monolib/lib/UnkClass_8045F564.hpp"
#include "monolib/scn/IScnRender.hpp"
#include "monolib/util/MemManager.hpp"
#include "monolib/work/CWorkThreadSystem.hpp"

#include "kyoshin/menu/CMenuBattleEnd.hpp"

void cbRenderBefore__14CMenuBattleEndFv(void*);

// ---------------------------------------------------------------------------
// Constructor (unmangled retail symbol). Runs CProcess's ctor then fills the
// CProcess-style region by hand (vtable, run-time ptmf hooks, state fields)
// and constructs the embedded MEM2 layout region.
// ---------------------------------------------------------------------------
extern "C" CMenuBattleEnd* __ct__CMenuBattleEnd(CMenuBattleEnd* self, CScn* scene,
                                                u8 mode, u32 param) {
    __ct__8CProcessFv((CProcess*)self);

    self->mVtab = (u32)lbl_eu_8052D238;

    // Post-increment pointer form: MWCC folds the base @l into a single
    // `lwzu` for the first load and keeps the base register for the rest;
    // the second triple re-reads the same elements through the folded
    // negative offsets (retail re-loads ptmf[0..2] after the first stores).
    const u32* ptmf = __ptmf_null;
    u32 p0, p1, p2, q0, q1, q2;
    p0 = *ptmf++;
    p1 = *ptmf++;
    self->mPtMf3C[1] = p1;
    self->mPtMf3C[0] = p0;
    p2 = *ptmf++;
    self->mPtMf3C[2] = p2;
    q0 = ptmf[-3];
    q1 = ptmf[-2];
    self->mPtMf48[1] = q1;
    self->mPtMf48[0] = q0;
    q2 = ptmf[-1];
    self->mPtMf48[2] = q2;

    self->mLayout = 0;
    self->mField58 = 0;
    self->mField5C = 0;
    self->mField60 = -1;
    self->mField64 = 0;
    self->mField65 = 0;
    self->mField66 = 0;
    self->mActive = 1;
    self->mField68 = 0;
    // Retail re-stores +0x10 with the dispatch vtable cluster base after the
    // class vtable (same double-store scheme as CMenuQstCnt); +0x24/+0xac are
    // the IWorkEvent / IScnRender dispatch slots inside that cluster.
    self->mVtab = (u32)lbl_eu_80537AB0;
    self->mWorkEventVt = (u32)lbl_eu_80537AB0 + 0x24;
    self->mScnRenderVt = (u32)lbl_eu_80537AB0 + 0xac;
    self->mScene = scene;

    __ct__17UnkClass_8045F564Fv(&self->mMemRegion[0]);

    self->mAnim88 = 0;
    self->mAnim8C = 0;
    self->mAnim90 = 0;
    self->mEntries[0].flag = 0;
    self->mEntries[0].param = 0;
    // Pointer loop over entries[1..3]: retail computes the trip count from the
    // end/start addresses and guards with a redundant cmplw/bge pair.
    for (UnkBattleEntry* p = &self->mEntries[1]; p < &self->mEntries[4]; p++) {
        p->flag = 0;
        p->param = 0;
    }

    self->mState = 1;
    self->mMode = mode;
    self->mParam = param;
    // Retail re-clears the whole queue unrolled after setting the state fields.
    self->mEntries[0].flag = 0;
    self->mEntries[0].param = 0;
    self->mEntries[1].flag = 0;
    self->mEntries[1].param = 0;
    self->mEntries[2].flag = 0;
    self->mEntries[2].param = 0;
    self->mEntries[3].flag = 0;
    self->mEntries[3].param = 0;
    return self;
}

// ---------------------------------------------------------------------------
// Destructor. The CProcess subobject is destroyed OOL (flag 0), the embedded
// MEM2 region runs its member dtor (flag -1), and the deleting-flag path calls
// operator delete. The inner re-test of `obj` is required: retail re-emits the
// compare and branches twice (MWCC keeps one beq per source-level if).
// ---------------------------------------------------------------------------
extern "C" CMenuBattleEnd* __dt__14CMenuBattleEndFv(CMenuBattleEnd* obj,
                                                    int flags) {
    if (obj != 0) {
        __dt__17UnkClass_8045F564Fv(&obj->mMemRegion[0], -1);
        if (obj != 0) {
            if (obj != 0) {
                __dt__8CProcessFv(obj, 0);
            }
        }
        if (flags > 0) {
            __dl__FPv(obj);
        }
    }
    return obj;
}

void CMenuBattleEnd::Init() {
    mtl::ALLOC_HANDLE handle = mtl::MemManager::getHandleMEM2();
    nw4r::lyt::ArcResourceAccessor* accessor;

    reinterpret_cast<UnkClass_8045F564*>(mMemRegion)
        ->createRegion((int)handle, 0x4000, lbl_eu_8050E6F8, 0);
    Class_8045F858 regionGuard(reinterpret_cast<UnkClass_8045F564*>(mMemRegion));

    accessor = func_801355F4();
    func_80136E84(&mLayout, accessor, &lbl_eu_8050E6F8[0xf]);

    accessor = func_801355F4();
    func_80136F08(mLayout, &mAnim88, accessor, (char*)&lbl_eu_8050E6F8[0x28]);

    accessor = func_801355F4();
    func_80136F08(mLayout, &mAnim8C, accessor, (char*)&lbl_eu_8050E6F8[0x48]);

    nw4r::lyt::Pane* rootPane = mLayout->GetRootPane();
    u32 fontResult = func_801355A0();
    func_8013676C(rootPane, fontResult);

    mLayout->UnbindAllAnimation();

    switch (mMode) {
    case 1:
        mAnim90 = mAnim88;
        func_80136910(mLayout, &lbl_eu_8050E6F8[0x68], (u8)mParam);
        func_80136910(mLayout, &lbl_eu_8050E6F8[0x74], (u8)mParam);
        break;
    case 2:
        mAnim90 = mAnim8C;
        func_80136910(mLayout, &lbl_eu_8050E6F8[0x80], (u8)mParam);
        func_80136910(mLayout, &lbl_eu_8050E6F8[0x8c], (u8)mParam);
        break;
    default:
        break;
    }

    if (mAnim90 != 0) {
        mLayout->BindAnimation(mAnim90);
        mLayout->SetAnimationEnable(mAnim90, true);
        mLayout->Animate(0);
        func_80138078(0x2c);
    }

    {
        IScnRender* cb = reinterpret_cast<IScnRender*>(this);
        if (this != 0) {
            cb = reinterpret_cast<IScnRender*>(&mScnRenderVt);
        }
        addRenderCB__4CScnFP10IScnRenderUlUl(mScene, cb, 0xa, 0);
    }

    reinterpret_cast<UnkClass_8045F564*>(mMemRegion)->func_8045F810();
}

void CMenuBattleEnd::Term() {
    waitForDrawDone__9CDeviceVIFv();
    IScnRender* cb = reinterpret_cast<IScnRender*>(this);
    if (this != 0) {
        cb = reinterpret_cast<IScnRender*>(&mScnRenderVt);
    }
    removeRenderCB__4CScnFP10IScnRender(mScene, cb);
    if (mLayout) {
        // Deleting-dtor dispatch: virtual ~Layout at vtable slot +8 (flag 1).
        delete mLayout;
        mLayout = 0;
    }
    lbl_eu_80664898 = 0;
    func_8045F778__17UnkClass_8045F564Fv(&mMemRegion[0]);
}

void CMenuBattleEnd::Move() {
    // Single short-circuit OR so MWCC emits: func test -> bne exit;
    // bit test -> beq continue / b exit (CSystemWindow::Move shape).
    if (CTaskGame::getInstance()->func_800426F0() ||
        (lbl_eu_80663E28 & 0x200000))
        return;
    if (!func_8013BE50()) return;
    if (mState == 1) {
        func_8026F95C(this);
    }
    mLayout->Animate(0);
}

void CMenuBattleEnd::cbRenderBefore() {
    // Same single-OR guard shape as Move / CSystemWindow::cbRenderBefore.
    if (CTaskGame::getInstance()->func_800426F0() ||
        (lbl_eu_80663E28 & 0x200000))
        return;
    if (!func_8013BE50()) return;
    GXSetZMode(GX_FALSE, GX_NEVER, GX_FALSE);
    // Raw-storage DrawInfo built/destroyed via the C-ABI ct/dt calls so the
    // scope-exit destructor is not virtual-dispatched (same scheme as
    // CSystemWindow::cbRenderBefore).
    u8 drawInfo[0x54];
    __ct__Q34nw4r3lyt8DrawInfoFv(&drawInfo[0]);
    func_80137250((nw4r::lyt::DrawInfo*)&drawInfo[0]);
    func_80137038(mLayout, (nw4r::lyt::DrawInfo*)&drawInfo[0], 0, 1);
    __dt__Q34nw4r3lyt8DrawInfoFv(&drawInfo[0], -1);
}

// ---------------------------------------------------------------------------
// func_8026F8B0 - task entry: if an instance already exists, queue the new
// request onto it; otherwise allocate (0xbc bytes on the work heap), construct
// and register the battle-end screen under `parent`.
// ---------------------------------------------------------------------------
extern "C" CMenuBattleEnd* func_8026F8B0(CProcess* parent, CScn* scene, u8 mode,
                                         u32 param) {
    if (lbl_eu_80664898 != 0) {
        func_8026FB0C(lbl_eu_80664898, mode, param);
        return 0;
    }
    CMenuBattleEnd* obj = (CMenuBattleEnd*)mtl::MemManager::allocate(
        0xbc, CWorkThreadSystem::getWorkMem());
    if (obj != 0) {
        // Reassign from the ctor return so obj stays in volatile r3 (retail
        // never spills it to a callee-saved register; a discarded ctor result
        // would force a 5th saved register and stmw prologue).
        obj = __ct__CMenuBattleEnd(obj, scene, mode, param);
    }
    lbl_eu_80664898 = obj;
    Regist__8CProcessFP8CProcessb(obj, parent, 0);
    return lbl_eu_80664898;
}

// ---------------------------------------------------------------------------
// func_8026F95C - drive the currently-playing animation; once its frame
// reaches the target, drain the next queued entry (if any) to the screen.
// ---------------------------------------------------------------------------
extern "C" void func_8026F95C(CMenuBattleEnd* obj) {
    if (func_80137444(obj->mAnim90, lbl_eu_80668990) == 0) return;

    obj->mLayout->Animate(0);
    obj->mLayout->UnbindAnimation(obj->mAnim90);
    obj->mAnim90->SetFrame(lbl_eu_80668994);
    obj->mAnim90 = 0;
    obj->mState = 1;

    // Retail walks the queue with a u8 counter (clrlslwi 24,3 index math);
    // applying an entry returns immediately, otherwise the tail clears state.
    for (u8 i = 0; i < 4; i++) {
        if (obj->mEntries[i].flag == 0) continue;

        switch (obj->mEntries[i].flag) {
        case 1:
            obj->mAnim90 = obj->mAnim88;
            func_80136910(obj->mLayout, &lbl_eu_8050E6F8[0x68], obj->mEntries[i].param);
            func_80136910(obj->mLayout, &lbl_eu_8050E6F8[0x74], obj->mEntries[i].param);
            break;
        case 2:
            obj->mAnim90 = obj->mAnim8C;
            func_80136910(obj->mLayout, &lbl_eu_8050E6F8[0x80], obj->mEntries[i].param);
            func_80136910(obj->mLayout, &lbl_eu_8050E6F8[0x8c], obj->mEntries[i].param);
            break;
        default:
            break;
        }

        if (obj->mAnim90 != 0) {
            obj->mLayout->BindAnimation(obj->mAnim90);
            obj->mLayout->SetAnimationEnable(obj->mAnim90, true);
            obj->mLayout->Animate(0);
            obj->mEntries[i].flag = 0;
            obj->mEntries[i].param = 0;
            func_80138078(0x2c);
            return;
        }
    }

    obj->mField64 = 1;
    obj->mState = 0;
}

// ---------------------------------------------------------------------------
// func_8026FB0C - append a {flag,param} entry to the queue, compacting the
// active entries to the front. If the queue is already full, nothing changes.
// ---------------------------------------------------------------------------
extern "C" void func_8026FB0C(CMenuBattleEnd* obj, u8 flag, u32 param) {
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

void func_8026FC90(void* self) { ((void(*)(void*))__dt__14CMenuBattleEndFv)((char*)self - 0x6c); }

void func_8026FC98(void* self) { ((void(*)(void*))cbRenderBefore__14CMenuBattleEndFv)((char*)self - 0x70); }

void func_8026FCA0(void* self) { ((void(*)(void*))__dt__14CMenuBattleEndFv)((char*)self - 0x70); }