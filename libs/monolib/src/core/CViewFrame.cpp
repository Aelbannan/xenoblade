#include "monolib/core/CViewFrame.hpp"
#include "monolib/core/CView.hpp"
#include "monolib/core/CDrawGX.hpp"
#include "monolib/core/CViewRectData.hpp"

// Opaque pointer-item layout shared by the frame layout functions: the retail
// ABI hands these a CWorkThread* whose first 4 bytes are a CPnt16 position
// and whose +0x10 word is a type tag (the work-type gate).
struct CFrameWorkPos {
    ml::CPnt16 mPos; // 0x0
    u8 pad04[0xC]; // 0x4
    s32 mType; // 0x10 (compared signed in retail: cmpwi)
};

extern "C" {
// CView method imports (retail mangled names; definitions live in CView.o).
void func_8043E58C__5CViewFRQ22ml5CRectP5CView(ml::CRect* rect, CView* view);
void func_8043E6AC__5CViewFRQ22ml5CRectP5CView(ml::CRect* rect, CView* view);
void func_80442B54__9CViewRootFPvPv(void* a, void* b, void* c);
int func_8043CAFC__5CViewFv(CView* view);
int func_8043CE90__5CViewFv(CView* view);
s16 getSplitLine__5CViewFv(CView* view);
void setSplitLine__5CViewFs(CView* view, s16 line);
bool hasCurrent__5CViewCFv(const CView* view);
CView* func_8043DF3C__5CViewFv(CView* view);
void func_8043E46C__5CViewFRQ22ml5CRectP5CView(ml::CRect* rect, CView* view);
void func_8043CCCC__5CViewFv(CView* view, ml::CRect16* rect);
CView* func_8043CEAC__5CViewFv(CView* view);
void func_8043E7CC__5CViewFRQ22ml5CRectP5CView(ml::CRect* rect, CView* view);
void func_80442C68__9CViewRootFv();
// CFontLayer method imports (retail Fv names; the args flow through the
// tail-call stubs into CDeviceFont).
void func_80449078__10CFontLayerFv(CFontLayer* layer, int r4, int r5, void* r6, ...);
int func_80449148__10CFontLayerFv(CFontLayer* layer);
void func_8044914C__10CFontLayerFv(CFontLayer* layer, void* r4);
void func_80449150__10CFontLayerFv(CFontLayer* layer, float f1, float f2);
void func_80449154__10CFontLayerFv(CFontLayer* layer, u32 r4);
void func_80449158__10CFontLayerFv(CFontLayer* layer, u32 r4);
void fontFlush__10CFontLayerFi(CFontLayer* layer, int r4);

// Retail sdata2 constants used by the frame drawing (imports; the definitions
// live in the monolib shared data blob, not this TU).
float lbl_eu_8066A308;
float lbl_eu_8066A30C;
float lbl_eu_8066A310;
float lbl_eu_8066A314;
float lbl_eu_8066A318;
float lbl_eu_8066A2EC;
float lbl_eu_8066A2F0;
float lbl_eu_8066A2F4;
double lbl_eu_8066A300;

// Definitions in this TU (retail symbol names; declared here so earlier
// callers in the file can reference them).
int func_80441310__10CViewFrameFP11CWorkThread(CViewFrame* self, CWorkThread* pThread);
void func_804406D8__10CViewFrameFPv(CViewFrame* self, void* draw);
void func_80440D78__10CViewFrameFPvPv(CViewFrame* self, void* draw, void* pos);
void func_804409D0__10CViewFrameFPvPv(CViewFrame* self, void* draw, void* rect);
int CView_UnkVirtualFunc9__10CViewFrameFv(CViewFrame* self, CWorkThread* pThread);
int CView_UnkVirtualFunc1__10CViewFrameFv(CViewFrame* self, CWorkThread* pThread);
int func_80441290__10CViewFrameFi(CViewFrame* self, int r4);
}

