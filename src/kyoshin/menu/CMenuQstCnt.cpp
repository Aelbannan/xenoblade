// Auto-scaffolded catalog TU for kyoshin/menu/CMenuQstCnt
// Replace stubs with high-level C/C++ during decomp.

#include <types.h>

#include "kyoshin/code_80135FDC.hpp"
#include "kyoshin/cf/CfGameManager.hpp"
#include "kyoshin/CTaskGameImports.hpp"
#include "libs/monolib/src/scn/CScn_8049603C.hpp" // Scn_QueryUnk80State (single owner decl)
#include "kyoshin/CUICfManager.hpp"
#include "monolib/work/CProcess.hpp"
#include "monolib/lib/UnkClass_8045F564.hpp"
#include "monolib/device/CDeviceFont.hpp"
#include "monolib/scn/CScn.hpp"
#include "monolib/util/MemManager.hpp"
#include <nw4r/lyt.h>
#include <nw4r/lyt/lyt_drawInfo.h>
#include <revolution/gx/GXPixel.h>
#include <revolution/gx/GXTypes.h>

#include "kyoshin/menu/CMenuQstCnt.hpp"

struct QstMenuData {
    u8 mPad88[0x88];                            // 0x00
    nw4r::lyt::AnimTransform* mAnim88;          // 0x88
    u32 mState8C;                               // 0x8C
    f32 mValue90;                               // 0x90
};

extern void playUISound(u32);

// C-linkage (retail symbols are unmangled) declarations for the free
// functions defined in this TU. Definitions keep the same linkage.
extern "C" {
void setQstEntry(QstEntry* entry, short a, short b, unsigned char c, unsigned char d, unsigned char e);
QstEntry* copyQstEntry(QstEntry* dst, const QstEntry* src);
QstEntry* copyQstEntry2(QstEntry* dst, const QstEntry* src);
void __ct__CMenuQstCnt(CMenuQstCnt* self, CScn* scene, short a, short b, unsigned char c, unsigned char d, unsigned char e);
CMenuQstCnt* QstCnt_GetOrCreate(CProcess* parent, CScn* scene, short a, short b, unsigned char c, unsigned char d, unsigned char e);
int QstCnt_HasInstance_6B94();
void invalidateQstFlag();
void QstCnt_SignalReady_6BBC(QstMenuData* self);
void QstCnt_AdvanceToOpen_6C18(QstMenuData* self);
void QstCnt_TickToOpened_6C5C(QstMenuData* self);
void QstCnt_FinishAnim(CMenuQstCnt* self);
// (memberized: QstCnt_RefreshDisplay -> CMenuQstCnt::RefreshDisplay; declaration moved into class CMenuQstCnt)
}

// --- OOL base/subobject + helper/data imports are declared in CMenuQstCnt.hpp ---

CMenuQstCnt* lbl_eu_80664720;

// (lbl_eu_80664720 != 0) - retail lwz sda21; subic; subfe
extern "C" int QstCnt_HasInstance_6B94() { return lbl_eu_80664720 != 0; }

// Retail keeps setQstEntry/copyQstEntry/copyQstEntry2 as out-of-line calls
// from every callsite; without the auto_inline guard, MWCC -inline auto
// inlines them and the 32-iteration loops blow up (MWCC_CASES sec hbm/seq).
#pragma push
#pragma auto_inline off
void setQstEntry(QstEntry* entry, short a, short b, unsigned char c, unsigned char d, unsigned char e) {
    entry->f0 = a;
    entry->f2 = b;
    entry->f4 = c;
    entry->f5 = d;
    entry->f6 = e;
}
#pragma pop

