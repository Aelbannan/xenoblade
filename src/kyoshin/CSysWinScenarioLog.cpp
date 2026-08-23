// Auto-scaffolded catalog TU for kyoshin/CSysWinScenarioLog

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
// active; returns the (possibly bumped) value for later gate checks. The pause
// gate is written as an early return (same shape as the matched func_8027EE88
// helper) so MWCC lays the bump out like retail's branch-over-branch.
static inline u32 scenarioBump(u32 id) {
    u32 v = func_80082694__Q22cf13CfGameManagerFv(id);
    u16 pauseFlag = lbl_eu_80664772;
    u32 n = v;
    if (pauseFlag != 0) {
        return n;
    }
    n = n + 1;
    if (n >= 0xFFFF) {
        n = 0xFFFF;
    }
    func_8008269C__Q22cf13CfGameManagerFv(id, n);
    return n;
}

static inline void scenarioClose(u32 id) {
    bool booting = func_800822F4__Q22cf13CfGameManagerFv() <= 3;
    if (!booting && isSceneActive()) {
        func_800826F0__Q22cf13CfGameManagerFv(id);
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
// ---- Target 1: func_8027F0A0 (us-80281524) -------------------------------
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
// ---- Target 5: CSysWinScenarioLog::~CSysWinScenarioLog (us-80280a38) ------
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
// ---- Target 5: CSysWinScenarioLog::CSysWinScenarioLog (us-80280934) -------
// C-linkage ctor (retail symbol __ct__CSysWinScenarioLog carries no class-length
// mangling, so it stays a plain global function - same idiom as CSystemWindow).
// Runs CProcess's ctor, fills the IUIWindow base region by hand (temp vtable at
// +0x10, the two __ptmf_null callback slots, control words), then the composite
// vtable + work-event/render slots, the embedded UnkClass_8045F564 region, and
// the window fields; finally bumps the open-window bookkeeping counters.
// ---------------------------------------------------------------------------
extern "C" void* __ct__CSysWinScenarioLog(void* _this, void* param) {
    CSysWinScenarioLog* s = (CSysWinScenarioLog*)_this;
    __ct__8CProcessFv((CProcess*)s);

    ((CSysWinProcessVtable*)s)->mpVtable = (u32)lbl_eu_8052D238;
    u32* ptmf = __ptmf_null;
    s->ptmf0[0] = ptmf[0];
    s->ptmf0[1] = ptmf[1];
    s->ptmf0[2] = ptmf[2];
    s->ptmf1[0] = ptmf[0];
    s->ptmf1[1] = ptmf[1];
    s->ptmf1[2] = ptmf[2];

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

    func_8008294C__Q22cf13CfGameManagerFv(true);
    code80135FDC_postIncByte_64080();
    return s;
}

// ---------------------------------------------------------------------------
// ---- Target 6: func_8027E9E8 (us-80280e6c) -------------------------------
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
// ---- Target 8: func_8027F0B8 (us-8028153c) -------------------------------
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
            bool booting = func_800822F4__Q22cf13CfGameManagerFv() <= 3;
            if (!booting && isSceneActive()) {
                func_800826F0__Q22cf13CfGameManagerFv(0x3A);
            }
        }
        lbl_eu_80664910 = 1;
    }
}

// ---------------------------------------------------------------------------
// ---- Target 10: func_80280804 (us-80282c88) ------------------------------
// Object that owns a state-flags dword at +0x3F00.
// ---------------------------------------------------------------------------
struct CScenarioFlagObj {
    u8 _0[0x3F00];
    u32 field_0x3F00;   // flags
};

