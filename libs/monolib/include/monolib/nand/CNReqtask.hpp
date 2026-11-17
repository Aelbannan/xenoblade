#pragma once

#include <types.h>
#include <revolution/NAND.h>

// Base of the NAND sub-tasks a CNRequest runs. Each sub-task is a stateless
// singleton; its per-request state lives in the CNRequest parameter block.
//
// The static members wrap the asynchronous NAND API: each call marks the
// subsystem busy, and the completion callback stores the result and clears
// the flag again. Tasks poll isBusy()/getResult() from update().
class CNReqtask {
public:
    // One step of the task: 0 = still running, 1 = done, 2 = failed.
    virtual int update(void* param) = 0;
    // Called once after update() reports completion.
    virtual void close(void* param);

    static s32 check(u32 neededBlocks, u32 neededFiles, u32* answer);
    static s32 open(const char* path, u8 access);
    static s32 read(void* buf, u32 size);
    static s32 write(const void* buf, u32 size);
    static s32 closeFile();
    static s32 create(const char* path, u8 perm, u8 attr);
    static s32 remove(const char* path);
    static s32 move(const char* from, const char* to);
    static s32 tell(u32* pos);
    static s32 readDir(char* nameList, u32* num, const char* path);
    static s32 createDir(const char* path, u8 perm, u8 attr);
    static s32 changeDir(const char* path);

    // Home directory of the title, or its "share" sub-directory.
    static const char* getHomeDir(bool share);
    // "<home dir>/<name>"
    static const char* getPath(const char* name, bool share);

    static void error(s32 code);

    static bool sBusy;   // an asynchronous NAND call is in flight
    static s32 sResult;  // result of the last completed call
    static bool sOpen;   // the shared file handle is open

private:
    static void callback(s32 result, NANDCommandBlock* block);
};

void setGeneralNandErrorMessage(const wchar_t* msg);
void setSaveNandErrorMessage(const wchar_t* msg);
void setExtraNandErrorMessage(const wchar_t* msg);