// Constructor (unmangled retail symbol). Runs CProcess's ctor then fills the
// IUIWindow base region by hand (the IUIWindow base ctor is out-of-line in
// retail, so its effects are emitted inline at the head of this ctor).
void __ct__CMenuQstCnt(CMenuQstCnt* self, CScn* scene, short a, short b, unsigned char c, unsigned char d, unsigned char e) {
    __ct__8CProcessFv(self);

    self->mVtab = (u32)lbl_eu_8052D238;
    u32* ptmf = __ptmf_null;
    self->mPtmfFunc0 = ptmf[0];
    self->mPtmfThis0 = ptmf[1];
    self->mPtmfDelta0 = ptmf[2];
    self->mPtmfFunc1 = ptmf[0];
    self->mPtmfThis1 = ptmf[1];
    self->mPtmfDelta1 = ptmf[2];
    self->mLayout = 0;
    self->mField58 = 0;
    self->mField5C = 0;
    self->mField60 = -1;
    self->mField64 = 0;
    self->mField65 = 0;
    self->mField66 = 0;
    self->mField67 = 1;
    self->mField68 = 0;

    self->mVtab = (u32)lbl_eu_80536128;
    self->mOcc6C = (u32)lbl_eu_80536128 + 0x24;
    self->mOcc70 = (u32)lbl_eu_80536128 + 0xAC;
    self->mScene = scene;

    __ct__17UnkClass_8045F564Fv(&self->mMemRegion[0]);

    self->mAnim88 = 0;
    self->mState8C = 0;
    self->mValue90 = lbl_eu_80668568;

    u8 i;
    for (i = 0; i < 32; i++) {
        setQstEntry(&self->mEntries[i], 0, 0, 0, 0, 0);
    }
    setQstEntry(&self->mSelEntry, 0, 0, 0, 0, 0);
    self->mField19C = 0;

    for (i = 0; i < 32; i++) {
        QstEntry tmp;
        setQstEntry(&tmp, 0, 0, 0, 0, 0);
        copyQstEntry(&self->mEntries[i], &tmp);
    }
    {
        QstEntry tmp;
        setQstEntry(&tmp, a, b, c, d, e);
        copyQstEntry(&self->mSelEntry, &tmp);
    }
}

#pragma push
#pragma auto_inline off
QstEntry* copyQstEntry(QstEntry* dst, const QstEntry* src) {
    dst->f0 = src->f0;
    dst->f2 = src->f2;
    dst->f4 = src->f4;
    dst->f5 = src->f5;
    dst->f6 = src->f6;
    return dst;
}

QstEntry* copyQstEntry2(QstEntry* dst, const QstEntry* src) {
    dst->f0 = src->f0;
    dst->f2 = src->f2;
    dst->f4 = src->f4;
    dst->f5 = src->f5;
    dst->f6 = src->f6;
    return dst;
}
#pragma pop

#pragma push
#pragma optimize_for_size on
extern "C" CMenuQstCnt* __dt__11CMenuQstCntFv(CMenuQstCnt* self, int flags) {
    if (self != 0) {
        __dt__17UnkClass_8045F564Fv(&self->mMemRegion[0], -1);
        __dt__9IUIWindowFv(self, 0);
        if (flags > 0) {
            operator delete(self);
        }
    }
    return self;
}
#pragma pop

// C++-linkage retail symbol (getPackedFont__Fv): returns a u32 message/flag
// value used to fill the text fields below.
u32 getPackedFont();

