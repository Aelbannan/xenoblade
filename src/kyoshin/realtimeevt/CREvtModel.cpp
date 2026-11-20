// TU: kyoshin/realtimeevt/CREvtModel
// Class: cf::CREvtModel (derives from cf::CREvtObj, kind 3) - base of the
// realtime-event model objects (map / object / pc models).
//
// Emits the CREvtModel vtable (0x8053167C) and base list (0x805316C0), and,
// through the dynamic_cast in setupShadow, the weak type_info of
// CScnMaruShadowNw4r / IScnPolyShadow (base list 0x80531670). vfunc_0C and
// vfunc_24 are defined in CTaskREvtSequence.cpp (retail text order).

#include "kyoshin/realtimeevt/CREvtModel.hpp"
extern "C" void simSetFlag2OnTree(void* model, int flag);
#include "kyoshin/realtimeevt/CREvtModelMap.hpp"
#include "kyoshin/realtimeevt/CREvtModelObj.hpp"
#include "kyoshin/realtimeevt/CREvtModelPc.hpp"
#include "libs/monolib/src/scn/CScnItemModel.hpp"
#include "libs/monolib/src/scn/IScnPolyShadow.hpp"
#include <nw4r/g3d/res/g3d_resfile.h>
#include <nw4r/g3d/res/g3d_resanmchr.h>
#include <nw4r/g3d/res/g3d_resdict.h>
#include "kyoshin/cf/CfGameManagerData.hpp"  // H3 label-owner decl (gmScene; gmGameFlags)
#include "kyoshin/cf/CTaskREvtSequence.hpp"
namespace nw4r { namespace db { void Panic(const char* file, int line, const char* fmt, ...); } }

extern "C" {
extern bool Scn_IsAnimActiveOrNull(void* ptr);
extern void* Scn_InitGlobalA(void* global, void* data, int param);


extern u32  REvtCam_IsField98Set(void);
extern void ScnEnvLgt_EnableFlag100(void* ptr, int flag);

extern void simMarkTree100000(void* model, int flag);

extern void simMarkTree80000(void* model, int flag);
extern "C" void simSetValue7E8(void* model, int value);

extern void* GetResAnmChr__Q34nw4r3g3d7ResFileCFi(void* resFile, int index);
extern void* GetResUserData__Q34nw4r3g3d9ResAnmChrFv(void* self);
extern void* __vc__Q34nw4r3g3d6ResDicCFPCc(void* self, const char* key);
// nw4r panic file/format strings (shared .data pool of another unit).
}

// Event-data block
// +0x58, four toggle frame numbers at +0x6C.
struct CREvtModelData {
    /* 0x00 */ u8 _pad00[0x10];
    /* 0x10 */ char mName[0x58 - 0x10];
    /* 0x58 */ u32 mFlags;
    /* 0x5C */ u8 _pad5C[0x6C - 0x5C];
    /* 0x6C */ u16 mToggleFrame[4];
};

// Model internals touched here (CScnItemModel is a partial view).
struct CREvtItemModelView {
    /* 0x000 */ u8 _pad000[0x7A0];
    /* 0x7A0 */ u32 mFlags7A0;
    /* 0x7A4 */ u32 mFlags7A4;
};

