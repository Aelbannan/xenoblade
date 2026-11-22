// CDeviceFontLayer - font layer work object.
//
// Holds a queue of text draw commands (reslist at 0x1c4) that are replayed
// into an embedded CDrawGX at 0x220. measureTextWidth measures the widest line
// of a string; drawTextGrid renders a string as a glyph grid; replayQueue
// replays the queued commands.
//
// NOTE: the retail symbol-map entries for the three functions below have
// decompiler-guessed parameter fragments (Fv / FP7CDrawGX) that do not match
// the real C++ mangling of their signatures, so they are defined as extern
// "C" with the exact retail names. Same for the getFontInfo import, which
// retail callers invoke with a single argument (the callee never reads the
// second).

#include <harness_catalog.h>
#include <string.h>
#include <revolution/GX.h>
#include "monolib/device/CDeviceFontLayer.hpp"
#include "monolib/math.hpp"
#include "monolib/util/MemManager.hpp"
#include <new>
#include <decomp.h>

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
    virtual u16 GetTextLength();  // 0x1C (u16 return: retail callers see the
                                  // rlwinm normalization of a u16-returning
                                  // virtual at this slot)
    virtual const char* GetFontTexture(const char* str, void** texOut,
                                       u32* xOut, u32* yOut,
                                       u32* widthOut); // 0x20
    virtual void* GetSlot9();                         // 0x24 - dispatch-only
};

// Draw-command payload (replay view of a queued record).
struct CDeviceFontLayerCmdData;

// Draw-command list node (reslist-style: next at 0x0, payload at 0x8).
struct CDeviceFontLayerCmdNode {
    CDeviceFontLayerCmdNode* mNext; // 0x0
    u32 mPad4;                      // 0x4
    // volatile: retail re-loads the payload pointer in every switch case
    CDeviceFontLayerCmdData* volatile mData; // 0x8
};

// 8-byte fixed-size command header carved from the shared scratch buffer;
// records are addressed via the end-of-record cursor (header at [-1]).
struct SCRATCH_CMD {
    u16 mCmd;  // 0x0
    u16 mSize; // 0x2
    u32 mArg;  // 0x4
};

// 0xC-byte scale-command record as addressed via its end pointer.
struct ScaleCmd {
    u16 mCmd;  // 0x0
    u16 mSize; // 0x2
    f32 mSX;   // 0x4
    f32 mSY;   // 0x8
};

// 8-byte text-command header carved from the shared scratch buffer; the
// string payload follows the header inline.
struct TextQue {
    u16 mCmd;  // 0x0
    u16 mSize; // 0x2
    s16 mX;    // 0x4
    s16 mY;    // 0x6
};

// Draw-command payload (replay view of a queued record).
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

// Shared lazy reservation of the command-slot array (0x180 nodes x 0xC bytes
// + item padding); the node array is zeroed in blocks of 8 (retail unroll
// shape).
#define FONT_LAYER_RESERVE_SLOTS(self) \
    do { \
        if ((self)->mQueList.mCapacity == 0) \
            (self)->mQueList.reserve((self)->mAllocHandle, 0x180); \
    } while (0)

typedef CDeviceFontLayer::LAYER_QUE LAYER_QUE;
typedef _reslist_node<const LAYER_QUE*> LayerQueNode;

// CDeviceFont::getFontInfo(u32) - font-info lookup by index. Declared with
// the exact retail name; callers pass only the index (see file header note).
extern "C" IDeviceFontInfo* getFontInfo__11CDeviceFontFUlPQ34nw4r3lyt6Layout(
    u32 index);

