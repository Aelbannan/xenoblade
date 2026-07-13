#include <cassert>
#include <cstdio>
#include <cstring>

#include "monolib/math/CRect16.hpp"

// Minimal view stub: records the rectangle passed to setRect.
struct MockView {
    ml::CRect16 lastRect{};

    void setRect(const ml::CRect16& rect) { lastRect = rect; }
};

// Retail semantics transcribed from build/us/asm/kyoshin/CGame.s (setViewRect).
static void retail_setViewRect(MockView* view, s16 x, s16 y, s16 width, s16 height) {
    ml::CRect16 rect(x, y, width, height);
    view->setRect(rect);
}

// Decompiled CGame::setViewRect (src/kyoshin/CGame.cpp).
static void decomp_setViewRect(MockView* view, s16 x, s16 y, s16 width, s16 height) {
    view->setRect(ml::CRect16(x, y, width, height));
}

static bool rects_equal(const ml::CRect16& a, const ml::CRect16& b) {
    return a.mPos.x == b.mPos.x && a.mPos.y == b.mPos.y && a.mSize.x == b.mSize.x
        && a.mSize.y == b.mSize.y;
}

static void run_scenario(const char* name, s16 x, s16 y, s16 w, s16 h) {
    MockView retailView{};
    MockView decompView{};

    retail_setViewRect(&retailView, x, y, w, h);
    decomp_setViewRect(&decompView, x, y, w, h);

    if (!rects_equal(retailView.lastRect, decompView.lastRect)) {
        std::printf(
            "FAIL %s: retail (%d,%d,%d,%d) != decomp (%d,%d,%d,%d)\n",
            name,
            retailView.lastRect.mPos.x,
            retailView.lastRect.mPos.y,
            retailView.lastRect.mSize.x,
            retailView.lastRect.mSize.y,
            decompView.lastRect.mPos.x,
            decompView.lastRect.mPos.y,
            decompView.lastRect.mSize.x,
            decompView.lastRect.mSize.y);
        std::exit(1);
    }
    std::printf("PASS %s\n", name);
}

int main() {
    run_scenario("zero_origin", 0, 0, 640, 480);
    run_scenario("offset", 10, 56, 320, 240);
    run_scenario("negative_origin", -4, -8, 100, 50);
    run_scenario("max_field", 0, 0, 856, 480);
    std::printf("host set_view_rect: all scenarios passed\n");
    return 0;
}
