// TU: kyoshin/realtimeevt/CREvtEffect
// Class: CREvtEffect
// Vtable at 0x805322D8 (main) + 0x28 (secondary callback interface)
// Inherits from cf::CREvtObj (offset 0x00)
// Secondary callback interface at offset 0x14

#include <types.h>
#include "monolib/scn/CScnTimeApi.hpp"
#include <string.h>
#include <nw4r/g3d/res/g3d_resanmchr.h>
#include <nw4r/math/math_types.h>
#include "kyoshin/realtimeevt/CREvtEffect.hpp"
#include "kyoshin/realtimeevt/CREvtObj.hpp"
#include "kyoshin/code_801862C0.hpp"

// C-linkage imports relocated to CREvtEffect.hpp (see the "C-linkage imports"
// section there). This TU still defines the CREvt* functions below.

// ============================================================================
// Constructor: __ct__CREvtEffect (0x80185F54)
// ============================================================================
extern "C" CREvtEffect* __ct__CREvtEffect(CREvtEffect* self, CREvtParam* param) {
    // Call CREvtObj base constructor with arg=1
    __ct__cf_CREvtObj((cf::CREvtObj*)self, 1);

    // Set vtable pointers (main + secondary callback interface at +0x28)
    self->vtable = (void*)lbl_eu_805322D8;
    self->mSecondaryVtable = (char*)lbl_eu_805322D8 + 0x28;

    // Initialize fields
    self->mPtr18 = 0;
    self->mParam1C = param;
    self->mModel = 0;
    self->mMaterial = 0;
    self->mAnim28 = 0;
    self->mBdatEntry = 0;
    self->mEffectCount = 0;

    // Node IDs to -1 (mNodeB4/B8/BC and mEffects[] are left uninitialized)
    self->mNodeIdC0 = -1;
    self->mNodeIdC4 = -1;
    self->mNodeIdC8 = -1;

    // Animation defaults: translate/rotate/scale groups = 1.0
    self->mAnm1Translate[0] = lbl_eu_8066793C;
    self->mAnm1Translate[1] = lbl_eu_8066793C;
    self->mAnm1Translate[2] = lbl_eu_8066793C;
    self->mAnm1Rotate[0] = lbl_eu_8066793C;
    self->mAnm1Rotate[1] = lbl_eu_8066793C;
    self->mAnm1Rotate[2] = lbl_eu_8066793C;
    self->mAnm1Scale[0] = lbl_eu_8066793C;
    self->mAnm1Scale[1] = lbl_eu_8066793C;
    self->mAnm1Scale[2] = lbl_eu_8066793C;

    self->mAnm2Scale[0] = lbl_eu_80667940;
    self->mAnm2Scale[1] = lbl_eu_80667940;
    self->mAnm2Scale[2] = lbl_eu_80667940;

    self->mAnm2Rotate[0] = lbl_eu_8066793C;
    self->mAnm2Rotate[1] = lbl_eu_8066793C;
    self->mAnm2Rotate[2] = lbl_eu_8066793C;
    self->mAnm2Translate[0] = lbl_eu_8066793C;
    self->mAnm2Translate[1] = lbl_eu_8066793C;
    self->mAnm2Translate[2] = lbl_eu_8066793C;
    self->mAnm3Translate[0] = lbl_eu_8066793C;
    self->mAnm3Translate[1] = lbl_eu_8066793C;
    self->mAnm3Translate[2] = lbl_eu_8066793C;

    self->mAnm3Scale[0] = lbl_eu_80667940;
    self->mAnm3Scale[1] = lbl_eu_80667940;
    self->mAnm3Scale[2] = lbl_eu_80667940;
    self->mFloat12C = lbl_eu_80667940;

    self->mLastFrame = -1;
    self->mFlag134 = 0;
    self->mBdatData = 0;
    self->mTime = 0;
    self->mType = 0;

    // Load the bdat block for the param's effect id and register it
    void* bdatData = func_8016A24C(param->mBdatId);
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
    if (self != 0) {
        // Restore the vtables, then dispatch func_80184D18 through the main vtable
        self->vtable = (void*)lbl_eu_805322D8;
        self->mSecondaryVtable = (char*)lbl_eu_805322D8 + 0x28;
        ((void (**)(CREvtEffect*))lbl_eu_805322D8)[4](self);

        // Wait for the effect system to release all running effects
        while (self->mEffectCount != 0) {
            func_804E3CCC(self->mEffects[0]);
        }

        // Release the bdat block, material and model
        if (self->mBdatData != 0) {
            func_804CC1D8(&lbl_eu_8065FC18, self->mBdatData);
        }
        if (self->mMaterial != 0) {
            func_80495E60(self->mMaterial);
            self->mMaterial = 0;
        }
        if (self->mModel != 0) {
            func_80495E60(self->mModel);
            self->mModel = 0;
        }

        // Destroy the CREvtObj base, then free the allocation if requested
        __dt__Q22cf8CREvtObjFv((cf::CREvtObj*)self, 0);
        if (dealloc_flag > 0) {
            __dt__80185754(self);
        }
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

    // "eff" animation -> mAnm2* groups
    nw4r::g3d::ChrAnmResult* result =
        func_8048BAD4(model, lbl_eu_80662478, (f32)func_8016A35C());
    result->GetTranslate((nw4r::math::VEC3*)self->mAnm2Translate);
    result->GetRotateDeg((nw4r::math::VEC3*)self->mAnm2Rotate);
    result->GetScale((nw4r::math::VEC3*)self->mAnm2Scale);

    // "effAtr" animation -> mAnm1* groups
    result = func_8048BAD4(model, lbl_eu_80662474, (f32)func_8016A35C());
    result->GetTranslate((nw4r::math::VEC3*)self->mAnm1Translate);
    result->GetRotateDeg((nw4r::math::VEC3*)self->mAnm1Rotate);
    result->GetScale((nw4r::math::VEC3*)self->mAnm1Scale);

    // Normalize the alpha in mAnm1Scale.z: keep the integer part and drop the
    // fractional part once it falls below the epsilon threshold.
    f32 alpha = self->mAnm1Scale[2];
    if (alpha > lbl_eu_8066793C) {
        int intPart = (int)alpha;
        f32 fracPart = alpha - (f32)intPart;
        if (fracPart > lbl_eu_80667944) {
            fracPart = lbl_eu_80667940;
        }
        self->mAnm1Scale[2] = (f32)(int)self->mAnm1Scale[2] + fracPart;
    }

    // Copy rotate into mAnm3Scale and translate.z into mFloat12C
    self->mAnm3Scale[0] = self->mAnm1Rotate[0];
    self->mAnm3Scale[1] = self->mAnm1Rotate[1];
    self->mAnm3Scale[2] = self->mAnm1Rotate[2];
    self->mFloat12C = self->mAnm1Translate[2];

    // "effTgt" animation -> mAnm3Translate
    result = func_8048BAD4(model, lbl_eu_80662470, (f32)func_8016A35C());
    result->GetTranslate((nw4r::math::VEC3*)self->mAnm3Translate);

    // Refresh the spawned effects when the translate.y threshold is crossed
    if (self->mAnm1Translate[1] > lbl_eu_80667948) {
        if (self->mLastFrame != func_8016A35C()) {
            if (func_80496288(lbl_eu_80663E14) >= lbl_eu_8066794C ||
                (func_80496288(lbl_eu_80663E14) < lbl_eu_8066794C &&
                 self->mFlag134 == 0)) {
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
    // Max 32 concurrent effects
    if (self->mEffectCount >= 32) {
        return 0;
    }
    if (self->mBdatData == 0) {
        return 0;
    }

    // Language gating: default to language 1 when the console language is exotic.
    // lang is kept raw (upper bits garbage) and masked per use, like retail.
    int lang = getLanguage__9CDeviceSCFv();
    if ((lang & 0xFF) > 5) {
        lang = 1;
    }

    // The bdat entry may declare a specific language (bits 24-31)
    u32 langByte = self->mParam1C->mLangInfo >> 24;
    if (langByte != (lang & 0xFF) && langByte != 0xFF && langByte != 0) {
        return 0;
    }

    // Create the effect instance and register it in the slot array
    CEffectInst* effect = (CEffectInst*)func_804CC1F4(
        &lbl_eu_8065FC18, self->mBdatData, lbl_eu_80663E14, 0, 1, 0);
    if (effect == 0) {
        return 0;
    }

    int count = self->mEffectCount;
    self->mEffects[count] = effect;
    self->mEffectCount = count + 1;

    // Bind to the callback sub-object (self+0x14), null-guarded like retail
    void* parent = (char*)self;
    if (self != 0) {
        parent = (char*)self + 0x14;
    }
    func_804E3D0C(effect, parent);

    // Scale: mAnm1Scale.y clamped below -1.0 to 0.0
    f32 scale = self->mAnm1Scale[1];
    if (scale < lbl_eu_80667958) {
        scale = lbl_eu_80667940;
    }

    // Culling mode forces effect state 7
    if (func_8016C410()) {
        effect->mField59 = 7;
    }

    func_804E3CDC(effect, self->mAnm1Scale[0], scale);

    // Priority from the bdat entry
    if (((CBdatEntry*)self->mBdatEntry)->mPriority >= 0) {
        effect->mPriority = (u8)((CBdatEntry*)self->mBdatEntry)->mPriority;
    }

    // Copy the entry's name into a local buffer
    void* nameObj = func_8016A27C(((CBdatEntry*)self->mBdatEntry)->mNameData);
    char nameBuf[32];
    u32 nameLen;
    nameBuf[0] = 0;
    nameLen = 0;
    if (nameObj != 0) {
        const char* nameStr =
            ((const char* (*)(void*))((void**)nameObj)[4])(nameObj);
        nameLen = strlen(nameStr);
        strcpy(nameBuf, nameStr);
    }

    // Mark the effect as spawned this frame
    self->mFlag134 = 1;
    self->mLastFrame = func_8016A35C();

    // Per-type duration mode
    if (self->mTime == 1) {
        switch (self->mType) {
        case 0:
            effect->mMode = 1;
            break;
        case 1:
            effect->mMode = 2;
            break;
        case 2:
            effect->mMode = 3;
            break;
        }
    } else {
        effect->mMode = 0;
    }

    return 1;
}

// ============================================================================
// func_8018515C (0x801865E4)
// Updates all effects
// ============================================================================
extern "C" void func_8018515C(CREvtEffect* self) {
    if (self->mBdatEntry != 0) {
        // Resolve the entry's name object; used for parent binding below.
        void* nameObj = 0;
        void* nameData = ((CBdatEntry*)self->mBdatEntry)->mNameData;
        if ((s32)nameData >= 0) {
            nameObj = func_8016A27C(nameData);
        }

        f32 fpsDiv = lbl_eu_8066A1F8 / lbl_eu_8066795C;
        f32 half = lbl_eu_80667960;

        // Push the current anim state into every running effect.
        int i = 0;
        while (i < self->mEffectCount) {
            CEffectInst* effect = (CEffectInst*)self->mEffects[i];

            effect->mPosX = self->mAnm1Translate[0];

            // Rotation params: raw word copies of mAnm2Translate.
            effect->mRot[0] = *(s32*)&self->mAnm2Translate[0];
            effect->mRot[1] = *(s32*)&self->mAnm2Translate[1];
            effect->mRot[2] = *(s32*)&self->mAnm2Translate[2];

            // Rotation scaled by the frame-rate divisor. VEC3Scale is an nw4r
            // inline whose ASM emits the retail psq_l/ps_muls0/psq_st sequence.
            nw4r::math::VEC3 rot;
            VEC3Scale(&rot, (const nw4r::math::VEC3*)self->mAnm2Rotate, fpsDiv);
            f32 rot0 = rot.x;
            f32 rot1 = rot.y;
            f32 rot2 = rot.z;
            effect->mRotScaled[0] = *(s32*)&rot0;
            effect->mRotScaled[1] = *(s32*)&rot1;
            effect->mRotScaled[2] = *(s32*)&rot2;

            effect->mScale[0] = *(s32*)&self->mAnm2Scale[0];
            effect->mScale[1] = *(s32*)&self->mAnm2Scale[1];
            effect->mScale[2] = *(s32*)&self->mAnm2Scale[2];
            effect->mScale2[0] = *(s32*)&self->mAnm3Scale[0];
            effect->mScale2[1] = *(s32*)&self->mAnm3Scale[1];
            effect->mScale2[2] = *(s32*)&self->mAnm3Scale[2];
            effect->mFloat4C = *(s32*)&self->mFloat12C;

            // Alpha byte, rounded up by 0.5.
            if (self->mAnm1Scale[2] > half) {
                effect->mField59 = (u8)(int)(half + self->mAnm1Scale[2]);
            }

            // Bind the effect to the name object's model when it accepts it.
            if (effect->mParent == 0 && nameObj != 0) {
                if (((bool (*)(void*))((void**)nameObj)[0x19])(nameObj)) {
                    effect->mFlags |= 0x4000;
                    effect->mParent = nameObj;
                } else {
                    effect->mFlags &= ~0x4000;
                }
            }
            i++;
        }

        // Random chance to clear the spawn flag.
        if (self->mFlag134 != 0 &&
            func_80496288(lbl_eu_80663E14) < lbl_eu_8066794C) {
            self->mFlag134 = 0;
        }

        // Invalidate the last-frame cache once per new frame.
        if (func_80496288(lbl_eu_80663E14) < lbl_eu_8066794C &&
            self->mLastFrame != func_8016A35C()) {
            self->mLastFrame = -1;
        }
    }
}

// ============================================================================
// func_80185378 (0x80186800)
// Initializes with model and bdat data
// r3 = this, r4 = src, r5 = bdatEntry
// ============================================================================
extern "C" void func_80185378(CREvtEffect* self, void* src, void* bdatEntry) {
    if (self->mMaterial != 0) {
        // Release the previous animation resource (virtual, vtable[4]).
        ((void (*)(CREvtEffect*))((void**)self->vtable)[4])(self);

        self->mAnim28 = self->mMaterial;
        self->mMaterial = 0;

        // Drain remaining effects unless the event system is already doing so.
        if (!func_80180954()) {
            while (self->mEffectCount != 0) {
                func_804E3CCC(self->mEffects[0]);
            }
        }

        self->mLastFrame = -1;

        // Free the old model when no new source is provided.
        if (src == 0 && self->mModel != 0) {
            func_80495E60(self->mModel);
            self->mModel = 0;
        }
    }

    if (src != 0) {
        if (self->mModel == 0) {
            // Load the monochrome layout model from the static archive.
            void* handle;
            getStaticFileData__14CLibStaticDataFPCcP16StaticDataHandlePUl(
                &lbl_eu_80503800[0], &handle, 0);

            self->mModel = func_80489A60(lbl_eu_80663E14, handle, 1, 1, 0, 0x50);
            func_80484E5C(self->mModel, lbl_eu_8066793C);
            ((CLibLayoutModel*)self->mModel)->mFlags |= 4;

            // Bind the three anim nodes by name.
            self->mNodeB4 = ((void* (*)(CLibLayoutModel*, const char*))((void**)((CLibLayoutModel*)self->mModel)->vtable)[15])((CLibLayoutModel*)self->mModel, lbl_eu_80662474);
            self->mNodeB8 = ((void* (*)(CLibLayoutModel*, const char*))((void**)((CLibLayoutModel*)self->mModel)->vtable)[15])((CLibLayoutModel*)self->mModel, lbl_eu_80662478);
            self->mNodeBC = ((void* (*)(CLibLayoutModel*, const char*))((void**)((CLibLayoutModel*)self->mModel)->vtable)[15])((CLibLayoutModel*)self->mModel, lbl_eu_80662470);

            // Cache the anim node ids; retail keeps the model in a register
            // across all three calls.
            CLibLayoutModel* animModel = (CLibLayoutModel*)self->mModel;
            self->mNodeIdC0 = ((s32 (*)(CLibLayoutModel*, const char*))((void**)animModel->vtable)[17])(animModel, lbl_eu_80662478);
            self->mNodeIdC4 = ((s32 (*)(CLibLayoutModel*, const char*))((void**)animModel->vtable)[17])(animModel, lbl_eu_80662474);
            self->mNodeIdC8 = ((s32 (*)(CLibLayoutModel*, const char*))((void**)animModel->vtable)[17])(animModel, lbl_eu_80662470);

            func_804827DC(self->mModel, 1);
        }

        // Load the material entry for the source.
        void* material = func_80495EAC(lbl_eu_80663E14, src, 0);
        self->mMaterial = material;
        if (material != 0) {
            func_804839D4(self->mModel, material, 0, 0, 0, 1, -1);

            // Read the duration/type properties from the entry (retail reloads
            // mMaterial for each call rather than keeping a local alive).
            self->mTime = 0;
            self->mType = 0;
            func_8016AF4C(self->mMaterial, &lbl_eu_80503800[4], &self->mTime);
            func_8016AF4C(self->mMaterial, &lbl_eu_80503800[0xF], &self->mType);
        }
    }

    self->mBdatEntry = bdatEntry;
}

// ============================================================================
// func_801855C4 (0x80186A4C)
// Removes an effect by pointer from the array
// r3 = this, r4 = effect pointer
// ============================================================================
extern "C" void func_801855C4(CREvtEffect* self, void* effect) {
    for (int i = 0; i < self->mEffectCount; i++) {
        if (self->mEffects[i] == effect) {
            // Detach from the callback sub-object (self+0x14), null-guarded like retail
            void* parent = (char*)self;
            if (self != 0) {
                parent = (char*)self + 0x14;
            }
            func_804E3D48(effect, parent);

            // Shift the remaining effects down and drop the count
            for (; i < self->mEffectCount - 1; i++) {
                self->mEffects[i] = self->mEffects[i + 1];
            }
            self->mEffectCount = self->mEffectCount - 1;
            break;
        }
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