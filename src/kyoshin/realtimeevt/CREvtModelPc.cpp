// TU: kyoshin/realtimeevt/CREvtModelPc
// Class: CREvtModelPc (inherits CREvtModel + IWorkEvent)

#include "kyoshin/harness_catalog.hpp"
#include "kyoshin/realtimeevt/CREvtModelPc.hpp"
#include "kyoshin/realtimeevt/CREvtModelMap.hpp"

#include <cstring>
#include "kyoshin/cf/CfGameManagerData.hpp"  // H3 label-owner decl (lbl_eu_80663E14; lbl_eu_80663E24)

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
    extern int func_8016BDA8(void* self, void* pId);
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
extern "C" __declspec(noinline) void* __ct__8018385C(void* self, int flag) {
    if (self != 0) {
        char* s = (char*)self;

        // Install vtables in retail emission order (base first), then invoke
        // slot-15 entry straight out of the vtable data symbol.
        FLD(void*, s, 0x00) = lbl_eu_805321F0;
        FLD(void*, s, 0x38) = (char*)lbl_eu_805321F0 + 0x44;
        ((void (*)(void*))lbl_eu_805321F0[0x3C / 4])(s);

        __ct__80172668(self, 0);

        if (flag > 0) {
            __dt__80185754(self);
        }
    }

    return self;
}

