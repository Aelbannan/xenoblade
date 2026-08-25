#include <nw4r/snd.h>
#include <nw4r/ut.h>

#include <cstring>

// ---------------------------------------------------------------------------
// Retail-layout mirrors
//
// The shared headers declare SoundArchive::SoundInfo, StrmSoundInfo and
// SoundCommonInfo without the extra retail fields this build carries, so the
// Read*SoundInfo implementations below access the retail layouts through
// these layout mirrors (same pattern as SoundArchiveFileReaderLayout at the
// bottom of this file).
// ---------------------------------------------------------------------------
namespace {

// Mirrors nw4r::snd::SoundArchive::SoundInfo (retail has an extra field at
// 0x8; the shared header layout is not byte-compatible).
struct SoundInfoLayout {
    u32 fileId;         // at 0x0
    u32 playerId;       // at 0x4
    int field_0x8;      // at 0x8 (copied from SoundCommonInfo+0x2A)
    int playerPriority; // at 0xC
    int volume;         // at 0x10
    int remoteFilter;   // at 0x14
    nw4r::snd::PanMode panMode;   // at 0x18
    nw4r::snd::PanCurve panCurve; // at 0x1C
};

// Mirrors nw4r::snd::detail::SoundArchiveFile::SoundCommonInfo (retail has a
// trailing byte at 0x2A that the shared header does not declare).
struct SoundCommonInfoLayout {
    u32 stringId; // at 0x0
    u32 fileId;   // at 0x4
    u32 playerId; // at 0x8
    nw4r::snd::detail::Util::DataRef<
        nw4r::snd::detail::SoundArchiveFile::Sound3DParam> param3dRef; // at 0xC
    u8 volume;              // at 0x14
    u8 playerPriority;      // at 0x15
    u8 soundType;           // at 0x16
    u8 remoteFilter;        // at 0x17
    nw4r::snd::detail::SoundArchiveFile::SoundInfoOffset soundInfoRef; // at 0x18
    u32 userParam[2];       // at 0x20
    u8 panMode;             // at 0x28
    u8 panCurve;            // at 0x29
    u8 field_0x2a;          // at 0x2A
};

// Mirrors SoundArchive::StrmSoundInfo / SoundArchiveFile::StrmSoundInfo
// (both declared empty in the shared headers; retail carries three fields).
struct StrmSoundInfoLayout {
    u32 dataOffset;   // at 0x0
    u16 channelCount; // at 0x4
    u16 sampleRate;   // at 0x6
};

} // namespace

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

    // impl_GetSoundInfo (inline helper) reproduces the retail fetch exactly:
    // its internal NULL returns funnel into the shared check below.
    const SoundCommonInfoLayout* pCmnInfo =
        reinterpret_cast<const SoundCommonInfoLayout*>(impl_GetSoundInfo(id));

    if (pCmnInfo == NULL) {
        return false;
    }

    // The retail SoundInfo layout (see SoundInfoLayout above) differs from
    // the shared header, so copy through the layout mirror.
    SoundInfoLayout* pOut = reinterpret_cast<SoundInfoLayout*>(pSoundInfo);
    pOut->fileId = pCmnInfo->fileId;
    pOut->playerId = pCmnInfo->playerId;
    pOut->field_0x8 = pCmnInfo->field_0x2a;
    pOut->playerPriority = pCmnInfo->playerPriority;
    pOut->volume = pCmnInfo->volume;
    pOut->remoteFilter = pCmnInfo->remoteFilter;

    if (GetVersion() >= NW4R_VERSION(1, 2)) {
        pOut->panMode = static_cast<PanMode>(pCmnInfo->panMode);
        pOut->panCurve = static_cast<PanCurve>(pCmnInfo->panCurve);
    } else {
        pOut->panMode = PAN_MODE_BALANCE;
        pOut->panCurve = PAN_CURVE_SQRT;
    }

    return true;
}

bool SoundArchiveFileReader::ReadSeqSoundInfo(
    u32 id, SoundArchive::SeqSoundInfo* pInfo) const {

    // NOTE(pi-batch): retail locates the seq info through the out-parameter
    // form of impl_GetSoundInfoOffset preceded by a GetSoundType check; the
    // shared header only declares the by-value form, so the sequence is
    // reproduced through impl_GetSeqSoundInfo instead.
    if (GetSoundType(id) != SOUND_TYPE_SEQ) {
        return false;
    }

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

    // The retail reader locates the strm info through impl_GetSoundInfoOffset
    // (a real call site). This build cannot emit that call: the shared header
    // declares impl_GetSoundInfoOffset with a by-value return, so the offset
    // computation is inlined here instead. Converting to the retail call
    // requires changing that header declaration to the out-parameter form.
    const StrmSoundInfoLayout* pSrc;

    if (GetSoundType(id) != SOUND_TYPE_STRM) {
        pSrc = NULL;
    } else {
        const SoundArchiveFile::SoundCommonTable* pTable =
            Util::GetDataRefAddress0(mInfo->soundTableRef, mInfo);

        if (pTable != NULL && id < pTable->count) {
            SoundArchiveFile::SoundInfoOffset offset;

            if (GetVersion() >= NW4R_VERSION(1, 1)) {
                const SoundCommonInfoLayout* pCmnInfo =
                    reinterpret_cast<const SoundCommonInfoLayout*>(
                        Util::GetDataRefAddress0(pTable->items[id], mInfo));

                if (pCmnInfo != NULL) {
                    offset = pCmnInfo->soundInfoRef;
                    pSrc = reinterpret_cast<const StrmSoundInfoLayout*>(
                        Util::GetDataRefAddress2(offset, mInfo));
                } else {
                    pSrc = NULL;
                }
            } else {
                offset.refType = pTable->items[id].refType;
                offset.dataType = pTable->items[id].dataType;
                offset.value = pTable->items[id].value + 0x1C;
                pSrc = reinterpret_cast<const StrmSoundInfoLayout*>(
                    Util::GetDataRefAddress2(offset, mInfo));
            }
        } else {
            pSrc = NULL;
        }
    }

    if (pSrc == NULL) {
        return false;
    }

    StrmSoundInfoLayout* pOut = reinterpret_cast<StrmSoundInfoLayout*>(pInfo);
    pOut->dataOffset = pSrc->dataOffset;
    pOut->sampleRate = pSrc->sampleRate;

    if (GetVersion() >= NW4R_VERSION(1, 4)) {
        pOut->channelCount = pSrc->channelCount;
    } else {
        // Pre-1.4 archives packed the channel count as 15-bit flag words:
        // a set low bit counts one channel; a clear low bit with any
        // remaining data is an invalid encoding.
        pOut->channelCount = 0;

        u32 flag = pSrc->channelCount;
        while (flag & 0xFFFF) {
            if (flag & 1) {
                pOut->channelCount++;
            } else if (flag & 0xFFFF) {
                return false;
            }
            flag = (flag >> 16) & 0x7FFF;
        }
    }

    return true;
}

