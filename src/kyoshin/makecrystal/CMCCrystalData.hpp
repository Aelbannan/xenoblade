#pragma once

#include <types.h>

// First word of a CItem_kindPtr item record (packed id word; >>20 is the
// sub-category).
struct CMCCrystalItemObj {
    u32 word0;
};

// One crystal-state entry (item index + selected flag). The default ctor is
// the inlined clear the CMCCrystalData ctor runs over the array; operator= is
// out of line (retail calls it from the second reset loop) and copies only
// the two named fields.
struct CMCCrystalDataEntry {
    s16 id;     // +0x00
    bool flag;  // +0x02

    CMCCrystalDataEntry() : id(-1), flag(false) {}
    CMCCrystalDataEntry& operator=(const CMCCrystalDataEntry& other);
};

// Crystal-state table embedded in CMCCrystalBox at +0x3D8 (retail ctor
// 0x802152E0, dtor 0x80215388 in code_80213488). 1024 entries paged 30 per
// row, a small state header, then the name buffer for getItemName.
class CMCCrystalData {
public:
    CMCCrystalData();
    ~CMCCrystalData();

    void collectByTarget(u8 target);
    void setFlag(int idx, bool val);
    bool getFlag(u8 idx);
    int allFlagsSet();
    void nextPage();
    void prevPage();
    u16 getSubCategory(u8 idx);
    CMCCrystalItemObj* getItemObject(u8 idx);

    // Defined in CMCCrystalBox.cpp (retail 0x80215710..0x80215A48).
    char* getItemName(int idx);
    void swapEntry(CMCCrystalDataEntry* a, CMCCrystalDataEntry* b);
    void sortIdOrder();
    void sortKeyOrder();

    CMCCrystalDataEntry entries[0x400]; // +0x0000
    u16 count;                          // +0x1000 filled-entry count
    u8  category;                       // +0x1002 item category
    u8  limit;                          // +0x1003 page count
    s8  current;                        // +0x1004 current page
    char name[0x10A8 - 0x1005];         // +0x1005 getItemName buffer
};
