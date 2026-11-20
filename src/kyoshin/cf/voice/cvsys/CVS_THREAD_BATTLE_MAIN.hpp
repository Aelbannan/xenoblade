#pragma once

struct CVoiceHandle;

// Canonical C++ declarations (definitions live in CVS_THREAD_BATTLE_MAIN.cpp).
int BmVoice_PlayA0(CVoiceHandle* self);
int BmVoice_Play12C(CVoiceHandle* self);
int BmVoice_Play0A(CVoiceHandle* self);
int BmVoice_PlayReqB4(CVoiceHandle* self, int param);
int BmVoice_Play32(CVoiceHandle* self);
int BmVoice_PlayGated(CVoiceHandle* self, int param, int size);
