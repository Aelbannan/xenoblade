// TU: kyoshin/realtimeevt/CREvtEffect
// Class: CREvtEffect
// Vtable at 0x805322D8 (main) + 0x28 (secondary callback interface)
// Inherits from cf::CREvtObj (offset 0x00)
// Secondary callback interface at offset 0x14

#include <types.h>
#include <string.h>
#include <nw4r/g3d/res/g3d_resanmchr.h>
#include "kyoshin/realtimeevt/CREvtEffect.hpp"
#include "kyoshin/realtimeevt/CREvtObj.hpp"
#include "kyoshin/code_801862C0.hpp"

// ============================================================================
// External function declarations
// ============================================================================

// CREvtObj functions
extern "C" void __ct__cf_CREvtObj(cf::CREvtObj* self, int arg);
extern "C" void __dt__Q22cf8CREvtObjFv(cf::CREvtObj* self, int dealloc_flag);
extern "C" void __dt__80185754(void* ptr);

// Bdat / data loading
extern "C" void* func_8016A24C(u32 arg);
extern "C" void* func_8016A27C(void* ptr);
extern "C" int func_8016A35C();
extern "C" bool func_8016C410();
extern "C" bool func_80180954();
extern "C" void func_8016AF4C(void* obj, const char* name, s32* out);

// Effect system
extern "C" void func_804CC1BC(void* mgr, void* data);
extern "C" void func_804CC1D8(void* mgr, void* data);
extern "C" void* func_804CC1F4(void* mgr, void* bdat, void* global, int r6, int r7, int r8);
extern "C" void func_804E3CCC(void* effect);
extern "C" void func_804E3D0C(void* effect, void* parent);
extern "C" void func_804E3CDC(void* effect, f32 f1, f32 f2);
extern "C" void func_804E3D48(void* effect, void* parent);

// Model / animation
extern "C" nw4r::g3d::ChrAnmResult* func_8048BAD4(void* model, const char* animName, f32 time);
extern "C" void* func_80489A60(void* global, void* handle, int, int, int, int);
extern "C" void func_80484E5C(void* model, f32 val);
extern "C" void func_804827DC(void* model, int val);
extern "C" void func_804839D4(void* model, void* animData, int, int, int, int, int);

// Memory
extern "C" void func_80495E60(void* ptr);
extern "C" void* func_80495EAC(void* global, void* bdat, int val);

// Random
extern "C" f32 func_80496288(void* global);

// Device
extern "C" u8 getLanguage__9CDeviceSCFv();
extern "C" bool getStaticFileData__14CLibStaticDataFPCcP16StaticDataHandlePUl(
    const char* name, void* handle, u32* outSize);

// Globals
extern "C" {
    extern void* lbl_eu_805322D8[];
    extern u8 lbl_eu_8065FC18;
    extern u8 lbl_eu_80663E14;
    extern const char* lbl_eu_80662470;  // "effTgt"
    extern const char* lbl_eu_80662474;  // "effAtr"
    extern const char* lbl_eu_80662478;  // "eff"
    extern const char lbl_eu_80503800[]; // "EFFMonochrome\0MonoRGB\0"
    extern const f32 lbl_eu_8066793C;    // 1.0f
    extern const f32 lbl_eu_80667940;    // 0.0f
    extern const f32 lbl_eu_80667944;    // epsilon
    extern const f32 lbl_eu_80667948;    // 1.0f threshold
    extern const f32 lbl_eu_8066794C;    // 0.0f
    extern const double lbl_eu_80667950; // double magic for int->float
    extern const f32 lbl_eu_80667958;    // -1.0f
    extern const f32 lbl_eu_8066795C;    // frame rate divisor
    extern const f32 lbl_eu_80667960;    // 0.5f
    extern const f32 lbl_eu_8066A1F8;    // 60.0f
}

// Forward declarations for functions defined later in this file
extern "C" void func_80184D90(CREvtEffect* self);
extern "C" bool func_80184F90(CREvtEffect* self);
extern "C" void func_8018515C(CREvtEffect* self);

