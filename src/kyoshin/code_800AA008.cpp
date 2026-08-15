#include "kyoshin/code_800AA008.hpp"

#include <cstring>
#include <cstdio>

#include "monolib/util/CPathUtil.hpp"

extern "C" {
extern const char lbl_eu_804FC044[];
extern const u8 lbl_eu_805283B0[];
extern const u32 jumptable_eu_805285A0[];
extern const u32 jumptable_eu_805285C4[];
extern char lbl_eu_80572C80[0x44];
extern char lbl_eu_80661A40;
}

struct FormatEntry {
    u16 id;
    u8 formatType;
    u8 _pad;
    const char* name;
    const char* template_;
    const char* suffix;
};

static inline char* fixstr_buf(ml::FixStr<64>& buf) {
    return const_cast<char*>(buf.c_str());
}

void func_800AA008(ml::FixStr<64>& buf, int type, u32 arg1, u32 arg2, u32 arg3) {
    char tmp[0x100];
    memset(tmp, 0, sizeof(tmp));

    switch (type) {
    case 0:
        sprintf(tmp, &lbl_eu_804FC044[0]);
        break;
    case 1:
        sprintf(tmp, &lbl_eu_804FC044[0x0D]);
        break;
    case 2:
        sprintf(tmp, &lbl_eu_804FC044[0x12]);
        break;
    case 3:
    case 4:
        sprintf(tmp, &lbl_eu_804FC044[0x17]);
        break;
    case 5:
        sprintf(tmp, &lbl_eu_804FC044[0x17], arg2, arg3);
        break;
    case 6:
        sprintf(tmp, &lbl_eu_804FC044[0x20]);
        break;
    case 7:
        sprintf(tmp, &lbl_eu_804FC044[0x2D]);
        break;
    default:
        if ((type) == 8) {
            sprintf(tmp, &lbl_eu_804FC044[0x36]);
        } else {
            if (arg3 != 0) {
                sprintf(tmp, &lbl_eu_804FC044[0]);
            } else {
                sprintf(tmp, &lbl_eu_804FC044[0x36]);
            }
        }
        break;
    }

    buf += tmp;
}

int func_800AA1B4(const char* str, int digitCount, int* out) {
    *out = 0;
    if (str == nullptr) return 0;

    switch (digitCount) {
    case 1:
        *out = str[0] - '0';
        break;
    case 2: {
        int val = (str[0] - '0') * 10;
        val += str[1] - '0';
        *out = val;
        break;
    }
    case 3: {
        int val = (str[0] - '0') * 100;
        val += (str[1] - '0') * 10;
        val += str[2] - '0';
        *out = val;
        break;
    }
    case 4: {
        int val = (str[0] - '0') * 1000;
        val += (str[1] - '0') * 100;
        val += (str[2] - '0') * 10;
        val += str[3] - '0';
        *out = val;
        break;
    }
    }
    return 1;
}

u32 func_800AA2BC(u32 a, u32 b) {
    return (b << 10) | ((a << 20) | 0x08000000);
}

u32 func_800AA2D0(u32 a, u32 b, u32 c) {
    u32 t = ((a & 0xFFF) << 20) | 0xE0000000;
    u32 u = c | (b << 10);
    return u | t;
}

u32 func_800AA2E8(u32 a, u32 b, u32 c) {
    u32 t = ((a & 0xFFF) << 20) | 0x10000000;
    u32 u = c | (b << 10);
    return u | t;
}

u32 func_800AA300(u32 a, u32 b, u32 c) {
    u32 t = ((a & 0xFFF) << 20) | 0x20000000;
    u32 u = c | (b << 10);
    return u | t;
}

