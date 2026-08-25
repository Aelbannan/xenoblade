// Auto-scaffolded catalog TU for kyoshin/cf/CSuddenCommu
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/cf/CBattleManagerApi.hpp"
#include "kyoshin/cf/CfMapItemManager.hpp"
#include "kyoshin/harness_catalog.hpp"
#include "libs/monolib/src/scn/CScn_8049603C.hpp" // func_8049603C (single owner decl)
#include "monolib/scn/CScnTimeApi.hpp"
#include "kyoshin/cf/CSuddenCommu.hpp"
#include "kyoshin/cf/CfGameManager.hpp"
// ml::math::mtRand(int) - mangles to the retail mtRand__Q22ml4mathFi.
#include "monolib/math/Random.hpp"
// CBattleManager.hpp currently pulls a conflicting CfObjectActor.hpp decl
// (func_80149154 overload); use the standalone header chain instead.
#include "kyoshin/cf/CVision.hpp"
#include "kyoshin/cf/chain/CChainTime.hpp"
#include "kyoshin/cf/CfObjectEnumList.hpp"

namespace cf {}
using namespace cf;

// Same-TU siblings defined later in this file.
void func_801BB464(CSuddenCommu* self, int playerIdx, int mode, CSuddenCommuActor* player, int arg5);
void func_801BC474(CSuddenCommu* self);

cf::CSuddenCommu::CSuddenCommu() {}

void func_801BA1DC(){}

extern "C" void func_801BA250(void* self) { *(u32*)((u8*)self + 0x20) = 0; }

void func_801BA25C(){}

unsigned long func_801BA2C8(void* self) {
    unsigned long v = *(unsigned long*)((char*)self + 0x14);
    return !!v;
}

// Sudden-commu availability gate. Returns 0 while the commu state machine is
// live (field_14 non-zero) or the battle/voice system is busy, 1 when the
// commu may start. Read-only - no side effects on self.
__declspec(noinline) int func_801BA2DC(CSuddenCommu* self) {
    if (self->field_14 != 0) return 0;
    bool voiceRes = (func_8009CF8C(0x335F) == 0);
    if (voiceRes) return 0;
    if (((CSuddenCommuBmView*)getInstance__Q22cf14CBattleManagerFv())->field_1AA != 0) return 0;
    if (((CSuddenCommuBmView*)getInstance__Q22cf14CBattleManagerFv())->field_20C8 != 0) return 0;
    if ((self->field_24 & 1) == 0) return 0;
    CSuddenCommuGlobal* global = getUnk80664658();
    if (global->field_214 & 2) return 0;
    if (func_8017FD44(global) != 0) return 0;
    CSuddenCommuActor* player = (CSuddenCommuActor*)cf::CfGameManager::getPlayer(0);
    if (player != 0) player = (CSuddenCommuActor*)((char*)player - 0x3E9C);
    if (player == 0) return 0;
    if (lbl_eu_806625E4 == player->field_3F28) {
        u32 vA = *player->field_4->vf30();
        u32 vB;
        // Both the A (0xA) and B (0xB) voice probes must be idle before the
        // commu can start; then the current voice action must be in the 7..9
        // window (anything else means the commu is blocked).
        if (func_80174C98(player, (int*)&vA, 0xA) != 0 ||
            func_80174C98(player, (int*)&(vB = *player->field_4->vf30()), 0xB) != 0) {
            // Commu armed: the current voice action must be in the 7..9 window
            // (anything else means the commu is blocked).
            if (((CSuddenCommuActorVt*)player)->vf298()->field_48 >= 7 &&
                ((CSuddenCommuActorVt*)player)->vf298()->field_48 <= 9) return 0;
        }
    }
    return 1;
}

// Sudden-commu voice-count sweep: for each of the three player slots, probe
// the battle object at +0x15E0 via func_80260518 (0x34) and broadcast the
// command payload to every idle actor (vtable 0x2BC == 0); accumulate the
// 0x61 stat into the global voice count and, when the 0x47 stat is present,
// scale it (lbl_eu_80667E34) and feed each idle actor's 0x11C slot with the
// truncated result. Then commit the count to the party gauge and either reset
// the commu state (field_C == 4) or re-arm the partner pairing.
//
// The u32->f32 conversion magic (0x4330000080000000) is defined as a named
// .sdata2 double below so MWCC's constant pool reuses it for the (f32)(u32)
// casts instead of emitting a TU-local @N label (CfResReloadImpl idiom).
double lbl_eu_80667E40 = 4503601774854144.0;

