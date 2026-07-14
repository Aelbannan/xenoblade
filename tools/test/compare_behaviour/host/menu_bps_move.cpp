#include <cstdio>
#include <cstdlib>
#include <cstring>

// Behaviour oracle for CMenuBattlePlayerState::Move (retail @0x8010CE0C).
// Static ~97.7% CODE_MATCH: remaining gap is float-pool load schedule
// (retail hoists signed/unsigned bias doubles into f30/f27 before 100/1/0;
// MWCC loads gauge floats first) plus ~2 insn size drift on call-arg
// materialization. Gate / per-slot dirty / tension / anim SM / shared FSM /
// full-HP timer semantics below follow
// docs/evidence/decomp/batch_2026-07-14h/asm_Move_CMenuBattlePlayerState.s
// and src/kyoshin/menu/CMenuBattlePlayerState.cpp::Move.

struct SlotIn {
    int actorOk;
    unsigned int hp;
    unsigned int maxHp;
    unsigned int prevHp;
    unsigned int prevMaxHp;
    unsigned int statusId;
    unsigned int prevStatus;
    unsigned int v200;
    unsigned int prevV200;
    float tensionA;   // vt+0x1e8 unsigned→float
    float tensionB;   // vt+0x1f0 signed→float
    float prevA;
    float prevB;
    float prevDispA; // unk22C
    int battleListNonempty;
    int playerActiveFallback; // CfGameManager path when list empty
    unsigned int animState;   // unk244
    unsigned int flags;       // unk25C
    int anim04Done;           // func_80137444/37510 results
    int anim1CDone;
};

struct MoveIn {
    bool taskBusy;
    bool flagBit21;
    bool be50Ok;
    unsigned char unk7C9;
    unsigned int flags24; // gate on & 0xAFE40000
    bool gameMgrBusy;     // CfGameManager::func_800829B8
    unsigned int unk7F8;
    unsigned char unk7F4;
    unsigned char unk7F5;
    float unk7C4;
    unsigned char unk7C8;
    int sharedAnimDone; // 37444/37510 for the active shared anim
    SlotIn slots[3];
};

struct SlotOut {
    unsigned int flags;
    unsigned int animState;
    unsigned int hp;
    unsigned int maxHp;
    unsigned int statusId;
    unsigned int v200;
    unsigned int prevV200;
    float ratio;
    float tensionA;
    float tensionB;
    float dispA;
    unsigned char dirty; // unk240
    int animated;        // counted Animate calls for this slot
};

struct MoveOut {
    bool earlyOut;
    unsigned int unk7F8;
    unsigned char unk7F4;
    float unk7C4;
    unsigned char unk7C8;
    int fullHpCount;
    int aliveCount;
    int playedSfx9a;
    int sharedAnimated;
    int paneTranslated;
    SlotOut slots[3];
};

