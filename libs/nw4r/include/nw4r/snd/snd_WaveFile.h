#ifndef NW4R_SND_WAVE_FILE_H
#define NW4R_SND_WAVE_FILE_H
#include <nw4r/types_nw4r.h>

#include <nw4r/snd/snd_AxVoice.h>
#include <nw4r/snd/snd_FxBase.h>
#include <nw4r/snd/snd_Types.h>

#include <nw4r/ut.h>

namespace nw4r {
namespace snd {
namespace detail {

/******************************************************************************
 *
 * RWAV binary layout
 *
 ******************************************************************************/
namespace WaveFile {

enum Format { FORMAT_PCM8, FORMAT_PCM16, FORMAT_ADPCM };

struct FileHeader {
    ut::BinaryFileHeader fileHeader; // at 0x0
    u32 infoBlockOffset;              // at 0x10
    u32 infoBlockSize;                // at 0x14
    u32 dataBlockOffset;              // at 0x18
    u32 dataBlockSize;                // at 0x1C
};

// Retail nests WaveChannelInfo inside WaveFile (mangled
// Q54nw4r3snd6detail8WaveFile15WaveChannelInfo).
struct WaveChannelInfo {
    u32 channelDataOffset; // at 0x0
    u32 adpcmOffset;       // at 0x4
    u32 volumeFrontLeft;   // at 0x8
    u32 volumeFrontRight;  // at 0xC
    u32 volumeRearLeft;    // at 0x10
    u32 volumeRearRight;   // at 0x14
    u32 reserved;          // at 0x18
};

// On-disk RWAV wave-info header.
struct WaveInfo {
    u8 format;                  // at 0x0
    u8 loopFlag;                // at 0x1
    u8 numChannels;             // at 0x2
    u8 sampleRate24;            // at 0x3
    u16 sampleRate;             // at 0x4
    u8 dataType;                // at 0x6
    u8 PADDING_0x7;             // at 0x7
    u32 loopStart;              // at 0x8
    u32 loopEnd;                // at 0xC
    u32 channelInfoTableOffset; // at 0x10
    u32 dataOffset;             // at 0x14
    u32 reserved;               // at 0x18
};

}; // namespace WaveFile

/******************************************************************************
 *
 * WaveInfo (runtime wave description)
 *
 ******************************************************************************/
struct ChannelParam {
    void* dataAddr;      // at 0x0
    AdpcmInfo adpcmInfo; // at 0x4
};

struct WaveInfo {
    SampleFormat sampleFormat;              // at 0x0
    bool loopFlag;                          // at 0x4
    int numChannels;                        // at 0x8
    int sampleRate;                         // at 0xC
    u32 loopStart;                          // at 0x10
    u32 loopEnd;                            // at 0x14
    ChannelParam channelParam[CHANNEL_MAX]; // at 0x18
};

/******************************************************************************
 *
 * WaveFileReader
 *
 ******************************************************************************/
class WaveFileReader {
public:
    explicit WaveFileReader(const WaveFile::FileHeader* pFileHeader);
    explicit WaveFileReader(const WaveFile::WaveInfo* pWaveInfo);

    bool ReadWaveInfo(WaveInfo* pWaveInfo, const void* pWaveAddr) const;
    void* GetWaveDataAddress(const WaveFile::WaveChannelInfo* pInfo,
                             const void* pWaveAddr) const;

private:
    const WaveFile::WaveInfo* mWaveInfo; // at 0x0
};

class WaveArchiveReader {
public:
    explicit WaveArchiveReader(const void* pData);
    const void* GetWaveFile(int index) const;

private:
    const void* mFileStart; // at 0x0
    const void* mWaveData;  // at 0x4
};

inline AxVoice::Format WaveFormatToAxFormat(u32 format) {
    if (format == WaveFile::FORMAT_PCM16) {
        return AxVoice::FORMAT_PCM16;
    }

    if (format == WaveFile::FORMAT_PCM8) {
        return AxVoice::FORMAT_PCM8;
    }

    return AxVoice::FORMAT_ADPCM;
}

} // namespace detail
} // namespace snd
} // namespace nw4r

#endif
