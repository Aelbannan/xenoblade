// Decompiled CREvtModelMap — realtime event model map
// High-level C++ reconstruction.

#include <string.h>
#include "kyoshin/harness_catalog.hpp"
#include "kyoshin/realtimeevt/CREvtModelMap.hpp"
#include "monolib/device/CDeviceFile.hpp"
#include "monolib/util/MemManager.hpp"
#include "include/kyoshin/cf/CfGameManager.hpp"

// ---------------------------------------------------------------------------
// Action table entries — {startTime, creatureId, callback}
// ---------------------------------------------------------------------------
struct ActionEntry {
    s32 startTime;
    s32 creatureId;
    void* callback;
};

extern "C" {
    extern ActionEntry lbl_eu_80531D20[];  // 1 entry: {0, -1, loadFiles}
    extern ActionEntry lbl_eu_80531D2C[];  // 1 entry: {0, -1, loadCreature}
    extern ActionEntry lbl_eu_80531D38[];  // 4 entries
    extern ActionEntry lbl_eu_80531D68[];  // 1 entry: {0, -1, updatePosition}
    extern ActionEntry lbl_eu_80531D74[];  // 1 entry: {0, -1, updatePosition}
    extern const char lbl_eu_8050372C[];   // path strings: ".lod\0/obj/\0.map\0.lgt"

    // Forward declarations for internal helpers
    void func_800AA33C(char* buf, u32 archiveId, int r5, int r6);
    void func_8016BC1C(CREvtModelMap* self);
    bool func_8016BDA8(s32* pId);
    u32 func_8016A35C();
    void func_8016846C();
    void func_80168514(CREvtModelMap* self);
    void func_80180960(CREvtModelMap* self, s32* out);
    s32 func_80180978();
    s32 func_80180990();
    void func_8016FC0C(int val);
    void func_80462B30__8CTaskLODFv();
    void func_80462B68__8CTaskLODFv();
    void func_80462B4C__8CTaskLODFv();
    void func_80495FF0();
    void func_80495E60(void* pEmote);
    void* func_80495E8C(int r5, int r6);
    void func_804838DC(void* pEmote, int r4);
    void func_80484E5C(void* pEmote, float scale);
    void func_80484F80(void* pEmote, float time);
    void func_804C1D7C(void* pData);
    void func_804C1BA0(void* pData, int r5);
    void func_80490098__Fv();
    void getGlobalSda();
    bool func_8016D094(CREvtModelMap* self, s32* pId);
    void func_80169710();
    void func_8016B600();
    void func_80181D5C();
    void func_80181D74();
    void func_80181D8C();
    void func_80171008();
    void __dt__80185754();
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
    void func_801726DC();
    void func_80172668();
    void func_80181A54(void* self);
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
}

// ---------------------------------------------------------------------------
// 1. __ct__CREvtModelMap — constructor (0x80181DB4)
// ---------------------------------------------------------------------------
CREvtModelMap::CREvtModelMap()
{
    // Call base constructor: CREvtModel(self, pDataFromCaller, 1)
    // The memory for this is allocated by the caller, __ct__CREvtModel
    // is called implicitly by the compiler.

    // Base class constructor already ran. Now initialize CREvtModelMap fields.
    // (The assembly shows an explicit __ct__CREvtModel call, but in C++ this
    //  is handled by the constructor chain. We match the assembly by ensuring
    //  the base is initialized before our field init.)

    // Set vtable to CREvtModelMap vtable (implicit in C++ for the class)
    // Set IWorkEvent vtable at offset 0x38 (implicit via inheritance)

    mVisible = 1;
    mIsGuest = 0;

    mFileHandle1 = 0;
    mFileData1 = 0;
    mFileHandle2 = 0;
    mFileData2 = 0;

    mBasePath[0] = 0;
    mBasePathLen = 0;
    mFileHandle3 = 0;
    mFileData3 = 0;
    mPad9C = 0;

    mUnkDC = 0;
    mLoadedModelData = 0;
    mEmoteModel = 0;

    memset(mModelName, 0, sizeof(mModelName));
    mCreatureCount = 0;
    mCreatureId = -1;

    // Check if this is a guest model
    void* innerPtr = mPtr1C;
    if (innerPtr) {
        u32 scriptData = *(u32*)((u8*)innerPtr + 0x20);
        // Extract bit fields
        u32 result1 = (scriptData >> 27) & 0x1F;
        u32 result2 = (scriptData >> 5) & 0x7F;
        u32 result3 = (scriptData >> 12) & 0x3FF;
        u32 result4 = scriptData & 0x3FF;

        if (result1 == 1) {
            mIsGuest = 1;

            // Set position data from the guest action table
            ActionEntry* entry = &lbl_eu_80531D20[0];
            mPtmf[0] = entry->startTime;
            mPtmf[1] = entry->creatureId;
            mPtmf[2] = (u32)entry->callback;

            mFlags = (mFlags & ~0xFF) | 0x10;

            // Store as global visible model
            lbl_eu_806642B4 = this;
        } else {
            // Non-guest — set up the default action table
            ActionEntry* entry = &lbl_eu_80531D2C[0];
            mPtmf[0] = entry->startTime;
            mPtmf[1] = entry->creatureId;
            mPtmf[2] = (u32)entry->callback;

            func_8016BC1C(this);

            void* inner = mPtr1C;
            if (inner && *(u32*)((u8*)inner + 0x30) != 0) {
                mFlags = (mFlags & ~0xFF) | 0x30;
            }
        }
    }
}

