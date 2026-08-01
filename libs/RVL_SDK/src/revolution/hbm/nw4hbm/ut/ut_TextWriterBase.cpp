#include <nw4hbm/ut.h>

namespace nw4hbm {
namespace ut {

template <typename T> T* TextWriterBase<T>::mFormatBuffer = NULL;

template <typename T>
u32 TextWriterBase<T>::mFormatBufferSize = DEFAULT_FORMAT_BUFFER_SIZE;

template <typename T>
TagProcessorBase<T> TextWriterBase<T>::mDefaultTagProcessor;

template <typename T>
TextWriterBase<T>::TextWriterBase()
    : mCharSpace(0.0f),
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

template <typename T> void TextWriterBase<T>::SetLineHeight(f32 height) {
    const Font* pFont = GetFont();
    int lf = pFont != NULL ? pFont->GetLineFeed() : 0;
    mLineSpace = height - GetScaleV() * lf;
}

// Defined before the printing routines so MWCC can inline them (retail codegen
// inlines these accessors at every call site); the out-of-line bodies are still
// emitted here for the standalone retail symbols.
template <>
f32 TextWriterBase<char>::GetCharSpace() const {
    return mCharSpace;
}
template <>
f32 TextWriterBase<wchar_t>::GetCharSpace() const {
    return mCharSpace;
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
    Rect rect;
    TextWriterBase<T> clone(*this);

    clone.SetCursor(0.0f, 0.0f);
    clone.CalcLineRectImpl(&rect, pStr, len);

    return rect.GetWidth();
}

template <typename T>
int TextWriterBase<T>::CalcLineRectImpl(Rect* pRect, const T* pStr, int len) {
    const T* pStrBegin = pStr;

    PrintContext<T> context = { this, pStr };

    CharStrmReader reader = GetFont()->GetCharStrmReader();

    pRect->left = 0.0f;
    pRect->right = 0.0f;

    f32 x = 0.0f;
    bool charSpace = false;

    pRect->top = Min(0.0f, GetLineHeight());
    pRect->bottom = Max(0.0f, GetLineHeight());

    reader.Set(pStrBegin);
    u16 ch = reader.Next();

    while (static_cast<const T*>(reader.GetCurrentPos()) - pStrBegin <= len) {
        if (ch < ' ') {
            Rect r(x, 0.0f, 0.0f, 0.0f);
            context.str = static_cast<const T*>(reader.GetCurrentPos());
            context.flags = charSpace ? 0 : PrintContext<T>::FLAGS_CHARSPACE;

            SetCursorX(x);

            typename TagProcessorType::Operation oper =
                mTagProcessor->CalcRect(&r, ch, &context);

            reader.Set(context.str);

            pRect->left = Min(pRect->left, r.left);
            pRect->top = Min(pRect->top, r.top);
            pRect->right = Max(pRect->right, r.right);
            pRect->bottom = Max(pRect->bottom, r.bottom);

            x = GetCursorX();

            if (oper == TagProcessorType::OPERATION_END_DRAW) {
                return len;
            }

            if (oper == TagProcessorType::OPERATION_NO_CHAR_SPACE) {
                charSpace = false;
            } else if (oper == TagProcessorType::OPERATION_CHAR_SPACE) {
                charSpace = true;
            } else if (oper == TagProcessorType::OPERATION_NEXT_LINE) {
                break;
            }
        } else {
            if (charSpace) {
                x += mCharSpace;
            }

            charSpace = true;

            if (IsWidthFixed()) {
                x += GetFixedWidth();
            } else {
                x += GetFont()->GetCharWidth(ch) * GetScaleH();
            }

            pRect->left = Min(pRect->left, x);
            pRect->right = Max(pRect->right, x);
        }

        ch = reader.Next();
    }

    return static_cast<const T*>(reader.GetCurrentPos()) - pStrBegin;
}

template <typename T>
void TextWriterBase<T>::CalcStringRectImpl(Rect* pRect, const T* pStr,
                                           int len) {
    int remain = len;

    pRect->left = 0.0f;
    pRect->right = 0.0f;
    pRect->top = 0.0f;
    pRect->bottom = 0.0f;

    SetCursor(0.0f, 0.0f);

    do {
        Rect r;
        int consumed = CalcLineRectImpl(&r, pStr, remain);
        pStr += consumed;
        remain -= consumed;

        pRect->left = Min(pRect->left, r.left);
        pRect->top = Min(pRect->top, r.top);
        pRect->right = Max(pRect->right, r.right);
        pRect->bottom = Max(pRect->bottom, r.bottom);
    } while (remain > 0);
}

template <typename T> f32 TextWriterBase<T>::PrintImpl(const T* pStr, int len, bool bMutable) {
    f32 cursorX = GetCursorX();
    f32 cursorY = GetCursorY();

    bool useLimit = NW4R_MATH_FLT_MAX < NW4R_MATH_FLT_MAX;

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
                Rect rect;

                context2.writer = &clone;
                oper = mTagProcessor->CalcRect(&rect, ch, &context2);

                if (rect.GetWidth() > 0.0f &&
                    clone.GetCursorX() - context.x > NW4R_MATH_FLT_MAX) {
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

                    f32 offset = (textWidth - width) / 2.0f;
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
                                : GetFont()->GetCharWidth(ch) * GetScaleH();

                if (baseX - cursorX + space + width > NW4R_MATH_FLT_MAX) {
                    ch = '\n';
                    reader.Set(pPrevStream);
                    continue;
                }
            }

            if (charSpace) {
                MoveCursorX(GetCharSpace());
            }

            charSpace = true;

            f32 adj = -GetFont()->GetBaselinePos() * GetScaleV();
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

        Rect rect;
        CalcStringRect(&rect, pStr, len);

        textWidth = rect.left + rect.right;
        textHeight = rect.top + rect.bottom;
    }

