// High-level C++ decompilation for monolib/src/core/CScriptCode
//
// The retail vtable/RTTI for CScriptCode live inside a data blob owned by this
// TU and are defined explicitly below; the class is therefore novtable so the
// compiler does not emit duplicates. The func_* script-parser routines have
// retail symbols mangled as zero-argument members yet take register
// arguments, which cannot be expressed as C++ member declarations, so they
// keep their exact retail symbol names as free functions.

#include <harness_catalog.h>
#include <string.h>

#include "monolib/work/CWorkThread.hpp"
#include "monolib/core/CToken.hpp"
#include "monolib/work/CWorkSystem.hpp"
#include "monolib/util/MemManager.hpp"
#include "monolib/work/CWorkThreadSystem.hpp"
#include "monolib/work/CWorkUtil.hpp"
#include "monolib/util/reslist.hpp"
#include "monolib/core/CProcRoot.hpp"
#include "monolib/core/CViewRoot.hpp"
#include "libs/monolib/src/core/CScriptCode.hpp"

// --- Blob monolibdata1.s dissolve: this TU owns .data 0x8056B418-0x8056B4D0,
// .rodata 0x80522540-0x80522558, .sdata 0x80663558-0x80663560, .sbss
// 0x806655B0-0x806655B8. ---

// Retail sbss singleton slot lbl_eu_806655B0 (8 bytes; word 0 in use).
CScriptCode* lbl_eu_806655B0[2];

// RTTI name strings (.rodata) and RTTI locator (.sdata, 8 bytes):
// { name, base-list }.
const char lbl_eu_80522540[] = "CScriptCode";
const char lbl_eu_8052254C[] = "CScriptCode";

// RTTI locators for the base interfaces (defined by kyoshin/CGame.cpp,
// .sdata 0x806618A0/0x806618A8).
extern u32 __RTTI__10IWorkEvent[];
extern u32 __RTTI__11CWorkThread[];

// RTTI base list (.data, 0x18): [IWorkEvent, 0, CWorkThread, 0, 0, 0].
u32 lbl_eu_8056B4B8[6] = {
    (u32)__RTTI__10IWorkEvent, 0, (u32)__RTTI__11CWorkThread, 0, 0, 0,
};

// RTTI locator (.sdata, 8 bytes): { name, base-list }.
u32 lbl_eu_80663558[2] = { (u32)lbl_eu_80522540, (u32)lbl_eu_8056B4B8 };

// CScriptCode vtable (.data, 0xA0): [rtti locator, 0, dtor, IWorkEvent 1-31,
// wkUpdate/wkRender/wkRenderAfter/wkStandbyLogin (CWorkThread slots),
// wkStandbyLogout (CScriptCode), wkStandbyExceptionRetry].
u32 lbl_eu_8056B418[0xA0 / 4] = {
    (u32)&lbl_eu_80663558, 0, (u32)&CScriptCode::~CScriptCode,
    (u32)&IWorkEvent::WorkEvent1, (u32)&IWorkEvent::OnFileEvent,
    (u32)&IWorkEvent::WorkEvent3, (u32)&IWorkEvent::WorkEvent4,
    (u32)&IWorkEvent::OnPauseTrigger,
    (u32)&IWorkEvent::WorkEvent6, (u32)&IWorkEvent::WorkEvent7,
    (u32)&IWorkEvent::WorkEvent8, (u32)&IWorkEvent::WorkEvent9,
    (u32)&IWorkEvent::WorkEvent10, (u32)&IWorkEvent::WorkEvent11,
    (u32)&IWorkEvent::WorkEvent12, (u32)&IWorkEvent::WorkEvent13,
    (u32)&IWorkEvent::WorkEvent14, (u32)&IWorkEvent::WorkEvent15,
    (u32)&IWorkEvent::WorkEvent16, (u32)&IWorkEvent::WorkEvent17,
    (u32)&IWorkEvent::WorkEvent18, (u32)&IWorkEvent::WorkEvent19,
    (u32)&IWorkEvent::WorkEvent20, (u32)&IWorkEvent::WorkEvent21,
    (u32)&IWorkEvent::WorkEvent22, (u32)&IWorkEvent::WorkEvent23,
    (u32)&IWorkEvent::WorkEvent24, (u32)&IWorkEvent::WorkEvent25,
    (u32)&IWorkEvent::WorkEvent26, (u32)&IWorkEvent::WorkEvent27,
    (u32)&IWorkEvent::WorkEvent28, (u32)&IWorkEvent::WorkEvent29,
    (u32)&IWorkEvent::WorkEvent30, (u32)&IWorkEvent::WorkEvent31,
    (u32)&CWorkThread::wkUpdate, (u32)&CWorkThread::wkRender,
    (u32)&CWorkThread::wkRenderAfter, (u32)&CWorkThread::wkStandbyLogin,
    (u32)&CScriptCode::wkStandbyLogout,
    (u32)&CWorkThread::wkStandbyExceptionRetry,
};