// ============================================================================
// Constructor: __ct__CREvtEffect (0x80185F54)
// ============================================================================
extern "C" CREvtEffect* __ct__CREvtEffect(CREvtEffect* self, u32 param) {
    // Call CREvtObj base constructor with arg=1
    __ct__cf_CREvtObj((cf::CREvtObj*)self, 1);

    // Set vtable pointers
    void* mainVtable = (void*)lbl_eu_805322D8;
    self->vtable = mainVtable;
    self->mSecondaryVtable = (char*)mainVtable + 0x28;

    // Initialize fields
    self->mPtr18 = 0;
    self->mParam1C = param;
    self->mModel = 0;
    self->mMaterial = 0;
    self->mAnim28 = 0;
    self->mBdatEntry = 0;
    self->mEffectCount = 0;

    // Node IDs to -1
    // Note: mNodeB4, mNodeB8, mNodeBC, mEffects[] are NOT initialized here
    self->mNodeIdC0 = -1;
    self->mNodeIdC4 = -1;
    self->mNodeIdC8 = -1;

    // Animation data: mAnm1Translate/Rotate/Scale = 1.0
    self->mAnm1Translate[0] = 1.0f;
    self->mAnm1Translate[1] = 1.0f;
    self->mAnm1Translate[2] = 1.0f;
    self->mAnm1Rotate[0] = 1.0f;
    self->mAnm1Rotate[1] = 1.0f;
    self->mAnm1Rotate[2] = 1.0f;
    self->mAnm1Scale[0] = 1.0f;
    self->mAnm1Scale[1] = 1.0f;
    self->mAnm1Scale[2] = 1.0f;

    // mAnm2Scale = 0.0
    self->mAnm2Scale[0] = 0.0f;
    self->mAnm2Scale[1] = 0.0f;
    self->mAnm2Scale[2] = 0.0f;

    // mAnm2Rotate/Translate and mAnm3Translate = 1.0
    self->mAnm2Rotate[0] = 1.0f;
    self->mAnm2Rotate[1] = 1.0f;
    self->mAnm2Rotate[2] = 1.0f;
    self->mAnm2Translate[0] = 1.0f;
    self->mAnm2Translate[1] = 1.0f;
    self->mAnm2Translate[2] = 1.0f;
    self->mAnm3Translate[0] = 1.0f;
    self->mAnm3Translate[1] = 1.0f;
    self->mAnm3Translate[2] = 1.0f;

    // mAnm3Scale = 0.0, mFloat12C = 0.0
    self->mAnm3Scale[0] = 0.0f;
    self->mAnm3Scale[1] = 0.0f;
    self->mAnm3Scale[2] = 0.0f;
    self->mFloat12C = 0.0f;

    // Last frame = -1
    self->mLastFrame = -1;

    // Flags
    self->mFlag134 = 0;
    self->mBdatData = 0;
    self->mTime = 0;
    self->mType = 0;

    // Load bdat data from param's field at +0x10
    u32 bdatField = *(u32*)((char*)param + 0x10);
    void* bdatData = func_8016A24C(bdatField);
    self->mBdatData = bdatData;
    if (bdatData != 0) {
        func_804CC1BC(&lbl_eu_8065FC18, bdatData);
    }

    return self;
}

// ============================================================================
// Destructor: __ct__80184C3C (0x8018607C)
// r3 = this, r4 = dealloc_flag
// ============================================================================
extern "C" CREvtEffect* __ct__80184C3C(CREvtEffect* self, int dealloc_flag) {
    if (self == 0) {
        return self;
    }

    // Restore vtables for virtual dispatch during destruction
    void* mainVtable = (void*)lbl_eu_805322D8;
    self->vtable = mainVtable;
    self->mSecondaryVtable = (char*)mainVtable + 0x28;

    // Call virtual function at vtable+0x10 (func_80184D18 - free animation)
    ((void (*)(CREvtEffect*))((void**)mainVtable)[4])(self);

    // Wait for all effects to be destroyed
    while (self->mEffectCount != 0) {
        func_804E3CCC(self->mEffects[0]);
    }

    // Free bdat data
    if (self->mBdatData != 0) {
        func_804CC1D8(&lbl_eu_8065FC18, self->mBdatData);
    }

    // Free material
    if (self->mMaterial != 0) {
        func_80495E60(self->mMaterial);
        self->mMaterial = 0;
    }

    // Free model
    if (self->mModel != 0) {
        func_80495E60(self->mModel);
        self->mModel = 0;
    }

    // Call CREvtObj destructor (dealloc_flag=0)
    __dt__Q22cf8CREvtObjFv((cf::CREvtObj*)self, 0);

    // If dealloc_flag > 0, free the memory
    if (dealloc_flag > 0) {
        __dt__80185754(self);
    }

    return self;
}

