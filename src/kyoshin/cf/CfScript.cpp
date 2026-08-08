// CfScript.cpp - Script management for cf subsystem
// Based on retail assembly analysis

#include "kyoshin/cf/CfScript.hpp"
#include "kyoshin/cf/CfGameManager.hpp"
#include <cstring>
#include <cstdio>
#include <cstdarg>

// External C function declarations
extern "C" {
    void vmInit();
    void vmExec();
    void vmLink(void* ctx);
    void vmStart(void* ctx);
    void vmUnlink(void* ctx);
    void vmThreadSleepAll(void* ctx);
    void pluginRegist__Fv();
    void* func_800A82BC();
    void* func_800A837C();
    void* func_800A843C();
    void* CfRes_readCommonArchive(void* fileHandle, const char* path, void* callback);
    int getFileSize__11CDeviceFileFPCc(const char* path, int flags);
    void cancel__11CDeviceFileFP11CFileHandle(void* fileHandle);
    void __dl__FPv(void* ptr);

    // String constants from data
    extern char lbl_eu_805708D0[];
    extern char lbl_eu_80661AC0[];
    extern char lbl_eu_80661AD0[];
    extern char lbl_eu_804FB3A4[];
    extern char lbl_eu_80570918[];
    extern u8 lbl_eu_80663D88;

    // ml namespace functions
    void getNoPathExtName__Q22ml9CPathUtilFRQ22ml10FixStr64PCc(void* out, const char* path);
    void format__Q22ml10FixStr128FPCce(void* out, const char* fmt, ...);
    void format__Q22ml10FixStr64FPCce(void* out, const char* fmt, ...);
}

