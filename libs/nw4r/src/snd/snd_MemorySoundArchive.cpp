#include <nw4r/snd.h>
#include <nw4r/ut.h>

#include <cstring>

namespace nw4r {
namespace ut {

// Retail never emits a standalone ~IOStream symbol; the destructor is
// inline-empty so MWCC elides the base-dtor call in derived destructors
// (retail MemoryFileStream dtor has no base call). The definition must be
// visible in this TU; without it MWCC emits a `bl ~FileStream` in the
// derived destructor (retail has none).
inline IOStream::~IOStream() {}

} // namespace ut
} // namespace nw4r

namespace nw4r {
namespace snd {

// MemoryFileStream is a private stream adapter over a caller-supplied
// buffer. The trivial query virtuals (CanSeek/CanCancel/CanAsync/CanRead/
// CanWrite/Tell/GetSize) and GetRuntimeTypeInfo are defined out-of-line
// below so they become strong symbols matching the retail split (inline
// in-class bodies would instead be emitted as extra weak vtable copies).
class MemorySoundArchive::MemoryFileStream : public ut::FileStream {
public:
    MemoryFileStream(const void* pBuffer, u32 size)
        : mData(pBuffer), mSize(size), mOffset(0) {}
    virtual ~MemoryFileStream(); // at 0xC

    virtual void Close();                      // at 0x10
    virtual s32 Read(void* pDst, u32 size);    // at 0x14
    virtual void Seek(s32 offset, u32 origin); // at 0x44

    virtual bool CanSeek() const;   // at 0x50
    virtual bool CanCancel() const; // at 0x54

    virtual bool CanAsync() const; // at 0x28
    virtual bool CanRead() const;  // at 0x2C
    virtual bool CanWrite() const; // at 0x30

    virtual u32 Tell() const;    // at 0x58
    virtual u32 GetSize() const; // at 0x40

private:
    const void* mData; // at 0x14
    s32 mSize;         // at 0x18
    s32 mOffset;       // at 0x1C
};

MemorySoundArchive::MemorySoundArchive() : mData(NULL) {}

MemorySoundArchive::~MemorySoundArchive() {}

MemorySoundArchive::MemoryFileStream::~MemoryFileStream() {}

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

bool MemorySoundArchive::MemoryFileStream::CanSeek() const {
    return true;
}

bool MemorySoundArchive::MemoryFileStream::CanCancel() const {
    return true;
}

bool MemorySoundArchive::MemoryFileStream::CanAsync() const {
    return false;
}

bool MemorySoundArchive::MemoryFileStream::CanRead() const {
    return true;
}

bool MemorySoundArchive::MemoryFileStream::CanWrite() const {
    return false;
}

u32 MemorySoundArchive::MemoryFileStream::Tell() const {
    return mOffset;
}

u32 MemorySoundArchive::MemoryFileStream::GetSize() const {
    return mSize;
}

} // namespace snd
} // namespace nw4r
