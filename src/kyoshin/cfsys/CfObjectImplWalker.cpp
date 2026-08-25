// Auto-scaffolded catalog TU for kyoshin/cfsys/CfObjectImplWalker
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/cf/CBattleManagerApi.hpp"
#include "kyoshin/cf/CfMapItemManager.hpp"
#include "kyoshin/harness_catalog.hpp"
#include "monolib/scn/CScnTimeApi.hpp"

#include "kyoshin/cfsys/CfObjectImplWalker.hpp"
#include "kyoshin/cf/object/CfObjectMoveApi.hpp" // func_800BE12C (owner decl)
#include <monolib/device/CDeviceVI.hpp>
#include <string.h>
#include "kyoshin/cf/CfGameManagerData.hpp"  // H3 label-owner decl (lbl_eu_80663E14; lbl_eu_80663E24)
namespace cf {
    void* CfObjectImplWalker::func_800C22C4() { return (void*)((u8*)this + 0x68); }
    u32 CfObjectImplWalker::func_800C5970() { return *(u32*)((u8*)this + 0x380); }
}

// Resets the walker's battle hook state: notifies the battle manager that
// the owning actor joins a battle, runs the PC-side init, then clears the
// walker's own status fields (six u32s, a byte and a float).
void func_800C1F44(cf::CfObjectImplWalker* self) {
    ((CfWalkBMView*)getInstance__Q22cf14CBattleManagerFv())->func_80085220(1, 1);
    func_800C5998(self);
    self->field_384 = 0;
    self->field_388 = 0;
    self->field_38C = 0;
    self->field_390 = 0;
    self->field_394 = 0;
    self->field_398 = 0;
    self->field_39C = 0;
    self->field_3A0 = lbl_eu_80666B84;
}

// Per-frame battle-object sync: pushes the move sub-object's current value
// through the battle hooks, then walks the presentation/event flags to arm
// the battle-start/stop notifications, syncs the walker state hooks and
// dispatches the battle/talk notifications before the per-frame area update.
// (self fields: field_04, field_14, field_18, field_28)
void func_800C1FB8(cf::CfObjectImplWalker* self) {
    if (self->field_18 != 0 &&
        self->field_18->mSub.m138()->field_0 != lbl_eu_80666B84) {
        self->field_18->mSub.m134(self->field_18->mSub.m138()->field_0);
        self->field_18->mSub.m1D4(
            self->field_18->mSub.m138()->field_0 * lbl_eu_80666B88);
    }
    if (self->field_04->vf74() != 0) {
        if (self->field_18 != 0) {
            self->vfDC();
            if (lbl_eu_80663E24 & 0x800000) {
                getInstance__Q22cf13CfGameManagerFv();
                if (func_8006EF04(0x1000000) == 0) {
                    self->field_18->mSub.m1AC(0, lbl_eu_804FC694);
                    func_80174B4C(self->field_18, 3);
                    lbl_eu_80663E24 &= ~0x800000;
                }
            }
            if (lbl_eu_80663E24 & 0x08000000) {
                getInstance__Q22cf13CfGameManagerFv();
                if (func_8006EF04(0x1000000) == 0) {
                    self->field_18->mSub.m1AC(0, lbl_eu_804FC694);
                    func_80174B4C(self->field_18, 3);
                    lbl_eu_80663E24 &= ~0x08000000;
                    func_80140E00(2, 0, 0);
                }
            }
            self->vfD4();
            func_800C2C90(self);
            self->vfD8();
            cf::CfWalkBattleSub* cast = (cf::CfWalkBattleSub*)__dynamic_cast(
                self->field_18->mSub.m110(), 0,
                (const void*)&lbl_eu_80661C08,
                (const void*)&lbl_eu_80661C10, 0);
            if (cast != 0) {
                if (cast->vf4C() != 0) self->vf9C();
                if (cast->vf50() != 0) self->vf100();
                if (cast->vf54() != 0) self->vf104();
                if (lbl_eu_80663E24 & 0x200000) func_800C525C(self);
            }
            func_800CC020(self);
        }
        if (self->field_14->field_90 != 0) {
            self->vfE4()->e10();
            func_802A0E08(self->field_28);
        }
    }
}

// Per-frame battle-state watcher for the walker's own battle object:
// gates on presentation flags and the player's battle binding, accumulates
// the out-of-battle timer against the target framerate, tracks the battle
// manager's current target record, and disarms the battle when the tracked
// distance decays past the threshold.
void func_800C22CC(cf::CfObjectImplWalker* self) {
    func_800C5B00();
    if (lbl_eu_80663E28 & 0x00100000) return;

    // Only run while the player has a battle object and the game state gate
    // (0x800) is clear with the battle flag bit set.
    void* bf = (void*)(uintptr_t)self->field_18->field_3F60;
    if (bf != 0 && func_8007FE24__Q22cf13CfGameManagerFv(0x800) == 0 &&
        (((cf::CfWalkBattleFlags*)bf)->field_4EC & 0x80)) {
        // If the bound battle object belongs to the player and area step
        // 0x82 has not been taken yet, take it now.
        void* player = getPlayer__Q22cf13CfGameManagerFi(0);
        void* playerObj = player ? (void*)((u8*)player - 0x3e9c) : (void*)0;
        if ((void*)self->field_18 == playerObj) {
            if (func_80082694__Q22cf13CfGameManagerFv(0x82) == 0) {
                func_8008269C__Q22cf13CfGameManagerFv(0x82, 1);
                func_800826F0__Q22cf13CfGameManagerFv(0x82);
            }
        }
        if (getInstance__Q22cf14CBattleManagerFv() != 0) {
            func_8018C8F4((u8*)getInstance__Q22cf14CBattleManagerFv() + 0x194,
                          0);
        }
        func_80084654__Q22cf13CfGameManagerFv(1);
    }

    // Move-value changed?
    if (self->field_18->vf12C() == lbl_eu_80666B84) return;

    cf::CfWalkBattleObj* bo = self->field_18;
    u32 v = bo->field_04->b30()->field_0;
    if (func_80174C98(self->field_18, &v, 0x1c) != 0) {
        getInstance__Q22cf13CfGameManagerFv();
        if (func_8006EF04(0x4000000) == 0) {
            // Accumulate the elapsed time; once it passes one target frame
            // window, disarm the battle.
            self->field_3A0 = self->field_3A0 + func_80496288(lbl_eu_80663E14);
            int frames = CDeviceVI::getTargetFramerate() * 0x3c;
            if ((f64)self->field_3A0 >= (f64)frames - 0x80000000ull + 0x80000000ull) {
                func_80084654__Q22cf13CfGameManagerFv(0);
                self->field_3A0 = lbl_eu_80666B84;
            }
        }

        cf::CfWalkEA444* rec =
            (cf::CfWalkEA444*)func_800EA444(getInstance__Q22cf14CBattleManagerFv());
        u32 id = self->field_18->mSub.field_74;
        if (!(func_8006EF04(0x4000000) != 0 && rec != 0 &&
              rec->field_4 != id && rec->field_0 != id)) {
            goto L30FC;
        }

        {
            f32 dist = *(f32*)bo->vf234();
            cf::CfWalkEnumHolder holder;
            func_80043D90(&holder);
            func_800F4A98(func_80043F18(&holder), 0x20, 0x800);
            if (bf != 0) {
                if (((cf::CfWalkPlayerRec*)bf)->vf80q(0) != 0) {
                    // Retail walks the party list here, accumulating the
                    // nearest hostile distance into the threshold compare.
                    for (u32 i = 0;
                         i < (u32)func_80043F18(&holder)->field_620; i++) {
                        void* obj = func_800F6E98(
                            func_80043F18(&holder), (int)i);
                        if (obj != 0) {
                            dist = dist + lbl_eu_80666B90;
                        }
                    }
                }
            }
            if (dist > lbl_eu_80666B90) {
                // Count live battles; only a non-empty chain with the
                // multi-battle flag keeps the fight armed.
                CfWalkBMView* bm =
                    (CfWalkBMView*)getInstance__Q22cf14CBattleManagerFv();
                u32 count = 0;
                CfWalkChainNode* head = bm->field_48;
                CfWalkChainNode* cur;
                for (cur = head->next; cur != head; cur = cur->next) {
                    count++;
                }
                if (count >= 0x64) {
                    if ((u32)func_80043F18(&holder)->field_620 != 0) {
                        count = 0;
                        head = bm->field_48;
                        for (cur = head->next; cur != head; cur = cur->next) {
                            count++;
                        }
                        if (count != 0 &&
                            !(self->field_18->field_3374 & 0x20)) {
                            func_80084654__Q22cf13CfGameManagerFv(0);
                        }
                    }
                } else {
                    func_80084654__Q22cf13CfGameManagerFv(0);
                }
                if (bf != 0 && (((cf::CfWalkBattleFlags*)bf)->field_4EC &
                                0x100)) {
                    func_80084654__Q22cf13CfGameManagerFv(0);
                }
            }
            __dt__80043E88(&holder, -1);
        }
    }
L30FC:
    // Re-run the 0x1c query; on a miss the timer resets.
    {
        cf::CfWalkBattleObj* bo2 = self->field_18;
        u32 v2 = bo2->field_04->b30()->field_0;
        if (func_80174C98(bo2, &v2, 0x1c) == 0) {
            self->field_3A0 = lbl_eu_80666B84;
        }
    }
}