void CMenuQstCnt::Init() {
    // Retail Init is -O4,s shaped (_savegpr_29 prologue, elf-vector frame);
    // the -O4,p unit emits individual stw otherwise. optimize_for_size
    // reproduces the retail prologue/epilogue (MWCC_CASES sec WUD).
#pragma push
#pragma optimize_for_size on
#pragma dont_inline on
    // Set up the scoped MEM2 region guard, then build the layout/animations.
    reinterpret_cast<UnkClass_8045F564*>(mMemRegion)
        ->createRegion((int)mtl::MemManager::getHandleMEM2(), 0x4000,
                       (const char*)lbl_eu_80509A10, 0);
    Class_8045F858 regionGuard(
        reinterpret_cast<UnkClass_8045F564*>(mMemRegion));

    buildLayout(&mLayout, CUICfManager_getArcResourceAccessor(),
                  &lbl_eu_80509A10[0xc]);
    bindLayoutAnimTransform(mLayout, &mAnim88, CUICfManager_getArcResourceAccessor(),
                  (char*)&lbl_eu_80509A10[0x25]);

    // Bind the font and hand the loaded font object over to the root pane.
    nw4r::lyt::Pane* rootPane = mLayout->GetRootPane();
    u8* fontObj = (u8*)CDeviceFont::getFontInfo(1, mLayout);
    u32 fontResult =
        reinterpret_cast<CMenuQstCntFont*>(fontObj)->getFontHandle();
    ApplyFontToTxtPanes(rootPane, (const nw4r::ut::Font*)fontResult);

    // First three text fields are filled straight from the message table.
    setLayoutTextBoxFont(mLayout, &lbl_eu_80509A10[0x41], getPackedFont());
    setLayoutTextBoxFont(mLayout, &lbl_eu_80509A10[0x4d], getPackedFont());
    setLayoutTextBoxFont(mLayout, &lbl_eu_80509A10[0x59], getPackedFont());

    LayoutSetTextBoxFmtValue(mLayout, &lbl_eu_80509A10[0x64], &lbl_eu_80509A10[0x73], 0);
    LayoutSetTextBoxFmtValue(mLayout, &lbl_eu_80509A10[0x74], &lbl_eu_80509A10[0x73], 0);

    // Helper returns formatted text for the two quest-log fields (the first,
    // 12-entry form, is computed and discarded); the remaining field uses it.
    BdatTouchStringCell(&lbl_eu_80509A10[0x7b], &lbl_eu_80509A10[0x85], 12);
    LayoutSetTextBoxFmtValue(mLayout, &lbl_eu_80509A10[0x8a], &lbl_eu_80509A10[0x73], 0);

    char* text =
        (char*)BdatTouchStringCell(&lbl_eu_80509A10[0x7b], &lbl_eu_80509A10[0x85], 11);
    LayoutSetTextBoxFmtValue(mLayout, &lbl_eu_80509A10[0x59], text, 0);

    // Start the open animation from frame 0 and tick it once.
    mLayout->SetAnimationEnable(mAnim88, true);
    mAnim88->SetFrame(lbl_eu_80668568);
    mLayout->Animate(0);

    mField67 = 0;
    this->RefreshDisplay();

    // Retail guards a dead null-check on this before picking the +0x70
    // IScnRender subobject (same shape as Term()).
    IScnRender* cb = reinterpret_cast<IScnRender*>(this);
    if (this != 0) {
        cb = reinterpret_cast<IScnRender*>(&this->mOcc70);
    }
    mScene->addRenderCB(cb, 0xa, 0);

    reinterpret_cast<UnkClass_8045F564*>(mMemRegion)->validateHeap();
    mField19C = 1;
}
#pragma pop

void CMenuQstCnt::Term() {
    waitForDrawDone__9CDeviceVIFv();
    IScnRender* cb = reinterpret_cast<IScnRender*>(this);
    if (this != 0) {
        cb = reinterpret_cast<IScnRender*>(&this->mOcc70);
    }
    mScene->removeRenderCB(cb);
    if (mLayout) {
        // Deleting-dtor dispatch: virtual ~Layout at vtable slot +8 (flag 1).
        delete mLayout;
        mLayout = 0;
    }
    deleteRegion__17UnkClass_8045F564Fv(&mMemRegion[0]);
    lbl_eu_80664720 = 0;
}

void CMenuQstCnt::Move() {
    CTaskGame::getInstance();
    // Single short-circuit OR so MWCC emits: func test -> bne exit;
    // bit test -> beq continue / b exit (CSystemWindow::Move shape).
    if (CTaskGame::isFlag01Set() || (lbl_eu_80663E28 & 0x200000)) return;
    if (!IsMenuState621F0()) return;
    // Retail loads the global twice up front (lwz r0; lwz r3) - the rlwinm
    // destroys r0 and the andis needs a second copy. Two adjacent volatile
    // reads stop MWCC CSE-ing them into one load while keeping both loads
    // before the branch like retail.
    u32 e24a = *(volatile u32*)&lbl_eu_80663E24;
    u32 e24b = *(volatile u32*)&lbl_eu_80663E24;
    if (!(e24a & (1u << 24)) && (e24b & 0xAFE40000u)) return;
    if (cf::CfGameManager::isSceneLoading()) return;
    if (lbl_eu_80663E24 & (1u << 9)) return;
    {
        CTaskGameCamView* cam = (CTaskGameCamView*)Scn_QueryUnk80State(lbl_eu_80663E14);
        // Load camDist into a local first so MWCC emits lfs f1,0xc(r3)
        // before lfs f0,CONST (retail operand order for the fsubs/fcmpo).
        f32 camDist = *(f32*)((u8*)cam + 0xC);
        f32 result = lbl_eu_8066856C - camDist;
        if (result < lbl_eu_8066856C) return;
    }
    if (isQuestLogMenuActive()) return;
    if (PTNotice_IsActive_3C10()) return;
    if (MenuTutorialIsCreated()) return;
    if (GetItemMulti_IsActiveFlag()) return;
    if (SysWinGetSingleton()) return;
    // Retail compares the state with signed cmpi.
    switch ((s32)mState8C) {
    case 0: QstCnt_SignalReady_6BBC((QstMenuData*)this); break;
    case 1: QstCnt_AdvanceToOpen_6C18((QstMenuData*)this); break;
    case 2: QstCnt_TickToOpened_6C5C((QstMenuData*)this); break;
    case 3: QstCnt_FinishAnim(this); break;
    }
    // Tail virtual call: Layout::Animate (vtable slot 0x38), option 0.
    mLayout->Animate(0);
}

