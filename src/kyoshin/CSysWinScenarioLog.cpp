// Auto-scaffolded catalog TU for kyoshin/CSysWinScenarioLog

#include "kyoshin/cf/CBattleManagerApi.hpp"
#include "kyoshin/cf/CfMapItemManager.hpp"
#include <types.h>

#include "kyoshin/CSysWinScenarioLog.hpp"
#include "monolib/util/MemManager.hpp"
#include "monolib/work/CWorkThreadSystem.hpp"
#include "monolib/device/CFileHandle.hpp"
#include "monolib/device/CDeviceFont.hpp"
#include "kyoshin/cf/CfGameManager.hpp"
#include "kyoshin/cf/object/CfObject.hpp"
#include "kyoshin/CTaskGame.hpp"
#include "monolib/device/CDeviceFile.hpp"
#include "monolib/work/IWorkEvent.hpp"
#include "monolib/device/CDeviceVI.hpp"

#include <revolution/gx/GXPixel.h>
#include <nw4r/lyt/lyt_layout.h>
#include <nw4r/lyt/lyt_arcResourceAccessor.h>

// True when the pause flag is clear (game not frozen by a subwindow).
static inline bool isSceneActive() {
    return lbl_eu_80664772 == 0;
}

// Bump the scenario counter `id` by one (clamped to 0xFFFF) while the scene is
// active; returns the (possibly bumped) value for later gate checks. Early
// return on the paused path makes MWCC materialize the result copy before the
// compare, giving retail's `lhz / mr / cmpi / beq / b` ladder.
static inline u32 scenarioBump(u32 id) {
    u32 n = getEventValue40__Q22cf13CfGameManagerFv(id);
    if (lbl_eu_80664772 != 0) {
        return n;
    }
    n = n + 1;
    if (n >= 0xFFFF) {
        n = 0xFFFF;
    }
    setEventManagerValue__Q22cf13CfGameManagerFv(id, n);
    return n;
}

static inline void scenarioClose(u32 id) {
    bool booting = getQueuedFileEventCount__Q22cf13CfGameManagerFv() <= 3;
    if (!booting && isSceneActive()) {
        queueEventId__Q22cf13CfGameManagerFv(id);
    }
}

// Bump the scenario counter `id` by one while the scene is active, then close
// the sequence once the counter passes 100 (0x64). Used by func_8027F2DC's
// 1..7 ladder (ids 0x3F..0x45).
static inline void scenarioLadder(u32 id) {
    u32 w = scenarioBump(id);
    if (w >= 0x64) {
        scenarioClose(id);
    }
}

// ---------------------------------------------------------------------------
// func_8027F0A0 (us-80281524)
// Clears the "scenario log open / state" globals to their closed values.
// ---------------------------------------------------------------------------
void func_8027F0A0() {
    lbl_eu_80664910 = 0;
    lbl_eu_80664911 = 0;
    lbl_eu_80664912 = 0;
    lbl_eu_80664914 = 0;
}

// Definitions of these two helpers are placed at the BOTTOM of this file so
// that callers below (func_80280D04 / func_80280DBC / func_80280E9C) only see
// the prototypes from CSysWinScenarioLog.hpp - MWCC then emits a direct `bl`
// to the retail symbol instead of inlining the body at each call site (retail
// calls them out-of-line).

// ---------------------------------------------------------------------------
// CSysWinScenarioLog::~CSysWinScenarioLog (us-80280a38)
// D1-inlined-into-D2 virtual dtor (same shape as CMainMenu). Subobjects are
// destroyed in reverse order: mMemRegion@0x7c, then CProcess base, then
// conditional operator delete. The nested double null-check is the MWCC
// D2-inlined-into-D1 artifact.
// ---------------------------------------------------------------------------
extern "C" void* __dt__18CSysWinScenarioLogFv(CSysWinScenarioLog* _this, int flags) {
    if (_this) {
        __dt__17UnkClass_8045F564Fv((char*)_this + 0x7C, -1);
        if (_this) {
            if (_this) {
                __dt__8CProcessFv(_this, 0);
            }
        }
        if (flags > 0) {
            operator delete(_this);
        }
    }
    return _this;
}

// ---------------------------------------------------------------------------
// CSysWinScenarioLog::CSysWinScenarioLog (us-80280934)
// C-linkage ctor (retail symbol __ct__CSysWinScenarioLog carries no class-length
// mangling, so it stays a plain global function - same idiom as CSystemWindow).
// Runs CProcess's ctor, fills the IUIWindow base region by hand (temp vtable at
// +0x10, the two __ptmf_null callback slots, control words), then the composite
// vtable + work-event/render slots, the embedded UnkClass_8045F564 region, and
// the window fields; finally bumps the open-window bookkeeping counters.
// NOTE: the parameters are used directly (no local copies) - MWCC colors the
// two spilled params in declaration order (this=r29, scene=r30) only when
// neither is aliased into a local.
// ---------------------------------------------------------------------------
extern "C" CSysWinScenarioLog* __ct__CSysWinScenarioLog(CSysWinScenarioLog* s,
                                                        void* param) {
    __ct__8CProcessFv((CProcess*)s);

    ((CSysWinProcessVtable*)s)->mpVtable = (u32)lbl_eu_8052D238;
    // ptmf-triplet copies: walking-pointer reads with the store order [1],[0],[2]
    // reproduce retail's lwzu-fused load schedule (CREvtModelMap func_80180E60
    // pattern).
    u32 w1, w2, w0;
    u32 n1, n2, n0;
    u32* src = __ptmf_null;
    w0 = *src++;
    w1 = *src++;
    s->ptmf0[1] = w1;
    s->ptmf0[0] = w0;
    w2 = *src++;
    s->ptmf0[2] = w2;

    u32* src2 = __ptmf_null;
    n0 = *src2++;
    n1 = *src2++;
    s->ptmf1[1] = n1;
    s->ptmf1[0] = n0;
    n2 = *src2++;
    s->ptmf1[2] = n2;

    s->field_54 = 0;
    s->field_58 = 0;
    s->field_5C = 0;
    s->field_60 = -1;
    s->mFlag64 = 0;
    s->_65[0] = 0;
    s->_65[1] = 0;
    s->mFlag67 = 1;
    s->field_68 = 0;

    ((CSysWinProcessVtable*)s)->mpVtable = (u32)lbl_eu_80538358;
    s->mWorkEvent = (u32)lbl_eu_80538358 + 0x24;
    s->mScnRender = (u32)lbl_eu_80538358 + 0xac;
    s->mFileHandle74 = 0;
    s->mFileHandle78 = 0;

    __ct__17UnkClass_8045F564Fv(&s->mMemRegion);

    s->mScene = (CScn*)param;
    s->mState = 0;
    s->mField94 = 0;
    s->mpAccessor = 0;
    s->mpLayout = 0;
    s->mpAnim = 0;

    setPresentationFlag__Q22cf13CfGameManagerFv(true);
    code80135FDC_postIncByte_64080();
    return s;
}

// ---------------------------------------------------------------------------
// func_8027E9E8 (us-80280e6c)
// Allocate the CSysWinScenarioLog singleton from work memory, construct it,
// store it, and register it as a child of `parent`. Returns null when already
// created. (Regist is called even if the allocation failed, matching retail.)
// ---------------------------------------------------------------------------
CSysWinScenarioLog* func_8027E9E8(CProcess* parent, u8* param) {
    if (lbl_eu_80664908 != 0) {
        return nullptr;
    }
    CSysWinScenarioLog* obj =
        (CSysWinScenarioLog*)mtl::MemManager::allocate(0xA4, CWorkThreadSystem::getWorkMem());
    if (obj != nullptr) {
        // The ctor returns `this` in r3, so obj stays live in r3 across the
        // call and no extra callee-saved spill is needed (matches retail).
        obj = (CSysWinScenarioLog*)__ct__CSysWinScenarioLog(obj, param);
    }
    lbl_eu_80664908 = (u32)obj;
    obj->Regist(parent, false);
    return (CSysWinScenarioLog*)lbl_eu_80664908;
}