// ---------------------------------------------------------------------------
// 2. __ct__80180B00 — destructor / reset constructor (0x80181EFC)
// Called as both the primary destructor and a reset.
// ---------------------------------------------------------------------------
void __ct__80180B00(CREvtModelMap* self, int dealloc)
{
    if (!self) {
        return;
    }

    // Restore vtable
    // (C++ compiler handles this implicitly)

    u8 wasGuest = self->mIsGuest;

    // Restore IWorkEvent vtable
    // (C++ compiler handles this implicitly)

    if (wasGuest) {
        // Call virtual setVisible(0)
        self->setVisible(0);

        cf::CfGameManager* mgr = cf::CfGameManager::func_80083298();
        if (mgr) {
            // virtual call to CfGameManager::someMethod(1)
            ((void(*)(void*, int))mgr->vtable[0x158/4])(mgr, 1);
        }

        func_80462B30__8CTaskLODFv();

        if (self->mCreatureCount > 0) {
            for (s32 i = 0; i < self->mCreatureCount; i++) {
                void* creature = *(void**)((u8*)cf::CfGameManager::func_80086B10()->field_0x4 + i * 4);
                // Actually need to iterate through the list
                // Simplified: iterate through the list
            }
        }

        // Clear global pointer
        lbl_eu_806642B0 = 0;

        if (cf::CfGameManager::func_800828DC()) {
            func_8016FC0C(1);
        }
    }

    // Call virtual at vtable+0x3C (vfunc_3C = cleanup)
    // In C++ this is the destructor chain, handled by the compiler

    func_80462B68__8CTaskLODFv();

    // Call __ct__80172668 (the base destructor for CREvtModel)
    __ct__80172668(self, 0);

    if (dealloc <= 0) {
        return;
    }

    // Deallocate memory
    // (handled by the compiler in C++ — operator delete)
}

// ---------------------------------------------------------------------------
// 3. func_80180C60 — isMapModel (0x8018205C)
// ---------------------------------------------------------------------------
bool CREvtModelMap::isMapModel()
{
    void* inner = mPtr1C;
    if (!inner) return 0;

    u8* name = (u8*)inner + 0x10;
    if (name[0] == 0x6d && name[1] == 0x61) { // 'm' 'a'
        return 1;
    }
    return 0;
}

// ---------------------------------------------------------------------------
// 4. func_80180C88 — isBusy (0x80182084)
// ---------------------------------------------------------------------------
bool CREvtModelMap::isBusy()
{
    if (mFileHandle1) return 1;
    if (mFileHandle2) return 1;
    if (mFileHandle3) return 1;
    return 0;
}