void CMenuQstCnt::cbRenderBefore() {
    // Single short-circuit OR so MWCC emits: func test -> bne exit;
    // bit test -> beq continue / b exit (CSystemWindow::Move shape).
    if (CTaskGame::getInstance()->isFlag01Set() ||
        (lbl_eu_80663E28 & 0x200000))
        return;
    if (IsMenuState621F0() == 0) return;
    // Retail loads the global twice up front (lwz r0; lwz r3) - the rlwinm
    // destroys r0 and the andis needs a second copy. Two adjacent volatile
    // reads stop MWCC CSE-ing them into one load (the missing 4 bytes) while
    // keeping both loads before the branch like retail.
    u32 e24a = *(volatile u32*)&lbl_eu_80663E24;
    u32 e24b = *(volatile u32*)&lbl_eu_80663E24;
    if (!(e24a & (1u << 24)) && (e24b & 0xAFE40000u)) return;
    if (cf::CfGameManager::isSceneLoading()) return;
    {
        CTaskGameCamView* cam = (CTaskGameCamView*)Scn_QueryUnk80State(lbl_eu_80663E14);
        f32 camDist = cam->field_C;
        f32 result = lbl_eu_8066856C - camDist;
        if (result < lbl_eu_8066856C) return;
    }
    if (PTNotice_IsActive_3C10()) return;
    if (MenuTutorialIsCreated()) return;
    if (GetItemMulti_IsActiveFlag()) return;
    if (SysWinGetSingleton()) return;
    if (mField64 != 0) return;
    GXSetZMode(GX_FALSE, GX_NEVER, GX_FALSE);
    nw4r::lyt::DrawInfo drawInfo;
    SetWideLocAdjust(&drawInfo);
    drawLayout(mLayout, &drawInfo, 0, 1);
}

// Quest-log menu factory/update. When the singleton already exists and is
// active, only sync the new selection into it (returning 0); otherwise
// allocate + construct + register a fresh menu (returning the object).
CMenuQstCnt* QstCnt_GetOrCreate(CProcess* parent, CScn* scene, short a, short b, unsigned char c, unsigned char d, unsigned char e) {
    CMenuQstCnt* g = lbl_eu_80664720;
    if (g != 0 && g->mField64 == 0) {
        int changed = 0;
        if (g->mSelEntry.f0 != (u16)a || g->mSelEntry.f2 != (u16)b) {
            changed = 1;
        }
        if (!changed) {
            switch (g->mState8C) {
            case 0:
                g->mSelEntry.f4 = c;
                if (lbl_eu_80664720->mField19C) lbl_eu_80664720->RefreshDisplay();
                break;
            case 1:
                g->mSelEntry.f4 = c;
                lbl_eu_80664720->RefreshDisplay();
                break;
            case 2:
                g->mSelEntry.f4 = c;
                lbl_eu_80664720->mValue90 = lbl_eu_80668568;
                lbl_eu_80664720->RefreshDisplay();
                break;
            default:
                changed = 1;
                break;
            }
        }
        if (changed) {
            u8 i;
            for (i = 0; i < 0x20; i++) {
                if (g->mEntries[i].f0 == 0) {
                    QstEntry tmp;
                    setQstEntry(&tmp, a, b, c, d, e);
                    copyQstEntry(&g->mEntries[i], &tmp);
                    break;
                }
            }
        }
        return 0;
    }
    u32 mem = getWorkMem__17CWorkThreadSystemFv();
    CMenuQstCnt* obj = (CMenuQstCnt*)allocate__Q23mtl10MemManagerFUlUl(0x1A0, mem);
    if (obj) {
        __ct__CMenuQstCnt(obj, scene, a, b, c, d, e);
    }
    lbl_eu_80664720 = obj;
    Regist__8CProcessFP8CProcessb(obj, parent, 0);
    return lbl_eu_80664720;
}


