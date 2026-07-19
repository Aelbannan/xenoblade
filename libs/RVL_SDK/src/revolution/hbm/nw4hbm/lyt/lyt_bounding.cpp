#include <nw4hbm/lyt.h>

namespace nw4hbm {
namespace lyt {

NW4R_UT_RTTI_DEF_DERIVED(Bounding, Pane);

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
} // namespace nw4hbm

// LLM-HARNESS-BEGIN: us-8032ee80
extern "C" void GetRuntimeTypeInfo__Q36nw4hbm3lyt8BoundingCFv() {}
// LLM-HARNESS-END: us-8032ee80
