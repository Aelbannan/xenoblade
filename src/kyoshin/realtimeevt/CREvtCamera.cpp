// Auto-scaffolded catalog TU for kyoshin/realtimeevt/CREvtCamera
// High-level C/C++ reconstruction.

#include "kyoshin/realtimeevt/CREvtCamera.hpp"
#include "kyoshin/cf/CfGameManager.hpp"

// External function declarations
extern "C" {
    void __ct__cf_CREvtObj(CREvtCamera* self, u32 param);
    void __dt__Q22cf8CREvtObjFv(CREvtCamera* self, int deleteFlag);
    void __dt__80185754(CREvtCamera* self);
    void* func_800821F8__Q22cf13CfGameManagerFv();
    void func_80086B5C__Q22cf13CfGameManagerFv(u32, u32, u32);
    void  func_8006BBF4(void* mgr, u32 mask, int flag);
    unsigned char func_80462E1C__8CTaskLODFv(s16 taskID);
    void func_80462D04__8CTaskLODFv(s16 taskID);
    void func_80462D5C__8CTaskLODFv(s16 taskID);
    void func_80462F4C__8CTaskLODFv(s16 taskID, int flag);
    int getStaticFileData__14CLibStaticDataFPCcP16StaticDataHandlePUl(const char*, void*, unsigned long*);
    void* func_80495E8C(void* mgr, int a, int b);
    void func_80495E60(void* ptr);
    void* func_80495EAC(void* mgr, void* param, int flag);
    void func_8049EB60();
    void func_8049F774(void* obj, void* matrix);
    void* func_80496264(void* mgr, int a);
    void func_804827DC(void* obj, int flag);
    void func_80484E5C(void* obj, float val);
    float func_804850A4(void* obj);
    void* func_8048BAD4(void* obj, void* name);
    void GetRotTrans__Q34nw4r3g3d12ChrAnmResultCFPQ34nw4r4math5MTX34(void* result, void* mtx);
    int func_8016A35C();
    void* func_801644B4();
    void func_8016841C();
    void func_8016AF4C(void* obj, const char* name, u32* out);
    int func_8016B5A4(void* obj, const char* name, void* out);
    int func_8016B164(void* obj, const char* name, void* out1, void* out2);
    void func_804839D4(void* obj, void* param, int a, int b, int c, int d, int e, int f);
    float func_804850A4(void* obj);
    void* getGlobalSda();
    void func_800599E0(void* sda, u32 a, u32 b, u32 c);
    int atoi(const char* str);
}

// External global data
extern u32 lbl_eu_80663E14;
extern void* lbl_eu_80662448;
extern void* lbl_eu_8066244C;
extern const char lbl_eu_805036D8[];
extern float lbl_eu_806678A4;
extern float lbl_eu_806678A8;
extern float lbl_eu_806678AC;
extern float lbl_eu_806678B0;

