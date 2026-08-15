// High-level C decompilation for monolib/src/core/CScriptCode
//
// The retail symbols for these functions are all 'Fv'-mangled yet take
// register arguments, so they are written as extern "C" free functions (as
// the original scaffold and sibling monolib TUs like CSchedule do) to emit
// the exact retail symbols. The class layout is declared here because the
// build's only monolib include root is libs/monolib/include (the writable
// staged header under src/core is not on the include path).

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

// CScriptCode vtable (0x8056B418), name strings (0x80522540/0x8052254C) and
// global singleton pointer (0x806655B0) data labels.
// Retail sbss singleton slot lbl_eu_806655B0 (8 bytes; word 0 in use) - blob monolibdata1d dissolve
void* lbl_eu_806655B0[2];

extern "C" {
// Vtable/RTTI targets referenced by the CScriptCode data definitions below.
// (dtor + wkStandbyLogout are defined later in this TU; forward declared so
// the vtable initializer above them resolves the retail names.)
extern void __dt__11CScriptCodeFv(void* self, int freeFlag);
extern bool wkStandbyLogout__11CScriptCodeFv(void* self);
// IWorkEvent virtual handlers (weak defaults live in kyoshin/CGame.cpp).
extern int WorkEvent1__10IWorkEventFPvPCc(void*, const char*);
extern int OnFileEvent__10IWorkEventFP10CEventFile(void*);
extern int WorkEvent3__10IWorkEventFPv(void*);
extern int WorkEvent4__10IWorkEventFv();
extern void OnPauseTrigger__10IWorkEventFb(int);
extern int WorkEvent6__10IWorkEventFv();
extern int WorkEvent7__10IWorkEventFv();
extern int WorkEvent8__10IWorkEventFv();
extern int WorkEvent9__10IWorkEventFv();
extern int WorkEvent10__10IWorkEventFv();
extern int WorkEvent11__10IWorkEventFv();
extern int WorkEvent12__10IWorkEventFv();
extern int WorkEvent13__10IWorkEventFv();
extern int WorkEvent14__10IWorkEventFv();
extern int WorkEvent15__10IWorkEventFv();
extern int WorkEvent16__10IWorkEventFv();
extern int WorkEvent17__10IWorkEventFv();
extern int WorkEvent18__10IWorkEventFv();
extern int WorkEvent19__10IWorkEventFv();
extern int WorkEvent20__10IWorkEventFv();
extern int WorkEvent21__10IWorkEventFv();
extern int WorkEvent22__10IWorkEventFv();
extern int WorkEvent23__10IWorkEventFv();
extern int WorkEvent24__10IWorkEventFv();
extern int WorkEvent25__10IWorkEventFv();
extern int WorkEvent26__10IWorkEventFv();
extern int WorkEvent27__10IWorkEventFv();
extern int WorkEvent28__10IWorkEventFv();
extern int WorkEvent29__10IWorkEventFv();
extern int WorkEvent30__10IWorkEventFv();
extern void WorkEvent31__10IWorkEventFv();
// CWorkThread work-method vtable slots.
extern void wkUpdate__11CWorkThreadFv();
extern void wkRender__11CWorkThreadFv();
extern void wkRenderAfter__11CWorkThreadFv();
extern bool wkStandbyLogin__11CWorkThreadFv();
extern void wkStandbyExceptionRetry__11CWorkThreadFUl(unsigned int);
// RTTI locators (defined by kyoshin/CGame.cpp, .sdata 0x806618A0/0x806618A8).
extern void* __RTTI__10IWorkEvent;
extern void* __RTTI__11CWorkThread;
}

// --- Blob monolibdata1.s dissolve: this TU owns .data 0x8056B418-0x8056B4D0,
// .rodata 0x80522540-0x80522558, .sdata 0x80663558-0x80663560, .sbss
// 0x806655B0-0x806655B8. ---

// RTTI name strings (.rodata).
extern const char lbl_eu_80522540[];
extern const char lbl_eu_8052254C[];
// RTTI locator (.sdata, 8 bytes): { name, base-list }.
extern void* lbl_eu_80663558[2];

