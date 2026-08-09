#include <nw4r/g3d.h>

#include <algorithm>

namespace nw4r {
namespace g3d {

AnmScn::~AnmScn() {}

void AnmScn::GetLightSetting(LightSetting* pSetting) {
    const u32 numLightSet = GetLightSetMaxRefNumber();
    const u32 numAmbLight = GetAmbLightMaxRefNumber();
    const u32 numDiffLight = GetDiffuseLightMaxRefNumber();

    if (numLightSet > 0) {
        const u32 numLightSetObj = pSetting->GetNumLightSet();
        const u32 numLoadableSet = std::min(numLightSet, numLightSetObj);

        for (u32 i = 0; i < numLoadableSet; i++) {
            LightSet set = pSetting->GetLightSet(i);
            GetLightSet(set, i);
        }
    }

    if (numAmbLight > 0) {
        AmbLightObj* pAmbObjArray = pSetting->GetAmbLightObjArray();
        const u32 numAmbObj = pSetting->GetNumLightObj();
        const u32 numLoadableAmb = std::min(numAmbLight, numAmbObj);

        for (u32 i = 0; i < numLoadableAmb; i++) {
            AmbLightObj* pAmbObj = &pAmbObjArray[i];
            *reinterpret_cast<u32*>(&pAmbObj->r) = GetAmbLightColor(i);
        }
    }

    if (numDiffLight > 0) {
        LightObj* pLightObjArray = pSetting->GetLightObjArray();
        const u32 numLightObj = pSetting->GetNumLightObj();
        const u32 numSpecLight = GetNumSpecularLight();

        const u32 numLight = numDiffLight + numSpecLight;
        const u32 numLoadableDiffLight = std::min(numDiffLight, numLightObj);
        const u32 numLoadableLight = std::min(numLight, numLightObj);

        for (u32 i = 0; i < numLoadableDiffLight; i++) {
            LightObj* pObj = &pLightObjArray[i];
            pObj->Disable();
        }

        for (u32 i = 0; i < numLoadableDiffLight; i++) {
            LightObj* pDiffObj = &pLightObjArray[i];
            LightObj* pSpecObj = NULL;

            if (pDiffObj->IsEnable()) {
                continue;
            }

            if (HasSpecularLight(i)) {
                const u32 specId = GetSpecularLightID(i);
                if (specId < numLoadableLight) {
                    pSpecObj = &pLightObjArray[specId];
                }
            }

            GetLight(pDiffObj, pSpecObj, i);
        }
    }
}

extern "C" const nw4r::g3d::G3dObj::ResNameDataT<sizeof("AnmScn")> lbl_eu_8051D620 = {sizeof("AnmScn"), "AnmScn"};

bool AnmScn::IsDerivedFrom(G3dObj::TypeObj other) const {
    return other == TypeObj(lbl_eu_8051D620) ? true
         : (other == TypeObj(lbl_eu_8051D640));
}

const G3dObj::TypeObj AnmScn::GetTypeObj() const {
    return TypeObj(lbl_eu_8051D620);
}

const char* AnmScn::GetTypeName() const {
    return GetTypeObj().GetTypeName();
}


extern "C" const nw4r::g3d::G3dObj::ResNameDataT<sizeof("AnmScnRes")> lbl_eu_8051D630 = {sizeof("AnmScnRes"), "AnmScnRes"};

bool AnmScnRes::IsDerivedFrom(G3dObj::TypeObj other) const {
    return other == TypeObj(lbl_eu_8051D630) ? true
         : other == TypeObj(lbl_eu_8051D620) ? true
         : (other == TypeObj(lbl_eu_8051D640));
}

const G3dObj::TypeObj AnmScnRes::GetTypeObj() const {
    return TypeObj(lbl_eu_8051D630);
}

const char* AnmScnRes::GetTypeName() const {
    return GetTypeObj().GetTypeName();
}


} // namespace g3d
} // namespace nw4r

