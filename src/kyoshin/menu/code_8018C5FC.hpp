#pragma once

#include <types.h>

// Owner header for kyoshin/menu/code_8018C5FC.cpp.
namespace cf {

// Party-gauge controller (cf::CBattleManager +0x194): holds the gauge counter
// (clamped to [0, 300]) plus a frame-time accumulator used to tick it down
// during play.
class UnkClass_8018C5FC {
public:
    UnkClass_8018C5FC();
    s32 gaugeValue;   // +0x00: party gauge counter, clamped to [0, 0x12c] // name-guess: gaugeValue
    f32 tickAccum;    // +0x04: accumulated scaled frame time until next tick // name-guess: tickAccum
};

s32 PartyGaugeAddClamped(UnkClass_8018C5FC* self, s32 delta);
s32 PartyGaugeSetClamped(UnkClass_8018C5FC* self, s32 value);
}
