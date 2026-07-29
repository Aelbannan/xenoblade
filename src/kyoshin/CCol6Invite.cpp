// Decompiled code for kyoshin/CCol6Invite

#include "kyoshin/CCol6Invite.hpp"
#include <stdio.h>
#include <string.h>
#include <stdarg.h>

#include "monolib/work/CProcess.hpp"
#include "monolib/work/CWorkThreadSystem.hpp"
#include "monolib/util/MemManager.hpp"

// Singleton instance pointer (lbl_eu_8066423C in retail).
CCol6Invite* gCol6Invite;

// Raw constructor for CProcess base (abstract class, must use C-linkage ctor).
extern "C" void __ct__8CProcessFv(void* self);

// Vtable symbols (declared as opaque byte arrays).
extern "C" u8 lbl_eu_8052FF3C[];  // CCol6Invite primary vtable
extern "C" u8 lbl_eu_8052D238[];  // temporary vtable used during construction
extern "C" const u8 __ptmf_null[12];  // null pointer-to-member-function (all zeros)

// Helper struct to access the vtable pointer at offset 0x10 (after CDoubleListNode).
struct CProcessBase {
    u8 data[0x10];
    u8* vtable;
};

// Buffer layout for func_eu_801651A0: a 0x100-byte string followed by a u32 length.
struct CCol6InviteMsgBuf {
    char str[0x100];
    u32 length;
};

// Factory function for CCol6Invite singleton.
// Returns the singleton instance, or NULL if already created.
//
// RetailASM: checks gCol6Invite first, allocates 0x78 bytes from work memory,
// constructs CProcess base, sets temporary vtable, zeros callbacks, initializes
// all fields, sets final vtable + secondary vtable ptr at 0x6C, then calls Regist.
__attribute__((noinline)) void* __ct__CCol6Invite(CProcess* parent, u16 arg2, u8 arg3, u8 arg4) {
    CCol6Invite* obj;
    u32* ptmf;
    u32 ptmfWord1;
    u32 ptmfWord0;
    u32 ptmfWord2;
    u32* vtFinal;
    u32* vtTmp;

    // Check if singleton already exists.
    if (gCol6Invite != nullptr) {
        return nullptr;
    }

    // Allocate from work memory pool.
    obj = static_cast<CCol6Invite*>(
        mtl::MemManager::allocate(0x78, CWorkThreadSystem::getWorkMem()));

    if (obj != nullptr) {
        // Construct CProcess base class in-place.
        __ct__8CProcessFv(obj);

        // Set temporary vtable (used during field initialization).
        vtTmp = (u32*)lbl_eu_8052D238;
        ((u32*)obj)[4] = (u32)vtTmp;  // offset 0x10 / 4 = 4

        // Initialize callback fields from __ptmf_null (all zeros).
        // Retail loads __ptmf_null addr once (r9), then loads word1, word0, word2
        // and interleaves stores with field initializations.
        ptmf = (u32*)__ptmf_null;
        ptmfWord1 = ptmf[1];
        vtFinal = (u32*)lbl_eu_8052FF3C;
        ptmfWord0 = ptmf[0];
        ((u32*)obj)[21] = 0;  // mField54 = 0
        ((u32*)obj)[15] = ptmfWord0;  // mCallbackA[0]
        ((u32*)obj)[22] = 0;  // mField58 = 0
        ((u32*)obj)[16] = ptmfWord1;  // mCallbackA[1]
        ((u32*)obj)[23] = 0;  // mField5C = 0
        ptmfWord2 = ptmf[2];
        ((u32*)obj)[17] = ptmfWord2;  // mCallbackA[2]
        ptmfWord1 = ptmf[1];
        ptmfWord0 = ptmf[0];
        ((u32*)obj)[18] = ptmfWord0;  // mCallbackB[0]
        ((u32*)obj)[19] = ptmfWord1;  // mCallbackB[1]
        ptmfWord2 = ptmf[2];
        ((u32*)obj)[20] = ptmfWord2;  // mCallbackB[2]

        // Initialize remaining fields to their defaults.
        obj->mIndex = -1;
        obj->mFlag64 = 0;
        obj->mFlag65 = 0;
        obj->mFlag66 = 0;
        obj->mActive = 1;
        obj->mField68 = 0;

        // Set final CCol6Invite vtable.
        ((u32*)obj)[4] = (u32)vtFinal;  // offset 0x10

        // Set secondary vtable pointer: lbl_eu_8052FF3C + 0x24.
        obj->mField6C = (u32)vtFinal + 0x24;

        // Store constructor arguments.
        obj->mArg2 = arg2;
        obj->mArg3 = arg3;
        obj->mArg4 = arg4;
        obj->mField74 = 0;
    }

    // Store singleton pointer (even if allocation failed).
    gCol6Invite = obj;

    // Register with parent process (retail calls Regist even with null obj).
    if (obj != nullptr) {
        obj->Regist(parent, false);
    }

    return gCol6Invite;
}

// Destructor stub.
CCol6Invite::~CCol6Invite() {}

// CProcess overrides (stubs).
void CCol6Invite::Init() {}
void CCol6Invite::Term() {}
void CCol6Invite::Move() {}
void CCol6Invite::Draw() {}

// Standalone string formatting helper.
// Formats a string into a 0x100-byte stack buffer using vsnprintf, writes the
// string length at offset 0x100 of the output buffer, then copies the string.
void func_eu_801651A0(char* buffer, const char* format, ...) {
    char buf[0x100];
    va_list va;
    va_start(va, format);
    vsnprintf(buf, 0x100, format, va);
    va_end(va);

    CCol6InviteMsgBuf* msg = reinterpret_cast<CCol6InviteMsgBuf*>(buffer);
    msg->length = strlen(buf);
    strcpy(msg->str, buf);
}
