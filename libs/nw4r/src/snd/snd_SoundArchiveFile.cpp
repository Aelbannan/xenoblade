#include <nw4r/snd.h>
#include <nw4r/ut.h>

#include <cstring>

namespace nw4r {
namespace snd {
namespace detail {

SoundArchiveFileReader::SoundArchiveFileReader()
    : mInfo(NULL),
      mStringBase(NULL),
      mStringTable(NULL),
      mStringTreeSound(NULL),
      mStringTreePlayer(NULL),
      mStringTreeGroup(NULL),
      mStringTreeBank(NULL) {}

void SoundArchiveFileReader::Init(const void* pSoundArchiveBin) {
    if (!IsValidFileHeader(pSoundArchiveBin)) {
        return;
    }

    mHeader = *static_cast<const SoundArchiveFile::Header*>(pSoundArchiveBin);
}

void SoundArchiveFileReader::SetStringChunk(const void* pChunk, u32 size) {
#pragma unused(size)

    const SoundArchiveFile::SymbolBlock* pSymbolBlock =
        static_cast<const SoundArchiveFile::SymbolBlock*>(pChunk);

    const SoundArchiveFile::StringBlock* pStringBlock =
        &pSymbolBlock->stringBlock;

    mStringBase = pStringBlock;

    mStringTable = static_cast<const SoundArchiveFile::StringTable*>(
        GetPtrConst(mStringBase, pStringBlock->stringChunk.tableOffset));

    mStringTreeSound = static_cast<const SoundArchiveFile::StringTree*>(
        GetPtrConst(mStringBase, pStringBlock->stringChunk.soundTreeOffset));

    mStringTreePlayer = static_cast<const SoundArchiveFile::StringTree*>(
        GetPtrConst(mStringBase, pStringBlock->stringChunk.playerTreeOffset));

    mStringTreeGroup = static_cast<const SoundArchiveFile::StringTree*>(
        GetPtrConst(mStringBase, pStringBlock->stringChunk.groupTreeOffset));

    mStringTreeBank = static_cast<const SoundArchiveFile::StringTree*>(
        GetPtrConst(mStringBase, pStringBlock->stringChunk.bankTreeOffset));
}

void SoundArchiveFileReader::SetInfoChunk(const void* pChunk, u32 size) {
#pragma unused(size)

    const SoundArchiveFile::InfoBlock* pInfoBlock =
        static_cast<const SoundArchiveFile::InfoBlock*>(pChunk);

    mInfo = &pInfoBlock->info;
}

SoundType SoundArchiveFileReader::GetSoundType(u32 id) const {
    SoundType soundType;

    const SoundArchiveFile::SoundCommonTable* pTable =
        Util::GetDataRefAddress0(mInfo->soundTableRef, mInfo);

    if (pTable == NULL) {
        return SOUND_TYPE_INVALID;
    }

    if (id >= pTable->count) {
        return SOUND_TYPE_INVALID;
    }

    if (GetVersion() >= NW4R_VERSION(1, 1)) {
        const SoundArchiveFile::SoundCommonInfo* pCmnInfo =
            Util::GetDataRefAddress0(pTable->items[id], mInfo);

        if (pCmnInfo == NULL) {
            return SOUND_TYPE_INVALID;
        }

        soundType = static_cast<SoundType>(pCmnInfo->soundType);
    } else {
        soundType = static_cast<SoundType>(pTable->items[id].dataType);
    }

    switch (soundType) {
    case SOUND_TYPE_SEQ: {
        return SOUND_TYPE_SEQ;
    }

    case SOUND_TYPE_STRM: {
        return SOUND_TYPE_STRM;
    }

    case SOUND_TYPE_WAVE: {
        return SOUND_TYPE_WAVE;
    }

    default: {
        return SOUND_TYPE_INVALID;
    }
    }
}

bool SoundArchiveFileReader::ReadSoundInfo(
    u32 id, SoundArchive::SoundInfo* pSoundInfo) const {

    const SoundArchiveFile::SoundCommonInfo* pCmnInfo = impl_GetSoundInfo(id);

    if (pCmnInfo == NULL) {
        return false;
    }

    pSoundInfo->fileId = pCmnInfo->fileId;
    pSoundInfo->playerId = pCmnInfo->playerId;
    pSoundInfo->playerPriority = pCmnInfo->playerPriority;
    pSoundInfo->volume = pCmnInfo->volume;
    pSoundInfo->remoteFilter = pCmnInfo->remoteFilter;

    if (GetVersion() >= NW4R_VERSION(1, 2)) {
        pSoundInfo->panMode = static_cast<PanMode>(pCmnInfo->panMode);
        pSoundInfo->panCurve = static_cast<PanCurve>(pCmnInfo->panCurve);
    } else {
        pSoundInfo->panMode = PAN_MODE_BALANCE;
        pSoundInfo->panCurve = PAN_CURVE_SQRT;
    }

    return true;
}

bool SoundArchiveFileReader::ReadSeqSoundInfo(
    u32 id, SoundArchive::SeqSoundInfo* pInfo) const {

    const SoundArchiveFile::SeqSoundInfo* pSrc = impl_GetSeqSoundInfo(id);

    if (pSrc == NULL) {
        return false;
    }

    pInfo->dataOffset = pSrc->dataOffset;
    pInfo->bankId = pSrc->bankId;
    pInfo->channelPriority = pSrc->channelPriority;
    pInfo->allocTrack = pSrc->allocTrack;

    if (GetVersion() >= NW4R_VERSION(1, 3)) {
        pInfo->releasePriorityFixFlag = pSrc->releasePriorityFix;
    } else {
        pInfo->releasePriorityFixFlag = false;
    }

    return true;
}

bool SoundArchiveFileReader::ReadStrmSoundInfo(
    u32 id, SoundArchive::StrmSoundInfo* pInfo) const {
#pragma unused(pInfo)

    const SoundArchiveFile::StrmSoundInfo* pSrc = impl_GetStrmSoundInfo(id);

    if (pSrc == NULL) {
        return false;
    }

    // StrmSoundInfo is empty in this version of NW4R
    return true;
}

bool SoundArchiveFileReader::ReadWaveSoundInfo(
    u32 id, SoundArchive::WaveSoundInfo* pInfo) const {

    const SoundArchiveFile::WaveSoundInfo* pSrc = impl_GetWaveSoundInfo(id);

    if (pSrc == NULL) {
        return false;
    }

    pInfo->subNo = pSrc->subNo;
    pInfo->channelPriority = pSrc->channelPriority;

    if (GetVersion() >= NW4R_VERSION(1, 3)) {
        pInfo->releasePriorityFixFlag = pSrc->releasePriorityFix;
    } else {
        pInfo->releasePriorityFixFlag = false;
    }

    return true;
}

bool SoundArchiveFileReader::ReadBankInfo(u32 id,
                                          SoundArchive::BankInfo* pInfo) const {

    const SoundArchiveFile::BankInfo* pSrc = impl_GetBankInfo(id);

    if (pSrc == NULL) {
        return false;
    }

    pInfo->fileId = pSrc->fileId;

    return true;
}

bool SoundArchiveFileReader::ReadPlayerInfo(
    u32 id, SoundArchive::PlayerInfo* pInfo) const {

    const SoundArchiveFile::PlayerInfo* pSrc = impl_GetPlayerInfo(id);

    if (pSrc == NULL) {
        return false;
    }

    pInfo->playableSoundCount = pSrc->playableSoundCount;
    pInfo->heapSize = pSrc->heapSize;

    return true;
}

bool SoundArchiveFileReader::ReadGroupInfo(
    u32 id, SoundArchive::GroupInfo* pInfo) const {

    const SoundArchiveFile::GroupInfo* pSrc = impl_GetGroupInfo(id);

    if (pSrc == NULL) {
        return false;
    }

    const SoundArchiveFile::GroupItemTable* pTable =
        Util::GetDataRefAddress0(pSrc->itemTableRef, mInfo);

    if (pTable == NULL) {
        return false;
    }

    pInfo->extFilePath = Util::GetDataRefAddress0(pSrc->extFilePathRef, mInfo);
    pInfo->offset = pSrc->offset;
    pInfo->size = pSrc->size;
    pInfo->waveDataOffset = pSrc->waveDataOffset;
    pInfo->waveDataSize = pSrc->waveDataSize;
    pInfo->itemCount = pTable->count;

    return true;
}

bool SoundArchiveFileReader::ReadGroupItemInfo(
    u32 groupId, u32 itemId, SoundArchive::GroupItemInfo* pInfo) const {

    const SoundArchiveFile::GroupInfo* pGroup = impl_GetGroupInfo(groupId);

    if (pGroup == NULL) {
        return false;
    }

    const SoundArchiveFile::GroupItemTable* pTable =
        Util::GetDataRefAddress0(pGroup->itemTableRef, mInfo);

    if (pTable == NULL) {
        return false;
    }

    if (itemId >= pTable->count) {
        return false;
    }

    const SoundArchiveFile::GroupItemInfo* pSrc =
        Util::GetDataRefAddress0(pTable->items[itemId], mInfo);

    if (pSrc == NULL) {
        return false;
    }

    pInfo->fileId = pSrc->fileId;
    pInfo->offset = pSrc->offset;
    pInfo->size = pSrc->size;
    pInfo->waveDataOffset = pSrc->waveDataOffset;
    pInfo->waveDataSize = pSrc->waveDataSize;

    return true;
}

bool SoundArchiveFileReader::ReadSoundArchivePlayerInfo(
    SoundArchive::SoundArchivePlayerInfo* pInfo) const {

    const SoundArchiveFile::SoundArchivePlayerInfo* pSrc =
        Util::GetDataRefAddress0(mInfo->soundArchivePlayerInfoRef, mInfo);

    // @bug Doesn't check dataref result
    if (pInfo == NULL) {
        return false;
    }

    pInfo->seqSoundCount = pSrc->seqSoundCount;
    pInfo->seqTrackCount = pSrc->seqTrackCount;
    pInfo->strmSoundCount = pSrc->strmSoundCount;
    pInfo->strmTrackCount = pSrc->strmTrackCount;
    pInfo->strmChannelCount = pSrc->strmChannelCount;
    pInfo->waveSoundCount = pSrc->waveSoundCount;
    pInfo->waveTrackCount = pSrc->waveTrackCount;

    return true;
}

u32 SoundArchiveFileReader::GetPlayerCount() const {
    const SoundArchiveFile::PlayerTable* pTable =
        Util::GetDataRefAddress0(mInfo->playerTableRef, mInfo);

    if (pTable == NULL) {
        return 0;
    }

    return pTable->count;
}

u32 SoundArchiveFileReader::GetGroupCount() const {
    const SoundArchiveFile::GroupTable* pTable =
        Util::GetDataRefAddress0(mInfo->groupTableRef, mInfo);

    if (pTable == NULL) {
        return 0;
    }

    return pTable->count - 1;
}

u32 SoundArchiveFileReader::GetSoundUserParam(u32 id) const {
    const SoundArchiveFile::SoundCommonInfo* pInfo = impl_GetSoundInfo(id);

    if (pInfo == NULL) {
        return 0;
    }

    return pInfo->userParam[0];
}

bool SoundArchiveFileReader::ReadFileInfo(u32 id,
                                          SoundArchive::FileInfo* pInfo) const {

    const SoundArchiveFile::FileTable* pFileTable =
        Util::GetDataRefAddress0(mInfo->fileTableRef, mInfo);

    if (pFileTable == NULL) {
        return false;
    }

    if (id >= pFileTable->count) {
        return false;
    }

    const SoundArchiveFile::FileInfo* pFile =
        Util::GetDataRefAddress0(pFileTable->items[id], mInfo);

    if (pFile == NULL) {
        return false;
    }

    const SoundArchiveFile::FilePosTable* pPosTable =
        Util::GetDataRefAddress0(pFile->filePosTableRef, mInfo);

    if (pPosTable == NULL) {
        return false;
    }

    pInfo->fileSize = pFile->fileSize;
    pInfo->waveDataFileSize = pFile->waveDataSize;
    pInfo->extFilePath = Util::GetDataRefAddress0(pFile->extFilePathRef, mInfo);
    pInfo->filePosCount = pPosTable->count;

    return true;
}

bool SoundArchiveFileReader::ReadFilePos(u32 fileId, u32 id,
                                         SoundArchive::FilePos* pPos) const {

    const SoundArchiveFile::FileTable* pFileTable =
        Util::GetDataRefAddress0(mInfo->fileTableRef, mInfo);

    if (pFileTable == NULL) {
        return false;
    }

    if (fileId >= pFileTable->count) {
        return false;
    }

    const SoundArchiveFile::FileInfo* pInfo =
        Util::GetDataRefAddress0(pFileTable->items[fileId], mInfo);

    if (pInfo == NULL) {
        return false;
    }

    const SoundArchiveFile::FilePosTable* pPosTable =
        Util::GetDataRefAddress0(pInfo->filePosTableRef, mInfo);

    if (pPosTable == NULL) {
        return false;
    }

    if (id >= pPosTable->count) {
        return false;
    }

    const SoundArchive::FilePos* pSrc =
        Util::GetDataRefAddress0(pPosTable->items[id], mInfo);

    if (pSrc == NULL) {
        return false;
    }

    *pPos = *pSrc;

    return true;
}

u32 SoundArchiveFileReader::ConvertLabelStringToId(
    const SoundArchiveFile::StringTree* pTree, const char* pLabel) const {

    if (pTree == NULL) {
        return SoundArchive::INVALID_ID;
    }

    if (pTree->rootIdx >= pTree->nodeTable.count) {
        return SoundArchive::INVALID_ID;
    }

    const SoundArchiveFile::StringTreeNode* pNode =
        &pTree->nodeTable.items[pTree->rootIdx];

    int length = std::strlen(pLabel);

    while (!(pNode->flags & 1)) {
        int pos = pNode->bit >> 3;
        int bit = pNode->bit & 7;

        u32 nodeIndex;
        if (pos < length && (1 << (7 - bit)) & pLabel[pos]) {
            nodeIndex = pNode->rightIdx;
        } else {
            nodeIndex = pNode->leftIdx;
        }

        pNode = &pTree->nodeTable.items[nodeIndex];
    }

    const char* pExpected = GetString(pNode->strIdx);

    if (std::strcmp(pLabel, pExpected) == 0) {
        return pNode->id;
    }

    return SoundArchive::INVALID_ID;
}

SoundArchiveFile::SoundInfoOffset
SoundArchiveFileReader::impl_GetSoundInfoOffset(u32 id) const {
    static SoundArchiveFile::SoundInfoOffset INVALID_DATA_REF;

    const SoundArchiveFile::SoundCommonTable* pTable =
        Util::GetDataRefAddress0(mInfo->soundTableRef, mInfo);

    if (pTable == NULL) {
        return INVALID_DATA_REF;
    }

    if (id >= pTable->count) {
        return INVALID_DATA_REF;
    }

    if (GetVersion() >= NW4R_VERSION(1, 1)) {
        const SoundArchiveFile::SoundCommonInfo* pInfo =
            Util::GetDataRefAddress0(pTable->items[id], mInfo);

        if (pInfo == NULL) {
            return INVALID_DATA_REF;
        }

        return pInfo->soundInfoRef;
    }

    SoundArchiveFile::SoundInfoOffset ref;

    // TODO(kiwi) Why is the offset changed?
    ref.refType = pTable->items[id].refType;
    ref.dataType = pTable->items[id].dataType;
    ref.value = pTable->items[id].value + 0x1C;

    return ref;
}

} // namespace detail
} // namespace snd
} // namespace nw4r

extern "C" u32 GetSoundCount__Q44nw4r3snd6detail22SoundArchiveFileReaderCFv(void* self) { return 0; }
extern "C" u32 GetFileCount__Q44nw4r3snd6detail22SoundArchiveFileReaderCFv(void* self) { return 0; }
