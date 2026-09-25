// Auto-scaffolded catalog TU for kyoshin/cf/CtrlPc
// Replace stubs with high-level C/C++ during decomp.
//
// NOTE: this file was rebuilt from session transcripts after an accidental
// clobber. Functions marked REBUILD-STUB are compilable placeholders whose
// original drafts must be re-derived; their bodies may regress prior
// non-target matches.

#include "kyoshin/harness_catalog.hpp"

#include "kyoshin/cf/CtrlPc.hpp"
#include "kyoshin/cf/CArtsSet.hpp"
#include "kyoshin/cf/code_800F42AC.hpp"
#include "monolib/core/CPadManager.hpp"

// Retail __ct__cf_CtrlPad (0x80097274): base-constructs CtrlRemote (passing
// posObj through, arg5 = 0), stores the CtrlPad vtable manually (novtable),
// caches the current pad, then copies the 30-entry pad config table via
// setPadConfigEntry.
cf::CtrlPad* __ct__cf_CtrlPad(cf::CtrlPad* obj, void* posObj) {
    // CtrlPad's retail layout is CtrlRemote's (retail ctor chain
    // __ct__cf_CtrlPad -> __ct__CtrlRemote -> __ct__cf_CtrlPc).
    __ct__CtrlRemote(reinterpret_cast<void*>(obj), posObj, 0);
    *(void**)obj = (void*)lbl_eu_80527C80;
    reinterpret_cast<cf::CtrlPc*>(obj)->mField1D4 =
        (u32)cf::CfGameManager::getCurrentPad();
    u32* tbl;
    int i;
    for (tbl = lbl_eu_80527C08, i = 0; i < 0x1E; i++, tbl++) {
        setPadConfigEntry(i, *tbl);
    }
    return obj;
}

cf::CtrlRemote::~CtrlRemote() {
    // Destroy the CtrlPc sub-object with a direct call (retail: li r4,0;
    // bl __dt__Q22cf6CtrlPcFv). Extern "C" name prevents MWCC inlining the
    // in-TU ~CtrlPc stub body.
    __dt__Q22cf6CtrlPcFv(reinterpret_cast<cf::CtrlPc*>(this), 0);
}

