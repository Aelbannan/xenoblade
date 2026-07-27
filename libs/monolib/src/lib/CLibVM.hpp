#pragma once

#include <types.h>

class CLibVM {
public:
    CLibVM();
    void isInitialized();
    void setCallbacks();
    void wkStandbyLogin();
    void wkStandbyLogout();

    // TODO: add fields
    virtual ~CLibVM();
    void getInstance();
};