// Battle-entry/exit sync for the walker: arms the battle-entry notification
// when a talk source is idle (or the global actor is missing), runs the
// target-query checks, then either starts a new battle (enumerating the
// fight-capable actors, arming the walker hooks and the move sub-object) or
// forwards the pending battle-exit to the move sub-object / walker hooks.
void func_800C2714(cf::CfObjectImplWalker* self) {
    u32 v1, v2, v3, v4, v5;
    if (lbl_eu_80663E24 & 0x5000000) return;
    if (func_80496288(lbl_eu_80663E14) == lbl_eu_80666B84) return;
    u32 gid = func_800FE68C()->field_90E4;
    u32 gid0 = gid;
    cf::CfWalkTalkSrc* actor =
        (cf::CfWalkTalkSrc*)func_800B708C((int)func_800FE68C()->field_90E4);
    if (gid0 == 0) goto L3284;
    if (actor == 0 || actor->t160() == 0) goto L320C;
    if (!(actor->field_64 & 8) || actor->t16C() == lbl_eu_80666B84) goto L3234;
L320C:
    self->vf70(0);
    func_80174B4C(self->field_18, 0x800000);
    goto L3294;
L3234:
    {
        cf::CfWalkTalkSrc* src = (cf::CfWalkTalkSrc*)func_800BBC0C(actor);
        if (src != 0 && src->t164() == 0) {
            self->vf70(0);
            func_80174B4C(self->field_18, 0x800000);
        }
    }
    goto L3294;
L3284:
    // Retail keeps a redundant `bne L3294` here (gid is always 0 on this
    // path) that MWCC constant-folds away; the fold only costs 4 bytes of
    // size, and every later reloc shifts by that same amount.
    if (gid != 0) goto L3294;
    func_80174B4C(self->field_18, 0x800000);
L3294:
    // Target-query gates: with no query hit the target is attackable and a
    // fresh battle can start; the middle block runs only when the walker's
    // own 0x802 query misses.
    {
        cf::CfWalkBattleObj* battleObj = self->field_18;
        v1 = battleObj->field_04->b30()->field_0;
        if (func_80174C98(battleObj, &v1, 0x802) != 0) goto L34E4;
        v2 = battleObj->field_04->b30()->field_0;
        if (func_80174C98(battleObj, &v2, 0x400000) != 0 && actor == 0) goto L3360;
        v3 = battleObj->field_04->b30()->field_0;
        if (func_80174C98(battleObj, &v3, 0x800000) != 0) goto L3360;
        if (actor == 0) goto L34E4;
        if (actor->field_64 & 0x4) goto L34E4;
        if (actor->field_64 & 0x2) goto L34E4;
    }
L3360:
    // New-battle path: enumerate fight-capable actors, feed the walker's
    // selector, then route the first hit to the battle-entry hook.
    {
        cf::CfWalkEnumHolder holder;
        func_80043D90(&holder);
        func_800F4A98(func_80043F18(&holder), 0x80000002, 0);
        void* v48 = self->vf48();
        func_800F6ED0(func_80043F18(&holder), v48);
        if (func_80043F18(&holder)->field_620 != 0) {
            void* item = func_800F6E98(func_80043F18(&holder), 0);
            if (func_800FE6A4(func_800FE68C(), 0x80000003, 0,
                              (u32)(uintptr_t)item) != 0) {
                self->vf70(func_800FE68C()->field_90E4);
            } else {
                self->field_18->mSub.m08(0x200);
            }
        } else {
            cf::CfWalkBattleObj* bo = self->field_18;
            v4 = bo->field_04->b30()->field_0;
            if (func_80174C98(bo, &v4, 0x806) != 0) {
                func_800BE12C(&self->field_18->mSub, 0x31, 0, -1, 1);
            } else {
                cf::CfWalkBattleObj* bo2 = self->field_18;
                v5 = bo2->field_04->b30()->field_0;
                if (func_80174C98(bo2, &v5, 0x803) != 0) {
                    self->field_18->mSub.m08(0x200);
                }
            }
        }
        __dt__80043E88(&holder, -1);
    }
L34E4:
    // Battle-exit forwarding: when the target actor differs from the move
    // sub-object's current target, bounce the notification to the selector
    // or the move sub-object depending on the battle-manager state.
    {
        if (func_800FE68C()->field_90E4 == 0) goto L3600;
        u32 gid2 = func_800FE68C()->field_90E4;
        if (gid2 == self->field_18->mSub.m4C()) goto end;
        if (func_80148778(&self->field_18->_pad08[0], 0x11) != 0) goto end;
        if (((cf::CfWalkField3ED4*)self->field_18->mSub.field_38)->vf40() == 0) {
            goto L35D8;
        }
        u8 b = ((CfWalkBMView*)getInstance__Q22cf14CBattleManagerFv())->field_1AA;
        bool ok = (b >= 1 && b <= 0x18);
        if (ok) {
            if (self->field_18->vf298()->field_4 != 0) goto L35D8;
        }
        if (((CfWalkBMView*)getInstance__Q22cf14CBattleManagerFv())->field_20C8 == 0) goto end;
        if (self->field_18->vf298()->field_4 == 0) goto end;
    }
L35D8:
    self->vf70(func_800FE68C()->field_90E4);
    goto end;
L3600:
    if (self->field_18->mSub.m4C() == 0) goto end;
    if (lbl_eu_80663E24 & 0x1000000) goto end;
    {
        u32 m = self->field_18->mSub.m4C();
        if (func_800FE6A4(func_800FE68C(), 0x80000003, 0, m) != 0) {
            if (((CfWalkBMView*)getInstance__Q22cf14CBattleManagerFv())->vf28(1) != 0) {
                self->vf80();
            }
        } else {
            self->vf70(0);
            func_80174B4C(self->field_18, 0x800000);
        }
    }
end:
    return;
}

// Per-frame battle-state sync: asks the move sub-object's battle sub-object
// (dynamic-cast) which battle flags changed and forwards the notifications
// to the selector global, plus the walker's own state hook.
void func_800C2C90(cf::CfObjectImplWalker* self) {
    cf::CfWalkBattleObj* battleObj = self->field_18;
    void* cast = __dynamic_cast(battleObj->mSub.m110(), 0,
                                (const void*)&lbl_eu_80661C08,
                                (const void*)&lbl_eu_80661C10, 0);
    if (func_80496288(lbl_eu_80663E14) != lbl_eu_80666B84 && cast != 0) {
        cf::CfWalkBattleSub* sub = (cf::CfWalkBattleSub*)cast;
        if (sub->vf44() != 0) {
            func_800FE920(func_800FE68C());
        }
        if (sub->vf48() != 0) {
            func_800FE938(func_800FE68C());
        }
        if (sub->vf20() != 0) {
            func_800FE7D8(func_800FE68C());
            self->vf70(0);
        }
        if (sub->vf18() != 0) {
            func_800FE68C();
            func_800FE738();
        }
        if (sub->vf1C() != 0) {
            func_800FE68C();
            func_800FE738();
        }
        sub->vf30();
        sub->vf34();
        sub->vf38();
        sub->vf3C();
        sub->vf40();
    }
}

