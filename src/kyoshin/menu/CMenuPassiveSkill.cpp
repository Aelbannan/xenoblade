// Auto-scaffolded catalog TU for kyoshin/menu/CMenuPassiveSkill
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/cf/CfGameManager.hpp"
#include "kyoshin/harness_catalog.hpp"
#include "kyoshin/CPassiveSkill.hpp"
#include "kyoshin/menu/CMenuPassiveSkill.hpp"
#include "kyoshin/CTaskGame.hpp"
#include "monolib/device/CDeviceVI.hpp"
#include "monolib/work/CWorkThreadSystem.hpp"
#include "monolib/util/MemManager.hpp"
#include <revolution/GX.h>
#include <nw4r/lyt/lyt_drawInfo.h>
#include "kyoshin/menu/CMenuPassiveSkill.hpp"
#include "kyoshin/CMainMenu.hpp" // CMainMenu_GetInstancePtr
#include "kyoshin/CTitleAHelp.hpp"
#include "kyoshin/CTitleAHelpImports.hpp"
#include "kyoshin/code_80135FDC_State.hpp"

CMenuPassiveSkill::CMenuPassiveSkill(CScn* scene)
    : mScnRef(scene), mBgTex(0), mTitleAHelp(nullptr, 0) {
    __ct__UI_CPassiveSkill(mPassiveSkill);
    field_2AC = 0;
    field_2B0 = 0.0f;
}

// The embedded UI::CPassiveSkill is raw storage, so its teardown is the only
// work here; CBgTex / CTitleAHelp / IUICf are destroyed by the compiler.
CMenuPassiveSkill::~CMenuPassiveSkill() {
    __dt__Q22UI13CPassiveSkillFv(mPassiveSkill, -1);
}

