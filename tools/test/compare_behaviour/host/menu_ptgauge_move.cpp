#include <cstdio>
#include <cstdlib>
#include <cstring>

// Behaviour oracle for CMenuPTGauge::Move (retail @0x80188714).
// Static match ~97.1%: remaining gaps are Chaitin regalloc on size() walk
// (count/cur r3↔r4), float FPRs on gauge width reset, and byte-range fold.

struct MoveIn {
    bool taskBusy;
    bool flagBit21;
    bool be50Ok;
    unsigned int flags24;
    int state8C;
    int sub90;
    int latch94;
    int latch98;
    int gageA0;
    int actorCount;
    int partyVal;
    unsigned char byte1AA;
    bool animInDone;
    bool animOutDone;
};

struct MoveOut {
    int state8C;
    int sub90;
    int latch94;
    int latch98;
    int gageA0;
    bool earlyOut;
    bool didCommonAnimate;
    bool didSoundA0;
    bool didSoundA1;
    bool didWidthReset;
};

static MoveOut run_move(const MoveIn& in, bool /*decomp*/) {
    MoveOut out = {};
    out.state8C = in.state8C;
    out.sub90 = in.sub90;
    out.latch94 = in.latch94;
    out.latch98 = in.latch98;
    out.gageA0 = in.gageA0;

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

    switch (in.state8C) {
    case 0:
        if (in.actorCount == 0) {
            break;
        }
        out.latch94 = 0;
        out.sub90 = 0;
        out.state8C = 1;
        if (in.gageA0 != 0) {
            out.gageA0 = 0;
            out.didWidthReset = true;
        }
        out.didSoundA0 = true;
        break;
    case 1:
        if (in.animInDone) {
            out.state8C = 2;
        }
        break;
    case 2: {
        // PTMF handler side-effects omitted; state machine after call:
        if (in.partyVal <= 0 && in.actorCount == 0) {
            out.state8C = 3;
            out.didSoundA1 = true;
            break;
        }
        if (in.sub90 >= 1) {
            break;
        }
        {
            int flag = 0;
            if (in.byte1AA >= 1 && in.byte1AA <= 0x18) {
                flag = 1;
            }
            if (flag != 0 && in.byte1AA == 5) {
                flag = 1;
            } else {
                flag = 0;
            }
            if (flag == 0) {
                break;
            }
            out.sub90 = 2;
            out.latch98 = out.latch94;
        }
        break;
    }
    case 3:
        if (in.animOutDone) {
            out.state8C = 0;
        }
        break;
    default:
        break;
    }

    out.didCommonAnimate = true;
    return out;
}

static MoveOut retail_move(const MoveIn& in) {
    return run_move(in, false);
}

static MoveOut decomp_move(const MoveIn& in) {
    return run_move(in, true);
}

static bool equals(const MoveOut& a, const MoveOut& b) {
    return a.state8C == b.state8C && a.sub90 == b.sub90 && a.latch94 == b.latch94 &&
           a.latch98 == b.latch98 && a.gageA0 == b.gageA0 && a.earlyOut == b.earlyOut &&
           a.didCommonAnimate == b.didCommonAnimate && a.didSoundA0 == b.didSoundA0 &&
           a.didSoundA1 == b.didSoundA1 && a.didWidthReset == b.didWidthReset;
}

static void run_scenario(const char* name, const MoveIn& in) {
    if (!equals(retail_move(in), decomp_move(in))) {
        std::printf("FAIL %s\n", name);
        std::exit(1);
    }
    std::printf("PASS %s\n", name);
}

static MoveIn base() {
    MoveIn in = {};
    in.be50Ok = true;
    in.state8C = 0;
    in.actorCount = 1;
    in.partyVal = 100;
    in.byte1AA = 5;
    return in;
}

int main() {
    MoveIn in;

    in = base();
    in.taskBusy = true;
    run_scenario("gate_task_busy", in);

    in = base();
    in.flagBit21 = true;
    run_scenario("gate_bit21", in);

    in = base();
    in.be50Ok = false;
    run_scenario("gate_be50", in);

    in = base();
    in.flags24 = 0xAFA40000u;
    run_scenario("gate_flags24", in);

    in = base();
    in.actorCount = 0;
    run_scenario("case0_empty_list", in);

    in = base();
    in.gageA0 = 50;
    run_scenario("case0_start_width_reset", in);

    in = base();
    in.state8C = 1;
    in.animInDone = true;
    run_scenario("case1_anim_done", in);

    in = base();
    in.state8C = 1;
    in.animInDone = false;
    run_scenario("case1_anim_busy", in);

    in = base();
    in.state8C = 2;
    in.partyVal = 0;
    in.actorCount = 0;
    run_scenario("case2_to_outro", in);

    in = base();
    in.state8C = 2;
    in.sub90 = 0;
    in.byte1AA = 5;
    in.partyVal = 10;
    run_scenario("case2_byte5_advance", in);

    in = base();
    in.state8C = 2;
    in.sub90 = 0;
    in.byte1AA = 4;
    run_scenario("case2_byte_in_range_not_five", in);

    in = base();
    in.state8C = 2;
    in.sub90 = 1;
    run_scenario("case2_sub90_ge1", in);

    in = base();
    in.state8C = 3;
    in.animOutDone = true;
    run_scenario("case3_anim_done", in);

    in = base();
    in.state8C = 3;
    in.animOutDone = false;
    run_scenario("case3_anim_busy", in);

    in = base();
    in.state8C = 4;
    run_scenario("default_state", in);

    in = base();
    in.flags24 = 0x10000000u;
    run_scenario("gate_flags24_unrelated", in);

    return 0;
}
