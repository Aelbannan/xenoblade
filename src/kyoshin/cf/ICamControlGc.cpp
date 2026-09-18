// ICamControlGc leaf overrides (cf::ICamControlGc). Each calls the pad-status
// provider at vtable +0x8 (this->fetchPad(), shared with the Remote
// table) and extracts one held-flag bit from the returned CPad.

#include "kyoshin/harness_catalog.hpp"
#include "kyoshin/cf/ICamControl.hpp"
#include "monolib/core/CPadManager.hpp"

namespace cf {

u32 ICamControlGc::isHeld2(int arg) {
    CPad* st = this->fetchPad();
    return (st->mHeldButtonFlags >> 19) & 1;
}

u32 ICamControlGc::isHeld3(int arg) {
    CPad* st = this->fetchPad();
    return (st->mHeldButtonFlags >> 20) & 1;
}

u32 ICamControlGc::isHeld0(int arg) {
    CPad* st = this->fetchPad();
    return (st->mHeldButtonFlags >> 17) & 1;
}

u32 ICamControlGc::isHeld1(int arg) {
    CPad* st = this->fetchPad();
    return (st->mHeldButtonFlags >> 18) & 1;
}

} // namespace cf
