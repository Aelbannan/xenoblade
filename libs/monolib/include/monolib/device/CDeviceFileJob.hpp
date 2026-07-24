#pragma once

#include <types.h>
#include "monolib/work/CWorkThread.hpp"
#include "monolib/device/CFileHandle.hpp"

// Forward declaration for the parameter type of cancel overload
struct CDeviceFileJob_UnkStruct1;

//Base class for jobs carried out by CDeviceFile.
class CDeviceFileJob : public CWorkThread {
public:
    CDeviceFileJob(const char* pName, CWorkThread* pParent);

    virtual ~CDeviceFileJob(){}
    virtual bool CDeviceFileJob_UnkVirtualFunc1(){ return false; }
    virtual bool cancel(const char* pFilename);
    virtual bool cancel(CDeviceFileJob_UnkStruct1* pStruct){ return false; }

    inline const char* getFilename(){
        return mHandle->mName.c_str();
    }

    inline void call(CBM cbm){
        mHandle->call(cbm);
    }

    //0x0: vtable
    //0x0-1C4: CWorkThread (parent class)
    CFileHandle* mHandle; //0x1C4 — file handle for the current job
    u8 unk1C8; //0x1C8 — unknown byte field (possibly FixStr-related)
    u8 unk1C9[0x208 - 0x1C9]; //padding
    u32 unk208; //0x208 — unknown status field
    u32 unk20C; //0x20C — unknown status field
    u8 unk210; //0x210 — unknown byte flag
};
