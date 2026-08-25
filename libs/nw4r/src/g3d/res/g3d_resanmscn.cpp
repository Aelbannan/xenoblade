#include <nw4r/g3d.h>

namespace nw4r {
namespace g3d {
namespace {

NW4R_G3D_RESFILE_NAME_DEF(LightSet, "LightSet(NW4R)");
NW4R_G3D_RESFILE_NAME_DEF(AmbLights, "AmbLights(NW4R)");
NW4R_G3D_RESFILE_NAME_DEF(Lights, "Lights(NW4R)");
NW4R_G3D_RESFILE_NAME_DEF(Fog, "Fogs(NW4R)");
NW4R_G3D_RESFILE_NAME_DEF(Camera, "Cameras(NW4R)");

// Name passed first so the name temp is built before the dic temp.
// Each top-level lookup yields a nested resource dictionary.
inline ResDicData* GetDicData(const ResAnmScn& scn, ResName name) {
    return static_cast<ResDicData*>(
        ResDic(scn.ofs_to_obj<ResDic>(scn.ref().toScnTopLevelDic))[name]);
}

ResLightSet GetResLightSetImpl(const ResAnmScn& scn, int idx) {
    ResDicData* pDicData = GetDicData(scn, ResName(&ResNameData_LightSet));

    if (pDicData != NULL) {
        return ResLightSet(ResDic(pDicData)[idx]);
    }

    return ResLightSet(NULL);
}

ResAnmAmbLight GetResAnmAmbLightImpl(const ResAnmScn& scn, int idx) {
    ResDicData* pDicData =
        static_cast<ResDicData*>(ResDic(scn.ofs_to_obj<ResDic>(
            scn.ref().toScnTopLevelDic))[ResName(&ResNameData_AmbLights)]);

    if (pDicData != NULL) {
        return ResAnmAmbLight(ResDic(pDicData)[idx]);
    }

    return ResAnmAmbLight(NULL);
}

ResAnmLight GetResAnmLightImpl(const ResAnmScn& scn, int idx) {
    ResDicData* pDicData =
        static_cast<ResDicData*>(ResDic(scn.ofs_to_obj<ResDic>(
            scn.ref().toScnTopLevelDic))[ResName(&ResNameData_Lights)]);

    if (pDicData != NULL) {
        return ResAnmLight(ResDic(pDicData)[idx]);
    }

    return ResAnmLight(NULL);
}

ResAnmFog GetResAnmFogImpl(const ResAnmScn& scn, int idx) {
    ResDicData* pDicData =
        static_cast<ResDicData*>(ResDic(scn.ofs_to_obj<ResDic>(
            scn.ref().toScnTopLevelDic))[ResName(&ResNameData_Fog)]);

    if (pDicData != NULL) {
        return ResAnmFog(ResDic(pDicData)[idx]);
    }

    return ResAnmFog(NULL);
}

ResAnmCamera GetResAnmCameraImpl(const ResAnmScn& scn, int idx) {
    ResDicData* pDicData =
        static_cast<ResDicData*>(ResDic(scn.ofs_to_obj<ResDic>(
            scn.ref().toScnTopLevelDic))[ResName(&ResNameData_Camera)]);

    if (pDicData != NULL) {
        return ResAnmCamera(ResDic(pDicData)[idx]);
    }

    return ResAnmCamera(NULL);
}

} // namespace

bool ResAnmScn::HasResAnmAmbLight() const {
    return ResDic(ofs_to_obj<ResDic>(
               ref().toScnTopLevelDic))[ResName(&ResNameData_AmbLights)] !=
           NULL;
}

bool ResAnmScn::HasResAnmLight() const {
    return ResDic(ofs_to_obj<ResDic>(
               ref().toScnTopLevelDic))[ResName(&ResNameData_Lights)] != NULL;
}

u32 ResAnmScn::GetResLightSetNumEntries() const {
    ResDic dic(ofs_to_obj<ResDic>(ref().toScnTopLevelDic));
    ResDicData* pResLightSetDicData =
        static_cast<ResDicData*>(dic[ResName(&ResNameData_LightSet)]);

    if (pResLightSetDicData != NULL) {
        return ResDic(pResLightSetDicData).GetNumData();
    }

    return 0;
}

ResAnmAmbLight ResAnmScn::GetResAnmAmbLight(const ResName name) const {
    ResDic dic(ofs_to_obj<ResDic>(ref().toScnTopLevelDic));
    ResDicData* pResAnmAmbLightDicData =
        static_cast<ResDicData*>(dic[ResName(&ResNameData_AmbLights)]);

    if (pResAnmAmbLightDicData != NULL) {
        return ResAnmAmbLight(ResDic(pResAnmAmbLightDicData)[name]);
    }

    return ResAnmAmbLight(NULL);
}

ResAnmAmbLight ResAnmScn::GetResAnmAmbLight(u32 idx) const {
    return GetResAnmAmbLightImpl(*this, static_cast<int>(idx));
}

ResAnmLight ResAnmScn::GetResAnmLight(const ResName name) const {
    ResDicData* pResAnmLightDicData = static_cast<ResDicData*>(ResDic(
        ofs_to_obj<ResDic>(ref().toScnTopLevelDic))[ResName(&ResNameData_Lights)]);

    if (pResAnmLightDicData != NULL) {
        return ResAnmLight(ResDic(pResAnmLightDicData)[name]);
    }

    return ResAnmLight(NULL);
}

ResAnmLight ResAnmScn::GetResAnmLight(u32 idx) const {
    return GetResAnmLightImpl(*this, static_cast<int>(idx));
}

ResLightSet ResAnmScn::GetResLightSetByRefNumber(u32 refNumber) const {
    const ResAnmScnInfoData& rInfoData = ref().info;

    if (rInfoData.numResLightSetData <= refNumber) {
        return ResLightSet(NULL);
    }

    const ResLightSetData* pArray =
        ofs_to_ptr<ResLightSetData>(ref().toResLightSetDataArray);

    ResLightSetData* pTarget = const_cast<ResLightSetData*>(&pArray[refNumber]);

    return ResLightSet(pTarget->id < rInfoData.numResLightSetData ? pTarget
                                                                  : NULL);
}

ResAnmAmbLight ResAnmScn::GetResAnmAmbLightByRefNumber(u32 refNumber) const {
    const ResAnmScnInfoData& rInfoData = ref().info;

    if (rInfoData.numResAnmAmbLightData <= refNumber) {
        return ResAnmAmbLight(NULL);
    }

    const ResAnmAmbLightData* pArray =
        ofs_to_ptr<ResAnmAmbLightData>(ref().toResAnmAmbLightDataArray);

    ResAnmAmbLightData* pTarget =
        const_cast<ResAnmAmbLightData*>(&pArray[refNumber]);

    return ResAnmAmbLight(
        pTarget->id < rInfoData.numResAnmAmbLightData ? pTarget : NULL);
}

ResAnmLight ResAnmScn::GetResAnmLightByRefNumber(u32 refNumber) const {
    const ResAnmScnInfoData& rInfoData = ref().info;

    if (rInfoData.numResAnmLightData <= refNumber) {
        return ResAnmLight(NULL);
    }

    const ResAnmLightData* pArray =
        ofs_to_ptr<ResAnmLightData>(ref().toResAnmLightDataArray);

    ResAnmLightData* pTarget = const_cast<ResAnmLightData*>(&pArray[refNumber]);

    return ResAnmLight(pTarget->id < rInfoData.numResAnmLightData ? pTarget
                                                                  : NULL);
}

ResAnmFog ResAnmScn::GetResAnmFogByRefNumber(u32 refNumber) const {
    const ResAnmScnInfoData& rInfoData = ref().info;

    if (rInfoData.numResAnmFogData <= refNumber) {
        return ResAnmFog(NULL);
    }

    const ResAnmFogData* pArray =
        ofs_to_ptr<ResAnmFogData>(ref().toResAnmFogDataArray);

    ResAnmFogData* pTarget = const_cast<ResAnmFogData*>(&pArray[refNumber]);

    return ResAnmFog(pTarget->id < rInfoData.numResAnmFogData ? pTarget : NULL);
}

ResAnmCamera ResAnmScn::GetResAnmCameraByRefNumber(u32 refNumber) const {
    const ResAnmScnInfoData& rInfoData = ref().info;

    if (rInfoData.numResAnmCameraData <= refNumber) {
        return ResAnmCamera(NULL);
    }

    const ResAnmCameraData* pArray =
        ofs_to_ptr<ResAnmCameraData>(ref().toResAnmCameraDataArray);

    ResAnmCameraData* pTarget =
        const_cast<ResAnmCameraData*>(&pArray[refNumber]);

    return ResAnmCamera(pTarget->id < rInfoData.numResAnmCameraData ? pTarget
                                                                    : NULL);
}

bool ResAnmScn::Bind(const ResAnmScn scene) {
    u32 lightSetNum = GetResLightSetNumEntries();
    bool success = true;

    for (u32 i = 0; i < lightSetNum; i++) {
        ResLightSet set = GetResLightSetImpl(*this, static_cast<int>(i));
        success = set.Bind(scene) && success;
    }

    return success;
}

} // namespace g3d
} // namespace nw4r

