#include <nw4hbm/ut.h>

namespace nw4hbm {
namespace ut {

bool IsValidBinaryFile(const BinaryFileHeader* pHeader, u32 signature,
                       u16 version, u16 minBlocks) {
    if (pHeader->signature != signature) {
        return false;
    }

    if (pHeader->byteOrder != NW4R_BYTEORDER_NATIVE) {
        return false;
    }

    if (pHeader->version != version) {
        return false;
    }

    if (pHeader->fileSize <
        sizeof(BinaryFileHeader) + (minBlocks * sizeof(BinaryBlockHeader))) {
        return false;
    }

    if (pHeader->dataBlocks < minBlocks) {
        return false;
    }

    return true;
}

#if 0
// unused — not present in retail
bool IsReverseEndianBinaryFile(const BinaryFileHeader* pHeader) {
    return false;
}

BinaryBlockHeader* GetNextBinaryBlockHeader(BinaryFileHeader* pHeader, BinaryBlockHeader* pBlockHeader) {
    return nullptr;
}
#endif

} // namespace ut
} // namespace nw4hbm
