#pragma once

#include <types.h>

class CScn;
class CScnBloom;
struct CScnCamLayout;

// Scene-root base (size 0xC). Polymorphic: __vt__8CScnRoot (CScnRoot.cpp
// .data) = [rtti, 0, dtor, 7 pure slots, getGroupHandle, getBaseHandle,
// getAllocHandle, pure getBloom]; the three handle getters default to an
// invalid handle. (getBloom must live here: CScnRootNw4r's table has it at
// +0x34 ahead of the IScnObjCallback secondary table, where MWCC would
// append a newly introduced virtual instead.)
// CScnRootNw4r (CScnRootNw4r.hpp) is the only concrete implementation.
class CScnRoot {
public:
    CScnRoot(CScn* scn);
    virtual ~CScnRoot();
    virtual void RemoveId(u32 id) = 0;                  // +0x0C
    virtual void ApplyCamera(CScnCamLayout* cam) = 0;   // +0x10
    virtual void UpdateFrame() = 0;                     // +0x14
    virtual void DrawFrame() = 0;                       // +0x18
    virtual void AttachQueued() = 0;                    // +0x1C
    virtual void AddPair(u32 cb, u32 id) = 0;           // +0x20
    virtual void RemovePair(u32 cb, s32 id) = 0;        // +0x24
    virtual u32 getGroupHandle();                       // +0x28
    virtual u32 getBaseHandle();                        // +0x2C
    virtual u32 getAllocHandle();                       // +0x30
    virtual CScnBloom* getBloom() = 0;                  // +0x34

    CScn* mpScn;        // 0x04 owning scene
    u32 mAttachCount;   // 0x08 camera children attached to the scn groups
};
