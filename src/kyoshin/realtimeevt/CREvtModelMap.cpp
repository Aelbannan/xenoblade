// Decompiled CREvtModelMap - realtime event model map
// High-level C++ reconstruction.

#include <string.h>
#include "kyoshin/harness_catalog.hpp"
#include "kyoshin/realtimeevt/CREvtModelMap.hpp"
#include "kyoshin/realtimeevt/CREvtLight.hpp"
#include "monolib/device/CDeviceFile.hpp"
#include "monolib/util/MemManager.hpp"
#include "include/kyoshin/cf/CfGameManager.hpp"

// ---------------------------------------------------------------------------
// Action table entries - {startTime, creatureId, callback}
// ---------------------------------------------------------------------------
struct ActionEntry {
    s32 startTime;
    s32 creatureId;
    void* callback;
};

extern "C" {
    extern ActionEntry lbl_eu_80531D20[];  // 1 entry: {0, -1, loadFiles}
    extern ActionEntry lbl_eu_80531D2C[];  // 1 entry: {0, -1, loadCreature}
    extern u32 lbl_eu_80531D38[];  // 4 action-table entries (12 bytes each; loadCreature reads the first as u32 words)
    extern ActionEntry lbl_eu_80531D68[];  // 1 entry: {0, -1, updatePosition}
    extern ActionEntry lbl_eu_80531D74[];  // 1 entry: {0, -1, updatePosition}
    extern const char lbl_eu_8050372C[];   // path strings: ".lod\0/obj/\0.map\0.lgt"

    // Forward declarations for internal helpers
    void func_800AA33C(ml::FixStr<64>& buf, u32 packed, int prefixFlag, int suffixFlag);
    bool func_80181988(CREvtModelMap* self, CEventFile* ev); // handleFileEvent (retail C-linkage name)
    void func_8016BC1C(CREvtModelMap* self);
    bool func_8016BDA8(void* self, s32* pId);
    u32 func_8016A35C();
    u32 func_8016846C();
    void func_80168514(CREvtModelMap* self);
    u32 func_80180960();
    s32 func_80180978();
    s32 func_80180990();
    void func_8016FC0C(int val);
    void func_80462A08__8CTaskLODFv(int val);
    void func_80462B30__8CTaskLODFv();
    void func_80462B68__8CTaskLODFv();
    void func_80462B4C__8CTaskLODFv();
    extern mtl::ALLOC_HANDLE func_80495FF0(void* self); // retail: virtual at vtable[0x2C] of *(lbl_eu_80663E14)
    void func_80495E60(void* pEmote);
    void* func_80495E8C(void* a, void* b, int c, int d);
    void func_804838DC(void* pEmote, int r4);
    void func_80484E5C(void* pEmote, float scale);
    void func_80484F80(void* pEmote, float time);
    void func_804C1D7C(void* pData);
    void* func_804C1BA0(void* pData, void* pFile, int r5);
    mtl::ALLOC_HANDLE func_80490098__Fv();
    void* getGlobalSda();
    void __dt__80185754(CREvtModelMap* self);
    void __ct__80172668(CREvtModelMap* self, int dealloc);
    void func_8016A354();
    void func_801729F0();
    void func_80172768(CREvtModelMap* self, int r4);
    void func_801727DC();
    void func_801728F8();
    void func_8017298C();
    void func_80169F24();
    void func_801731A0();
    void func_80173194();
    void func_801731AC();
    void func_80172CC0();
    void func_80172CC4();
    void func_801726DC(CREvtModelMap* self);
    void func_80172668();
    void func_80181A54(void* self, int dealloc);
    void WorkEvent1__10IWorkEventFPvPCc();
    bool WorkEvent3__10IWorkEventFPv();
    bool WorkEvent4__10IWorkEventFv();
    void OnPauseTrigger__10IWorkEventFb();
    bool WorkEvent6__10IWorkEventFv();
    bool WorkEvent7__10IWorkEventFv();
    bool WorkEvent8__10IWorkEventFv();
    bool WorkEvent9__10IWorkEventFv();
    bool WorkEvent10__10IWorkEventFv();
    bool WorkEvent11__10IWorkEventFv();
    bool WorkEvent12__10IWorkEventFv();
    bool WorkEvent13__10IWorkEventFv();
    bool WorkEvent14__10IWorkEventFv();
    bool WorkEvent15__10IWorkEventFv();
    bool WorkEvent16__10IWorkEventFv();
    bool WorkEvent17__10IWorkEventFv();
    bool WorkEvent18__10IWorkEventFv();
    bool WorkEvent19__10IWorkEventFv();
    bool WorkEvent20__10IWorkEventFv();
    bool WorkEvent21__10IWorkEventFv();
    bool WorkEvent22__10IWorkEventFv();
    bool WorkEvent23__10IWorkEventFv();
    bool WorkEvent24__10IWorkEventFv();
    bool WorkEvent25__10IWorkEventFv();
    bool WorkEvent26__10IWorkEventFv();
    bool WorkEvent27__10IWorkEventFv();
    bool WorkEvent28__10IWorkEventFv();
    bool WorkEvent29__10IWorkEventFv();
    bool WorkEvent30__10IWorkEventFv();
    void WorkEvent31__10IWorkEventFv();

    // Global pointers
    extern CREvtModelMap* lbl_eu_806642B0;  // current active model map
    extern CREvtModelMap* lbl_eu_806642B4;  // current visible model map
    extern void* lbl_eu_80663E14;            // CDeviceFile instance
    extern float lbl_eu_806678C0;            // 1.0f scale
    extern double lbl_eu_806678C8;           // 0x43300000_80000000 double for int→float conv
extern "C" void* func_80086B10__Q22cf13CfGameManagerFv();
extern "C" void* func_800828DC__Q22cf13CfGameManagerFv();
extern "C" void func_8044F400__11CDeviceFileFP11CFileHandleUl(CFileHandle*, u32);
extern "C" void __ct__CREvtModel(void* self, void* pData, int pArg); // retail base ctor name
extern "C" void func_800AA318(u32 packed, u32* out0, u32* out1, u32* out2, u32* out3);
}

