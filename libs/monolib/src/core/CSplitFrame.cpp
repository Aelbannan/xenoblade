#include "libs/monolib/src/core/CSplitFrame.hpp"
#include "monolib/core/CViewRoot.hpp"
#include "monolib/core/CView.hpp"
#include "monolib/core/CViewFrame.hpp"
#include "monolib/math.hpp"
#include <types.h>

// us-8043df30: apply() — push the split rects to both child views. view1 gets
// the full pane (split line replaces the size along the split axis), view2 gets
// the remaining pane beyond the border. Runs only while both views exist.
extern "C" void apply__11CSplitFrameFv(CSplitFrame* self) {
    CView* view1 = CViewRoot::getView(self->mView1);
    CView* view2 = CViewRoot::getView(self->mView2);

    if (view1 != nullptr) {
        CView* parent = self->mParent;
        ml::CRect16 rect;
        rect.mPos.x = 0;
        rect.mPos.y = 0;
        rect.mSize.x = parent->mRectData.mBoundsSize.x;
        rect.mSize.y = parent->mRectData.mBoundsSize.y;
        if (self->mVertical != 0) {
            rect.mSize.y = self->mSplitY;
        } else {
            rect.mSize.x = self->mSplitX;
        }
        view1->setRect(rect);
    }

    if (view2 != nullptr) {
        // Decl order drives MWCC scratch coloring: boundsY/boundsX/parent born
        // in this order (parent's lwz still scheduled first) so retail's
        // r4/r5/r6 = boundsY/boundsX/parent assignment is reproduced.
        s16 boundsY;
        s16 boundsX;
        CView* parent;
        parent = self->mParent;
        ml::CRect16 rect;
        rect.mPos.x = 0;
        rect.mPos.y = 0;
        boundsX = parent->mRectData.mBoundsSize.x;
        rect.mSize.x = boundsX;
        boundsY = parent->mRectData.mBoundsSize.y;
        rect.mSize.y = boundsY;
        if (self->mVertical != 0) {
            rect.mPos.y = (s16)(self->mSplitY + parent->mFrame.mBorder);
            rect.mSize.y = (s16)(boundsY - (s16)(self->mSplitY + parent->mFrame.mBorder));
        } else {
            rect.mPos.x = (s16)(self->mSplitX + parent->mFrame.mBorder);
            rect.mSize.x = (s16)(boundsX - (s16)(self->mSplitX + parent->mFrame.mBorder));
        }
        view2->setRect(rect);
    }
}


extern "C" CView* getView1__11CSplitFrameFv(CSplitFrame* self) {
    return CViewRoot::getView(self->mView1);
}

extern "C" CView* getView2__11CSplitFrameFv(CSplitFrame* self) {
    return CViewRoot::getView(self->mView2);
}

extern "C" bool isActive__11CSplitFrameFv(CSplitFrame* self) {
    CView* view1 = CViewRoot::getView(self->mView1);
    CView* view2 = CViewRoot::getView(self->mView2);
    return view1 != nullptr || view2 != nullptr;
}

