// RVL_SDK/src/revolution/hbm/HBMAxSound

#include <harness_catalog.h>

#include <revolution/AX.h>
#include <revolution/OS.h>
#include <revolution/arc/arc.h>

namespace homebutton {

void PlaySeq(int seqId);
void InitAxSound(const void* pWork, void* pWorkEnd, u32 workSize);
void ShutdownAxSound();
void AxSoundMain();
void StopAllSeq();
void SetVolumeAllSeq(float volume);
void SetSoundMode(u32 mode);

} // namespace homebutton

// --- hard-symbol stubs (scaffold_hard_symbols) ---
namespace {

/******************************************************************************
 *
 * sWork (retail .bss 0x805DA058: pointer to the work area)
 *
 * Players: 7 HBMSEQSEQUENCE blocks of 0x2E2C bytes each.
 * The first 4 form the "normal" pool (count 4), the last 3 the "special"
 * pool (count 3, selected for seq ids 4/0x17/0x19).
 ******************************************************************************/
struct HBMSEQSEQUENCE {
    u8 pad0[0x2E1C];         // 0x00000
    u8 inUse;                // 0x2E1C
    u8 pad1[3];              // 0x2E1D
    HBMSEQSEQUENCE* next;    // 0x2E20 (newer player)
    HBMSEQSEQUENCE* prev;    // 0x2E24 (older player)
    s32 seqId;               // 0x2E28
};                           // stride 0x2E2C

struct SeqPool {
    HBMSEQSEQUENCE* first;   // 0x00
    HBMSEQSEQUENCE* last;    // 0x04
};

struct HBMWork {
    HBMSEQSEQUENCE players[7];     // 0x00000 - 0x14334
    SeqPool pool[2];               // 0x14334, 0x1433C
    u8 pad_14344[4];               // 0x14344
    AXOutCallback prevFrameCb;     // 0x14348
    ARCHandle archive;             // 0x1434C (0x1C bytes)
    OSThread thread;               // 0x14368 (0x318 bytes)
    OSMessageQueue msgQueue;       // 0x14680 (0x20 bytes)
    u8 pad_146A0[0x14];            // .. 0x146B4
    void* seqWork1;                // 0x146B4
    void* seqWork2;                // 0x146B8
};

HBMWork* sWork;

// Retail rodata: 28 filenames indexed by seq id (SOUND_FILENAME).
extern "C" const char* SOUND_FILENAME[];

// seq.c exports (unmangled in retail).
extern "C" void HBMSEQAddSequence(HBMSEQSEQUENCE* seq, const u8* data,
                                  void* synth, void* p1, u32 p2);
extern "C" void HBMSEQRemoveSequence(HBMSEQSEQUENCE* seq);
extern "C" void HBMSEQSetState(HBMSEQSEQUENCE* seq, u32 state);
extern "C" void HBMSEQQuit(void);
extern "C" void HBMSYNQuit(void);
extern "C" void HBMMIXQuit(void);

HBMSEQSEQUENCE* GetFreePlayer(int soundId) {
    HBMSEQSEQUENCE* players;
    HBMSEQSEQUENCE* p = NULL;
    int count;
    int i;

    if (soundId == 4 || soundId == 0x17 || soundId == 0x19) {
        players = &sWork->players[4];
        count = 3;
    } else {
        players = &sWork->players[0];
        count = 4;
    }

    for (i = 0; i < count; i++) {
        if (players[i].inUse == 0) {
            p = &players[i];
            p->inUse = 1;
            break;
        }
    }

    if (p == NULL) {
        SeqPool* pool;

        if (soundId == 4 || soundId == 0x17 || soundId == 0x19) {
            pool = &sWork->pool[1];
        } else {
            pool = &sWork->pool[0];
        }
        p = pool->first;

        if (p->seqId == 4 || p->seqId == 0x17 || p->seqId == 0x19) {
            pool = &sWork->pool[1];
        } else {
            pool = &sWork->pool[0];
        }

        HBMSEQSetState(p, 0);
        HBMSEQRemoveSequence(p);
        p->inUse = 0;

        if (p->prev == NULL)
            pool->first = p->next;
        else
            p->prev->next = p->next;

        if (p->next == NULL)
            pool->last = p->prev;
        else
            p->next->prev = p->prev;

        p->next = NULL;
        p->prev = NULL;
        p->inUse = 1;
    }

    return p;
}

void AudioFrameCallback() {
    if (sWork != NULL) {
        OSSendMessage(&sWork->msgQueue, reinterpret_cast<OSMessage>(1), 0);

        if (sWork->prevFrameCb != NULL) {
            sWork->prevFrameCb();
        }
    }
}

void AudioSoundThreadProc(void* /* arg */) {}

} // namespace

namespace homebutton {

void PlaySeq(int seqId) {
    ARCFileInfo fileInfo;
    void* pData;

    if (ARCOpen(&sWork->archive, SOUND_FILENAME[seqId], &fileInfo) != 0) {
        pData = ARCGetStartAddrInMem(&fileInfo);
    } else {
        pData = NULL;
    }

    if (pData != NULL) {
        HBMSEQSEQUENCE* player = GetFreePlayer(seqId);
        SeqPool* pool;

        HBMSEQAddSequence(player, reinterpret_cast<const u8*>(pData),
                          sWork->seqWork1, sWork->seqWork2, 0);
        HBMSEQSetState(player, 1);
        player->seqId = seqId;

        if (seqId == 4 || seqId == 0x17 || seqId == 0x19) {
            pool = &sWork->pool[1];
        } else {
            pool = &sWork->pool[0];
        }

        if (pool->last == NULL) {
            pool->first = player;
            pool->last = player;
            player->next = NULL;
            player->prev = NULL;
        } else {
            pool->last->next = player;
            player->prev = pool->last;
            player->next = NULL;
            pool->last = player;
        }
    }
}

void InitAxSound(const void* /* pWork */, void* /* pWorkEnd */,
                 u32 /* workSize */) {}

void ShutdownAxSound() {
    if (sWork != NULL) {
        StopAllSeq();
        AXRegisterCallback(sWork->prevFrameCb);
        OSJamMessage(&sWork->msgQueue, reinterpret_cast<OSMessage>(8),
                     OS_MSG_BLOCKING);
        OSJoinThread(&sWork->thread, NULL);
        HBMSEQQuit();
        HBMSYNQuit();
        HBMMIXQuit();
        sWork = NULL;
    }
}

void AxSoundMain() {}

#pragma dont_inline on
void StopAllSeq() {}
#pragma dont_inline reset

void SetVolumeAllSeq(float /* volume */) {}

void SetSoundMode(u32 /* mode */) {}

} // namespace homebutton

// Force-emit the anonymous-namespace helpers (retail references them from
// InitAxSound via AXRegisterCallback / OSCreateThread / PlaySeq).
DECOMP_FORCEACTIVE(HBMAxSound_cpp, GetFreePlayer, AudioFrameCallback,
                   AudioSoundThreadProc);