// ---------------------------------------------------------------------------
// 1. __ct__CREvtModelMap - constructor (0x80181DB4)
// ---------------------------------------------------------------------------
// Retail C-linkage ctor: takes (self, parent); the parent (a CREvtModel) is
// forwarded to the base ctor as mPtr1C and its packed token (parent+0x20)
// selects the guest vs non-guest action table.
extern "C" CREvtModelMap* __ct__CREvtModelMap(CREvtModelMap* self, void* parent)
{
    // The retail ctor calls the base CREvtModel ctor first, then installs
    // the vtables. The flattened class has no compiler-generated vtable, so
    // both are done explicitly here (lbl_eu_80531D80 main vtable, +0x44 for
    // the IWorkEvent subobject).
    __ct__CREvtModel(self, parent, 1);

    self->vtable = (void*)lbl_eu_80531D80;
    self->vtableIWork = (u8*)lbl_eu_80531D80 + 0x44;

    self->mVisible = 1;
    self->mIsGuest = 0;

    self->mFileHandle1 = 0;
    self->mFileData1 = 0;
    self->mFileHandle2 = 0;
    self->mFileData2 = 0;

    self->mBasePath[0] = 0;
    self->mBasePathLen = 0;
    self->mFileHandle3 = 0;
    self->mFileData3 = 0;
    self->mPad9C = 0;

    self->mUnkDC = 0;
    self->mLoadedModelData = 0;
    self->mEmoteModel = 0;
    self->mCreatureCount = 0;
    self->mCreatureId = -1;

    memset(self->mModelName, 0, sizeof(self->mModelName));

    // Unpack the parent's packed token; retail derefs mPtr1C unconditionally
    // (the base ctor always stored the parent there).
    u32 out0, out1, out2, out3;
    func_800AA318(((CREvtModelParentIf*)self->mPtr1C)->mPacked, &out0, &out1, &out2, &out3);

    if (out0 == 1) {
        // Guest model: install the guest action table and publish as visible.
        self->mIsGuest = 1;

        // w0/w1/w2 locals with the store order +0x0C, +0x08, +0x10 reproduce
        // retail's lwzu load schedule (same shape as func_80180E60).
        u32 w1, w2, w0;
        u32* src = (u32*)lbl_eu_80531D20;
        w0 = *src++;
        w1 = *src++;
        self->mCallback[1] = w1;
        self->mCallback[0] = w0;
        w2 = *src++;
        self->mCallback[2] = w2;

        self->mFlags |= 0x10;
        lbl_eu_806642B4 = self;
    } else {
        // Non-guest: install the default action table, then request loading.
        u32 w1, w2, w0;
        u32* src = (u32*)lbl_eu_80531D2C;
        w0 = *src++;
        w1 = *src++;
        self->mCallback[1] = w1;
        self->mCallback[0] = w0;
        w2 = *src++;
        self->mCallback[2] = w2;

        func_8016BC1C(self);

        if (((CREvtModelParentIf*)self->mPtr1C)->mField30 != 0) {
            self->mFlags |= 0x30;
        }
    }

    return self;
}

