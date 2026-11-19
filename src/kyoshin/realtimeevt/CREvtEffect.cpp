// TU: kyoshin/realtimeevt/CREvtEffect
// Class: cf::CREvtEffect (CREvtObj + private CScheduleChild at 0x14), size 0x144

#include <types.h>
#include "monolib/scn/CScnTimeApi.hpp"
#include <string.h>
#include <nw4r/g3d/res/g3d_resanmchr.h>
#include <nw4r/math/math_types.h>
#include "kyoshin/realtimeevt/CREvtEffect.hpp"
#include "kyoshin/realtimeevt/CREvtObj.hpp"
#include "kyoshin/code_801862C0.hpp"
#include "kyoshin/cf/CfGameManagerData.hpp"  // H3 label-owner decl (gmScene; gmGameFlags)


// C-linkage imports relocated to CREvtEffect.hpp (see the "C-linkage imports"
// section there). This TU still defines the CREvt* functions below.

// ============================================================================
// Constructor (0x80185F54): CREvtObj(1) (effect kind), then the effect fields.
// mNodeB4/B8/BC and mEffects[] are left uninitialized (retail).
// ============================================================================
cf::CREvtEffect::CREvtEffect(CREvtParam* param)
    : CREvtObj(1), mPtr18(0), mParam1C(param), mModel(0), mMaterial(0), mAnim28(0),
      mBdatEntry(0), mEffectCount(0), mNodeIdC0(-1), mNodeIdC4(-1), mNodeIdC8(-1) {
    mAnm1Translate[0] = lbl_eu_8066793C;
    mAnm1Translate[1] = lbl_eu_8066793C;
    mAnm1Translate[2] = lbl_eu_8066793C;
    mAnm1Rotate[0] = lbl_eu_8066793C;
    mAnm1Rotate[1] = lbl_eu_8066793C;
    mAnm1Rotate[2] = lbl_eu_8066793C;
    mAnm1Scale[0] = lbl_eu_8066793C;
    mAnm1Scale[1] = lbl_eu_8066793C;
    mAnm1Scale[2] = lbl_eu_8066793C;

    mAnm2Scale[0] = lbl_eu_80667940;
    mAnm2Scale[1] = lbl_eu_80667940;
    mAnm2Scale[2] = lbl_eu_80667940;

    mAnm2Rotate[0] = lbl_eu_8066793C;
    mAnm2Rotate[1] = lbl_eu_8066793C;
    mAnm2Rotate[2] = lbl_eu_8066793C;
    mAnm2Translate[0] = lbl_eu_8066793C;
    mAnm2Translate[1] = lbl_eu_8066793C;
    mAnm2Translate[2] = lbl_eu_8066793C;
    mAnm3Translate[0] = lbl_eu_8066793C;
    mAnm3Translate[1] = lbl_eu_8066793C;
    mAnm3Translate[2] = lbl_eu_8066793C;

    mAnm3Scale[0] = lbl_eu_80667940;
    mAnm3Scale[1] = lbl_eu_80667940;
    mAnm3Scale[2] = lbl_eu_80667940;
    mFloat12C = lbl_eu_80667940;

    mLastFrame = -1;
    mFlag134 = 0;
    mBdatData = 0;
    mTime = 0;
    mType = 0;

    // Load the bdat block for the param's effect id and register it
    void* bdatData = (void*)EvtSeqResolveListEntryAddr(param->mBdatId);
    mBdatData = bdatData;
    if (bdatData != 0) {
        EffSched_LookupA(&lbl_eu_8065FC18, bdatData);
    }
}

// ============================================================================
// Destructor (0x8018607C). The CREvtObj base dtor and class operator delete
// follow implicitly.
// ============================================================================
cf::CREvtEffect::~CREvtEffect() {
    vfunc_08();

    // Wait for the effect system to release all running effects
    while (mEffectCount != 0) {
        schedClearFlag15Update(mEffects[0]);
    }

    // Release the bdat block, material and model
    if (mBdatData != 0) {
        EffSched_LookupB(&lbl_eu_8065FC18, mBdatData);
    }
    if (mMaterial != 0) {
        Scn_IsAnimActiveOrNull(mMaterial);
        mMaterial = 0;
    }
    if (mModel != 0) {
        Scn_IsAnimActiveOrNull(mModel);
        mModel = 0;
    }
}

