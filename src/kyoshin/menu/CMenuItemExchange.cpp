// Auto-scaffolded catalog TU for kyoshin/menu/CMenuItemExchange
// High-level C/C++ reconstruction from retail PPC.

#include "kyoshin/menu/CMenuItemExchange.hpp"

#include <revolution/GX.h>

#include "kyoshin/CBgTex.hpp"
#include <monolib/util/FixStr.hpp>

extern "C" void cbRenderBefore__17CMenuItemExchangeFv(void*);
extern "C" void __dt__9IUIWindowFv(void* self, int flags);

/**
 * Offset from the OC thunk interface (CMenuItemExchangeOC*) to the
 * containing CMenuItemExchange object. The OC system passes a pointer
 * to a sub-object embedded at this offset; thunks subtract this value
 * to recover the real this pointer before delegating.
 */
static const u32 CMENU_ITEM_EXCHANGE_OC_OFFSET = 0x6c;

/**
 * Factory-built object constructor (retail unmangled symbol). Builds the
 * CProcess base, installs the temp then composite vtables (+ OC sub-vtable
 * at +0x24), copies the null PMF triple into the Move/Draw callback slots,
 * zeroes/-1s the tail fields, then constructs the four embedded widgets and
 * clears the fade timers / phase latch.
 */
extern "C" __declspec(noinline) CMenuItemExchange*
__ct__CMenuItemExchange(CMenuItemExchange* self, CScn* scene, u32 arg) {
    __ct__8CProcessFv(self);

    // vtable fixups: temp (CProcess) vtable first, then the composite vtable.
    // PMF triple reads share one base register; post-increment derefs fold the
    // first access into `lwzu`. Stores are [1],[0],[2] per group.
    self->mVtab = (u32)lbl_eu_8052D238;
    const u32* src = __ptmf_null;
    u32 w0 = *src++;
    u32 w1 = *src++;
    self->mPtMf3C[1] = w1;
    self->mPtMf3C[0] = w0;
    self->mPtMf3C[2] = *src++;
    src = __ptmf_null;
    w1 = *src++;
    w0 = *src++;
    self->mPtMf48[1] = w0;
    self->mPtMf48[0] = w1;
    self->mPtMf48[2] = *src++;

    // Tail scalar fields: render vtable cleared, dispatch slot -1, mode byte 1.
    self->mScnRenderVt = 0;
    self->field_5C = 0;
    self->field_60 = -1;
    self->field_64 = 0;
    self->field_65 = 0;
    self->field_66 = 0;
    self->field_67 = 1;
    self->field_68 = 0;

    self->mVtab = (u32)lbl_eu_80533BC0;
    self->mOCVt = (u32)lbl_eu_80533BC0 + 0x24;
    self->mScene = scene;
    self->mField74 = arg;

    __ct__CBgTex(&self->mBgTex, 0);
    __ct__CTitleAHelp(&self->mTitleAHelp, 0, 0);
    __ct__CItemBoxLine(reinterpret_cast<CItemBoxLine*>(self->mItemBoxLine), 0, 0);
    __ct__CItemBoxGrid(&self->mItemBoxGrid, 0, 0, 0, 1);

    const f32 initTimer = lbl_eu_80667E78;
    self->mFloat511C = initTimer;
    self->mFloat5120 = initTimer;
    self->field_5118 = 0;
    lbl_eu_8066442C = 0;
    return self;
}