// ============================================================================
// func_80184D18 (0x80186158)
// Frees mAnim28 resource
// ============================================================================
extern "C" void func_80184D18(CREvtEffect* self) {
    if (self->mAnim28 != 0) {
        func_80495E60(self->mAnim28);
        self->mAnim28 = 0;
    }
}

// ============================================================================
// func_80184D5C (0x8018619C)
// Calls func_80184D90 then func_8018515C
// ============================================================================
extern "C" void func_80184D5C(CREvtEffect* self) {
    func_80184D90(self);
    func_8018515C(self);
}

// ============================================================================
// func_80184D90 (0x801861D0)
// Updates animation from model's chr results
// ============================================================================
extern "C" void func_80184D90(CREvtEffect* self) {
    if (self->mMaterial == 0) {
        return;
    }

    void* model = self->mModel;
    int time = func_8016A35C();

    // Get chr result for "eff" animation -> writes to mAnm2Translate/Rotate/Scale
    nw4r::g3d::ChrAnmResult* result = func_8048BAD4(model, lbl_eu_80662478, (f32)time);
    result->GetTranslate((nw4r::math::VEC3*)self->mAnm2Translate);
    result->GetRotateDeg((nw4r::math::VEC3*)self->mAnm2Rotate);
    result->GetScale((nw4r::math::VEC3*)self->mAnm2Scale);

    // Get chr result for "effAtr" animation -> writes to mAnm1Translate/Rotate/Scale
    time = func_8016A35C();
    result = func_8048BAD4(model, lbl_eu_80662474, (f32)time);
    result->GetTranslate((nw4r::math::VEC3*)self->mAnm1Translate);
    result->GetRotateDeg((nw4r::math::VEC3*)self->mAnm1Rotate);
    result->GetScale((nw4r::math::VEC3*)self->mAnm1Scale);

    // Alpha fade handling for mAnm1Scale.z
    f32 alpha = self->mAnm1Scale[2];
    if (alpha > 1.0f) {
        int intPart = (int)alpha;
        f32 fracPart = alpha - (f32)intPart;
        if (fracPart > 0.0001f) {
            fracPart = 0.0f;
        }
        self->mAnm1Scale[2] = (f32)intPart + fracPart;
    }

    // Copy mAnm1Rotate to mAnm3Scale, and mAnm1Scale.z to mFloat12C
    self->mAnm3Scale[0] = self->mAnm1Rotate[0];
    self->mAnm3Scale[1] = self->mAnm1Rotate[1];
    self->mAnm3Scale[2] = self->mAnm1Rotate[2];
    self->mFloat12C = self->mAnm1Scale[2];

    // Get chr result for "effTgt" animation -> writes to mAnm3Translate
    time = func_8016A35C();
    result = func_8048BAD4(model, lbl_eu_80662470, (f32)time);
    result->GetTranslate((nw4r::math::VEC3*)self->mAnm3Translate);

    // Check if alpha threshold exceeded
    f32 alphaVal = self->mAnm1Rotate[1];
    if (alphaVal > 1.0f) {
        int currentTime = func_8016A35C();
        if (self->mLastFrame != currentTime) {
            // Check random value
            f32 randVal = func_80496288(&lbl_eu_80663E14);
            if (randVal >= 0.0f) {
                randVal = func_80496288(&lbl_eu_80663E14);
                if (randVal >= 0.0f) {
                    if (self->mFlag134 == 0) {
                        func_80184F90(self);
                    }
                }
            } else {
                func_80184F90(self);
            }
        }
    }
}