// ---------------------------------------------------------------------------
// 5. func_80180CBC — reset (0x801820B8)
// ---------------------------------------------------------------------------
bool CREvtModelMap::reset()
{
    if (mFlags & 0x100) { // bit 24
        return 0;
    }

    // Call func_801726DC (base class reset)
    func_801726DC();

    // Free emote model
    if (mEmoteModel) {
        func_80495E60(mEmoteModel);
        mEmoteModel = 0;
    }

    // Free loaded model data
    if (mLoadedModelData) {
        void* deviceFile = *(void**)lbl_eu_80663E14;
        func_804C1D7C(*(void**)((u8*)deviceFile + 0x7C));
        mLoadedModelData = 0;
    }

    // Free file data buffers
    if (mFileData1) {
        mtl::MemManager::deallocate(mFileData1);
        mFileData1 = 0;
    }
    if (mFileData2) {
        mtl::MemManager::deallocate(mFileData2);
        mFileData2 = 0;
    }
    if (mFileData3) {
        mtl::MemManager::deallocate(mFileData3);
        mFileData3 = 0;
    }

    // Cancel file handles
    if (mFileHandle1) {
        CDeviceFile::cancel(mFileHandle1);
    }
    if (mFileHandle2) {
        CDeviceFile::cancel(mFileHandle2);
    }
    if (mFileHandle3) {
        CDeviceFile::cancel(mFileHandle3);
    }

    // Clear file handles
    mFileHandle1 = 0;
    mFileHandle2 = 0;
    mFileHandle3 = 0;

    return 1;
}

// ---------------------------------------------------------------------------
// 6. func_80180DCC — setGuestModeOff (0x801821C8)
// ---------------------------------------------------------------------------
void CREvtModelMap::setGuestModeOff()
{
    if (lbl_eu_806642B0) {
        return;
    }

    cf::CfGameManager* mgr = cf::CfGameManager::func_80083298();
    if (mgr) {
        // virtual call: CfGameManager::setGuestMode(0)
        ((void(*)(void*, int))mgr->vtable[0x158/4])(mgr, 0);
        func_80462B4C__8CTaskLODFv();
    }
}

// ---------------------------------------------------------------------------
// 7. func_80180E1C — setGuestModeOn (0x80182218)
// ---------------------------------------------------------------------------
void CREvtModelMap::setGuestModeOn()
{
    if (lbl_eu_806642B0) {
        return;
    }

    cf::CfGameManager* mgr = cf::CfGameManager::func_80083298();
    if (mgr) {
        // virtual call: CfGameManager::setGuestMode(1)
        ((void(*)(void*, int))mgr->vtable[0x158/4])(mgr, 1);
        func_80462B30__8CTaskLODFv();
    }
}

// ---------------------------------------------------------------------------
// 8. func_80180E60 — loadCreature (0x8018225C)
// ---------------------------------------------------------------------------
void CREvtModelMap::loadCreature()
{
    mCreatureId = -1;

    if (func_8016BDA8(&mCreatureId)) {
        // Set ptmf to the first entry of the action table at 80531D38
        ActionEntry* entry = &lbl_eu_80531D38[0];
        mPtmf[0] = entry->startTime;
        mPtmf[1] = entry->creatureId;
        mPtmf[2] = (u32)entry->callback;
    }
}

