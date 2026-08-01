#include "monolib/core/CToken.hpp"
#include <cstring>

// String table for func_8043AA68 (vertex attribute name lookup)
// Each entry corresponds to an index written to the output buffer when matched.
static const char* const kAttributeNames[] = {
    "VtxPosition",
    "VtxNormal",
    "VtxColor0",
    "VtxColor1",
    "VtxTexCoord0",
    "VtxTexCoord1",
    "VtxTexCoord2",
    "VtxTexCoord3",
    "VtxTexCoord4",
    "VtxTexCoord5",
    "VtxTexCoord6",
    "VtxTexCoord7",
    "VtxBoneWeight",
    "VtxBoneIndex",
    nullptr
};

// Retail vtable data symbol (vtable lives in monolibdata1.s at 0x8056B52C).
// The class is __declspec(novtable), so the ctor assigns the retail label
// explicitly instead of the compiler-generated __vt__6CToken.
extern "C" u8 lbl_eu_8056B52C[];

CToken::CToken() {
    *(void**)this = (void*)lbl_eu_8056B52C;
    m_fieldCount = 0;
    for (int i = 0; i < 16; i++) {
        m_data[i] = 0;
    }
}

CToken::~CToken() {
}

u32 CToken::func_8043A8D8() {
    // Parses a hex string (with "0x" prefix) into a u32.
    // Reads bytes directly from the this pointer.
    char* r3 = reinterpret_cast<char*>(this);
    u32 r6 = 0;
    int r7 = 2;
    u32 r8 = 1;
    char* r4;
    u32 r0;

    if (r3[0] != '0') goto end;
    if (r3[1] != 'x') goto end;

    r4 = r3 + 2;
    do {
        char r5 = *r4;
        r0 = static_cast<u8>(r5 - '0');
        if (r0 <= 9) {
            r7++;
            r4++;
            continue;
        }
        r0 = static_cast<u8>(r5 - 'a');
        if (r0 <= 5) {
            r7++;
            r4++;
            continue;
        }
        break;
    } while (1);

    {
        int r4i = r7 - 1;
        r0 = r4i - 1;
        r3 = r3 + r4i;
        if (r4i < 2) goto end;
        while (r0-- > 0) {
            char r4c = *r3;
            u32 digit;
            if (static_cast<u8>(r4c - '0') <= 9) {
                digit = r4c - '0';
            } else {
                digit = static_cast<s8>(r4c) - 0x57;
            }
            r6 += digit * r8;
            r8 *= 16;
            r3--;
        }
    }

end:
    return r6;
}

void CToken::func_8043AA1C() {
    // Resets the token data (same as constructor initialization)
    m_fieldCount = 0;
    for (int i = 0; i < 16; i++) {
        m_data[i] = 0;
    }
}

void CToken::func_8043AA68(char* pOutBuffer, int bufferLen, const char* pName) {
    // Looks up a name in the attribute string table.
    // If found, writes the matching index byte to pOutBuffer.
    // If not found, copies the original name and null-terminates at the name's length.
    int nameLen = strlen(pName);

    int index = 0;
    for (int i = 0; kAttributeNames[i] != nullptr; i++) {
        if (strlen(kAttributeNames[i]) == static_cast<u32>(nameLen) &&
            strncmp(kAttributeNames[i], pName, nameLen) == 0) {
            pOutBuffer[0] = static_cast<char>(index);
            return;
        }
        index++;
    }

    // No match found - copy original name into output buffer
    strncpy(pOutBuffer, pName, bufferLen);
    pOutBuffer[nameLen] = '\0';
}

char* CToken::func_8043AB14(char* pInput, int tokenIndex) {
    // Stub - not yet matched
    return pInput;
}

bool CToken::func_8043ABD4(const char* pInput, const char* mode) {
    // Stub - not yet matched
    return false;
}
