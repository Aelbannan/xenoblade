// RVL_SDK/src/revolution/hbm/HBMAxSound

#include <harness_catalog.h>

#include <math.h>

#include <revolution/AI.h>
#include <revolution/AX.h>
#include <revolution/OS.h>
#include <revolution/arc/arc.h>
#include <revolution/hbm/HBMAxSound.hpp>

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

// Player block layout (shared with the C-linkage HBMSEQ* imports declared in
// revolution/hbm/HBMAxSound.hpp; global scope so the type matches the
// header's forward declaration).
struct HBMSEQSEQUENCE {
    u8 pad0[0x418];          // 0x00000
    u32 finished;            // 0x418 (synth track-end flag)
    u8 pad1[0x2E1C - 0x41C]; // 0x41C
    u8 inUse;                // 0x2E1C
    u8 pad2[3];              // 0x2E1D
    HBMSEQSEQUENCE* next;    // 0x2E20 (newer player)
    HBMSEQSEQUENCE* prev;    // 0x2E24 (older player)
    s32 seqId;               // 0x2E28
};                           // stride 0x2E2C

namespace {

/******************************************************************************
 *
 * sWork (retail .bss 0x805DA058: pointer to the work area)
 *
 * Players: 7 HBMSEQSEQUENCE blocks of 0x2E2C bytes each.
 * The first 4 form the "normal" pool (count 4), the last 3 the "special"
 * pool (count 3, selected for seq ids 4/0x17/0x19).
 ******************************************************************************/
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
    OSMessage msgBuffer[4];        // 0x146A0 (0x10 bytes)
    u8* workEnd;                 // 0x146B0
    void* seqWork1;              // 0x146B4
    void* seqWork2;              // 0x146B8
};

HBMWork* sWork;

// Retail rodata: 28 filenames indexed by seq id (SOUND_FILENAME).
extern "C" const char* SOUND_FILENAME[] = {
    "midi\\00_home_button.mid",
    "midi\\01_return_app.mid",
    "midi\\02_goto_menu.mid",
    "midi\\03_reset_app.mid",
    "midi\\04_focus.mid",
    "midi\\05_select.mid",
    "midi\\06_cancel.mid",
    "midi\\07_open_controller.mid",
    "midi\\08_close_controller.mid",
    "midi\\09_volume_plus.mid",
    "midi\\10_volume_minus.mid",
    "midi\\11_volume_plus_limit.mid",
    "midi\\12_volume_minus_limit.mid",
    "midi\\13_nothing_done.mid",
    "midi\\14_vibe_on.mid",
    "midi\\15_vibe_off.mid",
    "midi\\16_start_connect_window.mid",
    "midi\\17_connected1.mid",
    "midi\\18_connected2.mid",
    "midi\\19_connected3.mid",
    "midi\\20_connected4.mid",
    "midi\\21_end_connect_window.mid",
    "midi\\22_manual_open.mid",
    "midi\\23_manual_focus.mid",
    "midi\\24_manual_select.mid",
    "midi\\25_manual_scroll.mid",
    "midi\\26_manual_cancel.mid",
    "midi\\27_manual_return_app.mid",
};

// Sound data filenames inside the sound archive.

// seq.c exports are declared in revolution/hbm/HBMAxSound.hpp (C-linkage imports).

