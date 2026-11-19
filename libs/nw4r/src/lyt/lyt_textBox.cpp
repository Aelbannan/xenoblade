#include <nw4r/lyt.h>
#include <nw4r/ut.h>

#include <cstring>
#include <cwchar>

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
    pRect->left = 0.0f;
    pRect->right = 0.0f;

    f32 lineWidth = 0.0f;
    bool charSpace = false;
    const T* pPrevStream = NULL;

    pRect->top = ut::Min(0.0f, pWriter->GetLineHeight());
    pRect->bottom = ut::Max(0.0f, pWriter->GetLineHeight());

    *pHasNextLine = false;
    reader.Set(pStr);
    ut::Rect prevRect = *pRect;
    u16 ch = reader.Next();

    while (static_cast<const T*>(reader.GetCurrentPos()) - pStr <= len) {
        if (ch < ' ') {
            ut::Rect r(lineWidth, 0.0f, 0.0f, 0.0f);

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
            CalcLineRectImpl(&r, pWriter, L"\n", 1, width, &hasNextLine);

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

} // namespace


/******************************************************************************
 *
 * TextBox
 *
 ******************************************************************************/
// Inlined by both constructors (retail has no out-of-line Init symbol).
inline void TextBox::Init(u16 len) {
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

// Default constructor - Pane is set up by the implicit base construction and
// mTextColors/mFontSize are default-initialised by the member constructors.
TextBox::TextBox(u16 len) {
    Init(len);

    Material* pMaterialBuf =
        static_cast<Material*>(Layout::AllocMemory(sizeof(Material)));

    Material* pMaterial;
    if (pMaterialBuf != NULL) {
        pMaterial = new (pMaterialBuf) Material();
    } else {
        pMaterial = NULL;
    }

    mpMaterial = pMaterial;

    if (pMaterial != NULL) {
        pMaterial->ReserveGXMem(0, 0, 0, 0, false, 0, 0, false, false, false,
                                false);
    }
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
    mBits.textAlignment = pRes->textAlignment;
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
            ut::ResFont* pFontBuf = static_cast<ut::ResFont*>(
                Layout::AllocMemory(sizeof(ut::ResFont)));

            ut::ResFont* pResFont;
            if (pFontBuf != NULL) {
                pResFont = new (pFontBuf) ut::ResFont();
            } else {
                pResFont = NULL;
            }

            if (pResFont != NULL) {
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

const ut::Font* TextBox::GetFont() const {
    return mpFont;
}

void TextBox::SetFont(const ut::Font* pFont) {
    if (mBits.bAllocFont) {
        // Old font kept in a local so it stays live across the dtor call.
        ut::Font* pOldFont = const_cast<ut::Font*>(mpFont);

        if (pOldFont != NULL) {
            pOldFont->~Font();
            Layout::FreeMemory(pOldFont);
        }

        mBits.bAllocFont = false;
    }

    mpFont = pFont;

    if (mpFont != NULL) {
        SetFontSize(Size(mpFont->GetWidth(), mpFont->GetHeight()));
    } else {
        SetFontSize(Size(0.0f, 0.0f));
    }
}

TextBox::~TextBox() {
    SetFont(NULL);

    if (mpMaterial != NULL && !mpMaterial->IsUserAllocated()) {
        Layout::DeleteObj(mpMaterial);
        mpMaterial = NULL;
    }

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

    // The 1-based text alignment overrides the packed text position when set.
    f32 magH;
    switch (mBits.textAlignment) {
    default: {
        switch (GetTextPositionH()) {
        default: {
            magH = 0.0f;
            break;
        }

        case HORIZONTALPOSITION_CENTER: {
            magH = 0.5f;
            break;
        }

        case HORIZONTALPOSITION_RIGHT: {
            magH = 1.0f;
            break;
        }
        }
        break;
    }

    case HORIZONTALPOSITION_LEFT + 1: {
        magH = 0.0f;
        break;
    }

    case HORIZONTALPOSITION_CENTER + 1: {
        magH = 0.5f;
        break;
    }

    case HORIZONTALPOSITION_RIGHT + 1: {
        magH = 1.0f;
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
        lineRect.left = 0.0f;
        lineRect.top = 0.0f;
        lineRect.right = 0.0f;
        lineRect.bottom = 0.0f;

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
            writer.PrintMutable(L"\n", 1);
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

    if (static_cast<u32>(len + 1) * sizeof(wchar_t) > mTextBufBytes) {
        FreeStringBuffer();

        wchar_t* pBuf = static_cast<wchar_t*>(
            Layout::AllocMemory((len + 1) * sizeof(wchar_t)));

        if (pBuf != NULL) {
            wchar_t* pStr = pBuf;
            for (u32 i = 0; i <= len; i++) {
                if (pStr != NULL) {
                    *pStr = L'\0';
                }
                pStr++;
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
            Layout::FreeMemory(mTextBuf);
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

    pWriter->SetCursor(0.0f, 0.0f);
    pWriter->SetFont(*mpFont);
    pWriter->SetFontSize(mFontSize.width, mFontSize.height);
    pWriter->SetLineSpace(mLineSpace);
    pWriter->SetCharSpace(mCharSpace);

    if (mpTagProcessor != NULL) {
        pWriter->SetTagProcessor(mpTagProcessor);
    }

    // Measure with a stack copy of the configured writer.
    {
        ut::WideTextWriter writer = *pWriter;
        CalcStringRectImpl(&rect, &writer, mTextBuf, mTextLen, mSize.width);
    }

    math::VEC2 base = GetVtxPos();
    rect.MoveTo(base.x + (mSize.width - rect.GetWidth()) * GetTextMagH(),
                base.y + (mSize.height - rect.GetHeight()) * GetTextMagV());

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

void TextBox::LoadMtx(const DrawInfo& rInfo) {
    math::MTX34 mtx;

    if (rInfo.IsMultipleViewMtxOnDraw()) {
        PSMTXConcat(rInfo.GetViewMtx(), GetGlobalMtx(), mtx);
    } else {
        PSMTXCopy(GetGlobalMtx(), mtx);
    }

    // Flip the Y axis - text is drawn from a Y-down origin.
    mtx._01 = -mtx._01;
    mtx._11 = -mtx._11;
    mtx._21 = -mtx._21;

    GXLoadPosMtxImm(mtx, GX_PNMTX0);
    GXSetCurrentMtx(GX_PNMTX0);
}

NW4R_UT_RTTI_DEF_DERIVED(TextBox, Pane);

const nw4r::ut::detail::RuntimeTypeInfo* TextBox::GetRuntimeTypeInfo() const {
    return &typeInfo;
}

} // namespace lyt
} // namespace nw4r



