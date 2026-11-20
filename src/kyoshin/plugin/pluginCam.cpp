#include "kyoshin/plugin/pluginGame.hpp"
#include "kyoshin/cf/CfCamEventApi.hpp"
#include "kyoshin/cf/CfMapEffectManager.hpp"
#include "kyoshin/plugin/pluginCam.hpp"
#include "monolib/vm/yvm2.h"
#include "monolib/math.hpp"
#include "kyoshin/code_801862C0.hpp"
#include "monolib/core.hpp"

// Camera manager driven by the cam plugin VM funcs (retail object behind
// getCameraDataBlock). Each used slot is named after its caller: 0x14 set
// camera position, 0x3C set fov, 0x4C set direction, 0x50/0x54 set rotation,
// 0x58 get fov float
// position/look-at offsets. Same slots as CREvtCamManager in
// realtimeevt/CREvtCamera.hpp.
class UnkCamIntf {
public:
    virtual void v08();
    virtual void v0C();
    virtual void v10();
    virtual void setPosition(void* pos);  // 0x14
    virtual void v18();
    virtual void v1C();
    virtual void v20();
    virtual void v24();
    virtual void v28();
    virtual void v2C();
    virtual void v30();
    virtual void v34();
    virtual void v38();
    virtual void setFov(f32 fov);  // 0x3C
    virtual void v40();
    virtual void v44();
    virtual void v48();
    virtual void setDirection(void* dir) {}  // 0x4C
    virtual void setRotationX(f32 rotX) {}  // 0x50
    virtual void setRotationY(f32 rotY) {}  // 0x54
    virtual f32 getFov();  // 0x58
    virtual void setTarget(u32 targetId);  // 0x5C
    virtual void v60();
    virtual void setLookAt(void* lookat) {}  // 0x64
    virtual void setPositionOfs(void* obj, void* pos, int flags) {}  // 0x68
    virtual void setLookAtOfs(void* obj, void* lookat, int flags) {}  // 0x6C
    u8 field_0x04[0x0C - 0x04];
    CamStateObj* objPtr;
};

extern "C" {
UnkCamIntf* getCameraDataBlock__Q22cf13CfGameManagerFv();
void cleanupMapEffects__Q22cf13CfGameManagerFv();
void notifyCameraManager__Q22cf13CfGameManagerFv();
int isCameraReady__Q22cf13CfGameManagerFv();
void getCamManagerData__Q22cf13CfGameManagerFv(u32 mode);
void lookupEffectForResource__Q22cf13CfGameManagerFv(u32 third, u32 fourth, u32 seventh);
u32 notifyBattleSystem__Q22cf13CfGameManagerFv(u32 first, u32 second, u32 third,
                                          u32 fourth, u32 fifth);
void isEffectReady__Q22cf13CfGameManagerFv(u32 first, ml::CVec3* pos,
                                           ml::CVec3* lookat, u32 extra, f32 seventh);
}

static inline f32 fixedToFloat(s32 fixed) {
    return (f32)fixed / 4096.0f;
}

static inline s32 floatToFixed(f32 val) {
    return (s32)(4096.0f * val);
}

static inline f32 degToRad(s32 fixedDeg) {
    return fixedToFloat(fixedDeg) * ml::deg2rad;
}

static inline s32 radToDegFixed(f32 rad) {
    return (s32)(4096.0f * (rad * ml::rad2deg));
}

extern "C" int select(VMThread* pThread) {
    s32 mode = vmArgIntGet(2, vmArgPtrGet(pThread, 1));
    getCamManagerData__Q22cf13CfGameManagerFv(mode);
    return 0;
}

extern "C" int restore(VMThread* pThread) {
    s32 cameraId;
    s32 transitionTime;
    s32 nextArg = 1;

    if (vmArgOmitChk(pThread, 1)) {
        cameraId = 0;
        nextArg++;
    } else {
        // Post-increment: MWCC folds the pre-increment value (1) to an
        // immediate and emits the r30 update just before the call.
        VMArg* arg = vmArgPtrGet(pThread, nextArg++);
        cameraId = vmArgIntGet(nextArg, arg);
    }

    if (vmArgOmitChk(pThread, nextArg)) {
        transitionTime = 0;
    } else {
        // Arg 3: optional; ptr index is nextArg (2), int value index nextArg+1 (3).
        // Post-increment keeps the side effect observable (retail stores r30 back).
        VMArg* arg = vmArgPtrGet(pThread, nextArg++);
        transitionTime = vmArgIntGet(nextArg, arg);
    }

    // Retinal bool args: neg/or/srwi booleanize of each value (nonzero -> 1).
    lookupEffectForResource__Q22cf13CfGameManagerFv(
        (u32)(-cameraId | cameraId) >> 31,
        (u32)(-transitionTime | transitionTime) >> 31,
        0);
    return 0;
}

