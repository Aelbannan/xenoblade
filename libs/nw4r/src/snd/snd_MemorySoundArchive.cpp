#include <nw4r/snd.h>
#include <nw4r/ut.h>

#include <cstring>

namespace nw4r {
namespace snd {

class MemorySoundArchive::MemoryFileStream : public ut::FileStream {
public:
    MemoryFileStream(const void* pBuffer, u32 size);

    virtual void Close();                      // at 0x10
    virtual s32 Read(void* pDst, u32 size);    // at 0x14
    virtual void Seek(s32 offset, u32 origin); // at 0x44

    virtual bool CanSeek() const {
        return true;
    } // at 0x50

    virtual bool CanCancel() const {
        return true;
    } // at 0x54

    virtual bool CanAsync() const {
        return false;
    } // at 0x28

    virtual bool CanRead() const {
        return true;
    } // at 0x2C

    virtual bool CanWrite() const {
        return false;
    } // at 0x30

    virtual u32 Tell() const {
        return mOffset;
    } // at 0x58

    virtual u32 GetSize() const {
        return mSize;
    } // at 0x40

private:
    const void* mData; // at 0x14
    s32 mSize;         // at 0x18
    s32 mOffset;       // at 0x1C
};

MemorySoundArchive::MemorySoundArchive() : mData(NULL) {}

MemorySoundArchive::~MemorySoundArchive() {}

bool MemorySoundArchive::Setup(const void* pBuffer) {
    mFileReader.Init(pBuffer);
    SoundArchive::Setup(&mFileReader);

    const void* pInfoChunk =
        ut::AddOffsetToPtr(pBuffer, mFileReader.GetInfoChunkOffset());

    mFileReader.SetInfoChunk(pInfoChunk, mFileReader.GetInfoChunkSize());

    const void* pStringChunk =
        ut::AddOffsetToPtr(pBuffer, mFileReader.GetLabelStringChunkOffset());

    mFileReader.SetStringChunk(pStringChunk,
                               mFileReader.GetLabelStringChunkSize());

    mData = pBuffer;
    return true;
}

void MemorySoundArchive::Shutdown() {
    mData = NULL;
    SoundArchive::Shutdown();
}

const void* MemorySoundArchive::detail_GetFileAddress(u32 id) const {
    SoundArchive::FilePos pos;
    if (!detail_ReadFilePos(id, 0, &pos)) {
        return NULL;
    }

    SoundArchive::GroupInfo groupInfo;
    if (!detail_ReadGroupInfo(pos.groupId, &groupInfo)) {
        return NULL;
    }

    SoundArchive::GroupItemInfo itemInfo;
    if (!detail_ReadGroupItemInfo(pos.groupId, pos.index, &itemInfo)) {
        return NULL;
    }

    if (groupInfo.extFilePath != NULL) {
        return NULL;
    }

    return ut::AddOffsetToPtr(mData, groupInfo.offset + itemInfo.offset);
}

const void* MemorySoundArchive::detail_GetWaveDataFileAddress(u32 id) const {
    SoundArchive::FilePos pos;
    if (!detail_ReadFilePos(id, 0, &pos)) {
        return NULL;
    }

    SoundArchive::GroupInfo groupInfo;
    if (!detail_ReadGroupInfo(pos.groupId, &groupInfo)) {
        return NULL;
    }

    SoundArchive::GroupItemInfo itemInfo;
    if (!detail_ReadGroupItemInfo(pos.groupId, pos.index, &itemInfo)) {
        return NULL;
    }

    if (groupInfo.extFilePath != NULL) {
        return NULL;
    }

    return ut::AddOffsetToPtr(mData, groupInfo.waveDataOffset +
                                         itemInfo.waveDataOffset);
}

MemorySoundArchive::MemoryFileStream::MemoryFileStream(const void* pBuffer,
                                                       u32 size)
    : mData(pBuffer), mSize(size), mOffset(0) {}

ut::FileStream* MemorySoundArchive::OpenStream(void* pBuffer, int size,
                                               u32 offset, u32 length) const {
    if (mData == NULL) {
        return NULL;
    }

    if (size < sizeof(MemoryFileStream)) {
        return NULL;
    }

    return new (pBuffer)
        MemoryFileStream(ut::AddOffsetToPtr(mData, offset), length);
}

ut::FileStream* MemorySoundArchive::OpenExtStream(void* pBuffer, int size,
                                                  const char* pExtPath,
                                                  u32 offset,
                                                  u32 length) const {
#pragma unused(pBuffer)
#pragma unused(size)
#pragma unused(pExtPath)
#pragma unused(offset)
#pragma unused(length)

    return NULL;
}

int MemorySoundArchive::detail_GetRequiredStreamBufferSize() const {
    return sizeof(MemoryFileStream);
}

void MemorySoundArchive::MemoryFileStream::Close() {
    mData = NULL;
    mSize = 0;
    mOffset = 0;
}

s32 MemorySoundArchive::MemoryFileStream::Read(void* pDst, u32 size) {
    u32 bytesRead = ut::Min<u32>(size, mSize - mOffset);
    std::memcpy(pDst, ut::AddOffsetToPtr(mData, mOffset), bytesRead);

    return bytesRead;
}

void MemorySoundArchive::MemoryFileStream::Seek(s32 offset, u32 origin) {
    switch (origin) {
    case SEEK_ORIGIN_BEG: {
        mOffset = offset;
        break;
    }

    case SEEK_ORIGIN_CUR: {
        mOffset += offset;
        break;
    }

    case SEEK_ORIGIN_END: {
        mOffset = mSize - offset;
        break;
    }

    default: {
        return;
    }
    }
}

} // namespace snd
} // namespace nw4r