void func_80280804(CScenarioFlagObj* self) {
    if ((self->field_0x3F00 & 0x2) != 0) {
        // Bump sequence counter 0xB while active, then close it past 100.
        // Bump sequence counter 0xB while active, then close it past 100.
        u32 cur = scenarioBump(0xB);
        if (cur >= 0x64) {
            scenarioClose(0xB);
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
                bool booting = func_800822F4__Q22cf13CfGameManagerFv() <= 3;
                if (!booting && isSceneActive()) {
                    func_800826F0__Q22cf13CfGameManagerFv(0xC);
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
                bool booting = func_800822F4__Q22cf13CfGameManagerFv() <= 3;
                if (!booting && isSceneActive()) {
                    func_800826F0__Q22cf13CfGameManagerFv(0x3B);
                }
            }
        }
    }
    lbl_eu_80664910 = 0;
    lbl_eu_80664912 = 0;
}
// ---------------------------------------------------------------------------
// ---- Target: CSysWinScenarioLog::cbRenderBefore (us-80280dc0) -------------
// Draw the scenario-log layout behind the scene while the game is live: same
// gate chain as Move() but testing bit 10 of the pause word, then Z-off and
// a layout draw with a stack DrawInfo (raw buffer, direct ctor/dtor calls).
// ---------------------------------------------------------------------------
void CSysWinScenarioLog::cbRenderBefore() {
    // Same gate chain as Move() (pause word bit 21 here): skip the draw while
    // a modal task-game state is live, the scenario bit is set, an event is
    // pending, a camera event runs, or the layout isn't loaded.
    if (CTaskGame::getInstance()->func_800426F0() == false) {
        if (lbl_eu_80663E28 & 0x200000) {
            // scenario/pause bit set: skip the whole draw
        } else if (func_8013BE50() != 0 &&
                   func_8029A658() == 0 &&
                   mpLayout != 0) {
            GXSetZMode(GX_FALSE, GX_NEVER, GX_FALSE);
            u8 drawInfo[0x60];
            __ct__Q34nw4r3lyt8DrawInfoFv(drawInfo);
            func_80137250(reinterpret_cast<nw4r::lyt::DrawInfo*>(drawInfo));
            func_80137038(mpLayout, reinterpret_cast<nw4r::lyt::DrawInfo*>(drawInfo),
                          0, 1);
            __dt__Q34nw4r3lyt8DrawInfoFv(
                reinterpret_cast<nw4r::lyt::DrawInfo*>(drawInfo), -1);
        }
    }
}

