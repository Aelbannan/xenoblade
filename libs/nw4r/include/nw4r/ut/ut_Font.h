#ifndef NW4R_UT_FONT_H
#define NW4R_UT_FONT_H
#include <nw4r/types_nw4r.h>

#include <nw4r/ut/ut_CharStrmReader.h>

#include <revolution/GX.h>

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
    s8 left;       // at 0x0
    u8 glyphWidth; // at 0x1
    s8 charWidth;  // at 0x2
};

struct Glyph {
    void* pTexture;     // at 0x0
    CharWidths widths;  // at 0x4
    u8 height;          // at 0x7
    GXTexFmt texFormat; // at 0x8
    u16 texWidth;       // at 0xC
    u16 texHeight;      // at 0xE
    u16 cellX;          // at 0x10
    u16 cellY;          // at 0x12
};

/******************************************************************************
 *
 * Font
 *
 ******************************************************************************/
// Retail vtable data (nw4r_data.s __vt__Q34nw4r2ut4Font, .data 0x8056AEC8) and
// the shared default-reader member-pointer pool entry (lbl_eu_8056AF28 =
// {0,-1,&ReadNextCharCP1252}). Font is __declspec(novtable) so TUs emit no
// local vtable; the inline ctor assigns the retail vtable label explicitly
// and copies the default reader from the shared pool (MWCC_CASES
// "Retail-owned vtable data").
// lbl_eu_8056AE60 is a content-equal second copy of that pool entry;
// retail RomFont's ctor reads its default reader from this one.
extern "C" void* __vt__Q34nw4r2ut4Font[];
extern "C" CharStrmReader::ReadFunc lbl_eu_8056AF28;
extern "C" CharStrmReader::ReadFunc lbl_eu_8056AE60;

class __declspec(novtable) Font {
public:
    enum Type { TYPE_NULL, TYPE_ROM, TYPE_RESOURCE, TYPE_PAIR };

public:
    Font() {
        *(void**)this = (void*)__vt__Q34nw4r2ut4Font;
        mReadFunc = lbl_eu_8056AF28;
    }
    // RomFont's retail ctor copies its default reader from the second
    // content-equal pool copy (lbl_eu_8056AE60); pass that pool by address.
    explicit Font(CharStrmReader::ReadFunc* pReadFunc) {
        *(void**)this = (void*)__vt__Q34nw4r2ut4Font;
        mReadFunc = *pReadFunc;
    }
    virtual ~Font() {} // at 0x8

    virtual int GetWidth() const = 0;  // at 0xC
    virtual int GetHeight() const = 0; // at 0x10

    virtual int GetAscent() const = 0;      // at 0x14
    virtual int GetDescent() const = 0;     // at 0x18
    virtual int GetBaselinePos() const = 0; // at 0x1C

    virtual int GetCellHeight() const = 0;   // at 0x20
    virtual int GetCellWidth() const = 0;    // at 0x24
    virtual int GetMaxCharWidth() const = 0; // at 0x28

    virtual Type GetType() const = 0;              // at 0x2C
    virtual GXTexFmt GetTextureFormat() const = 0; // at 0x30
    virtual int GetLineFeed() const = 0;           // at 0x34

    virtual CharWidths GetDefaultCharWidths() const = 0;              // at 0x38
    virtual void SetDefaultCharWidths(const CharWidths& rWidths) = 0; // at 0x3C

    virtual bool SetAlternateChar(u16 ch) = 0; // at 0x40
    virtual void SetLineFeed(int lf) = 0;      // at 0x44

    virtual int GetCharWidth(u16 ch) const = 0;             // at 0x48
    virtual CharWidths GetCharWidths(u16 ch) const = 0;     // at 0x4C
    virtual void GetGlyph(Glyph* pGlyph, u16 ch) const = 0; // at 0x50
    virtual bool HasGlyph(u16 ch) const = 0;                // at 0x54
    virtual FontEncoding GetEncoding() const = 0;           // at 0x58

    void InitReaderFunc(FontEncoding encode);

    CharStrmReader GetCharStrmReader() const {
        return CharStrmReader(mReadFunc);
    }

private:
    CharStrmReader::ReadFunc mReadFunc; // at 0x4
};

} // namespace ut
} // namespace nw4r

#endif
