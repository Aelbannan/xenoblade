#include <cmath>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>

using s16 = std::int16_t;

struct CRect16 {
    s16 mPosX;
    s16 mPosY;
    s16 mSizeX;
    s16 mSizeY;
};

struct MockView {
    s16 unk0;
    s16 unk2;
    s16 frameOffsetX;
    s16 frameOffsetY;
    s16 unk54;
    s16 unk56;
    int childCount;
};

struct MockProc {
    int viewIdCount;
    MockView* firstView;
};

static constexpr float kScale = 0.6f;

static s16 retail_float_margin(s16 size) {
    const float f = static_cast<float>(size);
    const float delta = (f * kScale - f) * 0.5f;
    return static_cast<s16>(std::trunc(delta));
}

static s16 decomp_float_margin(s16 size) {
    const float f = static_cast<float>(size);
    return static_cast<s16>((f * kScale - f) / 2.0f);
}

static CRect16 retail_make_client_rect(const MockProc& proc) {
    CRect16 rect{};
    if (proc.viewIdCount == 0 || proc.firstView == nullptr) {
        return rect;
    }

    const MockView& view = *proc.firstView;
    const s16 anotherX = retail_float_margin(view.unk0);
    const s16 anotherY = retail_float_margin(view.unk2);
    const s16 childOffset = static_cast<s16>(view.childCount * 20);

    rect.mPosX = static_cast<s16>((view.unk54 + view.frameOffsetX) - anotherX + childOffset);
    rect.mPosY = static_cast<s16>((view.unk56 + view.frameOffsetY) - anotherY + childOffset);
    rect.mSizeX = static_cast<s16>(view.unk0 + anotherX * 2);
    rect.mSizeY = static_cast<s16>(view.unk2 + anotherY * 2);
    return rect;
}

static CRect16 decomp_make_client_rect(const MockProc& proc) {
    CRect16 rect{};
    if (proc.viewIdCount == 0 || proc.firstView == nullptr) {
        rect.mPosX = 0;
        rect.mPosY = 0;
        rect.mSizeX = 0;
        rect.mSizeY = 0;
        return rect;
    }

    const MockView& view = *proc.firstView;
    const s16 posX = static_cast<s16>(view.frameOffsetX + view.unk54);
    const s16 posY = static_cast<s16>(view.frameOffsetY + view.unk56);
    const s16 anotherX = decomp_float_margin(view.unk0);
    const s16 anotherY = decomp_float_margin(view.unk2);
    const s16 childOffset = static_cast<s16>(view.childCount * 20);

    rect.mPosX = static_cast<s16>((posX - anotherX) + childOffset);
    rect.mPosY = static_cast<s16>((posY - anotherY) + childOffset);
    rect.mSizeX = static_cast<s16>(view.unk0 + anotherX * 2);
    rect.mSizeY = static_cast<s16>(view.unk2 + anotherY * 2);
    return rect;
}

static void build_view_name_retail(char* out, const char* suffix) {
    std::strcpy(out, "(View)");
    std::strcat(out, suffix);
}

static void build_view_name_decomp(char* out, const char* suffix) {
    std::strcpy(out, "(View)");
    std::strcat(out, suffix);
}

static bool rects_equal(const CRect16& a, const CRect16& b) {
    return a.mPosX == b.mPosX && a.mPosY == b.mPosY && a.mSizeX == b.mSizeX && a.mSizeY == b.mSizeY;
}

static void run_rect_scenario(const char* name, const MockProc& proc) {
    const CRect16 retail = retail_make_client_rect(proc);
    const CRect16 decomp = decomp_make_client_rect(proc);
    if (!rects_equal(retail, decomp)) {
        std::printf("FAIL %s retail=(%d,%d,%d,%d) decomp=(%d,%d,%d,%d)\n", name, retail.mPosX,
                    retail.mPosY, retail.mSizeX, retail.mSizeY, decomp.mPosX, decomp.mPosY,
                    decomp.mSizeX, decomp.mSizeY);
        std::exit(1);
    }
    std::printf("PASS %s\n", name);
}

static void run_name_scenario(const char* name, const char* suffix) {
    char retail[64]{};
    char decomp[64]{};
    build_view_name_retail(retail, suffix);
    build_view_name_decomp(decomp, suffix);
    if (std::strcmp(retail, decomp) != 0) {
        std::printf("FAIL %s retail=%s decomp=%s\n", name, retail, decomp);
        std::exit(1);
    }
    std::printf("PASS %s\n", name);
}

