#pragma once

#include <types.h>
#include "monolib/nand/CNBanner.hpp"

// BDAT string getters for the NAND save banner title/description
// (defined in kyoshin/CErrMes.cpp).
const char* func_eu_802B14A4();
const char* func_eu_802B14BC();

// Module init routine that populates the NAND save banner singleton
// (defined in kyoshin/CNandData.cpp, called from kyoshin/main.cpp).
void func_eu_802B11C0();

// Banner image file pointer (retail .sdata object, SDA21-referenced).
extern const char* lbl_eu_80662E00;

// Ordered list of up to 8 icon textures + parallel per-icon frame counts
// (retail .data arrays, 9 entries each).
extern const char* lbl_eu_8053A3D8[9];
extern int lbl_eu_8053A3FC[9];

// Japanese-region NAND save display data. The single sInstance owns the
// CNBanner that is configured from the string/array statics above.
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
    static CNandData sInstance;
};