bool SoundArchiveFileReader::ReadWaveSoundInfo(
    u32 id, SoundArchive::WaveSoundInfo* pInfo) const {

    if (GetSoundType(id) != SOUND_TYPE_WAVE) {
        return false;
    }

    // NOTE(pi-batch): retail fetches the offset through the out-parameter
    // form of impl_GetSoundInfoOffset (bool return, SoundInfoOffset* result).
    // Emitting that exact symbol requires redeclaring the header's by-value
    // overload; until the header changes, the by-value helper is called
    // directly here to keep the two-call shape.
    SoundArchiveFile::SoundInfoOffset offset = impl_GetSoundInfoOffset(id);

    const SoundArchiveFile::WaveSoundInfo* pSrc =
        Util::GetDataRefAddress3(offset, mInfo);

    if (pSrc == NULL) {
        return false;
    }

    pInfo->subNo = pSrc->subNo;
    pInfo->channelPriority = pSrc->channelPriority;

    if (GetVersion() >= NW4R_VERSION(1, 3)) {
        pInfo->releasePriorityFixFlag = pSrc->releasePriorityFix != 0;
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

// By-value form (shared header declaration); MWCC uses a hidden sret
// pointer for the 12-byte SoundInfoOffset result.
SoundArchiveFile::SoundInfoOffset
SoundArchiveFileReader::impl_GetSoundInfoOffset(u32 id) const {
    const SoundArchiveFile::SoundCommonTable* pTable =
        Util::GetDataRefAddress0(mInfo->soundTableRef, mInfo);

    if (pTable == NULL) {
        return SoundArchiveFile::SoundInfoOffset();
    }

    if (id >= pTable->count) {
        return SoundArchiveFile::SoundInfoOffset();
    }

    if (GetVersion() >= NW4R_VERSION(1, 1)) {
        const SoundArchiveFile::SoundCommonInfo* pInfo =
            Util::GetDataRefAddress0(pTable->items[id], mInfo);

        if (pInfo == NULL) {
            return SoundArchiveFile::SoundInfoOffset();
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

// ---------------------------------------------------------------------------
// GetSoundCount / GetFileCount
//
// The retail SDK exports these as mangled member symbols, but the
// SoundArchiveFileReader class declaration (snd_SoundArchiveFile.h) does not
// declare them in this build. The extern "C" declarations in snd_SoundArchive.h
// pin the C-ABI symbol names, so these are defined here as C-linkage free
// functions reading the reader's private state through the layout mirror
// below.
//
// The `const void* self` parameter (vs the header's plain `void*`) is
// deliberate: MWCC's scheduler only hoists the first member load above the
// LR-save `stw` in the prologue when the self pointer is const-qualified
// (docs/evidence/decomp/attempts.jsonl func_8049BF0C family: "const self
// param hoists mpMgr load above LR-save stw"). MWCC accepts the const
// definition against the header's extern "C" void* declaration and still
// emits the bare C symbol, so the bytes come out retail-identical.
// ---------------------------------------------------------------------------

namespace {
// Mirrors SoundArchiveFileReader's leading members (see snd_SoundArchiveFile.h).
struct SoundArchiveFileReaderLayout {
    nw4r::snd::detail::SoundArchiveFile::Header header;    // at 0x0
    const nw4r::snd::detail::SoundArchiveFile::Info* info; // at 0x28
};
} // namespace

u32 GetSoundCount__Q44nw4r3snd6detail22SoundArchiveFileReaderCFv(const void* self) {
    const nw4r::snd::detail::SoundArchiveFile::Info* pInfo =
        reinterpret_cast<const SoundArchiveFileReaderLayout*>(self)->info;

    const nw4r::snd::detail::SoundArchiveFile::SoundCommonTable* pTable =
        nw4r::snd::detail::Util::GetDataRefAddress0(pInfo->soundTableRef, pInfo);

    if (pTable == NULL) {
        return 0;
    }

    return pTable->count;
}

u32 GetFileCount__Q44nw4r3snd6detail22SoundArchiveFileReaderCFv(const void* self) {
    const nw4r::snd::detail::SoundArchiveFile::Info* pInfo =
        reinterpret_cast<const SoundArchiveFileReaderLayout*>(self)->info;

    const nw4r::snd::detail::SoundArchiveFile::FileTable* pFileTable =
        nw4r::snd::detail::Util::GetDataRefAddress0(pInfo->fileTableRef, pInfo);

    if (pFileTable == NULL) {
        return 0;
    }

    return pFileTable->count;
}
