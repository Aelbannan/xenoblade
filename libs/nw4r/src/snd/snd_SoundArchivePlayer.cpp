// Pre-include everything snd_SoundArchivePlayer.h needs so the instance-
// manager member type can be renamed for this TU only (see below).
#include <nw4r/types_nw4r.h>
#include <nw4r/ut.h>

#include <nw4r/snd/snd_DisposeCallback.h>
#include <nw4r/snd/snd_DisposeCallbackManager.h>
#include <nw4r/snd/snd_InstancePool.h>
#include <nw4r/snd/snd_MmlParser.h>
#include <nw4r/snd/snd_MmlSeqTrackAllocator.h>
#include <nw4r/snd/snd_MmlSeqTrack.h>
#include <nw4r/snd/snd_NoteOnCallback.h>
#include <nw4r/snd/snd_SeqSound.h>
// Retail SoundInfo carries an extra field at 0xC (volume sits at 0x10, not
// 0xC as the shared header declares). Inject it while parsing.
#define playerPriority playerPriority_; int field_0x0C
#include <nw4r/snd/snd_SoundArchive.h>
#undef playerPriority
#include <nw4r/snd/snd_SoundArchiveLoader.h>
#include <nw4r/snd/snd_SoundHandle.h>
#include <nw4r/snd/snd_BasicSound.h>
#include <nw4r/snd/snd_SoundActor.h>
#include <nw4r/snd/snd_SoundPlayer.h>
#include <nw4r/snd/snd_SoundInstanceManager.h>
#include <nw4r/snd/snd_SoundStartable.h>
#include <nw4r/snd/snd_StrmChannel.h>
#include <nw4r/snd/snd_StrmSound.h>
#include <nw4r/snd/snd_Util.h>
#include <nw4r/snd/snd_SeqFile.h>
#include <nw4r/snd/snd_Bank.h>
#include <nw4r/snd/snd_WaveFile.h>
#include <nw4r/snd/snd_WaveSound.h>
#include <nw4r/snd/snd_WsdPlayer.h>

#include <revolution/OS.h>

namespace nw4r {
namespace snd {
namespace detail {

// TU-local stand-in for WsdPlayer: the nested name is preserved so override
// symbols keep their retail mangling while the shared-header callback stays a
// bare vtable-pointer base (reference lands at 0x4, as in retail).
struct WsdPlayer_X : WsdPlayer {
    struct WsdCallback : ::nw4r::snd::detail::WsdPlayer::WsdCallback {};
};

} // namespace detail
} // namespace snd
} // namespace nw4r

// Rename the base while parsing the player header so the WsdCallback member
// picks up the padded TU-local layout above.
#define WsdPlayer WsdPlayer_X
// Rewrite CreatePlayerHeap to its retail signature while parsing the player
// header (the shared header still carries the upstream two-parameter shape).
#define CreatePlayerHeap(unused1, unused2) \
    CreatePlayerHeap(void** ppBuffer, void* pEnd, u32 bufferSize)
// Retail layout carries three extra pointer slots between mSoundPlayers and
// mSoundPlayerCount (everything from the instance managers onward sits 12
// bytes higher than the shared header declares). Inject them while parsing.
#define mSoundPlayers mSoundPlayers_; void* field_0x38; void* field_0x3C; void* field_0x40
// The retail-named PrepareWaveSoundImpl entry point below is a free function
// (locked header carries a stale 'voices' parameter), so open access to the
// private members while parsing the class definition.
#define private public
#include <nw4r/snd/snd_SoundArchivePlayer.h>
#undef private
#undef CreatePlayerHeap
#undef mSoundPlayers
#undef WsdPlayer

// Retail-named entry points (see definitions further below for why they are
// free functions with C linkage).
extern "C" nw4r::snd::SoundStartable::StartResult
PrepareWaveSoundImpl__Q34nw4r3snd18SoundArchivePlayerFPQ44nw4r3snd6detail9WaveSoundPCQ44nw4r3snd12SoundArchive9SoundInfoPCQ44nw4r3snd12SoundArchive13WaveSoundInfoQ54nw4r3snd14SoundStartable9StartInfo15StartOffsetTypei(
    nw4r::snd::SoundArchivePlayer* self, nw4r::snd::detail::WaveSound* pSound,
    const nw4r::snd::SoundArchive::SoundInfo* pSndInfo,
    const nw4r::snd::SoundArchive::WaveSoundInfo* pWsdInfo,
    nw4r::snd::SoundStartable::StartInfo::StartOffsetType startType,
    int startOffset);

// Retail nw4r_data.s pool constants (plain linker labels; declared at global
// scope so the names are not mangled into the nw4r::snd namespace).
extern const double lbl_eu_8066A050;
extern const float lbl_eu_8066A048;

