#pragma once

#include <types.h>
#include <revolution/GX.h>                  // GXLightObj members
#include <nw4r/g3d/res/g3d_resfile.h>        // nw4r::g3d::ResFile member
#include "monolib/lod/LODMemMan.hpp"
#include "monolib/lod/UnkClass_8046A530.hpp"

struct CEventFile;   // global (retail mangles P10CEventFile)
struct CFileHandle;
class CScn;
struct LODViewDesc;   // TU-local view record (LODMemMan.cpp)
struct LODVec3f;      // TU-local 3-float vector (LODMemMan.cpp)
struct LODElem20;     // TU-local element record (LODMemMan.cpp)
struct LODElem48;     // TU-local element descriptor (LODMemMan.cpp)
struct LODTypeDesc18; // TU-local type descriptor (LODMemMan.cpp)
struct LODRec1C;      // TU-local per-element record (LODMemMan.cpp)
struct LODDesc74;     // TU-local type descriptor (LODMemMan.cpp)
struct LODBoxElem;    // TU-local box element (LODMemMan.cpp)
struct LODChildDesc;  // TU-local child descriptor (LODMemMan.cpp)

namespace nw4r {
namespace g3d {
class G3dObj;
class ScnGroup;
}
}

class CTaskLOD;
namespace LOD {

class UnkClass_8046D144;

/** One g3d object slot of the sub-manager (0xC bytes). */
struct LODG3dSlot {
    /* 0x00 */ nw4r::g3d::G3dObj* mObject; // g3d scene object
    /* 0x04 */ s32 mId;                    // -1 while the slot is free
    /* 0x08 */ u16 field_0x08;             // (only ever cleared)
    /* 0x0A */ u16 mFlags;                 // bit 0: busy, bit 2: torn down
};

/** One child-manager slot of the sub-manager (0xC bytes). */
struct LODChildEntry {
    /* 0x00 */ nw4r::g3d::ResFileData* mResData; // child resource file data
    /* 0x04 */ u8* mBlock;                  // pool block holding the child manager
    /* 0x08 */ s16 mId;                     // element id (-1 while the slot is free)
    /* 0x0A */ u16 mFlags;                  // bit 0: bound
};

/** Per-child GX light block of the sub-manager (0x28C bytes). */
struct LODChildLight {
    /* 0x000 */ GXLightObj mLightObjs[8];  // light objects loaded per slot
    /* 0x200 */ f32        mDirScales[8][4]; // per-slot direction/scale words
    /* 0x280 */ s32        mSlotCount;     // slots in use
    /* 0x284 */ u32        mUsedMask;      // per-slot bound mask
    /* 0x288 */ u32        mDoneMask;      // per-slot initialised mask
};

/**
 * g3d sub-manager of a LOD manager, embedded at UnkClass_8046D144 +0x0CC
 * (0x978 bytes, no vtable of its own).
 *
 * It owns the scene group that holds the manager's g3d objects, the sixteen
 * child-manager slots, the three per-child GX light blocks and the shared
 * light object/scale the LOD light pass reloads every frame.
 */
class LODG3dMgr {
public:
    // The member-init / body split below is what retail's store order requires:
    // the ctor emits mResFile first, then the child-state walk, then mGroup.
    LODG3dMgr() : mResFile(0) {
        LODChildEntry* e = mChildren;
        do {
            e->mResData = 0;
        } while (++e < mChildren + 16);
        mGroup = 0;
        for (int i = 0; i < 16; i++) mChildren[i].mBlock = 0;
        for (int i = 0; i < 16; i++) mSlots[i].mObject = 0;
    }

    // Tear down the g3d slots and unlink the child elements from `sub`.
    void teardownSubEntries(LODMemMan* sub);
    void destroyAllG3dObjects();
    void notifyG3dManager(CScn* scene);
    void initSceneGroup(u8* resBuf, CScn* scene);
    void resetChildMasks();
    void resetLightColor();
    void setLightScale(s16 value);
    void initLightScales();
    void reloadChildGxLights(int childIdx);