__declspec(noinline) void func_801BA490(CSuddenCommu* self) {
    u32 total = lbl_eu_806625E0;
    f32 scale = lbl_eu_80667E34;

    for (int i = 0; i < 3; i++) {
        void* spot = cf::CfGameManager::getPlayer(i);
        CSuddenCommuActor* player = (CSuddenCommuActor*)spot;
        if (spot != 0) player = (CSuddenCommuActor*)((char*)spot - 0x3E9C);
        if (player == 0) continue;
        void* obj = player->field_15E0;
        if (obj == 0) continue;

        u32 valA;
        f32 valB;
        if (func_80260518(obj, 0x34, &valA, &valB) != 0) {
            CSuddenCommuCmd cmd;
            std::memset(&cmd, 0, sizeof(cmd));
            cmd.field_C = 2;
            cmd.field_10 = valA;
            cmd.field_20 = valB;
            for (int j = 0; j < 3; j++) {
                void* spot2 = cf::CfGameManager::getPlayer(j);
                CSuddenCommuActor* other = (CSuddenCommuActor*)spot2;
                if (spot2 != 0) other = (CSuddenCommuActor*)((char*)spot2 - 0x3E9C);
                if (other == 0) continue;
                if (((CSuddenCommuActorVt*)other)->vf2BC() == 0) {
                    func_800EA9A8(getInstance__Q22cf14CBattleManagerFv(), other, &cmd, 0x34, 0);
                }
            }
        }

        if (func_80260264(obj, 0x61, &valA) != 0) {
            total += valA;
        }

        if (func_80260264(obj, 0x47, &valA) != 0) {
            f32 f28 = scale * (f32)valA;
            for (int j = 0; j < 3; j++) {
                void* spot2 = cf::CfGameManager::getPlayer(j);
                CSuddenCommuActor* other = (CSuddenCommuActor*)spot2;
                if (spot2 != 0) other = (CSuddenCommuActor*)((char*)spot2 - 0x3E9C);
                if (other == 0) continue;
                if (((CSuddenCommuActorVt*)other)->vf2BC() == 0) {
                    getInstance__Q22cf14CBattleManagerFv();
                    f32 f29 = func_800D81A8(0, other, 0);
                    f32 w = f29 * (f28 * ((CSuddenCommuActorVt*)other)->vf12C()) + scale;
                    s32 i2 = (s32)w;
                    f32 g = (f32)(u32)i2;
                    s32 j2 = (s32)g;
                    ((CSuddenCommuActorVt*)other)->vf11C((f32)(u32)j2);
                }
            }
        }
    }

    void* spot0 = cf::CfGameManager::getPlayer(self->field_4);
    CSuddenCommuActor* player0 = (CSuddenCommuActor*)spot0;
    if (spot0 != 0) player0 = (CSuddenCommuActor*)((char*)spot0 - 0x3E9C);

    if (total != 0) {
        if (self->field_24 & 0x4) total += 0x19;
        func_8018C820(&((CSuddenCommuBmGauge*)getInstance__Q22cf14CBattleManagerFv())->field_194, total);
    }

    if (self->field_C == 4) {
        func_800EA484((cf::CBattleManager*)getInstance__Q22cf14CBattleManagerFv(), lbl_eu_80667E38, 0x13);
        self->field_14 = 0;
        self->field_4 = -1;
        self->field_6 = -1;
        self->field_8 = -1;
        self->field_A = 0;
        self->field_C = 0;
        self->field_E = 0;
        func_802A35B8(self->field_10);
        self->field_10 = -1;
        self->field_18 = lbl_eu_80667E30;
        self->field_24 = 0;
        if (player0 != 0) {
            u32 v = *(u32*)player0->field_4->vf30();
            if (func_80174C98(player0, (int*)&v, 0x803) != 0) {
                ((CSuddenCommuActorVt*)player0)->vf304(4);
            }
        }
    } else {
        self->field_14 = 5;
        if (player0 != 0) {
            u32 v = *(u32*)player0->field_4->vf30();
            if (func_80174C98(player0, (int*)&v, 0x803) != 0) {
                if (self->field_C == 0) {
                    if (((CSuddenCommuActorVt*)player0)->vf308() == 0) {
                        ((CSuddenCommuActorVt*)player0)->vf314();
                    }
                } else {
                    if (((CSuddenCommuActorVt*)player0)->vf308() == 3) {
                        ((CSuddenCommuActorVt*)player0)->vf304(4);
                    } else if (((CSuddenCommuActorVt*)player0)->vf308() == 0) {
                        ((CSuddenCommuActorVt*)player0)->vf314();
                    }
                }
            }
        }
        int limit = 0xA;
        if (self->field_24 & 0x4) limit = 0x1E;
        void* spotP = cf::CfGameManager::getPlayer(self->field_4);
        CSuddenCommuActor* partner = (CSuddenCommuActor*)spotP;
        if (spotP != 0) partner = (CSuddenCommuActor*)((char*)spotP - 0x3E9C);
        if (partner != 0) {
            for (int j = 0; j < 3; j++) {
                if (self->field_4 == j) continue;
                void* spot2 = cf::CfGameManager::getPlayer(j);
                CSuddenCommuActor* other = (CSuddenCommuActor*)spot2;
                if (spot2 != 0) other = (CSuddenCommuActor*)((char*)spot2 - 0x3E9C);
                if (other == 0) continue;
                func_80082568__Q22cf13CfGameManagerFv(partner->field_3F28, other->field_3F28, limit);
            }
        }
    }
    func_80280BF0();
}

