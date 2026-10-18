#include <nw4r/ut.h>

// Retail owns the IOStream RuntimeTypeInfo in ut_FileStream's .sbss slice
// (lbl_eu_80665540); declared extern so this split emits no data of its own.
extern "C" nw4r::ut::detail::RuntimeTypeInfo lbl_eu_80665540;

// The retail split keeps the TU static-init for the FileStream-owned
// typeinfo object (0xC parent-link NULL store); the guard's inline ctor
// folds into the auto-__sinit_ with no emitted storage of its own.
namespace {
struct IOStreamTypeInfoInit { IOStreamTypeInfoInit() {
    lbl_eu_80665540.mParentTypeInfo = NULL; } };
static IOStreamTypeInfoInit s_ioStreamTypeInfoInit;
} // namespace

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