// ---------------------------------------------------------------------------
// 9. func_80180EBC — loadFiles (0x801822B8)
// ---------------------------------------------------------------------------
void CREvtModelMap::loadFiles()
{
    mFlags |= 0x40;
    lbl_eu_806642B0 = this;

    if (mIsGuest) {
        // Guest path: set up base path from CfGameManager
        cf::CfGameManager* mgr = cf::CfGameManager::func_80083298();
        if (mgr) {
            func_800AA33C(mBasePath, mgr->unk70, 1, 0);
        }

        // Append ".lod" to the base path
        s32 len = strlen(&lbl_eu_8050372C[0]);
        strcat(mBasePath, &lbl_eu_8050372C[0]);
        mBasePathLen += len;

        // Set ptmf to {0, -1, updatePosition} from 80531D20 + 0x24
        // (which is entry 1 of the 80531D38 table)
        ActionEntry* entry = (ActionEntry*)((u8*)&lbl_eu_80531D20[0] + 0x24);
        mPtmf[0] = entry->startTime;
        mPtmf[1] = entry->creatureId;
        mPtmf[2] = (u32)entry->callback;

        mFlags = (mFlags & ~0xFF) | 0x11;
    } else {
        // Non-guest path: load .map, .lod, .lgt files
        // String table: 8050372C = ".lod\0/obj/\0.map\0.lgt"
        const char* basePath = &lbl_eu_8050372C[5];  // "/obj/"
        const char* extMap = &lbl_eu_8050372C[11];   // ".map"
        const char* extLod = &lbl_eu_8050372C[0];    // ".lod"
        const char* extLgt = &lbl_eu_8050372C[16];   // ".lgt" (wait, need to check offset)

        // Actually, let me compute the offsets:
        // 8050372C: ".lod\0/obj/\0.map\0.lgt\0"
        // 8050372C + 0 = ".lod"
        // 8050372C + 5 = "/obj/"
        // 8050372C + 11 = ".map"
        // 8050372C + 16 = ".lgt"

        // Get model name from inner struct
        void* inner = mPtr1C;
        const char* modelName = (const char*)inner + 0x10;
        s32 nameLen = strlen(modelName);

        // Build file path buffer on stack
        char filePath[0x100];
        char baseBuf[0x40];

        strcpy(baseBuf, basePath);
        s32 baseLen = strlen(basePath);

        // --- File 1: .map ---
        {
            strcpy(filePath, baseBuf);
            strcat(filePath, modelName);
            strcat(filePath, extMap);
            s32 fileSize = CDeviceFile::getFileSize(filePath);
            if (fileSize > 0) {
                void* deviceFile = *(void**)lbl_eu_80663E14;
                u32* pDev = (u32*)deviceFile;
                // Check flags
                u32 innerFlags = *(u32*)((u8*)inner + 0x58);
                if ((innerFlags & 2) && !(innerFlags & 0x40)) {
                    // Read from archive
                    mtl::ALLOC_HANDLE archiveHandle = func_80495FF0();
                    IWorkEvent* pEvent = static_cast<IWorkEvent*>(this);
                    mFileHandle1 = CDeviceFile::readFile(archiveHandle, filePath, pEvent, 0, 0);
                } else {
                    // Read from file
                    mtl::ALLOC_HANDLE mem2Handle = mtl::MemManager::getHandleMEM2();
                    IWorkEvent* pEvent = static_cast<IWorkEvent*>(this);
                    mFileHandle1 = CDeviceFile::readFile(mem2Handle, filePath, pEvent, 0, 0);
                }

                CDeviceFile::func_8044F400(mFileHandle1, mtl::MemManager::getHandleMEM2());
                func_8016846C();
                if (func_8016846C()) {  // This is a bit odd — re-call
                    mtl::ALLOC_HANDLE h = func_80490098__Fv();
                    CDeviceFile::func_8044F400(mFileHandle1, h);
                }
                CDeviceFile::func_8044F400(mFileHandle1, mtl::MemManager::getHandleMEM1());

                inner = mPtr1C;
                if (*(u32*)((u8*)inner + 0x58) & 1) {
                    CDeviceFile::setHandleFlag1(mFileHandle1);
                }
            }
        }

        // --- File 2: .lod ---
        {
            // Rebuild path: base + modelName + ".lod"
            strcpy(filePath, baseBuf);
            strcat(filePath, modelName);
            strcat(filePath, extLod);
            s32 fileSize = CDeviceFile::getFileSize(filePath);
            if (fileSize > 0) {
                void* inner = mPtr1C;
                u32 innerFlags = *(u32*)((u8*)inner + 0x58);
                if ((innerFlags & 2) && !(innerFlags & 0x40)) {
                    mtl::ALLOC_HANDLE archiveHandle = func_80495FF0();
                    IWorkEvent* pEvent = static_cast<IWorkEvent*>(this);
                    mFileHandle2 = CDeviceFile::readFile(archiveHandle, filePath, pEvent, 0, 0);
                } else {
                    mtl::ALLOC_HANDLE mem2Handle = mtl::MemManager::getHandleMEM2();
                    IWorkEvent* pEvent = static_cast<IWorkEvent*>(this);
                    mFileHandle2 = CDeviceFile::readFile(mem2Handle, filePath, pEvent, 0, 0);
                }

                CDeviceFile::func_8044F400(mFileHandle2, mtl::MemManager::getHandleMEM2());
                func_8016846C();
                if (func_8016846C()) {
                    mtl::ALLOC_HANDLE h = func_80490098__Fv();
                    CDeviceFile::func_8044F400(mFileHandle2, h);
                }
                CDeviceFile::func_8044F400(mFileHandle2, mtl::MemManager::getHandleMEM1());

                inner = mPtr1C;
                if (*(u32*)((u8*)inner + 0x58) & 1) {
                    CDeviceFile::setHandleFlag1(mFileHandle2);
                }
            }
        }

        // --- File 3: .lgt ---
        {
            // Rebuild path: base + modelName + ".lgt"
            strcpy(filePath, baseBuf);
            strcat(filePath, modelName);
            strcat(filePath, extLgt);
            s32 fileSize = CDeviceFile::getFileSize(filePath);
            if (fileSize > 0) {
                // Read file
                void* inner = mPtr1C;
                u32 innerFlags = *(u32*)((u8*)inner + 0x58);
                if ((innerFlags & 2) && !(innerFlags & 0x40)) {
                    mtl::ALLOC_HANDLE archiveHandle = func_80495FF0();
                    IWorkEvent* pEvent = static_cast<IWorkEvent*>(this);
                    mFileHandle3 = CDeviceFile::readFile(archiveHandle, filePath, pEvent, 0, 0);
                } else {
                    mtl::ALLOC_HANDLE mem2Handle = mtl::MemManager::getHandleMEM2();
                    IWorkEvent* pEvent = static_cast<IWorkEvent*>(this);
                    mFileHandle3 = CDeviceFile::readFile(mem2Handle, filePath, pEvent, 0, 0);
                }

                CDeviceFile::func_8044F400(mFileHandle3, mtl::MemManager::getHandleMEM2());
                func_8016846C();
                if (func_8016846C()) {
                    mtl::ALLOC_HANDLE h = func_80490098__Fv();
                    CDeviceFile::func_8044F400(mFileHandle3, h);
                }
                CDeviceFile::func_8044F400(mFileHandle3, mtl::MemManager::getHandleMEM1());

                inner = mPtr1C;
                if (*(u32*)((u8*)inner + 0x58) & 1) {
                    CDeviceFile::setHandleFlag1(mFileHandle3);
                }
            }
        }

        // If any file was loaded (we reach here), set flags and ptmf
        // Set ptmf to entry 2 of 80531D38 = {0, -1, workEvent}
        ActionEntry* successEntry = (ActionEntry*)((u8*)&lbl_eu_80531D38[0] + 0x18);
        mFlags = (mFlags & ~0xFF) | 0x02;
        mPtmf[0] = successEntry->startTime;
        mPtmf[1] = successEntry->creatureId;
        mPtmf[2] = (u32)successEntry->callback;
    }
}

