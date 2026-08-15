// The retail RBNK reader is reconstructed here as real C++ members. The
// shared snd_BankFile.h / snd_WaveFile.h headers cannot be extended from
// this unit (the retail GetInstParam / ReadWaveInfo members and the
// WaveFileReader(WaveFile::WaveInfo*) ctor overload are not declared there),
// so the three classes that need the extra retail members are renamed out of
// the way during the header include and re-declared below with the complete
// retail member sets. Every retail symbol then mangles naturally from real
// member functions - no C-linkage stand-ins.
//
// The mirror layouts below (InstParamData / InstInfoData / RuntimeWaveInfo)
// expose the exact on-disk offsets the reader touches; the header InstParam
// packs bytes 0x8..0xb into one padded array and the header InstInfo is a
// smaller public-only structure.
#define BankFileReader BankFileReader_RetailHeader
#define InstInfo InstInfo_RetailHeader
#define WaveFileReader WaveFileReader_RetailHeader
#include <nw4r/snd.h>
#include <nw4r/ut.h>
#undef BankFileReader
#undef InstInfo
#undef WaveFileReader

// Retail .sdata2 pool constant 1.0f referenced by ReadInstInfo's legacy
// (pre-1.1) path; referenced by name so the SDA21 float reloc matches the
// stripped retail object.
extern const f32 lbl_eu_80669ED8;

namespace nw4r {
namespace snd {
namespace detail {

// Re-declared InstInfo with the retail nested WaveDataLocation record (the
// shared header's InstInfo is a smaller public-only structure). The reader
// writes the runtime layout through InstInfoData below; this type only
// carries the retail mangled name.
struct InstInfo {
    // On-disk wave-data location record read by ReadWaveInfo. Signed so the
    // reader's location dispatch lowers with the retail signed compares.
    struct WaveDataLocation {
        s32 location;  // at 0x0
        s32 waveIndex; // at 0x4
    };

    s32 waveIndex;  // at 0x0
    u8 attack;      // at 0x4
    u8 decay;       // at 0x5
    u8 sustain;     // at 0x6
    u8 release;     // at 0x7
    u8 originalKey; // at 0x8
    u8 pan;         // at 0x9
    u8 volume;      // at 0xA
    f32 tune;       // at 0xC
};

// On-disk RWAV wave-info entry (retail nested WaveFile::WaveInfo). Only the
// pointer passes through this unit; the layout mirrors the on-disk header.
namespace WaveFile {
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
} // namespace WaveFile

// Re-declared WaveFileReader with the retail WaveFile::WaveInfo ctor
// overload (the shared header only has the runtime WaveInfo / FileHeader
// overloads). The reader only stores the pointer.
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

// Re-declared BankFileReader with the full retail member set (GetInstParam /
// ReadWaveInfo are not in the shared header).
class BankFileReader {
public:
    static const u32 SIGNATURE = FOURCC('R', 'B', 'N', 'K');
    static const int VERSION = NW4R_VERSION(1, 2);

public:
    explicit BankFileReader(const void*
                            pBankBin);

    bool IsValidFileHeader(const ut::BinaryFileHeader* pFileHeader);

    bool ReadInstInfo(InstInfo* pInfo, int prgNo, int key,
                      int velocity) const;

    const BankFile::InstParam* GetInstParam(int prgNo, int key,
                                            int velocity) const;
    bool ReadWaveInfo(WaveInfo* pWaveInfo,
                      const InstInfo::WaveDataLocation& location,
                      const void*
                          pWaveData,
                      const WaveInfo** ppWaveInfo) const;

private:
    const BankFile::DataRegion*
    GetReferenceToSubRegion(const BankFile::DataRegion* pRef,
                            int splitKey) const;