// On a battle-id match on the own battle object, forwards to the walker's
// battle-entry hook and hands the battle object to the manager helper.
void func_800C3658(cf::CfObjectImplWalker* self, u32 battleId) {
    if (battleId == self->field_18->field_3F60) {
        self->vf64();
        func_800E1B5C(getInstance__Q22cf14CBattleManagerFv(), self->field_18);
    }
}

// Battle-object command dispatch (battle id must match the walker's own):
// routes sub-object commands (0x12 arts, 0x7 fight, 0x2/0x4 target queries)
// to the move sub-object / battle manager.
void func_800C36AC(cf::CfObjectImplWalker* self, u32 battleId, u32 cmd) {
    if (battleId != self->field_18->field_3F60) return;
    func_800C819C();
    switch (cmd) {
        case 0x12:
            self->field_18->mSub.m10(4);
            func_800FE950(func_800FE68C(), 0x80000003, 0, 0);
            break;
        case 0x7:
            self->field_18->mSub.m10(0x400);
            self->field_18->mSub.field_68 &= ~0x800;
            func_800FE950(func_800FE68C(), 0x80000004, 0x4002, 0);
            break;
        case 0x2: {
            cf::CfWalkBattleObj* bo = self->field_18;
            u32 v = bo->field_04->b30()->field_0;
            if (func_80174C98(bo, &v, 3) != 0) {
                func_800D9978(getInstance__Q22cf14CBattleManagerFv(),
                              self->field_18);
            }
            break;
        }
        case 0x4: {
            cf::CfWalkBattleObj* bo = self->field_18;
            u32 v2 = bo->field_04->b30()->field_0;
            u32 v3;
            // Either target query hitting arms the battle-exit notification.
            if (func_80174C98(bo, &v2, 1) != 0) goto battle_exit;
            bo = self->field_18;
            v3 = bo->field_04->b30()->field_0;
            if (func_80174C98(bo, &v3, 2) == 0) goto battle_skip;
        battle_exit:
            func_80174B4C(self->field_18, 0xE);
            func_8013EC6C(1, 0);
        battle_skip:
            self->field_18->mSub.m10(0x100);
            break;
        }
    }
}

// Party/battle-state update: mirrors the PC battle flags into the move
// sub-object, sweeps the party list feeding the move's fight state, and
// arms the area's battle-exit notification when nothing else is fighting.
void func_800C3878(cf::CfObjectImplWalker* self) {
    if (func_800FE68C()->field_90E4 != 0) {
        self->vf80();
    }
    self->field_18->field_04->b20(0x400000);
    self->field_18->field_04->b20(0x800000);
    void* gm = func_800821F8__Q22cf13CfGameManagerFv(
        getInstance__Q22cf13CfGameManagerFv());
    if (gm != 0) {
        func_8006BBF4(gm, 0x10, 1);
        if (func_800FE68C() != 0) {
            void* obj = func_800B708C((int)func_800FE68C()->field_90E4);
            if (obj != 0) {
                ((cf::CfWalkDynMgr*)gm)->vf2C(((cf::CfWalkObjAC*)obj)->vfAC());
            }
        }
    }
    // Sweep the party list, pushing each member into the move's fight state.
    cf::CfWalkEnumHolder holder;
    func_80043D90(&holder);
    func_800F4A98(func_80043F18(&holder), 0x20, 1);
    for (u32 i = 0; i < func_80043F18(&holder)->field_620; i++) {
        void* obj = func_800B708C((int)(uintptr_t)func_800F6E98(
            func_80043F18(&holder), i));
        if (obj != getPlayer__Q22cf13CfGameManagerFi(0)) {
            ((cf::CfWalkPlayerRec*)func_8016FE34(obj))->mSub.m08(0x100);
        }
    }
    // With no battle running, arm the walker's battle-exit notification.
    CfWalkBMView* bm = (CfWalkBMView*)getInstance__Q22cf14CBattleManagerFv();
    CfWalkChainNode* cur;
    u32 count = 0;
    CfWalkChainNode* head = bm->field_48;
    for (cur = head->next; cur != head; cur = cur->next, count++) {
    }
    if (count == 0) {
        void* r = ((cf::CfWalkBattleObjVtbl*)self->field_18)->vf2A4();
        if (!(((cf::CfWalkVf2A4Result*)r)->field_78 & 0xC00)) {
            // Retail zero-tests func_8009CF8C via the cntlzw/srwi boolean idiom.
#undef __cntlzw
            {
                u32 v = func_8009CF8C(0x335F);
                if (((u32)__cntlzw(v) >> 5) == 0) {
                    func_80133F48(1, lbl_eu_80666B8C);
                    func_800451D8(0xC3, 0);
                }
            }
        }
    }
    func_800C86E8(self);
    __dt__80043E88(&holder, -1);
}

// Leaves the current battle: cancels the move sub-object's fight state,
// then runs the PC-side leave hook.
void func_800C3A88(cf::CfObjectImplWalker* self) {
    self->field_18->mSub.m10C(0);
    func_800C969C(self);
}

// Toggles the current target selection: resolves the actor from the global
// battle list (or from the move sub-object), then walks the enum list of
// battle actors and applies a sight-line clamp to the selected one.
void func_800C3AD4(cf::CfObjectImplWalker* self) {
    void* v = func_800B708C((int)func_800FE68C()->field_90E4);
    if (v == 0) {
        v = func_800B708C((int)self->field_18->mSub.m4C());
    }
    if (v == 0) return;
    cf::CfWalkTalkSrc* mgr = (cf::CfWalkTalkSrc*)func_800BF324(v);
    if (mgr == 0) return;

    cf::CfWalkEnumHolder holder;
    func_80043D90(&holder);
    func_800F4A98(func_80043F18(&holder), 0x300, 0);
    for (u32 i = 0; i < func_80043F18(&holder)->field_620; i++) {
        cf::CfWalkTalkSrc* src = (cf::CfWalkTalkSrc*)func_800BBC0C(
            func_800B708C((int)(uintptr_t)func_800F6E98(func_80043F18(&holder), i)));
        if (src == mgr) continue;
        if (src->field_64 & 0x10000) continue;
        if (src->t160() == 0) continue;
        func_800BC3D8(src, lbl_eu_80666BA4);
    }
    __dt__80043E88(&holder, -1);
}

