#pragma once

#include <types.h>

// NAND path/file primitives defined in CNReqtaskSave.cpp and shared by the
// sibling NAND sub-task units. Their retail names are stripped placeholders, so
// they keep C linkage to emit/resolve the unmangled retail symbols.
extern "C" {
    const char* CNReqSaveBuildTempPath(u8 id);                      // temp-path builder
    s32 CNReqSaveNandOpen(const char* path, u8 flag);               // NAND open wrapper
    s32 CNReqSaveNandClose(void);                                   // NAND close primitive
    s32 CNReqSaveNandCreate(const char* path, u8 perm, u8 attr);    // NAND create (async)
    s32 CNReqSaveNandMove(const char* from, const char* to);        // NAND move (async)
    s32 CNReqSaveNandChangeDir(const char* path);                   // NAND change-dir (async)
    s32 CNReqSaveNandTell(u32* pos);                                // NAND tell (async)
    s32 CNReqSaveNandWrite(u32 addr, u32 size);                     // NAND write (async) - fixed save file-info
}