// ============================================================================
// func_80184F90 (0x801863D0)
// Spawns an effect
// ============================================================================
extern "C" bool func_80184F90(CREvtEffect* self) {
    // Check if effect count exceeds max (32)
    if (self->mEffectCount >= 32) {
        return 0;
    }

    // Check if bdat data is loaded
    if (self->mBdatData == 0) {
        return 0;
    }

    // Get language
    u8 lang = getLanguage__9CDeviceSCFv();
    if (lang > 5) {
        lang = 1;
    }
    lang &= 0xFF;

    // Check language compatibility
    u32 bdatLang = *(u32*)((char*)(self->mParam1C) + 0x0C);
    u32 bdatLangByte = (bdatLang >> 24) & 0xFF;
    if (bdatLangByte != lang && bdatLangByte != 0xFF && bdatLangByte != 0) {
        return 0;
    }

    // Create effect
    void* effect = func_804CC1F4(
        &lbl_eu_8065FC18,
        self->mBdatData,
        &lbl_eu_80663E14,
        0,  // r6
        1,  // r7
        0   // r8
    );

    if (effect == 0) {
        return 0;
    }

    // Store effect in array
    int count = self->mEffectCount;
    self->mEffects[count] = effect;
    self->mEffectCount = count + 1;

    // Set parent
    func_804E3D0C(effect, (char*)self + 0x14);

    // Scale
    f32 scale = self->mAnm1Scale[0];
    if (scale < -1.0f) {
        scale = 0.0f;
    }

    // Check if in culling mode
    if (func_8016C410()) {
        *(u8*)((char*)effect + 0x59) = 7;
    }

    // Set effect scale
    func_804E3CDC(effect, self->mAnm1Scale[0], scale);

    // Set priority
    if (self->mBdatEntry != 0) {
        s32 priority = *(s32*)((char*)self->mBdatEntry + 0x04);
        if (priority >= 0) {
            *(u8*)((char*)effect + 0x58) = (u8)priority;
        }
    }

    // Get name string
    void* nameObj = 0;
    if (self->mBdatEntry != 0) {
        nameObj = func_8016A27C(*(void**)((char*)self->mBdatEntry + 0x08));
    }
    char nameBuf[32];
    u32 nameLen = 0;
    nameBuf[0] = 0;

    if (nameObj != 0) {
        // Call virtual function to get name
        void* nameStr = ((void* (*)(void*))(*(void***)nameObj)[4])(nameObj);
        nameLen = strlen((const char*)nameStr);
        strcpy(nameBuf, (const char*)nameStr);
    }

    // Set flag
    self->mFlag134 = 1;

    // Update last frame
    self->mLastFrame = func_8016A35C();

    // Set type-specific parameters
    if (self->mTime == 1) {
        switch (self->mType) {
            case 0:
                *(s32*)((char*)effect + 0x5C) = 1;
                break;
            case 1:
                *(s32*)((char*)effect + 0x5C) = 2;
                break;
            case 2:
                *(s32*)((char*)effect + 0x5C) = 3;
                break;
        }
    } else {
        *(s32*)((char*)effect + 0x5C) = 0;
    }

    return 1;
}

