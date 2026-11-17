#pragma once

#include <types.h>

#include "monolib/nand/CNRequest.hpp"

void setGeneralNandErrorMessage(const wchar_t* msg);
void setSaveNandErrorMessage(const wchar_t* msg);
void setExtraNandErrorMessage(const wchar_t* msg);

// Queue of NAND requests, run one at a time from update().
class CNand {
public:
    enum { REQUEST_MAX = 8 };

    CNRequest mRequests[REQUEST_MAX];
    s16 mHead;
    s16 mTail;
    bool mSuccess; // result of the last finished request
    bool mShare;   // work in the "share" sub-directory of the home directory

    CNand();
    virtual ~CNand();

    void reset();
    void update();

    bool check(u32 neededBlocks, u32 neededFiles, u32* answer);
    bool save(const char* name, const void* buf, u32 size, CNBanner* banner, u8 perm);
    bool load(const char* name, void* buf, u32 size);
    bool remove(const char* name);
    bool readdir(char* nameList, u32 size, u32* num);
    bool saveBanner(CNBanner* banner);
    bool loadBanner(void* buf, u32 size);

    void setShare(bool share) { mShare = share; }

private:
    CNRequest* alloc();
};

extern CNand nand;
