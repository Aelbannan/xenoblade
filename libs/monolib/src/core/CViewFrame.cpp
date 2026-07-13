#include "monolib/core/CViewFrame.hpp"
#include "monolib/core/CView.hpp"
#include "monolib/core/CDrawGX.hpp"

extern "C" {
void func_8043E58C__5CViewFRQ22ml5CRectP5CView(ml::CRect* rect, CView* view);
void func_80442B54__9CViewRootFPvPv(void* a, void* b, void* c);
int func_8043CAFC__5CViewFv(CView* view);
void func_80442C68__9CViewRootFv();

// Undefined here so MWCC cannot DCE same-TU empty stubs.
void func_804406D8__10CViewFrameFPv(CViewFrame* self, void* draw);
void func_804409D0__10CViewFrameFPvPv(CViewFrame* self, void* draw, void* rect);
void func_80440D78__10CViewFrameFPvPv(CViewFrame* self, void* draw, void* pos);
}

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
        CView* view = owner->unk1DC.mOwner;
        u32 flags = view->unk27C;
        // Volatile loads pin 0x230/232/1c8/1ca order; expand already live → retail r6/r5/r3/r0.
        s16 px = *(volatile s16*)&view->unk1DC.unk54;
        s16 py = *(volatile s16*)&view->unk1DC.unk56;
        s16 sx = *(volatile s16*)&view->unk1C8.unk0;
        s16 sy = *(volatile s16*)&view->unk1C8.unk2;
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
            view = *(CView* volatile*)&owner->unk1DC.mOwner;
            expand = 0;

            {
                s16 border = owner->unk1DC.unk58;
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
                s16 b1 = owner->unk1DC.unk58;
                rect.mSize.y =
                    (s16)(rect.mSize.y + (s16)(b1 * 3 + 0x16));
            } else {
                s16 b2 = owner->unk1DC.unk58;
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
        CView* vw = own->unk1DC.mOwner;
        width = vw->unk1C8.unk0;
        height = vw->unk1C8.unk2;

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
            border = own->unk1DC.unk58;
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
                float opacity = *(float*)((u8*)mOwner + 0x458);
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
                    s16 adj = (s16)(unk58 + 0x16);
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
