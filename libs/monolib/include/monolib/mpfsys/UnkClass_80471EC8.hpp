#pragma once

#include <types.h>

namespace mpfsys {

class UnkClass_80471EC8 {
public:
    void tryAttach();
    void update();
    void submitDraw();
    void buildDisplayList();
    void setLayerScale();
    void clearFlag();
    void setFlag();
    void getLayerColors();
    void initLayerRecords();
    void markDirty();
    void setColorMode();
    void simulateLayer();
    void updateLayers();
    void* getLayerRecord(u8 layerIndex);
    void setupLayerDraw();
    void drawMapLayer();
    void pushLayerDraw();
    void setupGfxMode0();
    void setupGfxMode1();
    void setupGfxMode2();
    void setupGfxMode3();
    void setupGfxMode4();
    void setupGfxMode5();
    void enableAlphaBlend();
    void disableAlphaBlend();
    void loadLayerMatrix();
    void loadTevKColor();
    void applyMatColor();
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