// ---------------------------------------------------------------------------
// 2. __ct__80180B00 - destructor / reset constructor (0x80181EFC)
// Called as both the primary destructor and a reset.
// ---------------------------------------------------------------------------
extern "C" CREvtModelMap* __ct__80180B00(CREvtModelMap* self, int dealloc)
{
    if (self) {
        // Restore vtable and IWorkEvent vtable
        self->vtable = (void*)lbl_eu_80531D80;
        self->vtableIWork = (u8*)lbl_eu_80531D80 + 0x44;

        if (self->mIsGuest) {
            // Hide the model map, then tell the game-manager core to leave
            // guest mode (retail dispatches vtable+0x30 with a 0 arg).
            ((CREvtMapVtIf*)self)->v30(0);

            // Retail re-fetches the singleton for the virtual call (MWCC
            // does not CSE calls).
            if (cf::CfGameManager::func_80083298()) {
                ((CGameMgrCoreIf*)cf::CfGameManager::func_80083298())->v158(1);
            }

            func_80462B30__8CTaskLODFv();

            if (self->mCreatureCount > 0) {
                // Hide every tracked creature (circular list; the head is
                // re-fetched each iteration because MWCC does not CSE the call).
                s32 i = 0;
                CCreatureNode* head = (CCreatureNode*)((cf::CfGameManager*)func_80086B10__Q22cf13CfGameManagerFv())->field_0x4;
                CCreatureNode* it = head->next;
                while (it != (CCreatureNode*)((cf::CfGameManager*)func_80086B10__Q22cf13CfGameManagerFv())->field_0x4) {
                    u8 v = (u8)self->mModelName[i];
                    it->obj->v158(!v);
                    it = it->next;
                    i++;
                }
            }

            // Clear global pointer
            lbl_eu_806642B0 = 0;

            if (func_800828DC__Q22cf13CfGameManagerFv()) {
                func_800828DC__Q22cf13CfGameManagerFv();
                func_8016FC0C(1);
            }
        }

        // Cleanup virtual, then base-class teardown
        ((CREvtMapVtIf*)self)->v3C();
        func_80462B68__8CTaskLODFv();
        __ct__80172668(self, 0);

        if (dealloc > 0) {
            __dt__80185754(self);
        }
    }

    return self;
}

// ---------------------------------------------------------------------------
// 3. func_80180C60 - isMapModel (0x8018205C)
// ---------------------------------------------------------------------------
// retail func_80180C60: (field_1C->b10 == 'm' && b11 == 'a')
extern "C" int func_80180C60(void* self) {
    s8* info = *(s8**)((char*)self + 0x1C);
    return (info[0x10] == 0x6D && info[0x11] == 0x61) ? 1 : 0;
}

// ---------------------------------------------------------------------------
// 4. func_80180C88 - isBusy (0x80182084)
// ---------------------------------------------------------------------------
extern "C" bool func_80180C88(CREvtModelMap* self)
{
    return self->mFileHandle1 || self->mFileHandle2 || self->mFileHandle3;
}

// ---------------------------------------------------------------------------
// 5. func_80180CBC - reset (0x801820B8)
// ---------------------------------------------------------------------------
extern "C" bool func_80180CBC(CREvtModelMap* self)
{
    if (self->mFlags & 0x80) {  // bit 24: already busy/loaded
        return 0;
    }

    // Call base-class reset
    func_801726DC(self);

    // Free emote model.
    if (self->mEmoteModel) {
        func_80495E60(self->mEmoteModel);
        self->mEmoteModel = nullptr;
    }

    // Free loaded model data
    void* loaded = self->mLoadedModelData;
    if (loaded) {
        func_804C1D7C(*(void**)((u8*)lbl_eu_80663E14 + 0x7C));
        self->mLoadedModelData = nullptr;
    }

    // Free file data buffers.  The nested if (checked against a local) makes
    // MWCC emit the retail double-beq (same shape as the matched
    // func_80183978 in CREvtModelPc).
    void* p1 = self->mFileData1;
    if (p1) {
        if (p1) {
            mtl::MemManager::deallocate(p1);
            self->mFileData1 = 0;
        }
    }
    void* p2 = self->mFileData2;
    if (p2) {
        if (p2) {
            mtl::MemManager::deallocate(p2);
            self->mFileData2 = 0;
        }
    }
    void* p3 = self->mFileData3;
    if (p3) {
        if (p3) {
            mtl::MemManager::deallocate(p3);
            self->mFileData3 = 0;
        }
    }

    // Cancel file handles
    if (self->mFileHandle1) {
        CDeviceFile::cancel(self->mFileHandle1);
    }
    if (self->mFileHandle2) {
        CDeviceFile::cancel(self->mFileHandle2);
    }
    if (self->mFileHandle3) {
        CDeviceFile::cancel(self->mFileHandle3);
    }

    // Clear file handles
    self->mFileHandle1 = 0;
    self->mFileHandle2 = 0;
    self->mFileHandle3 = 0;

    return 1;
}