// Global singleton accessor (MWCC has no lambdas)
static CREvtCamera* GetCamera() {
    extern unsigned long lbl_eu_806642A8;
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
// ============================================================================
extern "C" void func_80180394(CREvtCamera* self) {
    extern unsigned long lbl_eu_806642A8;
    if (!lbl_eu_806642A8) return;
    CREvtCamera* global = (CREvtCamera*)lbl_eu_806642A8;
    for (s32 i = 0; i < (s32)global->mTaskCount2AC; i++) {
        s16 taskID = *(s16*)((u8*)self + 0x1A6 + i * 2);
        func_80462D04__8CTaskLODFv(taskID);
        func_80462F4C__8CTaskLODFv(taskID, 1);
    }
}

// ============================================================================
// Constructor: __ct__CREvtCamera (0x80181374, size 0x110)
// ============================================================================
extern "C" CREvtCamera* __ct__CREvtCamera(CREvtCamera* self, u32 param) {
    __ct__cf_CREvtObj(self, 0);
    extern u32 lbl_eu_80531CE8;

    self->mField14 = 0;
    self->mField18 = param;
    self->mField1C = 0;
    self->mField20 = 0;
    self->mField24 = 0;
    self->mField88 = 0;
    self->mField8C = lbl_eu_806678A4;
    self->mField90 = 0;
    self->mField94 = 0;
    self->mField98 = 0;
    self->mField9C = 0;
    self->mFieldA0 = 0;
    self->mFieldA4 = 0;
    self->mFieldA5 = 0;
    self->mTaskCount2A8 = 0;
    self->mTaskCount2AC = 0;
    self->mField2B0 = 0;
    self->mTaskCount434 = 0;
    self->mField438 = 0;
    self->mField43C = -1;
    self->mField440 = lbl_eu_806678A8;
    self->mField444 = lbl_eu_806678AC;

    void* mgr = func_800821F8__Q22cf13CfGameManagerFv();
    if (mgr) {
        void* player = *(void**)((u8*)mgr + 0x0C);
        if (player) {
            void* mgr2 = func_800821F8__Q22cf13CfGameManagerFv();
            void* vt = *(void**)mgr2;
            ((void (*)(void*, float))*(void**)((u8*)vt + 0x58))(mgr2, self->mField8C);

            void* mgr3 = func_800821F8__Q22cf13CfGameManagerFv();
            self->mField440 = *(float*)((u8*)(*(void**)((u8*)mgr3 + 0x0C)) + 0x1EC);

            void* mgr4 = func_800821F8__Q22cf13CfGameManagerFv();
            self->mField444 = *(float*)((u8*)(*(void**)((u8*)mgr4 + 0x0C)) + 0x1F0);
        }
    }

    extern unsigned long lbl_eu_806642A8;
    lbl_eu_806642A8 = (unsigned long)self;
    return self;
}

// ============================================================================
// Destructor: __ct__80180088 (0x80181484, size 0x188)
// ============================================================================
extern "C" CREvtCamera* __ct__80180088(CREvtCamera* self, int deleteFlag) {
    if (!self) return self;

    extern unsigned long lbl_eu_806642A8;
    lbl_eu_806642A8 = 0;

    void* mgr = func_800821F8__Q22cf13CfGameManagerFv();
    if (mgr) {
        void* player = *(void**)((u8*)mgr + 0x0C);
        if (player) {
            func_8006BBF4(mgr, 0x400, 0);

            if (self->mFieldA4) {
                float pos[3] = {
                    self->mMatrix28[0][3],
                    self->mMatrix28[1][3],
                    self->mMatrix28[2][3],
                };
                void* mgr2 = func_800821F8__Q22cf13CfGameManagerFv();
                void* vt = *(void**)mgr2;
                ((void (*)(void*, float*))*(void**)((u8*)vt + 0x14))(mgr2, pos);
            }

            void* mgr3 = func_800821F8__Q22cf13CfGameManagerFv();
            void* vt2 = *(void**)mgr3;
            ((void (*)(void*, float))*(void**)((u8*)vt2 + 0x3C))(mgr3, self->mField8C);

            float f30 = self->mField444;
            float f31 = self->mField440;
            void* mgr4 = func_800821F8__Q22cf13CfGameManagerFv();
            void* player2 = *(void**)((u8*)mgr4 + 0x0C);
            *(float*)((u8*)player2 + 0x1EC) = f31;
            *(float*)((u8*)player2 + 0x1F0) = f30;
            func_8049EB60();
        }
    }

    void* vt = *(void**)self;
    ((void (*)(CREvtCamera*))*(void**)((u8*)vt + 0x10))(self);

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
    return self;
}

// ============================================================================
// func_80180210 (0x8018160C, size 0x184)
// ============================================================================
extern "C" void func_80180210(CREvtCamera* taskData) {
    extern unsigned long lbl_eu_806642A8;
    CREvtCamera* global = (CREvtCamera*)lbl_eu_806642A8;
    if (!global) return;

    if (taskData) {
        for (s32 i = 0; i < (s32)global->mTaskCount2A8; i++) {
            s16 taskID = taskData->mTaskArrayA6[i];
            u8 isActive = func_80462E1C__8CTaskLODFv(taskID);
            u32 found = 0;
            for (u32 j = 0; j < global->mTaskCount434; j++) {
                if (global->mTaskArray334[j] == taskID) { found = 1; break; }
            }
            if (!found) {
                global->mFlagArray2B4[global->mTaskCount434] = isActive;
                global->mTaskArray334[global->mTaskCount434] = taskID;
                global->mTaskCount434++;
            }
            if (isActive) func_80462D5C__8CTaskLODFv(taskID);
        }
        global->mField438 = 1;
    } else {
        if (global->mField438) {
            for (u32 i = 0; i < global->mTaskCount434; i++) {
                s16 taskID = global->mTaskArray334[i];
                u8 isActive = func_80462E1C__8CTaskLODFv(taskID);
                if (global->mFlagArray2B4[i]) {
                    if (!isActive) func_80462D04__8CTaskLODFv(taskID);
                } else {
                    if (isActive) func_80462D5C__8CTaskLODFv(taskID);
                }
            }
            global->mField438 = 0;
        }
    }
}

// ============================================================================
// func_80180414 (0x80181810, size 0xB8)
// ============================================================================
extern "C" void func_80180414(CREvtCamera* self) {
    void* handle;
    unsigned long size;
    if (getStaticFileData__14CLibStaticDataFPCcP16StaticDataHandlePUl(
            lbl_eu_805036D8, &handle, &size) && handle) {
        void* obj = func_80495E8C((void*)&lbl_eu_80663E14, -1, 1);
        self->mField1C = (u32)obj;
        func_804827DC(obj, 1);
        func_80484E5C(obj, lbl_eu_806678B0);
        *(u32*)((u8*)obj + 0x7A8) |= 0x4;
        void* vt = *(void**)obj;
        void* (*vfunc)(void*, void*) = (void* (*)(void*, void*))*(void**)((u8*)vt + 0x3C);
        self->mField90 = (u32)vfunc(obj, lbl_eu_80662448);
        self->mField94 = (u32)vfunc(obj, lbl_eu_8066244C);
    }
}

// ============================================================================
// func_801804CC (0x801818C8, size 0x154)
// ============================================================================
extern "C" void func_801804CC(CREvtCamera* self) {
    void* mgr = func_800821F8__Q22cf13CfGameManagerFv();
    if (mgr) func_8006BBF4(mgr, 0x400, 1);
    if (!self->mField1C) return;
    if (cf::CfGameManager::func_800829B8()) return;

    void* sceneObj = (void*)self->mField1C;
    {
        int t = func_8016A35C();
        float ft = (float)(s32)t;
        void* r = func_8048BAD4(sceneObj, lbl_eu_8066244C);
        GetRotTrans__Q34nw4r3g3d12ChrAnmResultCFPQ34nw4r4math5MTX34(r, self->mMatrix58);
    }
    self->mFieldA4 = 1;
    {
        int t = func_8016A35C();
        float ft = (float)(s32)t;
        void* r = func_8048BAD4(sceneObj, lbl_eu_80662448);
        GetRotTrans__Q34nw4r3g3d12ChrAnmResultCFPQ34nw4r4math5MTX34(r, self->mMatrix28);
    }
    void* camObj = func_80496264((void*)&lbl_eu_80663E14, -1);
    if (!camObj) return;

    mgr = func_800821F8__Q22cf13CfGameManagerFv();
    if (mgr) {
        void* vt = *(void**)mgr;
        ((void (*)(void*, float))*(void**)((u8*)vt + 0x3C))(mgr, self->mMatrix28[2][3]);
    }
    *(float*)((u8*)camObj + 0x1E0) = self->mMatrix28[2][3];
    func_8049EB60();
    *(float*)((u8*)camObj + 0x1EC) = self->mMatrix28[0][3];
    *(float*)((u8*)camObj + 0x1F0) = self->mMatrix28[1][1];
    func_8049EB60();
    func_8049F774(camObj, self->mMatrix58);
    self->mFieldA4 = 1;
}

// ============================================================================
// func_80180664 (0x80181A60, size 0x2DC)
// ============================================================================
extern "C" void func_80180664(CREvtCamera* self, void* eventData, void* someParam) {
    // Set fieldA5 based on mField88's low word
    if (self->mField88) {
        self->mFieldA5 = (*(u32*)((u8*)self->mField88 + 4) != 0) ? 1 : 0;
    } else {
        self->mFieldA5 = 0;
    }
    self->mTaskCount2A8 = 0;
    self->mTaskCount2AC = 0;
    if (!self->mField1C) { self->mField88 = (u32)someParam; return; }
    self->mField98 = 0;
    self->mField9C = 0;
    self->mFieldA0 = 0;
    if (self->mField20) {
        void* vt = *(void**)self;
        ((void (*)(CREvtCamera*))*(void**)((u8*)vt + 0x10))(self);
        self->mField24 = self->mField20;
        self->mField20 = 0;
    }
    if (eventData) {
        void* taskObj = func_80495EAC((void*)&lbl_eu_80663E14, eventData, 0);
        self->mField20 = (u32)taskObj;
        func_804839D4((void*)self->mField1C, taskObj, 0, 0, 0, 0, 1, -1);
        self->mField43C = (u32)func_804850A4((void*)self->mField1C);
        func_8016AF4C(taskObj, lbl_eu_805036D8 + 4, &self->mField98);
        func_8016AF4C(taskObj, lbl_eu_805036D8 + 15, &self->mField9C);
        func_8016AF4C(taskObj, lbl_eu_805036D8 + 23, &self->mFieldA0);
        func_8016AF4C(taskObj, lbl_eu_805036D8 + 30, &self->mField2B0);

        if (!GetCamera()->mFieldA5) {
            *(u32*)((u8*)func_801644B4() + 0x1B8) = 1;
        }

        u32 timeVal;
        if (func_8016B5A4(taskObj, lbl_eu_805036D8 + 38, &timeVal)) {
            int val = atoi((const char*)timeVal);
            u32 v = (u32)val & 0xFFFF;
            int a = (int)((long long)v * 0x51EC851FLL >> 32);
            u32 seconds = (u32)(a >> 5);
            seconds += (u32)((u32)a >> 31);
            u32 remainder = v - seconds * 100;
            if (seconds > 99 && remainder > 99) {
                func_8016841C();
            } else {
                func_80086B5C__Q22cf13CfGameManagerFv(seconds, remainder, 0);
                void* sda = getGlobalSda();
                func_800599E0(sda, seconds, remainder, 0);
            }
        }

        u32 taskCount;
        u32 taskBuf[128];
        if (func_8016B164(taskObj, lbl_eu_805036D8 + 43, &taskBuf, &taskCount)) {
            for (u32 i = 0; i < taskCount; i++) {
                self->mTaskArrayA6[self->mTaskCount2A8] = (s16)taskBuf[i];
                self->mTaskCount2A8++;
            }
        }

        u32 taskCount2;
        u32 taskBuf2[128];
        if (func_8016B164(taskObj, lbl_eu_805036D8 + 51, &taskBuf2, &taskCount2)) {
            for (u32 i = 0; i < taskCount2; i++) {
                self->mTaskArray1A6[self->mTaskCount2AC] = (s16)taskBuf2[i];
                self->mTaskCount2AC++;
            }
        }
    }
    self->mField88 = (u32)someParam;
}