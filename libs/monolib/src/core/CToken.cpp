#include "monolib/core/CToken.hpp"
#include <cstring>

// ============================================================================
// Blob monolibdata1.s dissolve - CToken-owned data ranges (retail bytes below
// are the DOL ground truth; reloc names match the retail blob monolibdata1.o).
// .data 0x8056B4D0-0x8056B538: tokenizer string table, matchKeyWords switch
//   jumptable (function+offset relocs) and the CToken vtable.
// .rodata 0x80522558-0x80522588: token-size table used by findTokenName.
// .sdata 0x80663560-0x80663568: CToken RTTI base list {&"CToken", 0}.
// ============================================================================

// Pooled token strings owned by FloatUtils (.sdata2 range 0x8066A178+).
extern "C" const char lbl_eu_8066A288[5];  // "????"
extern "C" const char lbl_eu_8066A290[3];  // "%f"
extern "C" const char lbl_eu_8066A294[3];  // "%d"
extern "C" const char lbl_eu_8066A298[3];  // "%x"
extern "C" const char lbl_eu_8066A29C[4];  // "%v3"
extern "C" const char lbl_eu_8066A2A0[4];  // "%va"
extern "C" const char lbl_eu_8066A2A4[4];  // "%v4"
extern "C" const char lbl_eu_8066A2A8[4];  // "%i3"
extern "C" const char lbl_eu_8066A2AC[4];  // "%i4"
extern "C" const char lbl_eu_8066A2B0[6];  // "%rect"
extern "C" const char lbl_eu_8066A2B8[3];  // "%s"
extern "C" const char lbl_eu_8066A2C8[7];  // "CToken"
// .sdata 0x80663560: CToken RTTI base list (defined below).
extern "C" u32 lbl_eu_80663560[2];

// CToken dtor / tokenizer entry (retail C-linkage names; coexist with the
// class member declarations below).
extern "C" void __dt__6CTokenFv();
extern "C" int matchKeyWords__6CTokenFPCcPCc(const char*, const char*);

// .data 0x8056B4D0: tokenizer format-string table (11 pointers + null).
extern "C" u32 lbl_eu_8056B4D0[12] = {
    (u32)&lbl_eu_8066A288[0], (u32)&lbl_eu_8066A290[0], (u32)&lbl_eu_8066A294[0],
    (u32)&lbl_eu_8066A298[0], (u32)&lbl_eu_8066A29C[0], (u32)&lbl_eu_8066A2A0[0],
    (u32)&lbl_eu_8066A2A4[0], (u32)&lbl_eu_8066A2A8[0], (u32)&lbl_eu_8066A2AC[0],
    (u32)&lbl_eu_8066A2B0[0], (u32)&lbl_eu_8066A2B8[0], 0,
};

// .data 0x8056B500: matchKeyWords switch jumptable (function + case offset).
extern "C" u32 jumptable_eu_8056B500[11] = {
    (u32)((char*)&matchKeyWords__6CTokenFPCcPCc + 2400),
    (u32)((char*)&matchKeyWords__6CTokenFPCcPCc + 356),
    (u32)((char*)&matchKeyWords__6CTokenFPCcPCc + 480),
    (u32)((char*)&matchKeyWords__6CTokenFPCcPCc + 600),
    (u32)((char*)&matchKeyWords__6CTokenFPCcPCc + 720),
    (u32)((char*)&matchKeyWords__6CTokenFPCcPCc + 2400),
    (u32)((char*)&matchKeyWords__6CTokenFPCcPCc + 972),
    (u32)((char*)&matchKeyWords__6CTokenFPCcPCc + 1288),
    (u32)((char*)&matchKeyWords__6CTokenFPCcPCc + 1576),
    (u32)((char*)&matchKeyWords__6CTokenFPCcPCc + 1940),
    (u32)((char*)&matchKeyWords__6CTokenFPCcPCc + 2240),
};

