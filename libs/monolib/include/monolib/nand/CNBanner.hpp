#pragma once

#include <types.h>
#include "monolib/work.hpp"

class CNBanner : IWorkEvent {
public:
    CNBanner();
    virtual ~CNBanner();
    void init();
    virtual bool OnFileEvent(CEventFile* pEventFile);
    void setTitle(const char* str);
    void setSubtitle(const char* str);
    void setBannerPath(const char* str);
    void setFileList(const char** filenames, int* someArray);

    //0x0: vtable
    u8 unk4[0xC - 0x4];
    u32 unkC;
    u8 unk10[0x325 - 0x10];
};