// extern "C" free-function form (CCol6CheckBat precedent): sub-object dtors in
// retail order +0x70C (CItemBoxGrid), +0xD0 (CItemBoxLine), +0x98
// (CTitleAHelp), +0x78 (CBgTex), then the +0x00 IUIWindow dtor (flags 0), then
// the flags-based delete; stmw/lmw frame via optimize_for_size.
#pragma push
#pragma optimize_for_size on
extern "C" void* __dt__17CMenuItemExchangeFv(void* self, int flags) {
    if (self != 0) {
        __dt__12CItemBoxGridFv(reinterpret_cast<CItemBoxGrid*>((u8*)self + 0x70C), -1);
        __dt__12CItemBoxLineFv(reinterpret_cast<CItemBoxLine*>((u8*)self + 0xD0), -1);
        __dt__11CTitleAHelpFv(reinterpret_cast<CTitleAHelp*>((u8*)self + 0x98), -1);
        __dt__6CBgTexFv(reinterpret_cast<CBgTex*>((u8*)self + 0x78), -1);
        __dt__9IUIWindowFv(self, 0);
        if (flags > 0) {
            operator delete(self);
        }
    }
    return self;
}
#pragma pop

// ---------------------------------------------------------------------------
// CMenuItemExchange::Init (us-801bf844)
// Construct each embedded widget through a stack temporary (retail unmangled
// ctor), copy its body into the member with the retail copy helper, then
// destroy the temporary. Finishes by registering the OC render callback on
// the owning scene.
// ---------------------------------------------------------------------------
void CMenuItemExchange::Init() {
    setPresentationFlag__Q22cf13CfGameManagerFv(1);

    // Background layout widget: temp CBgTex -> copy -> destroy, then load.
    u8 tmpBgRaw[0x20];
    __ct__CBgTex(reinterpret_cast<CBgTex*>(tmpBgRaw), 0);
    func_801BE108(&mBgTex, reinterpret_cast<CBgTex*>(tmpBgRaw));
    __dt__6CBgTexFv(reinterpret_cast<CBgTex*>(tmpBgRaw), -1);

    func_801C3C14(&mBgTex);

    // Title/help bar: temp CTitleAHelp (named from the locale string) ->
    // copy -> destroy, then load.
    u8 tmpTitleRaw[0x38];
    __ct__CTitleAHelp(reinterpret_cast<CTitleAHelp*>(tmpTitleRaw),
                      func_80136190(lbl_eu_80505324, lbl_eu_80505324 + 9, 0x1a), 0);
    func_801BE16C(&mTitleAHelp, reinterpret_cast<CTitleAHelp*>(tmpTitleRaw));
    __dt__11CTitleAHelpFv(reinterpret_cast<CTitleAHelp*>(tmpTitleRaw), -1);

    CTitleAHelp_load(&mTitleAHelp);

    // Item-box line: temp CItemBoxLine -> copy -> destroy, then register the
    // tab entries (retail call order 2,4,5,6,7,8,0xb,3,0xa) and finish the
    // line setup.
    u8 tmpLineRaw[0x63C];
    __ct__CItemBoxLine(reinterpret_cast<CItemBoxLine*>(tmpLineRaw), (u16)mField74, 2);
    func_801BE208(reinterpret_cast<CItemBoxLine*>(&mItemBoxLine[0]),
                  reinterpret_cast<CItemBoxLine*>(tmpLineRaw));
    __dt__12CItemBoxLineFv(reinterpret_cast<CItemBoxLine*>(tmpLineRaw), -1);

    func_801EDA08(reinterpret_cast<CItemBoxLine*>(&mItemBoxLine[0]));
    func_801EDA4C(reinterpret_cast<CItemBoxLine*>(&mItemBoxLine[0]), 2);
    func_801EDA4C(reinterpret_cast<CItemBoxLine*>(&mItemBoxLine[0]), 4);
    func_801EDA4C(reinterpret_cast<CItemBoxLine*>(&mItemBoxLine[0]), 5);
    func_801EDA4C(reinterpret_cast<CItemBoxLine*>(&mItemBoxLine[0]), 6);
    func_801EDA4C(reinterpret_cast<CItemBoxLine*>(&mItemBoxLine[0]), 7);
    func_801EDA4C(reinterpret_cast<CItemBoxLine*>(&mItemBoxLine[0]), 8);
    func_801EDA4C(reinterpret_cast<CItemBoxLine*>(&mItemBoxLine[0]), 0xb);
    func_801EDA4C(reinterpret_cast<CItemBoxLine*>(&mItemBoxLine[0]), 3);
    func_801EDA4C(reinterpret_cast<CItemBoxLine*>(&mItemBoxLine[0]), 0xa);
    func_801ED31C(reinterpret_cast<CItemBoxLine*>(&mItemBoxLine[0]));

    // Item-box grid: temp CItemBoxGrid -> copy -> destroy, then register the
    // two tab lists.
    u8 tmpGridRaw[0x4a0c];
    __ct__CItemBoxGrid(reinterpret_cast<CItemBoxGrid*>(tmpGridRaw), 2, (u16)mField74, 0, 0);
    func_801BE590(&mItemBoxGrid, reinterpret_cast<CItemBoxGrid*>(tmpGridRaw));
    __dt__12CItemBoxGridFv(reinterpret_cast<CItemBoxGrid*>(tmpGridRaw), -1);

    func_801CB480(&mItemBoxGrid);
    PushToList(&mItemBoxGrid, 0xb);
    PushToList(&mItemBoxGrid, 0xa);

    // Register the OC render callback (adjusted-this idiom: null this stays
    // null, otherwise the +0x6c sub-object).
    IScnRender* render = reinterpret_cast<IScnRender*>(this);
    if (this) render = reinterpret_cast<IScnRender*>(&mOCVt);
    mScene->addRenderCB(render, 0xd, 0);
}

