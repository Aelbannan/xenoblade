#include <revolution/cx/CXUncompression.h>

// Byte-swap a 32-bit word (the CX streams store integers big-endian).
static inline u32 CXSwap32(u32 val) {
    return (val >> 24 & 0xFF) | (val >> 8 & 0xFF00) | (val << 8 & 0xFF0000) |
           (val << 24 & 0xFF000000);
}

// Load a big-endian 32-bit word and return its numeric value.
static inline u32 CXLoadBE32(const void* p) {
    return CXSwap32(*(const u32*)p);
}

u32 CXGetUncompressedSize(const u8* src) {
    u32 size = CXLoadBE32(src) >> 8;
    if (size == 0) {
        size = CXLoadBE32(src + 4);
    }
    return size;
}

void CXUncompressHuffman(const u8* src, u8* dst) {
    s32 size;
    u32 shift;
    u32 bitBuffer;
    u32 byteCount;
    u32 countMax;
    const u8* data;
    const u8* treeBase;
    const u8* tree;
    const u8* words;
    u32 bits;
    s32 bitsLeft;

    size = CXLoadBE32(src) >> 8;
    if (size != 0) {
        data = src + 4;
    } else {
        data = src + 8;
    }

    bitBuffer = 0;
    treeBase = data + 1;
    countMax = 4 + (src[0] & 7);
    byteCount = 0;
    shift = src[0] & 0xF;
    if (size == 0) {
        size = CXLoadBE32(src + 4);
    }

    tree = treeBase;
    words = data + 2 * (data[0] + 1);

    while (size > 0) {
        bits = CXLoadBE32(words);
        words += 4;
        bitsLeft = 32;
        while (--bitsLeft >= 0) {
            u8 node = *tree;
            u32 bit = bits >> 31;
            tree = (const u8*)(((u32)tree & ~1) + bit + 2 * ((node & 0x3F) + 1));
            if ((node << bit) & 0x80) {
                u8 out = *tree;
                bitBuffer = (bitBuffer >> shift) | (out << (32 - shift));
                tree = (const u8*)treeBase;
                if (++byteCount == countMax) {
                    *(u32*)dst = CXSwap32(bitBuffer);
                    dst += 4;
                    byteCount = 0;
                    size -= 4;
                }
            }
            if (size <= 0)
                break;
            bits <<= 1;
        }
    }
}
