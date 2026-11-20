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

// The compiler emits CScriptCode's RTTI, base list and vtable.
CScriptCode* CScriptCode::sInstance;

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
    CScriptCode::sInstance = NULL;
}

// us-8043cb74: line parser (retail symbol is Fv-mangled but takes arguments).
// Copies whitespace-delimited lines from pData into a local buffer, calls
// dispatchCommand on each, then skips to the next line.
void parseLines__11CScriptCodeFv(CScriptCode* self, u8* pData, u32 dataSize) {
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
        dispatchCommand__11CScriptCodeFv((CCmdDispatcher*)self, (const char*)lineBuf, 1);

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
s16 splitSegments__11CScriptCodeFv(CScriptCode* self, u8* pOut, u8* pIn) {
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
void registerCmd__11CScriptCodeFv(const char* pKey, CCmdDispatcher* pValue) {
    CScriptCode* inst = CScriptCode::sInstance;
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
    inst = CScriptCode::sInstance;
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
        new (ths) CWorkThread("CScriptCode", pParent, 1);

        ths->m_cnt0 = 0;
        ths->m_cnt1 = 0;

        new (&ths->m_token) CToken();
        ths->field_0x1738 = 0;
        CScriptCode::sInstance = ths;
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
int dispatchCommand__11CScriptCodeFv(CCmdDispatcher* self, const char* pData, int doSecondary) {
    u8 buf[0x800];
    s16 segCount = splitSegments__11CScriptCodeFv(CScriptCode::sInstance, buf, (u8*)pData);

    for (s16 s = 0; s < segCount; s++) {
        const u8* seg = &buf[s * 0x100];

        // Search the secondary slot array (m_slot1)
        for (s16 i = 0; i < CScriptCode::sInstance->m_cnt1; i++) {
            CScriptCodeEntry* entry = CScriptCode::sInstance->m_slot1[i];
            if (CScriptCode::sInstance->m_token.matchKeyWords(entry->key, (const char*)seg)) {
                int r = 0;
                if (self != NULL) {
                    r = self->cmdHandler(CScriptCode::sInstance->m_token, entry->key);
                }
                if (entry->handler != NULL) {
                    return entry->handler->cmdHandler(CScriptCode::sInstance->m_token, entry->key);
                }
                return r;
            }
        }

        if (doSecondary) {
            // Search the primary slot array (m_slot0)
            for (s16 i = 0; i < CScriptCode::sInstance->m_cnt0; i++) {
                CScriptCodeEntry* entry = CScriptCode::sInstance->m_slot0[i];
                if (CScriptCode::sInstance->m_token.matchKeyWords(entry->key, (const char*)seg)) {
                    int r = 0;
                    if (self != NULL) {
                        r = self->cmdHandler(CScriptCode::sInstance->m_token, entry->key);
                    }
                    if (entry->handler != NULL) {
                        r = entry->handler->cmdHandler(CScriptCode::sInstance->m_token, entry->key);
                    }
                    CScriptCode::sInstance->field_0x1738 = (u32)entry;
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
void unregisterCmd__11CScriptCodeFv(const char* pKey, CCmdDispatcher* pTarget) {
    const char* key = pKey;

    // Primary slot array: find first entry whose key matches, free + compact.
    // The singleton is re-read after the delete/compaction (retail keeps inst
    // and cnt in volatile regs across the search, not callee-saved).
    s16 cnt = CScriptCode::sInstance->m_cnt0;
    for (s16 i = 0; i < cnt; i++) {
        CScriptCodeEntry* entry = CScriptCode::sInstance->m_slot0[i];
        if (entry->key == key) {
            if (entry != NULL) {
                ::operator delete(entry);
                CScriptCode::sInstance->m_slot0[i] = NULL;
            }
            while (i < CScriptCode::sInstance->m_cnt0 - 1) {
                CScriptCode::sInstance->m_slot0[i]
                    = CScriptCode::sInstance->m_slot0[i + 1];
                i++;
            }
            CScriptCode::sInstance->m_cnt0 = CScriptCode::sInstance->m_cnt0 - 1;
            return;
        }
    }

    // Secondary slot array
    cnt = CScriptCode::sInstance->m_cnt1;
    for (s16 i = 0; i < cnt; i++) {
        CScriptCodeEntry* entry = CScriptCode::sInstance->m_slot1[i];
        if (entry->key == key) {
            if (entry != NULL) {
                ::operator delete(entry);
                CScriptCode::sInstance->m_slot1[i] = NULL;
            }
            while (i < CScriptCode::sInstance->m_cnt1 - 1) {
                CScriptCode::sInstance->m_slot1[i]
                    = CScriptCode::sInstance->m_slot1[i + 1];
                i++;
            }
            CScriptCode::sInstance->m_cnt1 = CScriptCode::sInstance->m_cnt1 - 1;
            return;
        }
    }
}

// us-...: CScriptCode::getInstance()
CScriptCode* CScriptCode::getInstance() {
    return CScriptCode::sInstance;
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
