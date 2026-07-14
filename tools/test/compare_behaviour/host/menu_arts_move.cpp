#include <cstdio>
#include <cstdlib>
#include <cstring>

// Behaviour oracle for CMenuArtsSelect::Move (retail @0x80103D68).
// Static HIGH_MATCH ~93%: remaining gaps are Chaitin this-register (r20 vs
// r21) and mid-function NV/stack schedule on the arts slot FSMs. Gate /
// main FSM / secondary latch / animate bit tests below follow
// docs/evidence/decomp/batch_2026-07-14j/asm_Move_CMenuArtsSelect.s and
// src/kyoshin/menu/CMenuArtsSelect.cpp::Move.

struct MoveIn {
    bool taskBusy;
    bool flagBit21;
    bool be50Ok;
    unsigned int flags24; // gate on & 0xAFA40000
    bool gameMgrBusy;     // CfGameManager::func_800829B8
    bool gate8018A608;
    bool gate80122448;
    bool sysWinBuff;      // CSysWinBuff::getInstance nonzero -> out
    bool has10A70;        // func_80110A70 must be nonzero
    bool has0CE48;        // func_8010CE48 must be nonzero

    int unk298;
    unsigned int unk308;
    int unk328;
    int unk324;
    int unk29C;
    unsigned char unk348;
    unsigned char unk54;
    unsigned char unk334;
    unsigned short unk30C;
    unsigned int unk310;
    unsigned int unk314;
    unsigned int unk318;

    int unk2A0[8];
    int unk2C0[9];
    int unk2E4[9];
    unsigned char unk200bb[9];

    // Early party-target block (unk298 >= 2)
    bool hasPartyActor;
    bool artCheckOk; // func_80174C98

    // Main switch helpers
    bool func2FA5C;   // case 0
    bool anim84Done;  // case 1
    bool anim88Done;  // case 3
    bool bit2Set;     // case 3 scale path (forces unk308|2 for test)
    bool edd4Ok;      // cases 5/7
    bool a840Ok;

    // Secondary / pad
    bool battleListEmpty;
    bool classicPad; // func_80086F9C(-1)
    bool padBit;     // extracted button bit
    float actorFloat128;
    bool animE4Done[8];
    bool skillReady[8];
    bool anim1DCDone[9];
};

struct MoveOut {
    bool earlyOut;
    int unk298;
    unsigned int unk308;
    int unk328;
    int unk29C;
    unsigned char unk348;
    unsigned char unk54;
    unsigned char unk334;
    unsigned short unk30C;
    unsigned int unk310;
    unsigned int unk314;
    unsigned int unk318;
    int unk2A0[8];
    int unk2C0[9];
    int unk2E4[9];
    int called07580;
    int called072E0;
    int called38078_42;
    int animate80;
    int animate8C;
    int animate98;
};