// ---------------------------------------------------------------------------
// func_8027F0B8 (us-8028153c)
// When the battle list holds members and the scene is ready, force the
// scenario-sequence close for a fixed id, then mark the window as shown.
// ---------------------------------------------------------------------------
struct CBattleListNode {
    struct CBattleListNode* next;   // 0x0
};

struct CBattleListOwner {
    u8 _0[0x28];
    CBattleListNode* list;          // 0x28
};

void func_8027F0B8() {
    CBattleListNode* p;
    s32 count;
    CBattleListNode* head =
        ((CBattleListOwner*)getInstance__Q22cf14CBattleManagerFv())->list;
    count = 0;
    p = head->next;
    while (p != head) {
        p = p->next;
        ++count;
    }
    if (count > 0) {
        if (lbl_eu_80664912 != 0) {
            bool booting = getQueuedFileEventCount__Q22cf13CfGameManagerFv() <= 3;
            if (!booting && isSceneActive()) {
                queueEventId__Q22cf13CfGameManagerFv(0x3A);
            }
        }
        lbl_eu_80664910 = 1;
    }
}

// ---------------------------------------------------------------------------
// func_80280804 (us-80282c88)
// Object that owns a state-flags dword at +0x3F00.
// ---------------------------------------------------------------------------
struct CScenarioFlagObj {
    u8 _0[0x3F00];
    u32 field_0x3F00;   // flags
};

// Bump sequence counter 0xB once (while not paused), then close it once the
// counter passes 100. Written as a two-arm if/else so MWCC emits retail's
// `beq bump; b after` guard pair; the keep-arm copy is the phi resolution.
void func_80280804(CScenarioFlagObj* self) {
    if ((self->field_0x3F00 & 0x2) != 0) {
        u32 n;
        u32 cur = getEventValue40__Q22cf13CfGameManagerFv(0xB);
        u16 pauseFlag = lbl_eu_80664772;
        if (pauseFlag != 0) {
            // Scene frozen by a subwindow: keep the un-bumped value.
            n = cur;
        } else {
            n = cur + 1;
            if (n >= 0xFFFF) {
                n = 0xFFFF;
            }
            setEventManagerValue__Q22cf13CfGameManagerFv(0xB, n);
        }
        if (n >= 0x64) {
            bool booting = getQueuedFileEventCount__Q22cf13CfGameManagerFv() <= 3;
            if (!booting && lbl_eu_80664772 == 0) {
                queueEventId__Q22cf13CfGameManagerFv(0xB);
            }
        }
    }
}

// ---------------------------------------------------------------------------
// Remaining unmatched func_8027* stubs in this unit (non-target).
// ---------------------------------------------------------------------------
void func_8027EE70(void* self) { ((void(*)(void*))__dt__18CSysWinScenarioLogFv)((char*)self - 0x6c); }
void func_8027EE78(void* self) { ((CSysWinScenarioLog*)((char*)self - 0x70))->cbRenderBefore(); }
void func_8027EE80(void* self) { ((void(*)(void*))__dt__18CSysWinScenarioLogFv)((char*)self - 0x70); }

void func_8027EF50() {
    if (lbl_eu_80664910 != 0) {
        // Walk the battle-manager ring until we're back at the sentinel and
        // count how many active battle entries there are. (Declaration order
        // mirrors the matched func_8027F0B8 loop so MWCC colors the node r3 /
        // sentinel r5 like retail.)
        CSysWinListNode* node;
        s32 count;
        CSysWinListNode* sentinel =
            ((CSysWinBattleMgrView*)getInstance__Q22cf14CBattleManagerFv())->mListSentinel;
        count = 0;
        node = sentinel->next;
        while (node != sentinel) {
            node = node->next;
            ++count;
        }
        if (count <= 0) {
            if (lbl_eu_80664911 == 0) {
                bool booting = getQueuedFileEventCount__Q22cf13CfGameManagerFv() <= 3;
                if (!booting && isSceneActive()) {
                    queueEventId__Q22cf13CfGameManagerFv(0xC);
                }
            }
            // Player voice/action range check: both sides call the same virtual.
            bool inRange;
            cf::CfObject* obj = (cf::CfObject*)func_8016FE34(cf::CfGameManager::getPlayer(0));
            if (obj != 0) {
                inRange = false;
                if (lbl_eu_80668AE0 < obj->CfObject_UnkVirtualFunc56() &&
                    obj->CfObject_UnkVirtualFunc56() < lbl_eu_80668AE4) {
                    inRange = true;
                }
            } else {
                inRange = false;
            }
            if (inRange) {
                bool booting = getQueuedFileEventCount__Q22cf13CfGameManagerFv() <= 3;
                if (!booting && isSceneActive()) {
                    queueEventId__Q22cf13CfGameManagerFv(0x3B);
                }
            }
        }
    }
    lbl_eu_80664910 = 0;
    lbl_eu_80664912 = 0;
}
// ---------------------------------------------------------------------------
// CSysWinScenarioLog::cbRenderBefore (us-80280dc0)
// Draw the scenario-log layout behind the scene while the game is live: same
// gate chain as Move() but testing bit 10 of the pause word, then Z-off and
// a layout draw with a stack DrawInfo (raw buffer, direct ctor/dtor calls).
// noinline keeps the this-adjusting vtable thunk func_8027EE78 a two-instruction
// subi + tail-call instead of a full inline copy.
// ---------------------------------------------------------------------------
__declspec(noinline) void CSysWinScenarioLog::cbRenderBefore() {
    // OR-combined guards with early return (MWCC_CASES control_flow pattern):
    // the first disjunct folds to a direct branch to the shared epilogue and
    // the second becomes the branch-over-branch gate (beq forward + b end).
    if (CTaskGame::getInstance()->isFlag01Set() != 0 ||
        (lbl_eu_80663E28 & 0x200000) != 0) {
        return;
    }
    // Separate single-condition early returns fold to direct branches
    // (an || chain would put a spurious branch-over-branch on the last
    // disjunct).
    if (func_8013BE50() == 0) {
        return;
    }
    if (func_8029A658() != 0) {
        return;
    }
    if (mpLayout == 0) {
        return;
    }
    GXSetZMode(GX_FALSE, GX_NEVER, GX_FALSE);
    // Raw-storage DrawInfo built/destroyed via the pre-mangled ct/dt calls so
    // the body keeps retail's direct bl sequence.
    u8 drawInfo[0x60];
    __ct__Q34nw4r3lyt8DrawInfoFv(drawInfo);
    func_80137250(reinterpret_cast<nw4r::lyt::DrawInfo*>(drawInfo));
    func_80137038(mpLayout,
                  reinterpret_cast<nw4r::lyt::DrawInfo*>(drawInfo), 0, 1);
    __dt__Q34nw4r3lyt8DrawInfoFv(
        reinterpret_cast<nw4r::lyt::DrawInfo*>(drawInfo), -1);
}

// ---------------------------------------------------------------------------
// CSysWinScenarioLog::Move (us-80280c54)
// Advance the scenario-log window's opening/closing state machine.
// Gate layout mirrors retail: the first guard folds to a direct branch to the
// shared epilogue, the pause-bit guard renders as the branch-over-branch
// (beq forward + b end, cbRenderBefore pattern), and the remaining guards are
// separate single-condition early returns (direct conditional branches).
// ---------------------------------------------------------------------------
void CSysWinScenarioLog::Move() {
    if (CTaskGame::getInstance()->isFlag01Set() != 0 ||
        (lbl_eu_80663E28 & 0x200000) != 0) {
        return;
    }
    if (func_8013BE50() == 0) {
        return;
    }
    if (func_8029A658() != 0) {
        return;
    }
    if (mpLayout == 0) {
        return;
    }
    {
        switch (mState) {
        case 0:
            func_8027EA6C(this);
            break;
        case 1:
            if (func_80137444__FPQ34nw4r3lyt13AnimTransformf(mpAnim, lbl_eu_80668AD0) != 0) {
                mState = 2;
            }
            break;
        case 2: {
            CSysWinPadView* pad = (CSysWinPadView*)cf::CfGameManager::getCurrentPad();
            bool pressed;
            if (isClassicController__Q22cf13CfGameManagerFv(-1) != 0) {
                pressed = (pad->mButtons & 0x4600000) != 0;
            } else {
                pressed = (pad->mButtons & 0x1030) != 0;
            }
            if (pressed) {
                mState = 3;
                func_80138078__FUl(6);
                func_80138078__FUl(0xE);
            }
            break;
        }
        case 3:
            if (func_80137510(mpAnim, lbl_eu_80668AD0) != 0) {
                mFlag67 = 0;
                mFlag64 = 1;
            }
            break;
        }
        // Layout virtual update (vtable slot 0x38) with a null argument.
        ((CSysWinLayoutHook*)mpLayout)->mAt38(0);
    }
}

