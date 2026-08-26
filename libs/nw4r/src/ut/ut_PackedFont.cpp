// nw4r::ut::PackedFont - packed archive font (streaming construction)
//
// The construct machinery builds the font object in place: while the archive
// is being streamed, the ConstructContext aliases the font object's memory
// (see ut_ArchiveFontBase.cpp). Class layout mirrors retail: Font base
// (vtable + CharStrmReader func), ResFontBase resource state, ArchiveFontBase
// width pointer, then the PackedFont buffer/link fields.

#include <harness_catalog.h>
#include <string.h>

#include <nw4r/ut/ut_binaryFileFormat.h>

#include <revolution/GX.h>
#include <revolution/OS.h>

void __ct__Q44nw4r2ut6detail15ArchiveFontBaseFv(void*);
extern unsigned char lbl_eu_8056B084[];
extern const double lbl_eu_8066A138;  // 2^52 magic double for the int->float conversion

// CX stream decompression. revolution/CX.h pulls in CXUncompression.h, whose
// public header is missing its own API declarations in this tree (and is
// outside this session's writable scope), so declare them with C linkage
// here - they are genuine C-ABI SDK functions.
extern "C" {
u32 CXGetUncompressedSize(const u8* src);
void CXUncompressHuffman(const u8* src, u8* dst);
}

// Rebuild a double from its two 32-bit halves; reproduces the retail int->float
// conversion (bit pattern + the named sdata2 2^52 constant) so the pool reloc
// keeps the retail label instead of an MWCC-synthesised entry.
union F64Conv {
    u32 w[2];
    double d;
};


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

enum FontEncoding {
    FONT_ENCODING_UTF8,
    FONT_ENCODING_UTF16,
    FONT_ENCODING_SJIS,
    FONT_ENCODING_CP1252,
    FONT_ENCODING_MAX
};

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

// Call-side view of the retail Font vtable: 20 slots before GetEncoding
// (offset 0x58). Used only for virtual dispatch - never constructed, so it
// adds no vtable/vptr stores to this TU.
class FontVTable {
public:
    virtual void slot00();
    virtual void slot01();
    virtual void slot02();
    virtual void slot03();
    virtual void slot04();
    virtual void slot05();
    virtual void slot06();
    virtual void slot07();
    virtual void slot08();
    virtual void slot09();
    virtual void slot10();
    virtual void slot11();
    virtual void slot12();
    virtual void slot13();
    virtual void slot14();
    virtual void slot15();
    virtual void slot16();
    virtual void slot17();
    virtual void slot18();
    virtual void slot19();
    virtual FontEncoding GetEncoding() const;
};

// Local flattened mirror of nw4r::ut::Font: vptr at +0x0, CharStrmReader
// function pointer at +0x4. Only the member this TU calls is declared
// (InitReaderFunc is defined in ut_Font.cpp).
class Font {
public:
    void InitReaderFunc(FontEncoding encode);

protected:
    u32 field_0x0;  // vtable (at 0x0)
    u32 field_0x4;  // CharStrmReader read func (at 0x4)
};

// Retail's SetResourceBuffer takes a top-level nw4r::ut::FontInformation
// (same layout as ut_ResFontBase.h); defined here so call-site mangling
// matches retail.
namespace detail { struct FontTextureGlyph; }

struct FontInformation {
    u8 fontType;              // at 0x0
    s8 linefeed;              // at 0x1
    u16 alterCharIndex;       // at 0x2
    CharWidths defaultWidth;  // at 0x4
    u8 encoding;              // at 0x7
    detail::FontTextureGlyph* pGlyph; // at 0x8
    u32 pWidth;               // at 0xC
    u32 pMap;                 // at 0x10
    u8 height;                // at 0x14
    u8 width;                 // at 0x15
    u8 ascent;                // at 0x16
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

// Base font object: vtable at 0x0, CharStrmReader func at 0x4, then the
// ResFontBase resource state (mResource/mFontInfo at 0x10/0x14).
class ResFontBase : public Font {
public:
    const CharWidths& GetCharWidthsFromIndex(u16 index) const;
    u16 GetGlyphIndex(u16 ch) const;

protected:
    u32 field_0x8;               // at 0x8
    u32 field_0xC;               // at 0xC
    u32 mResource;               // at 0x10
    FontInformation* mFontInfo;  // at 0x14
    u16 mLastCharCode;           // at 0x18
    u16 mLastGlyphIndex;         // at 0x1A
};

class ArchiveFontBase : public ResFontBase {
public:
    ArchiveFontBase();

    class CachedStreamReader;
    struct ConstructContext;
    struct FontGlyphGroupsAcs;

