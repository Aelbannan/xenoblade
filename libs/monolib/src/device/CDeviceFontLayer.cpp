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
// "C" with the exact retail names. Same for the getFontInfo import, which
// retail callers invoke with a single argument (the callee never reads the
// second).

#include <harness_catalog.h>
#include <string.h>
#include <revolution/GX.h>
#include "monolib/core/CDrawGX.hpp"
#include "monolib/math.hpp"
#include "monolib/util/MemManager.hpp"
#include <new>
#include <decomp.h>

// ---------------------------------------------------------------------------
// Dissolved retail data forward decls
// Retail bytes/relocs dumped from build/us/asm/monolib/src/device/CDeviceFontLayer.s
// .rodata 0x80522E40 size 0x78 align 8, .data 0x8056C7D0 size 0xD8 align 8,
// .sdata 0x80663730 size 0x18 align 8, .sbss 0x80665690 size 0x8 align 8
// Emitted as extern "C" u32 arrays with (u32)&extern, rodata align, novtable.
// ---------------------------------------------------------------------------
extern "C" {
    extern const char lbl_eu_80522E40[];
    extern const char lbl_eu_80522E54[];
    extern const char lbl_eu_80522E84[];
    extern u32 lbl_eu_80663730[];
    extern u32 lbl_eu_80663738[];
    extern u32 lbl_eu_80663740[];
    extern u32 lbl_eu_8056C7D0[];
    extern u32 lbl_eu_8056C870[];
    extern u32 lbl_eu_8056C884[];
    extern u32 lbl_eu_8056C890[];
    extern u32 lbl_eu_8056C89C[];
    extern void* lbl_eu_80665690;
    extern u32 lbl_eu_80665694;
    extern u32 __RTTI__10IWorkEvent[];
    extern u32 __RTTI__11CWorkThread[];
    extern void __dt__16CDeviceFontLayerFv();
    extern void __ct__7CDrawGXFv(void* self);
    extern void clear__7CDrawGXFv(void* self);
    extern void WorkEvent1__10IWorkEventFPvPCc();
    extern void OnFileEvent__10IWorkEventFP10CEventFile();
    extern void WorkEvent3__10IWorkEventFPv();
    extern void WorkEvent4__10IWorkEventFv();
    extern void OnPauseTrigger__10IWorkEventFb();
    extern void WorkEvent6__10IWorkEventFv();
    extern void WorkEvent7__10IWorkEventFv();
    extern void WorkEvent8__10IWorkEventFv();
    extern void WorkEvent9__10IWorkEventFv();
    extern void WorkEvent10__10IWorkEventFv();
    extern void WorkEvent11__10IWorkEventFv();
    extern void WorkEvent12__10IWorkEventFv();
    extern void WorkEvent13__10IWorkEventFv();
    extern void WorkEvent14__10IWorkEventFv();
    extern void WorkEvent15__10IWorkEventFv();
    extern void WorkEvent16__10IWorkEventFv();
    extern void WorkEvent17__10IWorkEventFv();
    extern void WorkEvent18__10IWorkEventFv();
    extern void WorkEvent19__10IWorkEventFv();
    extern void WorkEvent20__10IWorkEventFv();
    extern void WorkEvent21__10IWorkEventFv();
    extern void WorkEvent22__10IWorkEventFv();
    extern void WorkEvent23__10IWorkEventFv();
    extern void WorkEvent24__10IWorkEventFv();
    extern void WorkEvent25__10IWorkEventFv();
    extern void WorkEvent26__10IWorkEventFv();
    extern void WorkEvent27__10IWorkEventFv();
    extern void WorkEvent28__10IWorkEventFv();
    extern void WorkEvent29__10IWorkEventFv();
    extern void WorkEvent30__10IWorkEventFv();
    extern void WorkEvent31__10IWorkEventFv();
    extern void wkUpdate__16CDeviceFontLayerFv();
    extern void wkRender__11CWorkThreadFv();
    extern void wkRenderAfter__11CWorkThreadFv();
    extern void wkStandbyLogin__11CWorkThreadFv();
    extern void wkStandbyLogout__16CDeviceFontLayerFv();
    extern void wkStandbyExceptionRetry__11CWorkThreadFUl();
    extern f32 lbl_eu_8066A408;
    extern f32 lbl_eu_8066A40C;
    extern f32 lbl_eu_8066A410;
    extern const f64 lbl_eu_8066A418; // 0x4330000080000000 int->double bias
}


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