void GetLightSet__Q34nw4r3g3d12LightSettingFi(nw4r::g3d::LightSet* result, nw4r::g3d::LightSetting* self, int index) {
    nw4r::g3d::LightSetting* setting = self;
    nw4r::g3d::LightSet* lightSet = result;
    *lightSet = setting->GetLightSet(index);
}
int Attach__Q34nw4r3g3d6AnmScnFiPQ34nw4r3g3d9AnmScnRes(void) { return 0x0; }
int Detach__Q34nw4r3g3d6AnmScnFi(int) { return 0; }
unsigned int GetResLightSetNumEntries__Q34nw4r3g3d9ResAnmScnCFv(const void*);

unsigned int GetNumLightSet__Q34nw4r3g3d9AnmScnResCFv(const nw4r::g3d::AnmScnRes* this_ptr) {
    return GetResLightSetNumEntries__Q34nw4r3g3d9ResAnmScnCFv(
        reinterpret_cast<const char*>(this_ptr) + 0x20);
}
int GetResAnmAmbLightNumEntries__Q34nw4r3g3d9ResAnmScnCFv(const void*);

int GetNumAmbLight__Q34nw4r3g3d9AnmScnResCFv(const nw4r::g3d::AnmScnRes* ths) {
    return GetResAnmAmbLightNumEntries__Q34nw4r3g3d9ResAnmScnCFv(
        reinterpret_cast<const char*>(ths) + 0x20);
}
u32 GetResAnmLightNumEntries__Q34nw4r3g3d9ResAnmScnCFv(const void *);

