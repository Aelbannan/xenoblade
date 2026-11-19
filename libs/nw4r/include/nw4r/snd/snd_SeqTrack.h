#ifndef NW4R_SND_SEQ_TRACK_H
#define NW4R_SND_SEQ_TRACK_H
#include <nw4r/types_nw4r.h>

#include <nw4r/snd/snd_Channel.h>
#include <nw4r/snd/snd_Lfo.h>
#include <nw4r/snd/snd_MoveValue.h>

#include <revolution/WPAD.h>

namespace nw4r {
namespace snd {

// Forward declarations
class SeqPlayer;

enum SeqMute { MUTE_OFF, MUTE_NO_STOP, MUTE_RELEASE, MUTE_STOP };

enum ParseResult { PARSE_RESULT_CONTINUE, PARSE_RESULT_FINISH };

namespace detail {

class SeqTrack {
public:
    static const int VARIABLE_NUM = 16;
    static const int PRGNO_MAX = 0xFFFF;

    static const int CALL_STACK_MAX_DEPTH = 3;

    // Saved MML call state. The loop count / return address pair is what
    // MML_CALL, MML_LOOP_START and MML_RET save and restore.
    struct CallStack {
        u8 loopFlag;           // at 0x0
        u8 loopCount;          // at 0x1
        const u8* currentAddr; // at 0x4
    };

    // MML interpreter state. It is reached through the track's parser
    // parameters because MmlParser drives those fields directly.
    struct MmlParserParam {
        // cmpFlag is a raw byte: the userproc callback rewrites it in place
        // through SeqUserprocCallbackParam (retail stores it unnormalized).
        u8 cmpFlag;                           // at 0x0
        bool noteWaitFlag;                    // at 0x1
        bool tieFlag;                         // at 0x2
        // Set by MML_SET_MONOPHONIC: NoteOn then reuses the newest channel
        // instead of allocating a new one (retail gates the block on it).
        bool monophonicFlag;                  // at 0x3
        CallStack callStack[CALL_STACK_MAX_DEPTH]; // at 0x4
        u8 callStackDepth;                    // at 0x1C
    };

    struct ParserTrackParam {
        const u8* baseAddr;    // at 0x0
        const u8* currentAddr; // at 0x4

        MmlParserParam mmlParserParam; // at 0x8

        s32 wait; // at 0x28

        bool muteFlag;       // at 0x2C
        bool silenceFlag;    // at 0x2D
        bool noteFinishWait; // at 0x2E
        bool portaFlag;      // at 0x2F
        bool damperFlag;     // at 0x30

        int bankNo; // at 0x34
        int prgNo;  // at 0x38

        LfoParam lfoParam; // at 0x3C

        u8 lfoTarget;   // at 0x4C
        f32 sweepPitch; // at 0x50

        MoveValue<u8, s16> volume;      // at 0x54
        MoveValue<s8, s16> pan;         // at 0x5A
        MoveValue<s8, s16> surroundPan; // at 0x60

        u8 volume2;       // at 0x66
        u8 velocityScale; // at 0x67

        s8 pitchBend; // at 0x68
        u8 bendRange; // at 0x69

        s8 initPan;   // at 0x6A
        s8 transpose; // at 0x6B

        u8 priority;  // at 0x6C
        u8 portaKey;  // at 0x6D
        u8 portaTime; // at 0x6E

        u8 attack;  // at 0x6F
        u8 decay;   // at 0x70
        u8 sustain; // at 0x71
        u8 release; // at 0x72

        s16 hold; // at 0x74

        u8 mainSend;            // at 0x76
        u8 fxSend[AUX_BUS_NUM]; // at 0x77

        u8 biquadType; // at 0x7A

        f32 lpfFreq; // at 0x7C

        // MML_SET_BIQUAD_FREQ writes this through the parser-param reference,
        // so it lives inside ParserTrackParam (SeqTrack+0x9C).
        f32 biquadFreq; // at 0x80
    };

public:
    SeqTrack();
    virtual ~SeqTrack(); // at 0x8

    virtual ParseResult Parse(bool doNoteOn) = 0; // at 0xC

    bool IsOpened() const {
        return mOpenFlag;
    }

    void SetPlayerTrackNo(int no);
    u8 GetPlayerTrackNo() const {
        return mPlayerTrackNo;
    }

    void InitParam();
    void SetSeqData(const void* pBase, s32 offset);

    void Open();
    void Close();

    void UpdateChannelLength();
    void UpdateChannelRelease(Channel* pChannel);

    int ParseNextTick(bool doNoteOn);

    void StopAllChannel();
    void ReleaseAllChannel(int release);
    void PauseAllChannel(bool flag);
    void AddChannel(Channel* pChannel);
    void UpdateChannelParam();
    void FreeAllChannel();

    void SetMute(SeqMute mute);
    void SetVolume(f32 volume);
    void SetPitch(f32 pitch);

    ParserTrackParam& GetParserTrackParam() {
        return mParserTrackParam;
    }

    volatile s16* GetVariablePtr(int idx);

    SeqPlayer* GetSeqPlayer() {
        return mPlayer;
    }
    void SetSeqPlayer(SeqPlayer* pPlayer) {
        mPlayer = pPlayer;
    }

    Channel* GetLastChannel() const {
        return mChannelList;
    }

    Channel* NoteOn(int key, int velocity, s32 length, bool tie);

private:
    static const int DEFAULT_PRIORITY = 64;
    static const int DEFAULT_BENDRANGE = 2;
    static const int DEFAULT_PORTA_KEY = 60;
    static const int DEFAULT_VARIABLE_VALUE = -1;

    static const int DEFAULT_VOLUME = 127;
    static const int DEFAULT_MAINSEND = 127;
    static const int DEFAULT_ENV = 0xFF;

private:
    static void ChannelCallbackFunc(Channel* pDropChannel,
                                    Channel::ChannelCallbackStatus status,
                                    u32 callbackArg);

private:
    u8 mPlayerTrackNo; // at 0x4
    bool mOpenFlag;    // at 0x5

    f32 mExtVolume;      // at 0x8
    f32 mExtPitch;       // at 0xC
    f32 mExtPan;         // at 0x10
    f32 mExtSurroundPan; // at 0x14
    f32 mPanRange;       // at 0x18

    ParserTrackParam mParserTrackParam;        // at 0x1C
    volatile s16 mTrackVariable[VARIABLE_NUM]; // at 0xA0
    SeqPlayer* mPlayer;                        // at 0xC0
    Channel* mChannelList;                     // at 0xC4
};

} // namespace detail
} // namespace snd
} // namespace nw4r

#endif
