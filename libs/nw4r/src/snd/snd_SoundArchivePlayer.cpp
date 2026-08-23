#include <nw4r/snd.h>
#include <nw4r/ut.h>

// Import from snd_SoundArchive.cpp (retail symbol; not declared in snd_SoundArchive.h)
extern "C" u32 detail_GetFileCount__Q34nw4r3snd12SoundArchiveCFv(
    const nw4r::snd::SoundArchive* self);

namespace nw4r {
namespace snd {

extern "C" SoundStartable::StartResult PrepareSeqImpl__Q34nw4r3snd18SoundArchivePlayerFPQ44nw4r3snd6detail8SeqSoundPCQ44nw4r3snd12SoundArchive9SoundInfoPCQ44nw4r3snd12SoundArchive12SeqSoundInfoQ54nw4r3snd14SoundStartable9StartInfo15StartOffsetTypeiPCvPCc(
    SoundArchivePlayer* thisPtr, detail::SeqSound* pSound,
    const SoundArchive::SoundInfo* pSndInfo,
    const SoundArchive::SeqSoundInfo* pSeqInfo,
    SoundStartable::StartInfo::StartOffsetType startType, int startOffset,
    const void* pSeqBin, const char* pLabelStr);
} // namespace snd
} // namespace nw4r

namespace nw4r {
namespace snd {

SoundArchivePlayer::SoundArchivePlayer()
    : mSoundArchive(NULL),
      mGroupTable(NULL),
      mFileManager(NULL),
      mSeqCallback(*this),
      mWsdCallback(*this),
      mSoundPlayerCount(0),
      mSoundPlayers(NULL),
      mMmlSeqTrackAllocator(&mMmlParser),
      mSetupBufferAddress(NULL),
      mSetupBufferSize(0) {

    detail::DisposeCallbackManager::GetInstance().RegisterDisposeCallback(this);
}

SoundArchivePlayer::~SoundArchivePlayer() {
    detail::DisposeCallbackManager::GetInstance().UnregisterDisposeCallback(
        this);
}

bool SoundArchivePlayer::IsAvailable() const {
    if (mSoundArchive == NULL) {
        return false;
    }

    return mSoundArchive->IsAvailable();
}

bool SoundArchivePlayer::Setup(const SoundArchive* pArchive, void* pMramBuffer,
                               u32 mramBufferSize, void* pStrmBuffer,
                               u32 strmBufferSize) {
    if (!SetupMram(pArchive, pMramBuffer, mramBufferSize)) {
        return false;
    }

    if (!SetupStrmBuffer(pArchive, pStrmBuffer, strmBufferSize)) {
        return false;
    }

    mSeqTrackAllocator = &mMmlSeqTrackAllocator;
    return true;
}

void SoundArchivePlayer::Shutdown() {
    mSoundArchive = NULL;
    mGroupTable = NULL;
    mFileManager = NULL;
    mSeqTrackAllocator = NULL;

    for (int i = 0; i < mSoundPlayerCount; i++) {
        mSoundPlayers[i].~SoundPlayer();
    }

    mSoundPlayerCount = 0;
    mSoundPlayers = NULL;

    mSeqSoundInstanceManager.Destroy(mSetupBufferAddress, mSetupBufferSize);
    mStrmSoundInstanceManager.Destroy(mSetupBufferAddress, mSetupBufferSize);
    mWaveSoundInstanceManager.Destroy(mSetupBufferAddress, mSetupBufferSize);
    mMmlSeqTrackAllocator.Destroy(mSetupBufferAddress, mSetupBufferSize);

    mStrmBufferPool.Shutdown();
    mSetupBufferAddress = NULL;
    mSetupBufferSize = 0;
}

u32 SoundArchivePlayer::GetRequiredMemSize(const SoundArchive* pArchive) {
    u32 size = 0;

    u32 playerCount = pArchive->GetPlayerCount();
    size += ut::RoundUp(playerCount * sizeof(SoundPlayer), 4);

    for (u32 i = 0; i < playerCount; i++) {
        SoundArchive::PlayerInfo info;
        if (!pArchive->ReadPlayerInfo(i, &info)) {
            continue;
        }

        // Heap size is rounded first, then the running total is aligned
        // down to a 32-byte boundary once per playable sound.
        for (int j = 0; j < info.playableSoundCount; j++) {
            if (info.heapSize != 0) {
                size = ut::RoundUp(size + sizeof(detail::PlayerHeap), 32);
                size += ut::RoundUp(info.heapSize, 4);
            }
        }
    }

    size += (pArchive->GetGroupCount() * sizeof(Group) + 7) & ~3u;

    SoundArchive::SoundArchivePlayerInfo info;
    if (pArchive->ReadSoundArchivePlayerInfo(&info)) {
        // clang-format off
        // Retail instance sizes (headers over-allocate)
        size += ut::RoundUp(info.seqSoundCount  * 0x4d0, 4);   // SeqSound
        size += ut::RoundUp(info.strmSoundCount * 0x10d0, 4);  // StrmSound
        size += ut::RoundUp(info.waveSoundCount * 0x250, 4);   // WaveSound
        size += ut::RoundUp(info.seqTrackCount  * 0xcc, 4);    // MmlSeqTrack
        // clang-format on
    }

    size += (detail_GetFileCount__Q34nw4r3snd12SoundArchiveCFv(pArchive) *
                 sizeof(SoundArchive::FilePos) +
             7) &
            ~3u;

    return size;
}

u32 SoundArchivePlayer::GetRequiredStrmBufferSize(
    const SoundArchive* pArchive) {
    int strmChannels = 0;

    SoundArchive::SoundArchivePlayerInfo info;
    if (pArchive->ReadSoundArchivePlayerInfo(&info)) {
        strmChannels = info.strmChannelCount;
    }

    // TODO(kiwi) How is this calculated?
    return strmChannels * 0xA000;
}

bool SoundArchivePlayer::SetupMram(const SoundArchive* pArchive, void* pBuffer,
                                   u32 bufferSize) {
    void* pEndPtr = static_cast<u8*>(pBuffer) + bufferSize;
    void* pPtr = pBuffer;

    if (!SetupSoundPlayer(pArchive, &pPtr, pEndPtr)) {
        return false;
    }

    if (!CreateGroupAddressTable(pArchive, &pPtr, pEndPtr)) {
        return false;
    }

    SoundArchive::SoundArchivePlayerInfo info;
    if (pArchive->ReadSoundArchivePlayerInfo(&info)) {
        if (!SetupSeqSound(pArchive, info.seqSoundCount, &pPtr, pEndPtr)) {
            return false;
        }

        if (!SetupStrmSound(pArchive, info.strmSoundCount, &pPtr, pEndPtr)) {
            return false;
        }

        if (!SetupWaveSound(pArchive, info.waveSoundCount, &pPtr, pEndPtr)) {
            return false;
        }

        if (!SetupSeqTrack(pArchive, info.seqTrackCount, &pPtr, pEndPtr)) {
            return false;
        }
    }

    mSoundArchive = pArchive;
    mSetupBufferAddress = pBuffer;
    mSetupBufferSize = bufferSize;

    return true;
}

detail::PlayerHeap* SoundArchivePlayer::CreatePlayerHeap(void* pBuffer,
                                                         u32 bufferSize) {
    detail::PlayerHeap* pHeap = new (pBuffer) detail::PlayerHeap();

    pBuffer = ut::AddOffsetToPtr(pBuffer, sizeof(detail::PlayerHeap));

    if (!pHeap->Create(pBuffer, bufferSize)) {
        return NULL;
    }

    return pHeap;
}

bool SoundArchivePlayer::SetupSoundPlayer(const SoundArchive* pArchive,
                                          void** ppBuffer, void* pEnd) {
    u32 playerCount = pArchive->GetPlayerCount();
    u32 requireSize = playerCount * sizeof(SoundPlayer);

    void* pPlayerEnd =
        ut::RoundUp(ut::AddOffsetToPtr(*ppBuffer, requireSize), 4);

    if (ut::ComparePtr(pPlayerEnd, pEnd) > 0) {
        return false;
    }

    void* pPlayerBuffer = *ppBuffer;
    *ppBuffer = pPlayerEnd;

    mSoundPlayers = static_cast<SoundPlayer*>(pPlayerBuffer);
    mSoundPlayerCount = playerCount;

    u8* pPtr = static_cast<u8*>(pPlayerBuffer);

    for (u32 i = 0; i < playerCount; i++, pPtr += sizeof(SoundPlayer)) {
        SoundPlayer* pPlayer = new (pPtr) SoundPlayer();

        SoundArchive::PlayerInfo info;
        if (!pArchive->ReadPlayerInfo(i, &info)) {
            continue;
        }

        pPlayer->SetPlayableSoundCount(info.playableSoundCount);
        pPlayer->detail_SetPlayableSoundLimit(info.playableSoundCount);

        if (info.heapSize == 0) {
            continue;
        }

        for (int j = 0; j < info.playableSoundCount; j++) {
            u32 requireSize = sizeof(detail::PlayerHeap) + info.heapSize;

            void* pHeapEnd =
                ut::RoundUp(ut::AddOffsetToPtr(*ppBuffer, requireSize), 4);

            if (ut::ComparePtr(pHeapEnd, pEnd) > 0) {
                return false;
            }

            void* pHeapBuffer = *ppBuffer;
            *ppBuffer = pHeapEnd;

            detail::PlayerHeap* pHeap =
                CreatePlayerHeap(pHeapBuffer, info.heapSize);

            if (pHeap == NULL) {
                return false;
            }

            pPlayer->detail_AppendPlayerHeap(pHeap);
        }
    }

    return true;
}

bool SoundArchivePlayer::CreateGroupAddressTable(const SoundArchive* pArchive,
                                                 void** ppBuffer, void* pEnd) {
    // clang-format off
    u32 requireSize = 
        pArchive->GetGroupCount() * sizeof(Group) + (sizeof(GroupTable) - sizeof(Group));
    // clang-format on

    void* pTableEnd =
        ut::RoundUp(ut::AddOffsetToPtr(*ppBuffer, requireSize), 4);

    if (ut::ComparePtr(pTableEnd, pEnd) > 0) {
        return false;
    }

    mGroupTable = static_cast<GroupTable*>(*ppBuffer);
    *ppBuffer = pTableEnd;

    mGroupTable->count = pArchive->GetGroupCount();

    for (int i = 0; i < mGroupTable->count; i++) {
        mGroupTable->items[i].address = NULL;
        mGroupTable->items[i].waveDataAddress = NULL;
    }

    return true;
}

bool SoundArchivePlayer::SetupSeqSound(const SoundArchive* pArchive, int sounds,
                                       void** ppBuffer, void* pEnd) {
#pragma unused(pArchive)

    u32 requireSize = sounds * sizeof(detail::SeqSound);

    void* pSoundEnd =
        ut::RoundUp(ut::AddOffsetToPtr(*ppBuffer, requireSize), 4);

    if (ut::ComparePtr(pSoundEnd, pEnd) > 0) {
        return false;
    }

    mSeqSoundInstanceManager.Create(*ppBuffer, requireSize);
    *ppBuffer = pSoundEnd;

    return true;
}

bool SoundArchivePlayer::SetupWaveSound(const SoundArchive* pArchive,
                                        int sounds, void** ppBuffer,
                                        void* pEnd) {
#pragma unused(pArchive)

    u32 requireSize = sounds * sizeof(detail::WaveSound);

    void* pSoundEnd =
        ut::RoundUp(ut::AddOffsetToPtr(*ppBuffer, requireSize), 4);

    if (ut::ComparePtr(pSoundEnd, pEnd) > 0) {
        return false;
    }

    mWaveSoundInstanceManager.Create(*ppBuffer, requireSize);
    *ppBuffer = pSoundEnd;

    return true;
}

bool SoundArchivePlayer::SetupStrmSound(const SoundArchive* pArchive,
                                        int sounds, void** ppBuffer,
                                        void* pEnd) {
#pragma unused(pArchive)

    u32 requireSize = sounds * sizeof(detail::StrmSound);

    void* pSoundEnd =
        ut::RoundUp(ut::AddOffsetToPtr(*ppBuffer, requireSize), 4);

    if (ut::ComparePtr(pSoundEnd, pEnd) > 0) {
        return false;
    }

    mStrmSoundInstanceManager.Create(*ppBuffer, requireSize);
    *ppBuffer = pSoundEnd;

    return true;
}

bool SoundArchivePlayer::SetupSeqTrack(const SoundArchive* pArchive, int tracks,
                                       void** ppBuffer, void* pEnd) {
#pragma unused(pArchive)

    u32 requireSize = tracks * sizeof(detail::MmlSeqTrack);

    void* pTrackEnd =
        ut::RoundUp(ut::AddOffsetToPtr(*ppBuffer, requireSize), 4);

    if (ut::ComparePtr(pTrackEnd, pEnd) > 0) {
        return false;
    }

    mMmlSeqTrackAllocator.Create(*ppBuffer, requireSize);
    *ppBuffer = pTrackEnd;

    return true;
}

bool SoundArchivePlayer::SetupStrmBuffer(const SoundArchive* pArchive,
                                         void* pBuffer, u32 bufferSize) {
    if (bufferSize < GetRequiredStrmBufferSize(pArchive)) {
        return false;
    }

    int strmChannels = 0;

    SoundArchive::SoundArchivePlayerInfo info;
    if (pArchive->ReadSoundArchivePlayerInfo(&info)) {
        strmChannels = info.strmChannelCount;
    }

    mStrmBufferPool.Setup(pBuffer, bufferSize, strmChannels);
    return true;
}

void SoundArchivePlayer::Update() {
    for (u32 i = 0; i < mSoundPlayerCount; i++) {
        GetSoundPlayer(i).Update();
    }

    mSeqSoundInstanceManager.SortPriorityList();
    mStrmSoundInstanceManager.SortPriorityList();
    mWaveSoundInstanceManager.SortPriorityList();
}

const SoundArchive& SoundArchivePlayer::GetSoundArchive() const {
    return *mSoundArchive;
}

SoundPlayer& SoundArchivePlayer::GetSoundPlayer(u32 idx) {
    SoundPlayer* players = *(SoundPlayer**)((u8*)this + 0x40);
    return players[idx];
}

const void* SoundArchivePlayer::detail_GetFileAddress(u32 id) const {
    if (mFileManager != NULL) {
        const void* pAddr = mFileManager->GetFileAddress(id);
        if (pAddr != NULL) {
            return pAddr;
        }
    }

    const void* pAddr = mSoundArchive->detail_GetFileAddress(id);
    if (pAddr != NULL) {
        return pAddr;
    }

    SoundArchive::FileInfo file;
    if (!mSoundArchive->detail_ReadFileInfo(id, &file)) {
        return NULL;
    }

    for (unsigned int i = 0; i < file.filePosCount; i++) {
        SoundArchive::FilePos pos;
        if (!mSoundArchive->detail_ReadFilePos(id, i, &pos)) {
            continue;
        }

        // GetGroupAddress, inlined
        const void* pGroup = NULL;
        if (mGroupTable != NULL && pos.groupId < mGroupTable->count) {
            pGroup = mGroupTable->items[pos.groupId].address;
        }

        if (pGroup == NULL) {
            continue;
        }

        SoundArchive::GroupItemInfo item;
        if (mSoundArchive->detail_ReadGroupItemInfo(id, pos.index, &item)) {
            return static_cast<const u8*>(pGroup) + item.offset;
        }
    }

    return NULL;
}

const void* SoundArchivePlayer::detail_GetFileWaveDataAddress(u32 id) const {
    if (mFileManager != NULL) {
        const void* pAddr = mFileManager->GetFileWaveDataAddress(id);
        if (pAddr != NULL) {
            return pAddr;
        }
    }

    const void* pAddr = mSoundArchive->detail_GetWaveDataFileAddress(id);
    if (pAddr != NULL) {
        return pAddr;
    }

    SoundArchive::FileInfo file;
    if (!mSoundArchive->detail_ReadFileInfo(id, &file)) {
        return NULL;
    }

    for (unsigned int i = 0; i < file.filePosCount; i++) {
        SoundArchive::FilePos pos;
        if (!mSoundArchive->detail_ReadFilePos(id, i, &pos)) {
            continue;
        }

        // GetGroupWaveDataAddress, inlined
        const void* pGroup = NULL;
        if (mGroupTable != NULL && pos.groupId < mGroupTable->count) {
            pGroup = mGroupTable->items[pos.groupId].waveDataAddress;
        }

        if (pGroup == NULL) {
            continue;
        }

        SoundArchive::GroupItemInfo item;
        if (mSoundArchive->detail_ReadGroupItemInfo(id, pos.index, &item)) {
            return static_cast<const u8*>(pGroup) + item.waveDataOffset;
        }
    }

    return NULL;
}

const void* SoundArchivePlayer::GetGroupAddress(u32 id) const {
    if (mGroupTable == NULL) {
        return NULL;
    }

    if (id >= mGroupTable->count) {
        return NULL;
    }

    return mGroupTable->items[id].address;
}

void SoundArchivePlayer::SetGroupAddress(u32 id, const void* pAddr) {
    if (mGroupTable == NULL) {
        return;
    }

    // @bug Index not validated
    mGroupTable->items[id].address = pAddr;
}

const void* SoundArchivePlayer::GetGroupWaveDataAddress(u32 id) const {
    if (mGroupTable == NULL) {
        return NULL;
    }

    if (id >= mGroupTable->count) {
        return NULL;
    }

    return mGroupTable->items[id].waveDataAddress;
}

void SoundArchivePlayer::SetGroupWaveDataAddress(u32 id, const void* pAddr) {
    if (mGroupTable == NULL) {
        return;
    }

    // @bug Index not validated
    mGroupTable->items[id].waveDataAddress = pAddr;
}

SoundStartable::StartResult SoundArchivePlayer::detail_SetupSound(
    SoundHandle* pHandle, u32 id, bool hold, const StartInfo* pStartInfo) {
    return detail_SetupSoundImpl(pHandle, id, NULL, NULL, hold, pStartInfo);
}

SoundStartable::StartResult SoundArchivePlayer::detail_SetupSoundImpl(
    SoundHandle* pHandle, u32 id, detail::BasicSound::AmbientInfo* pArgInfo,
    SoundActor* pActor, bool hold, const StartInfo* pStartInfo) {

    if (!IsAvailable()) {
        return SoundStartable::START_ERR_NOT_AVAILABLE;
    }

    if (pHandle->IsAttachedSound()) {
        pHandle->DetachSound();
    }

    SoundArchive::SoundInfo sndInfo;
    if (!mSoundArchive->ReadSoundInfo(id, &sndInfo)) {
        return SoundStartable::START_ERR_INVALID_SOUNDID;
    }

    u32 playerId = sndInfo.playerId;
    int playerPriority = sndInfo.playerPriority;

    SoundStartable::StartInfo::StartOffsetType startType =
        SoundStartable::StartInfo::START_OFFSET_TYPE_MILLISEC;

    int startOffset = 0;

    if (pStartInfo != NULL) {
        if (pStartInfo->enableFlag &
            SoundStartable::StartInfo::ENABLE_START_OFFSET) {
            startType = pStartInfo->startOffsetType;
            startOffset = pStartInfo->startOffset;
        }

        if (pStartInfo->enableFlag &
            SoundStartable::StartInfo::ENABLE_PLAYER_ID) {
            playerId = pStartInfo->playerId;
        }

        if (pStartInfo->enableFlag &
            SoundStartable::StartInfo::ENABLE_PLAYER_PRIORITY) {
            playerPriority = pStartInfo->playerPriority;
        }
    }

    int playerPriorityStart = hold ? playerPriority - 1 : playerPriority;

    SoundPlayer& rPlayer = GetSoundPlayer(playerId);
    detail::BasicSound* pSound = NULL;

    switch (mSoundArchive->GetSoundType(id)) {
    case SOUND_TYPE_SEQ: {
        SoundArchive::SeqSoundInfo seqInfo;
        if (!mSoundArchive->detail_ReadSeqSoundInfo(id, &seqInfo)) {
            return SoundStartable::START_ERR_INVALID_SOUNDID;
        }

        detail::SeqSound* pSeqSound = rPlayer.detail_AllocSeqSound(
            playerPriority, playerPriorityStart, pArgInfo, pActor, id,
            &mSeqSoundInstanceManager);

        if (pSeqSound == NULL) {
            return SoundStartable::START_ERR_LOW_PRIORITY;
        }

        pSeqSound->SetId(id);

        const void* pFileAddress = detail_GetFileAddress(sndInfo.fileId);

        SoundStartable::StartResult result =
            PrepareSeqImpl__Q34nw4r3snd18SoundArchivePlayerFPQ44nw4r3snd6detail8SeqSoundPCQ44nw4r3snd12SoundArchive9SoundInfoPCQ44nw4r3snd12SoundArchive12SeqSoundInfoQ54nw4r3snd14SoundStartable9StartInfo15StartOffsetTypeiPCvPCc(
                this, pSeqSound, &sndInfo, &seqInfo, startType, startOffset,
                pFileAddress, NULL);

        if (result != SoundStartable::START_SUCCESS) {
            pSeqSound->Shutdown();
            return result;
        }

        pSound = pSeqSound;
        break;
    }

    case SOUND_TYPE_STRM: {
        SoundArchive::StrmSoundInfo strmInfo;
        if (!mSoundArchive->detail_ReadStrmSoundInfo(id, &strmInfo)) {
            return SoundStartable::START_ERR_INVALID_SOUNDID;
        }

        detail::StrmSound* pStrmSound = rPlayer.detail_AllocStrmSound(
            playerPriority, playerPriorityStart, pArgInfo, pActor, id,
            &mStrmSoundInstanceManager);

        if (pStrmSound == NULL) {
            return SoundStartable::START_ERR_LOW_PRIORITY;
        }

        pStrmSound->SetId(id);

        SoundStartable::StartResult result = PrepareStrmImpl(
            pStrmSound, &sndInfo, &strmInfo, startType, startOffset, 1);

        if (result != SoundStartable::START_SUCCESS) {
            pStrmSound->Shutdown();
            return result;
        }

        pSound = pStrmSound;
        break;
    }

    case SOUND_TYPE_WAVE: {
        SoundArchive::WaveSoundInfo waveInfo;
        if (!mSoundArchive->detail_ReadWaveSoundInfo(id, &waveInfo)) {
            return SoundStartable::START_ERR_INVALID_SOUNDID;
        }

        detail::WaveSound* pWaveSound = rPlayer.detail_AllocWaveSound(
            playerPriority, playerPriorityStart, pArgInfo, pActor, id,
            &mWaveSoundInstanceManager);

        if (pWaveSound == NULL) {
            return SoundStartable::START_ERR_LOW_PRIORITY;
        }

        pWaveSound->SetId(id);

        SoundStartable::StartResult result = PrepareWaveSoundImpl(
            pWaveSound, &sndInfo, &waveInfo, startType, startOffset, 1);

        if (result != SoundStartable::START_SUCCESS) {
            pWaveSound->Shutdown();
            return result;
        }

        pSound = pWaveSound;
        break;
    }

    default: {
        return SoundStartable::START_ERR_INVALID_SOUNDID;
    }
    }

    pHandle->detail_AttachSound(pSound);
    return SoundStartable::START_SUCCESS;
}

namespace {

// Retail member offsets for the SoundArchivePlayer fields this function
// touches; the shared header layout is not byte-compatible (see also
// GetSoundPlayer below).
struct SapFields {
    char field_0x0[0x10];
    /*0x10*/ const SoundArchive* mSoundArchive;
    char field_0x14[0x0C];
    /*0x20*/ char mSeqCallback[8]; // SeqNoteOnCallback (vtable + player ref)
    char field_0x28[0x08];
    detail::SeqTrackAllocator* mSeqTrackAllocator;   // at 0x30
    void (*mSeqUserprocCallback)(u16, void*, void*); // at 0x34
    void* mSeqUserprocData;                          // at 0x38
    u32 mSoundPlayerCount;                           // at 0x3C
    SoundPlayer* mSoundPlayers;                      // at 0x40
};

// Retail SoundArchive::SoundInfo layout (an extra field at 0x8 shifts the
// tail by 4; see SoundInfoLayout in snd_SoundArchiveFile.cpp).
struct SoundInfoLayout {
    u32 fileId;         // at 0x0
    u32 playerId;       // at 0x4
    int field_0x8;      // at 0x8
    int playerPriority; // at 0xC
    int volume;         // at 0x10
    int remoteFilter;   // at 0x14
    PanMode panMode;    // at 0x18
    PanCurve panCurve;  // at 0x1C
};

} // namespace

// Imports resolved in other TUs (retail symbol names; the shared headers do
// not declare these shapes).
extern const double lbl_eu_8066A050;
extern const float lbl_eu_8066A048;
extern "C" bool ReadOffsetByLabel__Q44nw4r3snd6detail13SeqFileReaderCFPCcPUl(
    const detail::SeqFileReader* pReader, const char* pLabel, u32* pOffset);
extern "C" u32 ParseAllocTrack__Q44nw4r3snd6detail9MmlParserFPCvUlPUl(
    const void* pBuffer, u32 offset, u32* pSize);
extern "C" detail::SeqPlayer::SetupResult
Setup__Q44nw4r3snd6detail8SeqSoundFPQ44nw4r3snd6detail17SeqTrackAllocatorUlPQ44nw4r3snd6detail14NoteOnCallback(
    detail::SeqSound* pSound, detail::SeqTrackAllocator* pAllocator,
    u32 allocTrackFlags, detail::NoteOnCallback* pCallback);
extern "C" void
SetSeqUserprocCallback__Q44nw4r3snd6detail8SeqSoundFPFUsPQ34nw4r3snd24SeqUserprocCallbackParamPv_vPv(
    detail::SeqSound* pSound, void (*pCallback)(u16, void*, void*),
    void* pUserData);

extern "C" SoundStartable::StartResult PrepareSeqImpl__Q34nw4r3snd18SoundArchivePlayerFPQ44nw4r3snd6detail8SeqSoundPCQ44nw4r3snd12SoundArchive9SoundInfoPCQ44nw4r3snd12SoundArchive12SeqSoundInfoQ54nw4r3snd14SoundStartable9StartInfo15StartOffsetTypeiPCvPCc(
    SoundArchivePlayer* thisPtr, detail::SeqSound* pSound,
    const SoundArchive::SoundInfo* pSndInfo,
    const SoundArchive::SeqSoundInfo* pSeqInfo,
    SoundStartable::StartInfo::StartOffsetType startType, int startOffset,
    const void* pSeqBin, const char* pLabelStr) {

    SapFields& rFields = *reinterpret_cast<SapFields*>(thisPtr);
    const SoundInfoLayout& rInfo =
        *reinterpret_cast<const SoundInfoLayout*>(pSndInfo);

    // The instance-manager template reads its list/mutex relative to a base
    // that sits 4 bytes below the retail slot.
    detail::SoundInstanceManager<detail::SeqSound>* pMgr =
        reinterpret_cast<detail::SoundInstanceManager<detail::SeqSound>*>(
            reinterpret_cast<char*>(thisPtr) + 0x44);

    u32 allocTrack = pSeqInfo->allocTrack;
    u32 seqDataOffset = pSeqInfo->dataOffset;
    const void* pFileAddress = pSeqBin;
    ut::FileStream* pFileStream = NULL;

    if (pSeqBin != NULL) {
        detail::SeqFileReader reader(pSeqBin);

        if (pLabelStr != NULL) {
            // Resolve the start position from a label inside the sequence
            // data and derive the track allocation mask from it.
            if (!ReadOffsetByLabel__Q44nw4r3snd6detail13SeqFileReaderCFPCcPUl(
                    &reader, pLabelStr, &seqDataOffset)) {
                return SoundStartable::
                    START_ERR_INVALID_SEQ_START_LOCATION_LABEL;
            }

            seqDataOffset =
                ParseAllocTrack__Q44nw4r3snd6detail9MmlParserFPCvUlPUl(
                    reader.GetBaseAddress(), seqDataOffset, &allocTrack);
        }
    } else {
        pFileAddress = thisPtr->detail_GetFileAddress(rInfo.fileId);

        if (pLabelStr != NULL) {
            detail::SeqFileReader reader(pFileAddress);

            if (!ReadOffsetByLabel__Q44nw4r3snd6detail13SeqFileReaderCFPCcPUl(
                    &reader, pLabelStr, &seqDataOffset)) {
                return SoundStartable::
                    START_ERR_INVALID_SEQ_START_LOCATION_LABEL;
            }
        }
    }

    // Sequence data not memory-resident: stream it through the sound's own
    // file stream buffer backed by the player heap.
    if (pFileAddress == NULL) {
        detail::PlayerHeap* pHeap = pSound->GetPlayerHeap();
        if (pHeap == NULL) {
            return SoundStartable::START_ERR_NOT_DATA_LOADED;
        }

        pFileStream = rFields.mSoundArchive->detail_OpenFileStream(
            rInfo.fileId, pSound->GetFileStreamBuffer(),
            pSound->GetFileStreamBufferSize());

        if (pFileStream == NULL) {
            return SoundStartable::START_ERR_CANNOT_OPEN_FILE;
        }

        if (pHeap->GetFreeSize() < pFileStream->GetSize()) {
            pFileStream->Close();
            return SoundStartable::START_ERR_NOT_ENOUGH_PLAYER_HEAP;
        }
    }

    detail::SeqPlayer::SetupResult result =
        Setup__Q44nw4r3snd6detail8SeqSoundFPQ44nw4r3snd6detail17SeqTrackAllocatorUlPQ44nw4r3snd6detail14NoteOnCallback(
            pSound, rFields.mSeqTrackAllocator, allocTrack,
            reinterpret_cast<detail::NoteOnCallback*>(rFields.mSeqCallback));

    // Retry while tracks cannot be allocated: drop the lowest-priority
    // sound until either we fit or this sound is itself the drop target.
    while (result != detail::SeqPlayer::SETUP_SUCCESS) {
        if (result != detail::SeqPlayer::SETUP_ERR_CANNOT_ALLOCATE_TRACK) {
            return SoundStartable::START_ERR_UNKNOWN;
        }

        if (pMgr->GetActiveCount() == 1) {
            if (pFileStream != NULL) {
                pFileStream->Close();
            }
            return SoundStartable::START_ERR_NOT_ENOUGH_INSTANCE;
        }

        detail::BasicSound* pDropSound = pMgr->GetLowestPrioritySound();

        if (pDropSound == pSound) {
            if (pFileStream != NULL) {
                pFileStream->Close();
            }
            return SoundStartable::START_ERR_NOT_ENOUGH_INSTANCE;
        }

        pDropSound->Stop(0);
        result =
            Setup__Q44nw4r3snd6detail8SeqSoundFPQ44nw4r3snd6detail17SeqTrackAllocatorUlPQ44nw4r3snd6detail14NoteOnCallback(
                pSound, rFields.mSeqTrackAllocator, allocTrack,
                reinterpret_cast<detail::NoteOnCallback*>(
                    rFields.mSeqCallback));
    }

    // Volume goes through MWCC's integer-to-double magic-constant
    // conversion before the single-precision divide by 127.
    double volume = rInfo.volume - lbl_eu_8066A050;
    pSound->SetInitialVolume(volume / lbl_eu_8066A048);
    pSound->SetRemoteFilter(rInfo.remoteFilter);
    pSound->SetPanMode(rInfo.panMode);
    pSound->SetPanCurve(rInfo.panCurve);
    pSound->SetChannelPriority(pSeqInfo->channelPriority);
    pSound->SetReleasePriorityFix(pSeqInfo->releasePriorityFixFlag);
    SetSeqUserprocCallback__Q44nw4r3snd6detail8SeqSoundFPFUsPQ34nw4r3snd24SeqUserprocCallbackParamPv_vPv(
        pSound, rFields.mSeqUserprocCallback, rFields.mSeqUserprocData);

    detail::SeqPlayer::OffsetType seqOffsetType;

    switch (startType) {
    case SoundStartable::StartInfo::START_OFFSET_TYPE_MILLISEC: {
        seqOffsetType = detail::SeqPlayer::OFFSET_TYPE_MILLISEC;
        break;
    }

    case SoundStartable::StartInfo::START_OFFSET_TYPE_TICK: {
        seqOffsetType = detail::SeqPlayer::OFFSET_TYPE_TICK;
        break;
    }

    case SoundStartable::StartInfo::START_OFFSET_TYPE_SAMPLE: {
        seqOffsetType = detail::SeqPlayer::OFFSET_TYPE_TICK;
        startOffset = 0;
        break;
    }

    default: {
        seqOffsetType = detail::SeqPlayer::OFFSET_TYPE_TICK;
        startOffset = 0;
        break;
    }
    }

    if (pFileAddress != NULL) {
        detail::SeqFileReader reader(pFileAddress);
        pSound->Prepare(reader.GetBaseAddress(), seqDataOffset, seqOffsetType,
                        startOffset);
    } else {
        pSound->Prepare(pFileStream, seqDataOffset, seqOffsetType,
                        startOffset);
    }

    return SoundStartable::START_SUCCESS;
}

SoundStartable::StartResult SoundArchivePlayer::PrepareStrmImpl(
    detail::StrmSound* pSound, const SoundArchive::SoundInfo* pSndInfo,
    const SoundArchive::StrmSoundInfo* pStrmInfo,
    SoundStartable::StartInfo::StartOffsetType startType, int startOffset,
    int voices) {

    // StrmSoundInfo is empty in this version of NW4R
#pragma unused(pStrmInfo)

    detail::StrmPlayer::StartOffsetType strmOffsetType;

    switch (startType) {
    case SoundStartable::StartInfo::START_OFFSET_TYPE_MILLISEC: {
        strmOffsetType = detail::StrmPlayer::START_OFFSET_TYPE_MILLISEC;
        break;
    }

    case SoundStartable::StartInfo::START_OFFSET_TYPE_TICK: {
        strmOffsetType = detail::StrmPlayer::START_OFFSET_TYPE_SAMPLE;
        startOffset = 0;
        break;
    }

    case SoundStartable::StartInfo::START_OFFSET_TYPE_SAMPLE: {
        strmOffsetType = detail::StrmPlayer::START_OFFSET_TYPE_SAMPLE;
        break;
    }

    default: {
        strmOffsetType = detail::StrmPlayer::START_OFFSET_TYPE_SAMPLE;
        startOffset = 0;
        break;
    }
    }

    void* pStreamBuffer = pSound->GetFileStreamBuffer();
    u32 streamBufferSize = pSound->GetFileStreamBufferSize();

    ut::FileStream* pFileStream = mSoundArchive->detail_OpenFileStream(
        pSndInfo->fileId, pStreamBuffer, streamBufferSize);

    if (pFileStream == NULL) {
        return SoundStartable::START_ERR_CANNOT_OPEN_FILE;
    }

    if (!pSound->Setup(&mStrmBufferPool, voices, 0)) {
        return SoundStartable::START_ERR_UNKNOWN;
    }
    if (!pSound->Prepare(strmOffsetType, startOffset, pFileStream)) {
        return SoundStartable::START_ERR_UNKNOWN;
    }

    pSound->SetInitialVolume(pSndInfo->volume / 127.0f);
    pSound->SetRemoteFilter(pSndInfo->remoteFilter);
    pSound->SetPanMode(pSndInfo->panMode);
    pSound->SetPanCurve(pSndInfo->panCurve);

    return SoundStartable::START_SUCCESS;
}

SoundStartable::StartResult SoundArchivePlayer::PrepareWaveSoundImpl(
    detail::WaveSound* pSound, const SoundArchive::SoundInfo* pSndInfo,
    const SoundArchive::WaveSoundInfo* pWsdInfo,
    SoundStartable::StartInfo::StartOffsetType startType, int startOffset,
    int voices) {

    const void* pWsdBin = detail_GetFileAddress(pSndInfo->fileId);
    if (pWsdBin == NULL) {
        return SoundStartable::START_ERR_NOT_DATA_LOADED;
    }

    detail::WsdPlayer::StartOffsetType wsdOffsetType;

    switch (startType) {
    case SoundStartable::StartInfo::START_OFFSET_TYPE_MILLISEC: {
        wsdOffsetType = detail::WsdPlayer::START_OFFSET_TYPE_MILLISEC;
        break;
    }

    case SoundStartable::StartInfo::START_OFFSET_TYPE_TICK: {
        wsdOffsetType = detail::WsdPlayer::START_OFFSET_TYPE_SAMPLE;
        startOffset = 0;
        break;
    }

    case SoundStartable::StartInfo::START_OFFSET_TYPE_SAMPLE: {
        wsdOffsetType = detail::WsdPlayer::START_OFFSET_TYPE_SAMPLE;
        break;
    }

    default: {
        wsdOffsetType = detail::WsdPlayer::START_OFFSET_TYPE_SAMPLE;
        startOffset = 0;
        break;
    }
    }

    if (!pSound->Prepare(pWsdBin, pWsdInfo->subNo, wsdOffsetType, startOffset,
                         voices, &mWsdCallback, pSndInfo->fileId)) {
        return SoundStartable::START_ERR_UNKNOWN;
    }

    pSound->SetInitialVolume(pSndInfo->volume / 127.0f);
    pSound->SetChannelPriority(pWsdInfo->channelPriority);
    pSound->SetReleasePriorityFix(pWsdInfo->releasePriorityFixFlag);
    pSound->SetRemoteFilter(pSndInfo->remoteFilter);
    pSound->SetPanMode(pSndInfo->panMode);
    pSound->SetPanCurve(pSndInfo->panCurve);

    return SoundStartable::START_SUCCESS;
}

bool SoundArchivePlayer::LoadGroup(u32 id, SoundMemoryAllocatable* pAllocatable,
                                   u32 blockSize) {
    if (!IsAvailable()) {
        return false;
    }

    if (id >= mSoundArchive->GetGroupCount()) {
        return false;
    }

    if (GetGroupAddress(id) != NULL) {
        return true;
    }

    if (pAllocatable == NULL) {
        return false;
    }

    detail::SoundArchiveLoader loader(*mSoundArchive);

    void* pWaveBuffer;
    const void* pGroup =
        loader.LoadGroup(id, pAllocatable, &pWaveBuffer, blockSize);

    if (pGroup == NULL) {
        return NULL;
    }

    SetGroupAddress(id, pGroup);
    SetGroupWaveDataAddress(id, pWaveBuffer);

    return true;
}

bool SoundArchivePlayer::LoadGroup(const char* pLabel,
                                   SoundMemoryAllocatable* pAllocatable,
                                   u32 blockSize) {
    u32 id = mSoundArchive->ConvertLabelStringToGroupId(pLabel);
    if (id == SoundArchive::INVALID_ID) {
        return false;
    }

    return LoadGroup(id, pAllocatable, blockSize);
}

// Retail walks two GroupTable-shaped slots here: mGroupTable plus the
// secondary table kept in the adjacent pointer slot. Both entries are
// {address, waveDataAddress} pairs guarded by a leading count.

void SoundArchivePlayer::InvalidateData(const void* pStart, const void* pEnd) {
    if (reinterpret_cast<GroupTable*>(mFileManager) != NULL) {
        for (int i = 0;
             i < reinterpret_cast<GroupTable*>(mFileManager)->count; i++) {
            const void* pAddr =
                reinterpret_cast<GroupTable*>(mFileManager)->items[i].address;

            if (pStart <= pAddr && pAddr <= pEnd) {
                reinterpret_cast<GroupTable*>(mFileManager)->items[i].address =
                    NULL;
            }
        }
    }

    if (mGroupTable == NULL) {
        return;
    }

    for (int i = 0; i < mGroupTable->count; i++) {
        const void* pAddr = mGroupTable->items[i].address;

        if (pStart <= pAddr && pAddr <= pEnd) {
            mGroupTable->items[i].address = NULL;
        }
    }
}

void SoundArchivePlayer::InvalidateWaveData(const void* pStart,
                                            const void* pEnd) {
    if (reinterpret_cast<GroupTable*>(mFileManager) != NULL) {
        for (int i = 0;
             i < reinterpret_cast<GroupTable*>(mFileManager)->count; i++) {
            const void* pAddr =
                reinterpret_cast<GroupTable*>(mFileManager)
                    ->items[i].waveDataAddress;

            if (pStart <= pAddr && pAddr <= pEnd) {
                reinterpret_cast<GroupTable*>(mFileManager)
                    ->items[i].waveDataAddress = NULL;
            }
        }
    }

    if (mGroupTable == NULL) {
        return;
    }

    for (int i = 0; i < mGroupTable->count; i++) {
        const void* pAddr = mGroupTable->items[i].waveDataAddress;

        if (pStart <= pAddr && pAddr <= pEnd) {
            mGroupTable->items[i].waveDataAddress = NULL;
        }
    }
}

detail::Channel*
SoundArchivePlayer::SeqNoteOnCallback::NoteOn(detail::SeqPlayer* pSeqPlayer,
                                              int bankNo,
                                              const detail::NoteOnInfo& rInfo) {
#pragma unused(bankNo)

    if (!mSoundArchivePlayer.IsAvailable()) {
        return NULL;
    }

    const SoundArchive& rArchive = mSoundArchivePlayer.GetSoundArchive();
    u32 soundId = pSeqPlayer->GetId();

    SoundArchive::SeqSoundInfo seqInfo;
    if (!rArchive.detail_ReadSeqSoundInfo(soundId, &seqInfo)) {
        return NULL;
    }

    SoundArchive::BankInfo bankInfo;
    if (!rArchive.detail_ReadBankInfo(seqInfo.bankId, &bankInfo)) {
        return NULL;
    }

    const void* pBankBin =
        mSoundArchivePlayer.detail_GetFileAddress(bankInfo.fileId);

    if (pBankBin == NULL) {
        return NULL;
    }

    detail::Bank bank(pBankBin);

    const void* pWaveData =
        mSoundArchivePlayer.detail_GetFileWaveDataAddress(bankInfo.fileId);

    if (pWaveData == NULL) {
        return NULL;
    }

    bank.SetWaveDataAddress(pWaveData);
    return bank.NoteOn(rInfo);
}

bool SoundArchivePlayer::WsdCallback::GetWaveSoundData(
    detail::WaveSoundInfo* pSoundInfo, detail::WaveSoundNoteInfo* pNoteInfo,
    detail::WaveData* pWaveData, const void* pWsdData, int index, int noteIndex,
    u32 callbackArg) const {

    u32 fileId = callbackArg;

    if (!mSoundArchivePlayer.IsAvailable()) {
        return false;
    }

    const void* pWaveAddr =
        mSoundArchivePlayer.detail_GetFileWaveDataAddress(fileId);

    if (pWaveAddr == NULL) {
        return false;
    }

    detail::WsdFileReader reader(pWsdData);

    if (!reader.ReadWaveSoundInfo(pSoundInfo, index)) {
        return false;
    }

    if (!reader.ReadWaveSoundNoteInfo(pNoteInfo, index, noteIndex)) {
        return false;
    }

    // This build reads the wave entry through ReadWaveInfo rather than
    // ReadWaveParam; the wave data buffer rides along as its output pointer.
    return reader.ReadWaveInfo(pNoteInfo->waveIndex,
                               reinterpret_cast<detail::WaveInfo*>(pWaveData),
                               pWaveAddr);
}

} // namespace snd
} // namespace nw4r

