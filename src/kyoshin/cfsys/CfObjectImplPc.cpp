// Auto-scaffolded catalog TU for kyoshin/cfsys/CfObjectImplPc
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/harness_catalog.hpp"
#include "kyoshin/cf/CfMapItemManager.hpp"
#include "kyoshin/cfsys/CfObjectImplPc.hpp"

#include "monolib/util/MemManager.hpp"
#include <string.h>
#include "kyoshin/cf/CfGameManager.hpp"
#include "kyoshin/cf/CfSoundMan.hpp"
#include "kyoshin/cf/CfGameManagerData.hpp"  // lbl_eu_80663E14 (CScn*)
#include "monolib/scn/CScnTimeApi.hpp"        // func_80496288

// Minimal cf::CBattleManager view (the full header conflicts with this TU's
// C-ABI import declarations). Only getInstance / +0x194 / +0x1A8 are needed.
namespace cf {
struct CfBattleChainNode;  // defined in CfObjectImplPc.hpp
class CBattleManager {
public:
    virtual ~CBattleManager();
    static CBattleManager* getInstance();
    u8 _pad04[0x28 - 0x04];
    CfBattleChainNode mListA;        // 0x28: second intrusive list sentinel
    u8 _pad2C[0x48 - 0x2C];
    CfBattleChainNode mListHead;     // 0x48: intrusive list sentinel, next at +0
    u8 _pad4C[0x194 - 0x4C];
    u8 unk194[0x1A8 - 0x194];        // party-gauge sub-object
    u8 mChain[0x283D8 - 0x1A8];      // chain gauge block at +0x1A8
};
} // namespace cf

// Player init/refresh: prepares via func_800CA948, dispatches vtable slot
// 0xE4 and feeds its result + sub-fields into func_8015BB3C, binds the +0x28
// holder to the battle object's +0x3E9C sub-object, and when the +0x70 token
// formats with entry id 7, allocates + constructs a CPcEffect07 whose +4
// pointer becomes field_378. Finally resets the battle object's idle flag
// word (vtable 0x27C result) and notifies the +0x3E9C sub-object (vtable
// 0x20C) when the battle id at +0x3F28 is 1.
void func_800C5998(cf::CfObjectImplPc* self)
{
    func_800CA948((u8*)self);
    func_8015BB3C(self->vfE4(), (u8*)self->field_14, (u8*)self->field_18);
    u8* owner = (u8*)self->field_18;
    if (owner != 0) {
        owner += 0x3e9c;
    }
    func_802A0B8C((u8*)&self->field_28, owner);
    u32 out14;
    u32 out10;
    u32 outC;
    u32 out8;
    func_800AA318(self->field_14->field_70, &out14, &out10, &outC, &out8);
    if (out10 == 7) {
        u8* mem = (u8*)mtl::MemManager::allocate(0x64, func_80061FE8());
        if (mem != 0) {
            u8* actor = (u8*)self->field_14;
            if (actor != 0) {
                actor -= 0x3e9c;
            }
            mem = __ct__cf_CPcEffect07(mem, actor);
        }
        if (mem != 0) {
            mem = (u8*)mem + 4;
        }
        self->field_378 = (cf::CfObjectImplPcSub*)mem;
    }
    if (self->field_18->field_3F28 == 1) {
        cf::CfObjectImplPc27C* p = self->field_18->vf27C();
        p->field_00 = 0;
        self->field_18->mSub.sf20C(0x27);
    }
}

void func_800C5AA0(cf::CfObjectImplPc* self)
{
    if (self->field_378 != 0) {
        if (self->field_378 != 0) {
            self->field_378->vf08(1);
        }
        self->field_378 = 0;
    }
    func_800CAA44(self);
}

// Second-call-site hack removed: __declspec(noinline) on func_800C891C
// keeps the retail out-of-line `bl`.

// Result of func_800EA444: two id words compared against the battle
// object's own id (+0x3F10).
struct CfObjectImplPcEA444 {
    u32 field_00;
    u32 field_04;
};

// 0x20-byte status-effect record built on the stack for func_8014AC38.
struct CfObjectImplPcRec90 {
    u8 _pad00[0x06];
    u8 field_06;
    u8 _pad07[0x0D - 0x07];
    u8 field_0D;
    u16 field_10;
    u16 field_12;
    f32 field_14;
    u8 _pad18[0x20 - 0x18];
};

int func_800CA294(cf::CfObjectImplPc* self);

// Data overlay over the battle object's arts container (+0x08): pending
// flag word at +0x1528, forwarded through the container's slot 0x20.
struct CfObjectImplPcArtsData {
    u8 _pad00[0x1528];
    u32 field_1528;
};

// Retail never caches field_18 across calls: every access reloads it
// (lwz 0x18(r30)). Textually expand every reference so MWCC allocates a
// fresh reload per use.
#define obj self->field_18