void invalidateQstFlag() {
    if (lbl_eu_80664720 != 0) {
        lbl_eu_80664720->mField64 = 1;
    }
}

// Quest-log gating: when the CF-game resource gate reports the scene is open
// (not mid-load) and the input state allows it, play the sound and set state 1.
void QstCnt_SignalReady_6BBC(QstMenuData* self) {
    if (isLandTelopIdle() == 0 || cf::CfGameManager::isSceneLoading() || getLandTelopE0()) {
        playUISound(0x1f);
        self->mState8C = 1;
    }
}

// Advance the open/close animation at 0x88 by one frame; when it finishes the
// animation (advanceAnimTransform returns 1), move to state 2.
void QstCnt_AdvanceToOpen_6C18(QstMenuData* self) {
    if (advanceAnimTransform(self->mAnim88, 1.0f) != 0) {
        self->mState8C = 2;
    }
}

// Per-frame timer: 0x90 counts frames (1 per call); once it reaches 90 frames
// it clamps to state 3. Uses >= (cror eq,gt,eq; bnelr returns while below).
void QstCnt_TickToOpened_6C5C(QstMenuData* self) {
    self->mValue90 += 1.0f;
    if (self->mValue90 >= 90.0f) {
        self->mState8C = 3;
    }
}

// Finish the quest-log animation. Pull the head quest into the selection,
// compact the entry list (keeping only non-zero quest ids), and refresh the
// displayed quest info.
// Retail's prologue is -O4,s shaped (_savegpr_29 at 3 saved regs, elf-vector
// frame); the -O4,p unit emits individual stw. optimize_for_size reproduces
// the retail -O4,s codegen for this function (MWCC_CASES sec WUD), and
// dont_inline stops it being folded into Move.
#pragma push
#pragma optimize_for_size on
#pragma dont_inline on
void QstCnt_FinishAnim(CMenuQstCnt* self) {
    if (AnimRewindFrame(self->mAnim88, lbl_eu_8066856C) == 0) return;
    if (self->mEntries[0].f0 == 0) {
        self->mField64 = 1;
        return;
    }
    self->mValue90 = lbl_eu_80668568;
    self->mState8C = 0;
    QstEntry cur;
    copyQstEntry(&self->mSelEntry, copyQstEntry2(&cur, &self->mEntries[0]));
    QstEntry tmp0;
    setQstEntry(&tmp0, 0, 0, 0, 0, 0);
    copyQstEntry(&self->mEntries[0], &tmp0);
    self->RefreshDisplay();

    QstEntry scratch[32];
    u8 i, j;
    for (i = 0; i < 32; i++) {
        setQstEntry(&scratch[i], 0, 0, 0, 0, 0);
    }
    for (i = 0; i < 32; i++) {
        QstEntry t;
        setQstEntry(&t, 0, 0, 0, 0, 0);
        copyQstEntry(&scratch[i], &t);
    }
    j = 0;
    for (i = 0; i < 32; i++) {
        if (self->mEntries[i].f0 != 0) {
            QstEntry t;
            copyQstEntry2(&t, &self->mEntries[i]);
            copyQstEntry(&scratch[j], &t);
            j++;
        }
    }
    for (j = 0; j < 32; j++) {
        QstEntry t;
        copyQstEntry2(&t, &scratch[j]);
        copyQstEntry(&self->mEntries[j], &t);
    }
}
#pragma pop

