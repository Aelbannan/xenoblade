#include <nw4r/g3d.h>
#include <nw4r/math.h>

// Retail .sdata2 zero constant ships from the nw4r data unit (nw4r_data.s);
// referenced by name so no local pool is emitted.
extern "C" {
    extern const f32 lbl_eu_80669BC0; // 0.0f
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

    if (frame >= lbl_eu_80669BC0) {
        return math::FMod(frame, length);
    }

    f32 offset = math::FMod(frame + length, length);
    return offset + math::FSelect(offset, lbl_eu_80669BC0, length);
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

// Absorbed retail sdata (play-policy dispatch + floats)
extern "C" {
__declspec(section ".sdata") __attribute__((aligned(8))) const void* lbl_eu_80663458[2] = {
    (const void*)nw4r::g3d::PlayPolicy_Onetime,
    (const void*)nw4r::g3d::PlayPolicy_Loop,
};
__declspec(section ".sdata") __attribute__((aligned(8))) const float lbl_eu_80663460[2] = {1.0f, 0.0f};
}
// Keep sdata live (referenced via FrameCtrl policy table)
namespace {
struct KeepAnmObjSdata { KeepAnmObjSdata(){ (void)lbl_eu_80663458; (void)lbl_eu_80663460; } } keepAnmObjSdata;
}