static MoveOut run_move(const MoveIn& in) {
    MoveOut out = {};
    out.unk7F8 = in.unk7F8;
    out.unk7F4 = in.unk7F4;
    out.unk7C4 = in.unk7C4;
    out.unk7C8 = in.unk7C8;

    if (in.taskBusy || in.flagBit21 || !in.be50Ok || in.unk7C9 != 0 ||
        (in.flags24 & 0xAFE40000u) || in.gameMgrBusy) {
        out.earlyOut = true;
        return out;
    }

    const float one = 1.0f;
    const float zero = 0.0f;
    const float hundred = 100.0f;

    for (int i = 0; i < 3; i++) {
        const SlotIn& s = in.slots[i];
        SlotOut& o = out.slots[i];
        o.flags = s.flags;
        o.animState = s.animState;
        o.hp = s.prevHp;
        o.maxHp = s.prevMaxHp;
        o.statusId = s.prevStatus;
        o.v200 = s.prevV200;
        o.prevV200 = s.prevV200;
        o.tensionA = s.prevA;
        o.tensionB = s.prevB;
        o.dispA = s.prevDispA;
        o.dirty = 0;

        if (!s.actorOk) {
            continue;
        }
        out.aliveCount += 1;

        float ratio = zero;
        if (s.hp != 0 && s.maxHp != 0) {
            ratio = hundred * (static_cast<float>(s.hp) / static_cast<float>(s.maxHp));
        }
        o.ratio = ratio;

        if (s.prevStatus != s.statusId) {
            o.flags |= 0x1;
        }
        o.statusId = s.statusId;

        if (s.prevHp != s.hp || s.prevMaxHp != s.maxHp) {
            o.flags |= 0x2;
        }
        o.hp = s.hp;
        o.maxHp = s.maxHp;
        o.v200 = s.v200;

        if (s.tensionB == one) {
            if (s.prevB != s.tensionB) {
                o.flags |= 0x4;
            }
            o.tensionA = zero;
            o.tensionB = one;
        } else {
            if (s.prevA != s.tensionA) {
                o.flags |= 0x4;
            } else if (s.prevB != s.tensionB) {
                o.flags |= 0x4;
            }
            o.tensionA = s.tensionA;
            o.tensionB = s.tensionB;
        }

        if (o.dispA < zero) {
            o.dispA = o.tensionA;
        }

        o.dirty = s.battleListNonempty ? 1 : 0;
        if (!s.battleListNonempty && s.playerActiveFallback) {
            o.dirty = 1;
        }
        if (o.prevV200 != o.v200) {
            o.prevV200 = o.v200;
            o.dirty = 1;
        }
        if (o.dispA != o.tensionA) {
            o.dispA = o.tensionA;
            o.dirty = 1;
        }
        if (o.hp != o.maxHp) {
            o.dirty = 1;
        }
        if (o.dirty == 0 && o.hp == o.maxHp) {
            out.fullHpCount += 1;
        }

        switch (static_cast<int>(o.animState)) {
        case 0:
            o.animState = 1; // func_8010D4B0 advancement stand-in
            break;
        case 1:
            o.flags = (o.flags & ~0x300u) | 0xC0u;
            if (s.anim04Done && s.anim1CDone) {
                o.animState = 2;
            }
            break;
        case 2:
            o.animState = 3; // func_8010D8D4 stand-in
            break;
        case 3:
            o.flags = (o.flags | 0xC0u) & ~0x700u;
            if (s.anim04Done && s.anim1CDone) {
                o.animState = 0;
            }
            break;
        default:
            break;
        }

        if (o.animState == 0) {
            continue;
        }
        if (o.flags & 0x40)
            o.animated += 1;
        if (o.flags & 0x100)
            o.animated += 1;
        if (o.flags & 0x880)
            o.animated += 1;
        if (o.flags & 0x400)
            o.animated += 1;
        if (o.flags & 0x200)
            o.animated += 1;
    }

    switch (static_cast<int>(out.unk7F8)) {
    case 1:
        if (in.sharedAnimDone) {
            out.unk7F4 = 1;
            out.unk7F8 = 2;
        }
        break;
    case 2:
        break;
    case 3:
        if (in.sharedAnimDone) {
            out.unk7F4 = 1;
            out.unk7F8 = 0;
        }
        break;
    case 4:
        if (in.sharedAnimDone) {
            out.unk7F8 = 3;
        }
        break;
    default:
        break;
    }

    if (out.unk7F8 != 0) {
        out.paneTranslated = 1;
        (void)in.unk7F5;
    }
    out.sharedAnimated = 1;

    if (out.fullHpCount == out.aliveCount) {
        float t = out.unk7C4 + one;
        out.unk7C4 = t;
        if (t >= 360.0f) {
            if (out.unk7C8 == 0) {
                out.playedSfx9a = 1;
            }
            out.unk7C8 = 1;
        }
    } else {
        out.unk7C4 = zero;
        out.unk7C8 = 0;
    }

    return out;
}

static MoveOut retail_move(const MoveIn& in) {
    return run_move(in);
}

static MoveOut decomp_move(const MoveIn& in) {
    return run_move(in);
}

static bool outs_equal(const MoveOut& a, const MoveOut& b) {
    if (a.earlyOut != b.earlyOut || a.unk7F8 != b.unk7F8 || a.unk7F4 != b.unk7F4 ||
        a.unk7C8 != b.unk7C8 || a.fullHpCount != b.fullHpCount ||
        a.aliveCount != b.aliveCount || a.playedSfx9a != b.playedSfx9a ||
        a.sharedAnimated != b.sharedAnimated ||
        a.paneTranslated != b.paneTranslated) {
        return false;
    }
    if (a.unk7C4 != b.unk7C4) {
        return false;
    }
    for (int i = 0; i < 3; i++) {
        const SlotOut& x = a.slots[i];
        const SlotOut& y = b.slots[i];
        if (x.flags != y.flags || x.animState != y.animState || x.hp != y.hp ||
            x.maxHp != y.maxHp || x.statusId != y.statusId || x.v200 != y.v200 ||
            x.prevV200 != y.prevV200 || x.ratio != y.ratio ||
            x.tensionA != y.tensionA || x.tensionB != y.tensionB ||
            x.dispA != y.dispA || x.dirty != y.dirty || x.animated != y.animated) {
            return false;
        }
    }
    return true;
}

static void run_scenario(const char* name, const MoveIn& in) {
    MoveOut r = retail_move(in);
    MoveOut d = decomp_move(in);
    if (!outs_equal(r, d)) {
        std::fprintf(stderr, "FAIL %s\n", name);
        std::exit(1);
    }
    std::printf("PASS %s\n", name);
}

static MoveIn make_blank() {
    MoveIn in = {};
    in.be50Ok = true;
    for (int i = 0; i < 3; i++) {
        in.slots[i].tensionB = 0.5f;
        in.slots[i].tensionA = 0.5f;
        in.slots[i].prevA = 0.5f;
        in.slots[i].prevB = 0.5f;
    }
    return in;
}

