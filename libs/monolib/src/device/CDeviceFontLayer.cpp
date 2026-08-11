// CDeviceFontLayer - font layer work object.
//
// Holds a queue of text draw commands (reslist at 0x1c8) that are replayed
// into an embedded CDrawGX at 0x220. func_80453D78 measures the widest line
// of a string; func_804546C8 renders a string as a glyph grid; func_80454B70
// replays the queued commands.
//
// NOTE: the retail symbol-map entries for the three functions below have
// decompiler-guessed parameter fragments (Fv / FP7CDrawGX) that do not match
// the real C++ mangling of their signatures, so they are defined as extern
// "C" with the exact retail names. Same for the func_80452C10 import, which
// retail callers invoke with a single argument (the callee never reads the
// second).

#include <harness_catalog.h>
#include <string.h>
#include <revolution/GX.h>
#include "monolib/core/CDrawGX.hpp"
#include "monolib/math.hpp"

// ---------------------------------------------------------------------------
// Font-info provider vtable interface (impls: CDeviceFontInfoRom / Ext).
// Slots 0x8-0x20 as used by the layer renderer.
// ---------------------------------------------------------------------------
class IDeviceFontInfo {
public:
    virtual ~IDeviceFontInfo(); // 0x8
    virtual u32 GetIndex();     // 0xC
    virtual u32 GetGlyphWidth();  // 0x10
    virtual u32 GetGlyphHeight(); // 0x14
    virtual u32 GetGlyphFormat(); // 0x18
    virtual u32 GetTextLength();  // 0x1C
    virtual const char* GetFontTexture(const char* str, void** texOut,
                                       u32* xOut, u32* yOut,
                                       u32* widthOut); // 0x20
};

// Draw-command list node (reslist-style: next at 0x0, payload at 0x8).
struct CDeviceFontLayerCmdNode {
    CDeviceFontLayerCmdNode* mNext; // 0x0
    u32 mPad4;                      // 0x4
    struct CDeviceFontLayerCmdData* mData; // 0x8
};

// Draw-command payload.
struct CDeviceFontLayerCmdData {
    u16 mCmd;  // 0x0
    u16 mPad2; // 0x2
    s32 mArg0; // 0x4
    s32 mArg1; // 0x8
    s32 mArg2; // 0xC
    s32 mArg3; // 0x10
};

// Glyph texture lookup outputs (GetFontTexture). Packed so the four
// address-taken outputs occupy one 16-byte stack slot.
struct GlyphOut {
    void* tex; // 0x0
    u32 x;     // 0x4
    u32 y;     // 0x8
    u32 w;     // 0xC
};

// Layout mirror of CDrawGX 0x0-0x13 so the (private) color can be read.
struct CDrawGXColorLayout {
    u32 mFlags;          // 0x0
    ml::CCol4 mCol;      // 0x4
};

// ---------------------------------------------------------------------------
// Font layer object.
// ---------------------------------------------------------------------------
class CDeviceFontLayer {
public:
    // 0x0-0x1c8: base (vtable / work-thread state)
    u8 _base[0x1C8];                    // 0x0
    CDeviceFontLayerCmdNode* mCmdList;  // 0x1c8 (list head: next@0, prev@4)
    u8 pad_1CC[0x1D8 - 0x1CC];
    void* mCmdArray; // 0x1d8 (allocated command array)
    u32 mCmdCount;   // 0x1dc
    u8 pad_1E0[4];   // 0x1e0
    CDeviceFontLayerCmdNode* mCurNode; // 0x1e4
    u8 mDirty;                         // 0x1e8
    u8 pad_1E9[3];
    f32 mScaleX; // 0x1ec
    f32 mScaleY; // 0x1f0
    u32 mFontId; // 0x1f4
    u8 pad_1F8[4]; // 0x1f8
    u8 mFlag1FC;   // 0x1fc
    u8 pad_1FD[3];
    ml::CCol4 mColor;   // 0x200
    ml::CCol4 mBgColor; // 0x210
    CDrawGX mDrawGX;    // 0x220

    // Stub declarations retained for the catalog TU (matched separately).
    void func_80453BB4();
    void func_80453F78();
    void func_80453FF0();
    void func_804541F8();
    void func_8045438C();
    void func_80454508();
    void func_80454684();
    void func_80454DE4();
    void func_80454E2C();
    void func_80454E6C();
    u32 func_80454E78();
    void wkStandbyLogout();
    void wkUpdate();
};

// CDeviceFont::func_80452C10(u32) - font-info lookup by index. Declared with
// the exact retail name; callers pass only the index (see file header note).
extern "C" IDeviceFontInfo* func_80452C10__11CDeviceFontFUlPQ34nw4r3lyt6Layout(
    u32 index);

void __ct__CDeviceFontLayer(){}

void __dt___reslist_base_const_CDeviceFontLayer_LAYER_QUE(){}

void __dt__reslist_const_CDeviceFontLayer_LAYER_QUE(){}

