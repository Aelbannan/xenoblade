// Decomp of kyoshin/menu/CMenuShopSell.
// Shop sell menu screen process (CProcess-header + IScnRender MI layout).

#include "kyoshin/menu/CMenuShopSell.hpp"

#include "kyoshin/CTaskGame.hpp"
#include "monolib/device/CDeviceVI.hpp"
#include "monolib/scn/CScn.hpp"
#include "monolib/util/MemManager.hpp"
#include "monolib/work/CWorkThreadSystem.hpp"

#include <revolution/GX.h>

extern "C" void cbRenderBefore__13CMenuShopSellFv(void*);

/*
 * Retail constructor symbol (unmangled global in US). Written as a free
 * function so the factory emits a real bl to the bare retail symbol; returns
 * `this` in r3 like retail. Mirrors the CMenuGCItem ctor pattern: CProcess
 * base ctor, temp vtable store, null PMF data copy, then the composite
 * vtable + the IScnRender sub-vtable at +0x58, then each embedded widget's
 * ctor and the final state byte.
 */
extern "C" __declspec(noinline) CMenuShopSell* __ct__CMenuShopSell(CMenuShopSell* self, CScn* scene) {
    __ct__8CProcessFv((CProcess*)self);

    // vtable fixups: temp (CProcess) vtable first, then the composite vtable
    // and the IScnRender sub-vtable at +0x58. The pmf copy reads the null
    // member-function-pointer triple through one base register: retail loads
    // [0] via lwzu (materialising the base), then [1]/[2] at fixed offsets.
    *(u32*)((u8*)self + 0x10) = (u32)lbl_eu_8052BF70;
    u32 pmf0_0 = __ptmf_null[0];
    u32 pmf0_1 = __ptmf_null[1];
    self->ptmf0[1] = pmf0_1;
    self->ptmf0[0] = pmf0_0;
    u32 pmf0_2 = __ptmf_null[2];
    self->ptmf0[2] = pmf0_2;
    u32 pmf1_0 = __ptmf_null[0];
    u32 pmf1_1 = __ptmf_null[1];
    self->ptmf1[1] = pmf1_1;
    self->ptmf1[0] = pmf1_0;
    u32 pmf1_2 = __ptmf_null[2];
    self->ptmf1[2] = pmf1_2;
    self->mField54 = 0;
    self->mField55 = 0;

    *(u32*)((u8*)self + 0x10) = (u32)lbl_eu_80532650;
    *(u32*)((u8*)self + 0x58) = (u32)lbl_eu_80532650 + 0x24;
    self->mScene = scene;

    __ct__CBgTex(&self->mBgTex, 0);
    __ct__CTitleAHelp(&self->mTitleAHelp, 0, 0);
    __ct__CItemBoxGrid(&self->mItemBoxGrid, 0, 0, 0, 1);
    self->mState = 0;
    return self;
}

/*
 * Destructor (retail D2 form, symbol keeps its C++ mangling in US).
 *
 * Written with an explicit flags parameter so the base-class destruction can
 * target the retail CProcess dtor wrapper __dt__800FED0C: a real member
 * destructor would make MWCC auto-emit __dt__8CProcessFv (the library D1)
 * for the base call, which cannot match the retail reloc name at the call
 * site. Subobjects are destroyed in reverse construction order, then
 * conditional operator delete when flags > 0 (same shape as CMenuShopBuy).
 */
extern "C" CMenuShopSell* __dt__13CMenuShopSellFv(CMenuShopSell* _this, int flags) {
    if (_this != 0) {
        __dt__12CItemBoxGridFv(&_this->mItemBoxGrid, -1);
        __dt__11CTitleAHelpFv(&_this->mTitleAHelp, -1);
        __dt__6CBgTexFv(&_this->mBgTex, -1);
        __dt__800FED0C(_this, 0);
        if (flags > 0) {
            operator delete(_this);
        }
    }
    return _this;
}

void CMenuShopSell::Init() {}

