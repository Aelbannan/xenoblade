#pragma once

#include <types.h>

class CDeviceFile {
public:
    CDeviceFile();
    virtual ~CDeviceFile();
    void isInitialized();
    void func_8044E770();
    void func_8044E780();
    void readFile();
    void readCommonArchiveFile();
    void getFileSize();
    void removeFileJob();
    void func_8044F0E4();
    void cancel() const;
    void func_8044F154() const;
    void func_8044F1B8() const;
    void func_8044F400() const;
    void setHandleFlag1() const;
    void setHandleFlag2() const;
    void wkStandbyLogin();
    void wkStandbyLogout();

    // TODO: add fields
    void getInstance();
    void func_8044E768();
};

class CEventFile {
public:
    CEventFile();
    void getFileDataPtr();

    // TODO: add fields
};

