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

// ResFontBase private helpers, called across the TU boundary by the retail
// build (UNDEF relocs in the DOL-extracted object).
extern "C" unsigned short
GetGlyphIndex__Q44nw4r2ut6detail11ResFontBaseCFUs(const void* self,
                                                  unsigned short ch);
extern "C" unsigned short
FindGlyphIndex__Q44nw4r2ut6detail11ResFontBaseCFUs(const void* self,
                                                   unsigned short ch);
extern "C" const void*
GetCharWidthsFromIndex__Q44nw4r2ut6detail11ResFontBaseCFUs(const void* self,
                                                           unsigned short index);

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

    static bool IncludeName(const char* pList, const char* pName);

    virtual bool HasGlyph(u16 ch) const; // at 0x54

    virtual CharWidths GetCharWidths(u16 ch) const; // at 0x4C

    // Inlined width-sheet remap shared by the index-adjust path.
    // ResFontBase::mFontInfo (at 0x14) is private to the base class.
    u16 GetAdjustedIndex(u16 index) const {
        const FontTextureGlyph* pGlyph =
            (*reinterpret_cast<FontInformation* const*>(
                 reinterpret_cast<const char*>(this) + 0x14))
                ->pGlyph;

        int cellsInASheet = pGlyph->sheetRow * pGlyph->sheetLine;

        u16 mapped = mWidth[index / cellsInASheet];
        return (mapped == 0xFFFF) ? 0xFFFF : (u16)(index - mapped);
    }

    int RequestData(ConstructContext* pCtx, CachedStreamReader* pReader, u32 size);

    static int ConstructOpCopy(ConstructContext* pCtx,
                               CachedStreamReader* pReader);

    static int ConstructOpSkip(ConstructContext* pCtx,
                               CachedStreamReader* pReader);

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

