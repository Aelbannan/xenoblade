#ifndef NW4R_SND_PLAYER_HEAP_H
#define NW4R_SND_PLAYER_HEAP_H
#include <nw4r/types_nw4r.h>

#include <nw4r/ut.h>

namespace nw4r {
namespace snd {

// Forward declarations
class SoundPlayer;

namespace detail {
class BasicSound;
} // namespace detail

namespace detail {

// US Xenoblade PlayerHeap is a compact heap (not stock SoundHeap subclass).
// Layout: vt@0, mSound@4, mPlayer@8, buffer ptrs@0xC/0x10/0x14, node@0x18.
class PlayerHeap {
public:
    PlayerHeap();
    virtual ~PlayerHeap();

    bool Create(void* pBase, u32 size);
    void* Alloc(u32 size);
    void Clear();
    u32 GetFreeSize() const;

    void AttachSound(BasicSound* pSound);
    void DetachSound(BasicSound* pSound);

    void SetSoundPlayer(SoundPlayer* pPlayer) {
        mPlayer = pPlayer;
    }

private:
    BasicSound* mSound;   // at 0x4
    SoundPlayer* mPlayer; // at 0x8
    void* mStart;         // at 0xC
    void* mUnk0x10;       // at 0x10
    void* mEnd;           // at 0x14

public:
    NW4R_UT_LINKLIST_NODE_DECL(); // at 0x18
};

NW4R_UT_LINKLIST_TYPEDEF_DECL(PlayerHeap);

} // namespace detail
} // namespace snd
} // namespace nw4r

#endif
