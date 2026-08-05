#include "kyoshin/CNandData.hpp"

// ---------------------------------------------------------------------------
// NAND save-slot display data (banner, title, description, icons) injected at
// the nearest explicit link boundary. These string/array statics populate the
// read-only .data slice of this TU.
// ---------------------------------------------------------------------------

// System-menu NAND banner title / description (Japanese region only).
const char* CNandData::scNandTitleText = "ゼノブレイド";
const char* CNandData::scNandDescriptionText = "巨神と機神　モナドが未来を斬り開く";

// Banner image file embedded in the save data.
const char* CNandData::scNandBannerFilename = "bannerTexture.tpl";

// Ordered list of up to 8 icon textures; nullptr marks the end of the list.
const char* CNandData::sNandIconTextureFilenames[] = {
    "iconTexture[0].tpl",
    "iconTexture[1].tpl",
    "iconTexture[2].tpl",
    "iconTexture[3].tpl",
    "iconTexture[4].tpl",
    "iconTexture[5].tpl",
    "iconTexture[6].tpl",
    "iconTexture[7].tpl",
    nullptr,
};

// Per-icon frame count, parallel to sNandIconTextureFilenames (2 frames each,
// terminating 0). 0x8053A3FC.
int CNandData::sNandIconTextureSomething[] = {
    2, 2, 2, 2, 2, 2, 2, 2, 0,
};

// ---------------------------------------------------------------------------
// Cross-TU BDAT string getters (defined in the CErrMes TU).
//
// These functions (and the init routine below) are emitted by retail as plain
// UNMANGLED global C symbols (func_eu_802B14A4 / func_eu_802B14BC), so C
// linkage is mandatory here - C++ linkage would mangle the call target to
// `...__Fv` and break the FULL_MATCH. They return BDAT-driven display strings.
// ---------------------------------------------------------------------------
extern "C" const char* func_eu_802B14A4();
extern "C" const char* func_eu_802B14BC();

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

    CNandData::sInstance.mNandBanner.func_804F5310(CNandData::scNandBannerFilename);
    CNandData::sInstance.mNandBanner.func_804F531C(
        CNandData::sNandIconTextureFilenames, CNandData::sNandIconTextureSomething);
    CNandData::sInstance.mNandBanner.unkC &= ~0x10;
}