extern "C" int setPos(VMThread* pThread) {
    s32 fixedX = vmArgFixedGet(2, vmArgPtrGet(pThread, 1));
    s32 fixedY = vmArgFixedGet(3, vmArgPtrGet(pThread, 2));
    s32 fixedZ = vmArgFixedGet(4, vmArgPtrGet(pThread, 3));

    ml::CVec3 pos;
    pos.x = (f32)fixedX / 4096.0f;
    pos.y = (f32)fixedY / 4096.0f;
    pos.z = (f32)fixedZ / 4096.0f;

    UnkCamIntf* cam = getCameraDataBlock__Q22cf13CfGameManagerFv();
    cam->setPosition(&pos);
    cleanupMapEffects__Q22cf13CfGameManagerFv();
    return 0;
}

extern "C" int setLookat(VMThread* pThread) {
    s32 fixedX = vmArgFixedGet(2, vmArgPtrGet(pThread, 1));
    s32 fixedY = vmArgFixedGet(3, vmArgPtrGet(pThread, 2));
    s32 fixedZ = vmArgFixedGet(4, vmArgPtrGet(pThread, 3));

    ml::CVec3 lookat;
    lookat.x = (f32)fixedX / 4096.0f;
    lookat.y = (f32)fixedY / 4096.0f;
    lookat.z = (f32)fixedZ / 4096.0f;

    UnkCamIntf* cam = getCameraDataBlock__Q22cf13CfGameManagerFv();
    cam->setLookAt(&lookat);
    cf::CfMapEffectManager::pushFlash(0.0f, 1.0f);
    return 0;
}

extern "C" int setDir(VMThread* pThread) {
    s32 fixedAngle = vmArgFixedGet(2, vmArgPtrGet(pThread, 1));
    s32 fixedDist = vmArgFixedGet(3, vmArgPtrGet(pThread, 2));

    ml::CVec3 dir;
    dir.x = (f32)fixedAngle / 4096.0f * ml::deg2rad;
    dir.y = (f32)fixedDist / 4096.0f * ml::deg2rad;
    dir.z = 0.0f;

    UnkCamIntf* cam = getCameraDataBlock__Q22cf13CfGameManagerFv();
    cam->setDirection(&dir);
    cleanupMapEffects__Q22cf13CfGameManagerFv();
    return 0;
}

extern "C" int setRotX(VMThread* pThread) {
    s32 fixedAngle = vmArgFixedGet(2, vmArgPtrGet(pThread, 1));
    f32 value = fixedToFloat(fixedAngle);
    f32 rotX = value * ml::deg2rad;

    UnkCamIntf* cam = getCameraDataBlock__Q22cf13CfGameManagerFv();
    cam->setRotationX(rotX);
    return 0;
}

extern "C" int setRotY(VMThread* pThread) {
    s32 fixedAngle = vmArgFixedGet(2, vmArgPtrGet(pThread, 1));
    f32 value = fixedToFloat(fixedAngle);
    f32 rotY = value * ml::deg2rad;

    UnkCamIntf* cam = getCameraDataBlock__Q22cf13CfGameManagerFv();
    cam->setRotationY(rotY);
    return 0;
}

extern "C" int setFov(VMThread* pThread) {
    s32 fixedFov = vmArgFixedGet(2, vmArgPtrGet(pThread, 1));

    UnkCamIntf* cam = getCameraDataBlock__Q22cf13CfGameManagerFv();
    cam->setFov(fixedToFloat(fixedFov));
    return 0;
}

extern "C" int setTarget(VMThread* pThread) {
    CamOCHandle* oc = (CamOCHandle*)vmArgOCGet(2, vmArgPtrGet(pThread, 1));

    UnkCamIntf* cam = getCameraDataBlock__Q22cf13CfGameManagerFv();
    cam->setTarget(oc->field_0x04);
    cleanupMapEffects__Q22cf13CfGameManagerFv();
    return 0;
}

