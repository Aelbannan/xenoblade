// The retail ctor takes a nested const WaveFile::WaveInfo*, which the shared
// snd_WaveFile.h header does not declare. Rename the header class out of the
// way during the include and re-declare WaveFileReader below with the retail
// overload, so every symbol mangles naturally from real member functions
// (no C-linkage stand-ins), the same approach as snd_BankFile.cpp.
#define WaveFileReader WaveFileReader_RetailHeader
#include <nw4r/snd.h>
#undef WaveFileReader

// Retail .sdata2 pool constant 1.0f, referenced by name so the SDA21
// float relocation matches the stripped retail object (PLAN.md §17.6).
extern const f32 lbl_eu_8066A100;

namespace {

} // namespace

namespace nw4r {
namespace snd {
namespace detail {

// Re-declared WaveFileReader with the retail nested WaveFile::WaveInfo ctor
// overload (the shared header only declares the detail::WaveInfo and
// WaveFile::FileHeader overloads). The reader only stores the pointer.
namespace WaveFile {
struct WaveInfo; // on-disk wave-info header; only the pointer passes through
} // namespace WaveFile

class WaveFileReader {
public:
    explicit WaveFileReader(const WaveInfo* pWaveInfo);
    explicit WaveFileReader(const WaveFile::FileHeader* pFileHeader);
    explicit WaveFileReader(const WaveFile::WaveInfo* pWaveInfo);