// Retail func_80096974 (0x8009734C): player auto-pad/aiming handler.
// Routes the pad state through the sub-objects, applies target-lock flags,
// and computes the facing angle toward the locked target.
void func_80096974(cf::CtrlPc* self) {
    u32 v1, v2, v3, v4;
    u32 mask;
    int lock;
    int flag28;
    int flag27;
    int flag26;
    UnkClass_800821F8View* gmSub;
    CtrlVoiceHandle* vh;
    CtrlPlayerObj* p;

    cf::CfGameManager::getInstance();
    gmSub = (UnkClass_800821F8View*)getCameraDataBlock__Q22cf13CfGameManagerFv();
    CfObj90E4* g = Selector_GetInstance();
    vh = (CtrlVoiceHandle*)(void*)findObjectById__Fi((int)g->mField90E4);
    p = self->mField5C;
    v1 = *p->mField4->vf30();
    if (func_80174C98(p, &v1, 1) != 0) {
        return;
    }
    p = self->mField5C;
    v2 = *p->mField4->vf30();
    if (func_80174C98(p, &v2, 2) != 0) {
        return;
    }
    lock = 0;
    flag28 = 0;
    flag27 = 0;
    flag26 = 0;
    self->mField5C->mSub3ED4->vf10(0x400, 1);
    mask = getPadConfigEntry(0x12);
    if ((self->vf37()->mField0 & mask) != 0) {
        if (vh == 0) {
            flag28 = 1;
        } else if ((self->vf37()->mField0 & 3) == 0) {
            flag28 = 1;
        } else if ((self->vf37()->mField14 & getPadConfigEntry(0x12)) != 0) {
            lock = 1;
        }
    } else if ((gmSub->mField4 & 0x10) != 0) {
        flag28 = 1;
    }
    // Clear the lock-on request when its mask bit is not set.
    if ((self->vf37()->mField0 & getPadConfigEntry(0x10)) == 0) {
        self->mField5C->mSub3ED4->vf11(0x2000);
    }
    if (flag28 != 0) {
        self->mField5C->mSub3ED4->vf10(0x2000, 1);
        if ((gmSub->mField4 & 0x10) != 0) {
            cfCam_andcUnk04(gmSub, 0x10);
            if (self->mField5C->mSub3ED4->vf14(0x400) == 0) {
                flag26 = 1;
            }
        }
    }
    if (lock != 0) {
        if ((gmSub->mField4 & 0x10) == 0) {
            if (self->mField5C->mSub3ED4->vf14(0x2000) == 0) {
                cfCam_setClear04(gmSub, 0x10, 1);
                flag27 = 1;
            }
        }
    }
    if ((gmSub->mField4 & 0x10) != 0) {
        gmSub->vf09(vh->vf41());
    }
    if ((self->vf37()->mField10 & getPadConfigEntry(0x13)) != 0) {
        cfCam_setClear04(gmSub, 0x20, 1);
    }
    cfCam_setClear04(gmSub, 3, self->mField5C->mSub3ED4->vf14(0x400) == 0);
    if ((self->vf37()->mField4 & getPadConfigEntry(0x14)) != 0) {
        v4 = self->mField5C->mSub3ED4->vf14(0x1000);
        if (v4 != 0) {
            flag26 = 1;
        }
        if (v4 == 0) {
            flag27 = 1;
        }
    }
    // Facing-angle sync: aim the player at the active voice/talk source.
    if (self->mField5C->mSub3ED4->vf14(0x1000) != 0) {
        if (vh != 0) {
            CVoicePos* p1 = self->mField5C->mSub3E9C.getPosition();
            CVoicePos* p2 = (CVoicePos*)vh->vf41();
            ml::CVec3 diff;
            diff.x = p2->f[0] - p1->f[0];
            diff.y = p2->f[1] - p1->f[1];
            diff.z = p2->f[2] - p1->f[2];
            ml::CVec3 d = diff;
            self->mField10 = lbl_eu_80666718 * nw4r::math::Atan2FIdx(d.x, d.z);
        } else {
            flag26 = 1;
        }
    }
    p = self->mField5C;
    v3 = *p->mField4->vf30();
    if (func_80174C98(p, &v3, 3) != 0) {
        flag26 = 1;
        flag27 = 0;
    }
    if (flag27 != 0) {
        if (self->mField5C->mSub3ED4->vf14(0x1000) == 0) {
            self->mField5C->mSub3ED4->vf10(0x1000, 1);
        }
    }
    if (flag26 != 0) {
        if (self->mField5C->mSub3ED4->vf14(0x1000) != 0) {
            self->mField5C->mSub3ED4->vf11(0x1000);
        }
    }
}

