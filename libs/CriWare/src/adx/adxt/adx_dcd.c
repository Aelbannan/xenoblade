// Auto-scaffolded catalog TU for CriWare/src/adx/adxt/adx_dcd
// Mangled extern stubs for llm-harness / coop selection.
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>

// LLM-HARNESS-BEGIN: us-8038cd48
void ADX_GetCoefficient() {}
// LLM-HARNESS-END: us-8038cd48

// LLM-HARNESS-BEGIN: us-8038ce6c
void ADX_ScanInfoCode() {}
// LLM-HARNESS-END: us-8038ce6c

// LLM-HARNESS-BEGIN: us-8038cee4
void ADX_DecodeInfo() {}
// LLM-HARNESS-END: us-8038cee4

// LLM-HARNESS-BEGIN: us-8038cfcc
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
// LLM-HARNESS-END: us-8038cfcc

// LLM-HARNESS-BEGIN: us-8038d014
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
// LLM-HARNESS-END: us-8038d014

// LLM-HARNESS-BEGIN: us-8038d064
void ADX_DecodeInfoExIdly() {}
// LLM-HARNESS-END: us-8038d064

// LLM-HARNESS-BEGIN: us-8038d134
void ADX_DecodeInfoExLoop() {}
// LLM-HARNESS-END: us-8038d134

// LLM-HARNESS-BEGIN: us-8038d250
void ADX_DecodeInfoAinf() {}
// LLM-HARNESS-END: us-8038d250

// LLM-HARNESS-BEGIN: us-8038d3d4
void ADX_DecodeFooter() {}
// LLM-HARNESS-END: us-8038d3d4