const char lbl_eu_80522540[] = "CScriptCode";
const char lbl_eu_8052254C[] = "CScriptCode";

// CScriptCode vtable (.data, 0xA0): [rtti, 0, dtor, IWorkEvent 1-31, wkUpdate/
// wkRender/wkRenderAfter/wkStandbyLogin (CWorkThread slots), wkStandbyLogout
// (CScriptCode), wkStandbyExceptionRetry].
u32 lbl_eu_8056B418[0xA0 / 4] = {
    (u32)&lbl_eu_80663558, 0, (u32)&__dt__11CScriptCodeFv,
    (u32)&WorkEvent1__10IWorkEventFPvPCc, (u32)&OnFileEvent__10IWorkEventFP10CEventFile,
    (u32)&WorkEvent3__10IWorkEventFPv, (u32)&WorkEvent4__10IWorkEventFv,
    (u32)&OnPauseTrigger__10IWorkEventFb,
    (u32)&WorkEvent6__10IWorkEventFv, (u32)&WorkEvent7__10IWorkEventFv,
    (u32)&WorkEvent8__10IWorkEventFv, (u32)&WorkEvent9__10IWorkEventFv,
    (u32)&WorkEvent10__10IWorkEventFv, (u32)&WorkEvent11__10IWorkEventFv,
    (u32)&WorkEvent12__10IWorkEventFv, (u32)&WorkEvent13__10IWorkEventFv,
    (u32)&WorkEvent14__10IWorkEventFv, (u32)&WorkEvent15__10IWorkEventFv,
    (u32)&WorkEvent16__10IWorkEventFv, (u32)&WorkEvent17__10IWorkEventFv,
    (u32)&WorkEvent18__10IWorkEventFv, (u32)&WorkEvent19__10IWorkEventFv,
    (u32)&WorkEvent20__10IWorkEventFv, (u32)&WorkEvent21__10IWorkEventFv,
    (u32)&WorkEvent22__10IWorkEventFv, (u32)&WorkEvent23__10IWorkEventFv,
    (u32)&WorkEvent24__10IWorkEventFv, (u32)&WorkEvent25__10IWorkEventFv,
    (u32)&WorkEvent26__10IWorkEventFv, (u32)&WorkEvent27__10IWorkEventFv,
    (u32)&WorkEvent28__10IWorkEventFv, (u32)&WorkEvent29__10IWorkEventFv,
    (u32)&WorkEvent30__10IWorkEventFv, (u32)&WorkEvent31__10IWorkEventFv,
    (u32)&wkUpdate__11CWorkThreadFv, (u32)&wkRender__11CWorkThreadFv,
    (u32)&wkRenderAfter__11CWorkThreadFv, (u32)&wkStandbyLogin__11CWorkThreadFv,
    (u32)&wkStandbyLogout__11CScriptCodeFv, (u32)&wkStandbyExceptionRetry__11CWorkThreadFUl,
};

// RTTI base list (.data, 0x18): [IWorkEvent, 0, CWorkThread, 0, 0, 0].
u32 lbl_eu_8056B4B8[6] = {
    (u32)&__RTTI__10IWorkEvent, 0, (u32)&__RTTI__11CWorkThread, 0, 0, 0,
};

void* lbl_eu_80663558[2] = { (void*)lbl_eu_80522540, (void*)lbl_eu_8056B4B8 };

// Layout of CScriptCode. The build's CWorkThread header size is unreliable, so
// the base is carried as an opaque block and every field is placed at its exact
// retail offset (base CWorkThread ends at 0x1C4).
class CScriptCode {
public:
    u8 unk_0[0x50];      //0x0: CWorkThread bytes up to mType
    u32 mType;           //0x50: ThreadType
    u8 unk_54[0x5C - 0x54];      //0x54: CWorkThread mAllocHandle/mParent
    reslist<CWorkThread*> mChildren; //0x5C: child work thread list
    u8 unk_7C[0x1C4 - 0x7C];      //0x7C: rest of CWorkThread
    void* m_slot0[128];  //0x1C4: primary slot array
    s16 m_cnt0;          //0x3C4: primary count
    s16 pad_0x3C6;       //0x3C6
    void* m_slot1[128];  //0x3C8: secondary slot array
    s16 m_cnt1;          //0x5C8: secondary count
    s16 pad_0x5CA;       //0x5CA
    CToken m_token;      //0x5CC
    u32 pad_0x1734;      //0x1734
    u32 m_1738;          //0x1738
};

