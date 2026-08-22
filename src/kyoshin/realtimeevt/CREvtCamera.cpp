// Auto-scaffolded catalog TU for kyoshin/realtimeevt/CREvtCamera
// High-level C/C++ reconstruction.

#include "kyoshin/realtimeevt/CREvtCamera.hpp"
#include "kyoshin/cf/CfGameManager.hpp"
#include "nw4r/g3d/res/g3d_resanmchr.h"

// External function declarations
extern "C" {
    void __ct__cf_CREvtObj(CREvtCamera* self, u32 param);
    void __dt__Q22cf8CREvtObjFv(CREvtCamera* self, int deleteFlag);
    void __dt__80185754(CREvtCamera* self);
    CREvtCamManager* func_800821F8__Q22cf13CfGameManagerFv();
    void func_80086B5C__Q22cf13CfGameManagerFv(u32, u32, u32);
    void  func_8006BBF4(void* mgr, u32 mask, int flag);
    unsigned char func_80462E1C__8CTaskLODFv(s16 taskID);
    void func_80462D04__8CTaskLODFv(s16 taskID);
    void func_80462D5C__8CTaskLODFv(s16 taskID);
    void func_80462F4C__8CTaskLODFv(s16 taskID, int flag);
    int getStaticFileData__14CLibStaticDataFPCcP16StaticDataHandlePUl(const char*, u8**, u32*);
    CREvtSceneModel* func_80495E8C(u32 mgr, u8* handle, int flag, int flag2);
    void func_80495E60(void* ptr);
    void* func_80495EAC(void* mgr, void* param, int flag);
    void func_8049EB60();
    void func_8049F774(CREvtCamObj* camObj, const f32 mtx[3][4]);
    CREvtCamObj* func_80496264(u32 mgr, int a);
    void func_804827DC(CREvtSceneModel* obj, int flag);
    void func_80484E5C(CREvtSceneModel* obj, float val);
    float func_804850A4(void* obj);
    nw4r::g3d::ChrAnmResult* func_8048BAD4(CREvtSceneModel* model, const char* name, f32 time);
    int func_8016A35C();
    void* func_801644B4();
    void func_8016841C();
    void func_8016AF4C(void* obj, const char* name, u32* out);
    int func_8016B5A4(void* obj, const char* name, void* out);
    int func_8016B164(void* obj, const char* name, void* out1, void* out2);
    void func_804839D4(void* obj, void* param, int a, int b, int c, int d, int e);
    float func_804850A4(void* obj);
    void* getGlobalSda();
    void func_800599E0(void* sda, u32 a, u32 b, u32 c);
    int atoi(const char* str);
}

// External global data (declared in CREvtCamera.hpp)
// Global singleton accessor (MWCC has no lambdas)
static CREvtCamera* GetCamera() {
    return (CREvtCamera*)lbl_eu_806642A8;
}

// ============================================================================
// func_80180940 (0x80181D3C, size 0x14) - already implemented
// ============================================================================
extern "C" unsigned long func_80180940() {
    extern unsigned long lbl_eu_806642A8;
    unsigned long* ptr = (unsigned long*)lbl_eu_806642A8;
    unsigned long v = ptr[0xa0 / 4];
    return v ? 0 : 1;
}

// ============================================================================
// func_80180954 (0x80181D50, size 0xC)
// ============================================================================
extern "C" unsigned char func_80180954() {
    extern unsigned long lbl_eu_806642A8;
    return ((CREvtCamera*)lbl_eu_806642A8)->mFieldA5;
}

// ============================================================================
// func_80180960 (0x80181D5C, size 0x18)
// ============================================================================
extern "C" unsigned long func_80180960() {
    extern unsigned long lbl_eu_806642A8;
    return ((CREvtCamera*)lbl_eu_806642A8)->mField98 == 1 ? 1 : 0;
}

