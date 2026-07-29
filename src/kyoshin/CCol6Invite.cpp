// Decompiled code for kyoshin/CCol6Invite

#include "kyoshin/CCol6Invite.hpp"
#include <stdio.h>
#include <string.h>
#include <stdarg.h>

// External functions
extern "C" {
    // Returns pool ID from global variable (r13 - 0x6364).
    u32 sub_80439F4C();
    // operator new(size, pool) - allocates from pool.
    void* operator_new_804373F4(u32 size, u32 pool);
    // CProcess constructor.
    void __ct__8CProcessFv(CProcess* proc);
    // CProcess::Regist
    void Regist__8CProcessFP8CProcessb(CProcess* proc, CProcess* parent, bool insertTop);
}

// Singleton instance pointer (lbl_eu_8066423C in retail).
CCol6Invite* gCol6Invite;

// Symbols from data sections
extern u32 lbl_eu_8052FF3C;  // vtable for CCol6Invite
extern u32 lbl_eu_8052D238;  // temporary vtable
extern u8 __ptmf_null[12];   // null pointer-to-member-function (lbl_eu_805139E8)

// Factory function for CCol6Invite singleton.
// Returns the singleton instance, or NULL if already created.
CCol6Invite* CCol6Invite::CCol6Invite(CProcess* parent, u16 arg2, u8 arg3, u8 arg4) {
    // Check if singleton already exists.
    if (gCol6Invite != nullptr) {
        return nullptr;
    }

    // Get pool ID and allocate.
    u32 pool = sub_80439F4C();
    CCol6Invite* obj = (CCol6Invite*)operator_new_804373F4(0x78, pool);
    if (obj == nullptr) {
        return nullptr;
    }

    // Call CProcess constructor.
    __ct__8CProcessFv(obj);

    // Set temporary vtable.
    *(u32*)((u8*)obj + 0x10) = lbl_eu_8052D238;

    // Initialize callback fields from __ptmf_null (all zeros).
    memcpy((u8*)obj + 0x3C, __ptmf_null, 12);
    memcpy((u8*)obj + 0x48, __ptmf_null, 12);

    // Initialize remaining fields.
    *(u32*)((u8*)obj + 0x54) = 0;
    *(u32*)((u8*)obj + 0x58) = 0;
    *(u32*)((u8*)obj + 0x5C) = 0;
    *(s32*)((u8*)obj + 0x60) = -1;
    *(u8*)((u8*)obj + 0x64) = 0;
    *(u8*)((u8*)obj + 0x65) = 0;
    *(u8*)((u8*)obj + 0x66) = 0;
    *(u8*)((u8*)obj + 0x67) = 1;
    *(u32*)((u8*)obj + 0x68) = 0;

    // Set final vtable and field at 0x6C.
    *(u32*)((u8*)obj + 0x10) = lbl_eu_8052FF3C;
    *(u32*)((u8*)obj + 0x6C) = lbl_eu_8052FF3C + 0x24;

    // Store arguments.
    *(u16*)((u8*)obj + 0x70) = arg2;
    *(u8*)((u8*)obj + 0x72) = arg3;
    *(u8*)((u8*)obj + 0x73) = arg4;
    *(u8*)((u8*)obj + 0x74) = 0;

    // Store singleton.
    gCol6Invite = obj;

    // Register with parent.
    Regist__8CProcessFP8CProcessb(obj, parent, false);

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
// Formats a string into buffer, stores length at buffer+0x100.
void func_eu_801651A0(char* buffer, const char* format, ...) {
    char buf[0x100];
    va_list va;
    va_start(va, format);
    vsnprintf(buf, 0x100, format, va);
    va_end(va);
    *(u32*)(buffer + 0x100) = strlen(buf);
    strcpy(buffer, buf);
}