// Entry stored in the m_slot0/m_slot1 command arrays: field_0 is the registration
// key, field_4 the associated value object.
struct CScriptCodeEntry {
    u32 field_0;
    u32 field_4;
};

// Interface for command handler objects dispatched through vtable slot 3. All
// pure virtuals so no vtable is emitted.
class CCmdDispatcher {
public:
    virtual void d0() = 0;
    virtual void d1() = 0;
    virtual void d2() = 0;
    virtual int cmdHandler(CToken& token, void* arg) = 0;
};

// Forward declaration (called by func_8043A1DC)
extern "C" int func_8043A390__11CScriptCodeFv(void* self, void* pData, int doSecondary);

// us-8043ca78: CScriptCode::~CScriptCode()
extern "C" void __dt__11CScriptCodeFv(void* self, int freeFlag) {
    CScriptCode* ths = (CScriptCode*)self;
    if (self == NULL) return;

    // Set vtable
    *(void**)self = (void*)lbl_eu_8056B418;

    // Free primary slot array entries
    for (int i = 0; i < ths->m_cnt0; i++) {
        if (ths->m_slot0[i] != NULL) {
            ::operator delete(ths->m_slot0[i]);
            ths->m_slot0[i] = NULL;
        }
    }

    // Free secondary slot array entries
    for (int i = 0; i < ths->m_cnt1; i++) {
        if (ths->m_slot1[i] != NULL) {
            ::operator delete(ths->m_slot1[i]);
            ths->m_slot1[i] = NULL;
        }
    }

    ths->m_cnt0 = 0;
    ths->m_cnt1 = 0;
    lbl_eu_806655B0[0] = NULL;

    // Destroy token member and base class
    ths->m_token.~CToken();
    ((CWorkThread*)ths)->CWorkThread::~CWorkThread();

    if (freeFlag > 0) {
        ::operator delete(self);
    }
}

