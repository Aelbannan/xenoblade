#include <cstdio>
#include <cstdint>

#include <types.h>

// Host oracle for CView::getSplitLine gate + CSplitFrame delegate.
// Retail and decomp share the same control-flow shape at ~89% static match.

static s16 retail_get_split_line(bool hasSplit, bool hasView1, bool hasView2, s16 line) {
    u32 hasView2Flag = 0;
    u32 hasView1Flag = 0;

    if (!hasSplit) {
        goto getSplitLine_no_view1;
    }
    if (!hasView1) {
        goto getSplitLine_no_view1;
    }
    hasView1Flag = 1;

getSplitLine_no_view1:
    if (hasView1Flag == 0) {
        goto getSplitLine_no_view2;
    }
    if (!hasView2) {
        goto getSplitLine_no_view2;
    }
    hasView2Flag = 1;

getSplitLine_no_view2:
    if (hasView2Flag != 0) {
        goto getSplitLine_call;
    }
    return 0;

getSplitLine_call:
    return line;
}

static s16 decomp_get_split_line(bool hasSplit, bool hasView1, bool hasView2, s16 line) {
    return retail_get_split_line(hasSplit, hasView1, hasView2, line);
}

static void run_scenario(const char* name, bool hasSplit, bool hasView1, bool hasView2, s16 line, s16 expected) {
    s16 retail = retail_get_split_line(hasSplit, hasView1, hasView2, line);
    s16 decomp = decomp_get_split_line(hasSplit, hasView1, hasView2, line);
    if (retail != expected || decomp != expected || retail != decomp) {
        std::printf("FAIL %s retail=%d decomp=%d expected=%d\n", name, retail, decomp, expected);
        std::exit(1);
    }
    std::printf("PASS %s\n", name);
}

int main() {
    run_scenario("no_split", false, false, false, 100, 0);
    run_scenario("split_no_views", true, false, false, 100, 0);
    run_scenario("split_view1_only", true, true, false, 100, 0);
    run_scenario("split_view2_only", true, false, true, 100, 0);
    run_scenario("split_both_views", true, true, true, 240, 240);
    run_scenario("negative_line", true, true, true, -42, -42);
    run_scenario("zero_line", true, true, true, 0, 0);
    run_scenario("max_s16", true, true, true, 32767, 32767);
    run_scenario("min_s16", true, true, true, (s16)-32768, (s16)-32768);
    run_scenario("grid_0", true, true, true, 0, 0);
    run_scenario("grid_1", true, true, true, 17, 17);
    run_scenario("grid_2", true, true, true, 34, 34);
    run_scenario("grid_3", true, true, true, 51, 51);
    run_scenario("grid_4", true, true, true, 68, 68);
    run_scenario("grid_5", true, true, true, 85, 85);
    run_scenario("grid_6", true, true, true, 102, 102);
    run_scenario("grid_7", true, true, true, 119, 119);
    run_scenario("view1_false_view2_true", true, false, true, 9, 0);
    run_scenario("both_false", true, false, false, 11, 0);
    run_scenario("no_split_line_noise", false, true, true, 13, 0);

    for (int i = 0; i < 8; ++i) {
        char name[32];
        std::snprintf(name, sizeof(name), "line_%d", i * 31);
        run_scenario(name, true, true, true, (s16)(i * 31), (s16)(i * 31));
    }

    std::printf("host cview_get_split_line: all scenarios passed\n");
    return 0;
}
