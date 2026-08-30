#include <nw4r/ut.h>

// Retail .sbss 0x80665540-0x8066554C (0xC): lbl_eu_80665540 (0x4) + gap (0x4) + lbl_eu_80665548 (0x4)
// lbl_eu_80665540 is IOStream's typeinfo (parent of FileStream). Gap pads to retail 0xC.
extern "C" {
    nw4r::ut::detail::RuntimeTypeInfo lbl_eu_80665540(NULL);
}
unsigned char gap_80665544[4] __attribute__((aligned(4)));
extern "C" nw4r::ut::detail::RuntimeTypeInfo lbl_eu_80665548(&lbl_eu_80665540);

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
