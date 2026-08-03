// High-level C reconstruction of RVL_SDK/src/revolution/mix
//
// MIX mixer (predecessor of the HBM fork in revolution/hbm/mix.c).
// Maintains 96 internal mixer channels (0x70 bytes each) backed by a
// static array; __MIXChannel / __MIXRmtChannel are pointers into it.
//
// Volume/pan tables are data-exact copies of the retail .data region
// (build/us/asm/.../mix.s): __MIXVolumeTable[968] followed contiguously
// by the pan table (0x790) and the DPL2 front (0x990) / rear (0xA90)
// tables. MWCC folds the adjacent arrays into base-relative addressing.

#include <types.h>
#include <string.h>
#include <revolution/ax/AXVPB.h>
#include <revolution/os/OSInterrupt.h>

typedef struct MIXChannel {
    AXVPB* vpb;       // at 0x00 - attached AX voice (NULL = free slot)
    u32 flags;        // at 0x04 - pending update flags
    s32 input;        // at 0x08 - main input volume (dB x10)
    s32 auxA;         // at 0x0C - AuxA send volume (dB x10)
    s32 auxB;         // at 0x10 - AuxB send volume (dB x10)
    s32 pan;          // at 0x14 - pan [0, 127]
    s32 panL;         // at 0x18 - left pan [0, 127]
    s32 panR;         // at 0x1C - right pan [0, 127]
    s32 fader;        // at 0x20 - fader volume (dB x10)
    s32 vL;           // at 0x24 - computed pan gain
    s32 vR;           // at 0x28 - computed pan gain
    s32 vSL;          // at 0x2C - computed side gain
    s32 vSR;          // at 0x30 - computed side gain
    s32 vRL;          // at 0x34 - DPL2 rear-left gain
    s32 vRR;          // at 0x38 - DPL2 rear-right gain
    u16 volMainCur;   // at 0x3C - main volume (current)
    u16 volMainTgt;   // at 0x3E - main volume (target)
    u16 volLCur;      // at 0x40 - L volume (current)
    u16 volLTgt;      // at 0x42 - L volume (target)
    u16 volRCur;      // at 0x44 - R volume (current)
    u16 volRTgt;      // at 0x46 - R volume (target)
    u16 volSCur;      // at 0x48 - S volume (current)
    u16 volSTgt;      // at 0x4A - S volume (target)
    u16 volALCur;     // at 0x4C - AuxA-L volume (current)
    u16 volALTgt;     // at 0x4E - AuxA-L volume (target)
    u16 volARCur;     // at 0x50 - AuxA-R volume (current)
    u16 volARTgt;     // at 0x52 - AuxA-R volume (target)
    u16 volASCur;     // at 0x54 - AuxA-S volume (current)
    u16 volASTgt;     // at 0x56 - AuxA-S volume (target)
    u16 volBLCur;     // at 0x58 - AuxB-L volume (current)
    u16 volBLTgt;     // at 0x5A - AuxB-L volume (target)
    u16 volBRCur;     // at 0x5C - AuxB-R volume (current)
    u16 volBRTgt;     // at 0x5E - AuxB-R volume (target)
    u16 volBSCur;     // at 0x60 - AuxB-S volume (current)
    u16 volBSTgt;     // at 0x62 - AuxB-S volume (target)
    u16 volCLCur;     // at 0x64 - AuxC-L volume (current)
    u16 volCLTgt;     // at 0x66 - AuxC-L volume (target)
    u16 volCRCur;     // at 0x68 - AuxC-R volume (current)
    u16 volCRTgt;     // at 0x6A - AuxC-R volume (target)
    u16 volCSCur;     // at 0x6C - AuxC-S volume (current)
    u16 volCSTgt;     // at 0x6E - AuxC-S volume (target)
} MIXChannel; // size 0x70

// Sound modes (__MIXSoundMode)
#define MIX_MODE_MONO  0
#define MIX_MODE_STEREO 1
#define MIX_MODE_DPL2  3

// dB limits for the volume table (dB x 10)
#define MIX_DB_MIN (-904)
#define MIX_DB_MAX 60

// Channel update flags (MIXChannel.flags)
#define MIX_FLAG_INPUT 0x10000000u // input volume changed
#define MIX_FLAG_MAIN  0x20000000u // main volume target ready -> apply
#define MIX_FLAG_MIX   0x40000000u // auxA/pan/fader changed
#define MIX_FLAG_APPLY 0x80000000u // mix targets ready -> apply

