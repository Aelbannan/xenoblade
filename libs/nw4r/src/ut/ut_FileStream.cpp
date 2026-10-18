// Retail .sbss 0x80665540-0x8066554C (0xC): lbl_eu_80665540 is IOStream's
// typeInfo, an 8-byte object (RuntimeTypeInfo parent link at +0x0 plus 4
// inert pad bytes; only offset 0 is ever written). It is zero-initialized
// so it emits no __sinit_ entry. lbl_eu_80665548 is FileStream's own 4-byte
// typeInfo (parent = &lbl_eu_80665540). The TU __sinit_ is the single
// parent-link store (0xC: li/stw/blr) -- same wrapper recipe as the accepted
// ut_DvdFileStream lbl_eu_80665550.
// The header declares lbl_eu_80665540 as RuntimeTypeInfo; rename that decl
// for the duration of the include and define the retail-sized storage here.
#define lbl_eu_80665540 lbl_eu_80665540_size_hidden
#include <nw4r/ut.h>
#undef lbl_eu_80665540

namespace {
// Plain storage mirroring RuntimeTypeInfo's layout (parent link at +0x0)
// plus the 4 inert pad bytes. Constant-initialized to zero so it lands in
// .sbss with no __sinit_ entry of its own.
struct IOStreamTypeInfo {
    const nw4r::ut::detail::RuntimeTypeInfo* parentLink; // +0x0
    u32 pad;                                             // +0x4
};
} // namespace

extern "C" IOStreamTypeInfo lbl_eu_80665540 = {NULL, 0};
extern "C" nw4r::ut::detail::RuntimeTypeInfo lbl_eu_80665548(
    reinterpret_cast<const nw4r::ut::detail::RuntimeTypeInfo*>(
        &lbl_eu_80665540));

namespace nw4r {
namespace ut {

void FileStream::Cancel() {}

bool FileStream::CancelAsync(StreamCallback pCallback, void* pCallbackArg) {
#pragma unused(pCallback)
#pragma unused(pCallbackArg)
    return true;
}

} // namespace ut
} // namespace nw4r
