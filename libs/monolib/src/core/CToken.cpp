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

CToken::CToken() {
    m_fieldCount = 0;
    for (int i = 0; i < 16; i++) {
        m_data[i] = 0;
    }
}

CToken::~CToken() {
}

u32 CToken::func_8043A8D8() {
    // Parses a hex string starting with "0x" prefix into a u32.
    // Returns 0 if the string doesn't start with "0x" or has no valid hex digits.
    const u8* str = reinterpret_cast<const u8*>(this);

    // Check for "0x" prefix
    if (str[0] != '0' || str[1] != 'x') {
        return 0;
    }

    // Count consecutive hex digits after the "0x" prefix
    int count = 2;
    while (true) {
        u8 c = str[count];
        u8 digit = c - '0';
        if (digit > 9) {
            digit = c - 'a';
            if (digit > 5) {
                break;
            }
        }
        count++;
    }

    // Need at least one hex digit after "0x"
    if (count < 3) {
        return 0;
    }

    // Parse hex digits from right to left, accumulating the value
    u32 result = 0;
    u32 multiplier = 1;
    const u8* cur = str + count - 1;
    int remaining = count - 2;
    while (remaining-- > 0) {
        u8 c = *cur;
        u8 digit = c - '0';
        if (digit > 9) {
            digit = c - 0x57;
        }
        result += digit * multiplier;
        multiplier *= 16;
        cur--;
    }

    return result;
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

    // No match found — copy original name into output buffer
    strncpy(pOutBuffer, pName, bufferLen);
    pOutBuffer[nameLen] = '\0';
}

void CToken::func_8043AB14() {
    // Stub — not yet matched
}

void CToken::func_8043ABD4() {
    // Stub — not yet matched
}