// Queued draw-command record carved from the shared scratch buffer.
struct LAYER_QUE {
    u16 mCmd;   // 0x00
    u16 mSize;  // 0x02
    union {
        u32 mArg[4]; // 0x04
        f32 mF[4];   // 0x04
        ml::CCol4 mCol;
        struct { u16 mPad4; u16 mPad6; s16 mX; s16 mY; }; // 0x04
    };
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

// ---------------------------------------------------------------------------
// Font layer object.
// ---------------------------------------------------------------------------
// Shared lazy reservation of the command-slot array (0x180 nodes x 0xC bytes
// + item padding); the node array is zeroed in blocks of 8 (retail unroll
// shape).
#define FONT_LAYER_RESERVE_SLOTS(self) \
    do { \
        if ((self)->mQueList.mCapacity == 0) \
            (self)->mQueList.reserve((self)->mAllocHandle); \
    } while (0)
// reslist node (next@0, prev@4) as embedded in the work-thread base.
struct CWorkThreadNode {
    CWorkThreadNode* mNext; // 0x0
    CWorkThreadNode* mPrev; // 0x4
};

// reslist<CWorkThread*> view at +0x5C of the work-thread base: a pointer to
// the embedded start node; the list is empty when the start node's mNext
// cycles back to the start node itself.
struct CWorkThreadChildren {
    void* mVtbl;                     // 0x5C
    CWorkThreadNode* mStartNodePtr;  // 0x60
    CWorkThreadNode mStartNode;      // 0x64 (mNext@0x64, mPrev@0x68)
    void* mList;                     // 0x6C
    int mCapacity;                   // 0x70
    bool mUnk1C;                     // 0x74
    u8 _pad[7];                      // pad to the reslist's 0x20 size

    bool empty() const {
        return mStartNodePtr->mNext == mStartNodePtr;
    }
};

// Minimal CWorkThread base view (CDeviceFont.hpp pattern): the ctor/dtor
// resolve directly to the retail __ct__11CWorkThreadFPCcP11CWorkThreadi /
// __dt__11CWorkThreadFv symbols.
class CWorkThread {
public:
    CWorkThread(const char* pName, CWorkThread* pParent, int capacity);
    ~CWorkThread();
};

// reslist<const LAYER_QUE*> node mirror (reslist.hpp _reslist_node).
struct LayerQueNode {
    LayerQueNode* mNext; // 0x0
    LayerQueNode* mPrev; // 0x4
    const LAYER_QUE* mItem; // 0x8
};

// Queued draw-command record appended into the shared scratch buffer.
// (LAYER_QUE defined above)

// reslist<const LAYER_QUE*> container mirror (reslist.hpp layout, 0x20 bytes)
// as embedded at +0x1C4 of the layer.
struct LayerQueList {
    u32 mVtbl;                   // 0x00
    LayerQueNode* mStartNodePtr; // 0x04
    LayerQueNode mStartNode;     // 0x08 (embedded sentinel)
    LayerQueNode* mList;         // 0x14 (node slot array)
    int mCapacity;               // 0x18
    bool field_0x1C;             // 0x1C (externally-owned flag)

    void clearList() {
        LayerQueNode* node = mStartNodePtr->mNext;
        while (node != mStartNodePtr) {
            LayerQueNode* prev = node;
            node = node->mNext;
            prev->mNext = NULL;
        }
        mStartNodePtr->mNext = mStartNodePtr;
        mStartNodePtr->mPrev = mStartNodePtr;
    }

    int findFirstEmptySlotIndex() {
        int i = 0;
        while (i < mCapacity) {
            if (mList[i].mNext == NULL)
                break;
            i++;
        }
        return i;
    }