// us-8043cb74: CScriptCode::func_8043A1DC()
// Line parser: copies whitespace-delimited lines from pData into a local
// buffer, calls func_8043A390 on each, then skips to the next line.
extern "C" void func_8043A1DC__11CScriptCodeFv(void* self, u8* pData, u32 dataSize) {
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
        func_8043A390__11CScriptCodeFv(self, lineBuf, 1);

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

// us-8043cc90: CScriptCode::func_8043A2F8()
// Splits pIn on '|' into 256-byte segments stored into pOut. Each segment is
// null-terminated. Returns the number of segments (incl. the final one) when
// the input ends with '\0', or 8 once 8 segments have been filled.
extern "C" s16 func_8043A2F8__11CScriptCodeFv(void* self, u8* pOut, u8* pIn) {
    int base = 0;
    s16 seg = 0;
    char c;
    s16 pos = 0;

    for (;;) {
        c = (char)*pIn;
        if (c == '|') {
            seg++;
            pOut[base + pos] = 0;
            pos = 0;
            pIn++;
            base += 0x100;
            if (seg < 8) continue;
            return 8;
        } else if (c == 0) {
            pOut[(seg << 8) + pos] = 0;
            return (s16)(seg + 1);
        } else {
            pOut[base + pos] = c;
            pIn++;
            pos++;
        }
    }
}

// us-8043d0a4: CScriptCode::func_8043A70C()
// Registers a (key, value) pair in the global ScriptCode singleton's primary
// slot array. Does nothing if the key is already registered.
extern "C" void func_8043A70C__11CScriptCodeFv(void* self, void* pValue) {
    CScriptCode* inst = (CScriptCode*)lbl_eu_806655B0[0];
    s16 cnt = inst->m_cnt0;

    // Search for an existing entry whose key matches `self`
    for (int i = 0; i < cnt; i++) {
        u32* entry = (u32*)inst->m_slot0[i];
        if (entry[0] == (u32)self) return;
    }

    // Allocate and fill a new 8-byte entry
    mtl::ALLOC_HANDLE mem = CWorkSystem::getMem();
    u32* entry = (u32*)mtl::MemManager::allocate(8, mem);
    memset(entry, 0, 8);
    entry[0] = (u32)self;
    entry[1] = (u32)pValue;

    // Append to the global instance's primary array
    inst = (CScriptCode*)lbl_eu_806655B0[0];
    s16 idx = inst->m_cnt0;
    inst->m_slot0[idx] = entry;
    inst->m_cnt0 = idx + 1;
}

// us-8043d1b8: CScriptCode::create()
extern "C" void* create__11CScriptCodeFP11CWorkThread(CWorkThread* pParent) {
    mtl::ALLOC_HANDLE mem = CWorkThreadSystem::getWorkMem();
    CScriptCode* ths = (CScriptCode*)mtl::MemManager::allocate(0x1740, mem);

    if (ths != NULL) {
        // Construct base class and initialize the object
        new (ths) CWorkThread(lbl_eu_8052254C, pParent, 1);

        *(void**)ths = (void*)lbl_eu_8056B418;
        ths->m_cnt0 = 0;
        ths->m_cnt1 = 0;

        new (&ths->m_token) CToken();
        ths->m_1738 = 0;
        lbl_eu_806655B0[0] = ths;
        ths->m_token.func_8043AA1C();

        ths->mType = CWorkThread::THREAD_CSCRIPTCODE;
    }

    CWorkUtil::entryWork((CWorkThread*)ths, pParent, false);

    return ths;
}

// us-8043cd28: CScriptCode::func_8043A390()
// Script-command line dispatcher. Splits the input on '|' into commands; for each
// command segment, searches the secondary slot array (then the primary, if
// doSecondary) for an entry whose key string matches the segment, and dispatches
// to the matched handler(s) through vtable slot 3 with the token. Returns 1 when
// no command matched.
extern "C" int func_8043A390__11CScriptCodeFv(void* self, void* pData, int doSecondary) {
    u8 buf[0x800];
    s16 segCount = func_8043A2F8__11CScriptCodeFv((CScriptCode*)lbl_eu_806655B0[0], buf, (u8*)pData);

    for (s16 s = 0; s < segCount; s++) {
        const u8* seg = &buf[s * 0x100];

        // Search the secondary slot array (m_slot1)
        for (s16 i = 0; i < ((CScriptCode*)lbl_eu_806655B0[0])->m_cnt1; i++) {
            CScriptCodeEntry* entry = (CScriptCodeEntry*)((CScriptCode*)lbl_eu_806655B0[0])->m_slot1[i];
            if (((CScriptCode*)lbl_eu_806655B0[0])->m_token.func_8043ABD4((const char*)entry->field_0, (const char*)seg)) {
                int r = 0;
                if (self != NULL) {
                    r = ((CCmdDispatcher*)self)->cmdHandler(((CScriptCode*)lbl_eu_806655B0[0])->m_token, (void*)entry->field_0);
                }
                if (entry->field_4 != NULL) {
                    return ((CCmdDispatcher*)(void*)entry->field_4)
                        ->cmdHandler(((CScriptCode*)lbl_eu_806655B0[0])->m_token, (void*)entry->field_0);
                }
                return r;
            }
        }

        if (doSecondary) {
            // Search the primary slot array (m_slot0)
            for (s16 i = 0; i < ((CScriptCode*)lbl_eu_806655B0[0])->m_cnt0; i++) {
                CScriptCodeEntry* entry = (CScriptCodeEntry*)((CScriptCode*)lbl_eu_806655B0[0])->m_slot0[i];
                if (((CScriptCode*)lbl_eu_806655B0[0])->m_token.func_8043ABD4((const char*)entry->field_0, (const char*)seg)) {
                    int r = 0;
                    if (self != NULL) {
                        r = ((CCmdDispatcher*)self)->cmdHandler(((CScriptCode*)lbl_eu_806655B0[0])->m_token, (void*)entry->field_0);
                    }
                    if (entry->field_4 != NULL) {
                        r = ((CCmdDispatcher*)(void*)entry->field_4)
                            ->cmdHandler(((CScriptCode*)lbl_eu_806655B0[0])->m_token, (void*)entry->field_0);
                    }
                    ((CScriptCode*)lbl_eu_806655B0[0])->m_1738 = (u32)entry;
                    return r;
                }
            }
        }
    }

    return 1;
}

// us-8043cf14: CScriptCode::func_8043A57C()
// Unregisters the first slot entry whose key matches `self`: frees the entry
// object, then compacts the array by shifting all later entries down one slot
// and decrements the count. Searches the primary slot array first, falling back
// to the secondary slot array.
extern "C" void func_8043A57C__11CScriptCodeFv(void* self, void* pTarget) {
    u32 key = (u32)self;

    // Primary slot array: find first entry whose key matches, free + compact.
    // The singleton is re-read after the delete/compaction (retail keeps inst
    // and cnt in volatile regs across the search, not callee-saved).
    s16 cnt = ((CScriptCode*)lbl_eu_806655B0[0])->m_cnt0;
    for (s16 i = 0; i < cnt; i++) {
        CScriptCodeEntry* entry = (CScriptCodeEntry*)((CScriptCode*)lbl_eu_806655B0[0])->m_slot0[i];
        if (entry->field_0 == key) {
            if (entry != NULL) {
                ::operator delete(entry);
                ((CScriptCode*)lbl_eu_806655B0[0])->m_slot0[i] = NULL;
            }
            while (i < ((CScriptCode*)lbl_eu_806655B0[0])->m_cnt0 - 1) {
                ((CScriptCode*)lbl_eu_806655B0[0])->m_slot0[i]
                    = ((CScriptCode*)lbl_eu_806655B0[0])->m_slot0[i + 1];
                i++;
            }
            ((CScriptCode*)lbl_eu_806655B0[0])->m_cnt0 =
                ((CScriptCode*)lbl_eu_806655B0[0])->m_cnt0 - 1;
            return;
        }
    }

    // Secondary slot array
    cnt = ((CScriptCode*)lbl_eu_806655B0[0])->m_cnt1;
    for (s16 i = 0; i < cnt; i++) {
        CScriptCodeEntry* entry = (CScriptCodeEntry*)((CScriptCode*)lbl_eu_806655B0[0])->m_slot1[i];
        if (entry->field_0 == key) {
            if (entry != NULL) {
                ::operator delete(entry);
                ((CScriptCode*)lbl_eu_806655B0[0])->m_slot1[i] = NULL;
            }
            while (i < ((CScriptCode*)lbl_eu_806655B0[0])->m_cnt1 - 1) {
                ((CScriptCode*)lbl_eu_806655B0[0])->m_slot1[i]
                    = ((CScriptCode*)lbl_eu_806655B0[0])->m_slot1[i + 1];
                i++;
            }
            ((CScriptCode*)lbl_eu_806655B0[0])->m_cnt1 =
                ((CScriptCode*)lbl_eu_806655B0[0])->m_cnt1 - 1;
            return;
        }
    }
}

// us-... getInstance
extern "C" void* getInstance__11CScriptCodeFv(void) {
    return lbl_eu_806655B0;
}

// us-8043d158: CScriptCode::wkStandbyLogout()
// Logs out only while the child list is empty and neither the proc root nor
// the view root has been created yet; otherwise reports failure.
extern "C" bool wkStandbyLogout__11CScriptCodeFv(void* self) {
    CScriptCode* ths = (CScriptCode*)self;
    if (ths->mChildren.empty()
        && CProcRoot::getInstance() == NULL
        && CViewRoot::getInstance() == NULL) {
        return ((CWorkThread*)ths)->CWorkThread::wkStandbyLogout();
    }
    return false;
}