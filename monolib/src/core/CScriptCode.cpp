// CScriptCode - Script code management for CWorkThread system
// Decompiled from retail assembly

#include <harness_catalog.h>

// External declarations
extern void* lbl_eu_8056B418;  // vtable
extern void* lbl_eu_8052254C;  // name string
extern void* lbl_eu_806655B0;  // singleton instance

// External function declarations
s32 func_8043ABD4__6CTokenFPCcPCc(void* token, const char* a, const char* b);
void func_8043AA1C__6CTokenFv(void* token);
void* getMem__11CWorkSystemFv();
void* allocate__Q23mtl10MemManagerFUlUl(u32 size, u32 align);
void* getWorkMem__17CWorkThreadSystemFv();
void __ct__11CWorkThreadFPCcP11CWorkThreadi(const char* name, void* parent, s32 flag);
void __ct__6CTokenFv(void* token);
void __dt__6CTokenFv(void* token, s32 flag);
void __dt__11CWorkThreadFv(void* self, s32 flag);
void entryWork__9CWorkUtilFP11CWorkThreadP11CWorkThreadb(void* work, void* parent, s32 flag);
void* getInstance__9CProcRootFv();
void* getInstance__9CViewRootFv();
void wkStandbyLogout__11CWorkThreadFv(void* self);
void __dl__FPv(void* ptr);

// CScriptCode::~CScriptCode()
void __dt__11CScriptCodeFv(void* self, s32 flag) {
    if (self == NULL) return;
    
    // Set vtable
    *(void**)self = &lbl_eu_8056B418;
    
    // Free script entries (array at 0x1C4, count at 0x3C4)
    // Use pointer iteration to match retail codegen
    void* scriptBase = (u8*)self + 0x1C4;
    s16 scriptCount = *(s16*)((u8*)self + 0x3C4);
    s16 scriptIdx = 0;
    void* scriptPtr = scriptBase;
    
    while (scriptIdx < scriptCount) {
        void* entry = *(void**)scriptPtr;
        if (entry != NULL) {
            __dl__FPv(entry);
            *(void**)scriptPtr = NULL;
        }
        scriptPtr = (u8*)scriptPtr + 4;
        scriptIdx++;
    }
    
    // Free command entries (array at 0x3C8, count at 0x5C8)
    void* cmdBase = (u8*)self + 0x3C8;
    s16 cmdCount = *(s16*)((u8*)self + 0x5C8);
    s16 cmdIdx = 0;
    void* cmdPtr = cmdBase;
    
    while (cmdIdx < cmdCount) {
        void* entry = *(void**)cmdPtr;
        if (entry != NULL) {
            __dl__FPv(entry);
            *(void**)cmdPtr = NULL;
        }
        cmdPtr = (u8*)cmdPtr + 4;
        cmdIdx++;
    }
    
    // Reset counts
    *(s16*)((u8*)self + 0x3C4) = 0;
    *(s16*)((u8*)self + 0x5C8) = 0;
    
    // Clear singleton
    lbl_eu_806655B0 = NULL;
    
    // Destroy token at 0x5CC
    __dt__6CTokenFv((u8*)self + 0x5CC, -1);
    
    // Destroy base class
    __dt__11CWorkThreadFv(self, 0);
    
    // Free memory if requested
    if (flag > 0) {
        __dl__FPv(self);
    }
}

// CScriptCode::getInstance()
void* getInstance__11CScriptCodeFv() {
    return lbl_eu_806655B0;
}

// CScriptCode::func_8043A1DC - Parse script lines
s32 func_8043A1DC__11CScriptCodeFv(void* self, const char* script, s32 length) {
    if (script == NULL) return 0;
    
    char buffer[256];
    s32 pos = 0;
    const char* src = script;
    char* dst = buffer;
    s32 lineLen = 0;
    
    while (*src != '\0') {
        // Copy line, replacing tabs with spaces
        lineLen = 0;
        while (lineLen < 255 && pos < length) {
            char c = *src;
            if (c < 0x20 && c != '\t') break;
            
            *dst = (c == '\t') ? ' ' : c;
            src++;
            dst++;
            pos++;
            lineLen++;
        }
        
        // Null terminate
        *dst = '\0';
        
        // Process line
        func_8043A390__11CScriptCodeFv(0, buffer, 1);
        
        // Skip to next line
        while (*src != '\0') {
            if (*src == '\n') {
                src++;
                pos++;
                break;
            }
            src++;
            pos++;
        }
        dst = buffer;
    }
    
    return 0;
}

