// Auto-scaffolded catalog TU for kyoshin/cf/CTaskREvent
// Replace stubs with high-level C/C++ during decomp.

#include <types.h>
#include <string.h>
#include "kyoshin/cf/CTaskREvent.hpp"
#include "kyoshin/cf/CfGameManager.hpp"
#include "monolib/device/CDeviceFile.hpp"
#include "monolib/scn/CScnNw4r.hpp"

extern "C" void cbRenderBefore__Q22cf11CTaskREventFv(void*);

namespace cf {}
using namespace cf;

// --- CTTask<cf::CTaskREvent> out-of-line specializations ---
// The canonical declared-only template emits no bodies; these explicit
// specializations produce the retail standalone Move/Draw/dtor symbols.
template<>
void CTTask<cf::CTaskREvent>::Move() {
    if (mMoveFunc) {
        (static_cast<cf::CTaskREvent*>(this)->*mMoveFunc)();
    }
}

template<>
void CTTask<cf::CTaskREvent>::Draw() {
    if (mDrawFunc) {
        (static_cast<cf::CTaskREvent*>(this)->*mDrawFunc)();
    }
}

template<>
CTTask<cf::CTaskREvent>::~CTTask() {}

class CEventFile;

// Retail ctor symbol is the pre-mangled name __ct__cf_CTaskREvent (a global
// function, not a cf::CTaskREvent member); the member ctor would mangle the
// symbol. Hand-rolled construction in the retail store order: the CProcess
// base ctor, the interim CTTask<CTaskREvent> vtable and null PTMF members,
// the CDeviceVICb subobject, the final five-vtable reset, the CInfoCf /
// CREvtMem subobjects, the field block, the global instance pointer, the
// subobject registrations, and the dimming/frame-target setup.
// noinline keeps the call a real bl (an empty inline body would make MWCC
// fold the ctor away and shrink func_801665A4 below the retail size).
__declspec(noinline) cf::CTaskREvent* __ct__cf_CTaskREvent(cf::CTaskREvent* pMem, CScnNw4r* pScene, CView* pView) {
    // CProcess base ctor.
    __ct__8CProcessFv(reinterpret_cast<CProcess*>(pMem));

    // Interim CTTask<CTaskREvent> vtable at +0x10 (overwritten below) and
    // null PTMF members at +0x3C/+0x48, stored in the retail order 0x40,0x3C,
    // 0x44 then 0x4C,0x48,0x50 (CTaskGameEvt precedent: post-increment derefs
    // of a local pointer fold the first load into lwzu).
    u32* p = reinterpret_cast<u32*>(pMem);
    p[4] = reinterpret_cast<u32>(lbl_eu_805308A8);
    const u32* src = __ptmf_null;
    u32 w0 = *src++;
    u32 w1 = *src++;
    p[0x10] = w1;        // 0x40 mMoveFunc[1]
    p[0xF] = w0;         // 0x3C mMoveFunc[0]
    u32 w2 = *src++;
    p[0x11] = w2;        // 0x44 mMoveFunc[2]
    src = __ptmf_null;
    w1 = *src++;
    w0 = *src++;
    p[0x13] = w0;        // 0x4C mDrawFunc[1]
    p[0x12] = w1;        // 0x48 mDrawFunc[0]
    w2 = *src++;
    p[0x14] = w2;        // 0x50 mDrawFunc[2]

    // CDeviceVICb subobject at +0x54.
    __ct__11CDeviceVICbFv(reinterpret_cast<CDeviceVICb*>(reinterpret_cast<u8*>(pMem) + 0x54));

    // Final CTaskREvent vtable: primary at +0x10, the four secondary
    // sub-vtables at +0x24 / +0x3C / +0x4C / +0x5C into the retail vtable
    // block (materialized as locals to reproduce the addi-then-store burst).
    u8* vtbl = lbl_eu_80530790;
    u8* v54 = vtbl + 0x24;
    u8* v58 = vtbl + 0x3c;
    u8* v5C = vtbl + 0x4c;
    u8* v60 = vtbl + 0x5c;
    p[4] = reinterpret_cast<u32>(vtbl);
    pMem->mVtbl54 = v54;
    pMem->mVtbl58 = v58;
    pMem->mVtbl5C = v5C;
    pMem->mVtbl60 = v60;
    pMem->field_0x64 = 0;
    pMem->field_0x68 = 0;
    pMem->field_0x6C = 0;

    // Embedded subobject ctors (retail C-ABI names - the members have no C++
    // member ctor, so the retail C-linkage ctors are called directly).
    __ct__cf_CInfoCf(reinterpret_cast<cf::CInfoCf*>(pMem->mInfoCf));
    __ct__cf_CREvtMem(&pMem->mEvtMem);

    // Field initialization in retail store order.
    f32 f = lbl_eu_80667628;
    pMem->field_0xB0 = 0;
    pMem->mNameBuf[0] = 0;
    pMem->field_0x1B4 = 0;
    pMem->field_0x1B8 = 0;
    pMem->field_0x1BC = 0;
    pMem->field_0x1C0 = 0;
    pMem->field_0x1C4 = 0;
    pMem->mBuf = 0;
    pMem->mIdx = 0;
    pMem->field_0x1D0 = 0;
    pMem->field_0x1D4 = -1;
    pMem->field_0x1D8 = 0;
    pMem->field_0x1DC = 0;
    pMem->mCri = (CLibCri*)-1;
    pMem->field_0x1E4 = f;
    pMem->field_0x1E8 = f;
    pMem->field_0x1EC = 0;
    pMem->field_0x1F0 = 0;
    pMem->field_0x1F4 = 0;
    // The task doubles as the event manager root (the global is typed on the
    // manager struct; the ctor stores the task pointer).
    lbl_eu_80664240 = reinterpret_cast<CEventMgr*>(pMem);

    // Register the IFlagEvent subobject (+0x58); the null-this guard shape
    // mirrors the dtor's func_8009D514 call.
    cf::IFlagEvent* flagEvent = reinterpret_cast<cf::IFlagEvent*>(pMem);
    if (pMem != nullptr) {
        flagEvent = reinterpret_cast<cf::IFlagEvent*>(reinterpret_cast<u8*>(pMem) + 0x58);
    }
    func_8009D414(flagEvent);

    // Register the IScnRender subobject (+0x5C) with the scene.
    IScnRender* render = reinterpret_cast<IScnRender*>(pMem);
    if (pMem != nullptr) {
        render = reinterpret_cast<IScnRender*>(reinterpret_cast<u8*>(pMem) + 0x5c);
    }
    pScene->addRenderCB(render, 6, 0);

    // Disable Wii Remote auto-sleep / dimming while the event runs.
    WPADSetAutoSleepTime(5);
    VIEnableDimming(1);
    VISetTimeToDimming(0);

    pMem->field_0x6C |= 0x800;
    lbl_eu_80664244 = 2;
    lbl_eu_80664248 = CDeviceVI::getTargetFramerate() * 300;
    return pMem;
}

