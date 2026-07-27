#pragma once

#include <types.h>

class CDeviceFileDvd {
public:
    CDeviceFileDvd();
    virtual ~CDeviceFileDvd();
    void getFileSize();
    void isRequestFile();
    void cancel() const;
    void cancelCurrent();
    void transState0();
    void transState3();
    void wkUpdate();
    void wkStandbyLogin();
    void wkStandbyLogout();
    void wkStandbyExceptionRetry();

    // TODO: add fields
    void getInstance();
};

class CFileHandle {
public:
    virtual ~CFileHandle();
    void setup1() const;
    void setup2() const;
    void init() const;
    void func_80451984() const;
    void destroy() const;
    void func_80451CBC() const;
    void call() const;
    void checkExistRsrc() const;
    void getRsrc() const;

    // TODO: add fields
};

