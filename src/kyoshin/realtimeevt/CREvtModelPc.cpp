// TU: kyoshin/realtimeevt/CREvtModelPc
// Class: CREvtModelPc (inherits CREvtModel + IWorkEvent)

#include "kyoshin/harness_catalog.hpp"
#include "kyoshin/realtimeevt/CREvtModelPc.hpp"

#include <cstring>

// Helper macros for field access
#define FLD(type, base, off) (*(type*)((char*)(base) + (off)))
#define FLDP(type, base, off) ((type*)((char*)(base) + (off)))

// ==============================
// External data labels
// ==============================
extern "C" {
    extern u32 lbl_eu_805321F0[];    // CREvtModelPc vtable
    extern u32 lbl_eu_80532180[];    // ptmf constant
    extern u32 lbl_eu_8053218C[];    // ptmf constant
    extern u32 lbl_eu_80532198[];    // ptmf compare
    extern u32 lbl_eu_805321A4[];    // ptmf compare
    extern u32 lbl_eu_805321B0[];    // ptmf constant
    extern u32 lbl_eu_805321BC[];    // ptmf constant
    extern u32 lbl_eu_805321C8[];    // ptmf constant
    extern u32 lbl_eu_805321D4[];    // ptmf constant
    extern u32 lbl_eu_805321E0[];    // ptmf constant
    extern u32 lbl_eu_805037A8[];    // slot param lookup table
    extern void* lbl_eu_80663E14;      // scene / alloc handle
    extern f32 lbl_eu_80667918;      // float constant (0.0f)
    extern f64 lbl_eu_80667920;      // double constant (int-to-float conversion)
}

// ==============================
// External function declarations
// ==============================
extern "C" {
    // Base class (CREvtModel: this, parent, flag)
    extern void __ct__CREvtModel(void* self, void* parent, int flag);
    // CREvtModel member (this, flag)
    extern void __ct__80172668(void* self, int flag);
    // Dealloc
    extern void __dt__80185754(void* ptr);

    // CREvtModel helpers
    extern void func_801726DC(void* self);
    extern void func_80172768(void* self);
    extern void func_801729F0(void* self, int r4, int r5);

    // Memory
    extern void* func_80167F6C(void* ptr, u32 alignment, int useMEM1);

    // Task helpers
    extern int func_8016A3C4();
    extern int func_8016A35C();
    extern void func_8016BC1C(void* self);
    extern int func_8016BDA8(void* self);
    extern void* func_8016C300(void* self);
    extern void func_80168514(void* self);
    extern int func_801683FC();
    extern int func_8016840C();
    extern void* func_80164724(void* parent, u32 type, int slot);

    // CfGameManager
    extern void* func_80086B04__Q22cf13CfGameManagerFv();
    extern void* func_8007DE94__Q22cf13CfGameManagerFv(u32 type, int slot);
    extern void func_8007E038__Q22cf13CfGameManagerFv(u32 type, int flag);

    // Resource helpers
    extern void* func_80062AD8(void* handle, u32* outType);
    extern void func_800A9344(void* p, int type);
    // Packed-token helpers (retail C-symbol names)
    extern char* func_800AA5C0(void* handle);
    extern void func_800AA318(u32 packed, u32* out0, u32* out1, u32* out2, u32* out3);
    extern int func_800AA33C(ml::FixStr<64>& buf, u32 packed, int prefixFlag, int suffixFlag);
    extern u32 func_800AA2E8(u32 a, u32 b, u32 c);
    // Archive
    extern int func_800A8E6C(u32 value, int enable);

    // Model helpers
    extern void* func_80495E8C(void* a, void* b, int c, int d);
    extern void* func_80495FF0(void* arg);
    extern void func_80484E5C(void* model, f32 val);
    extern void func_804827DC(void* model, int flag);
    extern void func_80485684(void* model, int flag);
    extern void func_804831C4(void* model, void* texName);
    extern void func_80483448(void* model, void* texName);
    extern void func_80484F80(void* model, f32 val);

    // __ptmf intrinsics
    extern void func_8044F400__11CDeviceFileFP11CFileHandleUl(CFileHandle* pFileHandle, u32 val);

    extern u32 __ptmf_null[3];
    extern int __ptmf_cmpr(void* ptmf, void* other);
}