// Retail func_80097134 (0x80097B0C): per-frame player pad-state maintenance.
// Probes actor-id flags via func_80174C98, feeds the battle manager, syncs
// the voice-owner interface and the target's flag word. Each probe result
// gets its own stack word (retail offsets 0x24 down to 0x08).
void func_80097134(cf::CtrlPc* self) {
    CtrlPlayerObj* obj;
    int ok4;
    CtrlPlayerObj* pl;
    u32 v2, v1, v3, v4, v5, v6, v7, v8, v9;
    int dispatched;

    self->mField4 = 0;
    obj = (CtrlPlayerObj*)getCfObjectPc(cf::CfGameManager::getPlayer(0));
    // Global gate: either cutscene-ish bit set -> skip everything but the tail.
    if (((lbl_eu_80663E24 & 0x02000000) | (lbl_eu_80663E24 & 0x400)) != 0) {
        return;
    }
    dispatched = 0;
    pl = self->mField5C;
    if (pl == 0) {
        goto probe100;
    }
    // Both probes must fail to pass (retail ORs the two results; the first
    // result stays live across the second fetch).
    v1 = *pl->mField4->vf30();
    ok4 = func_80174C98(pl, &v1, 4);
    pl = self->mField5C;
    v2 = *pl->mField4->vf30();
    if ((ok4 | func_80174C98(pl, &v2, 3)) == 0) {
        goto probe100;
    }
    pl = self->mField5C;
    v3 = *pl->mField4->vf30();
    if (func_80174C98(pl, &v3, 0x4000) != 0 ||
        func_80148778(&self->mField5C->mField8, 6) != 0 ||
        func_80148778(&self->mField5C->mField8, 0xCD) != 0) {
        goto probe100;
    }
    v4 = *obj->mField4->vf30();
    if (func_80174C98(obj, &v4, 0x803) == 0) {
        goto probe100;
    }
    v5 = *obj->mField4->vf30();
    if (func_80174C98(obj, &v5, 0x800000) != 0) {
        goto probe100;
    }
    if (CBattleMan_ListHasValue(getInstance__Q22cf14CBattleManagerFv(), obj) == 0) {
        goto probe100;
    }
    self->mField5C->mSub3E9C.v00(0x100);

probe100:
    if (self->mField5C->mSub3E9C.v01(0x100) != 0) {
        v6 = *obj->mField4->vf30();
        if (func_80174C98(obj, &v6, 0x803) != 0) {
            v7 = *obj->mField4->vf30();
            if (func_80174C98(obj, &v7, 0x1F) == 0 && obj->vf173() == 0) {
                self->mField4 |= 4;
            }
        }
    }

    pl = self->mField5C;
    if (pl->mField3F60 != 0) {
        v8 = *pl->mField4->vf30();
        if (func_80174C98(pl, &v8, 0x1A) != 0) {
            // Lock-on target: snapshot its y position once the flag is set.
            CtrlPlayerSub3F60* t = pl->mField3F60;
            if ((t->mField4EC & 0x10) == 0) {
                t->mField4EC |= 0x10;
                CVoicePos* pos = pl->mSub3E9C.getPosition();
                t->mField510 = pos->f[1];
            }
        } else {
            cf::CfGameManager::getInstance();
            if (isGlobalCamFlagSet__Fi(0x400000) == 0) {
                // Clear the lock-on bit when both target words agree it is
                // stale (rlwinm keep-range with wraparound == clearing one
                // bit).
                CtrlPlayerSub3F60* t2 = pl->mField3F60;
                u32 f = t2->mField4EC;
                if ((f & 0x10) != 0 &&
                    ((t2->mFieldC & 2) != 0 || (f & 2) != 0)) {
                    t2->mField4EC = f & ~0x10u;
                }
            }
        }
    }

    pl = self->mField5C;
    if (pl != 0) {
        v9 = *pl->mField4->vf30();
        if (func_80174C98(pl, &v9, 0x803) != 0) {
            self->vf33();
            self->vf20();
            self->vf34();
            self->vf21();
            dispatched = 1;
            goto tail;
        }
    }
    movePcTopTickDispatch((cf::CCtrlMovePC*)(void*)self->mSubObj8C);
    self->mField10 = self->mFieldC;
    if (func_80148778(&self->mField5C->mField8, 6) != 0 ||
        (cf::CfGameManager::getInstance(), isGlobalCamFlagSet__Fi(0x400000) != 0)) {
        self->mField14 = lbl_eu_80666720;
    }
tail:
    self->vf35();
    self->vf36();
    if (dispatched != 0) {
        func_8019A9C4((cf::CCtrlMovePC*)(void*)self->mSubObj8C);
    }
}

