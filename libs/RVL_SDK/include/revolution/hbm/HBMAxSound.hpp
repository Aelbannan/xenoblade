#pragma once

#include <types.h>

struct HBMSEQSEQUENCE;

// C-linkage imports (retail symbol names - keep linkage/signatures verbatim)

// MSL libm export referenced by retail (not declared in stl/math.h).
extern "C" double log10(double);

// seq.c exports (unmangled in retail).
extern "C" void HBMSEQInit(void);
extern "C" void HBMSEQAddSequence(HBMSEQSEQUENCE* seq, const u8* data,
                                  void* synth, void* p1, u32 p2);
extern "C" void HBMSEQRemoveSequence(HBMSEQSEQUENCE* seq);
extern "C" void HBMSEQSetState(HBMSEQSEQUENCE* seq, u32 state);
extern "C" u32 HBMSEQGetState(HBMSEQSEQUENCE* seq);
extern "C" void HBMSEQRunAudioFrame(void);
extern "C" void HBMSEQQuit(void);
extern "C" void HBMSYNInit(void);
extern "C" void HBMSYNRunAudioFrame(void);
extern "C" void HBMSYNQuit(void);
extern "C" void HBMMIXInit(void);
extern "C" void HBMMIXUpdateSettings(void);
extern "C" void HBMMIXQuit(void);
extern "C" void HBMMIXSetSoundMode(u32 mode);
// Retail declares (void*, int) here but the retail body ignores the volume.
extern "C" void HBMSEQSetVolume(HBMSEQSEQUENCE* seq, int volume);
