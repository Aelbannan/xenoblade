#pragma once

#include <types.h>

namespace nw4r {
namespace math {
struct VEC3;
}
namespace lyt {
class DrawInfo;
}
}

class CPresentWin;

// Canonical declarations for the free helpers defined in CPresentWin.cpp.
void drawPresentWin(CPresentWin* self, nw4r::lyt::DrawInfo* drawInfo);
u32 isPresentActive(CPresentWin* self);
u32 isPresentSettled(CPresentWin* self);
void advancePresentState(CPresentWin* self);
void stepPresentCounter(CPresentWin* self);
void togglePresentPath(CPresentWin* self);
u32 getPresentPath(CPresentWin* self);
u32 isPresentBusy(CPresentWin* self);
void getPresentPaneWorldPos(nw4r::math::VEC3* out, CPresentWin* src);
// Retail plain-C thunks defined in CPresentWin.cpp.
extern "C" void dispatchCounterStepBack(CPresentWin* self);
extern "C" void updatePresentWin(CPresentWin* self);
