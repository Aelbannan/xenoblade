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

void AdjustIndex__Q44nw4r2ut6detail15ArchiveFontBaseCFUs(){}

void IncludeName__Q44nw4r2ut6detail15ArchiveFontBaseFPCcPCc(){}

void IsValidResource__Q44nw4r2ut6detail15ArchiveFontBaseFPCvUl(){}

void RequestData__Q44nw4r2ut6detail15ArchiveFontBaseFPQ54nw4r2ut6detail15ArchiveFontBase16ConstructContextPQ54nw4r2ut6detail15ArchiveFontBase18CachedStreamReaderUl(){}

void ConstructOpDispatch__Q44nw4r2ut6detail15ArchiveFontBaseFPQ54nw4r2ut6detail15ArchiveFontBase16ConstructContextPQ54nw4r2ut6detail15ArchiveFontBase18CachedStreamReader(){}

void ConstructOpAnalyzeFileHeader__Q44nw4r2ut6detail15ArchiveFontBaseFPQ54nw4r2ut6detail15ArchiveFontBase16ConstructContextPQ54nw4r2ut6detail15ArchiveFontBase18CachedStreamReader(){}

void ConstructOpAnalyzeFINF__Q44nw4r2ut6detail15ArchiveFontBaseFPQ54nw4r2ut6detail15ArchiveFontBase16ConstructContextPQ54nw4r2ut6detail15ArchiveFontBase18CachedStreamReader(){}

void ConstructOpAnalyzeCMAP__Q44nw4r2ut6detail15ArchiveFontBaseFPQ54nw4r2ut6detail15ArchiveFontBase16ConstructContextPQ54nw4r2ut6detail15ArchiveFontBase18CachedStreamReader(){}

void ConstructOpAnalyzeCWDH__Q44nw4r2ut6detail15ArchiveFontBaseFPQ54nw4r2ut6detail15ArchiveFontBase16ConstructContextPQ54nw4r2ut6detail15ArchiveFontBase18CachedStreamReader(){}

void ConstructOpAnalyzeTGLP__Q44nw4r2ut6detail15ArchiveFontBaseFPQ54nw4r2ut6detail15ArchiveFontBase16ConstructContextPQ54nw4r2ut6detail15ArchiveFontBase18CachedStreamReader(){}

void ConstructOpCopy__Q44nw4r2ut6detail15ArchiveFontBaseFPQ54nw4r2ut6detail15ArchiveFontBase16ConstructContextPQ54nw4r2ut6detail15ArchiveFontBase18CachedStreamReader(){}

void ConstructOpSkip__Q44nw4r2ut6detail15ArchiveFontBaseFPQ54nw4r2ut6detail15ArchiveFontBase16ConstructContextPQ54nw4r2ut6detail15ArchiveFontBase18CachedStreamReader(){}

void ConstructOpFatalError__Q44nw4r2ut6detail15ArchiveFontBaseFPQ54nw4r2ut6detail15ArchiveFontBase16ConstructContextPQ54nw4r2ut6detail15ArchiveFontBase18CachedStreamReader(){}

void Init__Q54nw4r2ut6detail15ArchiveFontBase18CachedStreamReaderFv(){}

void Attach__Q54nw4r2ut6detail15ArchiveFontBase18CachedStreamReaderFPCvUl(){}

extern "C" unsigned int
GetRemain__Q54nw4r2ut6detail15ArchiveFontBase18CachedStreamReaderCFv(const void* self) {
    return (*(const unsigned int*)((const char*)self + 8) - *(const unsigned int*)((const char*)self + 4)) +
           (*(const unsigned int*)((const char*)self + 0x14) - *(const unsigned int*)((const char*)self + 0x10));
}

void CopyTo__Q54nw4r2ut6detail15ArchiveFontBase18CachedStreamReaderFPvUl(){}

void RequestData__Q54nw4r2ut6detail15ArchiveFontBase18CachedStreamReaderFPQ54nw4r2ut6detail15ArchiveFontBase16ConstructContextUl(){}