// Battle-start gate for the walker: with no battle registered and the time
// outside the flagged cutscene windows, validates the actor/talk state and
// starts a battle (camera/art setup, party sight-line clamps) or runs the
// non-talk fallback for other actor kinds.
void func_800C3BF0(cf::CfObjectImplWalker* self) {
    CfWalkBMView* bm = (CfWalkBMView*)getInstance__Q22cf14CBattleManagerFv();
    CfWalkChainNode* head = bm->field_08;
    u32 count = 0;
    CfWalkChainNode* cur;
    for (cur = head->next; cur != head; cur = cur->next, count++) {
    }
    if (count != 0) return;
    if (lbl_eu_80663E24 & 0x1000000) {
        func_800C3AD4(self);
        return;
    }
    u32 a = (u16)func_80086DA0__Q22cf13CfGameManagerFv();
    u32 t = a * 60 + (u16)func_80086DA4__Q22cf13CfGameManagerFv();
    if (t >= 0x167 && t <= 0x169) return;
    if (t >= 0x437 && t <= 0x439) return;
    if (((cf::CfWalkGimmickGlobal*)getUnk80664658())->field_214 & 0x20) return;
    void* actor = func_800B708C((int)func_800FE68C()->field_90E4);
    if (actor == 0) {
        actor = func_800B708C((int)self->field_18->mSub.m4C());
    }
    if (actor == 0) return;
    if (func_80226B94() != 0) return;
    void* bf = (void*)(uintptr_t)self->field_18->field_3F60;
    if (bf != 0) {
        if (((cf::CfWalkBattleFlags*)bf)->field_530 & 1) return;
        u32 v = ((cf::CfWalkBattleFlags*)bf)->field_4EC;
        if (!(v & 0x10000) && !(v & 0x4)) return;
    }
    // Talk-source gate: bit 3 of the actor flags selects the talk path
    // (retail computes the boolean via the cntlzw/srwi idiom).
    u32 f64 = ((cf::CfWalkActorObj*)actor)->field_64;
#undef __cntlzw
    if (((u32)__cntlzw((u32)__cntlzw(f64 & 8) >> 5) >> 5) != 0) {
        cf::CfWalkTalkSrc* mgr = (cf::CfWalkTalkSrc*)func_800BF324(actor);
        if (mgr->vf228() == 9) return;
        if (mgr->vf174() < lbl_eu_80666B8C) return;
        if (!(mgr->field_68 & 0x100000)) return;
        if (mgr->field_68 & 0x6000) return;
        if (func_80496044((CScn*)lbl_eu_80663E14) == 0) return;
        if (func_80084BF4__Q22cf13CfGameManagerFv() != 0) return;
        mgr->t110();
        if (func_800967F8() != 0) return;
        if (func_8011CD5C() != 0) return;
        if (func_80257308() != 0) return;
        if (func_802AC510() != 0) return;
        if (getInstance__11CSysWinBuffFv() != 0) return;
        cf::CfWalkMoveSub* msub = &self->field_18->mSub;
        cf::CfWalkBattleSubC4* bsub = (cf::CfWalkBattleSubC4*)((cf::CfWalkMoveSubDeep*)msub)->field_C4;
        if (!(bsub->field_4EC & 0x4)) {
            // Ground-probe the talk position; if the probe hits, the actor
            // is not free to start a battle.
            u32 free = 1;
            ml::CVec3 local(lbl_eu_80666B84, lbl_eu_80666BB0, lbl_eu_80666B84);
            ml::CVec3 pos = *(ml::CVec3*)msub->mAC();
            ml::CVec3 sum = pos + local;
            if (func_804BE398(&sum, 0x4a05, 0, 0, lbl_eu_80666BB4,
                              lbl_eu_80666B84) != 0) {
                void* res = func_804BE520(0);
                if (((ml::CVec3*)res)->y > lbl_eu_80666B84) {
                    free = 0;
                }
            }
            if (free != 0) return;
        }
        if (self->field_18->mSub.m0C(1) == 0) {
            func_8013EC6C(1, 1);
        }
        func_80142C80();
        mgr->t00(1);
        mgr->t48(self->field_18->mSub.field_74);
        cf::CfWalkBattleSubC4* sub = (cf::CfWalkBattleSubC4*)mgr->field_C4;
        if (sub != 0) {
            if (!(sub->field_270 & 0x100)) {
                if (mgr->field_64 & 1) {
                    ((cf::CfObject*)mgr)->func_800BE898(
                        0x1195, -1, lbl_eu_80666B8C, lbl_eu_80666BB8);
                } else {
                    func_800BF29C(mgr, 0x65, 0, (u32)lbl_eu_80666B80,
                                  lbl_eu_80666B8C, lbl_eu_80666BB8);
                }
            }
        }
        if (bf != 0) {
            ((cf::CfWalkBattleFlags*)bf)->field_4EC |= 0x1000;
            self->field_18->mSub.m9C(
                &((cf::CfWalkSubC4_3B4*)((cf::CfWalkMoveSubDeep*)
                    &self->field_18->mSub)->field_C4)->field_3B4);
        }
        lbl_eu_80663E24 |= 0x1000000;
        if (((cf::CfWalkActorObj*)actor)->field_C4 != 0) {
            self->field_18->mSub.m1AC((u32)actor, lbl_eu_804FC694);
        }
        // Clamp the sight-line of every other talk source in the area.
        cf::CfWalkEnumHolder holder;
        func_80043D90(&holder);
        func_800F4A98(func_80043F18(&holder), 0x300, 0);
        for (u32 i = 0; i < func_80043F18(&holder)->field_620; i++) {
            cf::CfWalkTalkSrc* src = (cf::CfWalkTalkSrc*)func_800BBC0C(
                func_800B708C((int)(uintptr_t)func_800F6E98(
                    func_80043F18(&holder), i)));
            if (src == mgr) continue;
            if (src->t160() == 0) continue;
            func_800BC3D8(src, lbl_eu_80666BA4);
        }
        func_8029194C();
        __dt__80043E88(&holder, -1);
        goto L4C30;
    }
    // Non-talk actor: route through the cast checks and the area hooks.
    if ((f64 & 0x20000) != 0 || (f64 & 0x10000) != 0) {
        cf::CfWalkCastFlags* cast = (cf::CfWalkCastFlags*)__dynamic_cast(
            actor, 0, (const void*)&lbl_eu_80661CB0,
            (const void*)&lbl_eu_806618F0, 0);
        if (cast->field_91 == 0xc) return;
        if (cast->field_90 != 0) {
            ((cf::CfWalkGimmickGlobal*)getUnk80664658())->field_218 = actor;
            return;
        }
        if (func_8011CD5C() != 0) return;
        if (func_80257308() != 0) return;
        if (func_802AC510() != 0) return;
        if (getInstance__11CSysWinBuffFv() != 0) return;
        func_80142C80();
        ((cf::CfWalkTalkSrc*)cast)->t00(1);
        lbl_eu_80663E24 |= 0x1000000;
        cf::CfWalkEnumHolder holder;
        func_80043D90(&holder);
        func_800F4A98(func_80043F18(&holder), 0x300, 0);
        for (u32 i = 0; i < func_80043F18(&holder)->field_620; i++) {
            void* src = func_800BBC0C(func_800B708C(
                (int)(uintptr_t)func_800F6E98(func_80043F18(&holder), i)));
            if (((cf::CfWalkTalkSrc*)src)->t160() != 0) {
                func_800BC3D8(src, lbl_eu_80666BA4);
            }
        }
        __dt__80043E88(&holder, -1);
        goto L4C30;
    }
    if (f64 & 0x100) return;
L4C30:
    func_8013EC60();
    self->field_18->mSub.m10(1);
    func_80174B4C(self->field_18, 1);
    self->field_18->vf230();
}

