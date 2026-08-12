// nw4r::ut::detail::ArchiveFontBase - streaming (in-place) font construction
//
// The construct machinery aliases the font object's memory over the resource
// buffer: while a font is being built the object bytes are interpreted as the
// archive layout (BinaryFileHeader at +0x0, first BinaryBlockHeader at +0x10)
// and the ConstructContext doubles as the stream reader. The retail vtable is
// a data object (lbl_eu_8056AFF0), so the class is __declspec(novtable) and
// the ctor assigns that label explicitly (keeps the ctor reloc byte-identical).

#include <harness_catalog.h>
#include <string.h>

#include <nw4r/ut/ut_ArchiveFontBase.hpp>
#include <nw4r/ut/ut_ResFontBase.h>
#include <nw4r/ut/ut_binaryFileFormat.h>

// Retail vtable label (data object, owned by the .data section).
extern void* lbl_eu_8056AFF0[];

namespace nw4r {
namespace ut {
namespace detail {

class ArchiveFontBase : public ResFontBase {
public:
    class CachedStreamReader;
    struct ConstructContext;

public:
    ArchiveFontBase();
    virtual ~ArchiveFontBase(); // at 0x8

    void SetResourceBuffer(void* pBuffer, FontInformation* pInfo, u16* pWidth);
    void* RemoveResourceBuffer();

    static bool IsValidResource(const void* pResource, u32 size);

    // ------------------------------------------------------------------
    // Streaming construct machinery
    // ------------------------------------------------------------------
    class CachedStreamReader {
    public:
        void Init();
        void Attach(const void* pStream, u32 size);
        u32 GetRemain() const;
        void CopyTo(void* pDst, u32 size);
        int RequestData(ConstructContext* pCtx, u32 size);

        u8* mStreamStart;  // at 0x00
        u8* mStreamPos;    // at 0x04
        u8* mStreamEnd;    // at 0x08
        u8* mBufferStart;  // at 0x0C
        u8* mBufferPos;    // at 0x10
        u8* mBufferEnd;    // at 0x14
        u32 mLastSize;     // at 0x18
    };

    struct ConstructContext {
        u32 field_0x0;  // at 0x00
        u32 field_0x4;  // at 0x04
        u32 field_0x8;  // at 0x08
        u32 field_0xC;  // at 0x0C
        u32 field_0x10; // at 0x10
        u32 field_0x14; // at 0x14
        u32 field_0x18; // at 0x18
        u32 field_0x1C; // at 0x1C
        u32 field_0x20; // at 0x20
        u32 field_0x24; // at 0x24
        u32 field_0x28; // at 0x28
        u32 field_0x2C; // at 0x2C
        u32 field_0x30; // at 0x30
        u32 field_0x34; // at 0x34
        u32 field_0x38; // at 0x38
        u32 field_0x3C; // at 0x3C
        u32 field_0x40; // at 0x40
        u32 field_0x44; // at 0x44
        u32 field_0x48; // at 0x48
        u32 field_0x4C; // at 0x4C
        u32 field_0x50; // at 0x50
        u32 field_0x54; // at 0x54
        u32 field_0x58; // at 0x58
        u32 field_0x5C; // at 0x5C
        u16 field_0x60; // at 0x60
        u16 field_0x62; // at 0x62
    };

    int RequestData(ConstructContext* pCtx, CachedStreamReader* pReader, u32 size);

    static int ConstructOpAnalyzeFINF(ConstructContext* pCtx,
                                      CachedStreamReader* pReader);