// Retail D1 dtor written as a free function (CHelp_LandMark precedent): the
// member declaration in the header keeps the vtable slot, but this body has
// full control over the five-vtable reset (lbl_eu_80530790), the subobject
// dtor calls and the delete-flag handling - the retail bytes that MWCC's
// auto dtor machinery (auto __vt__ store) cannot reproduce.
extern "C" __declspec(noinline) cf::CTaskREvent* __dt__Q22cf11CTaskREventFv(cf::CTaskREvent* self, s32 deleteFlag) {
    if (self != nullptr) {
        // Reset all five vtables by hand (retail writes lbl_eu_80530790;
        // 0x10 is the CTTask primary slot, 0x54-0x60 the four secondary
        // subobject slots).
        void* vbase = (void*)lbl_eu_80530790;
        reinterpret_cast<CTaskREventVtblView*>(self)->vtbl10 = vbase;
        self->mVtbl54 = (char*)vbase + 0x24;
        self->mVtbl58 = (char*)vbase + 0x3c;
        self->mVtbl5C = (char*)vbase + 0x4c;
        self->mVtbl60 = (char*)vbase + 0x5c;

        // Unregister the IScnRender subobject from the scene's render list
        // (MWCC null-guards the this+0x5C argument computation; the guard
        // reuses the outer null-compare's CR because nothing in between
        // modifies it).
        IScnRender* render = reinterpret_cast<IScnRender*>(self);
        if (self != nullptr) {
            render = reinterpret_cast<IScnRender*>(reinterpret_cast<u8*>(self) + 0x5c);
        }
        lbl_eu_80663E14->removeRenderCB(render);

        // Unregister the IFlagEvent subobject (same guarded shape).
        cf::IFlagEvent* flagEvent = reinterpret_cast<cf::IFlagEvent*>(self);
        if (self != nullptr) {
            flagEvent = reinterpret_cast<cf::IFlagEvent*>(reinterpret_cast<u8*>(self) + 0x58);
        }
        func_8009D514(flagEvent);

        // Restore Wii Remote auto-sleep / dimming state.
        WPADSetAutoSleepTime(5);
        VIEnableDimming(1);
        VISetTimeToDimming(0);

        lbl_eu_80664240 = 0;

        // Destroy embedded subobjects (retail order: CREvtMem, CInfoCf,
        // CDeviceVICb, then CProcess).
        __dt__Q22cf8CREvtMemFv(&self->mEvtMem, -1);
        __dt__Q22cf7CInfoCfFv(reinterpret_cast<cf::CInfoCf*>(self->mInfoCf), -1);
        __dt__11CDeviceVICbFv(reinterpret_cast<CDeviceVICb*>(&self->mVtbl54), 0);
        if (self != nullptr) {
            __dt__8CProcessFv(reinterpret_cast<CProcess*>(self), 0);
        }

        if (deleteFlag > 0) {
            __dl__FPv(self);
        }
    }
    return self;
}

// Returns 1 while an event sequence is active: manager present, sequence
// index valid, the +0xB0 gate set, or the +0x6C bit0 flag raised. The final
// `mgr &&` reuses the CR1 null compare from the first guard (retail shape).
int func_80164410() {
    CEventMgr* mgr = lbl_eu_80664240;
    if (mgr == 0) return 0;
    if (mgr->field_0x1D4 != -1) return 1;
    if (mgr->field_0xB0 != 0) return 1;
    // Split the `mgr && flag` value into an explicit pointer-test if/else so
    // MWCC materializes the 0/1 word via the CR1 compare (reused from the
    // first guard) and branch-selects the return (MWCC_CASES
    // __wpadGetExtType / HBMDelete patterns).
    u32 flag;
    if (mgr != 0) {
        flag = mgr->field_0x6C & 1;
    } else {
        flag = 0;
    }
    if (flag != 0) {
        return 1;
    } else {
        return 0;
    }
}

// Convert the .sdata u32 counter to float and scale it. MWCC lowers the
// signed u32->float conversion through the 0x4330 double trick (xoris sign
// flip + subtract 2^52+2^31), which is exactly what the retail body shows.
// The magic constant (0x4330000080000000) pools to a TU-local @N label
// whose value equals retail's shared .sdata2 blob lbl_eu_80667630 —
// name-only reloc drift, accepted at EQUIVALENT_MATCH (MWCC_CASES §7i).
float func_80164478() {
    return lbl_eu_8066762C * (float)(s32)lbl_eu_80662384;
}

