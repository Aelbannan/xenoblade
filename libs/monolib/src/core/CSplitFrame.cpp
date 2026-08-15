#include "monolib/core/CSplitFrame.hpp"
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

void getScissorRect1(ml::CRect16* out, const CSplitFrame* self) {
    // MWCC: first local = higher addr. Retail wants split@sp+0x10, offset@sp+0x8.
    volatile ml::CRect16 split;
    volatile ml::CRect16 offset;
    CView* view = self->mParent;
    getFrame2ViewOffset__10CViewFrameFR7CRect16PC10CViewFrame(
        (ml::CRect16*)&offset, &view->mFrame);

    // Retail post-bl schedule: lbz/li, lha size@1cc/1ce, cmp, sth zeros,
    // lha bound@1c8/1ca interleaved with sth size.
    s16 zero = 0;
    u8 vert = self->mVertical;
    s16 sizeX = view->mRectData.mBoundsSize.x;
    s16 sizeY = view->mRectData.mBoundsSize.y;
    s16 boundW;
    s16 boundH;
    if ((vert != 0, split.mPos.x = zero, boundW = view->mRectData.mViewSize.x,
         split.mPos.y = zero, boundH = view->mRectData.mViewSize.y,
         split.mSize.x = sizeX, split.mSize.y = sizeY, vert != 0)) {
        split.mSize.y = self->mSplitY;
    } else {
        split.mSize.x = self->mSplitX;
    }

    s16 sx = split.mPos.x;
    s16 sy = split.mPos.y;
    s16 sw = split.mSize.x;
    s16 sh = split.mSize.y;

    // Retail max(boundW, sx+sw) / max(0,sx) then signed-overlap via xor/srawi.
    s16 edge = (s16)(sx + sw);
    if (boundW > edge) {
        edge = boundW;
    }
    s32 mask = ((-(s32)sx) & ~(s32)sx) >> 31;
    s16 lo = (s16)((s32)sx & ~mask);
    s16 span = (s16)(edge - lo);
    s16 sum = (s16)(boundW + sw);
    s32 t = (s32)sum ^ (s32)span;
    s32 u = (t >> 1) - (t & (s32)sum);
    s16 flag = (s16)((u32)u >> 31);

    if (flag) {
        edge = (s16)(sy + sh);
        if (boundH > edge) {
            edge = boundH;
        }
        mask = ((-(s32)sy) & ~(s32)sy) >> 31;
        lo = (s16)((s32)sy & ~mask);
        span = (s16)(edge - lo);
        sum = (s16)(boundH + sh);
        t = (s32)sum ^ (s32)span;
        u = (t >> 1) - (t & (s32)sum);
        flag = (s16)((u32)u >> 31);
    }

    if (flag) {
        out->mPos.x = 0;
        out->mPos.y = 0;
        out->mSize.x = 0;
        out->mSize.y = 0;
        return;
    }

    s16 x0 = sx;
    mask = (s32)sx >> 31;
    x0 = (s16)((s32)sx & ~mask);
    s16 y0 = sy;
    mask = (s32)sy >> 31;
    y0 = (s16)((s32)sy & ~mask);

    s16 x1 = (s16)(sx + sw);
    if (x1 > boundW) {
        x1 = boundW;
    }
    s16 y1 = (s16)(sy + sh);
    if (y1 > boundH) {
        y1 = boundH;
    }

    out->mPos.x = x0;
    out->mPos.y = y0;
    out->mSize.x = (s16)(x1 - x0);
    out->mSize.y = (s16)(y1 - y0);
}

void getScissorRect2(ml::CRect16* out, const CSplitFrame* self) {
    volatile ml::CRect16 split;
    volatile ml::CRect16 offset;
    CView* view = self->mParent;
    getFrame2ViewOffset__10CViewFrameFR7CRect16PC10CViewFrame(
        (ml::CRect16*)&offset, &view->mFrame);

    s16 zero = 0;
    u8 vert = self->mVertical;
    s16 sizeX = view->mRectData.mBoundsSize.x;
    s16 sizeY = view->mRectData.mBoundsSize.y;
    s16 boundW;
    s16 boundH;
    s16 border;
    if ((vert != 0, split.mPos.x = zero, boundW = view->mRectData.mViewSize.x,
         split.mPos.y = zero, boundH = view->mRectData.mViewSize.y,
         split.mSize.x = sizeX, split.mSize.y = sizeY,
         border = view->mFrame.mBorder, vert != 0)) {
        s16 y = (s16)(self->mSplitY + border);
        split.mPos.y = y;
        split.mSize.y = (s16)(split.mSize.y - y);
    } else {
        s16 x = (s16)(self->mSplitX + border);
        split.mPos.x = x;
        split.mSize.x = (s16)(split.mSize.x - x);
    }

    s16 sx = split.mPos.x;
    s16 sy = split.mPos.y;
    s16 sw = split.mSize.x;
    s16 sh = split.mSize.y;

    s16 edge = (s16)(sx + sw);
    if (edge > boundW) {
        edge = boundW;
    }
    s32 mask = ((-(s32)sx) & ~(s32)sx) >> 31;
    s16 lo = (s16)((s32)sx & ~mask);
    s16 span = (s16)(edge - lo);
    s16 sum = (s16)(boundW + sw);
    s32 t = (s32)sum ^ (s32)span;
    s32 u = (t >> 1) - (t & (s32)sum);
    s16 flag = (s16)((u32)u >> 31);

    if (flag) {
        edge = (s16)(sy + sh);
        if (edge > boundH) {
            edge = boundH;
        }
        mask = ((-(s32)sy) & ~(s32)sy) >> 31;
        lo = (s16)((s32)sy & ~mask);
        span = (s16)(edge - lo);
        sum = (s16)(boundH + sh);
        t = (s32)sum ^ (s32)span;
        u = (t >> 1) - (t & (s32)sum);
        flag = (s16)((u32)u >> 31);
    }

    if (flag) {
        out->mPos.x = 0;
        out->mPos.y = 0;
        out->mSize.x = 0;
        out->mSize.y = 0;
        return;
    }

    s16 x0 = sx;
    mask = (s32)sx >> 31;
    x0 = (s16)((s32)sx & ~mask);
    s16 y0 = sy;
    mask = (s32)sy >> 31;
    y0 = (s16)((s32)sy & ~mask);

    s16 x1 = (s16)(sx + sw);
    if (x1 > boundW) {
        x1 = boundW;
    }
    s16 y1 = (s16)(sy + sh);
    if (y1 > boundH) {
        y1 = boundH;
    }

    out->mPos.x = x0;
    out->mPos.y = y0;
    out->mSize.x = (s16)(x1 - x0);
    out->mSize.y = (s16)(y1 - y0);
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