// Sudden-commu state dispatch (called from func_801BB9DC when no 0x20/0x40
// flag is set). Player slot field_4 selects the actor; then on the actor:
// state 4 -> full reset + voice-action probe (0x803) and vf314 kick;
// state 0 -> record state 0xB, probe 0x803 and chain vf308/vf304;
// otherwise just reset. A missing player also resets.
__declspec(noinline) void func_801BA978(CSuddenCommu* self) {
    CSuddenCommuActor* player = (CSuddenCommuActor*)cf::CfGameManager::getPlayer(self->field_4);
    if (player != 0) player = (CSuddenCommuActor*)((char*)player - 0x3E9C);
    if (player != 0) {
        s16 state = self->field_C;
        if (state == 4) {
            func_800EA484((cf::CBattleManager*)getInstance__Q22cf14CBattleManagerFv(), lbl_eu_80667E38, 0x13);
            self->field_14 = 0;
            u32 voice = self->field_10;
            self->field_4 = -1;
            self->field_6 = -1;
            self->field_8 = -1;
            self->field_A = 0;
            self->field_C = 0;
            self->field_E = 0;
            func_802A35B8(voice);
            f32 v30 = lbl_eu_80667E30;
            volatile f32* p18 = &self->field_18;
            self->field_10 = -1;
            *p18 = v30;
            self->field_24 = 0;
            // Probe the actor's current voice action; a hit kicks vf314.
            u32 v = *player->field_4->vf30();
            if (func_80174C98(player, (int*)&v, 0x803) != 0) {
                ((CSuddenCommuActorVt*)player)->vf314();
            }
        } else if (state == 0) {
            self->field_14 = 0xB;
            u32 v = *player->field_4->vf30();
            if (func_80174C98(player, (int*)&v, 0x803) != 0) {
                if (((CSuddenCommuActorVt*)player)->vf308() == 1) {
                    ((CSuddenCommuActorVt*)player)->vf304(0);
                }
            }
        } else {
            func_800EA484((cf::CBattleManager*)getInstance__Q22cf14CBattleManagerFv(), lbl_eu_80667E38, 0x13);
            self->field_14 = 0;
            u32 voice = self->field_10;
            self->field_4 = -1;
            self->field_6 = -1;
            self->field_8 = -1;
            self->field_A = 0;
            self->field_C = 0;
            self->field_E = 0;
            func_802A35B8(voice);
            f32 v30 = lbl_eu_80667E30;
            volatile f32* p18 = &self->field_18;
            self->field_10 = -1;
            *p18 = v30;
            self->field_24 = 0;
        }
    } else {
        func_800EA484((cf::CBattleManager*)getInstance__Q22cf14CBattleManagerFv(), lbl_eu_80667E38, 0x13);
        self->field_14 = 0;
        u32 voice = self->field_10;
        self->field_4 = -1;
        self->field_6 = -1;
        self->field_8 = -1;
        self->field_A = 0;
        self->field_C = 0;
        self->field_E = 0;
        func_802A35B8(voice);
        f32 v30 = lbl_eu_80667E30;
        volatile f32* p18 = &self->field_18;
        self->field_10 = -1;
        *p18 = v30;
        self->field_24 = 0;
    }
}

// Sudden-commu attack trigger: gate on the attacker being player-controlled
// (+0x3F00 bit 0x2) and the partner-state helper, then scan the three player
// slots for the actor matching the attacker. Depending on the target's HP
// (vtable slot 0x128) against lbl_eu_80667E48 and the move flag words,
// dispatch the commu (func_801BB464) with mode 3 / 1 / 0.
void func_801BAB94(CSuddenCommu* self, CSuddenCommuActor* attacker,
                   CSuddenCommuActor* target, CSuddenCommuMoveData* move) {
    if (!(attacker->flags3F00 & 0x2)) return;
    if (func_801BA2DC(self) == 0) return;
    for (int i = 0; i < 3; i++) {
        void* spot = cf::CfGameManager::getPlayer(i);
        CSuddenCommuActor* player = (CSuddenCommuActor*)spot;
        if (spot != 0) player = (CSuddenCommuActor*)((char*)spot - 0x3E9C);
        if (player == 0) continue;
        if (player != attacker) continue;
        if (((CSuddenCommuActorVt*)target)->vf128() <= lbl_eu_80667E48) {
            CSuddenCommuBmList* bm = (CSuddenCommuBmList*)getInstance__Q22cf14CBattleManagerFv();
            CSuddenCommuListNode* sentinel = (CSuddenCommuListNode*)bm->mHead;
            int count = 0;
            CSuddenCommuListNode* node;
            for (node = sentinel->next; node != sentinel; node = node->next) {
                count++;
            }
            if (count > 1) {
                int arg5 = 0;
                if (!(move->field_78 & 0x200)) arg5 = 0x19;
                func_801BB464(self, i, 3, player, arg5);
                break;
            }
        } else {
            if (move->field_74 & 0x100) {
                func_801BB464(self, i, 1, player, 0);
                break;
            } else if (move->field_74 & 0x40) {
                if (((CSuddenCommuActorVt*)target)->vfE0() == 1 ||
                    ((CSuddenCommuActorVt*)target)->vfE0() == 2) {
                    func_801BB464(self, i, 0, player, 0);
                    break;
                }
                break;
            }
        }
    }
}

// Sudden-commu partner dispatch: gate on the partner-state helper, then pick
// the match target (the given target when its +0x3F00 bit 0x2 is set, else the
// attacker) and scan the three player slots for the matching player actor.
void func_801BAD24(CSuddenCommu* self, CSuddenCommuActor* attacker, CSuddenCommuActor* target) {
    if (func_801BA2DC(self) == 0) return;
    int mode;
    CSuddenCommuActor* match;
    if (target->flags3F00 & 0x2) {
        mode = 2;
        match = target;
    } else {
        mode = 0;
        match = attacker;
    }
    for (int i = 0; i < 3; i++) {
        void* spot = cf::CfGameManager::getPlayer(i);
        CSuddenCommuActor* player = (CSuddenCommuActor*)spot;
        if (spot != 0) player = (CSuddenCommuActor*)((char*)spot - 0x3E9C);
        if (player == 0) continue;
        if (player != match) continue;
        func_801BB464(self, i, mode, player, 0);
        break;
    }
}