// ---------------------------------------------------------------------------
// Re-initialise the passive-skill menu screen: rebuild each embedded widget
// (CBgTex / CTitleAHelp / UI::CPassiveSkill) by constructing a stack temporary
// and copying its body into the embedded storage (vtable word skipped; mem
// regions via __as__17UnkClass_8045F564FRC17UnkClass_8045F564; the 5x5 skill-grid cells through the
// counted 8-byte pair loops), then register this screen as a render callback
// on the owning scene (CMenuCollepedia::Init pattern).
// ---------------------------------------------------------------------------
void CMenuPassiveSkill::Init() {
    setPresentationFlag__Q22cf13CfGameManagerFv(1);

    // --- Re-initialise the background CBgTex via a temporary ---
    // No cached temp/sub pointers: every access folds to a sp/r31-relative
    // address so retail's single-saved-register prologue holds.
    u8 tempBgTex[0x20];
    __ct__6CBgTexFUc((CBgTex*)tempBgTex, 0);

    __as__17UnkClass_8045F564FRC17UnkClass_8045F564(&mBgTex.mMemRegion, tempBgTex + 0x4);
    mBgTex.mFileHandle = *(CFileHandle**)(tempBgTex + 0x14);
    mBgTex.mLayout = *(nw4r::lyt::Layout**)(tempBgTex + 0x18);
    mBgTex.mLayoutReady = *(bool*)(tempBgTex + 0x1c);
    mBgTex.mLoaded = *(u8*)(tempBgTex + 0x1d);
    mBgTex.mPtmMode = *(u8*)(tempBgTex + 0x1e);
    __dt__6CBgTexFv((CBgTex*)tempBgTex, -1);

    BgTex_Acquire_3C14(&mBgTex);

    // --- Re-initialise the embedded CTitleAHelp ---
    char* name = BdatTouchStringCell(lbl_eu_8050DB4C, lbl_eu_8050DB4C + 0xc, 1);

    u8 tempTitle[0x38];
    __ct__11CTitleAHelpFPcUc((CTitleAHelp*)tempTitle, name, 0x5a);

    __as__17UnkClass_8045F564FRC17UnkClass_8045F564(&mTitleAHelp.unk4, tempTitle + 0x4);
    mTitleAHelp.mFileHandle = *(CFileHandle**)(tempTitle + 0x14);
    mTitleAHelp.mArcResourceAccessor =
        *(nw4r::lyt::ArcResourceAccessor**)(tempTitle + 0x18);
    mTitleAHelp.mLayout = *(nw4r::lyt::Layout**)(tempTitle + 0x1c);
    mTitleAHelp.mAnimTrans20 = *(nw4r::lyt::AnimTransform**)(tempTitle + 0x20);
    mTitleAHelp.mAnimTrans24 = *(nw4r::lyt::AnimTransform**)(tempTitle + 0x24);
    mTitleAHelp.unk28 = *(u8*)(tempTitle + 0x28);
    mTitleAHelp.unk2c = *(s32*)(tempTitle + 0x2c);
    mTitleAHelp.mName = *(char**)(tempTitle + 0x30);
    mTitleAHelp.unk34 = *(u8*)(tempTitle + 0x34);
    mTitleAHelp.unk35 = *(u8*)(tempTitle + 0x35);
    mTitleAHelp.unk36 = *(u8*)(tempTitle + 0x36);
    mTitleAHelp.unk37 = *(u8*)(tempTitle + 0x37);
    __dt__11CTitleAHelpFv((CTitleAHelp*)tempTitle, -1);

    mTitleAHelp.CTitleAHelp_load();

    // --- Re-initialise the embedded UI::CPassiveSkill via a temporary ---
    u8 tempPS[0x1F4];
    __ct__UI_CPassiveSkill(tempPS);

    CMenuPassivePSView& p = *(CMenuPassivePSView*)tempPS;
    CMenuPassivePSView& q = *(CMenuPassivePSView*)&mPassiveSkill[0];

    q.field_4 = p.field_4;
    __as__17UnkClass_8045F564FRC17UnkClass_8045F564(q.mUnk8, p.mUnk8);

    q.field_18 = p.field_18;
    q.field_1C = p.field_1C;
    q.field_20 = p.field_20;
    q.field_24 = p.field_24;
    q.field_25 = p.field_25;
    q.field_26 = p.field_26;
    q.field_27 = p.field_27;
    q.field_2C = p.field_2C;
    q.field_30 = p.field_30;
    q.field_34 = p.field_34;
    q.field_38 = p.field_38;
    q.field_3C = p.field_3C;
    q.field_40 = p.field_40;

    // 5x5 skill-grid cell block (+0x44, 0xCC bytes) assigned as a unit so MWCC
    // emits the retail unrolled-word + counted lwzu/stwu pair loop.
    q.block1 = p.block1;

    q.headBlock = p.headBlock;
    q.field_11A[0] = p.field_11A[0];
    q.field_11A[1] = p.field_11A[1];
    q.field_11A[2] = p.field_11A[2];
    q.field_11A[3] = p.field_11A[3];
    q.field_11A[4] = p.field_11A[4];
    q.field_11A[5] = p.field_11A[5];
    q.field_11A[6] = p.field_11A[6];
    q.field_11A[7] = p.field_11A[7];
    q.field_11A[8] = p.field_11A[8];
    q.field_11A[9] = p.field_11A[9];
    q.field_11A[10] = p.field_11A[10];
    q.field_11A[11] = p.field_11A[11];
    q.field_11A[12] = p.field_11A[12];

    // Second counted loop over the 3 remaining cell pairs, based at the
    // +0x128 frame float; the final byte (+0x144) rides the still-live
    // loop pointers after the loop (retail lbz 4(r4)/stb 4(r5)).
    CPSkillCellPair* dst = (CPSkillCellPair*)&q.field_128;
    CPSkillCellPair* src = (CPSkillCellPair*)&p.field_128;

    q.field_128 = p.field_128;

    u32 count = 3;
    do {
        dst->field_4 = src->field_4;
        dst->field_0 = src->field_0;
        dst++;
        src++;
    } while (--count != 0);
    dst->field_4 = src->field_4;

    q.field_145 = p.field_145;
    q.field_146 = p.field_146;
    q.field_147 = p.field_147;
    q.field_14C = p.field_14C;
    q.field_150 = p.field_150;
    q.field_154 = p.field_154;
    q.field_158 = p.field_158;
    q.field_15C = p.field_15C;
    q.field_160[0] = p.field_160[0];
    q.field_160[1] = p.field_160[1];
    q.field_160[2] = p.field_160[2];
    q.field_160[3] = p.field_160[3];
    q.field_164 = p.field_164;
    q.field_168 = p.field_168;
    q.field_16C = p.field_16C;
    q.field_170 = p.field_170;
    q.field_174 = p.field_174;
    q.field_178 = p.field_178;
    q.field_17C = p.field_17C;
    q.field_180[0] = p.field_180[0];
    q.field_180[1] = p.field_180[1];
    q.field_180[2] = p.field_180[2];
    q.field_180[3] = p.field_180[3];
    q.field_188 = p.field_188;
    q.field_18C = p.field_18C;
    q.field_190 = p.field_190;
    q.field_194 = p.field_194;
    q.field_198[0] = p.field_198[0];
    q.field_198[1] = p.field_198[1];
    q.field_198[2] = p.field_198[2];
    q.field_19C = p.field_19C;
    q.field_1A4 = p.field_1A4;
    q.field_1A8 = p.field_1A8;
    q.field_1AC = p.field_1AC;
    q.field_1B0 = p.field_1B0;
    q.field_1B4 = p.field_1B4;
    q.field_1B5 = p.field_1B5;

    __as__17UnkClass_8045F564FRC17UnkClass_8045F564(q.mUnk1BC, p.mUnk1BC);
    q.field_1CC = p.field_1CC;
    q.field_1D0 = p.field_1D0;
    q.field_1D4 = p.field_1D4;
    q.field_1D8 = p.field_1D8;
    q.field_1DC = p.field_1DC;
    q.field_1E0 = p.field_1E0;
    q.field_1E4 = p.field_1E4;
    q.field_1E8 = p.field_1E8;
    q.field_1EC[0] = p.field_1EC[0];
    q.field_1EC[1] = p.field_1EC[1];
    q.field_1EC[2] = p.field_1EC[2];
    q.field_1EC[3] = p.field_1EC[3];
    q.field_1EC[4] = p.field_1EC[4];
    q.field_1EC[5] = p.field_1EC[5];

    __dt__Q22UI13CPassiveSkillFv(tempPS, -1);

    CPassiveSkill_loadSkillData(&mPassiveSkill[0]);

    // Register this screen as a render callback on the owning scene.
    addRenderCB__4CScnFP10IScnRenderUlUl(mScnRef, this, 0xd, 0);
}

