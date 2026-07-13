#include <nw4r/ut.h>

extern "C" nw4r::ut::detail::RuntimeTypeInfo lbl_eu_80665540;
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