// ---------------------------------------------------------------------------
// func_8027EA6C (us-80280ef0)
// Open the scenario-log window content (Move state 0): guard on the layout
// being loaded and the BDAT file pointer being set, mark the window busy,
// pick the first message row whose id reaches the message-count threshold,
// stamp the four text fields, resolve the texture for the current game mode
// and size the picture pane from its dims.
// ---------------------------------------------------------------------------
extern "C" void func_8027EA6C(CSysWinScenarioLog* self) {
    if (self->mpLayout == 0) {
        return;
    }
    if (lbl_eu_8066490C == 0) {
        return;
    }
    self->mState = 1;
    func_80138078__FUl(0xd);
    // (u32) cast pins the extern "C" decl (pluginUi.hpp's C++ int overload
    // would otherwise mangle the reloc to func_8009CF8C__Fi).
    u16 maxId = (u16)func_8009CF8C((u32)0x20);
    int chosen = 1;
    u16 count = (u16)func_8003B1EC((void*)lbl_eu_8066490C);
    int i = 1;
    while ((u16)i <= count) {
        if ((u16)func_80136254((void*)lbl_eu_8066490C, &lbl_eu_8050EE24[0x36],
                               (u16)i) >= maxId) {
            chosen = i;
            break;
        }
        i++;
    }
    char* s = func_80136190(&lbl_eu_8050EE24[0x3b], &lbl_eu_8050EE24[0x44], 0x19);
    func_80136B4C(self->mpLayout, &lbl_eu_8050EE24[0x49], s, 0);
    char* t = func_8013639C((void*)lbl_eu_8066490C, &lbl_eu_8050EE24[0x56],
                            (u16)chosen);
    func_80136B4C(self->mpLayout, &lbl_eu_8050EE24[0x5b], t, (u32)self->mField94);
    char* u = func_80136190(&lbl_eu_8050EE24[0x68], &lbl_eu_8050EE24[0x56], 0x2c);
    func_80136B4C(self->mpLayout, &lbl_eu_8050EE24[0x76], u, 0);
    const char* sel = isClassicController__Q22cf13CfGameManagerFv(-1) != 0
                          ? &lbl_eu_8050EE24[0x82]
                          : &lbl_eu_8050EE24[0x8b];
    u16 msgId = (u16)func_8013606C(&lbl_eu_8050EE24[0x68], sel, 0x2c);
    void* tex = func_80138F78(msgId);
    nw4r::lyt::ArcResourceAccessor* mgr = func_801355F4();
    void* mat = mgr->GetResource(0x74696d67, (const char*)tex, 0);
    if (mat == 0) {
        return;
    }
    func_80137E7C(self->mpLayout, &lbl_eu_8050EE24[0x94], mat);
    CSysWinTexMeta* meta = (CSysWinTexMeta*)mat;
    u16 w = meta->field_0x8->field_0x2;
    u16 h = meta->field_0x8->field_0x0;
    void* pane =
        ((CSysWinPaneView*)self->mpLayout->GetRootPane())->mSetText(
            &lbl_eu_8050EE24[0x94], 1);
    if (pane == 0) {
        return;
    }
    CSysWinF64Conv convW;
    CSysWinF64Conv convH;
    CSysWinPaneView* pv = (CSysWinPaneView*)pane;
    convW.w[1] = w;
    convW.w[0] = 0x43300000;
    pv->field_0x4C = (f32)(convW.d - lbl_eu_80668AD8);
    convH.w[1] = h;
    convH.w[0] = 0x43300000;
    pv->field_0x50 = (f32)(convH.d - lbl_eu_80668AD8);
}

// ---------------------------------------------------------------------------
// func_8027F148 (us-802815cc)
// Scenario-log open gate: every actor in the manager list must report its
// device usable (vtable 0x2bc) and pass the arts-state gate
// (func_80174C98 with the +0x4 sub-object's slot-0x30 state, mode 0x1d);
// when all pass, bump/close the 0x3c counter (100-threshold close) and the
// 0x57 counter (50-threshold close).
// ---------------------------------------------------------------------------
void func_8027F148() {
    int result;
    u32 n;
    CSysWinActorList* list = getListB28();
    // Declaration order drives MWCC callee-saved coloring (retail colors the
    // walk pointer r29 and the adjusted device base r30).
    u8* base;
    CSysWinActorListNode* node = list->sentinel->next;
    while (node != list->sentinel) {
        // First usable-gate call takes the base pointer as a pure argument
        // expression (retail keeps it in the caller-temp r3, not a
        // callee-saved); only the arts-state half below materializes it.
        if (((CSysWinDevView*)(node->object != 0 ? node->object - 0x3E9C
                                                 : node->object))
                ->mAt2BC() == 0) {
            result = 0;
            goto done;
        }
        base = node->object;
        if (base != 0) {
            base -= 0x3E9C;
        }
        u32 x = ((CSysWinSubObjView*)((CSysWinOwnerSubView*)base)->field_0x4)
                    ->mAt30()->field_0;
        if (func_80174C98(base, &x, 0x1d) != 0) {
            result = 0;
            goto done;
        }
        node = node->next;
    }
    // All actors passed the gates: result = 1 on the loop fall-through only
    // (the failure paths jump past this, matching retail's volatile r0).
    result = 1;
done:
    if (result != 0) {
        // Inline bump (scenarioBump shape hoists the keep-copy wrong here):
        // the two-arm if/else makes MWCC hoist the phi copy above the branch,
        // matching retail's lhz/mr/cmpi schedule.
        // Two-arm keep-first bump (best known shape; MWCC sinks the phi
        // copy after the branch - same residual 3 as siblings 80280804/
        // 802809C8/80280BF0, scheduling-swap class per MWCC_CASES).
        u32 cur = getEventValue40__Q22cf13CfGameManagerFv(0x3c);
        if (lbl_eu_80664772 != 0) {
            // Scene frozen by a subwindow: keep the un-bumped value.
            n = cur;
        } else {
            n = cur + 1;
            if (n >= 0xFFFF) {
                n = 0xFFFF;
            }
            setEventManagerValue__Q22cf13CfGameManagerFv(0x3c, n);
        }
        if (n >= 1) {
            scenarioClose(0x3c);
        }
        if (n >= 0x32) {
            scenarioClose(0x57);
        }
    }
}

