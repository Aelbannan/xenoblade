// Classic-controller camera control overrides (cf::ICamControlClassic).
// Each override calls the pad-status provider at vtable +0x8
// (this->fetchPad(), retail camClassicGetMainPad body) and extracts flag bits
// from the returned CPad's button-flag words.

#include "kyoshin/cf/ICamControlClassic.hpp"
#include "kyoshin/cf/CfGameManager.hpp"
#include "monolib/core/CPadManager.hpp"

// vtable +0x08 override: the main controller pad (index 0).
CPad* cf::ICamControlClassic::fetchPad() {
    return cf::CfGameManager::getPad(0);
}

// vtable +0x0C override (retail EB0 body): bit 4 of the pressed flags.
u32 cf::ICamControlClassic::isShortPress11() {
    return (this->fetchPad()->mPressedButtonFlags >> 27) & 1;
}

// vtable +0x10 override (retail EE4 body): nonzero when both held bit 4 and
// pressed bit 7 are set.
u32 cf::ICamControlClassic::isPressed12(int controllerId) {
    CPad* st = this->fetchPad();
    return (st->mHeldButtonFlags & 0x08000000) && (st->mPressedButtonFlags & 0x01000000);
}

// vtable +0x14 override (retail F34 body): tri-state on held bits.
u32 cf::ICamControlClassic::isPadBit4(int controllerId) {
    u32 v = this->fetchPad()->mHeldButtonFlags;
    u32 b10 = (v >> 21) & 1;
    if (b10 && (v & 0x00400000)) return 0;   // bit 9
    return (v & 0x08000000) && b10;          // bit 4
}

// vtable +0x18 override (retail F94 body): tri-state on held bits.
u32 cf::ICamControlClassic::isPadBit5(int controllerId) {
    CPad* st = this->fetchPad();
    u32 v = st->mHeldButtonFlags;
    if ((v & 0x00200000) && (v & 0x00400000)) return 0;   // bits 10, 9
    return (v & 0x08000000) && (v & 0x00400000);          // bits 4, 9
}

// vtable +0x1C override (retail FF4 body): bit 12 of the held flags.
u32 cf::ICamControlClassic::isHeld2(int arg) {
    return (this->fetchPad()->mHeldButtonFlags >> 19) & 1;
}

// vtable +0x20 override (retail 5028 body): bit 11 of the held flags.
u32 cf::ICamControlClassic::isHeld3(int arg) {
    return (this->fetchPad()->mHeldButtonFlags >> 20) & 1;
}

// vtable +0x24 override (retail 505C body): bit 14 of the held flags.
u32 cf::ICamControlClassic::isHeld0(int arg) {
    return (this->fetchPad()->mHeldButtonFlags >> 17) & 1;
}

// vtable +0x28 override (retail 5090 body): bit 13 of the held flags.
u32 cf::ICamControlClassic::isHeld1(int arg) {
    return (this->fetchPad()->mHeldButtonFlags >> 18) & 1;
}

// vtable +0x2C override (retail 50C4 body): chained OR of bit conditions.
u32 cf::ICamControlClassic::isLongHold11(int arg) {
    CPad* st = this->fetchPad();
    int a = 1;
    int b = 1;
    int c = 1;
    u32 v = st->mHeldButtonFlags;
    if ((v & 0x00080000) == 0 && (v & 0x00100000) == 0) c = 0;  // bits 12, 11
    if (c == 0 && (v & 0x00020000) == 0) b = 0;                 // bit 14
    if (b == 0 && (v & 0x00040000) == 0) a = 0;                 // bit 13
    return a;
}
