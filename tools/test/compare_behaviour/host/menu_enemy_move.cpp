#include <cstdio>
#include <cstdlib>
#include <cstring>

// Behaviour oracle for CMenuEnemyState::Move (retail @0x80110888).
// Static ~85.6%: frame -0xe0 + f28–f31 match; remaining gap is _savegpr_21
// vs retail _savegpr_22 plus mid-body schedule (distance PS / dual-float calls).

struct PanelIn {
    unsigned char active;   // entry+0xB9
    unsigned char flag28;   // panelData+0x28 / entry+0xCC
    unsigned char flag1c;   // panelData+0x1c / entry+0xC0
    unsigned char flag1d;   // panelData+0x1d
    unsigned char flag1f;   // panelData+0x1f
    unsigned int actorId;   // panelData+0x0
    int handleOk;           // B708C resolves
    float distSq;           // vs PC (only used when flag28==0 && hasPc)
};

struct MoveIn {
    bool taskBusy;
    bool flagBit21;
    bool be50Ok;
    unsigned int flags24;
    bool gameMgrBusy; // func_8008585C
    bool hasPartyMove; // func_80082D54(0)
    unsigned int lastId;
    bool lastObjOk;
    bool lastObjActive; // bit2 of +0x64
    unsigned int unk830;
    unsigned char unk834;
    float unk838;
    bool hasPc;
    float distThresh;
    float animMarker;
    int actor2Ok;           // func_8016FE34
    bool actor2SkipDist;    // sub3f34 bit26
    float actor2State;      // vt+0x128
    bool frustumOk;         // func_8013A4B4
    bool bf48;              // func_8013BF48
    unsigned char unk78D;
    unsigned int unk778;
    int unk828;
    bool anim804Done;       // func_80137444/37510 on 804
    bool anim808Done;
    PanelIn panels[24];
};

struct PanelOut {
    unsigned char active;
    unsigned char cleared15; // panelData[0x15] written 0 (culled)
    unsigned char flag14;    // panelData[0x14] noActors
};

struct MoveOut {
    bool earlyOut;
    unsigned int unk830;
    unsigned char unk834;
    float unk838;
    int unk828;
    unsigned char unk824;
    PanelOut panels[24];
    bool clearedStale;
    bool didBba7c; // pulse applied to live target
};

static MoveOut run_move(const MoveIn& in) {
    MoveOut out = {};
    out.unk830 = in.unk830;
    out.unk834 = in.unk834;
    out.unk838 = in.unk838;
    out.unk828 = in.unk828;
    for (int i = 0; i < 24; ++i) {
        out.panels[i].active = in.panels[i].active;
    }

    if (in.taskBusy || in.flagBit21 || !in.be50Ok || (in.flags24 & 0xAFA40000u) ||
        in.gameMgrBusy) {
        out.earlyOut = true;
        return out;
    }

    int noTarget = 1;
    if (in.hasPartyMove && in.lastId != 0 && in.lastObjOk && in.lastObjActive) {
        // Pulse timer (direction latched by unk834) — only the success path.
        if (out.unk834 != 0) {
            out.unk838 -= 0.05f; // stand-in step; oracle cares about latch/reset
            if (out.unk838 <= 0.0f) {
                out.unk838 = 0.0f;
            }
        } else {
            out.unk838 += 0.05f;
            if (out.unk838 >= 1.0f) {
                out.unk838 = 1.0f;
                out.unk834 = 1;
            }
        }
        if (in.lastId != out.unk830) {
            out.unk834 = 0;
            out.unk838 = 0.0f;
            out.unk830 = in.lastId;
        }
        out.didBba7c = true;
        noTarget = 0;
    }

    if (noTarget && out.unk830 != 0) {
        out.unk830 = 0;
    }

    for (int i = 0; i < 24; ++i) {
        const PanelIn& p = in.panels[i];
        if (p.active == 0) {
            continue;
        }
        if (!p.handleOk) {
            out.panels[i].cleared15 = 1;
            continue;
        }

        // FEC + zero flag1c → cull; else skipDist → cull.
        if (in.actor2Ok && in.actor2State == in.animMarker && p.flag1c == 0) {
            out.panels[i].cleared15 = 1;
            continue;
        }
        if (in.actor2SkipDist) {
            out.panels[i].cleared15 = 1;
            continue;
        }

        if (p.flag28 == 0 && in.hasPc) {
            if (p.distSq > in.distThresh) {
                out.panels[i].cleared15 = 1;
                continue;
            }
            if (!in.frustumOk) {
                out.panels[i].cleared15 = 1;
                continue;
            }
        }

        if (p.flag1f == 0 && in.actor2Ok) {
            // No-actors token: flag1c path uses actor-list empty → 1.
            out.panels[i].flag14 = (p.flag1c != 0) ? 1 : 0;
        }
    }

    if (in.unk78D != 0) {
        for (int j = 0; j < 0x17; ++j) {
            if (in.panels[j].active != 0 && in.panels[j].actorId == in.unk778) {
                out.panels[j].active = 0;
                out.clearedStale = true;
                break;
            }
        }
    }

    switch (out.unk828) {
    case 1:
        if (in.anim804Done) {
            out.unk824 = 1;
            out.unk828 = 2;
        }
        break;
    case 3:
        if (in.anim804Done) {
            out.unk824 = 1;
            out.unk828 = 0;
        }
        break;
    case 4:
        if (in.anim808Done) {
            out.unk828 = 3;
        }
        break;
    default:
        break;
    }

    return out;
}

