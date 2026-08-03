#include <revolution/OS.h>
#include <revolution/SC.h>
#include <string.h>

#define XOR_KEY 0x73B5DBFA

typedef struct SCArea {
    s8 area;      // at 0x0
    char name[4]; // at 0x1
} SCArea;

typedef struct SCRegion {
    s8 region;    // at 0x0
    char name[3]; // at 0x1
} SCRegion;

static SCArea ProductAreaAndStringTbl[] = {{SC_AREA_JPN, "JPN"},
                                           {SC_AREA_USA, "USA"},
                                           {SC_AREA_EUR, "EUR"},
                                           {SC_AREA_AUS, "AUS"},
                                           {SC_AREA_BRA, "BRA"},
                                           {SC_AREA_TWN, "TWN"},
                                           {SC_AREA_TWN, "ROC"},
                                           {SC_AREA_KOR, "KOR"},
                                           {SC_AREA_HKG, "HKG"},
                                           {SC_AREA_ASI, "ASI"},
                                           {SC_AREA_LTN, "LTN"},
                                           {SC_AREA_SAF, "SAF"},
                                           {SC_AREA_CHN, "CHN"},
                                           {-1, ""}};

static SCRegion ProductGameRegionAndStringTbl[] = {{SC_REGION_JP, "JP"},
                                                   {SC_REGION_US, "US"},
                                                   {SC_REGION_EU, "EU"},
                                                   {SC_REGION_KR, "KR"},
                                                   {SC_REGION_CN, "CN"},
                                                   {-1, ""}};

// "AREA"/"GAME" settings keys; 8-byte objects so the .sdata layout matches
// the retail DOL (each string is 8-byte aligned in the retail .sdata).
static char ProductAreaString[8] = "AREA";
static char ProductGameRegionString[8] = "GAME";

BOOL __SCF1(const char* type, char* buf, u32 sz) {
    u8 ptext;
    BOOL found = FALSE;
    u32 i;
    const u8* settings = (const u8*)OSPhysicalToCached(OS_PHYS_SC_PRDINFO);
    u32 key = XOR_KEY;
    u32 typeOfs = 0;
    u32 bufOfs = 0;

    for (i = 0; i < SC_PRDINFO_SIZE; i++, key = key >> 31 | key << 1) {
        ptext = settings[i];

        if (ptext != 0x00) {
            ptext ^= key;
            if (type[typeOfs] == 0x00 && ptext == '=') {
                found = TRUE;
                break;
            }

            if (((ptext ^ type[typeOfs]) & 0xDF) == 0) {
                typeOfs++;
            } else {
                typeOfs = 0;
            }
        }
    }

    if (found) {
        for (i++; i < SC_PRDINFO_SIZE && bufOfs < sz; i++) {
            key = key >> 31 | key << 1;
            ptext = settings[i];

            if (ptext != 0x00) {
                ptext ^= key;
                if (ptext == '\r' || ptext == '\n') {
                    ptext = 0x00;
                }
            }

            buf[bufOfs++] = ptext;

            if (ptext == 0x00) {
                return TRUE;
            }
        }
    }

    return FALSE;
}

s8 SCGetProductArea(void) {
    s8 area;
    char name[4];
    SCArea* iter = ProductAreaAndStringTbl;

    if (__SCF1(ProductAreaString, name, sizeof(name))) {
        for (; (area = iter->area) != -1; iter++) {
            if (!strcmp(iter->name, name)) {
                return area;
            }
        }
    }

    return -1;
}

s8 SCGetProductGameRegion(void) {
    s8 area;
    char name[3];
    SCRegion* iter = ProductGameRegionAndStringTbl;

    if (__SCF1(ProductGameRegionString, name, sizeof(name))) {
        for (; (area = iter->region) != -1; iter++) {
            if (!strcmp(iter->name, name)) {
                return area;
            }
        }
    }

    return -1;
}
