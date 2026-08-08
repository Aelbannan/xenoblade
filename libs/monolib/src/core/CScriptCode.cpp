// High-level C decompilation for monolib/src/core/CScriptCode
// Uses extern "C" linkage to match MWCC mangled symbols.

#include <harness_catalog.h>
#include "monolib/core/CScriptCode.hpp"

extern "C" {
    extern void* lbl_eu_806655B0;
    void __dl__FPv(void* p);
    void __dt__6CTokenFv(void* obj, int flag);
    void __dt__11CWorkThreadFv(void* obj, int flag);
    void* getMem__11CWorkSystemFv();
    void* allocate__Q23mtl10MemManagerFUlUl(void* mem, unsigned long size);
    void* getWorkMem__17CWorkThreadSystemFv();
    void __ct__11CWorkThreadFPCcP11CWorkThreadi(void* obj, const char* name, void* parent, int flag);
    void __ct__6CTokenFv(void* obj);
    void func_8043AA1C__6CTokenFv(void* obj);
    void entryWork__9CWorkUtilFP11CWorkThreadP11CWorkThreadb(void* a, void* b, int flag);
    int func_8043ABD4__6CTokenFPCcPCc(void* token, const char* a, const char* b);
    void* getInstance__9CProcRootFv();
    void* getInstance__9CViewRootFv();
    int wkStandbyLogout__11CWorkThreadFv(void* self);
}

extern "C" void __dt__11CScriptCodeFv(void* self, int freeFlag) {
    int i;
    void* inst;
    if (self == NULL) return;

    // Set vtable
    *(u32*)self = 0x8056B418;

    inst = self;

    // Delete primary array entries (offset 0x1C4, count at 0x3C4)
    for (i = 0; i < *(s16*)((u8*)self + 0x3C4); i++) {
        void* ptr = *(void**)((u8*)inst + 0x1C4);
        if (ptr != NULL) {
            __dl__FPv(ptr);
            *(u32*)((u8*)inst + 0x1C4) = 0;
        }
        inst = (u8*)inst + 4;
    }

    inst = self;

    // Delete secondary array entries (offset 0x3C8, count at 0x5C8)
    for (i = 0; i < *(s16*)((u8*)self + 0x5C8); i++) {
        void* ptr = *(void**)((u8*)inst + 0x3C8);
        if (ptr != NULL) {
            __dl__FPv(ptr);
            *(u32*)((u8*)inst + 0x3C8) = 0;
        }
        inst = (u8*)inst + 4;
    }

    *(s16*)((u8*)self + 0x3C4) = 0;
    *(s16*)((u8*)self + 0x5C8) = 0;
    lbl_eu_806655B0 = NULL;

    // Destroy token member at offset 0x5CC
    __dt__6CTokenFv((u8*)self + 0x5CC, -1);

    // Call base class destructor
    __dt__11CWorkThreadFv(self, 0);

    if (freeFlag > 0) {
        __dl__FPv(self);
    }
}

extern "C" u32 getInstance__11CScriptCodeFv(void) {
    return (u32)lbl_eu_806655B0;
}

// Line parser: processes pData, splitting into lines and calling func_8043A390
extern "C" void func_8043A1DC__11CScriptCodeFv(void* self, void* pData, u32 dataSize) {
    u32 offset = 0;
    u8 lineBuf[256];
    u32 lineLen;
    u32 count;

    if (pData == NULL) return;

    while (((u8*)pData)[offset] != 0) {
        lineLen = 0;
        count = 0;

        // Copy bytes until control char or 256 chars
        while (1) {
            u8 ch = ((u8*)pData)[offset];
            if (ch < 0x20 && ch != 0x09) break;
            lineBuf[lineLen] = ch;
            offset++;
            lineLen++;
            // Replace tabs with spaces
            if (ch == 0x09) {
                lineBuf[lineLen - 1] = 0x20;
            }
            count++;
            if (count >= 0xFF || offset >= dataSize) break;
        }

        if (offset >= dataSize) return;

        // Null-terminate and process
        lineBuf[lineLen] = 0;
        func_8043A390__11CScriptCodeFv((void*)((u32)lbl_eu_806655B0), lineBuf, 1);

        // Skip past newline
        while (1) {
            u8 ch = ((u8*)pData)[offset];
            if (ch == 0x0A) {
                offset++;
                break;
            }
            if (ch == 0 || offset >= dataSize) return;
            offset++;
        }
    }
}

