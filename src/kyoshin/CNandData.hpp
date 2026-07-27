#pragma once

#include "monolib/nand/CNBanner.hpp"

extern "C" void func_eu_802B11C0();

class CNandData {
public:
    CNBanner mNandBanner;

    CNandData() : mNandBanner() {
    }

    ~CNandData(){
    }

    friend void func_eu_802B11C0();

private:
    static const char* scNandTitleText;
    static const char* scNandDescriptionText;
    static const char* scNandBannerFilename;
    static const char* sNandIconTextureFilenames[];
    static int sNandIconTextureSomething[];
    static CNandData sInstance;
};