    static int ConstructOpDispatch(ConstructContext* pCtx,
                                   CachedStreamReader* pReader);
    static int ConstructOpAnalyzeFileHeader(ConstructContext* pCtx,
                                            CachedStreamReader* pReader);
    static int ConstructOpAnalyzeTGLP(ConstructContext* pCtx,
                                      CachedStreamReader* pReader);

private:
    u16* mWidth; // at 0x1C
};

/******************************************************************************
 *
 * Constructor / destructor
 *
 ******************************************************************************/

ArchiveFontBase::ArchiveFontBase() : mWidth(NULL) {
    *(void**)this = (void*)lbl_eu_8056AFF0;
}

ArchiveFontBase::~ArchiveFontBase() {}

/******************************************************************************
 *
 * Resource management
 *
 ******************************************************************************/

void ArchiveFontBase::SetResourceBuffer(void* pBuffer, FontInformation* pInfo,
                                        u16* pWidth) {
    ResFontBase::SetResourceBuffer(pBuffer, pInfo);
    mWidth = pWidth;
}

void* ArchiveFontBase::RemoveResourceBuffer() {
    mWidth = NULL;
    return ResFontBase::RemoveResourceBuffer();
}

/******************************************************************************
 *
 * Validation
 *
 ******************************************************************************/

bool ArchiveFontBase::IsValidResource(const void* pResource, u32 size) {
    if (!IsValidBinaryFile(static_cast<const BinaryFileHeader*>(pResource),
                           0x52464E41 /* 'RFNA' */, NW4R_VERSION(1, 4), 2)) {
        return false;
    }

    // First block must be the glyph block ('GLGR'); its header lives at +0x10.
    const BinaryBlockHeader* pBlock = reinterpret_cast<const BinaryBlockHeader*>(
        reinterpret_cast<const u8*>(pResource) + 0x10);

    if (pBlock->kind != 0x474C4752 /* 'GLGR' */) {
        return false;
    }

    // The resource must be large enough to contain the first block
    // (block size + its 0x10-byte header). Unsigned >= via MWCC's
    // branchless compare idiom.
    u32 blockEnd = pBlock->size + 0x10;
    return size >= blockEnd;
}

/******************************************************************************
 *
 * Construct machinery
 *
 ******************************************************************************/

int ArchiveFontBase::RequestData(ConstructContext* pCtx,
                                 CachedStreamReader* pReader, u32 size) {
    // The construct context aliases the font object being built (in-place
    // construction): accumulate the consumed-byte total on this->0x18.
    ConstructContext* pSelf = reinterpret_cast<ConstructContext*>(this);
    pSelf->field_0x18 +=
        (pCtx->field_0x4 - pCtx->field_0x0) + (pCtx->field_0x10 - pCtx->field_0xC);

    // Note: retail passes the reader pointer where the size argument belongs
    // (the size parameter is unused).
    int result = reinterpret_cast<CachedStreamReader*>(pCtx)->RequestData(
        pSelf, reinterpret_cast<u32>(pReader));
    int ret = 2;
    if (result != 0) {
        ret = 0;
    }
    return ret;
}

int ArchiveFontBase::ConstructOpAnalyzeFINF(ConstructContext* pCtx,
                                            CachedStreamReader* pReader) {
    if (pCtx->field_0x48 - pCtx->field_0x4C < pCtx->field_0x14 - 8) {
        return 2;
    }

    pCtx->field_0x54 = pCtx->field_0x14 - 8;
    pCtx->field_0x0 =
        *reinterpret_cast<volatile u32*>(&pCtx->field_0x4C);
    pCtx->field_0xC = 9;
    pCtx->field_0x50 = 0;
    return 3;
}

int ArchiveFontBase::CachedStreamReader::RequestData(ConstructContext* pCtx,
                                                     u32 size) {
    u32 remain = (mStreamEnd - mStreamPos) + (mBufferEnd - mBufferPos);
    if (remain == 0) {
        mBufferStart = 0;
        mBufferPos = 0;
        mBufferEnd = 0;
        mLastSize = 0;
        return 1;
    }

    // The font buffer must still have room for twice the requested size
    // (the request is served from the write cursor in the font buffer).
    if (pCtx->field_0x48 - pCtx->field_0x4C < size * 2) {
        return 0;
    }

    u32 avail = mBufferEnd - mBufferPos;
    u8* pDst = reinterpret_cast<u8*>(pCtx->field_0x4C) + size;
    if (avail >= remain) {
        memmove(pDst, mBufferPos, remain);
        mBufferPos += remain;
    } else {
        u32 remaining = remain - avail;
        memmove(pDst, mBufferPos, avail);
        memmove(pDst + avail, mStreamPos, remaining);
        mBufferPos = mBufferEnd;
        mStreamPos += remaining;
    }

    mBufferStart = pDst;
    mBufferPos = pDst;
    mBufferEnd = pDst + remain;
    mLastSize = size;
    return 1;
}

int ArchiveFontBase::ConstructOpDispatch(ConstructContext* pCtx,
                                         CachedStreamReader* pReader) {
    if (pCtx->field_0x5C >= pCtx->field_0x58) {
        return 1;
    }

    // Need at least an 8-byte block header in the stream cache.
    if ((pCtx->field_0x8 - pCtx->field_0x4) +
            (pCtx->field_0x14 - pCtx->field_0x10) <
        0x8) {
        pCtx->field_0x18 = pCtx->field_0x18 +
            (pCtx->field_0x4 - pCtx->field_0x0) +
            (pCtx->field_0x10 - pCtx->field_0xC);
        if (pReader->RequestData(pCtx, 0x8) == 0) {
            return 2;
        }
        return 0;
    }

    // Copy the block header (kind + size) into the context at +0x10.
    u32 avail = pCtx->field_0x14 - pCtx->field_0x10;
    void* pDst = &pCtx->field_0x10;
    if (avail >= 0x8) {
        memcpy(pDst, reinterpret_cast<const void*>(pCtx->field_0x10), 0x8);
        pReader->mBufferPos += 0x8;
    } else {
        u32 remaining = 0x8 - avail;
        memcpy(pDst, reinterpret_cast<const void*>(pCtx->field_0x10), avail);
        memcpy(reinterpret_cast<u8*>(pDst) + avail, pReader->mStreamPos,
               remaining);
        pReader->mBufferPos = pReader->mBufferEnd;
        pReader->mStreamPos += remaining;
    }

    // Dispatch on the block kind to the per-block analyzer.
    switch (pCtx->field_0x10) {
    case 0x474C4752: // 'GLGR'
        pCtx->field_0xC = 2;
        break;
    case 0x46494E46: // 'FINF'
        pCtx->field_0xC = 3;
        break;
    case 0x434D4150: // 'CMAP'
        pCtx->field_0xC = 4;
        break;
    case 0x43574844: // 'CWHD'
        pCtx->field_0xC = 5;
        break;
    case 0x54474C50: // 'TGLP'
        pCtx->field_0xC = 6;
        break;
    default:
        pCtx->field_0xC = 0xC;
        return 2;
    }

    pCtx->field_0x5C += 1;
    pCtx->field_0x4C = (pCtx->field_0x4C + 3) & ~3;
    return 3;
}

int ArchiveFontBase::ConstructOpAnalyzeFileHeader(ConstructContext* pCtx,
                                                  CachedStreamReader* pReader) {
    // Need at least a full 0x10-byte block header in the stream cache.
    if ((pCtx->field_0x8 - pCtx->field_0x4) +
            (pCtx->field_0x14 - pCtx->field_0x10) <
        0x10) {
        pCtx->field_0x18 = pCtx->field_0x18 +
            (pCtx->field_0x4 - pCtx->field_0x0) +
            (pCtx->field_0x10 - pCtx->field_0xC);
        if (pReader->RequestData(pCtx, 0x10) == 0) {
            return 2;
        }
        return 0;
    }

    if (pCtx->field_0x48 - pCtx->field_0x4C < 0x10) {
        return 2;
    }

    u32 avail = pCtx->field_0x14 - pCtx->field_0x10;
    if (avail >= 0x10) {
        memcpy(reinterpret_cast<void*>(pCtx->field_0x4C),
               reinterpret_cast<const void*>(pCtx->field_0x10), 0x10);
        pReader->mBufferPos += 0x10;
    } else {
        u32 remaining = 0x10 - avail;
        memcpy(reinterpret_cast<void*>(pCtx->field_0x4C),
               reinterpret_cast<const void*>(pCtx->field_0x10), avail);
        memcpy(reinterpret_cast<void*>(pCtx->field_0x4C + avail),
               pReader->mStreamPos, remaining);
        pReader->mBufferPos = pReader->mBufferEnd;
        pReader->mStreamPos += remaining;
    }

    pCtx->field_0xC = 0;
    return 3;
}

int ArchiveFontBase::ConstructOpAnalyzeTGLP(ConstructContext* pCtx,
                                            CachedStreamReader* pReader) {
    // Need at least a full 0x18-byte TGLP header in the stream cache.
    if ((pCtx->field_0x8 - pCtx->field_0x4) +
            (pCtx->field_0x14 - pCtx->field_0x10) <
        0x18) {
        pCtx->field_0x18 = pCtx->field_0x18 +
            (pCtx->field_0x4 - pCtx->field_0x0) +
            (pCtx->field_0x10 - pCtx->field_0xC);
        if (pReader->RequestData(pCtx, 0x18) == 0) {
            return 2;
        }
        return 0;
    }

    if (pCtx->field_0x48 - pCtx->field_0x4C < 0x18) {
        return 2;
    }

    // The context's first word aliases the FontInformation of the font being
    // built: point its glyph sheet at the current write position.
    reinterpret_cast<FontInformation*>(pCtx->field_0x0)->pGlyph =
        reinterpret_cast<FontTextureGlyph*>(pCtx->field_0x4C);

    u32 avail = pCtx->field_0x14 - pCtx->field_0x10;
    if (avail >= 0x18) {
        memcpy(reinterpret_cast<void*>(pCtx->field_0x4C),
               reinterpret_cast<const void*>(pCtx->field_0x10), 0x18);
        pReader->mBufferPos += 0x18;
    } else {
        u32 remaining = 0x18 - avail;
        memcpy(reinterpret_cast<void*>(pCtx->field_0x4C),
               reinterpret_cast<const void*>(pCtx->field_0x10), avail);
        memcpy(reinterpret_cast<void*>(pCtx->field_0x4C + avail),
               pReader->mStreamPos, remaining);
        pReader->mBufferPos = pReader->mBufferEnd;
        pReader->mStreamPos += remaining;
    }

    pCtx->field_0x4C += 0x18;

    // TGLP header: clear the 'reverse' flag in sheetFormat and remember it.
    u16 sheetFormat = reinterpret_cast<FontInformation*>(pCtx->field_0x0)
                          ->pGlyph->sheetFormat;
    reinterpret_cast<FontInformation*>(pCtx->field_0x0)->pGlyph->sheetFormat =
        sheetFormat & 0x7FFF;
    u32 reverse = (sheetFormat >> 15) & 1;

    // Count the glyphs whose code-map entry is not 0xFFFF.
    u16 count = 0;
    for (u32 i = 0; i < pCtx->field_0x62; i++) {
        if (reinterpret_cast<const u16*>(pCtx->field_0x40)[i] != 0xFFFF) {
            count++;
        }
    }
    reinterpret_cast<FontInformation*>(pCtx->field_0x0)->pGlyph->sheetNum =
        count;

    // Align the write position to 0x20 and record the sheet image pointer.
    u32 image = reinterpret_cast<u32>(
        reinterpret_cast<FontInformation*>(pCtx->field_0x0)
            ->pGlyph->sheetImage);
    pCtx->field_0x4C = (pCtx->field_0x4C + 0x1F) & ~0x1F;
    reinterpret_cast<FontInformation*>(pCtx->field_0x0)->pGlyph->sheetImage =
        reinterpret_cast<u8*>(pCtx->field_0x4C);

    pCtx->field_0xC = 0xA;
    pCtx->field_0x50 = (reverse != 0) ? 8 : 7;
    pCtx->field_0x54 =
        image -
        (pCtx->field_0x18 + (pReader->mStreamPos - pReader->mStreamStart) +
         (pReader->mBufferPos - pReader->mBufferStart));
    return 3;
}

} // namespace detail
} // namespace ut
} // namespace nw4r

void GetCharWidths__Q44nw4r2ut6detail15ArchiveFontBaseCFUs(){}

void HasGlyph__Q44nw4r2ut6detail15ArchiveFontBaseCFUs(){}

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

extern "C" void
CopyTo__Q54nw4r2ut6detail15ArchiveFontBase18CachedStreamReaderFPvUl(void* self,
                                                                 void* pDst,
                                                                 unsigned int size) {
    unsigned int* p = (unsigned int*)self;

    unsigned int avail = p[5] - p[4];  // mStreamEnd - mStart

    if (avail >= size) {
        memcpy(pDst, (void*)p[4], size);
        p[4] = p[4] + size;
    } else {
        unsigned int remaining = size - avail;
        memcpy(pDst, (void*)p[4], avail);
        memcpy((void*)((unsigned int)pDst + avail), (void*)p[1], remaining);
        p[4] = p[5];
        p[1] = p[1] + remaining;
    }
}

