#pragma once

#include <types.h>
#include "monolib/work/CEventFile.hpp"
#include "monolib/device/CDeviceFile.hpp"
#include "monolib/util/FixStr.hpp"

namespace cf {

// 8-byte entry in the file-event lookup tables (sections at mData+0x000 /
// +0x100 / +0x200 / +0x400 / +0x600). func_80069C78 matches entries by mKey;
// func_80069C28 returns the first entry whose mField00 is non-zero.
struct CfTFileEntry {
    u32 mField00;  // 0x00
    u16 mField04;  // 0x04
    u16 mKey;      // 0x06
};  // size 0x08

// 0x628-byte payload at CfTFile+0x834 (cleared/copied as one block).
struct CfTFileData {
    CfTFileEntry mEntries0[0x20];  // 0x000
    CfTFileEntry mEntries1[0x20];  // 0x100
    CfTFileEntry mEntries2[0x40];  // 0x200
    CfTFileEntry mEntries3[0x40];  // 0x400
    CfTFileEntry mEntries4[0x03];  // 0x600
    u16 mCount0;                   // 0x618
    u16 mCount1;                   // 0x61A
    u16 mCount2;                   // 0x61C
    u16 mCount3;                   // 0x61E
    u16 mCount4;                   // 0x620
    u8 mPad[0x628 - 0x622];        // 0x622
};  // size 0x628

class CfTFile {
public:
    CfTFile();
    virtual ~CfTFile();
    virtual int OnFileEvent(CEventFile* pEventFile);

    // 0x000: vtable (retail lbl_eu_80526F98, stored by __ct__cf_CfTFile)
    u8 mPad004[0x824 - 0x004];  // 0x004
    u8* mDataAligned;           // 0x824 (32-byte aligned sub-buffer)
    CFileHandle* mFile;         // 0x828 (active file handle, CDeviceFile::cancel target)
    u32 mField82C;              // 0x82C
    u32 mField830;              // 0x830
    CfTFileData mData;          // 0x834
};

} // namespace cf

// C-ABI helper imports referenced by this TU. func_8006A53C is defined in
// CfTFile.cpp; func_800C1CAC is defined in the code_800C17DC TU (its C++
// definition there is emitted under the retail C name via the symbol map).
// The remaining entries are cross-TU imports: retail-unmangled C names, plus
// the cf::CfGameManager statics whose identifiers already carry the C++
// mangling and are therefore emitted verbatim. Declared at global scope
// (outside namespace cf) so the TU's global-scope helpers can call them.
extern "C" {
__declspec(noinline) int func_8006A53C(u16 a, u16 b);
int func_800AA33C(ml::FixStr<64>& buf, u32 packed, int prefixFlag, int suffixFlag);
int getFileSize__11CDeviceFileFPCc(const char* pPath, int flags);
void func_800C1CAC(u32 arg0, u32 arg1);
void* func_8003AA34();
int CfRes_getD80Flag();
u32 getBdatStringColumnValue(void* bdat, const char* column, int index);
u32 func_80086B1C__Q22cf13CfGameManagerFv();
u32 func_80086B2C__Q22cf13CfGameManagerFv();
}

// CfTFile vtable (retail .data:0x80526F98).
extern u8 lbl_eu_80526F98[];

// --- File-event / timer state globals (retail sbss symbols) ---

// File event frame timer (sbss, packed layout):
//   bits 0-5:   60 Hz sub-second counter (0-59)
//   bits 6-11:  second counter (0-59)
//   bits 12-31: minute counter (0-9999)
extern u32 lbl_eu_80663DA8;

// Bitfield view of the packed frame timer (big-endian: first field = MSBs).
union CfFrameTimer {
    u32 raw;
    struct {
        u32 min : 20;    // bits 12-31
        u32 sec : 6;     // bits 6-11
        u32 subsec : 6;  // bits 0-5
    };
};

// Shared play-time state used by the CfTFile helpers (sbss).
extern u32 lbl_eu_80663D90;
extern float lbl_eu_80663D94;
extern float lbl_eu_80663D98;
extern float lbl_eu_80663D9C;
extern u8 lbl_eu_80663DA0;
extern u32 lbl_eu_80663DA4;

// File-event flags / counters (sdata).
extern float lbl_eu_80661AF0;
extern u16 lbl_eu_80661AF4;
extern u16 lbl_eu_80661AF6;
extern u16 lbl_eu_80661AF8;

// Clock-display string table (bdat column names) and the static fallback
// time tables indexed by display mode.
extern char lbl_eu_804FB40C[];
extern char lbl_eu_804FB420[];
extern u16 lbl_eu_80527030[];
extern u16 lbl_eu_8052703C[];

// Float/double pool constants (retail .sdata2 entries used by the helpers).
extern const double lbl_eu_80666220; // 2^52 (u16 -> float conversion magic)
extern const float lbl_eu_80666228;  // play-clock display threshold
extern const double lbl_eu_80666230; // fmod divisor for the discarded frame-mod call
extern const float lbl_eu_80666238;
extern const float lbl_eu_8066623C;
extern const double lbl_eu_80666240;
extern const float lbl_eu_80666248;
extern const double lbl_eu_80666250;
extern const float lbl_eu_80666258;
extern const double lbl_eu_80666260;
