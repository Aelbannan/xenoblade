#include <nw4r/ut.h>

// Absorbed .bss 0x80653EC8 (12) + 0x80653ED4 (12) + .sbss 0x80665560-0x80665570 (0x10)
unsigned char lbl_eu_80653EC8[12] __attribute__((aligned(4)));
unsigned char lbl_eu_80653ED4[12] __attribute__((aligned(4)));


// Absorbed .bss 0x80653EC8-0x80653EE0 (0x18) + .sbss 0x80665560-0x80665570 (0x10)


// Retail .sbss data owned by this TU (monolibdata1 blob dissolve): the real
// static members below are the two 4-byte tag-processor objects at +0/+4, and
// the compiler-generated __sinit_ constructs them with guards.
//   lbl_eu_80665560 (.sbss 0x4) = TextWriterBase<char>::mDefaultTagProcessor
//   lbl_eu_80665564 (.sbss 0x4) = TextWriterBase<wchar_t>::mDefaultTagProcessor
//   lbl_eu_80665568 (.sbss 0x1) = __sinit__ init guard (char)
//   lbl_eu_80665569 (.sbss 0x7) = __sinit__ init guard (wchar_t) - retail
//                                 recovered symbol size is 7 (1 used byte)
namespace nw4r {
namespace ut {

// Real static members; MWCC emits the guarded construction in __sinit_.
template <> TagProcessorBase<char> TextWriterBase<char>::mDefaultTagProcessor;
template <>
TagProcessorBase<wchar_t> TextWriterBase<wchar_t>::mDefaultTagProcessor;

extern "C" {
    u8 lbl_eu_80665568;
    // Retail symbol lbl_eu_80665569 is a 7-byte object at 0x80665569. MWCC
    // 4-aligns a u8[7] array (offset 0xC), overshooting the retail 0x10
    // section -- individual u8 globals pack at byte granularity (0x9..0xF).
    u8 lbl_eu_80665569;
    u8 lbl_eu_8066556A;
    u8 lbl_eu_8066556B;
    u8 lbl_eu_8066556C;
    u8 lbl_eu_8066556D;
    u8 lbl_eu_8066556E;
    u8 lbl_eu_8066556F;
}

// mFormatBuffer / mFormatBufferSize are not defined here (retail
// ut_TextWriterBase.o has .sdata/.bss size 0; the FormatV/VPrintf family that
// references them is not part of the retail nw4r split).

template <typename T>
TextWriterBase<T>::TextWriterBase()
    : mWidthLimit(NW4R_MATH_FLT_MAX),
      mCharSpace(0.0f),
      mLineSpace(0.0f),
      mTabWidth(4),
      mDrawFlag(0),
      mTagProcessor(&mDefaultTagProcessor) {}

template <typename T> TextWriterBase<T>::~TextWriterBase() {}

template <typename T> f32 TextWriterBase<T>::GetLineHeight() const {
    const Font* pFont = GetFont();
    int lf = pFont != NULL ? pFont->GetLineFeed() : 0;
    return mLineSpace + GetScaleV() * lf;
}

template <typename T>
f32 TextWriterBase<T>::VPrintf(const T* pStr, std::va_list args) {
    T* pBuffer;

    if (mFormatBuffer != NULL) {
        pBuffer = mFormatBuffer;
    } else {
        pBuffer = static_cast<T*>(__alloca(mFormatBufferSize));
    }

    int len = VSNPrintf(pBuffer, mFormatBufferSize, pStr, args);
    return Print(pBuffer, len);
}

template <typename T> f32 TextWriterBase<T>::Print(const T* pStr, int len) {
    TextWriterBase<T> clone(*this);

    f32 width = clone.PrintImpl(pStr, len, false);
    SetCursor(clone.GetCursorX(), clone.GetCursorY());

    return width;
}

template <typename T> f32 TextWriterBase<T>::PrintMutable(const T* pStr, int len) {
    return PrintImpl(pStr, len, true);
}

template <typename T>
f32 TextWriterBase<T>::CalcLineWidth(const T* pStr, int len) {
    Rect rect(0.0f, 0.0f, 0.0f, 0.0f);
    TextWriterBase<T> clone(*this);

    clone.SetCursor(0.0f, 0.0f);
    clone.CalcLineRectImpl(&rect, &pStr, len);

    return rect.GetWidth();
}

template <typename T>
bool TextWriterBase<T>::CalcLineRectImpl(Rect* pRect, const T** ppStr,
                                         int len) {
    const T* pStrBegin = *ppStr;
    const T* pStrEnd = pStrBegin + len;
    bool useLimit = mWidthLimit < NW4R_MATH_FLT_MAX;

    PrintContext<T> context = {
        this,     // writer
        pStrBegin // str
    };

    f32 x = 0.0f;
    bool charSpace = false;
    bool overLimit = false;

    const T* pPrevStream = NULL;
    Rect prevRect(0.0f, 0.0f, 0.0f, 0.0f);

    CharStrmReader reader = GetFont()->GetCharStrmReader();

    pRect->left = 0.0f;
    pRect->right = 0.0f;
    pRect->top = Min(0.0f, GetLineHeight());
    pRect->bottom = Max(0.0f, GetLineHeight());
    prevRect = *pRect;

    reader.Set(pStrBegin);
    pPrevStream = NULL;

    u16 ch = reader.Next();

    while (static_cast<const T*>(reader.GetCurrentPos()) <= pStrEnd) {
        if (ch < ' ') {
            Rect r(x, 0.0f, 0.0f, 0.0f);
            context.str = static_cast<const T*>(reader.GetCurrentPos());
            context.flags = charSpace ? 0 : PrintContext<T>::FLAGS_CHARSPACE;

            SetCursorX(x);

            if (useLimit && ch != '\n' && pPrevStream != NULL) {
                PrintContext<T> context2 = context;
                TextWriterBase<T> clone(*this);

                Rect r(0.0f, 0.0f, 0.0f, 0.0f);
                context2.writer = &clone;
                mTagProcessor->CalcRect(&r, ch, &context2);

                if (r.GetWidth() > 0.0f &&
                    clone.GetCursorX() - context.x > mWidthLimit) {
                    overLimit = true;
                    ch = '\n';
                    reader.Set(pPrevStream);
                    continue;
                }
            }

            typename TagProcessorType::Operation oper =
                mTagProcessor->CalcRect(&r, ch, &context);

            reader.Set(context.str);

            pRect->left = Min(pRect->left, r.left);
            pRect->top = Min(pRect->top, r.top);
            pRect->right = Max(pRect->right, r.right);
            pRect->bottom = Max(pRect->bottom, r.bottom);

            x = GetCursorX();

            if (oper == TagProcessorType::OPERATION_END_DRAW) {
                *ppStr += len;
                return false;
            }

            if (oper == TagProcessorType::OPERATION_NO_CHAR_SPACE) {
                charSpace = false;
            } else if (oper == TagProcessorType::OPERATION_CHAR_SPACE) {
                charSpace = true;
            } else if (oper == TagProcessorType::OPERATION_NEXT_LINE) {
                break;
            }
        } else {
            f32 dx = 0.0f;

            if (charSpace) {
                dx += GetCharSpace();
            }

            if (IsWidthFixed()) {
                dx += GetFixedWidth();
            } else {
                dx += (f32)GetFont()->GetCharWidth(ch) * GetScaleH();
            }

            if (useLimit && pPrevStream != NULL && x + dx > mWidthLimit) {
                overLimit = true;
                ch = '\n';
                reader.Set(pPrevStream);
                continue;
            }

            x += dx;
            pRect->left = Min(pRect->left, x);
            pRect->right = Max(pRect->right, x);

            charSpace = true;
        }

        if (useLimit) {
            pPrevStream = static_cast<const T*>(reader.GetCurrentPos());
        }

        ch = reader.Next();
    }

    *ppStr = static_cast<const T*>(reader.GetCurrentPos());
    return overLimit;
}

template <typename T>
void TextWriterBase<T>::CalcStringRectImpl(Rect* pRect, const T* pStr,
                                           int len) {
    const T* pEnd = pStr + len;
    int remain = len;

    pRect->left = 0.0f;
    pRect->right = 0.0f;
    pRect->top = 0.0f;
    pRect->bottom = 0.0f;

    SetCursor(0.0f, 0.0f);

    do {
        Rect r(0.0f, 0.0f, 0.0f, 0.0f);
        CalcLineRectImpl(&r, &pStr, remain);
        remain = pEnd - pStr;

        pRect->left = Min(pRect->left, r.left);
        pRect->top = Min(pRect->top, r.top);
        pRect->right = Max(pRect->right, r.right);
        pRect->bottom = Max(pRect->bottom, r.bottom);
    } while (remain > 0);
}

template <typename T> f32 TextWriterBase<T>::PrintImpl(const T* pStr, int len, bool bMutable) {
    f32 cursorX = GetCursorX();
    f32 cursorY = GetCursorY();

    bool useLimit = mWidthLimit < NW4R_MATH_FLT_MAX;

    f32 orgCursorX = cursorX;
    f32 orgCursorY = cursorY;

    f32 cursorXAdj = 0.0f;
    f32 cursorYAdj = 0.0f;

    bool charSpace = false;

    const T* pPrevStream = pStr;
    const T* pPrevNewLine = pStr;

    f32 textWidth = AdjustCursor(&cursorX, &cursorY, pStr, len);

    cursorXAdj = orgCursorX - GetCursorX();
    cursorYAdj = orgCursorY - GetCursorY();

    PrintContext<T> context = {
        this,    // writer
        pStr,    // str
        cursorX, // x
        cursorY  // y
    };

    CharStrmReader reader = GetFont()->GetCharStrmReader();
    reader.Set(pStr);

    typename TagProcessorType::Operation oper;
    u16 ch = reader.Next();

    while (static_cast<const T*>(reader.GetCurrentPos()) - pStr <= len) {
        if (ch < ' ') {
            context.str = static_cast<const T*>(reader.GetCurrentPos());
            context.flags = charSpace ? 0 : PrintContext<T>::FLAGS_CHARSPACE;

            if (useLimit && ch != '\n' && pPrevStream != pPrevNewLine) {
                PrintContext<T> context2 = context;
                TextWriterBase<T> clone(*this);
                Rect rect(0.0f, 0.0f, 0.0f, 0.0f);

                context2.writer = &clone;
                oper = mTagProcessor->CalcRect(&rect, ch, &context2);

                if (rect.GetWidth() > 0.0f &&
                    clone.GetCursorX() - context.x > mWidthLimit) {
                    ch = '\n';
                    reader.Set(pPrevStream);
                    continue;
                }
            }

            oper = mTagProcessor->Process(ch, &context);
            if (oper == TagProcessorType::OPERATION_NEXT_LINE) {
                if (IsDrawFlagSet(DRAWFLAG_MASK_ALIGN_TEXT,
                                  DRAWFLAG_ALIGN_TEXT_CENTER)) {

                    int remain = len - (context.str - pStr);
                    f32 width = CalcLineWidth(context.str, remain);

                    f32 offset = (textWidth - width) * 0.5f;
                    SetCursorX(context.x + offset);
                } else if (IsDrawFlagSet(DRAWFLAG_MASK_ALIGN_TEXT,
                                         DRAWFLAG_ALIGN_TEXT_RIGHT)) {

                    int remain = len - (context.str - pStr);
                    f32 width = CalcLineWidth(context.str, remain);

                    f32 offset = textWidth - width;
                    SetCursorX(context.x + offset);
                } else {
                    f32 width = GetCursorX() - context.x;
                    textWidth = Max(textWidth, width);
                    SetCursorX(context.x);
                }

                if (useLimit) {
                    pPrevNewLine =
                        static_cast<const T*>(reader.GetCurrentPos());
                }

                charSpace = false;
            } else if (oper == TagProcessorType::OPERATION_NO_CHAR_SPACE) {
                charSpace = false;
            } else if (oper == TagProcessorType::OPERATION_CHAR_SPACE) {
                charSpace = true;
            } else if (oper == TagProcessorType::OPERATION_END_DRAW) {
                break;
            }

            reader.Set(context.str);
        } else {
            f32 baseY = GetCursorY();
            if (useLimit && pPrevStream != pPrevNewLine) {
                f32 baseX = GetCursorX();
                f32 space = charSpace ? GetCharSpace() : 0.0f;

                f32 width = IsWidthFixed()
                                ? GetFixedWidth()
                                : (f32)GetFont()->GetCharWidth(ch) *
                                      GetScaleH();

                if (baseX - cursorX + space + width > mWidthLimit) {
                    ch = '\n';
                    reader.Set(pPrevStream);
                    continue;
                }
            }

            if (charSpace) {
                MoveCursorX(GetCharSpace());
            }

            charSpace = true;

            f32 adj = -(f32)GetFont()->GetBaselinePos() * GetScaleV();
            MoveCursorY(adj);
            CharWriter::Print(ch);
            SetCursorY(baseY);
        }

        if (useLimit) {
            pPrevStream = static_cast<const T*>(reader.GetCurrentPos());
        }

        ch = reader.Next();
    }

    f32 width = GetCursorX() - context.x;
    textWidth = Max(textWidth, width);

    if (IsDrawFlagSet(DRAWFLAG_MASK_ALIGN_V, DRAWFLAG_ALIGN_V_CENTER) ||
        IsDrawFlagSet(DRAWFLAG_MASK_ALIGN_V, DRAWFLAG_ALIGN_V_TOP)) {
        SetCursorY(orgCursorY);
    } else if(bMutable) {
        if (IsDrawFlagSet(DRAWFLAG_MASK_ALIGN_V, DRAWFLAG_ALIGN_TEXT_BASELINE)) {
            SetCursorY(GetCursorY() - GetFontAscent());
        }
    } else {
        MoveCursorY(cursorYAdj);
    }

    return textWidth;
}

template <typename T>
f32 TextWriterBase<T>::AdjustCursor(f32* pX, f32* pY, const T* pStr, int len) {
    f32 textWidth = 0.0f;
    f32 textHeight = 0.0f;

    if (!IsDrawFlagSet(DRAWFLAG_MASK_ALL, DRAWFLAG_MASK_ALIGN_V) &&
        !IsDrawFlagSet(DRAWFLAG_MASK_ALL, 0)) {

        Rect rect(0.0f, 0.0f, 0.0f, 0.0f);
        CalcStringRect(&rect, pStr, len);

        textWidth = rect.left + rect.right;
        textHeight = rect.top + rect.bottom;
    }

    if (IsDrawFlagSet(DRAWFLAG_MASK_ALIGN_H, DRAWFLAG_ALIGN_H_CENTER)) {
        *pX -= textWidth / 2.0f;
    } else if (IsDrawFlagSet(DRAWFLAG_MASK_ALIGN_H, DRAWFLAG_ALIGN_H_RIGHT)) {
        *pX -= textWidth;
    }

    if (IsDrawFlagSet(DRAWFLAG_MASK_ALIGN_V, DRAWFLAG_ALIGN_V_CENTER)) {
        *pY -= textHeight / 2.0f;
    } else if (IsDrawFlagSet(DRAWFLAG_MASK_ALIGN_V, DRAWFLAG_ALIGN_V_TOP)) {
        *pY -= textHeight;
    }

    if (IsDrawFlagSet(DRAWFLAG_MASK_ALIGN_TEXT, DRAWFLAG_ALIGN_TEXT_CENTER)) {
        f32 adjusted = textWidth - CalcLineWidth(pStr, len);
        SetCursorX(*pX + adjusted / 2.0f);
    } else if (IsDrawFlagSet(DRAWFLAG_MASK_ALIGN_TEXT,
                             DRAWFLAG_ALIGN_TEXT_RIGHT)) {
        SetCursorX(*pX + (textWidth - CalcLineWidth(pStr, len)));
    } else {
        SetCursorX(*pX);
    }

    if (IsDrawFlagSet(DRAWFLAG_MASK_ALIGN_V,
                      DRAWFLAG_ALIGN_V_CENTER | DRAWFLAG_ALIGN_V_TOP)) {
        SetCursorY(*pY);
    } else {
        SetCursorY(*pY + GetFontAscent());
    }

    return textWidth;
}

template <typename T>
f32 TextWriterBase<T>::CalcStringWidth(const T* pStr, int len) const {
    Rect rect(0.0f, 0.0f, 0.0f, 0.0f);
    CalcStringRect(&rect, pStr, len);
    return rect.GetWidth();
}

template <typename T>
void TextWriterBase<T>::CalcStringRect(Rect* pRect, const T* pStr,
                                       int len) const {
    TextWriterBase<T> clone(*this);
    clone.CalcStringRectImpl(pRect, pStr, len);
}

// Member-level explicit instantiations -- retail ut_TextWriterBase.o emits
// exactly these 11 members (+ __sinit_), in this order. Whole-class
// `template struct TextWriterBase<T>;` drags in both twins' full member sets
// (VPrintf/CalcStringRect/GetCharSpace/IsDrawFlagSet/...) and overruns the
// 0x2054 split by 0x2C28.
template TextWriterBase<char>::TextWriterBase();

template f32 TextWriterBase<char>::GetLineHeight() const;

template TextWriterBase<wchar_t>::TextWriterBase();
template TextWriterBase<wchar_t>::~TextWriterBase();
template f32 TextWriterBase<wchar_t>::GetLineHeight() const;
template f32 TextWriterBase<wchar_t>::CalcStringWidth(const wchar_t* pStr,
                                                     int len) const;
template f32 TextWriterBase<wchar_t>::Print(const wchar_t* pStr, int len);
template f32 TextWriterBase<wchar_t>::PrintMutable(const wchar_t* pStr,
                                                   int len);
template bool TextWriterBase<wchar_t>::CalcLineRectImpl(Rect* pRect,
                                                       const wchar_t** ppStr,
                                                       int len);
template void TextWriterBase<wchar_t>::CalcStringRectImpl(
    Rect* pRect, const wchar_t* pStr, int len);
template f32 TextWriterBase<wchar_t>::PrintImpl(const wchar_t* pStr, int len,
                                               bool bMutable);
template f32 TextWriterBase<wchar_t>::AdjustCursor(f32* pX, f32* pY,
                                                  const wchar_t* pStr,
                                                  int len);

} // namespace ut
} // namespace nw4r
