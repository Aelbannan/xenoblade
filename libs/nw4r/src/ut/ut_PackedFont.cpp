// Auto-scaffolded catalog TU for nw4r/src/ut/ut_PackedFont
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>

extern "C" void __ct__Q44nw4r2ut6detail15ArchiveFontBaseFv(void*);
extern "C" unsigned char lbl_eu_8056B084[];
extern "C" void* __ct__Q34nw4r2ut10PackedFontFv(void* self) {
    struct Layout {
        void* vtable;
        unsigned char padding[0x24];
        unsigned short field28;
        unsigned short field2A;
        unsigned int field2C;
        unsigned int field30;
        unsigned int field34;
        unsigned int field38;
    };
    Layout* object = static_cast<Layout*>(self);
    __ct__Q44nw4r2ut6detail15ArchiveFontBaseFv(self);
    object->vtable = static_cast<void*>(lbl_eu_8056B084);
    object->field28 = 0;
    object->field2A = 0;
    object->field2C = 0;
    object->field30 = 0;
    object->field34 = 0;
    object->field38 = 0;
    return self;
}

extern "C" void* __dt__Q44nw4r2ut6detail15ArchiveFontBaseFv(void*, int);
extern "C" void __dl__FPv(void*);
extern "C" void* __dt__Q34nw4r2ut10PackedFontFv(void* self, int freeFlag)
{
    if (self != 0) {
        __dt__Q44nw4r2ut6detail15ArchiveFontBaseFv(self, 0);
        if (freeFlag > 0)
            __dl__FPv(self);
    }
    return self;
}

extern "C" void GetRequireBufferSize__Q34nw4r2ut10PackedFontFPCvPCcf() {}

extern "C" void StreamingConstruct__Q34nw4r2ut10PackedFontFPQ54nw4r2ut6detail15ArchiveFontBase16ConstructContextPCvUl() {}

extern "C" void Construct__Q34nw4r2ut10PackedFontFPvUlPCvPCc() {}

extern "C" void Destroy__Q34nw4r2ut10PackedFontFv() {}

extern "C" void GetGlyph__Q34nw4r2ut10PackedFontCFPQ34nw4r2ut5GlyphUs() {}

extern "C" void CalcCopySize__Q34nw4r2ut10PackedFontFRCQ54nw4r2ut6detail15ArchiveFontBase18FontGlyphGroupsAcsPCcPi() {}

extern "C" void AssignMemory__Q34nw4r2ut10PackedFontFPUcUlUlUlUl() {}

extern "C" void MakeGlyph__Q34nw4r2ut10PackedFontCFPQ34nw4r2ut5GlyphUsi() {}

extern "C" void ConstructOpAnalyzeGLGRPacked__Q34nw4r2ut10PackedFontFPQ54nw4r2ut6detail15ArchiveFontBase16ConstructContextPQ54nw4r2ut6detail15ArchiveFontBase18CachedStreamReader() {}

extern "C" void ConstructOpPrepairCopyPackedSheet__Q34nw4r2ut10PackedFontFPQ54nw4r2ut6detail15ArchiveFontBase16ConstructContextPQ54nw4r2ut6detail15ArchiveFontBase18CachedStreamReader() {}
