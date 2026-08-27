#pragma once

#include "monolib/work/CWorkThread.hpp"

struct __declspec(novtable) UnkStruct_8044F65C {
    ~UnkStruct_8044F65C(){}
    virtual void UnkStruct_8044F65C_UnkVirtualFunc1() = 0;
    virtual void UnkStruct_8044F65C_UnkVirtualFunc2() = 0;
    virtual void UnkStruct_8044F65C_UnkVirtualFunc3() = 0;
};

class CDeviceFileJobReadDvd;
class CFileHandle;
class CDeviceFileJob;
class CException;
class IException;

// novtable: the retail vtable/typeinfo for CDeviceFileCri are hand-built in
// CDeviceFileCri.cpp (lbl_eu_8056C354/lbl_eu_806636D8); without novtable MWCC
// emits an implicit __vt__/__RTTI__ chain in the key-function TU.
class __declspec(novtable) CDeviceFileCri : public CWorkThread, public UnkStruct_8044F65C {
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

    bool checkDriveStatus();
    static void closeAdxfHandle();
    static int getFileSize(const char* pPath, int arg1);
    static void cancelJobsForPath(const char* pPath);
    static bool cancel(CFileHandle* pHandle);
    static void func_8044FC38();
    bool func_8044FCFC();
    bool func_80450058();
    bool func_80450260();
    bool func_8045042C();

    static void setCriFilePathA(const wchar_t*);
    static void setCriFilePathB(const wchar_t*);
    static void setCriFilePathC(const wchar_t*);
    static void teardownAdxf(unsigned long);

private:
    CDeviceFileJobReadDvd* getFirstCDeviceFileJobReadDvd();
    void closeADXFAndCleanup();

    u32 mState;                         //0x1C8
    WORK_ID mActiveWorkID;              //0x1CC
    u32 unk1D0;                         //0x1D0
    void* mADXFHandle;                  //0x1D4 (opaque CRI ADXF handle)
    void* mBuffer;                      //0x1D8 (written from allocate_head void*)
    u32 mIdleCounter;                   //0x1DC
    u32 mRetryCounter;                  //0x1E0
    u8 mExceptionPending;               //0x1E4
    u32 mTimeoutCounter;                //0x1E8

    static CDeviceFileCri* sInstance;
};


// C-linkage imports (retail symbol names - keep linkage/signatures verbatim)
extern "C" {
    int func_eu_804521C4();
    int func_eu_804520D0(const char*);
    int func_804DDCD4(const char*, const char*);
    extern u32 lbl_eu_806636C8[1];
    extern u32 lbl_eu_806636CC[1];
    extern u32 lbl_eu_806636D0[2];
    extern u32 lbl_eu_8056C354[];
    extern u8 lbl_eu_806576C8[];
}

extern "C" {
    void func_804591BC__10CExceptionFP10IException(CException* self, IException* pException);
    void func_804591DC__10CExceptionFP10IException(CException* self);
    void logExceptionMessage__10CExceptionFv(const char* msg);
    void func_8045925C__10CExceptionFv(CException* self);
    CException* func_80457CA4__10CExceptionFP11CWorkThreadPCwUl(CWorkThread* thread, const wchar_t* msg, u32 val);
    void removeFileJob__11CDeviceFileFP14CDeviceFileJob(CDeviceFileJob* job);
}

extern "C" {
    void ADXF_Stop(void* adxf);
    int ADXF_GetNumReqSct(void* adxf);
    void ADXF_Close(void* adxf);
    int ADXF_IsOpened(void* adxf);
    int ADXF_GetFsizeByte(void* adxf);
    int ADXF_GetFsizeSct(void* adxf);
    void ADXF_ReadNw(void* adxf, int sectors, void* buffer);
    void* ADXF_OpenNw(const char* filename, int arg);
    // Used by CDeviceFileCri::wkUpdate state-1 (sector-boundary seek before
    // the 1-sector read; retail .text 0x804534D8).
    void ADXF_Seek(void* adxf, u32 sectors, const void* buffer);
}

extern "C" bool isOff__11CWorkSystemFv();
extern "C" void* getInstance__11CWorkSystemFv();
extern "C" void* getInstance__4CLibFv();
extern "C" void advanceReadProgress__11CFileHandleFi(CFileHandle* handle, int val);
extern "C" void destroy__11CFileHandleFv(CFileHandle* handle);
extern "C" void call__11CFileHandleF3CBM(CFileHandle* handle, int cbm);
extern "C" void callCBM3__21CDeviceFileJobReadDvdFv(CDeviceFileJobReadDvd* job);
extern "C" u32 getTargetFramerate__9CDeviceVIFv();
extern "C" void checkDriveStatus__14CDeviceFileCriFv(CDeviceFileCri* self);