HBMSEQSEQUENCE* GetFreePlayer(int soundId) {
    HBMSEQSEQUENCE* players;
    HBMSEQSEQUENCE* p;
    SeqPool* pool;
    int count;
    int i;

    if (soundId == 4 || soundId == 0x17 || soundId == 0x19) {
        players = &sWork->players[4];
        count = 3;
    } else {
        players = &sWork->players[0];
        count = 4;
    }

    p = NULL;
    for (i = 0; i < count; i++) {
        if (players[i].inUse == 0) {
            p = &players[i];
            p->inUse = 1;
            break;
        }
    }

    if (p == NULL) {
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

void* AudioSoundThreadProc(void* /* arg */) {
    OSMessage msg = 0;
    HBMSEQSEQUENCE* p;
    HBMSEQSEQUENCE* p_next;
    SeqPool* pool;

    while (true) {
        OSReceiveMessage(&sWork->msgQueue, &msg, OS_MSG_BLOCKING);

        if (msg == reinterpret_cast<OSMessage>(1)) {
            HBMSEQRunAudioFrame();
            HBMSYNRunAudioFrame();
            HBMMIXUpdateSettings();

            if (sWork != NULL) {
                p = sWork->pool[0].first;
                while (p != NULL) {
                    p_next = p->next;
                    if (p->inUse != 0 && HBMSEQGetState(p) == 0 && p->finished == 0) {
                        if (p->seqId == 4 || p->seqId == 0x17 || p->seqId == 0x19) {
                            pool = &sWork->pool[1];
                        } else {
                            pool = &sWork->pool[0];
                        }

                        HBMSEQSetState(p, 0);
                        HBMSEQRemoveSequence(p);
                        p->inUse = 0;

                        if (p->prev == NULL) {
                            pool->first = p->next;
                        } else {
                            p->prev->next = p->next;
                        }

                        if (p->next == NULL) {
                            pool->last = p->prev;
                        } else {
                            p->next->prev = p->prev;
                        }

                        p->next = NULL;
                        p->prev = NULL;
                    }
                    p = p_next;
                }

                p = sWork->pool[1].first;
                while (p != NULL) {
                    p_next = p->next;
                    if (p->inUse != 0 && HBMSEQGetState(p) == 0 && p->finished == 0) {
                        if (p->seqId == 4 || p->seqId == 0x17 || p->seqId == 0x19) {
                            pool = &sWork->pool[1];
                        } else {
                            pool = &sWork->pool[0];
                        }

                        HBMSEQSetState(p, 0);
                        HBMSEQRemoveSequence(p);
                        p->inUse = 0;

                        if (p->prev == NULL) {
                            pool->first = p->next;
                        } else {
                            p->prev->next = p->next;
                        }

                        if (p->next == NULL) {
                            pool->last = p->prev;
                        } else {
                            p->next->prev = p->prev;
                        }

                        p->next = NULL;
                        p->prev = NULL;
                    }
                    p = p_next;
                }
            }
        } else if (msg == reinterpret_cast<OSMessage>(8)) {
            return NULL;
        }
    }
}

// Retail anonymous-namespace rodata anchors (mangled @unnamed@ form).
static const char WT_FILENAME[0x14] = "wt\\HomeButtonSe.wt";
static const char PCM_FILENAME[0x14] = "wt\\HomeButtonSe.pcm";

} // namespace

// Retail .rodata has a 4th float (0.0f) after the generated pool.
extern const float s_volumeZeroPad = 0.0f;
u8 hbmAxSound_bss_pad[4]; /* retail .bss 0x4 -> 0x8 (align tail) */

namespace homebutton {

void PlaySeq(int seqId) {
    ARCFileInfo fileInfo;
    const u8* pData;

    if (ARCOpen(&sWork->archive, SOUND_FILENAME[seqId], &fileInfo) != 0) {
        pData = static_cast<const u8*>(ARCGetStartAddrInMem(&fileInfo));
    } else {
        pData = NULL;
    }

    if (pData != NULL) {
        HBMSEQSEQUENCE* player = GetFreePlayer(seqId);
        SeqPool* pool;

        HBMSEQAddSequence(player, pData, sWork->seqWork1, sWork->seqWork2, 0);
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

void InitAxSound(const void* pWork, void* pWorkEnd, u32 workSize) {
    HBMWork* work;
    ARCFileInfo fileInfo;
    ARCFileInfo fileInfo2;
    u32 intr;
    int i;

    if (workSize < 0x18700) {
        return;
    }

    if (AICheckInit() == 0) {
        AIInit(NULL);
        AXInit();
    }

    HBMMIXInit();
    HBMSYNInit();
    HBMSEQInit();

    work = static_cast<HBMWork*>(pWorkEnd);

    for (i = 0; i < 7; i++) {
        work->players[i].inUse = 0;
        work->players[i].next = NULL;
        work->players[i].prev = NULL;
    }

    work->seqWork1 = NULL;
    work->seqWork2 = NULL;
    work->workEnd = reinterpret_cast<u8*>(pWorkEnd) + 0x14700;
    work->pool[0].first = NULL;
    work->pool[0].last = NULL;
    work->pool[1].first = NULL;
    work->pool[1].last = NULL;
    work->prevFrameCb = NULL;

    if (ARCInitHandle(const_cast<void*>(pWork), &work->archive) != 0) {
        if (ARCOpen(&work->archive, WT_FILENAME, &fileInfo) != 0) {
            work->seqWork1 = ARCGetStartAddrInMem(&fileInfo);
            if (ARCOpen(&work->archive, PCM_FILENAME, &fileInfo2) != 0) {
                work->seqWork2 = ARCGetStartAddrInMem(&fileInfo2);
                OSInitMessageQueue(&work->msgQueue, work->msgBuffer, 4);
                if (OSCreateThread(
                        &work->thread, AudioSoundThreadProc, NULL,
                        work->workEnd + workSize - 0x14700,
                        workSize - 0x14700, 4, 0) != 0) {
                    sWork = work;
                    OSResumeThread(&work->thread);
                    intr = OSDisableInterrupts();
                    work->prevFrameCb =
                        AXRegisterCallback(AudioFrameCallback);
                    OSRestoreInterrupts(intr);
                }
            }
        }
    }
}

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
void StopAllSeq() {
    HBMSEQSEQUENCE* p;
    int i;
    SeqPool* pool;

    if (sWork == NULL) {
        return;
    }

    for (i = 0; i < 4; i++) {
        p = &sWork->players[i];
        if (p->inUse != 0) {
            if (p->seqId == 4 || p->seqId == 0x17 || p->seqId == 0x19) {
                pool = &sWork->pool[1];
            } else {
                pool = &sWork->pool[0];
            }

            HBMSEQSetState(p, 0);
            HBMSEQRemoveSequence(p);
            p->inUse = 0;

            if (p->prev == NULL) {
                pool->first = p->next;
            } else {
                p->prev->next = p->next;
            }

            if (p->next == NULL) {
                pool->last = p->prev;
            } else {
                p->next->prev = p->prev;
            }

            p->next = NULL;
            p->prev = NULL;
        }
    }

    for (i = 0; i < 3; i++) {
        p = &sWork->players[4] + i;
        if (p->inUse != 0) {
            if (p->seqId == 4 || p->seqId == 0x17 || p->seqId == 0x19) {
                pool = &sWork->pool[1];
            } else {
                pool = &sWork->pool[0];
            }

            HBMSEQSetState(p, 0);
            HBMSEQRemoveSequence(p);
            p->inUse = 0;

            if (p->prev == NULL) {
                pool->first = p->next;
            } else {
                p->prev->next = p->next;
            }

            if (p->next == NULL) {
                pool->last = p->prev;
            } else {
                p->next->prev = p->prev;
            }

            p->next = NULL;
            p->prev = NULL;
        }
    }
}
#pragma dont_inline reset

void SetVolumeAllSeq(float volume)
{
    int vol;

    if (sWork == NULL) {
        return;
    }

    if (volume <= 0.0f) {
        vol = -0x388;
    } else {
        vol = (int)(10.0f * (20.0f * (float)log10((double)volume)));
    }

    if (vol > 0x3C) {
        vol = 0x3C;
    }
    if (vol < -0x388) {
        vol = -0x388;
    }

    {
        int i;
        for (i = 0; i < 4; i++) {
            HBMSEQSEQUENCE* players = &sWork->players[i];
            if (players->inUse != 0) {
                HBMSEQSetVolume(players, vol);
            }
        }
    }
    {
        int i;
        for (i = 0; i < 3; i++) {
            HBMSEQSEQUENCE* players = &sWork->players[4] + i;
            if (players->inUse != 0) {
                HBMSEQSetVolume(players, vol);
            }
        }
    }
}

void SetSoundMode(u32 mode) { HBMMIXSetSoundMode(mode); }

} // namespace homebutton
