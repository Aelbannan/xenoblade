#include "monolib/core/CViewFrame.hpp"
#include "monolib/core/CView.hpp"
#include "monolib/core/CDrawGX.hpp"
#include "monolib/core/CViewRectData.hpp"

extern "C" {
void func_8043E58C__5CViewFRQ22ml5CRectP5CView(ml::CRect* rect, CView* view);
void func_80442B54__9CViewRootFPvPv(void* a, void* b, void* c);
int func_8043CAFC__5CViewFv(CView* view);
void func_80442C68__9CViewRootFv();
float lbl_eu_8066A318;
float lbl_eu_8066A2F4;

// Undefined here so MWCC cannot DCE same-TU empty stubs.
void func_804406D8__10CViewFrameFPv(CViewFrame* self, CDrawGX* draw);
void func_804409D0__10CViewFrameFPvPv(CViewFrame* self, CDrawGX* draw, ml::CRect16* rect);
void func_80440D78__10CViewFrameFPvPv(CViewFrame* self, CDrawGX* draw, ml::CPnt16* pos);
}

// Render the view frame: compute the visible rectangle (with optional
// border expansion for menu/target frames), clip to parent, and draw
// border quads via CDrawGX. Returns false if the frame has no owner or
// the computed content rect is degenerate (zero width/height).
bool CViewFrame::render() {
    // Decl order first = higher addr. Target: col@0x30 view@0x28 frame@0x20
    // early rect@0x18 clip@0x10 scratch@0x08; CDrawGX nested late @0x40.
    ml::CCol4 col;
    ml::CRect viewRect;
    ml::CRect16 frameRect;
    // Volatile: retail sth of mPos (0x230/0x232) is otherwise DCE'd.
    volatile ml::CRect16 rect;
    ml::CRect16 clipRect;
    ml::CPnt16 scratchPos;

    // Volatile check so the mOwner reload below is not CSE'd into the cmp load.
    if (*(CView* volatile*)&mOwner == NULL) {
        return false;
    }

    // Scope early expand locals so they cannot bleed into the post-bl own/r3 schedule.
    // expand declared first: keep live across rect lhas so Chaitin prefers r4 (arg home).
    {
        int expand = 0;
        CView* owner = mOwner;
        CView* view = owner->mFrame.mOwner;
        u32 flags = view->unk27C;
        // Volatile loads pin 0x230/232/1c8/1ca order; expand already live → retail r6/r5/r3/r0.
        s16 px = *(volatile s16*)&view->mFrame.mContentX;
        s16 py = *(volatile s16*)&view->mFrame.mContentY;
        s16 sx = *(volatile s16*)&view->mRectData.mViewSize.x;
        s16 sy = *(volatile s16*)&view->mRectData.mViewSize.y;
        ml::CRect16* r = (ml::CRect16*)&rect;
        r->mPos.x = px;
        r->mPos.y = py;
        r->mSize.x = sx;
        r->mSize.y = sy;

        if ((flags & 1) != 0) {
            u32 mode = view->unk278;
            if ((mode & 1) == 0 && (mode & 2) == 0) {
                expand = 1;
            }
        }

        if (expand != 0) {
            view = *(CView* volatile*)&owner->mFrame.mOwner;
            expand = 0;

            {
                s16 border = owner->mFrame.mBorder;
                rect.mSize.x = (s16)(rect.mSize.x + (s16)(border * 2));
            }

            if ((view->unk27C & 2) != 0) {
                u32 mode = view->unk278;
                if ((mode & 1) == 0 && (mode & 2) == 0) {
                    expand = 1;
                }
            }

            // Separate scopes so width-path border is dead before these lha 0x234.
            if (expand != 0) {
                s16 b1 = owner->mFrame.mBorder;
                rect.mSize.y =
                    (s16)(rect.mSize.y + (s16)(b1 * 3 + 0x16));
            } else {
                s16 b2 = owner->mFrame.mBorder;
                rect.mSize.y = (s16)(rect.mSize.y + (s16)(b2 * 2));
            }
        }
    }

    {
        // Retail ble-shared badSize set (PLAN.md goto-gate style).
        int badSize = 0;
        if (rect.mSize.x <= 0) {
            goto set_bad_size;
        }
        if (rect.mSize.y > 0) {
            goto after_bad_size;
        }
    set_bad_size:
        badSize = 1;
    after_bad_size:
        if (badSize != 0) {
            return false;
        }
    }

    scratchPos.x = 0;
    scratchPos.y = 0;
    func_8043E58C__5CViewFRQ22ml5CRectP5CView(&viewRect, mOwner);

    clipRect.mPos.x = 0;
    clipRect.mPos.y = 0;
    clipRect.mSize.x = viewRect.mSize.x;
    clipRect.mSize.y = viewRect.mSize.y;
    func_80442B54__9CViewRootFPvPv(&viewRect, &viewRect, &clipRect);

    {
        s16 width;
        s16 height;
        int flagExpand;
        int flagInner;
        s16 border;

        // Fresh owner local after bl; flag zeros claim r4/r5 like retail.
        CView* own = mOwner;
        flagExpand = 0;
        flagInner = 0;
        CView* vw = own->mFrame.mOwner;
        width = vw->mRectData.mViewSize.x;
        height = vw->mRectData.mViewSize.y;

        if ((vw->unk27C & 1) != 0) {
            if ((vw->unk278 & 1) == 0) {
                flagInner = 1;
            }
        }
        if (flagInner != 0) {
            if ((vw->unk278 & 2) == 0) {
                flagExpand = 1;
            }
        }

        if (flagExpand != 0) {
            border = own->mFrame.mBorder;
            flagExpand = 0;
            // No outer (s16) on the sum: retail does add r31,r31,r0 without final extsh.
            width += (s16)(border * 2);

            if ((vw->unk27C & 2) != 0) {
                u32 mode = vw->unk278;
                if ((mode & 1) == 0 && (mode & 2) == 0) {
                    flagExpand = 1;
                }
            }

            if (flagExpand != 0) {
                height = (s16)(height + (s16)(border * 3 + 0x16));
            } else {
                height = (s16)(height + (s16)(border * 2));
            }
        }

        {
            CDrawGX draw;
            int drawBorder;
            int drawSplit;

            draw.func_80456570(0);

            col = mFrameColor;
            {
                float opacity = mOwner->mAlpha;
                col.a = col.a * opacity;
            }
            draw.setCol(col);

            if (func_8043CAFC__5CViewFv(mOwner) != 0) {
                func_80440D78__10CViewFrameFPvPv(this, &draw, &scratchPos);
            }

            drawBorder = 0;
            own = mOwner;
            if ((own->unk27C & 1) != 0) {
                u32 mode = own->unk278;
                if ((mode & 1) == 0 && (mode & 2) == 0) {
                    drawBorder = 1;
                }
            }

            if (drawBorder != 0) {
                drawSplit = 0;
                frameRect.mPos.x = 0;
                frameRect.mPos.y = 0;
                frameRect.mSize.x = width;
                frameRect.mSize.y = height;

                own = mOwner;
                if ((own->unk27C & 2) != 0) {
                    u32 mode = own->unk278;
                    if ((mode & 1) == 0 && (mode & 2) == 0) {
                        drawSplit = 1;
                    }
                }

                if (drawSplit != 0) {
                    func_804406D8__10CViewFrameFPv(this, &draw);
                    s16 adj = (s16)(mBorder + 0x16);
                    frameRect.mPos.y = (s16)(frameRect.mPos.y + adj);
                    frameRect.mSize.y = (s16)(frameRect.mSize.y - adj);
                }

                func_804409D0__10CViewFrameFPvPv(this, &draw, &frameRect);
            }

            func_80442C68__9CViewRootFv();
        }
    }

    return true;
}

