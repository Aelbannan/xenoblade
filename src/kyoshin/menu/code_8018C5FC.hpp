#pragma once

// Owner header for kyoshin/menu/code_8018C5FC.cpp.  The full party-gauge
// controller layout is private to the defining TU; callers only hold pointers.
namespace cf {
class UnkClass_8018C5FC;

s32 PartyGaugeAddClamped(UnkClass_8018C5FC* self, s32 delta);
s32 PartyGaugeSetClamped(UnkClass_8018C5FC* self, s32 value);
}
