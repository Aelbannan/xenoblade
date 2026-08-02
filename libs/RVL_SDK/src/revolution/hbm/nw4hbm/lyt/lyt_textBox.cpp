#include <nw4hbm/lyt.h>
#include <nw4hbm/ut.h>

#include <cstring>
#include <cwchar>

/******************************************************************************
 *
 * Utility functions
 *
 ******************************************************************************/
namespace nw4hbm {
namespace ut {

/* Defined inline here so MWCC inlines away all ~Color calls in this TU
 * (retail DrawSelf emits no dtor calls; the strong ~Color symbol lives in
 * lyt_bounding.o per retail). */
inline Color::~Color() {}

} // namespace ut
} // namespace nw4hbm

namespace {

using namespace nw4hbm;
using namespace nw4hbm::lyt;

inline u8 ClampColor(s16 value) {
    return value < 0 ? 0 : (value > 255 ? 255 : value);
}

GXColor ClampTevColor(const GXColorS10& rColor) {
    GXColor color;
    color.r = ClampColor(rColor.r);
    color.g = ClampColor(rColor.g);
    color.b = ClampColor(rColor.b);
    color.a = ClampColor(rColor.a);
    return color;
}

ut::Color GetColor(const GXColorS10& rColor16) {
    GXColor color8 = {ClampColor(rColor16.r), ClampColor(rColor16.g),
                      ClampColor(rColor16.b), ClampColor(rColor16.a)};

    return ut::Color(color8);
}

// Forward declarations for local template helpers
template <typename T>
int CalcLineRectImpl(ut::Rect* pRect, ut::TextWriterBase<T>* pWriter,
                     const T* pStr, int len, f32 width, bool* pHasNextLine);

template <typename T>
void CalcStringRectImpl(ut::Rect* pRect, ut::TextWriterBase<T>* pWriter,
                        const T* pStr, int len, f32 width);

#pragma push
#pragma auto_inline off

template <typename T>
int CalcLineRectImpl(ut::Rect* pRect, ut::TextWriterBase<T>* pWriter,
                     const T* pStr, int len, f32 width, bool* pHasNextLine) {
    ut::PrintContext<T> context = { pWriter, pStr };

    ut::CharStrmReader reader = pWriter->GetFont()->GetCharStrmReader();

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
                lineWidth += static_cast<f32>(pWriter->GetFont()->GetCharWidth(ch)) *
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

    if (*pHasNextLine && pPrevStream != NULL) {
        *pRect = prevRect;
        return static_cast<int>(pPrevStream - pStr);
    }

    return static_cast<int>(static_cast<const T*>(reader.GetCurrentPos()) - pStr);
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

        if (hasNextLine) {
            CalcLineRectImpl(&r, pWriter, L"\n", 1, width, &hasNextLine);
        }

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
    } while (len > 0);
}

template int CalcLineRectImpl<wchar_t>(ut::Rect*, ut::TextWriterBase<wchar_t>*,
                                        const wchar_t*, int, f32, bool*);
template void CalcStringRectImpl<wchar_t>(ut::Rect*,
                                           ut::TextWriterBase<wchar_t>*,
                                           const wchar_t*, int, f32);

#pragma pop

} // namespace

namespace nw4hbm {
namespace lyt {

NW4R_UT_RTTI_DEF_DERIVED(TextBox, Pane);

/******************************************************************************
 *
 * TextBox
 *
 ******************************************************************************/
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
            void* pFontBuf = Layout::AllocMemory(sizeof(ut::ResFont));

            if (pFontBuf != NULL) {
                ut::ResFont* pResFont = new (pFontBuf) ut::ResFont();
                pResFont->SetResource(pFontBinary);

                mpFont = pResFont;
                mBits.bAllocFont = true;
            }
        }
    }

    void* pMaterialBuf = Layout::AllocMemory(sizeof(Material));

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

// NOTE: Init/GetTextDrawRect/GetFont/SetFont/GetTextMagH/GetTextMagV/
// MakeDrawFlag/GetStringBufferLength are NOT standalone retail symbols in the
// nw4hbm (HBM) TextBox unit - the retail build only emits the 16 functions
// listed in the split. They are kept here as inline definitions so MWCC folds
// them into their single call sites (ctor, dtor, SetString, DrawSelf) and emits
// no out-of-line copies, keeping the unit's .text within the retail budget.
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

TextBox::~TextBox() {
    SetFont(NULL);

    if (mpMaterial != NULL && !mpMaterial->IsUserAllocated()) {
        mpMaterial->~Material();
        Layout::FreeMemory(mpMaterial);
        mpMaterial = NULL;
    }

    FreeStringBuffer();
}