// ============================================================================
// func_8018515C (0x801865E4)
// Updates all effects
// ============================================================================
extern "C" void func_8018515C(CREvtEffect* self) {
    // Get model data
    void* nameObj = 0;
    if (self->mBdatEntry != 0) {
        void* animData = *(void**)((char*)self->mBdatEntry + 0x08);
        if ((s32)animData >= 0) {
            nameObj = func_8016A27C(animData);
        }
    }

    f32 fpsDiv = 60.0f;
    f32 half = 0.5f;

    // Update each effect
    for (int i = 0; i < self->mEffectCount; i++) {
        void* effect = self->mEffects[i];

        // Set position
        *(f32*)((char*)effect + 0x50) = self->mAnm1Translate[0];

        // Set rotation from mAnm2Translate
        *(s32*)((char*)effect + 0x1C) = *(s32*)&self->mAnm2Translate[0];
        *(s32*)((char*)effect + 0x20) = *(s32*)&self->mAnm2Translate[1];
        *(s32*)((char*)effect + 0x24) = *(s32*)&self->mAnm2Translate[2];

        // Apply rotation conversion (fpsDiv factor)
        f32 rotX = self->mAnm1Rotate[0] * fpsDiv;
        f32 rotY = self->mAnm1Rotate[1] * fpsDiv;
        f32 rotZ = self->mAnm1Rotate[2] * fpsDiv;

        *(s32*)((char*)effect + 0x28) = *(s32*)&rotX;
        *(s32*)((char*)effect + 0x2C) = *(s32*)&rotY;
        *(s32*)((char*)effect + 0x30) = *(s32*)&rotZ;

        // Set scale from mAnm2Scale
        *(s32*)((char*)effect + 0x34) = *(s32*)&self->mAnm2Scale[0];
        *(s32*)((char*)effect + 0x38) = *(s32*)&self->mAnm2Scale[1];
        *(s32*)((char*)effect + 0x3C) = *(s32*)&self->mAnm2Scale[2];

        // Set additional scale from mAnm3Scale
        *(s32*)((char*)effect + 0x40) = *(s32*)&self->mAnm3Scale[0];
        *(s32*)((char*)effect + 0x44) = *(s32*)&self->mAnm3Scale[1];
        *(s32*)((char*)effect + 0x48) = *(s32*)&self->mAnm3Scale[2];

        // Set alpha
        f32 alpha = self->mAnm1Scale[2];
        if (alpha > half) {
            *(u8*)((char*)effect + 0x59) = (u8)(int)(half + alpha);
        }

        // Check for parent binding
        if (*(s32*)((char*)effect + 0x14) == 0) {
            if (nameObj != 0) {
                // Check if model can bind the effect
                if (((bool (*)(void*))(*(void***)nameObj)[0x19])(nameObj)) {
                    // Bind effect to model
                    *(u16*)((char*)effect + 0x00) |= 0x4000;
                    *(void**)((char*)effect + 0x14) = nameObj;
                } else {
                    // Clear bind flag
                    *(u16*)((char*)effect + 0x00) &= ~0x4000;
                }
            }
        }
    }

    // Check flag and random
    if (self->mFlag134 != 0) {
        f32 randVal = func_80496288(&lbl_eu_80663E14);
        if (randVal < 0.0f) {
            self->mFlag134 = 0;
        }
    }

    // Check random and update last frame
    f32 randVal = func_80496288(&lbl_eu_80663E14);
    if (randVal < 0.0f) {
        int currentTime = func_8016A35C();
        if (self->mLastFrame != currentTime) {
            self->mLastFrame = -1;
        }
    }
}

