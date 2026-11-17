#ifndef NW4R_LYT_UTIL_H
#define NW4R_LYT_UTIL_H
#include <nw4r/types_nw4r.h>

namespace nw4r {
namespace lyt {

// Forward declarations
class AnimTransform;
class Group;

void BindAnimation(Group* pGroup, AnimTransform* pAnimTrans, bool recursive,
                   bool disable);

} // namespace lyt
} // namespace nw4r

#endif
