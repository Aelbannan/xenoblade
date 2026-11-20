#pragma once

#include <types.h>

struct CVoiceHandle;
struct CVoiceHandleState;

// Canonical C++ declarations for the helpers owned by CVS_THREAD_HP.cpp.
int HpVoice_InitPlayerVoice(CVoiceHandle* self);
int HpVoice_InitVoiceId(CVoiceHandle* self);
int HpVoice_MapSubState(CVoiceHandle* handle);
int HpVoice_CollectIdle(CVoiceHandle** out, int capacity, CVoiceHandle* exclude);
int HpVoice_CountActiveVoices(CVoiceHandle* exclude);
int HpVoice_CheckPair26(CVoiceHandleState* a, CVoiceHandleState* b);
int HpVoice_CheckPair13(CVoiceHandleState* a, CVoiceHandleState* b);
int HpVoice_CheckHpBase(CVoiceHandle* handle);
int HpVoice_CheckPairGate(CVoiceHandle* a, CVoiceHandle* b);
int HpVoice_CheckIdleGate(CVoiceHandle* handle);
int HpVoice_SeqBelow94();