// Battle-state change driver: dispatches on the battle id (+0x3F28), fires
// arts commands through the +0x08 container, replays missing per-battle
// setup effects from a copied handler table, then runs the big flag ladder
// (0x1c/0x1d/0x1a/0x19/0x4000000) selecting between the landing-reset and
// raise paths, and finally recounts via func_800CA294.
void func_800C5B00(cf::CfObjectImplPc* self)
{
    func_800CAB2C(self);
    // Retail reads the battle id once and dispatches with an unsigned
    // equality chain (cmplwi), not a jump table.
    u16 mode = self->field_18->field_3F28;
    if (mode == 1) {
        // Battle start: replay arts 0xea while the state word (slot 0x160)
        // is still below the idle threshold.
        if (func_80148778(&obj->mArts, 0xea)) {
            if (obj->v160() < lbl_eu_80666BC8) {
                obj->mArts.vf20(0xea);
            }
        }
        goto L_tail;
    } else if (mode == 2) {
        // Solo battle: exactly one actor in the +0x28 list, none queued at
        // +0x48, then replay arts 0xeb.
        cf::CBattleManager* bm = cf::CBattleManager::getInstance();
        cf::CfBattleChainNode* head = &bm->mListA;
        cf::CfBattleChainNode* p = head->mNext;
        int n = 0;
        while (p != head) {
            p = p->mNext;
            n++;
        }
        if (n != 1) {
            goto L_tail;
        }
        head = &bm->mListHead;
        p = head->mNext;
        n = 0;
        while (p != head) {
            p = p->mNext;
            n++;
        }
        if (n != 0) {
            goto L_tail;
        }
        if (func_80148778(&obj->mArts, 0xeb)) {
            obj->mArts.vf20(0xeb);
        }
        goto L_tail;
    } else if (mode == 5) {
        // Multi battle: once the state word passes the threshold, register
        // the arts-0xf1 status record, gate the lists, replay arts 0xef,
        // then run the shared setup-effect table below.
        if (obj->v160() >= lbl_eu_80666BC8) {
            if (func_80148778(&obj->mArts, 0xf1) == 0) {
                u32 id = *obj->field_04->vf30();
                if (func_80174C98(obj, &id, 6) != 0) {
                    CfObjectImplPcRec90 rec;
                    memset((u8*)&rec + 4, 0, 0xe);
                    memset(&rec, 0, sizeof(rec));
                    rec.field_06 = 6;
                    rec.field_0D = 6;
                    rec.field_10 = 0;
                    rec.field_12 = 0;
                    rec.field_14 = lbl_eu_80666BCC;
                    func_8014AC38(&obj->field_3380, &rec);
                }
            }
        }
        {
            cf::CBattleManager* bm = cf::CBattleManager::getInstance();
            cf::CfBattleChainNode* head = &bm->mListA;
            cf::CfBattleChainNode* p = head->mNext;
            int n = 0;
            while (p != head) {
                p = p->mNext;
                n++;
            }
            if (n != 1) {
                goto L_tail;
            }
            head = &bm->mListHead;
            p = head->mNext;
            n = 0;
            while (p != head) {
                p = p->mNext;
                n++;
            }
            if (n != 0) {
                goto L_tail;
            }
        }
        if (func_80148778(&obj->mArts, 0xef)) {
            obj->mArts.vf20(0xef);
        }
        goto L_table;
    } else if (mode == 7) {
        // Duel vs queue: when flag 6 is live and arts 0x10a is absent,
        // forward the arts container's pending flag and notify the manager;
        // otherwise only when the state word is still low does a present
        // 0x10a entry get reported.
        if (obj->v160() >= lbl_eu_80666BC8) {
            u32 id = *obj->field_04->vf30();
            if (func_80174C98(obj, &id, 6) != 0 &&
                func_80148778(&obj->mArts, 0x10a) == 0) {
                if (reinterpret_cast<CfObjectImplPcArtsData*>(&obj->mArts)
                        ->field_1528
                    != 0) {
                    obj->mArts.vf20(
                        reinterpret_cast<CfObjectImplPcArtsData*>(&obj->mArts)
                            ->field_1528);
                }
                func_800F38E0(cf::CBattleManager::getInstance(), obj, 0x10a);
                goto L_table;
            }
        }
        if (obj->v160() < lbl_eu_80666BC8) {
            if (func_80148778(&obj->mArts, 0x10a)) {
                func_800F3958(cf::CBattleManager::getInstance(), obj, 0x10a);
            }
        }
        goto L_table;
    }

L_table:
{
    // Per-battle setup effects: copy the six handler ids, count how many of
    // the three player slots requested each one, and fire whatever nobody
    // requested through the arts container check.
    u32 ids[6] = {lbl_eu_804FC718[0], lbl_eu_804FC718[1], lbl_eu_804FC718[2],
                  lbl_eu_804FC718[3], lbl_eu_804FC718[4], lbl_eu_804FC718[5]};
    int counts[6] = {0, 0, 0, 0, 0, 0};
    for (u32 i = 0; i < 3; i++) {
        int v = func_801B1CCC(i);
        if (v == 6) {
            counts[0]++;
        } else if (v == 4) {
            counts[1]++;
        } else if (v == 7) {
            counts[2]++;
        } else if (v == 8) {
            counts[3]++;
        } else if (v == 9) {
            counts[4]++;
        } else if (v == 5) {
            counts[5]++;
        }
    }
    for (u32 j = 0; j < 6; j++) {
        if (counts[j] == 0) {
            if (func_80148778(&self->field_18->mArts, (int)ids[j])) {
                func_800F3958(cf::CBattleManager::getInstance(), self->field_18,
                              (s32)ids[j]);
            }
        }
    }
}
L_tail:
    obj = self->field_18;
    if (obj->v12C() == lbl_eu_80666BCC) {
        goto L_end;
    }
    {
        u32 f1c = *obj->field_04->vf30();
        if (func_80174C98(obj, &f1c, 0x1c)) {
            obj->v234();
            cf::CfGameManager::getInstance();
            if (isGlobalCamFlagSet(0x04000000) == 0) {
                if (func_800B8C78(0x9c5)) {
                    u8 h58[8];
                    func_80043D90(h58);
                    func_800F4A98(func_80043F18(h58), 0x20, 0x800);
                    if (((cf::CfEnumList*)func_80043F18(h58))->field_620 != 0) {
                        __dt__80043E88(h58, -1);
                    } else {
                        // No other actors queued: drop every player into the
                        // battle-start state.
                        func_80084654__Q22cf13CfGameManagerFv(0);
                        for (u32 i = 0; i < 3; i++) {
                            cf::CfObjectImplPc18* pl =
                                reinterpret_cast<cf::CfObjectImplPc18*>(func_8016FE34(
                                    (u8*)cf::CfGameManager::getPlayer(i)));
                            if (pl != 0) {
                                u32 pid = *pl->field_04->vf30();
                                if (func_80174C98(pl, &pid, 0x1c) == 0) {
                                    func_800BE12C(
                                        reinterpret_cast<cf::CfObjectImplPcSub3E9C*>(
                                            (u8*)pl + 0x3e9c),
                                        5, 0, -1, 1);
                                }
                            }
                        }
                        __dt__80043E88(h58, -1);
                        goto L_end;
                    }
                }
            }
            // Shared gates: no queued actors, embedded sub-object is not
            // player 0's, and the paired object reports a live target.
            {
                cf::CBattleManager* bm = cf::CBattleManager::getInstance();
                cf::CfBattleChainNode* head = &bm->mListHead;
                cf::CfBattleChainNode* p = head->mNext;
                int n = 0;
                while (p != head) {
                    p = p->mNext;
                    n++;
                }
                if (n != 0) {
                    goto L_end;
                }
            }
            cf::CfObjectImplPcSub3E9C* me =
                obj != 0 ? &obj->mSub : (cf::CfObjectImplPcSub3E9C*)0;
            if ((void*)me == (void*)cf::CfGameManager::getPlayer(0)) {
                goto L_end;
            }
            if (obj->field_3F60 == 0) {
                goto L_end;
            }
            if (reinterpret_cast<cf::CfObjectImplPc3F60V80*>(obj->field_3F60)
                    ->vf80(0)
                == 0) {
                goto L_end;
            }
            cf::CfObjectImplPc18* act = reinterpret_cast<cf::CfObjectImplPc18*>(
                func_8016FE34((u8*)cf::CfGameManager::getPlayer(0)));
            u32 aid = *act->field_04->vf30();
            if (func_80174C98(act, &aid, 0x802) == 0) {
                u32 aid2 = *act->field_04->vf30();
                if (func_80174C98(act, &aid2, 0x1d) == 0) {
                    goto L_end;
                }
            }
            cf::CfGameManager::getInstance();
            if (isGlobalCamFlagSet(0x10000000)) {
                goto L_end;
            }
            if (isGlobalCamFlagSet(0x02000000)) {
                goto L_end;
            }
            u8 h50[8];
            func_80043D90(h50);
            func_800F4A98(func_80043F18(h50), 0x20, 0x1000);
            if (((cf::CfEnumList*)func_80043F18(h50))->field_620 != 0) {
                obj->v11C(lbl_eu_80666BC8);
                obj->v314();
                func_800BE12C(&obj->mSub, 0x2f, 1, -1, 1);
                obj->mSub.v08(0x200);
            }
            __dt__80043E88(h50, -1);
            goto L_end;
        }
        // 0x1c clear path: four more flags must pass before either the
        // raise or the reset branch runs.
        if (!(obj->v128() <= lbl_eu_80666BCC)) {
            goto L_end;
        }
        u32 a = *obj->field_04->vf30();
        if (func_80174C98(obj, &a, 0x1c) == 0) {
            goto L_end;
        }
        u32 b = *obj->field_04->vf30();
        if (func_80174C98(obj, &b, 0x1d) == 0) {
            goto L_end;
        }
        u32 c = *obj->field_04->vf30();
        if (func_80174C98(obj, &c, 0x1a) == 0) {
            goto L_end;
        }
        u32 d = *obj->field_04->vf30();
        if (func_80174C98(obj, &d, 0x19) == 0) {
            goto L_end;
        }
        u32 e = *obj->field_04->vf30();
        if (func_80174C98(obj, &e, 0x04000000) == 0) {
            // Raise: push the battle-start state; when nothing is paired,
            // also arm the 0x1c flag for next time.
            func_800BE12C(&obj->mSub, 5, 0, -1, 1);
            if (obj->field_3F60 == 0) {
                func_80174B4C(obj, 0x1c);
            }
            return;
        }
        func_80174B4C(obj, 0x04000000);
        {
            void* q = obj->vf290();
            if (q != 0) {
                q = obj->vf290();
                if (q != 0) {
                    if (func_8026178C(q, 0x65) == 0) {
                        obj->vf304(0);
                    }
                }
            }
        }
        {
            // Landing reset: enumerate actors, replay each idler's landing
            // scale, then rebase the chain gauge from the 0x48 query rate.
            u8 h48[8];
            func_80043D90(h48);
            func_800F4A98(func_80043F18(h48), 0x20, 0);
            cf::CfEnumList* list = (cf::CfEnumList*)func_80043F18(h48);
            for (u32 i = 0; i < list->field_620; i++) {
                cf::CfObjectImplPc18* o2 = reinterpret_cast<cf::CfObjectImplPc18*>(
                    func_8016FE34(
                        func_800F6EAC((u8*)func_80043F18(h48), i)));
                if (o2 != self->field_18 && o2->vf308() == 4) {
                    o2->vf304(1);
                    cf::CfObjectImplPc2F4* t = o2->vf2F4();
                    u8* pb = (u8*)t + t->field_02;
                    o2->vf2FC((s32)(t->field_10 * pb[4]) / 2);
                }
            }
            func_800BE12C(&self->field_18->mSub, 5, 0, -1, 1);
            CfObjectImplPcEA444* res = reinterpret_cast<CfObjectImplPcEA444*>(
                func_800EA444(cf::CBattleManager::getInstance()));
            cf::CfGameManager::getInstance();
            bool runRest = false;
            if (isGlobalCamFlagSet(0x04000000) == 0 && res != 0) {
                u32 own = self->field_18->field_3F10;
                if (res->field_04 != own && res->field_00 != own) {
                    runRest = true;
                }
            }
            if (runRest) {
                cf::CfObjectImplPc18* ob = self->field_18;
                ob->vC8();
                ob->v230();
                ob->v17C();
                ob->v194(0);
                func_800E9B54(cf::CBattleManager::getInstance(), ob, 0, 0);
                func_800D9CA0(cf::CBattleManager::getInstance(), ob);
                u8 h40[8];
                func_80043D90(h40);
                func_800F4A98(func_80043F18(h40), 0x20, 0x800);
                void* q = ob->vf290();
                u32 w38 = 0;
                f32 fv34 = 0.0f;
                cf::CfObjectImplPcEffArg eff;
                if (q != 0) {
                    q = ob->vf290();
                    if (q != 0) {
                        if (func_80260264(q, 0x48, (s32*)&w38)) {
                            cf::CfEnumList* list2 =
                                (cf::CfEnumList*)func_80043F18(h40);
                            for (u32 i = 0; i < list2->field_620; i++) {
                                cf::CfObjectImplPc18* o2 =
                                    reinterpret_cast<cf::CfObjectImplPc18*>(
                                        func_8016FE34(func_800F6EAC(
                                            (u8*)func_80043F18(h40), i)));
                                func_800D81A8(0, o2, 0);
                                // Rebase the stored gauge off the magic
                                // double bias, then rescale by BD0.
                                s32 amt =
                                    (s32)(((f64)(u32)w38 - lbl_eu_80666BE0));
                                o2->v124(lbl_eu_80666BD0 * (f32)amt);
                            }
                        }
                    }
                }
                for (u32 i = 0;
                     i < ((cf::CfEnumList*)func_80043F18(h40))->field_620;
                     i++) {
                    cf::CfObjectImplPc18* o2 =
                        reinterpret_cast<cf::CfObjectImplPc18*>(func_8016FE34(
                            func_800F6EAC((u8*)func_80043F18(h40), i)));
                    if (o2->vf290() == 0) {
                        continue;
                    }
                    if (o2->vf290() == 0) {
                        continue;
                    }
                    if (func_80260518(o2->vf290(), 5, &w38, &fv34)) {
                        memset(&eff, 0, sizeof(eff));
                        eff.field_0C = 0x58;
                        eff.field_10 = w38;
                        eff.field_20 = fv34;
                        func_800EA9A8(cf::CBattleManager::getInstance(), o2,
                                      &eff, 5, 0);
                    }
                    if (func_80260518(o2->vf290(), 8, &w38, &fv34)) {
                        memset(&eff, 0, sizeof(eff));
                        eff.field_0C = 0x58;
                        eff.field_10 = w38;
                        eff.field_20 = fv34;
                        func_800EA9A8(cf::CBattleManager::getInstance(), o2,
                                      &eff, 8, 0);
                        eff.field_0C = 0x5b;
                        func_800EA9A8(cf::CBattleManager::getInstance(), o2,
                                      &eff, 8, 0);
                    }
                    if (func_80260518(o2->vf290(), 0x18, &w38, &fv34)) {
                        memset(&eff, 0, sizeof(eff));
                        eff.field_0C = 0x5d;
                        eff.field_10 = w38;
                        eff.field_20 = fv34;
                        func_800EA9A8(cf::CBattleManager::getInstance(), o2,
                                      &eff, 0x18, 0);
                    }
                    if (func_80260518(o2->vf290(), 0x9a, &w38, &fv34)) {
                        memset(&eff, 0, sizeof(eff));
                        eff.field_0C = 0x59;
                        eff.field_10 = w38;
                        eff.field_20 = fv34;
                        func_800EA9A8(cf::CBattleManager::getInstance(), o2,
                                      &eff, 0x9a, 0);
                    }
                    if (func_80260264(o2->vf290(), 0x62, (s32*)&w38)) {
                        if (((cf::CfEnumList*)func_80043F18(h40))->field_620
                            == 1) {
                            func_8018C820(
                                &cf::CBattleManager::getInstance()->unk194,
                                (s32)w38);
                        }
                    }
                }
                __dt__80043E88(h40, -1);
            }
            __dt__80043E88(h48, -1);
        }
    }
L_end:
    func_800CA294(self);
    if (self->field_378 != 0) {
        self->field_378->s14();
    }
}
#undef obj