extern "C" u32 func_801644AC() { return (u32)lbl_eu_80662380; }

extern "C" u32 func_801644B4() { return (u32)lbl_eu_80664240; }
void func_801644BC(u32 arg) {
    CEventMgr* mgr = lbl_eu_80664240;
    if (!mgr) return;
    func_80166150(mgr, arg);
}

// func_8016462C: called with a table index by func_801644D8; stub for an
// as-yet-unmatched helper (C linkage inherited from the header declaration
// so the retail plain symbol is emitted). noinline so the call stays a real
// bl (an empty inline body would make MWCC DCE the whole walk loop).
// Fills one event-table index's character data: 7 rows of 5 bytes (byte
// extracted from CfGameManager::func_8007DE94(row, col) at bits 10-17), 7
// words, then the id halfword (func_80086D98 output) and a flag byte.
__declspec(noinline) void func_8016462C(int index) {
    // Byte rows are indexed 1..7 (row 0 unused): the retail offsets start at
    // 5*1 / 4*1 and step by 5 / 4 per row.
    for (int row = 1; row < 8; row++) {
        for (int col = 0; col <= 4; col++) {
            u32 v = func_8007DE94__Q22cf13CfGameManagerFv(row, col);
            func_8009D5FC()->mByteRows[index * 0x28 + row * 5 + col] = (u8)((v >> 10) & 0xFF);
        }
        func_8009D5FC()->mWordRows[index][row] =
            func_8007DE94__Q22cf13CfGameManagerFv(row, 5);
    }
    u16 hi = 0xC;
    u16 lo = 0;
    func_80086D98__Q22cf13CfGameManagerFv(&hi, &lo);
    func_8009D5FC()->mHalfSlots[index] = (u16)((hi << 8) | lo);
    func_8009D5FC()->mByteFlags[index] = (u8)func_8016DF2C();
}

// Walks the 32-entry event-id table; for each entry strictly above `lower`
// and at most `upper`, forwards the index to func_8016462C. Only runs when
// `type` equals 0x20.
void func_801644D8(cf::CTaskREvent* self, int type, int upper, int lower) {
    if (type != 0x20) return;
    for (u32 i = 0; i < 0x20; i++) {
        s16 v = lbl_eu_80502F90[i];
        if (v > lower && v <= upper) {
            func_8016462C(i);
        }
    }
}

// Initializes the 32-slot event data table: each slot's u16 id is set to
// 0xC00, its u8 flag cleared, its two 0x10-byte word sub-slots zeroed, and
// its two 0x14-byte byte sub-slots filled with 1.
// NOTE: retail keeps the inner (trip-2) loop as mtctr/bdnz; under this
// unit's -O4,p MWCC fully unrolls constant-trip loops instead (wall #6,
// needs an -O4,s unit-flag split).
void func_8016455C(CEventDataTable* self) {
    for (int i = 0; i < 0x20; i++) {
        self->mIds920[i] = 0xC00;
        self->mFlags900[i] = 0;
        CEventDataTable::WordSlot* w = &self->mWords[i][0];
        CEventDataTable::ByteSlot* b = &self->mBytes[i][0];
        for (int j = 0; j < 2; j++) {
            w->data[0] = 0;
            w->data[1] = 0;
            w->data[2] = 0;
            w->data[3] = 0;
            b->data[0] = 1;
            b->data[1] = 1;
            b->data[2] = 1;
            b->data[3] = 1;
            b->data[4] = 1;
            b->data[5] = 1;
            b->data[6] = 1;
            b->data[7] = 1;
            b->data[8] = 1;
            b->data[9] = 1;
            b->data[10] = 1;
            b->data[11] = 1;
            b->data[12] = 1;
            b->data[13] = 1;
            b->data[14] = 1;
            b->data[15] = 1;
            b->data[16] = 1;
            b->data[17] = 1;
            b->data[18] = 1;
            b->data[19] = 1;
            w++;
            b++;
        }
    }
}

// Local string-build workspace shared by the event-name table walkers: buf at
// +0x00, running total at +0x20. The `len` member is volatile because the
// retail keeps the strlen-derived combined length in a stack slot that is
// never read again (a dead store the retail compiler did not eliminate).
struct CTaskREventNameBuf {
    char buf[0x20];
    volatile int len;
};

// Walks the 32-entry event-name table; for each entry builds the
// "lbl_eu_80503008" + suffix string and compares it with `key`. On a match
// reads the [idx][type][slot] byte from the character blob (0x388A base,
// 0x28-per-index rows of 5-byte entries), clamps it to 1, and packs it into
// an event command word: slot | (value << 10) | (type << 20) | 0x10000000.
// Called from CREvtModelPc.cpp with (name, entryId, slot).
u32 func_80164724(const char* key, int type, u32 slot) {
    CTaskREventNameBuf sb;
    const char* base = lbl_eu_80503008;
    const char* const* tbl = lbl_eu_80530710;
    int idx = -1;
    for (u32 i = 0; i < 0x20; i++) {
        sb.len = strlen(base);
        strcpy(sb.buf, base);
        int len2 = strlen(tbl[i]);
        strcat(sb.buf, tbl[i]);
        sb.len += len2;
        if (strcmp(sb.buf, key) == 0) {
            idx = (int)i;
            break;
        }
    }
    CEventCharBlob* blob = func_8009D5FC();
    u8 v = blob->field_0x00[0x388A + idx * 0x28 + type * 5 + slot];
    if (v == 0) v = 1;
    if (v > 0x63) v = 1;
    if (type == 8 && v == 1) v = 4;
    return slot | ((u32)v << 10) | (((u32)type << 20) | 0x10000000);
}