u16 __MIXVolumeTable[968] = {
    0x0000, 0x0001, 0x0001, 0x0001, 0x0001, 0x0001, 0x0001, 0x0001, 0x0001, 0x0001, 0x0001, 0x0001,
    0x0001, 0x0001, 0x0001, 0x0001, 0x0001, 0x0001, 0x0001, 0x0001, 0x0001, 0x0001, 0x0001, 0x0001,
    0x0001, 0x0001, 0x0001, 0x0001, 0x0001, 0x0001, 0x0001, 0x0001, 0x0001, 0x0001, 0x0001, 0x0001,
    0x0001, 0x0001, 0x0001, 0x0001, 0x0001, 0x0001, 0x0001, 0x0001, 0x0001, 0x0001, 0x0001, 0x0001,
    0x0001, 0x0001, 0x0001, 0x0001, 0x0001, 0x0001, 0x0001, 0x0001, 0x0001, 0x0001, 0x0001, 0x0001,
    0x0001, 0x0001, 0x0002, 0x0002, 0x0002, 0x0002, 0x0002, 0x0002, 0x0002, 0x0002, 0x0002, 0x0002,
    0x0002, 0x0002, 0x0002, 0x0002, 0x0002, 0x0002, 0x0002, 0x0002, 0x0002, 0x0002, 0x0002, 0x0002,
    0x0002, 0x0002, 0x0002, 0x0002, 0x0002, 0x0002, 0x0002, 0x0002, 0x0002, 0x0002, 0x0002, 0x0002,
    0x0002, 0x0003, 0x0003, 0x0003, 0x0003, 0x0003, 0x0003, 0x0003, 0x0003, 0x0003, 0x0003, 0x0003,
    0x0003, 0x0003, 0x0003, 0x0003, 0x0003, 0x0003, 0x0003, 0x0003, 0x0003, 0x0003, 0x0003, 0x0003,
    0x0003, 0x0003, 0x0004, 0x0004, 0x0004, 0x0004, 0x0004, 0x0004, 0x0004, 0x0004, 0x0004, 0x0004,
    0x0004, 0x0004, 0x0004, 0x0004, 0x0004, 0x0004, 0x0004, 0x0004, 0x0004, 0x0005, 0x0005, 0x0005,
    0x0005, 0x0005, 0x0005, 0x0005, 0x0005, 0x0005, 0x0005, 0x0005, 0x0005, 0x0005, 0x0005, 0x0005,
    0x0005, 0x0006, 0x0006, 0x0006, 0x0006, 0x0006, 0x0006, 0x0006, 0x0006, 0x0006, 0x0006, 0x0006,
    0x0006, 0x0006, 0x0007, 0x0007, 0x0007, 0x0007, 0x0007, 0x0007, 0x0007, 0x0007, 0x0007, 0x0007,
    0x0007, 0x0007, 0x0008, 0x0008, 0x0008, 0x0008, 0x0008, 0x0008, 0x0008, 0x0008, 0x0008, 0x0008,
    0x0009, 0x0009, 0x0009, 0x0009, 0x0009, 0x0009, 0x0009, 0x0009, 0x0009, 0x000A, 0x000A, 0x000A,
    0x000A, 0x000A, 0x000A, 0x000A, 0x000A, 0x000A, 0x000B, 0x000B, 0x000B, 0x000B, 0x000B, 0x000B,
    0x000B, 0x000C, 0x000C, 0x000C, 0x000C, 0x000C, 0x000C, 0x000C, 0x000D, 0x000D, 0x000D, 0x000D,
    0x000D, 0x000D, 0x000D, 0x000E, 0x000E, 0x000E, 0x000E, 0x000E, 0x000E, 0x000F, 0x000F, 0x000F,
    0x000F, 0x000F, 0x0010, 0x0010, 0x0010, 0x0010, 0x0010, 0x0011, 0x0011, 0x0011, 0x0011, 0x0011,
    0x0012, 0x0012, 0x0012, 0x0012, 0x0012, 0x0013, 0x0013, 0x0013, 0x0013, 0x0013, 0x0014, 0x0014,
    0x0014, 0x0014, 0x0015, 0x0015, 0x0015, 0x0015, 0x0016, 0x0016, 0x0016, 0x0016, 0x0017, 0x0017,
    0x0017, 0x0018, 0x0018, 0x0018, 0x0018, 0x0019, 0x0019, 0x0019, 0x001A, 0x001A, 0x001A, 0x001A,
    0x001B, 0x001B, 0x001B, 0x001C, 0x001C, 0x001C, 0x001D, 0x001D, 0x001D, 0x001E, 0x001E, 0x001E,
    0x001F, 0x001F, 0x0020, 0x0020, 0x0020, 0x0021, 0x0021, 0x0021, 0x0022, 0x0022, 0x0023, 0x0023,
    0x0023, 0x0024, 0x0024, 0x0025, 0x0025, 0x0026, 0x0026, 0x0026, 0x0027, 0x0027, 0x0028, 0x0028,
    0x0029, 0x0029, 0x002A, 0x002A, 0x002B, 0x002B, 0x002C, 0x002C, 0x002D, 0x002D, 0x002E, 0x002E,
    0x002F, 0x002F, 0x0030, 0x0031, 0x0031, 0x0032, 0x0032, 0x0033, 0x0033, 0x0034, 0x0035, 0x0035,
    0x0036, 0x0037, 0x0037, 0x0038, 0x0038, 0x0039, 0x003A, 0x003A, 0x003B, 0x003C, 0x003D, 0x003D,
    0x003E, 0x003F, 0x003F, 0x0040, 0x0041, 0x0042, 0x0042, 0x0043, 0x0044, 0x0045, 0x0046, 0x0046,
    0x0047, 0x0048, 0x0049, 0x004A, 0x004B, 0x004B, 0x004C, 0x004D, 0x004E, 0x004F, 0x0050, 0x0051,
    0x0052, 0x0053, 0x0054, 0x0055, 0x0056, 0x0057, 0x0058, 0x0059, 0x005A, 0x005B, 0x005C, 0x005D,
    0x005E, 0x005F, 0x0060, 0x0061, 0x0062, 0x0064, 0x0065, 0x0066, 0x0067, 0x0068, 0x006A, 0x006B,
    0x006C, 0x006D, 0x006F, 0x0070, 0x0071, 0x0072, 0x0074, 0x0075, 0x0076, 0x0078, 0x0079, 0x007B,
    0x007C, 0x007E, 0x007F, 0x0080, 0x0082, 0x0083, 0x0085, 0x0087, 0x0088, 0x008A, 0x008B, 0x008D,
    0x008F, 0x0090, 0x0092, 0x0094, 0x0095, 0x0097, 0x0099, 0x009B, 0x009C, 0x009E, 0x00A0, 0x00A2,
    0x00A4, 0x00A6, 0x00A8, 0x00AA, 0x00AB, 0x00AD, 0x00AF, 0x00B2, 0x00B4, 0x00B6, 0x00B8, 0x00BA,
    0x00BC, 0x00BE, 0x00C0, 0x00C3, 0x00C5, 0x00C7, 0x00CA, 0x00CC, 0x00CE, 0x00D1, 0x00D3, 0x00D6,
    0x00D8, 0x00DB, 0x00DD, 0x00E0, 0x00E2, 0x00E5, 0x00E7, 0x00EA, 0x00ED, 0x00F0, 0x00F2, 0x00F5,
    0x00F8, 0x00FB, 0x00FE, 0x0101, 0x0104, 0x0107, 0x010A, 0x010D, 0x0110, 0x0113, 0x0116, 0x011A,
    0x011D, 0x0120, 0x0124, 0x0127, 0x012A, 0x012E, 0x0131, 0x0135, 0x0138, 0x013C, 0x0140, 0x0143,
    0x0147, 0x014B, 0x014F, 0x0153, 0x0157, 0x015B, 0x015F, 0x0163, 0x0167, 0x016B, 0x016F, 0x0173,
    0x0178, 0x017C, 0x0180, 0x0185, 0x0189, 0x018E, 0x0193, 0x0197, 0x019C, 0x01A1, 0x01A6, 0x01AB,
    0x01AF, 0x01B4, 0x01BA, 0x01BF, 0x01C4, 0x01C9, 0x01CE, 0x01D4, 0x01D9, 0x01DF, 0x01E4, 0x01EA,
    0x01EF, 0x01F5, 0x01FB, 0x0201, 0x0207, 0x020D, 0x0213, 0x0219, 0x021F, 0x0226, 0x022C, 0x0232,
    0x0239, 0x0240, 0x0246, 0x024D, 0x0254, 0x025B, 0x0262, 0x0269, 0x0270, 0x0277, 0x027E, 0x0286,
    0x028D, 0x0295, 0x029D, 0x02A4, 0x02AC, 0x02B4, 0x02BC, 0x02C4, 0x02CC, 0x02D5, 0x02DD, 0x02E6,
    0x02EE, 0x02F7, 0x0300, 0x0309, 0x0312, 0x031B, 0x0324, 0x032D, 0x0337, 0x0340, 0x034A, 0x0354,
    0x035D, 0x0367, 0x0371, 0x037C, 0x0386, 0x0390, 0x039B, 0x03A6, 0x03B1, 0x03BB, 0x03C7, 0x03D2,
    0x03DD, 0x03E9, 0x03F4, 0x0400, 0x040C, 0x0418, 0x0424, 0x0430, 0x043D, 0x0449, 0x0456, 0x0463,
    0x0470, 0x047D, 0x048A, 0x0498, 0x04A5, 0x04B3, 0x04C1, 0x04CF, 0x04DD, 0x04EC, 0x04FA, 0x0509,
    0x0518, 0x0527, 0x0536, 0x0546, 0x0555, 0x0565, 0x0575, 0x0586, 0x0596, 0x05A6, 0x05B7, 0x05C8,
    0x05D9, 0x05EB, 0x05FC, 0x060E, 0x0620, 0x0632, 0x0644, 0x0657, 0x066A, 0x067D, 0x0690, 0x06A4,
    0x06B7, 0x06CB, 0x06DF, 0x06F4, 0x0708, 0x071D, 0x0732, 0x0748, 0x075D, 0x0773, 0x0789, 0x079F,
    0x07B6, 0x07CD, 0x07E4, 0x07FB, 0x0813, 0x082B, 0x0843, 0x085C, 0x0874, 0x088E, 0x08A7, 0x08C1,
    0x08DA, 0x08F5, 0x090F, 0x092A, 0x0945, 0x0961, 0x097D, 0x0999, 0x09B5, 0x09D2, 0x09EF, 0x0A0D,
    0x0A2A, 0x0A48, 0x0A67, 0x0A86, 0x0AA5, 0x0AC5, 0x0AE5, 0x0B05, 0x0B25, 0x0B47, 0x0B68, 0x0B8A,
    0x0BAC, 0x0BCF, 0x0BF2, 0x0C15, 0x0C39, 0x0C5D, 0x0C82, 0x0CA7, 0x0CCC, 0x0CF2, 0x0D19, 0x0D3F,
    0x0D67, 0x0D8E, 0x0DB7, 0x0DDF, 0x0E08, 0x0E32, 0x0E5C, 0x0E87, 0x0EB2, 0x0EDD, 0x0F09, 0x0F36,
    0x0F63, 0x0F91, 0x0FBF, 0x0FEE, 0x101D, 0x104D, 0x107D, 0x10AE, 0x10DF, 0x1111, 0x1144, 0x1177,
    0x11AB, 0x11DF, 0x1214, 0x124A, 0x1280, 0x12B7, 0x12EE, 0x1326, 0x135F, 0x1399, 0x13D3, 0x140D,
    0x1449, 0x1485, 0x14C2, 0x14FF, 0x153E, 0x157D, 0x15BC, 0x15FD, 0x163E, 0x1680, 0x16C3, 0x1706,
    0x174A, 0x178F, 0x17D5, 0x181C, 0x1863, 0x18AC, 0x18F5, 0x193F, 0x198A, 0x19D5, 0x1A22, 0x1A6F,
    0x1ABE, 0x1B0D, 0x1B5D, 0x1BAE, 0x1C00, 0x1C53, 0x1CA7, 0x1CFC, 0x1D52, 0x1DA9, 0x1E01, 0x1E5A,
    0x1EB4, 0x1F0F, 0x1F6B, 0x1FC8, 0x2026, 0x2086, 0x20E6, 0x2148, 0x21AA, 0x220E, 0x2273, 0x22D9,
    0x2341, 0x23A9, 0x2413, 0x247E, 0x24EA, 0x2557, 0x25C6, 0x2636, 0x26A7, 0x271A, 0x278E, 0x2803,
    0x287A, 0x28F2, 0x296B, 0x29E6, 0x2A62, 0x2AE0, 0x2B5F, 0x2BDF, 0x2C61, 0x2CE5, 0x2D6A, 0x2DF1,
    0x2E79, 0x2F03, 0x2F8E, 0x301B, 0x30AA, 0x313A, 0x31CC, 0x325F, 0x32F5, 0x338C, 0x3425, 0x34BF,
    0x355B, 0x35FA, 0x369A, 0x373C, 0x37DF, 0x3885, 0x392C, 0x39D6, 0x3A81, 0x3B2F, 0x3BDE, 0x3C90,
    0x3D43, 0x3DF9, 0x3EB1, 0x3F6A, 0x4026, 0x40E5, 0x41A5, 0x4268, 0x432C, 0x43F4, 0x44BD, 0x4589,
    0x4657, 0x4727, 0x47FA, 0x48D0, 0x49A8, 0x4A82, 0x4B5F, 0x4C3E, 0x4D20, 0x4E05, 0x4EEC, 0x4FD6,
    0x50C3, 0x51B2, 0x52A4, 0x5399, 0x5491, 0x558C, 0x5689, 0x578A, 0x588D, 0x5994, 0x5A9D, 0x5BAA,
    0x5CBA, 0x5DCD, 0x5EE3, 0x5FFC, 0x6119, 0x6238, 0x635C, 0x6482, 0x65AC, 0x66D9, 0x680A, 0x693F,
    0x6A77, 0x6BB2, 0x6CF2, 0x6E35, 0x6F7B, 0x70C6, 0x7214, 0x7366, 0x74BC, 0x7616, 0x7774, 0x78D6,
    0x7A3D, 0x7BA7, 0x7D16, 0x7E88, 0x7FFF, 0x817B, 0x82FB, 0x847F, 0x8608, 0x8795, 0x8927, 0x8ABE,
    0x8C59, 0x8DF9, 0x8F9E, 0x9148, 0x92F6, 0x94AA, 0x9663, 0x9820, 0x99E3, 0x9BAB, 0x9D79, 0x9F4C,
    0xA124, 0xA302, 0xA4E5, 0xA6CE, 0xA8BC, 0xAAB0, 0xACAA, 0xAEAA, 0xB0B0, 0xB2BC, 0xB4CE, 0xB6E5,
    0xB904, 0xBB28, 0xBD53, 0xBF84, 0xC1BC, 0xC3FA, 0xC63F, 0xC88B, 0xCADD, 0xCD37, 0xCF97, 0xD1FE,
    0xD46D, 0xD6E3, 0xD960, 0xDBE4, 0xDE70, 0xE103, 0xE39E, 0xE641, 0xE8EB, 0xEB9E, 0xEE58, 0xF11B,
    0xF3E6, 0xF6B9, 0xF994, 0xFC78, 0xFF64, 0x0000, 0x0000, 0x0000,
};

