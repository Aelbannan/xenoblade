#pragma once

#include <types.h>

struct MpfsysResFileHandle;
typedef struct _GXColor GXColor;

namespace mpfsys {

class UnkClass_80471EC8 {
public:
    bool tryAttach(void* desc, MpfsysResFileHandle* resFile, void* view, u32 arg);
    void update();
    void submitDraw();
    void buildDisplayList();
    void setLayerScale();
    void clearFlag(int index);
    void setFlag(int index);
    void getLayerColors(GXColor* outMat, GXColor* outFog);
    void initLayerRecords();
    void markDirty();
    void setColorMode(s32 mode);
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
    static void setKeyAlpha(f32 value);
    void setFogIndex();
    void resetAmbient();
    void applyAmbient();
    void resetTevSwap();
    void disableZMode();
    void enableZMode();

    // TODO: add fields
};
} // namespace mpfsys