    /* 0x000 */ nw4r::g3d::ScnGroup* mGroup;   // scene group holding the objects
    /* 0x004 */ nw4r::g3d::ResFile mResFile;   // bound g3d resource file
    /* 0x008 */ LODChildEntry mChildren[16];   // 0x008..0x0C8
    /* 0x0C8 */ LODG3dSlot mSlots[16];         // 0x0C8..0x188
    /* 0x188 */ LODChildLight mChildLights[3]; // 0x188..0x92C
    /* 0x92C */ GXLightObj mLight;             // shared light object
    /* 0x96C */ f32 mLightScale;               // base light scale
    /* 0x970 */ f32 mStoredLightScale;         // scale of the last reload
    /* 0x974 */ s32 mLastChildIdx;             // child bound to the light
};

/**
 * LOD manager.  0x1CE0 bytes; embedded in CTaskLOD at +0x5C and independently
 * allocated (size 0x1CE0) for the secondary LOD instance.
 *
 * Layout is anchored by the three embedded objects the ctor stores and the
 * dtor destroys:
 *   0x0A44  LOD::LODMemMan            (0x78, IWorkEvent; retail RTTI)
 *   0x0ABC  LOD::UnkClass_8046A530    (0x1220 sub-object)
 *   0x00CC  LOD::LODG3dMgr            (0x978 g3d sub-manager)
 *
 * The class has no vtable/RTTI of its own (retail emits no second RTTI record).
 */
class UnkClass_8046D144 {
public:
    UnkClass_8046D144();
    ~UnkClass_8046D144();

    // Forwards (u32 handle, u32 size) to the embedded LODMemMan and tail-calls
    // it (retail `addi r3,r3,0xA44; b ...`).
    void initElementBuffers(u32 handle, u32 size);

