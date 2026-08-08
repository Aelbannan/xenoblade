#include <nw4r/lyt.h>

namespace nw4r {
namespace lyt {

extern "C" nw4r::ut::detail::RuntimeTypeInfo lbl_eu_80665498(&lbl_eu_80665470);

Bounding::Bounding(const res::Bounding* pRes, const ResBlockSet& rBlockSet)
    : Pane(pRes) {

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
