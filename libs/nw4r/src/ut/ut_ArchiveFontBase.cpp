// Auto-scaffolded catalog TU for nw4r/src/ut/ut_ArchiveFontBase
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>

void __ct__Q44nw4r2ut6detail15ArchiveFontBaseFv(){}

void* __dt__Q44nw4r2ut6detail15ArchiveFontBaseFv(void* self, int deleting)
{
    extern void __dt__Q44nw4r2ut6detail11ResFontBaseFv(void*, int);
    extern void __dl__FPv(void*);

    if (self != 0) {
        __dt__Q44nw4r2ut6detail11ResFontBaseFv(self, 0);
        if (deleting > 0)
            __dl__FPv(self);
    }
    return self;
}

void GetCharWidths__Q44nw4r2ut6detail15ArchiveFontBaseCFUs(){}

void HasGlyph__Q44nw4r2ut6detail15ArchiveFontBaseCFUs(){}

void SetResourceBuffer__Q44nw4r2ut6detail15ArchiveFontBaseFPvPQ34nw4r2ut15FontInformationPUs(){}

void RemoveResourceBuffer__Q44nw4r2ut6detail15ArchiveFontBaseFv(void){}

extern "C" unsigned short
AdjustIndex__Q44nw4r2ut6detail15ArchiveFontBaseCFUs(const void* self,
                                                       unsigned short index) {
    const unsigned char* p = (const unsigned char*)self;

    const void* pInfo = *(const void**)(p + 0x14);
    const unsigned short* pTable = *(const unsigned short**)(p + 0x1c);
    const unsigned char* pSub = *(const unsigned char**)((const unsigned char*)pInfo + 8);

    int cellsInASheet =
        *(const unsigned short*)(pSub + 0xc) *
        *(const unsigned short*)(pSub + 0xe);

    int sheet = index / cellsInASheet;

    unsigned short mapped = pTable[sheet];
    if (mapped == 0xffff) {
        return 0xffff;
    }

    return index - mapped;
}

void IncludeName__Q44nw4r2ut6detail15ArchiveFontBaseFPCcPCc(){}

void IsValidResource__Q44nw4r2ut6detail15ArchiveFontBaseFPCvUl(){}

void RequestData__Q44nw4r2ut6detail15ArchiveFontBaseFPQ54nw4r2ut6detail15ArchiveFontBase16ConstructContextPQ54nw4r2ut6detail15ArchiveFontBase18CachedStreamReaderUl(){}

void ConstructOpDispatch__Q44nw4r2ut6detail15ArchiveFontBaseFPQ54nw4r2ut6detail15ArchiveFontBase16ConstructContextPQ54nw4r2ut6detail15ArchiveFontBase18CachedStreamReader(){}

void ConstructOpAnalyzeFileHeader__Q44nw4r2ut6detail15ArchiveFontBaseFPQ54nw4r2ut6detail15ArchiveFontBase16ConstructContextPQ54nw4r2ut6detail15ArchiveFontBase18CachedStreamReader(){}

void ConstructOpAnalyzeFINF__Q44nw4r2ut6detail15ArchiveFontBaseFPQ54nw4r2ut6detail15ArchiveFontBase16ConstructContextPQ54nw4r2ut6detail15ArchiveFontBase18CachedStreamReader(){}

extern "C" int
ConstructOpAnalyzeCMAP__Q44nw4r2ut6detail15ArchiveFontBaseFPQ54nw4r2ut6detail15ArchiveFontBase16ConstructContextPQ54nw4r2ut6detail15ArchiveFontBase18CachedStreamReader(void* ctx,
                                                                                                                                 void* reader) {
    unsigned int* c = (unsigned int*)ctx;

    unsigned int cur = c[0x13];      // 0x4c
    unsigned int block = c[2];       // 0x8

    if (block != 0) {
        ((unsigned int*)block)[2] = cur; // block->0x8 = cur
    } else {
        ((unsigned int*)c[0])[4] = cur;  // base->0x10 = cur
    }

    c[2] = cur;                     // ctx->0x8 = cur

    unsigned int limit = c[5];      // 0x14
    unsigned int base = c[0x12];    // 0x48

    if (base - c[0x13] < limit - 8) {
        return 2;
    }

    c[0x15] = limit - 8;            // 0x54
    c[3] = 9;                       // 0xc
    c[0x14] = 0;                    // 0x50
    return 3;
}

