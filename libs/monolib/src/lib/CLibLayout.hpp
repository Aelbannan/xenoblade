#pragma once

#include <types.h>

class CLibLayout {
public:
    CLibLayout();
    virtual ~CLibLayout();
    void getAllocHandle();
    void isInitialized();
    void createLayout();
    void createArcResourceAccessor();
    void createPicture();
    void createTextbox();
    void deleteTextboxOrPicture();
    void wkStandbyLogin();
    void wkStandbyLogout();
    void func_8045F438();
    void func_8045F4E4();

    // TODO: add fields
    void getInstance();
    void wkUpdate();
};

