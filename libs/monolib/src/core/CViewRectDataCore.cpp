#include "monolib/core/CViewRectData.hpp"
#include "monolib/core/CViewRectDataCore.hpp"
#include "monolib/device.hpp"

// Initialises view rect from the current framebuffer display mode.
// Copies the framebuffer width/height into both mViewSize and mBoundsSize,
// then zeroes all scroll/inset fields. Calls getRenderModeObj on each access
// to match retail register allocation rather than caching the pointer.
// @return Pointer to this CViewRectDataCore instance for chaining.
CViewRectDataCore* CViewRectDataCore::initViewRect() {
    s16 efbHeight;
    s16 zero = 0;

    efbHeight = getRenderModeObj__9CDeviceVIFv()->efbHeight;
    mViewSize.x = getRenderModeObj__9CDeviceVIFv()->fbWidth;
    mViewSize.y = efbHeight;
    efbHeight = getRenderModeObj__9CDeviceVIFv()->efbHeight;
    mBoundsSize.x = getRenderModeObj__9CDeviceVIFv()->fbWidth;
    mBoundsSize.y = efbHeight;
    mScrollX = zero;
    mScrollY = zero;
    mInsetLeft = zero;
    mInsetTop = zero;
    mInsetRight = zero;
    mInsetBottom = zero;
    return this;
}

// Recalculates scroll offsets after the viewport |size| changes.
// Clamps mScrollX and mScrollY so the visible area does not extend beyond
// mBoundsSize after subtracting the inset margins on each axis.
// @param size New viewport dimensions (x=width, y=height).
// Recalculates scroll offsets after the viewport |size| changes.
// Clamps mScrollX and mScrollY so the visible area does not extend beyond
// mBoundsSize after subtracting the inset margins on each axis.
//
// KNOWN GAP: retail holds widthRemain in r8 (fresh register) while MWCC
// reuses r7 here, and the y-basis loads (mInsetTop/y) land in lower regs; a
// documented Chaitin allocation wall (see docs/evidence/decomp attempts.jsonl
// :173) that resists every high-level ordering. Structural match is exact;
// only register assignment (r8 vs r7, r4/r7/r6 vs r6/r5/r4) differs.
// @param size New viewport dimensions (x=width, y=height).
// Recalculates scroll offsets after the viewport |size| changes.
// Clamps mScrollX and mScrollY so the visible area does not extend beyond
// mBoundsSize after subtracting the inset margins on each axis.
//
// KNOWN GAP: retail holds widthRemain in r8 (fresh register) while MWCC
// reuses r7 here, and the y-basis loads (mInsetTop/y) land in lower regs; a
// documented Chaitin allocation wall (see docs/evidence/decomp attempts.jsonl
// :173) that resists every high-level ordering. Structural match is exact;
// only register assignment (r8 vs r7, r4/r7/r6 vs r6/r5/r4) differs.
// @param size New viewport dimensions (x=width, y=height).
// Recalculates scroll offsets after the viewport |size| changes.
// Clamps mScrollX and mScrollY so the visible area does not extend beyond
// mBoundsSize after subtracting the inset margins on each axis.
//
// KNOWN GAP: retail holds widthRemain in r8 (fresh register) while MWCC
// reuses r7 here, and the y-basis loads (mInsetTop/y) land in lower regs; a
// documented Chaitin allocation wall (docs/evidence/decomp attempts.jsonl
// :173) that resists every high-level ordering. Structural match is exact;
// only register assignment (r8 vs r7, r4/r7/r6 vs r6/r5/r4) differs.
// @param size New viewport dimensions (x=width, y=height).
void CViewRectDataCore::updateScrollForSize(const ml::CPnt16& size) {
    int maxWidth;
    int maxHeight;
    int widthRemain;
    int heightRemain;

    mViewSize = size;
    maxHeight = (s16)(mViewSize.y - mInsetTop - mInsetBottom);
    maxWidth = (s16)(mViewSize.x - mInsetLeft - mInsetRight);
    widthRemain = mBoundsSize.x - maxWidth;
    if (mScrollX > widthRemain) {
        mScrollX = widthRemain;
    }
    heightRemain = mBoundsSize.y - maxHeight;
    if (mScrollY > heightRemain) {
        mScrollY = heightRemain;
    }
    if (mScrollX < 0) {
        mScrollX = 0;
    }
    if (mScrollY < 0) {
        mScrollY = 0;
    }
}

void CViewRectDataCore::setBoundsSize(const ml::CPnt16& maxSize) {
    mBoundsSize = maxSize;
}