// ---------------------------------------------------------------------------
// Term (us-80265b7c): wait for VI idle, drop this screen's render callback
// from the owning scene, tear down the three embedded widgets in reverse
// construction order (bgTex / titleAHelp / passiveSkill), clear the
// created-screen singleton and re-enable pad input.
// ---------------------------------------------------------------------------
void CMenuPassiveSkill::Term() {
    CDeviceVI::waitForDrawDone();

    removeRenderCB__4CScnFP10IScnRender(mScnRef, this);

    BgTex_Release_3D9C(&mBgTex);
    mTitleAHelp.teardown();
    CPassiveSkill_teardown(mPassiveSkill);

    sInstance = nullptr;
    setPresentationFlag__Q22cf13CfGameManagerFv(0);
}

// ---------------------------------------------------------------------------
// Per-frame move (us-80265be8): gated by the game-task idle check and the
// shared mode bitfield (same bit 21 guard as cbRenderBefore), then dispatch
// on the widgets-ready flag (+0x2AC) and tick the three embedded widgets.
// ---------------------------------------------------------------------------
void CMenuPassiveSkill::Move() {
    CTaskGame::getInstance();
    if (CTaskGame::isFlag01Set() != 0) goto exit;
    // Branch-over-branch guard keeps MWCC from folding the bit test into a
    // single conditional branch (see cbRenderBefore).
    if (gmPauseFlags & (1u << 21)) goto exit;
    goto body;
exit:
    return;
body:
    int state = field_2AC;
    if (state == 0) goto case0;
    if (state == 1) goto case1;
    if (state == 2) goto case2;
    if (state == 3) goto case3;
    goto tick;
case0:
    pollPassiveSkillReady(this);
    goto tick;
case1:
    updatePassiveSkillMenu(this);
    goto tick;
case2:
    dispatchPassiveSkillInput(this);
    goto tick;
case3:
    setPassiveSkillReady(this);
tick:

    BgTex_Tick_3D54(&mBgTex);
    mTitleAHelp.updateHelp();
    CPassiveSkill_update((UI_CPassiveSkill*)mPassiveSkill);
}

