#include <nw4r/g3d.h>
#include <nw4r/math.h>

// Retail .sdata2 zero constant ships from the nw4r data unit (nw4r_data.s);
// referenced by name so no local pool is emitted.
extern "C" {
}

namespace nw4r {
namespace g3d {

f32 PlayPolicy_Onetime(f32 start, f32 end, f32 frame) {
#pragma unused(start)
#pragma unused(end)

    return frame;
}

f32 PlayPolicy_Loop(f32 start, f32 end, f32 frame) {
    f32 length = end - start;

    if (frame >= 0.0f) {
        return math::FMod(frame, length);
    }

    f32 offset = math::FMod(frame + length, length);
    return offset + math::FSelect(offset, 0.0f, length);
}

void AnmObj::SetAnmFlag(AnmFlag flag, bool value) {
    if (value) {
        mFlags |= flag;
    } else {
        mFlags &= ~flag;
    }
}

bool AnmObj::TestAnmFlag(AnmFlag flag) const {
    return mFlags & flag;
}

} // namespace g3d
} // namespace nw4r

// NOTE: intentionally non-const. A const float pair unreferenced in-TU is
// dead-stripped by MWCC; the old static-keeper struct kept it alive but
// emitted __sinit_ into .text (split budget). Mutable storage is always
// emitted, with identical bytes, and no code.
extern "C" {
__declspec(section ".sdata") __attribute__((aligned(8))) const void* lbl_eu_80663458[2] = {
    (const void*)nw4r::g3d::PlayPolicy_Onetime,
    (const void*)nw4r::g3d::PlayPolicy_Loop,
};
__declspec(section ".sdata") __attribute__((aligned(8))) float lbl_eu_80663460[2] = {1.0f, 0.0f};
}