// ---------------------------------------------------------------------------
// func_8027F2DC (us-80281760)
// Advance scenario-log counters based on the owner's sub-state. Guards against
// two condition shapes (base==1 with sub value 7, base==5 with sub value in
// [7,14]) that force an early return, bumps/closes the 0x3d/0x3e counters,
// then, when the embedded player matches, runs a 1..7 ladder (0x3F..0x45) and
// marks the shown flag.
// ---------------------------------------------------------------------------
void func_8027F2DC(CScenarioLogOwner* self) {
    if ((self->field_0x3F00 & 0x2) == 0) {
        return;
    }
    // Guard 1: base==1 with the sub-state value exactly 7 forces an early
    // return. The test is computed into a result (retail's cntlzw boolean),
    // with the else-branches written first so MWCC keeps retail's layout.
    {
        u16 h = self->field_0x3F28;
        int base = (h == 0x8) ? 3 : h;
        int result;
        if (base != 1) {
            result = 0;
        } else {
            CScenarioLogSub* p = (CScenarioLogSub*)self->field_0x3F60;
            if (p == 0) {
                result = 0;
            } else {
                result = (p->field_0x8->field_0x18 == 7);
            }
        }
        if (result != 0) {
            return;
        }
    }
    // Guard 2: base==5 with the sub-state value in [7,14] forces an early
    // return.
    {
        u16 h = self->field_0x3F28;
        int base = (h == 0x8) ? 3 : h;
        int result;
        if (base != 5) {
            result = 0;
        } else {
            CScenarioLogSub* p = (CScenarioLogSub*)self->field_0x3F60;
            if (p == 0) {
                result = 0;
            } else {
                // Switch lowering keeps both compares intact (retail emits
                // cmpi 7 / blt + cmpi 0xe / ble without range fusion).
                s32 val = (s32)p->field_0x8->field_0x18;
                switch (val) {
                case 7:
                case 8:
                case 9:
                case 10:
                case 11:
                case 12:
                case 13:
                case 14:
                    result = 1;
                    break;
                default:
                    result = 0;
                    break;
                }
            }
        }
        if (result != 0) {
            return;
        }
    }
    // Bump 0x3d then close the 0x3d / 0x3e gates past their thresholds.
    u32 v = scenarioBump(0x3d);
    if (v >= 0x3E8) {
        scenarioClose(0x3d);
    }
    if (v >= 0x2710) {
        scenarioClose(0x3e);
    }
    // Player check: continue with the ladder only for the first player slot or
    // when force-flagged.
    const u8* p0 = (const u8*)self;
    if (self != 0) {
        p0 = (const u8*)self + 0x3E9C;
    }
    if (p0 == (const u8*)cf::CfGameManager::getPlayer(0) ||
        (self->field_0x3E6C & 0x1000)) {
        u16 h3 = self->field_0x3F28;
        int base3 = (h3 == 0x8) ? 3 : h3;
        if (base3 == 1) {
            if (scenarioBump(0x3F) >= 0x64) {
                scenarioClose(0x3F);
            }
        }
        if (base3 == 2) {
            if (scenarioBump(0x40) >= 0x64) {
                scenarioClose(0x40);
            }
        }
        if (base3 == 3) {
            if (scenarioBump(0x41) >= 0x64) {
                scenarioClose(0x41);
            }
        }
        if (base3 == 4) {
            if (scenarioBump(0x42) >= 0x64) {
                scenarioClose(0x42);
            }
        }
        if (base3 == 5) {
            if (scenarioBump(0x43) >= 0x64) {
                scenarioClose(0x43);
            }
        }
        if (base3 == 6) {
            if (scenarioBump(0x44) >= 0x64) {
                scenarioClose(0x44);
            }
        }
        if (base3 == 7) {
            if (scenarioBump(0x45) >= 0x64) {
                scenarioClose(0x45);
            }
        }
        lbl_eu_80664911 = 1;
    }
}

// ---------------------------------------------------------------------------
// func_8027F848 (us-80281ccc)
// Scenario-log sequence progress. Guarded by the owner flag word bit 1. Emits
// one-shot sequence-close calls at several time/event gates, bumps and closes
// the 0x26/0x38 counters, then marks the global flags and increments the
// open-window counter.
// ---------------------------------------------------------------------------
void func_8027F848(CScenarioLogOwner* arg0, s32 arg1, CScenarioLogArg2* arg2) {
    if (arg0->field_0x3F00 & 0x2) {
        if (arg1 >= 0xBB8) {
            scenarioClose(0x1);
        }
        if (arg1 >= 0x61A8) {
            scenarioClose(0x2);
        }
        if (arg1 >= 0x10000 - 0x3CB0) {
            scenarioClose(0x3);
        }
        if (arg1 == 0x29A) {
            scenarioClose(0x5F);
        }
        if (arg1 == 0x309) {
            scenarioClose(0x60);
        }
        if (arg2->field_0x74 & 0x100) {
            u32 v26 = scenarioBump(0x26);
            if (v26 >= 0x32) {
                scenarioClose(0x26);
            }
            if (v26 >= 0x5DC) {
                scenarioClose(0x27);
            }
            if (v26 >= 0x1388) {
                scenarioClose(0x28);
            }
        }

        // Player check: enter the 0x38 ladder when the embedded player matches
        // the first player slot, or the forced-player flag is set.
        const u8* p0 = (const u8*)arg0;
        if (arg0 != 0) {
            p0 = (const u8*)arg0 + 0x3E9C;
        }
        if (p0 == (const u8*)cf::CfGameManager::getPlayer(0) ||
            (arg0->field_0x3E6C & 0x1000)) {
            if (arg2->field_0x78 & 0x2) {
                u32 v38 = scenarioBump(0x38);
                if (v38 >= 1) {
                    scenarioClose(0x38);
                }
                if (v38 >= 0x32) {
                    scenarioClose(0x39);
                }
                if ((s32)lbl_eu_80664914 <= 0) {
                    lbl_eu_80664912 = 1;
                }
            }
        }

        // Second player check: advance arg0 to the embedded player marker in
        // place (retail's addi writes r29), then gate the shown flag.
        if (arg0 != 0) {
            arg0 = (CScenarioLogOwner*)((u8*)arg0 + 0x3E9C);
        }
        if ((const u8*)arg0 == (const u8*)cf::CfGameManager::getPlayer(0)) {
            if (arg1 > 0) {
                lbl_eu_80664911 = 1;
            }
        }
        lbl_eu_80664914 += 1;
    }
}

// ---------------------------------------------------------------------------
// func_8027FC04 (us-80282088)
// When both the owner (bit 2) and the peer window (bit 1) gates are set,
// resolve both device objects via their vtable-0x28c virtual and drive the
// scenario-log counters for the pair.
// ---------------------------------------------------------------------------
// func_8027FC80 has an unmangled retail name, so it is defined with C linkage
// (same convention as the func_8027EE88/func_8027EEF4 helpers) to keep the
// call-site relocs named func_8027FC80.
extern "C" void func_8027FC80(CSysWinDevice* arg0, void* arg1);

void func_8027FC04(CScenarioLogOwner* self, CScenarioLogOwner* other) {
    if ((self->field_0x3F00 & 4) != 0 && (other->field_0x3F00 & 2) != 0) {
        CSysWinDevice* devOther = csysWinCall28C(other);
        func_8027FC80(csysWinCall28C(self), devOther);
    }
}