// ---------------------------------------------------------------------------
// 10. func_8018140C — workEvent (0x80182814)
// ---------------------------------------------------------------------------
void CREvtModelMap::workEvent()
{
    if (mFlags & 2) {  // bit 1 (LOD flag)
        // Check if we need to create emote model
        if (!mFileHandle1 && mFileData1 && !mEmoteModel) {
            void* deviceFile = *(void**)lbl_eu_80663E14;
            // Create emote model
            mEmoteModel = func_80495E8C(7, 1);
            func_804838DC(mEmoteModel, 0);
            func_80484E5C(mEmoteModel, lbl_eu_806678C0);

            // Set flag on emote model
            *(u32*)((u8*)mEmoteModel + 0x7A8) |= 4;

            mVisible = 0;

            if (lbl_eu_806642B4 == this) {
                lbl_eu_806642B4 = 0;
            }
        }

        // Check file handles
        bool hasHandles = (mFileHandle2 || mFileHandle3 || mFileHandle1);
        if (!hasHandles || (mFlags & 4)) {  // bit 2
            goto set_idle;
        }
        return;

    set_idle:
        // Set to idle state
        mFlags = (mFlags & ~0xFF) | 0x11;

        // Set ptmf to {0, -1, updatePosition} from 80531D38 + 0x24 (entry 3)
        ActionEntry* entry = (ActionEntry*)((u8*)&lbl_eu_80531D38[0] + 0x24);
        mPtmf[0] = entry->startTime;
        mPtmf[1] = entry->creatureId;
        mPtmf[2] = (u32)entry->callback;
    } else {
        // No LOD flag — set ptmf to {0, -1, updatePosition}
        ActionEntry* entry = (ActionEntry*)&lbl_eu_80531D74[0];
        mPtmf[0] = entry->startTime;
        mPtmf[1] = entry->creatureId;
        mPtmf[2] = (u32)entry->callback;
    }
}

