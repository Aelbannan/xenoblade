#include <nw4r/snd.h>
#include <nw4r/ut.h>

namespace nw4r {
namespace snd {
namespace detail {

// Version-1 track info entry (carries volume/pan in the file).
struct StrmTrackInfoV1Layout {
    u8 volume;          // at 0x0
    u8 pan;             // at 0x1
    u8 padding[6];      // at 0x2
    u8 channelCount;    // at 0x8
    u8 channelIndex[];  // at 0x9
};

// Retail has no standalone IsValidFileHeader / Setup symbol: MWCC inlines
// both into StrmFileLoader::LoadFileHeader, so they are defined inline here to
// avoid emitting extra out-of-line copies (same approach as snd_WsdFile.cpp).
// Their signatures take the raw file image as declared in the locked header;
inline bool StrmFileReader::IsValidFileHeader(
    const ut::BinaryFileHeader* pFileHeader) {
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


inline void StrmFileReader::Setup(const StrmFile::Header* pStrmBin) {
    if (!IsValidFileHeader(&pStrmBin->fileHeader)) {
        return;
    }

    mHeader = pStrmBin;

    mHeadBlock = static_cast<const StrmFile::HeadBlock*>(
        ut::AddOffsetToPtr(mHeader, mHeader->headBlockOffset));

    // Retail performs this dead ref resolution (debug leftover); keep it so
    // the instruction schedule matches.
    (void)Util::GetDataRefAddress0(
        mHeadBlock->refDataHeader,
        &mHeadBlock->refDataHeader);
}


// ---------------------------------------------------------------------------
// StrmFileReader / StrmFileLoader read methods
// ---------------------------------------------------------------------------

bool StrmFileReader::ReadStrmInfo(StrmInfo* pStrmInfo) const {
    const StrmFile::HeadBlock* pHead = mHeadBlock;
    // Pointer-local for the ref makes MWCC materialize the base address
    // before reading the ref fields (retail schedule).
    const Util::DataRef<StrmFile::StrmDataInfo>* pRef =
        &pHead->refDataHeader;
    const StrmFile::StrmDataInfo* pStrmData =
        Util::GetDataRefAddress0(*pRef, pRef);

    // File format tag -> runtime StrmInfo format id (SampleFormat).
    // Retail lowers this as a signed-range comparison BST.
    int format;
    switch (pStrmData->format) {
    case 2:
        format = 3;
        break;
    case 1:
        format = 1;
        break;
    case 0:
        format = 2;
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



bool StrmFileReader::ReadStrmTrackInfo(StrmTrackInfo* pTrackInfo,
                                       int trackNo) const {
    const StrmFile::TrackTable* pTrackTable = Util::GetDataRefAddress0(
        mHeadBlock->refTrackTable, &mHeadBlock->refDataHeader);

    if (trackNo >= pTrackTable->trackCount) {
        return false;
    }

    // TrackData tag byte selects between the two on-disk track-info layouts.
    switch (pTrackTable->trackDataType) {
    case 0: {
        const StrmFile::TrackInfo* pSrcInfo = Util::GetDataRefAddress0(
            pTrackTable->refTrackHeader[trackNo],
            &mHeadBlock->refDataHeader);

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
                                         &mHeadBlock->refDataHeader));

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

    if (!reader.IsValidFileHeader(&pHeader->fileHeader)) {
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

    mReader.Setup(static_cast<StrmFile::Header*>(pStrmBin));

    return true;
}



int StrmFileLoader::GetChannelCount() const {
    if (!mReader.IsAvailable()) {
        return 0;
    }

    const StrmFile::HeadBlock* pHead = mReader.mHeadBlock;
    const StrmFile::ChannelTable* pChannelTable = Util::GetDataRefAddress0(
        pHead->refChannelTable, &pHead->refDataHeader);

    return pChannelTable->channelCount;
}



bool StrmFileLoader::ReadStrmInfo(StrmFileReader::StrmInfo* pStrmInfo) const {
    if (!mReader.IsAvailable()) {
        return false;
    }

    mReader.ReadStrmInfo(pStrmInfo);
    return true;
}



bool StrmFileLoader::ReadStrmTrackInfo(StrmFileReader::StrmTrackInfo* pTrackInfo,
                                       int trackNo) const {
    if (!mReader.IsAvailable()) {
        return false;
    }

    mReader.ReadStrmTrackInfo(pTrackInfo, trackNo);
    return true;
}



bool StrmFileLoader::ReadAdpcmInfo(AdpcmParam* pParam,
                                   AdpcmLoopParam* pLoopParam,
                                   int channel) const {
    if (!mReader.IsAvailable()) {
        return false;
    }

    const StrmFile::StrmDataInfo* pStrmData = Util::GetDataRefAddress0(
        mReader.mHeadBlock->refDataHeader,
        &mReader.mHeadBlock->refDataHeader);

    // Both guard checks branch forward to the shared "return true" tail.
    if (pStrmData->format == WaveFile::FORMAT_ADPCM) {
        const StrmFile::ChannelTable* pChannelTable = Util::GetDataRefAddress0(
            mReader.mHeadBlock->refChannelTable,
            &mReader.mHeadBlock->refDataHeader);

        if (channel < pChannelTable->channelCount) {
            const StrmFile::ChannelInfo* pChannelInfo = Util::GetDataRefAddress0(
                pChannelTable->refChannelHeader[channel],
                &mReader.mHeadBlock->refDataHeader);

            const AdpcmInfo* pAdpcmData = Util::GetDataRefAddress0(
                pChannelInfo->refAdpcmInfo,
                &mReader.mHeadBlock->refDataHeader);

            *pParam = pAdpcmData->param;
            *pLoopParam = pAdpcmData->loopParam;
        }
    }
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


} // namespace detail
} // namespace snd
} // namespace nw4r