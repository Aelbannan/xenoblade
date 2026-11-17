#pragma once

#include <types.h>
#include "monolib/lod/UnkClass_8046A530.hpp"
#include "monolib/lod/LODMemMan.hpp"

struct CEventFile;  // global (retail mangles P10CEventFile)

class CTaskLOD;
namespace LOD {

/**
 * LOD memory manager.  Large object; only fields used by matched
 * functions are declared here.  Remaining layout is unknown padding.
 */
// The real name is unknown; anchored at the retail destructor address.
class UnkClass_8046D144 {
public:
    void func_8046E594(int enable);
    void func_8046E5BC(CTaskLOD* source);
    void func_8046F088(CTaskLOD* task);
    void func_8046E920(CTaskLOD* task);
    void func_8046EB50(CTaskLOD* task);
    void func_8046EBEC(CTaskLOD* task);
    void func_8046EEE8(CTaskLOD* task);
    void* func_8046E8C8(CTaskLOD* task);
    float func_8046EC88(CTaskLOD* task);
    void* func_8046EE9C(CTaskLOD* task);
    float func_8046EF30(CTaskLOD* task);
    void* func_8046EF7C(CTaskLOD* task);
    void func_8046F024(int a, int b);
    UnkClass_8046D144();
    ~UnkClass_8046D144();
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
    void setLodScaleAndRefresh(float scale);
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
public:
    u8 _00[0x6c];
    u32 field_0x6c;
    u8 _70[0x7c - 0x70];                    // unknown leading fields /* 0x00 */
    f32 mFloat_7C;                    // 0x7C
    u8  _80[0xa44 - 0x80];            // unknown padding /* 0x80 */
    LODMemMan mMemMan;               // 0xA44, 0x78-byte IWorkEvent callback
    UnkClass_8046A530 mSubObject;     // 0xABC (embedded sub-object)
    f32 mDistanceScale;               // 0x1CDC
};

} // namespace LOD
