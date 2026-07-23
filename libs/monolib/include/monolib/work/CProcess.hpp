#pragma once

#include <types.h>
#include "monolib/util.hpp"

/*
Instance of a game task.

Processes can contain child sub-processes,
and somewhat maintain a priority system.

Each process can implement specific behavior
for its initialization/termination,
and for the update ("Move") and render ("Draw") game events.
*/
class CProcess : public CChildListNode{
    friend class CProcessMan;

public:
    CProcess();
    virtual ~CProcess();

    virtual void Init() = 0;
    virtual void Term() = 0;

    virtual void Move() = 0;
    virtual void Draw() = 0;
    // Out-of-line: inline `{}` emits Tail__8CProcessFv into every derived TU (CfPadTask +.text).
    virtual void Tail();

    void Regist(CProcess* parent, bool insertTop);
    void Remove();

    void SetRemove(){
        mIsRemove = true;    
    }

private:
    //0x0-10: CDoubleListNode
    //0x10: vtable
    //0x14-38: CChildListNode
    bool mIsRegist; //0x38
    bool mIsRemove; //0x39
    bool mIsDisableMove; //0x3A
    bool mIsDisableDraw; //0x3B
};

/*
Process manager.

Responsible for dispatching all process events every tick,
and for maintaining the lists of both root-level processes and released processes.
*/
class CProcessMan {
public:
    static void Reset();
    static void Delete();

    static void Init();
    static void Term();
    
    static void Move();
    static void Draw();
    // Not present as OOL in retail CProcess.s; keep inline API for callers.
    static void Tail() {
        TChildListHeader<CProcess>& list = GetRootProcessList();
        for (CProcess* proc = list.Begin(); proc != nullptr; proc = list.IterNext(proc)) {
            TailImpl(proc);
        }
    }

    static TChildListHeader<CProcess>& GetFreeProcessList() {
        return sFreeProcessList;
    }
    static TChildListHeader<CProcess>& GetRootProcessList() {
        return sRootProcessList;
    }

private:
    static void MoveImpl(CProcess* proc);
    static void DrawImpl(CProcess* proc);
    static void TailImpl(CProcess* proc);

    static bool Remove(CProcess* proc);

    static void DeleteImpl(CProcess* proc);

    static bool sIsInitialized;
    static TChildListHeader<CProcess> sFreeProcessList;
    static TChildListHeader<CProcess> sRootProcessList;
};