// Retail dispatchCtrlPcActions (0x80097F70): pad action dispatch for the player.
// Validates the pad-state, installs AI actions, and syncs the voice owner.
// Each actor-id probe result gets its own stack word (retail offsets
// 0x30 down to 0x08), so the temporaries are declared at function scope.
void dispatchCtrlPcActions(cf::CtrlPc* self) {
    u32 v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11;
    CtrlPlayerObj* p;
    CtrlPlayerObj* obj;

    cf::CfGameManager::getInstance();
    if (isGlobalCamFlagSet__Fi(0x1000000) != 0) {
        goto fail;
    }
    p = self->mField5C;
    v1 = *p->mField4->vf30();
    if (func_80174C98(p, &v1, 1) != 0) goto fail;
    p = self->mField5C;
    v2 = *p->mField4->vf30();
    if (func_80174C98(p, &v2, 8) != 0) goto fail;
    p = self->mField5C;
    v3 = *p->mField4->vf30();
    if (func_80174C98(p, &v3, 0x4000) != 0) goto fail;
    p = self->mField5C;
    v4 = *p->mField4->vf30();
    if (func_80174C98(p, &v4, 0x1D) != 0) goto fail;
    if (func_80148778(&self->mField5C->mField8, 0xF) != 0 ||
        func_80148778(&self->mField5C->mField8, 9) != 0) {
    fail:
        self->mField14 = lbl_eu_80666720;
        return;
    }

    obj = (CtrlPlayerObj*)getCfObjectPc(cf::CfGameManager::getPlayer(0));
    if (self->mField5C->mSub3E9C.v01(0x200) != 0) {
        self->mField4 |= 8;
    } else if (self->mField5C->mSub3E9C.v01(0x100) != 0 &&
               self->mField5C->vf173() == 0) {
        self->mField4 |= 4;
    }
    if (self->mField5C->mSub3E9C.v17() == 0) {
        void* src = func_80098694(self);
        if (src != 0) {
            self->mField5C->mSub3E9C.v18(src);
        }
    }
    if (self->vf32() != 0) {
        return;
    }
    p = self->mField5C;
    v5 = *p->mField4->vf30();
    if (func_80174C98(p, &v5, 5) != 0) {
        self->mField14 = lbl_eu_80666720;
    } else {
        p = self->mField5C;
        v6 = *p->mField4->vf30();
        if (func_80174C98(p, &v6, 0x800) == 0) {
            CtrlPlayerObj* p3 = obj;
            v7 = *p3->mField4->vf30();
            if (func_80174C98(p3, &v7, 0x802) != 0) {
                self->mField5C->mSub3E9C.v00(0x200);
                return;
            }
            self->vf22();
        }
    }
    p = self->mField5C;
    v8 = *p->mField4->vf30();
    if (func_80174C98(p, &v8, 0x4000) != 0 ||
        func_80148778(&self->mField5C->mField8, 6) != 0 ||
        func_80148778(&self->mField5C->mField8, 0xCD) != 0) {
        self->mField14 = lbl_eu_80666720;
    }
    {
        // Any of the three actor-id probes set routes into the voice sync.
        p = self->mField5C;
        v9 = *p->mField4->vf30();
        if (func_80174C98(p, &v9, 3) == 0) {
            p = self->mField5C;
            v10 = *p->mField4->vf30();
            if (func_80174C98(p, &v10, 6) == 0) {
                p = self->mField5C;
                v11 = *p->mField4->vf30();
                if (func_80174C98(p, &v11, 0x806) == 0) {
                    return;
                }
            }
        }
    }
    if (self->mField5C->mSub3E9C.v01(4) == 0) {
        return;
    }
    self->mField14 = lbl_eu_80666720;
    self->mField5C->mSub3E9C.v47(self->mFieldC);
    CfObjectMove_setAnimModeArgs((u8*)&self->mField5C->mSub3E9C, 3, 0, -1, 1);
    self->mField5C->mSub3E9C.v02(4);
}