    if (IsDrawFlagSet(DRAWFLAG_MASK_ALIGN_H, DRAWFLAG_ALIGN_H_CENTER)) {
        *pX -= textWidth / 2;
    } else if (IsDrawFlagSet(DRAWFLAG_MASK_ALIGN_H, DRAWFLAG_ALIGN_H_RIGHT)) {
        *pX -= textWidth;
    }

    if (IsDrawFlagSet(DRAWFLAG_MASK_ALIGN_V, DRAWFLAG_ALIGN_V_CENTER)) {
        *pY -= textHeight / 2;
    } else if (IsDrawFlagSet(DRAWFLAG_MASK_ALIGN_V, DRAWFLAG_ALIGN_V_TOP)) {
        *pY -= textHeight;
    }

    if (IsDrawFlagSet(DRAWFLAG_MASK_ALIGN_TEXT, DRAWFLAG_ALIGN_TEXT_CENTER)) {
        SetCursorX(*pX + (textWidth - CalcLineWidth(pStr, len)) / 2);
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
int TextWriterBase<T>::GetTabWidth() const {
    return *(int*)((u8*)this + 84);
}

template <typename T>
f32 TextWriterBase<T>::CalcStringWidth(const T* pStr, int len) const {
    Rect rect;
    CalcStringRect(&rect, pStr, len);
    return rect.GetWidth();
}

template <typename T>
void TextWriterBase<T>::CalcStringRect(Rect* pRect, const T* pStr,
                                       int len) const {
    TextWriterBase<T> clone(*this);
    clone.CalcStringRectImpl(pRect, pStr, len);
}

template <typename T> T* TextWriterBase<T>::SetBuffer(u32 size) {
    T* pOldBuffer = mFormatBuffer;
    mFormatBufferSize = size;
    mFormatBuffer = NULL;
    return pOldBuffer;
}

// Explicit template instantiations for getter/setter functions.
// These are defined out-of-class to force MWCC to emit out-of-line
// copies (the inline definitions in the header are not always emitted
// on explicit instantiation).

template <>
void TextWriterBase<char>::SetLineSpace(f32 space) {
    mLineSpace = space;
}
template <>
void TextWriterBase<char>::SetCharSpace(f32 space) {
    mCharSpace = space;
}
template <>
f32 TextWriterBase<char>::GetLineSpace() const {
    return mLineSpace;
}
template <>
void TextWriterBase<char>::SetTabWidth(int width) {
    mTabWidth = width;
}
template <>
void TextWriterBase<char>::SetDrawFlag(u32 flag) {
    mDrawFlag = flag;
}
template <>
u32 TextWriterBase<char>::GetDrawFlag() const {
    return mDrawFlag;
}
template <>
void TextWriterBase<char>::SetTagProcessor(TagProcessorBase<char>* pProcessor) {
    mTagProcessor = pProcessor;
}
template <>
TagProcessorBase<char>* TextWriterBase<char>::GetTagProcessor() const {
    return mTagProcessor;
}

template <>
void TextWriterBase<wchar_t>::SetLineSpace(f32 space) {
    mLineSpace = space;
}
template <>
void TextWriterBase<wchar_t>::SetCharSpace(f32 space) {
    mCharSpace = space;
}
template <>
f32 TextWriterBase<wchar_t>::GetLineSpace() const {
    return mLineSpace;
}
template <>
void TextWriterBase<wchar_t>::SetTabWidth(int width) {
    mTabWidth = width;
}
template <>
void TextWriterBase<wchar_t>::SetDrawFlag(u32 flag) {
    mDrawFlag = flag;
}
template <>
u32 TextWriterBase<wchar_t>::GetDrawFlag() const {
    return mDrawFlag;
}
template <>
void TextWriterBase<wchar_t>::SetTagProcessor(TagProcessorBase<wchar_t>* pProcessor) {
    mTagProcessor = pProcessor;
}
template <>
TagProcessorBase<wchar_t>* TextWriterBase<wchar_t>::GetTagProcessor() const {
    return mTagProcessor;
}

template <>
void TextWriterBase<char>::ResetTagProcessor() {
    mTagProcessor = &mDefaultTagProcessor;
}

template <>
char* TextWriterBase<char>::SetBuffer(char* pBuffer, u32 size) {
    char* pOldBuffer = mFormatBuffer;
    mFormatBuffer = pBuffer;
    mFormatBufferSize = size;
    return pOldBuffer;
}

template <>
char* TextWriterBase<char>::GetBuffer() {
    return mFormatBuffer;
}

template <>
u32 TextWriterBase<char>::GetBufferSize() {
    return mFormatBufferSize;
}

template <>
void TextWriterBase<wchar_t>::ResetTagProcessor() {
    mTagProcessor = &mDefaultTagProcessor;
}

template <>
wchar_t* TextWriterBase<wchar_t>::SetBuffer(wchar_t* pBuffer, u32 size) {
    wchar_t* pOldBuffer = mFormatBuffer;
    mFormatBuffer = pBuffer;
    mFormatBufferSize = size;
    return pOldBuffer;
}

template <>
wchar_t* TextWriterBase<wchar_t>::GetBuffer() {
    return mFormatBuffer;
}

template <>
u32 TextWriterBase<wchar_t>::GetBufferSize() {
    return mFormatBufferSize;
}

template struct TextWriterBase<char>;
template struct TextWriterBase<wchar_t>;

} // namespace ut
} // namespace nw4hbm