// String segment parser: splits pIn on '|' into segments of 256 bytes
extern "C" long func_8043A2F8__11CScriptCodeFv(void* self, void* pOut, void* pIn) {
    u32 segIdx = 0;
    u32 charPos = 0;
    u32 segBase = 0;
    u32 segOff = 0;

    while (1) {
        u8 ch = ((u8*)pIn)[segOff];
        s8 sch = (s8)ch;

        if (sch == '|') {
            ((u8*)pOut)[segBase + charPos] = 0;
            segIdx++;
            charPos = 0;
            segOff++;
            segBase += 0x100;
            if ((s16)segIdx >= 8) {
                return 8;
            }
        } else if (sch == 0) {
            ((u8*)pOut)[segBase + charPos] = 0;
            return (short)(segIdx + 1);
        } else {
            ((u8*)pOut)[segBase + charPos] = ch;
            segOff++;
            charPos++;
        }
    }
}

// Segment processor: parses data and dispatches to callbacks
extern "C" void func_8043A390__11CScriptCodeFv(void* self, void* pData, int doSecondary) {
    // Stub - complex function with virtual calls
}

// Array search and delete: removes target from primary or secondary array
extern "C" void func_8043A57C__11CScriptCodeFv(void* self, void* pTarget) {
    // Stub - complex array manipulation
}

// Add key-value pair to primary array
extern "C" void func_8043A70C__11CScriptCodeFv(void* self, void* pKey, void* pValue) {
    int i;
    void* inst = lbl_eu_806655B0;
    s16 count = *(s16*)((u8*)inst + 0x3C4);

    // Check if key already exists
    void* arrPtr = inst;
    for (i = 0; i < count; i++) {
        void* entry = *(void**)((u8*)arrPtr + 0x1C4);
        if (*(u32*)entry == (u32)pKey) {
            return; // Already exists
        }
        arrPtr = (u8*)arrPtr + 4;
    }

    // Allocate new entry (8 bytes)
    {
        void* mem = getMem__11CWorkSystemFv();
        void* entry = allocate__Q23mtl10MemManagerFUlUl(mem, 8);
        memset(entry, 0, 8);
        *(u32*)entry = (u32)pKey;
        *(u32*)((u8*)entry + 4) = (u32)pValue;

        // Add to array
        inst = lbl_eu_806655B0;
        s16 idx = *(s16*)((u8*)inst + 0x3C4);
        *(u32*)((u8*)inst + 0x1C4 + idx * 4) = (u32)entry;
        *(s16*)((u8*)inst + 0x3C4) = idx + 1;
    }
}

// Check if work list is empty and no root instances exist
extern "C" int wkStandbyLogout__11CScriptCodeFv(void* self) {
    void* listHead = *(void**)((u8*)self + 0x60);
    void* first = *(void**)listHead;

    if (first == listHead) {
        // List is empty, check singletons
        if (getInstance__9CProcRootFv() == NULL) {
            if (getInstance__9CViewRootFv() == NULL) {
                return wkStandbyLogout__11CWorkThreadFv(self);
            }
        }
    }

    return 0;
}

// Create and register a new CScriptCode instance
extern "C" void* create__11CScriptCodeFP11CWorkThread(void* self, void* pParent) {
    void* workMem = getWorkMem__17CWorkThreadSystemFv();
    void* obj = allocate__Q23mtl10MemManagerFUlUl(workMem, 0x1740);
    void* instance = obj;

    if (obj != NULL) {
        // Construct base CWorkThread
        __ct__11CWorkThreadFPCcP11CWorkThreadi(obj, "CScriptCode", pParent, 1);

        // Set vtable
        *(u32*)obj = 0x8056B418;

        // Initialize array counts
        *(s16*)((u8*)obj + 0x3C4) = 0;
        *(s16*)((u8*)obj + 0x5C8) = 0;

        // Construct token at offset 0x5CC
        __ct__6CTokenFv((u8*)obj + 0x5CC);

        // Initialize field
        *(u32*)((u8*)obj + 0x1738) = 0;

        // Initialize token
        func_8043AA1C__6CTokenFv((u8*)obj + 0x5CC);

        // Set type identifier
        *(u32*)((u8*)obj + 0x50) = 0x3B;

        // Set global instance
        lbl_eu_806655B0 = instance;
    }

    // Register with parent work thread
    entryWork__9CWorkUtilFP11CWorkThreadP11CWorkThreadb(instance, pParent, 0);

    return instance;
}
