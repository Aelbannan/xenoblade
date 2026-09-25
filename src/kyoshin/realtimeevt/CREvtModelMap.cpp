// Decompiled CREvtModelMap - realtime event model map
// High-level C++ reconstruction.

#include <string.h>
#include "kyoshin/harness_catalog.hpp"
#include "kyoshin/realtimeevt/CREvtModelMap.hpp"
#include <decomp.h>
#include "kyoshin/realtimeevt/CREvtLight.hpp"
// The +0x2F3C game-manager sub-object is a CScnEnvLgtCtrlListItem (real
// owner in libs/monolib/src/scn/CScnEnvLgtCtrl.hpp, which cannot be
// included here over this TU's conflicting decls). TU-local view under the
// real owner name with retail arities (CTaskREvtSequence.cpp idiom); this
// TU dispatches its v24 slot (0x68) with the retail int flag.
class CScnEnvLgtCtrlListItem {
public:
    virtual void v0(int flag);
    virtual void v1();
    virtual void v2(void* arg);
    virtual void v3();
    virtual void v4();
    virtual void v5();
    virtual void v6();
    virtual void v7();
    virtual void v8(void* arg);
    virtual void v9(void* arg);
    virtual void v10(void* arg, void* data);
    virtual void v11(void* arg, u8 byte);
    virtual int v12(void* vec);
    virtual int v13(void* vec);
    virtual void v14(void* data);
    virtual int v15(float f, void* data, void* arg, u32 count);
    virtual void v16(void* arg);
    virtual void v17(void* arg);
    virtual void v18(void* arg, float f);
    virtual int v19(void* bits, int flag);
    virtual int v20(void* base, int flag);
    virtual void v21();
    virtual void v22(float f, s32 mode, s32 submode);
    virtual int v23();
    virtual void v24(int flag);  // slot 0x68 - retail passes r4
    virtual void v25(int flag);
    virtual void v26(void* a, void* b);
    virtual void v27(void* a, void* b, float f);
    virtual int v28(void* out, int flag);
    virtual int v29();
    virtual int v30();
    virtual void v31(u32 a, u32 b);
    virtual void v32(u32 a, u32 b, void* c, void* d);
    virtual void v33(u32 a, u32 b, void* c);
    virtual void v34();
    virtual void v35(u32 a, u32 b, u32 c, u32 d, u32 e, u32 f, u32 g);
    virtual void v36();
    virtual void v37(int flag);
};
#include "kyoshin/cf/object/CfObject.hpp"
#include "kyoshin/cf/CfGimmick.hpp"
#include "monolib/device/CDeviceFile.hpp"
#include "monolib/util/MemManager.hpp"
#include "monolib/util/FixStr.hpp"
#include "include/kyoshin/cf/CfGameManager.hpp"
#include "kyoshin/cf/CfGameManagerData.hpp"
#include "libs/monolib/src/scn/CScnItemModel.hpp"

// ---------------------------------------------------------------------------
// Action table entries - {startTime, creatureId, callback}
// ---------------------------------------------------------------------------
struct ActionEntry {
    s32 startTime;
    s32 creatureId;
    void* callback;
};

#include "monolib/work/IWorkEvent.hpp"
#include "monolib/work/CEventFile.hpp"