// ---------------------------------------------------------------------------
// func_80453D78 (0x80457E48) - measure the widest line of a string.
//
// Walks the string line by line, measuring each line's accumulated glyph
// width (glyph width scaled by `scale`, plus 2px per glyph when fontId is
// nonzero) and returns the maximum. Measured lines are copied into a local
// buffer; the trailing (final) line is measured in place.
// ---------------------------------------------------------------------------
extern "C" u32 func_80453D78__16CDeviceFontLayerFv(const char* str,
                                                   u32 fontId, f32 scale) {
    char buf[0x400];
    u32 maxWidth = 0;
    u32 count = 0;
    u32 cur = 0;
    IDeviceFontInfo* info;

    while (str[count] != 0) {
        if (str[count] == '\n') {
            memcpy(buf, str, count);
            buf[count] = 0;
            const char* p = buf;
            info = func_80452C10__11CDeviceFontFUlPQ34nw4r3lyt6Layout(fontId);
            if (info == 0) {
                cur = 0;
            } else {
                cur = 0;
                GlyphOut g;
                while (*p != 0) {
                    p = info->GetFontTexture(p, &g.tex, &g.x, &g.y, &g.w);
                    if (fontId != 0) {
                        g.w += 2;
                    }
                    cur = (s32)((f32)cur + (f32)g.w * scale);
                }
            }
            if (maxWidth < cur) {
                maxWidth = cur;
            }
            str += count + 1;
            count = 0;
        } else {
            count++;
        }
    }

    info = func_80452C10__11CDeviceFontFUlPQ34nw4r3lyt6Layout(fontId);
    if (info == 0) {
        cur = 0;
    } else {
        cur = 0;
        GlyphOut g;
        while (*str != 0) {
            str = info->GetFontTexture(str, &g.tex, &g.x, &g.y, &g.w);
            if (fontId != 0) {
                g.w += 2;
            }
            cur = (s32)((f32)cur + (f32)g.w * scale);
        }
    }
    if (maxWidth < cur) {
        maxWidth = cur;
    }
    return maxWidth;
}

void CDeviceFontLayer::func_80453BB4() {}

void CDeviceFontLayer::func_80453F78() {}

void CDeviceFontLayer::func_80453FF0() {}

void CDeviceFontLayer::func_804541F8() {}

void CDeviceFontLayer::func_8045438C() {}

void CDeviceFontLayer::func_80454508() {}

void CDeviceFontLayer::func_80454684() {}

// ---------------------------------------------------------------------------
// func_804546C8 (0x804587C4) - render a string as a glyph grid.
//
// Draws the string at (x, y) with the given scales. When `col` is given and
// its alpha is not fully opaque, a measurement rect is drawn first. The text
// is rendered one line at a time (1 or 2 lines depending on `flag`), each
// glyph as a textured quad via the font-info provider.
// ---------------------------------------------------------------------------
extern "C" void func_804546C8__16CDeviceFontLayerFP7CDrawGX(
    CDrawGX* draw, u32 fontId, const char* str, s16 x, s16 y, f32 scaleX,
    f32 scaleY, const ml::CCol4* col, u8 flag) {
    IDeviceFontInfo* info =
        func_80452C10__11CDeviceFontFUlPQ34nw4r3lyt6Layout(fontId);
    if (info == 0) {
        return;
    }

    // Save the draw color; it is restored at the end.
    ml::CCol4 drawCol = ((CDrawGXColorLayout*)draw)->mCol;

    if (col != 0) {
        draw->setCol(*col);
        if (col->a != 1.0f) {
            // Semi-transparent text: draw a background rect sized by the
            // measured text width and the scaled line height.
            u32 w = func_80453D78__16CDeviceFontLayerFv(str, fontId, scaleX);
            IDeviceFontInfo* info2 =
                func_80452C10__11CDeviceFontFUlPQ34nw4r3lyt6Layout(fontId);
            u32 h;
            if (info2 == 0) {
                h = 0;
            } else {
                h = (u32)((f32)(u16)info2->GetTextLength() * scaleY);
            }
            ml::CRect16 rect(x, y, w, h);
            draw->renderRect(rect);
        }
    }

    u32 lastTex = 0;
    u32 textLen = (u16)info->GetTextLength();
    // Copy kept by retail (unused afterwards); mirrors the color layout.
    ml::CCol4 colCopy = drawCol;
    ml::CCol4 lineCol[2];
    lineCol[0] = drawCol;
    if (flag != 0) {
        lineCol[0].set(1.0f, 1.0f, 1.0f, drawCol.a);
        lineCol[1] = drawCol;
    }

    u32 lineCount = 1 + (flag != 0);
    ml::CCol4* pCol = lineCol;
    u32 lineIndex = 0;
    u32 advance;

    while (lineIndex < lineCount) {
        const char* curStr = str;
        draw->setCol(*pCol);
        advance = (lineCount - 1 - lineIndex) * 2;
        s16 lineY = y + advance;

        while (*curStr != 0) {
            void* tex;
            u32 xOut, yOut, w;
            curStr = info->GetFontTexture(curStr, &tex, &xOut, &yOut, &w);
            if (fontId != 0) {
                w += 2;
            }
            if (lastTex != (u32)tex) {
                lastTex = (u32)tex;
                u32 texW = info->GetGlyphWidth();
                u32 texH = info->GetGlyphHeight();
                u32 fmt = info->GetGlyphFormat();
                GXTexObj texObj;
                if (fmt >= GX_TF_C4 && fmt <= GX_TF_C14X2) {
                    u32 tlutIdx = 0;
                    if (fmt == GX_TF_C14X2) {
                        tlutIdx = GX_BIGTLUT0;
                    }
                    GXTlutObj tlut;
                    GXInitTlutObj(&tlut, NULL, GX_TL_IA8, 0);
                    GXLoadTlut(&tlut, tlutIdx);
                    GXInitTexObjCI(&texObj, tex, (u16)texW, (u16)texH,
                                   (GXTexFmt)fmt, GX_CLAMP, GX_CLAMP, GX_FALSE,
                                   1);
                } else {
                    GXInitTexObj(&texObj, tex, (u16)texW, (u16)texH,
                                 (GXTexFmt)fmt, GX_CLAMP, GX_CLAMP, GX_FALSE);
                }
                draw->setTex(&texObj, (u16)info->GetGlyphWidth(),
                             (u16)info->GetGlyphHeight());
            }

            ml::CRect16 rect2(xOut, yOut, w, textLen);
            ml::CRect16 rect1(x + advance, lineY,
                              (s32)((f32)w * scaleX),
                              (s32)((f32)textLen * scaleY));
            draw->begin(0x9, 0x1);
            draw->add(rect1, rect2);
            draw->end();
            advance = (s32)((f32)advance + (f32)w * scaleX);
        }
        pCol++;
        lineIndex++;
    }

    draw->setCol(drawCol);
}