// Walks the 32-entry event-name table: for each entry builds the
// "lbl_eu_80503008" + suffix string and compares it with `key`. On a match
// returns the per-index word at blob->mWordRows[idx][slot], else 0. The
// strlen-derived combined length is kept in the shared CTaskREventNameBuf
// workspace (dead store reproduced via the volatile member, see above).
int func_80164838(const char* key, int slot) {
    CTaskREventNameBuf sb;
    const char* base = lbl_eu_80503008;
    const char* const* tbl = lbl_eu_80530710;
    int idx = -1;
    for (u32 i = 0; i < 0x20; i++) {
        sb.len = strlen(base);
        strcpy(sb.buf, base);
        int len2 = strlen(tbl[i]);
        strcat(sb.buf, tbl[i]);
        sb.len += len2;
        if (strcmp(sb.buf, key) == 0) {
            idx = (int)i;
            break;
        }
    }
    if (idx < 0) return 0;
    return (int)func_8009D5FC()->mWordRows[idx][slot];
}

u32 func_80164910() {
    CEventMgr* mgr = lbl_eu_80664240;
    if (mgr == nullptr) return 0;
    u32 result = 0;
    if (mgr->field_0x1BC != 0 ||
        (mgr->field_0x1D4 >= 0 && (lbl_eu_80663E28 & 0x01000000) == 0)) {
        result = 1;
    }
    return result;
}

// Shuffles the 0x26-byte lbl_eu_80530430 table (swap with a random slot for
// every odd index) and marks the event manager busy (+0x6C bit1). Returns 1
// on success; 0 when the manager is missing, the sequence counter is
// running, or the busy flag is already set.
int func_80164954() {
    CEventMgr* mgr = lbl_eu_80664240;
    if (mgr == 0) return 0;
    if (mgr->field_0x1D4 >= 0) return 0;
    u32 flag;
    if (mgr != 0) {
        flag = mgr->field_0x6C & 1;
    } else {
        flag = 0;
    }
    if (flag != 0) return 0;
    mgr->field_0x1D4 = 0;
    u8* p = lbl_eu_80530430;
    for (u32 i = 0; i < 0x26; i++, p++) {
        if (i & 1) {
            u8 v = *p;
            u32 r = (u32)ml::math::mtRand(0x26);
            if ((r & 1) == 0) {
                r += 1;
                if (r >= 0x26) r = 1;
            }
            *p = lbl_eu_80530430[r];
            lbl_eu_80530430[r] = v;
        }
    }
    lbl_eu_80664240->field_0x6C |= 2;
    return 1;
}

// Event-load kick (returns 0 always): validates the manager/sequence state,
// refreshes the CInfoCf settings, checks the file exists, then stores the
// gate object returned by func_8016AED4 and marks the manager busy. When the
// optional arg2 is set the CRI controller is consulted first. Finally the
// path's file extension is trimmed off and the resulting name is matched
// against the 32-entry event-name table, forwarding the index to
// func_8016462C on a hit.
int func_80164A50(const char* path, int arg1, int arg2) {
    CEventMgr* mgr = lbl_eu_80664240;
    if (mgr == 0) return 0;
    if (arg1 == 0 && mgr->field_0x1D4 >= 0) return 0;
    func_801667AC(reinterpret_cast<cf::CInfoCf*>(mgr->field_0x70));
    CEventMgr* m2 = lbl_eu_80664240;
    if ((m2->field_0x74 & 1) == 0) {
        func_80166784(reinterpret_cast<cf::CInfoCf*>(m2->field_0x70));
        return 0;
    }
    if (getFileSize__11CDeviceFileFPCc(path, 1) < 0) return 0;
    if ((lbl_eu_80663E28 & 0x01000000) == 0) {
        func_80043BC4();
    }
    lbl_eu_80664240->field_0x1BC = 0;
    lbl_eu_80664240->field_0xB0 = func_8016AED4();
    if (lbl_eu_80664240->field_0xB0 == 0) return 0;
    if (arg2 != 0) {
        func_8016C2C8();
    }
    lbl_eu_80664240->field_0x6C |= 1;
    const char* file = ml::CPathUtil::getFilePtrFromPath(path);
    // The name-match workspace is declared before the path workspace: MWCC
    // assigns automatic storage in declaration order from the top of the
    // frame, and the retail places the path buffer below the loop workspace.
    CTaskREventNameBuf sb;
    CTaskREventNameBuf pathBuf;
    pathBuf.len = strlen(file);
    strcpy(pathBuf.buf, file);
    // Trim the (up to 4-character) file extension off the name. The temp
    // keeps the trimmed length in a register for the terminator store (the
    // volatile member would otherwise force a reload).
    int trim = pathBuf.len < 4 ? pathBuf.len : 4;
    if (trim > 0) {
        int nl = pathBuf.len - trim;
        pathBuf.len = nl;
        pathBuf.buf[nl] = '\0';
    }
    const char* const* tbl = lbl_eu_80530710;
    const char* base = lbl_eu_80503008;
    for (u32 i = 0; i < 0x20; i++) {
        sb.len = strlen(base);
        strcpy(sb.buf, base);
        const char* suffix = tbl[i];
        int sufLen = strlen(suffix);
        strcat(sb.buf, suffix);
        sb.len += sufLen;
        if (strcmp(pathBuf.buf, sb.buf) == 0) {
            func_8016462C((int)i);
            break;
        }
    }
    return 0;
}

int func_80164C28() {
    CEventMgr* mgr = lbl_eu_80664240;
    if (mgr) {
        return mgr->field_0x6C & 1;
    }
    return 0;
}