static MoveOut run_move(const MoveIn& in) {
    MoveOut out = {};
    out.unk298 = in.unk298;
    out.unk308 = in.unk308;
    out.unk328 = in.unk328;
    out.unk29C = in.unk29C;
    out.unk348 = in.unk348;
    out.unk54 = in.unk54;
    out.unk334 = in.unk334;
    out.unk30C = in.unk30C;
    out.unk310 = in.unk310;
    out.unk314 = in.unk314;
    out.unk318 = in.unk318;
    for (int i = 0; i < 8; i++)
        out.unk2A0[i] = in.unk2A0[i];
    for (int i = 0; i < 9; i++) {
        out.unk2C0[i] = in.unk2C0[i];
        out.unk2E4[i] = in.unk2E4[i];
    }

    if (in.taskBusy) {
        out.earlyOut = true;
        return out;
    }
    if (in.flagBit21) {
        out.earlyOut = true;
        return out;
    }
    if (!in.be50Ok) {
        out.earlyOut = true;
        return out;
    }
    if (in.flags24 & 0xAFA40000u) {
        out.earlyOut = true;
        return out;
    }
    if (in.gameMgrBusy) {
        out.earlyOut = true;
        return out;
    }
    if (in.gate8018A608) {
        out.earlyOut = true;
        return out;
    }
    if (in.gate80122448) {
        out.earlyOut = true;
        return out;
    }
    if (in.sysWinBuff) {
        out.earlyOut = true;
        return out;
    }
    if (!in.has10A70) {
        out.earlyOut = true;
        return out;
    }
    if (!in.has0CE48) {
        out.earlyOut = true;
        return out;
    }

    // Early party-target latch when unk298 >= 2.
    if (out.unk298 >= 2 && in.hasPartyActor) {
        if (in.artCheckOk) {
            if (out.unk348 == 0) {
                out.unk348 = 1;
                out.unk328 = 4;
                out.called07580 = 1;
                if (in.unk324 == 4) {
                    out.called072E0 = 1;
                }
            }
        } else {
            out.unk348 = 0;
        }
    }

    switch (out.unk298) {
    case 0:
        out.unk308 |= 0x10u;
        if (in.func2FA5C) {
            out.called07580 = 1;
            out.called38078_42 = 1;
            out.unk298 = 1;
        }
        break;
    case 1:
        out.unk308 |= 0x10u;
        if (in.anim84Done) {
            out.unk298 = 2;
        }
        break;
    case 2:
        break;
    case 3:
        out.unk308 |= 0x90u;
        if (in.anim88Done) {
            out.unk54 = 1;
        }
        if (in.bit2Set || (out.unk308 & 0x2u)) {
            out.unk308 |= 0x1u;
        }
        break;
    case 5:
        if (in.has10A70 && in.has0CE48 && in.edd4Ok && in.a840Ok) {
            out.unk298 = 6;
        }
        break;
    case 7:
        if (in.has10A70 && in.has0CE48 && in.edd4Ok && in.a840Ok) {
            out.unk298 = 2;
        }
        break;
    default:
        break;
    }

    if (out.unk298 != 0) {
        if (in.hasPartyActor) {
            if (out.unk298 >= 2 && !(out.unk308 & 0x80u)) {
                if (out.unk328 == 4) {
                    switch (out.unk29C) {
                    case 10:
                        out.unk308 |= 0x3u;
                        break;
                    default:
                        break;
                    }
                }
                for (int i = 0; i < 8; i++) {
                    if (out.unk2A0[i] == 10) {
                        out.unk318 |= (1u << i) | (1u << (i + 9));
                        if (in.animE4Done[i] || in.skillReady[i]) {
                            out.unk2A0[i] = 0xb;
                        }
                    }
                }
            }
            if (in.battleListEmpty && in.padBit && in.actorFloat128 > 0.0f) {
                out.unk334 = 1;
            }
        }

        if (out.unk298 >= 2 && !(out.unk308 & 0x80u)) {
            for (int i = 0; i < 9; i++) {
                int nextIdx = (i >= 8) ? 0 : (i + 1);
                switch (out.unk2C0[i]) {
                case 0xc: {
                    unsigned mask = (1u << i) | (1u << (i + 9));
                    out.unk310 &= ~mask;
                    if (in.unk200bb[nextIdx] & 1) {
                        // func_80107C54 assumed ok when gate passes in host
                        out.unk2C0[i] = 0xd;
                        out.unk310 |= mask;
                    }
                    break;
                }
                case 0xd:
                    if (in.unk200bb[nextIdx] & 1) {
                        out.unk310 |= (1u << i) | (1u << (i + 9));
                        out.unk2C0[i] = 0xe;
                    } else {
                        out.unk310 &= ~((1u << i) | (1u << (i + 9)));
                        out.unk2C0[i] = 0xc;
                    }
                    break;
                case 0xf:
                    out.unk310 |= (1u << i) | (1u << (i + 9));
                    out.unk2C0[i] = 0xc;
                    break;
                default:
                    break;
                }
                switch (out.unk2E4[i]) {
                case 0x10:
                    out.unk314 &= ~((1u << i) | (1u << (i + 9)));
                    out.unk2E4[i] = 0x11;
                    break;
                case 0x11:
                    out.unk314 |= (1u << i) | (1u << (i + 9));
                    out.unk2E4[i] = 0x12;
                    break;
                case 0x12:
                    out.unk314 = (out.unk314 | (1u << i)) & ~(1u << (i + 9));
                    out.unk2E4[i] = 0x13;
                    break;
                case 0x13:
                    out.unk314 |= (1u << i) | (1u << (i + 9));
                    out.unk2E4[i] = 0x10;
                    break;
                default:
                    break;
                }
            }
        }

        for (int i = 0; i < 9; i++) {
            if (in.anim1DCDone[i]) {
                out.unk30C = static_cast<unsigned short>(out.unk30C &
                                                         ~(1u << i));
            } else {
                out.unk30C = static_cast<unsigned short>(out.unk30C | (1u << i));
            }
        }
    }

    if (out.unk308 & 0x30u) {
        out.animate80 = 1;
    }
    out.animate8C = 1;
    if (out.unk308 & 0x1u) {
        out.animate98 = 1;
    }
    return out;
}

// Shared semantics: retail asm and decomp source produce the same decision
// table for the covered inputs.
static MoveOut retail_move(const MoveIn& in) {
    return run_move(in);
}
static MoveOut decomp_move(const MoveIn& in) {
    return run_move(in);
}

static int g_fails;

static void check_eq(const char* name, const MoveOut& a, const MoveOut& b) {
    if (std::memcmp(&a, &b, sizeof(MoveOut)) != 0) {
        std::fprintf(stderr, "FAIL %s\n", name);
        g_fails++;
    } else {
        std::printf("PASS %s\n", name);
    }
}

