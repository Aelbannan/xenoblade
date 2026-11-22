#pragma once

#include <types.h>
#include "monolib/work/CTTask.hpp"
#include "monolib/lod/UnkClass_8046D144.hpp"

class CScn;
class CView;

/*
Task wrapper around the LOD manager (LOD::UnkClass_8046D144).

A single instance is created by CTaskGame and published in spInstance; every
public entry point is a static that forwards to the instance's active manager
(mpActiveLOD), so callers never need the task pointer.  The secondary manager
is a separately allocated LOD::UnkClass_8046D144 that temporarily replaces the
embedded one (acquireLODResource / restorePrimaryLOD / resetActiveLOD).

The `id` parameters are the per-element LOD ids forwarded unchanged to the
manager (retail passes them through r3 -> r4 untouched).
*/
class CTaskLOD : public CTTask<CTaskLOD> {
public:
    CTaskLOD(CScn* scn, CView* view, u32 handle, u32 size);
    virtual ~CTaskLOD();
    virtual void Init();
    virtual void Term();
    virtual void Move();

    // name-guess: all entry-point names below (retail symbols carry no names).
    static CTaskLOD* create(CProcess* parent, CScn* scn, CView* view, u32 handle, u32 size);
    static bool acquireLODResource(void* data, bool enable);
    static void resetActiveLOD(void* data);
    static void enableLOD();
    static void disableLOD();
    static void restorePrimaryLOD();
    static void bindTaskToLOD(void* scene);
    static void notifyLODTick(float scale);
    static void updateLODFrame(float scale);
    static float getLODScale();
    static void setLODFilterFlag(void* enable);
    static void updateLODRange(u16 value, float amount);
    void syncLODTask();
    static void dispatchLODArgs(u16 a, u16 b, u16 c);
    static void activateLOD(int id);
    static void deactivateLOD(int id);
    static void setLODEnable(int id, int enable);
    static bool getLODData(int id);
    static void refreshLOD(int id, f32 value);
    static void setLODParam(u32 id, u32 flag, f32 value);
    static void addLODEntry(int id, int flag);
    static void removeLODEntry(int id, f32 value);
    static void clearLODEntry(int id);
    static f32 getLODDistance(int id);
    static void attachLODObject(int id, int flag);
    static void detachLODObject(int id, int flag);
    static void setLODObject(int id, u16 value);
    static u8 getLODObject(int id);
    static void updateLODObject(int id, f32 value);
    static f32 getLODLevel(int id);
    static void getLODOrSelf(int id);

    static CTaskLOD* spInstance;

    CScn* mScn;                                 // 0x54
    CView* mView;                               // 0x58
    LOD::UnkClass_8046D144 mLODMemMan;          // 0x5C (0x1CE0)
    LOD::UnkClass_8046D144* mpSecondaryLOD;     // 0x1D3C
    LOD::UnkClass_8046D144* mpActiveLOD;        // 0x1D40
};
// Size: 0x1D44 bytes