// noinline: retail keeps the virtual dispatch as a real call (the thunks
// tail-branch to this symbol; an inline would fold the thunks to empty).
__declspec(noinline) void CMenuPassiveSkill::cbRenderBefore() {
    // Draw only while the game task is idle and the realtime-event busy bit
    // (bit 21 of the shared mode word) is clear.
    CTaskGame::getInstance();
    if (CTaskGame::isFlag01Set() != 0) goto exit;
    // Branch-over-branch guard: the exit label + return placed BEFORE `body`
    // keeps MWCC from folding the bit test to a single `bne` -- it emits
    // retail's `beq body; b exit` (see CMenuZeal::cbRenderBefore).
    if (gmPauseFlags & (1u << 21)) goto exit;
    goto body;
exit:
    return;
body:
    if (IsMenuState621F0() == 0) goto exit;

    // Inner block: scope-exit dtor auto-emits the retail direct
    // `bl __dt__Q34nw4r3lyt8DrawInfoFv(drawInfo, -1)`; an explicit
    // `drawInfo.~DrawInfo()` would ALSO emit a virtual-dispatched call.
    {
        GXSetZMode(GX_FALSE, GX_NEVER, GX_FALSE);
        nw4r::lyt::DrawInfo drawInfo;
        SetWideLocAdjust(&drawInfo);
        BgTex_Draw_3D7C(&mBgTex, &drawInfo);
        CPassiveSkill_draw(mPassiveSkill, &drawInfo);
        mTitleAHelp.drawHelp(&drawInfo);
    }
}

CMenuPassiveSkill* CMenuPassiveSkill::sInstance;

// Only one passive skill menu exists at a time.
CMenuPassiveSkill* CMenuPassiveSkill::create(CProcess* parent, CScn* scene) {
    if (sInstance != nullptr) {
        return nullptr;
    }

    CMenuPassiveSkill* menu = new (CWorkThreadSystem::getWorkMem()) CMenuPassiveSkill(scene);
    sInstance = menu;
    menu->Regist(parent, false);
    return sInstance;
}


// ---------------------------------------------------------------------------
// Per-frame readiness poll (us-80265dc4): once the background texture has
// loaded, the title/help widget is ready and the passive-skill layout finished
// loading, arm the help window, mark the screen ready, fire the 0x6d sound op
// and push the cursor state byte from the passive-skill sub-object into the
// title/help widget.
// ---------------------------------------------------------------------------
extern "C" void pollPassiveSkillReady(CMenuPassiveSkill* self) {
    if (BgTex_IsLoaded_3E34(&self->mBgTex) != 0 && self->mTitleAHelp.isInitialized() != 0 &&
        CPassiveSkill_getVisibleIfReady((UI_CPassiveSkill*)self->mPassiveSkill) != 0) {
        self->mTitleAHelp.func_801C412C();
        CPassiveSkill_open((UI_CPassiveSkill*)self->mPassiveSkill);
        self->field_2AC = 1;

        playUISound(0x6d);

        u8 curState = CPassiveSkill_pickOpenState((UI_CPassiveSkill*)self->mPassiveSkill);
        self->mTitleAHelp.func_801C41E8(curState);
    }
}

void stub_us_80265db4() {}

// isIdle declared int (not u8) so the caller compares with cmpi directly
// (no byte-mask rlwinm) - CMenuCollepedia pattern.

// Per-frame update (us-80265e54): once both the title/help widget and the
// passive-skill sub-object report ready, mark the screen state as 2.
extern "C" void updatePassiveSkillMenu(CMenuPassiveSkill* self) {
    if (self->mTitleAHelp.func_801C4124() != 0 && CPassiveSkill_getActiveIfBusy((UI_CPassiveSkill*)self->mPassiveSkill) != 0) {
        self->field_2AC = 2;
    }
}