// Retail func_80097A5C (0x80098434): post-action pad handler. Gated on
// func_80097E00; routes menu-select state into func_80098194/func_800983B8.
void func_80097A5C(cf::CtrlPc* self) {
    u32 mask;
    ArtsSelStateViewPc* sel;
    if (func_80097E00(self) == 0) {
        return;
    }
    Fd44State* h = func_8017FD44();
    if (h != 0 && h->mFieldB8 == 4) {
        // Battle-menu open: gate the action on the pad bit mask.
        mask = getPadConfigEntry(0x16);
        if ((self->vf38()->mField4 & mask) == 0) {
            return;
        }
        int sw = func_8017FD4C(h);
        // Retail duplicates the whole attach sequence in each arm.
        if (sw == 2) {
            self->mField5C->vf2F8(0x19);
            func_800983B8(self, 5);
            void* hand = self->mField5C->mSub3E9C.v134(0x5f);
            if (hand != 0) {
                CVoiceOwnerIntfPc* tgt = (CVoiceOwnerIntfPc*)self->mField5C;
                if (self->mField5C != 0) {
                    tgt = &self->mField5C->mSub3E9C;
                }
                bindPartnerO_(hand, tgt, 0);
            }
        } else if (sw == 1) {
            func_800983B8(self, 5);
            void* hand = self->mField5C->mSub3E9C.v134(0x5f);
            if (hand != 0) {
                CVoiceOwnerIntfPc* tgt = (CVoiceOwnerIntfPc*)self->mField5C;
                if (self->mField5C != 0) {
                    tgt = &self->mField5C->mSub3E9C;
                }
                bindPartnerO_(hand, tgt, 0);
            }
        }
        return;
    }
    sel = CMenuArtsSelect_getSelectState();
    if (sel == 0) {
        return;
    }
    s8 b1 = sel->byte1;
    if (b1 == -1) {
        return;
    }
    s8 b0 = sel->byte0;
    if (b0 == 1) {
        func_80098194(self, b1, sel->byte2);
        return;
    }
    if (b0 != 0) {
        return;
    }
    // Combo-window guard: while the pad id is 4 and the combo counter is in
    // [7,10], the dispatch below is suppressed. Each probe re-issues the
    // slot-0x298 virtual (retail calls it three times).
    if (self->mField5C->mField3F28 == 4) {
        if (self->mField5C->vf164()->mField48 != 0) {
            if (self->mField5C->vf164()->mField48 >= 7) {
                if (self->mField5C->vf164()->mField48 <= 0xA) {
                    return;
                }
            }
        }
    }
    func_800983B8(self, b1);
}

// Retail func_80097C74 (0x8009864C): battle-state maintenance. Sets/clears
// bits of the player's target word (mField4EC) based on the battle-manager
// state, then sets/clears another bit depending on the actor-id / art queries
// via func_80174C98.
void func_80097C74(cf::CtrlPc* self) {
    CtrlPlayerObj* p;
    CtrlPlayerObj* obj;
    u32 v6, v7, v8, v9, v10, v11;
    if (self->mField5C->mField3F60 != 0) {
        u8 bs = ((CBattleManagerViewPc*)getInstance__Q22cf14CBattleManagerFv())->mField1AA;
        // Commuted second compare keeps retail's two cmplwi branches (avoids
        // subi/cmpli range-check fusion).
        int inBattle = (bs >= 1u && 0x18u >= bs) ? 1 : 0;
        if (inBattle != 0 ||
            ((CBattleManagerViewPc*)getInstance__Q22cf14CBattleManagerFv())->mField20C8 != 0) {
            self->mField5C->mField3F60->mField4EC |= 4;
        } else {
            // rlwinm keep-range with wraparound == clearing a single bit.
            self->mField5C->mField3F60->mField4EC &= ~4;
        }
    }
    obj = (CtrlPlayerObj*)getCfObjectPc(cf::CfGameManager::getPlayer(0));
    p = self->mField5C;
    v6 = *p->mField4->vf30();
    if (func_80174C98(p, &v6, 0x800) == 0) {
        v7 = *obj->mField4->vf30();
        if (func_80174C98(obj, &v7, 0x802) != 0) {
            self->mField5C->mSub3E9C.v00(0x200);
            return;
        }
        self->vf22();
    }
    p = self->mField5C;
    v8 = *p->mField4->vf30();
    if (func_80174C98(p, &v8, 0x4000) != 0 ||
        func_80148778(&self->mField5C->mField8, 6) != 0 ||
        func_80148778(&self->mField5C->mField8, 0xCD) != 0) {
        self->mField14 = lbl_eu_80666720;
    }
    // Any of the three actor-id probes set routes into the voice sync.
    p = self->mField5C;
    v9 = *p->mField4->vf30();
    if (func_80174C98(p, &v9, 3) == 0) {
        p = self->mField5C;
        v10 = *p->mField4->vf30();
        if (func_80174C98(p, &v10, 6) == 0) {
            p = self->mField5C;
            v11 = *p->mField4->vf30();
            if (func_80174C98(p, &v11, 0x806) == 0) {
                return;
            }
        }
    }
    if (self->mField5C->mSub3E9C.v01(4) == 0) {
        return;
    }
    self->mField14 = lbl_eu_80666720;
    self->mField5C->mSub3E9C.v47(self->mFieldC);
    CfObjectMove_setAnimModeArgs((u8*)&self->mField5C->mSub3E9C, 3, 0, -1, 1);
    self->mField5C->mSub3E9C.v02(4);
}

