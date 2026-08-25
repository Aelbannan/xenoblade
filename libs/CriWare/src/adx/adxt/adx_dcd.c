// adx_dcd.c - ADX decode info parsing (CriWare ADX)
// Retail .text slice: 0x8038CD48..0x8038D40C

#include <harness_catalog.h>
#include <math.h>
#include <string.h>

// ADX coefficient constant pool (rodata 0x80517468, size 0x30):
//   [0] f32  2*pi
//   [1] f64  2.0
//   [2] f32  1.0
//   [3] f32  4096.0
//   [4] f32  2.0
//   [5] f64  0x4330000080000000 (u32->double magic)
extern const double lbl_eu_80517468[];

void ADX_GetCoefficient(int highpass_frequency, int sample_rate, s16* coef1_ptr, s16* coef2_ptr)
{
    float f21;
    float f20;
    float r;
    f21 = (float)cos((6.2831855f * highpass_frequency) / sample_rate);
    f21 = (float)sqrt(2.0) - f21;
    f20 = (float)sqrt(2.0) - 1.0f;
    r = (f21 - (float)sqrt((f21 + f20) * (f21 - f20))) / f20;
    *coef1_ptr = (s16)(4096.0f * (2.0f * r));
    *coef2_ptr = (s16)(4096.0f * (-r * r));
}

int ADX_ScanInfoCode(const u16* codes, int size, u16* out)
{
    int i = 0;
    int found = 0x7FFFFFFF;
    for (i = 0; i < size - 1; i += 2, codes++) {
        if (*codes == 0x8000) {
            found = 0x7FFFFFFF;
            if (i < found)
                found = i;
            break;
        }
    }
    if (found != 0x7FFFFFFF) {
        *out = (u16)found;
        return 0;
    }
    *out = 0;
    return -1;
}

int ADX_DecodeInfo(u8* info, int size, u16* outA, u8* outB, u8* outC,
                   u8* outD, u8* outE, u32* outF, u32* outG, u32* outH)
{
    if (size < 0x10)
        return -1;
    if ((u16)((info[0] << 8) | info[1]) != 0x8000)
        return -2;
    *outA = (u16)((u16)((info[2] << 8) | info[3]) + 4);
    *outB = info[4];
    *outD = info[5];
    *outC = info[6];
    *outE = info[7];
    *outF = (u32)((info[8] << 24) | (info[9] << 16) | (info[10] << 8) | info[11]);
    *outG = (u32)((info[12] << 24) | (info[13] << 16) | (info[14] << 8) | info[15]);
    if ((s8)*outC == 0) {
        *outH = 0;
    } else {
        *outH = (u32)(((s8)*outD - 2) * 8 / (s8)*outC);
    }
    return 0;
}

int ADX_DecodeInfoExADPCM2(void* info, int version, short* outScale)
{
    if (version < 0x12)
        return -1;
    if ((*(unsigned short*)info) != 0x8000)
        return -2;
    if ((*(short*)((char*)info + 2)) < 0xE)
        return -1;
    *outScale = *(short*)((char*)info + 0x10);
    return 0;
}

int ADX_DecodeInfoExVer(void *info, int size, unsigned char *outBits, unsigned char *outCh)
{
    if (size < 0x14)
        return -1;
    if (*(unsigned short *)info != 0x8000)
        return -2;
    typedef struct ADXDecodeInfoHeader {
        u16 format;
        s16 version;
    } ADXDecodeInfoHeader;
    if (((ADXDecodeInfoHeader*)info)->version < 0x10)
        return -1;
    *outBits = ((unsigned char *)info)[0x12];
    *outCh   = ((unsigned char *)info)[0x13];
    return 0;
}

