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
    const WaveInfo* pHeader = mWaveInfo;
    RuntimeWaveInfo* pInfo = reinterpret_cast<RuntimeWaveInfo*>(pWaveInfo);

    // Convert the on-disk sample format into a runtime sample format.
    int format;
    switch (pHeader->format) {
    case WaveFile::FORMAT_PCM16:
        format = SAMPLE_FORMAT_PCM_S16;
        break;
    case WaveFile::FORMAT_PCM8:
        format = SAMPLE_FORMAT_PCM_S8;
        break;
    case WaveFile::FORMAT_ADPCM:
        format = SAMPLE_FORMAT_DSP_ADPCM;
        break;
    default:
        format = SAMPLE_FORMAT_DSP_ADPCM;
        break;
    }

    pInfo->format = format;
    pInfo->numChannels = pHeader->numChannels;
    pInfo->sampleRate = (pHeader->sampleRate24 << 16) + pHeader->sampleRate;
    pInfo->loopFlag = (pHeader->loopFlag != 0);

    pInfo->loopStart = AxVoice::GetSampleByDspAddress(
        NULL, pHeader->loopStart, static_cast<AxVoice::Format>(format));
    pInfo->loopEnd = AxVoice::GetSampleByDspAddress(
        NULL, pHeader->loopEnd, static_cast<AxVoice::Format>(format)) +
                     1;

    const u32* pInfoOffsetTable = reinterpret_cast<const u32*>(
        reinterpret_cast<const u8*>(pHeader) +
        pHeader->channelInfoTableOffset);

    // Fill one channel (2 max) from the channel info table until all channels
    // in the header are accounted for.
    for (u32 i = 0; i < pHeader->numChannels; i++) {
        if (i < CHANNEL_MAX) {
            const WaveFile::WaveChannelInfo* pChannelInfo =
                reinterpret_cast<const WaveFile::WaveChannelInfo*>(
                    reinterpret_cast<const u8*>(pHeader) +
                    pInfoOffsetTable[i]);

            if (pChannelInfo->adpcmOffset != 0) {
                const AdpcmInfo* pAdpcm = reinterpret_cast<const AdpcmInfo*>(
                    reinterpret_cast<const u8*>(pHeader) +
                    pChannelInfo->adpcmOffset);

                pInfo->channel[i].adpcmInfo.param.coef[0] =
                    pAdpcm->param.coef[0];
                pInfo->channel[i].adpcmInfo.param.coef[1] =
                    pAdpcm->param.coef[1];
                pInfo->channel[i].adpcmInfo.param.coef[2] =
                    pAdpcm->param.coef[2];
                pInfo->channel[i].adpcmInfo.param.coef[3] =
                    pAdpcm->param.coef[3];
                pInfo->channel[i].adpcmInfo.param.coef[4] =
                    pAdpcm->param.coef[4];
                pInfo->channel[i].adpcmInfo.param.coef[5] =
                    pAdpcm->param.coef[5];
                pInfo->channel[i].adpcmInfo.param.coef[6] =
                    pAdpcm->param.coef[6];
                pInfo->channel[i].adpcmInfo.param.coef[7] =
                    pAdpcm->param.coef[7];
                pInfo->channel[i].adpcmInfo.param.coef[8] =
                    pAdpcm->param.coef[8];
                pInfo->channel[i].adpcmInfo.param.coef[9] =
                    pAdpcm->param.coef[9];
                pInfo->channel[i].adpcmInfo.param.coef[10] =
                    pAdpcm->param.coef[10];
                pInfo->channel[i].adpcmInfo.param.coef[11] =
                    pAdpcm->param.coef[11];
                pInfo->channel[i].adpcmInfo.param.coef[12] =
                    pAdpcm->param.coef[12];
                pInfo->channel[i].adpcmInfo.param.coef[13] =
                    pAdpcm->param.coef[13];
                pInfo->channel[i].adpcmInfo.param.coef[14] =
                    pAdpcm->param.coef[14];
                pInfo->channel[i].adpcmInfo.param.coef[15] =
                    pAdpcm->param.coef[15];
                pInfo->channel[i].adpcmInfo.param.gain = pAdpcm->param.gain;
                pInfo->channel[i].adpcmInfo.param.pred_scale =
                    pAdpcm->param.pred_scale;
                pInfo->channel[i].adpcmInfo.param.yn1 = pAdpcm->param.yn1;
                pInfo->channel[i].adpcmInfo.param.yn2 = pAdpcm->param.yn2;
                pInfo->channel[i].adpcmInfo.loopParam.loop_pred_scale =
                    pAdpcm->loopParam.loop_pred_scale;
                pInfo->channel[i].adpcmInfo.loopParam.loop_yn1 =
                    pAdpcm->loopParam.loop_yn1;
                pInfo->channel[i].adpcmInfo.loopParam.loop_yn2 =
                    pAdpcm->loopParam.loop_yn2;
            }

            pInfo->channel[i].dataAddr =
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
