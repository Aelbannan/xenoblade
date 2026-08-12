// Decompiled code for kyoshin/CCol6Invite

#include "kyoshin/CCol6Invite.hpp"
#include "kyoshin/CCol6System.hpp"
#include "monolib/util/MemManager.hpp"
#include <stdio.h>
#include <string.h>
#include <stdarg.h>

// Retail data symbols referenced by this factory.
extern "C" {
CCol6Invite* lbl_eu_8066423C; // singleton instance (sda21-accessed)
char lbl_eu_8052D238[];       // temporary vtable written before copying __ptmf_null
char lbl_eu_8052FF3C[];       // CCol6Invite final vtable
u32 __ptmf_null[3];           // null pointer-to-member-function constant
void __ct__8CProcessFv(CProcess*); // CProcess base constructor (abstract class, so extern)
}

// Byte-range shim over the CProcess header + owned fields so the factory can
// write the vtable (+0x10), the __ptmf_null callback slots (+0x3C..0x53) and
// the trailing scalars without raw pointer arithmetic. Matches CProcess's
// 0x00-0x3B layout exactly (CProcess.hpp: CProcess : CChildListNode).
struct CCol6InviteCtorShim {
    u8 _00[0x10];
    void* vtable;        // 0x10 - CProcess vtable, overwritten by this factory
    u8 _14[0x28];        // 0x14-0x3B - rest of CProcess
    u32 callbacks[6];    // 0x3C-0x53 - __ptmf_null callback slots
    u32 field54;         // 0x54
    u32 field58;         // 0x58
    u32 field5C;         // 0x5C
    s32 index;           // 0x60 - init -1
    u8 flag64;           // 0x64
    u8 flag65;           // 0x65
    u8 flag66;           // 0x66
    u8 active;           // 0x67 - init 1
    u32 field68;         // 0x68
    u32 field6C;         // 0x6C - lbl_eu_8052FF3C + 0x24
    u16 arg2;            // 0x70
    u8 arg3;             // 0x72
    u8 arg4;             // 0x73
    u8 field74;          // 0x74
};

// Factory for the CCol6Invite singleton. Allocates 0x78 bytes from work
// memory, constructs the CProcess base, fills the callback/field block, stores
// itself as the singleton and registers as a child of `parent`. Mirrors retail
// control flow: the singleton store + Regist run even when the allocation
// failed (r31 stays NULL) and the object (possibly NULL) is returned.
CCol6Invite* CCol6Invite::Create(CProcess* parent, u16 arg2, u8 arg3, u8 arg4) {
    // Return NULL if the singleton already exists.
    if (lbl_eu_8066423C != 0) {
        return 0;
    }

    CCol6InviteCtorShim* obj =
        (CCol6InviteCtorShim*)mtl::MemManager::allocate(
            0x78, CWorkThreadSystem::getWorkMem());

    if (obj != 0) {
        __ct__8CProcessFv((CProcess*)obj);
        obj->vtable = (void*)lbl_eu_8052D238;

        // Copy the null member-function pointer into both callback slots.
        // Retail loads __ptmf_null[1],[0],[2] then stores per slot (load-both /
        // store-both), so use intermediate locals to force the retail ordering.
        const u32* ptmf = __ptmf_null;
        u32 ptmfWord1 = ptmf[1];
        u32 ptmfWord0 = ptmf[0];
        obj->callbacks[0] = ptmfWord0;
        obj->callbacks[1] = ptmfWord1;
        u32 ptmfWord2 = ptmf[2];
        obj->callbacks[2] = ptmfWord2;
        ptmfWord1 = ptmf[1];
        ptmfWord0 = ptmf[0];
        obj->callbacks[3] = ptmfWord0;
        obj->callbacks[4] = ptmfWord1;
        ptmfWord2 = ptmf[2];
        obj->callbacks[5] = ptmfWord2;

        obj->field54 = 0;
        obj->field58 = 0;
        obj->field5C = 0;
        obj->index = -1;
        obj->flag64 = 0;
        obj->flag65 = 0;
        obj->flag66 = 0;
        obj->active = 1;
        obj->field68 = 0;

        obj->vtable = (void*)lbl_eu_8052FF3C;
        obj->field6C = (u32)(lbl_eu_8052FF3C + 0x24);
        obj->arg2 = arg2;
        obj->arg3 = arg3;
        obj->arg4 = arg4;
        obj->field74 = 0;
    }

    lbl_eu_8066423C = (CCol6Invite*)obj;
    ((CProcess*)obj)->Regist(parent, false);
    return lbl_eu_8066423C;
}

// Destructor defined in CCol6System.cpp as the extern "C" free function
// __dt__11CCol6InviteFv (retail form with nested null-guards + delete).

// CProcess overrides (stubs).
void CCol6Invite::Init() {}
void CCol6Invite::Term() {}
void CCol6Invite::Move() {}
void CCol6Invite::Draw() {}

// --- this-adjusting thunks over CCol6Hint / CCol6System (defined in the
// CCol6System TU). Retail "this" lands at an embedded subobject offset; back
// it off to the CProcess/CCol6Hint/CCol6System base and forward.

// func_801640E0: CCol6Hint dtor at subobject +0x6c.
void func_801640E0(u8* self) {
    typedef void* (*DtorFn)(CCol6Hint*);
    ((DtorFn)__dt__9CCol6HintFv)(reinterpret_cast<CCol6Hint*>(self - 0x6c));
}

// func_801640E8: CCol6Hint::cbRenderBefore at subobject +0x70.
void func_801640E8(u8* self) {
    reinterpret_cast<CCol6Hint*>(self - 0x70)->cbRenderBefore();
}

// func_80164100: CCol6System dtor at subobject +0x6c.
void func_80164100(u8* self) {
    typedef void* (*DtorFn)(CCol6System*);
    ((DtorFn)__dt__11CCol6SystemFv)(reinterpret_cast<CCol6System*>(self - 0x6c));
}

// func_80164108: CCol6System::cbRenderBefore at subobject +0x70.
void func_80164108(u8* self) {
    reinterpret_cast<CCol6System*>(self - 0x70)->cbRenderBefore();
}

// func_80164110: CCol6System dtor at subobject +0x70.
void func_80164110(u8* self) {
    typedef void* (*DtorFn)(CCol6System*);
    ((DtorFn)__dt__11CCol6SystemFv)(reinterpret_cast<CCol6System*>(self - 0x70));
}

// func_80164118: CCol6Invite dtor at subobject +0x6c. Backs `this` off to the
// CCol6Invite embedded subobject and tail-calls the non-deleting destructor.
void func_80164118(u8* self) {
    // Retail thunk: subi r3,r3,0x6c; b __dt__11CCol6InviteFv — r4 (flags)
    // passes through untouched, so call through a 1-arg pointer view.
    typedef void* (*DtorFn)(CCol6Invite*);
    ((DtorFn)__dt__11CCol6InviteFv)(reinterpret_cast<CCol6Invite*>(self - 0x6c));
}

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