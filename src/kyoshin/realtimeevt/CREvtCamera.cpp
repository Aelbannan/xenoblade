// Auto-scaffolded catalog TU for kyoshin/realtimeevt/CREvtCamera
// High-level C/C++ reconstruction.

#include "kyoshin/realtimeevt/CREvtCamera.hpp"
#include "monolib/lod/CTaskLOD.hpp"
#include "monolib/math/CVec4.hpp"
#include "kyoshin/cf/CfGameManager.hpp"
#include "kyoshin/cf/CTaskREvtSequence.hpp"


// External function declarations
extern "C" {
    UnkClass_800821F8* getCameraDataBlock__Q22cf13CfGameManagerFv();
    void cfCam_setClear04(void* mgr, u32 mask, int flag);
    CScnItemModel* Scn_SetupAnim(u32 global, u8* handle, int flag, int flag2);
    bool Scn_IsAnimActiveOrNull(void* ptr);
    void* Scn_InitGlobalA(void* mgr, void* param, int flag);
    void ScnCamLoadMtx(CREvtCamObj* camObj, const f32 mtx[3][4]);
    CREvtCamObj* Scn_FindCamItem(u32 mgr, int a);
    float simGetLeafAnimDist2(void* obj);
    void simBindChrAnim(void* obj, void* param, int a, int b, int c, int d, int e);
    u32 evtGetManagerAddr();
    u32 REvtCam_IsTaskListEmpty();
    u8 REvtCam_GetFieldA5Flag();
    u32 REvtCam_IsField98Set();
    u32 REvtCam_IsMode2B0Eq1();
    u32 REvtCam_IsMode2B0Eq2();
    int atoi(const char* str);
}

CREvtCamera* cf::CREvtCamera::sInstance;

// name-guess: retail .sdata pointer at 0x80662448 targets the "camAtr" name.
const char* sCamAtrAnimName = "camAtr";
// name-guess: retail .sdata pointer at 0x8066244C targets the "cam" name.
const char* sCamAnimName = "cam";

// ============================================================================
// Constructor (0x80181374, size 0x110): base CREvtObj(0) (camera kind).
// ============================================================================
cf::CREvtCamera::CREvtCamera(u32 param)
    : CREvtObj(0), mField14(0), mField18(param), mField1C(0), mField20(0), mField24(0),
      mField88(0), mField8C(30.0f), mField90(0), mField94(0), mField98(0),
      mField9C(0), mFieldA0(0), mFieldA4(0), mFieldA5(0), mTaskCount2A8(0), mTaskCount2AC(0),
      mField2B0(0), mTaskCount434(0), mField438(0), mField43C(-1), mField440(0.1f),
      mField444(85000.0f) {
    // Mirror the game manager's camera settings into this camera. Each
    // getCameraDataBlock() call is a separate retrieval (MWCC cannot CSE calls).
    if (getCameraDataBlock__Q22cf13CfGameManagerFv()) {
        if (((CREvtCamManager*)getCameraDataBlock__Q22cf13CfGameManagerFv())->field_0x0C) {
            mField8C = ((CREvtCamManager*)getCameraDataBlock__Q22cf13CfGameManagerFv())->getFov();
            mField440 = ((CREvtPlayerObj*)((CREvtCamManager*)getCameraDataBlock__Q22cf13CfGameManagerFv())->field_0x0C)->field_0x1EC;
            mField444 = ((CREvtPlayerObj*)((CREvtCamManager*)getCameraDataBlock__Q22cf13CfGameManagerFv())->field_0x0C)->field_0x1F0;
        }
    }

    CREvtCamera::sInstance = this;
}