// ---------------------------------------------------------------------------
// func_8027FC80 (us-80282104)
// Drive the scenario-log counters for a given device/window pair: bump 0x4
// with its 50/200/1000/5000 gate ladder, then, unless the game mode is 0x16,
// run state-dependent ladders: window state 1/2 bumps 0xd (and sets a guard
// that suppresses the remaining flag-driven ladders), the clear-flag case
// bumps 0x10, the 0x100-flag case bumps 0x13, fixed window states 6/7/9 bump
// 0x16/0x18/0x1a, and the 0x8000-flag case bumps 0x1c. Finally, when arg1's
// embedded player matches the first slot, compare progress values (vtable
// 0x224) and close 0x5d/0x5e once arg1 trails arg0 by 5 / 10.
// ---------------------------------------------------------------------------
extern "C" void func_8027FC80(CSysWinDevice* arg0, void* arg1) {
    u32 dev = csysWinCallE0(arg0);
    u32 guard = 0;

    u32 v4 = scenarioBump(0x4);
    if (v4 >= 0x32) {
        scenarioClose(0x4);
    }
    if (v4 >= 0xC8) {
        scenarioClose(0x5);
    }
    if (v4 >= 0x3E8) {
        scenarioClose(0x6);
    }
    if (v4 >= 0x1388) {
        scenarioClose(0x7);
    }

    if (lbl_eu_80663E42 != 0x16) {
        if ((u32)(dev - 1) <= 1) {
            u32 vd = scenarioBump(0xD);
            if (vd >= 0x1E) {
                scenarioClose(0xD);
            }
            if (vd >= 0x64) {
                scenarioClose(0xE);
            }
            if (vd >= 0xFA) {
                scenarioClose(0xF);
            }
            guard = 1;
        }
        if (guard == 0) {
            // Bool-materialized gate (retail emits li r3,0 / two bit tests /
            // li r3,1 / cmpi): enter the ladder only when neither flag is set.
            u32 flags = arg0->field_0x3374;
            int ok = 0;
            if ((flags & 0x100) == 0) {
                if ((flags & 0x10000) == 0) {
                    ok = 1;
                }
            }
            if (ok != 0) {
                u32 v10 = scenarioBump(0x10);
                if (v10 >= 0x1E) {
                    scenarioClose(0x10);
                }
                if (v10 >= 0x64) {
                    scenarioClose(0x11);
                }
                if (v10 >= 0xFA) {
                    scenarioClose(0x12);
                }
            }
        }
        if (guard == 0) {
            if (arg0->field_0x3374 & 0x100) {
                u32 v13 = scenarioBump(0x13);
                if (v13 >= 0x1E) {
                    scenarioClose(0x13);
                }
                if (v13 >= 0x64) {
                    scenarioClose(0x14);
                }
                if (v13 >= 0xFA) {
                    scenarioClose(0x15);
                }
            }
        }
        if ((s32)dev == 6) {
            u32 v16 = scenarioBump(0x16);
            if (v16 >= 0x1E) {
                scenarioClose(0x16);
            }
            if (v16 >= 0x64) {
                scenarioClose(0x17);
            }
        }
        if ((s32)dev == 7) {
            u32 v18 = scenarioBump(0x18);
            if (v18 >= 0x1E) {
                scenarioClose(0x18);
            }
            if (v18 >= 0x64) {
                scenarioClose(0x19);
            }
        }
        if ((s32)dev == 9) {
            u32 v1a = scenarioBump(0x1A);
            if (v1a >= 0x1E) {
                scenarioClose(0x1A);
            }
            if (v1a >= 0x64) {
                scenarioClose(0x1B);
            }
        }
        if (guard == 0) {
            if (arg0->field_0x3374 & 0x10000) {
                u32 v1c = scenarioBump(0x1C);
                if (v1c >= 0x1E) {
                    scenarioClose(0x1C);
                }
            }
        }
    }

    if (arg1 != 0) {
        CScenarioLogOwner* p = csysWinCall9C(arg1);
        const u8* pp = (const u8*)p;
        if (p != 0) {
            pp += 0x3E9C;
        }
        if (pp == (const u8*)cf::CfGameManager::getPlayer(0)) {
            u32 a0 = csysWinCall224(arg0)->field_0;
            u32 b0 = csysWinCall224(arg1)->field_0;
            if (b0 + 5 <= a0) {
                scenarioClose(0x5D);
            }
            a0 = csysWinCall224(arg0)->field_0;
            b0 = csysWinCall224(arg1)->field_0;
            if (b0 + 10 <= a0) {
                scenarioClose(0x5E);
            }
        }
    }
}
// ---------------------------------------------------------------------------
// func_802804F8 (us-8028297c)
// Device-side one-shot latch: when the device's owner has scenario flag bit 2
// live, store the device's usable result (vtable 0x2bc) into the one-shot
// byte lbl_eu_80664918.
// ---------------------------------------------------------------------------
void func_802804F8(CSysWinDevice* self) {
    int flag = 0;
    if (((CSysWinDevView*)self)->mAt9C() != 0 &&
        ((((CSysWinDevView*)self)->mAt9C())->field_0x3F00 & 4)) {
        flag = 1;
    }
    if (flag != 0) {
        lbl_eu_80664918 = (u8)((CSysWinDevView*)self)->mAt2BC();
    }
}

// ---------------------------------------------------------------------------
// func_80280588 (us-80282a0c)
// Device-side scenario-log gate: when the device's owner is set and its flag
// word bit 1 is live, and the one-shot byte is still clear, ask the device
// (vtable 0x2bc) whether it is usable; if so, drive the pair's counters via
// func_8027FC80 and latch the one-shot byte.
// ---------------------------------------------------------------------------
void func_80280588(CSysWinDevice* self) {
    bool flag = false;
    if (((CSysWinDevView*)self)->mAt9C() != 0 &&
        ((((CSysWinDevView*)self)->mAt9C())->field_0x3F00 & 0x4)) {
        flag = true;
    }
    // Branch-over-branch (MWCC_CASES uusb_ppc pattern): exit label BEFORE
    // body label plus explicit returns keeps retail's `bne walk; b done`.
    if (flag == false) {
        goto gate_done;
    }
    if (lbl_eu_80664918 != 0) {
        goto gate_done;
    }
    if (((CSysWinDevView*)self)->mAt2BC() != 0) {
        goto walk;
    }
    goto gate_done;
gate_done:
    return;
walk:
    func_8027FC80(self, 0);
    lbl_eu_80664918 = 1;
}

// ---------------------------------------------------------------------------
// func_80280640 (us-80282ac4)
// Device-side scenario-log gate (variant of func_80280588): when the device's
// owner is set with flag bit 2 live and the one-shot byte is clear, ask the
// device (vtable 0x2bc) whether it is usable; if so, scan the actor list for
// the first owner that passes a state gate (sub-state 1: flag bits 14/15;
// sub-state 4: the character category is 1) and drive the pair's counters
// with that owner, then latch the one-shot byte.
// NOTE: all three guards are one || chain so MWCC keeps retail's layout:
// the flag recheck survives mid-chain, and the final usable test renders as
// the branch-over-branch (bne body; b epilogue).
// RESIDUAL (10 pure reg-swaps, Chaitin ceiling): retail colors the walk as
// node=r29 / owner+found=r30; every source lever tried (declaration order,
// block scoping, early dead inits, inline helpers, for/while forms) leaves
// node=r30 / owner=r29. Same class as menu-bps-move party-fill permutation.
// ---------------------------------------------------------------------------