// ---------------------------------------------------------------------------
// func_80454B70 (0x80458C60) - replay the queued draw commands.
//
// Walks the command list (from the current node when dirty), dispatching each
// command into the draw GX. When `flag` is nonzero the command list is
// cleared and the state reset afterward.
// ---------------------------------------------------------------------------
extern "C" void func_80454B70__16CDeviceFontLayerFv(CDeviceFontLayer* self,
                                                    u32 flag) {
    if (self->mCmdCount == 0) {
        return;
    }

    self->mDrawGX.func_80456570(0);
    self->mDrawGX.func_8045657C(0);

    CDeviceFontLayerCmdNode* node = self->mCmdList->mNext;
    if (self->mDirty) {
        node = self->mCurNode->mNext;
        self->mCurNode = node;
    }
    while (node != self->mCmdList) {
        u16 cmd = node->mData->mCmd;
        switch (cmd) {
        case 3:
            self->mScaleX = *(f32*)&node->mData->mArg0;
            self->mScaleY = *(f32*)&node->mData->mArg1;
            break;
        case 4:
            self->mDrawGX.setGXCacheId(node->mData->mArg0);
            break;
        case 1:
            self->mDrawGX.setCol(*(ml::CCol4*)&node->mData->mArg0);
            break;
        case 2:
            self->mColor = *(ml::CCol4*)&node->mData->mArg0;
            break;
        case 0:
            func_804546C8__16CDeviceFontLayerFP7CDrawGX(
                &self->mDrawGX, self->mFontId,
                (const char*)&node->mData->mArg1,
                (s16)node->mData->mArg0,
                (s16)(node->mData->mArg0 >> 16), self->mScaleX,
                self->mScaleY, &self->mColor, self->mFlag1FC);
            break;
        case 5:
            self->mFontId = node->mData->mArg0;
            break;
        case 6:
            self->mFlag1FC = node->mData->mArg0 != 0;
            break;
        }
        self->mCurNode = node;
        node = node->mNext;
    }

    self->mDirty = 1;
    if (flag != 0) {
        self->mColor.set(1.0f, 1.0f, 1.0f, 1.0f);
        self->mBgColor.set(0.0f, 0.0f, 0.0f, 0.0f);
        // Unlink every queued node and reset the list head to empty.
        CDeviceFontLayerCmdNode* n = self->mCmdList->mNext;
        while (n != self->mCmdList) {
            CDeviceFontLayerCmdNode* next = n->mNext;
            n->mNext = NULL;
            n = next;
        }
        self->mCmdList->mNext = self->mCmdList;
        self->mCmdList->mPad4 = (u32)self->mCmdList;
        self->mDrawGX.clear();
        self->mDrawGX.setFlag(0x10, true);
        self->mDirty = 0;
    }

    self->mDrawGX.func_80456570(1);
    self->mDrawGX.func_8045657C(1);
}

void CDeviceFontLayer::wkUpdate() {}

void CDeviceFontLayer::wkStandbyLogout() {}

void CDeviceFontLayer::func_80454DE4() {}

void CDeviceFontLayer::func_80454E2C() {}

extern u32 lbl_eu_80665694;
void CDeviceFontLayer::func_80454E6C() { lbl_eu_80665694 = 0; }

u32 CDeviceFontLayer::func_80454E78() { return 0x10000; }