// Sudden-commu re-arm: clear the armed flag, then while the battle manager is
// active, decay field_18 and check the party state. If player 0 exists, probe
// its sub-object list with six voice/action flags (0x1C/0x16/0x17/0x18/0x1A/
// 0x19) plus the body-36C non-zero test; a hit clamps field_18 to E4C and
// exits, otherwise field_18 decays to E48. Then scan players 1..2 for the
// same flags: if every one is idle (all probes fail) and at least two slots
// were checked, arm the commu again (field_24 |= 1); otherwise run the
// pre-start gate (voice, battle flags, camera mode, player voice ids) and
// either re-enter the commu (state 4) or restart the timer.
void func_801BADE4(CSuddenCommu* self) {
    // Single "current player" slot reused across the probe blocks (retail r30).
    CSuddenCommuActor* player;
    self->field_24 &= ~1;
    if (func_8007F91C__Q22cf13CfGameManagerFv() != 0) {
        self->field_18 -= lbl_eu_80667E38;
        if (cf::CfGameManager::getPlayer(0) != 0) {
            void* spotP = cf::CfGameManager::getPlayer(0);
            player = (CSuddenCommuActor*)spotP;
            if (spotP != 0) player = (CSuddenCommuActor*)((char*)spotP - 0x3E9C);
            u32 v6, v5, v4, v3, v2, v1;
            int found = 0;
            if (func_80174C98(player, (int*)&(v1 = *player->field_4->vf30()), 0x1c) != 0) found = 1;
            else if (func_80174C98(player, (int*)&(v2 = *player->field_4->vf30()), 0x16) != 0) found = 1;
            else if (func_80174C98(player, (int*)&(v3 = *player->field_4->vf30()), 0x17) != 0) found = 1;
            else if (func_80174C98(player, (int*)&(v4 = *player->field_4->vf30()), 0x18) != 0) found = 1;
            else if (func_80174C98(player, (int*)&(v5 = *player->field_4->vf30()), 0x1a) != 0) found = 1;
            else if (func_80174C98(player, (int*)&(v6 = *player->field_4->vf30()), 0x19) != 0) found = 1;
            else found = !!player->field_3ED4->field_36C;
            if (found != 0) {
                if (self->field_18 < lbl_eu_80667E4C) self->field_18 = lbl_eu_80667E4C;
                goto commu_end;
            }
        } else {
            self->field_18 = lbl_eu_80667E50;
        }
        // The whole re-arm scan only runs while the timer is still at/below
        // the E48 threshold; retail skips straight to the epilogue otherwise.
        if (self->field_18 <= lbl_eu_80667E48) {
            self->field_18 = lbl_eu_80667E48;

            int flag29 = 1;
            int count28 = 1;
            for (int i = 1; i < 3; i++) {
                void* spot = cf::CfGameManager::getPlayer(i);
                player = (CSuddenCommuActor*)spot;
                if (spot != 0) player = (CSuddenCommuActor*)((char*)spot - 0x3E9C);
                if (player == 0) continue;
                u32 w6, w5, w4, w3, w2, w1;
                int found2 = 0;
                if (func_80174C98(player, (int*)&(w1 = *player->field_4->vf30()), 0x1c) != 0) found2 = 1;
                else if (func_80174C98(player, (int*)&(w2 = *player->field_4->vf30()), 0x16) != 0) found2 = 1;
                else if (func_80174C98(player, (int*)&(w3 = *player->field_4->vf30()), 0x17) != 0) found2 = 1;
                else if (func_80174C98(player, (int*)&(w4 = *player->field_4->vf30()), 0x18) != 0) found2 = 1;
                else if (func_80174C98(player, (int*)&(w5 = *player->field_4->vf30()), 0x1a) != 0) found2 = 1;
                else if (func_80174C98(player, (int*)&(w6 = *player->field_4->vf30()), 0x19) != 0) found2 = 1;
                else found2 = !!player->field_3ED4->field_36C;
                if (found2 != 0) flag29 = 0;
                if (player->field_3F28 == 0xa) goto commu_end;
                count28++;
            }
            if (flag29 != 0 && count28 >= 2) {
                self->field_24 |= 1;
            } else if (self->field_14 == 0) {
                bool voiceRes = (func_8009CF8C(0x335F) == 0);
                if (voiceRes) goto commu_end;
                if (((CSuddenCommuBmView*)getInstance__Q22cf14CBattleManagerFv())->field_1AA != 0) goto commu_end;
                if (((CSuddenCommuBmView*)getInstance__Q22cf14CBattleManagerFv())->field_20C8 != 0) goto commu_end;
                cf::CfGameManager::getInstance();
                if (func_8006EF04__Fi(0x04000000) != 0) goto commu_end;
                if (getUnk80664658()->field_214 & 2) goto commu_end;
                void* spotG = cf::CfGameManager::getPlayer(0);
                CSuddenCommuActor* pG = (CSuddenCommuActor*)spotG;
                if (spotG != 0) pG = (CSuddenCommuActor*)((char*)spotG - 0x3E9C);
                if (pG == 0) goto commu_end;
                if (lbl_eu_806625E4 == pG->field_3F28) {
                    u32 gvA;
                    u32 gvB;
                    if (func_80174C98(pG, (int*)&(gvA = *pG->field_4->vf30()), 0xa) != 0 ||
                        func_80174C98(pG, (int*)&(gvB = *pG->field_4->vf30()), 0xb) != 0) {
                        if (((CSuddenCommuActorVt*)pG)->vf298()->field_48 >= 7 &&
                            ((CSuddenCommuActorVt*)pG)->vf298()->field_48 <= 9) goto commu_end;
                    }
                }
                if (((CSuddenCommuActorVt*)pG)->vf308() > 1) goto commu_end;
                int allFound = 1;
                for (int i2 = 1; i2 < 3; i2++) {
                    void* spot2 = cf::CfGameManager::getPlayer(i2);
                    player = (CSuddenCommuActor*)spot2;
                    if (spot2 != 0) player = (CSuddenCommuActor*)((char*)spot2 - 0x3E9C);
                    if (player == 0) continue;
                    u32 av;
                    if (func_80174C98(player, (int*)&(av = *player->field_4->vf30()), 0x1c) == 0) {
                        allFound = 0;
                        break;
                    }
                }
                if (allFound != 0) {
                    void* spotA = cf::CfGameManager::getPlayer(0);
                    player = (CSuddenCommuActor*)spotA;
                    if (spotA != 0) player = (CSuddenCommuActor*)((char*)spotA - 0x3E9C);
                    u32 pv;
                    if (func_80174C98(player, (int*)&(pv = *player->field_4->vf30()), 0x803) != 0) {
                        self->field_4 = 0;
                        self->field_A = 0;
                        self->field_C = 4;
                        self->field_E = 0;
                        self->field_14 = 1;
                    } else {
                        self->field_18 = lbl_eu_80667E50;
                    }
                }
            }
        }
    } else {
        self->field_18 = lbl_eu_80667E50;
    }
commu_end:
    return;
}