void func_80280640(CSysWinDevice* self) {
    int flag = 0;
    if (((CSysWinDevView*)self)->mAt9C() != 0 &&
        ((((CSysWinDevView*)self)->mAt9C())->field_0x3F00 & 4)) {
        flag = 1;
    }
    // One || chain for all three guards: the flag recheck stays (mid-chain
    // disjuncts can't be folded past the lbl byte read) and the final test
    // renders as retail's branch-over-branch (bne body; b epilogue).
    if (flag == 0 || lbl_eu_80664918 != 0 ||
        ((CSysWinDevView*)self)->mAt2BC() == 0) {
        return;
    }
    CSysWinActorList* list = getListB28();
    CSysWinActorListNode* node = list->sentinel->next;
    CScenarioLogOwner* found;
    while (node != list->sentinel) {
        CScenarioLogOwner* owner =
            (CScenarioLogOwner*)(node->object != 0 ? node->object - 0x3E9C
                                                   : node->object);
        int result;
        u16 h = owner->field_0x3F28;
        if (h == 1) {
            u32 flags = owner->field_0x3374;
            result = 1;
            if ((flags & 0x4000) == 0) {
                if ((flags & 0x8000) == 0) {
                    result = 0;
                }
            }
        } else if (h == 4) {
            func_8009EC9C(4);
            result = (func_800A32BC() == 1);
        } else {
            result = 0;
        }
        if (result != 0) {
            found = owner;
            goto emit;
        }
        node = node->next;
    }
    // Loop fell through with no matching owner: pass null. (The
    // found-owner path jumps past this default, matching retail.)
    found = 0;
emit:
    func_8027FC80(self, found);
    lbl_eu_80664918 = 1;
}
// ---------------------------------------------------------------------------
// func_802807A0 (us-80282c24)
// When the owner's scenario gate (flag bit 2) is set and the owner is a
// member of the battle manager's list, resolve the owner's device object via
// its vtable and drive the scenario-log counters for that device.
// ---------------------------------------------------------------------------
void func_802807A0(CScenarioLogOwner* self) {
    if ((self->field_0x3F00 & 4) != 0) {
        if (func_800DA06C(getInstance__Q22cf14CBattleManagerFv(), self) != 0) {
            func_8027FC80(csysWinCall28C(self), 0);
        }
    }
}
void func_802808AC(s32 self) {
    if (self < 4) {
        return;
    }
    u32 seq = getEventValue40__Q22cf13CfGameManagerFv(0x2C);
    // Two-arm phi merge reproduces retail's `beq bump; b after` pair.
    // RESIDUAL (3 structural): MWCC sinks the keep-path copy (`mr r31,r3`)
    // after the cmpi/beq; retail hoists it between the lhz and cmpi.
    // Ruled out: goto dispatch (direct + negated), empty-then/else,
    // do-while break, switch equality chain, ternary+comma merge (adds a
    // temp), pre-assigned keep value (+duplicated arm), flag-read-first
    // order (hoists lhz above the bl into r31), arms swapped. Same
    // scheduling-swap class as rfc_FlowReq (MWCC_CASES ~line 703).
    {
        u16 pauseFlag = lbl_eu_80664772;
        u32 n;
        if (pauseFlag != 0) {
            // Scene frozen by a subwindow: keep the un-bumped value.
            n = seq;
        } else {
            n = seq + 1;
            if (n >= 0xFFFF) {
                n = 0xFFFF;
            }
            setEventManagerValue__Q22cf13CfGameManagerFv(0x2C, n);
        }
        seq = n;
    }
after:
    if (seq >= 0x1) {
        bool booting = getQueuedFileEventCount__Q22cf13CfGameManagerFv() <= 3;
        if (!booting && isSceneActive()) {
            queueEventId__Q22cf13CfGameManagerFv(0x2C);
        }
    }
    if (seq >= 0x32) {
        bool booting = getQueuedFileEventCount__Q22cf13CfGameManagerFv() <= 3;
        if (!booting && isSceneActive()) {
            queueEventId__Q22cf13CfGameManagerFv(0x2D);
        }
    }
    if (seq >= 0xC8) {
        bool booting = getQueuedFileEventCount__Q22cf13CfGameManagerFv() <= 3;
        if (!booting && isSceneActive()) {
            queueEventId__Q22cf13CfGameManagerFv(0x2E);
        }
    }
}

// ---------------------------------------------------------------------------
// func_802809C8 (us-80282e4c)
// Bump/close the scenario-log sequence counters at three gates (10 / 100 /
// 500). Retails only clamps once per open window and only closes each gate
// when the scene is not booting and no subwindow is freezing the game.
// ---------------------------------------------------------------------------
void func_802809C8() {
    u32 n;
    u32 cur = getEventValue40__Q22cf13CfGameManagerFv(0x23);
    // Two-arm if/else assigning n on both sides reproduces retail's
    // `beq bump; b after` guard pair; the keep-arm copy is the phi resolution.
    if (lbl_eu_80664772 != 0) {
        // Scene frozen by a subwindow: keep the un-bumped value.
        n = cur;
    } else {
        n = cur + 1;
        if (n >= 0xFFFF) {
            n = 0xFFFF;
        }
        setEventManagerValue__Q22cf13CfGameManagerFv(0x23, n);
    }
after:
    ;
    if (n >= 0xA) {
        bool booting = getQueuedFileEventCount__Q22cf13CfGameManagerFv() <= 3;
        if (!booting && isSceneActive()) {
            queueEventId__Q22cf13CfGameManagerFv(0x23);
        }
    }
    if (n >= 0x64) {
        bool booting = getQueuedFileEventCount__Q22cf13CfGameManagerFv() <= 3;
        if (!booting && isSceneActive()) {
            queueEventId__Q22cf13CfGameManagerFv(0x24);
        }
    }
    if (n >= 0x1F4) {
        bool booting = getQueuedFileEventCount__Q22cf13CfGameManagerFv() <= 3;
        if (!booting && isSceneActive()) {
            queueEventId__Q22cf13CfGameManagerFv(0x25);
        }
    }
}

void func_80280ADC() {
    u32 seq = getEventValue40__Q22cf13CfGameManagerFv(0x29);
    u32 n;
    // Two-arm if/else assigning n on both sides reproduces retail's
    // `beq bump; b after` pair (same shape as func_80280BF0).
    if (lbl_eu_80664772 == 0) {
        n = seq + 1;
        if (n >= 0xFFFF) {
            n = 0xFFFF;
        }
        setEventManagerValue__Q22cf13CfGameManagerFv(0x29, n);
    } else {
        n = seq;
    }
    if (n >= 0x1) {
        bool booting = getQueuedFileEventCount__Q22cf13CfGameManagerFv() <= 3;
        if (!booting && isSceneActive()) {
            queueEventId__Q22cf13CfGameManagerFv(0x29);
        }
    }
    if (n >= 0x64) {
        bool booting = getQueuedFileEventCount__Q22cf13CfGameManagerFv() <= 3;
        if (!booting && isSceneActive()) {
            queueEventId__Q22cf13CfGameManagerFv(0x2A);
        }
    }
    if (n >= 0x3E8) {
        bool booting = getQueuedFileEventCount__Q22cf13CfGameManagerFv() <= 3;
        if (!booting && isSceneActive()) {
            queueEventId__Q22cf13CfGameManagerFv(0x2B);
        }
    }
}

// Bump sequence counter 0x5A once while the scene is active, then fire
// one-shot closes at the 1 / 300 / 2000 thresholds. All three gates read the
// post-bump value (retail keeps it live in r31 across every gate).
void func_80280BF0() {
    u32 n;
    u32 cur = getEventValue40__Q22cf13CfGameManagerFv(0x5A);
    // Two-arm if/else assigning n on both sides reproduces retail's
    // `beq bump; b after` guard pair (same shape as func_802809C8).
    if (lbl_eu_80664772 != 0) {
        // Scene frozen by a subwindow: keep the un-bumped value.
        n = cur;
    } else {
        n = cur + 1;
        if (n >= 0xFFFF) {
            n = 0xFFFF;
        }
        setEventManagerValue__Q22cf13CfGameManagerFv(0x5A, n);
    }
    if (n >= 0x1) {
        bool booting = getQueuedFileEventCount__Q22cf13CfGameManagerFv() <= 3;
        if (!booting && isSceneActive()) {
            queueEventId__Q22cf13CfGameManagerFv(0x5A);
        }
    }
    if (n >= 0x12C) {
        bool booting = getQueuedFileEventCount__Q22cf13CfGameManagerFv() <= 3;
        if (!booting && isSceneActive()) {
            queueEventId__Q22cf13CfGameManagerFv(0x5B);
        }
    }
    if (n >= 0x7D0) {
        bool booting = getQueuedFileEventCount__Q22cf13CfGameManagerFv() <= 3;
        if (!booting && isSceneActive()) {
            queueEventId__Q22cf13CfGameManagerFv(0x5C);
        }
    }
}

// ---------------------------------------------------------------------------
// func_80280D04 (us-80283188)
// Progress the scenario-log sequence through its tiers. Each gate closes an
// earlier tier before later tiers can advance (1 / 100 / 1000, then, once
// self reaches 5, a nested 1 / 20 / 50 ladder).
// ---------------------------------------------------------------------------
void func_80280D04(s32 self) {
    u32 v = func_8027EE88(0x1D, self);
    if (v >= 1) {
        func_8027EEF4(0x1D);
    }
    if (v >= 0x64) {
        func_8027EEF4(0x1E);
    }
    if (v >= 0x3E8) {
        func_8027EEF4(0x1F);
    }
    if (self >= 5) {
        u32 w = func_8027EE88(0x20, 1);
        if (w >= 1) {
            func_8027EEF4(0x20);
        }
        if (w >= 0x14) {
            func_8027EEF4(0x21);
        }
        if (w >= 0x32) {
            func_8027EEF4(0x22);
        }
    }
}