// ============================================================================
// func_80180978 (0x80181D74, size 0x18)
// ============================================================================
extern "C" unsigned long func_80180978() {
    extern unsigned long lbl_eu_806642A8;
    return ((CREvtCamera*)lbl_eu_806642A8)->mField2B0 == 1 ? 1 : 0;
}

// ============================================================================
// func_80180990 (0x80181D8C, size 0x18)
// ============================================================================
extern "C" unsigned long func_80180990() {
    extern unsigned long lbl_eu_806642A8;
    return ((CREvtCamera*)lbl_eu_806642A8)->mField2B0 == 2 ? 1 : 0;
}

// ============================================================================
// func_80180620 (0x80181A1C, size 0x44)
// ============================================================================
extern "C" void func_80180620(CREvtCamera* self) {
    if (self->mField24) {
        func_80495E60((void*)self->mField24);
        self->mField24 = 0;
    }
}

// ============================================================================
// func_80180394 (0x80181790, size 0x80)
// Activates every task in the global camera's mTaskArray1A6 list.
// ============================================================================
extern "C" void func_80180394(CREvtCamera* self) {
    extern unsigned long lbl_eu_806642A8;
    if (!lbl_eu_806642A8) return;
    for (s32 i = 0; i < (s32)((CREvtCamera*)lbl_eu_806642A8)->mTaskCount2AC; i++) {
        s16 taskID = ((CREvtCamera*)lbl_eu_806642A8)->mTaskArray1A6[i];
        func_80462D04__8CTaskLODFv(taskID);
        func_80462F4C__8CTaskLODFv(taskID, 1);
    }
}

// ============================================================================
// Constructor: __ct__CREvtCamera (0x80181374, size 0x110)
// ============================================================================
extern "C" CREvtCamera* __ct__CREvtCamera(CREvtCamera* self, u32 param) {
    __ct__cf_CREvtObj(self, 0);

    // Declaration/use split reproduces retail scheduling: the first default
    // float loads before the vtable address formation, the other two inside
    // the lis/@ha - addi/@l gap.
    f32 f2 = lbl_eu_806678A4;
    u8* vt = (u8*)lbl_eu_80531CE8;
    u32 zero = 0;
    f32 f1 = lbl_eu_806678A8;
    f32 f0 = lbl_eu_806678AC;
    self->vtable = vt;
    s32 neg1 = -1;

    self->mField14 = zero;
    self->mField18 = param;
    self->mField1C = zero;
    self->mField20 = zero;
    self->mField24 = zero;
    self->mField88 = (CREvtCamFile*)zero;
    self->mField8C = f2;
    self->mField90 = zero;
    self->mField94 = zero;
    self->mField98 = zero;
    self->mField9C = zero;
    self->mFieldA0 = zero;
    self->mFieldA4 = 0;
    self->mFieldA5 = 0;
    self->mTaskCount2A8 = zero;
    self->mTaskCount2AC = zero;
    self->mField2B0 = zero;
    self->mTaskCount434 = zero;
    self->mField438 = 0;
    self->mField43C = neg1;
    self->mField440 = f1;
    self->mField444 = f0;

    // Mirror the game manager's camera settings into this camera. Each
    // func_800821F8() call is a separate retrieval (MWCC cannot CSE calls).
    if (func_800821F8__Q22cf13CfGameManagerFv()) {
        if (func_800821F8__Q22cf13CfGameManagerFv()->field_0x0C) {
            self->mField8C = func_800821F8__Q22cf13CfGameManagerFv()->vfunc_0x58();
            self->mField440 = ((CREvtPlayerObj*)func_800821F8__Q22cf13CfGameManagerFv()->field_0x0C)->field_0x1EC;
            self->mField444 = ((CREvtPlayerObj*)func_800821F8__Q22cf13CfGameManagerFv()->field_0x0C)->field_0x1F0;
        }
    }

    extern unsigned long lbl_eu_806642A8;
    lbl_eu_806642A8 = (unsigned long)self;
    return self;
}

