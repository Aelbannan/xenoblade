#include <nw4r/lyt.h>

// Retail vtable lbl_eu_80569CA0 (0x18) now owned by this TU (absorbed from
// nw4r_data.s). The class is novtable, so the ctor stores this label
// explicitly; we emit the retail vtable here with relocs.
extern "C" void __dt__Q34nw4r3lyt16ResourceAccessorFv();
extern "C" void* GetFont__Q34nw4r3lyt16ResourceAccessorFPCc();
extern "C" {
__declspec(section ".data") __attribute__((aligned(8))) const void* lbl_eu_80569CA0[6] = {
    nullptr, nullptr, (const void*)&__dt__Q34nw4r3lyt16ResourceAccessorFv, nullptr,
    (const void*)&GetFont__Q34nw4r3lyt16ResourceAccessorFPCc, nullptr
};
}

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