u32 __MIXPanTable[128] = {
    0x00000000, 0x00000000, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFE, 0xFFFFFFFE, 0xFFFFFFFE,
    0xFFFFFFFD, 0xFFFFFFFD, 0xFFFFFFFC, 0xFFFFFFFC, 0xFFFFFFFC, 0xFFFFFFFB, 0xFFFFFFFB, 0xFFFFFFFB,
    0xFFFFFFFA, 0xFFFFFFFA, 0xFFFFFFF9, 0xFFFFFFF9, 0xFFFFFFF9, 0xFFFFFFF8, 0xFFFFFFF8, 0xFFFFFFF7,
    0xFFFFFFF7, 0xFFFFFFF6, 0xFFFFFFF6, 0xFFFFFFF6, 0xFFFFFFF5, 0xFFFFFFF5, 0xFFFFFFF4, 0xFFFFFFF4,
    0xFFFFFFF3, 0xFFFFFFF3, 0xFFFFFFF2, 0xFFFFFFF2, 0xFFFFFFF2, 0xFFFFFFF1, 0xFFFFFFF1, 0xFFFFFFF0,
    0xFFFFFFF0, 0xFFFFFFEF, 0xFFFFFFEF, 0xFFFFFFEE, 0xFFFFFFEE, 0xFFFFFFED, 0xFFFFFFEC, 0xFFFFFFEC,
    0xFFFFFFEB, 0xFFFFFFEB, 0xFFFFFFEA, 0xFFFFFFEA, 0xFFFFFFE9, 0xFFFFFFE9, 0xFFFFFFE8, 0xFFFFFFE7,
    0xFFFFFFE7, 0xFFFFFFE6, 0xFFFFFFE6, 0xFFFFFFE5, 0xFFFFFFE4, 0xFFFFFFE4, 0xFFFFFFE3, 0xFFFFFFE2,
    0xFFFFFFE2, 0xFFFFFFE1, 0xFFFFFFE0, 0xFFFFFFDF, 0xFFFFFFDF, 0xFFFFFFDE, 0xFFFFFFDD, 0xFFFFFFDC,
    0xFFFFFFDC, 0xFFFFFFDB, 0xFFFFFFDA, 0xFFFFFFD9, 0xFFFFFFD8, 0xFFFFFFD8, 0xFFFFFFD7, 0xFFFFFFD6,
    0xFFFFFFD5, 0xFFFFFFD4, 0xFFFFFFD3, 0xFFFFFFD2, 0xFFFFFFD1, 0xFFFFFFD0, 0xFFFFFFCF, 0xFFFFFFCE,
    0xFFFFFFCD, 0xFFFFFFCC, 0xFFFFFFCA, 0xFFFFFFC9, 0xFFFFFFC8, 0xFFFFFFC7, 0xFFFFFFC5, 0xFFFFFFC4,
    0xFFFFFFC3, 0xFFFFFFC1, 0xFFFFFFC0, 0xFFFFFFBE, 0xFFFFFFBD, 0xFFFFFFBB, 0xFFFFFFB9, 0xFFFFFFB8,
    0xFFFFFFB6, 0xFFFFFFB4, 0xFFFFFFB2, 0xFFFFFFB0, 0xFFFFFFAD, 0xFFFFFFAB, 0xFFFFFFA9, 0xFFFFFFA6,
    0xFFFFFFA3, 0xFFFFFFA0, 0xFFFFFF9D, 0xFFFFFF9A, 0xFFFFFF96, 0xFFFFFF92, 0xFFFFFF8D, 0xFFFFFF88,
    0xFFFFFF82, 0xFFFFFF7B, 0xFFFFFF74, 0xFFFFFF6A, 0xFFFFFF5D, 0xFFFFFF4C, 0xFFFFFF2E, 0xFFFFFC78,
};

