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
    s16 maxWidth;
    s16 maxHeight;
    int partialH;
    int widthRemain;
    int heightRemain;
    int overW;
    int overH;

    *(ml::CPnt16*)&unk0 = size;

    maxWidth = (s16)(unk0 - unkC - unk10);
    // Keep remains/partial as int so cmpw / subf skip extra extsh (retail).
    widthRemain = unk4 - maxWidth;
    partialH = unk2 - unkE;

    // Retail: cmpw unk8,widthRemain then maxHeight=partialH-unk12 then ble.
    if ((overW = (unk8 > widthRemain), maxHeight = (s16)(partialH - unk12), overW)) {
        unk8 = widthRemain;
    }

    heightRemain = unk6 - maxHeight;
    if ((overH = (unkA > heightRemain), overH)) {
        unkA = heightRemain;
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
