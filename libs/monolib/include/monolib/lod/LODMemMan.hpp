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
    void rebuildFromSharedBuffer();
    void resetAndReload();
    void resetFrameFields();
    void clearManagers();
    void teardownG3dObjects();
    void updateLodTick();
    void updateLodLayersForView();
    void updateSubViewFrame();
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
    void linkTaskElements();
    void getElementState();
    void setElementCachedValue();
    void getElementCachedValue();
    void updateSingleElement();
    void setBaseScale(float a);
    void configureFadeMode();
    void updateViewLayers();
    void updateBoxElement();
    void refreshGroupFromList();
    void updateElementsByDesc();
    void refreshPaletteBinding();
    void unlinkElement();
    void linkElementToChains();
    void rebuildElementChains();
    void updateElementLod();
    void updateLodFrame();
    void bindLodChildren();
    void dispatchViewUpdate();
    void handleViewUpdate();
    void initSceneGroup();
    void bindResMdl();
    void acquireResFileSlot();
    void bindModelToSlot();
    void teardownSubEntries();
    void destroyAllG3dObjects();
    void notifyG3dManager();
    void resetChildMasks();
    void resetLightColor();
    void registerChildSlot();
    void setLightScale();
    void initLightScales();
    void reloadChildGxLights();
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
