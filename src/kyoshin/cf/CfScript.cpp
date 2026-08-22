// CfScript.cpp - Script management for cf subsystem
// Based on retail assembly analysis

#include "kyoshin/cf/CfScript.hpp"
#include "kyoshin/cf/CfGameManager.hpp"
#include "monolib/util/FixStr.hpp"
#include "monolib/util/CPathUtil.hpp"
#include "monolib/vm/yvm2.h"
#include <cstring>

// Resolve ml::FixStr<128>::format calls to the explicit specialization so
// they bind to the retail-mangled symbol format__Q22ml10FixStr<128>FPCce
// (the generic-template call drifts to ...11FixStr...).
template <> void ml::FixStr<128>::format(const char* fmt, ...);
#include <cstdio>
#include <cstdarg>

namespace cf {

// Forward declarations for functions in this TU
__declspec(noinline) char* func_80068A30(char* dest, const char* src);
__declspec(noinline) void func_80068B58(CfScriptManager* mgr, const char* name);

// func_80068A20 - initializer for path string
void func_80068A20() {
    func_80068A30(lbl_eu_805708D0, lbl_eu_80661AC0);
}

// func_80068A30 - copy string with length tracking; returns dest
__declspec(noinline) char* func_80068A30(char* dest, const char* src) {
    u32 len = strlen(src);
    *((u32*)(dest + 0x40)) = len;
    strcpy(dest, src);
    return dest;
}

// func_80068A80 - get singleton and init
void func_80068A80() {
    CfScriptManager* mgr = CfScriptManager::getInstance();
    mgr->init();
}

// func_80068AA4 - get singleton and call func_800694B0
extern "C" void func_80068AA4() {
    CfScriptManager* mgr = CfScriptManager::getInstance();
    mgr->func_800694B0();
}

// func_80068AC8 - get singleton and call func_8006953C
void func_80068AC8() {
    CfScriptManager* mgr = CfScriptManager::getInstance();
    mgr->func_8006953C();
}

// func_80068AEC - get singleton and load slot 0
void func_80068AEC(const char* name) {
    CfScriptManager* mgr = CfScriptManager::getInstance();
    mgr->func_80068B20(name);
}

// CfScriptManager::func_80068B20 - forward slot 0 (== this) + name to loader
__declspec(noinline) void CfScriptManager::func_80068B20(const char* name) {
    func_80068ECC(&mScripts[0], name);
}

// func_80068B24 - get singleton and call func_80068B58
void func_80068B24(const char* name) {
    CfScriptManager* mgr = CfScriptManager::getInstance();
    func_80068B58(mgr, name);
}

// func_80068B58 - load script into slot 1
__declspec(noinline) void func_80068B58(CfScriptManager* mgr, const char* name) {
    CfScript* script = &mgr->mScripts[1];
    func_80068ECC(script, name);
}

// func_80068B60 - get singleton and call func_80068B94
void func_80068B60(const char* name) {
    CfScriptManager* mgr = CfScriptManager::getInstance();
    mgr->func_80068B94(name);
}

// CfScriptManager::func_80068B94 - load script into slot 2
__declspec(noinline) void CfScriptManager::func_80068B94(const char* name) {
    CfScript* script = &mScripts[2];
    func_80068ECC(script, name);
}

// func_80068B9C - get singleton and call func_80068BC0
extern "C" void func_80068B9C() {
    CfScriptManager* mgr = CfScriptManager::getInstance();
    mgr->func_80068BC0();
}

// CfScriptManager::func_80068BC0 - set flag bit 2 on slot 0
__declspec(noinline) void CfScriptManager::func_80068BC0() {
    mScripts[0].mFlags |= 0x4;
}

// func_80068BD0 - get singleton and call func_80068BF4
extern "C" void func_80068BD0() {
    CfScriptManager* mgr = CfScriptManager::getInstance();
    mgr->func_80068BF4();
}

// CfScriptManager::func_80068BF4 - set flag bit 2 on slot 1
__declspec(noinline) void CfScriptManager::func_80068BF4() {
    mScripts[1].mFlags |= 0x4;
}

// func_80068C04 - get singleton and call func_80068C28
extern "C" void func_80068C04() {
    CfScriptManager* mgr = CfScriptManager::getInstance();
    mgr->func_80068C28();
}

// CfScriptManager::func_80068C28 - set flag bit 2 on slot 2
__declspec(noinline) void CfScriptManager::func_80068C28() {
    mScripts[2].mFlags |= 0x4;
}

// func_80068C38 - get singleton and call func_80068C5C
extern "C" void func_80068C38() {
    CfScriptManager* mgr = CfScriptManager::getInstance();
    mgr->func_80068C5C();
}

// CfScriptManager::func_80068C5C - sleep VM thread for slot 2 if flag set
__declspec(noinline) void CfScriptManager::func_80068C5C() {
    CfScript& script = mScripts[2];
    if (script.mFlags & 0x20) {
        if (script.mVmContext != nullptr) {
            vmThreadSleepAll((u8*)script.mVmContext);
        }
    }
}

// func_80068C7C - get singleton and call func_80068CA0
extern "C" void func_80068C7C() {
    CfScriptManager* mgr = CfScriptManager::getInstance();
    mgr->func_80068CA0();
}

// CfScriptManager::func_80068CA0 - reset/cleanup slot 0
__declspec(noinline) void CfScriptManager::func_80068CA0() {
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
extern "C" void func_80068D14() {
    CfScriptManager* mgr = CfScriptManager::getInstance();
    mgr->func_80068D38();
}

// CfScriptManager::func_80068D38 - reset/cleanup slot 1
__declspec(noinline) void CfScriptManager::func_80068D38() {
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
extern "C" void func_80068DAC() {
    CfScriptManager* mgr = CfScriptManager::getInstance();
    mgr->func_80068DD0();
}

// CfScriptManager::func_80068DD0 - reset/cleanup slot 2
__declspec(noinline) void CfScriptManager::func_80068DD0() {
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
void func_80068E44(int mask) {
    func_80068E7C(CfScriptManager::getInstance(), 2, mask);
}

// func_80068E7C - check flags with mask (returns (flags & mask) != 0)
extern "C" u32 func_80068E7C(CfScriptManager* mgr, int index, int mask) {
    CfScript& script = mgr->mScripts[index];
    u32 result = script.mFlags & mask;
    return result != 0;
}

// func_80068E9C - format or copy string based on condition
// retail: pure tail calls (b func_80068A30 / b format) - func_80068A30 is noinline
// so the if-branch collapses to a tail call; format string is base of lbl_eu_804FB3A4.
void func_80068E9C(char* dest, const char* src1, const char* src2, const char* src3) {
    if (src3 != nullptr) {
        func_80068A30(dest, src3);
    } else {
        ((ml::FixStr<64>*)dest)->format(lbl_eu_804FB3A4, src1, src2);
    }
}

// func_80068ECC - main script loading function
extern "C" bool func_80068ECC(CfScript* script, const char* name) {
    CfScriptNameBuffer extBuffer;   // 0x08 - no ctor (retail does not zero it)
    char tempBuffer[0x80];          // 0x4C
    u32 tempLen;                    // 0xCC
    ml::FixStr<128> pathBuffer;     // 0xD0 (mLength at 0x150)

    // Ignore the two reserved "no-op" script names.
    if (name != nullptr) {
        if (std::strcmp(name, &lbl_eu_804FB3A4[0xA]) == 0 ||
            std::strcmp(name, &lbl_eu_804FB3A4[0x14]) == 0) {
            return false;
        }
    }

    pathBuffer.mLength = std::strlen(name);
    std::strcpy(pathBuffer.mString, name);

    // Search backward for the extension separator (lbl_eu_80661AD0).  Retail
    // initialises extPos inside the if/else (not before the length test), so
    // the li -1 lands in the branches.
    int extPos;
    if (pathBuffer.mLength > 0) {
        u32 sepLen = std::strlen(lbl_eu_80661AD0);
        char* searchPos = pathBuffer.mString + pathBuffer.mLength - 1;
        char* searchEnd = pathBuffer.mString - 1;
        while (searchPos != searchEnd) {
            if (std::strncmp(searchPos, lbl_eu_80661AD0, sepLen) == 0) {
                extPos = (int)(searchPos - pathBuffer.mString);
                break;
            }
            searchPos--;
        }
        if (searchPos == searchEnd) {
            extPos = -1;
        }
    } else {
        extPos = -1;
    }

    // Cut the extension off so only the bare filename remains.
    if ((u32)(extPos + 1) > 1u) {
        tempBuffer[0] = '\0';
        tempLen = 0;
        if (pathBuffer.mLength > 0) {
            if (extPos == -1) {
                extPos = pathBuffer.mLength;
            }
            std::strncpy(tempBuffer, pathBuffer.mString, extPos);
            tempBuffer[extPos] = '\0';
            tempLen = std::strlen(tempBuffer);
        }
        pathBuffer.mLength = std::strlen(tempBuffer);
        std::strcpy(pathBuffer.mString, tempBuffer);
    }

    // Build "dir + noext-name" into pathBuffer (format varargs).
    ml::CPathUtil::getNoPathExtName(*(ml::FixStr<64>*)&extBuffer, pathBuffer.mString);
    pathBuffer.format(&lbl_eu_804FB3A4[0x1C], lbl_eu_805708D0, extBuffer.mString);

    const char* extStr = &lbl_eu_804FB3A4[0x21];
    u32 extStrLen = std::strlen(extStr);
    std::strcat(pathBuffer.mString, extStr);
    pathBuffer.mLength += extStrLen;

    // If the current directory already matches, sanity-check the file exists.
    if (std::strcmp(lbl_eu_805708D0, lbl_eu_80661AC0) == 0) {
        if (getFileSize__11CDeviceFileFPCc(pathBuffer.mString, 1) < 0) {
            if (std::strstr(pathBuffer.mString, &lbl_eu_804FB3A4[0x25]) == nullptr) {
                std::strstr(pathBuffer.mString, &lbl_eu_804FB3A4[0x2B]);
            }
            return false;
        }
    }

    // If the same script is already loaded, leave it alone.
    if (extBuffer.mLength > 0) {
        if (std::strcmp(extBuffer.mString, script->mName) == 0 &&
            !(script->mFlags & 0x2) && (script->mFlags & 0x1)) {
            return true;
        }
    }

    // Cancel any pending load and unlink a running VM for this slot.
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
    flags &= ~0x3B;
    flags &= ~0x09;
    script->mFlags = flags;

    void* handle = CfRes_readCommonArchive(script->mVmContext, pathBuffer.mString, script);
    script->mFileHandle = handle;
    // Bool computed before the branch so it survives the name-store calls in a
    // saved register (retail: neg/or/srwi. r27, then `beq` on the same bits).
    bool loaded = handle != nullptr;
    if (loaded) {
        script->mNameLen = std::strlen(extBuffer.mString);
        std::strcpy(script->mName, extBuffer.mString);
        script->mFlags |= 0x1;
    }

    return loaded;
}

// CfScript::waitLoad
void CfScript::waitLoad() {
    if ((mFlags & 0x2) && (mFlags & 0x4)) {
        // If the VM thread is not yet running, link + start it.
        if (!(mFlags & 0x8)) {
            if (vmLink((u8*)mVmContext) != 0) {
                vmStart((u8*)mVmContext);
                mFlags |= 0x8;
            }
        }
        mWaitCount++;
    }
}

// CfScript::update
void CfScript::update() {
    mFlags |= 0x20;
}

// CfScript::OnFileEvent (retail OnFileEvent__8CfScriptFP10CEventFile) -
// completion of an async file read.  The retail symbol is a global-scope
// class member; our CfScript sits in namespace cf, so it is emitted under
// the pre-mangled retail name with an explicit self (same convention as
// CUICfManager/CSortMenu).
extern "C" bool OnFileEvent__8CfScriptFP10CEventFile(cf::CfScript* self, cf::CEventFile* event) {
    bool ret = false;

    if (self->mFileHandle == event->field_04) {
        if (event->field_00 == 1 && event->field_14 != 0) {
            // Loaded: remember the extension-less name.
            self->mFlags |= 0x2;
            CfScriptNameBuffer tmp;
            ml::CPathUtil::getNoPathExtName(*(ml::FixStr<64>*)&tmp, event->field_0C);
            self->mNameLen = std::strlen(tmp.mString);
            std::strcpy(self->mName, tmp.mString);
        } else {
            // Failed/other event: look for fallback names, mark "ready".
            const char* path = event->field_0C;
            if (std::strstr(path, lbl_eu_80661AC0) != nullptr &&
                std::strstr(path, &lbl_eu_804FB3A4[0x25]) == nullptr) {
                std::strstr(path, &lbl_eu_804FB3A4[0x2B]);
            }
            self->mFlags |= 0x10;
        }
        self->mFileHandle = nullptr;
        ret = true;
    }

    return ret;
}

// CfScriptManager::getInstance - singleton accessor
__declspec(noinline) CfScriptManager* CfScriptManager::getInstance() {
    if (!lbl_eu_80663D88) {
        CfScriptManager* mgr = (CfScriptManager*)lbl_eu_80570918;
        // Construct the 3-script array in place at the static manager address
        // (retail emits __construct_array with CfScript ctor/dtor, size 0x58, count 3).
        new (mgr->mScripts) CfScript[3];
        mgr->init();
        lbl_eu_80663D88 = 1;
    }

    return (CfScriptManager*)lbl_eu_80570918;
}

// CfScript constructor - the novtable class stores the retail vtable label
// explicitly (MWCC would otherwise emit a __vt__Q22cf8CfScript reloc).
extern "C" void __ct__cf_CfScript(CfScript* self) {
    *(void**)self = (void*)lbl_eu_80526DE8;
    self->mName[0] = '\0';
    self->mNameLen = 0;
    self->mFileHandle = nullptr;
    self->mFlags = 0;
    self->mVmContext = nullptr;
    self->mWaitCount = 0;
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

    for (u32 i = 0; i < 3; i++) {
        CfScript& script = mScripts[i];

        // Per-slot VM constructor (retail: if/else-if chain, cmpwi/cmplwi).
        void* vmCtx = nullptr;
        if (i == 0) {
            vmCtx = func_800A82BC();
        } else if (i == 1) {
            vmCtx = func_800A837C();
        } else if (i == 2) {
            vmCtx = func_800A843C();
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

// CfScriptManager::func_800694B0 - main update loop: while a slot is not yet
// loaded (waitCount 0/1), dispatch its state handler through the retail ptmf
// table lbl_eu_80526DD0 ({waitLoad, update}); then run the VM unless the game
// manager says we are in a menu/frozen state.
__declspec(noinline) void CfScriptManager::func_800694B0() {
    for (u32 i = 0; i < 3; i++) {
        CfScript& script = mScripts[i];

        if (script.mWaitCount >= 0 && (u32)script.mWaitCount < 2) {
            (script.*lbl_eu_80526DD0[script.mWaitCount])();
        }
    }

    if (cf::CfGameManager::func_800829B8() == 0) {
        vmExec();
    }
}

// CfScriptManager::func_8006953C - cleanup: reset slots 0 and 1, then re-init VM.
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

    vmInit();
}

// ml::FixStr<N>::format - vsnprintf into a stack buffer, then copy into the
// fixed string (mString at 0x00, mLength at 0x80 for FixStr<128>).
template <>
void ml::FixStr<128>::format(const char* fmt, ...) {
    char buffer[0x100];
    va_list args;
    va_start(args, fmt);
    vsnprintf(buffer, sizeof(buffer), fmt, args);
    va_end(args);

    mLength = (int)std::strlen(buffer);
    std::strcpy(mString, buffer);
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