/* Copy-construct a CBgTex field-by-field from src into dest (retail
 * func_801BE108): the +0x4 mem region through the shared UnkClass_8011C974
 * helper, then the file-handle / layout pointers and the three status bytes.
 * Returns dest (copy-ctor idiom, retail restores r3 to dest before the tail). */
extern "C" CBgTex* func_801BE108(CBgTex* dest, CBgTex* src) {
    __ct__UnkClass_8011C974(&dest->mMemRegion, &src->mMemRegion);
    dest->mFileHandle = src->mFileHandle;
    dest->mLayout = src->mLayout;
    dest->mLayoutReady = src->mLayoutReady;
    dest->mLoaded = src->mLoaded;
    dest->mPtmMode = src->mPtmMode;
    return dest;
}

/* Copy-construct a CTitleAHelp field-by-field from src into dest (retail
 * func_801BE16C): the +0x4 mem region via UnkClass_8011C974, then the file
 * handle / layout / anim pointers and the trailing status bytes. */
extern "C" CTitleAHelp* func_801BE16C(CTitleAHelp* dest, CTitleAHelp* src) {
    __ct__UnkClass_8011C974(&dest->unk4, &src->unk4);
    dest->mFileHandle = src->mFileHandle;
    dest->mArcResourceAccessor = src->mArcResourceAccessor;
    dest->mLayout = src->mLayout;
    dest->mAnimTrans20 = src->mAnimTrans20;
    dest->mAnimTrans24 = src->mAnimTrans24;
    dest->unk28 = src->unk28;
    dest->unk2c = src->unk2c;
    dest->mName = src->mName;
    dest->unk34 = src->unk34;
    dest->unk35 = src->unk35;
    dest->unk36 = src->unk36;
    dest->unk37 = src->unk37;
    return dest;
}

/* Copy-construct a CItemBoxLine body field-by-field from src into dest
 * (retail func_801BE208), accessed through CItemBoxLineView (see header).
 * Layout regions go through the shared 0x10-byte region helper, scalars are
 * copied individually, the four embedded cursors and the info2 / num-select /
 * scroll-bar / sys-win sub-objects go through their copy helpers, and the two
 * tab-name strings via FixStr::operator= (strlen + strcpy). */
