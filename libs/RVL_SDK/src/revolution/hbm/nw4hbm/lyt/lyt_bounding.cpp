#include <nw4hbm/lyt.h>

namespace nw4hbm {
namespace ut {

/* Emitted here to match the retail binary: only lyt_bounding.o defines the
 * ut::Color destructor (other lyt TUs reference it via __destroy_arr). */
Color::~Color() {}

} // namespace ut
} // namespace nw4hbm

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

extern "C" void* GetRuntimeTypeInfo__Q36nw4hbm3lyt8BoundingCFv(void) { return 0; }
