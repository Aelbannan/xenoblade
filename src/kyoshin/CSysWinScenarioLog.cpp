// Auto-scaffolded catalog TU for kyoshin/CSysWinScenarioLog

#include <types.h>

#include "kyoshin/harness_catalog.hpp"
#include "kyoshin/CSysWinScenarioLog.hpp"
#include "monolib/util/MemManager.hpp"
#include "monolib/work/CWorkThreadSystem.hpp"
#include "kyoshin/cf/CfGameManager.hpp"
#include "kyoshin/cf/object/CfObject.hpp"
#include "kyoshin/CTaskGame.hpp"

#include <nw4r/lyt/lyt_layout.h>

// ---------------------------------------------------------------------------
// Retail callees referenced by the (still-unmatched) CSysWinScenarioLog unit.
// The CfGameManager "sequence/flag" helpers are mangled Fv in retail even
// though they take an argument (see CfMapMineManager / code_80135FDC for the
// same convention), so reference them via their retail mangled names.
// ---------------------------------------------------------------------------
extern "C" u32  func_80082694__Q22cf13CfGameManagerFv(u32 id);   // get sequence value
extern "C" void func_8008269C__Q22cf13CfGameManagerFv(u32 id, u32 value); // set sequence value
extern "C" void func_800826F0__Q22cf13CfGameManagerFv(u32 value);
extern "C" u32  func_800822F4__Q22cf13CfGameManagerFv(); // unsigned cf sequence counter
extern "C" void* getInstance__Q22cf14CBattleManagerFv();
extern "C" void* func_8016FE34(void* source);
extern "C" u16  lbl_eu_80664772;          // pause / non-enemy-scene flag
extern "C" u16  lbl_eu_80663E42;          // current area id (func_8027FC80 gate)
extern "C" u32  lbl_eu_80664908;          // CSysWinScenarioLog singleton
extern "C" u8   lbl_eu_80664910;
extern "C" u8   lbl_eu_80664911;
extern "C" u8   lbl_eu_80664912;
extern "C" u32  lbl_eu_80664914;
extern "C" u8   lbl_eu_80664918;

extern "C" void __dt__17UnkClass_8045F564Fv(void*, int);
extern "C" void __dt__8CProcessFv(void*, int);
extern "C" void* __dl__FPv(void*);
extern "C" void __ct__CSysWinScenarioLog(void* _this, void* param);

// Float thresholds used by func_8027EF50's player-range check.
extern "C" f32 lbl_eu_80668AE0;
extern "C" f32 lbl_eu_80668AE4;

// CSysWinScenarioLog::Move helpers.
extern "C" u8   code80135FDC_getByte_621F0();
extern "C" s32  func_8029A658();
extern "C" u32  func_80137444__FPQ34nw4r3lyt13AnimTransformf(nw4r::lyt::AnimTransform*, float);
extern "C" u32  func_80137510(nw4r::lyt::AnimTransform*, float);
extern "C" void func_80138078__FUl(u32);
extern "C" bool func_80086F9C__Q22cf13CfGameManagerFv(int arg);
extern "C" u32  lbl_eu_80663E28;
extern "C" f32  lbl_eu_80668AD0;
extern "C" void func_8027EA6C(CSysWinScenarioLog* self);

// True when the pause flag is clear (game not frozen by a subwindow).
static inline bool isSceneActive() {
    return lbl_eu_80664772 == 0;
}

