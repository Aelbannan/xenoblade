#include "monolib/core/CViewRectData.hpp"
#include "monolib/device.hpp"

extern "C" GXRenderModeObj* getRenderModeObj__9CDeviceVIFv();

CViewRectDataCore* CViewRectDataCore::func_80459270() {
    s16 efbHeight;
    s16 zero = 0;

    efbHeight = getRenderModeObj__9CDeviceVIFv()->efbHeight;
    unk0 = getRenderModeObj__9CDeviceVIFv()->fbWidth;
    unk2 = efbHeight;
    efbHeight = getRenderModeObj__9CDeviceVIFv()->efbHeight;
    unk4 = getRenderModeObj__9CDeviceVIFv()->fbWidth;
    unk6 = efbHeight;
    unk8 = zero;
    unkA = zero;
    unkC = zero;
    unkE = zero;
    unk10 = zero;
    unk12 = zero;
    return this;
}

void CViewRectDataCore::func_804592F0(const ml::CPnt16& size) {
    BOOL overW;
    BOOL overH;
    s16 maxHeight;
    s16 heightLimit;
    s16 partialH;
    s16 maxWidth;
    s16 widthLimit;

    *(ml::CPnt16*)&unk0 = size;
    maxWidth = (s16)(unk0 - unkC - unk10);
    widthLimit = (s16)(unk4 - maxWidth);
    partialH = (s16)(unk2 - unkE);
    if ((overW = (s32)unk8 > (s32)unk4 - (s32)maxWidth, maxHeight = (s16)(partialH - unk12), overW)) {
        unk8 = widthLimit;
    }
    heightLimit = (s16)(unk6 - maxHeight);
    if ((overH = (s32)unkA > (s32)unk6 - (s32)maxHeight, overH)) {
        unkA = heightLimit;
    }
    if (unk8 < 0) {
        unk8 = 0;
    }
    if (unkA >= 0) {
        return;
    }
    unkA = 0;
}

void CViewRectDataCore::func_80459384(const ml::CPnt16& maxSize) {
    *(ml::CPnt16*)&unk4 = maxSize;
}
