#include "kyoshin/CNandData.hpp"

CNandData CNandData::sInstance;

const char* CNandData::scNandBannerFilename = "bannerTexture.tpl";

const char* CNandData::sNandIconTextureFilenames[9] = {
    "iconTexture[0].tpl",
    "iconTexture[1].tpl",
    "iconTexture[2].tpl",
    "iconTexture[3].tpl",
    "iconTexture[4].tpl",
    "iconTexture[5].tpl",
    "iconTexture[6].tpl",
    "iconTexture[7].tpl",
    0,
};

int CNandData::sNandIconTextureSomething[9] = {
    2, 2, 2, 2, 2, 2, 2, 2, 0,
};

// Initializes the NAND save banner shown in the system menu:
//  - title / description fetched from BDAT,
//  - banner image filename and the icon filenames + frame counts,
//  - then clears the banner's graphic-layout bit (unkC & ~0x10).
void NandData_SetupBanner_11C0()
{
    const char* title = getErrMesText19();
    CNandData::sInstance.mNandBanner.setTitle(title);

    const char* desc = getErrMesText20();
    CNandData::sInstance.mNandBanner.setSubtitle(desc);

    CNandData::sInstance.mNandBanner.setBannerPath(CNandData::scNandBannerFilename);
    CNandData::sInstance.mNandBanner.setFileList(CNandData::sNandIconTextureFilenames, CNandData::sNandIconTextureSomething);
    CNandData::sInstance.mNandBanner.mFlags &= ~0x10;
}
