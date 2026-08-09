#include <nw4r/snd.h>
#include <nw4r/ut.h>

namespace nw4r {
namespace snd {
namespace detail {

// The retail RBNK reader works against field layouts that differ from the
// placeholder headers shipped in snd_BankFile.h (the header InstParam packs
// bytes 0x8..0xb into one padded array, and the header InstInfo is a smaller
// public-only structure). These mirror types expose the exact on-disk offsets
// the reader touches; the pointers are re-cast to them at the use site.
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

// See BankFile::Region
enum {
    DATATYPE_NONE = Util::DATATYPE_T0,
    DATATYPE_INSTPARAM = Util::DATATYPE_T1,
    DATATYPE_RANGETABLE = Util::DATATYPE_T2,
    DATATYPE_INDEXTABLE = Util::DATATYPE_T3,
};

bool BankFileReader::IsValidFileHeader(const void* pBankBin) {
    const ut::BinaryFileHeader* pFileHeader =
        static_cast<const ut::BinaryFileHeader*>(pBankBin);

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

BankFileReader::BankFileReader(const void* pBankBin)
    : mHeader(NULL), mDataBlock(NULL), mWaveBlock(NULL) {
    if (!IsValidFileHeader(pBankBin)) {
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

bool BankFileReader::ReadInstInfo(InstInfo* pInfo, int prgNo, int key,
                                  int velocity) const {
    if (mHeader == NULL) {
        return false;
    }

    if (prgNo < 0 || prgNo >= static_cast<int>(mDataBlock->instTable.count)) {
        return false;
    }

    const BankFile::DataRegion* pRef = &mDataBlock->instTable.items[prgNo];
    if (pRef->dataType == Util::DATATYPE_INVALID) {
        return false;
    }

    if (pRef->dataType != DATATYPE_INSTPARAM) {
        pRef = GetReferenceToSubRegion(pRef, key);
        if (pRef == NULL) {
            return false;
        }
    }

    if (pRef->dataType == Util::DATATYPE_INVALID) {
        return false;
    }

    if (pRef->dataType != DATATYPE_INSTPARAM) {
        pRef = GetReferenceToSubRegion(pRef, velocity);
        if (pRef == NULL) {
            return false;
        }
    }

    if (pRef->dataType != DATATYPE_INSTPARAM) {
        return false;
    }

    const BankFile::InstParam* pParam =
        Util::GetDataRefAddress1(*pRef, &mDataBlock->instTable);

    if (pParam == NULL) {
        return false;
    }

    if (pParam->waveIndex < 0) {
        return false;
    }

    pInfo->waveIndex = pParam->waveIndex;
    pInfo->attack = pParam->attack;
    pInfo->decay = pParam->decay;
    pInfo->sustain = pParam->sustain;
    pInfo->release = pParam->release;
    pInfo->originalKey = pParam->originalKey;
    pInfo->pan = pParam->pan;

    if (mHeader->fileHeader.version >= VERSION) {
        pInfo->volume = pParam->volume;
        pInfo->tune = pParam->tune;
    } else {
        pInfo->volume = 127;
        pInfo->tune = 1.0f;
    }

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

bool BankFileReader::ReadWaveParam(WaveData* pData, int waveIndex,
                                   const void* pWaveAddr) const {
    if (mHeader == NULL) {
        return false;
    }

    if (mWaveBlock == NULL) {
        return false;
    }

    if (waveIndex >= mWaveBlock->waveInfoTable.count) {
        return false;
    }

    const BankFile::WaveRegion* pRef =
        &mWaveBlock->waveInfoTable.items[waveIndex];

    const WaveInfo* pInfo =
        Util::GetDataRefAddress0(*pRef, &mWaveBlock->waveInfoTable);

    if (pInfo == NULL) {
        return false;
    }

    WaveFileReader wfr(pInfo);
    return wfr.ReadWaveParam(pData, pWaveAddr);
}

} // namespace detail
} // namespace snd
} // namespace nw4r
