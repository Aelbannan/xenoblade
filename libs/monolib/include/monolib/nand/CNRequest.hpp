#pragma once

#include <types.h>

class CNReqtask;
class CNBanner;

// One queued NAND request: the sub-task that carries it out plus that task's
// parameter block.
class CNRequest {
public:
    CNReqtask* mTask;
    u8 mParam[0x1C];

    CNRequest();
    virtual ~CNRequest() {}

    // Runs the task one step. Returns true once it has finished, with
    // *success telling whether it completed without error.
    bool update(bool* success);

    bool check(u32 neededBlocks, u32 neededFiles, u32* answer);
    bool save(const char* name, const void* buf, u32 size, u8 perm, bool share);
    bool load(const char* name, void* buf, u32 size, bool share);
    bool remove(const char* name, bool share);
    bool readdir(char* nameList, u32 size, u32* num, bool share);
    bool mkdir(const char* name, u8 perm, bool share);
    bool saveBanner(CNBanner* banner, bool share);
};