// ---------------------------------------------------------------------------
// 6. func_80180DCC - setGuestModeOff (0x801821C8)
// ---------------------------------------------------------------------------
// Turns guest mode off on the game-manager core. If a guest model map is
// already active (lbl_eu_806642B0) do nothing; otherwise tell the core
// object (vtable+0x158) to disable guest mode and resume LOD tasks. Retail
// re-fetches the core for the virtual call (MWCC does not CSE calls).
extern "C" void func_80180DCC(CREvtModelMap* self)
{
    if (lbl_eu_806642B0) {
        return;
    }

    if (cf::CfGameManager::func_80083298()) {
        cf::CfGameManager* mgr = (cf::CfGameManager*)cf::CfGameManager::func_80083298();
        // virtual call: core->vtable[0x158](0) - real dispatch (r12 ABI)
        ((CGameMgrCoreIf*)mgr)->v158(0);
        func_80462B4C__8CTaskLODFv();
    }
}

// ---------------------------------------------------------------------------
// 7. func_80180E1C - setGuestModeOn (0x80182218)
// ---------------------------------------------------------------------------
extern "C" void func_80180E1C(CREvtModelMap* self)
{
    if (lbl_eu_806642B0) {
        return;
    }

    cf::CfGameManager* mgr = (cf::CfGameManager*)cf::CfGameManager::func_80083298();
    // Virtual call CfGameManager::setGuestMode(1) (vtable slot 0x158);
    // retail does NOT null-check the singleton result.
    ((CGameMgrCoreIf*)mgr)->v158(1);
    func_80462B30__8CTaskLODFv();
}

// ---------------------------------------------------------------------------
// 8. func_80180E60 - loadCreature (0x8018225C)
// ---------------------------------------------------------------------------
// Resets the creature id and asks the character manager for a new one. On
// success the ptmf triplet from the action table at 0x80531D38 is installed
// into +0x08 (cf::CREvtObj::mCallback storage).
extern "C" void func_80180E60(CREvtModelMap* self)
{
    s32* pId = &self->mCreatureId;
    *pId = -1;

    if (func_8016BDA8(self, pId)) {
        // w1/w2/w0 locals DECLARED BEFORE the src pointer: the w-locals' VRs
        // are born first, so the allocator colors w0->r3 and src->r4 like
        // retail (src-first declaration swaps the colors — 5 reg_swap). The
        // store order +0x0C, +0x08, +0x10 reproduces retail's lwzu schedule.
        u32 w1, w2, w0;
        u32* src = (u32*)lbl_eu_80531D38;
        w0 = *src++;
        w1 = *src++;
        self->mCallback[1] = w1;
        self->mCallback[0] = w0;
        w2 = *src++;
        self->mCallback[2] = w2;
    }
}