// ============================================================================
// Destructor (0x80181484, size 0x188): clears the singleton, hands the
// camera settings back to the game manager, releases the animation handle
// and scene items; the CREvtObj base dtor and class operator delete follow
// implicitly.
// ============================================================================
cf::CREvtCamera::~CREvtCamera() {
    // Declared up front (not in the nested block): block-scoped locals add
    // redundant destroy-base ranges to the retail exception table.
    f32 z, y, x;
    CREvtCamera::sInstance = 0;

    // Each getCameraDataBlock() call is a separate retrieval (MWCC cannot CSE
    // function calls) - retail makes 3 calls before cfCam_setClear04.
    if (getCameraDataBlock__Q22cf13CfGameManagerFv()) {
        if (((CREvtCamManager*)getCameraDataBlock__Q22cf13CfGameManagerFv())->field_0x0C) {
            cfCam_setClear04(getCameraDataBlock__Q22cf13CfGameManagerFv(), 0x04000000, 0);

            if (mFieldA4) {
                // Mirror the camera position into the game manager.
                // Block-local temps reproduce retail's load-all/store-all
                // scheduling (z/y/x declaration order -> retail's reverse
                // load order into f0/f1/f2).
                z = mMatrix58[2][3];
                y = mMatrix58[1][3];
                x = mMatrix58[0][3];
                f32 pos[3] = { x, y, z };
                ((CREvtCamManager*)getCameraDataBlock__Q22cf13CfGameManagerFv())->setPosition(pos);
            }

            ((CREvtCamManager*)getCameraDataBlock__Q22cf13CfGameManagerFv())->setFov(mField8C);

            // Restore the player object's stored fields. Declaring both
            // locals first then assigning keeps MWCC's f30/f31 allocation
            // on declaration order while emitting the loads in assignment
            // order (0x444 before 0x440, matching retail).
            f32 f31, f30;
            f30 = mField444;
            f31 = mField440;
            CREvtPlayerObj* player =
                (CREvtPlayerObj*)((CREvtCamManager*)getCameraDataBlock__Q22cf13CfGameManagerFv())->field_0x0C;
            player->field_0x1EC = f31;
            player->field_0x1F0 = f30;
            ScnCamDepthUp((CScnItemCamera*)player);
        }
    }

    // vtable+0x10: release the animation handle (vfunc_08).
    vfunc_08();

    if (mField20) {
        if (gmScene) Scn_IsAnimActiveOrNull((void*)mField20);
        mField20 = 0;
    }
    if (mField1C) {
        if (gmScene) Scn_IsAnimActiveOrNull((void*)mField1C);
        mField1C = 0;
    }
}

// ============================================================================
// REvtCam_ActivateTasks (0x8018160C, size 0x184)
// Registers/activates tasks from the task-data camera into the global
// camera, or (when taskData == 0) syncs the registered tasks' activity.
// All reads go through the global singleton (CREvtCamera::sInstance) since the
// CTaskLOD calls may modify it.
// ============================================================================
extern "C" void REvtCam_ActivateTasks(int taskData) {
    if (!CREvtCamera::sInstance) return;

    if (taskData) {
        for (s32 i = 0; i < (s32)CREvtCamera::sInstance->mTaskCount2A8; i++) {
            s16 taskID = CREvtCamera::sInstance->mTaskArrayA6[i];
            u8 isActive = CTaskLOD::getLODData(taskID);
            s32 found = 0;
            s32 count = (s32)CREvtCamera::sInstance->mTaskCount434;
            for (s32 j = 0; j < count; j++) {
                if (CREvtCamera::sInstance->mTaskArray334[j] == taskID) {
                    found = 1;
                    break;
                }
            }
            if (!found) {
                CREvtCamera::sInstance->mFlagArray2B4[CREvtCamera::sInstance->mTaskCount434] = isActive;
                CREvtCamera::sInstance->mTaskArray334[CREvtCamera::sInstance->mTaskCount434] = taskID;
                CREvtCamera::sInstance->mTaskCount434++;
            }
            if (isActive) CTaskLOD::deactivateLOD(taskID);
        }
        CREvtCamera::sInstance->mField438 = 1;
    } else {
        if (CREvtCamera::sInstance->mField438) {
            for (s32 i = 0; i < (s32)CREvtCamera::sInstance->mTaskCount434; i++) {
                s16 taskID = CREvtCamera::sInstance->mTaskArray334[i];
                u8 isActive = CTaskLOD::getLODData(taskID);
                if (CREvtCamera::sInstance->mFlagArray2B4[i]) {
                    if (!isActive) CTaskLOD::activateLOD(taskID);
                } else {
                    if (isActive) CTaskLOD::deactivateLOD(taskID);
                }
            }
            CREvtCamera::sInstance->mField438 = 0;
        }
    }
}

// ============================================================================
// REvtCam_ActivateLodTasks (0x80181790, size 0x80)
// Activates every task in the global camera's mTaskArray1A6 list.
// ============================================================================
extern "C" void REvtCam_ActivateLodTasks() {
    if (!CREvtCamera::sInstance) return;
    for (s32 i = 0; i < (s32)CREvtCamera::sInstance->mTaskCount2AC; i++) {
        s16 taskID = CREvtCamera::sInstance->mTaskArray1A6[i];
        CTaskLOD::activateLOD(taskID);
        CTaskLOD::attachLODObject(taskID, 1);
    }
}

