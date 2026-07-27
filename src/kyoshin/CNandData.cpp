#include "kyoshin/CNandData.hpp"

const char* CNandData::scNandTitleText = "ゼノブレイド";
const char* CNandData::scNandDescriptionText = "巨神と機神　モナドが未来を斬り開く";
const char* CNandData::scNandBannerFilename = "bannerTexture.tpl";
const char* CNandData::sNandIconTextureFilenames[] = {
    "iconTexture[0].tpl",
    "iconTexture[1].tpl",
    "iconTexture[2].tpl",
    "iconTexture[3].tpl",
    "iconTexture[4].tpl",
    "iconTexture[5].tpl",
    "iconTexture[6].tpl",
    "iconTexture[7].tpl",
    nullptr
};
int CNandData::sNandIconTextureSomething[] = {
    2,
    2,
    2,
    2,
    2,
    2,
    2,
    2,
    0
};

// Forward declarations for functions in CErrMes unit that return BDAT strings
const char* func_eu_802B14A4();
const char* func_eu_802B14BC();

CNandData CNandData::sInstance;

// Initializes the NAND banner with title, description, icon filenames, and icon counts.
// Clears the banner's bit 0x10 flag after setup.
extern "C" void func_eu_802B11C0()
{
    const char* title = func_eu_802B14A4();
    CNandData::sInstance.mNandBanner.func_804F52F8(title);

    const char* desc = func_eu_802B14BC();
    CNandData::sInstance.mNandBanner.func_804F5304(desc);

    CNandData::sInstance.mNandBanner.func_804F5310(CNandData::scNandBannerFilename);
    CNandData::sInstance.mNandBanner.func_804F531C(CNandData::sNandIconTextureFilenames, CNandData::sNandIconTextureSomething);
    CNandData::sInstance.mNandBanner.unkC &= ~0x10;
}
