#include "kyoshin/CNandData.hpp"

// ---------------------------------------------------------------------------
// NAND save-slot display data. The retail CNandData split object carries NO
// data sections: every static lives in the retail data objects (split1.s)
// under flat address labels, declared in CNandData.hpp. The Japanese
// title/description strings are unused by any retail code path and are not
// emitted at all.
// ---------------------------------------------------------------------------

// The singleton instance itself stays DEFINED here: MWCC emits the 0x10-byte
// boundary structure (retail lbl_eu_80577348) before it for the __sinit_
// __register_global_object call, and the postprocess strip turns both into
// UNDEF references to the retail .bss symbols.
CNandData CNandData::sInstance;

// Initializes the NAND save banner shown in the system menu:
//  - title / description fetched from BDAT,
//  - banner image filename and the icon filenames + frame counts,
//  - then clears the banner's graphic-layout bit (unkC & ~0x10).
void func_eu_802B11C0()
{
    const char* title = func_eu_802B14A4();
    CNandData::sInstance.mNandBanner.func_804F52F8(title);

    const char* desc = func_eu_802B14BC();
    CNandData::sInstance.mNandBanner.func_804F5304(desc);

    CNandData::sInstance.mNandBanner.func_804F5310(lbl_eu_80662E00);
    CNandData::sInstance.mNandBanner.func_804F531C(lbl_eu_8053A3D8, lbl_eu_8053A3FC);
    CNandData::sInstance.mNandBanner.unkC &= ~0x10;
}