extern "C" __declspec(noinline) void func_801BE208(CItemBoxLine* destArg, CItemBoxLine* srcArg) {
    CItemBoxLineView* dest = reinterpret_cast<CItemBoxLineView*>(destArg);
    CItemBoxLineView* src = reinterpret_cast<CItemBoxLineView*>(srcArg);

    __ct__UnkClass_8011C974(&dest->region04[0], &src->region04[0]);
    __ct__UnkClass_8011C974(&dest->region14[0], &src->region14[0]);

    // File handles / layout / anim pointers.
    dest->field24 = src->field24;
    dest->field28 = src->field28;
    dest->field2C = src->field2C;
    dest->field30 = src->field30;
    dest->field34 = src->field34;
    dest->field38 = src->field38;
    dest->field3C = src->field3C;
    dest->field40 = src->field40;
    dest->field44 = src->field44;
    dest->field48 = src->field48;
    dest->field4C = src->field4C;
    dest->field50 = src->field50;
    dest->field54 = src->field54;
    dest->field58 = src->field58;
    dest->unk59 = src->unk59;

    // Tab entry bytes + occupancy bytes (retail copies these as word pairs).
    for (int i = 0; i < 9; i++) {
        dest->tabEntries[i] = src->tabEntries[i];
    }
    dest->tabCount = src->tabCount;
    for (int i = 0; i < 9; i++) {
        dest->unk64[i] = src->unk64[i];
    }
    dest->field6D = src->field6D;

    func_8018B0FC(&dest->cur70[0], &src->cur70[0]);
    func_8018B0FC(&dest->cur88[0], &src->cur88[0]);
    func_8018B0FC(&dest->curA0[0], &src->curA0[0]);
    func_8018B0FC(&dest->curB8[0], &src->curB8[0]);

    // Item-box-info2 state region: two typed sub-regions + raw body.
    __ct__UnkClass_8011C974(&dest->info2D0[0x4], &src->info2D0[0x4]);
    __ct__UnkClass_8011C974(&dest->info2D0[0x14], &src->info2D0[0x14]);
    for (int i = 0x24; i < 0xB0; i++) {
        dest->info2D0[i] = src->info2D0[i];
    }
    func_8018BE74(&dest->info2D0[0xB0], &src->info2D0[0xB0]);

    func_801671D4(&dest->numSel2DC[0], &src->numSel2DC[0]);
    func_8011C998(&dest->scrollBar310[0], &src->scrollBar310[0]);
    func_8016742C(&dest->sysWin350[0], &src->sysWin350[0]);

    // Tab-page state block.
    dest->unk38C = src->unk38C;
    dest->unk38E = src->unk38E;
    dest->field390 = src->field390;
    dest->pad391 = src->pad391;
    dest->field392 = src->field392;
    dest->field394 = src->field394;
    dest->field396 = src->field396;
    dest->unk39E = src->unk39E;
    dest->field39F = src->field39F;
    dest->field3A0 = src->field3A0;
    dest->field3A1 = src->field3A1;
    dest->field3A2 = src->field3A2;
    dest->field3A3 = src->field3A3;
    for (int i = 0; i < 12; i++) {
        dest->entries[i].f4 = src->entries[i].f4;
        dest->entries[i].f0 = src->entries[i].f0;
    }

    dest->tabCount2 = src->tabCount2;
    dest->field92 = src->field92;
    dest->field93 = src->field93;

    // Tab-name strings (FixStr::operator=: strlen into mLength + strcpy).
    dest->str438 = src->str438;
    dest->str45C = src->str45C;

    func_8018BE74(&dest->pageWords4E0[0], &src->pageWords4E0[0]);
}

extern "C" __declspec(noinline) void func_801BE590(CItemBoxGrid* dest, CItemBoxGrid* src) {
    __ct__UnkClass_8011C974(reinterpret_cast<u8*>(dest) + 4, reinterpret_cast<u8*>(src) + 4);
    __ct__UnkClass_8011C974(reinterpret_cast<u8*>(dest) + 8, reinterpret_cast<u8*>(src) + 8);
}

