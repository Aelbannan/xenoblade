#include <nw4r/snd.h>
#include <nw4r/ut.h>

namespace nw4r {
namespace snd {
namespace detail {

// Retail symbol/ABI mirrors. The retail StrmInfo/StrmTrackInfo are nested
// types (StrmFileReader::StrmInfo / StrmFileReader::StrmTrackInfo) that the
// locked header cannot declare, and the retail extension entry points are
// emitted with those exact mangled names via C linkage. These mirror structs
// give the C-linkage functions (which stand in for the missing members)
// plain access to the object layouts and the private reader subobjects.
struct StrmReaderLayout {
    const StrmFile::Header* header;      // StrmFileReader.mHeader    at 0x0
    const StrmFile::HeadBlock* headBlock; // StrmFileReader.mHeadBlock at 0x4
};

struct StrmLoaderLayout {
    ut::FileStream& rStream;                      // StrmFileLoader.mStream    at 0x0
    const StrmFile::Header* readerHeader;         // mReader.mHeader           at 0x4
    const StrmFile::HeadBlock* readerHeadBlock;   // mReader.mHeadBlock        at 0x8
};

struct StrmInfoLayout {
    int format;                  // at 0x0
    u8 loopFlag;                 // at 0x4
    int numChannels;             // at 0x8
    int sampleRate;              // at 0xC
    u16 blockHeaderOffset;       // at 0x10
    u32 loopStart;               // at 0x14
    u32 loopEnd;                 // at 0x18
    u32 dataOffset;              // at 0x1C
    u32 numBlocks;               // at 0x20
    u32 blockSize;               // at 0x24
    u32 blockSamples;            // at 0x28
    u32 lastBlockSize;           // at 0x2C
    u32 lastBlockSamples;        // at 0x30
    u32 lastBlockPaddedSize;     // at 0x34
    u32 adpcmDataInterval;       // at 0x38
    u32 adpcmDataSize;           // at 0x3C
};

// Mirror of StrmFileReader::StrmTrackInfo (locked header cannot declare it).
// Layout: [volume][pan][pad2][int channelCount][channelIndex[]]
struct StrmTrackInfoLayout {
    u8 volume;          // at 0x0
    u8 pan;             // at 0x1
    int channelCount;   // at 0x4
    u8 channelIndex[];  // at 0x8
};

// Version-1 track info entry (carries volume/pan in the file).
struct StrmTrackInfoV1Layout {
    u8 volume;          // at 0x0
    u8 pan;             // at 0x1
    u8 padding[6];      // at 0x2
    u8 channelCount;    // at 0x8
    u8 channelIndex[];  // at 0x9
};

inline const StrmFile::HeadBlock* ReaderHeadBlock(const StrmFileReader* pReader) {
    return reinterpret_cast<const StrmReaderLayout*>(pReader)->headBlock;
}

inline const StrmFileReader* LoaderReaderPtr(const StrmFileLoader* pLoader) {
    return reinterpret_cast<const StrmFileReader*>(
        &reinterpret_cast<const StrmLoaderLayout*>(pLoader)->readerHeader);
}

inline bool LoaderReaderAvailable(const StrmFileLoader* pLoader) {
    return reinterpret_cast<const StrmLoaderLayout*>(pLoader)->readerHeader !=
           NULL;
}

bool StrmFileReader::IsValidFileHeader(const void* pStrmBin) {
    const ut::BinaryFileHeader* pFileHeader =
        static_cast<const ut::BinaryFileHeader*>(pStrmBin);

    if (pFileHeader->signature != SIGNATURE) {
        return false;
    }

    if (pFileHeader->version < NW4R_VERSION(1, 0)) {
        return false;
    }

    if (pFileHeader->version > VERSION) {
        return false;
    }

    return true;
}

StrmFileReader::StrmFileReader() : mHeader(NULL), mHeadBlock(NULL) {}

void StrmFileReader::Setup(const void* pStrmBin) {
    if (!IsValidFileHeader(pStrmBin)) {
        return;
    }

    mHeader = static_cast<const StrmFile::Header*>(pStrmBin);

    mHeadBlock = static_cast<const StrmFile::HeadBlock*>(
        ut::AddOffsetToPtr(mHeader, mHeader->headBlockOffset));

    (void)Util::GetDataRefAddress0(
        mHeadBlock->refDataHeader,
        &mHeadBlock->refDataHeader); // debug leftover
}

bool StrmFileReader::ReadStrmInfo(StrmInfo* pStrmInfo) const {
    const StrmFile::StrmDataInfo* pStrmData = Util::GetDataRefAddress0(
        mHeadBlock->refDataHeader, &mHeadBlock->refDataHeader);

    pStrmInfo->format = pStrmData->format;
    pStrmInfo->loopFlag = pStrmData->loopFlag;
    pStrmInfo->numChannels = pStrmData->numChannels;
    pStrmInfo->sampleRate =
        (pStrmData->sampleRate24 << 16) + pStrmData->sampleRate;
    pStrmInfo->blockHeaderOffset = pStrmData->blockHeaderOffset;
    pStrmInfo->loopStart = pStrmData->loopStart;
    pStrmInfo->loopEnd = pStrmData->loopEnd;
    pStrmInfo->dataOffset = pStrmData->dataOffset;
    pStrmInfo->numBlocks = pStrmData->numBlocks;
    pStrmInfo->blockSize = pStrmData->blockSize;
    pStrmInfo->blockSamples = pStrmData->blockSamples;
    pStrmInfo->lastBlockSize = pStrmData->lastBlockSize;
    pStrmInfo->lastBlockSamples = pStrmData->lastBlockSamples;
    pStrmInfo->lastBlockPaddedSize = pStrmData->lastBlockPaddedSize;
    pStrmInfo->adpcmDataInterval = pStrmData->adpcmDataInterval;
    pStrmInfo->adpcmDataSize = pStrmData->adpcmDataSize;

    return true;
}

bool StrmFileReader::ReadAdpcmInfo(AdpcmInfo* pAdpcmInfo, int channels) const {
    const StrmFile::StrmDataInfo* pStrmData = Util::GetDataRefAddress0(
        mHeadBlock->refDataHeader, &mHeadBlock->refDataHeader);

    if (pStrmData->format != WaveFile::FORMAT_ADPCM) {
        return false;
    }

    const StrmFile::ChannelTable* pChannelTable = Util::GetDataRefAddress0(
        mHeadBlock->refChannelTable, &mHeadBlock->refDataHeader);

    if (channels >= pChannelTable->channelCount) {
        return false;
    }

    const StrmFile::ChannelInfo* pChannelInfo = Util::GetDataRefAddress0(
        pChannelTable->refChannelHeader[channels], &mHeadBlock->refDataHeader);

    const AdpcmInfo* pSrcInfo = Util::GetDataRefAddress0(
        pChannelInfo->refAdpcmInfo, &mHeadBlock->refDataHeader);

    *pAdpcmInfo = *pSrcInfo;
    return true;
}

bool StrmFileLoader::LoadFileHeader(void* pStrmBin, u32 size) {
    u8 headerArea[HEADER_ALIGNED_SIZE + 32];
    u32 bytesRead;

    mStream.Seek(0, ut::FileStream::SEEK_ORIGIN_BEG);
    bytesRead = mStream.Read(ut::RoundUp(headerArea, 32), HEADER_ALIGNED_SIZE);
    if (bytesRead != HEADER_ALIGNED_SIZE) {
        return false;
    }

    StrmFile::Header* pHeader =
        static_cast<StrmFile::Header*>(ut::RoundUp(headerArea, 32));

    StrmFileReader reader;
    if (!reader.IsValidFileHeader(pHeader)) {
        return false;
    }

    if (pHeader->adpcBlockOffset > size) {
        return false;
    }

    u32 loadSize = pHeader->headBlockOffset + pHeader->headBlockSize;

    mStream.Seek(0, ut::FileStream::SEEK_ORIGIN_BEG);
    bytesRead = mStream.Read(pStrmBin, loadSize);
    if (bytesRead != loadSize) {
        return false;
    }

    mReader.Setup(pStrmBin);
    return true;
}

bool StrmFileLoader::ReadAdpcBlockData(u16* pYN1, u16* pYN2, int block,
                                       int channels) {
    if (!mReader.IsAvailable()) {
        return false;
    }

    s32 offset = mReader.GetAdpcBlockOffset() +
                 block * channels * (2 * sizeof(u16)) +
                 sizeof(ut::BinaryBlockHeader);

    mStream.Seek(offset, ut::FileStream::SEEK_ORIGIN_BEG);

    u16 buffer[StrmPlayer::StrmHeader::STRM_CHANNEL_MAX * 2] ALIGN(32);
    if (mStream.Read(buffer, sizeof(buffer)) != sizeof(buffer)) {
        return false;
    }

    for (int i = 0; i < channels; i++) {
        pYN1[i] = buffer[i * 2];
        pYN2[i] = buffer[i * 2 + 1];
    }

    return true;
}

// ---------------------------------------------------------------------------
// Retail C-linkage entry points (fixed ABI, nested-type signatures that the
// locked header cannot express). Bodies reconstructed from the retail ASM.
// ---------------------------------------------------------------------------

// StrmFileReader::ReadStrmInfo(StrmFileReader::StrmInfo*)
extern "C" bool ReadStrmInfo__Q44nw4r3snd6detail14StrmFileReaderCFPQ54nw4r3snd6detail14StrmFileReader8StrmInfo(
    const StrmFileReader* self, StrmInfoLayout* pStrmInfo) {
    const StrmFile::HeadBlock* pHead = ReaderHeadBlock(self);
    const StrmFile::StrmDataInfo* pStrmData = Util::GetDataRefAddress0(
        pHead->refDataHeader, &pHead->refDataHeader);

    int format;
    switch (pStrmData->format) {
    case 1:
        format = 1;
        break;
    case 0:
        format = 2;
        break;
    case 2:
        format = 3;
        break;
    default:
        format = 3;
        break;
    }
    pStrmInfo->format = format;

    pStrmInfo->loopFlag = pStrmData->loopFlag != 0;
    pStrmInfo->numChannels = pStrmData->numChannels;
    pStrmInfo->sampleRate =
        (pStrmData->sampleRate24 << 16) + pStrmData->sampleRate;
    pStrmInfo->blockHeaderOffset = pStrmData->blockHeaderOffset;
    pStrmInfo->loopStart = pStrmData->loopStart;
    pStrmInfo->loopEnd = pStrmData->loopEnd;
    pStrmInfo->dataOffset = pStrmData->dataOffset;
    pStrmInfo->numBlocks = pStrmData->numBlocks;
    pStrmInfo->blockSize = pStrmData->blockSize;
    pStrmInfo->blockSamples = pStrmData->blockSamples;
    pStrmInfo->lastBlockSize = pStrmData->lastBlockSize;
    pStrmInfo->lastBlockSamples = pStrmData->lastBlockSamples;
    pStrmInfo->lastBlockPaddedSize = pStrmData->lastBlockPaddedSize;
    pStrmInfo->adpcmDataInterval = pStrmData->adpcmDataInterval;
    pStrmInfo->adpcmDataSize = pStrmData->adpcmDataSize;
    return true;
}

// StrmFileReader::ReadStrmTrackInfo(StrmFileReader::StrmTrackInfo*, int)
extern "C" bool ReadStrmTrackInfo__Q44nw4r3snd6detail14StrmFileReaderCFPQ54nw4r3snd6detail14StrmFileReader13StrmTrackInfoi(
    const StrmFileReader* self, StrmTrackInfoLayout* pTrackInfo, int trackNo) {
    const StrmFile::TrackTable* pTrackTable = Util::GetDataRefAddress0(
        ReaderHeadBlock(self)->refTrackTable,
        &ReaderHeadBlock(self)->refDataHeader);

    if (trackNo >= pTrackTable->trackCount) {
        return false;
    }

    // TrackData tag byte selects between the two on-disk track-info layouts.
    switch (pTrackTable->trackDataType) {
    case 0: {
        const StrmFile::TrackInfo* pSrcInfo = Util::GetDataRefAddress0(
            pTrackTable->refTrackHeader[trackNo],
            &ReaderHeadBlock(self)->refDataHeader);

        if (pSrcInfo == NULL) {
            return false;
        }

        pTrackInfo->volume = 0x7f;
        pTrackInfo->pan = 0x40;

        int channelCount = pSrcInfo->channelCount;
        pTrackInfo->channelCount = channelCount;

        // Bounded channel-index copy; the bound defaults to STRM_CHANNEL_MAX.
        int count = channelCount > 0x20 ? 0x20 : channelCount;

        for (int i = 0; i < count; i++) {
            pTrackInfo->channelIndex[i] = pSrcInfo->channelIndexTable[i];
        }
        break;
    }
    case 1: {
        const StrmTrackInfoV1Layout* pSrcInfo =
            reinterpret_cast<const StrmTrackInfoV1Layout*>(
                Util::GetDataRefAddress0(pTrackTable->refTrackHeader[trackNo],
                                         &ReaderHeadBlock(self)->refDataHeader));

        if (pSrcInfo == NULL) {
            return false;
        }

        pTrackInfo->volume = pSrcInfo->volume;
        pTrackInfo->pan = pSrcInfo->pan;

        int channelCount = pSrcInfo->channelCount;
        pTrackInfo->channelCount = channelCount;

        int count = channelCount > 0x20 ? 0x20 : channelCount;

        for (int i = 0; i < count; i++) {
            pTrackInfo->channelIndex[i] = pSrcInfo->channelIndex[i];
        }
        break;
    }
    }

    return true;
}

// StrmFileLoader::ReadStrmInfo(StrmFileReader::StrmInfo*)
extern "C" bool ReadStrmInfo__Q44nw4r3snd6detail14StrmFileLoaderCFPQ54nw4r3snd6detail14StrmFileReader8StrmInfo(
    const StrmFileLoader* self, StrmInfoLayout* pStrmInfo) {
    // The reader subobject starts at the loader's mReader.mHeader field.
    const StrmLoaderLayout* pLayout = reinterpret_cast<const StrmLoaderLayout*>(self);

    if (pLayout->readerHeader == NULL) {
        return false;
    }

    const StrmFileReader* pReader =
        reinterpret_cast<const StrmFileReader*>(&pLayout->readerHeader);

    ReadStrmInfo__Q44nw4r3snd6detail14StrmFileReaderCFPQ54nw4r3snd6detail14StrmFileReader8StrmInfo(
        pReader, pStrmInfo);
    return true;
}

// StrmFileLoader::ReadStrmTrackInfo(StrmFileReader::StrmTrackInfo*, int)
extern "C" bool ReadStrmTrackInfo__Q44nw4r3snd6detail14StrmFileLoaderCFPQ54nw4r3snd6detail14StrmFileReader13StrmTrackInfoi(
    const StrmFileLoader* self, StrmTrackInfoLayout* pTrackInfo, int trackNo) {
    if (!LoaderReaderAvailable(self)) {
        return false;
    }

    ReadStrmTrackInfo__Q44nw4r3snd6detail14StrmFileReaderCFPQ54nw4r3snd6detail14StrmFileReader13StrmTrackInfoi(
        LoaderReaderPtr(self), pTrackInfo, trackNo);
    return true;
}

// StrmFileLoader::GetChannelCount()
extern "C" int GetChannelCount__Q44nw4r3snd6detail14StrmFileLoaderCFv(
    const StrmFileLoader* self) {
    if (!LoaderReaderAvailable(self)) {
        return 0;
    }

    const StrmFile::HeadBlock* pHead =
        reinterpret_cast<const StrmLoaderLayout*>(self)->readerHeadBlock;
    const StrmFile::ChannelTable* pChannelTable = Util::GetDataRefAddress0(
        pHead->refChannelTable, &pHead->refDataHeader);

    return pChannelTable->channelCount;
}

// StrmFileLoader::ReadAdpcmInfo(AdpcmParam*, AdpcmLoopParam*, int)
extern "C" bool ReadAdpcmInfo__Q44nw4r3snd6detail14StrmFileLoaderCFPQ44nw4r3snd6detail10AdpcmParamPQ44nw4r3snd6detail14AdpcmLoopParami(
    const StrmFileLoader* self, AdpcmParam* pParam, AdpcmLoopParam* pLoopParam,
    int channel) {
    if (!LoaderReaderAvailable(self)) {
        return false;
    }

    const StrmFile::HeadBlock* pHead =
        reinterpret_cast<const StrmLoaderLayout*>(self)->readerHeadBlock;
    const StrmFile::StrmDataInfo* pStrmData = Util::GetDataRefAddress0(
        pHead->refDataHeader, &pHead->refDataHeader);

    if (pStrmData->format != WaveFile::FORMAT_ADPCM) {
        return true;
    }

    const StrmFile::ChannelTable* pChannelTable = Util::GetDataRefAddress0(
        pHead->refChannelTable, &pHead->refDataHeader);

    if (channel >= pChannelTable->channelCount) {
        return true;
    }

    const StrmFile::ChannelInfo* pChannelInfo = Util::GetDataRefAddress0(
        pChannelTable->refChannelHeader[channel], &pHead->refDataHeader);

    const AdpcmInfo* pAdpcmData = Util::GetDataRefAddress0(
        pChannelInfo->refAdpcmInfo, &pHead->refDataHeader);

    *pParam = pAdpcmData->param;
    *pLoopParam = pAdpcmData->loopParam;
    return true;
}

} // namespace detail
} // namespace snd
} // namespace nw4r