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
    static bool func_8045E530();
    static bool isInitialized();

    virtual void wkUpdate();
    virtual void wkRender();
    virtual bool wkStandbyLogin();
    virtual bool wkStandbyLogout();

    //0x0: vtable
    //0x0-1ec: CProc
    HBMControllerData mHBMControllerData; //0x1EC
    u32 unk22C;
    int unk230;
    u32 unk234;
private:
    static const int MAX_CHILD = 8;

    static CLibHbmControl* spInstance;
};
