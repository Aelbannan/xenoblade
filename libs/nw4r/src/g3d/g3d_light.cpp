#include <nw4r/g3d.h>
#include <nw4r/math.h>

#include <algorithm>
#include <cstddef>

namespace nw4r {
namespace g3d {

/******************************************************************************
 *
 * LightObj
 *
 ******************************************************************************/
LightObj::LightObj(const LightObj& rLightObj){
    mFlag = rLightObj.mFlag;
    detail::Copy32ByteBlocks(&mObj, &rLightObj.mObj, sizeof(GXLightObj));
}

// Default ctor defined in this TU (retail header declares it inline) so the
// LightSetting ctor below can inline the mFlag=0 store instead of emitting a
// call to the g3d_state definition (retail emits no call).
inline LightObj::LightObj() : mFlag(0) {}

LightObj& LightObj::operator=(const LightObj& rOther) {
    if (this != &rOther) {
        mFlag = rOther.mFlag;
        detail::Copy32ByteBlocks(&mObj, &rOther.mObj, sizeof(GXLightObj));
    }

    return *this;
}

//Whoever programmed this should feel bad ngl
bool LightObj::operator!=(const LightObj& rOther) const {
    bool result = false;
    
    if(mFlag != rOther.mFlag
        ||mObj.dummy[0] != rOther.mObj.dummy[0]
        ||mObj.dummy[1] != rOther.mObj.dummy[1]
        ||mObj.dummy[2] != rOther.mObj.dummy[2]
        ||mObj.dummy[3] != rOther.mObj.dummy[3]
        ||mObj.dummy[4] != rOther.mObj.dummy[4]
        ||mObj.dummy[5] != rOther.mObj.dummy[5]
        ||mObj.dummy[6] != rOther.mObj.dummy[6]
        ||mObj.dummy[7] != rOther.mObj.dummy[7]
        ||mObj.dummy[8] != rOther.mObj.dummy[8]
        ||mObj.dummy[9] != rOther.mObj.dummy[9]
        ||mObj.dummy[10] != rOther.mObj.dummy[10]
        ||mObj.dummy[11] != rOther.mObj.dummy[11]
        ||mObj.dummy[12] != rOther.mObj.dummy[12]
        ||mObj.dummy[13] != rOther.mObj.dummy[13]
        ||mObj.dummy[14] != rOther.mObj.dummy[14]
        ||mObj.dummy[15] != rOther.mObj.dummy[15]) result = true;
 
    return result;
}

void LightObj::Clear() {
    mFlag = 0;
    detail::ZeroMemory32ByteBlocks(&mObj, sizeof(GXLightObj));
}

void LightObj::InitLightColor(GXColor color) {
    GXInitLightColor(&mObj, color);
}

void LightObj::InitLightPos(f32 x, f32 y, f32 z) {
    GXInitLightPos(&mObj, x, y, z);
    mFlag &= ~FLAG_SPECULAR;
}

void LightObj::InitLightDir(f32 nx, f32 ny, f32 nz) {
    GXInitLightDir(&mObj, nx, ny, nz);
    mFlag &= ~FLAG_SPECULAR;
    mFlag |= FLAG_SPOT;
}

void LightObj::InitLightSpot(f32 cutoff, GXSpotFn spotFn) {
    GXInitLightSpot(&mObj, cutoff, spotFn);
    mFlag &= ~FLAG_SPECULAR;
    mFlag |= FLAG_SPOT;
}

void LightObj::InitLightAttnA(f32 aa, f32 ab, f32 ac) {
    GXInitLightAttnA(&mObj, aa, ab, ac);
    mFlag &= ~FLAG_SPECULAR;
    mFlag |= FLAG_SPOT;
}

void LightObj::InitLightDistAttn(f32 distance, f32 brightness,
                                 GXDistAttnFn distAttnFn) {
    GXInitLightDistAttn(&mObj, distance, brightness, distAttnFn);
    mFlag &= ~FLAG_SPECULAR;
    mFlag |= FLAG_SPOT;
}

void LightObj::InitLightAttnK(f32 ka, f32 kb, f32 kc) {
    GXInitLightAttnK(&mObj, ka, kb, kc);
    mFlag &= ~FLAG_SPECULAR;
    mFlag |= FLAG_SPOT;
}

void LightObj::InitSpecularDir(f32 nx, f32 ny, f32 nz) {
    GXInitLightDir(&mObj, nx, ny, nz);
    mFlag &= ~FLAG_SPOT;
    mFlag |= FLAG_SPECULAR;
    mFlag |= FLAG_SPECULAR_DIR;
}

void LightObj::InitLightShininess(f32 shininess) {
    GXInitLightAttn(&mObj, 0.0f, 0.0f, 1.0f, shininess / 2.0f, 0.0f,
                    1.0f - shininess / 2.0f);
    mFlag &= ~FLAG_SPOT;
    mFlag |= FLAG_SPECULAR;
}

void LightObj::GetLightPos(math::VEC3* pPos) const {
    if (!pPos) {
        return;
    }

    GXGetLightPos(&mObj, &pPos->x, &pPos->y, &pPos->z);
}

void LightObj::GetLightDir(math::VEC3* pDir) const {
    if (!pDir) {
        return;
    }

    GXGetLightDir(&mObj, &pDir->x, &pDir->y, &pDir->z);
}

void LightObj::ApplyViewMtx(const math::MTX34& rCamera) {
    math::VEC3 dir;
    GetLightDir(&dir);
    math::VEC3TransformNormal(&dir, &rCamera, &dir);

    if (IsSpecularDir()) {
        GXInitSpecularDir(&mObj, dir.x, dir.y, dir.z);
    } else {
        math::VEC3 pos;
        GetLightPos(&pos);
        math::VEC3Transform(&pos, &rCamera, &pos);

        GXInitLightPos(&mObj, pos.x, pos.y, pos.z);
        GXInitLightDir(&mObj, dir.x, dir.y, dir.z);
    }
}

/******************************************************************************
 *
 * LightSetting
 *
 ******************************************************************************/
namespace {
// Scratch-light storage.
// an out-of-line __dt__ copy in this TU, and the retail split has room for
// exactly the 23 owned functions — so the four scratch lights live in this
// union instead: `init` provides the explicit mFlag = 0 store (mirroring
// the default ctor retail inlines), while `obj` gives direct member access
// with per-site `sp + offset` addressing exactly like retail's calls.
// (A LightObj&/pointer alias instead holds the address live in a register
// and perturbs allocation; the union itself is never constructed, only its
// trivially-destructible `init` member is written, so no out-of-line dtor
// for the union's own construction path is needed here. MWCC still emits
// weak __dt__ copies for the union and its member under its
// "nontrivial-member" warning; both merge at link and only cost split
// bytes, which is the documented residual below.)
struct LightWork {
    u32 mFlag;       // at 0x0
    GXLightObj mObj; // at 0x4
};

// Scratch-light storage. Naming a LightObj-typed local forces MWCC to emit
// an out-of-line __dt__ copy in this TU, and the retail split has room for
// exactly the 23 owned functions — so the four scratch lights live in this
// union instead: `init` provides the explicit mFlag = 0 store (mirroring
// the default ctor retail inlines), while `obj` gives direct member access
// with per-site `sp + offset` addressing exactly like retail's calls.
// (A LightObj&/pointer alias or an inline-forwarding mirror instead holds
// the address live in a register and perturbs allocation; only the union
// member path reproduces retail's call sequence byte-for-byte.)
// Residual: MWCC warns (10402) on the nontrivial union member and emits two
// weak __dt__ copies (union + member, 0x40 each). Both merge at link and
// neither is referenced; they cost only split bytes (see TU size check).
union LightScratch {
    LightWork init; // write path: explicit mFlag = 0 (mirrors default ctor)
    LightObj obj;   // use path: Clear/Init*/Enable/operator= calls
};
} // namespace
LightSetting::LightSetting(LightObj* pLightObjArray,
                           AmbLightObj* pAmbLightObjArray, u32 numLight,
                           LightSetData* pLightSetDataArray, u32 numLightSet)
    : mNumLight(numLight),
      mNumLightSet(numLightSet),
      mpLightObjArray(pLightObjArray),
      mpAmbLightObjArray(pAmbLightObjArray),
      mpLightSetDataArray(pLightSetDataArray) {

    for (u32 i = 0; i < mNumLightSet; i++) {
        LightSetData& rData = mpLightSetDataArray[i];
        rData.idxAmbLight = LightSetData::IDX_INVALID_LIGHT;

        for (u32 j = 0; j < G3DState::NUM_LIGHT_IN_LIGHT_SET; j++) {
            rData.idxLight[j] = LightSetData::IDX_INVALID_LIGHT;
        }
    }

    mpLightSetDataArray[0].idxLight[0] = 0;
    mpLightSetDataArray[0].idxLight[1] = 2;
    mpLightSetDataArray[0].idxAmbLight = LightSetData::IDX_INVALID_LIGHT;

    GXColor white = {255, 255, 255, 255};
    AmbLightObj ambWhite = {255, 255, 255, 255};

    LightScratch lobj0;
    lobj0.init.mFlag = 0;
    LightScratch lobj1;
    lobj1.init.mFlag = 0;
    LightScratch lobj2;
    lobj2.init.mFlag = 0;
    LightScratch lobj3;
    lobj3.init.mFlag = 0;

    lobj0.obj.Clear();
    lobj1.obj.Clear();
    lobj2.obj.Clear();
    lobj3.obj.Clear();

    lobj0.obj.InitLightColor(white);
    lobj0.obj.InitLightPos(4000000.0f, 4000000.0f, 4000000.0f);
    lobj0.obj.InitLightDir(0.0f, -1.0f, 0.0f);
    lobj0.obj.InitLightSpot(90.0f, GX_SP_OFF);
    lobj0.obj.InitLightDistAttn(10.0f, 0.5f, GX_DA_OFF);
    lobj0.obj.Enable();

    lobj1.obj.InitLightColor(white);
    lobj1.obj.InitLightPos(4000000.0f, 4000000.0f, 4000000.0f);
    lobj1.obj.Enable();

    lobj2.obj.InitLightColor(white);
    lobj2.obj.InitLightPos(4000000.0f, 4000000.0f, 4000000.0f);
    lobj2.obj.InitSpecularDir(0.0f, -1.0f, 0.0f);
    lobj2.obj.InitLightShininess(16.0f);
    lobj2.obj.Enable();

    lobj3.obj.Disable();

    if (mNumLight > 0) {
        mpLightObjArray[0] = lobj0.obj;
        mpAmbLightObjArray[0] = ambWhite;
    }
    if (mNumLight > 1) {
        mpLightObjArray[1] = lobj1.obj;
        mpAmbLightObjArray[1] = ambWhite;
    }
    if (mNumLight > 2) {
        mpLightObjArray[2] = lobj2.obj;
        mpAmbLightObjArray[2] = ambWhite;
    }

    for (u32 i = 3; i < mNumLight; i++) {
        mpLightObjArray[i] = lobj3.obj;
        mpAmbLightObjArray[i] = ambWhite;
    }
}

bool LightSetting::Import(const LightSetting& rSetting) {
    if (mNumLight < rSetting.mNumLight ||
        mNumLightSet < rSetting.mNumLightSet) {
        return false;
    }

    for (u32 i = 0; i < rSetting.mNumLight; i++) {
        if (rSetting.mpLightObjArray[i].IsEnable()) {
            mpLightObjArray[i] = rSetting.mpLightObjArray[i];
        } else {
            mpLightObjArray[i].Disable();
        }

        mpAmbLightObjArray[i] = rSetting.mpAmbLightObjArray[i];
    }

    for (u32 i = 0; i < rSetting.mNumLightSet; i++) {
        mpLightSetDataArray[i] = rSetting.mpLightSetDataArray[i];
    }

    return true;
}

void LightSetting::ApplyViewMtx(const math::MTX34& rCamera, u32 numLight) {
    numLight = std::max<u32>(numLight, mNumLight);

    for (u32 i = 0; i < numLight; i++) {
        if (mpLightObjArray[i].IsEnable()) {
            mpLightObjArray[i].ApplyViewMtx(rCamera);
        }
    }
}

/******************************************************************************
 *
 * LightSet
 *
 ******************************************************************************/
LightObj* LightSet::GetLightObj(u32 lightIdx){
    if (IsValid() && lightIdx < G3DState::NUM_LIGHT_IN_LIGHT_SET) {
        s8 lightObjIdx = mpLightSetData->idxLight[lightIdx];
        if(lightObjIdx >= 0){
            return &mpSetting->GetLightObjArray()[lightObjIdx];
        }
    }

    return NULL;
}

AmbLightObj* LightSet::GetAmbLightObj(){
    if (IsValid() &&  mpLightSetData->idxAmbLight >= 0) {
        return &mpSetting->GetAmbLightObjArray()[mpLightSetData->idxAmbLight];
    }

    return NULL;
}

bool LightSet::SelectLightObj(u32 lightIdx, int lightObjIdx) {
    if (IsValid() && lightIdx < G3DState::NUM_LIGHT_IN_LIGHT_SET) {
        if (lightObjIdx < 0) {
            mpLightSetData->idxLight[lightIdx] =
                LightSetData::IDX_INVALID_LIGHT;

            return true;
        }

        if (lightObjIdx < static_cast<int>(mpSetting->GetNumLightObj())) {
            mpLightSetData->idxLight[lightIdx] = lightObjIdx;
            return true;
        }
    }

    return false;
}

bool LightSet::SelectAmbLightObj(int lightObjIdx) {
    if (IsValid()) {
        if (lightObjIdx < 0) {
            mpLightSetData->idxAmbLight = LightSetData::IDX_INVALID_LIGHT;
            return true;
        }

        if (lightObjIdx < static_cast<int>(mpSetting->GetNumLightObj())) {
            mpLightSetData->idxAmbLight = lightObjIdx;
            return true;
        }
    }

    return false;
}

} // namespace g3d
} // namespace nw4r

