#include <revolution/cx/CXStreamingUncompression.h>

static __inline u32 CXiConvertEndian_(u32 val) {
    return ((val & 0xFF000000) >> 24) | ((val & 0xFF0000) >> 8) | ((val & 0xFF00) << 8) | ((val & 0xFF) << 24);
}

// Bit reader state: data[0] = source pointer, data[1] = remaining bytes,
// data[2] = bit buffer, data[3] = bit count.
#pragma inline_max_size(10000)
#pragma inline_max_total_size(10000)
static __inline s32 BitReader_Read(u32* data, u32 len) {
    s32 iVar1;

    for (; data[3] < (len & 0xff); data[3] += 8) {
        if (data[1] == 0) {
            return -1;
        }
        data[2] <<= 8;
        data[2] += *(u8*)data[0];
        data[0]++;
        data[1]--;
    }

    iVar1 = ((1 << (len & 0xff)) - 1) & (data[2] >> (data[3] - (len & 0xff)));
    data[3] -= (len & 0xff);
    return iVar1;
}

static __inline s64 BitReader_ReadEx(u32* data, s32 len) {
    s64 r29;
    u32 r28 = 0;

    for (; data[3] < (len & 0xFF); data[3] += 8) {
        if (data[1] == 0) {
            return -1;
        }
        if (data[3] > 24) {
            r28 = (u8)(data[2] >> 24);
        }
        data[2] <<= 8;
        data[2] += *(u8*)data[0];
        data[0]++;
        data[1]--;
    }

    r29 = data[2];
    r29 |= (s64)(r28 & 0xFF) << 32;
    r29 = ((r29 >> (data[3] - (len & 0xFF))) & ((1 << (len & 0xFF)) - 1));
    data[3] -= (len & 0xFF);
    return r29;
}

void CXInitUncompContextLH(CXUncompContextLH* context, void* dest) {
    context->destp = (u8*)dest;
    context->destCount = -1;
    context->nodep = context->huffTable9 + 1;
    context->tableSize9 = -1;
    context->tableSize12 = -1;
    context->headerSize = 8;
    context->length = 0;
    context->stream = 0;
    context->stream_len = 0;
    context->offset_bits = -1;
    context->forceDestCount = 0;
}

