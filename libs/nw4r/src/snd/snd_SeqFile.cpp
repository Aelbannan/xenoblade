#include <nw4r/snd.h>
#include <nw4r/ut.h>

#include <string.h>

namespace nw4r {
namespace snd {
namespace detail {
namespace {

// Label block / entry inside the RSEQ label table. Values are read natively
// on the big-endian host, so no endian conversion is applied while searching.
struct SeqLabelEntry {
    ut::BinaryBlockHeader blockHeader; // at 0x0
    u32 value;                         // at 0x8
    u32 nameLen;                       // at 0xc
    char name[1];                      // at 0x10
};

struct SeqLabelBlock {
    ut::BinaryBlockHeader blockHeader; // at 0x0
    u32 entryCount;                    // at 0x8
    u32 offset[1];                     // at 0xc
};

// Views SeqFileReader's data members so the offset-0 header pointer is
// reachable without its private members.
struct SeqFileReaderView {
    const SeqFile::Header* mHeader; // at 0x0
    const void* mDataBlock;         // at 0x4
};

// Pointer add that keeps the source operand order (base first) when lowered.
template <typename T>
inline const void* AddPtrBaseFirst(const void* pBase, T offset) {
    return reinterpret_cast<const void*>(reinterpret_cast<const u8*>(pBase) + offset);
}

} // namespace

bool SeqFileReader::IsValidFileHeader(const void* pSeqBin) {
    const ut::BinaryFileHeader* pFileHeader =
        static_cast<const ut::BinaryFileHeader*>(pSeqBin);

    if (pFileHeader->signature != SIGNATURE) {
        return false;
    }

    if (Util::ReadBigEndian(pFileHeader->version) < NW4R_VERSION(1, 0)) {
        return false;
    }

    if (Util::ReadBigEndian(pFileHeader->version) > VERSION) {
        return false;
    }

    return true;
}

SeqFileReader::SeqFileReader(const void* pSeqBin)
    : mHeader(NULL), mDataBlock(NULL) {
    if (!IsValidFileHeader(pSeqBin)) {
        return;
    }

    mHeader = static_cast<const SeqFile::Header*>(pSeqBin);

    mDataBlock = static_cast<const SeqFile::DataBlock*>(ut::AddOffsetToPtr(
        mHeader, Util::ReadBigEndian(mHeader->dataBlockOffset)));
}

const void* SeqFileReader::GetBaseAddress() const {
    return ut::AddOffsetToPtr(mDataBlock,
                              Util::ReadBigEndian(mDataBlock->baseOffset));
}

bool ReadOffsetByLabel__Q44nw4r3snd6detail13SeqFileReaderCFPCcPUl(
    const SeqFileReaderView* pReader, const char* label, u32* outOffset) {
    const SeqFile::Header* pHeader = pReader->mHeader;
    const SeqLabelBlock* pLabelBlock =
        reinterpret_cast<const SeqLabelBlock*>(ut::GetIntPtr(pHeader) +
                                               pHeader->labelBlockOffset);

    if (pLabelBlock == NULL) {
        return false;
    }

    u32 nameLen = strlen(label);

    // Explicit offset cursor: defined before the entryCount CSE temp so it
    // takes the lower register, matching retail allocation.
    const u32* pOffset = pLabelBlock->offset;

    for (u32 i = 0; i < pLabelBlock->entryCount; i++, pOffset++) {
        const SeqLabelEntry* pEntry =
            static_cast<const SeqLabelEntry*>(AddPtrBaseFirst(pLabelBlock, *pOffset));

        if (nameLen == pEntry->nameLen &&
            strncmp(label, pEntry->name, nameLen) == 0) {
            *outOffset = pEntry->value;
            return true;
        }
    }

    return false;
}

} // namespace detail
} // namespace snd
} // namespace nw4r