// Battle-actor sweep for the walker: scans the party list for a talk source
// in range and facing the player, dispatches its art/action hooks, then
// closes the battle with the move sub-object and clears the event flag.
void func_800C2E3C(cf::CfObjectImplWalker* self) {
    cf::CfWalkEnumHolder holder;
    func_80043D90(&holder);
    func_800F4A98(func_80043F18(&holder), 0x200, 0);
    void* mAC = self->field_18->mSub.mAC();
    __ct__800FB044(func_80043F18(&holder), lbl_eu_80666BA0, mAC, 1);
    __ct__800FA9B4(func_80043F18(&holder), lbl_eu_80663E14, 1);
    cf::CfWalkRect4 rect;
    func_8043E928__5CViewFRQ22ml5CRectP5CView(
        &rect, func_8049627C(lbl_eu_80663E14, -1));
    // Declaration order drives FPR allocation (retail: f29=BA8, f30=B84,
    // f31=B8C); keep these adjacent and in this exact order.
    f64 magic = lbl_eu_80666BA8;
    f32 zero = lbl_eu_80666B84;
    f32 f8c = lbl_eu_80666B8C;
    s32 found = 0;
    for (u32 i = 0; i < func_80043F18(&holder)->field_620; i++) {
        cf::CfWalkEnumItem* item = (cf::CfWalkEnumItem*)func_800F6EC0(
            func_80043F18(&holder), i);
        cf::CfWalkTalkSrc* src =
            (cf::CfWalkTalkSrc*)func_800BBC0C(item->field_04);
        if (lbl_eu_80663E24 & 0x1000000) goto L3ADC;
        if (item->field_18 & 0x38) goto L3ADC;
        s32 q = ((s32)(s16)rect.field_4) / 4;
        if (item->field_08 < (f32)q) goto L3ADC;
        if (item->field_08 > (f32)(q * 3)) goto L3ADC;
        if (found != 0) goto L3ADC;
        if (src->t16C() != zero) goto L3ADC;
        if (src->vf174() != f8c) goto L3ADC;
        if (src->t160() == 0) goto L3ADC;
        if (src->field_C4 == 0) goto L3ADC;
        if (src->t164() == 0) goto L3ADC;
        if (src->field_98 == 0) goto L3ADC;
        if (func_80496044((CScn*)lbl_eu_80663E14) == 0) goto L3ADC;
        // Target found: run the target-query checks to decide the hook.
        cf::CfWalkBattleObj* bo = self->field_18;
        u32 v1 = bo->field_04->b30()->field_0;
        if (func_80174C98(bo, &v1, 3) != 0) goto L3B20;
        bo = self->field_18;
        u32 v2 = bo->field_04->b30()->field_0;
        if (func_80174C98(bo, &v2, 4) != 0) goto L3B20;
L3ADC:
        if (src->t04(8) == 0) goto L3B48;
        func_8013D1E8((void*)(uintptr_t)src->field_74);
        src->t08(8);
        goto L3B48;
L3B20:
        if (func_8012CD24() != 0) goto L3B48;
        src->t00(8);
        found = 1;
L3B48:
        ;
    }
    __dt__80043E88(&holder, -1);
    // Battle-entry: with the event flag set and no active battle object,
    // bind the player actor into the move sub-object and clear the flag.
    if (lbl_eu_80663E24 & 0x1000000) {
        void* bf = (void*)(uintptr_t)self->field_18->field_3F60;
        if (bf == 0 || (((cf::CfWalkBattleState3F60*)bf)->field_0C & 0x4) ||
            (((cf::CfWalkBattleState3F60*)bf)->field_4EC & 0x4)) {
            goto L3C68;
        }
        func_8013EC6C(1, 0);
        self->field_18->mSub.m1AC(0, lbl_eu_804FC694);
        func_80174B4C(self->field_18, 3);
        lbl_eu_80663E24 &= ~0x1000000;
        void* actor = func_800B708C((int)func_800FE68C()->field_90E4);
        if (actor != 0) {
            ((cf::CfWalkTalkSrc*)actor)->t08(1);
            ((cf::CfWalkTalkSrc*)actor)->t10(3);
            ((cf::CfWalkTalkSrc*)actor)->t48(0);
            if (((cf::CfWalkActorObj*)actor)->field_64 & 8) {
                ((cf::CfWalkTalkSrc*)actor)->t1AC(0, lbl_eu_804FC694);
            }
        }
        goto L4070;
    }
L3C68:
    // Party/action sweep: runs the pending battle-state transitions, then
    // the party-wide sight-line clamps before closing the battle.
    {
        u32 done = 0;
        if (func_8013EB90(1) == 0) {
            if (self->vf40(0x8000) != 0) {
                if (func_800C4244(self, self->field_384, 0) != 0) goto L4070;
            } else if (self->vf40(0x80000) != 0) {
                if (func_800C4BD4(self, 0, 0) != 0) goto L4070;
            }
        }
        if (func_80122448() != 0) goto L4070;
        func_80043D90(&holder);
        func_800F4A98(func_80043F18(&holder), 0xa20, 0);
        for (u32 i = 0; i < func_80043F18(&holder)->field_620; i++) {
            void* obj = func_800B708C((int)(uintptr_t)func_800F6E98(
                func_80043F18(&holder), i));
            void* msub = self->field_18;
            if (msub != 0) {
                msub = &((cf::CfWalkBattleObj*)msub)->mSub;
            }
            if (msub == obj) continue;
            cf::CfWalkTalkSrc* src = (cf::CfWalkTalkSrc*)obj;
            if (src->t04(1) != 0) continue;
            if (src->t04(0x10) != 0) continue;
            if (src->t1C(1) == 0) continue;
            done = 1;
            goto L3DD4;
        }
    L3DD4:
        if (done != 0) goto L4064;
        if (func_8013EB90(1) != 0) goto L4064;
        func_800F4A98(func_80043F18(&holder), 0x300, 0);
        for (u32 i = 0; i < func_80043F18(&holder)->field_620; i++) {
            cf::CfWalkTalkSrc* src = (cf::CfWalkTalkSrc*)func_800BBC0C(
                func_800B708C((int)(uintptr_t)func_800F6E98(
                    func_80043F18(&holder), i)));
            if (src->field_68 & 0x100000) {
                func_800BC3B0(src, lbl_eu_80666BA4);
            }
        }
        void* actor = func_800B708C((int)self->field_18->mSub.m4C());
        if (actor != 0) {
            if (self->vf40(0x10000) == 0) {
                if (!(lbl_eu_80663E24 & 0x100000)) {
                    void* art = func_80140AFC(
                        ((cf::CfWalkTalkSrc*)actor)->field_8C);
                    if (art != 0) {
                        u32 t = ((cf::CfWalkArtRec*)art)->field_4;
                        bool isFC = (t == 0xfc || t == 0xfd);
                        if (isFC) {
                            func_8013F354(art);
                            self->vf30(0x10000, 1);
                            __dt__80043E88(&holder, -1);
                            goto L4070;
                        }
                        if (t == 0x2) {
                            if (func_8013EC58() == 0) {
                                func_8013F354(art);
                            } else {
                                func_8013F3EC(art);
                            }
                        }
                    } else {
                        void* art2 = func_80140CA4(
                            ((cf::CfWalkTalkSrc*)actor)->field_8C);
                        if (art2 != 0) {
                            if (func_8013EC58() == 1) {
                                func_8013F2A0(art2);
                            } else {
                                func_8013EC58();
                            }
                        }
                    }
                }
            } else {
                self->vf34(0x10000);
            }
            if (((cf::CfWalkActorObj*)actor)->field_64 & 8) {
                cf::CfWalkTalkSrc* mgr =
                    (cf::CfWalkTalkSrc*)func_800BF324(actor);
                if (mgr->vf22C() != 0) {
                    u32 v = mgr->vf22C();
                    func_8009D018((u32)(uintptr_t)v + 0xa20, 1);
                    func_80291A04();
                }
                if (!(lbl_eu_80663E24 & 0x100000)) {
                    u16 v5 = self->field_18->mSub.field_8C;
                    if (v5 == 8) v5 = 3;
                    func_80140E00(3, ((cf::CfWalkTalkSrc*)actor)->field_8C,
                                  v5);
                }
                func_8009ECD0(mgr->field_8C);
            } else {
                u32 f64 = ((cf::CfWalkActorObj*)actor)->field_64;
                if ((f64 & 0x20000) != 0 || (f64 & 0x10000) != 0) {
                    func_80140E00(5, ((cf::CfWalkTalkSrc*)actor)->field_8C,
                                  0);
                }
            }
        }
        self->field_18->mSub.m1AC(0, lbl_eu_804FC694);
        func_80174B4C(self->field_18, 3);
        lbl_eu_80663E24 &= ~0x1000000;
    }
L4064:
    __dt__80043E88(&holder, -1);
L4070:
    return;
}

