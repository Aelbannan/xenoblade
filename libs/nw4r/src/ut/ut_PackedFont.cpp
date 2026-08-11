// nw4r::ut::PackedFont - packed archive font (streaming construction)
//
// The construct machinery builds the font object in place: while the archive
// is being streamed, the ConstructContext aliases the font object's memory
// (see ut_ArchiveFontBase.cpp). Class layout mirrors retail: Font base
// (vtable + CharStrmReader func), ResFontBase resource state, ArchiveFontBase
// width pointer, then the PackedFont buffer/link fields.

#include <harness_catalog.h>

#include <nw4r/ut/ut_binaryFileFormat.h>

void __ct__Q44nw4r2ut6detail15ArchiveFontBaseFv(void*);
extern unsigned char lbl_eu_8056B084[];


void* __dt__Q44nw4r2ut6detail15ArchiveFontBaseFv(void*, int);
void __dl__FPv(void*);
void* __dt__Q34nw4r2ut10PackedFontFv(void* self, int freeFlag)
{
    if (self != 0) {
        __dt__Q44nw4r2ut6detail15ArchiveFontBaseFv(self, 0);
        if (freeFlag > 0)
            __dl__FPv(self);
    }
    return self;
}

namespace nw4r {
namespace ut {

struct CharWidths {
    s8 left;        // at 0x0
    u8 glyphWidth;  // at 0x1
    s8 charWidth;   // at 0x2
};

struct Glyph {
    void* pTexture;     // at 0x0
    CharWidths widths;  // at 0x4
    u8 height;          // at 0x7
    u32 texFormat;      // at 0x8
    u16 texWidth;       // at 0xC
    u16 texHeight;      // at 0xE
    u16 cellX;          // at 0x10
    u16 cellY;          // at 0x12
};

namespace detail {

struct FontTextureGlyph {
    u8 cellWidth;     // at 0x0
    u8 cellHeight;    // at 0x1
    s8 baselinePos;   // at 0x2
    u8 maxCharWidth;  // at 0x3
    u32 sheetSize;    // at 0x4
    u16 sheetNum;     // at 0x8
    u16 sheetFormat;  // at 0xA
    u16 sheetRow;     // at 0xC
    u16 sheetLine;    // at 0xE
    u16 sheetWidth;   // at 0x10
    u16 sheetHeight;  // at 0x12
    u8* sheetImage;   // at 0x14
};

struct FontInformation {
    u8 fontType;              // at 0x0
    s8 linefeed;              // at 0x1
    u16 alterCharIndex;       // at 0x2
    CharWidths defaultWidth;  // at 0x4
    u8 encoding;              // at 0x7
    FontTextureGlyph* pGlyph; // at 0x8
    u32 pWidth;               // at 0xC
    u32 pMap;                 // at 0x10
    u8 height;                // at 0x14
    u8 width;                 // at 0x15
    u8 ascent;                // at 0x16
};

// Base font object: vtable at 0x0, CharStrmReader func at 0x4, then the
// ResFontBase resource state (mResource/mFontInfo at 0x10/0x14).
class ResFontBase {
public:
    const CharWidths& GetCharWidthsFromIndex(u16 index) const;

protected:
    u32 field_0x0;               // vtable (at 0x0)
    u32 field_0x4;               // read func (at 0x4)
    u32 field_0x8;               // at 0x8
    u32 field_0xC;               // at 0xC
    u32 mResource;               // at 0x10
    FontInformation* mFontInfo;  // at 0x14
    u16 mLastCharCode;           // at 0x18
    u16 mLastGlyphIndex;         // at 0x1A
};

class ArchiveFontBase : public ResFontBase {
public:
    class CachedStreamReader;
    struct ConstructContext;

    // Declared static so construct-op calls pass (ctx, reader, size) straight
    // through r3/r4/r5. The retail definition in ut_ArchiveFontBase.cpp is a
    // non-static member whose `this` aliases the context being built; both
    // mangle to the same symbol.
    static int RequestData(ConstructContext* pCtx, CachedStreamReader* pReader,
                           u32 size);
    void* RemoveResourceBuffer();

protected:
    u32 mWidth; // at 0x1C
};

class ArchiveFontBase::CachedStreamReader {
public:
    void Init();
    void Attach(const void* pStream, u32 size);
    u32 GetRemain() const;
    void CopyTo(void* pDst, u32 size);

