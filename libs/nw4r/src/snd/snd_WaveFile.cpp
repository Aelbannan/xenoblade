#include <nw4r/snd.h>

namespace nw4r {
namespace snd {
namespace detail {

WaveFileReader::WaveFileReader(const WaveFile::WaveInfo* pWaveInfo)
    : mWaveInfo(pWaveInfo) {}

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
    if (dataAddr == NULL) {
        dataAddr = mWaveInfo;
    }
    
    const WaveFile::WaveInfo* wi = mWaveInfo;
    u32 dtype = wi->dataType;
    const u8* result;
    
    if (dtype == 0) {
        result = reinterpret_cast<const u8*>(dataAddr) + wi->dataOffset;
        if (addr == NULL) {
            result += 8;
        }
    } else if (dtype == 1) {
        result = reinterpret_cast<const u8*>(dataAddr) + wi->dataOffset;
    } else {
        return NULL;
    }
    
    return const_cast<u8*>(result + info->channelDataOffset);
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
    
    u32 magic = reinterpret_cast<const u32*>(bytes)[0];
    if (magic != 0x52574152) { // 'RWAR'
        return;
    }
    
    u16 version = reinterpret_cast<const u16*>(bytes + 6)[0];
    if (version < 0x100) {
        return;
    }
    
    u32 offset1 = reinterpret_cast<const u32*>(bytes + 0x10)[0];
    u32 offset2 = reinterpret_cast<const u32*>(bytes + 0x18)[0];
    
    mFileStart = bytes + offset1;
    mWaveData = bytes + offset2;
}

} // namespace detail
} // namespace snd
} // namespace nw4r
