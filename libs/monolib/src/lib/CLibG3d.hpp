#pragma once

#include <types.h>

class CLibG3d {
public:
    CLibG3d();
    virtual ~CLibG3d();
    void isInitialized();
    void wkStandbyLogin();
    void wkStandbyLogout();

    // TODO: add fields
    void getInstance();
};