// static member: CSplitFrame::getScissorRect1(ml::CRect16&, const CSplitFrame*)
void CSplitFrame::getScissorRect1(ml::CRect16& out, const CSplitFrame* self) {
    ml::CRect16 split;
    ml::CRect16 offset;
    CView* view = self->mParent;
    getFrame2ViewOffset__10CViewFrameFR7CRect16PC10CViewFrame(&offset, &view->mFrame);

    // Split rect built directly first; the local reference below makes MWCC
    // keep the rect memory-resident for the check/clamp math like retail.
    // Split rect built directly first; the local reference below makes MWCC
    // keep the rect memory-resident for the check/clamp math like retail.
    split.mPos.x = 0;
    split.mPos.y = 0;
    s16 boundW = view->mRectData.mViewSize.x;
    s16 boundH = view->mRectData.mViewSize.y;
    split.mSize.x = view->mRectData.mBoundsSize.x;
    split.mSize.y = view->mRectData.mBoundsSize.y;
    ml::CRect16& r = split;
    if (self->mVertical != 0) {
        r.mSize.y = self->mSplitY;
    } else {
        r.mSize.x = self->mSplitX;
    }

    // Per-axis validity check. On each axis: span = max(sx+w, bound) -
    // min(sx, 0), sum = bound + w, t = sum ^ span; the axis is valid when
    // (t >> 1) - (t & sum) is negative. X is checked first; only if it passes
    // is Y checked. Either failing yields a zeroed rect.
    s16 edge = (s16)(r.mPos.x + r.mSize.x);
    if (boundW > edge) {
        edge = boundW;
    }
    s32 mask = ((-(s32)r.mPos.x) & ~(s32)r.mPos.x) >> 31;
    s16 lo = (s16)((s32)r.mPos.x & ~mask);
    s32 sum = (s32)boundW + (s32)r.mSize.x;
    s16 span = (s16)(edge - lo);
    s32 t = sum ^ (s32)span;
    s16 flag = (s16)((u32)((t >> 1) - (t & sum)) >> 31);

    if (flag) {
        edge = (s16)(r.mPos.y + r.mSize.y);
        if (boundH > edge) {
            edge = boundH;
        }
        mask = ((-(s32)r.mPos.y) & ~(s32)r.mPos.y) >> 31;
        lo = (s16)((s32)r.mPos.y & ~mask);
        sum = (s32)boundH + (s32)r.mSize.y;
        span = (s16)(edge - lo);
        t = sum ^ (s32)span;
        flag = (s16)((u32)((t >> 1) - (t & sum)) >> 31);
    }

    if (!flag) {
        out.mPos.x = 0;
        out.mPos.y = 0;
        out.mSize.x = 0;
        out.mSize.y = 0;
        return;
    }

    // Clamp: origin at max(s, 0), end at min(s+w, bound).
    s16 x0 = (s16)((s32)r.mPos.x & ~((s32)r.mPos.x >> 31));
    s16 y0 = (s16)((s32)r.mPos.y & ~((s32)r.mPos.y >> 31));
    s16 x1 = (s16)(r.mPos.x + r.mSize.x);
    if (x1 > boundW) {
        x1 = boundW;
    }
    s16 y1 = (s16)(r.mPos.y + r.mSize.y);
    if (y1 > boundH) {
        y1 = boundH;
    }

    out.mPos.x = x0;
    out.mPos.y = y0;
    out.mSize.x = (s16)(x1 - x0);
    out.mSize.y = (s16)(y1 - y0);
}

// static member: CSplitFrame::getScissorRect2(ml::CRect16&, const CSplitFrame*)
// Same validity/clamp idiom as getScissorRect1, but the split rect is offset
// by the frame-to-view offset and shifted past the border along the split
// axis before the check.
void CSplitFrame::getScissorRect2(ml::CRect16& out, const CSplitFrame* self) {
    ml::CRect16 split;
    ml::CRect16 offset;
    CView* view = self->mParent;
    getFrame2ViewOffset__10CViewFrameFR7CRect16PC10CViewFrame(&offset, &view->mFrame);

    s16 boundsX = view->mRectData.mBoundsSize.x;
    CView* v = self->mParent; // rebirth of the parent web (retail r8 copy)
    s16 boundsY = view->mRectData.mBoundsSize.y;
    s16 viewW = view->mRectData.mViewSize.x;
    s16 viewH = view->mRectData.mViewSize.y;
    split.mPos.x = 0;
    split.mPos.y = 0;
    split.mSize.x = boundsX;
    split.mSize.y = boundsY;
    if (self->mVertical != 0) {
        s16 y = (s16)(self->mSplitY + v->mFrame.mBorder);
        split.mPos.y = y;
        split.mSize.y = (s16)(boundsY - y);
    } else {
        s16 x = (s16)(self->mSplitX + v->mFrame.mBorder);
        split.mPos.x = x;
        split.mSize.x = (s16)(boundsX - x);
    }

    // Per-axis validity check, identical idiom to getScissorRect1: the axis is
    // valid when (t >> 1) - (t & sum) has its sign bit set, where span is the
    // clamped extent and sum = bound + size.
    ml::CRect16& r = split;
    s16 edge = (s16)(r.mPos.x + r.mSize.x);
    if (viewW > edge) {
        edge = viewW;
    }
    s32 mask = ((-(s32)r.mPos.x) & ~(s32)r.mPos.x) >> 31;
    s16 lo = (s16)((s32)r.mPos.x & ~mask);
    s32 sum = (s32)viewW + (s32)r.mSize.x;
    s16 span = (s16)(edge - lo);
    s32 t = sum ^ (s32)span;
    s16 flag = (s16)((u32)((t >> 1) - (t & sum)) >> 31);

    if (flag) {
        edge = (s16)(r.mPos.y + r.mSize.y);
        if (viewH > edge) {
            edge = viewH;
        }
        mask = ((-(s32)r.mPos.y) & ~(s32)r.mPos.y) >> 31;
        lo = (s16)((s32)r.mPos.y & ~mask);
        sum = (s32)viewH + (s32)r.mSize.y;
        span = (s16)(edge - lo);
        t = sum ^ (s32)span;
        flag = (s16)((u32)((t >> 1) - (t & sum)) >> 31);
    }

    if (!flag) {
        out.mPos.x = 0;
        out.mPos.y = 0;
        out.mSize.x = 0;
        out.mSize.y = 0;
        return;
    }

    // Clamp: end at min(s+w, bound), origin at max(s, 0). x1 is formed first
    // so its size operand reuses the validity check's reload like retail.
    s16 x1 = (s16)(r.mPos.x + r.mSize.x);
    if (x1 > viewW) {
        x1 = viewW;
    }
    s16 x0 = (s16)((s32)r.mPos.x & ~((s32)r.mPos.x >> 31));
    s16 y0 = (s16)((s32)r.mPos.y & ~((s32)r.mPos.y >> 31));
    s16 y1 = (s16)(r.mPos.y + r.mSize.y);
    if (y1 > viewH) {
        y1 = viewH;
    }

    out.mPos.x = x0;
    out.mPos.y = y0;
    out.mSize.x = (s16)(x1 - x0);
    out.mSize.y = (s16)(y1 - y0);
}