s16 __MIX_DPL2_front[128] = {
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    0x0000, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFE, 0xFFFE, 0xFFFE, 0xFFFE, 0xFFFD, 0xFFFD,
    0xFFFD, 0xFFFC, 0xFFFC, 0xFFFC, 0xFFFB, 0xFFFB, 0xFFFA, 0xFFFA, 0xFFFA, 0xFFF9, 0xFFF9, 0xFFF8,
    0xFFF8, 0xFFF7, 0xFFF7, 0xFFF6, 0xFFF5, 0xFFF5, 0xFFF4, 0xFFF4, 0xFFF3, 0xFFF2, 0xFFF2, 0xFFF1,
    0xFFF0, 0xFFEF, 0xFFEF, 0xFFEE, 0xFFED, 0xFFEC, 0xFFEB, 0xFFEB, 0xFFEA, 0xFFE9, 0xFFE8, 0xFFE7,
    0xFFE6, 0xFFE5, 0xFFE4, 0xFFE3, 0xFFE2, 0xFFE1, 0xFFE0, 0xFFDE, 0xFFDD, 0xFFDC, 0xFFDB, 0xFFDA,
    0xFFD8, 0xFFD7, 0xFFD6, 0xFFD4, 0xFFD3, 0xFFD1, 0xFFD0, 0xFFCE, 0xFFCC, 0xFFCB, 0xFFC9, 0xFFC7,
    0xFFC6, 0xFFC4, 0xFFC2, 0xFFC0, 0xFFBE, 0xFFBC, 0xFFBA, 0xFFB7, 0xFFB5, 0xFFB3, 0xFFB0, 0xFFAE,
    0xFFAB, 0xFFA8, 0xFFA6, 0xFFA3, 0xFFA0, 0xFF9C, 0xFF99, 0xFF96, 0xFF92, 0xFF8E, 0xFF8A, 0xFF86,
    0xFF82, 0xFF7D, 0xFF78, 0xFF73, 0xFF6E, 0xFF68, 0xFF61, 0xFF5A, 0xFF53, 0xFF4B, 0xFF42, 0xFF37,
    0xFF2C, 0xFF1F, 0xFF0F, 0xFEFB, 0xFEE2, 0xFEBF, 0xFE83, 0xFC40,
};

s16 __MIX_DPL2_rear[128] = {
    0xFFC3, 0xFFC3, 0xFFC4, 0xFFC5, 0xFFC5, 0xFFC6, 0xFFC6, 0xFFC7, 0xFFC8, 0xFFC8, 0xFFC9, 0xFFC9,
    0xFFCA, 0xFFCB, 0xFFCB, 0xFFCC, 0xFFCC, 0xFFCD, 0xFFCE, 0xFFCE, 0xFFCF, 0xFFCF, 0xFFD0, 0xFFD0,
    0xFFD1, 0xFFD1, 0xFFD2, 0xFFD2, 0xFFD3, 0xFFD3, 0xFFD4, 0xFFD4, 0xFFD5, 0xFFD5, 0xFFD6, 0xFFD6,
    0xFFD7, 0xFFD7, 0xFFD8, 0xFFD8, 0xFFD9, 0xFFD9, 0xFFDA, 0xFFDA, 0xFFDA, 0xFFDB, 0xFFDB, 0xFFDC,
    0xFFDC, 0xFFDD, 0xFFDD, 0xFFDD, 0xFFDE, 0xFFDE, 0xFFDF, 0xFFDF, 0xFFE0, 0xFFE0, 0xFFE0, 0xFFE1,
    0xFFE1, 0xFFE1, 0xFFE2, 0xFFE2, 0xFFE3, 0xFFE3, 0xFFE3, 0xFFE4, 0xFFE4, 0xFFE4, 0xFFE5, 0xFFE5,
    0xFFE5, 0xFFE6, 0xFFE6, 0xFFE6, 0xFFE7, 0xFFE7, 0xFFE7, 0xFFE8, 0xFFE8, 0xFFE8, 0xFFE9, 0xFFE9,
    0xFFE9, 0xFFEA, 0xFFEA, 0xFFEA, 0xFFEB, 0xFFEB, 0xFFEB, 0xFFEC, 0xFFEC, 0xFFEC, 0xFFEC, 0xFFED,
    0xFFED, 0xFFED, 0xFFEE, 0xFFEE, 0xFFEE, 0xFFEE, 0xFFEF, 0xFFEF, 0xFFEF, 0xFFEF, 0xFFF0, 0xFFF0,
    0xFFF0, 0xFFF0, 0xFFF1, 0xFFF1, 0xFFF1, 0xFFF1, 0xFFF2, 0xFFF2, 0xFFF2, 0xFFF2, 0xFFF3, 0xFFF3,
    0xFFF3, 0xFFF3, 0xFFF3, 0xFFF4, 0xFFF4, 0xFFF4, 0xFFF4, 0xFFF5,
};

// Mixer state (sbss globals)
extern MIXChannel* __MIXChannel;    // active channel table
extern MIXChannel* __MIXRmtChannel; // remote channel table
extern u32 __MIXSoundMode;          // current output mode
extern s32 __MIXMaxVoices;          // number of mixer channels (from AXGetMaxVoices)
extern s32 __init;                  // mixer initialized flag

// Backing BSS arrays (named per retail symbols.txt)
extern MIXChannel __s_MIXChannel[];
extern u8 __s_MIXRmtChannel[0x1988];

// AX header misses these
extern BOOL AXIsInit(void);

void __MIXRmtResetChannel(s32 idx);
void __MIXRmtUpdateSettings(s32 idx, void* out);

// Inline volume lookup: clamp the dB value to [-904, 60] and index the table.
// Retail inlines this pattern inside MIXInitChannel / MIXUpdateSettings; the
// local index forces the lhzx indexed-addressing form MWCC emits for the
// standalone __MIXGetVolume (see below).
//
// Sum-chain operand order is deliberate (matches retail load order): MWCC
// loads a 3-term sum s0+s1+s2 (no trailing constant) in [s2,s0,s1] order,
// loads a 3-term sum with a trailing constant in source order, and rotates a
// flat 4-term sum; explicit (a+b)+(c+d) parens load in written order.  See
// docs/MWCC_REFERENCE.md "RVL_SDK mix — MIXInitChannel".
static u16 MIXGetVolumeInline(s32 db)
{
    s32 idx;

    if (db <= MIX_DB_MIN) {
        return 0;
    }
    if (db >= MIX_DB_MAX) {
        return 0xFF64;
    }
    idx = db + 904;
    return __MIXVolumeTable[idx];
}