int ADX_DecodeInfoExIdly(u8* info, int size, s16* outA, s16* outB)
{
    int err = 0;
    u8 ch;
    if (size < 0x14) {
        err = -1;
    } else if (*(u16*)info != 0x8000) {
        err = -2;
    } else if (*(s16*)(info + 2) < 0x10) {
        err = -1;
    } else {
        ch = info[0x12];
        err = 0;
    }
    if (err != 0)
        return -1;
    if (ch >= 4) {
        if (size < 0x20)
            return -1;
        if (*(u16*)info != 0x8000)
            return -2;
        if (*(s16*)(info + 2) < 0x1C)
            return -1;
        *outA = *(s16*)(info + 0x18);
        *outB = *(s16*)(info + 0x1A);
        *(outA + 1) = *(s16*)(info + 0x1C);
        *(outB + 1) = *(s16*)(info + 0x1E);
    } else {
        *(outB + 1) = 0;
        *(outA + 1) = 0;
        *outB = 0;
        *outA = 0;
    }
    return 0;
}

int ADX_DecodeInfoExLoop(u8* info, int size, u32* outA, s16* outB,
                         s16* outC, u32* outD, u32* outE, u32* outF, u32* outG)
{
    int err = 0;
    int minSize;
    int off;
    u8 ch;
    *outB = 0;
    if (size < 0x14) {
        err = -1;
    } else if (*(u16*)info != 0x8000) {
        err = -2;
    } else if (*(s16*)(info + 2) < 0x10) {
        err = -1;
    } else {
        ch = info[0x12];
        err = 0;
    }
    if (err != 0)
        return err;
    {
        minSize = 0x30;
        if (ch == 4)
            minSize = 0x3C;
        if (size < minSize)
            return -1;
        if (*(u16*)info != 0x8000)
            return -2;
        if (*(s16*)(info + 2) < minSize - 4)
            return -1;
        {
            off = 0x14;
            if (ch == 4)
                off = 0x20;
            *outA = *(s16*)(info + off);
            const u8* p = (u8*)((u32)off + (u32)info);
            {
                s16 loop = *(s16*)(p + 2);
                *outB = loop;
                if (loop != 1)
                    return -2;
            }
            *outC = *(s16*)(p + 6);
            *outD = *(u32*)(p + 8);
            *outE = *(u32*)(p + 0xC);
            *outF = *(u32*)(p + 0x10);
            *outG = *(u32*)(p + 0x14);
        }
    }
    return 0;
}

int ADX_DecodeInfoAinf(u8* info, int size, u32* outA, u32* outB,
                       s16* outC, s16* outD)
{
    int err;
    int minSize;
    u8 ch;
    *outA = 0;
    if (size < 0x14) {
        err = -1;
    } else if (*(u16*)info != 0x8000) {
        err = -2;
    } else if (*(s16*)(info + 2) < 0x10) {
        err = -1;
    } else {
        ch = info[0x12];
        err = 0;
    }
    if (err != 0)
        return err;
    {
        minSize = 0x3C;
        if (ch == 4)
            minSize = 0x48;
        if (size < minSize)
            return -1;
        if (*(u16*)info != 0x8000)
            return -2;
        if (*(s16*)(info + 2) < minSize - 4)
            return -1;
        {
            int off = 0x14;
            if (ch == 4)
                off = 0x20;
            const u8* p = (u8*)((u32)off + (u32)info);
            off += 4;
            if (*(s16*)(p + 2) != 0)
                off += 0x14;
            {
                const u8* p2 = info + off;
                u32 tag = ((u32)p2[0] << 24) | ((u32)p2[1] << 16) | ((u32)p2[2] << 8) | p2[3];
                if (tag != 0x41494E46)
                    return -2;
                *outA = *(u32*)(p2 + 4);
                memcpy(outB, p2 + 8, 0x10);
                const u8* tail = (u8*)((u32)off + (u32)info);
                *outC = *(s16*)(tail + 0x18);
                *outD = *(s16*)(tail + 0x1C);
                *(outD + 1) = *(s16*)(tail + 0x1E);
            }
        }
    }
    return 0;
}

int ADX_DecodeFooter(void* info, int size, u16* out) {
    if (size < 0x10) return -1;
    if (*(u16*)info != 0x8001) return -2;
    *out = (u16)(*(s16*)((u8*)info + 2) + 4);
    return 0;
}
