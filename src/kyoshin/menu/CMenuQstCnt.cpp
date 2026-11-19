// kyoshin/menu/CMenuQstCnt - quest-status menu screen.
//
// Real MI class (IUIWindow + IWorkEvent + IScnRender, see the header): the
// composite vtable __vt__11CMenuQstCnt, its 0x40 base list, the weak RTTI
// record and the "CMenuQstCnt" type-name string are compiler output.

#define IWORK_EVENT_INLINE_DTOR
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
#include "kyoshin/CUICfManager_Res.hpp"

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
CMenuQstCnt* QstCnt_GetOrCreate(CProcess* parent, CScn* scene, u16 a, u16 b, unsigned char c, unsigned char d, unsigned char e);
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


// ---------------------------------------------------------------------------
// CMenuQstCnt (retail 0x802280F8): CProcess base ctor, then the inlined
// CTTask<IUIWindow>/IUIWindow ctor bodies (base vptr + null ptmf hooks +
// layout/state members), the composite + sub-vptrs, the embedded MEM2 region,
// the 32-entry page table and the selected entry.
// ---------------------------------------------------------------------------
CMenuQstCnt::CMenuQstCnt(CScn* scene, short a, short b, u8 c, u8 d, u8 e)
    : mScene(scene), mMemRegion(), mAnim88(0), mState8C(0), mValue90(0.0f),
      mField19C(0) {
    for (u8 i = 0; i < 32; i++) {
        mEntries[i] = QstEntry(0, 0, 0, 0, 0);
    }
    mSelEntry = QstEntry(a, b, c, d, e);
}

QstEntry::QstEntry(int a, int b, u8 c, u8 d, u8 e) {
    f0 = a;
    f2 = b;
    f4 = c;
    f5 = d;
    f6 = e;
}

QstEntry& QstEntry::operator=(const QstEntry& other) {
    f0 = other.f0;
    f2 = other.f2;
    f4 = other.f4;
    f5 = other.f5;
    f6 = other.f6;
    return *this;
}

// ---------------------------------------------------------------------------
// CMenuQstCnt::~CMenuQstCnt (retail 0x80228254): the IWorkEvent/IScnRender
// dtors are inline, the IUIWindow base dtor is out of line (retail
// `bl __dt__9IUIWindowFv`), the MEM2 region runs its member dtor, then the
// deleting path calls operator delete.
// ---------------------------------------------------------------------------
#pragma push
CMenuQstCnt::~CMenuQstCnt() {}
#pragma pop

// C++-linkage retail symbol (getPackedFont__Fv): returns a u32 message/flag
// value used to fill the text fields below.

void CMenuQstCnt::Init() {
    // Retail Init is -O4,s shaped (_savegpr_29 prologue, elf-vector frame).
#pragma push
    // Set up the scoped MEM2 region guard, then build the layout/animations.
    mMemRegion.createRegion((int)mtl::MemManager::getHandleMEM2(), 0x4000,
                            "CMenuQstCnt", 0);
    Class_8045F858 regionGuard(&mMemRegion);

    buildLayout(&mLayout, CUICfManager_getArcResourceAccessor(),
                "mf70_cf60_qst_cunt.brlyt");
    bindLayoutAnimTransform(mLayout, &mAnim88, CUICfManager_getArcResourceAccessor(),
                            "mf70_cf60_qst_cunt_in.brlan");

    // Bind the font and hand the loaded font object over to the root pane.
    nw4r::lyt::Pane* rootPane = mLayout->GetRootPane();
    u8* fontObj = (u8*)CDeviceFont::getFontInfo(1, mLayout);
    u32 fontResult =
        reinterpret_cast<CMenuQstCntFont*>(fontObj)->getFontHandle();
    ApplyFontToTxtPanes(rootPane, (const nw4r::ut::Font*)fontResult);

    // First three text fields are filled straight from the message table.
    setLayoutTextBoxFont(mLayout, "txt_value00", (u32)getPackedFont());
    setLayoutTextBoxFont(mLayout, "txt_value01", (u32)getPackedFont());
    setLayoutTextBoxFont(mLayout, "txt_para00", (u32)getPackedFont());

    LayoutSetTextBoxFmtValue(mLayout, "txt_questtitle", "", 0);
    LayoutSetTextBoxFmtValue(mLayout, "txt_en", "", 0);

    // Helper returns formatted text for the two quest-log fields (the first,
    // 12-entry form, is computed and discarded); the remaining field uses it.
    BdatTouchStringCell("MNU_quest", "name", 12);
    LayoutSetTextBoxFmtValue(mLayout, "txt_num", "", 0);

    char* text = (char*)BdatTouchStringCell("MNU_quest", "name", 11);
    LayoutSetTextBoxFmtValue(mLayout, "txt_para00", text, 0);

    // Start the open animation from frame 0 and tick it once.
    mLayout->SetAnimationEnable(mAnim88, true);
    mAnim88->SetFrame(0.0f);
    mLayout->Animate(0);

    mField67 = 0;
    this->RefreshDisplay();

    mScene->addRenderCB(this, 0xa, 0);

    mMemRegion.validateHeap();
    mField19C = 1;
}
#pragma pop

void CMenuQstCnt::Term() {
    waitForDrawDone__9CDeviceVIFv();
    mScene->removeRenderCB(this);
    if (mLayout) {
        // Deleting-dtor dispatch: virtual ~Layout at vtable slot +8 (flag 1).
        delete mLayout;
        mLayout = 0;
    }
    mMemRegion.deleteRegion();
    lbl_eu_80664720 = 0;
}