u16 __MIXGetVolume(s32 db) {
    s32 idx;

    if (db <= MIX_DB_MIN) {
        return 0;
    }
    if (db >= MIX_DB_MAX) {
        return 0xFF64;
    }
    idx = db + 904;
    return __MIXVolumeTable[idx];
}

void __MIXSetPan(MIXChannel* ch) {
    s32 pan = ch->panL;
    s32 panR = ch->panR;
    s32 invPan = 127 - pan;
    s32 invPanR = 127 - panR;

    if (__MIXSoundMode == MIX_MODE_DPL2) {
        ch->vL = __MIX_DPL2_front[pan];
        ch->vR = __MIX_DPL2_front[invPan];
        ch->vSL = __MIX_DPL2_front[invPanR];
        ch->vSR = __MIX_DPL2_front[panR];
        ch->vRL = __MIX_DPL2_rear[invPan];
        ch->vRR = __MIX_DPL2_rear[pan];
    } else {
        ch->vL = __MIXPanTable[pan];
        ch->vR = __MIXPanTable[invPan];
        ch->vSL = __MIXPanTable[invPanR];
        ch->vSR = __MIXPanTable[panR];
        ch->vRL = 0;
        ch->vRR = 0;
    }
}

void MIXInit(void) {
    u32 i;
    MIXChannel* ch;

    if (!AXIsInit()) {
        return;
    }
    if (__init) {
        return;
    }

    __MIXMaxVoices = AXGetMaxVoices();
    __MIXChannel = __s_MIXChannel;
    __MIXRmtChannel = (MIXChannel*)__s_MIXRmtChannel;

    for (i = 0; i < (u32)__MIXMaxVoices; i++) {
        __MIXChannel[i].vpb = NULL;
        ch = &__MIXChannel[i];
        ch->flags = 0x50000000;
        ch->input = 0;
        ch->auxA = -0x3C0;
        ch->auxB = -0x3C0;
        ch->pan = -0x3C0;
        ch->fader = 0;
        ch->panL = 0x40;
        ch->panR = 0x7F;
        {
            u16* v = (u16*)((u8*)ch + 0x6C);
            *v = 0; v -= 2; *v = 0; v -= 2; *v = 0; v -= 2; *v = 0; v -= 2;
            *v = 0; v -= 2; *v = 0; v -= 2; *v = 0; v -= 2; *v = 0; v -= 2;
            *v = 0; v -= 2; *v = 0; v -= 2; *v = 0; v -= 2; *v = 0; v -= 2;
            *v = 0;
        }
        __MIXSetPan(ch);
        __MIXRmtResetChannel(i);
    }

    __MIXSoundMode = MIX_MODE_STEREO;
    __init = 1;
}

void MIXQuit(void) {
    __MIXChannel = NULL;
    __MIXRmtChannel = NULL;
    __init = 0;
}