// ---------------------------------------------------------------------------
// 9. func_80180EBC - loadFiles (0x801822B8)
// ---------------------------------------------------------------------------
// Loads the model files. Guest models publish as active and build the archive
// base path; non-guest models load .map/.lod/.lgt (path built with two
// ml::FixStr<32> locals: filePath at 0x08, baseBuf at 0x2C). The ptmf triplets
// come from the 0x80531D38 table (guest/success/failure = entries 1/2/3).
extern "C" void func_80180EBC(CREvtModelMap* self)
{
    // Action-table base; the ptmf triplets at +0x24/+0x30/+0x3C are entries
    // 1/2/3 of the 0x80531D38 table. Kept in a callee-saved register for the
    // whole function (retail loads it in the prologue).
    u32* tbl = (u32*)lbl_eu_80531D20;

    // Retail keeps mFlags|0x40 live in a register across the guest branch;
    // the guest path ORs 0x11 into the same value and stores it once.
    u32 flags = self->mFlags | 0x40;

    if (self->mIsGuest) {
        // Guest path: publish as active, install the updatePosition ptmf
        // (table entry 1, words 9-11), then build the archive base path.
        lbl_eu_806642B0 = self;
        flags |= 0x11;

        // Fixed-offset loads from tbl; store order +0x0C, +0x08, +0x10.
        u32 w0 = tbl[9];
        u32 w1 = tbl[10];
        u32 w2 = tbl[11];
        self->mCallback[1] = w1;
        self->mCallback[0] = w0;
        self->mCallback[2] = w2;

        self->mFlags = flags;

        cf::CfGameManager* mgr = (cf::CfGameManager*)cf::CfGameManager::func_80083298();
        func_800AA33C(*(ml::FixStr<64>*)self->mBasePath, mgr->unk70, 1, 0);

        // Append ".lod" to the base path.
        s32 len = strlen(&lbl_eu_8050372C[0]);
        strcat(self->mBasePath, &lbl_eu_8050372C[0]);
        self->mBasePathLen += len;
    } else {
        // Non-guest path: build "obj/<modelName><ext>" and load .map/.lod/.lgt.
        ml::FixStr<32> baseBuf;    // 0x2C (mString) / 0x4C (mLength)
        ml::FixStr<32> filePath;   // 0x08 (mString) / 0x28 (mLength)

        // String table: ".lod\0/obj/\0.map\0.lgt"; the base is CSE'd for the
        // file-1 strings (retail reloads it for .lod, keeps it for .lgt).
        const char* base = &lbl_eu_8050372C[0];
        baseBuf = base + 5;        // "/obj/"
        filePath = baseBuf;
        filePath += ((CREvtModelMapNameInfo*)self->mPtr1C)->mName;
        filePath += base + 0xB;    // ".map"

        if (CDeviceFile::getFileSize(filePath.mString) > 0) {
            // --- File 1: .map ---
            u32 innerFlags = ((CREvtModelMapNameInfo*)self->mPtr1C)->field_0x58;
            if ((innerFlags & 2) && !(innerFlags & 0x40)) {
                // Read through the common archive.
                self->mFileHandle1 = CDeviceFile::readFile(
                    func_80495FF0(lbl_eu_80663E14), filePath.mString,
                    (IWorkEvent*)(self ? (char*)self + 0x38 : (char*)self), 0, 0);
                func_8044F400__11CDeviceFileFP11CFileHandleUl(self->mFileHandle1, mtl::MemManager::getHandleMEM2());
            } else {
                // Read directly from disc.
                self->mFileHandle1 = CDeviceFile::readFile(
                    mtl::MemManager::getHandleMEM2(), filePath.mString,
                    (IWorkEvent*)(self ? (char*)self + 0x38 : (char*)self), 0, 0);
                func_8044F400__11CDeviceFileFP11CFileHandleUl(self->mFileHandle1, func_80495FF0(lbl_eu_80663E14));
            }

            if (func_8016846C()) {
                func_8044F400__11CDeviceFileFP11CFileHandleUl(self->mFileHandle1, func_80490098__Fv());
            }
            func_8044F400__11CDeviceFileFP11CFileHandleUl(self->mFileHandle1, mtl::MemManager::getHandleMEM1());
            if (((CREvtModelMapNameInfo*)self->mPtr1C)->field_0x58 & 1) {
                CDeviceFile::setHandleFlag1(self->mFileHandle1);
            }

            // --- File 2: .lod ---
            filePath = baseBuf;
            filePath += ((CREvtModelMapNameInfo*)self->mPtr1C)->mName;
            filePath += &lbl_eu_8050372C[0];   // ".lod"
            if (CDeviceFile::getFileSize(filePath.mString) > 0) {
                u32 innerFlags = ((CREvtModelMapNameInfo*)self->mPtr1C)->field_0x58;
                if ((innerFlags & 2) && !(innerFlags & 0x40)) {
                    self->mFileHandle2 = CDeviceFile::readFile(
                        func_80495FF0(lbl_eu_80663E14), filePath.mString,
                        (IWorkEvent*)(self ? (char*)self + 0x38 : (char*)self), 0, 0);
                    func_8044F400__11CDeviceFileFP11CFileHandleUl(self->mFileHandle2, mtl::MemManager::getHandleMEM2());
                } else {
                    self->mFileHandle2 = CDeviceFile::readFile(
                        mtl::MemManager::getHandleMEM2(), filePath.mString,
                        (IWorkEvent*)(self ? (char*)self + 0x38 : (char*)self), 0, 0);
                    func_8044F400__11CDeviceFileFP11CFileHandleUl(self->mFileHandle2, func_80495FF0(lbl_eu_80663E14));
                }

                if (func_8016846C()) {
                    func_8044F400__11CDeviceFileFP11CFileHandleUl(self->mFileHandle2, func_80490098__Fv());
                }
                func_8044F400__11CDeviceFileFP11CFileHandleUl(self->mFileHandle2, mtl::MemManager::getHandleMEM1());
                if (((CREvtModelMapNameInfo*)self->mPtr1C)->field_0x58 & 1) {
                    CDeviceFile::setHandleFlag1(self->mFileHandle2);
                }
            }

            // --- File 3: .lgt ---
            filePath = baseBuf;
            filePath += ((CREvtModelMapNameInfo*)self->mPtr1C)->mName;
            filePath += base + 0x10;  // ".lgt"
            if (CDeviceFile::getFileSize(filePath.mString) > 0) {
                u32 innerFlags = ((CREvtModelMapNameInfo*)self->mPtr1C)->field_0x58;
                if ((innerFlags & 2) && !(innerFlags & 0x40)) {
                    self->mFileHandle3 = CDeviceFile::readFile(
                        func_80495FF0(lbl_eu_80663E14), filePath.mString,
                        (IWorkEvent*)(self ? (char*)self + 0x38 : (char*)self), 0, 0);
                    func_8044F400__11CDeviceFileFP11CFileHandleUl(self->mFileHandle3, mtl::MemManager::getHandleMEM2());
                } else {
                    self->mFileHandle3 = CDeviceFile::readFile(
                        mtl::MemManager::getHandleMEM2(), filePath.mString,
                        (IWorkEvent*)(self ? (char*)self + 0x38 : (char*)self), 0, 0);
                    func_8044F400__11CDeviceFileFP11CFileHandleUl(self->mFileHandle3, func_80495FF0(lbl_eu_80663E14));
                }

                // Retail bug: this error path re-uses mFileHandle2 (0x48)
                // instead of mFileHandle3 (0x94); kept for byte identity.
                if (func_8016846C()) {
                    func_8044F400__11CDeviceFileFP11CFileHandleUl(self->mFileHandle2, func_80490098__Fv());
                }
                func_8044F400__11CDeviceFileFP11CFileHandleUl(self->mFileHandle3, mtl::MemManager::getHandleMEM1());
                if (((CREvtModelMapNameInfo*)self->mPtr1C)->field_0x58 & 1) {
                    CDeviceFile::setHandleFlag1(self->mFileHandle3);
                }
            }

            // Success: mark loaded and install the updatePosition ptmf
            // (table entry 2, words 12-14).
            self->mFlags |= 0x2;
            u32 w0 = tbl[12];
            u32 w1 = tbl[13];
            u32 w2 = tbl[14];
            self->mCallback[1] = w1;
            self->mCallback[0] = w0;
            self->mCallback[2] = w2;
        } else {
            // Failure (.map missing): error flags + failure ptmf
            // (table entry 3, words 15-17).
            self->mFlags |= 0x11;
            u32 w0 = tbl[15];
            u32 w1 = tbl[16];
            u32 w2 = tbl[17];
            self->mCallback[1] = w1;
            self->mCallback[0] = w0;
            self->mCallback[2] = w2;
        }
    }
}