// Copies the +0x4..+0x15 region (4 words + 2 bytes) of a 0x18-byte cursor
// object. The source fields are read into locals first so MWCC holds all six
// loads before the stores (retail load-all/store-all shape). Declared extern
// "C" in CItemBoxGrid.hpp (retail keeps the bare func_ name; CMenuShopBuy's
// item-box-line copy passes &cursors[i]).
void func_8018B0FC(void* dst, void* src) {
    ShopSellCursor18* d = static_cast<ShopSellCursor18*>(dst);
    const ShopSellCursor18* s = static_cast<const ShopSellCursor18*>(src);
    u32 v15 = s->field_0x15;
    u32 v14 = s->field_0x14;
    u32 v10 = s->field_0x10;
    u32 vC = s->field_0xC;
    u32 v8 = s->field_0x8;
    u32 v4 = s->field_0x4;
    d->field_0x4 = v4;
    d->field_0x8 = v8;
    d->field_0xC = vC;
    d->field_0x10 = v10;
    d->field_0x14 = v14;
    d->field_0x15 = v15;
}

// Copies the 0x806-byte sell data blob: the 256 x 8-byte item array at +0x4
// (the mItems struct assignment lowers to the retail mtctr/lwzu/stwu counted
// loop) followed by the trailing u16 at +0x804.
// Copies the 0x806-byte sell data blob: the 256 x 8-byte item array at +0x4
// (the mItems struct assignment lowers to the retail mtctr/lwzu/stwu counted
// loop) followed by the trailing u16 at +0x804.
void func_8018B130(ShopSellData* dst, const ShopSellData* src) {
    dst->mItems = src->mItems;
    dst->mField804 = src->mField804;
}

/* Tear the shop-sell screen down: wait for the draw callback to finish,
 * remove the render callback from the owning scene, release each embedded
 * widget and clear the singleton instance flag. */
void CMenuShopSell::Term() {
    CDeviceVI::waitForDrawDone();

    // The `if (this)` is the MWCC idiom that splits mr r4,r31 / beq / addi r4,+0x58.
    IScnRender* renderCB = reinterpret_cast<IScnRender*>(this);
    if (this != NULL) {
        renderCB = &mIScnRender;
    }
    mScene->removeRenderCB(renderCB);

    func_801C3D9C(&mBgTex);
    func_801C40A0(&mTitleAHelp);
    func_801CAE9C(&mItemBoxGrid);

    lbl_eu_806642F0 = 0;
    func_8008294C__Q22cf13CfGameManagerFv(0);
}

/* Per-frame update of the shop-sell screen. Gate on the task/busy flags, then
 * drive the 4-state FSM and refresh the bg / item grid / title bar each frame. */
void CMenuShopSell::Move() {
    CTaskGame::getInstance();
    if (CTaskGame::func_800426F0() || (lbl_eu_80663E28 & 0x200000))
        return;

    switch (mState) {
    case 0:
        func_8018B3A8(this);
        break;
    case 1:
        func_8018B420(this);
        break;
    case 2:
        func_8018B470(this);
        break;
    case 3:
        func_8018B658(this);
        break;
    default:
        break;
    }

    func_801C3D54(&mBgTex);
    func_801CABC8(&mItemBoxGrid);
    func_801C3FF0(&mTitleAHelp);
}

/* Render the shop-sell screen through a stack DrawInfo: gate on the task/busy
 * flags and the scene-active check, then draw bg, item grid and title/help
 * bar in that order. */
