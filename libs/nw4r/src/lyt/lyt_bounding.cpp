#include <nw4r/lyt.h>

namespace nw4r {
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
} // namespace nw4r

// LLM-HARNESS-BEGIN: us-804077d8
extern "C" bool GetRuntimeTypeInfo__Q34nw4r3lyt8BoundingCFv() { return false; }
// LLM-HARNESS-END: us-804077d8