// ---------------------------------------------------------------------------
// 10. func_8018140C - workEvent (0x80182814)
// ---------------------------------------------------------------------------
extern "C" void func_8018140C(CREvtModelMap* self)
{
    if (self->mFlags & 2) {  // bit 1 (LOD flag)
        // Check if we need to create emote model
        if (!self->mFileHandle1 && self->mFileData1 && !self->mEmoteModel) {
            // Create emote model (retail passes the device-file instance in
            // r3 and mFileData1 in r4, then 7 and 1).
            self->mEmoteModel = (CEmoteModelObj*)func_80495E8C(lbl_eu_80663E14, self->mFileData1, 7, 1);
            func_804838DC(self->mEmoteModel, 0);
            func_80484E5C(self->mEmoteModel, lbl_eu_806678C0);

            // Set flag on emote model
            self->mEmoteModel->field_7A8 |= 4;

            self->mVisible = 0;

            if (lbl_eu_806642B4 == self) {
                lbl_eu_806642B4 = 0;
            }
        }

        // Any pending file handles keep us busy; only go idle when all are
        // done and the LOD flag allows it.
        if (self->mFileHandle2 || self->mFileHandle3 || self->mFileHandle1) {
            if (!(self->mFlags & 4)) {  // bit 2
                return;
            }
        }

        // Set to idle state
        self->mFlags |= 0x11;

        // Set ptmf to {0, -1, updatePosition} from 80531D38 + 0x24 (entry 3)
        u32* src = (u32*)lbl_eu_80531D68;
        u32 w0 = *src++;
        u32 w1 = *src++;
        self->mCallback[1] = w1;
        self->mCallback[0] = w0;
        u32 w2 = *src++;
        self->mCallback[2] = w2;
    } else {
        // No LOD flag - set ptmf to {0, -1, updatePosition}
        u32* src = (u32*)lbl_eu_80531D74;
        u32 w0 = *src++;
        u32 w1 = *src++;
        self->mCallback[1] = w1;
        self->mCallback[0] = w0;
        u32 w2 = *src++;
        self->mCallback[2] = w2;
    }
}

// ---------------------------------------------------------------------------
// 11. func_8018152C - updatePosition (0x80182934)
// ---------------------------------------------------------------------------
// Drives the emote model's animation clock, then re-dispatches the camera
// state through vtable+0x30 (CREvtModel::vfunc_30) and refreshes position.
extern "C" void func_8018152C(CREvtModelMap* self)
{
    if (self->mEmoteModel) {
        f32 fTime = (f32)(s32)func_8016A35C();
        func_80484F80(self->mEmoteModel, fTime);
    }

    ((CREvtMapVtIf*)self)->v30(func_80180960());
    func_80168514(self);
}

