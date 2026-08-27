#include <nw4r/lyt.h>
#include <nw4r/ut.h>

#include <cstring>
#include <cwchar>

// Shared nw4r data symbols referenced by retail.
extern MEMAllocator* lbl_eu_80665478; // Layout allocator (mspAllocator)
extern const f32 lbl_eu_80669D68;    // 0.0f
extern const f64 lbl_eu_80669D70;    // 0x4330000080000000 (signed int->f32 magic)
extern const f32 lbl_eu_80669D78;    // 0.5f
extern const f32 lbl_eu_80669D7C;    // 1.0f
extern wchar_t lbl_eu_806634C0[2]; // L"\n" literal pool (.sdata)

// int -> f32 conversion matching retail: build the 2^52+x double on the stack
// (high word = 0x43300000, low word = x ^ 0x80000000) and subtract the shared
// signed magic constant.
inline f32 ConvF32S(s32 v) {
    union { f64 d; u32 w[2]; } u;
    u.w[1] = (u32)v ^ 0x80000000;
    u.w[0] = 0x43300000;
    return (f32)(u.d - lbl_eu_80669D70);
}

/******************************************************************************
 *
 * Utility functions
 *
 ******************************************************************************/
namespace nw4r {
namespace lyt {
namespace {

inline u8 ClampColor(s16 value) {
    return value < 0 ? 0 : (value > 255 ? 255 : value);
}

ut::Color GetColor(const GXColorS10& rColor16) {
    GXColor color8 = {ClampColor(rColor16.r), ClampColor(rColor16.g),
                      ClampColor(rColor16.b), ClampColor(rColor16.a)};

    return ut::Color(color8);
}

// CalcStringRectImpl / CalcLineRectImpl: anonymous-namespace helpers (retail
// scope nw4r::lyt::@unnamed@lyt_textBox_cpp@).
template <typename T>
int CalcLineRectImpl(ut::Rect* pRect, ut::TextWriterBase<T>* pWriter,
                     const T* pStr, int len, f32 width, bool* pHasNextLine);

template <typename T>
void CalcStringRectImpl(ut::Rect* pRect, ut::TextWriterBase<T>* pWriter,
                        const T* pStr, int len, f32 width);
template <typename T>
int CalcLineRectImpl(ut::Rect* pRect, ut::TextWriterBase<T>* pWriter,
                     const T* pStr, int len, f32 width, bool* pHasNextLine) {
    ut::PrintContext<T> context = {pWriter, pStr};

    ut::CharStrmReader reader = pWriter->GetFont()->GetCharStrmReader();

    // Retail reloads the shared 0.0f constant around each call.
    pRect->left = lbl_eu_80669D68;
    pRect->right = lbl_eu_80669D68;

    f32 lineWidth = lbl_eu_80669D68;
    bool charSpace = false;
    // Initial stream position (NULL until reader.Set below).
    const T* pPrevStream = static_cast<const T*>(reader.GetCurrentPos());

    pRect->top = ut::Min(lbl_eu_80669D68, pWriter->GetLineHeight());
    pRect->bottom = ut::Max(lbl_eu_80669D68, pWriter->GetLineHeight());

    *pHasNextLine = false;
    reader.Set(pStr);
    ut::Rect prevRect = *pRect;
    u16 ch = reader.Next();

    while (static_cast<const T*>(reader.GetCurrentPos()) - pStr <= len) {
        if (ch < ' ') {
            ut::Rect r(lineWidth, lbl_eu_80669D68, lbl_eu_80669D68,
                       lbl_eu_80669D68);

            context.str = static_cast<const T*>(reader.GetCurrentPos());
            context.flags = charSpace ? 0 : ut::PrintContext<T>::FLAGS_CHARSPACE;

            pWriter->SetCursorX(lineWidth);

            ut::TagProcessorBase<T>::Operation oper =
                pWriter->GetTagProcessor()->CalcRect(&r, ch, &context);

            reader.Set(context.str);

            pRect->left = ut::Min(pRect->left, r.left);
            pRect->top = ut::Min(pRect->top, r.top);
            pRect->right = ut::Max(pRect->right, r.right);
            pRect->bottom = ut::Max(pRect->bottom, r.bottom);

            lineWidth = pWriter->GetCursorX();

            if (pRect->right - pRect->left > width) {
                *pHasNextLine = true;
                break;
            }

            if (oper == ut::TagProcessorBase<T>::OPERATION_END_DRAW) {
                return len;
            }

            if (oper == ut::TagProcessorBase<T>::OPERATION_NO_CHAR_SPACE) {
                charSpace = false;
            } else if (oper == ut::TagProcessorBase<T>::OPERATION_CHAR_SPACE) {
                charSpace = true;
            } else if (oper == ut::TagProcessorBase<T>::OPERATION_NEXT_LINE) {
                break;
            }
        } else {
            if (charSpace) {
                lineWidth += pWriter->GetCharSpace();
            }

            charSpace = true;

            if (pWriter->IsWidthFixed()) {
                lineWidth += pWriter->GetFixedWidth();
            } else {
                lineWidth += static_cast<f32>(
                                 pWriter->GetFont()->GetCharWidth(ch)) *
                             pWriter->GetScaleH();
            }

            pRect->left = ut::Min(pRect->left, lineWidth);
            pRect->right = ut::Max(pRect->right, lineWidth);

            if (pRect->right - pRect->left > width) {
                *pHasNextLine = true;
                break;
            }
        }

        pPrevStream = static_cast<const T*>(reader.GetCurrentPos());
        ch = reader.Next();
        prevRect = *pRect;
    }

    // Overflowed: rewind to the last character that still fit.
    if (*pHasNextLine && pPrevStream != NULL) {
        *pRect = prevRect;
        return static_cast<int>(pPrevStream - pStr);
    }

    return static_cast<int>(static_cast<const T*>(reader.GetCurrentPos()) -
                            pStr);
}

template <typename T>
void CalcStringRectImpl(ut::Rect* pRect, ut::TextWriterBase<T>* pWriter,
                        const T* pStr, int len, f32 width) {
    pRect->left = 0.0f;
    pRect->right = 0.0f;
    pRect->top = 0.0f;
    pRect->bottom = 0.0f;

    pWriter->SetCursor(0.0f, 0.0f);

    do {
        ut::Rect r;
        bool hasNextLine;

        int consumed =
            CalcLineRectImpl(&r, pWriter, pStr, len, width, &hasNextLine);

        pStr += consumed;
        len -= consumed;

        // Expand the output rect to encompass this line's rect
        {
            f32 newLeft = r.left;
            pRect->left = pRect->left > newLeft ? newLeft : pRect->left;
        }
        {
            f32 newTop = r.top;
            pRect->top = pRect->top > newTop ? newTop : pRect->top;
        }
        {
            f32 newRight = r.right;
            pRect->right = pRect->right < newRight ? newRight : pRect->right;
        }
        {
            f32 newBottom = r.bottom;
            pRect->bottom = pRect->bottom < newBottom ? newBottom : pRect->bottom;
        }

        // Line overflowed: measure the linebreak itself as a one-char string.
        if (hasNextLine) {
            CalcLineRectImpl(&r, pWriter, lbl_eu_806634C0, 1, width,
                             &hasNextLine);

            {
                f32 newLeft = r.left;
                pRect->left = pRect->left > newLeft ? newLeft : pRect->left;
            }
            {
                f32 newTop = r.top;
                pRect->top = pRect->top > newTop ? newTop : pRect->top;
            }
            {
                f32 newRight = r.right;
                pRect->right = pRect->right < newRight ? newRight : pRect->right;
            }
            {
                f32 newBottom = r.bottom;
                pRect->bottom =
                    pRect->bottom < newBottom ? newBottom : pRect->bottom;
            }
        }
    } while (len > 0);
}

typedef void (*LLMH_ForceFn_us_80404fa4)(nw4r::ut::Rect*,
                                         nw4r::ut::TextWriterBase<wchar_t>*,
                                         const wchar_t*, int, float);
typedef int (*LLMH_ForceFn_us_80404368)(nw4r::ut::Rect*,
                                        nw4r::ut::TextWriterBase<wchar_t>*,
                                        const wchar_t*, int, float, bool*);

} // namespace

extern LLMH_ForceFn_us_80404fa4 LLMH_force_us_80404fa4 =
    &CalcStringRectImpl<wchar_t>;
extern LLMH_ForceFn_us_80404368 LLMH_force_us_80404368 =
    &CalcLineRectImpl<wchar_t>;


/******************************************************************************
 *
 * TextBox
 *
 ******************************************************************************/
namespace {

// Layout overlay matching retail TextBox storage. The locked header places
// mTextPosition/mBits one byte lower than retail (0x100/0x101 there) and its
// members are private, so the retail-mangled constructor below operates
// through this struct instead.
struct TextBoxInitLayout {
    u8 paneHead[0x28];       // Pane fields up to mpMaterial
    void* mpMaterial;        // 0x28
    u8 paneTail[0xD8 - 0x2C];
    wchar_t* mTextBuf;       // 0xD8
    u32 mTextColorsRaw[2];   // 0xDC
    const void* mpFont;      // 0xE4
    f32 fontSizeWidth;       // 0xE8
    f32 fontSizeHeight;      // 0xEC
    f32 mLineSpace;          // 0xF0
    f32 mCharSpace;          // 0xF4
    void* mpTagProcessor;    // 0xF8
    u16 mTextBufBytes;       // 0xFC
    u16 mTextLen;            // 0xFE (occupies 0xFE-0xFF)
    u8 textPosition;         // 0x100
    u8 bits;                 // 0x101
};

// By-reference helper so the Size temporary lives on the stack, matching
// retail's SetFontSize(const Size&) inlining shape.
void StoreFontSize(TextBoxInitLayout& rFields, const nw4r::lyt::Size& rSize) {
    rFields.fontSizeWidth = rSize.width;
    rFields.fontSizeHeight = rSize.height;
}

} // namespace

// MWCC array-construction helper (retail calls this for the color pair).
// The Color ctor/dtor symbols are compiler-generated, so they can only be
// referenced under their mangled names.
extern "C" void __construct_array(void* elements,
                                  void* (*ctor)(void*), void (*dtor)(void*),
                                  u32 elemSize, u32 count);
extern "C" void* __ct__Q34nw4r2ut5ColorFv(void*);
extern "C" void __dt__Q34nw4r2ut5ColorFv(void*);

// Compiler-generated TextBox vtable (retail label lbl_eu_80569A30).
extern "C" void* __vt__Q34nw4r3lyt7TextBox[];

// Base/member constructors referenced by the retail constructor.
extern "C" void __ct__Q34nw4r3lyt4PaneFv(void*);
extern "C" void __ct__Q34nw4r3lyt8MaterialFv(void*);

// Retail TextBox(u16) constructor. The locked header declares only the
// resource constructor, so the retail symbol is provided here under its
// mangled name (same approach as the snd wrappers).
extern "C" nw4r::lyt::TextBox* __ct__Q34nw4r3lyt7TextBoxFUs(
    nw4r::lyt::TextBox* self, unsigned short len) {
    TextBoxInitLayout& rFields = *reinterpret_cast<TextBoxInitLayout*>(self);

    // Base construction.
    __ct__Q34nw4r3lyt4PaneFv(self);

    // Derived vptr install (retail stores the TextBox vtable right after
    // base construction).
    *reinterpret_cast<void**>(rFields.paneHead) =
        static_cast<void*>(__vt__Q34nw4r3lyt7TextBox);

    // Default-construct the vertex color pair.
    __construct_array(&rFields.mTextColorsRaw,
                      reinterpret_cast<void* (*)(void*)>(
                          __ct__Q34nw4r2ut5ColorFv),
                      reinterpret_cast<void (*)(void*)>(
                          __dt__Q34nw4r2ut5ColorFv),
                      sizeof(nw4r::ut::Color), 2);

    // Retail clears the font size directly here, then writes it again
    // through a stack temporary further down.
    rFields.fontSizeWidth = lbl_eu_80669D68;
    rFields.fontSizeHeight = lbl_eu_80669D68;

    rFields.mTextBuf = NULL;
    rFields.mTextBufBytes = 0;
    rFields.mTextLen = 0;

    rFields.mpFont = NULL;

    detail::SetHorizontalPosition(&rFields.textPosition,
                                  HORIZONTALPOSITION_CENTER);
    detail::SetVerticalPosition(&rFields.textPosition,
                                VERTICALPOSITION_CENTER);

    StoreFontSize(rFields, nw4r::lyt::Size(lbl_eu_80669D68, lbl_eu_80669D68));

    rFields.mLineSpace = lbl_eu_80669D68;
    rFields.mCharSpace = lbl_eu_80669D68;

    rFields.mpTagProcessor = NULL;

    std::memset(&rFields.bits, 0, sizeof(rFields.bits));

    if (len > 0) {
        self->AllocStringBuffer(len);
    }

    nw4r::lyt::Material* pMaterialBuf = static_cast<nw4r::lyt::Material*>(
        // Retail references the layout allocator by its retail label here.
        MEMAllocFromAllocator(lbl_eu_80665478, sizeof(nw4r::lyt::Material)));

    nw4r::lyt::Material* pMaterial;
    if (pMaterialBuf != NULL) {
        pMaterial = new (pMaterialBuf) nw4r::lyt::Material();
    } else {
        pMaterial = NULL;
    }

    rFields.mpMaterial = pMaterial;

    if (pMaterial != NULL) {
        pMaterial->ReserveGXMem(0, 0, 0, 0, false, 0, 0, false, false, false,
                                false);
    }

    return self;
}

TextBox::TextBox(const res::TextBox* pRes, const ResBlockSet& rBlockSet)
    : Pane(pRes) {

    u16 len = pRes->textBufBytes / sizeof(wchar_t);
    if (len > 0) {
        len--;
    }

    Init(len);

    if (pRes->textStrBytes >= sizeof(wchar_t) && mTextBuf != NULL) {
        const wchar_t* const pResStr =
            detail::ConvertOffsToPtr<wchar_t>(pRes, pRes->textStrOffset);

        u16 resLen = pRes->textStrBytes / sizeof(wchar_t) - 1;
        SetString(pResStr, 0, resLen);
    }

    for (int i = 0; i < TEXTCOLOR_MAX; i++) {
        mTextColors[i] = pRes->textCols[i];
    }

    mFontSize = pRes->fontSize;
    mTextPosition = pRes->textPosition;
    mCharSpace = pRes->charSpace;
    mLineSpace = pRes->lineSpace;

    const res::Font* const pResFonts = detail::ConvertOffsToPtr<res::Font>(
        rBlockSet.pFontList, sizeof(res::FontList));

    const char* const pFontName = detail::ConvertOffsToPtr<char>(
        pResFonts, pResFonts[pRes->fontIdx].nameStrOffset);

    ut::Font* pFont = rBlockSet.pResAccessor->GetFont(pFontName);

    if (pFont != NULL) {
        mpFont = pFont;
    } else {
        void* pFontBinary = rBlockSet.pResAccessor->GetResource(
            ArcResourceAccessor::RES_TYPE_FONT, pFontName, NULL);

        if (pFontBinary != NULL) {
            ut::ResFont* pFontBuf = static_cast<ut::ResFont*>(Layout::AllocMemory(sizeof(ut::ResFont)));

            if (pFontBuf != NULL) {
                ut::ResFont* pResFont = new (pFontBuf) ut::ResFont();
                pResFont->SetResource(pFontBinary);

                mpFont = pResFont;
                mBits.bAllocFont = true;
            }
        }
    }

    Material* pMaterialBuf = static_cast<Material*>(Layout::AllocMemory(sizeof(Material)));

    if (pMaterialBuf != NULL) {
        const u32* const pMatOffsetTbl = detail::ConvertOffsToPtr<u32>(
            rBlockSet.pMaterialList, sizeof(res::MaterialList));

        const res::Material* const pResMaterial =
            detail::ConvertOffsToPtr<res::Material>(
                rBlockSet.pMaterialList, pMatOffsetTbl[pRes->materialIdx]);

        Material* pMaterial =
            new (pMaterialBuf) Material(pResMaterial, rBlockSet);

        mpMaterial = pMaterial;
    }
}

void TextBox::Init(u16 len) {
    mTextBuf = NULL;
    mTextBufBytes = 0;
    mTextLen = 0;

    mpFont = NULL;
    mFontSize = Size(0.0f, 0.0f);

    SetTextPositionH(HORIZONTALPOSITION_CENTER);
    SetTextPositionV(VERTICALPOSITION_CENTER);

    mLineSpace = 0.0f;
    mCharSpace = 0.0f;

    mpTagProcessor = NULL;

    std::memset(&mBits, 0, sizeof(mBits));

    if (len > 0) {
        AllocStringBuffer(len);
    }
}

const ut::Font* TextBox::GetFont() const {
    return mpFont;
}

void TextBox::SetFont(const ut::Font* pFont) {
    if (mBits.bAllocFont) {
        // Old font kept in a local so it stays live across the dtor call.
        ut::Font* pOldFont = const_cast<ut::Font*>(mpFont);

        if (pOldFont != NULL) {
            pOldFont->~Font();
            MEMFreeToAllocator(lbl_eu_80665478, pOldFont);
        }

        mBits.bAllocFont = false;
    }

    mpFont = pFont;

    if (pFont != NULL) {
        // Implicit int->f32 conversions; MWCC evaluates args right-to-left:
        // pFont->GetHeight() runs first, then GetWidth via a fresh mpFont load.
        SetFontSize(Size(mpFont->GetWidth(), pFont->GetHeight()));
    } else {
        SetFontSize(Size(lbl_eu_80669D68, lbl_eu_80669D68));
    }
}

TextBox::~TextBox() {
    // Retail inlines the SetFont(NULL) body here. Note the metric branch
    // re-tests the mpFont member after the null store (as in the hbm twin),
    // so MWCC emits both arms.
    if (mBits.bAllocFont) {
        ut::Font* pOldFont = const_cast<ut::Font*>(mpFont);

        if (pOldFont != NULL) {
            pOldFont->~Font();
            MEMFreeToAllocator(lbl_eu_80665478, pOldFont);
        }

        mBits.bAllocFont = false;
    }

    mpFont = NULL;

    if (mpFont != NULL) {
        Size size(lbl_eu_80669D68, lbl_eu_80669D68);

        // Right-to-left argument evaluation: height first, then width via a
        // fresh mpFont load. Each conversion lands in the temp before the
        // next virtual call, so no value lives across a call.
        size.height = ConvF32S(mpFont->GetHeight());
        size.width = ConvF32S(mpFont->GetWidth());

        mFontSize = size;
    } else {
        mFontSize = Size(lbl_eu_80669D68, lbl_eu_80669D68);
    }

    if (mpMaterial != NULL && !mpMaterial->IsUserAllocated()) {
        mpMaterial->~Material();
        // Retail references the layout allocator by its label here.
        MEMFreeToAllocator(lbl_eu_80665478, mpMaterial);
        mpMaterial = NULL;
    }

    // Retail dispatches this virtually (vtable slot 0x78) even inside the
    // dtor.
    FreeStringBuffer();
}

ut::Color TextBox::GetVtxColor(u32 idx) const {
    return GetTextColor(idx / TEXTCOLOR_MAX);
}

void TextBox::SetVtxColor(u32 idx, ut::Color color) {
    SetTextColor(idx / TEXTCOLOR_MAX, color);
}

u8 TextBox::GetVtxColorElement(u32 idx) const {
    return reinterpret_cast<const u8*>(
        &mTextColors[idx / (4 * TEXTCOLOR_MAX)])[idx % 4];
}

void TextBox::SetVtxColorElement(u32 idx, u8 value) {
    reinterpret_cast<u8*>(&mTextColors[idx / (4 * TEXTCOLOR_MAX)])[idx % 4] =
        value;
}

ut::Rect TextBox::GetTextDrawRect(const DrawInfo& rInfo) const {
    ut::WideTextWriter writer;
    ut::Rect rect = GetTextDrawRect(&writer);

    if (rInfo.IsYAxisUp()) {
        rect.top = -rect.top;
        rect.bottom = -rect.bottom;
    }

    return rect;
}

void TextBox::DrawSelf(const DrawInfo& rInfo) {
    if (mTextBuf == NULL || mpFont == NULL || mpMaterial == NULL) {
        return;
    }

    // TextBox overrides Pane::LoadMtx (vtable slot 0x70); retail dispatches
    // virtually, so route the call through a base-class pointer.
    Pane* pPane = this;
    pPane->LoadMtx(rInfo);

    ut::TextWriterBase<wchar_t> writer;
    ut::Rect rect = GetTextDrawRect(&writer);

    ut::Color top =
        detail::MultipleAlpha(mTextColors[TEXTCOLOR_TOP], mGlbAlpha);
    ut::Color bottom =
        detail::MultipleAlpha(mTextColors[TEXTCOLOR_BOTTOM], mGlbAlpha);

    writer.SetGradationMode(top != bottom ? ut::CharWriter::GRADMODE_V
                                          : ut::CharWriter::GRADMODE_NONE);
    writer.SetTextColor(top, bottom);

    // Clamp the material TEV colors (GXColorS10 s16 -> u8) for color mapping.
    ut::Color min = GetColor(mpMaterial->GetTevColor(TEVCOLOR_REG0));
    ut::Color max = GetColor(mpMaterial->GetTevColor(TEVCOLOR_REG1));

    writer.SetColorMapping(min, max);
    writer.SetupGX();

    GXSetAlphaCompare(GX_ALWAYS, 0, GX_AOP_AND, GX_ALWAYS, 0);

    // Horizontal alignment factor. Retail keeps two flag bytes at the class
    // tail (after mTextLen): a packed text-position byte at 0x100 whose low
    // modulo-3 value gives the H position, and an alignment byte at 0x101
    // whose bits 5-6 (1-based L/C/R) override that fallback when nonzero.
    struct TailFlags {
        u8 position; // at 0x100
        u8 align;    // at 0x101
    };
    const TailFlags& rFlags = reinterpret_cast<const TailFlags&>(mTextPosition);

    f32 magH;
    switch (rFlags.align >> 5) {
    default: {
        switch (rFlags.position % 3) {
        default: {
            magH = lbl_eu_80669D68;
            break;
        }

        case 1: {
            magH = lbl_eu_80669D78;
            break;
        }

        case 2: {
            magH = lbl_eu_80669D7C;
            break;
        }
        }
        break;
    }

    case 1: {
        magH = lbl_eu_80669D68;
        break;
    }

    case 2: {
        magH = lbl_eu_80669D78;
        break;
    }

    case 3: {
        magH = lbl_eu_80669D7C;
        break;
    }
    }

    wchar_t* pStr = mTextBuf;
    int remaining = mTextLen;

    // Text is drawn Y-down (LoadMtx flips the axis), so negate the top.
    writer.SetCursor(rect.left, -rect.top);

    f32 width = rect.right - rect.left;

    while (remaining > 0) {
        ut::Rect lineRect;
        // Retail reloads the shared 0.0f constant around these stores.
        lineRect.left = lbl_eu_80669D68;
        lineRect.top = lbl_eu_80669D68;
        lineRect.right = lbl_eu_80669D68;
        lineRect.bottom = lbl_eu_80669D68;

        bool hasNextLine;
        int length;
        f32 lineWidth;

        // Measure this line with a scratch copy of the configured writer.
        {
            ut::TextWriterBase<wchar_t> lineWriter = writer;
            length = CalcLineRectImpl(&lineRect, &lineWriter, pStr, remaining,
                                      mSize.width, &hasNextLine);
            lineWidth = lineRect.right - lineRect.left;
        }

        // Align the measured line within the full text rect.
        writer.SetCursorX(rect.left + magH * (width - lineWidth));
        writer.PrintMutable(pStr, length);

        if (hasNextLine) {
            writer.PrintMutable(lbl_eu_806634C0, 1);
        }

        pStr += length;
        remaining -= length;
    }
}

u16 TextBox::GetStringBufferLength() const {
    if (mTextBufBytes == 0) {
        return 0;
    }

    return mTextBufBytes / sizeof(wchar_t) - 1;
}

void TextBox::AllocStringBuffer(u16 len) {
    if (len == 0) {
        return;
    }

    if ((len + 1) * sizeof(wchar_t) > mTextBufBytes) {
        FreeStringBuffer();

        u16 chars = len + 1;

        wchar_t* pBuf = static_cast<wchar_t*>(MEMAllocFromAllocator(
            lbl_eu_80665478, chars * sizeof(wchar_t)));

        // Zero-fill; retail re-tests the cursor inside the loop.
        if (pBuf != NULL) {
            wchar_t* pStr = pBuf;
            for (int i = chars; i > 0; i--) {
                if (pStr != NULL) {
                    *pStr = L'\0';
                    pStr++;
                }
            }
        }

        mTextBuf = pBuf;

        if (pBuf != NULL) {
            mTextBufBytes = (len + 1) * sizeof(wchar_t);
        }
    }
}

void TextBox::FreeStringBuffer() {
    if (mTextBuf != NULL) {
        if (mTextBuf != NULL) {  // retail keeps the redundant re-check
            // Layout::FreeMemory spelled out so the allocator is referenced by
            // its retail label (lbl_eu_80665478) instead of the mangled
            // static-member name (mspAllocator -> lbl_eu_80665488 postprocess).
            MEMFreeToAllocator(lbl_eu_80665478, mTextBuf);
        }
        mTextBuf = NULL;
        mTextBufBytes = 0;
    }
}

u16 TextBox::SetString(const wchar_t* pStr, u16 pos, u16 len) {
    if (mTextBuf == NULL) {
        return 0;
    }

    // Retail inlines GetStringBufferLength() here.
    const u16 maxlen = GetStringBufferLength();
    if (pos >= maxlen) {
        return 0;
    }

    // Retail computes the clamp bound first, then copies len.
    const u32 diff = maxlen - pos;
    u32 chars = len;
    if (chars > diff) {
        chars = diff;
    }

    memcpy(&mTextBuf[pos], pStr, chars * sizeof(wchar_t));

    mTextLen = pos + chars;
    mTextBuf[mTextLen] = L'\0';

    return chars;
}

// Defined after the 3-arg overload so MWCC inlines it here (retail shape).
// The 3-arg overload is virtual, so delegating would emit a vtable dispatch -
// the body is spelled out instead.
u16 TextBox::SetString(const wchar_t* pStr, u16 pos) {
    const size_t len = std::wcslen(pStr);

    if (mTextBuf == NULL) {
        return 0;
    }

    // Retail inlines GetStringBufferLength() here.
    const u16 maxlen = GetStringBufferLength();
    if (pos >= maxlen) {
        return 0;
    }

    // Retail computes the clamp bound first, then copies len.
    const u32 diff = maxlen - pos;
    u32 chars = len;
    if (chars > diff) {
        chars = diff;
    }

    memcpy(&mTextBuf[pos], pStr, chars * sizeof(wchar_t));

    mTextLen = pos + chars;
    mTextBuf[mTextLen] = L'\0';

    return chars;
}

ut::Rect TextBox::GetTextDrawRect(ut::WideTextWriter* pWriter) const {
    ut::Rect rect;

    rect.left = 0.0f;
    rect.top = 0.0f;
    rect.right = 0.0f;
    rect.bottom = 0.0f;

    // Writer configuration (mostly inlined stores in retail).
    pWriter->SetCursor(0.0f, 0.0f);

    pWriter->SetFont(*mpFont);
    pWriter->SetFontSize(mFontSize.width, mFontSize.height);

    pWriter->SetLineSpace(mLineSpace);
    pWriter->SetCharSpace(mCharSpace);

    if (mpTagProcessor != NULL) {
        pWriter->SetTagProcessor(mpTagProcessor);
    }

    // Measure the string with a stack copy of the configured writer.
    {
        ut::WideTextWriter writer = *pWriter;
        CalcStringRectImpl(&rect, &writer, mTextBuf, mTextLen, mSize.width);
    }

    // Retail computes the measured extents before calling GetVtxPos().
    f32 textHeight = rect.bottom - rect.top;
    f32 textWidth = rect.right - rect.left;

    math::VEC2 base = GetVtxPos();

    // Retail inlines the horizontal-position switch twice here.
    f32 magH;
    switch (GetTextPositionH()) {
    case HORIZONTALPOSITION_CENTER: {
        magH = lbl_eu_80669D78;
        break;
    }

    case HORIZONTALPOSITION_RIGHT: {
        magH = lbl_eu_80669D7C;
        break;
    }

    default: {
        magH = lbl_eu_80669D68;
        break;
    }
    }

    f32 offsetX = base.x + (mSize.width - textWidth) * magH;

    rect.left = offsetX;
    rect.right = offsetX + textWidth;

    f32 magV;
    switch (GetTextPositionH()) {
    case HORIZONTALPOSITION_CENTER: {
        magV = lbl_eu_80669D78;
        break;
    }

    case HORIZONTALPOSITION_RIGHT: {
        magV = lbl_eu_80669D7C;
        break;
    }

    default: {
        magV = lbl_eu_80669D68;
        break;
    }
    }

    // NOTE: retail negates here - the text origin is Y-down within the pane,
    // so top/bottom are laid out downward from base.y.
    f32 offsetY = base.y - (mSize.height - textHeight) * magV;

    rect.top = offsetY;
    rect.bottom = rect.top - textHeight;

    return rect;
}

f32 TextBox::GetTextMagH() const {
    f32 mag = 0.0f;

    switch (GetTextPositionH()) {
    default:
    case HORIZONTALPOSITION_LEFT: {
        mag = 0.0f;
        break;
    }

    case HORIZONTALPOSITION_CENTER: {
        mag = 0.5f;
        break;
    }

    case HORIZONTALPOSITION_RIGHT: {
        mag = 1.0f;
        break;
    }
    }

    return mag;
}

f32 TextBox::GetTextMagV() const {
    f32 mag = 0.0f;

    switch (GetTextPositionV()) {
    default:
    case VERTICALPOSITION_TOP: {
        mag = 0.0f;
        break;
    }

    case VERTICALPOSITION_CENTER: {
        mag = 0.5f;
        break;
    }

    case VERTICALPOSITION_BOTTOM: {
        mag = 1.0f;
        break;
    }
    }

    return mag;
}

u32 TextBox::MakeDrawFlag() const {
    u32 flag = 0;

    switch (GetTextPositionH()) {
    case HORIZONTALPOSITION_CENTER: {
        flag |= ut::WideTextWriter::DRAWFLAG_ALIGN_TEXT_CENTER;
        break;
    }

    case HORIZONTALPOSITION_RIGHT: {
        flag |= ut::WideTextWriter::DRAWFLAG_ALIGN_TEXT_RIGHT;
        break;
    }

    default: {
        break;
    }
    }

    return flag;
}

ut::Color TextBox::GetTextColor(u32 idx) const {
    return mTextColors[idx / TEXTCOLOR_MAX];
}

void TextBox::SetTextColor(u32 idx, ut::Color color) {
    mTextColors[idx] = color;
}

} // namespace lyt
} // namespace nw4r

