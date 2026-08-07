#pragma once

#include <types.h>

namespace nw4r { namespace lyt {
    class Layout;
    class AnimTransform;
}}
class CScn;

// 8-byte quest entry record (used at 0x94 and 0x194 inside CMenuQstCnt).
struct QstEntry {
    u16 f0;   // 0x00 quest id
    u16 f2;   // 0x02
    u8  f4;   // 0x04
    u8  f5;   // 0x05
    u8  f6;   // 0x06
    void setQstEntry(short a, short b, u8 c, u8 d, u8 e);
};

// 8-byte quest-info record (element of the 0x400-element list in QstData).
struct QstInfo {
    u16 f0;
    u8  f2;
    u8  f3;
    u8  f4;
    u8  f5;
    u8  f6;
    u8  f7;
};

/*
 * Quest-status menu object (size 0x1A0).
 *
 * Retail stores the vtable pointer at +0x10 (CProcess-style layout). The base
 * IUIWindow subobject is destroyed OOL via __dt__9IUIWindowFv, and the ctor
 * directly runs CProcess's ctor then fills the IUIWindow region by hand, so
 * the struct is laid out explicitly rather than inheriting.
 */
struct CMenuQstCnt {
    u8  mPad0[0x10];                     // 0x00 link-list nodes
    u32 mVtab;                           // 0x10 vtable ptr
    u8  mPad14[0x3C - 0x14];             // 0x14
    u32 mPtmfFunc0;                      // 0x3C __ptmf_null
    u32 mPtmfThis0;                      // 0x40
    u32 mPtmfDelta0;                     // 0x44
    u32 mPtmfFunc1;                      // 0x48
    u32 mPtmfThis1;                      // 0x4C
    u32 mPtmfDelta1;                     // 0x50
    nw4r::lyt::Layout* mLayout;          // 0x54
    u32 mField58;                        // 0x58
    u32 mField5C;                        // 0x5C
    s32 mField60;                        // 0x60 (default -1)
    u8  mField64;                        // 0x64 (bool)
    u8  mField65;                        // 0x65
    u8  mField66;                        // 0x66
    u8  mField67;                        // 0x67 (default 1)
    u32 mField68;                        // 0x68
    u32 mOcc6C;                          // 0x6C vtable-adjusted trackers
    u32 mOcc70;                          // 0x70 (IScnRender pointer for removeRenderCB)
    CScn* mScene;                        // 0x74
    u8  mMemRegion[0x10];                // 0x78 UnkClass_8045F564 storage
    nw4r::lyt::AnimTransform* mAnim88;   // 0x88
    u32 mState8C;                        // 0x8C
    f32 mValue90;                        // 0x90
    QstEntry mEntries[32];               // 0x94 (0x100 bytes)
    QstEntry mSelEntry;                  // 0x194
    u8  mField19C;                       // 0x19C
    // sizeof = 0x1A0

    void Init();
    void Term();
    void Move();
    void cbRenderBefore();
};