void MIXInitChannel(AXVPB* vpb, u32 flags, s32 input, s32 auxA, s32 auxB, s32 pan,
                    s32 panL, s32 panR, s32 fader)
{
    BOOL enabled;
    MIXChannel* ch = &__MIXChannel[vpb->index];
    u32 ctrl;
    u16* q;

    ch->vpb = vpb;
    ch->flags = flags & 0xF;
    ch->input = input;
    ch->auxA = auxA;
    ch->auxB = auxB;
    ch->pan = pan;
    ch->panL = panL;
    ch->panR = panR;
    ch->fader = fader;
    __MIXSetPan(ch);

    if (ch->flags & 0x8) {
        ch->volMainCur = 0;
    } else {
        ch->volMainCur = MIXGetVolumeInline(input);
    }

    ctrl = 0;

    switch (__MIXSoundMode) {
    case MIX_MODE_MONO:
        ch->volLCur = MIXGetVolumeInline(ch->fader + ch->vSL);
        ch->volRCur = MIXGetVolumeInline(ch->fader + ch->vSL);
        ch->volSCur = MIXGetVolumeInline(ch->fader + ch->vSR - 30);

        if (ch->flags & 0x1) {
            ch->volALCur = MIXGetVolumeInline(ch->auxA + ch->vSL);
            ch->volARCur = MIXGetVolumeInline(ch->auxA + ch->vSL);
            ch->volASCur = MIXGetVolumeInline(ch->auxA + ch->vSR - 30);
        } else {
            ch->volALCur = MIXGetVolumeInline(ch->fader + ch->auxA + ch->vSL);
            ch->volARCur = MIXGetVolumeInline(ch->fader + ch->auxA + ch->vSL);
            ch->volASCur = MIXGetVolumeInline(ch->fader + ch->auxA + ch->vSR - 30);
        }

        if (ch->flags & 0x2) {
            ch->volBLCur = MIXGetVolumeInline(ch->auxB + ch->vSL);
            ch->volBRCur = MIXGetVolumeInline(ch->auxB + ch->vSL);
            ch->volBSCur = MIXGetVolumeInline(ch->auxB + ch->vSR - 30);
        } else {
            ch->volBLCur = MIXGetVolumeInline(ch->fader + ch->auxB + ch->vSL);
            ch->volBRCur = MIXGetVolumeInline(ch->fader + ch->auxB + ch->vSL);
            ch->volBSCur = MIXGetVolumeInline(ch->fader + ch->auxB + ch->vSR - 30);
        }

        if (ch->flags & 0x4) {
            ch->volCLCur = MIXGetVolumeInline(ch->pan + ch->vSL);
            ch->volCRCur = MIXGetVolumeInline(ch->pan + ch->vSL);
            ch->volCSCur = MIXGetVolumeInline(ch->pan + ch->vSR - 30);
        } else {
            ch->volCLCur = MIXGetVolumeInline(ch->fader + ch->pan + ch->vSL);
            ch->volCRCur = MIXGetVolumeInline(ch->fader + ch->pan + ch->vSL);
            ch->volCSCur = MIXGetVolumeInline(ch->fader + ch->pan + ch->vSR - 30);
        }
        break;

    case MIX_MODE_STEREO:
    case 2:
        ch->volLCur = MIXGetVolumeInline(ch->fader + ch->vL + ch->vSL);
        ch->volRCur = MIXGetVolumeInline(ch->fader + ch->vR + ch->vSL);
        ch->volSCur = MIXGetVolumeInline(ch->fader + ch->vSR - 30);

        if (ch->flags & 0x1) {
            ch->volALCur = MIXGetVolumeInline(ch->auxA + ch->vL + ch->vSL);
            ch->volARCur = MIXGetVolumeInline(ch->auxA + ch->vR + ch->vSL);
            ch->volASCur = MIXGetVolumeInline(ch->auxA + ch->vSR - 30);
        } else {
            ch->volALCur = MIXGetVolumeInline((ch->vSL + ch->vL) + (ch->fader + ch->auxA));
            ch->volARCur = MIXGetVolumeInline((ch->vSL + ch->vR) + (ch->fader + ch->auxA));
            ch->volASCur = MIXGetVolumeInline(ch->fader + ch->auxA + ch->vSR - 30);
        }

        if (ch->flags & 0x2) {
            ch->volBLCur = MIXGetVolumeInline(ch->auxB + ch->vL + ch->vSL);
            ch->volBRCur = MIXGetVolumeInline(ch->auxB + ch->vR + ch->vSL);
            ch->volBSCur = MIXGetVolumeInline(ch->auxB + ch->vSR - 30);
        } else {
            ch->volBLCur = MIXGetVolumeInline((ch->vSL + ch->vL) + (ch->fader + ch->auxB));
            ch->volBRCur = MIXGetVolumeInline((ch->vSL + ch->vR) + (ch->fader + ch->auxB));
            ch->volBSCur = MIXGetVolumeInline(ch->fader + ch->auxB + ch->vSR - 30);
        }

        if (ch->flags & 0x4) {
            ch->volCLCur = MIXGetVolumeInline(ch->pan + ch->vL + ch->vSL);
            ch->volCRCur = MIXGetVolumeInline(ch->pan + ch->vR + ch->vSL);
            ch->volCSCur = MIXGetVolumeInline(ch->pan + ch->vSR - 30);
        } else {
            ch->volCLCur = MIXGetVolumeInline((ch->vSL + ch->vL) + (ch->fader + ch->pan));
            ch->volCRCur = MIXGetVolumeInline((ch->vSL + ch->vR) + (ch->fader + ch->pan));
            ch->volCSCur = MIXGetVolumeInline(ch->fader + ch->pan + ch->vSR - 30);
        }
        break;

    case MIX_MODE_DPL2:
        ch->volLCur = MIXGetVolumeInline(ch->fader + ch->vL + ch->vSL);
        ch->volRCur = MIXGetVolumeInline(ch->fader + ch->vR + ch->vSL);
        ch->volSCur = MIXGetVolumeInline(ch->fader + ch->vRL + ch->vSR);
        ch->volCLCur = MIXGetVolumeInline(ch->fader + ch->vRR + ch->vSR);

        if (ch->flags & 0x1) {
            ch->volALCur = MIXGetVolumeInline(ch->auxA + ch->vL + ch->vSL);
            ch->volARCur = MIXGetVolumeInline(ch->auxA + ch->vR + ch->vSL);
            ch->volASCur = MIXGetVolumeInline(ch->auxA + ch->vRL + ch->vSR);
            ch->volCRCur = MIXGetVolumeInline(ch->auxA + ch->vRR + ch->vSR);
        } else {
            ch->volALCur = MIXGetVolumeInline((ch->vSL + ch->vL) + (ch->fader + ch->auxA));
            ch->volARCur = MIXGetVolumeInline((ch->vSL + ch->vR) + (ch->fader + ch->auxA));
            ch->volASCur = MIXGetVolumeInline((ch->vSR + ch->vRL) + (ch->fader + ch->auxA));
            ch->volCRCur = MIXGetVolumeInline((ch->vSR + ch->vRR) + (ch->fader + ch->auxA));
        }

        if (ch->flags & 0x2) {
            ch->volBLCur = MIXGetVolumeInline(ch->auxB + ch->vL + ch->vSL);
            ch->volBRCur = MIXGetVolumeInline(ch->auxB + ch->vR + ch->vSL);
            ch->volBSCur = MIXGetVolumeInline(ch->auxB + ch->vRL + ch->vSR);
            ch->volCSCur = MIXGetVolumeInline(ch->auxB + ch->vRR + ch->vSR);
        } else {
            ch->volBLCur = MIXGetVolumeInline((ch->vSL + ch->vL) + (ch->fader + ch->auxB));
            ch->volBRCur = MIXGetVolumeInline((ch->vSL + ch->vR) + (ch->fader + ch->auxB));
            ch->volBSCur = MIXGetVolumeInline((ch->vSR + ch->vRL) + (ch->fader + ch->auxB));
            ch->volCSCur = MIXGetVolumeInline((ch->vSR + ch->vRR) + (ch->fader + ch->auxB));
        }

        ctrl |= 0x80000000;
        break;
    }

    __MIXRmtResetChannel(vpb->index);

    enabled = OSDisableInterrupts();

    vpb->pb.ve.currentVolume = ch->volMainCur;
    vpb->pb.ve.currentDelta = 0;

    // Walk pb.mix as a u16 stream: MWCC keeps `q` in a base register for the
    // whole unrolled delta/value copy (retail materialises addi r3,r28,0x3e).
    // Plain field stores fold back to direct offsets and break the match.
    q = &vpb->pb.mix.vL;
    *q++ = ch->volLCur;
    if (vpb->pb.mix.vL != 0) {
        ctrl |= AX_MIXER_CTRL_L;
    }

    *q++ = 0;
    *q++ = ch->volRCur;
    if (vpb->pb.mix.vR != 0) {
        ctrl |= AX_MIXER_CTRL_R;
    }
    *q++ = 0;
    *q++ = ch->volALCur;
    if (vpb->pb.mix.vAuxAL != 0) {
        ctrl |= AX_MIXER_CTRL_AL;
    }
    *q++ = 0;
    *q++ = ch->volARCur;
    if (vpb->pb.mix.vAuxAR != 0) {
        ctrl |= AX_MIXER_CTRL_AR;
    }
    *q++ = 0;
    *q++ = ch->volBLCur;
    if (vpb->pb.mix.vAuxBL != 0) {
        ctrl |= AX_MIXER_CTRL_BL;
    }
    *q++ = 0;
    *q++ = ch->volBRCur;
    if (vpb->pb.mix.vAuxBR != 0) {
        ctrl |= AX_MIXER_CTRL_BR;
    }
    *q++ = 0;
    *q++ = ch->volCLCur;
    if (vpb->pb.mix.vAuxCL != 0) {
        ctrl |= AX_MIXER_CTRL_CL;
    }
    *q++ = 0;
    *q++ = ch->volCRCur;
    if (vpb->pb.mix.vAuxCR != 0) {
        ctrl |= AX_MIXER_CTRL_CR;
    }
    *q++ = 0;
    *q++ = ch->volSCur;
    if (vpb->pb.mix.vS != 0) {
        ctrl |= AX_MIXER_CTRL_S;
    }
    *q++ = 0;
    *q++ = ch->volASCur;
    if (vpb->pb.mix.vAuxAS != 0) {
        ctrl |= AX_MIXER_CTRL_AS;
    }
    *q++ = 0;
    *q++ = ch->volBSCur;
    if (vpb->pb.mix.vAuxBS != 0) {
        ctrl |= AX_MIXER_CTRL_BS;
    }
    *q++ = 0;
    *q++ = ch->volCSCur;
    if (vpb->pb.mix.vAuxCS != 0) {
        ctrl |= AX_MIXER_CTRL_CS;
    }
    *q = 0;

    vpb->pb.mixerCtrl = ctrl;
    vpb->sync |= AX_PBSYNC_MIXER_CTRL | AX_PBSYNC_MIX | AX_PBSYNC_VE;

    memset(&vpb->pb.rmtMix, 0, sizeof(vpb->pb.rmtMix));
    vpb->pb.rmtMixerCtrl = 0;
    vpb->sync |= AX_PBSYNC_RMT_MIXER_CTRL | AX_PBSYNC_RMTMIX;

    OSRestoreInterrupts(enabled);
}

void MIXReleaseChannel(AXVPB* vpb) {
    __MIXChannel[vpb->index].vpb = NULL;
}

void MIXSetInput(AXVPB* vpb, s32 input) {
    MIXChannel* ch = &__MIXChannel[vpb->index];

    ch->input = input;
    ch->flags |= MIX_FLAG_INPUT;
}

void MIXSetPan(AXVPB* vpb, s32 pan) {
    MIXChannel* ch = &__MIXChannel[vpb->index];
    s32 p;

    if (pan < 0)
        p = 0;
    else if (pan > 0x7F)
        p = 0x7F;
    else
        p = pan;

    ch->panL = p;
    __MIXSetPan(ch);
    ch->flags |= MIX_FLAG_MIX;
}