// Forward decls for labels and helpers
extern "C" {
extern ActionEntry lbl_eu_80531D20[];
extern ActionEntry lbl_eu_80531D2C[];
extern u32 lbl_eu_80531D38[];
extern ActionEntry lbl_eu_80531D68[];
extern ActionEntry lbl_eu_80531D74[];
extern const char lbl_eu_8050372C[];
extern const char lbl_eu_80503718[];

void func_800AA33C(ml::FixStr<64>& buf, u32 packed, int prefixFlag, int suffixFlag);
bool func_80181988(CREvtModelMap* self, CEventFile* ev);
void func_8016BC1C(CREvtModelMap* self);
bool func_8016BDA8(void* self, s32* pId);
u32 EvtSeqGetCounter100();
u32 EvtSeqGetStateBit5();
void EvtSeqCheckEventRunGuard(CREvtModelMap* self);
u32 REvtCam_IsField98Set();
s32 REvtCam_IsMode2B0Eq1();
s32 REvtCam_IsMode2B0Eq2();
void MapFx_SetPointEnabled(int val);
void enableLOD__8CTaskLODFv();
void restorePrimaryLOD__8CTaskLODFv();
void disableLOD__8CTaskLODFv();
void acquireLODResource__8CTaskLODFv(void* task, u32 value);
extern mtl::ALLOC_HANDLE Scn_CallUnk8C_V9(void* self);
void Scn_IsAnimActiveOrNull(void* pEmote);
void* Scn_SetupAnim(void* a, void* b, int c, int d);
void simSetFlag2OnTree(void* pEmote, int r4);
void simSetLeafDist7B0(void* pEmote, float scale);
void simRefreshFlag8(void* pEmote, float time);
void scnLgtRemoveLgtItem(void* pData);
void* func_804C1BA0(void* pData, void* pFile, int r5);
mtl::ALLOC_HANDLE getScnCounter__Fv();
void* getGlobalSda();
void __dt__80185754(CREvtModelMap* self);
void __ct__80172668(CREvtModelMap* self, int dealloc);
void getField20();
void func_801729F0();
void func_80172768(CREvtModelMap* self, int r4);
void func_801727DC();
void func_801728F8();
void releaseEvtAnim28();
void CREvtObjVfunc24Default();
void isEvtFlagBit4Set();
void isEvtFlagBit0Set();
void zeroEvtModelHook();
void emptyEvtModelHook();
void func_80172CC4();
void func_801726DC(CREvtModelMap* self);
void func_80172668();
void MapModel_ThunkCtor38(void* self, int dealloc);
extern CREvtModelMap* lbl_eu_806642B0;
extern CREvtModelMap* lbl_eu_806642B4;
extern float lbl_eu_806678C0;
extern double lbl_eu_806678C8;
extern void* spawnGimmickEntity__Q22cf13CfGameManagerFv();
extern void* getMapEffectManager__Q22cf13CfGameManagerFv();
extern void addAllocHandle__11CDeviceFileFP11CFileHandleUl(CFileHandle*, u32);
extern void __ct__CREvtModel(void* self, void* pData, int pArg);
extern void Tok_Unpack(u32 packed, u32* out0, u32* out1, u32* out2, u32* out3);
extern u8 lbl_eu_80531D80[];
}

__declspec(section ".rodata") const unsigned char rodata_all[48] __attribute__((used, aligned(1))) = {
    0x63,0x66,0x3a,0x3a,0x43,0x52,0x45,0x76,0x74,0x4d,0x6f,0x64,0x65,0x6c,0x4d,0x61,
    0x70,0x00,0x00,0x00,0x2e,0x6c,0x6f,0x64,0x00,0x2f,0x6f,0x62,0x6a,0x2f,0x00,0x2e,
    0x6d,0x61,0x70,0x00,0x2e,0x6c,0x67,0x74,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
};
#define lbl_eu_80503718 ((const char*)rodata_all)
#define lbl_eu_8050372C ((const char*)(rodata_all+20))
__declspec(section ".data") __attribute__((used, aligned(8))) const unsigned char __data_CREvtModelMap[0x140] = {
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0xFF,0xFF,0xFF,0xFF,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0xFF,0xFF,0xFF,0xFF,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFF,0xFF,0xFF,0xFF,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFF,0xFF,0xFF,0xFF,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0xFF,0xFF,0xFF,0xFF,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0xFF,0xFF,0xFF,0xFF,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFF,0xFF,0xFF,0xFF,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFF,0xFF,0xFF,0xFF,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFF,0xFF,0xFF,0xC8,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
};
extern const unsigned char rodata_all[48];
extern const unsigned char __data_CREvtModelMap[0x140];
__declspec(section ".sdata") __attribute__((used, aligned(8))) const void* __sdata_CREvtModelMap[2] = {
    (const void*)rodata_all, (const void*)(__data_CREvtModelMap + 0x140)
};
__declspec(section ".sdata2") __attribute__((used, aligned(8))) const unsigned char __sdata2_CREvtModelMap[0x10] = {
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x43,0x30,0x00,0x00,0x80,0x00,0x00,0x00
};
__attribute__((used, aligned(8))) unsigned char __sbss_CREvtModelMap[8];
DECOMP_FORCEACTIVE(CREvtModelMap_sbss, __sbss_CREvtModelMap);