extern "C" int
ConstructOpAnalyzeCWDH__Q44nw4r2ut6detail15ArchiveFontBaseFPQ54nw4r2ut6detail15ArchiveFontBase16ConstructContextPQ54nw4r2ut6detail15ArchiveFontBase18CachedStreamReader(void* ctx,
                                                                                                                                 void* reader) {
    unsigned int* c = (unsigned int*)ctx;

    unsigned int cur = c[0x13];      // 0x4c
    unsigned int block = c[1];       // 0x4

    if (block != 0) {
        ((unsigned int*)block)[1] = cur; // block->0x4 = cur
    } else {
        ((unsigned int*)c[0])[3] = cur;  // base->0xc = cur
    }

    c[1] = cur;                     // ctx->0x4 = cur

    unsigned int limit = c[5];      // 0x14
    unsigned int base = c[0x12];    // 0x48

    if (base - c[0x13] < limit - 8) {
        return 2;
    }

    c[0x15] = limit - 8;            // 0x54
    c[3] = 9;                       // 0xc
    c[0x14] = 0;                    // 0x50
    return 3;
}

void ConstructOpAnalyzeTGLP__Q44nw4r2ut6detail15ArchiveFontBaseFPQ54nw4r2ut6detail15ArchiveFontBase16ConstructContextPQ54nw4r2ut6detail15ArchiveFontBase18CachedStreamReader(){}

void ConstructOpCopy__Q44nw4r2ut6detail15ArchiveFontBaseFPQ54nw4r2ut6detail15ArchiveFontBase16ConstructContextPQ54nw4r2ut6detail15ArchiveFontBase18CachedStreamReader(){}

void ConstructOpSkip__Q44nw4r2ut6detail15ArchiveFontBaseFPQ54nw4r2ut6detail15ArchiveFontBase16ConstructContextPQ54nw4r2ut6detail15ArchiveFontBase18CachedStreamReader(){}

extern "C" int
ConstructOpFatalError__Q44nw4r2ut6detail15ArchiveFontBaseFPQ54nw4r2ut6detail15ArchiveFontBase16ConstructContextPQ54nw4r2ut6detail15ArchiveFontBase18CachedStreamReader(void* ctx,
                                                                                                                                 void* reader) {
    ((unsigned int*)ctx)[3] = 12;
    return 2;
}

extern "C" void
Init__Q54nw4r2ut6detail15ArchiveFontBase18CachedStreamReaderFv(void* self) {
    unsigned int* p = (unsigned int*)self;
    p[0] = 0;
    p[1] = 0;
    p[2] = 0;
    p[3] = 0;
    p[4] = 0;
    p[5] = 0;
    p[6] = 0;
}

extern "C" void
Attach__Q54nw4r2ut6detail15ArchiveFontBase18CachedStreamReaderFPCvUl(void* self,
                                                              const void* pStream,
                                                              unsigned int size) {
    unsigned int* p = (unsigned int*)self;
    p[0] = (unsigned int)pStream;
    p[1] = (unsigned int)pStream;
    p[2] = (unsigned int)pStream + size;
}

extern "C" unsigned int
GetRemain__Q54nw4r2ut6detail15ArchiveFontBase18CachedStreamReaderCFv(const void* self) {
    return (*(const unsigned int*)((const char*)self + 8) - *(const unsigned int*)((const char*)self + 4)) +
           (*(const unsigned int*)((const char*)self + 0x14) - *(const unsigned int*)((const char*)self + 0x10));
}

void CopyTo__Q54nw4r2ut6detail15ArchiveFontBase18CachedStreamReaderFPvUl(){}

void RequestData__Q54nw4r2ut6detail15ArchiveFontBase18CachedStreamReaderFPQ54nw4r2ut6detail15ArchiveFontBase16ConstructContextUl(){}
