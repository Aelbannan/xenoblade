#include <nw4r/ut.h>

namespace nw4r {
namespace ut {

u32 IOStream::GetOffsetAlign() const {
    return 1;
}

u32 IOStream::GetSizeAlign() const {
    return 1;
}

u32 IOStream::GetBufferAlign() const {
    return 1;
}

} // namespace ut
} // namespace nw4r