// ============================================================================
// __ct__CREvtModelPc (us-80184b40) - Constructor
// ============================================================================
extern "C" void __ct__CREvtModelPc(void* self, void* parent) {
    char* s = (char*)self;
    int i;

    // CREvtModel(this, parent, 2) - parent is passed through as r4.
    __ct__CREvtModel(self, parent, 2);

    // Set vtable
    u32* vtable = lbl_eu_805321F0;
    u32* iwvt = (u32*)((char*)vtable + 0x44);
    u32 zero = 0;
    FLD(u32*, s, 0x00) = vtable;
    FLD(u32*, s, 0x38) = iwvt;

    // Initialize fields
    FLD(f32, s, 0xA8) = lbl_eu_80667918;
    FLD(u8, s, 0xAC) = zero;
    FLD(s32, s, 0xB0) = -1;

    // Clear all 6 slots
    for (i = 0; i < 6; i++) {
        FLD(u32, s, 0x3C + i * 4) = zero;
        FLD(u32, s, 0x54 + i * 4) = zero;
        FLD(u32, s, 0x6C + i * 4) = zero;
        FLD(u32, s, 0x84 + i * 4) = zero;
    }

    // Copy ptmf from lbl_eu_80532180
    {
        u32* ptmf = lbl_eu_80532180;
        FLD(u32, s, 0x08) = ptmf[0];
        FLD(u32, s, 0x0C) = ptmf[1];
        FLD(u32, s, 0x10) = ptmf[2];
    }

    func_8016BC1C(self);

    u32* pParent = FLD(u32*, s, 0x1C);
    u32 field30 = pParent[0x30 / 4];

    if (field30 == 0xFFFFFFFF) {
        FLD(u32, s, 0x18) |= 0x50;
        u32* ptmf2 = lbl_eu_8053218C;
        FLD(u32, s, 0x08) = ptmf2[0];
        FLD(u32, s, 0x0C) = ptmf2[1];
        FLD(u32, s, 0x10) = ptmf2[2];
    } else if (field30 > 1) {
        FLD(u32, s, 0x18) |= 0x30;
    }
}

// ============================================================================
// __ct__8018385C (us-80184c78) - Constructor with dealloc flag
// ============================================================================
extern "C" void* __ct__8018385C(void* self, int flag) {
    if (self == 0) {
        return self;
    }

    char* s = (char*)self;
    u32* vtable = lbl_eu_805321F0;

    FLD(u32*, s, 0x00) = vtable;
    FLD(u32*, s, 0x38) = (u32*)((char*)vtable + 0x44);

    // Call vtable[0x3C/4] = vtable[15]
    {
        void (*vfunc)(void*) = (void (*)(void*))vtable[0x3C / 4];
        vfunc(self);
    }

    __ct__80172668(self, 0);

    if (flag > 0) {
        __dt__80185754(self);
    }

    return self;
}

// ============================================================================
// func_801838D8 (us-80184cf4) - Check if ready
// ============================================================================
extern "C" int func_801838D8(void* self) {
    char* s = (char*)self;

    // Ready only if the task counter matches the current one.
    if (FLD(s32, s, 0xB0) != (int)func_8016A3C4() + 1) {
        return 0;
    }

    int result = 0;
    u32 field30 = FLD(u32, FLD(void*, s, 0x1C), 0x30);
    if (field30 > 1) {
        // Ready if the callback ptmf is one of the two accepted constants.
        if (__ptmf_cmpr(FLDP(void, s, 0x08), lbl_eu_80532198) == 0 ||
            __ptmf_cmpr(FLDP(void, s, 0x08), lbl_eu_805321A4) == 0) {
            result = 1;
        }
    }
    return result;
}

