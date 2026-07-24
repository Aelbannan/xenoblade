#pragma once

#include "monolib/monolib_types.hpp"
#include "monolib/util/reslist.hpp"
#include "monolib/work/CWorkThread.hpp"

class CProc;
class CView;

struct CViewRootPool {
    _reslist_node<CWorkThread*> mSentinel;
    u8 pad0[0x100 - sizeof(_reslist_node<CWorkThread*>)];
    _reslist_node<CWorkThread*>* mStartNodePtr;
    _reslist_node<CWorkThread*>* mList;
    u32 mUsed;
    s32 mCapacity;
};

class CViewRoot : public CWorkThread {
public:
    CViewRoot(const char* pName, CWorkThread* pParent);
    virtual ~CViewRoot();

    static CViewRoot* create(CWorkThread* pParent);
    static CViewRoot* getInstance();
    static CView* getCurrent();
    static bool isCurrent(const CView* view);
    static bool isCurrentChild(const CView* view, const CView* current);
    static bool isInitialized();
    static void destroyProc(CProc* pProc);
    static void setCurrent(CView* view);
    static void invalidCurrent(CView* view);
    static void func_80442DA8();
    static void renderView();
    static CView* getFullScreenView();
    static CView* getView(WORK_ID id);

    virtual bool wkStandbyLogin();
    virtual bool wkStandbyLogout();

    CViewRootPool mPool0; //0x1C4
    CViewRootPool mPool1; //0x2D4
    CViewRootPool mPool2; //0x3E4
    reslist<WORK_ID> mViewHistory; //0x4F4
    CView* mCurrentView; //0x514
    CProc* mAttachedProc0; //0x518
    CProc* mAttachedProc1; //0x51C
};