u32 GetNumDiffuseLight__Q34nw4r3g3d9AnmScnResCFv(const nw4r::g3d::AnmScnRes* self) {
    return GetResAnmLightNumEntries__Q34nw4r3g3d9ResAnmScnCFv(
        reinterpret_cast<const void*>(reinterpret_cast<const char*>(self) + 0x20));
}
unsigned short GetNumSpecularLight__Q34nw4r3g3d9AnmScnResCFv(const nw4r::g3d::AnmScnRes* __this) {
    return *reinterpret_cast<const unsigned short*>(
        *reinterpret_cast<const unsigned char* const*>(
            reinterpret_cast<const unsigned char*>(__this) + 0x20) + 0x36);
}
int GetNumFog__Q34nw4r3g3d9AnmScnResCFv(const nw4r::g3d::AnmScnRes* this_) {
    extern int GetResAnmFogNumEntries__Q34nw4r3g3d9ResAnmScnCFv(const void*);
    return GetResAnmFogNumEntries__Q34nw4r3g3d9ResAnmScnCFv(reinterpret_cast<const char*>(this_) + 0x20);
}
void GetResAnmCameraNumEntries__Q34nw4r3g3d9ResAnmScnCFv(void*);
void GetNumCamera__Q34nw4r3g3d9AnmScnResCFv(nw4r::g3d::AnmScnRes* self) {
    reinterpret_cast<void(*)(void*)>(GetResAnmCameraNumEntries__Q34nw4r3g3d9ResAnmScnCFv)(
        reinterpret_cast<char*>(self) + 0x20);
}
u16 GetLightSetMaxRefNumber__Q34nw4r3g3d9AnmScnResCFv(const nw4r::g3d::AnmScnRes* _this) {
    return *reinterpret_cast<const u16*>(*reinterpret_cast<const u8* const*>(reinterpret_cast<const u8*>(_this) + 0x20) + 0x3c);
}
unsigned short GetAmbLightMaxRefNumber__Q34nw4r3g3d9AnmScnResCFv(const nw4r::g3d::AnmScnRes* self) {
    const unsigned char* res = *reinterpret_cast<const unsigned char* const*>(
        reinterpret_cast<const unsigned char*>(self) + 0x20);
    return *reinterpret_cast<const unsigned short*>(res + 0x3e);
}
int GetDiffuseLightMaxRefNumber__Q34nw4r3g3d9AnmScnResCFv(const nw4r::g3d::AnmScnRes* p) {
    return *reinterpret_cast<const unsigned short*>(
        *reinterpret_cast<const char* const*>(
            reinterpret_cast<const char*>(p) + 0x20) + 0x40);
}
unsigned short GetFogMaxRefNumber__Q34nw4r3g3d9AnmScnResCFv(const nw4r::g3d::AnmScnRes* this_) {
    return *reinterpret_cast<const unsigned short*>(
        *reinterpret_cast<const char* const*>(
            reinterpret_cast<const char*>(this_) + 0x20) + 0x42);
}
u16 GetCameraMaxRefNumber__Q34nw4r3g3d9AnmScnResCFv(const nw4r::g3d::AnmScnRes* pThis) {
    return *reinterpret_cast<const u16*>(
        reinterpret_cast<const u8*>(
            *reinterpret_cast<const u32*>(reinterpret_cast<const u8*>(pThis) + 0x20)) + 0x44);
}
namespace nw4r {
namespace g3d {

void AnmScnRes::G3dProc(u32 task, u32 arg, void* pArg) {
    switch (task) {
    case G3DPROC_UPDATEFRAME:
        UpdateFrame();
        break;
    case G3DPROC_DETACH_PARENT:
        SetParent(NULL);
        break;
    case G3DPROC_ATTACH_PARENT:
        SetParent(static_cast<G3dObj*>(pArg));
        break;
    default:
        break;
    }
}

AnmScnRes::AnmScnRes(MEMAllocator* pAllocator, ResAnmScn res,
                     AmbLightAnmResult* pAmbCache,
                     LightAnmResult* pLightCache, FogAnmResult* pFogCache,
                     CameraAnmResult* pCameraCache)
    : AnmScn(pAllocator),
      FrameCtrl(0.0f, static_cast<f32>(res.ref().info.numFrame),
                GetAnmPlayPolicy(res.ref().info.policy)),
      mRes(res) {
    mUpdateCacheFlag = 0;
    mpAmbLightCache = pAmbCache;
    mpLightCache = pLightCache;
    mpFogCache = pFogCache;
    mpCameraCache = pCameraCache;

    if (pAmbCache != NULL || pLightCache != NULL || pFogCache != NULL ||
        pCameraCache != NULL) {
        mUpdateCacheFlag |= 1;
    }

    if (mUpdateCacheFlag & 1) {
        UpdateCache();
    }
}

u32 AnmScnRes::GetNumLightSet() const {
    return mRes.GetResLightSetNumEntries();
}

u32 AnmScnRes::GetNumAmbLight() const {
    return mRes.GetResAnmAmbLightNumEntries();
}

u32 AnmScnRes::GetNumDiffuseLight() const {
    return mRes.GetResAnmLightNumEntries();
}

u32 AnmScnRes::GetNumSpecularLight() const {
    return mRes.ref().info.numSpecularLight;
}

u32 AnmScnRes::GetNumFog() const {
    return mRes.GetResAnmFogNumEntries();
}

u32 AnmScnRes::GetNumCamera() const {
    return mRes.GetResAnmCameraNumEntries();
}

u32 AnmScnRes::GetLightSetMaxRefNumber() const {
    return mRes.ref().info.numResLightSetData;
}

u32 AnmScnRes::GetAmbLightMaxRefNumber() const {
    return mRes.ref().info.numResAnmAmbLightData;
}

u32 AnmScnRes::GetDiffuseLightMaxRefNumber() const {
    return mRes.ref().info.numResAnmLightData;
}

u32 AnmScnRes::GetFogMaxRefNumber() const {
    return mRes.ref().info.numResAnmFogData;
}

u32 AnmScnRes::GetCameraMaxRefNumber() const {
    return mRes.ref().info.numResAnmCameraData;
}

AnmScnRes* AnmScnRes::Construct(MEMAllocator* pAllocator, u32* pSize,
                                ResAnmScn res, void* pUserData) {
    if (!res.IsValid()) {
        return NULL;
    }

    u32 ambNum;

    if (pUserData != NULL) {
        ambNum = res.GetResAnmAmbLightNumEntries();
    } else {
        ambNum = 0;
    }

    u32 lightNum;

    if (pUserData != NULL) {
        lightNum = res.GetResAnmLightNumEntries();
    } else {
        lightNum = 0;
    }

    u32 fogNum;

    if (pUserData != NULL) {
        fogNum = res.GetResAnmFogNumEntries();
    } else {
        fogNum = 0;
    }

    u32 camNum;

    if (pUserData != NULL) {
        camNum = res.GetResAnmCameraNumEntries();
    } else {
        camNum = 0;
    }

    u32 ambCacheSize =
        ut::RoundUp<u32>(sizeof(AnmScnRes) +
                             ambNum * sizeof(AmbLightAnmResult), 4);
    u32 lightCacheSize =
        ut::RoundUp<u32>(ambCacheSize + lightNum * sizeof(LightAnmResult), 4);
    u32 fogCacheSize =
        ut::RoundUp<u32>(lightCacheSize + fogNum * sizeof(FogAnmResult), 4);
    u32 totalSize =
        ut::RoundUp<u32>(fogCacheSize + camNum * sizeof(CameraAnmResult), 4);

    if (pSize != NULL) {
        *pSize = totalSize;
    }

    if (pAllocator == NULL) {
        return NULL;
    }

    void* pMem = MEMAllocFromAllocator(pAllocator, totalSize);

    if (pMem == NULL) {
        return NULL;
    }

    AmbLightAnmResult* pAmbCache = NULL;

    if (ambNum != 0) {
        pAmbCache = reinterpret_cast<AmbLightAnmResult*>(
            static_cast<char*>(pMem) + 0x38);
    }

    LightAnmResult* pLightCache = NULL;

    if (lightNum != 0) {
        pLightCache = reinterpret_cast<LightAnmResult*>(
            static_cast<char*>(pMem) + ambCacheSize);
    }

    FogAnmResult* pFogCache = NULL;

    if (fogNum != 0) {
        pFogCache = reinterpret_cast<FogAnmResult*>(
            static_cast<char*>(pMem) + lightCacheSize);
    }

    CameraAnmResult* pCameraCache = NULL;

    if (camNum != 0) {
        pCameraCache = reinterpret_cast<CameraAnmResult*>(
            static_cast<char*>(pMem) + fogCacheSize);
    }

    return new (pMem) AnmScnRes(pAllocator, res, pAmbCache, pLightCache,
                                pFogCache, pCameraCache);
}

} // namespace g3d
} // namespace nw4r