    void push_back(const LAYER_QUE* item) {
        LayerQueNode* startNode = mStartNodePtr;
        int i = findFirstEmptySlotIndex();
        LayerQueNode* temp = &mList[i];
        const LAYER_QUE** ptr = &temp->mItem;
        if (ptr != NULL) {
            try {
                *ptr = item;
            } catch (...) {
                throw;
            }
        }
        temp->mNext = startNode;
        temp->mPrev = startNode->mPrev;
        startNode->mPrev->mNext = temp;
        startNode->mPrev = temp;
    }

    // Lazily allocate + zero the slot array (zeroed in blocks of 8, the
    // retail unroll shape).
    void reserve(u32 handle) {
        mList = (LayerQueNode*)mtl::MemManager::allocate_array(0x1200, handle);
        for (int i = 0; i < 0x180;) {
            mList[i + 0].mNext = NULL;
            mList[i + 1].mNext = NULL;
            mList[i + 2].mNext = NULL;
            mList[i + 3].mNext = NULL;
            mList[i + 4].mNext = NULL;
            mList[i + 5].mNext = NULL;
            mList[i + 6].mNext = NULL;
            mList[i + 7].mNext = NULL;
            i += 8;
        }
        mCapacity = 0x180;
    }
};

class __declspec(novtable) CDeviceFontLayer : public CWorkThread {
public:
    CDeviceFontLayer(const char* pName, CWorkThread* pParent, int unk);
    ~CDeviceFontLayer();

    // 0x0-0x5C: work-thread base (retail vtable stored manually in the ctor)
    u8 _base[0x50];                // 0x00 (vtable + name + state)
    u32 mType;                     // 0x50 (CWorkThread::ThreadType)
    u32 mAllocHandle;              // 0x54
    CWorkThread* mParent;          // 0x58
    CWorkThreadChildren mChildren; // 0x5C
    u8 _baseTail[0x1C4 - 0x7C];   // 0x7C (flags/msgqueue/base tail)
    LayerQueList mQueList;         // 0x1C4 (command queue)
    CDeviceFontLayerCmdNode* mCurNode; // 0x1e4
    u8 mDirty;                         // 0x1e8
    u8 pad_1E9[3];
    f32 mScaleX; // 0x1ec
    f32 mScaleY; // 0x1f0
    u32 mFontId; // 0x1f4
    u32 field_0x1F8; // 0x1f8
    u8 mFlag1FC;   // 0x1fc
    u8 pad_1FD[3];
    ml::CCol4 mColor;   // 0x200
    ml::CCol4 mBgColor; // 0x210
    // Raw storage for the embedded CDrawGX at 0x220: constructed/destructed
    // explicitly so the ctor keeps the retail initialization order.
    u8 mDrawGXBuf[sizeof(CDrawGX)];
    CDrawGX* drawGX() { return (CDrawGX*)mDrawGXBuf; }
    u8 mFlag2F0;        // 0x2F0 (read by wkUpdate)

