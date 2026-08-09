#pragma once

#include <types.h>
#include "monolib/work/CTTask.hpp"
#include "monolib/lod/LODMemMan.hpp"

/*
Task wrapper around a LODMemMan instance.

CTaskLOD manages the LOD (level-of-detail) memory manager lifecycle.  A single
global instance is reachable through `lbl_eu_80665730`; most public methods
delegate to the manager pointed at by mpActiveLOD (offset 0x1D40).

Inherits from CTTask<CTaskLOD> (0x54 bytes: CDoubleListNode + vtable +
CProcess + Move/Draw member pointers) which provides the Move/Draw callback
slots.  Those base-class Move/Draw slots are overridden here; the explicit
specialisations for CTTask<CTaskLOD> Move/Draw (used when other TUs include
CTasks' inline-only vtable helpers) are separate.
*/
class CTaskLOD : public CTTask<CTaskLOD> {
public:
    virtual ~CTaskLOD();
    virtual void Init();
    virtual void Term();
    virtual void Move();
    virtual void Draw();
    void create();
    void func_80462A08();
    void func_80462AC0();
    void func_80462B30();
    void func_80462B4C();
    void func_80462B68();
    void func_80462BC8();
    void func_80462BE4();
    void func_80462BFC();
    void func_80462C14();
    void func_80462C48();
    void func_80462C80();
    void func_80462CBC();
    void func_80462CD8(void* a, void* b, void* c);
    void func_80462D04();
    void func_80462D5C();
    void func_80462DB4();
    void* func_80462E1C();
    void func_80462E3C();
    void func_80462E58();
    void func_80462ED0(void* a, void* b);
    void func_80462EF4();
    void func_80462F10();
    float func_80462F2C();
    void func_80462F4C(void* a, void* b);
    void func_80462F70(void* a, void* b);
    void func_80462F94(void* a, void* b);
    void* func_80462FB8();
    void func_80462FD8();
    float func_80462FF4();
    void* func_80463014();

private:
    // Layout (0x1D44 total):
    //   0x00-0x53 : CTTask<CTaskLOD> base (CDoubleListNode + vtable + CProcess + PTMFs)
    //   0x54 ..    : unknown state fields (never used in matched code)
    //   0x1D40    : mpActiveLOD (points to the LODMemMan to delegate to)
    u8 _54[0x1D40 - 0x54];       // 0x54 .. 0x1D40

public:
    LOD::LODMemMan* mpActiveLOD; // 0x1D40
};

// The one CTaskLOD instance reference used by the delegates below.
extern CTaskLOD* lbl_eu_80665730;