namespace nw4r {
namespace g3d {

void AnmScnRes::SetFrame(f32 frame) {
    SetFrm(frame);

    if (mUpdateCacheFlag & 1) {
        UpdateCache();
    }
}

} // namespace g3d
} // namespace nw4r


float GetFrame__Q34nw4r3g3d9AnmScnResCFv(const nw4r::g3d::AnmScnRes* this_ptr) {
    return *(const float*)((const unsigned char*)this_ptr + 0xC);
}
namespace nw4r {
namespace g3d {

void AnmScnRes::SetUpdateRate(f32 rate) {
    SetRate(rate);

    if (rate == 1.0f && (mUpdateCacheFlag & 1)) {
        UpdateCache();
    }
}

} // namespace g3d
} // namespace nw4r


float GetUpdateRate__Q34nw4r3g3d9AnmScnResCFv(const nw4r::g3d::AnmScnRes* this_) { return *(const float*)((const char*)this_ + 0x10); }
namespace nw4r {
namespace g3d {

void AnmScnRes::UpdateFrame() {
    if (GetRate() != 1.0f) {
        UpdateFrm();

        if (mUpdateCacheFlag & 1) {
            UpdateCache();
        }
    }
}

} // namespace g3d
} // namespace nw4r


void G3dProc__Q34nw4r3g3d9AnmScnResFUlUlPv(){}
namespace nw4r {
namespace g3d {

bool AnmScnRes::GetLightSet(LightSet set, u32 refNumber) {
    ResLightSet lightSet = mRes.GetResLightSetByRefNumber(refNumber);

    if (!lightSet.IsValid()) {
        return false;
    }

    u32 numLight = lightSet.ref().numLight;
    u32 numSpecular = 7;
    u32 i = 0;

    if (numLight != 0) {
        for (; i < numLight; i++) {
            u16 lightIdx = lightSet.ref().lightId[i];

            if (lightIdx != ResLightSetData::INVALID_ID) {
                ResAnmLight light = mRes.GetResAnmLight(lightIdx);

                set.SelectLightObj(i, light.ref().refNumber);

                if (light.ref().flags &
                    ResAnmLightData::FLAG_SPECULAR_ENABLE) {
                    set.SelectLightObj(numSpecular--, light.ref().specLightObjIdx);
                }
            } else {
                set.SelectLightObj(i, -1);
            }
        }
    }

    for (u32 i = numLight; i <= numSpecular; i++) {
        set.SelectLightObj(i, -1);
    }

    if (lightSet.ref().ambLightName != 0) {
        ResAnmAmbLight amb =
            mRes.GetResAnmAmbLight(lightSet.ref().ambLightId);
        set.SelectAmbLightObj(amb.GetRefNumber());
    } else {
        set.SelectAmbLightObj(-1);
    }

    return true;
}

} // namespace g3d
} // namespace nw4r


