#pragma once

#include <types.h>

#include "monolib/util/FixStr.hpp"
#include "monolib/work/IWorkEvent.hpp"

class CEventFile;

// Builds the save banner (banner.bin) from textures loaded off the disc.
class CNBanner : public IWorkEvent {
public:
    CNBanner();
    ~CNBanner();
    // IWorkEvent override: only OnFileEvent is ever specialised here.
    virtual bool OnFileEvent(CEventFile* event);
    void setFileList(const char** names, int* ids);
    void setTitle(const char* str);
    void setSubtitle(const char* str);
    void setBannerPath(const char* str);

    // Allocates the banner block and starts reading its textures; mBusy stays
    // set until they have all arrived. Returns false on failure.
    bool load();
    // Cancels pending reads and frees the banner block.
    void unload();

    void* mData;               // 0x004 NANDBanner block, allocated by load()
    u32 mSize;                 // 0x008 bytes of the block in use
    u32 mFlags;                // 0x00C NANDInitBanner flags
    ml::FixStr<64> mTitle;     // 0x010 (title string, mLength at 0x050)
    ml::FixStr<64> mDesc;      // 0x054 (subtitle string, mLength at 0x094)
    ml::FixStr<64> mPath;      // 0x098
    ml::FixStr<64> mFiles[8];  // 0x0DC
    u32 mFileId[8];            // 0x2FC
    s32 mCount;                // 0x31C
    s32 mCountRef;             // 0x320
    u8 mBusy;                  // 0x324
    u8 mFlag;                  // 0x325
    u8 mKind;                  // 0x326
};
