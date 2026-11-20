#pragma once

// Canonical C++ declarations for the free helpers defined in CtrlNpc.cpp.
// The concrete CtrlNpc types stay in CtrlNpc.hpp; this lean header lets
// callers emit the real mangled relocs without pulling in that heavy header.

#include <types.h>

namespace cf {
class CtrlNpc;
class CCtrlNpcChar;
}

namespace ml {
struct CVec3;
}

void CtrlNpcCharInitState(cf::CCtrlNpcChar* self);
void CtrlNpcInitMoveWindow(cf::CtrlNpc* self, u32 a, u32 b, int count,
                           const ml::CVec3* src, f32 f);
void func_80094E44(cf::CtrlNpc* self, u32 val);
void CtrlNpcArmMoveTarget5(cf::CtrlNpc* self, int animMain, int animAlt1,
                           int animAlt2);
