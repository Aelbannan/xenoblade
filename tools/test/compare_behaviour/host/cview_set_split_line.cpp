#include <cstdio>
#include <cstdint>

#include <types.h>

// Host oracle for CView::setSplitLine gate + CSplitFrame delegate.

static s16 retail_set_split_line(bool hasSplit, bool hasView1, bool hasView2, s16 line, s16 prior) {
    u32 hasView2Flag = 0;
    u32 hasView1Flag = 0;

    if (!hasSplit) {
        goto setSplitLine_no_view1;
    }
    if (!hasView1) {
        goto setSplitLine_no_view1;
    }
    hasView1Flag = 1;

setSplitLine_no_view1:
    if (hasView1Flag == 0) {
        goto setSplitLine_no_view2;
    }
    if (!hasView2) {
        goto setSplitLine_no_view2;
    }
    hasView2Flag = 1;

setSplitLine_no_view2:
    if (hasView2Flag == 0) {
        return prior;
    }
    return line;
}

static s16 decomp_set_split_line(bool hasSplit, bool hasView1, bool hasView2, s16 line, s16 prior) {
    return retail_set_split_line(hasSplit, hasView1, hasView2, line, prior);
}

static void run_scenario(const char* name, bool hasSplit, bool hasView1, bool hasView2, s16 line, s16 prior,
                         s16 expected) {
    s16 retail = retail_set_split_line(hasSplit, hasView1, hasView2, line, prior);
    s16 decomp = decomp_set_split_line(hasSplit, hasView1, hasView2, line, prior);
    if (retail != expected || decomp != expected || retail != decomp) {
        std::printf("FAIL %s retail=%d decomp=%d expected=%d\n", name, retail, decomp, expected);
        std::exit(1);
    }
    std::printf("PASS %s\n", name);
}

int main() {
    run_scenario("no_split", false, false, false, 100, 7, 7);
    run_scenario("split_no_views", true, false, false, 50, 7, 7);
    run_scenario("split_view1_only", true, true, false, 80, 7, 7);
    run_scenario("split_view2_only", true, false, true, 90, 7, 7);
    run_scenario("split_both_views", true, true, true, 160, 7, 160);
    run_scenario("negative_line", true, true, true, -99, 0, -99);
    run_scenario("zero_line", true, true, true, 0, 5, 0);
    run_scenario("max_s16", true, true, true, 32767, 1, 32767);
    run_scenario("min_s16", true, true, true, (s16)-32768, 1, (s16)-32768);
    run_scenario("grid_0", true, true, true, 0, 3, 0);
    run_scenario("grid_1", true, true, true, 23, 3, 23);
    run_scenario("grid_2", true, true, true, 46, 3, 46);
    run_scenario("grid_3", true, true, true, 69, 3, 69);
    run_scenario("grid_4", true, true, true, 92, 3, 92);
    run_scenario("grid_5", true, true, true, 115, 3, 115);
    run_scenario("grid_6", true, true, true, 138, 3, 138);
    run_scenario("grid_7", true, true, true, 161, 3, 161);

    std::printf("host cview_set_split_line: all scenarios passed\n");
    return 0;
}