extern "C" int setPosOfs(VMThread* pThread) {
    CamOCHandle* oc = (CamOCHandle*)vmArgOCGet(2, vmArgPtrGet(pThread, 1));
    s32 fixedX = vmArgFixedGet(3, vmArgPtrGet(pThread, 2));
    s32 fixedY = vmArgFixedGet(4, vmArgPtrGet(pThread, 3));
    s32 fixedZ = vmArgFixedGet(5, vmArgPtrGet(pThread, 4));

    s32 flags;
    if (vmArgOmitChk(pThread, 5)) {
        flags = 0;
    } else {
        flags = vmArgIntGet(6, vmArgPtrGet(pThread, 5));
    }

    ml::CVec3 pos;
    pos.x = (f32)fixedX / 4096.0f;
    pos.y = (f32)fixedY / 4096.0f;
    pos.z = (f32)fixedZ / 4096.0f;

    UnkCamIntf* cam = getCameraDataBlock__Q22cf13CfGameManagerFv();
    void* obj = ArtsSelect_GetContainer();
    void* slot = ArtsSelect_Lookup(obj, oc->field_0x04);
    cam->setPositionOfs(slot, &pos, -flags);
    return 0;
}

extern "C" int setLookatOfs(VMThread* pThread) {
    CamOCHandle* oc = (CamOCHandle*)vmArgOCGet(2, vmArgPtrGet(pThread, 1));
    s32 fixedX = vmArgFixedGet(3, vmArgPtrGet(pThread, 2));
    s32 fixedY = vmArgFixedGet(4, vmArgPtrGet(pThread, 3));
    s32 fixedZ = vmArgFixedGet(5, vmArgPtrGet(pThread, 4));

    s32 flags;
    if (vmArgOmitChk(pThread, 5)) {
        flags = 0;
    } else {
        flags = vmArgIntGet(6, vmArgPtrGet(pThread, 5));
    }

    ml::CVec3 lookat;
    lookat.x = (f32)fixedX / 4096.0f;
    lookat.y = (f32)fixedY / 4096.0f;
    lookat.z = (f32)fixedZ / 4096.0f;

    UnkCamIntf* cam = getCameraDataBlock__Q22cf13CfGameManagerFv();
    void* obj = ArtsSelect_GetContainer();
    void* slot = ArtsSelect_Lookup(obj, oc->field_0x04);
    cam->setLookAtOfs(slot, &lookat, -flags);
    cf::CfMapEffectManager::pushFlash(0.0f, 1.0f);
    return 0;
}

extern "C" int getPos(VMThread* pThread) {
    s32 index = vmArgIntGet(2, vmArgPtrGet(pThread, 1));

    UnkCamIntf* cam = getCameraDataBlock__Q22cf13CfGameManagerFv();
    CamStateObj* camObj = cam->objPtr;

    VMArg ret;

    if (index == 0) {
        ret.type = VM_TYPE_FIXED;
        ret.value.intVal = floatToFixed(camObj->field_0x10C);
    } else if (index == 1) {
        ret.type = VM_TYPE_FIXED;
        ret.value.intVal = floatToFixed(camObj->field_0x110);
    } else if (index == 2) {
        ret.type = VM_TYPE_FIXED;
        ret.value.intVal = floatToFixed(camObj->field_0x114);
    }

    vmRetValSet(pThread, &ret);
    return 1;
}

extern "C" int getRot(VMThread* pThread) {
    s32 index = vmArgIntGet(2, vmArgPtrGet(pThread, 1));

    UnkCamIntf* cam = getCameraDataBlock__Q22cf13CfGameManagerFv();
    CamStateObj* camObj = cam->objPtr;

    VMArg ret;

    if (index == 0) {
        ret.type = VM_TYPE_FIXED;
        ret.value.intVal = radToDegFixed(camObj->field_0x118);
    } else if (index == 1) {
        ret.type = VM_TYPE_FIXED;
        ret.value.intVal = radToDegFixed(camObj->field_0x11C);
    } else if (index == 2) {
        ret.type = VM_TYPE_FIXED;
        ret.value.intVal = radToDegFixed(camObj->field_0x120);
    }

    vmRetValSet(pThread, &ret);
    return 1;
}

