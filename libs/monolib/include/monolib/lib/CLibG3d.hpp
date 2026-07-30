#pragma once

#include <types.h>
#include "monolib/work/CWorkThread.hpp"

class CLibG3d : public CWorkThread {
public:
    CLibG3d(const char* pName, CWorkThread* pParent);
    virtual ~CLibG3d();
    virtual bool wkStandbyLogin();
    virtual bool wkStandbyLogout();

    DECL_WORKTHREAD_CREATE(CLibG3d);

    static bool isInitialized();
    static CLibG3d* getInstance();

    //0x0: vtable
    //0x0-1c4: CWorkThread
    u32 unk1C4;
};
