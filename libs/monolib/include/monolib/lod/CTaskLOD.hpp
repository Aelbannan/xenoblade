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

    bool func_80462A08(bool enable);
    void func_80462AC0();
    void func_80462B30();
    void func_80462B4C();
    void func_80462B68();
    void func_80462BC8(void* param);
    void func_80462BE4();
    void func_80462BFC();
    float func_80462C14();
    void func_80462C48(bool flag);
    void func_80462C80(bool flag);
    void func_80462CBC(void* param);
    void func_80462CD8(void* a, void* b, void* c);
    void func_80462D04(void* param);
    void func_80462D5C(void* param);
    void func_80462DB4(void* a, void* b);
    void* func_80462E1C(void* param);
    void func_80462E3C(void* param);
    void func_80462E58(float val, void* a, void* b);
    void func_80462ED0(void* a, void* b);
    void func_80462EF4(void* param);
    void func_80462F10(void* param);
    float func_80462F2C(void* param);
    void func_80462F4C(void* a, void* b);
    void func_80462F70(void* a, void* b);
    void func_80462F94(void* a, void* b);
    void* func_80462FB8(void* param);
    void func_80462FD8(void* param);
    float func_80462FF4(void* param);
    void func_80463014(void* param);

private:
    // Layout (0x1D44 total):
    //   0x00-0x53 : CTTask<CTaskLOD> base (CProcess + PTMFs)
    //   0x54      : mParam1
    //   0x58      : mParam2
    //   0x5C      : mLODMemMan (embedded / primary)
    //   0x1D3C    : mpSecondaryLOD (optional separately-allocated LODMemMan)
    //   0x1D40    : mpActiveLOD (preferred: points to &mLODMemMan or mpSecondaryLOD)

    void*       mParam1;          // 0x54
    void*       mParam2;          // 0x58
    LOD::LODMemMan mLODMemMan;   // 0x5C
    void*       mpSecondaryLOD;  // 0x1D3C
    LOD::LODMemMan* mpActiveLOD; // 0x1D40
};
// Size: 0x1D44 bytes