    u8* mStreamStart;  // at 0x00
    u8* mStreamPos;    // at 0x04
    u8* mStreamEnd;    // at 0x08
    u8* mBufferStart;  // at 0x0C
    u8* mBufferPos;    // at 0x10
    u8* mBufferEnd;    // at 0x14
    u32 mLastSize;     // at 0x18
};

// The reader is embedded at +0x1C: Construct() keeps the context on the stack
// with the CachedStreamReader sitting right after its first seven words, so
// StreamingConstruct can recover the reader from the context.
struct ArchiveFontBase::ConstructContext {
    u32 field_0x0;              // at 0x00
    u32 field_0x4;              // at 0x04
    u32 field_0x8;              // at 0x08
    u32 field_0xC;              // at 0x0C
    u32 field_0x10;             // at 0x10
    u32 field_0x14;             // at 0x14
    u32 field_0x18;             // at 0x18
    CachedStreamReader mReader; // at 0x1C
    u32 field_0x38;             // at 0x38
    const char* field_0x3C;     // at 0x3C
    u16* field_0x40;            // at 0x40
    u32 field_0x44;             // at 0x44
    u32 field_0x48;             // at 0x48
    u32 field_0x4C;             // at 0x4C
    u32 field_0x50;             // at 0x50
    u32 field_0x54;             // at 0x54
    u32 field_0x58;             // at 0x58
    u32 field_0x5C;             // at 0x5C
    u16 field_0x60;             // at 0x60
    u16 field_0x62;             // at 0x62
    u16 field_0x64;             // at 0x64
};

} // namespace detail

class PackedFont : public detail::ArchiveFontBase {
public:
    bool Construct(void* pBuffer, u32 bufferSize, const void* pArchive,
                   const char* pName);
    void Destroy();
    void MakeGlyph(Glyph* pGlyph, u16 charCode, int charIndex) const;
    u8* AssignMemory(u8* pBuffer, u32 bufferSize, u32 sheetNum, u32 glyphNum,
                     u32 imageSize);

    int StreamingConstruct(detail::ArchiveFontBase::ConstructContext* pCtx,
                           const void* pArchive, u32 size);