    // Declared static so construct-op calls pass (ctx, reader, size) straight
    // through r3/r4/r5. The retail definition in ut_ArchiveFontBase.cpp is a
    // non-static member whose `this` aliases the context being built; both
    // mangle to the same symbol.
    static int RequestData(ConstructContext* pCtx, CachedStreamReader* pReader,
                           u32 size);
    void* RemoveResourceBuffer();

    u16 AdjustIndex(u16 index) const;
    static bool IsValidResource(const void* pResource, u32 size);
    void SetResourceBuffer(void* pBuffer, FontInformation* pInfo, u16* pWidth);
    static bool IncludeName(const char* pName, const char* pSheet);

    static int ConstructOpDispatch(ConstructContext* pCtx,
                                   CachedStreamReader* pReader);
    static int ConstructOpAnalyzeFileHeader(ConstructContext* pCtx,
                                            CachedStreamReader* pReader);
    static int ConstructOpAnalyzeFINF(ConstructContext* pCtx,
                                      CachedStreamReader* pReader);
    static int ConstructOpAnalyzeCMAP(ConstructContext* pCtx,
                                      CachedStreamReader* pReader);
    static int ConstructOpAnalyzeCWDH(ConstructContext* pCtx,
                                      CachedStreamReader* pReader);
    static int ConstructOpCopy(ConstructContext* pCtx,
                               CachedStreamReader* pReader);
    static int ConstructOpSkip(ConstructContext* pCtx,
                               CachedStreamReader* pReader);
    static int ConstructOpFatalError(ConstructContext* pCtx,
                                     CachedStreamReader* pReader);
    static int ConstructOpAnalyzeTGLP(ConstructContext* pCtx,
                                      CachedStreamReader* pReader);

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
    u32 field_0x40;             // at 0x40 (code map base, u16 entries)
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

// Summary of the packed glyph-group tables laid out in the resource. The
// pointer fields are resource-relative addresses, the stride fields are the
// per-group sheet-mask strides in bytes (see GetRequireBufferSize).
struct ArchiveFontBase::FontGlyphGroupsAcs {
    const u8* field_0x0;   // at 0x00: resource base
    const u8* field_0x4;   // at 0x04: resource + 0x10 (info/block data)
    u32 field_0x8;         // at 0x08
    const u32* field_0xC;  // at 0x0C: B-group glyph size table
    const u32* field_0x10; // at 0x10: C-group glyph size table
    const u32* field_0x14; // at 0x14: D-group glyph size table
    const u8* field_0x18;  // at 0x18: B-group sheet mask table
    const u8* field_0x1C;  // at 0x1C: C-group sheet mask table
    const u8* field_0x20;  // at 0x20: D-group sheet mask table
    u32 field_0x24;        // at 0x24: B mask stride
    u32 field_0x28;        // at 0x28: C mask stride
    u32 field_0x2C;        // at 0x2C: D mask stride
};

} // namespace detail

// Layout of the packed-font archive resource (RFNA file: 0x10-byte file
// header, then the GLGR block).
struct PackedFontResource {
    u8 field_0x0[0x10];  // BinaryFileHeader
    u32 field_0x10;      // 'GLGR' block kind
    u32 field_0x14;      // GLGR block size
    u32 field_0x18;      // sheet image size
    u16 field_0x1C;      // extra count (glyph-map skip step)
    u16 field_0x1E;      // glyphs per group
    u16 field_0x20;      // total glyphs (B)
    u16 field_0x22;      // C-group count
    u16 field_0x24;      // D-group count
    u16 field_0x26[];    // per-glyph sheet-name indices
};

// The GLGR block itself (sits at resource + 0x10, i.e. FontGlyphGroupsAcs
// field_0x4): header followed by per-glyph sheet-name indices.
struct GlyphGroupsBlock {
    u32 kind;           // at 0x00 ('GLGR')
    u32 size;           // at 0x04
    u32 imageSize;      // at 0x08
    u16 field_0xC;      // at 0x0C: extra count (glyph-map skip step)
    u16 glyphsPerGroup; // at 0x0E: glyphs per group (A)
    u16 groupBCount;    // at 0x10: total glyphs (B)
    u16 groupCCount;    // at 0x12: C-group count
    u16 groupDCount;    // at 0x14: D-group count
    u16 nameIndices[];  // at 0x16: per-glyph sheet-name indices
};

class PackedFont : public detail::ArchiveFontBase {
public:
    PackedFont();

    bool Construct(void* pBuffer, u32 bufferSize, const void* pArchive,
                   const char* pName);
    void Destroy();
    void MakeGlyph(Glyph* pGlyph, u16 charCode, int charIndex) const;
    u8* AssignMemory(u8* pBuffer, u32 bufferSize, u32 sheetNum, u32 glyphNum,
                     u32 imageSize);

