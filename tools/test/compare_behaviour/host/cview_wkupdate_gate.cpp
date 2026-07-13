#include <cstdio>
#include <cstdint>

#include <types.h>

static bool retail_should_apply(bool hasSplit, bool hasView1, bool hasView2) {
    u32 hasView1a = 0;
    u32 hasView2a = 0;
    if (!hasSplit) {
        goto wkUpdate_no_apply;
    }
    if (!hasView1) {
        goto wkUpdate_no_apply;
    }
    hasView1a = 1;

wkUpdate_no_apply:
    if (hasView1a == 0) {
        goto wkUpdate_apply_check;
    }
    if (!hasView2) {
        goto wkUpdate_apply_check;
    }
    hasView2a = 1;

wkUpdate_apply_check:
    return hasView2a != 0;
}

static bool decomp_should_apply(bool hasSplit, bool hasView1, bool hasView2) {
    u32 hasView1a = 0;
    u32 hasView2a = 0;
    if (!hasSplit) {
        goto wkUpdate_no_apply;
    }
    if (!hasView1) {
        goto wkUpdate_no_apply;
    }
    hasView1a = 1;

wkUpdate_no_apply:
    if (hasView1a == 0) {
        goto wkUpdate_apply_check;
    }
    if (!hasView2) {
        goto wkUpdate_apply_check;
    }
    hasView2a = 1;

wkUpdate_apply_check:
    return hasView2a != 0;
}

static bool retail_should_set_evt_none(bool childrenEmpty, bool unk238Empty, bool hasSplit, bool hasView1,
                                       bool hasView2) {
    if (!childrenEmpty || !unk238Empty) {
        return false;
    }

    u32 hasView1Local = 0;
    u32 hasView2Local = 0;
    if (!hasSplit) {
        goto wkUpdate_no_setevent_check;
    }
    if (!hasView1) {
        goto wkUpdate_no_setevent_check;
    }
    hasView1Local = 1;

wkUpdate_no_setevent_check:
    if (hasView1Local == 0) {
        goto wkUpdate_setevent;
    }
    if (!hasView2) {
        goto wkUpdate_setevent;
    }
    hasView2Local = 1;

wkUpdate_setevent:
    return hasView2Local == 0;
}

static bool decomp_should_set_evt_none(bool childrenEmpty, bool unk238Empty, bool hasSplit, bool hasView1,
                                       bool hasView2) {
    return retail_should_set_evt_none(childrenEmpty, unk238Empty, hasSplit, hasView1, hasView2);
}

static void run_scenario_apply(const char* name, bool hasSplit, bool hasView1, bool hasView2, bool expected) {
    bool retail = retail_should_apply(hasSplit, hasView1, hasView2);
    bool decomp = decomp_should_apply(hasSplit, hasView1, hasView2);
    if (retail != decomp || retail != expected) {
        std::printf("FAIL %s apply: retail=%d decomp=%d expected=%d\n", name, retail, decomp, expected);
        std::exit(1);
    }
    std::printf("PASS %s\n", name);
}

static void run_scenario_evt(const char* name, bool childrenEmpty, bool unk238Empty, bool hasSplit,
                             bool hasView1, bool hasView2, bool expected) {
    bool retail = retail_should_set_evt_none(childrenEmpty, unk238Empty, hasSplit, hasView1, hasView2);
    bool decomp = decomp_should_set_evt_none(childrenEmpty, unk238Empty, hasSplit, hasView1, hasView2);
    if (retail != decomp || retail != expected) {
        std::printf("FAIL %s evt: retail=%d decomp=%d expected=%d\n", name, retail, decomp, expected);
        std::exit(1);
    }
    std::printf("PASS %s\n", name);
}

int main() {
    run_scenario_apply("apply_no_split", false, false, false, false);
    run_scenario_apply("apply_split_no_v1", true, false, false, false);
    run_scenario_apply("apply_v1_no_v2", true, true, false, false);
    run_scenario_apply("apply_both_views", true, true, true, true);

    run_scenario_evt("evt_children_nonempty", false, true, true, true, true, false);
    run_scenario_evt("evt_unk238_nonempty", true, false, true, true, true, false);
    run_scenario_evt("evt_no_split", true, true, false, false, false, true);
    run_scenario_evt("evt_split_no_v1", true, true, true, false, false, true);
    run_scenario_evt("evt_split_v1_no_v2", true, true, true, true, false, true);
    run_scenario_evt("evt_split_both_views", true, true, true, true, true, false);

    std::printf("host cview_wkupdate_gate: all scenarios passed\n");
    return 0;
}
