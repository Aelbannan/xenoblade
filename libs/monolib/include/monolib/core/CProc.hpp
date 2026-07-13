#pragma once

#include <types.h>
#include "monolib/monolib_types.hpp"
#include "monolib/work/CWorkThread.hpp"
#include "monolib/util.hpp"

struct CProc_UnkStruct1 {
    void* unk0;
    void* unk4;
    void* unk8;
    void* unkC;
};

//size: 0x1ec
class CProc : public CWorkThread {
public:
    CProc(const char* pName, CWorkThread* pParent, s16 capacity);
    virtual ~CProc();
    virtual bool wkStandbyLogin();
    virtual bool wkStandbyLogout();

    static CProc* pssGetRoot(CProc* pProc);
    void pssSetFocus();
    bool pssDetachView(WORK_ID id);
    // pssGetView / pssAttachView / no-arg pssDetachView / pssMakeClientRect are
    // not separate US .text symbols; attach/detach-all are inlined at call sites.
    CView* pssCreateView(const char* pName, CWorkThread* pThread2, int r6);

    CWorkThread* pssGetParent() const {
        return mParent;
    }

    //Tries to cast the given thread to CProc.
    static CProc* convertToProc(CWorkThread* pThread) {
        if(pThread == nullptr){
            return nullptr;
        }

        int type = pThread->mType;

        //Check if the thread's type is in the CProc range
        if(THREAD_CPROC > type || type >= THREAD_CPROC_MAX) return nullptr;
        return static_cast<CProc*>(pThread);
    }

    WORK_ID getFirstViewID() const {
        return mViewIDList.front();
    }

    //0x0: vtable
    //0x0-1c4: CWorkThread
    reslist<WORK_ID> mViewIDList; //0x1C4
    u32 unk1E4;
    u32 unk1E8;
};
