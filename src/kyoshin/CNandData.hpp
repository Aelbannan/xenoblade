#pragma once

#include <types.h>
#include "monolib/nand/CNBanner.hpp"

// BDAT string getters for the NAND save banner title/description
// (defined in kyoshin/CErrMes.cpp with C linkage, matching the retail
// flat relocs -- keep extern "C" so call sites emit unmangled names).
extern "C" {
const char* getErrMesText19();
const char* getErrMesText20();
}

// Module init routine that populates the NAND save banner singleton
// (defined in kyoshin/CNandData.cpp, called from kyoshin/main.cpp).
void NandData_SetupBanner_11C0();

class CNandData {
public:
    CNBanner mNandBanner;

    CNandData() : mNandBanner() {
    }

    ~CNandData() {
    }

    // Allows the module init routine and NAND save paths to reach the
    // single banner instance without spelling the static data member's
    // mangled name at each call site.
    friend void NandData_SetupBanner_11C0();

    static CNandData& instance() { return sInstance; }

private:
    static CNandData sInstance;

    static const char* scNandBannerFilename;
    static const char* sNandIconTextureFilenames[9];
    static int sNandIconTextureSomething[9];
};
