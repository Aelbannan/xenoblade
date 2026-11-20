#pragma once

#include <types.h>
#include <revolution/dvd/dvd.h>
#include "monolib/util/reslist.hpp"
#include "monolib/work/CWorkThread.hpp"

// Missing from the SDK headers; defined in revolution/dvd/dvdfs.c.
extern "C" u32 DVDGetTransferredSize(DVDCommandBlock* block);

#include "monolib/lib/CLib.hpp"
#include "monolib/work/CWorkSystem.hpp"

//size: 0x1D8
class CDeviceFileDvd : public CWorkThread {
public:
    CDeviceFileDvd(const char* pName, CWorkThread* pParent);
    virtual ~CDeviceFileDvd();
    static CDeviceFileDvd* getInstance();

    static void isRequestFile(const char* pPath);
    static void cancel(CFileHandle* pHandle);
    static void cancelCurrent();
    bool transState0();
    bool transState3();
    virtual void wkUpdate();
    virtual bool wkStandbyLogin();
    virtual bool wkStandbyLogout();
    virtual bool wkStandbyExceptionRetry(WORK_ID wid);

    static int getFileSize(const char* pPath);

    //0x0: vtable (compiler-emitted)
    //0x0-0x1C4: CWorkThread
    u32 field_0x1C4; //0x1C4 (device state: 0-4)
    s32 field_0x1C8; //0x1C8 (active work id; -1 = none)
    u32 field_0x1CC; //0x1CC (transferred size)
    u8 field_0x1D0;  //0x1D0 (dvd file open flag)
    u32 field_0x1D4; //0x1D4 (drive-status retry counter)

    static CDeviceFileDvd* spInstance; // retail .sbss symbol spInstance__14CDeviceFileDvd
};

struct CFileHandle;