// Draw the 8-segment frame border into the CDrawGX batch: 4 outer
// edge rectangles (left, bottom, right, top) with the frame colour,
// then 4 inner 1px highlight/outline lines (right+bottom outer
// outline, top+left inner highlight) at the scaled-down colour.
// @param self  CViewFrame with mBorder thickness and mFrameColor.
// @param draw  Target CDrawGX draw context.
// @param rect  Bounding rectangle of the frame area to draw.
extern "C" void func_804409D0__10CViewFrameFPvPv(CViewFrame* self, CDrawGX* draw,
                                                 ml::CRect16* rect) {
    ml::CCol4 col;
    ml::CRect16 piece;
    s16 border = self->mBorder;
    float scale = lbl_eu_8066A318;
    float opacity = self->mOwner->mAlpha;

    col.r = self->mFrameColor.r * scale;
    col.g = self->mFrameColor.g * scale;
    col.b = self->mFrameColor.b * scale;
    col.a = self->mFrameColor.a * opacity;
    draw->setCol(col);

    draw->begin(9, 1);
    piece.mPos.x = rect->mPos.x;
    piece.mPos.y = rect->mPos.y;
    piece.mSize.x = border;
    piece.mSize.y = rect->mSize.y;
    draw->add(piece);
    draw->end();

    draw->begin(9, 1);
    piece.mPos.x = rect->mPos.x;
    piece.mPos.y = (s16)((s16)(rect->mPos.y + rect->mSize.y) - border);
    piece.mSize.x = rect->mSize.x;
    piece.mSize.y = border;
    draw->add(piece);
    draw->end();

    draw->begin(9, 1);
    piece.mPos.x = (s16)((s16)(rect->mPos.x + rect->mSize.x) - border);
    piece.mPos.y = rect->mPos.y;
    piece.mSize.x = border;
    piece.mSize.y = rect->mSize.y;
    draw->add(piece);
    draw->end();

    draw->begin(9, 1);
    piece.mPos.x = rect->mPos.x;
    piece.mPos.y = rect->mPos.y;
    piece.mSize.x = rect->mSize.x;
    piece.mSize.y = border;
    draw->add(piece);
    draw->end();

    scale = lbl_eu_8066A2F4;
    col.r = self->mFrameColor.r * scale;
    col.g = self->mFrameColor.g * scale;
    col.b = self->mFrameColor.b * scale;
    col.a = self->mFrameColor.a * opacity;
    draw->setCol(col);

    draw->begin(9, 1);
    piece.mPos.x = (s16)((s16)(border + rect->mPos.x) - 1);
    piece.mPos.y = (s16)(rect->mPos.y + border);
    piece.mSize.x = 1;
    piece.mSize.y = (s16)(rect->mSize.y - (s16)(border * 2));
    draw->add(piece);
    draw->end();

    draw->begin(9, 1);
    piece.mPos.x = (s16)(rect->mPos.x + border);
    piece.mPos.y = (s16)((s16)(border + rect->mPos.y) - 1);
    piece.mSize.x = (s16)(rect->mSize.x - (s16)(border * 2));
    piece.mSize.y = 1;
    draw->add(piece);
    draw->end();

    draw->begin(9, 1);
    piece.mPos.x = (s16)(rect->mPos.x - 1);
    piece.mPos.y = (s16)((s16)(rect->mPos.y + rect->mSize.y) - 1);
    piece.mSize.x = rect->mSize.x;
    piece.mSize.y = 1;
    draw->add(piece);
    draw->end();

    draw->begin(9, 1);
    piece.mPos.x = (s16)((s16)(rect->mPos.x + rect->mSize.x) - 1);
    piece.mPos.y = rect->mPos.y;
    piece.mSize.x = 1;
    piece.mSize.y = rect->mSize.y;
    draw->add(piece);
    draw->end();
}