// CScriptCode::func_8043A2F8 - Split string by delimiter
s32 func_8043A2F8__11CScriptCodeFv(void* self, const char* input, char* output, const char* delim) {
    s32 fieldIndex = 0;
    s32 charIndex = 0;
    s32 offset = 0;
    s32 count = 0;
    
    while (true) {
        char c = delim[count];
        s32 val = (s32)(s8)c;
        
        if (val == '|') {
            fieldIndex++;
            s32 idx = (s32)(s16)charIndex;
            s32 fld = (s32)(s16)fieldIndex;
            
            output[offset + idx] = '\0';
            charIndex = 0;
            count++;
            offset += 256;
            
            if (fld >= 8) {
                return 8;
            }
            continue;
        }
        
        if (val == 0) {
            s32 idx = (s32)(s16)charIndex;
            s32 fld = (s32)(s16)fieldIndex;
            
            output[offset + idx] = '\0';
            return fld + 1;
        }
        
        s32 idx = (s32)(s16)charIndex;
        output[offset + idx] = c;
        count++;
        charIndex++;
    }
}

// CScriptCode::func_8043A390 - Match and execute script command
s32 func_8043A390__11CScriptCodeFv(s32 callback, const char* script, s32 flag) {
    char buffer[2048];
    void* instance = lbl_eu_806655B0;
    
    s32 fieldCount = func_8043A2F8__11CScriptCodeFv(instance, script, buffer, "|");
    
    for (s32 i = 0; i < fieldCount; i++) {
        // Try command entries first
        s16 commandCount = *(s16*)((u8*)instance + 0x5C8);
        for (s32 j = 0; j < commandCount; j++) {
            void* entry = ((void**)((u8*)instance + 0x3C8))[j];
            const char* name = *(const char**)entry;
            
            if (func_8043ABD4__6CTokenFPCcPCc((u8*)instance + 0x5CC, name, &buffer[i * 256])) {
                // Match found
                if (callback != 0) {
                    // Call callback with matched entry
                    typedef void (*CallbackFn)(s32, void*, const char*);
                    void** vtable = *(void***)callback;
                    CallbackFn fn = (CallbackFn)vtable[3];
                    fn(callback, (u8*)instance + 0x5CC, name);
                }
                
                // Check for sub-entry
                void* subEntry = ((void**)entry)[1];
                if (subEntry != NULL) {
                    typedef void (*SubEntryFn)(s32, void*, const char*);
                    void** vtable = *(void***)subEntry;
                    SubEntryFn fn = (SubEntryFn)vtable[3];
                    fn(callback, (u8*)instance + 0x5CC, name);
                }
                
                return 0;
            }
        }
        
        // Try script entries if flag is set
        if (flag != 0) {
            s16 scriptCount = *(s16*)((u8*)instance + 0x3C4);
            for (s32 j = 0; j < scriptCount; j++) {
                void* entry = ((void**)((u8*)instance + 0x1C4))[j];
                const char* name = *(const char**)entry;
                
                if (func_8043ABD4__6CTokenFPCcPCc((u8*)instance + 0x5CC, name, &buffer[i * 256])) {
                    // Match found
                    if (callback != 0) {
                        typedef void (*CallbackFn)(s32, void*, const char*);
                        void** vtable = *(void***)callback;
                        CallbackFn fn = (CallbackFn)vtable[3];
                        fn(callback, (u8*)instance + 0x5CC, name);
                    }
                    
                    // Check for sub-entry
                    void* subEntry = ((void**)entry)[1];
                    if (subEntry != NULL) {
                        typedef void (*SubEntryFn)(s32, void*, const char*);
                        void** vtable = *(void***)subEntry;
                        SubEntryFn fn = (SubEntryFn)vtable[3];
                        fn(callback, (u8*)instance + 0x5CC, name);
                    }
                    
                    // Store matched entry
                    *(void**)((u8*)instance + 0x1738) = entry;
                    return 0;
                }
            }
        }
    }
    
    return 1;
}