extern "C" s16 getSplitLine__11CSplitFrameFv(u8* self) {
    if (*(u8*)((u8*)self + 8) != 0) return *(s16*)((u8*)self + 0xC);
    return *(s16*)((u8*)self + 0xA);
}


// us-8043e0ec: setSplitLine() — store the split line, clamp it against the
// parent viewport (lower: 0 / -border, upper: viewSize + border / viewSize),
// then re-apply the split rects to both child views like apply().
extern "C" void setSplitLine__11CSplitFrameFs(CSplitFrame* self, s16 line) {
    if (self->mVertical != 0) {
        self->mSplitY = line;
    } else {
        self->mSplitX = line;
    }

    CView* parent = self->mParent;
    ml::CRect16 rect2;
    ml::CRect16 rect1;
    ml::CRect16 offset;
    getFrame2ViewOffset__10CViewFrameFR7CRect16PC10CViewFrame(&offset, &parent->mFrame);

    s16 viewSizeX = parent->mRectData.mViewSize.x;
    s16 viewSizeY = parent->mRectData.mViewSize.y;

    if (self->mSplitX < 0) {
        self->mSplitX = 0;
    }

    {
        s32 maxX = (s32)viewSizeX + self->mParent->mFrame.mBorder;
        if (self->mSplitX > (s16)maxX) {
            self->mSplitX = (s16)maxX;
        }
    }

    {
        s32 minY = -(s32)self->mParent->mFrame.mBorder;
        if (self->mSplitY < (s16)minY) {
            self->mSplitY = (s16)minY;
        }
    }

    if (self->mSplitY > viewSizeY) {
        self->mSplitY = viewSizeY;
    }

    // rect1: full pane for view1 (split line replaces size along the axis).
    {
        CView* parent = self->mParent;
        rect1.mPos.x = 0;
        rect1.mPos.y = 0;
        rect1.mSize.x = parent->mRectData.mBoundsSize.x;
        rect1.mSize.y = parent->mRectData.mBoundsSize.y;
        if (self->mVertical != 0) {
            rect1.mSize.y = self->mSplitY;
        } else {
            rect1.mSize.x = self->mSplitX;
        }
    }
    CView* view1 = CViewRoot::getView(self->mView1);
    view1->setRect(rect1);

    // rect2: remaining pane beyond the border for view2.
    {
        // Same decl-order lever as apply(): boundsY/boundsX/parent birth order
        // yields retail's r4/r5/r6 scratch coloring for the subf operands.
        s16 boundsY;
        s16 boundsX;
        CView* parent;
        parent = self->mParent;
        rect2.mPos.x = 0;
        rect2.mPos.y = 0;
        boundsX = parent->mRectData.mBoundsSize.x;
        rect2.mSize.x = boundsX;
        boundsY = parent->mRectData.mBoundsSize.y;
        rect2.mSize.y = boundsY;
        if (self->mVertical != 0) {
            rect2.mPos.y = (s16)(self->mSplitY + parent->mFrame.mBorder);
            rect2.mSize.y = (s16)(boundsY - (s16)(self->mSplitY + parent->mFrame.mBorder));
        } else {
            rect2.mPos.x = (s16)(self->mSplitX + parent->mFrame.mBorder);
            rect2.mSize.x = (s16)(boundsX - (s16)(self->mSplitX + parent->mFrame.mBorder));
        }
    }
    CView* view2 = CViewRoot::getView(self->mView2);
    view2->setRect(rect2);
}
