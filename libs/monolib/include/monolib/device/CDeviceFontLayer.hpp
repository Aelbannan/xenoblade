#pragma once

#include <types.h>
#include "monolib/work/CWorkThread.hpp"
#include "monolib/util/reslist.hpp"
#include "monolib/core/CDrawGX.hpp"
#include "monolib/math.hpp"

// Font layer work thread: queues text draw commands (LAYER_QUE records carved
// from a shared scratch buffer) and replays them into an embedded CDrawGX.
// Retail RTTI: CDeviceFontLayer -> CWorkThread -> IWorkEvent.
//size: 0x2F4
class CDeviceFontLayer : public CWorkThread {
public:
    // Queued draw-command record header (payload follows inline).
    struct LAYER_QUE {
        u16 mCmd;  // 0x00
        u16 mSize; // 0x02
        union {
            u32 mArg[4]; // 0x04
            f32 mF[4];   // 0x04
            ml::CCol4 mCol;
            struct { u16 mPad4; u16 mPad6; s16 mX; s16 mY; }; // 0x04
        };
    };

    CDeviceFontLayer(const char* pName, CWorkThread* pParent, int unk);
    virtual ~CDeviceFontLayer();
    virtual void wkUpdate();
    virtual bool wkStandbyLogout();

    void setBackgroundColor(const ml::CCol4* col);
    void setFontScale(f32 scaleX, f32 scaleY);
    void setFontId(u32 id);
    u32 getScaledTextHeight();

    static void allocFontHeap();
    static void freeFontHeap();
    static void resetFontCursor();
    static u32 getFontHeapSize();

    //0x0-0x1C4: CWorkThread
    reslist<const LAYER_QUE*> mQueList;         // 0x1C4
    _reslist_node<const LAYER_QUE*>* mCurNode;  // 0x1E4
    u8 mDirty;                                  // 0x1E8
    f32 mScaleX;                                // 0x1EC
    f32 mScaleY;                                // 0x1F0
    u32 mFontId;                                // 0x1F4
    u32 field_0x1F8;                            // 0x1F8
    u8 mFlag1FC;                                // 0x1FC
    ml::CCol4 mColor;                           // 0x200
    ml::CCol4 mBgColor;                         // 0x210
    CDrawGX mDrawGX;                            // 0x220
    u8 mFlag2F0;                                // 0x2F0
};

extern void* sFontCommandBuffer;
extern u32 sFontCommandCursor;