// ---------------------------------------------------------------------------
// 11. func_8018152C — updatePosition (0x80182934)
// ---------------------------------------------------------------------------
void CREvtModelMap::updatePosition()
{
    if (mEmoteModel) {
        u32 time = func_8016A35C();
        // Convert u32 to float
        double dVal = (double)(s32)(time ^ 0x80000000);
        float fTime = (float)(dVal + (double)(s32)0x80000000);
        func_80484F80(mEmoteModel, fTime);
    }

    s32 val;
    func_80180960(this, &val);
    setVisible(val);
    func_80168514(this);
}

// ---------------------------------------------------------------------------
// 12. func_801815AC — onEvent (0x801829B4)
// ---------------------------------------------------------------------------
void CREvtModelMap::onEvent(int r4)
{
    if (mIsGuest) {
        cf::CfGameManager* mgr = cf::CfGameManager::func_80083298();
        if (mgr) {
            s32 result = func_80180978();
            if (result) {
                // Clear bit 2 on CfGameManager flag
                mgr->unk68 &= ~4;
            } else {
                if (r4) {
                    // Set bit 2
                    mgr->unk68 |= 4;
                }
            }
        }
    }

    if (mVisible == (u8)r4) {
        goto check_guest;
    }

    if (mIsGuest) {
        cf::CfGameManager* mgr = cf::CfGameManager::func_80083298();
        if (mgr) {
            // virtual call: CfGameManager::setGuestMode(r4)
            ((void(*)(void*, int))mgr->vtable[0x158/4])(mgr, r4);
        }

        if (r4) {
            func_80462B68__8CTaskLODFv();
            lbl_eu_806642B4 = this;
        } else {
            if (lbl_eu_806642B4 == this) {
                func_80462B4C__8CTaskLODFv();
                lbl_eu_806642B4 = 0;
            }
        }

        if (cf::CfGameManager::func_800828DC()) {
            func_8016FC0C(r4);
        }

        // Iterate creature list
        s32 i = 0;
        // ... list iteration ...
        // Simplified: iterate through CfGameManager's creature list
        for (void* it = /* first element */; it != /* list end */; it = /* next */) {
            if (!r4) {
                u8 val = /* get creature value */;
                mBasePath[i] = val;
                mCreatureCount++;
                // virtual call on creature
            } else {
                // Restore creature visibility
                u8 val = mBasePath[i];
                // virtual call on creature
            }
            i++;
        }

        if (r4) {
            mCreatureCount = 0;
        }

        goto check_guest;
    }

    // Non-guest path
    if (mEmoteModel) {
        if (r4) {
            // Set visible
            if (mFileData2) {
                // notifyTaskLOD
            }

            if (mFileData3) {
                // Load model data
                void* deviceFile = *(void**)lbl_eu_80663E14;
                mLoadedModelData = func_804C1BA0(mFileData3, 7);
                cf::CfGameManager* mgr = cf::CfGameManager::func_80083298();
                if (mgr && mgr->unk2F3C) {
                    // virtual call
                }
            }

            lbl_eu_806642B4 = this;
        } else {
            if (lbl_eu_806642B4 == this) {
                func_80462B4C__8CTaskLODFv();
                lbl_eu_806642B4 = 0;
            }

            if (mLoadedModelData) {
                void* deviceFile = *(void**)lbl_eu_80663E14;
                func_804C1D7C(*(void**)((u8*)deviceFile + 0x7C));
                mLoadedModelData = 0;
            }

            cf::CfGameManager* mgr = cf::CfGameManager::func_80083298();
            if (mgr && mgr->unk2F3C) {
                // virtual call
            }
        }

        func_804838DC(mEmoteModel, r4);
    }

check_guest:
    mVisible = (u8)r4;

    if (mIsGuest) {
        s32 val = func_80180990();
        if (val) {
            cf::CfGameManager* mgr = cf::CfGameManager::func_80083298();
            if (mgr) {
                mgr->unk68 |= 0x10;
            }
        } else {
            cf::CfGameManager* mgr = cf::CfGameManager::func_80083298();
            if (mgr) {
                mgr->unk68 &= ~0x10;
            }
        }
    }
}