int main() {
    MockView views[4]{};

    run_rect_scenario("empty_view_list", MockProc{0, nullptr});
    run_rect_scenario("null_first_view", MockProc{1, nullptr});

    views[0] = MockView{640, 480, 0, 0, 10, 20, 0};
    run_rect_scenario("default_640x480_no_children", MockProc{1, &views[0]});

    views[1] = MockView{640, 480, 5, -3, 0, 0, 1};
    run_rect_scenario("one_child_offset", MockProc{1, &views[1]});

    views[2] = MockView{320, 240, -10, 15, 30, 40, 2};
    run_rect_scenario("two_children_frame_offset", MockProc{1, &views[2]});

    views[3] = MockView{100, 100, 0, 0, 0, 0, 3};
    run_rect_scenario("three_children_square", MockProc{1, &views[3]});

    views[0] = MockView{200, 150, 12, -8, 4, 6, 0};
    run_rect_scenario("small_rect_pos_bias", MockProc{1, &views[0]});

    views[1] = MockView{800, 600, -20, 25, -5, 7, 1};
    run_rect_scenario("large_rect_negative_frame", MockProc{1, &views[1]});

    views[2] = MockView{50, 50, 1, 1, 1, 1, 4};
    run_rect_scenario("tiny_four_children", MockProc{1, &views[2]});

    views[3] = MockView{640, 480, 100, 200, -100, -200, 0};
    run_rect_scenario("big_frame_translation", MockProc{1, &views[3]});

    views[0] = MockView{1, 1, 0, 0, 0, 0, 0};
    run_rect_scenario("min_size_one", MockProc{1, &views[0]});

    views[1] = MockView{127, 127, 0, 0, 0, 0, 0};
    run_rect_scenario("size_127", MockProc{1, &views[1]});

    views[2] = MockView{128, 128, 0, 0, 0, 0, 0};
    run_rect_scenario("size_128", MockProc{1, &views[2]});

    views[3] = MockView{255, 255, 0, 0, 0, 0, 0};
    run_rect_scenario("size_255", MockProc{1, &views[3]});

    views[0] = MockView{256, 256, 0, 0, 0, 0, 0};
    run_rect_scenario("size_256", MockProc{1, &views[0]});

    views[1] = MockView{400, 300, 3, -3, 9, -9, 1};
    run_rect_scenario("mid_size_mixed_sign", MockProc{1, &views[1]});

    views[2] = MockView{640, 480, 0, 0, 50, 60, 5};
    run_rect_scenario("five_children", MockProc{1, &views[2]});

    views[3] = MockView{640, 480, 0, 0, 0, 0, 10};
    run_rect_scenario("ten_children", MockProc{1, &views[3]});

    views[0] = MockView{-100, -50, 0, 0, 0, 0, 0};
    run_rect_scenario("negative_size_dims", MockProc{1, &views[0]});

    views[1] = MockView{640, 480, -50, -50, 25, 25, 2};
    run_rect_scenario("negative_frame_offset", MockProc{1, &views[1]});

    views[2] = MockView{300, 200, 10, 10, -10, -10, 3};
    run_rect_scenario("mixed_child_and_frame", MockProc{1, &views[2]});

    views[3] = MockView{512, 384, 7, 11, 13, 17, 6};
    run_rect_scenario("six_children_prime_offsets", MockProc{1, &views[3]});

    views[0] = MockView{640, 480, 1, 2, 3, 4, 7};
    run_rect_scenario("seven_children", MockProc{1, &views[0]});

    views[1] = MockView{1024, 768, 0, 0, 0, 0, 8};
    run_rect_scenario("eight_children_hd", MockProc{1, &views[1]});

    views[2] = MockView{160, 120, 0, 0, 5, 5, 9};
    run_rect_scenario("nine_children_low_res", MockProc{1, &views[2]});

    views[3] = MockView{640, 480, 0, 0, 0, 0, 15};
    run_rect_scenario("fifteen_children", MockProc{1, &views[3]});

    run_name_scenario("name_empty_suffix", "");
    run_name_scenario("name_game", "Game");
    run_name_scenario("name_scn", "Scn");
    run_name_scenario("name_long", "VeryLongChildViewNameForProc");
    run_name_scenario("name_digits", "12345");
    run_name_scenario("name_underscore", "_child");
    run_name_scenario("name_slash", "/sub");
    run_name_scenario("name_space", " child");

    std::printf("host proc_pss_create_view: all scenarios passed\n");
    return 0;
}
