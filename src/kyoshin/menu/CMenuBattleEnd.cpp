// Catalog TU for kyoshin/menu/CMenuBattleEnd
// Battle-end (result) screen task reconstruction.

#include "kyoshin/harness_catalog.hpp"

#include "kyoshin/code_80135FDC.hpp"
#include "monolib/lib/UnkClass_8045F564.hpp"
#include "monolib/scn/IScnRender.hpp"
#include "monolib/util/MemManager.hpp"

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

    u32* ptmf = __ptmf_null;
    self->mPtMf3C[0] = ptmf[0];
    self->mPtMf3C[1] = ptmf[1];
    self->mPtMf3C[2] = ptmf[2];
    self->mPtMf48[0] = ptmf[0];
    self->mPtMf48[1] = ptmf[1];
    self->mPtMf48[2] = ptmf[2];

    self->mLayout = 0;
    self->mField58 = 0;
    self->mField5C = 0;
    self->mField60 = -1;
    self->mField64 = 0;
    self->mField65 = 0;
    self->mField66 = 0;
    self->mActive = 1;
    self->mField68 = 0;
    // Dispatch handles for the secondary (non-inherited) virtual interfaces.
    self->mWorkEventVt = (u32)lbl_eu_80537AB0 + 0x24;
    self->mScnRenderVt = (u32)lbl_eu_80537AB0 + 0xac;
    self->mScene = scene;

    __ct__17UnkClass_8045F564Fv(&self->mMemRegion[0]);

    self->mAnim88 = 0;
    self->mAnim8C = 0;
    self->mAnim90 = 0;
    for (int i = 0; i < 4; i++) {
        self->mEntries[i].flag = 0;
        self->mEntries[i].param = 0;
    }

    self->mState = 1;
    self->mMode = mode;
    self->mParam = param;
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
    removeRenderCB__4CScnFP10IScnRender(
        mScene, reinterpret_cast<IScnRender*>(this ? &mScnRenderVt : (u32*)this));
    if (mLayout) {
        if (mLayout) {
            void* vtab = *(void**)mLayout;
            ((void (*)(void*, int))((void**)vtab)[0x8 / 4])(mLayout, 1);
        }
        mLayout = 0;
    }
    lbl_eu_80664898 = 0;
    func_8045F778__17UnkClass_8045F564Fv(&mMemRegion[0]);
}

void CMenuBattleEnd::Move() {
    getInstance__9CTaskGameFv();
    if (func_800426F0__9CTaskGameFv()) return;
    if (lbl_eu_80663E28 & (1u << 21)) return;
    if (!func_8013BE50()) return;
    if (mState == 1) {
        func_8026F95C(this);
    }
    {
        void* vtab = *(void**)mLayout;
        ((void (*)(void*, int))((void**)vtab)[0x38 / 4])(mLayout, 0);
    }
}

void CMenuBattleEnd::cbRenderBefore() {
    getInstance__9CTaskGameFv();
    if (func_800426F0__9CTaskGameFv()) return;
    if (lbl_eu_80663E28 & (1u << 21)) return;
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
    u32 mem = getWorkMem__17CWorkThreadSystemFv();
    CMenuBattleEnd* obj = (CMenuBattleEnd*)mtl::MemManager::allocate(0xbc, mem);
    if (obj != 0) {
        __ct__CMenuBattleEnd(obj, scene, mode, param);
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

    bool queued = false;
    for (int i = 0; i < 4; i++) {
        if (obj->mEntries[i].flag == 0) continue;

        switch (obj->mEntries[i].flag) {
        case 1:
            obj->mAnim90 = obj->mAnim88;
            func_80136910(obj->mLayout, &lbl_eu_8050E6F8[0x68], (u8)obj->mEntries[i].param);
            func_80136910(obj->mLayout, &lbl_eu_8050E6F8[0x74], (u8)obj->mEntries[i].param);
            break;
        case 2:
            obj->mAnim90 = obj->mAnim8C;
            func_80136910(obj->mLayout, &lbl_eu_8050E6F8[0x80], (u8)obj->mEntries[i].param);
            func_80136910(obj->mLayout, &lbl_eu_8050E6F8[0x8c], (u8)obj->mEntries[i].param);
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
            queued = true;
            break;
        }
    }

    if (!queued) {
        obj->mField64 = 1;
        obj->mState = 0;
    }
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

    if (obj->mEntries[0].flag != 0) {
        UnkBattleEntry& dst = packed[count];
        dst.flag = obj->mEntries[0].flag;
        dst.param = obj->mEntries[0].param;
        count++;
    }
    if (obj->mEntries[1].flag != 0) {
        UnkBattleEntry& dst = packed[count];
        dst.flag = obj->mEntries[1].flag;
        dst.param = obj->mEntries[1].param;
        count++;
    }
    if (obj->mEntries[2].flag != 0) {
        UnkBattleEntry& dst = packed[count];
        dst.flag = obj->mEntries[2].flag;
        dst.param = obj->mEntries[2].param;
        count++;
    }
    if (obj->mEntries[3].flag != 0) {
        UnkBattleEntry& dst = packed[count];
        dst.flag = obj->mEntries[3].flag;
        dst.param = obj->mEntries[3].param;
        count++;
    }

    if (count < 4) {
        for (u32 i = 0; i < 4; i++) {
            if (i < count) {
                obj->mEntries[i].flag = packed[i].flag;
                obj->mEntries[i].param = packed[i].param;
            } else {
                obj->mEntries[i].flag = 0;
                obj->mEntries[i].param = 0;
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