// ---------------------------------------------------------------------------
// Active-state per-frame input dispatch (us-80265ea4). Two mirror chains
// selected by isClassicController (classic vs Wii controller layout):
// - classic chain: a "special" gate (event byte > 1 and no pending transition
//   on the +0xB8 object) enables the two turbo shortcuts, then the chain
//   tests turbo/held flag words top to bottom.
// - Wii chain: the top held-flag bit (bit 19) resets the +0x2B0 idle timer
//   (playing sound op 2 if it had climbed past lbl_eu_806688F4), re-arms the
//   +0x180 state byte and handles the two turbo masks directly; everything
//   else falls into the lower mirror chain.
// The shared tail pushes the sub-object's state byte into the help widget.
// ---------------------------------------------------------------------------
extern "C" __declspec(noinline) void dispatchPassiveSkillInput(CMenuPassiveSkill* self) {
    if (CPassiveSkill_getActiveIfBusy((UI_CPassiveSkill*)self->mPassiveSkill) == 0) {
        return;
    }

    // Idle timer tick, clamped at lbl_eu_806688F8.
    // Decl order cap,step,cur → f0,f1,f2 (Rule C). sum = cur+step keeps cap
    // live in f0 so the add dest cannot take f0 and should reuse step (f1),
    // giving retail fadds f1,f2,f1.
    f32 cap;
    f32 step;
    f32 cur;
    cur = self->field_2B0;
    step = lbl_eu_806688F4;
    cap = lbl_eu_806688F8;
    f32 sum = cur + step;
    self->field_2B0 = sum;
    if (sum > cap) {
        self->field_2B0 = cap;
    }

    CMenuPassivePadView* pad = getCfPadData__Q22cf13CfGameManagerFv();
    int isClassic = cf::CfGameManager::isClassicController(-1);

    if (isClassic != 0) {
        // -- classic-controller chain --
        int special = 0;
        if (code80135FDC_getByte_64077() > 1 && CPassiveSkill_isSyswinOrLearned((UI_CPassiveSkill*)self->mPassiveSkill) == 0) {
            special = 1;
        }
        UI_CPassiveSkill_setByte180(self->mPassiveSkill, 0);
        u32 turbo = pad->mTurboPressButtonFlags;
        if ((turbo & 0x02000000) != 0 && special != 0) {
            UI_CPassiveSkill_thunk28_67C44(self->mPassiveSkill);
        } else if ((turbo & 0x04000000) != 0 && special != 0) {
            UI_CPassiveSkill_thunk28_67BA0(self->mPassiveSkill);
        } else {
            u32 held = pad->field_4;
            if ((held & 0x00200000) != 0) {
                UI_CPassiveSkill_thunk28_68594(self->mPassiveSkill);
            } else if ((held & 0x00400000) != 0) {
                advancePassiveSkillState(self);
            } else if ((turbo & 0x8004) != 0) {
                UI_CPassiveSkill_thunk28_67CE0(self->mPassiveSkill);
            } else if (((turbo & 0x00010000) | (turbo & 0x00000008)) != 0) {
                UI_CPassiveSkill_thunk28_67E2C(self->mPassiveSkill);
            } else if ((turbo & 0x2001) != 0) {
                UI_CPassiveSkill_thunk28_67F88(self->mPassiveSkill);
            } else if ((turbo & 0x4002) != 0) {
                UI_CPassiveSkill_thunk28_68250(self->mPassiveSkill);
            } else if ((held & 0x00000400) != 0) {
                tryOpenPassiveSkill(self);
            } else if ((held & 0x10000000) != 0) {
                UI_CPassiveSkill_thunk28_68C38(self->mPassiveSkill);
            } else if ((held & 0x00800000) != 0) {
                confirmPassiveSkillMenu(self);
            }
        }
    } else {
        // -- Wii chain --
        UI_CPassiveSkill_setByte180(self->mPassiveSkill, 0);
        u32 first = pad->field_0;
        if ((first & 0x00001000) != 0 && code80135FDC_getByte_64077() > 1 &&
            CPassiveSkill_isSyswinOrLearned((UI_CPassiveSkill*)self->mPassiveSkill) == 0) {
            // Reset the idle timer (sound op 2 when it had run past the F4
            // threshold), re-arm the state byte and handle the turbo masks.
            if (self->field_2B0 > lbl_eu_806688F4) {
                playUISound(2);
            }
            self->field_2B0 = lbl_eu_806688F0;
            UI_CPassiveSkill_setByte180(self->mPassiveSkill, 1);
            u32 turboNow = pad->mTurboPressButtonFlags;
            if ((turboNow & 0x2001) != 0) {
                UI_CPassiveSkill_thunk28_67C44(self->mPassiveSkill);
            } else if ((turboNow & 0x4002) != 0) {
                UI_CPassiveSkill_thunk28_67BA0(self->mPassiveSkill);
            }
        } else {
            u32 held = pad->field_4;
            if ((held & 0x00000010) != 0) {
                UI_CPassiveSkill_thunk28_68594(self->mPassiveSkill);
            } else if ((held & 0x00000020) != 0) {
                advancePassiveSkillState(self);
            } else {
                u32 turbo = pad->mTurboPressButtonFlags;
                if ((turbo & 0x8004) != 0) {
                    UI_CPassiveSkill_thunk28_67CE0(self->mPassiveSkill);
                } else if (((turbo & 0x00010000) | (turbo & 0x00000008)) != 0) {
                    UI_CPassiveSkill_thunk28_67E2C(self->mPassiveSkill);
                } else if ((turbo & 0x2001) != 0) {
                    UI_CPassiveSkill_thunk28_67F88(self->mPassiveSkill);
                } else if ((turbo & 0x4002) != 0) {
                    UI_CPassiveSkill_thunk28_68250(self->mPassiveSkill);
                } else if ((held & 0x00000040) != 0) {
                    tryOpenPassiveSkill(self);
                } else if ((held & 0x00000080) != 0) {
                    UI_CPassiveSkill_thunk28_68C38(self->mPassiveSkill);
                } else if ((held & 0x00000400) != 0) {
                    confirmPassiveSkillMenu(self);
                }
            }
        }
    }

    u8 curState = CPassiveSkill_pickOpenState((UI_CPassiveSkill*)self->mPassiveSkill);
    self->mTitleAHelp.func_801C41E8(curState);
}