// Retail emptyCtrlPcHook (0x800987D4): 4-byte trivial function.
void emptyCtrlPcHook() {}

// Retail func_80097E00 (0x800987D8): idle/busy probe for the player pad
// handler. Returns 1 when the player is in a usable (non-busy) state, else 0.
extern "C" int func_80097E00(cf::CtrlPc* self) {
    CtrlPlayerObj* pl;
    u32 battleState = ((CBattleManagerViewPc*)getInstance__Q22cf14CBattleManagerFv())->mField1AA;
    u32 chainActive;
    int inBattle = 0;
    if (battleState >= 1 && 0x18 >= battleState) {
        inBattle = 1;
    }
    chainActive = (((CBattleManagerViewPc*)getInstance__Q22cf14CBattleManagerFv())->mField20C8 != 0);
    cf::CfGameManager::getInstance();
    if (isGlobalCamFlagSet__Fi(0x4000000) != 0) {
        return 0;
    }
    CVoiceOwnerIntfPc* owner = (CVoiceOwnerIntfPc*)self->mField5C;
    if (self->mField5C != 0) {
        owner = &self->mField5C->mSub3E9C;
    }
    if ((void*)owner != (void*)cf::CfGameManager::getPlayer(0)) {
        return 0;
    }
    if (CMainMenu_GetInstancePtr() != 0) {
        return 0;
    }
    if (self->mField5C->mSub3ED4->vf14(0x400) != 0 && inBattle == 0 &&
        chainActive == 0) {
        return 0;
    }
    if (self->mField5C->mSub3ED4->vf14(0x40000) != 0) {
        return 0;
    }

    // All actor-id flags must be clear for the player to be idle; each probe
    // uses its own stack word (retail keeps them at distinct offsets).
    pl = self->mField5C;
    { u32 v = *pl->mField4->vf30(); if (func_80174C98(pl, &v, 3) != 0) goto busy; }
    pl = self->mField5C;
    { u32 v = *pl->mField4->vf30(); if (func_80174C98(pl, &v, 0xA) != 0) goto busy; }
    pl = self->mField5C;
    { u32 v = *pl->mField4->vf30(); if (func_80174C98(pl, &v, 0xB) != 0) goto busy; }
    pl = self->mField5C;
    { u32 v = *pl->mField4->vf30(); if (func_80174C98(pl, &v, 0x806) != 0) goto busy; }
    return 1;
busy:
    return 0;
}

// REBUILD-STUB: original draft lost; restore from history.
extern "C" void* func_80098694(cf::CtrlPc* self);
__declspec(noinline) void* func_80098694(cf::CtrlPc* self) { return 0; }

// Retail func_80098194 (0x80098B6C): install an AI-action slot built from the
// player's arts-set slot and the enum-list selection, then sync the voice
// owner handle.
// REBUILD-STUB: only the prologue survived the accidental clobber; the body
// below is a compile-correct reconstruction and must be re-derived from
// retail ASM before any matching attempt.
void func_80098194(cf::CtrlPc* self, char arg1, char arg2) {
    CVisionFxParam slot;
    std::memset((u8*)&slot + 4, 0, 0xE);
    std::memset(&slot, 0, sizeof(slot));

    // Arts slot index: base halfword from the CArtsSet holder (slot 0x27C),
    // scaled by 8, offset by the selected entry + 0x11.
    u16 artsBase = *(u16*)self->mField5C->vf157();
    int scaled8 = ((int)(s16)artsBase << 3) + 0x11;
    int slotIdx = arg1 + scaled8;

    // Pad mask depends on whether a chain/counter state is active.
    u32 mask = ((CBattleManagerViewPc*)getInstance__Q22cf14CBattleManagerFv())
                   ->mField20C8 != 0
                   ? 0x1800
                   : 0x804;
    slot.w00 = (u32)slotIdx;
    slot.b_06 = 0x25;
    slot.b_0D = (u8)arg2;
    slot.b_0E = 0x64;
    slot.h_10 = (u16)mask;
    slot.f_14 = lbl_eu_80666720;
    func_8014AC38(self->mField5C, &slot);
    void* hand = self->mField5C->mSub3E9C.v17();
    if (hand == 0) {
        void* src = func_80098694(self);
        if (src != 0) {
            self->mField5C->mSub3E9C.v18(src);
        }
    }
}

