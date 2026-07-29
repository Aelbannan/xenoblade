#pragma once

#include <types.h>

class CFileHandle;

class CMainMenu {
public:
    CMainMenu();
    virtual ~CMainMenu();
    void Init();
    void Term();
    void cbRenderBefore();
    int func_800FF778();

    // vtable pointer at 0x00 (implicit)
    u8 _04[0x3C - 0x04];
    u32 field_0x3C;
    u32 field_0x40;
    u32 field_0x44;
    u8 _48[0x58 - 0x48];
    u32 mIWorkEventVtbl;        // 0x58 — IWorkEvent vtable (raw, see MWCC_REFERENCE)
    u8 _5C[0x74 - 0x5C];
    CFileHandle* field_0x74;    // 0x74 — file handle from CDeviceFile::readFile

    void OnFileEvent();
};