// Battle arts-state machine: uses the bdat table for the current battle
// state (field_388) to pick the next action row, then advances the state or
// resets the walker's battle fields. Returns 1 when a step was taken.
int func_800C4244(cf::CfObjectImplWalker* self, u32 battleId, u32 slot) {
    u32 tbl[3];
    tbl[0] = lbl_eu_804FC670[0];
    tbl[1] = lbl_eu_804FC670[1];
    tbl[2] = lbl_eu_804FC670[2];
    void* r28 = func_8003AA34();
    if (battleId == 0) goto reset;
    if (self->field_388 == 3) goto reset;
    if (self->vf40(0x8000) == 0) {
        self->vf30(0x8000, 1);
        self->field_388 = 0;
        self->field_384 = battleId;
    }
    func_8003AA34();
    void* fp = getFP__FPCc((const char*)tbl[self->field_388]);
    if (fp == 0) goto reset;
    if (r28 == 0) goto reset;
    u32 rowCount = func_8003B1EC(fp);
    switch (self->field_388) {
    case 0: {
        self->field_398 = slot;
        void* obj = func_800B708C((int)func_800FE68C()->field_90E4);
        if (obj != 0) {
            ((cf::CfWalkTalkSrc*)obj)->t10(1);
        }
        for (u32 i = 0; i < rowCount; i++) {
            char* col = (char*)getBdatStringColumnValue(fp, lbl_eu_804FC694 + 7, i + 1);
            if ((u8)col[0] == battleId) {
                char* col2 = (char*)getBdatStringColumnValue(fp, lbl_eu_804FC694 + 0xc, i + 1);
                u16 v = self->field_18->mSub.field_8C;
                if (v == 8) v = 3;
                if (v == (u8)col2[0]) {
                    char* col3 =
                        (char*)getBdatStringColumnValue(fp, lbl_eu_804FC694 + 0x13, i + 1);
                    func_8013D07C(self->field_18->mSub.field_74, col3, 1);
                    self->field_388++;
                    return 1;
                }
            }
        }
        goto reset;
    }
    case 1: {
        if (self->field_398 == 0) {
            u32 r25;
            if ((u32)rand() % 100 < 0x56) {
                if ((u32)rand() & 1) {
                    r25 = 0x100000;
                } else {
                    r25 = 0x200000;
                }
            } else {
                r25 = 0;
            }
            cf::CfWalkEnumHolder holder;
            func_80043D90(&holder);
            func_800F4A98(func_80043F18(&holder), r25, 0);
            self->field_394 = (u32)func_800F6E08(func_80043F18(&holder));
            __dt__80043E88(&holder, -1);
        }
        void* talk = func_800BBC0C(func_800B708C((int)self->field_394));
        if (talk == 0) goto reset;
        u16 t8c = ((cf::CfWalkTalkSrc*)talk)->field_8C;
        if (t8c == 8) t8c = 3;
        // Retail reuses the `slot` param register (r23) as this loop counter.
        for (slot = 0; slot < rowCount; slot++) {
            char* col = (char*)getBdatStringColumnValue(fp, lbl_eu_804FC694 + 7, slot + 1);
            if ((u8)col[0] == battleId) {
                char* col2 = (char*)getBdatStringColumnValue(fp, lbl_eu_804FC694 + 0xc, slot + 1);
                u16 v = self->field_18->mSub.field_8C;
                if (v == 8) v = 3;
                if (v == (u8)col2[0]) {
                    char* col3 =
                        (char*)getBdatStringColumnValue(fp, lbl_eu_804FC694 + 0x1b, slot + 1);
                    if ((u8)col3[0] == t8c) {
                        char* col4 =
                            (char*)getBdatStringColumnValue(fp, lbl_eu_804FC694 + 0x24, slot + 1);
                        s32 digit = (s8)col4[9] - '0';
                        if ((u8)col4[0xb] == ':') {
                            digit = (s8)col4[0xa] + digit * 10 - '0';
                        }
                        u32 r27 = (u32)func_8009CF8C(
                            (u32)func_801413DC(0x280016, digit + 1));
                        self->field_390 = 0;
                        u32 rem = (u32)rand() % 100;
                        f32 fr = (f32)(s32)r27;
                        if ((f32)(s32)rem <
                            fr * lbl_eu_80666BC0 * lbl_eu_80666BBC +
                            lbl_eu_80666BA4) {
                            self->field_390 = 1;
                        }
                        char* col5 = (char*)getBdatStringColumnValue(
                            fp, lbl_eu_804FC694 +
                                (self->field_390 ? 0x31 : 0x24),
                            slot + 1);
                        func_8013D07C(((cf::CfWalkTalkSrc*)talk)->field_74,
                                      col5, 1);
                        self->field_38C = slot + 1;
                        self->field_388++;
                        void* msub = self->field_18;
                        if (msub != 0) {
                            msub = &((cf::CfWalkBattleObj*)msub)->mSub;
                        }
                        ((cf::CfWalkTalkSrc*)talk)->t1AC((u32)msub,
                                                        lbl_eu_804FC694);
                        ((cf::CfWalkTalkSrc*)talk)->t10(1);
                        func_802919A0();
                        return 1;
                    }
                }
            }
        }
        goto reset;
    }
    case 2: {
        if (self->field_390 == 0) goto reset;
        char* col = (char*)getBdatStringColumnValue(
            fp, lbl_eu_804FC694 + 0x3f, self->field_38C);
        func_8013D07C(self->field_18->mSub.field_74, col, 1);
        void* talk = func_800BBC0C(func_800B708C((int)self->field_394));
        if (talk != 0) {
            self->field_18->mSub.m1AC((u32)talk, lbl_eu_804FC694);
        }
        self->field_388++;
        return 1;
    }
    default:
        break;
    }
reset:
    {
        void* talk = func_800BBC0C(func_800B708C((int)self->field_394));
        if (talk != 0) {
            ((cf::CfWalkTalkSrc*)talk)->t1AC(0, lbl_eu_804FC694);
            ((cf::CfWalkTalkSrc*)talk)->t10(3);
        }
        void* talk2 =
            func_800BBC0C(func_800B708C((int)func_800FE68C()->field_90E4));
        if (talk2 != 0) {
            self->field_18->mSub.m1AC((u32)talk2, lbl_eu_804FC694);
        }
        self->field_384 = 0;
        self->field_388 = 0;
        self->field_38C = 0;
        self->field_390 = 0;
        self->field_398 = 0;
        self->vf34(0x8000);
        return 0;
    }
}

// Battle-state gate for the walker: refuses while any battle/event is
// registered or the target is in a flagged time window, then walks the
// talk-source state checks before starting a new battle (notifies the
// source, arms the walker hooks, sweeps the party for sight-line clamps,
// syncs the battle object and resets the walker's state word).
void func_800C4888(cf::CfObjectImplWalker* self) {
    CfWalkBMView* bm = (CfWalkBMView*)getInstance__Q22cf14CBattleManagerFv();
    CfWalkChainNode* head = bm->field_08;
    u32 count = 0;
    CfWalkChainNode* cur;
    for (cur = head->next; cur != head; cur = cur->next, count++) {
    }
    if (count != 0) return;
    if (lbl_eu_80663E24 & 0x9048000) return;
    void* actor = func_800B708C((int)func_800FE68C()->field_90E4);
    if (actor == 0) return;
    if (func_80226B94() != 0) return;
    u32 a = (u16)func_80086DA0__Q22cf13CfGameManagerFv();
    u32 b = (u16)func_80086DA4__Q22cf13CfGameManagerFv();
    u32 t = a * 60 + b;
    // WALL (MWCC_CASES "Range-test normalization"): retail keeps two
    // cmplwi/blt/ble pairs per check; MWCC 1.1 -O4,p fuses lo<=t<=hi into
    // subi/cmplwi span. The OR'd form keeps the SECOND range un-fused (best
    // observed: 156 structural); the first range still fuses.
    if ((t >= 0x167 && t <= 0x169) || (t >= 0x437 && t <= 0x439)) return;

    cf::CfWalkActorObj* obj = (cf::CfWalkActorObj*)actor;
    if (obj->field_64 & 8) {
        cf::CfWalkTalkSrc* mgr = (cf::CfWalkTalkSrc*)func_800BF324(actor);
        if (mgr->vf228() != 2) return;
        if (mgr->vf174() < lbl_eu_80666B8C) return;
        if (!(mgr->field_68 & 0x100000)) return;
        if (mgr->field_68 & 0x6000) return;
        if (mgr->t1C(0x1000) != 0) return;
        func_8013EC6C(1, 1);
        if (func_8011CD5C() != 0) return;
        if (func_80257308() != 0) return;
        if (func_802AC510() != 0) return;
        if (getInstance__11CSysWinBuffFv() != 0) return;
        func_80142C80();
        mgr->t00(1);
        mgr->t48(self->field_18->mSub.field_74);
        self->vf30(0x80000000, 1);
        lbl_eu_80663E24 |= 0x90000000;
        self->field_18->mSub.m1AC((u32)(uintptr_t)actor, lbl_eu_804FC694);

        // Sweep the party list; clamp the sight-line of every talk source
        // except the one we are starting the battle with.
        cf::CfWalkEnumHolder holder;
        func_80043D90(&holder);
        func_800F4A98(func_80043F18(&holder), 0x300, 0);
        for (u32 i = 0; i < func_80043F18(&holder)->field_620; i++) {
            cf::CfWalkTalkSrc* src = (cf::CfWalkTalkSrc*)func_800BBC0C(
                func_800B708C((int)(uintptr_t)func_800F6E98(
                    func_80043F18(&holder), i)));
            if (src == mgr) continue;
            if (src->t160() == 0) continue;
            func_800BC3D8(src, lbl_eu_80666BA4);
        }
        func_80174B4C(self->field_18, 1);
        self->field_18->vf230();
        if (self->field_18->field_3F60 != 0) {
            ((cf::CfWalkBattleFlags*)(uintptr_t)self->field_18->field_3F60)
                ->field_4EC |= 0x1000;
            self->field_18->mSub.m9C(
                &((cf::CfWalkMoveSubDeep*)&self->field_18->mSub)
                    ->field_C4->field_3B4);
        }
        // Scan the 9-slot weapon/arts table; flag the entry-0x8F marker.
        u32 found = 0;
        u32* p = (u32*)func_8009ECB0() + 1;
        for (u32 j = 0; j < 9; j++, p++) {
            if (*p != 0 &&
                func_8026178C(func_8009EC9C((u16)*p) + 0x3534, 0x8f) != 0) {
                found = 1;
            }
        }
        if (found != 0) {
            func_80109784(self->field_18->mSub.field_74, 0x8f, 9);
        }
        __dt__80043E88(&holder, -1);
    }
    self->field_388 = 0;
}

