#pragma once

#include "monolib/nand/CNBanner.hpp"

// Unmangled global C symbols (retail func_eu_802B11C0). Declared with C
// linkage to match the retail symbols.txt names exactly.
extern "C" void func_eu_802B11C0();

// Japanese-region NAND save display data. The single sInstance owns the
// CNBanner that is configured from the string/array statics below.
class CNandData {
public:
    CNBanner mNandBanner;

    CNandData() : mNandBanner() {
    }

    ~CNandData() {
    }

    // Allows the module init routine to reach the private statics.
    friend void func_eu_802B11C0();

private:
    static const char* scNandTitleText;
    static const char* scNandDescriptionText;
    static const char* scNandBannerFilename;
    static const char* sNandIconTextureFilenames[];
    static int sNandIconTextureSomething[];
    static CNandData sInstance;
};