// Sudden-commu start: roll a chance check against a per-type threshold table
// indexed by the partner actor's vf308 state (type 2 adds a 0x6a battle-stat
// bonus), then scan the three player slots: record the initiating slot in
// field_4, queue the others in pair_6/field_A, optionally shuffle the queue,
// probe the partner's voice action (0x803) and either arm (field_14 = 1) or
// park the timer at E50.
__declspec(noinline) void func_801BB464(CSuddenCommu* self, int playerIdx, int type, CSuddenCommuActor* player, int arg5) {
    cf::CfGameManager::getInstance();
    if (func_8006EF04__Fi(0x04000000) != 0) return;
    int chance = ml::math::mtRand(100) - arg5;
    if (chance < 0) chance = 0;
    self->field_C = type;
    self->field_E = type;
    u8 thresh;
    if (type == 0) {
        thresh = (&lbl_eu_806625E8)[((CSuddenCommuActorVt*)player)->vf308()];
    }
    if (type == 1) {
        thresh = (&lbl_eu_806625F0)[((CSuddenCommuActorVt*)player)->vf308()];
    }
    if (type == 2) {
        thresh = (&lbl_eu_806625F8)[((CSuddenCommuActorVt*)player)->vf308()];
        void* obj = player->field_15E0;
        if (obj != 0) {
            u32 v;
            if (func_80260264(obj, 0x6a, &v) != 0) thresh += v;
        }
    }
    if (type == 3) {
        thresh = (&lbl_eu_80662600)[((CSuddenCommuActorVt*)player)->vf308()];
        self->field_E = 1;
    }
    if (chance >= thresh) return;
    self->field_A = 0;
    int found = 0;
    for (int i = 0; i < 3; i++) {
        void* spot = cf::CfGameManager::getPlayer(i);
        CSuddenCommuActor* p = (CSuddenCommuActor*)spot;
        if (spot != 0) p = (CSuddenCommuActor*)((char*)spot - 0x3E9C);
        if (p == 0) continue;
        // Any active voice/action flag (or a non-zero body-36C word) aborts
        // the whole commu start.
        int busy = 0;
        u32 w1, w2, w3, w4, w5, w6;
        if (func_80174C98(p, (int*)&(w1 = *p->field_4->vf30()), 0x1c) != 0) busy = 1;
        else if (func_80174C98(p, (int*)&(w2 = *p->field_4->vf30()), 0x16) != 0) busy = 1;
        else if (func_80174C98(p, (int*)&(w3 = *p->field_4->vf30()), 0x17) != 0) busy = 1;
        else if (func_80174C98(p, (int*)&(w4 = *p->field_4->vf30()), 0x18) != 0) busy = 1;
        else if (func_80174C98(p, (int*)&(w5 = *p->field_4->vf30()), 0x1a) != 0) busy = 1;
        else if (func_80174C98(p, (int*)&(w6 = *p->field_4->vf30()), 0x19) != 0) busy = 1;
        else busy = !!p->field_3ED4->field_36C;
        if (busy != 0) return;
        if (playerIdx == i) {
            self->field_4 = i;
            found = 1;
        } else {
            self->pair_6[self->field_A] = i;
            self->field_A = self->field_A + 1;
        }
    }
    if (self->field_A == 0) return;
    if (found == 0) return;
    // With exactly two queued partners the retail randomly swaps the pair.
    if (self->field_A == 2) {
        if (ml::math::mtRand(2) != 0) {
            s16 t = self->field_6;
            self->field_6 = self->field_8;
            self->field_8 = t;
        }
    }
    u32 v = *player->field_4->vf30();
    if (func_80174C98(player, (int*)&v, 0x803) != 0) {
        self->field_14 = 1;
    } else {
        self->field_18 = lbl_eu_80667E50;
    }
}

void func_801BB818() {}

// Battle-state entry (mode 2/3): refresh the actor enum lists, fade the BGM,
// retire the old voice id and pick the next one, attach a voice-cue object to
// player 0, then arm the commu via func_801BC6A4.
void func_801BB81C(CSuddenCommu* self) {
    CSuddenCommuVoiceCue* cue;
    void* player;
    func_801BC474(self);
    func_800EA484((cf::CBattleManager*)getInstance__Q22cf14CBattleManagerFv(), lbl_eu_80667E54, 0x13);
    func_802A35B8(self->field_10);
    self->field_10 = func_802A3290();
    player = cf::CfGameManager::getPlayer(0);
    if (player != 0) player = (char*)player - 0x3E9C;
    if (player != 0) {
        if (player != 0)
            player = (char*)player + 0x3E9C;
        cue = (CSuddenCommuVoiceCue*)func_800451D8(0xC0, (int)player);
        self->field_20 = (u32)cue;
        if (cue != 0) {
            ((CSuddenCommuVoiceCueVt*)cue)->vf88(lbl_eu_80667E58);
            ((CSuddenCommuVoiceCue*)self->field_20)->field_B0 = self;
            func_801BFC38__Q22cf10CfSoundManFUlUlUlUlf(0, 0x8e, 0, 0, lbl_eu_80667E38);
        }
    }
    // Retail interleaves the field_1C store inside the field_24 read-modify-
    // write, so the flag update is spelled out as load/modify/store with the
    // timer store textually between the modify and the store.
    u32 flags = self->field_24 | 0x10;
    self->field_1C = lbl_eu_80667E48;
    self->field_24 = flags;
    func_801BC6A4(self, self->field_4, 0);
    if (self->field_C != 4)
        self->field_14 = 2;
    else
        self->field_14 = 3;
}