// ============================================================================
// EvtFx_FreeAnim (0x80186158)
// Frees mAnim28 resource
// ============================================================================
void cf::CREvtEffect::vfunc_08() {
    if (mAnim28 != 0) {
        Scn_IsAnimActiveOrNull(mAnim28);
        mAnim28 = 0;
    }
}

// ============================================================================
// EvtFx_StepAnims (0x8018619C)
// Calls EvtFx_RefreshChrAnim then EvtFx_UpdateEffects
// ============================================================================
void cf::CREvtEffect::vfunc_1C() {
    refreshChrAnim();
    updateEffects();
}

// ============================================================================
// EvtFx_RefreshChrAnim (0x801861D0)
// Updates animation from model's chr results
// ============================================================================
void cf::CREvtEffect::refreshChrAnim() {
    if (mMaterial == 0) {
        return;
    }

    void* model = mModel;

    // Distinct result locals per anim group: each dies after its Get*
    // trio so model (live across all three) can claim r31 (retail).
    nw4r::g3d::ChrAnmResult* resultEff =
        scnImN4AnimFn(model, lbl_eu_80662478, (f32)(int)EvtSeqGetCounter100());
    resultEff->GetTranslate((nw4r::math::VEC3*)mAnm2Translate);
    resultEff->GetRotateDeg((nw4r::math::VEC3*)mAnm2Rotate);
    resultEff->GetScale((nw4r::math::VEC3*)mAnm2Scale);

    nw4r::g3d::ChrAnmResult* resultAtr =
        scnImN4AnimFn(model, lbl_eu_80662474, (f32)(int)EvtSeqGetCounter100());
    resultAtr->GetTranslate((nw4r::math::VEC3*)mAnm1Translate);
    resultAtr->GetRotateDeg((nw4r::math::VEC3*)mAnm1Rotate);
    resultAtr->GetScale((nw4r::math::VEC3*)mAnm1Scale);

    // Normalize the alpha in mAnm1Scale.z: keep the integer part and drop the
    // fractional part once it falls below the epsilon threshold.
    f32 alpha = mAnm1Scale[2];
    if (alpha > lbl_eu_8066793C) {
        int intPart = (int)alpha;
        f32 fracPart = alpha - (f32)intPart;
        if (fracPart > 0.9f) {
            fracPart = lbl_eu_80667940;
        }
        mAnm1Scale[2] = (f32)(int)mAnm1Scale[2] + fracPart;
    }

    // Copy rotate into mAnm3Scale and translate.z into mFloat12C
    mAnm3Scale[0] = mAnm1Rotate[0];
    mAnm3Scale[1] = mAnm1Rotate[1];
    mAnm3Scale[2] = mAnm1Rotate[2];
    mFloat12C = mAnm1Translate[2];

    // "effTgt" animation -> mAnm3Translate
    nw4r::g3d::ChrAnmResult* resultTgt =
        scnImN4AnimFn(model, lbl_eu_80662470, (f32)(int)EvtSeqGetCounter100());
    resultTgt->GetTranslate((nw4r::math::VEC3*)mAnm3Translate);

    // Refresh the spawned effects when the translate.y threshold is crossed
    if (mAnm1Translate[1] > 0.95f) {
        if (mLastFrame != EvtSeqGetCounter100()) {
            if (Scn_GetFrameDelta(gmScene) >= 0.001f ||
                (Scn_GetFrameDelta(gmScene) < 0.001f &&
                 mFlag134 == 0)) {
                spawnEffect();
            }
        }
    }
}

