#pragma once

#include <types.h>
#include <revolution/dvd/dvd.h>
#include "monolib/util/reslist.hpp"

// Missing from the SDK headers; defined in revolution/dvd/dvdfs.c.
extern "C" u32 DVDGetTransferredSize(DVDCommandBlock* block);

// Minimal layout view of CWorkThread (same class name + signatures -> the
// base-ctor/dtor/login/logout calls resolve to the real retail symbols). The
// real CWorkThread is virtual; declaring it non-virtual here keeps
// CDeviceFileDvd's same-signature methods hiding (not overriding) so MWCC
// emits no vtable of its own, and the ctor can store the retail rodata vtable
// (lbl_eu_8056C420) manually. Only mChildren is accessed from this TU.
class CWorkThread {
public:
    CWorkThread(const char* pName, CWorkThread* pParent, int capacity);
    ~CWorkThread();
    bool wkStandbyLogin();
    bool wkStandbyLogout();

    //0x0: vtable (real CWorkThread is polymorphic; pad 0x0-0x5C here)
    u8 field_0x0[0x5C];              //0x0 (vtable + name/state/id/type/alloc/parent)
    reslist<CWorkThread*> mChildren; //0x5C
    u8 field_0x7C[0x1C4 - 0x7C];     //0x7C (flags/msgqueue/etc.)
};

// Minimal views of the work/CLib singletons used by wkStandbyLogout. The
// static getInstance() calls resolve to the retail symbols; this TU does not
// include the full system headers.
class CWorkSystem {
public:
    static CWorkSystem* getInstance();
};

class CLib {
public:
    static CLib* getInstance();
};

//size: 0x1D8
class CDeviceFileDvd : public CWorkThread {
public:
    CDeviceFileDvd(const char* pName, CWorkThread* pParent);
    ~CDeviceFileDvd();
    static CDeviceFileDvd* getInstance();

    static void isRequestFile(const char* pPath);
    static void cancel(CFileHandle* pHandle);
    static void cancelCurrent();
    bool transState0();
    bool transState3();
    void wkUpdate();
    bool wkStandbyLogin();
    bool wkStandbyLogout();
    int wkStandbyExceptionRetry(u32 wid);

    static int getFileSize(const char* pPath);

    //0x0: vtable (retail rodata lbl_eu_8056C420 - stored manually in the ctor)
    //0x0-0x1C4: CWorkThread
    u32 field_0x1C4; //0x1C4 (device state: 0-4)
    s32 field_0x1C8; //0x1C8 (active work id; -1 = none)
    u32 field_0x1CC; //0x1CC (transferred size)
    u8 field_0x1D0;  //0x1D0 (dvd file open flag)
    u32 field_0x1D4; //0x1D4 (drive-status retry counter)

    static CDeviceFileDvd* spInstance; // -> retail lbl_eu_80665670
};

struct CFileHandle;