extern "C" void waitForDrawDone__9CDeviceVIFv();
extern "C" void func_801C3D9C(void*);
extern "C" void func_801C40A0(void*);
extern "C" void func_801ED618(void*);
extern "C" void func_801CAE9C(void*);
extern u32 lbl_eu_80664428;

// Detach the render callback, tear down the embedded widgets, clear the
// global active flag, and reset the CfGameManager state.
void CMenuItemExchange::Term() {
    waitForDrawDone__9CDeviceVIFv();
    IScnRender* render = reinterpret_cast<IScnRender*>(this);
    if (this) render = reinterpret_cast<IScnRender*>(&mOCVt);
    mScene->removeRenderCB(render);
    func_801C3D9C(&mBgTex);
    func_801C40A0(&mTitleAHelp);
    func_801ED618(&mItemBoxLine[0]);
    func_801CAE9C(&mItemBoxGrid);
    lbl_eu_80664428 = 0;
    setPresentationFlag__Q22cf13CfGameManagerFv(0);
}

// Phase handlers used by Move()'s state dispatch (defined below).
// Retail dispatches through the raw jump table jumptable_eu_80533B98
// (9 entries, states 0..8); noinline keeps each handler out-of-line.
__declspec(noinline) void func_801BEE74(CMenuItemExchange* self);
__declspec(noinline) void func_801BEEF4(CMenuItemExchange* self);
__declspec(noinline) void func_801BEF44(CMenuItemExchange* self);
__declspec(noinline) void func_801BF2E8(CMenuItemExchange* self);
__declspec(noinline) void func_801BF348(CMenuItemExchange* self);
__declspec(noinline) void func_801BF414(CMenuItemExchange* self);
__declspec(noinline) void func_801BF464(CMenuItemExchange* self);
__declspec(noinline) void func_801BF6A0(CMenuItemExchange* self);
__declspec(noinline) void func_801BF70C(CMenuItemExchange* self);
typedef void (*CMenuExchangePhaseFn)(CMenuItemExchange*);

/*
 * Per-frame update: skip while the task system is busy or the exchange screen
 * mode flag (bit 10) is set; otherwise run the current phase handler
 * (state dispatch through the retail jump table) and tick every widget.
 */
void CMenuItemExchange::Move() {
    getInstance__9CTaskGameFv();
    if (isFlag01Set__9CTaskGameFv() == 0) {
        if ((lbl_eu_80663E28 & 0x200000) == 0) {
            goto body;
        }
    }
    goto end;
end:
    return;
body:
    switch (field_5118) {
    case 0: func_801BEE74(this); break;
    case 1: func_801BEEF4(this); break;
    case 2: func_801BEF44(this); break;
    case 3: func_801BF2E8(this); break;
    case 4: func_801BF348(this); break;
    case 5: func_801BF414(this); break;
    case 6: func_801BF464(this); break;
    case 7: func_801BF6A0(this); break;
    case 8: func_801BF70C(this); break;
    }
    func_801C3D54(&mBgTex);
    func_801ED3E8(reinterpret_cast<CItemBoxLine*>(mItemBoxLine));
    func_801CABC8(&mItemBoxGrid);
    func_801C3FF0(&mTitleAHelp);
}

/*
 * Render the exchange screen through a stack DrawInfo. The empty
 * if/goto/end/body chain reproduces the retail gate shape: task-busy check
 * (`bne end`), then the global mode flag tested with branch-over-branch
 * (`beq body`; fallthrough = end).
 */