// ---------------------------------------------------------------------------
// 13. func_801818BC — setVisible (0x80182CC4)
// ---------------------------------------------------------------------------
void CREvtModelMap::setVisible(int r4)
{
    if (mIsGuest) {
        cf::CfGameManager* mgr = cf::CfGameManager::func_80083298();
        if (mgr) {
            // virtual call at vtable+0x190
            ((void(*)(void*, int))mgr->vtable[0x190/4])(mgr, r4);
        }

        void* sda = getGlobalSda();
        if (sda) {
            *(u8*)((u8*)sda + 0xA8) = (u8)r4;
        }

        void* deviceFile = *(void**)lbl_eu_80663E14;
        *(u8*)((u8*)(*(void**)((u8*)deviceFile + 0x78)) + 0x28) = (u8)r4;
    } else {
        if (mEmoteModel) {
            s32 val;
            func_80180960(this, &val);
            // virtual call on emote model at vtable+0xB4
            ((void(*)(void*, s32))mEmoteModel->vtable[0xB4/4])(mEmoteModel, val);
        }
    }
}

// ---------------------------------------------------------------------------
// 14. func_8018196C — getGuestState (0x80182D74)
// ---------------------------------------------------------------------------
int CREvtModelMap::getGuestState()
{
    if (mIsGuest) {
        return 0;
    }
    return 3;
}

// ---------------------------------------------------------------------------
// 15. func_80181988 — handleFileEvent (0x80182D90)
// ---------------------------------------------------------------------------
bool CREvtModelMap::handleFileEvent(CEventFile* pEvent)
{
    CFileHandle* handle = pEvent->mFileHandle;

    // Check if this event matches mFileHandle1
    if (handle == mFileHandle1) {
        if (pEvent->unk0 == 1) {
            // Success — extract data
            mFileData1 = mFileHandle1->getData();
            mFileHandle1->mData = 0;
        } else {
            // Error — set flag
            mFlags |= 4;
        }
        mFileHandle1 = 0;
        return 1;
    }

    // Check if this event matches mFileHandle2
    if (handle == mFileHandle2) {
        if (pEvent->unk0 == 1) {
            mFileData2 = mFileHandle2->getData();
            mFileHandle2->mData = 0;
        }
        mFileHandle2 = 0;
        return 1;
    }

    // Check if this event matches mFileHandle3
    if (handle == mFileHandle3) {
        if (pEvent->unk0 == 1) {
            mFileData3 = mFileHandle3->getData();
            mFileHandle3->mData = 0;
        }
        mFileHandle3 = 0;
        return 1;
    }

    return 0;
}

// ---------------------------------------------------------------------------
// Thunks and IWorkEvent interface
// ---------------------------------------------------------------------------

// OnFileEvent thunk: upcasts from IWorkEvent* to CREvtModelMap*
bool CREvtModelMap::OnFileEvent(CEventFile* pEventFile)
{
    return handleFileEvent(pEventFile);
}

// Destructor
CREvtModelMap::~CREvtModelMap()
{
    // The compiler generates the destructor chain.
    // The retail symbol __ct__80180B00 serves as both the destructor
    // and a reset constructor, handling deallocation.
}

// Adjusting thunk: upcasts from IWorkEvent sub-object to full CREvtModelMap
void func_80181A54(void* self)
{
    CREvtModelMap* map = reinterpret_cast<CREvtModelMap*>(
        static_cast<char*>(self) - 0x38);
    __ct__80180B00(map, 1);
}

// OnFileEvent adjusting thunk in assembly:
//   subi r3, r3, 0x38
//   b func_80181988
// This is handled by the compiler-generated thunk for the interface override.