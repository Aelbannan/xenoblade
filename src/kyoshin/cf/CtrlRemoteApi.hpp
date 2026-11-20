#pragma once

// Canonical C++ declarations for the helpers defined in CtrlRemote.cpp.
// Kept lean: callers that only need these functions do not have to include
// the full CtrlRemote view header.

#include <types.h>

namespace cf {
class CtrlRemote;
}

struct CtrlRemoteBuf;

u32 CtrlRemote_TestFlag800(cf::CtrlRemote* self);
u32 CtrlRemote_Check800Cleared1000(cf::CtrlRemote* self);
u32 CtrlRemote_DispatchBufOp(cf::CtrlRemote* self, CtrlRemoteBuf* src, s32 type, u32 arg);
u32* CtrlRemote_GetSharedBufPtr();
int CtrlRemote_GetFixedSize1214(void* self);
u32* CtrlRemote_FetchSharedBufPtr();
void CtrlRemote_ResetSlotArrayObj(void* obj);
void CtrlRemote_ResetSlotArrayByIndex(u32 index);