// Returns 1 while an event sequence is fully idle: manager present, game
// manager running, no +0x74/+0x6C active flags, no +0xB0 gate, sequence
// counter clean, and no event word pending. Each guard is materialized as a
// 0/1 word (cntlzw/srwi) and AND-accumulated (retail shape).
int func_80164C48() {
    CEventMgr* mgr = lbl_eu_80664240;
    if (mgr == 0) return 0;
    if (cf::CfGameManager::func_8007E1B4() == 0) return 0;
    int result = lbl_eu_80664240->field_0x74 & 1;
    if (result) {
        u32 flag;
        if (lbl_eu_80664240 != 0) {
            flag = lbl_eu_80664240->field_0x6C & 1;
        } else {
            flag = 0;
        }
        result = !flag;
    }
    if (result) {
        result = !lbl_eu_80664240->field_0xB0;
    }
    if (result) {
        result = !func_8016847C();
    }
    if (result) {
        result = !lbl_eu_80664240->field_0x1D0;
    }
    return result;
}

// Tears down the active event sequence: clears the +0xB0 gate object, the
// +0x1D0 word, the +0x6C bit0 flag, notifies the game manager, and clears
// the global event bit when the +0x1BC byte flag is set.
void func_80164CFC() {
    CEventMgr* mgr = lbl_eu_80664240;
    if (mgr == 0) return;
    u32 v;
    if (mgr != 0) {
        v = mgr->field_0x6C & 1;
    } else {
        v = 0;
    }
    if (v == 0) return;
    CEventMgrB0* p = mgr->field_0xB0;
    if (p != 0) {
        p->field_0x39 = 1;
        lbl_eu_80664240->field_0xB0 = 0;
        lbl_eu_80664240->field_0x1D0 = 0;
    }
    lbl_eu_80664240->field_0x6C &= ~1;
    if (cf::CfGameManager::func_80083298() != 0) {
        if (&cf::CfGameManager::func_80083298()->field_0xF0 != 0) {
            func_8047BDA0__17UnkClass_8047BB54Fv(
                &cf::CfGameManager::func_80083298()->field_0xF0);
        }
    }
    if (lbl_eu_80664240->field_0x1BC != 0) {
        lbl_eu_80663E24 &= ~0x02000000;
    }
}

// Event-update kick: when the manager is present and its +0xB0 gate is set,
// poke the sequence processor, run the bit7-gated cleanup, then clear the
// +0x1D0 word and the +0x6C bit4 flag (re-reading the global after the calls
// because they may have replaced the manager).
void func_80164DB8() {
    CEventMgr* mgr = lbl_eu_80664240;
    if (mgr == 0) return;
    if (mgr->field_0xB0 == 0) return;
    func_80168484(1);
    if ((lbl_eu_80663E28 & 0x01000000) == 0) {
        func_80043BC4();
    }
    lbl_eu_80664240->field_0x1D0 = 0;
    lbl_eu_80664240->field_0x6C &= ~0x10;
}

// Builds the 0x1400-byte string table: 0xAE entries of 0x14 bytes, each
// "lbl_eu_80503008" + suffix from lbl_eu_80530458. The entry index lives in
// mIdx (reloaded per use - the calls may clobber memory).
void cf::CTaskREvent::Init() {
    this->mBuf = (u8*)mtl::MemManager::allocate_head(mtl::MemManager::getHandleMEM2(), 0x1400, 4);
    for (u32 i = 0; i < 0xAE; i++) {
        strcpy((char*)this->mBuf + this->mIdx * 0x14, lbl_eu_80503008);
        strcat((char*)this->mBuf + this->mIdx * 0x14, lbl_eu_80530458[i]);
        this->mIdx++;
    }
}

// Loads an SFD movie through the CRI player when the manager is idle
// (mCri == (CLibCri*)-1). `handle` selects the buffer source: a fresh MEM2
// handle vs the scene alloc handle (func_80495FF0(lbl_eu_80663E14)). The
// finished player is stored back through the global, re-read after the call
// because it may have been replaced.
void func_80164ED0(const char* path, int flag, u8* handle) {
    CEventMgr* mgr = lbl_eu_80664240;
    if (mgr != 0 && (u32)mgr->mCri == 0xFFFFFFFF) {
        u32 buffer;
        if (handle != 0) {
            buffer = mtl::MemManager::getHandleMEM2();
        } else {
            buffer = func_80495FF0(lbl_eu_80663E14);
        }
        CLibCri* cri = func_80459AA8__7CLibCriFv(
            path, mtl::MemManager::getHandleMEM2(), buffer, flag, 0);
        lbl_eu_80664240->mCri = cri;
    }
}

void func_80164F6C() {
    CEventMgr* mgr = lbl_eu_80664240;
    if (mgr == nullptr) return;
    CLibCri* cri = mgr->mCri;
    if ((u32)cri == 0xFFFFFFFF) return;
    cri->func_80459AAC();
    // Store through the global again: the call above may have changed it,
    // so retail re-reads lbl_eu_80664240 instead of reusing the local.
    lbl_eu_80664240->mCri = (CLibCri*)-1;
}

int func_80164FB4() {
    CEventMgr* mgr = lbl_eu_80664240;
    if (mgr == 0) {
        return 0;
    }
    CLibCri* cri = mgr->mCri;
    if ((u32)cri == 0xFFFFFFFF) {
        return 0;
    }
    return func_80459AC4__7CLibCriFv(cri);
}

int func_80164FE8(void) {
    int* ptr = (int*)lbl_eu_80664240;
    if (!ptr) return 0;
    int val = *(int*)((char*)ptr + 0x1e0);
    return (unsigned)((-1 - val) | (val + 1)) >> 31;
}

void func_80165014() {
    CEventMgr* mgr = lbl_eu_80664240;
    if (!mgr) return;
    CLibCri* cri = mgr->mCri;
    if ((u32)cri == 0xFFFFFFFF) return;
    cri->func_80459AC0();
}

