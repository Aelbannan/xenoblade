#pragma once
#include <types.h>
#include "monolib/work/IWorkEvent.hpp"
#include "monolib/util/FixStr.hpp"

struct CEventFile;
struct CFileHandle;

namespace LOD {

/**
 * LOD file / element-pool manager.  0x78 bytes, IWorkEvent-derived; the retail
 * RTTI record at 0x80663848 (name string "LOD::LODMemMan", base list
 * IWorkEvent) belongs to this object's vtable (retail .data 0x8056DB08).
 *
 * It is embedded at LOD::UnkClass_8046D144 +0xA44 and drives the asynchronous
 * archive read (CDeviceFile callbacks + OnFileEvent) and the per-id pool
 * block bookkeeping.  Its low fields have nothing to do with the enclosing
 * manager's fields at the same offsets - the two objects are distinct.
 */
class LODMemMan : public IWorkEvent {
public:
    LODMemMan();
    virtual ~LODMemMan() {}
    virtual bool OnFileEvent(CEventFile* evt);

    void initElementBuffers(u32 handle, u32 size);
    void driveArchiveLoadState();
    u8* getOrCreatePoolData(int id);
    static void allocPersistentBuffer(u32 handle, u32 size);
    static void freePersistentBuffer();
    void resetManagerState();
    void cancelFileAndRelease();
    void clearFlagAndResetName();
    void setArchiveReadyFlag();
    u32 getFlags() const { return mFlags; }
    u8* getData() const { return mData; }
    void allocSecondaryBuffers(u32 count20, u32 count18, u32 count48,
                               u32 count4, u8** outBase, u8** out18,
                               u8** out48, u8** out4);
    void adjustBufferOffsets(int unused, int unused2, int delta);

private:
    /* 0x04 */ u8* mData;            // archive header / element data buffer
    /* 0x08 */ u8* mSecondaryData;   // secondary (element-record) buffer
    /* 0x0C */ u8* mElementList;     // element-list header (pool block list)
    /* 0x10 */ u32 field_0x10;        // (not used by the matched functions)
    /* 0x14 */ u32 field_0x14;        // (not used by the matched functions)
    /* 0x18 */ s32 mCount_18;        // element count
    /* 0x1C */ u8* mView_1C;         // view pointer
    /* 0x20 */ u32 mFlags;           // archive/state flag word
    /* 0x24 */ u8* mBlock;           // allocated pool block
    /* 0x28 */ u32 mBlockOffset;     // size-table offset of mBlock
    /* 0x2C */ u32 mBlockSize;       // size-table size of mBlock
    /* 0x30 */ CFileHandle* mFile;   // archive file handle
    /* 0x34 */ ml::FixStr<64> mName; // archive name string
};

} // namespace LOD
