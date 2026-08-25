#include <nw4r/lyt.h>

// Retail vtable label (data object in nw4r_data.s). The class is novtable, so
// the ctor stores this label explicitly (keeps the ctor reloc byte-identical
// to retail while emitting no local .data vtable copy).
extern "C" unsigned char lbl_eu_80569CA0[];

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

ResourceAccessor::ResourceAccessor() {
    *(void**)this = (void*)lbl_eu_80569CA0;
}

ut::Font* ResourceAccessor::GetFont(const char* /*pName*/) {
    // Default implementation: no named font available.
    return NULL;
}

} // namespace lyt
} // namespace nw4r
