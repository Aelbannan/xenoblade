#pragma once

#include <types.h>

class CDeviceFileCri {
public:
    CDeviceFileCri();
    virtual ~CDeviceFileCri();
    void func_8044F744();
    void func_8044F964();
    void getFileSize();
    void func_8044FB08();
    void cancel() const;
    void func_8044FC38();
    void func_8044FCFC() const;
    void func_80450058();
    void func_80450260();
    void func_8045042C();
    void wkUpdate();
    void wkStandbyLogin();
    void wkStandbyLogout();
    void wkStandbyExceptionRetry();
    void func_80450AB8();

    // TODO: add fields
    void getInstance();
    void func_80450B14(const wchar_t* pData);
    void func_80450B1C(const wchar_t* pData);
    void func_80450B24(const wchar_t* pData);
};

