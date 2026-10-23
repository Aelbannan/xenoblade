#pragma once

#include <types.h>
#include "monolib/work/CWorkThread.hpp"

class CFileHandle;
class CEventFile;

// Work thread that streams the system font file and hands it to CDeviceFont.
// size: 0x210
// __declspec(novtable): the retail vtable is the hand-laid lbl_eu_8056C8A8
// (blob dissolve) stored explicitly by the ctor; novtable also keeps MWCC from
// emitting a local vtable + weak stubs for the inherited virtuals.
class __declspec(novtable) CDeviceFontLoader : public CWorkThread {
public:
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

// Retail symbols that cannot be expressed as C++ members: the ctor is emitted
// unmangled, and setFontPath's retail mangling (Fv) does not encode its two
// parameters.
extern "C" {
CDeviceFontLoader* __ct__CDeviceFontLoader(CDeviceFontLoader* self, const char* pName, CWorkThread* pParent);
void setFontPath__17CDeviceFontLoaderFv(CDeviceFontLoader* self, u32 arg, const char* pPath);
}
