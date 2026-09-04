// ICamControlGc leaf overrides (cf::ICamControlGc). Each calls the pad-status
// provider at vtable +0x8 (this->func_80274B28(), shared with the Remote
// table) and extracts one held-flag bit from the returned CPad.

#include "kyoshin/harness_catalog.hpp"
#include "kyoshin/cf/ICamControl.hpp"
#include "monolib/core/CPadManager.hpp"

namespace cf {

u32 ICamControlGc::func_80274CD4(int arg) {
    CPad* st = this->func_80274B28();
    return (st->mHeldButtonFlags >> 19) & 1;
}

u32 ICamControlGc::func_80274D08(int arg) {
    CPad* st = this->func_80274B28();
    return (st->mHeldButtonFlags >> 20) & 1;
}

u32 ICamControlGc::func_80274D3C(int arg) {
    CPad* st = this->func_80274B28();
    return (st->mHeldButtonFlags >> 17) & 1;
}

u32 ICamControlGc::func_80274D70(int arg) {
    CPad* st = this->func_80274B28();
    return (st->mHeldButtonFlags >> 18) & 1;
}

} // namespace cf