// ============================================================================
// Destructor: __ct__80180088 (0x80181484, size 0x188)
//
// Note: this retail symbol is really a destructor (vtable reset + cleanup +
// base-class dtor + operator delete). The whole body runs under an explicit
// `if (self)` guard so MWCC emits a single `beq` to the epilogue for the
// null check, matching retail.
// ============================================================================
extern "C" CREvtCamera* __ct__80180088(CREvtCamera* self, int deleteFlag) {
    if (self) {
        self->vtable = (u8*)lbl_eu_80531CE8;
        lbl_eu_806642A8 = 0;

        // Each func_800821F8() call is a separate retrieval (MWCC cannot CSE
        // function calls) - retail makes 3 calls before func_8006BBF4.
        if (func_800821F8__Q22cf13CfGameManagerFv()) {
            if (func_800821F8__Q22cf13CfGameManagerFv()->field_0x0C) {
                func_8006BBF4(func_800821F8__Q22cf13CfGameManagerFv(), 0x04000000, 0);

                if (self->mFieldA4) {
                    // Mirror the camera position into the game manager.
                    // Block-local temps reproduce retail's load-all/store-all
                    // scheduling (z/y/x declaration order -> retail's reverse
                    // load order into f0/f1/f2).
                    f32 z = self->mMatrix58[2][3];
                    f32 y = self->mMatrix58[1][3];
                    f32 x = self->mMatrix58[0][3];
                    f32 pos[3] = { x, y, z };
                    func_800821F8__Q22cf13CfGameManagerFv()->vfunc_0x14(pos);
                }

                func_800821F8__Q22cf13CfGameManagerFv()->vfunc_0x3C(self->mField8C);

                // Restore the player object's stored fields. Declaring both
                // locals first then assigning keeps MWCC's f30/f31 allocation
                // on declaration order while emitting the loads in assignment
                // order (0x444 before 0x440, matching retail).
                f32 f31, f30;
                f30 = self->mField444;
                f31 = self->mField440;
                CREvtPlayerObj* player =
                    (CREvtPlayerObj*)func_800821F8__Q22cf13CfGameManagerFv()->field_0x0C;
                player->field_0x1EC = f31;
                player->field_0x1F0 = f30;
                func_8049EB60();
            }
        }

        // vtable+0x10: virtual cleanup callback on self. Casting through
        // CREvtCamManager (vtable slot 0x10 = 3rd declared virtual) makes
        // MWCC emit the r12 two-step virtual call matching retail.
        ((CREvtCamManager*)self)->vfunc_0x10();

        if (self->mField20) {
            if (lbl_eu_80663E14) func_80495E60((void*)self->mField20);
            self->mField20 = 0;
        }
        if (self->mField1C) {
            if (lbl_eu_80663E14) func_80495E60((void*)self->mField1C);
            self->mField1C = 0;
        }

        __dt__Q22cf8CREvtObjFv(self, 0);
        if (deleteFlag > 0) __dt__80185754(self);
    }
    return self;
}

