#include <nw4r/ut.h>

// Retail owns the IOStream RuntimeTypeInfo in nw4r_data.s (.sbss); declared
// extern so this .text-only split emits no data. ut_FileStream.h consumes it.
extern "C" nw4r::ut::detail::RuntimeTypeInfo lbl_eu_80665540;

// The retail split keeps the auto-static-init for the (retail-owned) typeinfo
// object as a standalone 0xC function in this TU; reproduce it without
// defining the object (the blob's copy is already initialized). The symbol is
// renamed to the retail backslash form (__sinit_\ut_IOStream_cpp) via the
// postprocess exact_renames rule for this object.
extern "C" void __sinit_ut_IOStream_cpp() {
    *(const nw4r::ut::detail::RuntimeTypeInfo**)&lbl_eu_80665540 = NULL;
}

namespace nw4r {
namespace ut {

bool IOStream::ReadAsync(void* pDst, u32 size, StreamCallback pCallback,
                         void* pCallbackArg) {
#pragma unused(pDst)
#pragma unused(size)
#pragma unused(pCallback)
#pragma unused(pCallbackArg)
    return false;
}

bool IOStream::Write(const void* pSrc, u32 size) {
#pragma unused(pSrc)
#pragma unused(size)
    return false;
}

bool IOStream::WriteAsync(const void* pSrc, u32 size, StreamCallback pCallback,
                          void* pCallbackArg) {
#pragma unused(pSrc)
#pragma unused(size)
#pragma unused(pCallback)
#pragma unused(pCallbackArg)
    return false;
}

bool IOStream::IsBusy() const {
    return false;
}

} // namespace ut
} // namespace nw4r
