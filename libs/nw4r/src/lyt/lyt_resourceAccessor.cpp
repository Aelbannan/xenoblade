#include <nw4r/lyt.h>

namespace nw4r {
namespace lyt {

/******************************************************************************
 *
 * ResourceAccessor
 *
 * Abstract base for resource containers (archive / arc). This TU holds the
 * virtual-destructor key function (which places the vtable in .data) and the
 * default font lookup; the raw resource access is pure virtual.
 *
 ******************************************************************************/

ResourceAccessor::~ResourceAccessor() {}

ResourceAccessor::ResourceAccessor() {}

ut::Font* ResourceAccessor::GetFont(const char* /*pName*/) {
    // Default implementation: no named font available.
    return NULL;
}

} // namespace lyt
} // namespace nw4r