// ============================================================================
// func_80183978 (us-80184d94) - Cleanup / cancellation
// ============================================================================
extern "C" int func_80183978(void* self) {
    char* s = (char*)self;

    // Bail out early if the "in use" flag bit is already set.
    if (FLD(u32, s, 0x18) & 0x80) {
        return 0;
    }

    func_801726DC(self);

    u32 i;
    char* base = s;  // walks per-slot via each slot's status word
    for (i = 0; i < 6; i++) {
        int status = FLD(s32, base, 0x84);

        if (status == 3) {
            // Loaded into MEM2 via MemManager: free it.
            void* data = FLD(void*, base, 0x6C);
            if (data != 0) {
                if (data != 0) {
                    mtl::MemManager::deallocate(data);
                    FLD(u32, base, 0x6C) = 0;
                }
            }
        } else if (status == 2) {
            // Archived: release the archive allocation.
            void* data = FLD(void*, base, 0x6C);
            if (data != 0) {
                func_800A9344(data, 0);
            }
        }

        void* fileReq = FLD(void*, base, 0x54);
        if (fileReq != 0) {
            CDeviceFile::cancel((CFileHandle*)fileReq);
            FLD(u32, base, 0x54) = 0;
        }
        base += 4;
    }

    return 1;
}

// ============================================================================
// func_80183A3C (us-80184e58) - Reset / reinitialize
// ============================================================================
extern "C" void func_80183A3C(void* self) {
    int i;

    if (FLD(u32, FLD(void*, self, 0x1C), 0x30) == 0xFFFFFFFF) {
        FLD(u32, self, 0x18) |= 0x50;
        FLD(u32, self, 0x08) = lbl_eu_805321B0[0];
        FLD(u32, self, 0x0C) = lbl_eu_805321B0[1];
        FLD(u32, self, 0x10) = lbl_eu_805321B0[2];
        return;
    }

    // If any slot is still loading (status == 1), or the reset flag is set,
    // bail out early.
    if (FLD(u32, self, 0x84) == 1 ||
        FLD(u32, self, 0x88) == 1 ||
        FLD(u32, self, 0x8C) == 1 ||
        FLD(u32, self, 0x90) == 1 ||
        FLD(u32, self, 0x94) == 1 ||
        FLD(u32, self, 0x98) == 1) return;

    if (FLD(u32, self, 0x18) & 0x100) return;
    FLD(u32, self, 0x18) |= 0x100;

    func_80172768(self);

    // Free any loaded/archived slot data.
    for (i = 0; i < 6; i++) {
        u32 status = FLD(u32, self, 0x84 + i * 4);
        if (status == 3) {
            void* data = FLD(void*, self, 0x6C + i * 4);
            if (data != 0) {
                mtl::MemManager::deallocate(data);
                FLD(void*, self, 0x6C + i * 4) = 0;
            }
        } else if (status == 2) {
            void* data = FLD(void*, self, 0x6C + i * 4);
            if (data != 0) {
                func_800A9344(data, 0);
            }
        }
    }

    FLD(u32, self, 0x08) = lbl_eu_805321BC[0];
    FLD(u32, self, 0x0C) = lbl_eu_805321BC[1];
    FLD(u32, self, 0x10) = lbl_eu_805321BC[2];
    FLD(u32, self, 0x18) = (FLD(u32, self, 0x18) | 0x30) & ~0x42;

    // Reinitialize all six slots.
    for (i = 0; i < 6; i++) {
        FLD(u32, self, 0x3C + i * 4) = 0;
        FLD(u32, self, 0x54 + i * 4) = 0;
        FLD(u32, self, 0x6C + i * 4) = 0;
        FLD(u32, self, 0x84 + i * 4) = 0;
    }
    FLD(u8, self, 0xAC) = 0;
}

// ============================================================================
// func_80183C1C (us-80185038) - Init counter
// ============================================================================
extern "C" void func_80183C1C(void* self) {
    char* s = (char*)self;

    FLD(s32, s, 0xB0) = -1;

    int result = func_8016BDA8(FLDP(void, s, 0xB0));
    if (result != 0) {
        if (FLD(u32, s, 0x18) & 0x100) {
            FLD(u32, s, 0x18) &= ~0x200;
        }

        u32* ptmf = lbl_eu_805321C8;
        FLD(u32, s, 0x08) = ptmf[0];
        FLD(u32, s, 0x0C) = ptmf[1];
        FLD(u32, s, 0x10) = ptmf[2];
    }
}

