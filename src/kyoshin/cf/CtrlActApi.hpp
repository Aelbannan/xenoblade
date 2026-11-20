#pragma once

// C++ declarations for the free helpers installed in cf::CtrlAct's vtable.
// The defining TU is CtrlAct.cpp; callers include this header instead of
// keeping TU-local copies of the old C-linkage spellings.

namespace cf {
class CtrlAct;
}

struct CtrlActAtkArg;

int ctrlActDispatchEntry(cf::CtrlAct* self, CtrlActAtkArg* arg);
int ctrlActAimAtTarget(void* self);
int ctrlActComputeAimVec(void* self);
int ctrlActFaceTargetVec(void* self);
void ctrlActClearState30(void* self);
unsigned long ctrlActIsFlagBit15Clear(void* self);
void ctrlActUpdateData70(void* self);
int ctrlActCheckBlockedProbe(void* self);
int ctrlActProbeTargetReset(void* self);
