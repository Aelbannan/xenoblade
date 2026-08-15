// monolib_eu_804F9E98 - request-record setup/poll helpers and a `.ctors`
// static initializer.
//
// Both helpers operate on a small "request" record: a fixed inline string
// payload (0x00-0x0c) followed by two byte flags (0x0d, 0x0e) and a signed
// lifecycle-state byte (0x0f). func_eu_804F9E98 builds such a record and
// returns a module-global string pointer; func_eu_804F9EE0 polls the record's
// lifecycle state.
//
// sinit_eu_804F9FA4 installs the module-global string pointer via a retail
// tail-call thunk into the adjacent helper func_804F9FAC (the annotation
// originally merged the two bodies into one 0x18 symbol; see
// MWCC_REFERENCE "b .+4 sinit ceiling" RESOLVED note).

#include <types.h>
#include "monolib/core/monolib_eu_804F9E98.hpp"
#include <string.h>

// ---- Global storage referenced by this TU ---------------------------------
// These names must equal the retail linker symbols, so they are not renamed.
// The storage itself is owned by the retail data split monolibdata2.s (sbss
// 0x806659D0/0x806659D4/0x80665A98), so the MWCC TU must only *reference* the
// symbols: a TU-local definition would create a local .sbss section (data
// diff FAIL) and duplicate the symbol at link time (see MWCC_REFERENCE
// "monolibdata2-owned sbss globals must be extern").

extern char* lbl_eu_80665A98;  // sbss:0x448 - module-global string pointer;
                               //               filled by sinit, returned by
                               //               func_eu_804F9E98.
extern char lbl_eu_80570410[];  // data:0x44A8 - a string constant. Declared as
                                // char[] to avoid SDA-based addressing.
extern u8   lbl_eu_806659D0;    // sbss:0x380 - activity guard flag.
extern s32  lbl_eu_806659D4;    // sbss:0x384 - last client error code.

// ---- External request-processing functions (retail C-linkage names) ------
// Declarations live in monolib/core/monolib_eu_804F9E98.hpp.

struct MonoRequestState;  // forward decl; full layout below

// ---- Request record shared by both helpers --------------------------------

struct MonoRequestState {
    char payload[0xd];  // inline string data, 0x00-0x0c
    u8   field_0xD;     // 0x0d
    u8   field_0xE;     // 0x0e
    s8   state;         // 0x0f lifecycle state
};

// us-804f9e98: func_eu_804F9E98  size=0x48
// Copies src into the request's string payload, stamps the two payload bytes,
// clears the lifecycle state, and returns the module-global string pointer.
extern "C" char** func_eu_804F9E98(MonoRequestState* req, const char* src,  // returns &lbl_eu_80665A98
                                   u8 flagD, u8 flagE) {
    strcpy(req->payload, src);
    req->field_0xD = flagD;
    req->field_0xE = flagE;
    req->state = 0;
    return &lbl_eu_80665A98;
}

// us-804f9ee0: func_eu_804F9EE0  size=0xC4
// Lifecycle poll. The record's signed state byte drives a small state machine:
//   returns 0 = still busy, 1 = done, 2 = error.
extern "C" int func_eu_804F9EE0(u8* unused, MonoRequestState* req) {  // polls lbl_eu_806659D0/D4
    if (lbl_eu_806659D0 != 0) return 0;

    switch (req->state) {
        case 0: {  // kick off the request
            s32 result = func_804DA9C4(req, req->field_0xE);
            if (func_eu_804DEB4C(result, req->field_0xD, 0) != 0) return 2;
            req->state = 1;
            break;
        }
        case 1:  // wait for completion
            if (lbl_eu_806659D4 != 0 && lbl_eu_806659D4 != -6) return 2;
            req->state = 2;
            break;
        case 2:  // done
            return 1;
        default:
            break;
    }
    return 0;
}

// us-804f9fa4: sinit_eu_804F9FA4  size=0x8 (+ helper func_804F9FAC size=0x10)
// `.ctors` static initializer: installs the module-global string pointer.
// Retail keeps the thunk `li r3,&lbl_eu_80665A98@sda21; b func_804F9FAC` as
// a tail call into the adjacent helper (the annotation originally merged the
// two bodies into one 0x18 symbol).  The helper stores the vtable/string
// address through r3; `char[]` keeps the address constant in a lis/addi pair.
extern "C" __declspec(noinline) void func_804F9FAC(void* dest) {
    *(void**)dest = (void*)lbl_eu_80570410;
}
extern "C" void sinit_eu_804F9FA4() {  // installs lbl_eu_80665A98
    func_804F9FAC(&lbl_eu_80665A98);
}