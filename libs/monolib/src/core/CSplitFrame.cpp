#include "monolib/core/CSplitFrame.hpp"
#include "monolib/core/CViewRoot.hpp"
#include "monolib/core/CView.hpp"
#include "monolib/core/CViewFrame.hpp"
#include "monolib/math.hpp"
#include <types.h>

extern "C" void getFrame2ViewOffset__10CViewFrameFR7CRect16PC10CViewFrame(
    ml::CRect16* out, const CViewFrame* frame);

// LLM-HARNESS-BEGIN: us-8043df30
extern "C" void apply__11CSplitFrameFv(CSplitFrame* self) {
    // Implementation pending - currently NonMatching
}
// LLM-HARNESS-END: us-8043df30

// LLM-HARNESS-BEGIN: us-8043e060
extern "C" CView* getView1__11CSplitFrameFv(CSplitFrame* self) {
    return CViewRoot::getView(self->mView1);
}
// LLM-HARNESS-END: us-8043e060

// LLM-HARNESS-BEGIN: us-8043e068
extern "C" CView* getView2__11CSplitFrameFv(CSplitFrame* self) {
    return CViewRoot::getView(self->mView2);
}
// LLM-HARNESS-END: us-8043e068

// LLM-HARNESS-BEGIN: us-8043e070
extern "C" bool isActive__11CSplitFrameFv(CSplitFrame* self) {
    return getView1__11CSplitFrameFv(self) != nullptr || getView2__11CSplitFrameFv(self) != nullptr;
}
// LLM-HARNESS-END: us-8043e070

// LLM-HARNESS-BEGIN: us-8043e288
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

    if (flag != 0) {
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

    if (flag == 0) {
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
    if (boundW < x1) {
        x1 = boundW;
    }
    s16 y1 = (s16)(sy + sh);
    if (boundH < y1) {
        y1 = boundH;
    }

    out->mPos.x = x0;
    out->mPos.y = y0;
    out->mSize.x = (s16)(x1 - x0);
    out->mSize.y = (s16)(y1 - y0);
}
// LLM-HARNESS-END: us-8043e288

// LLM-HARNESS-BEGIN: us-8043e43c
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

    if (flag != 0) {
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

    if (flag == 0) {
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
    if (boundW < x1) {
        x1 = boundW;
    }
    s16 y1 = (s16)(sy + sh);
    if (boundH < y1) {
        y1 = boundH;
    }

    out->mPos.x = x0;
    out->mPos.y = y0;
    out->mSize.x = (s16)(x1 - x0);
    out->mSize.y = (s16)(y1 - y0);
}
// LLM-HARNESS-END: us-8043e43c