namespace nw4r {
namespace g3d {

ut::Color AnmScnRes::GetAmbLightColor(u32 refNumber) {
    AmbLightAnmResult result;

    AmbLightAnmResult* pResult = GetAmbLightResult(&result, refNumber);

    u32 color = pResult->color;

    if (!(pResult->flags & AmbLightAnmResult::FLAG_COLOR_ENABLE)) {
        color |= 0xFFFFFF00;
    }

    if (!(pResult->flags & AmbLightAnmResult::FLAG_ALPHA_ENABLE)) {
        color |= 0xFF;
    }

    return ut::Color(color);
}

} // namespace g3d
} // namespace nw4r


namespace nw4r {
namespace g3d {
namespace {

void MakeDiffuseLightObj(LightObj* pLightObj, const LightAnmResult* pResult) {
    u32 lightType = pResult->flags & LightAnmResult::FLAG_LIGHT_TYPE_MASK;

    pLightObj->Clear();

    switch (lightType) {
    case 0: {
        pLightObj->InitLightColor(pResult->color);
        pLightObj->InitLightPos(pResult->pos.x,
                               pResult->pos.y, pResult->pos.z);
        pLightObj->InitLightSpot(0.0f, GX_SP_OFF);
        pLightObj->InitLightDistAttn(pResult->refDistance,
                                     pResult->refBrightness,
                                     pResult->distFunc);
        pLightObj->InitLightDir(0.0f, 0.0f, 0.0f);
        break;
    }
    case 1: {
        math::VEC3 dir = pResult->aim - pResult->pos;
        math::VEC3 pos = dir * -10000000000.0f;

        pLightObj->InitLightColor(pResult->color);
        pLightObj->InitLightPos(pos.x, pos.y, pos.z);
        pLightObj->InitLightAttnA(1.0f, 0.0f, 0.0f);
        pLightObj->InitLightAttnK(1.0f, 0.0f, 0.0f);
        pLightObj->InitLightDir(0.0f, 0.0f, 0.0f);
        break;
    }
    case 2: {
        math::VEC3 dir = pResult->aim - pResult->pos;

        PSVECNormalize(reinterpret_cast<const Vec*>(&dir),
                      reinterpret_cast<Vec*>(&dir));

        pLightObj->InitLightColor(pResult->color);
        pLightObj->InitLightPos(pResult->pos.x,
                               pResult->pos.y, pResult->pos.z);
        pLightObj->InitLightSpot(pResult->cutoff, pResult->spotFunc);
        pLightObj->InitLightDistAttn(pResult->refDistance,
                                     pResult->refBrightness,
                                     pResult->distFunc);
        pLightObj->InitLightDir(dir.x, dir.y, dir.z);
        break;
    }
    default:
        break;
    }

    if (!(pResult->flags & LightAnmResult::FLAG_COLOR_ENABLE)) {
        pLightObj->DisableColor();
    }

    if (!(pResult->flags & LightAnmResult::FLAG_ALPHA_ENABLE)) {
        pLightObj->DisableAlpha();
    }

    pLightObj->Enable();
}

} // namespace
} // namespace g3d
} // namespace nw4r