    u32 getScaledTextHeight();
    void allocFontHeap();
    void freeFontHeap();
    void resetFontCursor();
    u32 getFontHeapSize();
    bool wkStandbyLogout();
    void wkUpdate();
};

// CDeviceFont::getFontInfo(u32) - font-info lookup by index. Declared with
// the exact retail name; callers pass only the index (see file header note).
extern "C" IDeviceFontInfo* getFontInfo__11CDeviceFontFUlPQ34nw4r3lyt6Layout(
    u32 index);

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

// CWorkThread::wkSetEvent - post an event to this layer's work queue.
extern "C" void wkSetEvent__11CWorkThreadFQ211CWorkThread3EVT(void* self, int evt);
// CWorkThread::wkStandbyLogout - base standby-logout; returns whether the
// thread may be released (no pending children / events).
extern "C" bool wkStandbyLogout__11CWorkThreadFv(void* self);

// ---------------------------------------------------------------------------
// reslist<const LAYER_QUE*> deleting destructors (retail flat template
// mangling; plain __-prefixed globals over the mirror layout, per the
// CUIBattleManager recipe).
// ---------------------------------------------------------------------------
extern "C" void* __dt___reslist_base_const_CDeviceFontLayer_LAYER_QUE(LayerQueList* self,
                                                                  int mode) {
    if (self != NULL) {
        self->mVtbl = (u32)lbl_eu_8056C89C;
        LayerQueNode* node = self->mStartNodePtr->mNext;
        while (node != self->mStartNodePtr) {
            LayerQueNode* prev = node;
            node = node->mNext;
            prev->mNext = NULL;
        }
        self->mStartNodePtr->mNext = self->mStartNodePtr;
        self->mStartNodePtr->mPrev = self->mStartNodePtr;
        if (self->field_0x1C == false) {
            if (self->mList != NULL) {
                delete[] self->mList;
                self->mList = NULL;
            }
        }
        if (mode > 0) {
            delete self;
        }
    }
    return self;
}

extern "C" void* __dt__reslist_const_CDeviceFontLayer_LAYER_QUE(LayerQueList* self,
                                                     int mode) {
    // Doubled null check mirrors retail (MWCC keeps the dead second beq);
    // the deleting-mode delete sits outside the inner check.
    if (self != NULL) {
        if (self != NULL) {
            self->mVtbl = (u32)lbl_eu_8056C89C;
            LayerQueNode* node;
            node = self->mStartNodePtr->mNext;
            while (self->mStartNodePtr != node) {
                LayerQueNode* prev = node;
                node = node->mNext;
                prev->mNext = NULL;
            }
            self->mStartNodePtr->mNext = self->mStartNodePtr;
            self->mStartNodePtr->mPrev = self->mStartNodePtr;
            if (self->field_0x1C == false) {
                if (self->mList != NULL) {
                    delete[] self->mList;
                    self->mList = NULL;
                }
            }
        }
        if (mode > 0) {
            delete self;
        }
    }
    return self;
}

// ---- CDeviceFontLayer constructor (0x80457908) ----
CDeviceFontLayer::CDeviceFontLayer(const char* pName, CWorkThread* pParent,
                                   int unk)
    : CWorkThread(pName, pParent, 0) {
    // Inlined reslist<const LAYER_QUE*> construction: clear slots, store base
    // vtable + empty ring, then overwrite with the derived vtable.
    mQueList.mList = NULL;
    *(u32**)this = lbl_eu_8056C7D0; // manual retail vtable (non-virtual TU view)
    mQueList.mVtbl = (u32)lbl_eu_8056C89C;
    mQueList.mCapacity = 0;
    mQueList.field_0x1C = false;
    mQueList.mStartNodePtr = &mQueList.mStartNode;
    mQueList.mStartNodePtr->mNext = mQueList.mStartNodePtr;
    mQueList.mStartNodePtr->mPrev = mQueList.mStartNodePtr;
    mQueList.mVtbl = (u32)lbl_eu_8056C884;
    // Chained assign: one shared lfs (retail shape), stores 0x1ec then 0x1f0.
    mScaleY = mScaleX = lbl_eu_8066A408;
    mFontId = 0;
    field_0x1F8 = 0;
    mFlag1FC = 1;
    // Constructed here (not in the init list) to keep the retail order.
    // Direct ctor-symbol call: placement-new would emit MWCC's intrinsic
    // null guard (addic./beq) that retail does not have.
    __ct__7CDrawGXFv(drawGX());
    // Retail hoists all three constant reads to right after the ctor call.
    f32 col = lbl_eu_8066A40C;
    f32 bg = lbl_eu_8066A410;
    mFlag2F0 = 0;
    mType = 0x3F; // CWorkThread::TYPE_FONT_LAYER
    mScaleY = mScaleX = lbl_eu_8066A408;
    mColor.set(col, col, col, col);
    mBgColor.set(bg, bg, bg, bg);
    mQueList.clearList();
    drawGX()->clear();
    mDirty = 0;
    drawGX()->setFlag(0x10, true);
}

// ---- CDeviceFontLayer destructor (0x80457BA4) ----
CDeviceFontLayer::~CDeviceFontLayer() {
    drawGX()->~CDrawGX();
    // Inlined reslist<const LAYER_QUE*> teardown: doubled null check mirrors
    // retail. The CWorkThread base dtor call and deleting-flag delete are
    // emitted implicitly after this body.
    LayerQueList* list = &mQueList;
    if (list != NULL) {
        if (list != NULL) {
            list->mVtbl = (u32)lbl_eu_8056C89C;
            list->clearList();
            if (list->field_0x1C == false) {
                if (list->mList != NULL) {
                    delete[] list->mList;
                    list->mList = NULL;
                }
            }
        }
    }
}

// ---------------------------------------------------------------------------
// func_80453D78 (0x80457E48) - measure the widest line of a string.
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
extern "C" u32 func_80453D78__16CDeviceFontLayerFv(const char* str,
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
void func_80453BB4__16CDeviceFontLayerFv(CDeviceFontLayer* self, s16 x, s16 y,
                                         const char* str) {
    if (self->mQueList.mCapacity == 0)
        self->mQueList.reserve(self->mAllocHandle);

    // Carve the record out of the shared scratch buffer: TextQue is exactly
    // 8 bytes, so end-1 addresses the record header (the retail shape keeps
    // the end-of-record cursor live and rebases to the start afterwards).
    u32 cursor = lbl_eu_80665694 + 8;
    TextQue* cmd = (TextQue*)((u8*)lbl_eu_80665690 + cursor) - 1;
    lbl_eu_80665694 = cursor;
    cmd->mCmd = 0;
    cmd->mX = x;
    cmd->mY = y;

    // Inline string payload (with terminator).
    u32 len = strlen(str) + 1;
    u32 newCursor = lbl_eu_80665694 + len;
    lbl_eu_80665694 = newCursor;
    memcpy((u8*)lbl_eu_80665690 + newCursor - len, str, len);

    // Align the end cursor up to 4 and record the total record size.
    int cur = lbl_eu_80665694;
    int rem = cur & 3;
    if (rem != 0)
        cur = cur + 4 - rem;
    int total = (u8*)lbl_eu_80665690 + cur - (u8*)cmd;
    lbl_eu_80665694 = cur;
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
void setBackgroundColor__16CDeviceFontLayerFv(CDeviceFontLayer* self,
                                         const ml::CCol4* col) {
    bool same = self->mBgColor.r == col->r && self->mBgColor.g == col->g &&
                self->mBgColor.b == col->b && self->mBgColor.a == col->a;
    if (same)
        return;

    LAYER_QUE* cmd;
    FONT_LAYER_RESERVE_SLOTS(self);

    // Record addressed via its end pointer (retail writes fields
    // end-relative), rebased to the start for the queue append.
    u32 cursor = lbl_eu_80665694 + 0x14;
    lbl_eu_80665694 = cursor;
    cmd = (LAYER_QUE*)((u8*)lbl_eu_80665690 + cursor) - 1;
    cmd->mCmd = 1;
    cmd->mSize = 0x14;
    cmd->mCol = *col;

    // Hand-inlined reslist<const LAYER_QUE*> append (retail walks the slot
    // array with a count + byte-offset pair, then links the node).
    LayerQueList* list = &self->mQueList;
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

    self->mBgColor = *col;
}

// Queue a text-scale change; the layer's own scale is updated immediately.
// Single chained record-pointer expression: MWCC forms the end-of-record
// pointer, writes the leading fields end-relative, then rebases to the
// record start (subi) for the trailing field and the queue append.
void setFontScale__16CDeviceFontLayerFv(CDeviceFontLayer* self, f32 scaleX,
                                         f32 scaleY) {
    FONT_LAYER_RESERVE_SLOTS(self);

    u32 cursor = lbl_eu_80665694 + 0xc;
    lbl_eu_80665694 = cursor;
    LAYER_QUE* cmd = (LAYER_QUE*)((u8*)lbl_eu_80665690 + cursor - 0xc);
    cmd->mCmd = 3;
    cmd->mSize = 0xc;
    cmd->mF[0] = scaleX;
    cmd->mF[1] = scaleY;

    LayerQueList* list = &self->mQueList;
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

    self->mScaleX = scaleX;
    self->mScaleY = scaleY;
}

// Queue a font-id change; the layer's own font id is updated immediately.
// The reslist append follows the func_8048C524/CDeviceVI recipe: hand-inlined
// push_back walk with explicit byteOff + comma-init so the register assignment
// matches retail (cmd r4, i r5, byteOff r6, capacity r7, sentinel r8).
void setFontId__16CDeviceFontLayerFv(CDeviceFontLayer* self, u32 arg) {
    FONT_LAYER_RESERVE_SLOTS(self);

    u32 cursor = lbl_eu_80665694 + 8;
    lbl_eu_80665694 = cursor;
    LAYER_QUE* cmd = (LAYER_QUE*)((u8*)lbl_eu_80665690 + cursor - 8);
    cmd->mCmd = 5;
    cmd->mSize = 8;
    cmd->mArg[0] = arg;

    LayerQueList* list = &self->mQueList;
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

    self->mFontId = arg;
}

// ---------------------------------------------------------------------------
// func_80454508 (0x80458604) - queue a draw command.
//
// Lazily reserves the 0x180-node slot array (8-byte command records in the
// shared scratch buffer), then appends an {cmd=6, size=8, arg} record to the
// scratch cursor and links a queue node pointing at it (push_back).
// ---------------------------------------------------------------------------
extern "C" void func_80454508__16CDeviceFontLayerFv(CDeviceFontLayer* self,
                                                    u32 arg) {
    // Lazily allocate the slot array via the shared reserve() helper.
    FONT_LAYER_RESERVE_SLOTS(self);

    // Append the command record at the scratch-buffer write cursor. The
    // record is addressed via the end-of-record pointer, one 8-byte header
    // back (retail keeps the end pointer live and stores at [-1]).
    u32 cursor = lbl_eu_80665694 + 8;
    lbl_eu_80665694 = cursor;
    LAYER_QUE* cmd = (LAYER_QUE*)((u8*)lbl_eu_80665690 + cursor - 8);
    cmd->mCmd = 6;
    cmd->mSize = 8;
    cmd->mArg[0] = arg;

    self->mQueList.push_back(cmd);
}



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
        getFontInfo__11CDeviceFontFUlPQ34nw4r3lyt6Layout(fontId);
    if (info == 0) {
        return;
    }

    // Save the draw color; it is restored at the end.
    ml::CCol4 drawCol = ((CDrawGXColorLayout*)draw)->mCol;

    if (col != 0) {
        draw->setCol(*col);
        if (lbl_eu_8066A40C != col->a) {
            // Semi-transparent text: draw a background rect sized by the
            // measured text width and the scaled line height.
            u32 w = func_80453D78__16CDeviceFontLayerFv(str, fontId, scaleX);
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
        lineCol[0].set(lbl_eu_8066A40C, lbl_eu_8066A40C, lbl_eu_8066A40C,
                       drawCol.a);
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
    if (self->mQueList.mCapacity == 0) {
        return;
    }

    self->drawGX()->setZCompare(0);
    self->drawGX()->setZWriteEnable(0);

    CDeviceFontLayerCmdNode* node =
        (CDeviceFontLayerCmdNode*)self->mQueList.mStartNodePtr->mNext;
    if (self->mDirty) {
        node = self->mCurNode->mNext;
        self->mCurNode = node;
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
            self->drawGX()->setGXCacheId(node->mData->mArg0);
            break;
        case 1:
            self->drawGX()->setCol(*(ml::CCol4*)&node->mData->mArg0);
            break;
        case 2:
            self->mColor = *(ml::CCol4*)&node->mData->mArg0;
            break;
        case 0: {
            CDeviceFontLayerCmdData* d = node->mData;
            func_804546C8__16CDeviceFontLayerFP7CDrawGX(
                self->drawGX(), self->mFontId,
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
        self->mCurNode = node;
        node = node->mNext;
    }

    self->mDirty = 1;
    if (flag != 0) {
        // Color reset pulls 1.0f/0.0f from the shared float globals; both
        // loads are hoisted above their store groups (retail shape), with
        // col landing in f1 and bg in f0.
        f32 bg = lbl_eu_8066A410, col = lbl_eu_8066A40C;
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
        self->drawGX()->clear();
        // Read-modify-write split so the flags load is scheduled before the
        // mDirty store (retail shape).
        CDrawGXColorLayout* gxLayout = (CDrawGXColorLayout*)self->mDrawGXBuf;
        u32 gxFlags = gxLayout->mFlags;
        self->mDirty = 0;
        gxLayout->mFlags = gxFlags | 0x10;
    }

    self->drawGX()->setZCompare(1);
    self->drawGX()->setZWriteEnable(1);
}

void CDeviceFontLayer::wkUpdate() {
    if (this->mFlag2F0 == 0)
        return;
    wkSetEvent__11CWorkThreadFQ211CWorkThread3EVT(this, 0); // EVT_NONE
}

bool CDeviceFontLayer::wkStandbyLogout() {
    if (mChildren.empty() && this->mFlag2F0 != 0)
        return wkStandbyLogout__11CWorkThreadFv(this);
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
// lbl_eu_80665690 / 80665694 now dissolved above

void CDeviceFontLayer::allocFontHeap() {
    if (lbl_eu_80665690 != 0)
        return;
    int handle = getDevSys1Handle__7CDeviceFv();
    lbl_eu_80665690 = allocate_array_ex__Q23mtl10MemManagerFUlUli(0x10000,
                                                                  handle, 16);
    lbl_eu_80665694 = 0;
}

// Font-layer global state: an allocated object pointer plus an adjacent word.
extern "C" void __dla__FPv(void* p);

void CDeviceFontLayer::freeFontHeap() {
    if (lbl_eu_80665690 == 0)
        return;
    if (lbl_eu_80665690 != 0) {
        __dla__FPv(lbl_eu_80665690);
        lbl_eu_80665690 = 0;
    }
    lbl_eu_80665694 = 0;
}

void CDeviceFontLayer::resetFontCursor() { lbl_eu_80665694 = 0; }

u32 CDeviceFontLayer::getFontHeapSize() { return 0x10000; }

// ===== Dissolved retail data (CDeviceFontLayer TU) =====
// .rodata 0x80522E40 (0x78) - RTTI type names. Retail packs them at +0x00,
// +0x14, +0x44 with 3/3/1 zero gap bytes between, so each array carries its
// trailing gap. MWCC ignores __attribute__((aligned(1))) on section data;
// the native __declspec(align(n)) is what actually controls placement
// (CNBanner/CScnFilter recipe). First object sets the 8-section alignment.
extern "C" __declspec(section ".rodata") __attribute__((aligned(8))) const char lbl_eu_80522E40[0x11] = {0x43,0x44,0x65,0x76,0x69,0x63,0x65,0x46,0x6F,0x6E,0x74,0x4C,0x61,0x79,0x65,0x72,0x00};
extern "C" __declspec(section ".rodata") const char lbl_eu_80522E54[0x2D] = {0x72,0x65,0x73,0x6C,0x69,0x73,0x74,0x3C,0x63,0x6F,0x6E,0x73,0x74,0x20,0x43,0x44,0x65,0x76,0x69,0x63,0x65,0x46,0x6F,0x6E,0x74,0x4C,0x61,0x79,0x65,0x72,0x3A,0x3A,0x4C,0x41,0x59,0x45,0x52,0x5F,0x51,0x55,0x45,0x20,0x2A,0x3E,0x00};
extern "C" __declspec(section ".rodata") const char lbl_eu_80522E84[0x34] = {0x5F,0x72,0x65,0x73,0x6C,0x69,0x73,0x74,0x5F,0x62,0x61,0x73,0x65,0x3C,0x63,0x6F,0x6E,0x73,0x74,0x20,0x43,0x44,0x65,0x76,0x69,0x63,0x65,0x46,0x6F,0x6E,0x74,0x4C,0x61,0x79,0x65,0x72,0x3A,0x3A,0x4C,0x41,0x59,0x45,0x52,0x5F,0x51,0x55,0x45,0x20,0x2A,0x3E,0x00,0x00};

// .sdata 0x80663730 (0x18) - RTTI locators
extern "C" __declspec(section ".sdata") __attribute__((aligned(8))) u32 lbl_eu_80663730[2] = { (u32)&lbl_eu_80522E40, (u32)&lbl_eu_8056C870 };
extern "C" __declspec(section ".sdata") __attribute__((aligned(8))) u32 lbl_eu_80663738[2] = { (u32)&lbl_eu_80522E54, (u32)&lbl_eu_8056C890 };
extern "C" __declspec(section ".sdata") __attribute__((aligned(8))) u32 lbl_eu_80663740[2] = { (u32)&lbl_eu_80522E84, 0 };

// .data 0x8056C7D0 (0xD8) - vtables / RTTI chains
extern "C" u32 lbl_eu_8056C7D0[40] __attribute__((aligned(8))) = {
    (u32)&lbl_eu_80663730, 0, (u32)&__dt__16CDeviceFontLayerFv,
    (u32)&WorkEvent1__10IWorkEventFPvPCc, (u32)&OnFileEvent__10IWorkEventFP10CEventFile,
    (u32)&WorkEvent3__10IWorkEventFPv, (u32)&WorkEvent4__10IWorkEventFv,
    (u32)&OnPauseTrigger__10IWorkEventFb,
    (u32)&WorkEvent6__10IWorkEventFv, (u32)&WorkEvent7__10IWorkEventFv,
    (u32)&WorkEvent8__10IWorkEventFv, (u32)&WorkEvent9__10IWorkEventFv,
    (u32)&WorkEvent10__10IWorkEventFv, (u32)&WorkEvent11__10IWorkEventFv,
    (u32)&WorkEvent12__10IWorkEventFv, (u32)&WorkEvent13__10IWorkEventFv,
    (u32)&WorkEvent14__10IWorkEventFv, (u32)&WorkEvent15__10IWorkEventFv,
    (u32)&WorkEvent16__10IWorkEventFv, (u32)&WorkEvent17__10IWorkEventFv,
    (u32)&WorkEvent18__10IWorkEventFv, (u32)&WorkEvent19__10IWorkEventFv,
    (u32)&WorkEvent20__10IWorkEventFv, (u32)&WorkEvent21__10IWorkEventFv,
    (u32)&WorkEvent22__10IWorkEventFv, (u32)&WorkEvent23__10IWorkEventFv,
    (u32)&WorkEvent24__10IWorkEventFv, (u32)&WorkEvent25__10IWorkEventFv,
    (u32)&WorkEvent26__10IWorkEventFv, (u32)&WorkEvent27__10IWorkEventFv,
    (u32)&WorkEvent28__10IWorkEventFv, (u32)&WorkEvent29__10IWorkEventFv,
    (u32)&WorkEvent30__10IWorkEventFv, (u32)&WorkEvent31__10IWorkEventFv,
    (u32)&wkUpdate__16CDeviceFontLayerFv, (u32)&wkRender__11CWorkThreadFv,
    (u32)&wkRenderAfter__11CWorkThreadFv, (u32)&wkStandbyLogin__11CWorkThreadFv,
    (u32)&wkStandbyLogout__16CDeviceFontLayerFv, (u32)&wkStandbyExceptionRetry__11CWorkThreadFUl,
};
extern "C" u32 lbl_eu_8056C870[5] __attribute__((aligned(8))) = {
    (u32)&__RTTI__10IWorkEvent, 0, (u32)&__RTTI__11CWorkThread, 0, 0,
};
extern "C" u32 lbl_eu_8056C884[3] __attribute__((aligned(4))) = {
    (u32)&lbl_eu_80663738, 0, (u32)&__dt__reslist_const_CDeviceFontLayer_LAYER_QUE,
};
extern "C" u32 lbl_eu_8056C890[3] __attribute__((aligned(4))) = {
    (u32)&lbl_eu_80663740, 0, 0,
};
extern "C" u32 lbl_eu_8056C89C[3] __attribute__((aligned(4))) = {
    (u32)&lbl_eu_80663740, 0, (u32)&__dt___reslist_base_const_CDeviceFontLayer_LAYER_QUE,
};

// .sbss 0x80665690 (0x8) - zero-initialized globals
void* lbl_eu_80665690;
u32 lbl_eu_80665694;