void func_801BB91C(){}

// Voice-node gate: if no voice node is registered for the id at +0x10,
// record state 4 at +0x14 (flag dispatch in func_801BB9DC picks it up).
void func_801BB998(CSuddenCommu* self) {
    bool missing = !func_802A3748(self->field_10);
    if (missing) {
        self->field_14 = 4;
    }
}

// Flag dispatch: reads the flag word at +0x24. If any of bits 5/6/7
// (0xE0) are set, dispatch on bit 5 (0x20) -> set flag 0x4 and call
// func_801BA490, else bit 6 (0x40) -> func_801BA490, else func_801BA978.
void func_801BB9DC(CSuddenCommu* self) {
    u32 flags = self->field_24;
    if ((flags & 0xE0) == 0) return;
    if (flags & 0x20) {
        self->field_24 |= 0x4;
        func_801BA490(self);
    } else if (flags & 0x40) {
        func_801BA490(self);
    } else {
        func_801BA978(self);
    }
}

void func_801BBA14(CSuddenCommu* self) {
    func_801BC6A4(self, self->field_6, 1);
    self->field_14 = 6;
}

// Same voice-node gate as func_801BB998, but the recorded state depends on
// +0x0A and bit 2 of the +0x24 flag word (0x4).
void func_801BBA50(CSuddenCommu* self) {
    bool missing = !func_802A3748(self->field_10);
    if (missing) {
        if (self->field_A == 1 || (self->field_24 & 0x4) == 0) {
            self->field_14 = 9;
        } else {
            self->field_14 = 7;
        }
    }
}

void func_801BBAB8(CSuddenCommu* self) {
    func_801BC6A4(self, self->field_8, 3);
    self->field_14 = 8;
}

void func_801BBAF4(CSuddenCommu* self) {
    bool missing = !func_802A3748(self->field_10);
    if (missing) {
        self->field_14 = 9;
    }
}

void func_801BBB38(CSuddenCommu* self) {
    func_801BC6A4(self, self->field_4, 5);
    self->field_14 = 10;
}

void func_801BBB74(CSuddenCommu* self) {
    bool missing = !func_802A3748(self->field_10);
    if (missing) {
        self->field_14 = 0xD;
    }
}

void func_801BBBB8(CSuddenCommu* self) {
    func_801BC6A4(self, self->field_4, 2);
    self->field_14 = 12;
}

void func_801BBBF4(CSuddenCommu* self) {
    bool missing = !func_802A3748(self->field_10);
    if (missing) {
        self->field_14 = 0xD;
    }
}

void func_801BBC38(){}