void CMenuItemExchange::cbRenderBefore() {
    getInstance__9CTaskGameFv();
    if (isFlag01Set__9CTaskGameFv() == 0 &&
        (lbl_eu_80663E28 & 0x200000) == 0) {
        goto body;
    }
    goto end;
end:
    return;
body:
    if (func_8013BE50() != 0) {
        GXSetZMode(GX_FALSE, GX_NEVER, GX_FALSE);
        // Raw-storage DrawInfo built/destroyed via pre-mangled ct/dt calls
        // (a C++ local would virtual-dispatch its scope-exit dtor).
        u8 drawInfo[0x54];
        __ct__Q34nw4r3lyt8DrawInfoFv(reinterpret_cast<nw4r::lyt::DrawInfo*>(&drawInfo[0]));
        func_80137250(reinterpret_cast<nw4r::lyt::DrawInfo*>(&drawInfo[0]));
        func_801C3D7C(&mBgTex, reinterpret_cast<nw4r::lyt::DrawInfo*>(&drawInfo[0]));
        func_801ED4FC(reinterpret_cast<CItemBoxLine*>(mItemBoxLine),
                      reinterpret_cast<nw4r::lyt::DrawInfo*>(&drawInfo[0]));
        func_801CAD8C(&mItemBoxGrid, reinterpret_cast<nw4r::lyt::DrawInfo*>(&drawInfo[0]));
        func_801C4080(&mTitleAHelp, reinterpret_cast<nw4r::lyt::DrawInfo*>(&drawInfo[0]));
        __dt__Q34nw4r3lyt8DrawInfoFv(reinterpret_cast<nw4r::lyt::DrawInfo*>(&drawInfo[0]), -1);
    }
}

/* Singleton factory (retail unmangled symbol): allocate the 0x5124-byte
 * screen from the work-thread heap, build it, register it under the parent
 * process and store/return the instance (0 when one already exists). */
extern "C" CMenuItemExchange* func_801BEDE0(CProcess* registParent, CScn* scene,
                                            u32 arg) {
    if (lbl_eu_80664428 != 0) {
        return NULL;
    }
    CMenuItemExchange* obj = reinterpret_cast<CMenuItemExchange*>(
        allocate__Q23mtl10MemManagerFUlUl(
            0x5124, reinterpret_cast<u32>(getWorkMem__17CWorkThreadSystemFv())));
    if (obj != NULL) {
        obj = __ct__CMenuItemExchange(obj, scene, arg);
    }
    lbl_eu_80664428 = reinterpret_cast<u32>(obj);
    Regist__8CProcessFP8CProcessb(reinterpret_cast<CProcess*>(lbl_eu_80664428),
                                  registParent, false);
    return reinterpret_cast<CMenuItemExchange*>(lbl_eu_80664428);
}

// (lbl_eu_80664428 != 0)
extern "C" bool func_801BEE5C() { extern u32 lbl_eu_80664428; return lbl_eu_80664428 != 0; }

extern u8 lbl_eu_8066442C;
u8 func_801BEE6C() { return lbl_eu_8066442C; }

/* Load-finish hook: once the background layout and item-box line are ready,
 * hide the title/help bar prompt, finish the line load, ready the grid and
 * advance to phase 1 (field_5118), then kick op 0x6d. */
__declspec(noinline) void func_801BEE74(CMenuItemExchange* self) {
    if (func_801C3E34(&self->mBgTex) != 0 &&
        func_801C4114(&self->mTitleAHelp) != 0 &&
        func_801ED774(reinterpret_cast<CItemBoxLine*>(self->mItemBoxLine)) != 0) {
        func_801C412C(&self->mTitleAHelp);
        func_801ED864(reinterpret_cast<CItemBoxLine*>(self->mItemBoxLine));
        func_801CAA6C(&self->mItemBoxGrid);
        self->field_5118 = 1;
        playUISound__FUl(0x6d);
    }
}

/* Exchange-menu phase gate: once the title/help bar is idle and the item-box
 * line is ready, advance the menu to phase 2 (field_5118). */
