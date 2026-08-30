#include <nw4r/ut.h>

namespace nw4r {
namespace ut {
namespace {

inline bool IsSJISLeadByte(u8 ch) {
    return (ch >= 0x81 && ch < 0xA0) || ch >= 0xE0;
}

} // namespace

u16 CharStrmReader::ReadNextCharUTF8() {
    u16 ch;

    if (!(GetChar<u8>(0) & 0x80)) {
        ch = GetChar<u8>(0);
        StepStrm<u8>(1);
    } else if ((GetChar<u8>(0) & 0xE0) == 0xC0) {
        ch = ((GetChar<u8>(0) & 0x1F) << 6) | (GetChar<u8>(1) & 0x3F);
        StepStrm<u8>(2);
    } else {
        ch = ((GetChar<u8>(0) & 0x1F) << 12) | ((GetChar<u8>(1) & 0x3F) << 6) |
             (GetChar<u8>(2) & 0x3F);
        StepStrm<u8>(3);
    }

    return ch;
}

u16 CharStrmReader::ReadNextCharUTF16() {
    u16 ch = GetChar<u16>(0);
    StepStrm<u16>(1);
    return ch;
}

u16 CharStrmReader::ReadNextCharCP1252() {
    u16 ch = GetChar<u8>(0);
    StepStrm<u8>(1);
    return ch;
}

u16 CharStrmReader::ReadNextCharSJIS() {
    u16 ch;

    if (IsSJISLeadByte(GetChar<u8>(0))) {
        ch = (GetChar<u8>(0) << 8) | GetChar<u8>(1);
        StepStrm<u8>(2);
    } else {
        ch = GetChar<u8>(0);
        StepStrm<u8>(1);
    }

    return ch;
}

} // namespace ut
} // namespace nw4r

extern "C" {
void ReadNextCharUTF8__Q34nw4r2ut14CharStrmReaderFv();
void ReadNextCharUTF16__Q34nw4r2ut14CharStrmReaderFv();
void ReadNextCharSJIS__Q34nw4r2ut14CharStrmReaderFv();
void ReadNextCharCP1252__Q34nw4r2ut14CharStrmReaderFv();
__declspec(section ".data") __attribute__((aligned(8))) const void* lbl_eu_8056AE30[12] = {
    (const void*)0, (const void*)0xFFFFFFFF, (const void*)&ReadNextCharUTF8__Q34nw4r2ut14CharStrmReaderFv,
    (const void*)0, (const void*)0xFFFFFFFF, (const void*)&ReadNextCharUTF16__Q34nw4r2ut14CharStrmReaderFv,
    (const void*)0, (const void*)0xFFFFFFFF, (const void*)&ReadNextCharSJIS__Q34nw4r2ut14CharStrmReaderFv,
    (const void*)0, (const void*)0xFFFFFFFF, (const void*)&ReadNextCharCP1252__Q34nw4r2ut14CharStrmReaderFv,
};
__declspec(section ".data") __attribute__((aligned(8))) const void* lbl_eu_8056AE60[3] = {
    (const void*)0, (const void*)0xFFFFFFFF, (const void*)&ReadNextCharCP1252__Q34nw4r2ut14CharStrmReaderFv,
};
}