// ============================================================================
// REvtCam_SetupSceneModel (0x80181810, size 0xB8)
// Loads the camera scene model from static file data and grabs the two
// animation handles ("Cam" matrices) from it.
// ============================================================================
void cf::CREvtCamera::vfunc_04() {
    u8* handle;
    getStaticFileData__14CLibStaticDataFPCcP16StaticDataHandlePUl(lbl_eu_805036D8, &handle, 0);
    if (handle) {
        CScnItemModel* obj = Scn_SetupAnim((u32)gmScene, handle, -1, 1);
        mField1C = (u32)obj;
        simSetFlag2000Chain(obj, 1);
        simSetLeafDist7B0((CScnItemModel*)mField1C, lbl_eu_806678B0);
        ((CScnItemModel*)mField1C)->flags7A8 |= 4;
        mField90 = (u32)((CScnItemModel*)mField1C)->vfunc3C(sCamAtrAnimName);
        mField94 = (u32)((CScnItemModel*)mField1C)->vfunc3C(sCamAnimName);
    }
}

// Reproduces retail's unsigned-int -> double conversion, which references
// the float-pool constant lbl_eu_806678B8 (0x4330000080000000): store the
// 0x4330 high word and the sign-toggled value, reload as a double, subtract.
static f32 ConvU32ToTime(u32 v) {
    struct {
        u32 hi;
        u32 lo;
    } bits;
    bits.hi = 0x43300000;
    bits.lo = v ^ 0x80000000;
    f64 raw = *(const f64*)&bits;
    return (f32)(raw - lbl_eu_806678B8);
}

// ============================================================================
// REvtCam_UpdateFromAnim (0x801818C8, size 0x154)
// Updates the camera from the scene model's animation results: samples the
// "Cam" matrices at the current time, mirrors the camera position into the
// game manager, and applies the result to the active camera object.
// ============================================================================
void cf::CREvtCamera::vfunc_1C() {
    if (getCameraDataBlock__Q22cf13CfGameManagerFv()) {
        cfCam_setClear04(getCameraDataBlock__Q22cf13CfGameManagerFv(), 0x04000000, 1);
    }
    if (!mField1C) return;
    if (cf::CfGameManager::isSceneLoading()) return;

    CScnItemModel* sceneObj = (CScnItemModel*)mField1C;
    nw4r::g3d::ChrAnmResult* result =
        scnImN4AnimFn(sceneObj, sCamAnimName, ConvU32ToTime((u32)EvtSeqGetCounter100()));
    result->GetRotTrans((nw4r::math::MTX34*)mMatrix58);
    mFieldA4 = 1;
    result = scnImN4AnimFn(sceneObj, sCamAtrAnimName, ConvU32ToTime((u32)EvtSeqGetCounter100()));
    result->GetRotTrans((nw4r::math::MTX34*)mMatrix28);

    CREvtCamObj* camObj = Scn_FindCamItem((u32)gmScene, -1);
    if (!camObj) return;

    if (getCameraDataBlock__Q22cf13CfGameManagerFv()) {
        ((CREvtCamManager*)getCameraDataBlock__Q22cf13CfGameManagerFv())->setFov(mMatrix28[2][3]);
    }
    camObj->field_0x1E0 = mMatrix28[2][3];
    ScnCamDepthUp((CScnItemCamera*)camObj);
    camObj->field_0x1EC = mMatrix28[0][3];
    camObj->field_0x1F0 = mMatrix28[1][3];
    ScnCamDepthUp((CScnItemCamera*)camObj);
ScnCamLoadMtx((CREvtCamObj*)camObj, (const f32 (*)[4])mMatrix58);
    mFieldA4 = 1;
}

// ============================================================================
// REvtCam_ClearAnimHandle (0x80181A1C, size 0x44)
// ============================================================================
void cf::CREvtCamera::vfunc_08() {
    if (mField24) {
        Scn_IsAnimActiveOrNull((void*)mField24);
        mField24 = 0;
    }
}

