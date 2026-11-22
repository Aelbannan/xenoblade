#pragma once

#include <types.h>
#include "monolib/work/CWorkThread.hpp"

class CFileHandle;
class CEventFile;

// Work thread that streams the system font file and hands it to CDeviceFont.
// size: 0x210
class CDeviceFontLoader : public CWorkThread {
public:
    CDeviceFontLoader(const char* pName, CWorkThread* pParent);
    virtual ~CDeviceFontLoader();
    virtual bool OnFileEvent(CEventFile* pEventFile);
    virtual bool wkStandbyLogin();
    virtual bool wkStandbyLogout();

    //0x0: vtable
    //0x0-1c4: CWorkThread
    u32 mFontArg;              // 0x1C4 setFontPath arg (callers pass 1); forwarded to createFontInfo
    char mFileName[64];        // 0x1C8
    u32 mFileNameLen;          // 0x208
    CFileHandle* mFileHandle;  // 0x20C
};

// setFontPath's retail mangling (Fv) does not encode its two parameters, so it
// cannot be expressed as a C++ member.
extern "C" {
void setFontPath__17CDeviceFontLoaderFv(CDeviceFontLoader* self, u32 arg, const char* pPath);
}