// Sudden-commu per-frame tick. Returns 1 while the commu camera/pad handling
// stays live, 0 once the commu finishes. While field_14 is non-zero the three
// players are scanned for the current commu partner: when the partner's
// sub-object list shows one of the six voice/action flags (0x1C/0x16/0x17/
// 0x18/0x1A/0x19) the commu state is reset (voice retired, cue cleared) and
// the camera path is skipped; otherwise the camera/trigger gate runs and, on
// pad input, the voice-cue is advanced and the commu flags updated.
//
// NOTE: the retail brief elides the camera/trigger block between .L_801BDA68
// and the .L_801BDB5C pad dispatch - that section is reconstructed below as
// a best-effort (field_24 bit-3 arm + cue advance) and marked.
int func_801BBCBC(CSuddenCommu* self) {
    CSuddenCommuActor* player;
    if (self->field_14 != 0) {
        void* spot0 = cf::CfGameManager::getPlayer(0);
        player = (CSuddenCommuActor*)spot0;
        if (spot0 != 0) player = (CSuddenCommuActor*)((char*)spot0 - 0x3E9C);
        int handled = 0;
        int r30 = 0;
        if (player != 0) {
            handled = 1;
            if (self->field_C == 4) {
                u32 v6, v5, v4, v3, v2, v1;
                int found = 0;
                if (func_80174C98(player, (int*)&(v1 = *player->field_4->vf30()), 0x1c) != 0) found = 1;
                else if (func_80174C98(player, (int*)&(v2 = *player->field_4->vf30()), 0x16) != 0) found = 1;
                else if (func_80174C98(player, (int*)&(v3 = *player->field_4->vf30()), 0x17) != 0) found = 1;
                else if (func_80174C98(player, (int*)&(v4 = *player->field_4->vf30()), 0x18) != 0) found = 1;
                else if (func_80174C98(player, (int*)&(v5 = *player->field_4->vf30()), 0x1a) != 0) found = 1;
                else if (func_80174C98(player, (int*)&(v6 = *player->field_4->vf30()), 0x19) != 0) found = 1;
                else found = !!player->field_3ED4->field_36C;
                if (found != 0) {
                    // Voice retired and state reset; the camera path is skipped.
                    func_801BC590(self);
                    func_800EA484((cf::CBattleManager*)getInstance__Q22cf14CBattleManagerFv(), lbl_eu_80667E38, 0x13);
                    func_800EA484((cf::CBattleManager*)getInstance__Q22cf14CBattleManagerFv(), lbl_eu_80667E38, 0x13);
                    self->field_14 = 0;
                    self->field_4 = -1;
                    self->field_6 = -1;
                    self->field_8 = -1;
                    self->field_A = 0;
                    self->field_C = 0;
                    self->field_E = 0;
                    func_802A35B8(self->field_10);
                    self->field_10 = -1;
                    self->field_18 = lbl_eu_80667E30;
                    self->field_24 = 0;
                    handled = 0;
                }
            } else {
                int found = 0;
                for (int i = 0; i < 3; i++) {
                    void* spot = cf::CfGameManager::getPlayer(i);
                    player = (CSuddenCommuActor*)spot;
                    if (spot != 0) player = (CSuddenCommuActor*)((char*)spot - 0x3E9C);
                    if (player == 0) continue;
                    u32 w6, w5, w4, w3, w2, w1;
                    found = 0;
                    if (func_80174C98(player, (int*)&(w1 = *player->field_4->vf30()), 0x1c) != 0) found = 1;
                    else if (func_80174C98(player, (int*)&(w2 = *player->field_4->vf30()), 0x16) != 0) found = 1;
                    else if (func_80174C98(player, (int*)&(w3 = *player->field_4->vf30()), 0x17) != 0) found = 1;
                    else if (func_80174C98(player, (int*)&(w4 = *player->field_4->vf30()), 0x18) != 0) found = 1;
                    else if (func_80174C98(player, (int*)&(w5 = *player->field_4->vf30()), 0x1a) != 0) found = 1;
                    else if (func_80174C98(player, (int*)&(w6 = *player->field_4->vf30()), 0x19) != 0) found = 1;
                    else found = !!player->field_3ED4->field_36C;
                    if (found != 0) break;
                }
                if (found != 0) {
                    func_801BC590(self);
                    func_800EA484((cf::CBattleManager*)getInstance__Q22cf14CBattleManagerFv(), lbl_eu_80667E38, 0x13);
                    func_800EA484((cf::CBattleManager*)getInstance__Q22cf14CBattleManagerFv(), lbl_eu_80667E38, 0x13);
                    self->field_14 = 0;
                    self->field_4 = -1;
                    self->field_6 = -1;
                    self->field_8 = -1;
                    self->field_A = 0;
                    self->field_C = 0;
                    self->field_E = 0;
                    func_802A35B8(self->field_10);
                    self->field_10 = -1;
                    self->field_18 = lbl_eu_80667E30;
                    self->field_24 = 0;
                    handled = 0;
                }
            }
        } else {
            r30 = 1;
        }

        // Camera/trigger gate: when the global flag mask or the camera
        // distance check passes, the camera/voice-cue block (elided from the
        // retail brief) runs and dispatches into the pad handler; otherwise
        // the camera is parked and the tick ends.
        if (handled != 0) {
            bool camTrigger = (lbl_eu_80663E24 & 0xAFA40000) != 0;
            if (!camTrigger) {
                f32 dist = ((CSuddenCommuCamView*)func_8049603C(lbl_eu_80663E14))->field_C;
                if (lbl_eu_80667E38 - dist >= lbl_eu_80667E38) goto cam_park;
            }
            // .L_801BDA68 (elided: camera/voice-cue advance) - best effort:
            self->field_24 &= ~0x8;
            func_80133F48(0, lbl_eu_80667E60);
            self->field_24 |= 0x8;
            self->field_1C = lbl_eu_80667E48;
            return 0;
        }
        goto cam_park2;
    cam_park:
        self->field_24 &= ~0x8;
        func_80133F48(0, lbl_eu_80667E60);
        self->field_24 |= 0x8;
        self->field_1C = lbl_eu_80667E48;
        return 0;
    cam_park2:
        // .L_801BDB5C pad/flag dispatch.
        if ((self->field_24 & 0x8) != 0) {
            if (self->field_20 != 0) {
                ((CSuddenCommuVoiceCueVt*)self->field_20)->vf88(lbl_eu_80667E58);
            }
            self->field_1C += func_80496288(lbl_eu_80663E14);
            int padBit = 0;
            int moveSel = 0;
            void* winState = (void*)func_8017FD44(lbl_eu_80663E14);
            if (winState != 0) {
                CSuddenCommuPadView* pad = (CSuddenCommuPadView*)cf::CfGameManager::getCurrentPad();
                r30 = 1;
                if (func_80086F9C__Q22cf13CfGameManagerFv(-1) != 0) {
                    padBit = (pad->field_00 >> 22) & 1;
                } else {
                    padBit = (pad->field_00 >> 5) & 1;
                }
                if (padBit != 0) {
                    moveSel = func_8017FD4C((int)winState);
                } else {
                    return 0;
                }
            }
            if (self->field_20 != 0) {
                ((CSuddenCommuVoiceCue*)self->field_20)->field_B0 = 0;
                func_800ACC14((void*)self->field_20, 1);
                ((CSuddenCommuVoiceCueVt*)self->field_20)->vf88(lbl_eu_80667E38);
                self->field_20 = 0;
            }
            if (padBit != 0) {
                if (moveSel == 2) {
                    self->field_24 |= 0x24;
                    func_8017FEF0(winState, 1);
                } else if (moveSel == 1) {
                    self->field_24 |= 0x40;
                    func_8017FEF0(winState, 0);
                } else {
                    self->field_24 |= 0x80;
                }
            } else {
                self->field_24 |= 0x80;
            }
            func_801BC590(self);
            func_800EA484((cf::CBattleManager*)getInstance__Q22cf14CBattleManagerFv(), lbl_eu_80667E38, 0x13);
            self->field_24 &= ~0x4;
        }
        return 0;
    } else {
        // field_14 == 0: idle commu - clamp the timer, then retire the cue.
        if (func_8017FD44(self) != 0) {
            if (self->field_18 < lbl_eu_80667E64) self->field_18 = lbl_eu_80667E64;
        }
        if (self->field_20 != 0) {
            ((CSuddenCommuVoiceCue*)self->field_20)->field_B0 = 0;
            func_800ACC14((void*)self->field_20, 1);
            self->field_20 = 0;
        }
        return 0;
    }
}