// ============================================================================
// REvtCam_SetupEvent (0x80181A60, size 0x2DC)
// ============================================================================
extern "C" void REvtCam_SetupEvent(CREvtCamera* self, void* eventData, void* someParam) {
    // Mirror the event-file descriptor's flag word into mFieldA5.
    if (self->mField88) {
        self->mFieldA5 = self->mField88->field_04 != 0;
    } else {
        self->mFieldA5 = 0;
    }

    self->mTaskCount2A8 = 0;
    self->mTaskCount2AC = 0;
    if (self->mField1C) {
        self->mField98 = 0;
        self->mField9C = 0;
        self->mFieldA0 = 0;
        if (self->mField20) {
            // vtable+0x10: release the animation handle (vfunc_08).
            self->vfunc_08();
            self->mField24 = self->mField20;
            self->mField20 = 0;
        }
        if (eventData) {
            void* taskObj = Scn_InitGlobalA((void*)gmScene, eventData, 0);
            self->mField20 = (u32)taskObj;
            simBindChrAnim((void*)self->mField1C, taskObj, 0, 0, 0, 1, -1);
            self->mField43C = (s32)(u32)simGetLeafAnimDist2((void*)self->mField1C);
            EvtSeqResolveAnmChr((u8*)(uintptr_t)self->mField20, lbl_eu_805036D8 + 4, (s32*)&self->mField98);
            EvtSeqResolveAnmChr((u8*)(uintptr_t)self->mField20, lbl_eu_805036D8 + 15, (s32*)&self->mField9C);
            EvtSeqResolveAnmChr((u8*)(uintptr_t)self->mField20, lbl_eu_805036D8 + 23, (s32*)&self->mFieldA0);
            EvtSeqResolveAnmChr((u8*)(uintptr_t)self->mField20, lbl_eu_805036D8 + 30, (s32*)&self->mField2B0);

            if (!CREvtCamera::sInstance->mFieldA5) {
                ((CREvtCamTaskEvent*)(uintptr_t)evtGetManagerAddr())->field_1B8 = 1;
            }

            const char* timeStr;
            u32* taskBuf;
            s32 taskCount;
            if (EvtSeqFindResAddr((u8*)(uintptr_t)self->mField20, lbl_eu_805036D8 + 38, (s32*)&timeStr)) {
                int val = atoi(timeStr);
                u16 v = (u16)val;
                if ((u32)(u16)(v / 100) == 99 && (u32)(u16)(v % 100) == 99) {
                    EvtSeqPublishIdHalfwords();
                } else {
                    func_80086B5C__Q22cf13CfGameManagerFv((u32)(u16)(v / 100), (u32)(u16)(v % 100), 0);
                    Env_SetClock((CTaskEnvTime*)getGlobalSda(), (int)(u32)(u16)(v / 100), (int)(u32)(u16)(v % 100), 0);
                }
            }

            // The two task-id lists reuse the same stack locals (retail
            // reloads taskBuf/taskCount from 8(sp)/12(sp) for both loops).
            if (EvtSeqResolveAnmChr2((u8*)(uintptr_t)self->mField20, lbl_eu_805036D8 + 43, (s32*)&taskBuf, &taskCount)) {
                for (s32 i = 0; i < taskCount; i++) {
                    self->mTaskArrayA6[self->mTaskCount2A8] = (s16)taskBuf[i];
                    self->mTaskCount2A8++;
                }
            }

            if (EvtSeqResolveAnmChr2((u8*)(uintptr_t)self->mField20, lbl_eu_805036D8 + 51, (s32*)&taskBuf, &taskCount)) {
                for (s32 i = 0; i < taskCount; i++) {
                    self->mTaskArray1A6[self->mTaskCount2AC] = (s16)taskBuf[i];
                    self->mTaskCount2AC++;
                }
            }
        }
    }
    self->mField88 = (CREvtCamFile*)someParam;
}

// ============================================================================
// REvtCam_IsTaskListEmpty (0x80181D3C, size 0x14) - already implemented
// ============================================================================
u32 REvtCam_IsTaskListEmpty() {
    return CREvtCamera::sInstance->mFieldA0 ? 0 : 1;
}

// ============================================================================
// REvtCam_GetFieldA5Flag (0x80181D50, size 0xC)
// ============================================================================
u8 REvtCam_GetFieldA5Flag() {
    return CREvtCamera::sInstance->mFieldA5;
}

// ============================================================================
// REvtCam_IsField98Set (0x80181D5C, size 0x18)
// ============================================================================
u32 REvtCam_IsField98Set() {
    return CREvtCamera::sInstance->mField98 == 1 ? 1 : 0;
}

// ============================================================================
// REvtCam_IsMode2B0Eq1 (0x80181D74, size 0x18)
// ============================================================================
u32 REvtCam_IsMode2B0Eq1() {
    return CREvtCamera::sInstance->mField2B0 == 1 ? 1 : 0;
}

// ============================================================================
// REvtCam_IsMode2B0Eq2 (0x80181D8C, size 0x18)
// ============================================================================
u32 REvtCam_IsMode2B0Eq2() {
    return CREvtCamera::sInstance->mField2B0 == 2 ? 1 : 0;
}

// ============================================================================
// vfunc_14 (CREvtObj, 0x80181DA4): always true.
// ============================================================================
int cf::CREvtObj::vfunc_14() { return 1; }

// ============================================================================
// vfunc_0C (0x80181DAC): the scene model pointer.
// ============================================================================
int cf::CREvtCamera::vfunc_0C() { return mField1C; }