// ============================================================================
// func_801838D8 (us-80184cf4) - Check if ready
// ============================================================================
extern "C" int func_801838D8(void* self) {
    char* s = (char*)self;
    int result;

    // Ready only if the task counter matches the current one.
    if (FLD(s32, s, 0xB0) != func_8016A3C4() + 1) {
        return 0;
    }

    void* parent = FLD(void*, s, 0x1C);

    // If the parent's scene has more than one chunk, only consider the event
    // ready when the callback is one of the two accepted ptmf constants.
    if (FLD(u32, parent, 0x30) <= 1) {
        result = 0;
    } else {
        result = 0;
        if (__ptmf_cmpr(s + 0x08, lbl_eu_80532198) == 0 ||
            __ptmf_cmpr(s + 0x08, lbl_eu_805321A4) == 0) {
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
void func_80183A3C(void* self) {
    CREvtModelPc& pc = *(CREvtModelPc*)self;

    // Parent scene counter check: retail tests (u32)(mField30 + 0x10000)
    // against 0xFFFF (the addis/cmplwi lowering of == -1).
    if ((u32)(((CREvtModelParentIf*)pc.mParent)->mField30 + 0x10000) == 0xFFFF) {
        pc.mFlags |= 0x50;

        // Post-increment derefs fold the first access into `lwzu` and force
        // the retail load/store order w0,w1 -> +0xC,+0x8,+0x10.
        const u32* src = lbl_eu_805321B0;
        u32 w0 = *src++;
        u32 w1 = *src++;
        pc.mCallback[1] = w1;
        pc.mCallback[0] = w0;
        pc.mCallback[2] = *src;
        return;
    }

    // If any slot is still loading (status == 1), bail out. Statuses are
    // compared signed; retail addresses slots 3-5 via a this+8 base.
    // If any slot is still loading (status == 1), bail out. Statuses are
    // compared signed; retail addresses slots 3-5 off a this+8 base.
    if ((int)pc.mStatus[0] == 1 || (int)pc.mStatus[1] == 1 || (int)pc.mStatus[2] == 1)
        return;
    int* tail = (int*)pc.mCallback;   // this + 8
    if (tail[34] == 1 || tail[35] == 1 || tail[36] == 1)
        return;

    if (pc.mFlags & 0x100) return;
    pc.mFlags |= 0x100;

    func_80172768(self);

    // Free any loaded/archived slot data. Status 3 = loaded into MEM2 via
    // MemManager, status 2 = archived. Retail carries a duplicated null
    // check in the status-3 path (two consecutive beq), reproduced by the
    // nested if (same shape as func_80183978).
    u32 zero = 0;
    for (int i = 0; i < 6; i++) {
        int status = (int)pc.mStatus[i];
        if (status == 3) {
            void* data = pc.mData[i];
            if (data != 0) {
                if (data != 0) {
                    mtl::MemManager::deallocate(data);
                    pc.mData[i] = (void*)zero;
                }
            }
        } else if (status == 2) {
            void* data = pc.mData[i];
            if (data != 0) {
                func_800A9344(data, 0);
            }
        }
    }

    // Install the reset ptmf callback (loads w0,w1 via *src++ then stores
    // +0xC,+0x8; w2 late) and clear flags bits 0x40/0x01 while setting 0x30.
    const u32* src = lbl_eu_805321BC;
    u32 w0 = *src++;
    u32 w1 = *src++;
    u32 flags = pc.mFlags | 0x30;
    pc.mCallback[1] = w1;
    flags &= ~0x41;
    pc.mCallback[0] = w0;
    pc.mCallback[2] = *src;
    pc.mFlags = flags;

    // Reinitialize all six slots (four arrays cleared per iteration).
    for (int i = 0; i < 6; i++) {
        pc.mFileHandle[i] = 0;
        pc.mFileReq[i] = 0;
        pc.mData[i] = 0;
        pc.mStatus[i] = 0;
    }
    pc.mLoaded = 0;
}

// ============================================================================
// func_80183C1C (us-80185038) - Init counter
// ============================================================================
extern "C" void func_80183C1C(void* self) {
    char* s = (char*)self;

    FLD(s32, s, 0xB0) = -1;

    int result = func_8016BDA8(self, s + 0xB0);
    if (result != 0) {
        if (FLD(u32, s, 0x18) & 0x100) {
            *(volatile u32*)(s + 0x18) &= ~0x100;
        }

        u32 v0;
        u32* p = (u32*)lbl_eu_805321C8;
        v0 = p[0];
        FLD(u32, s, 0x0C) = p[1];
        FLD(u32, s, 0x08) = v0;
        FLD(u32, s, 0x10) = p[2];
    }
}

// ============================================================================
// func_80183C90 (us-801850ac) - Main file loading function
// ============================================================================
extern "C" void func_80183C90(void* self) {
    CREvtModelPc& pc = *(CREvtModelPc*)self;
    char* s = (char*)self;

    // Decoded fields of the parent's packed token:
    //   decA: top field (<<27 when rebuilding a token)
    //   decB: character/id field (compared against 3/8, passed to getters)
    //   decC: slot flag (== 1 tests)
    //   decD: special id (== 9 gates the extra slot-5 load)
    u32 decA, decB, decC, decD;
    u32 objA, objB, objC, objD;
    int typeOut;              // func_80062AD8 out-type (-1 sentinel)
    void* gameMgr;
    void* obj;
    void* objList;
    void* objPtr;
    void* matchChr = 0;
    int isNewFile = 0;
    int hasAllHandles = 0;
    int skipLoading = 0;
    int slotIdx0, slotIdx1, slotIdx2;
    void* parent;
    u32 handle;
    u32 fileSize;
    void* alloc;
    int ok;
    const char* pathName;
    CFileHandle* reqHandle;
    void* memHandle;
    void* h;
    void* h2;
    int i;
    char nameBuf[0x20];
    int nameLen;
    char fix3[0x44];
    char fix2[0x44];
    char fix1[0x44];

    pc.mFlags |= 0x40;

    // Decode the parent scene's packed token.
    parent = FLD(void*, s, 0x1C);
    func_800AA318(FLD(u32, parent, 0x20), &decA, &decB, &decC, &decD);

    // If the request is for character 3 or 8, check whether such a character
    // is already being loaded by another model object.
    if (decB == 3 || decB == 8) {
        isNewFile = (func_8016C300(self) != 0) ? 1 : 0;
        if (isNewFile == 0) {
            gameMgr = func_80086B04__Q22cf13CfGameManagerFv();
            objList = FLD(void*, gameMgr, 0x04);
            obj = FLD(void*, objList, 0x00);
            while (obj != objList) {
                objPtr = FLD(void*, obj, 0x08);
                if (objPtr != 0) {
                    objPtr = (char*)objPtr - 0x3E9C;
                }
                if ((FLD(u16, objPtr, 0x3F28) == 8 && decB == 3) ||
                    (FLD(u16, objPtr, 0x3F28) == 3 && decB == 8)) {
                    isNewFile = 1;
                    goto handles_done;
                }
                obj = FLD(void*, obj, 0x00);
            }
        }
    }

    // Find a live character whose request matches ours (matchChr).
    if (decB <= 10) {
        gameMgr = func_80086B04__Q22cf13CfGameManagerFv();
        objList = FLD(void*, gameMgr, 0x04);
        obj = FLD(void*, objList, 0x00);
        while (obj != objList) {
            objPtr = FLD(void*, obj, 0x08);
            func_800AA318(FLD(u32, objPtr, 0x70), &objA, &objB, &objC, &objD);
            if (decB == objB && decC == 1) {
                matchChr = FLD(void*, obj, 0x08);
                if (matchChr != 0) {
                    matchChr = (char*)matchChr - 0x3E9C;
                }
                goto handles_done;
            }
            obj = FLD(void*, obj, 0x00);
        }
    }

handles_done:
    // First choice: reuse handles registered on the parent resource.
    hasAllHandles = 0;
    parent = FLD(void*, s, 0x1C);
    if (FLD(s8, parent, 0x48) != 0 && !func_8016840C()) {
        for (i = 1; i <= 5; i++) {
            handle = (u32)func_80164724((char*)parent + 0x48, decB, i);
            FLD(u32, s, 0x38 + i * 4) = handle;
        }
        if (FLD(u32, s, 0x3C) != 0 && FLD(u32, s, 0x40) != 0 &&
            FLD(u32, s, 0x44) != 0 && FLD(u32, s, 0x48) != 0 &&
            FLD(u32, s, 0x4C) != 0) {
            hasAllHandles = 1;
        }
    }

    skipLoading = 0;
    if (!hasAllHandles) {
        if (matchChr != 0 && !func_8016840C()) {
            // Pull handles out of the live character via its getter virtual.
            for (i = 1; i <= 5; i++) {
                objPtr = (char*)matchChr + 0x3E9C;
                void* vtable = FLD(void*, objPtr, 0x00);
                void* (*getHandle)(void*, int) = (void* (*)(void*, int))FLD(u32, vtable, 0x148);
                handle = (u32)getHandle(objPtr, i);
                FLD(u32, s, 0x38 + i * 4) = handle;
            }
        } else if (decC == 1 && !func_8016840C()) {
            // Already-loading shortcut through the game manager cache.
            if (isNewFile) {
                func_8007E038__Q22cf13CfGameManagerFv(decB, 0);
                skipLoading = 1;
            }
            for (i = 0; i < 5; i++) {
                handle = (u32)func_8007DE94__Q22cf13CfGameManagerFv(decB, i);
                FLD(u32, s, 0x3C + i * 4) = handle;
            }
        } else {
            // Build the five packed tokens from scratch.
            slotIdx0 = slotIdx1 = slotIdx2 = decC;
            if (func_8016840C()) {
                slotIdx0 = FLD(u32, lbl_eu_805037A8, decB * 4);
            }
            if (decB == 8 && slotIdx0 == 1) {
                slotIdx0 = slotIdx1 = slotIdx2 = 4;
            } else if (func_8016840C()) {
                // Co-op mode remaps some slot indices per character id.
                if (decB == 1 && slotIdx0 == 2) slotIdx1 = 1;
                if (decB == 2 && slotIdx0 == 3) slotIdx1 = 1;
                if (decB == 5 && slotIdx0 == 2) slotIdx1 = 1;
                if (decB == 6 && slotIdx0 == 3) slotIdx2 = 1;
            }
            {
                u32 base = (decA << 27) | (decB << 20);
                FLD(u32, s, 0x3C) = base | (slotIdx1 << 10) | (slotIdx2 << 10) | 1;
                FLD(u32, s, 0x40) = base | (slotIdx0 << 10) | 2;
                FLD(u32, s, 0x44) = base | (slotIdx2 << 10) | 3;
                FLD(u32, s, 0x48) = base | (slotIdx0 << 10) | 4;
                FLD(u32, s, 0x4C) = base | (slotIdx0 << 10) | 5;
            }
        }
    }

    if (!skipLoading) {
        // Load the five files named by the slot tokens.
        u32 status1 = 1, status2 = 2, status3 = 3;
        u32 neg1 = -1;
        char* walk = s;
        for (i = 0; i <= 4; i++, walk += 4) {
            handle = FLD(u32, walk, 0x3C);
            if (handle == 0) continue;

            pathName = func_800AA5C0((void*)handle);
            nameLen = strlen(pathName);
            strcpy(nameBuf, pathName);
            func_80062AD8((void*)handle, (u32*)&typeOut);
            if (isNewFile) {
                typeOut = (int)neg1;
            }
            if (typeOut != -1) continue;

            // Build the file path, then try MEM1 -> archive -> streamed MEM2.
            FLD(u8, fix1, 0x00) = 0;
            FLD(u32, fix1, 0x40) = 0;
            func_800AA33C(*(ml::FixStr<64>*)fix1, handle, 1, 1);
            fileSize = CDeviceFile::getFileSize(fix1);
            alloc = func_80167F6C((void*)fileSize, 0x20, 0);
            if (alloc != 0) {
                FLD(u32, walk, 0x84) = status1;
                ok = (int)alloc;
            } else {
                if (func_800A8E6C(fileSize, 1) != 0) {
                    ok = func_800A8E6C(fileSize, 0);
                } else {
                    ok = 0;
                }
                if (ok != 0) {
                    FLD(u32, walk, 0x84) = status2;
                }
            }

            if (ok != 0) {
                // Archive-backed read.
                reqHandle = (CFileHandle*)CDeviceFile::readCommonArchiveFile(
                    (mtl::ALLOC_HANDLE)ok, fix1,
                    (IWorkEvent*)((s != 0) ? s + 0x38 : s), 0, 0);
                FLD(u32, walk, 0x54) = (u32)reqHandle;
            } else {
                // Streamed read into MEM2.
                memHandle = (void*)mtl::MemManager::getHandleMEM2();
                h = func_80495FF0(lbl_eu_80663E14);
                h2 = h;
                parent = FLD(void*, s, 0x1C);
                if (FLD(u32, parent, 0x58) & 0x2) {
                    h = func_80495FF0(lbl_eu_80663E14);
                    memHandle = h;
                    h2 = (void*)mtl::MemManager::getHandleMEM2();
                }
                reqHandle = (CFileHandle*)CDeviceFile::readFile(
                    (mtl::ALLOC_HANDLE)memHandle, fix1,
                    (IWorkEvent*)((s != 0) ? s + 0x38 : s), 0, 0);
                FLD(u32, walk, 0x54) = (u32)reqHandle;
                func_8044F400__11CDeviceFileFP11CFileHandleUl(reqHandle, (u32)h2);
                parent = FLD(void*, s, 0x1C);
                if (FLD(u32, parent, 0x58) & 0x1) {
                    CDeviceFile::setHandleFlag1(reqHandle);
                }
                if (func_801683FC()) {
                    CDeviceFile::setHandleFlag2(reqHandle);
                }
                FLD(u32, walk, 0x84) = status3;
            }
        }
    }

    // Slot 5 loads only for special id 9.
    if (decD == 9) {
        handle = (u32)func_800AA2E8(decB, 1, 0);
        FLD(u32, s, 0x50) = handle;
        FLD(u8, fix3, 0x00) = 0;
        FLD(u32, fix3, 0x40) = 0;
        func_800AA33C(*(ml::FixStr<64>*)fix3, handle, 1, 1);
        fileSize = CDeviceFile::getFileSize(fix3);
        alloc = func_80167F6C((void*)fileSize, 0x20, 0);
        if (alloc != 0) {
            FLD(u32, s, 0x98) = 1;
            ok = (int)alloc;
        } else {
            ok = func_800A8E6C(fileSize, 0);
            if (ok != 0) {
                FLD(u32, s, 0x98) = 2;
            }
        }

        if (ok != 0) {
            reqHandle = (CFileHandle*)CDeviceFile::readCommonArchiveFile(
                (mtl::ALLOC_HANDLE)ok, fix3,
                (IWorkEvent*)((s != 0) ? s + 0x38 : s), 0, 0);
            FLD(u32, s, 0x68) = (u32)reqHandle;
        } else {
            memHandle = (void*)mtl::MemManager::getHandleMEM2();
            h = func_80495FF0(lbl_eu_80663E14);
            h2 = h;
            parent = FLD(void*, s, 0x1C);
            if (FLD(u32, parent, 0x58) & 0x2) {
                h = func_80495FF0(lbl_eu_80663E14);
                memHandle = h;
                h2 = (void*)mtl::MemManager::getHandleMEM2();
            }
            reqHandle = (CFileHandle*)CDeviceFile::readFile(
                (mtl::ALLOC_HANDLE)memHandle, fix3,
                (IWorkEvent*)((s != 0) ? s + 0x38 : s), 0, 0);
            FLD(u32, s, 0x68) = (u32)reqHandle;
            func_8044F400__11CDeviceFileFP11CFileHandleUl(reqHandle, (u32)h2);
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
    // Install the post-load callback ptmf.
    {
        const u32* src = lbl_eu_805321D4;
        u32 w0 = *src++;
        u32 w1 = *src++;
        pc.mCallback[1] = w1;
        pc.mCallback[0] = w0;
        pc.mCallback[2] = *src;
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

    if (FLD(void*, s, 0x20) != 0) {
        int time = func_8016A35C();
        float ft = (float)time;
        func_80484F80(FLD(void*, s, 0x20), ft);
    }

    if (FLD(u32, s, 0x18) & 0x800) {
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

    func_80484E5C(model, lbl_eu_80667918);
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
extern "C" int func_80184A24(void* self, CEventFile* ev) {
    CREvtModelPc* m = (CREvtModelPc*)self;

    // For each of the 6 file slots, if the completed request matches the
    // event's handle, record the loaded data (or drop the handle on error).
    for (int i = 0; i < 6; i++) {
        if (m->mFileReq[i] == ev->mFileHandle) {
            if (ev->unk0 == 1) {
                m->mData[i] = ev->mFileHandle->getData();
            } else {
                m->mFileHandle[i] = NULL;
            }
            m->mFileReq[i] = NULL;
        }
    }

    return 0;
}

// ============================================================================
// OnFileEvent__12CREvtModelPcFP10CEventFile - IWorkEvent override
// ============================================================================
extern "C" void OnFileEvent__12CREvtModelPcFP10CEventFile(void* self) {
    ((void(*)(void*))func_80184A24)((char*)self - 0x38);
}

// ============================================================================
// func_80184B0C - Thunk to __ct__8018385C
// ============================================================================
extern "C" void func_80184B0C(void* self, int flags) {
    __ct__8018385C((char*)self - 0x38, flags);
}