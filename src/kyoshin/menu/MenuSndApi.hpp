#pragma once

#include <types.h>

// Defined in kyoshin/menu/code_80187F14.cpp. C++ linkage: these signatures
// produce the retail mangled symbol names.
struct SoundSlot;

void MenuSnd_StopAllSlots_8774(SoundSlot* slot);
void MenuSnd_SetSlotsPause_8890(s32 active);
s32 MenuSnd_IsSlotLive_892C(s32 index);
void MenuSnd_PushSlotVolume_896C(s32 index, u32 type, float f1, float f2);
void MenuSnd_StopSlot0Clear_9318(s32 clearName, float vol);
void MenuSnd_StopSlot0Store_9390(const char* name);
void MenuSnd_StopSlot1_9424(float vol);
float MenuSnd_GetMasterVol_95EC();
float MenuSnd_GetSeVol_96A0();
void MenuSnd_SetBalancePush_96A8(s32 index, float f1, float f2);
s32 MenuSnd_SetSlotPan_9C40(s32 index, f32* pos, s32 val, float f1, float f2, float f3);
void MenuSnd_ResetFadeTargetA_9C70();
void MenuSnd_ResetFadeTargetB_9C7C();
