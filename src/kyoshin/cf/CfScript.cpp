// CfScript.cpp - Script management for cf subsystem
// Based on retail assembly analysis

#include "kyoshin/cf/CfScript.hpp"
#include "kyoshin/cf/CfGameManager.hpp"
#include "kyoshin/plugin/pluginMain.hpp"
#include "monolib/util/FixStr.hpp"
#include "monolib/util/CPathUtil.hpp"
#include "monolib/vm/yvm2.h"
#include <stdio.h>
#include <stdarg.h>
#include <string.h>

// ml::FixStr<128>::format is an explicit specialization defined at the end of
// this TU (retail places it after CfScriptManager::resetScripts). It is
// declared here, before its callers, so member calls bind to the
// specialization instead of instantiating the generic template.
namespace ml {
template <>
void FixStr<128>::format(const char* fmt, ...);
} // namespace ml

namespace cf {

// Forward declarations for functions in this TU
extern "C" char* CfScript_CopyName(char* dest, const char* src);

// CfScript_InitPath - initializer for path string
extern "C" void CfScript_InitPath() {
    CfScript_CopyName(lbl_eu_805708D0, lbl_eu_80661AC0);
}

// CfScript_CopyName - copy string with length tracking; returns dest
extern "C" char* CfScript_CopyName(char* dest, const char* src) {
    u32 len = strlen(src);
    CfScriptNameBuffer* buf = (CfScriptNameBuffer*)dest;
    buf->mLength = len;
    strcpy(buf->mString, src);
    return dest;
}

// CfScript_InitMgr - get singleton and init
extern "C" void CfScript_InitMgr() {
    CfScriptManager* mgr = CfScriptManager::getInstance();
    mgr->init();
}

// CfScript_UpdateMgr - get singleton and call func_800694B0
extern "C" void CfScript_UpdateMgr() {
    CfScriptManager* mgr = CfScriptManager::getInstance();
    mgr->updateScript();
}

// CfScript_ResetMgr - get singleton and call func_8006953C
extern "C" void CfScript_ResetMgr() {
    CfScriptManager* mgr = CfScriptManager::getInstance();
    mgr->resetScripts();
}

// CfScript_LoadSlot0 - get singleton and load slot 0
extern "C" void CfScript_LoadSlot0(const char* name) {
    CfScriptManager* mgr = CfScriptManager::getInstance();
    mgr->loadSlotZero(name);
}

// CfScriptManager::func_80068B20 - forward slot 0 (== this) + name to loader
void CfScriptManager::loadSlotZero(const char* name) {
    CfScript_LoadScript(&mScripts[0], name);
}

// CfScript_LoadSlot1 - get singleton and call CfScript_PutSlot1 (free function: loads
// slot 1 by offsetting the singleton base, mirroring the retail split where
// slot 1 has no member thunk).
extern "C" void CfScript_LoadSlot1(const char* name) {
    CfScriptManager* mgr = CfScriptManager::getInstance();
    mgr->CfScript_PutSlot1(name);
}

// CfScriptManager::CfScript_PutSlot1 - load script into slot 1
void CfScriptManager::CfScript_PutSlot1(const char* name) {
    CfScript* script = &mScripts[1];
    CfScript_LoadScript(script, name);
}

// CfScript_LoadSlot2 - get singleton and call CfScript_PutSlot2
extern "C" void CfScript_LoadSlot2(const char* name) {
    CfScriptManager* mgr = CfScriptManager::getInstance();
    mgr->CfScript_PutSlot2(name);
}

// CfScriptManager::CfScript_PutSlot2 - load script into slot 2
void CfScriptManager::CfScript_PutSlot2(const char* name) {
    CfScript* script = &mScripts[2];
    CfScript_LoadScript(script, name);
}

// CfScript_FlagSlot0 - get singleton and call CfScript_SetFlag0
extern "C" void CfScript_FlagSlot0() {
    CfScriptManager* mgr = CfScriptManager::getInstance();
    mgr->CfScript_SetFlag0();
}

// CfScriptManager::CfScript_SetFlag0 - set flag bit 2 on slot 0
void CfScriptManager::CfScript_SetFlag0() {
    mScripts[0].mFlags |= 0x4;
}

// CfScript_FlagSlot1 - get singleton and call CfScript_SetFlag1
extern "C" void CfScript_FlagSlot1() {
    CfScriptManager* mgr = CfScriptManager::getInstance();
    mgr->CfScript_SetFlag1();
}

// CfScriptManager::CfScript_SetFlag1 - set flag bit 2 on slot 1
void CfScriptManager::CfScript_SetFlag1() {
    mScripts[1].mFlags |= 0x4;
}

// CfScript_FlagSlot2 - get singleton and call CfScript_SetFlag2
extern "C" void CfScript_FlagSlot2() {
    CfScriptManager* mgr = CfScriptManager::getInstance();
    mgr->CfScript_SetFlag2();
}

// CfScriptManager::CfScript_SetFlag2 - set flag bit 2 on slot 2
void CfScriptManager::CfScript_SetFlag2() {
    mScripts[2].mFlags |= 0x4;
}

// CfScript_SleepReq2 - get singleton and call CfScript_SleepSlot2
extern "C" void CfScript_SleepReq2() {
    CfScriptManager* mgr = CfScriptManager::getInstance();
    mgr->CfScript_SleepSlot2();
}

// CfScriptManager::CfScript_SleepSlot2 - sleep VM thread for slot 2 if flag set
void CfScriptManager::CfScript_SleepSlot2() {
    CfScript& script = mScripts[2];
    if (script.mFlags & 0x20) {
        if (script.mVmContext != nullptr) {
            vmThreadSleepAll(script.mVmContext);
        }
    }
}

// CfScript_ClearReq0 - get singleton and call CfScript_ClearSlot0
extern "C" void CfScript_ClearReq0() {
    CfScriptManager* mgr = CfScriptManager::getInstance();
    mgr->CfScript_ClearSlot0();
}

// CfScriptManager::CfScript_ClearSlot0 - reset/cleanup slot 0
void CfScriptManager::CfScript_ClearSlot0() {
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

// CfScript_ClearReq1 - get singleton and call CfScript_ClearSlot1
extern "C" void CfScript_ClearReq1() {
    CfScriptManager* mgr = CfScriptManager::getInstance();
    mgr->CfScript_ClearSlot1();
}

// CfScriptManager::CfScript_ClearSlot1 - reset/cleanup slot 1
void CfScriptManager::CfScript_ClearSlot1() {
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

// CfScript_ClearReq2 - get singleton and call CfScript_ClearSlot2
extern "C" void CfScript_ClearReq2() {
    CfScriptManager* mgr = CfScriptManager::getInstance();
    mgr->CfScript_ClearSlot2();
}

// CfScriptManager::CfScript_ClearSlot2 - reset/cleanup slot 2
void CfScriptManager::CfScript_ClearSlot2() {
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

// CfScript_TestMask - get singleton and call CfScript_TestFlags
extern "C" void CfScript_TestMask(int mask) {
    CfScript_TestFlags(CfScriptManager::getInstance(), 2, mask);
}

// CfScript_TestFlags - check flags with mask (returns (flags & mask) != 0)
extern "C" u32 CfScript_TestFlags(CfScriptManager* mgr, int index, int mask) {
    CfScript& script = mgr->mScripts[index];
    u32 result = script.mFlags & mask;
    return result != 0;
}

// CfScript_MakeName - format or copy string based on condition
// retail: pure tail calls (b CfScript_CopyName / b format); format string is base of lbl_eu_804FB3A4.
extern "C" void CfScript_MakeName(char* dest, const char* src1, const char* src2, const char* src3) {
    if (src3 != nullptr) {
        CfScript_CopyName(dest, src3);
    } else {
        ((ml::FixStr<64>*)dest)->format(lbl_eu_804FB3A4, src1, src2);
    }
}

// CfScript_LoadScript - main script loading function
extern "C" bool CfScript_LoadScript(CfScript* script, const char* name) {
    // Ctor-less raw storage laid out by declaration order (sp+0x08/0x4C/0xD0);
    // retail inlines the FixStr ctors/clear() rather than calling them.
    CfScriptNameBuffer extBuffer;   // 0x08: FixStr<64>-shaped
    u8 pathStorage[0x84];                // 0xD0: ctor-less FixStr<128>
    u8 tempStorage[0x84];           // 0x4C: ctor-less FixStr<128>

    (*(ml::FixStr<128>*)pathStorage).clear();

    // Ignore the two reserved "no-op" script names.
    if (name != nullptr) {
        if (strcmp(name, &lbl_eu_804FB3A4[0xA]) == 0 ||
            strcmp(name, &lbl_eu_804FB3A4[0x14]) == 0) {
            return false;
        }
    }

    // operator=(const char*): strlen + strcpy.
    (*(ml::FixStr<128>*)pathStorage) = name;

    // Backward search for the extension separator; hand-inlined rfind whose
    // result register merges with sepLen after its last use.
    int length = (*(ml::FixStr<128>*)pathStorage).mLength;
    int index;
    if (length == 0) {
        index = -1;
    } else {
        int sepLen = strlen(lbl_eu_80661AD0);
        char* p = &(*(ml::FixStr<128>*)pathStorage).mString[-1] + length;
        // No end local (an end local CSE-shares the single base addi;
        // retail has two). Plain while with the bottom compare; the
        // early exit jumps over the trailing index = -1
        // (goto-gate, PLAN 17.6).
        while (p != &(*(ml::FixStr<128>*)pathStorage).mString[-1]) {
            if (strncmp(p, lbl_eu_80661AD0, sepLen) != 0) {
                p--;
                continue;
            }
            index = (int)(p - (*(ml::FixStr<128>*)pathStorage).mString);
            goto strip_done;
        }
        index = -1;
    strip_done:;
    }

    // Strip the extension so only the bare filename remains.
    if ((u32)(index + 1) > 1u) {
        ((ml::FixStr<128>*)tempStorage)->mString[0] = '\0';
        ((ml::FixStr<128>*)tempStorage)->mLength = 0;
        if (length != 0) {
            int cut = index;
            if (cut == -1) {
                cut = (*(ml::FixStr<128>*)pathStorage).mLength;
            }
            strncpy(((ml::FixStr<128>*)tempStorage)->mString,
                         (*(ml::FixStr<128>*)pathStorage).mString, cut);
            ((ml::FixStr<128>*)tempStorage)->mString[cut] = '\0';
            ((ml::FixStr<128>*)tempStorage)->mLength =
                strlen(((ml::FixStr<128>*)tempStorage)->mString);
        }
        (*(ml::FixStr<128>*)pathStorage) = ((ml::FixStr<128>*)tempStorage)->mString;
    }

    // Build "dir + bare-name" into path (format varargs).
    ml::CPathUtil::getNoPathExtName(*(ml::FixStr<64>*)&extBuffer, (*(ml::FixStr<128>*)pathStorage).mString);
    ml::FixStr<128>& path = *(ml::FixStr<128>*)pathStorage;
    path.format(&lbl_eu_804FB3A4[0x1C], lbl_eu_805708D0, extBuffer.mString);

    // Retail derives the archive extension from the live pool base (addi
    // off the format pool register) and reuses it for strlen/strcat.
    const char* dotSb = &lbl_eu_804FB3A4[0x21];
    (*(ml::FixStr<128>*)pathStorage) += dotSb;

    // If the current directory already matches, sanity-check the file exists.
    // Retail materializes the strcmp == 0 test into a named int temp
    // (MWCC cntlzw/srwi boolean idiom, CR reused by the branch).
    int sameDir = strcmp(lbl_eu_805708D0, lbl_eu_80661AC0) == 0;
    if (sameDir) {
        if (getFileSize__11CDeviceFileFPCc((*(ml::FixStr<128>*)pathStorage).mString, 1) < 0) {
            if (strstr((*(ml::FixStr<128>*)pathStorage).mString, &lbl_eu_804FB3A4[0x25]) == nullptr) {
                strstr((*(ml::FixStr<128>*)pathStorage).mString, &lbl_eu_804FB3A4[0x2B]);
            }
            return false;
        }
    }

    // If the slot holds a different loaded script, leave it alone.
    // Same named-temp boolean idiom as above; retail enters the
    // keep-check when the names are EQUAL.
    if (extBuffer.mLength != 0) {
        int sameScript = strcmp(extBuffer.mString, script->mName) == 0;
        if (sameScript) {
            u32 flags = script->mFlags;
            if (!(flags & 0x2) && (flags & 0x1)) {
                return true;
            }
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
    script->mFlags = script->mFlags & ~0x3B;
    script->mFlags = script->mFlags & ~0x09;

    CFileHandle* handle = CfRes_readCommonArchive(script->mVmContext, (*(ml::FixStr<128>*)pathStorage).mString, script);
    script->mFileHandle = handle;
    // Bool survives the name-store calls in a saved register (retail:
    // neg/or/srwi. r27, then `beq` on the same bits).
    bool loaded = handle != nullptr;
    if (loaded) {
        script->mNameLen = strlen(extBuffer.mString);
        strcpy(script->mName, extBuffer.mString);
        script->mFlags |= 0x1;
    }

    return loaded;
}

// CfScript::waitLoad
void CfScript::waitLoad() {
    if ((mFlags & 0x2) && (mFlags & 0x4)) {
        // If the VM thread is not yet running, link + start it.
        if (!(mFlags & 0x8)) {
            if (vmLink(mVmContext) != 0) {
                vmStart(mVmContext);
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
extern "C" bool OnFileEvent__8CfScriptFP10CEventFile(cf::CfScript* self, ::CEventFile* event) {
    bool ret = false;

    if (self->mFileHandle == event->mFileHandle) {
        if (event->unk0 == 1 && event->field_14 != 0) {
            // Loaded: remember the extension-less name.
            self->mFlags |= 0x2;
            CfScriptNameBuffer tmp;
            ml::CPathUtil::getNoPathExtName(*(ml::FixStr<64>*)&tmp, event->mName);
            self->mNameLen = strlen(tmp.mString);
            strcpy(self->mName, tmp.mString);
        } else {
            // Failed/other event: look for fallback names, mark "ready".
            const char* path = event->mName;
            if (strstr(path, lbl_eu_80661AC0) != nullptr &&
                strstr(path, &lbl_eu_804FB3A4[0x25]) == nullptr) {
                strstr(path, &lbl_eu_804FB3A4[0x2B]);
            }
            self->mFlags |= 0x10;
        }
        self->mFileHandle = nullptr;
        ret = true;
    }

    return ret;
}

// Placement-new[] would emit __construct_new_array + __ct__Q22cf8CfScriptFv;
// retail uses the C-runtime helper with the unmangled ctor label.
extern "C" void __construct_array(void* ptr, void* ctor, void* dtor, u32 size, u32 n);
extern "C" void __ct__cf_CfScript(CfScript* self);
extern "C" void __dt__Q22cf8CfScriptFv(CfScript* self, s16 flags);

// CfScriptManager::getInstance - singleton accessor
CfScriptManager* CfScriptManager::getInstance() {
    if (!lbl_eu_80663D88) {
        CfScriptManager* mgr = (CfScriptManager*)lbl_eu_80570918;
        // Construct the 3-script array in place at the static manager address
        // (retail emits __construct_array with CfScript ctor/dtor, size 0x58, count 3).
        __construct_array(mgr->mScripts, (void*)__ct__cf_CfScript,
                          (void*)__dt__Q22cf8CfScriptFv, 0x58, 3);
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
    pluginRegist();

    for (u32 i = 0; i < 3; i++) {
        CfScript& script = mScripts[i];

        // Per-slot VM constructor (retail: if/else-if chain, cmpwi/cmplwi).
        u8* vmCtx = nullptr;
        if (i == 0) {
            vmCtx = KyoshinHeap_GetFieldA0();
        } else if (i == 1) {
            vmCtx = KyoshinHeap_GetFieldA4();
        } else if (i == 2) {
            vmCtx = KyoshinHeap_GetFieldA8();
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
void CfScriptManager::updateScript() {
    for (u32 i = 0; i < 3; i++) {
        CfScript& script = mScripts[i];

        if (script.mWaitCount >= 0 && (u32)script.mWaitCount < 2) {
            (script.*lbl_eu_80526DD0[script.mWaitCount])();
        }
    }

    if (cf::CfGameManager::isSceneLoading() == 0) {
        vmExec();
    }
}

// CfScriptManager::func_8006953C - cleanup: reset slots 0 and 1, then re-init VM.
void CfScriptManager::resetScripts() {
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

// sinit_800696C8 - static initializer
void sinit_800696C8() {
    const char* src = lbl_eu_80661AC0;
    char* dest = lbl_eu_805708D0;
    u32 len = strlen(src);
    ((CfScriptNameBuffer*)dest)->mLength = len;
    strcpy(dest, src);
}

} // namespace cf

// ============================================================================
// Retail data sections (wave DL class C rebuild, byte-faithful).
//
// The retail unit owns .rodata (0x50) / .data (0x350) / .bss (0x15C) /
// .sdata (0x18) / .sbss (0x8); the decomp TU previously emitted none of
// them, so every data label was a link-frontier row. Shapes decoded from
// the retail object (objdump -dr) and from the consumers:
// - .data 0x50 tables: hand-rolled resource-descriptor tables (NOT C++
//   vtables: all 19 function slots point at CfRes/IResInfo externals, and
//   the TU defines no key function). IResInfo.cpp stores their ADDRESSES
//   into its ctor slots ("resource-object vtable") and CfRes.o's own
//   .data tables point at the companion 0x10 descriptors, so the honest
//   shape is arrays of pointers (same idiom as CfBdat/CfGimmickItem).
// - .data 0x18: the CfScript waitLoad/update PMF pair (2 x 12 bytes:
//   {0, -1, func}) dispatched by updateScript via __ptmf_scall.
// - .data 0x88: the CfScript IWorkEvent vtable blob [ti, 0, dtor, ...]
//   (same [ti, 0, ...] head shape as CfBdat's 8052E718); the class stays
//   novtable with the ctor storing the label explicitly, so no TU emits
//   a competing __vt__ symbol.
// - .data 0x10 tail (80526E70): {__RTTI__10IWorkEvent, 0, 0, 0}, the
//   base-descriptor the .sdata typeinfo points at (CfBdat 8052E7A0 shape).
// - .rodata head: "/script/jp/" (sdata pointer target) + "cf::CfScript"
//   (typeinfo name, referenced by the .sdata typeinfo), then the 0x34
//   script-name/extension string pool the code indexes as lbl_eu_804FB3A4+N.
// - .sdata: script-dir string pointer (8-byte slot: pointer + pad so the
//   typeinfo pair lands at +8), {name, base} typeinfo pair, "." separator.
// - .bss: path buffer (0x48), manager storage (0x108 = 3 x 0x58 CfScript),
//   camera-default Vec3 (0xC, copied as 3 words into CfCam's 80570A2C by
//   resetCamConsts).
// - .sbss: singleton-init flag.
// - .ctors: {sinit_800696C8} (retail static-init entry).
// ============================================================================

extern "C" {
// Resource-table function slots (all defined in the linked CfRes.o /
// IResInfo.o retail objects under these bare names; signatures are opaque
// to the tables, so plain void(void) declarations carry the relocs).
extern "C" void CfRes_getLazyField50(void);
void CfRes_getLimit512b(void);
void isResFlag1(void);
void resNoop(void);
void CfRes_stubFalse_64388(void);
void CfRes_stubFalse_643A8(void);
void isResFlag4(void);
void CfRes_stubFalse_643E8(void);
void CfRes_stubFalse_643C8(void);
void isResFlag3(void);
void isResFlag2(void);
void CfRes_stub_63C2C(void);
void CfRes_stub_63C78(void);
void CfRes_isField40NonZero(void);
void CfRes_checkObjState(void);
void CfRes_cmpField4Eq(void);
void CfRes_isEntryValid(void);
void isResFlag0(void);
void CfRes_dispatchTypeA(void);
void CfRes_getLimit512c(void);
void CfRes_dispatchTypeB(void);
extern "C" void CfRes_cacheSoundBase(void);
extern "C" void CfRes_getLazyFieldAC(void);
void CfRes_getLimit32(void);
void CfRes_getField24(void);
void CfRes_stub_65D8C(void);
extern "C" void CfRes_cacheEffectBase(void);
extern "C" void CfRes_getLazyField18(void);
void CfRes_getLimit512d(void);
extern "C" void CfRes_getLazyField20b(void);
void CfRes_getLimit512e(void);
extern "C" void CfRes_getLazyDeviceBuf(void);
void CfRes_getLimit512f(void);
void CfRes_stub_65D78(void);
void CfRes_stub_65D7C(void);
extern "C" void CfRes_getLazyField10b(void);
extern "C" void CfRes_checkEntryReady(void);
void CfRes_vcall16(void);
void getResMaxCount(void);
// CfScript vtable slots owned by other TUs (defined in linked CGame.o).
void WorkEvent1__10IWorkEventFPvPCc(void);
void WorkEvent3__10IWorkEventFPv(void);
void WorkEvent4__10IWorkEventFv(void);
void OnPauseTrigger__10IWorkEventFb(void);
void WorkEvent6__10IWorkEventFv(void);
void WorkEvent7__10IWorkEventFv(void);
void WorkEvent8__10IWorkEventFv(void);
void WorkEvent9__10IWorkEventFv(void);
void WorkEvent10__10IWorkEventFv(void);
void WorkEvent11__10IWorkEventFv(void);
void WorkEvent12__10IWorkEventFv(void);
void WorkEvent13__10IWorkEventFv(void);
void WorkEvent14__10IWorkEventFv(void);
void WorkEvent15__10IWorkEventFv(void);
void WorkEvent16__10IWorkEventFv(void);
void WorkEvent17__10IWorkEventFv(void);
void WorkEvent18__10IWorkEventFv(void);
void WorkEvent19__10IWorkEventFv(void);
void WorkEvent20__10IWorkEventFv(void);
void WorkEvent21__10IWorkEventFv(void);
void WorkEvent22__10IWorkEventFv(void);
void WorkEvent23__10IWorkEventFv(void);
void WorkEvent24__10IWorkEventFv(void);
void WorkEvent25__10IWorkEventFv(void);
void WorkEvent26__10IWorkEventFv(void);
void WorkEvent27__10IWorkEventFv(void);
void WorkEvent28__10IWorkEventFv(void);
void WorkEvent29__10IWorkEventFv(void);
void WorkEvent30__10IWorkEventFv(void);
void WorkEvent31__10IWorkEventFv(void);
// Deleting dtor: defined in this TU as cf::CfScript::~CfScript (member
// definition, C++ linkage, same mangled symbol); declared here so the
// vtable below can take its address (same idiom as the TU's existing
// member-decl + extern "C"-def pairing for setRootPaneTranslate).
void __dt__Q22cf8CfScriptFv(void);
// RTTI base (defined in linked CGame.o; CfBdat-8052E7A0 idiom).
// Declared as an incomplete array: a scalar `extern u32` tentative-defines
// into our .sbss under MWCC, while an array bound MWCC cannot size stays a
// pure UNDEF reference.
extern "C" u32 __RTTI__10IWorkEvent[];
} // extern "C"

// CfRes-owned .sdata descriptor words referenced by the tables below
// (8-byte objects; array-of-u32 is the neutral spelling, cf. IResInfo.cpp).
extern u32 __RTTI__Q22cf12IResInfoTBox[2];
extern u32 __RTTI__Q22cf10IResInfoSe[2];
extern u32 __RTTI__Q22cf14IResInfoEffect[2];
extern u32 __RTTI__Q22cf11IResInfoLOD[2];
extern u32 __RTTI__Q22cf10IResInfoID[2];
extern u32 __RTTI__Q22cf18IResInfoPreloadMap[2];
extern u32 __RTTI__Q22cf11IResInfoMap[2];
extern u32 __RTTI__Q22cf8IResInfo[2];
// Forward declaration for the .sdata typeinfo initializer below
// (defined in the .data block, retail order).
extern u32 lbl_eu_80526E70[4];

// --- .rodata (retail 0x50): dir prefix + typeinfo name + string pool ---
// NOTE: no aligned() attributes here: an alignment attribute rounds the
// object's footprint up (the 0x0C head would occupy 0x10 and shift the
// typeinfo name off its retail offset). MWCC packs these in source order.
__declspec(section ".rodata") __attribute__((used))
char lbl_eu_804FB388[0x0C] = "/script/jp/";
// MWCC packs .rodata by size tier (16-byte arrays go 8/16-aligned, smaller
// ones pack tight): exact-sized [13] lands at +0x0C, and the [0x34] pool
// 4-aligns to +0x1C with its 3 lead bytes as section pad (all zero, matching
// retail). No aligned() attributes: they can only widen the footprint.
__declspec(section ".rodata") __attribute__((used))
char lbl_eu_804FB394[13] = "cf::CfScript";
__declspec(section ".rodata") __attribute__((used))
char lbl_eu_804FB3A4[0x34] =
    "%02d%02df\0"
    "eventView\0"
    "abcd_00\0"
    "%s%s\0"
    ".sb\0"
    "am.sb\0"
    "pm.sb\0";

// --- .sdata (retail 0x18) ---
__declspec(section ".sdata") __attribute__((used, aligned(8)))
char* lbl_eu_80661AC0 = (char*)lbl_eu_804FB388;
__declspec(section ".sdata") __attribute__((used, aligned(8)))
void* __RTTI__Q22cf8CfScript[2] = { lbl_eu_804FB394, lbl_eu_80526E70 };
__declspec(section ".sdata") __attribute__((used))
// Sized [8]: the 2-byte "." string plus the section's 6-byte tail fill
// (retail sh_size 0x18 over 0x12 content; same tail-fill idiom as
// CfGimmickItem's rodata pad). Auditor is offset-only; the gate needs.
char lbl_eu_80661AD0[8] = ".";

// --- .data (retail 0x350): descriptor tables in retail order ---
__declspec(section ".data") __attribute__((used, aligned(8)))
void* lbl_eu_80526B30[20] = {
    __RTTI__Q22cf12IResInfoTBox, 0,
    (void*)&CfRes_getLazyField50, (void*)&CfRes_getLimit512b,
    (void*)&isResFlag1, (void*)&resNoop,
    (void*)&CfRes_stubFalse_64388, (void*)&CfRes_stubFalse_643A8,
    (void*)&isResFlag4, (void*)&CfRes_stubFalse_643E8,
    (void*)&CfRes_stubFalse_643C8, (void*)&isResFlag3,
    (void*)&isResFlag2, (void*)&CfRes_stub_63C2C,
    (void*)&CfRes_stub_63C78, (void*)&CfRes_isField40NonZero,
    (void*)&CfRes_checkObjState, (void*)&CfRes_cmpField4Eq,
    (void*)&CfRes_isEntryValid, (void*)&isResFlag0,
};
__declspec(section ".data") __attribute__((used))
void* lbl_eu_80526B80[4] = { __RTTI__Q22cf8IResInfo, 0, 0, 0 };
__declspec(section ".data") __attribute__((used))
void* lbl_eu_80526B90[20] = {
    __RTTI__Q22cf10IResInfoSe, 0,
    (void*)&CfRes_dispatchTypeA, (void*)&CfRes_getLimit512c,
    (void*)&isResFlag1, (void*)&resNoop,
    (void*)&CfRes_stubFalse_64388, (void*)&CfRes_stubFalse_643A8,
    (void*)&isResFlag4, (void*)&CfRes_stubFalse_643E8,
    (void*)&CfRes_stubFalse_643C8, (void*)&isResFlag3,
    (void*)&isResFlag2, (void*)&CfRes_dispatchTypeB,
    (void*)&CfRes_cacheSoundBase, (void*)&CfRes_isField40NonZero,
    (void*)&CfRes_checkObjState, (void*)&CfRes_cmpField4Eq,
    (void*)&CfRes_isEntryValid, (void*)&isResFlag0,
};
__declspec(section ".data") __attribute__((used))
void* lbl_eu_80526BE0[4] = { __RTTI__Q22cf8IResInfo, 0, 0, 0 };
__declspec(section ".data") __attribute__((used))
void* lbl_eu_80526BF0[20] = {
    __RTTI__Q22cf14IResInfoEffect, 0,
    (void*)&CfRes_getLazyFieldAC, (void*)&CfRes_getLimit32,
    (void*)&isResFlag1, (void*)&resNoop,
    (void*)&CfRes_stubFalse_64388, (void*)&CfRes_stubFalse_643A8,
    (void*)&isResFlag4, (void*)&CfRes_stubFalse_643E8,
    (void*)&CfRes_stubFalse_643C8, (void*)&CfRes_getField24,
    (void*)&isResFlag2, (void*)&CfRes_stub_65D8C,
    (void*)&CfRes_cacheEffectBase, (void*)&CfRes_isField40NonZero,
    (void*)&CfRes_checkObjState, (void*)&CfRes_cmpField4Eq,
    (void*)&CfRes_isEntryValid, (void*)&isResFlag0,
};
__declspec(section ".data") __attribute__((used))
void* lbl_eu_80526C40[4] = { __RTTI__Q22cf8IResInfo, 0, 0, 0 };
__declspec(section ".data") __attribute__((used))
void* lbl_eu_80526C50[20] = {
    __RTTI__Q22cf11IResInfoLOD, 0,
    (void*)&CfRes_getLazyField18, (void*)&CfRes_getLimit512d,
    (void*)&isResFlag1, (void*)&resNoop,
    (void*)&CfRes_stubFalse_64388, (void*)&CfRes_stubFalse_643A8,
    (void*)&isResFlag4, (void*)&CfRes_stubFalse_643E8,
    (void*)&CfRes_stubFalse_643C8, (void*)&isResFlag3,
    (void*)&isResFlag2, (void*)&CfRes_stub_63C2C,
    (void*)&CfRes_stub_63C78, (void*)&CfRes_isField40NonZero,
    (void*)&CfRes_checkObjState, (void*)&CfRes_cmpField4Eq,
    (void*)&CfRes_isEntryValid, (void*)&isResFlag0,
};
__declspec(section ".data") __attribute__((used))
void* lbl_eu_80526CA0[4] = { __RTTI__Q22cf8IResInfo, 0, 0, 0 };
__declspec(section ".data") __attribute__((used))
void* lbl_eu_80526CB0[20] = {
    __RTTI__Q22cf10IResInfoID, 0,
    (void*)&CfRes_getLazyField20b, (void*)&CfRes_getLimit512e,
    (void*)&isResFlag1, (void*)&resNoop,
    (void*)&CfRes_stubFalse_64388, (void*)&CfRes_stubFalse_643A8,
    (void*)&isResFlag4, (void*)&CfRes_stubFalse_643E8,
    (void*)&CfRes_stubFalse_643C8, (void*)&isResFlag3,
    (void*)&isResFlag2, (void*)&CfRes_stub_63C2C,
    (void*)&CfRes_stub_63C78, (void*)&CfRes_isField40NonZero,
    (void*)&CfRes_checkObjState, (void*)&CfRes_cmpField4Eq,
    (void*)&CfRes_isEntryValid, (void*)&isResFlag0,
};
__declspec(section ".data") __attribute__((used))
void* lbl_eu_80526D00[4] = { __RTTI__Q22cf8IResInfo, 0, 0, 0 };
__declspec(section ".data") __attribute__((used))
void* lbl_eu_80526D10[20] = {
    __RTTI__Q22cf18IResInfoPreloadMap, 0,
    (void*)&CfRes_getLazyDeviceBuf, (void*)&CfRes_getLimit512f,
    (void*)&isResFlag1, (void*)&resNoop,
    (void*)&CfRes_stubFalse_64388, (void*)&CfRes_stubFalse_643A8,
    (void*)&isResFlag4, (void*)&CfRes_stubFalse_643E8,
    (void*)&CfRes_stubFalse_643C8, (void*)&isResFlag3,
    (void*)&isResFlag2, (void*)&CfRes_stub_65D78,
    (void*)&CfRes_stub_65D7C, (void*)&CfRes_isField40NonZero,
    (void*)&CfRes_checkObjState, (void*)&CfRes_cmpField4Eq,
    (void*)&CfRes_isEntryValid, (void*)&isResFlag0,
};
__declspec(section ".data") __attribute__((used))
void* lbl_eu_80526D60[4] = { __RTTI__Q22cf8IResInfo, 0, 0, 0 };
__declspec(section ".data") __attribute__((used))
void* lbl_eu_80526D70[20] = {
    __RTTI__Q22cf11IResInfoMap, 0,
    (void*)&CfRes_getLazyField10b, (void*)&getResMaxCount,
    (void*)&isResFlag1, (void*)&resNoop,
    (void*)&CfRes_stubFalse_64388, (void*)&CfRes_stubFalse_643A8,
    (void*)&isResFlag4, (void*)&CfRes_stubFalse_643E8,
    (void*)&CfRes_stubFalse_643C8, (void*)&isResFlag3,
    (void*)&isResFlag2, (void*)&CfRes_stub_63C2C,
    (void*)&CfRes_stub_63C78, (void*)&CfRes_isField40NonZero,
    (void*)&CfRes_checkEntryReady, (void*)&CfRes_cmpField4Eq,
    (void*)&CfRes_vcall16, (void*)&isResFlag0,
};
__declspec(section ".data") __attribute__((used))
void* lbl_eu_80526DC0[4] = { __RTTI__Q22cf8IResInfo, 0, 0, 0 };
// PMF dispatch pair selected by mWaitCount (retail bytes {0,-1,func} x2).
__declspec(section ".data") __attribute__((used))
CfScriptPMF lbl_eu_80526DD0[2] = { &cf::CfScript::waitLoad, &cf::CfScript::update };
// CfScript IWorkEvent vtable blob [ti, 0, dtor, ...] (retail 0x88).
__declspec(section ".data") __attribute__((used))
void* lbl_eu_80526DE8[34] = {
    __RTTI__Q22cf8CfScript, 0,
    (void*)&__dt__Q22cf8CfScriptFv,
    (void*)&WorkEvent1__10IWorkEventFPvPCc,
    (void*)&OnFileEvent__8CfScriptFP10CEventFile,
    (void*)&WorkEvent3__10IWorkEventFPv, (void*)&WorkEvent4__10IWorkEventFv,
    (void*)&OnPauseTrigger__10IWorkEventFb,
    (void*)&WorkEvent6__10IWorkEventFv, (void*)&WorkEvent7__10IWorkEventFv,
    (void*)&WorkEvent8__10IWorkEventFv, (void*)&WorkEvent9__10IWorkEventFv,
    (void*)&WorkEvent10__10IWorkEventFv, (void*)&WorkEvent11__10IWorkEventFv,
    (void*)&WorkEvent12__10IWorkEventFv, (void*)&WorkEvent13__10IWorkEventFv,
    (void*)&WorkEvent14__10IWorkEventFv, (void*)&WorkEvent15__10IWorkEventFv,
    (void*)&WorkEvent16__10IWorkEventFv, (void*)&WorkEvent17__10IWorkEventFv,
    (void*)&WorkEvent18__10IWorkEventFv, (void*)&WorkEvent19__10IWorkEventFv,
    (void*)&WorkEvent20__10IWorkEventFv, (void*)&WorkEvent21__10IWorkEventFv,
    (void*)&WorkEvent22__10IWorkEventFv, (void*)&WorkEvent23__10IWorkEventFv,
    (void*)&WorkEvent24__10IWorkEventFv, (void*)&WorkEvent25__10IWorkEventFv,
    (void*)&WorkEvent26__10IWorkEventFv, (void*)&WorkEvent27__10IWorkEventFv,
    (void*)&WorkEvent28__10IWorkEventFv, (void*)&WorkEvent29__10IWorkEventFv,
    (void*)&WorkEvent30__10IWorkEventFv, (void*)&WorkEvent31__10IWorkEventFv,
};
// RTTI base descriptor (CfBdat-8052E7A0 idiom).
__declspec(section ".data") __attribute__((used))
u32 lbl_eu_80526E70[4] = { (u32)__RTTI__10IWorkEvent, 0, 0, 0 };

// --- .bss (retail 0x15C) ---
// NOTE: no aligned() attribute on the path/manager buffers: an alignment
// attribute on the definition perturbs MWCC's scheduling of the users
// (sinit_800696C8, CfScript_LoadScript) away from retail. Section align 8
// is held by the camera vector below (0x150 is 8-aligned, so it is free).
__attribute__((used))
char lbl_eu_805708D0[0x48];
__attribute__((used))
char lbl_eu_80570918[0x108];
// Camera-default vector (copied as 3 words into CfCam's 80570A2C).
__attribute__((used, aligned(8)))
f32 lbl_eu_80570A20[3];

// --- .sbss (retail 0x8): singleton-init flag + section-fill reservation ---
// The file-static sort experiment showed MWCC emits sbss statics ahead of
// globals, so the fill is a plain global (source order kept, offset 0 kept).
__attribute__((used))
s8 lbl_eu_80663D88;
// MWCC 4-aligns sbss items: this lands at +4 (bytes +1..+3 are alignment
// fill) and closes the section at exactly 0x8.
__attribute__((used))
u8 cfscript_sbss_fill[4];

// --- .ctors (retail 1 entry): static-init entry point ---
__declspec(section ".ctors") __attribute__((used))
static void (*cfscript_ctors_entry)() = cf::sinit_800696C8;

// ml::FixStr<128>::format (retail standalone symbol, last in this unit's .text):
// vsnprintf into a 0x100 stack buffer, then copy into the fixed string.
namespace ml {
template <>
void FixStr<128>::format(const char* fmt, ...) {
    char buffer[0x100];
    va_list args;
    va_start(args, fmt);
    vsnprintf(buffer, sizeof(buffer), fmt, args);
    va_end(args);

    mLength = (int)strlen(buffer);
    strcpy(mString, buffer);
}
} // namespace ml
