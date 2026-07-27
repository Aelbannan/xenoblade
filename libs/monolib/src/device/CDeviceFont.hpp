#pragma once

#include <types.h>

class CDeviceFont {
public:
    CDeviceFont();
    virtual ~CDeviceFont();
    void func_804525D4();
    void func_804525F0();
    void func_80452690();
    void func_8045271C();
    void func_804527A4();
    void func_8045283C();
    void func_804528C4();
    void func_8045294C();
    void func_804529D4();
    void func_80452B78();
    void func_80452C10();
    void func_eu_804558F4();
    void func_80452CF8() const;
    void func_80452D80();
    void wkUpdate();
    void wkRender();
    void wkStandbyLogin();
    void wkStandbyLogout();

    // TODO: add fields
    void getInstance();
};

class CDeviceFontInfo {
public:
    virtual ~CDeviceFontInfo();

    // TODO: add fields
};