s32 CXReadUncompLH(CXUncompContextLH* context, const void* data, u32 len) {
    u32 br[4];
    u16* nodep;
    u16 length;
    s64 value64;
    s32 v;
    u32 idx;
    u16 node;
    s32 bit;
    u32 offsetPlus1;
    u32 t;

    br[0] = (u32)data;
    br[1] = len;
    br[2] = context->stream;
    br[3] = context->stream_len;

    while (context->headerSize != 0) {
        value64 = BitReader_ReadEx(br, 32);
        if (value64 < 0) {
            goto exit;
        }

        context->headerSize -= 4;
        if (context->headerSize == 4) {
            u32 w = CXiConvertEndian_((u32)value64);
            if ((w & 0xF0) != 0x40) {
                return -1;
            }
            context->destCount = w >> 8;
            if (context->destCount == 0) {
                context->headerSize = 4;
                context->destCount = -1;
            } else {
                context->headerSize = 0;
            }
        } else {
            context->destCount = CXiConvertEndian_((u32)value64);
        }

        if (context->headerSize == 0 && context->forceDestCount > 0 && context->forceDestCount < context->destCount) {
            context->destCount = context->forceDestCount;
        }
    }

    if (context->tableSize9 < 0) {
        v = BitReader_Read(br, 16);
        if (v < 0) {
            goto exit;
        }
        context->tableIdx = 1;
        context->tableSize9 = ((((((v & 0xFF00) >> 8) | ((v & 0xFF) << 8)) + 1) << 5) - 16);
    }

    while (context->tableSize9 >= 9) {
        v = BitReader_Read(br, 9);
        if (v < 0) {
            goto exit;
        }
        t = context->tableIdx;
        context->huffTable9[t] = (u16)v;
        context->tableIdx = t + 1;
        context->tableSize9 -= 9;
    }

    while (context->tableSize9 > 0) {
        v = BitReader_Read(br, (u8)context->tableSize9);
        if (v < 0) {
            goto exit;
        }
        context->tableSize9 = 0;
    }

    if (!CXiLHVerifyTable(context->huffTable9, 9)) {
        return -5;
    }

    if (context->tableSize12 < 0) {
        v = BitReader_Read(br, 8);
        if (v < 0) {
            goto exit;
        }
        context->tableIdx = 1;
        context->tableSize12 = ((((u16)v + 1) << 5) - 8);
    }

    while (context->tableSize12 >= 5) {
        v = BitReader_Read(br, 5);
        if (v < 0) {
            goto exit;
        }
        t = context->tableIdx;
        context->huffTable12[t] = (u16)v;
        context->tableIdx = t + 1;
        context->tableSize12 -= 5;
    }

    while (context->tableSize12 > 0) {
        v = BitReader_Read(br, (u8)context->tableSize12);
        if (v < 0) {
            goto exit;
        }
        context->tableSize12 = 0;
    }

    if (!CXiLHVerifyTable(context->huffTable12, 5)) {
        return -5;
    }

    nodep = context->nodep;
    length = context->length;

    while (context->destCount > 0) {
        if (length == 0) {
            do {
                bit = BitReader_Read(br, 1);
                if (bit < 0) {
                    context->nodep = nodep;
                    context->length = length;
                    goto exit;
                }
                bit &= 1;
                node = *nodep;
                idx = ((node & 0x7F) + 1) * 2 + bit;
                if (node & (0x100 >> bit)) {
                    length = ((u16*)((u32)nodep & ~3))[idx];
                    nodep = &context->huffTable12[1];
                    break;
                }
                nodep = ((u16*)((u32)nodep & ~3)) + idx;
            } while (1);
        }

        if (length < 0x100) {
            *context->destp++ = (u8)length;
            context->destCount--;
            nodep = &context->huffTable9[1];
            length = 0;
        } else {
            u16 copyCount = ((u32)length & 0xFF) + 3;
            u32 offset;

            if ((s8)context->offset_bits < 0) {
                do {
                    bit = BitReader_Read(br, 1);
                    if (bit < 0) {
                        context->nodep = nodep;
                        context->length = length;
                        goto exit;
                    }
                    bit &= 1;
                    node = *nodep;
                    idx = ((node & 0x7) + 1) * 2 + bit;
                    if (node & (0x10 >> bit)) {
                        context->offset_bits = (u8)((u16*)((u32)nodep & ~3))[idx];
                        break;
                    }
                    nodep = ((u16*)((u32)nodep & ~3)) + idx;
                } while (1);
            }

            switch ((u8)context->offset_bits) {
            case 0:
            case 1:
                break;
            default:
                offset = BitReader_Read(br, (u8)((s8)context->offset_bits - 1));
                if ((s32)offset < 0) {
                    context->nodep = nodep;
                    context->length = length;
                    goto exit;
                }
                break;
            }

            if ((s8)context->offset_bits >= 2) {
                offset |= (u32)1 << ((s8)context->offset_bits - 1);
            }

            offsetPlus1 = (offset + 1) & 0xFFFF;
            context->offset_bits = -1;

            if (context->destCount < copyCount) {
                if (context->forceDestCount == 0) {
                    return -4;
                }
                copyCount = (u16)context->destCount;
            }

            context->destCount -= copyCount;
            while (copyCount-- != 0) {
                *context->destp = *(context->destp - offsetPlus1);
                context->destp++;
            }
            length = 0;
            nodep = &context->huffTable9[1];
        }
    }

exit:
    context->stream = br[2];
    context->stream_len = br[3];
    if (context->destCount == 0 && context->forceDestCount == 0 && len > 32) {
        return -3;
    }
    return context->destCount;
}