// Bump the scenario counter `id` by one (clamped to 0xFFFF) while the scene is
// active; returns the (possibly bumped) value for later gate checks.
static inline u32 scenarioBump(u32 id) {
    u32 v = func_80082694__Q22cf13CfGameManagerFv(id);
    u32 n = v;
    if (isSceneActive()) {
        n = v + 1;
        if (n >= 0xFFFF) {
            n = 0xFFFF;
        }
        func_8008269C__Q22cf13CfGameManagerFv(id, n);
    }
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
// the prototypes here - MWCC then emits a direct `bl` to the retail symbol
// instead of inlining the body at each call site (retail calls them out-of-line).
extern "C" u32 __declspec(noinline) func_8027EE88(u32 self, u32 arg);
extern "C" void __declspec(noinline) func_8027EEF4(u32 self);

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
// ---- Target 6: func_8027E9E8 (us-80280e6c) -------------------------------
// Allocate the CSysWinScenarioLog singleton from work memory, construct it,
// store it, and register it as a child of `parent`. Returns null when already
// created. (Regist is called even if the allocation failed, matching retail.)
// ---------------------------------------------------------------------------
CSysWinScenarioLog* func_8027E9E8(CProcess* parent, u8* param) {
    if (lbl_eu_80664908 != 0) {
        return nullptr;
    }
    void* mem = mtl::MemManager::allocate(0xA4, CWorkThreadSystem::getWorkMem());
    if (mem != nullptr) {
        __ct__CSysWinScenarioLog(mem, param);
    }
    lbl_eu_80664908 = (u32)mem;
    ((CSysWinScenarioLog*)mem)->Regist(parent, false);
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
    CBattleListNode* head =
        ((CBattleListOwner*)getInstance__Q22cf14CBattleManagerFv())->list;
    s32 count = 0;
    for (CBattleListNode* p = head->next; p != head; p = p->next) {
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
        u32 cur = func_80082694__Q22cf13CfGameManagerFv(0xB);
        if (isSceneActive()) {
            u32 bumped = cur + 1;
            if (bumped >= 0xFFFF) {
                bumped = 0xFFFF;
            }
            func_8008269C__Q22cf13CfGameManagerFv(0xB, bumped);
            cur = bumped;
        }
        if (cur >= 0x64) {
            bool booting = func_800822F4__Q22cf13CfGameManagerFv() <= 3;
            if (!booting && isSceneActive()) {
                func_800826F0__Q22cf13CfGameManagerFv(0xB);
            }
        }
    }
}

// ---------------------------------------------------------------------------
// Remaining unmatched func_8027* stubs in this unit (non-target).
// ---------------------------------------------------------------------------
extern "C" void cbRenderBefore__18CSysWinScenarioLogFv(void* self);

void func_8027EE70(void* self) { ((void(*)(void*))__dt__18CSysWinScenarioLogFv)((char*)self - 0x6c); }
void func_8027EE78(void* self) { ((void(*)(void*))cbRenderBefore__18CSysWinScenarioLogFv)((char*)self - 0x70); }
void func_8027EE80(void* self) { ((void(*)(void*))__dt__18CSysWinScenarioLogFv)((char*)self - 0x70); }

void func_8027EF50() {
    if (lbl_eu_80664910 != 0) {
        // Walk the battle-manager ring until we're back at the sentinel and
        // count how many active battle entries there are.
        CSysWinBattleMgrView* mgr =
            (CSysWinBattleMgrView*)getInstance__Q22cf14CBattleManagerFv();
        CSysWinListNode* sentinel = mgr->mListSentinel;
        s32 count = 0;
        CSysWinListNode* node = sentinel->next;
        while (node != sentinel) {
            node = node->next;
            count++;
        }
        if (count == 0) {
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
// ---- Target 6: CSysWinScenarioLog::Move (us-80280c54) ---------------------
// Advance the scenario-log window's opening/closing state machine.
// ---------------------------------------------------------------------------
void CSysWinScenarioLog::Move() {
    // Gate: not in a modal task-game state, pause bit clear, event byte set,
    // no active camera event, and the layout is loaded.
    if (CTaskGame::getInstance()->func_800426F0() == false) {
        if (lbl_eu_80663E28 & 0x200000) {
            // pause/scenario bit set: skip the whole update
        } else if (code80135FDC_getByte_621F0() != 0 &&
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

void func_8027F148(){}

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
    u16 h = self->field_0x3F28;
    int base = (h == 0x8) ? 3 : h;
    if (base == 1) {
        CScenarioLogSub* p = (CScenarioLogSub*)self->field_0x3F60;
        if (p != 0 && p->field_0x8->field_0x18 == 7) {
            return;
        }
    }
    if (base == 5) {
        CScenarioLogSub* p = (CScenarioLogSub*)self->field_0x3F60;
        if (p != 0) {
            u32 val = p->field_0x8->field_0x18;
            if (val >= 7 && val <= 0xE) {
                return;
            }
        }
    }
    // Bump 0x3d then close the 0x3d / 0x3e gates past their thresholds.
    u32 v = func_80082694__Q22cf13CfGameManagerFv(0x3d);
    if (isSceneActive()) {
        v = v + 1;
        if (v >= 0xFFFF) {
            v = 0xFFFF;
        }
        func_8008269C__Q22cf13CfGameManagerFv(0x3d, v);
    }
    if (v >= 0x3E8) {
        bool booting = func_800822F4__Q22cf13CfGameManagerFv() <= 3;
        if (!booting && isSceneActive()) {
            func_800826F0__Q22cf13CfGameManagerFv(0x3d);
        }
    }
    if (v >= 0x2710) {
        bool booting = func_800822F4__Q22cf13CfGameManagerFv() <= 3;
        if (!booting && isSceneActive()) {
            func_800826F0__Q22cf13CfGameManagerFv(0x3e);
        }
    }
    // Player check: continue with the ladder only for the first player slot or
    // when force-flagged.
    const u8* p0 = self ? &self->field_0x3E9C : nullptr;
    if (p0 == (const u8*)cf::CfGameManager::getPlayer(0) ||
        (self->field_0x3E6C & 0x1000)) {
        int base3 = (h == 0x8) ? 3 : h;
        if (base3 == 1) {
            scenarioLadder(0x3F);
        }
        if (base3 == 2) {
            scenarioLadder(0x40);
        }
        if (base3 == 3) {
            scenarioLadder(0x41);
        }
        if (base3 == 4) {
            scenarioLadder(0x42);
        }
        if (base3 == 5) {
            scenarioLadder(0x43);
        }
        if (base3 == 6) {
            scenarioLadder(0x44);
        }
        if (base3 == 7) {
            scenarioLadder(0x45);
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
            bool booting = func_800822F4__Q22cf13CfGameManagerFv() <= 3;
            if (!booting && isSceneActive()) {
                func_800826F0__Q22cf13CfGameManagerFv(0x1);
            }
        }
        if (arg1 >= 0x61A8) {
            bool booting = func_800822F4__Q22cf13CfGameManagerFv() <= 3;
            if (!booting && isSceneActive()) {
                func_800826F0__Q22cf13CfGameManagerFv(0x2);
            }
        }
        if (arg1 >= 0x10000 - 0x3CB0) {
            bool booting = func_800822F4__Q22cf13CfGameManagerFv() <= 3;
            if (!booting && isSceneActive()) {
                func_800826F0__Q22cf13CfGameManagerFv(0x3);
            }
        }
        if (arg1 == 0x29A) {
            bool booting = func_800822F4__Q22cf13CfGameManagerFv() <= 3;
            if (!booting && isSceneActive()) {
                func_800826F0__Q22cf13CfGameManagerFv(0x5F);
            }
        }
        if (arg1 == 0x309) {
            bool booting = func_800822F4__Q22cf13CfGameManagerFv() <= 3;
            if (!booting && isSceneActive()) {
                func_800826F0__Q22cf13CfGameManagerFv(0x60);
            }
        }
        if (arg2->field_0x74 & 0x100) {
            u32 v = scenarioBump(0x26);
            if (v >= 0x32) {
                scenarioClose(0x26);
            }
            if (v >= 0x5DC) {
                scenarioClose(0x27);
            }
            if (v >= 0x1388) {
                scenarioClose(0x28);
            }
        }

        // Player check: enter the 0x38 ladder when the embedded player matches
        // the first player slot, or the forced-player flag is set.
        const u8* p0 = arg0 ? &arg0->field_0x3E9C : nullptr;
        if (p0 == (const u8*)cf::CfGameManager::getPlayer(0) ||
            (arg0->field_0x3E6C & 0x1000)) {
            if (arg2->field_0x78 & 0x2) {
                u32 w = scenarioBump(0x38);
                if (w >= 1) {
                    scenarioClose(0x38);
                }
                if (w >= 0x32) {
                    scenarioClose(0x39);
                }
                if (lbl_eu_80664914 <= 0) {
                    lbl_eu_80664912 = 1;
                }
            }
        }

        // Final player/arg1 check then bump the open-window counter.
        const u8* q0 = arg0 ? &arg0->field_0x3E9C : nullptr;
        if (q0 == (const u8*)cf::CfGameManager::getPlayer(0)) {
            if (arg1 > 0) {
                lbl_eu_80664911 = 1;
            }
        }
        lbl_eu_80664914 += 1;
    }
}

void func_8027FC04(){}

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
void func_8027FC80(CSysWinDevice* arg0, void* arg1) {
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
void func_802804F8(){}
void func_80280588(){}
void func_80280640(){}
void func_802807A0(){}
void func_802808AC(s32 self) {
    if (self < 4) {
        return;
    }
    u32 cur = func_80082694__Q22cf13CfGameManagerFv(0x2C);
    if (isSceneActive()) {
        cur = cur + 1;
        if (cur >= 0xFFFF) {
            cur = 0xFFFF;
        }
        func_8008269C__Q22cf13CfGameManagerFv(0x2C, cur);
    }
    if (cur >= 1) {
        bool booting = func_800822F4__Q22cf13CfGameManagerFv() <= 3;
        if (!booting && isSceneActive()) {
            func_800826F0__Q22cf13CfGameManagerFv(0x2C);
        }
    }
    if (cur >= 0x32) {
        bool booting = func_800822F4__Q22cf13CfGameManagerFv() <= 3;
        if (!booting && isSceneActive()) {
            func_800826F0__Q22cf13CfGameManagerFv(0x2D);
        }
    }
    if (cur >= 0xC8) {
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
    if (isSceneActive()) {
        seq = seq + 1;
        if (seq >= 0xFFFF) {
            seq = 0xFFFF;
        }
        func_8008269C__Q22cf13CfGameManagerFv(0x23, seq);
    }
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
    u32 cur = func_80082694__Q22cf13CfGameManagerFv(0x29);
    if (isSceneActive()) {
        cur = cur + 1;
        if (cur >= 0xFFFF) {
            cur = 0xFFFF;
        }
        func_8008269C__Q22cf13CfGameManagerFv(0x29, cur);
    }
    if (cur >= 1) {
        bool booting = func_800822F4__Q22cf13CfGameManagerFv() <= 3;
        if (!booting && isSceneActive()) {
            func_800826F0__Q22cf13CfGameManagerFv(0x29);
        }
    }
    if (cur >= 0x64) {
        bool booting = func_800822F4__Q22cf13CfGameManagerFv() <= 3;
        if (!booting && isSceneActive()) {
            func_800826F0__Q22cf13CfGameManagerFv(0x2A);
        }
    }
    if (cur >= 0x3E8) {
        bool booting = func_800822F4__Q22cf13CfGameManagerFv() <= 3;
        if (!booting && isSceneActive()) {
            func_800826F0__Q22cf13CfGameManagerFv(0x2B);
        }
    }
}

void func_80280BF0() {
    u32 cur = func_80082694__Q22cf13CfGameManagerFv(0x5A);
    if (isSceneActive()) {
        cur = cur + 1;
        if (cur >= 0xFFFF) {
            cur = 0xFFFF;
        }
        func_8008269C__Q22cf13CfGameManagerFv(0x5A, cur);
    }
    if (cur >= 1) {
        bool booting = func_800822F4__Q22cf13CfGameManagerFv() <= 3;
        if (!booting && isSceneActive()) {
            func_800826F0__Q22cf13CfGameManagerFv(0x5A);
        }
    }
    if (cur >= 0x12C) {
        bool booting = func_800822F4__Q22cf13CfGameManagerFv() <= 3;
        if (!booting && isSceneActive()) {
            func_800826F0__Q22cf13CfGameManagerFv(0x5B);
        }
    }
    if (cur >= 0x7D0) {
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
    u32 count = 0;
    for (int o = 0; o < 2; o++) {
        u8* p1 = self;
        for (int m = 0; m < 2; m++) {
            u8* p2 = p1;
            for (int i = 0; i < 8; i++) {
                u8 val = p2[0];
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
        self += 0x49;
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

void func_80280F44(){}

extern "C" void func_802811FC(void* self){
    *(u32*)((u8*)self + 0x1D80) = 0;
    *(u32*)((u8*)self + 0x1D84) = 0;
}

void func_8028120C(){}

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
    if (isSceneActive()) {
        u32 sum = func_80082694__Q22cf13CfGameManagerFv(self) + arg;
        if (sum >= 0xFFFF) {
            sum = 0xFFFF;
        }
        func_8008269C__Q22cf13CfGameManagerFv(self, sum);
        arg = sum;
    }
    return arg;
}

extern "C" void func_8027EC80() {}

extern "C" void Init__18CSysWinScenarioLogFv() {}
extern "C" void Term__18CSysWinScenarioLogFv() {}