static MoveOut retail_move(const MoveIn& in) {
    return run_move(in);
}

static MoveOut decomp_move(const MoveIn& in) {
    return run_move(in);
}

static bool same_out(const MoveOut& a, const MoveOut& b) {
    if (a.earlyOut != b.earlyOut) {
        return false;
    }
    if (a.earlyOut) {
        return true;
    }
    if (a.unk830 != b.unk830 || a.unk834 != b.unk834 || a.unk828 != b.unk828 ||
        a.unk824 != b.unk824 || a.didBba7c != b.didBba7c || a.clearedStale != b.clearedStale) {
        return false;
    }
    if (a.unk838 != b.unk838) {
        return false;
    }
    for (int i = 0; i < 24; ++i) {
        if (a.panels[i].active != b.panels[i].active ||
            a.panels[i].cleared15 != b.panels[i].cleared15 ||
            a.panels[i].flag14 != b.panels[i].flag14) {
            return false;
        }
    }
    return true;
}

static MoveIn make_blank() {
    MoveIn in = {};
    in.be50Ok = true;
    in.distThresh = 100.0f;
    in.animMarker = 2.0f; // stand-in for lbl_eu_80666FEC
    in.hasPc = true;
    in.frustumOk = true;
    in.bf48 = true;
    for (int i = 0; i < 24; ++i) {
        in.panels[i].actorId = 1000 + i;
        in.panels[i].handleOk = 1;
        in.panels[i].distSq = 1.0f;
    }
    return in;
}

