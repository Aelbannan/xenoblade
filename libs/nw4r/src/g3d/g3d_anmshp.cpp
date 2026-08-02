#include <nw4r/g3d.h>

namespace nw4r {
namespace g3d {

bool AnmObjShp::TestExistence(u32 idx) const {
    return (mpBinding[idx] & (BINDING_UNDEFINED | BINDING_INVALID)) == 0;
}

} // namespace g3d
} // namespace nw4r