// .data 0x8056B52C: CToken vtable (retail label; ctor stores it at [0]).
extern "C" u32 lbl_eu_8056B52C[3] = {
    (u32)&lbl_eu_80663560[0], 0, (u32)&__dt__6CTokenFv,
};

// .rodata 0x80522558: token-size table for findTokenName's lookup.
extern "C" const u32 lbl_eu_80522558[12] = {
    4, 2, 2, 2, 3, 3, 3, 3, 3, 5, 2, 0,
};

// .sdata 0x80663560: CToken RTTI base list.
extern "C" u32 lbl_eu_80663560[2] = { (u32)&lbl_eu_8066A2C8[0], 0 };

// NOTE: retail findTokenName reads the CToken format-string table
// (lbl_eu_8056B4D0, .data) with the size table lbl_eu_80522558 (.rodata,
// 12th entry 0 = terminator) -- there is no TU-local Vtx* attribute-name
// table (the old kAttributeNames array was decomp-invented and polluted
// .rodata; removed so CToken.o matches its retail .rodata 0x30 range).

CToken::CToken() {
    *(void**)this = (void*)lbl_eu_8056B52C;
    m_fieldCount = 0;
    for (int i = 0; i < 16; i++) {
        m_data[i] = 0;
    }
}

CToken::~CToken() {
}

u32 CToken::parseHexToken() {
    // Parses a hex string (with "0x" prefix) into a u32.
    // Reads bytes directly from the this pointer.
    char* cursor = reinterpret_cast<char*>(this);
    u32 value = 0;
    int digits = 2;
    u32 place = 1;
    char* p;
    u32 r0;

    if (cursor[0] != '0') goto end;
    if (cursor[1] != 'x') goto end;

    p = cursor + 2;
    do {
        char ch = *p;
        r0 = static_cast<u8>(ch - '0');
        if (r0 <= 9) {
            digits++;
            p++;
            continue;
        }
        r0 = static_cast<u8>(ch - 'a');
        if (r0 <= 5) {
            digits++;
            p++;
            continue;
        }
        break;
    } while (1);

    {
        int hexLen = digits - 1;
        r0 = hexLen - 1;
        cursor = cursor + hexLen;
        if (hexLen < 2) goto end;
        while (r0-- > 0) {
            char hexDigit = *cursor;
            u32 digit;
            if (static_cast<u8>(hexDigit - '0') <= 9) {
                digit = hexDigit - '0';
            } else {
                digit = static_cast<s8>(hexDigit) - 0x57;
            }
            value += digit * place;
            place *= 16;
            cursor--;
        }
    }

end:
    return value;
}

void CToken::clear() {
    // Resets the token data (same as constructor initialization)
    m_fieldCount = 0;
    for (int i = 0; i < 16; i++) {
        m_data[i] = 0;
    }
}

void CToken::findTokenName(char* pOutBuffer, int bufferLen, const char* pName) {
    // Looks up a name in the format-string table (retail layout: pointer
    // table lbl_eu_8056B4D0 + size table lbl_eu_80522558, whose 12th entry
    // (0) terminates the walk). If found, writes the matching index byte to
    // pOutBuffer. If not found, copies the original name and null-terminates
    // at the name's length.
    int nameLen = strlen(pName);

    for (int i = 0; lbl_eu_80522558[i] != 0; i++) {
        if (strlen((const char*)lbl_eu_8056B4D0[i]) == static_cast<u32>(nameLen) &&
            strncmp((const char*)lbl_eu_8056B4D0[i], pName, nameLen) == 0) {
            pOutBuffer[0] = static_cast<char>(i);
            return;
        }
    }

    // No match found - copy original name into output buffer
    strncpy(pOutBuffer, pName, bufferLen);
    pOutBuffer[nameLen] = '\0';
}

char* CToken::extractWordAt(char* pInput, int tokenIndex) {
    // Stub - not yet matched
    return pInput;
}

bool CToken::matchKeyWords(const char* pInput, const char* mode) {
    // Stub - not yet matched
    return false;
}
