#include <nw4r/snd.h>
#include <nw4r/ut.h>

// Retail .sdata2 pool constant 1.0f referenced by ReadInstInfo's legacy
// (pre-1.1) path; referenced by name so the SDA21 float reloc matches the
// stripped retail object.

namespace nw4r {
namespace snd {
namespace detail {

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
    : mHeader(nullptr), mDataBlock(nullptr), mWaveBlock(nullptr) {
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
// down to the inst-param record, or nullptr.
const BankFile::InstParam* BankFileReader::GetInstParam(int prgNo, int key,
                                                        int velocity) const {
    if (mHeader == nullptr) {
        return nullptr;
    }

    if (prgNo < 0 || prgNo >= static_cast<int>(mDataBlock->instTable.count)) {
        return nullptr;
    }

    const BankFile::DataRegion* pRef = &mDataBlock->instTable.items[prgNo];
    if (pRef->dataType == Util::DATATYPE_INVALID) {
        return nullptr;
    }

    if (pRef->dataType != DATATYPE_INSTPARAM) {
        pRef = GetReferenceToSubRegion(pRef, key);
        if (pRef == nullptr) {
            return nullptr;
        }
    }

    if (pRef->dataType == Util::DATATYPE_INVALID) {
        return nullptr;
    }

    if (pRef->dataType != DATATYPE_INSTPARAM) {
        pRef = GetReferenceToSubRegion(pRef, velocity);
        if (pRef == nullptr) {
            return nullptr;
        }
    }

    if (pRef->dataType != DATATYPE_INSTPARAM) {
        return nullptr;
    }

    return Util::GetDataRefAddress1(*pRef, &mDataBlock->instTable);
}

bool BankFileReader::ReadInstInfo(InstInfo* pInfo, int prgNo, int key,
                                  int velocity) const {
    const InstParamData* pData =
        reinterpret_cast<const InstParamData*>(GetInstParam(prgNo, key, velocity));
    if (pData == nullptr) {
        return false;
    }

    InstInfo* pOut = pInfo;

    // Wave-index resolution: linear equality chain (retail lowers this as
    // cmpwi/cmplwi/cmplwi tests, not as a range-checked switch).
    if (pData->waveIndexRange == 0) {
        if (pData->waveIndex < 0) {
            return false;
        }
        pOut->waveDataLocation.location = 0;
        pOut->waveDataLocation.waveIndex = pData->waveIndex;
    } else if (pData->waveIndexRange == 1) {
        if (pData->waveIndex == 0) {
            return false;
        }
        pOut->waveDataLocation.location = 1;
        pOut->waveDataLocation.waveIndex = pData->waveIndex;
    } else if (pData->waveIndexRange == 2) {
        if (pData->waveIndex == 0) {
            return false;
        }
        pOut->waveDataLocation.location = 2;
        pOut->waveDataLocation.waveIndex = pData->waveIndex;
    } else {
        return false;
    }

    pOut->attack = pData->attack;
    pOut->hold = pData->field_0x8;
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
        pOut->tune = 1.0f;
    }

    switch (pData->field_0xa) {
    case 0:
        pOut->noteOffType = 0;
        break;

    case 1:
        pOut->noteOffType = 1;
        break;

    default:
        return false;
    }

    pOut->alternateAssign = pData->field_0xb;
    return true;
}

const BankFile::DataRegion*
BankFileReader::GetReferenceToSubRegion(const BankFile::DataRegion* pRef,
                                        int splitKey) const {
    const BankFile::DataRegion* pSub = nullptr;

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

        if (pRangeTable == nullptr) {
            return nullptr;
        }

        int i = 0;
        while (splitKey > ReadByte(pRangeTable->key + i)) {
            if (++i >= pRangeTable->tableSize) {
                return nullptr;
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

        if (pIndexTable == nullptr) {
            return nullptr;
        }

        if (splitKey < pIndexTable->min || splitKey > pIndexTable->max) {
            return nullptr;
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
    if (ppWaveInfo != nullptr) {
        *ppWaveInfo = nullptr;
    }

    if (mHeader == nullptr) {
        return false;
    }

    // Linear location dispatch (retail lowers as cmpwi/cmpwi/cmpwi equality
    // tests, not a range-checked switch). Case 0 hoists waveIndex into a
    // callee-saved register before the archive reader calls (retail: lwz
    // r30,4(r5) ahead of the mWaveBlock branch).
    if (location.location == 0) {
        u32 waveIndex = location.waveIndex;

        if (mWaveBlock == nullptr) {
            WaveArchiveReader archiveReader(pWaveData);
            const WaveFile::FileHeader* pFile =
                static_cast<const WaveFile::FileHeader*>(
                    archiveReader.GetWaveFile(waveIndex));
            if (pFile == nullptr) {
                return false;
            }

            WaveFileReader wfReader(pFile);
            return wfReader.ReadWaveInfo(pWaveInfo, nullptr);
        }

        if (waveIndex >= mWaveBlock->waveInfoTable.count) {
            return false;
        }

        const WaveFile::WaveInfo* pInfo = Util::GetDataRefAddress0(
            mWaveBlock->waveInfoTable.items[waveIndex],
            &mWaveBlock->waveInfoTable);
        if (pInfo == nullptr) {
            return false;
        }

        WaveFileReader wfReader(pInfo);
        return wfReader.ReadWaveInfo(pWaveInfo, pWaveData);
    } else if (location.location == 1) {
        if (location.waveIndex == 0) {
            return false;
        }

        if (ppWaveInfo != nullptr) {
            *ppWaveInfo = reinterpret_cast<const WaveInfo*>(location.waveIndex);
        }

        *pWaveInfo = *reinterpret_cast<const WaveInfo*>(location.waveIndex);
        return true;
    } else if (location.location == 2) {
        if (location.waveIndex == 0) {
            return false;
        }

        const WaveInfo* pInfo =
            reinterpret_cast<const WaveDataProvider*>(location.waveIndex)
                ->GetWaveInfo();
        if (pInfo == nullptr) {
            return false;
        }

        if (ppWaveInfo != nullptr) {
            *ppWaveInfo = pInfo;
        }

        *pWaveInfo = *pInfo;
        return true;
    } else {
        return false;
    }

    return false;
}

} // namespace detail
} // namespace snd
} // namespace nw4r
