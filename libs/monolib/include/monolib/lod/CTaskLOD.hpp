#pragma once

#include <types.h>
#include "monolib/work/CTTask.hpp"
#include "monolib/lod/LODMemMan.hpp"

/*
Task wrapper around a LODMemMan instance.

CTaskLOD manages the LOD (level-of-detail) memory manager lifecycle.  It owns
an embedded LODMemMan member and can optionally allocate a secondary
swap‑instance.  Most public methods delegate to the active LODMemMan.

Inherits from CTTask<CTaskLOD> which provides the Move/Draw callback slots.
*/
class CTaskLOD : public CTTask<CTaskLOD> {
public:
    virtual ~CTaskLOD();
    virtual void Init();
    virtual void Term();
    virtual void Move();
    virtual void Draw();

    // Factory: allocate and initialise a full CTaskLOD from work memory.
    static CTaskLOD* create(CProcess* parent, void* p1, void* p2,
                            void* p3, void* p4);

    // ---- active-LODMemMan wrappers -----------------------------------------

    bool acquireLODResource(bool enable);
    void resetActiveLOD();
    void enableLOD();
    void disableLOD();
    void restorePrimaryLOD();
    void bindTaskToLOD(void* param);
    void notifyLODTick();
    void updateLODFrame();
    float getLODScale();
    void setLODFilterFlag(bool flag);
    void updateLODRange(bool flag);
    void syncLODTask(void* param);
    void dispatchLODArgs(void* a, void* b, void* c);
    void activateLOD(void* param);
    void deactivateLOD(void* param);
    void setLODEnable(void* a, void* b);
    void* getLODData(void* param);
    void refreshLOD(void* param);
    void setLODParam(float val, void* a, void* b);
    void addLODEntry(void* a, void* b);
    void removeLODEntry(void* param);
    void clearLODEntry(void* param);
    float getLODDistance(void* param);
    void attachLODObject(void* a, void* b);
    void detachLODObject(void* a, void* b);
    void setLODObject(void* a, void* b);
    void* getLODObject(void* param);
    void updateLODObject(void* param);
    float getLODLevel(void* param);
    void* getLODOrSelf(void* param);

private:
    // Layout (0x1D44 total):
    //   0x00-0x53 : CTTask<CTaskLOD> base (CProcess + PTMFs)
    //   0x54      : mParam1
    //   0x58      : mParam2
    //   0x5C      : mLODMemMan (embedded / primary)
    //   0x1D3C    : mpSecondaryLOD (optional separately-allocated LODMemMan)
    //   0x1D40    : mpActiveLOD (preferred: points to &mLODMemMan or mpSecondaryLOD)

    void*       mParam1;          // 0x54 (unknowable pointee - never used in code)
    void*       mParam2;          // 0x58 (unknowable pointee - never used in code)
    LOD::LODMemMan mLODMemMan;   // 0x5C
    LOD::LODMemMan* mpSecondaryLOD; // 0x1D3C
    LOD::LODMemMan* mpActiveLOD; // 0x1D40
};
// Size: 0x1D44 bytes