// Battle-start/target-sync dispatcher: after the global scenario gates,
// resolves the current action target (own sub-object id or the enumerated
// battle list), notifies it via vtable 0x70 / 0x2C4, then runs the
// func_80174C98 flag ladder (0x803 / 0x400000 / 0x800000) that selects
// between list sync, player-target sync, and full reset paths.
void func_800C6A58(cf::CfObjectImplPc* self)
{
    cf::CfGameManager::getInstance();
    if (isGlobalCamFlagSet(0x04000000)) {
        return;
    }
    if (func_80496288(lbl_eu_80663E14) == lbl_eu_80666BCC) {
        return;
    }
    // NOTE: field_18 is deliberately never cached in a local - retail
    // reloads it (lwz 0x18(r31)) at every use and consumes the embedded
    // mSub via update-form loads.
    if (self->field_18->mSub.sf4C() != 0) {
        // Current target invalid: fall back to the actor behind its id.
        // Sync proceeds silently only when the actor exists, its validity
        // word (slot 0x160) is set, and either its state machine is idle
        // (v2BC == 0) or its +0x3F00 bit 30 is set; otherwise re-arm the
        // 0x800000 flag via the fallback path below.
        cf::CfObjectImplPc18* obj = reinterpret_cast<cf::CfObjectImplPc18*>(
            func_8016FE34((u8*)findObjectById((int)self->field_18->mSub.sf4C())));
        if (obj != 0 && obj->mSub.v160() != 0 &&
        (obj->v2BC() == 0 || (obj->field_3F00 & 2) != 0)) {
        goto L_flags;
    }
        // vtable offset 0x70 (declared index 26).
        self->vf68(0);
        func_80174B4C(self->field_18, 0x800000);
    } else {
        if (self->field_18->mSub.sf4C() == 0) {
            func_80174B4C(self->field_18, 0x800000);
        }
    }

    // Flag ladder: 0x803 must pass; 0x400000 short-circuits past 0x800000.
    // Each check keeps its own id local (retail uses a distinct stack slot
    // per store) and re-reads field_18 into a callee-saved local per use.
L_flags:
    cf::CfObjectImplPc18* battle;
    u32 f803;
    u32 f400;
    u32 f800;
    u32 f806a;
    u32 f806b;
    u32 f803b;
    battle = self->field_18;
    f803 = *battle->field_04->vf30();
    if (func_80174C98(battle, &f803, 0x803) == 0) {
        return;
    }
    battle = self->field_18;
    f400 = *battle->field_04->vf30();
    if (func_80174C98(battle, &f400, 0x400000) == 0) {
        battle = self->field_18;
        f800 = *battle->field_04->vf30();
        if (func_80174C98(battle, &f800, 0x800000) == 0) {
            return;
        }
    }

    u8 holder[8];
    func_80043D90(holder);
    func_800F4A98(func_80043F18(holder), 0x80000002, 0);
    u32 startId = self->vf40();
    func_800F6ED0(func_80043F18(holder), (void*)startId);
    if (((cf::CfEnumList*)func_80043F18(holder))->field_620 != 0) {
        // Non-empty list: reset iteration (holder passed directly), publish
        // the result via vtable 0x70, and forward entry 0's actor to 0x2C4.
        u32 resetId = (u32)func_800F6E08(holder);
        self->vf68(resetId);
        u8* item = (u8*)func_800F6EAC(func_80043F18(holder), 0);
        u8* target = item;
        if (target != 0) {
            target -= 0x3e9c;  // back up from the embedded mSub to the owner
        }
        self->field_18->vf2C4(target, lbl_eu_80666BCC, lbl_eu_80666BCC,
                              lbl_eu_80666BCC);
    } else {
        // Empty list: chase the player's current target instead.
        cf::CfObjectImplPc18* player =
            reinterpret_cast<cf::CfObjectImplPc18*>(
                getCfObjectPc(cf::CfGameManager::getPlayer(0)));
        cf::CfObjectImplPc18* tgt = reinterpret_cast<cf::CfObjectImplPc18*>(
            func_8016FE34((u8*)findObjectById((int)player->mSub.sf4C())));
        if (tgt != 0 && tgt->v2BC() == 0) {
            u32 tid = *tgt->field_04->vf30();
            if (func_80174C98(tgt, &tid, 0x100000) != 0) {
                self->vf68(player->mSub.sf4C());
                battle = self->field_18;
                f806a = *battle->field_04->vf30();
                if (func_80174C98(battle, &f806a, 0x806) == 0) {
                    func_8014B2DC(&self->field_18->field_3380);
                }
            } else {
                goto L_reset;
            }
        } else {
            // Shared tail: 0x806 picks a full state reset + target bounce,
            // otherwise 0x803 replays the landing dispatch.
L_reset:
            battle = self->field_18;
            f806b = *battle->field_04->vf30();
            if (func_80174C98(battle, &f806b, 0x806) != 0) {
                battle->field_3380.field_3594 = 0;
                battle->field_3380.field_3590 = 0;
                func_8014B2DC(&self->field_18->field_3380);
                func_800BE12C(&self->field_18->mSub, 0x31, 0, -1, 1);
            } else {
                battle = self->field_18;
                f803b = *battle->field_04->vf30();
                if (func_80174C98(battle, &f803b, 0x803) != 0) {
                    self->field_18->mSub.v08(0x200);
                }
            }
        }
    }
    __dt__80043E88(holder, -1);
}

void func_800C6EC0(cf::CfObjectImplPc* self, u32 param)
{
    if (self->field_378 != 0 && self->field_18->field_3F2C != 0) {
        self->field_378->vf10();
    }
    func_800CEE28(self, param);
}

// Arts-command feedback: when the battle-id gate passes and the 0x400000
// battle flag is clear, dispatch the command id (0xEA..0xF2 -> action ids)
// to the battle object's +0x3E9C sub-object (vtable 0x20C), then forward the
// original args to func_800CB9AC.
void func_800C6F30(cf::CfObjectImplPc* self, int arg2, int arg3, int arg4)
{
    if (arg2 != 0 && (u32)arg2 != (u32)self->field_18->field_3F60) {
        return;
    }
    cf::CfObjectImplPc18* battleObj = getCfObjectPc((cf::CfObjectMove*)(
        self->field_18 != 0 ? (cf::CfObjectImplPc18*)((u8*)self->field_18 + 0x3E9C)
                            : (cf::CfObjectImplPc18*)0));
    char* name = 0;
    int flag = 0;
    switch (arg3) {
    case 0:
    {
        // Laid out to match retail's dispatch order: tests, default jump,
        // slot-0 body, jump, slot-1 body, then the shared name check.
        if (arg4 == 0) {
            flag = 0;
            name = func_800BEDC4((u8*)&battleObj->mSub, 0);
        } else if (arg4 == 1) {
            flag = 1;
            name = func_800BEDC4((u8*)&battleObj->mSub, 1);
        }
        if (strcmp(name, lbl_eu_804FC758) == 0) {
            name = 0;
        }
        if (name != 0) {
            battleObj->mSub.sf104(name, flag);
            battleObj->field_45B0 = 1;
        }
        break;
    }
    case 1:
    {
        if (arg4 == 0) {
            flag = 0;
            name = func_800BED80((u8*)&battleObj->mSub, 0);
        } else if (arg4 == 1) {
            flag = 1;
            name = func_800BED80((u8*)&battleObj->mSub, 1);
        }
        if (strcmp(name, lbl_eu_804FC758) == 0) {
            name = 0;
        }
        if (name != 0) {
            battleObj->mSub.sf104(name, flag);
            battleObj->field_45B0 = 0;
        }
        break;
    }
    }
}

// Battle effect dispatch (the player-side twin of CfObjectImplMove's
// move-request driver). Resolves a manager/effect object per the request
// mode byte (+0x0A), attaches it to the battle object's embedded sub-object
// with the child id from slot 0xA8, passes the event's bit-7 flag through
// slot 0x194 and mirrors the actor's +0x3F34 data block. Mode 1 resolves
// the manager via the battle object's slot 0x220 and re-resolves the action
// target through the slot-0x298 entry table.
void func_800C70BC(cf::CfObjectImplPc* self, u32 id, CfObjectImplPcEvt* param)
{
    u32 child = 0;

    // Carried through the whole call: starts as the battle object, is
    // advanced onto its embedded +0x3E9C sub-object when attached, and is
    // finally reassigned to the resolved action-target actor.
    cf::CfObjectImplPc18* actor = self->field_18;

    u8 type7 = param->field_0E & 0x7F;    // low 7 bits: effect type
    u8 bit7 = (param->field_0E >> 7) & 1; // bit 7: flag passed to slot 0x194

    cf::CfObjectImplPcTgt* tgt = 0;
    if (type7 == 2 || type7 == 5 || type7 == 0x85 ||
        id == actor->field_3F64) {
        tgt = actor->field_4594;
    } else if (type7 == 3 || type7 == 6 || type7 == 0x86 ||
               id == actor->field_3F68) {
        tgt = actor->field_4598;
    }

    u8 mode = param->field_0A;
    if (mode == 0) {
        cf::CfObjectImplPcEffObj* mgr = (cf::CfObjectImplPcEffObj*)
            createNpcActor__Q22cf13CfGameManagerFv(param->field_0C);
        if (mgr == 0) {
            return;
        }
        bindPartnerO_(mgr, actor != 0 ? (u8*)&actor->mSub : 0, child);
        mgr->vf194(bit7);
        if (self->field_14->field_98 != 0) {
            setChild34Sc_(mgr, (u8*)self->field_14->field_98 + 0x304);
        }
    } else if (mode == 2) {
        cf::CfObjectImplPcEffObj* mgr = (cf::CfObjectImplPcEffObj*)
            createBattleActor__Q22cf13CfGameManagerFv(param->field_0C, 0);
        if (mgr == 0) {
            return;
        }
        bindPartnerO_(mgr, actor != 0 ? (u8*)&actor->mSub : 0, child);
        mgr->vf194(bit7);
        if (self->field_14->field_98 != 0) {
            setChild34Sc_(mgr, (u8*)self->field_14->field_98 + 0x304);
        }
    } else if (mode == 1) {
        cf::CfObjectImplPcEffObj* mgr = 0;
        if (type7 == 1) {
            self->field_18->v2A8();
            cf::CfObjectImplPc2A4Res* res = self->field_18->v29C(0);
            // Dispatch when the target query flags an active/queued state,
            // or - with no query result - for the 0x6b event on battle 6.
            if (res != 0 && (((res->field_74 & 1) != 0) ||
                             ((res->field_78 & 0x80) != 0))) {
                mgr = (cf::CfObjectImplPcEffObj*)actor->mSub.sf220(param->field_0C);
                if (mgr != 0) {
                    // Advance the carrier onto the embedded sub-object.
                    if (actor != 0) {
                        actor = (cf::CfObjectImplPc18*)((u8*)actor + 0x3E9C);
                    }
                    bindPartnerO_(mgr, actor, child);
                    mgr->vf194(bit7);
                    if (self->field_14->field_98 != 0) {
                        setChild34Sc_(mgr, (u8*)self->field_14->field_98 + 0x304);
                    }
                }
            } else if (res == 0 && actor->field_3F28 == 6 &&
                       param->field_0C == 0x6b) {
                mgr = (cf::CfObjectImplPcEffObj*)actor->mSub.sf220(param->field_0C);
                if (mgr != 0) {
                    if (actor != 0) {
                        actor = (cf::CfObjectImplPc18*)((u8*)actor + 0x3E9C);
                    }
                    bindPartnerO_(mgr, actor, child);
                    mgr->vf194(bit7);
                    if (self->field_14->field_98 != 0) {
                        setChild34Sc_(mgr, (u8*)self->field_14->field_98 + 0x304);
                    }
                }
            } else {
                return;
            }
        } else {
            mgr = (cf::CfObjectImplPcEffObj*)actor->mSub.sf220(param->field_0C);
            if (mgr != 0) {
                if (actor != 0) {
                    actor = (cf::CfObjectImplPc18*)((u8*)actor + 0x3E9C);
                }
                bindPartnerO_(mgr, actor, child);
                mgr->vf194(bit7);
                if (self->field_14->field_98 != 0) {
                    setChild34Sc_(mgr, (u8*)self->field_14->field_98 + 0x304);
                }
            }
        }
        if (mgr == 0) {
            return;
        }

        // Re-resolve the action target: an entry equal to the table's id
        // word wins outright; otherwise the first non-null entry is used.
        cf::CfObjectImplPc298Res* list = self->field_18->v298();
        bool resolved = false;
        u32* p = list->entries;
        for (u32 i = 0; i < 2 && !resolved; i++) {
            for (u32 j = 0; j < 8; j++) {
                if (p[j] == (u32)list->field_04) {
                    actor = (cf::CfObjectImplPc18*)
                        func_8016FE34((u8*)findObjectById((int)list->field_04));
                    resolved = true;
                    break;
                }
            }
            p += 8;
        }
        if (!resolved) {
            for (u32 i = 0; i < 16; i++) {
                if (list->entries[i] != 0) {
                    actor = (cf::CfObjectImplPc18*)
                        func_8016FE34((u8*)findObjectById((int)list->entries[i]));
                    break;
                }
            }
        }
        // Keep the actor only for effect types {1,4,5,6} / {0x85,0x86}
        // (mod-256 window tests, mirroring retail's addi/clrlwi pairs).
        if ((u8)(type7 + 0xFC) > 2) {
            if ((u8)(type7 + 0x7B) > 1) {
                if (type7 != 1) {
                    actor = 0;
                }
            }
        }
        if (actor != 0) {
            setTargetObj_(mgr, actor != 0 ? (u8*)&actor->mSub : 0);
            if (actor->field_3F34 != 0) {
                setChild34Sc_(mgr, (u8*)actor->field_3F34 + 0x304);
            }
        }
        mgr->vf194(bit7);
    }
}