void CMenuShopSell::cbRenderBefore() {
    CTaskGame::getInstance();
    if (CTaskGame::func_800426F0() || (lbl_eu_80663E28 & 0x200000))
        return;
    if (func_8013BE50() == 0) return;

    GXSetZMode(GX_FALSE, GX_NEVER, GX_FALSE);
    // Raw-storage DrawInfo built/destroyed via C-ABI pre-mangled ct/dt calls
    // (a C++ local would virtual-dispatch its scope-exit destructor).
    u8 drawInfo[0x54];
    __ct__Q34nw4r3lyt8DrawInfoFv(&drawInfo[0]);
    func_80137250((nw4r::lyt::DrawInfo*)&drawInfo[0]);
    func_801C3D7C(&mBgTex, (nw4r::lyt::DrawInfo*)&drawInfo[0]);
    func_801CAD8C(&mItemBoxGrid, (nw4r::lyt::DrawInfo*)&drawInfo[0]);
    func_801C4080(&mTitleAHelp, (nw4r::lyt::DrawInfo*)&drawInfo[0]);
    __dt__Q34nw4r3lyt8DrawInfoFv(&drawInfo[0], -1);
}

/* Shop-sell menu factory: if no instance exists yet, allocate 0x4ac8 bytes
 * from the work heap, construct the CMenuShopSell, publish it to the
 * singleton flag and register it as a CProcess child of `parent`. Returns the
 * singleton (or 0 when one already exists). */
extern "C" CMenuShopSell* func_8018B324(CProcess* parent, CScn* scene) {
    if (lbl_eu_806642F0 != 0)
        return 0;

    CMenuShopSell* obj = (CMenuShopSell*)mtl::MemManager::allocate(
        sizeof(CMenuShopSell), CWorkThreadSystem::getWorkMem());
    if (obj != 0) {
        // Reassign from the ctor return so obj stays in volatile r3 (retail
        // never spills it to a callee-saved register).
        obj = __ct__CMenuShopSell(obj, scene);
    }
    lbl_eu_806642F0 = (u32)obj;
    obj->Regist(parent, 0);
    return (CMenuShopSell*)lbl_eu_806642F0;
}

void func_8018B398(){}

/* Phase 0 -> 1 (open): once the background, title bar and item grid are all
 * ready, run the open sequence (title bar + item grid) and play the open
 * sound, then advance the phase byte at 0x4AC4. */
extern "C" __declspec(noinline) void func_8018B3A8(CMenuShopSell* self) {
    if (func_801C3E34(&self->mBgTex) != 0 && func_801C4114(&self->mTitleAHelp) != 0 &&
        func_801CB038(&self->mItemBoxGrid) != 0) {
        func_801C412C(&self->mTitleAHelp);
        func_801CB28C(&self->mItemBoxGrid);
        self->mState = 1;
        func_80138078__FUl(0x6d);
    }
}

// Phase transition: once the title bar is idle and the item grid is ready,
// advance the state byte at 0x4AC4 to 2 (the CMenuGCItem::Move case-1 check,
// kept as a separate helper here).
extern "C" __declspec(noinline) void func_8018B420(CMenuShopSell* self) {
    if (self->mTitleAHelp.isIdle() != 0 && GetField61(&self->mItemBoxGrid) != 0) {
        self->mState = 2;
    }
}

// Phase 2 (interaction) - not yet decompiled.
extern "C" __declspec(noinline) void func_8018B470(CMenuShopSell* self) {}

// Phase transition: once the title bar is idle and the item grid is ready,
// set the closing-state flag at 0x54 (the CMenuGCItem::Move case-3 check,
// kept as a separate helper here).
extern "C" __declspec(noinline) void func_8018B658(CMenuShopSell* self) {
    if (self->mTitleAHelp.isIdle() != 0 && GetField61(&self->mItemBoxGrid) != 0) {
        self->mField54 = 1;
    }
}

// Adjusting thunk: upcasts from secondary base (at offset +0x58 within CMenuShopSell)
// to the full object, then tail-calls cbRenderBefore.
extern "C" void func_8018B6A8(void* self) {
    ((void(*)(void*))cbRenderBefore__13CMenuShopSellFv)((char*)self - 0x58);
}

// Adjusting thunk: upcasts from secondary base (at offset +0x58 within CMenuShopSell)
// to the full object, then tail-calls the destructor.
extern "C" void func_8018B6B0(void* self) {
    ((void(*)(void*))__dt__13CMenuShopSellFv)((char*)self - 0x58);
}