void CMenuQstCnt::RefreshDisplay() {
    if (this->mSelEntry.f0 == 0) return;
    if (this->mSelEntry.f4 > this->mSelEntry.f5) {
        this->mSelEntry.f4 = this->mSelEntry.f5;
    }
    u32 v = SearchThresholdIdx(this->mSelEntry.f0);
    char* s = BdatGetPtrDirect((void*)lbl_eu_80573D18[v], &lbl_eu_80509A10[0x92], this->mSelEntry.f0);
    LayoutSetTextBoxFmtValue(this->mLayout, &lbl_eu_80509A10[0x64], s, 0);
    if (this->mSelEntry.f6 == 0) {
        s = BdatGetPtrDirect(lbl_eu_80664094, &lbl_eu_80509A10[0x85], this->mSelEntry.f2);
    } else {
        s = BuildItemNameStr(this->mSelEntry.f2);
    }
    LayoutSetTextBoxFmtValue(this->mLayout, &lbl_eu_80509A10[0x74], s, 0);
    if (this->mSelEntry.f6 == 0) {
        s = (char*)BdatTouchStringCell(&lbl_eu_80509A10[0x7b], &lbl_eu_80509A10[0x85], 12);
    } else {
        s = (char*)BdatTouchStringCell(&lbl_eu_80509A10[0x7b], &lbl_eu_80509A10[0x85], 13);
    }
    LayoutSetTextBoxFmtValue(this->mLayout, &lbl_eu_80509A10[0x8a], s, 0);
    setLayoutTextBoxNumber(this->mLayout, &lbl_eu_80509A10[0x41], this->mSelEntry.f4);
    setLayoutTextBoxNumber(this->mLayout, &lbl_eu_80509A10[0x4d], this->mSelEntry.f5);
}

QstData::QstData() {
    mCount = 0;
    mFilterCount = 0;
    mSortKind = 0xff;
    mSortFilter = 0xff;
}

// Rebuild the 0x400-entry quest-info table from the game's quest catalog.
// Categories 1..0x1A (ranges from GetU16PairEntry) each contribute their quests;
// entries whose kind is 0 / 0xC8 are skipped.
void QstData::Refresh() {
    mCount = 0;
    u16 cat;
    for (cat = 1; cat < 0x1B; cat++) {
        u32 start = GetU16PairEntry(cat);
        if ((start & 0xffff) == 0) continue;
        u16 end = GetU16PairEntry(cat + 1);
        u16 i = (u16)start;
        for (; i < end; i++) {
            u8 kind = (u8)CtrlRemote_TouchBitByArg(i + 0x220);
            if (kind == 0 || kind == 0xC8) continue;
            int kindClass;
            if (kind >= 0xFE) {
                kindClass = 3;
            } else if (kind == 1) {
                kindClass = 1;
            } else {
                kindClass = 2;
            }
            u32 v = SearchThresholdIdx(i);
            u32 tbl = lbl_eu_80573D18[v];
            u32 res = BdatGetU8Direct((const void*)tbl, &lbl_eu_80509AB4[0], i);
            if ((res & 0xff) == 2) continue;
            u32 showFlag = (res & 0xff) == 1;
            if (kindClass == 3) showFlag = 0;
            u8 x = (u8)BdatGetU8Direct((const void*)lbl_eu_806640A8, &lbl_eu_80509AB4[0xa], cat);
            mList[mCount++] = QstInfo(i, kindClass, cat, kind, showFlag, res, x);
        }
    }
}