// Player battle-event dispatcher: gates on the sender id matching the
// object's own battle id (+0x3F60), special-cases command 0x2F (arts
// confirm -> 0x1B state push), syncs the field_3F64/field_3F68 battle-id
// objects, forwards the event to func_800CD5DC, then dispatches on the
// command byte (jump table over 0x00-0x31, mostly no-ops).
void func_800C819C(cf::CfObjectImplPc* self, u32 id, u32 a3, u32 a4, u32 a5,
                   u32 a6)
{
    // Retail hoists the mSub address into a volatile reg before the gate
    // (addi r3, r9, 0x3E9C sits above the arg copies).
    cf::CfObjectImplPcSub3E9C* sub = &self->field_18->mSub;
    // Retail reloads field_18 per use; the gate shares one load (r9) for
    // both the +0x3F60 word and the mSub address.
    if (id != (u32)self->field_18->field_3F60) {
        return;
    }
    if (a5 == 0x2f) {
        // Flag word 0x200000 (retail: lis r4, 0x20).
        if (sub->v0C(0x200000) != 0) {
            self->field_18->mSub.v10();
            func_800BE12C(&self->field_18->mSub, 0x1b, 0, 6, 1);
            return;
        }
    }

    // Flag ladder entry 0x1A; result gates case 30.
    u32 flagWord = *self->field_18->field_04->vf30();
    int flagged = func_80174C98(self->field_18, &flagWord, 0x1a);

    // Sync both battle-id objects against their source: refresh the cached
    // word and re-run the pairing; when the source moved to a new state,
    // reset through func_8004CEF8 and pair again. field_18 is reloaded per
    // access (no cached pointers - retail holds none).
    if (self->field_18->field_3F64 != 0) {
        ((cf::CfObjectImplPc3F60*)self->field_18->field_3F64)->field_4B4 =
            func_8004B9B8((cf::CfObjectImplPc3F60*)self->field_18->field_3F60);
        func_8004B9D4((cf::CfObjectImplPc3F60*)self->field_18->field_3F64, a3,
                      0,
                      ((cf::CfObjectImplPc3F60*)self->field_18->field_3F60)
                          ->field_4C8,
                      0);
        if (((cf::CfObjectImplPc3F60*)self->field_18->field_3F60)->field_374 !=
            ((cf::CfObjectImplPc3F60*)self->field_18->field_3F64)->field_374) {
            func_8004CEF8((cf::CfObjectImplPc3F60*)self->field_18->field_3F64,
                          ((cf::CfObjectImplPc3F60*)self->field_18->field_3F60)
                              ->field_374);
            ((cf::CfObjectImplPc3F60*)self->field_18->field_3F64)->field_4B4 =
                func_8004B9B8(
                    (cf::CfObjectImplPc3F60*)self->field_18->field_3F60);
            func_8004B9D4(
                (cf::CfObjectImplPc3F60*)self->field_18->field_3F64, a3, 0,
                ((cf::CfObjectImplPc3F60*)self->field_18->field_3F60)->field_4C8,
                0);
        }
    }
    if (self->field_18->field_3F68 != 0) {
        ((cf::CfObjectImplPc3F60*)self->field_18->field_3F68)->field_4B4 =
            func_8004B9B8((cf::CfObjectImplPc3F60*)self->field_18->field_3F60);
        func_8004B9D4((cf::CfObjectImplPc3F60*)self->field_18->field_3F68, a3,
                      0,
                      ((cf::CfObjectImplPc3F60*)self->field_18->field_3F60)
                          ->field_4C8,
                      0);
        if (((cf::CfObjectImplPc3F60*)self->field_18->field_3F60)->field_374 !=
            ((cf::CfObjectImplPc3F60*)self->field_18->field_3F68)->field_374) {
            func_8004CEF8((cf::CfObjectImplPc3F60*)self->field_18->field_3F68,
                          ((cf::CfObjectImplPc3F60*)self->field_18->field_3F60)
                              ->field_374);
            ((cf::CfObjectImplPc3F60*)self->field_18->field_3F68)->field_4B4 =
                func_8004B9B8(
                    (cf::CfObjectImplPc3F60*)self->field_18->field_3F60);
            func_8004B9D4(
                (cf::CfObjectImplPc3F60*)self->field_18->field_3F68, a3, 0,
                ((cf::CfObjectImplPc3F60*)self->field_18->field_3F60)->field_4C8,
                0);
        }
    }

    func_800CD5DC(self, id, a3, a4, a5, a6);
    // Jump table spans 0x00-0x31; unmapped commands fall through to return.
    if (a5 > 0x31) {
        return;
    }
    switch (a5) {
    // Every value 0x00-0x31 is listed so MWCC emits the full 50-entry jump
    // table seen in retail (unlisted ids would trim the table bounds).
    case 0:
    case 3:
    case 4:
    case 8:
    case 9:
    case 10:
    case 12:
    case 13:
    case 14:
    case 15:
    case 16:
    case 18:
    case 19:
    case 20:
    case 21:
    case 22:
    case 23:
    case 24:
    case 25:
    case 26:
    case 28:
    case 31:
    case 32:
    case 33:
    case 34:
    case 35:
    case 36:
    case 37:
    case 38:
    case 39:
    case 40:
    case 41:
    case 42:
    case 43:
    case 44:
    case 45:
    case 46:
    case 47:
    case 48:
        break;
    case 1:
    {
        // Arts-state gate: only reacts when the paired battle object sits
        // in states 0xC/0xD/0xE and the battle phase matches. Bodies are
        // duplicated via gotos so MWCC keeps four separate BE12C sites
        // (retail does not tail-merge them).
        s32 st = self->field_18->field_3F60 != 0
                     ? ((cf::CfObjectImplPc3F60*)self->field_18->field_3F60)
                           ->field_374
                     : 0;
        if (st == 0xc) {
            goto L_st_c;
        }
        if (st == 0xd) {
            goto L_st_d;
        }
        if (st != 0xe) {
            goto L_case_end;
        }
        if (self->field_18->field_3F28 == 5 &&
            func_80148778(&self->field_18->mArts, 0xf1) == 0) {
            func_800BE12C(&self->field_18->mSub, 0x31, 0, -1, 1);
        }
        goto L_case_end;
    L_st_c:
        if (self->field_18->field_3F28 == 2) {
            if (func_80148778(&self->field_18->mArts, 0xeb) == 0) {
                func_800BE12C(&self->field_18->mSub, 0x31, 0, -1, 1);
            }
            goto L_case_end;
        }
        if (self->field_18->field_3F28 == 5) {
            if (func_80148778(&self->field_18->mArts, 0xef) == 0) {
                func_800BE12C(&self->field_18->mSub, 0x31, 0, -1, 1);
            }
        }
        goto L_case_end;
    L_st_d:
        if (self->field_18->field_3F28 == 5 &&
            func_80148778(&self->field_18->mArts, 0xf0) == 0) {
            func_800BE12C(&self->field_18->mSub, 0x31, 0, -1, 1);
        }
    L_case_end:
        break;
    }
    case 27:
        // Reset the per-battle action bookkeeping words.
        self->field_36C = 0;
        self->field_370 = 0;
        self->field_374 = 0;
        self->field_368 = 0;
        break;
    case 2:
    case 5:
    case 7:
    case 11:
    case 49:
        func_801BFE8C(0, 0x1bb, 0);
        break;
    case 17:
        cf::CfSoundMan::playActorSound(0, 0x1bb, 0, 0, lbl_eu_80666BC8);
        break;
    case 6:
        if (self->field_18->field_3F60 != 0 &&
            (((cf::CfObjectImplPc3F60*)self->field_18->field_3F60)->field_4EC &
             0x40000000) == 0) {
            func_80174B4C(self->field_18, 4);
            func_802A3074(func_8016FE34((u8*)self->field_14));
        }
        break;
    case 29:
        func_800F3970(cf::CBattleManager::getInstance(), self->field_18, 0,
                      0x14, 0);
        break;
    case 30:
        // Only fires when flag 0x1A was clear: replay landing scale for
        // state 4, drop to state 0 for state 1, otherwise notify 0x13.
        if (flagged == 0) {
            if (self->field_18->vf308() == 4) {
                self->field_18->vf304(3);
                cf::CfObjectImplPc2F4* t = self->field_18->vf2F4();
                // Scale byte selected by t's s16 offset, converted via
                // MWCC's implicit uint->double path, halved toward zero.
                u8* p = (u8*)t + t->field_02;
                self->field_18->vf2FC((s32)(t->field_10 * p[4]) / 2);
            } else if (self->field_18->vf308() == 1) {
                self->field_18->vf304(0);
            } else {
                func_800F3970(cf::CBattleManager::getInstance(),
                              self->field_18, 0, 0x13, 0);
            }
        }
        break;
    }
}