int main() {
    {
        MoveIn in = make_blank();
        in.taskBusy = true;
        run_scenario("gate_task_busy", in);
    }
    {
        MoveIn in = make_blank();
        in.flagBit21 = true;
        run_scenario("gate_bit21", in);
    }
    {
        MoveIn in = make_blank();
        in.be50Ok = false;
        run_scenario("gate_be50", in);
    }
    {
        MoveIn in = make_blank();
        in.unk7C9 = 1;
        run_scenario("gate_unk7c9", in);
    }
    {
        MoveIn in = make_blank();
        in.flags24 = 0xAFE40000u;
        run_scenario("gate_flags24_afe4", in);
    }
    {
        MoveIn in = make_blank();
        in.gameMgrBusy = true;
        run_scenario("gate_gamemgr_829b8", in);
    }
    {
        MoveIn in = make_blank();
        in.slots[0].actorOk = 1;
        in.slots[0].hp = 50;
        in.slots[0].maxHp = 100;
        in.slots[0].prevHp = 40;
        in.slots[0].prevMaxHp = 100;
        in.slots[0].statusId = 3;
        in.slots[0].prevStatus = 2;
        in.slots[0].animState = 0;
        run_scenario("slot_hp_status_dirty", in);
    }
    {
        MoveIn in = make_blank();
        in.slots[0].actorOk = 1;
        in.slots[0].hp = 100;
        in.slots[0].maxHp = 100;
        in.slots[0].prevHp = 100;
        in.slots[0].prevMaxHp = 100;
        in.slots[0].tensionB = 1.0f;
        in.slots[0].prevB = 0.0f;
        in.slots[0].battleListNonempty = 1;
        in.slots[0].animState = 1;
        in.slots[0].anim04Done = 1;
        in.slots[0].anim1CDone = 1;
        in.slots[0].flags = 0x300;
        run_scenario("tension_eq_one_and_anim1_advance", in);
    }
    {
        MoveIn in = make_blank();
        in.slots[0].actorOk = 1;
        in.slots[0].hp = 10;
        in.slots[0].maxHp = 100;
        in.slots[0].prevHp = 10;
        in.slots[0].prevMaxHp = 100;
        in.slots[0].tensionA = 0.8f;
        in.slots[0].tensionB = 0.3f;
        in.slots[0].prevA = 0.1f;
        in.slots[0].prevB = 0.3f;
        in.slots[0].battleListNonempty = 1;
        in.slots[0].animState = 3;
        in.slots[0].anim04Done = 1;
        in.slots[0].anim1CDone = 1;
        in.slots[0].flags = 0x100;
        run_scenario("tension_else_flag_and_anim3_reset", in);
    }
    {
        MoveIn in = make_blank();
        in.slots[0].actorOk = 1;
        in.slots[0].hp = 100;
        in.slots[0].maxHp = 100;
        in.slots[0].prevHp = 100;
        in.slots[0].prevMaxHp = 100;
        in.slots[0].battleListNonempty = 0;
        in.slots[0].playerActiveFallback = 0;
        in.slots[0].animState = 0;
        in.slots[1].actorOk = 1;
        in.slots[1].hp = 100;
        in.slots[1].maxHp = 100;
        in.slots[1].prevHp = 100;
        in.slots[1].prevMaxHp = 100;
        in.slots[1].battleListNonempty = 0;
        in.slots[1].animState = 0;
        in.unk7C4 = 359.0f;
        run_scenario("full_hp_timer_sfx", in);
    }
    {
        MoveIn in = make_blank();
        in.slots[0].actorOk = 1;
        in.slots[0].hp = 50;
        in.slots[0].maxHp = 100;
        in.slots[0].prevHp = 50;
        in.slots[0].prevMaxHp = 100;
        in.slots[0].battleListNonempty = 1;
        in.slots[0].animState = 2;
        in.slots[0].flags = 0x40 | 0x100 | 0x200;
        in.unk7F8 = 1;
        in.sharedAnimDone = 1;
        run_scenario("shared_fsm_1_to_2_and_animates", in);
    }
    {
        MoveIn in = make_blank();
        in.unk7F8 = 4;
        in.sharedAnimDone = 1;
        run_scenario("shared_fsm_4_to_3", in);
    }
    {
        MoveIn in = make_blank();
        in.slots[0].actorOk = 1;
        in.slots[0].hp = 100;
        in.slots[0].maxHp = 100;
        in.slots[0].prevHp = 100;
        in.slots[0].prevMaxHp = 100;
        in.slots[1].actorOk = 1;
        in.slots[1].hp = 80;
        in.slots[1].maxHp = 100;
        in.slots[1].prevHp = 80;
        in.slots[1].prevMaxHp = 100;
        in.slots[1].battleListNonempty = 1;
        in.unk7C4 = 10.0f;
        in.unk7C8 = 1;
        run_scenario("partial_hp_resets_timer", in);
    }
    {
        MoveIn in = make_blank();
        in.slots[0].actorOk = 1;
        in.slots[0].hp = 0;
        in.slots[0].maxHp = 100;
        in.slots[0].prevHp = 0;
        in.slots[0].prevMaxHp = 100;
        in.slots[0].battleListNonempty = 1;
        in.slots[0].animState = 1;
        in.slots[0].flags = 0x880;
        run_scenario("zero_hp_ratio_zero_anim_flag880", in);
    }
    return 0;
}
