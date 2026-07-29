#include <nw4hbm/lyt.h>
#include <nw4hbm/ut.h>

namespace nw4hbm {
namespace lyt {

/******************************************************************************
 *
 * ResourceAccessor
 *
 ******************************************************************************/
ResourceAccessor::ResourceAccessor() {}

ResourceAccessor::~ResourceAccessor() {}

ut::Font* ResourceAccessor::GetFont(const char* /*pName*/) {
    return NULL;
}

} // namespace lyt
} // namespace nw4hbm