// Battle-actor scan: raises/clears the battle flags, enumerates actors into a
// scoped list notifying each via vtable 0x2C4, then syncs the battle object's
// action target id (vtable 0x4C/0x50 on the +0x3E9C sub-object) against the
// enumerated list or the player's current target. Early exits skip the tail
// (func_800C891C + vtable 0x80); retail relies on leftover registers for the
// "return" value, hence void.
void func_800C891C(cf::CfObjectImplPc* self);
void func_800C86E8(cf::CfObjectImplPc* self)
{
    self->field_18->field_04->vf20(0x400000);
    self->field_18->field_04->vf20(0x800000);
    u8 holder[8];
    func_80043D90(holder);
    func_800F4A98(func_80043F18(holder), 0x80000002, 0);
    u32 startId = self->vf40();
    func_800F6ED0(func_80043F18(holder), (void*)startId);
    // Unrotated in retail (init; b cond; body; incr; cond) - kept as a plain
    // while since MWCC emits identical bytes either way.
    u32 i = 0;
    while (i < ((cf::CfEnumList*)func_80043F18(holder))->field_620) {
        cf::CfObjectImplPc18* obj = (cf::CfObjectImplPc18*)func_8016FE34(
            func_800F6EAC(func_80043F18(holder), i));
        self->field_18->vf2C4((u8*)obj, lbl_eu_80666BCC, lbl_eu_80666BCC, lbl_eu_80666BCC);
        i++;
    }
    u32 listId = (u32)func_800F6E08(func_80043F18(holder));
    __dt__80043E88(holder, -1);
    if (self->field_18->mSub.sf4C() == 0) {
        self->field_18->mSub.sf50(listId);
        if (self->field_18->mSub.sf4C() == 0) {
            cf::CfObjectImplPc18* pobj = (cf::CfObjectImplPc18*)func_8016FE34(
                (u8*)findObjectById((int)getCfObjectPc(cf::CfGameManager::getPlayer(0))->mSub.sf4C()));
            if (pobj == 0) {
                return;
            }
            u32 id = *pobj->field_04->vf30();
            if (func_80174C98((u8*)pobj, &id, 0x100000) == 0) {
                return;
            }
            self->field_18->mSub.sf50(pobj->field_3F10);
            self->field_18->vf2C4((u8*)pobj, lbl_eu_80666BCC, lbl_eu_80666BCC, lbl_eu_80666BCC);
        }
    }
    func_800C891C(self);
    self->vf78();
}

// Player battle-start effect driver: gates on the 0x80000000 battle flag,
// resolves the current action-target actor (with a global-data fallback),
// then fires a series of status-effect requests driven by the battle
// object's +0x290 query object and the arts container entries 0xae/0xaf/0xb0.
// Two request families exist: the first six ids roll mtRand(100) against the
// query result, the last five fire unconditionally; the final 0x2e block
// chains three extra effect ids through the same buffer.
void func_800C891C(cf::CfObjectImplPc* self)
{
    // Retail caches field_18 in r28 only for the opening flag gate, then
    // reloads it per use.
    cf::CfObjectImplPc18* gate = self->field_18;
    u32 f80000000 = *gate->field_04->vf30();
    if (func_80174C98((u8*)gate, &f80000000, 0x80000000) != 0) {
        return;
    }
    func_80174B4C(self->field_18, 0x80000000);

    // Shared status-effect request buffer (retail keeps one 0x34-byte
    // stack record at sp+0xC and refills it with memset per request).
    // The query results are also shared locals so MWCC allocates single
    // callee-saved registers instead of a fresh slot per block.
    cf::CfObjectImplPcEffArg eff;
    int wA;
    int wB;
    f32 fv;

    // Resolve the current action target's actor; when missing, fall back to
    // the id stored in the global record returned by func_800FE68C.
    cf::CfObjectImplPc18* actor = reinterpret_cast<cf::CfObjectImplPc18*>(
        func_8016FE34(
            (u8*)findObjectById((int)self->field_18->mSub.sf4C())));
    if (actor == 0) {
        actor = reinterpret_cast<cf::CfObjectImplPc18*>(func_8016FE34((u8*)
            findObjectById(
                (int)reinterpret_cast<cf::CfObjectImplPcFe68C*>(func_800FE68C())
                    ->field_90E4)));
        if (actor == 0) {
            return;
        }
    }

    // Arts entry 0xae present: forward its magnitude through slot 0x2F8.
    if (func_80148778(&self->field_18->mArts, 0xae)) {
        u32 mag = ((cf::CfObjectImplPc149154*)func_80149154(
                       &self->field_18->mArts, 0xae))
                      ->field_10;
        self->field_18->w2F8(mag);
    }

    // Arts entries 0xaf / 0xb0 each build a fresh 0x34-byte status-effect
    // request (battle id, flag word 0x8000, payload from the arts magnitude)
    // and dispatch it twice (source + target both set to the battle object).
    if (func_80148778(&self->field_18->mArts, 0xaf)) {
        u32 mag = ((cf::CfObjectImplPc149154*)func_80149154(
                       &self->field_18->mArts, 0xaf))
                      ->field_10;
        cf::CfObjectImplPcEffArg eff;
        memset(&eff, 0, sizeof(eff));
        cf::CfObjectImplPc18* bt = self->field_18;
        eff.field_00 = bt->field_3F10;
        eff.field_0C = 0x44;
        eff.field_10 = mag;
        eff.field_20 = lbl_eu_80666BF8;
        eff.field_08 = 0x8000;
        cf::CBattleManager* mgr = cf::CBattleManager::getInstance();
        func_800EC918(mgr, bt, bt, &eff, 0);
    }
    if (func_80148778(&self->field_18->mArts, 0xb0)) {
        u32 mag = ((cf::CfObjectImplPc149154*)func_80149154(
                       &self->field_18->mArts, 0xb0))
                      ->field_10;
        cf::CfObjectImplPcEffArg eff;
        memset(&eff, 0, sizeof(eff));
        cf::CfObjectImplPc18* bt = self->field_18;
        eff.field_00 = bt->field_3F10;
        eff.field_0C = 0x45;
        eff.field_10 = mag;
        eff.field_20 = lbl_eu_80666BF8;
        eff.field_08 = 0x8000;
        cf::CBattleManager* mgr = cf::CBattleManager::getInstance();
        func_800EC918(mgr, bt, bt, &eff, 0);
    }

    // First gated block: a null query object aborts the whole function.
    void* q = self->field_18->vf290();
    if (q == 0) {
        return;
    }

    // Rolling blocks (ids 6/0xe/0x12/0x17/0x1c/0x26): every stage queries the
    // +0x290 object afresh, then mtRand(100) is rolled against the FDB8
    // result before firing the paired effect id.
#define PC891C_ROLL(id_, eff_)                                              \
    do {                                                                    \
        q = self->field_18->vf290();                                        \
        if (q != 0 && func_8026178C(q, id_) != 0) {                         \
            wA = func_8025FB10(self->field_18->vf290(), id_);               \
            if (wA != 0) {                                                  \
                wB = func_8025FDB8(self->field_18->vf290(), id_);           \
                if (wB != 0) {                                              \
                    fv = func_80260010(self->field_18->vf290(), id_);       \
                    if (fv != lbl_eu_80666BCC) {                            \
                        if ((s16)ml::math::mtRand(100) < (s16)wB) {         \
                            memset(&eff, 0, sizeof(eff));                   \
                            eff.field_10 = wA;                              \
                            eff.field_20 = fv;                              \
                            eff.field_0C = eff_;                            \
                            cf::CBattleManager* mgr =                       \
                                cf::CBattleManager::getInstance();          \
                            func_800EA9A8(mgr, self->field_18, &eff, id_,   \
                                          0);                               \
                        }                                                   \
                    }                                                       \
                }                                                           \
            }                                                               \
        }                                                                   \
    } while (0)

    // Fixed blocks (ids 7/0xf/0x19/0x9b): same ladder but no random roll.
#define PC891C_FIXED(id_, eff_)                                             \
    do {                                                                    \
        q = self->field_18->vf290();                                        \
        if (q != 0 && func_8026178C(q, id_) != 0) {                         \
            wA = func_8025FB10(self->field_18->vf290(), id_);               \
            if (wA != 0 && func_8025FDB8(self->field_18->vf290(), id_) != 0) { \
                fv = func_80260010(self->field_18->vf290(), id_);           \
                if (fv != lbl_eu_80666BCC) {                                \
                    memset(&eff, 0, sizeof(eff));                           \
                    eff.field_10 = wA;                                      \
                    eff.field_20 = fv;                                      \
                    eff.field_0C = eff_;                                    \
                    cf::CBattleManager* mgr =                               \
                        cf::CBattleManager::getInstance();                  \
                    func_800EA9A8(mgr, self->field_18, &eff, id_, 0);       \
                }                                                           \
            }                                                               \
        }                                                                   \
    } while (0)

    PC891C_ROLL(6, 0x58);
    PC891C_ROLL(0xe, 0x5b);
    PC891C_ROLL(0x12, 0x5a);
    PC891C_ROLL(0x17, 0x5d);
    PC891C_ROLL(0x1c, 0x5e);
    PC891C_ROLL(0x26, 0x5f);

    // Block 0x31 swaps the two query results (FB10 lands at +0x1C, FDB8 is
    // the roll threshold) and overrides the effect id with 2.
    do {
        q = self->field_18->vf290();
        if (q != 0 && func_8026178C(q, 0x31) != 0) {
            wB = func_8025FB10(self->field_18->vf290(), 0x31);
            if (wB != 0) {
                wA = func_8025FDB8(self->field_18->vf290(), 0x31);
                if (wA != 0) {
                    fv = func_80260010(self->field_18->vf290(), 0x31);
                    if (fv != lbl_eu_80666BCC) {
                        if ((s16)ml::math::mtRand(100) < (s16)wA) {
                            memset(&eff, 0, sizeof(eff));
                            eff.field_10 = wB;
                            eff.field_20 = fv;
                            eff.field_0C = 2;
                            cf::CBattleManager* mgr =
                                cf::CBattleManager::getInstance();
                            func_800EA9A8(mgr, self->field_18, &eff, 0x31, 0);
                        }
                    }
                }
            }
        }
    } while (0);

    // Slot 0x5C0 on the battle object consumes the resolved actor; a nonzero
    // result aborts. Then both the actor and the battle object report their
    // chain counts (slot 0x108); fewer than 3 apart also aborts.
    if (self->field_18->v368(actor) != 0) {
        return;
    }
    void* cntA = actor->v100();
    void* cntB = self->field_18->v100();
    if ((int)((char*)cntA - (char*)cntB) < 3) {
        return;
    }

    PC891C_FIXED(7, 0x58);
    PC891C_FIXED(0xf, 0x5b);
    PC891C_FIXED(0x19, 0x5d);
    PC891C_FIXED(0x9b, 0x59);

    // Final 0x2e block: like FIXED plus a kind byte at +0x3A, then the same
    // buffer replays effect ids 0x58/0x59/0x5a with kind 0.
    do {
        q = self->field_18->vf290();
        if (q != 0 && func_8026178C(q, 0x2e) != 0) {
            wA = func_8025FB10(self->field_18->vf290(), 0x2e);
            if (wA != 0 && func_8025FDB8(self->field_18->vf290(), 0x2e) != 0) {
                fv = func_80260010(self->field_18->vf290(), 0x2e);
                if (fv != lbl_eu_80666BCC) {
                    memset(&eff, 0, sizeof(eff));
                    eff.field_10 = wA;
                    eff.field_20 = fv;
                    eff.field_0C = 0x5f;
                    eff.field_2E = 0x2e;
                    cf::CBattleManager* mgr =
                        cf::CBattleManager::getInstance();
                    func_800EA9A8(mgr, self->field_18, &eff, 0x2e, 0);
                    eff.field_0C = 0x58;
                    mgr = cf::CBattleManager::getInstance();
                    func_800EA9A8(mgr, self->field_18, &eff, 0, 0);
                    eff.field_0C = 0x59;
                    mgr = cf::CBattleManager::getInstance();
                    func_800EA9A8(mgr, self->field_18, &eff, 0, 0);
                    eff.field_0C = 0x5a;
                    mgr = cf::CBattleManager::getInstance();
                    func_800EA9A8(mgr, self->field_18, &eff, 0, 0);
                }
            }
        }
    } while (0);
#undef PC891C_FIXED
#undef PC891C_ROLL
}