extern "C" int keyBegin(VMThread* pThread) {
    // Optional-arg chain state: declared first so MWCC assigns the callee-saved
    // registers the same way as retail (nextArg=r31, third=r30, fourth=r29,
    // first=r28, second=r27, pThread=r26).
    s32 nextArg = 3;
    s32 third;
    s32 fourth;
    VMArg* arg;
    s32 first = vmArgIntGet(2, vmArgPtrGet(pThread, 1));
    s32 second = vmArgIntGet(3, vmArgPtrGet(pThread, 2));

    if (vmArgOmitChk(pThread, nextArg)) {
        third = 0;
        nextArg = 4;
    } else {
        arg = vmArgPtrGet(pThread, nextArg++);
        third = vmArgIntGet(nextArg, arg);
    }

    if (vmArgOmitChk(pThread, nextArg)) {
        fourth = 0;
        nextArg++;
    } else {
        arg = vmArgPtrGet(pThread, nextArg++);
        fourth = vmArgIntGet(nextArg, arg);
    }

    CamOCHandle* oc;
    if (vmArgOmitChk(pThread, nextArg)) {
        oc = nullptr;
    } else {
        arg = vmArgPtrGet(pThread, nextArg++);
        oc = (CamOCHandle*)vmArgOCGet(nextArg, arg);
    }

    // Resolve the OC instance to a camera key id (0 when omitted/invalid).
    u32 ocId = 0;
    if (oc != nullptr) {
        ocId = (u32)ArtsSelect_Lookup(ArtsSelect_GetContainer(), oc->field_0x04);
    }

    notifyBattleSystem__Q22cf13CfGameManagerFv(
        first, second, third, (u32)fourth & 0xFFFF, ocId);
    return 0;
}

extern "C" int keyEnd(VMThread* pThread) {
    notifyCameraManager__Q22cf13CfGameManagerFv();
    return 0;
}

extern "C" int keyAdd(VMThread* pThread) {
    // Callee-saved allocation follows declaration order descending from r31:
    // fixed6=r31, fixed5=r30, fixed4=r29, fixed3=r28, fixed2=r27, fixed1=r26,
    // first=r25, seventh=r24, nextArg=r23, pThread=r22.
    s32 extra;
    s32 fixed6;
    s32 fixed5;
    s32 fixed4;
    s32 fixed3;
    s32 fixed2;
    s32 fixed1;
    s32 first = vmArgIntGet(2, vmArgPtrGet(pThread, 1));
    fixed1 = vmArgFixedGet(3, vmArgPtrGet(pThread, 2));
    fixed2 = vmArgFixedGet(4, vmArgPtrGet(pThread, 3));
    fixed3 = vmArgFixedGet(5, vmArgPtrGet(pThread, 4));
    fixed4 = vmArgFixedGet(6, vmArgPtrGet(pThread, 5));
    fixed5 = vmArgFixedGet(7, vmArgPtrGet(pThread, 6));
    fixed6 = vmArgFixedGet(8, vmArgPtrGet(pThread, 7));

    // Optional-arg chain: nextArg carries the running index (retail r23), so
    // the post-increment form reproduces the `li r4, N; li r23, N+1; bl` order.
    s32 seventh;
    s32 nextArg = 8;
    if (vmArgOmitChk(pThread, nextArg)) {
        seventh = 0;
        nextArg = 9;
    } else {
        VMArg* arg = vmArgPtrGet(pThread, nextArg++);
        seventh = vmArgFixedGet(nextArg, arg);
    }

    if (vmArgOmitChk(pThread, nextArg)) {
        extra = 0;
    } else {
        VMArg* arg = vmArgPtrGet(pThread, nextArg++);
        extra = vmArgIntGet(nextArg, arg);
    }
    u32 extraArg = (u32)extra & 0xFFFF;

    // Key-frame: pos (fixed1-3), lookat (fixed4-6) and an optional 7th
    // fixed value passed as the trailing float. lookat declared first so it
    // gets the lower stack slot (retail lookat@+8, pos@+0x14).
    struct {
        ml::CVec3 lookat;
        ml::CVec3 pos;
    } cam;
    cam.pos.x = (f32)fixed1 / 4096.0f;
    cam.pos.y = (f32)fixed2 / 4096.0f;
    cam.pos.z = (f32)fixed3 / 4096.0f;
    cam.lookat.x = (f32)fixed4 / 4096.0f;
    cam.lookat.y = (f32)fixed5 / 4096.0f;
    cam.lookat.z = (f32)fixed6 / 4096.0f;

    isEffectReady__Q22cf13CfGameManagerFv(
        (u32)first & 0xFFFF, &cam.pos, &cam.lookat, extraArg,
        (f32)seventh / 4096.0f);
    return 0;
}