// ---------------------------------------------------------------------------
// 12. func_801815AC - onEvent (0x801829B4)
// ---------------------------------------------------------------------------
// `visible` is unsigned in the retail source: the mVisible compare compiles
// to a bare lbz + cmplw (no rlwinm mask on the parameter).
extern "C" void func_801815AC(CREvtModelMap* self, unsigned int visible)
{
    // Guest visibility is mirrored into the game-manager core's flag word at
    // +0x100 (bit 2), gated on func_80180978's global state.
    if (self->mIsGuest) {
        cf::CfGameManager* mgr = (cf::CfGameManager*)cf::CfGameManager::func_80083298();
        if (mgr) {
            if (func_80180978()) {
                ((CGameMgrCoreFlagsIf*)cf::CfGameManager::func_80083298())->field_100 &= ~4;
            } else if (visible) {
                ((CGameMgrCoreFlagsIf*)cf::CfGameManager::func_80083298())->field_100 |= 4;
            }
        }
    }

    if (self->mVisible == visible) {
        goto set_guest_flags;
    }

    if (self->mIsGuest) {
        // Tell the game-manager core to enter/leave guest mode.
        cf::CfGameManager* mgr = (cf::CfGameManager*)cf::CfGameManager::func_80083298();
        if (mgr) {
            ((CGameMgrCoreIf*)mgr)->v158(visible);
        }

        if (visible) {
            func_80462B68__8CTaskLODFv();
            lbl_eu_806642B4 = self;
        } else {
            if (lbl_eu_806642B4 == self) {
                func_80462B4C__8CTaskLODFv();
                lbl_eu_806642B4 = 0;
            }
        }

        // Retail re-fetches the singleton for the actual call (two bl's).
        if (func_800828DC__Q22cf13CfGameManagerFv()) {
            func_800828DC__Q22cf13CfGameManagerFv();
            func_8016FC0C(visible);
        }

        // Walk the circular creature list (head at CfGameManager +0x04;
        // re-fetched every iteration because MWCC does not CSE the call).
        s32 i = 0;
        CCreatureNode* head = (CCreatureNode*)((cf::CfGameManager*)func_80086B10__Q22cf13CfGameManagerFv())->field_0x4;
        CCreatureNode* it = head->next;
        while (it != (CCreatureNode*)((cf::CfGameManager*)func_80086B10__Q22cf13CfGameManagerFv())->field_0x4) {
            if (!visible) {
                // Query each creature's status byte and record it, then hide.
                u8 v = it->obj->v160();
                self->mModelName[i] = (char)v;
                self->mCreatureCount++;
                it->obj->v158(0);
            } else {
                // Restore each creature's visibility from the recorded byte.
                u8 v = (u8)self->mModelName[i];
                it->obj->v158(!v);
            }
            it = it->next;
            i++;
        }

        if (visible) {
            self->mCreatureCount = 0;
        }

        goto visible_store;
    }

    // Non-guest path
    if (self->mEmoteModel) {
        if (visible) {
            // Set visible
            if (self->mFileData2) {
                func_80462A08__8CTaskLODFv(1);
            }

            if (self->mFileData3) {
                // Load model data
                self->mLoadedModelData = func_804C1BA0(*(void**)((u8*)lbl_eu_80663E14 + 0x7C), self->mFileData3, 7);

                cf::CfGameManager* m2 = (cf::CfGameManager*)cf::CfGameManager::func_80083298();
                if (m2 && *(void**)((u8*)m2 + 0x2F3C)) {
                    CREvtLightNotifyIf* notif =
                        (CREvtLightNotifyIf*)*(void**)((u8*)cf::CfGameManager::func_80083298() + 0x2F3C);
                    notif->_v068(0);
                }
            }

            lbl_eu_806642B4 = self;
        } else {
            // Clear visible
            if (lbl_eu_806642B4 == self) {
                func_80462B4C__8CTaskLODFv();
                lbl_eu_806642B4 = 0;
            }

            if (self->mLoadedModelData) {
                func_804C1D7C(*(void**)((u8*)lbl_eu_80663E14 + 0x7C));
                self->mLoadedModelData = 0;

                cf::CfGameManager* m2 = (cf::CfGameManager*)cf::CfGameManager::func_80083298();
                if (m2 && *(void**)((u8*)m2 + 0x2F3C)) {
                    CREvtLightNotifyIf* notif =
                        (CREvtLightNotifyIf*)*(void**)((u8*)cf::CfGameManager::func_80083298() + 0x2F3C);
                    notif->_v068(1);
                }
            }
        }

        func_804838DC(self->mEmoteModel, visible);
    }

visible_store:
    self->mVisible = (u8)visible;

set_guest_flags:
    // Guest mode also mirrors a global bit (0x10) in the core flag word.
    if (self->mIsGuest) {
        s32 isZero = (func_80180990() == 0);
        cf::CfGameManager* m3 = (cf::CfGameManager*)cf::CfGameManager::func_80083298();
        if (isZero) {
            ((CGameMgrCoreFlagsIf*)m3)->field_100 |= 0x10;
        } else {
            ((CGameMgrCoreFlagsIf*)m3)->field_100 &= ~0x10;
        }
    }
}