// ---------------------------------------------------------------------------
// ---- Target 6: CSysWinScenarioLog::Move (us-80280c54) ---------------------
// Advance the scenario-log window's opening/closing state machine.
// ---------------------------------------------------------------------------
void CSysWinScenarioLog::Move() {
    // Gate: not in a modal task-game state, pause bit clear, event byte set,
    // no active camera event, and the layout is loaded.
    if (CTaskGame::getInstance()->func_800426F0() == false) {
        if (lbl_eu_80663E28 & 0x200000) {
            // pause/scenario bit set: skip the whole update
        } else if (func_8013BE50() != 0 &&
                   func_8029A658() == 0 &&
                   mpLayout != 0) {
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
            if (func_80086F9C__Q22cf13CfGameManagerFv(-1) != 0) {
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
}

// ---------------------------------------------------------------------------
// ---- Target 4: func_8027EA6C (us-80280ef0) -------------------------------
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
    const char* sel = func_80086F9C__Q22cf13CfGameManagerFv(-1) != 0
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
// ---- Target 2: func_8027F148 (us-802815cc) -------------------------------
// Scenario-log open gate: every actor in the manager list must report its
// device usable (vtable 0x2bc) and pass the arts-state gate
// (func_80174C98 with the +0x4 sub-object's slot-0x30 state, mode 0x1d);
// when all pass, bump/close the 0x3c counter (100-threshold close) and the
// 0x57 counter (50-threshold close).
// ---------------------------------------------------------------------------
void func_8027F148() {
    int result;
    CSysWinActorList* list = func_800B6BA4();
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
        u8* base = node->object;
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
        u32 v = scenarioBump(0x3c);
        if (v >= 1) {
            scenarioClose(0x3c);
        }
        if (v >= 0x32) {
            scenarioClose(0x57);
        }
    }
}

// ---------------------------------------------------------------------------
// ---- Target 2: func_8027F2DC (us-80281760) -------------------------------
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
                s32 val = (s32)p->field_0x8->field_0x18;
                if (val < 7 || val > 0xE) {
                    result = 0;
                } else {
                    result = 1;
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
// ---- Target 1: func_8027F848 (us-80281ccc) -------------------------------
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
// ---- Target 4: func_8027FC04 (us-80282088) -------------------------------
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
// ---- Target 3: func_8027FC80 (us-80282104) -------------------------------
// Drive the scenario-log counters for a given device/window pair: bumps 0x4,
// then conditionally runs 0xd/0x10/0x13/0x1c ladders based on the window state
// (device flag word + a vtable-returned frame counter), and finally, when the
// arg1 player matches the first slot, closes 0x5d/0x5e.
// NOTE: the 0x13 ladder's exact per-id thresholds are reconstructed as a
// best-effort ladder over ids 0x13..0x1b (the 0x1b close is visible in
// retail); only 0x13/0x1b and the surrounding control flow are byte-visible.
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
    }

    if (guard == 0) {
        u32 flags = arg0->field_0x3374;
        if ((flags & 0x100) == 0 && (flags & 0x8000) == 0) {
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
            // 0x13 ladder: closing 0x13..0x1b (best-effort thresholds).
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
            if (v13 >= 0x3E8) {
                scenarioClose(0x16);
            }
            if (v13 >= 0x1388) {
                scenarioClose(0x17);
            }
            if (v13 >= 0x2710) {
                scenarioClose(0x18);
            }
            if (v13 >= 0x4E20) {
                scenarioClose(0x19);
            }
            if (v13 >= 0x9C40) {
                scenarioClose(0x1A);
            }
            if (v13 >= 0x13880) {
                scenarioClose(0x1B);
            }
        }
    }

    if (guard == 0) {
        if (arg0->field_0x3374 & 0x8000) {
            u32 v1c = scenarioBump(0x1C);
            if (v1c >= 0x1E) {
                scenarioClose(0x1C);
            }
        }
    }

    if (arg1 != 0) {
        CScenarioLogOwner* p = csysWinCall9C(arg1);
        const u8* pp = p ? &p->field_0x3E9C : nullptr;
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
// ---- Target: func_802804F8 (us-8028297c) ----------------------------------
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
// ---- Target 1: func_80280588 (us-80282a0c) --------------------------------
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
// ---- Target 1: func_80280640 (us-80282ac4) -------------------------------
// Device-side scenario-log gate (variant of func_80280588): when the device's
// owner is set with flag bit 2 live and the one-shot byte is clear, ask the
// device (vtable 0x2bc) whether it is usable; if so, scan the actor list for
// the first owner that passes a state gate (sub-state 1: flag bits 14/15;
// sub-state 4: the character category is 1) and drive the pair's counters
// with that owner, then latch the one-shot byte.
// ---------------------------------------------------------------------------
void func_80280640(CSysWinDevice* self) {
    bool flag = false;
    if (((CSysWinDevView*)self)->mAt9C() != 0 &&
        ((((CSysWinDevView*)self)->mAt9C())->field_0x3F00 & 0x4)) {
        flag = true;
    }
    // Gate chain: retail emits `bne walk; b gate_done` (branch-over-branch)
    // for the final device-usable test - the last test written negated with
    // an explicit `goto walk` after it (MWCC_CASES verified pattern).
    if (flag == 0) {
        goto gate_done;
    }
    if (lbl_eu_80664918 != 0) {
        goto gate_done;
    }
    if (((CSysWinDevView*)self)->mAt2BC() == 0) {
        goto gate_done;
    }
    goto walk;
walk:
    {
        CSysWinActorList* list = func_800B6BA4();
        CSysWinActorListNode* node = list->sentinel->next;
        CScenarioLogOwner* found;
        while (node != list->sentinel) {
            CScenarioLogOwner* owner = (CScenarioLogOwner*)node->object;
            if (owner != 0) {
                owner = (CScenarioLogOwner*)((u8*)owner - 0x3E9C);
            }
            int result = 0;
            u16 h = owner->field_0x3F28;
            if (h == 1) {
                u32 flags = owner->field_0x3374;
                result = 1;
                if ((flags & 0x4000) == 0 && (flags & 0x8000) == 0) {
                    result = 0;
                }
            } else if (h == 4) {
                func_8009EC9C(4);
                result = (func_800A32BC() == 1);
            }
            if (result != 0) {
                found = owner;
                goto done;
            }
            node = node->next;
        }
        // Loop fell through with no matching owner: pass null. (The
        // found = owner path jumps past this default, matching retail.)
        found = 0;
    done:
        func_8027FC80(self, found);
        lbl_eu_80664918 = 1;
    }
gate_done:
    ;
}
// ---------------------------------------------------------------------------
// ---- Target 1: func_802807A0 (us-80282c24) -------------------------------
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
    u32 seq = func_80082694__Q22cf13CfGameManagerFv(0x2C);
    // Explicit gotos reproduce retail's `beq bump; b after` pair.
    if (lbl_eu_80664772 == 0) {
        goto bump;
    }
    goto after;
bump:
    seq = seq + 1;
    if (seq >= 0xFFFF) {
        seq = 0xFFFF;
    }
    func_8008269C__Q22cf13CfGameManagerFv(0x2C, seq);
after:
    ;
    if (seq >= 0x1) {
        bool booting = func_800822F4__Q22cf13CfGameManagerFv() <= 3;
        if (!booting && isSceneActive()) {
            func_800826F0__Q22cf13CfGameManagerFv(0x2C);
        }
    }
    if (seq >= 0x32) {
        bool booting = func_800822F4__Q22cf13CfGameManagerFv() <= 3;
        if (!booting && isSceneActive()) {
            func_800826F0__Q22cf13CfGameManagerFv(0x2D);
        }
    }
    if (seq >= 0xC8) {
        bool booting = func_800822F4__Q22cf13CfGameManagerFv() <= 3;
        if (!booting && isSceneActive()) {
            func_800826F0__Q22cf13CfGameManagerFv(0x2E);
        }
    }
}

// ---------------------------------------------------------------------------
// ---- Target 10: func_802809C8 (us-80282e4c) -------------------------------
// Bump/close the scenario-log sequence counters at three gates (10 / 100 /
// 500). Retails only clamps once per open window and only closes each gate
// when the scene is not booting and no subwindow is freezing the game.
// ---------------------------------------------------------------------------
void func_802809C8() {
    u32 seq = func_80082694__Q22cf13CfGameManagerFv(0x23);
    // Single-case switch reproduces retail's `beq bump; b after` pair.
    switch (lbl_eu_80664772) {
    case 0:
        seq = seq + 1;
        if (seq >= 0xFFFF) {
            seq = 0xFFFF;
        }
        func_8008269C__Q22cf13CfGameManagerFv(0x23, seq);
        break;
    }
after:
    ;
    if (seq >= 0xA) {
        bool booting = func_800822F4__Q22cf13CfGameManagerFv() <= 3;
        if (!booting && isSceneActive()) {
            func_800826F0__Q22cf13CfGameManagerFv(0x23);
        }
    }
    if (seq >= 0x64) {
        bool booting = func_800822F4__Q22cf13CfGameManagerFv() <= 3;
        if (!booting && isSceneActive()) {
            func_800826F0__Q22cf13CfGameManagerFv(0x24);
        }
    }
    if (seq >= 0x1F4) {
        bool booting = func_800822F4__Q22cf13CfGameManagerFv() <= 3;
        if (!booting && isSceneActive()) {
            func_800826F0__Q22cf13CfGameManagerFv(0x25);
        }
    }
}

void func_80280ADC() {
    u32 seq = func_80082694__Q22cf13CfGameManagerFv(0x29);
    // Void-arm ternary (bump as false-arm): MWCC keeps the two-branch pair
    // (`beq bump; b gates`) without spilling a second register.
    lbl_eu_80664772 != 0 ? (void)0
                         : (void)(seq = seq + 1 < 0x10000 ? seq + 1 : 0xFFFF,
                                  func_8008269C__Q22cf13CfGameManagerFv(0x29, seq));
gates:
    ;
    if (seq >= 0x1) {
        bool booting = func_800822F4__Q22cf13CfGameManagerFv() <= 3;
        if (!booting && isSceneActive()) {
            func_800826F0__Q22cf13CfGameManagerFv(0x29);
        }
    }
    if (seq >= 0x64) {
        bool booting = func_800822F4__Q22cf13CfGameManagerFv() <= 3;
        if (!booting && isSceneActive()) {
            func_800826F0__Q22cf13CfGameManagerFv(0x2A);
        }
    }
    if (seq >= 0x3E8) {
        bool booting = func_800822F4__Q22cf13CfGameManagerFv() <= 3;
        if (!booting && isSceneActive()) {
            func_800826F0__Q22cf13CfGameManagerFv(0x2B);
        }
    }
}

void func_80280BF0() {
    u32 seq = func_80082694__Q22cf13CfGameManagerFv(0x5A);
    u32 n;
    // If/else with both arms assigning: retail hoists the common value then
    // emits `beq bump; b after` around the two arms.
    if (lbl_eu_80664772 == 0) {
        n = seq + 1;
        if (n >= 0xFFFF) {
            n = 0xFFFF;
        }
        func_8008269C__Q22cf13CfGameManagerFv(0x5A, n);
    } else {
        n = seq;
    }
    if (n >= 0x1) {
        bool booting = func_800822F4__Q22cf13CfGameManagerFv() <= 3;
        if (!booting && isSceneActive()) {
            func_800826F0__Q22cf13CfGameManagerFv(0x5A);
        }
    }
    if (seq >= 0x12C) {
        bool booting = func_800822F4__Q22cf13CfGameManagerFv() <= 3;
        if (!booting && isSceneActive()) {
            func_800826F0__Q22cf13CfGameManagerFv(0x5B);
        }
    }
    if (n >= 0x7D0) {
        bool booting = func_800822F4__Q22cf13CfGameManagerFv() <= 3;
        if (!booting && isSceneActive()) {
            func_800826F0__Q22cf13CfGameManagerFv(0x5C);
        }
    }
}

// ---------------------------------------------------------------------------
// ---- Target 5: func_80280D04 (us-80283188) --------------------------------
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
// ---- Target 6: func_80280DBC (us-80283240) --------------------------------
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
// ---- Target 1: func_80280E9C (us-80283320) --------------------------------
// Walk 2 segments of 3 runs; each run scans 5 slots (stride 0x20) for a free
// one at +0x24. Returns when a whole run is free; otherwise closes the
// sequence for 0x4E once all segments have at least one taken slot. Void
// return keeps the final func_8027EEF4 call a tail jump (no callee-saved regs/
// frame needed, matching retail).
// ---------------------------------------------------------------------------
void func_80280E9C(u8* self) {
    u8* p = self + 0x3D4;
    for (int s = 0; s < 2; s++) {
        for (int run = 0; run < 3; run++) {
            u16* q = (u16*)(p + 0x24);
            int i;
            for (i = 0; i < 5; i++) {
                if (q[0] != 0) {
                    break;
                }
                q += 0x10;   /* slot stride 0x20 / sizeof(u16) */
            }
            if (i >= 5) {
                return;
            }
            p += 0xC4;
        }
    }
    func_8027EEF4(0x4E);
}

// ---------------------------------------------------------------------------
// ---- Target 5: func_80280F44 (us-802833c8) -------------------------------
// Character-slot pressure sweep: for each character id 1..7 count how many of
// the 5 party-slot entries are fully occupied, emit sequence gates at 1 / 3
// (per-id 0x50..0x56 ladder) / 5 occupied entries, and close with a summary
// gate once 7 characters reached the >= 5 threshold.
// ---------------------------------------------------------------------------
// Count how many of the 5 character entries (stride 0xC4) are fully occupied
// (every one of the five sub-slot pointers at +0x20..+0xA0 is non-null). The
// scan loops are written inline (three copies) so the loop counters land in
// the retail's high registers (r29-r31) rather than the inlined helper's.
static inline int countFullEntries(u8* p) {
    int n = 0;
    for (int k = 0; k < 5; k++, p += 0xC4) {
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
            n++;
        }
    }
    return n;
}

void func_80280F44() {
    int total = 0;
    for (int i = 1; i <= 7; i++) {
        int count;
        if (i != 3) {
            u8* p = (u8*)func_8009EC9C((u16)i) + 0x3534;
            count = 0;
            for (int k = 5; k > 0; k--) {
                CSysWinCharSlot* e = (CSysWinCharSlot*)p;
                int slot;
                if (e->sub0 == 0) {
                    slot = 0;
                    goto chk;
                }
                if (e->sub1 == 0) {
                    slot = 1;
                    goto chk;
                }
                if (e->sub2 == 0) {
                    slot = 2;
                    goto chk;
                }
                if (e->sub3 == 0) {
                    slot = 3;
                    goto chk;
                }
                if (e->sub4 == 0) {
                    slot = 4;
                    goto chk;
                }
                slot = -1;
            chk:
                if (slot == -1) {
                    count++;
                }
                p += 0xC4;
            }
        } else {
            u8* p3 = (u8*)func_8009EC9C(3) + 0x3534;
            int c3 = 0;
            for (int k = 5; k > 0; k--, p3 += 0xC4) {
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
            }
            u8* p8 = (u8*)func_8009EC9C(8) + 0x3534;
            count = 0;
            for (int k = 5; k > 0; k--, p8 += 0xC4) {
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
// ---- Target 4: func_8028120C (us-80283690) --------------------------------
// Find a free slot in one of the two round-robin slot tables (selected by the
// owner's flag word: bit 1 -> table A at +0x00, bit 2 -> table B at +0x180),
// bump the table's index (wrapping at 3 / 0x38), call the free slot's vtable
// slot-3 method, and return the slot pointer. Returns null when neither bit is
// set.
// ---------------------------------------------------------------------------
void* func_8028120C(CSysWinSlotTable* self, CScenarioLogOwner* arg0) {
    u32 flags = arg0->field_0x3F00;
    if (flags & 0x2) {
        int idx = 0;
        while (self->mEntriesA[self->mIdxA].mUsed != 0) {
            if (++self->mIdxA >= 3) {
                self->mIdxA = idx;
            }
        }
        csysWinSlotCall3(&self->mEntriesA[self->mIdxA]);
        return &self->mEntriesA[self->mIdxA];
    }
    if (flags & 0x4) {
        int idx = 0;
        while (self->mEntriesB[self->mIdxB].mUsed != 0) {
            if (++self->mIdxB >= 0x38) {
                self->mIdxB = idx;
            }
        }
        csysWinSlotCall3(&self->mEntriesB[self->mIdxB]);
        return &self->mEntriesB[self->mIdxB];
    }
    return 0;
}

// ---------------------------------------------------------------------------
// Target-only helper definitions (moved below their callers to block inline).
// ---------------------------------------------------------------------------
extern "C" void __declspec(noinline) func_8027EEF4(u32 self) {
    bool booting = func_800822F4__Q22cf13CfGameManagerFv() <= 3;
    if (!booting && lbl_eu_80664772 == 0) {
        func_800826F0__Q22cf13CfGameManagerFv(self);
    }
}

extern "C" u32 __declspec(noinline) func_8027EE88(u32 self, u32 arg) {
    u32 v = func_80082694__Q22cf13CfGameManagerFv(self);
    if (lbl_eu_80664772 != 0) {
        // Scene frozen by a subwindow: return the un-bumped value.
        return v;
    }
    u32 sum = v + arg;
    if (sum >= 0xFFFF) {
        sum = 0xFFFF;
    }
    func_8008269C__Q22cf13CfGameManagerFv(self, sum);
    return sum;
}

// ---------------------------------------------------------------------------
// ---- Target 3: func_8027EC80 (us-80281104) -------------------------------
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
        mtl::MemManager::func_80434A4C(false);
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
        void* fontObj = CDeviceFont::func_80452C10(1, self->mpLayout);
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
// ---- Target: CSysWinScenarioLog::Init (us-80280aac) -----------------------
// Register this window's IScnRender sub-object (+0x70) with the owning scene,
// then kick off both file loads against the IWorkEvent sub-object (+0x6C):
// the layout archive from MEM2 and the BDAT common archive (+0x18 into the
// string pool).
// ---------------------------------------------------------------------------
void CSysWinScenarioLog::Init() {
    IScnRender* render = reinterpret_cast<IScnRender*>(this); // null-this -> null cb
    if (this) render = reinterpret_cast<IScnRender*>(&mScnRender);
    mScene->addRenderCB(render, 0xD, 0);

    IWorkEvent* ev = reinterpret_cast<IWorkEvent*>(this); // default keeps null-this
    if (this) ev = reinterpret_cast<IWorkEvent*>(&mWorkEvent);

    mFileHandle74 = CDeviceFile::readFile(
        mtl::MemManager::getHandleMEM2(), lbl_eu_8050EE24, ev, 0, 0);
    mFileHandle78 = CDeviceFile::readCommonArchiveFile(
        func_800A9D90(), &lbl_eu_8050EE24[0x18], ev, 0, 0);
}

// ---------------------------------------------------------------------------
// ---- Target 3: CSysWinScenarioLog::Term (us-80280b60) ---------------------
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
    func_8003AA8C__5CBdatFUl(2);

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
        func_8008294C__Q22cf13CfGameManagerFv(false);
    }
}