// ============================================================================
// func_80180210 (0x8018160C, size 0x184)
// Registers/activates tasks from the task-data camera into the global
// camera, or (when taskData == 0) syncs the registered tasks' activity.
// All reads go through the global singleton (lbl_eu_806642A8) since the
// CTaskLOD calls may modify it.
// ============================================================================
extern "C" void func_80180210(CREvtCamera* taskData) {
    extern unsigned long lbl_eu_806642A8;
    if (!lbl_eu_806642A8) return;

    if (taskData) {
        for (s32 i = 0; i < (s32)((CREvtCamera*)lbl_eu_806642A8)->mTaskCount2A8; i++) {
            s16 taskID = ((CREvtCamera*)lbl_eu_806642A8)->mTaskArrayA6[i];
            u8 isActive = func_80462E1C__8CTaskLODFv(taskID);
            s32 found = 0;
            s32 count = (s32)((CREvtCamera*)lbl_eu_806642A8)->mTaskCount434;
            for (s32 j = 0; j < count; j++) {
                if (((CREvtCamera*)lbl_eu_806642A8)->mTaskArray334[j] == taskID) {
                    found = 1;
                    break;
                }
            }
            if (!found) {
                ((CREvtCamera*)lbl_eu_806642A8)->mFlagArray2B4[((CREvtCamera*)lbl_eu_806642A8)->mTaskCount434] = isActive;
                ((CREvtCamera*)lbl_eu_806642A8)->mTaskArray334[((CREvtCamera*)lbl_eu_806642A8)->mTaskCount434] = taskID;
                ((CREvtCamera*)lbl_eu_806642A8)->mTaskCount434++;
            }
            if (isActive) func_80462D5C__8CTaskLODFv(taskID);
        }
        ((CREvtCamera*)lbl_eu_806642A8)->mField438 = 1;
    } else {
        if (((CREvtCamera*)lbl_eu_806642A8)->mField438) {
            for (s32 i = 0; i < (s32)((CREvtCamera*)lbl_eu_806642A8)->mTaskCount434; i++) {
                s16 taskID = ((CREvtCamera*)lbl_eu_806642A8)->mTaskArray334[i];
                u8 isActive = func_80462E1C__8CTaskLODFv(taskID);
                if (((CREvtCamera*)lbl_eu_806642A8)->mFlagArray2B4[i]) {
                    if (!isActive) func_80462D04__8CTaskLODFv(taskID);
                } else {
                    if (isActive) func_80462D5C__8CTaskLODFv(taskID);
                }
            }
            ((CREvtCamera*)lbl_eu_806642A8)->mField438 = 0;
        }
    }
}

