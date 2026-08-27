#pragma once

#include <types.h>
#include "monolib/lod/UnkClass_8046A530.hpp"

struct CEventFile;  // global (retail mangles P10CEventFile)

namespace LOD {

/**
 * LOD memory manager.  Large object; only fields used by matched
 * functions are declared here.  Remaining layout is unknown padding.
 */
class LODMemMan {
public:
    LODMemMan();
    ~LODMemMan();
    float getBaseScale();
    void updateSubObject();
    void initElementBuffers();

    bool OnFileEvent(CEventFile* evt);
    void func_8046D264();
    void func_8046D898();
    void func_8046DA04();
    void clearManagers();
    void func_8046DAC0();
    void updateLodTick();
    void func_8046DD9C();
    void func_8046E1DC();
    void setPauseFlag();
    void attachSceneObjects();
    void setLodScaleAndRefresh();
    void setTimeParams();
    void clearElementFlag();
    void setElementFlag();
    void toggleElementFlag();
    void isElementFlagClear();
    void setElementLodLevel();
    void configureShrinkTargets();
    void setElementVisibility();
    void setElementScale();
    void refreshElementScale();
    void getElementScale();
    void updateElementVisibility();
    void setElementFlag80();
    void func_8046EDD0();
    void getElementState();
    void setElementCachedValue();
    void getElementCachedValue();
    void updateSingleElement();
    void setBaseScale(float a);
    void configureFadeMode();
    void updateViewLayers();
    void updateBoxElement();
    void func_8046F258();
    void func_8046F594();
    void func_8046FC04();
    void func_8046FEB8();
    void linkElementToChains();
    void func_80470184();
    void func_804702F0();
    void func_80470634();
    void func_804708B4();
    void dispatchViewUpdate();
    void handleViewUpdate();
    void initSceneGroup();
    void bindResMdl();
    void acquireResFileSlot();
    void bindModelToSlot();
    void func_8047108C();
    void destroyAllG3dObjects();
    void notifyG3dManager();
    void resetChildMasks();
    void resetLightColor();
    void func_8047133C();
    void setLightScale();
    void initLightScales();
    void func_80471484();
    void allocatePoolBlock();
    void mergeElementList();
    void findPoolBlockById();
    void markPoolBlockReady();
    void initElementList();
    void allocPersistentBuffer();
    void freePersistentBuffer();
    void resetManagerState();
    void cancelFileAndRelease();
    void clearFlagAndResetName();
    void allocSecondaryBuffers();
    void adjustBufferOffsets();
    void getOrCreatePoolData();
    void delegateMergeToSub();
    void setArchiveReadyFlag();
    void driveArchiveLoadState();
private:
    u8  _00[0x7c];                    // unknown leading fields
    f32 mFloat_7C;                    // offset 0x7c
    u8  _80[0xabc - 0x80];            // unknown padding
    UnkClass_8046A530 mSubObject;     // offset 0xabc (embedded sub-object)
};

} // namespace LOD