// ---- H5W6 name recovery: pad-flag bit testers (retail 80096EE8..80096F9C).
// Each isolates one bit of the +0x2C pad-flags word: retail
// `extrwi r3,r0,1,N` for N=8..23, i.e. LSB (31-N). CtrlPad vtable slots;
// named after the CtrlRemote_* siblings in the same table
// (CtrlRemote_TestFlag800, CtrlRemote_Check800Cleared1000, ...).
// Proven shape pre-clobber (attempts.jsonl): bit extract (x>>K)&1 from +0x2C.
extern "C" u32 CtrlPad_TestBit08(cf::CtrlPc* self) { return (self->mPadFlags >> 23) & 1; }
extern "C" u32 CtrlPad_TestBit09(cf::CtrlPc* self) { return (self->mPadFlags >> 22) & 1; }
extern "C" u32 CtrlPad_TestBit10(cf::CtrlPc* self) { return (self->mPadFlags >> 21) & 1; }
extern "C" u32 CtrlPad_TestBit11(cf::CtrlPc* self) { return (self->mPadFlags >> 20) & 1; }
extern "C" u32 CtrlPad_TestBit12(cf::CtrlPc* self) { return (self->mPadFlags >> 19) & 1; }
extern "C" u32 CtrlPad_TestBit13(cf::CtrlPc* self) { return (self->mPadFlags >> 18) & 1; }
extern "C" u32 CtrlPad_TestBit14(cf::CtrlPc* self) { return (self->mPadFlags >> 17) & 1; }
extern "C" u32 CtrlPad_TestBit15(cf::CtrlPc* self) { return (self->mPadFlags >> 16) & 1; }
extern "C" u32 CtrlPad_TestBit16(cf::CtrlPc* self) { return (self->mPadFlags >> 15) & 1; }
extern "C" u32 CtrlPad_TestBit17(cf::CtrlPc* self) { return (self->mPadFlags >> 14) & 1; }
extern "C" u32 CtrlPad_TestBit18(cf::CtrlPc* self) { return (self->mPadFlags >> 13) & 1; }
extern "C" u32 CtrlPad_TestBit19(cf::CtrlPc* self) { return (self->mPadFlags >> 12) & 1; }
extern "C" u32 CtrlPad_TestBit20(cf::CtrlPc* self) { return (self->mPadFlags >> 11) & 1; }
extern "C" u32 CtrlPad_TestBit21(cf::CtrlPc* self) { return (self->mPadFlags >> 10) & 1; }
extern "C" u32 CtrlPad_TestBit22(cf::CtrlPc* self) { return (self->mPadFlags >> 9) & 1; }
extern "C" u32 CtrlPad_TestBit23(cf::CtrlPc* self) { return (self->mPadFlags >> 8) & 1; }

// Walker field-0x380 setter (retail Walker_SetField380): `stw r4,0x380(r3)`.
// CfObjectImplPc/CfObjectImplNpc vtable slot next to Walker_GetField380
// (same +0x380 word); byte-offset idiom mirrors that getter.
extern "C" void Walker_SetField380(void* self, u32 value) {
    *(u32*)((u8*)self + 0x380) = value;
}

// Move-base query dispatcher (retail CtrlPad_QueryMoveBase): with the flag word set,
// clear the caller's flag byte and run func_80089398 on the embedded
// CCtrlMovePC (+0x8C) reporting success; otherwise tail the func_800890A8
// query through on the same subobject and return its result.
extern "C" int CtrlPad_QueryMoveBase(cf::CtrlPc* self, ml::CVec3* dst, u8* flagOut,
                                       const ml::CVec3* src, int flag, int usePrimary) {
    if (usePrimary != 0) {
        *flagOut = 0;
        func_80089398((cf::CCtrlMoveBase*)&self->mSubObj8C, dst, src, flag);
        return 1;
    }
    return func_800890A8((cf::CCtrlMoveBase*)&self->mSubObj8C, dst, flagOut, src, flag);
}

