#pragma once

#include <types.h>
#include "monolib/math.hpp"

// CViewRectDataCore: viewport rectangle state at CView::mRectData (size 0x14).
class CViewRectDataCore {
public:
    CViewRectDataCore* initViewRect();
    void updateScrollForSize(const ml::CPnt16& size);
    void setBoundsSize(const ml::CPnt16& maxSize);

    // Viewport rect data: first two pairs are CPnt16 structs for lwz/stw copies.
    ml::CPnt16 mViewSize;      // offset 0x00 - current viewport size (x=width, y=height)
    ml::CPnt16 mBoundsSize;    // offset 0x04 - maximum bounding size
    s16 mScrollX;              // offset 0x08 - horizontal scroll/offset
    s16 mScrollY;              // offset 0x0A - vertical scroll/offset
    s16 mInsetLeft;            // offset 0x0C - left inset
    s16 mInsetTop;             // offset 0x0E - top inset
    s16 mInsetRight;           // offset 0x10 - right inset
    s16 mInsetBottom;          // offset 0x12 - bottom inset
};