// ============================================================================
// func_80183C90 (us-801850ac) - Main file loading function
// ============================================================================
extern "C" void func_80183C90(void* self) {
    char* s = (char*)self;
    u32 entryId, param1, param2, param3;
    u32 objEntryId, objParam1, objParam2, objParam3;
    void* gameMgr;
    void* obj;
    void* objList;
    void* objPtr;
    void* matchChr = 0;
    void* chr;
    int isNewFile = 0;
    int i;
    int hasAllHandles = 0;
    int skipLoading = 0;
    int slotParam;
    void* parent;
    u32 packedBase;
    char pathBuf[0x40];
    void* handle;
    int fileSize;
    void* alloc;
    int archiveResult;
    void* workEvent;
    CFileHandle* reqHandle;
    void* memHandle;
    void* memHandle2;
    void* h;
    void* h2;

    // Set flags bit 6
    FLD(u32, s, 0x18) |= 0x40;

    // Get parent's packed token
    parent = FLD(void*, s, 0x1C);
    u32 parentPacked = FLD(u32, parent, 0x20);
    func_800AA318(parentPacked, &entryId, &param1, &param2, &param3);

    // Check if entry type is 3 or 8
    if (param1 == 3 || param1 == 8) {
        void* fileHandle = func_8016C300(self);
        isNewFile = (int)fileHandle; // r31 = fileHandle
        if (fileHandle != 0) {
            // non-zero fileHandle means isNewFile is true, skip the iteration
        } else {
            gameMgr = func_80086B04__Q22cf13CfGameManagerFv();
            objList = FLD(void*, gameMgr, 0x04);
            obj = objList;

            while (obj != 0) {
                objPtr = FLD(void*, obj, 0x08);
                if (objPtr != 0) {
                    objPtr = (char*)objPtr - 0x3E9C;
                }
                u16 chrType = FLD(u16, objPtr, 0x3F28);
                if ((chrType == 8 && param1 == 3) || (chrType == 3 && param1 == 8)) {
                    isNewFile = 1;
                    break;
                }
                obj = FLD(void*, obj, 0x00);
            }
        }
    }

    // Find matching character (r30 = matchChr)
    if (param1 <= 10) {
        gameMgr = func_80086B04__Q22cf13CfGameManagerFv();
        objList = FLD(void*, gameMgr, 0x04);
        obj = objList;

        while (obj != 0) {
            objPtr = FLD(void*, obj, 0x08);
            u32 objPacked = FLD(u32, objPtr, 0x70);
            func_800AA318(objPacked, &objEntryId, &objParam1, &objParam2, &objParam3);

            if (objParam1 == param1 && objParam2 == 1) {
                matchChr = FLD(void*, obj, 0x08);
                if (matchChr != 0) {
                    matchChr = (char*)matchChr - 0x3E9C;
                }
                break;
            }
            obj = FLD(void*, obj, 0x00);
        }
    }

    // Try allocating file handles from parent's resource
    parent = FLD(void*, s, 0x1C);
    if (FLD(s8, parent, 0x48) != 0 && !func_8016840C()) {
        hasAllHandles = 1;
        for (i = 0; i < 5; i++) {
            handle = func_80164724((char*)parent + 0x48, entryId, i + 1);
            FLD(u32, s, 0x3C + i * 4) = (u32)handle;
            if (handle == 0) {
                hasAllHandles = 0;
            }
        }
    }

    if (!hasAllHandles) {
        if (matchChr != 0 && !func_8016840C()) {
            // Get handles from matchChr via virtual call
            for (i = 0; i < 5; i++) {
                chr = (char*)matchChr + 0x3E9C;
                void* vtable = FLD(void*, chr, 0x00);
                void* (*getHandle)(void*, int) = (void* (*)(void*, int))FLD(u32, vtable, 0x148);
                handle = getHandle(chr, i + 1);
                FLD(u32, s, 0x3C + i * 4) = (u32)handle;
            }
        } else if (param2 == 1 && !func_8016840C()) {
            if (isNewFile) {
                func_8007E038__Q22cf13CfGameManagerFv(entryId, 0);
                skipLoading = 1;
            }
            // Get handles from func_8007DE94 (even when skipLoading is set)
            for (i = 0; i < 5; i++) {
                handle = func_8007DE94__Q22cf13CfGameManagerFv(entryId, i);
                FLD(u32, s, 0x3C + i * 4) = (u32)handle;
            }
        } else {
            slotParam = param2;
            if (func_8016840C()) {
                slotParam = FLD(u32, lbl_eu_805037A8, entryId * 4);
            }

            if (param1 == 8 && slotParam == 1) {
                slotParam = 4;
            }

            // Build packed tokens
            packedBase = (param3 << 27) | (entryId << 20) | (slotParam << 10);
            FLD(u32, s, 0x3C) = packedBase | 3;
            FLD(u32, s, 0x40) = packedBase | 2;
            FLD(u32, s, 0x44) = packedBase | 3;
            FLD(u32, s, 0x48) = packedBase | 4;
            FLD(u32, s, 0x4C) = packedBase | 5;
        }
    }

    // If skipLoading is set, skip the file loading loop
    if (skipLoading) {
        goto set_ptmf;
    }

    // Load files for slots 0-4
    for (i = 0; i <= 4; i++) {
        handle = FLD(void*, s, 0x3C + i * 4);
        if (handle == 0) continue;

        // Build filename
        func_800AA33C(*(ml::FixStr<64>*)(pathBuf), (u32)handle, 1, 1);

        // Get file size
        fileSize = CDeviceFile::getFileSize(pathBuf);

        // Try allocating from MEM1
        alloc = func_80167F6C((void*)fileSize, 0x20, 0);
        if (alloc != 0) {
            FLD(u32, s, 0x84 + i * 4) = 1;
        } else {
            // Try archive
            archiveResult = func_800A8E6C(fileSize, 1);
            if (archiveResult != 0) {
                func_800A8E6C(fileSize, 0);
            } else {
                archiveResult = 0;
            }

            if (archiveResult != 0) {
                FLD(u32, s, 0x84 + i * 4) = 2;
            } else {
                workEvent = (void*)(s + 0x38);
                reqHandle = CDeviceFile::readCommonArchiveFile(
                    (mtl::ALLOC_HANDLE)0, pathBuf,
                    (IWorkEvent*)workEvent, 0, 0);
                FLD(u32, s, 0x54 + i * 4) = (u32)reqHandle;
                if (reqHandle != 0) {
                    FLD(u32, s, 0x84 + i * 4) = 3;
                }
            }
        }
    }

    // Handle slot 5 (special case for type 9)
    if (param3 == 9) {
        // Create file handle for slot 5
        handle = (void*)func_800AA2E8(entryId, 1, 0);
        FLD(u32, s, 0x50) = (u32)handle;

        // Build filename
        func_800AA33C(*(ml::FixStr<64>*)(pathBuf), (u32)handle, 1, 1);

        // Get file size
        fileSize = CDeviceFile::getFileSize(pathBuf);

        // Try allocating
        alloc = func_80167F6C((void*)fileSize, 0x20, 0);
        if (alloc != 0) {
            FLD(u32, s, 0x98) = 1;
        } else {
            archiveResult = func_800A8E6C(fileSize, 0);
            if (archiveResult != 0) {
                FLD(u32, s, 0x98) = 2;
            }
        }

        if (alloc != 0) {
            workEvent = (void*)(s + 0x38);
            reqHandle = CDeviceFile::readCommonArchiveFile(
                (mtl::ALLOC_HANDLE)0, pathBuf,
                (IWorkEvent*)workEvent, 0, 0);
            FLD(u32, s, 0x68) = (u32)reqHandle;
        } else if (archiveResult != 0) {
            // Read from file
            memHandle = (void*)mtl::MemManager::getHandleMEM2();
            h = (void*)func_80495FF0(lbl_eu_80663E14);
            h2 = h;

            parent = FLD(void*, s, 0x1C);
            if (FLD(u32, parent, 0x58) & 0x2) {
                h = (void*)func_80495FF0(lbl_eu_80663E14);
                memHandle = h;
                h2 = (void*)mtl::MemManager::getHandleMEM2();
            }

            workEvent = (void*)(s + 0x38);
            reqHandle = CDeviceFile::readFile(
                (mtl::ALLOC_HANDLE)memHandle, pathBuf,
                (IWorkEvent*)workEvent, 0, 0);
            FLD(u32, s, 0x68) = (u32)reqHandle;

            func_8044F400__11CDeviceFileFP11CFileHandleUl((CFileHandle*)reqHandle, (u32)h2);

            parent = FLD(void*, s, 0x1C);
            if (FLD(u32, parent, 0x58) & 0x1) {
                CDeviceFile::setHandleFlag1(reqHandle);
            }

            if (func_801683FC()) {
                CDeviceFile::setHandleFlag2(reqHandle);
            }

            FLD(u32, s, 0x98) = 3;
        }
    }

set_ptmf:
    // Set ptmf from lbl_eu_805321D4
    {
        u32* ptmf = lbl_eu_805321D4;
        FLD(u32, s, 0x08) = ptmf[0];
        FLD(u32, s, 0x0C) = ptmf[1];
        FLD(u32, s, 0x10) = ptmf[2];
    }
}