// Enemy-side battle arts state machine: scans the fight-capable actor list
// for a free talk source, then advances the bdat-driven battle step counter
// (field_388) through its six states. Returns 1 when a step was taken.
int func_800C4BD4(cf::CfObjectImplWalker* self, u32 a, u32 b) {
    // Resolve the global actor and its talk-source manager.
    void* actor = func_800B708C((int)(uintptr_t)func_800FE68C()->field_90E4);
    cf::CfWalkTalkSrc* mgr = (cf::CfWalkTalkSrc*)func_800BF324(actor);

    cf::CfWalkEnumHolder holder;
    func_80043D90(&holder);
    int found = 0;
    func_800F4A98(func_80043F18(&holder), 0x220, 0);
    for (u32 i = 0; i < (u32)func_80043F18(&holder)->field_620; i++) {
        cf::CfWalkTalkSrc* talk =
            (cf::CfWalkTalkSrc*)func_800BBC0C(func_800B708C(
                (int)(uintptr_t)func_800F6E98(func_80043F18(&holder), i)));
        // Skip entries bound to this walker's own move sub-object.
        void* own = self->field_18 ? (void*)&self->field_18->mSub : (void*)0;
        if ((void*)talk == own) continue;
        if (talk->t04(1) != 0 || talk->t1C(1) != 0) {
            found = 1;
            break;
        }
    }

    if (found == 0 || mgr == 0) {
        // No candidate: detach the move sub-object and reset to state 5.
        self->field_18->mSub.m1AC(0, lbl_eu_804FC694);
        func_80174B4C(self->field_18, 3);
        lbl_eu_80663E24 &= ~0x00800000;
        self->field_388 = 5;
    }
    __dt__80043E88(&holder, -1);

    switch (self->field_388) {
    case 0:
        if (mgr->t1C(1) != 0) {
            // First contact: pick the intro line for the actor kind.
            u8 kind = (u8)getBdatStringColumnValue(
                lbl_eu_80664098, lbl_eu_804FC694 + 0x4b,
                (int)(u16)mgr->field_8C);
            char* col = (char*)getBdatStringColumnValue(
                lbl_eu_806640C4, lbl_eu_804FC694 + 0x57, (int)kind);
            func_8013EC6C(1, 1);
            self->field_394 = (u32)(uintptr_t)mgr->field_74;
            self->field_38C = kind;
            func_8013D07C((u32)(uintptr_t)mgr->field_74, col, 1);
            // Retail converts the sdata2 constant through __cvt_fp2unsigned.
            func_800BF29C(mgr, 0x65, 0, (u32)lbl_eu_80666B80,
                          lbl_eu_80666B8C, lbl_eu_80666BB8);
            self->field_388++;
        }
        break;
    case 1:
        func_8013D07C(self->field_394, lbl_eu_804FC694 + 0x61, 1);
        func_800BF29C(mgr, 0x65, 0, self->field_388, 0, 0);
        self->field_388++;
        break;
    case 2:
        func_8013D07C(self->field_394,
                      (const char*)getBdatStringColumnValue(
                          lbl_eu_806640C4, lbl_eu_804FC694 + 0x70,
                          (int)self->field_38C),
                      1);
        self->field_388 = 5;
        break;
    case 3: {
        // Queue the follow-up event with fixed flags.
        u16 v = (u16)(uintptr_t)getBdatStringColumnValue(
            lbl_eu_80664098, lbl_eu_804FC694 + 0x7a,
            (int)(u16)mgr->field_8C);
        func_8013E2E0(v, 0, 0, 0, 0, 1, 0, 1);
        self->field_388++;
        break;
    }
    case 4:
        func_8013D07C(self->field_394,
                      (const char*)getBdatStringColumnValue(
                          lbl_eu_806640C4, lbl_eu_804FC694 + 0x70,
                          (int)self->field_38C),
                      1);
        self->field_388++;
        break;
    case 5:
        // Reset every battle-step field and drop the local flag bit.
        self->field_384 = 0;
        self->field_388 = 0;
        self->field_38C = 0;
        self->field_390 = 0;
        self->field_394 = 0;
        self->field_398 = 0;
        self->vf34(0x80000000);
        break;
    }
    return 1;
}

// Battle-start effect fanfare: clears every party member's pending battle
// state, notifies the battle manager, then walks the actor list rolling
// random gates to broadcast three area effects (0x44/0x45/0x02) plus a
// per-member command (0x36).
static void SpawnEffect(cf::CfWalkEnumList* list, u16 effId,
                        cf::CfWalkSpawnRec* rec) {
    CfWalkBMView* bm = (CfWalkBMView*)getInstance__Q22cf14CBattleManagerFv();
    for (u32 j = 0; j < (u32)list->field_620; j++) {
        void* item = func_8016FE34(func_800F6EAC(list, (int)j));
        func_800EA9A8(bm, item, rec, effId, 0);
    }
}

void func_800C551C(cf::CfObjectImplWalker* self, u32 flag) {
    // Reset pass over the party records.
    cf::CfWalkEnumHolder holderA;
    func_80043D90(&holderA);
    cf::CfWalkEnumList* listA = func_80043F18(&holderA);
    func_800F4A98(listA, 0x20, 0);
    for (u32 i = 0; i < (u32)listA->field_620; i++) {
        cf::CfWalkPlayerRec* rec =
            (cf::CfWalkPlayerRec*)func_8016FE34(func_800F6EAC(listA, (int)i));
        if (rec->f192(1) == 0) {
            rec->f191(1);
        }
    }
    __dt__80043E88(&holderA, -1);

    // Notify the battle manager of the battle start (variant depends on the
    // caller's flag), then sync the battle object.
    if (flag != 0) {
        func_800F3970(getInstance__Q22cf14CBattleManagerFv(), self->field_18,
                      0, 0, 0);
    } else {
        func_800F3970(getInstance__Q22cf14CBattleManagerFv(), self->field_18,
                      0, 1, 0);
    }
    func_802A216C(self->field_18);

    // Effect roll pass.
    cf::CfWalkEnumHolder holder;
    func_80043D90(&holder);
    cf::CfWalkEnumList* list = func_80043F18(&holder);
    func_800F4A98(list, 0x20, 0);
    const u16 effA = 0x44;
    const u16 effB = 0x45;
    const u16 effC = 0x2;
    for (u32 i = 0; i < (u32)list->field_620; i++) {
        cf::CfWalkPlayerRec* rec =
            (cf::CfWalkPlayerRec*)func_8016FE34(func_800F6EAC(list, (int)i));
        if (rec->f162() == 0) continue;
        rec->f162();

        u32 val10;
        u32 valC;
        f32 val8;
        // Three gated effect broadcasts: two random-chance (0x20/0x21) and
        // one unconditional-query (0x33), then a forced command (0x36).
        if (func_80260FB0(rec, 0x20, &val10, &valC, &val8) != 0 &&
            (s32)ml::math::mtRand(100) < (s32)valC) {
            cf::CfWalkSpawnRec spawn;
            memset(&spawn, 0, sizeof(cf::CfWalkSpawnRec));
            spawn.field_C = effA;
            spawn.field_10 = val10;
            spawn.field_20 = val8;
            SpawnEffect(list, effA, &spawn);
        }
        if (func_80260FB0(rec, 0x21, &val10, &valC, &val8) != 0 &&
            (s32)ml::math::mtRand(100) < (s32)valC) {
            cf::CfWalkSpawnRec spawn;
            memset(&spawn, 0, sizeof(cf::CfWalkSpawnRec));
            spawn.field_C = effB;
            spawn.field_10 = val10;
            spawn.field_20 = val8;
            SpawnEffect(list, effB, &spawn);
        }
        if (func_80260518(rec, 0x33, &val10, &val8) != 0) {
            cf::CfWalkSpawnRec spawn;
            memset(&spawn, 0, sizeof(cf::CfWalkSpawnRec));
            spawn.field_C = effC;
            spawn.field_10 = val10;
            spawn.field_20 = val8;
            SpawnEffect(list, effC, &spawn);
        }
        if (func_80260264(rec, 0x36, &val10) != 0) {
            for (u32 j = 0; j < (u32)list->field_620; j++) {
                cf::CfWalkPlayerRec* item =
                    (cf::CfWalkPlayerRec*)func_8016FE34(
                        func_800F6EAC(list, (int)j));
                item->vf2F8(val10);
            }
        }
    }
    __dt__80043E88(&holder, -1);
}

