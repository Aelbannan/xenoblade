#include <nw4r/ut.h>

// Retail DvdFileStream::typeInfo ships from the monolibdata1 data unit
// (lbl_eu_80665550, .sbss 8 bytes). The recovered symbol is 8 bytes: the
// RuntimeTypeInfo parent pointer at +0x0 (FileStream's typeInfo,
// lbl_eu_80665548) plus 4 inert pad bytes (only offset 0 is ever written;
// consumers only take the address). The wrapper keeps the real type at +0
// and carries the retail size; the auto-__sinit__ emits the retail 0xC body
//   li r0, lbl_eu_80665548@sda21
//   stw r0, lbl_eu_80665550@sda21(r13)
//   blr
// with the retail reloc names directly (no postprocess retarget needed).
namespace {

struct DvdFileStreamTypeInfo {
    nw4r::ut::detail::RuntimeTypeInfo typeInfo; // parent typeInfo ptr at 0x0
    u32 pad;                                    // 0x4 (retail symbol size 8)

    explicit DvdFileStreamTypeInfo(
        const nw4r::ut::detail::RuntimeTypeInfo* pParent)
        : typeInfo(pParent) {}
};

} // namespace

extern "C" DvdFileStreamTypeInfo lbl_eu_80665550(&lbl_eu_80665548);