// ============================================================================
// EvtFx_SpawnEffect (0x801863D0)
// Spawns an effect
// ============================================================================
bool cf::CREvtEffect::spawnEffect() {
    // Max 32 concurrent effects
    if (mEffectCount >= 32) {
        return 0;
    }
    if (mBdatData == 0) {
        return 0;
    }

    // Language gating: default to language 1 when the console language is exotic.
    // lang is kept raw (upper bits garbage) and masked per use, like retail.
    int lang = getLanguage__9CDeviceSCFv();
    if ((lang & 0xFF) > 5) {
        lang = 1;
    }

    // The bdat entry may declare a specific language (bits 24-31)
    u32 langByte = mParam1C->mLangInfo >> 24;
    if (langByte != (lang & 0xFF) && langByte != 0xFF && langByte != 0) {
        return 0;
    }

    // Create the effect instance and register it in the slot array
    CEffectInst* effect = (CEffectInst*)EffRoot_AddSchedReq(
        &lbl_eu_8065FC18, mBdatData, gmScene, 0, 1, 0);
    if (effect == 0) {
        return 0;
    }

    int count = mEffectCount;
    mEffects[count] = effect;
    mEffectCount = count + 1;

    // Bind to the callback sub-object (self+0x14), null-guarded like retail
    schedAttachChildSlot(effect, static_cast<CScheduleChild*>(this));

    // Scale: mAnm1Scale.y clamped below -1.0 to 0.0
    f32 scale = mAnm1Scale[1];
    if (scale < 0.5f) {
        scale = lbl_eu_80667940;
    }

    // Culling mode forces effect state 7
    if (EvtSeqIsSlotIndexFour()) {
        effect->mField59 = 7;
    }

    schedSetStepIntervals(effect, mAnm1Scale[0], scale);

    // Priority from the bdat entry
    if (((CBdatEntry*)mBdatEntry)->mPriority >= 0) {
        effect->mPriority = (u8)((CBdatEntry*)mBdatEntry)->mPriority;
    }

    // Copy the entry's name into a local buffer
    void* nameObj = EvtSeqFindEventByNameData(((CBdatEntry*)mBdatEntry)->mNameData);
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
    mFlag134 = 1;
    mLastFrame = EvtSeqGetCounter100();

    // Per-type duration mode
    if (mTime == 1) {
        switch (mType) {
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
// EvtFx_UpdateEffects (0x801865E4)
// Updates all effects
// ============================================================================
void cf::CREvtEffect::updateEffects() {
    if (mBdatEntry != 0) {
        // Resolve the entry's name object; used for parent binding below.
        void* nameObj = 0;
        void* nameData = ((CBdatEntry*)mBdatEntry)->mNameData;
        if ((s32)nameData >= 0) {
            nameObj = EvtSeqFindEventByNameData(nameData);
        }

        f32 fpsDiv = ml::pi / 180.0f;
        f32 half = 0.1f;

        // Push the current anim state into every running effect.
        int i = 0;
        while (i < mEffectCount) {
            CEffectInst* effect = (CEffectInst*)mEffects[i];

            effect->mPosX = mAnm1Translate[0];

            // Rotation params: raw word copies of mAnm2Translate.
            effect->mRot[0] = *(s32*)&mAnm2Translate[0];
            effect->mRot[1] = *(s32*)&mAnm2Translate[1];
            effect->mRot[2] = *(s32*)&mAnm2Translate[2];

            // Rotation scaled by the frame-rate divisor. VEC3Scale is an nw4r
            // inline whose ASM emits the retail psq_l/ps_muls0/psq_st sequence.
            nw4r::math::VEC3 rot;
            VEC3Scale(&rot, (const nw4r::math::VEC3*)mAnm2Rotate, fpsDiv);
            f32 rot0 = rot.x;
            f32 rot1 = rot.y;
            f32 rot2 = rot.z;
            effect->mRotScaled[0] = *(s32*)&rot0;
            effect->mRotScaled[1] = *(s32*)&rot1;
            effect->mRotScaled[2] = *(s32*)&rot2;

            effect->mScale[0] = *(s32*)&mAnm2Scale[0];
            effect->mScale[1] = *(s32*)&mAnm2Scale[1];
            effect->mScale[2] = *(s32*)&mAnm2Scale[2];
            effect->mScale2[0] = *(s32*)&mAnm3Scale[0];
            effect->mScale2[1] = *(s32*)&mAnm3Scale[1];
            effect->mScale2[2] = *(s32*)&mAnm3Scale[2];
            effect->mFloat4C = *(s32*)&mFloat12C;

            // Alpha byte, rounded up by 0.5.
            if (mAnm1Scale[2] > half) {
                effect->mField59 = (u8)(int)(half + mAnm1Scale[2]);
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
        if (mFlag134 != 0 &&
            Scn_GetFrameDelta(gmScene) < 0.001f) {
            mFlag134 = 0;
        }

        // Invalidate the last-frame cache once per new frame.
        if (Scn_GetFrameDelta(gmScene) < 0.001f &&
            mLastFrame != EvtSeqGetCounter100()) {
            mLastFrame = -1;
        }
    }
}

// ============================================================================
// EvtFx_InitModel (0x80186800)
// Initializes with model and bdat data
// r3 = this, r4 = src, r5 = bdatEntry
// ============================================================================
void cf::CREvtEffect::initModel(void* src, void* bdatEntry) {
    if (mMaterial != 0) {
        // Release the previous animation resource (virtual, vtable[4]).
        vfunc_08();

        mAnim28 = mMaterial;
        mMaterial = 0;

        // Drain remaining effects unless the event system is already doing so.
        if (!REvtCam_GetFieldA5Flag()) {
            while (mEffectCount != 0) {
                schedClearFlag15Update(mEffects[0]);
            }
        }

        mLastFrame = -1;

        // Free the old model when no new source is provided.
        if (src == 0 && mModel != 0) {
            Scn_IsAnimActiveOrNull(mModel);
            mModel = 0;
        }
    }

    if (src != 0) {
        if (mModel == 0) {
            // Load the monochrome layout model from the static archive.
            void* handle;
            getStaticFileData__14CLibStaticDataFPCcP16StaticDataHandlePUl(
                &lbl_eu_80503800[0], &handle, 0);

            mModel = scnImN4BuildByIdx(gmScene, handle, 1, 1, 0, 0x50);
            simSetLeafDist7B0(mModel, lbl_eu_8066793C);
            ((CLibLayoutModel*)mModel)->mFlags |= 4;

            // Bind the three anim nodes by name.
            mNodeB4 = ((void* (*)(CLibLayoutModel*, const char*))((void**)((CLibLayoutModel*)mModel)->vtable)[15])((CLibLayoutModel*)mModel, lbl_eu_80662474);
            mNodeB8 = ((void* (*)(CLibLayoutModel*, const char*))((void**)((CLibLayoutModel*)mModel)->vtable)[15])((CLibLayoutModel*)mModel, lbl_eu_80662478);
            mNodeBC = ((void* (*)(CLibLayoutModel*, const char*))((void**)((CLibLayoutModel*)mModel)->vtable)[15])((CLibLayoutModel*)mModel, lbl_eu_80662470);

            // Cache the anim node ids; retail keeps the model in a register
            // across all three calls.
            CLibLayoutModel* animModel = (CLibLayoutModel*)mModel;
            mNodeIdC0 = ((s32 (*)(CLibLayoutModel*, const char*))((void**)animModel->vtable)[17])(animModel, lbl_eu_80662478);
            mNodeIdC4 = ((s32 (*)(CLibLayoutModel*, const char*))((void**)animModel->vtable)[17])(animModel, lbl_eu_80662474);
            mNodeIdC8 = ((s32 (*)(CLibLayoutModel*, const char*))((void**)animModel->vtable)[17])(animModel, lbl_eu_80662470);

            simSetFlag2000Chain(mModel, 1);
        }

        // Load the material entry for the source.
        void* material = Scn_InitGlobalA(gmScene, src, 0);
        mMaterial = material;
        if (material != 0) {
            simBindChrAnim(mModel, material, 0, 0, 0, 1, -1);

            // Read the duration/type properties from the entry (retail reloads
            // mMaterial for each call rather than keeping a local alive).
            mTime = 0;
            mType = 0;
            EvtSeqResolveAnmChr((u8*)mMaterial, &lbl_eu_80503800[4], &mTime);
            EvtSeqResolveAnmChr((u8*)mMaterial, &lbl_eu_80503800[0xF], &mType);
        }
    }

    mBdatEntry = bdatEntry;
}

// ============================================================================
// EvtFx_DetachEffect (0x80186A4C)
// Removes an effect by pointer from the array
// r3 = this, r4 = effect pointer
// ============================================================================
void cf::CREvtEffect::OnScheduleDelete(CSchedule* effect) {
    for (int i = 0; i < mEffectCount; i++) {
        if (mEffects[i] == effect) {
            // Detach from the callback sub-object (self+0x14), null-guarded like retail
            schedDetachChildSlot(effect, static_cast<CScheduleChild*>(this));

            // Shift the remaining effects down and drop the count
            for (; i < mEffectCount - 1; i++) {
                mEffects[i] = mEffects[i + 1];
            }
            mEffectCount = mEffectCount - 1;
            break;
        }
    }
}

// ============================================================================
// EvtFx_GetModel (0x80186AFC)
// Returns mModel
// ============================================================================
int cf::CREvtEffect::vfunc_0C() {
    return (int)mModel;
}

// ============================================================================
// EvtFx_Noop (0x80186B04)
// Empty function
// ============================================================================
void cf::CREvtEffect::vfunc_04() {
}

