#pragma once

#include <types.h>

#include "monolib/device/CDeviceBase.hpp"
#include "monolib/monolib_types.hpp"
#include "monolib/util.hpp"
#include "monolib/util/reslist.hpp"

class CFileHandle;
class CDeviceFileJob;

//size: 0x1f0
class CDeviceFile : public CDeviceBase {
public:
    CDeviceFile(const char* pName, CWorkThread* pParent);
    virtual ~CDeviceFile();
    static CDeviceFile* getInstance();

    DECL_WORKTHREAD_CREATE(CDeviceFile);

    static int setParent(CWorkThread* parent);
    bool clearParent();
    static void cancelJobsForPath(const char* pPath);
    static int isInitialized();
    static bool isStandbyReady();
    static bool tryUpdateJobPriority(CFileHandle* pFileHandle, int param);
    bool updateJobPriority(CFileHandle* pFileHandle, int param);
    static void setHandleParam(CFileHandle* pHandle, unsigned long param);
    static int getFileSize(const char* pPath);
    static CFileHandle* readFile(mtl::ALLOC_HANDLE allocHandle, const char* pPath, IWorkEvent* pWorkEvent, int r6, int r7);
    static CFileHandle* readCommonArchiveFile(mtl::ALLOC_HANDLE allocHandle, const char* pPath, IWorkEvent* pWorkEvent, int r6, int r7);
    static bool removeFileJob(CDeviceFileJob* pJob);
    static void cancel(CFileHandle* pFileHandle);
    static void setHandleFlag1(CFileHandle* pFileHandle);
    static void setHandleFlag2(CFileHandle* pFileHandle);

    virtual bool wkStandbyLogin();
    virtual bool wkStandbyLogout();

    //0x0: vtable
    //0x0-1c8: CDeviceBase
    CWorkThread* field_0x1C8;             //0x1C8
    reslist<CFileHandle*> mFileList;      //0x1CC
    u8 field_0x1EC[0x1F0 - 0x1EC];       //0x1EC
};

extern s8 sLanguageOverride;
extern u8 sStandbyLoginDoneFlag;