ut::Color TextBox::GetVtxColor(u32 idx) const {
    // Offset to color array (2 colors, each 4 bytes)
    const u8* base = reinterpret_cast<const u8*>(this) + 0xd8;
    return reinterpret_cast<const ut::Color*>(base)[idx >> 1];
}

void TextBox::SetVtxColor(u32 idx, ut::Color color) {
    u8* dst = reinterpret_cast<u8*>(&mTextColors[(idx & 0x7ffffffe) >> 1]);
    u32 packed = *reinterpret_cast<const u32*>(&color);
    const u8* src = reinterpret_cast<const u8*>(&packed);
    dst[0] = src[0];
    dst[1] = src[1];
    dst[2] = src[2];
    dst[3] = src[3];
}

u8 TextBox::GetVtxColorElement(u32 idx) const {
    return reinterpret_cast<const u8*>(
        &mTextColors[idx / (4 * TEXTCOLOR_MAX)])[idx % 4];
}

void TextBox::SetVtxColorElement(u32 idx, u8 value) {
    reinterpret_cast<u8*>(&mTextColors[idx / (4 * TEXTCOLOR_MAX)])[idx % 4] =
        value;
}

inline ut::Rect TextBox::GetTextDrawRect(const DrawInfo& rInfo) const {
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

    LoadMtx(rInfo);

    ut::WideTextWriter writer;
    writer.SetFont(*mpFont);
    writer.SetFontSize(mFontSize.width, mFontSize.height);
    writer.SetLineSpace(mLineSpace);
    writer.SetCharSpace(mCharSpace);

    ut::Color top =
        detail::MultipleAlpha(mTextColors[TEXTCOLOR_TOP], mGlbAlpha);
    ut::Color bottom =
        detail::MultipleAlpha(mTextColors[TEXTCOLOR_BOTTOM], mGlbAlpha);

    writer.SetGradationMode(top != bottom ? ut::CharWriter::GRADMODE_V
                                          : ut::CharWriter::GRADMODE_NONE);
    writer.SetTextColor(top, bottom);

    // Clamp TEV colors (GXColorS10 s16->u8) for color mapping
    writer.SetColorMapping(
        ut::Color(ClampTevColor(mpMaterial->GetTevColor(TEVCOLOR_REG0))),
        ut::Color(ClampTevColor(mpMaterial->GetTevColor(TEVCOLOR_REG1))));

    if (mpTagProcessor != NULL) {
        writer.SetTagProcessor(mpTagProcessor);
    }

    writer.SetupGX();

    // Calculate the text bounding rect
    ut::Rect rect;
    rect.left = 0.0f;
    rect.top = 0.0f;
    rect.right = 0.0f;
    rect.bottom = 0.0f;

    writer.SetCursor(0.0f, 0.0f);

    {
        f32 textWidth2 = mSize.width;
        int textLen = mTextLen;
        const wchar_t* pText = mTextBuf;

        ut::WideTextWriter rectWriter = writer;
        CalcStringRectImpl(&rect, &rectWriter, pText, textLen, textWidth2);
    }

    // Align rect within the pane using text position flags
    math::VEC2 pos = GetVtxPos();

    f32 magH;
    switch ((int)GetTextPositionH()) {
    default:
    case HORIZONTALPOSITION_LEFT: {
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

    f32 magV;
    switch ((int)GetTextPositionV()) {
    default:
    case VERTICALPOSITION_TOP: {
        magV = 0.0f;
        break;
    }

    case VERTICALPOSITION_CENTER: {
        magV = 0.5f;
        break;
    }

    case VERTICALPOSITION_BOTTOM: {
        magV = 1.0f;
        break;
    }
    }

    f32 textHeight = rect.bottom - rect.top;
    f32 textWidth = rect.right - rect.left;

    f32 offsetY = pos.y + (mSize.height - textHeight) * magV;
    f32 offsetX = pos.x + (mSize.width - textWidth) * magH;

    rect.top = offsetY;
    rect.bottom = offsetY + textHeight;
    rect.left = offsetX;
    rect.right = offsetX + textWidth;

    // Per-line horizontal alignment factor (recomputed same way)
    f32 lineMagH;
    switch ((int)GetTextPositionH()) {
    default:
    case HORIZONTALPOSITION_LEFT: {
        lineMagH = 0.0f;
        break;
    }

    case HORIZONTALPOSITION_CENTER: {
        lineMagH = 0.5f;
        break;
    }

    case HORIZONTALPOSITION_RIGHT: {
        lineMagH = 1.0f;
        break;
    }
    }

    f32 totalWidth = rect.right - rect.left;

    const wchar_t* pStr = mTextBuf;
    writer.SetCursor(rect.left, rect.top);

    int remaining = mTextLen;

    while (remaining > 0) {
        f32 width = mSize.width;

        ut::Rect lineRect;
        lineRect.left = 0.0f;
        lineRect.top = 0.0f;
        lineRect.right = 0.0f;
        lineRect.bottom = 0.0f;

        bool hasNextLine;
        int consumed;
        f32 lineWidth;
        {
            ut::WideTextWriter lineWriter = writer;
            lineWriter.SetCursor(0.0f, 0.0f);
            consumed = CalcLineRectImpl(&lineRect, &lineWriter, pStr,
                                        remaining, width, &hasNextLine);
            lineWidth = lineRect.right - lineRect.left;
        }

        f32 x = rect.left + (totalWidth - lineWidth) * lineMagH;
        writer.SetCursorX(x);

        writer.Print(pStr, consumed);

        if (hasNextLine) {
            writer.Print(L"\n");
        }

        pStr += consumed;
        remaining -= consumed;
    }
}

inline u16 TextBox::GetStringBufferLength() const {
    if (mTextBufBytes == 0) {
        return 0;
    }

    return mTextBufBytes / sizeof(wchar_t) - 1;
}

void TextBox::AllocStringBuffer(u16 len) {
    if (len == 0) {
        return;
    }

    u16 chars = len + 1;
    u16 bytes = chars * sizeof(wchar_t);

    if (bytes > mTextBufBytes) {
        FreeStringBuffer();
        mTextBuf = static_cast<wchar_t*>(Layout::AllocMemory(bytes));

        if (mTextBuf != NULL) {
            mTextBufBytes = bytes;
        }
    }
}

void TextBox::FreeStringBuffer() {
    if (mTextBuf == NULL) {
        return;
    }

    Layout::FreeMemory(mTextBuf);
    mTextBuf = NULL;
    mTextBufBytes = 0;
}

u16 TextBox::SetString(const wchar_t* pStr, u16 pos) {
    return SetString(pStr, pos, std::wcslen(pStr));
}

u16 TextBox::SetString(const wchar_t* pStr, u16 pos, u16 len) {
    if (mTextBuf == NULL) {
        return 0;
    }

    const u16 maxlen = GetStringBufferLength();
    if (pos >= maxlen) {
        return 0;
    }

    const u16 chars = ut::Min<u16>(len, maxlen - pos);
    memcpy(&mTextBuf[pos], pStr, chars * sizeof(wchar_t));

    mTextLen = pos + chars;
    mTextBuf[mTextLen] = L'\0';

    return chars;
}

inline const ut::Font* TextBox::GetFont() const {
    return mpFont;
}

inline void TextBox::SetFont(const ut::Font* pFont) {
    if (mBits.bAllocFont) {
        mpFont->~Font();
        Layout::FreeMemory(const_cast<ut::Font*>(mpFont));
        mBits.bAllocFont = false;
    }

    mpFont = pFont;

    if (mpFont != NULL) {
        SetFontSize(Size(static_cast<f32>(mpFont->GetWidth()),
                         static_cast<f32>(mpFont->GetHeight())));
    } else {
        SetFontSize(Size(0.0f, 0.0f));
    }
}

inline ut::Rect TextBox::GetTextDrawRect(ut::WideTextWriter* pWriter) const {
    ut::Rect rect;

    pWriter->SetCursor(0.0f, 0.0f);

    pWriter->SetFont(*mpFont);
    pWriter->SetFontSize(mFontSize.width, mFontSize.height);

    pWriter->SetLineSpace(mLineSpace);
    pWriter->SetCharSpace(mCharSpace);

    pWriter->SetDrawFlag(MakeDrawFlag());

    if (mpTagProcessor != NULL) {
        pWriter->SetTagProcessor(mpTagProcessor);
    }

    pWriter->CalcStringRect(&rect, mTextBuf, mTextLen);

    math::VEC2 base = GetVtxPos();
    rect.MoveTo(base.x + (mSize.width - rect.GetWidth()) * GetTextMagH(),
                base.y + (mSize.height - rect.GetHeight()) * GetTextMagV());

    return rect;
}

inline f32 TextBox::GetTextMagH() const {
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

inline f32 TextBox::GetTextMagV() const {
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

inline u32 TextBox::MakeDrawFlag() const {
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

} // namespace lyt
} // namespace nw4hbm

extern "C" void* GetRuntimeTypeInfo__Q36nw4hbm3lyt7TextBoxCFv(void) {
    extern const char typeInfo__Q36nw4hbm3lyt7TextBox[];
    return (void*)typeInfo__Q36nw4hbm3lyt7TextBox;
}