// ============================================================================
// func_80180414 (0x80181810, size 0xB8)
// Loads the camera scene model from static file data and grabs the two
// animation handles ("Cam" matrices) from it.
// ============================================================================
extern "C" void func_80180414(CREvtCamera* self) {
    u8* handle;
    getStaticFileData__14CLibStaticDataFPCcP16StaticDataHandlePUl(lbl_eu_805036D8, &handle, 0);
    if (handle) {
        CREvtSceneModel* obj = func_80495E8C(lbl_eu_80663E14, handle, -1, 1);
        self->mField1C = (u32)obj;
        func_804827DC(obj, 1);
        func_80484E5C((CREvtSceneModel*)self->mField1C, lbl_eu_806678B0);
        ((CREvtSceneModel*)self->mField1C)->field_0x7A8 |= 4;
        self->mField90 = ((CREvtSceneModel*)self->mField1C)->vfunc_0x3C(lbl_eu_80662448);
        self->mField94 = ((CREvtSceneModel*)self->mField1C)->vfunc_0x3C(lbl_eu_8066244C);
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
// func_801804CC (0x801818C8, size 0x154)
// Updates the camera from the scene model's animation results: samples the
// "Cam" matrices at the current time, mirrors the camera position into the
// game manager, and applies the result to the active camera object.
// ============================================================================
extern "C" void func_801804CC(CREvtCamera* self) {
    if (func_800821F8__Q22cf13CfGameManagerFv()) {
        func_8006BBF4(func_800821F8__Q22cf13CfGameManagerFv(), 0x04000000, 1);
    }
    if (!self->mField1C) return;
    if (cf::CfGameManager::func_800829B8()) return;

    CREvtSceneModel* sceneObj = (CREvtSceneModel*)self->mField1C;
    nw4r::g3d::ChrAnmResult* result =
        func_8048BAD4(sceneObj, lbl_eu_8066244C, ConvU32ToTime((u32)func_8016A35C()));
    result->GetRotTrans((nw4r::math::MTX34*)self->mMatrix58);
    self->mFieldA4 = 1;
    result = func_8048BAD4(sceneObj, lbl_eu_80662448, ConvU32ToTime((u32)func_8016A35C()));
    result->GetRotTrans((nw4r::math::MTX34*)self->mMatrix28);

    CREvtCamObj* camObj = func_80496264(lbl_eu_80663E14, -1);
    if (!camObj) return;

    if (func_800821F8__Q22cf13CfGameManagerFv()) {
        func_800821F8__Q22cf13CfGameManagerFv()->vfunc_0x3C(self->mMatrix28[2][3]);
    }
    camObj->field_0x1E0 = self->mMatrix28[2][3];
    func_8049EB60();
    camObj->field_0x1EC = self->mMatrix28[0][3];
    camObj->field_0x1F0 = self->mMatrix28[1][3];
    func_8049EB60();
    func_8049F774(camObj, self->mMatrix58);
    self->mFieldA4 = 1;
}

// ============================================================================
// func_80180664 (0x80181A60, size 0x2DC)
// ============================================================================
extern "C" void func_80180664(CREvtCamera* self, void* eventData, void* someParam) {
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
            // vtable+0x10: virtual cleanup callback (r12 two-step).
            ((CREvtCamManager*)self)->vfunc_0x10();
            self->mField24 = self->mField20;
            self->mField20 = 0;
        }
        if (eventData) {
            void* taskObj = func_80495EAC((void*)lbl_eu_80663E14, eventData, 0);
            self->mField20 = (u32)taskObj;
            func_804839D4((void*)self->mField1C, taskObj, 0, 0, 0, 1, -1);
            self->mField43C = (s32)(u32)func_804850A4((void*)self->mField1C);
            func_8016AF4C((void*)self->mField20, lbl_eu_805036D8 + 4, &self->mField98);
            func_8016AF4C((void*)self->mField20, lbl_eu_805036D8 + 15, &self->mField9C);
            func_8016AF4C((void*)self->mField20, lbl_eu_805036D8 + 23, &self->mFieldA0);
            func_8016AF4C((void*)self->mField20, lbl_eu_805036D8 + 30, &self->mField2B0);

            if (!((CREvtCamera*)lbl_eu_806642A8)->mFieldA5) {
                ((CREvtCamTaskEvent*)func_801644B4())->field_1B8 = 1;
            }

            const char* timeStr;
            u32* taskBuf;
            s32 taskCount;
            if (func_8016B5A4((void*)self->mField20, lbl_eu_805036D8 + 38, &timeStr)) {
                int val = atoi(timeStr);
                u16 v = (u16)val;
                if ((u32)(u16)(v / 100) == 99 && (u32)(u16)(v % 100) == 99) {
                    func_8016841C();
                } else {
                    func_80086B5C__Q22cf13CfGameManagerFv((u32)(u16)(v / 100), (u32)(u16)(v % 100), 0);
                    func_800599E0(getGlobalSda(), (u32)(u16)(v / 100), (u32)(u16)(v % 100), 0);
                }
            }

            // The two task-id lists reuse the same stack locals (retail
            // reloads taskBuf/taskCount from 8(sp)/12(sp) for both loops).
            if (func_8016B164((void*)self->mField20, lbl_eu_805036D8 + 43, &taskBuf, &taskCount)) {
                for (s32 i = 0; i < taskCount; i++) {
                    self->mTaskArrayA6[self->mTaskCount2A8] = (s16)taskBuf[i];
                    self->mTaskCount2A8++;
                }
            }

            if (func_8016B164((void*)self->mField20, lbl_eu_805036D8 + 51, &taskBuf, &taskCount)) {
                for (s32 i = 0; i < taskCount; i++) {
                    self->mTaskArray1A6[self->mTaskCount2AC] = (s16)taskBuf[i];
                    self->mTaskCount2AC++;
                }
            }
        }
    }
    self->mField88 = (CREvtCamFile*)someParam;
}