// Target/party switch: if the move sub-object already tracks the given
// actor, only the non-battle fallback runs; otherwise retargets the move
// sub-object, guards on the global/self targets, and reports the change to
// the walker's own vtable hook.
void func_800C4FB8(cf::CfObjectImplWalker* self, u32 arg) {
    if (self->field_18->mSub.m4C() != arg) {
        self->field_18->mSub.m50(arg);
        u32 g = func_800FE68C()->field_90E4;
        if (self->field_18->mSub.m4C() != g &&
            self->field_18->mSub.m4C() != self->field_18->mSub.field_74) {
            self->field_18->mSub.m08(4);
        }
        if (arg == 0) {
            self->field_18->mSub.m10(1);
        }
        self->vf84(arg);
    } else {
        if (arg != 0) {
            self->field_18->field_04->b20(0x00800000);
        }
    }
}

// Battle-entry hook for the walker: refuses while any battle object is
// already registered, then validates the presentation flags and the player's
// battle state before binding the player's battle object into the walker.
void func_800C50F4(cf::CfObjectImplWalker* self) {
    // Count live battle objects; bail if any battle is already registered.
    CfWalkBMView* bm = (CfWalkBMView*)getInstance__Q22cf14CBattleManagerFv();
    CfWalkChainNode* cur;
    u32 count = 0;
    CfWalkChainNode* head = bm->field_08;
    for (cur = head->next; cur != head; cur = cur->next, count++) {
    }
    if (count != 0) return;
    if (lbl_eu_80663E24 & 0xD2C8004) return;
    if (func_8013EB90(1) != 0) return;
    cf::CfWalkPlayerRec* rec = (cf::CfWalkPlayerRec*)func_8016FE34(
        getPlayer__Q22cf13CfGameManagerFi(0));
    if (rec != 0) {
        if (rec->field_3F60 != 0 &&
            ((cf::CfWalkBattleFlags*)rec->field_3F60)->field_4EC & 0x140000) {
            return;
        }
        if (rec->vf2BC() != 0) return;
        if (lbl_eu_80663E24 & 0x400000) return;
    }
    void* v = func_800B708C((int)func_800FE68C()->field_90E4);
    cf::CfWalkActorObj* actor = (cf::CfWalkActorObj*)v;
    if (v == 0) return;
    if ((((cf::CfWalkActorObj*)v)->field_64 & 0x100) == 0) return;
    void* sub = actor->field_98;
    if (sub != 0) ((cf::CfWalkSub88*)sub)->vf88(0);
    func_800BE12C(actor, 0x21, 0, -1, 1);
    func_801F8E20(actor, 1);
    func_8013EC6C(1, 0);
    lbl_eu_80663E24 |= 0x200000;
    getInstance__Q22cf13CfGameManagerFv()->field_80 = actor->field_74;
    self->field_39C = 1;
}

// Battle-leave cleanup for the walker: refuses while battles/events run,
// validates the player battle state, then either aborts the leave (reset
// flags, detach) or commits it (play the exit voice, bump area sequence
// counters, clear the battle binding).
void func_800C525C(cf::CfObjectImplWalker* self) {
    if (lbl_eu_80663E24 & 0x0D048000) return;
    if (func_8013EB90(1) != 0) return;
    if (self->field_39C == 0) return;
    if ((lbl_eu_80663E24 & 0x200000) == 0) return;
    cf::CfWalkActorObj* obj = (cf::CfWalkActorObj*)func_800B708C(
        (int)(uintptr_t)getInstance__Q22cf13CfGameManagerFv()->field_80);
    if (obj == 0) return;
    if (!(obj->field_64 & 0x100)) return;
        // Validate that the player really is out of battle before leaving.
        bool ok = true;
        cf::CfWalkPlayerRec* rec = (cf::CfWalkPlayerRec*)func_8016FE34(
            getPlayer__Q22cf13CfGameManagerFi(0));
        if (rec == 0 || rec->vf2BC() != 0) {
            ok = false;
        } else if (obj == 0) {
            ok = false;
        } else if (lbl_eu_80663E24 & 0x400000) {
            ok = false;
        } else if (lbl_eu_80663E24 & 0x4) {
            ok = false;
        } else if (func_80085840__Q22cf13CfGameManagerFv() == 0) {
            ok = false;
        }
        if (!ok) {
            // Abort the leave: detach the battle sub-object and clear the binding.
            self->field_39C = 0;
        if (obj->field_98 != 0) func_80485774(obj->field_98, 0);
        lbl_eu_80663E24 &= ~0x400;
        getInstance__Q22cf13CfGameManagerFv()->field_80 = 0;
    } else {
        cf::CfWalkSubC4* sub = obj->field_C4;
        if (sub != 0 && sub->vf0C() == sub->vf14() - lbl_eu_80666B8C) {
            func_8015AFA4(obj->field_734, obj->field_738);
            func_8013E424(&lbl_eu_80573E18, 0);
            lbl_eu_80663E24 &= ~0x400;
            self->field_39C = 0;
            // Bump the area step counters for the leaving area.
            if (obj->field_734 == 2) {
                u32 c = func_80082694__Q22cf13CfGameManagerFv(0x6B) + 1;
                func_8008269C__Q22cf13CfGameManagerFv(0x6B, c);
                if (c == 10) {
                    func_800826F0__Q22cf13CfGameManagerFv(0x6B);
                } else if (c == 0x32) {
                    func_800826F0__Q22cf13CfGameManagerFv(0x6C);
                }
            }
            if (obj->field_734 == 3) {
                u32 c = func_80082694__Q22cf13CfGameManagerFv(0x6D) + 1;
                func_8008269C__Q22cf13CfGameManagerFv(0x6D, c);
                if (c == 1) {
                    func_800826F0__Q22cf13CfGameManagerFv(0x6D);
                } else if (c == 10) {
                    func_800826F0__Q22cf13CfGameManagerFv(0x6E);
                } else if (c == 0x32) {
                    func_800826F0__Q22cf13CfGameManagerFv(0x6F);
                }
            }
            if (obj->field_734 >= 2) {
                u32 c = func_80082694__Q22cf13CfGameManagerFv(0xBA) + 1;
                func_8008269C__Q22cf13CfGameManagerFv(0xBA, c);
                if (c == 0x3E8) {
                    func_800826F0__Q22cf13CfGameManagerFv(0xBA);
                }
            }
        }
    }
}

void func_800C5928(void) {}

extern "C" void* __dt__Q22cf18CfObjectImplWalkerFv(
    cf::CfObjectImplWalker* self, int deleteFlag) {
    if (self != nullptr && deleteFlag > 0) {
        operator delete(self);
    }
    return self;
}

void func_800C596C(void) {}

void func_800C5978(char* self) { ((void(*)(void*))func_800C6EC0)(self - 0xc); }

void func_800C5980(char* self) { ((void(*)(void*))__dt__Q22cf18CfObjectImplWalkerFv)(self - 0xc); }

void func_800C5988(char* self) { ((void(*)(void*))func_800CFFA0)(self - 0x10); }

void func_800C5990(char* self) { ((void(*)(void*))__dt__Q22cf18CfObjectImplWalkerFv)(self - 0x10); }