// ============================================================================
// func_801845F0 (us-80185a18) - Check if all slots loaded
// ============================================================================
extern "C" void func_801845F0(void* self) {
    char* s = (char*)self;
    u32 tmp;

    // Get data for slot 0; func_80062AD8 returns the loaded pointer.
    void* data0;
    if (FLD(void*, s, 0x6C) != 0) {
        data0 = FLD(void*, s, 0x6C);
    } else {
        data0 = func_80062AD8(FLD(void*, s, 0x3C), &tmp);
    }

    // Confirm all remaining slots that have been requested are loaded.
    // Walk slot base (mFileHandle[1] is at s+0x40), offsets folded from s+4.
    char* base = (char*)s + 4;
    int allLoaded = 1;
    for (u32 i = 1; i <= 5; i++) {
        void* handle = FLD(void*, base, 0x3C);
        if (handle == 0) {
            base += 4;
            continue;
        }

        void* data;
        if (FLD(void*, base, 0x6C) != 0) {
            data = FLD(void*, base, 0x6C);
        } else {
            data = func_80062AD8(handle, &tmp);
        }

        if (data == 0) {
            allLoaded = 0;
        }
        base += 4;
    }

    if (data0 != 0 && allLoaded) {
        FLD(u32, s, 0x18) |= 0x11;
        u32* ptmf = lbl_eu_805321E0;
        FLD(u32, s, 0x0C) = ptmf[1];
        FLD(u32, s, 0x08) = ptmf[0];
        FLD(u32, s, 0x10) = ptmf[2];
    }
}