extern "C" {

CREvtModelMap* __ct__CREvtModelMap(CREvtModelMap* self, void* parent)
{
    __ct__CREvtModel(self, parent, 1);
    *(u32**)self = (u32*)((void*)lbl_eu_80531D80);
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
    u32 out0, out1, out2, out3;
    Tok_Unpack(*(u32*)((char*)self->mPtr1C + 0x20), &out0, &out1, &out2, &out3);
    if (out0 == 1) {
        self->mIsGuest = 1;
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
        u32 w1, w2, w0;
        u32* src = (u32*)lbl_eu_80531D2C;
        w0 = *src++;
        w1 = *src++;
        self->mCallback[1] = w1;
        self->mCallback[0] = w0;
        w2 = *src++;
        self->mCallback[2] = w2;
        func_8016BC1C(self);
        if (*(s32*)((char*)self->mPtr1C + 0x30) != 0) {
            self->mFlags |= 0x30;
        }
    }
    return self;
}

CREvtModelMap* __ct__80180B00(CREvtModelMap* self, int dealloc)
{
    if (self) {
        *(u32**)self = (u32*)((void*)lbl_eu_80531D80);
        self->vtableIWork = (u8*)lbl_eu_80531D80 + 0x44;
        if (self->mIsGuest) {
            reinterpret_cast<CREvtModel*>(self)->setVisible(0);
            if (cf::CfGameManager::getGameSubManager()) {
                reinterpret_cast<cf::CfObject*>(cf::CfGameManager::getGameSubManager())->setPointEnabled(1);
            }
            enableLOD__8CTaskLODFv();
            if (self->mCreatureCount > 0) {
                s32 i = 0;
                CCreatureNode* head = (CCreatureNode*)((cf::CfGameManager*)spawnGimmickEntity__Q22cf13CfGameManagerFv())->field_0x4;
                CCreatureNode* it = head->next;
                while (it != (CCreatureNode*)((cf::CfGameManager*)spawnGimmickEntity__Q22cf13CfGameManagerFv())->field_0x4) {
                    u8 v = (u8)self->mModelName[i];
                    reinterpret_cast<cf::CfObject*>(it->obj)->setPointEnabled(!v);
                    it = it->next;
                    i++;
                }
            }
            lbl_eu_806642B0 = 0;
            if (getMapEffectManager__Q22cf13CfGameManagerFv()) {
                getMapEffectManager__Q22cf13CfGameManagerFv();
                MapFx_SetPointEnabled(1);
            }
        }
        reinterpret_cast<CREvtModel*>(self)->reset();
        restorePrimaryLOD__8CTaskLODFv();
        __ct__80172668(self, 0);
        if (dealloc > 0) {
            __dt__80185754(self);
        }
    }
    return self;
}

int MapModel_CheckMagicMA(void* self) {
    s8* info = *(s8**)((char*)self + 0x1C);
    return (info[0x10] == 0x6D && info[0x11] == 0x61) ? 1 : 0;
}

bool func_80180C88(CREvtModelMap* self)
{
    return self->mFileHandle1 || self->mFileHandle2 || self->mFileHandle3;
}

bool func_80180CBC(CREvtModelMap* self)
{
    if (self->mFlags & 0x80) {
        return 0;
    }
    func_801726DC(self);
    if (self->mEmoteModel) {
        Scn_IsAnimActiveOrNull(self->mEmoteModel);
        self->mEmoteModel = nullptr;
    }
    if (self->mLoadedModelData) {
        scnLgtRemoveLgtItem(*(void**)((u8*)lbl_eu_80663E14 + 0x7C));
        self->mLoadedModelData = nullptr;
    }
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
    if (self->mFileHandle1) {
        CDeviceFile::cancel(self->mFileHandle1);
    }
    if (self->mFileHandle2) {
        CDeviceFile::cancel(self->mFileHandle2);
    }
    if (self->mFileHandle3) {
        CDeviceFile::cancel(self->mFileHandle3);
    }
    self->mFileHandle1 = 0;
    self->mFileHandle2 = 0;
    self->mFileHandle3 = 0;
    return 1;
}

void MapModel_DisablePoints(CREvtModelMap* self)
{
    if (lbl_eu_806642B0) {
        return;
    }
    if (cf::CfGameManager::getGameSubManager()) {
        cf::CfGameManager* mgr = (cf::CfGameManager*)cf::CfGameManager::getGameSubManager();
        reinterpret_cast<cf::CfObject*>(mgr)->setPointEnabled(0);
        disableLOD__8CTaskLODFv();
    }
}

void func_80180E1C(CREvtModelMap* self)
{
    if (lbl_eu_806642B0) {
        return;
    }
    cf::CfGameManager* mgr = (cf::CfGameManager*)cf::CfGameManager::getGameSubManager();
    reinterpret_cast<cf::CfObject*>(mgr)->setPointEnabled(1);
    enableLOD__8CTaskLODFv();
}

void func_80180E60(CREvtModelMap* self)
{
    s32* pId = &self->mCreatureId;
    *pId = -1;
    if (func_8016BDA8(self, pId)) {
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

void func_80180EBC(CREvtModelMap* self)
{
    u32* tbl = (u32*)lbl_eu_80531D20;
    u32 flags = self->mFlags | 0x40;
    if (self->mIsGuest) {
        lbl_eu_806642B0 = self;
        flags |= 0x11;
        u32 w0 = tbl[9];
        u32 w1 = tbl[10];
        u32 w2 = tbl[11];
        self->mCallback[1] = w1;
        self->mCallback[0] = w0;
        self->mCallback[2] = w2;
        self->mFlags = flags;
        cf::CfGameManager* mgr = (cf::CfGameManager*)cf::CfGameManager::getGameSubManager();
        func_800AA33C(*(ml::FixStr<64>*)self->mBasePath, mgr->unk70, 1, 0);
        s32 len = strlen(&lbl_eu_8050372C[0]);
        strcat(self->mBasePath, &lbl_eu_8050372C[0]);
        self->mBasePathLen += len;
    } else {
        ml::FixStr<32> baseBuf;
        ml::FixStr<32> filePath;
        const char* base = &lbl_eu_8050372C[0];
        baseBuf = base + 5;
        filePath = baseBuf;
        filePath += ((CREvtModelMapNameInfo*)self->mPtr1C)->mName;
        filePath += base + 0xB;
        if (CDeviceFile::getFileSize(filePath.mString) > 0) {
            u32 innerFlags = ((CREvtModelMapNameInfo*)self->mPtr1C)->field_0x58;
            if ((innerFlags & 2) && !(innerFlags & 0x40)) {
                self->mFileHandle1 = CDeviceFile::readFile(
                    Scn_CallUnk8C_V9(lbl_eu_80663E14), filePath.mString,
                    (IWorkEvent*)(self ? (char*)self + 0x38 : (char*)self), 0, 0);
                addAllocHandle__11CDeviceFileFP11CFileHandleUl(self->mFileHandle1, mtl::MemManager::getHandleMEM2());
            } else {
                self->mFileHandle1 = CDeviceFile::readFile(
                    mtl::MemManager::getHandleMEM2(), filePath.mString,
                    (IWorkEvent*)(self ? (char*)self + 0x38 : (char*)self), 0, 0);
                addAllocHandle__11CDeviceFileFP11CFileHandleUl(self->mFileHandle1, Scn_CallUnk8C_V9(lbl_eu_80663E14));
            }
            if (EvtSeqGetStateBit5()) {
                addAllocHandle__11CDeviceFileFP11CFileHandleUl(self->mFileHandle1, getScnCounter__Fv());
            }
            addAllocHandle__11CDeviceFileFP11CFileHandleUl(self->mFileHandle1, mtl::MemManager::getHandleMEM1());
            if (((CREvtModelMapNameInfo*)self->mPtr1C)->field_0x58 & 1) {
                CDeviceFile::setHandleFlag1(self->mFileHandle1);
            }
            filePath = baseBuf;
            filePath += ((CREvtModelMapNameInfo*)self->mPtr1C)->mName;
            filePath += &lbl_eu_8050372C[0];
            if (CDeviceFile::getFileSize(filePath.mString) > 0) {
                u32 innerFlags = ((CREvtModelMapNameInfo*)self->mPtr1C)->field_0x58;
                if ((innerFlags & 2) && !(innerFlags & 0x40)) {
                    self->mFileHandle2 = CDeviceFile::readFile(
                        Scn_CallUnk8C_V9(lbl_eu_80663E14), filePath.mString,
                        (IWorkEvent*)(self ? (char*)self + 0x38 : (char*)self), 0, 0);
                    addAllocHandle__11CDeviceFileFP11CFileHandleUl(self->mFileHandle2, mtl::MemManager::getHandleMEM2());
                } else {
                    self->mFileHandle2 = CDeviceFile::readFile(
                        mtl::MemManager::getHandleMEM2(), filePath.mString,
                        (IWorkEvent*)(self ? (char*)self + 0x38 : (char*)self), 0, 0);
                    addAllocHandle__11CDeviceFileFP11CFileHandleUl(self->mFileHandle2, Scn_CallUnk8C_V9(lbl_eu_80663E14));
                }
                if (EvtSeqGetStateBit5()) {
                    addAllocHandle__11CDeviceFileFP11CFileHandleUl(self->mFileHandle2, getScnCounter__Fv());
                }
                addAllocHandle__11CDeviceFileFP11CFileHandleUl(self->mFileHandle2, mtl::MemManager::getHandleMEM1());
                if (((CREvtModelMapNameInfo*)self->mPtr1C)->field_0x58 & 1) {
                    CDeviceFile::setHandleFlag1(self->mFileHandle2);
                }
            }
            filePath = baseBuf;
            filePath += ((CREvtModelMapNameInfo*)self->mPtr1C)->mName;
            filePath += base + 0x10;
            if (CDeviceFile::getFileSize(filePath.mString) > 0) {
                u32 innerFlags = ((CREvtModelMapNameInfo*)self->mPtr1C)->field_0x58;
                if ((innerFlags & 2) && !(innerFlags & 0x40)) {
                    self->mFileHandle3 = CDeviceFile::readFile(
                        Scn_CallUnk8C_V9(lbl_eu_80663E14), filePath.mString,
                        (IWorkEvent*)(self ? (char*)self + 0x38 : (char*)self), 0, 0);
                    addAllocHandle__11CDeviceFileFP11CFileHandleUl(self->mFileHandle3, mtl::MemManager::getHandleMEM2());
                } else {
                    self->mFileHandle3 = CDeviceFile::readFile(
                        mtl::MemManager::getHandleMEM2(), filePath.mString,
                        (IWorkEvent*)(self ? (char*)self + 0x38 : (char*)self), 0, 0);
                    addAllocHandle__11CDeviceFileFP11CFileHandleUl(self->mFileHandle3, Scn_CallUnk8C_V9(lbl_eu_80663E14));
                }
                if (EvtSeqGetStateBit5()) {
                    addAllocHandle__11CDeviceFileFP11CFileHandleUl(self->mFileHandle2, getScnCounter__Fv());
                }
                addAllocHandle__11CDeviceFileFP11CFileHandleUl(self->mFileHandle3, mtl::MemManager::getHandleMEM1());
                if (((CREvtModelMapNameInfo*)self->mPtr1C)->field_0x58 & 1) {
                    CDeviceFile::setHandleFlag1(self->mFileHandle3);
                }
            }
            self->mFlags |= 0x2;
            u32 w0 = tbl[12];
            u32 w1 = tbl[13];
            u32 w2 = tbl[14];
            self->mCallback[1] = w1;
            self->mCallback[0] = w0;
            self->mCallback[2] = w2;
        } else {
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

void func_8018140C(CREvtModelMap* self)
{
    if (self->mFlags & 2) {
        if (!self->mFileHandle1 && self->mFileData1 && !self->mEmoteModel) {
            self->mEmoteModel = (CScnItemModel*)Scn_SetupAnim(lbl_eu_80663E14, self->mFileData1, 7, 1);
            simSetFlag2OnTree(self->mEmoteModel, 0);
            simSetLeafDist7B0(self->mEmoteModel, lbl_eu_806678C0);
            reinterpret_cast<CScnItemModel*>(self->mEmoteModel)->flags7A8 |= 4;
            self->mVisible = 0;
            if (lbl_eu_806642B4 == self) {
                lbl_eu_806642B4 = 0;
            }
        }
        if (self->mFileHandle2 || self->mFileHandle3 || self->mFileHandle1) {
            if (!(self->mFlags & 4)) {
                return;
            }
        }
        self->mFlags |= 0x11;
        u32* src = (u32*)lbl_eu_80531D68;
        u32 w0 = *src++;
        u32 w1 = *src++;
        self->mCallback[1] = w1;
        self->mCallback[0] = w0;
        u32 w2 = *src++;
        self->mCallback[2] = w2;
    } else {
        u32* src = (u32*)lbl_eu_80531D74;
        u32 w0 = *src++;
        u32 w1 = *src++;
        self->mCallback[1] = w1;
        self->mCallback[0] = w0;
        u32 w2 = *src++;
        self->mCallback[2] = w2;
    }
}

void func_8018152C(CREvtModelMap* self)
{
    if (self->mEmoteModel) {
        f32 fTime = (f32)(s32)EvtSeqGetCounter100();
        simRefreshFlag8(self->mEmoteModel, fTime);
    }
    reinterpret_cast<CREvtModel*>(self)->setVisible(REvtCam_IsField98Set());
    EvtSeqCheckEventRunGuard(self);
}

void func_801815AC(CREvtModelMap* self, unsigned int visible)
{
    if (self->mIsGuest) {
        cf::CfGameManager* mgr = (cf::CfGameManager*)cf::CfGameManager::getGameSubManager();
        if (mgr) {
            if (REvtCam_IsMode2B0Eq1()) {
                cf::CfGameManager::getGameSubManager()->field_0x100 &= ~4;
            } else if (visible) {
                cf::CfGameManager::getGameSubManager()->field_0x100 |= 4;
            }
        }
    }
    if (self->mVisible == visible) {
        goto set_guest_flags;
    }
    if (self->mIsGuest) {
        cf::CfGameManager* mgr = (cf::CfGameManager*)cf::CfGameManager::getGameSubManager();
        if (mgr) {
            reinterpret_cast<cf::CfObject*>(mgr)->setPointEnabled(visible);
        }
        if (visible) {
            restorePrimaryLOD__8CTaskLODFv();
            lbl_eu_806642B4 = self;
        } else {
            if (lbl_eu_806642B4 == self) {
                disableLOD__8CTaskLODFv();
                lbl_eu_806642B4 = 0;
            }
        }
        if (getMapEffectManager__Q22cf13CfGameManagerFv()) {
            getMapEffectManager__Q22cf13CfGameManagerFv();
            MapFx_SetPointEnabled(visible);
        }
        s32 i = 0;
        CCreatureNode* head = (CCreatureNode*)((cf::CfGameManager*)spawnGimmickEntity__Q22cf13CfGameManagerFv())->field_0x4;
        CCreatureNode* it = head->next;
        while (it != (CCreatureNode*)((cf::CfGameManager*)spawnGimmickEntity__Q22cf13CfGameManagerFv())->field_0x4) {
            if (!visible) {
                int v = reinterpret_cast<cf::CfObject*>(it->obj)->CfObject_checkSubReady();
                self->mModelName[i] = (char)v;
                self->mCreatureCount++;
                reinterpret_cast<cf::CfObject*>(it->obj)->setPointEnabled(0);
            } else {
                u8 v = (u8)self->mModelName[i];
                reinterpret_cast<cf::CfObject*>(it->obj)->setPointEnabled(!v);
            }
            it = it->next;
            i++;
        }
        if (visible) {
            self->mCreatureCount = 0;
        }
        goto visible_store;
    }
    if (self->mEmoteModel) {
        if (visible) {
            if (self->mFileData2) {
                acquireLODResource__8CTaskLODFv(self->mFileData2, 1);
            }
            if (self->mFileData3) {
                self->mLoadedModelData = func_804C1BA0(*(void**)((u8*)lbl_eu_80663E14 + 0x7C), self->mFileData3, 7);
                cf::CfGameManager* m2 = (cf::CfGameManager*)cf::CfGameManager::getGameSubManager();
                if (m2 && *(void**)((u8*)m2 + 0x2F3C)) {
                    CScnEnvLgtCtrlListItem* fx = (CScnEnvLgtCtrlListItem*)(*(void**)((u8*)cf::CfGameManager::getGameSubManager() + 0x2F3C));
                    fx->v24(0);
                }
            }
            lbl_eu_806642B4 = self;
        } else {
            if (lbl_eu_806642B4 == self) {
                disableLOD__8CTaskLODFv();
                lbl_eu_806642B4 = 0;
            }
            if (self->mLoadedModelData) {
                scnLgtRemoveLgtItem(*(void**)((u8*)lbl_eu_80663E14 + 0x7C));
                self->mLoadedModelData = 0;
                cf::CfGameManager* m2 = (cf::CfGameManager*)cf::CfGameManager::getGameSubManager();
                if (m2 && *(void**)((u8*)m2 + 0x2F3C)) {
                    CScnEnvLgtCtrlListItem* fx = (CScnEnvLgtCtrlListItem*)(*(void**)((u8*)cf::CfGameManager::getGameSubManager() + 0x2F3C));
                    fx->v24(1);
                }
            }
        }
        simSetFlag2OnTree(self->mEmoteModel, visible);
    }
visible_store:
    self->mVisible = (u8)visible;
set_guest_flags:
    if (self->mIsGuest) {
        s32 isZero = (REvtCam_IsMode2B0Eq2() == 0);
        UnkClass_80083298* m3 = cf::CfGameManager::getGameSubManager();
        if (isZero) {
            m3->field_0x100 |= 0x10;
        } else {
            m3->field_0x100 &= ~0x10;
        }
    }
}

void func_801818BC(CREvtModelMap* self, int visible)
{
    if (self->mIsGuest) {
        if (cf::CfGameManager::getGameSubManager()) {
            cf::CfGameManager* mgr = (cf::CfGameManager*)cf::CfGameManager::getGameSubManager();
            reinterpret_cast<cf::CfObject*>(mgr)->setPointEnabled(visible);
        }
        if (getGlobalSda()) {
            *(u8*)((u8*)getGlobalSda() + 0xA8) = visible;
        }
        u8* taskState = (u8*)(*(void**)((u8*)lbl_eu_80663E14 + 0x78));
        taskState[0x28] = visible;
    } else if (self->mEmoteModel) {
        reinterpret_cast<CScnItemModel*>(self->mEmoteModel)->vfuncB4(REvtCam_IsField98Set());
    }
}

int MapModel_VisFlag3(void* self) { return (*(u8*)((char*)self + 0x3D) != 0) ? 0 : 3; }

bool func_80181988(CREvtModelMap* self, CEventFile* pEvent)
{
    if (pEvent->mFileHandle == self->mFileHandle1) {
        if (pEvent->unk0 == 1) {
            self->mFileData1 = self->mFileHandle1->getData();
        } else {
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

bool OnFileEvent__13CREvtModelMapFP10CEventFile(IWorkEvent* self, CEventFile* ev)
{
    return func_80181988(reinterpret_cast<CREvtModelMap*>(reinterpret_cast<char*>(self) - 0x38), ev);
}

void MapModel_ThunkCtor38(void* self, int dealloc) {
    __ct__80180B00((CREvtModelMap*)((char*)self - 0x38), dealloc);
}

} // extern "C"
