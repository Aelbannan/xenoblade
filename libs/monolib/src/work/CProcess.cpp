#include "monolib/work.hpp"
#include "monolib/data_vtables.hpp"

// ==== blob monolibdata1/1d dissolve: retail data owned by this TU ====
// rodata 0x80522828 (0x1B): RTTI class-name string for TChildListHeader<CProcess>.
const char lbl_eu_80522828[] = "TChildListHeader<CProcess>";
// sdata 0x806635D8 (0x8): RTTI typeinfo { class-name, cast-table=0 }.
u32 lbl_eu_806635D8[2] = { (u32)&lbl_eu_80522828, 0 };
// data 0x8056BB60 (0x24): CProcess vtable {rtti, 0, dtor, Reset, 0,0,0,0, Tail}.
u32 lbl_eu_8056BB60[9] = {
    (u32)&lbl_eu_80661948, 0, (u32)&__dt__8CProcessFv,
    (u32)&Reset__14CChildListNodeFv, 0, 0, 0, 0, (u32)&Tail__8CProcessFv,
};
/* UNRESOLVED: lbl_eu_8056BB84 (0xC, TChildListHeader<CProcess> vtable
 * {&lbl_eu_806635D8, 0, &__dt__27TChildListHeader<8CProcess>Fv}) - the dtor
 * reloc name cannot be written in C++ source (MWCC rejects '<' in extern "C"
 * ids and &T::~T). MWCC emits the equivalent weak __vt__27TChildListHeader
 * <8CProcess> (0xC, .data @+0x24: {&__RTTI__27TChildListHeader<8CProcess>, 0,
 * &__dt__...Fv}) in this TU; coordinator must rename it to lbl_eu_8056BB84,
 * retarget its [0] reloc -> lbl_eu_806635D8 (this TU already defines that
 * typeinfo; the weak __RTTI__27TChildListHeader<8CProcess> + its @2016 name
 * string must be dropped). Not faked. */

// Retail sbss flag lbl_eu_80665628 (8-byte object; byte 0 = sIsInitialized) - blob monolibdata1d dissolve
u8 lbl_eu_80665628[8];

TChildListHeader<CProcess> CProcessMan::sFreeProcessList;
TChildListHeader<CProcess> CProcessMan::sRootProcessList;

CProcess::CProcess() {
    // novtable: write the retail vptr (0x8056BB60, +0x10) first so the stores
    // land in retail order (vptr, then the member flags, then InsertEnd).
    *(void**)((char*)this + 0x10) = (void*)&lbl_eu_8056BB60;
    mIsRegist = false;
    mIsRemove = false;
    mIsDisableMove = false;
    mIsDisableDraw = false;

    CProcessMan::GetFreeProcessList().InsertEnd(this);
}

CProcess::~CProcess() {
    // novtable: re-store the retail vptr (0x8056BB60, +0x10) like the ctor.
    *(void**)((char*)this + 0x10) = (void*)&lbl_eu_8056BB60;

    //Delete child processes (next fetched before the destructive delete)
    CProcess* iter;
    CProcess* next;
    for (iter = static_cast<CProcess*>(mChildren.Begin());
        iter != nullptr; iter = next) {

        next = static_cast<CProcess*>(mChildren.IterNext(iter));
        delete iter;
    }

    //Remove from process lists
    if (mParent == nullptr) {
        //Retail shape: root list by default, overridden when unregistered.
        TChildListHeader<CProcess>* list = &CProcessMan::GetRootProcessList();
        if (!mIsRegist) {
            list = &CProcessMan::GetFreeProcessList();
        }

        list->Remove(this);
    }
    else {
        mParent->GetChildren().Remove(this);
    }
}

void CProcess::Regist(CProcess* parent, bool insertTop) {
    if (mIsRegist) {
        return;
    }

    CProcessMan::GetFreeProcessList().Remove(this);

    //NULL parent registers to a root list
    if (parent == nullptr) {
        if (insertTop) {
            CProcessMan::GetRootProcessList().InsertTop(this);
        }
        else {
            CProcessMan::GetRootProcessList().InsertEnd(this);
        }
    }
    else {
        if (insertTop) {
            parent->mChildren.InsertTop(this);
        }
        else {
            parent->mChildren.InsertEnd(this);
        }
    }

    SetParent(parent);
    Init();
    mIsRegist = true;
}

void CProcess::Remove() {
    if (mIsRegist != true) {
        return;
    }

    //Unregister children
    for (CProcess* proc = static_cast<CProcess*>(mChildren.End());
        proc != nullptr; proc = static_cast<CProcess*>(mChildren.End())) {

        proc->Remove();
    }

    Term();

    //Remove parent
    if (mParent == nullptr) {
        CProcessMan::GetRootProcessList().Remove(this);
    }
    else {
        mParent->GetChildren().Remove(this);
    }

    CProcessMan::GetFreeProcessList().InsertEnd(this);
    SetParent(nullptr);
    mIsRegist = false;
}