// Import from snd_SoundArchive.cpp (retail symbol; not declared in snd_SoundArchive.h)
extern "C" u32 detail_GetFileCount__Q34nw4r3snd12SoundArchiveCFv(
    const nw4r::snd::SoundArchive* self);

// Import from snd_SoundSystem.cpp (retail symbol; SoundSystem.h does not
// declare the query and is outside this session's writable scope).
extern "C" bool IsInitializedSoundSystem__Q34nw4r3snd11SoundSystemFv();

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
      mSeqTrackAllocator(NULL),
      mSeqCallback(*this),
      mWsdCallback(*this),
      mSoundPlayers_(NULL),
      field_0x38(NULL),
      field_0x3C(NULL),
      field_0x40(NULL),
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
    if (!IsInitializedSoundSystem__Q34nw4r3snd11SoundSystemFv()) {
        return false;
    }

    if (!SetupMram(pArchive, pMramBuffer, mramBufferSize)) {
        return false;
    }

    // SetupStrmBuffer + GetRequiredStrmBufferSize were inlined by MWCC into
    // a single body whose result lands in one bool: an info read for the
    // size check, then another for the channel count handed to the pool.
    int channels = 0;

    SoundArchive::SoundArchivePlayerInfo reqInfo;
    if (pArchive->ReadSoundArchivePlayerInfo(&reqInfo)) {
        channels = reqInfo.strmChannelCount;
    }

    bool result;

    if (strmBufferSize < static_cast<u32>(channels) * 0xA000) {
        result = false;
    } else {
        channels = 0;

        SoundArchive::SoundArchivePlayerInfo info;
        if (pArchive->ReadSoundArchivePlayerInfo(&info)) {
            channels = info.strmChannelCount;
        }

        // Retail member offsets differ from the shared-header layout here:
        // the stream pool lives at 0xC8 and the allocator pointer slot at
        // 0x30.
        reinterpret_cast<detail::StrmBufferPool*>(
            reinterpret_cast<char*>(this) + 0xC8)
            ->Setup(pStrmBuffer, strmBufferSize, channels);

        result = true;
    }

    if (!result) {
        return false;
    }

    *reinterpret_cast<detail::SeqTrackAllocator**>(
        reinterpret_cast<char*>(this) + 0x30) =
        reinterpret_cast<detail::SeqTrackAllocator*>(
            reinterpret_cast<char*>(this) + 0xBC);

    return true;
}

// TU-local stand-in for SoundInstanceManager with public members; layout
// matches the real manager (pool 0x00, priority list 0x04, mutex 0x10).
template <typename T> class SapInstanceManager {
public:
    void Destroy(void* pBuffer, u32 size) {
        ut::detail::AutoLock<OSMutex> lock(mMutex);
        mPool.Destroy(pBuffer, size);
    }

    // Retail headers over-allocate instance strides, so the pool Create
    // needs an explicit stride rather than sizeof(T).
    u32 Create(void* pBuffer, u32 size, u32 stride) {
        ut::detail::AutoLock<OSMutex> lock(mMutex);
        return reinterpret_cast<detail::PoolImpl*>(&mPool)
            ->CreateImpl(pBuffer, size, stride);
    }

    detail::MemoryPool<T> mPool; // at 0x0
    char field_0x4[0x0C];        // priority list (unused here)
    OSMutex mMutex;              // at 0x10
};

// Retail SoundArchivePlayer layout touched by Shutdown; the shared header
// member offsets are not byte-compatible.
struct SapShutdownLayout {
    char field_0x0[0x10];
    /*0x10*/ const SoundArchive* mSoundArchive;
    /*0x14*/ detail::Util::Table<SoundArchivePlayer::Group>* mGroupTable;
    /*0x18*/ SoundArchivePlayer_FileManager* mFileManager;
    /*0x1C*/ void* field_0x1C;
    char field_0x20[0x10];
    /*0x30*/ detail::SeqTrackAllocator* mSeqTrackAllocator;
    char field_0x34[0x08];
    /*0x3C*/ u32 mSoundPlayerCount;
    /*0x40*/ SoundPlayer* mSoundPlayers;
    /*0x44*/ SapInstanceManager<detail::SeqSound> mSeqSoundMgr;
    /*0x6C*/ SapInstanceManager<detail::StrmSound> mStrmSoundMgr;
    /*0x94*/ SapInstanceManager<detail::WaveSound> mWaveSoundMgr;
    char field_0xBC[0x28];
    /*0xE4*/ void* mSetupBufferAddress;
    /*0xE8*/ u32 mSetupBufferSize;
};