// Player battle-start reset: dispatches arts command 0xEA through the +0x08
// container, syncs the action target from v2A4, then clears per-battle state
// (0x3E98 / 0x3380 flags / 0x3590-94), resets the +0x3E9C sub-object's
// target, and replays the landing scale when the state machine is idle.
void func_800C969C(cf::CfObjectImplPc* self)
{
    if (func_80148778(&self->field_18->mArts, 0xea) != 0) {
        self->field_18->mArts.vf20(0xea);
    }
    if (self->field_18->v29C(0)->field_50 != 0) {
        if (self->field_18->v29C(0)->field_50->field_48 != 0) {
            // Named local forces retail's mr-copy of the fresh query result.
            cf::CfObjectImplPc2A4Res* res = self->field_18->v29C(0);
            self->vf98(res->field_50->field_48);
        }
    }
    if ((self->field_18->v29C(0)->field_78 & 0x400) != 0) {
        // Target vanished while the retry flag is set: re-dispatch its id.
        if (self->field_18->v2A8() == 0) {
            cf::CfObjectImplPc2A4Res* res = self->field_18->v29C(0);
            func_800CB9AC((u8*)self, res->field_50->field_48);
        }
    }
    self->field_18->mArts.vf20(0xf);
    self->field_18->mArts.vf30(0x100);
    // Retail loads lis r4, 0x40 / 0x80 -> flag words 0x400000 / 0x800000.
    self->field_18->field_04->vf20(0x400000);
    self->field_18->field_04->vf20(0x800000);
    self->field_18->mSub.sf50(0);

    // Zero held in a local: retail keeps it in r30 across the three stores.
    s32 cleared = 0;
    s32 enable = 1;
    self->field_18->field_3E98 = cleared;
    self->field_18->field_3380.field_08 &= 0xFFEF;
    cf::CfObjectImplPc3380* blk = &self->field_18->field_3380;
    blk->field_AFC = enable;
    blk->field_04 = enable;
    func_8014B2DC(blk);
    // One reload for both stores (retail shares the field_18 load here).
    cf::CfObjectImplPc3380& blk2 = self->field_18->field_3380;
    blk2.field_3594 = cleared;
    blk2.field_3590 = cleared;
    self->field_18->v2B0();
    if (self->field_18->field_3F28 == 7) {
        func_801B248C(0);
    }
    if (self->field_18->v2BC() == 0) {
        // Landing/state change: rescale via the table entry selected by the
        // s16 offset, converted through MWCC's uint->double path.
        if (self->field_18->vf308() == 4) {
            self->field_18->vf304(3);
            cf::CfObjectImplPc2F4* t = self->field_18->vf2F4();
            // Scale byte selected by t's s16 offset into the table at +0x04;
            // converted through MWCC's uint->double magic (lbl_eu_80666BD8).
            f32 scale = t->field_10;
            self->field_18->vf2FC((s32)(((u8*)t + t->field_02)[4] * scale));
        } else if (self->field_18->vf308() == 0) {
            self->field_18->vf304(1);
            cf::CfObjectImplPc2F4* t = self->field_18->vf2F4();
            f32 scale = t->field_10;
            self->field_18->vf2FC((s32)(((u8*)t + t->field_02)[4] * scale));
        }
    }
}

// Event dispatch (func_800C9A20): gate on the event id, run the arts-command
// path for ids 0xEA/0xF0/0x10A, then chain-gauge handling and the +0x290
// notification pipeline.
// Field accesses are deliberately not cached (no local id/obj copies): retail
// holds self in r30 and evt in r31 and reloads field_18 / field_0C per use.
void func_800C9A20(cf::CfObjectImplPc* self, CfObjectImplPcEvt* evt)
{
    // Flat short-circuit form reproduces retail's single beq-skip of the
    // whole gate block.
    if (func_80145F78(evt->field_0C) != 0 && (evt->field_2E != 0 || (evt->field_30 & 2) != 0)) {
            func_800F3970(cf::CBattleManager::getInstance(), self->field_18, 0, 0x15, 0);
    }
    if (evt->id() == 0x10a) {
        // slot 0x204: (id, 0, 0x61, 0, 0)
        self->field_18->mSub.sf204(0x1a, 0, 0x61, 0, 0);
    }
    // Retail lowers this as a linear equality chain whose three identical
    // bodies are appended after the tests (KB §1379): express it as an
    // explicit goto-chain so MWCC keeps the bodies out-of-line.
    if (evt->id() == 0xea) {
        goto L_ea;
    }
    if (evt->id() == 0xf0) {
        goto L_f0;
    }
    if (evt->id() == 0x10a) {
        goto L_10a;
    }
    goto L_chainEnd;
L_ea:
    func_800BF29C(&self->field_18->mSub, 0x66, lbl_eu_80666BC8, 0, lbl_eu_80666BF8, 0);
    goto L_chainEnd;
L_f0:
    func_800BF29C(&self->field_18->mSub, 0x66, lbl_eu_80666BC8, 0, lbl_eu_80666BF8, 0);
    goto L_chainEnd;
L_10a:
    func_800BF29C(&self->field_18->mSub, 0x66, lbl_eu_80666BC8, 0, lbl_eu_80666BF8, 0);
L_chainEnd:
    if (func_802799F0(&cf::CBattleManager::getInstance()->mChain, self->field_18) == 0) {
        if ((u32)(evt->id() - 0xF) <= 1 || evt->id() == 9 || evt->id() == 0xB) {
            if (self->field_18->vf308() == 4) {
                self->field_18->vf304(3);
                cf::CfObjectImplPc2F4* t = self->field_18->vf2F4();
                // Scale byte selected by t's s16 offset, converted through
                // MWCC's implicit uint->double path (0x43300000 magic),
                // single-precision scale, fctiwz, halved toward zero.
                u8* p = (u8*)t + t->field_02;
                s32 half = (s32)(t->field_10 * p[4]) / 2;
                self->field_18->vf2FC(half);
            } else if (self->field_18->vf308() == 1) {
                self->field_18->vf304(0);
            } else {
                func_800F3970(cf::CBattleManager::getInstance(), self->field_18, 0, 0x11, 0);
            }
        }
        s32 gauge;
        if (self->field_18->vf290() != 0 && func_80260264(self->field_18->vf290(), 0x63, &gauge) != 0 &&
            func_80145C00(evt->field_0C)) {
            func_8018C820(&cf::CBattleManager::getInstance()->unk194, gauge);
        }
    }
    func_800CAB30(self, evt);
}

// Arts-command event dispatch (called with the battle object's +0x3F60 gate
// live): forwards the command id to vtable 0x94, runs per-id handling on the
// battle object / its +0x3E9C sub-object, then refreshes the 0x66 entry for
// ids 0xEA/0xF0/0x10A and chains to func_800CB454.
void func_800C9CEC(cf::CfObjectImplPc* self, CfObjectImplPcEvt* evt)
{
    if (self->field_18->field_3F60 == 0) {
        return;
    }
    self->vf94(evt->field_0C);
    // Retail lowers the dispatch as a linear equality chain with the case
    // bodies appended out-of-line in order (KB 1379); express it literally.
    if (evt->id() == 0xea) {
        goto L_ea;
    }
    if (evt->id() == 0xeb) {
        goto L_eb;
    }
    if (evt->id() == 0xef) {
        goto L_ef;
    }
    if (evt->id() == 0xf0) {
        goto L_f0;
    }
    if (evt->id() == 0xf1) {
        goto L_f1;
    }
    if (evt->id() == 0xf8) {
        goto L_f8;
    }
    if (evt->id() == 0x10a) {
        goto L_10a;
    }
    goto L_default;
L_ea:
    // Battle start: clear the idle flag word via vtable 0x27C.
    if (self->field_18->field_3F28 == 1) {
        self->field_18->vf27C()->field_00 = 0;
    }
    goto L_default;
L_eb:
    if (reinterpret_cast<cf::CfObjectImplPcSub3E9CData*>(&self->field_18->mSub)->field_8C == 2 &&
        reinterpret_cast<cf::CfObjectImplPcSub3E9CData*>(&self->field_18->mSub)
            ->field_C4->field_374 != 0xb) {
        func_800BE12C(&self->field_18->mSub, 0x31, 0, -1, 1);
    }
    goto L_default;
L_ef:
    {
        cf::CfObjectImplPcSub3E9CData* sub = reinterpret_cast<cf::CfObjectImplPcSub3E9CData*>(
            &self->field_18->mSub);
        if (sub->field_8C == 5) {
            s32 state = sub->field_C4->field_374;
            if (state != 0xb) {
                if (state == 0xc) {
                    func_800BE12C(sub, 0x31, 0, -1, 1);
                } else {
                    func_8004CEF8(sub->field_C4, 0xb);
                }
            }
        }
    }
    goto L_default;
L_f0:
    {
        cf::CfObjectImplPcSub3E9CData* sub = reinterpret_cast<cf::CfObjectImplPcSub3E9CData*>(
            &self->field_18->mSub);
        if (sub->field_8C == 5 && sub->field_C4->field_374 != 0xb) {
            func_800BE12C(sub, 0x31, 0, -1, 1);
            func_802A300C(self->field_18);
        }
    }
    goto L_default;
L_f1:
    {
        cf::CfObjectImplPcSub3E9CData* const sub = reinterpret_cast<cf::CfObjectImplPcSub3E9CData*>(
            &self->field_18->mSub);
        if (sub->field_8C == 5 && sub->field_C4->field_374 != 0xb) {
            func_800BE12C(sub, 0x31, 0, -1, 1);
        }
    }
    goto L_default;
L_f8:
    {
        cf::CfObjectImplPc18* obj = self->field_18;
        if (obj->v2BC() == 0) {
            func_800BE12C(&obj->mSub, 0x2f, 0, -1, 1);
            // Re-dispatch a synthetic 0x93 event (float payload swapped)
            // into the battle manager.
            CfObjectImplPcEvt copy = *evt;
            copy.field_0C = 0x93;
            copy.field_20 = lbl_eu_80666BFC;
            cf::CBattleManager* mgr = cf::CBattleManager::getInstance();
            func_800EC8FC(mgr, obj, &copy, 0);
            mgr = cf::CBattleManager::getInstance();
            // Count nodes in the intrusive list rooted at +0x48; skip when
            // any actor holds a chain entry.
            u32 count = 0;
            for (cf::CfBattleChainNode* n = mgr->mListHead.mNext; n != &mgr->mListHead;
                 n = n->mNext) {
                count++;
            }
            if (count == 0) {
                // Fresh reload of field_18 (retail: lwz 0x18(r30)); also
                // forces the sub-object pointer's definition after the
                // calls above.
                cf::CfObjectImplPc18* obj2 = self->field_18;
                // Retail keeps the null-check on the battle object before
                // stepping into the embedded sub-object.
                cf::CfObjectImplPcSub3E9C* me = obj2 ? &obj2->mSub : 0;
                if (cf::CfGameManager::getPlayer(0) ==
                    reinterpret_cast<cf::CfObjectMove*>(me)) {
                    // Retail re-derives the sub-object from self for the
                    // dispatch (reload + update-indexed vtable load).
                    self->field_18->mSub.v08(0x200);
                }
            }
        }
    }
    goto L_default;
L_10a:
    self->field_18->vf154(lbl_eu_80666C00);
L_default:
    // Three ids share the same body; retail emits them as duplicated
    // out-of-line blocks after an equality chain (KB 1379), so keep the
    // bodies separate via explicit gotos.
    if (evt->id() == 0xea) {
        goto L_refresh66_a;
    }
    if (evt->id() == 0xf0) {
        goto L_refresh66_b;
    }
    if (evt->id() == 0x10a) {
        goto L_refresh66_c;
    }
    goto L_chain;
L_refresh66_a:
    func_800BF2B0(&self->field_18->mSub, 0x66, 0);
    goto L_chain;
L_refresh66_b:
    func_800BF2B0(&self->field_18->mSub, 0x66, 0);
    goto L_chain;
L_refresh66_c:
    func_800BF2B0(&self->field_18->mSub, 0x66, 0);
L_chain:
    func_800CB454(self, evt);
}

