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


/* Retail .bss slice is 8 bytes: typeInfo then 4 zero pad bytes (aligning the
   next unit's .bss); sdata_threshold 0 keeps this 4-byte global in .bss.
   Non-static so MWCC emits it. Declared last so the pragma does not affect
   the string-literal placement of the class code above. */
#pragma push
#pragma sdata_threshold 0
u32 __LytBoundingBssPad;
#pragma pop