// dont_inline: retail's PackedFont ctor CALLS this base ctor (bl
// __ct__ArchiveFontBase) rather than letting -ipa inline+DSE it away when the
// derived vptr overwrites (CScnBloom.cpp pattern).
#pragma dont_inline on
ArchiveFontBase::ArchiveFontBase() : mWidth(NULL) {
    *(void**)this = (void*)lbl_eu_8056AFF0;
}
#pragma dont_inline reset

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
    const BinaryFileHeader* pHeader =
        static_cast<const BinaryFileHeader*>(pResource);

    if (!IsValidBinaryFile(pHeader, 0x52464E41 /* 'RFNA' */,
                           NW4R_VERSION(1, 4), 2)) {
        return false;
    }

    // First block must be the glyph block ('GLGR'); its header lives at +0x10.
    const BinaryBlockHeader* pBlock =
        reinterpret_cast<const BinaryBlockHeader*>(pHeader + 1);

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
    // The volatile qualifier pins the buffer-field loads at this point in
    // the schedule (volatiles cannot be reordered); retail emits the stream
    // diff first, then the buffer diff.
    u32 streamDiff = mStreamEnd - mStreamPos;
    u32 remain = streamDiff +
                 (*reinterpret_cast<volatile u32*>(&mBufferEnd) -
                  *reinterpret_cast<volatile u32*>(&mBufferPos));

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

    // Retail recomputes the buffered count here (no CSE across the capacity
    // check), so force the reload with volatile reads.
    u32 avail = *reinterpret_cast<volatile u32*>(&mBufferEnd) -
                *reinterpret_cast<volatile u32*>(&mBufferPos);
    u8* pDst = reinterpret_cast<u8*>(pCtx->field_0x4C) + size;
    if (avail >= remain) {
        memmove(pDst, mBufferPos, remain);
        mBufferPos += remain;
    } else {
        // Retail computes the remainder length between the two memmoves;
        // keep it there to preserve the short live range.
        memmove(pDst, mBufferPos, avail);
        u32 remaining = remain - avail;
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

    // Need at least an 8-byte block header available across the stream and
    // the font buffer.
    // Diffs are unsigned (retail compares with cmpli).
    if ((u32)(pReader->mStreamEnd - pReader->mStreamPos) +
            (u32)(pReader->mBufferEnd - pReader->mBufferPos) <
        0x8) {
        // Accumulate consumed bytes on the aliased font object, then pull in
        // one more chunk.
        pCtx->field_0x18 = pCtx->field_0x18 +
                           (pReader->mStreamPos - pReader->mStreamStart) +
                           (pReader->mBufferPos - pReader->mBufferStart);
        int result = pReader->RequestData(pCtx, 0x8);
        int ret = 2;
        if (result != 0) {
            ret = 0;
        }
        return ret;
    }

    // Copy the block header (kind + size) into the context at +0x10.
    // Retail recomputes the buffered count here (no CSE across the capacity
    // check), so force the reload with volatile reads.
    int remaining;
    void* pDst;
    u32 avail;

    u32 bufferedPos = *reinterpret_cast<volatile u32*>(&pReader->mBufferPos);
    avail = *reinterpret_cast<volatile u32*>(&pReader->mBufferEnd) - bufferedPos;
    pDst = &pCtx->field_0x10;
    if (avail >= 0x8) {
        memcpy(pDst, reinterpret_cast<u8*>(bufferedPos), 0x8);
        pReader->mBufferPos += 0x8;
    } else {
        // Wrap: consume the buffer tail, then the rest from the stream.
        remaining = 0x8 - avail;
        memcpy(pDst, reinterpret_cast<u8*>(bufferedPos), avail);
        memcpy(static_cast<u8*>(pDst) + avail, pReader->mStreamPos,
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
    // NOTE: retail's compare literal for this block kind is 0x43574448 (not
    // the ASCII 'CWHD' = 0x43574844); kept byte-identical to retail.
    case 0x43574448:
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
    // Need at least a full 0x10-byte block header available across the stream
    // and the font buffer. Diffs are unsigned (retail compares with cmplwi).
    u8* bufferPos = pReader->mBufferPos;
    u8* bufferEnd = pReader->mBufferEnd;
    u8* streamPos = pReader->mStreamPos;
    u8* streamEnd = pReader->mStreamEnd;

    if ((u32)(streamEnd - streamPos) + (u32)(bufferEnd - bufferPos) < 0x10) {
        // Accumulate the total consumed-byte count on the aliased font
        // object (both cursor offsets measured from their buffer starts).
        u32 total = pCtx->field_0x18 +
                    (u32)(streamPos - pReader->mStreamStart);
        pCtx->field_0x18 =
            (u32)(bufferPos - pReader->mBufferStart) + total;
        int result = pReader->RequestData(pCtx, 0x10);
        int ret = 2;
        if (result != 0) {
            ret = 0;
        }
        return ret;
    }

    if (pCtx->field_0x48 - pCtx->field_0x4C < 0x10) {
        return 2;
    }

    // Retail recomputes the buffered count here (fresh loads past the branch).
    // Declaration order drives callee-saved coloring: remaining -> r29,
    // pDst -> r28, avail -> r27 (retail).
    int remaining;
    void* pDst;
    u32 avail;

    u32 bufferedPos =
        *reinterpret_cast<volatile u32*>(&pReader->mBufferPos);
    avail = *reinterpret_cast<volatile u32*>(&pReader->mBufferEnd) -
            bufferedPos;
    pDst = reinterpret_cast<void*>(
        *reinterpret_cast<volatile u32*>(&pCtx->field_0x4C));
    if (avail >= 0x10) {
        memcpy(pDst, reinterpret_cast<u8*>(bufferedPos), 0x10);
        pReader->mBufferPos += 0x10;
    } else {
        // Wrap: consume the buffer tail, then the rest from the stream.
        remaining = 0x10 - avail;
        memcpy(pDst, reinterpret_cast<u8*>(bufferedPos), avail);
        memcpy(static_cast<u8*>(pDst) + avail, pReader->mStreamPos,
               remaining);
        pReader->mBufferPos = pReader->mBufferEnd;
        pReader->mStreamPos += remaining;
    }

    pCtx->field_0xC = 0;
    return 3;
}

int ArchiveFontBase::ConstructOpAnalyzeTGLP(ConstructContext* pCtx,
                                            CachedStreamReader* pReader) {
    // Need at least a full 0x18-byte TGLP header available across the stream
    // and the font buffer. Diffs are unsigned (retail compares with cmplwi).
    if ((u32)(pReader->mStreamEnd - pReader->mStreamPos) +
            (u32)(pReader->mBufferEnd - pReader->mBufferPos) <
        0x18) {
        // Accumulate consumed bytes on the aliased font object, then pull in
        // one more chunk.
        pCtx->field_0x18 = pCtx->field_0x18 +
                           (pReader->mStreamPos - pReader->mStreamStart) +
                           (pReader->mBufferPos - pReader->mBufferStart);
        int result = pReader->RequestData(pCtx, 0x18);
        int ret = 2;
        if (result != 0) {
            ret = 0;
        }
        return ret;
    }

    if (pCtx->field_0x48 - pCtx->field_0x4C < 0x18) {
        return 2;
    }

    // The context's first word aliases the FontInformation of the font being
    // built: point its glyph sheet at the current write position.
    // Retail reloads the write position past the capacity check (no CSE).
    reinterpret_cast<FontInformation*>(pCtx->field_0x0)->pGlyph =
        reinterpret_cast<FontTextureGlyph*>(
            *reinterpret_cast<volatile u32*>(&pCtx->field_0x4C));

    // Retail recomputes the buffered count here (fresh loads past the branch).
    // Declaration order drives callee-saved coloring: avail -> r29,
    // pDst -> r28, remaining -> r27 (retail).
    u32 avail;
    void* pDst;
    u32 remaining;

    u32 bufferedPos =
        *reinterpret_cast<volatile u32*>(&pReader->mBufferPos);
    avail = *reinterpret_cast<volatile u32*>(&pReader->mBufferEnd) -
            bufferedPos;
    pDst = reinterpret_cast<void*>(pCtx->field_0x4C);
    if (avail >= 0x18) {
        memcpy(pDst, reinterpret_cast<const u8*>(bufferedPos), 0x18);
        pReader->mBufferPos += 0x18;
    } else {
        // Wrap: consume the buffer tail, then the rest from the stream.
        remaining = 0x18 - avail;
        memcpy(pDst, reinterpret_cast<const u8*>(bufferedPos), avail);
        memcpy(static_cast<u8*>(pDst) + avail, pReader->mStreamPos,
               remaining);
        pReader->mBufferPos = pReader->mBufferEnd;
        pReader->mStreamPos += remaining;
    }

    // Count init precedes the write-cursor advance in retail's schedule;
    // pInfo is read BEFORE the advance store, pGlyph AFTER it.
    u16 count = 0;

    FontInformation* pInfo =
        reinterpret_cast<FontInformation*>(pCtx->field_0x0);
    u32 pos = pCtx->field_0x4C;
    pCtx->field_0x4C = pos + 0x18;

    // TGLP header: clear the 'reverse' flag in sheetFormat and remember it.
    FontTextureGlyph* pGlyph = pInfo->pGlyph;
    u16 sheetFormat = pGlyph->sheetFormat;
    u32 reverse = (sheetFormat >> 15) & 1;

    for (u16 i = 0; i < pCtx->field_0x62; i++) {
        if (*reinterpret_cast<const u16*>(
                pCtx->field_0x40 + i * sizeof(u16)) != 0xFFFF) {
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

    // Next op depends on the 'reverse' flag (branchy default + overwrite).
    u32 opNext = 7;
    if (reverse != 0) {
        opNext = 8;
    }

    pCtx->field_0xC = 0xA;
    pCtx->field_0x50 = opNext;
    pCtx->field_0x54 =
        image -
        (pCtx->field_0x18 + (pReader->mStreamPos - pReader->mStreamStart) +
         (pReader->mBufferPos - pReader->mBufferStart));
    return 3;
}

} // namespace detail
} // namespace ut
} // namespace nw4r

namespace nw4r {
namespace ut {
namespace detail {

// ArchiveFontBase::GetCharWidths(u16) const - remaps the glyph index through
// the archive width sheet (inlined AdjustIndex: an unmapped sheet yields
// 0xFFFF), falls back to the alternate char, then forwards to
// ResFontBase::GetCharWidthsFromIndex.
CharWidths ArchiveFontBase::GetCharWidths(unsigned short ch) const {
    unsigned short index = GetGlyphIndex__Q44nw4r2ut6detail11ResFontBaseCFUs(this, ch);
    unsigned short charIndex = index;

    // Inlined AdjustIndex: remap through the per-sheet width table; an
    // unmapped (non-resident) sheet yields 0xFFFF.
    unsigned short remapped = GetAdjustedIndex(index);

    // ResFontBase::mFontInfo (at 0x14) is private to the base class.
    FontInformation* pInfo = *reinterpret_cast<FontInformation* const*>(
        reinterpret_cast<const char*>(this) + 0x14);

    // Fall back to the alternate char when the glyph's sheet is not resident;
    // otherwise use the original index.
    if (remapped == 0xFFFF) {
        charIndex = pInfo->alterCharIndex;
    }
    return *reinterpret_cast<const CharWidths*>(
        GetCharWidthsFromIndex__Q44nw4r2ut6detail11ResFontBaseCFUs(
            this, charIndex));
}

} // namespace detail
} // namespace ut
} // namespace nw4r
// Sheet-remap of a glyph index through the archive width table; returns
// 0xffff when the glyph's sheet is not resident.
static inline unsigned short
AdjustSheetIndex_(const unsigned char* pSelf, unsigned short index) {
    const void* pInfo = *(const void**)(pSelf + 0x14);
    const unsigned short* pTable = *(const unsigned short**)(pSelf + 0x1c);
    const unsigned char* pSub =
        *(const unsigned char**)((const unsigned char*)pInfo + 8);

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

// ArchiveFontBase::HasGlyph(u16) const - a glyph exists when its sheet-remapped
// index is valid.
bool nw4r::ut::detail::ArchiveFontBase::HasGlyph(unsigned short ch) const {
    const unsigned char* pSelf = reinterpret_cast<const unsigned char*>(this);
    unsigned short index = FindGlyphIndex__Q44nw4r2ut6detail11ResFontBaseCFUs(pSelf, ch);

    if (index != 0xffff) {
        return AdjustSheetIndex_(pSelf, index) != 0xffff;
    }

    return false;
}extern "C" unsigned short
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

// Returns true if pName appears in the comma-separated pList (entries may be
// padded with spaces on either side of the name).
bool nw4r::ut::detail::ArchiveFontBase::IncludeName(const char* pList, const char* pName) {
    u32 len = strlen(pName);
    const char* pIt = pList - 1;

    while (true) {
        pIt = strstr(pIt + 1, pName);
        if (pIt == NULL) {
            return false;
        }

        // The match must start at a list boundary: either the very start of
        // the list or right after a comma (skipping back over spaces).
        const char* pCheck = pIt;
        if (pCheck != pList) {
            pCheck--;
            while (pCheck > pList && *pCheck == ' ') {
                pCheck--;
            }
            if (*pCheck != ',') {
                continue;
            }
        }

        // The match must also end at a boundary: skip spaces after the name
        // and require the next character to be the entry terminator.
        const char* pEnd = strchr(pIt, ',');
        u32 entryLen =
            (pEnd != NULL) ? (u32)(pEnd - pIt) : strlen(pIt);
        const char* pAfter = pIt + len;
        const char* pStop = pIt + entryLen;
        while (pAfter < pStop && *pAfter == ' ') {
            pAfter++;
        }
        if (pAfter == pStop) {
            return true;
        }
    }
}

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

int nw4r::ut::detail::ArchiveFontBase::ConstructOpCopy(ConstructContext* pCtx,
                                                       CachedStreamReader* pReader) {
    // Copy as much as the stream cache holds, bounded by the op's remaining
    // byte count. The second clamp re-reads the remaining count like retail
    // (the compiler does not know the first clamp capped it).
    u32 amount = pReader->mStreamEnd - pReader->mStreamPos;
    if (amount > pCtx->field_0x54) {
        amount = pCtx->field_0x54;
    }

    memcpy((void*)pCtx->field_0x4C, pReader->mStreamPos, amount);
    pReader->mStreamPos += amount;
    pCtx->field_0x4C += amount;

    u32 remaining = *reinterpret_cast<volatile u32*>(&pCtx->field_0x54);
    if (amount > remaining) {
        amount = remaining;
    }
    pCtx->field_0x54 -= amount;

    if (pCtx->field_0x54 == 0) {
        pCtx->field_0xC = pCtx->field_0x50;
    } else {
        // Still more to copy: account consumed bytes and pull in more data.
        pCtx->field_0x18 += (pReader->mStreamPos - pReader->mStreamStart) +
                            (pReader->mBufferPos - pReader->mBufferStart);
        int result = pReader->RequestData(pCtx, pCtx->field_0x54);
        int ret = 2;
        if (result != 0) {
            ret = 0;
        }
        return ret;
    }
    return 3;
}

int nw4r::ut::detail::ArchiveFontBase::ConstructOpSkip(ConstructContext* pCtx,
                                                       CachedStreamReader* pReader) {
    // Skip up to what the stream cache holds, bounded by the op's remaining
    // byte count (field_0x54). Retail re-reads the remaining count around the
    // cursor updates, so each clamp/subtract performs its own reload.
    // Retail keeps the call's `this` in a separate copy that interferes with
    // the parameter (both stay live), so it survives as its own register web.
    CachedStreamReader* pStream = pReader;

    u32 avail = (pReader->mStreamEnd - pReader->mStreamPos) +
                (pReader->mBufferEnd - pReader->mBufferPos);
    u32 amount = pCtx->field_0x54;
    if (amount > avail) {
        amount = avail;
    }

    // Consume the buffered range first; once it is exhausted, spill the rest
    // of the skip into the stream itself.
    u32 buffered = pReader->mBufferEnd - pReader->mBufferPos;
    if (buffered > amount) {
        pReader->mBufferPos += amount;
    } else {
        pReader->mBufferPos = pReader->mBufferEnd;
        pReader->mStreamPos += amount - buffered;
    }

    u32 remaining = pCtx->field_0x54;
    if (amount > remaining) {
        amount = remaining;
    }

    u32 left = pCtx->field_0x54 - amount;
    pCtx->field_0x54 = left;

    if (left == 0) {
        pCtx->field_0xC = pCtx->field_0x50;
        return 3;
    }

    // Still more to skip: accumulate consumed bytes and pull in more data.
    pCtx->field_0x18 += (pReader->mStreamPos - pReader->mStreamStart) +
                        (pReader->mBufferPos - pReader->mBufferStart);
    int result = pStream->RequestData(pCtx, left);
    if (result == 0) {
        return 2;
    }
    return 0;
}

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

void nw4r::ut::detail::ArchiveFontBase::CachedStreamReader::CopyTo(
    void* pDst, u32 size) {
    // Fast path: requested range is fully inside the stream buffer.
    if (mBufferEnd - mBufferPos >= size) {
        memcpy(pDst, mBufferPos, size);
        mBufferPos += size;
    } else {
        // Wrap around: consume the buffer tail, then read the rest from the stream.
        u32 buffered = mBufferEnd - mBufferPos;
        u32 rest = size - buffered;
        memcpy(pDst, mBufferPos, buffered);
        memcpy(static_cast<u8*>(pDst) + buffered, mStreamPos, rest);
        mBufferPos = mBufferEnd;
        mStreamPos += rest;
    }
}

