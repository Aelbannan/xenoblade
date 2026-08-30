#include <nw4r/lyt.h>

#include <revolution/OS.h>

extern "C" {
__declspec(section ".data") __attribute__((aligned(8), used)) const char lbl_eu_805698A8[0x48] = "<< NW4R    - LYT \tfinal   build: Aug 21 2008 05:21:58 (0x4199_60831) >>";
__declspec(section ".sdata") __attribute__((aligned(8), used)) const void* lbl_eu_806634B8[2] = { lbl_eu_805698A8, nullptr };
}

namespace nw4r {
namespace lyt {

void LytInit() {
    extern const void* lbl_eu_806634B8[2];
    OSRegisterVersion((const char*)lbl_eu_806634B8[0]);
    OSInitFastCast();
}

} // namespace lyt
} // namespace nw4r