// Interface for command handler objects dispatched through vtable slot 3. All
// pure virtuals so no vtable is emitted.
class CCmdDispatcher {
public:
    virtual void d0() = 0;
    virtual void d1() = 0;
    virtual void d2() = 0;
    virtual int cmdHandler(CToken& token, const char* key) = 0;
};

// us-8043ca78: CScriptCode::~CScriptCode()
// Frees both slot arrays, clears the singleton, then lets the generated
// epilogue destroy the token member and the CWorkThread base (and delete the
// object itself when the implicit free flag is set).
CScriptCode::~CScriptCode() {
    *(u32*)this = (u32)lbl_eu_8056B418;

    for (int i = 0; i < m_cnt0; i++) {
        if (m_slot0[i] != NULL) {
            ::operator delete(m_slot0[i]);
            m_slot0[i] = NULL;
        }
    }

    for (int i = 0; i < m_cnt1; i++) {
        if (m_slot1[i] != NULL) {
            ::operator delete(m_slot1[i]);
            m_slot1[i] = NULL;
        }
    }

    m_cnt0 = 0;
    m_cnt1 = 0;
    lbl_eu_806655B0[0] = NULL;
}

// us-8043cb74: line parser (retail symbol is Fv-mangled but takes arguments).
// Copies whitespace-delimited lines from pData into a local buffer, calls
// func_8043A390 on each, then skips to the next line.
void func_8043A1DC__11CScriptCodeFv(CScriptCode* self, u8* pData, u32 dataSize) {
    int offset = 0;

    if (pData == NULL) return;

    while ((s8)pData[offset] != 0) {
        u8 lineBuf[256];
        u32 count = 0;

        // Copy bytes >= 0x20 or tabs into lineBuf, converting tabs to spaces
        while (pData[offset] >= 0x20 || pData[offset] == 0x09) {
            lineBuf[count] = pData[offset];
            offset++;
            count++;
            if (lineBuf[count - 1] == 0x09) {
                lineBuf[count - 1] = 0x20;
            }
            if (count >= 0xFF || offset >= dataSize) break;
        }

        if (offset >= dataSize) return;

        // Null-terminate the line and process it
        lineBuf[count] = 0;
        func_8043A390__11CScriptCodeFv((CCmdDispatcher*)self, (const char*)lineBuf, 1);

        // Skip forward to the end of the line
        while (1) {
            s8 ch = (s8)pData[offset];
            if (ch == '\n') {
                offset++;
                break;
            }
            if (offset >= dataSize || ch == 0) break;
            offset++;
        }
    }
}