__declspec(noinline) void func_801BEEF4(CMenuItemExchange* self) {
    if (isIdle__11CTitleAHelpFv(&self->mTitleAHelp) != 0 &&
        func_801ED800(reinterpret_cast<CItemBoxLine*>(self->mItemBoxLine)) != 0) {
        self->field_5118 = 2;
    }
}

__declspec(noinline) void func_801BEF44(CMenuItemExchange* self){}

/* Exchange-menu phase gate: idle title/help bar plus ready item-box grid
 * advances the menu to phase 1 (field_5118). */
__declspec(noinline) void func_801BF2E8(CMenuItemExchange* self) {
    if (isIdle__11CTitleAHelpFv(&self->mTitleAHelp) != 0 &&
        func_801ED800(reinterpret_cast<CItemBoxLine*>(self->mItemBoxLine)) != 0 &&
        GetField61(&self->mItemBoxGrid) != 0) {
        self->field_64 = 1;
    }
}

/* Phase 4: once help bar/line/grid are ready, run the fade-in timer; when it
 * leaves the initial value, apply the grid selection to help bar + line and
 * advance to phase 5, kicking menu-op 0x6d. */
__declspec(noinline) void func_801BF348(CMenuItemExchange* self) {
    if (isIdle__11CTitleAHelpFv(&self->mTitleAHelp) != 0 &&
        func_801ED800(reinterpret_cast<CItemBoxLine*>(self->mItemBoxLine)) != 0 &&
        func_801CB038(&self->mItemBoxGrid) != 0) {
        self->mFloat5120 = self->mFloat5120 + lbl_eu_80667E7C;
        if (!(self->mFloat5120 <= lbl_eu_80667E7C)) {
            self->mFloat5120 = lbl_eu_80667E78;
            func_801C41E8(&self->mTitleAHelp, func_801CDFB4(&self->mItemBoxGrid));
            func_801CDB94(&self->mItemBoxGrid,
                          func_801EECC8(reinterpret_cast<CItemBoxLine*>(self->mItemBoxLine)));
            func_801C416C(&self->mTitleAHelp);
            func_801CB28C(&self->mItemBoxGrid);
            self->field_5118 = 5;
            playUISound__FUl(0x6d);
        }
    }
}

/* Exchange-menu phase gate: idle title/help bar plus ready item-box grid
 * advances the menu to phase 6 (field_5118). */
__declspec(noinline) void func_801BF414(CMenuItemExchange* self) {
    if (isIdle__11CTitleAHelpFv(&self->mTitleAHelp) != 0 &&
        GetField61(&self->mItemBoxGrid) != 0) {
        self->field_5118 = 6;
    }
}

/* Phase 6 (main exchange input): decode turbo/pressed pad flags (the pressed
 * bit positions depend on the game-manager mode), dispatch one cursor/grid
 * action, then run the help-bar sync and the two phase transitions. */
