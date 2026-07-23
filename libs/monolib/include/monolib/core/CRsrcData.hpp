#pragma once

#include "monolib/work/CWorkThread.hpp"
#include "monolib/util/MemManager.hpp"

// size: 0x4E8
class CRsrcData : public CWorkThread {
public:
    CRsrcData(const char* pName, CWorkThread* pParent);
    virtual ~CRsrcData();

    virtual void wkUpdate();
    virtual bool wkStandbyLogin();
    virtual bool wkStandbyLogout();

    void destruct(int arg);
    bool releaseCache(const void* data);
    void setRsrcFile(const char* name, void* path, void* data, u32 length, bool flag);
    int isSameName(const char* name) const;

    // Layout matches retail stores (CWorkThread ends at 0x1C4).
    char mName[0x100];     // 0x1C4
    u32 mNameLength;       // 0x2C4
    char mAltPath[0x100];  // 0x2C8
    u32 mAltPathLength;    // 0x3C8
    char mPath[0x100];     // 0x3CC
    u32 mPathLength;       // 0x4CC
    void* mCacheData;      // 0x4D0
    u32 mCacheLength;      // 0x4D4
    u32 mRefCount;         // 0x4D8
    u32 mFlags4DC;         // 0x4DC
    u8 unk4E0;             // 0x4E0
    s16 unk4E2;            // 0x4E2
    s16 unk4E4;            // 0x4E4
};
