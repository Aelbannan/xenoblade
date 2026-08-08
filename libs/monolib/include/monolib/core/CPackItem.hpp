#pragma once

#include <types.h>
#include "monolib/work.hpp"
#include "monolib/util.hpp"

struct PackHeader {
    u32 mMagic;                   // 0x00 - always 0x00FE1200
    u32 mVersion;                 // 0x04 - always 0x00000002
    u32 mFileHashTableOffset;     // 0x08
    u32 mPkhFilesize;             // 0x0C - total pkh file size
    u32 mFiles;                   // 0x10 - number of files in pack
    char mFilename[32];           // 0x14 - pack filename
    u32 mHashValTableLength;      // 0x34 - length of hash value table
    u8 mHashValTable[0x40];       // 0x38 - bit-position table for hash calculation
    // Hash table: array of u64 pairs (lower, upper) at 0x78
    // Followed by u16 file IDs, then optionally u32 file data offsets
    u64 mFileHashTable[];         // 0x78
};

//size: 0x8C
class CPackItem : public IWorkEvent {
public:
    CPackItem(const char* name, int partitionId);
    virtual ~CPackItem();
    virtual bool OnFileEvent(CEventFile* pEventFile);

    void update();
    bool lookupFile(const char* filename, char** outPkbPath, u32* outEntryId, u32* outIndex, u32* outFileId);
    int findHashIndex(int startIndex, int endIndex);
    bool isNotLoaded();
    bool calculatePackFileHash(const char* filename);
    void setupHashTable();

    enum LoadState {
        LOAD_STATE_NOT_LOADED,
        LOAD_STATE_OPENED_PKH_FILE,
        LOAD_STATE_LOADING_AHX_ADX_FILE,
        LOAD_STATE_LOADED
    };

public:
    //0x0: vtable
    //0x0-4: IWorkEvent

    ml::FixStr<32> mBaseName;       // 0x04 - filename without path or extension
    ml::FixStr<32> mPkbFilename;     // 0x28 - pkb archive path
    CFileHandle* mFileHandle;        // 0x4C
    PackHeader* mPackHeader;         // 0x50
    const char* mArchiveName;        // 0x54 - archive name from constructor
    u64* mFileHashTable;             // 0x58
    u16* mFileIds;                   // 0x5C - per-file ID table (indexed by hash entry)
    u32* mFileDataOffsets;           // 0x60 - per-file data offset or partition ID table
    int mAdxPartitionId;             // 0x64
    u32 field_0x68;                  // 0x68
    u8* mAhxAdxBuffer;               // 0x6C - work buffer for ADX/AHX load
    u32 mHashLowerHalf;              // 0x70
    u32 mHashUpperHalf;              // 0x74
    LoadState mLoadState;            // 0x78
    u8 mFileReadFailed;              // 0x7C - set when async file read errors
    u8 mPackHeaderExternal;          // 0x7D - set when pack header owned by work system
    bool mIsAhxAdxFile;              // 0x7E
    u8 unk7F;                        // 0x7F - padding
    u32 mWorkPackDataPtr;            // 0x80 - pack data pointer from work system
    u32 mWorkPackDataSize;           // 0x84 - pack data size from work system
    const char* mFilePath;           // 0x88 - full path to the pack file
};

// ---------------------------------------------------------------------------
// C-linkage imports (retail symbol names - keep linkage/signatures verbatim)
// ---------------------------------------------------------------------------

// Device-file helpers (defined in monolib/src/device/CDeviceFile.cpp).
// func_eu_804520D0 is NOT declared here: its canonical C-linkage declaration
// lives in monolib/device/CDeviceFileCri.hpp (int, const char*) and the
// inline copy in CPackItem.cpp drifted (void, char*), breaking the build with
// "illegal function overloading" - it was deleted instead of relocated.
extern "C" void func_8044F400__11CDeviceFileFP11CFileHandleUl(CFileHandle*, u32);
extern "C" void func_eu_804521A8(s8);
extern "C" void func_eu_804521B0();