// Same shape as updatePassiveSkillMenu but writes the +0x54 state flag instead.
void setPassiveSkillReady(CMenuPassiveSkill* self) {
    if (self->mTitleAHelp.func_801C4124() != 0 && CPassiveSkill_getActiveIfBusy((UI_CPassiveSkill*)self->mPassiveSkill) != 0) {
        self->mField54 = 1;
    }
}

// Per-frame state advance: if the passive-skill sub-object reports a pending
// transition, commit it; otherwise reset the help widget, run the sub-object's
// cancel path and mark the screen state 3.
extern "C" void advancePassiveSkillState(CMenuPassiveSkill* self) {
    if (CPassiveSkill_cellAtLeast8(self->mPassiveSkill) != 0) {
        CPassiveSkill_confirm((UI_CPassiveSkill*)self->mPassiveSkill);
    } else {
        self->mTitleAHelp.beginClose();
        CPassiveSkill_close((UI_CPassiveSkill*)self->mPassiveSkill);
        self->field_2AC = 3;
    }
}

// Open-trigger: only fires once all three progress gates pass (feature
// unlocked, its flag set, collection count reached), then kicks the
// passive-skill sub-object's open thunk.
extern "C" void tryOpenPassiveSkill(CMenuPassiveSkill* self) {
    u32 unlocked = CtrlRemote_TouchBitByArg(0x3372);
    // Retail zero-tests the unlock flag via the cntlzw/srwi boolean idiom;
    // the second gate is an OR - a zeroed 0x3508 flag bypasses the count
    // check entirely.
    if (((u32)__cntlzw(unlocked) >> 5) == 0 &&
        (CtrlRemote_TouchBitByArg(0x3508) == 0 || CtrlRemote_TouchBitByArg(0x20) >= 0x38)) {
        UI_CPassiveSkill_thunk28_68518(self->mPassiveSkill);
    }
}

// Confirm/close handler: if the passive-skill window still has a pending
// transition, leave it alone; otherwise back out to the previous screen,
// stopping the movie player first if one is active.
extern "C" void confirmPassiveSkillMenu(CMenuPassiveSkill* self) {
    if (UI_CPassiveSkill_thunk1b8_CSysWin_getUnk34((UI_CPassiveSkill*)self->mPassiveSkill) == 0) {
        if (CMainMenu_GetInstancePtr() != 0) {
            ArtsInfo_SetReadyFlag();
            playUISound(6);
        }
        self->field_2AC = 4;
        self->mField54 = 1;
    }
}

extern "C" unsigned long hasPassiveSkillMenu(void) { return CMenuPassiveSkill::sInstance != nullptr; }
