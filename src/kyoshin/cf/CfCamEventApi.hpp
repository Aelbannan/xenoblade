#pragma once

#include <types.h>

class CfCamEventManager;
struct CfCamEventCopySrc;

// Canonical C++ declarations for the helpers owned by CfCamEvent_1.cpp.
void CamEvtReleaseMappedSlot(CfCamEventManager* self, u32 idx);
void* CamEvtFetchSlotPtr(void* self, unsigned long idx);
int CamEvtBusyChk();
int CamEvtIsIntro();
void CamEvtCopyBlk(CfCamEventManager* dst, CfCamEventCopySrc* src);
void* CamEvtReleaseThenFire(CfCamEventManager* self, u32 idx, u32 param);
u32 CamEvtResetManager(CfCamEventManager* self, u32 first, u32 second,
                       u32 third, u32 fourth, u32 fifth);
int CamEvtForwardShakeFlag(int arg0, int arg1);
int CamEvtLookupStateVal(int val);
int CamEvtSearchStateVal(int val);
bool CamEvtGetFl46();
