#ifndef NW4R_SND_STRM_SOUND_H
#define NW4R_SND_STRM_SOUND_H
#include <nw4r/types_nw4r.h>

#include <nw4r/snd/snd_BasicSound.h>
#include <nw4r/snd/snd_StrmPlayer.h>

#include <nw4r/ut.h>

namespace nw4r {
namespace snd {

// Forward declarations
class StrmSoundHandle;

namespace detail {
template <typename T> class SoundInstanceManager;
class StrmBufferPool;
} // namespace detail

namespace detail {

class StrmSound : public BasicSound {
    friend class nw4r::snd::StrmSoundHandle;

public:
    NW4R_UT_RTTI_DECL(StrmSound);

public:
    explicit StrmSound(SoundInstanceManager<StrmSound>* pManager);
    virtual ~StrmSound();

    virtual void Shutdown(); // at 0x28
    virtual bool IsPrepared() const {
        return mStrmPlayer.IsPrepared();
    } // at 0x2C

    virtual void SetPlayerPriority(int priority); // at 0x4C
    virtual bool IsAttachedTempSpecialHandle();   // at 0x5C
    virtual void DetachTempSpecialHandle();       // at 0x60

    virtual BasicPlayer& GetBasicPlayer() {
        return mStrmPlayer;
    } // at 0x68
    virtual const BasicPlayer& GetBasicPlayer() const {
        return mStrmPlayer;
    } // at 0x6C

    bool Setup(StrmBufferPool* pPool, int voices, u16 unk);
    bool Prepare(StrmPlayer::StartOffsetType offsetType, s32 offset,
                 ut::FileStream* pStream);
    void InitParam();
    void UpdateMoveValue();

    void* GetFileStreamBuffer() {
        return mFileStreamBuffer;
    }
    s32 GetFileStreamBufferSize() {
        return sizeof(mFileStreamBuffer);
    }

private:
    static const int FILE_STREAM_BUFFER_SIZE = 512;

private:
    StrmPlayer mStrmPlayer;                    // at 0xD8
    u8 _padStrmPlayer[0xD6C - sizeof(StrmPlayer) - 0x34];
    StrmSoundHandle* mTempSpecialHandle;       // at 0xE44
    SoundInstanceManager<StrmSound>* mManager; // at 0xE48
    char UNK_0xE4C[0xE60 - 0xE4C];
    char mFileStreamBuffer[FILE_STREAM_BUFFER_SIZE]; // at 0xE60
};

} // namespace detail
} // namespace snd
} // namespace nw4r

#endif
