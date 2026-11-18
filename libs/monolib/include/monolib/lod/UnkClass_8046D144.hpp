#pragma once

#include <types.h>
#include "monolib/lod/UnkClass_8046A530.hpp"
#include "monolib/lod/LODMemMan.hpp"

struct CEventFile;  // global (retail mangles P10CEventFile)
struct LODViewDesc;  // TU-local view record (LODMemMan.cpp)

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
    void teardownG3dObjects(int param);
    void updateLodTick();
    void updateLodLayersForView();
    void updateSubViewFrame();
    void setPauseFlag(int param);
    void attachSceneObjects(u32 param);
    void setLodScaleAndRefresh(float scale);
    void setTimeParams(u32 a, u32 b, u32 c);
    void clearElementFlag(int task);
    void setElementFlag(int task);
    void toggleElementFlag(int task, int flag);
    void* isElementFlagClear(int task);
    void setElementLodLevel();
    void configureShrinkTargets();
    void setElementVisibility(int task, int flag);
    void setElementScale();
    void refreshElementScale(int task);
    f32 getElementScale(int task);
    void updateElementVisibility(int task, int flag);
    void setElementFlag80(int task, int flag);
    void linkTaskElements(int task, int value);
    void* getElementState(int task);
    void setElementCachedValue();
    f32 getElementCachedValue(int task);
    void* updateSingleElement(int id);
    void setBaseScale(float a);
    void configureFadeMode();
    void updateViewLayers(LODViewDesc* view);
    void updateBoxElement();
    void refreshGroupFromList();
    void updateElementsByDesc();
    void refreshPaletteBinding();
    void unlinkElement();
    void linkElementToChains();
    void rebuildElementChains(int count);
    void updateElementLod();
    void updateLodFrame();
    void bindLodChildren();
    static void dispatchViewUpdate(UnkClass_8046D144* self, int task);
    void handleViewUpdate();
    void initSceneGroup();
    void bindResMdl();
    void acquireResFileSlot();
    void bindModelToSlot();
    void teardownSubEntries(UnkClass_8046D144* sub);
    void destroyAllG3dObjects();
    void notifyG3dManager();
    void resetChildMasks();
    void resetLightColor();
    void registerChildSlot();
    void setLightScale(s16 value);
    void initLightScales();
    void reloadChildGxLights(int childIdx);
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
    // getOrCreatePoolData is a LOD::LODMemMan member (see LODMemMan.hpp).
    void delegateMergeToSub();
    void setArchiveReadyFlag();
    // driveArchiveLoadState is a LOD::LODMemMan member (see LODMemMan.hpp).
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
