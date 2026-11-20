#pragma once

#include <types.h>

class CPcKizunagram;
namespace nw4r { namespace lyt { class DrawInfo; } }

// Real C++ linkage helpers defined in CPcKizunagram.cpp. Retail symbols carry
// these manglings; callers include this owning header instead of declaring
// per-TU extern "C" copies.
void KizunagramDraw(CPcKizunagram* self, nw4r::lyt::DrawInfo* drawInfo);
void KizunagramOpen(CPcKizunagram* self);
void KizunagramClose(CPcKizunagram* self);
void KizunagramCursorUp(CPcKizunagram* self);
void KizunagramCursorDown(CPcKizunagram* self);
void KizunagramCursorPageUp(CPcKizunagram* self);
void KizunagramCursorPageDown(CPcKizunagram* self);
extern "C" int KizunagramIsOpen(CPcKizunagram* self);
