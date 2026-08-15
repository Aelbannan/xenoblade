#include "kyoshin/CNandData.hpp"

// ---------------------------------------------------------------------------
// NAND save-slot display data. The retail CNandData split object carries NO
// data sections: every static lives in the retail data objects (split1.s)
// under flat address labels, referenced from this TU as typed externs with
// the exact retail names (config symbols.txt). The Japanese title/description
// strings are unused by any retail code path and are not emitted at all.
// ---------------------------------------------------------------------------

// Banner image file pointer (retail .sdata lbl_eu_80662E00, SDA21).
extern "C" const char* lbl_eu_80662E00;

// Ordered list of up to 8 icon textures + parallel per-icon frame counts
// (retail .data lbl_eu_8053A3D8 / lbl_eu_8053A3FC, 9 entries each).
extern "C" const char* lbl_eu_8053A3D8[9];
extern "C" int lbl_eu_8053A3FC[9];

// The singleton instance itself stays DEFINED here: MWCC emits the 0x10-byte
// boundary structure (retail lbl_eu_80577348) before it for the __sinit_
// __register_global_object call, and the postprocess strip turns both into
// UNDEF references to the retail .bss symbols.
CNandData CNandData::sInstance;

// Initializes the NAND save banner shown in the system menu:
//  - title / description fetched from BDAT,
//  - banner image filename and the icon filenames + frame counts,
//  - then clears the banner's graphic-layout bit (unkC & ~0x10).
// Unmangled global C symbol (retail func_eu_802B11C0) - C linkage required.
extern "C" void func_eu_802B11C0()
{
    const char* title = func_eu_802B14A4();
    CNandData::sInstance.mNandBanner.func_804F52F8(title);

    const char* desc = func_eu_802B14BC();
    CNandData::sInstance.mNandBanner.func_804F5304(desc);

    CNandData::sInstance.mNandBanner.func_804F5310(lbl_eu_80662E00);
    CNandData::sInstance.mNandBanner.func_804F531C(lbl_eu_8053A3D8, lbl_eu_8053A3FC);
    CNandData::sInstance.mNandBanner.unkC &= ~0x10;
}