// CScriptCode::func_8043A57C - Remove entry by name
void func_8043A57C__11CScriptCodeFv(void* self, const char* name) {
    void* instance = lbl_eu_806655B0;
    
    // Search script entries
    s16 scriptCount = *(s16*)((u8*)instance + 0x3C4);
    void** scriptArray = (void**)((u8*)instance + 0x1C4);
    for (s32 i = 0; i < scriptCount; i++) {
        void* entry = scriptArray[i];
        if (*(const char**)entry == name) {
            // Free entry
            if (entry != NULL) {
                __dl__FPv(entry);
                scriptArray[i] = NULL;
            }
            
            // Shift entries down
            for (s32 j = i; j < scriptCount - 1; j++) {
                scriptArray[j] = scriptArray[j + 1];
            }
            *(s16*)((u8*)instance + 0x3C4) = scriptCount - 1;
            return;
        }
    }
    
    // Search command entries
    s16 commandCount = *(s16*)((u8*)instance + 0x5C8);
    void** cmdArray = (void**)((u8*)instance + 0x3C8);
    for (s32 i = 0; i < commandCount; i++) {
        void* entry = cmdArray[i];
        if (*(const char**)entry == name) {
            // Free entry
            if (entry != NULL) {
                __dl__FPv(entry);
                cmdArray[i] = NULL;
            }
            
            // Shift entries down
            for (s32 j = i; j < commandCount - 1; j++) {
                cmdArray[j] = cmdArray[j + 1];
            }
            *(s16*)((u8*)instance + 0x5C8) = commandCount - 1;
            return;
        }
    }
}

// CScriptCode::func_8043A70C - Add command entry
void func_8043A70C__11CScriptCodeFv(void* self, const char* name, void* callback) {
    void* instance = lbl_eu_806655B0;
    
    // Check if already exists
    s16 scriptCount = *(s16*)((u8*)instance + 0x3C4);
    void** scriptArray = (void**)((u8*)instance + 0x1C4);
    for (s32 i = 0; i < scriptCount; i++) {
        void* entry = scriptArray[i];
        if (*(const char**)entry == name) {
            return;
        }
    }
    
    // Allocate new entry
    void* mem = getMem__11CWorkSystemFv();
    void* entry = allocate__Q23mtl10MemManagerFUlUl(8, (u32)mem);
    
    // Initialize entry
    memset(entry, 0, 8);
    *(const char**)entry = name;
    *(void**)((u8*)entry + 4) = callback;
    
    // Add to array
    s32 idx = (s32)(s16)scriptCount;
    scriptArray[idx] = entry;
    *(s16*)((u8*)instance + 0x3C4) = (s16)(idx + 1);
}

// CScriptCode::wkStandbyLogout
s32 wkStandbyLogout__11CScriptCodeFv(void* self) {
    // Check if work list is empty
    void* workList = *(void**)((u8*)self + 0x60);
    if (*(void**)workList != workList) {
        return 0;
    }
    
    // Check singletons
    if (getInstance__9CProcRootFv() != NULL) {
        return 0;
    }
    if (getInstance__9CViewRootFv() != NULL) {
        return 0;
    }
    
    // Call base class implementation
    wkStandbyLogout__11CWorkThreadFv(self);
    return 1;
}

// CScriptCode::create - Static factory method
void* create__11CScriptCodeFP11CWorkThread(void* parent) {
    void* mem = getWorkMem__17CWorkThreadSystemFv();
    void* instance = allocate__Q23mtl10MemManagerFUlUl(0x1740, (u32)mem);
    
    if (instance != NULL) {
        // Construct base class
        __ct__11CWorkThreadFPCcP11CWorkThreadi((const char*)&lbl_eu_8052254C, parent, 1);
        
        // Set vtable
        *(void**)instance = &lbl_eu_8056B418;
        
        // Initialize arrays
        *(s16*)((u8*)instance + 0x3C4) = 0;
        *(s16*)((u8*)instance + 0x5C8) = 0;
        
        // Construct token at 0x5CC
        __ct__6CTokenFv((u8*)instance + 0x5CC);
        
        // Initialize last matched
        *(void**)((u8*)instance + 0x1738) = NULL;
        
        // Initialize token
        func_8043AA1C__6CTokenFv((u8*)instance + 0x5CC);
        
        // Set work type
        *(u32*)((u8*)instance + 0x50) = 0x3B;
    }
    
    // Register with parent
    entryWork__9CWorkUtilFP11CWorkThreadP11CWorkThreadb(instance, parent, 0);
    
    return instance;
}