void func_801BC474(CSuddenCommu* self) {
    // Two holders declared side by side keep the retail stack slots (sp+0x10,
    // sp+0x08) while presenting a single lexical region to the scheduler.
    CSuddenCommuEnumHolder holderA;
    CSuddenCommuEnumHolder holderB;
    func_80043D90(&holderA);
    func_800F4A98(func_80043F18(&holderA), 0x100, 0);
    for (u32 i = 0; i < ((cf::CfObjEnumList*)func_80043F18(&holderA))->mPtrCount; i++) {
        cf::CfObjEnumList* list = (cf::CfObjEnumList*)func_80043F18(&holderA);
        void* p = func_800F6EAC(list, i);
        CSuddenCommuActor* actor = (CSuddenCommuActor*)p;
        if (p != 0) actor = (CSuddenCommuActor*)((char*)p - 0x3E9C);
        actor->voiceAct.field_3388 |= 0x2;
    }
    __dt__80043E88(&holderA, -1);
    func_80043D90(&holderB);
    func_800F4A98(func_80043F18(&holderB), 0x20, 0);
    for (u32 i = 0; i < ((cf::CfObjEnumList*)func_80043F18(&holderB))->mPtrCount; i++) {
        cf::CfObjEnumList* list = (cf::CfObjEnumList*)func_80043F18(&holderB);
        void* p = func_800F6EAC(list, i);
        CSuddenCommuActor* actor = (CSuddenCommuActor*)p;
        if (p != 0) actor = (CSuddenCommuActor*)((char*)p - 0x3E9C);
        actor->voiceAct.field_3388 |= 0x2;
    }
    __dt__80043E88(&holderB, -1);
    self->field_24 |= 0x2;
}

// Sudden-commu flag-clear sweep: when field_24 bit 0x2 is set, clear it and
// call func_801537E0 on every battle actor's voice-act sub-object (+0x3380)
// across both enum lists.
void func_801BC590(CSuddenCommu* self) {
    if (!(self->field_24 & 0x2)) return;
    self->field_24 &= ~0x2;
    {
        CSuddenCommuEnumHolder holder;
        func_80043D90(&holder);
        func_800F4A98(func_80043F18(&holder), 0x100, 0);
        for (u32 i = 0; i < ((cf::CfObjEnumList*)func_80043F18(&holder))->mPtrCount; i++) {
            cf::CfObjEnumList* list = (cf::CfObjEnumList*)func_80043F18(&holder);
            void* p = func_800F6EAC(list, i);
            CSuddenCommuActor* actor = (CSuddenCommuActor*)p;
            if (p != 0) actor = (CSuddenCommuActor*)((char*)p - 0x3E9C);
            func_801537E0(&actor->voiceAct);
        }
        __dt__80043E88(&holder, -1);
    }
    {
        CSuddenCommuEnumHolder holder;
        func_80043D90(&holder);
        func_800F4A98(func_80043F18(&holder), 0x20, 0);
        for (u32 i = 0; i < ((cf::CfObjEnumList*)func_80043F18(&holder))->mPtrCount; i++) {
            cf::CfObjEnumList* list = (cf::CfObjEnumList*)func_80043F18(&holder);
            void* p = func_800F6EAC(list, i);
            CSuddenCommuActor* actor = (CSuddenCommuActor*)p;
            if (p != 0) actor = (CSuddenCommuActor*)((char*)p - 0x3E9C);
            func_801537E0(&actor->voiceAct);
        }
        __dt__80043E88(&holder, -1);
    }
}

// Pick the battle-voice id for the commu's current phase and hand it to the
// voice manager (func_802A3680). `playerIdx` selects the actor whose spot the
// voice is attached to; `num` selects the phase table: 0 = random companion
// line, 1 = random entry from the field_E table, 2/3 = fixed ids, 4 =
// character-specific (matches partner voice ids), 5 = party table.
// Written as an if/else chain (retail emits a compare chain, not a jumptable).
__declspec(noinline) void func_801BC6A4(CSuddenCommu* self, int playerIdx, int num) {
    const u8* tbl = lbl_eu_805050B0;
    CSuddenCommuActor* player1 = (CSuddenCommuActor*)cf::CfGameManager::getPlayer(playerIdx);
    if (player1 != 0) player1 = (CSuddenCommuActor*)((char*)player1 - 0x3E9C);
    int result = -1;
    switch (num) {
    case 0: {
        int r = ml::math::mtRand(2);
        int scaled = r + self->field_E * 100;
        result = scaled + 0x3E9;
        break;
    }
    case 1:
        int rand;
        if (self->field_C != 0)
            rand = ml::math::mtRand(3);
        else
            rand = ml::math::mtRand(2);
        result = lbl_eu_80662608[rand] + self->field_E * 100;
        break;
    case 2:
        result = self->field_E * 100 + 0x3EE;
        break;
    case 3:
        result = self->field_E * 100 + 0x3ED;
        break;
    case 4: {
        CSuddenCommuActor* player2 = (CSuddenCommuActor*)cf::CfGameManager::getPlayer(self->field_4);
        if (player2 != 0) player2 = (CSuddenCommuActor*)((char*)player2 - 0x3E9C);
        u16 v2 = player2->field_3F28;
        if (v2 > 0xB) return;
        result = ((self->field_E == 0) ? 0x214 : 0x20B) + tbl[v2 - 1];
        if (player1->field_3F28 == 2 && v2 == 6 &&
            cf::CfGameManager::func_800822F4() < 0x91) {
            result = 0x1FA;
        }
        break;
    }
    case 5: {
        u16 v1 = player1->field_3F28;
        if (v1 > 0xB) return;
        int idx = tbl[v1 - 1] - 1;
        if (self->field_E != 0)
            result = ((const u16*)tbl)[0x10 + idx];
        else
            result = ((const u16*)tbl)[6 + idx];
        break;
    }
    }
    func_802A3680(self->field_10, player1 != 0 ? (CChainBattleObjTail*)((char*)player1 + 0x3E9C) : 0, result);
}

// --- hard-symbol stubs (scaffold_hard_symbols) ---
void sinit_801BC86C(){}
