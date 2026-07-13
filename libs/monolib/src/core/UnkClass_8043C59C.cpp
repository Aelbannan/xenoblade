#include "monolib/core/CViewRectData.hpp"
#include "monolib/device.hpp"

void CViewRectData::func_80459270() {
    GXRenderModeObj* mode = CDeviceVI::getRenderModeObj();
    unk0 = mode->fbWidth;
    unk2 = mode->efbHeight;
    unk4 = mode->fbWidth;
    unk6 = mode->efbHeight;
    unk8 = 0;
    unkA = 0;
    unkC = 0;
    unkE = 0;
    unk10 = 0;
    unk12 = 0;
}

void CViewRectData::func_804592F0(const ml::CRect16& rect) {
    *(u32*)&unk0 = *(const u32*)&rect.mPos;

    s16 v0 = (s16)((unk0 - unkC) - unk10);
    s16 limitW = (s16)(unk4 - v0);
    if (unk8 > limitW) {
        unk8 = limitW;
    }

    s16 v1 = (s16)((unk2 - unkE) - unk12);
    s16 limitH = (s16)(v1 - unk6);
    if (unkA > limitH) {
        unkA = limitH;
    }

    if (unk8 < 0) {
        unk8 = 0;
    }

    if (unkA < 0) {
        unkA = 0;
    }
}

void CViewRectData::func_80459384(const ml::CRect16& rect) {
    *(u32*)&unk4 = *(const u32*)&rect.mSize;
}