    static int ConstructOpPrepairCopyPackedSheet(
        detail::ArchiveFontBase::ConstructContext* pCtx,
        detail::ArchiveFontBase::CachedStreamReader* pReader);

private:
    u8* mBuffer;     // at 0x20
    u16 mSheetCount; // at 0x24
    u16 field_0x26;  // at 0x26
    u16 field_0x28;  // at 0x28
    u16 field_0x2A;  // at 0x2A
    u32 field_0x2C;  // at 0x2C
    u32 field_0x30;  // at 0x30
    u32 field_0x34;  // at 0x34
    u32 field_0x38;  // at 0x38
};

/******************************************************************************
 *
 * Construct
 *
 ******************************************************************************/

bool PackedFont::Construct(void* pBuffer, u32 bufferSize, const void* pArchive,
                           const char* pName) {
    ConstructContext ctx;
    ctx.mReader.Init();

    u32 bufferEnd = (u32)pBuffer + bufferSize;

    ctx.field_0x38 = (bufferEnd - 0x220) & ~3u;
    ctx.field_0x3C = pName;
    ctx.field_0x40 = NULL;
    ctx.field_0x44 = (u32)pBuffer;
    ctx.field_0x48 = bufferEnd;
    ctx.field_0x4C = (u32)pBuffer;
    ctx.field_0x50 = 0xE;
    ctx.field_0x54 = 0;
    ctx.field_0x58 = 1;
    ctx.field_0x5C = 0;
    ctx.field_0x60 = 0;
    ctx.field_0x62 = 0;
    ctx.field_0x64 = 0;
    ctx.field_0x0 = 0;
    ctx.field_0x4 = 0;
    ctx.field_0x8 = 0;
    ctx.field_0xC = 1;
    ctx.field_0x18 = 0;

    const BinaryFileHeader* pHeader =
        static_cast<const BinaryFileHeader*>(pArchive);

    // StreamingConstruct returns an op status code; success is exactly 1.
    return StreamingConstruct(&ctx, pArchive, pHeader->fileSize) == 1;
}

/******************************************************************************
 *
 * Destroy
 *
 ******************************************************************************/

void PackedFont::Destroy() {
    field_0x2A = 0;
    field_0x2C = 0;
    field_0x30 = 0;
    field_0x34 = 0;
    field_0x38 = 0;
    RemoveResourceBuffer();
}

void GetRequireBufferSize__Q34nw4r2ut10PackedFontFPCvPCcf(){}

void StreamingConstruct__Q34nw4r2ut10PackedFontFPQ54nw4r2ut6detail15ArchiveFontBase16ConstructContextPCvUl(){}

void GetGlyph__Q34nw4r2ut10PackedFontCFPQ34nw4r2ut5GlyphUs(){}

void CalcCopySize__Q34nw4r2ut10PackedFontFRCQ54nw4r2ut6detail15ArchiveFontBase18FontGlyphGroupsAcsPCcPi(){}

/******************************************************************************
 *
 * AssignMemory
 *
 ******************************************************************************/

u8* PackedFont::AssignMemory(u8* pBuffer, u32 bufferSize, u32 sheetNum,
                             u32 glyphNum, u32 imageSize) {
    // Layout of the assigned buffer (all offsets rounded up to 2/4/32):
    //   [0]        linked list of sheets (4 bytes each, u16 next + u16 prev)
    //   offA       sheet map (2 bytes per sheetNum entry)
    //   offB       glyph table
    //   offC       per-glyph 4-byte entries
    //   offImage   sheet image data
    u32 offA = (sheetNum * 2 + 1) & ~1u;
    u32 offB = (glyphNum * 2 + 1) & ~1u;
    u32 offC = (glyphNum * 4 + 3) & ~3u;

    u32 remaining = bufferSize - offA - offB - offC - 4;
    if (remaining > bufferSize) {
        return NULL;
    }

    u32 count = remaining / (imageSize + 6);
    field_0x26 = 0;
    mBuffer = pBuffer;
    mSheetCount = (u16)count;

    u32 offList = ((count + 1) * 4 + 1) & ~1u;
    u32 offMap = (offList + offA + 1) & ~1u;
    u32 offGlyphTable = (offMap + offB + 1) & ~1u;
    u32 offGlyph = (offGlyphTable + count * 2 + 3) & ~3u;
    u32 offImage = (offGlyph + glyphNum * 4 + 31) & ~31u;

    // Doubly-linked sheet chain: entry i points to i+1 (last entry terminates
    // with 0) and back to i-1 (first entry wraps to `count`).
    for (u32 i = 0; i < count; i++) {
        ((u16*)mBuffer)[i * 2] = (u16)(i + 1);
        ((u16*)mBuffer)[i * 2 + 1] = (u16)((i == 0) ? count : i - 1);
    }

    ((u16*)mBuffer)[mSheetCount * 2] = 0;
    ((u16*)mBuffer)[mSheetCount * 2 + 1] = (u16)(count - 1);
    field_0x28 = 0;
    field_0x2A = (u16)count;
    field_0x2C = (u32)(pBuffer + offMap);
    field_0x30 = (u32)(pBuffer + offGlyphTable);
    field_0x34 = (u32)(pBuffer + offGlyph);
    field_0x38 = (u32)(pBuffer + offImage);

    return pBuffer + offList;
}

/******************************************************************************
 *
 * MakeGlyph
 *
 ******************************************************************************/

void PackedFont::MakeGlyph(Glyph* pGlyph, u16 charCode, int charIndex) const {
    detail::FontTextureGlyph* pSheet = mFontInfo->pGlyph;
    u32 cellsInSheet = pSheet->sheetRow * pSheet->sheetLine;
    u32 cell = charCode % cellsInSheet;
    u32 row = cell / pSheet->sheetRow;
    u32 col = cell % pSheet->sheetRow;

    pGlyph->pTexture = (void*)(field_0x38 + charIndex * pSheet->sheetSize);

    const CharWidths& widths = GetCharWidthsFromIndex(charCode);
    pGlyph->widths.left = widths.left;
    pGlyph->widths.glyphWidth = widths.glyphWidth;
    pGlyph->widths.charWidth = widths.charWidth;

    pGlyph->height = pSheet->cellHeight;
    pGlyph->texFormat = pSheet->sheetFormat;
    pGlyph->texWidth = pSheet->sheetWidth;
    pGlyph->texHeight = pSheet->sheetHeight;
    pGlyph->cellX = (u16)(col * (pSheet->cellWidth + 1) + 1);
    pGlyph->cellY = (u16)(row * (pSheet->cellHeight + 1) + 1);
}

void ConstructOpAnalyzeGLGRPacked__Q34nw4r2ut10PackedFontFPQ54nw4r2ut6detail15ArchiveFontBase16ConstructContextPQ54nw4r2ut6detail15ArchiveFontBase18CachedStreamReader(){}

/******************************************************************************
 *
 * ConstructOpPrepairCopyPackedSheet
 *
 ******************************************************************************/

int PackedFont::ConstructOpPrepairCopyPackedSheet(
    ConstructContext* pCtx, CachedStreamReader* pReader) {
    if (pCtx->field_0x60 >= pCtx->field_0x62) {
        pCtx->field_0xC = 0;
        return 3;
    }

    if (pReader->GetRemain() < 4) {
        return RequestData(pCtx, pReader, 4);
    }

    u32 data;
    pReader->CopyTo(&data, 4);

    // The next sheet's code-map entry decides how its image data is stored:
    // 0xFFFF means "not copied" (the sheet data stays in the archive).
    if (pCtx->field_0x40[pCtx->field_0x60] != 0xFFFF) {
        if (pCtx->field_0x48 - pCtx->field_0x4C < data) {
            return 2;
        }
        pCtx->field_0x54 = data;
        pCtx->field_0xC = 9;
        pCtx->field_0x50 = 8;
    } else {
        pCtx->field_0x54 = data;
        pCtx->field_0xC = 10;
        pCtx->field_0x50 = 8;
    }

    pCtx->field_0x60++;
    return 3;
}

} // namespace ut
} // namespace nw4r