// Unpack a packed token into entry ID and up to three parameters.
// Bits: [31:27]=entryId, [26:20]=param1(7bit), [19:10]=param2(10bit), [9:0]=param3(10bit)
void func_800AA318(u32 packedToken, u32* outEntryId, u32* outParam1, u32* outParam2, u32* outParam3) {
    *outEntryId = packedToken >> 27;
    *outParam1 = (packedToken >> 20) & 0x7F;
    *outParam2 = (packedToken >> 10) & 0x3FF;
    *outParam3 = packedToken & 0x3FF;
}

int func_800AA33C(ml::FixStr<64>& buf, u32 packed, int prefixFlag, int suffixFlag) {
    buf.clear();

    if (packed == 0) return 0;

    u32 id = packed >> 27;
    u32 field1 = (packed >> 20) & 0x7F;
    u32 field2 = (packed >> 10) & 0x3FF;
    u32 field3 = packed & 0x3FF;

    const FormatEntry* entry = reinterpret_cast<const FormatEntry*>(lbl_eu_805283B0) + id;
    u32 idx = id;

    int result = 0;
    while (idx < 0x1F) {
        if (entry->id == id) {
            if (prefixFlag != 0) {
                buf = entry->template_;
            }

            buf += entry->name;

            func_800AA008(buf, entry->formatType, field1, field2, field3);

            if (suffixFlag != 0) {
                if (id == 1 || id == 0x1D) {
                    buf += entry->suffix;
                } else if (id - 2 <= 4) {
                    buf += entry->suffix;
                } else if (id - 7 <= 4) {
                    buf += entry->suffix;
                } else if (id == 0x1C) {
                    buf += entry->suffix;
                } else if (id - 0xC <= 5) {
                    buf += entry->suffix;
                } else if (id == 0x1E) {
                    buf += entry->suffix;
                } else if (id - 0x12 <= 4) {
                    buf += entry->suffix;
                }
            }

            result = 1;
        }
        entry++;
        idx++;
    }

    return result;
}

// Forward the arg into the FixStr formatter, then return the buffer address
// (retail re-materializes r3 = &lbl_eu_80572C80 after the call; dispatch-table
// slot, so the result is consumed by a caller).
extern "C" ml::FixStr<64>* func_800AA5C0(void* arg) {
    func_800AA33C(*(ml::FixStr<64>*)lbl_eu_80572C80, (u32)arg, 0, 1);
    return (ml::FixStr<64>*)lbl_eu_80572C80;
}

u32 func_800AA600(const char* str) {
    if (str == nullptr) return 0;

    const u32* table = reinterpret_cast<const u32*>(lbl_eu_805283B0);
    const FormatEntry* entry = reinterpret_cast<const FormatEntry*>(lbl_eu_805283B0) + 1;
    for (int i = 0; i < 10; i++) {
        if (str[0] == entry[0].name[0] && str[1] == entry[0].name[1])
            return table[(entry[0].name[1] & 0xF0) / 4];
        if (str[0] == entry[1].name[0] && str[1] == entry[1].name[1])
            return table[(entry[1].name[1] & 0xF0) / 4];
        if (str[0] == entry[2].name[0] && str[1] == entry[2].name[1])
            return table[(entry[2].name[1] & 0xF0) / 4];
        entry += 3;
    }
    return 0;
}