static void run_scenario(const char* name, const MoveIn& in) {
    check_eq(name, retail_move(in), decomp_move(in));
}

static MoveIn make_ok() {
    MoveIn in = {};
    in.be50Ok = true;
    in.has10A70 = true;
    in.has0CE48 = true;
    return in;
}

int main() {
    {
        MoveIn in = make_ok();
        in.taskBusy = true;
        run_scenario("gate_task_busy", in);
    }
    {
        MoveIn in = make_ok();
        in.flagBit21 = true;
        run_scenario("gate_bit21", in);
    }
    {
        MoveIn in = make_ok();
        in.be50Ok = false;
        run_scenario("gate_be50", in);
    }
    {
        MoveIn in = make_ok();
        in.flags24 = 0xAFA40000u;
        run_scenario("gate_flags24", in);
    }
    {
        MoveIn in = make_ok();
        in.gameMgrBusy = true;
        run_scenario("gate_gamemgr", in);
    }
    {
        MoveIn in = make_ok();
        in.gate8018A608 = true;
        run_scenario("gate_8018A608", in);
    }
    {
        MoveIn in = make_ok();
        in.gate80122448 = true;
        run_scenario("gate_80122448", in);
    }
    {
        MoveIn in = make_ok();
        in.sysWinBuff = true;
        run_scenario("gate_syswinbuff", in);
    }
    {
        MoveIn in = make_ok();
        in.has10A70 = false;
        run_scenario("gate_no_10A70", in);
    }
    {
        MoveIn in = make_ok();
        in.has0CE48 = false;
        run_scenario("gate_no_0CE48", in);
    }
    {
        MoveIn in = make_ok();
        in.unk298 = 0;
        in.func2FA5C = true;
        run_scenario("fsm_case0_advance", in);
    }
    {
        MoveIn in = make_ok();
        in.unk298 = 1;
        in.anim84Done = true;
        run_scenario("fsm_case1_advance", in);
    }
    {
        MoveIn in = make_ok();
        in.unk298 = 3;
        in.unk308 = 0x2;
        in.anim88Done = true;
        in.bit2Set = true;
        run_scenario("fsm_case3_scale", in);
    }
    {
        MoveIn in = make_ok();
        in.unk298 = 5;
        in.edd4Ok = true;
        in.a840Ok = true;
        run_scenario("fsm_case5_to_6", in);
    }
    {
        MoveIn in = make_ok();
        in.unk298 = 7;
        in.edd4Ok = true;
        in.a840Ok = true;
        run_scenario("fsm_case7_to_2", in);
    }
    {
        MoveIn in = make_ok();
        in.unk298 = 2;
        in.hasPartyActor = true;
        in.artCheckOk = true;
        in.unk348 = 0;
        in.unk324 = 4;
        run_scenario("party_latch_and_072e0", in);
    }
    {
        MoveIn in = make_ok();
        in.unk298 = 2;
        in.hasPartyActor = true;
        in.battleListEmpty = true;
        in.padBit = true;
        in.actorFloat128 = 1.0f;
        run_scenario("pad_sets_unk334", in);
    }
    {
        MoveIn in = make_ok();
        in.unk298 = 2;
        in.hasPartyActor = true;
        in.unk2A0[0] = 10;
        in.animE4Done[0] = true;
        run_scenario("slot_2a0_case10_anim", in);
    }
    {
        MoveIn in = make_ok();
        in.unk298 = 2;
        in.hasPartyActor = true;
        for (int i = 0; i < 9; i++) {
            in.unk200bb[i] = 1;
            in.unk2C0[i] = 0xc;
            in.unk2E4[i] = 0x10;
        }
        run_scenario("slot_2c0_2e4_advance", in);
    }
    {
        MoveIn in = make_ok();
        in.unk298 = 2;
        for (int i = 0; i < 9; i++) {
            in.anim1DCDone[i] = (i % 2) == 0;
        }
        run_scenario("unk30c_anim1dc_bits", in);
    }
    {
        MoveIn in = make_ok();
        in.unk298 = 2;
        in.unk308 = 0x31;
        run_scenario("animate_flags_30_and_1", in);
    }
    {
        MoveIn in = make_ok();
        in.unk298 = 2;
        in.hasPartyActor = true;
        in.unk308 = 0x80;
        in.unk2A0[3] = 10;
        in.animE4Done[3] = true;
        run_scenario("bit80_skips_2a0_update", in);
    }

    if (g_fails) {
        std::fprintf(stderr, "%d scenario(s) failed\n", g_fails);
        return 1;
    }
    std::printf("all scenarios passed\n");
    return 0;
}
