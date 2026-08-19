#include <nw4r/ut.h>

// Retail .sbss data owned by this TU (monolibdata1 blob dissolve; the auto
// __sinit__ below constructs the tag-processor objects and reads the guards):
//   lbl_eu_80665560 (.sbss 0x4) = TextWriterBase<char>::mDefaultTagProcessor
//                                 (TagProcessorBase<char>, vptr storage)
//   lbl_eu_80665564 (.sbss 0x4) = TextWriterBase<wchar_t>::mDefaultTagProcessor
//                                 (TagProcessorBase<wchar_t>, vptr storage)
//   lbl_eu_80665568 (.sbss 0x1) = __sinit__ init guard (char)
//   lbl_eu_80665569 (.sbss 0x7) = __sinit__ init guard (wchar_t) — retail
//                                 recovered symbol size is 7 (1 used byte)
// The template static members below stay defined so MWCC emits the retail
// 0x84 __sinit_ (guarded construction + __register_global_object); the raw
// definitions here are the inert storage the coordinator's reloc retargets
// point the __sinit__ at once the blob ranges move into this TU. u8 arrays
// (not the class type) keep MWCC from auto-constructing a second copy.
extern "C" {
    u8 lbl_eu_80665560[4];
    u8 lbl_eu_80665564[4];
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

namespace nw4r {
namespace ut {

// Shared nw4r .sdata2 float constants (retail ut_TextWriterBase.o has .sdata2
// size 0 -- every constant is referenced from the shared pool, no TU-local
// copies).
extern f32 lbl_eu_8066A160;  // 0x7F7FFFFF (FLT_MAX)
extern f32 lbl_eu_8066A164;  // lbl_eu_8066A164
extern f64 lbl_eu_8066A168;  // 0x4330000080000000 (signed int->f32 magic)
extern f32 lbl_eu_8066A170;  // 0.5f

// int -> f32 conversion matching retail (MWCC_CASES 7i): build the 2^52+x
// double on the stack (low word = x ^ 0x80000000, high word = 0x43300000) and
// subtract the shared signed magic. Statement order matters: the value word
// first, then 0x43300000.
inline f32 ConvF32S(s32 v) {
    union { f64 d; u32 w[2]; } u;
    u.w[1] = (u32)v ^ 0x80000000;
    u.w[0] = 0x43300000;
    return (f32)(u.d - lbl_eu_8066A168);
}

// The retail .sbss slots lbl_eu_80665560 / lbl_eu_80665564 ARE the
// mDefaultTagProcessor storage (TagProcessorBase<T> is vptr-only, 4 bytes; the
// retail __sinit_ constructs them in place). The template static member is
// declared in the header but NOT defined here -- the ctor binds to the retail
// slot instead, and mFormatBuffer / mFormatBufferSize are not defined either
// (retail ut_TextWriterBase.o has .sdata/.bss size 0; the FormatV/VPrintf
// family that references them is not part of the retail nw4r split).
template <typename T> TagProcessorBase<T>* GetDefaultTagProcessor();
template <> TagProcessorBase<char>* GetDefaultTagProcessor<char>() {
    return (TagProcessorBase<char>*)&lbl_eu_80665560;
}
template <> TagProcessorBase<wchar_t>* GetDefaultTagProcessor<wchar_t>() {
    return (TagProcessorBase<wchar_t>*)&lbl_eu_80665564;
}

template <typename T>
TextWriterBase<T>::TextWriterBase()
    : mCharSpace(lbl_eu_8066A164),
      mWidthLimit(lbl_eu_8066A160),
      mLineSpace(lbl_eu_8066A164),
      mTabWidth(4),
      mDrawFlag(0),
      mTagProcessor(GetDefaultTagProcessor<T>()) {}

template <typename T> TextWriterBase<T>::~TextWriterBase() {}

template <typename T> f32 TextWriterBase<T>::GetLineHeight() const {
    const Font* pFont = GetFont();
    int lf = pFont != NULL ? pFont->GetLineFeed() : 0;
    return mLineSpace + GetScaleV() * ConvF32S(lf);
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
    Rect rect(lbl_eu_8066A164, lbl_eu_8066A164, lbl_eu_8066A164, lbl_eu_8066A164);
    TextWriterBase<T> clone(*this);

    clone.SetCursor(lbl_eu_8066A164, lbl_eu_8066A164);
    clone.CalcLineRectImpl(&rect, &pStr, len);

    return rect.GetWidth();
}

template <typename T>
bool TextWriterBase<T>::CalcLineRectImpl(Rect* pRect, const T** ppStr,
                                         int len) {
    const T* pStrBegin = *ppStr;
    const T* pStrEnd = pStrBegin + len;
    bool useLimit = mWidthLimit < lbl_eu_8066A160;

    PrintContext<T> context = {
        this,     // writer
        pStrBegin // str
    };

    f32 x = lbl_eu_8066A164;
    bool charSpace = false;
    bool overLimit = false;

    const T* pPrevStream = NULL;
    Rect prevRect(lbl_eu_8066A164, lbl_eu_8066A164, lbl_eu_8066A164, lbl_eu_8066A164);

    CharStrmReader reader = GetFont()->GetCharStrmReader();

    pRect->left = lbl_eu_8066A164;
    pRect->right = lbl_eu_8066A164;
    pRect->top = Min(lbl_eu_8066A164, GetLineHeight());
    pRect->bottom = Max(lbl_eu_8066A164, GetLineHeight());
    prevRect = *pRect;

    reader.Set(pStrBegin);
    pPrevStream = NULL;

    u16 ch = reader.Next();

    while (static_cast<const T*>(reader.GetCurrentPos()) <= pStrEnd) {
        if (ch < ' ') {
            Rect r(x, lbl_eu_8066A164, lbl_eu_8066A164, lbl_eu_8066A164);
            context.str = static_cast<const T*>(reader.GetCurrentPos());
            context.flags = charSpace ? 0 : PrintContext<T>::FLAGS_CHARSPACE;

            SetCursorX(x);

            if (useLimit && ch != '\n' && pPrevStream != NULL) {
                PrintContext<T> context2 = context;
                TextWriterBase<T> clone(*this);

                Rect r(lbl_eu_8066A164, lbl_eu_8066A164, lbl_eu_8066A164, lbl_eu_8066A164);
                context2.writer = &clone;
                mTagProcessor->CalcRect(&r, ch, &context2);

                if (r.GetWidth() > lbl_eu_8066A164 &&
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
            f32 dx = lbl_eu_8066A164;

            if (charSpace) {
                dx += GetCharSpace();
            }

            if (IsWidthFixed()) {
                dx += GetFixedWidth();
            } else {
                dx += ConvF32S(GetFont()->GetCharWidth(ch)) * GetScaleH();
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

    pRect->left = lbl_eu_8066A164;
    pRect->right = lbl_eu_8066A164;
    pRect->top = lbl_eu_8066A164;
    pRect->bottom = lbl_eu_8066A164;

    SetCursor(lbl_eu_8066A164, lbl_eu_8066A164);

    do {
        Rect r(lbl_eu_8066A164, lbl_eu_8066A164, lbl_eu_8066A164, lbl_eu_8066A164);
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

    bool useLimit = mWidthLimit < lbl_eu_8066A160;

    f32 orgCursorX = cursorX;
    f32 orgCursorY = cursorY;

    f32 cursorXAdj = lbl_eu_8066A164;
    f32 cursorYAdj = lbl_eu_8066A164;

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
                Rect rect(lbl_eu_8066A164, lbl_eu_8066A164, lbl_eu_8066A164, lbl_eu_8066A164);

                context2.writer = &clone;
                oper = mTagProcessor->CalcRect(&rect, ch, &context2);

                if (rect.GetWidth() > lbl_eu_8066A164 &&
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

                    f32 offset = (textWidth - width) * lbl_eu_8066A170;
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
                f32 space = charSpace ? GetCharSpace() : lbl_eu_8066A164;

                f32 width = IsWidthFixed()
                                ? GetFixedWidth()
                                : ConvF32S(GetFont()->GetCharWidth(ch)) *
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

            f32 adj = -ConvF32S(GetFont()->GetBaselinePos()) * GetScaleV();
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
    f32 textWidth = lbl_eu_8066A164;
    f32 textHeight = lbl_eu_8066A164;

    if (!IsDrawFlagSet(DRAWFLAG_MASK_ALL, DRAWFLAG_MASK_ALIGN_V) &&
        !IsDrawFlagSet(DRAWFLAG_MASK_ALL, 0)) {

        Rect rect(lbl_eu_8066A164, lbl_eu_8066A164, lbl_eu_8066A164, lbl_eu_8066A164);
        CalcStringRect(&rect, pStr, len);

        textWidth = rect.left + rect.right;
        textHeight = rect.top + rect.bottom;
    }

    if (IsDrawFlagSet(DRAWFLAG_MASK_ALIGN_H, DRAWFLAG_ALIGN_H_CENTER)) {
        *pX -= textWidth * lbl_eu_8066A170;
    } else if (IsDrawFlagSet(DRAWFLAG_MASK_ALIGN_H, DRAWFLAG_ALIGN_H_RIGHT)) {
        *pX -= textWidth;
    }

    if (IsDrawFlagSet(DRAWFLAG_MASK_ALIGN_V, DRAWFLAG_ALIGN_V_CENTER)) {
        *pY -= textHeight * lbl_eu_8066A170;
    } else if (IsDrawFlagSet(DRAWFLAG_MASK_ALIGN_V, DRAWFLAG_ALIGN_V_TOP)) {
        *pY -= textHeight;
    }

    if (IsDrawFlagSet(DRAWFLAG_MASK_ALIGN_TEXT, DRAWFLAG_ALIGN_TEXT_CENTER)) {
        SetCursorX(*pX + (textWidth - CalcLineWidth(pStr, len)) * lbl_eu_8066A170);
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
    Rect rect(lbl_eu_8066A164, lbl_eu_8066A164, lbl_eu_8066A164, lbl_eu_8066A164);
    CalcStringRect(&rect, pStr, len);
    return rect.GetWidth();
}

template <typename T>
void TextWriterBase<T>::CalcStringRect(Rect* pRect, const T* pStr,
                                       int len) const {
    TextWriterBase<T> clone(*this);
    clone.CalcStringRectImpl(pRect, pStr, len);
}

template struct TextWriterBase<char>;
template struct TextWriterBase<wchar_t>;

} // namespace ut
} // namespace nw4r