// LLM-HARNESS-BEGIN: us-80442564
// CViewFrame default constructor — empty; initialisation is done by the
// placement-new caller or caller-side inline init.
extern "C" void __ct__CViewFrame() {}
// LLM-HARNESS-END: us-80442564
// LLM-HARNESS-BEGIN: us-80442600
// Compute the offset from the frame's outer rect to its viewport content
// area (i.e. how much the content region is inset by border + split gap).
// Only applies when the owner view has border-expand/split flags enabled.
// @param out    Receives the offset as a position vector.
// @param frame  The frame whose border inset to compute.
extern "C" void getFrame2ViewOffset__10CViewFrameFR7CRect16PC10CViewFrame(
    ml::CRect16* out, const CViewFrame* frame) {
    out->mPos.x = 0;
    out->mPos.y = 0;

    CView* owner = frame->mOwner;
    int apply = 0;
    if ((owner->unk27C & 1) != 0) {
        u32 flags = owner->unk278;
        if ((flags & 1) == 0 && (flags & 2) == 0) {
            apply = 1;
        }
    }
    if (apply == 0) {
        return;
    }

    out->mPos.x = (s16)(out->mPos.x + frame->mBorder);
    out->mPos.y = (s16)(out->mPos.y + frame->mBorder);

    apply = 0;
    owner = frame->mOwner;
    if ((owner->unk27C & 2) != 0) {
        u32 flags = owner->unk278;
        if ((flags & 1) == 0 && (flags & 2) == 0) {
            apply = 1;
        }
    }
    if (apply == 0) {
        return;
    }

    out->mPos.y = (s16)(out->mPos.y + (s16)(frame->mBorder + 0x16));
}
// LLM-HARNESS-END: us-80442600

// Get the client rect of the frame's owner view, optionally expanded
// by the border thickness (same gate logic as render()). The result
// is the visible content area in the owner's coordinate space.
// @param out    Receives the client rectangle.
// @param frame  The frame whose owner's client rect to query.
extern "C" void func_8043FD10__10CViewFrameFR7CRect16PC10CViewFrame(
    ml::CRect16* out, const CViewFrame* frame) {
    CView* view = frame->mOwner;
    int expand = 0;

    out->mPos.x = view->mFrame.mContentX;
    out->mPos.y = view->mFrame.mContentY;
    out->mSize.x = view->mRectData.mViewSize.x;
    out->mSize.y = view->mRectData.mViewSize.y;

    if ((view->unk27C & 1) != 0) {
        u32 mode = view->unk278;
        if ((mode & 1) == 0 && (mode & 2) == 0) {
            expand = 1;
        }
    }
    if (expand == 0) {
        return;
    }

    {
        s16 border = frame->mBorder;
        out->mSize.x = (s16)(out->mSize.x + (s16)(border * 2));
    }

    expand = 0;
    view = frame->mOwner;
    if ((view->unk27C & 2) != 0) {
        u32 mode = view->unk278;
        if ((mode & 1) == 0 && (mode & 2) == 0) {
            expand = 1;
        }
    }
    if (expand != 0) {
        s16 border = frame->mBorder;
        out->mSize.y = (s16)(out->mSize.y + (s16)(border * 3 + 0x16));
    } else {
        s16 border = frame->mBorder;
        out->mSize.y = (s16)(out->mSize.y + (s16)(border * 2));
    }
}

// LLM-HARNESS-BEGIN: us-80444550
// Detach the frame from a CWorkThread's render list. Currently a stub;
// the retail function unlinks from a linked-list render-work chain.
extern "C" void detachRenderWork__10CViewFrameFP11CWorkThread() {}
// LLM-HARNESS-END: us-80444550
