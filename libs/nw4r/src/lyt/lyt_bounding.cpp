#include <nw4r/lyt.h>

namespace nw4r {
namespace lyt {

// Local RTTI definition: required to generate the retail __sinit_ stub
// (the C-linkage ctor definition emits the 0xC dead-init placeholder; the
// object itself lives in the shared pool in the retail split).
extern "C" nw4r::ut::detail::RuntimeTypeInfo lbl_eu_80665498(&lbl_eu_80665470);

Bounding::Bounding(const res::Bounding* pRes, const ResBlockSet& rBlockSet)
    : Pane(pRes) {

    // Retail's vptr store is the last statement of the ctor body; the
    // __declspec(novtable) class defers the store to this explicit write.
    *(void**)this = (void*)lbl_eu_80569B80;

#pragma unused(rBlockSet)
}

Bounding::~Bounding() {}

void Bounding::DrawSelf(const DrawInfo& rInfo) {
#pragma unused(rInfo)
    // Debug draw stripped out
}

} // namespace lyt
} // namespace nw4r

namespace nw4r {
namespace lyt {

const nw4r::ut::detail::RuntimeTypeInfo* Bounding::GetRuntimeTypeInfo() const {
    return &lbl_eu_80665498;
}

} // namespace lyt
} // namespace nw4r
