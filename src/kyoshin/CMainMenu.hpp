#pragma once

#include <types.h>

class CFileHandle;
class CProcess;

namespace nw4r { namespace lyt {
class Layout;
class Pane;
class ArcResourceAccessor;
class AnimTransform;
class DrawInfo;
} }

class CBaseCur;
class CSubCur;

class CMainMenu {
public:
    CMainMenu();
    CMainMenu(void* param);
    virtual ~CMainMenu();
    void Init();
    void Term();
    void cbRenderBefore();
    int func_800FF778();
    void func_800FEB14(float* pos);

    // CProcess layout (inherited, 0x00-0x3C):
    //   +0x00: vtable (CProcess)
    //   +0x04: mParent, mChild, mNext, mPrev, etc.
    //   +0x10: CProcess vtable set in constructor
    //   +0x3C-0x44: PTMF (IWorkEvent vtable entry)
    //   +0x48-0x50: PTMF (IWorkEvent vtable entry)
    //   +0x54: byte (field_0x54)
    //   +0x55: byte (field_0x55)
    //   +0x58-0x5C: IWorkEvent vtable slot (2 words)
    //   +0x60: UnkClass_8045F564 (embedded object)
    //   +0x70: void* param (from constructor)
    //   +0x74: CFileHandle* (file handle)
    //   +0x78: ArcResourceAccessor*
    //   +0x7C: Layout* (main layout)
    //   +0x80: AnimTransform* (anim0)
    //   +0x84: AnimTransform* (anim1)
    //   +0x88: AnimTransform* (anim2)
    //   +0x8C: AnimTransform* (anim3)
    //   +0x90: CBaseCur (embedded)
    //   +0xA8: CSubCur (embedded)
    //   +0xC0: s32 cursorIndex
    //   +0xC4: s32 subIndex
    //   +0xC8: u8 cursorAvail[8] (cursor availability flags)
    //   +0xD0: u8 flags[13] (various flags)
    //   +0xE0: s32 state

    u8 _04[0x10 - 0x04];                      // 0x04-0x0F
    // +0x10: CProcess vtable set by constructor
    u8 _10[0x3C - 0x10];                      // 0x10-0x3B
    // PTMF at 0x3C (IWorkEvent vtable, 3 words)
    u32 field_0x3C;                            // 0x3C
    u32 field_0x40;                            // 0x40
    u32 field_0x44;                            // 0x44
    u32 field_0x48;                            // 0x48
    u32 field_0x4C;                            // 0x4C
    u32 field_0x50;                            // 0x50
    u8 field_0x54;                             // 0x54
    u8 field_0x55;                             // 0x55
    u8 _56[0x58 - 0x56];                      // 0x56-0x57
    u32 mIWorkEventVtbl;                       // 0x58 — IWorkEvent vtable (raw)
    u32 field_0x5C;                            // 0x5C
    u8 _60[0x70 - 0x60];                       // 0x60-0x6F — UnkClass_8045F564 embedded
    void* field_0x70;                          // 0x70 — param from constructor
    CFileHandle* field_0x74;                   // 0x74 — file handle
    nw4r::lyt::ArcResourceAccessor* field_0x78; // 0x78 — arc resource accessor
    nw4r::lyt::Layout* field_0x7C;             // 0x7C — main layout
    nw4r::lyt::AnimTransform* field_0x80;      // 0x80 — anim0
    nw4r::lyt::AnimTransform* field_0x84;      // 0x84 — anim1
    nw4r::lyt::AnimTransform* field_0x88;      // 0x88 — anim2
    nw4r::lyt::AnimTransform* field_0x8C;      // 0x8C — anim3
    // CBaseCur at +0x90 (16 bytes), CSubCur at +0xA8
    u8 _90[0xC0 - 0x90];                       // 0x90-0xBF — CBaseCur + CSubCur
    s32 field_0xC0;                            // 0xC0 — cursor index
    s32 field_0xC4;                            // 0xC4 — sub-index
    u8 field_0xC8[8];                          // 0xC8-0xCF — cursor availability flags
    u8 field_0xD0;                             // 0xD0 — flag: hasSave
    u8 field_0xD1;                             // 0xD1 — flag: something1
    u8 field_0xD2;                             // 0xD2 — flag: something2
    u8 field_0xD3;                             // 0xD3 — flag: something3
    u8 field_0xD4;                             // 0xD4 — flag: something4
    u8 field_0xD5;                             // 0xD5 — flag: something5
    u8 field_0xD6;                             // 0xD6 — flag: something6
    u8 field_0xD7;                             // 0xD7
    u8 field_0xD8;                             // 0xD8
    u8 field_0xD9;                             // 0xD9
    u8 field_0xDA;                             // 0xDA
    u8 field_0xDB;                             // 0xDB
    u8 field_0xDC;                             // 0xDC
    u8 _DD[0xE0 - 0xDD];                       // 0xDD-0xDF padding
    s32 field_0xE0;                            // 0xE0 — state

    void OnFileEvent();
};