void MIXSetFader(AXVPB* vpb, s32 fader) {
    MIXChannel* ch = &__MIXChannel[vpb->index];

    ch->fader = fader;
    ch->flags |= MIX_FLAG_MIX;
}

void MIXUpdateSettings(void)
{
    u32 i;
    u32 ctrl;

    if (!__init) {
        return;
    }

    for (i = 0; i < (u32)__MIXMaxVoices; i++) {
        s32 veChanged = 0;
        s32 mixChanged = 0;
        MIXChannel* ch = &__MIXChannel[i];
        AXVPB* vpb = ch->vpb;

        if (vpb == NULL) {
            continue;
        }

        ctrl = 0;

        if (ch->flags & MIX_FLAG_MAIN) {
            ch->volMainCur = ch->volMainTgt;
            veChanged = 1;
            ch->flags &= ~MIX_FLAG_MAIN;
        }

        if (ch->flags & MIX_FLAG_INPUT) {
            if (ch->flags & 0x8) {
                ch->volMainTgt = 0;
            } else {
                ch->volMainTgt = MIXGetVolumeInline(ch->input);
            }
            veChanged = 1;
            ch->flags = (ch->flags & ~MIX_FLAG_INPUT) | MIX_FLAG_MAIN;
        }

        if (ch->flags & MIX_FLAG_APPLY) {
            ch->volLCur = ch->volLTgt;
            ch->volRCur = ch->volRTgt;
            ch->volSCur = ch->volSTgt;
            ch->volALCur = ch->volALTgt;
            ch->volARCur = ch->volARTgt;
            ch->volASCur = ch->volASTgt;
            ch->volBLCur = ch->volBLTgt;
            ch->volBRCur = ch->volBRTgt;
            ch->volBSCur = ch->volBSTgt;
            ch->volCLCur = ch->volCLTgt;
            ch->volCRCur = ch->volCRTgt;
            ch->volCSCur = ch->volCSTgt;
            mixChanged = 1;
            ch->flags &= ~MIX_FLAG_APPLY;
        }

        if (ch->flags & MIX_FLAG_MIX) {
            switch (__MIXSoundMode) {
            case MIX_MODE_MONO:
                ch->volLTgt = MIXGetVolumeInline(ch->fader + ch->vSL);
                ch->volRTgt = MIXGetVolumeInline(ch->fader + ch->vSL);
                ch->volSTgt = MIXGetVolumeInline(ch->fader + ch->vSR - 30);

                if (ch->flags & 0x1) {
                    ch->volALTgt = MIXGetVolumeInline(ch->auxA + ch->vSL);
                    ch->volARTgt = MIXGetVolumeInline(ch->auxA + ch->vSL);
                    ch->volASTgt = MIXGetVolumeInline(ch->auxA + ch->vSR - 30);
                } else {
                    ch->volALTgt = MIXGetVolumeInline(ch->auxA + ch->fader + ch->vSL);
                    ch->volARTgt = MIXGetVolumeInline(ch->auxA + ch->fader + ch->vSL);
                    ch->volASTgt = MIXGetVolumeInline(ch->auxA + ch->fader + ch->vSR - 30);
                }

                if (ch->flags & 0x2) {
                    ch->volBLTgt = MIXGetVolumeInline(ch->auxB + ch->vSL);
                    ch->volBRTgt = MIXGetVolumeInline(ch->auxB + ch->vSL);
                    ch->volBSTgt = MIXGetVolumeInline(ch->auxB + ch->vSR - 30);
                } else {
                    ch->volBLTgt = MIXGetVolumeInline(ch->auxB + ch->fader + ch->vSL);
                    ch->volBRTgt = MIXGetVolumeInline(ch->auxB + ch->fader + ch->vSL);
                    ch->volBSTgt = MIXGetVolumeInline(ch->auxB + ch->fader + ch->vSR - 30);
                }

                if (ch->flags & 0x4) {
                    ch->volCLTgt = MIXGetVolumeInline(ch->pan + ch->vSL);
                    ch->volCRTgt = MIXGetVolumeInline(ch->pan + ch->vSL);
                    ch->volCSTgt = MIXGetVolumeInline(ch->pan + ch->vSR - 30);
                } else {
                    ch->volCLTgt = MIXGetVolumeInline(ch->pan + ch->fader + ch->vSL);
                    ch->volCRTgt = MIXGetVolumeInline(ch->pan + ch->fader + ch->vSL);
                    ch->volCSTgt = MIXGetVolumeInline(ch->pan + ch->fader + ch->vSR - 30);
                }
                break;

            case MIX_MODE_STEREO:
            case 2:
                ch->volLTgt = MIXGetVolumeInline(ch->fader + ch->vSL + ch->vL);
                ch->volRTgt = MIXGetVolumeInline(ch->fader + ch->vSL + ch->vR);
                ch->volSTgt = MIXGetVolumeInline(ch->fader + ch->vSR - 30);

                if (ch->flags & 0x1) {
                    ch->volALTgt = MIXGetVolumeInline(ch->auxA + ch->vSL + ch->vL);
                    ch->volARTgt = MIXGetVolumeInline(ch->auxA + ch->vSL + ch->vR);
                    ch->volASTgt = MIXGetVolumeInline(ch->auxA + ch->vSR - 30);
                } else {
                    ch->volALTgt = MIXGetVolumeInline(ch->vSL + ch->vL + ch->fader + ch->auxA);
                    ch->volARTgt = MIXGetVolumeInline(ch->vSL + ch->vR + ch->fader + ch->auxA);
                    ch->volASTgt = MIXGetVolumeInline(ch->fader + ch->auxA + ch->vSR - 30);
                }

                if (ch->flags & 0x2) {
                    ch->volBLTgt = MIXGetVolumeInline(ch->auxB + ch->vSL + ch->vL);
                    ch->volBRTgt = MIXGetVolumeInline(ch->auxB + ch->vSL + ch->vR);
                    ch->volBSTgt = MIXGetVolumeInline(ch->auxB + ch->vSR - 30);
                } else {
                    ch->volBLTgt = MIXGetVolumeInline(ch->vSL + ch->vL + ch->fader + ch->auxB);
                    ch->volBRTgt = MIXGetVolumeInline(ch->vSL + ch->vR + ch->fader + ch->auxB);
                    ch->volBSTgt = MIXGetVolumeInline(ch->fader + ch->auxB + ch->vSR - 30);
                }

                if (ch->flags & 0x4) {
                    ch->volCLTgt = MIXGetVolumeInline(ch->pan + ch->vSL + ch->vL);
                    ch->volCRTgt = MIXGetVolumeInline(ch->pan + ch->vSL + ch->vR);
                    ch->volCSTgt = MIXGetVolumeInline(ch->pan + ch->vSR - 30);
                } else {
                    ch->volCLTgt = MIXGetVolumeInline(ch->vSL + ch->vL + ch->fader + ch->pan);
                    ch->volCRTgt = MIXGetVolumeInline(ch->vSL + ch->vR + ch->fader + ch->pan);
                    ch->volCSTgt = MIXGetVolumeInline(ch->fader + ch->pan + ch->vSR - 30);
                }
                break;

            case MIX_MODE_DPL2:
                ch->volLTgt = MIXGetVolumeInline(ch->fader + ch->vSL + ch->vL);
                ch->volRTgt = MIXGetVolumeInline(ch->fader + ch->vSL + ch->vR);
                ch->volSTgt = MIXGetVolumeInline(ch->fader + ch->vSR + ch->vRL);
                ch->volCLTgt = MIXGetVolumeInline(ch->fader + ch->vSR + ch->vRR);

                if (ch->flags & 0x1) {
                    ch->volALTgt = MIXGetVolumeInline(ch->auxA + ch->vSL + ch->vL);
                    ch->volARTgt = MIXGetVolumeInline(ch->auxA + ch->vSL + ch->vR);
                    ch->volASTgt = MIXGetVolumeInline(ch->auxA + ch->vSR + ch->vRL);
                    ch->volCRTgt = MIXGetVolumeInline(ch->auxA + ch->vSR + ch->vRR);
                } else {
                    ch->volALTgt = MIXGetVolumeInline(ch->vSL + ch->vL + ch->fader + ch->auxA);
                    ch->volARTgt = MIXGetVolumeInline(ch->vSL + ch->vR + ch->fader + ch->auxA);
                    ch->volASTgt = MIXGetVolumeInline(ch->vSR + ch->vRL + ch->fader + ch->auxA);
                    ch->volCRTgt = MIXGetVolumeInline(ch->vSR + ch->vRR + ch->fader + ch->auxA);
                }

                if (ch->flags & 0x2) {
                    ch->volBLTgt = MIXGetVolumeInline(ch->auxB + ch->vSL + ch->vL);
                    ch->volBRTgt = MIXGetVolumeInline(ch->auxB + ch->vSL + ch->vR);
                    ch->volBSTgt = MIXGetVolumeInline(ch->auxB + ch->vSR + ch->vRL);
                    ch->volCSTgt = MIXGetVolumeInline(ch->auxB + ch->vSR + ch->vRR);
                } else {
                    ch->volBLTgt = MIXGetVolumeInline(ch->vSL + ch->vL + ch->fader + ch->auxB);
                    ch->volBRTgt = MIXGetVolumeInline(ch->vSL + ch->vR + ch->fader + ch->auxB);
                    ch->volBSTgt = MIXGetVolumeInline(ch->vSR + ch->vRL + ch->fader + ch->auxB);
                    ch->volCSTgt = MIXGetVolumeInline(ch->vSR + ch->vRR + ch->fader + ch->auxB);
                }

                ctrl |= 0x80000000;
                break;
            }

            mixChanged = 1;
            ch->flags = (ch->flags & ~MIX_FLAG_MIX) | MIX_FLAG_APPLY;
        }

        if (veChanged) {
            vpb->pb.ve.currentVolume = ch->volMainCur;
            vpb->pb.ve.currentDelta =
                (s16)((ch->volMainTgt - ch->volMainCur) / 96);
            vpb->sync |= AX_PBSYNC_VE;
        }

        if (mixChanged) {
            // Walk pb.mix as a u16 stream (retail materialises addi r3,r4,0x3e
            // and stores value/delta through base-relative offsets) — same
            // trick as MIXInitChannel; plain field stores fold to direct
            // offsets and break the match. The != 0 tests use locals because
            // q is an opaque pointer and field tests would force reloads.
            u16* q = &vpb->pb.mix.vL;
            u16 cur;
            u16 delta;

            cur = ch->volLCur;
            *q++ = cur;
            if (cur != 0) {
                ctrl |= AX_MIXER_CTRL_L;
            }
            delta = (u16)((ch->volLTgt - ch->volLCur) / 96);
            *q++ = delta;
            if (delta != 0) {
                ctrl |= AX_MIXER_CTRL_DELTA;
            }

            cur = ch->volRCur;
            *q++ = cur;
            if (cur != 0) {
                ctrl |= AX_MIXER_CTRL_R;
            }
            delta = (u16)((ch->volRTgt - ch->volRCur) / 96);
            *q++ = delta;
            if (delta != 0) {
                ctrl |= AX_MIXER_CTRL_DELTA;
            }

            cur = ch->volALCur;
            *q++ = cur;
            if (cur != 0) {
                ctrl |= AX_MIXER_CTRL_AL;
            }
            delta = (u16)((ch->volALTgt - ch->volALCur) / 96);
            *q++ = delta;
            if (delta != 0) {
                ctrl |= AX_MIXER_CTRL_DELTA_A;
            }

            cur = ch->volARCur;
            *q++ = cur;
            if (cur != 0) {
                ctrl |= AX_MIXER_CTRL_AR;
            }
            delta = (u16)((ch->volARTgt - ch->volARCur) / 96);
            *q++ = delta;
            if (delta != 0) {
                ctrl |= AX_MIXER_CTRL_DELTA_A;
            }

            cur = ch->volBLCur;
            *q++ = cur;
            if (cur != 0) {
                ctrl |= AX_MIXER_CTRL_BL;
            }
            delta = (u16)((ch->volBLTgt - ch->volBLCur) / 96);
            *q++ = delta;
            if (delta != 0) {
                ctrl |= AX_MIXER_CTRL_DELTA_B;
            }

            cur = ch->volBRCur;
            *q++ = cur;
            if (cur != 0) {
                ctrl |= AX_MIXER_CTRL_BR;
            }
            delta = (u16)((ch->volBRTgt - ch->volBRCur) / 96);
            *q++ = delta;
            if (delta != 0) {
                ctrl |= AX_MIXER_CTRL_DELTA_B;
            }

            cur = ch->volCLCur;
            *q++ = cur;
            if (cur != 0) {
                ctrl |= AX_MIXER_CTRL_CL;
            }
            delta = (u16)((ch->volCLTgt - ch->volCLCur) / 96);
            *q++ = delta;
            if (delta != 0) {
                ctrl |= AX_MIXER_CTRL_DELTA_C;
            }

            cur = ch->volCRCur;
            *q++ = cur;
            if (cur != 0) {
                ctrl |= AX_MIXER_CTRL_CR;
            }
            delta = (u16)((ch->volCRTgt - ch->volCRCur) / 96);
            *q++ = delta;
            if (delta != 0) {
                ctrl |= AX_MIXER_CTRL_DELTA_C;
            }

            cur = ch->volSCur;
            *q++ = cur;
            if (cur != 0) {
                ctrl |= AX_MIXER_CTRL_S;
            }
            delta = (u16)((ch->volSTgt - ch->volSCur) / 96);
            *q++ = delta;
            if (delta != 0) {
                ctrl |= AX_MIXER_CTRL_DELTA_S;
            }

            cur = ch->volASCur;
            *q++ = cur;
            if (cur != 0) {
                ctrl |= AX_MIXER_CTRL_AS;
            }
            delta = (u16)((ch->volASTgt - ch->volASCur) / 96);
            *q++ = delta;
            if (delta != 0) {
                ctrl |= AX_MIXER_CTRL_DELTA_AS;
            }

            cur = ch->volBSCur;
            *q++ = cur;
            if (cur != 0) {
                ctrl |= AX_MIXER_CTRL_BS;
            }
            delta = (u16)((ch->volBSTgt - ch->volBSCur) / 96);
            *q++ = delta;
            if (delta != 0) {
                ctrl |= AX_MIXER_CTRL_DELTA_BS;
            }

            cur = ch->volCSCur;
            *q++ = cur;
            if (cur != 0) {
                ctrl |= AX_MIXER_CTRL_CS;
            }
            delta = (u16)((ch->volCSTgt - ch->volCSCur) / 96);
            *q++ = delta;
            if (delta != 0) {
                ctrl |= AX_MIXER_CTRL_DELTA_CS;
            }

            vpb->pb.mixerCtrl = ctrl;
            vpb->sync |= AX_PBSYNC_MIXER_CTRL | AX_PBSYNC_MIX;
        }

        __MIXRmtUpdateSettings(i, vpb);
    }
}