extern "C" int isKeyMove(VMThread* pThread) {
    // Retail stores only the VMArg type byte: VM_TYPE_TRUE (1) when the key
    // is moving, VM_TYPE_FALSE (2) otherwise. `!x + 1` reproduces the
    // branchless cntlzw/srwi/addi idiom.
    VMArg ret;
    ret.type = !isCameraReady__Q22cf13CfGameManagerFv() + 1;
    vmRetValSet(pThread, &ret);
    return 1;
}

extern "C" int waitKeyMove(VMThread* pThread) {
    if (isCameraReady__Q22cf13CfGameManagerFv()) {
        vmWaitModeSet(pThread);
    }
    return 0;
}

extern "C" int shake(VMThread* pThread) {
    // Args 2..0xE: intensity (int), 11 fixed-point values (fixed1..11), then a
    // bool flag. The data block is 13 floats: data[0] is the raw int,
    // data[1..6]/data[8..12] are the fixed values /4096, and data[7] is 0.0f
    // (the gap between the two fixed groups). The bool is passed separately as
    // the second arg, booleanized via the neg/or/srwi idiom.
    s32 intParam = vmArgIntGet(2, vmArgPtrGet(pThread, 1));
    s32 fixed1 = vmArgFixedGet(3, vmArgPtrGet(pThread, 2));
    s32 fixed2 = vmArgFixedGet(4, vmArgPtrGet(pThread, 3));
    s32 fixed3 = vmArgFixedGet(5, vmArgPtrGet(pThread, 4));
    s32 fixed4 = vmArgFixedGet(6, vmArgPtrGet(pThread, 5));
    s32 fixed5 = vmArgFixedGet(7, vmArgPtrGet(pThread, 6));
    s32 fixed6 = vmArgFixedGet(8, vmArgPtrGet(pThread, 7));
    s32 fixed7 = vmArgFixedGet(9, vmArgPtrGet(pThread, 8));
    s32 fixed8 = vmArgFixedGet(10, vmArgPtrGet(pThread, 9));
    s32 fixed9 = vmArgFixedGet(11, vmArgPtrGet(pThread, 10));
    s32 fixed10 = vmArgFixedGet(12, vmArgPtrGet(pThread, 11));
    s32 fixed11 = vmArgFixedGet(13, vmArgPtrGet(pThread, 12));
    s32 boolParam = vmArgBoolGet(14, vmArgPtrGet(pThread, 13));

    f32 data[13];
    memset(data, 0, sizeof(data));

    data[0] = (f32)intParam;
    data[1] = (f32)fixed1 / 4096.0f;
    data[2] = (f32)fixed2 / 4096.0f;
    data[3] = (f32)fixed3 / 4096.0f;
    data[4] = (f32)fixed4 / 4096.0f;
    data[5] = (f32)fixed5 / 4096.0f;
    data[6] = (f32)fixed6 / 4096.0f;
    data[7] = 0.0f; // 0.0f
    data[8] = (f32)fixed7 / 4096.0f;
    data[9] = (f32)fixed8 / 4096.0f;
    data[10] = (f32)fixed9 / 4096.0f;
    data[11] = (f32)fixed10 / 4096.0f;
    data[12] = (f32)fixed11 / 4096.0f;

    CamEvtForwardShakeFlag((int)(uintptr_t)data, (u32)(-boolParam | boolParam) >> 31);
    return 0;
}

extern "C" int stopShake(VMThread* pThread) {
    CamEvtLookupStateVal(0);
    return 0;
}

// name-guess: 20-entry VM function dispatch table registered by
// pluginCamRegist; retail .data object 0x80526560.
static PluginFuncData sCamPluginFuncs[] = {
    { "select", select },
    { "restore", restore },
    { "setPos", setPos },
    { "setDir", setDir },
    { "setLookat", setLookat },
    { "setTarget", setTarget },
    { "setRotX", setRotX },
    { "setRotY", setRotY },
    { "setFov", setFov },
    { "getPos", getPos },
    { "getRot", getRot },
    { "setPosOfs", setPosOfs },
    { "setLookatOfs", setLookatOfs },
    { "keyBegin", keyBegin },
    { "keyEnd", keyEnd },
    { "keyAdd", keyAdd },
    { "isKeyMove", isKeyMove },
    { "waitKeyMove", waitKeyMove },
    { "shake", shake },
    { "stopShake", stopShake },
    { nullptr, nullptr },
};

void pluginCamRegist() {
    vmPluginRegist("cam", sCamPluginFuncs);
}