// ---- CDeviceFontLayer constructor (0x80457908) ----
CDeviceFontLayer::CDeviceFontLayer(const char* pName, CWorkThread* pParent,
                                   int unk)
    : CWorkThread(pName, pParent, 0), mScaleX(1.0f), mScaleY(1.0f),
      mFontId(0), field_0x1F8(0), mFlag1FC(1) {
    mFlag2F0 = 0;
    mType = THREAD_CDEVICEFONTLAYER;
    mScaleY = mScaleX = 1.0f;
    mColor.set(0.0f, 0.0f, 0.0f, 0.0f);
    mBgColor.set(-1.0f, -1.0f, -1.0f, -1.0f);
    mQueList.clearList();
    mDrawGX.clear();
    mDrawGX.setFlag4();
    mDirty = 0;
}

// ---- CDeviceFontLayer destructor (0x80457BA4) ----
// Members (mDrawGX, then mQueList with its inlined _reslist_base teardown)
// and the CWorkThread base are destroyed implicitly.
CDeviceFontLayer::~CDeviceFontLayer() {}

// ---------------------------------------------------------------------------
// measureTextWidth (0x80457E48) - measure the widest line of a string.
//
// Walks the string line by line, measuring each line's accumulated glyph
// width (glyph width scaled by `scale`, plus 2px per glyph when fontId is
// nonzero) and returns the maximum. Measured lines are copied into a local
// buffer; the trailing (final) line is measured in place.
//
// Accumulation uses natural u32->double casts; MWCC lowers them to the
// retail 0x43300000/xoris magic-double sequence with its own frame-top
// temporaries and the shared .sdata2 bias constant.
// ---------------------------------------------------------------------------
extern "C" u32 measureTextWidth__16CDeviceFontLayerFv(const char* str,
                                                   u32 fontId, f32 scale) {
    char buf[0x400];
    u32 maxWidth = 0;
    u32 count = 0;
    u32 cur;
    IDeviceFontInfo* info;

    while (str[count] != 0) {
        if (str[count] == '\n') {
            memcpy(buf, str, count);
            buf[count] = '\0';
            const char* p = buf;
            info = getFontInfo__11CDeviceFontFUlPQ34nw4r3lyt6Layout(fontId);
            if (info == 0) {
                cur = 0;
            } else {
                cur = 0;
                void* tex;
                u32 gx, gy, gw;
                while (*p != 0) {
                    p = info->GetFontTexture(p, &tex, &gx, &gy, &gw);
                    if (fontId != 0) {
                        gw += 2;
                    }
                    cur = (u32)((f64)gw * scale + (f64)cur);
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

    info = getFontInfo__11CDeviceFontFUlPQ34nw4r3lyt6Layout(fontId);
    if (info == 0) {
        cur = 0;
    } else {
        cur = 0;
        void* tex;
        u32 gx, gy, gw;
        while (*str != 0) {
            str = info->GetFontTexture(str, &tex, &gx, &gy, &gw);
            if (fontId != 0) {
                gw += 2;
            }
            cur = (u32)((f64)gw * scale + (f64)cur);
        }
    }
    if (maxWidth < cur) {
        maxWidth = cur;
    }
    return maxWidth;
}

// Queue a text-draw command (cmd 0) with an inline string payload.
// No return value (retail leaves r3 untouched at the blr).
void queueText__16CDeviceFontLayerFv(CDeviceFontLayer* self, s16 x, s16 y,
                                         const char* str) {
    if (self->mQueList.mCapacity == 0)
        self->mQueList.reserve(self->mAllocHandle, 0x180);

    // Carve the record out of the shared scratch buffer: TextQue is exactly
    // 8 bytes, so end-1 addresses the record header (the retail shape keeps
    // the end-of-record cursor live and rebases to the start afterwards).
    u32 cursor = sFontCommandCursor + 8;
    TextQue* cmd = (TextQue*)((u8*)sFontCommandBuffer + cursor) - 1;
    sFontCommandCursor = cursor;
    cmd->mCmd = 0;
    cmd->mX = x;
    cmd->mY = y;

    // Inline string payload (with terminator).
    u32 len = strlen(str) + 1;
    u32 newCursor = sFontCommandCursor + len;
    sFontCommandCursor = newCursor;
    memcpy((u8*)sFontCommandBuffer + newCursor - len, str, len);

    // Align the end cursor up to 4 and record the total record size.
    int cur = sFontCommandCursor;
    int rem = cur & 3;
    if (rem != 0)
        cur = cur + 4 - rem;
    int total = (u8*)sFontCommandBuffer + cur - (u8*)cmd;
    sFontCommandCursor = cur;
    cmd->mSize = (u16)total;

    // Hand-inlined reslist<const LAYER_QUE*> append (retail walks the slot
    // array with a count + byte-offset pair, then links the node).
    int i;
    int capacity;
    LayerQueNode* startNode;
    startNode = self->mQueList.mStartNodePtr;
    capacity = self->mQueList.mCapacity;
    for (i = 0; i < capacity; i++) {
        if (self->mQueList.mList[i].mNext == NULL)
            break;
    }
    LayerQueNode* temp = self->mQueList.mList + i;
    const LAYER_QUE** ptr = &temp->mItem;
    if (ptr != NULL) {
        try {
            *ptr = (const LAYER_QUE*)cmd;
        } catch (...) {
            throw;
        }
    }
    temp->mNext = startNode;
    temp->mPrev = startNode->mPrev;
    startNode->mPrev->mNext = temp;
    startNode->mPrev = temp;
}

// Look up the font-info record for this layer's font id; when found, query
// its text length (u16), scale it by mScaleY and return the integer height.
// The float conversions follow the retail double-trick (u16->f32 via 2^52
// lfd) and __cvt_fp2unsigned for the final (u32) cast.
u32 CDeviceFontLayer::getScaledTextHeight() {
    IDeviceFontInfo* info = getFontInfo__11CDeviceFontFUlPQ34nw4r3lyt6Layout(
        mFontId);
    if (info == 0) {
        return 0;
    }
    u16 v = info->GetTextLength();
    return (u32)(mScaleY * (f32)v);
}

// Queue a background-color change unless the color already matches.
void CDeviceFontLayer::setBackgroundColor(const ml::CCol4* col) {
    bool same = this->mBgColor.r == col->r && this->mBgColor.g == col->g &&
                this->mBgColor.b == col->b && this->mBgColor.a == col->a;
    if (same)
        return;

    LAYER_QUE* cmd;
    FONT_LAYER_RESERVE_SLOTS(this);

    // Record addressed via its end pointer
    // end-relative), rebased to the start for the queue append.
    u32 cursor = sFontCommandCursor + 0x14;
    sFontCommandCursor = cursor;
    cmd = (LAYER_QUE*)((u8*)sFontCommandBuffer + cursor) - 1;
    cmd->mCmd = 1;
    cmd->mSize = 0x14;
    cmd->mCol = *col;

    // Hand-inlined reslist<const LAYER_QUE*> append (retail walks the slot
    // array with a count + byte-offset pair, then links the node).
    reslist<const LAYER_QUE*>* list = &this->mQueList;
    int i;
    int byteOff;
    int capacity;
    LayerQueNode* sentinel;
    sentinel = list->mStartNodePtr;
    capacity = list->mCapacity;
    for (i = 0, byteOff = 0; i < capacity; i++) {
        if (*(void**)((u8*)list->mList + byteOff) == NULL)
            break;
        byteOff += sizeof(LayerQueNode);
    }
    LayerQueNode* temp = &list->mList[i];
    const LAYER_QUE** ptr = &temp->mItem;
    if (ptr != NULL) {
        try {
            *ptr = cmd;
        } catch (...) {
            throw;
        }
    }
    temp->mNext = sentinel;
    temp->mPrev = sentinel->mPrev;
    sentinel->mPrev->mNext = temp;
    sentinel->mPrev = temp;

    this->mBgColor = *col;
}

// Queue a text-scale change;
// Single chained record-pointer expression: MWCC forms the end-of-record
// pointer, writes the leading fields end-relative, then rebases to the
// record start (subi) for the trailing field and the queue append.
void CDeviceFontLayer::setFontScale(f32 scaleX, f32 scaleY) {
    FONT_LAYER_RESERVE_SLOTS(this);

    u32 cursor = sFontCommandCursor + 0xc;
    sFontCommandCursor = cursor;
    LAYER_QUE* cmd = (LAYER_QUE*)((u8*)sFontCommandBuffer + cursor - 0xc);
    cmd->mCmd = 3;
    cmd->mSize = 0xc;
    cmd->mF[0] = scaleX;
    cmd->mF[1] = scaleY;

    reslist<const LAYER_QUE*>* list = &this->mQueList;
    int i;
    int byteOff;
    int capacity;
    LayerQueNode* sentinel;
    sentinel = list->mStartNodePtr;
    capacity = list->mCapacity;
    for (i = 0, byteOff = 0; i < capacity; i++) {
        if (*(void**)((u8*)list->mList + byteOff) == NULL)
            break;
        byteOff += sizeof(LayerQueNode);
    }
    LayerQueNode* temp = &list->mList[i];
    const LAYER_QUE** ptr = &temp->mItem;
    if (ptr != NULL) {
        try {
            *ptr = cmd;
        } catch (...) {
            throw;
        }
    }
    temp->mNext = sentinel;
    temp->mPrev = sentinel->mPrev;
    sentinel->mPrev->mNext = temp;
    sentinel->mPrev = temp;

    this->mScaleX = scaleX;
    this->mScaleY = scaleY;
}

// Queue a font-id change; the layer's own font id is updated immediately.
// The reslist append follows the CScnItemPool_pushItemToList/CDeviceVI recipe: hand-inlined
// push_back walk with explicit byteOff + comma-init so the register assignment
// matches retail (cmd r4, i r5, byteOff r6, capacity r7, sentinel r8).
void CDeviceFontLayer::setFontId(u32 arg) {
    FONT_LAYER_RESERVE_SLOTS(this);

    u32 cursor = sFontCommandCursor + 8;
    sFontCommandCursor = cursor;
    LAYER_QUE* cmd = (LAYER_QUE*)((u8*)sFontCommandBuffer + cursor - 8);
    cmd->mCmd = 5;
    cmd->mSize = 8;
    cmd->mArg[0] = arg;

    reslist<const LAYER_QUE*>* list = &this->mQueList;
    int i;
    int byteOff;
    int capacity;
    LayerQueNode* startNode;
    startNode = list->mStartNodePtr;
    capacity = list->mCapacity;
    for (i = 0, byteOff = 0; i < capacity; i++) {
        if (*(void**)((u8*)list->mList + byteOff) == NULL)
            break;
        byteOff += sizeof(LayerQueNode);
    }
    LayerQueNode* temp = &list->mList[i];
    const LAYER_QUE** ptr = &temp->mItem;
    if (ptr != NULL) {
        try {
            *ptr = cmd;
        } catch (...) {
            throw;
        }
    }
    temp->mNext = startNode;
    temp->mPrev = startNode->mPrev;
    startNode->mPrev->mNext = temp;
    startNode->mPrev = temp;

    this->mFontId = arg;
}

// ---------------------------------------------------------------------------
// queueLineMode (0x80458604) - queue a draw command.
//
// Lazily reserves the 0x180-node slot array (8-byte command records in the
// shared scratch buffer), then appends an {cmd=6, size=8, arg} record to the
// scratch cursor and links a queue node pointing at it (push_back).
// ---------------------------------------------------------------------------
extern "C" void queueLineMode__16CDeviceFontLayerFv(CDeviceFontLayer* self,
                                                    u32 arg) {
    // Lazily allocate the slot array via the shared reserve() helper.
    FONT_LAYER_RESERVE_SLOTS(self);

    // Append the command record at the scratch-buffer write cursor. The
    // record is addressed via the end-of-record pointer, one 8-byte header
    // back (retail keeps the end pointer live and stores at [-1]).
    u32 cursor = sFontCommandCursor + 8;
    sFontCommandCursor = cursor;
    LAYER_QUE* cmd = (LAYER_QUE*)((u8*)sFontCommandBuffer + cursor - 8);
    cmd->mCmd = 6;
    cmd->mSize = 8;
    cmd->mArg[0] = arg;

    self->mQueList.push_back(cmd);
}



// Font-info lookup + slot-9 dispatch. The retail symbol carries the
// decompiler-guessed Fv suffix; the body passes r4 (a font index) through to
// getFontInfo, so the real signature takes a u32.
extern "C" void* getFontSlotData__16CDeviceFontLayerFv(void* self, u32 index) {
    IDeviceFontInfo* info = getFontInfo__11CDeviceFontFUlPQ34nw4r3lyt6Layout(
        index);
    if (info == 0)
        return 0;
    return info->GetSlot9();
}

// ---------------------------------------------------------------------------
// drawTextGrid (0x804587C4) - render a string as a glyph grid.
//
// Draws the string at (x, y) with the given scales. When `col` is given and
// its alpha is not fully opaque, a measurement rect is drawn first. The text
// is rendered one line at a time (1 or 2 lines depending on `flag`), each
// glyph as a textured quad via the font-info provider.
// ---------------------------------------------------------------------------
extern "C" void drawTextGrid__16CDeviceFontLayerFP7CDrawGX(
    CDrawGX* draw, u32 fontId, const char* str, s16 x, s16 y, f32 scaleX,
    f32 scaleY, const ml::CCol4* col, u8 flag) {
    IDeviceFontInfo* info =
        getFontInfo__11CDeviceFontFUlPQ34nw4r3lyt6Layout(fontId);
    if (info == 0) {
        return;
    }

    // Save the draw color; it is restored at the end.
    ml::CCol4 drawCol = ((CDrawGXColorLayout*)draw)->mCol;

    if (col != 0) {
        draw->setCol(*col);
        if (1.0f != col->a) {
            // Semi-transparent text: draw a background rect sized by the
            // measured text width and the scaled line height.
            u32 w = measureTextWidth__16CDeviceFontLayerFv(str, fontId, scaleX);
            IDeviceFontInfo* info2 =
                getFontInfo__11CDeviceFontFUlPQ34nw4r3lyt6Layout(fontId);
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
// replayQueue (0x80458C60) - replay the queued draw commands.
//
// Walks the command list (from the current node when dirty), dispatching each
// command into the draw GX. When `flag` is nonzero the command list is
// cleared and the state reset afterward.
// ---------------------------------------------------------------------------
extern "C" void replayQueue__16CDeviceFontLayerFv(CDeviceFontLayer* self,
                                                    u32 flag) {
    if (self->mQueList.mCapacity == 0) {
        return;
    }

    self->mDrawGX.setZCompare(0);
    self->mDrawGX.setZWriteEnable(0);

    CDeviceFontLayerCmdNode* node =
        (CDeviceFontLayerCmdNode*)self->mQueList.mStartNodePtr->mNext;
    if (self->mDirty) {
        node = (CDeviceFontLayerCmdNode*)self->mCurNode->mNext;
        self->mCurNode = (LayerQueNode*)node;
    }
    while (node != (CDeviceFontLayerCmdNode*)self->mQueList.mStartNodePtr) {
        switch (node->mData->mCmd) {
        case 3: {
            CDeviceFontLayerCmdData* d = node->mData;
            self->mScaleX = *(f32*)&d->mArg0;
            self->mScaleY = *(f32*)&d->mArg1;
            break;
        }
        case 4:
            self->mDrawGX.setGXCacheId(node->mData->mArg0);
            break;
        case 1:
            self->mDrawGX.setCol(*(ml::CCol4*)&node->mData->mArg0);
            break;
        case 2:
            self->mColor = *(ml::CCol4*)&node->mData->mArg0;
            break;
        case 0: {
            CDeviceFontLayerCmdData* d = node->mData;
            drawTextGrid__16CDeviceFontLayerFP7CDrawGX(
                &self->mDrawGX, self->mFontId,
                (const char*)&d->mArg1,
                ((TextQue*)d)->mX,
                ((TextQue*)d)->mY, self->mScaleX,
                self->mScaleY, &self->mColor, self->mFlag1FC);
            break;
        }
        case 5:
            self->mFontId = node->mData->mArg0;
            break;
        case 6:
            self->mFlag1FC = node->mData->mArg0 != 0;
            break;
        }
        self->mCurNode = (LayerQueNode*)node;
        node = node->mNext;
    }

    self->mDirty = 1;
    if (flag != 0) {
        // Color reset uses opaque white and a -1.0f background; both
        // loads are hoisted above their store groups (retail shape), with

        f32 bg = -1.0f, col = 1.0f;
        self->mColor.set(col, col, col, col);
        self->mBgColor.set(bg, bg, bg, bg);
        // Unlink every queued node and reset the list ring to empty.
        LayerQueNode* n = self->mQueList.mStartNodePtr->mNext;
        while (n != self->mQueList.mStartNodePtr) {
            LayerQueNode* done = n;
            n = n->mNext;
            done->mNext = NULL;
        }
        self->mQueList.mStartNodePtr->mNext = self->mQueList.mStartNodePtr;
        self->mQueList.mStartNodePtr->mPrev = self->mQueList.mStartNodePtr;
        self->mDrawGX.clear();
        // Read-modify-write split so the flags load is scheduled before the
        // mDirty store (retail shape).
        CDrawGXColorLayout* gxLayout = (CDrawGXColorLayout*)&self->mDrawGX;
        u32 gxFlags = gxLayout->mFlags;
        self->mDirty = 0;
        gxLayout->mFlags = gxFlags | 0x10;
    }

    self->mDrawGX.setZCompare(1);
    self->mDrawGX.setZWriteEnable(1);
}

void CDeviceFontLayer::wkUpdate() {
    if (this->mFlag2F0 == 0)
        return;
    wkSetEvent(EVT_NONE);
}

bool CDeviceFontLayer::wkStandbyLogout() {
    if (mChildren.empty() && this->mFlag2F0 != 0)
        return CWorkThread::wkStandbyLogout();
    return false;
}

// Cross-unit helpers (retail mangled names; not included as C++ here).
extern "C" int getDevSys1Handle__7CDeviceFv();
extern "C" void* allocate_array_ex__Q23mtl10MemManagerFUlUli(u32 size,
                                                              u32 handle,
                                                              int align);

// Lazily allocate the font-layer scratch object once: if the global slot is
// still null, grab the device heap handle, allocate 0x10000 bytes aligned 16,
// and reset the adjacent word.
// The scratch buffer and its write cursor are defined at the top of this TU.

void CDeviceFontLayer::allocFontHeap() {
    if (sFontCommandBuffer != 0)
        return;
    int handle = getDevSys1Handle__7CDeviceFv();
    sFontCommandBuffer = allocate_array_ex__Q23mtl10MemManagerFUlUli(0x10000,
                                                                  handle, 16);
    sFontCommandCursor = 0;
}

void CDeviceFontLayer::freeFontHeap() {
    if (sFontCommandBuffer == 0)
        return;
    if (sFontCommandBuffer != 0) {
        operator delete[](sFontCommandBuffer);
        sFontCommandBuffer = 0;
    }
    sFontCommandCursor = 0;
}

void CDeviceFontLayer::resetFontCursor() { sFontCommandCursor = 0; }

u32 CDeviceFontLayer::getFontHeapSize() { return 0x10000; }

// .sbss 0x80665690 (0x8) - zero-initialized globals
void* sFontCommandBuffer;
u32 sFontCommandCursor;


