// Auto-scaffolded catalog TU for kyoshin/realtimeevt/CREvtCamera
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/realtimeevt/CREvtCamera.hpp"

// External function declarations
extern "C" {
    // cf::CREvtObj constructor (takes this + param stored at offset 0x04)
    extern void __ct__cf_CREvtObj(CREvtCamera* self, u32 param);
    // cf::CREvtObj destructor
    extern void __dt__Q22cf8CREvtObjFv(CREvtCamera* self, int deleteFlag);
    // Operator delete for CREvtCamera
    extern void __dt__80185754(CREvtCamera* self);

    // cf::CfGameManager functions
    extern void* func_800821F8__Q22cf13CfGameManagerFv();
    extern void func_800829B8__Q22cf13CfGameManagerFv();
    extern void func_80086B5C__Q22cf13CfGameManagerFv(u32, u32, u32);

    // CTaskLOD functions
    extern unsigned char func_80462E1C__8CTaskLODFv(s16 taskID);
    extern void func_80462D04__8CTaskLODFv(s16 taskID);
    extern void func_80462D5C__8CTaskLODFv(s16 taskID);
    extern void func_80462F4C__8CTaskLODFv(s16 taskID, int flag);

    // CLibStaticData
    extern int getStaticFileData__14CLibStaticDataFPCcP16StaticDataHandlePUl(
        const char* path, void* handle, unsigned long* size);

    // Memory / resource functions
    extern void* func_80495E8C(void* mgr, int a, int b);
    extern void func_80495E60(void* ptr);
    extern void* func_80495EAC(void* mgr, void* param, int flag);
    extern void func_8049EB60();
    extern void func_8049F774(void* obj, void* matrix);
    extern void* func_80496264(void* mgr, int a);

    // Scene functions
    extern void func_804827DC(void* obj, int flag);
    extern void func_80484E5C(void* obj, float val);
    extern void func_804839D4(void* obj, void* param, int a, int b, int c, int d, int e, int f);
    extern float func_804850A4(void* obj);
    extern void* func_8048BAD4(void* obj, void* name);

    // nw4r
    extern void GetRotTrans__Q34nw4r3g3d12ChrAnmResultCFPQ34nw4r4math5MTX34(
        void* result, void* mtx);

    // Event/camera functions
    extern void func_8006BBF4(void* mgr, u32 mask, int flag);
    extern int func_8016A35C();
    extern void* func_801644B4();
    extern void func_8016841C();
    extern void func_8016AF4C(void* obj, const char* name, u32* out);
    extern int func_8016B5A4(void* obj, const char* name, void* out);
    extern int func_8016B164(void* obj, const char* name, void* out1, void* out2);

    // Global functions
    extern void* getGlobalSda();
    extern void func_800599E0(void* sda, u32 a, u32 b, u32 c);
    extern int atoi(const char* str);
}

// Global singleton pointer
extern "C" {
    extern CREvtCamera* lbl_eu_806642A8;
}

// External global data
extern u32 lbl_eu_80663E14;
extern void* lbl_eu_80662448;
extern void* lbl_eu_8066244C;

// ============================================================================
// Constructor: __ct__CREvtCamera (0x80181374, size 0x110)
// ============================================================================
extern "C" CREvtCamera* __ct__CREvtCamera(CREvtCamera* self, u32 param) {
    // Call parent constructor with param=0
    __ct__cf_CREvtObj(self, 0);

    // Constants from sdata2
    extern float lbl_eu_806678A4;
    extern float lbl_eu_806678A8;
    extern float lbl_eu_806678AC;

    // Vtable at 0x80531CE8
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

    // Update camera params from game manager
    void* gameMgr = func_800821F8__Q22cf13CfGameManagerFv();
    if (gameMgr) {
        void* player = *(void**)((u8*)gameMgr + 0x0C);
        if (player) {
            void* gameMgr2 = func_800821F8__Q22cf13CfGameManagerFv();
            void* vtable = *(void**)gameMgr2;
            void (*vfunc)(void*, float) = (void (*)(void*, float))*(void**)((u8*)vtable + 0x58);
            vfunc(gameMgr2, self->mField8C);

            void* gameMgr3 = func_800821F8__Q22cf13CfGameManagerFv();
            void* player2 = *(void**)((u8*)gameMgr3 + 0x0C);
            self->mField440 = *(float*)((u8*)player2 + 0x1EC);

            void* gameMgr4 = func_800821F8__Q22cf13CfGameManagerFv();
            void* player3 = *(void**)((u8*)gameMgr4 + 0x0C);
            self->mField444 = *(float*)((u8*)player3 + 0x1F0);
        }
    }

    // Set global singleton
    lbl_eu_806642A8 = self;

    return self;
}