// us-8043cc90: segment splitter (retail symbol is Fv-mangled but takes
// arguments). Splits pIn on '|' into 256-byte segments stored into pOut. Each
// segment is null-terminated. Returns the number of segments (incl. the final
// one) when the input ends with '\0', or 8 once 8 segments have been filled.
// Register-shape notes: seg/pos are s16 (re-sign-extended on use) and base
// accumulates the segment offset; addresses are formed base-first so MWCC
// emits add rBase, rPtr in retail operand order.
s16 func_8043A2F8__11CScriptCodeFv(CScriptCode* self, u8* pOut, u8* pIn) {
    s16 seg = 0;
    s16 pos = 0;
    int base = 0;
    char z = 0;

    for (;;) {
        char c = *pIn;
        if (c == '|') {
            seg++;
            (base + pOut)[pos] = z;
            pos = 0;
            pIn++;
            base += 0x100;
            if (seg < 8) {
                continue;
            }
            return 8;
        } else if (c == '\0') {
            ((seg << 8) + pOut)[pos] = 0;
            return seg + 1;
        } else {
            (base + pOut)[pos] = c;
            pIn++;
            pos++;
        }
    }
}

// us-8043d0a4: registration routine (retail symbol is Fv-mangled but takes
// arguments). Registers a (key, handler) pair in the global ScriptCode
// singleton's primary slot array. Does nothing if the key is already
// registered.
void func_8043A70C__11CScriptCodeFv(const char* pKey, CCmdDispatcher* pValue) {
    CScriptCode* inst = lbl_eu_806655B0[0];
    s16 cnt = inst->m_cnt0;

    // Search for an existing entry whose key matches `pKey`
    for (int i = 0; i < cnt; i++) {
        CScriptCodeEntry* entry = inst->m_slot0[i];
        if (entry->key == pKey) return;
    }

    // Allocate and fill a new 8-byte entry
    mtl::ALLOC_HANDLE mem = CWorkSystem::getMem();
    CScriptCodeEntry* entry =
        (CScriptCodeEntry*)mtl::MemManager::allocate(8, mem);
    memset(entry, 0, 8);
    entry->key = pKey;
    entry->handler = pValue;

    // Append to the global instance's primary array
    inst = lbl_eu_806655B0[0];
    s16 idx = inst->m_cnt0;
    inst->m_slot0[idx] = entry;
    inst->m_cnt0 = idx + 1;
}

// us-8043d1b8: CScriptCode::create()
CScriptCode* CScriptCode::create(CWorkThread* pParent) {
    mtl::ALLOC_HANDLE mem = CWorkThreadSystem::getWorkMem();
    CScriptCode* ths = (CScriptCode*)mtl::MemManager::allocate(0x1740, mem);

    if (ths != NULL) {
        // Construct base class and initialize the object
        new (ths) CWorkThread(lbl_eu_8052254C, pParent, 1);

        *(u32*)ths = (u32)lbl_eu_8056B418;
        ths->m_cnt0 = 0;
        ths->m_cnt1 = 0;

        new (&ths->m_token) CToken();
        ths->field_0x1738 = 0;
        lbl_eu_806655B0[0] = ths;
        ths->m_token.func_8043AA1C();

        ths->mType = CWorkThread::THREAD_CSCRIPTCODE;
    }

    CWorkUtil::entryWork(ths, pParent, false);

    return ths;
}