    int StreamingConstruct(detail::ArchiveFontBase::ConstructContext* pCtx,
                           const void* pArchive, u32 size);
    int ConstructOpAnalyzeGLGRPacked(
        detail::ArchiveFontBase::ConstructContext* pCtx,
        detail::ArchiveFontBase::CachedStreamReader* pReader);

    static u32 GetRequireBufferSize(const void* pResource, const char* pName,
                                    float scale);
    static u32 CalcCopySize(
        const detail::ArchiveFontBase::FontGlyphGroupsAcs& rGroups,
        const char* pName, int* pResult);
    void GetGlyph(Glyph* pGlyph, u16 charCode) const;

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
 * Constructor
 *
 ******************************************************************************/

// Retail ctor: base ArchiveFontBase ctor runs first (its own vptr store),
// then the first member zero lands BEFORE this class's vptr overwrite
// (lbl_eu_8056B084), then the remaining fields zero.
PackedFont::PackedFont() {
    field_0x28 = 0;
    *(void**)this = (void*)lbl_eu_8056B084;
    field_0x2A = 0;
    field_0x2C = 0;
    field_0x30 = 0;
    field_0x34 = 0;
    field_0x38 = 0;
}

/******************************************************************************
 *
 * Construct
 *
 ******************************************************************************/

bool PackedFont::Construct(void* pBuffer, u32 bufferSize, const void* pArchive,
                           const char* pName) {
    // Total archive byte count to stream (BinaryFileHeader::fileSize). Read
    // first so it lands in a saved register like retail.
    u32 fileSize =
        static_cast<const BinaryFileHeader*>(pArchive)->fileSize;

    // Assignment order mirrors the retail store schedule; it also pins
    // MWCC's temp register allocation.
    ConstructContext ctx;
    ctx.mReader.Init();

    u32 bufferEnd = (u32)pBuffer + bufferSize;
    ctx.field_0x3C = pName;
    ctx.field_0x40 = 0;
    ctx.field_0x44 = (u32)pBuffer;
    ctx.field_0x48 = bufferEnd;
    ctx.field_0x4C = (u32)pBuffer;
    ctx.field_0x38 = (bufferEnd - 0x220) & ~3u;
    ctx.field_0x50 = 0xE;
    ctx.field_0x54 = 0;
    ctx.field_0x58 = 1;
    ctx.field_0x5C = 0;
    ctx.field_0x60 = 0;
    ctx.field_0x62 = 0;
    ctx.field_0x64 = 0;
    ctx.field_0x18 = 0;
    ctx.field_0x0 = 0;
    ctx.field_0x4 = 0;
    ctx.field_0x8 = 0;
    ctx.field_0xC = 1;

    // StreamingConstruct returns an op status code; success is exactly 1.
    return StreamingConstruct(&ctx, pArchive, fileSize) == 1;
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

/******************************************************************************
 *
 * GetRequireBufferSize
 *
 ******************************************************************************/

u32 PackedFont::GetRequireBufferSize(const void* pResource, const char* pName,
                                     float scale) {
    if (!IsValidResource(pResource, 0x4000)) {
        return 0;
    }

    const PackedFontResource* pRes =
        static_cast<const PackedFontResource*>(pResource);
    const u8* pInfo = static_cast<const u8*>(pResource) + 0x10;
    FontGlyphGroupsAcs groups;
    int count;

    // Header field reads are inlined into the offset/stride expressions;
    // MWCC CSEs the repeated reads into single lhz loads.
    u32 off1 = (pRes->field_0x1E * 2 + 0x29) & ~3u;
    int qB = pRes->field_0x20 + 31;
    u32 strideB = (qB / 32) * 4;
    u32 off2 = (off1 + pRes->field_0x20 * 4 + 3) & ~3u;
    u32 strideC = ((pRes->field_0x22 + 31) / 32) * 4;
    u32 off3 = (off2 + pRes->field_0x22 * 4 + 3) & ~3u;
    int qD = pRes->field_0x24 + 31;
    u32 strideD = (qD / 32) * 4;
    u32 off4 = (off3 + pRes->field_0x24 * 4 + 3) & ~3u;
    u32 off5 = (off4 + strideB * pRes->field_0x1E + 3) & ~3u;
    u32 off6 = (off5 + strideC * pRes->field_0x1E + 3) & ~3u;
    const u8* p = static_cast<const u8*>(pResource);
    groups.field_0x0 = p;
    groups.field_0x4 = pInfo;
    groups.field_0xC = reinterpret_cast<const u32*>(off1 + p);
    groups.field_0x10 = reinterpret_cast<const u32*>(off2 + p);
    groups.field_0x14 = reinterpret_cast<const u32*>(off3 + p);
    groups.field_0x18 = off4 + p;
    groups.field_0x1C = off5 + p;
    groups.field_0x20 = off6 + p;
    groups.field_0x24 = strideB;
    groups.field_0x28 = strideC;
    groups.field_0x2C = strideD;

    u32 base = CalcCopySize(groups, pName, &count);

    // Reserve space for `scale`-scaled glyph count (clamped to [1, B]) plus
    // the per-glyph tables. B and the sheet image size are re-read through
    // the groups struct (retail keeps only pName/groups live across call).
    F64Conv conv;
    conv.w[0] = 0x43300000;
    conv.w[1] = (u32)count ^ 0x80000000;
    int n = (int)((float)(conv.d - lbl_eu_8066A138) * scale);
    const GlyphGroupsBlock* pBlock =
        reinterpret_cast<const GlyphGroupsBlock*>(groups.field_0x4);
    int glyphTotal = pBlock->groupBCount;
    u32 imageSize = pBlock->imageSize;
    if (n > glyphTotal) {
        n = glyphTotal;
    } else if (n < 1) {
        n = 1;
    }

    u32 w = (u32)n;
    u32 offA = ((w + 1) * 4 + 3) & ~3;
    u32 offB = (glyphTotal * 4 + offA + 1) & ~1;
    u32 offC = (glyphTotal * 4 + offB + 1) & ~1;
    u32 offD = (w * 2 + offC + 31) & ~31;
    return base + ((imageSize * w + offD + 3) & ~3);
}

/******************************************************************************
 *
 * StreamingConstruct
 *
 ******************************************************************************/

// The streaming construct machine: the context holds the stream reader and the
// next op code; each ConstructOp* step either returns a status (1 = done,
// 2 = error, 3 = continue) or requests more data.
int PackedFont::StreamingConstruct(ConstructContext* pCtx, const void* pArchive,
                                   u32 size) {
    if (mFontInfo != 0) {
        // Already constructed: succeed if the stream was fully consumed.
        int result = 1;
        if (pCtx->field_0x5C < pCtx->field_0x58) {
            result = 2;
        }
        return result;
    }

    int status = 3;
    CachedStreamReader* pReader = &pCtx->mReader;
    pReader->Attach(pArchive, size);
    while (status == 3) {
        switch (pCtx->field_0xC) {
        case 0:
            status = ConstructOpDispatch(pCtx, pReader);
            break;
        case 1:
            status = ConstructOpAnalyzeFileHeader(pCtx, pReader);
            break;
        case 2:
            status = ConstructOpAnalyzeGLGRPacked(pCtx, pReader);
            break;
        case 3:
            status = ConstructOpAnalyzeFINF(pCtx, pReader);
            break;
        case 4:
            status = ConstructOpAnalyzeCMAP(pCtx, pReader);
            break;
        case 5:
            status = ConstructOpAnalyzeCWDH(pCtx, pReader);
            break;
        case 6:
            status = ConstructOpCopy(pCtx, pReader);
            break;
        case 7:
            status = ConstructOpSkip(pCtx, pReader);
            break;
        case 8:
            status = ConstructOpFatalError(pCtx, pReader);
            break;
        case 9:
            status = ConstructOpAnalyzeTGLP(pCtx, pReader);
            break;
        case 10:
            status = ConstructOpPrepairCopyPackedSheet(pCtx, pReader);
            // Signed compare: retail emits cmpwi here (int-typed check).
            if ((int)pCtx->field_0xC == 9) {
                reinterpret_cast<u32*>(field_0x34)[field_0x28] =
                    pCtx->field_0x4C;
                field_0x28++;
            }
            break;
        case 11:
            pCtx->field_0xC = 0xC;
            return 2;
        case 12:
        default:
            pCtx->field_0xC = 0xC;
            return 2;
        }
    }

    // Construction finished: flush the built font image, install the resource
    // buffer and reset the per-glyph index tables.
    if (status == 1 && mFontInfo == 0) {
        DCFlushRange(reinterpret_cast<void*>(pCtx->field_0x44),
                     pCtx->field_0x48 - pCtx->field_0x44);
        SetResourceBuffer(reinterpret_cast<void*>(pCtx->field_0x44),
                          reinterpret_cast<FontInformation*>(pCtx->field_0x0),
                          reinterpret_cast<u16*>(pCtx->field_0x40));

        if (AdjustIndex(mFontInfo->alterCharIndex) == 0xFFFF) {
            mFontInfo->alterCharIndex = 0;
        }

        // Base pointers are NOT cached: retail reloads field_0x2C/0x30 every
        // iteration (sthx against a fresh lwz), so index the members directly.
        for (int i = 0; i < field_0x28; i++) {
            reinterpret_cast<u16*>(field_0x2C)[i] = 0xFFFF;
        }
        for (int i = 0; i < field_0x2A; i++) {
            reinterpret_cast<u16*>(field_0x30)[i] = 0xFFFF;
        }

        // Virtual GetEncoding() dispatch (slot 22 / offset 0x58) via the
        // call-side vtable view.
        InitReaderFunc(reinterpret_cast<const FontVTable*>(this)->GetEncoding());
    }
    return status;
}

/******************************************************************************
 *
 * GetGlyph
 *
 ******************************************************************************/

void PackedFont::GetGlyph(Glyph* pGlyph, u16 charCode) const {
    // Resolve the char code to a packed glyph index; if the sheet it maps to
    // is not resident yet, fall back to the alternate char's index.
    u16 index = GetGlyphIndex(charCode);
    u16 adjusted = AdjustIndex(index);
    if (adjusted == 0xFFFF) {
        index = mFontInfo->alterCharIndex;
        adjusted = AdjustIndex(index);
    }

    int sheet =
        adjusted / (mFontInfo->pGlyph->sheetRow * mFontInfo->pGlyph->sheetLine);
    u16 slot = reinterpret_cast<u16*>(field_0x2C)[sheet];

    // The requested sheet is not resident: take the head of the free list,
    // decompress its image into the slot and update both index tables.
    if (slot == 0xFFFF) {
        slot = reinterpret_cast<u16*>(mBuffer)[mSheetCount * 2];
        int oldSheet = reinterpret_cast<u16*>(field_0x30)[slot];
        const u8* pCompressed = reinterpret_cast<const u8*>(
            reinterpret_cast<const u32*>(field_0x34)[sheet]);
        u8* pImage = reinterpret_cast<u8*>(
            field_0x38 + slot * mFontInfo->pGlyph->sheetSize);

        if (oldSheet != 0xFFFF) {
            GXDrawDone();
            u16 oldSlot = reinterpret_cast<u16*>(field_0x2C)[oldSheet];
            reinterpret_cast<u16*>(field_0x2C)[oldSheet] = 0xFFFF;
            reinterpret_cast<u16*>(field_0x30)[oldSlot] = 0xFFFF;
        }

        CXUncompressHuffman(pCompressed, pImage);
        reinterpret_cast<u16*>(field_0x2C)[sheet] = slot;
        reinterpret_cast<u16*>(field_0x30)[slot] = (u16)sheet;
        GXInvalidateTexAll();
        DCFlushRange(pImage, CXGetUncompressedSize(pCompressed));
    }

    // Rotate the consumed slot to the tail of the sheet list (round-robin).
    u16 next = reinterpret_cast<u16*>(mBuffer)[slot * 2];    if (next != 0xFFFF) {
        u16 prev = reinterpret_cast<u16*>(mBuffer)[slot * 2 + 1];
        reinterpret_cast<u16*>(mBuffer)[next * 2 + 1] = prev;
        reinterpret_cast<u16*>(mBuffer)[prev * 2] = next;
        u16 tail = reinterpret_cast<u16*>(mBuffer)[mSheetCount * 2 + 1];
        reinterpret_cast<u16*>(mBuffer)[slot * 2 + 1] = tail;
        reinterpret_cast<u16*>(mBuffer)[slot * 2] =
            reinterpret_cast<u16*>(mBuffer)[tail * 2];
        reinterpret_cast<u16*>(mBuffer)[mSheetCount * 2 + 1] = slot;
        reinterpret_cast<u16*>(mBuffer)[tail * 2] = slot;
    }

    MakeGlyph(pGlyph, index, slot);
}

/******************************************************************************
 *
 * CalcCopySize
 *
 ******************************************************************************/

u32 PackedFont::CalcCopySize(const FontGlyphGroupsAcs& rGroups,
                             const char* pName, int* pResult) {
    u32 size1 = 0;  // B-group sizes
    u32 size2 = 0;  // C-group sizes
    u32 size3 = 0;  // D-group sizes
    u32 count = 0;  // included glyph count

    // B groups: add the per-glyph size of every sheet that is included.
    {
        u32 offMask = 0;
        int g = 0;
        u32 offSize = 0;
        for (; g < reinterpret_cast<const GlyphGroupsBlock*>(rGroups.field_0x4)
                    ->groupBCount;
             g += 0x20) {
            u32 maskB = 0;
            for (int i = 0;
                 i < reinterpret_cast<const GlyphGroupsBlock*>(
                         rGroups.field_0x4)
                         ->glyphsPerGroup;
                 i++) {
                const char* pSheet = reinterpret_cast<const char*>(
                    rGroups.field_0x0 +
                    reinterpret_cast<const GlyphGroupsBlock*>(rGroups.field_0x4)
                        ->nameIndices[i]);
                if (*pName == 0 || IncludeName(pName, pSheet)) {
                    maskB |= *(const u32*)(rGroups.field_0x18 + offMask +
                                           ((i * rGroups.field_0x24) & ~3));
                }
            }
            for (int b = 0; b < 32; b++) {
                if ((maskB << b) & 0x80000000u) {
                    count++;
                    size1 += *(const u32*)(
                        (const u8*)rGroups.field_0xC + offSize + b * 4);
                }
            }
            offSize += 0x80;
            offMask += 4;
        }
    }

    // C groups: each included glyph shares its 8-byte header with the sheet.
    {
        u32 offMask = 0;
        int g = 0;
        u32 offSize = 0;
        for (; g < reinterpret_cast<const GlyphGroupsBlock*>(rGroups.field_0x4)
                    ->groupCCount;
             g += 0x20) {
            u32 mask = 0;
            for (int i = 0;
                 i < reinterpret_cast<const GlyphGroupsBlock*>(
                         rGroups.field_0x4)
                         ->glyphsPerGroup;
                 i++) {
                const char* pSheet = reinterpret_cast<const char*>(
                    rGroups.field_0x0 +
                    reinterpret_cast<const GlyphGroupsBlock*>(rGroups.field_0x4)
                        ->nameIndices[i]);
                if (*pName == 0 || IncludeName(pName, pSheet)) {
                    mask |= *(const u32*)(rGroups.field_0x1C + offMask +
                                          ((i * rGroups.field_0x28) & ~3));
                }
            }
            for (int b = 0; b < 32; b++) {
                if ((mask << b) & 0x80000000u) {
                    size2 += *(const u32*)(
                        (const u8*)rGroups.field_0x10 + offSize + b * 4) -
                             8;
                }
            }
            offSize += 0x80;
            offMask += 4;
        }
    }

    // D groups: same header sharing as the C pass.
    {
        u32 offMask = 0;
        int g = 0;
        u32 offSize = 0;
        for (; g < reinterpret_cast<const GlyphGroupsBlock*>(rGroups.field_0x4)
                    ->groupDCount;
             g += 0x20) {
            u32 mask = 0;
            for (int i = 0;
                 i < reinterpret_cast<const GlyphGroupsBlock*>(
                         rGroups.field_0x4)
                         ->glyphsPerGroup;
                 i++) {
                const char* pSheet = reinterpret_cast<const char*>(
                    rGroups.field_0x0 +
                    reinterpret_cast<const GlyphGroupsBlock*>(rGroups.field_0x4)
                        ->nameIndices[i]);
                if (*pName == 0 || IncludeName(pName, pSheet)) {
                    mask |= *(const u32*)(rGroups.field_0x20 + offMask +
                                          ((i * rGroups.field_0x2C) & ~3));
                }
            }
            for (int b = 0; b < 32; b++) {
                if ((mask << b) & 0x80000000u) {
                    size3 += *(const u32*)(
                        (const u8*)rGroups.field_0x14 + offSize + b * 4) -
                             8;
                }
            }
            offSize += 0x80;
            offMask += 4;
        }
    }

    if (pResult != 0) {
        *pResult = (int)count;
    }

    u32 total = size1 + size2 + size3 + 0x40;
    u32 min = (reinterpret_cast<const GlyphGroupsBlock*>(rGroups.field_0x4)
                   ->size +
               0x10) *
              2;
    return total >= min ? total : min;
}

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

    // Unsigned wrap check: underflow means the buffer cannot even hold the
    // fixed tables.
    u32 remaining = bufferSize - offA;
    remaining -= offB;
    remaining -= offC;
    remaining -= 4;
    if (remaining > bufferSize) {
        return NULL;
    }

    // In-place divide keeps the quotient in the dividend's register.
    remaining /= imageSize + 6;
    int count = remaining;
    field_0x26 = 0;
    mBuffer = pBuffer;
    mSheetCount = (u16)count;

    u32 count2 = count * 2;
    u32 offList = ((count + 1) * 4 + 1) & ~1u;
    u32 offMap = (offList + offA + 1) & ~1u;
    u32 offGlyphTable = (offMap + offB + 1) & ~1u;
    u32 offGlyph = (offGlyphTable + count2 + 3) & ~3u;
    // Reuses the rounded offC, not raw glyphNum*4 (matches retail reg reuse).
    u32 offImage = (offGlyph + offC + 31) & ~31u;

    // Doubly-linked sheet chain: entry i points to i+1 (last entry terminates
    // with 0) and back to i-1 (first entry wraps to `count`).
    for (u32 i = 0; i < count; i++) {
        u32 prev = i != 0 ? i - 1 : count;
        ((u16*)mBuffer)[i * 2] = (u16)(i + 1);
        ((u16*)mBuffer)[i * 2 + 1] = (u16)prev;
    }

    // Terminate the chain head/tail and install the layout pointers.
    u16* pEntry = reinterpret_cast<u16*>(mBuffer) + mSheetCount * 2;
    pEntry[0] = 0;
    pEntry[1] = (u16)(count - 1);
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

    u32 texAddr = field_0x38 + charIndex * pSheet->sheetSize;
    u32 cw = pSheet->cellWidth + 1;
    u32 ch = pSheet->cellHeight + 1;
    u32 cellsInSheet = pSheet->sheetRow * pSheet->sheetLine;

    u32 cell = charCode % cellsInSheet;
    u32 row = cell / pSheet->sheetRow;
    u32 col = cell % pSheet->sheetRow;
    u32 cellX = col * cw;
    u32 cellY = row * ch;

    pGlyph->pTexture = (void*)texAddr;

    const CharWidths& widths = GetCharWidthsFromIndex(charCode);
    pGlyph->widths.left = widths.left;
    pGlyph->widths.glyphWidth = widths.glyphWidth;
    pGlyph->widths.charWidth = widths.charWidth;

    pGlyph->height = pSheet->cellHeight;
    pGlyph->texFormat = pSheet->sheetFormat;
    pGlyph->texWidth = pSheet->sheetWidth;
    pGlyph->texHeight = pSheet->sheetHeight;
    pGlyph->cellX = (u16)(cellX + 1);
    pGlyph->cellY = (u16)(cellY + 1);
}

/******************************************************************************
 *
 * ConstructOpAnalyzeGLGRPacked
 *
 ******************************************************************************/

// GLGR block analysis for packed fonts: validates the block, builds the
// glyph-group summary, marks the glyphs belonging to included sheets, then
// AssignMemory lays out the resident glyph map and sheet images.
int PackedFont::ConstructOpAnalyzeGLGRPacked(ConstructContext* pCtx,
                                             CachedStreamReader* pReader) {
    u32 blockSize = pCtx->field_0x14;
    u32 need = blockSize - 8;

    if (*reinterpret_cast<const u32*>(
            reinterpret_cast<const void*>(pCtx->field_0x4C)) !=
        0x52464E41 /* 'RFNA' */) {
        return 2;
    }

    if (pReader->GetRemain() < need) {
        return RequestData(pCtx, pReader, need);
    }

    if (pCtx->field_0x48 - pCtx->field_0x4C < blockSize + 0x10) {
        return 2;
    }

    const u8* pCursor = reinterpret_cast<const u8*>(pCtx->field_0x4C);
    u8* pBlockRaw = const_cast<u8*>(pCursor + 0x10);

    memcpy(pBlockRaw, &pCtx->field_0x10, 8);
    pReader->CopyTo(pBlockRaw + 8, need);
    if (!IsValidResource(pCursor, blockSize + 0x10)) {
        return 2;
    }

    const GlyphGroupsBlock* pBlock =
        reinterpret_cast<const GlyphGroupsBlock*>(pBlockRaw);

    // Summarise the glyph-group tables (same layout GetRequireBufferSize
    // computes): glyphs per group (A) and the three group counts B/C/D.
    FontGlyphGroupsAcs groups;
    groups.field_0x0 = pCursor;
    groups.field_0x4 = pBlockRaw;

    u32 avail = pCtx->field_0x48 - pCtx->field_0x4C;
    u32 bufferEnd = (u32)pCursor + avail;

    int bCnt = pBlock->groupBCount;
    u32 strideB = ((bCnt + 31) / 32) * 4;
    groups.field_0x24 = strideB;
    int cCnt = pBlock->groupCCount;
    u32 strideC = ((cCnt + 31) / 32) * 4;
    groups.field_0x28 = strideC;
    int dCnt = pBlock->groupDCount;
    u32 strideD = ((dCnt + 31) / 32) * 4;
    groups.field_0x2C = strideD;

    u16 grpA = pBlock->glyphsPerGroup;
    u16 grpB = pBlock->groupBCount;
    u16 grpC = pBlock->groupCCount;
    u16 grpD = pBlock->groupDCount;

    u32 off1 = (grpA * 2 + 0x29) & ~3u;
    groups.field_0xC = reinterpret_cast<const u32*>(pCursor + off1);
    u32 off2 = (off1 + grpB * 4 + 3) & ~3u;
    groups.field_0x10 = reinterpret_cast<const u32*>(pCursor + off2);
    u32 off3 = (off2 + grpC * 4 + 3) & ~3u;
    groups.field_0x14 = reinterpret_cast<const u32*>(pCursor + off3);
    u32 off4 = (off3 + grpD * 4 + 3) & ~3u;
    groups.field_0x18 = pCursor + off4;
    u32 off5 = (off4 + strideB * grpA + 3) & ~3u;
    groups.field_0x1C = pCursor + off5;
    u32 off6 = (off5 + strideC * grpA + 3) & ~3u;
    groups.field_0x20 = pCursor + off6;

    // Room for the block plus the resident glyph map?
    u16 mapCount = pBlock->groupBCount;
    u16 extra = pBlock->field_0xC;
    u16 mapSkip = *reinterpret_cast<const u16*>(pCursor + 0xE);
    u32 imageSize = pBlock->imageSize;
    if (avail < ((mapCount * 2 + 3) & ~3u) + blockSize) {
        return 2;
    }

    // Zero the glyph->sheet map (B u16 entries) above the streamed data.
    u16* pGlyphMap = reinterpret_cast<u16*>(
        (bufferEnd - ((mapCount * 2 + 3) & ~3u)) & ~1u);
    for (int i = 0; i < mapCount; i++) {
        pGlyphMap[i] = 0;
    }

    // Mark every glyph whose sheet is included by name.
    u32 count = 0;
    for (int g = 0;
         g < reinterpret_cast<const GlyphGroupsBlock*>(groups.field_0x4)
                 ->glyphsPerGroup;
         g++) {
        const char* pSheet = reinterpret_cast<const char*>(
            groups.field_0x0 +
            reinterpret_cast<const GlyphGroupsBlock*>(groups.field_0x4)
                ->nameIndices[g]);
        if (*reinterpret_cast<const char*>(pCtx->field_0x3C) != 0 &&
            !IncludeName(reinterpret_cast<const char*>(pCtx->field_0x3C),
                         pSheet)) {
            continue;
        }
        for (int b = 0; b < mapCount; b++) {
            u32 bitPos = b + g * groups.field_0x24 * 8;
            u32 word = *reinterpret_cast<const u32*>(groups.field_0x18 +
                                                     (bitPos & ~7u));
            if ((word << (bitPos & 31)) & 0x80000000u) {
                pGlyphMap[b] = 1;
            }
        }
    }

    // Assign sequential indices to the used glyphs; unused runs are skipped
    // by the extra count stored in the resource.
    u16 nextIndex = 0;
    for (int i = 0; i < mapCount; i++) {
        if (pGlyphMap[i] == 1) {
            pGlyphMap[i] = nextIndex;
            count++;
        } else {
            pGlyphMap[i] = 0xFFFF;
            nextIndex += reinterpret_cast<const GlyphGroupsBlock*>(
                            groups.field_0x4)
                            ->field_0xC;
        }
    }

    int result = 0;
    u32 copySize = CalcCopySize(
        groups, reinterpret_cast<const char*>(pCtx->field_0x3C), &result);
    if (copySize >= avail) {
        return 2;
    }
    u32 remain = (avail - copySize) & ~3u;

    u8* pAssigned =
        AssignMemory(const_cast<u8*>(pCursor), remain, mapCount, count,
                     imageSize);
    if (pAssigned == 0) {
        return 2;
    }

    memmove(pAssigned, pGlyphMap, mapCount * 2);

    pCtx->field_0x40 = reinterpret_cast<u32>(pAssigned);
    pCtx->field_0x4C += remain;
    pCtx->field_0x62 = mapCount;
    pCtx->field_0x64 = extra;
    pCtx->field_0x58 = mapSkip;
    pCtx->field_0xC = 0;
    return 3;
}

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

    // Materialize a register copy: 'data' is stack-resident because its
    // address escapes to CopyTo, so every direct use would reload from
    // memory after the ctx stores below.
    u32 needed = data;

    // The next sheet's code-map entry decides how its image data is stored:
    // 0xFFFF means "not copied" (the sheet data stays in the archive).
    if (reinterpret_cast<const u16*>(pCtx->field_0x40)[pCtx->field_0x60] != 0xFFFF) {
        if (pCtx->field_0x48 - pCtx->field_0x4C < needed) {
            return 2;
        }
        pCtx->field_0x54 = needed;
        pCtx->field_0xC = 9;
        pCtx->field_0x50 = 8;
    } else {
        pCtx->field_0x54 = needed;
        pCtx->field_0xC = 10;
        pCtx->field_0x50 = 8;
    }

    pCtx->field_0x60++;
    return 3;
}

} // namespace ut
} // namespace nw4r