// ============================================================================
// func_801846C4 (us-80185aec) - Update model position
// ============================================================================
extern "C" void func_801846C4(void* self) {
    char* s = (char*)self;

    void* model = FLD(void*, s, 0x20);
    if (model != 0) {
        int time = func_8016A35C();
        f32 fTime = (f32)(s32)time;
        func_80484F80(model, fTime);
    }

    if (FLD(u32, s, 0x18) & 0x1000) {
        func_80168514(self);
    }
}

// ============================================================================
// func_80184728 (us-80185b5c) - Get model pointer (already implemented)
// ============================================================================
// u32 func_80184728(void* self) { return FLD(u32, self, 0x20); }

// ============================================================================
// func_80184730 (us-80185b64) - Create model from loaded data
// ============================================================================
extern "C" void func_80184730(void* self) {
    char* s = (char*)self;
    int i;

    if (!(FLD(u32, s, 0x18) & 0x100)) return;

    // Get data for slot 0
    void* data0 = FLD(void*, s, 0x6C);
    if (data0 == 0) {
        u32 type;
        func_80062AD8(FLD(void*, s, 0x3C), &type);
        data0 = (void*)type;
    }

    // Check slots 1-5
    int allLoaded = 1;
    for (i = 1; i <= 5; i++) {
        void* handle = FLD(void*, s, 0x3C + i * 4);
        if (handle == 0) continue;

        void* data = FLD(void*, s, 0x6C + i * 4);
        if (data == 0) {
            u32 type;
            func_80062AD8(handle, &type);
            data = (void*)type;
        }

        if (data == 0) {
            allLoaded = 0;
        }
    }

    if (data0 == 0 || !allLoaded) return;

    // Create model
    void* model = func_80495E8C(lbl_eu_80663E14, data0, -1, 1);
    FLD(u32, s, 0x20) = (u32)model;

    func_80484E5C(model);
    func_804827DC(model, 1);
    func_80485684(model, 1);
    FLD(u32, model, 0x7A8) |= 0x4;

    // Set textures
    for (i = 1; i <= 5; i++) {
        void* handle = FLD(void*, s, 0x3C + i * 4);
        if (handle == 0) continue;

        void* texData = FLD(void*, s, 0x6C + i * 4);
        if (texData == 0) {
            u32 type;
            func_80062AD8(handle, &type);
            texData = (void*)type;
        }

        if (i == 5) {
            func_80483448(model, texData);
        } else {
            func_804831C4(model, texData);
        }
    }

    // Call virtual functions on model
    {
        void* vtable = FLD(void*, model, 0x00);
        void (*vfunc1)(void*, int) = (void (*)(void*, int))FLD(u32, vtable, 0x64);
        vfunc1(model, 1);
        void (*vfunc2)(void*, int, int) = (void (*)(void*, int, int))FLD(u32, vtable, 0x9C);
        vfunc2(model, 0, 0);
    }

    FLD(u8, s, 0xAC) = 1;
    FLD(u32, s, 0x18) |= 0x800;
}