namespace cf {

// Forward declarations for functions in this TU
void func_80068A30(char* dest, const char* src);
void func_80068B58(CfScriptManager* mgr, const char* name);
void func_80068ECC(CfScript* script, const char* name);
void func_80068E7C(CfScriptManager* mgr, int index, int mask);

// func_80068A20 - initializer for path string
void func_80068A20() {
    func_80068A30(lbl_eu_805708D0, lbl_eu_80661AC0);
}

// func_80068A30 - copy string with length tracking
void func_80068A30(char* dest, const char* src) {
    u32 len = strlen(src);
    *((u32*)(dest + 0x40)) = len;
    strcpy(dest, src);
}

// func_80068A80 - get singleton and init
void func_80068A80() {
    CfScriptManager* mgr = CfScriptManager::getInstance();
    mgr->init();
}

// func_80068AA4 - get singleton and call func_800694B0
void func_80068AA4() {
    CfScriptManager* mgr = CfScriptManager::getInstance();
    mgr->func_800694B0();
}

// func_80068AC8 - get singleton and call func_8006953C
void func_80068AC8() {
    CfScriptManager* mgr = CfScriptManager::getInstance();
    mgr->func_8006953C();
}

// func_80068AEC - get singleton and call func_80068B20
void func_80068AEC(const char* name) {
    CfScriptManager* mgr = CfScriptManager::getInstance();
    mgr->func_80068B20();
}

// CfScriptManager::func_80068B20 - tail call to func_80068ECC
// mScripts[0] is at offset 0 of CfScriptManager, so this == &mScripts[0]
void CfScriptManager::func_80068B20() {
    func_80068ECC(&mScripts[0], nullptr);
}

// func_80068B24 - get singleton and call func_80068B58
void func_80068B24(const char* name) {
    CfScriptManager* mgr = CfScriptManager::getInstance();
    func_80068B58(mgr, name);
}

// func_80068B58 - load script into slot 1
void func_80068B58(CfScriptManager* mgr, const char* name) {
    CfScript* script = &mgr->mScripts[1];
    func_80068ECC(script, name);
}

// func_80068B60 - get singleton and call func_80068B94
void func_80068B60(const char* name) {
    CfScriptManager* mgr = CfScriptManager::getInstance();
    mgr->func_80068B94(name);
}

// CfScriptManager::func_80068B94 - load script into slot 2
void CfScriptManager::func_80068B94(const char* name) {
    CfScript* script = &mScripts[2];
    func_80068ECC(script, name);
}

// func_80068B9C - get singleton and call func_80068BC0
void func_80068B9C() {
    CfScriptManager* mgr = CfScriptManager::getInstance();
    mgr->func_80068BC0();
}

// CfScriptManager::func_80068BC0 - set flag bit 2 on slot 0
void CfScriptManager::func_80068BC0() {
    mScripts[0].mFlags |= 0x4;
}

// func_80068BD0 - get singleton and call func_80068BF4
void func_80068BD0() {
    CfScriptManager* mgr = CfScriptManager::getInstance();
    mgr->func_80068BF4();
}

// CfScriptManager::func_80068BF4 - set flag bit 2 on slot 1
void CfScriptManager::func_80068BF4() {
    mScripts[1].mFlags |= 0x4;
}

// func_80068C04 - get singleton and call func_80068C28
void func_80068C04() {
    CfScriptManager* mgr = CfScriptManager::getInstance();
    mgr->func_80068C28();
}

// CfScriptManager::func_80068C28 - set flag bit 2 on slot 2
void CfScriptManager::func_80068C28() {
    mScripts[2].mFlags |= 0x4;
}

// func_80068C38 - get singleton and call func_80068C5C
void func_80068C38() {
    CfScriptManager* mgr = CfScriptManager::getInstance();
    mgr->func_80068C5C();
}

// CfScriptManager::func_80068C5C - sleep VM thread for slot 2 if flag set
void CfScriptManager::func_80068C5C() {
    CfScript& script = mScripts[2];
    if (script.mFlags & 0x20) {
        if (script.mVmContext != nullptr) {
            vmThreadSleepAll(script.mVmContext);
        }
    }
}

// func_80068C7C - get singleton and call func_80068CA0
void func_80068C7C() {
    CfScriptManager* mgr = CfScriptManager::getInstance();
    mgr->func_80068CA0();
}

// CfScriptManager::func_80068CA0 - reset/cleanup slot 0
void CfScriptManager::func_80068CA0() {
    CfScript& script = mScripts[0];

    if (script.mFileHandle != nullptr) {
        cancel__11CDeviceFileFP11CFileHandle(script.mFileHandle);
        script.mFileHandle = nullptr;
    }

    if (script.mFlags & 0x8) {
        if (script.mVmContext != nullptr) {
            vmUnlink(script.mVmContext);
        }
    }

    script.mWaitCount = 0;
    script.mFlags = 0;
    script.mName[0] = '\0';
    script.mNameLen = 0;
}

// func_80068D14 - get singleton and call func_80068D38
void func_80068D14() {
    CfScriptManager* mgr = CfScriptManager::getInstance();
    mgr->func_80068D38();
}

// CfScriptManager::func_80068D38 - reset/cleanup slot 1
void CfScriptManager::func_80068D38() {
    CfScript& script = mScripts[1];

    if (script.mFileHandle != nullptr) {
        cancel__11CDeviceFileFP11CFileHandle(script.mFileHandle);
        script.mFileHandle = nullptr;
    }

    if (script.mFlags & 0x8) {
        if (script.mVmContext != nullptr) {
            vmUnlink(script.mVmContext);
        }
    }

    script.mWaitCount = 0;
    script.mFlags = 0;
    script.mName[0] = '\0';
    script.mNameLen = 0;
}

// func_80068DAC - get singleton and call func_80068DD0
void func_80068DAC() {
    CfScriptManager* mgr = CfScriptManager::getInstance();
    mgr->func_80068DD0();
}

// CfScriptManager::func_80068DD0 - reset/cleanup slot 2
void CfScriptManager::func_80068DD0() {
    CfScript& script = mScripts[2];

    if (script.mFileHandle != nullptr) {
        cancel__11CDeviceFileFP11CFileHandle(script.mFileHandle);
        script.mFileHandle = nullptr;
    }

    if (script.mFlags & 0x8) {
        if (script.mVmContext != nullptr) {
            vmUnlink(script.mVmContext);
        }
    }

    script.mWaitCount = 0;
    script.mFlags = 0;
    script.mName[0] = '\0';
    script.mNameLen = 0;
}

// func_80068E44 - get singleton and call func_80068E7C
void func_80068E44(int param) {
    CfScriptManager* mgr = CfScriptManager::getInstance();
    func_80068E7C(mgr, 2, param);
}

// func_80068E7C - check flags with mask
void func_80068E7C(CfScriptManager* mgr, int index, int mask) {
    CfScript& script = mgr->mScripts[index];
    u32 result = script.mFlags & mask;
    u32 neg = (u32)(-(s32)result);
    u32 combined = neg | result;
    u32 ret = combined >> 31;
    // Returns ret in r3
}

// func_80068E9C - format or copy string based on condition
void func_80068E9C(char* dest, const char* src1, const char* src2, const char* src3) {
    if (src3 != nullptr) {
        func_80068A30(dest, src3);
    } else {
        format__Q22ml10FixStr64FPCce(dest, &lbl_eu_804FB3A4[0x1C], src1, src2);
    }
}

// func_80068ECC - main script loading function
void func_80068ECC(CfScript* script, const char* name) {
    char pathBuffer[0x80];
    char extBuffer[0x44];
    char tempBuffer[0x80];
    u32 pathLen = 0;

    if (name == nullptr) {
        return;
    }

    if (strcmp(name, &lbl_eu_804FB3A4[0xA]) == 0 ||
        strcmp(name, &lbl_eu_804FB3A4[0x14]) == 0) {
        return;
    }

    pathLen = strlen(name);
    strcpy(pathBuffer, name);

    int extPos = -1;
    if (pathLen > 0) {
        u32 sepLen = strlen(lbl_eu_80661AD0);
        char* searchStart = pathBuffer + pathLen - 1;
        char* searchEnd = pathBuffer;

        while (searchStart >= searchEnd) {
            if (strncmp(searchStart, lbl_eu_80661AD0, sepLen) == 0) {
                extPos = (int)(searchStart - pathBuffer);
                break;
            }
            searchStart--;
        }
    }

    if (extPos > 0) {
        memset(tempBuffer, 0, sizeof(tempBuffer));
        if (pathLen > 0) {
            u32 copyLen = (extPos == -1) ? pathLen : (u32)extPos;
            strncpy(tempBuffer, pathBuffer, copyLen);
            tempBuffer[copyLen] = '\0';
            pathLen = strlen(tempBuffer);
            strcpy(pathBuffer, tempBuffer);
        }
    }

    getNoPathExtName__Q22ml9CPathUtilFRQ22ml10FixStr64PCc(extBuffer, pathBuffer);

    format__Q22ml10FixStr128FPCce(pathBuffer, &lbl_eu_804FB3A4[0x1C],
                                   lbl_eu_805708D0, extBuffer);

    const char* extStr = &lbl_eu_804FB3A4[0x21];
    u32 extStrLen = strlen(extStr);
    strcat(pathBuffer, extStr);
    pathLen += extStrLen;

    if (strcmp(lbl_eu_805708D0, lbl_eu_80661AC0) == 0) {
        int fileSize = getFileSize__11CDeviceFileFPCc(pathBuffer, 1);
        if (fileSize < 0) {
            if (strstr(pathBuffer, &lbl_eu_804FB3A4[0x25]) == nullptr &&
                strstr(pathBuffer, &lbl_eu_804FB3A4[0x2B]) == nullptr) {
                return;
            }
        }
    }

    if (script->mNameLen > 0) {
        if (strcmp(extBuffer, script->mName) == 0) {
            if (!(script->mFlags & 0x2) && (script->mFlags & 0x1)) {
                return;
            }
        }
    }

    if (script->mFileHandle != nullptr) {
        cancel__11CDeviceFileFP11CFileHandle(script->mFileHandle);
        script->mFileHandle = nullptr;
    }

    if (script->mFlags & 0x8) {
        if (script->mVmContext != nullptr) {
            vmUnlink(script->mVmContext);
        }
    }

    script->mWaitCount = 0;
    script->mName[0] = '\0';
    script->mNameLen = 0;

    u32 flags = script->mFlags;
    flags &= ~0x3C;
    flags &= ~0x0A;
    script->mFlags = flags;

    void* fileHandle = CfRes_readCommonArchive(script->mVmContext, pathBuffer, script);

    script->mFileHandle = fileHandle;
    if (fileHandle != nullptr) {
        u32 nameLen = strlen(extBuffer);
        script->mNameLen = nameLen;
        strcpy(script->mName, extBuffer);
        script->mFlags |= 0x1;
    }
}

// CfScript::waitLoad
void CfScript::waitLoad() {
    if (!(mFlags & 0x2)) return;
    if (!(mFlags & 0x4)) return;

    if (!(mFlags & 0x8)) {
        if (mVmContext != nullptr) {
            vmLink(mVmContext);
            if (mVmContext != nullptr) {
                vmStart(mVmContext);
                mFlags |= 0x8;
            }
        }
    }

    mWaitCount++;
}

// CfScript::update
void CfScript::update() {
    mFlags |= 0x20;
}

// CfScript::OnFileEvent
void CfScript::OnFileEvent() {
    // Complex function - placeholder implementation
}

// CfScriptManager::getInstance - singleton accessor
CfScriptManager* CfScriptManager::getInstance() {
    if (lbl_eu_80663D88 == 0) {
        CfScriptManager* mgr = (CfScriptManager*)lbl_eu_80570918;

        for (int i = 0; i < 3; i++) {
            CfScript& script = mgr->mScripts[i];
            script.mName[0] = '\0';
            script.mNameLen = 0;
            script.mFileHandle = nullptr;
            script.mFlags = 0;
            script.mVmContext = nullptr;
            script.mWaitCount = 0;
            script.mIndex = (u16)i;
        }

        mgr->init();
        lbl_eu_80663D88 = 1;
    }

    return (CfScriptManager*)lbl_eu_80570918;
}

// CfScript constructor
CfScript::CfScript() {
    mName[0] = '\0';
    mNameLen = 0;
    mFileHandle = nullptr;
    mFlags = 0;
    mVmContext = nullptr;
    mWaitCount = 0;
}

// CfScript destructor
CfScript::~CfScript() {
    if (this != nullptr) {
        // Cleanup if needed
    }
}

// CfScriptManager::init
void CfScriptManager::init() {
    vmInit();
    pluginRegist__Fv();

    for (int i = 0; i < 3; i++) {
        CfScript& script = mScripts[i];

        void* vmCtx = nullptr;
        switch (i) {
            case 0:
                vmCtx = func_800A82BC();
                break;
            case 1:
                vmCtx = func_800A837C();
                break;
            case 2:
                vmCtx = func_800A843C();
                break;
        }

        script.mVmContext = vmCtx;
        script.mFileHandle = nullptr;
        script.mWaitCount = 0;
        script.mName[0] = '\0';
        script.mNameLen = 0;
        script.mFlags = 0;
        script.mIndex = (u16)i;
    }
}

// CfScriptManager::func_800694B0 - main update loop
void CfScriptManager::func_800694B0() {
    for (int i = 0; i < 3; i++) {
        CfScript& script = mScripts[i];

        if (script.mWaitCount >= 0 && script.mWaitCount < 2) {
            // Call appropriate function based on waitCount
        }
    }

    if (cf::CfGameManager::func_800829B8() == 0) {
        vmExec();
    }
}

// CfScriptManager::func_8006953C - cleanup all scripts
void CfScriptManager::func_8006953C() {
    // Reset slot 0
    CfScript& s0 = mScripts[0];
    if (s0.mFileHandle != nullptr) {
        cancel__11CDeviceFileFP11CFileHandle(s0.mFileHandle);
        s0.mFileHandle = nullptr;
    }
    if (s0.mFlags & 0x8) {
        if (s0.mVmContext != nullptr) {
            vmUnlink(s0.mVmContext);
        }
    }
    s0.mWaitCount = 0;
    s0.mFlags = 0;
    s0.mName[0] = '\0';
    s0.mNameLen = 0;

    // Reset slot 1
    CfScript& s1 = mScripts[1];
    if (s1.mFileHandle != nullptr) {
        cancel__11CDeviceFileFP11CFileHandle(s1.mFileHandle);
        s1.mFileHandle = nullptr;
    }
    if (s1.mFlags & 0x8) {
        if (s1.mVmContext != nullptr) {
            vmUnlink(s1.mVmContext);
        }
    }
    s1.mWaitCount = 0;
    s1.mFlags = 0;
    s1.mName[0] = '\0';
    s1.mNameLen = 0;

    // Reset slot 2
    CfScript& s2 = mScripts[2];
    if (s2.mFileHandle != nullptr) {
        cancel__11CDeviceFileFP11CFileHandle(s2.mFileHandle);
        s2.mFileHandle = nullptr;
    }
    if (s2.mFlags & 0x8) {
        if (s2.mVmContext != nullptr) {
            vmUnlink(s2.mVmContext);
        }
    }
    s2.mWaitCount = 0;
    s2.mFlags = 0;
    s2.mName[0] = '\0';
    s2.mNameLen = 0;

    vmInit();
}

// ml::FixStr<128>::format
void ml_format_FixStr128(void* self, const char* fmt, ...) {
    char buffer[0x100];
    va_list args;
    va_start(args, fmt);
    vsnprintf(buffer, sizeof(buffer), fmt, args);
    va_end(args);

    u32 len = strlen(buffer);
    *((u32*)((char*)self + 0x80)) = len;
    strcpy((char*)self, buffer);
}

// sinit_800696C8 - static initializer
void sinit_800696C8() {
    const char* src = lbl_eu_80661AC0;
    char* dest = lbl_eu_805708D0;
    u32 len = strlen(src);
    *((u32*)(dest + 0x40)) = len;
    strcpy(dest, src);
}

} // namespace cf