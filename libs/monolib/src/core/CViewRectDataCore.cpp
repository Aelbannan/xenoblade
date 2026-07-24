#include "monolib/core/CViewRectData.hpp"
#include "monolib/device.hpp"

extern "C" GXRenderModeObj* getRenderModeObj__9CDeviceVIFv();

// Initialises view rect from the current framebuffer display mode.
// Copies the framebuffer width/height into both mViewSize and mBoundsSize,
// then zeroes all scroll/inset fields. Calls getRenderModeObj on each access
// to match retail register allocation rather than caching the pointer.
// @return Pointer to this CViewRectDataCore instance for chaining.
CViewRectDataCore* CViewRectDataCore::func_80459270() {
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
void CViewRectDataCore::func_804592F0(const ml::CPnt16& size) {
    s16 maxWidth;
    s16 maxHeight;
    int partialH;
    int widthRemain;
    int heightRemain;
    int overW;
    int overH;

    mViewSize = size;

    // Match retail register allocation:
    // maxWidth in r5 (via extsh), widthRemain in r8, partialH in r4
    maxWidth = (s16)(mViewSize.x - mInsetLeft - mInsetRight);

    // Compute widthRemain first so it stays in r8 (mBoundsSize.x in r4, maxWidth in r5)
    widthRemain = mBoundsSize.x - maxWidth;

    // Then partialH in r4 (mViewSize.y in r6, mInsetTop in r7)
    partialH = mViewSize.y - mInsetTop;

    // Retail: cmpw mScrollX,widthRemain then maxHeight=partialH-mInsetBottom then ble.
    if ((overW = (mScrollX > widthRemain), maxHeight = (s16)(partialH - mInsetBottom), overW)) {
        mScrollX = widthRemain;
    }

    heightRemain = mBoundsSize.y - maxHeight;
    if ((overH = (mScrollY > heightRemain), overH)) {
        mScrollY = heightRemain;
    }

    if (mScrollX < 0) {
        mScrollX = 0;
    }

    if (mScrollY >= 0) {
        return;
    }
    mScrollY = 0;
}

void CViewRectDataCore::func_80459384(const ml::CPnt16& maxSize) {
    mBoundsSize = maxSize;
}