// LLM-HARNESS-BEGIN: us-80419880
extern "C" int GetRuntimeTypeInfo__Q34nw4r2ut10FileStreamCFv(void) { return 0; }
// LLM-HARNESS-END: us-80419880
// LLM-HARNESS-BEGIN: us-80419888
extern "C" u32 GetSize__Q44nw4r3snd18MemorySoundArchive16MemoryFileStreamCFv(void* self) { return *(u32*)((u8*)self + 0x18); }
// LLM-HARNESS-END: us-80419888
// LLM-HARNESS-BEGIN: us-80419890
extern "C" u32 Tell__Q44nw4r3snd18MemorySoundArchive16MemoryFileStreamCFv(void* self) { return *(u32*)((u8*)self + 0x1c); }
// LLM-HARNESS-END: us-80419890
// LLM-HARNESS-BEGIN: us-80419898
extern "C" int CanWrite__Q44nw4r3snd18MemorySoundArchive16MemoryFileStreamCFv(void) { return 0x0; }
// LLM-HARNESS-END: us-80419898
// LLM-HARNESS-BEGIN: us-804198a0
extern "C" int CanRead__Q44nw4r3snd18MemorySoundArchive16MemoryFileStreamCFv(void) { return 0x1; }
// LLM-HARNESS-END: us-804198a0
// LLM-HARNESS-BEGIN: us-804198a8
extern "C" int CanAsync__Q44nw4r3snd18MemorySoundArchive16MemoryFileStreamCFv(void) { return 0x0; }
// LLM-HARNESS-END: us-804198a8
// LLM-HARNESS-BEGIN: us-804198b0
extern "C" int CanCancel__Q44nw4r3snd18MemorySoundArchive16MemoryFileStreamCFv(void) { return 0x1; }
// LLM-HARNESS-END: us-804198b0
// LLM-HARNESS-BEGIN: us-804198b8
extern "C" int CanSeek__Q44nw4r3snd18MemorySoundArchive16MemoryFileStreamCFv(void) { return 0x1; }
// LLM-HARNESS-END: us-804198b8
