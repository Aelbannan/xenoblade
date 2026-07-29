#pragma once

#include <types.h>

class CFileHandle;
class CProcess;

namespace nw4r { namespace lyt {
class Layout;
class Pane;
} }

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

    // vtable pointer at 0x00 (implicit)
    u8 _04[0x08 - 0x04];                    // 0x04-0x07
    nw4r::lyt::Layout* mpLayout;             // 0x08
    u8 _0C[0x3C - 0x0C];                    // 0x0C-0x3B
    u32 field_0x3C;                          // 0x3C
    u32 field_0x40;                          // 0x40
    u32 field_0x44;                          // 0x44
    u8 _48[0x58 - 0x48];                    // 0x48-0x57
    u32 mIWorkEventVtbl;                     // 0x58 — IWorkEvent vtable (raw, see MWCC_REFERENCE)
    u8 _5C[0x74 - 0x5C];                   // 0x5C-0x73
    CFileHandle* field_0x74;                 // 0x74 — file handle from CDeviceFile::readFile
    u8 _78[0xE0 - 0x78];                   // 0x78-0xDF
    s32 field_0xE0;                          // 0xE0 — state (4/8 = active states)

    void OnFileEvent();
};