namespace cf {

CREvtModel::CREvtModel(void* pData, int kind)
    : CREvtObj(3), mField14(kind), mFlags(0), mPtr1C(pData), mModel(nullptr), mData(nullptr),
      mPtr28(nullptr), mField2C(0), mField30(0), mField34(0) {
    setFlag(0x8);
}

CREvtModel::~CREvtModel() {
    reset();
}

int CREvtModel::reset() {
    if (mFlags & 0x80) {
        return 1;
    }
    vfunc_08();
    if (mData != nullptr) {
        Scn_IsAnimActiveOrNull(mData);
        mData = nullptr;
    }
    if (mModel != nullptr) {
        Scn_IsAnimActiveOrNull(mModel);
        mModel = nullptr;
    }
    mFlags |= 0x80;
    return 1;
}

void CREvtModel::releaseAnims() {
    vfunc_08();
    if (mData != nullptr) {
        Scn_IsAnimActiveOrNull(mData);
        mData = nullptr;
    }
    if (mModel != nullptr) {
        Scn_IsAnimActiveOrNull(mModel);
        mModel = nullptr;
    }
}

void* CREvtModel::evtModelDataPtr() {
    return static_cast<CREvtModelData*>(mPtr1C)->mName;
}

void CREvtModel::vfunc_1C() {
    CREvtObj::vfunc_1C();
    if (mField34 < 4) {
        CREvtModelData* data = static_cast<CREvtModelData*>(mPtr1C);
        if (data->mToggleFrame[mField34] != 0 &&
            EvtSeqGetCounter104() == static_cast<CREvtModelData*>(mPtr1C)->mToggleFrame[mField34]) {
            if (!isFlag(0x400)) {
                setFlag(0x400);
            } else {
                clrFlag(0x400);
            }
            mField34++;
        }
    }
    if (mModel != nullptr) {
        ScnEnvLgt_EnableFlag100((char*)mModel + 0x31C, (mFlags >> 10) & 1);
        if (REvtCam_IsField98Set() == 0) {
            setVisible(mField2C == 1);
        } else {
            setVisible(mField2C != 2);
        }
    }
}

void CREvtModel::vfunc_04() {
    if (mModel != nullptr) {
        Vec3 pos;
        pos.a[0] = 0.0f;
        pos.a[1] = 0.0f;
        pos.a[2] = 0.0f;
        ((CREvtModelPose*)simGetLeafActData(mModel))->translate = pos;
        Vec3 rot;
        rot.a[0] = 0.0f;
        rot.a[1] = 0.0f;
        rot.a[2] = 0.0f;
        ((CREvtModelPose*)simGetLeafActData(mModel))->rotate = rot;
    }
}

void CREvtModel::vfunc_08() {
    if (mPtr28 != nullptr) {
        Scn_IsAnimActiveOrNull(mPtr28);
        mPtr28 = nullptr;
    }
}

int CREvtModel::isEvtDataFlagSet() {
    CREvtModelData* data = static_cast<CREvtModelData*>(mPtr1C);
    if (data == nullptr) { return 0; }
    return (data->mFlags >> 4) & 1;
}

void CREvtModel::setEventData(void* pData, void* pModelData) {
    u32 flags = *(u32*)((char*)pModelData + 4);
    onEvent(flags & 1);
    CScnItemModel* model = mModel;
    if (model == nullptr) return;
    simSetFlag2OnTree(model, *(u32*)((char*)pModelData + 4) & 1);
    void* anim = mData;
    if (anim != nullptr) {
        vfunc_08();
        if (REvtCam_GetFieldA5Flag() == 0) { applyHideParts(anim, model, 1); }
        mPtr28 = anim;
        mData = nullptr;
    }
    if (pData == nullptr) {
        if (!(static_cast<CREvtModelData*>(mPtr1C)->mFlags & 0x8)) {
            mModel->vfunc84(1);
        }
        return;
    }
    void* newData = Scn_InitGlobalA(&gmScene, pData, 0);
    mData = newData;
    simMarkTree100000(model, REvtCam_GetFieldA5Flag() == 0 ? 1 : 0);
simBindChrAnim(model, newData, 0, 0, 0, 1, -1);
    setupShadow(model);
    model->vfunc84(0);
    simSetValue7E8(model, 1);
    reinterpret_cast<CREvtItemModelView*>(model)->mFlags7A4 |= 0x20000000;
    mField2C = 0;
    mField30 = 0;
    { int tmp = 0; EvtSeqResolveAnmChr((u8*)newData, "Monochrome", (s32*)&tmp); mField2C = tmp; }
    { int tmp = 0; EvtSeqResolveAnmChr((u8*)newData, "MonoRGB", (s32*)&tmp); mField30 = tmp; }
    if (REvtCam_IsField98Set() == 0) {
        setVisible((mField2C - 1) == 0 ? 1 : 0);
    } else {
        setVisible((mField2C != 2) ? 1 : 0);
    }
    int sv = 0; EvtSeqResolveAnmChr((u8*)newData, "noDynamics", (s32*)&sv);
    if (sv >= 1) { simMarkTree80000(model, 1); }
    int sv2 = 1; EvtSeqResolveAnmChr((u8*)newData, "LgtID", (s32*)&sv2);
    if (sv2 != 0) { reinterpret_cast<CREvtItemModelView*>(model)->mFlags7A0 |= 0x8; }
    else { reinterpret_cast<CREvtItemModelView*>(model)->mFlags7A0 &= ~0x8; }
    if (REvtCam_GetFieldA5Flag() == 0) { applyHideParts(newData, model, 0); }
    int result = EvtSeqCheckRegionStatus((u8*)newData);
    model->vfunc84(result);
}

void CREvtModel::onEvent(u32 visible) {}

void CREvtModel::setVisible(int visible) {
    if (mModel != nullptr) { mModel->vfuncB4(visible); }
}

void CREvtModel::setupShadow(CScnItemModel* pModel) {
    int type = 0;
    f32 radius = 0.9f;
    f32 pow = 0.35f;
    EvtSeqResolveAnmChr((u8*)mData, "SdwType", (s32*)&type);
    if (type > 0) {
        type--;
        EvtSeqFindResFloat((u8*)mData, "SdwRadius", &radius);
        EvtSeqFindResFloat((u8*)mData, "SdwPow", &pow);
    } else { type = 1; }
    pModel->vfunc50(type == 0 ? 0 : 1);
    CScnMaruShadowNw4r* shadow =
        dynamic_cast<CScnMaruShadowNw4r*>(reinterpret_cast<IScnPolyShadow*>(pModel->field_7EC));
    if (shadow != nullptr) {
        if (type == 1) { shadow->mMono = 1; }
        else if (type == 2) { shadow->mMono = 0; }
        shadow->mColorR = 0; shadow->mColorG = 0;
        shadow->mColorB = 0; shadow->mColorA = (u8)(int)(255.0f * pow);
        shadow->mRadiusScale = radius;
    }
}

void CREvtModel::applyHideParts(void* pAnim, CScnItemModel* pModel, int flag) {
    nw4r::g3d::ResFile resFile((void*)((char*)pAnim + 0xC));
    int numAnmChr = resFile.GetResAnmChrNumEntries();
    for (int i = 0; i < numAnmChr; i++) {
        void* anmChrData = GetResAnmChr__Q34nw4r3g3d7ResFileCFi((char*)pAnim + 0xC, i);
        nw4r::g3d::ResAnmChr anmChr(anmChrData);
        void* userData = GetResUserData__Q34nw4r3g3d9ResAnmChrFv(&anmChr);
        if (userData == 0) continue;
        void* dic = (char*)userData + 4;
        if ((u32)dic & 3) { nw4r::db::Panic(lbl_eu_80530F08, 0x54, lbl_eu_80530EE0); }
        void* result = __vc__Q34nw4r3g3d6ResDicCFPCc(dic, "HidePT");
        if ((u32)result & 3) { nw4r::db::Panic(lbl_eu_80530D54, 0x26, lbl_eu_80530D2C); }
        if (result == 0) continue;
        if (*(u32*)((char*)result + 0xC) != 2) { nw4r::db::Panic(lbl_eu_80530ECC, 0x44, lbl_eu_80530E88); }
        void* dataPtr = 0;
        if (*(u32*)((char*)result + 4) != 0) { dataPtr = (char*)result + *(u32*)((char*)result + 4); }
        if (dataPtr != 0) {
            pModel->vfunc28((u32)dataPtr, flag);
        }
    }
    if (flag == 0) {
        if (static_cast<CREvtModelData*>(mPtr1C)->mFlags & 0x100) {
            pModel->vfunc28((u32)"core", 0);
        }
    }
}

// Factory used by CTaskREvtSequence: kind 1 = map model, kind 2 = pc model
// (unless the name is "00"), otherwise an object model.
CREvtModel* CREvtModel::create(void* pData) {
    u32 type = *(u32*)((char*)pData + 0x28);
    if (type == 1) {
        return new CREvtModelMap(pData);
    } else if (type == 2) {
        if (*((s8*)pData + 0x16) != 0x30 || *((s8*)pData + 0x17) != 0x30) {
            return new CREvtModelPc(pData);
        }
    }
    return new CREvtModelObj(pData);
}

int CREvtModel::vfunc_14() {
    return mFlags & 1;
}

int CREvtModel::vfunc_10() {
    return (mFlags >> 4) & 1;
}

int CREvtModel::vfunc_20() {
    return 0;
}

} // namespace cf