    void rebuildFromSharedBuffer(u32 arg);
    bool resetAndReload(u32 arg, u8* view);
    void resetFrameFields();
    void clearManagers();
    void teardownG3dObjects(int param);
    void updateLodTick();
    void updateLodLayersForView(u8* p, f32 scale);
    void updateSubViewFrame(u8* p, int a);
    void setPauseFlag(int param);
    void attachSceneObjects(u32 param);
    void setLodScaleAndRefresh(float scale);
    void setTimeParams(u32 a, u32 b, u32 c);
    void clearElementFlag(int task);
    void setElementFlag(int task);
    void toggleElementFlag(int task, int flag);
    void* isElementFlagClear(int task);
    void setElementLodLevel(int task, f32 value);
    void configureShrinkTargets(f32 val, s16 task, int flag);
    void setElementVisibility(int task, int flag);
    void setElementScale(int task, f32 value);
    void refreshElementScale(int task);
    f32 getElementScale(int task);
    void updateElementVisibility(int task, int flag);
    void setElementFlag80(int task, int flag);
    void linkTaskElements(int task, int value);
    void* getElementState(int task);
    void setElementCachedValue(int task, f32 value);
    f32 getElementCachedValue(int task);
    void* updateSingleElement(int id);
    void setBaseScale(float a);
    float getBaseScale();
    void configureFadeMode(int mode, u16 value, f32 f1);
    void updateSubObject();
    void updateViewLayers(LODViewDesc* view);
    void updateBoxElement(LODBoxElem* box);
    void refreshGroupFromList(u16 value, u8* data);
    void updateElementsByDesc();
    void refreshPaletteBinding();
    void unlinkElement(LODElem20* elem);
    void linkElementToChains(LODElem20* elem, u16 id, const LODVec3f* v);
    void rebuildElementChains(int count);
    void updateElementLod(LODElem20* elem);
    void updateLodFrame();
    void bindLodChildren(const f32* matrixBlock);
    void handleViewUpdate(int param);
    static void dispatchViewUpdate(UnkClass_8046D144* self, int task);

public:
    /* 0x000 */ nw4r::g3d::G3dObj* field_0x0;    // g3d manager object
    /* 0x004 */ LODElem20* field_0x4;             // 0x20-strided element list
    /* 0x008 */ u8* field_0x8;                    // element-record buffer
    /* 0x00C */ u8* field_0xC;                    // descriptor/pool region
    /* 0x010 */ LODElem20* field_0x10;            // index-list head
    /* 0x014 */ LODElem20* field_0x14;            // index-list tail
    /* 0x018 */ s32 mCount_18;                    // element count
    /* 0x01C */ u8* mView_1C;                     // view pointer
    /* 0x020 */ f32 field_0x20;                   // manager position x
    /* 0x024 */ f32 field_0x24;                   // manager position y
    /* 0x028 */ f32 field_0x28;                   // manager position z
    /* 0x02C */ f32 field_0x2C;                   // plane origin x
    /* 0x030 */ f32 field_0x30;                   // plane origin y
    /* 0x034 */ f32 field_0x34;                   // plane origin z
    /* 0x038 */ f32 field_0x38;                   // plane distance scale
    /* 0x03C */ u32 field_0x3C;                   // per-frame scratch (float bits)
    /* 0x040 */ u32 field_0x40;                   // per-frame scratch
    /* 0x044 */ u32 field_0x44;                   // per-frame scratch
    /* 0x048 */ u32 field_0x48;                   // per-frame scratch
    /* 0x04C */ u32 field_0x4C;                   // per-frame scratch
    /* 0x050 */ u32 field_0x50;                   // per-frame scratch
    /* 0x054 */ u32 field_0x54;                   // cached element slot 0
    /* 0x058 */ u32 field_0x58;                   // cached element slot 1
    /* 0x05C */ u8* field_0x5C;                   // shared buffer base
    /* 0x060 */ LODBoxElem* field_0x60;           // 0x20-strided box elements
    /* 0x064 */ u32* field_0x64;                  // used-element bitmap
    /* 0x068 */ u32 field_0x68;                   // flag word (bit 4)
    /* 0x06C */ u32 field_0x6C;                   // flag bits (bit 0: enabled)
    /* 0x070 */ u32 field_0x70;                   // element-flag accumulator
    /* 0x074 */ f32 field_0x74;                   // inner-radius factor
    /* 0x078 */ f32 field_0x78;                   // outer-radius factor
    /* 0x07C */ f32 mFloat_7C;                   // base scale
    /* 0x080 */ f32 field_0x80;                   // scaled frame value
    /* 0x084 */ f32 field_0x84;                   // second scale value (0.0f init)
    /* 0x088 */ f32 field_0x88;                   // shrink-path stored scale
    /* 0x08C */ f32 field_0x8C;                   // shrink-path stored value
    /* 0x090 */ LODElem48* field_0x90;            // 0x48-strided descriptors
    /* 0x094 */ LODTypeDesc18* field_0x94;        // 0x18-strided type descriptors
    /* 0x098 */ LODRec1C* field_0x98;             // 0x1c-strided records
    /* 0x09C */ LODDesc74* field_0x9C;            // 0x74-strided type descriptors
    /* 0x0A0 */ u8* field_0xA0;                   // descriptor-array base
    /* 0x0A4 */ s32 field_0xA4;                   // type-descriptor count
    /* 0x0A8 */ nw4r::g3d::G3dObj* field_0xA8;    // scene proc (destroyed on reset)
    /* 0x0AC */ nw4r::g3d::G3dObj* field_0xAC;    // scene proc (destroyed on reset)
    /* 0x0B0 */ u32 field_0xB0;                   // cleared on reset
    /* 0x0B4 */ u8* field_0xB4;                   // scene hook (attachSceneObjects)
    /* 0x0B8 */ u16 field_0xB8;                   // time param (setTimeParams)
    /* 0x0BA */ u16 field_0xBA;                   // time param
    /* 0x0BC */ u16 field_0xBC;                   // time param
    /* 0x0BE */ u16 field_0xBE;                   // time param
    /* 0x0C0 */ f32 field_0xC0;                   // LOD distance constant
    /* 0x0C4 */ f32 field_0xC4;                   // LOD distance constant
    /* 0x0C8 */ f32 field_0xC8;                   // distance/scale slot
    /* 0x0CC */ LODG3dMgr mG3dMgr;               // g3d sub-manager (0x978)
    /* 0x0A44 */ LOD::LODMemMan mMemMan;          // embedded archive/element manager
    /* 0x0ABC */ LOD::UnkClass_8046A530 mSubObject; // embedded scene sub-object
    /* 0x1CDC */ f32 field_0x1CDC;                // shared LOD distance scale

    // NOTE: the +0x1CC8 scale slot sits inside the mSubObject tail; setBaseScale
    // reaches it through the LODMemManLayout overlay (open layout item).
};

} // namespace LOD