namespace nw4r {
namespace g3d {

void AnmScnRes::GetLight(LightObj* pDiff, LightObj* pSpec, u32 refNumber) {
    LightAnmResult result;

    result.color = -1;
    result.specColor = -1;

    LightAnmResult* pResult = GetLightResult(&result, refNumber);

    if (!(pResult->flags & LightAnmResult::FLAG_LIGHT_ENABLE)) {
        if (pDiff != NULL) {
            pDiff->mFlag &= ~(LightObj::FLAG_SPECULAR | LightObj::FLAG_ENABLE_LIGHT |
                              LightObj::FLAG_SPECULAR_DIR);
        }

        if (pSpec != NULL) {
            pSpec->mFlag &= ~(LightObj::FLAG_SPECULAR | LightObj::FLAG_ENABLE_LIGHT |
                              LightObj::FLAG_SPECULAR_DIR);
        }

        return;
    }

    if (pDiff != NULL) {
        MakeDiffuseLightObj(pDiff, pResult);
    }

    if (pSpec != NULL) {
        if (pResult->flags & LightAnmResult::FLAG_SPECULAR_ENABLE) {
            math::VEC3 dir = pResult->aim - pResult->pos;

            PSVECNormalize(reinterpret_cast<const Vec*>(&dir),
                      reinterpret_cast<Vec*>(&dir));

            pSpec->Clear();
            pSpec->InitLightColor(static_cast<GXColor>(pResult->specColor));
            pSpec->InitSpecularDir(dir.x, dir.y, dir.z);
            pSpec->InitLightShininess(pResult->shininess);

            if (!(pResult->flags & LightAnmResult::FLAG_COLOR_ENABLE)) {
                pSpec->DisableColor();
            }

            if (!(pResult->flags & LightAnmResult::FLAG_ALPHA_ENABLE)) {
                pSpec->DisableAlpha();
            }

            pSpec->Enable();
        } else {
            pSpec->mFlag &= ~(LightObj::FLAG_SPECULAR | LightObj::FLAG_ENABLE_LIGHT |
                              LightObj::FLAG_SPECULAR_DIR);
        }
    }
}

} // namespace g3d
} // namespace nw4r


namespace nw4r {
namespace g3d {

void AnmScnRes::GetFog(Fog fog, u32 refNumber) {
    FogAnmResult result;

    result.color = -1;

    FogAnmResult* pResult = GetFogResult(&result, refNumber);

    if (fog.ptr() != NULL) {
        fog.ptr()->type = pResult->type;
    }

    if (fog.ptr() != NULL) {
        fog.ptr()->startz = pResult->startz;
        fog.ptr()->endz = pResult->endz;
    }

    if (fog.ptr() != NULL) {
        fog.ptr()->color = pResult->color;
    }
}

} // namespace g3d
} // namespace nw4r


namespace nw4r {
namespace g3d {

bool AnmScnRes::GetCamera(Camera camera, u32 refNumber) {
    CameraAnmResult result;

    CameraAnmResult* pResult = GetCameraResult(&result, refNumber);

    if (!(pResult->flags & CameraAnmResult::FLAG_ANM_EXISTS)) {
        return false;
    }

    switch (pResult->projType) {
    case GX_PERSPECTIVE: {
        camera.SetPerspective(pResult->perspFovy, pResult->aspect,
                              pResult->near, pResult->far);
        break;
    }
    case GX_ORTHOGRAPHIC: {
        f32 height = pResult->orthoHeight * 2.0f;
        f32 width = height * pResult->aspect;

        camera.SetOrtho(height, -height, -width, width, pResult->near,
                        pResult->far);
        break;
    }
    default:
        break;
    }

    Camera::PostureInfo posture;

    switch (pResult->flags & CameraAnmResult::FLAG_CAMERA_TYPE_MASK) {
    case 0: {
        posture.tp = Camera::POSTURE_ROTATE;

        math::VEC3 rotate = pResult->rotate.rot;

        posture.cameraRotate = rotate;
        break;
    }
    case 1: {
        posture.tp = Camera::POSTURE_AIM;

        math::VEC3 aim = pResult->aim.aim;

        posture.cameraTarget = aim;
        posture.cameraTwist = pResult->aim.twist;
        break;
    }
    default:
        break;
    }

    camera.SetPosition(pResult->pos);
    camera.SetPosture(posture);

    return true;
}

} // namespace g3d
} // namespace nw4r


