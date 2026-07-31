#pragma once

#include "monolib/work/CWorkThread.hpp"

struct UnkStruct_8044F65C {
    ~UnkStruct_8044F65C(){}
    virtual void UnkStruct_8044F65C_UnkVirtualFunc1() = 0;
    virtual void UnkStruct_8044F65C_UnkVirtualFunc2() = 0;
    virtual void UnkStruct_8044F65C_UnkVirtualFunc3() = 0;
};

class CDeviceFileJobReadDvd;
class CFileHandle;

class CDeviceFileCri : public CWorkThread, public UnkStruct_8044F65C {
public:
    CDeviceFileCri(const char* pName, CWorkThread* pParent, int capacity = 0x100);
    ~CDeviceFileCri();
    static CDeviceFileCri* getInstance();

    DECL_WORKTHREAD_CREATE(CDeviceFileCri);

    virtual void wkUpdate();
    virtual bool wkStandbyLogin();
    virtual bool wkStandbyLogout();
    virtual bool wkStandbyExceptionRetry(u32 wid);

    virtual void UnkStruct_8044F65C_UnkVirtualFunc1();
    virtual void UnkStruct_8044F65C_UnkVirtualFunc2();
    virtual void UnkStruct_8044F65C_UnkVirtualFunc3();

    bool func_8044F744();
    static void func_8044F964();
    static int getFileSize(const char* pPath, int arg1);
    static void func_8044FB08(const char* pPath);
    static bool cancel(CFileHandle* pHandle);
    static void func_8044FC38();
    bool func_8044FCFC();
    bool func_80450058();
    bool func_80450260();
    bool func_8045042C();

    static void func_80450B14(const wchar_t*);
    static void func_80450B1C(const wchar_t*);
    static void func_80450B24(const wchar_t*);
    static void func_80450AB8(unsigned long);

private:
    CDeviceFileJobReadDvd* getFirstCDeviceFileJobReadDvd();
    void closeADXFAndCleanup();

    u32 mState;                         //0x1C8
    WORK_ID mActiveWorkID;              //0x1CC
    u32 unk1D0;                         //0x1D0
    void* mADXFHandle;                  //0x1D4
    void* mBuffer;                      //0x1D8
    u32 mIdleCounter;                   //0x1DC
    u32 mRetryCounter;                  //0x1E0
    u8 mExceptionPending;               //0x1E4
    u32 mTimeoutCounter;                //0x1E8

    static CDeviceFileCri* sInstance;
};
