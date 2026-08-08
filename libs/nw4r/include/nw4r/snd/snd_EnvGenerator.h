#ifndef NW4R_SND_ENV_GENERATOR_H
#define NW4R_SND_ENV_GENERATOR_H
#include <nw4r/types_nw4r.h>

// ---------------------------------------------------------------------------
// C-linkage imports (retail symbol names - keep linkage/signatures verbatim)
// ---------------------------------------------------------------------------

// Retail .sdata2/.rodata pool constants and tables referenced by name so the
// SDA21/data relocations match the stripped retail object (PLAN.md §17.6
// approved extern "C" lbl_eu_* pattern, MWCC_REFERENCE §1b float pools).
extern "C" const f32 lbl_eu_80669F30; // -90.4f (volume min dB)
extern "C" const f32 lbl_eu_80669F34; // 65535.0f
extern "C" const f32 lbl_eu_80669F38; // 10.0f
extern "C" const f32 lbl_eu_80669F3C; // 0.0f
extern "C" const f32 lbl_eu_80669F40; // -1/32
extern "C" const f32 lbl_eu_80669F48; // 176.0f

// Attack table (128 f32 entries).  Retail: lbl_eu_8051FD40.
extern "C" const f32 lbl_eu_8051FD40[128];

namespace nw4r {
namespace snd {
namespace detail {

// Retail .rodata table referenced by the inline CalcDecibelSquare below so
// the data relocation matches the stripped retail object.
extern "C" const s16 lbl_eu_8051FC40[128];

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

    int CalcDecibelSquare(int scale) {
        return lbl_eu_8051FC40[scale];
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