extern "C" void func_8016C2E4();
extern "C" void func_80165038() { func_8016C2E4(); }

// Tears down the active event sequence. Runs only while the manager is
// present and its +0x6C bit0 flag is raised (the `mgr &&` test materializes
// the 0/1 word via the CR1 compare reused from the outer guard - func_80164410
// precedent): clears the +0xB0 gate object, the +0x6C bit0 flag and the
// +0x1D0 word, notifies the game manager, then clears the global event bit
// when the +0x1BC byte flag is set. Afterwards cancels the two file handles,
// runs the bit7-gated cleanup, releases the CRI movie player and frees the
// string-table buffer.
void cf::CTaskREvent::Term() {
    CEventMgr* mgr = lbl_eu_80664240;
    if (mgr != 0) {
        u32 flag;
        if (mgr != 0) {
            flag = mgr->field_0x6C & 1;
        } else {
            flag = 0;
        }
        if (flag != 0) {
            CEventMgrB0* gate = mgr->field_0xB0;
            if (gate != 0) {
                gate->field_0x39 = 1;
                lbl_eu_80664240->field_0xB0 = 0;
                lbl_eu_80664240->field_0x1D0 = 0;
            }
            lbl_eu_80664240->field_0x6C &= ~1;
            if (cf::CfGameManager::func_80083298() != 0) {
                if (&cf::CfGameManager::func_80083298()->field_0xF0 != 0) {
                    func_8047BDA0__17UnkClass_8047BB54Fv(
                        &cf::CfGameManager::func_80083298()->field_0xF0);
                }
            }
            if (lbl_eu_80664240->field_0x1BC != 0) {
                lbl_eu_80663E24 &= ~0x02000000;
            }
        }
    }
    if (this->field_0x1C0 != 0) {
        CDeviceFile::cancel(this->field_0x1C0);
        this->field_0x1C0 = 0;
    }
    if (this->field_0x1C4 != 0) {
        CDeviceFile::cancel(this->field_0x1C4);
        this->field_0x1C4 = 0;
    }
    if ((lbl_eu_80663E28 & 0x01000000) == 0) {
        func_80043BC4();
    }
    // Re-read the global (not the local) after the manager section: the calls
    // above may have replaced the manager object.
    if (lbl_eu_80664240 != 0 && (u32)lbl_eu_80664240->mCri != 0xFFFFFFFF) {
        lbl_eu_80664240->mCri->func_80459AAC();
        lbl_eu_80664240->mCri = (CLibCri*)-1;
    }
    if (this->mBuf != 0) {
        mtl::MemManager::deallocate(this->mBuf);
        this->mBuf = 0;
    }
}

void cf::CTaskREvent::Move() {}

void cf::CTaskREvent::Draw() {
    func_80165DF4(this, 0);
    func_80166050(this, 0);
}

// Event busy/dimming driver: recomputes the busy state from the global event
// bitfields (and the task game state), syncs the task's +0x6C bit5 to it,
// then runs the dimming/auto-sleep countdowns and refreshes the camera
// object's position when the game manager is idle.
__declspec(noinline) void func_80165DF4(cf::CTaskREvent* self, int arg) {
    int busy = (lbl_eu_80663E24 & 0x02440000) != 0;
    if ((lbl_eu_80663E28 & 0x1000) == 0) busy = 0;
    if ((lbl_eu_80663E28 & 0x01000000) != 0) busy = func_8004368C__9CTaskGameFv();
    if (busy != 0) {
        if ((lbl_eu_80663E24 & 0x00040000) != 0 &&
            Class_80296898::getInstance()->mConfigData[0x20] == 0) {
            busy = 0;
        }
        if (!(lbl_eu_80663E24 & 0x02040000) &&
            (lbl_eu_80663E24 & 0x00400000) &&
            (lbl_eu_80663E24 & 0x10000000)) {
            busy = 0;
        }
        if ((lbl_eu_80663E28 & 0x20000000) != 0 || func_8012CD24() != 0) {
            busy = 0;
        }
    }
    if (((busy ^ ((self->field_0x6C >> 5) & 1)) != 0) || arg != 0) {
        if (busy != 0) {
            self->field_0x6C |= 0x20;
        } else {
            self->field_0x6C &= ~0x20;
        }
        if (busy != 0) {
            if ((self->field_0x6C & 0xC0) == 0) {
                VIEnableDimming(0);
                self->field_0x6C &= ~0x800;
                if (func_8004368C__9CTaskGameFv() != 0) {
                    WPADSetAutoSleepTime(5);
                } else {
                    WPADSetAutoSleepTime(0);
                }
            }
            if (arg == 0) {
                lbl_eu_80664244 = 2;
                lbl_eu_80664248 = CDeviceVI::getTargetFramerate() * 300;
            }
        }
    }
    if ((self->field_0x6C & 0x20) == 0 || arg != 0) {
        if (lbl_eu_80664244 > 0) {
            lbl_eu_80664244 -= 1;
            if (lbl_eu_80664244 == 0) {
                if (func_8004368C__9CTaskGameFv() == 0 &&
                    (self->field_0x6C & 0x800) == 0) {
                    VIEnableDimming(1);
                    VISetTimeToDimming(0);
                    VIResetDimmingCount();
                    self->field_0x6C |= 0x800;
                }
            }
        }
        if (lbl_eu_80664248 > 0) {
            lbl_eu_80664248 -= 1;
            if (lbl_eu_80664248 == 0) {
                WPADSetAutoSleepTime(5);
            }
        }
    }
    UnkClass_800821F8* cam = func_800821F8__Q22cf13CfGameManagerFv();
    if (cam != 0) {
        cam = func_800821F8__Q22cf13CfGameManagerFv();
        if (cam->field_0xC != 0) {
            u32 mgrFlag;
            if (lbl_eu_80664240 != 0) {
                mgrFlag = lbl_eu_80664240->field_0x6C & 1;
            } else {
                mgrFlag = 0;
            }
            if (mgrFlag == 0) {
                CfEvtCamPlayerObj* obj =
                    func_800821F8__Q22cf13CfGameManagerFv()->field_0xC;
                obj->field_0x1EC = lbl_eu_80667644;
                obj->field_0x1F0 = lbl_eu_80667648;
                func_8049EB60();
            }
        }
    }
}