u32 func_800AA714(const char* path) {
    if (path == nullptr) return 0;

    const char* filePtr = ml::CPathUtil::getFilePtrFromPath(path);
    ml::FixStr<64> nameBuf;
    nameBuf = filePtr;

    int extLen = nameBuf.size();
    int extOff;
    if (extLen == 0) {
        extOff = -1;
    } else {
        int dotLen = strlen(&lbl_eu_80661A40);
        char* p = const_cast<char*>(nameBuf.c_str()) + 0x5F + extLen;
        char* pEnd = const_cast<char*>(nameBuf.c_str()) + 0x5F;
        while (p != pEnd) {
            if (strncmp(p, &lbl_eu_80661A40, dotLen) == 0) {
                extOff = (int)(p - nameBuf.mString);
                goto found_ext;
            }
            p--;
        }
        extOff = -1;
    }

found_ext:
    if (extOff + 1 > 1) {
        if (extLen != 0) {
            ml::FixStr<64> stripped;
            if (extOff != -1) {
                strncpy(stripped.mString, nameBuf.mString, extOff);
                stripped.mString[extOff] = '\0';
                stripped.mLength = strlen(stripped.mString);
            }
            nameBuf = stripped;
        }
    }

    u32 result = 0;
    u8 firstChar = (u8)nameBuf[0];
    u8 secondChar = (u8)nameBuf[1];
    const FormatEntry* entry = reinterpret_cast<const FormatEntry*>(lbl_eu_805283B0) + 1;
    for (int i = 0; i < 0x1E; i++) {
        if ((u8)entry->name[0] == firstChar && (u8)entry->name[1] == secondChar) {
            u32 entryId = entry->id;
            int remaining = nameBuf.mLength - 2;
            const char* nameTail = &nameBuf.mString[2];
            u8 fmtType = entry->formatType;

            switch (fmtType) {
            case 0:
                if (remaining >= 3) {
                    int val;
                    func_800AA1B4(nameTail, 3, &val);
                    result = (entryId << 27) | (val << 20);
                }
                break;
            case 1:
                if (remaining >= 2) {
                    int val;
                    func_800AA1B4(nameTail, 2, &val);
                    result = (entryId << 27) | (val << 20);
                }
                break;
            case 2:
                if (remaining >= 6) {
                    int val1, val2;
                    func_800AA1B4(nameTail, 3, &val1);
                    func_800AA1B4(nameTail + 3, 3, &val2);
                    result = (entryId << 27) | (val1 << 20) | (val2 << 10);
                }
                break;
            case 3:
                if (remaining >= 6) {
                    int val1, val2;
                    func_800AA1B4(nameTail, 3, &val1);
                    func_800AA1B4(nameTail + 3, 3, &val2);
                    result = (entryId << 27) | (val1 << 10) | val2;
                }
                break;
            case 4:
                if (remaining >= 8) {
                    int val1, val2, val3;
                    func_800AA1B4(nameTail, 2, &val1);
                    func_800AA1B4(nameTail + 2, 3, &val2);
                    func_800AA1B4(nameTail + 5, 3, &val3);
                    result = (entryId << 27) | (val1 << 20) | (val2 << 10) | val3;
                }
                break;
            case 5:
                if (remaining >= 6) {
                    int val1, val2, val3;
                    func_800AA1B4(nameTail, 2, &val1);
                    func_800AA1B4(nameTail + 2, 2, &val2);
                    func_800AA1B4(nameTail + 4, 2, &val3);
                    result = (entryId << 27) | (val1 << 20) | (val2 << 10) | val3;
                }
                break;
            case 6:
                if (remaining >= 5) {
                    int val1, val2;
                    func_800AA1B4(nameTail, 2, &val1);
                    func_800AA1B4(nameTail + 2, 3, &val2);
                    result = (entryId << 27) | (val1 << 20) | (val2 << 10);
                }
                break;
            case 7:
                if (remaining >= 4) {
                    int val1, val2;
                    func_800AA1B4(nameTail, 2, &val1);
                    func_800AA1B4(nameTail + 2, 2, &val2);
                    result = (entryId << 27) | (val1 << 20) | (val2 << 10);
                }
                break;
            case 8:
                if (remaining >= 6) {
                    int val1, val2, val3;
                    func_800AA1B4(nameTail, 2, &val1);
                    func_800AA1B4(nameTail + 2, 2, &val2);
                    func_800AA1B4(nameTail + 4, 2, &val3);
                    result = (entryId << 27) | (val1 << 20) | (val2 << 10) | val3;
                }
                break;
            }
            break;
        }
        entry++;
    }

    return result;
}

void sinit_800AABBC() {
    reinterpret_cast<ml::FixStr<64>*>(lbl_eu_80572C80)->clear();
}