// Arts command dispatch: promotes id 0xCF to 0xD0 while the battle object's
// +0x3374 bit-0x8000 flag is set, then asks func_80148778 whether the arts
// container holds the id; when missing, bounces to the object's vtable[0x94].
void func_800CA084(cf::CfObjectImplPc* self, u32 param)
{
    if (param == 0xcf && (self->field_18->field_3374 & 0x8000) != 0) {
        param = 0xd0;
    }
    if (func_80148778(&self->field_18->mArts, (int)param) == 0) {
        self->vf94(param);
    }
}

// Arts command dispatch (jump table): when the 0x400000 battle flag is clear,
// map the command id 0xEA..0xF2 onto action ids sent to the battle object's
// +0x3E9C sub-object (vtable 0x20C), then forward the args to func_800CB9AC.
void func_800CA104(cf::CfObjectImplPc* self, u32 param)
{
    cf::CfGameManager::getInstance();
    // Retail loads lis r3, 0x400 -> the flag word passed is 0x04000000.
    if (isGlobalCamFlagSet(0x04000000) != 0) {
        return;
    }
    switch (param - 0xea) {
    case 0:
        self->field_18->mSub.sf20C(0x15);
        break;
    case 1:
        self->field_18->mSub.sf20C(0x16);
        break;
    case 2:
        self->field_18->mSub.sf20C(0x18);
        break;
    case 3:
        self->field_18->mSub.sf20C(0x17);
        break;
    case 4:
        self->field_18->mSub.sf20C(0x19);
        break;
    case 5:
        self->field_18->mSub.sf20C(0x1a);
        break;
    case 6:
        self->field_18->mSub.sf20C(0x1b);
        break;
    case 7:
        self->field_18->mSub.sf20C(0x16);
        break;
    // Grouping 0x22 with case 8 raises the jump-table bounds to retail's
    // 35-entry table without emitting an extra block (an empty case would be
    // folded into default).
    case 8:
    case 0x22:
        self->field_18->mSub.sf20C(0x27);
        break;
    }
    func_800CB9AC((u8*)self, param);
}

void func_800CEA34(void*);

void func_800CA274(void* self, int value)
{
    CfObjectImplPcSubObj* sub = static_cast<CfObjectImplPcSubObj*>(static_cast<CfObjectImplPcData*>(self)->mSubObj);
    if (sub->mSomeValue != 0 && value != 0) {
        sub->mSomeValue = value;
    }
    func_800CEA34(self);
}

// Battle-actor recount: clears the +0x37C count, recounts actors whose
// +0x3E9C sub-object's action target (vtable 0x4C) matches the battle
// object's +0x3F10 id, and when the count grew, cancels/refreshes the
// actor's actions (vtable 0x210 / 0x208 / 0x204) and plays a sound if
// nothing was active. Returns 1 when the count grew, else 0.
int func_800CA294(cf::CfObjectImplPc* self)
{
    u32 v = 0;
    u32 prev = self->field_37C;
    self->field_37C = 0;
    u8 holder[8];
    func_80043D90(holder);
    func_800F4A98(func_80043F18(holder), 0x80000000, 0x800);
    for (u32 i = 0; i < ((cf::CfEnumList*)func_80043F18(holder))->field_620; i++) {
        cf::CfObjectImplPc18* obj = (cf::CfObjectImplPc18*)func_8016FE34(
            func_800F6EAC(func_80043F18(holder), i));
        if (obj->mSub.sf4C() == self->field_18->field_3F10) {
            self->field_37C++;
        }
    }
    if (self->field_37C > prev) {
        v = self->field_18->mSub.sf210(0x11);
        self->field_18->mSub.sf208(0x11);
        self->field_18->mSub.sf204(0x11, 0, -1, 0, 0);
        if (v == 0) {
            cf::CfSoundMan::playActorSound(0, 0x5c, 0, 0, lbl_eu_80666BC8);
        }
        __dt__80043E88(holder, -1);
        return 1;
    }
    if (self->field_37C == 0) {
        self->field_18->mSub.sf208(0x11);
    }
    __dt__80043E88(holder, -1);
    return 0;
}

// Tail-calls the +0x378 sub-object's vtable[0x0C] when the sub-object
// exists, forwarding args 1, 3, 4, 5 (arg 2 is not forwarded).
void func_800CA42C(cf::CfObjectImplPc* self, u32 a, u32 b, u32 c, u32 d, u32 e)
{
    if (self->field_378 != 0) {
        self->field_378->vf0C(a, c, d, e);
    }
}

// Per-frame player update: runs func_800CEBE0, raises the battle object's
// +0x3F60 flag bit 0x400000, and while the +0x14 object is in state 8 past
// scenario 0x167, notifies the +0x98 sub-object with the string at +0x3B.
void func_800CA458(cf::CfObjectImplPc* self)
{
    func_800CEBE0(self);
    self->field_18->field_3F60->field_0C |= 0x400000;
    if (self->field_14->field_8C == 8) {
        if (getQueuedFileEventCount__Q22cf13CfGameManagerFv() >= 0x167) {
            self->field_14->field_98->vf28((const char*)(lbl_eu_804FC758 + 0x3b), 0);
        }
    }
}

// Adjuster thunk for CfObjectImplPc virtual destructor at vtable slot this-0xc.
// Adjusts this pointer to the complete object, then tail-calls the real destructor.
extern "C" void func_800CA4DC(void* self)
{
    __dt__Q22cf14CfObjectImplPcFv((u8*)self - 0xc);
}

// Adjuster thunk for CfObjectImplPc virtual destructor at vtable slot this-0x10.
// Adjusts this pointer to the complete object, then tail-calls the real destructor.
extern "C" void func_800CA4E4(void* self)
{
    __dt__Q22cf14CfObjectImplPcFv((u8*)self - 0x10);
}