    const BankFile::Header* mHeader;       // at 0x0
    const BankFile::DataBlock* mDataBlock; // at 0x4
    const BankFile::WaveBlock* mWaveBlock; // at 0x8
};

// On-disk InstParam layout as the reader touches it (the header InstParam
// packs bytes 0x8..0xb into one padded array).
struct InstParamData {
    s32 waveIndex;     // at 0x0
    u8 attack;         // at 0x4
    u8 decay;          // at 0x5
    u8 sustain;        // at 0x6
    u8 release;        // at 0x7
    u8 field_0x8;      // at 0x8
    u8 waveIndexRange; // at 0x9
    u8 field_0xa;      // at 0xA
    u8 field_0xb;      // at 0xB
    u8 originalKey;    // at 0xC
    u8 volume;         // at 0xD
    u8 pan;            // at 0xE
    u8 field_0xf;      // at 0xF
    f32 tune;          // at 0x10
};

// Runtime InstInfo layout ReadInstInfo fills (the header InstInfo is a
// smaller public-only structure).
struct InstInfoData {
    s32 waveIndexType; // at 0x0
    s32 waveIndex;     // at 0x4
    u8 attack;         // at 0x8
    u8 field_0x9;      // at 0x9
    u8 decay;          // at 0xA
    u8 sustain;        // at 0xB
    u8 release;        // at 0xC
    u8 field_0xd;      // at 0xD
    u8 field_0xe;      // at 0xE
    u8 field_0xf;      // at 0xF
    s32 field_0x10;    // at 0x10
    u8 field_0x14;     // at 0x14
    u8 originalKey;    // at 0x15
    u8 pan;            // at 0x16
    u8 volume;         // at 0x17
    f32 tune;          // at 0x18
};

// Mirror of the runtime detail::WaveInfo layout (0x80 bytes) that
// BankFileReader::ReadWaveInfo copies in bulk; the header's detail::WaveInfo
// is the smaller on-disk layout. Same shape as snd_WaveFile.cpp's
// RuntimeWaveInfo so the 0x18-prelude + 13x8 copy loop reproduces. The pad
// bytes at 0x5..0x8 are left as implicit alignment padding so MWCC skips
// them during the struct copy (an explicit member would be copied byte-wise).
struct RuntimeWaveInfo { // 0x80
    u32 format;       // at 0x0
    u8 loopFlag;      // at 0x4
    u32 numChannels;  // at 0x8
    u32 sampleRate;   // at 0xC
    u32 loopStart;    // at 0x10
    u32 loopEnd;      // at 0x14
    u32 tail[0x1A];   // at 0x18 (channel params, 13x8 word copy loop)
};

// External wave-data source behind WaveDataLocation::location == 2: retail
// calls a no-arg virtual at vtable slot 2 (offset 8) that returns the
// runtime WaveInfo. The concrete retail type lives outside this unit; this
// reproduces the call shape only (MWCC reserves two leading vtable slots,
// so a single declared virtual lands at slot 2).
struct WaveDataProvider {
    virtual const WaveInfo* GetWaveInfo() const = 0;
};

// See BankFile::Region
enum {
    DATATYPE_NONE = Util::DATATYPE_T0,
    DATATYPE_INSTPARAM = Util::DATATYPE_T1,
    DATATYPE_RANGETABLE = Util::DATATYPE_T2,
    DATATYPE_INDEXTABLE = Util::DATATYPE_T3,
};

// Inline so MWCC folds the checks into the ctor and emits no out-of-line
// body: the retail unit has no standalone IsValidFileHeader symbol, so the
// unit's .text stays exactly at the retail split.
inline bool BankFileReader::IsValidFileHeader(
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

BankFileReader::BankFileReader(const void*
                               pBankBin)
    : mHeader(NULL), mDataBlock(NULL), mWaveBlock(NULL) {
    if (!IsValidFileHeader(
            static_cast<const ut::BinaryFileHeader*>(pBankBin))) {
        return;
    }

    mHeader = static_cast<const BankFile::Header*>(pBankBin);

    if (mHeader->dataBlockOffset != 0) {
        mDataBlock = static_cast<const BankFile::DataBlock*>(
            ut::AddOffsetToPtr(mHeader, mHeader->dataBlockOffset));
    }

    if (mHeader->waveBlockOffset != 0) {
        mWaveBlock = static_cast<const BankFile::WaveBlock*>(
            ut::AddOffsetToPtr(mHeader, mHeader->waveBlockOffset));
    }
}

// GetInstParam(int, int, int): walk the program/key/velocity DataRegion chain
// down to the inst-param record, or NULL.
const BankFile::InstParam* BankFileReader::GetInstParam(int prgNo, int key,
                                                        int velocity) const {
    if (mHeader == NULL) {
        return NULL;
    }

    if (prgNo < 0 || prgNo >= static_cast<int>(mDataBlock->instTable.count)) {
        return NULL;
    }

    const BankFile::DataRegion* pRef = &mDataBlock->instTable.items[prgNo];
    if (pRef->dataType == Util::DATATYPE_INVALID) {
        return NULL;
    }

    if (pRef->dataType != DATATYPE_INSTPARAM) {
        pRef = GetReferenceToSubRegion(pRef, key);
        if (pRef == NULL) {
            return NULL;
        }
    }

    if (pRef->dataType == Util::DATATYPE_INVALID) {
        return NULL;
    }

    if (pRef->dataType != DATATYPE_INSTPARAM) {
        pRef = GetReferenceToSubRegion(pRef, velocity);
        if (pRef == NULL) {
            return NULL;
        }
    }

    if (pRef->dataType != DATATYPE_INSTPARAM) {
        return NULL;
    }

    return Util::GetDataRefAddress1(*pRef, &mDataBlock->instTable);
}

bool BankFileReader::ReadInstInfo(InstInfo* pInfo, int prgNo, int key,
                                  int velocity) const {
    const InstParamData* pData =
        reinterpret_cast<const InstParamData*>(GetInstParam(prgNo, key, velocity));
    if (pData == NULL) {
        return false;
    }

    InstInfoData* pOut = reinterpret_cast<InstInfoData*>(pInfo);

    // Wave-index resolution: linear equality chain (retail lowers this as
    // cmpwi/cmplwi/cmplwi tests, not as a range-checked switch).
    if (pData->waveIndexRange == 0) {
        if (pData->waveIndex < 0) {
            return false;
        }
        pOut->waveIndexType = 0;
        pOut->waveIndex = pData->waveIndex;
    } else if (pData->waveIndexRange == 1) {
        if (pData->waveIndex == 0) {
            return false;
        }
        pOut->waveIndexType = 1;
        pOut->waveIndex = pData->waveIndex;
    } else if (pData->waveIndexRange == 2) {
        if (pData->waveIndex == 0) {
            return false;
        }
        pOut->waveIndexType = 2;
        pOut->waveIndex = pData->waveIndex;
    } else {
        return false;
    }

    pOut->attack = pData->attack;
    pOut->field_0x9 = pData->field_0x8;
    pOut->decay = pData->decay;
    pOut->sustain = pData->sustain;
    pOut->release = pData->release;
    pOut->originalKey = pData->originalKey;
    pOut->pan = pData->pan;

    if (mHeader->fileHeader.version >= NW4R_VERSION(1, 1)) {
        pOut->volume = pData->volume;
        pOut->tune = pData->tune;
    } else {
        pOut->volume = 127;
        pOut->tune = lbl_eu_80669ED8;
    }

    switch (pData->field_0xa) {
    case 0:
        pOut->field_0x10 = 0;
        break;

    case 1:
        pOut->field_0x10 = 1;
        break;

    default:
        return false;
    }

    pOut->field_0x14 = pData->field_0xb;
    return true;
}

const BankFile::DataRegion*
BankFileReader::GetReferenceToSubRegion(const BankFile::DataRegion* pRef,
                                        int splitKey) const {
    const BankFile::DataRegion* pSub = NULL;

    switch (pRef->dataType) {
    case DATATYPE_NONE: {
        break;
    }

    case DATATYPE_INSTPARAM: {
        pSub = pRef;
        break;
    }

    case DATATYPE_RANGETABLE: {
        const BankFile::RangeTable* pRangeTable =
            Util::GetDataRefAddress2(*pRef, &mDataBlock->instTable);

        if (pRangeTable == NULL) {
            return NULL;
        }

        int i = 0;
        while (splitKey > ReadByte(pRangeTable->key + i)) {
            if (++i >= pRangeTable->tableSize) {
                return NULL;
            }
        }

        const u8* pBase = reinterpret_cast<const u8*>(pRangeTable);
        u32 refOffset = i * sizeof(BankFile::DataRegion);
        u32 refStart = ut::RoundUp<u32>(pRangeTable->tableSize + 1, 4);

        pSub = reinterpret_cast<const BankFile::DataRegion*>(pBase + refOffset +
                                                             refStart);
        break;
    }

    case DATATYPE_INDEXTABLE: {
        const BankFile::IndexTable* pIndexTable =
            Util::GetDataRefAddress3(*pRef, &mDataBlock->instTable);

        if (pIndexTable == NULL) {
            return NULL;
        }

        if (splitKey < pIndexTable->min || splitKey > pIndexTable->max) {
            return NULL;
        }

        pSub = reinterpret_cast<const BankFile::DataRegion*>(
            pIndexTable->ref +
            (splitKey - pIndexTable->min) * sizeof(BankFile::DataRegion));
        break;
    }
    }

    return pSub;
}

// ReadWaveInfo(WaveInfo*, const InstInfo::WaveDataLocation&, const void*,
//              const WaveInfo**): resolve wave data by location kind
// (0 = bank wave archive / RWAV, 1 = direct memory pointer, 2 = external
// provider object) and copy the runtime WaveInfo out.
bool BankFileReader::ReadWaveInfo(
    WaveInfo* pWaveInfo, const InstInfo::WaveDataLocation& location,
    const void*
        pWaveData,
    const WaveInfo** ppWaveInfo) const {
    if (ppWaveInfo != NULL) {
        *ppWaveInfo = NULL;
    }

    if (mHeader == NULL) {
        return false;
    }

    // Linear location dispatch (retail lowers as cmpwi/cmpwi/cmpwi equality
    // tests, not a range-checked switch). Case 0 hoists waveIndex into a
    // callee-saved register before the archive reader calls (retail: lwz
    // r30,4(r5) ahead of the mWaveBlock branch).
    if (location.location == 0) {
        u32 waveIndex = location.waveIndex;

        if (mWaveBlock == NULL) {
            WaveArchiveReader archiveReader(pWaveData);
            const WaveFile::FileHeader* pFile =
                static_cast<const WaveFile::FileHeader*>(
                    archiveReader.GetWaveFile(waveIndex));
            if (pFile == NULL) {
                return false;
            }

            WaveFileReader wfReader(pFile);
            return wfReader.ReadWaveInfo(pWaveInfo, NULL);
        }

        if (waveIndex >= mWaveBlock->waveInfoTable.count) {
            return false;
        }

        const WaveInfo* pInfo = Util::GetDataRefAddress0(
            mWaveBlock->waveInfoTable.items[waveIndex],
            &mWaveBlock->waveInfoTable);
        if (pInfo == NULL) {
            return false;
        }

        // Retail constructs the reader with the on-disk WaveFile::WaveInfo
        // pointer (the const WaveFile::WaveInfo* ctor overload).
        WaveFileReader wfReader(reinterpret_cast<const WaveFile::WaveInfo*>(pInfo));
        return wfReader.ReadWaveInfo(pWaveInfo, pWaveData);
    } else if (location.location == 1) {
        if (location.waveIndex == 0) {
            return false;
        }

        if (ppWaveInfo != NULL) {
            *ppWaveInfo = reinterpret_cast<const WaveInfo*>(location.waveIndex);
        }

        *reinterpret_cast<RuntimeWaveInfo*>(pWaveInfo) =
            *reinterpret_cast<const RuntimeWaveInfo*>(location.waveIndex);
        return true;
    } else if (location.location == 2) {
        if (location.waveIndex == 0) {
            return false;
        }

        const WaveInfo* pInfo =
            reinterpret_cast<const WaveDataProvider*>(location.waveIndex)
                ->GetWaveInfo();
        if (pInfo == NULL) {
            return false;
        }

        if (ppWaveInfo != NULL) {
            *ppWaveInfo = pInfo;
        }

        *reinterpret_cast<RuntimeWaveInfo*>(pWaveInfo) =
            *reinterpret_cast<const RuntimeWaveInfo*>(pInfo);
        return true;
    } else {
        return false;
    }

    return false;
}

} // namespace detail
} // namespace snd
} // namespace nw4r