// Float-fed move delegate (retail CtrlPad_Delegate899C0): loads the sdata2 constant
// into f1 and tail-calls func_800899C0 on the embedded CCtrlMovePC (+0x8C).
// (Pre-clobber stub family name: delegateTo899C0.)
extern const f32 lbl_eu_80666728;
extern "C" void func_800899C0(void* self, float value);
extern "C" int func_80089E88();
extern "C" void CtrlPad_Delegate899C0(cf::CtrlPc* self) {
    float c = lbl_eu_80666728;
    typedef void (*Fn)(void*, float);
    ((Fn)func_800899C0)((u8*)self + 0x8c, c);
}

// Move-base tail-call delegates (retail CtrlPad_Delegate89B24 / CtrlPad_Delegate89E88 /
// CtrlPad_Delegate89F68): re-base `this` onto the embedded CCtrlMovePC (+0x8C) and
// tail-jump to the matching handler, passing the incoming registers through.
// (Pre-clobber stub family names: delegateTo89E88 / delegateTo89F68.)
extern "C" int CtrlPad_Delegate89B24(cf::CtrlPc* self, void* out) {
    typedef int (*Fn)(void*, void*);
    return ((Fn)func_80089B24)((u8*)self + 0x8c, out);
}
extern "C" int CtrlPad_Delegate89E88(cf::CtrlPc* self, void* out) {
    typedef int (*Fn)(void*, void*);
    return ((Fn)func_80089E88)((u8*)self + 0x8c, out);
}
extern "C" int CtrlPad_Delegate89F68(cf::CtrlPc* self, void* out) {
    typedef int (*Fn)(void*, void*);
    return ((Fn)func_80089F68)((u8*)self + 0x8c, out);
}

// Cached-pad-handle getter (retail CtrlPad_GetField1D4): the +0x1D4 word
// (__ct__cf_CtrlPad caches CfGameManager::getCurrentPad() there).
extern "C" u32 CtrlPad_GetField1D4(cf::CtrlPc* self) { return self->mField1D4; }

// Constant-1 vtable slot (retail CtrlPad_ReturnTrue): `li r3,1; blr`.
// (Pre-clobber stub guess: returnTrue.)
extern "C" int CtrlPad_ReturnTrue() { return 1; }

// Retail func_80098A04: writes the tri-state pad state (-1/0/1) into
// mField24.
// REBUILD-STUB: body lost in the accidental clobber; re-derive from retail
// ASM (uses isGlobalCamFlagSet__Fi(0x10000000) gate).
void func_80098A04(cf::CtrlPc* self) {
    if (isGlobalCamFlagSet__Fi(0x10000000) != 0) {
        self->mField24 = -1;
        return;
    }
    self->mField24 = 0;
}

// Pad-config mask-table lookup (retail getPadConfigEntry): select the
// classic-controller table (lbl_eu_80527F10) when a classic controller is
// attached, else the standard table (lbl_eu_80527E98), and return entry
// `index`. All pad-handler mask queries in this TU go through here.
u32 getPadConfigEntry(int index) {
    // Index stays in a saved register across the controller probe (retail
    // keeps it in r31); each arm returns straight off its own table base
    // (retail beq-first: non-classic reads lbl_eu_80527E98).
    u32 idx = (u32)index;
    if (isClassicController__Q22cf13CfGameManagerFv(-1) != 0) {
        return lbl_eu_80527F10[idx];
    }
    return lbl_eu_80527E98[idx];
}

// Pad-config table writer (retail setPadConfigEntry): store `value` at entry
// `index` of the standard table. Used by __ct__cf_CtrlPad to copy the 30
// pad-config entries into place.
void setPadConfigEntry(int index, u32 value) {
    lbl_eu_80527E98[index] = value;
}

void cf::CAttackParam::CAttackParam_UnkVirtualFunc3(u8 val) { unk2A = val; }