void QstData::Sort(u32 kind, u32 filter) {
    // Select/re-select the sort column. kind==4 uses the quest-level filter
    // (mSortFilter); otherwise kind is the column (mSortKind). If the selection
    // didn't change, there is nothing to re-sort.
    if (kind == 4) {
        if (mSortFilter == filter) return;
        mSortKind = kind;
        mSortFilter = filter;
    } else {
        if (mSortKind == kind) return;
        mSortKind = kind;
        mSortFilter = 0;
    }
    mFilterCount = -1;   // "unsorted" marker until the count is recomputed

    // Pass 1: bubble sort quest entries by quest id ascending.
    for (u16 i = 0; i < mCount - 1; i++) {
        bool changed = true;
        for (u16 j = 0; j < mCount - 1 - i; j++) {
            QstInfo* pa = &mList[j];
            QstInfo* pb = &mList[j + 1];
            if (pa->mId > pb->mId) {
                const QstInfo tmp(*pa);
                *pa = QstInfo(*pb);
                *pb = QstInfo(tmp);
                changed = false;
            }
        }
        if (changed) break;
    }

    // Pass 2: bubble sort by category order ascending.
    for (u16 i = 0; i < mCount - 1; i++) {
        bool changed = true;
        for (u16 j = 0; j < mCount - 1 - i; j++) {
            QstInfo* pa = &mList[j];
            QstInfo* pb = &mList[j + 1];
            if (pa->mOrder > pb->mOrder) {
                const QstInfo tmp(*pa);
                *pa = QstInfo(*pb);
                *pb = QstInfo(tmp);
                changed = false;
            }
        }
        if (changed) break;
    }

    // Pass 3: bubble sort by show flag descending.
    for (u16 i = 0; i < mCount - 1; i++) {
        bool changed = true;
        for (u16 j = 0; j < mCount - 1 - i; j++) {
            QstInfo* pa = &mList[j];
            QstInfo* pb = &mList[j + 1];
            if (pa->mShow < pb->mShow) {
                const QstInfo tmp(*pa);
                *pa = QstInfo(*pb);
                *pb = QstInfo(tmp);
                changed = false;
            }
        }
        if (changed) break;
    }

    // When a column filter is active, bubble the matching entries to the front
    // (sort by match flag ascending, so matches land first), then count them.
    if (kind != 0) {
        for (u16 i = 0; i < mCount - 1; i++) {
            bool changed = true;
            for (u16 j = 0; j < mCount - 1 - i; j++) {
                QstInfo* pa = &mList[j];
                QstInfo* pb = &mList[j + 1];
                u8 m1 = (kind == 4) ? (pa->mCategory == filter)
                                    : (pa->mKind == kind);
                u8 m2 = (kind == 4) ? (pb->mCategory == filter)
                                    : (pb->mKind == kind);
                if (m1 < m2) {
                    const QstInfo tmp(*pa);
                    *pa = QstInfo(*pb);
                    *pb = QstInfo(tmp);
                    changed = false;
                }
            }
            if (changed) break;
        }

        mFilterCount = 0;
        for (u16 k = 0; k < mCount; k++) {
            u8 m = (kind == 4) ? (mList[k].mCategory == filter)
                               : (mList[k].mKind == kind);
            if (m == 0) break;
            mFilterCount++;
        }
    }
}

u16 QstData::GetCount() {
    short r = mFilterCount;
    // mFilterCount (signed) is negative -> use the u16 count instead. Keep the
    // temp as short so the single u16 conversion/clrlwi happens at the join.
    if (r < 0)
        r = (short)mCount;
    return (unsigned short)r;
}

QstInfo* QstData::GetInfo(u16 idx) {
    if (idx >= 0x400) return 0;
    return &mList[idx];
}

// Returns the index of the entry with the given quest id, or 0.
u16 QstData::FindIndex(u16 id) {
    for (u16 i = 0; i < mCount; i++) {
        if (mList[i].mId == id) return i;
    }
    return 0;
}

// --- IScnRender vtable this-adjusting thunks ---
// The IScnRender subobject sits at +0x70 within CMenuQstCnt; vtable dispatch
// through that subobject passes 'this' pointing at +0x70, so each thunk
// subtracts it back and tail-calls the real implementation.
// Retail: subi r3, r3, 0x70; b cbRenderBefore__11CMenuQstCntFv
void renderQstBase_70(CMenuQstCnt* sub) {
    reinterpret_cast<CMenuQstCnt*>(reinterpret_cast<char*>(sub) - 0x70)->cbRenderBefore();
}

// Retail: subi r3, r3, 0x6c; b __dt__11CMenuQstCntFv
void dtorQstBase_6C(u8* sub) {
    ((void(*)(void*))__dt__11CMenuQstCntFv)(sub - 0x6C);
}

// Retail: subi r3, r3, 0x70; b __dt__11CMenuQstCntFv
void dtorQstBase_70(u8* sub) {
    ((void(*)(void*))__dt__11CMenuQstCntFv)(sub - 0x70);
}