namespace nw4r {
namespace g3d {

AmbLightAnmResult* AnmScnRes::GetAmbLightResult(AmbLightAnmResult* pResult,
                                                u32 idx) {
    ResAnmAmbLight amb = mRes.GetResAnmAmbLightByRefNumber(idx);

    if (!amb.IsValid()) {
        pResult->flags = 0;
        pResult->color = -1;
        return pResult;
    }

    if (mpAmbLightCache != NULL) {
        return &mpAmbLightCache[amb.GetID()];
    }

    amb.GetAnmResult(pResult, GetFrm());
    return pResult;
}

LightAnmResult* AnmScnRes::GetLightResult(LightAnmResult* pResult, u32 idx) {
    ResAnmLight light = mRes.GetResAnmLightByRefNumber(idx);

    if (!light.IsValid()) {
        pResult->flags = 0;
        return pResult;
    }

    if (mpLightCache != NULL) {
        return &mpLightCache[light.GetID()];
    }

    light.GetAnmResult(pResult, GetFrm());
    return pResult;
}

FogAnmResult* AnmScnRes::GetFogResult(FogAnmResult* pResult, u32 idx) {
    ResAnmFog fog = mRes.GetResAnmFogByRefNumber(idx);

    if (!fog.IsValid()) {
        pResult->type = GX_FOG_NONE;
        return pResult;
    }

    if (mpFogCache != NULL) {
        return &mpFogCache[fog.GetID()];
    }

    fog.GetAnmResult(pResult, GetFrm());
    return pResult;
}

CameraAnmResult* AnmScnRes::GetCameraResult(CameraAnmResult* pResult,
                                            u32 idx) {
    ResAnmCamera camera = mRes.GetResAnmCameraByRefNumber(idx);

    if (!camera.IsValid()) {
        pResult->flags = 0;
        return pResult;
    }

    if (mpCameraCache != NULL) {
        return &mpCameraCache[camera.GetID()];
    }

    camera.GetAnmResult(pResult, GetFrm());
    return pResult;
}

} // namespace g3d
} // namespace nw4r


namespace nw4r {
namespace g3d {

u32 AnmScnRes::GetSpecularLightID(u32 idx) const {
    ResAnmLight light = mRes.GetResAnmLightByRefNumber(idx);

    if (light.IsValid()) {
        return light.ref().specLightObjIdx;
    }

    return -1;
}

bool AnmScnRes::HasSpecularLight(u32 idx) const {
    ResAnmLight light = mRes.GetResAnmLightByRefNumber(idx);
    bool result = false;

    if (light.IsValid() &&
        (light.ref().flags & ResAnmLightData::FLAG_SPECULAR_ENABLE)) {
        result = true;
    }

    return result;
}

} // namespace g3d
} // namespace nw4r



namespace nw4r {
namespace g3d {

void AnmScnRes::UpdateCache() {
    u32 numAmb = mRes.GetResAnmAmbLightNumEntries();
    u32 numLight = mRes.GetResAnmLightNumEntries();
    u32 numFog = mRes.GetResAnmFogNumEntries();
    u32 numCamera = mRes.GetResAnmCameraNumEntries();
    f32 frame = GetFrm();

    for (u32 i = 0; i < numAmb; i++) {
        mRes.GetResAnmAmbLight(i).GetAnmResult(&mpAmbLightCache[i], frame);
    }

    for (u32 i = 0; i < numLight; i++) {
        mRes.GetResAnmLight(i).GetAnmResult(&mpLightCache[i], frame);
    }

    for (u32 i = 0; i < numFog; i++) {
        mRes.GetResAnmFog(i).GetAnmResult(&mpFogCache[i], frame);
    }

    for (u32 i = 0; i < numCamera; i++) {
        mRes.GetResAnmCamera(i).GetAnmResult(&mpCameraCache[i], frame);
    }
}

} // namespace g3d
} // namespace nw4r


void IsDerivedFrom__Q34nw4r3g3d9AnmScnResCFQ44nw4r3g3d6G3dObj7TypeObj(){}
void IsDerivedFrom__Q34nw4r3g3d6AnmScnCFQ44nw4r3g3d6G3dObj7TypeObj(){}
void GetTypeName__Q34nw4r3g3d9AnmScnResCFv(){}
extern const char lbl_eu_8051D630[];

const void* GetTypeObj__Q34nw4r3g3d9AnmScnResCFv() {
    return lbl_eu_8051D630;
}
void GetTypeName__Q34nw4r3g3d6AnmScnCFv(){}
extern "C" void* GetTypeObj__Q34nw4r3g3d6AnmScnCFv(void) { return (void*)lbl_eu_8051D620; }
