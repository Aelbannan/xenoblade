// Auto-scaffolded catalog TU for kyoshin/CSysWinScenarioLog

#include <types.h>

#include "kyoshin/harness_catalog.hpp"
#include "kyoshin/CSysWinScenarioLog.hpp"
#include "monolib/util/MemManager.hpp"
#include "monolib/work/CWorkThreadSystem.hpp"
#include "kyoshin/cf/CfGameManager.hpp"

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
extern "C" u16  lbl_eu_80664772;          // pause / non-enemy-scene flag
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

// True when the pause flag is clear (game not frozen by a subwindow).
static inline bool isSceneActive() {
    return lbl_eu_80664772 == 0;
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

// ---------------------------------------------------------------------------
// ---- Target 2: func_8027EEF4 (us-80281378) -------------------------------
// Closes the sequence for `self` once the game is in a progression gate.
// ---------------------------------------------------------------------------
void func_8027EEF4(u32 self) {
    bool booting = func_800822F4__Q22cf13CfGameManagerFv() <= 3;
    if (!booting && lbl_eu_80664772 == 0) {
        func_800826F0__Q22cf13CfGameManagerFv(self);
    }
}

// ---------------------------------------------------------------------------
// ---- Target 4: func_8027EE88 (us-8028130c) -------------------------------
// Bump the cf sequence value keyed `self` by `arg` (clamped to 0xFFFF) and
// store it back; when the pause flag is set, leave it untouched.
// ---------------------------------------------------------------------------
u32 func_8027EE88(u32 self, u32 arg) {
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

void func_8027EF50(){}
void func_8027F148(){}
void func_8027F2DC(){}
void func_8027F848(){}
void func_8027FC04(){}
void func_8027FC80(){}
void func_802804F8(){}
void func_80280588(){}
void func_80280640(){}
void func_802807A0(){}
void func_802808AC(){}

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

void func_80280ADC(){}
void func_80280BF0(){}

// ---------------------------------------------------------------------------
// ---- Target 5: func_80280D04 (us-80283188) --------------------------------
// Progress the scenario-log sequence through its tiers. Each gate closes an
// earlier tier before later tiers can advance (1 / 100 / 1000, then, once
// self reaches 5, a nested 1 / 20 / 50 ladder).
// ---------------------------------------------------------------------------
void func_80280D04(u32 self) {
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
        u8* p1 = self + o * 0x49;
        for (int m = 0; m < 2; m++) {
            u8* p2 = p1 + m * 0x10;
            for (int i = 0; i < 8; i++) {
                u8 val = p2[i * 2];
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
            }
        }
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
// one at +0x24. Returns the run base when a whole run is free; otherwise
// closes the sequence for 0x4E once all segments have at least one taken slot.
// ---------------------------------------------------------------------------
u8* func_80280E9C(u8* self) {
    u8* p = self + 0x3D4;
    for (int s = 0; s < 2; s++) {
        for (int run = 0; run < 3; run++) {
            int i;
            for (i = 0; i < 5; i++) {
                if (*(u16*)(p + 0x20 * i + 0x24) != 0) {
                    break;
                }
            }
            if (i >= 5) {
                return p;
            }
            p += 0xC4;
        }
    }
    func_8027EEF4(0x4E);
    return p;
}

void func_80280F44(){}

extern "C" void func_802811FC(void* self){
    *(u32*)((u8*)self + 0x1D80) = 0;
    *(u32*)((u8*)self + 0x1D84) = 0;
}

void func_8028120C(){}