void SoundArchivePlayer::Shutdown() {
    SapShutdownLayout* pSelf = reinterpret_cast<SapShutdownLayout*>(this);

    pSelf->mSoundArchive = NULL;
    pSelf->mGroupTable = NULL;
    pSelf->mFileManager = NULL;
    pSelf->field_0x1C = NULL;
    pSelf->mSeqTrackAllocator = NULL;

    for (int i = 0; i < pSelf->mSoundPlayerCount; i++) {
        pSelf->mSoundPlayers[i].~SoundPlayer();
    }

    pSelf->mSoundPlayerCount = 0;
    pSelf->mSoundPlayers = NULL;

    // Retail member offsets for the stream pool / instance heaps differ
    // from the shared-header layout.
    reinterpret_cast<detail::StrmBufferPool*>(reinterpret_cast<char*>(this) +
                                              0xC8)
        ->Shutdown();

    // The stream buffer heap is only torn down when it was allocated.
    if (pSelf->mSetupBufferAddress != NULL) {
        pSelf->mSeqSoundMgr.Destroy(pSelf->mSetupBufferAddress,
                                    pSelf->mSetupBufferSize);
        pSelf->mStrmSoundMgr.Destroy(pSelf->mSetupBufferAddress,
                                     pSelf->mSetupBufferSize);
        pSelf->mWaveSoundMgr.Destroy(pSelf->mSetupBufferAddress,
                                     pSelf->mSetupBufferSize);

        reinterpret_cast<detail::MmlSeqTrackAllocator*>(
            reinterpret_cast<char*>(this) + 0xBC)
            ->Destroy(pSelf->mSetupBufferAddress, pSelf->mSetupBufferSize);

        pSelf->mSetupBufferAddress = NULL;
        pSelf->mSetupBufferSize = 0;
    }
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

// Single-call-site stage helpers: retail compiles each of these inlined
// into SetupMram (bounds-check first, then carve the block off *ppBuffer).
namespace {

bool SapSetupGroupAddressTable(SoundArchivePlayer* pPlayer,
                               const SoundArchive* pArchive, void** ppBuffer,
                               void* pEnd) {
    // clang-format off
    u32 requireSize =
        pArchive->GetGroupCount() * sizeof(SoundArchivePlayer::Group) +
        (sizeof(SoundArchivePlayer::GroupTable) -
         sizeof(SoundArchivePlayer::Group));
    // clang-format on

    void* pTableEnd =
        ut::RoundUp(ut::AddOffsetToPtr(*ppBuffer, requireSize), 4);

    if (ut::ComparePtr(pTableEnd, pEnd) > 0) {
        return false;
    }

    pPlayer->mGroupTable = static_cast<SoundArchivePlayer::GroupTable*>(*ppBuffer);
    *ppBuffer = pTableEnd;

    pPlayer->mGroupTable->count = pArchive->GetGroupCount();

    for (int i = 0; i < pPlayer->mGroupTable->count; i++) {
        pPlayer->mGroupTable->items[i].address = NULL;
        pPlayer->mGroupTable->items[i].waveDataAddress = NULL;
    }

    return true;
}

// File-level twin of the group table; retail keeps it in the slot the
// shared header names mFileManager.
bool SapSetupFileAddressTable(SoundArchivePlayer* pPlayer,
                              const SoundArchive* pArchive, void** ppBuffer,
                              void* pEnd) {
    // clang-format off
    u32 requireSize =
        detail_GetFileCount__Q34nw4r3snd12SoundArchiveCFv(pArchive) *
            sizeof(SoundArchivePlayer::Group) +
        (sizeof(SoundArchivePlayer::GroupTable) -
         sizeof(SoundArchivePlayer::Group));
    // clang-format on

    void* pTableEnd =
        ut::RoundUp(ut::AddOffsetToPtr(*ppBuffer, requireSize), 4);

    if (ut::ComparePtr(pTableEnd, pEnd) > 0) {
        return false;
    }

    reinterpret_cast<SoundArchivePlayer::GroupTable*&>(
        pPlayer->mFileManager) =
        static_cast<SoundArchivePlayer::GroupTable*>(*ppBuffer);
    *ppBuffer = pTableEnd;

    reinterpret_cast<SoundArchivePlayer::GroupTable*&>(
        pPlayer->mFileManager)
        ->count = detail_GetFileCount__Q34nw4r3snd12SoundArchiveCFv(
        pArchive);

    for (int i = 0;
         i < reinterpret_cast<SoundArchivePlayer::GroupTable*&>(
                 pPlayer->mFileManager)
                 ->count;
         i++) {
        reinterpret_cast<SoundArchivePlayer::GroupTable*&>(
            pPlayer->mFileManager)
            ->items[i].address = NULL;
        reinterpret_cast<SoundArchivePlayer::GroupTable*&>(
            pPlayer->mFileManager)
            ->items[i].waveDataAddress = NULL;
    }

    return true;
}

// Pool stages: the instance-manager Create locks its own mutex; the pool
// receives the raw byte count while the rounded size only drives the
// bounds check and the buffer-pointer advance.
bool SapSetupSeqSoundPool(SapInstanceManager<detail::SeqSound>* pMgr,
                          void** ppBuffer, void* pEnd, int sounds) {
    u32 requireSize = sounds * 0x4d0; // retail SeqSound stride (over-allocated)

    void* pSoundEnd =
        ut::RoundUp(ut::AddOffsetToPtr(*ppBuffer, requireSize), 4);

    if (ut::ComparePtr(pSoundEnd, pEnd) > 0) {
        return false;
    }

    pMgr->Create(*ppBuffer, requireSize, 0x4d0);
    *ppBuffer = pSoundEnd;

    return true;
}

bool SapSetupStrmSoundPool(SapInstanceManager<detail::StrmSound>* pMgr,
                           void** ppBuffer, void* pEnd, int sounds) {
    u32 requireSize = sounds * 0x10d0; // retail StrmSound stride

    void* pSoundEnd =
        ut::RoundUp(ut::AddOffsetToPtr(*ppBuffer, requireSize), 4);

    if (ut::ComparePtr(pSoundEnd, pEnd) > 0) {
        return false;
    }

    pMgr->Create(*ppBuffer, requireSize, 0x10d0);
    *ppBuffer = pSoundEnd;

    return true;
}

bool SapSetupWaveSoundPool(
    detail::SoundInstanceManager<detail::WaveSound>* pMgr, void** ppBuffer,
    void* pEnd, int sounds) {
    u32 requireSize = sounds * 0x250; // retail WaveSound stride

    void* pSoundEnd =
        ut::RoundUp(ut::AddOffsetToPtr(*ppBuffer, requireSize), 4);

    if (ut::ComparePtr(pSoundEnd, pEnd) > 0) {
        return false;
    }

    pMgr->Create(*ppBuffer, requireSize);
    *ppBuffer = pSoundEnd;

    return true;
}

bool SapSetupSeqTrackPool(detail::MmlSeqTrackAllocator* pAllocator,
                          void** ppBuffer, void* pEnd, int tracks) {
    u32 requireSize = tracks * 0xcc; // retail MmlSeqTrack stride

    void* pTrackEnd =
        ut::RoundUp(ut::AddOffsetToPtr(*ppBuffer, requireSize), 4);

    if (ut::ComparePtr(pTrackEnd, pEnd) > 0) {
        return false;
    }

    pAllocator->Create(*ppBuffer, requireSize);
    *ppBuffer = pTrackEnd;

    return true;
}

} // namespace

bool SoundArchivePlayer::SetupMram(const SoundArchive* pArchive, void* pBuffer,
                                   u32 bufferSize) {
    void* pPtr = pBuffer;
    void* pEnd = static_cast<u8*>(pBuffer) + bufferSize;

    if (!SetupSoundPlayer(pArchive, &pPtr, pEnd)) {
        return false;
    }

    if (!SapSetupGroupAddressTable(this, pArchive, &pPtr, pEnd)) {
        return false;
    }

    if (!SapSetupFileAddressTable(this, pArchive, &pPtr, pEnd)) {
        return false;
    }

    SoundArchive::SoundArchivePlayerInfo info;
    if (pArchive->ReadSoundArchivePlayerInfo(&info)) {
        if (!SapSetupSeqSoundPool(
                reinterpret_cast<SapInstanceManager<detail::SeqSound>*>(
                    &mSeqSoundInstanceManager),
                &pPtr, pEnd, info.seqSoundCount)) {
            return false;
        }

        if (!SapSetupStrmSoundPool(
                reinterpret_cast<SapInstanceManager<detail::StrmSound>*>(
                    &mStrmSoundInstanceManager),
                &pPtr, pEnd, info.strmSoundCount)) {
            return false;
        }

        if (!SapSetupWaveSoundPool(&mWaveSoundInstanceManager, &pPtr, pEnd,
                                   info.waveSoundCount)) {
            return false;
        }

        if (!SapSetupSeqTrackPool(&mMmlSeqTrackAllocator, &pPtr, pEnd,
                                  info.seqTrackCount)) {
            return false;
        }
    }

    mSoundArchive = pArchive;
    mSetupBufferAddress = pBuffer;
    mSetupBufferSize = bufferSize;

    return true;
}

// Retail member offsets for the SoundArchivePlayer fields touched by
// SetupSoundPlayer and PrepareSeqImpl; the shared header layout is not
// byte-compatible.
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

bool SoundArchivePlayer::SetupSoundPlayer(const SoundArchive* pArchive,
                                          void** ppBuffer, void* pEnd) {
    SapFields& rFields = *reinterpret_cast<SapFields*>(this);

    u32 playerCount = pArchive->GetPlayerCount();
    u32 requireSize = playerCount * sizeof(SoundPlayer);

    void* pPlayerEnd =
        ut::RoundUp(ut::AddOffsetToPtr(*ppBuffer, requireSize), 4);

    if (ut::ComparePtr(pPlayerEnd, pEnd) > 0) {
        return false;
    }

    u8* pPtr = static_cast<u8*>(*ppBuffer);
    *ppBuffer = pPlayerEnd;

    rFields.mSoundPlayers = reinterpret_cast<SoundPlayer*>(pPtr);
    rFields.mSoundPlayerCount = playerCount;

    for (u32 i = 0; i < playerCount; i++, pPtr += sizeof(SoundPlayer)) {
        SoundPlayer* pPlayer = new (pPtr) SoundPlayer();

        SoundArchive::PlayerInfo info;
        if (!pArchive->ReadPlayerInfo(i, &info)) {
            continue;
        }

        pPlayer->SetPlayableSoundCount(info.playableSoundCount);

        if (info.heapSize == 0) {
            continue;
        }

        // Heap space for each playable sound is carved off ppBuffer inside
        // CreatePlayerHeap (which owns the end-of-buffer check).
        for (int j = 0; j < info.playableSoundCount; j++) {
            detail::PlayerHeap* pHeap =
                CreatePlayerHeap(ppBuffer, pEnd, info.heapSize);

            if (pHeap == NULL) {
                return false;
            }

            pPlayer->detail_AppendPlayerHeap(pHeap);
        }

        pPlayer->detail_SetPlayableSoundLimit(info.playableSoundCount);
    }

    return true;
}

// Retail shape: bounds-checks against pEnd, then carves the heap off
// *ppBuffer and advances it. noinline keeps retail's out-of-line call from
// SetupSoundPlayer (-inline auto would otherwise fold this small body in).
// Retail shape: bounds-checks against pEnd, then carves the heap off
// *ppBuffer and advances it.
// Keep retail's out-of-line call shape: with -inline auto/-ipa file MWCC folds
// this single-call-site helper into SetupSoundPlayer.
#pragma push
#pragma auto_inline off
detail::PlayerHeap* SoundArchivePlayer::CreatePlayerHeap(
    void** ppBuffer, void* pEnd, u32 bufferSize) {
    void* pHeapEnd = ut::RoundUp(
        ut::AddOffsetToPtr(*ppBuffer, sizeof(detail::PlayerHeap) + bufferSize),
        4);

    if (ut::ComparePtr(pHeapEnd, pEnd) > 0) {
        return NULL;
    }

    void* pHeapBuffer = *ppBuffer;
    *ppBuffer = pHeapEnd;

    detail::PlayerHeap* pHeap = new (pHeapBuffer) detail::PlayerHeap();
    void* pBody = ut::AddOffsetToPtr(pHeapBuffer, sizeof(detail::PlayerHeap));

    if (!pHeap->Create(pBody, bufferSize)) {
        return NULL;
    }

    return pHeap;
}
#pragma pop

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

namespace {

// Retail SoundArchivePlayer layout touched by detail_GetFileAddress; the
// shared header lacks the file address table member at 0x18.
struct SapFileAddressLayout {
    u32 field_0x0[0x4];
    /*0x10*/ const SoundArchive* mSoundArchive;
    /*0x14*/ detail::Util::Table<SoundArchivePlayer::Group>* mGroupTable;
    /*0x18*/ detail::Util::Table<SoundArchivePlayer::Group>* mFileTable;
    /*0x1C*/ SoundArchivePlayer_FileManager* mFileManager;
};

// GetFileAddress-style table lookup, inlined at both call sites below.
inline const void* SapLookupTableAddress(
    const detail::Util::Table<SoundArchivePlayer::Group>* pTable, u32 id) {
    if (pTable == NULL) {
        return NULL;
    }
    if (id >= pTable->count) {
        return NULL;
    }
    return pTable->items[id].address;
}

// Wave-data twin of the lookup above.
inline const void* SapLookupTableWaveDataAddress(
    const detail::Util::Table<SoundArchivePlayer::Group>* pTable, u32 id) {
    if (pTable == NULL) {
        return NULL;
    }
    if (id >= pTable->count) {
        return NULL;
    }
    return pTable->items[id].waveDataAddress;
}

// Accessor for the retail layout fields touched by detail_GetFileAddress.
// Kept as a plain cast expression so the compiler schedules the member
// loads directly off the incoming this-pointer.
#define SAP_LAYOUT(pPlayer) \
    (reinterpret_cast<const SapFileAddressLayout*>(pPlayer))

} // namespace

const void* SoundArchivePlayer::detail_GetFileAddress(u32 id) const {
    if (SAP_LAYOUT(this)->mFileManager != NULL) {
        const void* pAddr = SAP_LAYOUT(this)->mFileManager->GetFileAddress(id);
        if (pAddr != NULL) {
            return pAddr;
        }
    }

    const void* pAddr =
        SAP_LAYOUT(this)->mSoundArchive->detail_GetFileAddress(id);
    if (pAddr != NULL) {
        return pAddr;
    }

    // Direct file address table lookup, keyed by fileId
    const void* pDirect = SapLookupTableAddress(SAP_LAYOUT(this)->mFileTable, id);
    if (pDirect != NULL) {
        return pDirect;
    }

    SoundArchive::FileInfo file;
    if (!SAP_LAYOUT(this)->mSoundArchive->detail_ReadFileInfo(id, &file)) {
        return NULL;
    }

    for (unsigned int i = 0; i < file.filePosCount; i++) {
        SoundArchive::FilePos pos;
        if (!SAP_LAYOUT(this)->mSoundArchive->detail_ReadFilePos(id, i, &pos)) {
            continue;
        }

        // GetGroupAddress, inlined
        const void* pGroup =
            SapLookupTableAddress(SAP_LAYOUT(this)->mGroupTable, pos.groupId);
        if (pGroup == NULL) {
            continue;
        }

        SoundArchive::GroupItemInfo item;
        if (SAP_LAYOUT(this)->mSoundArchive->detail_ReadGroupItemInfo(
                pos.groupId, pos.index, &item)) {
            return static_cast<const u8*>(pGroup) + item.offset;
        }
    }

    return NULL;
}

// Retail member offsets again diverge from the shared header: the manager
// sits at 0x1C and a second (file-level) wave-data table occupies 0x18.
const void* SoundArchivePlayer::detail_GetFileWaveDataAddress(u32 id) const {
    if (SAP_LAYOUT(this)->mFileManager != NULL) {
        const void* pAddr =
            SAP_LAYOUT(this)->mFileManager->GetFileWaveDataAddress(id);
        if (pAddr != NULL) {
            return pAddr;
        }
    }

    const void* pAddr =
        SAP_LAYOUT(this)->mSoundArchive->detail_GetWaveDataFileAddress(id);
    if (pAddr != NULL) {
        return pAddr;
    }

    // Direct file wave-data table lookup, keyed by fileId
    const void* pDirect = SapLookupTableWaveDataAddress(
        SAP_LAYOUT(this)->mFileTable, id);
    if (pDirect != NULL) {
        return pDirect;
    }

    SoundArchive::FileInfo file;
    if (!SAP_LAYOUT(this)->mSoundArchive->detail_ReadFileInfo(id, &file)) {
        return NULL;
    }

    for (unsigned int i = 0; i < file.filePosCount; i++) {
        SoundArchive::FilePos pos;
        if (!SAP_LAYOUT(this)->mSoundArchive->detail_ReadFilePos(id, i, &pos)) {
            continue;
        }

        // GetGroupWaveDataAddress, inlined
        const void* pGroup = SapLookupTableWaveDataAddress(
            SAP_LAYOUT(this)->mGroupTable, pos.groupId);

        if (pGroup == NULL) {
            continue;
        }

        SoundArchive::GroupItemInfo item;
        if (SAP_LAYOUT(this)->mSoundArchive->detail_ReadGroupItemInfo(
                pos.groupId, pos.index, &item)) {
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
    int playerPriority = sndInfo.playerPriority_; // padded retail layout

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

        SoundStartable::StartResult result =
            PrepareWaveSoundImpl__Q34nw4r3snd18SoundArchivePlayerFPQ44nw4r3snd6detail9WaveSoundPCQ44nw4r3snd12SoundArchive9SoundInfoPCQ44nw4r3snd12SoundArchive13WaveSoundInfoQ54nw4r3snd14SoundStartable9StartInfo15StartOffsetTypei(
                this, pWaveSound, &sndInfo, &waveInfo, startType, startOffset);

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

// Retail StrmSoundInfo carries two u16s consumed by StrmSound::Setup (the
// shared header declares the struct empty).
struct StrmSoundInfoLayout {
    char field_0x0[4];
    u16 field_0x4; // voices
    u16 field_0x6; // setup flag
};

// Priority-list bookkeeping of the retail StrmSound instance manager (based
// at this+0x6C): state word at 0x70, front node link at 0x74. The list node
// sits 0xEC bytes into its owning StrmSound.
struct StrmMgrState {
    char field_0x0[0x70];
    u32 field_0x70;   // 0 = empty list, 1 = allocation blocked
    char* field_0x74; // front node link
};

} // namespace

// Retail SeqSoundInfo layout (the shared header's field offsets differ).
struct SeqSoundInfoLayout {
    u32 dataOffset;            // at 0x0
    u32 field_0x04;
    u32 allocTrack;            // at 0x8
    int channelPriority;       // at 0xC
    u8 releasePriorityFixFlag; // at 0x10
};

// Retail SeqSound prefix touched by PrepareSeqImpl: player heap pointer at
// 0x4 and the sequence file-stream buffer at 0x2AC.
struct SeqSoundLayout {
    char field_0x00[0x04];
    /*0x04*/ detail::PlayerHeap* mPlayerHeap;
    char field_0x08[0x2A4];
    /*0x2AC*/ char mFileStreamBuffer[0x200];
};

// Imports resolved in other TUs (retail symbol names; the shared headers do
// not declare these shapes).
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

    // Layout accessors (macro forms keep these out of register coloring).
    SapFields& rFields = *reinterpret_cast<SapFields*>(thisPtr);
    const SoundInfoLayout& rInfo =
        *reinterpret_cast<const SoundInfoLayout*>(pSndInfo);
    const SeqSoundInfoLayout& rSeqInfo =
        *reinterpret_cast<const SeqSoundInfoLayout*>(pSeqInfo);
    SeqSoundLayout& rSeqSound = *reinterpret_cast<SeqSoundLayout*>(pSound);

    // Priority-list bookkeeping of the retail SeqSound instance manager
    // (based at this+0x44): state word at 0x48, front node link at 0x4C.
    // The list node sits 0xEC bytes into its owning BasicSound.
    u32& rMgrState = *reinterpret_cast<u32*>(reinterpret_cast<char*>(thisPtr) + 0x48);
    char*& rMgrFrontLink =
        *reinterpret_cast<char**>(reinterpret_cast<char*>(thisPtr) + 0x4C);

    const void* pFileAddress = NULL;
    ut::FileStream* pFileStream = NULL;
    u32 seqDataOffset = 0;
    u32 allocTrack = rSeqInfo.allocTrack;

    if (pSeqBin != NULL) {
        pFileAddress = pSeqBin;

        detail::SeqFileReader reader(pSeqBin);

        if (pLabelStr != NULL) {
            // Resolve the start position from a label inside the sequence
            // data.
            if (!ReadOffsetByLabel__Q44nw4r3snd6detail13SeqFileReaderCFPCcPUl(
                    &reader, pLabelStr, &seqDataOffset)) {
                return SoundStartable::
                    START_ERR_INVALID_SEQ_START_LOCATION_LABEL;
            }
        }

        // Derive the track allocation mask from the resolved position.
        seqDataOffset =
            ParseAllocTrack__Q44nw4r3snd6detail9MmlParserFPCvUlPUl(
                reader.GetBaseAddress(), seqDataOffset, &allocTrack);
    } else {
        pFileAddress = thisPtr->detail_GetFileAddress(rInfo.fileId);
        seqDataOffset = rSeqInfo.dataOffset;

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
        detail::PlayerHeap* pHeap = rSeqSound.mPlayerHeap;
        if (pHeap == NULL) {
            return SoundStartable::START_ERR_NOT_DATA_LOADED;
        }

        pFileStream = rFields.mSoundArchive->detail_OpenFileStream(
            rInfo.fileId, rSeqSound.mFileStreamBuffer,
            sizeof(rSeqSound.mFileStreamBuffer));

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

        if (rMgrState == 1) {
            if (pFileStream != NULL) {
                pFileStream->Close();
            }
            return SoundStartable::START_ERR_NOT_ENOUGH_INSTANCE;
        }

        detail::BasicSound* pDropSound;
        if (rMgrState == 0) {
            pDropSound = NULL;
        } else {
            // front node link sits 0xEC bytes into its owning BasicSound
            pDropSound = reinterpret_cast<detail::BasicSound*>(
                rMgrFrontLink - 0xEC);
        }

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
    pSound->SetChannelPriority(rSeqInfo.channelPriority);
    pSound->SetReleasePriorityFix(rSeqInfo.releasePriorityFixFlag);
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

    (void)voices;

    const StrmMgrState& rMgr = *reinterpret_cast<const StrmMgrState*>(this);

    // Setup may fail while every stream slot is busy; stop one sound and
    // retry until it succeeds or the situation proves unrecoverable.
    int setupResult = pSound->Setup(
        &mStrmBufferPool,
        reinterpret_cast<const StrmSoundInfoLayout*>(pStrmInfo)->field_0x4,
        reinterpret_cast<const StrmSoundInfoLayout*>(pStrmInfo)->field_0x6);

    while (setupResult != 0) {
        if (setupResult != 1) {
            return SoundStartable::START_ERR_UNKNOWN;
        }

        // All instances busy: consider stopping the lowest-priority sound
        // and retrying.
        u32 mgrState = rMgr.field_0x70;
        if (mgrState == 1) {
            return SoundStartable::START_ERR_NOT_ENOUGH_INSTANCE;
        }

        // front node link sits 0xEC bytes into its owning StrmSound
        detail::StrmSound* pLowest;
        if (mgrState == 0) {
            pLowest = NULL;
        } else {
            pLowest = reinterpret_cast<detail::StrmSound*>(
                rMgr.field_0x74 - 0xEC);
        }
        if (pLowest == pSound) {
            return SoundStartable::START_ERR_NOT_ENOUGH_INSTANCE;
        }

        pSound->Stop(0);

        setupResult = pSound->Setup(
            &mStrmBufferPool,
            reinterpret_cast<const StrmSoundInfoLayout*>(pStrmInfo)
                ->field_0x4,
            reinterpret_cast<const StrmSoundInfoLayout*>(pStrmInfo)
                ->field_0x6);
    }

    detail::StrmPlayer::StartOffsetType strmOffsetType;

    switch (startType) {
    case SoundStartable::StartInfo::START_OFFSET_TYPE_MILLISEC: {
        strmOffsetType = detail::StrmPlayer::START_OFFSET_TYPE_SAMPLE;
        break;
    }

    case SoundStartable::StartInfo::START_OFFSET_TYPE_TICK: {
        strmOffsetType = detail::StrmPlayer::START_OFFSET_TYPE_MILLISEC;
        startOffset = 0;
        break;
    }

    case SoundStartable::StartInfo::START_OFFSET_TYPE_SAMPLE: {
        strmOffsetType = detail::StrmPlayer::START_OFFSET_TYPE_MILLISEC;
        break;
    }

    default: {
        strmOffsetType = detail::StrmPlayer::START_OFFSET_TYPE_SAMPLE;
        startOffset = 0;
        break;
    }
    }

    ut::FileStream* pFileStream = mSoundArchive->detail_OpenFileStream(
        pSndInfo->fileId,
        reinterpret_cast<char*>(pSound) + 0xED0, // file stream buffer
        pSound->GetFileStreamBufferSize());

    if (pFileStream == NULL) {
        return SoundStartable::START_ERR_CANNOT_OPEN_FILE;
    }

    if (!pSound->Prepare(strmOffsetType, startOffset, pFileStream)) {
        return SoundStartable::START_ERR_UNKNOWN;
    }

    // int-to-float conversion via the 0x43300000/xoris trick; retail pools
    // the bias double and the 127.0f divisor in .sdata2
    pSound->SetInitialVolume(pSndInfo->volume / 127.0f);
    pSound->SetRemoteFilter(pSndInfo->remoteFilter);
    pSound->SetPanMode(pSndInfo->panMode);
    pSound->SetPanCurve(pSndInfo->panCurve);

    return SoundStartable::START_SUCCESS;
}

// Retail WaveSound::Prepare has NO 'voices' parameter (the sound's own
// GetVoiceOutCount() supplies the budget); the locked header declares a stale
// 7-parameter overload. The retail-named 6-parameter entry point is defined
// with C linkage in snd_WaveSound.cpp - declare it here so call sites emit the
// retail symbol.
extern "C" bool Prepare__Q44nw4r3snd6detail9WaveSoundFPCvlQ54nw4r3snd6detail9WsdPlayer15StartOffsetTypelPCQ54nw4r3snd6detail9WsdPlayer11WsdCallbackUl(
    detail::WaveSound* self, const void* pWsdData, s32 wsdOffset,
    detail::WsdPlayer::StartOffsetType startType, s32 startOffset,
    const detail::WsdPlayer::WsdCallback* pCallback, u32 callbackArg);

// Retail PrepareWaveSoundImpl has no 'voices' parameter either (locked header
// declares one), so like snd_WaveSound.cpp the retail-mangled entry point is
// defined here as a free function with C linkage.
extern "C" SoundStartable::StartResult
PrepareWaveSoundImpl__Q34nw4r3snd18SoundArchivePlayerFPQ44nw4r3snd6detail9WaveSoundPCQ44nw4r3snd12SoundArchive9SoundInfoPCQ44nw4r3snd12SoundArchive13WaveSoundInfoQ54nw4r3snd14SoundStartable9StartInfo15StartOffsetTypei(
    SoundArchivePlayer* self, detail::WaveSound* pSound,
    const SoundArchive::SoundInfo* pSndInfo,
    const SoundArchive::WaveSoundInfo* pWsdInfo,
    SoundStartable::StartInfo::StartOffsetType startType, int startOffset) {

    const void* pWsdBin = self->detail_GetFileAddress(pSndInfo->fileId);
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

    if (!Prepare__Q44nw4r3snd6detail9WaveSoundFPCvlQ54nw4r3snd6detail9WsdPlayer15StartOffsetTypelPCQ54nw4r3snd6detail9WsdPlayer11WsdCallbackUl(
            pSound, pWsdBin, pWsdInfo->subNo, wsdOffsetType, startOffset,
            &self->mWsdCallback, pSndInfo->fileId)) {
        return SoundStartable::START_ERR_UNKNOWN;
    }

    // Setter order matters for codegen parity with retail.
    pSound->SetInitialVolume(pSndInfo->volume / 127.0f);
    pSound->SetRemoteFilter(pSndInfo->remoteFilter);
    pSound->SetPanMode(pSndInfo->panMode);
    pSound->SetPanCurve(pSndInfo->panCurve);
    pSound->SetChannelPriority(pWsdInfo->channelPriority);
    pSound->SetReleasePriorityFix(pWsdInfo->releasePriorityFixFlag);

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