namespace nw4r {
namespace g3d {

u32 ResAnmScn::GetResAnmAmbLightNumEntries() const {
    ResDicData* pDicData = static_cast<ResDicData*>(ResDic(
        ofs_to_obj<ResDic>(
            ref().toScnTopLevelDic))[ResName(&ResNameData_AmbLights)]);

    if (pDicData != NULL) {
        return ResDic(pDicData).GetNumData();
    }

    return 0;
}

u32 ResAnmScn::GetResAnmLightNumEntries() const {
    ResDicData* pDicData = static_cast<ResDicData*>(ResDic(
        ofs_to_obj<ResDic>(
            ref().toScnTopLevelDic))[ResName(&ResNameData_Lights)]);

    if (pDicData != NULL) {
        return ResDic(pDicData).GetNumData();
    }

    return 0;
}

ResAnmFog ResAnmScn::GetResAnmFog(u32 idx) const {
    return GetResAnmFogImpl(*this, static_cast<int>(idx));
}

u32 ResAnmScn::GetResAnmFogNumEntries() const {
    ResDicData* pDicData = static_cast<ResDicData*>(ResDic(
        ofs_to_obj<ResDic>(ref().toScnTopLevelDic))[ResName(&ResNameData_Fog)]);

    if (pDicData != NULL) {
        return ResDic(pDicData).GetNumData();
    }

    return 0;
}

ResAnmCamera ResAnmScn::GetResAnmCamera(u32 idx) const {
    return GetResAnmCameraImpl(*this, static_cast<int>(idx));
}

u32 ResAnmScn::GetResAnmCameraNumEntries() const {
    ResDicData* pDicData = static_cast<ResDicData*>(ResDic(ofs_to_obj<ResDic>(
        ref().toScnTopLevelDic))[ResName(&ResNameData_Camera)]);

    if (pDicData != NULL) {
        return ResDic(pDicData).GetNumData();
    }

    return 0;
}

} // namespace g3d
} // namespace nw4r
