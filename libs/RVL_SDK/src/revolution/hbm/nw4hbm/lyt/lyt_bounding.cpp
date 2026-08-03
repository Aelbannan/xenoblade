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
    if (rInfo.IsDebugDrawMode()) {
        LoadMtx(rInfo);

        detail::DrawLine(GetVtxPos(), mSize, ut::Color::GREEN);
    }
}

} // namespace lyt
} // namespace nw4hbm

namespace nw4hbm {
namespace ut {

/* Emitted here to match the retail binary: only lyt_bounding.o defines the
 * ut::Color destructor (other lyt TUs reference it via __destroy_arr). */
Color::~Color() {}

} // namespace ut
} // namespace nw4hbm