// ---------------------------------------------------------------------------
// func_80280DBC (us-80283240)
// Scan a 2x2 grid of 8 u8 samples (stride 2) for values exceeding thresholds
// and notify the scenario log at ascending severities; count how many entries
// are >= 10 and emit a summary gate at 5 / 10 / 16.
// ---------------------------------------------------------------------------
void func_80280DBC(u8* self) {
    // Declaration order drives MWCC callee-saved coloring (first -> r31);
    // this order reproduces retail's register assignment.
    u8 val;
    // Alias keeps the parameter in the declaration stream so MWCC colors it
    // r30 like retail (params otherwise color last).
    u8* p = self;
    u8* p1;
    u8* p2;
    int count;
    int o;
    int m;
    int i;
    count = 0;
    for (o = 0; o < 2; o++) {
        p1 = p;
        for (m = 0; m < 2; m++) {
            p2 = p1;
            for (i = 0; i < 8; i++) {
                val = p2[0];
                if (val >= 2) {
                    func_8027EEF4(0x48);
                }
                if (val >= 5) {
                    func_8027EEF4(0x49);
                }
                if (val >= 10) {
                    func_8027EEF4(0x4A);
                }
                if (val >= 10) {
                    count++;
                }
                p2 += 2;
            }
            p1 += 0x10;
        }
        p += 0x49;
    }
    if (count >= 5) {
        func_8027EEF4(0x4B);
    }
    if (count >= 10) {
        func_8027EEF4(0x4C);
    }
    if (count >= 0x10) {
        func_8027EEF4(0x4D);
    }
}

// ---------------------------------------------------------------------------
// func_80280E9C (us-80283320)
// Walk 2 segments of 3 runs; each run scans 5 slots (stride 0x20) for any
// taken slot at +0x24. Returns early when a whole run is free; otherwise
// closes the sequence for 0x4E once all segments have at least one taken
// slot. The three run-scans are written out inline (retail unrolls them),
// and the segment stride is +0x24C (3 * 0xC4).
// ---------------------------------------------------------------------------
void func_80280E9C(u8* self) {
    u8* r;
    u8* s;
    int seg;
    int i;
    int run;
    u8* p = self + 0x3D4;
    for (seg = 0; seg < 4; seg += 2) {
        // Three identical run-scans (written as a 3-trip run loop; each scan
        // walks 5 slots of stride 0x20 looking for a taken u16 at +0x24; a
        // fully-free run makes the whole function return early).
        r = p;
        for (run = 0; run < 3; run++) {
            s = r;
            for (i = 0;; i++) {
                if (i >= 5)
                    return;
                if (*(u16*)(s + 0x24) != 0)
                    break;
                s += 0x20;
            }
            r += 0xC4;
        }
        p = r;
    }
    // seg == 4 at loop exit. Keeping this live-out use of seg is what makes
    // MWCC retain retail's segment-counter update inside the bdnz loop.
    func_8027EEF4(0x4A + seg);
}

// ---------------------------------------------------------------------------
// func_80280F44 (us-802833c8)
// Character-slot pressure sweep: for each character id 1..7 count how many of
// the 5 party-slot entries are fully occupied, emit sequence gates at 1 / 3
// (per-id 0x50..0x56 ladder) / 5 occupied entries, and close with a summary
// gate once 7 characters reached the >= 5 threshold.
// ---------------------------------------------------------------------------
void func_80280F44() {
    // Trip-count holders: constant-propagated to `li 5` and hoisted by LICM to
    // the function entry, where each colors into a callee-saved register that
    // feeds `mtctr` at every inner-loop entry (retail r31/r30/r29).
    int n8 = 5;
    int n3 = 5;
    int nN = 5;
    // Declaration order drives callee-saved coloring (first -> r31):
    // n8/n3/nN -> r31/r30/r29, c3 -> r28, count -> r27, total -> r26, i -> r25.
    int c3;
    int count;
    int total = 0;
    int i;
    for (i = 1; i <= 7; i++) {
        if (i != 3) {
            u8* p = (u8*)func_8009EC9C((u16)i) + 0x3534;
            count = 0;
            for (int k = nN; k > 0; k--) {
                CSysWinCharSlot* e = (CSysWinCharSlot*)p;
                int slot;
                if (e->sub0 == 0) {
                    slot = 0;
                } else if (e->sub1 == 0) {
                    slot = 1;
                } else if (e->sub2 == 0) {
                    slot = 2;
                } else if (e->sub3 == 0) {
                    slot = 3;
                } else if (e->sub4 == 0) {
                    slot = 4;
                } else {
                    slot = -1;
                }
                if (slot == -1) {
                    count++;
                }
                p += 0xC4;
            }
        } else {
            u8* p3 = (u8*)func_8009EC9C(3) + 0x3534;
            c3 = 0;
            for (int k = n3; k > 0; k--) {
                CSysWinCharSlot* e = (CSysWinCharSlot*)p3;
                int slot;
                if (e->sub0 == 0) {
                    slot = 0;
                } else if (e->sub1 == 0) {
                    slot = 1;
                } else if (e->sub2 == 0) {
                    slot = 2;
                } else if (e->sub3 == 0) {
                    slot = 3;
                } else if (e->sub4 == 0) {
                    slot = 4;
                } else {
                    slot = -1;
                }
                if (slot == -1) {
                    c3++;
                }
                p3 += 0xC4;
            }
            u8* p8 = (u8*)func_8009EC9C(8) + 0x3534;
            count = 0;
            for (int k = n8; k > 0; k--) {
                CSysWinCharSlot* e = (CSysWinCharSlot*)p8;
                int slot;
                if (e->sub0 == 0) {
                    slot = 0;
                } else if (e->sub1 == 0) {
                    slot = 1;
                } else if (e->sub2 == 0) {
                    slot = 2;
                } else if (e->sub3 == 0) {
                    slot = 3;
                } else if (e->sub4 == 0) {
                    slot = 4;
                } else {
                    slot = -1;
                }
                if (slot == -1) {
                    count++;
                }
                p8 += 0xC4;
            }
            if (count < c3) {
                count = c3;
            }
        }
        if (count >= 1) {
            func_8027EEF4(0x4f);
        }
        if (count >= 3) {
            if (i == 1) {
                func_8027EEF4(0x50);
            }
            if (i == 2) {
                func_8027EEF4(0x51);
            }
            if (i == 3) {
                func_8027EEF4(0x52);
            }
            if (i == 4) {
                func_8027EEF4(0x53);
            }
            if (i == 5) {
                func_8027EEF4(0x54);
            }
            if (i == 6) {
                func_8027EEF4(0x55);
            }
            if (i == 7) {
                func_8027EEF4(0x56);
            }
        }
        if (count >= 5) {
            func_8027EEF4(0x58);
            total++;
        }
    }
    if (total >= 7) {
        func_8027EEF4(0x59);
    }
}

extern "C" void func_802811FC(u8* self){
    *(u32*)(self + 0x1D80) = 0;
    *(u32*)(self + 0x1D84) = 0;
}

// ---------------------------------------------------------------------------
// func_8028120C (us-80283690)
// Find a free slot in one of the two round-robin slot tables (selected by the
// owner's flag word: bit 1 -> table A at +0x00, bit 2 -> table B at +0x180),
// bump the table's index (wrapping at 3 / 0x38), call the free slot's vtable
// slot-3 method, and return the slot pointer. Returns null when neither bit is
// set.
// ---------------------------------------------------------------------------
void* func_8028120C(CSysWinSlotTable* self, CScenarioLogOwner* arg0) {
    void* result;
    u32 flags = arg0->field_0x3F00;
    if ((flags & 0x2) != 0) {
        int idx = 0;
        while (self->mEntriesA[self->mIdxA].mUsed != 0) {
            self->mIdxA += 1;
            if (self->mIdxA >= 3) {
                self->mIdxA = idx;
            }
        }
        csysWinSlotCall3(&self->mEntriesA[self->mIdxA]);
        result = &self->mEntriesA[self->mIdxA];
        return result;
    }
    if ((flags & 0x4) != 0) {
        int idx = 0;
        while (self->mEntriesB[self->mIdxB].mUsed != 0) {
            self->mIdxB += 1;
            if (self->mIdxB >= 0x38) {
                self->mIdxB = idx;
            }
        }
        csysWinSlotCall3(&self->mEntriesB[self->mIdxB]);
        result = &self->mEntriesB[self->mIdxB];
        return result;
    }
    result = 0;
    return result;
}