// Event-busy kick: while the game-manager state flag disagrees with the
// task's +0x6C bit7, push bit7 to match, and when bit6 (auto-sleep) is clear
// reset the frame target and bump the task manager (func_80165DF4). The
// manager-idle branch only bumps when bit6 is set. The two bit6-guarded
// blocks share the flag re-test shape (see func_80166150).
__declspec(noinline) void func_80166050(cf::CTaskREvent* self, int arg) {
    bool flag = cf::CfGameManager::func_800829B8();
    u32 flags = self->field_0x6C;
    // Retail tests with xor. (flag ^ bit7); branch when different.
    if ((flag ^ ((flags >> 7) & 1)) != 0 || arg != 0) {
        if (flag != 0) {
            self->field_0x6C |= 0x80;
        } else {
            self->field_0x6C &= ~0x80;
        }
        // if/else-if with the shared bit6 condition reproduces the retail
        // cross-jump: the then-branch's bit6 failure branches into the
        // else-if's entry, whose flag re-test bails that path.
        if (flag != 0 && (self->field_0x6C & 0x40) == 0) {
            lbl_eu_80664244 = 2;
            lbl_eu_80664248 = CDeviceVI::getTargetFramerate() * 300;
            func_80165DF4(self, 1);
        } else if (flag == 0 && (self->field_0x6C & 0x40) == 0) {
            lbl_eu_80664244 = 2;
            lbl_eu_80664248 = CDeviceVI::getTargetFramerate() * 300;
            func_80165DF4(self, 1);
        }
    } else {
        if (flag != 0 || (flags & 0x40) != 0) {
            func_80165DF4(self, 1);
        }
    }
}

// Sets the +0x6C bit6 auto-sleep flag to match `arg` and reconfigures the
// dimming/frame-target state. When bit6 already equals `arg` nothing is done.
// While `arg` is set, bit8 (0x100) gates the sleep/dimming reset; the two
// bit8-guarded tails share the arg re-test shape (see func_80166050).
void func_80166150(CEventMgr* self, u32 arg) {
    // Retail compares with xor. (arg ^ bit6); the whole body is skipped when
    // they are equal (retail `beq` to the epilogue).
    if ((arg ^ ((self->field_0x6C >> 6) & 1)) != 0) {
        if (arg != 0) {
            u32 v = self->field_0x6C;
            self->field_0x6C = v | 0x40;
        } else {
            u32 v = self->field_0x6C;
            self->field_0x6C = v & ~0x40;
        }
        if (arg == 0) {
            lbl_eu_80664244 = 2;
            lbl_eu_80664248 = CDeviceVI::getTargetFramerate() * 300;
        }
        // if/else-if with the shared bit7 (0x80) condition reproduces the
        // retail cross-jump (then-branch bit7 failure branches into the
        // else-if entry).
        if (arg != 0 && (self->field_0x6C & 0x80) == 0) {
            WPADSetAutoSleepTime(5);
            if ((self->field_0x6C & 0x800) == 0) {
                VIEnableDimming(1);
                VISetTimeToDimming(0);
                self->field_0x6C |= 0x800;
            }
            lbl_eu_80664244 = 2;
            lbl_eu_80664248 = CDeviceVI::getTargetFramerate() * 300;
        } else if (arg == 0 && (self->field_0x6C & 0x80) == 0) {
            // The manager pointer is passed through unchanged (retail passes
            // r31 to a helper typed on the task object).
            func_80165DF4(reinterpret_cast<cf::CTaskREvent*>(self), 1);
        }
    }
}

// Render-before hook: if the event manager's CRI player is idle, copy the
// current view's rect into our player. Skips when this task or the manager
// holds a live CRI controller.
void cf::CTaskREvent::cbRenderBefore() {
    if ((u32)this->mCri == 0xFFFFFFFF) return;
    CView* view = getCurrentView__5CViewFv();
    CEventMgr* mgr = lbl_eu_80664240;
    u32 result;
    // Nested if/else materializes the CRI-player result into r3 with a
    // per-branch `li r3,0` (retail shape); a flat `&&`/else chain merges the
    // zero-blocks and shrinks the function.
    if (mgr != 0) {
        if ((u32)mgr->mCri != 0xFFFFFFFF) {
            result = func_80459AC4__7CLibCriFv(mgr->mCri);
        } else {
            result = 0;
        }
    } else {
        result = 0;
    }
    if (result != 0) return;
    ml::CRect rect;
    func_8043EA88__5CViewFRQ22ml5CRectP5CView(rect, view);
    func_80459ACC__7CLibCriFv(this->mCri, rect);
}

