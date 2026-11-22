#pragma once

#include <types.h>
#include <revolution/MTX.h>

extern const s32 MpfBillboardCount;

struct MpfsysResFileHandle;
typedef struct _GXColor GXColor;
class CScn;

// Per-layer animation record laid out by initLayerRecords (stride 0xB4).
// Uses a trivial POD vector so member copies stay inline word copies
// (retail never calls the ml::CVec3 assignment operator here).
struct MpfsysVec3 {
    union {
        struct {
            f32 x;
            f32 y;
            f32 z;
        };
        u32 w[3];
    };
};
struct MpfsysLayerRec {
    MpfsysVec3 f00;   // +0x00
    MpfsysVec3 f0C;   // +0x0C
    MpfsysVec3 f18;   // +0x18
    MpfsysVec3 f24;   // +0x24
    MpfsysVec3 f30;   // +0x30
    MpfsysVec3 f3C;   // +0x3C
    MpfsysVec3 dir;   // +0x48
    Mtx mtxA;         // +0x54
    Mtx mtxB;         // +0x84
};

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

    // 64 per-layer animation records (64 x 0xB4 = 0x2D00).
    MpfsysLayerRec mLayers[64]; // +0x0000

    // name-guess: weight table reset by initLayerRecords (+0x2D00..+0x2DDC).
    f32 mWeights[56];
    // name-guess: LOD constant triples reset by initLayerRecords.
    f32 mLod0[3];             // +0x2DE0
    f32 mLod1[4];             // +0x2DEC

    // Written by setLayerScale and read by mpfCullMarkLayers.
    f32 mLayerScale;          // +0x2DFC
    f32 mFoldedLayerScale;    // +0x2E00

    // name-guess: colour-mode selector written by setColorMode (+0x2E04).
    s32 mLayerColorMode;

    // name-guess: scene/camera pointer stored by tryAttach; callers use it
    // as the CScn argument of getScnGroupChild/getScnRootPtr.
    CScn* mCamera;            // +0x2E08
    // name-guess: second pointer passed with mCamera to Scn_HasCamItem.
    void* mView;              // +0x2E0C

    // markDirty sets this byte and submitDraw clears it.
    u8 mDirty;                // +0x2E10
    // name-guess: companion halfword cleared by initLayerRecords.
    u16 mUnkDrawState;        // +0x2E12
};
} // namespace mpfsys