    bool ReadWaveInfo(WaveInfo* pWaveInfo, const void*
                      pWaveAddr) const;

private:
    const WaveInfo* mWaveInfo; // at 0x0
};

// Retail has no standalone IsValidFileHeader symbol: MWCC inlines it into
// the constructor, so the body is defined inline here to avoid emitting an
// extra out-of-line copy (same approach as snd_BankFile.cpp).
inline bool WsdFileReader::IsValidFileHeader(
    const void*
    pWsdBin) {
    const ut::BinaryFileHeader* pFileHeader =
        static_cast<const ut::BinaryFileHeader*>(pWsdBin);

    if (pFileHeader->signature != SIGNATURE) {
        return false;
    }

    if (pFileHeader->version < NW4R_VERSION(1, 0)) {
        return false;
    }

    if (pFileHeader->version > NW4R_VERSION(1, 3)) {
        return false;
    }

    return true;
}

WsdFileReader::WsdFileReader(const void* pWsdBin)
    : mHeader(NULL), mDataBlock(NULL), mWaveBlock(NULL) {
    if (!IsValidFileHeader(pWsdBin)) {
        return;
    }

    mHeader = static_cast<const WsdFile::Header*>(pWsdBin);

    if (mHeader->dataBlockOffset != 0) {
        mDataBlock = static_cast<const WsdFile::DataBlock*>(
            ut::AddOffsetToPtr(mHeader, mHeader->dataBlockOffset));
    }

    if (mHeader->waveBlockOffset != 0) {
        mWaveBlock = static_cast<const WsdFile::WaveBlock*>(
            ut::AddOffsetToPtr(mHeader, mHeader->waveBlockOffset));
    }
}

bool WsdFileReader::ReadWaveInfo(int id, WaveInfo* pWaveInfo,
                                 const void* pWaveAddr) const {
    const WaveFile::WaveInfo* pWaveIn;

    if (mWaveBlock == NULL) {
        WaveArchiveReader archive(pWaveAddr);
        const void* waveFile = archive.GetWaveFile(id);

        if (waveFile == NULL) {
            return false;
        }

        WaveFileReader reader(static_cast<const WaveFile::FileHeader*>(waveFile));
        return reader.ReadWaveInfo(pWaveInfo, NULL);
    }

    // Use the embedded wave block.
    if (mHeader->fileHeader.version >= NW4R_VERSION(1, 1)) {
        if (id >= mWaveBlock->waveCount) {
            return false;
        }

        pWaveIn = static_cast<const WaveFile::WaveInfo*>(ut::AddOffsetToPtr(
            mWaveBlock, mWaveBlock->offsetTable[id]));
    } else {
        const WsdFile::WaveBlockOld* pWaveBlockOld =
            reinterpret_cast<const WsdFile::WaveBlockOld*>(mWaveBlock);

        pWaveIn = static_cast<const WaveFile::WaveInfo*>(ut::AddOffsetToPtr(
            pWaveBlockOld, pWaveBlockOld->offsetTable[id]));
    }

    WaveFileReader reader(pWaveIn);
    return reader.ReadWaveInfo(pWaveInfo, pWaveAddr);
}

bool WsdFileReader::ReadWaveSoundInfo(WaveSoundInfo* pSoundInfo, int id) const {
    const WsdFile::Wsd* pWsd =
        Util::GetDataRefAddress0(mDataBlock->refWsd[id], &mDataBlock->wsdCount);

    const WsdFile::WsdInfo* pWsdInfo =
        Util::GetDataRefAddress0(pWsd->refWsdInfo, &mDataBlock->wsdCount);

    if (mHeader->fileHeader.version >= NW4R_VERSION(1, 2)) {
        pSoundInfo->pitch = pWsdInfo->pitch;
        pSoundInfo->pan = pWsdInfo->pan;
        pSoundInfo->surroundPan = pWsdInfo->surroundPan;
        pSoundInfo->fxSendA = pWsdInfo->fxSendA;
        pSoundInfo->fxSendB = pWsdInfo->fxSendB;
        pSoundInfo->fxSendC = pWsdInfo->fxSendC;
        pSoundInfo->mainSend = pWsdInfo->mainSend;
    } else if (mHeader->fileHeader.version >= NW4R_VERSION(1, 1)) {
        pSoundInfo->pitch = pWsdInfo->pitch;
        pSoundInfo->pan = pWsdInfo->pan;
        pSoundInfo->surroundPan = pWsdInfo->surroundPan;
        pSoundInfo->fxSendA = 0;
        pSoundInfo->fxSendB = 0;
        pSoundInfo->fxSendC = 0;
        pSoundInfo->mainSend = 127;
    } else {
        pSoundInfo->pitch = lbl_eu_8066A100;
        pSoundInfo->pan = 64;
        pSoundInfo->surroundPan = 0;
        pSoundInfo->fxSendA = 0;
        pSoundInfo->fxSendB = 0;
        pSoundInfo->fxSendC = 0;
        pSoundInfo->mainSend = 127;
    }

    return true;
}

bool WsdFileReader::ReadWaveSoundNoteInfo(WaveSoundNoteInfo* pSoundNoteInfo,
                                          int id, int note) const {
    const WsdFile::Wsd* pWsd =
        Util::GetDataRefAddress0(mDataBlock->refWsd[id], &mDataBlock->wsdCount);

    const WsdFile::NoteTable* pTable =
        Util::GetDataRefAddress0(pWsd->refNoteTable, &mDataBlock->wsdCount);

    const WsdFile::NoteInfo* pNoteInfo =
        Util::GetDataRefAddress0(pTable->items[note], &mDataBlock->wsdCount);

    pSoundNoteInfo->waveIndex = pNoteInfo->waveIndex;
    pSoundNoteInfo->attack = pNoteInfo->attack;
    pSoundNoteInfo->hold = pNoteInfo->hold;
    pSoundNoteInfo->decay = pNoteInfo->decay;
    pSoundNoteInfo->sustain = pNoteInfo->sustain;
    pSoundNoteInfo->release = pNoteInfo->release;
    pSoundNoteInfo->originalKey = pNoteInfo->originalKey;
    pSoundNoteInfo->volume = pNoteInfo->volume;

    if (mHeader->fileHeader.version >= NW4R_VERSION(1, 1)) {
        pSoundNoteInfo->pan = pNoteInfo->pan;
        pSoundNoteInfo->surroundPan = pNoteInfo->surroundPan;
        pSoundNoteInfo->pitch = pNoteInfo->pitch;
    } else {
        pSoundNoteInfo->pan = 64;
        pSoundNoteInfo->surroundPan = 0;
        pSoundNoteInfo->pitch = lbl_eu_8066A100;
    }

    return true;
}

} // namespace detail
} // namespace snd
} // namespace nw4r

