// Auto-scaffolded catalog TU for CriWare/src/adx/adxt/adx_dcd
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>

void ADX_GetCoefficient() {}

void ADX_ScanInfoCode() {}

void ADX_DecodeInfo() {}

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
    if (((short *)info)[1] < 0x10)
        return -1;
    *outBits = ((unsigned char *)info)[0x12];
    *outCh   = ((unsigned char *)info)[0x13];
    return 0;
}

void ADX_DecodeInfoExIdly() {}

void ADX_DecodeInfoExLoop() {}

void ADX_DecodeInfoAinf() {}

void ADX_DecodeFooter() {}