// ---------------------------------------------------------------------------
// 13. func_801818BC - setVisible (0x80182CC4)
// ---------------------------------------------------------------------------
// Guest path mirrors the flag into the game-manager core (vtable+0x190),
// the global sda byte and the device-file task state. Non-guest path
// forwards the camera state to the emote model (vtable+0xB4).
extern "C" void func_801818BC(CREvtModelMap* self, int visible)
{
    if (self->mIsGuest) {
        if (cf::CfGameManager::func_80083298()) {
            cf::CfGameManager* mgr = (cf::CfGameManager*)cf::CfGameManager::func_80083298();
            // virtual call: core->vtable[0x190](visible) - real dispatch
            ((CGameMgrCoreIf*)mgr)->v190(visible);
        }

        if (getGlobalSda()) {
            *(u8*)((u8*)getGlobalSda() + 0xA8) = visible;
        }

        // Mirror the flag into the device-file task state byte
        // (lbl_eu_80663E14 + 0x78 -> +0x28).
        u8* taskState = (u8*)(*(void**)((u8*)lbl_eu_80663E14 + 0x78));
        taskState[0x28] = visible;
    } else if (self->mEmoteModel) {
        // virtual call on the emote model at vtable+0xB4 - real dispatch
        ((CEmoteModelIf*)self->mEmoteModel)->vB4(func_80180960());
    }
}

// ---------------------------------------------------------------------------
// 14. func_8018196C - getGuestState (0x80182D74)
// ---------------------------------------------------------------------------
// retail func_8018196C: (mIsGuest != 0) ? 0 : 3 via neg/or/srawi/andc
extern "C" int func_8018196C(void* self) { return (*(u8*)((char*)self + 0x3D) != 0) ? 0 : 3; }

// ---------------------------------------------------------------------------
// 15. func_80181988 - handleFileEvent (0x80182D90)
// ---------------------------------------------------------------------------
// Matches a completed file request against the three file slots. On success
// (event type 1) the loaded buffer is moved out of the handle; on failure
// slot 1 sets the error flag. Returns 1 when the event was consumed.
extern "C" bool func_80181988(CREvtModelMap* self, CEventFile* pEvent)
{
    // Operand order matters for byte identity: MWCC loads self->mFileHandleN
    // into r5 first, then pEvent->mFileHandle into r0, comparing r0 vs r5.
    if (pEvent->mFileHandle == self->mFileHandle1) {
        if (pEvent->unk0 == 1) {
            // Success - extract the loaded buffer
            self->mFileData1 = self->mFileHandle1->getData();
        } else {
            // Error - set flag
            self->mFlags |= 4;
        }
        self->mFileHandle1 = 0;
        return 1;
    }

    if (pEvent->mFileHandle == self->mFileHandle2) {
        if (pEvent->unk0 == 1) {
            self->mFileData2 = self->mFileHandle2->getData();
        }
        self->mFileHandle2 = 0;
        return 1;
    }

    if (pEvent->mFileHandle == self->mFileHandle3) {
        if (pEvent->unk0 == 1) {
            self->mFileData3 = self->mFileHandle3->getData();
        }
        self->mFileHandle3 = 0;
        return 1;
    }

    return 0;
}

// ---------------------------------------------------------------------------
// Thunks and IWorkEvent interface
// ---------------------------------------------------------------------------

// OnFileEvent is a retail this-adjusting thunk: the IWorkEvent vtable dispatches
// with `this` = IWorkEvent subobject (at +0x38 inside CREvtModelMap), so subtract
// it back and tail-call the real handler.
// Retail: subi r3, r3, 0x38; b func_80181988
bool OnFileEvent__13CREvtModelMapFP10CEventFile(IWorkEvent* self, CEventFile* ev)
{
    return func_80181988(reinterpret_cast<CREvtModelMap*>(reinterpret_cast<char*>(self) - 0x38), ev);
}

// Destructor
// (No member destructor: the flattened class has no vtable/dtor machinery,
// so the retail `__ct__80180B00` free function above is the only dtor.)

// Adjusting thunk: upcasts from IWorkEvent sub-object to full CREvtModelMap
// (retail: subi r3, r3, 0x38; b __ct__80180B00) and forwards the dealloc flag.
extern "C" void func_80181A54(void* self, int dealloc) {
    __ct__80180B00((CREvtModelMap*)((char*)self - 0x38), dealloc);
}