// Frame-timing callback (vtable entry of the CDeviceVICb subobject at 0x54,
// reached through the func_801666C4 thunk with the adjusted CTaskREvent
// pointer). Throttles the task-manager update count toward the PAL/NTSC
// frame target while the event manager is idle.
void func_801662E8(cf::CTaskREvent* self) {
    if (CGame::getInstance() == 0) return;
    CGame::setTaskManagerUpdateCount(1);
    if (func_80043D68() == 0) return;
    if (self->field_0xB0 == 0) return;
    if (cf::CfGameManager::func_800829B8() != 0) return;
    if (lbl_eu_80663E28 & 0x01000000) return;
    int v = func_80043B54();
    if (CDeviceVI::isTvFormatPal()) v += 2;
    if (func_8016A3A8() < v) {
        if (func_eu_8016DA48(self->field_0xB0) != 0) {
            int d = v - func_8016A3A8();
            if (d > 0x3c) d = 0x3c;
            CGame::setTaskManagerUpdateCount((u32)d);
            self->field_0x6C |= 0x100;
            return;
        }
    }
    self->field_0x6C &= ~0x100;
}

// Async file-event handler (IWorkEvent callback via the OnFileEvent thunk):
// only processes events for the handle stored in field_0x1C4. On a load
// event walks the loaded data buffer's linked records for the id-3 record,
// then scans its sub-entries for the event-id pair; a hit raises the global
// event bit, a miss clears it and notifies the game manager. Non-load events
// just build the base event-name string.
int func_801663A8(cf::CTaskREvent* self, CTaskREventFileEvent* ev) {
    if (ev->field_04 != self->field_0x1C4) return 0;
    self->field_0x1C4 = 0;
    if (ev->field_00 == 1) {
        func_8016C450(ev->field_0C, ev->field_10, 0x8000);
        CTaskREventDataBuf* buf =
            reinterpret_cast<CTaskREventDataBuf*>(ev->field_10);
        CTaskREventDataEntry* entry = reinterpret_cast<CTaskREventDataEntry*>(
            reinterpret_cast<u8*>(buf) + buf->field_0x8);
        CTaskREventDataEntry* found = 0;
        for (;;) {
            if (entry->field_0x0 == -1) break;
            if (entry->field_0x0 == 3) {
                found = entry;
                break;
            }
            entry = reinterpret_cast<CTaskREventDataEntry*>(
                reinterpret_cast<u8*>(entry) + entry->field_0x4);
        }
        if (found == 0) {
            lbl_eu_80663E24 |= 0x02000000;
            self->field_0x1D0 = 4;
            return 1;
        }
        for (u32 i = 0; i < buf->field_0x20; i++) {
            if (found->field_0x28 == 1) {
                u32 out0, out1, out2, out3;
                func_800AA318(found->field_0x20, &out0, &out1, &out2, &out3);
                if (out0 == 1) {
                    if (out1 == lbl_eu_80663E42 && out2 == lbl_eu_80663E44) {
                        lbl_eu_80663E24 |= 0x02000000;
                        self->field_0x1D0 = 4;
                    } else {
                        lbl_eu_80663E24 &= ~0x02000000;
                        f32 f = lbl_eu_80667628;
                        f32 v[3] = { f, f, f };
                        func_80083D50__Q22cf13CfGameManagerFv(
                            (u16)out1, (u16)out2, v, &lbl_eu_80503008[0xe]);
                        lbl_eu_80663E28 &= ~0x00100000;
                        self->field_0x1D0 = 3;
                    }
                    return 1;
                }
            }
            found = reinterpret_cast<CTaskREventDataEntry*>(
                reinterpret_cast<u8*>(found) + found->field_0x4);
        }
        self->field_0x1D0 = 4;
        return 1;
    }
    const char* s = lbl_eu_80503008 + 0xd;
    self->field_0x1B4 = strlen(s);
    strcpy(self->mNameBuf, s);
    self->field_0x1D0 = 0;
    return 1;
}

// Allocates a CTaskREvent (size 0x1F8) from the work-thread heap and
// registers it into pParent. Returns the task (or 0 if allocation failed).
cf::CTaskREvent* func_801665A4(CProcess* pParent, CScnNw4r* pScene, CView* pView) {
    u8* mem = static_cast<u8*>(
        mtl::MemManager::allocate(0x1f8, CWorkThreadSystem::getWorkMem()));
    cf::CTaskREvent* task = reinterpret_cast<cf::CTaskREvent*>(mem);
    if (mem != 0) {
        task = __ct__cf_CTaskREvent(task, pScene, pView);
    }
    task->Regist(pParent, false);
    return task;
}

void viAfterDrawDone__11CDeviceVICbFv() {}

void viBeforeDrawDone__11CDeviceVICbFv() {}

void func_801666C4(void* self) { ((void(*)(void*))func_801662E8)((char*)self - 0x54); }

void func_801666CC(void* self) { ((void(*)(void*))__dt__Q22cf11CTaskREventFv)((char*)self - 0x54); }

void func_801666D4(void* self) { ((void(*)(void*))func_801644D8)((char*)self - 0x58); }

void func_801666DC(void* self) { ((void(*)(void*))__dt__Q22cf11CTaskREventFv)((char*)self - 0x58); }

void func_801666E4(void* self) { ((void(*)(void*))cbRenderBefore__Q22cf11CTaskREventFv)((char*)self - 0x5c); }

extern "C" void func_801666EC(u8* self) { ((void(*)(void*))__dt__Q22cf11CTaskREventFv)((char*)self - 0x5c); }

// IWorkEvent dispatch thunk: the IWorkEvent subobject sits at +0x60 of the
// cf::CTaskREvent; subtract the offset and tail-call func_801663A8.
void ::CTaskREvent::OnFileEvent(CEventFile* ev) {
    func_801663A8(
        reinterpret_cast<cf::CTaskREvent*>(reinterpret_cast<u8*>(this) - 0x60),
        reinterpret_cast<CTaskREventFileEvent*>(ev));
}

extern "C" void func_801666FC(u8* self) { ((void(*)(void*))__dt__Q22cf11CTaskREventFv)((char*)self - 0x60); }

// CTTask<cf::CTaskREvent> specializations provided by header