// ---------------------------------------------------------------------------
// Target-only helper definitions (moved below their callers to block inline).
// ---------------------------------------------------------------------------
extern "C" void __declspec(noinline) func_8027EEF4(u32 self) {
    bool booting = getQueuedFileEventCount__Q22cf13CfGameManagerFv() <= 3;
    if (!booting && lbl_eu_80664772 == 0) {
        queueEventId__Q22cf13CfGameManagerFv(self);
    }
}

extern "C" u32 __declspec(noinline) func_8027EE88(u32 self, u32 arg) {
    u32 v = getEventValue40__Q22cf13CfGameManagerFv(self);
    if (lbl_eu_80664772 != 0) {
        // Scene frozen by a subwindow: return the un-bumped value.
        return v;
    }
    u32 sum = v + arg;
    if (sum >= 0xFFFF) {
        sum = 0xFFFF;
    }
    setEventManagerValue__Q22cf13CfGameManagerFv(self, sum);
    return sum;
}

// ---------------------------------------------------------------------------
// func_8027EC80 (us-80281104)
// File-event handler for the scenario-log window. When the exchanged file
// matches mFileHandle74: create the scratch region, take the file data,
// allocate + construct the tag processor, build the arc accessor + layout +
// animation, bind the font and animate, then return 1. When it matches
// mFileHandle78 instead: release the BDAT archive and store the loaded FP in
// lbl_eu_8066490C, return 1. Otherwise return 0.
// ---------------------------------------------------------------------------
extern "C" int func_8027EC80(CSysWinScenarioLog* self, CFileHandle* fh) {
    void* data = fh->mData;
    if (self->mFileHandle74 == data) {
        self->mMemRegion.createRegion(mtl::MemManager::getHandleMEM2(), 0x8000,
                                      &lbl_eu_8050EE24[0x9e], 0);
        Class_8045F858 memHost(&self->mMemRegion);
        void* fileData = self->mFileHandle74->getData();
        mtl::MemManager::setMemInitFlag(false);
        void* tag = mtl::MemManager::allocate(0x858, getAllocHandle__10CLibLayoutFv());
        if (tag != 0) {
            tag = __ct__CTagProcessor(tag);
        }
        self->mField94 = (nw4r::lyt::Layout*)tag;
        self->mpAccessor = createArcResourceAccessor__10CLibLayoutFv();
        self->mpAccessor->Attach(fileData, &lbl_eu_8050EE24[0xb1]);
        func_80136E84__FPPQ34nw4r3lyt6LayoutPQ34nw4r3lyt19ArcResourceAccessorPCc(
            &self->mpLayout, self->mpAccessor, &lbl_eu_8050EE24[0xb5]);
        func_80136F08__FPQ34nw4r3lyt6LayoutPPQ34nw4r3lyt13AnimTransformPQ34nw4r3lyt19ArcResourceAccessorPc(
            self->mpLayout, &self->mpAnim, self->mpAccessor, &lbl_eu_8050EE24[0xce]);
        nw4r::lyt::Pane* rootPane = self->mpLayout->GetRootPane();
        void* fontObj = CDeviceFont::getFontInfo(1, self->mpLayout);
        u32 fontHandle = ((CSysWinFontObjView*)fontObj)->getFontHandle();
        func_8013676C(rootPane, fontHandle);
        self->mpLayout->SetAnimationEnable(self->mpAnim, true);
        self->mpLayout->Animate(0);
        void* pane = ((CSysWinPaneView*)self->mpLayout->GetRootPane())->mSetText(
            &lbl_eu_8050EE24[0x5b], 1);
        ((CSysWinPaneView*)pane)->field_0xF8 = (u32)self->mField94;
        self->mFileHandle74 = 0;
        self->mMemRegion.func_8045F810();
        return 1;
    }
    if (self->mFileHandle78 == data) {
        void* fileData78 = self->mFileHandle78->getData();
        func_8003AA78__5CBdatFUlPv(2, fileData78);
        func_8003AA34();
        lbl_eu_8066490C = (u32)getFP__FPCc(&lbl_eu_8050EE24[0xea]);
        self->mFileHandle78 = 0;
        return 1;
    }
    return 0;
}

// ---------------------------------------------------------------------------
// CSysWinScenarioLog::Init (us-80280aac)
// Register this window's IScnRender sub-object (+0x70) with the owning scene,
// then kick off both file loads against the IWorkEvent sub-object (+0x6C):
// the layout archive from MEM2 and the BDAT common archive (+0x18 into the
// string pool).
// ---------------------------------------------------------------------------
void CSysWinScenarioLog::Init() {
    // Null-this tolerant sub-object selection: seed with the raw pointer,
    // then overwrite with the embedded interface offset when this is live.
    // A fresh selection per call site matches retail, which re-materializes
    // the +0x6c IWorkEvent offset before each file request.
    IScnRender* render = reinterpret_cast<IScnRender*>(this);
    if (this != nullptr) {
        render = reinterpret_cast<IScnRender*>(&mScnRender);
    }
    mScene->addRenderCB(render, 0xD, 0);

    IWorkEvent* ev74 = reinterpret_cast<IWorkEvent*>(this);
    if (this != nullptr) {
        ev74 = reinterpret_cast<IWorkEvent*>(&mWorkEvent);
    }
    mFileHandle74 = CDeviceFile::readFile(
        mtl::MemManager::getHandleMEM2(), lbl_eu_8050EE24, ev74, 0, 0);

    IWorkEvent* ev78 = reinterpret_cast<IWorkEvent*>(this);
    if (this != nullptr) {
        ev78 = reinterpret_cast<IWorkEvent*>(&mWorkEvent);
    }
    mFileHandle78 = CDeviceFile::readCommonArchiveFile(
        func_800A9D90(), &lbl_eu_8050EE24[0x18], ev78, 0, 0);
}

// ---------------------------------------------------------------------------
// CSysWinScenarioLog::Term (us-80280b60)
// Tear down the scenario-log window: wait for the VI, release the two file
// handles and the BDAT archive, destroy the layout pair (vtable slot-2 call
// with arg 1, guarded like retail's double-beq), free the arc accessor, clear
// the mem region + globals, deregister the render callback from the scene, and
// drop the open-window bookkeeping counter once per boot.
// ---------------------------------------------------------------------------
void CSysWinScenarioLog::Term() {
    CDeviceVI::waitForDrawDone();
    func_801390E0(&mFileHandle74);
    func_801390E0(&mFileHandle78);
    getEntry__5CBdatFUl(2);

    if (mpLayout != 0) {
        nw4r::lyt::Layout* p = mpLayout;
        if (p != 0) {
            ((CSysWinLayoutSlot2View*)p)->mSlot2(1);
        }
        mpLayout = 0;
    }
    if (mField94 != 0) {
        nw4r::lyt::Layout* p = mField94;
        if (p != 0) {
            ((CSysWinLayoutSlot2View*)p)->mSlot2(1);
        }
        mField94 = 0;
    }
    func_80139124(mpAccessor);
    mpAccessor = 0;
    lbl_eu_8066490C = 0;
    mMemRegion.func_8045F778();
    lbl_eu_80664908 = 0;

    IScnRender* render = reinterpret_cast<IScnRender*>(this);
    if (this) {
        render = reinterpret_cast<IScnRender*>(&mScnRender);
    }
    mScene->removeRenderCB(render);

    func_8013B980();
    if (code80135FDC_getByte_64080() == 0) {
        setPresentationFlag__Q22cf13CfGameManagerFv(false);
    }
}