// Retail sdata2 float constants + vtable used by the ctor (imports; the
// definitions live in the monolib shared data blob, not this TU).
extern float lbl_eu_8066A2E8; // 0.0
extern float lbl_eu_8066A2F8; // 0.4
extern float lbl_eu_8066A2FC; // 0.6
extern char lbl_eu_8056B700[]; // CViewFrame vtable (0x10-byte data blob)

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
    {
        CView* owner = mOwner;
        CView* view = owner->mFrame.mOwner;
        u32 flags = view->unk27C;
        ml::CRect16* r = (ml::CRect16*)&rect;
        // Direct volatile stores keep the 0x230/232/1c8/1ca read order; the
        // expand VR is created between the py and sx reads so the reverse-color
        // order lands expand at r4 (retail sx=r3 expand=r4 py=r5 px=r6).
        r->mPos.x = view->mFrame.mContentX;
        r->mPos.y = view->mFrame.mContentY;
        int expand = 0;
        r->mSize.x = view->mRectData.mViewSize.x;
        r->mSize.y = view->mRectData.mViewSize.y;

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
        // Fresh owner local after bl; declaration order matches retail coloring
        // (own=r3, flagExpand=r4, flagInner=r5, vw=r6, flags=r7, width=r31, height=r30).
        CView* own = mOwner;
        int flagExpand = 0;
        int flagInner = 0;
        CView* vw = own->mFrame.mOwner;
        u32 flags = vw->unk27C;
        s16 width = vw->mRectData.mViewSize.x;
        s16 height = vw->mRectData.mViewSize.y;

        if ((flags & 1) != 0) {
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
            s16 border = own->mFrame.mBorder;
            flagExpand = 0;
            // No outer (s16) on the sum: retail does add r31,r31,r0 without final extsh.
            width += (s16)(border * 2);

            if ((flags & 2) != 0) {
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
extern "C" void func_804409D0__10CViewFrameFPvPv(CViewFrame* self, void* draw, void* rect) {
    CDrawGX* d = (CDrawGX*)draw;
    ml::CRect16* r = (ml::CRect16*)rect;
    ml::CCol4 col;
    ml::CRect16 piece;
    s16 border = self->mBorder;
    float scale = lbl_eu_8066A318;
    float opacity = self->mOwner->mAlpha;

    col.r = self->mFrameColor.r * scale;
    col.g = self->mFrameColor.g * scale;
    col.b = self->mFrameColor.b * scale;
    col.a = self->mFrameColor.a * opacity;
    d->setCol(col);

    d->begin(9, 1);
    piece.mPos.x = r->mPos.x;
    piece.mPos.y = r->mPos.y;
    piece.mSize.x = border;
    piece.mSize.y = r->mSize.y;
    d->add(piece);
    d->end();

    d->begin(9, 1);
    piece.mPos.x = r->mPos.x;
    piece.mPos.y = (s16)((s16)(r->mPos.y + r->mSize.y) - border);
    piece.mSize.x = r->mSize.x;
    piece.mSize.y = border;
    d->add(piece);
    d->end();

    d->begin(9, 1);
    piece.mPos.x = (s16)((s16)(r->mPos.x + r->mSize.x) - border);
    piece.mPos.y = r->mPos.y;
    piece.mSize.x = border;
    piece.mSize.y = r->mSize.y;
    d->add(piece);
    d->end();

    d->begin(9, 1);
    piece.mPos.x = r->mPos.x;
    piece.mPos.y = r->mPos.y;
    piece.mSize.x = r->mSize.x;
    piece.mSize.y = border;
    d->add(piece);
    d->end();

    scale = lbl_eu_8066A2F4;
    col.r = self->mFrameColor.r * scale;
    col.g = self->mFrameColor.g * scale;
    col.b = self->mFrameColor.b * scale;
    col.a = self->mFrameColor.a * opacity;
    d->setCol(col);

    d->begin(9, 1);
    piece.mPos.x = (s16)((s16)(border + r->mPos.x) - 1);
    piece.mPos.y = (s16)(r->mPos.y + border);
    piece.mSize.x = 1;
    piece.mSize.y = (s16)(r->mSize.y - (s16)(border * 2));
    d->add(piece);
    d->end();

    d->begin(9, 1);
    piece.mPos.x = (s16)(r->mPos.x + border);
    piece.mPos.y = (s16)((s16)(border + r->mPos.y) - 1);
    piece.mSize.x = (s16)(r->mSize.x - (s16)(border * 2));
    piece.mSize.y = 1;
    d->add(piece);
    d->end();

    d->begin(9, 1);
    piece.mPos.x = (s16)(r->mPos.x - 1);
    piece.mPos.y = (s16)((s16)(r->mPos.y + r->mSize.y) - 1);
    piece.mSize.x = r->mSize.x;
    piece.mSize.y = 1;
    d->add(piece);
    d->end();

    d->begin(9, 1);
    piece.mPos.x = (s16)((s16)(r->mPos.x + r->mSize.x) - 1);
    piece.mPos.y = r->mPos.y;
    piece.mSize.x = 1;
    piece.mSize.y = r->mSize.y;
    d->add(piece);
    d->end();
}

// Draw the split-frame border: two horizontal split-line quads (a darker
// line at y=mBorder and a lighter one at y=mBorder+0x16, spanning the
// expanded width) using mColor28, then refresh the owner's font layer and
// hand the rect to func_804409D0 for the outer border.
extern "C" void func_804406D8__10CViewFrameFPv(CViewFrame* self, void* draw) {
    CDrawGX* d = (CDrawGX*)draw;
    CView* owner = self->mOwner;
    CView* view = owner->mFrame.mOwner;
    s16 width = view->mRectData.mViewSize.x;
    int inner = 0;
    if ((view->unk27C & 1) != 0) {
        u32 mode = view->unk278;
        if ((mode & 1) == 0) {
            inner = 1;
        }
    }
    int expand = 0;
    if (inner != 0) {
        u32 mode = view->unk278;
        if ((mode & 2) == 0) {
            expand = 1;
        }
    }
    if (expand != 0) {
        s16 ownerBorder = owner->mFrame.mBorder;
        width = (s16)(width + (s16)(ownerBorder * 2));
    }

    s16 border = self->mBorder;
    s16 x2 = (s16)(width - (s16)(border * 2));
    s16 xRight = (s16)(border + x2);
    s16 y2 = (s16)(border + 0x16);
    ml::CCol4 col;
    col.r = self->mColor28.r;
    col.g = self->mColor28.g;
    col.b = self->mColor28.b;
    col.a = self->mColor28.a;
    if (hasCurrent__5CViewCFv(owner) != 0) {
        // Grey out the frame colour when the view is not current.
        float lum = lbl_eu_8066A310 * col.g + lbl_eu_8066A30C * col.r +
                    lbl_eu_8066A308 * col.b;
        col.r = lum;
        col.g = lum;
        col.b = lum;
    }
    col.a = self->mOwner->mAlpha;

    d->begin(8, 4);
    {
        ml::CCol4 c = col;
        c.r = c.r * lbl_eu_8066A2EC;
        c.g = c.g * lbl_eu_8066A2EC;
        c.b = c.b * lbl_eu_8066A2EC;
        d->add(border, border, c);
    }
    {
        ml::CCol4 c = col;
        c.r = c.r * lbl_eu_8066A2F0;
        c.g = c.g * lbl_eu_8066A2F0;
        c.b = c.b * lbl_eu_8066A2F0;
        d->add(xRight, border, c);
    }
    {
        ml::CCol4 c = col;
        c.r = c.r * lbl_eu_8066A2EC;
        c.g = c.g * lbl_eu_8066A2EC;
        c.b = c.b * lbl_eu_8066A2EC;
        d->add(border, y2, c);
    }
    {
        ml::CCol4 c = col;
        c.r = c.r * lbl_eu_8066A2F0;
        c.g = c.g * lbl_eu_8066A2F0;
        c.b = c.b * lbl_eu_8066A2F0;
        d->add(xRight, y2, c);
    }
    d->end();

    func_80449154__10CFontLayerFv(self->mOwner, 0);
    func_80449158__10CFontLayerFv(self->mOwner, 0);
    func_8044914C__10CFontLayerFv(self->mOwner, &self->mColor18);
    func_80449150__10CFontLayerFv(self->mOwner, lbl_eu_8066A314, lbl_eu_8066A2F0);
    s16 border2 = self->mBorder;
    int n = func_80449148__10CFontLayerFv(self->mOwner);
    func_80449078__10CFontLayerFv(self->mOwner, border2, border2 + 0xb - (n >> 1),
                                  &self->mOwner->mName);
    fontFlush__10CFontLayerFi(self->mOwner, 1);

    ml::CRect16 rect;
    rect.mPos.x = 0;
    rect.mPos.y = 0;
    rect.mSize.x = width;
    rect.mSize.y = (s16)(border * 2 + 0x16);
    func_804409D0__10CViewFrameFPvPv(self, draw, &rect);
}

// Draw one edge of the frame border (outer rect + 1px highlight line) at the
// pointer position, expanding around the owner's border and adjusting for the
// split line when the owner view is in split mode.
extern "C" void func_80440D78__10CViewFrameFPvPv(CViewFrame* self, void* draw, void* pos) {
    CDrawGX* d = (CDrawGX*)draw;
    ml::CPnt16* p = (ml::CPnt16*)pos;
    CView* owner = self->mOwner;
    CView* view = owner->mFrame.mOwner;
    s16 width = view->mRectData.mViewSize.x;
    s16 height = view->mRectData.mViewSize.y;
    int inner = 0;
    if ((view->unk27C & 1) != 0 && (view->unk278 & 1) == 0) {
        inner = 1;
    }
    if (inner != 0 && (view->unk278 & 2) == 0) {
        width = (s16)(width + (s16)(owner->mFrame.mBorder * 2));
        int split = 0;
        if ((view->unk27C & 2) != 0 && (view->unk278 & 1) == 0 &&
            (view->unk278 & 2) == 0) {
            split = 1;
        }
        if (split != 0) {
            height = (s16)(height + (s16)(owner->mFrame.mBorder * 3 + 0x16));
        } else {
            height = (s16)(height + (s16)(owner->mFrame.mBorder * 2));
        }
    }

    s16 splitLine = getSplitLine__5CViewFv(owner);
    if (func_8043CE90__5CViewFv(owner) != 0) {
        // Split mode: the border quad sits to the right of the split line.
        s16 border = self->mBorder;
        s16 x = p->x;
        s16 y = (s16)(splitLine + p->y);
        s16 w = width;
        int inner2 = 0;
        if ((owner->unk27C & 1) != 0 && (owner->unk278 & 1) == 0) {
            inner2 = 1;
        }
        if (inner2 != 0 && (owner->unk278 & 2) == 0) {
            x = (s16)(x + border);
            y = (s16)(y + border);
            w = (s16)(w - (s16)(border * 2));
        }
        if ((owner->unk27C & 2) != 0 && (owner->unk278 & 1) == 0 &&
            (owner->unk278 & 2) == 0) {
            y = (s16)(y + (s16)(border + 0x16));
        }

        ml::CCol4 col = self->mFrameColor;
        col.a = col.a * owner->mAlpha;
        ml::CCol4 c;
        float scale = lbl_eu_8066A318;
        c.r = col.r * scale;
        c.g = col.g * scale;
        c.b = col.b * scale;
        c.a = col.a;
        d->setCol(c);

        d->begin(9, 1);
        ml::CRect16 rect;
        rect.mPos.x = x;
        rect.mPos.y = y;
        rect.mSize.x = w;
        rect.mSize.y = border;
        d->add(rect);
        d->end();

        scale = lbl_eu_8066A2F4;
        c.r = col.r * scale;
        c.g = col.g * scale;
        c.b = col.b * scale;
        c.a = col.a;
        d->setCol(c);

        d->begin(9, 1);
        ml::CRect16 line;
        line.mPos.x = x;
        line.mPos.y = (s16)(y + border - 1);
        line.mSize.x = w;
        line.mSize.y = 1;
        d->add(line);
        d->end();
    } else {
        // Normal mode: the border quad runs along the split line.
        s16 border = self->mBorder;
        ml::CRect16 rect;
        rect.mPos.x = (s16)(splitLine + p->x);
        rect.mPos.y = p->y;
        rect.mSize.x = border;
        rect.mSize.y = height;
        int inner2 = 0;
        if ((owner->unk27C & 1) != 0 && (owner->unk278 & 1) == 0) {
            inner2 = 1;
        }
        if (inner2 != 0) {
            rect.mPos.x = (s16)(rect.mPos.x + border);
        }
        if ((owner->unk27C & 2) != 0 && (owner->unk278 & 1) == 0 &&
            (owner->unk278 & 2) == 0) {
            rect.mPos.y = (s16)(rect.mPos.y + (s16)(border + 0x16));
            rect.mSize.y = (s16)(rect.mSize.y - (s16)(border + 0x16));
        }

        ml::CCol4 col = self->mFrameColor;
        col.a = col.a * owner->mAlpha;
        ml::CCol4 c;
        float scale = lbl_eu_8066A318;
        c.r = col.r * scale;
        c.g = col.g * scale;
        c.b = col.b * scale;
        c.a = col.a;
        d->setCol(c);

        d->begin(9, 1);
        d->add(rect);
        d->end();

        scale = lbl_eu_8066A2F4;
        c.r = col.r * scale;
        c.g = col.g * scale;
        c.b = col.b * scale;
        c.a = col.a;
        d->setCol(c);

        d->begin(9, 1);
        ml::CRect16 line;
        line.mPos.x = (s16)(rect.mPos.x + border - 1);
        line.mPos.y = rect.mPos.y;
        line.mSize.x = 1;
        line.mSize.y = rect.mSize.y;
        d->add(line);
        d->end();
    }
}

// CViewFrame default constructor - empty; initialisation is done by the
// placement-new caller or caller-side inline init.
// (Real ctor body is defined below as __ct__CViewFrame.)
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

// Detach the frame from a CWorkThread's render list: when the frame's work
// type is 8, resolve the thread via func_80441310 first, otherwise pass the
// stored type straight to the shared detach helper.
void CViewFrame::detachRenderWork(CWorkThread* pThread) {
    if ((int)unk38 == 8) {
        func_80441290__10CViewFrameFi(
            this, (int)func_80441310__10CViewFrameFP11CWorkThread(this, pThread));
    } else {
        func_80441290__10CViewFrameFi(this, (int)unk38);
    }
}

extern "C" void func_8043FC60__10CViewFrameFUl(CViewFrame* self, u32 val) {
    *(u32*)((u8*)self + 4) = val;
}

// CViewFrame::CView_UnkVirtualFunc1(CWorkThread*) - the retail symbol is Fv
// but the ABI passes the pointer thread in r4. Moves/resizes the owner view's
// rect (rect2) to follow the pointer: computes the rect from the frame layout,
// applies border expansion and per-mode clamping around the stored offsets
// (unk3C..unk4C set by CView_UnkVirtualFunc9), then pushes it via
// func_8043CCCC and, when a parent view exists, writes the split line from
// the pointer position (modes 0xa/0xb). Records the pointer position and
// returns 1.
extern "C" int CView_UnkVirtualFunc1__10CViewFrameFv(CViewFrame* self, CWorkThread* pThread) {
    const CFrameWorkPos* pos = (const CFrameWorkPos*)pThread;
    if ((s32)self->unk38 == 8) {
        return 0;
    }

    CView* owner = self->mOwner;
    CView* view = func_8043DF3C__5CViewFv(owner);
    ml::CRect rect;
    func_8043E46C__5CViewFRQ22ml5CRectP5CView(&rect, view);

    ml::CRect16 rect2;
    CView* frameOwner = owner->mFrame.mOwner;
    rect2.mPos.x = frameOwner->mFrame.mContentX;
    rect2.mPos.y = frameOwner->mFrame.mContentY;
    rect2.mSize.x = frameOwner->mRectData.mViewSize.x;
    rect2.mSize.y = frameOwner->mRectData.mViewSize.y;
    rect.mSize.x = (s16)(pos->mPos.x - rect.mPos.x);
    rect.mSize.y = (s16)(pos->mPos.y - rect.mPos.y);

    {
        int inner = 0;
        if ((frameOwner->unk27C & 1) != 0) {
            u32 mode = frameOwner->unk278;
            if ((mode & 1) == 0) {
                inner = 1;
            }
        }
        if (inner != 0) {
            s16 border = owner->mFrame.mBorder;
            rect2.mSize.x = (s16)(rect2.mSize.x + (s16)(border * 2));
            int split = 0;
            CView* v2 = owner->mFrame.mOwner;
            if ((v2->unk27C & 2) != 0) {
                u32 mode = v2->unk278;
                if ((mode & 1) == 0 && (mode & 2) == 0) {
                    split = 1;
                }
            }
            if (split != 0) {
                s16 b2 = owner->mFrame.mBorder;
                rect2.mSize.y = (s16)(rect2.mSize.y + (s16)(b2 * 3 + 0x16));
            } else {
                s16 b2 = owner->mFrame.mBorder;
                rect2.mSize.y = (s16)(rect2.mSize.y + (s16)(b2 * 2));
            }
        }
    }

    // Frame modes 9: the rect tracks the pointer delta from its last spot.
    if ((owner->unk27C & 0x200) == 0 && (s32)self->unk38 == 9) {
        rect2.mPos.x = (s16)(rect2.mPos.x + (s16)(pos->mPos.x - self->unk50));
        rect2.mPos.y = (s16)(rect2.mPos.y + (s16)(pos->mPos.y - self->unk52));
    }

    int mode = (s32)self->unk38;
    if ((owner->unk27C & 0x100) == 0) {
        if (mode == 4 || mode == 1 || mode == 3) {
            // Top-left anchored resize.
            rect2.mPos.x = (s16)((float)rect.mSize.x - self->unk40);
            rect2.mSize.x = (s16)((float)rect.mSize.x -
                                  ((float)(rect.mSize.x - rect2.mPos.x) - self->unk40));
            if (rect2.mSize.x < 8) {
                rect2.mPos.x = (s16)(rect2.mPos.x - (s16)(8 - rect2.mSize.x));
                rect2.mSize.x = 8;
            }
        }
        if (mode == 6 || (u32)mode <= 1) {
            rect2.mPos.y = (s16)((float)rect.mSize.y - self->unk44);
            rect2.mSize.y = (s16)(rect2.mSize.y -
                                  (s16)((rect.mSize.y - rect2.mPos.y) - (int)self->unk44));
            if (rect2.mSize.y < 8) {
                rect2.mPos.y = (s16)(rect2.mPos.y - (s16)(8 - rect2.mSize.y));
                rect2.mSize.y = 8;
            }
        }
        if (mode == 5 || mode == 0 || mode == 2) {
            rect2.mSize.x = (s16)((float)rect2.mSize.x +
                                  ((float)(rect.mSize.x - (rect2.mPos.x + rect2.mSize.x)) -
                                   self->unk3C));
            if (rect2.mSize.x < 8) {
                rect2.mSize.x = 8;
            }
        }
        if (mode == 7 || (u32)(mode - 2) <= 1) {
            rect2.mSize.y = (s16)((float)rect2.mSize.y +
                                  ((float)(rect.mSize.y - (rect2.mPos.y + rect2.mSize.y)) -
                                   self->unk48));
            if (rect2.mSize.y < 8) {
                rect2.mSize.y = 8;
            }
        }
    }

    func_8043CCCC__5CViewFv(owner, &rect2);
    if (func_8043CAFC__5CViewFv(owner) != 0) {
        ml::CRect parentRect;
        func_8043E6AC__5CViewFRQ22ml5CRectP5CView(&parentRect, owner);
        parentRect.mSize.x = (s16)(pos->mPos.x - parentRect.mPos.x);
        parentRect.mSize.y = (s16)(pos->mPos.y - parentRect.mPos.y);
        rect.mSize = parentRect.mSize;
        if ((s32)self->unk38 == 0xa) {
            setSplitLine__5CViewFs(owner, (s16)((float)rect.mSize.y - self->unk4C));
        }
        if ((s32)self->unk38 == 0xb) {
            setSplitLine__5CViewFs(owner, (s16)((float)rect.mSize.x - self->unk4C));
        }
    }

    *(u32*)&self->unk50 = *(u32*)&pos->mPos;
    return 1;
}

// Set the frame's work type to 8 and report success (retail returns 0).
extern "C" int CView_UnkVirtualFunc8__10CViewFrameFv(CViewFrame* self) {
    self->unk38 = 8;
    return 0;
}

// CViewFrame::CView_UnkVirtualFunc9(CWorkThread*) - the retail symbol is Fv
// but the ABI passes the pointer thread in r4 (the same CWorkThread* that
// func_80441310 takes). Positions the frame's stored offsets (unk3C..unk4C)
// from the thread position relative to the owner's content rect, applies the
// border-expand sizing, then records the thread position (unk50) and returns
// whether the frame's work type (xor 8) halves to a negative value (the
// split-frame indicator used by detachRenderWork).
extern "C" int CView_UnkVirtualFunc9__10CViewFrameFv(CViewFrame* self, CWorkThread* pThread) {
    const CFrameWorkPos* pos = (const CFrameWorkPos*)pThread;
    if (pos->mType != 1) {
        return 0;
    }

    self->unk38 = (u32)func_80441310__10CViewFrameFP11CWorkThread(self, pThread);

    ml::CRect viewRect;
    func_8043E58C__5CViewFRQ22ml5CRectP5CView(&viewRect, self->mOwner);

    ml::CRect16 scratch;
    int dx = pos->mPos.x - viewRect.mPos.x;
    int dy = pos->mPos.y - viewRect.mPos.y;
    self->unk40 = (float)(s16)dx;
    self->unk44 = (float)(s16)dy;
    scratch.mPos.x = (s16)dx;
    scratch.mPos.y = (s16)dy;

    {
        CView* view = self->mOwner->mFrame.mOwner;
        s16 width = view->mRectData.mViewSize.x;
        int inner = 0;
        if ((view->unk27C & 1) != 0) {
            u32 mode = view->unk278;
            if ((mode & 1) == 0) {
                inner = 1;
            }
        }
        int expand = 0;
        if (inner != 0) {
            u32 mode = view->unk278;
            if ((mode & 2) == 0) {
                expand = 1;
            }
        }
        if (expand != 0) {
            s16 border = self->mOwner->mFrame.mBorder;
            width = (s16)(width + (s16)(border * 2));
        }
        self->unk3C = (float)(scratch.mPos.x - width);
    }

    {
        CView* view = self->mOwner->mFrame.mOwner;
        s16 height = view->mRectData.mViewSize.y;
        int inner = 0;
        if ((view->unk27C & 1) != 0) {
            u32 mode = view->unk278;
            if ((mode & 1) == 0) {
                inner = 1;
            }
        }
        int expand = 0;
        if (inner != 0) {
            u32 mode = view->unk278;
            if ((mode & 2) == 0) {
                expand = 1;
            }
        }
        if (expand != 0) {
            s16 border = self->mOwner->mFrame.mBorder;
            int split = 0;
            if ((view->unk27C & 2) != 0) {
                u32 mode = view->unk278;
                if ((mode & 1) == 0 && (mode & 2) == 0) {
                    split = 1;
                }
            }
            if (split != 0) {
                height = (s16)(height + (s16)(border * 3 + 0x16));
            } else {
                height = (s16)(height + (s16)(border * 2));
            }
        }
        self->unk48 = (float)(scratch.mPos.y - height);
    }

    if (func_8043CAFC__5CViewFv(self->mOwner) != 0) {
        ml::CRect parentRect;
        func_8043E6AC__5CViewFRQ22ml5CRectP5CView(&parentRect, self->mOwner);
        parentRect.mSize.x = (s16)(pos->mPos.x - parentRect.mPos.x);
        parentRect.mSize.y = (s16)(pos->mPos.y - parentRect.mPos.y);
        scratch.mPos = parentRect.mSize;
        if (func_8043CE90__5CViewFv(self->mOwner) != 0) {
            self->unk4C = (float)(scratch.mPos.y - getSplitLine__5CViewFv(self->mOwner));
        } else {
            self->unk4C = (float)(scratch.mPos.x - getSplitLine__5CViewFv(self->mOwner));
        }
    }

    *(u32*)&self->unk50 = *(u32*)&pos->mPos;
    int v = (int)self->unk38 ^ 8;
    return (v >> 1) - ((v >> 3) & 1) < 0;
}

// Map a work-thread type to a render-list index (see detachRenderWork).
extern "C" int func_80441290__10CViewFrameFi(CViewFrame* self, int r4) {
    if (r4 == 0 || r4 == 3) {
        return 1;
    }
    if ((u32)(r4 - 1) <= 1u) {
        return 2;
    }
    if (r4 == 4 || r4 == 5 || r4 == 0xb) {
        return 3;
    }
    if (r4 == 6 || r4 == 7 || r4 == 0xa) {
        return 4;
    }
    int v = 0;
    if (r4 == 9) {
        v = 5;
    }
    return v;
}

// Hit-test the pointer position against the frame's resize handles and the
// owner view's split/current sub-rects. Returns the work-type index of the
// hit region (0-7 edge/corner handles, 9 split handle, 0xa/0xb view edges,
// 8 = no hit). The pointer thread is treated as a CPnt16 position.
extern "C" int func_80441310__10CViewFrameFP11CWorkThread(CViewFrame* self, CWorkThread* pThread) {
    const ml::CPnt16* p = (const ml::CPnt16*)pThread;
    ml::CRect16 rectA; // view content rect (func_8043E58C)
    ml::CRect16 rectB; // parent rect (func_8043E6AC)
    func_8043E58C__5CViewFRQ22ml5CRectP5CView((ml::CRect*)&rectA, self->mOwner);
    func_8043E6AC__5CViewFRQ22ml5CRectP5CView((ml::CRect*)&rectB, self->mOwner);

    CView* owner = self->mOwner;
    int inner = 0;
    if ((owner->unk27C & 1) != 0) {
        u32 mode = owner->unk278;
        if ((mode & 1) == 0) {
            inner = 1;
        }
    }
    int expand = 0;
    if (inner != 0) {
        u32 mode = owner->unk278;
        if ((mode & 2) == 0) {
            expand = 1;
        }
    }

    if (expand != 0) {
        // Split handle at the top of the frame (y = mBorder band).
        int f1 = 0;
        if ((owner->unk27C & 0x80) != 0) {
            u32 mode = owner->unk278;
            if ((mode & 1) == 0 && (mode & 2) == 0) {
                f1 = 1;
            }
        }
        if (f1 != 0) {
            ml::CRect16 hit;
            hit.mPos.x = (s16)(rectA.mPos.x + self->mBorder);
            hit.mPos.y = (s16)(rectA.mPos.y + self->mBorder);
            hit.mSize.x = (s16)(rectA.mSize.x - (s16)(self->mBorder * 2));
            hit.mSize.y = 0x16;
            int f2 = 0;
            if ((owner->unk27C & 0x80) != 0) {
                u32 mode = owner->unk278;
                if ((mode & 1) == 0 && (mode & 2) == 0) {
                    f2 = 1;
                }
            }
            if (f2 != 0) {
                hit = rectA;
            }
            if (hit.isInside(*p)) {
                return 9;
            }
        }

        if ((owner->unk27C & 0x100) == 0) {
            s16 xr = (s16)(rectA.mPos.x + rectA.mSize.x);
            s16 y = rectA.mPos.y;
            ml::CRect16 r1;
            r1.mPos.x = (s16)(xr - self->mBorder);
            r1.mPos.y = y;
            r1.mSize.x = self->mBorder;
            r1.mSize.y = 0x10;
            ml::CRect16 r2;
            r2.mPos.x = (s16)(xr - 0x10);
            r2.mPos.y = y;
            r2.mSize.x = 0x10;
            r2.mSize.y = self->mBorder;
            if (r1.isInside(*p) || r2.isInside(*p)) {
                return 0;
            }
        }
        if ((owner->unk27C & 0x100) == 0) {
            s16 x = rectA.mPos.x;
            s16 y = rectA.mPos.y;
            ml::CRect16 r1;
            r1.mPos.x = x;
            r1.mPos.y = y;
            r1.mSize.x = self->mBorder;
            r1.mSize.y = 0x10;
            ml::CRect16 r2;
            r2.mPos.x = x;
            r2.mPos.y = y;
            r2.mSize.x = 0x10;
            r2.mSize.y = self->mBorder;
            if (r1.isInside(*p) || r2.isInside(*p)) {
                return 1;
            }
        }
        if ((owner->unk27C & 0x100) == 0) {
            s16 xr = (s16)(rectA.mPos.x + rectA.mSize.x);
            s16 yr = (s16)(rectA.mPos.y + rectA.mSize.y);
            ml::CRect16 r1;
            r1.mPos.x = (s16)(xr - self->mBorder);
            r1.mPos.y = (s16)(yr - 0x10);
            r1.mSize.x = self->mBorder;
            r1.mSize.y = 0x10;
            ml::CRect16 r2;
            r2.mPos.x = (s16)(xr - 0x10);
            r2.mPos.y = (s16)(yr - self->mBorder);
            r2.mSize.x = 0x10;
            r2.mSize.y = self->mBorder;
            if (r1.isInside(*p) || r2.isInside(*p)) {
                return 2;
            }
        }
        if ((owner->unk27C & 0x100) == 0) {
            s16 yr = (s16)(rectA.mPos.y + rectA.mSize.y);
            ml::CRect16 r1;
            r1.mPos.x = rectA.mPos.x;
            r1.mPos.y = (s16)(yr - 0x10);
            r1.mSize.x = self->mBorder;
            r1.mSize.y = 0x10;
            ml::CRect16 r2;
            r2.mPos.x = rectA.mPos.x;
            r2.mPos.y = (s16)(yr - self->mBorder);
            r2.mSize.x = 0x10;
            r2.mSize.y = self->mBorder;
            if (r1.isInside(*p) || r2.isInside(*p)) {
                return 3;
            }
        }
        if ((owner->unk27C & 0x100) == 0) {
            ml::CRect16 r;
            r.mPos.x = rectA.mPos.x;
            r.mPos.y = (s16)(rectA.mPos.y + 0x10);
            r.mSize.x = self->mBorder;
            r.mSize.y = (s16)(rectA.mSize.y - 0x20);
            if (r.isInside(*p)) {
                return 4;
            }
        }
        if ((owner->unk27C & 0x100) == 0) {
            s16 xr = (s16)(rectA.mPos.x + rectA.mSize.x);
            ml::CRect16 r;
            r.mPos.x = (s16)(xr - self->mBorder);
            r.mPos.y = (s16)(rectA.mPos.y + 0x10);
            r.mSize.x = self->mBorder;
            r.mSize.y = (s16)(rectA.mSize.y - 0x20);
            if (r.isInside(*p)) {
                return 5;
            }
        }
        if ((owner->unk27C & 0x100) == 0) {
            ml::CRect16 r;
            r.mPos.x = (s16)(rectA.mPos.x + 0x10);
            r.mPos.y = rectA.mPos.y;
            r.mSize.x = (s16)(rectA.mSize.x - 0x20);
            r.mSize.y = self->mBorder;
            if (r.isInside(*p)) {
                return 6;
            }
        }
        if ((owner->unk27C & 0x100) == 0) {
            s16 yr = (s16)(rectA.mPos.y + rectA.mSize.y);
            ml::CRect16 r;
            r.mPos.x = (s16)(rectA.mPos.x + 0x10);
            r.mPos.y = (s16)(yr - self->mBorder);
            r.mSize.x = (s16)(rectA.mSize.x - 0x20);
            r.mSize.y = self->mBorder;
            if (r.isInside(*p)) {
                return 7;
            }
        }
        if ((owner->unk27C & 0x100) == 0) {
            // Split handle band again (full content width).
            ml::CRect16 hit;
            hit.mPos.x = (s16)(rectA.mPos.x + self->mBorder);
            hit.mPos.y = (s16)(rectA.mPos.y + self->mBorder);
            hit.mSize.x = (s16)(rectA.mSize.x - (s16)(self->mBorder * 2));
            hit.mSize.y = 0x16;
            int f2 = 0;
            if ((owner->unk27C & 0x80) != 0) {
                u32 mode = owner->unk278;
                if ((mode & 1) == 0 && (mode & 2) == 0) {
                    f2 = 1;
                }
            }
            if (f2 != 0) {
                hit = rectA;
            }
            if (hit.isInside(*p)) {
                return 9;
            }
        }
    } else {
        // Non-expanded: single split-handle band at the top.
        int f1 = 0;
        if ((owner->unk27C & 0x80) != 0) {
            u32 mode = owner->unk278;
            if ((mode & 1) == 0 && (mode & 2) == 0) {
                f1 = 1;
            }
        }
        if (f1 != 0) {
            ml::CRect16 hit;
            hit.mPos.x = (s16)(rectA.mPos.x + self->mBorder);
            hit.mPos.y = (s16)(rectA.mPos.y + self->mBorder);
            hit.mSize.x = (s16)(rectA.mSize.x - (s16)(self->mBorder * 2));
            hit.mSize.y = 0x16;
            int f2 = 0;
            if ((owner->unk27C & 0x80) != 0) {
                u32 mode = owner->unk278;
                if ((mode & 1) == 0 && (mode & 2) == 0) {
                    f2 = 1;
                }
            }
            if (f2 != 0) {
                hit = rectA;
            }
            if (hit.isInside(*p)) {
                return 9;
            }
        }
    }

    // Bottom edge under the current view / split view.
    {
        int hit = 0;
        if (func_8043CAFC__5CViewFv(owner) != 0 &&
            func_8043CE90__5CViewFv(owner) != 0 &&
            (owner->unk278 & 0x20) == 0) {
            CView* cur = func_8043CEAC__5CViewFv(owner);
            ml::CRect16 r;
            func_8043E7CC__5CViewFRQ22ml5CRectP5CView((ml::CRect*)&r, cur);
            r.mSize.y = self->mBorder;
            r.mPos.y = (s16)(r.mPos.y - self->mBorder);
            hit = r.isInside(*p);
        }
        if (hit != 0) {
            return 0xa;
        }
    }
    {
        int hit = 0;
        if (func_8043CAFC__5CViewFv(owner) != 0 &&
            func_8043CE90__5CViewFv(owner) == 0 &&
            (owner->unk278 & 0x20) == 0) {
            CView* cur = func_8043CEAC__5CViewFv(owner);
            ml::CRect16 r;
            func_8043E7CC__5CViewFRQ22ml5CRectP5CView((ml::CRect*)&r, cur);
            r.mSize.y = self->mBorder;
            r.mPos.y = (s16)(r.mPos.y - self->mBorder);
            hit = r.isInside(*p);
        }
        if (hit != 0) {
            return 0xb;
        }
    }
    return 8;
}

// The retail binary declares the CViewFrame constructor as a plain global
// function (symbol __ct__CViewFrame, no C++ mangling), so it is reconstructed
// as one. Initialises the whole 0x5C-byte layout; the float constants are
// imports from the shared sdata2 pool (declared above).
extern "C" void __ct__CViewFrame(CViewFrame* self) {
    // Preload the sdata2 float constants so MWCC hoists the lfs loads to the
    // top (retail loads f5..f0 in first-use order); declaration order drives
    // the register assignment (first declared → f5).
    float fZero = lbl_eu_8066A2E8; // 0.0
    float fHalf = lbl_eu_8066A2EC; // 0.5
    float fOne = lbl_eu_8066A2F0; // 1.0
    float f08 = lbl_eu_8066A2F4; // 0.8
    float f04 = lbl_eu_8066A2F8; // 0.4
    float f06 = lbl_eu_8066A2FC; // 0.6

    self->mVtable = (void*)&lbl_eu_8056B700;
    self->mOwner = 0;
    self->unk38 = 8;
    self->unk3C = fZero;
    self->unk40 = fZero;
    self->unk44 = fZero;
    self->unk48 = fZero;
    self->unk4C = fZero;
    self->mBorder = 4;
    self->mFrameColor.r = fHalf;
    self->mFrameColor.g = fHalf;
    self->mFrameColor.b = fHalf;
    self->mFrameColor.a = fOne;
    self->mColor18.r = f08;
    self->mColor18.g = f08;
    self->mColor18.b = f08;
    self->mColor18.a = fOne;
    self->mColor28.r = f04;
    self->mColor28.g = fHalf;
    self->mColor28.b = f06;
    self->mColor28.a = fOne;
    self->unk50 = 0;
    self->unk52 = 0;
    self->mContentX = 0;
    self->mContentY = 0;
}

// Compute the frame-to-view offset rect. Retail quirk: the caller passes a
// CRect16 as the object pointer, a CViewFrame as 'frame', and the source rect
// as 'src' (the member name is kept for symbol compatibility). Applies the
// same border-expand/split gates as getFrame2ViewOffset.
extern "C" void func_80441EF0__10CViewFrameFR7CRect16PC10CViewFrame(
    ml::CRect16* out, const CViewFrame* frame, const ml::CRect16* src) {
    out->mPos.x = src->mPos.x;
    out->mPos.y = src->mPos.y;
    out->mSize.x = src->mSize.x;
    out->mSize.y = src->mSize.y;

    CView* owner = frame->mOwner;
    s16 xAdj = 0;
    s16 yAdj = 0;
    int flag1 = 0;
    int flag2 = 0;

    if ((owner->unk27C & 1) != 0 && (owner->unk278 & 1) == 0) {
        flag2 = 1;
    }
    if (flag2 != 0 && (owner->unk278 & 2) == 0) {
        flag1 = 1;
    }
    if (flag1 != 0) {
        s16 border = frame->mBorder;
        xAdj = border;
        yAdj = border;
        int split = 0;
        if ((owner->unk27C & 2) != 0 && (owner->unk278 & 1) == 0 &&
            (owner->unk278 & 2) == 0) {
            split = 1;
        }
        if (split != 0) {
            yAdj = (s16)(border + (s16)(border + 0x16));
        }
    }
    out->mPos.x = (s16)(out->mPos.x + xAdj);
    out->mPos.y = (s16)(out->mPos.y + yAdj);

    owner = frame->mOwner;
    flag2 = 0;
    flag1 = 0;
    if ((owner->unk27C & 1) != 0 && (owner->unk278 & 1) == 0) {
        flag2 = 1;
    }
    if (flag2 != 0 && (owner->unk278 & 2) == 0) {
        flag1 = 1;
    }
    s16 sxAdj = 0;
    s16 syAdj = 0;
    if (flag1 != 0) {
        s16 border = frame->mBorder;
        sxAdj = (s16)(border * 2);
        syAdj = sxAdj;
        int split = 0;
        if ((owner->unk27C & 2) != 0 && (owner->unk278 & 1) == 0 &&
            (owner->unk278 & 2) == 0) {
            split = 1;
        }
        if (split != 0) {
            syAdj = (s16)(sxAdj + (s16)(border + 0x16));
        }
    }
    out->mSize.x = (s16)(out->mSize.x + (s16)(-sxAdj));
    out->mSize.y = (s16)(out->mSize.y + (s16)(-syAdj));
}
