#include <nw4r/lyt.h>
#include <nw4r/ut.h>

#include <cstring>

// Retail .data lbl_eu_80569C68 (0x10) DrawInfo table (0,0,dtor,0)
// Defined manually to match retail split (compiler would emit __vt__ under
// a different name; we emit the retail label directly).
extern "C" void __dt__Q34nw4r3lyt8DrawInfoFv();
extern "C" {
__declspec(section ".data") __attribute__((aligned(8))) const void* lbl_eu_80569C68[4] = {
    nullptr, nullptr, (const void*)&__dt__Q34nw4r3lyt8DrawInfoFv, nullptr
};
}

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