void SetColorMapping__Q34nw4r2ut10CharWriterFQ34nw4r2ut5ColorQ34nw4r2ut5Color(unsigned char* self, const unsigned char* color1, const unsigned char* color2) { self[0] = color1[0]; self[1] = color1[1]; self[2] = color1[2]; self[3] = color1[3]; self[4] = color2[0]; self[5] = color2[1]; self[6] = color2[2]; self[7] = color2[3]; }
namespace nw4r {
namespace ut {

// Out-of-line copy of Font::GetCharStrmReader - taking the address forces MWCC
// to emit the inline header body as a standalone function in this TU (retail
// emits GetCharStrmReader__Q34nw4r2ut4FontCFv here).
typedef CharStrmReader (Font::*LLMH_FontGetReaderFn)() const;
extern LLMH_FontGetReaderFn LLMH_force_us_80404748 = &Font::GetCharStrmReader;

} // namespace ut
} // namespace nw4r
// TextBox overrides Pane::LoadMtx in retail (vtable slot 0x70). The override is
// not declared in lyt_textBox.h, so it is defined here under its mangled name.
void LoadMtx__Q34nw4r3lyt7TextBoxFRCQ34nw4r3lyt8DrawInfo(
    nw4r::lyt::TextBox* self, const nw4r::lyt::DrawInfo& rInfo) {
    nw4r::math::MTX34 mtx;

    if (rInfo.IsMultipleViewMtxOnDraw()) {
        PSMTXConcat(rInfo.GetViewMtx(), self->GetGlobalMtx(), mtx);
    } else {
        PSMTXCopy(self->GetGlobalMtx(), mtx);
    }

    // Flip the Y axis (second column) - text is drawn with a Y-down origin.
    mtx._01 = -mtx._01;
    mtx._11 = -mtx._11;
    mtx._21 = -mtx._21;

    GXLoadPosMtxImm(mtx, GX_PNMTX0);
    GXSetCurrentMtx(GX_PNMTX0);
}
namespace nw4r {
namespace lyt {

extern "C" nw4r::ut::detail::RuntimeTypeInfo lbl_eu_80665488(&lbl_eu_80665470);

const nw4r::ut::detail::RuntimeTypeInfo* TextBox::GetRuntimeTypeInfo() const {
    return &lbl_eu_80665488;
}

} // namespace lyt
} // namespace nw4r