// ============================================================================
// func_801848EC (us-80185d2c) - Dispatch function
// this(r3), r4, r5
// ============================================================================
extern "C" void func_801848EC(void* self, int r4, int r5) {
    char* s = (char*)self;

    if (FLD(void*, s, 0x3C) == 0) return;

    if (r4 != 0 && FLD(u8, s, 0xAC) == 0) {
        func_80184730(self);
    }

    if (FLD(u8, s, 0xAC) != 0) {
        func_801729F0(self, r4, r5);
    }
}

// ============================================================================
// func_8018496C (us-80185dac) - Clear flag bit (already implemented)
// ============================================================================

// ============================================================================
// func_8018497C (us-80185dbc) - Check if file is already loaded
// ============================================================================
extern "C" int func_8018497C(void* self) {
    char* s = (char*)self;

    u32 entryId, param1, param2, param3;
    func_800AA318(FLD(u32, FLD(void*, s, 0x1C), 0x20), &entryId, &param1, &param2, &param3);

    // Walk the circular list of objects; the condition re-fetches the head.
    void* obj = FLD(void*, FLD(void*, func_80086B04__Q22cf13CfGameManagerFv(), 0x04), 0x00);
    while (obj != FLD(void*, func_80086B04__Q22cf13CfGameManagerFv(), 0x04)) {
        u32 objEntryId, objParam1, objParam2, objParam3;
        func_800AA318(FLD(u32, FLD(void*, obj, 0x08), 0x70), &objEntryId, &objParam1, &objParam2, &objParam3);

        // Already loaded if a live character object matches this request.
        if (param1 == objParam1 && param2 == 1) {
            return 1;
        }
        obj = FLD(void*, obj, 0x00);
    }

    return 6;
}

// ============================================================================
// func_80184A24 (us-80185e64) - OnFileEvent handler
// this(r3), event(r4)
// ============================================================================
extern "C" int func_80184A24(void* self, void* event) {
    char* s = (char*)self;

    // For each of the 6 file slots, if the completed request matches the
    // event's handle, record the loaded data (or drop the handle on error).
    for (int i = 0; i < 6; i++) {
        void* eventHandle = FLD(void*, event, 0x04);
        if (FLD(void*, s, 0x54 + i * 4) == eventHandle) {
            if (FLD(u32, event, 0x00) == 1) {
                u32 nextData = FLD(u32, eventHandle, 0x04);
                FLD(u32, eventHandle, 0x04) = 0;
                FLD(u32, s, 0x6C + i * 4) = nextData;
            } else {
                FLD(u32, s, 0x3C + i * 4) = 0;
            }
            FLD(u32, s, 0x54 + i * 4) = 0;
        }
    }

    return 0;
}

// ============================================================================
// OnFileEvent__12CREvtModelPcFP10CEventFile - IWorkEvent override
// ============================================================================
extern "C" void OnFileEvent__12CREvtModelPcFP10CEventFile(void* self) {
    func_80184A24((char*)self - 0x38, self);
}

// ============================================================================
// func_80184B0C - Thunk to __ct__8018385C
// ============================================================================
extern "C" void func_80184B0C(void* self) {
    __ct__8018385C((char*)self - 0x38, 0);
}