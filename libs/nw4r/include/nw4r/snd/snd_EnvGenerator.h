#ifndef NW4R_SND_ENV_GENERATOR_H
#define NW4R_SND_ENV_GENERATOR_H
#include <nw4r/types_nw4r.h>

namespace nw4r {
namespace snd {
namespace detail {

class EnvGenerator {
public:
    enum Status { STATUS_ATTACK, STATUS_HOLD, STATUS_DECAY, STATUS_SUSTAIN, STATUS_RELEASE };

public:
    EnvGenerator();

    void Init(f32 db = VOLUME_INIT);
    void Reset(f32 db = VOLUME_INIT);
    f32 GetValue() const;
    void Update(int msec);

    Status GetStatus() const {
        return mStatus;
    }
    void SetStatus(Status status) {
        mStatus = status;
    }

    void SetAttack(int attack);
    void SetDecay(int decay);
    void SetSustain(int sustain);
    void SetRelease(int release);
    void SetHold(int hold);

private:
    static const int DECIBEL_SQUARE_TABLE_SIZE = 128;

    static const volatile f32 VOLUME_INIT;
    static const int ATTACK_INIT = 127;
    static const int DECAY_INIT = 127;
    static const int SUSTAIN_INIT = 127;
    static const int RELEASE_INIT = 127;
    static const s16 DecibelSquareTable[128];

private:
    f32 CalcRelease(int release) {
        if (release == 127) {
            return 65535.0f;
        }

        if (release == 127 - 1) {
            return 24.0f;
        }

        if (release < 50) {
            return (release * 2 + 1) / 128.0f / 5.0f;
        }

        return 60.0f / (127 - 1 - release) / 5.0f;
    }

    // NOTE: the float literals above are kept pooled (byte-identical retail
    // codegen incl. the fmuls operand order); the TU-local pool entries are
    // renamed to the retail sdata2 labels by postprocess pool_patterns on
    // snd_EnvGenerator.o (65535.0f/50/54/58/5C) and the .sdata2 is
    // trimmed to the retail-empty split.

    int CalcDecibelSquare(int scale) {
        return DecibelSquareTable[scale];
    }

private:
    Status mStatus; // at 0x0
    f32 mValue;     // at 0x4
    f32 mDecay;     // at 0x8
    f32 mRelease;   // at 0xC
    f32 mAttack;    // at 0x10
    u8 mSustain;       // at 0x14
    s16 mHold;         // at 0x16
    u16 mHoldCounter;  // at 0x18
};

} // namespace detail
} // namespace snd
} // namespace nw4r

#endif
