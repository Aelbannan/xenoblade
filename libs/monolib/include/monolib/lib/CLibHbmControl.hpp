#pragma once

#include <types.h>
#include "monolib/core/CProc.hpp"
#include <revolution/HBM.h>

class CLibHbmControl : public CProc {
public:
    CLibHbmControl(const char* pName, CWorkThread* pParent);
    ~CLibHbmControl();

    DECL_WORKTHREAD_CREATE(CLibHbmControl);

    static CLibHbmControl* create();
    static CLibHbmControl* getInstance();
    static bool isActive();
    static bool isInitialized();

    virtual void wkUpdate();
    virtual void wkRender();
    virtual bool wkStandbyLogin();
    virtual bool wkStandbyLogout();

    //0x0: vtable
    //0x0-1ec: CProc
    HBMControllerData mHBMControllerData; //0x1EC
    u32 mHbmPhase;   //0x22C: state machine phase (0=load arc, 1=wait filesys, 2=init HBM, 3=active)
    int mWaitTimer;  //0x230: countdown timer, set to 210 in phase 0, decremented in phase 1
    u32 mUnused;     //0x234: reserved, never accessed
private:
    static const int MAX_CHILD = 8;

    static CLibHbmControl* spInstance;
};
