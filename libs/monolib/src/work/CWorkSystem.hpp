#pragma once

#include <types.h>

class CWorkSystem {
public:
    void wkUpdate();

    // TODO: add fields
    CWorkSystem();
    virtual ~CWorkSystem();
    void getInstance();
    void isOff();
    void getMem();
    void setSaveLoadInvalidReset(bool state);
    void wkStandbyLogin();
    void wkStandbyLogout();
    void create();
    void setExitFunc(ExitFunc func);
    void callExitFunc();
};

