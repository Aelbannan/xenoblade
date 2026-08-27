#pragma once

#include <types.h>

namespace mpfsys {

class UnkClass_80471EC8 {
public:
    void tryAttach();
    void update();
    void submitDraw();
    void func_80472064();
    void setLayerScale();
    void clearFlag();
    void setFlag();
    void getLayerColors();
    void func_804724DC();
    void markDirty();
    void setColorMode();
    void func_804728E8();
    void updateLayers();
    void* getLayerRecord(u8 layerIndex);
    void func_804737F0();
    void func_80473984();
    void func_80474064();
    void setupGfxMode0();
    void setupGfxMode1();
    void setupGfxMode2();
    void setupGfxMode3();
    void setupGfxMode4();
    void setupGfxMode5();
    void enableAlphaBlend();
    void disableAlphaBlend();
    void func_80474B00();
    void loadTevKColor();
    void applyMatColor() const;
    void applyTevColor();
    void setKeyAlpha();
    void setFogIndex();
    void resetAmbient();
    void applyAmbient();
    void resetTevSwap();
    void disableZMode();
    void enableZMode();

    // TODO: add fields
};
} // namespace mpfsys