void CMenuQstCnt::Move() {
    CTaskGame::getInstance();
    // Single short-circuit OR so MWCC emits: func test -> bne exit;
    // bit test -> beq continue / b exit (CSystemWindow::Move shape).
    if (CTaskGame::isFlag01Set() || (gmPauseFlags & 0x200000)) return;
    if (!IsMenuState621F0()) return;
    // Retail loads the global twice up front (lwz r0; lwz r3) - the rlwinm
    // destroys r0 and the andis needs a second copy. Two adjacent volatile
    // reads stop MWCC CSE-ing them into one load while keeping both loads
    // before the branch like retail.
    u32 e24a = *(volatile u32*)&gmGameFlags;
    u32 e24b = *(volatile u32*)&gmGameFlags;
    if (!(e24a & (1u << 24)) && (e24b & 0xAFE40000u)) return;
    if (cf::CfGameManager::isSceneLoading()) return;
    if (gmGameFlags & (1u << 9)) return;
    {
        CTaskGameCamView* cam = (CTaskGameCamView*)Scn_QueryUnk80State(gmScene);
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
        (gmPauseFlags & 0x200000))
        return;
    if (IsMenuState621F0() == 0) return;
    // Retail loads the global twice up front (lwz r0; lwz r3) - the rlwinm
    // destroys r0 and the andis needs a second copy. Two adjacent volatile
    // reads stop MWCC CSE-ing them into one load (the missing 4 bytes) while
    // keeping both loads before the branch like retail.
    u32 e24a = *(volatile u32*)&gmGameFlags;
    u32 e24b = *(volatile u32*)&gmGameFlags;
    if (!(e24a & (1u << 24)) && (e24b & 0xAFE40000u)) return;
    if (cf::CfGameManager::isSceneLoading()) return;
    {
        CTaskGameCamView* cam = (CTaskGameCamView*)Scn_QueryUnk80State(gmScene);
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
CMenuQstCnt* QstCnt_GetOrCreate(CProcess* parent, CScn* scene, u16 a, u16 b, unsigned char c, unsigned char d, unsigned char e) {
    CMenuQstCnt* g = lbl_eu_80664720;
    if (g != 0 && g->mField64 == 0) {
        int changed = 0;
        if (g->mSelEntry.f0 == a) {
            if (g->mSelEntry.f2 != b) {
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
        } else {
            changed = 1;
        }
        if (changed) {
            for (u8 i = 0; i < 0x20; i++) {
                if (lbl_eu_80664720->mEntries[i].f0 == 0) {
                    lbl_eu_80664720->mEntries[i] = QstEntry(a, b, c, d, e);
                    break;
                }
            }
        }
        return 0;
    }
    CMenuQstCnt* obj = new (CWorkThreadSystem::getWorkMem()) CMenuQstCnt(scene, a, b, c, d, e);
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
// Retail's prologue is -O4,s shaped (_savegpr_29 at 3 saved regs, elf-vector frame).
void QstCnt_FinishAnim(CMenuQstCnt* self) {
    if (AnimRewindFrame(self->mAnim88, lbl_eu_8066856C) == 0) return;
    if (self->mEntries[0].f0 == 0) {
        self->mField64 = 1;
        return;
    }
    self->mValue90 = lbl_eu_80668568;
    self->mState8C = 0;
    self->mSelEntry = QstEntry(self->mEntries[0]);
    self->mEntries[0] = QstEntry(0, 0, 0, 0, 0);
    self->RefreshDisplay();

    QstEntry scratch[32];
    u8 i, j;
    for (i = 0; i < 32; i++) {
        scratch[i] = QstEntry(0, 0, 0, 0, 0);
    }
    for (i = 0, j = 0; i < 32; i++) {
        if (self->mEntries[i].f0 != 0) {
            scratch[j++] = QstEntry(self->mEntries[i]);
        }
    }
    for (j = 0; j < 32; j++) {
        self->mEntries[j] = QstEntry(scratch[j]);
    }
}

QstEntry::QstEntry(const QstEntry& other) {
    f0 = other.f0;
    f2 = other.f2;
    f4 = other.f4;
    f5 = other.f5;
    f6 = other.f6;
}

void CMenuQstCnt::RefreshDisplay() {
    if (this->mSelEntry.f0 == 0) return;
    if (this->mSelEntry.f4 > this->mSelEntry.f5) {
        this->mSelEntry.f4 = this->mSelEntry.f5;
    }
    u32 v = SearchThresholdIdx(this->mSelEntry.f0);
    char* s = BdatGetPtrDirect((void*)lbl_eu_80573D18[v], "title", this->mSelEntry.f0);
    LayoutSetTextBoxFmtValue(this->mLayout, "txt_questtitle", s, 0);
    if (this->mSelEntry.f6 == 0) {
        s = BdatGetPtrDirect(lbl_eu_80664094, "name", this->mSelEntry.f2);
    } else {
        s = BuildItemNameStr(this->mSelEntry.f2);
    }
    LayoutSetTextBoxFmtValue(this->mLayout, "txt_en", s, 0);
    if (this->mSelEntry.f6 == 0) {
        s = (char*)BdatTouchStringCell("MNU_quest", "name", 12);
    } else {
        s = (char*)BdatTouchStringCell("MNU_quest", "name", 13);
    }
    LayoutSetTextBoxFmtValue(this->mLayout, "txt_num", s, 0);
    setLayoutTextBoxNumber(this->mLayout, "txt_value00", this->mSelEntry.f4);
    setLayoutTextBoxNumber(this->mLayout, "txt_value01", this->mSelEntry.f5);
}

