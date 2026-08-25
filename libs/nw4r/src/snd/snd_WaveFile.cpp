#include <nw4r/snd.h>

namespace nw4r {
namespace snd {
namespace detail {

// RWAV 'wave info' block that WaveArchiveReader::mFileStart refers to.
// Each table entry is a 12-byte DataRef (refType/dataType/reserved/value)
// plus padding.
namespace WaveFile {
struct WaveInfoFile { // 0xC
    Util::DataRef<const void> dataRef; // at 0x0 (8 bytes)
    u32 reserved2;                    // at 0x8
};

struct WaveInfoBlock {
    ut::BinaryBlockHeader blockHeader; // at 0x0
    u32 numWaves;                      // at 0x8
    WaveInfoFile table[1];             // at 0xC
};
} // namespace WaveFile

// Retail types GetSampleByDspAddress's format argument as nw4r::snd::Sample,
// Format (see the retail mangled name); alias that exact import here.
extern "C" u32
    GetSampleByDspAddress__Q44nw4r3snd6detail7AxVoiceFPCvUlQ34nw4r3snd12SampleFormat(
        const void* pBase, u32 addr, u32 fmt);

// Runtime wave-info layout that ReadWaveInfo fills. This is distinct from the
// on-disk detail::WaveInfo header that dwells in the file.
struct RuntimeChannelParam { // 0x34
    void* dataAddr;          // at 0x0
    AdpcmInfo adpcmInfo;     // at 0x4 (0x30 bytes)
};

struct RuntimeWaveInfo { // 0x18 + CHANNEL_MAX * 0x34
    u32 format;                               // at 0x0
    u8 loopFlag;                              // at 0x4
    u8 PADDING_0x5[3];                        // at 0x5
    u32 numChannels;                          // at 0x8
    u32 sampleRate;                           // at 0xC
    u32 loopStart;                            // at 0x10
    u32 loopEnd;                              // at 0x14
    RuntimeChannelParam channel[CHANNEL_MAX]; // at 0x18
};

WaveFileReader::WaveFileReader(const WaveInfo* pWaveInfo)
    : mWaveInfo(pWaveInfo) {}

WaveFileReader::WaveFileReader(const WaveFile::FileHeader* pFileHeader)
    : mWaveInfo(NULL) {
    // retail: add. ptr+offset; beqlr (null check folded into the pointer sum)
    const u8* p = (const u8*)((u32)pFileHeader->infoBlockOffset + (u32)pFileHeader);
    if (p != NULL) {
        mWaveInfo = (const WaveInfo*)(p + 8);
    }
}

bool WaveFileReader::ReadWaveInfo(WaveInfo* pWaveInfo,
                                   const void* pWaveAddr) const {
    // Convert the on-disk sample format into a runtime sample format.
    u32 format;

    const WaveInfo* pHeader = mWaveInfo;

    switch (pHeader->format) {
    case WaveFile::FORMAT_PCM16:
        format = SAMPLE_FORMAT_PCM_S16;
        break;
    case WaveFile::FORMAT_PCM8:
        format = SAMPLE_FORMAT_PCM_S8;
        break;
    case WaveFile::FORMAT_ADPCM:
    default:
        format = SAMPLE_FORMAT_DSP_ADPCM;
        break;
    }

    RuntimeWaveInfo* pInfo = reinterpret_cast<RuntimeWaveInfo*>(pWaveInfo);

    pInfo->format = format;
    pInfo->numChannels = pHeader->numChannels;
    pInfo->sampleRate =
        (pHeader->sampleRate24 << 16) + pHeader->sampleRate;
    pInfo->loopFlag = (pHeader->loopFlag != 0);

    pInfo->loopStart =
        GetSampleByDspAddress__Q44nw4r3snd6detail7AxVoiceFPCvUlQ34nw4r3snd12SampleFormat(
            NULL, pHeader->loopStart, format);
    pInfo->loopEnd =
        GetSampleByDspAddress__Q44nw4r3snd6detail7AxVoiceFPCvUlQ34nw4r3snd12SampleFormat(
            NULL, pHeader->loopEnd, format) +
        1;

    // pInfo's last use; the loop works through pChan below.
    RuntimeChannelParam* pChan = pInfo->channel;

    // Fill one channel (2 max) from the channel info table until all channels
    // in the header are accounted for.

    for (u32 i = 0; i < pHeader->numChannels; i++) {
        if (i < CHANNEL_MAX) {
            const u32* pInfoOffsetTable = reinterpret_cast<const u32*>(
                reinterpret_cast<const u8*>(pHeader) +
                pHeader->channelInfoTableOffset);

            const WaveFile::WaveChannelInfo* pChannelInfo =
                reinterpret_cast<const WaveFile::WaveChannelInfo*>(
                    reinterpret_cast<const u8*>(pHeader) +
                    pInfoOffsetTable[i]);

            if (pChannelInfo->adpcmOffset != 0) {
                // Retail quirk: only halfword 0 is read through the adpcm
                // offset (lhzux); the remaining 22 stream from pHeader+2.
                reinterpret_cast<u16*>(&pChan[i].adpcmInfo)[0] =
                    *reinterpret_cast<const u16*>(
                        reinterpret_cast<const u8*>(mWaveInfo) +
                        pChannelInfo->adpcmOffset);
                reinterpret_cast<u16*>(&pChan[i].adpcmInfo)[1] =
                    reinterpret_cast<const u16*>(mWaveInfo)[1];
                reinterpret_cast<u16*>(&pChan[i].adpcmInfo)[2] =
                    reinterpret_cast<const u16*>(mWaveInfo)[2];
                reinterpret_cast<u16*>(&pChan[i].adpcmInfo)[3] =
                    reinterpret_cast<const u16*>(mWaveInfo)[3];
                reinterpret_cast<u16*>(&pChan[i].adpcmInfo)[4] =
                    reinterpret_cast<const u16*>(mWaveInfo)[4];
                reinterpret_cast<u16*>(&pChan[i].adpcmInfo)[5] =
                    reinterpret_cast<const u16*>(mWaveInfo)[5];
                reinterpret_cast<u16*>(&pChan[i].adpcmInfo)[6] =
                    reinterpret_cast<const u16*>(mWaveInfo)[6];
                reinterpret_cast<u16*>(&pChan[i].adpcmInfo)[7] =
                    reinterpret_cast<const u16*>(mWaveInfo)[7];
                reinterpret_cast<u16*>(&pChan[i].adpcmInfo)[8] =
                    reinterpret_cast<const u16*>(mWaveInfo)[8];
                reinterpret_cast<u16*>(&pChan[i].adpcmInfo)[9] =
                    reinterpret_cast<const u16*>(mWaveInfo)[9];
                reinterpret_cast<u16*>(&pChan[i].adpcmInfo)[10] =
                    reinterpret_cast<const u16*>(mWaveInfo)[10];
                reinterpret_cast<u16*>(&pChan[i].adpcmInfo)[11] =
                    reinterpret_cast<const u16*>(mWaveInfo)[11];
                reinterpret_cast<u16*>(&pChan[i].adpcmInfo)[12] =
                    reinterpret_cast<const u16*>(mWaveInfo)[12];
                reinterpret_cast<u16*>(&pChan[i].adpcmInfo)[13] =
                    reinterpret_cast<const u16*>(mWaveInfo)[13];
                reinterpret_cast<u16*>(&pChan[i].adpcmInfo)[14] =
                    reinterpret_cast<const u16*>(mWaveInfo)[14];
                reinterpret_cast<u16*>(&pChan[i].adpcmInfo)[15] =
                    reinterpret_cast<const u16*>(mWaveInfo)[15];
                reinterpret_cast<u16*>(&pChan[i].adpcmInfo)[16] =
                    reinterpret_cast<const u16*>(mWaveInfo)[16];
                reinterpret_cast<u16*>(&pChan[i].adpcmInfo)[17] =
                    reinterpret_cast<const u16*>(mWaveInfo)[17];
                reinterpret_cast<u16*>(&pChan[i].adpcmInfo)[18] =
                    reinterpret_cast<const u16*>(mWaveInfo)[18];
                reinterpret_cast<u16*>(&pChan[i].adpcmInfo)[19] =
                    reinterpret_cast<const u16*>(mWaveInfo)[19];
                reinterpret_cast<u16*>(&pChan[i].adpcmInfo)[20] =
                    reinterpret_cast<const u16*>(mWaveInfo)[20];
                reinterpret_cast<u16*>(&pChan[i].adpcmInfo)[21] =
                    reinterpret_cast<const u16*>(mWaveInfo)[21];
                reinterpret_cast<u16*>(&pChan[i].adpcmInfo)[22] =
                    reinterpret_cast<const u16*>(mWaveInfo)[22];
            }

            pChan[i].dataAddr =
                GetWaveDataAddress(pChannelInfo, pWaveAddr);
        }
    }

    return true;
}

bool WaveFileReader::ReadWaveParam(WaveData* pWaveData,
                                   const void* pWaveAddr) const {
    AxVoice::Format format =
        GetAxVoiceFormatFromWaveFileFormat(mWaveInfo->format);

    pWaveData->sampleFormat = mWaveInfo->format;
    pWaveData->numChannels = mWaveInfo->numChannels;
    pWaveData->sampleRate =
        (mWaveInfo->sampleRate24 << 16) + mWaveInfo->sampleRate;

    pWaveData->loopFlag = mWaveInfo->loopFlag;
    pWaveData->loopStart =
        AxVoice::GetSampleByDspAddress(NULL, mWaveInfo->loopStart, format);
    pWaveData->loopEnd =
        AxVoice::GetSampleByDspAddress(NULL, mWaveInfo->loopEnd, format) + 1;

    const u32* pInfoOffsetTable = static_cast<const u32*>(
        ut::AddOffsetToPtr(mWaveInfo, mWaveInfo->channelInfoTableOffset));

    for (int i = 0; i < mWaveInfo->numChannels; i++) {
        ChannelParam& rParam = pWaveData->channelParam[i];

        const WaveFile::WaveChannelInfo* pChannelInfo =
            reinterpret_cast<const WaveFile::WaveChannelInfo*>(
                ut::AddOffsetToPtr(mWaveInfo, pInfoOffsetTable[i]));

        rParam.volumeFrontLeft = pChannelInfo->volumeFrontLeft;
        rParam.volumeFrontRight = pChannelInfo->volumeFrontRight;
        rParam.volumeRearLeft = pChannelInfo->volumeRearLeft;
        rParam.volumeRearRight = pChannelInfo->volumeRearRight;

        if (pChannelInfo->adpcmOffset != 0) {
            const AdpcmInfo* pAdpcmInfo = static_cast<const AdpcmInfo*>(
                ut::AddOffsetToPtr(mWaveInfo, pChannelInfo->adpcmOffset));

            rParam.adpcmInfo = *pAdpcmInfo;
        }

        rParam.dataAddr = const_cast<void*>(
            ut::AddOffsetToPtr(pWaveAddr, mWaveInfo->dataOffset +
                                              pChannelInfo->channelDataOffset));
    }

    return true;
}

void* WaveFileReader::GetWaveDataAddress(const WaveFile::WaveChannelInfo* info,
                                           const void* addr) const {
    const void* dataAddr = addr;
    bool addrNull = (addr == NULL);
    if (addr == NULL) {
        dataAddr = mWaveInfo;
    }

    const WaveInfo* wi = mWaveInfo;
    const u8* result;

    switch (wi->dataType) {
    case 0:
        result = reinterpret_cast<const u8*>(
            wi->dataOffset + reinterpret_cast<u32>(dataAddr));
        if (addrNull) {
            result += 8;
        }
        break;
    case 1:
        result = reinterpret_cast<const u8*>(wi->dataOffset);
        break;
    default:
        return NULL;
    }

    return reinterpret_cast<u8*>(
        info->channelDataOffset + reinterpret_cast<u32>(result));
}

AxVoice::Format WaveFileReader::GetAxVoiceFormatFromWaveFileFormat(u32 format) {
    if (format == WaveFile::FORMAT_PCM16) {
        return AxVoice::FORMAT_PCM16;
    }

    if (format == WaveFile::FORMAT_PCM8) {
        return AxVoice::FORMAT_PCM8;
    }

    return AxVoice::FORMAT_ADPCM;
}

WaveArchiveReader::WaveArchiveReader(const void* pData) {
    mFileStart = NULL;
    mWaveData = NULL;

    const u8* bytes = reinterpret_cast<const u8*>(pData);

    // FULL_MATCH (2026-08): the version check booleanizes ARITHMETICALLY.
    // The retail idiom subfic(256-v)/orc(256|~v)/srwi(1)/subf/srwi(31) is
    // MWCC's booleanization of `version <= 0x100` (constant-first operand
    // form) — `version >= 0x100` booleanizes to the mirrored addi(v-256)/
    // orc(v|~256) form. The if/else-if/else chain with the redundant
    // `ok = (version <= 0x100)` in the else keeps the arithmetic idiom AND
    // the retail's out-of-line block layout (magic beq / fail li r0,0 / b
    // shared cmpi; bge / fail / shared cmpi) under GC/3.0a5.2.
    u32 ok;
    if (*(reinterpret_cast<const u32*>(bytes)) != 0x52574152) {
        ok = 0;
    } else if (*(reinterpret_cast<const u16*>(bytes + 6)) < 0x100) {
        ok = 0;
    } else {
        ok = (*(reinterpret_cast<const u16*>(bytes + 6)) <= 0x100);
    }
    if (!ok) {
        return;
    }
    mWaveData = (const u8*)(*(reinterpret_cast<const u32*>(bytes + 0x18)) + (u32)bytes);
    mFileStart = (const u8*)(*(reinterpret_cast<const u32*>(bytes + 0x10)) + (u32)bytes);
}

const void* WaveArchiveReader::GetWaveFile(int index) const {
    if (mFileStart == NULL) {
        return NULL;
    }
    if (mWaveData == NULL) {
        return NULL;
    }
    if (index < 0) {
        return NULL;
    }

    const WaveFile::WaveInfoBlock* pBlock =
        static_cast<const WaveFile::WaveInfoBlock*>(mFileStart);

    if (static_cast<u32>(index) >= pBlock->numWaves) {
        return NULL;
    }

    const WaveFile::WaveInfoFile& fileInfo = pBlock->table[index];

    // Copy the DataRef into a local: this reproduces the retail stack spill.
    const Util::DataRef<const void> ref = fileInfo.dataRef;
    return Util::GetDataRefAddress0<const void>(ref, mWaveData);
}

} // namespace detail
} // namespace snd
} // namespace nw4r
