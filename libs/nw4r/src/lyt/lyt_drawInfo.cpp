#include <nw4r/lyt.h>
#include <nw4r/ut.h>

#include <cstring>

namespace nw4r {
namespace lyt {

DrawInfo::DrawInfo() : mViewRect(lbl_eu_80669DD0, lbl_eu_80669DD0, lbl_eu_80669DD0, lbl_eu_80669DD0),
                       mLocationAdjustScale(lbl_eu_80669DD4, lbl_eu_80669DD4),
                       mGlobalAlpha(lbl_eu_80669DD4) {
    std::memset(&mFlag, 0, sizeof(mFlag));
    math::MTX34Identity(&mViewMtx);
}

DrawInfo::~DrawInfo() {}

} // namespace lyt
} // namespace nw4r