// Battle event command dispatcher (request mode 0x12): after the scenario
// token gate, dispatches on the ASCII command name in the event record
// (+0x1C) against offsets into the string table at lbl_eu_804FC758. Each
// command drives a different mix of actor refresh (func_8016FE34 of
// findObjectById over field_3E5C), battle-manager notifications, arts
// queries and status-effect structs. Every exit shares one tail that
// replays the landing sound for battle state 4.
void func_800C75D4(cf::CfObjectImplPc* self, u32 token,
                   cf::CfObjectImplPcEvtCmd* evt)
{
    func_800CC964();
    if (token != self->field_14->field_C4) {
        return;
    }
    if (evt->field_0A != 0x12) {
        goto L_tail;
    }
    do {
        // Fired for every mode-0x12 event regardless of which command matches.
        reinterpret_cast<cf::CfObjectImplPcSubV10*>(&self->field_18->mSub)
            ->vf10(0xFFFFFDFF);
        const char* cmd = evt->cmd;

        if (strcmp(cmd, lbl_eu_804FC758 + 1) == 0) {
            // Self-target guard: the embedded sub-object must not be player 0.
            cf::CfObjectImplPc18* owner = self->field_18;
            cf::CfObjectImplPcSub3E9C* me = owner != 0 ? &owner->mSub : 0;
            if ((void*)me == (void*)cf::CfGameManager::getPlayer(0)) {
                break;
            }
            // State machine must sit in states 7..9 and no cancel flags set.
            cf::CfObjectImplPc298Res* res = self->field_18->v298();
            if (self->field_18->v298()->field_48 < 7) {
                break;
            }
            if (self->field_18->v298()->field_48 > 9) {
                break;
            }
            if ((res->field_74 & 0x62) != 0) {
                break;
            }
            func_800983B8(self->field_18->mSub.v110(), 5);
            break;
        }

        if (strcmp(cmd, lbl_eu_804FC758 + 7) == 0) {
            // Actor refresh; retail duplicates the F3970 notify (once in the
            // non-idle branch, once unconditionally).
            cf::CfObjectImplPc18* res = reinterpret_cast<cf::CfObjectImplPc18*>(
                func_8016FE34((u8*)findObjectById(
                    (int)self->field_18->field_3380.field_ADC)));
            if (res == 0 || (res->field_3F00 & 0x40000000) == 0) {
                break;
            }
            if (res->vf308() >= 2) {
                break;
            }
            if (res->vf308() != 0) {
                func_800F3970(cf::CBattleManager::getInstance(), self->field_18,
                              res, 6, 0);
            } else {
                res->v314();
            }
            func_800F3970(cf::CBattleManager::getInstance(), self->field_18,
                          res, 6, 0);
            addTableValueWithClamp__Q22cf13CfGameManagerFv(self->field_18->field_3F28,
                                                   res->field_3F28, 0x14);
            u32 f6 = *res->field_04->vf30();
            bool hit = func_80174C98(res, &f6, 6) != 0;
            if (!hit) {
                u32 f9 = *res->field_04->vf30();
                hit = func_80174C98(res, &f9, 9) != 0;
            }
            if (hit) {
                func_800BE12C(&res->mSub, 0x1b, 0, 6, 1);
            }
            if (self->field_18->vf290() == 0) {
                break;
            }
            // Battle 1: build the 0x34-byte status-effect request and fire it
            // four times with ids 0x5f/0x58/0x59/0x5a.
            if (self->field_18->field_3F28 == 1 && res->vf290() != 0 &&
                res->vf290() != 0) {
                u32 effW;
                f32 effF;
                if (func_80260518(res->vf290(), 0x2f, &effW, &effF)) {
                    cf::CfObjectImplPcEffArg eff;
                    memset(&eff, 0, sizeof(eff));
                    eff.field_0C = 0x5f;
                    eff.field_10 = effW;
                    eff.field_20 = effF;
                    eff.field_2E = 0x2e;
                    func_800EA9A8(cf::CBattleManager::getInstance(), res, &eff,
                                  0x2e, 0);
                    eff.field_0C = 0x58;
                    func_800EA9A8(cf::CBattleManager::getInstance(), res, &eff,
                                  0, 0);
                    eff.field_0C = 0x59;
                    func_800EA9A8(cf::CBattleManager::getInstance(), res, &eff,
                                  0, 0);
                    eff.field_0C = 0x5a;
                    func_800EA9A8(cf::CBattleManager::getInstance(), res, &eff,
                                  0, 0);
                }
            }
            if (self->field_18->field_3F28 == 0xb) {
                res->vf154(lbl_eu_80666BE8);
            }
            void* q43 = self->field_18->vf290();
            s32 out43;
            if (q43 != 0 && func_80260264(q43, 0x43, &out43)) {
                if (res->v2BC() == 0) {
                    // Damage amount scaled by the idle rate, then fed back
                    // through v11C after the D81A8 cancel.
                    s32 amt = (s32)((f64)out43 *
                                    (res->v12C() / lbl_eu_80666BE8));
                    func_800D81A8(0, res, 0);
                    res->v11C((f32)((f64)amt * lbl_eu_80666BE0));
                }
            }
            s32 out60;
            if (func_80260264(self->field_18->vf290(), 0x60, &out60)) {
                addTableValueWithClamp__Q22cf13CfGameManagerFv(
                    self->field_18->field_3F28, res->field_3F28, (u32)out60);
            }
            break;
        }

        if (strcmp(cmd, lbl_eu_804FC758 + 0xC) == 0) {
            // Like +7 but drives the arts container with ids 9 / 0xb and
            // notifies with kind 7 (no duplicated call, no query ladder).
            cf::CfObjectImplPc18* res = reinterpret_cast<cf::CfObjectImplPc18*>(
                func_8016FE34((u8*)findObjectById(
                    (int)self->field_18->field_3380.field_ADC)));
            if (res == 0 || (res->field_3F00 & 0x40000000) == 0) {
                break;
            }
            res->v5A8();
            res->mArts.vf20(9);
            res->mArts.vf20(0xb);
            if (res->vf308() != 0) {
                func_800F3970(cf::CBattleManager::getInstance(), self->field_18,
                              res, 7, 0);
            } else {
                res->v314();
            }
            addTableValueWithClamp__Q22cf13CfGameManagerFv(self->field_18->field_3F28,
                                                   res->field_3F28, 0x14);
            break;
        }

        if (strcmp(cmd, lbl_eu_804FC758 + 0x12) == 0) {
            // Same shape as +0xC minus the arts dispatch, slot 0x5AC.
            cf::CfObjectImplPc18* res = reinterpret_cast<cf::CfObjectImplPc18*>(
                func_8016FE34((u8*)findObjectById(
                    (int)self->field_18->field_3380.field_ADC)));
            if (res == 0 || (res->field_3F00 & 0x40000000) == 0) {
                break;
            }
            res->v5AC();
            if (res->vf308() != 0) {
                func_800F3970(cf::CBattleManager::getInstance(), self->field_18,
                              res, 7, 0);
            } else {
                res->v314();
            }
            addTableValueWithClamp__Q22cf13CfGameManagerFv(self->field_18->field_3F28,
                                                   res->field_3F28, 0x14);
            break;
        }

        if (strcmp(cmd, lbl_eu_804FC758 + 0x17) == 0) {
            // Fixed-damage command: 100 HP scaled down by the 0x7a query,
            // pushed into the party gauge block at CBattleManager+0x194.
            cf::CfGameManager::getInstance();
            if (isGlobalCamFlagSet(0x04000000)) {
                break;
            }
            cf::CfObjectImplPc18* res = reinterpret_cast<cf::CfObjectImplPc18*>(
                func_8016FE34((u8*)findObjectById(
                    (int)self->field_18->field_3380.field_ADC)));
            if (res == 0 || (res->field_3F00 & 0x40000000) == 0) {
                break;
            }
            u32 w = *res->field_04->vf30();
            if (func_80174C98(res, &w, 0x1c) == 0) {
                break;
            }
            func_800BE12C(&res->mSub, 0x2f, 1, -1, 1);
            s32 dmg = 100;
            void* q7a = self->field_18->vf290();
            if (q7a != 0) {
                s32 o18;
                if (func_80260264(self->field_18->vf290(), 0x7a, &o18)) {
                    dmg = 100 - o18;
                }
            }
            func_8018C820(&cf::CBattleManager::getInstance()->unk194, -dmg);
            addTableValueWithClamp__Q22cf13CfGameManagerFv(self->field_18->field_3F28,
                                                   res->field_3F28, 0x14);
            func_802A2A74(self->field_18, res);
            func_802809C8();
            break;
        }

        if (strcmp(cmd, lbl_eu_804FC758 + 0x1E) == 0) {
            break;
        }

        if (strcmp(cmd, lbl_eu_804FC758 + 0x28) == 0) {
            // Chain-gauge accumulation over all three players.
            if ((self->field_18->field_3374 & 0x02000000) != 0) {
                break;
            }
            cf::CfGameManager::getInstance();
            if (isGlobalCamFlagSet(0x04000000)) {
                break;
            }
            f32 total = lbl_eu_80666BEC;
            if (func_80148778(&self->field_18->mArts, 0xc4)) {
                cf::CfObjectImplPc149154* p =
                    (cf::CfObjectImplPc149154*)func_80149154(
                        &self->field_18->mArts, 0xc4);
                total = lbl_eu_80666BD0 * (f64)p->field_10 + total;
            }
            if (self->field_18->vf290() != 0) {
                // Retail pins the rate divisor in a callee-saved FPR across
                // the whole player loop.
                f32 scale = lbl_eu_80666BE8;
                for (s32 i = 0; i < 3; i++) {
                    cf::CfObjectImplPc18* pl =
                        reinterpret_cast<cf::CfObjectImplPc18*>(func_8016FE34(
                            (u8*)cf::CfGameManager::getPlayer(i)));
                    if (pl == 0) {
                        continue;
                    }
                    void* pq = pl->vf290();
                    if (pq == 0) {
                        continue;
                    }
                    s32 w49;
                    if (func_80260264(pq, 0x49, &w49)) {
                        total += (f32)(((f64)w49 - lbl_eu_80666BE0) / scale);
                    }
                }
            }
            self->field_18->v11C(total * self->field_18->v12C());
            break;
        }

        if (strcmp(cmd, lbl_eu_804FC758 + 0x33) == 0) {
            // Summon command: resolve the summonable via RTTI, refresh its
            // gauge display, then pick a handler from the copied entry table
            // indexed by the current battle id.
            void* dc = __dynamic_cast((void*)self->field_18->field_3ED4, 0,
                                      (const void*)&lbl_eu_80661C60,
                                      (const void*)&lbl_eu_80661BE8,
                                      (void*)0);
            if (dc == 0) {
                break;
            }
            cf::CfObjectImplPc18* obj = reinterpret_cast<cf::CfObjectImplPc18*>(
                func_8016FE34((u8*)findObjectById(
                    (int)((cf::CfObjectImplPc18*)dc)->vFC())));
            if (obj == 0) {
                break;
            }
            if (func_80148778(&obj->mArts, 0x116) == 0) {
                obj->v11C(lbl_eu_80666BF0);
            }
            if (self->field_18->field_3F10 == 0) {
                obj->mSub.sf50(self->field_18->field_3F10);
                obj->vf2C4((u8*)self->field_18, lbl_eu_80666BF4,
                           lbl_eu_80666BCC, lbl_eu_80666BCC);
            }
            if (func_800DA06C(cf::CBattleManager::getInstance(), obj) == 0) {
                obj->mSub.v08(0x100);
            }
            // Retail materializes this local table by copying the hidden
            // const initializer at lbl_eu_804FC730 word-by-word.
            u32 tbl[9] = {lbl_eu_804FC730[0], lbl_eu_804FC730[1],
                          lbl_eu_804FC730[2], lbl_eu_804FC730[3],
                          lbl_eu_804FC730[4], lbl_eu_804FC730[5],
                          lbl_eu_804FC730[6], lbl_eu_804FC730[7],
                          lbl_eu_804FC730[8]};
            if (self->field_18->field_3F28 > 8) {
                break;
            }
            cf::CfObjectImplPcSub3E9C* tgt =
                obj != 0 ? &obj->mSub : (cf::CfObjectImplPcSub3E9C*)0;
            func_800451D8(tbl[self->field_18->field_3F28], tgt);
            break;
        }
        break;
    } while (0);
L_tail:
    // Shared tail: when the event targets the player's own battle object in
    // mode 0x12 with command +1 while battle state is 4, replay the landing
    // sound through the resolved actor info record.
    cf::CfObjectImplPc18* owner = self->field_18;
    cf::CfObjectImplPcSub3E9C* me = owner != 0 ? &owner->mSub : 0;
    if ((void*)me != (void*)cf::CfGameManager::getPlayer(0)) {
        return;
    }
    if (evt->field_0A != 0x12) {
        return;
    }
    if (strcmp(evt->cmd, lbl_eu_804FC758 + 1) != 0) {
        return;
    }
    cf::CfObjectImplPc18* bt = self->field_18;
    if (bt->field_3F28 != 4) {
        return;
    }
    cf::CfObjectImplPc298Res* res = bt->v298();
    bt = self->field_18;
    cf::CfObjectImplPc2A4Sub* sub = res->field_50;
    u8* info = func_8009D7E4(func_8009EC9C(bt->field_3F28) + 0x1c, 5);
    if (info[0x1c] != 0) {
        return;
    }
    if ((res->field_78 & 0x800) == 0) {
        return;
    }
    if (sub->field_76 >= 3) {
        return;
    }
    if ((res->field_74 & 0x62) != 0) {
        return;
    }
    func_80133F48(4, lbl_eu_80666BC8);
}