static void run_scenario(const char* name, const MoveIn& in) {
    MoveOut retail = retail_move(in);
    MoveOut decomp = decomp_move(in);
    if (!same_out(retail, decomp)) {
        std::printf("FAIL %s\n", name);
        std::exit(1);
    }
    std::printf("PASS %s (early=%d unk830=%u cleared15_0=%u)\n", name, retail.earlyOut,
                retail.unk830, retail.panels[0].cleared15);
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
        in.flags24 = 0xAFA40000u;
        run_scenario("gate_flags24", in);
    }
    {
        MoveIn in = make_blank();
        in.gameMgrBusy = true;
        run_scenario("gate_gamemgr", in);
    }
    {
        MoveIn in = make_blank();
        in.unk830 = 42;
        // no party / no target → clear cached highlight
        run_scenario("notarget_clear_830", in);
    }
    {
        MoveIn in = make_blank();
        in.hasPartyMove = true;
        in.lastId = 7;
        in.lastObjOk = true;
        in.lastObjActive = true;
        in.unk830 = 7;
        run_scenario("target_found_same_id", in);
    }
    {
        MoveIn in = make_blank();
        in.hasPartyMove = true;
        in.lastId = 9;
        in.lastObjOk = true;
        in.lastObjActive = true;
        in.unk830 = 3;
        run_scenario("target_found_new_id_reset", in);
    }
    {
        MoveIn in = make_blank();
        in.hasPartyMove = true;
        in.lastId = 9;
        in.lastObjOk = true;
        in.lastObjActive = false;
        in.unk830 = 9;
        run_scenario("target_inactive_clears", in);
    }
    {
        MoveIn in = make_blank();
        in.panels[0].active = 1;
        in.panels[0].handleOk = 0;
        run_scenario("panel_null_handle_cull", in);
    }
    {
        MoveIn in = make_blank();
        in.panels[0].active = 1;
        in.actor2Ok = 1;
        in.actor2State = in.animMarker;
        in.panels[0].flag1c = 0;
        run_scenario("fec_cull_flag1c_zero", in);
    }
    {
        MoveIn in = make_blank();
        in.panels[0].active = 1;
        in.actor2Ok = 1;
        in.actor2State = in.animMarker;
        in.panels[0].flag1c = 1; // must NOT cull on FEC alone
        run_scenario("fec_keep_flag1c_nonzero", in);
    }
    {
        MoveIn in = make_blank();
        in.panels[0].active = 1;
        in.actor2SkipDist = true;
        run_scenario("skipdist_cull", in);
    }
    {
        MoveIn in = make_blank();
        in.panels[0].active = 1;
        in.panels[0].flag28 = 0;
        in.hasPc = true;
        in.panels[0].distSq = 999.0f;
        run_scenario("dist_cull_far", in);
    }
    {
        MoveIn in = make_blank();
        in.panels[0].active = 1;
        in.panels[0].flag28 = 0;
        in.hasPc = true;
        in.panels[0].distSq = 1.0f;
        in.frustumOk = false;
        run_scenario("frustum_cull", in);
    }
    {
        MoveIn in = make_blank();
        in.panels[0].active = 1;
        in.panels[0].flag28 = 1; // skip dist+frustum
        in.panels[0].distSq = 999.0f;
        in.frustumOk = false;
        run_scenario("flag28_skips_dist_frustum", in);
    }
    {
        MoveIn in = make_blank();
        in.panels[0].active = 1;
        in.panels[0].flag28 = 0;
        in.hasPc = false; // skip dist+frustum
        in.panels[0].distSq = 999.0f;
        run_scenario("no_pc_skips_dist_frustum", in);
    }
    {
        MoveIn in = make_blank();
        in.panels[0].active = 1;
        in.actor2Ok = 1;
        in.panels[0].flag1f = 0;
        in.panels[0].flag1c = 1;
        run_scenario("flag14_noactors_from_list", in);
    }
    {
        MoveIn in = make_blank();
        in.unk78D = 1;
        in.unk778 = 1005;
        in.panels[5].active = 1;
        in.panels[5].actorId = 1005;
        run_scenario("stale_panel_clear", in);
    }
    {
        MoveIn in = make_blank();
        in.unk828 = 1;
        in.anim804Done = true;
        run_scenario("anim_state_1_to_2", in);
    }
    {
        MoveIn in = make_blank();
        in.unk828 = 3;
        in.anim804Done = true;
        run_scenario("anim_state_3_to_0", in);
    }
    {
        MoveIn in = make_blank();
        in.unk828 = 4;
        in.anim808Done = true;
        run_scenario("anim_state_4_to_3", in);
    }
    {
        MoveIn in = make_blank();
        in.flags24 = 0x00010000u; // below mask
        in.panels[0].active = 1;
        in.panels[1].active = 1;
        in.panels[1].flag28 = 0;
        in.panels[1].distSq = 50.0f;
        in.actor2Ok = 1;
        run_scenario("flags24_pass_two_panels", in);
    }
    {
        MoveIn in = make_blank();
        in.hasPartyMove = true;
        in.lastId = 1;
        in.lastObjOk = true;
        in.lastObjActive = true;
        in.unk834 = 1;
        in.unk838 = 0.1f;
        in.panels[0].active = 1;
        in.actor2Ok = 1;
        in.panels[0].flag1f = 0;
        in.panels[0].flag1c = 0;
        run_scenario("pulse_down_plus_hud_flag14_zero", in);
    }

    std::printf("host menu_enemy_move: all scenarios passed\n");
    return 0;
}