void CProcessMan::Init() {
    if (!lbl_eu_80665628[0]) {
        lbl_eu_80665628[0] = true;
    }
}

void CProcessMan::Term() {
    if (lbl_eu_80665628[0] == true) {
        Reset();
        lbl_eu_80665628[0] = false;
    }
}

void CProcessMan::Reset() {
    CProcess* proc;

    //Terminate all processes
    while ((proc = sRootProcessList.Begin())) {
        proc->Remove();
    }

    //Then delete all processes 
    while ((proc = sFreeProcessList.Begin())) {
        delete proc;
    }
}

void CProcessMan::Move() {
    TChildListHeader<CProcess>& list = GetRootProcessList();
    CProcess* proc;

    for (proc = list.Begin(); proc != nullptr; proc = list.IterNext(proc)) {
        MoveImpl(proc);
    }

    for (proc = list.Begin(); proc != nullptr; proc = list.IterNext(proc)) {
        Remove(proc);
    }
}

void CProcessMan::MoveImpl(CProcess* proc) {
    if (!proc->mIsDisableMove && !proc->mIsRemove) {
        proc->Move();

        //Recurse through child processes
        TChildListHeader<CChildListNode>& children = proc->GetChildren();

        for (CProcess* iter = static_cast<CProcess*>(children.Begin());
            iter != nullptr;
            iter = static_cast<CProcess*>(children.IterNext(iter))) {

            MoveImpl(iter);
        }
    }
}

bool CProcessMan::Remove(CProcess* proc) {
    if (proc->mIsRegist == true && proc->mIsRemove == true) {
        proc->Remove();
        return true;
    }

    //Recurse through child processes
    while (true) {
        bool removedOne = false;
        TChildListHeader<CChildListNode>& children = proc->GetChildren();

        for (CProcess* iter = static_cast<CProcess*>(children.Begin());
            iter != nullptr;
            iter = static_cast<CProcess*>(children.IterNext(iter))) {

            if (Remove(iter)) {
                removedOne = true;
                break;
            }
        }

        if (!removedOne) {
            break;
        }
    };
    
    return false;
}

void CProcessMan::Draw() {
    TChildListHeader<CProcess>& list = GetRootProcessList();
    CProcess* proc;

    for (proc = list.Begin(); proc != nullptr; proc = list.IterNext(proc)) {
        DrawImpl(proc);
    }

    for (proc = list.Begin(); proc != nullptr; proc = list.IterNext(proc)) {
        TailImpl(proc);
    }
}

void CProcessMan::DrawImpl(CProcess* proc) {
    if (!proc->mIsDisableDraw && !proc->mIsRemove) {
        proc->Draw();

        //Recurse through child processes
        TChildListHeader<CChildListNode>& children = proc->GetChildren();

        for (CProcess* iter = static_cast<CProcess*>(children.Begin());
            iter != nullptr;
            iter = static_cast<CProcess*>(children.IterNext(iter))) {

            DrawImpl(iter);
        }
    }
}

void CProcessMan::TailImpl(CProcess* proc) {
    if (!proc->mIsDisableDraw && !proc->mIsRemove) {
        proc->Tail();

        //Recurse through child processes
        TChildListHeader<CChildListNode>& children = proc->GetChildren();

        for (CProcess* iter = static_cast<CProcess*>(children.Begin());
            iter != nullptr;
            iter = static_cast<CProcess*>(children.IterNext(iter))) {

            TailImpl(iter);
        }
    }
}

// Retail inlines DeleteList twice (no separate DeleteList symbol in this TU).
void CProcessMan::Delete() {
    CProcess* proc;
    CProcess* next;

    for (proc = sFreeProcessList.Begin(); proc != nullptr; proc = next) {
        next = sFreeProcessList.IterNext(proc);

        if (proc->mIsRemove == true) {
            delete proc;
        } else {
            DeleteImpl(proc);
        }
    }

    for (proc = sRootProcessList.Begin(); proc != nullptr; proc = next) {
        next = sRootProcessList.IterNext(proc);

        if (proc->mIsRemove == true) {
            delete proc;
        } else {
            DeleteImpl(proc);
        }
    }
}

void CProcessMan::DeleteImpl(CProcess* proc) {   
    //Recurse through child processes
    CProcess* iter;
    CProcess* next;
    TChildListHeader<CChildListNode>& children = proc->GetChildren();

    for (iter = static_cast<CProcess*>(children.Begin());
        iter != nullptr; iter = next) {

        next = static_cast<CProcess*>(children.IterNext(iter));

        if (iter->mIsRemove == true) {
            delete iter;
        } else {
            DeleteImpl(iter);
        }
    }
}