// us-8043cd28: script-command dispatcher (retail symbol is Fv-mangled but
// takes arguments). Splits the input on '|' into commands; for each command
// segment, searches the secondary slot array (then the primary, if
// doSecondary) for an entry whose key string matches the segment, and
// dispatches to the matched handler(s) through vtable slot 3 with the token.
// Returns 1 when no command matched.
int func_8043A390__11CScriptCodeFv(CCmdDispatcher* self, const char* pData, int doSecondary) {
    u8 buf[0x800];
    s16 segCount = func_8043A2F8__11CScriptCodeFv(lbl_eu_806655B0[0], buf, (u8*)pData);

    for (s16 s = 0; s < segCount; s++) {
        const u8* seg = &buf[s * 0x100];

        // Search the secondary slot array (m_slot1)
        for (s16 i = 0; i < lbl_eu_806655B0[0]->m_cnt1; i++) {
            CScriptCodeEntry* entry = lbl_eu_806655B0[0]->m_slot1[i];
            if (lbl_eu_806655B0[0]->m_token.func_8043ABD4(entry->key, (const char*)seg)) {
                int r = 0;
                if (self != NULL) {
                    r = self->cmdHandler(lbl_eu_806655B0[0]->m_token, entry->key);
                }
                if (entry->handler != NULL) {
                    return entry->handler->cmdHandler(lbl_eu_806655B0[0]->m_token, entry->key);
                }
                return r;
            }
        }

        if (doSecondary) {
            // Search the primary slot array (m_slot0)
            for (s16 i = 0; i < lbl_eu_806655B0[0]->m_cnt0; i++) {
                CScriptCodeEntry* entry = lbl_eu_806655B0[0]->m_slot0[i];
                if (lbl_eu_806655B0[0]->m_token.func_8043ABD4(entry->key, (const char*)seg)) {
                    int r = 0;
                    if (self != NULL) {
                        r = self->cmdHandler(lbl_eu_806655B0[0]->m_token, entry->key);
                    }
                    if (entry->handler != NULL) {
                        r = entry->handler->cmdHandler(lbl_eu_806655B0[0]->m_token, entry->key);
                    }
                    lbl_eu_806655B0[0]->field_0x1738 = (u32)entry;
                    return r;
                }
            }
        }
    }

    return 1;
}

// us-8043cf14: unregistration routine (retail symbol is Fv-mangled but takes
// arguments). Unregisters the first slot entry whose key matches `pKey`:
// frees the entry object, then compacts the array by shifting all later
// entries down one slot and decrements the count. Searches the primary slot
// array first, falling back to the secondary slot array.
void func_8043A57C__11CScriptCodeFv(const char* pKey, CCmdDispatcher* pTarget) {
    const char* key = pKey;

    // Primary slot array: find first entry whose key matches, free + compact.
    // The singleton is re-read after the delete/compaction (retail keeps inst
    // and cnt in volatile regs across the search, not callee-saved).
    s16 cnt = lbl_eu_806655B0[0]->m_cnt0;
    for (s16 i = 0; i < cnt; i++) {
        CScriptCodeEntry* entry = lbl_eu_806655B0[0]->m_slot0[i];
        if (entry->key == key) {
            if (entry != NULL) {
                ::operator delete(entry);
                lbl_eu_806655B0[0]->m_slot0[i] = NULL;
            }
            while (i < lbl_eu_806655B0[0]->m_cnt0 - 1) {
                lbl_eu_806655B0[0]->m_slot0[i]
                    = lbl_eu_806655B0[0]->m_slot0[i + 1];
                i++;
            }
            lbl_eu_806655B0[0]->m_cnt0 = lbl_eu_806655B0[0]->m_cnt0 - 1;
            return;
        }
    }

    // Secondary slot array
    cnt = lbl_eu_806655B0[0]->m_cnt1;
    for (s16 i = 0; i < cnt; i++) {
        CScriptCodeEntry* entry = lbl_eu_806655B0[0]->m_slot1[i];
        if (entry->key == key) {
            if (entry != NULL) {
                ::operator delete(entry);
                lbl_eu_806655B0[0]->m_slot1[i] = NULL;
            }
            while (i < lbl_eu_806655B0[0]->m_cnt1 - 1) {
                lbl_eu_806655B0[0]->m_slot1[i]
                    = lbl_eu_806655B0[0]->m_slot1[i + 1];
                i++;
            }
            lbl_eu_806655B0[0]->m_cnt1 = lbl_eu_806655B0[0]->m_cnt1 - 1;
            return;
        }
    }
}

// us-...: CScriptCode::getInstance()
CScriptCode* CScriptCode::getInstance() {
    return lbl_eu_806655B0[0];
}

// us-8043d158: CScriptCode::wkStandbyLogout()
// Logs out only while the child list is empty and neither the proc root nor
// the view root has been created yet; otherwise reports failure.
bool CScriptCode::wkStandbyLogout() {
    if (mChildren.empty()
        && CProcRoot::getInstance() == NULL
        && CViewRoot::getInstance() == NULL) {
        return CWorkThread::wkStandbyLogout();
    }
    return false;
}
