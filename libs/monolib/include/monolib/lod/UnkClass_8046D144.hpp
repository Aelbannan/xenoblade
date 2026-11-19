#pragma once

#include <types.h>
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
}
}

class CTaskLOD;
namespace LOD {

/**
 * LOD manager.  0x1CE0 bytes; embedded in CTaskLOD at +0x5C and independently
 * allocated (size 0x1CE0) for the secondary LOD instance.
 *
 * Layout is anchored by the three embedded objects the ctor stores and the
 * dtor destroys:
 *   0x0A44  LOD::LODMemMan            (0x78, IWorkEvent; retail RTTI)
 *   0x0ABC  LOD::UnkClass_8046A530    (0x1220 sub-object)
 *   0x00CC  g3d sub-manager           (0x978; class not yet recovered, kept
 *                                      as an opaque block: its helpers in
 *                                      LODMemMan.cpp work on it through
 *                                      LODMemManLayout with this = +0xCC)
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

    // g3d sub-manager helpers: these receive the +0xCC sub-object as `this`
    // (their addresses are still anchored here until that class is recovered).
    void teardownSubEntries(UnkClass_8046D144* sub);
    void resetChildMasks();
    void resetLightColor();
    void setLightScale(s16 value);
    void initLightScales();
    void reloadChildGxLights(int childIdx);

public:
    /* 0x000 */ nw4r::g3d::G3dObj* field_0x0;    // g3d manager object
    /* 0x004 */ LODElem20* field_0x4;             // 0x20-strided element list
    /* 0x008 */ u8* field_0x8;                    // element-record buffer
    /* 0x00C */ u8* field_0xC;                    // descriptor/pool region
    /* 0x010 */ LODElem20* field_0x10;            // index-list head
    /* 0x014 */ LODElem20* field_0x14;            // index-list tail
    /* 0x018 */ s32 mCount_18;                    // element count
    /* 0x01C */ u8* mView_1C;                     // view pointer
    /* 0x020 */ u32 field_0x20;                   // manager position x (float bits)
    /* 0x024 */ u32 field_0x24;                   // manager position y (float bits)
    /* 0x028 */ u32 field_0x28;                   // manager position z (float bits)
    /* 0x02C */ u32 field_0x2C;                   // plane origin x (float bits)
    /* 0x030 */ u32 field_0x30;                   // plane origin y (float bits)
    /* 0x034 */ u32 field_0x34;                   // plane origin z (float bits)
    /* 0x038 */ u32 field_0x38;                   // plane distance scale
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
    /* 0x074 */ s32 field_0x74;                   // inner-radius factor
    /* 0x078 */ u8 mPad_78[4];
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
    /* 0x0CC */ u8 mG3d[0xA44 - 0x0CC];           // g3d sub-manager (0x978)
    /* 0x0A44 */ LOD::LODMemMan mMemMan;          // embedded archive/element manager
    /* 0x0ABC */ LOD::UnkClass_8046A530 mSubObject; // embedded scene sub-object
    /* 0x1CDC */ f32 field_0x1CDC;                // shared LOD distance scale

    // NOTE: the +0x1CC8 scale slot sits inside the mSubObject tail; setBaseScale
    // reaches it through the LODMemManLayout overlay (open layout item).
};

} // namespace LOD