// ============================================================================
// Destructor: __ct__80180088 (0x80181484, size 0x188)
// Named __ct__80180088 in the symbol table, but functions as a destructor.
// ============================================================================
extern "C" CREvtCamera* __ct__80180088(CREvtCamera* self, int deleteFlag) {
    if (self) {
        extern u32 lbl_eu_80531CE8;

        // Clear global singleton
        lbl_eu_806642A8 = 0;

        void* gameMgr = func_800821F8__Q22cf13CfGameManagerFv();
        if (gameMgr) {
            void* player = *(void**)((u8*)gameMgr + 0x0C);
            if (player) {
                func_8006BBF4(gameMgr, 0x400, 0);

                if (self->mFieldA4) {
                    float x = self->mMatrix28[0][3];  // offset 0x64
                    float y = self->mMatrix28[1][3];  // offset 0x74
                    float z = self->mMatrix28[2][3];  // offset 0x84
                    float stack[3] = {x, y, z};

                    void* gameMgr2 = func_800821F8__Q22cf13CfGameManagerFv();
                    void* vtable = *(void**)gameMgr2;
                    void (*vfunc)(void*, float*) = (void (*)(void*, float*))*(void**)((u8*)vtable + 0x14);
                    vfunc(gameMgr2, stack);
                }

                void* gameMgr3 = func_800821F8__Q22cf13CfGameManagerFv();
                void* vtable2 = *(void**)gameMgr3;
                void (*vfunc2)(void*, float) = (void (*)(void*, float))*(void**)((u8*)vtable2 + 0x3C);
                vfunc2(gameMgr3, self->mField8C);

                float f30 = self->mField444;
                float f31 = self->mField440;

                void* gameMgr4 = func_800821F8__Q22cf13CfGameManagerFv();
                void* player2 = *(void**)((u8*)gameMgr4 + 0x0C);
                *(float*)((u8*)player2 + 0x1EC) = f31;
                *(float*)((u8*)player2 + 0x1F0) = f30;

                func_8049EB60();
            }
        }

        // Call vfunc at offset 0x10 (virtual function 4)
        // TODO: replace with actual virtual function call when vtable is defined
        void* vtable = *(void**)self;
        void (*vfunc10)(CREvtCamera*) = (void (*)(CREvtCamera*))*(void**)((u8*)vtable + 0x10);
        vfunc10(self);

        // Free member at offset 0x20
        if (self->mField20) {
            if (lbl_eu_80663E14) {
                func_80495E60((void*)self->mField20);
            }
            self->mField20 = 0;
        }

        // Free member at offset 0x1C
        if (self->mField1C) {
            if (lbl_eu_80663E14) {
                func_80495E60((void*)self->mField1C);
            }
            self->mField1C = 0;
        }

        // Call parent destructor
        __dt__Q22cf8CREvtObjFv(self, 0);

        // Optionally call operator delete
        if (deleteFlag > 0) {
            __dt__80185754(self);
        }
    }

    return self;
}