__declspec(noinline) void func_801BF464(CMenuItemExchange* self) {
    CMIExPadData* pad = getCfPadData__Q22cf13CfGameManagerFv();
    int turboCancel = (pad->mTurboFlags & 0x8004) != 0;
    int pageTurn = (pad->mTurboFlags & 0x10010000) != 0;
    int cancel = (pad->mTurboFlags & 0x2001) != 0;
    int decide = (pad->mTurboFlags & 0x4002) != 0;
    int up, down, leftTrigger, yHeld;
    if (isClassicController__Q22cf13CfGameManagerFv(-1) != 0) {
        up = (pad->mPressedFlags & 0x00400000) != 0;
        down = (pad->mPressedFlags & 0x00800000) != 0;
        leftTrigger = (pad->mPressedFlags & 0x01000000) != 0;
        yHeld = (pad->mPressedFlags & 0x00000400) != 0;
    } else {
        up = (pad->mPressedFlags & 0x00000010) != 0;
        down = (pad->mPressedFlags & 0x00000020) != 0;
        leftTrigger = (pad->mPressedFlags & 0x00000800) != 0;
        yHeld = (pad->mPressedFlags & 0x00000040) != 0;
    }

    if (up != 0) {
        func_801CCAF0(&self->mItemBoxGrid);
    } else if (down != 0) {
        if (func_801CB0FC(&self->mItemBoxGrid) != 0) {
            func_801CC7B0(&self->mItemBoxGrid, 0);
        } else if (func_801CDBE0(&self->mItemBoxGrid) == 0 &&
                   func_801CB1E4(&self->mItemBoxGrid) == 0) {
            func_801C4198(&self->mTitleAHelp);
            func_801CB38C(&self->mItemBoxGrid);
            self->field_5118 = 7;
        }
    } else if (turboCancel != 0) {
        func_801CB5F0(&self->mItemBoxGrid);
    } else if (pageTurn != 0) {
        func_801CBA04(&self->mItemBoxGrid);
    } else if (cancel != 0) {
        func_801CBDE8(&self->mItemBoxGrid);
    } else if (decide != 0) {
        func_801CC0EC(&self->mItemBoxGrid);
    } else if (leftTrigger != 0) {
        func_801CC5DC(&self->mItemBoxGrid);
    } else if (yHeld != 0) {
        func_801CDEE8(&self->mItemBoxGrid);
    }

    // Help bar follows the grid's current mode.
    func_801C41E8(&self->mTitleAHelp, func_801CDFB4(&self->mItemBoxGrid));
    if (func_801CDBE0(&self->mItemBoxGrid) != 0) {
        lbl_eu_8066442C = GetField52D(&self->mItemBoxGrid);
        func_801C414C(&self->mTitleAHelp);
        func_801CB38C(&self->mItemBoxGrid);
        self->field_5118 = 3;
    }
    if (func_801CB184(&self->mItemBoxGrid) != 0) {
        func_801C4198(&self->mTitleAHelp);
        func_801CB38C(&self->mItemBoxGrid);
        self->field_5118 = 7;
    }
}

/* Phase-8 entry: once the title/help bar is idle and the item-box grid is
 * ready, reset the help bar to mode 0, stop its animation, finish the line
 * load and advance the menu to phase 8 (field_5118). */
__declspec(noinline) void func_801BF6A0(CMenuItemExchange* self) {
    if (isIdle__11CTitleAHelpFv(&self->mTitleAHelp) != 0 &&
        GetField61(&self->mItemBoxGrid) != 0) {
        func_801C41E8(&self->mTitleAHelp, 0);
        func_801C416C(&self->mTitleAHelp);
        func_801ED864(reinterpret_cast<CItemBoxLine*>(self->mItemBoxLine));
        self->field_5118 = 8;
    }
}

/* Exchange-menu phase gate: twin of func_801BEEF4 (idle help bar + ready
 * item-box line) advancing the menu to phase 2. */
__declspec(noinline) void func_801BF70C(CMenuItemExchange* self) {
    if (isIdle__11CTitleAHelpFv(&self->mTitleAHelp) != 0 &&
        func_801ED800(reinterpret_cast<CItemBoxLine*>(self->mItemBoxLine)) != 0) {
        self->field_5118 = 2;
    }
}

/**
 * OC thunk for cbRenderBefore. Adjusts the OC wrapper pointer back to the
 * containing CMenuItemExchange and delegates to cbRenderBefore.
 */
extern "C" void func_801BF75C(CMenuItemExchangeOC* param) {
    ((void(*)(void*))cbRenderBefore__17CMenuItemExchangeFv)((char*)param - 0x6c);
}

/**
 * OC thunk for the destructor. Adjusts the OC wrapper pointer back to the
 * containing CMenuItemExchange and delegates to the real destructor.
 */
extern "C" void func_801BF764(CMenuItemExchangeOC* param) {
    ((void(*)(void*))__dt__17CMenuItemExchangeFv)((char*)param - 0x6c);
}