// ============================================================================
// func_80185378 (0x80186800)
// Initializes with model and bdat data
// r3 = this, r4 = src, r5 = bdatEntry
// ============================================================================
extern "C" void func_80185378(CREvtEffect* self, void* r4, void* r5) {
    // If material already exists, free old resources
    if (self->mMaterial != 0) {
        // Call virtual function at vtable+0x10 (func_80184D18)
        void* mainVtable = (void*)lbl_eu_805322D8;
        ((void (*)(CREvtEffect*))((void**)mainVtable)[4])(self);

        // Move material to mAnim28 and clear
        self->mAnim28 = self->mMaterial;
        self->mMaterial = 0;

        // Check if we should wait for effects
        if (!func_80180954()) {
            while (self->mEffectCount != 0) {
                func_804E3CCC(self->mEffects[0]);
            }
        }

        // Reset last frame
        self->mLastFrame = -1;

        // If r4 is null, free model
        if (r4 == 0) {
            if (self->mModel != 0) {
                func_80495E60(self->mModel);
                self->mModel = 0;
            }
        }
    }

    // If r4 is non-null, load model
    if (r4 != 0) {
        if (self->mModel == 0) {
            // Load static file data
            void* handle;
            u32 fileSize;
            getStaticFileData__14CLibStaticDataFPCcP16StaticDataHandlePUl(
                &lbl_eu_80503800[0], &handle, &fileSize);
            (void)fileSize;

            // Create model
            self->mModel = func_80489A60(
                &lbl_eu_80663E14,
                &handle,
                1,  // r5
                1,  // r6
                0,  // r7
                0x50 // r8
            );

            // Set model properties
            func_80484E5C(self->mModel, 1.0f);
            *(u32*)((char*)self->mModel + 0x7A8) |= 4;

            // Get animation bind indices
            self->mNodeB4 = (void*)((void* (*)(void*, const char*))(*(void***)self->mModel)[0x0F])(self->mModel, lbl_eu_80662478);
            self->mNodeB8 = (void*)((void* (*)(void*, const char*))(*(void***)self->mModel)[0x0F])(self->mModel, lbl_eu_80662474);
            self->mNodeBC = (void*)((void* (*)(void*, const char*))(*(void***)self->mModel)[0x0F])(self->mModel, lbl_eu_80662470);

            // Get animation result indices
            self->mNodeIdC0 = (s32)((void* (*)(void*, const char*))(*(void***)self->mModel)[0x11])(self->mModel, lbl_eu_80662478);
            self->mNodeIdC4 = (s32)((void* (*)(void*, const char*))(*(void***)self->mModel)[0x11])(self->mModel, lbl_eu_80662474);
            self->mNodeIdC8 = (s32)((void* (*)(void*, const char*))(*(void***)self->mModel)[0x11])(self->mModel, lbl_eu_80662470);

            // Enable animation
            func_804827DC(self->mModel, 1);
        }

        // Load bdat entry
        void* bdatEntry = func_80495EAC(&lbl_eu_80663E14, r4, 0);
        self->mMaterial = bdatEntry;

        if (bdatEntry != 0) {
            // Set animation on model
            func_804839D4(
                self->mModel,
                bdatEntry,
                0,  // r6
                0,  // r7
                0,  // r8
                1,  // r9
                -1  // r10
            );

            // Set time and type from bdat
            self->mTime = 0;
            self->mType = 0;
            func_8016AF4C(bdatEntry, &lbl_eu_80503800[12], &self->mTime);
            func_8016AF4C(bdatEntry, &lbl_eu_80503800[23], &self->mType);
        }
    }

    // Store bdat entry
    self->mBdatEntry = r5;
}

// ============================================================================
// func_801855C4 (0x80186A4C)
// Removes an effect by pointer from the array
// r3 = this, r4 = effect pointer
// ============================================================================
extern "C" void func_801855C4(CREvtEffect* self, void* effect) {
    int count = self->mEffectCount;
    int foundIdx = -1;

    for (int i = 0; i < count; i++) {
        if (self->mEffects[i] == effect) {
            foundIdx = i;
            break;
        }
    }

    if (foundIdx >= 0) {
        // Detach effect from parent
        void* parent = (self != 0) ? (char*)self + 0x14 : 0;
        func_804E3D48(effect, parent);

        // Shift remaining effects down
        for (int i = foundIdx; i < count - 1; i++) {
            self->mEffects[i] = self->mEffects[i + 1];
        }

        // Decrement count
        self->mEffectCount = count - 1;
    }
}

// ============================================================================
// func_80185674 (0x80186AFC)
// Returns mModel
// ============================================================================
extern "C" u32 func_80185674(CREvtEffect* self) {
    return (u32)self->mModel;
}

// ============================================================================
// func_8018567C (0x80186B04)
// Empty function
// ============================================================================
extern "C" void func_8018567C() {
}

// ============================================================================
// func_80185680 (0x80186B08)
// Adjustor thunk: adjusts this by -0x14 and calls func_801855C4
// ============================================================================
extern "C" void func_80185680(void* self) { ((void(*)(void*))func_801855C4)((char*)self - 0x14); }

// ============================================================================
// func_80185688 (0x80186B10)
// Adjustor thunk: adjusts this by -0x14 and calls __ct__80184C3C (destructor)
// ============================================================================
extern "C" void func_80185688(void* self) { ((void(*)(void*))__ct__80184C3C)((char*)self - 0x14); }