// ============================================================================
// func_80180210 (0x8018160C, size 0x184)
// Synchronizes task visibility based on activation state.
// When called with a non-null pointer, scans task array at 0xA6 and
// registers active tasks. When called with null, processes pending
// tasks from the task array at 0x334.
// ============================================================================
extern "C" void func_80180210(CREvtCamera* taskData) {
    CREvtCamera* global = lbl_eu_806642A8;
    if (!global) {
        return;
    }

    if (taskData) {
        // First pass: scan task array at +0xA6, register active tasks
        u32 count = 0;
        for (s32 i = 0; i < (s32)global->mTaskCount2A8; i++) {
            s16 taskID = taskData->mTaskArrayA6[i];
            unsigned char isActive = func_80462E1C__8CTaskLODFv(taskID);

            // Check if taskID is already registered in the array at +0x334
            s32 found = 0;
            for (u32 j = 0; j < global->mTaskCount434; j++) {
                if (global->mTaskArray334[j] == taskID) {
                    found = 1;
                    break;
                }
            }

            if (!found) {
                // Register the task: store flag and add task ID
                global->mFlagArray2B4[global->mTaskCount434] = isActive;
                global->mTaskArray334[global->mTaskCount434] = taskID;
                global->mTaskCount434++;
            }

            // If task is active, call func_80462D5C
            if (isActive) {
                func_80462D5C__8CTaskLODFv(taskID);
            }
        }

        // Mark as processed
        global->mField438 = 1;
    } else {
        // Second pass: process pending tasks from the array at +0x334
        if (global->mField438) {
            for (u32 i = 0; i < global->mTaskCount434; i++) {
                s16 taskID = global->mTaskArray334[i];
                unsigned char isActive = func_80462E1C__8CTaskLODFv(taskID);

                if (global->mFlagArray2B4[i]) {
                    // Was previously active
                    if (!isActive) {
                        // Now inactive: call func_80462D04 (stop)
                        func_80462D04__8CTaskLODFv(taskID);
                    }
                } else {
                    // Was previously inactive
                    if (isActive) {
                        // Now active: call func_80462D5C (start)
                        func_80462D5C__8CTaskLODFv(taskID);
                    }
                }
            }

            // Clear the processed flag
            global->mField438 = 0;
        }
    }
}

// ============================================================================
// func_80180394 (0x80181790, size 0x80)
// Iterates the task array at +0x1A6 and calls func_80462D04 + func_80462F4C
// on each task ID.
// ============================================================================
extern "C" void func_80180394(CREvtCamera* self) {
    CREvtCamera* global = lbl_eu_806642A8;
    if (!global) {
        return;
    }

    for (s32 i = 0; i < (s32)global->mTaskCount2AC; i++) {
        s16 taskID = self->mTaskArray1A6[i];
        func_80462D04__8CTaskLODFv(taskID);
        func_80462F4C__8CTaskLODFv(taskID, 1);
    }
}

// ============================================================================
// func_80180414 (0x80181810, size 0xB8)
// Loads a static file and creates a scene model object.
// ============================================================================
extern "C" void func_80180414(CREvtCamera* self) {
    extern const char lbl_eu_805036D8[];
    extern float lbl_eu_806678B0;

    void* handle;
    unsigned long size;

    int result = getStaticFileData__14CLibStaticDataFPCcP16StaticDataHandlePUl(
        lbl_eu_805036D8, &handle, &size);

    if (handle) {
        void* obj = func_80495E8C((void*)&lbl_eu_80663E14, -1, 1);
        self->mField1C = (u32)obj;

        func_804827DC(obj, 1);
        func_80484E5C(obj, lbl_eu_806678B0);

        // Set flag bit 2 on the object
        *(u32*)((u8*)obj + 0x7A8) |= 0x4;

        // Call vfunc at offset 0x3C to get handles
        void* vtable = *(void**)obj;
        void* (*vfunc3C)(void*, void*) = (void* (*)(void*, void*))*(void**)((u8*)vtable + 0x3C);
        self->mField90 = (u32)vfunc3C(obj, lbl_eu_80662448);
        self->mField94 = (u32)vfunc3C(obj, lbl_eu_8066244C);
    }
}

// ============================================================================
// func_801804CC (0x801818C8, size 0x154)
// Processes camera animation data and updates the game manager.
// ============================================================================
extern "C" void func_801804CC(CREvtCamera* self) {
    // Set up game manager flag
    void* gameMgr = func_800821F8__Q22cf13CfGameManagerFv();
    if (gameMgr) {
        func_8006BBF4(gameMgr, 0x400, 1);
    }

    // If we have a scene object, process animation
    if (self->mField1C) {
        void* func_800829B8 = func_800829B8__Q22cf13CfGameManagerFv();
        if (func_800829B8) {
            return;
        }

        void* sceneObj = (void*)self->mField1C;
        int time = func_8016A35C();

        // Convert time to float and call animation function
        {
            extern float lbl_eu_806678B8;
            float fTime = (float)(s32)(time ^ 0x80000000) + lbl_eu_806678B8;
            fTime = *(float*)&(int)(time ^ 0x80000000) + lbl_eu_806678B8;
            // Actually let me re-read the asm...
        }
    }
}